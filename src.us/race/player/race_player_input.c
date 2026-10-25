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
    frame = player->state.replayFrame;
    if (frame < RACE_INPUT_REPLAY_FRAME_COUNT) {
        history[(u16)player->playerIndex].inputs[frame].stickX = player->state.stickX;
        history[(u16)player->playerIndex].inputs[player->state.replayFrame].stickY = player->state.stickY;
        history[(u16)player->playerIndex].inputs[player->state.replayFrame].buttons = 0;

        heldInput = player->state.heldInput;
        if (heldInput & U_CBUTTONS) {
            history[(u16)player->playerIndex].inputs[player->state.replayFrame].buttons |= RACE_REPLAY_BUTTON_C_UP;
            heldInput = player->state.heldInput;
        }
        if (heldInput & D_CBUTTONS) {
            history[(u16)player->playerIndex].inputs[player->state.replayFrame].buttons |= RACE_REPLAY_BUTTON_C_DOWN;
            heldInput = player->state.heldInput;
        }
        if (heldInput & R_CBUTTONS) {
            history[(u16)player->playerIndex].inputs[player->state.replayFrame].buttons |= RACE_REPLAY_BUTTON_C_RIGHT;
            heldInput = player->state.heldInput;
        }
        if (heldInput & L_CBUTTONS) {
            history[(u16)player->playerIndex].inputs[player->state.replayFrame].buttons |= RACE_REPLAY_BUTTON_C_LEFT;
            heldInput = player->state.heldInput;
        }
        if (heldInput & A_BUTTON) {
            history[(u16)player->playerIndex].inputs[player->state.replayFrame].buttons |= RACE_REPLAY_BUTTON_A;
            heldInput = player->state.heldInput;
        }
        if (heldInput & B_BUTTON) {
            history[(u16)player->playerIndex].inputs[player->state.replayFrame].buttons |= RACE_REPLAY_BUTTON_B;
            heldInput = player->state.heldInput;
        }
        if (heldInput & Z_TRIG) {
            history[(u16)player->playerIndex].inputs[player->state.replayFrame].buttons |= RACE_REPLAY_BUTTON_Z;
        }

        player->state.replayFrame++;
    }
}

void playRaceReplayInputFrame(RacePlayer *player) {
    RaceInputReplayHistory *history;

    history = (RaceInputReplayHistory *)getRelocatableHeapBlockBase(gAssetHandles[0x2B]);
    if (player->state.replayFrame < RACE_INPUT_REPLAY_FRAME_COUNT) {
        player->state.stickX = history[player->playerIndex].inputs[player->state.replayFrame].stickX;
        player->state.stickY = history[player->playerIndex].inputs[player->state.replayFrame].stickY;
        player->state.heldInput = 0;
        if (history[player->playerIndex].inputs[player->state.replayFrame].buttons & RACE_REPLAY_BUTTON_C_UP) {
            player->state.heldInput |= U_CBUTTONS;
        }
        if (history[player->playerIndex].inputs[player->state.replayFrame].buttons & RACE_REPLAY_BUTTON_C_DOWN) {
            player->state.heldInput |= D_CBUTTONS;
        }
        if (history[player->playerIndex].inputs[player->state.replayFrame].buttons & RACE_REPLAY_BUTTON_C_RIGHT) {
            player->state.heldInput |= R_CBUTTONS;
        }
        if (history[player->playerIndex].inputs[player->state.replayFrame].buttons & RACE_REPLAY_BUTTON_C_LEFT) {
            player->state.heldInput |= L_CBUTTONS;
        }
        if (history[player->playerIndex].inputs[player->state.replayFrame].buttons & RACE_REPLAY_BUTTON_A) {
            player->state.heldInput |= A_BUTTON;
        }
        if (history[player->playerIndex].inputs[player->state.replayFrame].buttons & RACE_REPLAY_BUTTON_B) {
            player->state.heldInput |= B_BUTTON;
        }
        if (history[player->playerIndex].inputs[player->state.replayFrame].buttons & RACE_REPLAY_BUTTON_Z) {
            player->state.heldInput |= Z_TRIG;
        }
        player->state.replayFrame++;
    } else {
        player->state.stickX = 0;
        player->state.stickY = 0;
        player->state.heldInput = 0;
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

    history->stickX[index] = player->state.stickX;
    history->stickY[history->writeIndex] = player->state.stickY;
    history->buttons[history->writeIndex] = 0;

    heldInput = player->state.heldInput;
    if (heldInput & U_CBUTTONS) {
        history->buttons[history->writeIndex] |= RACE_REPLAY_BUTTON_C_UP;
        heldInput = player->state.heldInput;
    }
    if (heldInput & D_CBUTTONS) {
        history->buttons[history->writeIndex] |= RACE_REPLAY_BUTTON_C_DOWN;
        heldInput = player->state.heldInput;
    }
    if (heldInput & R_CBUTTONS) {
        history->buttons[history->writeIndex] |= RACE_REPLAY_BUTTON_C_RIGHT;
        heldInput = player->state.heldInput;
    }
    if (heldInput & L_CBUTTONS) {
        history->buttons[history->writeIndex] |= RACE_REPLAY_BUTTON_C_LEFT;
        heldInput = player->state.heldInput;
    }
    if (heldInput & A_BUTTON) {
        history->buttons[history->writeIndex] |= RACE_REPLAY_BUTTON_A;
        heldInput = player->state.heldInput;
    }
    if (heldInput & B_BUTTON) {
        history->buttons[history->writeIndex] |= RACE_REPLAY_BUTTON_B;
        heldInput = player->state.heldInput;
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
        playerAlias->state.stickX = history->stickX[index];
        playerAlias->state.stickY = history->stickY[history->writeIndex];
        goto dummy_label;
    dummy_label:
        playerAlias->state.heldInput = 0;

        replayButtons = history->buttons[history->writeIndex] ^ 0;
        if (replayButtons & RACE_REPLAY_BUTTON_C_UP) {
            playerAlias->state.heldInput |= U_CBUTTONS;
            buttonIndex = history->writeIndex;
            replayButtons = buttonHistory[buttonIndex];
        }
        if (replayButtons & RACE_REPLAY_BUTTON_C_DOWN) {
            playerAlias->state.heldInput |= D_CBUTTONS;
            replayButtons = history->buttons[history->writeIndex];
            if (history->stickY) {}
        }
        if (replayButtons & RACE_REPLAY_BUTTON_C_RIGHT) {
            playerAlias->state.heldInput |= R_CBUTTONS;
            replayButtons = history->buttons[history->writeIndex];
        }
        if (replayButtons & RACE_REPLAY_BUTTON_C_LEFT) {
            playerAlias->state.heldInput |= L_CBUTTONS;
            replayButtons = history->buttons[history->writeIndex];
        }
        if (replayButtons & RACE_REPLAY_BUTTON_A) {
            playerAlias->state.heldInput |= A_BUTTON;
            replayButtons = history->buttons[history->writeIndex];
        }
        if (replayButtons & RACE_REPLAY_BUTTON_B) {
            playerAlias->state.heldInput |= B_BUTTON;
            replayButtons = history->buttons[history->writeIndex];
        }

        if (1) {
            if (replayButtons & RACE_REPLAY_BUTTON_Z) {
                playerAlias->state.heldInput |= Z_TRIG;
            }
            (*history).writeIndex++;
        }
    }
}

void updateRacePlayerInput(RacePlayer *player) {
    u16 index;

    if (!(gMenuFlowState & 1) && !(player->state.stateFlags & 0x40)) {
        index = player->playerIndex;
        player->state.previousInput = gRacePlayers[index].state.heldInput;

        if (player->replayInputSource == RACE_PLAYER_INPUT_LIVE) {
            player->state.heldInput = gPlayerInputHeld[index];
            player->state.stickX = gPlayerStickX[index];
            player->state.stickY = gPlayerStickY[index];
            if (player->unk15 != 0) {
                recordRaceInputHistoryFrame(player);
            }
        } else {
            playRaceInputHistoryFrame(player);
        }

        player->state.heldInput &= ~(STICK_UP | STICK_DOWN | STICK_RIGHT | STICK_LEFT);
        if (player->state.stickX >= 0x1B) {
            player->state.heldInput |= STICK_RIGHT;
        }
        if (player->state.stickX < -0x1A) {
            player->state.heldInput |= STICK_LEFT;
        }
        if (player->state.stickY >= 0x1B) {
            player->state.heldInput |= STICK_UP;
        }
        if (player->state.stickY < -0x1A) {
            player->state.heldInput |= STICK_DOWN;
        }
        if (player->state.stickX < 8) {
            player->state.heldInput &= ~STICK_RIGHT;
        }
        if (player->state.stickX >= -7) {
            player->state.heldInput &= ~STICK_LEFT;
        }
        if (player->state.stickY < 8) {
            player->state.heldInput &= ~STICK_UP;
        }
        if (player->state.stickY >= -7) {
            player->state.heldInput &= ~STICK_DOWN;
        }

        player->state.pressedInput = player->state.heldInput & ~player->state.previousInput;
        return;
    }

    player->state.previousInput = 0;
    player->state.heldInput = 0;
    player->state.pressedInput = 0;
    player->state.stickX = 0;
    player->state.stickY = 0;
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
    temp_v0 = player->state.heldInput & ~player->state.previousInput;
    if (temp_v0 & INPUT_MASK_STICK_DIRECTIONS) {
        var_v1 = 1;
    }
    if (temp_v0 & INPUT_MASK_AB_BUTTONS) {
        var_v1++;
    }
    return var_v1;
}
