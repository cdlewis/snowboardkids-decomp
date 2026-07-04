#include "common.h"
#include "race_input_history.h"

#define INPUT_MASK_HARD_STEER_X 0xF0000
#define INPUT_MASK_HARD_STEER_Y 0xC000

#pragma GLOBAL_ASM("asm/nonmatchings/race_input_history/func_80083D80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_input_history/func_8008409C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_input_history/func_8008431C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_input_history/func_80084510.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_input_history/func_80084730.s")

extern s32 func_800430D0(void);

s32 func_80084958(RaceInputPlayer *player) {
    s32 sp18;
    s32 var_v1;
    s32 temp_v0;

    var_v1 = 0;
    if (player->unk4 != 0) {
        sp18 = 0;
        var_v1 = sp18;
        if (!(func_800430D0() & 3)) {
            return 1;
        }
    }
    temp_v0 = player->inputFlags & ~player->disabledInputFlags;
    if (temp_v0 & INPUT_MASK_HARD_STEER_X) {
        var_v1 = 1;
    }
    if (temp_v0 & INPUT_MASK_HARD_STEER_Y) {
        var_v1++;
    }
    return var_v1;
}
