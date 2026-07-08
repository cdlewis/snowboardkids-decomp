#include "common.h"

typedef struct {
    /* 0x0 */ s8 minutes;
    /* 0x1 */ s8 seconds;
    /* 0x2 */ s16 fraction;
} RaceTimer;

typedef struct {
    /* 0x00 */ char pad[0x14];
} CourseDataStride;

typedef struct {
    /* 0x00 */ char pad[0x4E];
    /* 0x4E */ s8 bestLapMinutes;
    /* 0x4F */ s8 bestLapSeconds;
    /* 0x50 */ s16 bestLapFraction;
} CourseBestLapView;

typedef struct {
    /* 0x00 */ char pad[0x38];
    /* 0x38 */ s16 hudFontHandle;
    /* 0x3A */ char pad3A[4];
    /* 0x3E */ s16 popupFontHandle;
} RaceTimerUiAssetHandles;

typedef struct {
    /* 0x000 */ char pad[0x512];
    /* 0x512 */ s8 unk512;
    /* 0x513 */ s8 unk513;
    /* 0x514 */ s8 unk514;
    /* 0x515 */ s8 unk515;
    /* 0x516 */ s8 unk516;
    /* 0x517 */ char pad517[0x51];
    /* 0x568 */ s32 unk568;
} RaceTimerUiState;

extern s32 func_80043040(s16);
extern void func_80045A78(s32, s32, s32, s32);
extern void func_80046D68(s16, s16, s32, s32, s32);
extern void func_80047174(s32, s32, s32, s32, s32);
extern void func_80047E88(s32, s16, s32, s32);
extern void func_80048278(s32, s32, char *, s32);
extern int sprintf(char *, const char *, ...);
extern CourseDataStride D_800EC9F0[];
extern RaceTimerUiAssetHandles D_80112130;
extern s16 D_8011216E;
extern s16 D_80121B52;
extern s16 D_80121B72;
extern s16 D_801222F6;
extern s16 D_80121B50;
extern u8 D_80121B56;
extern u8 D_80156608;
extern u8 D_800DC8F0[];
extern u8 D_800DC8F8[];
extern RaceTimer D_80121B74;
extern RaceTimer D_80121B78;
extern RaceTimerUiState D_80121D80;
extern s8 D_80122288[];

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80078430.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80078568.s")

const char D_800E1730[] = "Point";
const char D_800E1738[] = "Time Limit";

void func_80078974(s32 arg0) {
    func_80048278(0x60, -0x61, (char *)D_800E1730, 5);
    func_80048278(0x38, 0x47, (char *)D_800E1738, 5);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_800789C0.s")

const char D_800E1760[] = "Time Limit";
const char D_800E176C[] = "/%d";

void func_80078D3C(s32 arg0) {
    char sp18[0x20];

    func_80048278(0x38, 0x47, (char *)D_800E1760, 5);
    sprintf(sp18, D_800E176C, D_801222F6);
    func_80048278(0x70, -0x48, sp18, 6);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80078D9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80079068.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80079154.s")

const char D_800E17D8[] = "Lap Time";
const char D_800E17E4[] = "Best Lap";
const char D_800E17F0[] = "%2.2d'%2.2d\"%2.2d";

void func_80079394(s32 arg0) {
    char sp28[0x20];
    CourseBestLapView *course;

    func_80048278(0x48, 0x47, (char *)D_800E17D8, 5);
    func_80048278(0x48, -0x61, (char *)D_800E17E4, 7);
    course = (CourseBestLapView *)&D_800EC9F0[D_80121B50];
    sprintf(sp28, D_800E17F0, course->bestLapMinutes, course->bestLapSeconds, course->bestLapFraction >> 8);
    func_80048278(0x48, -0x58, sp28, 7);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80079438.s")

void func_80079750(s32 arg0) {

}

// func_80079758 best match: 94.630% at nonmatchings/func_80079758-5635509610426229442/base_8.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80079758.s")

#ifdef NON_MATCHING
const char D_800E180C[] = "%5ld";

void func_80079758(s32 arg0) {
    volatile u8 padding[0x18];
    char sp51;
    char sp50;
    char sp4F;
    char sp4E;
    char sp4D;
    char sp4C;
    char *digit;
    s32 x;
    s32 palette;

    sprintf(&sp4C, D_800E180C, D_80121D80.unk568);

    x = 0x50;
    digit = &sp4C;
    if (D_80121D80.unk568 < 100) {
        palette = 0x10;
    } else {
        palette = 0xE;
    }

    do {
        if (*digit != ' ') {
            func_80046D68((s16)x, 0x50, func_80043040(D_80112130.popupFontHandle), (*digit - 5) & 0xFFFF,
                          palette & 0xFFFF);
        }
        digit++;
        x += 8;
    } while (digit != &sp51);

    func_80045A78(0x78, 0x50, func_80043040(D_80112130.hudFontHandle), ((D_80121B72 >> 1) + 4) & 0xFFFF);

    if (D_80121D80.unk513 != 0) {
        func_80047174(-0x20, -0x60, func_80043040(D_80112130.popupFontHandle),
                      (D_80121D80.unk514 + D_800DC8F0[D_80121D80.unk512] - 1) & 0xFFFF, D_80121D80.unk513);
    } else {
        func_80045A78(-0x20, -0x60, func_80043040(D_80112130.popupFontHandle),
                      (D_80121D80.unk514 + D_800DC8F0[D_80121D80.unk512] - 1) & 0xFFFF);
    }

    if (D_80121D80.unk516 != 0) {
        func_80047174(0, -0x60, func_80043040(D_80112130.popupFontHandle), D_800DC8F8[D_80121D80.unk515],
                      D_80121D80.unk516);
    } else {
        func_80045A78(0, -0x60, func_80043040(D_80112130.popupFontHandle), D_800DC8F8[D_80121D80.unk515]);
    }

    func_80045A78(-0x88, 0x40, func_80043040(D_80112130.popupFontHandle), 0x24);
    func_80045A78(-0x88, 0x40, func_80043040(D_80112130.popupFontHandle), 0x29);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_800799DC.s")

void func_80079E48(s32 arg0) {
    int new_var2;
    int new_var;
    s32 sp1C;
    s32 temp_v0;
    s32 var_v1;

    temp_v0 = D_80156608;
    if (0 == temp_v0) {
        var_v1 = -0x30;
    } else {
        var_v1 = 0x2A;
    } sp1C = (s16)var_v1; new_var = 0xFF; func_80047E88(0x70, (s16)var_v1, (D_80122288[new_var2 = temp_v0 * 0x60C] + 0x31) & new_var, 2); func_80047E88(0x78, *(s16 *)((u8 *)&sp1C + 2), 0x2F, 2);
    func_80047E88(0x80, *(s16 *)((u8 *)&sp1C + 2), (D_80121B52 + 0x30) & new_var, 2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80079F04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_8007A108.s")

void func_8007A278(s32 arg0) {
    func_80045A78(0xC, 0x2C, func_80043040(D_8011216E), 0x90);
}

void func_8007A2B8(void) {
    if (D_80121B56 == 0) {
        D_80121B74.fraction = D_80121B74.fraction + 0x358;
        if (D_80121B74.fraction >= 0x6401) {
            D_80121B74.fraction = 0;
            D_80121B74.seconds = D_80121B74.seconds + 1;
            if (D_80121B74.seconds >= 0x3C) {
                D_80121B74.seconds = 0;
                D_80121B74.minutes = D_80121B74.minutes + 1;
                if (D_80121B74.minutes >= 0x63) {
                    D_80121B74.fraction = 0x6300;
                    D_80121B74.seconds = 0x3B;
                    D_80121B74.minutes = 0x63;
                }
            }
        }
    }
}

void func_8007A350(void) {
    if (D_80121B56 == 0) {
        D_80121B78.fraction = D_80121B78.fraction - 0x358;
        if (D_80121B78.fraction < 0) {
            D_80121B78.fraction = 0x60F8;
            D_80121B78.seconds = D_80121B78.seconds - 1;
            if (D_80121B78.seconds < 0) {
                D_80121B78.seconds = 0x3B;
                D_80121B78.minutes = D_80121B78.minutes - 1;
                if (D_80121B78.minutes < 0) {
                    D_80121B78.fraction = 0;
                    D_80121B78.seconds = 0;
                    D_80121B78.minutes = 0;
                }
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_8007A3D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_8007A8EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_8007AA50.s")

s32 func_8007B130(RaceTimer *arg0, RaceTimer *arg1, RaceTimer *arg2) {
    s32 total1;
    s32 diff;

    diff = (arg0->fraction & 0xFF00) + (arg0->seconds * 0x6400) + (arg0->minutes * 0x177000);
    total1 = (arg1->fraction & 0xFF00) + (arg1->seconds * 0x6400) + (arg1->minutes * 0x177000);
    arg0 = (RaceTimer *)1;
    if (diff >= total1) {
        arg0 = (RaceTimer *)0;
        diff -= total1;
    } else {
        diff = total1 - diff;
    }
    arg2->fraction = diff % 0x6400;
    diff /= 0x6400;
    arg1 = (RaceTimer *)60;
    arg2->seconds = diff % (s32)arg1;
    diff /= (s32)arg1;
    arg2->minutes = diff % 99;
    return (s32)arg0;
}
