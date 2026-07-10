#include "common.h"
#include "effect_task_scheduler.h"
#include "course_select_menu.h"
#include "input_task_scheduler.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 fade;
    /* 0x1C */ s32 timer;
    /* 0x20 */ s32 screenState;
} CourseSelectMenuState;

typedef struct {
    union {
        struct {
            /* 0x00 */ u8 unk0[4];
            /* 0x04 */ u8 playerOneCourseDecided;
            /* 0x05 */ u8 unk5[3];
            /* 0x08 */ u8 playerTwoCourseDecided;
            /* 0x09 */ u8 unk9[7];
            /* 0x10 */ u8 unk10[4];
        };
        struct {
            /* 0x00 */ u8 unk0Array[4];
            /* 0x04 */ u8 unk4Array[4];
            /* 0x08 */ u8 unk8Array[4];
            /* 0x0C */ u8 unkCArray[4];
            /* 0x10 */ u8 unk10Array[4];
        };
    };
    /* 0x14 */ s16 unk14[4];
    /* 0x1C */ s16 unk1C[4];
    /* 0x24 */ u8 unk24[4];
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 transitionState;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F[9];
} CourseSelectStatus;

typedef struct {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ u8 unk8;
} CourseSelectSelection;

typedef struct ObjectA3E0 {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad28[4];
    /* 0x2C */ void (*unk2C)(void);
    /* 0x30 */ u8 pad30[0x80];
} ObjectA3E0;

extern u8 D_14B450[];
extern u8 D_1502A0[];
extern u8 D_1E74E0[];
extern u8 D_1EC0F0[];
extern u8 D_1EF530[];
extern u8 D_1F1A90[];
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_59AAA0[];
extern u8 D_59DFE0[];
extern u8 D_59E7F0[];
extern u8 D_5A1ED0[];
extern u8 D_5C5320[];
extern u8 D_5CBA80[];
extern u8 D_5CCD40[];
extern u8 D_5D4280[];
extern u8 D_5DAF30[];
extern ObjectA3E0 *D_800EC9C4;
extern u16 D_800B34E0[];
extern s16 D_800DEF14;
extern s16 D_800EC9D0;
extern s8 D_800DEED4;
extern s8 D_800EC9C0;
extern s8 D_800EC9C1;
extern u8 D_800EC9E6;
extern u8 D_800EC9F0[];
extern s8 D_800EC9F1;
extern s8 D_800EC9F9;
extern s8 D_800ECA2F[];
extern s32 D_8010ADDC;
extern s8 D_8010AE64;
extern u8 D_8010AEA0[];
extern s8 D_8010AEA4;
extern s8 D_8010AEA8;
extern u8 D_8010AEAC[];
extern s8 D_8010AEB0;
extern u8 D_8010AEF8[];
extern u8 D_8010AEFB[];
extern u8 D_8010AF08[];
extern s16 D_8010AED0;
extern s16 D_8010ADF0;
extern s8 D_8010ADF8;
extern u8 D_8010AECC;
extern CourseSelectStatus D_8010AF18;
extern u8 D_8010AF19;
extern u8 D_8010AF1A;
extern u8 D_8010AF1B;
extern u8 D_8010AF1C;
extern u8 D_8010AF1D;
extern u8 D_8010AF1E;
extern u8 D_8010AF1F;
extern u8 D_8010AF20;
extern u8 D_8010AF21;
extern u8 D_8010AF22;
extern u8 D_8010AF23;
extern u8 D_8010AF24;
extern u8 D_8010AF25;
extern u8 D_8010AF26;
extern u8 D_8010AF27;
extern u8 D_8010AF28;
extern u8 D_8010AF29;
extern u8 D_8010AF2A;
extern u8 D_8010AF2B;
extern s16 D_8010AF2C;
extern s16 D_8010AF2E;
extern s16 D_8010AF30;
extern s16 D_8010AF32;
extern s16 D_8010AF34;
extern s16 D_8010AF36;
extern s16 D_8010AF38;
extern s16 D_8010AF3A;
extern u8 D_8010AF3C;
extern u8 D_8010AF3D;
extern u8 D_8010AF3E;
extern u8 D_8010AF3F;
extern s8 D_8010AF70;
extern s8 D_8010AF71;
extern s8 D_8010AF72;
extern s8 D_8010AF73;
extern s16 D_8010AF76;
extern s8 D_8010AF74;
extern s16 D_80112130[];
extern ObjectA3E0 D_801121E0[];
extern ObjectA3E0 D_80112340;
extern u8 D_80121B55;
extern u8 D_80121D86;
extern u8 D_80121D88;
extern CourseSelectSelection D_80121D80;
extern CourseSelectMenuState *D_801235B8;
extern s32 D_801235B4;
extern u8 D_80123750;
extern u8 D_80123751;
extern void func_80009C48(void);
extern void func_8000B220(void);
extern void func_8000B7B8(void);
extern void func_8000C010(void);
extern void func_8001710C(EffectTask *);
extern void func_800257F0(EffectTask *);
extern void func_80028194(EffectTask *);
extern void func_8002F854();
extern void func_8002FEF8(void);
extern s32 func_80013F88(s32, s32, s32);
extern s16 func_80042D58(s32);
extern s32 func_80043040(s16);
extern void func_800437F0(void *, void *, s32);
extern void func_80045914(void);
extern void func_8006D5CC(void);
extern void func_800704F0(void);
extern void func_80070614(s32);
extern void func_8007066C(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_80070E90(s32);
extern void func_800720E4(s32);
extern void func_80072138(s32, s32);
extern void func_80099C44(void *, void *, s32);
extern void n_alSeqpDelete(void);

// func_800097E0 best match: 98.706%
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_800097E0.s")

#ifdef NON_MATCHING
void func_800097E0(void) {
    u32 size;
    s32 i;
    s32 zero;
    s32 mask;
    s32 shiftedMask;
    u8 *unlockedCourse;
    u8 *otherCourse;
    ObjectA3E0 *obj;
    void (*nextCallback)(void);
    s32 savedUnlocks;

    func_800720E4(3);
    func_8006D5CC();
    func_800704F0();
    func_8007066C(0, 0xE8, 0x78, 0x90, 0xD0, 0xA0, 0xF0, 0.6666666865f);

    obj = D_801121E0;
    unlockedCourse = D_8010AEA0;
    otherCourse = D_8010AEAC;
    do {
        otherCourse++;
        obj++;
        unlockedCourse++;
        obj[-1].unk2C = n_alSeqpDelete;
        obj[-1].unk24 = 0xA40000;
        if ((D_5C5320 && D_5C5320) && D_5C5320) {
        }
        unlockedCourse[-1] = 0;
        otherCourse[-1] = 0;
    } while (otherCourse < D_8010AEAC + 4);

    D_800DEED4 = 0;
    D_801235B8->fade = 0xFF;
    func_800437F0(D_5CBA80, D_5CCD40, 0x21);
    func_800437F0(D_593D10, D_598A70, 0x22);
    func_800437F0(D_598A70, D_59AAA0, 0x23);
    func_800437F0(D_59AAA0, D_59DFE0, 0x24);
    func_800437F0(D_5A1ED0, D_5C5320, 0x25);
    func_800437F0(D_59DFE0, D_59E7F0, 0x26);
    func_800437F0(D_5D4280, D_5DAF30, 0x27);

    size = D_1502A0 - D_14B450;
    D_80112130[0xC] = func_80042D58(size);
    func_80099C44(D_14B450, (void *)func_80043040(D_80112130[0xC]), size);
    func_800437F0(D_1EF530, D_1F1A90, 0xD);
    func_800437F0(D_1E74E0, D_1EC0F0, 0x1C);
    nextCallback = func_80009C48;
    func_80070EC0(0);
    func_80071408(func_8001710C, 0, 0x5E);

    D_800EC9C1 = 0;
    D_800EC9C0 = 0;
    D_8010AF74 = 0;
    D_80121D80.unk8 = 0;
    D_80121D80.unk6 = 0;
    D_8010AE64 = 0;
    D_8010ADDC = 0;
    D_801235B8->timer = 0;
    D_8010ADF8 = 0;
    D_8010AF70 = 0;
    D_8010AF71 = 0;
    D_8010AF72 = 0;
    D_8010AF73 = 0;
    zero = 0;
    D_8010AED0 = zero;
    D_8010AEA8 = zero;
    D_8010AEA4 = zero;
    D_8010AEB0 = 0;
    D_800EC9D0 = 0;
    D_8010AECC = 0;
    D_8010ADF0 = 0;

    D_800DEF14 = D_801235B8->fade;
    mask = 1;
    i = 0;
    do {
        savedUnlocks = D_800EC9F0[0x78D7];
        i++;
        if (savedUnlocks & mask) {
            shiftedMask = mask;
            if (1) {
                shiftedMask = shiftedMask << 3;
                if (!(savedUnlocks & shiftedMask)) {
                    D_8010AF71 = 1;
                    D_8010AF70 = 1;
                    D_800EC9F0[0x78D7] = savedUnlocks | shiftedMask;
                }
            }
        }
        mask <<= 1;
    } while (i < 3);

    if (D_800EC9E6 == 2) {
        D_800EC9E6 = 0;
    }

    func_8009956C(nextCallback, 0);
    func_8007105C();

    D_8010AF18.unk0Array[zero] = zero;
    D_8010AF1C = zero;
    D_8010AF20 = 0;
    D_8010AF24 = 0;
    D_8010AF28 = 0;
    D_8010AF2C = 0;
    D_8010AF34 = zero;
    D_8010AF3C = 0;
    D_8010AF19 = 0;
    D_8010AF1D = 0;
    D_8010AF21 = zero;
    D_8010AF25 = 0;
    D_8010AF29 = 0;
    D_8010AF2E = zero;
    D_8010AF36 = zero;
    D_8010AF3D = 0;
    D_8010AF1A = 0;
    D_8010AF1E = 0;
    D_8010AF22 = 0;
    D_8010AF26 = zero;
    i = 0;
    D_8010AF2A = i;
    D_8010AF30 = i;
    D_8010AF38 = zero;
    D_8010AF3E = zero;
    D_8010AF1B = i;
    D_8010AF1F = i;
    D_8010AF23 = i;
    D_8010AF27 = i;
    D_8010AF2B = i;
    D_8010AF32 = i;
    D_8010AF3A = i;
    D_8010AF3F = i;
    D_8010AF18.transitionState = i;
    D_8010AF18.unk28 = i;
    D_8010AF18.unk2A = i;
    D_8010AF18.unk2C = i;
    D_8010AF18.unk2E = i;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_80009C48.s")

// func_8000A048 best match: 86.471%
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_8000A048.s")

#ifdef NON_MATCHING
void func_8000A048(void) {
    u8 courseFlags;
    s32 var_a0;
    s32 var_a2;
    s32 var_a2_2;
    u8 *var_v1;
    u8 *var_v1_2;
    s8 var_v0;
    s8 var_v0_2;
    s8 var_v0_3;
    u8 var_v0_4;

    D_80121D88 = 0;
    D_8010AEA0[0] = 0;
    D_800EC9C1 = 0;
    D_8010ADF0 = 0;
    func_80071408(func_800257F0, 0, 0x63);
    D_8010ADE8 = func_80071408(func_80028194, 0, 0x61);
    courseFlags = D_800F42C7;
    if (courseFlags & 7) {
        D_8010AEA0[0] = 1;
    }

    var_v1 = D_8010AEF8;
    var_v0 = 0;
    do {
        *var_v1 = var_v0;
        var_v0 += 1;
        var_v1 += 1;
    } while (var_v0 < 3);

    if (D_8010AEA0[0] == 1) {
        var_a2 = 1;
        var_v0_2 = 9;
loop_6:
        if (courseFlags & var_a2) {
            D_8010AEFB[0] = var_v0_2;
        } else {
            var_v0_2 += 1;
            var_a2 *= 2;
            if (var_v0_2 < 0xC) {
                goto loop_6;
            }
        }
        var_v0_3 = 9;
        var_a0 = 1;
        var_v1_2 = D_8010AF08;
        do {
            *var_v1_2 = 0;
            if (courseFlags & var_a0) {
                *var_v1_2 = var_v0_3;
                var_v1_2 += 1;
            }
            var_v0_3 += 1;
            var_a0 *= 2;
        } while (var_v0_3 != 0xC);
    } else {
        D_8010AEFB[0] = 0;
    }

    if (((s32) D_80121D80.unk6 >= 9) && ((s32) D_80121D80.unk6 < 0xC)) {
        var_v0_4 = D_8010AF46;
        var_a2_2 = 3;
    } else {
        var_v0_4 = D_8010AF18.unk2E;
        var_a2_2 = 0;
        if (var_v0_4 == 1) {
            D_8010AF18.unk2E = 0;
            var_v0_4 = 0;
        } else {
            var_a2_2 = (s32) D_80121D80.unk6 % 3;
        }
    }

    D_8010AE64 = var_a2_2;
    if (var_v0_4 == 1) {
        var_a2_2--;
    }

    D_80121D80.unk6 = D_8010AEF8[var_a2_2];
    func_8009956C(func_8000A214, 0);
    func_8007105C();
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_8000A214.s")

// func_8000AFE8 best match: 92.901%
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_8000AFE8.s")

#ifdef NON_MATCHING
void func_8000AFE8(void) {
    CourseSelectMenuState *volatile *statePtr;
    ObjectA3E0 **currentPtr;
    ObjectA3E0 *var_s1;
    u8 *countPtr;
    s32 var_s0;
    CourseSelectMenuState *state;
    s32 temp_v0;
    s32 temp_v1;

    statePtr = &D_801235B8;
    state = *statePtr;
    temp_v0 = state->timer;
    if (temp_v0 < 2) {
        temp_v1 = D_80123778;
        if ((temp_v1 & 0x10800) && (temp_v0 != 0)) {
            state->timer = temp_v0 - 1;
            func_80072138(0x19, 0x32);
            temp_v1 = D_80123778;
        } else if ((temp_v1 & 0x20400) && (temp_v0 != 1)) {
            state->timer = temp_v0 + 1;
            func_80072138(0x19, 0x32);
            temp_v1 = D_80123778;
        }

        if (temp_v1 & 0x8000) {
            D_801235B4 = 1;
            if ((*statePtr)->timer == 1) {
                func_80072138(0x18, 0x32);
                if (D_8010AECC == 0) {
                    D_8010AF1C = 1;
                } else {
                    D_8010AF20 = 1;
                }
                func_8009956C(func_8000A214, 0);
            } else {
                func_80072138(0x45, 0x32);
                (*statePtr)->timer += 2;
            }
        } else if (temp_v1 & 0x4000) {
            D_801235B4 = 1;
            func_80072138(0x18, 0x32);
            if (D_8010AECC == 0) {
                D_8010AF1C = 1;
            } else {
                D_8010AF20 = 1;
            }
            func_8009956C(func_8000A214, 0);
        }
    } else if (temp_v0 >= 4) {
        (*statePtr)->timer = 0;
        D_80121D88 = 9;
        func_8009956C(func_8000A214, 0);
    }

    countPtr = &D_80121B55;
    var_s0 = 0;
    if (*countPtr > 0) {
        currentPtr = &D_800EC9C4;
        var_s1 = D_801121E0;
        do {
            D_800EC9C4 = var_s1;
            var_s1->unk2C();
            var_s0 += 1;
            var_s1 += 1;
        } while (var_s0 < *countPtr);
    }
    func_8007105C();
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_8000B220.s")

void func_8000B690(void) {
    ObjectA3E0 *var_s1;
    s32 var_s0;
    s8 temp_v0;

    if (D_801235B8->screenState == 2) {
        func_80071408(func_8002F854, 0, 0x63);
        temp_v0 = D_800ECA2F[D_80121D86];
        D_8010AF72 = temp_v0 % 7;
        D_8010AF73 = temp_v0 / 7;
        D_8010ADF0 = 0;
        D_8010ADF8 = 0;
        func_8009956C(func_8000B7B8, 0);
    }

 do { var_s0 = 0; if (D_80121B55 > 0) { var_s1 = D_801121E0; do { ; (D_800EC9C4 = var_s1)->unk2C(); var_s0 += 1; var_s1 += 1; } while (var_s0 < D_80121B55); } } while (0);
    func_8007105C();
}

// func_8000B7B8 best match: 68.617% (nonmatchings/func_8000B7B8-2127290767680699791/base_3.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_8000B7B8.s")

#ifdef NON_MATCHING
void func_8000B7B8(void) {
    s32 sp24;
    ObjectA3E0 *var_s0;
    s32 temp_t2;
    s32 temp_t3_2;
    s32 temp_t7;
    s32 temp_a2;
    s32 temp_v0;
    s32 var_v0;
    s32 temp_t3;
    s32 temp_t8;
    u16 var_v1;
    u8 temp_a0;
    u8 temp_t1;
    u8 var_a1;

    if ((s32) D_8010AF41 >= 2) {
        var_a1 = (u8) D_8010AF72;
        temp_v0 = D_80123758 & 0x10800;
        temp_t1 = var_a1;
        if ((temp_v0 == 0) && !(D_80123758 & 0x20400)) {
            D_8010ADF0 = 0;
        }
        temp_a2 = D_80123778;
        if ((temp_a2 & 0x10800) ||
            ((temp_v0 != 0) && ((s32) (u16) D_8010ADF0 >= 0xB) && (((s32) (u16) D_8010ADF0 % 3) == 0))) {
            var_v1 = (u16) D_8010ADF0;
            temp_t3 = var_a1 - 1;
            temp_t2 = var_v1 + 1;
            if (var_v1 == 0) {
                D_8010ADF0 = temp_t2;
                var_v1 = temp_t2 & 0xFFFF;
            }
            if ((s32) var_a1 > 0) {
                D_8010AF72 = temp_t3;
                var_a1 = temp_t3 & 0xFF;
            }
        } else {
            var_v1 = (u16) D_8010ADF0;
            if (!(temp_a2 & 0x20400)) {
                if ((D_80123758 & 0x20400) && ((s32) var_v1 >= 0xB)) {
                    if (((s32) var_v1 % 3) == 0) {
                        goto block_16;
                    }
                    goto block_21;
                }
block_21:
                if (temp_a2 & 0x80200) {
                    D_8010ADF8 = 1;
                }
            } else {
block_16:
                if (var_v1 == 0) {
                    temp_t7 = var_v1 + 1;
                    D_8010ADF0 = temp_t7;
                    var_v1 = temp_t7 & 0xFFFF;
                }
                temp_t8 = var_a1 + 1;
                if ((s32) var_a1 < 9) {
                    D_8010AF72 = temp_t8;
                    var_a1 = temp_t8 & 0xFF;
                }
            }
        }
        if (var_v1 != 0) {
            temp_t3_2 = var_v1 + 1;
            D_8010ADF0 = temp_t3_2;
            if ((temp_t3_2 & 0xFFFF) == 0xFFFF) {
                D_8010ADF0 = 0xC;
            }
        }
        if (var_a1 != temp_t1) {
            func_80072138(0x19, 0x32);
            temp_a2 = D_80123778;
        }
        if (temp_a2 & 0x4000) {
            D_8010ADF8 = 2;
        }
        if ((temp_a2 & 0x8000) || (temp_a2 & 0x1000) || ((u8) D_8010ADF8 != 0)) {
            temp_a0 = (u8) D_8010AF72;
            if (((s32) temp_a0 >= 7) || ((u8) D_8010ADF8 != 0)) {
                func_80072138(0x18, 0x32);
            }
            if (((temp_a0 == 7) || ((u8) D_8010ADF8 == 1)) && ((u8) D_8010ADF8 != 2)) {
                D_8010AF41 = 3;
                if ((u8) D_8010ADF8 == 1) {
                    D_8010AF74 = 1;
                }
                D_8010ADF8 = 0;
                func_8009956C(func_8000BAFC, 0);
            } else if ((temp_a0 == 8) || ((u8) D_8010ADF8 == 2)) {
                func_8009956C(func_8000BBB4, 0);
                D_8010ADF8 = 0;
                D_801235B8->screenState = 3;
            } else if (temp_a0 == 9) {
                func_8009956C(func_8000BCA0, 0);
                D_801235B8->screenState = 9;
            } else {
                func_8009956C(func_8000BEC0, 0);
            }
        }
    }
    var_v0 = 0;
    if ((s32) D_80121B55 > 0) {
        var_s0 = D_801121E0;
        do {
            D_800EC9C4 = var_s0;
            sp24 = var_v0;
            var_s0->unk2C();
            var_v0 = sp24 + 1;
            var_s0 += 1;
        } while (var_v0 < (s32) D_80121B55);
    }
    func_8007105C();
}
#endif

void func_8000BAFC(void) {
    ObjectA3E0 *var_s1;
    s32 var_s0;

    if (D_8010AF18.transitionState == 2) {
        D_8010ADF0 = 0;
        D_8010AF74 = 0;
        func_8009956C(func_8000B7B8, 0);
    }

 do { var_s0 = 0; if (D_80121B55 > 0) { var_s1 = D_801121E0; do { ; (D_800EC9C4 = var_s1)->unk2C(); var_s0 += 1; var_s1 += 1; } while (var_s0 < D_80121B55); } } while (0);
    func_8007105C();
}

void func_8000BBB4(void) {
    ObjectA3E0 *var_s1;
    s32 var_s0;

    if (D_801235B8->screenState == 5) {
        if (D_8010AECC == 0) {
            D_8010AF18.playerOneCourseDecided = 1;
        } else {
            D_8010AF18.playerTwoCourseDecided = 1;
        }
        D_8010AF18.transitionState = 0;
        D_801235B8->screenState = 0;
        D_8010ADF0 = 0;
        func_8009956C(func_8000B220, 0);
    }

 do { var_s0 = 0; if (D_80121B55 > 0) { var_s1 = D_801121E0; do { ; (D_800EC9C4 = var_s1)->unk2C(); var_s0 += 1; var_s1 += 1; } while (var_s0 < D_80121B55); } } while (0);
    func_8007105C();
}

void func_8000BCA0(void) {
    ObjectA3E0 *var_s1;
    s32 var_s0;
    s32 i;

    if (D_801235B8->screenState == 0xB) {
        func_8009956C(func_80009C48, 0);
        D_801235B8->fade = 1;
        D_801235B8->timer = 0;
        D_801235B8->screenState = 0;
        D_80121D80.unk8 = 0;
        D_80121D80.unk6 = D_80121D80.unk6 % 3;
        D_800EC9D0 = 0;
        D_8010AECC = 0;
        D_8010ADF0 = 0;

        for (i = 0; i < 4; i++) {
            D_8010AF18.unk0Array[i] = 0;
            D_8010AF18.unk4Array[i] = 0;
            D_8010AF18.unk8Array[i] = 0;
            D_8010AF18.unkCArray[i] = 0;
            D_8010AF18.unk10Array[i] = 0;
            D_8010AF18.unk14[i] = 0;
            D_8010AF18.unk1C[i] = 0;
            D_8010AF18.unk24[i] = 0;
        }

        D_8010AF18.unk28 = 0;
        D_8010AF18.unk2A = 0;
        D_8010AF18.transitionState = 0;
        D_8010AF18.unk2C = 0;
        D_8010AF18.unk2E = 0;
    }

 do { var_s0 = 0; if (D_80121B55 > 0) { var_s1 = D_801121E0; do { ; (D_800EC9C4 = var_s1)->unk2C(); var_s0 += 1; var_s1 += 1; } while (var_s0 < D_80121B55); } } while (0);
    func_8007105C();
}

void func_8000BEC0(void) {
    ObjectA3E0 *var_s0;
    s8 *temp;

    D_8010AF76 = 0x78;
    func_8007066C(1, 0xE8, 0x78, 0x90, D_8010AF76, 0xA0, 0xF0, 0.6666666865f);
    func_80070E90(1);
    temp = (s8 *) &D_800EC9F0[D_80121D86];
    D_8010AED0 = temp[0x3F] + 1;
    temp[0x3F] = D_800B34E0[(u8) D_8010AF73 * 7 + (u8) D_8010AF72];
    D_8010AF18.transitionState = 6;
    func_80071408(&func_8002FEF8, 0, 0x64);
    func_8009956C(func_8000C010, 0); var_s0 = D_801121E0; do { D_800EC9C4 = var_s0; var_s0->unk2C();
        var_s0 += 1;
    } while (var_s0 != &D_80112340);
    func_8007105C();
    func_80072138(0x17, 0x32);
}

void func_8000C010(void) {
    ObjectA3E0 *var_s0;

    D_8010AF76 -= 4;
    if (D_8010AF76 < 0) {
        D_8010AF76 = (D_8010AF76 < 0) * 0;
    }
    func_8007066C(1, 0xE8, 0x78, 0x90, D_8010AF76, 0xA0, 0xF0, 0.6666666865f);
    if (D_8010AF76 == 0) {
        func_80070614(1);
        func_8009956C(func_8000B7B8, 0);
        D_8010AF18.transitionState = 2;
 D_8010AED0 = 0; } var_s0 = D_801121E0; do { (D_800EC9C4 = var_s0)->unk2C(); var_s0 += 1; } while (var_s0 != (&D_80112340)); func_8007105C();
}

void func_8000C114(void) {
    s8 *ptr;
    s32 count;

    if (D_801235B8->fade != 0xFF) {
        D_801235B8->fade = func_80013F88((s16) D_801235B8->fade, 0x24, 1);
        if (D_801235B8->fade == 0xFF) {
            D_80123751 = 1;
        } else {
            func_8007105C();
        }
    } else if (D_80123750 == 2) {
        func_80045914();
        D_80123751 = 0;
        do { D_800DEED4 = 0; D_801235B4 = 0; func_80099658(2); func_8009954C(4); count = 0; if (D_800ECA2F[0] == (-1)) { count = 1; } ptr = &D_800EC9F1; loop: if (ptr[0x3F] == (-1)) { count++; } if (ptr[0x40] == (-1)) { count++; } if (ptr[0x41] == (-1)) { count++; } if (ptr[0x42] == (-1)) { count++; } } while (0);
        ptr += 4;
        if (ptr != (&D_800EC9F9)) {
            goto loop;
        }
        if (count == 0) {
            D_800EC9F0[0x78D7] |= 4;
        }
    }
}
