#ifndef SNOWBOARD_TRAIL_EFFECTS_H
#define SNOWBOARD_TRAIL_EFFECTS_H

#include "common.h"

struct RacePlayer;

/* Active RACE_ACTION_SPEED_FAN model: front follows the board/local offset;
 * back adds spinYaw. The UI expired-fan actor takes over when this expires. */
typedef struct SpeedFanState {
    /* 0x00 */ s16 state;
    /* 0x02 */ u8 pad02[0x04 - 0x02];
    /* 0x04 */ Vec3i localOffset;
    /* 0x10 */ Vec3i scale;
    /* 0x1C */ Vec3i worldPos;
    /* 0x28 */ Transform3D frontTransform;
    /* 0x48 */ Transform3D backTransform;
    /* 0x68 */ s16 modelYaw;
    /* 0x6A */ s16 spinYaw;
    /* 0x6C */ Mtx *frontMatrix;
    /* 0x70 */ Mtx *backMatrix;
    /* 0x74 */ s16 scaleStep;
    /* 0x76 */ u8 matricesDirty;
} SpeedFanState;

typedef char SpeedFanStateSizeCheck[(sizeof(SpeedFanState) == 0x78) ? 1 : -1];

void renderSpeedFanEffect(SpeedFanState *trail);
void updateSpeedFanEffect(struct RacePlayer *player);
void startSpeedFanEffect(struct RacePlayer *player);

#endif
