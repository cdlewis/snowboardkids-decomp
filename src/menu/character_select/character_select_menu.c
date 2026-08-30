#include "common.h"
#include "assets.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/menu/character_select/character_select_menu.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/character_select/character_select_ui.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/engine/viewport_manager.h"
#include "game/race/player/race_player_input.h"
#include "game/audio/sound_manager.h"

#define CHARACTER_SELECT_MOVE_LEFT 1
#define CHARACTER_SELECT_MOVE_RIGHT 2

CharacterSelectState gCharacterSelectHudState;
s8 gCharacterSelectHighlightedRosterIndices[4];
u8 gCharacterSelectSecretCharacterUnlocked;
u16 gCharacterSelectIdOrder[] = { 5, 0, 1, 2, 3, 4 };
s16 gCharacterSelectVoiceSoundIds[] = { 0x5D, 0x57, 0x58, 0x5A, 0x59, 0x5B, 0, 0, 0, 0 };

extern void releaseMenuAssetHandles(void);
extern s32 enqueueSoundEffect(s16, s16);
extern CallbackTask *D_8010ADEC;
extern s16 gMenuFadeAlpha;
// Per-player highlighted index. During character-roster browsing
// (gCharacterSelectHudState.phase == 3), this indexes gCharacterSelectIdOrder,
// not RacePlayer.characterId directly. Live-observed values: 1 = Slash,
// 2 = Nancy, 3 = Jam, 4 = Linda, 5 = Tommy; 0/6 are the two Shinobin slots.
// During later board-type selection, this address is reused by different code:
// 0 = Free Style, 1 = All Around, 2 = Alpine, 3 = back, and 4/5 appear to be
// locked/unowned special board states.
extern s32 gMenuFlowState;
extern u8 D_800EC9E5;
extern u8 gMenuExitSelection;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

void initCharacterSelectMenu(void) {
    s32 i;
    s32 j;

    if ((D_800EC9E5 == 0) || (gMenuExitSelection == 1)) {
        requestMusicSequenceBank(1);
        resetAllViewports();
        configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
        gFramebufferRenderInterval.value = 0;
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
                    gCharacterSelectHighlightedRosterIndices[i] = j;
                    break;
                }
            }
        }
    } else {
        gCurrentGameTask->fade = 1;
        gMenuFadeAlpha = 1;

        for (i = 0; i < gPlayerCount; i++) {
            gRacePlayers[i].selectedCharacterId = i;
            gCharacterSelectHighlightedRosterIndices[i] = i + 1;
        }
    }

    gActiveMenuTask = createCallbackTask((CallbackTaskCallback)initCharacterSelectConfirmationBanner, 0, 0x64);
    D_8010ADEC = createCallbackTask((CallbackTaskCallback)initCharacterSelectSelectedCharacterTokens, 0, 0x64);
    D_8010ADE8 = createCallbackTask((CallbackTaskCallback)initCharacterSelectPlayerCursorMarkers, 0, 0x64);
    D_8010ADE0 = createCallbackTask((CallbackTaskCallback)initCharacterSelectRosterIcons, 0, 0x64);
    createCallbackTask((CallbackTaskCallback)initCharacterSelectPlayerStatsPanels, 0, 0x64);
    D_8010ADE4 = createCallbackTask((CallbackTaskCallback)initCharacterSelectPlayerPanelFrames, 0, 0x63);

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

// Per-frame driver for the character roster screen only. Live RAM-watch
// testing observed gCharacterSelectHudState.phase as:
//   0 = transient init value
//   1 = "select your character" banner shown, non-interactive
//   2 = likely one-frame transition
//   3 = interactive roster
//   4 = "is this ok?" Yes/No confirmation dialog
void updateCharacterSelectMenu(void) {
    s32 playerIndex;
    s32 otherPlayerIndex;
    s32 attempt;
    s32 pad[4];
    u8 selection;
    u8 moveDirection;
    u8 duplicateCount;
    u8 lastPlayerState;
    u16 minimumIndex;
    u16 maximumIndex;
    s32 pad2;

    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16)gCurrentGameTask->fade, 0x24, 0);
        if (gCurrentGameTask->fade == 0) {
            gCharacterSelectHudState.phase = CHARACTER_SELECT_PHASE_PROMPT;
        }
    } else if (gCharacterSelectHudState.phase < CHARACTER_SELECT_PHASE_ROSTER) {
        if ((gCharacterSelectHudState.phase == CHARACTER_SELECT_PHASE_PROMPT) &&
            (gCharacterSelectHudState.bannerAlpha == 0x100)) {
            if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                gCharacterSelectHudState.phase = CHARACTER_SELECT_PHASE_PROMPT_EXIT;
                enqueueSoundEffect(1, 0x32);
                for (playerIndex = 0; playerIndex < gPlayerCount; playerIndex++) {
                    if ((gControllerPakStatusCodes[playerIndex] == CONTROLLER_PAK_STATUS_USE_EXISTING_SAVE) ||
                        (gControllerPakStatusCodes[playerIndex] == CONTROLLER_PAK_STATUS_READY)) {
                        gControllerPakStatusCodes[playerIndex] = CONTROLLER_PAK_STATUS_MENU_COMPLETE;
                    }
                }
            }
        }
    } else if (gCharacterSelectHudState.rosterReady == 1) {
        minimumIndex = gCharacterSelectHudState.leftSecretSlotUnlocked ? 0 : 1;
        maximumIndex = gCharacterSelectHudState.rightSecretSlotUnlocked ? 6 : 5;

        for (playerIndex = 0; playerIndex < gPlayerCount; playerIndex++) {
            if (gRacePlayers[playerIndex].menuState == 0) {
                selection = gCharacterSelectHighlightedRosterIndices[playerIndex];
                if (!(gPlayerInputHeld[playerIndex] & (STICK_RIGHT | R_JPAD)) && !(gPlayerInputHeld[playerIndex] & (STICK_LEFT | L_JPAD))) {
                    gMenuInputRepeatTimers[playerIndex] = 0;
                }
                if ((gPlayerInputPressed[playerIndex] & (STICK_LEFT | L_JPAD)) || ((gPlayerInputHeld[playerIndex] & (STICK_LEFT | L_JPAD)) && ((otherPlayerIndex = gMenuInputRepeatTimers[playerIndex]) >= 0xB) && (otherPlayerIndex & 1))) {
                    if (gMenuInputRepeatTimers[playerIndex] == 0) {
                        gMenuInputRepeatTimers[playerIndex] = gMenuInputRepeatTimers[playerIndex] + 1;
                    }
                    if (minimumIndex < selection) {
                        selection--;
                        moveDirection = CHARACTER_SELECT_MOVE_LEFT;
                    }
                }
                if ((gPlayerInputPressed[playerIndex] & (STICK_RIGHT | R_JPAD)) || ((gPlayerInputHeld[playerIndex] & (STICK_RIGHT | R_JPAD)) && ((otherPlayerIndex = gMenuInputRepeatTimers[playerIndex]) >= 0xB) && (otherPlayerIndex & 1))) {
                    if (gMenuInputRepeatTimers[playerIndex] == 0) {
                        gMenuInputRepeatTimers[playerIndex] = gMenuInputRepeatTimers[playerIndex] + 1;
                    }
                    if (selection < maximumIndex) {
                        selection++;
                        moveDirection = CHARACTER_SELECT_MOVE_RIGHT;
                    }
                }

                if (gMenuInputRepeatTimers[playerIndex] != 0) {
                    gMenuInputRepeatTimers[playerIndex] = gMenuInputRepeatTimers[playerIndex] + 1;
                    if ((gMenuInputRepeatTimers[playerIndex]) == 0xFFFF) {
                        gMenuInputRepeatTimers[playerIndex] = 0xC;
                    }
                }

                if (gCharacterSelectSecretCharacterUnlocked == 0) {
                    attempt = 0;
                    duplicateCount = 1;
                    while ((duplicateCount != 0) && (attempt != 7)) {
                        duplicateCount = 0;
                        for (otherPlayerIndex = 0; otherPlayerIndex < gPlayerCount; otherPlayerIndex++) {
                            if ((otherPlayerIndex != playerIndex) && (selection == gCharacterSelectHighlightedRosterIndices[otherPlayerIndex])) {
                                duplicateCount++;
                            }
                        }
                        attempt++;
                        if ((gCharacterSelectHudState.leftSecretSlotUnlocked == 0) && (selection == 0)) {
                            duplicateCount++;
                        }
                        if ((gCharacterSelectHudState.rightSecretSlotUnlocked == 0) && (selection == 6)) {
                            duplicateCount++;
                        }
                        if (duplicateCount) {
                            if (moveDirection == CHARACTER_SELECT_MOVE_LEFT) {
                                selection--;
                            } else {
                                selection++;
                            }
                            if (selection < minimumIndex) {
                                selection = gCharacterSelectHighlightedRosterIndices[playerIndex];
                            }
                            if (maximumIndex < selection) {
                                selection = gCharacterSelectHighlightedRosterIndices[playerIndex];
                            }
                            if (1);
                        }
                    }
                }

                if (selection != gCharacterSelectHighlightedRosterIndices[playerIndex]) {
                    enqueueSoundEffect(0x19, 0x32);
                }
                gCharacterSelectHighlightedRosterIndices[playerIndex] = selection;
                gRacePlayers[playerIndex].selectedCharacterId = gCharacterSelectIdOrder[gCharacterSelectHighlightedRosterIndices[playerIndex]];

                if ((gPlayerInputPressed[playerIndex] & A_BUTTON) || (gPlayerInputPressed[playerIndex] & START_BUTTON)) {
                    if (gRacePlayers[playerIndex].menuState == 0) {
                        enqueueSoundEffect(gCharacterSelectVoiceSoundIds[gCharacterSelectHighlightedRosterIndices[playerIndex]], 0x32);
                        gRacePlayers[playerIndex].menuState = 1;
                        gCharacterSelectHudState.selectedTokenState[playerIndex] = CHARACTER_SELECT_TOKEN_START;
                    }
                }
            }

            if ((gPlayerInputPressed[playerIndex] & B_BUTTON) && (gRacePlayers[playerIndex].menuState != 0)) {
                gRacePlayers[playerIndex].menuState = 0;
                gCharacterSelectHudState.selectedTokenState[playerIndex] = CHARACTER_SELECT_TOKEN_IDLE;
            }
            lastPlayerState = gRacePlayers[playerIndex].menuState & 1;
        }

        if (lastPlayerState != 0) {
            otherPlayerIndex = 0;
            for (playerIndex = 0; playerIndex < gPlayerCount; playerIndex++) {
                otherPlayerIndex += gCharacterSelectHudState.selectedTokenState[playerIndex];
            }
            if (otherPlayerIndex == (gPlayerCount * CHARACTER_SELECT_TOKEN_LANDED)) {
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

void updateCharacterSelectConfirmationMenu(void) {
    s32 i;
    s32 buttons;
    u32 selection;
    u8 oldSelection;

    if (gCharacterSelectHudState.bannerAlpha == 0x100) {
        buttons = gPlayerInputPressed[0];
        selection = gCharacterSelectHudState.confirmationChoice;
        oldSelection = selection;

        if ((buttons & (STICK_UP | U_JPAD)) && (selection != 0)) {
            selection = (gCharacterSelectHudState.confirmationChoice = selection - 1);
        } else if ((buttons & (STICK_DOWN | D_JPAD)) && (selection == 0)) {
            selection = (gCharacterSelectHudState.confirmationChoice = selection + 1);
        }

        if (selection != oldSelection) {
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
                for (i = 0; i < gPlayerCount; i++) {
                    gRacePlayers[i].menuState = 0;
                    gCharacterSelectHudState.selectedTokenState[i] = 0;
                }
                setCurrentGameTaskCallback(updateCharacterSelectMenu, 0);
                gCharacterSelectHudState.phase = CHARACTER_SELECT_PHASE_ROSTER;
                gCharacterSelectHudState.bannerAlpha = 0;
            }
        } else if (buttons & B_BUTTON) {
            enqueueSoundEffect(0x18, 0x32);
            for (i = 0; i < gPlayerCount; i++) {
                gRacePlayers[i].menuState = 0;
                gCharacterSelectHudState.selectedTokenState[i] = 0;
            }
            setCurrentGameTaskCallback(updateCharacterSelectMenu, 0);
            gCharacterSelectHudState.phase = CHARACTER_SELECT_PHASE_ROSTER;
            gCharacterSelectHudState.bannerAlpha = 0;
        }
    }
    updateCallbackTasks();
}

void fadeOutCharacterSelectMenu(void) {
    if (gCurrentGameTask->fade != 0xFF) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16)gCurrentGameTask->fade, 0x24, 1);
        if (gCurrentGameTask->fade == 0xFF) {
            gFramebufferSwapHold = 1;
        } else {
            updateCallbackTasks();
        }
    } else {
        if (gPendingFramebufferSwapCount == 2) {
            releaseMenuAssetHandles();
            gFramebufferSwapHold = 0;
            gFramebufferRenderInterval.value = 0;
            gMenuFlowState = 0;
            resumeGameTask(2);
            removeGameTask(4);
        }
    }
}
