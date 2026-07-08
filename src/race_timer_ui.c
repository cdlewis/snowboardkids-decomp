#include "common.h"
#include "memory_allocator.h"

typedef struct {
    /* 0x0 */ s8 minutes;
    /* 0x1 */ s8 seconds;
    /* 0x2 */ s16 fraction;
} RaceTimer;

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
    /* 0x02 */ u8 pad2[0x48 - 0x02];
} RaceTimerCourseSpawnEntry;

typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ s16 mainFontHandle;
    /* 0x3A */ u8 pad3A[0x3E - 0x3A];
    /* 0x3E */ s16 popupFontHandle;
} RaceTimerUiAssetHandles;

typedef struct {
    /* 0x000 */ u8 pad0[0x512];
    /* 0x512 */ s8 tensDigitTile;
    /* 0x513 */ s8 tensDigitPalette;
    /* 0x514 */ s8 tensDigitOffset;
    /* 0x515 */ s8 onesDigitTile;
    /* 0x516 */ s8 onesDigitPalette;
    /* 0x517 */ u8 pad517[0x570 - 0x517];
    /* 0x570 */ s16 score;
    /* 0x572 */ s16 targetScore;
    /* 0x574 */ u8 pad574[0x57E - 0x574];
    /* 0x57E */ s16 raceProgress;
    /* 0x580 */ u8 pad580[0x60C - 0x580];
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
extern void func_8007A3D8(void);
extern void func_80087600(s32, s32 *, s32 *);
extern int sprintf(char *, const char *, ...);
extern RaceTimerCourseSpawnEntry D_800B9540[];
extern u8 D_800DC8F0[];
extern u8 D_800DC8F8[];
extern RaceUiCoursePosition D_800DC900[];
extern CourseDataStride D_800EC9F0[];
extern RaceTimerUiAssetHandles D_80112130;
extern s16 D_8011216E;
extern s16 D_80121B52;
extern s16 D_80121B72;
extern s16 D_80121B70;
extern s16 D_801222F6;
extern s16 D_80121B50;
extern u8 D_80121B56;
extern u8 D_80156608;
extern s32 D_80124868;
extern s32 D_80124878;
extern RaceTimer D_80121B74;
extern RaceTimer D_80121B78;
extern s8 D_80122288[];
extern RaceTimerUiPlayer D_80121D80[];
extern RaceTimerUiS8Stride D_80122289[];
extern RaceTimerUiS8Stride D_80122293[];
extern RaceTimerUiS8Stride D_80122295[];
extern RaceTimerUiS8Stride D_80122296[];
extern RaceTimerUiS32Stride D_801222E8[];

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

// func_80079154 best match: 99.792% at nonmatchings/func_80079154-5272447827802519043/base_8.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_timer_ui/func_80079154.s")

#ifdef NON_MATCHING
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
    end = &buffer[2];
    digit = buffer;
    do {
        func_80045A78((s16)x, 0x50, func_80043040(D_80112130.popupFontHandle), ((u8)*digit - 5) & 0xFFFF);
        digit++;
        x += 8;
    } while ((u32)digit < (u32)end);

    x += 8;
    sprintf(buffer, D_800E17C8, D_80121B74.seconds);
    digit = buffer;
    end = &buffer[2];
    endValue = (u32)end;
    do {
        func_80045A78((s16)x, 0x50, func_80043040(D_80112130.popupFontHandle), ((u8)*digit - 5) & 0xFFFF);
        digit++;
        x += 8;
    } while ((u32)digit < endValue);

    x += 8;
    sprintf(end = buffer, D_800E17D0, D_80121B74.fraction >> 8);
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
#endif

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
    RaceTimerUiPlayer *player;

    player = &D_80121D80[0];
    sprintf(&sp4C, D_800E180C, D_801222E8[0].value);

    x = 0x50;
    digit = &sp4C;
    if (D_801222E8[0].value < 100) {
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
