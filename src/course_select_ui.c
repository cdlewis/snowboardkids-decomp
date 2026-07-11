#include "common.h"
#include "memory_allocator.h"
#include "effect_task_scheduler.h"
#include "menu_rendering.h"
#include "course_select_ui.h"
#include "shop_menu_ui.h"
#include "fixed_point_matrix.h"
#include "race_position_ui.h"

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
} CourseSelectWidgetInitActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 coordinates[0x40];
    /* 0x98 */ u8 itemCounts[4];
    /* 0x9C */ u8 pad9C[4];
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

extern void func_800483FC(void *, void (*)(CourseSelectWidgetActor *), CourseSelectWidgetActor *);
extern void func_80023880(CourseSelectCoursePreviewActor *);
extern s32 func_8004885C(FixedTransform *);
extern void func_80025AA8(CourseSelectWidgetActor *);
extern s8 D_800ECA2F[][0x78F8];
extern s8 D_800EC9C0;
extern u8 D_800EC9C2;
extern u8 D_800EC9E6;
extern s32 D_8010ADDC;
extern EffectTask *D_8010ADE0;
extern EffectTask *D_8010ADE4;
extern s32 D_8010ADE8;
extern s8 D_8010AE64[];
extern u8 D_8010AECC[];
extern u16 D_8010AED0;
extern s32 D_8010AEE8[];
extern u8 D_8010AEFB[];
extern u8 D_8010AEA4[];
extern u8 D_8010AEB0;
extern u8 D_8010AEA0[];
extern u8 D_8010AF46;
extern u8 D_8010AF18[];
extern u8 D_8010AF1C;
extern s16 D_800EC9D0[];
extern u8 D_800B7040[];
extern u8 D_800B706C[];
extern u8 D_800B706D[];
extern u8 D_800B706E[];
extern s16 D_800B7084[][6];
extern s16 D_800B70A8[][4];
extern s16 D_800B70C0[][4];
extern s16 D_800B70D8[][2];
extern s16 D_80112130[];
extern s16 D_80112172;
extern s16 D_8011217A;
extern s16 D_8011217E;
extern u8 D_80121B55;
extern CourseSelectRacePlayer D_80121D80[];
extern u8 D_80121D86[][sizeof(CourseSelectRacePlayer)];
extern u8 D_80121D88;
extern s32 D_801235B4;
extern CourseSelectState *D_801235B8;
extern s32 D_80124868;
extern s32 D_80124898;
extern u8 D_80156608;

// func_80023880 best match: 97.975%
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80023880.s")

#ifdef NON_MATCHING
void func_80023880(CourseSelectCoursePreviewActor *arg0) {
    FixedTransform sp30;
    u8 sp2F;
    unsigned char sp2E;
    CourseSelectRacePlayer *temp_v0_3;
    s32 temp_v0_4;
    s8 temp_v0_2;
    int temp_v0;
    u8 temp_v1;
    u8 var_a3;
    u8 var_t0;
    u8 var_v1;

    if ((D_8010AED0 != 0) && (D_80156608 == 1)) {
        var_t0 = 0;
    } else {
        var_t0 = D_80156608;
    }
    temp_v1 = D_8010AECC[var_t0];
    if ((D_8010AECC[var_t0] == 0) || (D_8010AECC[var_t0] & 1)) {
        temp_v0 = arg0->playerFlags[var_t0];
        if ((temp_v0 == 0) || (temp_v0 & 1)) {
            if (D_8010AECC[var_t0] == 1) {
                var_a3 = arg0->playerSlots[var_t0].courseIndex;
            } else {
                var_a3 = D_80121D86[var_t0][0];
            }
            temp_v0_2 = D_800ECA2F[var_t0][var_a3];
            if (temp_v0_2 == -1) {
                var_v1 = 9;
            } else {
                var_v1 = temp_v0_2 & ((unsigned long long) 0xFF);
            }
            if ((D_8010AED0 != 0) && (D_80156608 == 1)) {
                var_v1 = D_8010AED0 - 1;
            }
            temp_v0_3 = &D_80121D80[(long long) D_80156608];
            if (temp_v0_3->state == 5) {
                var_v1 = 0;
                var_a3 = ((var_a3 % 3) + 0xC) & 0xFF;
            }
            if (temp_v0_3->courseIndex >= 9) {
                var_v1 = 0;
            }
            sp2E = var_v1;
            sp2F = var_a3;
            func_800987A0(&arg0->sourceTransform, &arg0->playerTransforms[var_t0], &sp30);
            temp_v0_4 = func_8004885C(&sp30);
            arg0->matrix = temp_v0_4;
            if (temp_v0_4 != 0) {
                func_8007C130((void *)(temp_v0_4 ^ 0), (s16) sp2F, (s16) sp2E);
            }
        }
    }
}
#endif

// func_80023A68 best match: 92.299% (nonmatchings/func_80023A68-6061209858023118177/base_10.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80023A68.s")

#ifdef NON_MATCHING
void func_80023A68(void *arg0) {
    CourseSelectTempVec3i sp78;
    CourseSelectAnimatedActor *actor;
    CourseSelectRacePlayer *player;
    s32 i;
    s32 move;
    u8 state;

    actor = arg0;
    i = 0;
    if ((s32)D_80121B55 > 0) {
        do {
            state = actor->state[i];
            if (D_8010AF18[4 + i] != state) {
                actor->state[i] = D_8010AF18[4 + i];
                actor->timer[i] = D_8010AF18[0xC + i];
                actor->angle[i] = *(u16 *)&D_8010AF18[(i * 2) + 0x14];
                D_8010AF18[0xC + i] = 0;
                *(s16 *)&D_8010AF18[(i * 2) + 0x14] = 0;
                state = actor->state[i];
            }

            if (D_801235B4 != 0 && state < 5) {
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
                } else if (D_801235B8->screenState == 9) {
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
                    if (D_80121B55 == 1) {
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
                if ((i == 2) && (D_80121B55 == 3)) {
                    D_80121D80[3].pad6[2] = 4;
                }
                state = actor->state[i];
                break;
            case 7:
                if (D_80121D80[i].pad6[2] == 3) {
                    actor->state[i] = 5;
                    state = 5;
                } else if (D_801235B8->screenState == 9) {
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
            func_80097C18(&actor->matrix[i * 0x10], (s16)actor->angle[i]);
            func_80098590(&actor->matrix[i * 0x10], &actor->vecs[i], &sp78);
            *(s32 *)((u8 *)actor + (i * 0x20) + 0x50) = sp78.x;
            *(s32 *)((u8 *)actor + (i * 0x20) + 0x54) = sp78.y;
            *(s32 *)((u8 *)actor + (i * 0x20) + 0x58) = sp78.z;
            D_8010AF18[4 + i] = actor->state[i];
            i++;
        } while (i < (s32)D_80121B55);
    }

    if ((D_80121D80[0].pad6[2] == 4) || (actor->state[0] == 9)) {
        func_800716E4(actor);
        func_800291F0(1);
        D_8010ADE0 = 0;
    } else {
        func_800483FC(&D_80124898, (void (*)(CourseSelectWidgetActor *))func_80023880, actor);
    }
}
#endif

void func_80024050(void *arg0) {
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
        func_80097C18(var_s3, *(s16 *) (var_s4 + 0xEC));
        if (actor && actor) {
        }
        func_80098590(var_s3, var_s7, &sp60);
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
    func_8009853C((s16 *) ((u8 *) actor + 0x1C), 0x400, 0x280);
    func_80071824(actor, func_80023A68);
}

// func_80024168 best match: 94.265% (nonmatchings/func_80024168-7273315160691878794/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80024168.s")

#ifdef NON_MATCHING
void func_80024168(CourseSelectCoursePreviewActor *arg0) {
    FixedTransform sp30;
    u8 sp2F;
    u8 sp2E;
    CourseSelectRacePlayer *temp_v0_3;
    s32 temp_v0_4;
    s8 temp_v0_2;
    int temp_v0;
    u8 temp_v1;
    u8 var_a3;
    u8 var_t0;
    u8 var_v1;

    if ((D_8010AED0 != 0) && (D_80156608 == 1)) {
        var_t0 = 0;
    } else {
        var_t0 = D_80156608;
    }
    temp_v1 = D_8010AECC[var_t0];
    if ((D_8010AECC[var_t0] == 2) || (D_8010AECC[var_t0] & 1)) {
        temp_v0 = arg0->playerFlags[var_t0];
        if ((temp_v0 == 0) || (temp_v0 & 1)) {
            if (D_8010AECC[var_t0] == 3) {
                var_a3 = arg0->playerSlots[var_t0].courseIndex;
            } else {
                var_a3 = D_80121D86[var_t0][0];
            }
            temp_v0_2 = D_800ECA2F[var_t0][var_a3];
            if (temp_v0_2 == -1) {
                var_v1 = 9;
            } else {
                var_v1 = temp_v0_2 & 0xFF;
            }
            if ((D_8010AED0 != 0) && (D_80156608 == 1)) {
                var_v1 = D_8010AED0 - 1;
            }
            temp_v0_3 = &D_80121D80[D_80156608];
            if (temp_v0_3->state == 5) {
                var_v1 = 0;
                var_a3 = ((var_a3 % 3) + 0xC) & 0xFF;
            }
            if (temp_v0_3->courseIndex >= 9) {
                var_v1 = 0;
            }
            sp2E = var_v1;
            sp2F = var_a3;
            func_800987A0(&arg0->sourceTransform, &arg0->playerTransforms[var_t0], &sp30);
            temp_v0_4 = func_8004885C(&sp30);
            arg0->matrix = temp_v0_4;
            if (temp_v0_4 != 0) {
                func_8007C130((void *)temp_v0_4, (s16) sp2F, (s16) sp2E);
            }
        }
    }
}
#endif

// func_80024380 best match: 83.285% (nonmatchings/func_80024380-2225551288923588688/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80024380.s")

#ifdef NON_MATCHING
extern u8 D_800EC9C0;
extern s32 D_8010ADE4;
extern u8 D_8010AECC[];
extern s32 D_8010AEE8[];
extern u8 D_8010AEFB[];
extern s32 D_80124898;

void func_80024380(void *arg0) {
    CourseSelectTempVec3i sp78;
    CourseSelectAnimatedActor *actor;
    CourseSelectRacePlayer *player;
    s32 i;
    s32 move;
    s32 offset;
    u8 state;

    actor = arg0;
    i = 0;
    if ((s32)D_80121B55 > 0) {
        offset = 0;
        do {
            state = D_8010AF18[8 + i];
            if (state != actor->state[i]) {
                actor->state[i] = state;
                actor->timer[i] = D_8010AF18[0x10 + i];
                actor->angle[i] = *(u16 *)&D_8010AF18[offset + 0x1C];
                D_8010AF18[0x10 + i] = 0;
                *(s16 *)&D_8010AF18[offset + 0x1C] = 0;
                state = actor->state[i];
            }

            if (D_801235B4 != 0 && state < 5) {
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
                        if ((s32)((u8 *)player)[0x11] >= 9) {
                            actor->targetCourse[i] = 2;
                        } else {
                            actor->targetCourse[i] = ((u8 *)player)[6] % 3 - 1;
                        }
                    } else {
                        actor->targetCourse[i] = ((u8 *)player)[6] % 3 + 1;
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
                } else if (D_801235B8->screenState == 9) {
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
                    if (D_80121B55 == 1) {
                        ((u8 *)&D_80121D80[0])[8] = 3;
                        D_800EC9C0 = 0x10;
                    }
                }
                state = actor->state[i];
                break;
            case 4:
                if (((u8 *)&D_80121D80[i])[8] == 3) {
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
                ((u8 *)&D_80121D80[i])[8] = 4;
                if (i == 2 && D_80121B55 == 3) {
                    ((u8 *)&D_80121D80[3])[8] = 4;
                }
                state = actor->state[i];
                break;
            case 7:
                if (((u8 *)&D_80121D80[i])[8] == 3) {
                    actor->state[i] = 5;
                    state = 5;
                } else if (D_801235B8->screenState == 9) {
                    actor->state[i] = 8;
                    state = 8;
                }
                break;
            case 8:
                actor->vecs[i].x += 0x200000;
                if (actor->vecs[i].x == 0xC00000) {
                    actor->state[i] = 9;
                    D_801235B8->screenState = 0xB;
                }
                state = actor->state[i];
                break;
            case 9:
                break;
            }

            if ((s32)state < 3) {
                u16 angle = actor->angle[i] + 0x20;
                actor->angle[i] = angle;
                actor->angle[i] = angle & 0xFFF;
            }
            func_80097C18(&actor->matrix[i * 0x10], (s16)actor->angle[i]);
            func_80098590(&actor->matrix[i * 0x10], &actor->vecs[i], &sp78);
            *(s32 *)((u8 *)actor + (i * 0x20) + 0x50) = sp78.x;
            *(s32 *)((u8 *)actor + (i * 0x20) + 0x54) = sp78.y;
            *(s32 *)((u8 *)actor + (i * 0x20) + 0x58) = sp78.z;
            D_8010AF18[8 + i] = actor->state[i];
            i++;
            offset += 2;
        } while (i < (s32)D_80121B55);
    }

    if (((u8 *)&D_80121D80[0])[8] == 4 || actor->state[0] == 9) {
        func_800716E4(actor);
        func_800291F0(2);
        D_8010ADE4 = 0;
    } else {
        func_800483FC(&D_80124898, func_80024168, actor);
    }
}
#endif

void func_80024968(void *arg0) {
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

    var_s3 = (u8 *) &D_8010AF18;
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
        func_80097C18(var_s4, *(s16 *) (var_s5 + 0xEC));
        if ((arg0 && arg0) && arg0) {
        }
        func_80098590(var_s4, var_s7, &sp60);
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
    func_8009853C((s16 *) ((u8 *) actor + 0x1C), 0x400, 0x280);
    func_80071824(actor, func_80024380);
}

// func_80024A90 best match: 90.174% (nonmatchings/func_80024A90-2225551288923588688/base.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80024A90.s")

#ifdef NON_MATCHING
void func_80024A90(CourseSelectIconListActor *arg0) {
    CourseSelectIconListActor *sp8C;
    CourseSelectIconListActor *sp74;
    u8 *sp60;
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
    i = 0;
    if ((s32)D_80121B55 > 0) {
        sp60 = (u8 *)arg0;
        do {
            j = 0;
            if (sp60[0x98] > 0) {
                sp74 = (CourseSelectIconListActor *)((u8 *)sp8C + i);
                do {
                    if (j < (sp60[0x98] - 1)) {
                        position = &arg0->coordinates[(i * 5) + j];
                        player = &D_80121D80[i];
                        alpha = 0;
                        clipOffset = (i >= 2) * 0x8C;
                        if (D_80121B55 == 1) {
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

                        func_8000F0EC(position[0], position[20], func_80043040(D_80112172),
                                      tileIndex & 0xFFFF, 0x20, 0x20, 0, alpha,
                                      arg0->clipLeft - clipOffset, arg0->clipTop, arg0->clipRight,
                                      arg0->clipBottom);
                    } else if (D_80121B55 == 1) {
                        alpha = 0;
                        selectedCourse = D_8010AEA4[i];
                        position = &arg0->coordinates[(i * 5) + j];
                        clipOffset = (i >= 2) * 0x88;
                        if ((selectedCourse > 0) && (selectedCourse < 8) && (D_8010AF46 == 1) &&
                            (selectedCourse & 1)) {
                            alpha = 0xFF;
                        }

                        if (D_800EC9C2 == 3) {
                            func_8000F0EC(position[0], position[20], func_80043040(D_8011217E), 5, 0x20,
                                          0x20, 0, alpha, arg0->clipLeft - clipOffset, arg0->clipTop,
                                          arg0->clipRight, arg0->clipBottom);
                        } else {
                            func_8000F0EC(position[0], position[20], func_80043040(D_80112172), 5, 0x20,
                                          0x20, 0, alpha, arg0->clipLeft - clipOffset, arg0->clipTop,
                                          arg0->clipRight, arg0->clipBottom);
                        }
                    }
                    j++;
                } while (j < sp74->itemCounts[0]);
            }
            i++;
            sp60++;
        } while (i < (s32)D_80121B55);
    }
}
#endif

// func_80024E98 best match: 78.897% (nonmatchings/func_80024E98-7273315160691878794/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80024E98.s")

// func_800257F0 best match: 62.914% (nonmatchings/func_800257F0-7273315160691878794/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_800257F0.s")

#ifdef NON_MATCHING
void func_800257F0(u8 *arg0) {
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

    if ((s32) D_80121B55 < 3) {
        layoutIndex = D_80121B55 - 1;
    } else {
        layoutIndex = 2;
    }
    i = 0;
    if ((s32) D_80121B55 > 0) {
        unlockPtr = D_8010AEA0;
        xyTable = D_800B7084[layoutIndex];
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
            xPair = &D_800B70A8[layoutIndex][(i >= 2) * 2];
            halfPtr[0x40] = xPair[1];
            halfPtr[0x3C] = xPair[0];
            ((s16 *) arg0)[0x52] = 0x78;
            ((s16 *) arg0)[0x51] = 0xA0;
            ((s16 *) arg0)[0x53] = 0x78;
            if (D_80121B55 == 1) {
                ((s16 *) arg0)[0x50] = 0xA0;
            } else if (D_80121B55 == 2) {
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
        } while (i < (s32) D_80121B55);
    }
    func_80071824(arg0, func_80024E98);
}
#endif

// func_80025AA8 best match: 97.722% (nonmatchings/func_80025AA8-4061930211835852828/base_14.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80025AA8.s")

#ifdef NON_MATCHING
void func_80025AA8(u8 *arg0) {
    u8 *actor;
    s32 i;
    u8 *statePtr;
    s16 *posPtr;
    s8 *directionPtr;
    s16 *handles;
    s8 direction;
    s32 tileIndex;
    s32 handleIndex;
    s32 one;
    s32 playerCount;

    actor = arg0;
    if (D_8010AEB0 == 0) {
        playerCount = D_80121B55;
        i = 0;
        if (playerCount > 0) {
            statePtr = arg0;
            handles = D_80112130;
            one = 1;
            do {
                directionPtr = &D_8010AE64[i];
                if (statePtr[0x30] < 2) {
                    posPtr = (s16 *)(actor + (i * 2));
                    if (one == D_80121B55) {
                        tileIndex = 7;
                        handleIndex = 0x25;
                    } else {
                        tileIndex = 0x13;
                        handleIndex = 0x21;
                    }
                    func_8000F8AC(posPtr[0xC],
                                  (s16)(posPtr[0x10] + (statePtr[0x38] * (direction = *directionPtr))),
                                  func_80043040(handles[handleIndex]), tileIndex & 0xFFFF, 0x20, 0x20, 0,
                                  posPtr[0x14], 0);
                }
                i++;
                statePtr++;
            } while (i < D_80121B55);
        }
    }
}
#endif

void func_80025BE4(CourseSelectWidgetActor *arg0) {
    register CourseSelectWidgetActor *actor;
    s32 i;
    CourseSelectWidgetActor *statePtr;
    CourseSelectRacePlayer *player;
    u8 state;

    actor = arg0;
    i = 0;
    if ((s32) D_80121B55 > 0) {
        statePtr = actor; do { if ((D_801235B4 != 0) && (D_800EC9C2 != 3)) { statePtr->unk30 = 4; }
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
                if (D_801235B8->screenState == 9) {
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
            D_8010AF18[i - 1] = state;
        } while (i < (s32) D_80121B55);
    }

    if (actor->unk30 == 4) {
        func_800716E4(actor);
    } else {
        func_800483FC(&D_80124868, func_80025AA8, actor);
    }
}

// func_80025E6C best match: 85.650% (nonmatchings/func_80025E6C-1315772375853892447/base_12.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80025E6C.s")

#ifdef NON_MATCHING
void func_80025E6C(CourseSelectWidgetActor *arg0) {
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

    if (D_80121B55 < 3) {
        idx = D_80121B55 - 1;
    } else {
        idx = 2;
    }

    i = 0;
    if ((s32)D_80121B55 > 0) {
        xyTable = &D_800B7084[idx];
        xTable = &D_800B70A8[idx];
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
            posPtr[0x14] = 0;
            posPtr[0xC] = (*xTable)[((i >= 2) * 2) + 1];
            bytePtr[0x30] = 0;
            bytePtr[0x34] = 0;
            i++;
            bytePtr++;
            posPtr = (s16 *)((u8 *)posPtr + 2);
        } while (i < (s32)D_80121B55);
    }

    func_80071824(actor, func_80025BE4);
}
#endif

void func_80025FA8(CourseSelectWidgetActor *arg0) {
    func_8000F030(arg0->x, arg0->y, func_80043040(D_8011217A), 3, 0x20, 0x20, 0, 0);
    func_8000F030((s16) (arg0->x + 0x40), arg0->y, func_80043040(D_8011217A), 4, 0x20, 0x20, 0, 0);
    func_8000F030(arg0->x, (s16) (arg0->y + 0x40), func_80043040(D_8011217A), 5, 0x20, 0x20, 0, 0);
    func_8000F030((s16) (arg0->x + 0x40), (s16) (arg0->y + 0x40), func_80043040(D_8011217A), 6, 0x20, 0x20, 0, 0);
}

void func_800260E8(CourseSelectWidgetActor *arg0) {
    s32 screenState;
    s32 forceState;

    forceState = 3;
    screenState = D_801235B8->screenState;
    if (screenState == 1) {
        arg0->pad18[4] = 2;
        screenState = D_801235B8->screenState;
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
            D_8010ADDC = func_80071408(func_80025E6C, 0, 0x64);
            func_80071408(func_800275A4, 0, 0x63);
            func_80071408(func_800273C4, 0, 0x61);
            func_80071408(func_80026B88, 0, 0x64);
            if ((D_800EC9C2 == 3) && (D_800EC9E6 == 0)) {
                func_80071408(func_8002E568, 0, 0x63);
            }
        }
        break;
    case 1:
        if (D_80121D88 == 3) {
            arg0->pad18[4] = 2;
        }
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0x10D) {
            if (D_801235B8->screenState != 0) {
                arg0->pad18[4] = 4;
                D_801235B8->screenState = 2;
            } else {
                arg0->pad18[4] = 3;
            }
        }
        break;
    case 4:
        if (D_801235B8->screenState == 9) {
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
        D_801235B8->screenState = 4;
        arg0->pad18[4] = 7;
        break;
    case 7:
        D_801235B8->screenState = 5;
        arg0->pad18[4] = 1;
        break;
    }

    if (arg0->pad18[4] == 3) {
        func_800716E4(arg0);
        func_800291F0(4);
        return;
    }

    func_800483FC(&D_80124868, func_80025FA8, arg0);
}

void func_800263A4(CourseSelectWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = 8;
    func_80071824(arg0, func_800260E8);
}

// func_800263D8 best match: 98.209% (nonmatchings/func_800263D8-4139837607000619032/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_800263D8.s")

#ifdef NON_MATCHING
extern int sprintf(u8 *, u8 *, ...);

void func_800263D8(CourseSelectWidgetActor *arg0) {
    volatile u16 frameTile;
    u8 text[2];
    s32 playerCount;
    s32 i;
    s32 courseId;
    s32 selectedCourseId;
    s32 j;
    s32 xOffset;
    s32 positionColumn;
    s32 bottomRow;
    u8 *ratings;
    CourseSelectWidgetActor *playerWidget;

    playerCount = D_80121B55;
    if (playerCount < 3) {
        frameTile = 0x10;
    } else {
        frameTile = 0x11;
    }

    i = 0;
    if (playerCount > 0) {
        do {
            if ((playerCount >= 2 || D_80121D80[i].pad6[2] == 0) &&
                (playerCount >= 2 || (playerCount == 1 && D_8010AF46 == 0))) {
                playerWidget = (CourseSelectWidgetActor *)((u8 *)arg0 + (i * 2));
                courseId = D_80121D80[i].pad6[0];
                if (!playerCount) {
                }
                if (courseId >= 9 && courseId < 12) {
                    xOffset = (u16)courseId;
                } else {
                    xOffset = (u16)(courseId % 3);
                }

                func_8000F8AC(playerWidget->coordinates[0], playerWidget->coordinates[4],
                              func_80043040(D_80112130[0x21]), frameTile, 0x20, 0x20, 0, arg0->coordinates[8], 0);

                if (D_80121B55 < 3) {
                    selectedCourseId = xOffset;
                    if (selectedCourseId >= 9) {
                        func_80013D0C((s16)(playerWidget->coordinates[0] + 0x38), playerWidget->coordinates[4],
                                      "?", 0, arg0->coordinates[8]);
                    } else {
                        ratings = &D_800B706C[selectedCourseId * 3];
                        j = 0;
                        if (ratings[0] > ((0, 0))) {
                            xOffset = 0;
                            do {
                                func_8000F8AC((s16)(playerWidget->coordinates[0] + xOffset + 0x38),
                                              (s16)(playerWidget->coordinates[4] - 2),
                                              func_80043040(D_80112130[0x24]), 0x25, 0x20, 0x20, 0,
                                              arg0->coordinates[8], 0);
                                j++;
                                xOffset += 0xC;
                            } while (j < ratings[0]);
                        }
                    }

                    if (selectedCourseId >= 9) {
                        func_80013D0C((s16)(playerWidget->coordinates[0] + 0x38),
                                      (s16)(playerWidget->coordinates[4] + 0xC), "?", 0, arg0->coordinates[8]);
                    } else {
                        ratings = &D_800B706C[selectedCourseId * 3];
                        if (1) {
                        }
                        if (1) {
                        }
                        j = 0;
                        if (ratings[1] > 0) {
                            xOffset = 0;
                            do {
                                func_8000F8AC((s16)(playerWidget->coordinates[0] + xOffset + 0x38),
                                              (s16)(playerWidget->coordinates[4] + 0xA),
                                              func_80043040(D_80112130[0x24]), 0x25, 0x20, 0x20, 0,
                                              arg0->coordinates[8], 0);
                                j++;
                                xOffset += 0xC;
                            } while (j < ratings[1]);
                        }
                    }

                    if (selectedCourseId >= 9) {
                        func_80013D0C((s16)(playerWidget->coordinates[0] + 0x38),
                                      (s16)(playerWidget->coordinates[4] + 0x18), "?", 0, arg0->coordinates[8]);
                    } else {
                        ratings = &D_800B706C[selectedCourseId * 3];
                        j = 0;
                        if (ratings[2] > 0) {
                            xOffset = 0;
                            do {
                                func_8000F8AC((s16)(playerWidget->coordinates[0] + xOffset + 0x38),
                                              (s16)(playerWidget->coordinates[4] + 0x16),
                                              func_80043040(D_80112130[0x24]), 0x25, 0x20, 0x20, 0,
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
                        sprintf(text, "%d", D_800B706C[selectedCourseId * 3]);
                        do {
                        } while (0);
                    }
                    func_80013D0C((s16)(playerWidget->coordinates[0] + 0x34), playerWidget->coordinates[4], text, 0,
                                  arg0->coordinates[8]);

                    if (selectedCourseId < 9) {
                        sprintf(text, "%d", D_800B706D[selectedCourseId * 3]);
                    }
                    func_80013D0C((s16)(playerWidget->coordinates[0] + 0x34),
                                  (s16)(playerWidget->coordinates[4] + 8), text, 0, arg0->coordinates[8]);

                    if (9 > selectedCourseId) {
                        sprintf(text, "%d", D_800B706E[selectedCourseId * 3]);
                    }
                    func_80013D0C((s16)(playerWidget->coordinates[0] + 0x34),
                                  (s16)(playerWidget->coordinates[4] + 0x10), text, 0, arg0->coordinates[8]);
                }

                playerCount = D_80121B55;
                if (playerCount >= 2) {
                    positionColumn = 1;
                    if (playerCount == 2) {
                        positionColumn = 0;
                    }
                    bottomRow = i >= 2;
                    func_8000F8AC((s16)(D_800B70D8[positionColumn * 2 + (i & 1)][0] + (bottomRow * 0x8C)),
                                  D_800B70D8[positionColumn * 2 + (i & 1)][1],
                                  func_80043040(D_80112130[0x21]), 0x12, 0x20, 0x20, 0, arg0->coordinates[8], 0);
                    playerCount = D_80121B55;
                }
            }
            i++;
        } while (i < playerCount);
    }
}
#endif

void func_80026A54(CourseSelectWidgetActor *arg0) {
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
        if (D_80121D88 == 3) {
            arg0->transitionState = 2;
        }
        if (D_801235B8->screenState == 9) {
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
        func_800716E4(arg0);
        func_800291F0(5);
        return;
    }

    func_800483FC(&D_80124868, func_800263D8, arg0);
}

void func_80026B88(CourseSelectWidgetActor *arg0) {
    s32 row;
    s32 i;
    CourseSelectWidgetActor *temp_a3;
    s16 (*table)[4];

    temp_a3 = arg0;
    if (D_80121B55 < 3) {
        row = D_80121B55 - 1;
    } else {
        row = 2;
    }

    i = 0;
    if ((s32)D_80121B55 > 0) {
        table = &D_800B70C0[row];
        do {
            temp_a3->coordinates[i] = (*table)[(i & 1) * 2] + ((i >= 2) * 0x8C);
            temp_a3->coordinates[i + 4] = (*table)[(i & 1) * 2 + 1];
            i++;
        } while (i < (s32)D_80121B55);
    }

    temp_a3->transitionOffset = 0;
    temp_a3->transitionState = 0;
    func_80071824(temp_a3, func_80026A54);
}

// func_80026C4C best match: 86.127% (nonmatchings/func_80026C4C-4923837976568703863/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80026C4C.s")

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

extern u32 D_800B34B0[];
extern u8 D_800B6BE4[];
extern u8 D_800B6DD0[];
extern u8 D_800B6DFC[];
extern u8 D_800B6EBC[];
extern u8 D_800B6FDC[];
extern u8 D_800F42C7;
extern u8 D_8010AF07[];

void func_80026C4C(CourseSelectWidgetActor *arg0) {
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

    status = (CourseSelectStatus26C4C *)D_8010AF18;
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
            text = D_800B6BE4 + (value * 0x46);
        } else {
            if ((D_800EC9D0[0] < 2) || (D_800EC9D0[0] == 9)) {
                selectedIndex = 1;
            } else if (D_800EC9D0[0] < 5) {
                selectedIndex = D_800EC9D0[0] - 1;
            }

            if ((D_800EC9D0[0] >= 5) && (D_800EC9D0[0] != 9)) {
                arg0->subState = 1;
            }

            if (arg0->subState == 0) {
                arg0->timer = selectedIndex;
            } else {
                selectedIndex = arg0->timer;
            }

            if ((D_800EC9C2 == 3) && (selection->courseId < 9)) {
                text = D_800B6DFC + ((selection->courseId % 3) * 0x30);
            } else if ((selection->courseId >= 9) && (selection->courseId < 12)) {
                text = D_800B6EBC + ((D_8010AF07[selectedIndex] % 3) * 0x60);
            } else {
                text = D_800B6DD0;
            }
        }

        func_80013154(arg0->x, arg0->y, text, 1, arg0->spriteIndex, 0);

        if ((D_800EC9C2 == 3) && ((selection->mode == 1) || (selection->mode == 2))) {
            if ((D_8010AE64[0] != 3) || !(D_800F42C7 & 7)) {
                buffer[0] = -4;
                buffer[1] = 6;
                buffer[2] = selectedIndex;
                buffer[3] = -1;
                func_80013154((s16)(arg0->x + 0x48), (s16)(arg0->y + 0x10), (u8 *)buffer, 1, arg0->spriteIndex, 0);
            }

            if (D_800EC9E6 == 0) {
                buffer[0] = -4;
                buffer[1] = 6;
                if ((selectedIndex >= 2) || (selection->courseId >= 9)) {
                    if (selection->courseId >= 9) {
                        courseIndex = D_8010AF07[selectedIndex];
                    } else {
                        courseIndex = ((selection->courseId % 3) + (selectedIndex * 3)) - 3;
                    }

                    price = D_800B34B0[courseIndex];
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
                func_80013154((s16)(arg0->x + 0x20), (s16)(arg0->y + 0x20), (u8 *)buffer, 1, arg0->spriteIndex, 0);
            }
        }
    } else {
        text = D_800B6FDC + ((status->unk2C * 0x32) - 0x32);
        func_80013154(arg0->x, arg0->y, text, 1, arg0->spriteIndex, 0);
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_800271CC.s")

void func_800273C4(CourseSelectWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = 0xC;
    arg0->spriteIndex = 0;
    arg0->state = 0;
    arg0->subState = 0;
    arg0->timer = 0;
    func_80071824(arg0, func_800271CC);
}

void func_80027408(CourseSelectWidgetActor *arg0) {
    u16 tileIndexSpill;
    s32 handleIndex;
    u16 unusedPadding;
    u16 tileIndex;
    u32 promotedTileIndex;

    if (D_800EC9E6 == 1) {
        tileIndex = 1;
        handleIndex = 0x27;
    } else {
        tileIndex = 0;
        handleIndex = 0x21;
    }

    tileIndexSpill = tileIndex;
    promotedTileIndex = tileIndexSpill;
    func_8000F8AC((*arg0).coordinates[0], arg0->coordinates[1], func_80043040(D_80112130[handleIndex]),
                  promotedTileIndex, 0x20, 0x20, 0, arg0->coordinates[2], 0);
}

void func_80027498(CourseSelectWidgetActor *arg0) {
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
        if ((D_80121D88 == 3) || (D_801235B8->screenState == 9)) {
            state = (arg0->state = 2);
        }
        break;
    case 2:
        arg0->x += 0x20;
        break;
    }
    state = arg0->state;
    if ((state == 2) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        func_800291F0(7);
        return;
    }
    func_800483FC(&D_80124868, func_80027408, arg0);
}

void func_800275A4(CourseSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x5C;
    arg0->spriteIndex = 0;
    arg0->state = 0;
    func_80071824(arg0, func_80027498);
}

// func_800275E0 best match: 89.545% (nonmatchings/func_800275E0-1315772375853892447/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_800275E0.s")

#ifdef NON_MATCHING
void func_800275E0(CourseSelectWidgetActor *arg0) {
    s32 playerIndex;
    s32 playerOffset;
    s32 alpha;
    s32 iconIndex;
    s32 rightSideOffset;
    s16 *state;
    CourseSelectWidgetActor *countActor;
    CourseSelectWidgetActor *slotActor;
    CourseSelectWidgetActor *alphaActor;

    playerIndex = 0;
    if ((s32)D_80121B55 > 0) {
        state = D_800EC9D0;
        playerOffset = 0;
        do {
            if (*state != 0) {
                countActor = (CourseSelectWidgetActor *)((u8 *)arg0 + playerIndex);
                if (*state != 9) {
                    iconIndex = countActor->itemCounts[0] - 1;
                    rightSideOffset = playerIndex < 2;
                    if (iconIndex >= 0) {
                        rightSideOffset ^= 1;
                        slotActor = (CourseSelectWidgetActor *)((u8 *)arg0 + (playerIndex * 6) + (iconIndex * 2));
                        rightSideOffset = (((rightSideOffset * 9) * 4) - rightSideOffset) * 4;
                        do {
                            if (*state == 1) {
                                alpha = 0x100;
                            } else if ((iconIndex + 2) == *state) {
                                alpha = 0x100;
                            } else {
                                alpha = 0x60;
                            }
                            func_8000F970(slotActor->coordinates[0], slotActor->coordinates[0xC],
                                          func_80043040(D_80112130[0x21]), slotActor->coordinates[0x28],
                                          0x20, 0x20, 0, alpha, 0, arg0->coordinates[0x18] - rightSideOffset,
                                          arg0->coordinates[0x1A], arg0->coordinates[0x19],
                                          arg0->coordinates[0x1B]);
                            iconIndex--;
                            slotActor = (CourseSelectWidgetActor *)((u8 *)slotActor - 2);
                        } while (iconIndex >= 0);
                    }
                    if (D_8010AEB0 == 0) {
                        if ((*state >= 2) && (*state < 5)) {
                            slotActor = (CourseSelectWidgetActor *)((u8 *)arg0 + (playerIndex * 6));
                            alphaActor = (CourseSelectWidgetActor *)((u8 *)arg0 + playerOffset);
                            if ((s32)D_80121B55 == 1) {
                                alpha = 0xF;
                            } else {
                                alpha = 0x1B;
                            }
                            func_8000F8AC(slotActor->coordinates[0],
                                          (s16)(slotActor->coordinates[0xC] + (countActor->pad18[0x38] * (*state - 2))),
                                          func_80043040(D_80112130[0x21]), alpha & 0xFFFF, 0x20, 0x20, 0,
                                          alphaActor->coordinates[0x24], 0);
                        }
                    }
                }
            }
            playerIndex++;
            playerOffset += 2;
            state++;
        } while (playerIndex < (s32)D_80121B55);
    }
}
#endif

void func_8002784C(CourseSelectWidgetActor *arg0) {
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
    } else if (D_801235B8->screenState == 4) {
        func_80071824(temp_a2, func_80027AF8);
    }
    func_800483FC(&D_80124868, func_800275E0, temp_a2);
}

void func_80027914(CourseSelectWidgetActor *arg0) {
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
        var_v0_3 = D_801235B8->screenState;
    } else {
        var_v0_3 = D_801235B8->screenState;
        if (var_v0_3 == 3) {
            func_80071824(arg0, func_8002784C);
            var_v0_3 = D_801235B8->screenState;
        }
    }
    if (var_v0_3 == 9) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_800275E0, arg0);
}

void func_80027A08(CourseSelectWidgetActor *arg0) {
    s32 i;
    s32 j;
    CourseSelectWidgetActor *temp_a2 = arg0;

    i = 0;
    if ((s32)D_80121B55 > 0) {
        do {
            j = 0;
            if ((s32)arg0->itemCounts[i] > 0) {
                do {
                    temp_a2->coordinateRows[i][j] -= 0x20;
                    j++;
                } while (j < (s32)arg0->itemCounts[i]);
            }
            i++;
        } while (i < (s32)D_80121B55);
    }

    if ((D_80121B55 == 1) && (temp_a2->itemCount == 0)) {
        func_800716E4(temp_a2);
        return;
    }

    if (temp_a2->x < -0xDF) {
        func_800716E4(temp_a2);
        func_800291F0(8);
        D_8010ADE8 = 0;
        return;
    }

    func_800483FC(&D_80124868, func_800275E0, temp_a2);
}

// func_80027AF8 best match: 90.349% (nonmatchings/func_80027AF8-7273315160691878794/base_11.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80027AF8.s")

// func_80028194 best match: 83.366% (nonmatchings/func_80028194-2225551288923588688/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80028194.s")

#ifdef NON_MATCHING
void func_80028194(CourseSelectWidgetActor *arg0) {
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
    s32 temp_s3;
    s32 temp_s2;
    s32 temp_a3;

    if ((s32)D_80121B55 < 3) {
        var_t5 = D_80121B55 - 1;
    } else {
        var_t5 = 2;
    }
    var_t0 = 0;
    if ((s32)D_80121B55 > 0) {
        temp_ra = D_800B70A8[var_t5];
        temp_s1 = D_800B7084[var_t5];
        var_t1 = (u8 *)arg0;
        var_t2 = (u8 *)arg0;
        var_t3 = (u8 *)arg0;
        temp_s6 = 0xC;
        temp_s4 = 1;
        temp_s3 = 0x100;
        temp_s2 = 0xC;
        temp_a3 = 3;
        do {
            var_v1 = 0;
            if ((s32)D_80121B55 >= 3) {
                var_t1[0x50] = temp_s2;
            } else {
                var_t1[0x50] = *(s16 *)((u8 *)D_800B7084 + (var_t5 * temp_s6));
            }
            var_t1[0x5C] = 0;
            *(s16 *)(var_t3 + 0x60) = temp_s3;
            if (D_80121B55 == temp_s4) {
                var_a0 = 8;
            } else {
                var_a0 = 4;
            }
            var_v0 = var_t2;
loop:
            var_v1 += 1;
            *(s16 *)(var_v0 + 0x18) = temp_ra[((var_t0 >= 2) * 2) + 1] + var_a0;
            var_v0 += 2;
            *(s16 *)(var_v0 + 0x2E) = temp_s1[((var_t0 & 1) * 2) + 2];
            if (var_v1 != temp_a3) {
                goto loop;
            }
            var_t0 += 1;
            var_t1 += 1;
            var_t2 += 6;
            var_t3 += 2;
        } while (var_t0 < (s32)D_80121B55);
    }

    *(s16 *)((u8 *)arg0 + 0x4A) = 0xA0;
    *(s16 *)((u8 *)arg0 + 0x4C) = 0x78;
    *(s16 *)((u8 *)arg0 + 0x4E) = 0x78;
    if (D_80121B55 == 1) {
        *(s16 *)((u8 *)arg0 + 0x48) = 0xA0;
    } else if (D_80121B55 == 2) {
        *(s16 *)((u8 *)arg0 + 0x48) = 0x7E;
    } else {
        *(s16 *)((u8 *)arg0 + 0x48) = 0x88;
    }
    func_80071824(arg0, func_80027AF8);
}
#endif

// func_80028354 best match: 96.854% (nonmatchings/func_80028354-6061209858023118177/base_13.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80028354.s")

#ifdef NON_MATCHING
extern int sprintf(u8 *, u8 *, ...);

void func_80028354(CourseSelectWidgetInitActor *actor) {
    s16 *yPtr;
    volatile s32 savedIndex;
    s32 count;
    u8 text[4];
    s32 nextIndex;
    s32 offset;
    s32 j;
    s32 middleCount;
    s32 edgeOffset;
    s32 tile;
    s32 alpha;
    s32 i;
    CourseSelectWidgetInitActor *cur;

    if (D_80121B55 == 2) {
        count = 2;
    } else {
        count = 4;
    }

    i = 0;
    cur = actor;
    if (count > 0) {
        do {
            j = 0;
            alpha = 0x100;
            yPtr = &cur->unk20;
            if (i == D_80121B55) {
                alpha = 0xC0;
            }
            if (i == 0) {
                tile = 2;
            } else {
                tile = (i + 5) & 0xFF;
            }

            savedIndex = i;
            func_8000F8AC(cur->unk18, *yPtr, func_80043040(D_80112130[0x21]), 0x22, 0x20, 0x20, 0,
                          alpha, tile);

            if (D_80121B55 == 2) {
                middleCount = 0xB;
            } else {
                middleCount = 3;
            }
            edgeOffset = 0x38;
            if (middleCount > 0) {
                do {
                    func_8000F8AC((s16)(cur->unk18 + edgeOffset), *yPtr, func_80043040(D_80112130[0x21]),
                                  0x23, 0x20, 0x20, 0, alpha, tile);
                    j++;
                    edgeOffset += 0x10;
                } while (j != middleCount);
            }

            func_8000F8AC((s16)(cur->unk18 + edgeOffset), *yPtr, func_80043040(D_80112130[0x21]), 0x24,
                          0x20, 0x20, 0, alpha, tile);

            offset = 0;
            if (D_80121B55 == 2) {
                if (1) {
                    edgeOffset = 0xF8;
                }
            } else {
                edgeOffset = 0x78;
            }
            do {
                func_8000F8AC(cur->unk18, (s16)(*yPtr + offset + 0x10), func_80043040(D_80112130[0x21]),
                              0x25, 0x20, 0x20, 0, alpha, tile);
                func_8000F8AC((s16)(cur->unk18 + edgeOffset), (s16)(*yPtr + offset + 0x10),
                              func_80043040(D_80112130[0x21]), 0x26, 0x20, 0x20, 0, alpha, tile);
                offset += 0x10;
            } while (offset < 0x40);

            func_8000F8AC(cur->unk18, (s16)(*yPtr + 0x50), func_80043040(D_80112130[0x21]), 0x27, 0x20,
                          0x20, 0, alpha, tile);

            if (D_80121B55 == 2) {
                middleCount = 0xE;
            } else {
                middleCount = 6;
            }
            j = 0;
            if (middleCount > 0) {
                offset = 0;
                do {
                    func_8000F8AC((s16)(cur->unk18 + offset + 0x10), (s16)(cur->unk20 + 0x50),
                                  func_80043040(D_80112130[0x21]), 0x28, 0x20, 0x20, 0, alpha, tile);
                    j++;
                    offset += 0x10;
                } while (j != middleCount);
            }

            func_8000F8AC((s16)(cur->unk18 + edgeOffset - 8), (s16)(*yPtr + 0x50),
                          func_80043040(D_80112130[0x21]), 0x29, 0x20, 0x20, 0, alpha, tile);

            nextIndex = savedIndex + 1;
            sprintf(text, "%d", nextIndex);
            func_80013D0C((s16)(cur->unk18 + 0x34), (s16)(*yPtr + 2), text, 0, alpha);

            if (alpha == 0xC0) {
                func_8000F8AC((s16)(cur->unk18 + 2), (s16)(cur->unk20 + 0x24),
                              func_80043040(D_80112130[0x1F]), 0x90, 0x20, 0x20, 0, 0xF0, 0);
            }
            i = nextIndex;
            cur = (CourseSelectWidgetInitActor *)((u8 *)cur + sizeof(s16));
            savedIndex = nextIndex;
        } while (nextIndex != count);
    }
}
#endif

// func_800287EC best match: 82.540% (nonmatchings/func_800287EC-8207005055717715604/base_15.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_800287EC.s")

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

void func_800287EC(CourseSelectWidgetActor *arg0) {
    volatile u8 pad[0x20];
    CourseSelectWidgetTransitionActor *actor;
    s32 i;
    s32 count;
    s32 next;
    s32 step;
    u8 *statePtr;
    u8 *modePtr;

    actor = (CourseSelectWidgetTransitionActor *)arg0;
    modePtr = &D_80121B55;
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
                                D_8010ADE0 = func_80071408((void (*)(EffectTask *))func_80024050, 0, 0x62);
                                D_8010ADE4 = func_80071408((void (*)(EffectTask *))func_80024968, 0, 0x62);
                                func_80071408(func_800257F0, 0, 0x62);
                            }
                            break;
                        }
                    } while (step != 0x18);
                    break;
                case 1:
                    if (*(&D_80121D88 + (i * sizeof(CourseSelectRacePlayer))) == 4) {
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
            }
            i = next;
            statePtr++;
        } while (next != count);
    }

    func_800483FC(&D_80124868, func_80028354, arg0);
}
#endif

// func_80028B0C best match: 92.500% (nonmatchings/func_80028B0C-180949888360117632/base_9.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80028B0C.s")

#ifdef NON_MATCHING
void func_80028B0C(CourseSelectWidgetInitActor *arg0) {
    if (D_80121B55 == 2) {
        arg0->unk40 = 0xA0;
        arg0->unk18 = 0x90;
        arg0->unk20 = -0xC8;
        arg0->unk1A = -0x18C;
        arg0->unk22 = 0x6C;
        arg0->unk28 = -0x7E;
        arg0->unk2A = -0x7E;
        arg0->unk32 = 1;
        arg0->unk38 = 1;
        arg0->unk30 = -1;
        arg0->unk3A = -1;
        arg0->unk42 = 0x10E;
        arg0->unk44 = 0x10E;
        arg0->unk4A = 0x68;
        arg0->unk4C = 0x68;
    } else {
        arg0->unk18 = -0x118;
        arg0->unk1A = -0x118;
        arg0->unk30 = 1;
        arg0->unk38 = 1;
        arg0->unk32 = 1;
        arg0->unk3C = 1;
        arg0->unk40 = -0x128;
        arg0->unk20 = -0xC8;
        arg0->unk22 = 0x6C;
        arg0->unk1C = 0x94;
        arg0->unk24 = -0xC8;
        arg0->unk1E = 0x94;
        arg0->unk26 = 0x6C;
        arg0->unk28 = -0x88;
        arg0->unk2A = -0x88;
        arg0->unk2C = 4;
        arg0->unk2E = 4;
        arg0->unk3A = -1;
        arg0->unk34 = -1;
        arg0->unk36 = -1;
        arg0->unk3E = -1;
        arg0->unk42 = 0x90;
        arg0->unk4A = 0x68;
        arg0->unk44 = 0x90;
        arg0->unk4C = 0x68;
        arg0->unk46 = 0x90;
        arg0->unk4E = 0x68;
        arg0->unk48 = 0x90;
        arg0->unk50 = 0x68;
    }
    arg0->unk54 = 0;
    arg0->unk5B = 0;
    arg0->unk56 = 0;
    arg0->unk5C = 0;
    arg0->unk58 = 0;
    arg0->unk5D = 0;
    arg0->unk52 = 0;
    arg0->unk5A = 0;
    func_80071824(arg0, func_800287EC);
}
#endif

void func_80028C68(CourseSelectPlayerPanelsActor *actor) {
    s32 i;
    s32 playerCount;
    s32 xOffset;
    s32 yOffset;
    s32 two;
    u32 rightEdgeTile;

    playerCount = D_80121B55;
    two = 2;
    rightEdgeTile = 2;
    for (i = 0; i < playerCount; i++) {
        if (actor->playerPanelFadeAlpha[i] != 0) {
            if (two == playerCount) {
                yOffset = i * 0x64;
                func_8000F8AC(actor->x, (s16)(actor->y + yOffset), func_80043040(D_80112130[0x24]), 0, 0x20,
                              0x20, 0, actor->playerPanelFadeAlpha[i], 0);
                func_8000F8AC((s16)(actor->x + 0x40), (s16)(actor->y + yOffset),
                              func_80043040(D_80112130[0x24]), 1, 0x20, 0x20, 0,
                              actor->playerPanelFadeAlpha[i], 0);
                func_8000F8AC((s16)(actor->x + 0x78), (s16)(actor->y + yOffset),
                              func_80043040(D_80112130[0x24]), 1, 0x20, 0x20, 0,
                              actor->playerPanelFadeAlpha[i], 0);
                func_8000F8AC((s16)(actor->x + 0xB0), (s16)(actor->y + yOffset),
                              func_80043040(D_80112130[0x24]), rightEdgeTile, 0x20, 0x20, 0,
                              actor->playerPanelFadeAlpha[i], 0);
                func_80013154((s16)(actor->x + 0x2E), (s16)(actor->y + yOffset + 0xC), D_800B7040, 0,
                              actor->playerPanelFadeAlpha[i], 0);
            } else {
                xOffset = (i >= 2) * 0x8C;
                yOffset = (i & 1) * 0x64;
                func_8000F8AC((s16)(actor->x + xOffset), (s16)(actor->y + yOffset),
                              func_80043040(D_80112130[0x24]), 8, 0x20, 0x20, 0,
                              actor->playerPanelFadeAlpha[i], 0);
                func_8000F8AC((s16)(actor->x + xOffset + 0x40), (s16)(actor->y + yOffset),
                              func_80043040(D_80112130[0x24]), 9, 0x20, 0x20, 0,
                              actor->playerPanelFadeAlpha[i], 0);
                func_80013154((s16)(actor->x + xOffset + 0x24), (s16)(actor->y + yOffset + 3), D_800B7040, 1,
                              actor->playerPanelFadeAlpha[i], 0);
            }
        }
        playerCount = D_80121B55;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80028FF0.s")

void func_8002916C(CourseSelectWidgetActor *arg0) {
    s32 var_v0;
    CourseSelectWidgetActor *var_v1;

    if (D_80121B55 == 2) {
        arg0->x = -0x6E;
        arg0->y = -0x4C;
    } else {
        arg0->x = -0x8C;
        arg0->y = -0x46;
    }
    var_v0 = 0;
    var_v1 = arg0;
    if ((s32)D_80121B55 > 0) {
        do {
            var_v1->spriteIndex = 0;
            var_v0++;
            var_v1 = (CourseSelectWidgetActor *)((u8 *)var_v1 + sizeof(s16));
        } while (var_v0 < (s32)D_80121B55);
    }
    func_80071824(arg0, func_80028FF0);
}

void func_800291F0(s32 arg0) {

}
