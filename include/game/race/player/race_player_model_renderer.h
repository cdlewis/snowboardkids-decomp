#ifndef RACE_PLAYER_MODEL_RENDERER_H
#define RACE_PLAYER_MODEL_RENDERER_H

#include "common.h"

#define RACE_PLAYER_MODEL_RENDERER_PART_COUNT 13
#define RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS 6
#define RACE_PLAYER_MODEL_RENDERER_PLAYER_PART_CAPACITY 14
#define RACE_PLAYER_MODEL_RENDERER_PLAYER_COUNT 4

#define RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW 0x80
#define RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW_READY 0x100
#define RACE_PLAYER_MODEL_RENDERER_FLAG_HIDE_MESHES 0x400000

typedef struct RacePlayerShadowVtx {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ u16 flag;
    /* 0x08 */ s16 s;
    /* 0x0A */ s16 t;
    /* 0x0C */ u8 r;
    /* 0x0D */ u8 g;
    /* 0x0E */ u8 b;
    /* 0x0F */ u8 a;
} RacePlayerShadowVtx;

typedef struct RacePlayerShadowMtx {
    /* 0x00 */ u32 words[16];
} RacePlayerShadowMtx;

typedef struct RacePlayerModelRenderState {
    /* 0x000 */ u16 playerIndex;
    /* 0x002 */ u8 pad02[0x10 - 0x02];
    /* 0x010 */ u8 textureSet;
    /* 0x011 */ u8 texHeaderIndex;
    /* 0x012 */ u8 textureVariant;
    /* 0x013 */ u8 pad13[0x94 - 0x13];
    /* 0x094 */ Transform3D partSources[RACE_PLAYER_MODEL_RENDERER_PLAYER_PART_CAPACITY];
    /* 0x254 */ u8 pad254[0x2FC - 0x254];
    /* 0x2FC */ u32 flags;
    /* 0x300 */ u8 pad300[0x320 - 0x300];
    /* 0x320 */ s16 blinkTimer;
    /* 0x322 */ u8 pad322[0x450 - 0x322];
    /* 0x450 */ s16 partCount;
    /* 0x452 */ u8 pad452[0x468 - 0x452];
    /* 0x468 */ Vec3i shadowPoints[RACE_PLAYER_MODEL_RENDERER_PLAYER_COUNT];
    /* 0x498 */ RacePlayerShadowVtx *shadowVtx;
    /* 0x49C */ RacePlayerShadowMtx *shadowMtx;
    /* 0x4A0 */ u8 pad4A0[0x530 - 0x4A0];
    /* 0x530 */ void *partVtx[RACE_PLAYER_MODEL_RENDERER_PLAYER_PART_CAPACITY];
} RacePlayerModelRenderState;

typedef char RacePlayerShadowVtxSizeCheck[(sizeof(RacePlayerShadowVtx) == 0x10) ? 1 : -1];
typedef char RacePlayerShadowMtxSizeCheck[(sizeof(RacePlayerShadowMtx) == 0x40) ? 1 : -1];

extern u32 gRacePlayerShadowMatrixTemplate[16];
extern Gfx gRacePlayerShadowRenderSetupDisplayList[];
extern Gfx *gRacePlayerModelRootPartDisplayLists[RACE_PLAYER_MODEL_RENDERER_PART_COUNT + 2];
extern Gfx *gRaceGhostPlayerModelRootPartDisplayLists[RACE_PLAYER_MODEL_RENDERER_PART_COUNT + 2];

void drawRacePlayerGroundShadow(RacePlayerModelRenderState *player);
void drawRacePlayerModelRootPart(void *asset, s16 dlIndex, s16 textureIndex);
void drawRaceGhostPlayerModelRootPart(void *asset, s16 dlIndex, s16 textureIndex);
void drawRacePlayerModel(RacePlayerModelRenderState *player);
void drawRaceGhostPlayerModel(RacePlayerModelRenderState *player);

#endif
