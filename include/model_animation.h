#ifndef MODEL_ANIMATION_H
#define MODEL_ANIMATION_H

#include "common.h"

typedef struct ModelAnimState ModelAnimState;
typedef struct ModelAnimInitState ModelAnimInitState;

void func_8007D190(void);
s32 func_8007D200(s32 index, s32 x, s32 z);
s32 func_8007D548(s32 x, s32 z);
void func_8007D87C(s32 *arg0, s32 *arg1, s32 arg2);
void func_8007DC38(void);
void func_8007ECF4(s16 arg0, s32 x, s32 z, s32 radius, s32 *pushX, s32 *pushZ, s32 *velocityX,
                  s32 *velocityZ);
void func_8007FD88(s32 arg0);
void func_8007FF88(s16 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4, s32 *arg5);
s32 func_80080CC4(s32 arg0, s32 arg1, s32 arg2);
s32 func_80081124(s32 arg0, s32 arg1, s32 arg2);
u32 func_800813F8(s32 arg0, s32 arg1, s32 arg2);
void func_80081508(s32 arg0, s32 *x, s32 *y, s32 *z, s16 *angle);
void func_800815D4(s32 arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4, s32 arg5, s32 arg6);
void func_80081C44(s32 arg0, s32 *arg1, s32 *arg2, s32 arg3);
s16 func_80081E1C(s32 arg0);
void func_80081E40(ModelAnimState *state, s32 animIndex);
void func_80081EF4(ModelAnimState *state);
void func_80082070(ModelAnimState *state);
void func_80082184(ModelAnimState *state, s32 animIndex, s32 frameTimer, s32 frameTimerReset);
void func_80082664(ModelAnimState *state, s32 animIndex, s32 frameTimer, s32 frameTimerReset);
void func_80082B58(ModelAnimState *state, s32 animIndex, s32 timer, s32 duration);
void func_80082DD0(ModelAnimState *state);
void func_80082E48(ModelAnimState *state);
s32 func_80082EC0(ModelAnimState *state);
s32 func_80082F44(ModelAnimState *state);
void func_80082FC8(ModelAnimInitState *state);

#endif
