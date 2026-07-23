#include "common.h"
#include "assets.h"
#include "game/engine/asset_manager.h"
#include "game/race/camera/race_camera.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/course_select/course_select_menu.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/race/effects/race_start_transition.h"
#include "game/menu/course_select/course_select_shop_ui.h"
#include "game/engine/system_runtime.h"
#include "game/menu/race_setup/race_setup_ui.h"
#include "game/engine/viewport_manager.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 fade;
    /* 0x1C */ s32 timer;
    /* 0x20 */ s32 screenState;
} CourseSelectMenuState;

typedef struct {
    union {
        struct {
            /* 0x00 */ u8 unk0[4];
            /* 0x04 */ u8 playerOneCourseDecided;
            /* 0x05 */ u8 unk5[3];
            /* 0x08 */ u8 playerTwoCourseDecided;
            /* 0x09 */ u8 unk9[7];
            /* 0x10 */ u8 unk10[4];
        };
        struct {
            /* 0x00 */ u8 unk0Array[4];
            /* 0x04 */ u8 unk4Array[4];
            /* 0x08 */ u8 unk8Array[4];
            /* 0x0C */ u8 unkCArray[4];
            /* 0x10 */ u8 unk10Array[4];
        };
    };
    /* 0x14 */ s16 unk14[4];
    /* 0x1C */ s16 unk1C[4];
    /* 0x24 */ u8 unk24[4];
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 transitionState;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F[9];
} CourseSelectStatus;

typedef struct {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ u8 unk8;
    /* 0x09 */ u8 pad9[0x603];
} CourseSelectSelection;

typedef struct ObjectA3E0 {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad28[4];
    /* 0x2C */ void (*unk2C)(void);
    /* 0x30 */ u8 pad30[0x80];
} ObjectA3E0;

typedef union {
    CourseSelectSelection *selection;
    ObjectA3E0 *object;
} CourseSelectPointer;

extern u8 D_5C5320[];
extern f32 D_800E0A30;
extern ObjectA3E0 *gCurrentMenuCameraObject;
extern u16 gCourseDetailsPreviewCourseTiles[];
extern s16 gMenuFadeAlpha;
extern s16 gMenuChoicePromptState;
extern s8 gFramebufferSwapDelay;
extern s8 D_800EC9C0;
extern u8 gMenuSelectionConfirmTimer;
extern u8 gCourseSelectModeSelection;
extern u8 gGameSaveDataBuffer[];
extern s8 D_800EC9F1;
extern s8 D_800EC9F9;
extern s8 gCourseUnlockSaveSlots[];
extern s32 gActiveMenuTask;
extern CallbackTask *D_8010ADE8;
extern s8 D_8010AE64;
extern u8 D_8010AEA0[];
extern s8 D_8010AEA4;
extern s8 D_8010AEA8;
extern u8 D_8010AEAC[];
extern s8 D_8010AEB0;
extern u8 D_8010AEF8[];
extern u8 D_8010AEFB[];
extern u8 D_8010AF08[];
extern u8 gCourseSelectExtraCourseIds[];
extern s16 D_8010AED0;
extern s16 gMenuInputRepeatTimers;
extern u8 gMenuExitSelection;
extern u8 D_8010AECC;
extern CourseSelectStatus gCourseSelectStatus;
extern u8 D_8010AF19;
extern u8 D_8010AF1A;
extern u8 D_8010AF1B;
extern u8 D_8010AF1C;
extern u8 D_8010AF1D;
extern u8 D_8010AF1E;
extern u8 D_8010AF1F;
extern u8 D_8010AF20;
extern u8 D_8010AF21;
extern u8 D_8010AF22;
extern u8 D_8010AF23;
extern u8 D_8010AF24;
extern u8 D_8010AF25;
extern u8 D_8010AF26;
extern u8 D_8010AF27;
extern u8 D_8010AF28;
extern u8 D_8010AF29;
extern u8 D_8010AF2A;
extern u8 D_8010AF2B;
extern s16 D_8010AF2C;
extern s16 D_8010AF2E;
extern s16 D_8010AF30;
extern s16 D_8010AF32;
extern s16 D_8010AF34;
extern s16 D_8010AF36;
extern s16 D_8010AF38;
extern s16 D_8010AF3A;
extern u8 D_8010AF3C;
extern u8 D_8010AF3D;
extern u8 D_8010AF3E;
extern u8 D_8010AF3F;
extern u8 gCourseSelectExtraCourseColumnState;
extern u8 gShopMenuDescriptionSeen;
extern u8 gShopMenuShowNewCoursesMessage;
extern s8 gCourseDetailsMenuSelection;
extern s8 gCourseDetailsPreviewPage;
extern s16 gCoursePreviewViewportHeight;
extern s8 gCourseDetailsCloseFromBack;
extern s16 gAssetHandles[];
extern ObjectA3E0 D_801121E0[];
extern ObjectA3E0 D_80112340;
extern u8 gPlayerCount;
extern u8 gCourseSelectSelectedCourseId;
extern s8 gCourseSelectSelectedCourseSavedSlot;
extern u8 gMenuTransitionState;
extern CourseSelectSelection gRacePlayers;
extern s32 gPlayer1Money;
extern CourseSelectMenuState *gCurrentGameTask;
extern s32 gMenuFlowState;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern s32 gPlayerInputHeld;
extern s32 gPlayerInputPressed;
extern void releaseMenuAssetHandles(void);
extern void requestMusicSequenceBank(s32);
extern void requestMusicSequenceStop(s32);
extern void enqueueSoundEffect(s32, s32);
// initCourseSelectMenu best match: 99.238% (nonmatchings/initCourseSelectMenu-2188069624939011928/base_23.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_menu/initCourseSelectMenu.s")

#ifdef NON_MATCHING
void initCourseSelectMenu(void) {
    u32 size;
    s32 i;
    s32 zero;
    s32 mask;
    s32 hasMore;
    u8 *unlockedCourse;
    u8 *otherCourse;
    ObjectA3E0 *obj;
    char savedUnlocks;

    requestMusicSequenceBank(3);
    resetRaceCameras();
    resetAllViewports();
    configureViewport(0, 0xE8, 0x78, 0x90, 0xD0, 0xA0, 0xF0, D_800E0A30);

    obj = D_801121E0;
    unlockedCourse = D_8010AEA0;
    otherCourse = D_8010AEAC;
    do {
        otherCourse++;
        hasMore = (u32)otherCourse < (u32)&D_8010AEB0;
        obj++;
        unlockedCourse++;
        obj[-1].unk2C = updateMenuCameraObjectLookAtOriginCallback;
        obj[-1].unk24 = 0xA40000;
        if ((obj && obj) && obj) {
        }
        unlockedCourse[-1] = 0;
        otherCourse[-1] = 0;
    } while (hasMore);

    gFramebufferSwapDelay = 0;
    gCurrentGameTask->fade = 0xFF;
    LOAD_ASSET(_5CBA80, 0x21);
    LOAD_ASSET(_593D10, 0x22);
    LOAD_ASSET(_598A70, 0x23);
    LOAD_ASSET(_59AAA0, 0x24);
    LOAD_ASSET(_5A1ED0, 0x25);
    LOAD_ASSET(_59DFE0, 0x26);
    LOAD_ASSET(_5D4280, 0x27);

    size = ASSET_SIZE(_14B450);
    gAssetHandles[0xC] = allocRelocatableHeapBlock(size);
    DMA_ASSET(_14B450, (void *)getRelocatableHeapBlockBase(gAssetHandles[0xC]), size);
    LOAD_ASSET(_1EF530, 0xD);
    LOAD_ASSET(_1E74E0, 0x1C);
    initCallbackTaskScheduler(0);
    createCallbackTask((void (*)(CallbackTask *))initMenuIconTilemapSpriteActor, 0, 0x5E);

    gMenuSelectionConfirmTimer = 0;
    D_800EC9C0 = 0;
    gCourseDetailsCloseFromBack = 0;
    gRacePlayers.unk8 = 0;
    gRacePlayers.unk6 = 0;
    D_8010AE64 = 0;
    gActiveMenuTask = 0;
    gCurrentGameTask->timer = 0;
    gMenuExitSelection = 0;
    gShopMenuDescriptionSeen = 0;
    gShopMenuShowNewCoursesMessage = 0;
    gCourseDetailsMenuSelection = 0;
    gCourseDetailsPreviewPage = 0;
    zero = 0;
    D_8010AED0 = zero;
    D_8010AEA8 = zero;
    D_8010AEA4 = zero;
    D_8010AEB0 = 0;
    gMenuChoicePromptState = 0;
    D_8010AECC = 0;
    gMenuInputRepeatTimers = 0;

    gMenuFadeAlpha = gCurrentGameTask->fade;
    mask = 1;
 i = 0; do { savedUnlocks = gGameSaveDataBuffer[0x78D7]; i++; if (mask & savedUnlocks) { if (!(savedUnlocks & (mask << 3))) { gShopMenuShowNewCoursesMessage = 1; gShopMenuDescriptionSeen = 1;
                gGameSaveDataBuffer[0x78D7] = savedUnlocks | (mask << 3);
            }
        }
        mask <<= 1;
    } while (i < 3);

    if (gCourseSelectModeSelection == 2) {
        gCourseSelectModeSelection = 0;
    }

    setCurrentGameTaskCallback(updateCourseSelectModeMenu, 0);
    updateCallbackTasks();

    gCourseSelectStatus.unk0Array[zero] = zero;
    D_8010AF1C = zero;
    D_8010AF20 = 0;
    D_8010AF24 = 0;
    D_8010AF28 = 0;
    D_8010AF2C = 0;
    D_8010AF34 = zero;
    D_8010AF3C = 0;
    D_8010AF19 = 0;
    D_8010AF1D = 0;
    D_8010AF21 = zero;
    D_8010AF25 = 0;
    D_8010AF29 = 0;
    D_8010AF2E = zero;
    D_8010AF36 = zero;
    D_8010AF3D = 0;
    D_8010AF1A = 0;
    D_8010AF1E = 0;
    D_8010AF22 = 0;
    D_8010AF26 = zero;
    i = 0;
    D_8010AF2A = i;
    D_8010AF30 = i;
    D_8010AF38 = zero;
    D_8010AF3E = zero;
    D_8010AF1B = i;
    D_8010AF1F = i;
    D_8010AF23 = i;
    D_8010AF27 = i;
    D_8010AF2B = i;
    D_8010AF32 = i;
    D_8010AF3A = i;
    D_8010AF3F = i;
    gCourseSelectStatus.transitionState = i;
    gCourseSelectStatus.unk28 = i;
    gCourseSelectStatus.unk2A = i;
    gCourseSelectStatus.unk2C = i;
    gCourseSelectStatus.unk2E = i;
}
#endif

// updateCourseSelectModeMenu best match: 88.922% (nonmatchings/updateCourseSelectModeMenu-1645024839200431810/base_29.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_menu/updateCourseSelectModeMenu.s")

#ifdef NON_MATCHING
void updateCourseSelectModeMenu(void) {
    s32 input;
    s32 pressed;
    s32 leftPressed;
    s32 held;
    s32 timer;
    s32 selection;
    s32 oldSelection;
    s32 tempSelection;
    s32 timerValue;
    s32 repeatNext;
    u32 timerOriginal;

    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) gCurrentGameTask->fade, 0x24, 0);
        if (gCurrentGameTask->fade == 0) {
            createCallbackTask(initShopMenuModeChoiceRows, 0, 0x63);
            if (gShopMenuDescriptionSeen == 0) {
                enqueueSoundEffect(0x44, 0x32);
            }
        }
    } else {
        timer = gMenuSelectionConfirmTimer;
        timerOriginal = timer;
        timerValue = timer;
        if (timer == 0) {
            if ((gMenuTransitionState == 0) && (gCourseSelectStatus.unk28 == 1)) {
                selection = gCourseSelectModeSelection;
                oldSelection = selection;
                pressed = (timerValue = gPlayerInputHeld);
                leftPressed = pressed & (STICK_UP | U_JPAD);
                if ((leftPressed == 0) && !(pressed & (STICK_DOWN | D_JPAD))) {
                    gMenuInputRepeatTimers = 0;
                }
                held = (timerOriginal = gPlayerInputPressed);
                if ((held & (STICK_UP | U_JPAD)) ||
                    ((leftPressed != 0) && ((s32) (u16) gMenuInputRepeatTimers >= 0xB) &&
                     (((s32) (u16) gMenuInputRepeatTimers % 3) == 0))) {
                    timer = (u16) gMenuInputRepeatTimers;
                    timer = (u16) gMenuInputRepeatTimers;
                    tempSelection = selection - 1;
                    timerValue = timer + 1;
                    if (timer == 0) {
                        gMenuInputRepeatTimers = timerValue;
                        timer = (u16) timerValue;
                    }
                    if (selection > 0) {
                        gCourseSelectModeSelection = tempSelection;
                        selection = (u8) tempSelection;
                    }
                } else {
                    timer = (u16) gMenuInputRepeatTimers;
                    if ((held & (STICK_DOWN | D_JPAD)) ||
                        ((pressed & (STICK_DOWN | D_JPAD)) && (timer >= 0xB) &&
                         ((timer % 3) == 0))) {
                        if (timer == 0) {
                            timerValue = timer + 1;
                            gMenuInputRepeatTimers = timerValue;
                            timer = (u16) timerValue;
                        }
                        if (selection < 2) {
                            tempSelection = selection + 1;
                            gCourseSelectModeSelection = tempSelection;
                            selection = (u8) tempSelection;
                        }
                    }
                }
                if (timer != 0) {
                    repeatNext = timer + 1;
                    gMenuInputRepeatTimers = repeatNext;
                    if ((u16) repeatNext == 0xFFFF) {
                        gMenuInputRepeatTimers = 0xC;
                    }
                }
                if (selection != oldSelection) {
                    enqueueSoundEffect(0x19, 0x32);
                    if (gShopMenuDescriptionSeen == 0) {
                        gShopMenuDescriptionSeen = 1;
                    }
                    if (gShopMenuShowNewCoursesMessage == 1) {
                        gShopMenuShowNewCoursesMessage = 0;
                    }
                }
                input = gPlayerInputPressed;
                if (((input & START_BUTTON) || (input & A_BUTTON)) && (gMenuFlowState == 2)) {
                    gMenuSelectionConfirmTimer = 1;
                    gCourseSelectStatus.unk28 = 2;
                    gCourseSelectStatus.unk2A = 0x100;
                    if ((s32) gCourseSelectModeSelection < 2) {
                        gMenuExitSelection = 0;
                        enqueueSoundEffect(0x18, 0x32);
                    } else {
                        gMenuExitSelection = 1;
                        enqueueSoundEffect(0x46, 0x32);
                    }
                    timer = gMenuSelectionConfirmTimer;
                    timerValue = timer;
                } else {
                    timer = gMenuSelectionConfirmTimer;
                    timerValue = timer;
                    if ((input & B_BUTTON) && (gMenuFlowState == 2) && (timerValue == 0)) {
                        gMenuSelectionConfirmTimer = 1;
                        gCourseSelectStatus.unk28 = 2;
                        gCourseSelectStatus.unk2A = 0x100;
                        gMenuExitSelection = 1;
                        enqueueSoundEffect(0x46, 0x32);
                        timer = gMenuSelectionConfirmTimer;
                        timerValue = timer;
                    }
                }
            }
        } else {
            timer = (u8) (timerOriginal + 1);
            timerValue = timer;
            gMenuSelectionConfirmTimer = timer;
        }
        if (timerValue == 8) {
            gMenuTransitionState = 1;
            gMenuSelectionConfirmTimer = timer + 1;
        }
        if (gMenuTransitionState == 2) {
            gMenuSelectionConfirmTimer = 0;
            if (gMenuExitSelection == 0) {
                setCurrentGameTaskCallback(initCourseSelectCourseList, 0);
            } else {
                setCurrentGameTaskCallback(exitCourseSelectMenu, 0);
                requestMusicSequenceStop(8);
                gMenuExitSelection = 0;
                gCourseSelectExtraCourseColumnState = 0;
            }
        }
    }
    gMenuFlowState = 0;
    updateCallbackTasks();
}
#endif

// initCourseSelectCourseList best match: 99.737% (nonmatchings/initCourseSelectCourseList-7998791169205557824/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_menu/initCourseSelectCourseList.s")

#ifdef NON_MATCHING
void initCourseSelectCourseList(void) {
    u8 courseFlags;
    s32 i;
    s32 mask;
    s32 listMask;
    s32 one;
    s32 column;
    u8 *extraCourse;
    s32 selected;
    u8 loadedCourseFlags;

    gMenuTransitionState = 0;
    D_8010AEA0[0] = 0;
    gMenuSelectionConfirmTimer = 0;
    gMenuInputRepeatTimers = 0;
    createCallbackTask(initCourseSelectCourseIconList, 0, 0x63);
    D_8010ADE8 = createCallbackTask(initCourseSelectExtraCourseIconList, 0, 0x61);
    loadedCourseFlags = gUnlockedExtraCourseFlags;
    courseFlags = loadedCourseFlags;
    if (courseFlags & 7) {
        D_8010AEA0[0] = 1;
    }

    for (i = 0; i < 3; i++) {
        D_8010AEF8[i] = i;
    }

    one = 1;
    if (D_8010AEA0[0] == one) {
        mask = one;
        for (i = 9; i < 0xC; i++) {
            if (courseFlags & mask) {
                D_8010AEFB[0] = i;
                break;
            }
            mask *= 2;
        }

        listMask = one;
        extraCourse = gCourseSelectExtraCourseIds + 1;
        for (i = 9; i != 0xC; i++) {
            *extraCourse = 0;
            if (courseFlags & listMask) {
                *extraCourse = i;
                extraCourse++;
            }
            listMask *= 2;
        }
    } else {
        D_8010AEFB[0] = 0;
    }

    if (((s32) gRacePlayers.unk6 >= 9) && ((s32) gRacePlayers.unk6 < 0xC)) {
        selected = gCourseSelectExtraCourseColumnState;
        column = 3;
    } else {
        selected = gCourseSelectStatus.unk2E;
        column = 0;
        if (one == selected) {
            /* Preserve IDO's selected-course register allocation. */
            if (column) {
                do { } while (0);
            }
            selected = (gCourseSelectStatus.unk2E = 0);
        } else {
            column = (s32) gRacePlayers.unk6 % 3;
        }
    }

    D_8010AE64 = column;
    if (one == selected) {
        column--;
    }

    gRacePlayers.unk6 = D_8010AEF8[column];
    setCurrentGameTaskCallback(updateCourseSelectCourseList, 0);
    updateCallbackTasks();
}
#endif

// updateCourseSelectCourseList best match: 77.188% (nonmatchings/updateCourseSelectCourseList-5787290371232622032/base_18.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_menu/updateCourseSelectCourseList.s")

#ifdef NON_MATCHING
void updateCourseSelectCourseList(void) {
    u8 sp32;
    s8 sp31;
    volatile CourseSelectPointer sp18;
    CourseSelectSelection *var_v0_5;
    s16 temp_t6;
    s16 temp_t7;
    s16 temp_v0;
    s32 *var_v1;
    s32 temp_t4;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a3;
    s8 temp_a0_2;
    u16 var_v1_2;
    u8 var_a1;
    u8 var_t1;
    u8 var_t8;
    u8 var_v0_3;
    u8 var_v0_4;
    u8 var_v0_6;
    CourseSelectSelection *var_v0;
    CourseSelectSelection *var_v0_2;

    var_a0 = 0;
    if (gMenuTransitionState == 9) {
        var_a3 = 0;
        if ((s32) gPlayerCount > 0) {
            var_v1 = D_8010AEE8;
            do {
                if ((*var_v1 != 0) || (temp_v0 = (&gMenuChoicePromptState)[var_a3], (temp_v0 == 1)) || (temp_v0 >= 5)) {
                    var_a0 += 1;
                }
                var_a3 += 1;
                var_v1 += 1;
            } while (var_a3 < (s32) gPlayerCount);
        }
        if (var_a0 == 0) {
            var_a3 = 0;
            if ((s32) gPlayerCount < 3) {
                var_a1 = gPlayerCount;
            } else {
                var_a1 = 4;
            }
            for (var_a3 = 0; var_a3 < (s32) var_a1; var_a3++) {
                (&gRacePlayers)[var_a3].unk8 = 3;
            }
            D_800EC9C0 = 0xF;
            gMenuFlowState = 1;
        }
        var_v0_3 = (u8) D_800EC9C0;
    } else if ((u8) D_800EC9C0 == 0) {

        var_a3 = 0;
        if (gMenuChoicePromptState == 0) {
            if ((gCourseSelectStatus.unk0[0] == 1) && (gMenuTransitionState == 0) && !(D_8010AECC & 1)) {
                if ((gMenuSelectionVariant == 5) || (*D_8010AEA0 == 0) || (var_t1 = 4, (gCourseSelectModeSelection == 1))) {
                    var_t1 = 3;
                }
                sp32 = var_t1;
                if ((s32) gPlayerCount >= 2) {
                    var_t1 = (var_t1 - 1) & 0xFF;
                }
                temp_v0_2 = gPlayerInputHeld & 0x10800;
                sp31 = D_8010AE64;
                if ((temp_v0_2 == 0) && !(gPlayerInputHeld & 0x20400)) {
                    gMenuInputRepeatTimers = 0;
                }
                if ((gPlayerInputPressed & 0x10800) || ((temp_v0_2 != 0) && ((s32) (u16) gMenuInputRepeatTimers >= 9))) {
                    if ((u16) gMenuInputRepeatTimers == 0) {
                        gMenuInputRepeatTimers = (u16) gMenuInputRepeatTimers + 1;
                    }
                    if (D_8010AE64 > 0) {
                        D_8010AE64 -= 1;
                        enqueueSoundEffect(0x19, 0x32);
                        var_a3 = 0;
                    }
                } else if ((gPlayerInputPressed & 0x20400) || ((gPlayerInputHeld & 0x20400) && ((s32) (u16) gMenuInputRepeatTimers >= 9))) {
                    if ((u16) gMenuInputRepeatTimers == 0) {
                        gMenuInputRepeatTimers = (u16) gMenuInputRepeatTimers + 1;
                    }
                    if (D_8010AE64 < (s32) var_t1) {
                        D_8010AE64 += 1;
                        enqueueSoundEffect(0x19, 0x32);
                        var_a3 = 0;
                    }
                }
                if ((u16) gMenuInputRepeatTimers != 0) {
                    var_v1_2 = (u16) gMenuInputRepeatTimers + 1;
                    gMenuInputRepeatTimers = var_v1_2;
                    if (var_v1_2 == 0xFFFF) {
                        gMenuInputRepeatTimers = 0xA;
                    }
                }
                if ((sp32 != D_8010AE64) && (sp32 != (u8) sp31) && ((u8) sp31 != D_8010AE64)) {
                    D_8010AECC += 1;
                    if ((s32) (u8) sp31 < D_8010AE64) {
                        D_8010AEE8[0] = 0xFF800000;
                    } else {
                        D_8010AEE8[0] = 0x800000;
                    }
                }
                if (sp32 == D_8010AE64) {
                    var_v0_4 = *(&D_8010AEF7 + sp32);
                    gCourseSelectExtraCourseColumnState = 2;
                } else {
                    var_v0_4 = D_8010AEF8[D_8010AE64];
                    gCourseSelectExtraCourseColumnState = 0;
                }
                gCourseSelectSelectedCourseId = var_v0_4;
                if ((s32) gRacePlayers.unk6 >= 9) {
                    gRacePlayers.unk7 = 0;
                } else {
                    temp_a0_2 = gCourseUnlockSaveSlots[gRacePlayers.unk6];
                    if (temp_a0_2 == -1) {
                        gRacePlayers.unk7 = (u8) ((s32) gRacePlayers.unk6 % 3);
                    } else {
                        gRacePlayers.unk7 = (u8) temp_a0_2;
                    }
                }
                if ((D_8010AEE8[0] == 0) && ((gPlayerInputPressed & 0x1000) || (gPlayerInputPressed & 0x8000))) {
                    enqueueSoundEffect(1, 0x32);
                    var_a3 = 0;
                    if ((gPlayerCount == 1) && (var_t1 == D_8010AE64)) {
                        gMenuTransitionState = 9;
                        gMenuFlowState = 1;
                    } else {
                        D_8010AEA4 = 1;
                        gMenuChoicePromptState = 9;
                        gMenuInputRepeatTimers = 0;
                        gMenuTransitionState = 1;
                        if (gCourseSelectModeSelection == 0) {
                            createCallbackTask(initCourseUnlockPricePanel, 0, 0x62);
                            var_a3 = 0;
                        }
                    }
                } else if (gPlayerInputPressed & 0x4000) {
                    enqueueSoundEffect(1, 0x32);
                    gMenuInputRepeatTimers = 0;
                    var_a3 = 0;
                    var_a1 = gPlayerCount;
                    if ((s32) var_a1 > 0) {
                        var_v0_5 = &gRacePlayers;
                        do {
                            var_v0_5->unk8 = 9;
                            var_v0_5 += 1;
                        } while ((u32) var_v0_5 < (u32) (&gRacePlayers + var_a1));
                    }
                }
            }
            if ((gMenuTransitionState == 1) && (gCourseSelectModeSelection == 1)) {
                setCurrentGameTaskCallback(updateCourseSelectUnlockCourseList, 0);
                gCurrentGameTask->screenState = 0;
            }
        } else {
            if (gMenuChoicePromptState == 9) {
                D_8010AEA4 = (u8) D_8010AEA4 + 1;
            } else {
                D_8010AEA4 = 0;
            }
            if ((gMenuChoicePromptState >= 2) && (gMenuChoicePromptState < 5)) {
                if (!(gPlayerInputHeld & 0x10800) && !(gPlayerInputHeld & 0x20400)) {
                    gMenuInputRepeatTimers = 0;
                }
                temp_t4 = gPlayerInputPressed;
                if ((temp_t4 & 0x10800) || ((gPlayerInputHeld & 0x10800) && ((s32) (u16) gMenuInputRepeatTimers >= 9) && ((u16) gMenuInputRepeatTimers & 1))) {
                    var_v1_2 = (u16) gMenuInputRepeatTimers;
                    if (var_v1_2 == 0) {
                        var_v1_2 = (var_v1_2 + 1) & 0xFFFF;
                    }
                    gMenuInputRepeatTimers = (s16) var_v1_2;
                    if (gMenuChoicePromptState >= 3) {
                        gMenuChoicePromptState -= 1;
                        enqueueSoundEffect(0x19, 0x32);
                        var_a3 = 0;
                        D_8010AF44 = 0;
                    }
                } else {
                    var_v1_2 = (u16) gMenuInputRepeatTimers;
                    if ((temp_t4 & 0x20400) || ((gPlayerInputHeld & 0x20400) && ((s32) var_v1_2 >= 9) && (var_v1_2 & 1))) {
                        if (var_v1_2 == 0) {
                            var_v1_2 = (var_v1_2 + 1) & 0xFFFF;
                        }
                        gMenuInputRepeatTimers = (s16) var_v1_2;
                        if (gMenuChoicePromptState < (D_8010AF3C + 1)) {
                            gMenuChoicePromptState += 1;
                            enqueueSoundEffect(0x19, 0x32);
                            var_a3 = 0;
                            D_8010AF44 = 0;
                        }
                    }
                }
                if ((u16) gMenuInputRepeatTimers != 0) {
                    var_v1_2 = (u16) gMenuInputRepeatTimers + 1;
                    gMenuInputRepeatTimers = var_v1_2;
                    if (var_v1_2 == 0xFFFF) {
                        gMenuInputRepeatTimers = 0xA;
                    }
                }
                if ((*D_8010AEA0 == 1) && (D_8010AE64 == 3)) {
                    var_t8 = *(&D_8010AF06 + gMenuChoicePromptState);
                } else {
                    var_t8 = ((gMenuChoicePromptState * 3) + ((s32) gCourseSelectSelectedCourseId % 3)) - 6;
                }
                gCourseSelectSelectedCourseId = var_t8;
                temp_t4 = gPlayerInputPressed;
                if (temp_t4 & 0x4000) {
                    enqueueSoundEffect(0x18, 0x32);
                    D_8010AF44 = 0;
                    gCourseSelectSelectedCourseId = D_8010AEF8[D_8010AE64];
                    gMenuInputRepeatTimers = 0;
                    var_a3 = 0;
                    gMenuChoicePromptState += 3;
                } else if ((temp_t4 & 0x8000) || (temp_t4 & 0x1000)) {
                    if (gCourseUnlockSaveSlots[gCourseSelectSelectedCourseId] == -1) {
                        if ((u32) gPlayer1Money >= (u32) gCourseUnlockPrices[gCourseSelectSelectedCourseId]) {
                            enqueueSoundEffect(0x49, 0x32);
                            var_a3 = 0;
                            if ((*D_8010AEA0 == 1) && (D_8010AE64 == 3)) {
                                var_v0_6 = *(&D_8010AF06 + gMenuChoicePromptState);
                            } else {
                                var_v0_6 = (((gMenuChoicePromptState * 3) + ((s32) gCourseSelectSelectedCourseId % 3)) - 6) & 0xFF;
                            }
                            gCourseSelectSelectedCourseSavedSlot = gCourseUnlockSaveSlots[var_v0_6];
                            gCourseSelectSelectedCourseId = var_v0_6;
                            if (D_8010AECC == 0) {
                                D_8010AF2C = 0;
                                D_8010AF1C = 7;
                            } else {
                                D_8010AF34 = 0;
                                D_8010AF20 = 7;
                            }
                        } else if (D_8010AF44 == 0) {
                            enqueueSoundEffect(0x47, 0x32);
                            var_a3 = 0;
                            D_8010AF44 = 1;
                        }
                    } else if (D_8010AF44 == 0) {
                        D_8010AF44 = 2;
                    }
                }
            }
        }
        if ((D_8010AF1C == 7) || (D_8010AF20 == 7)) {
            gCurrentGameTask->timer = 0;
            setCurrentGameTaskCallback(updateCourseSelectPurchasePrompt, 0);
            createCallbackTask(initCourseUnlockPurchasePrompt, 0, 0x64);
        }
        var_v0_3 = (u8) D_800EC9C0;
    } else {
        D_800EC9C0 = (u8) D_800EC9C0 + 1;
        var_v0_3 = (u8) D_800EC9C0;
        if ((s32) var_v0_3 >= 0x22) {
            D_800EC9C0 = 0;
            var_v0_3 = 0;
        }
    }
    if (var_v0_3 == 0x1B) {
        sp18.selection = &gRacePlayers + var_a3;
        gCurrentGameTask->fade = 1;
        gMenuTransitionState = 0;
        setCurrentGameTaskCallback(updateCourseSelectModeMenu, 0);
        sp18.selection->pad0[6] = D_8010AEF8[D_8010AE64];
        if (gMenuFlowState == 1) {
            gCourseSelectSelectedCourseId = 0;
        }
        gMenuFlowState = 0;
        D_800EC9C0 = 0;
        gMenuChoicePromptState = 0;
        D_8010AECC = 0;
        gCourseSelectStatus.unk0[0] = 0;
        D_8010AF1C = 0;
        D_8010AF20 = 0;
        D_8010AF24 = 0;
        D_8010AF28 = 0;
        D_8010AF2C = 0;
        D_8010AF34 = 0;
        D_8010AF3C = 0;
        D_8010AF19 = 0;
        D_8010AF1D = 0;
        D_8010AF21 = 0;
        D_8010AF25 = 0;
        D_8010AF29 = 0;
        D_8010AF2E = 0;
        D_8010AF36 = 0;
        D_8010AF3D = 0;
        D_8010AF1A = 0;
        D_8010AF1E = 0;
        D_8010AF22 = 0;
        D_8010AF26 = 0;
        D_8010AF2A = 0;
        D_8010AF30 = 0;
        D_8010AF38 = 0;
        D_8010AF3E = 0;
        D_8010AF1B = 0;
        D_8010AF1F = 0;
        D_8010AF23 = 0;
        D_8010AF27 = 0;
        D_8010AF2B = 0;
        D_8010AF32 = 0;
        D_8010AF3A = 0;
        D_8010AF3F = 0;
        gCourseSelectStatus.transitionState = 0;
        gCourseSelectStatus.unk28 = 0;
        gCourseSelectStatus.unk2A = 0;
        gCourseSelectStatus.unk2C = 0;
        gCourseSelectStatus.unk2E = 0;
    }
    var_a3 = 0;
    if ((s32) gPlayerCount > 0) {
        sp18.object = D_801121E0;
        do {
            gCurrentMenuCameraObject = sp18.object;
            sp18.object->unk2C();
            var_a3 += 1;
            sp18.object += 1;
        } while (var_a3 < (s32) gPlayerCount);
    }
    updateCallbackTasks();
}

#endif

void updateCourseSelectPurchasePrompt(void) {
    s32 i;

    if (gCurrentGameTask->timer < 2) {
        if ((gPlayerInputPressed & (STICK_UP | U_JPAD)) && (gCurrentGameTask->timer != 0)) {
            gCurrentGameTask->timer--;
            enqueueSoundEffect(0x19, 0x32);
        } else if ((gPlayerInputPressed & (STICK_DOWN | D_JPAD)) && (gCurrentGameTask->timer != 1)) {
            gCurrentGameTask->timer++;
            enqueueSoundEffect(0x19, 0x32);
        }

        if (gPlayerInputPressed & A_BUTTON) {
            gMenuFlowState = 1;
            if (gCurrentGameTask->timer == 1) {
                enqueueSoundEffect(0x18, 0x32);
                if (D_8010AECC == 0) {
                    D_8010AF1C = 1;
                } else {
                    D_8010AF20 = 1;
                }
                setCurrentGameTaskCallback(updateCourseSelectCourseList, 0);
            } else {
                enqueueSoundEffect(0x45, 0x32);
                gCurrentGameTask->timer += 2;
            }
        } else if (gPlayerInputPressed & B_BUTTON) {
            gMenuFlowState = 1;
            enqueueSoundEffect(0x18, 0x32);
            if (D_8010AECC == 0) {
                D_8010AF1C = 1;
            } else {
                D_8010AF20 = 1;
            }
            setCurrentGameTaskCallback(updateCourseSelectCourseList, 0);
        }
    } else if (gCurrentGameTask->timer >= 4) {
        gCurrentGameTask->timer = 0;
        gMenuTransitionState = 9;
        setCurrentGameTaskCallback(updateCourseSelectCourseList, 0);
    }

    for (i = 0; i < gPlayerCount; i++) {
        (gCurrentMenuCameraObject = &D_801121E0[i])->unk2C();
    }
    updateCallbackTasks();
}

// updateCourseSelectUnlockCourseList best match: 84.531%
// (nonmatchings/updateCourseSelectUnlockCourseList-1645024839200431810/base_33.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_menu/updateCourseSelectUnlockCourseList.s")

#ifdef NON_MATCHING
void updateCourseSelectUnlockCourseList(void) {
    u16 new_var3;
    u16 *new_var2;
    s32 buttonsHeld;
    s16 state;
    int new_var;
    s32 buttonsPressed;
    s32 held10800;
    u16 repeat;
    u8 new_var4;
    short divisor;
    s16 *new_var5;

    new_var4 = 1;
    if (gMenuChoicePromptState == 9) {
        D_8010AEA4++;
    } else {
        D_8010AEA4 = 0;
    }

    new_var5 = &gMenuChoicePromptState;
    state = *(volatile s16 *)&gMenuChoicePromptState;
    if (state < 2) {
        goto outside_menu_rows;
    }
    if (state >= 5) {
        goto outside_menu_rows;
    }

    buttonsHeld = gPlayerInputHeld;
    held10800 = buttonsHeld & (STICK_UP | U_JPAD);
    if ((held10800 == 0) && ((buttonsHeld & (STICK_DOWN | D_JPAD)) == 0)) {
        gMenuInputRepeatTimers = 0;
    }

    new_var = 0x19;
    buttonsPressed = gPlayerInputPressed;
    repeat = gMenuInputRepeatTimers;
    new_var2 = &repeat;
    new_var3 = repeat;
    if ((buttonsPressed & 0x10800) || ((held10800 != 0) && (new_var3 >= 9) && (new_var3 & new_var4))) {
        if (*new_var2 == 0) {
            repeat++;
        }
        gMenuInputRepeatTimers = *new_var2;
        if (state >= 3) {
            gMenuChoicePromptState = state - new_var4;
            enqueueSoundEffect(new_var, 0x32);
            buttonsPressed = gPlayerInputPressed;
        }
        state = *new_var5;
    } else {
        if (!(buttonsPressed & 0x20400)) {
            gMenuInputRepeatTimers = *new_var2;
            if (!(buttonsHeld & 0x20400)) {
                goto after_row_change;
            }
            gMenuInputRepeatTimers = *new_var2;
            if (*new_var2 < 9) {
                goto after_row_change;
            }
            gMenuInputRepeatTimers = *new_var2;
            if (!(*new_var2 & new_var4)) {
                goto after_row_change;
            }
        }

        if (new_var4) {
        }
        if (new_var4) {
        }
        if (new_var4) {
        }
        if (new_var4) {
        }
        if (new_var4) {
        }
        if (!*new_var2) {
            repeat++;
        }
        gMenuInputRepeatTimers = *new_var2;
        if (state < D_8010AF3C + new_var4) {
            gMenuChoicePromptState = state + 1;
            enqueueSoundEffect(0x19, 0x32);
            state = *new_var5;
            buttonsPressed = gPlayerInputPressed;
        }
    }

after_row_change:
    divisor = 3;

    repeat = gMenuInputRepeatTimers;
    if (*new_var2 != 0) {
        repeat++;
        gMenuInputRepeatTimers = *new_var2;
        if (*new_var2 == 0xFFFF) {
            repeat = 10;
            gMenuInputRepeatTimers = *new_var2;
            do {
            } while (0);
        }
    }

    gCourseSelectSelectedCourseId = ((state * divisor) + (gCourseSelectSelectedCourseId % divisor) - 6) & 0xFF;
    gMenuChoicePromptState = state;
    if (buttonsPressed & B_BUTTON) {
        enqueueSoundEffect(0x18, 0x32);
        gCourseSelectSelectedCourseId = (gCourseSelectSelectedCourseId % divisor) & 0xFF;
        gMenuInputRepeatTimers = 0;
        gMenuChoicePromptState = *new_var5 + 3;
    } else if ((buttonsPressed & 0x8000) || ((buttonsPressed & 0xFFFFu) & 0x1000)) {
        u8 index;

        enqueueSoundEffect(0x18, 0x32);
        index = ((gMenuChoicePromptState * divisor) + (gCourseSelectSelectedCourseId % divisor) - 6) & 0xFF;
        gCourseSelectSelectedCourseSavedSlot = gCourseUnlockSaveSlots[index];
        gCourseSelectSelectedCourseId = index;
        if (D_8010AECC == 0) {
            D_8010AF2C = 0;
            D_8010AF1C = 7;
        } else {
            D_8010AF34 = 0;
            D_8010AF20 = 7;
        }
    }
    goto after_input;

outside_menu_rows:
    if (gMenuChoicePromptState == 0) {
        setCurrentGameTaskCallback(updateCourseSelectCourseList, 0);
    }

after_input:
    if ((gCourseSelectStatus.unk4Array[0] == 7) || (gCourseSelectStatus.unk8Array[0] == 7)) {
        gCurrentGameTask->screenState = 1;
        setCurrentGameTaskCallback(initCourseSelectCourseDetailsMenu, 0);
    }

    {
        ObjectA3E0 *var_s0;
        s32 i;

        i = 0;
        if ((s32) gPlayerCount >
            (((gMenuChoicePromptState * divisor) + (gCourseSelectSelectedCourseId % divisor)) * 0)) {
            var_s0 = D_801121E0;
            do {
                (gCurrentMenuCameraObject = var_s0)->unk2C();
                i++;
                var_s0++;
            } while (i < (s32) gPlayerCount);
        }
    }
    updateCallbackTasks();
}
#endif

void initCourseSelectCourseDetailsMenu(void) {
    ObjectA3E0 *var_s1;
    s32 var_s0;
    s8 temp_v0;

    if (gCurrentGameTask->screenState == 2) {
        createCallbackTask(initCourseDetailsMenu, 0, 0x63);
        temp_v0 = gCourseUnlockSaveSlots[gCourseSelectSelectedCourseId];
        gCourseDetailsMenuSelection = temp_v0 % 7;
        gCourseDetailsPreviewPage = temp_v0 / 7;
        gMenuInputRepeatTimers = 0;
        gMenuExitSelection = 0;
        setCurrentGameTaskCallback(updateCourseSelectCourseDetailsMenu, 0);
    }

 do { var_s0 = 0; if (gPlayerCount > 0) { var_s1 = D_801121E0; do { ; (gCurrentMenuCameraObject = var_s1)->unk2C(); var_s0 += 1; var_s1 += 1; } while (var_s0 < gPlayerCount); } } while (0);
    updateCallbackTasks();
}

// updateCourseSelectCourseDetailsMenu best match: 99.426% (nonmatchings/updateCourseSelectCourseDetailsMenu-8699393380584516020/base_31.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_menu/updateCourseSelectCourseDetailsMenu.s")

#ifdef NON_MATCHING
void updateCourseSelectCourseDetailsMenu(void) {
    s32 i;
    s32 condition;
    u32 soundId;
    u8 selection;
    u8 oldSelection;
    s32 input;
    s32 heldUp;
    s32 tempSelection;
    register u16 repeat;
    s32 divisor;

    soundId = STICK_UP;
    if ((s32) gCourseDetailsMenuState >= 2) {
        selection = gCourseDetailsMenuSelection;
        heldUp = (input = gPlayerInputHeld) & (soundId | U_JPAD);
        oldSelection = selection;
        i = heldUp;
        if (!i && !(gPlayerInputHeld & (STICK_DOWN | D_JPAD))) {
            gMenuInputRepeatTimers = 0;
            if (1) {}
            if (1) {}
            if (1) {}
        }
        divisor = 3;
        if (1) {
        if ((gPlayerInputPressed & (soundId | U_JPAD)) ||
            (heldUp &&
             ((u16) gMenuInputRepeatTimers >= 0xB) &&
            (((u16) gMenuInputRepeatTimers % divisor) == ((gPlayerInputHeld & (STICK_DOWN | D_JPAD)) * 0)))) {
            repeat = (u16) gMenuInputRepeatTimers;
            tempSelection = ((u32) selection) - 1;
            if (!repeat) {
                gMenuInputRepeatTimers = repeat + 1;
                repeat = (u16) gMenuInputRepeatTimers;
            }
            if (selection > 0) {
                gCourseDetailsMenuSelection = tempSelection;
                selection = (u8) tempSelection;
            }
        } else {
            repeat = (u16) gMenuInputRepeatTimers;
            if ((gPlayerInputPressed & (STICK_DOWN | D_JPAD)) ||
                ((gPlayerInputHeld & (STICK_DOWN | D_JPAD)) &&
                 (repeat >= 0xB) && ((repeat % divisor) == 0))) {
                if (!repeat) {
                    gMenuInputRepeatTimers = repeat + 1;
                    repeat = (u16) gMenuInputRepeatTimers;
                }
                if (1) {
                    tempSelection = selection + 1;
                }
                if (selection < 9) {
                    gCourseDetailsMenuSelection = (u16) tempSelection;
                    selection = (u8) tempSelection;
                }
            } else if (gPlayerInputPressed & (STICK_LEFT | L_JPAD)) {
                gMenuExitSelection = 1;
            }
        }
        }
        if (repeat) {
            repeat += 1;
            gMenuInputRepeatTimers = repeat;
            soundId = (repeat & 0xFFFF) & 0xFFFF;
            if ((((((soundId & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) == ((0xFFFF & 0xFFFF) & 0xFFFF)) {
                gMenuInputRepeatTimers = 0xC;
            }
        }
        if (oldSelection != selection) {
            enqueueSoundEffect(0x19, 0x32);
        }
        if (gPlayerInputPressed & B_BUTTON) {
            gMenuExitSelection = 2;
        }
        soundId = 0x18;
        repeat = 1;
        if ((gPlayerInputPressed & A_BUTTON) || (gPlayerInputPressed & START_BUTTON) ||
            ((selection = gMenuExitSelection) != 0)) {
            if (((u8) gCourseDetailsMenuSelection >= 7) || (gMenuExitSelection != 0)) {
                enqueueSoundEffect(soundId, 0x32);
            }
            if ((((u8) gCourseDetailsMenuSelection == 7) || (gMenuExitSelection == 1)) &&
                (gMenuExitSelection != 2)) {
                gCourseDetailsMenuState = divisor;
                if (gMenuExitSelection == repeat) {
                    gCourseDetailsCloseFromBack = repeat;
                }
                gMenuExitSelection = 0;
                setCurrentGameTaskCallback(waitCourseSelectRecordsClose, 0);
            } else {
                input = gMenuExitSelection;
                if (((u8) gCourseDetailsMenuSelection == 8) || (input == 2)) {
                    setCurrentGameTaskCallback(returnToCourseSelectUnlockCourseList, 0);
                    gMenuExitSelection = 0;
                    gCurrentGameTask->screenState = divisor;
                } else if ((u8) gCourseDetailsMenuSelection == 9) {
                    setCurrentGameTaskCallback(returnToCourseSelectModeMenu, 0);
                    gCurrentGameTask->screenState = 9;
                } else {
                    setCurrentGameTaskCallback(initCourseSelectPreview, 0);
                }
            }
        }
    }

    for (i = 0; i < gPlayerCount; i++) {
        (gCurrentMenuCameraObject = &D_801121E0[i])->unk2C();
    }
    updateCallbackTasks();
}
#endif

void waitCourseSelectRecordsClose(void) {
    ObjectA3E0 *var_s1;
    s32 var_s0;

    if (gCourseSelectStatus.transitionState == 2) {
        gMenuInputRepeatTimers = 0;
        gCourseDetailsCloseFromBack = 0;
        setCurrentGameTaskCallback(updateCourseSelectCourseDetailsMenu, 0);
    }

 do { var_s0 = 0; if (gPlayerCount > 0) { var_s1 = D_801121E0; do { ; (gCurrentMenuCameraObject = var_s1)->unk2C(); var_s0 += 1; var_s1 += 1; } while (var_s0 < gPlayerCount); } } while (0);
    updateCallbackTasks();
}

void returnToCourseSelectUnlockCourseList(void) {
    ObjectA3E0 *var_s1;
    s32 var_s0;

    if (gCurrentGameTask->screenState == 5) {
        if (D_8010AECC == 0) {
            gCourseSelectStatus.playerOneCourseDecided = 1;
        } else {
            gCourseSelectStatus.playerTwoCourseDecided = 1;
        }
        gCourseSelectStatus.transitionState = 0;
        gCurrentGameTask->screenState = 0;
        gMenuInputRepeatTimers = 0;
        setCurrentGameTaskCallback(updateCourseSelectUnlockCourseList, 0);
    }

 do { var_s0 = 0; if (gPlayerCount > 0) { var_s1 = D_801121E0; do { ; (gCurrentMenuCameraObject = var_s1)->unk2C(); var_s0 += 1; var_s1 += 1; } while (var_s0 < gPlayerCount); } } while (0);
    updateCallbackTasks();
}

void returnToCourseSelectModeMenu(void) {
    ObjectA3E0 *var_s1;
    s32 var_s0;
    s32 i;

    if (gCurrentGameTask->screenState == 0xB) {
        setCurrentGameTaskCallback(updateCourseSelectModeMenu, 0);
        gCurrentGameTask->fade = 1;
        gCurrentGameTask->timer = 0;
        gCurrentGameTask->screenState = 0;
        gRacePlayers.unk8 = 0;
        gRacePlayers.unk6 = gRacePlayers.unk6 % 3;
        gMenuChoicePromptState = 0;
        D_8010AECC = 0;
        gMenuInputRepeatTimers = 0;

        for (i = 0; i < 4; i++) {
            gCourseSelectStatus.unk0Array[i] = 0;
            gCourseSelectStatus.unk4Array[i] = 0;
            gCourseSelectStatus.unk8Array[i] = 0;
            gCourseSelectStatus.unkCArray[i] = 0;
            gCourseSelectStatus.unk10Array[i] = 0;
            gCourseSelectStatus.unk14[i] = 0;
            gCourseSelectStatus.unk1C[i] = 0;
            gCourseSelectStatus.unk24[i] = 0;
        }

        gCourseSelectStatus.unk28 = 0;
        gCourseSelectStatus.unk2A = 0;
        gCourseSelectStatus.transitionState = 0;
        gCourseSelectStatus.unk2C = 0;
        gCourseSelectStatus.unk2E = 0;
    }

 do { var_s0 = 0; if (gPlayerCount > 0) { var_s1 = D_801121E0; do { ; (gCurrentMenuCameraObject = var_s1)->unk2C(); var_s0 += 1; var_s1 += 1; } while (var_s0 < gPlayerCount); } } while (0);
    updateCallbackTasks();
}

void initCourseSelectPreview(void) {
    ObjectA3E0 *var_s0;
    s8 *temp;

    gCoursePreviewViewportHeight = 0x78;
    configureViewport(1, 0xE8, 0x78, 0x90, gCoursePreviewViewportHeight, 0xA0, 0xF0, 0.6666666865f);
    enableViewportClear(1);
    temp = (s8 *) &gGameSaveDataBuffer[gCourseSelectSelectedCourseId];
    D_8010AED0 = temp[0x3F] + 1;
    temp[0x3F] = gCourseDetailsPreviewCourseTiles[(u8) gCourseDetailsPreviewPage * 7 + (u8) gCourseDetailsMenuSelection];
    gCourseSelectStatus.transitionState = 6;
    createCallbackTask(&initCoursePreviewCloseSparkles, 0, 0x64);
    setCurrentGameTaskCallback(updateCourseSelectPreviewClose, 0); var_s0 = D_801121E0; do { gCurrentMenuCameraObject = var_s0; var_s0->unk2C();
        var_s0 += 1;
    } while (var_s0 != &D_80112340);
    updateCallbackTasks();
    enqueueSoundEffect(0x17, 0x32);
}

void updateCourseSelectPreviewClose(void) {
    ObjectA3E0 *var_s0;

    gCoursePreviewViewportHeight -= 4;
    if (gCoursePreviewViewportHeight < 0) {
        gCoursePreviewViewportHeight = (gCoursePreviewViewportHeight < 0) * 0;
    }
    configureViewport(1, 0xE8, 0x78, 0x90, gCoursePreviewViewportHeight, 0xA0, 0xF0, 0.6666666865f);
    if (gCoursePreviewViewportHeight == 0) {
        resetViewport(1);
        setCurrentGameTaskCallback(updateCourseSelectCourseDetailsMenu, 0);
        gCourseSelectStatus.transitionState = 2;
 D_8010AED0 = 0; } var_s0 = D_801121E0; do { (gCurrentMenuCameraObject = var_s0)->unk2C(); var_s0 += 1; } while (var_s0 != (&D_80112340)); updateCallbackTasks();
}

void exitCourseSelectMenu(void) {
    s8 *ptr;
    s32 count;

    if (gCurrentGameTask->fade != 0xFF) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) gCurrentGameTask->fade, 0x24, 1);
        if (gCurrentGameTask->fade == 0xFF) {
            gFramebufferSwapHold = 1;
        } else {
            updateCallbackTasks();
        }
    } else if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        do { gFramebufferSwapDelay = 0; gMenuFlowState = 0; resumeGameTask(2); removeGameTask(4); count = 0; if (gCourseUnlockSaveSlots[0] == (-1)) { count = 1; } ptr = &D_800EC9F1; loop: if (ptr[0x3F] == (-1)) { count++; } if (ptr[0x40] == (-1)) { count++; } if (ptr[0x41] == (-1)) { count++; } if (ptr[0x42] == (-1)) { count++; } } while (0);
        ptr += 4;
        if (ptr != (&D_800EC9F9)) {
            goto loop;
        }
        if (count == 0) {
            gGameSaveDataBuffer[0x78D7] |= 4;
        }
    }
}
