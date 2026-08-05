#ifndef FONT_ENCODING_H
#define FONT_ENCODING_H

/*
 * Build-time encoding for MenuGlyphScript text.
 *
 * Source files that include this header may write readable text as:
 *
 *     MenuGlyphScript text[] = { _("{COLOR:2}HELLO WORLD") };
 *
 * tools/textconv.py replaces the _() expression with 16-bit glyph values
 * before the C compiler runs. An END command is appended automatically.
 * Supported directives are documented in tools/charmap.txt.
 */
#define _(text) 0
#define _FRAGMENT(text) 0

#endif
