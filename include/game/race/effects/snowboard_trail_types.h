#ifndef SNOWBOARD_TRAIL_TYPES_H
#define SNOWBOARD_TRAIL_TYPES_H

#include "common.h"

/* Active RACE_ITEM_SPEED_FAN model: front follows the board/local offset;
 * back adds an X-axis spin and local translation. The UI expired-fan actor takes over when this expires. */
typedef struct SpeedFanState {
    /* 0x00 */ s16 state;
    /* 0x02 */ u8 pad02[0x04 - 0x02];
    /* 0x04 */ Vec3i localOffset;
    /* 0x10 */ Vec3i backOffset;
    /* 0x1C */ Vec3i worldPos;
    /* 0x28 */ Transform3D frontTransform;
    /* 0x48 */ Transform3D backTransform;
    /* 0x68 */ s16 modelYaw;
    /* 0x6A */ s16 spinAngleX;
    /* 0x6C */ Mtx *frontMatrix;
    /* 0x70 */ Mtx *backMatrix;
    /* 0x74 */ s16 scaleStep;
    /* 0x76 */ u8 matricesDirty;
} SpeedFanState;

typedef char SpeedFanStateSizeCheck[(sizeof(SpeedFanState) == 0x78) ? 1 : -1];

#endif
