#include "common.h"
#include "menu_rendering.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    union {
        struct {
            /* 0x18 */ s16 x;
            /* 0x1A */ s16 y;
            /* 0x1C */ s16 spriteIndex;
            /* 0x1E */ s8 state;
            /* 0x1F */ s8 timer;
            /* 0x20 */ s8 subState;
        };
        struct {
            /* 0x18 */ u8 pad18[0x10];
            /* 0x28 */ s16 transitionOffset;
            /* 0x2A */ u8 transitionState;
        };
        /* 0x18 */ s16 coordinates[0x22];
        /* 0x18 */ s16 coordinateRows[4][3];
    };
    union {
        /* 0x5C */ u8 itemCount;
        /* 0x5C */ u8 itemCounts[4];
    };
} CourseSelectWidgetActor;

typedef struct {
    u8 pad0[0x20];
    /* 0x20 */ s32 screenState;
} CourseSelectState;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} CourseSelectTempVec3i;

extern void func_80023A68();
extern void func_800483FC(void *, void (*)(CourseSelectWidgetActor *), CourseSelectWidgetActor *);
extern void func_800716E4(CourseSelectWidgetActor *);
extern void func_80071824(void *task, void (*callback)());
extern void func_80097C18(s16 *mtx, s16 rotY);
extern void func_8009853C(s16 *mtx, s16 arg1, s16 arg2);
extern void func_80098590(s16 *mtx, CourseSelectTempVec3i *source, CourseSelectTempVec3i *dest);
extern void func_800260E8(CourseSelectWidgetActor *);
extern void func_800263D8(CourseSelectWidgetActor *);
extern void func_800271CC(CourseSelectWidgetActor *);
extern void func_80027498(CourseSelectWidgetActor *);
extern void func_800275E0(CourseSelectWidgetActor *);
extern void func_80027AF8(CourseSelectWidgetActor *);
extern void func_80028FF0(CourseSelectWidgetActor *);
extern void func_800291F0(s32);
extern s32 func_80043040(s16);
extern u8 D_800EC9E6;
extern s32 D_8010ADE8;
extern s16 D_800B70C0[][4];
extern s16 D_80112130[];
extern s16 D_8011217A;
extern u8 D_80121B55;
extern u8 D_80121D88;
extern CourseSelectState *D_801235B8;
extern s32 D_80124868;

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80023880.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80023A68.s")

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

// func_80024968 best match: 99.730%
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80024968.s")

#ifdef NON_MATCHING
extern u8 D_8010AF1C;

void func_80024968(void *arg0) {
    CourseSelectTempVec3i sp60;
    void *actor;
    u8 *var_s1;
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
        var_s3 += 1; *(var_s3 + 7) = one;
    } while (var_s3 != &D_8010AF1C);

    *(s32 *) ((u8 *) actor + 0x30) = 0;
    *(s32 *) ((u8 *) actor + 0x34) = 0;
    *(s32 *) ((u8 *) actor + 0x38) = 0;
    func_8009853C((s16 *) ((u8 *) actor + 0x1C), 0x400, 0x280);
    func_80071824(actor, func_80024380);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80024A90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80024E98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_800257F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80025AA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80025BE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80025E6C.s")

void func_80025FA8(CourseSelectWidgetActor *arg0) {
    func_8000F030(arg0->x, arg0->y, func_80043040(D_8011217A), 3, 0x20, 0x20, 0, 0);
    func_8000F030((s16) (arg0->x + 0x40), arg0->y, func_80043040(D_8011217A), 4, 0x20, 0x20, 0, 0);
    func_8000F030(arg0->x, (s16) (arg0->y + 0x40), func_80043040(D_8011217A), 5, 0x20, 0x20, 0, 0);
    func_8000F030((s16) (arg0->x + 0x40), (s16) (arg0->y + 0x40), func_80043040(D_8011217A), 6, 0x20, 0x20, 0, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_800260E8.s")

void func_800263A4(CourseSelectWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = 8;
    func_80071824(arg0, func_800260E8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_800263D8.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80026C4C.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_800275E0.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80028354.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_800287EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80028B0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80028C68.s")

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
