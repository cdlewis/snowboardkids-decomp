#ifndef RACE_ITEM_EFFECTS_H
#define RACE_ITEM_EFFECTS_H

#include "common.h"
#include "game/math/spatial_math.h"

typedef union {
    Vec3i vec;
    struct {
        /* 0x18 */ s16 x;
        /* 0x1A */ s16 y;
        /* 0x1C */ u16 frame;
        /* 0x1E */ u8 pad1E[2];
        /* 0x20 */ s8 colorR;
        /* 0x21 */ s8 colorG;
        /* 0x22 */ s8 colorB;
    } sprite;
} RaceItemEffectPayload;

typedef union {
    s32 word;
    void *palette;
    struct {
        /* 0x28 */ s8 unk28;
        /* 0x29 */ s8 phase;
        /* 0x2A */ s8 unk2A;
        /* 0x2B */ s8 unk2B;
    } bytes;
} RaceItemEffectWord28;

typedef union {
    s32 word;
    void *image;
} RaceItemEffectWord2C;

typedef union {
    s32 velocityX;
    s16 timer;
} RaceItemEffectWord24;

typedef union {
    Vec3i vec;
    struct {
        RaceItemEffectWord24 word24;
        RaceItemEffectWord28 word28;
        RaceItemEffectWord2C word2C;
    } fields;
} RaceItemEffectVector24;

typedef union {
    s16 halfword;
    s8 byte;
    u8 ubyte;
    struct {
        /* 0x34 */ s8 byte0;
        /* 0x35 */ s8 matrixDirty;
    } bytes;
} RaceItemEffectState;

typedef union {
    struct {
        /* 0x30 */ s16 x;
        /* 0x32 */ s16 y;
    } screen;
    void *matrix;
    struct {
        /* 0x30 */ s16 drawInitialized;
        /* 0x32 */ s16 delay;
    } particle;
} RaceItemEffectShorts30;

typedef union {
    s16 width;
    s16 alpha;
} RaceItemEffectHalf38;

typedef union {
    s16 halfword;
    s8 byte;
} RaceItemEffectHeight;

typedef union {
    void *matrix;
    struct {
        /* 0x34 */ RaceItemEffectState state;
        /* 0x36 */ RaceItemEffectHeight height;
    } shorts;
} RaceItemEffectWord34;

typedef struct RaceItemEffectActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 playerIndex;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ RaceItemEffectPayload payload;
    /* 0x24 */ RaceItemEffectVector24 vector24;
    /* 0x30 */ RaceItemEffectShorts30 unk30;
    /* 0x34 */ RaceItemEffectWord34 unk34;
    /* 0x38 */ RaceItemEffectHalf38 unk38;
    /* 0x3A */ u8 pad3A[2];
    /* 0x3C */ void *image;
    /* 0x40 */ void *palette;
    /* 0x44 */ u8 pad44[9];
    /* 0x4D */ u8 angleIndex;
    /* 0x4E */ u8 pad4E;
    /* 0x4F */ u8 followPlayerIndex;
    /* 0x50 */ u8 pad50[0x64 - 0x50];
    /* 0x64 */ s16 unk64;
    /* 0x66 */ u8 pad66[2];
    /* 0x68 */ u8 *unk68;
} RaceItemEffectActor;

typedef struct RaceItemFollowActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 playerIndex;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ Vec3i pos1;
    /* 0x24 */ Vec3i pos2;
    /* 0x30 */ Vec3i offset1;
    /* 0x3C */ Vec3i offset2;
    /* 0x48 */ void *matrix1;
    /* 0x4C */ void *matrix2;
    /* 0x50 */ s8 dirty;
    /* 0x51 */ s8 timer;
} RaceItemFollowActor;

typedef struct RaceItemTextureActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ void *images[4];
    /* 0x28 */ void *palettes[4];
} RaceItemTextureActor;

s32 getRaceItemEffectType(s32 arg0);
void updateRaceItemSparkBurst(RaceItemEffectActor *arg0);
void initRaceItemSparkBurst(RaceItemEffectActor *arg0);
void spawnRaceItemTrackSparkBurst(Vec3i *arg0, Vec3i *arg1, Vec3i *arg2, Vec3i *arg3, s32 arg4, s16 arg5);
void renderRacePlayerHitEffect(RaceItemEffectActor *arg0);
void updateRacePlayerHitEffect(RaceItemEffectActor *arg0);
void initRacePlayerHitEffect(RaceItemEffectActor *arg0);
void spawnRacePlayerHitEffect(s16 arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4);
void spawnRaceItemImpactEffect(s32 arg0, s32 arg1, s32 arg2, s16 arg3);
void renderRaceItemProjectileTrailEffect(RaceItemEffectActor *arg0);
void updateRaceItemProjectileTrailEffect(RaceItemEffectActor *arg0);
void initRaceItemProjectileTrailEffect(RaceItemEffectActor *arg0);
void spawnRaceItemProjectileTrailEffect(s32 arg0, s32 arg1, s32 arg2, s16 arg3);
void renderRacePlayerShockEffect(RaceItemEffectActor *arg0);
void updateRacePlayerShockEffect(RaceItemEffectActor *arg0);
void initRacePlayerShockEffect(RaceItemEffectActor *arg0);
void renderRaceItemBreakParticle(RaceItemEffectActor *arg0);
void updateRaceItemBreakParticle(RaceItemEffectActor *arg0);
void initRaceItemBreakParticle(RaceItemEffectActor *arg0);
void spawnRaceItemBreakParticles(s16 playerIndex, s16 itemIndex);
void renderRacePlayerRecoverySparkle(RaceItemEffectActor *arg0);
void updateRacePlayerRecoverySparkle(RaceItemEffectActor *arg0);
void initRacePlayerRecoverySparkle(RaceItemEffectActor *arg0);
void renderRacePlayerSnowSpray(RaceItemFollowActor *arg0);
void updateRacePlayerSnowSpray(RaceItemFollowActor *arg0);
void initRacePlayerSnowSpray(RaceItemFollowActor *arg0);
void renderRaceUiSparkle(RaceItemEffectActor *arg0);
void updateRaceUiSparkle(RaceItemEffectActor *arg0);
void initRaceUiSparkle(RaceItemEffectActor *arg0);
void spawnRaceUiSparkle(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4);
void renderRaceItemTextureEffects(RaceItemTextureActor *arg0);
void updateRaceItemTextureEffects(RaceItemTextureActor *arg0);
void initRaceItemTextureEffects(RaceItemTextureActor *arg0);
void renderRacePlayerLandingSnowSpray(RaceItemFollowActor *arg0);
void updateRacePlayerLandingSnowSpray(RaceItemFollowActor *arg0);
void initRacePlayerLandingSnowSpray(RaceItemFollowActor *arg0);

#endif
