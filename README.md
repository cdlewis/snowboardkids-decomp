Snowboard Kids  
[![Build](https://github.com/cdlewis/snowboardkids-decomp/actions/workflows/build.yaml/badge.svg)](https://github.com/cdlewis/snowboardkids-decomp/actions/workflows/build.yaml)
[![US](https://decomp.dev/cdlewis/snowboardkids-decomp/us.svg?mode=shield&measure=code&label=US%20Code)](https://decomp.dev/cdlewis/snowboardkids-decomp)
[![JP Code](https://decomp.dev/cdlewis/snowboardkids-decomp/jp.svg?mode=shield&measure=code&label=JP%20Code)](https://decomp.dev/cdlewis/snowboardkids-decomp)
[![JP Data](https://decomp.dev/cdlewis/snowboardkids-decomp/jp.svg?mode=shield&measure=data&label=JP%20Data)](https://decomp.dev/cdlewis/snowboardkids-decomp)
[![Chat](https://img.shields.io/discord/729856662357278750?color=%237289DA&logo=discord&logoColor=%23FFFFFF)](https://discord.gg/PJzG4F8SpF)
=============

![box art](Snowboard_Kids_Box.jpg)

A matching decompilation of [Snowboard Kids](https://en.wikipedia.org/wiki/Snowboard_Kids) for Nintendo 64.

**This repository does not contain any game assets or assembly whatsoever. An existing copy of the game is required. This project is not a port, to PC or any other platform. It takes a Nintendo 64 rom, extracts assets from it, and combines them with C code we reverse-engineered from the rom, to produce an identical rom. It is explicitly a non-goal for this project to be used for any commercial purpose.**

## Cloning

Clone this repository, including its submodules:

```
git clone --recurse-submodules -j8 https://github.com/cdlewis/snowboardkids-decomp.git
```

# Dependencies

The current build is verified on Linux (x86-64). The macOS setup below needs
validation with the updated toolchain selection.

System packages:

* make
* git
* python3
* pip3
* clang
* wget and tar (toolchain download)
* binutils-mips-linux-gnu

On Debian/Ubuntu:
```sh
sudo apt install make git python3 python3-venv python3-pip clang wget binutils-mips-linux-gnu
```

On macOS using homebrew. Note that calls below to `make` should use `gmake` instead to run the homebrew version.
```sh
brew install make python llvm wget tehzz/n64-dev/mips64-elf-binutils
export PATH="$(brew --prefix llvm)/bin:$PATH"
export MAKE=gmake
```

Build the toolchain:

```bash
make -C tools
```

Install Python dependencies in virtual environment:

```bash
python3 -m venv .venv
source .venv/bin/activate
python3 -m pip install -r requirements-build.txt
```

For optional matching helpers, asm-differ, the permuter, m2c, and task-runner
workflows, also install `python3 -m pip install -r requirements.txt`. Existing
development environments can continue using that requirements file.

The build selects `tools/ido-recomp/linux` or `tools/ido-recomp/macos` for the
host. Set `IDO_DIR` to use another installed IDO 5.3 toolchain. Linux is verified;
the updated macOS selection still needs validation on macOS.

## Building

Place your ROM in the repository root using the filename below:

| Version | Filename | SHA1 |
| --- | --- | --- |
| North America (`us`) | `snowboardkids.z64` | `1583bacc9046a360df8ea4d536942155247e154c` |
| Japan (`jp`) | `snowboardkids.jp.z64` | `96480c8fa7285d9e850f6ba002e56bcb7ada9796` |

Build and verify the ROM:

```bash
./tools/build-and-verify.sh              # North America (default)
./tools/build-and-verify.sh --version jp
```

The output is `build/snowboardkids.z64` for US or
`build/jp/snowboardkids.z64` for JP. A successful build prints the output path
followed by `OK`, confirming that its SHA1 matches the original ROM.

For incremental builds, extract once, then run make. Use `VERSION=us` or
`VERSION=jp` on both commands:

```bash
make VERSION=jp extract
make VERSION=jp -j8
```

JP currently builds from extracted assembly; C decompilation is in progress
for US. Readable asset tools, progress reporting, and `--mode shiftable` support
US only.

## Reverse-engineering notes

- [Multi-version development](docs/versions.md) covers version-specific paths,
  diff commands, and matching Japanese functions.
- [Readable asset extraction](docs/readable-assets.md) explains semantic asset
  bundles, Blender-friendly GLB models/animations, PNGs, text, replays, audio,
  safe migration, and byte-exact rebuilding. Start with
  `assets/items/pan/pan.glb` and `assets/courses/<course>/preview.glb`;
  regenerate GLBs with `make asset-previews`. All eleven items have named bundles.
- [Course asset inventory](docs/course-assets.md) documents the ten course
  environments, their ROM ranges, runtime asset roles, and known data layouts.

Contributing
============

Contributions are most welcome! There are a variety of ways you can assist:

* Fix compiler warnings
* Clean up code: you'll see plenty of hastily decompiled functions that use pointer arithmetic rather than proper struct access. We need help cleaning up these functions.
* Document code: some functions/variables have useful names (rather than func_XXX or D_XXX) but most don't. Some that do are incorrectly named. We need lots of help investigating and documenting what all these functions do.
* Support building the project on more platforms such as Windows or ARM Linux. The current build is verified on Linux; testing the macOS setup would also help.

If you have any additional questions, please reach out on Discord (linked in the header). However please note that, since this is a clean room decompilation, we cannot accept contributions based on leaked source code or from those with proprietary knowledge about the game or related subjects.

Acknowledgements
================

This project wouldn't be possible without the collective knowledge, tools, and support of the broader decompilation community. Huge thanks to contributors of other N64 decomp projects, the teams behind [decomp.me](https://decomp.me/) and [decomp.dev](https://decomp.dev/), and the incredibly helpful discussions happening on Discord. These resources have been invaluable for solving problems, speeding up setup, and staying motivated throughout the process.
