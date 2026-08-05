#!/usr/bin/env python3
"""Convert readable _("text") expressions to MenuGlyphScript values."""

import argparse
import re
import sys
from pathlib import Path


MACRO_PATTERN = re.compile(r'(?<![A-Za-z0-9_])(_|_FRAGMENT)\("')
RAW_WORD_PATTERN = re.compile(r"0x([0-9A-Fa-f]{4})")
COLOR_PATTERN = re.compile(r"COLOR:([0-9]+)")


class TextConversionError(ValueError):
    pass


def _unescape_charmap_key(value):
    return {
        r"\n": "\n",
        r"\0": "\0",
        r"\t": "\t",
        r"\'": "'",
        r'\"': '"',
        r"\\": "\\",
    }.get(value, value)


def parse_charmap(path):
    characters = {}
    named_values = {}

    for line_number, line in enumerate(Path(path).read_text().splitlines(), 1):
        line = line.strip()
        if not line or line.startswith("//"):
            continue

        character_match = re.fullmatch(r"'((?:\\.|[^'])*)'\s*=\s*(0x[0-9A-Fa-f]{4})", line)
        named_match = re.fullmatch(r"([A-Z][A-Z0-9_]*)\s*=\s*(0x[0-9A-Fa-f]{4})", line)
        if character_match:
            key = _unescape_charmap_key(character_match.group(1))
            characters[key] = int(character_match.group(2), 16)
        elif named_match:
            named_values[named_match.group(1)] = int(named_match.group(2), 16)
        else:
            raise TextConversionError(f"{path}:{line_number}: invalid charmap entry: {line}")

    if "\0" not in characters:
        raise TextConversionError(f"{path}: charmap must define a terminator for \\0")
    return characters, named_values


def _parse_directive(text, start, named_values):
    end = text.find("}", start + 1)
    if end < 0:
        raise TextConversionError(f"unterminated directive at character {start}")

    directive = text[start + 1:end]
    raw_match = RAW_WORD_PATTERN.fullmatch(directive)
    color_match = COLOR_PATTERN.fullmatch(directive)
    if raw_match:
        values = [int(raw_match.group(1), 16)]
    elif color_match:
        color = int(color_match.group(1), 10)
        if color > 0xFFFF:
            raise TextConversionError(f"COLOR value is outside u16 range: {color}")
        values = [0xFFFC, color]
    elif directive in named_values:
        values = [named_values[directive]]
    else:
        raise TextConversionError(f"unknown directive {{{directive}}}")
    return values, end + 1


def encode_text(text, characters, named_values, append_terminator=True):
    values = []
    index = 0
    while index < len(text):
        character = text[index]
        if character == "{":
            directive_values, index = _parse_directive(text, index, named_values)
            values.extend(directive_values)
            continue

        if character == "\\":
            if index + 1 >= len(text):
                raise TextConversionError("trailing backslash in encoded text")
            escape = text[index + 1]
            escapes = {
                "n": "\n",
                "0": "\0",
                "t": "\t",
                "\\": "\\",
                '"': '"',
                "{": "{",
                "}": "}",
            }
            if escape not in escapes:
                raise TextConversionError(f"unsupported escape \\{escape}")
            character = escapes[escape]
            index += 2
        else:
            index += 1

        if character not in characters:
            raise TextConversionError(f"unknown character {character!r}")
        values.append(characters[character])

    if append_terminator:
        values.append(characters["\0"])
    return values


def _inside_comment(source, position):
    line_start = source.rfind("\n", 0, position) + 1
    if "//" in source[line_start:position]:
        return True
    return source.rfind("/*", 0, position) > source.rfind("*/", 0, position)


def _read_string_literal(source, start):
    characters = []
    index = start
    while index < len(source):
        character = source[index]
        if character == "\\" and index + 1 < len(source):
            characters.extend((character, source[index + 1]))
            index += 2
        elif character == '"':
            return "".join(characters), index + 1
        else:
            characters.append(character)
            index += 1
    raise TextConversionError("unterminated _() string literal")


def process_source(source, characters, named_values):
    if "font_encoding.h" not in source:
        return source

    output = []
    position = 0
    while True:
        match = MACRO_PATTERN.search(source, position)
        if match is None:
            output.append(source[position:])
            break
        if _inside_comment(source, match.start()):
            output.append(source[position:match.end()])
            position = match.end()
            continue

        output.append(source[position:match.start()])
        macro_name = match.group(1)
        text, after_quote = _read_string_literal(source, match.end())
        if after_quote >= len(source) or source[after_quote] != ")":
            raise TextConversionError("_() must contain exactly one string literal")
        values = encode_text(text, characters, named_values, append_terminator=(macro_name == "_"))
        output.append(", ".join(f"0x{value:04X}" for value in values))
        position = after_quote + 1
    return "".join(output)


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("charmap")
    parser.add_argument("input")
    parser.add_argument("output")
    args = parser.parse_args(argv)

    try:
        characters, named_values = parse_charmap(args.charmap)
        source = sys.stdin.read() if args.input == "-" else Path(args.input).read_text()
        converted = process_source(source, characters, named_values)
        if args.output == "-":
            sys.stdout.write(converted)
        else:
            Path(args.output).write_text(converted)
    except (OSError, TextConversionError) as error:
        print(f"textconv: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
