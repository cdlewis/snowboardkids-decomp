#ifndef RACE_PLAYER_INPUT_H
#define RACE_PLAYER_INPUT_H

#include "common.h"
#include "game/math/geometry.h"
#include "game/race/effects/snowboard_trail_effects.h"
#include "game/race/player/character_model.h"

#define RACE_PLAYER_COUNT 4
#define RACE_PLAYER_MODEL_PART_CAPACITY 14
#define RACE_PLAYER_COLLISION_SQUASHED 0x200000

typedef u8 RacePlayerOrder[RACE_PLAYER_COUNT];

typedef struct RaceScoreAttackRingTrigger {
    /* 0x00 */ struct RaceScoreAttackRingTrigger *next;
    /* 0x04 */ Vec3i position;
    /* 0x10 */ s32 radius;
    /* 0x14 */ s8 triggered;
    /* 0x15 */ u8 pad15[3];
} RaceScoreAttackRingTrigger;

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
    /* 0x011 */ u8 characterVariant;
    /* 0x012 */ u8 snowboardTextureIndex;
    /* 0x013 */ s8 isActive;
    /* 0x014 */ s8 soundDisabled;
    /* 0x015 */ s8 unk15;
    /* 0x016 */ s8 replayInputSource;
    /* 0x017 */ s8 unk17;
    /* 0x018 */ s16 unk18;
    /* 0x01A */ u8 actionTriggerChance;
    /* 0x01B */ u8 itemTriggerChance;
    /* 0x01C */ Vec3i pos;
    /* 0x028 */ Vec3i unk28;
    /* 0x034 */ Vec3i previousPosition;
    /* 0x040 */ Vec3i velocity;
    /* 0x04C */ char pad4C[0xC];
    /* 0x058 */ s32 groundOffset;
    /* 0x05C */ s32 collisionBottomY;
    /* 0x060 */ s32 targetGroundOffset;
    /* 0x064 */ s32 groundCorrectionY;
    /* 0x068 */ s32 collisionCenterOffset;
    /* 0x06C */ s16 modelRotationX;
    /* 0x06E */ s16 modelRotationY;
    /* 0x070 */ s16 modelRotationZ;
    /* 0x072 */ char pad72[2];
    /* 0x074 */ s32 verticalVelocityLimit;
    /* 0x078 */ char pad78[4];
    /* 0x07C */ s32 stateTimer;
    /* 0x080 */ s32 unk80;
    /* 0x084 */ u32 inputFlags;
    /* 0x088 */ u32 currentInputFlags;
    /* 0x08C */ u32 disabledInputFlags;
    /* 0x090 */ s8 stickX;
    /* 0x091 */ s8 stickY;
    /* 0x092 */ s8 unk92;
    /* 0x093 */ s8 unk93;
    /* 0x094 */ Transform3D modelPartTransforms[RACE_PLAYER_MODEL_PART_CAPACITY];
    /* 0x254 */ s32 localVelocityZ;
    /* 0x258 */ s32 localVelocityX;
    /* 0x25C */ s32 unk25C;
    /* 0x260 */ s32 gravity;
    /* 0x264 */ s32 unk264;
    /* 0x268 */ s32 unk268;
    /* 0x26C */ s32 unk26C;
    /* 0x270 */ s32 unk270;
    /* 0x274 */ s32 unk274;
    /* 0x278 */ s32 unk278;
    /* 0x27C */ s32 unk27C;
    /* 0x280 */ s32 collisionRadius;
    /* 0x284 */ s32 collisionHeight;
    /* 0x288 */ s16 unk288;
    /* 0x28A */ s16 unk28A;
    /* 0x28C */ s16 unk28C;
    /* 0x28E */ char pad28E[2];
    /* 0x290 */ Vec3i projectedPos;
    /* 0x29C */ s32 movementSpeed;
    /* 0x2A0 */ s16 unk2A0;
    /* 0x2A2 */ s16 unk2A2;
    /* 0x2A4 */ s16 unk2A4;
    /* 0x2A6 */ s16 unk2A6;
    /* 0x2A8 */ s16 surfaceCueState[6];
    /* 0x2B4 */ s16 surfaceCueStep[6];
    /* 0x2C0 */ s16 trickAttackPointTotal;
    /* 0x2C2 */ s8 trickAttackBestMakeCount;
    /* 0x2C3 */ s8 trickAttackMakeCount;
    /* 0x2C4 */ u16 trickAttackCompletedTrickMask;
    /* 0x2C6 */ u16 pendingItemHitFlags;
    /* 0x2C8 */ s32 collisionVelocityX;
    /* 0x2CC */ s32 collisionVelocityZ;
    /* 0x2D0 */ char pad2D0[2];
    /* 0x2D2 */ s16 hitSourcePlayerIndex;
    /* 0x2D4 */ s16 unk2D4;
    /* 0x2D6 */ s16 unk2D6;
    /* 0x2D8 */ s16 activeSparkleEffectCount;
    /* 0x2DA */ s16 trailEffectTimer;
    /* 0x2DC */ u8 snowboardTrailSide;
    /* 0x2DD */ char pad2DD[1];
    /* 0x2DE */ s16 unk2DE;
    /* 0x2E0 */ s32 unk2E0;
    /* 0x2E4 */ s32 unk2E4;
    /* 0x2E8 */ s16 unk2E8;
    /* 0x2EA */ s16 pitchAngle;
    /* 0x2EC */ s16 facingAngle;
    /* 0x2EE */ s16 leanAngle;
    /* 0x2F0 */ s16 surfacePitchAngle;
    /* 0x2F2 */ char pad2F2[2];
    /* 0x2F4 */ s16 surfaceRollAngle;
    /* 0x2F6 */ s16 leanInput;
    /* 0x2F8 */ s16 leanScale;
    /* 0x2FA */ s16 unk2FA;
    /* 0x2FC */ u32 stateFlags;
    /* 0x300 */ s16 mode;
    /* 0x302 */ s16 updateState;
    /* 0x304 */ s16 updateTimer;
    /* 0x306 */ s16 unk306;
    /* 0x308 */ s16 subState;
    /* 0x30A */ s16 subStateTimer;
    /* 0x30C */ s16 subStateStep;
    /* 0x30E */ s16 subStateParam;
    /* 0x310 */ s32 unk310;
    /* 0x314 */ s32 speedLimit;
    /* 0x318 */ s32 unk318;
    /* 0x31C */ s16 unk31C;
    /* 0x31E */ s16 unk31E;
    /* 0x320 */ s16 actionSoundTimer;
    /* 0x322 */ char pad322[0xE];
    /* 0x330 */ s8 unk330;
    /* 0x331 */ s8 unk331;
    /* 0x332 */ s16 unk332;
    /* 0x334 */ s16 unk334;
    /* 0x336 */ s16 unk336;
    /* 0x338 */ CharacterModelPart modelParts[RACE_PLAYER_MODEL_PART_CAPACITY];
    /* 0x450 */ s16 modelPartCount;
    /* 0x452 */ s16 animationId;
    /* 0x454 */ s32 animationStartOffset;
    /* 0x458 */ s32 animationFrameDataOffset;
    /* 0x45C */ s16 animationFrameCount;
    /* 0x45E */ s16 animationFramesRemaining;
    /* 0x460 */ char pad460[4];
    /* 0x464 */ s16 animationFrameDuration;
    /* 0x466 */ s16 animationFrameTimer;
    /* 0x468 */ Vec3i markerPoints[RACE_PLAYER_COUNT];
    /* 0x498 */ Vtx *shadowVtx;
    /* 0x49C */ Mtx *shadowMtx;
    /* 0x4A0 */ Vec3i groundMarkerSources[8];
    /* 0x500 */ u8 groundContactMask;
    /* 0x501 */ char pad501[1];
    /* 0x502 */ s16 coursePathIndex;
    /* 0x504 */ s32 coursePathOffset;
    /* 0x508 */ s8 lapDigit;
    /* 0x509 */ s8 rankIndex;
    /* 0x50A */ char pad50A[2];
    /* 0x50C */ union {
        s16 *anglePtr;
        RaceScoreAttackRingTrigger *scoreAttackRingTriggerList;
    };
    /* 0x510 */ s16 shieldEffectTimer;
    /* 0x512 */ s8 itemEffectType;
    /* 0x513 */ s8 itemEffectPalette;
    /* 0x514 */ s8 itemEffectCount;
    /* 0x515 */ s8 actionEffectType;
    /* 0x516 */ s8 actionEffectPalette;
    /* 0x517 */ s8 unk517;
    /* 0x518 */ u8 randomIndex;
    /* 0x519 */ s8 unk519;
    /* 0x51A */ u8 itemTargetFlag;
    /* 0x51B */ char pad51B[1];
    /* 0x51C */ u32 checkpointEventMask;
    /* 0x520 */ s32 smoothedPathOffset;
    /* 0x524 */ s8 checkpointHit;
    /* 0x525 */ s8 unk525;
    /* 0x526 */ char pad526[1];
    /* 0x527 */ s8 surfaceCueOverrideMask;
    /* 0x528 */ s8 unk528;
    /* 0x529 */ s8 displayRank;
    /* 0x52A */ s8 rankArrow;
    /* 0x52B */ s8 rankTargetPlayer;
    /* 0x52C */ s8 actionTriggerCooldown;
    /* 0x52D */ s8 itemTriggerCooldown;
    /* 0x52E */ char pad52E[2];
    /* 0x530 */ Mtx *modelPartMatrices[RACE_PLAYER_MODEL_PART_CAPACITY];
    /* 0x568 */ s32 score;
    /* 0x56C */ s32 cumulativeRaceScore;
    /* 0x570 */ s16 collectedCourseCoinMarkerCount;
    /* 0x572 */ s16 totalCourseCoinMarkerCount;
    /* 0x574 */ s16 scoreAttackPointTotal;
    /* 0x576 */ s16 scoreAttackPointTarget;
    /* 0x578 */ s16 unk578;
    /* 0x57A */ s8 unk57A;
    /* 0x57B */ s8 unk57B;
    /* 0x57C */ s16 unk57C;
    /* 0x57E */ s16 progressMeterPosition;
    /* 0x580 */ s8 progressMeterSquashFrame;
    /* 0x581 */ s8 unk581;
    /* 0x582 */ s16 unk582;
    /* 0x584 */ s16 unk584;
    /* 0x586 */ char pad586[2];
    /* 0x588 */ f32 unk588;
    /* 0x58C */ SnowboardTrailState snowboardTrail;
    /* 0x604 */ s16 rumbleTimer;
    /* 0x606 */ s16 rumblePatternIndex;
    /* 0x608 */ s8 rumblePatternId;
    /* 0x609 */ char pad609[1];
    /* 0x60A */ s16 replayFrame;
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
