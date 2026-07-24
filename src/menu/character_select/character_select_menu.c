#include "common.h"
#include "assets.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/menu/character_select/character_select_menu.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/character_select/character_select_ui.h"
#include "game/engine/viewport_manager.h"
#include "game/race/player/race_player_input.h"

#define CHARACTER_SELECT_MOVE_LEFT 1
#define CHARACTER_SELECT_MOVE_RIGHT 2

#define CONTROLLER_PAK_STATUS_READY 5
#define CONTROLLER_PAK_STATUS_USE_EXISTING_SAVE 0x12
#define CONTROLLER_PAK_STATUS_MENU_COMPLETE 0x13

typedef struct {
    char pad[0x18];
    s32 fade;
    s32 timer;
} CharacterSelectMenuState;

CharacterSelectState gCharacterSelectHudState;
u8 gCharacterSelectSecretCharacterUnlocked;

extern void releaseMenuAssetHandles(void);
extern void enqueueSoundEffect(s32, s32);
extern void updateCharacterSelectMenu(void);
extern void fadeOutCharacterSelectMenu(void);
extern void requestMusicSequenceBank(s32);
extern CharacterSelectMenuState *gCurrentGameTask;
extern u16 gCharacterSelectIdOrder[];
extern s16 gCharacterSelectVoiceSoundIds[];
extern CallbackTask *gActiveMenuTask;
extern CallbackTask *D_8010ADE0;
extern CallbackTask *D_8010ADE4;
extern CallbackTask *D_8010ADE8;
extern CallbackTask *D_8010ADEC;
extern s16 gMenuFadeAlpha;
extern s16 gMenuInputRepeatTimers[];
extern s8 gFramebufferSwapDelay;
extern s8 gMenuSelectionConfirmTimer;
extern s8 D_8010AE64[];
// Per-player highlighted index. During character-roster browsing
// (gCharacterSelectHudState.phase == 3), this indexes gCharacterSelectIdOrder,
// not RacePlayer.characterId directly. Live-observed values: 1 = Slash,
// 2 = Nancy, 3 = Jam, 4 = Linda, 5 = Tommy; 0/6 are the two Shinobin slots.
// During later board-type selection, this address is reused by different code:
// 0 = Free Style, 1 = All Around, 2 = Alpine, 3 = back, and 4/5 appear to be
// locked/unowned special board states.
extern s32 gMenuFlowState;
extern u32 gPlayerInputHeld[];
extern u32 gPlayerInputPressed[];
extern s16 gControllerPakStatusCodes[];
extern u8 D_60F1A0[];
extern u8 gMainMenuSecretCodeUnlocked;
extern u8 D_800EC9E5;
extern u8 gMenuExitSelection;
extern u8 gPlayerCount;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

void initCharacterSelectMenu(void) {
    s32 i;
    s32 j;

    if ((D_800EC9E5 == 0) || (gMenuExitSelection == 1)) {
        requestMusicSequenceBank(1);
        resetAllViewports();
        configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
        gFramebufferSwapDelay = 0;
        gMenuSelectionConfirmTimer = 0;
        gMenuExitSelection = 0;
        gCurrentGameTask->fade = 0xFF;
        gCurrentGameTask->timer = 0;
        gMenuFadeAlpha = gCurrentGameTask->fade;
        LOAD_ASSET(_59AAA0, 0x21);
        LOAD_ASSET(_245A80, 0x1F);
        LOAD_ASSET(_593D10, 0x22);
        LOAD_ASSET(_598A70, 0x23);
        LOAD_ASSET(_60F1A0, 0x29);
        initCallbackTaskScheduler(0);

        for (i = 0; i < gPlayerCount; i++) {
            for (j = 0; j != 6; j++) {
                if (gRacePlayers[i].selectedCharacterId == gCharacterSelectIdOrder[j]) {
                    D_8010AE64[i] = j;
                    break;
                }
            }
        }
    } else {
        gCurrentGameTask->fade = 1;
        gMenuFadeAlpha = 1;

        for (i = 0; i < gPlayerCount; i++) {
            gRacePlayers[i].selectedCharacterId = i;
            D_8010AE64[i] = i + 1;
        }
    }

    gActiveMenuTask = createCallbackTask(initCharacterSelectConfirmationBanner, 0, 0x64);
    D_8010ADEC = createCallbackTask(initCharacterSelectSelectedCharacterTokens, 0, 0x64);
    D_8010ADE8 = createCallbackTask(initCharacterSelectPlayerCursorMarkers, 0, 0x64);
    D_8010ADE0 = createCallbackTask(initCharacterSelectRosterIcons, 0, 0x64);
    createCallbackTask(initCharacterSelectPlayerStatsPanels, 0, 0x64);
    D_8010ADE4 = createCallbackTask(initCharacterSelectPlayerPanelFrames, 0, 0x63);

    setCurrentGameTaskCallback(updateCharacterSelectMenu, 0);

    for (i = 0; i < gPlayerCount; i++) {
        gRacePlayers[i].menuState = 0;
        gMenuInputRepeatTimers[i] = 0;
    }
    gCharacterSelectHudState.phase = CHARACTER_SELECT_PHASE_INIT;
    gCharacterSelectHudState.exitMode = 0;
    gCharacterSelectHudState.rosterReady = 0;
    gCharacterSelectHudState.bannerAlpha = 0;
    gCharacterSelectHudState.bannerBounceOffset = 0;
    gCharacterSelectHudState.confirmationChoice = 0;
    gCharacterSelectHudState.bannerBounceTimer = 0;
    gCharacterSelectHudState.leftSecretSlotUnlocked = 0;
    gCharacterSelectHudState.rightSecretSlotUnlocked = 0;
    gCharacterSelectHudState.cursorX = 0;
    gCharacterSelectHudState.cursorY = 0;

    i = -1;
    while (++i < 4) {
        gCharacterSelectHudState.selectedTokenState[i] = CHARACTER_SELECT_TOKEN_IDLE;
    }
    gCharacterSelectSecretCharacterUnlocked = 0;
    if (gMainMenuSecretCodeUnlocked != 0) {
        gCharacterSelectSecretCharacterUnlocked = 1;
    }
    updateCallbackTasks();
}

// Best compiler-matching attempt: 69.415%
// (nonmatchings/updateCharacterSelectMenu-5787290371232622032/base_46.c).
// The implementation below favors readable source while the assembly include
// above preserves the exact original code.
// Per-frame driver for the character roster screen only. Live RAM-watch
// testing observed gCharacterSelectHudState.phase as:
//   0 = transient init value
//   1 = "select your character" banner shown, non-interactive
//   2 = likely one-frame transition
//   3 = interactive roster
//   4 = "is this ok?" Yes/No confirmation dialog
#pragma GLOBAL_ASM("asm/nonmatchings/menu/character_select/character_select_menu/updateCharacterSelectMenu.s")

#ifdef NON_MATCHING
void updateCharacterSelectMenu(void) {
    s32 playerIndex;
    s32 otherPlayerIndex;
    s32 conflictingSelectionCount;
    s32 selectionAttempts;
    s32 selectedTokenStateTotal;
    s32 minimumRosterIndex;
    s32 maximumRosterIndex;
    u32 heldButtons;
    u32 pressedButtons;
    u16 repeatTimer;
    s8 oldRosterIndex;
    u8 rosterIndex;
    u8 moveDirection;
    u8 lastPlayerConfirmed;
    RacePlayer *player;

    moveDirection = 0;
    lastPlayerConfirmed = 0;

    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16)gCurrentGameTask->fade, 0x24, FALSE);
        if (gCurrentGameTask->fade == 0) {
            gCharacterSelectHudState.phase = CHARACTER_SELECT_PHASE_PROMPT;
        }
    } else if (gCharacterSelectHudState.phase < CHARACTER_SELECT_PHASE_ROSTER) {
        if ((gCharacterSelectHudState.phase == CHARACTER_SELECT_PHASE_PROMPT) &&
            (gCharacterSelectHudState.bannerAlpha == 0x100) &&
            (gPlayerInputPressed[0] & (A_BUTTON | START_BUTTON))) {
            gCharacterSelectHudState.phase = CHARACTER_SELECT_PHASE_PROMPT_EXIT;
            enqueueSoundEffect(1, 0x32);

            for (playerIndex = 0; playerIndex < gPlayerCount; playerIndex++) {
                if ((gControllerPakStatusCodes[playerIndex] == CONTROLLER_PAK_STATUS_USE_EXISTING_SAVE) ||
                    (gControllerPakStatusCodes[playerIndex] == CONTROLLER_PAK_STATUS_READY)) {
                    gControllerPakStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_MENU_COMPLETE;
                }
            }
        }
    } else if (gCharacterSelectHudState.rosterReady == 1) {
        minimumRosterIndex = 1;
        maximumRosterIndex = 5;
        if (gCharacterSelectHudState.leftSecretSlotUnlocked != 0) {
            minimumRosterIndex = 0;
        }
        if (gCharacterSelectHudState.rightSecretSlotUnlocked != 0) {
            maximumRosterIndex = 6;
        }

        for (playerIndex = 0; playerIndex < gPlayerCount; playerIndex++) {
            player = &gRacePlayers[playerIndex];
            pressedButtons = gPlayerInputPressed[playerIndex];

            if (player->menuState == 0) {
                oldRosterIndex = gCharacterSelectHudState.highlightedRosterIndices[playerIndex];
                rosterIndex = oldRosterIndex;
                heldButtons = gPlayerInputHeld[playerIndex];
                repeatTimer = gMenuInputRepeatTimers[playerIndex];

                if (!(heldButtons & (STICK_LEFT | L_JPAD | STICK_RIGHT | R_JPAD))) {
                    gMenuInputRepeatTimers[playerIndex] = 0;
                    repeatTimer = 0;
                }

                if ((pressedButtons & (STICK_LEFT | L_JPAD)) ||
                    ((heldButtons & (STICK_LEFT | L_JPAD)) && (repeatTimer >= 0xB) && (repeatTimer & 1))) {
                    if (repeatTimer == 0) {
                        gMenuInputRepeatTimers[playerIndex] = 1;
                    }
                    if (minimumRosterIndex < rosterIndex) {
                        rosterIndex--;
                        moveDirection = CHARACTER_SELECT_MOVE_LEFT;
                    }
                }

                repeatTimer = gMenuInputRepeatTimers[playerIndex];
                if ((pressedButtons & (STICK_RIGHT | R_JPAD)) ||
                    ((heldButtons & (STICK_RIGHT | R_JPAD)) && (repeatTimer >= 0xB) && (repeatTimer & 1))) {
                    if (repeatTimer == 0) {
                        gMenuInputRepeatTimers[playerIndex] = 1;
                    }
                    if (rosterIndex < maximumRosterIndex) {
                        rosterIndex++;
                        moveDirection = CHARACTER_SELECT_MOVE_RIGHT;
                    }
                }

                if (gMenuInputRepeatTimers[playerIndex] != 0) {
                    gMenuInputRepeatTimers[playerIndex]++;
                    if ((u16)gMenuInputRepeatTimers[playerIndex] == 0xFFFF) {
                        gMenuInputRepeatTimers[playerIndex] = 0xC;
                    }
                }

                if (gCharacterSelectSecretCharacterUnlocked == 0) {
                    selectionAttempts = 0;
                    do {
                        conflictingSelectionCount = 0;
                        for (otherPlayerIndex = 0; otherPlayerIndex < gPlayerCount; otherPlayerIndex++) {
                            if ((otherPlayerIndex != playerIndex) &&
                                (rosterIndex ==
                                 gCharacterSelectHudState.highlightedRosterIndices[otherPlayerIndex])) {
                                conflictingSelectionCount++;
                            }
                        }

                        selectionAttempts++;
                        if ((gCharacterSelectHudState.leftSecretSlotUnlocked == 0) && (rosterIndex == 0)) {
                            conflictingSelectionCount++;
                        }
                        if ((gCharacterSelectHudState.rightSecretSlotUnlocked == 0) && (rosterIndex == 6)) {
                            conflictingSelectionCount++;
                        }

                        if (conflictingSelectionCount != 0) {
                            if (moveDirection == CHARACTER_SELECT_MOVE_LEFT) {
                                rosterIndex--;
                            } else {
                                rosterIndex++;
                            }
                            if ((rosterIndex < minimumRosterIndex) || (maximumRosterIndex < rosterIndex)) {
                                rosterIndex = oldRosterIndex;
                            }
                        }
                    } while ((conflictingSelectionCount != 0) && (selectionAttempts != 7));
                }

                if (rosterIndex != (u8)oldRosterIndex) {
                    enqueueSoundEffect(0x19, 0x32);
                }

                gCharacterSelectHudState.highlightedRosterIndices[playerIndex] = rosterIndex;
                player->selectedCharacterId = gCharacterSelectIdOrder[(s8)rosterIndex];

                if ((pressedButtons & (A_BUTTON | START_BUTTON)) && (player->menuState == 0)) {
                    enqueueSoundEffect(gCharacterSelectVoiceSoundIds[(s8)rosterIndex], 0x32);
                    player->menuState = 1;
                    gCharacterSelectHudState.selectedTokenState[playerIndex] = CHARACTER_SELECT_TOKEN_START;
                }
            }

            if ((pressedButtons & B_BUTTON) && (player->menuState != 0)) {
                player->menuState = 0;
                gCharacterSelectHudState.selectedTokenState[playerIndex] = CHARACTER_SELECT_TOKEN_IDLE;
            }

            lastPlayerConfirmed = player->menuState & 1;
        }

        if (lastPlayerConfirmed != 0) {
            selectedTokenStateTotal = 0;
            for (playerIndex = 0; playerIndex < gPlayerCount; playerIndex++) {
                selectedTokenStateTotal += gCharacterSelectHudState.selectedTokenState[playerIndex];
            }

            if (selectedTokenStateTotal == (gPlayerCount * CHARACTER_SELECT_TOKEN_LANDED)) {
                gCharacterSelectHudState.phase = CHARACTER_SELECT_PHASE_CONFIRM;
                gCharacterSelectHudState.confirmationChoice = 0;
                gCharacterSelectHudState.bannerBounceTimer = 0;
                gCharacterSelectHudState.bannerBounceOffset = 0x100;
                setCurrentGameTaskCallback(updateCharacterSelectConfirmationMenu, 0);
            }
        }
    }
    updateCallbackTasks();
}
#endif

// updateCharacterSelectConfirmationMenu best match: 91.299% (nonmatchings/updateCharacterSelectConfirmationMenu-2188069624939011928/base_15.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/character_select/character_select_menu/updateCharacterSelectConfirmationMenu.s")

#ifdef NON_MATCHING
void updateCharacterSelectConfirmationMenu(void) {
    CharacterSelectState *state;
    RacePlayer *player;
    u8 *readyPtr;
    u8 *readyEnd;
    s32 oldSelection;
    s32 buttons;
    s32 keepGoing;
    u8 playerCount;

    if (gCharacterSelectHudState.bannerAlpha == 0x100) {
        buttons = gPlayerInputPressed[0];
        oldSelection = gCharacterSelectHudState.confirmationChoice;

        if ((buttons & (STICK_UP | U_JPAD)) && (oldSelection != 0)) {
            gCharacterSelectHudState.confirmationChoice = oldSelection - 1;
        } else if ((buttons & (STICK_DOWN | D_JPAD)) && (oldSelection == 0)) {
            gCharacterSelectHudState.confirmationChoice = oldSelection + 1;
        }

        if (gCharacterSelectHudState.confirmationChoice != oldSelection) {
            enqueueSoundEffect(0x19, 0x32);
            buttons = gPlayerInputPressed[0];
        }

        if ((buttons & A_BUTTON) || (buttons & START_BUTTON)) {
            enqueueSoundEffect(0x18, 0x32);
            if (gCharacterSelectHudState.confirmationChoice == 0) {
                setCurrentGameTaskCallback(fadeOutCharacterSelectMenu, 0);
                gCharacterSelectHudState.cursorX = 0x8C;
                gCharacterSelectHudState.cursorY = 0x44;
                gCharacterSelectHudState.exitMode = 3;
            } else {
                playerCount = gPlayerCount;
                player = gRacePlayers;
                if ((s32) playerCount > 0) {
                    readyPtr = (u8 *) &gCharacterSelectHudState;
                    readyEnd = playerCount + readyPtr;
                    do {
                        readyPtr++;
                        keepGoing = (u32) readyPtr < (u32) readyEnd;
                        player++;
                        player[-1].menuState = 0;
                        readyPtr[2] = 0;
                    } while (keepGoing);
                }
                setCurrentGameTaskCallback(updateCharacterSelectMenu, 0);
                gCharacterSelectHudState.phase = CHARACTER_SELECT_PHASE_ROSTER;
                gCharacterSelectHudState.bannerAlpha = 0;
            }
        } else if (buttons & B_BUTTON) {
            enqueueSoundEffect(0x18, 0x32);
            playerCount = gPlayerCount;
            player = gRacePlayers;
            if ((s32) playerCount > 0) {
                readyPtr = (u8 *) &gCharacterSelectHudState;
                readyEnd = playerCount + readyPtr;
                do {
                    readyPtr++;
                    keepGoing = (u32) readyPtr < (u32) readyEnd;
                    player++;
                    player[-1].menuState = 0;
                    readyPtr[2] = 0;
                } while (keepGoing);
            }
            setCurrentGameTaskCallback(updateCharacterSelectMenu, 0);
            state = &gCharacterSelectHudState;
            state->phase = CHARACTER_SELECT_PHASE_ROSTER;
            state->bannerAlpha = 0;
        }
    }
    updateCallbackTasks();
}
#endif

void fadeOutCharacterSelectMenu(void) {
    if (gCurrentGameTask->fade != 0xFF) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) gCurrentGameTask->fade, 0x24, 1);
        if (gCurrentGameTask->fade == 0xFF) {
            gFramebufferSwapHold = 1;
        } else {
            updateCallbackTasks();
        }
    } else {
        if (gPendingFramebufferSwapCount == 2) {
            releaseMenuAssetHandles();
            gFramebufferSwapHold = 0;
            gFramebufferSwapDelay = 0;
            gMenuFlowState = 0;
            resumeGameTask(2);
            removeGameTask(4);
        }
    }
}
