#include "common.h"

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
        /* 0x18 */ s16 coordinates[0x22];
    };
    /* 0x5C */ u8 itemCount;
} CourseSelectWidgetActor;

typedef struct {
    u8 pad0[0x20];
    /* 0x20 */ s32 screenState;
} CourseSelectState;

extern void func_800483FC(void *, void (*)(CourseSelectWidgetActor *), CourseSelectWidgetActor *);
extern void func_800716E4(CourseSelectWidgetActor *);
extern void func_80071824(CourseSelectWidgetActor *, void (*)(CourseSelectWidgetActor *));
extern void func_800260E8(CourseSelectWidgetActor *);
extern void func_800271CC(CourseSelectWidgetActor *);
extern void func_80027408(CourseSelectWidgetActor *);
extern void func_80027498(CourseSelectWidgetActor *);
extern void func_800275E0(CourseSelectWidgetActor *);
extern void func_80027AF8(CourseSelectWidgetActor *);
extern void func_80028FF0(CourseSelectWidgetActor *);
extern void func_800291F0(s32);
extern u8 D_80121B55;
extern u8 D_80121D88;
extern CourseSelectState *D_801235B8;
extern s32 D_80124868;

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80023880.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80023A68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80024050.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80024168.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80024380.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80024968.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80024A90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80024E98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_800257F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80025AA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80025BE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80025E6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80025FA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_800260E8.s")

void func_800263A4(CourseSelectWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = 8;
    func_80071824(arg0, func_800260E8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_800263D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80026A54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80026B88.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80027408.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_ui/func_80027A08.s")

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
