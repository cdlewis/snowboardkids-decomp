#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_800458E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80045914.s")

void *func_8004597C(void *arg0, u32 arg1) {
    return (void *)((u8 *)arg0 + (arg1 & 0xFFFFFF));
}

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80045990.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_800459D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80045A1C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80045A78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80045E84.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80046358.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80046748.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80046970.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80046D68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80047174.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_8004767C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80047B84.s")

extern s32 func_80043040(s16);
extern s16 D_8011213C;
extern s16 D_801121B0;
extern s16 D_801121B2;
extern void *D_801121B4;

void func_80047E38(void) {
    s32 v0 = func_80043040(D_8011213C);
    D_801121B4 = (void *)((*(s32 *)(v0 + 4) << 3) + v0 + 8);
    D_801121B2 = -1;
    D_801121B0 = -1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80047E88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80048278.s")

extern s16 D_80112130;
s32 func_80043040(s16);                             /* extern */
extern s32 D_801121B8;
s16 func_80042D58(s32);

void func_80048338(void) {
    D_80112130 = func_80042D58(0x4000);
}

void func_8004835C(void) {
    D_801121B8 = func_80043040(D_80112130);
}

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80048388.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_800483FC.s")

extern s32 D_80124830;
extern s32 D_80124904;

typedef struct Node8004845C {
    struct Node8004845C *unk0;
    void (*unk4)(s32);
    s32 unk8;
} Node8004845C;

void func_8004845C(Node8004845C **arg0) {
    Node8004845C *s0 = *arg0;
    if (s0 != NULL) {
loop:
        if ((u32)((D_80124830 - D_80124904) - 0x5B8) < 0x14181U) {
            s0->unk4(s0->unk8);
            s0 = s0->unk0;
            if (s0 != NULL) {
                goto loop;
            }
        }
    }
}

extern s16 D_80112132;
extern s16 D_80112134;

void func_800484F0(void) {
    D_80112132 = func_80042D58(0x8000);
    D_80112134 = func_80042D58(0x8000);
}

extern s32 D_80123754;
extern s32 D_801121C0;
extern s32 D_801121BC;

void func_80048524(s32 arg0) {
    D_80123754 = 0;
    if (arg0 == 0) {
        D_801121BC = D_801121C0 = func_80043040(D_80112132);
    } else {
        D_801121BC = D_801121C0 = func_80043040(D_80112134);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80048594.s")

extern void osWritebackDCache(void *, s32);

void func_800485E8(s32 arg0) {
    if (arg0 == 0) {
        osWritebackDCache((void *)func_80043040(D_80112132), D_80123754);
    } else {
        osWritebackDCache((void *)func_80043040(D_80112134), D_80123754);
    }
}

extern void *func_80048594(s32);

typedef struct {
    s32 w[16];
} Block40;

void *func_8004864C(Block40 *arg0) {
    Block40 *p = func_80048594(0x40);
    if (p == NULL) {
        return NULL;
    }
    *p = *arg0;
    return p;
}

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_800486BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_8004885C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80048A38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80048BBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80048C90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80048CF8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80048D60.s")

void func_80048E3C(void *arg0, void *arg1, void *arg2) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80048E4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/464E0/func_80048E60.s")
