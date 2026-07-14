#ifndef RACE_PLAYER_INPUT_H
#define RACE_PLAYER_INPUT_H

#include "common.h"

#define RACE_PLAYER_COUNT 4

typedef s16 Matrix4s[0x10];

typedef struct RaceVec3i {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 z;
} RaceVec3i;

typedef struct RacePlayer {
    /* 0x000 */ union {
        s16 playerIndex;
        u16 playerIndexU16;
        u16 playerIndexU;
    };
    /* 0x002 */ s16 unk2;
    /* 0x004 */ union {
        u8 unk4;
        u8 isCpu;
    };
    /* 0x005 */ u8 selectedCharacterId;
    /* 0x006 */ u8 menuSelection;
    /* 0x007 */ u8 selectionUnlockState;
    /* 0x008 */ u8 menuState;
    /* 0x009 */ u8 unk9;
    /* 0x00A */ u8 unkA;
    /* 0x00B */ u8 unkB;
    /* 0x00C */ s32 unkC;
    /* 0x010 */ u8 characterId;
    /* 0x011 */ u8 unk11;
    /* 0x012 */ u8 unk12;
    /* 0x013 */ s8 isActive;
    /* 0x014 */ s8 soundDisabled;
    /* 0x015 */ s8 unk15;
    /* 0x016 */ s8 replayInputSource;
    /* 0x017 */ u8 unk17;
    /* 0x018 */ s16 unk18;
    /* 0x01A */ u8 actionTriggerChance;
    /* 0x01B */ u8 itemTriggerChance;
    /* 0x01C */ union {
        RaceVec3i pos;
        struct {
            s32 posX;
            s32 posY;
            s32 posZ;
        };
    };
    /* 0x028 */ RaceVec3i unk28;
    /* 0x034 */ RaceVec3i unk34;
    /* 0x040 */ union {
        RaceVec3i unk40;
        RaceVec3i velocity;
        struct {
            s32 unk40_x;
            s32 unk44;
            s32 unk48;
        };
    };
    /* 0x04C */ char pad4C[0xC];
    /* 0x058 */ s32 unk58;
    /* 0x05C */ s32 unk5C;
    /* 0x060 */ s32 unk60;
    /* 0x064 */ s32 unk64;
    /* 0x068 */ s32 unk68;
    /* 0x06C */ s16 unk6C;
    /* 0x06E */ s16 unk6E;
    /* 0x070 */ s16 unk70;
    /* 0x072 */ char pad72[2];
    /* 0x074 */ s32 unk74;
    /* 0x078 */ char pad78[4];
    /* 0x07C */ union {
        s32 stateTimer;
        struct {
            s16 stateTimerHigh;
            s16 stateTimerLow;
        };
        struct {
            s16 unk7C;
            s16 unk7E;
        };
    };
    /* 0x080 */ s32 unk80;
    /* 0x084 */ u32 inputFlags;
    /* 0x088 */ u32 currentInputFlags;
    /* 0x08C */ u32 disabledInputFlags;
    /* 0x090 */ s8 stickX;
    /* 0x091 */ s8 stickY;
    /* 0x092 */ s8 unk92;
    /* 0x093 */ s8 unk93;
    /* 0x094 */ char pad94[0x1C0];
    /* 0x254 */ s32 unk254;
    /* 0x258 */ s32 unk258;
    /* 0x25C */ s32 unk25C;
    /* 0x260 */ s32 unk260;
    /* 0x264 */ s32 unk264;
    /* 0x268 */ s32 unk268;
    /* 0x26C */ s32 unk26C;
    /* 0x270 */ s32 unk270;
    /* 0x274 */ s32 unk274;
    /* 0x278 */ s32 unk278;
    /* 0x27C */ s32 unk27C;
    /* 0x280 */ s32 unk280;
    /* 0x284 */ s32 unk284;
    /* 0x288 */ char pad288[8];
    /* 0x290 */ RaceVec3i projectedPos;
    /* 0x29C */ s32 unk29C;
    /* 0x2A0 */ s16 unk2A0;
    /* 0x2A2 */ s16 unk2A2;
    /* 0x2A4 */ s16 unk2A4;
    /* 0x2A6 */ s16 unk2A6;
    /* 0x2A8 */ s16 surfaceCueState[6];
    /* 0x2B4 */ s16 surfaceCueStep[6];
    /* 0x2C0 */ s16 unk2C0;
    /* 0x2C2 */ s8 unk2C2;
    /* 0x2C3 */ s8 unk2C3;
    /* 0x2C4 */ u16 unk2C4;
    /* 0x2C6 */ u16 pendingItemHitFlags;
    /* 0x2C8 */ s32 unk2C8;
    /* 0x2CC */ s32 unk2CC;
    /* 0x2D0 */ char pad2D0[2];
    /* 0x2D2 */ s16 unk2D2;
    /* 0x2D4 */ s16 unk2D4;
    /* 0x2D6 */ s16 unk2D6;
    /* 0x2D8 */ s16 unk2D8;
    /* 0x2DA */ s16 trailEffectTimer;
    /* 0x2DC */ u8 unk2DC;
    /* 0x2DD */ char pad2DD[1];
    /* 0x2DE */ s16 unk2DE;
    /* 0x2E0 */ s32 unk2E0;
    /* 0x2E4 */ s32 unk2E4;
    /* 0x2E8 */ s16 unk2E8;
    /* 0x2EA */ s16 pitchAngle;
    /* 0x2EC */ s16 facingAngle;
    /* 0x2EE */ s16 unk2EE;
    /* 0x2F0 */ s16 unk2F0;
    /* 0x2F2 */ char pad2F2[2];
    /* 0x2F4 */ s16 unk2F4;
    /* 0x2F6 */ s16 unk2F6;
    /* 0x2F8 */ s16 unk2F8;
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
    /* 0x314 */ s32 unk314;
    /* 0x318 */ char pad318[4];
    /* 0x31C */ s16 unk31C;
    /* 0x31E */ s16 unk31E;
    /* 0x320 */ s16 actionSoundTimer;
    /* 0x322 */ char pad322[0xE];
    /* 0x330 */ s8 unk330;
    /* 0x331 */ s8 unk331;
    /* 0x332 */ s16 unk332;
    /* 0x334 */ s16 unk334;
    /* 0x336 */ s16 unk336;
    /* 0x338 */ char pad338[2];
    /* 0x33A */ s16 unk33A;
    /* 0x33C */ s16 unk33C;
    /* 0x33E */ s16 unk33E;
    /* 0x340 */ s32 unk340;
    /* 0x344 */ s32 unk344;
    /* 0x348 */ s32 unk348;
    /* 0x34C */ char pad34C[0x106];
    /* 0x452 */ s16 animationId;
    /* 0x454 */ char pad454[0x14];
    /* 0x468 */ RaceVec3i markerPoints[RACE_PLAYER_COUNT];
    /* 0x498 */ char pad498[8];
    /* 0x4A0 */ RaceVec3i unk4A0;
    /* 0x4AC */ RaceVec3i unk4AC;
    /* 0x4B8 */ RaceVec3i unk4B8;
    /* 0x4C4 */ RaceVec3i unk4C4;
    /* 0x4D0 */ RaceVec3i unk4D0;
    /* 0x4DC */ RaceVec3i unk4DC;
    /* 0x4E8 */ RaceVec3i unk4E8;
    /* 0x4F4 */ RaceVec3i unk4F4;
    /* 0x500 */ u8 unk500;
    /* 0x501 */ char pad501[1];
    /* 0x502 */ s16 unk502;
    /* 0x504 */ s32 unk504;
    /* 0x508 */ s8 unk508;
    /* 0x509 */ union {
        s8 rankIndex;
        s8 racePosition;
    };
    /* 0x50A */ char pad50A[6];
    /* 0x510 */ s16 shieldEffectTimer;
    /* 0x512 */ s8 itemEffectType;
    /* 0x513 */ s8 itemEffectPalette;
    /* 0x514 */ s8 itemEffectCount;
    /* 0x515 */ s8 actionEffectType;
    /* 0x516 */ s8 actionEffectPalette;
    /* 0x517 */ s8 unk517;
    /* 0x518 */ s8 randomIndex;
    /* 0x519 */ s8 unk519;
    /* 0x51A */ u8 actionEffectEnabled;
    /* 0x51B */ char pad51B[9];
    /* 0x524 */ s8 unk524;
    /* 0x525 */ s8 unk525;
    /* 0x526 */ char pad526[1];
    /* 0x527 */ s8 surfaceCueOverrideMask;
    /* 0x528 */ s8 unk528;
    /* 0x529 */ char pad529[3];
    /* 0x52C */ s8 actionTriggerCooldown;
    /* 0x52D */ s8 itemTriggerCooldown;
    /* 0x52E */ char pad52E[0x3A];
    /* 0x568 */ s32 unk568;
    /* 0x56C */ s32 unk56C;
    /* 0x570 */ s16 unk570;
    /* 0x572 */ s16 unk572;
    /* 0x574 */ s16 unk574;
    /* 0x576 */ s16 unk576;
    /* 0x578 */ s16 unk578;
    /* 0x57A */ s8 unk57A;
    /* 0x57B */ s8 unk57B;
    /* 0x57C */ s16 unk57C;
    /* 0x57E */ s16 unk57E;
    /* 0x580 */ s8 unk580;
    /* 0x581 */ s8 unk581;
    /* 0x582 */ s16 unk582;
    /* 0x584 */ s16 unk584;
    /* 0x586 */ char pad586[2];
    /* 0x588 */ f32 unk588;
    /* 0x58C */ char pad58C[0x78];
    /* 0x604 */ union {
        s16 actionEffectLevel;
        s16 rumbleTimer;
    };
    /* 0x606 */ s16 rumblePatternIndex;
    /* 0x608 */ union {
        s8 actionEffectFrame;
        s8 rumblePatternId;
    };
    /* 0x609 */ char pad609[1];
    /* 0x60A */ s16 replayFrame;
} RacePlayer;

extern RacePlayer gRacePlayers[RACE_PLAYER_COUNT];

#define gRacePlayersEnd (&gRacePlayers[RACE_PLAYER_COUNT])

void recordRaceReplayInputFrame(RacePlayer *player);
void playRaceReplayInputFrame(RacePlayer *player);
void recordRaceInputHistoryFrame(RacePlayer *player);
void playRaceInputHistoryFrame(RacePlayer *player);
void updateRacePlayerInput(RacePlayer *player);
s32 getRaceInputTimerDecrementBonus(RacePlayer *player);

#endif
