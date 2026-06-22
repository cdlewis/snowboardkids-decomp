#include "common.h"

typedef struct {
    char pad[0x18];
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
} Struct33220;

typedef struct {
    char pad[0x1C];
    s32 unk1C;
} StructD801235B8;

extern StructD801235B8 *D_801235B8;
extern s32 D_80124868;
extern s16 D_8011217A;
extern s16 D_80112172;

extern void func_800483FC(void *, void *, void *);
extern void func_80071824(void *, void *);
extern void func_80011D74(void *, s32, s16, s16);
extern void func_80017168(void *, s32);
extern s32 func_80043040(s16);
extern void func_8000F030(s16, s16, s32, s32, s32, s32, s32, s32);

void func_80032620(Struct33220 *arg0);
void func_80032654(s32 arg0);
void func_8003288C(Struct33220 *arg0);
void func_800326EC(void *arg0);
void func_80032934(Struct33220 *arg0);
void func_80032984(Struct33220 *arg0);
void func_800329E0(Struct33220 *arg0);
void func_80032A50(Struct33220 *arg0);

void func_80032620(Struct33220 *arg0) {
    func_80011D74(&arg0->unk1C, 1, arg0->unk18, arg0->unk1A);
}

void func_80032654(s32 arg0) {
    func_800483FC(&D_80124868, func_80032620, (void *) arg0);
}

void func_80032684(Struct33220 *arg0) {
    func_80017168(&arg0->unk1C, func_80043040(D_8011217A));
    arg0->unk1C = 0x10;
    arg0->unk1E = 0x10;
    arg0->unk18 = arg0->unk24;
    arg0->unk1A = arg0->unk26;
    func_80071824(arg0, func_80032654);
}

#pragma GLOBAL_ASM("asm/nonmatchings/33220/func_800326EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/33220/func_8003288C.s")

void func_80032934(Struct33220 *arg0) {
    arg0->unk18 = -0x2C;
    arg0->unk1A = -0x2C;
    arg0->unk1C = 0x20;
    arg0->unk1E = 0x30;
    arg0->unk20 = 0;
    arg0->unk22 = 0;
    arg0->unk24 = 0x100;
    func_80071824(arg0, func_8003288C);
}

void func_80032984(Struct33220 *arg0) {
    s32 temp = func_80043040(D_80112172);
    func_8000F030(arg0->unk18, arg0->unk1A, temp, 3, 0x20, 0x20, 0, 0);
}

void func_800329E0(Struct33220 *arg0) {
    if (D_801235B8->unk1C != 1) {
        arg0->unk18 = -0x34;
    } else {
        arg0->unk18 = -0x3C;
    }
    arg0->unk1A = (D_801235B8->unk1C << 4) + 0x20;
    func_800483FC(&D_80124868, func_80032984, arg0);
}

void func_80032A50(Struct33220 *arg0) {
    arg0->unk1A = 0x20;
    func_80071824(arg0, func_800329E0);
}
