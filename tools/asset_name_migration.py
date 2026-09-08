"""Transactional, byte-preserving migration of known asset identities."""
import argparse
import os
from pathlib import Path
import re
import shutil
import sys
import tempfile

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.course_surface_data_common import load_yaml


def registry(root):
    entries = load_yaml(root/'tools/asset_names.yaml')['renames']
    for field in ('old','name','rom_start'):
        if len({e[field] for e in entries}) != len(entries):
            raise ValueError(f'duplicate asset naming registry {field}')
    return entries


def rewriters(entries):
    names = {e['old']:e['name'] for e in entries}
    tokens = re.compile(r'(?<![A-Za-z0-9_])('+'|'.join(map(re.escape,sorted(names,key=len,reverse=True)))+r')(?=$|[^A-Za-z0-9])')
    components = dict(names)
    for old,new in names.items():
        components[old.lstrip('_')] = new
        components[old.lstrip('_').lower()] = new.lower()
    ordered = sorted(components,key=len,reverse=True)
    def text(value):
        return tokens.sub(lambda m:names[m[0]],value)
    def reference(value):
        pieces = value.split('/')
        for i,piece in enumerate(pieces):
            for old in ordered:
                if piece == old or piece.startswith((old+'_',old+'.')):
                    pieces[i] = components[old]+piece[len(old):]
                    break
        return text('/'.join(pieces))
    def document(value):
        if isinstance(value,dict):
            return {reference(k) if isinstance(k,str) else k:
                    v if k in ('data','compression') else document(v) for k,v in value.items()}
        if isinstance(value,list):
            return [document(v) for v in value]
        return reference(value) if isinstance(value,str) else value
    return text, reference, document


def migrate_names(root):
    from tools.asset_bundles import LAYOUT, dump, pack_segment, write_makefile
    if not (root/LAYOUT).exists():
        return
    entries = registry(root)
    layout = load_yaml(root/LAYOUT)
    by_start = {r['rom_start']:r for r in layout['segments']}
    pending = []
    for entry in entries:
        row = by_start.get(entry['rom_start'])
        if row is None or row['name'] not in (entry['old'],entry['name']):
            raise ValueError(f"asset identity differs at ROM {entry['rom_start']:#x}")
        if row['name']==entry['old']:
            pending.append(entry)
    if not pending:
        return
    baseline = {r['rom_start']:pack_segment(root,r) for r in layout['segments']}
    rewrite_text,rewrite_path,rewrite_document = rewriters(entries)
    targets = {}
    for source in sorted((root/'assets').rglob('*')):
        if not source.is_file() or source == root/'assets/layout/bundles.mk':
            continue
        target = Path(rewrite_path(str(source.relative_to(root))))
        if target in targets:
            raise ValueError(f'conflicting asset rename destination: {target}; original sources untouched')
        targets[target] = source
    (root/'build').mkdir(exist_ok=True)
    stage = Path(tempfile.mkdtemp(prefix='asset-names-',dir=root/'build'))
    for target,source in targets.items():
        dest = stage/target
        dest.parent.mkdir(parents=True,exist_ok=True)
        if source.suffix=='.yaml':
            dest.write_bytes(dump(rewrite_document(load_yaml(source))))
        elif source.suffix=='.c':
            dest.write_text(rewrite_text(source.read_text()))
        else:
            shutil.copy2(source,dest)
    migrated = load_yaml(stage/LAYOUT)
    for row in migrated['segments']:
        if pack_segment(stage,row)!=baseline[row['rom_start']]:
            raise ValueError(f"{row['name']}: naming migration changed packed bytes; original sources untouched")
    write_makefile(stage,migrated)
    backup_parent = root/'asset-backups'
    backup_parent.mkdir(exist_ok=True)
    backup = Path(tempfile.mkdtemp(prefix='asset-names-',dir=backup_parent))
    os.rename(root/'assets',backup/'assets')
    try:
        os.rename(stage/'assets',root/'assets')
    except BaseException:
        os.rename(backup/'assets',root/'assets')
        raise
    print(f'Renamed {len(pending)} asset identities; all {len(baseline)} sections preserved.')
    print(f'Original editable tree retained at {backup}/assets')


if __name__=='__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root',type=Path,default=Path(__file__).resolve().parents[1])
    migrate_names(parser.parse_args().root.resolve())
