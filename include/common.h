#ifndef COMMON_H
#define COMMON_H

#include "include_asm.h"
#include "compiler_diagnostics.h"

/*
 * Pull in the libultra headers project-wide so every translation unit has the
 * fixed-width types (ultratypes.h), the _SHIFTL/_SHIFTR macros, the Gfx
 * display-list union, and the gSP / gDP command macros. mbi.h defines the
 * shift macros before including gbi.h (which needs them), so the order here
 * resolves cleanly without any per-file guards.
 */
#include <PR/mbi.h>

typedef struct Vec3i {
    s32 x;
    s32 y;
    s32 z;
} Vec3i;

int sprintf(char *buffer, const char *format, ...);

#define STICK_RIGHT 0x40000
#define STICK_LEFT 0x80000
#define STICK_UP 0x10000
#define STICK_DOWN 0x20000

#endif
