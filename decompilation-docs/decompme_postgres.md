# decomp.me PostgreSQL Similarity Search

This project can use a PostgreSQL import of the decomp.me database as an
external corpus of already matched IDO assembly. The useful workflow is:

1. Pick a local function from `asm/nonmatchings`.
2. Query matched decomp.me scratches from the database.
3. Normalize both assembly streams so register numbers, labels, symbols, and
   most immediates do not dominate the comparison.
4. Rank matched scratches by structural assembly similarity.

The helper script is `tools/decompme_postgres.py`.

## Database Tables

The local dump currently contains the core scratch tables:

```text
public.coreapp_scratch
public.coreapp_assembly
public.coreapp_asm
```

The relevant relationships are:

```text
coreapp_scratch.target_assembly_id -> coreapp_assembly.hash
coreapp_assembly.source_asm_id     -> coreapp_asm.hash
```

Exact matched scratches are rows where `coreapp_scratch.score = 0`. The helper
uses exact matches by default; pass `--include-match-overrides` only if you also
want decomp.me `match_override` rows.

## Defaults

The helper defaults to the compiler profile that matches Snowboard Kids best:

```text
compiler       ido5.3
compiler_flags contains -O2
compiler_flags contains -mips1
```

The local `decompme_dump` does not contain preset `241`; its preset IDs only go
up to `184`. Do not use preset `241` with this dump. If you want to narrow to the
largest matching IDO 5.3 `-O2 -mips1` preset in this dump, use `--preset 27`.

## Similarity Search

Search the matched decomp.me corpus for functions structurally similar to a local
nonmatching function:

```sh
python3 tools/decompme_postgres.py -d decompme_dump --similar-to func_8000D340 --limit 20
```

You can also pass a file path:

```sh
python3 tools/decompme_postgres.py -d decompme_dump \
  --similar-to asm/nonmatchings/race_to_main_menu_transition/func_8000D340.s \
  --limit 20
```

Narrow to preset `27` if you want that corpus only:

```sh
python3 tools/decompme_postgres.py -d decompme_dump \
  --similar-to func_8000D340 \
  --preset 27 \
  --limit 20
```

Machine-readable output:

```sh
python3 tools/decompme_postgres.py -d decompme_dump \
  --similar-to func_8000D340 \
  --limit 20 \
  --json
```

By default, results are deduplicated by matched function name. To see every
scratch row, including duplicates for the same function:

```sh
python3 tools/decompme_postgres.py -d decompme_dump \
  --similar-to func_8000D340 \
  --all-scratches
```

## Scoring

The similarity score is a weighted blend of:

```text
instruction  normalized instruction trigram overlap
opcode       opcode sequence and opcode n-gram similarity
memory       normalized load/store shape overlap
control      branch/jump sequence similarity
size         instruction-count ratio
```

Normalization is per function. It canonicalizes register roles by first use
within register classes, normalizes local labels and symbols, replaces memory
offsets with `OFF(base)`, and collapses most immediate values to `IMM`. This
preserves structure such as "the same temp register is reused here" without
requiring exact physical register numbers to match.

## Other Queries

The helper still supports basic scratch lookup/export:

```sh
python3 tools/decompme_postgres.py -d decompme_dump --function func_80000000
python3 tools/decompme_postgres.py -d decompme_dump --search-source some_identifier
python3 tools/decompme_postgres.py -d decompme_dump --search-asm 'jal func_80012345'
python3 tools/decompme_postgres.py -d decompme_dump --slug SCRATCH
```
