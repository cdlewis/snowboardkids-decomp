#!/usr/bin/env python3
"""Snowboard Kids' Huffman and Huffman/LZ asset codec."""

from __future__ import annotations

from dataclasses import dataclass


MAX_COPY_LENGTH = 0xF
MAX_COPY_OFFSET = 0xFFF


@dataclass
class HuffmanNode:
    weight: int
    left: int = -1
    right: int = -1
    value: int = -1


@dataclass
class CompressionMetadata:
    flags: int
    table: bytes
    padding_bits: str
    unused_tail: bytes


def _build_tree(table: bytes) -> tuple[list[HuffmanNode], int, int]:
    nodes: list[HuffmanNode] = []
    queue: list[int] = []
    offset = 0

    while True:
        if offset >= len(table):
            raise ValueError("Huffman symbol table has no terminator")
        range_start = table[offset]
        offset += 1
        if offset != 1 and range_start == 0:
            break
        if offset >= len(table):
            raise ValueError("truncated Huffman symbol range")
        range_end = table[offset]
        offset += 1
        if range_end < range_start:
            raise ValueError(f"invalid Huffman symbol range {range_start:#x}..{range_end:#x}")

        for value in range(range_start, range_end + 1):
            if offset >= len(table):
                raise ValueError("truncated Huffman weight table")
            node = HuffmanNode(weight=table[offset], value=value)
            offset += 1
            nodes.append(node)
            _insert_queue(queue, nodes, len(nodes) - 1)

    if not nodes:
        raise ValueError("empty Huffman symbol table")

    while len(queue) >= 2:
        right = queue.pop()
        left = queue.pop()
        nodes.append(
            HuffmanNode(
                weight=nodes[left].weight + nodes[right].weight,
                left=left,
                right=right,
            )
        )
        _insert_queue(queue, nodes, len(nodes) - 1)

    return nodes, queue[0], offset


def _insert_queue(queue: list[int], nodes: list[HuffmanNode], node_index: int) -> None:
    """Match insertHuffmanQueueNode's descending, stable weight ordering."""
    weight = nodes[node_index].weight
    index = 0
    while index < len(queue) and nodes[queue[index]].weight >= weight:
        index += 1
    queue.insert(index, node_index)


def _decode_symbol(data: bytes, bitstream_start: int, bit_position: int, nodes: list[HuffmanNode], root: int) -> tuple[int, int]:
    current = root
    while nodes[current].value < 0:
        byte_offset = bitstream_start + bit_position // 8
        if byte_offset >= len(data):
            raise ValueError("Huffman bitstream ended before the declared output size")
        bit = (data[byte_offset] >> (7 - bit_position % 8)) & 1
        bit_position += 1
        current = nodes[current].right if bit else nodes[current].left
    return nodes[current].value, bit_position


def decompress_huffman_asset(data: bytes) -> tuple[bytes, CompressionMetadata]:
    if len(data) < 6:
        raise ValueError("compressed Huffman asset is too small")

    output_size = int.from_bytes(data[0:4], "big")
    flags = data[4]
    if flags not in (0, 1):
        raise ValueError(f"unsupported Huffman asset flags {flags:#x}")

    nodes, root, table_size = _build_tree(data[5:])
    table = data[5 : 5 + table_size]
    bitstream_start = 5 + table_size
    bit_position = 0
    output = bytearray()

    while len(output) < output_size:
        first, bit_position = _decode_symbol(data, bitstream_start, bit_position, nodes, root)
        if flags == 0:
            output.append(first)
            continue

        second, bit_position = _decode_symbol(data, bitstream_start, bit_position, nodes, root)
        if first == 0:
            output.append(second)
            continue

        count = first >> 4
        source_offset = ((first << 8) | second) & MAX_COPY_OFFSET
        if count == 0 or source_offset == 0 or source_offset > len(output):
            raise ValueError(
                f"invalid Huffman/LZ copy at output {len(output):#x}: "
                f"count={count:#x}, offset={source_offset:#x}"
            )
        for _ in range(count):
            output.append(output[-source_offset])
            if len(output) > output_size:
                raise ValueError("Huffman/LZ copy exceeds the declared output size")

    used_bytes = (bit_position + 7) // 8
    padding_count = (-bit_position) % 8
    padding_bits = ""
    if padding_count:
        last_byte = data[bitstream_start + used_bytes - 1]
        padding_bits = f"{last_byte & ((1 << padding_count) - 1):0{padding_count}b}"

    metadata = CompressionMetadata(
        flags=flags,
        table=table,
        padding_bits=padding_bits,
        unused_tail=data[bitstream_start + used_bytes :],
    )
    return bytes(output), metadata


def _build_codes(nodes: list[HuffmanNode], root: int) -> dict[int, tuple[int, ...]]:
    codes: dict[int, tuple[int, ...]] = {}

    def visit(node_index: int, bits: tuple[int, ...]) -> None:
        node = nodes[node_index]
        if node.value >= 0:
            codes[node.value] = bits
            return
        visit(node.left, bits + (0,))
        visit(node.right, bits + (1,))

    visit(root, ())
    return codes


def _tokenize_lz(data: bytes) -> list[int]:
    tokens: list[int] = []
    positions_by_byte: list[list[int]] = [[] for _ in range(0x100)]
    position = 0

    while position < len(data):
        best_count = 0
        best_offset = 0
        window_start = max(0, position - MAX_COPY_OFFSET)

        for candidate in reversed(positions_by_byte[data[position]]):
            if candidate < window_start:
                break
            source_offset = position - candidate
            count = 0
            while (
                count < MAX_COPY_LENGTH
                and position + count < len(data)
                and data[candidate + count] == data[position + count]
            ):
                count += 1
            if count > best_count:
                best_count = count
                best_offset = source_offset
                if count == MAX_COPY_LENGTH:
                    break

        if best_count:
            tokens.extend((((best_count << 4) | (best_offset >> 8)), best_offset & 0xFF))
            for value in data[position : position + best_count]:
                positions_by_byte[value].append(position)
                position += 1
        else:
            tokens.extend((0, data[position]))
            positions_by_byte[data[position]].append(position)
            position += 1

    return tokens


def compress_huffman_asset(data: bytes, metadata: CompressionMetadata) -> bytes:
    nodes, root, table_size = _build_tree(metadata.table)
    if table_size != len(metadata.table):
        raise ValueError("Huffman table contains bytes after its terminator")
    codes = _build_codes(nodes, root)
    symbols = data if metadata.flags == 0 else _tokenize_lz(data)

    encoded = bytearray()
    current_byte = 0
    bit_count = 0
    for symbol in symbols:
        if symbol not in codes:
            raise ValueError(f"Huffman table has no code for symbol {symbol:#x}")
        for bit in codes[symbol]:
            current_byte |= bit << (7 - bit_count)
            bit_count += 1
            if bit_count == 8:
                encoded.append(current_byte)
                current_byte = 0
                bit_count = 0

    if bit_count:
        needed_padding = 8 - bit_count
        saved_padding = metadata.padding_bits[:needed_padding].ljust(needed_padding, "0")
        current_byte |= int(saved_padding, 2) if saved_padding else 0
        encoded.append(current_byte)

    return (
        len(data).to_bytes(4, "big")
        + bytes((metadata.flags,))
        + metadata.table
        + bytes(encoded)
        + metadata.unused_tail
    )
