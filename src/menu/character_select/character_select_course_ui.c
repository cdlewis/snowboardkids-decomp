#include "common.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/character_select/character_select_course_ui.h"
#include "game/menu/splitscreen_select/race_splitscreen_select_ui.h"
#define MENU_RENDERER_BROAD_PROTOTYPES
#define MENU_RENDERER_U32_TILE_INDEX_PROTOTYPE
#include "game/menu/renderer/menu_renderer.h"

#define CHARACTER_SELECT_FRAME_TEXTURE_HANDLE (gAssetHandles.textureHandle)
#define ASSET_HANDLE(index) (((s16 *)&gAssetHandles)[index])

typedef u8 CharacterSelectCourseText[0x94];

typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ s16 value;
    /* 0x01A */ u8 pad1A[0x5F2];
} CharacterSelectCoursePlayerRecord;

typedef struct {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ s16 popupFontHandle;
    /* 0x40 */ u8 pad40[2];
    /* 0x42 */ s16 textureHandle;
    /* 0x44 */ u8 pad44[4];
    /* 0x48 */ s16 iconTextureHandle;
} CharacterSelectCourseAssetHandles;

typedef struct {
    /* 0x00 */ u16 center[16];
    /* 0x20 */ u16 right[2];
    /* 0x24 */ u16 bottom[2];
    /* 0x28 */ u16 corner;
} CharacterSelectCourseFrameTileMap;

typedef struct {
    /* 0x00 */ u16 corner;
    /* 0x02 */ u8 pad2[0x28];
} CharacterSelectCourseFrameCornerTileMap;

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ u8 next;
    /* 0x09 */ u8 pad9[7];
    /* 0x10 */ s16 xNext;
    /* 0x12 */ s16 pad12;
    /* 0x14 */ s16 pad14;
    /* 0x16 */ s16 pad16;
    /* 0x18 */ s16 pad18;
    /* 0x1A */ s16 x0;
    /* 0x1C */ s16 x1;
    /* 0x1E */ s16 x2;
    /* 0x20 */ u8 pad20[6];
    /* 0x26 */ s16 y0;
    /* 0x28 */ s16 y1;
    /* 0x2A */ s16 y2;
    /* 0x2C */ s16 y3;
} CharacterSelectCourseListInitLoop;

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ u8 unk8;
} CharacterSelectCourseRaceState;

typedef union {
    u8 bytes[8];
    struct {
        /* 0x00 */ u8 state;
        /* 0x01 */ u8 otherState;
        /* 0x02 */ u8 pad2[2];
        /* 0x04 */ s16 spriteIndex;
        /* 0x06 */ u8 timer;
        /* 0x07 */ u8 otherTimer;
    } fields;
} CharacterSelectCourseCursorState;

enum {
    COURSE_LIST_SLIDE_IN,
    COURSE_LIST_IDLE,
    COURSE_LIST_SLIDE_OTHERS_OUT,
    COURSE_LIST_MOVE_SELECTED_UP,
    COURSE_LIST_SUBMENU_OPEN,
    COURSE_LIST_RESTORE_SELECTED_Y,
    COURSE_LIST_SLIDE_OTHERS_IN,
    COURSE_LIST_EXIT,
    COURSE_LIST_FINISH_EXIT,
    COURSE_LIST_REMOVE,
    COURSE_LIST_SLIDE_SELECTED_OUT
};

extern void drawAssetTableSpriteWithExplicitPalette(s16, s16, s32, s32, s32);
extern int sprintf(char *, const char *, ...);
extern CharacterSelectCourseAssetHandles gAssetHandles;
extern s16 gCharacterSelectCourseOptionsByUnlock[][11];
extern CharacterSelectCourseFrameTileMap gCharacterSelectCoursePreviewFrameTileMaps[];
extern CharacterSelectCourseFrameCornerTileMap gCharacterSelectCoursePreviewFrameCornerTileMaps[];
extern u16 gCharacterSelectCourseExitPreviewCornerTile;
extern s16 gCharacterSelectShortCourseOptions[];
extern s16 gCharacterSelectSingleCourseOption[];
extern u16 gCharacterSelectCourseStatsScoreValues[];
extern u8 gCharacterSelectCourseDifficultyRatings[];
extern u8 gCharacterSelectCourseMedalScoreThresholds[];
extern u8 gCharacterSelectCourseStartGameText[];
extern CharacterSelectCourseText gCharacterSelectCourseNameText[];
extern u8 gCharacterSelectBeginnerCourseDescriptionText[][0x74];
extern u8 gCharacterSelectAdvancedCourseDescriptionText[][0x60];
extern u8 gCharacterSelectExpertCourseDescriptionText[][0x70];
extern u8 gCharacterSelectCourseReturnDescriptionText[];
extern u8 D_800ECA24[];
extern s32 D_800EC9F8[];
extern s16 gRaceCourseIndex;
extern s32 gActiveMenuTask;
extern u16 gCharacterSelectCourseExitOptionIndex;
extern void *D_8010ADE0;
extern void *D_8010ADE4;
extern CharacterSelectCourseCursorState gCharacterSelectCourseCursorState;
extern u8 gCharacterSelectCourseCursorStateByte;
extern u8 gCharacterSelectCourseSubmenuState;
extern u8 D_8010AE8A;
extern u8 D_8010AE8F;
extern u8 gHighestUnlockedCourse;
extern u8 gMenuTransitionState;
extern u8 gMenuSelectionConfirmTimer;
extern s16 gMenuChoicePromptState;
extern u8 gCourseSelectFromRaceTypeMenu;
extern u8 gRaceSplitscreenMode;
extern u8 gRaceTypeSelection;
extern void *gMenuRenderCallbackList;
extern void addRenderCallback(void *, void *, void *);
extern u8 gPlayerCount;
extern CharacterSelectCourseRaceState gRacePlayers[];
extern CharacterSelectCoursePlayerRecord gFrameCounter;
extern s32 gMenuFlowState;

void drawCharacterSelectCourseListOptions(CharacterSelectCourseMenuFrameActor *arg0) {
    register CharacterSelectCourseMenuFrameActor *actor;
    s16 *characterIds;
    s32 i;
    s32 alpha;

    actor = arg0;
    if ((gRaceSplitscreenMode == 0) || (gRaceSplitscreenMode == 2)) {
        characterIds = gCharacterSelectCourseOptionsByUnlock[gHighestUnlockedCourse];
    } else if (gRaceTypeSelection >= 3) {
        characterIds = gCharacterSelectShortCourseOptions;
    } else {
        characterIds = gCharacterSelectSingleCourseOption;
    }

    i = 0;
    if (actor->itemCount > 0) {
        do {
            alpha = 0;
            if (i == 0) {
                if ((i == gRaceCourseIndex) && (gMenuSelectionConfirmTimer > 0) && (gMenuSelectionConfirmTimer < 8) && (gMenuSelectionConfirmTimer & 1)) {
                    alpha = 0xFF;
                }
                drawMenuSprite(actor->x[i], actor->y[i], getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x1C, 0x20, 0x20, 0, alpha);
            } else if (i == gCharacterSelectCourseExitOptionIndex) {
                if ((gRaceCourseIndex == gCharacterSelectCourseExitOptionIndex) && (gMenuSelectionConfirmTimer > 0) && (gMenuSelectionConfirmTimer < 8) && (gMenuSelectionConfirmTimer & 1)) {
                    alpha = 0xFF;
                }
                if ((gPlayerCount - 1) == 0) {
                    drawMenuSprite(actor->x[i], actor->y[i], getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x1B, 0x20, 0x20, 0, alpha);
                } else {
                    drawMenuSprite(actor->x[i], actor->y[i], getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x1E, 0x20, 0x20, 0, alpha);
                }
            } else {
                if ((i == gRaceCourseIndex) && (gMenuSelectionConfirmTimer > 0) && (gMenuSelectionConfirmTimer < 8) && (gMenuSelectionConfirmTimer & 1)) {
                    alpha = 0xFF;
                }
                drawMenuSprite(actor->x[i], actor->y[i], getRelocatableHeapBlockBase(gAssetHandles.textureHandle), (i + 0x12) & 0xFFFF, 0x20, 0x20, 0, alpha);
            }

            if (i != gCharacterSelectCourseExitOptionIndex) {
                if (((gPlayerCount - 1) == 0) && (D_800ECA24[characterIds[i]] != 0)) {
                    drawMenuSprite((s16)(actor->x[i] - 0x10), actor->y[i], getRelocatableHeapBlockBase(gAssetHandles.iconTextureHandle), (i + 0x1A) & 0xFFFF, 0x20, 0x20, 0, D_800ECA24[characterIds[i]] + 6);
                } else {
                    drawMenuSprite((s16)(actor->x[i] - 0x10), actor->y[i], getRelocatableHeapBlockBase(gAssetHandles.textureHandle), (i + 0x29) & 0xFFFF, 0x20, 0x20, 0, 0);
                }
            }
            i++;
        } while (i < actor->itemCount);
    }
}

void updateCharacterSelectUnlockedCourseList(CharacterSelectCourseMenuFrameActor *arg0) {
    s32 index;
    s32 visibleCourseCount;
    s32 movingCourseCount;
    CharacterSelectCourseMenuFrameActor *actor;

    actor = arg0;
    switch (arg0->state) {
    case COURSE_LIST_SLIDE_IN:
        movingCourseCount = 0;
        for (index = 0; index < actor->itemCount; index++) {
            if (arg0->x[index] < -0x7C) {
                arg0->x[index] += 0x10;
                movingCourseCount++;
                if (arg0->x[index] >= -0x7C) {
                    arg0->x[index] = -0x7C;
                }
            }
        }

        arg0->timer++;
        if (!(arg0->timer & 1)) {
            if (arg0->itemCount < (gCharacterSelectCourseExitOptionIndex + 1)) {
                arg0->itemCount++;
                if (arg0->itemCount == (gCharacterSelectCourseExitOptionIndex + 1)) {
                    D_8010ADE4 = createCallbackTask(initCharacterSelectCoursePreviewFrame, 0, 0x58);
                    createCallbackTask(initCharacterSelectCoursePreviewPanel1, 0, 0x59);
                    createCallbackTask(initCharacterSelectCoursePreviewPanel2, 0, 0x5A);
                    createCallbackTask(initCharacterSelectCoursePreviewPanel3, 0, 0x5B);
                    createCallbackTask(initCharacterSelectCoursePreviewPanel4, 0, 0x5C);
                    createCallbackTask(initCharacterSelectCoursePreviewPanel5, 0, 0x5D);
                    if (gHighestUnlockedCourse != 0) {
                        createCallbackTask(initCharacterSelectCoursePreviewPanel6, 0, 0x5E);
                        if (gHighestUnlockedCourse >= 2) {
                            createCallbackTask(initCharacterSelectCoursePreviewPanel7, 0, 0x5F);
                            if (gHighestUnlockedCourse >= 3) {
                                createCallbackTask(initCharacterSelectCoursePreviewPanel8, 0, 0x60);
                            }
                        }
                    }
                    createCallbackTask(initCharacterSelectCourseExitPreviewPanel, 0, 0x61);
                }
            }
        }
        if (movingCourseCount == 0) {
            arg0->state = COURSE_LIST_IDLE;
        }
        break;

    case COURSE_LIST_IDLE:
        if (gMenuTransitionState == 1) {
            arg0->state = COURSE_LIST_SLIDE_OTHERS_OUT;
        }
        if (gMenuTransitionState == 7) {
            arg0->state = COURSE_LIST_EXIT;
        }
        break;

    case COURSE_LIST_SLIDE_OTHERS_OUT:
        for (index = 0; index < 11; index++) {
            if (index != gRaceCourseIndex) {
                arg0->x[index] -= 0x20;
            }
        }

        if (gRaceCourseIndex != 0) {
            if (arg0->x[0] < -0x103) {
                arg0->state = COURSE_LIST_MOVE_SELECTED_UP;
            }
        } else if (arg0->x[1] < -0x103) {
            arg0->state = COURSE_LIST_MOVE_SELECTED_UP;
        }
        break;

    case COURSE_LIST_MOVE_SELECTED_UP:
        arg0->y[gRaceCourseIndex] -= arg0->itemSpacing;
        if (arg0->y[gRaceCourseIndex] < -0x5F) {
            arg0->y[gRaceCourseIndex] = -0x60;
            arg0->state = COURSE_LIST_SUBMENU_OPEN;
            if (gRaceSplitscreenMode == 2) {
                D_8010ADE0 = createCallbackTask(initCharacterSelectCourseExitPopup, 0, 0x62);
                createCallbackTask(initCharacterSelectCourseRecordsPopup, 0, 0x63);
            } else {
                createCallbackTask(initCharacterSelectCoursePlayerStatsPanel, 0, 0x61);
                createCallbackTask(initCharacterSelectCourseSubmenuFrame, 0, 0x62);
            }
        }
        break;

    case COURSE_LIST_SUBMENU_OPEN:
        if (gMenuTransitionState == 4) {
            arg0->state = COURSE_LIST_RESTORE_SELECTED_Y;
        }
        if (gMenuTransitionState == 7) {
            arg0->state = COURSE_LIST_EXIT;
        }
        break;

    case COURSE_LIST_RESTORE_SELECTED_Y:
        arg0->y[gRaceCourseIndex] += arg0->itemSpacing;
        if (arg0->y[gRaceCourseIndex] >= (arg0->baseY + (gRaceCourseIndex * arg0->itemSpacing))) {
            arg0->y[gRaceCourseIndex] = arg0->baseY + (gRaceCourseIndex * arg0->itemSpacing);
            arg0->state = COURSE_LIST_SLIDE_OTHERS_IN;
        }
        break;

    case COURSE_LIST_SLIDE_OTHERS_IN:
        visibleCourseCount = gCharacterSelectCourseExitOptionIndex + 1; index = 0; if (visibleCourseCount > 0) { do {
            if (index != gRaceCourseIndex) {
                arg0->x[index] += 0x20;
                visibleCourseCount = gCharacterSelectCourseExitOptionIndex + 1;
            }
            index++;
        } while (index < (gCharacterSelectCourseExitOptionIndex + 1));
        }
        if (gRaceCourseIndex != 0) {
            if (arg0->x[0] >= -0x7C) {
                arg0->state = COURSE_LIST_IDLE;
            }
        } else if (arg0->x[1] >= -0x7C) {
            arg0->state = COURSE_LIST_IDLE;
        }
        if (arg0->state == COURSE_LIST_IDLE) {
            gMenuTransitionState = 5;
        }
        break;

    case COURSE_LIST_EXIT:
        for (index = 0; index < (gCharacterSelectCourseExitOptionIndex + 1); index++) {
            arg0->x[index] -= 0x20;
        }
        break;
    }

    if ((arg0->state == COURSE_LIST_EXIT) && (arg0->x[0] < -0x103)) {
        removeCallbackTask(arg0);
        return;
    }

    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCourseListOptions, arg0);
}

void initCharacterSelectUnlockedCourseList(CharacterSelectCourseMenuFrameActor *arg0) {
    u8 spacing;
    s16 baseY;
    s32 i;
    int mode;
    unsigned int limit;
    CharacterSelectCourseListInitLoop *loop;

    mode = gHighestUnlockedCourse;
    if (mode == 0) {
        if (1) {}
        spacing = 0x18;
        baseY = -0x50;
    } else if (mode == 1) {
        spacing = 0x16;
        baseY = -0x54;
    } else {
        spacing = 0x13;
        if (mode == 2) {
            spacing = 0x16;
            baseY = -0x60;
        } else {
            baseY = -0x60;
        }
    }

    arg0->baseY = baseY;
    arg0->itemSpacing = spacing;
    do { arg0->y[1] = baseY + spacing; arg0->y[2] = baseY + (spacing * 2); arg0->x[0] = -0x104; arg0->x[1] = -0x104; arg0->x[2] = -0x104; arg0->x[2] = -0x104; arg0->y[0] = baseY; i = 3; loop = (CharacterSelectCourseListInitLoop *)&arg0->pad0[6]; limit = 0xB; do { loop->x0 = -0x104; loop->x1 = -0x104; loop->x2 = -0x104; loop = (CharacterSelectCourseListInitLoop *)&loop->next; loop->xNext = -0x104; loop->y0 = baseY + (i * spacing); loop->y1 = baseY + ((i + 1) * spacing); loop->y2 = baseY + ((i + 2) * spacing); loop->y3 = baseY + ((i + 3) * spacing); i += 4; } while (i != limit); } while (0);

    arg0->timer = 0;
    arg0->itemCount = 1;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectUnlockedCourseList);
}

void drawCharacterSelectLimitedCourseListOptions(CharacterSelectCourseMenuFrameActor *arg0) {
    register CharacterSelectCourseMenuFrameActor *actor;
    s32 arrowTexture;
    volatile u8 pad;
    u16 tile;
    s32 arrowBase;
    s32 lastArrowIndex;
    s32 i;
    s32 alpha;

    actor = arg0;
    arrowBase = (gRaceTypeSelection < 2) ? 2 : 0;
    i = 0;
    lastArrowIndex = arrowBase + 1;
    if (actor->itemCount > 0) {
        do {
            alpha = 0;
            if (i == 0) {
                tile = (gRaceTypeSelection == 2) ? 0x1D : 0x1C;
                if ((i == gRaceCourseIndex) && (gMenuSelectionConfirmTimer > 0) && (gMenuSelectionConfirmTimer < 8) && (gMenuSelectionConfirmTimer & 1)) {
                    alpha = 0xFF;
                }
                drawMenuSprite(actor->x[i], actor->y[i], getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), tile, 0x20, 0x20, 0, alpha);
            } else if (i == lastArrowIndex) {
                if ((i == gRaceCourseIndex) && (gMenuSelectionConfirmTimer > 0) && (gMenuSelectionConfirmTimer < 8) && (gMenuSelectionConfirmTimer & 1)) {
                    alpha = 0xFF;
                }
                drawMenuSprite(actor->x[i], actor->y[i], getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), 0x20, 0x20, 0x20, 0, alpha);
            } else {
                if ((i == gRaceCourseIndex) && (gMenuSelectionConfirmTimer > 0) && (gMenuSelectionConfirmTimer < 8) && (gMenuSelectionConfirmTimer & 1)) {
                    alpha = 0xFF;
                }
                drawMenuSprite(actor->x[i], actor->y[i], getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), (i + 0x12) & 0xFFFF, 0x20, 0x20, 0, alpha);
            }

            if (i != lastArrowIndex) {
                arrowTexture = getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE);
                drawMenuSprite((s16)(actor->x[i] - 0x10), actor->y[i], arrowTexture, (i + 0x29) & 0xFFFF, 0x20, 0x20, 0, 0);
            }
            i++;
        } while (i < actor->itemCount);
    }
}

void updateCharacterSelectLimitedCourseList(CharacterSelectCourseMenuFrameActor *arg0) {
    CharacterSelectCourseMenuFrameActor *actor;
    s32 optionIndex;
    s32 movingOptionCount;
    s32 visibleOptionCount;
    u16 extraCourseOptionCount;

    actor = arg0;
    extraCourseOptionCount = (gRaceTypeSelection < 2) ? 2 : 0;

    switch (arg0->state) {
    case COURSE_LIST_SLIDE_IN:
        movingOptionCount = 0;
        for (optionIndex = 0; optionIndex < actor->itemCount; optionIndex++) {
            if (arg0->x[optionIndex] < -0x7C) {
                arg0->x[optionIndex] += 0x10;
                movingOptionCount++;
                if (arg0->x[optionIndex] >= -0x7C) {
                    arg0->x[optionIndex] = -0x7C;
                }
            }
        }

        arg0->timer++;
        if (!(arg0->timer & 1)) {
            visibleOptionCount = extraCourseOptionCount;
            visibleOptionCount += 2;
            if (arg0->itemCount < visibleOptionCount) {
                arg0->itemCount++;
                if (visibleOptionCount == arg0->itemCount) {
                    D_8010ADE4 = createCallbackTask(initCharacterSelectCoursePreviewFrame, 0, 0x59);
                    if (gRaceTypeSelection < 2) {
                        createCallbackTask(initCharacterSelectCoursePreviewPanel1, 0, 0x5A);
                        createCallbackTask(initCharacterSelectCoursePreviewPanel2, 0, 0x5B);
                    }
                    createCallbackTask(initCharacterSelectCourseExitPreviewPanel, 0, 0x5C);
                    createCallbackTask(initCharacterSelectCourseRecordsFrame, 0, 0x62);
                }
            }
        }

        if (movingOptionCount == 0) {
            arg0->state = COURSE_LIST_IDLE;
        }
        break;

    case COURSE_LIST_IDLE:
        if (gMenuTransitionState == 7) {
            arg0->state = COURSE_LIST_EXIT;
        }
        if (gMenuTransitionState == 1) {
            arg0->state = COURSE_LIST_SLIDE_OTHERS_OUT;
        }
        break;

    case COURSE_LIST_SLIDE_OTHERS_OUT:
        for (optionIndex = 0; optionIndex < actor->itemCount; optionIndex++) {
            if (optionIndex != gRaceCourseIndex) {
                arg0->x[optionIndex] -= 0x20;
            }
        }

        if (gRaceCourseIndex != 0) {
            if (arg0->x[0] < -0x103) {
                arg0->state = COURSE_LIST_MOVE_SELECTED_UP;
            }
        } else if (arg0->x[1] < -0x103) {
            arg0->state = COURSE_LIST_MOVE_SELECTED_UP;
        }
        break;

    case COURSE_LIST_MOVE_SELECTED_UP:
        arg0->y[gRaceCourseIndex] -= arg0->itemSpacing;
        if (arg0->y[gRaceCourseIndex] < -0x57) {
            arg0->y[gRaceCourseIndex] = -0x58;
            arg0->state = COURSE_LIST_SUBMENU_OPEN;
            createCallbackTask(initCharacterSelectCourseRecordsPopup, 0, 0x62);
            D_8010ADE0 = createCallbackTask(initCharacterSelectCourseExitPopup, 0, 0x63);
        }
        break;

    case COURSE_LIST_SUBMENU_OPEN:
        if (gMenuTransitionState == 4) {
            arg0->state = COURSE_LIST_RESTORE_SELECTED_Y;
        }
        if (gMenuTransitionState == 7) {
            arg0->state = COURSE_LIST_SLIDE_SELECTED_OUT;
        }
        break;

    case COURSE_LIST_RESTORE_SELECTED_Y:
        arg0->y[gRaceCourseIndex] += arg0->itemSpacing;
        if (arg0->y[gRaceCourseIndex] >= (arg0->baseY + (gRaceCourseIndex * arg0->itemSpacing))) {
            arg0->y[gRaceCourseIndex] = arg0->baseY + (gRaceCourseIndex * arg0->itemSpacing);
            arg0->state = COURSE_LIST_SLIDE_OTHERS_IN;
        }
        break;

    case COURSE_LIST_SLIDE_OTHERS_IN:
        visibleOptionCount = extraCourseOptionCount + 2;
        optionIndex = 0;
        if (visibleOptionCount > 0) {
            do {
                if (optionIndex != gRaceCourseIndex) {
                    arg0->x[optionIndex] += 0x20;
                }
                optionIndex++;
            } while (optionIndex < (extraCourseOptionCount + 2));
        }
        if (gRaceCourseIndex != 0) {
            if (arg0->x[0] >= -0x7C) {
                arg0->state = COURSE_LIST_IDLE;
            }
        } else if (arg0->x[1] >= -0x7C) {
            arg0->state = COURSE_LIST_IDLE;
        }
        if (arg0->state == COURSE_LIST_IDLE) {
            gMenuTransitionState = 5;
        }
        break;

    case COURSE_LIST_EXIT:
        for (optionIndex = 0; optionIndex < arg0->itemCount; optionIndex++) {
            arg0->x[optionIndex] -= 0x20;
        }
        if (arg0->x[0] < -0x103) {
            arg0->state = COURSE_LIST_FINISH_EXIT;
        }
        break;

    case COURSE_LIST_FINISH_EXIT:
        arg0->state = COURSE_LIST_REMOVE;
        break;

    case COURSE_LIST_SLIDE_SELECTED_OUT:
        arg0->x[gRaceCourseIndex] -= 0x20;
        if (arg0->x[gRaceCourseIndex] < -0x103) {
            arg0->state = COURSE_LIST_FINISH_EXIT;
        }
        break;
    }

    if (arg0->state == COURSE_LIST_REMOVE) {
        removeCallbackTask((CallbackTask *)arg0);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectLimitedCourseListOptions, arg0);
    }
}

// initCharacterSelectLimitedCourseList best match: 83.158% (nonmatchings/initCharacterSelectLimitedCourseList-2188069624939011928/base_11.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/character_select/character_select_course_ui/initCharacterSelectLimitedCourseList.s")

#ifdef NON_MATCHING
void initCharacterSelectLimitedCourseList(CharacterSelectCourseMenuFrameActor *arg0) {
    s32 baseY;
    u8 spacing;
    s32 firstBase;
    s32 secondBase;
    u8 spacing1;
    u8 spacing2;
    s32 thirdBase;
    s32 fourthBase;
    s32 tripleSpacing;
    s32 doubleSpacing;
    s32 y1;
    s32 y2;
    s32 y3;
    s32 x;

    baseY = -0x58;
    spacing = 0x18;
    arg0->baseY = baseY;
    firstBase = *(volatile s16 *)&arg0->baseY;
    secondBase = *(volatile s16 *)&arg0->baseY;
    spacing = spacing2 = spacing1 = (arg0->itemSpacing = spacing);
    *(volatile s16 *)&arg0->y[0] = firstBase;
    thirdBase = *(volatile s16 *)&arg0->baseY;
    fourthBase = *(volatile s16 *)&arg0->baseY;
    tripleSpacing = spacing << 2;
    x = -0x104;
    y1 = secondBase + spacing1;
    tripleSpacing -= spacing;
    doubleSpacing = spacing2 << 1;
    spacing1 = 1;
    y2 = thirdBase + doubleSpacing;
    y3 = fourthBase + tripleSpacing;
    arg0->y[3] = y3;
    arg0->y[2] = y2;
    arg0->y[1] = y1;
    arg0->x[1] = x;
    arg0->x[2] = x;
    arg0->x[3] = x;
    arg0->x[0] = x;
    arg0->timer = 0;
    arg0->itemCount = spacing1;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectLimitedCourseList);
}
#endif

// drawCharacterSelectCoursePreviewFrame best match: 99.967% (nonmatchings/drawCharacterSelectCoursePreviewFrame-8909410381742387388/base_25.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/character_select/character_select_course_ui/drawCharacterSelectCoursePreviewFrame.s")

#ifdef NON_MATCHING
void drawCharacterSelectCoursePreviewFrame(CharacterSelectCourseWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 zero;
    s32 offset;
    s32 borderOffset;
    u16 frameIndex;

    if ((gRaceSplitscreenMode == 1) && (gRaceTypeSelection == 2)) {
        frameIndex = 9;
    } else {
        frameIndex = arg0->sprite.unsignedIndex;
    }

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTile((s16)(arg0->x + ((i & 3) << 5)), (s16)(arg0->y + ((i / 4) << 5)),
                           getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                           gCharacterSelectCoursePreviewFrameTileMaps[frameIndex].center[tileOffset], 0, 0x100);
    }

    if (shouldDraw) {
        if (!offset) {
        }
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    if ((i && i) && i) {
    }
    do {
        drawMenuSpriteTile((s16)(arg0->x + 0x80), (s16)(arg0->y + offset),
                           getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                           gCharacterSelectCoursePreviewFrameTileMaps[frameIndex].right[tileOffset], 0, 0x100);
        drawMenuSpriteTile((s16)(arg0->x + offset), (s16)(arg0->y + 0x80),
                           getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                           gCharacterSelectCoursePreviewFrameTileMaps[frameIndex].bottom[tileOffset], 0, 0x100);
        offset += 0x40;
        tileOffset++;
    } while (offset < 0x80);
    i++;
    i--;

    zero = 0;
    drawMenuSpriteTile((s16)(arg0->x + 0x80), (s16)(arg0->y + 0x80),
                       getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                       gCharacterSelectCoursePreviewFrameCornerTileMaps[frameIndex].corner, zero, 0x100);
    drawMenuSprite((s16)(arg0->x - 4), (s16)(arg0->y - 4), getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                   0x33, 0x20, 0x20, zero, zero);
    drawMenuSprite((s16)(arg0->x - 4), (s16)(arg0->y + 0x8C), getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                   0x38, 0x20, 0x20, zero, zero);
    drawMenuSprite((s16)(arg0->x + 0x8C), (s16)(arg0->y - 4), getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                   0x35, 0x20, 0x20, zero, zero);
    drawMenuSprite((s16)(arg0->x + 0x8C), (s16)(arg0->y + 0x8C), getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                   0x3A, 0x20, 0x20, zero, zero);

    for (borderOffset = zero; borderOffset != 0x80; borderOffset += 0x10) {
        drawMenuSprite((s16)(arg0->x + borderOffset + 0xC), (s16)(arg0->y - 4),
                       getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x34, 0x20, 0x20, zero, zero);
        drawMenuSprite((s16)(arg0->x + borderOffset + 0xC), (s16)(arg0->y + 0x8C),
                       getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x39, 0x20, 0x20, zero, zero);
        drawMenuSprite((s16)(arg0->x - 4), (s16)(arg0->y + borderOffset + 0xC),
                       getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x36, 0x20, 0x20, zero, zero);
        drawMenuSprite((s16)(arg0->x + 0x8C), (s16)(arg0->y + borderOffset + 0xC),
                       getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x37, 0x20, 0x20, zero, zero);
    }
}
#endif

void updateCharacterSelectCoursePreviewFrame(CharacterSelectCourseWidgetActor *arg0) {
    u8 state;
    int switchState;
    u32 selectedSpriteIndex;
    int menuState;
    register s32 selectedIndex;
    register u16 spriteIndex;

    selectedIndex = gRaceCourseIndex;
    spriteIndex = (u16)arg0->sprite.index;
    selectedSpriteIndex = spriteIndex;
    if ((selectedIndex >= (s32)selectedSpriteIndex) && (arg0->transition.bytes.state != 0) && (arg0->y != -0x48)) {
        switchState = state = arg0->transition.bytes.state = 2;
    } else {
        switchState = state = arg0->transition.bytes.state;
        if ((selectedIndex < (s32)selectedSpriteIndex) && (switchState != 0) && (arg0->y != -0x140)) {
            switchState = state = arg0->transition.bytes.state = 1;
        } else if ((switchState != 0) && (switchState < 4)) {
            switchState++;
            switchState--;
            switchState = state = arg0->transition.bytes.state = 3;
        }
    }

    if (switchState != D_8010AE8A) {
        switchState = state = arg0->transition.bytes.state = D_8010AE8A;
    }

    switch (switchState) {
    case 0:
        arg0->x -= 0x26;
        if (arg0->x < -7) {
            arg0->x = -8;
            arg0->transition.bytes.state = 3;
            gActiveMenuTask = (s32)createCallbackTask(initCharacterSelectCourseListCursor, 0, 0x64);
            createCallbackTask(initCharacterSelectCourseTitleCursor, 0, 0x62);
            createCallbackTask(initCharacterSelectCourseStatsBadge, 0, 0x62);
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        gMenuFlowState += 1;
        menuState = gRacePlayers[0].unk8;
        if (menuState == 1) {
            arg0->transition.bytes.state = 4;
            menuState = gMenuTransitionState;
        }
        if (menuState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (gRacePlayers[0].unk8 == 5) {
            arg0->transition.bytes.state = 3;
            gRacePlayers[0].unk8 = 6;
        }
        if (gRacePlayers[0].unk8 == 7) {
            state = 6 & 0xFF;
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        if (arg0->x >= 0x94) {
            arg0->transition.bytes.state = 6;
        }
        state = arg0->transition.bytes.state;
        break;
    case 6:
        arg0->transition.bytes.state = 7;
        gRacePlayers[0].unk8 = 8;
        state = arg0->transition.bytes.state;
        break;
    }

    D_8010AE8A = state;
    if (arg0->transition.bytes.state == 7) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCoursePreviewFrame, arg0);
}

void initCharacterSelectCoursePreviewFrame(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = 0x96;
    arg0->y = -0x48;
    arg0->sprite.index = 0;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCoursePreviewFrame);
}

void drawCharacterSelectCoursePreviewPanel1(CharacterSelectCourseWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped((s16)(arg0->x + ((i & 3) << 5)), (s16)(arg0->y + ((i / 4) << 5)),
                      getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped((s16)(arg0->x + 0x80), (s16)(arg0->y + offset),
                      getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        drawMenuSpriteTileClipped((s16)(arg0->x + offset), (s16)(arg0->y + 0x80),
                      getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped((s16)(arg0->x + 0x80), (s16)(arg0->y + 0x80), getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                  gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void updateCharacterSelectCoursePreviewPanel1(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((gRaceCourseIndex >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((gRaceCourseIndex < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        gMenuFlowState += 1;
        if (gMenuTransitionState == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (gMenuTransitionState == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    case 6:
        state = arg0->transition.bytes.state = 7;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCoursePreviewPanel1, arg0);
}

void initCharacterSelectCoursePreviewPanel1(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x1;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCoursePreviewPanel1);
}

void drawCharacterSelectCoursePreviewPanel2(CharacterSelectCourseWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(arg0->x + ((i & 3) << 5), arg0->y + ((i / 4) << 5), getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + offset, getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        drawMenuSpriteTileClipped(arg0->x + offset, arg0->y + 0x80, getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + 0x80, getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                  gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void updateCharacterSelectCoursePreviewPanel2(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((gRaceCourseIndex >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((gRaceCourseIndex < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        gMenuFlowState += 1;
        if (gMenuTransitionState == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (gMenuTransitionState == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCoursePreviewPanel2, arg0);
}

void initCharacterSelectCoursePreviewPanel2(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x2;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCoursePreviewPanel2);
}

void drawCharacterSelectCoursePreviewPanel3(CharacterSelectCourseWidgetActor *arg0) {
    s32 i;
    s32 tileOffset;

    tileOffset = 0;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(arg0->x + ((i & 3) << 5), arg0->y + ((i / 4) << 5), getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    tileOffset = 0;
    for (i = 0; i < 2; i++) {
        drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + i * 0x40, getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        drawMenuSpriteTileClipped(arg0->x + i * 0x40, arg0->y + 0x80, getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        tileOffset++;
    }

    drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + 0x80, getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                  gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void updateCharacterSelectCoursePreviewPanel3(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((gRaceCourseIndex >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((gRaceCourseIndex < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        gMenuFlowState += 1;
        if (gMenuTransitionState == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (gMenuTransitionState == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCoursePreviewPanel3, arg0);
}

void initCharacterSelectCoursePreviewPanel3(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x3;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCoursePreviewPanel3);
}

void drawCharacterSelectCoursePreviewPanel4(CharacterSelectCourseWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped((s16)(arg0->x + ((i & 3) << 5)), (s16)(arg0->y + ((i / 4) << 5)),
                      getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped((s16)(arg0->x + 0x80), (s16)(arg0->y + offset),
                      getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        drawMenuSpriteTileClipped((s16)(arg0->x + offset), (s16)(arg0->y + 0x80),
                      getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped((s16)(arg0->x + 0x80), (s16)(arg0->y + 0x80), getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                  gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void updateCharacterSelectCoursePreviewPanel4(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((gRaceCourseIndex >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((gRaceCourseIndex < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        gMenuFlowState += 1;
        if (gMenuTransitionState == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (gMenuTransitionState == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCoursePreviewPanel4, arg0);
}

void initCharacterSelectCoursePreviewPanel4(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x4;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCoursePreviewPanel4);
}

void drawCharacterSelectCoursePreviewPanel5(CharacterSelectCourseWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(arg0->x + ((i & 3) << 5), arg0->y + ((i / 4) << 5), getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + offset, getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        drawMenuSpriteTileClipped(arg0->x + offset, arg0->y + 0x80, getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + 0x80, getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                  gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void updateCharacterSelectCoursePreviewPanel5(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((gRaceCourseIndex >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((gRaceCourseIndex < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        gMenuFlowState += 1;
        if (gMenuTransitionState == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (gMenuTransitionState == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCoursePreviewPanel5, arg0);
}

void initCharacterSelectCoursePreviewPanel5(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x5;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCoursePreviewPanel5);
}

void drawCharacterSelectCoursePreviewPanel6(CharacterSelectCourseWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped((s16)(arg0->x + ((i & 3) << 5)), (s16)(arg0->y + ((i / 4) << 5)),
                      getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped((s16)(arg0->x + 0x80), (s16)(arg0->y + offset),
                      getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        drawMenuSpriteTileClipped((s16)(arg0->x + offset), (s16)(arg0->y + 0x80),
                      getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped((s16)(arg0->x + 0x80), (s16)(arg0->y + 0x80), getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                  gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void updateCharacterSelectCoursePreviewPanel6(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((gRaceCourseIndex >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((gRaceCourseIndex < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        gMenuFlowState += 1;
        if (gMenuTransitionState == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (gMenuTransitionState == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCoursePreviewPanel6, arg0);
}

void initCharacterSelectCoursePreviewPanel6(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x6;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCoursePreviewPanel6);
}

void drawCharacterSelectCoursePreviewPanel7(CharacterSelectCourseWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(arg0->x + ((i & 3) << 5), arg0->y + ((i / 4) << 5), getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + offset, getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        drawMenuSpriteTileClipped(arg0->x + offset, arg0->y + 0x80, getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + 0x80, getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                  gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void updateCharacterSelectCoursePreviewPanel7(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((gRaceCourseIndex >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((gRaceCourseIndex < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        gMenuFlowState += 1;
        if (gMenuTransitionState == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (gMenuTransitionState == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCoursePreviewPanel7, arg0);
}

void initCharacterSelectCoursePreviewPanel7(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x7;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCoursePreviewPanel7);
}

void drawCharacterSelectCoursePreviewPanel8(CharacterSelectCourseWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileIndex;
    s32 offset;

    tileIndex = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileIndex++) {
        drawMenuSpriteTileClipped((s16)(arg0->x + ((i & 3) << 5)), (s16)(arg0->y + ((i / 4) << 5)),
                      getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[tileIndex], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileIndex = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped((s16)(arg0->x + 0x80), (s16)(arg0->y + offset), getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[tileIndex], 0, 0x100, 0xA0, 0x49);
        drawMenuSpriteTileClipped((s16)(arg0->x + offset), (s16)(arg0->y + 0x80), getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileIndex], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileIndex++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped((s16)(arg0->x + 0x80), (s16)(arg0->y + 0x80), getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                  gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void updateCharacterSelectCoursePreviewPanel8(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((gRaceCourseIndex >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((gRaceCourseIndex < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        gMenuFlowState += 1;
        if (gMenuTransitionState == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (gMenuTransitionState == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCoursePreviewPanel8, arg0);
}

void initCharacterSelectCoursePreviewPanel8(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x8;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCoursePreviewPanel8);
}

void drawCharacterSelectCoursePreviewPanel9(CharacterSelectCourseWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    register s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(arg0->x + ((i & 3) << 5), arg0->y + ((i / 4) << 5), getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + offset, getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        drawMenuSpriteTileClipped(arg0->x + offset, arg0->y + 0x80, getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + 0x80, getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                  gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void updateCharacterSelectCoursePreviewPanel9(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((gRaceCourseIndex >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((gRaceCourseIndex < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        gMenuFlowState += 1;
        if (gMenuTransitionState == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (gMenuTransitionState == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCoursePreviewPanel9, arg0);
}

void initCharacterSelectCoursePreviewPanel9(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x9;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCoursePreviewPanel9);
}

void drawCharacterSelectCourseExitPreviewPanel(CharacterSelectCourseWidgetActor *arg0) {
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(arg0->x + ((i & 3) << 5), arg0->y + ((i / 4) << 5), getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                      gCharacterSelectCoursePreviewFrameTileMaps[10].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    tileOffset = 0; offset = 0; i = 0x80; do {
        drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + offset, getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                      gCharacterSelectCoursePreviewFrameTileMaps[10].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        drawMenuSpriteTileClipped(arg0->x + offset, arg0->y + 0x80, getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                      gCharacterSelectCoursePreviewFrameTileMaps[10].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);

    i++;
    i--;

    drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + 0x80, getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                  gCharacterSelectCourseExitPreviewCornerTile, 0, 0x100, 0xA0, 0x49);
}

void updateCharacterSelectCourseExitPreviewPanel(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->row.bytes.subState;

    if ((gRaceCourseIndex == gCharacterSelectCourseExitOptionIndex) && (arg0->y != -0x48) && ((s32) arg0->row.bytes.subState < 6)) {
        state = arg0->row.bytes.subState = 2;
    } else {
        state = (s32) arg0->row.bytes.subState;
        if ((gRaceCourseIndex != gCharacterSelectCourseExitOptionIndex) && (arg0->y != -0x140) && (state < 6)) {
            state = arg0->row.bytes.subState = 1;
        } else {
            state = arg0->row.bytes.subState;
            if (state < 4) {
                state = arg0->row.bytes.subState = 3;
            }
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->row.bytes.subState = 3;
        }
        state = arg0->row.bytes.subState;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->row.bytes.subState = 6;
            arg0->transition.alpha = 8;
        }
        state = arg0->row.bytes.subState;
        break;
    case 3:
        gMenuFlowState += 1;
        if (gMenuTransitionState == 1) {
            arg0->row.bytes.subState = 4;
        }
        if (gMenuTransitionState == 7) {
            arg0->row.bytes.subState = 5;
        }
        state = arg0->row.bytes.subState;
        break;
    case 4:
        if (gMenuTransitionState == 5) {
            arg0->row.bytes.subState = 3;
        }
        if (gMenuTransitionState == 7) {
            arg0->row.bytes.subState = 5;
        }
        state = arg0->row.bytes.subState;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    case 6:
        arg0->y -= arg0->transition.alpha;
        state = arg0->row.bytes.subState = 7;
        break;
    case 7:
        arg0->y += arg0->transition.alpha;
        arg0->transition.alpha /= 2;
        if (arg0->transition.alpha == 0) {
            arg0->row.bytes.subState = 3;
        } else {
            arg0->row.bytes.subState = 6;
        }
        state = arg0->row.bytes.subState;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCourseExitPreviewPanel, arg0);
}

void initCharacterSelectCourseExitPreviewPanel(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0;
    arg0->row.bytes.subTimer = 0;
    arg0->row.bytes.subState = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCourseExitPreviewPanel);
}

void drawCharacterSelectCourseListCursor(CharacterSelectCourseWidgetActor *arg0) {
    if (arg0->transition.bytes.state < 3) {
        drawMenuSpriteWithAlpha(
            arg0->x,
            (s16)(arg0->y + gRaceCourseIndex * arg0->selection.bytes.rowSpacing),
            getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)),
            0x12,
            0x20,
            0x20,
            0,
            arg0->sprite.index,
            0);
    }
}

void updateCharacterSelectCourseListCursor(CharacterSelectCourseWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = gCharacterSelectCourseCursorState.fields.state)) {
        arg0->transition.bytes.state = globalState;
        state = globalState;
        arg0->sprite.index = gCharacterSelectCourseCursorState.fields.spriteIndex;
        arg0->transition.bytes.timer = gCharacterSelectCourseCursorState.fields.timer;
    }

    /* Preserve IDO's state selector register allocation. */
    switch (state ^ 0) {
    case 0:
        arg0->sprite.index += 0x26;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 1;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        if ((s32) arg0->transition.bytes.timer < 0x10) {
            arg0->sprite.index -= 9;
        } else {
            arg0->sprite.index += 9;
        }
        state = arg0->transition.bytes.state;
        arg0->transition.bytes.timer = (arg0->transition.bytes.timer + 1) & 0x1F;
        break;
    case 2:
        if (gMenuTransitionState == 1) {
            state = arg0->transition.bytes.state = 3;
        }
        break;
    case 3:
        break;
    }

    gCharacterSelectCourseCursorStateByte = state;
    if (gMenuTransitionState == 7) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCourseListCursor, arg0);
}

void initCharacterSelectCourseListCursor(CharacterSelectCourseWidgetActor *arg0) {
    u8 rowSpacing;
    s16 y;
    int mode;

    mode = gHighestUnlockedCourse;
    if (mode == 0) {
        if (1) {}
        rowSpacing = 0x18;
        y = -0x50;
    } else if (mode == 1) {
        y = -0x54;
        rowSpacing = 0x16;
    } else if (mode == 2) {
        rowSpacing = 0x16;
        y = -0x60;
    } else {
        y = -0x60;
        rowSpacing = 0x13;
    }

    if (gCourseSelectFromRaceTypeMenu != 0) {
        y = -0x58;
        rowSpacing = 0x18;
    }

    arg0->x = -0x7C;
    arg0->y = y;
    arg0->selection.bytes.rowSpacing = rowSpacing;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    arg0->transition.bytes.timer = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCourseListCursor);
}

void drawCharacterSelectCourseTitleCursor(CharacterSelectCourseWidgetActor *arg0) {
    drawMenuSpriteWithAlpha(arg0->x, arg0->y, getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)), 1, 0x20, 0x20, 0, arg0->sprite.index, 0);
}

void updateCharacterSelectCourseTitleCursor(CharacterSelectCourseWidgetActor *arg0) {
    u8 state = arg0->transition.bytes.state;

    switch (state) {
    case 0:
        arg0->sprite.index += 0x26;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 1;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        if (gMenuTransitionState == 1) {
            arg0->transition.bytes.state = 2;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        if (gMenuTransitionState == 5) {
            arg0->transition.bytes.state = 1;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        arg0->x += 0x20;
        if (arg0->x >= 0x94) {
            arg0->transition.bytes.state = 4;
        }
        state = arg0->transition.bytes.state;
        break;
    }
    if ((unsigned int)state == 4) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCourseTitleCursor, arg0);
}

void initCharacterSelectCourseTitleCursor(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x5C;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCourseTitleCursor);
}

const char gCharacterSelectCourseStatsScoreFormat[] = "%4d";

void drawCharacterSelectCourseStatsBadge(CharacterSelectCourseWidgetActor *arg0) {
    char pad[8];
    char buf[4];
    s32 temp_v0;
    s32 xOffset;
    s32 i;
    s32 var_t8;

    if (gRaceCourseIndex != gCharacterSelectCourseExitOptionIndex) {
        drawMenuPanelBackdrop(arg0->x, arg0->y, 0x3800, 0x5800);
        drawMenuSpriteWithAlpha((s16)(arg0->x + 4), (s16)(arg0->y + 4), getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      0x23, 0x20, 0x20, 0, arg0->sprite.index, 0);
        sprintf(buf - 4, gCharacterSelectCourseStatsScoreFormat, gCharacterSelectCourseStatsScoreValues[gRaceCourseIndex]);
        drawMenuAsciiText((s16)(arg0->x + 8), (s16)(arg0->y + 0xC), buf - 4, 0, arg0->sprite.index);
        drawMenuSpriteWithAlpha((s16)(arg0->x + 0x28), (s16)(arg0->y + 0xC), getRelocatableHeapBlockBase(gAssetHandles.textureHandle),
                      0x24, 0x20, 0x20, 0, arg0->sprite.index, 0);
        drawMenuSpriteWithAlpha(arg0->x, (s16)(arg0->y + 0x14), getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x25, 0x20,
                      0x20, 0, arg0->sprite.index, 0);

        temp_v0 = gCharacterSelectCourseDifficultyRatings[gRaceCourseIndex];
        xOffset = 0;
        i = 0;
        var_t8 = temp_v0 & 1;
        if ((temp_v0 / 2) > 0) {
            do {
                drawMenuSpriteWithAlpha((s16)(arg0->x + xOffset + 4), (s16)(arg0->y + 0x1C),
                              getRelocatableHeapBlockBase(gAssetHandles.iconTextureHandle), 0x25, 0x20, 0x20, 0,
                              arg0->sprite.index, 0);
                i++;
                temp_v0 = gCharacterSelectCourseDifficultyRatings[gRaceCourseIndex];
                xOffset += 0xC;
            } while (i < (temp_v0 / 2));
        }
        var_t8 = temp_v0 & 1;
        if (var_t8 != 0) {
            drawMenuSpriteWithAlpha((s16)(arg0->x + xOffset + 4), (s16)(arg0->y + 0x1C),
                          getRelocatableHeapBlockBase(gAssetHandles.iconTextureHandle), 0x26, 0x20, 0x20, 0, arg0->sprite.index,
                          0);
        }
    }
}

void updateCharacterSelectCourseStatsBadge(CharacterSelectCourseWidgetActor *arg0) {
    u8 state = arg0->transition.bytes.state;

    switch (state) {
    case 0:
        arg0->sprite.index += 0x26;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 1;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        if (gMenuTransitionState == 1) {
            arg0->transition.bytes.state = 2;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        if (gMenuTransitionState == 5) {
            arg0->transition.bytes.state = 1;
        }
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        arg0->x += 0x20;
        if (arg0->x >= 0x94) {
            arg0->transition.bytes.state = 4;
        }
        state = arg0->transition.bytes.state;
        break;
    }
    if ((unsigned int)state == 4) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCourseStatsBadge, arg0);
}

void initCharacterSelectCourseStatsBadge(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = 0x48;
    arg0->y = 0x34;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCourseStatsBadge);
}

const char gCharacterSelectCourseBestScoreFormat[] = "%5d";
const char gCharacterSelectCourseMedalScoreFormat[] = "%5d";
const char gCharacterSelectCoursePlayerRankFormat[] = "%2d";

void drawCharacterSelectCoursePlayerStatsPanel(CharacterSelectCourseWidgetActor *arg0) {
    char buf[4];
    s16 *characterIds;
    s32 yOffset;
    s32 valueOffset;
    s32 three;
    CharacterSelectCoursePlayerRecord *player;
    do { characterIds = gCharacterSelectCourseOptionsByUnlock[gHighestUnlockedCourse]; if (gPlayerCount == 1) { drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x21, 0x20, 0x20, 0, 0); drawMenuSprite((s16)(arg0->x + 0x30), arg0->y, getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x22, 0x20, 0x20, 0, 0); sprintf(buf - 0x10, gCharacterSelectCourseBestScoreFormat, D_800EC9F8[characterIds[*(&gRaceCourseIndex)]]); drawMenuAsciiText((s16)(arg0->x + 0x14), (s16)(arg0->y + 0x2A), buf - 0x10, 0, 0x100); valueOffset = 0; yOffset = 0; three = 3; do { sprintf(buf - 0x10, gCharacterSelectCourseMedalScoreFormat, *((u16 *)(&gCharacterSelectCourseMedalScoreThresholds[(((*(&gRaceCourseIndex)) * three) * 2) + valueOffset]))); drawMenuAsciiText((s16)(arg0->x + 0x28), (s16)((arg0->y + yOffset) + 9), buf - 0x10, 0, 0x100); yOffset += 8; valueOffset += 2; } while (yOffset != 0x18); } else { drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x26, 0x20, 0x20, 0, 0); drawMenuSprite(arg0->x, (s16)(arg0->y + 0x10), getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x27, 0x20, 0x20, 0, 0); player = (CharacterSelectCoursePlayerRecord *)gRacePlayers; yOffset = 0; do { sprintf(buf - 0x10, gCharacterSelectCoursePlayerRankFormat, player->value); drawMenuAsciiText((s16)(arg0->x + 0x40), (s16)((arg0->y + yOffset) + 0x10), buf - 0x10, 0, 0x100); player++; yOffset += 8; } while (player != &gFrameCounter); } } while (0);
}

void updateCharacterSelectCoursePlayerStatsPanel(CharacterSelectCourseWidgetActor *arg0) {
    u8 state = arg0->sprite.bytes.state;
    s16 target;

    switch (state) {
    case 0:
        if (gPlayerCount == 1) {
            target = -0x78;
        } else {
            target = -0x80;
        }
        arg0->x += 0x20;
        if (arg0->x >= target) {
            arg0->x = target;
            arg0->sprite.bytes.state = 1;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 1:
        if ((gMenuTransitionState == 3) || (gMenuTransitionState == 7)) {
            state = arg0->sprite.bytes.state = 2;
        }
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0x107) {
            arg0->sprite.bytes.state = 3;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 3:
        break;
    }
    if ((unsigned int)state == 3) {
        removeCallbackTask(arg0);
        if (gRacePlayers[0].unk8 == 3) {
            gRacePlayers[0].unk8 = 4;
        }
    } else {
        addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCoursePlayerStatsPanel, arg0);
    }
}

void initCharacterSelectCoursePlayerStatsPanel(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = -0x50;
    arg0->sprite.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCoursePlayerStatsPanel);
}

void drawCharacterSelectCourseSubmenuFrame(CharacterSelectCourseWidgetActor *arg0) {
    drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)), 3, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16) (arg0->x + 0x40), arg0->y, getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)), 4, 0x20, 0x20, 0, 0);
    drawMenuSprite(arg0->x, (s16) (arg0->y + 0x40), getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)), 5, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16) (arg0->x + 0x40), (s16) (arg0->y + 0x40), getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)), 6, 0x20,
                  0x20, 0, 0);
}

void updateCharacterSelectCourseSubmenuFrame(CharacterSelectCourseWidgetActor *arg0) {
    u8 state = arg0->sprite.bytes.state;

    switch (state) {
    case 0:
        arg0->x += 0x20;
        if (arg0->x >= -0x88) {
            arg0->x = -0x88;
            arg0->sprite.bytes.state = 1;
            D_8010ADE0 = createCallbackTask(initCharacterSelectCourseNamePopup, 0, 0x63);
        }
        state = arg0->sprite.bytes.state;
        break;
    case 1:
        if ((gMenuTransitionState == 3) || (gMenuTransitionState == 7)) {
            state = arg0->sprite.bytes.state = 2;
        }
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0x107) {
            arg0->sprite.bytes.state = 3;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 3:
        state = arg0->sprite.bytes.state = 4;
        break;
    }
    state = arg0->sprite.bytes.state;
    if (state == 4) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCourseSubmenuFrame, arg0);
}

void initCharacterSelectCourseSubmenuFrame(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = -0x18;
    arg0->sprite.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCourseSubmenuFrame);
}

void drawCharacterSelectCourseRecordsFrame(CharacterSelectCourseWidgetActor *arg0) {
    drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)), 3, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16) (arg0->x + 0x40), arg0->y, getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)), 4, 0x20, 0x20, 0, 0);
    drawMenuSprite(arg0->x, (s16) (arg0->y + 0x40), getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)), 5, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16) (arg0->x + 0x40), (s16) (arg0->y + 0x40), getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)), 6, 0x20, 0x20, 0, 0);
}

void updateCharacterSelectCourseRecordsFrame(CharacterSelectCourseWidgetActor *arg0) {
    u32 state = arg0->sprite.bytes.state;

    switch (state) {
    case 0:
        arg0->x += 0x20;
        if (arg0->x >= -0x88) {
            arg0->x = -0x88;
            arg0->sprite.bytes.state = 1;
            createCallbackTask(initCharacterSelectCourseDescriptionPopup, 0, 0x63);
        }
        state = arg0->sprite.bytes.state;
        break;
    case 1:
        if (gMenuTransitionState == 7) {
            arg0->sprite.bytes.state = 5;
        }
        if (gMenuTransitionState == 1) {
            arg0->sprite.bytes.state = 2;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0x127) {
            arg0->sprite.bytes.state = 3;
            arg0->x = -0x128;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 3:
        if (gMenuTransitionState == 5) {
            state = arg0->sprite.bytes.state = 4;
        }
        break;
    case 4:
        arg0->x += 0x20;
        if (arg0->x >= -0x88) {
            arg0->x = -0x88;
            arg0->sprite.bytes.state = 1;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 5:
        arg0->x -= 0x20;
        if (arg0->x < -0x107) {
            arg0->sprite.bytes.state = 6;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 6:
        state = arg0->sprite.bytes.state = 7;
        break;
    }
    if (state == 7) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCourseRecordsFrame, arg0);
}

void initCharacterSelectCourseRecordsFrame(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x128;
    arg0->y = 0x8;
    arg0->sprite.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCourseRecordsFrame);
}

void drawCharacterSelectCourseNamePopup(CharacterSelectCourseWidgetActor *arg0) {
    CharacterSelectCourseText *texts;
    u8 *text;

    texts = gCharacterSelectCourseNameText;
    text = texts[gRaceCourseIndex];
    drawMenuGlyphScript(arg0->x, arg0->y, text, 1, arg0->sprite.index, 0);
    if (arg0->transition.bytes.state == 2) {
        drawMenuSprite(
            (s16)(arg0->x + 0x60),
            (s16)(arg0->y + 0x40),
            getRelocatableHeapBlockBase(ASSET_HANDLE(0x24)),
            (((s32)arg0->transition.bytes.timer >= 8) + 5) & 0xFFFF,
            0x20,
            0x20,
            0,
            0);
    }
}

void updateCharacterSelectCourseNamePopup(CharacterSelectCourseWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = gCharacterSelectCourseCursorState.bytes[1])) {
        arg0->transition.bytes.state = globalState;
        state = globalState;
        arg0->transition.bytes.timer = gCharacterSelectCourseCursorState.bytes[7];
    }

    switch (state) {
    case 0:
        if ((++arg0->transition.bytes.timer) == 2) {
            arg0->transition.bytes.state = 1;
            arg0->transition.bytes.timer = state * 0;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        arg0->sprite.index += 0x30;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 2;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->transition.bytes.timer = (arg0->transition.bytes.timer + 1) & 0xF;
        if (gMenuTransitionState == 3) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        state = arg0->transition.bytes.state;
        arg0->transition.bytes.timer = 0;
        break;
    case 4:
        arg0->transition.bytes.timer++;
        if ((gMenuTransitionState == 3) || (gMenuTransitionState == 7)) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x -= 0x20;
        if (arg0->x < -0xFF) {
            arg0->transition.bytes.state = 6;
        }
        state = arg0->transition.bytes.state;
        break;
    case 6:
        break;
    }

    gCharacterSelectCourseSubmenuState = state;
    D_8010AE8F = arg0->transition.bytes.timer;

    if (arg0->transition.bytes.state == 6) {
        removeCallbackTask(arg0);
        D_8010ADE0 = NULL;
        gCharacterSelectCourseCursorState.bytes[1] = 0;
        gCharacterSelectCourseCursorState.bytes[7] = 0;
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCourseNamePopup, arg0);
}

void initCharacterSelectCourseNamePopup(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = -0x14;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCourseNamePopup);
}

void drawCharacterSelectCourseDescriptionPopup(CharacterSelectCourseWidgetActor *arg0) {
    u8 *text;
    s32 threshold;
    u16 idx;

    if (gRaceTypeSelection < 2) {
        threshold = 2;
    } else {
        threshold = 0;
    }
    if (threshold >= gRaceCourseIndex) {
        idx = (u16)gRaceCourseIndex;
        if (gRaceTypeSelection == 0) {
            text = gCharacterSelectBeginnerCourseDescriptionText[idx];
        } else if (gRaceTypeSelection == 1) {
            text = gCharacterSelectAdvancedCourseDescriptionText[idx];
        } else {
            text = gCharacterSelectExpertCourseDescriptionText[idx];
        }
    } else {
        text = gCharacterSelectCourseReturnDescriptionText;
    }
    drawMenuGlyphScript(arg0->x, arg0->y, text, 1, arg0->sprite.index, 0);
}

void updateCharacterSelectCourseDescriptionPopup(CharacterSelectCourseWidgetActor *arg0) {
    u32 state = arg0->transition.bytes.state;

    switch (state) {
    case 0:
        if (++arg0->transition.bytes.timer == 2) {
            arg0->transition.bytes.state = 1;
            arg0->transition.bytes.timer = 0;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        arg0->sprite.index += 0x30;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 2;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        if ((gMenuTransitionState == 7) || (gMenuTransitionState == 1)) {
            state = arg0->transition.bytes.state = 3;
        }
        break;
    case 3:
        arg0->x -= 0x20;
        if (arg0->x < -0x11F) {
            if (gMenuTransitionState == 7) {
                arg0->transition.bytes.state = 7;
            } else {
                arg0->transition.bytes.state = 4;
            }
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (gMenuTransitionState == 5) {
            state = arg0->transition.bytes.state = 5;
        }
        break;
    case 5:
        arg0->x += 0x20;
        if (arg0->x >= -0x84) {
            arg0->transition.bytes.state = 2;
        }
        state = arg0->transition.bytes.state;
        break;
    case 6:
        if (gMenuTransitionState == 7) {
            arg0->transition.bytes.state = 3;
        }
        if (gMenuTransitionState == 0) {
            arg0->transition.bytes.state = 2;
        }
        state = arg0->transition.bytes.state;
        break;
    case 7:
        state = arg0->transition.bytes.state = 8;
        break;
    }
    if (state == 8) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCourseDescriptionPopup, arg0);
}

void initCharacterSelectCourseDescriptionPopup(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = 0xC;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCourseDescriptionPopup);
}

void drawCharacterSelectCourseConfirmCursor(CharacterSelectCourseWidgetActor *arg0) {
    u32 drawAlpha;
    int isEvenState;
    u16 alpha;
    s16 state;

    state = gMenuChoicePromptState;
    if (state != 0) {
        isEvenState = !(state & 1);
        if (isEvenState) {
            alpha = 0x100;
        } else {
            alpha = 0x60;
        }

        drawAlpha = alpha;
        drawMenuSpriteWithAlpha(arg0->x, arg0->sprite.index, getRelocatableHeapBlockBase(ASSET_HANDLE(0x24)), 0x18, 0x20, 0x20, 0, drawAlpha, 0);

        if (drawAlpha == 0x100) {
            alpha = 0x60;
        } else {
            alpha = 0x100;
        }

        drawMenuSpriteWithAlpha(arg0->x, arg0->y, getRelocatableHeapBlockBase(ASSET_HANDLE(0x24)), 0x17, 0x20, 0x20, 0, alpha, 0);

        state = gMenuChoicePromptState;
        if ((state == 3) || (state == 4)) {
            drawMenuSpriteWithAlpha(arg0->x, (s16)(((gMenuChoicePromptState * 0x10) + arg0->y) - 0x30), getRelocatableHeapBlockBase(ASSET_HANDLE(0x24)), 0x12,
                          0x20, 0x20, 0, (u16)arg0->selection.counter, 0);
            state = gMenuChoicePromptState;
        }
    }
    if ((state >= 5) && (((s16)arg0->transition.alpha) == 0)) {
        removeCallbackTask(arg0);
    }
}

void updateCharacterSelectCourseConfirmCursor(CharacterSelectCourseWidgetActor *arg0) {
    s32 step;

    if ((gMenuChoicePromptState != 0) && (gMenuChoicePromptState != 3) && (gMenuChoicePromptState != 4)) {
        step = ((gMenuChoicePromptState < 5) ? 1 : -1) * 8;
        arg0->transition.signedAlpha += step;

        if (arg0->transition.signedAlpha == 0x20) {
            gMenuChoicePromptState += 2;
            arg0->row.value = 0;
            arg0->selection.counter = 0x100;
        }

        if (step == 8) {
            if (arg0->transition.signedAlpha < 0x18) {
                arg0->y += 8;
            }
            arg0->sprite.index += 8;
        } else {
            if (arg0->transition.signedAlpha < 0x10) {
                arg0->y -= 8;
            }
            arg0->sprite.index -= 8;
        }
    }

    if ((gMenuChoicePromptState == 3) || (gMenuChoicePromptState == 4)) {
        if ((u16)arg0->row.value < 0x10) {
            arg0->selection.counter = (u16)arg0->selection.counter - 9;
        } else {
            arg0->selection.counter = (u16)arg0->selection.counter + 9;
        }
        arg0->row.value = ((u16)arg0->row.value + 1) & 0x1F;
    }

    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCourseConfirmCursor, arg0);
}

void initCharacterSelectCourseConfirmCursor(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x74;
    arg0->y = 0x2F;
    arg0->sprite.index = 0x2F;
    setCallbackTaskCallback(arg0, updateCharacterSelectCourseConfirmCursor);
}

// drawCharacterSelectCourseRecordsPopup best match: 97.003% (nonmatchings/drawCharacterSelectCourseRecordsPopup-3379532139742180785/base_20.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/character_select/character_select_course_ui/drawCharacterSelectCourseRecordsPopup.s")

#ifdef NON_MATCHING
typedef struct {
    /* 0x0000 */ u8 pad0[0x4E];
    /* 0x004E */ u8 courseStatsNames[10][5][4];
} CharacterSelectCourseStatsNameData;

typedef struct {
    /* 0x0000 */ u8 pad0[0x156];
    /* 0x0156 */ u8 resultNames[10][5][4];
} CharacterSelectResultNameData;

typedef struct {
    /* 0x0000 */ u8 pad0[0x779C];
    /* 0x779C */ u16 trickValues[5];
} CharacterSelectStatsData;

typedef struct {
    /* 0x0000 */ u8 pad0[0x77E7];
    /* 0x77E7 */ u8 icon;
} CharacterSelectTrickIconData;

typedef struct {
    /* 0x00 */ CharacterSelectTrickIconData *trickIcon;
    /* 0x04 */ u8 pad04[0x15];
    /* 0x19 */ u8 color;
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ u16 quotient;
    /* 0x1E */ u16 remainder;
    /* 0x20 */ u8 pad20[8];
    /* 0x28 */ s16 *courseIds;
    /* 0x2C */ u8 pad2C[0x20];
} CharacterSelectCourseRecordsPopupStack;

extern u8 gGameSaveDataBuffer[];
extern u8 D_800F41EB[];
extern u8 D_800F4222[];
extern u8 D_800F4259[];
extern u8 D_800F4290[];
extern u8 D_800E0B90[];
extern u8 D_800E0B94[];

void drawCharacterSelectCourseRecordsPopup(CharacterSelectCourseWidgetActor *arg0) {
    volatile CharacterSelectCourseRecordsPopupStack stack;
    s16 *selectedCoursePtr;
    s32 quotient;
    s32 rowIndex;
    s32 newQuotient;
    s32 var_s1;
    s32 var_s5;
    s32 var_s7;
    s32 var_s0;
    volatile CharacterSelectCourseAssetHandles *assetHandles;

    if (gRaceSplitscreenMode == 2) {
        stack.courseIds = gCharacterSelectCourseOptionsByUnlock[gHighestUnlockedCourse];
    } else if ((s32)gRaceTypeSelection < 2) {
        stack.courseIds = gCharacterSelectShortCourseOptions;
    } else {
        stack.courseIds = gCharacterSelectSingleCourseOption;
    }

    assetHandles = &gAssetHandles;
    var_s7 = 0;
    var_s5 = 0;
    do {
        rowIndex = var_s7;
        if (rowIndex < 3) {
            stack.color = 6;
        } else {
            stack.color = 4;
        }

        drawMenuSprite(arg0->x, (s16)(arg0->y + var_s5), getRelocatableHeapBlockBase(assetHandles->popupFontHandle),
                      (rowIndex + 0x77) & 0xFFFF, 0x20, 0x20, 0, 0);

        if (gPlayerCount == 1) {
            if (gRaceSplitscreenMode != 2) {
                if (gRaceTypeSelection == 0) {
                    if (rowIndex < 3) {
                        stack.color = 0xC;
                    } else {
                        stack.color = 0xD;
                    }
                    drawCharacterSelectCourseRecordTime(
                        (CharacterSelectCourseRecordTime *)((CharacterSelectResultNameData *)gGameSaveDataBuffer)
                            ->resultNames[stack.courseIds[gRaceCourseIndex]][rowIndex],
                        arg0->x + 0x14, arg0->y + var_s5, stack.color);
                    drawMenuSprite((s16)(arg0->x + 0x54), (s16)(var_s5 + arg0->y),
                                  getRelocatableHeapBlockBase(assetHandles->popupFontHandle),
                                  ((D_800F4290[(stack.courseIds[gRaceCourseIndex] * 5) + rowIndex] & 7) + 0x51) & 0xFFFF,
                                  0x20, 0x20, 0, 0);
                    drawMenuSprite((s16)(arg0->x + 0x65), (s16)(arg0->y + var_s5),
                                  getRelocatableHeapBlockBase(assetHandles->popupFontHandle),
                                  ((D_800F4290[(stack.courseIds[gRaceCourseIndex] * 5) + rowIndex] >> 3) + 0x7C) & 0xFFFF,
                                  0x20, 0x20, 0, 0);
                } else {
                    s32 var_s4;

                    if (gRaceTypeSelection == 1) {
                        if (rowIndex < 3) {
                            var_s4 = 0xC;
                        } else {
                            var_s4 = 0xD;
                        }
                        selectedCoursePtr = &stack.courseIds[gRaceCourseIndex];
                        newQuotient = D_800F4222[((*selectedCoursePtr * 5) + rowIndex)] / 10;
                        quotient = (stack.quotient = newQuotient);
                        if (quotient & 0xFFFF) {
                            drawMenuSprite((s16)(arg0->x + 0x18), (s16)(arg0->y + var_s5),
                                          getRelocatableHeapBlockBase(assetHandles->popupFontHandle),
                                          (stack.quotient + 0x2B) & 0xFFFF, 0x20, 0x20, 0, var_s4 + 1);
                            selectedCoursePtr = &stack.courseIds[gRaceCourseIndex];
                        }
                        stack.remainder = D_800F4222[((*selectedCoursePtr * 5) + rowIndex)] % 10;
                        drawMenuSprite((s16)(arg0->x + 0x20), (s16)(arg0->y + var_s5),
                                      getRelocatableHeapBlockBase(assetHandles->popupFontHandle),
                                      (stack.remainder + 0x2B) & 0xFFFF, 0x20, 0x20, 0, (var_s4 + 1) & 0xFF);
                        drawMenuAsciiText((s16)(arg0->x + 0x28), (s16)(arg0->y + var_s5 + 8), D_800E0B90,
                                          stack.color, 0x100);
                        drawMenuSprite((s16)(arg0->x + 0x48), (s16)(arg0->y + var_s5),
                                      getRelocatableHeapBlockBase(assetHandles->popupFontHandle),
                                      ((D_800F4259[(stack.courseIds[gRaceCourseIndex] * 5) + rowIndex] & 7) + 0x51) &
                                          0xFFFF,
                                      0x20, 0x20, 0, 0);
                        drawMenuSprite((s16)(arg0->x + 0x60), (s16)(arg0->y + var_s5),
                                      getRelocatableHeapBlockBase(assetHandles->popupFontHandle),
                                      ((D_800F4259[(stack.courseIds[gRaceCourseIndex] * 5) + rowIndex] >> 3) + 0x7C) &
                                          0xFFFF,
                                      0x20, 0x20, 0, 0);
                    } else {
                        stack.trickIcon = (CharacterSelectTrickIconData *)&gGameSaveDataBuffer[rowIndex];
                        if (rowIndex < 3) {
                            var_s4 = 0xC;
                        } else {
                            var_s4 = 0xD;
                        }
                        var_s0 = ((CharacterSelectStatsData *)gGameSaveDataBuffer)->trickValues[rowIndex ^ 0];
                        var_s1 = 0;
                        do {
                            drawMenuSprite((s16)((arg0->x - var_s1) + 0x38), (s16)(arg0->y + var_s5),
                                          getRelocatableHeapBlockBase(assetHandles->popupFontHandle),
                                          ((var_s0 % 10) + 0x2B) & 0xFFFF, 0x20, 0x20, 0,
                                          (var_s4 + 1) & 0xFF);
                            var_s1 += 8;
                            var_s0 = var_s0 / 10;
                        } while (var_s0 != 0);
                        drawMenuAsciiText((s16)(arg0->x + 0x40), (s16)(arg0->y + var_s5 + 8), D_800E0B94,
                                          stack.color, 0x100);
                        drawMenuSprite((s16)(arg0->x + 0x4C), (s16)(arg0->y + var_s5),
                                      getRelocatableHeapBlockBase(assetHandles->popupFontHandle),
                                      ((stack.trickIcon->icon & 7) + 0x51) & 0xFFFF, 0x20, 0x20, 0, 0);
                        drawMenuSprite((s16)(arg0->x + 0x60), (s16)(arg0->y + var_s5),
                                      getRelocatableHeapBlockBase(assetHandles->popupFontHandle),
                                      ((stack.trickIcon->icon >> 3) + 0x7C) & 0xFFFF, 0x20, 0x20, 0, 0);
                    }
                }
            } else {
                if (rowIndex < 3) {
                    stack.color = 0xC;
                } else {
                    stack.color = 0xD;
                }
                drawCharacterSelectCourseRecordTime(
                    (CharacterSelectCourseRecordTime *)((CharacterSelectCourseStatsNameData *)gGameSaveDataBuffer)
                        ->courseStatsNames[stack.courseIds[gRaceCourseIndex]][rowIndex],
                    arg0->x + 0x14, arg0->y + var_s5, stack.color);
                drawMenuSprite((s16)(arg0->x + 0x54), (s16)(arg0->y + var_s5),
                              getRelocatableHeapBlockBase(assetHandles->popupFontHandle),
                              ((D_800F41EB[(stack.courseIds[gRaceCourseIndex] * 5) + rowIndex] & 7) + 0x51) & 0xFFFF,
                              0x20, 0x20, 0, 0);
                drawMenuSprite((s16)(arg0->x + 0x65), (s16)(arg0->y + var_s5),
                              getRelocatableHeapBlockBase(assetHandles->popupFontHandle),
                              ((D_800F41EB[(stack.courseIds[gRaceCourseIndex] * 5) + rowIndex] >> 3) + 0x7C) &
                                  0xFFFF,
                              0x20, 0x20, 0, 0);
            }
        }
        var_s7++;
        var_s5 += 0x14;
    } while (var_s7 != 5);
}
#endif

void updateCharacterSelectCourseRecordsPopup(CharacterSelectCourseWidgetActor *arg0) {
    u8 state = arg0->sprite.bytes.state;
    u8 unk;
    s16 target;

    switch (state) {
    case 0:
        unk = gRaceTypeSelection;
        if ((gRaceSplitscreenMode == 2) || ((gRaceSplitscreenMode == 1) && (unk == 0))) {
            target = -0x8A;
        } else {
            target = -0x88;
        }
        arg0->x += 0x20;
        if (arg0->x >= target) {
            arg0->x = target;
            arg0->sprite.bytes.state = 1;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 1:
        if ((gMenuTransitionState == 3) || (gMenuTransitionState == 7)) {
            state = arg0->sprite.bytes.state = 2;
        }
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0xEF) {
            arg0->x = -0xF0;
            arg0->sprite.bytes.state = 3;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 3:
        state = arg0->sprite.bytes.state = 4;
        break;
    case 4:
        break;
    }
    state = arg0->sprite.bytes.state;
    if (state == 4) {
        removeCallbackTask(arg0);
        gMenuTransitionState = 4;
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCourseRecordsPopup, arg0);
}

void initCharacterSelectCourseRecordsPopup(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0xF0;
    arg0->y = -0x40;
    arg0->sprite.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCourseRecordsPopup);
}

void drawCharacterSelectCourseExitPopup(CharacterSelectCourseWidgetActor *arg0) {
    drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)), 0x28, 0x20, 0x20, 0, 0);
    if ((s32) arg0->transition.bytes.state > 0) {
        drawMenuGlyphScript((s16) (arg0->x + 4), (s16) (arg0->y + 4), gCharacterSelectCourseStartGameText, 1, 0x100, 0);
        drawMenuSprite(
            (s16) (arg0->x + 0x64),
            (s16) (arg0->y + 4),
            getRelocatableHeapBlockBase(ASSET_HANDLE(0x24)),
            (u16) ((arg0->transition.bytes.timer >= 8) + 5),
            0x20,
            0x20,
            0,
            0);
    }
}

void updateCharacterSelectCourseExitPopup(CharacterSelectCourseWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = gCharacterSelectCourseCursorState.bytes[1])) {
        arg0->transition.bytes.state = globalState;
        /* Preserve IDO's state/globalState register allocation. */
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        state = globalState;
        arg0->transition.bytes.timer = gCharacterSelectCourseCursorState.bytes[7];
    }

    switch (state) {
    case 0:
        arg0->x += 0x20;
        if (arg0->x >= -0x88) {
            arg0->x = -0x88;
            arg0->transition.bytes.state = 2;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->transition.bytes.timer = (arg0->transition.bytes.timer + 1) & 0xF;
        if (gMenuTransitionState == 3) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        state = arg0->transition.bytes.state;
        arg0->transition.bytes.timer = 0;
        break;
    case 4:
        arg0->transition.bytes.timer++;
        if ((gMenuTransitionState == 3) || (gMenuTransitionState == 7)) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x -= 0x20;
        if (arg0->x < -0x107) {
            arg0->transition.bytes.state = 6;
        }
        state = arg0->transition.bytes.state;
        break;
    }

    gCharacterSelectCourseSubmenuState = state;
    D_8010AE8F = arg0->transition.bytes.timer;

    if (arg0->transition.bytes.state == 6) {
        removeCallbackTask(arg0);
        D_8010ADE0 = NULL;
        gCharacterSelectCourseCursorState.bytes[1] = 0;
        gCharacterSelectCourseCursorState.bytes[7] = 0;
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCourseExitPopup, arg0);
}

void initCharacterSelectCourseExitPopup(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = 0x28;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectCourseExitPopup);
}

// drawCharacterSelectCourseRecordTime best match: 97.948% (nonmatchings/drawCharacterSelectCourseRecordTime-6934502587000073416/base_12.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/character_select/character_select_course_ui/drawCharacterSelectCourseRecordTime.s")

#ifdef NON_MATCHING
extern char D_800E0B98[];
extern char D_800E0BA0[];
extern char D_800E0BA8[];

void drawCharacterSelectCourseRecordTime(CharacterSelectCourseRecordTime *arg0, s32 x, s32 y, s32 alpha) {
    char *end;
    s16 raceType;
    volatile u8 padding[0x58];
    char buffer[8];
    char *ptr;

    sprintf(buffer, D_800E0B98, arg0->minutes);
    alpha &= 0xFFFF;
    if ((!alpha) && (!alpha)) {
    }
    ptr = buffer; end = buffer + 2; do { drawAssetTableSpriteWithExplicitPalette((s16)x, (s16)y, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), ((u8)*ptr - 5) & 0xFFFF, alpha); ptr++; x += 8; } while (ptr < end); drawAssetTableSpriteWithExplicitPalette((s16)x, (s16)y, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x36, alpha);

    if ((gRaceSplitscreenMode == 2) || ((gRaceSplitscreenMode == 1) && ((raceType = gRaceTypeSelection) == 0))) {
        x += 6;
    } else {
        x += 8;
    }

    sprintf(buffer, D_800E0BA0, arg0->seconds);
    ptr = buffer;
    end = buffer + 2;
    end += 0;
    do {
        drawAssetTableSpriteWithExplicitPalette((s16)x, (s16)y, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                      ((u8)*ptr - 5) & 0xFFFF, alpha);
        ptr++;
        x += 8;
    } while (ptr < end);
    drawAssetTableSpriteWithExplicitPalette((s16)x, (s16)y, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x35, alpha);

    x += 8;
    sprintf(buffer, D_800E0BA8, arg0->centiseconds >> 8);
    ptr = buffer;
    end = buffer + 2;
    do {
    } while (0);
    do {
        drawAssetTableSpriteWithExplicitPalette((s16)x, (s16)y, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                      ((u8)*ptr - 5) & 0xFFFF, alpha);
        ptr++;
        x += 8;
    } while (ptr != end);
}
#endif
