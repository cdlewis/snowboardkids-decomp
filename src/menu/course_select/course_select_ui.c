#include "common.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/course_select/course_select_ui.h"
#include "game/menu/course_select/course_select_shop_ui.h"
#include "game/math/fixed_point_math.h"
#include "game/race/player/race_player_model_renderer.h"

typedef struct {
    u8 pad0[0x20];
    /* 0x20 */ s32 screenState;
} CourseSelectState;

typedef struct {
    /* 0x000 */ u8 pad0[5];
    /* 0x005 */ u8 selectedCharacterId;
    union {
        struct {
            /* 0x006 */ u8 menuSelection;
            /* 0x007 */ u8 selectionUnlockState;
            /* 0x008 */ u8 menuState;
            /* 0x009 */ u8 pad9[8];
            /* 0x011 */ u8 characterId;
            /* 0x012 */ u8 pad12[0x5FA];
        };
        /* 0x006 */ u8 courseIndex;
        /* 0x006 */ u8 pad6[0x606];
    };
} CourseSelectRacePlayer;

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
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ s32 matrix;
    /* 0x01C */ FixedTransform sourceTransform;
    /* 0x03C */ FixedTransform playerTransforms[4];
    /* 0x0BC */ Vec3i vecs[4];
    /* 0x0EC */ u16 angle[4];
    /* 0x0F4 */ s16 targetCourse[4];
    /* 0x0FC */ u8 timer[4];
    /* 0x100 */ u8 state[4];
} CourseSelectAnimatedActor;

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

extern void addRenderCallback(void *, void (*)(CourseSelectWidgetActor *), CourseSelectWidgetActor *);
extern void drawCourseSelectPlayerPanels(CourseSelectWidgetActor *);
extern s32 allocFixedTransformMatrix(FixedTransform *);
extern s8 gCourseUnlockSaveSlots[][0x78F8];
extern u8 D_800EC9C0;
extern u8 gRaceSplitscreenMode;
extern u8 gCourseSelectModeSelection;
extern s32 gActiveMenuTask;
extern CallbackTask *D_8010ADE0;
extern CallbackTask *D_8010ADE4;
extern s32 D_8010ADE8;
extern s8 D_8010AE64[];
extern u8 D_8010AECC[];
extern u16 D_8010AED0;
extern s32 D_8010AEE8[];
extern u8 D_8010AEFB[];
extern u8 D_8010AEA4[];
extern u8 D_8010AEB0;
extern u8 D_8010AEA0[];
extern u8 gCourseSelectExtraCourseColumnState;
extern u8 gCourseSelectStatus[];
extern u8 D_8010AEAC[];
extern u8 D_8010AF1C;
extern s16 gMenuChoicePromptState[];
extern u8 gCourseSelectCompleteText[];
extern u8 gCourseSelectCourseSpeedRatings[];
extern u8 gCourseSelectCourseHandlingRatings[];
extern u8 gCourseSelectCourseTrickRatings[];
extern s16 gCourseSelectIconListYLayout[][6];
extern s16 gCourseSelectIconListXLayout[][4];
extern s16 gCourseSelectStatsPanelLayout[][4];
extern s16 gCourseSelectStatsPlayerMarkerLayout[][2];
extern s16 gAssetHandles[];
extern u8 gPlayerCount;
extern CourseSelectRacePlayer gRacePlayers[];
extern u8 gCourseSelectSelectedCourseId[][sizeof(CourseSelectRacePlayer)];
extern u8 gMenuTransitionState;
extern s32 gMenuFlowState;
extern CourseSelectState *gCurrentGameTask;
extern s32 gMenuRenderCallbackList;
extern s32 gModelRenderCallbackList;
extern u8 gCurrentViewportIndex;
extern u8 gGameSaveDataBuffer[][0x78F8];

void drawCourseSelectPreviewModel(CourseSelectCoursePreviewActor *arg0) {
    u8 sp2F;
    unsigned char sp2E;
    u8 var_t0;
    s8 temp_v0_2;
    CourseSelectRacePlayer *temp_v0_3;
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
                var_a3 = gCourseSelectSelectedCourseId[var_t0][0];
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
            if (temp_v0_3->courseIndex >= 9) {
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

// updateCourseSelectPreviewModelIn best match: 98.638% (nonmatchings/updateCourseSelectPreviewModelIn-1219509448159986855/base.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_ui/updateCourseSelectPreviewModelIn.s")

#ifdef NON_MATCHING
void updateCourseSelectPreviewModelIn(void *arg0) {
    CourseSelectAnimatedActor *actor;
    CourseSelectStatusOverlay *courseSelectStatus;
    CourseSelectRacePlayer *player;
    s32 i;
    s32 slideStep;
    s32 requestedState;
    s32 state;
    Vec3i rotatedPosition;
    s32 switchState;
    s32 case2ReachedStep;

    actor = arg0;
    courseSelectStatus = (CourseSelectStatusOverlay *)gCourseSelectStatus;
    i = 0;
    if ((s32)gPlayerCount > 0) {
        do {
            player = &gRacePlayers[i];
            requestedState = courseSelectStatus->incomingPreviewModelState[i];
            state = actor->state[i];
            if (requestedState != state) {
                actor->state[i] = requestedState;
                actor->timer[i] = ((CourseSelectStatusOverlay *)gCourseSelectStatus)->incomingPreviewModelTimer[i];
                actor->angle[i] = ((CourseSelectStatusOverlay *)gCourseSelectStatus)->incomingPreviewModelAngle[i];
                ((CourseSelectStatusOverlay *)gCourseSelectStatus)->incomingPreviewModelTimer[i] = 0;
                ((CourseSelectStatusOverlay *)gCourseSelectStatus)->incomingPreviewModelAngle[i] = 0;
                state = actor->state[i];
            }

            if (gMenuFlowState != 0 && state < 5) {
                actor->state[i] = 4;
                actor->angle[i] = 0;
                state = actor->state[i];
            }

            switch (actor->state[i]) {
            case 0:
                actor->vecs[i].x += -0x200000;
                if (actor->vecs[i].x <= 0) {
                    actor->vecs[i].x = 0;
                    actor->state[i] = 1;
                }
                state = actor->state[i];
                break;
            case 1:
                if (D_8010AECC[i] & 1) {
                    if (D_8010AEE8[i] < 0) {
                        if ((s32)player->characterId >= 9) {
                            actor->targetCourse[i] = 2;
                        } else {
                            actor->targetCourse[i] = player->menuSelection % 3 - 1;
                        }
                    } else {
                        actor->targetCourse[i] = player->menuSelection % 3 + 1;
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
                    state = actor->state[i];
                } else if (gCurrentGameTask->screenState == 9) {
                    actor->state[i] = 8;
                    state = actor->state[i];
                }
                break;
            case 2:
                slideStep = 0x200000;
                if (D_8010AEE8[i] < 0) {
                    slideStep = -0x200000;
                }
                actor->vecs[i].y += slideStep;
                case2ReachedStep = D_8010AEE8[i] == slideStep;
                if (case2ReachedStep) {
                    actor->state[i] = 1;
                }
                // Compiler-shaping narrowing retained for IDO register allocation.
                state = ((((actor->state[i] & 0xFFFFU) & 0xFFFFU) & 0xFFFFU) & 0xFFFFU) & 0xFFFFU;
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
                state = actor->state[i];
                break;
            case 4:
                if (player->menuState == 3) {
                    actor->state[i] = 5;
                    state = actor->state[i];
                }
                break;
            case 5:
                actor->vecs[i].x += 0x200000;
                if (actor->vecs[i].x == 0x1000000) {
                    actor->state[i] = 6;
                }
                state = actor->state[i];
                break;
            case 6:
                player->menuState = 4;
                if ((i == 2) && (gPlayerCount == 3)) {
                    gRacePlayers[3].menuState = 4;
                }
                state = actor->state[i];
                break;
            case 7:
                if (player->menuState == 3) {
                    actor->state[i] = 5;
                    state = actor->state[i];
                } else if (gCurrentGameTask->screenState == 9) {
                    actor->state[i] = 8;
                    state = actor->state[i];
                }
                break;
            case 8:
                actor->vecs[i].x += 0x200000;
                if (actor->vecs[i].x == 0xC00000) {
                    actor->state[i] = 9;
                }
                state = actor->state[i];
                break;
            case 9:
                break;
            }

            if (state != 0) {
                if ((s32)state < 3) {
                    actor->angle[i] += 0x20;
                    actor->angle[i] &= 0xFFF;
                }
            }
            makeFixedRotationY(actor->playerTransforms[i].rotation, (s16)actor->angle[i]);
            transformVec3iByFixedMatrix(actor->playerTransforms[i].rotation, &actor->vecs[i], &rotatedPosition);
            actor->playerTransforms[i].translation.x = rotatedPosition.x;
            actor->playerTransforms[i].translation.y = rotatedPosition.y;
            actor->playerTransforms[i].translation.z = rotatedPosition.z;
            ((CourseSelectStatusOverlay *)gCourseSelectStatus)->incomingPreviewModelState[i] = actor->state[i];
            i++;
        } while (i < (s32)gPlayerCount);
    }

    if ((gRacePlayers[0].menuState == 4) || (actor->state[0] == 9)) {
        removeCallbackTask(actor);
        finishCourseSelectUiTask(1);
        D_8010ADE0 = 0;
    } else {
        addRenderCallback(&gModelRenderCallbackList, (void (*)(CourseSelectWidgetActor *))drawCourseSelectPreviewModel, actor);
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
    setCallbackTaskCallback(actor, updateCourseSelectPreviewModelIn);
}

void drawCourseSelectPreviewModelClose(CourseSelectCoursePreviewActor *arg0) {
    u8 sp2F;
    unsigned char sp2E;
    u8 var_t0;
    s8 temp_v0_2;
    CourseSelectRacePlayer *temp_v0_3;
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
                var_a3 = gCourseSelectSelectedCourseId[var_t0][0];
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
            if (temp_v0_3->courseIndex >= 9) {
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

// updateCourseSelectPreviewModelOut best match: 98.730% (nonmatchings/updateCourseSelectPreviewModelOut-1219509448159986855/base_25.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_ui/updateCourseSelectPreviewModelOut.s")

#ifdef NON_MATCHING
void updateCourseSelectPreviewModelOut(void *arg0)
{
  CourseSelectAnimatedActor *actor;
  CourseSelectStatusOverlay *courseSelectStatus;
  CourseSelectRacePlayer *player;
  Vec3i rotatedPosition;
  s32 i;
  s32 slideStep;
  s32 angleIndex;
  s32 requestedState;
  s32 state;
  actor = arg0;
  courseSelectStatus = (CourseSelectStatusOverlay *) gCourseSelectStatus;
  i = 0;
 if (((s32) gPlayerCount) > 0) { angleIndex = 0; do { requestedState = courseSelectStatus->outgoingPreviewModelState[i]; state = actor->state[i]; if (requestedState != state) { actor->state[i] = requestedState;
        actor->timer[i] = courseSelectStatus->outgoingPreviewModelTimer[i];
        actor->angle[i] = *((u16 *) (&courseSelectStatus->outgoingPreviewModelAngleBytes[angleIndex]));
        courseSelectStatus->outgoingPreviewModelTimer[i] = 0;
        *((u16 *) (&courseSelectStatus->outgoingPreviewModelAngleBytes[angleIndex])) = 0;
        state = actor->state[i];
      }
      if ((gMenuFlowState != 0) && (state < 5))
      {
        actor->state[i] = 4;
        actor->angle[i] = 0;
        state = actor->state[i];
      }
      switch (state)
      {
        case 0:
          break;

        case 1:
          if (D_8010AECC[i] & 1)
        {
          player = &gRacePlayers[i];
          if (D_8010AEE8[i] < 0)
          {
            if (((s32) player->characterId) >= 9)
            {
              actor->targetCourse[i] = 2;
            }
            else
            {
              actor->targetCourse[i] = (player->menuSelection % 3) - 1;
            }
          }
          else
          {
            actor->targetCourse[i] = (player->menuSelection % 3) + 1;
          }
          if (actor->targetCourse[i] < 0)
          {
            actor->targetCourse[i] = 2;
          }
          if (actor->targetCourse[i] == 3)
          {
            actor->targetCourse[i] = D_8010AEFB[i * 4];
          }
          if (actor->targetCourse[i] == 8)
          {
            actor->targetCourse[i] = 2;
          }
          if (D_8010AECC[i] == 1)
          {
            actor->vecs[i].y = -D_8010AEE8[i];
          }
          actor->state[i] = 2;
          state = 2;
        }
        else
          if (gCurrentGameTask->screenState == 9)
        {
          actor->state[i] = 8;
          state = 8;
        }
          break;

        case 2:
          slideStep = 0x200000;
          if (D_8010AEE8[i] < 0)
        {
          slideStep = -0x200000;
        }
          actor->vecs[i].y += slideStep;
          D_8010AEE8[i] -= slideStep;
          if (D_8010AEE8[i] == 0)
        {
          actor->state[i] = 1;
          D_8010AECC[i]++;
          D_8010AECC[i] &= 3;
        }
          state = actor->state[i];
          break;

        case 3:
          actor->timer[i]++;
          if (D_800EC9C0 != 0)
        {
          D_800EC9C0 = 1;
        }
          if (actor->timer[i] == 0xF)
        {
          actor->timer[i] = 0;
          actor->state[i] = 4;
          if (gPlayerCount == 1)
          {
            gRacePlayers[0].menuState = 3;
            D_800EC9C0 = 0x10;
          }
        }
          state = actor->state[i];
          break;

        case 4:
          if (gRacePlayers[i].menuState == 3)
        {
          actor->state[i] = 5;
          state = 5;
        }
          break;

        case 5:
          actor->vecs[i].x += 0x200000;
          if (actor->vecs[i].x == 0x1000000)
        {
          actor->state[i] = 6;
        }
          state = actor->state[i];
          break;

        case 6:
          gRacePlayers[i].menuState = 4;
          if ((i == 2) && (gPlayerCount == 3))
        {
          gRacePlayers[3].menuState = 4;
        }
          state = actor->state[i];
          break;

        case 7:
          if (gRacePlayers[i].menuState == 3)
        {
          actor->state[i] = 5;
          state = 5;
        }
        else
          if (gCurrentGameTask->screenState == 9)
        {
          actor->state[i] = 8;
          state = 8;
        }
          break;

        case 8:
          actor->vecs[i].x += 0x200000;
          if (actor->vecs[i].x == 0xC00000)
        {
          actor->state[i] = 9;
          gCurrentGameTask->screenState = 0xB;
        }
          break;

        case 9:
          break;

      }

      state = actor->state[i];
      if (((s32) state) < 3)
      {
        actor->angle[i] += 0x20;
        actor->angle[i] &= 0xFFF;
      }
      makeFixedRotationY(actor->playerTransforms[i].rotation, (s16) actor->angle[i]), transformVec3iByFixedMatrix(actor->playerTransforms[i].rotation, &actor->vecs[i], &rotatedPosition), actor->playerTransforms[i].translation.x = rotatedPosition.x, actor->playerTransforms[i].translation.y = rotatedPosition.y, actor->playerTransforms[i].translation.z = rotatedPosition.z, courseSelectStatus->outgoingPreviewModelState[i] = actor->state[i], i++, angleIndex += 2;
    }
    while (i < ((s32) gPlayerCount));
  }
  if ((gRacePlayers[0].menuState == 4) || (actor->state[0] == 9))
  {
    removeCallbackTask(actor);
    finishCourseSelectUiTask(2);
    D_8010ADE4 = 0;
  }
  else
  {
    addRenderCallback(&gModelRenderCallbackList, drawCourseSelectPreviewModelClose, actor);
  }
}
#endif

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
    setCallbackTaskCallback(actor, updateCourseSelectPreviewModelOut);
}

void drawCourseSelectCourseIconList(CourseSelectIconListActor *iconList) {
    CourseSelectIconListActor *localPtrs[12];
    CourseSelectIconListActor *new_var;
    s32 i;
    s32 j;
    s32 alpha;
    s32 tileIndex;
    s32 clipOffset;
    s32 courseId;
    CourseSelectIconListActor *playerActor;
    s32 selectedCourse;
    CourseSelectRacePlayer *player;
    s16 *position;

    localPtrs[11] = iconList;
    new_var = iconList;
    i = 0;
    if ((s32)gPlayerCount > 0) {
        localPtrs[0] = new_var;
        do {
            playerActor = (CourseSelectIconListActor *)((u8 *)localPtrs[11] + i);
            j = 0;
            clipOffset = (i >= 2) * 0x88;
            if (((u8 *)localPtrs[0])[0x98] > 0) {
                localPtrs[5] = playerActor;
                do {
                    if (j < (((u8 *)localPtrs[0])[0x98] - 1)) {
                        position = (s16 *)((u8 *)new_var + (i * 10) + (j * 2));
                        if (1) {
                        }
                        player = &gRacePlayers[i];
                        alpha = 0;
                        if (gPlayerCount == 1) {
                            if (player->selectedCharacterId != 5) {
                                tileIndex = (j + 1) & 0xFFFF;
                            } else {
                                tileIndex = (j + 6) & 0xFFFF;
                            }
                        } else if (player->selectedCharacterId != 5) {
                            tileIndex = (j + 0x14) & 0xFFFF;
                        } else {
                            tileIndex = (j + 0x18) & 0xFFFF;
                        }

                        selectedCourse = D_8010AEA4[i];
                        if ((selectedCourse > 0) && (selectedCourse < 8)) {
                            courseId = player->courseIndex;
                            if (courseId >= 9) {
                                if ((j == 3) && (selectedCourse & 1)) {
                                    alpha = 0xFF;
                                }
                            } else if ((j == (courseId % 3)) && (selectedCourse & 1)) {
                                alpha = 0xFF;
                            }
                        }

                        drawMenuSpriteClipped(position[12], position[32], getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                                      (u16)tileIndex, 0x20, 0x20, 0, alpha,
                                      new_var->clipLeft - ((i >= 2) * 0x8C), iconList->clipTop, iconList->clipRight,
                                      new_var->clipBottom);
                    } else if (gPlayerCount == 1) {
                        alpha = 0;
                        position = (s16 *)((u8 *)iconList + (i * 10) + (j * 2));
                        clipOffset = (i >= 2) * 0x88;
                        if ((D_8010AEA4[i] > 0) && (D_8010AEA4[i] < 8) && (gCourseSelectExtraCourseColumnState == 1) &&
                            (D_8010AEA4[i] & 1)) {
                            alpha = 0xFF;
                        }

                        if (gRaceSplitscreenMode == 3) {
                            drawMenuSpriteClipped(position[12], position[32], getRelocatableHeapBlockBase(gAssetHandles[0x27]), 5, 0x20,
                                          0x20, 0, alpha, iconList->clipLeft - clipOffset, new_var->clipTop,
                                          new_var->clipRight, new_var->clipBottom);
                        } else {
                            drawMenuSpriteClipped(position[12], position[32], getRelocatableHeapBlockBase(gAssetHandles[0x21]), 5, 0x20,
                                          0x20, 0, alpha, iconList->clipLeft - clipOffset, iconList->clipTop,
                                          iconList->clipRight, new_var->clipBottom);
                        }
                    }
                    j++;
                } while (j < localPtrs[5]->itemCounts[0]);
            }
            i++;
            localPtrs[0] = (CourseSelectIconListActor *)((u8 *)localPtrs[0] + 1);
        } while (i < (s32)gPlayerCount);
    }
}

// updateCourseSelectCourseIconList best match: 97.662% (nonmatchings/updateCourseSelectCourseIconList-5787290371232622032/base_23.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_ui/updateCourseSelectCourseIconList.s")

#ifdef NON_MATCHING
void updateCourseSelectCourseIconList(CourseSelectIconListActor *arg0) {
    CourseSelectIconListActor *base;
    CourseSelectIconListActor *actor;
    CourseSelectIconListActor *countBase;
    CourseSelectIconListActor *stateActor;
    s32 playerIndex;
    s32 i;
    CourseSelectIconListActor * volatile saved;
    s32 movingCount;
    void (*initOut)(void *);
    void (*initBackdrop)(void *);
    volatile u8 maxItems;

    saved = (base = arg0);
    playerIndex = 0;
    if ((s32)gPlayerCount > 0) {
        actor = arg0;
        do {
            if (gCurrentGameTask->screenState == 1) {
                actor->state[0] = 9;
            }
            if ((gCurrentGameTask->screenState == 3) && (actor->state[0] < 0xB)) {
                actor->state[0] = 0xB;
            }
            if (gCurrentGameTask->screenState == 9) {
                actor->state[0] = 0xD;
            }
            if ((gMenuFlowState != 0) && (actor->state[0] < 4)) {
                actor->state[0] = 4;
            }

            switch (actor->state[0]) {
            case 0:
                movingCount = (i = 0);
                if ((s32)actor->itemCounts[0] > 0) {
                    countBase = saved;
                    do {
                        if ((base->y[playerIndex][i] + 1) <= base->targetY[playerIndex]) {
                            base->y[playerIndex][i] += 0x10;
                            movingCount++;
                            if (base->y[playerIndex][i] >= base->targetY[playerIndex]) {
                                base->y[playerIndex][i] = base->targetY[playerIndex];
                            }
                        }
                        i++;
                    } while (i < (s32)countBase->itemCounts[playerIndex]);
                }
                actor->timer[0]++;
                if ((gRacePlayers[playerIndex].state == 5) || (D_8010AEA0[playerIndex] == 0) ||
                    (gCourseSelectModeSelection == 1)) {
                    maxItems = 4;
                } else {
                    maxItems = 5;
                }
                if (!(actor->timer[0] & 1) && ((s32)actor->itemCounts[0] < maxItems)) {
                    actor->itemCounts[0]++;
                }
                if (movingCount == 0) {
                    actor->state[0] = 1;
                    if (gPlayerCount == 1) {
                        initOut = initCourseSelectPreviewModelOut;
                        initBackdrop = initCourseSelectCourseListBackdrop;
                        D_8010ADE0 = createCallbackTask(initCourseSelectPreviewModelIn, 0, 0x63);
                        D_8010ADE4 = createCallbackTask(initOut, 0, 0x62);
                        createCallbackTask(initBackdrop, 0, 0x60);
                    } else if (playerIndex == 0) {
                        initOut = initCourseSelectCourseCursors;
                        createCallbackTask(initCourseSelectCourseStats, 0, 0x62);
                        gActiveMenuTask = createCallbackTask(initOut, 0, 0x64);
                    }
                }
                break;
            case 1:
                if (*(&gMenuTransitionState + (playerIndex * sizeof(CourseSelectRacePlayer))) == 1) {
                    actor->state[0] = 2;
                }
                break;
            case 2:
                i = 0;
                if ((s32)D_8010AEA4[playerIndex] >= 9) {
                    if ((s32)actor->itemCounts[0] > 0) {
                        countBase = saved;
                        do {
                            if ((i != D_8010AE64[playerIndex]) != 0) {
                                base->y[playerIndex][i] -= 0x20;
                            }
                            i++;
                        } while (i < (s32)countBase->itemCounts[playerIndex]);
                    }
                    if (D_8010AE64[playerIndex] != 0) {
                        if (base->startY[playerIndex] >= base->y[playerIndex][0]) {
                            actor->state[0] = 3;
                        }
                    } else if (base->startY[playerIndex] >= base->y[playerIndex][1]) {
                        actor->state[0] = 3;
                    }
                    if (actor->state[0] == 3) {
                        if (base->x[playerIndex][D_8010AE64[playerIndex]] < base->targetX[playerIndex]) {
                            base->direction[playerIndex] = 1;
                        } else {
                            base->direction[playerIndex] = -1;
                        }
                    }
                }
                break;
            case 3:
                base->x[playerIndex][D_8010AE64[playerIndex]] += actor->speed[0] * base->direction[playerIndex];
                if (((base->direction[playerIndex] == 1) &&
                     (base->x[playerIndex][D_8010AE64[playerIndex]] >= base->targetX[playerIndex])) ||
                    ((base->direction[playerIndex] == -1) &&
                     (base->targetX[playerIndex] >= base->x[playerIndex][D_8010AE64[playerIndex]]))) {
                    base->x[playerIndex][D_8010AE64[playerIndex]] = base->targetX[playerIndex];
                    actor->state[0] = 4;
                    gMenuChoicePromptState[playerIndex] = 1;
                }
                break;
            case 4:
                if ((gMenuFlowState == 0) && (gMenuChoicePromptState[playerIndex] == 0)) {
                    actor->state[0] = 5;
                }
                if (gRacePlayers[playerIndex].pad6[2] == 3) {
                    stateActor = actor;
                    stateActor->state[0] = 7;
                }
                break;
            case 5:
                base->x[playerIndex][D_8010AE64[playerIndex]] += actor->speed[0] * base->direction[playerIndex] * -1;
                if (((base->direction[playerIndex] == 1) &&
                     (((D_8010AE64[playerIndex] * actor->speed[0]) + base->baseX[playerIndex]) >=
                      base->x[playerIndex][D_8010AE64[playerIndex]])) ||
                    ((base->direction[playerIndex] == -1) &&
                     (base->x[playerIndex][D_8010AE64[playerIndex]] >=
                      (base->baseX[playerIndex] + (actor->speed[0] * D_8010AE64[playerIndex]))))) {
                    base->x[playerIndex][D_8010AE64[playerIndex]] =
                        (D_8010AE64[playerIndex] * actor->speed[0]) + base->baseX[playerIndex];
                    actor->state[0] = 6;
                }
                break;
            case 6:
                i = 0;
                if ((s32)actor->itemCounts[0] > 0) {
                    countBase = saved;
                    do {
                        if ((i != D_8010AE64[playerIndex]) != 0) {
                            base->y[playerIndex][i] += 0x20;
                        }
                        i++;
                    } while (i < (s32)countBase->itemCounts[playerIndex]);
                }
                if (D_8010AE64[playerIndex] != 0) {
                    if (base->y[playerIndex][0] >= base->targetY[playerIndex]) {
                        actor->state[0] = 1;
                    }
                } else if (base->y[playerIndex][1] >= base->targetY[playerIndex]) {
                    actor->state[0] = 1;
                }
                if (actor->state[0] == 1) {
                    *(&gMenuTransitionState + (playerIndex * sizeof(CourseSelectRacePlayer))) = 0;
                }
                break;
            case 7:
                if (gMenuFlowState != 0) {
                    i = 0;
                    if ((s32)actor->itemCounts[0] > 0) {
                        do {
                            base->y[playerIndex][i] -= 0x20;
                            i++;
                        } while (i < (s32)actor->itemCounts[0]);
                    }
                } else {
                    base->y[playerIndex][D_8010AE64[playerIndex]] -= 0x20;
                }
                if (base->startY[playerIndex] >= base->y[playerIndex][D_8010AE64[playerIndex]]) {
                    actor->state[0] = 8;
                }
                break;
            case 9:
                base->y[playerIndex][D_8010AE64[playerIndex]] -= 0x20;
                if (base->startY[playerIndex] >= base->y[playerIndex][D_8010AE64[playerIndex]]) {
                    actor->state[0] = 0xA;
                }
                break;
            case 11:
                base->y[playerIndex][D_8010AE64[playerIndex]] += 0x20;
                if (base->y[playerIndex][D_8010AE64[playerIndex]] >= base->targetY[playerIndex]) {
                    base->y[playerIndex][D_8010AE64[playerIndex]] = base->targetY[playerIndex];
                    actor->state[0] = 0xC;
                }
                break;
            case 12:
                if (gCurrentGameTask->screenState == 4) {
                    actor->state[0] = 4;
                }
                break;
            case 13:
                break;
            }
            playerIndex++;
            actor = (CourseSelectIconListActor *)((u8 *)actor + 1);
        } while (playerIndex < (s32)gPlayerCount);
    }

    if ((base->state[0] == 8) || (base->state[0] == 0xD)) {
        removeCallbackTask(base);
        finishCourseSelectUiTask(3);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (void (*)(CourseSelectWidgetActor *))drawCourseSelectCourseIconList,
                      (CourseSelectWidgetActor *)base);
}
#endif

// initCourseSelectCourseIconList best match: 96.310% (nonmatchings/initCourseSelectCourseIconList-6934502587000073416/base_19.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_ui/initCourseSelectCourseIconList.s")

#ifdef NON_MATCHING
void initCourseSelectCourseIconList(CourseSelectIconListActor *arg0) {
    CourseSelectIconListActor *actor;
    u8 *courseUnlocked;
    s16 *yLayout;
    s16 *xLayout;
    s16 hasExtraCourse;
    s32 isCharacterFive;
    s32 playerIndex;
    s32 layoutIndex;
    s32 iconIndex;

    actor = arg0;
    if ((s32)gPlayerCount < 3) {
        layoutIndex = gPlayerCount - 1;
    } else {
        layoutIndex = 2;
    }

    playerIndex = 0;
    if ((s32)gPlayerCount > 0) {
        courseUnlocked = D_8010AEA0;
        yLayout = gCourseSelectIconListYLayout[layoutIndex];
        xLayout = gCourseSelectIconListXLayout[layoutIndex];
        do {
            if (*courseUnlocked != 0) {
                isCharacterFive = gRacePlayers[playerIndex].selectedCharacterId == 5;
                hasExtraCourse = 1;
                if (isCharacterFive) {
                    hasExtraCourse = 0;
                }
            } else {
                hasExtraCourse = 0;
            }

            actor->speed[playerIndex] = yLayout[hasExtraCourse];
            actor->baseX[playerIndex] = yLayout[((playerIndex & 1) * 2) + hasExtraCourse + 2];
            actor->targetX[playerIndex] = yLayout[((playerIndex & 1) * 2) + 2];
            actor->targetY[playerIndex] = xLayout[((playerIndex >= 2) * 2) + 1];
            actor->startY[playerIndex] = xLayout[(playerIndex >= 2) * 2];

            actor->clipTop = 0x78;
            actor->clipRight = 0xA0;
            actor->clipBottom = 0x78;
            if (gPlayerCount == 1) {
                actor->clipLeft = 0xA0;
            } else if (gPlayerCount == 2) {
                actor->clipLeft = 0x7E;
            } else {
                actor->clipLeft = 0x88;
            }

            iconIndex = 0;
            do {
                actor->y[playerIndex][iconIndex] = actor->startY[playerIndex];
                actor->x[playerIndex][iconIndex] =
                    actor->baseX[playerIndex] + (actor->speed[playerIndex] * iconIndex);
                iconIndex++;
            } while (iconIndex < 5);
            actor->timer[playerIndex] = 0;
            actor->itemCounts[playerIndex] = 1;
            actor->state[playerIndex] = 0;

            courseUnlocked++;
            playerIndex++;
        } while (playerIndex < (s32)gPlayerCount);
    }
    setCallbackTaskCallback(arg0, updateCourseSelectCourseIconList);
}
#endif

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
                                            getRelocatableHeapBlockBase(handles[handleIndex]), tileIndex & (0xFFFF ^ 0),
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
    CourseSelectRacePlayer *player;
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
            gCourseSelectStatus[i - 1] = state;
        } while (i < (s32) gPlayerCount);
    }

    if (actor->courseCursorState[0] == 4) {
        removeCallbackTask(actor);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, drawCourseSelectCourseCursors, actor);
    }
}

// initCourseSelectCourseCursors best match: 97.456% (nonmatchings/initCourseSelectCourseCursors-3549320994361659932/base_18.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_ui/initCourseSelectCourseCursors.s")

#ifdef NON_MATCHING
void initCourseSelectCourseCursors(CourseSelectWidgetActor *arg0) {
    CourseSelectWidgetActor *actor = arg0;
    s32 layoutIndex;
    s32 idx;
    s32 i;
    u8 *courseUnlocked;
    s16 *xLayout;
    char mask;
    s32 unlockState;

    mask = 0xFFFFFFFFFFFFFFFFu;
    if (actor && actor) {
    }
    if (gPlayerCount < 3) {
        idx = gPlayerCount - 1;
    } else {
        idx = 2;
    }
    i = 0;
    layoutIndex = idx;
    if ((s32) gPlayerCount > 0) {
        s16 *yLayout;
        yLayout = gCourseSelectIconListYLayout[layoutIndex];
        xLayout = gCourseSelectIconListXLayout[layoutIndex]; courseUnlocked = D_8010AEA0; do {
            unlockState = *courseUnlocked & mask;
            courseUnlocked++;
            if ((0, unlockState) != 0) {
                idx = 1;
                if (gRacePlayers[i].selectedCharacterId == 5) {
                    idx = 0;
                }
            } else {
                idx = 0;
            }
            if ((i && i) && i) {
            }
            actor->courseCursorBobOffset[i] = yLayout[idx];
            actor->courseCursorY[i] = yLayout[((i & 1) * 2) + idx + 2];
            actor->courseCursorX[i] = xLayout[((i >= 2) * 2) + 1];
            actor->courseCursorAlpha[i] = 0;
            actor->courseCursorState[i] = 0;
            actor->courseCursorTimer[i] = 0;
            i++;
        } while (i < (s32) gPlayerCount);
    }
    setCallbackTaskCallback(actor, updateCourseSelectCourseCursors);
}
#endif

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
            gActiveMenuTask = createCallbackTask(initCourseSelectCourseCursors, 0, 0x64);
            createCallbackTask(initCourseSelectExtraCourseBadge, 0, 0x63);
            createCallbackTask(initCourseSelectCourseDescription, 0, 0x61);
            createCallbackTask(initCourseSelectCourseStats, 0, 0x64);
            if ((gRaceSplitscreenMode == 3) && (gCourseSelectModeSelection == 0)) {
                createCallbackTask(initShopMenuMoneyPanelForCourseSelectReturn, 0, 0x63);
            }
        }
        break;
    case 1:
        if (gMenuTransitionState == 3) {
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

    addRenderCallback(&gMenuRenderCallbackList, drawCourseSelectCourseListBackdrop, arg0);
}

void initCourseSelectCourseListBackdrop(CourseSelectWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = 8;
    setCallbackTaskCallback(arg0, updateCourseSelectCourseListBackdrop);
}

// drawCourseSelectCourseStats best match: 98.714% (nonmatchings/drawCourseSelectCourseStats-4588856335748919862/base_13.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_ui/drawCourseSelectCourseStats.s")

#ifdef NON_MATCHING
extern int sprintf(u8 *, u8 *, ...);
extern u8 D_800E0DA0[];
extern u8 D_800E0DA4[];
extern u8 D_800E0DA8[];
extern u8 D_800E0DAC[];
extern u8 D_800E0DB0[];
extern u8 D_800E0DB4[];

void drawCourseSelectCourseStats(CourseSelectWidgetActor *arg0) {
    s32 playerCount;
    s32 i;
    s32 courseId;
    s32 selectedCourseId;
    s32 j;
    s32 xOffset;
    s32 positionColumn;
    s32 bottomRow;
    volatile u16 frameTile;
    u8 text[2];
    u8 *ratings;
    CourseSelectWidgetActor *playerWidget;

    playerCount = gPlayerCount;
    if (playerCount < 3) {
        frameTile = 0x10;
    } else {
        frameTile = 0x11;
    }

    i = 0;
    if (playerCount > 0) {
        do {
            if ((playerCount >= 2 || gRacePlayers[i].pad6[2] == 0) &&
                (playerCount >= 2 || (playerCount == 1 && gCourseSelectExtraCourseColumnState == 0))) {
                playerWidget = (CourseSelectWidgetActor *)((u8 *)arg0 + (i * 2));
                courseId = gRacePlayers[i].pad6[0];
                if (!playerCount) {
                }
                if (courseId >= 9 && gRacePlayers[i].pad6[0] < 12) {
                    xOffset = (u16)courseId;
                } else {
                    xOffset = (u16)(courseId % 3);
                }

                drawMenuSpriteWithAlpha(playerWidget->coordinates[0], playerWidget->coordinates[4],
                              getRelocatableHeapBlockBase(gAssetHandles[0x21]), frameTile, 0x20, 0x20, 0, arg0->coordinates[8], 0);

                if (gPlayerCount < 3) {
                    selectedCourseId = xOffset;
                    if (selectedCourseId >= 9) {
                        drawMenuAsciiText((s16)(playerWidget->coordinates[0] + 0x38), playerWidget->coordinates[4],
                                      D_800E0DA0, 0, arg0->coordinates[8]);
                    } else {
                        ratings = &gCourseSelectCourseSpeedRatings[selectedCourseId * 3];
                        j = 0;
                        if (ratings[0] > ((0, 0))) {
                            xOffset = 0;
                            do {
                                drawMenuSpriteWithAlpha((s16)(playerWidget->coordinates[0] + xOffset + 0x38),
                                              (s16)(playerWidget->coordinates[4] - 2),
                                              getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0x25, 0x20, 0x20, 0,
                                              arg0->coordinates[8], 0);
                                j++;
                                xOffset += 0xC;
                            } while (j < ratings[0]);
                        }
                    }

                    if (selectedCourseId >= 9) {
                        drawMenuAsciiText((s16)(playerWidget->coordinates[0] + 0x38),
                                      (s16)(playerWidget->coordinates[4] + 0xC), D_800E0DA4, 0, arg0->coordinates[8]);
                    } else {
                        ratings = &gCourseSelectCourseSpeedRatings[selectedCourseId * 3];
                        if (1) {
                        }
                        if (1) {
                        }
                        j = 0;
                        if (ratings[1] > 0) {
                            xOffset = 0;
                            do {
                                drawMenuSpriteWithAlpha((s16)(playerWidget->coordinates[0] + xOffset + 0x38),
                                              (s16)(playerWidget->coordinates[4] + 0xA),
                                              getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0x25, 0x20, 0x20, 0,
                                              arg0->coordinates[8], 0);
                                j++;
                                xOffset += 0xC;
                            } while (j < ratings[1]);
                        }
                    }

                    if (selectedCourseId >= 9) {
                        drawMenuAsciiText((s16)(playerWidget->coordinates[0] + 0x38),
                                      (s16)(playerWidget->coordinates[4] + 0x18), D_800E0DA8, 0, arg0->coordinates[8]);
                    } else {
                        ratings = &gCourseSelectCourseSpeedRatings[selectedCourseId * 3];
                        j = 0;
                        if (ratings[2] > 0) {
                            xOffset = 0;
                            do {
                                drawMenuSpriteWithAlpha((s16)(playerWidget->coordinates[0] + xOffset + 0x38),
                                              (s16)(playerWidget->coordinates[4] + 0x16),
                                              getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0x25, 0x20, 0x20, 0,
                                              arg0->coordinates[8], 0);
                                j++;
                                xOffset += 0xC;
                            } while (j < ratings[2]);
                        }
                    }
                } else {
                    selectedCourseId = xOffset;
                    if (selectedCourseId >= 9) {
                        text[0] = '?';
                        text[1] = 0;
                    }
                    xOffset++;
                    xOffset--;
                    if (selectedCourseId < 9) {
                        playerWidget = playerWidget;
                        sprintf(text, D_800E0DAC, gCourseSelectCourseSpeedRatings[selectedCourseId * 3]);
                        do {
                        } while (0);
                    }
                    drawMenuAsciiText((s16)(playerWidget->coordinates[0] + 0x34), playerWidget->coordinates[4], text, 0,
                                  arg0->coordinates[8]);

                    if (selectedCourseId < 9) {
                        sprintf(text, D_800E0DB0, gCourseSelectCourseHandlingRatings[selectedCourseId * 3]);
                    }
                    drawMenuAsciiText((s16)(playerWidget->coordinates[0] + 0x34),
                                  (s16)(playerWidget->coordinates[4] + 8), text, 0, arg0->coordinates[8]);

                    if (9 > selectedCourseId) {
                        sprintf(text, D_800E0DB4, gCourseSelectCourseTrickRatings[selectedCourseId * 3]);
                    }
                    drawMenuAsciiText((s16)(playerWidget->coordinates[0] + 0x34),
                                  (s16)(playerWidget->coordinates[4] + 0x10), text, 0, arg0->coordinates[8]);
                }

                playerCount = gPlayerCount;
                if (playerCount >= 2) {
                    positionColumn = 1;
                    if (playerCount == 2) {
                        positionColumn = 0;
                    }
                    bottomRow = i >= 2;
                    drawMenuSpriteWithAlpha((s16)(gCourseSelectStatsPlayerMarkerLayout[positionColumn * 2 + (i & 1)][0] + (bottomRow * 0x8C)),
                                  gCourseSelectStatsPlayerMarkerLayout[positionColumn * 2 + (i & 1)][1],
                                  getRelocatableHeapBlockBase(gAssetHandles[0x21]), 0x12, 0x20, 0x20, 0, arg0->coordinates[8], 0);
 playerCount = gPlayerCount; } } i++;
        } while (i < playerCount);
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
        if (gMenuTransitionState == 3) {
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

    addRenderCallback(&gMenuRenderCallbackList, drawCourseSelectCourseStats, arg0);
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
    setCallbackTaskCallback(temp_a3, updateCourseSelectCourseStats);
}

// drawCourseSelectCourseDescription best match: 92.819% (nonmatchings/drawCourseSelectCourseDescription-5787290371232622032/base_71.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_ui/drawCourseSelectCourseDescription.s")

#ifdef NON_MATCHING
typedef struct {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ u8 courseId;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ u8 mode;
} CourseSelectSelection26C4C;

typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ u16 unk2C;
    /* 0x2E */ u8 unk2E;
} CourseSelectStatus26C4C;

extern u32 gCourseUnlockPrices[];
extern u8 gCourseSelectModeDescriptionText[];
extern u8 gCourseSelectBoardLevelText[];
extern u8 gCourseSelectBoardLevelByCourseText[];
extern u8 gCourseSelectExtraCourseBoardLevelText[];
extern u8 gCourseSelectPurchaseMessageText[];
extern u8 gUnlockedExtraCourseFlags;
extern u8 gCourseSelectExtraCourseIds[];

void drawCourseSelectCourseDescription(CourseSelectWidgetActor *arg0) {
    volatile CourseSelectStatus26C4C *status = (CourseSelectStatus26C4C *)gCourseSelectStatus;
    u8 *volatile text;
    volatile CourseSelectSelection26C4C *selection;
    s16 buffer[8];
    s32 value;
    s32 courseId;
    s32 digitCount;
    u32 price;
    s16 *digits;
    u16 selectedIndex;
    u16 splitMode;

    selection = (CourseSelectSelection26C4C *)gRacePlayers;
    price = 3;
    digitCount = status->unk2C;
    value = digitCount;
    if ((u32)value == 0) {
        s32 courseIndex;
        volatile s32 padSelected;

        if ((D_8010AEA8 == 0) && ((value = selection->mode) == 0 || (value == 3) || (value == 9))) {
            value = status->unk2E;
            if (value == 1) {
                value = price;
            } else if (value == 2) {
                value = 4;
            } else if ((courseId = selection->courseId, courseId >= 9) && (courseId < 12)) {
                value = 5;
            } else {
                value = (courseId % 3) & 0xFFFFU;
            }
            text = gCourseSelectModeDescriptionText + (value * 0x46);
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

            splitMode = gRaceSplitscreenMode;
            if ((splitMode == 3) && ((courseId = selection->courseId) <= 8)) {
                text = gCourseSelectBoardLevelByCourseText + ((courseId % 3) * 0x30);
            } else if (((courseId = selection->courseId) >= 9) && (courseId < 12)) {
                text = gCourseSelectExtraCourseBoardLevelText + ((gCourseSelectExtraCourseIds[selectedIndex] % 3) * 0x60);
            } else {
                text = gCourseSelectBoardLevelText;
            }
        }

        drawMenuGlyphScript(arg0->x, arg0->y, text, 1, arg0->spriteIndex, 0);

        if ((gRaceSplitscreenMode == 3) && (((value = selection->mode) == 1) || (value == 2))) {
            if ((D_8010AE64[0] != 3) || !(gUnlockedExtraCourseFlags & 7)) {
                buffer[0] = -4;
                buffer[1] = 6;
                buffer[2] = selectedIndex;
                buffer[3] = -1;
                drawMenuGlyphScript((s16)(arg0->x + 0x48), (s16)(arg0->y + 0x10), (u8 *)buffer, 1, arg0->spriteIndex, 0);
            }

            if (gCourseSelectModeSelection == 0) {
                buffer[0] = -4;
                buffer[1] = 6;
                if ((selectedIndex >= 2) || (selection->courseId >= 9)) {
                    if (selection->courseId >= 9) {
                        courseIndex = gCourseSelectExtraCourseIds[selectedIndex];
                    } else {
                        courseIndex = ((selection->courseId % 3) + (selectedIndex * 3)) - 3;
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
                    if (selection->courseId >= 9) {
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
                drawMenuGlyphScript((s16)(arg0->x + 0x20), (s16)(arg0->y + 0x20), (u8 *)buffer, 1, arg0->spriteIndex, 0);
            }
        }
    } else {
        digitCount = arg0->spriteIndex;
        drawMenuGlyphScript(arg0->x, arg0->y, gCourseSelectPurchaseMessageText + ((value * 0x32) - 0x32), 1,
                            digitCount, 0);
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
        if (gMenuTransitionState == temp_a0) {
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

    if (gMenuTransitionState == 0) {
        arg0->pad18_2[8] = 0;
    }

    if (arg0->pad18_2[6] == temp_a0) {
        removeCallbackTask(arg0);
        finishCourseSelectUiTask(6);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCourseSelectCourseDescription, arg0);
}

void initCourseSelectCourseDescription(CourseSelectWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = 0xC;
    arg0->spriteIndex = 0;
    arg0->state = 0;
    arg0->subState = 0;
    arg0->timer = 0;
    setCallbackTaskCallback(arg0, updateCourseSelectCourseDescription);
}

void drawCourseSelectExtraCourseBadge(CourseSelectWidgetActor *arg0) {
    u16 tileIndexSpill;
    s32 handleIndex;
    u16 unusedPadding;
    u16 tileIndex;
    u32 promotedTileIndex;

    if (gCourseSelectModeSelection == 1) {
        tileIndex = 1;
        handleIndex = 0x27;
    } else {
        tileIndex = 0;
        handleIndex = 0x21;
    }

    tileIndexSpill = tileIndex;
    promotedTileIndex = tileIndexSpill;
    drawMenuSpriteWithAlpha((*arg0).coordinates[0], arg0->coordinates[1], getRelocatableHeapBlockBase(gAssetHandles[handleIndex]),
                  promotedTileIndex, 0x20, 0x20, 0, arg0->coordinates[2], 0);
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
        if ((gMenuTransitionState == 3) || (gCurrentGameTask->screenState == 9)) {
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
    addRenderCallback(&gMenuRenderCallbackList, drawCourseSelectExtraCourseBadge, arg0);
}

void initCourseSelectExtraCourseBadge(CourseSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x5C;
    arg0->spriteIndex = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, updateCourseSelectExtraCourseBadge);
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
        setCallbackTaskCallback(temp_a2, updateCourseSelectExtraCourseIconList);
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCourseSelectExtraCourseIconList, temp_a2);
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
            setCallbackTaskCallback(arg0, updateCourseSelectExtraCourseIconListIn);
            var_v0_3 = gCurrentGameTask->screenState;
        }
    }
    if (var_v0_3 == 9) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCourseSelectExtraCourseIconList, arg0);
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

    addRenderCallback(&gMenuRenderCallbackList, drawCourseSelectExtraCourseIconList, temp_a2);
}

// updateCourseSelectExtraCourseIconList best match: 97.539% (nonmatchings/updateCourseSelectExtraCourseIconList-5787290371232622032/base_14.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_ui/updateCourseSelectExtraCourseIconList.s")

#ifdef NON_MATCHING
void updateCourseSelectExtraCourseIconList(CourseSelectWidgetActor *arg0) {
    CourseSelectExtraCourseIconListActor *actor;
    s32 unlockMask;
    CourseSelectRacePlayer *player;
    s16 *promptState;
    s32 playerIndex;
    s32 iconIndex;
    s16 direction;
    s32 layoutIndex;
    s32 itemCount;
    s32 nextItemCount;

    actor = (CourseSelectExtraCourseIconListActor *)arg0;
    if (gMenuTransitionState == 3) {
        setCallbackTaskCallback(arg0, updateCourseSelectExtraCourseIconListClose);
        playerIndex = 0;
        if ((s32)gPlayerCount > 0) {
            do {
                ((CourseSelectExtraCourseIconListActor *)arg0)->alpha[playerIndex] = 0x100;
                playerIndex++;
            } while (playerIndex < (s32)gPlayerCount);
        }
    } else if (gCurrentGameTask->screenState == 1) {
        setCallbackTaskCallback(arg0, updateCourseSelectExtraCourseIconListOut);
        ((CourseSelectExtraCourseIconListActor *)arg0)->alpha[0] = 0x100;
        actor->pulseTimer[0] = 0;
    } else {
        playerIndex = 0;
        if ((s32)gPlayerCount > 0) {
            player = gRacePlayers;
            do {
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
                                        } else if ((s8)gGameSaveDataBuffer[playerIndex][iconIndex + 0x48] != -1) {
                                            itemCount = actor->itemCounts[playerIndex] + 1;
                                            actor->itemCounts[playerIndex] = itemCount;
                                            if (gPlayerCount == 1) {
                                                actor->tileIndices[playerIndex][itemCount - 1] = iconIndex + 0xC;
                                            } else {
                                                actor->tileIndices[playerIndex][actor->itemCounts[playerIndex] - 1] =
                                                    iconIndex + 0x1F;
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
                gCourseSelectStatus[playerIndex + 0x24] =
                    ((CourseSelectExtraCourseIconListActor *)arg0)->itemCounts[playerIndex];
                playerIndex++;
                player++;
            } while (playerIndex < (s32)gPlayerCount);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCourseSelectExtraCourseIconList, arg0);
}
#endif

// initCourseSelectExtraCourseIconList best match: 98.393% (nonmatchings/initCourseSelectExtraCourseIconList-2663524570355072948/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_ui/initCourseSelectExtraCourseIconList.s")

#ifdef NON_MATCHING
void initCourseSelectExtraCourseIconList(CourseSelectWidgetActor *arg0) {
    CourseSelectExtraCourseIconListActor *actor;
    s16 *xLayout;
    s16 *yLayout;
    s32 layoutIndex;
    s32 playerIndex;
    s32 iconIndex;
    s32 iconX;
    s32 iconXOffset;

    actor = (CourseSelectExtraCourseIconListActor *)arg0;
    if ((s32)gPlayerCount < 3) {
        layoutIndex = gPlayerCount - 1;
    } else {
        layoutIndex = 2;
    }

    if ((s32)gPlayerCount > 0) {
        xLayout = gCourseSelectIconListXLayout[layoutIndex];
        yLayout = gCourseSelectIconListYLayout[layoutIndex];
        playerIndex = 0;
        do {
            if ((s32)gPlayerCount >= 3) {
                actor->rowSpacing[playerIndex] = 0xC;
            } else {
                actor->rowSpacing[playerIndex] = yLayout[0];
            }
            actor->itemCounts[playerIndex] = 0;
            actor->alpha[playerIndex] = 0x100;

            if (gPlayerCount == 1) {
                iconXOffset = 8;
            } else {
                iconXOffset = 4;
            }
            iconX = xLayout[((playerIndex >= 2) * 2) + 1] + iconXOffset;

            iconIndex = 0;
            do {
                actor->iconX[playerIndex][iconIndex] = iconX;
                actor->iconY[playerIndex][iconIndex] = yLayout[((playerIndex & 1) * 2) + 2];
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
    setCallbackTaskCallback(arg0, updateCourseSelectExtraCourseIconList);
}
#endif

// drawCourseSelectPlayerPanels best match: 99.592% (nonmatchings/drawCourseSelectPlayerPanels-2188069624939011928/base_20.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_ui/drawCourseSelectPlayerPanels.s")

#ifdef NON_MATCHING
extern int sprintf(u8 *, u8 *, ...);

void drawCourseSelectPlayerPanels(CourseSelectWidgetInitActor *actor) {
    s16 *yPtr;
    s32 nextIndex;
    u8 text[4];
    volatile s32 savedIndex;
    s32 new_var;
    s32 offset;
    s32 j;
    s32 count;
    s32 middleCount;
    s32 tile;
    s32 edgeOffset;
    s32 alpha;
    s32 i;

    if (gPlayerCount == 2) {
        {
            if (1) {
            }
        }
        count = 2;
    } else {
        count = 4;
    }

    i = 0;
    if (count > 0) {
        new_var = i;
        do {
            j = 0;
            yPtr = &actor->unk20;
            if (new_var == gPlayerCount) {
                alpha = 0xC0;
            } else {
                alpha = 0x100;
            }
            if (i == 0) {
                tile = 2;
            } else {
                tile = (new_var + 5) & 0xFF;
            }

            savedIndex = new_var;
            drawMenuSpriteWithAlpha(actor->unk18, *(&actor->unk20), getRelocatableHeapBlockBase(gAssetHandles[0x21]), 0x22, 0x20, 0x20, 0,
                          alpha, tile);

            if (gPlayerCount == 2) {
                middleCount = 0xB;
            } else {
                middleCount = 3;
            }
            edgeOffset = 0x38;
            if (middleCount > 0) {
                do {
                    drawMenuSpriteWithAlpha((s16)(actor->unk18 + edgeOffset), *(&actor->unk20), getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                                  0x23, 0x20, 0x20, 0, alpha, tile);
                    j++;
                    edgeOffset += 0x10;
                } while (j != middleCount);
            }

            j = getRelocatableHeapBlockBase(gAssetHandles[0x21]);
            drawMenuSpriteWithAlpha((s16)(actor->unk18 + edgeOffset), *(&actor->unk20), j, 0x24,
                          0x20, 0x20, 0, alpha, tile);

            offset = 0;
            if (gPlayerCount == 2) {
                if (1) {
                    edgeOffset = 0xF8;
                }
            } else {
                edgeOffset = 0x78;
            }
            do {
                drawMenuSpriteWithAlpha(actor->unk18, (s16)((*(&actor->unk20)) + offset + 0x10), getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                              0x25, 0x20, 0x20, 0, alpha, tile);
                drawMenuSpriteWithAlpha((s16)(actor->unk18 + edgeOffset), (s16)((*(&actor->unk20)) + offset + 0x10),
                              getRelocatableHeapBlockBase(gAssetHandles[0x21]), 0x26, 0x20, 0x20, 0, alpha, tile);
                offset += 0x10;
            } while (offset < 0x40);

            drawMenuSpriteWithAlpha(actor->unk18, (s16)((*(&actor->unk20)) + 0x50), getRelocatableHeapBlockBase(gAssetHandles[0x21]), 0x27, 0x20,
                          0x20, 0, alpha, tile);

            if (gPlayerCount == 2) {
                middleCount = 0xE;
            } else {
                middleCount = 6;
            }
            j = 0;
            if (middleCount > 0) {
                if (new_var) {
                }
                offset = 0;
                do {
                    drawMenuSpriteWithAlpha((s16)(actor->unk18 + offset + 0x10), (s16)(actor->unk20 + 0x50),
                                  getRelocatableHeapBlockBase(gAssetHandles[0x21]), 0x28, 0x20, 0x20, 0, alpha, tile);
                    j++;
                    offset += 0x10;
                } while (j != middleCount);
            }

            drawMenuSpriteWithAlpha((s16)(actor->unk18 + edgeOffset - 8), (s16)((*(&actor->unk20)) + 0x50),
                          getRelocatableHeapBlockBase(gAssetHandles[0x21]), 0x29, 0x20, 0x20, 0, alpha, tile);

            nextIndex = savedIndex + 1;
            sprintf(text - 0x18, "%d", nextIndex);
            drawMenuAsciiText((s16)(actor->unk18 + 0x34), (s16)(*yPtr + 2), text - 0x18, 0, alpha);

            if (alpha == 0xC0) {
                drawMenuSpriteWithAlpha((s16)(actor->unk18 + 2), (s16)(actor->unk20 + 0x24),
                              getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x90, 0x20, 0x20, 0, 0xF0, 0);
            }
            i = nextIndex;
            actor = (CourseSelectWidgetInitActor *)((u8 *)actor + sizeof(s16));
            savedIndex = nextIndex;
        } while (nextIndex != count);
    }
}
#endif

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
                                D_8010ADE0 = createCallbackTask(initCourseSelectPreviewModelIn, 0, 0x62);
                                D_8010ADE4 = createCallbackTask(initCourseSelectPreviewModelOut, 0, 0x62);
                                createCallbackTask(initCourseSelectCourseIconList, 0, 0x62);
                            }
                            break;
                        }
                    } while (step != 0x18);
                    break;
                case 1:
                    if (*(&gMenuTransitionState + (i * sizeof(CourseSelectRacePlayer))) == 4) {
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

    addRenderCallback(&gMenuRenderCallbackList, drawCourseSelectPlayerPanels, arg0);
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
    setCallbackTaskCallback(arg0, updateCourseSelectPlayerPanels);
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

// updateCourseSelectCompletePanels best match: 99.468% (nonmatchings/updateCourseSelectCompletePanels-6887713755923057488/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/course_select/course_select_ui/updateCourseSelectCompletePanels.s")

#ifdef NON_MATCHING
void updateCourseSelectCompletePanels(CourseSelectWidgetActor *arg0) {
    s32 maxAlpha;
    CourseSelectPlayerPanelsActor *base;
    CourseSelectCompletePanelSource *source0;
    CourseSelectCompletePanelSource *sourceAlias;
    CourseSelectCompletePanelSource *source1;
    s32 i;
    s32 j;
    u16 alpha;
    s16 nextAlpha;
    u8 playerState;
    long long fadeStep;

    i = 0;
    source0 = (CourseSelectCompletePanelSource *)D_8010ADE0;
    source1 = (CourseSelectCompletePanelSource *)D_8010ADE4;
    maxAlpha = 0x100;
    base = (CourseSelectPlayerPanelsActor *)arg0;
    if ((s32)gPlayerCount > 0) {
        do {
            fadeStep = 0x30;
            alpha = base->playerPanelFadeAlpha[i];
            sourceAlias = source0;
            if (alpha == 0) {
                if ((sourceAlias != NULL) && ((source0->playerStates[i] == 4) || (source1->playerStates[i] == 4))) {
                    base->playerPanelFadeAlpha[i] = 1;
                }
            } else {
                if (alpha != maxAlpha) {
                    nextAlpha = fadeStep;
                    nextAlpha = alpha + nextAlpha;
                    base->playerPanelFadeAlpha[i] = nextAlpha;
                    if ((u16)(alpha + fadeStep) >= maxAlpha) {
                        base->playerPanelFadeAlpha[i] = 0x100;
                    }
                }
                playerState = gRacePlayers[i].menuState;
                if (playerState == 1) {
                    base->playerPanelFadeAlpha[i] = 0;
                }
            }
            i++;
        } while (i < (s32)gPlayerCount);
    }

    if ((s32)D_800EC9C0 == 0x10) {
        ((void (*)())removeCallbackTask)(base, 0);
        for (j = 0; j < (s32)gPlayerCount; j++) {
            gRacePlayers[j].menuState = 3;
        }
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (void (*)(CourseSelectWidgetActor *))drawCourseSelectCompletePanels,
                          (CourseSelectWidgetActor *)base);
    }
}
#endif

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
    setCallbackTaskCallback(arg0, updateCourseSelectCompletePanels);
}

void finishCourseSelectUiTask(s32 arg0) {

}
