#include "common.h"
#include "asset_decompression.h"
#include "memory_allocator.h"
#include "race_item_effects.h"
#include "race_player_movement.h"
#include "race_timer_ui.h"
#include "race_ui_effects.h"

typedef struct {
    /* 0x00 */ char pad[0x14];
} CourseDataStride;

typedef struct {
    /* 0x00 */ s8 x;
    /* 0x01 */ s8 y;
    /* 0x02 */ s16 z;
} RaceUiCoursePosition;

typedef struct {
    /* 0x00 */ char pad[0x4E];
    /* 0x4E */ s8 bestLapMinutes;
    /* 0x4F */ s8 bestLapSeconds;
    /* 0x50 */ s16 bestLapFraction;
} CourseBestLapView;

typedef struct {
    /* 0x00 */ s16 pathIndex;
    /* 0x02 */ u8 pad2[0x3C - 0x02];
    /* 0x3C */ s16 finishLinePathIndex;
    /* 0x3E */ u8 pad3E[0x48 - 0x3E];
} RaceTimerCourseSpawnEntry;

typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ s16 mainFontHandle;
    /* 0x3A */ u8 pad3A[0x3E - 0x3A];
    /* 0x3E */ s16 popupFontHandle;
} RaceTimerUiAssetHandles;

typedef struct {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ u8 iconGroup;
    /* 0x011 */ u8 pad11[0x2D8 - 0x11];
    /* 0x2D8 */ s16 unk2D8;
    /* 0x2DA */ u8 pad2DA[0x2FC - 0x2DA];
    /* 0x2FC */ s32 unk2FC;
    /* 0x300 */ u8 pad300[0x508 - 0x300];
    /* 0x508 */ s8 lapDigit;
    /* 0x509 */ s8 iconTile;
    /* 0x50A */ u8 pad50A[0x512 - 0x50A];
    /* 0x512 */ s8 tensDigitTile;
    /* 0x513 */ s8 tensDigitPalette;
    /* 0x514 */ s8 tensDigitOffset;
    /* 0x515 */ s8 onesDigitTile;
    /* 0x516 */ s8 onesDigitPalette;
    /* 0x517 */ u8 pad517[0x568 - 0x517];
    /* 0x568 */ s32 timerValue;
    /* 0x56C */ u8 pad56C[0x570 - 0x56C];
    /* 0x570 */ s16 score;
    /* 0x572 */ s16 targetScore;
    /* 0x574 */ u8 pad574[0x57E - 0x574];
    /* 0x57E */ s16 raceProgress;
    /* 0x580 */ s8 flashFrame;
    /* 0x581 */ u8 pad581[0x60C - 0x581];
} RaceTimerUiPlayer;

typedef struct {
    /* 0x000 */ s8 value;
    /* 0x001 */ u8 pad1[0x60C - 0x001];
} RaceTimerUiS8Stride;

typedef struct {
    /* 0x000 */ s32 value;
    /* 0x004 */ u8 pad4[0x60C - 0x004];
} RaceTimerUiS32Stride;

extern void func_80045A78(s32, s32, s32, s32);
extern void func_80046D68(s16, s16, s32, s32, s32);
extern void func_80047174(s32, s32, s32, s32, s32);
extern void func_80047E88(s32, s16, s32, s32);
extern void func_80048278(s32, s32, char *, s32);
extern void func_800483FC(void *, void *, s32);
extern void func_80071408(void *, s32, s32);
extern int sprintf(char *, const char *, ...);
extern u8 D_245A80[];
extern u8 D_24C8E0[];
extern RaceTimer D_800DC928[];
extern RaceTimer D_800DC950;
extern u8 D_800DC8F9;
extern u8 D_800EC9C2;
extern RaceTimerCourseSpawnEntry D_800B9540[];
extern u8 D_800DC8F0[];
extern u8 D_800DC8F8[];
extern u16 D_800DC954[];
extern u16 D_800DC99C[];
extern RaceUiCoursePosition D_800DC900[];
extern CourseDataStride D_800EC9F0[];
extern RaceTimerUiAssetHandles D_80112130;
extern s16 D_8011216E;
extern s16 D_80121B52;
extern s16 D_80121B72;
extern s16 D_80121B70;
extern s16 D_80122040;
extern s8 D_80122043;
extern s16 D_801222F6;
extern s16 D_801222F4;
extern s16 D_80121B50;
extern u8 D_80121B55;
extern u8 D_80121B56;
extern u8 D_80121B5A;
extern u8 D_80121B5B;
extern u8 D_80121B5E;
extern u8 D_80156608;
extern s16 D_80156612;
extern s32 D_80124868;
extern s32 D_80124878;
extern RaceTimer D_80121B74;
extern s8 D_80121B75;
extern s16 D_80121B76;
extern RaceTimer D_80121B78;
extern u8 D_80121B80;
extern u8 D_80121B81;
extern s8 D_80122288[];
extern RaceTimerUiPlayer D_80121D80[];
extern RaceTimerUiS8Stride D_80122289[];
extern RaceTimerUiS8Stride D_80122293[];
extern RaceTimerUiS8Stride D_80122295[];
extern RaceTimerUiS8Stride D_80122296[];
extern RaceTimerUiS32Stride D_801222E8[];
extern s16 D_80122290;
extern s16 D_801222F0;
extern s8 D_80121B79;
extern s16 D_80121B7A;

void func_80078430(void) {
    func_800437F0(D_245A80, D_24C8E0, 0x1F);
    D_80121B70 = -1;
    D_80121B72 = 0;

    if (D_80121B55 == 1) {
        if (D_800EC9C2 == 0) {
            D_80121B70 = 0;
        }
        if (D_800EC9C2 == 2) {
            D_80121B70 = 4;
        }
        if (D_800EC9C2 == 1) {
            if (D_80121B5E == 0) {
                D_80121B70 = 5;
            }
            if (D_80121B5E == 1) {
                D_80121B70 = 6;
                D_80121B78 = D_800DC928[D_80121B50];
            }
            if (D_80121B5E == 2) {
                D_80121B70 = 7;
                D_80121B78 = D_800DC950;
            }
        }
    } else {
        D_80121B70 = D_80121B55 - 1;
    }

    if ((D_80121B5A != 0) || (D_80121B5B != 0)) {
        D_80121B70 = 8;
    }

    D_80121B74.minutes = 0;
    D_80121B74.seconds = 0;
    D_80121B74.fraction = 0;
    D_80121B80 = 0;
    D_80121B81 = 0;
}

// func_80078568 best match: 98.869% at nonmatchings/func_80078568-2225551288923588688/base_17.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80078568.s")

#ifdef NON_MATCHING
const char D_800E1710[] = "%2.2d";
const char D_800E1718[] = "%2.2d";
const char D_800E1720[] = "%2.2d";
const char D_800E1728[] = "%4d";
const char D_800E172C[] = "%2d";

void func_80078568(s32 arg0) {
    char buffer[4];
    s32 x;
    char *digit;
    s32 palette;
    s32 finalPalette;
    s32 i;

    palette = 0xC;
    if ((D_80121B78.minutes == 0) && (D_80121B78.seconds < 10) && (D_80156612 & 1)) {
        palette = 0x10;
    }

    {
        char *end;

        sprintf(buffer, D_800E1710, D_80121B78.minutes);
        finalPalette = palette & 0xFFFF;
        x = 0x40;
        digit = buffer;
        end = &buffer[2];
        do {
            func_80046D68((s16)x, 0x50, func_80043040(D_80112130.popupFontHandle),
                          ((u8)*digit - 5) & 0xFFFF, finalPalette);
            digit++;
            x += 8;
        } while ((u32)digit < (u32)end);

        x += 8;
        sprintf(buffer, D_800E1718, D_80121B79);
        digit = buffer;
        end = &buffer[2];
        end += 0;
        do {
            func_80046D68((s16)x, 0x50, func_80043040(D_80112130.popupFontHandle),
                          ((u8)*digit - 5) & 0xFFFF, finalPalette);
            digit++;
            x += 8;
        } while ((u32)digit < (u32)end);

        x += 8;
        sprintf(buffer, D_800E1720, D_80121B7A >> 8);
        digit = buffer;
        end = &buffer[2];
        do {
            func_80046D68((s16)x, 0x50, func_80043040(D_80112130.popupFontHandle),
                          ((u8)*digit - 5) & 0xFFFF, finalPalette);
            digit++;
            x += 8;
        } while ((u32)digit < (u32)end);
    }

    func_80046D68(0x50, 0x50, func_80043040(D_80112130.popupFontHandle), 0x36, finalPalette);
    func_80046D68(0x68, 0x50, func_80043040(D_80112130.popupFontHandle), 0x35, finalPalette);

    if (D_80122043 < 10) {
        x = -0x90;
        i = 0;
        if (D_80122043 > 0) {
            do {
                func_80047174((s16)x, -0x68, func_80043040(D_80112130.popupFontHandle), 0x26, 1);
                i++;
                x += 8;
            } while (i < D_80122043);
        }
    } else {
        func_80045A78(-0x84, -0x60, func_80043040(D_80112130.popupFontHandle), 0x26);
    }

    {
        char *end;

        sprintf(buffer, D_800E1728, D_80122040);
        x = 0x64;
        digit = buffer;
        end = &buffer[4];
        do {
            if ((u8)*digit != ' ') {
                func_80045A78((s16)x, -0x58, func_80043040(D_80112130.popupFontHandle),
                              ((u8)*digit - 5) & 0xFFFF);
            }
            digit++;
            x += 8;
        } while (digit != end);
    }

    func_80045A78(-0x88, 0x40, func_80043040(D_80112130.popupFontHandle), 0x27);
    func_80045A78(-0x88, 0x40, func_80043040(D_80112130.popupFontHandle), 0x28);
    if (D_80121B81 != 0) {
        func_80045A78(0x40, 0x54, func_80043040(D_80112130.popupFontHandle), 0x4F);
    }

    if (D_80122043 >= 10) {
        sprintf(buffer, D_800E172C, D_80122043);
        func_80045A78(-0x64, -0x59, func_80043040(D_80112130.popupFontHandle), ((u8)buffer[0] - 5) & 0xFFFF);
        func_80045A78(-0x5C, -0x59, func_80043040(D_80112130.popupFontHandle), ((u8)buffer[1] - 5) & 0xFFFF);
    }
}
#endif

const char D_800E1730[] = "Point";
const char D_800E1738[] = "Time Limit";

void func_80078974(s32 arg0) {
    func_80048278(0x60, -0x61, (char *)D_800E1730, 5);
    func_80048278(0x38, 0x47, (char *)D_800E1738, 5);
}

// func_800789C0 best match: 92.076% at nonmatchings/func_800789C0-2225551288923588688/base_14.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_800789C0.s")

#ifdef NON_MATCHING
const char D_800E1744[] = "%2.2d";
const char D_800E174C[] = "%2.2d";
const char D_800E1754[] = "%2.2d";
const char D_800E175C[] = "%2d";

void func_800789C0(s32 arg0) {
    char buffer[0xC];
    s32 x;
    char *digit;
    s32 palette;
    s32 i;

    palette = 0xC;
    if ((D_80121B78.minutes == 0) && (D_80121B78.seconds < 10) && (D_80156612 & 1)) {
        palette = 0x10;
    }

    {
        char *end;

        sprintf(buffer, D_800E1744, D_80121B78.minutes);
        x = 0x40;
        digit = buffer;
        end = &buffer[2];
        do {
            func_80046D68((s16)x, 0x50, func_80043040(D_80112130.popupFontHandle), ((u8)*digit - 5) & 0xFFFF,
                          palette & 0xFFFF);
            digit++;
            x += 8;
        } while ((u32)digit < (u32)end);

        x += 8;
        sprintf(buffer, D_800E174C, D_80121B79);
        digit = buffer;
        end = &buffer[2];
        do {
            func_80046D68((s16)x, 0x50, func_80043040(D_80112130.popupFontHandle), ((u8)*digit - 5) & 0xFFFF,
                          palette & 0xFFFF);
            digit++;
            x += 8;
        } while ((u32)digit < (u32)end);

        x += 8;
        sprintf(buffer, D_800E1754, D_80121B7A >> 8);
        digit = buffer;
        end = &buffer[2];
        do {
            func_80046D68((s16)x, 0x50, func_80043040(D_80112130.popupFontHandle), ((u8)*digit - 5) & 0xFFFF,
                          palette & 0xFFFF);
            digit++;
            x += 8;
        } while ((u32)digit < (u32)end);

        func_80046D68(0x50, 0x50, func_80043040(D_80112130.popupFontHandle), 0x36, palette & 0xFFFF);
        func_80046D68(0x68, 0x50, func_80043040(D_80112130.popupFontHandle), 0x35, palette & 0xFFFF);
    }

    func_80045A78(0x68, -0x60, func_80043040(D_80112130.popupFontHandle), 0x20);

    {
        char *end;

        sprintf(buffer, D_800E175C, D_801222F4);
        x = 0;
        digit = buffer;
        end = &buffer[2];
        do {
            if ((u8)*digit != ' ') {
                func_80045A78((s16)(x + 0x60), -0x50, func_80043040(D_80112130.popupFontHandle),
                              ((u8)*digit - 5) & 0xFFFF);
            }
            digit++;
            x += 8;
        } while ((u32)digit < (u32)end);
    }

    x = 0;
    i = 0;
    if (D_80122290 > 0) {
        do {
            func_80045A78((s16)(x - 0x88), -0x60, func_80043040(D_80112130.popupFontHandle), 0x21);
            i++;
            x += 8;
        } while (i < D_80122290);
    }

    func_80045A78(-0x88, 0x40, func_80043040(D_80112130.popupFontHandle), 0x22);
    func_80045A78(-0x88, 0x40, func_80043040(D_80112130.popupFontHandle), 0x23);
    if (D_80121B81 != 0) {
        func_80045A78(0x40, 0x54, func_80043040(D_80112130.popupFontHandle), 0x4F);
    }
}
#endif

const char D_800E1760[] = "Time Limit";
const char D_800E176C[] = "/%d";

void func_80078D3C(s32 arg0) {
    char sp18[0x20];

    func_80048278(0x38, 0x47, (char *)D_800E1760, 5);
    sprintf(sp18, D_800E176C, D_801222F6);
    func_80048278(0x70, -0x48, sp18, 6);
}

// func_80078D9C best match: 94.116% at nonmatchings/func_80078D9C-2127290767680699791/base_3.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80078D9C.s")

#ifdef NON_MATCHING
const char D_800E1770[] = "%2.2d";
const char D_800E1778[] = "%2.2d";
const char D_800E1780[] = "%2.2d";
const char D_800E1788[] = "%d";

void func_80078D9C(s32 arg0) {
    char buffer[0xC];
    s32 x;
    char *digit;
    char *end;
    s32 endValue;

    sprintf(buffer, D_800E1770, D_80121B74.minutes);
    x = 0x48;
    if (x && D_80112130.popupFontHandle) {
    }
    end = &buffer[2];
    digit = buffer;
    do {
        func_80045A78((s16)x, 0x50, func_80043040(D_80112130.popupFontHandle), ((u8)*digit - 5) & 0xFFFF);
        digit++;
        x += 8;
    } while ((u32)digit < (u32)end);

    x += 8;
    sprintf(buffer, D_800E1778, D_80121B75);
    digit = buffer;
    end = &buffer[2];
    endValue = (u32)end;
    do {
        func_80045A78((s16)x, 0x50, func_80043040(D_80112130.popupFontHandle), ((u8)*digit - 5) & 0xFFFF);
        digit++;
        x += 8;
    } while ((u32)digit < endValue);

    x += 8;
    sprintf(end = buffer, D_800E1780, D_80121B76 >> 8);
    digit = end;
    end = &buffer[2];
    do {
        func_80045A78((s16)x, 0x50, func_80043040(D_80112130.popupFontHandle), ((u8)*digit - 5) & 0xFFFF);
        digit++;
        x += 8;
    } while (digit != end);

    func_80045A78(0x58, 0x50, func_80043040(D_80112130.popupFontHandle), 0x36);
    func_80045A78(0x70, 0x50, func_80043040(D_80112130.popupFontHandle), 0x35);

    if (D_80121B81 != 0) {
        func_80045A78(0x48, 0x54, func_80043040(D_80112130.popupFontHandle), 0x4F);
    }

    func_80045A78(-0x88, -0x60, func_80043040(D_80112130.popupFontHandle), D_800DC8F9);
    func_80045A78(-0x88, 0x40, func_80043040(D_80112130.popupFontHandle), 0x24);
    func_80045A78(-0x88, 0x40, func_80043040(D_80112130.popupFontHandle), 0x25);

    sprintf(buffer, D_800E1788, D_801222F0);
    func_80045A78(-0x60, -0x50, func_80043040(D_80112130.popupFontHandle), ((u8)buffer[0] - 5) & 0xFFFF);
    if (buffer[1] != 0) {
        func_80045A78(-0x58, -0x50, func_80043040(D_80112130.popupFontHandle), ((u8)buffer[1] - 5) & 0xFFFF);
    }
}
#endif

const char D_800E178C[] = "Lap Time";
const char D_800E1798[] = "Target Time";
const char D_800E17A4[] = "%2.2d'%2.2d\"%2.2d";
const char D_800E17B8[] = "*%d/%d";

void func_80079068(s32 arg0) {
    char sp28[0x20];
    RaceUiCoursePosition *pos;

    func_80048278(0x48, 0x47, (char *)D_800E178C, 5);
    func_80048278(0x32, -0x60, (char *)D_800E1798, 7);
    pos = &D_800DC900[D_80121B50];
    sprintf(sp28, D_800E17A4, pos->x, pos->y, pos->z >> 8);
    func_80048278(0x48, -0x57, sp28, 7);
    sprintf(sp28, D_800E17B8, D_80121D80[0].score, D_80121D80[0].targetScore);
    sp28[1] = ' ';
    if (sp28[2] != '/') {
        sp28[2] = ' ';
    }
    func_80048278(-0x68, -0x48, sp28, 6);
}

const char D_800E17C0[] = "%2.2d";
const char D_800E17C8[] = "%2.2d";
const char D_800E17D0[] = "%2.2d";

void func_80079154(s32 arg0) {
    char buffer[0xC];
    s32 x;
    char *digit;
    char *end;
    s32 endValue;

    sprintf(buffer, D_800E17C0, D_80121B74.minutes);
    x = 0x48;
    if (x && D_80112130.popupFontHandle) {
    }
    digit = buffer;
    end = &buffer[2];
    do {
        func_80045A78((s16)x, 0x50, func_80043040(D_80112130.popupFontHandle), ((u8)*digit - 5) & 0xFFFF);
        digit++;
        x += 8;
    } while ((u32)digit < (u32)end);

    x += 8;
    sprintf(buffer, D_800E17C8, D_80121B75);
    digit = buffer;
    end = &buffer[2];
    endValue = (u32)end;
    do {
        func_80045A78((s16)x, 0x50, func_80043040(D_80112130.popupFontHandle), ((u8)*digit - 5) & 0xFFFF);
        digit++;
        x += 8;
    } while ((u32)digit < endValue);

    x += 8;
    sprintf(end = buffer, D_800E17D0, D_80121B76 >> 8);
    digit = end;
    end = &buffer[2];
    do {
        func_80045A78((s16)x, 0x50, func_80043040(D_80112130.popupFontHandle), ((u8)*digit - 5) & 0xFFFF);
        digit++;
        x += 8;
    } while (digit != end);

    func_80045A78(0x58, 0x50, func_80043040(D_80112130.popupFontHandle), 0x36);
    func_80045A78(0x70, 0x50, func_80043040(D_80112130.popupFontHandle), 0x35);

    if (D_80121D80[0].onesDigitTile != 0) {
        func_80045A78(-0x10, -0x60, func_80043040(D_80112130.popupFontHandle), D_800DC8F8[D_80121D80[0].onesDigitTile]);
    }

    func_80045A78(-0x88, 0x40, func_80043040(D_80112130.popupFontHandle), 0x24);
    func_80045A78(-0x88, 0x40, func_80043040(D_80112130.popupFontHandle), 0x2A);
}

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

// func_80079438 best match: 95.528% at nonmatchings/func_80079438-3236181511606361864/base_5.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80079438.s")

#ifdef NON_MATCHING
const char D_800E1804[] = "%5ld";

void func_80079438(s32 arg0) {
    volatile u8 padding[0x18];
    char end;
    char buffer[5];
    s32 x;
    s32 palette;
    char *digit;
    volatile RaceTimerUiPlayer *player;

    player = &D_80121D80[0];
    sprintf(buffer, D_800E1804, player->timerValue);

    x = 0x50;
    digit = buffer;
    if (player->timerValue < 100) {
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
    } while (digit != &end);

    func_80045A78(0x78, 0x50, func_80043040(D_80112130.mainFontHandle), ((D_80121B72 >> 1) + 4) & 0xFFFF);

    if (player->tensDigitPalette != 0) {
        func_80047174(-0x20, -0x60, func_80043040(D_80112130.popupFontHandle),
                      (player->tensDigitOffset + D_800DC8F0[player->tensDigitTile] - 1) & 0xFFFF,
                      player->tensDigitPalette);
    } else {
        func_80045A78(-0x20, -0x60, func_80043040(D_80112130.popupFontHandle),
                      (player->tensDigitOffset + D_800DC8F0[player->tensDigitTile] - 1) & 0xFFFF);
    }

    if (player->onesDigitPalette != 0) {
        func_80047174(0, -0x60, func_80043040(D_80112130.popupFontHandle), D_800DC8F8[player->onesDigitTile],
                      player->onesDigitPalette);
    } else {
        func_80045A78(0, -0x60, func_80043040(D_80112130.popupFontHandle), D_800DC8F8[player->onesDigitTile]);
    }

    func_80045A78(-0x88, 0x40, func_80043040(D_80112130.popupFontHandle), player->iconTile & 0xFFFF);
    func_80045A78(-0x88, -0x60, func_80043040(D_80112130.popupFontHandle), 0x39);
    func_80046D68(-0x68, -0x60, func_80043040(D_80112130.popupFontHandle), (player->lapDigit + 0x2C) & 0xFFFF, 0xE);
    func_80045A78(-0x5C, -0x60, func_80043040(D_80112130.popupFontHandle), 0x38);
    func_80046D68(-0x50, -0x60, func_80043040(D_80112130.popupFontHandle), (D_80121B52 + 0x2B) & 0xFFFF, 0xE);
}
#endif

void func_80079750(s32 arg0) {

}

// func_80079758 best match: 99.255% at nonmatchings/func_80079758-3836525038718587862/base_11.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80079758.s")

#ifdef NON_MATCHING
const char D_800E180C[] = "%5ld";

void func_80079758(s32 arg0) {
    volatile u8 padding[0x18];
    char end;
    char buffer[5];
    char *digit;
    s32 x;
    s32 palette;
    RaceTimerUiPlayer *player;

    player = &D_80121D80[0];
    sprintf(buffer, D_800E180C, D_80121D80[0].timerValue);

    x = 0x50;
    digit = buffer;
    if (D_80121D80[0].timerValue < 100) {
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
    } while (digit != &end);

    func_80045A78(0x78, 0x50, func_80043040(D_80112130.mainFontHandle), ((D_80121B72 >> 1) + 4) & 0xFFFF);
    if (((!buffer) && (!buffer)) && (!buffer)) {
    }

    if (player->tensDigitPalette != 0) {
        func_80047174(-0x20, -0x60, func_80043040(D_80112130.popupFontHandle),
                      (player->tensDigitOffset + D_800DC8F0[player->tensDigitTile] - 1) & 0xFFFF,
                      player->tensDigitPalette);
    } else {
        func_80045A78(-0x20, -0x60, func_80043040(D_80112130.popupFontHandle),
                      (player->tensDigitOffset + D_800DC8F0[player->tensDigitTile] - 1) & 0xFFFF);
    }

    if (player->onesDigitPalette != 0) {
        func_80047174(0, -0x60, func_80043040(D_80112130.popupFontHandle), D_800DC8F8[player->onesDigitTile],
                      player->onesDigitPalette);
    } else {
        func_80045A78(0, -0x60, func_80043040(D_80112130.popupFontHandle), D_800DC8F8[player->onesDigitTile]);
    }

    func_80045A78(-0x88, 0x40, func_80043040(D_80112130.popupFontHandle), 0x24);
    func_80045A78(-0x88, 0x40, func_80043040(D_80112130.popupFontHandle), 0x29);
}
#endif

// func_800799DC best match: 84.859% (nonmatchings/func_800799DC-4033633224288138541/base_3.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_800799DC.s")

#ifdef NON_MATCHING
const char D_800E1814[] = "%5ld";

void func_800799DC(s32 arg0) {
    char buffer[8];
    s16 y;
    s16 finalY;
    s16 x;
    s32 color;
    char *ptr;
    char *end;
    RaceTimerUiPlayer *player;

    if (D_80156608 == 0) {
        y = -0x28;
    } else {
        y = 0x1A;
    }

    sprintf(buffer, D_800E1814, D_801222E8[D_80156608].value);
    x = 0x50;
    ptr = buffer;
    end = &buffer[5];
    if (D_801222E8[D_80156608].value < 0x64) {
        color = 0x10;
    } else {
        color = 0xE;
    }

    do {
        if (*ptr != ' ') {
            func_80046D68(x, y, func_80043040(D_80112130.popupFontHandle), ((*ptr) - 5) & 0xFFFF, color & 0xFFFF);
        }
        ptr++;
        x += 8;
    } while (ptr != end);

    func_80045A78(0x78, y, func_80043040(D_80112130.mainFontHandle), ((D_80121B72 >> 1) + 4) & 0xFFFF);

    if (D_80122293[D_80156608].value != 0) {
        player = &D_80121D80[D_80156608];
        func_80047174(-0x88, -0x30, func_80043040(D_80112130.popupFontHandle),
                      (player->tensDigitOffset + D_800DC8F0[player->tensDigitTile] - 1) & 0xFFFF,
                      player->tensDigitPalette);
    } else {
        player = &D_80121D80[D_80156608];
        func_80045A78(-0x88, -0x30, func_80043040(D_80112130.popupFontHandle),
                      (player->tensDigitOffset + D_800DC8F0[player->tensDigitTile] - 1) & 0xFFFF);
    }

    if (D_80122296[D_80156608].value != 0) {
        player = &D_80121D80[D_80156608];
        func_80047174(-0x68, -0x30, func_80043040(D_80112130.popupFontHandle), D_800DC8F8[player->onesDigitTile],
                      player->onesDigitPalette);
    } else {
        func_80045A78(-0x68, -0x30, func_80043040(D_80112130.popupFontHandle),
                      D_800DC8F8[D_80122295[D_80156608].value]);
    }

    func_80045A78(-0x88, 0x12, func_80043040(D_80112130.popupFontHandle), D_80122289[D_80156608].value & 0xFFFF);

    finalY = 0x2A;
    if (D_80156608 == 0) {
        finalY = -0x30;
    }
    func_80045A78(0x58, finalY, func_80043040(D_80112130.popupFontHandle), 0x1A);
}
#endif

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

void func_80079F04(s32 arg0) {
    RaceTimerUiPlayer *player;
    s32 texture;

    func_80047174(0x38, 0x24, func_80043040(D_80112130.mainFontHandle), ((D_80121B72 >> 1) + 4) & 0xFFFF, 1);

    texture = func_80043040(D_80112130.popupFontHandle);
    player = &D_80121D80[D_80156608];
    func_80047174(-0x18, -0x38, texture,
                  (D_800DC8F0[D_80121D80[D_80156608].tensDigitTile] + D_80121D80[D_80156608].tensDigitOffset - 1) & 0xFFFF,
                  player->tensDigitPalette + 1);

    texture = func_80043040(D_80112130.popupFontHandle);
    player = &D_80121D80[D_80156608];
    func_80047174(-8, -0x38, texture, D_800DC8F8[player->onesDigitTile], player->onesDigitPalette + 1);

    texture = func_80043040(D_80112130.popupFontHandle);
    func_80047174(-0x4C, 0x18, texture, D_80122289[D_80156608].value & 0xFFFF, 1);

    if (D_80156608 < 2) {
        func_80045A78(-0x44, -0x30, func_80043040(D_80112130.popupFontHandle), 0x1A);
        return;
    }
    func_80045A78(0x14, -0x30, func_80043040(D_80112130.popupFontHandle), 0x1A);
}

const char D_800E181C[] = "%5ld";

void func_8007A108(s32 arg0) {
    s32 x;
    s32 palette;
    char buffer[0x20];

    sprintf(buffer, D_800E181C, D_801222E8[D_80156608].value);
    if (D_801222E8[D_80156608].value < 0x64) {
        palette = 1;
    } else {
        palette = 2;
    }
    func_80048278(0x14, 0x28, buffer, palette & 0xFFFF);

    if (D_80156608 < 2) {
        x = -0x2C;
    } else {
        x = 0x2C;
    }

    func_80047E88((s16)x, -0x30, (((RaceTimerUiS8Stride *)D_80122288)[D_80156608].value + 0x31) & 0xFF, 2);
    func_80047E88((s16)(x + 8), -0x30, 0x2F, 2);
    func_80047E88((s16)(x + 0x10), -0x30, (D_80121B52 + 0x30) & 0xFF, 2);
}

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

// func_8007A3D8 best match: 50.818% at nonmatchings/func_8007A3D8-2225551288923588688/base_3.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_8007A3D8.s")

#ifdef NON_MATCHING
void func_8007A3D8(s32 arg0) {
    s16 yBase;
    s16 xBase;
    s32 order[4];
    s32 previous;
    s32 index;
    s32 *slot;
    s32 *scan;
    s32 temp;
    s8 bestTile;
    RaceTimerUiPlayer *player;
    s16 x;
    u8 iconGroup;

    order[0] = 0;
    order[1] = 1;
    order[2] = 2;
    order[3] = 3;

    previous = 0;
    index = 1;
    do {
        if (index < 4) {
            slot = &order[previous];
            if ((4 - index) & 1) {
                scan = &order[index];
                temp = *scan;
                if (D_80121D80[temp].iconTile < D_80121D80[*slot].iconTile) {
                    *scan = *slot;
                    *slot = temp;
                }
                index++;
                if (index == 4) {
                    goto sort_next;
                }
            }

            scan = &order[index];
            do {
                temp = *scan;
                bestTile = D_80121D80[*slot].iconTile;
                if (D_80121D80[temp].iconTile < bestTile) {
                    *scan = *slot;
                    *slot = temp;
                    bestTile = D_80121D80[*slot].iconTile;
                }

                temp = scan[1];
                if (D_80121D80[temp].iconTile < bestTile) {
                    scan[1] = *slot;
                    *slot = temp;
                }
                scan += 2;
            } while (scan != &order[4]);
        }
sort_next:
        previous = index;
        index++;
    } while (previous < 3);

    if (D_80121B70 == 0) {
        yBase = -0x56;
        xBase = 0x78;
    }
    if (D_80121B70 == 1) {
        xBase = 0x78;
        yBase = -0x48;
    }
    if ((D_80121B70 == 2) || (D_80121B70 == 3)) {
        xBase = -8;
        yBase = -0x48;
    }

    func_80045A78((s16)(xBase + 4), (s16)(yBase + 4), func_80043040(D_80112130.popupFontHandle), 0x50);

    slot = &order[3];
    do {
        player = &D_80121D80[*slot];
        x = xBase - 8;

        temp = player->unk2FC & 0x200000;
        if ((temp != 0) || (player->flashFrame != 0)) {
            player->flashFrame++;
            temp = player->unk2FC & 0x200000;
        }
        if ((temp != 0) && (player->flashFrame >= 5)) {
            player->flashFrame = 4;
        }
        if (player->flashFrame >= 6) {
            player->flashFrame = 0;
        }

        if (player->flashFrame != 0) {
            if (player->unk2D8 != 0) {
                player = &D_80121D80[*slot];
                iconGroup = player->iconGroup;
                func_80046D68(x, (s16)(player->raceProgress + yBase),
                              func_80043040(D_80112130.popupFontHandle),
                              D_800DC954[player->flashFrame + (iconGroup * 6)],
                              D_800DC99C[iconGroup]);
            } else {
                player = &D_80121D80[*slot];
                func_80045A78(x, (s16)(player->raceProgress + yBase),
                              func_80043040(D_80112130.popupFontHandle),
                              D_800DC954[player->flashFrame + (player->iconGroup * 6)]);
            }
        } else if (player->unk2D8 != 0) {
            player = &D_80121D80[*slot];
            iconGroup = player->iconGroup;
            func_80046D68(xBase, (s16)(player->raceProgress + yBase),
                          func_80043040(D_80112130.popupFontHandle),
                          D_800DC954[player->flashFrame + (iconGroup * 6)],
                          D_800DC99C[iconGroup]);
        } else {
            player = &D_80121D80[*slot];
            func_80045A78(xBase, (s16)(player->raceProgress + yBase),
                          func_80043040(D_80112130.popupFontHandle),
                          D_800DC954[player->flashFrame + (player->iconGroup * 6)]);
        }

        slot--;
    } while ((u32)slot >= (u32)&order[0]);
}
#endif

// func_8007A8EC best match: 98.371% at nonmatchings/func_8007A8EC-5272447827802519043/base_13.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_8007A8EC.s")

#ifdef NON_MATCHING
void func_8007A8EC(void) {
    u8 pad[4];
    s32 sp50;
    s32 sp4C;
    RaceTimerUiPlayer *player;
    s32 shiftedProgress;
    s16 pathIndex;
    s32 i;
    s32 progress;

    do { player = D_80121D80; i = 0; do { func_80087600(i, &sp50, &sp4C); progress = sp50; i++; if (progress < 0) { progress = 0; } shiftedProgress = progress << 7; pathIndex = D_800B9540[D_80121B50].pathIndex; sp50 = progress; player->raceProgress = shiftedProgress / (pathIndex * 8); if (player->raceProgress >= 0x81) { player->raceProgress = 0x80; } player++; } while (i != 4); } while (0);

    if (D_80121B70 == 0) {
        func_800483FC(&D_80124878, func_8007A3D8, 0);
        return;
    }
    func_800483FC(&D_80124868, func_8007A3D8, 0);
}
#endif

// func_8007AA50 best match: 93.254% at nonmatchings/func_8007AA50-7273315160691878794/base_17.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_8007AA50.s")

#ifdef NON_MATCHING
void func_8007AA50(void) {
    RaceTimer sp38;
    s32 sp40;
    s32 sp3C;
    RaceTimerUiPlayer *player;
    s16 i;

    D_80121B72++;
    if (D_80121B72 >= 12) {
        D_80121B72 = 0;
    }

    switch ((u16)D_80121B70) {
    case 0:
        player = D_80121D80;
        if (player->tensDigitPalette != 0) {
            if (player->tensDigitPalette == 4) {
                func_800503D0(-0x20, -0x60, 0, 0, 0);
            }
            player->tensDigitPalette--;
        }
        if (player->onesDigitPalette != 0) {
            if (player->onesDigitPalette == 4) {
                func_800503D0(0, -0x60, 0, 0, 1);
            }
            player->onesDigitPalette--;
        }
        func_800483FC(&D_80124878, func_80079438, 0);
        func_800483FC(&D_80124888, func_80079750, 0);
        func_8007A8EC();
        return;
    case 1:
        player = D_80121D80;
        i = 0;
        do {
            if (player->tensDigitPalette != 0) {
                if (player->tensDigitPalette == 4) {
                    func_800503D0(-0x88, -0x30, i, 0, 0);
                }
                player->tensDigitPalette--;
            }
            if (player->onesDigitPalette != 0) {
                if (player->onesDigitPalette == 4) {
                    func_800503D0(-0x68, -0x30, i, 0, 1);
                }
                player->onesDigitPalette--;
            }
            i++;
            player++;
        } while (i != 2);
        func_800483FC(&D_80124878, func_800799DC, 0);
        func_800483FC(&D_80124888, func_80079E48, 0);
        func_8007A8EC();
        return;
    case 3:
        player = D_80121D80;
        if (player[3].tensDigitPalette != 0) {
            if (player[3].tensDigitPalette == 4) {
                func_800503D0(-0x10, -0x30, 3, 1, 0);
            }
            player[3].tensDigitPalette--;
        }
        if (player[3].onesDigitPalette != 0) {
            if (player[3].onesDigitPalette == 4) {
                func_800503D0(0, -0x30, 3, 1, 1);
            }
            player[3].onesDigitPalette--;
        }
    case 2:
        player = D_80121D80;
        i = 0;
        do {
            if (player->tensDigitPalette != 0) {
                if (player->tensDigitPalette == 4) {
                    func_800503D0(-0x10, -0x30, i, 1, 0);
                }
                player->tensDigitPalette--;
            }
            if (player->onesDigitPalette != 0) {
                if (player->onesDigitPalette == 4) {
                    func_800503D0(0, -0x30, i, 1, 1);
                }
                player->onesDigitPalette--;
            }
            i++;
            player++;
        } while (i != 3);
        func_800483FC(&D_80124878, func_80079F04, 0);
        func_800483FC(&D_80124888, func_8007A108, 0);
        func_8007A8EC();
        if ((s16)D_80121B70 == 2) {
            func_800483FC(&D_80124868, func_8007A278, 0);
        }
        return;
    case 4:
        if (!(D_801235B4 & 3)) {
            func_8007A2B8();
            if (D_80121B80 == 0) {
                func_80087600(0, &sp40, &sp3C);
                if ((D_800B9540[D_80121B50].finishLinePathIndex * 8) < sp40) {
                    D_80121B7C = *(s32 *)&D_80121B74;
                    D_80121B80 = 1;
                    func_80071408(func_80065144, 0, 0);
                }
            }
        }
        func_800483FC(&D_80124878, func_80079154, 0);
        func_800483FC(&D_80124888, func_80079394, 0);
        return;
    case 5:
        if (!(D_801235B4 & 3)) {
            func_8007A2B8();
        }
        if (func_8007B130((RaceTimer *)&D_800DC900[D_80121B50], &D_80121B74, &sp38) != 0) {
            D_80121B81 = 1;
        }
        func_800483FC(&D_80124878, func_80078D9C, 0);
        func_800483FC(&D_80124888, func_80079068, 0);
        return;
    case 6:
        if (!(D_801235B4 & 3)) {
            func_8007A350();
        }
        if ((D_80121B78.minutes == 0) && (D_80121B78.seconds == 0) && (D_80121B78.fraction == 0)) {
            D_80121B81 = 1;
        }
        func_800483FC(&D_80124878, func_800789C0, 0);
        func_800483FC(&D_80124888, func_80078D3C, 0);
        return;
    case 7:
        if (!(D_801235B4 & 3)) {
            func_8007A350();
        }
        if ((D_80121B78.minutes == 0) && (D_80121B78.seconds == 0) && (D_80121B78.fraction == 0)) {
            D_80121B81 = 1;
        }
        func_800483FC(&D_80124878, func_80078568, 0);
        func_800483FC(&D_80124888, func_80078974, 0);
        return;
    case 8:
        player = D_80121D80;
        if (player->tensDigitPalette != 0) {
            if (player->tensDigitPalette == 4) {
                func_800503D0(-0x20, -0x60, 0, 0, 0);
            }
            player->tensDigitPalette--;
        }
        if (player->onesDigitPalette != 0) {
            if (player->onesDigitPalette == 4) {
                func_800503D0(0, -0x60, 0, 0, 1);
            }
            player->onesDigitPalette--;
        }
        func_800483FC(&D_80124878, func_80079758, 0);
        return;
    }
}
#endif

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
