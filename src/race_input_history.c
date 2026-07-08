#include "common.h"
#include "race_input_history.h"

#define RACE_INPUT_HISTORY_LENGTH 0x1194
#define INPUT_MASK_HARD_STEER_X 0xF0000
#define INPUT_MASK_HARD_STEER_Y 0xC000

typedef struct {
    /* 0x0000 */ s32 writeIndex;
    /* 0x0004 */ s32 lastWriteIndex;
    /* 0x0008 */ s8 enabled;
    /* 0x0009 */ char pad9[4];
    /* 0x000D */ s8 stickX[RACE_INPUT_HISTORY_LENGTH];
    /* 0x11A1 */ s8 stickY[RACE_INPUT_HISTORY_LENGTH];
    /* 0x2335 */ u8 buttons[RACE_INPUT_HISTORY_LENGTH];
} RaceInputHistoryBuffer;

extern s16 D_80112186;

extern RaceInputHistoryBuffer *func_80043040(s16 assetId);
extern s32 func_800430D0(void);
void func_80083D80(RaceInputPlayer *player);

#pragma GLOBAL_ASM("asm/nonmatchings/race_input_history/func_80083D80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_input_history/func_8008409C.s")

void func_8008431C(RaceInputPlayer *player) {
    RaceInputHistoryBuffer *history;
    s32 index;
    u32 inputFlags;

    if (player->unk15 == 5) {
        func_80083D80(player);
        return;
    }

    history = func_80043040(D_80112186);
    index = history->writeIndex;
    if (index >= RACE_INPUT_HISTORY_LENGTH) {
        history->enabled = 0;
        return;
    }

    history->stickX[index] = player->stickX;
    history->stickY[history->writeIndex] = player->stickY;
    history->buttons[history->writeIndex] = 0;

    inputFlags = player->inputFlags;
    if (inputFlags & 8) {
        history->buttons[history->writeIndex] |= 1;
        inputFlags = player->inputFlags;
    }
    if (inputFlags & 4) {
        history->buttons[history->writeIndex] |= 2;
        inputFlags = player->inputFlags;
    }
    if (inputFlags & 1) {
        history->buttons[history->writeIndex] |= 8;
        inputFlags = player->inputFlags;
    }
    if (inputFlags & 2) {
        history->buttons[history->writeIndex] |= 4;
        inputFlags = player->inputFlags;
    }
    if (inputFlags & 0x8000) {
        history->buttons[history->writeIndex] |= 0x10;
        inputFlags = player->inputFlags;
    }
    if (inputFlags & 0x4000) {
        history->buttons[history->writeIndex] |= 0x20;
        inputFlags = player->inputFlags;
    }
    if (inputFlags & 0x2000) {
        history->buttons[history->writeIndex] |= 0x40;
    }

    index = (history->writeIndex + 1) & 0xFFFFFFFFFFFFFFFFu;
    history->writeIndex = index;
    history->lastWriteIndex = index;
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_input_history/func_80084510.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_input_history/func_80084730.s")

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
