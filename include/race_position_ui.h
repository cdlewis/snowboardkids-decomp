#ifndef RACE_POSITION_UI_H
#define RACE_POSITION_UI_H

#include "common.h"

typedef struct RacePositionUiPlayer RacePositionUiPlayer;

#define drawRacePositionUiPlayerModel func_8007C5E8
#define drawRacePositionUiPlayerMarker func_8007BE80
#define drawRacePositionUiPlayerGhostModel func_8007CBC0

void drawRacePositionUiPlayerMarker(RacePositionUiPlayer *player);
void func_8007C130(void *asset, s16 dlIndex, s16 textureIndex);
void func_8007C38C(void *asset, s16 dlIndex, s16 textureIndex);
void drawRacePositionUiPlayerModel(RacePositionUiPlayer *player);
void drawRacePositionUiPlayerGhostModel(RacePositionUiPlayer *player);

#endif
