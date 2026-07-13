#include "common.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "character_select_menu.h"
#include "game_task_scheduler.h"
#include "menu_renderer.h"
#include "character_select_ui.h"
#include "viewport_manager.h"
#include "race_player_input.h"

typedef struct {
    char pad[0x18];
    s32 fade;
    s32 timer;
} CharacterSelectMenuState;

typedef struct {
    u8 phase;
    u8 exitMode;
    u8 readyCount;
    u8 pad3;
    char pad4[4];
    s16 fade;
    s16 unkA;
    u8 confirmSelection;
    u8 unkD;
    u8 blockDecrease;
    u8 blockIncrease;
    s16 cursorX;
    s16 cursorY;
    s8 playerSelections[4];
} CharacterSelectState;

typedef struct {
    u16 characterId;
} CharacterId;

extern void releaseMenuAssetHandles(void);
extern void enqueueSoundEffect(s32, s32);
extern void updateCharacterSelectMenu(void);
extern void fadeOutCharacterSelectMenu(void);
extern void requestMusicSequenceBank(s32);
extern CharacterSelectMenuState *gCurrentGameTask;
extern CharacterSelectState gCharacterSelectHudState;
extern CharacterId gCharacterSelectIdOrder[];
extern CallbackTask *gActiveMenuTask;
extern CallbackTask *D_8010ADE0;
extern CallbackTask *D_8010ADE4;
extern CallbackTask *D_8010ADE8;
extern CallbackTask *D_8010ADEC;
extern f32 gCharacterSelectViewportAspectRatio;
extern s16 gMenuFadeAlpha;
extern s16 gMenuInputRepeatTimers[];
extern s8 gFramebufferSwapDelay;
extern s8 gMenuSelectionConfirmTimer;
extern s8 D_8010AE53;
extern s8 D_8010AE54;
extern s8 D_8010AE55;
extern s8 D_8010AE56;
extern s8 D_8010AE64[];
extern s32 gMenuFlowState;
extern s32 gPlayerInputPressed;
extern u8 D_245A80[];
extern u8 D_24C8E0[];
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_59AAA0[];
extern u8 D_59DFE0[];
extern u8 D_60F1A0[];
extern u8 D_60F990[];
extern u8 gMainMenuSecretCodeUnlocked;
extern u8 D_800EC9E5;
extern u8 gMenuExitSelection;
extern u8 D_8010AE68;
extern u8 gPlayerCount;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

// initCharacterSelectMenu best match: 92.662% (nonmatchings/initCharacterSelectMenu-2663524570355072948/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_menu/initCharacterSelectMenu.s")

#ifdef NON_MATCHING
void initCharacterSelectMenu(void) {
    RacePlayer *player;
    CharacterSelectState *state;
    s32 i;
    s32 j;
    s32 keepGoing;
    s32 playerCount;
    u8 *selectionPtr;
    s16 *timerPtr;
    s16 *timerEnd;

    if ((D_800EC9E5 == 0) || (gMenuExitSelection == 1)) {
        requestMusicSequenceBank(1);
        resetAllViewports();
        configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, gCharacterSelectViewportAspectRatio);
        gFramebufferSwapDelay = 0;
        gMenuSelectionConfirmTimer = 0;
        gMenuExitSelection = 0;
        gCurrentGameTask->fade = 0xFF;
        gCurrentGameTask->timer = 0;
        gMenuFadeAlpha = gCurrentGameTask->fade;
        loadCompressedRomAsset(D_59AAA0, D_59DFE0, 0x21);
        loadCompressedRomAsset(D_245A80, D_24C8E0, 0x1F);
        loadCompressedRomAsset(D_593D10, D_598A70, 0x22);
        loadCompressedRomAsset(D_598A70, D_59AAA0, 0x23);
        loadCompressedRomAsset(D_60F1A0, D_60F990, 0x29);
        initCallbackTaskScheduler(0);

        playerCount = gPlayerCount;
        i = 0;
        if (playerCount > 0) {
            player = gRacePlayers;
            do {
                j = 0;
loop_1:
                if (player->selectedCharacterId == gCharacterSelectIdOrder[j].characterId) {
                    D_8010AE64[i] = j;
                } else {
                    j++;
                    if (j != 6) {
                        goto loop_1;
                    }
                }
                i++;
                player++;
            } while (i < playerCount);
        }
    } else {
        gCurrentGameTask->fade = 1;
        gMenuFadeAlpha = 1;
        playerCount = gPlayerCount;
        i = 0;
        if (playerCount > 0) {
            selectionPtr = (u8 *) D_8010AE64;
            player = gRacePlayers;
            do {
                j = i + 1;
                keepGoing = j < playerCount;
                player->selectedCharacterId = i;
                i = j;
                player++;
                selectionPtr++;
                selectionPtr[-1] = j;
            } while (keepGoing);
        }
    }

    gActiveMenuTask = createCallbackTask(initCharacterSelectConfirmationBanner, 0, 0x64);
    D_8010ADEC = createCallbackTask(initCharacterSelectSelectedCharacterTokens, 0, 0x64);
    D_8010ADE8 = createCallbackTask(initCharacterSelectPlayerCursorMarkers, 0, 0x64);
    D_8010ADE0 = createCallbackTask(initCharacterSelectRosterIcons, 0, 0x64);
    createCallbackTask(initCharacterSelectPlayerStatsPanels, 0, 0x64);
    D_8010ADE4 = createCallbackTask(initCharacterSelectPlayerPanelFrames, 0, 0x63);
    setCurrentGameTaskCallback(updateCharacterSelectMenu, 0);

    playerCount = gPlayerCount;
    if (playerCount > 0) {
        timerPtr = gMenuInputRepeatTimers;
        timerEnd = timerPtr + playerCount;
        player = gRacePlayers;
        do {
            timerPtr++;
            player++;
            player[-1].menuState = 0;
            timerPtr[-1] = 0;
        } while ((u32) timerPtr < (u32) timerEnd);
    }

    state = &gCharacterSelectHudState;
    state->phase = 0;
    state->exitMode = 0;
    state->readyCount = 0;
    state->fade = 0;
    state->unkA = 0;
    state->confirmSelection = 0;
    state->unkD = 0;
    state->blockDecrease = 0;
    state->blockIncrease = 0;
    state->cursorX = 0;
    state->cursorY = 0;
    D_8010AE53 = 0;
    D_8010AE54 = 0;
    D_8010AE55 = 0;
    D_8010AE56 = 0;
    D_8010AE68 = 0;
    if (gMainMenuSecretCodeUnlocked != 0) {
        D_8010AE68 = 1;
    }
    updateCallbackTasks();
}
#endif

// updateCharacterSelectMenu best match: 55.575% (nonmatchings/func_80004CC8-731940616440357983/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_menu/updateCharacterSelectMenu.s")

// updateCharacterSelectConfirmationMenu best match: 85.264%
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_menu/updateCharacterSelectConfirmationMenu.s")

#ifdef NON_MATCHING
void updateCharacterSelectConfirmationMenu(void) {
    CharacterSelectState *state;
    RacePlayer *player;
    u8 *readyPtr;
    u8 *readyEnd;
    s32 oldSelection;
    s32 buttons;
    s32 selection;
    u8 playerCount;

    state = &gCharacterSelectHudState;
    if (state->fade == 0x100) {
        buttons = gPlayerInputPressed;
        selection = state->confirmSelection;
        oldSelection = selection;

        if ((buttons & 0x10800) && (selection != 0)) {
            state->confirmSelection = selection - 1;
            selection = (u8) (selection - 1);
        } else if (buttons & 0x20400) {
            if (selection == 0) {
                state->confirmSelection = selection + 1;
                selection = (u8) (selection + 1);
            }
        }

        if (selection != oldSelection) {
            enqueueSoundEffect(0x19, 0x32);
            buttons = gPlayerInputPressed;
        }

        if ((buttons & 0x8000) || (buttons & 0x1000)) {
            enqueueSoundEffect(0x18, 0x32);
            state = &gCharacterSelectHudState;
            if (state->confirmSelection == 0) {
                setCurrentGameTaskCallback(fadeOutCharacterSelectMenu, 0);
                state = &gCharacterSelectHudState;
                state->cursorX = 0x8C;
                state->cursorY = 0x44;
                state->exitMode = 3;
            } else {
                playerCount = gPlayerCount;
                player = gRacePlayers;
                if ((s32) playerCount > 0) {
                    readyPtr = (u8 *) &gCharacterSelectHudState;
                    readyEnd = playerCount + readyPtr;
                    do {
                        readyPtr++;
                        player++;
                        player[-1].menuState = 0;
                        readyPtr[2] = 0;
                    } while ((u32) readyPtr < (u32) readyEnd);
                }
                setCurrentGameTaskCallback(updateCharacterSelectMenu, 0);
                state = &gCharacterSelectHudState;
                state->phase = 3;
                state->fade = 0;
            }
        } else if (buttons & 0x4000) {
            enqueueSoundEffect(0x18, 0x32);
            playerCount = gPlayerCount;
            player = gRacePlayers;
            if ((s32) playerCount > 0) {
                readyPtr = (u8 *) &gCharacterSelectHudState;
                readyEnd = playerCount + readyPtr;
                do {
                    readyPtr++;
                    player++;
                    player[-1].menuState = 0;
                    readyPtr[2] = 0;
                } while ((u32) readyPtr < (u32) readyEnd);
            }
            setCurrentGameTaskCallback(updateCharacterSelectMenu, 0);
            state = &gCharacterSelectHudState;
            state->phase = 3;
            state->fade = 0;
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
