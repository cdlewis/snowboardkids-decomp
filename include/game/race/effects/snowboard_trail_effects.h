#ifndef SNOWBOARD_TRAIL_EFFECTS_H
#define SNOWBOARD_TRAIL_EFFECTS_H

#include "common.h"
#include "game/race/effects/snowboard_trail_types.h"
#include "game/race/player/race_player_input.h"

void renderSpeedFanEffect(SpeedFanState *trail);
void updateSpeedFanEffect(RacePlayer *player);
void startSpeedFanEffect(RacePlayer *player);

#endif
