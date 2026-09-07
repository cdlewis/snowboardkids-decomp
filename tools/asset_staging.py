"""Keep Splat's linker identities separate from its temporary extraction files."""
from pathlib import Path
from splat.util import options


def staging_path(logical_path):
    return Path(options.opts.base_path) / 'build/asset-staging' / logical_path.relative_to(options.opts.asset_path)
