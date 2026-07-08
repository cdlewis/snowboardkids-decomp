#include "controller_rumble.h"
#include "main_menu.h"

#define RUMBLE_PATTERN_WRAP_MASK 0xF
#define RUMBLE_PATTERN_FAST 2
#define RUMBLE_PATTERN_FAST_LENGTH 6

typedef u8 ControllerRumblePattern[];

extern ControllerRumblePattern D_800DE550;
extern ControllerRumblePattern D_800DE560;
extern ControllerRumblePattern D_800DE570;
extern ControllerRumblePattern D_800DE578;

void func_800830C0(RaceInputPlayer *arg0) {
    s32 patternId;
    s32 fastPatternId;
    u8 *pattern;

    if (arg0->unk4 != 0) {
        return;
    }
    if (arg0->soundDisabled != 0) {
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
        func_80000C00(arg0->playerIndex);
    }
    arg0->rumblePatternIndex = (arg0->rumblePatternIndex + 1) & RUMBLE_PATTERN_WRAP_MASK;
}
