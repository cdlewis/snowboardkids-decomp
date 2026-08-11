#include "common.h"
#include "game/engine/controller_input.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/asset_manager.h"
#include "game/race/player/race_player_input.h"
#include "game/race/race_replay.h"

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

RacePlayer gRacePlayers[RACE_PLAYER_COUNT];

extern s32 gMenuFlowState;

void recordRaceReplayInputFrame(RacePlayer *player) {
    RaceInputReplayHistory *history;
    s16 frame;
    u32 inputFlags;

    history = (RaceInputReplayHistory *)getRelocatableHeapBlockBase(gAssetHandles[0x2B]);
    frame = player->replayFrame;
    if (frame < RACE_INPUT_REPLAY_FRAME_COUNT) {
        history[(u16)player->playerIndex].inputs[frame].stickX = player->stickX;
        history[(u16)player->playerIndex].inputs[player->replayFrame].stickY = player->stickY;
        history[(u16)player->playerIndex].inputs[player->replayFrame].buttons = 0;

        inputFlags = player->inputFlags;
        if (inputFlags & 8) {
            history[(u16)player->playerIndex].inputs[player->replayFrame].buttons |= 1;
            inputFlags = player->inputFlags;
        }
        if (inputFlags & 4) {
            history[(u16)player->playerIndex].inputs[player->replayFrame].buttons |= 2;
            inputFlags = player->inputFlags;
        }
        if (inputFlags & 1) {
            history[(u16)player->playerIndex].inputs[player->replayFrame].buttons |= 8;
            inputFlags = player->inputFlags;
        }
        if (inputFlags & 2) {
            history[(u16)player->playerIndex].inputs[player->replayFrame].buttons |= 4;
            inputFlags = player->inputFlags;
        }
        if (inputFlags & 0x8000) {
            history[(u16)player->playerIndex].inputs[player->replayFrame].buttons |= 0x10;
            inputFlags = player->inputFlags;
        }
        if (inputFlags & 0x4000) {
            history[(u16)player->playerIndex].inputs[player->replayFrame].buttons |= 0x20;
            inputFlags = player->inputFlags;
        }
        if (inputFlags & 0x2000) {
            history[(u16)player->playerIndex].inputs[player->replayFrame].buttons |= 0x40;
        }

        player->replayFrame++;
    }
}

// playRaceReplayInputFrame best match: 97.956% (base_39.c)
// Current campaign candidate: 50 register-only word differences, exact through instruction 32.
#pragma GLOBAL_ASM("asm/nonmatchings/race/player/race_player_input/playRaceReplayInputFrame.s")

#ifdef NON_MATCHING
typedef struct {
    /* 0x0000 */ s8 bytes[0x1C20];
} RaceReplayInputHistoryPlayer;

#define PLAYER_INDEX(player) (*(volatile u16 *)&(player)->playerIndex)

void playRaceReplayInputFrame(RacePlayer *player) {
    RaceReplayInputHistoryPlayer *history;
    s16 replayFrame;
    u64 newVar = 0xFF;
    u8 buttons;
    s32 replayOffset;
    s8 stickX;

    history = (RaceReplayInputHistoryPlayer *)getRelocatableHeapBlockBase(gAssetHandles[0x2B]);
    replayFrame = player->replayFrame;
    do { if (replayFrame < 0x960) { stickX = ((RaceInputReplayHistory *)history)[(u16)player->playerIndex].inputs[replayFrame].stickX; player->stickX = stickX; player->stickY = ((RaceInputReplayHistory *)history)[(u16)player->playerIndex].inputs[replayFrame].stickY; player->inputFlags = 0; buttons = ((RaceInputReplayHistory *)history)[(u16)player->playerIndex].inputs[replayFrame].buttons; if (buttons & 1) { player->inputFlags |= 8; replayFrame = player->replayFrame; replayOffset = (replayFrame * 4) - replayFrame; buttons = (u8)history[PLAYER_INDEX(player)].bytes[replayOffset]; } if ((((((((buttons & 2) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) { player->inputFlags |= 4; replayFrame = player->replayFrame; replayOffset = (replayFrame * 4) - replayFrame; buttons = (u8)history[PLAYER_INDEX(player)].bytes[replayOffset]; } if ((((((((buttons & 8) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) { player->inputFlags |= 1; replayFrame = player->replayFrame; replayOffset = (replayFrame * 4) - replayFrame; buttons = (u8)history[PLAYER_INDEX(player)].bytes[replayOffset]; } if ((((((((buttons & 4) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) { player->inputFlags |= 2; replayFrame = player->replayFrame; replayOffset = (replayFrame * 4) - replayFrame; buttons = (u8)history[PLAYER_INDEX(player)].bytes[replayOffset]; if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} if (1 != 0) {} } if (buttons & 0x10) { player->inputFlags = player->inputFlags | 0x8000; replayFrame = player->replayFrame; replayOffset = (replayFrame * 4) - replayFrame; buttons = (u8)history[PLAYER_INDEX(player)].bytes[replayOffset]; } if ((((((((buttons & 0x20) & 0xFF) & 0xFF) & 0xFF) & newVar) & 0xFF) & newVar) & newVar) { player->inputFlags |= 0x4000; replayFrame = player->replayFrame; replayOffset = (replayFrame * 4) - replayFrame; buttons = (u8)history[PLAYER_INDEX(player)].bytes[replayOffset]; } if ((((((((buttons & 0x40) & 0xFF) & 0xFF) & newVar) & newVar) & 0xFF) & newVar) & 0xFF) { player->inputFlags |= 0x2000; replayFrame = player->replayFrame; } player->replayFrame = replayFrame + 1; return; } player->stickX = 0; player->stickY = 0; } while (0);
    player->inputFlags = 0;
}

#undef PLAYER_INDEX
#endif

void recordRaceInputHistoryFrame(RacePlayer *player) {
    RaceInputHistoryBuffer *history;
    s32 index;
    u32 inputFlags;

    if (player->unk15 == 5) {
        recordRaceReplayInputFrame(player);
        return;
    }

    history = (RaceInputHistoryBuffer *)getRelocatableHeapBlockBase(gAssetHandles[0x2B]);
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

void playRaceInputHistoryFrame(RacePlayer *player) {
    RaceInputHistoryBuffer *history;
    RacePlayer *playerAlias;
    s32 buttons;
    s32 index;
    u32 buttonIndex;
    u8 *buttonHistory;

    playerAlias = player;
    if (playerAlias->replayInputSource == 5) {
        playRaceReplayInputFrame(playerAlias);
        return;
    }

    if (playerAlias->replayInputSource == 1) {
        history = (RaceInputHistoryBuffer *)getRelocatableHeapBlockBase(gAssetHandles[0x7]);
    }

    if (playerAlias->replayInputSource == 2) {
        history = (RaceInputHistoryBuffer *)getRelocatableHeapBlockBase(gAssetHandles[0x2B]);
    }

    if (history->enabled == 0) {
        return;
    }

    buttonHistory = history->buttons;
    index = history->writeIndex;
    if (index < RACE_INPUT_HISTORY_LENGTH) {
        playerAlias->stickX = history->stickX[index];
        playerAlias->stickY = history->stickY[history->writeIndex];
        goto dummy_label;
    dummy_label:
        playerAlias->inputFlags = 0;

        buttons = history->buttons[history->writeIndex] ^ 0;
        if (buttons & 1) {
            playerAlias->inputFlags |= 8;
            buttonIndex = history->writeIndex;
            buttons = buttonHistory[buttonIndex];
        }
        if (buttons & 2) {
            playerAlias->inputFlags |= 4;
            buttons = history->buttons[history->writeIndex];
            if (history->stickY) {}
        }
        if (buttons & 8) {
            playerAlias->inputFlags |= 1;
            buttons = history->buttons[history->writeIndex];
        }
        if (buttons & 4) {
            playerAlias->inputFlags |= 2;
            buttons = history->buttons[history->writeIndex];
        }
        if (buttons & 0x10) {
            playerAlias->inputFlags |= 0x8000;
            buttons = history->buttons[history->writeIndex];
        }
        if (buttons & 0x20) {
            playerAlias->inputFlags |= 0x4000;
            buttons = history->buttons[history->writeIndex];
        }

        if (1) {
            if (buttons & 0x40) {
                playerAlias->inputFlags |= 0x2000;
            }
            (*history).writeIndex++;
        }
    }
}

void updateRacePlayerInput(RacePlayer *player) {
    u16 index;

    if (!(gMenuFlowState & 1) && !(player->stateFlags & 0x40)) {
        index = player->playerIndex;
        player->disabledInputFlags = gRacePlayers[index].inputFlags;

        if (player->replayInputSource == 0) {
            player->inputFlags = gPlayerInputHeld[index];
            player->stickX = gPlayerStickX[index];
            player->stickY = gPlayerStickY[index];
            if (player->unk15 != 0) {
                recordRaceInputHistoryFrame(player);
            }
        } else {
            playRaceInputHistoryFrame(player);
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

s32 getRaceInputTimerDecrementBonus(RacePlayer *player) {
    s32 sp18;
    s32 var_v1;
    s32 temp_v0;

    var_v1 = 0;
    if (player->isCpu != 0) {
        sp18 = 0;
        var_v1 = sp18;
        if (!(randomNextMain() & 3)) {
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
