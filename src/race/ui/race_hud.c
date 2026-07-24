#include "common.h"
#include "assets.h"
#include "game/engine/asset_manager.h"
#include "game/engine/relocatable_heap.h"
#include "game/race/items/race_item_effects.h"
#include "game/race/player/race_player_movement.h"
#include "game/race/ui/race_hud.h"
#include "game/race/ui/race_ui_effects.h"
#include "game/race/player/race_player_input.h"

#define ASSET_HANDLE(index) (((s16 *)&gAssetHandles)[index])

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
    /* 0x00 */ s16 finishLinePathIndex;
    /* 0x02 */ u8 pad2[0x48 - 0x02];
} RaceFinishLinePathIndexEntry;

typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ s16 mainFontHandle;
    /* 0x3A */ u8 pad3A[0x3E - 0x3A];
    /* 0x3E */ s16 popupFontHandle;
} RaceTimerUiAssetHandles;

extern void drawAssetTableSprite(s16, s16, s32, s32);
extern void drawAssetTableSpriteWithExplicitPalette(s16, s16, s32, s32, s32);
extern void drawScaledAssetTableSprite(s32, s32, s32, s32, s32);
extern void drawMenuAsciiChar(s32, s16, s32, s32);
extern void drawMenuAsciiTextDefaultScale(s32, s32, char *, s32);
extern void addRenderCallback(void *, void *, s32);
extern void *createCallbackTask(void (*)(), s32, s32);
extern int sprintf(char *, const char *, ...);
extern RaceTimer D_800DC928[];
extern RaceTimer D_800DC950;
extern u8 gRaceSplitscreenMode;
extern RaceTimerCourseSpawnEntry gRaceCourseStartEntries[];
extern RaceFinishLinePathIndexEntry gRaceCourseFinishLineData[];
extern u8 gRaceTimerTensDigitTileOffsets[];
extern u8 gRaceTimerOnesDigitTileIds[];
extern u16 gRaceProgressMeterIconTiles[];
extern u16 gRaceProgressMeterIconPalettes[];
extern RaceUiCoursePosition gRaceCourseTargetTimes[];
extern CourseDataStride gGameSaveDataBuffer[];
extern RaceTimerUiAssetHandles gAssetHandles;
extern s16 gRaceLapCount;
extern s16 gRaceHudSpinnerFrame;
extern s16 gRaceHudMode;
extern s16 gRaceTrickAttackPointTotal;
extern s8 D_80122043;
extern s16 gRaceScoreAttackPointTarget;
extern s16 gRaceScoreAttackPointTotal;
extern s16 gRaceCourseIndex;
extern u8 gPlayerCount;
extern u8 gRaceUpdatePaused;
extern u8 gTrainingCourseLesson;
extern u8 gMainMenuModeSelection;
extern u8 gRaceTypeSelection;
extern u8 gCurrentViewportIndex;
extern s16 gUiBlinkTimer;
extern s32 gMenuRenderCallbackList;
extern s32 gRaceOverlayRenderCallbackList;
extern s32 gRaceForegroundRenderCallbackList;
extern RaceTimer gRaceElapsedTimer;
extern RaceTimer gRaceChallengeTimeLimit;
extern u8 gRaceTimeTrialFinishRecorded;
extern u8 gRaceChallengeFailed;
extern s32 gMenuFlowState;
extern s32 gRaceTimeTrialFinishTime;

void initRaceHud(void) {
    LOAD_ASSET(_245A80, 0x1F);
    gRaceHudMode = -1;
    gRaceHudSpinnerFrame = 0;

    if (gPlayerCount == 1) {
        if (gRaceSplitscreenMode == 0) {
            gRaceHudMode = 0;
        }
        if (gRaceSplitscreenMode == 2) {
            gRaceHudMode = 4;
        }
        if (gRaceSplitscreenMode == 1) {
            if (gRaceTypeSelection == 0) {
                gRaceHudMode = 5;
            }
            if (gRaceTypeSelection == 1) {
                gRaceHudMode = 6;
                gRaceChallengeTimeLimit = D_800DC928[gRaceCourseIndex];
            }
            if (gRaceTypeSelection == 2) {
                gRaceHudMode = 7;
                gRaceChallengeTimeLimit = D_800DC950;
            }
        }
    } else {
        gRaceHudMode = gPlayerCount - 1;
    }

    if ((gTrainingCourseLesson != 0) || (gMainMenuModeSelection != 0)) {
        gRaceHudMode = 8;
    }

    gRaceElapsedTimer.minutes = 0;
    gRaceElapsedTimer.seconds = 0;
    gRaceElapsedTimer.fraction = 0;
    gRaceTimeTrialFinishRecorded = 0;
    gRaceChallengeFailed = 0;
}

const char gRaceHudChallengeTimeLimitMinutesFormat[] = "%2.2d";
const char gRaceHudChallengeTimeLimitSecondsFormat[] = "%2.2d";
const char gRaceHudChallengeTimeLimitFractionFormat[] = "%2.2d";
const char gRaceHudTrickAttackPointTotalFormat[] = "%4d";
const char gRaceHudTrickAttackSecondaryCounterFormat[] = "%2d";

void drawTrickAttackChallengeHud(s32 arg0) {
    s32 x;
    char *digit;
    char buffer[4];
    s32 palette;
    s32 finalPalette;
    union {
        char *end;
        s32 i;
    } temp;

    palette = 0xC;
    if ((gRaceChallengeTimeLimit.minutes == 0) && (gRaceChallengeTimeLimit.seconds < 10) && (gUiBlinkTimer & 1)) {
        palette = 0x10;
    }

    {
        sprintf(buffer, gRaceHudChallengeTimeLimitMinutesFormat, gRaceChallengeTimeLimit.minutes);
        finalPalette = palette & 0xFFFF;
        x = 0x40;
        digit = buffer; temp.end = &buffer[2]; do {
            drawAssetTableSpriteWithExplicitPalette((s16)x, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                          ((u8)*digit - 5) & 0xFFFF, finalPalette); digit++;
            x += 8;
        } while ((u32)digit < (u32)temp.end);

        x += 8;
        sprintf(buffer, gRaceHudChallengeTimeLimitSecondsFormat, gRaceChallengeTimeLimit.seconds);
        digit = buffer;
        temp.end = &buffer[2];
        temp.end += 0;
        do {
            drawAssetTableSpriteWithExplicitPalette((s16)x, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                          ((u8)*digit - 5) & 0xFFFF, finalPalette);
            digit++;
            x += 8;
        } while ((u32)digit < (u32)temp.end);

        x += 8;
        sprintf(buffer, gRaceHudChallengeTimeLimitFractionFormat, gRaceChallengeTimeLimit.fraction >> 8);
        digit = buffer;
        temp.end = &buffer[2];
        do {
            drawAssetTableSpriteWithExplicitPalette((s16)x, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                          ((u8)*digit - 5) & 0xFFFF, finalPalette);
            digit++;
            x += 8;
        } while ((u32)digit < (u32)temp.end);
    }

    drawAssetTableSpriteWithExplicitPalette(0x50, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x36, finalPalette);
    drawAssetTableSpriteWithExplicitPalette(0x68, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x35, finalPalette);

    if (D_80122043 < 10) {
        x = -0x90;
        temp.i = 0;
        if (D_80122043 > 0) {
            do {
                drawScaledAssetTableSprite((s16)x, -0x68, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x26, 1);
                temp.i++;
                x += 8;
            } while (temp.i < D_80122043);
        }
    } else {
        drawAssetTableSprite(-0x84, -0x60, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x26);
    }

    {
        char *end;

        x = 0x64;
        sprintf(buffer, gRaceHudTrickAttackPointTotalFormat, gRaceTrickAttackPointTotal);
        digit = buffer;
        end = &buffer[4];
        do {
            if ((u8)*digit != ' ') {
                drawAssetTableSprite((s16)x, -0x58, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                              ((u8)*digit - 5) & 0xFFFF);
            }
            digit++;
            x += 8;
        } while (digit != end);
    }

    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x27);
    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x28);
    if (gRaceChallengeFailed != 0) {
        drawAssetTableSprite(0x40, 0x54, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x4F);
    }

    if (D_80122043 >= 10) {
        sprintf(buffer, gRaceHudTrickAttackSecondaryCounterFormat, D_80122043);
        drawAssetTableSprite(-0x64, -0x59, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), ((u8)buffer[0] - 5) & 0xFFFF);
        drawAssetTableSprite(-0x5C, -0x59, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), ((u8)buffer[1] - 5) & 0xFFFF);
    }
}

const char gRaceHudTrickAttackPointLabel[] = "Point";
const char gRaceHudTrickAttackTimeLimitLabel[] = "Time Limit";

void drawTrickAttackChallengeLabels(s32 arg0) {
    drawMenuAsciiTextDefaultScale(0x60, -0x61, (char *)gRaceHudTrickAttackPointLabel, 5);
    drawMenuAsciiTextDefaultScale(0x38, 0x47, (char *)gRaceHudTrickAttackTimeLimitLabel, 5);
}

const char gRaceHudScoreAttackTimeLimitMinutesFormat[] = "%2.2d";
const char gRaceHudScoreAttackTimeLimitSecondsFormat[] = "%2.2d";
const char gRaceHudScoreAttackTimeLimitFractionFormat[] = "%2.2d";
const char gRaceHudScoreAttackPointTotalFormat[] = "%2d";

void drawScoreAttackChallengeHud(s32 arg0) {
    s32 x;
    char *digit;
    char buffer[4];
    s32 palette;
    s32 finalPalette;
    union {
        char *end;
        s32 i;
    } temp;

    palette = 0xC;
    if ((gRaceChallengeTimeLimit.minutes == 0) && (gRaceChallengeTimeLimit.seconds < 10) && (gUiBlinkTimer & 1)) {
        palette = 0x10;
        if (!gRacePlayers[0].shieldEffectTimer) {
        }
    }

    {
        sprintf(buffer, gRaceHudScoreAttackTimeLimitMinutesFormat, gRaceChallengeTimeLimit.minutes);
        finalPalette = palette & 0xFFFF;
        x = 0x40;
        digit = buffer; temp.end = &buffer[2]; do { drawAssetTableSpriteWithExplicitPalette((s16)x, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), ((u8)*digit - 5) & 0xFFFF, finalPalette); digit++;
            x += 8;
        } while ((u32)digit < (u32)temp.end);

        x += 8;
        sprintf(buffer, gRaceHudScoreAttackTimeLimitSecondsFormat, gRaceChallengeTimeLimit.seconds);
        digit = buffer;
        temp.end = &buffer[2];
        temp.end += 0;
        do {
            drawAssetTableSpriteWithExplicitPalette((s16)x, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                          ((u8)*digit - 5) & 0xFFFF, finalPalette);
            digit++;
            x += 8;
        } while ((u32)digit < (u32)temp.end);

        x += 8;
        sprintf(buffer, gRaceHudScoreAttackTimeLimitFractionFormat, gRaceChallengeTimeLimit.fraction >> 8);
        digit = buffer;
        temp.end = &buffer[2];
        do {
            drawAssetTableSpriteWithExplicitPalette((s16)x, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                          ((u8)*digit - 5) & 0xFFFF, finalPalette);
            digit++;
            x += 8;
        } while ((u32)digit < (u32)temp.end);
    }

    drawAssetTableSpriteWithExplicitPalette(0x50, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x36, finalPalette);
    drawAssetTableSpriteWithExplicitPalette(0x68, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x35, finalPalette);
    drawAssetTableSprite(0x68, -0x60, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x20);

    {
        char *end;

        sprintf(buffer, gRaceHudScoreAttackPointTotalFormat, gRaceScoreAttackPointTotal);
        x = 0;
        digit = buffer;
        end = &buffer[2];
        do {
            if ((u8)*digit != ' ') {
                drawAssetTableSprite((s16)(x + 0x60), -0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                              ((u8)*digit - 5) & 0xFFFF);
            }
            digit++;
            x += 8;
        } while ((u32)digit < (u32)end);
    }

    x = 0;
    temp.i = 0;
    if (gRacePlayers[0].shieldEffectTimer > 0) {
        do {
            drawAssetTableSprite((s16)(x - 0x88), -0x60, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x21);
            temp.i++;
            x += 8;
        } while (temp.i < gRacePlayers[0].shieldEffectTimer);
    }

    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x22);
    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x23);
    if (gRaceChallengeFailed != 0) {
        drawAssetTableSprite(0x40, 0x54, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x4F);
    }
}

const char gRaceHudScoreAttackTimeLimitLabel[] = "Time Limit";
const char gRaceHudScoreAttackPointTargetFormat[] = "/%d";

void drawScoreAttackChallengeLabels(s32 arg0) {
    char sp18[0x20];

    drawMenuAsciiTextDefaultScale(0x38, 0x47, (char *)gRaceHudScoreAttackTimeLimitLabel, 5);
    sprintf(sp18, gRaceHudScoreAttackPointTargetFormat, gRaceScoreAttackPointTarget);
    drawMenuAsciiTextDefaultScale(0x70, -0x48, sp18, 6);
}

// drawTargetTimeChallengeHud best match: 96.511% at nonmatchings/drawTargetTimeChallengeHud-6887713755923057488/base_18.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race/ui/race_hud/drawTargetTimeChallengeHud.s")

#ifdef NON_MATCHING
const char gRaceHudTargetTimeChallengeMinutesFormat[] = "%2.2d";
const char gRaceHudTargetTimeChallengeSecondsFormat[] = "%2.2d";
const char gRaceHudTargetTimeChallengeFractionFormat[] = "%2.2d";
const char gRaceHudTargetTimeChallengeSecondaryCounterFormat[] = "%d";

void drawTargetTimeChallengeHud(s32 arg0) {
    char *bufferAlias;
    char *endPointer;
    char buffer[0xC];
    s32 x;
    char *digit;
    u32 end;

    sprintf(buffer, gRaceHudTargetTimeChallengeMinutesFormat, gRaceElapsedTimer.minutes);
    x = 0x48;
    digit = buffer;
    do {
        drawAssetTableSprite((s16)x, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                             ((u8)*digit - 5) & 0xFFFF);
        digit++;
        x += 8;
        endPointer = &buffer[2];
        end = (u32)endPointer;
    } while (((u32)digit < end) != 0);

    x += 8;
    sprintf(buffer, gRaceHudTargetTimeChallengeSecondsFormat, gRaceElapsedTimer.seconds);
    digit = buffer;
    do {
        drawAssetTableSprite((s16)((short)x), 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                             ((u8)*digit - 5) & 0xFFFF);
        digit++;
        x += 8;
        end = (u32)endPointer;
    } while ((u32)digit < end);

    x += 8;
    sprintf(buffer, gRaceHudTargetTimeChallengeFractionFormat, gRaceElapsedTimer.fraction >> 8);
    digit = buffer;
    do {
        drawAssetTableSprite((s16)x, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                             (((u8)*digit & 0xFF) - 5) & 0xFFFF);
        do {
        } while (0);
        digit++;
        x += 8;
        end = (u32)&buffer[2];
    } while ((u32)digit != end);

    drawAssetTableSprite(0x58, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x36);
    drawAssetTableSprite(0x70, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x35);

    if (gRaceChallengeFailed != 0) {
        drawAssetTableSprite(0x48, 0x54, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x4F);
    }

    drawAssetTableSprite(-0x88, -0x60, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), gRaceTimerOnesDigitTileIds[1]);
    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x24);
    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x25);

    sprintf(buffer, gRaceHudTargetTimeChallengeSecondaryCounterFormat, gRacePlayers[0].unk570);
    bufferAlias = buffer;
    drawAssetTableSprite(-0x60, -0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                         ((u8)bufferAlias[0] - 5) & 0xFFFF);
    if (bufferAlias[1] != 0) {
        drawAssetTableSprite(-0x58, -0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                             ((u8)bufferAlias[1] + -5) & 0xFFFF);
    }
    if (!end) {
    }
}
#endif

const char gRaceHudTargetTimeChallengeLapTimeLabel[] = "Lap Time";
const char gRaceHudTargetTimeChallengeTargetTimeLabel[] = "Target Time";
const char gRaceHudTargetTimeChallengeTargetTimeFormat[] = "%2.2d'%2.2d\"%2.2d";
const char gRaceHudTargetTimeChallengeLapProgressFormat[] = "*%d/%d";

void drawTargetTimeChallengeLabels(s32 arg0) {
    char sp28[0x20];
    RaceUiCoursePosition *pos;

    drawMenuAsciiTextDefaultScale(0x48, 0x47, (char *)gRaceHudTargetTimeChallengeLapTimeLabel, 5);
    drawMenuAsciiTextDefaultScale(0x32, -0x60, (char *)gRaceHudTargetTimeChallengeTargetTimeLabel, 7);
    pos = &gRaceCourseTargetTimes[gRaceCourseIndex];
    sprintf(sp28, gRaceHudTargetTimeChallengeTargetTimeFormat, pos->x, pos->y, pos->z >> 8);
    drawMenuAsciiTextDefaultScale(0x48, -0x57, sp28, 7);
    sprintf(sp28, gRaceHudTargetTimeChallengeLapProgressFormat, gRacePlayers[0].unk570, gRacePlayers[0].unk572);
    sp28[1] = ' ';
    if (sp28[2] != '/') {
        sp28[2] = ' ';
    }
    drawMenuAsciiTextDefaultScale(-0x68, -0x48, sp28, 6);
}

const char gRaceHudTimeTrialMinutesFormat[] = "%2.2d";
const char gRaceHudTimeTrialSecondsFormat[] = "%2.2d";
const char gRaceHudTimeTrialFractionFormat[] = "%2.2d";

void drawTimeTrialHud(s32 arg0) {
    char buffer[0xC];
    s32 x;
    char *digit;
    char *end;
    s32 endValue;

    sprintf(buffer, gRaceHudTimeTrialMinutesFormat, gRaceElapsedTimer.minutes);
    x = 0x48;
    if (x && gAssetHandles.popupFontHandle) {
    }
    digit = buffer;
    end = &buffer[2];
    do {
        drawAssetTableSprite((s16)x, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), ((u8)*digit - 5) & 0xFFFF);
        digit++;
        x += 8;
    } while ((u32)digit < (u32)end);

    x += 8;
    sprintf(buffer, gRaceHudTimeTrialSecondsFormat, gRaceElapsedTimer.seconds);
    digit = buffer;
    end = &buffer[2];
    endValue = (u32)end;
    do {
        drawAssetTableSprite((s16)x, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), ((u8)*digit - 5) & 0xFFFF);
        digit++;
        x += 8;
    } while ((u32)digit < endValue);

    x += 8;
    sprintf(end = buffer, gRaceHudTimeTrialFractionFormat, gRaceElapsedTimer.fraction >> 8);
    digit = end;
    end = &buffer[2];
    do {
        drawAssetTableSprite((s16)x, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), ((u8)*digit - 5) & 0xFFFF);
        digit++;
        x += 8;
    } while (digit != end);

    drawAssetTableSprite(0x58, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x36);
    drawAssetTableSprite(0x70, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x35);

    if (gRacePlayers[0].actionEffectType != 0) {
        drawAssetTableSprite(-0x10, -0x60, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), gRaceTimerOnesDigitTileIds[gRacePlayers[0].actionEffectType]);
    }

    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x24);
    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x2A);
}

const char gRaceHudTimeTrialLapTimeLabel[] = "Lap Time";
const char gRaceHudTimeTrialBestLapLabel[] = "Best Lap";
const char gRaceHudTimeTrialBestLapFormat[] = "%2.2d'%2.2d\"%2.2d";

void drawTimeTrialLabels(s32 arg0) {
    char sp28[0x20];
    CourseBestLapView *course;

    drawMenuAsciiTextDefaultScale(0x48, 0x47, (char *)gRaceHudTimeTrialLapTimeLabel, 5);
    drawMenuAsciiTextDefaultScale(0x48, -0x61, (char *)gRaceHudTimeTrialBestLapLabel, 7);
    course = (CourseBestLapView *)&gGameSaveDataBuffer[gRaceCourseIndex];
    sprintf(sp28, gRaceHudTimeTrialBestLapFormat, course->bestLapMinutes, course->bestLapSeconds, course->bestLapFraction >> 8);
    drawMenuAsciiTextDefaultScale(0x48, -0x58, sp28, 7);
}

const char gRaceHudSinglePlayerTimerFormat[] = "%5ld";

void drawSinglePlayerRaceHud(s32 arg0) {
    s32 palette;
    s32 i;
    s32 var_s1;
    char buffer[0x20];

    sprintf(buffer, gRaceHudSinglePlayerTimerFormat, gRacePlayers[0].timerValue);
    if (gRacePlayers[0].timerValue < 100) {
        palette = 0x10;
    } else {
        palette = 0xE;
    }

    for (i = 0, var_s1 = 0x50; i < 5; i++, var_s1 += 8) {
        if (buffer[i] != ' ') {
            drawAssetTableSpriteWithExplicitPalette(var_s1, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                                                    ((u8)buffer[i] - 5) & 0xFFFF, palette & 0xFFFF);
        }
    }

    drawAssetTableSprite(0x78, 0x50, getRelocatableHeapBlockBase(gAssetHandles.mainFontHandle),
                         ((gRaceHudSpinnerFrame >> 1) + 4) & 0xFFFF);

    if (gRacePlayers[0].tensDigitPalette != 0) {
        drawScaledAssetTableSprite(-0x20, -0x60, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                                   (gRaceTimerTensDigitTileOffsets[gRacePlayers[0].tensDigitTile] + gRacePlayers[0].tensDigitOffset - 1) &
                                       0xFFFF,
                                   gRacePlayers[0].tensDigitPalette);
    } else {
        drawAssetTableSprite(-0x20, -0x60, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                             (gRaceTimerTensDigitTileOffsets[gRacePlayers[0].tensDigitTile] + gRacePlayers[0].tensDigitOffset - 1) &
                                 0xFFFF);
    }

    if (gRacePlayers[0].onesDigitPalette != 0) {
        drawScaledAssetTableSprite(0, -0x60, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                                   gRaceTimerOnesDigitTileIds[gRacePlayers[0].onesDigitTile], gRacePlayers[0].onesDigitPalette);
    } else {
        drawAssetTableSprite(0, -0x60, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                             gRaceTimerOnesDigitTileIds[gRacePlayers[0].onesDigitTile]);
    }

    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                         gRacePlayers[0].iconTile & 0xFFFF);
    drawAssetTableSprite(-0x88, -0x60, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x39);
    drawAssetTableSpriteWithExplicitPalette(-0x68, -0x60, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                                            (gRacePlayers[0].lapDigit + 0x2C) & 0xFFFF, 0xE);
    drawAssetTableSprite(-0x5C, -0x60, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x38);
    drawAssetTableSpriteWithExplicitPalette(-0x50, -0x60, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                                            (gRaceLapCount + 0x2B) & 0xFFFF, 0xE);
}

void noopRaceHudCallback(s32 arg0) {

}

const char gRaceHudTrainingTimerFormat[] = "%5ld";

void drawTrainingRaceHud(s32 arg0) {
    s32 palette;
    s32 i;
    s32 x;
    char buffer[0x20];

    sprintf(buffer, gRaceHudTrainingTimerFormat, gRacePlayers[0].unk568);
    if (gRacePlayers[0].unk568 < 100) {
        palette = 0x10;
    } else {
        palette = 0xE;
    }

    for (i = 0, x = 0x50; i < 5; i++, x += 8) {
        if (buffer[i] != ' ') {
            drawAssetTableSpriteWithExplicitPalette(x, 0x50, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                                                    ((u8)buffer[i] - 5) & 0xFFFF, palette & 0xFFFF);
        }
    }

    drawAssetTableSprite(0x78, 0x50, getRelocatableHeapBlockBase(gAssetHandles.mainFontHandle),
                         ((gRaceHudSpinnerFrame >> 1) + 4) & 0xFFFF);

    if (gRacePlayers[0].itemEffectPalette != 0) {
        drawScaledAssetTableSprite(-0x20, -0x60, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                                   (gRaceTimerTensDigitTileOffsets[gRacePlayers[0].itemEffectType] +
                                    gRacePlayers[0].itemEffectCount - 1) &
                                       0xFFFF,
                                   gRacePlayers[0].itemEffectPalette);
    } else {
        drawAssetTableSprite(-0x20, -0x60, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                             (gRaceTimerTensDigitTileOffsets[gRacePlayers[0].itemEffectType] + gRacePlayers[0].itemEffectCount - 1) &
                                 0xFFFF);
    }

    if (gRacePlayers[0].actionEffectPalette != 0) {
        drawScaledAssetTableSprite(0, -0x60, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                                   gRaceTimerOnesDigitTileIds[gRacePlayers[0].actionEffectType],
                                   gRacePlayers[0].actionEffectPalette);
    } else {
        drawAssetTableSprite(0, -0x60, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                             gRaceTimerOnesDigitTileIds[gRacePlayers[0].actionEffectType]);
    }

    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x24);
    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x29);
}

const char gRaceHudTwoPlayerTimerFormat[] = "%5ld";

void drawTwoPlayerRaceHud(s32 arg0) {
    volatile u8 padding[0x18];
    s32 y;
    s32 x;
    s32 color;
    char *ptr;
    volatile char filler1;
    volatile char filler2;
    char end;
    char buffer[5];

    if (gCurrentViewportIndex == 0) {
        y = -0x28;
    } else {
        y = 0x1A;
    }

    sprintf(buffer, gRaceHudTwoPlayerTimerFormat, gRacePlayers[gCurrentViewportIndex].unk568);
    x = 0x50;
    ptr = buffer;
    if (gRacePlayers[gCurrentViewportIndex].unk568 < 0x64) {
        color = 0x10;
    } else {
        color = 0xE;
    }

    do {
        if (*ptr != ' ') {
            drawAssetTableSpriteWithExplicitPalette(x, y, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), ((*ptr) - 5) & 0xFFFF, color & 0xFFFF);
        }
        ptr++;
        x += 8;
    } while (ptr != &end);

    drawAssetTableSprite(0x78, y, getRelocatableHeapBlockBase(gAssetHandles.mainFontHandle), ((gRaceHudSpinnerFrame >> 1) + 4) & 0xFFFF);

    if (gRacePlayers[gCurrentViewportIndex].itemEffectPalette != 0) {
        drawScaledAssetTableSprite(-0x88, -0x30, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                      (gRaceTimerTensDigitTileOffsets[gRacePlayers[gCurrentViewportIndex].itemEffectType] + gRacePlayers[gCurrentViewportIndex].itemEffectCount - 1) & 0xFFFF,
                      gRacePlayers[gCurrentViewportIndex].itemEffectPalette);
    } else {
        drawAssetTableSprite(-0x88, -0x30, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                      (gRaceTimerTensDigitTileOffsets[gRacePlayers[gCurrentViewportIndex].itemEffectType] + gRacePlayers[gCurrentViewportIndex].itemEffectCount - 1) & 0xFFFF);
    }

    if (gRacePlayers[gCurrentViewportIndex].actionEffectPalette != 0) {
        drawScaledAssetTableSprite(-0x68, -0x30, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), gRaceTimerOnesDigitTileIds[gRacePlayers[gCurrentViewportIndex].actionEffectType],
                      gRacePlayers[gCurrentViewportIndex].actionEffectPalette);
    } else {
        drawAssetTableSprite(-0x68, -0x30, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle),
                      gRaceTimerOnesDigitTileIds[gRacePlayers[gCurrentViewportIndex].actionEffectType]);
    }

    drawAssetTableSprite(-0x88, 0x12, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), gRacePlayers[gCurrentViewportIndex].iconTile & 0xFFFF);

    if (gCurrentViewportIndex == 0) {
        y = -0x30;
    } else {
        y = 0x2A;
    }
    if (buffer) {
    }
    drawAssetTableSprite(0x58, y, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x1A);
}

void drawTwoPlayerLapCounter(s32 arg0) {
    s32 y;
    s32 viewportIndex;

    viewportIndex = gCurrentViewportIndex;
    if (viewportIndex == 0) {
        y = -0x30;
    } else {
        y = 0x2A;
    }

    drawMenuAsciiChar(0x70, (s16)y, (gRacePlayers[viewportIndex].lapDigit + '1') & 0xFF, 2);
    drawMenuAsciiChar(0x78, (s16)y, '/', 2);
    drawMenuAsciiChar(0x80, (s16)y, (gRaceLapCount + '0') & 0xFF, 2);
}

void drawMultiplayerRaceHud(s32 arg0) {
    RacePlayer *player;
    s32 texture;

    drawScaledAssetTableSprite(0x38, 0x24, getRelocatableHeapBlockBase(gAssetHandles.mainFontHandle), ((gRaceHudSpinnerFrame >> 1) + 4) & 0xFFFF, 1);

    texture = getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle);
    player = &gRacePlayers[gCurrentViewportIndex];
    drawScaledAssetTableSprite(-0x18, -0x38, texture,
                  (gRaceTimerTensDigitTileOffsets[gRacePlayers[gCurrentViewportIndex].itemEffectType] + gRacePlayers[gCurrentViewportIndex].itemEffectCount - 1) & 0xFFFF,
                  player->itemEffectPalette + 1);

    texture = getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle);
    player = &gRacePlayers[gCurrentViewportIndex];
    drawScaledAssetTableSprite(-8, -0x38, texture, gRaceTimerOnesDigitTileIds[player->actionEffectType], player->actionEffectPalette + 1);

    texture = getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle);
    drawScaledAssetTableSprite(-0x4C, 0x18, texture, gRacePlayers[gCurrentViewportIndex].iconTile & 0xFFFF, 1);

    if (gCurrentViewportIndex < 2) {
        drawAssetTableSprite(-0x44, -0x30, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x1A);
        return;
    }
    drawAssetTableSprite(0x14, -0x30, getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x1A);
}

const char gRaceHudMultiplayerLapCounterFormat[] = "%5ld";

void drawMultiplayerLapCounter(s32 arg0) {
    s32 x;
    s32 palette;
    char buffer[0x20];

    sprintf(buffer, gRaceHudMultiplayerLapCounterFormat, gRacePlayers[gCurrentViewportIndex].unk568);
    if (gRacePlayers[gCurrentViewportIndex].unk568 < 0x64) {
        palette = 1;
    } else {
        palette = 2;
    }
    drawMenuAsciiTextDefaultScale(0x14, 0x28, buffer, palette & 0xFFFF);

    if (gCurrentViewportIndex < 2) {
        x = -0x2C;
    } else {
        x = 0x2C;
    }

    drawMenuAsciiChar((s16)x, -0x30, (gRacePlayers[gCurrentViewportIndex].lapDigit + '1') & 0xFF, 2);
    drawMenuAsciiChar((s16)(x + 8), -0x30, 0x2F, 2);
    drawMenuAsciiChar((s16)(x + 0x10), -0x30, (gRaceLapCount + 0x30) & 0xFF, 2);
}

void drawThreePlayerHudDivider(s32 arg0) {
    drawAssetTableSprite(0xC, 0x2C, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), 0x90);
}

void incrementRaceElapsedTimer(void) {
    if (gRaceUpdatePaused == 0) {
        gRaceElapsedTimer.fraction = gRaceElapsedTimer.fraction + 0x358;
        if (gRaceElapsedTimer.fraction >= 0x6401) {
            gRaceElapsedTimer.fraction = 0;
            gRaceElapsedTimer.seconds = gRaceElapsedTimer.seconds + 1;
            if (gRaceElapsedTimer.seconds >= 0x3C) {
                gRaceElapsedTimer.seconds = 0;
                gRaceElapsedTimer.minutes = gRaceElapsedTimer.minutes + 1;
                if (gRaceElapsedTimer.minutes >= 0x63) {
                    gRaceElapsedTimer.fraction = 0x6300;
                    gRaceElapsedTimer.seconds = 0x3B;
                    gRaceElapsedTimer.minutes = 0x63;
                }
            }
        }
    }
}

void decrementRaceChallengeTimeLimit(void) {
    if (gRaceUpdatePaused == 0) {
        gRaceChallengeTimeLimit.fraction = gRaceChallengeTimeLimit.fraction - 0x358;
        if (gRaceChallengeTimeLimit.fraction < 0) {
            gRaceChallengeTimeLimit.fraction = 0x60F8;
            gRaceChallengeTimeLimit.seconds = gRaceChallengeTimeLimit.seconds - 1;
            if (gRaceChallengeTimeLimit.seconds < 0) {
                gRaceChallengeTimeLimit.seconds = 0x3B;
                gRaceChallengeTimeLimit.minutes = gRaceChallengeTimeLimit.minutes - 1;
                if (gRaceChallengeTimeLimit.minutes < 0) {
                    gRaceChallengeTimeLimit.fraction = 0;
                    gRaceChallengeTimeLimit.seconds = 0;
                    gRaceChallengeTimeLimit.minutes = 0;
                }
            }
        }
    }
}

// drawRaceCourseProgressMeter best match: 99.769% at nonmatchings/drawRaceCourseProgressMeter-210831275846872038/base_21.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race/ui/race_hud/drawRaceCourseProgressMeter.s")

#ifdef NON_MATCHING
void drawRaceCourseProgressMeter(s32 arg0) {
    s32 i;
    s32 j;
    s32 temp;
    RacePlayer *player;
    s16 x;
    u8 characterId;
    s32 order[4];
    s16 xBase;
    s16 yBase;

    order[0] = 0;
    order[1] = 1;
    order[2] = 2;
    order[3] = 3;

    for (i = 0; i < 3; i++) {
        for (j = i + 1; j < 4; j++) {
            if (gRacePlayers[order[j]].rankIndex < gRacePlayers[order[i]].rankIndex) {
                temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }
        }
    }

    if (gRaceHudMode == 0) {
        xBase = 0x78;
        yBase = -0x56;
    }
    if (gRaceHudMode == 1) {
        xBase = 0x78;
        yBase = -0x48;
    }
    if ((gRaceHudMode == 2) || (gRaceHudMode == 3)) {
        xBase = -8;
        yBase = -0x48;
    }

    drawAssetTableSprite((s16)(xBase + 4), (s16)(yBase + 4),
                         getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle), 0x50);

    i = 3;
    do {
        player = &gRacePlayers[order[i]];
        x = xBase - 8;

        if (((player->stateFlags & 0x200000) != 0) || (player->unk580 != 0)) {
            player->unk580++;
        }
        if (((player->stateFlags & 0x200000) != 0) && (player->unk580 >= 5)) {
            player->unk580 = 4;
        }
        if (player->unk580 >= 6) {
            player->unk580 = 0;
        }

        if (player->unk580 != 0) {
            if (player->unk2D8 != 0) {
                temp = getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle);
                player = &gRacePlayers[order[i]];
                characterId = player->characterId;
                drawAssetTableSpriteWithExplicitPalette(
                    x, (s16)(player->unk57E + yBase), temp,
                    gRaceProgressMeterIconTiles[player->unk580 + (characterId * 6)],
                    gRaceProgressMeterIconPalettes[characterId]);
            } else {
                temp = getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle);
                player = &gRacePlayers[order[i]];
                drawAssetTableSprite(
                    x, (s16)(gRacePlayers[order[i]].unk57E + yBase), temp,
                    gRaceProgressMeterIconTiles[gRacePlayers[order[i]].unk580 + (player->characterId * 6)]);
            }
        } else if (player->unk2D8 != 0) {
            temp = getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle);
            player = &gRacePlayers[order[i]];
            characterId = player->characterId;
            drawAssetTableSpriteWithExplicitPalette(
                xBase, (s16)(player->unk57E + yBase), temp,
                gRaceProgressMeterIconTiles[player->unk580 + (characterId * 6)],
                gRaceProgressMeterIconPalettes[characterId]);
        } else {
            temp = getRelocatableHeapBlockBase(gAssetHandles.popupFontHandle);
            player = &gRacePlayers[order[i]];
            drawAssetTableSprite(
                xBase, (s16)(gRacePlayers[order[i]].unk57E + yBase), temp,
                gRaceProgressMeterIconTiles[gRacePlayers[order[i]].unk580 + (player->characterId * 6)]);
        }
        i--;
    } while (i >= 0);
}
#endif

// updateRaceCourseProgressMeter best match: 98.708% at nonmatchings/updateRaceCourseProgressMeter-1936695454966205676/base_8.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race/ui/race_hud/updateRaceCourseProgressMeter.s")

#ifdef NON_MATCHING
void updateRaceCourseProgressMeter(void) {
    s32 pathIndex;
    s32 slots[2];
    RacePlayer *player;
    s32 i;
    s32 progress;

    player = gRacePlayers; if (0) { } i = 0; do { getRacePlayerRankingProgress(i, &slots[1], &slots[0]); progress = slots[1]; i++;
        if (progress < 0) {
            progress = 0;
        }
        pathIndex = gRaceCourseStartEntries[gRaceCourseIndex].pathIndex;
        slots[1] = progress;
        player->unk57E = (progress << 7) / (pathIndex * 8);
        if (player->unk57E >= 0x81) {
            player->unk57E = 0x80;
        }
        player++;
    } while (i != 4);

    if (gRaceHudMode == 0) {
        addRenderCallback(&gRaceOverlayRenderCallbackList, drawRaceCourseProgressMeter, 0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawRaceCourseProgressMeter, 0);
}
#endif

void updateRaceHud(void) {
    s32 i;

    gRaceHudSpinnerFrame++;
    if (gRaceHudSpinnerFrame >= 12) {
        gRaceHudSpinnerFrame = 0;
    }

    switch (*(u16 *)&gRaceHudMode) {
    case 0:
        if (gRacePlayers[0].itemEffectPalette != 0) {
            if (gRacePlayers[0].itemEffectPalette == 4) {
                spawnRaceUiSparkle(-0x20, -0x60, 0, 0, 0);
            }
            gRacePlayers[0].itemEffectPalette--;
        }
        if (gRacePlayers[0].actionEffectPalette != 0) {
            if (gRacePlayers[0].actionEffectPalette == 4) {
                spawnRaceUiSparkle(0, -0x60, 0, 0, 1);
            }
            gRacePlayers[0].actionEffectPalette--;
        }
        addRenderCallback(&gRaceOverlayRenderCallbackList, drawSinglePlayerRaceHud, 0);
        addRenderCallback(&gRaceForegroundRenderCallbackList, noopRaceHudCallback, 0);
        updateRaceCourseProgressMeter();
        return;
    case 1:
        for (i = 0; i < 2; i++) {
            if (gRacePlayers[i].itemEffectPalette != 0) {
                if (gRacePlayers[i].itemEffectPalette == 4) {
                    spawnRaceUiSparkle(-0x88, -0x30, (s16)i, 0, 0);
                }
                gRacePlayers[i].itemEffectPalette--;
            }
            if (gRacePlayers[i].actionEffectPalette != 0) {
                if (gRacePlayers[i].actionEffectPalette == 4) {
                    spawnRaceUiSparkle(-0x68, -0x30, (s16)i, 0, 1);
                }
                gRacePlayers[i].actionEffectPalette--;
            }
        }
        addRenderCallback(&gRaceOverlayRenderCallbackList, drawTwoPlayerRaceHud, 0);
        addRenderCallback(&gRaceForegroundRenderCallbackList, drawTwoPlayerLapCounter, 0);
        updateRaceCourseProgressMeter();
        return;
    case 3:
        if (gRacePlayers[3].itemEffectPalette != 0) {
            if (gRacePlayers[3].itemEffectPalette == 4) {
                spawnRaceUiSparkle(-0x10, -0x30, 3, 1, 0);
            }
            gRacePlayers[3].itemEffectPalette--;
        }
        if (gRacePlayers[3].actionEffectPalette != 0) {
            if (gRacePlayers[3].actionEffectPalette == 4) {
                spawnRaceUiSparkle(0, -0x30, 3, 1, 1);
            }
            gRacePlayers[3].actionEffectPalette--;
        }
    case 2:
        for (i = 0; i < 3; i++) {
            if (gRacePlayers[i].itemEffectPalette != 0) {
                if (gRacePlayers[i].itemEffectPalette == 4) {
                    spawnRaceUiSparkle(-0x10, -0x30, (s16)i, 1, 0);
                }
                gRacePlayers[i].itemEffectPalette--;
            }
            if (gRacePlayers[i].actionEffectPalette != 0) {
                if (gRacePlayers[i].actionEffectPalette == 4) {
                    spawnRaceUiSparkle(0, -0x30, (s16)i, 1, 1);
                }
                gRacePlayers[i].actionEffectPalette--;
            }
        }
        addRenderCallback(&gRaceOverlayRenderCallbackList, drawMultiplayerRaceHud, 0);
        addRenderCallback(&gRaceForegroundRenderCallbackList, drawMultiplayerLapCounter, 0);
        updateRaceCourseProgressMeter();
        if ((s16)gRaceHudMode == 2) {
            addRenderCallback(&gMenuRenderCallbackList, drawThreePlayerHudDivider, 0);
        }
        return;
    case 4:
    {
        s32 sp40;
        s32 sp3C;

        if (!(gMenuFlowState & 3)) {
            incrementRaceElapsedTimer();
            if (gRaceTimeTrialFinishRecorded == 0) {
                getRacePlayerRankingProgress(0, &sp40, &sp3C);
                if ((gRaceCourseFinishLineData[gRaceCourseIndex].finishLinePathIndex * 8) < sp40) {
                    gRaceTimeTrialFinishTime = *(s32 *)&gRaceElapsedTimer;
                    gRaceTimeTrialFinishRecorded = 1;
                    createCallbackTask(initTimeTrialRecordDeltaPopup, 0, 0);
                }
            }
        }
        addRenderCallback(&gRaceOverlayRenderCallbackList, drawTimeTrialHud, 0);
        addRenderCallback(&gRaceForegroundRenderCallbackList, drawTimeTrialLabels, 0);
        return;
    }
    case 5:
    {
        RaceTimer sp38;

        if (!(gMenuFlowState & 3)) {
            incrementRaceElapsedTimer();
        }
        if (calculateRaceTimerDelta((RaceTimer *)&gRaceCourseTargetTimes[gRaceCourseIndex], &gRaceElapsedTimer, &sp38) != 0) {
            gRaceChallengeFailed = 1;
        }
        addRenderCallback(&gRaceOverlayRenderCallbackList, drawTargetTimeChallengeHud, 0);
        addRenderCallback(&gRaceForegroundRenderCallbackList, drawTargetTimeChallengeLabels, 0);
        return;
    }
    case 6:
        if (!(gMenuFlowState & 3)) {
            decrementRaceChallengeTimeLimit();
        }
        if ((gRaceChallengeTimeLimit.minutes == 0) && (gRaceChallengeTimeLimit.seconds == 0) && (gRaceChallengeTimeLimit.fraction == 0)) {
            gRaceChallengeFailed = 1;
        }
        addRenderCallback(&gRaceOverlayRenderCallbackList, drawScoreAttackChallengeHud, 0);
        addRenderCallback(&gRaceForegroundRenderCallbackList, drawScoreAttackChallengeLabels, 0);
        return;
    case 7:
        if (!(gMenuFlowState & 3)) {
            decrementRaceChallengeTimeLimit();
        }
        if ((gRaceChallengeTimeLimit.minutes == 0) && (gRaceChallengeTimeLimit.seconds == 0) && (gRaceChallengeTimeLimit.fraction == 0)) {
            gRaceChallengeFailed = 1;
        }
        addRenderCallback(&gRaceOverlayRenderCallbackList, drawTrickAttackChallengeHud, 0);
        addRenderCallback(&gRaceForegroundRenderCallbackList, drawTrickAttackChallengeLabels, 0);
        return;
    case 8:
        if (gRacePlayers[0].itemEffectPalette != 0) {
            if (gRacePlayers[0].itemEffectPalette == 4) {
                spawnRaceUiSparkle(-0x20, -0x60, 0, 0, 0);
            }
            gRacePlayers[0].itemEffectPalette--;
        }
        if (gRacePlayers[0].actionEffectPalette != 0) {
            if (gRacePlayers[0].actionEffectPalette == 4) {
                spawnRaceUiSparkle(0, -0x60, 0, 0, 1);
            }
            gRacePlayers[0].actionEffectPalette--;
        }
        addRenderCallback(&gRaceOverlayRenderCallbackList, drawTrainingRaceHud, 0);
        return;
    }
}

s32 calculateRaceTimerDelta(RaceTimer *arg0, RaceTimer *arg1, RaceTimer *arg2) {
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
