#include "common.h"
#include "relocatable_heap.h"
#include "callback_task_scheduler.h"
#include "character_select_course_ui.h"
#include "race_splitscreen_select_ui.h"
#define MENU_RENDERER_BROAD_PROTOTYPES
#include "menu_renderer.h"

#define CHARACTER_SELECT_FRAME_TEXTURE_HANDLE (gAssetHandles.textureHandle)

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

extern void drawAssetTableSpriteWithExplicitPalette(s16, s16, s32, u16, s32);
extern int sprintf(char *, const char *, ...);
extern CharacterSelectCourseAssetHandles gAssetHandles;
extern s16 gMenuCommonSpritesAssetHandle;
extern s16 D_80112178;
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
extern u8 D_80121D80[];
extern CharacterSelectCoursePlayerRecord gFrameCounter;
extern s32 gMenuFlowState;

// drawCharacterSelectCourseListOptions best match: 95.048%
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_course_ui/drawCharacterSelectCourseListOptions.s")

#ifdef NON_MATCHING
void drawCharacterSelectCourseListOptions(CharacterSelectCourseMenuFrameActor *arg0) {
    CharacterSelectCourseMenuFrameActor *base;
    s16 *characterIds;
    s16 *characterId;
    s16 *pos;
    s32 i;
    s32 alpha;
    s32 characterIdOffset;

    base = arg0;
    if ((gRaceSplitscreenMode == 0) || (gRaceSplitscreenMode == 2)) {
        characterIds = gCharacterSelectCourseOptionsByUnlock[gHighestUnlockedCourse];
    } else if (gRaceTypeSelection >= 3) {
        characterIds = gCharacterSelectShortCourseOptions;
    } else {
        characterIds = gCharacterSelectSingleCourseOption;
    }

    i = 0;
    characterIdOffset = 0;
    if (base->itemCount > 0) {
        pos = (s16 *)base;
        do {
            alpha = 0;
            if (i == 0) {
                if ((i == gRaceCourseIndex) && (gMenuSelectionConfirmTimer > 0) && (gMenuSelectionConfirmTimer < 8) && (gMenuSelectionConfirmTimer & 1)) {
                    alpha = 0xFF;
                }
                drawMenuSprite(pos[12], pos[23], getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x1C, 0x20, 0x20, 0, alpha);
            } else if (i == gCharacterSelectCourseExitOptionIndex) {
                if ((gCharacterSelectCourseExitOptionIndex == gRaceCourseIndex) && (gMenuSelectionConfirmTimer > 0) && (gMenuSelectionConfirmTimer < 8) && (gMenuSelectionConfirmTimer & 1)) {
                    alpha = 0xFF;
                }
                if ((gPlayerCount - 1) == 0) {
                    drawMenuSprite(pos[12], pos[23], getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x1B, 0x20, 0x20, 0, alpha);
                } else {
                    drawMenuSprite(pos[12], pos[23], getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x1E, 0x20, 0x20, 0, alpha);
                }
            } else {
                if ((i == gRaceCourseIndex) && (gMenuSelectionConfirmTimer > 0) && (gMenuSelectionConfirmTimer < 8) && (gMenuSelectionConfirmTimer & 1)) {
                    alpha = 0xFF;
                }
                drawMenuSprite(pos[12], pos[23], getRelocatableHeapBlockBase(gAssetHandles.textureHandle), (i + 0x12) & 0xFFFF, 0x20, 0x20, 0, alpha);
            }

            if (i != gCharacterSelectCourseExitOptionIndex) {
                characterId = (s16 *)((u8 *)characterIds + characterIdOffset);
                if (((gPlayerCount - 1) == 0) && (D_800ECA24[*characterId] != 0)) {
                    drawMenuSprite((s16)(pos[12] - 0x10), pos[23], getRelocatableHeapBlockBase(gAssetHandles.iconTextureHandle), (i + 0x1A) & 0xFFFF, 0x20, 0x20, 0, D_800ECA24[*characterId] + 6);
                } else {
                    drawMenuSprite((s16)(pos[12] - 0x10), pos[23], getRelocatableHeapBlockBase(gAssetHandles.textureHandle), (i + 0x29) & 0xFFFF, 0x20, 0x20, 0, 0);
                }
            }
            i++;
            characterIdOffset += 2;
            pos++;
        } while (i < base->itemCount);
    }
}
#endif

// updateCharacterSelectUnlockedCourseList best match: 99.352% (nonmatchings/updateCharacterSelectUnlockedCourseList-4923837976568703863/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_course_ui/updateCharacterSelectUnlockedCourseList.s")

#ifdef NON_MATCHING
void updateCharacterSelectUnlockedCourseList(CharacterSelectCourseMenuFrameActor *arg0) {
    s32 anyMoving;
    s32 i;
    s32 limit;
    s32 targetY;
    void (*finalCallback)(CallbackTask *);
    CharacterSelectCourseMenuFrameActor *base;
    CharacterSelectCourseMenuFrameActor *ptr;
    s16 *x;

    base = arg0;
    switch (arg0->state) {
    case 0:
        anyMoving = 0;
        i = 0;
        if (base->itemCount > 0) {
            ptr = arg0;
            do {
                if (ptr->x[0] < -0x7C) {
                    ptr->x[0] += 0x10;
                    anyMoving++;
                    if (ptr->x[0] >= -0x7C) {
                        ptr->x[0] = -0x7C;
                    }
                }
                i++;
                ptr = (CharacterSelectCourseMenuFrameActor *)((s16 *)ptr + 1);
            } while (i < base->itemCount);
        }

        arg0->timer++;
        if (!(arg0->timer & 1)) {
            if (arg0->itemCount < (gCharacterSelectCourseExitOptionIndex + 1)) {
                arg0->itemCount++;
                if (arg0->itemCount == (gCharacterSelectCourseExitOptionIndex + 1)) {
                    D_8010ADE4 = createCallbackTask((void (*)(CallbackTask *))initCharacterSelectCoursePreviewFrame, 0, 0x58);
                    createCallbackTask((void (*)(CallbackTask *))initCharacterSelectCoursePreviewPanel1, 0, 0x59);
                    createCallbackTask((void (*)(CallbackTask *))initCharacterSelectCoursePreviewPanel2, 0, 0x5A);
                    finalCallback = (void (*)(CallbackTask *))initCharacterSelectCourseExitPreviewPanel;
                    createCallbackTask((void (*)(CallbackTask *))initCharacterSelectCoursePreviewPanel3, 0, 0x5B);
                    createCallbackTask((void (*)(CallbackTask *))initCharacterSelectCoursePreviewPanel4, 0, 0x5C);
                    createCallbackTask((void (*)(CallbackTask *))initCharacterSelectCoursePreviewPanel5, 0, 0x5D);
                    if (gHighestUnlockedCourse != 0) {
                        createCallbackTask((void (*)(CallbackTask *))initCharacterSelectCoursePreviewPanel6, 0, 0x5E);
                        if (gHighestUnlockedCourse >= 2) {
                            createCallbackTask((void (*)(CallbackTask *))initCharacterSelectCoursePreviewPanel7, 0, 0x5F);
                            if (gHighestUnlockedCourse >= 3) {
                                createCallbackTask((void (*)(CallbackTask *))initCharacterSelectCoursePreviewPanel8, 0, 0x60);
                            }
                        }
                    }
                    createCallbackTask(finalCallback, 0, 0x61);
                }
            }
        }
        if (anyMoving == 0) {
            arg0->state = 1;
        }
        break;
    case 1:
        if (gMenuTransitionState == 1) {
            arg0->state = 2;
        }
        if (gMenuTransitionState == 7) {
            arg0->state = 7;
        }
        break;
    case 2:
        if (gRaceCourseIndex != 0) {
            arg0->x[0] -= 0x20;
        }
        if (gRaceCourseIndex != 1) {
            arg0->x[1] -= 0x20;
        }
        if (gRaceCourseIndex != 2) {
            arg0->x[2] -= 0x20;
        }
        i = 3;
        do {
            x = (s16 *)arg0;
            if (i != gRaceCourseIndex) {
                x[i + 12] -= 0x20;
            }
            if ((i + 1) != gRaceCourseIndex) {
                x[i + 13] -= 0x20;
            }
            if ((i + 2) != gRaceCourseIndex) {
                x[i + 14] -= 0x20;
            }
            if ((i + 3) != gRaceCourseIndex) {
                x[i + 15] -= 0x20;
            }
            i += 4;
        } while (i != 0xB);

        if (gRaceCourseIndex != 0) {
            if (arg0->x[0] < -0x103) {
                arg0->state = 3;
            }
        } else if (arg0->x[1] < -0x103) {
            arg0->state = 3;
        }
        break;
    case 3:
        arg0->y[gRaceCourseIndex] -= arg0->itemSpacing;
        if (arg0->y[gRaceCourseIndex] < -0x5F) {
            arg0->y[gRaceCourseIndex] = -0x60;
            arg0->state = 4;
            if (gRaceSplitscreenMode == 2) {
                D_8010ADE0 = createCallbackTask((void (*)(CallbackTask *))initCharacterSelectCourseExitPopup, 0, 0x62);
                createCallbackTask((void (*)(CallbackTask *))initCharacterSelectCourseRecordsPopup, 0, 0x63);
            } else {
                createCallbackTask((void (*)(CallbackTask *))initCharacterSelectCoursePlayerStatsPanel, 0, 0x61);
                createCallbackTask((void (*)(CallbackTask *))initCharacterSelectCourseSubmenuFrame, 0, 0x62);
            }
        }
        break;
    case 4:
        if (gMenuTransitionState == 4) {
            arg0->state = 5;
        }
        if (gMenuTransitionState == 7) {
            arg0->state = 7;
        }
        break;
    case 5:
        arg0->y[gRaceCourseIndex] += arg0->itemSpacing;
        targetY = (gRaceCourseIndex * arg0->itemSpacing) + arg0->baseY;
        if (arg0->y[gRaceCourseIndex] >= targetY) {
            arg0->y[gRaceCourseIndex] = targetY;
            arg0->state = 6;
        }
        break;
    case 6:
        limit = gCharacterSelectCourseExitOptionIndex + 1;
        i = 0;
        if (limit > 0) {
            do {
                if (i != gRaceCourseIndex) {
                    arg0->x[i] += 0x20;
                    limit = gCharacterSelectCourseExitOptionIndex + 1;
                }
                i++;
            } while (i < (gCharacterSelectCourseExitOptionIndex + 1));
        }
        if (gRaceCourseIndex != 0) {
            if (arg0->x[0] >= -0x7C) {
                arg0->state = 1;
            }
        } else if (arg0->x[1] >= -0x7C) {
            arg0->state = 1;
        }
        if (arg0->state == 1) {
            gMenuTransitionState = 5;
        }
        break;
    case 7:
        i = 0;
        if ((gCharacterSelectCourseExitOptionIndex + 1) > 0) {
            ptr = arg0;
            do {
                i++;
                ptr->x[0] -= 0x20;
                ptr = (CharacterSelectCourseMenuFrameActor *)((s16 *)ptr + 1);
            } while (i < (gCharacterSelectCourseExitOptionIndex + 1));
        }
        break;
    }

    if ((arg0->state == 7) && (arg0->x[0] < -0x103)) {
        removeCallbackTask((CallbackTask *)arg0);
        return;
    }

    addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectCourseListOptions, arg0);
}
#endif

// initCharacterSelectUnlockedCourseList best match: 96.776% (nonmatchings/initCharacterSelectUnlockedCourseList-3836525038718587862/base_9.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_course_ui/initCharacterSelectUnlockedCourseList.s")

#ifdef NON_MATCHING
void initCharacterSelectUnlockedCourseList(CharacterSelectCourseMenuFrameActor *arg0) {
    u8 spacing;
    s16 baseY;
    s32 i;
    int mode;
    unsigned int limit;

    mode = gHighestUnlockedCourse;
    if (mode == 0) {
        if (1) {}
        spacing = 0x18;
        baseY = -0x50;
    } else if (mode == 1) {
        baseY = -0x54;
        spacing = 0x16;
    } else if (mode == 2) {
        spacing = 0x16;
        baseY = -0x60;
    } else {
        baseY = -0x60;
        spacing = 0x13;
    }

    arg0->baseY = baseY;
    arg0->itemSpacing = spacing;
    arg0->y[1] = baseY + spacing;
    arg0->y[2] = baseY + (spacing * 2);
    arg0->x[0] = -0x104;
    arg0->x[1] = -0x104;
    arg0->x[2] = -0x104;
    arg0->x[2] = -0x104;
    limit = 0xB;
    arg0->y[0] = baseY;

    i = 3;
    do {
        arg0->x[i + 1] = -0x104;
        arg0->x[i + 2] = -0x104;
        arg0->x[i + 3] = -0x104;
        arg0->x[i] = -0x104;
        arg0->y[i] = baseY + (i * spacing);
        arg0->y[i + 1] = baseY + ((i + 1) * spacing);
        arg0->y[i + 2] = baseY + ((i + 2) * spacing);
        arg0->y[i + 3] = baseY + ((i + 3) * spacing);
        i += 4;
    } while (i != limit);

    arg0->timer = 0;
    arg0->itemCount = 1;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectUnlockedCourseList);
}
#endif

// drawCharacterSelectLimitedCourseListOptions best match: 99.556%
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_course_ui/drawCharacterSelectLimitedCourseListOptions.s")

#ifdef NON_MATCHING
void drawCharacterSelectLimitedCourseListOptions(CharacterSelectCourseMenuFrameActor *arg0) {
    register CharacterSelectCourseMenuFrameActor *actor;
    s32 arrowBase;
    s32 lastArrowIndex;
    s32 i;
    s32 alpha;
    u16 tile;

    actor = arg0;
    if (gRaceTypeSelection < 2) {
        arrowBase = 2;
    } else {
        arrowBase = 0;
    }
    i = 0;
    lastArrowIndex = arrowBase + 1;
    if (actor->itemCount > 0) {
        do {
            alpha = 0;
            if (i == 0) {
                if (gRaceTypeSelection == 2) {
                    goto use_alt_tile;
                }
                tile = 0x1C;
                goto tile_selected;
use_alt_tile:
                tile = 0x1D;
tile_selected:
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
                drawMenuSprite((s16)(actor->x[i] - 0x10), actor->y[i], getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), (i + 0x29) & 0xFFFF, 0x20, 0x20, 0, 0);
            }
            i++;
        } while (i < actor->itemCount);
    }
}
#endif

// updateCharacterSelectLimitedCourseList best match: 84.118% (nonmatchings/updateCharacterSelectLimitedCourseList-7273315160691878794/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_course_ui/updateCharacterSelectLimitedCourseList.s")

#ifdef NON_MATCHING
void updateCharacterSelectLimitedCourseList(CharacterSelectCourseMenuFrameActor *arg0) {
    s32 movedCount;
    s32 visibleCount;
    volatile s32 state6Count;
    s32 i;
    s32 remainder;
    s32 selected;
    s32 modeOffset;
    s32 targetY;
    u8 nextItemCount;
    CharacterSelectCourseMenuFrameActor *actor;
    CharacterSelectCourseMenuFrameActor *actorCopy;

    actorCopy = arg0;

    if (gRaceTypeSelection < 2) {
        modeOffset = 2;
    } else {
        modeOffset = 0;
    }

    switch (arg0->state) {
    case 0:
        movedCount = 0;
        i = 0;
        if (arg0->itemCount > 0) {
            actor = arg0;
            do {
                if (actor->x[0] < -0x7C) {
                    actor->x[0] += 0x10;
                    movedCount++;
                    if (actor->x[0] >= -0x7C) {
                        actor->x[0] = -0x7C;
                    }
                }
                i++;
                actor = (CharacterSelectCourseMenuFrameActor *)((s16 *)actor + 1);
            } while (i < actorCopy->itemCount);
        }

        arg0->timer++;
        if ((arg0->timer & 1) == 0) {
            visibleCount = modeOffset + 2;
            if (arg0->itemCount < visibleCount) {
                nextItemCount = arg0->itemCount + 1;
                arg0->itemCount = nextItemCount;
                if (visibleCount == nextItemCount) {
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

        if (movedCount == 0) {
            arg0->state = 1;
        }
        break;

    case 1:
        if (gMenuTransitionState == 7) {
            arg0->state = 7;
        }
        if (gMenuTransitionState == 1) {
            arg0->state = 2;
        }
        break;

    case 2:
        i = 0;
        if (arg0->itemCount > 0) {
            do {
                if (i != gRaceCourseIndex) {
                    arg0->x[i] -= 0x20;
                }
                i++;
            } while (i < actorCopy->itemCount);
        }

        if (gRaceCourseIndex != 0) {
            if (arg0->x[0] < -0x103) {
                arg0->state = 3;
            }
        } else if (arg0->x[1] < -0x103) {
            arg0->state = 3;
        }
        break;

    case 3:
        arg0->y[gRaceCourseIndex] -= arg0->itemSpacing;
        if (arg0->y[gRaceCourseIndex] < -0x57) {
            arg0->y[gRaceCourseIndex] = -0x58;
            arg0->state = 4;
            createCallbackTask(initCharacterSelectCourseRecordsPopup, 0, 0x62);
            D_8010ADE0 = createCallbackTask(initCharacterSelectCourseExitPopup, 0, 0x63);
        }
        break;

    case 4:
        if (gMenuTransitionState == 4) {
            arg0->state = 5;
        }
        if (gMenuTransitionState == 7) {
            arg0->state = 10;
        }
        break;

    case 5:
        arg0->y[gRaceCourseIndex] += arg0->itemSpacing;
        targetY = arg0->baseY + (gRaceCourseIndex * arg0->itemSpacing);
        if (arg0->y[gRaceCourseIndex] >= targetY) {
            arg0->y[gRaceCourseIndex] = targetY;
            arg0->state = 6;
        }
        break;

    case 6:
        state6Count = modeOffset + 2;
        i = 0;
        if (state6Count > 0) {
            remainder = state6Count & 3;
            if (remainder != 0) {
                do {
                    if (i != gRaceCourseIndex) {
                        arg0->x[i] += 0x20;
                    }
                    i++;
                } while (remainder != i);
                if (i == state6Count) {
                    goto state_6_done;
                }
            }
            do {
                selected = gRaceCourseIndex;
                if (i != selected) {
                    arg0->x[i] += 0x20;
                    selected = gRaceCourseIndex;
                }
                if (i + 1 != selected) {
                    arg0->x[i + 1] += 0x20;
                    selected = gRaceCourseIndex;
                }
                if (i + 2 != selected) {
                    arg0->x[i + 2] += 0x20;
                    selected = gRaceCourseIndex;
                }
                if (i + 3 != selected) {
                    arg0->x[i + 3] += 0x20;
                }
                i += 4;
            } while (i != state6Count);
        }
state_6_done:
        if (gRaceCourseIndex != 0) {
            if (arg0->x[0] >= -0x7C) {
                arg0->state = 1;
            }
        } else if (arg0->x[1] >= -0x7C) {
            arg0->state = 1;
        }
        if (arg0->state == 1) {
            gMenuTransitionState = 5;
        }
        break;

    case 7:
        i = 0;
        actor = arg0;
        if (arg0->itemCount > 0) {
            do {
                i++;
                actor->x[0] -= 0x20;
                actor = (CharacterSelectCourseMenuFrameActor *)((s16 *)actor + 1);
            } while (i < arg0->itemCount);
        }
        if (arg0->x[0] < -0x103) {
            arg0->state = 8;
        }
        break;

    case 8:
        arg0->state = 9;
        break;

    case 10:
        arg0->x[gRaceCourseIndex] -= 0x20;
        if (arg0->x[gRaceCourseIndex] < -0x103) {
            arg0->state = 8;
        }
        break;
    }

    if (arg0->state == 9) {
        removeCallbackTask((CallbackTask *)arg0);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, drawCharacterSelectLimitedCourseListOptions, arg0);
    }
}
#endif

// initCharacterSelectLimitedCourseList best match: 60.000% (nonmatchings/initCharacterSelectLimitedCourseList-5802343343535905907/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_course_ui/initCharacterSelectLimitedCourseList.s")

#ifdef NON_MATCHING
void initCharacterSelectLimitedCourseList(CharacterSelectCourseMenuFrameActor *arg0) {
    volatile CharacterSelectCourseMenuFrameActor *actor = arg0;
    s32 spacing0;
    s32 spacing1;
    s32 spacing2;
    s32 base0;
    s32 base1;
    s32 base2;
    s32 base3;
    s32 x;
    s32 one;

    actor->baseY = -0x58;
    actor->itemSpacing = 0x18;
    base0 = actor->baseY;
    base1 = actor->baseY;
    spacing0 = actor->itemSpacing;
    spacing1 = actor->itemSpacing;
    spacing2 = actor->itemSpacing;
    actor->y[0] = base0;
    base2 = actor->baseY;
    base3 = actor->baseY;
    x = -0x104;
    one = 1;
    actor->y[3] = base3 + ((spacing2 << 2) - spacing2);
    actor->y[2] = base2 + (spacing1 << 1);
    actor->y[1] = base1 + spacing0;
    actor->x[1] = x;
    actor->x[2] = x;
    actor->x[3] = x;
    actor->x[0] = x;
    actor->timer = 0;
    actor->itemCount = one;
    actor->state = 0;
    setCallbackTaskCallback(arg0, updateCharacterSelectLimitedCourseList);
}
#endif

// drawCharacterSelectCoursePreviewFrame best match: 98.863% (nonmatchings/drawCharacterSelectCoursePreviewFrame-3836525038718587862/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_course_ui/drawCharacterSelectCoursePreviewFrame.s")

#ifdef NON_MATCHING
void drawCharacterSelectCoursePreviewFrame(CharacterSelectCourseWidgetActor *arg0) {
    volatile s32 padTop[2];
    volatile u16 frameIndex;
    volatile s32 padAfter[1];
    CharacterSelectCourseFrameTileMap *savedFrame;
    volatile s32 padBefore[2];
    s32 yOffset;
    register CharacterSelectCourseFrameTileMap *tileMap;
    register s32 i;
    register s32 offset;
    register CharacterSelectCourseWidgetActor *actor;

    actor = arg0;
    if ((gRaceSplitscreenMode == 1) && (gRaceTypeSelection == 2)) {
        do {
            frameIndex = 9;
        } while (0);
    } else {
 frameIndex = actor->sprite.unsignedIndex; } savedFrame = (tileMap = &gCharacterSelectCoursePreviewFrameTileMaps[frameIndex]); i = (offset = 0); if (arg0 == 0) { } do { drawMenuSpriteTile((s16) (actor->x + ((i & 3) << 5)), (s16) (actor->y + ((i / 4) << 5)), getRelocatableHeapBlockBase(gAssetHandles.textureHandle), tileMap->center[0], 0, 0x100); i += 1; tileMap = (CharacterSelectCourseFrameTileMap *) (((u16 *) tileMap) + 1); } while (i < 0x10); tileMap = savedFrame; yOffset = offset; do { drawMenuSpriteTile((s16) (actor->x + 0x80), (s16) (actor->y + yOffset), getRelocatableHeapBlockBase(gAssetHandles.textureHandle), tileMap->right[0], 0, 0x100); drawMenuSpriteTile((s16) (actor->x + offset), (s16) (actor->y + 0x80), getRelocatableHeapBlockBase(gAssetHandles.textureHandle), tileMap->bottom[0], 0, 0x100); offset += 0x40; tileMap = (CharacterSelectCourseFrameTileMap *) (((u16 *) tileMap) + 1); } while (offset < 0x80); drawMenuSpriteTile((s16) (actor->x + 0x80), (s16) (actor->y + 0x80), getRelocatableHeapBlockBase(gAssetHandles.textureHandle), gCharacterSelectCoursePreviewFrameCornerTileMaps[frameIndex].corner, 0, 0x100);

    drawMenuSprite((s16)(actor->x - 4), (s16)(actor->y - 4), getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), 0x33,
                  0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(actor->x - 4), (s16)(actor->y + 0x8C), getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                  0x38, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(actor->x + 0x8C), (s16)(actor->y - 4), getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                  0x35, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(actor->x + 0x8C), (s16)(actor->y + 0x8C), getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                  0x3A, 0x20, 0x20, 0, 0);

    offset = 0;
    do {
        drawMenuSprite((s16)(actor->x + offset + 0xC), (s16)(actor->y - 4),
                      getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), 0x34, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)(actor->x + offset + 0xC), (s16)(actor->y + 0x8C),
                      getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), 0x39, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)(actor->x - 4), (s16)(actor->y + offset + 0xC),
                      getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), 0x36, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)(actor->x + 0x8C), (s16)(actor->y + offset + 0xC),
                      getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), 0x37, 0x20, 0x20, 0, 0);
        offset += 0x10;
    } while (offset != 0x80);
}
#endif

// updateCharacterSelectCoursePreviewFrame best match: 91.995%
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_course_ui/updateCharacterSelectCoursePreviewFrame.s")

#ifdef NON_MATCHING
void updateCharacterSelectCoursePreviewFrame(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    int stateValue;
    u16 selectedSpriteIndex;
    s32 selectedIndex;
    u16 spriteIndex;

    selectedIndex = gRaceCourseIndex;
    spriteIndex = (u16)arg0->sprite.index;
    selectedSpriteIndex = spriteIndex;
    if ((selectedIndex >= (s32)selectedSpriteIndex) && (arg0->transition.bytes.state != 0) && (arg0->y != -0x48)) {
        stateValue = 2;
        state = stateValue & 0xFF;
        arg0->transition.bytes.state = stateValue;
    } else {
        state = arg0->transition.bytes.state;
        if ((selectedIndex < (s32)selectedSpriteIndex) && (state != 0) && (arg0->y != -0x140)) {
            stateValue = 1;
            state = stateValue & 0xFF;
            arg0->transition.bytes.state = stateValue;
        } else if ((state != 0) && (state < 4)) {
            stateValue = 3;
            state = stateValue & 0xFF;
            arg0->transition.bytes.state = stateValue;
        }
    }

    if (state != D_8010AE8A) {
        state = D_8010AE8A & 0xFF;
        arg0->transition.bytes.state = D_8010AE8A;
    }

    switch (state) {
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
        state = D_80121D80[8];
        if (state == 1) {
            arg0->transition.bytes.state = 4;
            state = gMenuTransitionState;
        }
        if (state == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        state = D_80121D80[8];
        if (state == 5) {
            arg0->transition.bytes.state = 3;
            D_80121D80[8] = 6;
            state = 6 & 0xFF;
        }
        if (state == 7) {
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
        D_80121D80[8] = 8;
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
#endif

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
            getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle),
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
    drawMenuSpriteWithAlpha(arg0->x, arg0->y, getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), 1, 0x20, 0x20, 0, arg0->sprite.index, 0);
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
    do { characterIds = gCharacterSelectCourseOptionsByUnlock[gHighestUnlockedCourse]; if (gPlayerCount == 1) { drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x21, 0x20, 0x20, 0, 0); drawMenuSprite((s16)(arg0->x + 0x30), arg0->y, getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x22, 0x20, 0x20, 0, 0); sprintf(buf - 0x10, gCharacterSelectCourseBestScoreFormat, D_800EC9F8[characterIds[*(&gRaceCourseIndex)]]); drawMenuAsciiText((s16)(arg0->x + 0x14), (s16)(arg0->y + 0x2A), buf - 0x10, 0, 0x100); valueOffset = 0; yOffset = 0; three = 3; do { sprintf(buf - 0x10, gCharacterSelectCourseMedalScoreFormat, *((u16 *)(&gCharacterSelectCourseMedalScoreThresholds[(((*(&gRaceCourseIndex)) * three) * 2) + valueOffset]))); drawMenuAsciiText((s16)(arg0->x + 0x28), (s16)((arg0->y + yOffset) + 9), buf - 0x10, 0, 0x100); yOffset += 8; valueOffset += 2; } while (yOffset != 0x18); } else { drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x26, 0x20, 0x20, 0, 0); drawMenuSprite(arg0->x, (s16)(arg0->y + 0x10), getRelocatableHeapBlockBase(gAssetHandles.textureHandle), 0x27, 0x20, 0x20, 0, 0); player = (CharacterSelectCoursePlayerRecord *)D_80121D80; yOffset = 0; do { sprintf(buf - 0x10, gCharacterSelectCoursePlayerRankFormat, player->value); drawMenuAsciiText((s16)(arg0->x + 0x40), (s16)((arg0->y + yOffset) + 0x10), buf - 0x10, 0, 0x100); player++; yOffset += 8; } while (player != &gFrameCounter); } } while (0);
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
        if (D_80121D80[8] == 3) {
            D_80121D80[8] = 4;
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
    drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), 3, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16) (arg0->x + 0x40), arg0->y, getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), 4, 0x20, 0x20, 0, 0);
    drawMenuSprite(arg0->x, (s16) (arg0->y + 0x40), getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), 5, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16) (arg0->x + 0x40), (s16) (arg0->y + 0x40), getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), 6, 0x20,
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
    drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), 3, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16) (arg0->x + 0x40), arg0->y, getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), 4, 0x20, 0x20, 0, 0);
    drawMenuSprite(arg0->x, (s16) (arg0->y + 0x40), getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), 5, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16) (arg0->x + 0x40), (s16) (arg0->y + 0x40), getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), 6, 0x20, 0x20, 0, 0);
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
            getRelocatableHeapBlockBase(D_80112178),
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
        drawMenuSpriteWithAlpha(arg0->x, arg0->sprite.index, getRelocatableHeapBlockBase(D_80112178), 0x18, 0x20, 0x20, 0, drawAlpha, 0);

        if (drawAlpha == 0x100) {
            alpha = 0x60;
        } else {
            alpha = 0x100;
        }

        drawMenuSpriteWithAlpha(arg0->x, arg0->y, getRelocatableHeapBlockBase(D_80112178), 0x17, 0x20, 0x20, 0, alpha, 0);

        state = gMenuChoicePromptState;
        if ((state == 3) || (state == 4)) {
            drawMenuSpriteWithAlpha(arg0->x, (s16)(((gMenuChoicePromptState * 0x10) + arg0->y) - 0x30), getRelocatableHeapBlockBase(D_80112178), 0x12,
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

// drawCharacterSelectCourseRecordsPopup best match: 90.478% (nonmatchings/drawCharacterSelectCourseRecordsPopup-2225551288923588688/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_course_ui/drawCharacterSelectCourseRecordsPopup.s")

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
    /* 0x0000 */ u8 pad0[0x7756];
    /* 0x7756 */ u16 trickValues[10][5];
} CharacterSelectStatsData;

extern u8 gGameSaveDataBuffer[];
extern u8 D_800F41EB[];
extern u8 D_800F4222[];
extern u8 D_800F4259[];
extern u8 D_800F4290[];

void drawCharacterSelectCourseRecordsPopup(CharacterSelectCourseWidgetActor *arg0) {
    volatile u8 padding[0x40];
    s16 *courseIds;
    u16 sp72;
    u16 sp70;
    volatile u8 color;
    u8 *trickIcon;
    s16 *selectedCoursePtr;
    s32 quotient;
    s32 var_s1;
    s32 var_s4;
    s32 var_s4_2;
    s32 var_s5;
    s32 var_s7;
    u16 var_s0;

    if (gRaceSplitscreenMode == 2) {
        courseIds = gCharacterSelectCourseOptionsByUnlock[gHighestUnlockedCourse];
    } else if ((s32)gRaceTypeSelection < 2) {
        courseIds = gCharacterSelectShortCourseOptions;
    } else {
        courseIds = gCharacterSelectSingleCourseOption;
    }

    var_s7 = 0;
    var_s5 = 0;
    do {
        if (var_s7 < 3) {
            color = 6;
        } else {
            color = 4;
        }

        drawMenuSprite(arg0->x, (s16)(arg0->y + var_s5), getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                      (var_s7 + 0x77) & 0xFFFF, 0x20, 0x20, 0, 0);

        if (gPlayerCount == 1) {
            if (gRaceSplitscreenMode != 2) {
                if (gRaceTypeSelection == 0) {
                    if (var_s7 < 3) {
                        color = 0xC;
                    } else {
                        color = 0xD;
                    }
                    drawCharacterSelectCourseRecordTime((CharacterSelectCourseRecordTime *)((CharacterSelectResultNameData *)gGameSaveDataBuffer)->resultNames[courseIds[gRaceCourseIndex]][var_s7],
                                  arg0->x + 0x14, arg0->y + var_s5, color);
                    drawMenuSprite((s16)(arg0->x + 0x54), (s16)(arg0->y + var_s5),
                                  getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                                  ((D_800F4290[(courseIds[gRaceCourseIndex] * 5) + var_s7] & 7) + 0x51) & 0xFFFF, 0x20, 0x20, 0, 0);
                    drawMenuSprite((s16)(arg0->x + 0x65), (s16)(arg0->y + var_s5),
                                  getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                                  ((D_800F4290[(courseIds[gRaceCourseIndex] * 5) + var_s7] >> 3) + 0x7C) & 0xFFFF, 0x20, 0x20, 0, 0);
                } else if (gRaceTypeSelection == 1) {
                    if (var_s7 < 3) {
                        var_s4 = 0xC;
                    } else {
                        var_s4 = 0xD;
                    }
                    selectedCoursePtr = &courseIds[gRaceCourseIndex];
                    quotient = D_800F4222[((*selectedCoursePtr * 5) + var_s7)] / 10;
                    sp70 = quotient;
                    if (sp70 != 0) {
                        drawMenuSprite((s16)(arg0->x + 0x18), (s16)(arg0->y + var_s5),
                                      getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), (sp70 + 0x2B) & 0xFFFF, 0x20, 0x20, 0,
                                      var_s4 + 1);
                        selectedCoursePtr = &courseIds[gRaceCourseIndex];
                    }
                    sp72 = D_800F4222[((*selectedCoursePtr * 5) + var_s7)] % 10;
                    drawMenuSprite((s16)(arg0->x + 0x20), (s16)(arg0->y + var_s5), getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                                  (sp72 + 0x2B) & 0xFFFF, 0x20, 0x20, 0, (var_s4 + 1) & 0xFF);
                    drawMenuAsciiText((s16)(arg0->x + 0x28), (s16)(arg0->y + var_s5 + 8), "HIT", color, 0x100);
                    drawMenuSprite((s16)(arg0->x + 0x48), (s16)(arg0->y + var_s5),
                                  getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                                  ((D_800F4259[(courseIds[gRaceCourseIndex] * 5) + var_s7] & 7) + 0x51) & 0xFFFF, 0x20, 0x20, 0, 0);
                    drawMenuSprite((s16)(arg0->x + 0x60), (s16)(arg0->y + var_s5),
                                  getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                                  ((D_800F4259[(courseIds[gRaceCourseIndex] * 5) + var_s7] >> 3) + 0x7C) & 0xFFFF, 0x20, 0x20, 0, 0);
                } else {
                    trickIcon = &gGameSaveDataBuffer[var_s7];
                    if (var_s7 < 3) {
                        var_s4_2 = 0xC;
                    } else {
                        var_s4_2 = 0xD;
                    }
                    var_s0 = ((CharacterSelectStatsData *)gGameSaveDataBuffer)->trickValues[0][var_s7];
                    var_s1 = 0;
                    do {
                        drawMenuSprite((s16)((arg0->x - var_s1) + 0x38), (s16)(arg0->y + var_s5),
                                      getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), ((var_s0 % 10) + 0x2B) & 0xFFFF, 0x20, 0x20, 0,
                                      (var_s4_2 + 1) & 0xFF);
                        var_s1 += 8;
                        var_s0 = var_s0 / 10;
                    } while (var_s0 != 0);
                    drawMenuAsciiText((s16)(arg0->x + 0x40), (s16)(arg0->y + var_s5 + 8), "P", color, 0x100);
                    drawMenuSprite((s16)(arg0->x + 0x4C), (s16)(arg0->y + var_s5), getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                                  ((trickIcon[0x77E7] & 7) + 0x51) & 0xFFFF, 0x20, 0x20, 0, 0);
                    drawMenuSprite((s16)(arg0->x + 0x60), (s16)(arg0->y + var_s5), getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                                  ((trickIcon[0x77E7] >> 3) + 0x7C) & 0xFFFF, 0x20, 0x20, 0, 0);
                }
            } else {
                if (var_s7 < 3) {
                    color = 0xC;
                } else {
                    color = 0xD;
                }
                drawCharacterSelectCourseRecordTime((CharacterSelectCourseRecordTime *)((CharacterSelectCourseStatsNameData *)gGameSaveDataBuffer)->courseStatsNames[courseIds[gRaceCourseIndex]][var_s7],
                              arg0->x + 0x14, arg0->y + var_s5, color);
                drawMenuSprite((s16)(arg0->x + 0x54), (s16)(arg0->y + var_s5), getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                              ((D_800F41EB[(courseIds[gRaceCourseIndex] * 5) + var_s7] & 7) + 0x51) & 0xFFFF, 0x20, 0x20, 0, 0);
                drawMenuSprite((s16)(arg0->x + 0x65), (s16)(arg0->y + var_s5), getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                              ((D_800F41EB[(courseIds[gRaceCourseIndex] * 5) + var_s7] >> 3) + 0x7C) & 0xFFFF, 0x20, 0x20, 0, 0);
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
    drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), 0x28, 0x20, 0x20, 0, 0);
    if ((s32) arg0->transition.bytes.state > 0) {
        drawMenuGlyphScript((s16) (arg0->x + 4), (s16) (arg0->y + 4), gCharacterSelectCourseStartGameText, 1, 0x100, 0);
        drawMenuSprite(
            (s16) (arg0->x + 0x64),
            (s16) (arg0->y + 4),
            getRelocatableHeapBlockBase(D_80112178),
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

// drawCharacterSelectCourseRecordTime best match: 93.301% (nonmatchings/drawCharacterSelectCourseRecordTime-6061209858023118177/base_10.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_course_ui/drawCharacterSelectCourseRecordTime.s")

#ifdef NON_MATCHING
extern char D_800E0B98[];
extern char D_800E0BA0[];
extern char D_800E0BA8[];

void drawCharacterSelectCourseRecordTime(CharacterSelectCourseRecordTime *arg0, s32 x, s32 y, s32 alpha) {
    char buffer[0x60];
    char *ptr;
    char *end;

    sprintf(buffer, D_800E0B98, arg0->minutes);
    y = (s16)y;
    alpha &= 0xFFFF;
    ptr = buffer;
    end = buffer + 2;
    do {
        drawAssetTableSpriteWithExplicitPalette((s16)x, y, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), ((u8)*ptr - 5) & 0xFFFF,
                      alpha);
        ptr++;
        x += 8;
    } while (ptr < end);
    drawAssetTableSpriteWithExplicitPalette((s16)x, y, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x36, alpha);

    if ((gRaceSplitscreenMode == 2) || ((gRaceSplitscreenMode == 1) && (gRaceTypeSelection == 0))) {
        x += 6;
    } else {
        x += 8;
    }

    sprintf(buffer, D_800E0BA0, arg0->seconds);
    ptr = buffer;
    end = buffer + 2;
    do {
        drawAssetTableSpriteWithExplicitPalette((s16)x, y, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), ((u8)*ptr - 5) & 0xFFFF,
                      alpha);
        ptr++;
        x += 8;
    } while (ptr < end);
    drawAssetTableSpriteWithExplicitPalette((s16)x, y, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x35, alpha);

    x += 8;
    sprintf(buffer, D_800E0BA8, arg0->centiseconds >> 8);
    ptr = buffer;
    end = buffer + 2;
    do {
        drawAssetTableSpriteWithExplicitPalette((s16)x, y, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), ((u8)*ptr - 5) & 0xFFFF,
                      alpha);
        ptr++;
        x += 8;
    } while (ptr != end);
}
#endif
