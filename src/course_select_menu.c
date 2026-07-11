#include "common.h"
#include "asset_manager.h"
#include "race_camera.h"
#include "effect_task_scheduler.h"
#include "course_select_menu.h"
#include "input_task_scheduler.h"
#include "menu_rendering.h"
#include "shop_menu_ui.h"
#include "title_menu.h"
#include "viewport_manager.h"

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
extern s16 gMenuFadeAlpha;
extern s16 D_800EC9D0;
extern s8 gFramebufferSwapDelay;
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
extern CourseSelectStatus gCourseSelectStatus;
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
extern u8 D_8010AF46;
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
extern s8 D_80121D87;
extern u8 D_80121D88;
extern CourseSelectSelection D_80121D80;
extern CourseSelectMenuState *D_801235B8;
extern s32 D_801235B4;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern s32 gPlayerInputHeld;
extern s32 gPlayerInputPressed;
extern void func_80009C48(void);
extern void func_8000B220(void);
extern void func_8000B7B8(void);
extern void func_8000C010(void);
extern void func_800257F0(EffectTask *);
extern void func_80028194(EffectTask *);
extern s16 func_80042D58(s32);
extern s32 func_80043040(s16);
extern void releaseMenuAssetHandles(void);
extern void func_800720E4(s32);
extern void func_80072114(s32);
extern void enqueueSoundEffect(s32, s32);
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

    gFramebufferSwapDelay = 0;
    D_801235B8->fade = 0xFF;
    loadCompressedRomAsset(D_5CBA80, D_5CCD40, 0x21);
    loadCompressedRomAsset(D_593D10, D_598A70, 0x22);
    loadCompressedRomAsset(D_598A70, D_59AAA0, 0x23);
    loadCompressedRomAsset(D_59AAA0, D_59DFE0, 0x24);
    loadCompressedRomAsset(D_5A1ED0, D_5C5320, 0x25);
    loadCompressedRomAsset(D_59DFE0, D_59E7F0, 0x26);
    loadCompressedRomAsset(D_5D4280, D_5DAF30, 0x27);

    size = D_1502A0 - D_14B450;
    D_80112130[0xC] = func_80042D58(size);
    func_80099C44(D_14B450, (void *)func_80043040(D_80112130[0xC]), size);
    loadCompressedRomAsset(D_1EF530, D_1F1A90, 0xD);
    loadCompressedRomAsset(D_1E74E0, D_1EC0F0, 0x1C);
    nextCallback = func_80009C48;
    func_80070EC0(0);
    createEffectTask((void (*)(EffectTask *))func_8001710C, 0, 0x5E);

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

    gMenuFadeAlpha = D_801235B8->fade;
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

    setCurrentInputTaskCallback(nextCallback, 0);
    updateEffectTasks();

    gCourseSelectStatus.unk0Array[zero] = zero;
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
    gCourseSelectStatus.transitionState = i;
    gCourseSelectStatus.unk28 = i;
    gCourseSelectStatus.unk2A = i;
    gCourseSelectStatus.unk2C = i;
    gCourseSelectStatus.unk2E = i;
}
#endif

// func_80009C48 best match: 76.508% (nonmatchings/func_80009C48-7273315160691878794/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_80009C48.s")

#ifdef NON_MATCHING
void func_80009C48(void) {
    s32 input;
    s32 pressed;
    s32 leftPressed;
    s32 held;
    u16 repeat;
    u8 selection;
    u8 oldSelection;
    u8 transition;

    if (D_801235B8->fade != 0) {
        D_801235B8->fade = stepMenuFadeAlpha((s16) D_801235B8->fade, 0x24, 0);
        if (D_801235B8->fade == 0) {
            createEffectTask(func_8002C800, 0, 0x63);
            if (D_8010AF70 == 0) {
                enqueueSoundEffect(0x44, 0x32);
            }
        }
    } else {
        transition = D_800EC9C1;
        if (transition == 0) {
            if ((D_80121D88 == 0) && (gCourseSelectStatus.unk28 == 1)) {
                selection = D_800EC9E6;
                oldSelection = selection;
                pressed = gPlayerInputHeld;
                leftPressed = pressed & 0x10800;
                if ((leftPressed == 0) && !(pressed & 0x20400)) {
                    D_8010ADF0 = 0;
                }
                held = gPlayerInputPressed;
                if ((held & 0x10800) || ((leftPressed != 0) && ((repeat = D_8010ADF0) >= 0xB) && ((repeat % 3) == 0))) {
                    repeat = D_8010ADF0;
                    if (repeat == 0) {
                        repeat += 1;
                        D_8010ADF0 = repeat;
                    }
                    if ((s32) selection > 0) {
                        selection -= 1;
                        D_800EC9E6 = selection;
                    }
                } else {
                    repeat = D_8010ADF0;
                    if ((held & 0x20400) || ((pressed & 0x20400) && ((s32) repeat >= 0xB) && ((repeat % 3) == 0))) {
                        if (repeat == 0) {
                            repeat += 1;
                            D_8010ADF0 = repeat;
                        }
                        if ((s32) selection < 2) {
                            selection += 1;
                            D_800EC9E6 = selection;
                        }
                    }
                }
                if (repeat != 0) {
                    repeat += 1;
                    D_8010ADF0 = repeat;
                    if (repeat == 0xFFFF) {
                        D_8010ADF0 = 0xC;
                    }
                }
                if (selection != oldSelection) {
                    enqueueSoundEffect(0x19, 0x32);
                    if (D_8010AF70 == 0) {
                        D_8010AF70 = 1;
                    }
                    if (D_8010AF71 == 1) {
                        D_8010AF71 = 0;
                    }
                }
                input = gPlayerInputPressed;
                if (((input & 0x1000) || (input & 0x8000)) && (D_801235B4 == 2)) {
                    D_800EC9C1 = 1;
                    gCourseSelectStatus.unk28 = 2;
                    gCourseSelectStatus.unk2A = 0x100;
                    if ((s32) D_800EC9E6 < 2) {
                        D_8010ADF8 = 0;
                        enqueueSoundEffect(0x18, 0x32);
                    } else {
                        D_8010ADF8 = 1;
                        enqueueSoundEffect(0x46, 0x32);
                    }
                    transition = D_800EC9C1;
                } else {
                    transition = D_800EC9C1;
                    if ((input & 0x4000) && (D_801235B4 == 2) && (transition == 0)) {
                        D_800EC9C1 = 1;
                        gCourseSelectStatus.unk28 = 2;
                        gCourseSelectStatus.unk2A = 0x100;
                        D_8010ADF8 = 1;
                        enqueueSoundEffect(0x46, 0x32);
                        transition = D_800EC9C1;
                    }
                }
            }
        } else {
            transition += 1;
            D_800EC9C1 = transition;
        }
        if (transition == 8) {
            D_80121D88 = 1;
            D_800EC9C1 = transition + 1;
        }
        if (D_80121D88 == 2) {
            D_800EC9C1 = 0;
            if (D_8010ADF8 == 0) {
                setCurrentInputTaskCallback(func_8000A048, 0);
            } else {
                setCurrentInputTaskCallback(func_8000C114, 0);
                func_80072114(8);
                D_8010ADF8 = 0;
                D_8010AF46 = 0;
            }
        }
    }
    D_801235B4 = 0;
    updateEffectTasks();
}
#endif

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
    createEffectTask(func_800257F0, 0, 0x63);
    D_8010ADE8 = createEffectTask(func_80028194, 0, 0x61);
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
        var_v0_4 = gCourseSelectStatus.unk2E;
        var_a2_2 = 0;
        if (var_v0_4 == 1) {
            gCourseSelectStatus.unk2E = 0;
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
    setCurrentInputTaskCallback(func_8000A214, 0);
    updateEffectTasks();
}
#endif

// func_8000A214 best match: 54.533% (nonmatchings/func_8000A214-7123131487808489545/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_8000A214.s")

#ifdef NON_MATCHING
void func_8000A214(void) {
    s32 sp44;
    s8 sp33;                                        /* compiler-managed */
    u8 sp32;
    s8 sp31;
    ObjectA3E0 *sp18;
    CourseSelectSelection *var_v0_5;
    CourseSelectSelection *temp_v0_3;
    ObjectA3E0 *var_v0_7;
    s16 temp_a0;
    s16 temp_t6;
    s16 temp_t7;
    s16 temp_v0;
    s32 *var_v1;
    s32 temp_t4;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a3;
    s32 var_a3_2;
    s32 var_a3_3;
    s8 temp_a0_2;
    u16 var_v1_2;
    u16 var_v1_3;
    u8 var_a1;
    u8 var_t1;
    u8 var_t8;
    u8 var_v0_3;
    u8 var_v0_4;
    u8 var_v0_6;
    CourseSelectSelection *var_v0;
    CourseSelectSelection *var_v0_2;

    var_a0 = 0;
    if (D_80121D88 == 9) {
        var_a3 = 0;
        if ((s32) D_80121B55 > 0) {
            var_v1 = D_8010AEE8;
            do {
                if ((*var_v1 != 0) || (temp_v0 = (&D_800EC9D0)[var_a3], (temp_v0 == 1)) || (temp_v0 >= 5)) {
                    var_a0 += 1;
                }
                var_a3 += 1;
                var_v1 += 1;
            } while (var_a3 < (s32) D_80121B55);
        }
        if (var_a0 == 0) {
            var_a3 = 0;
            if ((s32) D_80121B55 < 3) {
                var_a1 = D_80121B55;
            } else {
                var_a1 = 4;
            }
            if ((s32) var_a1 > 0) {
                temp_v1 = var_a1 & 3;
                if (temp_v1 != 0) {
                    var_v0 = &D_80121D80;
                    do {
                        var_a3 += 1;
                        var_v0->unk8 = 3;
                        var_v0 += 1;
                    } while (temp_v1 != var_a3);
                    if (var_a3 != var_a1) {
                        goto block_17;
                    }
                } else {
block_17:
                    var_v0_2 = &D_80121D80 + var_a3;
                    do {
                        var_a3 += 4;
                        var_v0_2->unk8 = 3;
                        var_v0_2[1].unk8 = 3;
                        var_v0_2[2].unk8 = 3;
                        var_v0_2[3].unk8 = 3;
                        var_v0_2 += 4;
                    } while (var_a3 != var_a1);
                }
            }
            D_800EC9C0 = 0xF;
            D_801235B4 = 1;
        }
        var_v0_3 = (u8) D_800EC9C0;
        sp44 = var_a3;
    } else if ((u8) D_800EC9C0 == 0) {
        temp_a0 = D_800EC9D0;
        var_a3_2 = 0;
        if (temp_a0 == 0) {
            if ((gCourseSelectStatus.unk0[0] == 1) && (D_80121D88 == 0) && !(D_8010AECC & 1)) {
                if ((D_80121D85 == 5) || (*D_8010AEA0 == 0) || (var_t1 = 4, (D_800EC9E6 == 1))) {
                    var_t1 = 3;
                }
                sp32 = var_t1;
                if ((s32) D_80121B55 >= 2) {
                    var_t1 = (var_t1 - 1) & 0xFF;
                }
                temp_v0_2 = gPlayerInputHeld & 0x10800;
                sp31 = D_8010AE64;
                if ((temp_v0_2 == 0) && !(gPlayerInputHeld & 0x20400)) {
                    D_8010ADF0 = 0;
                }
                if ((gPlayerInputPressed & 0x10800) || ((temp_v0_2 != 0) && ((s32) (u16) D_8010ADF0 >= 9))) {
                    if ((u16) D_8010ADF0 == 0) {
                        D_8010ADF0 = (u16) D_8010ADF0 + 1;
                    }
                    if (D_8010AE64 > 0) {
                        D_8010AE64 -= 1;
                        sp44 = 0;
                        sp33 = var_t1;
                        enqueueSoundEffect(0x19, 0x32);
                        var_a3_2 = 0;
                    }
                } else if ((gPlayerInputPressed & 0x20400) || ((gPlayerInputHeld & 0x20400) && ((s32) (u16) D_8010ADF0 >= 9))) {
                    if ((u16) D_8010ADF0 == 0) {
                        D_8010ADF0 = (u16) D_8010ADF0 + 1;
                    }
                    if (D_8010AE64 < (s32) var_t1) {
                        D_8010AE64 += 1;
                        sp44 = 0;
                        sp33 = (s8) var_t1;
                        enqueueSoundEffect(0x19, 0x32);
                        var_a3_2 = 0;
                        var_t1 = (u8) sp33;
                    }
                }
                if ((u16) D_8010ADF0 != 0) {
                    temp_t7 = (u16) D_8010ADF0 + 1;
                    D_8010ADF0 = temp_t7;
                    if ((temp_t7 & 0xFFFF) == 0xFFFF) {
                        D_8010ADF0 = 0xA;
                    }
                }
                if ((sp32 != D_8010AE64) && (sp32 != (u8) sp31) && ((u8) sp31 != D_8010AE64)) {
                    D_8010AECC += 1;
                    if ((s32) (u8) sp31 < D_8010AE64) {
                        D_8010AEE8[0] = 0xFF800000;
                    } else {
                        D_8010AEE8[0] = 0x800000;
                    }
                }
                if (sp32 == D_8010AE64) {
                    var_v0_4 = *(&D_8010AEF7 + sp32);
                    D_8010AF46 = 2;
                } else {
                    var_v0_4 = D_8010AEF8[D_8010AE64];
                    D_8010AF46 = 0;
                }
                D_80121D86 = var_v0_4;
                if ((s32) D_80121D80.unk6 >= 9) {
                    D_80121D80.unk7 = 0;
                } else {
                    temp_a0_2 = D_800ECA2F[D_80121D80.unk6];
                    if (temp_a0_2 == -1) {
                        D_80121D80.unk7 = (u8) ((s32) D_80121D80.unk6 % 3);
                    } else {
                        D_80121D80.unk7 = (u8) temp_a0_2;
                    }
                }
                if ((D_8010AEE8[0] == 0) && ((gPlayerInputPressed & 0x1000) || (gPlayerInputPressed & 0x8000))) {
                    sp44 = 0;
                    sp33 = var_t1;
                    enqueueSoundEffect(1, 0x32);
                    var_a3_2 = 0;
                    if ((D_80121B55 == 1) && (var_t1 == D_8010AE64)) {
                        D_80121D88 = 9;
                        D_801235B4 = 1;
                    } else {
                        D_8010AEA4 = 1;
                        D_800EC9D0 = 9;
                        D_8010ADF0 = 0;
                        D_80121D88 = 1;
                        if (D_800EC9E6 == 0) {
                            sp44 = 0;
                            createEffectTask(func_8002EC04, 0, 0x62);
                            var_a3_2 = 0;
                        }
                    }
                } else if (gPlayerInputPressed & 0x4000) {
                    sp44 = 0;
                    enqueueSoundEffect(1, 0x32);
                    D_8010ADF0 = 0;
                    var_a3_2 = 0;
                    if ((s32) D_80121B55 > 0) {
                        var_v0_5 = &D_80121D80;
                        do {
                            var_v0_5->unk8 = 9;
                            var_v0_5 += 1;
                        } while ((u32) var_v0_5 < (u32) (&D_80121D80 + D_80121B55));
                    }
                }
            }
            if ((D_80121D88 == 1) && (D_800EC9E6 == 1)) {
                sp44 = var_a3_2;
                setCurrentInputTaskCallback(func_8000B220, 0);
                D_801235B8->screenState = 0;
            }
        } else {
            if (temp_a0 == 9) {
                D_8010AEA4 = (u8) D_8010AEA4 + 1;
            } else {
                D_8010AEA4 = 0;
            }
            if ((D_800EC9D0 >= 2) && (D_800EC9D0 < 5)) {
                if (!(gPlayerInputHeld & 0x10800) && !(gPlayerInputHeld & 0x20400)) {
                    D_8010ADF0 = 0;
                }
                temp_t4 = gPlayerInputPressed;
                if ((temp_t4 & 0x10800) || ((gPlayerInputHeld & 0x10800) && ((s32) (u16) D_8010ADF0 >= 9) && ((u16) D_8010ADF0 & 1))) {
                    var_v1_2 = (u16) D_8010ADF0;
                    if (var_v1_2 == 0) {
                        var_v1_2 = (var_v1_2 + 1) & 0xFFFF;
                    }
                    D_8010ADF0 = (s16) var_v1_2;
                    if (D_800EC9D0 >= 3) {
                        D_800EC9D0 -= 1;
                        sp44 = 0;
                        enqueueSoundEffect(0x19, 0x32);
                        var_a3_2 = 0;
                        D_8010AF44 = 0;
                    }
                } else {
                    var_v1_3 = (u16) D_8010ADF0;
                    if ((temp_t4 & 0x20400) || ((gPlayerInputHeld & 0x20400) && ((s32) var_v1_3 >= 9) && (var_v1_3 & 1))) {
                        if (var_v1_3 == 0) {
                            var_v1_3 = (var_v1_3 + 1) & 0xFFFF;
                        }
                        D_8010ADF0 = (s16) var_v1_3;
                        if (D_800EC9D0 < (D_8010AF3C + 1)) {
                            D_800EC9D0 += 1;
                            sp44 = 0;
                            enqueueSoundEffect(0x19, 0x32);
                            var_a3_2 = 0;
                            D_8010AF44 = 0;
                        }
                    }
                }
                if ((u16) D_8010ADF0 != 0) {
                    temp_t6 = ((u16) D_8010ADF0 + 1) & 0xFFFF;
                    D_8010ADF0 = temp_t6;
                    if (temp_t6 == -1) {
                        D_8010ADF0 = 0xA;
                    }
                }
                if ((*D_8010AEA0 == 1) && (D_8010AE64 == 3)) {
                    var_t8 = *(&D_8010AF06 + temp_a0);
                } else {
                    var_t8 = ((temp_a0 * 3) + ((s32) D_80121D86 % 3)) - 6;
                }
                D_80121D86 = var_t8;
                if (temp_t4 & 0x4000) {
                    sp44 = 0;
                    enqueueSoundEffect(0x18, 0x32);
                    D_8010AF44 = 0;
                    D_80121D86 = D_8010AEF8[D_8010AE64];
                    D_8010ADF0 = 0;
                    var_a3_2 = 0;
                    D_800EC9D0 += 3;
                } else if ((temp_t4 & 0x8000) || (temp_t4 & 0x1000)) {
                    if (D_800ECA2F[D_80121D86] == -1) {
                        if ((u32) D_80121D8C >= (u32) *(&D_800B34B0 + (D_80121D86 * 4))) {
                            sp44 = 0;
                            enqueueSoundEffect(0x49, 0x32);
                            var_a3_2 = 0;
                            if ((*D_8010AEA0 == 1) && (D_8010AE64 == 3)) {
                                var_v0_6 = *(&D_8010AF06 + D_800EC9D0);
                            } else {
                                var_v0_6 = (((D_800EC9D0 * 3) + ((s32) D_80121D86 % 3)) - 6) & 0xFF;
                            }
                            D_80121D87 = D_800ECA2F[var_v0_6];
                            D_80121D86 = var_v0_6;
                            if (D_8010AECC == 0) {
                                D_8010AF2C = 0;
                                D_8010AF1C = 7;
                            } else {
                                D_8010AF34 = 0;
                                D_8010AF20 = 7;
                            }
                        } else if (D_8010AF44 == 0) {
                            sp44 = 0;
                            enqueueSoundEffect(0x47, 0x32);
                            var_a3_2 = 0;
                            D_8010AF44 = 1;
                        }
                    } else if (D_8010AF44 == 0) {
                        D_8010AF44 = 2;
                    }
                }
            }
        }
        if ((D_8010AF1C == 7) || (D_8010AF20 == 7)) {
            D_801235B8->timer = 0;
            sp44 = var_a3_2;
            setCurrentInputTaskCallback(func_8000AFE8, 0);
            createEffectTask(func_8002EFB8, 0, 0x64);
        }
        var_v0_3 = (u8) D_800EC9C0;
        sp44 = var_a3_2;
    } else {
        D_800EC9C0 = (u8) D_800EC9C0 + 1;
        var_v0_3 = (u8) D_800EC9C0;
        if ((s32) var_v0_3 >= 0x22) {
            D_800EC9C0 = 0;
            var_v0_3 = 0;
        }
    }
    if (var_v0_3 == 0x1B) {
        temp_v0_3 = &D_80121D80 + sp44;
        D_801235B8->fade = 1;
        D_80121D88 = 0;
        sp18 = temp_v0_3;
        setCurrentInputTaskCallback(func_80009C48, 0);
        temp_v0_3->pad0[6] = D_8010AEF8[D_8010AE64];
        if (D_801235B4 == 1) {
            D_80121D86 = 0;
        }
        D_801235B4 = 0;
        D_800EC9C0 = 0;
        D_800EC9D0 = 0;
        D_8010AECC = 0;
        gCourseSelectStatus.unk0[0] = 0;
        D_8010AF1C = 0;
        D_8010AF20 = 0;
        D_8010AF24 = 0;
        D_8010AF28 = 0;
        D_8010AF2C = 0;
        D_8010AF34 = 0;
        D_8010AF3C = 0;
        D_8010AF19 = 0;
        D_8010AF1D = 0;
        D_8010AF21 = 0;
        D_8010AF25 = 0;
        D_8010AF29 = 0;
        D_8010AF2E = 0;
        D_8010AF36 = 0;
        D_8010AF3D = 0;
        D_8010AF1A = 0;
        D_8010AF1E = 0;
        D_8010AF22 = 0;
        D_8010AF26 = 0;
        D_8010AF2A = 0;
        D_8010AF30 = 0;
        D_8010AF38 = 0;
        D_8010AF3E = 0;
        D_8010AF1B = 0;
        D_8010AF1F = 0;
        D_8010AF23 = 0;
        D_8010AF27 = 0;
        D_8010AF2B = 0;
        D_8010AF32 = 0;
        D_8010AF3A = 0;
        D_8010AF3F = 0;
        gCourseSelectStatus.transitionState = 0;
        gCourseSelectStatus.unk28 = 0;
        gCourseSelectStatus.unk2A = 0;
        gCourseSelectStatus.unk2C = 0;
        gCourseSelectStatus.unk2E = 0;
    }
    var_a3_3 = 0;
    if ((s32) D_80121B55 > 0) {
        var_v0_7 = D_801121E0;
        do {
            D_800EC9C4 = var_v0_7;
            sp18 = var_v0_7;
            sp44 = var_a3_3;
            var_v0_7->unk2C();
            var_a3_3 += 1;
            var_v0_7 += 0xB0;
        } while (var_a3_3 < (s32) D_80121B55);
    }
    updateEffectTasks();
}

#endif

// func_8000AFE8 best match: 99.401% (nonmatchings/func_8000AFE8-6061209858023118177/base_22.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_8000AFE8.s")

#ifdef NON_MATCHING
void func_8000AFE8(void) {
    ObjectA3E0 *var_s1;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_s0;
    s32 *new_var2;
    u8 new_var;

    temp_v0 = D_801235B8->timer;
    new_var2 = &gPlayerInputPressed;
    if (temp_v0 < 2) {
        temp_v1 = *new_var2;
        if ((temp_v1 & 0x10800) && (temp_v0 != 0)) {
            D_801235B8->timer = temp_v0 - 1;
            enqueueSoundEffect(0x19, 0x32);
            temp_v1 = gPlayerInputPressed;
        } else if ((temp_v1 & 0x20400) && (temp_v0 != 1)) {
            D_801235B8->timer = temp_v0 + 1;
            enqueueSoundEffect(0x19, 0x32);
            temp_v1 = gPlayerInputPressed;
        }

        if (temp_v1 & 0x8000) {
            D_801235B4 = 1;
            if (D_801235B8->timer == 1) {
                enqueueSoundEffect(0x18, 0x32);
                if (D_8010AECC == 0) {
                    D_8010AF1C = 1;
                } else {
                    D_8010AF20 = 1;
                }
                setCurrentInputTaskCallback(func_8000A214, 0);
            } else {
                enqueueSoundEffect(0x45, 0x32);
                D_801235B8->timer += 2;
            }
        } else if (temp_v1 & 0x4000) {
            D_801235B4 = 1;
            enqueueSoundEffect(0x18, 0x32);
            if (D_8010AECC == 0) {
                D_8010AF1C = 1;
            } else {
                D_8010AF20 = 1;
            }
            setCurrentInputTaskCallback(func_8000A214, 0);
        }
    } else if (temp_v0 > (4 - 1)) {
        D_801235B8->timer = 0;
        D_80121D88 = 9;
        setCurrentInputTaskCallback(func_8000A214, 0);
    }

    var_s0 = 0;
    if (D_80121B55 > 0) {
        var_s1 = D_801121E0;
        do {
            D_800EC9C4 = var_s1;
            var_s1->unk2C();
            var_s0++;
            var_s0--;
            var_s0 += 1;
            var_s1 += 1;
        } while (var_s0 < (s32) (new_var = D_80121B55));
    }
    updateEffectTasks();
}
#endif

// func_8000B220 best match: 56.849% (nonmatchings/func_8000B220-7273315160691878794/base_9.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_8000B220.s")

#ifdef NON_MATCHING
void func_8000B220(void) {
    s32 sp24;
    ObjectA3E0 *var_s0;
    s16 state;
    s32 buttonsHeld;
    s32 buttonsPressed;
    s32 held10800;
    u16 repeat;
    s32 divisor;
    s32 i;
    u8 index;
    ObjectA3E0 **currentPtr;

    if (D_800EC9D0 == 9) {
        D_8010AEA4++;
    } else {
        D_8010AEA4 = 0;
    }

    state = *(volatile s16 *)&D_800EC9D0;
    if (state < 2) {
        goto outside_menu_rows;
    }
    if (state >= 5) {
        goto outside_menu_rows;
    }

    buttonsHeld = gPlayerInputHeld;
    held10800 = buttonsHeld & 0x10800;
    if ((held10800 == 0) && ((buttonsHeld & 0x20400) == 0)) {
        D_8010ADF0 = 0;
    }

    buttonsPressed = gPlayerInputPressed;
    repeat = D_8010ADF0;
    if (buttonsPressed & 0x10800) {
        goto move_up;
    }
    if (held10800 != 0) {
        if (repeat >= 9) {
            if (repeat & 1) {
                goto move_up;
            }
        }
    }
    if (!(buttonsPressed & 0x20400)) {
        D_8010ADF0 = repeat;
        if (!(buttonsHeld & 0x20400)) {
            goto after_row_change;
        }
        D_8010ADF0 = repeat;
        if (repeat < 9) {
            goto after_row_change;
        }
        if (!(repeat & 1)) {
            goto after_row_change;
        }
    }

move_down:
    if (repeat == 0) {
        repeat++;
    }
    D_8010ADF0 = repeat;
    if (state < D_8010AF3C + 1) {
        D_800EC9D0 = state + 1;
        enqueueSoundEffect(0x19, 0x32);
        state = D_800EC9D0;
        buttonsPressed = gPlayerInputPressed;
    }
    goto after_row_change;

move_up:
    if (repeat == 0) {
        repeat++;
    }
    D_8010ADF0 = repeat;
    if (state >= 3) {
        D_800EC9D0 = state - 1;
        enqueueSoundEffect(0x19, 0x32);
        state = D_800EC9D0;
        buttonsPressed = gPlayerInputPressed;
    }

after_row_change:
    divisor = 3;
    repeat = D_8010ADF0;
    if (repeat != 0) {
        repeat++;
        D_8010ADF0 = repeat;
        if (repeat == 0xFFFF) {
            repeat = 10;
            D_8010ADF0 = repeat;
        }
    }

    D_80121D86 = ((state * divisor) + (D_80121D86 % divisor) - 6) & 0xFF;
    D_800EC9D0 = state;
    if (buttonsPressed & 0x4000) {
        enqueueSoundEffect(0x18, 0x32);
        D_80121D86 = (D_80121D86 % divisor) & 0xFF;
        D_8010ADF0 = 0;
        D_800EC9D0 += 3;
    } else if ((buttonsPressed & 0x8000) || (buttonsPressed & 0x1000)) {
        enqueueSoundEffect(0x18, 0x32);
        index = ((D_800EC9D0 * divisor) + (D_80121D86 % divisor) - 6) & 0xFF;
        D_80121D87 = D_800ECA2F[index];
        D_80121D86 = index;
        if (D_8010AECC == 0) {
            D_8010AF2C = 0;
            D_8010AF1C = 7;
        } else {
            D_8010AF34 = 0;
            D_8010AF20 = 7;
        }
    }
    goto after_input;

outside_menu_rows:
    if (D_800EC9D0 == 0) {
        setCurrentInputTaskCallback(func_8000A214, 0);
    }

after_input:
    if ((gCourseSelectStatus.unk4Array[0] == 7) || (gCourseSelectStatus.unk8Array[0] == 7)) {
        D_801235B8->screenState = 1;
        setCurrentInputTaskCallback(func_8000B690, 0);
    }
    i = 0;
    if ((s32) D_80121B55 > 0) {
        currentPtr = &D_800EC9C4;
        var_s0 = D_801121E0;
        do {
            *currentPtr = var_s0;
            sp24 = i;
            var_s0->unk2C();
            i += 1;
            var_s0 += 1;
        } while (i < (s32) D_80121B55);
    }
    updateEffectTasks();
}
#endif

void func_8000B690(void) {
    ObjectA3E0 *var_s1;
    s32 var_s0;
    s8 temp_v0;

    if (D_801235B8->screenState == 2) {
        createEffectTask(func_8002F854, 0, 0x63);
        temp_v0 = D_800ECA2F[D_80121D86];
        D_8010AF72 = temp_v0 % 7;
        D_8010AF73 = temp_v0 / 7;
        D_8010ADF0 = 0;
        D_8010ADF8 = 0;
        setCurrentInputTaskCallback(func_8000B7B8, 0);
    }

 do { var_s0 = 0; if (D_80121B55 > 0) { var_s1 = D_801121E0; do { ; (D_800EC9C4 = var_s1)->unk2C(); var_s0 += 1; var_s1 += 1; } while (var_s0 < D_80121B55); } } while (0);
    updateEffectTasks();
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
        temp_v0 = gPlayerInputHeld & 0x10800;
        temp_t1 = var_a1;
        if ((temp_v0 == 0) && !(gPlayerInputHeld & 0x20400)) {
            D_8010ADF0 = 0;
        }
        temp_a2 = gPlayerInputPressed;
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
                if ((gPlayerInputHeld & 0x20400) && ((s32) var_v1 >= 0xB)) {
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
            enqueueSoundEffect(0x19, 0x32);
            temp_a2 = gPlayerInputPressed;
        }
        if (temp_a2 & 0x4000) {
            D_8010ADF8 = 2;
        }
        if ((temp_a2 & 0x8000) || (temp_a2 & 0x1000) || ((u8) D_8010ADF8 != 0)) {
            temp_a0 = (u8) D_8010AF72;
            if (((s32) temp_a0 >= 7) || ((u8) D_8010ADF8 != 0)) {
                enqueueSoundEffect(0x18, 0x32);
            }
            if (((temp_a0 == 7) || ((u8) D_8010ADF8 == 1)) && ((u8) D_8010ADF8 != 2)) {
                D_8010AF41 = 3;
                if ((u8) D_8010ADF8 == 1) {
                    D_8010AF74 = 1;
                }
                D_8010ADF8 = 0;
                setCurrentInputTaskCallback(func_8000BAFC, 0);
            } else if ((temp_a0 == 8) || ((u8) D_8010ADF8 == 2)) {
                setCurrentInputTaskCallback(func_8000BBB4, 0);
                D_8010ADF8 = 0;
                D_801235B8->screenState = 3;
            } else if (temp_a0 == 9) {
                setCurrentInputTaskCallback(func_8000BCA0, 0);
                D_801235B8->screenState = 9;
            } else {
                setCurrentInputTaskCallback(func_8000BEC0, 0);
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
    updateEffectTasks();
}
#endif

void func_8000BAFC(void) {
    ObjectA3E0 *var_s1;
    s32 var_s0;

    if (gCourseSelectStatus.transitionState == 2) {
        D_8010ADF0 = 0;
        D_8010AF74 = 0;
        setCurrentInputTaskCallback(func_8000B7B8, 0);
    }

 do { var_s0 = 0; if (D_80121B55 > 0) { var_s1 = D_801121E0; do { ; (D_800EC9C4 = var_s1)->unk2C(); var_s0 += 1; var_s1 += 1; } while (var_s0 < D_80121B55); } } while (0);
    updateEffectTasks();
}

void func_8000BBB4(void) {
    ObjectA3E0 *var_s1;
    s32 var_s0;

    if (D_801235B8->screenState == 5) {
        if (D_8010AECC == 0) {
            gCourseSelectStatus.playerOneCourseDecided = 1;
        } else {
            gCourseSelectStatus.playerTwoCourseDecided = 1;
        }
        gCourseSelectStatus.transitionState = 0;
        D_801235B8->screenState = 0;
        D_8010ADF0 = 0;
        setCurrentInputTaskCallback(func_8000B220, 0);
    }

 do { var_s0 = 0; if (D_80121B55 > 0) { var_s1 = D_801121E0; do { ; (D_800EC9C4 = var_s1)->unk2C(); var_s0 += 1; var_s1 += 1; } while (var_s0 < D_80121B55); } } while (0);
    updateEffectTasks();
}

void func_8000BCA0(void) {
    ObjectA3E0 *var_s1;
    s32 var_s0;
    s32 i;

    if (D_801235B8->screenState == 0xB) {
        setCurrentInputTaskCallback(func_80009C48, 0);
        D_801235B8->fade = 1;
        D_801235B8->timer = 0;
        D_801235B8->screenState = 0;
        D_80121D80.unk8 = 0;
        D_80121D80.unk6 = D_80121D80.unk6 % 3;
        D_800EC9D0 = 0;
        D_8010AECC = 0;
        D_8010ADF0 = 0;

        for (i = 0; i < 4; i++) {
            gCourseSelectStatus.unk0Array[i] = 0;
            gCourseSelectStatus.unk4Array[i] = 0;
            gCourseSelectStatus.unk8Array[i] = 0;
            gCourseSelectStatus.unkCArray[i] = 0;
            gCourseSelectStatus.unk10Array[i] = 0;
            gCourseSelectStatus.unk14[i] = 0;
            gCourseSelectStatus.unk1C[i] = 0;
            gCourseSelectStatus.unk24[i] = 0;
        }

        gCourseSelectStatus.unk28 = 0;
        gCourseSelectStatus.unk2A = 0;
        gCourseSelectStatus.transitionState = 0;
        gCourseSelectStatus.unk2C = 0;
        gCourseSelectStatus.unk2E = 0;
    }

 do { var_s0 = 0; if (D_80121B55 > 0) { var_s1 = D_801121E0; do { ; (D_800EC9C4 = var_s1)->unk2C(); var_s0 += 1; var_s1 += 1; } while (var_s0 < D_80121B55); } } while (0);
    updateEffectTasks();
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
    gCourseSelectStatus.transitionState = 6;
    createEffectTask(&func_8002FEF8, 0, 0x64);
    setCurrentInputTaskCallback(func_8000C010, 0); var_s0 = D_801121E0; do { D_800EC9C4 = var_s0; var_s0->unk2C();
        var_s0 += 1;
    } while (var_s0 != &D_80112340);
    updateEffectTasks();
    enqueueSoundEffect(0x17, 0x32);
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
        setCurrentInputTaskCallback(func_8000B7B8, 0);
        gCourseSelectStatus.transitionState = 2;
 D_8010AED0 = 0; } var_s0 = D_801121E0; do { (D_800EC9C4 = var_s0)->unk2C(); var_s0 += 1; } while (var_s0 != (&D_80112340)); updateEffectTasks();
}

void func_8000C114(void) {
    s8 *ptr;
    s32 count;

    if (D_801235B8->fade != 0xFF) {
        D_801235B8->fade = stepMenuFadeAlpha((s16) D_801235B8->fade, 0x24, 1);
        if (D_801235B8->fade == 0xFF) {
            gFramebufferSwapHold = 1;
        } else {
            updateEffectTasks();
        }
    } else if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        do { gFramebufferSwapDelay = 0; D_801235B4 = 0; resumeInputTask(2); removeInputTask(4); count = 0; if (D_800ECA2F[0] == (-1)) { count = 1; } ptr = &D_800EC9F1; loop: if (ptr[0x3F] == (-1)) { count++; } if (ptr[0x40] == (-1)) { count++; } if (ptr[0x41] == (-1)) { count++; } if (ptr[0x42] == (-1)) { count++; } } while (0);
        ptr += 4;
        if (ptr != (&D_800EC9F9)) {
            goto loop;
        }
        if (count == 0) {
            D_800EC9F0[0x78D7] |= 4;
        }
    }
}
