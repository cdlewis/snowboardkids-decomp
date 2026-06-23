#include "common.h"

extern s32 func_80043040(s16);
extern s32 func_800430D0(void);
extern void func_80045990(s32, s32, void *, void *);
extern void func_80071824(void *, void *);
extern void func_80097C18(void *, s16);
extern void func_80098590(void *, void *, void *);
extern char D_800D9BD8[];
extern s16 D_80112168;
extern void func_80069678(void);

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80066760.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_800668EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_800669A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80066ABC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80066E10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80067034.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_8006713C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_800671F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80067364.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_800674B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_8006752C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_800675AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80067830.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_800681A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_800684E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80068BF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80068CD4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80068DB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80068EA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_800691C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_8006935C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80069678.s")

void func_80069754(void *arg0) {
    char sp28[0x20];

    *(s16 *)((char *)arg0 + 0x3C) = 0xA;
    *(s16 *)((char *)arg0 + 0x36) = func_800430D0() - 0x80;
    *(s16 *)((char *)arg0 + 0x38) = func_800430D0() - 0x80;
    *(s16 *)((char *)arg0 + 0x3A) = func_800430D0() - 0x80;
    func_80097C18(sp28, *(s16 *)((char *)arg0 + 0x32));
    func_80098590(sp28, (char *)&D_800D9BD8 + (*(u16 *)((char *)arg0 + 0x10) * 0xC), (char *)arg0 + 0x24);
    func_80045990(func_80043040(D_80112168), 0x22, (char *)arg0 + 0x4C, (char *)arg0 + 0x48);
    func_80071824(arg0, func_80069678);
}

#pragma GLOBAL_ASM("asm/nonmatchings/67360/func_80069808.s")
