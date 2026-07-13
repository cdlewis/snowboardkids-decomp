#include "common.h"
#include "relocatable_heap.h"
#include "callback_task_scheduler.h"
#include "menu_renderer.h"
#include "course_select_ui.h"
#include "course_select_shop_ui.h"
#include "fixed_point_math.h"
#include "race_player_model_renderer.h"

typedef struct {
    u8 pad0[0x20];
    /* 0x20 */ s32 screenState;
} CourseSelectState;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} CourseSelectTempVec3i;

typedef struct {
    /* 0x000 */ u8 pad0[5];
    /* 0x005 */ u8 state;
    union {
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
            /* 0x40 */ s16 exitTargetX;
            /* 0x42 */ u16 period[4];
            /* 0x4A */ u16 speed[4];
            /* 0x52 */ u16 timer[4];
            /* 0x5A */ u8 state[4];
        };
    };
} CourseSelectWidgetInitActor;

typedef struct {
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
} CourseSelectIconListActor;

typedef struct {
    /* 0x000 */ u8 pad0[0x3C];
    /* 0x03C */ s16 matrix[0x10];
    /* 0x05C */ u8 pad5C[0x60];
    /* 0x0BC */ CourseSelectTempVec3i vecs[4];
    /* 0x0EC */ u16 angle[4];
    /* 0x0F4 */ s16 targetCourse[4];
    /* 0x0FC */ u8 timer[4];
    /* 0x100 */ u8 state[4];
} CourseSelectAnimatedActor;

typedef struct {
    /* 0x000 */ u8 pad0[0x100];
    /* 0x100 */ u8 playerStates[4];
} CourseSelectCompletePanelSource;

extern void addRenderCallback(void *, void (*)(CourseSelectWidgetActor *), CourseSelectWidgetActor *);
extern void drawCourseSelectPreviewModel(CourseSelectCoursePreviewActor *);
extern s32 allocFixedTransformMatrix(FixedTransform *);
extern void drawCourseSelectCourseCursors(CourseSelectWidgetActor *);
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
extern s16 gMenuCommonSpritesAssetHandle;
extern s16 gMenuUiSpritesAssetHandle;
extern s16 gMenuPanelTilemapAssetHandle;
extern u8 gPlayerCount;
extern CourseSelectRacePlayer D_80121D80[];
extern u8 gCourseSelectSelectedCourseId[][sizeof(CourseSelectRacePlayer)];
extern u8 gMenuTransitionState;
extern s32 gMenuFlowState;
extern CourseSelectState *gCurrentGameTask;
extern s32 gMenuRenderCallbackList;
extern s32 gModelRenderCallbackList;
extern u8 gCurrentViewportIndex;

// drawCourseSelectPreviewModel best match: 99.298% (nonmatchings/drawCourseSelectPreviewModel-8331816093655448999/base_23.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/drawCourseSelectPreviewModel.s")

#ifdef NON_MATCHING
void drawCourseSelectPreviewModel(CourseSelectCoursePreviewActor *arg0) {
    u8 sp2F;
    unsigned char sp2E;
    s8 temp_v0_2;
    CourseSelectRacePlayer *temp_v0_3;
    FixedTransform sp30;
    int temp_v0;
    u8 var_a3;
    u8 var_t0;
    u8 var_v1;

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
                var_v1 = var_v1 & ((unsigned long long) 0xFF);
            }
            if ((D_8010AED0 != 0) && (gCurrentViewportIndex == 1)) {
                var_v1 = D_8010AED0 - 1;
            }
            temp_v0_3 = &D_80121D80[(long long) gCurrentViewportIndex];
            if (temp_v0_3->state == 5) {
                var_v1 = (short) 0;
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
#endif

// updateCourseSelectPreviewModelIn best match: 92.392% (nonmatchings/updateCourseSelectPreviewModelIn-2663524570355072948/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/updateCourseSelectPreviewModelIn.s")

#ifdef NON_MATCHING
void updateCourseSelectPreviewModelIn(void *arg0) {
    CourseSelectTempVec3i sp78;
    CourseSelectAnimatedActor *actor;
    CourseSelectRacePlayer *player;
    s32 i;
    s32 move;
    u8 statusState;
    u8 state;

    actor = arg0;
    i = 0;
    if ((s32)gPlayerCount > 0) {
        do {
            statusState = gCourseSelectStatus[4 + i];
            state = actor->state[i];
            if (statusState != state) {
                actor->state[i] = statusState;
                actor->timer[i] = gCourseSelectStatus[0xC + i];
                actor->angle[i] = *(u16 *)&gCourseSelectStatus[(i * 2) + 0x14];
                gCourseSelectStatus[0xC + i] = 0;
                *(s16 *)&gCourseSelectStatus[(i * 2) + 0x14] = 0;
                state = actor->state[i];
            }

            if (gMenuFlowState != 0 && state < 5) {
                actor->state[i] = 4;
                actor->angle[i] = 0;
                state = actor->state[i];
            }

            switch (state) {
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
                    player = &D_80121D80[i];
                    if (D_8010AEE8[i] < 0) {
                        if ((s32)player->pad6[0xB] >= 9) {
                            actor->targetCourse[i] = 2;
                        } else {
                            actor->targetCourse[i] = player->pad6[0] % 3 - 1;
                        }
                    } else {
                        actor->targetCourse[i] = player->pad6[0] % 3 + 1;
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
                    state = 2;
                } else if (gCurrentGameTask->screenState == 9) {
                    actor->state[i] = 8;
                    state = 8;
                }
                break;
            case 2:
                move = 0x200000;
                if (D_8010AEE8[i] < 0) {
                    move = -0x200000;
                }
                actor->vecs[i].y += move;
                if (D_8010AEE8[i] == move) {
                    actor->state[i] = 1;
                }
                state = actor->state[i];
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
                        D_80121D80[0].pad6[2] = 3;
                        D_800EC9C0 = 0x10;
                    }
                }
                state = actor->state[i];
                break;
            case 4:
                if (D_80121D80[i].pad6[2] == 3) {
                    actor->state[i] = 5;
                    state = 5;
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
                D_80121D80[i].pad6[2] = 4;
                if ((i == 2) && (gPlayerCount == 3)) {
                    D_80121D80[3].pad6[2] = 4;
                }
                state = actor->state[i];
                break;
            case 7:
                if (D_80121D80[i].pad6[2] == 3) {
                    actor->state[i] = 5;
                    state = 5;
                } else if (gCurrentGameTask->screenState == 9) {
                    actor->state[i] = 8;
                    state = 8;
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
            makeFixedRotationY(&actor->matrix[i * 0x10], (s16)actor->angle[i]);
            transformVec3iByFixedMatrix(&actor->matrix[i * 0x10], &actor->vecs[i], &sp78);
            *(s32 *)((u8 *)actor + (i * 0x20) + 0x50) = sp78.x;
            *(s32 *)((u8 *)actor + (i * 0x20) + 0x54) = sp78.y;
            *(s32 *)((u8 *)actor + (i * 0x20) + 0x58) = sp78.z;
            gCourseSelectStatus[4 + i] = actor->state[i];
            i++;
        } while (i < (s32)gPlayerCount);
    }

    if ((D_80121D80[0].pad6[2] == 4) || (actor->state[0] == 9)) {
        removeCallbackTask(actor);
        finishCourseSelectUiTask(1);
        D_8010ADE0 = 0;
    } else {
        addRenderCallback(&gModelRenderCallbackList, (void (*)(CourseSelectWidgetActor *))drawCourseSelectPreviewModel, actor);
    }
}
#endif

void initCourseSelectPreviewModelIn(void *arg0) {
    void *actor;
    CourseSelectTempVec3i sp60;
    u8 *var_s1;
    u8 *var_s2;
    s16 *var_s3;
    u8 *var_s4;
    s32 var_s5;
    u8 *var_s6;
    CourseSelectTempVec3i *var_s7;
    s32 const_s8;

    actor = arg0;
    var_s1 = arg0;
    var_s2 = arg0;
    var_s3 = (s16 *) ((u8 *) arg0 + 0x3C);
    var_s4 = arg0;
    var_s5 = 0;
    var_s6 = arg0;
    var_s7 = (CourseSelectTempVec3i *) ((u8 *) arg0 + 0xBC);
    const_s8 = 0xC00000;
    do {
        if (actor && actor) {
        }
        *(s32 *) (var_s2 + 0xBC) = const_s8;
        *(s32 *) (var_s2 + (short) 0xC0) = 0;
        *(s32 *) (var_s2 + 0xC4) = 0;
        *(s16 *) (var_s4 + 0xEC) = 0;
        makeFixedRotationY(var_s3, *(s16 *) (var_s4 + 0xEC));
        if (actor && actor) {
        }
        transformVec3iByFixedMatrix(var_s3, var_s7, &sp60);
        var_s5 += 2;
        *(s32 *) (var_s1 + 0x50) = sp60.x;
        *(s32 *) (var_s1 + 0x54) = sp60.y;
        var_s1 += 0x20;
        *(s32 *) (var_s1 + 0x38) = sp60.z;
        var_s6 = var_s6 + 1;
        var_s3 = (s16 *) ((u8 *) var_s3 + 0x20);
        var_s2 += 0xC;
        var_s7 += 1;
        var_s4 += 2;
        *(var_s6 + 0xFF) = 0;
    } while (var_s5 != 8);

    *(s32 *) ((u8 *) actor + 0x30) = 0;
    *(s32 *) ((u8 *) actor + 0x34) = 0;
    *(s32 *) ((u8 *) actor + 0x38) = 0;
    makeFixedRotationYX((s16 *) ((u8 *) actor + 0x1C), 0x400, 0x280);
    setCallbackTaskCallback(actor, updateCourseSelectPreviewModelIn);
}

// drawCourseSelectPreviewModelClose best match: 97.744% (nonmatchings/drawCourseSelectPreviewModelClose-5802343343535905907/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/drawCourseSelectPreviewModelClose.s")

#ifdef NON_MATCHING
void drawCourseSelectPreviewModelClose(CourseSelectCoursePreviewActor *arg0) {
    FixedTransform sp30;
    u8 sp2F;
    u8 sp2E;
    CourseSelectRacePlayer *temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_2;
    int temp_v0;
    u8 temp_v1;
    u8 var_a3;
    u8 var_t0;
    u8 var_v1;

    if ((D_8010AED0 != 0) && (gCurrentViewportIndex == 1)) {
        var_t0 = 0;
    } else {
        var_t0 = gCurrentViewportIndex;
    }
    temp_v1 = D_8010AECC[var_t0];
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
                var_v1 = D_8010AED0 - 1;
            }
            temp_v0_3 = &D_80121D80[gCurrentViewportIndex];
            if (temp_v0_3->state == 5) {
                var_v1 = 0;
                var_a3 = (var_a3 % 3) + 0xC;
            }
            if (temp_v0_3->courseIndex >= 9) {
                var_v1 = 0;
            }
            sp2E = var_v1;
            sp2F = var_a3;
            composeFixedTransforms(&arg0->sourceTransform, &arg0->playerTransforms[var_t0], &sp30);
            temp_v0_4 = allocFixedTransformMatrix(&sp30);
            arg0->matrix = temp_v0_4;
            if (temp_v0_4 != 0) {
                drawRacePlayerModelRootPart((void *)temp_v0_4, (s16) sp2F, (s16) sp2E);
            }
        }
    }
}
#endif

// updateCourseSelectPreviewModelOut best match: 96.512% (nonmatchings/updateCourseSelectPreviewModelOut-6866765942504228165/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/updateCourseSelectPreviewModelOut.s")

#ifdef NON_MATCHING
void updateCourseSelectPreviewModelOut(void *arg0) {
    CourseSelectTempVec3i sp78;
    CourseSelectAnimatedActor *actor;
    CourseSelectRacePlayer *player;
    s32 i;
    s32 move;
    s32 offset;
    u8 statusState;
    u8 state;

    actor = arg0;
    i = 0;
    if ((s32)gPlayerCount > 0) {
        offset = 0;
        do {
            statusState = gCourseSelectStatus[8 + i];
            state = actor->state[i];
            if (statusState != state) {
                actor->state[i] = statusState;
                actor->timer[i] = gCourseSelectStatus[0x10 + i];
                actor->angle[i] = *(u16 *)&gCourseSelectStatus[offset + 0x1C];
                gCourseSelectStatus[0x10 + i] = 0;
                *(s16 *)&gCourseSelectStatus[offset + 0x1C] = 0;
                state = actor->state[i];
            }

            if (gMenuFlowState != 0 && state < 5) {
                actor->state[i] = 4;
                actor->angle[i] = 0;
                state = actor->state[i];
            }

            switch (state) {
            case 0:
                break;
            case 1:
                if (D_8010AECC[i] & 1) {
                    player = &D_80121D80[i];
                    if (D_8010AEE8[i] < 0) {
                        if ((s32)player->pad6[0xB] >= 9) {
                            actor->targetCourse[i] = 2;
                        } else {
                            actor->targetCourse[i] = player->pad6[0] % 3 - 1;
                        }
                    } else {
                        actor->targetCourse[i] = player->pad6[0] % 3 + 1;
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
                    state = 2;
                } else if (gCurrentGameTask->screenState == 9) {
                    actor->state[i] = 8;
                    state = 8;
                }
                break;
            case 2:
                move = 0x200000;
                if (D_8010AEE8[i] < 0) {
                    move = -0x200000;
                }
                actor->vecs[i].y += move;
                D_8010AEE8[i] -= move;
                if (D_8010AEE8[i] == 0) {
                    actor->state[i] = 1;
                    D_8010AECC[i]++;
                    D_8010AECC[i] &= 3;
                }
                state = actor->state[i];
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
                        D_80121D80[0].pad6[2] = 3;
                        D_800EC9C0 = 0x10;
                    }
                }
                state = actor->state[i];
                break;
            case 4:
                if (D_80121D80[i].pad6[2] == 3) {
                    actor->state[i] = 5;
                    state = 5;
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
                D_80121D80[i].pad6[2] = 4;
                if (i == 2 && gPlayerCount == 3) {
                    D_80121D80[3].pad6[2] = 4;
                }
                state = actor->state[i];
                break;
            case 7:
                if (D_80121D80[i].pad6[2] == 3) {
                    actor->state[i] = 5;
                    state = 5;
                } else if (gCurrentGameTask->screenState == 9) {
                    actor->state[i] = 8;
                    state = 8;
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

            state = actor->state[i];
            if ((s32)state < 3) {
                actor->angle[i] += 0x20;
                actor->angle[i] &= 0xFFF;
            }
            makeFixedRotationY(&actor->matrix[i * 0x10], (s16)actor->angle[i]);
            transformVec3iByFixedMatrix(&actor->matrix[i * 0x10], &actor->vecs[i], &sp78);
            *(s32 *)((u8 *)actor + (i * 0x20) + 0x50) = sp78.x;
            *(s32 *)((u8 *)actor + (i * 0x20) + 0x54) = sp78.y;
            *(s32 *)((u8 *)actor + (i * 0x20) + 0x58) = sp78.z;
            gCourseSelectStatus[8 + i] = actor->state[i];
            i++;
            offset += 2;
        } while (i < (s32)gPlayerCount);
    }

    if ((D_80121D80[0].pad6[2] == 4) || (actor->state[0] == 9)) {
        removeCallbackTask(actor);
        finishCourseSelectUiTask(2);
        D_8010ADE4 = 0;
    } else {
        addRenderCallback(&gModelRenderCallbackList, drawCourseSelectPreviewModelClose, actor);
    }
}
#endif

void initCourseSelectPreviewModelOut(void *arg0) {
    u8 *var_s1;
    CourseSelectTempVec3i sp60;
    void *actor;
    u8 *var_s2;
    u8 *var_s3;
    s16 *var_s4;
    u8 *var_s5;
    u8 *var_s6;
    CourseSelectTempVec3i *var_s7;
    s32 one;

    var_s3 = (u8 *) &gCourseSelectStatus;
    actor = arg0;
    var_s1 = arg0; var_s2 = arg0;
    var_s4 = (s16 *) ((u8 *) arg0 + 0x3C);
    var_s5 = arg0;
    var_s6 = arg0;
    var_s7 = (CourseSelectTempVec3i *) ((u8 *) arg0 + 0xBC);
    one = 1;
    do {
        if (actor && actor) {
        }
        *(s32 *) (var_s2 + 0xBC) = 0;
        *(s32 *) (var_s2 + 0xC0) = 0x800000;
        *(s32 *) (var_s2 + 0xC4) = 0;
        *(s16 *) (var_s5 + 0xEC) = 0;
        makeFixedRotationY(var_s4, *(s16 *) (var_s5 + 0xEC));
        if ((arg0 && arg0) && arg0) {
        }
        transformVec3iByFixedMatrix(var_s4, var_s7, &sp60);
        *(s32 *) (var_s1 + 0x50) = sp60.x;
        *(s32 *) (var_s1 + 0x54) = sp60.y;
        *(s32 *) (var_s1 + 0x58) = sp60.z;
        *(u8 *) (var_s6 + 0x100) = one;
        var_s1 += 0x20;
        var_s6 += 1;
        var_s4 = (s16 *) ((u8 *) var_s4 + 0x20);
        var_s2 += 0xC;
        var_s7 += 1;
        var_s5 += 2;
 var_s3 += 1; *(var_s3 + 7) = one; } while (var_s3 != &D_8010AF1C);

    *(s32 *) ((u8 *) actor + 0x30) = 0;
    *(s32 *) ((u8 *) actor + 0x34) = 0;
    *(s32 *) ((u8 *) actor + 0x38) = 0;
    makeFixedRotationYX((s16 *) ((u8 *) actor + 0x1C), 0x400, 0x280);
    setCallbackTaskCallback(actor, updateCourseSelectPreviewModelOut);
}

// drawCourseSelectCourseIconList best match: 99.574% (nonmatchings/drawCourseSelectCourseIconList-5802343343535905907/base_11.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/drawCourseSelectCourseIconList.s")

#ifdef NON_MATCHING
void drawCourseSelectCourseIconList(CourseSelectIconListActor *arg0) {
    CourseSelectIconListActor *sp8C;
    CourseSelectIconListActor **sp8CPtr;
    CourseSelectIconListActor *sp74;
    u8 *sp60;
    CourseSelectIconListActor *new_var;
    s32 i;
    s32 j;
    s32 alpha;
    s32 tileIndex;
    s32 clipOffset;
    s32 courseId;
    s32 selectedCourse;
    CourseSelectRacePlayer *player;
    s16 *position;

    sp8C = arg0;
    sp8CPtr = &sp8C;
    new_var = arg0;
    i = 0;
    if ((s32)gPlayerCount > 0) {
        sp60 = (u8 *)new_var;
        do {
            j = 0;
            clipOffset = (i >= 2) * 0x88;
            if (sp60[0x98] > 0) {
                sp74 = (CourseSelectIconListActor *)((u8 *)*sp8CPtr + i);
                do {
                    if (j < (sp60[0x98] - 1)) {
                        position = (s16 *)((u8 *)new_var + (i * 10) + (j * 2));
                        if (1) {
                        }
                        player = &D_80121D80[i];
                        alpha = 0;
                        clipOffset = (i >= 2) * 0x8C;
                        if (gPlayerCount == 1) {
                            if (player->state != 5) {
                                tileIndex = (j + 1) & 0xFFFF;
                            } else {
                                tileIndex = (j + 6) & 0xFFFF;
                            }
                        } else if (player->state != 5) {
                            tileIndex = (j + 0x14) & 0xFFFF;
                        } else {
                            tileIndex = (j + 0x18) & 0xFFFF;
                        }

                        selectedCourse = D_8010AEA4[i];
                        if ((selectedCourse > 0) && (selectedCourse < 8)) {
                            courseId = player->pad6[0];
                            if (courseId >= 9) {
                                if ((j == 3) && (selectedCourse & 1)) {
                                    alpha = 0xFF;
                                }
                            } else if ((j == (courseId % 3)) && (selectedCourse & 1)) {
                                alpha = 0xFF;
                            }
                        }

                        drawMenuSpriteClipped(position[12], position[32], getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle),
                                      (u16)tileIndex, 0x20, 0x20, 0, alpha,
                                      new_var->clipLeft - clipOffset, arg0->clipTop, arg0->clipRight,
                                      new_var->clipBottom);
                    } else if (gPlayerCount == 1) {
                        alpha = 0;
                        position = (s16 *)((u8 *)arg0 + (i * 10) + (j * 2));
                        clipOffset = (i >= 2) * 0x88;
                        if ((D_8010AEA4[i] > 0) && (D_8010AEA4[i] < 8) && (gCourseSelectExtraCourseColumnState == 1) &&
                            (D_8010AEA4[i] & 1)) {
                            alpha = 0xFF;
                        }

                        if (gRaceSplitscreenMode == 3) {
                            drawMenuSpriteClipped(position[12], position[32], getRelocatableHeapBlockBase(gMenuPanelTilemapAssetHandle), 5, 0x20,
                                          0x20, 0, alpha, arg0->clipLeft - clipOffset, new_var->clipTop,
                                          new_var->clipRight, new_var->clipBottom);
                        } else {
                            drawMenuSpriteClipped(position[12], position[32], getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), 5, 0x20,
                                          0x20, 0, alpha, arg0->clipLeft - clipOffset, arg0->clipTop,
                                          arg0->clipRight, new_var->clipBottom);
                        }
                    }
                    j++;
                } while (j < sp74->itemCounts[0]);
            }
            i++;
            sp60++;
        } while (i < (s32)gPlayerCount);
    }
}
#endif

// updateCourseSelectCourseIconList best match: 82.656% (nonmatchings/updateCourseSelectCourseIconList-2694253543240320626/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/updateCourseSelectCourseIconList.s")

#ifdef NON_MATCHING
void updateCourseSelectCourseIconList(CourseSelectIconListActor *arg0) {
    CourseSelectIconListActor *actor;
    s32 playerIndex;
    s32 i;
    s32 movingCount;
    s32 screenState;
    s32 maxItems;
    s32 selectedIndex;
    s16 target;
    s16 current;
    void (*initOut)(void *);
    void (*initBackdrop)(void *);
    void (*initCursors)(void *);

    playerIndex = 0;
    if ((s32)gPlayerCount > 0) {
        actor = arg0;
        do {
            screenState = gCurrentGameTask->screenState;
            if (screenState == 1) {
                actor->state[playerIndex] = 9;
                screenState = gCurrentGameTask->screenState;
            }
            if ((screenState == 3) && (actor->state[playerIndex] < 0xB)) {
                actor->state[playerIndex] = 0xB;
                screenState = gCurrentGameTask->screenState;
            }
            if (screenState == 9) {
                actor->state[playerIndex] = 0xD;
            }
            screenState = actor->state[playerIndex];
            if ((gMenuFlowState != 0) && (screenState < 4)) {
                actor->state[playerIndex] = 4;
                screenState = 4;
            }

            switch (screenState) {
            case 0:
                movingCount = 0;
                i = 0;
                if ((s32)actor->itemCounts[playerIndex] > 0) {
                    do {
                        if (actor->y[playerIndex][i] < actor->targetY[playerIndex]) {
                            actor->y[playerIndex][i] += 0x10;
                            movingCount++;
                            if (actor->targetY[playerIndex] < actor->y[playerIndex][i]) {
                                actor->y[playerIndex][i] = actor->targetY[playerIndex];
                            }
                        }
                        i++;
                    } while (i < (s32)arg0->itemCounts[playerIndex]);
                }
                actor->timer[playerIndex]++;
                if ((D_80121D80[playerIndex].state != 5) && (D_8010AEA0[playerIndex] != 0) &&
                    (gCourseSelectModeSelection != 1)) {
                    maxItems = 5;
                } else {
                    maxItems = 4;
                }
                if (!(actor->timer[playerIndex] & 1) && ((s32)actor->itemCounts[playerIndex] < maxItems)) {
                    actor->itemCounts[playerIndex]++;
                }
                if (movingCount == 0) {
                    actor->state[playerIndex] = 1;
                    if (gPlayerCount == 1) {
                        initOut = initCourseSelectPreviewModelOut;
                        initBackdrop = initCourseSelectCourseListBackdrop;
                        D_8010ADE0 = createCallbackTask(initCourseSelectPreviewModelIn, 0, 0x63);
                        D_8010ADE4 = createCallbackTask(initOut, 0, 0x62);
                        createCallbackTask(initBackdrop, 0, 0x60);
                    } else if (playerIndex == 0) {
                        initCursors = initCourseSelectCourseCursors;
                        createCallbackTask(initCourseSelectCourseStats, 0, 0x62);
                        gActiveMenuTask = createCallbackTask(initCursors, 0, 0x64);
                    }
                }
                break;
            case 1:
                if (*(&gMenuTransitionState + (playerIndex * sizeof(CourseSelectRacePlayer))) == 1) {
                    actor->state[playerIndex] = 2;
                }
                break;
            case 2:
                i = 0;
                if ((s32)D_8010AEA4[playerIndex] >= 9) {
                    if ((s32)actor->itemCounts[playerIndex] > 0) {
                        do {
                            if (i != D_8010AE64[playerIndex]) {
                                actor->y[playerIndex][i] -= 0x20;
                            }
                            i++;
                        } while (i < (s32)arg0->itemCounts[playerIndex]);
                    }
                    if (D_8010AE64[playerIndex] != 0) {
                        if (actor->startY[playerIndex] >= actor->y[playerIndex][0]) {
                            actor->state[playerIndex] = 3;
                        }
                    } else if (actor->startY[playerIndex] >= actor->y[playerIndex][1]) {
                        actor->state[playerIndex] = 3;
                    }
                    if (actor->state[playerIndex] == 3) {
                        if (actor->x[playerIndex][D_8010AE64[playerIndex]] < actor->targetX[playerIndex]) {
                            actor->direction[playerIndex] = 1;
                        } else {
                            actor->direction[playerIndex] = -1;
                        }
                    }
                }
                break;
            case 3:
                actor->x[playerIndex][D_8010AE64[playerIndex]] += actor->speed[playerIndex] * actor->direction[playerIndex];
                current = actor->x[playerIndex][D_8010AE64[playerIndex]];
                target = actor->targetX[playerIndex];
                if (((actor->direction[playerIndex] == 1) && (current >= target)) ||
                    ((actor->direction[playerIndex] == -1) && (target >= current))) {
                    actor->x[playerIndex][D_8010AE64[playerIndex]] = target;
                    actor->state[playerIndex] = 4;
                    gMenuChoicePromptState[playerIndex] = 1;
                }
                break;
            case 4:
                if ((gMenuFlowState == 0) && (gMenuChoicePromptState[playerIndex] == 0)) {
                    actor->state[playerIndex] = 5;
                }
                if (D_80121D80[playerIndex].pad6[2] == 3) {
                    actor->state[playerIndex] = 7;
                }
                break;
            case 5:
                selectedIndex = D_8010AE64[playerIndex];
                actor->x[playerIndex][selectedIndex] += actor->speed[playerIndex] * actor->direction[playerIndex] * -1;
                current = actor->x[playerIndex][selectedIndex];
                target = (selectedIndex * actor->speed[playerIndex]) + actor->baseX[playerIndex];
                if (((actor->direction[playerIndex] == 1) && (target >= current)) ||
                    ((actor->direction[playerIndex] == -1) && (current >= target))) {
                    actor->x[playerIndex][selectedIndex] = target;
                    actor->state[playerIndex] = 6;
                }
                break;
            case 6:
                i = 0;
                if ((s32)actor->itemCounts[playerIndex] > 0) {
                    do {
                        if (i != D_8010AE64[playerIndex]) {
                            actor->y[playerIndex][i] += 0x20;
                        }
                        i++;
                    } while (i < (s32)arg0->itemCounts[playerIndex]);
                }
                if (D_8010AE64[playerIndex] != 0) {
                    if (actor->y[playerIndex][0] >= actor->targetY[playerIndex]) {
                        actor->state[playerIndex] = 1;
                    }
                } else if (actor->y[playerIndex][1] >= actor->targetY[playerIndex]) {
                    actor->state[playerIndex] = 1;
                }
                if (actor->state[playerIndex] == 1) {
                    *(&gMenuTransitionState + (playerIndex * sizeof(CourseSelectRacePlayer))) = 0;
                }
                break;
            case 7:
                if (gMenuFlowState != 0) {
                    i = 0;
                    if ((s32)actor->itemCounts[playerIndex] > 0) {
                        do {
                            actor->y[playerIndex][i] -= 0x20;
                            i++;
                        } while (i < (s32)actor->itemCounts[playerIndex]);
                    }
                } else {
                    actor->y[playerIndex][D_8010AE64[playerIndex]] -= 0x20;
                }
                if (actor->startY[playerIndex] >= actor->y[playerIndex][D_8010AE64[playerIndex]]) {
                    actor->state[playerIndex] = 8;
                }
                break;
            case 9:
                actor->y[playerIndex][D_8010AE64[playerIndex]] -= 0x20;
                if (actor->startY[playerIndex] >= actor->y[playerIndex][D_8010AE64[playerIndex]]) {
                    actor->state[playerIndex] = 0xA;
                }
                break;
            case 11:
                actor->y[playerIndex][D_8010AE64[playerIndex]] += 0x20;
                if (actor->y[playerIndex][D_8010AE64[playerIndex]] >= actor->targetY[playerIndex]) {
                    actor->y[playerIndex][D_8010AE64[playerIndex]] = actor->targetY[playerIndex];
                    actor->state[playerIndex] = 0xC;
                }
                break;
            case 12:
                if (gCurrentGameTask->screenState == 4) {
                    actor->state[playerIndex] = 4;
                }
                break;
            }
            playerIndex++;
        } while (playerIndex < (s32)gPlayerCount);
    }

    if ((arg0->state[0] == 8) || (arg0->state[0] == 0xD)) {
        removeCallbackTask(arg0);
        finishCourseSelectUiTask(3);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (void (*)(CourseSelectWidgetActor *))drawCourseSelectCourseIconList,
                      (CourseSelectWidgetActor *)arg0);
}
#endif

// initCourseSelectCourseIconList best match: 65.621% (nonmatchings/initCourseSelectCourseIconList-3357475854818838508/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/initCourseSelectCourseIconList.s")

#ifdef NON_MATCHING
void initCourseSelectCourseIconList(u8 *arg0) {
    s16 *xyTable;
    s32 temp_lo;
    s32 enabled;
    s32 i;
    u8 *unlockPtr;
    u8 *bytePtr;
    u8 *stride10Ptr;
    s16 *halfPtr;
    u8 *rowPtr;
    s16 *xPair;
    s32 layoutIndex;

    if ((s32) gPlayerCount < 3) {
        layoutIndex = gPlayerCount - 1;
    } else {
        layoutIndex = 2;
    }
    i = 0;
    if ((s32) gPlayerCount > 0) {
        unlockPtr = D_8010AEA0;
        xyTable = gCourseSelectIconListYLayout[layoutIndex];
        bytePtr = arg0;
        stride10Ptr = arg0;
        halfPtr = (s16 *) arg0;
        rowPtr = arg0;
        do {
            if (*unlockPtr != 0) {
                enabled = 1;
                if (D_80121D80[i].state == 5) {
                    enabled = 0;
                }
            } else {
                enabled = 0;
            }
            bytePtr[0x9C] = xyTable[enabled];
            halfPtr[0x34] = xyTable[((i & 1) * 2) + enabled + 2];
            halfPtr[0x44] = xyTable[((i & 1) * 2) + 2];
            xPair = &gCourseSelectIconListXLayout[layoutIndex][(i >= 2) * 2];
            halfPtr[0x40] = xPair[1];
            halfPtr[0x3C] = xPair[0];
            ((s16 *) arg0)[0x52] = 0x78;
            ((s16 *) arg0)[0x51] = 0xA0;
            ((s16 *) arg0)[0x53] = 0x78;
            if (gPlayerCount == 1) {
                ((s16 *) arg0)[0x50] = 0xA0;
            } else if (gPlayerCount == 2) {
                ((s16 *) arg0)[0x50] = 0x7E;
            } else {
                ((s16 *) arg0)[0x50] = 0x88;
            }
            ((s16 *) rowPtr)[0xC] = halfPtr[0x3C];
            ((s16 *) rowPtr)[0x20] = halfPtr[0x34];
            ((s16 *) (stride10Ptr + 2))[0xC] = halfPtr[0x3C];
            temp_lo = bytePtr[0x9C];
            stride10Ptr += 0xA;
            unlockPtr++;
            bytePtr++;
            halfPtr = (s16 *) ((u8 *) halfPtr + 2);
            rowPtr += 0xA;
            i++;
            ((s16 *) (stride10Ptr - 8))[0x20] = temp_lo + halfPtr[0x33];
            ((s16 *) (stride10Ptr - 8))[0xD] = halfPtr[0x3B];
            ((s16 *) (stride10Ptr - 8))[0x21] = (bytePtr[0x9B] * 2) + halfPtr[0x33];
            ((s16 *) (stride10Ptr - 8))[0xE] = halfPtr[0x3B];
            ((s16 *) (stride10Ptr - 8))[0x22] = (bytePtr[0x9B] * 3) + halfPtr[0x33];
            ((s16 *) (stride10Ptr - 8))[0xF] = halfPtr[0x3B];
            ((s16 *) (stride10Ptr - 8))[0x23] = (bytePtr[0x9B] * 4) + halfPtr[0x33];
            bytePtr[0x93] = 0;
            bytePtr[0x97] = 1;
            bytePtr[0x8F] = 0;
        } while (i < (s32) gPlayerCount);
    }
    setCallbackTaskCallback(arg0, updateCourseSelectCourseIconList);
}
#endif

// drawCourseSelectCourseCursors best match: 98.861% (nonmatchings/drawCourseSelectCourseCursors-2694253543240320626/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/drawCourseSelectCourseCursors.s")

#ifdef NON_MATCHING
void drawCourseSelectCourseCursors(u8 *arg0) {
    u8 *actor;
    s32 i;
    u8 *statePtr;
    s16 *posPtr;
    s8 *directionPtr;
    s16 *handles;
    s32 tileIndex;
    s32 handleIndex;
    s32 one;
    s32 playerCount;

    actor = arg0;
    if (D_8010AEB0 == 0) {
        playerCount = gPlayerCount;
        i = 0;
        if (playerCount > 0) {
            statePtr = arg0;
            handles = gAssetHandles;
            one = 1;
            do {
                directionPtr = &D_8010AE64[i];
                if (statePtr[0x30] < 2) {
                    posPtr = (s16 *)(actor + (i * 2));
                    if (one == gPlayerCount) {
                        tileIndex = 7;
                        handleIndex = 0x25;
                    } else {
                        tileIndex = 0x13;
                        handleIndex = 0x21;
                    }
                    drawMenuSpriteWithAlpha(posPtr[0xC],
                                  (s16)(posPtr[0x10] + ((*directionPtr) * statePtr[0x38])),
                                  getRelocatableHeapBlockBase(handles[handleIndex]), tileIndex & 0xFFFF, 0x20, 0x20, 0,
                                  posPtr[0x14], (short)0);
                }
                i++;
                statePtr++;
            } while (i < gPlayerCount);
        }
    }
}
#endif

void updateCourseSelectCourseCursors(CourseSelectWidgetActor *arg0) {
    register CourseSelectWidgetActor *actor;
    s32 i;
    CourseSelectWidgetActor *statePtr;
    CourseSelectRacePlayer *player;
    u8 state;

    actor = arg0;
    i = 0;
    if ((s32) gPlayerCount > 0) {
        statePtr = actor; do { if ((gMenuFlowState != 0) && (gRaceSplitscreenMode != 3)) { statePtr->unk30 = 4; }
            state = statePtr->unk30;
            switch (state) {
            case 0:
                actor->coordinates[i + 8] += 0x26;
                if (actor->coordinates[i + 8] >= 0x100) {
                    actor->coordinates[i + 8] = 0x100;
                    statePtr->unk30 = 1;
                }
                state = statePtr->unk30;
                break;
            case 1:
                player = &D_80121D80[i];
                if (statePtr->unk34 < 0x10) {
                    actor->coordinates[i + 8] -= 9;
                } else {
                    actor->coordinates[i + 8] += 9;
                }
                statePtr->unk34 = (statePtr->unk34 + 1) & 0x1F;
                if ((player->pad6[2] == 1) || (D_80121D80[0].pad6[2] == 3)) {
                    statePtr->unk30 = 2;
                }
                state = statePtr->unk30;
                break;
            case 2:
                player = &D_80121D80[i];
                if (player->pad6[2] == 3) {
                    statePtr->unk30 = 4;
                }
                if (player->pad6[2] == 0) {
                    statePtr->unk30 = 1;
                    actor->coordinates[i + 8] = 0x100;
                    statePtr->unk34 = 0;
                }
                if (gCurrentGameTask->screenState == 9) {
                    statePtr->unk30 = 4;
                }
                state = statePtr->unk30;
                break;
            case 3:
            case 4:
                break;
            }
            i++;
            statePtr = (CourseSelectWidgetActor *) ((u8 *) statePtr + 1);
            gCourseSelectStatus[i - 1] = state;
        } while (i < (s32) gPlayerCount);
    }

    if (actor->unk30 == 4) {
        removeCallbackTask(actor);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, drawCourseSelectCourseCursors, actor);
    }
}

// initCourseSelectCourseCursors best match: 89.025% (nonmatchings/initCourseSelectCourseCursors-8331816093655448999/base_11.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/initCourseSelectCourseCursors.s")

#ifdef NON_MATCHING
void initCourseSelectCourseCursors(CourseSelectWidgetActor *arg0) {
    CourseSelectWidgetActor *actor = arg0;
    s32 idx;
    s32 i;
    u8 *courseUnlocked;
    s16 (*xyTable)[6];
    s16 (*xTable)[4];
    u8 *bytePtr;
    s16 *posPtr;

    if (actor && actor) {
    }

    if (gPlayerCount < 3) {
        idx = gPlayerCount - 1;
    } else {
        idx = 2;
    }

    i = 0;
    if ((s32)gPlayerCount > 0) {
        xyTable = &gCourseSelectIconListYLayout[idx];
        xTable = &gCourseSelectIconListXLayout[idx];
        courseUnlocked = D_8010AEA0;
        bytePtr = (u8 *)actor;
        posPtr = (s16 *)actor;
        do {
            if (*courseUnlocked++ != 0) {
                idx = 1;
                if (D_80121D80[i].state == 5) {
                    idx = 0;
                }
            } else {
                idx = 0;
            }
            bytePtr[0x38] = (*xyTable)[idx];
            posPtr[0x10] = (*xyTable)[((i & 1) * 2) + idx + 2];
            posPtr[0xC] = (*xTable)[((i >= 2) * 2) + 1];
            posPtr[0x14] = 0;
            bytePtr[0x30] = 0;
            bytePtr[0x34] = 0;
            i++;
            bytePtr++;
            posPtr = (s16 *)((u8 *)posPtr + 2);
        } while (i < (s32)gPlayerCount);
    }

    setCallbackTaskCallback(actor, updateCourseSelectCourseCursors);
}
#endif

void drawCourseSelectCourseListBackdrop(CourseSelectWidgetActor *arg0) {
    drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(gMenuUiSpritesAssetHandle), 3, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16) (arg0->x + 0x40), arg0->y, getRelocatableHeapBlockBase(gMenuUiSpritesAssetHandle), 4, 0x20, 0x20, 0, 0);
    drawMenuSprite(arg0->x, (s16) (arg0->y + 0x40), getRelocatableHeapBlockBase(gMenuUiSpritesAssetHandle), 5, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16) (arg0->x + 0x40), (s16) (arg0->y + 0x40), getRelocatableHeapBlockBase(gMenuUiSpritesAssetHandle), 6, 0x20, 0x20, 0, 0);
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

// drawCourseSelectCourseStats best match: 98.594% (nonmatchings/drawCourseSelectCourseStats-6866765942504228165/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/drawCourseSelectCourseStats.s")

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
            if ((playerCount >= 2 || D_80121D80[i].pad6[2] == 0) &&
                (playerCount >= 2 || (playerCount == 1 && gCourseSelectExtraCourseColumnState == 0))) {
                playerWidget = (CourseSelectWidgetActor *)((u8 *)arg0 + (i * 2));
                courseId = D_80121D80[i].pad6[0];
                if (!playerCount) {
                }
                if (courseId >= 9 && courseId < 12) {
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

// drawCourseSelectCourseDescription best match: 86.127% (nonmatchings/drawCourseSelectCourseDescription-4923837976568703863/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/drawCourseSelectCourseDescription.s")

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
    CourseSelectSelection26C4C *selection;
    CourseSelectStatus26C4C *status;
    u8 *volatile text;
    s16 buffer[8];
    s32 value;
    s32 courseId;
    s32 digitCount;
    s32 courseIndex;
    u32 price;
    s16 *digits;

    status = (CourseSelectStatus26C4C *)gCourseSelectStatus;
    selection = (CourseSelectSelection26C4C *)D_80121D80;

    if (status->unk2C == 0) {
        volatile s32 padSelected;
        u16 selectedIndex;

        if ((D_8010AEA8 == 0) && ((selection->mode == 0) || (selection->mode == 3) || (selection->mode == 9))) {
            if (status->unk2E == 1) {
                value = 3;
            } else if (status->unk2E == 2) {
                value = 4;
            } else if ((selection->courseId >= 9) && (selection->courseId < 12)) {
                value = 5;
            } else {
                value = selection->courseId % 3;
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

            if ((gRaceSplitscreenMode == 3) && (selection->courseId < 9)) {
                text = gCourseSelectBoardLevelByCourseText + ((selection->courseId % 3) * 0x30);
            } else if ((selection->courseId >= 9) && (selection->courseId < 12)) {
                text = gCourseSelectExtraCourseBoardLevelText + ((gCourseSelectExtraCourseIds[selectedIndex] % 3) * 0x60);
            } else {
                text = gCourseSelectBoardLevelText;
            }
        }

        drawMenuGlyphScript(arg0->x, arg0->y, text, 1, arg0->spriteIndex, 0);

        if ((gRaceSplitscreenMode == 3) && ((selection->mode == 1) || (selection->mode == 2))) {
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
                            price /= 10;
                        } while (price != 0);
                    }

                    digits = &buffer[digitCount];
                    digits[1] = 0x10;
                    digits[2] = -1;
                    if (selection->courseId >= 9) {
                    }
                } else {
                    digitCount = 3;
                    buffer[2] = 0x2B;
                    digits = &buffer[digitCount];
                    digits[0] = 0x2B;
                    digits[1] = 0x2B;
                    digits[2] = 0x2B;
                    digits[3] = 0x2B;
                    buffer[7] = -1;
                }
                drawMenuGlyphScript((s16)(arg0->x + 0x20), (s16)(arg0->y + 0x20), (u8 *)buffer, 1, arg0->spriteIndex, 0);
            }
        }
    } else {
        text = gCourseSelectPurchaseMessageText + ((status->unk2C * 0x32) - 0x32);
        drawMenuGlyphScript(arg0->x, arg0->y, text, 1, arg0->spriteIndex, 0);
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

// drawCourseSelectExtraCourseIconList best match: 92.684% (nonmatchings/drawCourseSelectExtraCourseIconList-2694253543240320626/base_12.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/drawCourseSelectExtraCourseIconList.s")

#ifdef NON_MATCHING
#define EXTRA_COURSE_ICON_LIST_S16(ptr, off) (*(s16 *)((ptr) + (off)))
#define EXTRA_COURSE_ICON_LIST_U16(ptr, off) (*(u16 *)((ptr) + (off)))
#define EXTRA_COURSE_ICON_LIST_U8(ptr, off) (*(u8 *)((ptr) + (off)))

void drawCourseSelectExtraCourseIconList(CourseSelectWidgetActor *arg0_in) {
    u8 *arg0 = (u8 *)arg0_in;
    s32 sp74;
    u8 *sp68;
    s16 *var_s5;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 temp_v0_3;
    s32 var_fp;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s2;
    u8 *temp_s2;
    u8 *temp_t6;
    u8 *temp_s1;
    u8 *var_s1;

    var_fp = 0;
    if ((s32) gPlayerCount > 0) {
        var_s5 = gMenuChoicePromptState;
        sp74 = 0;
        do {
            temp_v0 = *var_s5;
            if (temp_v0 != 0) {
                temp_t6 = &arg0[var_fp];
                if (temp_v0 != 9) {
                    sp68 = temp_t6;
                    var_s2 = EXTRA_COURSE_ICON_LIST_U8(temp_t6, 0x5C) - 1;
                    if (var_s2 >= 0) {
                        var_s1 = arg0 + (var_fp * 6) + (var_s2 * 2);
                        do {
                            temp_v0_2 = *var_s5;
                            if (temp_v0_2 == 1) {
                                var_s0 = 0x100;
                            } else {
                                var_s0 = 0x60;
                                if ((var_s2 + 2) == temp_v0_2) {
                                    var_s0 = 0x100;
                                }
                            }
                            drawMenuSpriteWithAlphaClipped(EXTRA_COURSE_ICON_LIST_S16(var_s1, 0x18),
                                          EXTRA_COURSE_ICON_LIST_S16(var_s1, 0x30),
                                          getRelocatableHeapBlockBase(gAssetHandles[0x21]),
                                          EXTRA_COURSE_ICON_LIST_U16(var_s1, 0x68), 0x20, 0x20, 0, var_s0, 0,
                                          EXTRA_COURSE_ICON_LIST_S16(arg0, 0x48) - ((var_fp >= 2) * 0x8C),
                                          EXTRA_COURSE_ICON_LIST_S16(arg0, 0x4C),
                                          EXTRA_COURSE_ICON_LIST_S16(arg0, 0x4A),
                                          EXTRA_COURSE_ICON_LIST_S16(arg0, 0x4E));
                            var_s2 -= 1;
                            var_s1 -= 2;
                        } while (var_s2 >= 0);
                    }
                    if (D_8010AEB0 == 0) {
                        temp_v0_3 = *var_s5;
                        if ((temp_v0_3 >= 2) && (temp_v0_3 < 5)) {
                            temp_s1 = arg0 + (var_fp * 6);
                            temp_s2 = &arg0[sp74];
                            if ((s32) gPlayerCount == 1) {
                                var_s0_2 = 0xF;
                            } else {
                                var_s0_2 = 0x1B;
                            }
                            drawMenuSpriteWithAlpha(EXTRA_COURSE_ICON_LIST_S16(temp_s1, 0x18),
                                          (s16)((EXTRA_COURSE_ICON_LIST_U8(sp68, 0x50) * (*var_s5 - 2))
                                              + EXTRA_COURSE_ICON_LIST_S16(temp_s1, 0x30)),
                                          getRelocatableHeapBlockBase(gAssetHandles[0x21]), var_s0_2 & 0xFFFF, 0x20,
                                          0x20, 0, EXTRA_COURSE_ICON_LIST_U16(temp_s2, 0x60), 0);
                        }
                    }
                }
            }
            var_fp += 1;
            sp74 += 2;
            var_s5 += 1;
        } while (var_fp < (s32) gPlayerCount);
    }
}

#undef EXTRA_COURSE_ICON_LIST_S16
#undef EXTRA_COURSE_ICON_LIST_U16
#undef EXTRA_COURSE_ICON_LIST_U8
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

// updateCourseSelectExtraCourseIconList best match: 90.349% (nonmatchings/updateCourseSelectExtraCourseIconList-7273315160691878794/base_11.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/updateCourseSelectExtraCourseIconList.s")

// initCourseSelectExtraCourseIconList best match: 98.393% (nonmatchings/initCourseSelectExtraCourseIconList-2663524570355072948/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/initCourseSelectExtraCourseIconList.s")

#ifdef NON_MATCHING
void initCourseSelectExtraCourseIconList(CourseSelectWidgetActor *arg0) {
    s32 new_var3;
    s32 var_a0;
    s32 var_t0;
    s32 var_t5;
    s32 var_v1;
    u8 *var_t1;
    u8 *var_t2;
    u8 *var_t3;
    u8 *var_v0;
    s16 *temp_s1;
    s16 *temp_ra;
    s32 temp_s6;
    s32 temp_s4;
    CourseSelectWidgetActor *new_var;
    unsigned int new_var2;
    s32 temp_s3;
    s32 temp_s2;
    s32 temp_a3;

    do {
        if ((s32)gPlayerCount < 3) { var_t5 = gPlayerCount - 1; } else { var_t5 = 2; do { } while (0); } var_t0 = 0; do { if ((s32)gPlayerCount > 0) { new_var2 = var_t5; temp_ra = gCourseSelectIconListXLayout[new_var2]; temp_s1 = gCourseSelectIconListYLayout[new_var2]; var_t3 = (var_t2 = (var_t1 = (u8 *)arg0)); temp_s6 = 0xC; temp_s4 = 1; temp_s3 = 0x100; temp_s2 = 0xC; new_var3 = new_var2 * temp_s6; temp_a3 = 3; do { var_v1 = 0; if ((s32)gPlayerCount >= 3) { var_t1[0x50] = temp_s2; } else { var_t1[0x50] = *(s16 *)((u8 *)gCourseSelectIconListYLayout + new_var3); if (((arg0 && arg0) && arg0) != 0) { } } var_t1[0x5C] = 0; *(s16 *)(var_t3 + 0x60) = temp_s3; if (gPlayerCount == temp_s4) { var_a0 = 8; } else { var_a0 = 4; } var_v0 = var_t2; loop: var_v1 += 1; *(s16 *)(var_v0 + 0x18) = temp_ra[((var_t0 >= 2) * 2) + 1] + var_a0; var_v0 += 2; *(s16 *)(var_v0 + 0x2E) = temp_s1[((var_t0 & 1) * 2) + 2]; if (var_v1 != temp_a3) { goto loop; } var_t0 = var_t0 + 1; var_t1 += 1; var_t2 += 6; var_t3 += 2; } while (var_t0 < (s32)gPlayerCount); } } while (0); new_var = arg0; new_var->coordinates[0x19] = 0xA0; arg0->coordinates[0x1A] = 0x78; arg0->coordinates[0x1B] = 0x78; if (gPlayerCount == 1) { arg0->coordinates[0x18] = 0xA0; } else if (gPlayerCount == 2) { arg0->coordinates[0x18] = 0x7E; } else { arg0->coordinates[0x18] = 0x88; } setCallbackTaskCallback(arg0, updateCourseSelectExtraCourseIconList);
    } while (0);
}
#endif

// drawCourseSelectPlayerPanels best match: 98.748% (nonmatchings/drawCourseSelectPlayerPanels-6866765942504228165/base_16.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/drawCourseSelectPlayerPanels.s")

#ifdef NON_MATCHING
extern int sprintf(u8 *, u8 *, ...);

void drawCourseSelectPlayerPanels(CourseSelectWidgetInitActor *actor) {
    s16 *yPtr;
    u8 text[4];
    s32 nextIndex;
    volatile s32 savedIndex;
    s32 offset;
    s32 j;
    s32 middleCount;
    s32 count;
    s32 edgeOffset;
    s32 tile;
    s32 alpha;
    s32 i;

    if (gPlayerCount == 2) {
        if (((!actor->unk20) && (!actor->unk20)) && (!actor->unk20)) {
        }
        count = 2;
    } else {
        count = 4;
    }

    i = 0;
    if (count > 0) {
        do {
            j = 0;
            alpha = 0x100;
            yPtr = &actor->unk20;
            if (i == gPlayerCount) {
                alpha = 0xC0;
            }
            if (i == 0) {
                tile = 2;
            } else {
                tile = (i + 5) & 0xFF;
            }

            savedIndex = i;
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
                if (i) {
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
            sprintf(text, "%d", nextIndex);
            drawMenuAsciiText((s16)(actor->unk18 + 0x34), (s16)(*yPtr + 2), text, 0, alpha);

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

// updateCourseSelectPlayerPanels best match: 83.090% (nonmatchings/updateCourseSelectPlayerPanels-6866765942504228165/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/updateCourseSelectPlayerPanels.s")

#ifdef NON_MATCHING
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x[4];
    /* 0x20 */ s16 y[4];
    /* 0x28 */ s16 targetX[4];
    /* 0x30 */ s16 deltaX[4];
    /* 0x38 */ s16 deltaY[4];
    /* 0x40 */ s16 exitTargetX[1];
    /* 0x42 */ u16 period[4];
    /* 0x4A */ u16 speed[4];
    /* 0x52 */ u16 timer[4];
    /* 0x5A */ u8 state[4];
} CourseSelectWidgetTransitionActor;

void updateCourseSelectPlayerPanels(CourseSelectWidgetActor *arg0) {
    volatile u8 pad[0x20];
    CourseSelectWidgetTransitionActor *actor;
    s32 i;
    s32 count;
    s32 next;
    s32 step;
    u8 *statePtr;
    u8 *savedStatePtr;
    u8 *modePtr;

    actor = (CourseSelectWidgetTransitionActor *)arg0;
    modePtr = &gPlayerCount;
    if (*modePtr == 2) {
        count = 2;
    } else {
        count = 4;
    }

    i = 0;
    if (count > 0) {
        statePtr = actor->state;
        do {
            next = i + 1;
            switch (statePtr[0]) {
                case 0:
                    step = 0;
                    do {
                        u16 timer = actor->timer[i] + actor->speed[i];
                        u16 period = actor->period[i];
                        actor->x[i] += actor->deltaX[i];
                        actor->timer[i] = timer;
                        if (timer >= period) {
                            actor->y[i] += actor->deltaY[i];
                            actor->timer[i] = timer - period;
                        }
                        step++;
                        if (actor->x[i] == actor->targetX[i]) {
                            statePtr[0] = 1;
                            if (count == next) {
                                savedStatePtr = statePtr;
                                D_8010ADE0 = createCallbackTask((void (*)(CallbackTask *))initCourseSelectPreviewModelIn, 0, 0x62);
                                D_8010ADE4 = createCallbackTask((void (*)(CallbackTask *))initCourseSelectPreviewModelOut, 0, 0x62);
                                createCallbackTask(initCourseSelectCourseIconList, 0, 0x62);
                                statePtr = savedStatePtr;
                            }
                            break;
                        }
                    } while (step != 0x18);
                    break;
                case 1:
                    if (*(&gMenuTransitionState + (i * sizeof(CourseSelectRacePlayer))) == 4) {
                        statePtr[0] = 2;
                        actor->timer[i] = 0;
                    }
                    break;
                case 2:
                    step = 0;
                    do {
                        u16 timer = actor->timer[i] + actor->speed[i];
                        u16 period = actor->period[i];
                        actor->x[i] -= actor->deltaX[i];
                        actor->timer[i] = timer;
                        if (timer >= period) {
                            actor->y[i] -= actor->deltaY[i];
                            actor->timer[i] = timer - period;
                        }
                        step++;
                        if (count == next) {
                            if (((*modePtr == 2) && (actor->x[0] >= actor->exitTargetX[0])) ||
                                ((*modePtr >= 3) && (actor->exitTargetX[0] >= actor->x[0]))) {
                                s32 j = 0;
                                if (count > 0) {
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
            i = next;
            statePtr++;
        } while (next != count);
    }

    addRenderCallback(&gMenuRenderCallbackList, drawCourseSelectPlayerPanels, arg0);
}
#endif

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

// updateCourseSelectCompletePanels best match: 90.448% (nonmatchings/updateCourseSelectCompletePanels-2663524570355072948/base_11.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/updateCourseSelectCompletePanels.s")

#ifdef NON_MATCHING
void updateCourseSelectCompletePanels(CourseSelectWidgetActor *arg0) {
    CourseSelectRacePlayer *player;
    CourseSelectRacePlayer *end;
    CourseSelectWidgetActor *base;
    CourseSelectCompletePanelSource *source0;
    CourseSelectCompletePanelSource *source1;
    s32 i;
    u16 alpha;
    s16 nextAlpha;

    base = arg0;
    i = 0;
    source0 = (CourseSelectCompletePanelSource *)D_8010ADE0;
    source1 = (CourseSelectCompletePanelSource *)D_8010ADE4;
    if ((s32)gPlayerCount > 0) {
        do {
            alpha = arg0->alpha;
            if (alpha == 0) {
                if ((source0 != NULL) && ((source0->playerStates[i] == 4) || (source1->playerStates[i] == 4))) {
                    arg0->alpha = 1;
                }
            } else {
                if (alpha != 0x100) {
                    nextAlpha = alpha + 0x30;
                    arg0->alpha = nextAlpha;
                    if ((u16)nextAlpha >= 0x100) {
                        arg0->alpha = 0x100;
                    }
                }
                if (D_80121D80[i].pad6[2] == 1) {
                    arg0->alpha = 0;
                }
            }
            i++;
            arg0 = (CourseSelectWidgetActor *)((u8 *)arg0 + sizeof(s16));
        } while (i < (s32)gPlayerCount);
    }

    if (D_800EC9C0 == 0x10) {
        removeCallbackTask(base);
        player = D_80121D80;
        if ((s32)gPlayerCount > 0) {
            end = &D_80121D80[gPlayerCount];
            do {
                player->pad6[2] = 3;
                player++;
            } while ((u32)player < (u32)end);
        }
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (void (*)(CourseSelectWidgetActor *))drawCourseSelectCompletePanels, base);
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
