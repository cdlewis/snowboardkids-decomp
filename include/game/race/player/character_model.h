#ifndef CHARACTER_MODEL_H
#define CHARACTER_MODEL_H

#include "common.h"

typedef struct CharacterModelPart {
    /* 0x00 */ s8 partId;
    /* 0x01 */ u8 parentPartId;
    /* 0x02 */ Vec3s rotation;
    /* 0x08 */ Vec3i offset;
} CharacterModelPart;

#endif
