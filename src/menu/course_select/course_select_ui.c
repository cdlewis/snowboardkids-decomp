#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/character_select/character_select_menu.h"
#include "game/menu/course_select/course_select_ui.h"
#include "game/menu/course_select/course_select_shop_ui.h"
#include "game/math/fixed_point_math.h"
#include "game/race/player/race_player_model_renderer.h"
#include "game/race/player/race_player_input.h"

typedef struct {
    u8 pad0[0x20];
    /* 0x20 */ s32 screenState;
} CourseSelectState;

typedef struct {
    /* 0x000 */ u8 menuState;
    /* 0x001 */ u8 pad1[0x60B];
} CourseSelectPlayerMenuState;

typedef struct {
    /* 0x0 */ u8 pad0;
    /* 0x1 */ u8 courseIndex;
} CourseSelectWidgetPlayerSlot;

typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ s32 matrix;
    /* 0x01C */ FixedTransform sourceTransform;
    /* 0x03C */ FixedTransform playerTransforms[4];
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

typedef union {
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
} CourseSelectExtraCourseIconListActor;

typedef struct {
    /* 0x0000 */ u8 pad0[0x3F];
    /* 0x003F */ s8 courseUnlockStates[12];
    /* 0x004B */ u8 pad4B[0x788C];
    /* 0x78D7 */ u8 extraCourseUnlockFlags;
    /* 0x78D8 */ u8 pad78D8[0x20];
} CourseSelectExtraCourseSaveData;

typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ u8 extraCourseCounts[4];
} CourseSelectExtraCourseStatus;

struct CourseSelectAnimatedActor {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ s32 matrix;
    /* 0x01C */ FixedTransform sourceTransform;
    /* 0x03C */ FixedTransform playerTransforms[4];
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
    /* 0x00 */ u8 previewModelState[4];
    /* 0x04 */ u8 incomingPreviewModelState[4];
    /* 0x08 */ u8 outgoingPreviewModelState[4];
    /* 0x0C */ u8 incomingPreviewModelTimer[4];
    /* 0x10 */ u8 outgoingPreviewModelTimer[4];
    /* 0x14 */ u16 incomingPreviewModelAngle[4];
    union {
        /* 0x1C */ u16 outgoingPreviewModelAngle[4];
        /* 0x1C */ u8 outgoingPreviewModelAngleBytes[8];
    };
    /* 0x24 */ u8 pad24[0x14];
} CourseSelectStatusOverlay;

typedef union {
    struct {
        /* 0x00 */ u8 pad0[0x2C];
        /* 0x2C */ u16 purchaseMessage;
        /* 0x2E */ u8 descriptionMode;
    };
    u8 bytes[0x30];
} CourseSelectDescriptionStatus;

extern void drawCourseSelectPlayerPanels(CourseSelectWidgetActor *);
extern void drawMenuSpriteWithAlphaWideArgs(s32 x, s32 y, void *texture, s32 tileIndex, s32 width, s32 height,
                                            s32 palette, s32 alpha, u32 flip);
extern s32 allocFixedTransformMatrix(FixedTransform *);
extern u8 D_800E0DB8[];
extern s8 gCourseUnlockSaveSlots[][0x78F8];
extern u8 D_800EC9C0;
extern u8 gRaceSplitscreenMode;
extern u8 gCourseSelectModeSelection;
extern CallbackTask *D_8010ADE0;
extern CallbackTask *D_8010ADE4;
extern s8 D_8010AE64[];
extern u8 D_8010AECC[];
extern u16 D_8010AED0;
extern s32 D_8010AEE8[];
extern u8 D_8010AEFB[];
extern u8 D_8010AEA4[];
extern u8 D_8010AEB0;
extern u8 D_8010AEA0[];
extern u8 gCourseSelectExtraCourseColumnState;
extern CourseSelectDescriptionStatus gCourseSelectStatus[];
extern u8 D_8010AEAC[];
extern u8 D_8010AF1C;
extern s16 gMenuChoicePromptState[];
extern MenuGlyphScript gCourseSelectCompleteText[];
extern u8 gCourseSelectCourseSpeedRatings[];
extern u8 gCourseSelectCourseHandlingRatings[];
extern u8 gCourseSelectCourseTrickRatings[];
extern s16 gCourseSelectIconListYLayout[][6];
extern s16 gCourseSelectIconListXLayout[][4];
extern s16 gCourseSelectStatsPanelLayout[][4];
extern s16 gCourseSelectStatsPlayerMarkerLayout[][2][2];
extern u8 gPlayerCount;
extern s32 gMenuFlowState;
extern CourseSelectState *gCurrentGameTask;
extern u8 gCurrentViewportIndex;
extern u8 gGameSaveDataBuffer[][0x78F8];

void drawCourseSelectPreviewModel(CourseSelectCoursePreviewActor *arg0) {
    u8 sp2F;
    unsigned char sp2E;
    u8 var_t0;
    s8 temp_v0_2;
    RacePlayer *temp_v0_3;
    FixedTransform sp30;
    u8 var_a3;
    u8 var_v1;
    int temp_v0;

    if ((D_8010AED0 != 0) && (gCurrentViewportIndex == 1)) {
        var_t0 = 0;
    } else {
        var_t0 = gCurrentViewportIndex;
    }
    if ((D_8010AECC[var_t0] == 0) || (D_8010AECC[var_t0] & 1)) {
        temp_v0 = arg0->playerFlags[var_t0];
        if ((temp_v0 == 0) || (temp_v0 & 1)) {
            if (D_8010AECC[var_t0] == 1) {
                var_a3 = arg0->playerSlots[var_t0].courseIndex;
            } else {
                var_a3 = gRacePlayers[var_t0].menuSelection;
            }
            temp_v0_2 = gCourseUnlockSaveSlots[var_t0][var_a3];
            if (temp_v0_2 == -1) {
                var_v1 = 9;
            } else {
                var_v1 = temp_v0_2;
            }
            if ((D_8010AED0 != 0) && (gCurrentViewportIndex == 1)) {
                var_v1 = (u8)(D_8010AED0 - 1);
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
                drawRacePlayerModelRootPart((void *)(arg0->matrix ^ 0), (s16) sp2F, (s16) sp2E);
            }
        }
    }
}

// updateCourseSelectPreviewModelIn best match: 99.868% (nonmatchings/updateCourseSelectPreviewModelIn-1213871690025509423/base_38.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_ui/updateCourseSelectPreviewModelIn.s")

#ifdef NON_MATCHING
void updateCourseSelectPreviewModelIn(CourseSelectAnimatedActor *arg0)
{
    CourseSelectAnimatedActor *actor;
    u8 incomingState;
    s32 i;
    Vec3i rotatedPosition;
    s32 slideStep;
    s32 state;

    actor = arg0;

    for (i = 0; i < (s32)gPlayerCount; i++) {
        incomingState = ((CourseSelectStatusOverlay *)gCourseSelectStatus)->incomingPreviewModelState[i];
        if (incomingState != (state = actor->state[i])) {
            actor->state[i] = incomingState;
            actor->timer[i] =
                ((CourseSelectStatusOverlay *)gCourseSelectStatus)->incomingPreviewModelTimer[i];
            actor->angle[i] =
                ((CourseSelectStatusOverlay *)gCourseSelectStatus)->incomingPreviewModelAngle[i];
            ((CourseSelectStatusOverlay *)gCourseSelectStatus)->incomingPreviewModelTimer[i] = 0;
            ((CourseSelectStatusOverlay *)gCourseSelectStatus)->incomingPreviewModelAngle[i] = 0;
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
            if (D_8010AECC[i] & 1) {
                if (D_8010AEE8[i] < 0) {
                    if ((s32)gRacePlayers[i].courseSelectCharacterId >= 9) {
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
                    actor->targetCourse[i] = D_8010AEFB[i * 4];
                }
                if (actor->targetCourse[i] == 8) {
                    actor->targetCourse[i] = 2;
                }
                if (D_8010AECC[i] == 3) {
                    actor->vecs[i].y = -D_8010AEE8[i];
                }
                actor->state[i] = 2;
            } else if (gCurrentGameTask->screenState == 9) {
                actor->state[i] = 8;
            }
            break;
        case 2:
            slideStep = 0x200000;
            if (D_8010AEE8[i] < 0) {
                slideStep = -0x200000;
            }
            actor->vecs[i].y += slideStep;
            if ((D_8010AEE8[i] == slideStep) != 0) {
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
        ((CourseSelectStatusOverlay *)gCourseSelectStatus)->incomingPreviewModelState[i] =
            actor->state[i];
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
    FixedTransform sp30;
    u8 var_a3;
    u8 var_v1;
    int temp_v0;

    if ((D_8010AED0 != 0) && (gCurrentViewportIndex == 1)) {
        var_t0 = 0;
    } else {
        var_t0 = gCurrentViewportIndex;
    }
    if ((D_8010AECC[var_t0] == 2) || (D_8010AECC[var_t0] & 1)) {
        temp_v0 = arg0->playerFlags[var_t0];
        if ((temp_v0 == 0) || (temp_v0 & 1)) {
            if (D_8010AECC[var_t0] == 3) {
                var_a3 = arg0->playerSlots[var_t0].courseIndex;
            } else {
                var_a3 = gRacePlayers[var_t0].menuSelection;
            }
            temp_v0_2 = gCourseUnlockSaveSlots[var_t0][var_a3];
            if (temp_v0_2 == -1) {
                var_v1 = 9;
            } else {
                var_v1 = temp_v0_2;
            }
            if ((D_8010AED0 != 0) && (gCurrentViewportIndex == 1)) {
                var_v1 = (u8)(D_8010AED0 - 1);
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
                drawRacePlayerModelRootPart((void *)(arg0->matrix ^ 0), (s16) sp2F, (s16) sp2E);
            }
        }
    }
}

void updateCourseSelectPreviewModelOut(CourseSelectAnimatedActor *arg0)
{
    CourseSelectAnimatedActor *actor;
    Vec3i rotatedPosition;
    s32 i;
    s32 slideStep;

    actor = arg0;

    for (i = 0; i < (s32)gPlayerCount; i++) {
        if (((CourseSelectStatusOverlay *)gCourseSelectStatus)->outgoingPreviewModelState[i] != actor->state[i]) {
            actor->state[i] = ((CourseSelectStatusOverlay *)gCourseSelectStatus)->outgoingPreviewModelState[i];
            actor->timer[i] = ((CourseSelectStatusOverlay *)gCourseSelectStatus)->outgoingPreviewModelTimer[i];
            actor->angle[i] = ((CourseSelectStatusOverlay *)gCourseSelectStatus)->outgoingPreviewModelAngle[i];
            ((CourseSelectStatusOverlay *)gCourseSelectStatus)->outgoingPreviewModelTimer[i] = 0;
            ((CourseSelectStatusOverlay *)gCourseSelectStatus)->outgoingPreviewModelAngle[i] = 0;
        }

        if ((gMenuFlowState != 0) && (actor->state[i] < 5)) {
            actor->state[i] = 4;
            actor->angle[i] = 0;
        }

        switch (actor->state[i]) {
        case 0:
            break;
        case 1:
            if (D_8010AECC[i] & 1) {
                if (D_8010AEE8[i] < 0) {
                    if ((s32)gRacePlayers[i].courseSelectCharacterId >= 9) {
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
                    actor->targetCourse[i] = D_8010AEFB[i * 4];
                }
                if (actor->targetCourse[i] == 8) {
                    actor->targetCourse[i] = 2;
                }
                if (D_8010AECC[i] == 1) {
                    actor->vecs[i].y = -D_8010AEE8[i];
                }
                actor->state[i] = 2;
            } else if (gCurrentGameTask->screenState == 9) {
                actor->state[i] = 8;
            }
            break;
        case 2:
            slideStep = 0x200000;
            if (D_8010AEE8[i] < 0) {
                slideStep = -0x200000;
            }
            actor->vecs[i].y += slideStep;
            D_8010AEE8[i] -= slideStep;
            if (D_8010AEE8[i] == 0) {
                actor->state[i] = 1;
                D_8010AECC[i]++;
                D_8010AECC[i] &= 3;
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
        ((CourseSelectStatusOverlay *)gCourseSelectStatus)->outgoingPreviewModelState[i] = actor->state[i];
    }

    if ((gRacePlayers[0].menuState == 4) || (actor->state[0] == 9)) {
        removeCallbackTask(actor);
        finishCourseSelectUiTask(2);
        D_8010ADE4 = 0;
    } else {
        addRenderCallback(&gModelRenderCallbackList, (RenderCallback)(void (*)(CourseSelectWidgetActor *))drawCourseSelectPreviewModelClose,
                          (CourseSelectWidgetActor *)actor);
    }
}

void initCourseSelectPreviewModelOut(void *arg0) {
    CourseSelectAnimatedActor *actor;
    Vec3i position;
    u8 *outgoingModelState;
    s32 i;

    actor = arg0;
    outgoingModelState = (u8 *)&gCourseSelectStatus;
    i = 0;
    do {
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
        i++;
        outgoingModelState[8] = 1;
        outgoingModelState += 1;
    } while (outgoingModelState != &D_8010AF1C);

    actor->sourceTransform.translation.x = 0;
    actor->sourceTransform.translation.y = 0;
    actor->sourceTransform.translation.z = 0;
    makeFixedRotationYX(actor->sourceTransform.rotation, 0x400, 0x280);
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateCourseSelectPreviewModelOut);
}

void drawCourseSelectCourseIconList(CourseSelectIconListActor *iconList) {
    enum {
        CURRENT_ITEM_COUNT_ACTOR,
        PLAYER_ITEM_COUNT_ACTOR = 5,
        BASE_ICON_LIST_ACTOR = 11
    };
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

                        selectionState = D_8010AEA4[playerIndex];
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
                            iconList->y[playerIndex][iconIndex], iconList->x[playerIndex][iconIndex],
                            getRelocatableHeapBlockBase(gAssetHandles[0x21]), (u16)iconTileIndex, 0x20, 0x20, 0,
                            highlightAlpha, iconList->clipLeft - splitScreenClipOffset, iconList->clipTop,
                            iconList->clipRight, iconList->clipBottom);
                    } else if (gPlayerCount == 1) {
                        highlightAlpha = 0;
                        leftClipOffset = (playerIndex >= 2) * 0x88;
                        if ((D_8010AEA4[playerIndex] > 0) && (D_8010AEA4[playerIndex] < 8) &&
                            (gCourseSelectExtraCourseColumnState == 1) && (D_8010AEA4[playerIndex] & 1)) {
                            highlightAlpha = 0xFF;
                        }

                        if (gRaceSplitscreenMode == 3) {
                            drawMenuSpriteClipped(
                                iconList->y[playerIndex][iconIndex], iconList->x[playerIndex][iconIndex],
                                getRelocatableHeapBlockBase(gAssetHandles[0x27]), 5, 0x20, 0x20, 0, highlightAlpha,
                                iconList->clipLeft - leftClipOffset, iconList->clipTop, iconList->clipRight,
                                iconList->clipBottom);
                        } else {
                            drawMenuSpriteClipped(
                                iconList->y[playerIndex][iconIndex], iconList->x[playerIndex][iconIndex],
                                getRelocatableHeapBlockBase(gAssetHandles[0x21]), 5, 0x20, 0x20, 0, highlightAlpha,
                                iconList->clipLeft - leftClipOffset, iconList->clipTop, iconList->clipRight,
                                iconList->clipBottom);
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

// updateCourseSelectCourseIconList best match: 99.844% (nonmatchings/updateCourseSelectCourseIconList-4777730848216765513/base_33.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_ui/updateCourseSelectCourseIconList.s")

#ifdef NON_MATCHING
void updateCourseSelectCourseIconList(CourseSelectIconListActor *arg0) {
    CourseSelectIconListActor *actor;
    s32 playerIndex;
    s32 movingCount;
    s32 iconIndex;
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
            movingCount = 0;
            for (iconIndex = 0; iconIndex < (s32)actor->itemCounts[playerIndex]; iconIndex++) {
                if (actor->y[playerIndex][iconIndex] < actor->targetY[playerIndex]) {
                    actor->y[playerIndex][iconIndex] += 0x10;
                    movingCount++;
                    if (actor->y[playerIndex][iconIndex] >= actor->targetY[playerIndex]) {
                        actor->y[playerIndex][iconIndex] = actor->targetY[playerIndex];
                    }
                }
            }

            actor->timer[playerIndex]++;
            if ((D_80121D80[playerIndex].selectedCharacterId == 5) ||
                (D_8010AEA0[playerIndex] == 0) || (gCourseSelectModeSelection == 1)) {
                count = 4;
            } else if (D_8010AEA0[playerIndex] != 0) {
                count = 5;
            }
            if (((actor->timer[playerIndex] & 1) == 0) &&
                ((s32)actor->itemCounts[playerIndex] < (s32)count)) {
                actor->itemCounts[playerIndex]++;
            }

            if ((u32)movingCount == 0) {
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
            if (((CourseSelectPlayerMenuState *)&D_80121D88)[playerIndex].menuState == 1) {
                actor->state[playerIndex] = 2;
            }
            break;

        case 2:
            if (D_8010AEA4[playerIndex] >= 9) {
                for (iconIndex = 0; iconIndex < (s32)actor->itemCounts[playerIndex]; iconIndex++) {
                    if (iconIndex != gCharacterSelectHudState.highlightedRosterIndices[playerIndex]) {
                        actor->y[playerIndex][iconIndex] -= 0x20;
                    }
                }

                if (gCharacterSelectHudState.highlightedRosterIndices[playerIndex] != 0) {
                    if (actor->startY[playerIndex] >= actor->y[playerIndex][0]) {
                        actor->state[playerIndex] = 3;
                    }
                } else if (actor->startY[playerIndex] >= actor->y[playerIndex][1]) {
                    actor->state[playerIndex] = 3;
                }

                if (actor->state[playerIndex] == 3) {
                    if (actor->x[playerIndex][gCharacterSelectHudState.highlightedRosterIndices[playerIndex]] <
                        actor->targetX[playerIndex]) {
                        actor->direction[playerIndex] = 1;
                    } else {
                        actor->direction[playerIndex] = -1;
                    }
                }
            }
            break;

        case 3:
            actor->x[playerIndex][gCharacterSelectHudState.highlightedRosterIndices[playerIndex]] +=
                actor->speed[playerIndex] * actor->direction[playerIndex];
            if (((actor->direction[playerIndex] == 1) &&
                 (actor->x[playerIndex][gCharacterSelectHudState.highlightedRosterIndices[playerIndex]] >=
                  actor->targetX[playerIndex])) ||
                ((actor->direction[playerIndex] == -1) &&
                 (actor->x[playerIndex][gCharacterSelectHudState.highlightedRosterIndices[playerIndex]] <=
                  actor->targetX[playerIndex]))) {
                actor->x[playerIndex][gCharacterSelectHudState.highlightedRosterIndices[playerIndex]] =
                    actor->targetX[playerIndex];
                actor->state[playerIndex] = 4;
                gMenuChoicePromptState[playerIndex] = 1;
            }
            break;

        case 4:
            if ((gMenuFlowState == 0) && (gMenuChoicePromptState[playerIndex] == 0)) {
                actor->state[playerIndex] = 5;
            }
            if (D_80121D80[playerIndex].menuState == 3) {
                actor->state[playerIndex] = 7;
            }
            break;

        case 5:
            actor->x[playerIndex][gCharacterSelectHudState.highlightedRosterIndices[playerIndex]] +=
                actor->speed[playerIndex] * actor->direction[playerIndex] * -1;
            if (((actor->direction[playerIndex] == 1) &&
                 (actor->x[playerIndex][gCharacterSelectHudState.highlightedRosterIndices[playerIndex]] <=
                  actor->baseX[playerIndex] +
                      (gCharacterSelectHudState.highlightedRosterIndices[playerIndex] *
                       actor->speed[playerIndex]))) ||
                ((actor->direction[playerIndex] == -1) &&
                 (actor->x[playerIndex][gCharacterSelectHudState.highlightedRosterIndices[playerIndex]] >=
                  actor->baseX[playerIndex] +
                      (gCharacterSelectHudState.highlightedRosterIndices[playerIndex] *
                       actor->speed[playerIndex])))) {
                actor->x[playerIndex][gCharacterSelectHudState.highlightedRosterIndices[playerIndex]] =
                    actor->baseX[playerIndex] +
                    (gCharacterSelectHudState.highlightedRosterIndices[playerIndex] * actor->speed[playerIndex]);
                actor->state[playerIndex] = 6;
            }
            break;

        case 6:
            for (iconIndex = 0; iconIndex < (s32)actor->itemCounts[playerIndex]; iconIndex++) {
                if (iconIndex != gCharacterSelectHudState.highlightedRosterIndices[playerIndex]) {
                    actor->y[playerIndex][iconIndex] += 0x20;
                }
            }

            if (gCharacterSelectHudState.highlightedRosterIndices[playerIndex] != 0) {
                if (actor->y[playerIndex][0] >= actor->targetY[playerIndex]) {
                    actor->state[playerIndex] = 1;
                }
            } else if (actor->y[playerIndex][1] >= actor->targetY[playerIndex]) {
                actor->state[playerIndex] = 1;
            }
            if (actor->state[playerIndex] == 1) {
                ((CourseSelectPlayerMenuState *)&D_80121D88)[playerIndex].menuState = 0;
            }
            break;

        case 7:
            if (gMenuFlowState != 0) {
                for (iconIndex = 0; iconIndex < (s32)actor->itemCounts[playerIndex]; iconIndex++) {
                    actor->y[playerIndex][iconIndex] -= 0x20;
                }
            } else {
                actor->y[playerIndex][gCharacterSelectHudState.highlightedRosterIndices[playerIndex]] -= 0x20;
            }
            if (actor->y[playerIndex][gCharacterSelectHudState.highlightedRosterIndices[playerIndex]] <=
                actor->startY[playerIndex]) {
                actor->state[playerIndex] = 8;
            }
            break;

        case 9:
            actor->y[playerIndex][gCharacterSelectHudState.highlightedRosterIndices[playerIndex]] -= 0x20;
            if (actor->y[playerIndex][gCharacterSelectHudState.highlightedRosterIndices[playerIndex]] <=
                actor->startY[playerIndex]) {
                actor->state[playerIndex] = 10;
            }
            break;

        case 11:
            actor->y[playerIndex][gCharacterSelectHudState.highlightedRosterIndices[playerIndex]] += 0x20;
            if (actor->y[playerIndex][gCharacterSelectHudState.highlightedRosterIndices[playerIndex]] >=
                actor->targetY[playerIndex]) {
                actor->y[playerIndex][gCharacterSelectHudState.highlightedRosterIndices[playerIndex]] =
                    actor->targetY[playerIndex];
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
#endif

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
    do { playerIndex = 0; if ((s32) gPlayerCount > 0) { courseUnlocked = D_8010AEA0; yLayout = gCourseSelectIconListYLayout[layoutIndex]; xLayout = gCourseSelectIconListXLayout[layoutIndex]; do { if ((*courseUnlocked == 0) || (gRacePlayers[playerIndex].selectedCharacterId == 5)) { hasExtraCourse = 0; } else { hasExtraCourse = 1; } actor->speed[playerIndex] = yLayout[hasExtraCourse]; actor->baseX[playerIndex] = yLayout[((playerIndex & 1) * 2) + hasExtraCourse + 2]; actor->targetX[playerIndex] = yLayout[((playerIndex & 1) * 2) + 2]; actor->targetY[playerIndex] = xLayout[((playerIndex >= 2) * 2) + 1]; actor->startY[playerIndex] = xLayout[(playerIndex >= 2) * 2]; actor->clipBottom = 0x78; actor->clipRight = 0xA0; actor->clipTop = 0x78; if (gPlayerCount == 1) { actor->clipLeft = 0xA0; } else if (gPlayerCount == 2) { actor->clipLeft = 0x7E; } else { actor->clipLeft = 0x88; } iconIndex = 0; do { actor->y[playerIndex][iconIndex] = actor->startY[playerIndex]; actor->x[playerIndex][iconIndex] = actor->baseX[playerIndex] + (actor->speed[playerIndex] * iconIndex); iconIndex++; } while (iconIndex < 5); actor->state[playerIndex] = 0; actor->itemCounts[playerIndex] = 1; actor->timer[playerIndex] = 0; courseUnlocked++; playerIndex++; } while (playerIndex < (s32) gPlayerCount); } setCallbackTaskCallback(actor, (CallbackTaskCallback) updateCourseSelectCourseIconList); } while (0);
}

void drawCourseSelectCourseCursors(CourseSelectWidgetActor *arg0) {
    u8 *actor;
    s16 *drawPosPtr;
    s32 i;
    u8 *statePtr;
    s16 *posPtr;
    int cursorStride;
    s8 *directionPtr;
    s16 *handles;
    u8 *cursorBytes;
    s32 tileIndex;
    s32 handleIndex;
    s32 one;
    s32 playerCount;
    u8 *cursorBase;

    actor = (u8 *) arg0;
    if (D_8010AEB0 == 0) {
        playerCount = gPlayerCount;
        i = 0;
        if ((playerCount - 1) >= 0) {
            statePtr = (u8 *) arg0;
            cursorStride = 2;
            handles = gAssetHandles;
            one = 1;
            do {
                directionPtr = &D_8010AE64[i];
                if (statePtr[0x30] < cursorStride) {
                    cursorBase = actor;
                    posPtr = (s16 *) ((cursorBytes = cursorBase) + (i * cursorStride));
                    if (one == gPlayerCount) {
                        tileIndex = 7;
                        handleIndex = 0x25;
                    } else {
                        tileIndex = 0x13;
                        handleIndex = 0x21;
                    }
                    drawMenuSpriteWithAlpha(posPtr[0xC], (s16)((drawPosPtr = posPtr)[0x10] + ((*directionPtr) * statePtr[0x38])),
                                            getRelocatableHeapBlockBase(handles[handleIndex]), tileIndex,
                                            0x20, 0x20, 0, drawPosPtr[0x14], (short)0);
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
    if ((s32) gPlayerCount > 0) {
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
            gCourseSelectStatus->bytes[i - 1] = state;
        } while (i < (s32) gPlayerCount);
    }

    if (actor->courseCursorState[0] == 4) {
        removeCallbackTask(actor);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseSelectCourseCursors, actor);
    }
}

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
    mask = 0xFFFFFFFFFFFFFFFFu; if ((s32)gPlayerCount < 3) { layoutIndex = gPlayerCount - 1; } else { layoutIndex = 2; } playerIndex = 0; if ((s32)gPlayerCount > 0) { courseUnlocked = D_8010AEA0; yLayout = gCourseSelectIconListYLayout[layoutIndex]; xLayout = gCourseSelectIconListXLayout[layoutIndex]; do { unlockState = *courseUnlocked & mask; courseUnlocked++; hasExtraCourse = (((0, unlockState) == 0) || (gRacePlayers[playerIndex].selectedCharacterId == 5)) ? 0 : 1;
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


void drawCourseSelectCourseListBackdrop(CourseSelectWidgetActor *arg0) {
    drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(gAssetHandles[0x25]), 3, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16) (arg0->x + 0x40), arg0->y, getRelocatableHeapBlockBase(gAssetHandles[0x25]), 4, 0x20, 0x20, 0, 0);
    drawMenuSprite(arg0->x, (s16) (arg0->y + 0x40), getRelocatableHeapBlockBase(gAssetHandles[0x25]), 5, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16) (arg0->x + 0x40), (s16) (arg0->y + 0x40), getRelocatableHeapBlockBase(gAssetHandles[0x25]), 6, 0x20, 0x20, 0, 0);
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

// drawCourseSelectCourseStats best match: 99.880%
// (nonmatchings/drawCourseSelectCourseStats-5176680205357669729/base_47.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_ui/drawCourseSelectCourseStats.s")

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
                        drawMenuSpriteWithAlpha(arg0->courseStatsX[i], arg0->courseStatsY[i],
                                                getRelocatableHeapBlockBase(panelHandle), panelTile, 0x20, 0x20, 0,
                                                arg0->courseStatsAlpha, 0);
                        if ((s32)gPlayerCount < 3) {
                            u8 *ratings;

                            if (selectedCourseId >= 9) {
                                drawMenuAsciiText((s16)(arg0->courseStatsX[i] + 0x38), arg0->courseStatsY[i],
                                                  D_800E0DA0, 0, arg0->courseStatsAlpha);
                            } else {
                                ratings = &gCourseSelectCourseSpeedRatings[selectedCourseId * 3];
                                j = 0;
                                if (ratings[0] > 0) {
                                    k = 0;
                                    do {
                                        drawMenuSpriteWithAlpha(
                                            (s16)((arg0->courseStatsX[i] + k) + 0x38),
                                            (s16)(arg0->courseStatsY[i] - 2),
                                            getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0x25, 0x20, 0x20, 0,
                                            arg0->courseStatsAlpha, 0);
                                        j++;
                                        k += 0xC;
                                    } while (j < ratings[0]);
                                }
                            }
                            if (selectedCourseId >= 9) {
                                drawMenuAsciiText((s16)(arg0->courseStatsX[i] + 0x38),
                                                  (s16)(arg0->courseStatsY[i] + 0xC), D_800E0DA4, 0,
                                                  arg0->courseStatsAlpha);
                            } else {
                                ratings = &gCourseSelectCourseSpeedRatings[selectedCourseId * 3];
                                j = 0;
                                if (ratings[1] > 0) {
                                    k = 0;
                                    do {
                                        drawMenuSpriteWithAlpha(
                                            (s16)((arg0->courseStatsX[i] + k) + 0x38),
                                            (s16)(arg0->courseStatsY[i] + 0xA),
                                            getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0x25, 0x20, 0x20, 0,
                                            arg0->courseStatsAlpha, 0);
                                        j++;
                                        k += 0xC;
                                    } while (j < ratings[1]);
                                }
                            }
                            if (selectedCourseId >= 9) {
                                drawMenuAsciiText((s16)(arg0->courseStatsX[i] + 0x38),
                                                  (s16)(arg0->courseStatsY[i] + 0x18), D_800E0DA8, 0,
                                                  arg0->courseStatsAlpha);
                            } else {
                                ratings = &gCourseSelectCourseSpeedRatings[selectedCourseId * 3];
                                j = 0;
                                if (ratings[2] > 0) {
                                    k = 0;
                                    do {
                                        drawMenuSpriteWithAlpha(
                                            (s16)((arg0->courseStatsX[i] + k) + 0x38),
                                            (s16)(arg0->courseStatsY[i] + 0x16),
                                            getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0x25, 0x20, 0x20, 0,
                                            arg0->courseStatsAlpha, 0);
                                        j++;
                                        k += 0xC;
                                    } while (j < ratings[2]);
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
                                sprintf((char *)text, D_800E0DAC,
                                        gCourseSelectCourseSpeedRatings[selectedCourseId * 3]);
                            }
 drawMenuAsciiText((s16) (arg0->courseStatsX[i] + 0x34), arg0->courseStatsY[i], text, 0, arg0->courseStatsAlpha); if (selectedCourseId < 9) { sprintf((char *)text, D_800E0DB0, gCourseSelectCourseHandlingRatings[selectedCourseId * 3]); } drawMenuAsciiText((s16) (arg0->courseStatsX[i] + 0x34), (s16) (arg0->courseStatsY[i] + 8), text, 0, arg0->courseStatsAlpha); if (selectedCourseId < 9) { sprintf((char *)text, D_800E0DB4, gCourseSelectCourseTrickRatings[selectedCourseId * 3]); } drawMenuAsciiText((s16) (arg0->courseStatsX[i] + 0x34), (s16) ((unsigned short) (arg0->courseStatsY[i] + 0x10)), text, 0, arg0->courseStatsAlpha); } if (((s32) gPlayerCount) >= 2) { if (((s32) gPlayerCount) == 2) { j = 0; } else { j = 1; } drawMenuSpriteWithAlpha((s16) (gCourseSelectStatsPlayerMarkerLayout[j][i & 1][0] + ((i >= 2) * 0x8C)), gCourseSelectStatsPlayerMarkerLayout[j][i & 1][1], getRelocatableHeapBlockBase(gAssetHandles[0x21]), 0x12, 0x20, 0x20, new_var, arg0->courseStatsAlpha, 0); } } i++; } while (i < ((s32) gPlayerCount)); } } while (0);
    } while (0);
}
#endif

#ifdef NON_MATCHING
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
            ((gPlayerCount >= 2) ||
             ((gPlayerCount == 1) && (gCourseSelectExtraCourseColumnState == 0)))) {
            if ((gRacePlayers[i].menuSelection >= 9) &&
                (gRacePlayers[i].menuSelection < 12)) {
                courseIndex = gRacePlayers[i].menuSelection;
            } else {
                courseIndex = gRacePlayers[i].menuSelection % 3;
            }

            drawMenuSpriteWithAlpha(
                arg0->courseStatsX[i], arg0->courseStatsY[i],
                getRelocatableHeapBlockBase(gAssetHandles[0x21]), panelTile, 0x20,
                0x20, 0, arg0->courseStatsAlpha, 0);

            if (gPlayerCount < 3) {
                if (courseIndex >= 9) {
                    if ((D_800E0DA8 && D_800E0DA8) && D_800E0DA8) {
                    }
                    drawMenuAsciiText((s16)(arg0->courseStatsX[i] + 0x38),
                                      arg0->courseStatsY[i], D_800E0DA0, 0,
                                      arg0->courseStatsAlpha);
                } else {
                    j = 0;
                    if (j < gCourseSelectCourseSpeedRatings[courseIndex * 3]) {
                        xOffset = 0;
                        do {
                            drawMenuSpriteWithAlpha(
                                (s16)(arg0->courseStatsX[i] + xOffset + 0x38),
                                (s16)(arg0->courseStatsY[i] - 2),
                                getRelocatableHeapBlockBase(gAssetHandles[0x24]),
                                0x25, 0x20, 0x20, 0, arg0->courseStatsAlpha, 0);
                            j++;
                            xOffset += 0xC;
                        } while (j <
                                 gCourseSelectCourseSpeedRatings[courseIndex * 3]);
                    }
                }

                if (courseIndex >= 9) {
                    drawMenuAsciiText((s16)(arg0->courseStatsX[i] + 0x38),
                                      (s16)(arg0->courseStatsY[i] + 0xC),
                                      D_800E0DA4, 0, arg0->courseStatsAlpha);
                } else {
                    j = 0;
                    if (j <
                        gCourseSelectCourseSpeedRatings[(courseIndex * 3) + 1]) {
                        xOffset = 0;
                        do {
                            drawMenuSpriteWithAlpha(
                                (s16)(arg0->courseStatsX[i] + xOffset + 0x38),
                                (s16)(arg0->courseStatsY[i] + 0xA),
                                getRelocatableHeapBlockBase(gAssetHandles[0x24]),
                                0x25, 0x20, 0x20, 0, arg0->courseStatsAlpha, 0);
                            j++;
                            xOffset += 0xC;
                        } while (
                            j <
                            gCourseSelectCourseSpeedRatings[(courseIndex * 3) + 1]);
                    }
                }

                if (courseIndex >= 9) {
                    if (1) {
                        drawMenuAsciiText(
                            (s16)(arg0->courseStatsX[i] + 0x38),
                            (s16)(arg0->courseStatsY[i] + 0x18), D_800E0DA8, 0,
                            arg0->courseStatsAlpha);
                    }
                } else {
                    j = 0;
                    if (j <
                        gCourseSelectCourseSpeedRatings[(courseIndex * 3) + 2]) {
                        xOffset = 0;
                        do {
                            drawMenuSpriteWithAlpha(
                                (s16)(arg0->courseStatsX[i] + xOffset + 0x38),
                                (s16)(arg0->courseStatsY[i] + 0x16),
                                getRelocatableHeapBlockBase(gAssetHandles[0x24]),
                                0x25, 0x20, 0x20, 0, arg0->courseStatsAlpha, 0);
                            j++;
                            xOffset += 0xC;
                        } while (
                            j <
                            gCourseSelectCourseSpeedRatings[(courseIndex * 3) + 2]);
                    }
                }
            } else {
                if (courseIndex >= 9) {
                    text[0] = '?';
                    text[1] = '\0';
                }
                if (courseIndex < 9) {
                    sprintf(text, D_800E0DAC,
                            gCourseSelectCourseSpeedRatings[courseIndex * 3]);
                }
                drawMenuAsciiText((s16)(arg0->courseStatsX[i] + 0x34),
                                  arg0->courseStatsY[i], text, 0,
                                  arg0->courseStatsAlpha);

                if (courseIndex < 9) {
                    sprintf(text, D_800E0DB0,
                            gCourseSelectCourseHandlingRatings[courseIndex * 3]);
                }
                drawMenuAsciiText((s16)(arg0->courseStatsX[i] + 0x34),
                                  (s16)(arg0->courseStatsY[i] + 8), text, 0,
                                  arg0->courseStatsAlpha);

                if (courseIndex < 9) {
                    sprintf(text, D_800E0DB4,
                            gCourseSelectCourseTrickRatings[courseIndex * 3]);
                }
                drawMenuAsciiText((s16)(arg0->courseStatsX[i] + 0x34),
                                  (u16)(arg0->courseStatsY[i] + 0x10), text, 0,
                                  arg0->courseStatsAlpha);
            }

            if (gPlayerCount >= 2) {
                if (gPlayerCount == 2) {
                    j = 0;
                } else {
                    j = 1;
                }
                drawMenuSpriteWithAlpha(
                    (s16)(gCourseSelectStatsPlayerMarkerLayout[j][i & 1][0] +
                          ((i >= 2) * 0x8C)),
                    gCourseSelectStatsPlayerMarkerLayout[j][i & 1][1],
                    getRelocatableHeapBlockBase(gAssetHandles[0x21]), 0x12, 0x20,
                    0x20, 0, arg0->courseStatsAlpha, 0);
            }
        }
    }
}
#endif

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
    s16 (*table)[4];

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

// drawCourseSelectCourseDescription best match: 95.254% (nonmatchings/drawCourseSelectCourseDescription-4777730848216765513/base_18.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_ui/drawCourseSelectCourseDescription.s")

#if 0
#ifdef NON_MATCHING
typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ u16 unk2C;
    /* 0x2E */ u8 unk2E;
} CourseSelectStatus26C4C;

extern u32 gCourseUnlockPrices[];
extern u8 gUnlockedExtraCourseFlags;
extern u8 gCourseSelectExtraCourseIds[];

void drawCourseSelectCourseDescription(CourseSelectWidgetActor *arg0) {
    volatile CourseSelectStatus26C4C *status = (CourseSelectStatus26C4C *)gCourseSelectStatus;
    MenuGlyphScript *volatile text;
    volatile RacePlayer *selection;
    MenuGlyphScript buffer[8];
    s32 value;
    s32 courseId;
    s32 digitCount;
    u32 price;
    MenuGlyphScript *digits;
    u16 selectedIndex;
    u16 splitMode;

    selection = gRacePlayers;
    price = 3;
    digitCount = status->unk2C;
    value = digitCount;
    if ((u32)value == 0) {
        s32 courseIndex;
        s32 selectedCourseId;

        if ((D_8010AEA8 == 0) && ((value = selection->menuState) == 0 || (value == 3) || (value == 9))) {
            value = status->unk2E;
            if (value == 1) {
                value = price;
            } else if (value == 2) {
                value = 4;
            } else if ((courseId = selection->menuSelection, courseId >= 9) && (courseId < 12)) {
                value = 5;
            } else {
                value = courseId % 3;
                value &= 0xFFFFU;
            }
            text = gCourseSelectModeDescriptionText[value].text;
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

            if ((gRaceSplitscreenMode == 3) && ((courseId = selection->menuSelection) <= 8)) {
                text = gCourseSelectBoardLevelByCourseText[courseId % 3].text;
            } else if (((courseId = selection->menuSelection) >= 9) && (courseId < 12)) {
                text = gCourseSelectExtraCourseBoardLevelText[gCourseSelectExtraCourseIds[selectedIndex] % 3].text;
            } else {
                text = gCourseSelectBoardLevelText;
            }
        }

        drawMenuGlyphScript(arg0->x, arg0->y, text, 1, arg0->spriteIndex, 0);

        if ((gRaceSplitscreenMode == 3) && (((value = selection->menuState) == 1) || (value == 2))) {
            if ((D_8010AE64[0] != 3) || !(gUnlockedExtraCourseFlags & 7)) {
                buffer[0] = -4;
                buffer[1] = 6;
                buffer[2] = selectedIndex;
                buffer[3] = -1;
                drawMenuGlyphScript((s16)(arg0->x + 0x48), (s16)(arg0->y + 0x10), buffer, 1, arg0->spriteIndex, 0);
            }

            splitMode = gCourseSelectModeSelection;
            if (splitMode == 0) {
                buffer[0] = -4;
                buffer[1] = 6;
                if ((selectedIndex >= 2) || (selection->menuSelection >= 9)) {
                    selectedCourseId = selection->menuSelection;
                    if (selectedCourseId >= 9) {
                        courseIndex = gCourseSelectExtraCourseIds[selectedIndex];
                    } else {
                        courseIndex = ((selectedCourseId % 3) + (selectedIndex * 3)) - 3;
                    }

                    price = gCourseUnlockPrices[courseIndex];
                    if (price < 10000) {
                        digitCount = 5;
                    } else if (price < 100000) {
                        digitCount = 6;
                    } else {
                        digitCount = 7;
                    }

                    if (price != 0) {
                        digits = &buffer[digitCount];
                        do {
                            digits -= 1;
                            digits[1] = price % 10;
                            digitCount = 10;
                            price /= digitCount;
                        } while (price != 0);
                    }

                    digits = &buffer[digitCount];
                    digits[1] = 0x10;
                    digits[2] = -1;
                    if (selection->menuSelection >= 9) {
                    }
                } else {
                    digitCount = 3;
                    do {
                        buffer[2] = 0x2B;
                        digits = &buffer[digitCount];
                        digits[0] = 0x2B;
                        digits[1] = 0x2B;
                    } while (0);
                    digits[2] = 0x2B;
                    digits[3] = 0x2B;
                    buffer[7] = -1;
                }
                drawMenuGlyphScript((s16)(arg0->x + 0x20), (s16)(arg0->y + 0x20), buffer, 1, arg0->spriteIndex, 0);
            }
        }
    } else {
        digitCount = arg0->spriteIndex;
        drawMenuGlyphScript(arg0->x, arg0->y, gCourseSelectPurchaseMessageText[value - 1].text, 1, digitCount, 0);
    }
}
#endif
#endif

#ifdef NON_MATCHING
extern u8 gCourseSelectExtraCourseIds[];
extern u32 gCourseUnlockPrices[];
extern u8 gUnlockedExtraCourseFlags;

void drawCourseSelectCourseDescription(CourseSelectWidgetActor *arg0) {
    u8 unused[4];
    MenuGlyphScript *volatile text;
    MenuGlyphScript *boardText;
    MenuGlyphScript script[8];
    u32 descriptionIndex;
    s32 pricedCourseId;
    u32 price;
    s32 digitCount;
    MenuGlyphScript *digit;
    u16 selectedIndex;

    if (gCourseSelectStatus->purchaseMessage == 0) {
        if ((D_8010AEA8 == 0) &&
            ((D_80121D80->menuState == 0) || (D_80121D80->menuState == 3) ||
             (D_80121D80->menuState == 9))) {
            if (gCourseSelectStatus->descriptionMode == 1) {
                descriptionIndex = 3;
            } else if (gCourseSelectStatus->descriptionMode == 2) {
                descriptionIndex = 4;
            } else if ((D_80121D80->courseIndex >= 9) && (D_80121D80->courseIndex < 12)) {
                descriptionIndex = 5;
            } else {
                descriptionIndex = (D_80121D80->courseIndex % 3) & 0xFFFF;
            }
            text = gCourseSelectModeDescriptionText[descriptionIndex].text;
        } else {
            if ((gMenuChoicePromptState[0] < 2) || (gMenuChoicePromptState[0] == 9)) {
                selectedIndex = 1;
            } else if (gMenuChoicePromptState[0] < 5) {
                selectedIndex = gMenuChoicePromptState[0] - 1;
            }
            if ((gMenuChoicePromptState[0] >= 5) && (gMenuChoicePromptState[0] != 9)) {
                arg0->pad18_2[8] = 1;
            }
            if (arg0->pad18_2[8] == 0) {
                arg0->pad18_2[7] = selectedIndex;
            } else {
                selectedIndex = arg0->pad18_2[7];
            }
            if ((gRaceSplitscreenMode == 3) && (D_80121D80->courseIndex < 9)) {
                text = gCourseSelectBoardLevelByCourseText[D_80121D80->courseIndex % 3].text;
            } else {
                if ((D_80121D80->courseIndex >= 9) && (D_80121D80->courseIndex < 12)) {
                    descriptionIndex = gCourseSelectExtraCourseIds[selectedIndex] % 3;
                    boardText = gCourseSelectExtraCourseBoardLevelText[descriptionIndex].text;
                    text = boardText;
                } else {
                    boardText = gCourseSelectBoardLevelText;
                }
                text = boardText;
            }
        }

        drawMenuGlyphScript(arg0->x, arg0->y, text, 1, (u16)(s32)arg0->spriteIndex, 0);

        if ((gRaceSplitscreenMode == 3) &&
            ((D_80121D80->menuState == 1) || (D_80121D80->menuState == 2))) {
            if ((gCharacterSelectHudState.highlightedRosterIndices[0] != 3) ||
                ((gUnlockedExtraCourseFlags & 7) == 0)) {
                script[0] = 0xFFFC;
                script[1] = 6;
                script[2] = selectedIndex;
                script[3] = 0xFFFF;
                drawMenuGlyphScript(arg0->x + 0x48, arg0->y + 0x10, script, 1,
                                    (u16)(s32)arg0->spriteIndex, 0);
            }

            if (gCourseSelectModeSelection == 0) {
                script[0] = 0xFFFC;
                script[1] = 6;
                if ((selectedIndex >= 2) || (D_80121D80->courseIndex >= 9)) {
                    if (D_80121D80->courseIndex >= 9) {
                        pricedCourseId = gCourseSelectExtraCourseIds[selectedIndex];
                    } else {
                        pricedCourseId =
                            (D_80121D80->courseIndex % 3) + (selectedIndex * 3) - 3;
                    }
                    price = gCourseUnlockPrices[pricedCourseId];
                    if (price < 10000) {
                        digitCount = 5;
                    } else if (price < 100000) {
                        digitCount = 6;
                    } else {
                        digitCount = 7;
                    }
                    pricedCourseId = D_80121D80->courseIndex;
                    if (price != 0) {
                        digit = &script[digitCount];
                        do {
                            digit--;
                            digit[1] = price % 10;
                            price /= 10;
                        } while (price != 0);
                    }
                    script[digitCount + 1] = 0x10;
                    script[digitCount + 2] = 0xFFFF;
                    if (pricedCourseId >= 9) {
                    }
                } else {
                    digitCount = 3;
                    script[2] = 0x2B;
                    digit = &script[digitCount];
                    digit[3] = digit[2] = digit[1] = digit[0] = 0x2B;
                    script[7] = 0xFFFF;
                }
                descriptionIndex = arg0->y;
                drawMenuGlyphScript(arg0->x + 0x20, descriptionIndex + 0x20, script, 1,
                                    (u16)(s32)arg0->spriteIndex, 0);
            }
        }
    } else {
        digitCount = gCourseSelectStatus->purchaseMessage;
        drawMenuGlyphScript(arg0->x, arg0->y,
                            gCourseSelectPurchaseMessageText[digitCount - 1U].text,
                            1, (u16)(s32)arg0->spriteIndex, 0);
    }
}
#endif

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
        arg0->coordinates[0], arg0->coordinates[1],
        getRelocatableHeapBlockBase(assetHandle),
        renderTileIndex, 0x20, 0x20, 0, arg0->coordinates[2], 0);
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

// drawCourseSelectExtraCourseIconList best match: 99.742% (nonmatchings/drawCourseSelectExtraCourseIconList-6934502587000073416/base_14.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_ui/drawCourseSelectExtraCourseIconList.s")

#ifdef NON_MATCHING
void drawCourseSelectExtraCourseIconList(CourseSelectWidgetActor *arg0_in) {
    CourseSelectExtraCourseIconListActor *arg0 = (CourseSelectExtraCourseIconListActor *)arg0_in;
    CourseSelectExtraCourseIconListActor *alphaActor;
    s32 overlayTile;
    s16 *promptState;
    s32 iconAlpha;
    s32 playerIndex;
    s32 iconIndex;
    s32 alphaOffset;

    playerIndex = 0;
    if ((s32)gPlayerCount > 0) { promptState = gMenuChoicePromptState; alphaOffset = 0; do {
            if (*promptState != 0) {
                if (*promptState != 9) {
                    iconIndex = arg0->itemCounts[playerIndex] - 1;
                    if (iconIndex >= 0) {
                        do {
                            if (*promptState == 1) {
                                iconAlpha = 0x100;
                            } else if ((iconIndex + 2) == *promptState) {
                                iconAlpha = 0x100;
                            } else {
                                iconAlpha = 0x60;
                            }
                            drawMenuSpriteWithAlphaClipped(
                                arg0->iconX[playerIndex][iconIndex], arg0->iconY[playerIndex][iconIndex],
                                getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                                arg0->tileIndices[playerIndex][iconIndex], 0x20, 0x20, 0, iconAlpha, 0,
                                arg0->clipLeft - ((playerIndex >= 2) * 0x8C), arg0->clipTop,
                                arg0->clipRight, arg0->clipBottom);
                            iconIndex -= 1;
                        } while (iconIndex >= 0);
                    }
                    if (D_8010AEB0 == 0) {
                        if ((*promptState >= 2) && (*promptState < 5)) {
                            alphaActor = (CourseSelectExtraCourseIconListActor *)&arg0->bytes[alphaOffset];
                            if ((s32)gPlayerCount == 1) {
                                overlayTile = 0xF;
                            } else {
                                overlayTile = 0x1B;
                            }
                            drawMenuSpriteWithAlpha(
                                arg0->iconX[playerIndex][0],
                                (s16)(arg0->iconY[playerIndex][0] +
                                      ((*promptState - 2) * arg0->rowSpacing[playerIndex])),
                                getRelocatableHeapBlockBase(gAssetHandles[0x21]), overlayTile & 0xFFFF,
                                0x20, 0x20, 0, alphaActor->alpha[0], 0);
                        }
                    }
                }
            }
            playerIndex += 1;
            alphaOffset += 2;
            promptState += 1;
        } while (playerIndex < (s32)gPlayerCount);
    }
}
#endif

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
                                        if (((CourseSelectExtraCourseSaveData *)gGameSaveDataBuffer)[playerIndex]
                                                .extraCourseUnlockFlags &
                                            iconMask) {
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
                                    } else if (((CourseSelectExtraCourseSaveData *)gGameSaveDataBuffer)[playerIndex]
                                                   .courseUnlockStates[iconIndex + 9] != -1) {
                                        actor->itemCounts[playerIndex]++;
                                        if (gPlayerCount == 1) {
                                            actor->tileIndices[playerIndex][actor->itemCounts[playerIndex] - 1] =
                                                iconIndex + 0xC;
                                        } else {
                                            actor->tileIndices[playerIndex][actor->itemCounts[playerIndex] - 1] =
                                                iconIndex + 0x1F;
                                        }
                                    }
                                } else if (((CourseSelectExtraCourseSaveData *)gGameSaveDataBuffer)[playerIndex]
                                               .courseUnlockStates[(gRacePlayers[playerIndex].menuSelection % 3) +
                                                                   (iconIndex * 3)] != -1) {
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
                            *promptState = D_8010AEAC[playerIndex] + 2;
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

            ((CourseSelectExtraCourseStatus *)gCourseSelectStatus)->extraCourseCounts[playerIndex] =
                actor->itemCounts[playerIndex];
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
                                            if (gGameSaveDataBuffer[playerIndex][0x78D7] & unlockMask) {
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
                                            if ((s8)gGameSaveDataBuffer[playerIndex][iconIndex + 0x48] != -1) {
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
                                    } else if ((s8)gGameSaveDataBuffer[playerIndex]
                                                       [(player->courseIndex % 3) + (iconIndex * 3) + 0x3F] != -1) {
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
                                *promptState = D_8010AEAC[playerIndex] + 2;
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
                gCourseSelectStatus[playerIndex + 0x24] =
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
            if (0) {
            }

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

#pragma GLOBAL_ASM("src/menu/course_select/D_800E0DB8.s")

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
        drawMenuAsciiText(
            (s16)(actor->x[i] + 0x34),
            (s16)(actor->y[i] + 2),
            text,
            0,
            alpha
        );

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
                                D_8010ADE0 = createCallbackTask((CallbackTaskCallback)initCourseSelectPreviewModelIn, 0, 0x62);
                                D_8010ADE4 = createCallbackTask((CallbackTaskCallback)initCourseSelectPreviewModelOut, 0, 0x62);
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
                drawMenuSpriteWithAlpha(actor->x, (s16)(actor->y + yOffset), getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0, 0x20,
                              0x20, 0, actor->playerPanelFadeAlpha[i], 0);
                drawMenuSpriteWithAlpha((s16)(actor->x + 0x40), (s16)(actor->y + yOffset),
                              getRelocatableHeapBlockBase(gAssetHandles[0x24]), 1, 0x20, 0x20, 0,
                              actor->playerPanelFadeAlpha[i], 0);
                drawMenuSpriteWithAlpha((s16)(actor->x + 0x78), (s16)(actor->y + yOffset),
                              getRelocatableHeapBlockBase(gAssetHandles[0x24]), 1, 0x20, 0x20, 0,
                              actor->playerPanelFadeAlpha[i], 0);
                drawMenuSpriteWithAlpha((s16)(actor->x + 0xB0), (s16)(actor->y + yOffset),
                              getRelocatableHeapBlockBase(gAssetHandles[0x24]), rightEdgeTile, 0x20, 0x20, 0,
                              actor->playerPanelFadeAlpha[i], 0);
                drawMenuGlyphScript((s16)(actor->x + 0x2E), (s16)(actor->y + yOffset + 0xC), gCourseSelectCompleteText, 0,
                              actor->playerPanelFadeAlpha[i], 0);
            } else {
                xOffset = (i >= 2) * 0x8C;
                yOffset = (i & 1) * 0x64;
                drawMenuSpriteWithAlpha((s16)(actor->x + xOffset), (s16)(actor->y + yOffset),
                              getRelocatableHeapBlockBase(gAssetHandles[0x24]), 8, 0x20, 0x20, 0,
                              actor->playerPanelFadeAlpha[i], 0);
                drawMenuSpriteWithAlpha((s16)(actor->x + xOffset + 0x40), (s16)(actor->y + yOffset),
                              getRelocatableHeapBlockBase(gAssetHandles[0x24]), 9, 0x20, 0x20, 0,
                              actor->playerPanelFadeAlpha[i], 0);
                drawMenuGlyphScript((s16)(actor->x + xOffset + 0x24), (s16)(actor->y + yOffset + 3), gCourseSelectCompleteText, 1,
                              actor->playerPanelFadeAlpha[i], 0);
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
            if ((source0 != NULL) &&
                ((source0->playerStates[i] == 4) || (source1->playerStates[i] == 4))) {
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
