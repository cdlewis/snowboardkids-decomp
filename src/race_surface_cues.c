#include "common.h"

#define RACE_SURFACE_CUE_COUNT 6

typedef struct {
    /* 0x000 */ char pad0[0x4];
    /* 0x004 */ u8 inputOverrideFlags;
    /* 0x005 */ char pad5[0x7F];
    /* 0x084 */ s32 surfaceFlags;
    /* 0x088 */ char pad88[0x8];
    /* 0x090 */ s8 stickX;
    /* 0x091 */ s8 stickY;
    /* 0x092 */ char pad92[0x216];
    /* 0x2A8 */ s16 cueState[RACE_SURFACE_CUE_COUNT];
    /* 0x2B4 */ s16 cueStep[RACE_SURFACE_CUE_COUNT];
    /* 0x2C0 */ char pad2C0[0x267];
    /* 0x527 */ s8 overrideMask;
} RaceSurfaceCueActor;

typedef struct {
    /* 0x0 */ s16 surfaceType;
    /* 0x2 */ s16 cueValue;
} RaceSurfaceCue;

// func_800831C0 best match: 81.509% (base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_surface_cues/func_800831C0.s")

#ifdef NON_MATCHING
s32 func_800831C0(RaceSurfaceCue *cues, RaceSurfaceCueActor *actor, s16 cueIndex) {
    s16 state;
    s16 step;
    u32 surfaceType;

    state = actor->cueState[cueIndex];
    surfaceType = (actor->surfaceFlags & 0xF0000) >> 0x10;
    switch (state) {
    case 0:
        step = actor->cueStep[cueIndex];
        if (surfaceType != cues[step].surfaceType) {
            actor->cueStep[cueIndex] = 0;
            return 0;
        }
        if (cues[step].cueValue != 0) {
            return cues[step].cueValue;
        }
        actor->cueState[cueIndex] = state + 1;
    case 1:
        step = actor->cueStep[cueIndex];
        if (surfaceType == cues[step].surfaceType) {
            return 0;
        }
        if (surfaceType == 0) {
            return 0;
        }
        actor->cueState[cueIndex] = 0;
        actor->cueStep[cueIndex] = step + 1;
    default:
        return 0;
    }
}
#endif

void func_80083298(RaceSurfaceCueActor *actor) {
    actor->cueState[0] = 0;
    actor->cueState[1] = 0;
    actor->cueState[2] = 0;
    actor->cueState[3] = 0;
    actor->cueState[4] = 0;
    actor->cueState[5] = 0;
    actor->cueStep[0] = 0;
    actor->cueStep[1] = 0;
    actor->cueStep[2] = 0;
    actor->cueStep[3] = 0;
    actor->cueStep[4] = 0;
    actor->cueStep[5] = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_surface_cues/func_800832CC.s")
