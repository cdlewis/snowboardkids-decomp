"""Check runtime BSS allocation, which a ROM checksum alone cannot verify."""

from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
CLANG = shutil.which("clang")
LD = shutil.which("mips-linux-gnu-ld")
NM = shutil.which("mips-linux-gnu-nm")
BUFFERS = (
    "gRelocatableHeapStart", "gBootThreadStack", "gGameThreadStack",
    "gRuntimeBufferReserved", "gRspOutputBuffer", "gRspYieldBuffer",
    "gRspDramStack", "gDepthBuffer", "gFramebuffers",
)


def symbols(path):
    result = {}
    output = subprocess.check_output([NM, "-S", str(path)], text=True)
    for line in output.splitlines():
        fields = line.split()
        if len(fields) == 4:
            value, size, kind, name = fields
            result[name] = (int(value, 16), int(size, 16), kind)
        elif len(fields) == 3:
            value, kind, name = fields
            result[name] = (int(value, 16), 0, kind)
    return result


@unittest.skipUnless(CLANG and LD and NM, "requires Clang and MIPS binutils")
class RuntimeMemoryLayoutTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.workspace = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.workspace.cleanup)
        cls.directory = Path(cls.workspace.name)
        for version in ("us", "jp"):
            subprocess.run([
                CLANG, "--target=mips-unknown-linux-gnu", "-mabi=32", "-march=mips3",
                "-mno-abicalls", "-fno-pic", "-G0", "-ffreestanding", "-fno-common",
                "-fno-addrsig", "-DLANGUAGE_C", "-D_LANGUAGE_C", "-D_MIPS_SZLONG=32",
                "-DCOMPILING_LIBULTRA", "-DBUILD_VERSION=VERSION_I", "-DF3DEX_GBI",
                "-DVERSION_" + version.upper(), "-I.", "-Iinclude", "-Iinclude/PR",
                "-c", "src/engine/system_runtime_buffers.c", "-o",
                str(cls.directory / (version + ".o")),
            ], cwd=ROOT, check=True, capture_output=True, text=True)

    def link(self, version, displacement=0):
        base = 0x8015F7A0 if version == "jp" else 0x80160480
        script = self.directory / "layout.ld"
        script.write_text(f"""
main_BSS_END = {base};
main_BSS_SIZE = 0x7DAC0;
entry_ROM_START = 0x1000;
entry_VRAM = 0x80000400;
main_ROM_START = 0x1050;
main_VRAM = 0x80000450;
main_ROM_END = 0xE35C0;
SECTIONS {{
    .buffers_bss {base + displacement} (NOLOAD) : {{
        buffers_BSS_START = .;
        *(.bss)
        buffers_BSS_END = .;
    }}
}}
""")
        elf = self.directory / "layout.elf"
        result = subprocess.run([
            LD, "-T", str(script), "-T", str(ROOT / "linker_scripts/runtime_memory.ld"),
            "-o", str(elf), str(self.directory / (version + ".o")),
        ], capture_output=True, text=True)
        return result, elf

    def test_both_regions_allocate_real_bss_at_recorded_addresses(self):
        for version, metadata in (("us", "symbol_addrs.txt"), ("jp", "symbol_addrs.jp.txt")):
            with self.subTest(version=version):
                result, elf = self.link(version)
                self.assertEqual(result.returncode, 0, result.stderr)
                actual = symbols(elf)
                source = (ROOT / metadata).read_text()
                for name in BUFFERS:
                    match = re.search(r"^" + name + r" = (0x[0-9A-Fa-f]+);[^\n]*size:(0x[0-9A-Fa-f]+)", source, re.M)
                    self.assertIsNotNone(match, name)
                    self.assertEqual(actual[name], (int(match[1], 16), int(match[2], 16), "B"), name)

    def test_physical_depth_address_follows_buffer_placement(self):
        result, elf = self.link("us", displacement=64)
        self.assertEqual(result.returncode, 0, result.stderr)
        actual = symbols(elf)
        self.assertEqual(actual["gDepthBufferPhysicalAddress"][0], actual["gDepthBuffer"][0] - 0x80000000)
        self.assertEqual(actual["gDepthBufferPhysicalAddress"][0], 0x369040)

    def test_rejects_overlap_with_main_bss(self):
        result, _ = self.link("jp", displacement=-16)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("overlap main BSS", result.stderr)

    def test_rejects_ram_overflow(self):
        result, _ = self.link("jp", displacement=0x2000)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("exceeds original 4 MB RAM", result.stderr)

    def test_rejects_gpu_misalignment(self):
        result, _ = self.link("us", displacement=16)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("depth buffer requires 64-byte alignment", result.stderr)


if __name__ == "__main__":
    unittest.main()
