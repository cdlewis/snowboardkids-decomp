import unittest
from pathlib import Path

import yaml

from tools.generate_course_definitions import COURSE_COUNT, generated_files, load_courses, parse_int


class CourseDefinitionTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.repo_root = Path(__file__).resolve().parent.parent
        cls.courses = load_courses(cls.repo_root / "config/courses")

    def test_course_ids_match_the_runtime_order(self):
        self.assertEqual(len(self.courses), COURSE_COUNT)
        self.assertEqual(
            [course["key"] for course in self.courses],
            [
                "big_snowman",
                "sunset_rock",
                "night_highway",
                "grass_valley",
                "dizzy_land",
                "quicksand_valley",
                "silver_mountain",
                "animal_land",
                "ninja_land",
                "rookie_mountain",
            ],
        )

    def test_asset_starts_and_ends_exist_in_splat_layout(self):
        with (self.repo_root / "snowboardkids.yaml").open("r", encoding="utf-8") as stream:
            config = yaml.safe_load(stream)

        segments = []
        def collect(value):
            if isinstance(value, dict):
                if "start" in value:
                    segments.append(value)
                for child in value.values():
                    collect(child)
            elif isinstance(value, list):
                for child in value:
                    collect(child)
        collect(config)

        starts = {parse_int(segment["start"]) for segment in segments}
        named_starts = {
            str(segment["name"]): parse_int(segment["start"])
            for segment in segments
            if "name" in segment
        }
        for course in self.courses:
            for asset in course["assets"].values():
                if asset is None:
                    continue
                self.assertEqual(named_starts[asset["symbol"]], parse_int(asset["rom_start"]))
                self.assertIn(parse_int(asset["rom_end"]), starts)

    def test_checked_in_generated_files_are_current(self):
        for name, expected in generated_files(self.courses).items():
            actual = (self.repo_root / "include/generated" / name).read_text(encoding="utf-8")
            self.assertEqual(actual, expected, name)


if __name__ == "__main__":
    unittest.main()
