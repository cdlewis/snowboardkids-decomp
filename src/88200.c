#include "common.h"

typedef struct {
    char pad0[0x13];
    s8 unk13;
    char pad14[0x554];
    s32 unk568;
    s32 unk56C;
} Struct8008BB5C;

extern void func_8008B73C(s32, s32, s32, s32, s32, s32);
extern void func_80097FE4(void *, s16, s16, void *);
extern void func_80098590(void *, s32 *, s32 *);

#pragma GLOBAL_ASM("asm/nonmatchings/88200/func_80087600.s")

#pragma GLOBAL_ASM("asm/nonmatchings/88200/func_80087AFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/88200/func_80087E14.s")

#pragma GLOBAL_ASM("asm/nonmatchings/88200/func_80087EFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/88200/func_80088294.s")

#pragma GLOBAL_ASM("asm/nonmatchings/88200/func_80088664.s")

#pragma GLOBAL_ASM("asm/nonmatchings/88200/func_80088A1C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/88200/func_80088C80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/88200/func_80088E98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/88200/func_80089000.s")

#pragma GLOBAL_ASM("asm/nonmatchings/88200/func_800891B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/88200/func_80089374.s")

#pragma GLOBAL_ASM("asm/nonmatchings/88200/func_8008A940.s")

#pragma GLOBAL_ASM("asm/nonmatchings/88200/func_8008B408.s")

#pragma GLOBAL_ASM("asm/nonmatchings/88200/func_8008B508.s")

#pragma GLOBAL_ASM("asm/nonmatchings/88200/func_8008B60C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/88200/func_8008B73C.s")

void func_8008BB20(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_8008B73C(arg0, arg1, 0, arg2, arg3, arg4);
}

void func_8008BB5C(Struct8008BB5C *arg0, s32 arg1) {
    if (arg0->unk13 != 0) {
        arg0->unk568 += arg1;
        if (arg0->unk568 >= 0x186A0) {
            arg0->unk568 = 0x1869F;
        }
        arg0->unk56C += arg1;
        if (arg0->unk56C >= 0x186A0) {
            arg0->unk56C = 0x1869F;
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/88200/func_8008BBB8.s")

void func_8008BE1C(void *arg0) {
    s32 sp30[8];
    s32 sp24[3];
    s32 sp18[3];

    func_80097FE4(sp30, *(s16 *)((u8 *)arg0 + 0x2EA), *(s16 *)((u8 *)arg0 + 0x2EC), arg0);
    sp24[0] = 0;
    sp24[1] = 0xC0000;
    sp24[2] = 0;
    func_80098590(sp30, sp24, sp18);
    *(s32 *)((u8 *)arg0 + 0x290) = sp18[0] + *(s32 *)((u8 *)arg0 + 0x1C);
    *(s32 *)((u8 *)arg0 + 0x294) = sp18[1] + *(s32 *)((u8 *)arg0 + 0x20);
    *(s32 *)((u8 *)arg0 + 0x298) = sp18[2] + *(s32 *)((u8 *)arg0 + 0x24);
}
