#include "common.h"

extern void func_80045914(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);
extern s8 D_800DEED4;
extern u8 D_80123750;
extern u8 D_80123751;
extern s16 D_800DEF14;
extern void func_8005393C(void);
extern void func_8006D780(s32);
extern void func_8007105C(void);
extern void func_80071664(void *, s32, s32, s32);
extern void func_800720E4(s32);
extern void func_8009956C(void *, s32);
void func_8003E45C(void);
void func_8003E5A8(void);

typedef struct {
    char pad[0x18];
    s32 unk18;
} Struct801235B8;

typedef struct StructEC9C4 {
    char pad0[4];
    s16 unk4;
    s16 unk6;
    char pad8[0x10];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    char pad28[8];
    char unk30[0x14];
    s32 unk44;
    s32 unk48;
    s32 unk4C;
} StructEC9C4;

typedef struct StackDDD0 {
    char sp20[0x20];
    s32 sp40;
    s32 sp44;
    s32 sp48;
    s32 sp4C;
    s32 sp50;
    s32 sp54;
} StackDDD0;

typedef struct Vec2 {
    s32 x;
    s32 y;
} Vec2;

extern Struct801235B8 *D_801235B8;
extern StructEC9C4 *D_800EC9C4;
extern Vec2 D_8010B1B0;
extern void func_80097FE4(void *, s16, s16);
extern void func_8009853C(void *, s16, s16);
extern void func_80098590(void *, s32 *, s32 *);

void func_8003DDD0() {
    StackDDD0 stack;
    StructEC9C4 *temp;

    temp = D_800EC9C4;
    func_8009853C(temp->unk30, -temp->unk4, -temp->unk6);
    temp = D_800EC9C4;
    stack.sp4C = 0;
    stack.sp50 = 0;
    stack.sp54 = -temp->unk24;
    func_80097FE4(stack.sp20, temp->unk4, temp->unk6);
    func_80098590(stack.sp20, &stack.sp4C, &stack.sp40);
    temp = D_800EC9C4;
    temp->unk44 = stack.sp40 - temp->unk18;
    temp = D_800EC9C4;
    temp->unk48 = stack.sp44 - temp->unk1C;
    temp = D_800EC9C4;
    temp->unk4C = stack.sp48 - temp->unk20;
}

struct ALSeqPlayer;

void n_alSeqpDelete(struct ALSeqPlayer *seqp) {
    func_8003DDD0(seqp);
}

void func_8003DEC8(void) {
    StackDDD0 stack;
    StructEC9C4 *temp;

    temp = D_800EC9C4;
    func_8009853C(temp->unk30, -temp->unk4, -temp->unk6);
    temp = D_800EC9C4;
    stack.sp4C = D_8010B1B0.x;
    stack.sp50 = D_8010B1B0.y;
    stack.sp54 = -temp->unk24;
    func_80097FE4(stack.sp20, temp->unk4, temp->unk6);
    func_80098590(stack.sp20, &stack.sp4C, &stack.sp40);
    temp = D_800EC9C4;
    temp->unk44 = stack.sp40 - temp->unk18;
    temp = D_800EC9C4;
    temp->unk48 = stack.sp44 - temp->unk1C;
    temp = D_800EC9C4;
    temp->unk4C = stack.sp48 - temp->unk20;
}

void func_8003DFB0(void) {
    func_8003DEC8();
}

#pragma GLOBAL_ASM("asm/nonmatchings/3E9D0/func_8003DFD0.s")

void func_8003E3AC(void) {
    D_801235B8->unk18--;
    if (D_801235B8->unk18 == 0) {
        func_800720E4(4);
        D_801235B8->unk18 = 0x12C;
        func_8009956C(func_8003E45C, 0);
    }
    func_80071664(func_8005393C, 5, 0x64, 0);
    func_8006D780(0);
    func_8006D780(1);
    func_8006D780(2);
    func_8007105C();
}

#pragma GLOBAL_ASM("asm/nonmatchings/3E9D0/func_8003E45C.s")

void func_8003E514(void) {
    D_800DEF14 += 4;
    if (D_800DEF14 >= 0xFF) {
        D_801235B8->unk18 = 0xFF;
        D_80123751 = 1;
        func_8009956C(func_8003E5A8, 0);
    }
    func_80071664(func_8005393C, 5, 0x64, 0);
    func_8006D780(0);
    func_8006D780(1);
    func_8007105C();
}

void func_8003E5A8(void) {
    if (D_80123750 == 2) {
        func_80045914();
        D_80123751 = 0;
        D_800DEED4 = 0;
        func_80099658(2);
        func_8009954C(4);
    }
}
