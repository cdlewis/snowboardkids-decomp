#include "common.h"

extern s32 func_80011D74(void *, s32, s16, s16);
extern void func_8000F8AC(s16, s16, s32, s32, s32, s32, s32, s32, s32);
extern s32 func_80043040(s16);
extern void func_80017168(void *, s32);
extern void func_80071824(void *, void *);
extern void func_80014AA4(void);
extern void func_80014EF0(void);
extern void func_800152D0(void);
extern void func_80015680(void);
extern void func_80015A30(void);
extern void func_80015BD8(void *);
extern void func_80015F4C(void);
extern void func_80016284(void);
extern void func_80016E40(void);
extern void func_800170AC(void *);
extern void func_800483FC(void *, void *, s32);
extern void func_800716E4(void *);
extern s16 D_8010AE38;
extern s16 D_8010AE3A;
extern s16 D_8010AE3C;
extern s16 D_8010AE3E;
extern s16 D_8010AE40;
extern s16 D_8010AE42;
extern s16 D_8010AE44;
extern s16 D_8010AE46;
extern s16 D_80112172;
extern s16 D_8011217C;
extern u8 D_80121B55;
extern s32 D_801235B4;
extern void *D_801235B8;
extern s32 D_80124838;
extern s32 D_80124868;

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80014600.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80014AA4.s")

void func_80014C7C(void *arg0) {
    *(s16 *)((s32)arg0 + 0x18) = -0x70;
    *(s16 *)((s32)arg0 + 0x1A) = -0x1C;
    *(s16 *)((s32)arg0 + 0x1C) = 0;
    *(s8 *)((s32)arg0 + 0x1E) = 0;
    func_80071824(arg0, func_80014AA4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80014CB8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80014EF0.s")

void func_80015054(void *arg0) {
    *(s16 *)((char *)arg0 + 0x18) = 0x90;
    *(s16 *)((char *)arg0 + 0x1A) = -0x18;
    *(s8 *)((char *)arg0 + 0x1C) = 0;
    func_80071824(arg0, func_80014EF0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_8001508C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_800152D0.s")

void func_80015404(void *arg0) {
    *(s16 *)((char *)arg0 + 0x18) = 0x90;
    *(s16 *)((char *)arg0 + 0x1A) = 8;
    *(s8 *)((char *)arg0 + 0x1C) = 0;
    func_80071824(arg0, func_800152D0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_8001543C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80015680.s")

void func_800157B4(void *arg0) {
    *(s16 *)((char *)arg0 + 0x18) = 0x90;
    *(s16 *)((char *)arg0 + 0x1A) = 0x28;
    *(s8 *)((char *)arg0 + 0x1C) = 0;
    func_80071824(arg0, func_80015680);
}

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_800157EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80015A30.s")

void func_80015B20(void *arg0) {
    *(s16 *)((char *)arg0 + 0x18) = 0x90;
    *(s16 *)((char *)arg0 + 0x1A) = 0x48;
    *(s8 *)((char *)arg0 + 0x1C) = 0;
    func_80071824(arg0, func_80015A30);
}

void func_80015B58(void *arg0) {
    func_8000F8AC(
        *(s16 *)((u8 *)arg0 + 0x18),
        (s16)((*(s16 *)((u8 *)arg0 + 0x1A) + (D_80121B55 << 5)) - 0x20),
        func_80043040(D_80112172),
        4,
        0x20,
        0x20,
        0,
        *(u16 *)((u8 *)arg0 + 0x1C),
        0);
}

void func_80015BD8(void *arg0) {
    u16 temp_v0;
    s16 temp_t6;
    void *temp_a2;

    temp_v0 = *(u16 *)((u8 *)arg0 + 0x1C);
    temp_a2 = arg0;
    if ((((((((((temp_v0 & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) != 0x100) {
        temp_t6 = temp_v0 + 0x28;
        *(s16 *)((u8 *)arg0 + 0x1C) = temp_t6;
        if ((temp_t6 & 0xFFFF) >= 0x100) {
            *(s16 *)((u8 *)arg0 + 0x1C) = 0x100;
            *(s32 *)((u8 *)D_801235B8 + 0x1C) = 1;
        }
    } else if (D_801235B4 != 0) {
        *(s16 *)((u8 *)temp_a2 + 0x18) = *(s16 *)((u8 *)temp_a2 + 0x18) - 0x20;
    }
    if (*(s16 *)((u8 *)temp_a2 + 0x18) < -0x90) {
        func_800716E4(temp_a2);
    } else {
        func_800483FC(&D_80124868, func_80015B58, (s32)temp_a2);
    }
}

void func_80015C84(void *arg0) {
    *(s16 *)((char *)arg0 + 0x18) = -0x68;
    *(s16 *)((char *)arg0 + 0x1A) = -0x14;
    *(s16 *)((char *)arg0 + 0x1C) = 0;
    func_80071824(arg0, func_80015BD8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80015CBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80015F4C.s")

void func_8001621C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = -0x114;
    *(s16 *)((u8 *)arg0 + 0x20) = -0xA4;
    *(s16 *)((u8 *)arg0 + 0x1A) = -0x114;
    *(s16 *)((u8 *)arg0 + 0x22) = 0x48;
    *(s16 *)((u8 *)arg0 + 0x1C) = 0x90;
    *(s16 *)((u8 *)arg0 + 0x24) = -0xA4;
    *(s16 *)((u8 *)arg0 + 0x1E) = 0x90;
    *(s16 *)((u8 *)arg0 + 0x26) = 0x48;
    *(s16 *)((u8 *)arg0 + 0x28) = 0x8C;
    *(s16 *)((u8 *)arg0 + 0x2A) = 0x44;
    *(u8 *)((u8 *)arg0 + 0x30) = 0;
    func_80071824(arg0, func_80015F4C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80016284.s")

void func_80016560(void *arg0) {
    void *temp_a2;

    *(s16 *)((u8 *)arg0 + 0x18) = D_8010AE38;
    *(s16 *)((u8 *)arg0 + 0x20) = D_8010AE40;
    *(s16 *)((u8 *)arg0 + 0x1A) = D_8010AE3A;
    *(s16 *)((u8 *)arg0 + 0x22) = D_8010AE42;
    *(s16 *)((u8 *)arg0 + 0x1C) = D_8010AE3C;
    *(s16 *)((u8 *)arg0 + 0x24) = D_8010AE44;
    *(s16 *)((u8 *)arg0 + 0x1E) = D_8010AE3E;
    temp_a2 = arg0;
    *(s16 *)((u8 *)arg0 + 0x26) = D_8010AE46;
    func_800483FC(&D_80124868, func_80016284, (s32)temp_a2);
}

void func_800165F0(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = -0x114;
    *(s16 *)((u8 *)arg0 + 0x20) = -0xA4;
    *(s16 *)((u8 *)arg0 + 0x1A) = -0x114;
    *(s16 *)((u8 *)arg0 + 0x22) = 0x48;
    *(s16 *)((u8 *)arg0 + 0x1C) = 0x90;
    *(s16 *)((u8 *)arg0 + 0x24) = -0xA4;
    *(s16 *)((u8 *)arg0 + 0x1E) = 0x90;
    *(s16 *)((u8 *)arg0 + 0x26) = 0x48;
    *(s16 *)((u8 *)arg0 + 0x28) = 4;
    *(s16 *)((u8 *)arg0 + 0x2A) = 0x11;
    *(s16 *)((u8 *)arg0 + 0x2C) = 2;
    *(s16 *)((u8 *)arg0 + 0x2E) = 0x27;
    func_80071824(arg0, func_80016560);
}

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80016664.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80016948.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80016B54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80016BE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80016E40.s")

void func_80017014(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = -0x70;
    *(s16 *)((u8 *)arg0 + 0x20) = -0x34;
    *(s16 *)((u8 *)arg0 + 0x28) = -0x34;
    *(s16 *)((u8 *)arg0 + 0x1A) = -0x70;
    *(s16 *)((u8 *)arg0 + 0x22) = 0x30;
    *(s16 *)((u8 *)arg0 + 0x2A) = 0x30;
    *(s16 *)((u8 *)arg0 + 0x1C) = 0x1C;
    *(s16 *)((u8 *)arg0 + 0x24) = -0x34;
    *(s16 *)((u8 *)arg0 + 0x2C) = -0x34;
    *(s16 *)((u8 *)arg0 + 0x1E) = 0x1C;
    *(s16 *)((u8 *)arg0 + 0x26) = 0x30;
    *(s16 *)((u8 *)arg0 + 0x2E) = 0x30;
    func_80071824(arg0, func_80016E40);
}

void func_80017078(void *arg0) {
    func_80011D74((void *)((s32)arg0 + 0x1C), 0, *(s16 *)((s32)arg0 + 0x18), *(s16 *)((s32)arg0 + 0x1A));
}

void func_800170AC(void *arg0) {
    void *temp_a2;
    s16 *temp_v0;

    temp_a2 = arg0;
    (*(s16 *)((u8 *)arg0 + 0x1C))--;
    temp_v0 = (s16 *)((u8 *)temp_a2 + 0x1C);
    if (*(s16 *)((u8 *)arg0 + 0x1C) < 0) {
        *(s16 *)((u8 *)arg0 + 0x1C) = 0x2FF;
    }
    temp_v0[1] = (temp_v0[1] + 1) & 0x1FF;
    func_800483FC(&D_80124838, func_80017078, (s32)temp_a2);
}

void func_8001710C(void *arg0) {
    void *temp_a2 = arg0;

    func_80017168((u8 *)temp_a2 + 0x1C, func_80043040(D_8011217C));
    *(s16 *)((u8 *)temp_a2 + 0x18) = *(s16 *)((u8 *)temp_a2 + 0x24);
    *(s16 *)((u8 *)temp_a2 + 0x1A) = *(s16 *)((u8 *)temp_a2 + 0x26);
    func_80071824(temp_a2, func_800170AC);
}

#pragma GLOBAL_ASM("asm/nonmatchings/15200/func_80017168.s")

void n_alSynFreeFX(s32 arg0) {

}
