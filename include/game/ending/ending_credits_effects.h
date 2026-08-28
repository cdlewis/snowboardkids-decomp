#ifndef ENDING_CREDITS_EFFECTS_H
#define ENDING_CREDITS_EFFECTS_H

#include "common.h"
#include "game/menu/race_setup/race_setup_ui.h"

typedef struct EndingCreditsEffectActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
} EndingCreditsEffectActor; // size = 0x1C

typedef struct EndingCreditsAnimatedEffectActor {
    /* 0x00 */ EndingCreditsEffectActor common;
    /* 0x1C */ u16 frame;
    /* 0x1E */ u16 frameTimer;
} EndingCreditsAnimatedEffectActor; // size = 0x20

typedef struct EndingCreditsDelayedSparkleActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 durationMode;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ u16 frame;
    /* 0x1E */ u16 frameTimer;
} EndingCreditsDelayedSparkleActor; // size = 0x20

typedef struct EndingCreditsAttachedEffectActor {
    /* 0x00 */ EndingCreditsEffectActor common;
    /* 0x1C */ s16 offsetX;
    /* 0x1E */ s16 offsetY;
    /* 0x20 */ u16 frame;
    /* 0x22 */ u16 frameTimer;
    /* 0x24 */ u8 pad24;
    /* 0x25 */ u8 palette;
    /* 0x26 */ u8 characterId;
    /* 0x27 */ u8 pad27;
} EndingCreditsAttachedEffectActor; // size = 0x28

typedef struct EndingCreditsTommyBigBurstActor {
    /* 0x00 */ EndingCreditsEffectActor common;
    /* 0x1C */ s16 burstX;
    /* 0x1E */ s16 burstY;
    /* 0x20 */ u16 frame;
    /* 0x22 */ u16 frameTimer;
    /* 0x24 */ u16 loopCount;
    /* 0x26 */ u8 phase;
    /* 0x27 */ u8 pad27;
} EndingCreditsTommyBigBurstActor; // size = 0x28

typedef struct EndingCreditsNancySpeedLinesActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 xPositions[3];
    /* 0x1E */ s16 yPositions[3];
    /* 0x24 */ u16 lineIndex;
    /* 0x26 */ u16 timer;
} EndingCreditsNancySpeedLinesActor; // size = 0x28

typedef struct EndingCreditsTransitionSpriteActor {
    /* 0x00 */ EndingCreditsEffectActor common;
    /* 0x1C */ u16 wipeTimer;
    /* 0x1E */ u16 pad1E;
    /* 0x20 */ MenuTilemapSprite sprite;
} EndingCreditsTransitionSpriteActor; // size = 0x48

enum EndingCreditsCharacterId {
    ENDING_CREDITS_CHARACTER_SLASH,
    ENDING_CREDITS_CHARACTER_NANCY,
    ENDING_CREDITS_CHARACTER_JAM,
    ENDING_CREDITS_CHARACTER_LINDA,
    ENDING_CREDITS_CHARACTER_TOMMY,
    ENDING_CREDITS_CHARACTER_COUNT
};

extern u8 gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_COUNT];

void spawnEndingCreditsCharacterVanishPoof(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void initEndingCreditsNancySpeedLines(EndingCreditsNancySpeedLinesActor *arg0);
void spawnEndingCreditsCharacterLoopingSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingCreditsTommyBigBurst(EndingCreditsTommyBigBurstActor *arg0);
void initEndingCreditsSlashRisingStar(EndingCreditsAnimatedEffectActor *arg0);
void initEndingCreditsNancyRunDust(EndingCreditsAnimatedEffectActor *arg0);
void spawnEndingCreditsSmallBurst(s16 arg0, s16 arg1);
void spawnEndingCreditsCharacterAura(s16 arg0, s16 arg1, u8 arg2, u8 arg3);
void spawnEndingCreditsPhaseAdvanceSparkle(s16 arg0, s16 arg1);
void spawnEndingCreditsDelayedSparkle(s16 arg0, s16 arg1, u8 arg2);
void initEndingCreditsTommySnowmanEntrance(EndingCreditsAnimatedEffectActor *arg0);
void initEndingCreditsTransitionSnowboardIcon(EndingCreditsTransitionSpriteActor *arg0);
void initEndingCreditsTransitionSnowflakeIcon(EndingCreditsTransitionSpriteActor *arg0);
void initEndingCreditsTransitionLogoWipe(EndingCreditsTransitionSpriteActor *arg0);

#endif
