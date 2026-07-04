#include "common.h"

#define RUMBLE_PATTERN_WRAP_MASK 0xF
#define RUMBLE_PATTERN_FAST 2
#define RUMBLE_PATTERN_FAST_LENGTH 6

typedef u8 ControllerRumblePattern[];

typedef struct {
    /* 0x000 */ u16 controllerIndex;
    /* 0x002 */ char pad2[0x2];
    /* 0x004 */ u8 disabled;
    /* 0x005 */ char pad5[0xF];
    /* 0x014 */ s8 state;
    /* 0x015 */ char pad15[0x5EF];
    /* 0x604 */ s16 rumbleTimer;
    /* 0x606 */ s16 rumblePatternIndex;
    /* 0x608 */ s8 rumblePatternId;
} ControllerRumbleState;

extern ControllerRumblePattern D_800DE550;
extern ControllerRumblePattern D_800DE560;
extern ControllerRumblePattern D_800DE570;
extern ControllerRumblePattern D_800DE578;
extern void func_80000C00(u16);

void func_800830C0(ControllerRumbleState *arg0) {
    s32 patternId;
    s32 fastPatternId;
    u8 *pattern;

    if (arg0->disabled != 0) {
        return;
    }
    if (arg0->state != 0) {
        return;
    }
    if (arg0->rumbleTimer == 0) {
        arg0->rumblePatternIndex = 0;
        return;
    }
    arg0->rumbleTimer = arg0->rumbleTimer - 1;
    patternId = arg0->rumblePatternId;
    fastPatternId = RUMBLE_PATTERN_FAST;
    pattern = D_800DE550;
    if (patternId == 1) {
        pattern = D_800DE560;
    }
    if (fastPatternId == patternId) {
        pattern = D_800DE570;
    }
    if (patternId == 3) {
        pattern = D_800DE578;
    }
    if (fastPatternId == patternId) {
        if (!(arg0->rumblePatternIndex < RUMBLE_PATTERN_FAST_LENGTH)) {
            arg0->rumblePatternIndex = 0;
        }
    }
    if (pattern[arg0->rumblePatternIndex] != 0) {
        func_80000C00(arg0->controllerIndex);
    }
    arg0->rumblePatternIndex = (arg0->rumblePatternIndex + 1) & RUMBLE_PATTERN_WRAP_MASK;
}
