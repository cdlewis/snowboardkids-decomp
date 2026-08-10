#include "game/race/race_state.h"
#include "font_encoding.h"
#include "common.h"
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

#define CHARACTER_SELECT_FRAME_TEXTURE_HANDLE (gAssetHandles[0x21])
#define CHARACTER_SELECT_ICON_TEXTURE_HANDLE (gAssetHandles[0x24])
#define CHARACTER_SELECT_POPUP_FONT_HANDLE (gAssetHandles[0x1F])
#define ASSET_HANDLE(index) (gAssetHandles[index])

typedef MenuGlyphScript CharacterSelectCourseText[0x4A];

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
    u16 cornerTile;
    u16 padding;
    MenuGlyphScript exitText[14];
} CharacterSelectCourseExitPreviewData;

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

extern int sprintf(char *, const char *, ...);
extern void
drawAssetTableSpriteWithExplicitPaletteWideIndex(s16 x, s16 y, AssetTable *table, s32 entryIndex, u16 paletteIndex);
u16 gCharacterSelectCoursePreviewFrameTileMaps[20] = {
    0x0161, 0x0162, 0x0163, 0x0164, 0x0166, 0x0167, 0x0168, 0x0169, 0x016A, 0x016B,
    0x016C, 0x016D, 0x016F, 0x0170, 0x0171, 0x0172, 0x0165, 0x016E, 0x0173, 0x0174,
};

u16 gCharacterSelectCoursePreviewFrameCornerTileMaps[210] = {
    0x0175, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BE, 0x00BF, 0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C7,
    0x00C8, 0x00C9, 0x00CA, 0x00BD, 0x00C6, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF, 0x00D0, 0x00D1, 0x00D3, 0x00D4,
    0x00D5, 0x00D6, 0x00D7, 0x00D8, 0x00D9, 0x00DA, 0x00DC, 0x00DD, 0x00DE, 0x00DF, 0x00D2, 0x00DB, 0x00E0, 0x00E1,
    0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF, 0x00F1,
    0x00F2, 0x00F3, 0x00F4, 0x00E7, 0x00F0, 0x00F5, 0x00F6, 0x00F7, 0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FD, 0x00FE,
    0x00FF, 0x0100, 0x0101, 0x0102, 0x0103, 0x0104, 0x0106, 0x0107, 0x0108, 0x0109, 0x00FC, 0x0105, 0x010A, 0x010B,
    0x010C, 0x010D, 0x010E, 0x010F, 0x0110, 0x0112, 0x0113, 0x0114, 0x0115, 0x0116, 0x0117, 0x0118, 0x0119, 0x011B,
    0x011C, 0x011D, 0x011E, 0x0111, 0x011A, 0x011F, 0x0120, 0x0121, 0x0122, 0x0123, 0x0124, 0x0125, 0x0127, 0x0128,
    0x0129, 0x012A, 0x012B, 0x012C, 0x012D, 0x012E, 0x0130, 0x0131, 0x0132, 0x0133, 0x0126, 0x012F, 0x0134, 0x0135,
    0x0136, 0x0137, 0x0138, 0x0139, 0x013A, 0x013C, 0x013D, 0x013E, 0x013F, 0x0140, 0x0141, 0x0142, 0x0143, 0x0145,
    0x0146, 0x0147, 0x0148, 0x013B, 0x0144, 0x0149, 0x014A, 0x014B, 0x014C, 0x014D, 0x014E, 0x014F, 0x0151, 0x0152,
    0x0153, 0x0154, 0x0155, 0x0156, 0x0157, 0x0158, 0x015A, 0x015B, 0x015C, 0x015D, 0x0150, 0x0159, 0x015E, 0x015F,
    0x0160, 0x0176, 0x0177, 0x0178, 0x0179, 0x017B, 0x017C, 0x017D, 0x017E, 0x017F, 0x0180, 0x0181, 0x0182, 0x0184,
    0x0185, 0x0186, 0x0187, 0x017A, 0x0183, 0x0188, 0x0189, 0x018A, 0x00A4, 0x00A5, 0x00A6, 0x00A7, 0x00A9, 0x00AA,
    0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF, 0x00B0, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00A8, 0x00B1, 0x00B6, 0x00B7,
};

CharacterSelectCourseExitPreviewData gCharacterSelectCourseExitPreviewData = {
    0xB8,
    0,
    // textconv requires these _() invocations to retain their original line layout.
    // clang-format off
    { _("COURSE SELECT") },
};

u16 gCharacterSelectCourseStatsScoreValues[10] = {
    0x021D, 0x06D6, 0x06AA, 0x0558, 0x05D2, 0x05D2, 0x0666, 0x0680,
    0x017E, 0x01F4, 
};

u8 gCharacterSelectCourseDifficultyRatings[12] = {
    0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x08, 0x07,
    0x04, 0x05, 0x00, 0x00, 
};

u8 gCharacterSelectCourseMedalScoreThresholds[68] = {
    0x0B, 0xB8, 0x03, 0x20, 0x01, 0x90, 0x0E, 0x10,
    0x03, 0x84, 0x01, 0xF4, 0x0F, 0xA0, 0x03, 0xE8,
    0x01, 0xF4, 0x11, 0x30, 0x04, 0x4C, 0x02, 0x58,
    0x14, 0x50, 0x05, 0x14, 0x02, 0xBC, 0x17, 0x70,
    0x05, 0xDC, 0x03, 0x20, 0x1B, 0x58, 0x07, 0x08,
    0x03, 0x84, 0x1F, 0x40, 0x07, 0xD0, 0x03, 0xE8,
    0x27, 0x10, 0x09, 0xC4, 0x05, 0x14, 0x01, 0x2C,
    0x00, 0xC8, 0x00, 0x64, 0x01, 0x2C, 0x00, 0xC8,
    0x00, 0x64, 0x00, 0x00,
};

CharacterSelectCourseText gCharacterSelectCourseNameText[10] = {
    {
        _("{COLOR:7}A SHORT\nBEGINNERS\nCOURSE.\n   {COLOR:6}-5 LAPS-"),
    },
    {
        _("{COLOR:7}A MOUNTAIN\nCOVERED WITH\nSNOWMEN.\n   {COLOR:6}-2 LAPS-"),
    },
    {
        _("{COLOR:7}A MOUNTAIN\nWHERE\nPENGUINS\nOFTEN PLAY.\n   {COLOR:6}-3 LAPS-"),
    },
    {
        _("{COLOR:7}AN ABANDONED\nHIGHWAY AT\nNIGHT.\n   {COLOR:6}-3 LAPS-"),
    },
    {
        _("{COLOR:7}A GRASS\nCOVERED\nMOUNTAIN.\n   {COLOR:6}-3 LAPS-"),
    },
    {
        _("{COLOR:7}AFTER-HOURS\nAMUSEMENT\nPARK.\n   {COLOR:6}-3 LAPS-"),
    },
    {
        _("{COLOR:7}A HOT AND\nSANDY\nCOURSE.\n   {COLOR:6}-3 LAPS-"),
    },
    {
        _("{COLOR:7}A COURSE\nIN THE MIST.\n\n   {COLOR:6}-3 LAPS-"),
    },
    {
        _("{COLOR:7}A COURSE\nWHERE\nNINJA'S\nROAM.\n   {COLOR:6}-9 LAPS-"),
    },
    {
        _("{COLOR:7}RETURN TO\nMAIN MENU."),
    },
};

MenuGlyphScript gCharacterSelectBeginnerCourseDescriptionText[3][0x3A] = {
    {
        _("{COLOR:7}SLIDE DOWN\nFAST.\n\n{COLOR:6}SPEED FAN:10\nTIME:1'00\"00"),
    },
    {
        _("{COLOR:7}SLIDE DOWN\nFASTER.\n\n{COLOR:6}SPEED FAN:20\nTIME:2'30\"00"),
    },
    {
        _("{COLOR:7}SLIDE DOWN\nEVEN FASTER.\n\n{COLOR:6}SPEED FAN:20\nTIME:2'30\"00"),
    },
};

MenuGlyphScript gCharacterSelectAdvancedCourseDescriptionText[3][0x30] = {
    {
        _("{COLOR:6}\nSNOWMEN:30\nTIME:1'30\"00"),
    },
    {
        _("{COLOR:6}\nSNOWMEN:60\nTIME:3'00\"00"),
    },
    {
        _("{COLOR:6}\nSNOWMEN:60\nTIME:3'00\"00"),
    },
};

MenuGlyphScript gCharacterSelectExpertCourseDescriptionText[2][0x38] = {
    {
        _("{COLOR:6}HALFPIPE\n{COLOR:7}TO DO\nTRICKS.\n{COLOR:6}TIME:3'00\"00"),
    },
    {
        _(""),
    },
};

MenuGlyphScript gCharacterSelectCourseReturnDescriptionText[42] = {
    _("{COLOR:7}RETURN TO\nTHE SKILL\nGAME SELECT\nSCREEN."),
};

MenuGlyphScript gCharacterSelectCourseStartGameText[44] = {
    _("{COLOR:7}START GAME"),
    0, 0, 0, 0, 0, 0, 0,
    _("BOARD SELECT"),
    // clang-format on
    0,
    _FRAGMENT("BOARD PAIN"),
};

#define gCharacterSelectCoursePreviewFrameTileMaps \
    ((CharacterSelectCourseFrameTileMap *)gCharacterSelectCoursePreviewFrameTileMaps)
#define gCharacterSelectCoursePreviewFrameCornerTileMaps \
    ((CharacterSelectCourseFrameCornerTileMap *)gCharacterSelectCoursePreviewFrameCornerTileMaps)
#define gCharacterSelectCourseExitPreviewCornerTile (gCharacterSelectCourseExitPreviewData.cornerTile)
extern void *D_8010ADE0;
extern void *D_8010ADE4;
extern u8 gCharacterSelectCourseCursorStateByte;
extern u8 gCharacterSelectCourseSubmenuState;
extern u8 D_8010AE8F;
extern u8 gHighestUnlockedCourse;
extern u8 gCourseSelectFromRaceTypeMenu;
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
        } while (i < actor->itemCount);
    }
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
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
                    D_8010ADE4 = createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewFrame, 0, 0x58);
                    createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewPanel1, 0, 0x59);
                    createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewPanel2, 0, 0x5A);
                    createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewPanel3, 0, 0x5B);
                    createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewPanel4, 0, 0x5C);
                    createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewPanel5, 0, 0x5D);
                    if (gHighestUnlockedCourse != 0) {
                        createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewPanel6, 0, 0x5E);
                        if (gHighestUnlockedCourse >= 2) {
                            createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewPanel7, 0, 0x5F);
                            if (gHighestUnlockedCourse >= 3) {
                                createCallbackTask((CallbackTaskCallback)initCharacterSelectCoursePreviewPanel8, 0, 0x60);
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
        }

        if (gRaceCourseIndex.signedValue != 0) {
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
                D_8010ADE0 = createCallbackTask((CallbackTaskCallback)initCharacterSelectCourseExitPopup, 0, 0x62);
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
        visibleCourseCount = gCharacterSelectCourseExitOptionIndex + 1; index = 0; if (visibleCourseCount > 0) { do {
            if (index != gRaceCourseIndex.signedValue) {
                arg0->x[index] += 0x20;
                visibleCourseCount = gCharacterSelectCourseExitOptionIndex + 1;
            }
            index++;
        } while (index < (gCharacterSelectCourseExitOptionIndex + 1));
        }
        if (gRaceCourseIndex.signedValue != 0) {
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
    }

    if ((arg0->state == COURSE_LIST_EXIT) && (arg0->x[0] < -0x103)) {
        removeCallbackTask(arg0);
        return;
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCourseListOptions, arg0);
}
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
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
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectUnlockedCourseList);
}
// clang-format on

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
                        D_8010ADE4 =
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

            if (gRaceCourseIndex.signedValue != 0) {
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
                D_8010ADE0 = createCallbackTask((CallbackTaskCallback)initCharacterSelectCourseExitPopup, 0, 0x63);
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
            if (visibleOptionCount > 0) {
                do {
                    if (optionIndex != gRaceCourseIndex.signedValue) {
                        arg0->x[optionIndex] += 0x20;
                    }
                    optionIndex++;
                } while (optionIndex < (extraCourseOptionCount + 2));
            }
            if (gRaceCourseIndex.signedValue != 0) {
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

    if (switchState != gCharacterSelectCourseCursorState.fields.previewFrameState) {
        switchState = state = arg0->transition.bytes.state =
            gCharacterSelectCourseCursorState.fields.previewFrameState;
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

    gCharacterSelectCourseCursorState.fields.previewFrameState = state;
    if (arg0->transition.bytes.state == 7) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCoursePreviewFrame, arg0);
}

void initCharacterSelectCoursePreviewFrame(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = 0x96;
    arg0->y = -0x48;
    arg0->sprite.index = 0;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewFrame);
}

void drawCharacterSelectCoursePreviewPanel1(CharacterSelectCourseWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + ((i & 3) << 5)),
            (s16)(arg0->y + ((i / 4) << 5)),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + 0x80),
            (s16)(arg0->y + offset),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + offset),
            (s16)(arg0->y + 0x80),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped(
        (s16)(arg0->x + 0x80),
        (s16)(arg0->y + 0x80),
        getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
        gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner,
        0,
        0x100,
        0xA0,
        0x49
    );
}

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

void initCharacterSelectCoursePreviewPanel1(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x1;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewPanel1);
}

void drawCharacterSelectCoursePreviewPanel2(CharacterSelectCourseWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(
            arg0->x + ((i & 3) << 5),
            arg0->y + ((i / 4) << 5),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped(
            arg0->x + 0x80,
            arg0->y + offset,
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        drawMenuSpriteTileClipped(
            arg0->x + offset,
            arg0->y + 0x80,
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped(
        arg0->x + 0x80,
        arg0->y + 0x80,
        getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
        gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner,
        0,
        0x100,
        0xA0,
        0x49
    );
}

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
            0xA0,
            0x49
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
            0xA0,
            0x49
        );
        drawMenuSpriteTileClipped(
            arg0->x + i * 0x40,
            arg0->y + 0x80,
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
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
        0xA0,
        0x49
    );
}

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

void initCharacterSelectCoursePreviewPanel3(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x3;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewPanel3);
}

void drawCharacterSelectCoursePreviewPanel4(CharacterSelectCourseWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + ((i & 3) << 5)),
            (s16)(arg0->y + ((i / 4) << 5)),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + 0x80),
            (s16)(arg0->y + offset),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + offset),
            (s16)(arg0->y + 0x80),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped(
        (s16)(arg0->x + 0x80),
        (s16)(arg0->y + 0x80),
        getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
        gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner,
        0,
        0x100,
        0xA0,
        0x49
    );
}

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

void initCharacterSelectCoursePreviewPanel4(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x4;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewPanel4);
}

void drawCharacterSelectCoursePreviewPanel5(CharacterSelectCourseWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(
            arg0->x + ((i & 3) << 5),
            arg0->y + ((i / 4) << 5),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped(
            arg0->x + 0x80,
            arg0->y + offset,
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        drawMenuSpriteTileClipped(
            arg0->x + offset,
            arg0->y + 0x80,
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped(
        arg0->x + 0x80,
        arg0->y + 0x80,
        getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
        gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner,
        0,
        0x100,
        0xA0,
        0x49
    );
}

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

void initCharacterSelectCoursePreviewPanel5(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x5;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewPanel5);
}

void drawCharacterSelectCoursePreviewPanel6(CharacterSelectCourseWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + ((i & 3) << 5)),
            (s16)(arg0->y + ((i / 4) << 5)),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + 0x80),
            (s16)(arg0->y + offset),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + offset),
            (s16)(arg0->y + 0x80),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped(
        (s16)(arg0->x + 0x80),
        (s16)(arg0->y + 0x80),
        getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
        gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner,
        0,
        0x100,
        0xA0,
        0x49
    );
}

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

void initCharacterSelectCoursePreviewPanel6(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x6;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewPanel6);
}

void drawCharacterSelectCoursePreviewPanel7(CharacterSelectCourseWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(
            arg0->x + ((i & 3) << 5),
            arg0->y + ((i / 4) << 5),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped(
            arg0->x + 0x80,
            arg0->y + offset,
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        drawMenuSpriteTileClipped(
            arg0->x + offset,
            arg0->y + 0x80,
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped(
        arg0->x + 0x80,
        arg0->y + 0x80,
        getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
        gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner,
        0,
        0x100,
        0xA0,
        0x49
    );
}

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

void initCharacterSelectCoursePreviewPanel7(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x7;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewPanel7);
}

void drawCharacterSelectCoursePreviewPanel8(CharacterSelectCourseWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileIndex;
    s32 offset;

    tileIndex = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileIndex++) {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + ((i & 3) << 5)),
            (s16)(arg0->y + ((i / 4) << 5)),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[tileIndex],
            0,
            0x100,
            0xA0,
            0x49
        );
    }

    if (shouldDraw) {
        tileIndex = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + 0x80),
            (s16)(arg0->y + offset),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[tileIndex],
            0,
            0x100,
            0xA0,
            0x49
        );
        drawMenuSpriteTileClipped(
            (s16)(arg0->x + offset),
            (s16)(arg0->y + 0x80),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileIndex],
            0,
            0x100,
            0xA0,
            0x49
        );
        i = 0x80;
        offset += 0x40;
        tileIndex++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped(
        (s16)(arg0->x + 0x80),
        (s16)(arg0->y + 0x80),
        getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
        gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner,
        0,
        0x100,
        0xA0,
        0x49
    );
}

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

void initCharacterSelectCoursePreviewPanel8(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x8;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewPanel8);
}

void drawCharacterSelectCoursePreviewPanel9(CharacterSelectCourseWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    register s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(
            arg0->x + ((i & 3) << 5),
            arg0->y + ((i / 4) << 5),
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].center[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped(
            arg0->x + 0x80,
            arg0->y + offset,
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].right[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        drawMenuSpriteTileClipped(
            arg0->x + offset,
            arg0->y + 0x80,
            getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
            gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].bottom[tileOffset],
            0,
            0x100,
            0xA0,
            0x49
        );
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped(
        arg0->x + 0x80,
        arg0->y + 0x80,
        getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
        gCharacterSelectCoursePreviewFrameTileMaps[(u16)arg0->sprite.index].corner,
        0,
        0x100,
        0xA0,
        0x49
    );
}

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

void initCharacterSelectCoursePreviewPanel9(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x9;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCoursePreviewPanel9);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
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
// clang-format on

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

void updateCharacterSelectCourseListCursor(CharacterSelectCourseWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = gCharacterSelectCourseCursorState.fields.listCursorState)) {
        arg0->transition.bytes.state = globalState;
        state = globalState;
        arg0->sprite.index = gCharacterSelectCourseCursorState.fields.listCursorSpriteIndex;
        arg0->transition.bytes.timer = gCharacterSelectCourseCursorState.fields.listCursorTimer;
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
            if ((s32)arg0->transition.bytes.timer < 0x10) {
                arg0->sprite.index -= 9;
            } else {
                arg0->sprite.index += 9;
            }
            state = arg0->transition.bytes.state;
            arg0->transition.bytes.timer = (arg0->transition.bytes.timer + 1) & 0x1F;
            break;
        case 2:
            if (gRacePlayers[0].menuState == 1) {
                state = arg0->transition.bytes.state = 3;
            }
            break;
        case 3:
            break;
    }

    gCharacterSelectCourseCursorStateByte = state;
    if (gRacePlayers[0].menuState == 7) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCourseListCursor, arg0);
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

void initCharacterSelectCourseTitleCursor(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x5C;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCourseTitleCursor);
}

const char gCharacterSelectCourseStatsScoreFormat[] = "%4d";

void drawCharacterSelectCourseStatsBadge(CharacterSelectCourseWidgetActor *arg0) {
    char pad[8];
    char buf[4];
    s32 temp_v0;
    s32 xOffset;
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
            buf - 4,
            gCharacterSelectCourseStatsScoreFormat,
            gCharacterSelectCourseStatsScoreValues[gRaceCourseIndex.signedValue]
        );
        drawMenuAsciiText((s16)(arg0->x + 8), (s16)(arg0->y + 0xC), buf - 4, 0, arg0->sprite.index);
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
            do {
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
            } while (i < (temp_v0 / 2));
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

void initCharacterSelectCourseStatsBadge(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = 0x48;
    arg0->y = 0x34;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCourseStatsBadge);
}

const char gCharacterSelectCourseBestScoreFormat[] = "%5d";
const char gCharacterSelectCourseMedalScoreFormat[] = "%5d";
const char gCharacterSelectCoursePlayerRankFormat[] = "%2d";

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void drawCharacterSelectCoursePlayerStatsPanel(CharacterSelectCourseWidgetActor *arg0) {
    char buf[4];
    s16 *characterIds;
    s32 yOffset;
    s32 valueOffset;
    s32 three;
    register s32 i;
    do { characterIds = gCharacterSelectCourseOptionsByUnlock[gHighestUnlockedCourse]; if (gPlayerCount == 1) { drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), 0x21, 0x20, 0x20, 0, 0); drawMenuSprite((s16)(arg0->x + 0x30), arg0->y, getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), 0x22, 0x20, 0x20, 0, 0); sprintf(buf - 0x10, gCharacterSelectCourseBestScoreFormat, (&gGameSaveDataBuffer[0].highScores[1])[characterIds[*(&gRaceCourseIndex.signedValue)]]); drawMenuAsciiText((s16)(arg0->x + 0x14), (s16)(arg0->y + 0x2A), buf - 0x10, 0, 0x100); valueOffset = 0; yOffset = 0; three = 3; do { sprintf(buf - 0x10, gCharacterSelectCourseMedalScoreFormat, *((u16 *)(&gCharacterSelectCourseMedalScoreThresholds[(((*(&gRaceCourseIndex.signedValue)) * three) * 2) + valueOffset]))); drawMenuAsciiText((s16)(arg0->x + 0x28), (s16)((arg0->y + yOffset) + 9), buf - 0x10, 0, 0x100); yOffset += 8; valueOffset += 2; } while (yOffset != 0x18); } else { drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), 0x26, 0x20, 0x20, 0, 0); drawMenuSprite(arg0->x, (s16)(arg0->y + 0x10), getRelocatableHeapBlockBase(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), 0x27, 0x20, 0x20, 0, 0); yOffset = 0; for (i = 0; i < RACE_PLAYER_COUNT; i++) { sprintf(buf - 0x10, gCharacterSelectCoursePlayerRankFormat, gRacePlayers[i].unk18); drawMenuAsciiText((s16)(arg0->x + 0x40), (s16)((arg0->y + yOffset) + 0x10), buf - 0x10, 0, 0x100); yOffset += 8; } } } while (0);
}
// clang-format on

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

void updateCharacterSelectCourseSubmenuFrame(CharacterSelectCourseWidgetActor *arg0) {
    u8 state = arg0->sprite.bytes.state;

    switch (state) {
        case 0:
            arg0->x += 0x20;
            if (arg0->x >= -0x88) {
                arg0->x = -0x88;
                arg0->sprite.bytes.state = 1;
                D_8010ADE0 = createCallbackTask((CallbackTaskCallback)initCharacterSelectCourseNamePopup, 0, 0x63);
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

void updateCharacterSelectCourseNamePopup(CharacterSelectCourseWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = gCharacterSelectCourseCursorState.fields.submenuState)) {
        arg0->transition.bytes.state = globalState;
        state = globalState;
        arg0->transition.bytes.timer = gCharacterSelectCourseCursorState.fields.submenuTimer;
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
        gCharacterSelectCourseCursorState.fields.submenuState = 0;
        gCharacterSelectCourseCursorState.fields.submenuTimer = 0;
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCourseNamePopup, arg0);
}

void initCharacterSelectCourseNamePopup(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = -0x14;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCourseNamePopup);
}

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

void initCharacterSelectCourseDescriptionPopup(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = 0xC;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCourseDescriptionPopup);
}

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

void initCharacterSelectCourseConfirmCursor(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x74;
    arg0->y = 0x2F;
    arg0->sprite.index = 0x2F;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCourseConfirmCursor);
}

#include "character_select_course_records_popup.inc.c"

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

void updateCharacterSelectCourseExitPopup(CharacterSelectCourseWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = gCharacterSelectCourseCursorState.fields.submenuState)) {
        arg0->transition.bytes.state = globalState;
        /* Preserve IDO's state/globalState register allocation. */
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        state = globalState;
        arg0->transition.bytes.timer = gCharacterSelectCourseCursorState.fields.submenuTimer;
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

    gCharacterSelectCourseSubmenuState = state;
    D_8010AE8F = arg0->transition.bytes.timer;

    if (arg0->transition.bytes.state == 6) {
        removeCallbackTask(arg0);
        D_8010ADE0 = NULL;
        gCharacterSelectCourseCursorState.fields.submenuState = 0;
        gCharacterSelectCourseCursorState.fields.submenuTimer = 0;
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCharacterSelectCourseExitPopup, arg0);
}

void initCharacterSelectCourseExitPopup(CharacterSelectCourseWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = 0x28;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCharacterSelectCourseExitPopup);
}

const char D_800E0B98[] = "%2.2d";
const char D_800E0BA0[] = "%2.2d";
const char D_800E0BA8[] = "%2.2d";

void drawCharacterSelectCourseRecordTime(CharacterSelectCourseRecordTime *arg0, s32 x, s32 arg2, s32 arg3) {
    char buffer[0x58];
    CharacterSelectCourseRecordTime *record;
    s16 y;
    u16 color;
    s32 i;

    record = arg0;
    y = arg2;
    color = arg3;
    sprintf(&buffer[-0x10], D_800E0B98, record->minutes);
    i = -0x10;
    do {
        do {
        } while (0);
        drawAssetTableSpriteWithExplicitPaletteWideIndex(
            (s16)x,
            y,
            getRelocatableHeapBlockBase(CHARACTER_SELECT_POPUP_FONT_HANDLE),
            ((u8)buffer[i] - 5) & 0xFFFF,
            color
        );
        i++;
        x += 8;
    } while (i < -0xE);
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

    sprintf(&buffer[-0x10], D_800E0BA0, record->seconds);
    i = -0x10;
    do {
        drawAssetTableSpriteWithExplicitPaletteWideIndex(
            (s16)x,
            y,
            getRelocatableHeapBlockBase(CHARACTER_SELECT_POPUP_FONT_HANDLE),
            ((u8)buffer[i] - 5) & 0xFFFF,
            color
        );
        i++;
        x += 8;
    } while (i < -0xE);
    drawAssetTableSpriteWithExplicitPaletteWideIndex(
        (s16)x,
        y,
        getRelocatableHeapBlockBase(CHARACTER_SELECT_POPUP_FONT_HANDLE),
        0x35,
        color
    );

    x += 8;
    sprintf(&buffer[-0x10], D_800E0BA8, record->centiseconds >> 8);
    i = -0x10;
    do {
        drawAssetTableSpriteWithExplicitPaletteWideIndex(
            (s16)x,
            y,
            getRelocatableHeapBlockBase(CHARACTER_SELECT_POPUP_FONT_HANDLE),
            ((u8)buffer[i] - 5) & 0xFFFF,
            color
        );
        i++;
        x += 8;
    } while (i != -0xE);
}
