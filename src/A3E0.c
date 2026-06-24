#include "common.h"

typedef struct ObjectA3E0 {
    char pad0[0x2C];
    void (*unk2C)(void);
    char pad30[0x80];
} ObjectA3E0;

typedef struct {
    char pad0[0x20];
    s32 unk20;
} Struct801235B8A3E0;

typedef struct {
    char pad0[4];
    u8 unk4;
    char pad5[3];
    u8 unk8;
    char pad9[0x20];
    u8 unk29;
} Struct8010AF18;

extern ObjectA3E0 *D_800EC9C4;
extern f32 D_800E0A38;
extern s16 D_8010AED0;
extern s16 D_8010ADF0;
extern u8 D_8010AECC;
extern Struct8010AF18 D_8010AF18;
extern u8 D_8010AF41;
extern s16 D_8010AF76;
extern s8 D_8010AF74;
extern ObjectA3E0 D_801121E0[];
extern ObjectA3E0 D_80112340;
extern u8 D_80121B55;
extern Struct801235B8A3E0 *D_801235B8;
extern void func_8000B220(void);
extern void func_8000B7B8(void);
extern void func_80070614(s32);
extern void func_8007066C(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_8007105C(void);
extern void func_8009956C(void *, s32);

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_800097E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_80009C48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000A048.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000A214.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000AFE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000B220.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000B690.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000B7B8.s")

void func_8000BAFC(void) {
    ObjectA3E0 *var_s1;
    s32 var_s0;

    if (D_8010AF41 == 2) {
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

    if (D_801235B8->unk20 == 5) {
        if (D_8010AECC == 0) {
            D_8010AF18.unk4 = 1;
        } else {
            D_8010AF18.unk8 = 1;
        }
        D_8010AF18.unk29 = 0;
        D_801235B8->unk20 = 0;
        D_8010ADF0 = 0;
        func_8009956C(func_8000B220, 0);
    }
 do { var_s0 = 0; if (D_80121B55 > 0) { var_s1 = D_801121E0; do { ; (D_800EC9C4 = var_s1)->unk2C(); var_s0 += 1; var_s1 += 1; } while (var_s0 < D_80121B55); } } while (0);
    func_8007105C();
}

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000BCA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000BEC0.s")

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
        D_8010AF41 = 2;
 D_8010AED0 = 0; } var_s0 = D_801121E0; do { (D_800EC9C4 = var_s0)->unk2C(); var_s0 += 1; } while (var_s0 != (&D_80112340)); func_8007105C();
}

#pragma GLOBAL_ASM("asm/nonmatchings/A3E0/func_8000C114.s")
