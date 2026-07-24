#!/usr/bin/env python3

import tempfile
import unittest
from pathlib import Path

from tools import fix_redundant_externs as fixer


class RedundantExternFixerTests(unittest.TestCase):
    def setUp(self):
        self.temp_dir = tempfile.TemporaryDirectory()
        self.root = Path(self.temp_dir.name)
        (self.root / "src").mkdir()
        (self.root / "include" / "game").mkdir(parents=True)

    def tearDown(self):
        self.temp_dir.cleanup()

    def write(self, relative_path, text):
        path = self.root / relative_path
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text)
        return path

    def violations(self, source):
        return fixer.find_violations(
            [source],
            fixer.header_index(self.root / "include"),
            self.root,
        )

    def test_detects_and_fixes_multiline_extern(self):
        self.write(
            "include/game/render.h",
            "void drawThing(s32 id, void (*callback)(void));\n",
        )
        source = self.write(
            "src/render_user.c",
            '#include "common.h"\n\n'
            "extern void drawThing(s32 id,\n"
            "                          void (*callback)(void));\n"
            "extern void sourceOnly(void);\n",
        )

        violations = self.violations(source)
        self.assertEqual([item.declaration.name for item in violations], ["drawThing"])

        fixer.fix_file(source, violations)
        self.assertEqual(
            source.read_text(),
            '#include "common.h"\n'
            '#include "game/render.h"\n\n'
            "extern void sourceOnly(void);\n",
        )

    def test_reuses_an_existing_include(self):
        self.write("include/game/task.h", "void runTask(void);\n")
        source = self.write(
            "src/task_user.c",
            '#include "game/task.h"\n\nextern void runTask(void);\n',
        )

        violations = self.violations(source)
        fixer.fix_file(source, violations)

        self.assertEqual(source.read_text(), '#include "game/task.h"\n\n')

    def test_ignores_comments_variables_and_function_pointer_variables(self):
        self.write(
            "include/game/task.h",
            "void runTask(void);\nvoid callbackTarget(void);\n",
        )
        source = self.write(
            "src/task_user.c",
            "/* extern void runTask(void); */\n"
            "extern s32 runTaskCount;\n"
            "extern void (*callbackTarget)(void);\n",
        )

        self.assertEqual(self.violations(source), [])

    def test_ignores_function_pointer_array_variables(self):
        self.write("include/game/task.h", "void callbackTarget(void);\n")
        source = self.write(
            "src/task_user.c",
            "extern void (*callbackTarget[])(void);\n",
        )

        self.assertEqual(self.violations(source), [])

    def test_prefers_already_included_header(self):
        self.write("include/game/one.h", "void runTask(void);\n")
        self.write("include/game/two.h", "void runTask(void);\n")
        source = self.write(
            "src/task_user.c",
            '#include "game/two.h"\n\nextern void runTask(void);\n',
        )

        violations = self.violations(source)
        self.assertEqual(violations[0].selected_header, Path("game/two.h"))

    def test_reports_equal_header_choices_as_ambiguous(self):
        self.write("include/game/one.h", "void runTask(void);\n")
        self.write("include/game/two.h", "void runTask(void);\n")
        source = self.write("src/task_user.c", "extern void runTask(void);\n")

        violations = self.violations(source)
        self.assertIsNone(violations[0].selected_header)
        fixer.fix_file(source, violations)
        self.assertEqual(source.read_text(), "extern void runTask(void);\n")

    def test_prefers_source_corresponding_header(self):
        self.write("include/game/player.h", "void updatePlayer(void);\n")
        self.write("include/game/shared.h", "void updatePlayer(void);\n")
        source = self.write(
            "src/player.c",
            "extern void updatePlayer(void);\n",
        )

        violations = self.violations(source)
        self.assertEqual(violations[0].selected_header, Path("game/player.h"))

    def test_ignores_incompatible_header_signatures(self):
        self.write(
            "include/game/math.h",
            "s32 calculate(void *value);\n"
            "void rotate(void *matrix, s16 x, s16 y);\n",
        )
        source = self.write(
            "src/math_user.c",
            "extern void calculate(void *value);\n"
            "extern void rotate(void *matrix, s16 x, s16 y, ...);\n",
        )

        self.assertEqual(self.violations(source), [])

    def test_does_not_add_header_that_conflicts_with_another_local_extern(self):
        self.write(
            "include/game/math.h",
            "void rotateX(void *matrix, s16 angle);\n"
            "void rotateYX(void *matrix, s16 x, s16 y);\n",
        )
        source = self.write(
            "src/math_user.c",
            "extern void rotateX(void *matrix, s16 angle);\n"
            "extern void rotateYX(void *matrix, s16 x, s16 y, ...);\n",
        )

        self.assertEqual(self.violations(source), [])

    def test_adds_known_header_prerequisites(self):
        self.write(
            "include/PR/os_system.h",
            "void osInitialize(void);\n",
        )
        source = self.write(
            "src/runtime.c",
            "extern void osInitialize(void);\n",
        )

        violations = self.violations(source)
        fixer.fix_file(source, violations)

        self.assertEqual(
            source.read_text(),
            '#include "PR/os_exception.h"\n'
            '#include "PR/os_system.h"\n',
        )

    def test_moves_late_same_file_forward_declaration(self):
        source = self.write(
            "src/sound.c",
            '#include "game/sound.h"\n\n'
            "void firstFunction(void) {\n"
            "}\n\n"
            "extern void enqueue(void *position);\n\n"
            "void enqueue(void *position) {\n"
            "}\n",
        )

        violations = self.violations(source)
        self.assertEqual(len(violations), 1)
        self.assertTrue(violations[0].same_file_forward)

        fixer.fix_file(source, violations)
        self.assertEqual(
            source.read_text(),
            '#include "game/sound.h"\n\n'
            "extern void enqueue(void *position);\n\n"
            "void firstFunction(void) {\n"
            "}\n\n"
            "void enqueue(void *position) {\n"
            "}\n",
        )

    def test_allows_void_pointer_to_match_typed_pointer_header(self):
        self.write(
            "include/game/math.h",
            "void multiply(FixedMatrix3s left, FixedMatrix3s right);\n",
        )
        source = self.write(
            "src/math_user.c",
            "extern void multiply(void *left, void *right);\n",
        )

        violations = self.violations(source)
        self.assertEqual(violations[0].selected_header, Path("game/math.h"))

    def test_accepts_one_compatible_conditional_header_prototype(self):
        self.write(
            "include/game/math.h",
            "#ifdef IMPLEMENTATION\n"
            "void rotate(Matrix matrix, s16 x, s16 y);\n"
            "#else\n"
            "void rotate(Matrix matrix, s16 x, s16 y, ...);\n"
            "#endif\n"
            "void multiply(Matrix left, Matrix right);\n",
        )
        source = self.write(
            "src/math_user.c",
            "extern void rotate(void *matrix, s16 x, s16 y, ...);\n"
            "extern void multiply(void *left, void *right);\n",
        )

        violations = self.violations(source)
        self.assertEqual(
            [violation.declaration.name for violation in violations],
            ["rotate", "multiply"],
        )

    def test_moves_forward_declaration_outside_non_matching_block(self):
        source = self.write(
            "src/compress.c",
            '#include "game/compress.h"\n\n'
            '#pragma GLOBAL_ASM("compress.s")\n'
            "#ifdef NON_MATCHING\n"
            "s32 compress(u8 *source) {\n"
            "    return 0;\n"
            "}\n"
            "#endif\n\n"
            "extern s32 compress(u8 *source);\n",
        )

        violations = self.violations(source)
        fixer.fix_file(source, violations)

        self.assertEqual(
            source.read_text(),
            '#include "game/compress.h"\n\n'
            '#pragma GLOBAL_ASM("compress.s")\n'
            "extern s32 compress(u8 *source);\n\n"
            "#ifdef NON_MATCHING\n"
            "s32 compress(u8 *source) {\n"
            "    return 0;\n"
            "}\n"
            "#endif\n\n",
        )

if __name__ == "__main__":
    unittest.main()
