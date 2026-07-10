#include "common.h"
#include "memory_allocator.h"
#include "effect_task_scheduler.h"
#include "menu_rendering.h"
#include "course_select_ui.h"
#include "fixed_point_matrix.h"

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
    /* 0x006 */ u8 pad6[0x606];
} CourseSelectRacePlayer;

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

extern void func_800483FC(void *, void (*)(CourseSelectWidgetActor *), CourseSelectWidgetActor *);
extern void func_80023880(void *);
extern void func_80025AA8(CourseSelectWidgetActor *);
extern void func_8002E568(CourseSelectWidgetActor *);
extern s8 D_800EC9C0;
extern u8 D_800EC9C2;
extern u8 D_800EC9E6;
extern s32 D_8010ADE0;
extern s32 D_8010ADDC;
extern s32 D_8010ADE8;
extern s8 D_8010AE64[];
extern u8 D_8010AECC[];
extern s32 D_8010AEE8[];
extern u8 D_8010AEFB[];
extern u8 D_8010AEB0;
extern u8 D_8010AEA0[];
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
extern s16 D_8011217A;
extern u8 D_80121B55;
extern CourseSelectRacePlayer D_80121D80[];
extern u8 D_80121D88;
extern s32 D_801235B4;
extern CourseSelectState *D_801235B8;
extern s32 D_80124868;
extern s32 D_80124898;

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80023880.s")

// func_80023A68 best match: 76.253% (nonmatchings/func_80023A68-4923837976568703863/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80023A68.s")

#ifdef NON_MATCHING
void func_80023A68(void *arg0) {
    CourseSelectTempVec3i sp78;
    s32 sp64;
    CourseSelectTempVec3i *sp40;
    s16 *sp44;
    void *actor;
    u8 *var_s1;
    u8 *var_s2;
    u8 *var_s3;
    u8 *var_s6;
    u8 *var_s7;
    s32 i;
    s32 statusHalfIndex;
    u8 state;
    s32 step;
    s32 *targetVelocity;
    CourseSelectRacePlayer *player;

    actor = arg0;
    i = 0;
    if ((s32)D_80121B55 > 0) {
        var_s6 = D_8010AF18;
        sp44 = (s16 *)((u8 *)arg0 + 0x3C);
        sp40 = (CourseSelectTempVec3i *)((u8 *)arg0 + 0xBC);
        statusHalfIndex = 0;
        var_s1 = arg0;
        var_s2 = arg0;
        var_s3 = arg0;
        var_s7 = arg0;
        do {
            if (var_s6[4] != var_s1[0x100]) {
                var_s1[0x100] = var_s6[4];
                var_s1[0xFC] = var_s6[0xC];
                *(s16 *)(var_s2 + 0xEC) = *(u16 *)(D_8010AF18 + statusHalfIndex + 0x14);
                var_s6[0xC] = 0;
                *(s16 *)(D_8010AF18 + statusHalfIndex + 0x14) = 0;
            }

            state = var_s1[0x100];
            if (D_801235B4 != 0) {
                if ((s32)state < 5) {
                    var_s1[0x100] = 4;
                    *(s16 *)(var_s2 + 0xEC) = 0;
                    state = var_s1[0x100];
                }
            }

            if (state < 10) {
                switch (state) {
                case 0:
                    *(s32 *)(var_s3 + 0xBC) += -0x200000;
                    if (*(s32 *)(var_s3 + 0xBC) <= 0) {
                        *(s32 *)(var_s3 + 0xBC) = 0;
                        var_s1[0x100] = 1;
                    }
                    state = var_s1[0x100];
                    break;
                case 1:
                    if (D_8010AECC[i] & 1) {
                        targetVelocity = &D_8010AEE8[i];
                        player = &D_80121D80[i];
                        if (*targetVelocity < 0) {
                            if ((s32)player->pad6[0xB] >= 9) {
                                *(s16 *)(var_s2 + 0xF4) = 2;
                            } else {
                                *(s16 *)(var_s2 + 0xF4) = (player->pad6[0] % 3) - 1;
                            }
                        } else {
                            *(s16 *)(var_s2 + 0xF4) = (player->pad6[0] % 3) + 1;
                        }
                        if (*(s16 *)(var_s2 + 0xF4) < 0) {
                            *(s16 *)(var_s2 + 0xF4) = 2;
                        }
                        if (*(s16 *)(var_s2 + 0xF4) == 3) {
                            *(s16 *)(var_s2 + 0xF4) = D_8010AEFB[i * 4];
                        }
                        if (*(s16 *)(var_s2 + 0xF4) == 8) {
                            *(s16 *)(var_s2 + 0xF4) = 2;
                        }
                        if (D_8010AECC[i] == 3) {
                            *(s32 *)(var_s3 + 0xC0) = -*targetVelocity;
                        }
                        var_s1[0x100] = 2;
                        state = 2;
                    } else if (D_801235B8->screenState == 9) {
                        var_s1[0x100] = 8;
                        state = 8;
                    }
                    break;
                case 2:
                    targetVelocity = &D_8010AEE8[i];
                    step = 0x200000;
                    if (*targetVelocity < 0) {
                        step = -0x200000;
                    }
                    *(s32 *)(var_s3 + 0xC0) += step;
                    if (*targetVelocity == step) {
                        var_s1[0x100] = 1;
                    }
                    state = var_s1[0x100];
                    break;
                case 3:
                    var_s1[0xFC]++;
                    if (D_800EC9C0 != 0) {
                        D_800EC9C0 = 1;
                    }
                    if (var_s1[0xFC] == 0xF) {
                        var_s1[0xFC] = 0;
                        var_s1[0x100] = 4;
                        if (D_80121B55 == 1) {
                            D_80121D80[0].pad6[2] = 3;
                            D_800EC9C0 = 0x10;
                        }
                    }
                    state = var_s1[0x100];
                    break;
                case 4:
                    if (D_80121D80[i].pad6[2] == 3) {
                        var_s1[0x100] = 5;
                        state = 5;
                    }
                    break;
                case 5:
                    *(s32 *)(var_s3 + 0xBC) += 0x200000;
                    if (*(s32 *)(var_s3 + 0xBC) == 0x1000000) {
                        var_s1[0x100] = 6;
                    }
                    state = var_s1[0x100];
                    break;
                case 6:
                    D_80121D80[i].pad6[2] = 4;
                    if ((i == 2) && (D_80121B55 == 3)) {
                        D_80121D80[3].pad6[2] = 4;
                    }
                    state = var_s1[0x100];
                    break;
                case 7:
                    if (D_80121D80[i].pad6[2] == 3) {
                        var_s1[0x100] = 5;
                        state = 5;
                    } else if (D_801235B8->screenState == 9) {
                        var_s1[0x100] = 8;
                        state = 8;
                    }
                    break;
                case 8:
                    *(s32 *)(var_s3 + 0xBC) += 0x200000;
                    if (*(s32 *)(var_s3 + 0xBC) == 0xC00000) {
                        var_s1[0x100] = 9;
                    }
                    state = var_s1[0x100];
                    break;
                case 9:
                    break;
                }
            }

            if (state != 0) {
                if ((s32)state < 3) {
                    *(s16 *)(var_s2 + 0xEC) += 0x20;
                    *(s16 *)(var_s2 + 0xEC) &= 0xFFF;
                }
            }
            sp64 = statusHalfIndex;
            func_80097C18(sp44, *(s16 *)(var_s2 + 0xEC));
            func_80098590(sp44, sp40, &sp78);
            *(s32 *)(var_s7 + 0x50) = sp78.x;
            *(s32 *)(var_s7 + 0x54) = sp78.y;
            *(s32 *)(var_s7 + 0x58) = sp78.z;
            i++;
            var_s6[4] = var_s1[0x100];
            sp40++;
            sp44 = (s16 *)((u8 *)sp44 + 0x20);
            var_s6++;
            var_s1++;
            var_s2 += 2;
            var_s3 += 0xC;
            var_s7 += 0x20;
            statusHalfIndex = sp64 + 2;
        } while (i < (s32)D_80121B55);
    }

    if ((D_80121D80[0].pad6[2] == 4) || (((u8 *)actor)[0x100] == 9)) {
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

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80024168.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80024380.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80024A90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80024E98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_800257F0.s")

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

// func_80026C4C best match: 84.077% (nonmatchings/func_80026C4C-4923837976568703863/base_scope_pad.c)
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
    u8 *text;
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
            if ((D_8010AE64[0] != 3) || (D_800F42C7 & 7)) {
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

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80027AF8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80028194.s")

// func_80028354 best match: 95.650% (nonmatchings/func_80028354-3836525038718587862/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80028354.s")

#ifdef NON_MATCHING
extern int sprintf(u8 *, u8 *, ...);

void func_80028354(CourseSelectWidgetInitActor *actor) {
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
            if (i == D_80121B55) {
                alpha = 0xC0;
            }
            if (i == 0) {
                tile = 2;
            } else {
                tile = (i + 5) & 0xFF;
            }

            savedIndex = i;
            func_8000F8AC(cur->unk18, cur->unk20, func_80043040(D_80112130[0x21]), 0x22, 0x20, 0x20, 0,
                          alpha, tile);

            if (D_80121B55 == 2) {
                middleCount = 0xB;
            } else {
                middleCount = 3;
            }
            edgeOffset = 0x38;
            if (middleCount > 0) {
                do {
                    func_8000F8AC((s16)(cur->unk18 + edgeOffset), cur->unk20, func_80043040(D_80112130[0x21]),
                                  0x23, 0x20, 0x20, 0, alpha, tile);
                    j++;
                    edgeOffset += 0x10;
                } while (j != middleCount);
            }

            func_8000F8AC((s16)(cur->unk18 + edgeOffset), cur->unk20, func_80043040(D_80112130[0x21]), 0x24,
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
                func_8000F8AC(cur->unk18, (s16)(cur->unk20 + offset + 0x10), func_80043040(D_80112130[0x21]),
                              0x25, 0x20, 0x20, 0, alpha, tile);
                func_8000F8AC((s16)(cur->unk18 + edgeOffset), (s16)(cur->unk20 + offset + 0x10),
                              func_80043040(D_80112130[0x21]), 0x26, 0x20, 0x20, 0, alpha, tile);
                offset += 0x10;
            } while (offset < 0x40);

            func_8000F8AC(cur->unk18, (s16)(cur->unk20 + 0x50), func_80043040(D_80112130[0x21]), 0x27, 0x20,
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

            func_8000F8AC((s16)(cur->unk18 + edgeOffset - 8), (s16)(cur->unk20 + 0x50),
                          func_80043040(D_80112130[0x21]), 0x29, 0x20, 0x20, 0, alpha, tile);

            nextIndex = savedIndex + 1;
            sprintf(text, "%d", nextIndex);
            func_80013D0C((s16)(cur->unk18 + 0x34), (s16)(cur->unk20 + 2), text, 0, alpha);

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

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_800287EC.s")

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
