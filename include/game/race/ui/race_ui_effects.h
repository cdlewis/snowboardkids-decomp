#ifndef RACE_UI_EFFECTS_H
#define RACE_UI_EFFECTS_H

#include "common.h"
#include "game/race/player/race_player_input.h"
#include "game/math/spatial_math.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/race/ui/race_hud.h"

#define DIZZY_LAND_TEACUP_BUMPER_BASE_SCALE 0x1000
#define DIZZY_LAND_TEACUP_BUMPER_ROTATION_STEP 0x40

/* Persistent Dizzy Land teacup; task.userId selects one of ten positions.
 * xzScale affects X/Z only; yaw and yawStep are signed 16-bit angles. */
typedef struct DizzyLandTeacupBumperActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ s16 xzScale;
    /* 0x26 */ u8 pad26[2];
    /* 0x28 */ Mtx *matrix;
    /* 0x2C */ s16 yaw;
    /* 0x2E */ s16 yawStep;
    /* 0x30 */ u8 matrixDirty;
} DizzyLandTeacupBumperActor;

typedef char DizzyLandTeacupBumperActorSizeCheck[(sizeof(DizzyLandTeacupBumperActor) == 0x34) ? 1 : -1];

/*
 * Persistent course models use task.userId to select their course instance. They
 * have different matrix/angle layouts, and their angles are not XYZ vectors.
 */
/* Peaked canopy and two alternating groups of horse-and-pole quads. */
typedef struct DizzyLandCarouselActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Mtx *canopyMatrix;
    /* 0x28 */ Mtx *negativeSineHorsesMatrix;
    /* 0x2C */ Mtx *positiveSineHorsesMatrix;
    /* 0x30 */ s16 yaw;
    /* 0x32 */ u8 matrixDirty;
} DizzyLandCarouselActor;

/* Upright wheel rotating about its hub above a stationary support. */
typedef struct DizzyLandFerrisWheelActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Mtx *supportMatrix;
    /* 0x28 */ Mtx *wheelMatrix;
    /* 0x2C */ s16 yaw;
    /* 0x2E */ s16 wheelAngle;
    /* 0x30 */ u8 matrixDirty;
} DizzyLandFerrisWheelActor;

/* Pink pole and rotating canopy carrying face-decorated cabins. */
typedef struct DizzyLandSpinningCabinRideActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Mtx *poleMatrix;
    /* 0x28 */ Mtx *canopyMatrix;
    /* 0x2C */ s16 baseYaw;
    /* 0x2E */ s16 spinAngle;
    /* 0x30 */ s16 pitchPhase;
    /* 0x32 */ s16 rollPhase;
    /* 0x34 */ u8 matrixDirty;
} DizzyLandSpinningCabinRideActor;

typedef union {
    s32 word;
    struct {
        s16 hi;
        s16 lo;
    } half;
} SplitWord;

typedef struct {
    /* 0x00 */ u8 pad[0x10];
} RaceUiProjectileVertexBlock;

typedef struct RaceUiProjectileActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Vec3i velocity;
    /* 0x30 */ u16 *animationScript;
    /* 0x34 */ s16 frameTimer;
    /* 0x36 */ s16 flags;
    /* 0x38 */ Mtx *matrix;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ u8 pad3E[2];
    /* 0x40 */ void *palette;
    /* 0x44 */ void *image;
    /* 0x48 */ u8 pad48[4];
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 verticalVelocity;
    /* 0x54 */ s32 verticalAcceleration;
    /* 0x58 */ u8 matrixDirty;
} RaceUiProjectileActor;

typedef enum RaceCourseScrollingTextureId {
    RACE_COURSE_SCROLLING_TEXTURE_QUICKSAND_VALLEY_0,
    RACE_COURSE_SCROLLING_TEXTURE_QUICKSAND_VALLEY_1,
    RACE_COURSE_SCROLLING_TEXTURE_NIGHT_HIGHWAY,
    RACE_COURSE_SCROLLING_TEXTURE_ANIMAL_LAND,
    RACE_COURSE_SCROLLING_TEXTURE_DIZZY_LAND_0,
    RACE_COURSE_SCROLLING_TEXTURE_DIZZY_LAND_1,
    RACE_COURSE_SCROLLING_TEXTURE_SUNSET_ROCK_0,
    RACE_COURSE_SCROLLING_TEXTURE_SUNSET_ROCK_1,
    RACE_COURSE_SCROLLING_TEXTURE_COUNT
} RaceCourseScrollingTextureId;

/* Image/palette pointers follow the generic asset-loader void ** contract. */
typedef struct RaceUiOverlayActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s32 velocity;
    /* 0x2C */ u8 pad2C[4];
    /* 0x30 */ s16 timer;
    /* 0x32 */ s16 assetTimer;
    /* 0x34 */ Mtx *matrix;
    /* 0x38 */ void *panelAPalette;
    /* 0x3C */ void *panelAImage;
    /* 0x40 */ void *panelBPalette;
    /* 0x44 */ void *panelBImage;
    /* 0x48 */ u8 matrixDirty;
} RaceUiOverlayActor;

typedef char RaceUiOverlayActorSizeCheck[(sizeof(RaceUiOverlayActor) == 0x4C) ? 1 : -1];

typedef struct RaceUiFadingImpactActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Transform3D copyBlock;
    /* 0x38 */ Vec3i pos;
    /* 0x44 */ Mtx *matrix;
    /* 0x48 */ s16 angle;
    /* 0x4A */ s16 alpha;
    /* 0x4C */ s16 scale;
    /* 0x4E */ u8 matrixDirty;
} RaceUiFadingImpactActor;

typedef struct RaceUiTransitionActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ u8 pad18[0x24 - 0x18];
    /* 0x24 */ Transform3D transformSource;
    /* 0x44 */ u8 pad44[0x64 - 0x44];
    /* 0x64 */ Mtx *matrix;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s16 unk6C;
    /* 0x6E */ s16 unk6E;
    /* 0x70 */ u8 pad70[2];
    /* 0x72 */ u8 matrixDirty;
} RaceUiTransitionActor;

typedef struct RaceCourseScrollingTextureActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ void *texture;
    /* 0x1C */ void *palette;
    /* 0x20 */ s16 textureScrollOffset;
    /* 0x22 */ u8 pad22[2];
    /* 0x24 */ Gfx *displayList;
    /* 0x28 */ s32 useOverlayQueue;
    /* 0x2C */ s32 soundSourceIndex;
} RaceCourseScrollingTextureActor;

typedef struct RaceUiCounterActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 alpha;
    /* 0x1A */ s16 timer;
    /* 0x1C */ s16 state;
    union {
        /* 0x1E */ s16 value;
        /* 0x1E */ s16 pendingHitPrize;
    };
    union {
        /* 0x20 */ s16 bonus;
        /* 0x20 */ s16 pendingPerfectHitBonus;
    };
    union {
        /* 0x22 */ s16 target;
        /* 0x22 */ s16 pendingCompleteBonus;
    };
    union {
        /* 0x24 */ s16 flag;
        /* 0x24 */ s16 hasPerfectHitBonus;
    };
} RaceUiCounterActor;

typedef struct RaceUiDualCounterActor {
    /* 0x00 */ CallbackTaskHeader task;
    union {
        struct {
            /* 0x18 */ s8 row;
            /* 0x19 */ s8 column;
        };
        /* 0x18 */ s16 alpha18;
    };
    /* 0x1A */ s16 timeFraction;
    /* 0x1C */ s16 alpha;
    /* 0x1E */ s16 timer;
    /* 0x20 */ s16 state;
    /* 0x22 */ s16 leftValue;
    /* 0x24 */ s16 bonus;
    /* 0x26 */ s16 leftTarget;
    /* 0x28 */ s16 rightValue;
    /* 0x2A */ s16 flag;
} RaceUiDualCounterActor;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
} RaceUiEffectParticle;

typedef struct RaceUiExpiredSpeedFanActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ u8 pad18[0x24 - 0x18];
    /* 0x24 */ Vec3i scale;
    /* 0x30 */ Vec3i worldPos;
    /* 0x3C */ s32 velocityY;
    /* 0x40 */ Transform3D frontTransform;
    /* 0x60 */ Transform3D backTransform;
    /* 0x80 */ s16 playerIndex;
    /* 0x82 */ u8 pad82[2];
    /* 0x84 */ s16 spinYaw;
    /* 0x86 */ u8 pad86[2];
    /* 0x88 */ Mtx *frontMatrix;
    /* 0x8C */ Mtx *backMatrix;
    /* 0x90 */ s16 scaleStep;
    /* 0x92 */ s16 timer;
    /* 0x94 */ u8 matrixDirty;
} RaceUiExpiredSpeedFanActor;

typedef struct RaceUiRankParticleActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Transform3D copyBlock;
    /* 0x44 */ void *matrix;
    /* 0x48 */ u32 *displayLists[2];
    /* 0x50 */ u8 matrixDirty;
} RaceUiRankParticleActor;

typedef struct RaceUiPodiumTrailActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Transform3D copyBlock;
    /* 0x44 */ Mtx *matrix;
    /* 0x48 */ s32 height;
    /* 0x4C */ s32 velocity;
    /* 0x50 */ s16 playerIndex;
    /* 0x52 */ s16 targetPlayerIndex;
    /* 0x54 */ s16 state;
    /* 0x56 */ s16 timer;
    /* 0x58 */ u8 matrixDirty;
    /* 0x59 */ u8 playImpactSound;
} RaceUiPodiumTrailActor;

typedef struct RaceUiRankTrailActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Transform3D copyBlock;
    /* 0x44 */ Mtx *matrix;
    /* 0x48 */ s16 scale;
    /* 0x4A */ u8 matrixDirty;
} RaceUiRankTrailActor;

typedef struct RaceUiTextParticleActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Mtx *matrix;
    /* 0x28 */ void *palettes[4];
    /* 0x38 */ void *images[4];
    /* 0x48 */ Vec3i velocity;
    /* 0x54 */ s16 timer;
    /* 0x56 */ u8 matrixDirty;
    /* 0x57 */ u8 useAltTextures;
} RaceUiTextParticleActor;

typedef struct RaceUiThrownTrailActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ s16 surface;
    /* 0x26 */ s16 targetAngle;
    /* 0x28 */ s16 angle;
    /* 0x2A */ s16 angleStep;
    /* 0x2C */ s16 spin;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ Mtx *matrix;
    /* 0x34 */ s16 soundTimer;
    /* 0x36 */ u8 matrixDirty;
} RaceUiThrownTrailActor;

typedef struct RaceUiSingleTrailActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ u8 pad18[0x24 - 0x18];
    /* 0x24 */ Transform3D copyBlock;
    /* 0x44 */ Transform3D localTransform;
    /* 0x64 */ Mtx *matrix;
    /* 0x68 */ s16 timer;
    /* 0x6A */ u8 matrixDirty;
} RaceUiSingleTrailActor;

typedef struct RaceUiFadingTrailActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Transform3D copyBlock;
    /* 0x38 */ Vec3i pos;
    /* 0x44 */ Mtx *matrix;
    /* 0x48 */ u8 pad48[2];
    /* 0x4A */ s16 alpha;
    /* 0x4C */ u8 pad4C[2];
    /* 0x4E */ u8 matrixDirty;
} RaceUiFadingTrailActor;

typedef struct RaceUiEffectParticleActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ RaceUiEffectParticle *particles;
    /* 0x1C */ void *unk1C;
    /* 0x20 */ void *unk20;
    /* 0x24 */ s16 count;
} RaceUiEffectParticleActor;

typedef struct RaceUiGfxCommandActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Mtx *particles;
    /* 0x1C */ s16 textureOffset;
    /* 0x1E */ s16 count;
} RaceUiGfxCommandActor;

typedef struct RaceUiRankTextRenderActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Mtx *matrices;
    /* 0x1C */ s16 count;
} RaceUiRankTextRenderActor;

typedef struct RaceUiPopupActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ u16 timer;
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ s32 x;
    /* 0x20 */ SplitWord y;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s32 velocity;
    /* 0x2C */ void *parent;
    /* 0x30 */ s8 playerIndex;
} RaceUiPopupActor;

typedef struct RaceUiTimeTrialRecordDeltaPopupActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ u16 timer;
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ s32 x;
    /* 0x20 */ SplitWord y;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s32 velocity;
    /* 0x2C */ RaceTimer delta;
    /* 0x30 */ s8 isSlowerThanRecord;
} RaceUiTimeTrialRecordDeltaPopupActor;

typedef struct RaceUiPromptActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ u32 timer;
    /* 0x1C */ s16 y;
} RaceUiPromptActor;

typedef struct RaceUiCourseStatsActor {
    /* 0x00 */ CallbackTaskHeader task;
    union {
        /* 0x18 */ s16 alpha;
        /* 0x18 */ s16 titleAlpha;
    };
    union {
        /* 0x1A */ s16 timer;
        /* 0x1A */ s16 statsAlpha;
    };
    union {
        /* 0x1C */ s16 visibleRows;
        /* 0x1C */ s16 holdTimer;
    };
    /* 0x1E */ s16 pendingTrickPrize;
    /* 0x20 */ s16 pendingMakeBonus;
    /* 0x22 */ s16 pendingCompleteBonus;
} RaceUiCourseStatsActor;

typedef struct RaceUiAlphaActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ u8 pad18[0x1C - 0x18];
    /* 0x1C */ s16 alpha;
} RaceUiAlphaActor;

typedef struct RaceUiAlpha1AActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ u8 pad18[0x1A - 0x18];
    /* 0x1A */ s16 alpha;
} RaceUiAlpha1AActor;

typedef struct RaceUiAlpha18Actor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 alpha;
    /* 0x1A */ s16 timer;
    /* 0x1C */ s16 score;
} RaceUiAlpha18Actor;

typedef struct RaceUiPrizePayoutActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 alpha;
    /* 0x1A */ s16 timer;
    /* 0x1C */ s16 prizeAmount;
} RaceUiPrizePayoutActor;

typedef struct RaceUiResultsBannerActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 alpha;
    /* 0x1A */ s16 timer;
    union {
        struct {
            /* 0x1C */ s8 player0;
            /* 0x1D */ s8 player1;
            /* 0x1E */ s8 player2;
            /* 0x1F */ s8 player3;
        };
        /* 0x1C */ s8 playerIndices[4];
    };
} RaceUiResultsBannerActor;

typedef struct RaceUiOrbitingSpriteActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Mtx *matrix;
    /* 0x28 */ s16 angle;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ void *palette;
    /* 0x30 */ void *image;
    /* 0x34 */ u8 matrixDirty;
    /* 0x35 */ u8 playerIndex;
} RaceUiOrbitingSpriteActor;

typedef struct RaceUiSparkleActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Mtx *matrix;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ void *palettes[4];
    /* 0x3C */ void *images[4];
    /* 0x4C */ s32 zOffset;
    /* 0x50 */ s16 alpha;
    /* 0x52 */ s16 alphaStep;
    /* 0x54 */ s16 timer;
    /* 0x56 */ s16 scale;
    /* 0x58 */ u8 matrixDirty;
    /* 0x59 */ u8 playerIndex;
    /* 0x5A */ u8 frame;
} RaceUiSparkleActor;

typedef struct RaceUiCourseSpriteActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s32 x;
    /* 0x1C */ s32 y;
    /* 0x20 */ s32 z;
    /* 0x24 */ u8 pad24[0x48 - 0x24];
    /* 0x48 */ Vtx *vertices;
    /* 0x4C */ s16 angle;
    /* 0x4E */ u8 pad4E[0x50 - 0x4E];
    /* 0x50 */ void *images[2];
    /* 0x58 */ void *palettes[2];
    /* 0x60 */ u8 pad60[0x62 - 0x60];
    /* 0x62 */ u8 matrixDirty;
} RaceUiCourseSpriteActor;

extern Vec3i gDizzyLandTeacupBumperPositions[10];
extern Vec3i gDizzyLandFerrisWheelHubOffset;
extern RaceUiProjectileVertexBlock D_800D64A0[8];
extern Gfx gAlphaSpriteRenderModeDl[];

void renderRaceCourseScrollingTexture(RaceCourseScrollingTextureActor *arg0);
void updateRaceCourseScrollingTexture(RaceCourseScrollingTextureActor *arg0);
void initRaceCourseScrollingTexture(RaceCourseScrollingTextureActor *arg0);
void drawRaceUiBoardReversePrompt(RaceUiPromptActor *arg0);
void updateRaceUiBoardReversePrompt(RaceUiPromptActor *arg0);
void initRaceUiBoardReversePrompt(RaceUiPromptActor *arg0);
void drawRaceUiTrickScorePopup(RaceUiPopupActor *arg0);
void updateRaceUiTrickScorePopupSlideOut(RaceUiPopupActor *arg0);
void updateRaceUiTrickScorePopupHold(RaceUiPopupActor *arg0);
void updateRaceUiTrickScorePopupSlideIn(RaceUiPopupActor *arg0);
void initRaceUiTrickScorePopup(RaceUiPopupActor *arg0);
void spawnRaceUiTrickScorePopup(void *arg0, s16 arg1);
void drawRaceUiCrashScorePopup(RaceUiPopupActor *arg0);
void updateRaceUiCrashScorePopupSlideOut(RaceUiPopupActor *arg0);
void updateRaceUiCrashScorePopupHold(RaceUiPopupActor *arg0);
void updateRaceUiCrashScorePopupSlideIn(RaceUiPopupActor *arg0);
void initRaceUiCrashScorePopup(RaceUiPopupActor *arg0);
void drawRaceUiPrizePayoutContinuePrompt(void *arg0);
void updateRaceUiPrizePayoutContinuePrompt(void *arg0);
void drawRaceUiPrizePayoutRankPanel(RaceUiAlpha18Actor *arg0);
void drawRaceUiPrizePayoutRankPrize(RaceUiAlpha18Actor *arg0);
void drawRaceUiPrizePayoutCollectedCoins(void *arg0);
void drawRaceUiPrizePayoutTotalMoney(void *arg0);
void drawRaceUiPrizePayoutLabels(RaceUiAlpha18Actor *arg0);
void updateRaceUiPrizePayoutConfirmed(void *arg0);
void updateRaceUiPrizePayoutWaitForConfirm(void *arg0);
void updateRaceUiPrizePayoutCountMoney(RaceUiAlpha18Actor *arg0);
void updateRaceUiPrizePayoutWaitBeforeMoneyCount(RaceUiPrizePayoutActor *arg0);
void updateRaceUiPrizePayoutRevealGetCoin(RaceUiPrizePayoutActor *arg0);
void updateRaceUiPrizePayoutRevealRankPrize(RaceUiPrizePayoutActor *arg0);
void updateRaceUiPrizePayoutShowRankPrize(RaceUiPrizePayoutActor *arg0);
void updateRaceUiPrizePayoutFadeIn(RaceUiPrizePayoutActor *arg0);
void initRaceUiPrizePayout(RaceUiPrizePayoutActor *arg0);
void drawRaceUiResultsBannerPlacements(RaceUiResultsBannerActor *arg0);
void drawRaceUiResultsBannerPlayerLabels(RaceUiResultsBannerActor *arg0);
void sortRaceUiResultsBannerPlayers(RaceUiResultsBannerActor *arg0);
void updateRaceUiResultsBannerConfirmed(RaceUiResultsBannerActor *arg0);
void updateRaceUiResultsBannerWaitForInput(RaceUiResultsBannerActor *arg0);
void updateRaceUiResultsBannerFadeIn(RaceUiResultsBannerActor *arg0);
void initRaceUiResultsBanner(RaceUiResultsBannerActor *arg0);
void drawRaceUiPackedTime(RaceTimer *arg0, s32 x, s32 y, s32 color);
void drawRaceUiCourseStatsRows(RaceUiCourseStatsActor *arg0);
void drawRaceUiCourseStatsElapsedTime(RaceUiAlpha1AActor *arg0);
void drawRaceUiCourseStatsLapTimeLabel(RaceUiAlpha18Actor *arg0);
void updateRaceUiCourseStatsConfirmed(RaceUiCourseStatsActor *arg0);
void updateRaceUiCourseStatsWaitForConfirm(RaceUiCourseStatsActor *arg0);
void updateRaceUiCourseStatsHoldRows(RaceUiCourseStatsActor *arg0);
void updateRaceUiCourseStatsFadeInRows(RaceUiCourseStatsActor *arg0);
void updateRaceUiCourseStatsFadeInTitle(RaceUiCourseStatsActor *arg0);
void initRaceUiCourseStats(RaceUiCourseStatsActor *arg0);
void drawRaceUiScoreAttackPointTotal(RaceUiAlpha18Actor *arg0);
void drawRaceUiHitPrizePayoutCounters(RaceUiCounterActor *arg0);
void drawRaceUiScoreAttackCourseValues(RaceUiPopupActor *arg0);
void drawRaceUiScoreAttackTargetStatus(RaceUiAlpha18Actor *arg0);
void drawRaceUiHitPrizePayoutLabels(RaceUiCounterActor *arg0);
void drawRaceUiScoreAttackHitLabels(void *arg0);
void updateRaceUiHitPrizeCourseValuesConfirmed(void *arg0);
void updateRaceUiHitPrizeCourseValuesWaitForConfirm(void *arg0);
void updateRaceUiHitPrizePayoutConfirmed(void *arg0);
void updateRaceUiHitPrizePayoutWaitForConfirm(RaceUiCounterActor *arg0);
void updateRaceUiHitPrizePayoutCountMoney(RaceUiCounterActor *arg0);
void updateRaceUiHitPrizeWaitBeforePayout(RaceUiCounterActor *arg0);
void updateRaceUiHitPrizeRevealTotalMoney(RaceUiCounterActor *arg0);
void updateRaceUiHitPrizeRevealCompleteBonus(RaceUiCounterActor *arg0);
void updateRaceUiHitPrizeRevealPerfectHit(RaceUiCounterActor *arg0);
void updateRaceUiHitPrizeHoldHitPrize(RaceUiCounterActor *arg0);
void updateRaceUiHitPrizeWaitBeforeHitPrize(RaceUiCounterActor *arg0);
void updateRaceUiHitPrizeFadeIn(RaceUiCounterActor *arg0);
void initRaceUiHitPrizePayout(RaceUiCounterActor *arg0);
void drawRaceUiCourseRecordElapsedTime(RaceUiAlphaActor *arg0);
void drawRaceUiCourseRecordPayoutCounters(RaceUiDualCounterActor *arg0);
void drawRaceUiCourseRecordRows(RaceUiPopupActor *arg0);
void drawRaceUiCourseRecordLapTimeLabel(RaceUiAlphaActor *arg0);
void drawRaceUiCourseRecordPayoutLabels(RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordRowsConfirmed(void *arg0);
void updateRaceUiCourseRecordRowsWaitForConfirm(void *arg0);
void updateRaceUiCourseRecordPayoutConfirmed(void *arg0);
void updateRaceUiCourseRecordPayoutWaitForConfirm(RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordPayoutCountMoney(RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordWaitBeforePayout(RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordHoldFinalMoney(RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordRevealFinalMoney(RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordRevealPerfectGet(RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordRevealSpeedFanGet(RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordRevealTimePrize(RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordWaitBeforeReveal(RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordFadeIn(RaceUiDualCounterActor *arg0);
void initRaceUiCourseRecordPayout(RaceUiDualCounterActor *arg0);
void drawRaceUiTrickAttackTotals(RaceUiDualCounterActor *arg0);
void drawRaceUiTrickPrizePayoutCounters(RaceUiCourseStatsActor *arg0);
void drawRaceUiTrickAttackCourseValues(RaceUiCourseStatsActor *arg0);
void drawRaceUiTrickAttackResultLabels(RaceUiAlpha18Actor *arg0);
void drawRaceUiTrickPrizePayoutLabels(RaceUiAlpha18Actor *arg0);
void drawRaceUiTrickAttackPointLabels(void *arg0);
void updateRaceUiTrickPrizeCourseValuesConfirmed(void *arg0);
void updateRaceUiTrickPrizeCourseValuesWaitForConfirm(void *arg0);
void updateRaceUiTrickPrizePayoutConfirmed(void *arg0);
void updateRaceUiTrickPrizePayoutWaitForConfirm(RaceUiCourseStatsActor *arg0);
void updateRaceUiTrickPrizePayoutCountMoney(RaceUiCourseStatsActor *arg0);
void updateRaceUiTrickPrizePayoutHoldMoneyRow(RaceUiCourseStatsActor *arg0);
void updateRaceUiTrickPrizePayoutRevealMoneyRow(RaceUiCourseStatsActor *arg0);
void updateRaceUiTrickPrizePayoutRevealCompleteBonus(RaceUiCourseStatsActor *arg0);
void updateRaceUiTrickPrizePayoutRevealMakeBonus(RaceUiCourseStatsActor *arg0);
void updateRaceUiTrickPrizePayoutRevealTrickPrize(RaceUiCourseStatsActor *arg0);
void updateRaceUiTrickPrizePayoutWaitBeforeReveal(RaceUiCourseStatsActor *arg0);
void updateRaceUiTrickPrizePayoutFadeIn(RaceUiCourseStatsActor *arg0);
void initRaceUiTrickPrizePayout(RaceUiCourseStatsActor *arg0);
void drawRaceUiResultAwardBadge(CallbackTask *task);
void updateRaceUiResultAwardBadgeDelay(CallbackTask *task);
void renderRacePlayerSparkleEffect(RaceUiSparkleActor *arg0);
void updateRacePlayerSparkleOrbit(RaceUiSparkleActor *arg0);
void updateRacePlayerSparkleRetarget(RaceUiSparkleActor *arg0);
void updateRacePlayerSparkleRise(RaceUiSparkleActor *arg0);
void initRacePlayerSparkleEffect(RaceUiSparkleActor *arg0);
void spawnRacePlayerSparkleEffect(s16 arg0);
void renderRaceUiExpiredSpeedFanEffect(RaceUiExpiredSpeedFanActor *arg0);
void updateRaceUiExpiredSpeedFanEffect(RaceUiExpiredSpeedFanActor *arg0);
void initRaceUiExpiredSpeedFanEffect(RaceUiExpiredSpeedFanActor *arg0);
void spawnRaceUiExpiredSpeedFanEffect(RacePlayer *player);
void renderRaceUiItemStealTrailEffect(RaceUiRankTrailActor *arg0);
void updateRaceUiItemStealTrailEffect(RaceUiRankTrailActor *arg0);
void initRaceUiItemStealTrailEffect(RaceUiRankTrailActor *arg0);
void renderRaceUiBurstTextParticle(RaceUiTextParticleActor *arg0);
void renderRaceUiAltBurstTextParticle(RaceUiTextParticleActor *arg0);
void updateRaceUiBurstTextParticle(RaceUiTextParticleActor *arg0);
void initRaceUiBurstTextParticle(RaceUiTextParticleActor *arg0);
void spawnRaceUiBurstTextParticle(void *arg0, void *arg1, void *arg2, s16 arg3);
void spawnRaceUiAltBurstTextParticle(void *arg0, void *arg1, void *arg2, s16 arg3);
void renderGhostSlowdownActor(RaceUiPodiumTrailActor *arg0);
void updateGhostSlowdownDriftAway(RaceUiPodiumTrailActor *arg0);
void updateGhostSlowdownImpact(RaceUiPodiumTrailActor *arg0);
void updateGhostSlowdownRise(RaceUiPodiumTrailActor *arg0);
void initGhostSlowdownActor(RaceUiPodiumTrailActor *arg0);
void spawnGhostSlowdownTargets(s16 arg0);
void drawRaceUiScorePopup(RaceUiPopupActor *arg0);
void updateRaceUiScorePopupSlideOut(RaceUiPopupActor *arg0);
void updateRaceUiScorePopupHold(RaceUiPopupActor *arg0);
void updateRaceUiScorePopupSlideIn(RaceUiPopupActor *arg0);
void initRaceUiScorePopup(RaceUiPopupActor *arg0);
void spawnRaceUiScorePopup(void *arg0, s16 arg1);
void renderDizzyLandCarousel(DizzyLandCarouselActor *arg0);
void updateDizzyLandCarousel(DizzyLandCarouselActor *arg0);
void initDizzyLandCarousel(DizzyLandCarouselActor *arg0);
void renderRaceUiHeavyKnockdownTrailEffect(RaceUiRankTrailActor *arg0);
void updateRaceUiHeavyKnockdownTrailEffect(RaceUiRankTrailActor *arg0);
void initRaceUiHeavyKnockdownTrailEffect(void *arg0);
void renderRaceUiThrownTrailObstacle(RaceUiThrownTrailActor *arg0);
void updateRaceUiThrownTrailObstacle(RaceUiThrownTrailActor *arg0);
void initRaceUiThrownTrailObstacle(RaceUiThrownTrailActor *arg0);
void spawnRaceUiThrownTrailObstacle(s16 arg0, void *arg1, void *arg2, s16 arg3);
void renderRaceUiSingleTrailEffect(RaceUiSingleTrailActor *arg0);
void updateRaceUiSingleTrailEffect(RaceUiSingleTrailActor *arg0);
void initRaceUiSingleTrailEffect(RaceUiSingleTrailActor *arg0);
void renderRaceUiFadingImpact(RaceUiFadingTrailActor *arg0);
void updateRaceUiFadingImpact(RaceUiFadingImpactActor *arg0);
void initRaceUiFadingImpact(RaceUiFadingImpactActor *arg0);
void spawnRaceUiFadingImpact(s32 x, s32 y, s32 z, s32 userId);
void renderRaceUiSpinHitTransitionEffect(RaceUiTransitionActor *arg0);
void updateRaceUiSpinHitTransitionEffect(RaceUiTransitionActor *arg0);
void initRaceUiSpinHitTransitionEffect(RaceUiTransitionActor *arg0);
void renderRaceUiStunOrbitingIcon(RaceUiOrbitingSpriteActor *arg0);
void updateRaceUiStunOrbitingIcon(RaceUiOrbitingSpriteActor *arg0);
void initRaceUiStunOrbitingIcon(RaceUiOrbitingSpriteActor *arg0);
void spawnRaceUiStunOrbitingIcons(s16 arg0);
void renderDizzyLandTeacupBumper(DizzyLandTeacupBumperActor *arg0);
void updateDizzyLandTeacupBumper(DizzyLandTeacupBumperActor *arg0);
void initDizzyLandTeacupBumper(DizzyLandTeacupBumperActor *arg0);
void renderDizzyLandFerrisWheel(DizzyLandFerrisWheelActor *arg0);
void updateDizzyLandFerrisWheel(DizzyLandFerrisWheelActor *arg0);
void initDizzyLandFerrisWheel(DizzyLandFerrisWheelActor *arg0);
void renderDizzyLandSpinningCabinRide(DizzyLandSpinningCabinRideActor *arg0);
void updateDizzyLandSpinningCabinRide(DizzyLandSpinningCabinRideActor *arg0);
void initDizzyLandSpinningCabinRide(DizzyLandSpinningCabinRideActor *arg0);
void renderCourseStartFinishSprite(RaceUiCourseSpriteActor *arg0);
void updateCourseStartFinishSprite(RaceUiCourseSpriteActor *actor);
void initCourseStartFinishSprite(RaceUiCourseSpriteActor *actor);
void renderSilverMountainSnowParticles(RaceUiEffectParticleActor *arg0);
void updateSilverMountainSnowParticles(RaceUiEffectParticleActor *arg0);
void initSilverMountainSnowParticles(RaceUiEffectParticleActor *actor);
void renderRaceCourseRankModel(RaceUiRankParticleActor *arg0);
void updateRaceCourseRankModel(RaceUiRankParticleActor *arg0);
void initRaceCourseRankModel(RaceUiRankParticleActor *actor);
void setRaceUiProjectileAnimation(RaceUiProjectileActor *arg0, u16 *arg1);
s32 advanceRaceUiProjectileAnimation(RaceUiProjectileActor *arg0);
void renderRaceUiProjectile(RaceUiProjectileActor *arg0);
void updateRaceUiProjectileFlyAway(RaceUiProjectileActor *arg0);
void updateRaceUiProjectileStealScores(RaceUiProjectileActor *arg0);
void updateRaceUiProjectileBounceDown(RaceUiProjectileActor *arg0);
void updateRaceUiProjectileGrounded(RaceUiProjectileActor *arg0);
void updateRaceUiProjectileInitialFall(RaceUiProjectileActor *arg0);
void initForwardActionProjectileEffect(RaceUiProjectileActor *arg0);
void drawTimeTrialRecordDeltaPopup(RaceUiTimeTrialRecordDeltaPopupActor *arg0);
void updateTimeTrialRecordDeltaPopupSlideOut(RaceUiTimeTrialRecordDeltaPopupActor *arg0);
void updateTimeTrialRecordDeltaPopupHold(RaceUiTimeTrialRecordDeltaPopupActor *arg0);
void updateTimeTrialRecordDeltaPopupSlideIn(RaceUiTimeTrialRecordDeltaPopupActor *arg0);
void initTimeTrialRecordDeltaPopup(RaceUiTimeTrialRecordDeltaPopupActor *arg0);
void renderRaceCourseCoinMarkers(RaceUiGfxCommandActor *arg0);
void updateRaceCourseCoinMarkers(RaceUiGfxCommandActor *arg0);
void initRaceCourseCoinMarkerMatrices(RaceUiGfxCommandActor *arg0);
void initRaceCourseCoinMarkers(RaceUiGfxCommandActor *arg0);
void renderRaceStartOverlay(RaceUiOverlayActor *arg0);
void updateRaceStartOverlayFallOut(RaceUiOverlayActor *arg0);
void updateRaceStartOverlayAnimate(RaceUiOverlayActor *arg0);
void updateRaceStartOverlayRiseIn(RaceUiOverlayActor *arg0);
void initRaceStartOverlay(RaceUiOverlayActor *arg0);
void renderRaceScoreAttackRings(RaceUiRankTextRenderActor *arg0);
void updateRaceScoreAttackRings(void *arg0);
void initRaceScoreAttackRingMatrices(RaceUiRankTextRenderActor *arg0);
void initRaceScoreAttackRings(RaceUiRankTextRenderActor *arg0);

#endif
