#include "common.h"

extern void func_80071824(void *, void *);
extern void func_80018C80(void);
extern void func_800177F8(void);
extern void func_80017C34(void);
extern void func_80018AA0(void);
extern u8 D_80121B55;
extern u8 D_80121D80[];
extern u8 D_80112130[];
extern s32 func_80043040(s16);
extern void func_8000F030(s16, s16, s32, s32, s32, s32, s32, s32);

#pragma GLOBAL_ASM("asm/nonmatchings/18610/func_800171F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/18610/func_800177F8.s")

void func_800179D4(void *arg0) {
    *(s16 *)((s32)arg0 + 0x18) = -0x70;
    *(s16 *)((s32)arg0 + 0x1A) = -0x1C;
    *(s16 *)((s32)arg0 + 0x1C) = 0;
    *(s8 *)((s32)arg0 + 0x20) = 0;
    func_80071824(arg0, func_800177F8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/18610/func_80017A10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/18610/func_80017C34.s")

void func_80017D08(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = -0x88;
    *(s16 *)((u8 *)arg0 + 0x20) = -0x60;
    *(s16 *)((u8 *)arg0 + 0x1A) = -0x88;
    *(s16 *)((u8 *)arg0 + 0x22) = 4;
    *(s16 *)((u8 *)arg0 + 0x1C) = 4;
    *(s16 *)((u8 *)arg0 + 0x24) = -0x60;
    *(s16 *)((u8 *)arg0 + 0x1E) = 4;
    *(s16 *)((u8 *)arg0 + 0x26) = 4;
    *(s16 *)((u8 *)arg0 + 0x28) = 0x8C;
    *(s16 *)((u8 *)arg0 + 0x2A) = 0x44;
    *(s8 *)((u8 *)arg0 + 0x30) = 0;
    func_80071824(arg0, func_80017C34);
}

#pragma GLOBAL_ASM("asm/nonmatchings/18610/func_80017D6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/18610/func_80017F94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/18610/func_80018060.s")

#pragma GLOBAL_ASM("asm/nonmatchings/18610/func_80018134.s")

#pragma GLOBAL_ASM("asm/nonmatchings/18610/func_800182A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/18610/func_800183DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/18610/func_800184C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/18610/func_80018AA0.s")

void func_80018B6C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = -0x88;
    *(s16 *)((u8 *)arg0 + 0x20) = -0x60;
    *(s16 *)((u8 *)arg0 + 0x1A) = -0x88;
    *(s16 *)((u8 *)arg0 + 0x22) = 0x24;
    *(s16 *)((u8 *)arg0 + 0x1C) = 4;
    *(s16 *)((u8 *)arg0 + 0x24) = -0x60;
    *(s16 *)((u8 *)arg0 + 0x1E) = 4;
    *(s16 *)((u8 *)arg0 + 0x26) = 0x24;
    func_80071824(arg0, func_80018AA0);
}

void func_80018BC0(void *arg0) {
    u8 *base;
    s32 i;
    u8 *player;
    u8 *temp_s0;
    int new_var;
    s32 color;
    u16 temp_v1;
 base = arg0; i = 0; if (D_80121B55 > 0) { player = D_80121D80; do {
            if (player[8] != 0) {
                new_var = i * 2;
                temp_s0 = base + new_var;
                color = func_80043040(*(s16 *)&D_80112130[0x42]);
                temp_v1 = *(u16 *)(temp_s0 + 0x40);
                func_8000F030(*(s16 *)(temp_s0 + 0x18), *(s16 *)(temp_s0 + 0x20), color, 0xD, temp_v1, temp_v1, 0, 0);
            }
            i++;
            player += 0x60C;
        } while (i < D_80121B55);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/18610/func_80018C80.s")

void func_800191A0(void *arg0) {
    func_80071824(arg0, func_80018C80);
}
