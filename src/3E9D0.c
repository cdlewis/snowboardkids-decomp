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
extern void func_8009956C(void *, s32);
void func_8003E5A8(void);

typedef struct {
    char pad[0x18];
    s32 unk18;
} Struct801235B8;

extern Struct801235B8 *D_801235B8;

#pragma GLOBAL_ASM("asm/nonmatchings/3E9D0/func_8003DDD0.s")

struct ALSeqPlayer;

extern void func_8003DDD0(struct ALSeqPlayer *seqp);

void n_alSeqpDelete(struct ALSeqPlayer *seqp) {
    func_8003DDD0(seqp);
}

#pragma GLOBAL_ASM("asm/nonmatchings/3E9D0/func_8003DEC8.s")

extern void func_8003DEC8(void);

void func_8003DFB0(void) {
    func_8003DEC8();
}

#pragma GLOBAL_ASM("asm/nonmatchings/3E9D0/func_8003DFD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/3E9D0/func_8003E3AC.s")

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
