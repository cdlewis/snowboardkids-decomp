#include "common.h"
#include "asset_decompression.h"
#include "race_input_history.h"

#define RACE_INPUT_HISTORY_LENGTH 0x1194
#define RACE_INPUT_REPLAY_FRAME_COUNT 0x960
#define INPUT_MASK_HARD_STEER_X 0xF0000
#define INPUT_MASK_HARD_STEER_Y 0xC000

typedef struct {
    /* 0x0 */ u8 buttons;
    /* 0x1 */ s8 stickX;
    /* 0x2 */ s8 stickY;
} RaceInputReplayFrame;

typedef struct {
    /* 0x0000 */ RaceInputReplayFrame inputs[RACE_INPUT_REPLAY_FRAME_COUNT];
} RaceInputReplayHistory;

typedef struct {
    /* 0x0000 */ s32 writeIndex;
    /* 0x0004 */ s32 lastWriteIndex;
    /* 0x0008 */ s8 enabled;
    /* 0x0009 */ char pad9[4];
    /* 0x000D */ s8 stickX[RACE_INPUT_HISTORY_LENGTH];
    /* 0x11A1 */ s8 stickY[RACE_INPUT_HISTORY_LENGTH];
    /* 0x2335 */ u8 buttons[RACE_INPUT_HISTORY_LENGTH];
} RaceInputHistoryBuffer;

extern s16 D_8011213E;
extern s16 D_80112186;

extern void *func_80043040(s16 assetId);
extern u32 D_80121E04[][0x183];
extern s32 D_801235B4;
extern u32 D_80123758[];
extern s8 D_80123788[];
extern s8 D_8012378C[];

void func_80083D80(RaceInputPlayer *player);
void func_8008409C(RaceInputPlayer *player);
void func_8008431C(RaceInputPlayer *player);
void func_80084510(RaceInputPlayer *player);

void func_80083D80(RaceInputPlayer *player) {
    RaceInputReplayHistory *history;
    s16 frame;
    u32 inputFlags;

    history = func_80043040(D_80112186);
    frame = player->replayFrame;
    if (frame < RACE_INPUT_REPLAY_FRAME_COUNT) {
        history[(u16) player->playerIndex].inputs[frame].stickX = player->stickX;
        history[(u16) player->playerIndex].inputs[player->replayFrame].stickY = player->stickY;
        history[(u16) player->playerIndex].inputs[player->replayFrame].buttons = 0;

        inputFlags = player->inputFlags;
        if (inputFlags & 8) {
            history[(u16) player->playerIndex].inputs[player->replayFrame].buttons |= 1;
            inputFlags = player->inputFlags;
        }
        if (inputFlags & 4) {
            history[(u16) player->playerIndex].inputs[player->replayFrame].buttons |= 2;
            inputFlags = player->inputFlags;
        }
        if (inputFlags & 1) {
            history[(u16) player->playerIndex].inputs[player->replayFrame].buttons |= 8;
            inputFlags = player->inputFlags;
        }
        if (inputFlags & 2) {
            history[(u16) player->playerIndex].inputs[player->replayFrame].buttons |= 4;
            inputFlags = player->inputFlags;
        }
        if (inputFlags & 0x8000) {
            history[(u16) player->playerIndex].inputs[player->replayFrame].buttons |= 0x10;
            inputFlags = player->inputFlags;
        }
        if (inputFlags & 0x4000) {
            history[(u16) player->playerIndex].inputs[player->replayFrame].buttons |= 0x20;
            inputFlags = player->inputFlags;
        }
        if (inputFlags & 0x2000) {
            history[(u16) player->playerIndex].inputs[player->replayFrame].buttons |= 0x40;
        }

        player->replayFrame++;
    }
}

// func_8008409C best match: 86.789% (base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_input_history/func_8008409C.s")

#ifdef NON_MATCHING
typedef struct {
    /* 0x0000 */ s8 bytes[0x1C20];
} RaceReplayInputHistoryPlayer;

#define PLAYER_INDEX(player) (*(volatile u16 *)&(player)->playerIndex)

void func_8008409C(RaceInputPlayer *player) {
    RaceReplayInputHistoryPlayer *history;
    s16 replayFrame;
    u8 buttons;
    s32 replayOffset;

    history = (RaceReplayInputHistoryPlayer *) func_80043040(D_80112186);
    replayFrame = player->replayFrame;
    if (replayFrame < 0x960) {
        replayOffset = (replayFrame * 4) - replayFrame;
        player->stickX = history[player->playerIndex].bytes[replayOffset + 1];
        player->inputFlags = 0;
        player->stickY = history[player->playerIndex].bytes[replayOffset + 2];
        buttons = (u8) history[player->playerIndex].bytes[replayOffset];
        if (buttons & 1) {
            player->inputFlags = 8;
            replayFrame = player->replayFrame;
            replayOffset = (replayFrame * 4) - replayFrame;
            buttons = (u8) history[PLAYER_INDEX(player)].bytes[replayOffset];
        }
        if (buttons & 2) {
            player->inputFlags |= 4;
            replayFrame = player->replayFrame;
            replayOffset = (replayFrame * 4) - replayFrame;
            buttons = (u8) history[PLAYER_INDEX(player)].bytes[replayOffset];
        }
        if (buttons & 8) {
            player->inputFlags |= 1;
            replayFrame = player->replayFrame;
            replayOffset = (replayFrame * 4) - replayFrame;
            buttons = (u8) history[PLAYER_INDEX(player)].bytes[replayOffset];
        }
        if (buttons & 4) {
            player->inputFlags |= 2;
            replayFrame = player->replayFrame;
            replayOffset = (replayFrame * 4) - replayFrame;
            buttons = (u8) history[PLAYER_INDEX(player)].bytes[replayOffset];
        }
        if (buttons & 0x10) {
            player->inputFlags |= 0x8000;
            replayFrame = player->replayFrame;
            replayOffset = (replayFrame * 4) - replayFrame;
            buttons = (u8) history[PLAYER_INDEX(player)].bytes[replayOffset];
        }
        if (buttons & 0x20) {
            player->inputFlags |= 0x4000;
            replayFrame = player->replayFrame;
            replayOffset = (replayFrame * 4) - replayFrame;
            buttons = (u8) history[PLAYER_INDEX(player)].bytes[replayOffset];
        }
        if (buttons & 0x40) {
            player->inputFlags |= 0x2000;
        }
        player->replayFrame++;
        return;
    }

    player->stickX = 0;
    player->stickY = 0;
    player->inputFlags = 0;
}

#undef PLAYER_INDEX
#endif

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

// func_80084510 best match: 98.763% (base_9.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_input_history/func_80084510.s")

#ifdef NON_MATCHING
void func_80084510(RaceInputPlayer *player) {
    RaceInputHistoryBuffer *history;
    s32 index;
    s8 replayInputSource;
    u8 buttons;

    replayInputSource = player->replayInputSource;
    if (replayInputSource == 5) {
        func_8008409C(player);
        return;
    }

    if (replayInputSource == 1) {
        history = func_80043040(D_8011213E);
        replayInputSource = player->replayInputSource;
    }

    if (replayInputSource == 2) {
        history = func_80043040(D_80112186);
    }

    if (history->enabled == 0) {
        return;
    }

    index = history->writeIndex;
    if (index < RACE_INPUT_HISTORY_LENGTH) {
        player->stickX = history->stickX[index];
        player->stickY = history->stickY[history->writeIndex];
        goto dummy_label;
dummy_label:
        player->inputFlags = 0;

        buttons = history->buttons[history->writeIndex] ^ 0;
        if (buttons & 1) {
            player->inputFlags = 8;
            buttons = history->buttons[history->writeIndex];
        }
        if (buttons & 2) {
            player->inputFlags |= 4;
            buttons = history->buttons[history->writeIndex];
        }
        if (buttons & 8) {
            player->inputFlags |= 1;
            buttons = history->buttons[history->writeIndex];
        }
        if (buttons & 4) {
            player->inputFlags |= 2;
            buttons = history->buttons[history->writeIndex];
        }
        if (buttons & 0x10) {
            player->inputFlags |= 0x8000;
            buttons = history->buttons[history->writeIndex];
        }
        if (buttons & 0x20) {
            player->inputFlags |= 0x4000;
            buttons = history->buttons[history->writeIndex];
        }
        if (buttons & 0x40) {
            player->inputFlags |= 0x2000;
        }

        history->writeIndex++;
    }
}
#endif

void func_80084730(RaceInputPlayer *player) {
    u16 index;

    if (!(D_801235B4 & 1) && !(player->stateFlags & 0x40)) {
        index = player->playerIndex;
        player->disabledInputFlags = D_80121E04[index][0];

        if (player->replayInputSource == 0) {
            player->inputFlags = D_80123758[index];
            player->stickX = D_80123788[index];
            player->stickY = D_8012378C[index];
            if (player->unk15 != 0) {
                func_8008431C(player);
            }
        } else {
            func_80084510(player);
        }

        player->inputFlags &= 0xFFF0FFFF;
        if (player->stickX >= 0x1B) {
            player->inputFlags |= 0x40000;
        }
        if (player->stickX < -0x1A) {
            player->inputFlags |= 0x80000;
        }
        if (player->stickY >= 0x1B) {
            player->inputFlags |= 0x10000;
        }
        if (player->stickY < -0x1A) {
            player->inputFlags |= 0x20000;
        }
        if (player->stickX < 8) {
            player->inputFlags &= 0xFFFBFFFF;
        }
        if (player->stickX >= -7) {
            player->inputFlags &= 0xFFF7FFFF;
        }
        if (player->stickY < 8) {
            player->inputFlags &= 0xFFFEFFFF;
        }
        if (player->stickY >= -7) {
            player->inputFlags &= 0xFFFDFFFF;
        }

        player->currentInputFlags = player->inputFlags & ~player->disabledInputFlags;
        return;
    }

    player->disabledInputFlags = 0;
    player->inputFlags = 0;
    player->currentInputFlags = 0;
    player->stickX = 0;
    player->stickY = 0;
}

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
