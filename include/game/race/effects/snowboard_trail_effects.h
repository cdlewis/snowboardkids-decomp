#ifndef SNOWBOARD_TRAIL_EFFECTS_H
#define SNOWBOARD_TRAIL_EFFECTS_H

struct RacePlayer;
typedef struct SnowboardTrailState SnowboardTrailState;

#define renderSnowboardTrailEffect func_800837D0
#define updateSnowboardTrailEffect func_8008393C

void renderSnowboardTrailEffect(SnowboardTrailState *trail);
void updateSnowboardTrailEffect(struct RacePlayer *player);
void startSnowboardTrailEffect(struct RacePlayer *player);

#endif
