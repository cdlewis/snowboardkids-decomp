#include "common.h"
#include "asset_manager.h"
#include "callback_task_scheduler.h"
#include "character_select_course_menu.h"
#include "race_character_select_menu.h"
#include "game_task_scheduler.h"
#include "menu_rendering.h"
#include "race_camera.h"
#include "sound_manager.h"
#include "system_boot.h"
#include "title_menu.h"
#include "viewport_manager.h"

extern void releaseMenuAssetHandles(void);

extern CharacterSelectFlowState *gCurrentGameTask;
extern s8 gFramebufferSwapDelay;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;

typedef struct {
    char pad0[0x3F];
    s8 characterState[12];
    char pad4B[0x78AD];
} RaceCharacterSelectSaveData;

typedef struct {
    char pad0[5];
    u8 mode;
    u8 selection;
    s8 unk7;
    u8 state;
    char pad9[0x603];
} RaceCharacterSelectPlayer;

typedef struct {
    u8 unk0[4];
    u8 unk4[4];
    u8 unk8[4];
    u8 unkC[4];
    u8 unk10[4];
    s16 unk14[4];
    s16 unk1C[4];
    u8 unk24[4];
    u8 unk28;
    char pad29;
    s16 unk2A;
    s16 unk2C;
    u8 unk2E;
} RaceCharacterSelectStatus;

extern s16 func_80042D58(s32);
extern s32 func_80043040(s16);
extern void n_alSeqpDelete(void);

extern u8 D_14B450[];
extern u8 D_1502A0[];
extern u8 D_1EF530[];
extern u8 D_1F1A90[];
extern u8 D_245A80[];
extern u8 D_24C8E0[];
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_59AAA0[];
extern u8 D_59DFE0[];
extern u8 D_59E7F0[];
extern u8 D_5A1ED0[];
extern u8 D_5C5320[];
extern u8 D_5CBA80[];
extern u8 D_5CCD40[];
extern s16 gMenuFadeAlpha;
extern s8 D_800EC9C0;
extern u8 D_800EC9C2;
extern s16 D_800EC9D0[];
extern s8 D_800EC9E5;
extern s8 D_800EC9E6;
extern RaceCharacterSelectSaveData D_800EC9F0[];
extern s32 D_8010ADDC;
extern s32 D_8010ADE0;
extern s32 D_8010ADE4;
extern s16 D_8010ADF0[];
extern s8 D_8010AE64[];
extern u8 D_8010AEA0[];
extern s8 D_8010AEA4[];
extern s8 D_8010AEA8;
extern s8 D_8010AEAC[];
extern s8 D_8010AEB0;
extern s8 D_8010AEB8[][3];
extern u8 D_8010AEC8[];
extern s8 D_8010AECC[];
extern s16 D_8010AED0;
extern s32 D_8010AED8[];
extern s32 D_8010AEE8[];
extern u8 D_8010AEF8[][4];
extern s8 D_8010AEFB[];
extern u8 D_8010AF08[][3];
extern RaceCharacterSelectStatus gCourseSelectStatus;
extern s16 D_80112130[];
extern s32 D_80112204;
extern void (*D_8011220C)(void);
extern s32 D_801122B4;
extern void (*D_801122BC)(void);
extern s32 D_80112364;
extern void (*D_8011236C)(void);
extern s32 D_80112414;
extern void (*D_8011241C)(void);
extern u8 gPlayerCount;
extern RaceCharacterSelectPlayer D_80121D80[];
extern RaceCharacterSelectPlayer gFrameCounter;
extern s32 D_801235B4;

// initRaceCharacterSelectMenu best match: 78.174% (nonmatchings/initRaceCharacterSelectMenu-7273315160691878794/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_character_select_menu/initRaceCharacterSelectMenu.s")

#ifdef NON_MATCHING
void initRaceCharacterSelectMenu(void) {
    s32 size;
    s32 i;
    s32 j;
    s32 k;
    s32 sum;
    s32 selected;
    s32 screenBase;

    if (D_800EC9C2 == 1) {
        requestMusicSequenceBank(2);
    }
    func_8006D5CC();
    resetAllViewports();

    switch (gPlayerCount) {
    case 1:
        configureViewport(0, 0xE8, 0x78, 0x90, 0xD0, 0xA0, 0xF0, 0.6666666865f);
        screenBase = 0xA40000;
        break;
    case 2:
        configureViewport(0, 0xE4, 0x4A, 0x84, 0x54, 0xA8, 0x74, 1.448275805f);
        configureViewport(1, 0xE4, 0xAE, 0x84, 0x54, 0xA8, 0x74, 1.448275805f);
        screenBase = 0x894000;
        break;
    case 3:
    case 4:
        configureViewport(0, 0x78, 0x44, 0x44, 0x40, 0x60, 0x60, 1.333333373f);
        configureViewport(1, 0x78, 0xA8, 0x44, 0x40, 0x60, 0x60, 1.333333373f);
        configureViewport(2, 0x104, 0x44, 0x44, 0x40, 0x60, 0x60, 1.333333373f);
        configureViewport(3, 0x104, 0xA8, 0x44, 0x40, 0x60, 0x60, 1.333333373f);
        screenBase = 0x894000;
        break;
    default:
        break;
    }

    D_8011220C = n_alSeqpDelete;
    D_80112204 = screenBase;
    D_8010AEA0[0] = 0;
    D_8010AEAC[0] = 0;
    D_801122BC = n_alSeqpDelete;
    D_801122B4 = screenBase;
    D_8010AEA0[1] = 0;
    D_8010AEAC[1] = 0;
    D_8011236C = n_alSeqpDelete;
    D_80112364 = screenBase;
    D_8010AEA0[2] = 0;
    D_8010AEAC[2] = 0;
    D_8011241C = n_alSeqpDelete;
    D_80112414 = screenBase;
    D_8010AEA0[3] = 0;
    D_8010AEAC[3] = 0;
    gFramebufferSwapDelay = 0;
    D_800EC9E5 = 0;
    D_8010AEA8 = 0;
    D_8010AEB0 = 0;
    gCurrentGameTask->fade = 0xFF;
    gMenuFadeAlpha = gCurrentGameTask->fade;

    loadCompressedRomAsset(D_5CBA80, D_5CCD40, 0x21);
    loadCompressedRomAsset(D_593D10, D_598A70, 0x22);
    loadCompressedRomAsset(D_598A70, D_59AAA0, 0x23);
    loadCompressedRomAsset(D_59AAA0, D_59DFE0, 0x24);
    loadCompressedRomAsset(D_5A1ED0, D_5C5320, 0x25);
    loadCompressedRomAsset(D_59DFE0, D_59E7F0, 0x26);

    size = D_1502A0 - D_14B450;
    D_80112130[0xC] = func_80042D58(size);
    dmaReadRom((u32)D_14B450, (void *)func_80043040(D_80112130[0xC]), size);
    loadCompressedRomAsset(D_1EF530, D_1F1A90, 0xD);
    loadCompressedRomAsset(D_245A80, D_24C8E0, 0x1F);
    initCallbackTaskScheduler(0);
    createCallbackTask((void (*)(CallbackTask *))func_8001710C, 0, 0x63);

    D_800EC9C0 = 0;
    D_8010ADDC = 0;
    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    D_801235B4 = 0;
    D_8010AED0 = 0;
    D_800EC9E6 = 0;
    gCurrentGameTask->timer = 0;
    gCurrentGameTask->unk20 = 0;
    setCurrentGameTaskCallback(updateRaceCharacterSelectMenu, 0);

    for (i = 0; &D_80121D80[i] < &gFrameCounter; i++) {
        D_80121D80[i].mode = 0;
    }

    for (i = 0; i < gPlayerCount; i++) {
        D_8010AEA4[i] = 0;
        D_8010AEC8[i] = 0;
        D_8010AECC[i] = 0;
        D_800EC9D0[i] = 0;
        D_8010ADF0[i] = 0;
        D_8010AED8[i] = 0;
        D_8010AEE8[i] = 0;

        for (j = 0; j < 3; j++) {
            sum = 0;
            for (k = 0; k < 3; k++) {
                sum += D_800EC9F0[i].characterState[j + k * 3];
            }
            D_8010AEB8[i][j] = (sum != -3);
        }

        sum = 0;
        for (j = 9; j < 12; j++) {
            sum += D_800EC9F0[i].characterState[j];
        }
        if (sum >= -2) {
            D_8010AEC8[i] = 4;
        }
        if (D_8010AEC8[i] == 4) {
            D_8010AEA0[i] = 1;
        }

        for (j = 0; j < 3; j++) {
            D_8010AEF8[i][j] = j;
        }

        if (D_8010AEA0[i] == 1) {
            for (j = 9; j < 12; j++) {
                if (D_800EC9F0[i].characterState[j] != -1) {
                    D_8010AEFB[i * 4] = j;
                    break;
                }
            }

            k = 0;
            for (j = 9; j < 12; j++) {
                D_8010AF08[i][k] = 0;
                if (D_800EC9F0[i].characterState[j] != -1) {
                    D_8010AF08[i][k] = j;
                    k++;
                }
            }
        } else {
            D_8010AEFB[i * 4] = 0;
        }
    }

    for (i = 0; i < gPlayerCount; i++) {
        selected = D_80121D80[i].selection;
        if ((selected >= 9) && (selected < 12)) {
            j = 3;
            if (D_80121D80[i].mode == 5) {
                j = 0;
            }
            k = gCourseSelectStatus.unk2E;
        } else {
            k = gCourseSelectStatus.unk2E;
            if (k == 1) {
                j = 0;
                gCourseSelectStatus.unk2E = 0;
                k = 0;
            } else {
                j = selected % 3;
            }
        }

        D_8010AE64[i] = j;
        if (k == 1) {
            j--;
        }

        if (D_80121D80[i].mode == 5) {
            D_8010AEAC[i] = 0;
        } else if (selected < 9) {
            D_8010AEAC[i] = selected / 3;
        } else {
            if (selected >= 12) {
                D_8010AEAC[i] = 0;
            }
            if (selected >= 9 && selected < 12) {
                for (k = 0; k < 3; k++) {
                    if (D_8010AF08[i][k] == selected) {
                        D_8010AEAC[i] = k;
                        break;
                    }
                }
            }
        }

        D_80121D80[i].selection = D_8010AEF8[i][j];
        gCourseSelectStatus.unk0[i] = 0;
        gCourseSelectStatus.unk4[i] = 0;
        gCourseSelectStatus.unk8[i] = 0;
        gCourseSelectStatus.unkC[i] = 0;
        gCourseSelectStatus.unk10[i] = 0;
        gCourseSelectStatus.unk14[i] = 0;
        gCourseSelectStatus.unk1C[i] = 0;
        gCourseSelectStatus.unk24[i] = 0;
    }

    gCourseSelectStatus.unk28 = 0;
    gCourseSelectStatus.unk2A = 0;
    gCourseSelectStatus.unk2C = 0;
}
#endif

// updateRaceCharacterSelectMenu best match: 57.412% (nonmatchings/updateRaceCharacterSelectMenu-5752545231564691495/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_character_select_menu/updateRaceCharacterSelectMenu.s")

#ifdef NON_MATCHING
typedef struct RaceCharacterSelectObject {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ void (*update)(void);
    /* 0x30 */ u8 pad30[0x80];
} RaceCharacterSelectObject;

extern void initCourseSelectCourseIconList(CallbackTask *);
extern void initCourseSelectExtraCourseIconList(CallbackTask *);
extern void initCourseSelectPlayerPanels(CallbackTask *);
extern void initCourseSelectCompletePanels(CallbackTask *);
extern s32 enqueueSoundEffect(s16, s16);

extern void *D_8010ADE8;
extern u8 D_8010AF06[];
extern u8 D_8010AF46;
extern s16 D_800B349C[];
extern s8 D_800ECA2F[][0x78F8];
extern s32 gPlayerInputHeld[];
extern s32 gPlayerInputPressed[];
extern RaceCharacterSelectObject *D_800EC9C4;
extern RaceCharacterSelectObject D_801121E0[];
extern RaceCharacterSelectObject D_801124A0;

void updateRaceCharacterSelectMenu(void) {
    s32 readyCount;
    s32 blockingCount;
    s32 activeCount;
    s32 playerCount;
    s32 i;
    s32 row;
    u8 maxColumnOriginal;
    s8 oldColumn;
    u8 maxColumn;
    s32 held;
    s32 pressed;
    s32 heldHorizontal;
    s16 *repeatTimer;
    s8 *column;
    s8 *confirmHold;
    s8 *unlockColumn;
    s8 *rowLock;
    s32 *momentum;
    u8 *selections;
    RaceCharacterSelectPlayer *player;
    RaceCharacterSelectObject *obj;

    readyCount = 0;
    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) gCurrentGameTask->fade, 0x24, 0);
        if (gCurrentGameTask->fade == 0) {
            if (gPlayerCount == 1) {
                createCallbackTask(initCourseSelectCourseIconList, 0, 0x63);
            } else {
                createCallbackTask(initCourseSelectPlayerPanels, 0, 0x62);
                createCallbackTask(initCourseSelectCompletePanels, 0, 0x63);
            }
            D_8010ADE8 = createCallbackTask(initCourseSelectExtraCourseIconList, 0, 0x61);
        }
    } else {
        playerCount = gPlayerCount;
        readyCount = 0;
        for (i = 0; i < playerCount; i++) {
            if (D_80121D80[i].state == 9) {
                readyCount = (readyCount + 1) & 0xFF;
            }
        }

        if (playerCount == readyCount) {
            blockingCount = 0;
            for (i = 0; i < playerCount; i++) {
                row = D_800EC9D0[i];
                if ((D_8010AEE8[i] != 0) || (row == 1) || (row == 9) || (row >= 5)) {
                    blockingCount++;
                }
            }

            if (blockingCount == 0) {
                row = (playerCount < 3) ? playerCount : 4;
                for (i = 0; i < row; i++) {
                    D_80121D80[i].state = 3;
                }
                D_800EC9C0 = 0xF;
                D_801235B4 = 1;
            }
        } else if (D_800EC9C0 == 0) {
            activeCount = 0;
            player = D_80121D80;
            for (i = 0; i < playerCount; i++, player++) {
                row = D_800EC9D0[i];
                if (row == 0) {
                    if ((gCourseSelectStatus.unk0[i] == 1) && (player->state == 0)) {
                        confirmHold = &D_8010AECC[i];
                        if (((u8) *confirmHold & 1) == 0) {
                            if (D_8010AEB0 == 1) {
                                player->state = 9;
                            } else {
                                column = &D_8010AE64[i];
                                momentum = &D_8010AEE8[i];
                                selections = D_8010AEF8[i];
                                repeatTimer = &D_8010ADF0[i];
                                maxColumn = 4;
                                if ((player->mode == 5) || (D_8010AEA0[i] == 0)) {
                                    maxColumn = 3;
                                }
                                maxColumnOriginal = maxColumn;
                                if (playerCount >= 2) {
                                    maxColumn = (maxColumn - 1) & 0xFF;
                                }

                                held = gPlayerInputHeld[i];
                                oldColumn = *column;
                                heldHorizontal = held & 0x10800;
                                if ((heldHorizontal == 0) && !(held & 0x20400)) {
                                    *repeatTimer = 0;
                                }
                                pressed = gPlayerInputPressed[i];
                                if ((pressed & 0x10800) || ((heldHorizontal != 0) && (*repeatTimer >= 9))) {
                                    if (*repeatTimer == 0) {
                                        *repeatTimer = *repeatTimer + 1;
                                    }
                                    if (oldColumn > 0) {
                                        *column = oldColumn - 1;
                                        enqueueSoundEffect(0x19, 0x32);
                                        D_8010AEAC[i] = 0;
                                        oldColumn = *column;
                                        pressed = gPlayerInputPressed[i];
                                    }
                                } else if ((pressed & 0x20400) || ((held & 0x20400) && (*repeatTimer >= 9))) {
                                    if (*repeatTimer == 0) {
                                        *repeatTimer = *repeatTimer + 1;
                                    }
                                    if (oldColumn < maxColumn) {
                                        *column = oldColumn + 1;
                                        enqueueSoundEffect(0x19, 0x32);
                                        D_8010AEAC[i] = 0;
                                        oldColumn = *column;
                                        pressed = gPlayerInputPressed[i];
                                    }
                                }
                                if (*repeatTimer != 0) {
                                    *repeatTimer = *repeatTimer + 1;
                                    if ((u16) *repeatTimer == 0xFFFF) {
                                        *repeatTimer = 0xA;
                                    }
                                }

                                if ((maxColumnOriginal != oldColumn) && (maxColumnOriginal != (u8) oldColumn) && ((u8) oldColumn != oldColumn)) {
                                    *confirmHold = *confirmHold + 1;
                                    if ((u8) oldColumn < *column) {
                                        *momentum = -0x800000;
                                    } else {
                                        *momentum = 0x800000;
                                    }
                                }
                                if (maxColumnOriginal == *column) {
                                    player->selection = selections[maxColumnOriginal - 1];
                                    D_8010AF46 = 1;
                                } else {
                                    player->selection = selections[*column];
                                    D_8010AF46 = 0;
                                }

                                if (*momentum == 0) {
                                    if ((pressed & 0x1000) || (pressed & 0x8000)) {
                                        if ((gPlayerCount == 1) && (maxColumn == D_8010AE64[0])) {
                                            enqueueSoundEffect(0x18, 0x32);
                                            D_80121D80[0].state = 9;
                                            D_8010AEB0 = 1;
                                            D_801235B4 = 1;
                                        } else if ((player->selection >= 9) || (D_8010AEB8[i][*column] != 0)) {
                                            rowLock = &D_8010AEA4[i];
                                            enqueueSoundEffect(0x18, 0x32);
                                            player->state = 1;
                                            if (player->mode == 5) {
                                                enqueueSoundEffect(0x40, 0x32);
                                            } else {
                                                enqueueSoundEffect(D_800B349C[*column], 0x32);
                                            }
                                            D_800EC9D0[i] = 9;
                                            *rowLock = 1;
                                            *repeatTimer = 0;
                                        }
                                    } else if (pressed & 0x4000) {
                                        enqueueSoundEffect(0x18, 0x32);
                                        player->state = 9;
                                        D_8010AEB0 = 1;
                                    }
                                } else if (pressed & 0x4000) {
                                    enqueueSoundEffect(0x18, 0x32);
                                    player->state = 9;
                                    D_8010AEB0 = 1;
                                }
                            }
                        }
                    }
                } else {
                    rowLock = &D_8010AEA4[i];
                    if (row == 9) {
                        *rowLock = *rowLock + 1;
                    } else {
                        *rowLock = 0;
                    }

                    if ((row >= 2) && (row < 5)) {
                        if (D_8010AEB0 == 1) {
                            player->state = 9;
                        } else {
                            held = gPlayerInputHeld[i];
                            heldHorizontal = held & 0x10800;
                            unlockColumn = &D_8010AEA0[i];
                            repeatTimer = &D_8010ADF0[i];
                            if ((heldHorizontal == 0) && !(held & 0x20400)) {
                                *repeatTimer = 0;
                            }
                            pressed = gPlayerInputPressed[i];
                            if ((pressed & 0x10800) || ((heldHorizontal != 0) && (*repeatTimer >= 9) && (*repeatTimer & 1))) {
                                if (*repeatTimer == 0) {
                                    *repeatTimer = *repeatTimer + 1;
                                }
                                if (row >= 3) {
                                    D_800EC9D0[i] = row - 1;
                                    enqueueSoundEffect(0x19, 0x32);
                                    row = D_800EC9D0[i];
                                    pressed = gPlayerInputPressed[i];
                                }
                            } else if ((pressed & 0x20400) || ((held & 0x20400) && (*repeatTimer >= 9) && (*repeatTimer & 1))) {
                                if (*repeatTimer == 0) {
                                    *repeatTimer = *repeatTimer + 1;
                                }
                                if (row < gCourseSelectStatus.unk24[i] + 1) {
                                    D_800EC9D0[i] = row + 1;
                                    enqueueSoundEffect(0x19, 0x32);
                                    row = D_800EC9D0[i];
                                    pressed = gPlayerInputPressed[i];
                                }
                            }
                            if (*repeatTimer != 0) {
                                *repeatTimer = *repeatTimer + 1;
                                if ((u16) *repeatTimer == 0xFFFF) {
                                    *repeatTimer = 0xA;
                                }
                            }

                            if ((*unlockColumn == 1) && (D_8010AE64[i] == 3)) {
                                player->selection = D_8010AF06[(i * 3) + row];
                            } else {
                                player->selection = (row * 3) + (player->selection % 3) - 6;
                            }

                            if ((pressed & 0x8000) || (pressed & 0x1000)) {
                                enqueueSoundEffect(0x18, 0x32);
                                player->state = 2;
                                D_8010AEA8 = 1;
                                if ((*unlockColumn == 1) && (D_8010AE64[i] == 3)) {
                                    player->selection = D_8010AF06[(i * 3) + row];
                                } else {
                                    player->selection = (row * 3) + (player->selection % 3) + ((player->mode == 5) ? 0xC : 0) - 6;
                                }
                                if (player->selection >= 9) {
                                    player->unk7 = 0;
                                } else {
                                    player->unk7 = D_800ECA2F[i][player->selection];
                                }
                                D_800EC9D0[i] += 3;
                                if ((u8) D_8010AECC[i] == 0) {
                                    gCourseSelectStatus.unkC[i] = 1;
                                    gCourseSelectStatus.unk14[i] = 0;
                                    gCourseSelectStatus.unk4[i] = 3;
                                } else {
                                    gCourseSelectStatus.unk10[i] = 1;
                                    gCourseSelectStatus.unk1C[i] = 0;
                                    gCourseSelectStatus.unk8[i] = 3;
                                }
                            } else if (pressed & 0x4000) {
                                enqueueSoundEffect(0x18, 0x32);
                                *repeatTimer = 0;
                                D_800EC9D0[i] += 3;
                                player->selection = D_8010AEF8[i][D_8010AE64[i]];
                            }
                        }
                    }
                }

                if (player->state == 2) {
                    if (D_8010AEB0 == 1) {
                        player->state = 9;
                    } else if (gPlayerInputPressed[i] & 0x4000) {
                        enqueueSoundEffect(0x18, 0x32);
                        D_8010ADF0[i] = 0;
                        D_8010AEA8 = 0;
                        player->state = 1;
                        D_800EC9D0[i] -= 3;
                        if ((u8) D_8010AECC[i] == 0) {
                            gCourseSelectStatus.unk4[i] = 1;
                        } else {
                            gCourseSelectStatus.unk8[i] = 1;
                        }
                    } else {
                        activeCount++;
                    }
                }
                if (activeCount == gPlayerCount) {
                    D_800EC9C0 = 1;
                }
            }
        } else {
            D_800EC9C0++;
        }
    }

    if (D_800EC9C0 == 0x19) {
        setCurrentGameTaskCallback(fadeOutRaceCharacterSelectMenu, 0);
        if (D_801235B4 == 0) {
            requestMusicSequenceStop(8);
        }
    }

    obj = D_801121E0;
    do {
        D_800EC9C4 = obj;
        obj->update();
        obj++;
    } while (obj != &D_801124A0);
    updateCallbackTasks();
}

#endif

void fadeOutRaceCharacterSelectMenu(void) {
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
            resumeGameTask(2);
            removeGameTask(4);
        }
    }
}
