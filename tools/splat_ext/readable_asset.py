from splat.segtypes.common.segment import CommonSegment
from splat.util import options
from tools.asset_staging import staging_path

from tools.readable_assets import extract_asset
from tools.course_surface_data_common import write_yaml, load_yaml
from tools.audio_assets import parse_pointer_bank, extract_samples
from tools.huffman_asset import decompress_huffman_asset


class N64SegReadable_asset(CommonSegment):
    @staticmethod
    def is_data():
        return True

    @property
    def statistics_type(self):
        return "readable_asset"

    def get_linker_section(self):
        return ".data"

    def get_section_flags(self):
        return "wa"

    def out_path(self):
        return options.opts.asset_path / "readable" / f"{self.name}.yaml"

    def should_split(self):
        return self.extract and any(options.opts.is_mode_active(mode) for mode in (self.type, "bin", "all"))

    def split(self, rom_bytes):
        if self.rom_end is None:
            raise ValueError(f"{self.name}: missing ROM end")
        try:
            if self.yaml["asset_format"] == "sample_bank":
                segments = load_yaml(options.opts.base_path / "snowboardkids.yaml")["segments"]
                segments = [s for s in segments if isinstance(s, list) or "start" in s]
                ranges = {}
                for s, following in zip(segments, segments[1:]):
                    if isinstance(s, dict) and "name" in s:
                        ranges[str(s["name"])] = (s["start"], following["start"] if isinstance(following, dict) else following[0])
                a, b = ranges[self.yaml["bank_asset"]]
                bank = parse_pointer_bank(decompress_huffman_asset(rom_bytes[a:b])[0])
                manifest = dict(schema_version=2, name=self.name, format="sample_bank", compression="none",
                                decompressed_size=self.rom_end-self.rom_start,
                                bank_asset=self.yaml["bank_asset"],
                                **extract_samples(rom_bytes, self.rom_start, self.rom_end,
                                                  ranges[self.yaml["sample_base_asset"]][0], bank, staging_path(self.out_path())))
            else:
                manifest = extract_asset(rom_bytes[self.rom_start:self.rom_end], self.yaml, staging_path(self.out_path()))
            write_yaml(staging_path(self.out_path()), manifest)
        except ValueError as exc:
            raise ValueError(f"{self.name} at ROM 0x{self.rom_start:X}: {exc}") from exc
