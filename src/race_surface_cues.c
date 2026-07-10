#include "common.h"
#include "asset_decompression.h"

#define RACE_SURFACE_CUE_COUNT 6

typedef struct {
    /* 0x000 */ char pad0[0x4];
    /* 0x004 */ u8 inputOverrideFlags;
    /* 0x005 */ char pad5[0xB];
    /* 0x010 */ u8 cueMode;
    /* 0x011 */ char pad11[0x73];
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

extern RaceSurfaceCue D_800DE590[];
extern RaceSurfaceCue D_800DE5A0[];
extern RaceSurfaceCue D_800DE5AC[];
extern RaceSurfaceCue D_800DE5B4[];
extern RaceSurfaceCue D_800DE5C0[];
extern RaceSurfaceCue D_800DE5D0[];
extern RaceSurfaceCue D_800DE5DC[];
extern RaceSurfaceCue D_800DE5EC[];
extern RaceSurfaceCue D_800DE5F4[];
extern RaceSurfaceCue D_800DE5FC[];
extern RaceSurfaceCue D_800DE60C[];
extern RaceSurfaceCue D_800DE61C[];
extern RaceSurfaceCue D_800DE628[];
extern RaceSurfaceCue D_800DE630[];
extern RaceSurfaceCue D_800DE640[];
extern RaceSurfaceCue D_800DE684[];
extern RaceSurfaceCue D_800DE698[];
extern RaceSurfaceCue D_800DE6A0[];
extern RaceSurfaceCue D_800DE6B0[];
extern RaceSurfaceCue D_800DE6C8[];
extern RaceSurfaceCue D_800DE6EC[];
extern RaceSurfaceCue D_800DE6F8[];
extern RaceSurfaceCue D_800DE708[];
extern RaceSurfaceCue D_800DE718[];
extern RaceSurfaceCue D_800DE724[];
extern RaceSurfaceCue D_800DE768[];

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

s32 func_800832CC(RaceSurfaceCueActor *actor) {
    s32 value;
    s32 cue;
    s8 stickY;
    s8 stickX;

    cue = 0;
    if (actor->inputOverrideFlags != 0) {
        value = func_800430D0() & actor->overrideMask;
        cue = value;
        return cue;
    }

    stickY = actor->stickY;
    if (stickY >= 30) {
        cue = 1;
    }
    if (stickY < -29) {
        cue = 2;
    }

    stickX = actor->stickX;
    if (stickX >= 30) {
        cue = 3;
    }
    if (stickX < -29) {
        cue = 4;
    }
    if ((stickX >= 26) && (stickY >= 26)) {
        cue = 5;
    }
    if ((stickX < -25) && (stickY >= 26)) {
        cue = 6;
    }
    if ((stickX >= 26) && (stickY < -25)) {
        cue = 7;
    }
    if ((stickX < -25) && (stickY < -25)) {
        cue = 8;
    }

    switch (actor->cueMode) {
    case 0:
        value = func_800831C0(D_800DE590, actor, 0);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE5A0, actor, 1);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE5AC, actor, 2);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE5B4, actor, 3);
        if (value != 0) {
            cue = value;
        }
        break;
    case 1:
        value = func_800831C0(D_800DE5EC, actor, 3);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE5C0, actor, 0);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE5D0, actor, 1);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE5DC, actor, 2);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE5F4, actor, 4);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE5FC, actor, 5);
        if (value != 0) {
            cue = value;
        }
        break;
    case 2:
        value = func_800831C0(D_800DE628, actor, 2);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE60C, actor, 0);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE61C, actor, 1);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE630, actor, 3);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE640, actor, 4);
        if (value != 0) {
            cue = value;
        }
        break;
    case 3:
        value = func_800831C0(D_800DE684, actor, 0);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE698, actor, 1);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE6A0, actor, 2);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE6B0, actor, 3);
        if (value != 0) {
            cue = value;
        }
        break;
    case 4:
        value = func_800831C0(D_800DE6C8, actor, 0);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE6EC, actor, 1);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE6F8, actor, 2);
        if (value != 0) {
            cue = value;
        }
        break;
    case 5:
        value = func_800831C0(D_800DE708, actor, 0);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE718, actor, 1);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE724, actor, 2);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE768, actor, 3);
        if (value != 0) {
            cue = value;
        }
        break;
    }

    return cue;
}
