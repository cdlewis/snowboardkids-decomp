#ifndef CHARACTER_SELECT_FLOW_H
#define CHARACTER_SELECT_FLOW_H

#include "common.h"

typedef struct CharacterSelectFlowState {
    char pad0[0x18];
    /* 0x18 */ s32 fade;
    /* 0x1C */ s32 timer;
} CharacterSelectFlowState;

void func_80005C70(void);
void func_80005F18(void);
void func_800062F8(void);
void func_800066CC(void);
void func_80006AE8(void);
void func_80006D70(void);
void func_80006E3C(void);

#endif
