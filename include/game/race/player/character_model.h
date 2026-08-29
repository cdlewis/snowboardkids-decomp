#ifndef CHARACTER_MODEL_H
#define CHARACTER_MODEL_H

#include "common.h"

typedef struct CharacterModelPart {
    /* 0x00 */ u8 parentPartIndex;
    /* 0x01 */ u8 mirroredPartIndex;
    /* 0x02 */ Vec3s rotation;
    /* 0x08 */ Vec3i offset;
} CharacterModelPart;

typedef char CharacterModelPartSizeCheck[(sizeof(CharacterModelPart) == 0x14) ? 1 : -1];

#endif
