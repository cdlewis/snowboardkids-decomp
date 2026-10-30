#!/usr/bin/env python3
"""Convert readable _("text") expressions to MenuGlyphScript values."""

import argparse
import re
import sys
from pathlib import Path


SOURCE_TOKEN_PATTERN = re.compile(
    r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\''
    r'|(?<![A-Za-z0-9_])(?P<macro>_SMALL_FRAGMENT|_SMALL|_FRAGMENT|_)(?![A-Za-z0-9_])',
    re.DOTALL,
)
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


def _skip_trivia(source, position):
    while position < len(source):
        if source[position].isspace():
            position += 1
        elif source.startswith("/*", position):
            end = source.find("*/", position + 2)
            if end < 0:
                raise TextConversionError("unterminated comment in _() expression")
            position = end + 2
        elif source.startswith("//", position):
            end = source.find("\n", position + 2)
            position = len(source) if end < 0 else end + 1
        else:
            break
    return position


def _read_string_literal(source, start):
    characters = []
    index = start
    while index < len(source):
        character = source[index]
        if character == "\\" and index + 1 < len(source):
            if source[index + 1] == "\n":
                index += 2
                continue
            characters.extend((character, source[index + 1]))
            index += 2
        elif character == '"':
            return "".join(characters), index + 1
        else:
            characters.append(character)
            index += 1
    raise TextConversionError("unterminated _() string literal")


def process_source(source, characters, named_values, small_charmap=None):
    if "font_encoding.h" not in source:
        return source

    output = []
    position = 0
    while True:
        match = SOURCE_TOKEN_PATTERN.search(source, position)
        if match is None:
            output.append(source[position:])
            break
        if match.group("macro") is None:
            output.append(source[position:match.end()])
            position = match.end()
            continue

        opening = _skip_trivia(source, match.end())
        if opening >= len(source) or source[opening] != "(":
            output.append(source[position:match.end()])
            position = match.end()
            continue
        cursor = _skip_trivia(source, opening + 1)
        literals = []
        while cursor < len(source) and source[cursor] == '"':
            literal, cursor = _read_string_literal(source, cursor + 1)
            literals.append(literal)
            cursor = _skip_trivia(source, cursor)
        if not literals or cursor >= len(source) or source[cursor] != ")":
            raise TextConversionError("_() must contain string literals only")
        output.append(source[position:match.start()])
        macro_name = match.group("macro")
        text = "".join(literals)
        if macro_name.startswith("_SMALL"):
            if small_charmap is None:
                small_charmap = parse_charmap(Path(__file__).with_name("charmap.small.txt"))
            glyphs, directives = small_charmap
        else:
            glyphs, directives = characters, named_values
        values = encode_text(text, glyphs, directives, append_terminator=not macro_name.endswith("FRAGMENT"))
        output.append(", ".join(f"0x{value:04X}" for value in values))
        # Preserve following source line numbers, including when clang-format
        # wraps a call or splits its string into adjacent literals.
        output.append("\n" * source[match.start():cursor + 1].count("\n"))
        position = cursor + 1
    return "".join(output)


def decode_text(values, characters, named_values):
    """Decode every u16, retaining terminators and unknown/control words exactly."""
    chars = {value: key for key, value in reversed(list(characters.items()))}
    names = {value: key for key, value in reversed(list(named_values.items()))}
    out, i = [], 0
    while i < len(values):
        value = values[i]
        if not 0 <= value <= 65535:
            raise TextConversionError("text word exceeds u16 range")
        if value == 0xFFFC and i + 1 < len(values):
            i += 1
            out.append(f"{{COLOR:{values[i]}}}")
        elif value in chars:
            char = chars[value]
            out.append({"\0": r"\0", "\\": r"\\", "{": r"\{", "}": r"\}"}.get(char, char))
        elif value in names:
            out.append("{" + names[value] + "}")
        else:
            out.append(f"{{0x{value:04X}}}")
        i += 1
    return "".join(out)


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("charmap")
    parser.add_argument("input")
    parser.add_argument("output")
    parser.add_argument("--decode", action="store_true", help="decode big-endian u16 binary to readable text")
    parser.add_argument("--small-charmap", type=Path, default=Path(__file__).with_name("charmap.small.txt"))
    args = parser.parse_args(argv)

    try:
        characters, named_values = parse_charmap(args.charmap)
        if args.decode:
            data = sys.stdin.buffer.read() if args.input == "-" else Path(args.input).read_bytes()
            if len(data) % 2:
                raise TextConversionError("text binary has an odd byte count")
            converted = decode_text([int.from_bytes(data[i:i+2], "big") for i in range(0, len(data), 2)], characters, named_values)
        else:
            source = sys.stdin.read() if args.input == "-" else Path(args.input).read_text()
            converted = process_source(source, characters, named_values, parse_charmap(args.small_charmap))
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
