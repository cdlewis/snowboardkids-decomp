#ifndef SNOWBOARD_TRAIL_EFFECTS_H
#define SNOWBOARD_TRAIL_EFFECTS_H

struct RaceInputPlayer;
typedef struct SnowboardTrailState SnowboardTrailState;

#define updateSnowboardTrailEffect func_8008393C

void func_800837D0(SnowboardTrailState *trail);
void updateSnowboardTrailEffect(struct RaceInputPlayer *player);
void startSnowboardTrailEffect(struct RaceInputPlayer *player);

#endif
