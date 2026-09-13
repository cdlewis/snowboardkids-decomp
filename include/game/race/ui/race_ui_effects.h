#ifndef RACE_UI_EFFECTS_H
#define RACE_UI_EFFECTS_H

#include "common.h"
#include "game/math/spatial_math.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/race/ui/race_hud.h"

struct RaceUiAlpha18Actor;
struct RaceUiAlpha1AActor;
struct RaceUiAlphaActor;
struct RaceUiAnimatedTextActor;
struct RaceUiCounterActor;
struct RaceUiCourseSpriteActor;
struct RaceUiCourseStatsActor;
struct RaceUiDualCounterActor;
struct RaceUiEffectParticleActor;
struct RaceUiFadingTrailActor;
struct RaceUiGfxCommandActor;
struct RaceUiOrbitingSpriteActor;
struct RaceUiPodiumTrailActor;
struct RaceUiPopupActor;
struct RaceUiPrizePayoutActor;
struct RaceUiPromptActor;
struct RaceUiRankParticleActor;
struct RaceUiRankTextRenderActor;
struct RaceUiRankTrailActor;
struct RaceUiResultsBannerActor;
struct RaceUiRisingTrailActor;
struct RaceUiScaledParticleActor;
struct RaceUiSingleTrailActor;
struct RaceCourseScrollingTextureActor;
struct RacePlayer;

/*
 * Persistent course models. Bytes 0x00..0x17 are the CallbackTaskHeader:
 * index aliases its userId, and pad0/pad12 cover the remaining scheduler fields.
 * Keep the direct index and rotation fields for renderer consumers. The actors
 * have different matrix/angle layouts, and their angles are not XYZ vectors.
 */
typedef struct RaceUiTripleParticleActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Mtx *matrix0;
    /* 0x28 */ Mtx *matrix1;
    /* 0x2C */ Mtx *matrix2;
    /* 0x30 */ s16 rotY;
    /* 0x32 */ u8 matrixDirty;
} RaceUiTripleParticleActor;

typedef struct RaceUiTrailingParticleActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Mtx *matrix0;
    /* 0x28 */ Mtx *matrix1;
    /* 0x2C */ s16 rotY;
    /* 0x2E */ s16 rotX;
    /* 0x30 */ u8 matrixDirty;
} RaceUiTrailingParticleActor;

typedef struct RaceUiSpinningParticleActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Mtx *matrix0;
    /* 0x28 */ Mtx *matrix1;
    /* 0x2C */ s16 rotY;
    /* 0x2E */ s16 rotZ;
    /* 0x30 */ s16 rotX;
    /* 0x32 */ s16 rotX2;
    /* 0x34 */ u8 matrixDirty;
} RaceUiSpinningParticleActor;

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
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
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
struct RaceUiExpiredSpeedFanActor;
struct RaceUiSparkleActor;
struct RaceUiTextParticleActor;
struct RaceUiThrownTrailActor;
struct RaceUiTimeTrialRecordDeltaPopupActor;
struct RaceUiTransitionActor;
struct RaceUiTransitionRenderActor;

/* Image/palette pointers follow the generic asset-loader void ** contract. */
typedef struct RaceUiOverlayActor {
    /* 0x00 */ u8 pad0[0x18];
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
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 playerIndex;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    /* 0x18 */ Transform3D copyBlock;
    /* 0x38 */ Vec3i pos;
    /* 0x44 */ Mtx *matrix;
    /* 0x48 */ s16 angle;
    /* 0x4A */ s16 alpha;
    /* 0x4C */ s16 scale;
    /* 0x4E */ u8 matrixDirty;
} RaceUiFadingImpactActor;

extern Vec3i gDizzyLandTrailingParticleLocalOffset;
extern RaceUiProjectileVertexBlock D_800D64A0[8];
extern Gfx gAlphaSpriteRenderModeDl[];

void renderRaceCourseScrollingTexture(struct RaceCourseScrollingTextureActor *arg0);
void updateRaceCourseScrollingTexture(struct RaceCourseScrollingTextureActor *arg0);
void initRaceCourseScrollingTexture(struct RaceCourseScrollingTextureActor *arg0);
void drawRaceUiBoardReversePrompt(struct RaceUiPromptActor *arg0);
void updateRaceUiBoardReversePrompt(struct RaceUiPromptActor *arg0);
void initRaceUiBoardReversePrompt(struct RaceUiPromptActor *arg0);
void drawRaceUiTrickScorePopup(struct RaceUiPopupActor *arg0);
void updateRaceUiTrickScorePopupSlideOut(struct RaceUiPopupActor *arg0);
void updateRaceUiTrickScorePopupHold(struct RaceUiPopupActor *arg0);
void updateRaceUiTrickScorePopupSlideIn(struct RaceUiPopupActor *arg0);
void initRaceUiTrickScorePopup(struct RaceUiPopupActor *arg0);
void spawnRaceUiTrickScorePopup(void *arg0, s16 arg1);
void drawRaceUiCrashScorePopup(struct RaceUiPopupActor *arg0);
void updateRaceUiCrashScorePopupSlideOut(struct RaceUiPopupActor *arg0);
void updateRaceUiCrashScorePopupHold(struct RaceUiPopupActor *arg0);
void updateRaceUiCrashScorePopupSlideIn(struct RaceUiPopupActor *arg0);
void initRaceUiCrashScorePopup(struct RaceUiPopupActor *arg0);
void drawRaceUiPrizePayoutContinuePrompt(void *arg0);
void updateRaceUiPrizePayoutContinuePrompt(void *arg0);
void drawRaceUiPrizePayoutRankPanel(struct RaceUiAlpha18Actor *arg0);
void drawRaceUiPrizePayoutRankPrize(struct RaceUiAlpha18Actor *arg0);
void drawRaceUiPrizePayoutCollectedCoins(void *arg0);
void drawRaceUiPrizePayoutTotalMoney(void *arg0);
void drawRaceUiPrizePayoutLabels(struct RaceUiAlpha18Actor *arg0);
void updateRaceUiPrizePayoutConfirmed(void *arg0);
void updateRaceUiPrizePayoutWaitForConfirm(void *arg0);
void updateRaceUiPrizePayoutCountMoney(struct RaceUiAlpha18Actor *arg0);
void updateRaceUiPrizePayoutWaitBeforeMoneyCount(struct RaceUiPrizePayoutActor *arg0);
void updateRaceUiPrizePayoutRevealGetCoin(struct RaceUiPrizePayoutActor *arg0);
void updateRaceUiPrizePayoutRevealRankPrize(struct RaceUiPrizePayoutActor *arg0);
void updateRaceUiPrizePayoutShowRankPrize(struct RaceUiPrizePayoutActor *arg0);
void updateRaceUiPrizePayoutFadeIn(struct RaceUiPrizePayoutActor *arg0);
void initRaceUiPrizePayout(struct RaceUiPrizePayoutActor *arg0);
void drawRaceUiResultsBannerPlacements(struct RaceUiResultsBannerActor *arg0);
void drawRaceUiResultsBannerPlayerLabels(struct RaceUiResultsBannerActor *arg0);
void sortRaceUiResultsBannerPlayers(struct RaceUiResultsBannerActor *arg0);
void updateRaceUiResultsBannerConfirmed(struct RaceUiResultsBannerActor *arg0);
void updateRaceUiResultsBannerWaitForInput(struct RaceUiResultsBannerActor *arg0);
void updateRaceUiResultsBannerFadeIn(struct RaceUiResultsBannerActor *arg0);
void initRaceUiResultsBanner(struct RaceUiResultsBannerActor *arg0);
void drawRaceUiPackedTime(RaceTimer *arg0, s32 x, s32 y, s32 color);
void drawRaceUiCourseStatsRows(struct RaceUiCourseStatsActor *arg0);
void drawRaceUiCourseStatsElapsedTime(struct RaceUiAlpha1AActor *arg0);
void drawRaceUiCourseStatsLapTimeLabel(struct RaceUiAlpha18Actor *arg0);
void updateRaceUiCourseStatsConfirmed(struct RaceUiCourseStatsActor *arg0);
void updateRaceUiCourseStatsWaitForConfirm(struct RaceUiCourseStatsActor *arg0);
void updateRaceUiCourseStatsHoldRows(struct RaceUiCourseStatsActor *arg0);
void updateRaceUiCourseStatsFadeInRows(struct RaceUiCourseStatsActor *arg0);
void updateRaceUiCourseStatsFadeInTitle(struct RaceUiCourseStatsActor *arg0);
void initRaceUiCourseStats(struct RaceUiCourseStatsActor *arg0);
void drawRaceUiScoreAttackPointTotal(struct RaceUiAlpha18Actor *arg0);
void drawRaceUiHitPrizePayoutCounters(struct RaceUiCounterActor *arg0);
void drawRaceUiScoreAttackCourseValues(struct RaceUiPopupActor *arg0);
void drawRaceUiScoreAttackTargetStatus(struct RaceUiAlpha18Actor *arg0);
void drawRaceUiHitPrizePayoutLabels(struct RaceUiCounterActor *arg0);
void drawRaceUiScoreAttackHitLabels(void *arg0);
void updateRaceUiHitPrizeCourseValuesConfirmed(void *arg0);
void updateRaceUiHitPrizeCourseValuesWaitForConfirm(void *arg0);
void updateRaceUiHitPrizePayoutConfirmed(void *arg0);
void updateRaceUiHitPrizePayoutWaitForConfirm(struct RaceUiCounterActor *arg0);
void updateRaceUiHitPrizePayoutCountMoney(struct RaceUiCounterActor *arg0);
void updateRaceUiHitPrizeWaitBeforePayout(struct RaceUiCounterActor *arg0);
void updateRaceUiHitPrizeRevealTotalMoney(struct RaceUiCounterActor *arg0);
void updateRaceUiHitPrizeRevealCompleteBonus(struct RaceUiCounterActor *arg0);
void updateRaceUiHitPrizeRevealPerfectHit(struct RaceUiCounterActor *arg0);
void updateRaceUiHitPrizeHoldHitPrize(struct RaceUiCounterActor *arg0);
void updateRaceUiHitPrizeWaitBeforeHitPrize(struct RaceUiCounterActor *arg0);
void updateRaceUiHitPrizeFadeIn(struct RaceUiCounterActor *arg0);
void initRaceUiHitPrizePayout(struct RaceUiCounterActor *arg0);
void drawRaceUiCourseRecordElapsedTime(struct RaceUiAlphaActor *arg0);
void drawRaceUiCourseRecordPayoutCounters(struct RaceUiDualCounterActor *arg0);
void drawRaceUiCourseRecordRows(struct RaceUiPopupActor *arg0);
void drawRaceUiCourseRecordLapTimeLabel(struct RaceUiAlphaActor *arg0);
void drawRaceUiCourseRecordPayoutLabels(struct RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordRowsConfirmed(void *arg0);
void updateRaceUiCourseRecordRowsWaitForConfirm(void *arg0);
void updateRaceUiCourseRecordPayoutConfirmed(void *arg0);
void updateRaceUiCourseRecordPayoutWaitForConfirm(struct RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordPayoutCountMoney(struct RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordWaitBeforePayout(struct RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordHoldFinalMoney(struct RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordRevealFinalMoney(struct RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordRevealPerfectGet(struct RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordRevealSpeedFanGet(struct RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordRevealTimePrize(struct RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordWaitBeforeReveal(struct RaceUiDualCounterActor *arg0);
void updateRaceUiCourseRecordFadeIn(struct RaceUiDualCounterActor *arg0);
void initRaceUiCourseRecordPayout(struct RaceUiDualCounterActor *arg0);
void drawRaceUiTrickAttackTotals(struct RaceUiDualCounterActor *arg0);
void drawRaceUiTrickPrizePayoutCounters(struct RaceUiCourseStatsActor *arg0);
void drawRaceUiTrickAttackCourseValues(struct RaceUiCourseStatsActor *arg0);
void drawRaceUiTrickAttackResultLabels(struct RaceUiAlpha18Actor *arg0);
void drawRaceUiTrickPrizePayoutLabels(struct RaceUiAlpha18Actor *arg0);
void drawRaceUiTrickAttackPointLabels(void *arg0);
void updateRaceUiTrickPrizeCourseValuesConfirmed(void *arg0);
void updateRaceUiTrickPrizeCourseValuesWaitForConfirm(void *arg0);
void updateRaceUiTrickPrizePayoutConfirmed(void *arg0);
void updateRaceUiTrickPrizePayoutWaitForConfirm(struct RaceUiCourseStatsActor *arg0);
void updateRaceUiTrickPrizePayoutCountMoney(struct RaceUiCourseStatsActor *arg0);
void updateRaceUiTrickPrizePayoutHoldMoneyRow(struct RaceUiCourseStatsActor *arg0);
void updateRaceUiTrickPrizePayoutRevealMoneyRow(struct RaceUiCourseStatsActor *arg0);
void updateRaceUiTrickPrizePayoutRevealCompleteBonus(struct RaceUiCourseStatsActor *arg0);
void updateRaceUiTrickPrizePayoutRevealMakeBonus(struct RaceUiCourseStatsActor *arg0);
void updateRaceUiTrickPrizePayoutRevealTrickPrize(struct RaceUiCourseStatsActor *arg0);
void updateRaceUiTrickPrizePayoutWaitBeforeReveal(struct RaceUiCourseStatsActor *arg0);
void updateRaceUiTrickPrizePayoutFadeIn(struct RaceUiCourseStatsActor *arg0);
void initRaceUiTrickPrizePayout(struct RaceUiCourseStatsActor *arg0);
void drawRaceUiResultAwardBadge(CallbackTask *task);
void updateRaceUiResultAwardBadgeDelay(CallbackTask *task);
void renderRacePlayerSparkleEffect(struct RaceUiSparkleActor *arg0);
void updateRacePlayerSparkleOrbit(struct RaceUiSparkleActor *arg0);
void updateRacePlayerSparkleRetarget(struct RaceUiSparkleActor *arg0);
void updateRacePlayerSparkleRise(struct RaceUiSparkleActor *arg0);
void initRacePlayerSparkleEffect(struct RaceUiSparkleActor *arg0);
void spawnRacePlayerSparkleEffect(s16 arg0);
void renderRaceUiExpiredSpeedFanEffect(struct RaceUiExpiredSpeedFanActor *arg0);
void updateRaceUiExpiredSpeedFanEffect(struct RaceUiExpiredSpeedFanActor *arg0);
void initRaceUiExpiredSpeedFanEffect(struct RaceUiExpiredSpeedFanActor *arg0);
void spawnRaceUiExpiredSpeedFanEffect(struct RacePlayer *player);
void renderRaceUiItemStealTrailEffect(struct RaceUiRankTrailActor *arg0);
void updateRaceUiItemStealTrailEffect(struct RaceUiRankTrailActor *arg0);
void initRaceUiItemStealTrailEffect(struct RaceUiRankTrailActor *arg0);
void renderRaceUiBurstTextParticle(struct RaceUiAnimatedTextActor *arg0);
void renderRaceUiAltBurstTextParticle(struct RaceUiTextParticleActor *arg0);
void updateRaceUiBurstTextParticle(struct RaceUiTextParticleActor *arg0);
void initRaceUiBurstTextParticle(struct RaceUiTextParticleActor *arg0);
void spawnRaceUiBurstTextParticle(void *arg0, void *arg1, void *arg2, s16 arg3);
void spawnRaceUiAltBurstTextParticle(void *arg0, void *arg1, void *arg2, s16 arg3);
void renderGhostSlowdownActor(struct RaceUiPodiumTrailActor *arg0);
void updateGhostSlowdownDriftAway(struct RaceUiPodiumTrailActor *arg0);
void updateGhostSlowdownImpact(struct RaceUiPodiumTrailActor *arg0);
void updateGhostSlowdownRise(struct RaceUiPodiumTrailActor *arg0);
void initGhostSlowdownActor(struct RaceUiPodiumTrailActor *arg0);
void spawnGhostSlowdownTargets(s16 arg0);
void drawRaceUiScorePopup(struct RaceUiPopupActor *arg0);
void updateRaceUiScorePopupSlideOut(struct RaceUiPopupActor *arg0);
void updateRaceUiScorePopupHold(struct RaceUiPopupActor *arg0);
void updateRaceUiScorePopupSlideIn(struct RaceUiPopupActor *arg0);
void initRaceUiScorePopup(struct RaceUiPopupActor *arg0);
void spawnRaceUiScorePopup(void *arg0, s16 arg1);
void renderRaceCourseTripleParticle(struct RaceUiTripleParticleActor *arg0);
void updateRaceCourseTripleParticle(struct RaceUiTripleParticleActor *arg0);
void initRaceCourseTripleParticle(struct RaceUiTripleParticleActor *arg0);
void renderRaceUiHeavyKnockdownTrailEffect(struct RaceUiRankTrailActor *arg0);
void updateRaceUiHeavyKnockdownTrailEffect(struct RaceUiRankTrailActor *arg0);
void initRaceUiHeavyKnockdownTrailEffect(void *arg0);
void renderRaceUiThrownTrailObstacle(struct RaceUiRisingTrailActor *arg0);
void updateRaceUiThrownTrailObstacle(struct RaceUiThrownTrailActor *arg0);
void initRaceUiThrownTrailObstacle(struct RaceUiThrownTrailActor *arg0);
void spawnRaceUiThrownTrailObstacle(s16 arg0, void *arg1, void *arg2, s16 arg3);
void renderRaceUiSingleTrailEffect(struct RaceUiSingleTrailActor *arg0);
void updateRaceUiSingleTrailEffect(struct RaceUiSingleTrailActor *arg0);
void initRaceUiSingleTrailEffect(struct RaceUiSingleTrailActor *arg0);
void renderRaceUiFadingImpact(struct RaceUiFadingTrailActor *arg0);
void updateRaceUiFadingImpact(RaceUiFadingImpactActor *arg0);
void initRaceUiFadingImpact(RaceUiFadingImpactActor *arg0);
void spawnRaceUiFadingImpact(s32 x, s32 y, s32 z, s32 userId);
void renderRaceUiSpinHitTransitionEffect(struct RaceUiTransitionRenderActor *arg0);
void updateRaceUiSpinHitTransitionEffect(struct RaceUiTransitionActor *arg0);
void initRaceUiSpinHitTransitionEffect(struct RaceUiTransitionActor *arg0);
void renderRaceUiStunOrbitingIcon(struct RaceUiOrbitingSpriteActor *arg0);
void updateRaceUiStunOrbitingIcon(struct RaceUiOrbitingSpriteActor *arg0);
void initRaceUiStunOrbitingIcon(struct RaceUiOrbitingSpriteActor *arg0);
void spawnRaceUiStunOrbitingIcons(s16 arg0);
void renderIceCourseBumper(struct RaceUiScaledParticleActor *arg0);
void updateIceCourseBumper(struct RaceUiScaledParticleActor *arg0);
void initIceCourseBumper(struct RaceUiScaledParticleActor *arg0);
void renderDizzyLandTrailingParticle(struct RaceUiTrailingParticleActor *arg0);
void updateDizzyLandTrailingParticle(struct RaceUiTrailingParticleActor *arg0);
void initDizzyLandTrailingParticle(struct RaceUiTrailingParticleActor *arg0);
void renderRaceCourseSpinningObject(struct RaceUiSpinningParticleActor *arg0);
void updateRaceCourseSpinningObject(struct RaceUiSpinningParticleActor *arg0);
void initRaceCourseSpinningObject(struct RaceUiSpinningParticleActor *arg0);
void renderCourseStartFinishSprite(struct RaceUiCourseSpriteActor *arg0);
void updateCourseStartFinishSprite(struct RaceUiCourseSpriteActor *actor);
void initCourseStartFinishSprite(struct RaceUiCourseSpriteActor *actor);
void renderSilverMountainSnowParticles(struct RaceUiEffectParticleActor *arg0);
void updateSilverMountainSnowParticles(struct RaceUiEffectParticleActor *arg0);
void initSilverMountainSnowParticles(struct RaceUiEffectParticleActor *actor);
void renderRaceCourseRankModel(struct RaceUiRankParticleActor *arg0);
void updateRaceCourseRankModel(struct RaceUiRankParticleActor *arg0);
void initRaceCourseRankModel(struct RaceUiRankParticleActor *actor);
void setRaceUiProjectileAnimation(struct RaceUiProjectileActor *arg0, u16 *arg1);
s32 advanceRaceUiProjectileAnimation(struct RaceUiProjectileActor *arg0);
void renderRaceUiProjectile(struct RaceUiProjectileActor *arg0);
void updateRaceUiProjectileFlyAway(struct RaceUiProjectileActor *arg0);
void updateRaceUiProjectileStealScores(struct RaceUiProjectileActor *arg0);
void updateRaceUiProjectileBounceDown(struct RaceUiProjectileActor *arg0);
void updateRaceUiProjectileGrounded(struct RaceUiProjectileActor *arg0);
void updateRaceUiProjectileInitialFall(struct RaceUiProjectileActor *arg0);
void initForwardActionProjectileEffect(struct RaceUiProjectileActor *arg0);
void drawTimeTrialRecordDeltaPopup(struct RaceUiTimeTrialRecordDeltaPopupActor *arg0);
void updateTimeTrialRecordDeltaPopupSlideOut(struct RaceUiTimeTrialRecordDeltaPopupActor *arg0);
void updateTimeTrialRecordDeltaPopupHold(struct RaceUiTimeTrialRecordDeltaPopupActor *arg0);
void updateTimeTrialRecordDeltaPopupSlideIn(struct RaceUiTimeTrialRecordDeltaPopupActor *arg0);
void initTimeTrialRecordDeltaPopup(struct RaceUiTimeTrialRecordDeltaPopupActor *arg0);
void renderRaceCourseCoinMarkers(struct RaceUiGfxCommandActor *arg0);
void updateRaceCourseCoinMarkers(struct RaceUiGfxCommandActor *arg0);
void initRaceCourseCoinMarkerMatrices(struct RaceUiGfxCommandActor *arg0);
void initRaceCourseCoinMarkers(struct RaceUiGfxCommandActor *arg0);
void renderRaceStartOverlay(struct RaceUiOverlayActor *arg0);
void updateRaceStartOverlayFallOut(struct RaceUiOverlayActor *arg0);
void updateRaceStartOverlayAnimate(struct RaceUiOverlayActor *arg0);
void updateRaceStartOverlayRiseIn(struct RaceUiOverlayActor *arg0);
void initRaceStartOverlay(struct RaceUiOverlayActor *arg0);
void renderRaceScoreAttackRings(struct RaceUiRankTextRenderActor *arg0);
void updateRaceScoreAttackRings(void *arg0);
void initRaceScoreAttackRingMatrices(struct RaceUiRankTextRenderActor *arg0);
void initRaceScoreAttackRings(struct RaceUiRankTextRenderActor *arg0);

#endif
