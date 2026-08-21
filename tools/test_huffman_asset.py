import unittest

from huffman_asset import CompressionMetadata, compress_huffman_asset, decompress_huffman_asset


class HuffmanAssetTests(unittest.TestCase):
    def test_plain_huffman_known_stream(self):
        metadata = CompressionMetadata(
            flags=0,
            table=bytes.fromhex("000203020100"),
            padding_bits="10",
            unused_tail=b"\xAA",
        )
        compressed = compress_huffman_asset(bytes((0, 1, 2, 0)), metadata)

        self.assertEqual(compressed, bytes.fromhex("00000004000002030201005aaa"))
        decompressed, extracted_metadata = decompress_huffman_asset(compressed)
        self.assertEqual(decompressed, bytes((0, 1, 2, 0)))
        self.assertEqual(extracted_metadata, metadata)

    def test_huffman_lz_greedy_copy(self):
        metadata = CompressionMetadata(
            flags=1,
            table=bytes.fromhex("0001010130300100"),
            padding_bits="",
            unused_tail=b"",
        )
        data = b"\x01\x01\x01\x01"

        compressed = compress_huffman_asset(data, metadata)
        decompressed, _ = decompress_huffman_asset(compressed)
        self.assertEqual(decompressed, data)


if __name__ == "__main__":
    unittest.main()
