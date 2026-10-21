#include "common.h"
#include "game/race/items/race_items.h"
#include "game/race/race_state.h"
#include <PR/os_libc.h>
#include "game/save_data.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/engine/render_callback.h"
#include "assets.h"
#include "game/engine/asset_manager.h"
#include "game/engine/relocatable_heap.h"
#include "game/race/effects/race_visual_effects.h"
#include "game/race/player/race_player_movement.h"
#include "game/race/ui/race_hud.h"
#include "game/race/ui/race_ui_effects.h"
#include "game/race/player/race_player_input.h"
#include "game/engine/callback_task_scheduler.h"

#define RACE_HUD_MAIN_FONT_HANDLE (gAssetHandles[0x1C])
#define RACE_HUD_POPUP_FONT_HANDLE (gAssetHandles[0x1F])
#define ASSET_HANDLE(index) (gAssetHandles[index])

extern u16 gRaceProgressMeterIconTiles[36];

extern u16 gRaceProgressMeterIconPalettes[6];

extern s32 gMenuFlowState;

extern s16 gRaceLapCount;

extern u8 gTrainingCourseLesson;

extern u8 gMainMenuModeSelection;

extern RaceTimer gScoreAttackTimeLimitsByCourse[10];

extern RaceTimer gTrickAttackTimeLimit;

extern u8 gRaceTimeTrialFinishRecorded;

extern const char gRaceHudTrickAttackPointLabel[];
extern const char gRaceHudTrickAttackTimeLimitLabel[];
extern const char gRaceHudScoreAttackTimeLimitLabel[];
extern const char gRaceHudScoreAttackPointTargetFormat[];
extern const char gRaceHudTargetTimeChallengeSecondaryCounterFormat[];
extern const char gRaceHudTargetTimeChallengeFractionFormat[];
extern const char gRaceHudTargetTimeChallengeSecondsFormat[];
extern u8 gRaceChallengeFailed;
extern const char gRaceHudTargetTimeChallengeMinutesFormat[];
extern const char gRaceHudTargetTimeChallengeTargetTimeFormat[];
extern const char gRaceHudTargetTimeChallengeTargetTimeLabel[];
extern const char gRaceHudTargetTimeChallengeLapTimeLabel[];
extern const char gRaceHudTargetTimeChallengeLapProgressFormat[];
extern const char gRaceHudTimeTrialLapTimeLabel[];
extern const char gRaceHudTimeTrialBestLapFormat[];
extern const char gRaceHudTimeTrialBestLapLabel[];
extern u8 gRaceUpdatePaused;

void initRaceHud(void) {
    LOAD_ASSET(SHARED_UI_SPRITES, 0x1F);
    gRaceHudMode = RACE_HUD_MODE_NONE;
    gRaceHudCoinSpinnerFrame = 0;

    if (gPlayerCount == 1) {
        if (gRaceSplitscreenMode == 0) {
            gRaceHudMode = RACE_HUD_MODE_ONE_PLAYER;
        }
        if (gRaceSplitscreenMode == 2) {
            gRaceHudMode = RACE_HUD_MODE_TIME_TRIAL;
        }
        if (gRaceSplitscreenMode == 1) {
            if (gRaceTypeSelection == 0) {
                gRaceHudMode = RACE_HUD_MODE_TARGET_TIME_CHALLENGE;
            }
            if (gRaceTypeSelection == 1) {
                gRaceHudMode = RACE_HUD_MODE_SCORE_ATTACK;
                gRaceChallengeTimeLimit = gScoreAttackTimeLimitsByCourse[gRaceCourseIndex.signedValue];
            }
            if (gRaceTypeSelection == 2) {
                gRaceHudMode = RACE_HUD_MODE_TRICK_ATTACK;
                gRaceChallengeTimeLimit = gTrickAttackTimeLimit;
            }
        }
    } else {
        gRaceHudMode = gPlayerCount - 1;
    }

    if ((gTrainingCourseLesson != 0) || (gMainMenuModeSelection != 0)) {
        gRaceHudMode = RACE_HUD_MODE_TRAINING_OR_DEMO;
    }

    gRaceElapsedTimer.minutes = 0;
    gRaceElapsedTimer.seconds = 0;
    gRaceElapsedTimer.fraction = 0;
    gRaceTimeTrialFinishRecorded = 0;
    gRaceChallengeFailed = 0;
}

#pragma GLOBAL_ASM("asm.jp/nonmatchings/race/ui/race_hud/drawTrickAttackChallengeHud.s")

void drawTrickAttackChallengeLabels(void *arg0) {
    drawMenuAsciiTextDefaultScale(0x60, -0x61, gRaceHudTrickAttackPointLabel, 5);
    drawMenuAsciiTextDefaultScale(0x38, 0x47, gRaceHudTrickAttackTimeLimitLabel, 5);
}

#pragma GLOBAL_ASM("asm.jp/nonmatchings/race/ui/race_hud/drawScoreAttackChallengeHud.s")

void drawScoreAttackChallengeLabels(void *arg0) {
    char sp18[0x20];

    drawMenuAsciiTextDefaultScale(0x38, 0x47, gRaceHudScoreAttackTimeLimitLabel, 5);
    sprintf(sp18, gRaceHudScoreAttackPointTargetFormat, gRacePlayers[0].scoreAttackPointTarget);
    drawMenuAsciiTextDefaultScale(0x70, -0x48, sp18, 6);
}

void drawTargetTimeChallengeHud(void *arg0) {
    char buffer[12];
    s32 x;
    s32 i;

    sprintf(buffer, gRaceHudTargetTimeChallengeMinutesFormat, gRaceElapsedTimer.minutes);
    x = 0x48;
    for (i = 0; i < 2; i++) {
        drawAssetTableSprite((s16)x, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), (u8)buffer[i] - 5);
        x += 8;
    }

    x += 8;
    sprintf(buffer, gRaceHudTargetTimeChallengeSecondsFormat, gRaceElapsedTimer.seconds);
    for (i = 0; i < 2; i++) {
        drawAssetTableSprite((s16)x, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), (u8)buffer[i] - 5);
        x += 8;
    }

    x += 8;
    sprintf(buffer, gRaceHudTargetTimeChallengeFractionFormat, gRaceElapsedTimer.fraction >> 8);
    for (i = 0; i != 2; i++) {
        drawAssetTableSprite((s16)x, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), (u8)buffer[i] - 5);
        x += 8;
    }

    drawAssetTableSprite(0x58, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x36);
    drawAssetTableSprite(0x70, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x35);

    if (gRaceChallengeFailed != 0) {
        drawAssetTableSprite(0x48, 0x54, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x4F);
    }

    drawAssetTableSprite(-0x88, -0x60, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), gRaceHudItemTileIds[1]);
    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x24);
    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x25);

    sprintf(buffer, gRaceHudTargetTimeChallengeSecondaryCounterFormat, gRacePlayers[0].collectedCourseCoinMarkerCount);
    drawAssetTableSprite(-0x60, -0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), (u8)buffer[0] - 5);
    if (buffer[1] != 0) {
        drawAssetTableSprite(-0x58, -0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), (u8)buffer[1] - 5);
    }
}

void drawTargetTimeChallengeLabels(void *arg0) {
    char sp28[0x20];
    RaceTimer *targetTime;

    drawMenuAsciiTextDefaultScale(0x48, 0x47, gRaceHudTargetTimeChallengeLapTimeLabel, 5);
    drawMenuAsciiTextDefaultScale(0x32, -0x60, gRaceHudTargetTimeChallengeTargetTimeLabel, 7);
    targetTime = &gRaceCourseTargetTimes[gRaceCourseIndex.signedValue];
    sprintf(
        sp28,
        gRaceHudTargetTimeChallengeTargetTimeFormat,
        targetTime->minutes,
        targetTime->seconds,
        targetTime->fraction >> 8
    );
    drawMenuAsciiTextDefaultScale(0x48, -0x57, sp28, 7);
    sprintf(
        sp28,
        gRaceHudTargetTimeChallengeLapProgressFormat,
        gRacePlayers[0].collectedCourseCoinMarkerCount,
        gRacePlayers[0].totalCourseCoinMarkerCount
    );
    sp28[1] = ' ';
    if (sp28[2] != '/') {
        sp28[2] = ' ';
    }
    drawMenuAsciiTextDefaultScale(-0x68, -0x48, sp28, 6);
}

#pragma GLOBAL_ASM("asm.jp/nonmatchings/race/ui/race_hud/drawTimeTrialHud.s")

void drawTimeTrialLabels(void *unused) {
    char bestLapText[0x20];

    drawMenuAsciiTextDefaultScale(0x48, 0x47, gRaceHudTimeTrialLapTimeLabel, 5);
    drawMenuAsciiTextDefaultScale(0x48, -0x61, gRaceHudTimeTrialBestLapLabel, 7);
    sprintf(
        bestLapText,
        gRaceHudTimeTrialBestLapFormat,
        gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][0].minutes,
        gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][0].seconds,
        gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][0].fraction >> 8
    );
    drawMenuAsciiTextDefaultScale(0x48, -0x58, bestLapText, 7);
}

void drawSinglePlayerRaceHud(void *arg0) {
    s32 palette;
    s32 i;
    s32 var_s1;
    char buffer[0x20];

    sprintf(buffer, gRaceHudSinglePlayerScoreFormat, gRacePlayers[0].score);
    if (gRacePlayers[0].score < 100) {
        palette = 0x10;
    } else {
        palette = 0xE;
    }

    for (i = 0, var_s1 = 0x50; i < 5; i++, var_s1 += 8) {
        if (buffer[i] != ' ') {
            drawAssetTableSpriteWithExplicitPalette(
                var_s1,
                0x50,
                getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                (u8)buffer[i] - 5,
                palette
            );
        }
    }

    drawAssetTableSprite(
        0x78,
        0x50,
        getRelocatableHeapBlockBase(RACE_HUD_MAIN_FONT_HANDLE),
        (gRaceHudCoinSpinnerFrame >> 1) + 4
    );

    if (gRacePlayers[0].shotPalette != 0) {
        drawScaledAssetTableSprite(
            -0x20,
            -0x60,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudShotTileOffsets[gRacePlayers[0].shotType] + gRacePlayers[0].shotCount - 1,
            gRacePlayers[0].shotPalette
        );
    } else {
        drawAssetTableSprite(
            -0x20,
            -0x60,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudShotTileOffsets[gRacePlayers[0].shotType] + gRacePlayers[0].shotCount - 1
        );
    }

    if (gRacePlayers[0].itemPalette != 0) {
        drawScaledAssetTableSprite(
            0,
            -0x60,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudItemTileIds[gRacePlayers[0].itemType],
            gRacePlayers[0].itemPalette
        );
    } else {
        drawAssetTableSprite(
            0,
            -0x60,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudItemTileIds[gRacePlayers[0].itemType]
        );
    }

    drawAssetTableSprite(
        -0x88,
        0x40,
        getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
        gRacePlayers[0].rankIndex
    );
    drawAssetTableSprite(-0x88, -0x60, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x39);
    drawAssetTableSpriteWithExplicitPalette(
        -0x68,
        -0x60,
        getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
        gRacePlayers[0].lapDigit + 0x2C,
        0xE
    );
    drawAssetTableSprite(-0x5C, -0x60, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x38);
    drawAssetTableSpriteWithExplicitPalette(
        -0x50,
        -0x60,
        getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
        gRaceLapCount + 0x2B,
        0xE
    );
}

void noopRaceHudCallback(void *arg0) {
}

void drawTrainingRaceHud(void *arg0) {
    s32 palette;
    s32 i;
    s32 x;
    char buffer[0x20];

    sprintf(buffer, gRaceHudTrainingScoreFormat, gRacePlayers[0].score);
    if (gRacePlayers[0].score < 100) {
        palette = 0x10;
    } else {
        palette = 0xE;
    }

    for (i = 0, x = 0x50; i < 5; i++, x += 8) {
        if (buffer[i] != ' ') {
            drawAssetTableSpriteWithExplicitPalette(
                x,
                0x50,
                getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                (u8)buffer[i] - 5,
                palette
            );
        }
    }

    drawAssetTableSprite(
        0x78,
        0x50,
        getRelocatableHeapBlockBase(RACE_HUD_MAIN_FONT_HANDLE),
        (gRaceHudCoinSpinnerFrame >> 1) + 4
    );

    if (gRacePlayers[0].shotPalette != 0) {
        drawScaledAssetTableSprite(
            -0x20,
            -0x60,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudShotTileOffsets[gRacePlayers[0].shotType] + gRacePlayers[0].shotCount - 1,
            gRacePlayers[0].shotPalette
        );
    } else {
        drawAssetTableSprite(
            -0x20,
            -0x60,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudShotTileOffsets[gRacePlayers[0].shotType] + gRacePlayers[0].shotCount - 1
        );
    }

    if (gRacePlayers[0].itemPalette != 0) {
        drawScaledAssetTableSprite(
            0,
            -0x60,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudItemTileIds[gRacePlayers[0].itemType],
            gRacePlayers[0].itemPalette
        );
    } else {
        drawAssetTableSprite(
            0,
            -0x60,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudItemTileIds[gRacePlayers[0].itemType]
        );
    }

    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x24);
    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x29);
}

#pragma GLOBAL_ASM("asm.jp/nonmatchings/race/ui/race_hud/drawTwoPlayerRaceHud.s")

void drawTwoPlayerLapCounter(void *arg0) {
    s32 y;
    s32 viewportIndex;

    viewportIndex = gCurrentViewportIndex;
    if (viewportIndex == 0) {
        y = -0x30;
    } else {
        y = 0x2A;
    }

    drawMenuAsciiChar(0x70, (s16)y, gRacePlayers[viewportIndex].lapDigit + '1', 2);
    drawMenuAsciiChar(0x78, (s16)y, '/', 2);
    drawMenuAsciiChar(0x80, (s16)y, gRaceLapCount + '0', 2);
}

void drawMultiplayerRaceHud(void *arg0) {
    RacePlayer *player;
    AssetTable *texture;

    drawScaledAssetTableSprite(
        0x38,
        0x24,
        getRelocatableHeapBlockBase(RACE_HUD_MAIN_FONT_HANDLE),
        (gRaceHudCoinSpinnerFrame >> 1) + 4,
        1
    );

    texture = getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE);
    player = &gRacePlayers[gCurrentViewportIndex];
    drawScaledAssetTableSprite(
        -0x18,
        -0x38,
        texture,
        gRaceHudShotTileOffsets[gRacePlayers[gCurrentViewportIndex].shotType] +
            gRacePlayers[gCurrentViewportIndex].shotCount - 1,
        player->shotPalette + 1
    );

    texture = getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE);
    player = &gRacePlayers[gCurrentViewportIndex];
    drawScaledAssetTableSprite(-8, -0x38, texture, gRaceHudItemTileIds[player->itemType], player->itemPalette + 1);

    texture = getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE);
    drawScaledAssetTableSprite(-0x4C, 0x18, texture, gRacePlayers[gCurrentViewportIndex].rankIndex, 1);

    if (gCurrentViewportIndex < 2) {
        drawAssetTableSprite(-0x44, -0x30, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x1A);
        return;
    }
    drawAssetTableSprite(0x14, -0x30, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x1A);
}

void drawMultiplayerScoreAndLapCounter(void *arg0) {
    s32 x;
    s32 palette;
    char buffer[0x20];

    sprintf(buffer, gRaceHudMultiplayerScoreFormat, gRacePlayers[gCurrentViewportIndex].score);
    if (gRacePlayers[gCurrentViewportIndex].score < 0x64) {
        palette = 1;
    } else {
        palette = 2;
    }
    drawMenuAsciiTextDefaultScale(0x14, 0x28, buffer, palette);

    if (gCurrentViewportIndex < 2) {
        x = -0x2C;
    } else {
        x = 0x2C;
    }

    drawMenuAsciiChar((s16)x, -0x30, gRacePlayers[gCurrentViewportIndex].lapDigit + '1', 2);
    drawMenuAsciiChar((s16)(x + 8), -0x30, 0x2F, 2);
    drawMenuAsciiChar((s16)(x + 0x10), -0x30, gRaceLapCount + 0x30, 2);
}

void drawThreePlayerNoEntryPanel(void *arg0) {
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

void drawRaceCourseProgressMeter(void *arg0) {
    s32 i;
    s32 j;
    union {
        s32 playerId;
        AssetTable *assetTable;
    } temp;
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
                temp.playerId = order[i];
                order[i] = order[j];
                order[j] = temp.playerId;
            }
        }
    }

    if (gRaceHudMode == RACE_HUD_MODE_ONE_PLAYER) {
        xBase = 0x78;
        yBase = -0x56;
    }
    if (gRaceHudMode == RACE_HUD_MODE_TWO_PLAYER) {
        xBase = 0x78;
        yBase = -0x48;
    }
    if ((gRaceHudMode == RACE_HUD_MODE_THREE_PLAYER) || (gRaceHudMode == RACE_HUD_MODE_FOUR_PLAYER)) {
        xBase = -8;
        yBase = -0x48;
    }

    drawAssetTableSprite(
        (s16)(xBase + 4),
        (s16)(yBase + 4),
        getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
        0x50
    );

    i = 3;
    while (i >= 0) {
        if (((gRacePlayers[order[i]].stateFlags & RACE_PLAYER_COLLISION_SQUASHED) != 0) ||
            (gRacePlayers[order[i]].progressMeterSquashFrame != 0)) {
            gRacePlayers[order[i]].progressMeterSquashFrame++;
        }
        if (((gRacePlayers[order[i]].stateFlags & RACE_PLAYER_COLLISION_SQUASHED) != 0) &&
            (gRacePlayers[order[i]].progressMeterSquashFrame >= 5)) {
            gRacePlayers[order[i]].progressMeterSquashFrame = 4;
        }
        if (gRacePlayers[order[i]].progressMeterSquashFrame >= 6) {
            gRacePlayers[order[i]].progressMeterSquashFrame = 0;
        }

        if (gRacePlayers[order[i]].progressMeterSquashFrame != 0) {
            if (gRacePlayers[order[i]].activeSparkleEffectCount != 0) {
                temp.assetTable = getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE);
                drawAssetTableSpriteWithExplicitPalette(
                    (s16)(xBase - 8),
                    (s16)(gRacePlayers[order[i]].progressMeterPosition + yBase),
                    temp.assetTable,
                    (
                        &gRaceProgressMeterIconTiles[gRacePlayers[order[i]].progressMeterSquashFrame]
                    )[gRacePlayers[order[i]].characterId * 6],
                    gRaceProgressMeterIconPalettes[gRacePlayers[order[i]].characterId]
                );
            } else {
                temp.assetTable = getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE);
                drawAssetTableSprite(
                    (s16)(xBase - 8),
                    (s16)(gRacePlayers[order[i]].progressMeterPosition + yBase),
                    temp.assetTable,
                    (
                        &gRaceProgressMeterIconTiles[gRacePlayers[order[i]].progressMeterSquashFrame]
                    )[gRacePlayers[order[i]].characterId * 6]
                );
            }
        } else if (gRacePlayers[order[i]].activeSparkleEffectCount != 0) {
            temp.assetTable = getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE);
            drawAssetTableSpriteWithExplicitPalette(
                xBase,
                (s16)(gRacePlayers[order[i]].progressMeterPosition + yBase),
                temp.assetTable,
                (
                    &gRaceProgressMeterIconTiles[gRacePlayers[order[i]].progressMeterSquashFrame]
                )[gRacePlayers[order[i]].characterId * 6],
                gRaceProgressMeterIconPalettes[gRacePlayers[order[i]].characterId]
            );
        } else {
            temp.assetTable = getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE);
            drawAssetTableSprite(
                xBase,
                (s16)(gRacePlayers[order[i]].progressMeterPosition + yBase),
                temp.assetTable,
                (
                    &gRaceProgressMeterIconTiles[gRacePlayers[order[i]].progressMeterSquashFrame]
                )[gRacePlayers[order[i]].characterId * 6]
            );
        }
        i--;
    }
}

void updateRaceCourseProgressMeter(void) {
    s32 meterPosition;
    s32 slots[2];
    s32 i;

    for (i = 0; i < 4; i++) {
        getRacePlayerRankingProgress(i, &slots[1], &slots[0]);
        if (slots[1] < 0) {
            slots[1] = 0;
        }
        gRacePlayers[i].progressMeterPosition =
            (slots[1] << 7) / (gRaceCourseStartEntries[gRaceCourseIndex.signedValue].pathIndex * 8);
        if (gRacePlayers[i].progressMeterPosition >= 0x81) {
            gRacePlayers[i].progressMeterPosition = 0x80;
        }
    }

    if (gRaceHudMode == RACE_HUD_MODE_ONE_PLAYER) {
        addRenderCallback(&gRaceOverlayRenderCallbackList, drawRaceCourseProgressMeter, 0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawRaceCourseProgressMeter, 0);
}

void updateRaceHud(void) {
    s32 i;

    gRaceHudCoinSpinnerFrame++;
    if (gRaceHudCoinSpinnerFrame >= 12) {
        gRaceHudCoinSpinnerFrame = 0;
    }

    switch (*(u16 *)&gRaceHudMode) {
        case RACE_HUD_MODE_ONE_PLAYER:
            if (gRacePlayers[0].shotPalette != 0) {
                if (gRacePlayers[0].shotPalette == 4) {
                    spawnRaceUiSparkle(-0x20, -0x60, 0, 0, 0);
                }
                gRacePlayers[0].shotPalette--;
            }
            if (gRacePlayers[0].itemPalette != 0) {
                if (gRacePlayers[0].itemPalette == 4) {
                    spawnRaceUiSparkle(0, -0x60, 0, 0, 1);
                }
                gRacePlayers[0].itemPalette--;
            }
            addRenderCallback(&gRaceOverlayRenderCallbackList, drawSinglePlayerRaceHud, 0);
            addRenderCallback(&gRaceForegroundRenderCallbackList, noopRaceHudCallback, 0);
            updateRaceCourseProgressMeter();
            return;
        case RACE_HUD_MODE_TWO_PLAYER:
            for (i = 0; i < 2; i++) {
                if (gRacePlayers[i].shotPalette != 0) {
                    if (gRacePlayers[i].shotPalette == 4) {
                        spawnRaceUiSparkle(-0x88, -0x30, (s16)i, 0, 0);
                    }
                    gRacePlayers[i].shotPalette--;
                }
                if (gRacePlayers[i].itemPalette != 0) {
                    if (gRacePlayers[i].itemPalette == 4) {
                        spawnRaceUiSparkle(-0x68, -0x30, (s16)i, 0, 1);
                    }
                    gRacePlayers[i].itemPalette--;
                }
            }
            addRenderCallback(&gRaceOverlayRenderCallbackList, drawTwoPlayerRaceHud, 0);
            addRenderCallback(&gRaceForegroundRenderCallbackList, drawTwoPlayerLapCounter, 0);
            updateRaceCourseProgressMeter();
            return;
        case RACE_HUD_MODE_FOUR_PLAYER:
            if (gRacePlayers[3].shotPalette != 0) {
                if (gRacePlayers[3].shotPalette == 4) {
                    spawnRaceUiSparkle(-0x10, -0x30, 3, 1, 0);
                }
                gRacePlayers[3].shotPalette--;
            }
            if (gRacePlayers[3].itemPalette != 0) {
                if (gRacePlayers[3].itemPalette == 4) {
                    spawnRaceUiSparkle(0, -0x30, 3, 1, 1);
                }
                gRacePlayers[3].itemPalette--;
            }
        case RACE_HUD_MODE_THREE_PLAYER:
            for (i = 0; i < 3; i++) {
                if (gRacePlayers[i].shotPalette != 0) {
                    if (gRacePlayers[i].shotPalette == 4) {
                        spawnRaceUiSparkle(-0x10, -0x30, (s16)i, 1, 0);
                    }
                    gRacePlayers[i].shotPalette--;
                }
                if (gRacePlayers[i].itemPalette != 0) {
                    if (gRacePlayers[i].itemPalette == 4) {
                        spawnRaceUiSparkle(0, -0x30, (s16)i, 1, 1);
                    }
                    gRacePlayers[i].itemPalette--;
                }
            }
            addRenderCallback(&gRaceOverlayRenderCallbackList, drawMultiplayerRaceHud, 0);
            addRenderCallback(&gRaceForegroundRenderCallbackList, drawMultiplayerScoreAndLapCounter, 0);
            updateRaceCourseProgressMeter();
            if ((s16)gRaceHudMode == RACE_HUD_MODE_THREE_PLAYER) {
                addRenderCallback(&gMenuRenderCallbackList, drawThreePlayerNoEntryPanel, 0);
            }
            return;
        case RACE_HUD_MODE_TIME_TRIAL: {
            s32 sp40;
            s32 sp3C;

            if (!(gMenuFlowState & 3)) {
                incrementRaceElapsedTimer();
                if (gRaceTimeTrialFinishRecorded == 0) {
                    getRacePlayerRankingProgress(0, &sp40, &sp3C);
                    if ((gRaceCourseStartEntries[gRaceCourseIndex.signedValue].finishLinePathIndex * 8) < sp40) {
                        gRaceTimeTrialFinishTime = gRaceElapsedTimer;
                        gRaceTimeTrialFinishRecorded = 1;
                        createCallbackTask((CallbackTaskCallback)initTimeTrialRecordDeltaPopup, 0, 0);
                    }
                }
            }
            addRenderCallback(&gRaceOverlayRenderCallbackList, drawTimeTrialHud, 0);
            addRenderCallback(&gRaceForegroundRenderCallbackList, drawTimeTrialLabels, 0);
            return;
        }
        case RACE_HUD_MODE_TARGET_TIME_CHALLENGE: {
            RaceTimer sp38;

            if (!(gMenuFlowState & 3)) {
                incrementRaceElapsedTimer();
            }
            if (calculateRaceTimerDelta(
                    &gRaceCourseTargetTimes[gRaceCourseIndex.signedValue],
                    &gRaceElapsedTimer,
                    &sp38
                ) != 0) {
                gRaceChallengeFailed = 1;
            }
            addRenderCallback(&gRaceOverlayRenderCallbackList, drawTargetTimeChallengeHud, 0);
            addRenderCallback(&gRaceForegroundRenderCallbackList, drawTargetTimeChallengeLabels, 0);
            return;
        }
        case RACE_HUD_MODE_SCORE_ATTACK:
            if (!(gMenuFlowState & 3)) {
                decrementRaceChallengeTimeLimit();
            }
            if ((gRaceChallengeTimeLimit.minutes == 0) && (gRaceChallengeTimeLimit.seconds == 0) &&
                (gRaceChallengeTimeLimit.fraction == 0)) {
                gRaceChallengeFailed = 1;
            }
            addRenderCallback(&gRaceOverlayRenderCallbackList, drawScoreAttackChallengeHud, 0);
            addRenderCallback(&gRaceForegroundRenderCallbackList, drawScoreAttackChallengeLabels, 0);
            return;
        case RACE_HUD_MODE_TRICK_ATTACK:
            if (!(gMenuFlowState & 3)) {
                decrementRaceChallengeTimeLimit();
            }
            if ((gRaceChallengeTimeLimit.minutes == 0) && (gRaceChallengeTimeLimit.seconds == 0) &&
                (gRaceChallengeTimeLimit.fraction == 0)) {
                gRaceChallengeFailed = 1;
            }
            addRenderCallback(&gRaceOverlayRenderCallbackList, drawTrickAttackChallengeHud, 0);
            addRenderCallback(&gRaceForegroundRenderCallbackList, drawTrickAttackChallengeLabels, 0);
            return;
        case RACE_HUD_MODE_TRAINING_OR_DEMO:
            if (gRacePlayers[0].shotPalette != 0) {
                if (gRacePlayers[0].shotPalette == 4) {
                    spawnRaceUiSparkle(-0x20, -0x60, 0, 0, 0);
                }
                gRacePlayers[0].shotPalette--;
            }
            if (gRacePlayers[0].itemPalette != 0) {
                if (gRacePlayers[0].itemPalette == 4) {
                    spawnRaceUiSparkle(0, -0x60, 0, 0, 1);
                }
                gRacePlayers[0].itemPalette--;
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
