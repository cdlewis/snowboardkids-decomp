#ifndef RACE_PLAYER_MODEL_RENDERER_H
#define RACE_PLAYER_MODEL_RENDERER_H

#include "common.h"

typedef struct RacePlayerModelRenderState RacePlayerModelRenderState;

void drawRacePlayerGroundShadow(RacePlayerModelRenderState *player);
void drawRacePlayerModelBaseDisplayList(void *asset, s16 dlIndex, s16 textureIndex);
void drawRaceGhostPlayerModelBaseDisplayList(void *asset, s16 dlIndex, s16 textureIndex);
void drawRacePlayerModel(RacePlayerModelRenderState *player);
void drawRaceGhostPlayerModel(RacePlayerModelRenderState *player);

#endif
