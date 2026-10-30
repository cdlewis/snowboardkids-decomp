#!/usr/bin/env python3

import argparse
import os
import subprocess
import sys
import tempfile


SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
ROOT_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, ".."))

CPP_FLAGS = [
    "-Iinclude",
    "-DLANGUAGE_C",
    "-D_LANGUAGE_C",
    "-D_MIPS_SZLONG=32",
    "-DNDEBUG",
    "-DM2CTX",
]


def import_c_file(in_file, jp=False):
    rel_file = os.path.relpath(in_file, ROOT_DIR)
    parts = rel_file.split(os.sep)
    source_root = os.path.join(*parts[:2]) if parts[:2] in (["src", "us"], ["src", "jp"]) else parts[0]
    source_include = "-I" + (source_root if source_root in ("src", "src/us", "src/jp") else "src/us")
    cpp_flags = CPP_FLAGS + (["-DVERSION_JP"] if jp else ["-DVERSION_US"])
    macro_cmd = ["gcc", "-E", "-P", "-dM", source_include, *cpp_flags, rel_file]
    preprocess_cmd = ["gcc", "-E", "-P", source_include, *cpp_flags, rel_file]

    with tempfile.NamedTemporaryFile(suffix=".c") as tmp:
        stock_macros = subprocess.check_output(
            ["gcc", "-E", "-P", "-dM", tmp.name],
            cwd=ROOT_DIR,
            encoding="utf-8",
        )
    stock_macros += "#define __STDC_HOSTED__ 0\n"

    try:
        out_text = subprocess.check_output(macro_cmd, cwd=ROOT_DIR, encoding="utf-8")
        out_text += subprocess.check_output(preprocess_cmd, cwd=ROOT_DIR, encoding="utf-8")
    except subprocess.CalledProcessError:
        print(
            "Failed to preprocess input file, when running command:\n"
            + " ".join(preprocess_cmd),
            file=sys.stderr,
        )
        sys.exit(1)

    if not out_text:
        print("Output is empty - aborting", file=sys.stderr)
        sys.exit(1)

    for line in stock_macros.strip().splitlines():
        out_text = out_text.replace(line + "\n", "")

    return out_text


def main():
    parser = argparse.ArgumentParser(
        description="Create a context file for m2c / decomp.me"
    )
    parser.add_argument("c_file", help="C file from which to create context")
    parser.add_argument("--jp", action="store_true", help="Define VERSION_JP for Japanese code")
    args = parser.parse_args()

    output = import_c_file(args.c_file, jp=args.jp)

    with open(os.path.join(ROOT_DIR, "ctx.c"), "w", encoding="utf-8") as f:
        f.write(output)


if __name__ == "__main__":
    main()
