#include "game/race/race_state.h"
#include "font_encoding.h"
#include "common.h"
#include "game/save_data.h"
#include "game/engine/asset_manager.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/menu/character_select/character_select_menu.h"
#include "game/menu/course_select/course_select_menu.h"
#include "game/menu/course_select/course_select_ui.h"
#include "game/menu/course_select/course_select_shop_ui.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/math/fixed_point_math.h"
#include "game/race/player/race_player_model_renderer.h"
#include "game/race/player/race_player_input.h"

typedef struct {
    /* 0x0 */ u8 pad0;
    /* 0x1 */ u8 courseIndex;
} CourseSelectWidgetPlayerSlot;

typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ Mtx *matrix;
    /* 0x01C */ Transform3D sourceTransform;
    /* 0x03C */ Transform3D playerTransforms[4];
    /* 0x0BC */ u8 padBC[0x38];
    /* 0x0F4 */ CourseSelectWidgetPlayerSlot playerSlots[4];
    /* 0x0FC */ u8 playerFlags[4];
} CourseSelectCoursePreviewActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    union {
        struct {
            /* 0x18 */ s16 unk18;
            /* 0x1A */ s16 unk1A;
            /* 0x1C */ s16 unk1C;
            /* 0x1E */ s16 unk1E;
            /* 0x20 */ s16 unk20;
            /* 0x22 */ s16 unk22;
            /* 0x24 */ s16 unk24;
            /* 0x26 */ s16 unk26;
            /* 0x28 */ s16 unk28;
            /* 0x2A */ s16 unk2A;
            /* 0x2C */ s16 unk2C;
            /* 0x2E */ s16 unk2E;
            /* 0x30 */ s16 unk30;
            /* 0x32 */ s16 unk32;
            /* 0x34 */ s16 unk34;
            /* 0x36 */ s16 unk36;
            /* 0x38 */ s16 unk38;
            /* 0x3A */ s16 unk3A;
            /* 0x3C */ s16 unk3C;
            /* 0x3E */ s16 unk3E;
            /* 0x40 */ s16 unk40;
            /* 0x42 */ s16 unk42;
            /* 0x44 */ s16 unk44;
            /* 0x46 */ s16 unk46;
            /* 0x48 */ s16 unk48;
            /* 0x4A */ s16 unk4A;
            /* 0x4C */ s16 unk4C;
            /* 0x4E */ s16 unk4E;
            /* 0x50 */ s16 unk50;
            /* 0x52 */ s16 unk52;
            /* 0x54 */ s16 unk54;
            /* 0x56 */ s16 unk56;
            /* 0x58 */ s16 unk58;
            /* 0x5A */ u8 unk5A;
            /* 0x5B */ u8 unk5B;
            /* 0x5C */ u8 unk5C;
            /* 0x5D */ u8 unk5D;
        };
        struct {
            /* 0x18 */ s16 x[4];
            /* 0x20 */ s16 y[4];
            /* 0x28 */ s16 targetX[4];
            /* 0x30 */ s16 deltaX[4];
            /* 0x38 */ s16 deltaY[4];
            union {
                /* 0x40 */ s16 exitTargetX;
                /* 0x40 */ s16 exitTargetXs[1];
            };
            /* 0x42 */ u16 period[4];
            /* 0x4A */ u16 speed[4];
            /* 0x52 */ u16 timer[4];
            /* 0x5A */ u8 state[4];
        };
    };
} CourseSelectWidgetInitActor;

struct CourseSelectIconListActor {
    /* 0x00 */ u8 pad0[0x18];
    union {
        /* 0x18 */ s16 coordinates[0x40];
        struct {
            /* 0x18 */ s16 y[4][5];
            /* 0x40 */ s16 x[4][5];
            /* 0x68 */ s16 baseX[4];
            /* 0x70 */ s16 direction[4];
            /* 0x78 */ s16 startY[4];
            /* 0x80 */ s16 targetY[4];
            /* 0x88 */ s16 targetX[4];
            /* 0x90 */ u8 state[4];
            /* 0x94 */ u8 timer[4];
        };
    };
    /* 0x98 */ u8 itemCounts[4];
    /* 0x9C */ u8 speed[4];
    /* 0xA0 */ s16 clipLeft;
    /* 0xA2 */ s16 clipRight;
    /* 0xA4 */ s16 clipTop;
    /* 0xA6 */ s16 clipBottom;
};

union CourseSelectExtraCourseIconListActor {
    struct {
        /* 0x00 */ u8 pad0[0x18];
        /* 0x18 */ s16 iconX[4][3];
        /* 0x30 */ s16 iconY[4][3];
        /* 0x48 */ s16 clipLeft;
        /* 0x4A */ s16 clipRight;
        /* 0x4C */ s16 clipTop;
        /* 0x4E */ s16 clipBottom;
        /* 0x50 */ u8 rowSpacing[4];
        /* 0x54 */ u8 pulseTimer[4];
        /* 0x58 */ u8 revealTimer[4];
        /* 0x5C */ u8 itemCounts[4];
        /* 0x60 */ u16 alpha[4];
        /* 0x68 */ u16 tileIndices[4][3];
    };
    u8 bytes[0x80];
};

struct CourseSelectAnimatedActor {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ Mtx *matrix;
    /* 0x01C */ Transform3D sourceTransform;
    /* 0x03C */ Transform3D playerTransforms[4];
    /* 0x0BC */ Vec3i vecs[4];
    /* 0x0EC */ u16 angle[4];
    /* 0x0F4 */ s16 targetCourse[4];
    /* 0x0FC */ u8 timer[4];
    /* 0x100 */ u8 state[4];
};

typedef struct {
    /* 0x000 */ u8 pad0[0x100];
    /* 0x100 */ u8 playerStates[4];
} CourseSelectCompletePanelSource;

typedef struct {
    u8 speed;
    u8 handling;
    u8 trick;
} CourseSelectCourseRating;

typedef union {
    CourseSelectCourseRating courses[8];
    u8 values[0x18];
} CourseSelectCourseRatingTable;

typedef enum {
    COURSE_RATING_SPEED,
    COURSE_RATING_HANDLING,
    COURSE_RATING_TRICK,
    COURSE_RATING_COUNT
} CourseSelectCourseRatingType;

extern void drawCourseSelectPlayerPanels(CourseSelectWidgetActor *);
extern void drawMenuSpriteWithAlphaWideArgs(
    s32 x,
    s32 y,
    void *texture,
    s32 tileIndex,
    s32 width,
    s32 height,
    s32 palette,
    s32 alpha,
    u32 flip
);
extern const char D_800E0DB8[];
extern u8 D_800EC9C0;
extern CallbackTask *D_8010ADE0;
extern CallbackTask *D_8010ADE4;
extern s32 gMenuFlowState;
extern u8 gCurrentViewportIndex;

MenuGlyphScript gCourseSelectUnusedText[] = {
    // textconv requires these _() invocations to retain their original line layout.
    // clang-format off
    _("T"),
};

CourseModeDescriptionText gCourseSelectModeDescriptionText[7] = {
    { {
            _("{COLOR:7}SLOW, BUT EASY\nTO CONTROL."),
        } },
    { {
            _("{COLOR:7}BALANCED\nBOARD."),
        } },
    { {
            _("{COLOR:7}FAST, BUT HARD\nTO CONTROL."),
        } },
    { {
            _("{COLOR:7}RETURN TO\nCOURSE SELECT."),
        } },
    { {
            _("{COLOR:7}RETURN TO\nSHOP."),
        } },
    { {
            _("{COLOR:7}A VERY SPECIAL\nBOARD."),
        } },
    /* Unused graphical glyph data, not menu text. */
    { { 0x0095, 0x0091, 0x00A9, 0x00A1, 0x00A0, 0x008C, 0x0092, 0x0007, 0xFFFF, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000 } },
};

MenuGlyphScript gCourseSelectBoardLevelText[] = {
    _("{COLOR:7}SELECT\nBOARD\nLEVEL."),
};

CourseBoardLevelByCourseText gCourseSelectBoardLevelByCourseText[4] = {
    { {
            _("{COLOR:6} FREE STYLE\n    LEVEL"),
        } },
    { {
            _("{COLOR:6}  ALL ROUND\n    LEVEL"),
        } },
    { {
            _("{COLOR:6}   ALPINE\n    LEVEL"),
        } },
    /* Unused graphical glyph data, not menu text. */
    { { 0x004C, 0x0083, 0x004B, 0x003B, 0x0068, 0xFFFE, 0x0015, 0x000E, 0x001F, 0x000E, 0x0015,
        0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000 } },
};

CourseExtraBoardLevelText gCourseSelectExtraCourseBoardLevelText[3] = {
    { {
            _("{COLOR:7}FAST AND EASY\nTO CONTROL."),
        } },
    { {
            _("{COLOR:7}SPECIAL BOARD\nFOR TRICKS."),
        } },
    { {
            _("{COLOR:7}BOARD THAT IS\nUSELESS."),
        } },
};

CoursePurchaseMessageText gCourseSelectPurchaseMessageText[2] = {
    { {
            _("{COLOR:7}NOT ENOUGH\nMONEY."),
        } },
    { {
            _("{COLOR:7}OUT OF\nSTOCK."),
        } },
};

MenuGlyphScript gCourseSelectCompleteText[] = {
    _("{COLOR:7}SELECTION\nCOMPLETE."),
    // clang-format on
};

CourseSelectCourseRatingTable gCourseSelectCourseRatings = {
    {
     { 1, 3, 3 },
     { 2, 2, 2 },
     { 3, 1, 1 },
     { 2, 2, 2 },
     { 3, 1, 2 },
     { 3, 3, 3 },
     { 2, 2, 2 },
     { 0, 0, 0 },
     }
};

s16 gCourseSelectIconListYLayout[][6] = {
    { 0x0018, 0x0014, 0xFFA8, 0xFFA0, 0x0000, 0x0000 },
    { 0x000F, 0x000F, 0xFFB2, 0xFFB0, 0x0016, 0x0014 },
    { 0x000F, 0x000C, 0xFFAE, 0xFFAC, 0x0012, 0x0010 },
};

s16 gCourseSelectIconListXLayout[][4] = {
    { 0xFEFC, 0xFF84, 0x0000, 0x0000 },
    { 0xFF40, 0xFF88, 0x0000, 0x0000 },
    { 0xFF40, 0xFF7C, 0xFFCC, 0x0008 },
};

s16 gCourseSelectStatsPanelLayout[][4] = {
    { 0xFF88, 0x0034, 0x0000, 0x0000 },
    { 0xFFC8, 0xFFC0, 0xFFC8, 0x0024 },
    { 0xFF7C, 0xFFE0, 0xFF7C, 0x0044 },
};

s16 gCourseSelectStatsPlayerMarkerLayout[][4] = {
    { 0xFF88, 0xFFEA, 0xFF88, 0x004E },
    { 0xFFC0, 0xFFEC, 0xFFC0, 0x0050 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
};

void drawCourseSelectPreviewModel(CourseSelectCoursePreviewActor *arg0) {
    u8 sp2F;
    unsigned char sp2E;
    u8 var_t0;
    s8 temp_v0_2;
    RacePlayer *temp_v0_3;
    Transform3D sp30;
    u8 var_a3;
    u8 var_v1;
    int temp_v0;

    if ((gCourseSelectViewportSyncState != 0) && (gCurrentViewportIndex == 1)) {
        var_t0 = 0;
    } else {
        var_t0 = gCurrentViewportIndex;
    }
    if ((gCourseSelectSlideStates[var_t0] == 0) || (gCourseSelectSlideStates[var_t0] & 1)) {
        temp_v0 = arg0->playerFlags[var_t0];
        if ((temp_v0 == 0) || (temp_v0 & 1)) {
            if (gCourseSelectSlideStates[var_t0] == 1) {
                var_a3 = arg0->playerSlots[var_t0].courseIndex;
            } else {
                var_a3 = gRacePlayers[var_t0].menuSelection;
            }
            temp_v0_2 = gGameSaveDataBuffer[var_t0].courseUnlockStates[var_a3];
            if (temp_v0_2 == -1) {
                var_v1 = 9;
            } else {
                var_v1 = temp_v0_2;
            }
            if ((gCourseSelectViewportSyncState != 0) && (gCurrentViewportIndex == 1)) {
                var_v1 = (u8)(gCourseSelectViewportSyncState - 1);
            }
            temp_v0_3 = &gRacePlayers[gCurrentViewportIndex];
            if (temp_v0_3->selectedCharacterId == 5) {
                var_v1 = 0;
                var_a3 = (var_a3 % 3) + 0xC;
            }
            if (temp_v0_3->menuSelection >= 9) {
                var_v1 = 0;
            }
            sp2E = var_v1;
            sp2F = var_a3;
            composeFixedTransforms(&arg0->sourceTransform, &arg0->playerTransforms[var_t0], &sp30);
            arg0->matrix = allocFixedTransformMatrix(&sp30);
            if (arg0->matrix != 0) {
                drawSnowboardModel(arg0->matrix, (s16)sp2F, (s16)sp2E);
            }
        }
    }
}

void updateCourseSelectPreviewModelIn(CourseSelectAnimatedActor *arg0) {
    CourseSelectAnimatedActor *actor;
    Vec3i rotatedPosition;
    s32 i;
    s32 slideStep;

    actor = arg0;

    for (i = 0; i < (s32)gPlayerCount; i++) {
        if (gCourseSelectStatus.incomingPreviewModelState[i] != (actor->state[i] ^ 0)) {
            actor->state[i] = gCourseSelectStatus.incomingPreviewModelState[i];
            actor->timer[i] = gCourseSelectStatus.incomingPreviewModelTimer[i];
            actor->angle[i] = gCourseSelectStatus.incomingPreviewModelAngle[i];
            gCourseSelectStatus.incomingPreviewModelTimer[i] = 0;
            gCourseSelectStatus.incomingPreviewModelAngle[i] = 0;
        }

        if ((gMenuFlowState != 0) && ((actor->state[i] ^ 0) < 5)) {
            actor->state[i] = 4;
            actor->angle[i] = 0;
        }

        switch (actor->state[i] ^ 0) {
            case 0:
                actor->vecs[i].x -= 0x200000;
                if (actor->vecs[i].x <= 0) {
                    actor->vecs[i].x = 0;
                    actor->state[i] = 1;
                }
                break;
            case 1:
                if (gCourseSelectSlideStates[i] & 1) {
                    if (gCourseSelectHorizontalOffsets[i] < 0) {
                        if ((s32)gRacePlayers[i].characterVariant >= 9) {
                            actor->targetCourse[i] = 2;
                        } else {
                            actor->targetCourse[i] = (gRacePlayers[i].menuSelection % 3) - 1;
                        }
                    } else {
                        actor->targetCourse[i] = (gRacePlayers[i].menuSelection % 3) + 1;
                    }
                    if (actor->targetCourse[i] < 0) {
                        actor->targetCourse[i] = 2;
                    }
                    if (actor->targetCourse[i] == 3) {
                        actor->targetCourse[i] = gCourseSelectCourseIds[i][3];
                    }
                    if (actor->targetCourse[i] == 8) {
                        actor->targetCourse[i] = 2;
                    }
                    if (gCourseSelectSlideStates[i] == 3) {
                        actor->vecs[i].y = -gCourseSelectHorizontalOffsets[i];
                    }
                    actor->state[i] = 2;
                } else if (gCurrentGameTask->screenState == 9) {
                    actor->state[i] = 8;
                }
                break;
            case 2:
                slideStep = 0x200000;
                if (gCourseSelectHorizontalOffsets[i] < 0) {
                    slideStep = -0x200000;
                }
                actor->vecs[i].y += slideStep;
                if ((gCourseSelectHorizontalOffsets[i] ^ slideStep) == 0) {
                    actor->state[i] = 1;
                }
                break;
            case 3:
                actor->timer[i]++;
                if (D_800EC9C0 != 0) {
                    D_800EC9C0 = 1;
                }
                if (actor->timer[i] == 0xF) {
                    actor->timer[i] = 0;
                    actor->state[i] = 4;
                    if (gPlayerCount == 1) {
                        gRacePlayers[0].menuState = 3;
                        D_800EC9C0 = 0x10;
                    }
                }
                break;
            case 4:
                if (gRacePlayers[i].menuState == 3) {
                    actor->state[i] = 5;
                }
                break;
            case 5:
                actor->vecs[i].x += 0x200000;
                if (actor->vecs[i].x == 0x1000000) {
                    actor->state[i] = 6;
                }
                break;
            case 6:
                gRacePlayers[i].menuState = 4;
                if ((i == 2) && (gPlayerCount == 3)) {
                    gRacePlayers[3].menuState = 4;
                }
                break;
            case 7:
                if (gRacePlayers[i].menuState == 3) {
                    actor->state[i] = 5;
                } else if (gCurrentGameTask->screenState == 9) {
                    actor->state[i] = 8;
                }
                break;
            case 8:
                actor->vecs[i].x += 0x200000;
                if (actor->vecs[i].x == 0xC00000) {
                    actor->state[i] = 9;
                }
                break;
            case 9:
                break;
        }

        if ((actor->state[i] != 0) && ((s32)actor->state[i] < 3)) {
            actor->angle[i] += 0x20;
            actor->angle[i] &= 0xFFF;
        }

        makeFixedRotationY(actor->playerTransforms[i].rotation, (s16)actor->angle[i]);
        transformVec3iByFixedMatrix(actor->playerTransforms[i].rotation, &actor->vecs[i], &rotatedPosition);
        actor->playerTransforms[i].translation.x = rotatedPosition.x;
        actor->playerTransforms[i].translation.y = rotatedPosition.y;
        actor->playerTransforms[i].translation.z = rotatedPosition.z;
        gCourseSelectStatus.incomingPreviewModelState[i] = actor->state[i];
    }

    if ((gRacePlayers[0].menuState == 4) || (actor->state[0] == 9)) {
        removeCallbackTask(actor);
        finishCourseSelectUiTask(1);
        D_8010ADE0 = 0;
    } else {
        addRenderCallback(
            &gModelRenderCallbackList,
            (RenderCallback)(void (*)(CourseSelectWidgetActor *))drawCourseSelectPreviewModel,
            (CourseSelectWidgetActor *)actor
        );
    }
}

#if 0
void updateCourseSelectPreviewModelIn(CourseSelectAnimatedActor *arg0)
{
    CourseSelectAnimatedActor *actor;
    s32 incomingState;
    s32 i;
    Vec3i rotatedPosition;
    s32 slideStep;
    s32 state;

    actor = arg0;

    for (i = 0; i < (s32)gPlayerCount; i++) {
        incomingState = gCourseSelectStatus.incomingPreviewModelState[i];
        state = actor->state[i];
        if (incomingState != state) {
            actor->state[i] = gCourseSelectStatus.incomingPreviewModelState[i];
            actor->timer[i] = gCourseSelectStatus.incomingPreviewModelTimer[i];
            actor->angle[i] = gCourseSelectStatus.incomingPreviewModelAngle[i];
            gCourseSelectStatus.incomingPreviewModelTimer[i] = 0;
            gCourseSelectStatus.incomingPreviewModelAngle[i] = 0;
            state = actor->state[i];
        }

        /* Preserve IDO's state selector register allocation. */
        if ((gMenuFlowState != 0) && (slideStep = state < 5)) {
            actor->state[i] = 4;
            actor->angle[i] = 0;
            state = actor->state[i];
        }

        switch (state) {
        case 0:
            actor->vecs[i].x -= 0x200000;
            if (actor->vecs[i].x <= 0) {
                actor->vecs[i].x = 0;
                actor->state[i] = 1;
            }
            break;
        case 1:
            if (gCourseSelectSlideStates[i] & 1) {
                if (gCourseSelectHorizontalOffsets[i] < 0) {
                    if ((s32)gRacePlayers[i].characterVariant >= 9) {
                        actor->targetCourse[i] = 2;
                    } else {
                        actor->targetCourse[i] = (gRacePlayers[i].menuSelection % 3) - 1;
                    }
                } else {
                    actor->targetCourse[i] = (gRacePlayers[i].menuSelection % 3) + 1;
                }
                if (actor->targetCourse[i] < 0) {
                    actor->targetCourse[i] = 2;
                }
                if (actor->targetCourse[i] == 3) {
                    actor->targetCourse[i] = gCourseSelectCourseIds[i][3];
                }
                if (actor->targetCourse[i] == 8) {
                    actor->targetCourse[i] = 2;
                }
                if (gCourseSelectSlideStates[i] == 3) {
                    actor->vecs[i].y = -gCourseSelectHorizontalOffsets[i];
                }
                actor->state[i] = 2;
            } else if (gCurrentGameTask->screenState == 9) {
                actor->state[i] = 8;
            }
            break;
        case 2:
            slideStep = 0x200000;
            if (gCourseSelectHorizontalOffsets[i] < 0) {
                slideStep = -0x200000;
            }
            actor->vecs[i].y += slideStep;
            if ((gCourseSelectHorizontalOffsets[i] == slideStep) != 0) {
                actor->state[i] = 1;
            }
            break;
        case 3:
            actor->timer[i]++;
            if (D_800EC9C0 != 0) {
                D_800EC9C0 = 1;
            }
            if (actor->timer[i] == 0xF) {
                actor->timer[i] = 0;
                actor->state[i] = 4;
                if (gPlayerCount == 1) {
                    gRacePlayers[0].menuState = 3;
                    D_800EC9C0 = 0x10;
                }
            }
            break;
        case 4:
            if (gRacePlayers[i].menuState == 3) {
                actor->state[i] = 5;
            }
            break;
        case 5:
            actor->vecs[i].x += 0x200000;
            if (actor->vecs[i].x == 0x1000000) {
                actor->state[i] = 6;
            }
            break;
        case 6:
            gRacePlayers[i].menuState = 4;
            if ((i == 2) && (gPlayerCount == 3)) {
                gRacePlayers[3].menuState = 4;
            }
            break;
        case 7:
            if (gRacePlayers[i].menuState == 3) {
                actor->state[i] = 5;
            } else if (gCurrentGameTask->screenState == 9) {
                actor->state[i] = 8;
            }
            break;
        case 8:
            actor->vecs[i].x += 0x200000;
            if (actor->vecs[i].x == 0xC00000) {
                actor->state[i] = 9;
            }
            break;
        case 9:
            break;
        }

        if ((actor->state[i] != 0) && ((s32)actor->state[i] < 3)) {
            actor->angle[i] += 0x20;
            actor->angle[i] &= 0xFFF;
        }

        makeFixedRotationY(actor->playerTransforms[i].rotation, (s16)actor->angle[i]);
        transformVec3iByFixedMatrix(actor->playerTransforms[i].rotation, &actor->vecs[i],
                                    &rotatedPosition);
        actor->playerTransforms[i].translation.x = rotatedPosition.x;
        actor->playerTransforms[i].translation.y = rotatedPosition.y;
        actor->playerTransforms[i].translation.z = rotatedPosition.z;
        gCourseSelectStatus.incomingPreviewModelState[i] = actor->state[i];
    }

    if ((gRacePlayers[0].menuState == 4) || (actor->state[0] == 9)) {
        removeCallbackTask(actor);
        finishCourseSelectUiTask(1);
        D_8010ADE0 = 0;
    } else {
        addRenderCallback(&gModelRenderCallbackList, (RenderCallback)drawCourseSelectPreviewModel,
                          actor);
    }
}
#endif

void initCourseSelectPreviewModelIn(void *arg0) {
    CourseSelectAnimatedActor *actor;
    Vec3i position;
    s32 i;

    actor = arg0;
    i = 0;
    do {
        actor->vecs[i].x = 0xC00000;
        actor->vecs[i].y = 0;
        actor->vecs[i].z = 0;
        actor->angle[i] = 0;
        makeFixedRotationY(actor->playerTransforms[i].rotation, actor->angle[i]);
        transformVec3iByFixedMatrix(actor->playerTransforms[i].rotation, &actor->vecs[i], &position);
        actor->playerTransforms[i].translation.x = position.x;
        actor->playerTransforms[i].translation.y = position.y;
        actor->playerTransforms[i].translation.z = position.z;
        i++;
        actor->state[i - 1] = 0;
    } while (i < 4);

    actor->sourceTransform.translation.x = 0;
    actor->sourceTransform.translation.y = 0;
    actor->sourceTransform.translation.z = 0;
    makeFixedRotationYX(actor->sourceTransform.rotation, 0x400, 0x280);
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateCourseSelectPreviewModelIn);
}

void drawCourseSelectPreviewModelClose(CourseSelectCoursePreviewActor *arg0) {
    u8 sp2F;
    unsigned char sp2E;
    u8 var_t0;
    s8 temp_v0_2;
    RacePlayer *temp_v0_3;
    Transform3D sp30;
    u8 var_a3;
    u8 var_v1;
    int temp_v0;

    if ((gCourseSelectViewportSyncState != 0) && (gCurrentViewportIndex == 1)) {
        var_t0 = 0;
    } else {
        var_t0 = gCurrentViewportIndex;
    }
    if ((gCourseSelectSlideStates[var_t0] == 2) || (gCourseSelectSlideStates[var_t0] & 1)) {
        temp_v0 = arg0->playerFlags[var_t0];
        if ((temp_v0 == 0) || (temp_v0 & 1)) {
            if (gCourseSelectSlideStates[var_t0] == 3) {
                var_a3 = arg0->playerSlots[var_t0].courseIndex;
            } else {
                var_a3 = gRacePlayers[var_t0].menuSelection;
            }
            temp_v0_2 = gGameSaveDataBuffer[var_t0].courseUnlockStates[var_a3];
            if (temp_v0_2 == -1) {
                var_v1 = 9;
            } else {
                var_v1 = temp_v0_2;
            }
            if ((gCourseSelectViewportSyncState != 0) && (gCurrentViewportIndex == 1)) {
                var_v1 = (u8)(gCourseSelectViewportSyncState - 1);
            }
            temp_v0_3 = &gRacePlayers[gCurrentViewportIndex];
            if (temp_v0_3->selectedCharacterId == 5) {
                var_v1 = 0;
                var_a3 = (var_a3 % 3) + 0xC;
            }
            if (temp_v0_3->menuSelection >= 9) {
                var_v1 = 0;
            }
            sp2E = var_v1;
            sp2F = var_a3;
            composeFixedTransforms(&arg0->sourceTransform, &arg0->playerTransforms[var_t0], &sp30);
            arg0->matrix = allocFixedTransformMatrix(&sp30);
            if (arg0->matrix != 0) {
                drawSnowboardModel(arg0->matrix, (s16)sp2F, (s16)sp2E);
            }
        }
    }
}

void updateCourseSelectPreviewModelOut(CourseSelectAnimatedActor *arg0) {
    CourseSelectAnimatedActor *actor;
    Vec3i rotatedPosition;
    s32 i;
    s32 slideStep;

    actor = arg0;

    for (i = 0; i < (s32)gPlayerCount; i++) {
        if (gCourseSelectStatus.outgoingPreviewModelState[i] != actor->state[i]) {
            actor->state[i] = gCourseSelectStatus.outgoingPreviewModelState[i];
            actor->timer[i] = gCourseSelectStatus.outgoingPreviewModelTimer[i];
            actor->angle[i] = gCourseSelectStatus.outgoingPreviewModelAngle[i];
            gCourseSelectStatus.outgoingPreviewModelTimer[i] = 0;
            gCourseSelectStatus.outgoingPreviewModelAngle[i] = 0;
        }

        if ((gMenuFlowState != 0) && (actor->state[i] < 5)) {
            actor->state[i] = 4;
            actor->angle[i] = 0;
        }

        switch (actor->state[i]) {
            case 0:
                break;
            case 1:
                if (gCourseSelectSlideStates[i] & 1) {
                    if (gCourseSelectHorizontalOffsets[i] < 0) {
                        if ((s32)gRacePlayers[i].characterVariant >= 9) {
                            actor->targetCourse[i] = 2;
                        } else {
                            actor->targetCourse[i] = (gRacePlayers[i].menuSelection % 3) - 1;
                        }
                    } else {
                        actor->targetCourse[i] = (gRacePlayers[i].menuSelection % 3) + 1;
                    }
                    if (actor->targetCourse[i] < 0) {
                        actor->targetCourse[i] = 2;
                    }
                    if (actor->targetCourse[i] == 3) {
                        actor->targetCourse[i] = gCourseSelectCourseIds[i][3];
                    }
                    if (actor->targetCourse[i] == 8) {
                        actor->targetCourse[i] = 2;
                    }
                    if (gCourseSelectSlideStates[i] == 1) {
                        actor->vecs[i].y = -gCourseSelectHorizontalOffsets[i];
                    }
                    actor->state[i] = 2;
                } else if (gCurrentGameTask->screenState == 9) {
                    actor->state[i] = 8;
                }
                break;
            case 2:
                slideStep = 0x200000;
                if (gCourseSelectHorizontalOffsets[i] < 0) {
                    slideStep = -0x200000;
                }
                actor->vecs[i].y += slideStep;
                gCourseSelectHorizontalOffsets[i] -= slideStep;
                if (gCourseSelectHorizontalOffsets[i] == 0) {
                    actor->state[i] = 1;
                    gCourseSelectSlideStates[i]++;
                    gCourseSelectSlideStates[i] &= 3;
                }
                break;
            case 3:
                actor->timer[i]++;
                if (D_800EC9C0 != 0) {
                    D_800EC9C0 = 1;
                }
                if (actor->timer[i] == 0xF) {
                    actor->timer[i] = 0;
                    actor->state[i] = 4;
                    if (gPlayerCount == 1) {
                        gRacePlayers[0].menuState = 3;
                        D_800EC9C0 = 0x10;
                    }
                }
                break;
            case 4:
                if (gRacePlayers[i].menuState == 3) {
                    actor->state[i] = 5;
                }
                break;
            case 5:
                actor->vecs[i].x += 0x200000;
                if (actor->vecs[i].x == 0x1000000) {
                    actor->state[i] = 6;
                }
                break;
            case 6:
                gRacePlayers[i].menuState = 4;
                if ((i == 2) && (gPlayerCount == 3)) {
                    gRacePlayers[3].menuState = 4;
                }
                break;
            case 7:
                if (gRacePlayers[i].menuState == 3) {
                    actor->state[i] = 5;
                } else if (gCurrentGameTask->screenState == 9) {
                    actor->state[i] = 8;
                }
                break;
            case 8:
                actor->vecs[i].x += 0x200000;
                if (actor->vecs[i].x == 0xC00000) {
                    actor->state[i] = 9;
                    gCurrentGameTask->screenState = 0xB;
                }
                break;
            case 9:
                break;
        }

        if ((s32)actor->state[i] < 3) {
            actor->angle[i] += 0x20;
            actor->angle[i] &= 0xFFF;
        }

        makeFixedRotationY(actor->playerTransforms[i].rotation, (s16)actor->angle[i]);
        transformVec3iByFixedMatrix(actor->playerTransforms[i].rotation, &actor->vecs[i], &rotatedPosition);
        actor->playerTransforms[i].translation.x = rotatedPosition.x;
        actor->playerTransforms[i].translation.y = rotatedPosition.y;
        actor->playerTransforms[i].translation.z = rotatedPosition.z;
        gCourseSelectStatus.outgoingPreviewModelState[i] = actor->state[i];
    }

    if ((gRacePlayers[0].menuState == 4) || (actor->state[0] == 9)) {
        removeCallbackTask(actor);
        finishCourseSelectUiTask(2);
        D_8010ADE4 = 0;
    } else {
        addRenderCallback(
            &gModelRenderCallbackList,
            (RenderCallback)(void (*)(CourseSelectWidgetActor *))drawCourseSelectPreviewModelClose,
            (CourseSelectWidgetActor *)actor
        );
    }
}

void initCourseSelectPreviewModelOut(void *arg0) {
    CourseSelectAnimatedActor *actor;
    Vec3i position;
    s32 i;

    actor = arg0;
    for (i = 0; i < 4; i++) {
        actor->vecs[i].x = 0;
        actor->vecs[i].y = 0x800000;
        actor->vecs[i].z = 0;
        actor->angle[i] = 0;
        makeFixedRotationY(actor->playerTransforms[i].rotation, actor->angle[i]);
        transformVec3iByFixedMatrix(actor->playerTransforms[i].rotation, &actor->vecs[i], &position);
        actor->playerTransforms[i].translation.x = position.x;
        actor->playerTransforms[i].translation.y = position.y;
        actor->playerTransforms[i].translation.z = position.z;
        actor->state[i] = 1;
        gCourseSelectStatus.outgoingPreviewModelState[i] = 1;
    }

    actor->sourceTransform.translation.x = 0;
    actor->sourceTransform.translation.y = 0;
    actor->sourceTransform.translation.z = 0;
    makeFixedRotationYX(actor->sourceTransform.rotation, 0x400, 0x280);
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateCourseSelectPreviewModelOut);
}

void drawCourseSelectCourseIconList(CourseSelectIconListActor *iconList) {
    enum { CURRENT_ITEM_COUNT_ACTOR, PLAYER_ITEM_COUNT_ACTOR = 5, BASE_ICON_LIST_ACTOR = 11 };
    CourseSelectIconListActor *itemCountActors[12];
    s32 playerIndex;
    s32 iconIndex;
    s32 highlightAlpha;
    s32 iconTileIndex;
    s32 leftClipOffset;
    s32 selectedCourseId;
    s32 selectionState;
    s32 splitScreenClipOffset;
    CourseSelectIconListActor *playerItemCountActor;
    RacePlayer *player;

    itemCountActors[BASE_ICON_LIST_ACTOR] = iconList;
    playerIndex = 0;
    if ((s32)gPlayerCount > 0) {
        itemCountActors[CURRENT_ITEM_COUNT_ACTOR] = iconList;
        do {
            playerItemCountActor =
                (CourseSelectIconListActor *)&itemCountActors[BASE_ICON_LIST_ACTOR]->pad0[playerIndex];
            iconIndex = 0;
            leftClipOffset = (playerIndex >= 2) * 0x88;
            if (itemCountActors[CURRENT_ITEM_COUNT_ACTOR]->itemCounts[0] > 0) {
                itemCountActors[PLAYER_ITEM_COUNT_ACTOR] = playerItemCountActor;
                do {
                    if (iconIndex < (itemCountActors[CURRENT_ITEM_COUNT_ACTOR]->itemCounts[0] - 1)) {
                        do {
                            player = &gRacePlayers[playerIndex];
                        } while (0);
                        highlightAlpha = 0;
                        if (gPlayerCount == 1) {
                            if (player->selectedCharacterId != 5) {
                                iconTileIndex = (iconIndex + 1) & 0xFFFF;
                            } else {
                                iconTileIndex = (iconIndex + 6) & 0xFFFF;
                            }
                        } else if (player->selectedCharacterId != 5) {
                            iconTileIndex = (iconIndex + 0x14) & 0xFFFF;
                        } else {
                            iconTileIndex = (iconIndex + 0x18) & 0xFFFF;
                        }

                        selectionState = gCourseSelectSelectionTimers[playerIndex];
                        splitScreenClipOffset = (playerIndex >= 2) * 0x8C;
                        if ((selectionState > 0) && (selectionState < 8)) {
                            selectedCourseId = player->menuSelection;
                            if (selectedCourseId >= 9) {
                                if ((iconIndex == 3) && (selectionState & 1)) {
                                    highlightAlpha = 0xFF;
                                }
                            } else if ((iconIndex == (selectedCourseId % 3)) && (selectionState & 1)) {
                                highlightAlpha = 0xFF;
                            }
                        }

                        drawMenuSpriteClipped(
                            iconList->y[playerIndex][iconIndex],
                            iconList->x[playerIndex][iconIndex],
                            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                            (u16)iconTileIndex,
                            0x20,
                            0x20,
                            0,
                            highlightAlpha,
                            iconList->clipLeft - splitScreenClipOffset,
                            iconList->clipTop,
                            iconList->clipRight,
                            iconList->clipBottom
                        );
                    } else if (gPlayerCount == 1) {
                        highlightAlpha = 0;
                        leftClipOffset = (playerIndex >= 2) * 0x88;
                        if ((gCourseSelectSelectionTimers[playerIndex] > 0) && (gCourseSelectSelectionTimers[playerIndex] < 8) &&
                            (gCourseSelectExtraCourseColumnState == 1) && (gCourseSelectSelectionTimers[playerIndex] & 1)) {
                            highlightAlpha = 0xFF;
                        }

                        if (gRaceSplitscreenMode == 3) {
                            drawMenuSpriteClipped(
                                iconList->y[playerIndex][iconIndex],
                                iconList->x[playerIndex][iconIndex],
                                getRelocatableHeapBlockBase(gAssetHandles[0x27]),
                                5,
                                0x20,
                                0x20,
                                0,
                                highlightAlpha,
                                iconList->clipLeft - leftClipOffset,
                                iconList->clipTop,
                                iconList->clipRight,
                                iconList->clipBottom
                            );
                        } else {
                            drawMenuSpriteClipped(
                                iconList->y[playerIndex][iconIndex],
                                iconList->x[playerIndex][iconIndex],
                                getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                                5,
                                0x20,
                                0x20,
                                0,
                                highlightAlpha,
                                iconList->clipLeft - leftClipOffset,
                                iconList->clipTop,
                                iconList->clipRight,
                                iconList->clipBottom
                            );
                        }
                    }
                    iconIndex++;
                } while (iconIndex < itemCountActors[PLAYER_ITEM_COUNT_ACTOR]->itemCounts[0]);
            }
            playerIndex++;
            itemCountActors[CURRENT_ITEM_COUNT_ACTOR] =
                (CourseSelectIconListActor *)&itemCountActors[CURRENT_ITEM_COUNT_ACTOR]->pad0[1];
        } while (playerIndex < (s32)gPlayerCount);
    }
}

void updateCourseSelectCourseIconList(CourseSelectIconListActor *arg0) {
    CourseSelectIconListActor *actor;
    s32 playerIndex;
    s32 iconIndex;
    s32 movingCount;
    u8 count;
    u8 state;

    actor = arg0;
    for (playerIndex = 0; playerIndex < (s32)gPlayerCount; playerIndex++) {
        if (gCurrentGameTask->screenState == 1) {
            actor->state[playerIndex] = 9;
        }
        if ((gCurrentGameTask->screenState == 3) && (actor->state[playerIndex] < 11)) {
            actor->state[playerIndex] = 11;
        }
        if (gCurrentGameTask->screenState == 9) {
            actor->state[playerIndex] = 13;
        }
        state = actor->state[playerIndex];
        if ((gMenuFlowState != 0) && (state < 4)) {
            state = actor->state[playerIndex] = 4;
        }

        switch (state) {
            case 0:
                iconIndex = 0;
                for (movingCount = 0; movingCount < (s32)actor->itemCounts[playerIndex]; movingCount++) {
                    if (actor->y[playerIndex][movingCount] < actor->targetY[playerIndex]) {
                        actor->y[playerIndex][movingCount] += 0x10;
                        iconIndex++;
                        if (actor->y[playerIndex][movingCount] >= actor->targetY[playerIndex]) {
                            actor->y[playerIndex][movingCount] = actor->targetY[playerIndex];
                        }
                    }
                }

                actor->timer[playerIndex]++;
                if ((gRacePlayers[playerIndex].selectedCharacterId == 5) || (gCourseSelectHasExtraCourse[playerIndex] == 0) ||
                    (gCourseSelectModeSelection == 1)) {
                    count = 4;
                } else if (gCourseSelectHasExtraCourse[playerIndex] != 0) {
                    count = 5;
                }
                if (((actor->timer[playerIndex] & 1) == 0) && ((s32)actor->itemCounts[playerIndex] < (s32)count)) {
                    actor->itemCounts[playerIndex]++;
                }

                if ((u32)iconIndex == 0) {
                    actor->state[playerIndex] = 1;
                    if (gPlayerCount == 1) {
                        D_8010ADE0 = createCallbackTask((CallbackTaskCallback)initCourseSelectPreviewModelIn, 0, 0x63);
                        D_8010ADE4 = createCallbackTask((CallbackTaskCallback)initCourseSelectPreviewModelOut, 0, 0x62);
                        createCallbackTask((CallbackTaskCallback)initCourseSelectCourseListBackdrop, 0, 0x60);
                    } else if (playerIndex == 0) {
                        createCallbackTask((CallbackTaskCallback)initCourseSelectCourseStats, 0, 0x62);
                        gActiveMenuTask =
                            createCallbackTask((CallbackTaskCallback)initCourseSelectCourseCursors, 0, 0x64);
                    }
                }
                break;

            case 1:
                if (gRacePlayers[playerIndex].menuState == 1) {
                    actor->state[playerIndex] = 2;
                }
                break;

            case 2:
                if (gCourseSelectSelectionTimers[playerIndex] >= 9) {
                    for (iconIndex = 0; iconIndex < (s32)actor->itemCounts[playerIndex]; iconIndex++) {
                        if (iconIndex != gCharacterSelectHighlightedRosterIndices[playerIndex]) {
                            actor->y[playerIndex][iconIndex] -= 0x20;
                        }
                    }

                    if (gCharacterSelectHighlightedRosterIndices[playerIndex] != 0) {
                        if (actor->startY[playerIndex] >= actor->y[playerIndex][0]) {
                            actor->state[playerIndex] = 3;
                        }
                    } else if (actor->startY[playerIndex] >= actor->y[playerIndex][1]) {
                        actor->state[playerIndex] = 3;
                    }

                    if (actor->state[playerIndex] == 3) {
                        if (actor->x[playerIndex][gCharacterSelectHighlightedRosterIndices[playerIndex]] < actor->targetX[playerIndex]) {
                            actor->direction[playerIndex] = 1;
                        } else {
                            actor->direction[playerIndex] = -1;
                        }
                    }
                }
                break;

            case 3:
                actor->x[playerIndex][gCharacterSelectHighlightedRosterIndices[playerIndex]] +=
                    actor->speed[playerIndex] * actor->direction[playerIndex];
                if (((actor->direction[playerIndex] == 1) &&
                     (actor->x[playerIndex][gCharacterSelectHighlightedRosterIndices[playerIndex]] >= actor->targetX[playerIndex])) ||
                    ((actor->direction[playerIndex] == -1) &&
                     (actor->x[playerIndex][gCharacterSelectHighlightedRosterIndices[playerIndex]] <= actor->targetX[playerIndex]))) {
                    actor->x[playerIndex][gCharacterSelectHighlightedRosterIndices[playerIndex]] = actor->targetX[playerIndex];
                    actor->state[playerIndex] = 4;
                    gMenuChoicePromptState[playerIndex] = 1;
                }
                break;

            case 4:
                if ((gMenuFlowState == 0) && (gMenuChoicePromptState[playerIndex] == 0)) {
                    actor->state[playerIndex] = 5;
                }
                if (gRacePlayers[playerIndex].menuState == 3) {
                    actor->state[playerIndex] = 7;
                }
                break;

            case 5:
                actor->x[playerIndex][gCharacterSelectHighlightedRosterIndices[playerIndex]] +=
                    actor->speed[playerIndex] * actor->direction[playerIndex] * -1;
                if (((actor->direction[playerIndex] == 1) &&
                     (actor->x[playerIndex][gCharacterSelectHighlightedRosterIndices[playerIndex]] <=
                      actor->baseX[playerIndex] + (actor->speed[playerIndex] * gCharacterSelectHighlightedRosterIndices[playerIndex]))) ||
                    ((actor->direction[playerIndex] == -1) &&
                     (actor->x[playerIndex][gCharacterSelectHighlightedRosterIndices[playerIndex]] >=
                      actor->baseX[playerIndex] + (actor->speed[playerIndex] * gCharacterSelectHighlightedRosterIndices[playerIndex])))) {
                    actor->x[playerIndex][gCharacterSelectHighlightedRosterIndices[playerIndex]] =
                        actor->baseX[playerIndex] + (gCharacterSelectHighlightedRosterIndices[playerIndex] * actor->speed[playerIndex]);
                    actor->state[playerIndex] = 6;
                }
                break;

            case 6:
                for (iconIndex = 0; iconIndex < (s32)actor->itemCounts[playerIndex]; iconIndex++) {
                    if (iconIndex != gCharacterSelectHighlightedRosterIndices[playerIndex]) {
                        actor->y[playerIndex][iconIndex] += 0x20;
                    }
                }

                if (gCharacterSelectHighlightedRosterIndices[playerIndex] != 0) {
                    if (actor->y[playerIndex][0] >= actor->targetY[playerIndex]) {
                        actor->state[playerIndex] = 1;
                    }
                } else if (actor->y[playerIndex][1] >= actor->targetY[playerIndex]) {
                    actor->state[playerIndex] = 1;
                }
                if (actor->state[playerIndex] == 1) {
                    gRacePlayers[playerIndex].menuState = 0;
                }
                break;

            case 7:
                if (gMenuFlowState != 0) {
                    for (iconIndex = 0; iconIndex < (s32)actor->itemCounts[playerIndex]; iconIndex++) {
                        actor->y[playerIndex][iconIndex] -= 0x20;
                    }
                } else {
                    actor->y[playerIndex][gCharacterSelectHighlightedRosterIndices[playerIndex]] -= 0x20;
                }
                if (actor->y[playerIndex][gCharacterSelectHighlightedRosterIndices[playerIndex]] <= actor->startY[playerIndex]) {
                    actor->state[playerIndex] = 8;
                }
                break;

            case 9:
                actor->y[playerIndex][gCharacterSelectHighlightedRosterIndices[playerIndex]] -= 0x20;
                if (actor->y[playerIndex][gCharacterSelectHighlightedRosterIndices[playerIndex]] <= actor->startY[playerIndex]) {
                    actor->state[playerIndex] = 10;
                }
                break;

            case 11:
                actor->y[playerIndex][gCharacterSelectHighlightedRosterIndices[playerIndex]] += 0x20;
                if (actor->y[playerIndex][gCharacterSelectHighlightedRosterIndices[playerIndex]] >= actor->targetY[playerIndex]) {
                    actor->y[playerIndex][gCharacterSelectHighlightedRosterIndices[playerIndex]] = actor->targetY[playerIndex];
                    actor->state[playerIndex] = 12;
                }
                break;

            case 12:
                if (gCurrentGameTask->screenState == 4) {
                    actor->state[playerIndex] = 4;
                }
                break;

            case 8:
            case 10:
            case 13:
                break;
        }
    }

    if ((actor->state[0] == 8) || (actor->state[0] == 13)) {
        removeCallbackTask(arg0);
        finishCourseSelectUiTask(3);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseSelectCourseIconList, arg0);
    }
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initCourseSelectCourseIconList(CourseSelectIconListActor *arg0) {
    CourseSelectIconListActor *actor;
    u8 *courseUnlocked;
    s16 *yLayout;
    s16 *xLayout;
    s16 hasExtraCourse;
    s32 iconIndex;
    s32 playerIndex;
    s32 layoutIndex;

    actor = arg0;
    if ((s32) gPlayerCount < 3) {
        layoutIndex = gPlayerCount - 1;
    } else {
        layoutIndex = 2;
    }
    do { playerIndex = 0; if ((s32) gPlayerCount > 0) { courseUnlocked = gCourseSelectHasExtraCourse; yLayout = gCourseSelectIconListYLayout[layoutIndex]; xLayout = gCourseSelectIconListXLayout[layoutIndex]; do { if ((*courseUnlocked == 0) || (gRacePlayers[playerIndex].selectedCharacterId == 5)) { hasExtraCourse = 0; } else { hasExtraCourse = 1; } actor->speed[playerIndex] = yLayout[hasExtraCourse]; actor->baseX[playerIndex] = yLayout[((playerIndex & 1) * 2) + hasExtraCourse + 2]; actor->targetX[playerIndex] = yLayout[((playerIndex & 1) * 2) + 2]; actor->targetY[playerIndex] = xLayout[((playerIndex >= 2) * 2) + 1]; actor->startY[playerIndex] = xLayout[(playerIndex >= 2) * 2]; actor->clipBottom = 0x78; actor->clipRight = 0xA0; actor->clipTop = 0x78; if (gPlayerCount == 1) { actor->clipLeft = 0xA0; } else if (gPlayerCount == 2) { actor->clipLeft = 0x7E; } else { actor->clipLeft = 0x88; } iconIndex = 0; do { actor->y[playerIndex][iconIndex] = actor->startY[playerIndex]; actor->x[playerIndex][iconIndex] = actor->baseX[playerIndex] + (actor->speed[playerIndex] * iconIndex); iconIndex++; } while (iconIndex < 5); actor->state[playerIndex] = 0; actor->itemCounts[playerIndex] = 1; actor->timer[playerIndex] = 0; courseUnlocked++; playerIndex++; } while (playerIndex < (s32) gPlayerCount); } setCallbackTaskCallback(actor, (CallbackTaskCallback) updateCourseSelectCourseIconList); } while (0);
}
// clang-format on

void drawCourseSelectCourseCursors(CourseSelectWidgetActor *arg0) {
    u8 *actor;
    s16 *drawPosPtr;
    s32 i;
    u8 *statePtr;
    s16 *posPtr;
    int cursorStride;
    s16 *handles;
    u8 *cursorBytes;
    s32 tileIndex;
    s32 handleIndex;
    s32 one;
    s32 playerCount;
    u8 *cursorBase;

    actor = (u8 *)arg0;
    if (gCourseSelectInputLocked == 0) {
        playerCount = gPlayerCount;
        i = 0;
        if ((playerCount - 1) >= 0) {
            statePtr = (u8 *)arg0;
            cursorStride = 2;
            handles = gAssetHandles;
            one = 1;
            do {
                if (statePtr[0x30] < cursorStride) {
                    cursorBase = actor;
                    posPtr = (s16 *)((cursorBytes = cursorBase) + (i * cursorStride));
                    if (one == gPlayerCount) {
                        tileIndex = 7;
                        handleIndex = 0x25;
                    } else {
                        tileIndex = 0x13;
                        handleIndex = 0x21;
                    }
                    drawMenuSpriteWithAlpha(
                        posPtr[0xC],
                        (s16)((drawPosPtr = posPtr)[0x10] + (gCharacterSelectHighlightedRosterIndices[i] * statePtr[0x38])),
                        getRelocatableHeapBlockBase(handles[handleIndex]),
                        tileIndex,
                        0x20,
                        0x20,
                        0,
                        drawPosPtr[0x14],
                        (short)0
                    );
                }
                i++;
                statePtr++;
            } while (i < gPlayerCount);
        }
    }
}

void updateCourseSelectCourseCursors(CourseSelectWidgetActor *arg0) {
    register CourseSelectWidgetActor *actor;
    s32 i;
    RacePlayer *player;
    u8 state;

    actor = arg0;
    i = 0;
    if ((s32)gPlayerCount > 0) {
        do {
            if ((gMenuFlowState != 0) && (gRaceSplitscreenMode != 3)) {
                actor->courseCursorState[i] = 4;
            }
            state = actor->courseCursorState[i];
            switch (state) {
                case 0:
                    actor->courseCursorAlpha[i] += 0x26;
                    if (actor->courseCursorAlpha[i] >= 0x100) {
                        actor->courseCursorAlpha[i] = 0x100;
                        actor->courseCursorState[i] = 1;
                    }
                    state = actor->courseCursorState[i];
                    break;
                case 1:
                    player = &gRacePlayers[i];
                    if (actor->courseCursorTimer[i] < 0x10) {
                        actor->courseCursorAlpha[i] -= 9;
                    } else {
                        actor->courseCursorAlpha[i] += 9;
                    }
                    actor->courseCursorTimer[i] = (actor->courseCursorTimer[i] + 1) & 0x1F;
                    if ((player->menuState == 1) || (gRacePlayers[0].menuState == 3)) {
                        actor->courseCursorState[i] = 2;
                    }
                    state = actor->courseCursorState[i];
                    break;
                case 2:
                    player = &gRacePlayers[i];
                    if (player->menuState == 3) {
                        actor->courseCursorState[i] = 4;
                    }
                    if (player->menuState == 0) {
                        actor->courseCursorState[i] = 1;
                        actor->courseCursorAlpha[i] = 0x100;
                        actor->courseCursorTimer[i] = 0;
                    }
                    if (gCurrentGameTask->screenState == 9) {
                        actor->courseCursorState[i] = 4;
                    }
                    state = actor->courseCursorState[i];
                    break;
                case 3:
                case 4:
                    break;
            }
            i++;
            gCourseSelectStatus.bytes[i - 1] = state;
        } while (i < (s32)gPlayerCount);
    }

    if (actor->courseCursorState[0] == 4) {
        removeCallbackTask(actor);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseSelectCourseCursors, actor);
    }
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void initCourseSelectCourseCursors(CourseSelectWidgetActor *arg0) {
    CourseSelectWidgetActor *actor;
    u8 *courseUnlocked;
    s16 *yLayout;
    s16 *xLayout;
    char mask;
    s32 unlockState;
    s32 hasExtraCourse;
    s32 playerIndex;
    s32 layoutIndex;

    actor = arg0;
    mask = 0xFFFFFFFFFFFFFFFFu; if ((s32)gPlayerCount < 3) { layoutIndex = gPlayerCount - 1; } else { layoutIndex = 2; } playerIndex = 0; if ((s32)gPlayerCount > 0) { courseUnlocked = gCourseSelectHasExtraCourse; yLayout = gCourseSelectIconListYLayout[layoutIndex]; xLayout = gCourseSelectIconListXLayout[layoutIndex]; do { unlockState = *courseUnlocked & mask; courseUnlocked++; hasExtraCourse = (((0, unlockState) == 0) || (gRacePlayers[playerIndex].selectedCharacterId == 5)) ? 0 : 1;
            actor->courseCursorBobOffset[playerIndex] = yLayout[hasExtraCourse];
            actor->courseCursorY[playerIndex] =
                yLayout[((playerIndex & 1) * 2) + hasExtraCourse + 2];
            actor->courseCursorX[playerIndex] = xLayout[((playerIndex >= 2) * 2) + 1];
            actor->courseCursorAlpha[playerIndex] = 0;
            actor->courseCursorState[playerIndex] = 0;
            actor->courseCursorTimer[playerIndex] = 0;
            playerIndex++;
        } while (playerIndex < (s32)gPlayerCount);
    }
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateCourseSelectCourseCursors);
}
// clang-format on

void drawCourseSelectCourseListBackdrop(CourseSelectWidgetActor *arg0) {
    drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(gAssetHandles[0x25]), 3, 0x20, 0x20, 0, 0);
    drawMenuSprite(
        (s16)(arg0->x + 0x40),
        arg0->y,
        getRelocatableHeapBlockBase(gAssetHandles[0x25]),
        4,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        arg0->x,
        (s16)(arg0->y + 0x40),
        getRelocatableHeapBlockBase(gAssetHandles[0x25]),
        5,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0x40),
        (s16)(arg0->y + 0x40),
        getRelocatableHeapBlockBase(gAssetHandles[0x25]),
        6,
        0x20,
        0x20,
        0,
        0
    );
}

void updateCourseSelectCourseListBackdrop(CourseSelectWidgetActor *arg0) {
    s32 screenState;
    s32 forceState;

    forceState = 3;
    screenState = gCurrentGameTask->screenState;
    if (screenState == 1) {
        arg0->pad18[4] = 2;
        screenState = gCurrentGameTask->screenState;
    }
    if ((forceState == screenState) && (arg0->pad18[4] < 5)) {
        arg0->pad18[4] = 5;
    }

    switch (arg0->pad18[4]) {
        case 0:
            arg0->x += 0x20;
            if (arg0->x >= -0x88) {
                arg0->x = -0x88;
                arg0->pad18[4] = 1;
                gActiveMenuTask = createCallbackTask((CallbackTaskCallback)initCourseSelectCourseCursors, 0, 0x64);
                createCallbackTask((CallbackTaskCallback)initCourseSelectExtraCourseBadge, 0, 0x63);
                createCallbackTask((CallbackTaskCallback)initCourseSelectCourseDescription, 0, 0x61);
                createCallbackTask((CallbackTaskCallback)initCourseSelectCourseStats, 0, 0x64);
                if ((gRaceSplitscreenMode == 3) && (gCourseSelectModeSelection == 0)) {
                    createCallbackTask((CallbackTaskCallback)initShopMenuMoneyPanelForCourseSelectReturn, 0, 0x63);
                }
            }
            break;
        case 1:
            if (gRacePlayers[0].menuState == 3) {
                arg0->pad18[4] = 2;
            }
            break;
        case 2:
            arg0->x -= 0x20;
            if (arg0->x < -0x10D) {
                if (gCurrentGameTask->screenState != 0) {
                    arg0->pad18[4] = 4;
                    gCurrentGameTask->screenState = 2;
                } else {
                    arg0->pad18[4] = 3;
                }
            }
            break;
        case 4:
            if (gCurrentGameTask->screenState == 9) {
                arg0->pad18[4] = 3;
            }
            break;
        case 5:
            arg0->x += 0x20;
            if (arg0->x >= -0x88) {
                arg0->x = -0x88;
                arg0->pad18[4] = 6;
            }
            break;
        case 6:
            gCurrentGameTask->screenState = 4;
            arg0->pad18[4] = 7;
            break;
        case 7:
            gCurrentGameTask->screenState = 5;
            arg0->pad18[4] = 1;
            break;
    }

    if (arg0->pad18[4] == 3) {
        removeCallbackTask(arg0);
        finishCourseSelectUiTask(4);
        return;
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseSelectCourseListBackdrop, arg0);
}

void initCourseSelectCourseListBackdrop(CourseSelectWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = 8;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseSelectCourseListBackdrop);
}

#ifdef PREVIOUS_NON_MATCHING
extern u8 D_800E0DA0[];
extern u8 D_800E0DA4[];
extern u8 D_800E0DA8[];
extern char D_800E0DAC[];
extern char D_800E0DB0[];
extern char D_800E0DB4[];

void drawCourseSelectCourseStats(CourseSelectWidgetActor *arg0) {
    s32 i;
    register s32 j;
    s32 k;
    s32 new_var;
    u16 panelTile;
    u8 text[2];

    do {
        if ((s32)gPlayerCount < 3) {
            panelTile = 0x10;
        } else {
            panelTile = 0x11;
        }
        i = 0;
        do {
            new_var = 0;
            if ((s32)gPlayerCount > 0) {
                do {
                    if ((((s32)gPlayerCount >= 2) || (gRacePlayers[i].menuState == 0)) &&
                        (((s32)gPlayerCount >= 2) ||
                         (((s32)gPlayerCount == 1) && (gCourseSelectExtraCourseColumnState == 0)))) {
                        u8 selectedCourse;
                        u16 selectedCourseId;
                        s32 panelHandle;

                        selectedCourse = gRacePlayers[i].menuSelection;
                        panelHandle = gAssetHandles[0x21];
                        if (((s32)selectedCourse >= 9) && ((s32)selectedCourse < 12)) {
                            selectedCourseId = selectedCourse;
                        } else {
                            selectedCourseId = (s32)selectedCourse % 3;
                        }
                        drawMenuSpriteWithAlpha(
                            arg0->courseStatsX[i],
                            arg0->courseStatsY[i],
                            getRelocatableHeapBlockBase(panelHandle),
                            panelTile,
                            0x20,
                            0x20,
                            0,
                            arg0->courseStatsAlpha,
                            0
                        );
                        if ((s32)gPlayerCount < 3) {
                            if (selectedCourseId >= 9) {
                                drawMenuAsciiText(
                                    (s16)(arg0->courseStatsX[i] + 0x38),
                                    arg0->courseStatsY[i],
                                    D_800E0DA0,
                                    0,
                                    arg0->courseStatsAlpha
                                );
                            } else {
                                j = 0;
                                if (gCourseSelectCourseRatings.courses[selectedCourseId].speed > 0) {
                                    k = 0;
                                    do {
                                        drawMenuSpriteWithAlpha(
                                            (s16)((arg0->courseStatsX[i] + k) + 0x38),
                                            (s16)(arg0->courseStatsY[i] - 2),
                                            getRelocatableHeapBlockBase(gAssetHandles[0x24]),
                                            0x25,
                                            0x20,
                                            0x20,
                                            0,
                                            arg0->courseStatsAlpha,
                                            0
                                        );
                                        j++;
                                        k += 0xC;
                                    } while (j < gCourseSelectCourseRatings.courses[selectedCourseId].speed);
                                }
                            }
                            if (selectedCourseId >= 9) {
                                drawMenuAsciiText(
                                    (s16)(arg0->courseStatsX[i] + 0x38),
                                    (s16)(arg0->courseStatsY[i] + 0xC),
                                    D_800E0DA4,
                                    0,
                                    arg0->courseStatsAlpha
                                );
                            } else {
                                j = 0;
                                if (gCourseSelectCourseRatings.courses[selectedCourseId].handling > 0) {
                                    k = 0;
                                    do {
                                        drawMenuSpriteWithAlpha(
                                            (s16)((arg0->courseStatsX[i] + k) + 0x38),
                                            (s16)(arg0->courseStatsY[i] + 0xA),
                                            getRelocatableHeapBlockBase(gAssetHandles[0x24]),
                                            0x25,
                                            0x20,
                                            0x20,
                                            0,
                                            arg0->courseStatsAlpha,
                                            0
                                        );
                                        j++;
                                        k += 0xC;
                                    } while (j < gCourseSelectCourseRatings.courses[selectedCourseId].handling);
                                }
                            }
                            if (selectedCourseId >= 9) {
                                drawMenuAsciiText(
                                    (s16)(arg0->courseStatsX[i] + 0x38),
                                    (s16)(arg0->courseStatsY[i] + 0x18),
                                    D_800E0DA8,
                                    0,
                                    arg0->courseStatsAlpha
                                );
                            } else {
                                j = 0;
                                if (gCourseSelectCourseRatings.courses[selectedCourseId].trick > 0) {
                                    k = 0;
                                    do {
                                        drawMenuSpriteWithAlpha(
                                            (s16)((arg0->courseStatsX[i] + k) + 0x38),
                                            (s16)(arg0->courseStatsY[i] + 0x16),
                                            getRelocatableHeapBlockBase(gAssetHandles[0x24]),
                                            0x25,
                                            0x20,
                                            0x20,
                                            0,
                                            arg0->courseStatsAlpha,
                                            0
                                        );
                                        j++;
                                        k += 0xC;
                                    } while (j < gCourseSelectCourseRatings.courses[selectedCourseId].trick);
                                }
                            }
                        } else {
                            if (selectedCourseId >= 9) {
                                do {
                                    text[0] = '?';
                                } while (0);
                                text[1] = '\0';
                            }
                            if (selectedCourseId < 9) {
                                sprintf(
                                    (char *)text,
                                    D_800E0DAC,
                                    gCourseSelectCourseRatings.courses[selectedCourseId].speed
                                );
                            }
                            drawMenuAsciiText(
                                (s16)(arg0->courseStatsX[i] + 0x34),
                                arg0->courseStatsY[i],
                                text,
                                0,
                                arg0->courseStatsAlpha
                            );
                            if (selectedCourseId < 9) {
                                sprintf(
                                    (char *)text,
                                    D_800E0DB0,
                                    gCourseSelectCourseRatings.courses[selectedCourseId].handling
                                );
                            }
                            drawMenuAsciiText(
                                (s16)(arg0->courseStatsX[i] + 0x34),
                                (s16)(arg0->courseStatsY[i] + 8),
                                text,
                                0,
                                arg0->courseStatsAlpha
                            );
                            if (selectedCourseId < 9) {
                                sprintf(
                                    (char *)text,
                                    D_800E0DB4,
                                    gCourseSelectCourseRatings.courses[selectedCourseId].trick
                                );
                            }
                            drawMenuAsciiText(
                                (s16)(arg0->courseStatsX[i] + 0x34),
                                (s16)((unsigned short)(arg0->courseStatsY[i] + 0x10)),
                                text,
                                0,
                                arg0->courseStatsAlpha
                            );
                        }
                        if (((s32)gPlayerCount) >= 2) {
                            if (((s32)gPlayerCount) == 2) {
                                j = 0;
                            } else {
                                j = 1;
                            }
                            drawMenuSpriteWithAlpha(
                                (s16)(gCourseSelectStatsPlayerMarkerLayout[j][i & 1][0] + ((i >= 2) * 0x8C)),
                                gCourseSelectStatsPlayerMarkerLayout[j][i & 1][1],
                                getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                                0x12,
                                0x20,
                                0x20,
                                new_var,
                                arg0->courseStatsAlpha,
                                0
                            );
                        }
                    }
                    i++;
                } while (i < ((s32)gPlayerCount));
            }
        } while (0);
    } while (0);
}
#endif

const char D_800E0DA0[] = "?";
const char D_800E0DA4[] = "?";
const char D_800E0DA8[] = "?";
const char D_800E0DAC[] = "%d";
const char D_800E0DB0[] = "%d";
const char D_800E0DB4[] = "%d";

void drawCourseSelectCourseStats(CourseSelectWidgetActor *arg0) {
    s32 i;
    s32 j;
    s32 xOffset;
    u16 panelTile;
    u16 courseIndex;
    u8 text[2];

    if (gPlayerCount < 3) {
        panelTile = 0x10;
    } else {
        panelTile = 0x11;
    }

    for (i = 0; i < (s32)gPlayerCount; i++) {
        if (((gPlayerCount >= 2) || (gRacePlayers[i].menuState == 0)) &&
            ((gPlayerCount >= 2) || ((gPlayerCount == 1) && (gCourseSelectExtraCourseColumnState == 0)))) {
            if ((gRacePlayers[i].menuSelection >= 9) && (gRacePlayers[i].menuSelection < 12)) {
                courseIndex = gRacePlayers[i].menuSelection;
            } else {
                courseIndex = gRacePlayers[i].menuSelection % 3;
            }

            drawMenuSpriteWithAlpha(
                arg0->courseStatsX[i],
                arg0->courseStatsY[i],
                getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                panelTile,
                0x20,
                0x20,
                0,
                arg0->courseStatsAlpha,
                0
            );

            if (gPlayerCount < 3) {
                if (courseIndex >= 9) {
                    if ((D_800E0DA8 && D_800E0DA8) && D_800E0DA8) {}
                    drawMenuAsciiText(
                        (s16)(arg0->courseStatsX[i] + 0x38),
                        arg0->courseStatsY[i],
                        (u8 *)D_800E0DA0,
                        0,
                        arg0->courseStatsAlpha
                    );
                } else {
                    j = 0;
                    if (j <
                        gCourseSelectCourseRatings.values[(courseIndex * COURSE_RATING_COUNT) + COURSE_RATING_SPEED]) {
                        xOffset = 0;
                        do {
                            drawMenuSpriteWithAlpha(
                                (s16)(arg0->courseStatsX[i] + xOffset + 0x38),
                                (s16)(arg0->courseStatsY[i] - 2),
                                getRelocatableHeapBlockBase(gAssetHandles[0x24]),
                                0x25,
                                0x20,
                                0x20,
                                0,
                                arg0->courseStatsAlpha,
                                0
                            );
                            j++;
                            xOffset += 0xC;
                        } while (
                            j <
                            gCourseSelectCourseRatings.values[(courseIndex * COURSE_RATING_COUNT) + COURSE_RATING_SPEED]
                        );
                    }
                }

                if (courseIndex >= 9) {
                    drawMenuAsciiText(
                        (s16)(arg0->courseStatsX[i] + 0x38),
                        (s16)(arg0->courseStatsY[i] + 0xC),
                        (u8 *)D_800E0DA4,
                        0,
                        arg0->courseStatsAlpha
                    );
                } else {
                    j = 0;
                    if (j < gCourseSelectCourseRatings
                                .values[(courseIndex * COURSE_RATING_COUNT) + COURSE_RATING_HANDLING]) {
                        xOffset = 0;
                        do {
                            drawMenuSpriteWithAlpha(
                                (s16)(arg0->courseStatsX[i] + xOffset + 0x38),
                                (s16)(arg0->courseStatsY[i] + 0xA),
                                getRelocatableHeapBlockBase(gAssetHandles[0x24]),
                                0x25,
                                0x20,
                                0x20,
                                0,
                                arg0->courseStatsAlpha,
                                0
                            );
                            j++;
                            xOffset += 0xC;
                        } while (j < gCourseSelectCourseRatings
                                         .values[(courseIndex * COURSE_RATING_COUNT) + COURSE_RATING_HANDLING]);
                    }
                }

                if (courseIndex >= 9) {
                    if (1) {
                        drawMenuAsciiText(
                            (s16)(arg0->courseStatsX[i] + 0x38),
                            (s16)(arg0->courseStatsY[i] + 0x18),
                            (u8 *)D_800E0DA8,
                            0,
                            arg0->courseStatsAlpha
                        );
                    }
                } else {
                    j = 0;
                    if (j <
                        gCourseSelectCourseRatings.values[(courseIndex * COURSE_RATING_COUNT) + COURSE_RATING_TRICK]) {
                        xOffset = 0;
                        do {
                            drawMenuSpriteWithAlpha(
                                (s16)(arg0->courseStatsX[i] + xOffset + 0x38),
                                (s16)(arg0->courseStatsY[i] + 0x16),
                                getRelocatableHeapBlockBase(gAssetHandles[0x24]),
                                0x25,
                                0x20,
                                0x20,
                                0,
                                arg0->courseStatsAlpha,
                                0
                            );
                            j++;
                            xOffset += 0xC;
                        } while (
                            j <
                            gCourseSelectCourseRatings.values[(courseIndex * COURSE_RATING_COUNT) + COURSE_RATING_TRICK]
                        );
                    }
                }
            } else {
                if (courseIndex >= 9) {
                    text[0] = '?';
                    text[1] = '\0';
                }
                if (courseIndex < 9) {
                    sprintf(
                        text,
                        D_800E0DAC,
                        gCourseSelectCourseRatings.values[(courseIndex * COURSE_RATING_COUNT) + COURSE_RATING_SPEED]
                    );
                }
                drawMenuAsciiText(
                    (s16)(arg0->courseStatsX[i] + 0x34),
                    arg0->courseStatsY[i],
                    text,
                    0,
                    arg0->courseStatsAlpha
                );

                if (courseIndex < 9) {
                    sprintf(
                        text,
                        D_800E0DB0,
                        gCourseSelectCourseRatings.values[(courseIndex * COURSE_RATING_COUNT) + COURSE_RATING_HANDLING]
                    );
                }
                drawMenuAsciiText(
                    (s16)(arg0->courseStatsX[i] + 0x34),
                    (s16)(arg0->courseStatsY[i] + 8),
                    text,
                    0,
                    arg0->courseStatsAlpha
                );

                if (courseIndex < 9) {
                    sprintf(
                        text,
                        D_800E0DB4,
                        gCourseSelectCourseRatings.values[(courseIndex * COURSE_RATING_COUNT) + COURSE_RATING_TRICK]
                    );
                }
                drawMenuAsciiText(
                    (s16)(arg0->courseStatsX[i] + 0x34),
                    (s16)(arg0->courseStatsY[i] + 0x10),
                    text,
                    0,
                    arg0->courseStatsAlpha
                );
            }

            if (gPlayerCount >= 2) {
                if (gPlayerCount == 2) {
                    j = 0;
                } else {
                    j = 1;
                }
                drawMenuSpriteWithAlpha(
                    (s16)(gCourseSelectStatsPlayerMarkerLayout[j][(i & 1) * 2] + ((i >= 2) * 0x8C)),
                    gCourseSelectStatsPlayerMarkerLayout[j][((i & 1) * 2) + 1],
                    getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                    0x12,
                    0x20,
                    0x20,
                    0,
                    arg0->courseStatsAlpha,
                    0
                );
            }
        }
    }
}

void updateCourseSelectCourseStats(CourseSelectWidgetActor *arg0) {
    s32 state = arg0->transitionState;

    switch (state ^ 0) {
        case 0:
            arg0->transitionOffset += 0x26;
            if (arg0->transitionOffset >= 0x100) {
                arg0->transitionOffset = 0x100;
                arg0->transitionState = 1;
            }
            state = arg0->transitionState;
            break;
        case 1:
            if (gRacePlayers[0].menuState == 3) {
                arg0->transitionState = 2;
            }
            if (gCurrentGameTask->screenState == 9) {
                arg0->transitionState = 4;
            }
            state = arg0->transitionState;
            break;
        case 2:
            arg0->transitionOffset -= 0x40;
            if (arg0->transitionOffset <= 0) {
                arg0->transitionOffset = 0;
                arg0->transitionState = 3;
            }
            state = arg0->transitionState;
            break;
        case 3:
            state = (arg0->transitionState = 4);
            break;
    }

    if (state == 4) {
        removeCallbackTask(arg0);
        finishCourseSelectUiTask(5);
        return;
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseSelectCourseStats, arg0);
}

void initCourseSelectCourseStats(CourseSelectWidgetActor *arg0) {
    s32 row;
    s32 i;
    CourseSelectWidgetActor *temp_a3;
    s16(*table)[4];

    temp_a3 = arg0;
    if (gPlayerCount < 3) {
        row = gPlayerCount - 1;
    } else {
        row = 2;
    }

    i = 0;
    if ((s32)gPlayerCount > 0) {
        table = &gCourseSelectStatsPanelLayout[row];
        do {
            temp_a3->coordinates[i] = (*table)[(i & 1) * 2] + ((i >= 2) * 0x8C);
            temp_a3->coordinates[i + 4] = (*table)[(i & 1) * 2 + 1];
            i++;
        } while (i < (s32)gPlayerCount);
    }

    temp_a3->transitionOffset = 0;
    temp_a3->transitionState = 0;
    setCallbackTaskCallback(temp_a3, (CallbackTaskCallback)updateCourseSelectCourseStats);
}

void drawCourseSelectCourseDescription(CourseSelectWidgetActor *arg0) {
    s32 i;
    MenuGlyphScript *text;
    s32 pad;
    MenuGlyphScript script[8];
    u16 descriptionIndex;
    s32 pricedCourseId;
    u32 price;
    s32 digitCount;
    MenuGlyphScript *digit;
    u16 selectedIndex;

    if (COURSE_SELECT_STATUS_LAYOUT.purchaseMessageStateUnsigned == 0) {
        if ((gCourseSelectPurchaseFlowActive == 0) &&
            (((gRacePlayers[0].menuState == 0) || (gRacePlayers[0].menuState == 3)) ||
             (gRacePlayers[0].menuState == 9))) {
            if (COURSE_SELECT_STATUS_LAYOUT.extraCourseColumnState == 1) {
                descriptionIndex = 3;
            } else if (COURSE_SELECT_STATUS_LAYOUT.extraCourseColumnState == 2) {
                descriptionIndex = 4;
            } else if ((gRacePlayers[0].menuSelection >= 9) && (gRacePlayers[0].menuSelection < 12)) {
                descriptionIndex = 5;
            } else {
                descriptionIndex = gRacePlayers[0].menuSelection % 3;
            }
            text = gCourseSelectModeDescriptionText[descriptionIndex].text;
        } else {
            if ((gMenuChoicePromptState[0] < 2) || (gMenuChoicePromptState[0] == 9)) {
                selectedIndex = 1;
            } else if (gMenuChoicePromptState[0] < 5) {
                selectedIndex = gMenuChoicePromptState[0] - 1;
            }
            if ((gMenuChoicePromptState[0] >= 5) && (gMenuChoicePromptState[0] != 9)) {
                arg0->subState = 1;
            }
            if (arg0->subState == 0) {
                arg0->timer = selectedIndex;
            } else {
                selectedIndex = arg0->timer;
            }
            if ((gRaceSplitscreenMode == 3) && (gRacePlayers[0].menuSelection < 9)) {
                text = gCourseSelectBoardLevelByCourseText[gRacePlayers[0].menuSelection % 3].text;
            } else {
                if ((gRacePlayers[0].menuSelection >= 9) && (gRacePlayers[0].menuSelection < 12)) {
                    i = gCourseSelectCourseIds[4][selectedIndex - 1] % 3;
                    text = gCourseSelectExtraCourseBoardLevelText[i].text;
                } else {
                    text = gCourseSelectBoardLevelText;
                }
            }
        }
        drawMenuGlyphScript(arg0->x, arg0->y, text, 1, arg0->spriteIndex, 0);
        if ((gRaceSplitscreenMode == 3) && ((gRacePlayers[0].menuState == 1) || (gRacePlayers[0].menuState == 2))) {
            if ((gCharacterSelectHighlightedRosterIndices[0] != 3) ||
                ((gGameSaveDataBuffer[0].extraCourseUnlockFlags & 7) == 0)) {
                script[0] = 0xFFFC;
                script[1] = 6;
                script[2] = selectedIndex;
                script[3] = 0xFFFF;
                drawMenuGlyphScript(arg0->x + 0x48, arg0->y + 0x10, script, 1, arg0->spriteIndex, 0);
            }
            if (gCourseSelectModeSelection == 0) {
                script[0] = 0xFFFC;
                script[1] = 6;
                if ((selectedIndex >= 2) || (gRacePlayers[0].menuSelection >= 9)) {
                    if (gRacePlayers[0].menuSelection >= 9) {
                        i = gCourseSelectCourseIds[4][selectedIndex - 1];
                    } else {
                        i = ((gRacePlayers[0].menuSelection % 3) + (selectedIndex * 3)) - 3;
                    }
                    price = gCourseUnlockPrices[i];
                    if (price < 10000) {
                        digitCount = 5;
                    } else if (price < 100000) {
                        digitCount = 6;
                    } else {
                        digitCount = 7;
                    }
                    i = 0;
                    while (price != 0) {
                        script[digitCount - i] = price % 10;
                        price /= 10;
                        i++;
                    }
                    script[digitCount + 1] = 0x10;
                    script[digitCount + 2] = 0xFFFF;
                    if (gRacePlayers[0].menuSelection >= 9) {}
                } else {
                    for (i = 2; i < 7; i++) { script[i] = 0x2B; }
                    script[7] = 0xFFFF;
                }
                drawMenuGlyphScript(arg0->x + 0x20, arg0->y + 0x20, script, 1, arg0->spriteIndex, 0);
            }
        }
    } else {
        text = gCourseSelectPurchaseMessageText[COURSE_SELECT_STATUS_LAYOUT.purchaseMessageStateUnsigned - 1].text;
        drawMenuGlyphScript(
            arg0->x,
            arg0->y,
            text,
            1,
            arg0->spriteIndex,
            0
        );
    }
}


void updateCourseSelectCourseDescription(CourseSelectWidgetActor *arg0) {
    s32 screenState;
    s32 temp_a0;

    temp_a0 = 3;
    screenState = gCurrentGameTask->screenState;
    if (screenState == 1) {
        arg0->pad18_2[6] = 2;
        screenState = gCurrentGameTask->screenState;
    }
    if ((temp_a0 == screenState) && (arg0->pad18_2[6] < 5)) {
        arg0->pad18_2[6] = 5;
    }

    switch (arg0->pad18_2[6]) {
        case 0:
            arg0->coordinates[2] += 0x26;
            if (arg0->coordinates[2] >= 0x100) {
                arg0->coordinates[2] = 0x100;
                arg0->pad18_2[6] = 1;
            }
            break;
        case 1:
            if (gRacePlayers[0].menuState == temp_a0) {
                arg0->pad18_2[6] = 2;
            }
            break;
        case 2:
            arg0->coordinates[0] -= 0x20;
            if (arg0->coordinates[0] < -0xFF) {
                if (gCurrentGameTask->screenState != 0) {
                    arg0->pad18_2[6] = 4;
                } else {
                    arg0->pad18_2[6] = 3;
                }
            }
            break;
        case 4:
            if (gCurrentGameTask->screenState == 9) {
                arg0->pad18_2[6] = 3;
            }
            break;
        case 5:
            arg0->coordinates[0] += 0x20;
            if (arg0->coordinates[0] >= -0x84) {
                arg0->coordinates[0] = -0x84;
                arg0->pad18_2[6] = 6;
            }
            break;
        case 6:
            if (gCurrentGameTask->screenState == 4) {
                arg0->pad18_2[6] = 1;
            }
            break;
    }

    if (gRacePlayers[0].menuState == 0) {
        arg0->pad18_2[8] = 0;
    }

    if (arg0->pad18_2[6] == temp_a0) {
        removeCallbackTask(arg0);
        finishCourseSelectUiTask(6);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseSelectCourseDescription, arg0);
}

void initCourseSelectCourseDescription(CourseSelectWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = 0xC;
    arg0->spriteIndex = 0;
    arg0->state = 0;
    arg0->subState = 0;
    arg0->timer = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseSelectCourseDescription);
}

void drawCourseSelectExtraCourseBadge(CourseSelectWidgetActor *arg0) {
    u16 renderTileIndex;
    s32 handleIndex;
    s16 assetHandle;
    u16 tileIndex;

    if (gCourseSelectModeSelection == 1) {
        tileIndex = 1;
        handleIndex = 0x27;
    } else {
        tileIndex = 0;
        handleIndex = 0x21;
    }

    renderTileIndex = tileIndex;
    assetHandle = gAssetHandles[handleIndex];
    drawMenuSpriteWithAlpha(
        arg0->coordinates[0],
        arg0->coordinates[1],
        getRelocatableHeapBlockBase(assetHandle),
        renderTileIndex,
        0x20,
        0x20,
        0,
        arg0->coordinates[2],
        0
    );
}

void updateCourseSelectExtraCourseBadge(CourseSelectWidgetActor *arg0) {
    u8 state = arg0->state;

    switch (state) {
        case 0:
            arg0->spriteIndex += 0x26;
            if (arg0->spriteIndex >= 0x100) {
                arg0->spriteIndex = 0x100;
                arg0->state = 1;
            }
            state = arg0->state;
            break;
        case 1:
            if ((gRacePlayers[0].menuState == 3) || (gCurrentGameTask->screenState == 9)) {
                state = (arg0->state = 2);
            }
            break;
        case 2:
            arg0->x += 0x20;
            break;
    }
    state = arg0->state;
    if ((state == 2) && (arg0->x >= 0x94)) {
        removeCallbackTask(arg0);
        finishCourseSelectUiTask(7);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseSelectExtraCourseBadge, arg0);
}

void initCourseSelectExtraCourseBadge(CourseSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x5C;
    arg0->spriteIndex = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseSelectExtraCourseBadge);
}

void drawCourseSelectExtraCourseIconList(CourseSelectExtraCourseIconListActor *arg0) {
    s32 overlayTile;
    s32 iconAlpha;
    s32 playerIndex;
    s32 iconIndex;

    for (playerIndex = 0; playerIndex < gPlayerCount; playerIndex++) {
        if (gMenuChoicePromptState[playerIndex] != 0) {
            if (gMenuChoicePromptState[playerIndex] != 9) {
                overlayTile = (playerIndex >= 2) * 0x8C;
                for (iconIndex = arg0->itemCounts[playerIndex] - 1; iconIndex >= 0; iconIndex--) {
                    if (gMenuChoicePromptState[playerIndex] == 1) {
                        iconAlpha = 0x100;
                    } else if ((iconIndex + 2) == gMenuChoicePromptState[playerIndex]) {
                        iconAlpha = 0x100;
                    } else {
                        iconAlpha = 0x60;
                    }
                    drawMenuSpriteWithAlphaClipped(
                        arg0->iconX[playerIndex][iconIndex],
                        arg0->iconY[playerIndex][iconIndex],
                        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                        arg0->tileIndices[playerIndex][iconIndex],
                        0x20,
                        0x20,
                        0,
                        iconAlpha,
                        0,
                        arg0->clipLeft - overlayTile,
                        arg0->clipTop,
                        arg0->clipRight,
                        arg0->clipBottom
                    );
                }
                if (gCourseSelectInputLocked == 0) {
                    if ((gMenuChoicePromptState[playerIndex] >= 2) && (gMenuChoicePromptState[playerIndex] < 5)) {
                        if (gPlayerCount == 1) {
                            overlayTile = 0xF;
                        } else {
                            overlayTile = 0x1B;
                        }
                        drawMenuSpriteWithAlpha(
                            arg0->iconX[playerIndex][0],
                            (s16)(arg0->iconY[playerIndex][0] +
                                  ((gMenuChoicePromptState[playerIndex] - 2) * arg0->rowSpacing[playerIndex])),
                            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                            overlayTile,
                            0x20,
                            0x20,
                            0,
                            ((CourseSelectExtraCourseIconListActor *)&arg0->bytes[playerIndex * 2])->alpha[0],
                            0
                        );
                    }
                }
            }
        }
    }
}

void updateCourseSelectExtraCourseIconListIn(CourseSelectWidgetActor *arg0) {
    s32 var_v1;
    CourseSelectWidgetActor *var_v0;
    CourseSelectWidgetActor *temp_a2 = arg0;
    CourseSelectWidgetActor *temp_a1;

    temp_a1 = arg0;
    if (arg0->x < -0x74) {
        var_v1 = 0;
        if ((s32)arg0->itemCount > 0) {
            var_v0 = arg0;
            do {
                var_v0->x += 0x20;
                if (var_v0->x >= -0x74) {
                    var_v0->x = -0x74;
                }
                var_v1++;
                var_v0 = (CourseSelectWidgetActor *)((u8 *)var_v0 + sizeof(s16));
            } while (var_v1 < (s32)temp_a1->itemCount);
        }
    } else if (gCurrentGameTask->screenState == 4) {
        setCallbackTaskCallback(temp_a2, (CallbackTaskCallback)updateCourseSelectExtraCourseIconList);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseSelectExtraCourseIconList, temp_a2);
}

void updateCourseSelectExtraCourseIconListOut(CourseSelectWidgetActor *arg0) {
    s16 var_v0;
    s32 var_v1;
    CourseSelectWidgetActor *var_v0_2;
    s32 var_v0_3;

    var_v0 = arg0->x;
    if (var_v0 >= -0xE7) {
        var_v1 = 0;
        if ((s32)arg0->itemCount > 0) {
            var_v0_2 = arg0;
            do {
                var_v0_2->x -= 0x20;
                var_v1++;
                var_v0_2 = (CourseSelectWidgetActor *)((u8 *)var_v0_2 + sizeof(s16));
            } while (var_v1 < (s32)arg0->itemCount);
            var_v0 = arg0->x;
        }
        if (var_v0 < -0xE7) {
            arg0->x = -0xE8;
        }
        var_v0_3 = gCurrentGameTask->screenState;
    } else {
        var_v0_3 = gCurrentGameTask->screenState;
        if (var_v0_3 == 3) {
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseSelectExtraCourseIconListIn);
            var_v0_3 = gCurrentGameTask->screenState;
        }
    }
    if (var_v0_3 == 9) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseSelectExtraCourseIconList, arg0);
}

void updateCourseSelectExtraCourseIconListClose(CourseSelectWidgetActor *arg0) {
    s32 i;
    s32 j;
    CourseSelectWidgetActor *temp_a2 = arg0;

    i = 0;
    if ((s32)gPlayerCount > 0) {
        do {
            j = 0;
            if ((s32)arg0->itemCounts[i] > 0) {
                do {
                    temp_a2->coordinateRows[i][j] -= 0x20;
                    j++;
                } while (j < (s32)arg0->itemCounts[i]);
            }
            i++;
        } while (i < (s32)gPlayerCount);
    }

    if ((gPlayerCount == 1) && (temp_a2->itemCount == 0)) {
        removeCallbackTask(temp_a2);
        return;
    }

    if (temp_a2->x < -0xDF) {
        removeCallbackTask(temp_a2);
        finishCourseSelectUiTask(8);
        D_8010ADE8 = 0;
        return;
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseSelectExtraCourseIconList, temp_a2);
}

// Matched by queueram (decomp.me scratch QW2Pl).
void updateCourseSelectExtraCourseIconList(CourseSelectWidgetActor *arg0) {
    CourseSelectExtraCourseIconListActor *actor;
    s16 *promptState;
    s32 playerIndex;
    s32 iconIndex;
    s32 layoutIndex;
    s32 iconMask;
    s32 newCount;
    s16 direction;
    s16 state;
    u8 splitscreenMode;
    u8 count;

    actor = (CourseSelectExtraCourseIconListActor *)arg0;
    if (gRacePlayers[0].menuState == 3) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseSelectExtraCourseIconListClose);
        for (playerIndex = 0; playerIndex < gPlayerCount; playerIndex++) {
            actor->alpha[playerIndex] = 0x100;
        }
    } else if (gCurrentGameTask->screenState == 1) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseSelectExtraCourseIconListOut);
        actor->alpha[0] = 0x100;
        actor->pulseTimer[0] = 0;
    } else {
        for (playerIndex = 0; playerIndex < gPlayerCount; playerIndex++) {
            if (gRacePlayers[playerIndex].menuState != 2) {
                promptState = &gMenuChoicePromptState[playerIndex];
                if ((*promptState == 1) || ((*promptState >= 5) && (*promptState < 9))) {
                    if ((*promptState == 1) && (actor->itemCounts[playerIndex] == 0)) {
                        iconMask = 1;
                        if (gRacePlayers[playerIndex].selectedCharacterId == 5) {
                            actor->itemCounts[playerIndex] = 1;
                            if (gPlayerCount == 1) {
                                actor->tileIndices[playerIndex][0] = 9;
                            } else {
                                actor->tileIndices[playerIndex][0] = 0x1C;
                            }
                        } else {
                            for (iconIndex = 0; iconIndex < 3; iconIndex++) {
                                if (gRacePlayers[playerIndex].menuSelection >= 9) {
                                    if (gRaceSplitscreenMode == 3) {
                                        if (gGameSaveDataBuffer[playerIndex].extraCourseUnlockFlags & iconMask) {
                                            actor->itemCounts[playerIndex]++;
                                            if (gPlayerCount == 1) {
                                                actor->tileIndices[playerIndex][actor->itemCounts[playerIndex] - 1] =
                                                    iconIndex + 0xC;
                                            } else {
                                                actor->tileIndices[playerIndex][actor->itemCounts[playerIndex] - 1] =
                                                    iconIndex + 0x1F;
                                            }
                                        }
                                        iconMask *= 2;
                                    } else if (gGameSaveDataBuffer[playerIndex].courseUnlockStates[iconIndex + 9] !=
                                               -1) {
                                        actor->itemCounts[playerIndex]++;
                                        if (gPlayerCount == 1) {
                                            actor->tileIndices[playerIndex][actor->itemCounts[playerIndex] - 1] =
                                                iconIndex + 0xC;
                                        } else {
                                            actor->tileIndices[playerIndex][actor->itemCounts[playerIndex] - 1] =
                                                iconIndex + 0x1F;
                                        }
                                    }
                                } else if (gGameSaveDataBuffer[playerIndex].courseUnlockStates
                                               [(gRacePlayers[playerIndex].menuSelection % 3) + (iconIndex * 3)] !=
                                           -1) {
                                    actor->itemCounts[playerIndex]++;
                                    if (gPlayerCount == 1) {
                                        actor->tileIndices[playerIndex][iconIndex] = iconIndex + 9;
                                    } else {
                                        actor->tileIndices[playerIndex][iconIndex] = iconIndex + 0x1C;
                                    }
                                }
                            }

                            splitscreenMode = gRaceSplitscreenMode;
                            if ((gRaceSplitscreenMode == 3) && (gCourseSelectModeSelection == 0)) {
                                if (gRacePlayers[playerIndex].menuSelection < 9) {
                                    actor->itemCounts[playerIndex]++;
                                }
                                if (actor->itemCounts[playerIndex] >= 4) {
                                    actor->itemCounts[playerIndex] = 3;
                                }
                                if (gRacePlayers[playerIndex].menuSelection < 9) {
                                    actor->tileIndices[playerIndex][actor->itemCounts[playerIndex] - 1] =
                                        actor->itemCounts[playerIndex] + 8;
                                }
                            }
                        }
                    }

                    if (*promptState == 1) {
                        direction = 1;
                    } else {
                        direction = -1;
                    }
                    for (iconIndex = 0; iconIndex < (s32)actor->itemCounts[playerIndex]; iconIndex++) {
                        if (direction == 1) {
                            if ((s32)actor->revealTimer[playerIndex] < (iconIndex * 3) + 3) {
                                actor->iconY[playerIndex][iconIndex] += (s32)actor->rowSpacing[playerIndex] / 3;
                            }
                        } else if ((iconIndex * 2) + 2 >= (s32)actor->revealTimer[playerIndex]) {
                            actor->iconY[playerIndex][iconIndex] -= (s32)actor->rowSpacing[playerIndex] / 2;
                        }
                    }

                    if (direction == 1) {
                        actor->revealTimer[playerIndex]++;
                    } else {
                        actor->revealTimer[playerIndex]--;
                    }
                    if (direction == 1) {
                        if (actor->revealTimer[playerIndex] == (actor->itemCounts[playerIndex] * 3)) {
                            *promptState = gCourseSelectSelectedRows[playerIndex] + 2;
                        }
                    } else if (actor->revealTimer[playerIndex] == 0) {
                        *promptState = 0;
                        actor->itemCounts[playerIndex] = 0;
                        if ((s32)gPlayerCount < 3) {
                            layoutIndex = gPlayerCount - 1;
                        } else {
                            layoutIndex = 2;
                        }
                        for (iconIndex = 0; iconIndex < 3; iconIndex++) {
                            actor->iconY[playerIndex][iconIndex] =
                                gCourseSelectIconListYLayout[layoutIndex][((playerIndex & 1) * 2) + 2];
                        }
                    }
                }

                state = *promptState;
                if ((state >= 2) && (state < 5)) {
                    if ((s32)actor->pulseTimer[playerIndex] < 0x10) {
                        actor->alpha[playerIndex] -= 9;
                    } else {
                        actor->alpha[playerIndex] += 9;
                    }
                    actor->pulseTimer[playerIndex] = (actor->pulseTimer[playerIndex] + 1) & 0x1F;
                }
            }

            gCourseSelectStatus.extraCourseCounts[playerIndex] = actor->itemCounts[playerIndex];
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseSelectExtraCourseIconList, arg0);
}

#if 0
void updateCourseSelectExtraCourseIconList(CourseSelectWidgetActor *arg0) {
    CourseSelectExtraCourseIconListActor *actor;
    s32 unlockMask;
    RacePlayer *player;
    s16 *promptState;
    s32 playerIndex;
    s32 iconIndex;
    s16 direction;
    s32 layoutIndex;
    u8 itemCountByte;
    s32 itemCount;
    s32 nextItemCount;

    actor = (CourseSelectExtraCourseIconListActor *)arg0;
    if (gRacePlayers[0].menuState == 3) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseSelectExtraCourseIconListClose);
        playerIndex = 0;
        if ((s32)gPlayerCount > 0) {
            do {
                ((CourseSelectExtraCourseIconListActor *)arg0)->alpha[playerIndex] = 0x100;
                playerIndex++;
            } while (playerIndex < (s32)gPlayerCount);
        }
    } else if (gCurrentGameTask->screenState == 1) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseSelectExtraCourseIconListOut);
        ((CourseSelectExtraCourseIconListActor *)arg0)->alpha[0] = 0x100;
        actor->pulseTimer[0] = 0;
    } else {
        playerIndex = 0;
        if ((s32)gPlayerCount > 0) {
            player = gRacePlayers; do {
                if (player->menuState != 2) {
                    promptState = &gMenuChoicePromptState[playerIndex];
                    if ((*promptState == 1) || ((*promptState >= 5) && (*promptState < 9))) {
                        if ((*promptState == 1) && (actor->itemCounts[playerIndex] == 0)) {
                            unlockMask = 1;
                            if (player->selectedCharacterId == 5) {
                                actor->itemCounts[playerIndex] = 1;
                                if (gPlayerCount == 1) {
                                    actor->tileIndices[playerIndex][0] = 9;
                                } else {
                                    actor->tileIndices[playerIndex][0] = 0x1C;
                                }
                            } else {
                                iconIndex = 0;
                                do {
                                    if ((s32)player->courseIndex >= 9) {
                                        if (gRaceSplitscreenMode == 3) {
                                            if (gGameSaveDataBuffer[playerIndex].extraCourseUnlockFlags & unlockMask) {
                                                itemCount = actor->itemCounts[playerIndex] + 1;
                                                actor->itemCounts[playerIndex] = itemCount;
                                                if (gPlayerCount == 1) {
                                                    actor->tileIndices[playerIndex][(u8)itemCount - 1] = iconIndex + 0xC;
                                                } else {
                                                    actor->tileIndices[playerIndex][actor->itemCounts[playerIndex] - 1] =
                                                        iconIndex + 0x1F;
                                                }
                                            }
                                            unlockMask *= 2;
                                        } else {
                                            if (!gRaceSplitscreenMode) {
                                                /* Empty block preserves IDO's register allocation. */
                                            }
                                            if (gGameSaveDataBuffer[playerIndex].courseUnlockStates[iconIndex + 9] != -1) {
                                                itemCount = actor->itemCounts[playerIndex] + 1;
                                                actor->itemCounts[playerIndex] = itemCount;
                                                if (gPlayerCount == 1) {
                                                    actor->tileIndices[playerIndex][(((itemCount & 0xFF) & 0xFF) & 0xFF) - 1] = iconIndex + 0xC;
                                                } else {
                                                    itemCountByte = actor->itemCounts[playerIndex];
                                                    actor->tileIndices[playerIndex][itemCountByte - 1] = iconIndex + 0x1F;
                                                }
                                            }
                                        }
                                    } else if (gGameSaveDataBuffer[playerIndex]
                                                   .courseUnlockStates[(player->courseIndex % 3) + (iconIndex * 3)] != -1) {
                                        actor->itemCounts[playerIndex]++;
                                        if (gPlayerCount == 1) {
                                            actor->tileIndices[playerIndex][iconIndex] = iconIndex + 9;
                                        } else {
                                            actor->tileIndices[playerIndex][iconIndex] = iconIndex + 0x1C;
                                        }
                                    }
                                    iconIndex++;
                                } while (iconIndex != 3);

                                if ((gRaceSplitscreenMode == 3) && (gCourseSelectModeSelection == 0)) {
                                    itemCount = actor->itemCounts[playerIndex];
                                    nextItemCount = itemCount + 1;
                                    if ((s32)player->courseIndex < 9) {
                                        itemCount = (actor->itemCounts[playerIndex] = nextItemCount);
                                    }
                                    if (itemCount >= 4) {
                                        actor->itemCounts[playerIndex] = 3;
                                    }
                                    if ((s32)player->courseIndex < 9) {
                                        itemCount = actor->itemCounts[playerIndex];
                                        actor->tileIndices[playerIndex][itemCount - 1] = itemCount + 8;
                                    }
                                }
                            }
                        }

                        iconIndex = 0;
                        if (*promptState == 1) {
                            direction = 1;
                        } else {
                            direction = -1;
                        }
                        if ((s32)actor->itemCounts[playerIndex] > 0) {
                            do {
                                if (direction == 1) {
                                    if ((s32)actor->revealTimer[playerIndex] < ((iconIndex * 3) + 3)) {
                                        actor->iconY[playerIndex][iconIndex] += (s32)actor->rowSpacing[playerIndex] / 3;
                                    }
                                } else if (((iconIndex * 2) + 2) >= (s32)actor->revealTimer[playerIndex]) {
                                    actor->iconY[playerIndex][iconIndex] -=
                                        (s32)((CourseSelectExtraCourseIconListActor *)arg0)->rowSpacing[playerIndex] / 2;
                                }
                                iconIndex++;
                            } while (iconIndex < (s32)actor->itemCounts[playerIndex]);
                        }
                        if (direction == 1) {
                            ((CourseSelectExtraCourseIconListActor *)arg0)->revealTimer[playerIndex]++;
                        } else {
                            ((CourseSelectExtraCourseIconListActor *)arg0)->revealTimer[playerIndex]--;
                        }
                        if (direction == 1) {
                            if (((CourseSelectExtraCourseIconListActor *)arg0)->revealTimer[playerIndex] ==
                                (((CourseSelectExtraCourseIconListActor *)arg0)->itemCounts[playerIndex] * 3)) {
                                *promptState = gCourseSelectSelectedRows[playerIndex] + 2;
                            }
                        } else if (((CourseSelectExtraCourseIconListActor *)arg0)->revealTimer[playerIndex] == 0) {
                            *promptState = 0;
                            ((CourseSelectExtraCourseIconListActor *)arg0)->itemCounts[playerIndex] = 0;
                            if ((s32)gPlayerCount < 3) {
                                layoutIndex = gPlayerCount - 1;
                            } else {
                                layoutIndex = 2;
                            }
                            iconIndex = 0;
                            do {
                                ((CourseSelectExtraCourseIconListActor *)arg0)->iconY[playerIndex][iconIndex] =
                                    gCourseSelectIconListYLayout[layoutIndex][((playerIndex & 1) * 2) + 2];
                                iconIndex++;
                            } while (iconIndex != 3);
                        }
                    }
                    if ((*promptState >= 2) && (*promptState < 5)) {
                        if ((s32)((CourseSelectExtraCourseIconListActor *)arg0)->pulseTimer[playerIndex] < 0x10) {
                            ((CourseSelectExtraCourseIconListActor *)arg0)->alpha[playerIndex] -= 9;
                        } else {
                            ((CourseSelectExtraCourseIconListActor *)arg0)->alpha[playerIndex] += 9;
                        }
                        ((CourseSelectExtraCourseIconListActor *)arg0)->pulseTimer[playerIndex] =
                            (((CourseSelectExtraCourseIconListActor *)arg0)->pulseTimer[playerIndex] + 1) & 0x1F;
                    }
                }
                player++;
                gCourseSelectStatus.extraCourseCounts[playerIndex] =
                    ((CourseSelectExtraCourseIconListActor *)arg0)->itemCounts[playerIndex];
                playerIndex++;
            } while (playerIndex < (s32)gPlayerCount);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseSelectExtraCourseIconList, arg0);
}
#endif

void initCourseSelectExtraCourseIconList(CourseSelectWidgetActor *arg0) {
    CourseSelectExtraCourseIconListActor *actor;
    s32 layoutIndex;
    s32 playerIndex;
    s32 iconIndex;
    s32 xOffset;

    actor = (CourseSelectExtraCourseIconListActor *)arg0;
    if ((s32)gPlayerCount < 3) {
        layoutIndex = gPlayerCount - 1;
    } else {
        layoutIndex = 2;
    }

    playerIndex = 0;
    if ((s32)gPlayerCount > 0) {
        do {
            if ((s32)gPlayerCount >= 3) {
                actor->rowSpacing[playerIndex] = 0xC;
            } else {
                actor->rowSpacing[playerIndex] = gCourseSelectIconListYLayout[layoutIndex][0];
            }
            actor->itemCounts[playerIndex] = 0;
            actor->alpha[playerIndex] = 0x100;
            if (gPlayerCount == 1) {
                xOffset = 8;
            } else {
                xOffset = 4;
            }
            /* Preserve IDO's register allocation for the icon-position loop. */
            if (0) {}

            iconIndex = 0;
            do {
                actor->iconX[playerIndex][iconIndex] =
                    gCourseSelectIconListXLayout[layoutIndex][((playerIndex >= 2) * 2) + 1] + xOffset;
                actor->iconY[playerIndex][iconIndex] =
                    gCourseSelectIconListYLayout[layoutIndex][((playerIndex & 1) * 2) + 2];
                iconIndex++;
            } while (iconIndex != 3);
            playerIndex++;
        } while (playerIndex < (s32)gPlayerCount);
    }

    actor->clipRight = 0xA0;
    actor->clipTop = 0x78;
    actor->clipBottom = 0x78;
    if (gPlayerCount == 1) {
        actor->clipLeft = 0xA0;
    } else if (gPlayerCount == 2) {
        actor->clipLeft = 0x7E;
    } else {
        actor->clipLeft = 0x88;
    }
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateCourseSelectExtraCourseIconList);
}

const char D_800E0DB8[] = "%d";

void drawCourseSelectPlayerPanels(CourseSelectWidgetActor *arg0) {
    s32 j;
    s32 i;
    s32 middleCount;
    u8 tile;
    s32 alpha;
    s32 count;
    u8 text[4];
    s32 offset;
    CourseSelectWidgetInitActor *actor = (CourseSelectWidgetInitActor *)arg0;

    if (gPlayerCount == 2) {
        count = 2;
    } else {
        count = 4;
    }

    for (i = 0; i < count; i++) {
        if (i == gPlayerCount) {
            alpha = 0xC0;
        } else {
            alpha = 0x100;
        }

        if (i == 0) {
            tile = 2;
        } else {
            tile = i + 5;
        }

        drawMenuSpriteWithAlphaWideArgs(
            actor->x[i],
            actor->y[i],
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            0x22,
            0x20,
            0x20,
            0,
            alpha,
            tile
        );

        if (gPlayerCount == 2) {
            middleCount = 0xB;
        } else {
            middleCount = 3;
        }

        for (j = 0, offset = 0x38; j < middleCount; j++, offset += 0x10) {
            drawMenuSpriteWithAlphaWideArgs(
                (s16)(actor->x[i] + offset),
                actor->y[i],
                getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                0x23,
                0x20,
                0x20,
                0,
                alpha,
                tile
            );
        }

        drawMenuSpriteWithAlphaWideArgs(
            (s16)(actor->x[i] + offset),
            actor->y[i],
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            0x24,
            0x20,
            0x20,
            0,
            alpha,
            tile
        );

        if (gPlayerCount == 2) {
            offset = 0xF8;
        } else {
            offset = 0x78;
        }

        for (j = 0; j < 4; j++) {
            drawMenuSpriteWithAlphaWideArgs(
                actor->x[i],
                (s16)(actor->y[i] + j * 0x10 + 0x10),
                getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                0x25,
                0x20,
                0x20,
                0,
                alpha,
                tile
            );

            drawMenuSpriteWithAlphaWideArgs(
                (s16)(actor->x[i] + offset),
                (s16)(actor->y[i] + j * 0x10 + 0x10),
                getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                0x26,
                0x20,
                0x20,
                0,
                alpha,
                tile
            );
        }

        drawMenuSpriteWithAlphaWideArgs(
            actor->x[i],
            (s16)(actor->y[i] + 0x50),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            0x27,
            0x20,
            0x20,
            0,
            alpha,
            tile
        );

        if (gPlayerCount == 2) {
            middleCount = 0xE;
        } else {
            middleCount = 6;
        }

        for (j = 0; j < middleCount; j++) {
            drawMenuSpriteWithAlphaWideArgs(
                (s16)(actor->x[i] + j * 0x10 + 0x10),
                (s16)(actor->y[i] + 0x50),
                getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                0x28,
                0x20,
                0x20,
                0,
                alpha,
                tile
            );
        }

        drawMenuSpriteWithAlphaWideArgs(
            (s16)(actor->x[i] + offset - 8),
            (s16)(actor->y[i] + 0x50),
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            0x29,
            0x20,
            0x20,
            0,
            alpha,
            tile
        );

        sprintf(text, D_800E0DB8, i + 1);
        drawMenuAsciiText((s16)(actor->x[i] + 0x34), (s16)(actor->y[i] + 2), text, 0, alpha);

        if (alpha == 0xC0) {
            drawMenuSpriteWithAlphaWideArgs(
                (s16)(actor->x[i] + 2),
                (s16)(actor->y[i] + 0x24),
                getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                0x90,
                0x20,
                0x20,
                0,
                0xF0,
                0
            );
        }
    }
}

void updateCourseSelectPlayerPanels(CourseSelectWidgetActor *arg0) {
    volatile u8 pad[8];
    s32 next;
    CourseSelectWidgetInitActor *actor;
    s32 i;
    s16 *deltaX;
    s32 count;
    s32 step;
    u16 period;
    u16 timer;
    s32 zero;
    u8 *statePtr;

    actor = (CourseSelectWidgetInitActor *)arg0;
    if (gPlayerCount == 2) {
        count = 2;
    } else {
        count = 4;
    }

    i = 0;
    if (count > 0) {
        statePtr = (u8 *)actor;
        do {
            next = i + 1;
            switch (((CourseSelectWidgetInitActor *)statePtr)->state[0]) {
                case 0:
                    step = actor->x[i] * 0;
                    period = actor->period[i];
                    do {
                        actor->x[i] += *(deltaX = &actor->deltaX[i]);
                        timer = (actor->timer[i] += actor->speed[i]);
                        if (timer >= period) {
                            actor->y[i] += actor->deltaY[i];
                            actor->timer[i] = (unsigned long)timer - period;
                        }
                        step++;
                        if (actor->x[i] == actor->targetX[i]) {
                            ((CourseSelectWidgetInitActor *)statePtr)->state[0] = 1;
                            if (count == next) {
                                D_8010ADE0 =
                                    createCallbackTask((CallbackTaskCallback)initCourseSelectPreviewModelIn, 0, 0x62);
                                D_8010ADE4 =
                                    createCallbackTask((CallbackTaskCallback)initCourseSelectPreviewModelOut, 0, 0x62);
                                createCallbackTask((CallbackTaskCallback)initCourseSelectCourseIconList, 0, 0x62);
                            }
                            break;
                        }
                    } while (step != 0x18);
                    break;
                case 1:
                    if (gRacePlayers[i].menuState == 4) {
                        ((CourseSelectWidgetInitActor *)statePtr)->state[0] = 2;
                        actor->timer[i] = 0;
                    }
                    break;
                case 2:
                    step = 0;
                    period = actor->period[i];
                    do {
                        actor->x[i] -= actor->deltaX[i];
                        timer = (actor->timer[i] += actor->speed[i]);
                        if (timer >= period) {
                            actor->y[i] -= actor->deltaY[i];
                            actor->timer[i] = timer - period;
                        }
                        step++;
                        if (count == next) {
                            zero = 0;
                            if (((gPlayerCount == 2) && (actor->x[0] >= actor->exitTargetXs[i * zero])) ||
                                ((gPlayerCount >= 3) && (actor->exitTargetXs[0] >= actor->x[zero]))) {
                                s32 j = zero;
                                if (count > zero) {
                                    do {
                                        actor->state[j] = 3;
                                        j++;
                                    } while (j != count);
                                }
                                break;
                            }
                        }
                    } while (step != 0x18);
                    break;
                case 3:
                    break;
            }
            i++;
            statePtr++;
        } while (i != count);
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseSelectPlayerPanels, arg0);
}

void initCourseSelectPlayerPanels(CourseSelectWidgetInitActor *arg0) {
    if (gPlayerCount == 2) {
        arg0->exitTargetX = 0xA0;
        arg0->x[0] = 0x90;
        arg0->y[0] = -0xC8;
        arg0->x[1] = -0x18C;
        arg0->y[1] = 0x6C;
        arg0->targetX[0] = -0x7E;
        arg0->targetX[1] = -0x7E;
        arg0->deltaX[0] = -1;
        arg0->deltaX[1] = 1;
        arg0->deltaY[0] = 1;
        arg0->deltaY[1] = -1;
        arg0->period[0] = 0x10E;
        arg0->period[1] = 0x10E;
        arg0->speed[0] = 0x68;
        arg0->speed[1] = 0x68;
    } else {
        arg0->x[0] = -0x118;
        arg0->x[1] = -0x118;
        arg0->deltaX[0] = 1;
        arg0->deltaY[0] = 1;
        arg0->deltaX[1] = 1;
        arg0->deltaY[2] = 1;
        arg0->exitTargetX = -0x128;
        arg0->y[0] = -0xC8;
        arg0->y[1] = 0x6C;
        arg0->x[2] = 0x94;
        arg0->y[2] = -0xC8;
        arg0->x[3] = 0x94;
        arg0->y[3] = 0x6C;
        arg0->targetX[0] = -0x88;
        arg0->targetX[1] = -0x88;
        arg0->targetX[2] = 4;
        arg0->targetX[3] = 4;
        arg0->deltaY[1] = -1;
        arg0->deltaX[2] = -1;
        arg0->deltaX[3] = -1;
        arg0->deltaY[3] = -1;
        arg0->period[0] = 0x90;
        arg0->speed[0] = 0x68;
        arg0->period[1] = 0x90;
        arg0->speed[1] = 0x68;
        arg0->period[2] = 0x90;
        arg0->speed[2] = 0x68;
        arg0->period[3] = 0x90;
        arg0->speed[3] = 0x68;
    }
    arg0->timer[1] = 0;
    arg0->state[1] = 0;
    arg0->timer[2] = 0;
    arg0->state[2] = 0;
    arg0->timer[3] = 0;
    arg0->state[3] = 0;
    arg0->timer[0] = 0;
    arg0->state[0] = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseSelectPlayerPanels);
}

void drawCourseSelectCompletePanels(CourseSelectPlayerPanelsActor *actor) {
    s32 i;
    s32 playerCount;
    s32 xOffset;
    s32 yOffset;
    s32 two;
    u32 rightEdgeTile;

    playerCount = gPlayerCount;
    two = 2;
    rightEdgeTile = 2;
    for (i = 0; i < playerCount; i++) {
        if (actor->playerPanelFadeAlpha[i] != 0) {
            if (two == playerCount) {
                yOffset = i * 0x64;
                drawMenuSpriteWithAlpha(
                    actor->x,
                    (s16)(actor->y + yOffset),
                    getRelocatableHeapBlockBase(gAssetHandles[0x24]),
                    0,
                    0x20,
                    0x20,
                    0,
                    actor->playerPanelFadeAlpha[i],
                    0
                );
                drawMenuSpriteWithAlpha(
                    (s16)(actor->x + 0x40),
                    (s16)(actor->y + yOffset),
                    getRelocatableHeapBlockBase(gAssetHandles[0x24]),
                    1,
                    0x20,
                    0x20,
                    0,
                    actor->playerPanelFadeAlpha[i],
                    0
                );
                drawMenuSpriteWithAlpha(
                    (s16)(actor->x + 0x78),
                    (s16)(actor->y + yOffset),
                    getRelocatableHeapBlockBase(gAssetHandles[0x24]),
                    1,
                    0x20,
                    0x20,
                    0,
                    actor->playerPanelFadeAlpha[i],
                    0
                );
                drawMenuSpriteWithAlpha(
                    (s16)(actor->x + 0xB0),
                    (s16)(actor->y + yOffset),
                    getRelocatableHeapBlockBase(gAssetHandles[0x24]),
                    rightEdgeTile,
                    0x20,
                    0x20,
                    0,
                    actor->playerPanelFadeAlpha[i],
                    0
                );
                drawMenuGlyphScript(
                    (s16)(actor->x + 0x2E),
                    (s16)(actor->y + yOffset + 0xC),
                    gCourseSelectCompleteText,
                    0,
                    actor->playerPanelFadeAlpha[i],
                    0
                );
            } else {
                xOffset = (i >= 2) * 0x8C;
                yOffset = (i & 1) * 0x64;
                drawMenuSpriteWithAlpha(
                    (s16)(actor->x + xOffset),
                    (s16)(actor->y + yOffset),
                    getRelocatableHeapBlockBase(gAssetHandles[0x24]),
                    8,
                    0x20,
                    0x20,
                    0,
                    actor->playerPanelFadeAlpha[i],
                    0
                );
                drawMenuSpriteWithAlpha(
                    (s16)(actor->x + xOffset + 0x40),
                    (s16)(actor->y + yOffset),
                    getRelocatableHeapBlockBase(gAssetHandles[0x24]),
                    9,
                    0x20,
                    0x20,
                    0,
                    actor->playerPanelFadeAlpha[i],
                    0
                );
                drawMenuGlyphScript(
                    (s16)(actor->x + xOffset + 0x24),
                    (s16)(actor->y + yOffset + 3),
                    gCourseSelectCompleteText,
                    1,
                    actor->playerPanelFadeAlpha[i],
                    0
                );
            }
        }
        playerCount = gPlayerCount;
    }
}

void updateCourseSelectCompletePanels(CourseSelectPlayerPanelsActor *actor) {
    CourseSelectCompletePanelSource *source0;
    CourseSelectCompletePanelSource *source1;
    CourseSelectPlayerPanelsActor *task;
    CourseSelectPlayerPanelsActor *panel;
    u16 alpha;
    s32 i;

    source0 = (CourseSelectCompletePanelSource *)D_8010ADE0;
    source1 = (CourseSelectCompletePanelSource *)D_8010ADE4;
    panel = actor;
    task = panel;
    for (i = 0; i < gPlayerCount; i++) {
        alpha = panel->playerPanelFadeAlpha[i];
        if (alpha == 0) {
            if ((source0 != NULL) && ((source0->playerStates[i] == 4) || (source1->playerStates[i] == 4))) {
                panel->playerPanelFadeAlpha[i] = 1;
            }
        } else {
            if (alpha != 0x100) {
                panel->playerPanelFadeAlpha[i] = alpha + 0x30;
                if (panel->playerPanelFadeAlpha[i] >= 0x100) {
                    panel->playerPanelFadeAlpha[i] = 0x100;
                }
            }
            if (gRacePlayers[i].menuState == 1) {
                panel->playerPanelFadeAlpha[i] = 0;
            }
        }
    }

    if (D_800EC9C0 == 0x10) {
        removeCallbackTask(task);
        for (i = 0; i < gPlayerCount; i++) {
            gRacePlayers[i].menuState = 3;
        }
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseSelectCompletePanels, task);
    }
}

void initCourseSelectCompletePanels(CourseSelectWidgetActor *arg0) {
    s32 var_v0;
    CourseSelectWidgetActor *var_v1;

    if (gPlayerCount == 2) {
        arg0->x = -0x6E;
        arg0->y = -0x4C;
    } else {
        arg0->x = -0x8C;
        arg0->y = -0x46;
    }
    var_v0 = 0;
    var_v1 = arg0;
    if ((s32)gPlayerCount > 0) {
        do {
            var_v1->spriteIndex = 0;
            var_v0++;
            var_v1 = (CourseSelectWidgetActor *)((u8 *)var_v1 + sizeof(s16));
        } while (var_v0 < (s32)gPlayerCount);
    }
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseSelectCompletePanels);
}

void finishCourseSelectUiTask(s32 arg0) {
}
