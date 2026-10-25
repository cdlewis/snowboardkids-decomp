#ifndef RACE_PLAYER_INPUT_H
#define RACE_PLAYER_INPUT_H

#include "common.h"
#include "game/math/geometry.h"
#include "game/race/effects/snowboard_trail_types.h"
#include "game/race/player/character_model.h"

#define RACE_PLAYER_COUNT 4
#define RACE_INPUT_REPLAY_FRAME_COUNT 0x960
#define RACE_PLAYER_MODEL_PART_CAPACITY 14
#define RACE_PLAYER_COLLISION_SQUASHED 0x200000

typedef u8 RacePlayerOrder[RACE_PLAYER_COUNT];

/* Recorded-input racers can have isCpu == 0, including title/menu demos.
 * gRaceDemoPlaybackEnabled alone does not identify all recorded playback.
 * Item-grant/refill mods should exclude replayInputSource != RACE_PLAYER_INPUT_LIVE.
 * Title/menu demos and item-training preview races use source 5; saved ghosts
 * use sources 1 and 2. Source 0 reads the controller; nonzero sources enter
 * playRaceInputHistoryFrame, with source 5 delegating to replay playback.
 * These constants describe the s8 field; enum storage would change its layout. */
typedef enum RacePlayerInputSource {
    RACE_PLAYER_INPUT_LIVE = 0,
    RACE_PLAYER_INPUT_HISTORY_07 = 1, /* History buffer in asset slot 0x07. */
    RACE_PLAYER_INPUT_HISTORY_2B = 2, /* History buffer in asset slot 0x2B. */
    RACE_PLAYER_INPUT_REPLAY = 5      /* Delegates to playRaceReplayInputFrame. */
} RacePlayerInputSource;

typedef struct RaceScoreAttackRingTrigger {
    /* 0x00 */ struct RaceScoreAttackRingTrigger *next;
    /* 0x04 */ Vec3i position;
    /* 0x10 */ s32 radius;
    /* 0x14 */ s8 triggered;
    /* 0x15 */ u8 pad15[3];
} RaceScoreAttackRingTrigger;

/* CPU distance/role policy. Mode 0 still applies the smoothed rank bonus,
 * also used by humans; character 5 forces it, bypassing distance policy. */
typedef enum RaceCpuPaceMode {
    RACE_CPU_PACE_RANK_BONUS = 0,
    RACE_CPU_PACE_CATCH_UP = 1,  /* Add 0x70000 to target speed. */
    RACE_CPU_PACE_SLOW_DOWN = 2, /* Divide target speed by three. */
    RACE_CPU_PACE_YIELD = 3      /* Subtract 1/16 unless ranked fourth. */
} RaceCpuPaceMode;

/* Player movement, animation, equipment, scoring, and effects during a race.
 * Field offsets are relative to this struct; unk/pad names use RacePlayer offsets. */
typedef struct RacePlayerState {
    /* 0x000 */ Vec3i pos;
    /* 0x00C */ Vec3i renderPosition; /* pos with ground offset/correction applied for rendering and effects. */
    /* 0x018 */ Vec3i previousPosition;
    /* 0x024 */ Vec3i velocity;
    /* 0x030 */ char pad4C[0xC];
    /* 0x03C */ s32 groundOffset;
    /* 0x040 */ s32 collisionBottomY;
    /* 0x044 */ s32 targetGroundOffset;
    /* 0x048 */ s32 groundCorrectionY;
    /* 0x04C */ s32 collisionCenterOffset;
    /* 0x050 */ s16 modelRotationX;
    /* 0x052 */ s16 modelRotationY;
    /* 0x054 */ s16 modelRotationZ;
    /* 0x056 */ char pad72[2];
    /* 0x058 */ s32 verticalVelocityLimit;
    /* 0x05C */ char pad78[4];
    /* Mode-dependent scratch: frame timer, trick rotation progress, or jump impulse. */
    /* 0x060 */ s32 modeValue;
    /* 0x064 */ s32 unk80;
    /* 0x068 */ u32 heldInput;     /* Buttons and digital stick directions held this frame. */
    /* 0x06C */ u32 pressedInput;  /* Inputs newly pressed this frame. */
    /* 0x070 */ u32 previousInput; /* Held input from the previous frame. */
    /* 0x074 */ s8 stickX;
    /* 0x075 */ s8 stickY;
    /* 0x076 */ s8 stanceChangeTimer;
    /* 0x077 */ s8 stanceTransitionFrame;
    /* 0x078 */ Transform3D modelPartTransforms[RACE_PLAYER_MODEL_PART_CAPACITY];
    /* 0x238 */ s32 localVelocityZ;
    /* 0x23C */ s32 localVelocityX;
    /* 0x240 */ s32 baseSpeedLimit;
    /* 0x244 */ s32 gravity;
    /* 0x248 */ s32 aerialGravity;
    /* 0x24C */ s32 turnStrength;
    /* 0x250 */ s32 turnRadiusAtFullLean;
    /* 0x254 */ s32 turnRadiusAtHalfLean;
    /* 0x258 */ s32 lateralDeceleration;
    /* 0x25C */ s32 forwardDeceleration;
    /* 0x260 */ s32 reverseDeceleration;
    /* 0x264 */ s32 collisionRadius;
    /* 0x268 */ s32 collisionHeight;
    /* 0x26C */ s16 modelBasePitch;
    /* 0x26E */ s16 modelBaseYaw;
    /* 0x270 */ s16 modelBaseRoll;
    /* 0x272 */ char pad28E[2];
    /* 0x274 */ Vec3i projectedPos;
    /* 0x280 */ s32 movementSpeed;
    /* Frames waiting for grab input; a press in the first six selects the early-input variant. */
    /* 0x284 */ s16 grabInputFrames;
    /* 0x286 */ s16 grabTrickId;
    /* 0x288 */ s16 grabHoldFrames; /* Extra hold frames after the grab animation completes. */
    /* 0x28A */ s16 aerialTrickId;
    /* 0x28C */ s16 surfaceCueState[6];
    /* 0x298 */ s16 surfaceCueStep[6];
    /* 0x2A4 */ s16 trickAttackPointTotal;
    /* 0x2A6 */ s8 trickAttackBestMakeCount;
    /* 0x2A7 */ s8 trickAttackMakeCount;
    /* 0x2A8 */ u16 trickAttackCompletedTrickMask;
    /* 0x2AA */ u16 pendingItemHitFlags;
    /* 0x2AC */ s32 collisionVelocityX;
    /* 0x2B0 */ s32 collisionVelocityZ;
    /* 0x2B4 */ char pad2D0[2];
    /* 0x2B6 */ s16 hitSourcePlayerIndex;
    /* 0x2B8 */ s16 unk2D4;
    /* 0x2BA */ s16 unk2D6;
    /* 0x2BC */ s16 activeSparkleEffectCount;
    /* 0x2BE */ s16 trailEffectTimer;
    /* 0x2C0 */ u8 snowboardTrailSide;
    /* 0x2C1 */ char pad2DD[1];
    /* 0x2C2 */ s16 unk2DE;
    /* 0x2C4 */ s32 unk2E0;
    /* 0x2C8 */ s32 unk2E4;
    /* 0x2CC */ s16 unk2E8;
    /* 0x2CE */ s16 pitchAngle;
    /* 0x2D0 */ s16 facingAngle;
    /* 0x2D2 */ s16 leanAngle;
    /* 0x2D4 */ s16 surfacePitchAngle;
    /* 0x2D6 */ char pad2F2[2];
    /* 0x2D8 */ s16 surfaceRollAngle;
    /* 0x2DA */ s16 leanInput;
    /* 0x2DC */ s16 leanScale;
    /* Grounded lean-animation blend angle; launch-ramp modes reuse it as their target yaw. */
    /* 0x2DE */ s16 modeAngle;
    /* 0x2E0 */ u32 stateFlags;
    /* 0x2E4 */ s16 mode;
    /* 0x2E6 */ s16 updateState;
    /* 0x2E8 */ s16 updateTimer;
    /* 0x2EA */ s16 unk306;
    /* 0x2EC */ s16 subState;
    /* 0x2EE */ s16 subStateTimer;
    /* 0x2F0 */ s16 subStateStep;
    /* 0x2F2 */ s16 subStateParam;
    /* 0x2F4 */ s32 targetSpeedLimit;
    /* 0x2F8 */ s32 speedLimit;
    /* 0x2FC */ s32 rankSpeedBonus; /* Approaches rank table by at most +0x20/-0x30 per update. */
    /* 0x300 */ s16 unk31C;
    /* 0x302 */ s16 unk31E;
    /* 0x304 */ s16 invisibilityTimer;
    /* 0x306 */ char pad322[0xE];
    /* 0x314 */ s8 surfaceType;
    /* 0x315 */ s8 previousSurfaceType;
    /* 0x316 */ s16 unk332;
    /* 0x318 */ s16 unk334;
    /* 0x31A */ s16 unk336;
    /* 0x31C */ CharacterModelPart modelParts[RACE_PLAYER_MODEL_PART_CAPACITY];
    /* 0x434 */ s16 modelPartCount;
    /* 0x436 */ s16 animationId;
    /* 0x438 */ s32 animationStartOffset;
    /* 0x43C */ s32 animationFrameDataOffset;
    /* 0x440 */ s16 animationFrameCount;
    /* 0x442 */ s16 animationFramesRemaining;
    /* 0x444 */ char pad460[4];
    /* 0x448 */ s16 animationFrameDuration;
    /* 0x44A */ s16 animationFrameTimer;
    /* 0x44C */ Vec3i shadowCorners[4]; /* Board corners projected onto the terrain for the shadow. */
    /* 0x47C */ Vtx *shadowVtx;
    /* 0x480 */ Mtx *shadowMtx;
    /* 0x484 */ Vec3i boardContactPoints[8]; /* Current world-space corners [0..3], previous frame [4..7]. */
    /* 0x4E4 */ u8 groundContactMask;
    /* 0x4E5 */ char pad501[1];
    /* 0x4E6 */ s16 coursePathIndex;
    /* 0x4E8 */ s32 coursePathOffset;
    /* 0x4EC */ s8 lapDigit;
    /* 0x4ED */ s8 rankIndex; /* Zero-based: 0 is first, 3 is fourth. */
    /* 0x4EE */ char pad50A[2];
    /* 0x4F0 */ union {
        s16 *anglePtr;
        RaceScoreAttackRingTrigger *scoreAttackRingTriggerList;
    };
    /* Available shield shots: spent on spawn, returned when the projectile is removed. */
    /* 0x4F4 */ s16 shieldShotCount;
    /* Red-shop shot inventory (Z); blue-shop item inventory (B). See race_items.h. */
    /* 0x4F6 */ s8 shotType;
    /* 0x4F7 */ s8 shotPalette;
    /* 0x4F8 */ s8 shotCount;
    /* 0x4F9 */ s8 itemType;
    /* 0x4FA */ s8 itemPalette;
    /* 0x4FB */ s8 unk517;
    /* 0x4FC */ u8 randomIndex;
    /* 0x4FD */ s8 unk519;
    /* Set by homing shots and falling pans; cleared after the player mode update. */
    /* 0x4FE */ u8 isTargetedByAttack;
    /* 0x4FF */ char pad51B[1];
    /* 0x500 */ u32 checkpointEventMask;
    /* 0x504 */ s32 smoothedPathOffset;
    /* 0x508 */ s8 checkpointHit;
    /* 0x509 */ s8 unk525;
    /* 0x50A */ char pad526[1];
    /* 0x50B */ s8 surfaceCueOverrideMask;
    /* 0x50C */ s8 unk528;
    /* 0x50D */ s8 displayRank;
    /* 0x50E */ s8 cpuPaceMode;
    /* 0x50F */ s8 rankTargetPlayer;
    /* Count eligible CPU use-function calls, normally ~30/sec (US); skipped calls pause them.
     * Signed byte wraparound is intentional for the longer targeted-shot cooldown. */
    /* 0x510 */ s8 cpuItemUseCooldown;
    /* 0x511 */ s8 cpuShotUseCooldown;
    /* 0x512 */ char pad52E[2];
    /* 0x514 */ Mtx *modelPartMatrices[RACE_PLAYER_MODEL_PART_CAPACITY];
    /* 0x54C */ s32 score; /* Race gold: pickups spend 100; distinct from menu/save money at 0x00C. */
    /* 0x550 */ s32 cumulativeRaceScore;
    /* 0x554 */ s16 collectedCourseCoinMarkerCount;
    /* 0x556 */ s16 totalCourseCoinMarkerCount;
    /* 0x558 */ s16 scoreAttackPointTotal;
    /* 0x55A */ s16 scoreAttackPointTarget;
    /* 0x55C */ s16 unk578;
    /* 0x55E */ s8 unk57A;
    /* 0x55F */ s8 unk57B;
    /* 0x560 */ s16 unk57C;
    /* 0x562 */ s16 progressMeterPosition;
    /* 0x564 */ s8 progressMeterSquashFrame;
    /* 0x565 */ s8 unk581;
    /* 0x566 */ s16 loopSoundVolume;
    /* 0x568 */ s16 loopSoundId;
    /* 0x56A */ char pad586[2];
    /* 0x56C */ f32 loopSoundPitch;
    /* 0x570 */ SpeedFanState speedFan;
    /* 0x5E8 */ s16 rumbleTimer;
    /* 0x5EA */ s16 rumblePatternIndex;
    /* 0x5EC */ s8 rumblePatternId;
    /* 0x5ED */ char pad609[1];
    /* 0x5EE */ s16 replayFrame;
} RacePlayerState;

typedef struct RacePlayer {
    /* 0x000 */ u16 playerIndex;
    /* 0x002 */ s16 unk2;
    /* 0x004 */ u8 isCpu;
    /* 0x005 */ u8 selectedCharacterId;
    /* 0x006 */ u8 menuSelection;
    /* 0x007 */ u8 selectionUnlockState;
    /* 0x008 */ u8 menuState;
    /* 0x009 */ u8 unk9;
    /* 0x00A */ u8 unkA;
    /* 0x00B */ u8 unkB;
    /* 0x00C */ s32 money;
    /* 0x010 */ u8 characterId;
    /* 0x011 */ u8 snowboardIndex; /* Board properties and model index, separate from its texture. */
    /* 0x012 */ u8 snowboardTextureIndex;
    /* 0x013 */ s8 isActive;
    /* Also suppresses body collisions, rumble and effects. Post-update skips
     * main-RNG draws and track sparks when set. Saved ghost setup sets this
     * flag; title/menu demos and item-training previews leave it clear. */
    /* 0x014 */ s8 soundDisabled;
    /* 0x015 */ s8 unk15;
    /* 0x016 */ s8 replayInputSource; /* RacePlayerInputSource */
    /* 0x017 */ s8 unk17;
    /* 0x018 */ s16 unk18;
    /* CPU use thresholds, compared inclusively against a random byte; not percentages. */
    /* 0x01A */ u8 cpuItemUseChance;
    /* 0x01B */ u8 cpuShotUseChance;
    /* 0x01C */ RacePlayerState state;
} RacePlayer;

typedef char RacePlayerSizeCheck[(sizeof(RacePlayer) == 0x60C) ? 1 : -1];
typedef char RacePlayerArraySizeCheck[((sizeof(RacePlayer) * RACE_PLAYER_COUNT) == 0x1830) ? 1 : -1];

extern RacePlayer gRacePlayers[RACE_PLAYER_COUNT];
extern RacePlayerOrder gRaceOrderPlayerIds;
extern u8 gPlayerCount;

#define gRacePlayersEnd (&gRacePlayers[RACE_PLAYER_COUNT])

void recordRaceReplayInputFrame(RacePlayer *player);
void playRaceReplayInputFrame(RacePlayer *player);
void recordRaceInputHistoryFrame(RacePlayer *player);
void playRaceInputHistoryFrame(RacePlayer *player);
void updateRacePlayerInput(RacePlayer *player);
s32 getRaceInputTimerDecrementBonus(RacePlayer *player);

#endif
