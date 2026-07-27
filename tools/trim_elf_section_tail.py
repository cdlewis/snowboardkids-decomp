#!/usr/bin/env python3
import argparse
import struct
import sys


SHT_RELA = 4
SHT_REL = 9
SHT_SYMTAB = 2
STT_SECTION = 3


def main():
    parser = argparse.ArgumentParser(
        description="Remove verified zero padding from the logical end of an ELF32 section."
    )
    parser.add_argument("file", help="ELF object to update")
    parser.add_argument("section", help="section name to trim")
    parser.add_argument("byte_count", type=int, help="number of tail bytes to remove")
    args = parser.parse_args()

    if args.byte_count <= 0:
        parser.error("byte_count must be positive")

    with open(args.file, "r+b") as elf:
        data = bytearray(elf.read())

        if data[:4] != b"\x7fELF" or data[4] != 1:
            print("Error: expected an ELF32 object", file=sys.stderr)
            return 1

        if data[5] == 1:
            endian = "<"
        elif data[5] == 2:
            endian = ">"
        else:
            print("Error: unsupported ELF byte order", file=sys.stderr)
            return 1

        section_table_offset = struct.unpack_from(endian + "I", data, 0x20)[0]
        section_header_size = struct.unpack_from(endian + "H", data, 0x2E)[0]
        section_count = struct.unpack_from(endian + "H", data, 0x30)[0]
        section_names_index = struct.unpack_from(endian + "H", data, 0x32)[0]

        if section_header_size != 0x28 or section_names_index >= section_count:
            print("Error: malformed ELF32 section table", file=sys.stderr)
            return 1

        def section_header(index):
            offset = section_table_offset + (index * section_header_size)
            return offset, struct.unpack_from(endian + "10I", data, offset)

        _, names_header = section_header(section_names_index)
        names_offset = names_header[4]
        names_size = names_header[5]
        names = data[names_offset:names_offset + names_size]

        sections = []
        target_index = None
        for index in range(section_count):
            header_offset, header = section_header(index)
            name_offset = header[0]
            name_end = names.find(b"\0", name_offset)
            if name_end < 0:
                print("Error: malformed ELF section name table", file=sys.stderr)
                return 1
            name = names[name_offset:name_end].decode("ascii")
            sections.append((header_offset, header, name))
            if name == args.section:
                target_index = index

        if target_index is None:
            print(f"Error: section {args.section!r} was not found", file=sys.stderr)
            return 1

        target_header_offset, target_header, _ = sections[target_index]
        target_offset = target_header[4]
        target_size = target_header[5]
        if args.byte_count > target_size:
            print("Error: trim exceeds section size", file=sys.stderr)
            return 1

        new_size = target_size - args.byte_count
        padding = data[target_offset + new_size:target_offset + target_size]
        if any(padding):
            print("Error: section tail contains nonzero data", file=sys.stderr)
            return 1

        for _, header, name in sections:
            section_type = header[1]
            section_offset = header[4]
            section_size = header[5]
            linked_section = header[7]
            entry_size = header[9]

            if section_type in (SHT_REL, SHT_RELA) and linked_section == target_index:
                if entry_size == 0:
                    print(f"Error: relocation section {name!r} has no entry size", file=sys.stderr)
                    return 1
                for entry_offset in range(section_offset, section_offset + section_size, entry_size):
                    relocation_offset = struct.unpack_from(endian + "I", data, entry_offset)[0]
                    if relocation_offset >= new_size:
                        print("Error: trimmed range contains a relocation", file=sys.stderr)
                        return 1

            if section_type == SHT_SYMTAB:
                if entry_size != 0x10:
                    print("Error: unsupported ELF32 symbol entry size", file=sys.stderr)
                    return 1
                for entry_offset in range(section_offset, section_offset + section_size, entry_size):
                    symbol_value = struct.unpack_from(endian + "I", data, entry_offset + 4)[0]
                    symbol_size = struct.unpack_from(endian + "I", data, entry_offset + 8)[0]
                    symbol_type = data[entry_offset + 12] & 0xF
                    symbol_section = struct.unpack_from(endian + "H", data, entry_offset + 14)[0]
                    if (
                        symbol_section == target_index
                        and symbol_type == STT_SECTION
                        and symbol_value == 0
                        and symbol_size == target_size
                    ):
                        struct.pack_into(endian + "I", data, entry_offset + 8, new_size)
                    elif symbol_section == target_index and symbol_value + symbol_size > new_size:
                        print("Error: trimmed range overlaps a symbol", file=sys.stderr)
                        return 1

        struct.pack_into(endian + "I", data, target_header_offset + 0x14, new_size)
        elf.seek(0)
        elf.write(data)
        elf.truncate()

    print(
        f"Trimmed {args.byte_count} zero padding bytes from "
        f"{args.section} in {args.file}"
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
