#ifndef SNOWBOARD_TRAIL_EFFECTS_H
#define SNOWBOARD_TRAIL_EFFECTS_H

#include "game/math/geometry.h"

struct RacePlayer;

typedef struct SnowboardTrailState {
    /* 0x00 */ s16 state;
    /* 0x02 */ u8 pad02[0x04 - 0x02];
    /* 0x04 */ Vec3i localOffset;
    /* 0x10 */ Vec3i scale;
    /* 0x1C */ Vec3i worldPos;
    /* 0x28 */ Transform3D frontTransform;
    /* 0x48 */ Transform3D backTransform;
    /* 0x68 */ s16 modelYaw;
    /* 0x6A */ s16 spinYaw;
    /* 0x6C */ void *frontDisplayList;
    /* 0x70 */ void *backDisplayList;
    /* 0x74 */ s16 scaleStep;
    /* 0x76 */ u8 displayListsDirty;
} SnowboardTrailState;

typedef char SnowboardTrailStateSizeCheck[(sizeof(SnowboardTrailState) == 0x78) ? 1 : -1];

#define renderSnowboardTrailEffect func_800837D0
#define updateSnowboardTrailEffect func_8008393C

void renderSnowboardTrailEffect(SnowboardTrailState *trail);
void updateSnowboardTrailEffect(struct RacePlayer *player);
void startSnowboardTrailEffect(struct RacePlayer *player);

#endif
