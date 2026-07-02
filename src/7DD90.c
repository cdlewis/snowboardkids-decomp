#include "common.h"

extern s32 D_80121B98;
extern s32 D_80121B94;
extern s32 D_80121B90;
extern s16 D_8011215C[];
extern s16 D_80112166;

typedef struct Struct7DD90 {
    u16 unk0;
    char pad2[0x450];
    s16 unk452;
    s32 unk454;
    s32 unk458;
    s16 unk45C;
    s16 unk45E;
    char pad460[6];
    s16 unk466;
} Struct7DD90;

typedef struct Anim81508 {
    char pad0[0x10];
    s16 unk10;
    s16 unk12;
} Anim81508;

typedef struct Coord81508 {
    s16 x;
    s16 y;
    s16 z;
} Coord81508;

extern s32 func_80043040(s16);
extern void func_80081EF4(Struct7DD90 *);
extern void func_80082070(Struct7DD90 *);

void func_8007D190(void) {
    s32 ptr;
    s32 v1;

    ptr = func_80043040(D_80112166);
    D_80121B90 = ptr + 2;
    v1 = D_80121B90 + *(u16 *)ptr * 6;
    D_80121B94 = v1 + 2;
    v1 = D_80121B94 + *(u16 *)v1 * 8;
    D_80121B98 = v1 + 2;
}

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_8007D200.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_8007D548.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_8007D87C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_8007DC38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_8007ECF4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_8007FD88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_8007FF88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80080CC4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80081124.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_800813F8.s")

void func_80081508(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s16 *arg4) {
    s32 *temp_v1;
    s32 temp_v0;

    temp_v0 = arg0 * 0x1C;
    temp_v1 = &D_80121B90;
    *arg1 = ((Coord81508 *)(*temp_v1 + (((Anim81508 *)(D_80121B98 + temp_v0))->unk10 * 6)))->x << 0x11;
    *arg2 = ((Coord81508 *)(*temp_v1 + (((Anim81508 *)(D_80121B98 + temp_v0))->unk10 * 6)))->y << 0x11;
    *arg3 = ((Coord81508 *)(*temp_v1 + (((Anim81508 *)(D_80121B98 + temp_v0))->unk10 * 6)))->z << 0x11;
    *arg4 = -((Anim81508 *)(D_80121B98 + temp_v0))->unk12;
}

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_800815D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80081C44.s")

s16 func_80081E1C(s32 arg0) {
    int idx = arg0 * 0x1C;

    return *(s16 *)((D_80121B98 + idx) + 2);
}

void func_80081E40(Struct7DD90 *arg0, s32 arg1) {
    s16 *temp_a1;
    s16 temp_v1;
    s32 temp_v1_2;
    s16 *sp1C;
    s32 temp_v0;
    Struct7DD90 *temp_a2 = arg0;

    temp_v0 = func_80043040(D_8011215C[arg0->unk0]);
    temp_a1 = (s16 *)(temp_v0 + (((u16 *)temp_v0)[arg1] * 2));
    temp_v1 = *temp_a1;
    temp_a2->unk45E = temp_v1;
    temp_a2->unk45E++;
    temp_a2->unk45C = temp_v1;
    temp_a1 += 1;
    sp1C = temp_a1;
    temp_v1_2 = (s32)temp_a1 - func_80043040(D_8011215C[temp_a2->unk0]);
    temp_a2->unk454 = temp_v1_2;
    temp_a2->unk458 = temp_v1_2;
    temp_a2->unk466 = 0;
    temp_a2->unk452 = arg1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80081EF4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80082070.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80082184.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80082664.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80082B58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80082DD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80082E48.s")

s32 func_80082EC0(Struct7DD90 *arg0) {
    s32 temp_v1;
    s32 var_t8;
    s16 temp_v0;

    temp_v1 = arg0->unk466;
    var_t8 = temp_v1 - 1;
    if (temp_v1 == 0) {
        temp_v0 = arg0->unk45E;
        if (temp_v0 != 1) {
            arg0->unk45E = temp_v0 - 1;
            func_80081EF4(arg0);
            if (arg0->unk45E == 1) {
                return 1;
            }
            temp_v1 = arg0->unk466;
            var_t8 = temp_v1 - 1;
        } else {
            return 1;
        }
    }
    arg0->unk466 = var_t8;
    return 0;
}

s32 func_80082F44(Struct7DD90 *arg0) {
    s32 temp_v1;
    s32 var_t8;
    s16 temp_v0;

    temp_v1 = arg0->unk466;
    var_t8 = temp_v1 - 1;
    if (temp_v1 == 0) {
        temp_v0 = arg0->unk45E;
        if (temp_v0 != 1) {
            arg0->unk45E = temp_v0 - 1;
            func_80082070(arg0);
            if (arg0->unk45E == 1) {
                return 1;
            }
            temp_v1 = arg0->unk466;
            var_t8 = temp_v1 - 1;
        } else {
            return 1;
        }
    }
    arg0->unk466 = var_t8;
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/7DD90/func_80082FC8.s")
