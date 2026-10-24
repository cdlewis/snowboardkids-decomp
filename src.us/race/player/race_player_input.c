#include "common.h"
#include "game/engine/controller_input.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/asset_manager.h"
#include "game/race/player/race_player_input.h"
#include "game/race/race_replay.h"
#include "game/engine/game_task_scheduler.h"

#define INPUT_MASK_STICK_DIRECTIONS (STICK_UP | STICK_DOWN | STICK_RIGHT | STICK_LEFT)
#define INPUT_MASK_AB_BUTTONS (A_BUTTON | B_BUTTON)

typedef struct {
    /* 0x0 */ u8 buttons;
    /* 0x1 */ s8 stickX;
    /* 0x2 */ s8 stickY;
} RaceInputReplayFrame;

typedef struct {
    /* 0x0000 */ RaceInputReplayFrame inputs[RACE_INPUT_REPLAY_FRAME_COUNT];
} RaceInputReplayHistory;

RacePlayer gRacePlayers[RACE_PLAYER_COUNT];

void recordRaceReplayInputFrame(RacePlayer *player) {
    RaceInputReplayHistory *history;
    s16 frame;
    u32 heldInput;

    history = (RaceInputReplayHistory *)getRelocatableHeapBlockBase(gAssetHandles[0x2B]);
    frame = player->replayFrame;
    if (frame < RACE_INPUT_REPLAY_FRAME_COUNT) {
        history[(u16)player->playerIndex].inputs[frame].stickX = player->stickX;
        history[(u16)player->playerIndex].inputs[player->replayFrame].stickY = player->stickY;
        history[(u16)player->playerIndex].inputs[player->replayFrame].buttons = 0;

        heldInput = player->heldInput;
        if (heldInput & U_CBUTTONS) {
            history[(u16)player->playerIndex].inputs[player->replayFrame].buttons |= RACE_REPLAY_BUTTON_C_UP;
            heldInput = player->heldInput;
        }
        if (heldInput & D_CBUTTONS) {
            history[(u16)player->playerIndex].inputs[player->replayFrame].buttons |= RACE_REPLAY_BUTTON_C_DOWN;
            heldInput = player->heldInput;
        }
        if (heldInput & R_CBUTTONS) {
            history[(u16)player->playerIndex].inputs[player->replayFrame].buttons |= RACE_REPLAY_BUTTON_C_RIGHT;
            heldInput = player->heldInput;
        }
        if (heldInput & L_CBUTTONS) {
            history[(u16)player->playerIndex].inputs[player->replayFrame].buttons |= RACE_REPLAY_BUTTON_C_LEFT;
            heldInput = player->heldInput;
        }
        if (heldInput & A_BUTTON) {
            history[(u16)player->playerIndex].inputs[player->replayFrame].buttons |= RACE_REPLAY_BUTTON_A;
            heldInput = player->heldInput;
        }
        if (heldInput & B_BUTTON) {
            history[(u16)player->playerIndex].inputs[player->replayFrame].buttons |= RACE_REPLAY_BUTTON_B;
            heldInput = player->heldInput;
        }
        if (heldInput & Z_TRIG) {
            history[(u16)player->playerIndex].inputs[player->replayFrame].buttons |= RACE_REPLAY_BUTTON_Z;
        }

        player->replayFrame++;
    }
}

void playRaceReplayInputFrame(RacePlayer *player) {
    RaceInputReplayHistory *history;

    history = (RaceInputReplayHistory *)getRelocatableHeapBlockBase(gAssetHandles[0x2B]);
    if (player->replayFrame < RACE_INPUT_REPLAY_FRAME_COUNT) {
        player->stickX = history[player->playerIndex].inputs[player->replayFrame].stickX;
        player->stickY = history[player->playerIndex].inputs[player->replayFrame].stickY;
        player->heldInput = 0;
        if (history[player->playerIndex].inputs[player->replayFrame].buttons & RACE_REPLAY_BUTTON_C_UP) {
            player->heldInput |= U_CBUTTONS;
        }
        if (history[player->playerIndex].inputs[player->replayFrame].buttons & RACE_REPLAY_BUTTON_C_DOWN) {
            player->heldInput |= D_CBUTTONS;
        }
        if (history[player->playerIndex].inputs[player->replayFrame].buttons & RACE_REPLAY_BUTTON_C_RIGHT) {
            player->heldInput |= R_CBUTTONS;
        }
        if (history[player->playerIndex].inputs[player->replayFrame].buttons & RACE_REPLAY_BUTTON_C_LEFT) {
            player->heldInput |= L_CBUTTONS;
        }
        if (history[player->playerIndex].inputs[player->replayFrame].buttons & RACE_REPLAY_BUTTON_A) {
            player->heldInput |= A_BUTTON;
        }
        if (history[player->playerIndex].inputs[player->replayFrame].buttons & RACE_REPLAY_BUTTON_B) {
            player->heldInput |= B_BUTTON;
        }
        if (history[player->playerIndex].inputs[player->replayFrame].buttons & RACE_REPLAY_BUTTON_Z) {
            player->heldInput |= Z_TRIG;
        }
        player->replayFrame++;
    } else {
        player->stickX = 0;
        player->stickY = 0;
        player->heldInput = 0;
    }
}

void recordRaceInputHistoryFrame(RacePlayer *player) {
    RaceInputHistoryBuffer *history;
    s32 index;
    u32 heldInput;

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

    heldInput = player->heldInput;
    if (heldInput & U_CBUTTONS) {
        history->buttons[history->writeIndex] |= RACE_REPLAY_BUTTON_C_UP;
        heldInput = player->heldInput;
    }
    if (heldInput & D_CBUTTONS) {
        history->buttons[history->writeIndex] |= RACE_REPLAY_BUTTON_C_DOWN;
        heldInput = player->heldInput;
    }
    if (heldInput & R_CBUTTONS) {
        history->buttons[history->writeIndex] |= RACE_REPLAY_BUTTON_C_RIGHT;
        heldInput = player->heldInput;
    }
    if (heldInput & L_CBUTTONS) {
        history->buttons[history->writeIndex] |= RACE_REPLAY_BUTTON_C_LEFT;
        heldInput = player->heldInput;
    }
    if (heldInput & A_BUTTON) {
        history->buttons[history->writeIndex] |= RACE_REPLAY_BUTTON_A;
        heldInput = player->heldInput;
    }
    if (heldInput & B_BUTTON) {
        history->buttons[history->writeIndex] |= RACE_REPLAY_BUTTON_B;
        heldInput = player->heldInput;
    }
    if (heldInput & Z_TRIG) {
        history->buttons[history->writeIndex] |= RACE_REPLAY_BUTTON_Z;
    }

    index = (history->writeIndex + 1) & 0xFFFFFFFFFFFFFFFFu;
    history->writeIndex = index;
    history->lastWriteIndex = index;
}

void playRaceInputHistoryFrame(RacePlayer *player) {
    RaceInputHistoryBuffer *history;
    RacePlayer *playerAlias;
    s32 replayButtons;
    s32 index;
    u32 buttonIndex;
    u8 *buttonHistory;

    playerAlias = player;
    if (playerAlias->replayInputSource == RACE_PLAYER_INPUT_REPLAY) {
        playRaceReplayInputFrame(playerAlias);
        return;
    }

    if (playerAlias->replayInputSource == RACE_PLAYER_INPUT_HISTORY_07) {
        history = (RaceInputHistoryBuffer *)getRelocatableHeapBlockBase(gAssetHandles[0x7]);
    }

    if (playerAlias->replayInputSource == RACE_PLAYER_INPUT_HISTORY_2B) {
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
        playerAlias->heldInput = 0;

        replayButtons = history->buttons[history->writeIndex] ^ 0;
        if (replayButtons & RACE_REPLAY_BUTTON_C_UP) {
            playerAlias->heldInput |= U_CBUTTONS;
            buttonIndex = history->writeIndex;
            replayButtons = buttonHistory[buttonIndex];
        }
        if (replayButtons & RACE_REPLAY_BUTTON_C_DOWN) {
            playerAlias->heldInput |= D_CBUTTONS;
            replayButtons = history->buttons[history->writeIndex];
            if (history->stickY) {}
        }
        if (replayButtons & RACE_REPLAY_BUTTON_C_RIGHT) {
            playerAlias->heldInput |= R_CBUTTONS;
            replayButtons = history->buttons[history->writeIndex];
        }
        if (replayButtons & RACE_REPLAY_BUTTON_C_LEFT) {
            playerAlias->heldInput |= L_CBUTTONS;
            replayButtons = history->buttons[history->writeIndex];
        }
        if (replayButtons & RACE_REPLAY_BUTTON_A) {
            playerAlias->heldInput |= A_BUTTON;
            replayButtons = history->buttons[history->writeIndex];
        }
        if (replayButtons & RACE_REPLAY_BUTTON_B) {
            playerAlias->heldInput |= B_BUTTON;
            replayButtons = history->buttons[history->writeIndex];
        }

        if (1) {
            if (replayButtons & RACE_REPLAY_BUTTON_Z) {
                playerAlias->heldInput |= Z_TRIG;
            }
            (*history).writeIndex++;
        }
    }
}

void updateRacePlayerInput(RacePlayer *player) {
    u16 index;

    if (!(gMenuFlowState & 1) && !(player->stateFlags & 0x40)) {
        index = player->playerIndex;
        player->previousInput = gRacePlayers[index].heldInput;

        if (player->replayInputSource == RACE_PLAYER_INPUT_LIVE) {
            player->heldInput = gPlayerInputHeld[index];
            player->stickX = gPlayerStickX[index];
            player->stickY = gPlayerStickY[index];
            if (player->unk15 != 0) {
                recordRaceInputHistoryFrame(player);
            }
        } else {
            playRaceInputHistoryFrame(player);
        }

        player->heldInput &= ~(STICK_UP | STICK_DOWN | STICK_RIGHT | STICK_LEFT);
        if (player->stickX >= 0x1B) {
            player->heldInput |= STICK_RIGHT;
        }
        if (player->stickX < -0x1A) {
            player->heldInput |= STICK_LEFT;
        }
        if (player->stickY >= 0x1B) {
            player->heldInput |= STICK_UP;
        }
        if (player->stickY < -0x1A) {
            player->heldInput |= STICK_DOWN;
        }
        if (player->stickX < 8) {
            player->heldInput &= ~STICK_RIGHT;
        }
        if (player->stickX >= -7) {
            player->heldInput &= ~STICK_LEFT;
        }
        if (player->stickY < 8) {
            player->heldInput &= ~STICK_UP;
        }
        if (player->stickY >= -7) {
            player->heldInput &= ~STICK_DOWN;
        }

        player->pressedInput = player->heldInput & ~player->previousInput;
        return;
    }

    player->previousInput = 0;
    player->heldInput = 0;
    player->pressedInput = 0;
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
    temp_v0 = player->heldInput & ~player->previousInput;
    if (temp_v0 & INPUT_MASK_STICK_DIRECTIONS) {
        var_v1 = 1;
    }
    if (temp_v0 & INPUT_MASK_AB_BUTTONS) {
        var_v1++;
    }
    return var_v1;
}
