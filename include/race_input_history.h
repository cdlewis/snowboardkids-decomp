#ifndef RACE_INPUT_HISTORY_H
#define RACE_INPUT_HISTORY_H

#include "common.h"

#define RACE_INPUT_PLAYER_COUNT 4

typedef struct {
    /* 0x000 */ u16 playerIndex;
    /* 0x002 */ char pad2[2];
    /* 0x004 */ u8 unk4;
    /* 0x005 */ char pad5[0xE];
    /* 0x013 */ s8 isActive;
    /* 0x014 */ char pad14[1];
    /* 0x015 */ s8 unk15;
    /* 0x016 */ s8 replayInputSource;
    /* 0x017 */ char pad17[3];
    /* 0x01A */ u8 actionTriggerChance;
    /* 0x01B */ u8 itemTriggerChance;
    /* 0x01C */ s32 posX;
    /* 0x020 */ char pad20[4];
    /* 0x024 */ s32 posZ;
    /* 0x028 */ char pad28[0x5C];
    /* 0x084 */ u32 inputFlags;
    /* 0x088 */ u32 currentInputFlags;
    /* 0x08C */ u32 disabledInputFlags;
    /* 0x090 */ s8 stickX;
    /* 0x091 */ s8 stickY;
    /* 0x092 */ char pad92[0x248];
    /* 0x2DA */ s16 trailEffectTimer;
    /* 0x2DC */ char pad2DC[0x10];
    /* 0x2EC */ s16 facingAngle;
    /* 0x2EE */ char pad2EE[0xE];
    /* 0x2FC */ u32 stateFlags;
    /* 0x300 */ char pad300[2];
    /* 0x302 */ s16 updateState;
    /* 0x304 */ s16 updateTimer;
    /* 0x306 */ char pad306[0x1A];
    /* 0x320 */ s16 actionSoundTimer;
    /* 0x322 */ char pad322[0x1EE];
    /* 0x510 */ s16 shieldEffectTimer;
    /* 0x512 */ s8 itemEffectType;
    /* 0x513 */ char pad513[1];
    /* 0x514 */ s8 itemEffectCount;
    /* 0x515 */ s8 actionEffectType;
    /* 0x516 */ char pad516[4];
    /* 0x51A */ u8 actionEffectEnabled;
    /* 0x51B */ char pad51B[0x11];
    /* 0x52C */ s8 actionTriggerCooldown;
    /* 0x52D */ s8 itemTriggerCooldown;
    /* 0x52E */ char pad52E[0x54];
    /* 0x582 */ s16 unk582;
    /* 0x584 */ char pad584[0x86];
    /* 0x60A */ s16 replayFrame;
} RaceInputPlayer;

extern RaceInputPlayer D_80121D80[RACE_INPUT_PLAYER_COUNT];
extern RaceInputPlayer D_8012238C[];
extern RaceInputPlayer D_80122998[];
extern RaceInputPlayer D_80122FA4[];

void func_80084730(RaceInputPlayer *player);
s32 func_80084958(RaceInputPlayer *player);

#endif
