#!/usr/bin/env python3
"""Tests for find_struct_unions.py."""

import unittest

from tools.find_struct_unions import find_struct_union_lines


class FindStructUnionLinesTests(unittest.TestCase):
    def test_finds_anonymous_union_member(self):
        source = """\
typedef struct Example {
    int first;
    union {
        int integer;
        float decimal;
    } value;
} Example;
"""
        self.assertEqual(find_struct_union_lines(source), [3])

    def test_finds_tagged_union_member(self):
        source = """\
struct Example {
    union Value {
        int integer;
        float decimal;
    } value;
};
"""
        self.assertEqual(find_struct_union_lines(source), [2])

    def test_finds_union_typedef_member(self):
        source = """\
typedef union {
    int integer;
    float decimal;
} Value;

typedef struct {
    Value value;
} Example;
"""
        self.assertEqual(find_struct_union_lines(source), [7])

    def test_finds_tagged_union_reference(self):
        source = """\
union Value {
    int integer;
    float decimal;
};

struct Example {
    union Value value;
};
"""
        self.assertEqual(find_struct_union_lines(source), [7])

    def test_ignores_standalone_union(self):
        source = """\
typedef union Value {
    int integer;
    float decimal;
} Value;
"""
        self.assertEqual(find_struct_union_lines(source), [])

    def test_ignores_union_text_in_comments_and_literals(self):
        source = """\
/* struct Fake { union { int value; } member; }; */
const char *description = "struct Fake { union { int value; } member; };";
"""
        self.assertEqual(find_struct_union_lines(source), [])

    def test_does_not_treat_struct_parameter_as_definition(self):
        source = """\
void useValue(struct Value *value) {
    union Local {
        int integer;
        float decimal;
    } local;
}
"""
        self.assertEqual(find_struct_union_lines(source), [])


if __name__ == "__main__":
    unittest.main()
