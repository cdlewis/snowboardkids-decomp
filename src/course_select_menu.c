#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 fade;
    /* 0x1C */ s32 timer;
    /* 0x20 */ s32 screenState;
} CourseSelectMenuState;

typedef struct {
    /* 0x00 */ u8 unk0[4];
    /* 0x04 */ u8 playerOneCourseDecided;
    /* 0x05 */ u8 unk5[3];
    /* 0x08 */ u8 playerTwoCourseDecided;
    /* 0x09 */ u8 unk9[7];
    /* 0x10 */ u8 unk10[4];
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

typedef struct ObjectA3E0 {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ void (*unk2C)(void);
    /* 0x30 */ u8 pad30[0x80];
} ObjectA3E0;

extern ObjectA3E0 *D_800EC9C4;
extern f32 D_800E0A38;
extern s8 D_800ECA2F[];
extern s16 D_8010AED0;
extern s16 D_8010ADF0;
extern s8 D_8010ADF8;
extern u8 D_8010AECC;
extern CourseSelectStatus D_8010AF18;
extern s8 D_8010AF72;
extern s8 D_8010AF73;
extern s16 D_8010AF76;
extern s8 D_8010AF74;
extern ObjectA3E0 D_801121E0[];
extern ObjectA3E0 D_80112340;
extern u8 D_80121B55;
extern u8 D_80121D86;
extern CourseSelectMenuState *D_801235B8;
extern void func_8000B220(void);
extern void func_8000B7B8(void);
extern void func_8002F854(void);
extern void func_80070614(s32);
extern void func_8007066C(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_8007105C(void);
extern void func_80071408(void *, s32, s32);
extern void func_8009956C(void *, s32);

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_800097E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_80009C48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_8000A048.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_8000A214.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_8000AFE8.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_8000B7B8.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_8000BCA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_8000BEC0.s")

void func_8000C010(void) {
    ObjectA3E0 *var_s0;

    D_8010AF76 -= 4;
    if (D_8010AF76 < 0) {
        D_8010AF76 = (D_8010AF76 < 0) * 0;
    }
    func_8007066C(1, 0xE8, 0x78, 0x90, D_8010AF76, 0xA0, 0xF0, D_800E0A38);
    if (D_8010AF76 == 0) {
        func_80070614(1);
        func_8009956C(func_8000B7B8, 0);
        D_8010AF18.transitionState = 2;
 D_8010AED0 = 0; } var_s0 = D_801121E0; do { (D_800EC9C4 = var_s0)->unk2C(); var_s0 += 1; } while (var_s0 != (&D_80112340)); func_8007105C();
}

#pragma GLOBAL_ASM("asm/nonmatchings/course_select_menu/func_8000C114.s")
