#ifndef CHARACTER_SELECT_FLOW_H
#define CHARACTER_SELECT_FLOW_H

#include "common.h"

typedef struct CharacterSelectFlowState {
    char pad0[0x18];
    /* 0x18 */ s32 fade;
    /* 0x1C */ s32 timer;
} CharacterSelectFlowState;

#endif
