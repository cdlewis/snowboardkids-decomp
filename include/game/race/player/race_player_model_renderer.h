#ifndef RACE_PLAYER_MODEL_RENDERER_H
#define RACE_PLAYER_MODEL_RENDERER_H

#include "common.h"
#include "game/race/player/race_player_input.h"

#define RACE_PLAYER_MODEL_RENDERER_PART_COUNT 13
#define RACE_PLAYER_MODEL_RENDERER_TEXTURE_VARIANTS 6
#define RACE_PLAYER_MODEL_RENDERER_PLAYER_PART_CAPACITY RACE_PLAYER_MODEL_PART_CAPACITY
#define RACE_PLAYER_MODEL_RENDERER_PLAYER_COUNT 4

#define RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW 0x80
#define RACE_PLAYER_MODEL_RENDERER_FLAG_SHADOW_READY 0x100
#define RACE_PLAYER_MODEL_RENDERER_FLAG_HIDE_MESHES 0x400000

extern Mtx gRacePlayerShadowMatrixTemplate;
extern Gfx gRacePlayerShadowRenderSetupDisplayList[];
extern Gfx *gRacePlayerModelRootPartDisplayLists[RACE_PLAYER_MODEL_RENDERER_PART_COUNT + 2];
extern Gfx *gRaceGhostPlayerModelRootPartDisplayLists[RACE_PLAYER_MODEL_RENDERER_PART_COUNT + 2];

void drawRacePlayerGroundShadow(RacePlayer *player);
void drawRacePlayerModelRootPart(void *asset, s16 dlIndex, s16 textureIndex);
void drawRaceGhostPlayerModelRootPart(void *asset, s16 dlIndex, s16 textureIndex);
void drawRacePlayerModel(RacePlayer *player);
void drawRaceGhostPlayerModel(RacePlayer *player);

#endif
