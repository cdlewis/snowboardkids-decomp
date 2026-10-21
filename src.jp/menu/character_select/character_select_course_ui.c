#include "game/engine/system_render.h"
#include "common.h"
#include "game/menu/menu_scratch.h"
#include "game/race/race_state.h"
#include "font_encoding.h"
#include <PR/os_libc.h>
#include "game/save_data.h"
#include "game/engine/asset_manager.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/character_select/character_select_course_menu.h"
#include "game/menu/character_select/character_select_course_ui.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/splitscreen_select/race_splitscreen_select_ui.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/race/player/race_player_input.h"

#define CHARACTER_SELECT_ICON_TEXTURE_HANDLE (gAssetHandles[0x24])
#define gCharacterSelectCourseExitPreviewCornerTile (gCharacterSelectCourseExitPreviewData.cornerTile)
#define CHARACTER_SELECT_POPUP_FONT_HANDLE (gAssetHandles[0x1F])
#define CHARACTER_SELECT_FRAME_TEXTURE_HANDLE (gAssetHandles[0x21])
#define ASSET_HANDLE(index) (gAssetHandles[index])

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

typedef struct {
    u16 corner;
    u8 pad2[0x28];
} CharacterSelectCourseFrameCornerTileMap;

typedef struct {
    u16 cornerTile;
    u16 padding;
    MenuGlyphScript exitText[14];
} CharacterSelectCourseExitPreviewData;

typedef struct {
    u16 center[16];
    u16 right[2];
    u16 bottom[2];
    u16 corner;
} CharacterSelectCourseFrameTileMap;

typedef MenuGlyphScript CharacterSelectCourseText[0x4A];

typedef struct {
    u8 pad0[8];
    u8 next;
    u8 pad9[7];
    s16 xNext;
    s16 pad12;
    s16 pad14;
    s16 pad16;
    s16 pad18;
    s16 x0;
    s16 x1;
    s16 x2;
    u8 pad20[6];
    s16 y0;
    s16 y1;
    s16 y2;
    s16 y3;
} CharacterSelectCourseListInitLoop;

extern u8 gCharacterSelectCourseDifficultyRatings[12];

extern const char gCharacterSelectCourseStatsScoreFormat[];

extern u16 gCharacterSelectCourseStatsScoreValues[10];

extern char gCharacterSelectCourseRecordHitsLabel[];

extern char gCharacterSelectCourseRecordPointsLabel[];

extern CharacterSelectCourseFrameCornerTileMap gCharacterSelectCoursePreviewFrameCornerTileMaps[];

extern CharacterSelectCourseExitPreviewData gCharacterSelectCourseExitPreviewData;

extern CharacterSelectCourseFrameTileMap gCharacterSelectCoursePreviewFrameTileMaps[];

extern const char D_800DFEC8[];

extern const char D_800DFED0[];

extern const char D_800DFED8[];

extern s32 gMenuFlowState;

extern CharacterSelectCourseText gCharacterSelectCourseNameText[10];

extern u8 gCourseSelectFromRaceTypeMenu;

extern u8 gHighestUnlockedCourse;
extern MenuGlyphScript gCharacterSelectCourseStartGameText[44];

extern MenuGlyphScript D_800B6088[3][0x30];
extern MenuGlyphScript D_800B61A8[3][0x30];
extern MenuGlyphScript D_800B62C8[2][0x38];
extern MenuGlyphScript D_800B63A8[];

extern u16 D_800B5A7C[];
extern const char D_800DFEB4[];
extern const char D_800DFEB8[];
extern const char D_800DFEBC[];

void drawCharacterSelectCourseListOptions(CharacterSelectCourseMenuFrameActor *arg0) {
    register CharacterSelectCourseMenuFrameActor *actor;
    s16 *characterIds;
    s32 i;
    s32 alpha;

    actor = arg0;
    if ((gRaceSplitscreenMode == 0) || (gRaceSplitscreenMode == 2)) {
        characterIds = gCharacterSelectCourseMenuData.courseOptionsByUnlock[gHighestUnlockedCourse];
    } else if (gRaceTypeSelection >= 3) {
        characterIds = gCharacterSelectCourseMenuData.shortCourseOptions;
    } else {
        characterIds = gCharacterSelectCourseMenuData.singleCourseOption;
    }

    i = 0;

    while (i < actor->itemCount) {
        alpha = 0;
        if (i == 0) {
            if ((i == gRaceCourseIndex.signedValue) && (gMenuSelectionConfirmTimer > 0) &&
                (gMenuSelectionConfirmTimer < 8) && (gMenuSelectionConfirmTimer & 1)) {
                alpha = 0xFF;
            }
            drawMenuSprite(
                actor->x[i],
                actor->y[i],
                getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                0x1C,
                0x20,
                0x20,
                0,
                alpha
            );
        } else if (i == gCharacterSelectCourseExitOptionIndex) {
            if ((gRaceCourseIndex.signedValue == gCharacterSelectCourseExitOptionIndex) &&
                (gMenuSelectionConfirmTimer > 0) && (gMenuSelectionConfirmTimer < 8) &&
                (gMenuSelectionConfirmTimer & 1)) {
                alpha = 0xFF;
            }
            if ((gPlayerCount - 1) == 0) {
                drawMenuSprite(
                    actor->x[i],
                    actor->y[i],
                    getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                    0x1B,
                    0x20,
                    0x20,
                    0,
                    alpha
                );
            } else {
                drawMenuSprite(
                    actor->x[i],
                    actor->y[i],
                    getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                    0x1E,
                    0x20,
                    0x20,
                    0,
                    alpha
                );
            }
        } else {
            if ((i == gRaceCourseIndex.signedValue) && (gMenuSelectionConfirmTimer > 0) &&
                (gMenuSelectionConfirmTimer < 8) && (gMenuSelectionConfirmTimer & 1)) {
                alpha = 0xFF;
            }
            drawMenuSprite(
                actor->x[i],
                actor->y[i],
                getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                i + 0x12,
                0x20,
                0x20,
                0,
                alpha
            );
        }

        if (i != gCharacterSelectCourseExitOptionIndex) {
            if (((gPlayerCount - 1) == 0) && (gGameSaveDataBuffer[0].cupPlacements[characterIds[i]] != 0)) {
                drawMenuSprite(
                    (s16)(actor->x[i] - 0x10),
                    actor->y[i],
                    getRelocatableHeapBlockBase(CHARACTER_SELECT_ICON_TEXTURE_HANDLE),
                    i + 0x1A,
                    0x20,
                    0x20,
                    0,
                    gGameSaveDataBuffer[0].cupPlacements[characterIds[i]] + 6
                );
            } else {
                drawMenuSprite(
                    (s16)(actor->x[i] - 0x10),
                    actor->y[i],
                    getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                    i + 0x29,
                    0x20,
                    0x20,
                    0,
                    0
                );
            }
        }
        i++;
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
                        gMenuScratch1.task =
                            createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewFrame, 0, 0x58);
                        createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewPanel1, 0, 0x59);
                        createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewPanel2, 0, 0x5A);
                        createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewPanel3, 0, 0x5B);
                        createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewPanel4, 0, 0x5C);
                        createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewPanel5, 0, 0x5D);
                        if (gHighestUnlockedCourse != 0) {
                            createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewPanel6, 0, 0x5E);
                            if (gHighestUnlockedCourse >= 2) {
                                createCallbackTask(
                                    (CallbackTaskCallback)initCharacterSelectCoursePreviewPanel7,
                                    0,
                                    0x5F
                                );
                                if (gHighestUnlockedCourse >= 3) {
                                    createCallbackTask(
                                        (CallbackTaskCallback)initCharacterSelectCoursePreviewPanel8,
                                        0,
                                        0x60
                                    );
                                }
                            }
                        }
                        createCallbackTask((CallbackTaskCallback)initCharacterSelectCourseExitPreviewPanel, 0, 0x61);
                    }
                }
            }
            if (movingCourseCount == 0) {
                arg0->state = COURSE_LIST_IDLE;
            }
            break;

        case COURSE_LIST_IDLE:
            if (gRacePlayers[0].menuState == 1) {
                arg0->state = COURSE_LIST_SLIDE_OTHERS_OUT;
            }
            if (gRacePlayers[0].menuState == 7) {
                arg0->state = COURSE_LIST_EXIT;
            }
            break;

        case COURSE_LIST_SLIDE_OTHERS_OUT:
            for (index = 0; index < 11; index++) {
                if (index != gRaceCourseIndex.signedValue) {
                    arg0->x[index] -= 0x20;
                }
                // clang-format off
        }

        if (gRaceCourseIndex.signedValue != RACE_COURSE_BIG_SNOWMAN) {
            if (arg0->x[0] < -0x103) {
                arg0->state = COURSE_LIST_MOVE_SELECTED_UP;
            }
        } else if (arg0->x[1] < -0x103) {
            arg0->state = COURSE_LIST_MOVE_SELECTED_UP;
        }
        break;

    case COURSE_LIST_MOVE_SELECTED_UP:
        arg0->y[gRaceCourseIndex.signedValue] -= arg0->itemSpacing;
        if (arg0->y[gRaceCourseIndex.signedValue] < -0x5F) {
            arg0->y[gRaceCourseIndex.signedValue] = -0x60;
            arg0->state = COURSE_LIST_SUBMENU_OPEN;
            if (gRaceSplitscreenMode == 2) {
                gMenuScratch0.task = createCallbackTask((CallbackTaskCallback)initCharacterSelectCourseExitPopup, 0, 0x62);
                createCallbackTask((CallbackTaskCallback)initCharacterSelectCourseRecordsPopup, 0, 0x63);
            } else {
                createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePlayerStatsPanel, 0, 0x61);
                createCallbackTask((CallbackTaskCallback)initCharacterSelectCourseSubmenuFrame, 0, 0x62);
            }
        }
        break;

    case COURSE_LIST_SUBMENU_OPEN:
        if (gRacePlayers[0].menuState == 4) {
            arg0->state = COURSE_LIST_RESTORE_SELECTED_Y;
        }
        if (gRacePlayers[0].menuState == 7) {
            arg0->state = COURSE_LIST_EXIT;
        }
        break;

    case COURSE_LIST_RESTORE_SELECTED_Y:
        arg0->y[gRaceCourseIndex.signedValue] += arg0->itemSpacing;
        if (arg0->y[gRaceCourseIndex.signedValue] >= (arg0->baseY + (gRaceCourseIndex.signedValue * arg0->itemSpacing))) {
            arg0->y[gRaceCourseIndex.signedValue] = arg0->baseY + (gRaceCourseIndex.signedValue * arg0->itemSpacing);
            arg0->state = COURSE_LIST_SLIDE_OTHERS_IN;
        }
        break;

    case COURSE_LIST_SLIDE_OTHERS_IN:
        visibleCourseCount = gCharacterSelectCourseExitOptionIndex + 1; index = 0;  while (index < (gCharacterSelectCourseExitOptionIndex + 1)) {
            if (index != gRaceCourseIndex.signedValue) {
                arg0->x[index] += 0x20;
                visibleCourseCount = gCharacterSelectCourseExitOptionIndex + 1;
            }
            index++;
        }

        if (gRaceCourseIndex.signedValue != RACE_COURSE_BIG_SNOWMAN) {
            if (arg0->x[0] >= -0x7C) {
                arg0->state = COURSE_LIST_IDLE;
            }
        } else if (arg0->x[1] >= -0x7C) {
            arg0->state = COURSE_LIST_IDLE;
        }
        if (arg0->state == COURSE_LIST_IDLE) {
            gRacePlayers[0].menuState = 5;
        }
        break;

    case COURSE_LIST_EXIT:
        for (index = 0; index < (gCharacterSelectCourseExitOptionIndex + 1); index++) {
            arg0->x[index] -= 0x20;
        }
        break;
            // clang-format on
    }

    if ((arg0->state == COURSE_LIST_EXIT) && (arg0->x[0] < -0x103)) {
        removeCallbackTask(arg0);
        return;
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCourseListOptions, arg0);
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
    // clang-format off
    do { \
        arg0->y[1] = baseY + spacing; \
        arg0->y[2] = baseY + (spacing * 2); \
        arg0->x[0] = -0x104; \
        arg0->x[1] = -0x104; \
        arg0->x[2] = -0x104; \
        arg0->x[2] = -0x104; \
        arg0->y[0] = baseY; \
        i = 3; \
        loop = (CharacterSelectCourseListInitLoop *)&((u8 *)&arg0->taskHeader)[6]; \
        limit = 0xB; \
        do { \
            loop->x0 = -0x104; \
            loop->x1 = -0x104; \
            loop->x2 = -0x104; \
            loop = (CharacterSelectCourseListInitLoop *)&loop->next; \
            loop->xNext = -0x104; \
            loop->y0 = baseY + (i * spacing); \
            loop->y1 = baseY + ((i + 1) * spacing); \
            loop->y2 = baseY + ((i + 2) * spacing); \
            loop->y3 = baseY + ((i + 3) * spacing); \
            i += 4; \
        } while (i != limit); \
    } while (0);
    // clang-format on

    arg0->timer = 0;
    arg0->itemCount = 1;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectUnlockedCourseList);
}

void drawCharacterSelectLimitedCourseListOptions(CharacterSelectCourseMenuFrameActor *arg0) {
    register CharacterSelectCourseMenuFrameActor *actor;
    void *arrowTexture;
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
                if ((i == gRaceCourseIndex.signedValue) && (gMenuSelectionConfirmTimer > 0) &&
                    (gMenuSelectionConfirmTimer < 8) && (gMenuSelectionConfirmTimer & 1)) {
                    alpha = 0xFF;
                }
                drawMenuSprite(
                    actor->x[i],
                    actor->y[i],
                    getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                    tile,
                    0x20,
                    0x20,
                    0,
                    alpha
                );
            } else if (i == lastArrowIndex) {
                if ((i == gRaceCourseIndex.signedValue) && (gMenuSelectionConfirmTimer > 0) &&
                    (gMenuSelectionConfirmTimer < 8) && (gMenuSelectionConfirmTimer & 1)) {
                    alpha = 0xFF;
                }
                drawMenuSprite(
                    actor->x[i],
                    actor->y[i],
                    getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                    0x20,
                    0x20,
                    0x20,
                    0,
                    alpha
                );
            } else {
                if ((i == gRaceCourseIndex.signedValue) && (gMenuSelectionConfirmTimer > 0) &&
                    (gMenuSelectionConfirmTimer < 8) && (gMenuSelectionConfirmTimer & 1)) {
                    alpha = 0xFF;
                }
                tile = i + 0x12;
                drawMenuSprite(
                    actor->x[i],
                    actor->y[i],
                    getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                    tile,
                    0x20,
                    0x20,
                    0,
                    alpha
                );
            }

            if (i != lastArrowIndex) {
                arrowTexture = getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE);
                tile = i + 0x29;
                drawMenuSprite((s16)(actor->x[i] - 0x10), actor->y[i], arrowTexture, tile, 0x20, 0x20, 0, 0);
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
                        gMenuScratch1.task =
                            createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewFrame, 0, 0x59);
                        if (gRaceTypeSelection < 2) {
                            createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewPanel1, 0, 0x5A);
                            createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewPanel2, 0, 0x5B);
                        }
                        createCallbackTask((CallbackTaskCallback)initCharacterSelectCourseExitPreviewPanel, 0, 0x5C);
                        createCallbackTask((CallbackTaskCallback)initCharacterSelectCourseRecordsFrame, 0, 0x62);
                    }
                }
            }

            if (movingOptionCount == 0) {
                arg0->state = COURSE_LIST_IDLE;
            }
            break;

        case COURSE_LIST_IDLE:
            if (gRacePlayers[0].menuState == 7) {
                arg0->state = COURSE_LIST_EXIT;
            }
            if (gRacePlayers[0].menuState == 1) {
                arg0->state = COURSE_LIST_SLIDE_OTHERS_OUT;
            }
            break;

        case COURSE_LIST_SLIDE_OTHERS_OUT:
            for (optionIndex = 0; optionIndex < actor->itemCount; optionIndex++) {
                if (optionIndex != gRaceCourseIndex.signedValue) {
                    arg0->x[optionIndex] -= 0x20;
                }
            }

            if (gRaceCourseIndex.signedValue != RACE_COURSE_BIG_SNOWMAN) {
                if (arg0->x[0] < -0x103) {
                    arg0->state = COURSE_LIST_MOVE_SELECTED_UP;
                }
            } else if (arg0->x[1] < -0x103) {
                arg0->state = COURSE_LIST_MOVE_SELECTED_UP;
            }
            break;

        case COURSE_LIST_MOVE_SELECTED_UP:
            arg0->y[gRaceCourseIndex.signedValue] -= arg0->itemSpacing;
            if (arg0->y[gRaceCourseIndex.signedValue] < -0x57) {
                arg0->y[gRaceCourseIndex.signedValue] = -0x58;
                arg0->state = COURSE_LIST_SUBMENU_OPEN;
                createCallbackTask((CallbackTaskCallback)initCharacterSelectCourseRecordsPopup, 0, 0x62);
                gMenuScratch0.task =
                    createCallbackTask((CallbackTaskCallback)initCharacterSelectCourseExitPopup, 0, 0x63);
            }
            break;

        case COURSE_LIST_SUBMENU_OPEN:
            if (gRacePlayers[0].menuState == 4) {
                arg0->state = COURSE_LIST_RESTORE_SELECTED_Y;
            }
            if (gRacePlayers[0].menuState == 7) {
                arg0->state = COURSE_LIST_SLIDE_SELECTED_OUT;
            }
            break;

        case COURSE_LIST_RESTORE_SELECTED_Y:
            arg0->y[gRaceCourseIndex.signedValue] += arg0->itemSpacing;
            if (arg0->y[gRaceCourseIndex.signedValue] >=
                (arg0->baseY + (gRaceCourseIndex.signedValue * arg0->itemSpacing))) {
                arg0->y[gRaceCourseIndex.signedValue] =
                    arg0->baseY + (gRaceCourseIndex.signedValue * arg0->itemSpacing);
                arg0->state = COURSE_LIST_SLIDE_OTHERS_IN;
            }
            break;

        case COURSE_LIST_SLIDE_OTHERS_IN:
            visibleOptionCount = extraCourseOptionCount + 2;
            optionIndex = 0;

            while (optionIndex < (extraCourseOptionCount + 2)) {
                if (optionIndex != gRaceCourseIndex.signedValue) {
                    arg0->x[optionIndex] += 0x20;
                }
                optionIndex++;
            }

            if (gRaceCourseIndex.signedValue != RACE_COURSE_BIG_SNOWMAN) {
                if (arg0->x[0] >= -0x7C) {
                    arg0->state = COURSE_LIST_IDLE;
                }
            } else if (arg0->x[1] >= -0x7C) {
                arg0->state = COURSE_LIST_IDLE;
            }
            if (arg0->state == COURSE_LIST_IDLE) {
                gRacePlayers[0].menuState = 5;
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
            arg0->x[gRaceCourseIndex.signedValue] -= 0x20;
            if (arg0->x[gRaceCourseIndex.signedValue] < -0x103) {
                arg0->state = COURSE_LIST_FINISH_EXIT;
            }
            break;
    }

    if (arg0->state == COURSE_LIST_REMOVE) {
        removeCallbackTask((CallbackTask *)arg0);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectLimitedCourseListOptions, arg0);
    }
}

void initCharacterSelectLimitedCourseList(CharacterSelectCourseMenuFrameActor *arg0) {
    s32 i;

    arg0->baseY = -0x58;
    arg0->itemSpacing = 0x18;
    for (i = 0; i < 4; i++) {
        arg0->x[i] = -0x104;
        arg0->y[i] = arg0->baseY + (i * arg0->itemSpacing);
    }
    arg0->timer = 0;
    arg0->itemCount = 1;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectLimitedCourseList);
}

void drawCharacterSelectCoursePreviewFrame(CharacterSelectCourseWidgetActor *arg0) {
    union {
        s32 index;
        void *textureBase;
    } i;
    s32 borderOffset;
    u16 frameIndex;

    if ((gRaceSplitscreenMode == 1) && (gRaceTypeSelection == 2)) {
        if (i.index) {}
        frameIndex = 9;
    } else {
        frameIndex = arg0->sprite.unsignedIndex;
    }

    for (i.index = 0; i.index < 16; i.index++) {
        drawMenuSpriteTile(
            (s16)(arg0->x + ((i.index & 3) << 5)),
            (s16)(arg0->y + ((i.index / 4) << 5)),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[frameIndex].center[i.index],
            0,
            0x100
        );
    }

    for (i.index = 0; i.index < 2; i.index++) {
        drawMenuSpriteTile(
            (s16)(arg0->x + 0x80),
            (s16)(arg0->y + (i.index * 0x40)),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[frameIndex].right[i.index],
            0,
            0x100
        );
        drawMenuSpriteTile(
            (s16)(arg0->x + (i.index * 0x40)),
            (s16)(arg0->y + 0x80),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[frameIndex].bottom[i.index],
            0,
            0x100
        );
    }

    drawMenuSpriteTile(
        (s16)(arg0->x + 0x80),
        (s16)(arg0->y + 0x80),
        getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
        gCharacterSelectCoursePreviewFrameCornerTileMaps[frameIndex].corner,
        0,
        0x100
    );
    drawMenuSprite(
        (s16)(arg0->x - 4),
        (s16)(arg0->y - 4),
        getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
        0x33,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x - 4),
        (s16)(arg0->y + 0x8C),
        getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
        0x38,
        0x20,
        0x20,
        0,
        0
    );
    i.textureBase = getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE);
    drawMenuSprite((s16)(arg0->x + 0x8C), (s16)(arg0->y - 4), i.textureBase, 0x35, 0x20, 0x20, 0, 0);
    drawMenuSprite(
        (s16)(arg0->x + 0x8C),
        (s16)(arg0->y + 0x8C),
        getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
        0x3A,
        0x20,
        0x20,
        0,
        0
    );

    for (borderOffset = 0; borderOffset != 0x80; borderOffset += 0x10) {
        drawMenuSprite(
            (s16)((arg0->x + borderOffset) + 0xC),
            (s16)(arg0->y - 4),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            0x34,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            (s16)((arg0->x + borderOffset) + 0xC),
            (s16)(arg0->y + 0x8C),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            0x39,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            (s16)(arg0->x - 4),
            (s16)((arg0->y + borderOffset) + 0xC),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            0x36,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            (s16)(arg0->x + 0x8C),
            (s16)((arg0->y + borderOffset) + 0xC),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            0x37,
            0x20,
            0x20,
            0,
            0
        );
    }
}

// clang-format off
void updateCharacterSelectCoursePreviewFrame(CharacterSelectCourseWidgetActor *arg0) {
    u8 state;
    u8 *new_var;
    int switchState;
    u32 selectedSpriteIndex;
    int menuState;
    register s32 selectedIndex;
    register u16 spriteIndex;

    selectedIndex = gRaceCourseIndex.signedValue;
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

    if (switchState != gCharacterSelectCourseCursorState.previewFrameState) {
        switchState = state = arg0->transition.bytes.state =
            gCharacterSelectCourseCursorState.previewFrameState;
    }

    switch (switchState) {
        case 0:
            arg0->x -= 0x26;
            if (arg0->x < -7) {
                arg0->x = -8;
                arg0->transition.bytes.state = 3;
                gActiveMenuTask =
                    createCallbackTask((CallbackTaskCallback)initCharacterSelectCourseListCursor, 0, 0x64);
                createCallbackTask((CallbackTaskCallback)initCharacterSelectCourseTitleCursor, 0, 0x62);
                createCallbackTask((CallbackTaskCallback)initCharacterSelectCourseStatsBadge, 0, 0x62);
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
            menuState = gRacePlayers[0].menuState;
            new_var = &gRacePlayers[0].menuState;
            if (menuState == 1) {
                arg0->transition.bytes.state = 4;
                menuState = *new_var;
            }
            if (menuState == 7) {
                arg0->transition.bytes.state = 5;
            }
            state = arg0->transition.bytes.state;
            break;
        case 4:
            if (gRacePlayers[0].menuState == 5) {
                arg0->transition.bytes.state = 3;
                gRacePlayers[0].menuState = 6;
            }
            if (gRacePlayers[0].menuState == 7) {
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
            gRacePlayers[0].menuState = 8;
            state = arg0->transition.bytes.state;
            break;
    }

    gCharacterSelectCourseCursorState.previewFrameState = state;
    if (arg0->transition.bytes.state == 7) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCoursePreviewFrame, arg0);
}
// clang-format on

void initCharacterSelectCoursePreviewFrame(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = 0x96;
    arg0->y = -0x48;
    arg0->sprite.index = 0;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewFrame);
}

// clang-format off
void drawCharacterSelectCoursePreviewPanel1(CharacterSelectCourseWidgetActor *arg0) {
    s32 i;
    for (i = 0; i < 16; i++) {
        drawMenuSpriteTileClipped(
            arg0->x + ((i & 3) << 5),
            arg0->y + ((i / 4) << 5),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
    }
    for (i = 0; i < 2; i++) {
        drawMenuSpriteTileClipped(
            arg0->x + 0x80,
            arg0->y + i * 0x40,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
        drawMenuSpriteTileClipped(
            arg0->x + i * 0x40,
            arg0->y + 0x80,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
    }
    drawMenuSpriteTileClipped(
        arg0->x + 0x80,
        arg0->y + 0x80,
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
        gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner,
        0,
        0x100,
        gMenuViewportWidth / 2,
        gMenuViewportHeight / 2 - 0x2F
    );
}
// clang-format on

// clang-format off
void updateCharacterSelectCoursePreviewPanel1(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((gRaceCourseIndex.signedValue >= (s32)(u16)arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((gRaceCourseIndex.signedValue < (s32)(u16)arg0->sprite.index) && (arg0->y != -0x140)) {
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
            if (gRacePlayers[0].menuState == 1) {
                arg0->transition.bytes.state = 4;
            }
            if (gRacePlayers[0].menuState == 7) {
                arg0->transition.bytes.state = 5;
            }
            state = arg0->transition.bytes.state;
            break;
        case 4:
            if (gRacePlayers[0].menuState == 5) {
                arg0->transition.bytes.state = 3;
            }
            if (gRacePlayers[0].menuState == 7) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCoursePreviewPanel1, arg0);
}
// clang-format on

void initCharacterSelectCoursePreviewPanel1(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x1;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewPanel1);
}

// clang-format off
void drawCharacterSelectCoursePreviewPanel2(CharacterSelectCourseWidgetActor *arg0) {
    s32 i;
    for (i = 0; i < 16; i++) {
        drawMenuSpriteTileClipped(
            arg0->x + ((i & 3) * 32),
            arg0->y + ((i / 4) * 32),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
    }
    for (i = 0; i < 2; i++) {
        drawMenuSpriteTileClipped(
            arg0->x + 0x80,
            arg0->y + i * 0x40,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
        drawMenuSpriteTileClipped(
            arg0->x + i * 0x40,
            arg0->y + 0x80,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
    }
    drawMenuSpriteTileClipped(
        arg0->x + 0x80,
        arg0->y + 0x80,
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
        gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner,
        0,
        0x100,
        gMenuViewportWidth / 2,
        gMenuViewportHeight / 2 - 0x2F
    );
}
// clang-format on

// clang-format off
void updateCharacterSelectCoursePreviewPanel2(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((gRaceCourseIndex.signedValue >= (s32)(u16)arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((gRaceCourseIndex.signedValue < (s32)(u16)arg0->sprite.index) && (arg0->y != -0x140)) {
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
            if (gRacePlayers[0].menuState == 1) {
                arg0->transition.bytes.state = 4;
            }
            if (gRacePlayers[0].menuState == 7) {
                arg0->transition.bytes.state = 5;
            }
            state = arg0->transition.bytes.state;
            break;
        case 4:
            if (gRacePlayers[0].menuState == 5) {
                arg0->transition.bytes.state = 3;
            }
            if (gRacePlayers[0].menuState == 7) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCoursePreviewPanel2, arg0);
}
// clang-format on

void initCharacterSelectCoursePreviewPanel2(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x2;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewPanel2);
}

void drawCharacterSelectCoursePreviewPanel3(CharacterSelectCourseWidgetActor *arg0) {
    s32 i;
    s32 tileOffset;

    tileOffset = 0;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(
            arg0->x + ((i & 3) << 5),
            arg0->y + ((i / 4) << 5),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[tileOffset],
            0,
            0x100,
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
        );
    }

    tileOffset = 0;
    for (i = 0; i < 2; i++) {
        drawMenuSpriteTileClipped(
            arg0->x + 0x80,
            arg0->y + i * 0x40,
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[tileOffset],
            0,
            0x100,
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
        );
        drawMenuSpriteTileClipped(
            arg0->x + i * 0x40,
            arg0->y + 0x80,
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileOffset],
            0,
            0x100,
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
        );
        tileOffset++;
    }

    drawMenuSpriteTileClipped(
        arg0->x + 0x80,
        arg0->y + 0x80,
        getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
        gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner,
        0,
        0x100,
        gMenuViewportWidth / 2,
        (gMenuViewportHeight / 2) - 0x2F
    );
}

// clang-format off
void updateCharacterSelectCoursePreviewPanel3(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((gRaceCourseIndex.signedValue >= (s32)(u16)arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((gRaceCourseIndex.signedValue < (s32)(u16)arg0->sprite.index) && (arg0->y != -0x140)) {
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
            if (gRacePlayers[0].menuState == 1) {
                arg0->transition.bytes.state = 4;
            }
            if (gRacePlayers[0].menuState == 7) {
                arg0->transition.bytes.state = 5;
            }
            state = arg0->transition.bytes.state;
            break;
        case 4:
            if (gRacePlayers[0].menuState == 5) {
                arg0->transition.bytes.state = 3;
            }
            if (gRacePlayers[0].menuState == 7) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCoursePreviewPanel3, arg0);
}
// clang-format on

void initCharacterSelectCoursePreviewPanel3(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x3;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewPanel3);
}

// clang-format off
void drawCharacterSelectCoursePreviewPanel4(CharacterSelectCourseWidgetActor *arg0) {
    s32 i;
    for (i = 0; i < 16; i++) {
        drawMenuSpriteTileClipped(
            arg0->x + ((i & 3) * 32),
            arg0->y + ((i / 4) * 32),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
    }
    for (i = 0; i < 2; i++) {
        drawMenuSpriteTileClipped(
            arg0->x + 0x80,
            arg0->y + i * 0x40,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
        drawMenuSpriteTileClipped(
            arg0->x + i * 0x40,
            arg0->y + 0x80,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
    }
    drawMenuSpriteTileClipped(
        arg0->x + 0x80,
        arg0->y + 0x80,
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
        gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner,
        0,
        0x100,
        gMenuViewportWidth / 2,
        gMenuViewportHeight / 2 - 0x2F
    );
}
// clang-format on

// clang-format off
void updateCharacterSelectCoursePreviewPanel4(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((gRaceCourseIndex.signedValue >= (s32)(u16)arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((gRaceCourseIndex.signedValue < (s32)(u16)arg0->sprite.index) && (arg0->y != -0x140)) {
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
            if (gRacePlayers[0].menuState == 1) {
                arg0->transition.bytes.state = 4;
            }
            if (gRacePlayers[0].menuState == 7) {
                arg0->transition.bytes.state = 5;
            }
            state = arg0->transition.bytes.state;
            break;
        case 4:
            if (gRacePlayers[0].menuState == 5) {
                arg0->transition.bytes.state = 3;
            }
            if (gRacePlayers[0].menuState == 7) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCoursePreviewPanel4, arg0);
}
// clang-format on

void initCharacterSelectCoursePreviewPanel4(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x4;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewPanel4);
}

// clang-format off
void drawCharacterSelectCoursePreviewPanel5(CharacterSelectCourseWidgetActor *arg0) {
    s32 i;
    for (i = 0; i < 16; i++) {
        drawMenuSpriteTileClipped(
            arg0->x + ((i & 3) * 32),
            arg0->y + ((i / 4) * 32),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
    }
    for (i = 0; i < 2; i++) {
        drawMenuSpriteTileClipped(
            arg0->x + 0x80,
            arg0->y + i * 0x40,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
        drawMenuSpriteTileClipped(
            arg0->x + i * 0x40,
            arg0->y + 0x80,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
    }
    drawMenuSpriteTileClipped(
        arg0->x + 0x80,
        arg0->y + 0x80,
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
        gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner,
        0,
        0x100,
        gMenuViewportWidth / 2,
        gMenuViewportHeight / 2 - 0x2F
    );
}
// clang-format on

// clang-format off
void updateCharacterSelectCoursePreviewPanel5(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((gRaceCourseIndex.signedValue >= (s32)(u16)arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((gRaceCourseIndex.signedValue < (s32)(u16)arg0->sprite.index) && (arg0->y != -0x140)) {
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
            if (gRacePlayers[0].menuState == 1) {
                arg0->transition.bytes.state = 4;
            }
            if (gRacePlayers[0].menuState == 7) {
                arg0->transition.bytes.state = 5;
            }
            state = arg0->transition.bytes.state;
            break;
        case 4:
            if (gRacePlayers[0].menuState == 5) {
                arg0->transition.bytes.state = 3;
            }
            if (gRacePlayers[0].menuState == 7) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCoursePreviewPanel5, arg0);
}
// clang-format on

void initCharacterSelectCoursePreviewPanel5(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x5;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewPanel5);
}

// clang-format off
void drawCharacterSelectCoursePreviewPanel6(CharacterSelectCourseWidgetActor *arg0) {
    s32 i;
    for (i = 0; i < 16; i++) {
        drawMenuSpriteTileClipped(
            arg0->x + ((i & 3) * 32),
            arg0->y + ((i / 4) * 32),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
    }
    for (i = 0; i < 2; i++) {
        drawMenuSpriteTileClipped(
            arg0->x + 0x80,
            arg0->y + i * 0x40,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
        drawMenuSpriteTileClipped(
            arg0->x + i * 0x40,
            arg0->y + 0x80,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
    }
    drawMenuSpriteTileClipped(
        arg0->x + 0x80,
        arg0->y + 0x80,
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
        gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner,
        0,
        0x100,
        gMenuViewportWidth / 2,
        gMenuViewportHeight / 2 - 0x2F
    );
}
// clang-format on

// clang-format off
void updateCharacterSelectCoursePreviewPanel6(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((gRaceCourseIndex.signedValue >= (s32)(u16)arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((gRaceCourseIndex.signedValue < (s32)(u16)arg0->sprite.index) && (arg0->y != -0x140)) {
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
            if (gRacePlayers[0].menuState == 1) {
                arg0->transition.bytes.state = 4;
            }
            if (gRacePlayers[0].menuState == 7) {
                arg0->transition.bytes.state = 5;
            }
            state = arg0->transition.bytes.state;
            break;
        case 4:
            if (gRacePlayers[0].menuState == 5) {
                arg0->transition.bytes.state = 3;
            }
            if (gRacePlayers[0].menuState == 7) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCoursePreviewPanel6, arg0);
}
// clang-format on

void initCharacterSelectCoursePreviewPanel6(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x6;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewPanel6);
}

// clang-format off
void drawCharacterSelectCoursePreviewPanel7(CharacterSelectCourseWidgetActor *arg0) {
    s32 i;
    for (i = 0; i < 16; i++) {
        drawMenuSpriteTileClipped(
            arg0->x + ((i & 3) * 32),
            arg0->y + ((i / 4) * 32),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
    }
    for (i = 0; i < 2; i++) {
        drawMenuSpriteTileClipped(
            arg0->x + 0x80,
            arg0->y + i * 0x40,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
        drawMenuSpriteTileClipped(
            arg0->x + i * 0x40,
            arg0->y + 0x80,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
    }
    drawMenuSpriteTileClipped(
        arg0->x + 0x80,
        arg0->y + 0x80,
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
        gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner,
        0,
        0x100,
        gMenuViewportWidth / 2,
        gMenuViewportHeight / 2 - 0x2F
    );
}
// clang-format on

// clang-format off
void updateCharacterSelectCoursePreviewPanel7(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((gRaceCourseIndex.signedValue >= (s32)(u16)arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((gRaceCourseIndex.signedValue < (s32)(u16)arg0->sprite.index) && (arg0->y != -0x140)) {
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
            if (gRacePlayers[0].menuState == 1) {
                arg0->transition.bytes.state = 4;
            }
            if (gRacePlayers[0].menuState == 7) {
                arg0->transition.bytes.state = 5;
            }
            state = arg0->transition.bytes.state;
            break;
        case 4:
            if (gRacePlayers[0].menuState == 5) {
                arg0->transition.bytes.state = 3;
            }
            if (gRacePlayers[0].menuState == 7) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCoursePreviewPanel7, arg0);
}
// clang-format on

void initCharacterSelectCoursePreviewPanel7(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x7;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewPanel7);
}

// clang-format off
void drawCharacterSelectCoursePreviewPanel8(CharacterSelectCourseWidgetActor *arg0) {
    s32 i;
    for (i = 0; i < 16; i++) {
        drawMenuSpriteTileClipped(
            arg0->x + ((i & 3) * 32),
            arg0->y + ((i / 4) * 32),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
    }
    for (i = 0; i < 2; i++) {
        drawMenuSpriteTileClipped(
            arg0->x + 0x80,
            arg0->y + i * 0x40,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
        drawMenuSpriteTileClipped(
            arg0->x + i * 0x40,
            arg0->y + 0x80,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
    }
    drawMenuSpriteTileClipped(
        arg0->x + 0x80,
        arg0->y + 0x80,
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
        gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner,
        0,
        0x100,
        gMenuViewportWidth / 2,
        gMenuViewportHeight / 2 - 0x2F
    );
}
// clang-format on

// clang-format off
void updateCharacterSelectCoursePreviewPanel8(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((gRaceCourseIndex.signedValue >= (s32)(u16)arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((gRaceCourseIndex.signedValue < (s32)(u16)arg0->sprite.index) && (arg0->y != -0x140)) {
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
            if (gRacePlayers[0].menuState == 1) {
                arg0->transition.bytes.state = 4;
            }
            if (gRacePlayers[0].menuState == 7) {
                arg0->transition.bytes.state = 5;
            }
            state = arg0->transition.bytes.state;
            break;
        case 4:
            if (gRacePlayers[0].menuState == 5) {
                arg0->transition.bytes.state = 3;
            }
            if (gRacePlayers[0].menuState == 7) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCoursePreviewPanel8, arg0);
}
// clang-format on

void initCharacterSelectCoursePreviewPanel8(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x8;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewPanel8);
}

// clang-format off
void drawCharacterSelectCoursePreviewPanel9(CharacterSelectCourseWidgetActor *arg0) {
    s32 i;
    for (i = 0; i < 16; i++) {
        drawMenuSpriteTileClipped(
            arg0->x + ((i & 3) * 32),
            arg0->y + ((i / 4) * 32),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
    }
    for (i = 0; i < 2; i++) {
        drawMenuSpriteTileClipped(
            arg0->x + 0x80,
            arg0->y + i * 0x40,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
        drawMenuSpriteTileClipped(
            arg0->x + i * 0x40,
            arg0->y + 0x80,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[i],
            0,
            0x100,
            gMenuViewportWidth / 2,
            gMenuViewportHeight / 2 - 0x2F
        );
    }
    drawMenuSpriteTileClipped(
        arg0->x + 0x80,
        arg0->y + 0x80,
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
        gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner,
        0,
        0x100,
        gMenuViewportWidth / 2,
        gMenuViewportHeight / 2 - 0x2F
    );
}
// clang-format on

// clang-format off
void updateCharacterSelectCoursePreviewPanel9(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((gRaceCourseIndex.signedValue >= (s32)(u16)arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((gRaceCourseIndex.signedValue < (s32)(u16)arg0->sprite.index) && (arg0->y != -0x140)) {
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
            if (gRacePlayers[0].menuState == 1) {
                arg0->transition.bytes.state = 4;
            }
            if (gRacePlayers[0].menuState == 7) {
                arg0->transition.bytes.state = 5;
            }
            state = arg0->transition.bytes.state;
            break;
        case 4:
            if (gRacePlayers[0].menuState == 5) {
                arg0->transition.bytes.state = 3;
            }
            if (gRacePlayers[0].menuState == 7) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCoursePreviewPanel9, arg0);
}
// clang-format on

void initCharacterSelectCoursePreviewPanel9(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x9;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewPanel9);
}

void drawCharacterSelectCourseExitPreviewPanel(CharacterSelectCourseWidgetActor *arg0) {
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(
            arg0->x + ((i & 3) << 5),
            arg0->y + ((i / 4) << 5),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[10].center[tileOffset],
            0,
            0x100,
            gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F
        );
    }

    // clang-format off
    tileOffset = 0; offset = 0; i = 0x80; for (;;) {
        drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + offset, getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                      gCharacterSelectCoursePreviewFrameTileMaps[10].right[tileOffset], 0, 0x100, gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F);
        drawMenuSpriteTileClipped(arg0->x + offset, arg0->y + 0x80, getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                      gCharacterSelectCoursePreviewFrameTileMaps[10].bottom[tileOffset], 0, 0x100, gMenuViewportWidth / 2,
            (gMenuViewportHeight / 2) - 0x2F);
        // clang-format on
        i = 0x80;
        offset += 0x40;
        tileOffset++;

        if (!(offset != i)) {
            break;
        }
    }

    i++;
    i--;

    drawMenuSpriteTileClipped(
        arg0->x + 0x80,
        arg0->y + 0x80,
        getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
        gCharacterSelectCourseExitPreviewCornerTile,
        0,
        0x100,
        gMenuViewportWidth / 2,
        (gMenuViewportHeight / 2) - 0x2F
    );
}

// clang-format off
void updateCharacterSelectCourseExitPreviewPanel(CharacterSelectCourseWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->row.bytes.subState;

    if ((gRaceCourseIndex.signedValue == gCharacterSelectCourseExitOptionIndex) && (arg0->y != -0x48) &&
        ((s32)arg0->row.bytes.subState < 6)) {
        state = arg0->row.bytes.subState = 2;
    } else {
        state = (s32)arg0->row.bytes.subState;
        if ((gRaceCourseIndex.signedValue != gCharacterSelectCourseExitOptionIndex) && (arg0->y != -0x140) &&
            (state < 6)) {
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
            if (gRacePlayers[0].menuState == 1) {
                arg0->row.bytes.subState = 4;
            }
            if (gRacePlayers[0].menuState == 7) {
                arg0->row.bytes.subState = 5;
            }
            state = arg0->row.bytes.subState;
            break;
        case 4:
            if (gRacePlayers[0].menuState == 5) {
                arg0->row.bytes.subState = 3;
            }
            if (gRacePlayers[0].menuState == 7) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCourseExitPreviewPanel, arg0);
}
// clang-format on

void initCharacterSelectCourseExitPreviewPanel(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0;
    arg0->row.bytes.subTimer = 0;
    arg0->row.bytes.subState = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCourseExitPreviewPanel);
}

void drawCharacterSelectCourseListCursor(CharacterSelectCourseWidgetActor *arg0) {
    if (arg0->transition.bytes.state < 3) {
        drawMenuSpriteWithAlpha(
            arg0->x,
            (s16)(arg0->y + gRaceCourseIndex.signedValue * arg0->selection.bytes.rowSpacing),
            getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)),
            0x12,
            0x20,
            0x20,
            0,
            arg0->sprite.index,
            0
        );
    }
}

// clang-format off
void updateCharacterSelectCourseListCursor(CharacterSelectCourseWidgetActor *arg0) {
    u8 state;
    s32 primaryPlayerIndex;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = gCharacterSelectCourseCursorState.listCursorState)) {
        arg0->transition.bytes.state = globalState;
        state = globalState;
        arg0->sprite.index = gCharacterSelectCourseCursorState.listCursorSpriteIndex;
        arg0->transition.bytes.timer = gCharacterSelectCourseCursorState.listCursorTimer;
    }

    /* Preserve IDO's state selector register allocation. */
    primaryPlayerIndex = 0;
    switch (state ^ primaryPlayerIndex) {
        case 0:
            arg0->sprite.index += 0x26;
            if (arg0->sprite.index >= 0x100) {
                arg0->sprite.index = 0x100;
                arg0->transition.bytes.state = 1;
            }
            state = arg0->transition.bytes.state;
            break;
        case 1:
            if ((s32)arg0->transition.bytes.timer < 0x10) {
                arg0->sprite.index -= 9;
            } else {
                arg0->sprite.index += 9;
            }
            state = arg0->transition.bytes.state;
            arg0->transition.bytes.timer = (arg0->transition.bytes.timer + 1) & 0x1F;
            break;
        case 2:
            if (gRacePlayers[primaryPlayerIndex].menuState == 1) {
                state = arg0->transition.bytes.state = 3;
            }
            break;
        case 3:
            break;
    }

    (&gCharacterSelectCourseCursorState.listCursorState)[primaryPlayerIndex] = state;
    if (gRacePlayers[primaryPlayerIndex].menuState == 7) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCourseListCursor, arg0);
}
// clang-format on

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
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCourseListCursor);
}

void drawCharacterSelectCourseTitleCursor(CharacterSelectCourseWidgetActor *arg0) {
    drawMenuSpriteWithAlpha(
        arg0->x,
        arg0->y,
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)),
        1,
        0x20,
        0x20,
        0,
        arg0->sprite.index,
        0
    );
}

// clang-format off
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
            if (gRacePlayers[0].menuState == 1) {
                arg0->transition.bytes.state = 2;
            }
            if (gRacePlayers[0].menuState == 7) {
                arg0->transition.bytes.state = 3;
            }
            state = arg0->transition.bytes.state;
            break;
        case 2:
            if (gRacePlayers[0].menuState == 5) {
                arg0->transition.bytes.state = 1;
            }
            if (gRacePlayers[0].menuState == 7) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCourseTitleCursor, arg0);
}
// clang-format on

void initCharacterSelectCourseTitleCursor(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x5C;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCourseTitleCursor);
}

void drawCharacterSelectCourseStatsBadge(CharacterSelectCourseWidgetActor *arg0) {
    s32 temp_v0;
    s32 xOffset;
    char buf[8];
    s32 i;
    s32 var_t8;

    if (gRaceCourseIndex.signedValue != gCharacterSelectCourseExitOptionIndex) {
        drawMenuPanelBackdrop(arg0->x, arg0->y, 0x3800, 0x5800);
        drawMenuSpriteWithAlpha(
            (s16)(arg0->x + 4),
            (s16)(arg0->y + 4),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            0x23,
            0x20,
            0x20,
            0,
            arg0->sprite.index,
            0
        );
        sprintf(
            buf,
            gCharacterSelectCourseStatsScoreFormat,
            gCharacterSelectCourseStatsScoreValues[gRaceCourseIndex.signedValue]
        );
        drawMenuAsciiText((s16)(arg0->x + 8), (s16)(arg0->y + 0xC), buf, 0, arg0->sprite.index);
        drawMenuSpriteWithAlpha(
            (s16)(arg0->x + 0x28),
            (s16)(arg0->y + 0xC),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            0x24,
            0x20,
            0x20,
            0,
            arg0->sprite.index,
            0
        );
        drawMenuSpriteWithAlpha(
            arg0->x,
            (s16)(arg0->y + 0x14),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            0x25,
            0x20,
            0x20,
            0,
            arg0->sprite.index,
            0
        );

        temp_v0 = gCharacterSelectCourseDifficultyRatings[gRaceCourseIndex.signedValue];
        xOffset = 0;
        i = 0;
        var_t8 = temp_v0 & 1;
        if ((temp_v0 / 2) > 0) {
            for (;;) {
                drawMenuSpriteWithAlpha(
                    (s16)(arg0->x + xOffset + 4),
                    (s16)(arg0->y + 0x1C),
                    getRelocatableHeapBlockBase(CHARACTER_SELECT_ICON_TEXTURE_HANDLE),
                    0x25,
                    0x20,
                    0x20,
                    0,
                    arg0->sprite.index,
                    0
                );
                i++;
                temp_v0 = gCharacterSelectCourseDifficultyRatings[gRaceCourseIndex.signedValue];
                xOffset += 0xC;

                if (!(i < (temp_v0 / 2))) {
                    break;
                }
            }
        }
        var_t8 = temp_v0 & 1;
        if (var_t8 != 0) {
            drawMenuSpriteWithAlpha(
                (s16)(arg0->x + xOffset + 4),
                (s16)(arg0->y + 0x1C),
                getRelocatableHeapBlockBase(CHARACTER_SELECT_ICON_TEXTURE_HANDLE),
                0x26,
                0x20,
                0x20,
                0,
                arg0->sprite.index,
                0
            );
        }
    }
}

// clang-format off
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
            if (gRacePlayers[0].menuState == 1) {
                arg0->transition.bytes.state = 2;
            }
            if (gRacePlayers[0].menuState == 7) {
                arg0->transition.bytes.state = 3;
            }
            state = arg0->transition.bytes.state;
            break;
        case 2:
            if (gRacePlayers[0].menuState == 5) {
                arg0->transition.bytes.state = 1;
            }
            if (gRacePlayers[0].menuState == 7) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCourseStatsBadge, arg0);
}
// clang-format on

void initCharacterSelectCourseStatsBadge(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = 0x48;
    arg0->y = 0x34;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCourseStatsBadge);
}

// clang-format off
// drawCharacterSelectCoursePlayerStatsPanel best match: 99.899%
#pragma GLOBAL_ASM("asm.jp/nonmatchings/menu/character_select/character_select_course_ui/drawCharacterSelectCoursePlayerStatsPanel.s")

#ifdef NON_MATCHING
void drawCharacterSelectCoursePlayerStatsPanel(CharacterSelectCourseWidgetActor *arg0) {
 s16 *characterIds;
 s32 three;
 s32 yOffset;
 char buf[8];
 s32 valueOffset;
 characterIds=gCharacterSelectCourseMenuData.courseOptionsByUnlock[gHighestUnlockedCourse];
 if (gPlayerCount==1) {
 drawMenuSprite(arg0->x,arg0->y,getRelocatableHeapBlockBase(gAssetHandles[0x21]),0x21,0x20,0x20,0,0);
 drawMenuSprite(arg0->x+0x30,arg0->y,getRelocatableHeapBlockBase(gAssetHandles[0x21]),0x22,0x20,0x20,0,0);
 sprintf(buf,D_800DFEB4,(&gGameSaveDataBuffer[0].highScores[1])[characterIds[gRaceCourseIndex.signedValue]]);
 drawMenuAsciiText(arg0->x+0x14,arg0->y+0x2A,(u8*)buf,0,0x100);
 valueOffset=0;  yOffset=0;  three=3;  while(yOffset != 0x18) {  sprintf(buf,D_800DFEB8,D_800B5A7C[gRaceCourseIndex.signedValue * three + valueOffset]);  drawMenuAsciiText(arg0->x+0x28,arg0->y+yOffset+9,(u8*)buf,0,0x100);  yOffset += 8;  valueOffset++;  }  } else {
 drawMenuSprite(arg0->x,arg0->y,getRelocatableHeapBlockBase(gAssetHandles[0x21]),0x26,0x20,0x20,0,0);
 drawMenuSprite(arg0->x,arg0->y+0x10,getRelocatableHeapBlockBase(gAssetHandles[0x21]),0x27,0x20,0x20,0,0);
 for(valueOffset=0,yOffset=0;valueOffset<4;valueOffset++,yOffset+=8) {
 sprintf(buf,D_800DFEBC,gRacePlayers[valueOffset].unk18);
 drawMenuAsciiText(arg0->x+0x40,arg0->y+yOffset+0x10,(u8*)buf,0,0x100);
 }
 }
}
#endif
// clang-format on

// clang-format off
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
            if ((gRacePlayers[0].menuState == 3) || (gRacePlayers[0].menuState == 7)) {
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
        if (gRacePlayers[0].menuState == 3) {
            gRacePlayers[0].menuState = 4;
        }
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCoursePlayerStatsPanel, arg0);
    }
}
// clang-format on

void initCharacterSelectCoursePlayerStatsPanel(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = -0x50;
    arg0->sprite.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePlayerStatsPanel);
}

void drawCharacterSelectCourseSubmenuFrame(CharacterSelectCourseWidgetActor *arg0) {
    drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)), 3, 0x20, 0x20, 0, 0);
    drawMenuSprite(
        (s16)(arg0->x + 0x40),
        arg0->y,
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)),
        4,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        arg0->x,
        (s16)(arg0->y + 0x40),
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)),
        5,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0x40),
        (s16)(arg0->y + 0x40),
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)),
        6,
        0x20,
        0x20,
        0,
        0
    );
}

// clang-format off
void updateCharacterSelectCourseSubmenuFrame(CharacterSelectCourseWidgetActor *arg0) {
    u8 state = arg0->sprite.bytes.state;

    switch (state) {
        case 0:
            arg0->x += 0x20;
            if (arg0->x >= -0x88) {
                arg0->x = -0x88;
                arg0->sprite.bytes.state = 1;
                gMenuScratch0.task = createCallbackTask((CallbackTaskCallback)initCharacterSelectCourseNamePopup, 0, 0x63);
            }
            state = arg0->sprite.bytes.state;
            break;
        case 1:
            if ((gRacePlayers[0].menuState == 3) || (gRacePlayers[0].menuState == 7)) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCourseSubmenuFrame, arg0);
}
// clang-format on

void initCharacterSelectCourseSubmenuFrame(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = -0x18;
    arg0->sprite.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCourseSubmenuFrame);
}

void drawCharacterSelectCourseRecordsFrame(CharacterSelectCourseWidgetActor *arg0) {
    drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)), 3, 0x20, 0x20, 0, 0);
    drawMenuSprite(
        (s16)(arg0->x + 0x40),
        arg0->y,
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)),
        4,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        arg0->x,
        (s16)(arg0->y + 0x40),
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)),
        5,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0x40),
        (s16)(arg0->y + 0x40),
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)),
        6,
        0x20,
        0x20,
        0,
        0
    );
}

// clang-format off
void updateCharacterSelectCourseRecordsFrame(CharacterSelectCourseWidgetActor *arg0) {
    u32 state = arg0->sprite.bytes.state;

    switch (state) {
        case 0:
            arg0->x += 0x20;
            if (arg0->x >= -0x88) {
                arg0->x = -0x88;
                arg0->sprite.bytes.state = 1;
                createCallbackTask((CallbackTaskCallback)initCharacterSelectCourseDescriptionPopup, 0, 0x63);
            }
            state = arg0->sprite.bytes.state;
            break;
        case 1:
            if (gRacePlayers[0].menuState == 7) {
                arg0->sprite.bytes.state = 5;
            }
            if (gRacePlayers[0].menuState == 1) {
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
            if (gRacePlayers[0].menuState == 5) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCourseRecordsFrame, arg0);
}
// clang-format on

void initCharacterSelectCourseRecordsFrame(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x128;
    arg0->y = 0x8;
    arg0->sprite.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCourseRecordsFrame);
}

void drawCharacterSelectCourseNamePopup(CharacterSelectCourseWidgetActor *arg0) {
    CharacterSelectCourseText *texts;
    MenuGlyphScript *text;

    texts = gCharacterSelectCourseNameText;
    text = texts[gRaceCourseIndex.signedValue];
    drawMenuGlyphScript(arg0->x, arg0->y, text, 1, arg0->sprite.index, 0);
    if (arg0->transition.bytes.state == 2) {
        drawMenuSprite(
            (s16)(arg0->x + 0x60),
            (s16)(arg0->y + 0x40),
            getRelocatableHeapBlockBase(ASSET_HANDLE(0x24)),
            ((s32)arg0->transition.bytes.timer >= 8) + 5,
            0x20,
            0x20,
            0,
            0
        );
    }
}

// clang-format off
void updateCharacterSelectCourseNamePopup(CharacterSelectCourseWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = gCharacterSelectCourseCursorState.submenuState)) {
        arg0->transition.bytes.state = globalState;
        state = globalState;
        arg0->transition.bytes.timer = gCharacterSelectCourseCursorState.submenuTimer;
    }

    if (state) {
        while (0) {}
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
            if (gRacePlayers[0].menuState == 3) {
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
            if ((gRacePlayers[0].menuState == 3) || (gRacePlayers[0].menuState == 7)) {
                arg0->transition.bytes.state = 5;
            }
            if (globalState) {}
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

    (arg0 ? &gCharacterSelectCourseCursorState : &gCharacterSelectCourseCursorState)->submenuState = state;
    (arg0 ? &gCharacterSelectCourseCursorState : &gCharacterSelectCourseCursorState)->submenuTimer =
        arg0->transition.bytes.timer;

    if (arg0->transition.bytes.state == 6) {
        removeCallbackTask(arg0);
        gMenuScratch0.task = NULL;
        gCharacterSelectCourseCursorState.submenuState = 0;
        gCharacterSelectCourseCursorState.submenuTimer = 0;
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCourseNamePopup, arg0);
}
// clang-format on

void initCharacterSelectCourseNamePopup(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = -0x14;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCourseNamePopup);
}

// clang-format off
void drawCharacterSelectCourseDescriptionPopup(CharacterSelectCourseWidgetActor *arg0) {
    MenuGlyphScript *text;
    s32 threshold;
    u16 idx;

    if (gRaceTypeSelection < 2) {
        threshold = 2;
    } else {
        threshold = 0;
    }
    if (threshold >= gRaceCourseIndex.signedValue) {
        idx = (u16)gRaceCourseIndex.signedValue;
        if (gRaceTypeSelection == 0) {
            text = D_800B6088[idx];
        } else if (gRaceTypeSelection == 1) {
            text = D_800B61A8[idx];
        } else {
            text = D_800B62C8[idx];
        }
    } else {
        text = D_800B63A8;
    }
    drawMenuGlyphScript(arg0->x, arg0->y, text, 1, arg0->sprite.index, 0);
}
// clang-format on

// clang-format off
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
            if ((gRacePlayers[0].menuState == 7) || (gRacePlayers[0].menuState == 1)) {
                state = arg0->transition.bytes.state = 3;
            }
            break;
        case 3:
            arg0->x -= 0x20;
            if (arg0->x < -0x11F) {
                if (gRacePlayers[0].menuState == 7) {
                    arg0->transition.bytes.state = 7;
                } else {
                    arg0->transition.bytes.state = 4;
                }
            }
            state = arg0->transition.bytes.state;
            break;
        case 4:
            if (gRacePlayers[0].menuState == 5) {
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
            if (gRacePlayers[0].menuState == 7) {
                arg0->transition.bytes.state = 3;
            }
            if (gRacePlayers[0].menuState == 0) {
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCourseDescriptionPopup, arg0);
}
// clang-format on

void initCharacterSelectCourseDescriptionPopup(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = 0xC;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCourseDescriptionPopup);
}

// clang-format off
void drawCharacterSelectCourseConfirmCursor(CharacterSelectCourseWidgetActor *arg0) {
    u32 drawAlpha;
    int isEvenState;
    u16 alpha;
    s16 state;

    state = gMenuChoicePromptState[0];
    if (state != 0) {
        isEvenState = !(state & 1);
        if (isEvenState) {
            alpha = 0x100;
        } else {
            alpha = 0x60;
        }

        drawAlpha = alpha;
        drawMenuSpriteWithAlpha(
            arg0->x,
            arg0->sprite.index,
            getRelocatableHeapBlockBase(ASSET_HANDLE(0x24)),
            0x18,
            0x20,
            0x20,
            0,
            drawAlpha,
            0
        );

        if (drawAlpha == 0x100) {
            alpha = 0x60;
        } else {
            alpha = 0x100;
        }

        drawMenuSpriteWithAlpha(
            arg0->x,
            arg0->y,
            getRelocatableHeapBlockBase(ASSET_HANDLE(0x24)),
            0x17,
            0x20,
            0x20,
            0,
            alpha,
            0
        );

        state = gMenuChoicePromptState[0];
        if ((state == 3) || (state == 4)) {
            drawMenuSpriteWithAlpha(
                arg0->x,
                (s16)(((gMenuChoicePromptState[0] * 0x10) + arg0->y) - 0x30),
                getRelocatableHeapBlockBase(ASSET_HANDLE(0x24)),
                0x12,
                0x20,
                0x20,
                0,
                arg0->selection.unsignedCounter,
                0
            );
            state = gMenuChoicePromptState[0];
        }
    }
    if ((state >= 5) && (((s16)arg0->transition.alpha) == 0)) {
        removeCallbackTask(arg0);
    }
}
// clang-format on

// clang-format off
void updateCharacterSelectCourseConfirmCursor(CharacterSelectCourseWidgetActor *arg0) {
    s32 step;

    if ((gMenuChoicePromptState[0] != 0) && (gMenuChoicePromptState[0] != 3) && (gMenuChoicePromptState[0] != 4)) {
        step = ((gMenuChoicePromptState[0] < 5) ? 1 : -1) * 8;
        arg0->transition.signedAlpha += step;

        if (arg0->transition.signedAlpha == 0x20) {
            gMenuChoicePromptState[0] += 2;
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

    if ((gMenuChoicePromptState[0] == 3) || (gMenuChoicePromptState[0] == 4)) {
        if ((u16)arg0->row.value < 0x10) {
            arg0->selection.counter = (u16)arg0->selection.counter - 9;
        } else {
            arg0->selection.counter = (u16)arg0->selection.counter + 9;
        }
        arg0->row.value = ((u16)arg0->row.value + 1) & 0x1F;
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCourseConfirmCursor, arg0);
}
// clang-format on

void initCharacterSelectCourseConfirmCursor(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x74;
    arg0->y = 0x2F;
    arg0->sprite.index = 0x2F;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCourseConfirmCursor);
}

void drawCharacterSelectCourseRecordsPopup(CharacterSelectCourseWidgetActor *actor) {
    s16 *selectedCourse;
    s32 assetHandle;
    s32 remainingScore;
    s32 pad[4];
    s32 digitOffset;
    s16 *courseOptions;
    s32 recordPalette;
    s32 rowIndex;
    u16 digits[2];
    u16 pad2;
    u8 rowPalette;
    s32 pad3;

    if (gRaceSplitscreenMode == 2) {
        courseOptions = gCharacterSelectCourseMenuData.courseOptionsByUnlock[gHighestUnlockedCourse];
    } else if (gRaceTypeSelection < 2) {
        courseOptions = gCharacterSelectCourseMenuData.shortCourseOptions;
    } else {
        courseOptions = gCharacterSelectCourseMenuData.singleCourseOption;
    }

    for (rowIndex = 0; rowIndex < 5; rowIndex++) {
        if (rowIndex < 3) {
            rowPalette = 6;
        } else {
            rowPalette = 4;
        }
        drawMenuSprite(
            actor->x,
            actor->y + (0x14 * rowIndex),
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            rowIndex + 0x77,
            0x20,
            0x20,
            0,
            0
        );
        if (gPlayerCount == 1) {
            if (gRaceSplitscreenMode != 2) {
                if (gRaceTypeSelection == 0) {
                    if (rowIndex < 3) {
                        rowPalette = 0xC;
                    } else {
                        rowPalette = 0xD;
                    }
                    drawCharacterSelectCourseRecordTime(
                        (
                            CharacterSelectCourseRecordTime *
                        )&gGameSaveDataBuffer->raceRecords[courseOptions[gRaceCourseIndex.signedValue]][rowIndex],
                        actor->x + 0x14,
                        actor->y + (0x14 * rowIndex),
                        rowPalette
                    );
                    drawMenuSprite(
                        actor->x + 0x54,
                        actor->y + (0x14 * rowIndex),
                        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                        (gGameSaveDataBuffer
                             ->raceRecordCharacterIds[courseOptions[gRaceCourseIndex.signedValue]][rowIndex] &
                         7) +
                            0x51,
                        0x20,
                        0x20,
                        0,
                        0
                    );
                    drawMenuSprite(
                        actor->x + 0x65,
                        actor->y + (0x14 * rowIndex),
                        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                        (gGameSaveDataBuffer
                             ->raceRecordCharacterIds[courseOptions[gRaceCourseIndex.signedValue]][rowIndex] >>
                         3) +
                            0x7C,
                        0x20,
                        0x20,
                        0,
                        0
                    );
                } else if (gRaceTypeSelection == 1) {
                    if (rowIndex < 3) {
                        recordPalette = 0xC;
                    } else {
                        recordPalette = 0xD;
                    }

                    digits[0] =
                        gGameSaveDataBuffer->scoreAttackScores[courseOptions[gRaceCourseIndex.signedValue]][rowIndex] /
                        10;
                    if (digits[0]) {
                        drawMenuSprite(
                            actor->x + 0x18,
                            actor->y + (0x14 * rowIndex),
                            getRelocatableHeapBlockBase((s32)gAssetHandles[0x1F]),
                            digits[0] + 0x2B,
                            0x20,
                            0x20,
                            0,
                            recordPalette + 1
                        );
                    }
                    digits[1] =
                        gGameSaveDataBuffer->scoreAttackScores[courseOptions[gRaceCourseIndex.signedValue]][rowIndex] %
                        10;

                    drawMenuSprite(
                        actor->x + 0x20,
                        actor->y + (0x14 * rowIndex),
                        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                        digits[1] + 0x2B,
                        0x20,
                        0x20,
                        0,
                        recordPalette + 1
                    );
                    drawMenuAsciiText(
                        actor->x + 0x28,
                        (actor->y + (0x14 * rowIndex)) + 8,
                        gCharacterSelectCourseRecordHitsLabel,
                        rowPalette,
                        0x100
                    );
                    drawMenuSprite(
                        actor->x + 0x48,
                        actor->y + (0x14 * rowIndex),
                        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                        (gGameSaveDataBuffer
                             ->scoreAttackCharacterIds[courseOptions[gRaceCourseIndex.signedValue]][rowIndex] &
                         7) +
                            0x51,
                        0x20U,
                        0x20U,
                        0U,
                        0U
                    );
                    drawMenuSprite(
                        actor->x + 0x60,
                        actor->y + (0x14 * rowIndex),
                        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                        (gGameSaveDataBuffer
                             ->scoreAttackCharacterIds[courseOptions[gRaceCourseIndex.signedValue]][rowIndex] >>
                         3) +
                            0x7C,
                        0x20U,
                        0x20U,
                        0,
                        0
                    );
                } else {
                    if (rowIndex < 3) {
                        recordPalette = 0xC;
                    } else {
                        recordPalette = 0xD;
                    }

                    remainingScore = gGameSaveDataBuffer->trickAttackScores[7][rowIndex];
                    for (digitOffset = 0;;) {
                        drawMenuSprite(
                            (s16)((actor->x - digitOffset) + 0x38),
                            (s16)(actor->y + (0x14 * rowIndex)),
                            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                            (remainingScore % 10) + 0x2B,
                            0x20U,
                            0x20U,
                            0U,
                            recordPalette + 1
                        );
                        digitOffset += 8;
                        remainingScore /= 10;
                        if (remainingScore == 0) {
                            break;
                        }
                    }
                    drawMenuAsciiText(
                        actor->x + 0x40,
                        (actor->y + (0x14 * rowIndex)) + 8,
                        gCharacterSelectCourseRecordPointsLabel,
                        rowPalette,
                        0x100U
                    );
                    drawMenuSprite(
                        actor->x + 0x4C,
                        actor->y + (0x14 * rowIndex),
                        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                        (gGameSaveDataBuffer->trickAttackCharacterIds[7][rowIndex] & 7) + 0x51,
                        0x20U,
                        0x20U,
                        0,
                        0
                    );
                    drawMenuSprite(
                        actor->x + 0x60,
                        actor->y + (0x14 * rowIndex),
                        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                        (gGameSaveDataBuffer->trickAttackCharacterIds[7][rowIndex] >> 3) + 0x7C,
                        0x20U,
                        0x20,
                        0,
                        0
                    );
                }
            } else {
                if (rowIndex < 3) {
                    rowPalette = 0xC;
                } else {
                    rowPalette = 0xD;
                }
                drawCharacterSelectCourseRecordTime(
                    (
                        CharacterSelectCourseRecordTime *
                    )&gGameSaveDataBuffer->timeTrialRecords[courseOptions[gRaceCourseIndex.signedValue]][rowIndex],
                    actor->x + 0x14,
                    actor->y + (0x14 * rowIndex),
                    rowPalette
                );
                drawMenuSprite(
                    actor->x + 0x54,
                    actor->y + (0x14 * rowIndex),
                    getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                    (gGameSaveDataBuffer->timeTrialCharacterIds[courseOptions[gRaceCourseIndex.signedValue]][rowIndex] &
                     7) +
                        0x51,
                    0x20U,
                    0x20,
                    0,
                    0
                );
                drawMenuSprite(
                    actor->x + 0x65,
                    actor->y + (0x14 * rowIndex),
                    getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                    (gGameSaveDataBuffer
                         ->timeTrialCharacterIds[courseOptions[gRaceCourseIndex.signedValue]][rowIndex] >>
                     3) +
                        0x7C,
                    0x20U,
                    0x20U,
                    0,
                    0
                );
            }
        }
    }
}

// clang-format off
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
            if ((gRacePlayers[0].menuState == 3) || (gRacePlayers[0].menuState == 7)) {
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
        gRacePlayers[0].menuState = 4;
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCourseRecordsPopup, arg0);
}
// clang-format on

void initCharacterSelectCourseRecordsPopup(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0xF0;
    arg0->y = -0x40;
    arg0->sprite.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCourseRecordsPopup);
}

void drawCharacterSelectCourseExitPopup(CharacterSelectCourseWidgetActor *arg0) {
    drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(ASSET_HANDLE(0x21)), 0x28, 0x20, 0x20, 0, 0);
    if ((s32)arg0->transition.bytes.state > 0) {
        drawMenuGlyphScript((s16)(arg0->x + 4), (s16)(arg0->y + 4), gCharacterSelectCourseStartGameText, 1, 0x100, 0);
        drawMenuSprite(
            (s16)(arg0->x + 0x64),
            (s16)(arg0->y + 4),
            getRelocatableHeapBlockBase(ASSET_HANDLE(0x24)),
            (u16)((arg0->transition.bytes.timer >= 8) + 5),
            0x20,
            0x20,
            0,
            0
        );
    }
}

// clang-format off
void updateCharacterSelectCourseExitPopup(CharacterSelectCourseWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = gCharacterSelectCourseCursorState.submenuState)) {
        arg0->transition.bytes.state = globalState;
        /* Preserve IDO's state/globalState register allocation. */
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        state = globalState;
        arg0->transition.bytes.timer = gCharacterSelectCourseCursorState.submenuTimer;
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
            if (gRacePlayers[0].menuState == 3) {
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
            if ((gRacePlayers[0].menuState == 3) || (gRacePlayers[0].menuState == 7)) {
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

    (arg0 ? &gCharacterSelectCourseCursorState : &gCharacterSelectCourseCursorState)->submenuState = state;
    (arg0 ? &gCharacterSelectCourseCursorState : &gCharacterSelectCourseCursorState)
        ->submenuTimer = arg0->transition.bytes.timer;

    if (arg0->transition.bytes.state == 6) {
        removeCallbackTask(arg0);
        gMenuScratch0.task = NULL;
        gCharacterSelectCourseCursorState.submenuState = 0;
        gCharacterSelectCourseCursorState.submenuTimer = 0;
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCourseExitPopup, arg0);
}
// clang-format on

void initCharacterSelectCourseExitPopup(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = 0x28;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCourseExitPopup);
}

void drawCharacterSelectCourseRecordTime(CharacterSelectCourseRecordTime *arg0, s32 x, s32 arg2, s32 arg3) {
    char buffer[0x58];
    CharacterSelectCourseRecordTime *record;
    s16 y;
    u16 color;
    s32 i;

    record = arg0;
    y = arg2;
    color = arg3;
    sprintf(&buffer[-0x10], D_800DFEC8, record->minutes);
    i = -0x10;
    while (i < -0xE) {
        {}
        drawAssetTableSpriteWithExplicitPaletteWideIndex(
            (s16)x,
            y,
            getRelocatableHeapBlockBase(CHARACTER_SELECT_POPUP_FONT_HANDLE),
            ((u8)buffer[i] - 5) & 0xFFFF,
            color
        );
        i++;
        x += 8;
    }
    drawAssetTableSpriteWithExplicitPaletteWideIndex(
        (s16)x,
        y,
        getRelocatableHeapBlockBase(CHARACTER_SELECT_POPUP_FONT_HANDLE),
        0x36,
        color
    );

    if ((gRaceSplitscreenMode == 2) || ((gRaceSplitscreenMode == 1) && (gRaceTypeSelection == 0))) {
        x += 6;
    } else {
        x += 8;
    }

    sprintf(&buffer[-0x10], D_800DFED0, record->seconds);
    i = -0x10;
    while (i < -0xE) {
        drawAssetTableSpriteWithExplicitPaletteWideIndex(
            (s16)x,
            y,
            getRelocatableHeapBlockBase(CHARACTER_SELECT_POPUP_FONT_HANDLE),
            ((u8)buffer[i] - 5) & 0xFFFF,
            color
        );
        i++;
        x += 8;
    }
    drawAssetTableSpriteWithExplicitPaletteWideIndex(
        (s16)x,
        y,
        getRelocatableHeapBlockBase(CHARACTER_SELECT_POPUP_FONT_HANDLE),
        0x35,
        color
    );

    x += 8;
    sprintf(&buffer[-0x10], D_800DFED8, record->centiseconds >> 8);
    i = -0x10;
    while (i != -0xE) {
        drawAssetTableSpriteWithExplicitPaletteWideIndex(
            (s16)x,
            y,
            getRelocatableHeapBlockBase(CHARACTER_SELECT_POPUP_FONT_HANDLE),
            ((u8)buffer[i] - 5) & 0xFFFF,
            color
        );
        i++;
        x += 8;
    }
}
