#include "common.h"
#include "asset_manager.h"
#include "race_surface_cues.h"

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
s32 func_800831C0(RaceSurfaceCue *cues, RaceInputPlayer *player, s16 cueIndex) {
    s16 state;
    s16 step;
    u32 surfaceType;

    state = player->surfaceCueState[cueIndex];
    surfaceType = (player->inputFlags & 0xF0000) >> 0x10;
    switch (state) {
    case 0:
        step = player->surfaceCueStep[cueIndex];
        if (surfaceType != cues[step].surfaceType) {
            player->surfaceCueStep[cueIndex] = 0;
            return 0;
        }
        if (cues[step].cueValue != 0) {
            return cues[step].cueValue;
        }
        player->surfaceCueState[cueIndex] = state + 1;
    case 1:
        step = player->surfaceCueStep[cueIndex];
        if (surfaceType == cues[step].surfaceType) {
            return 0;
        }
        if (surfaceType == 0) {
            return 0;
        }
        player->surfaceCueState[cueIndex] = 0;
        player->surfaceCueStep[cueIndex] = step + 1;
    default:
        return 0;
    }
}
#endif

void func_80083298(RaceInputPlayer *player) {
    player->surfaceCueState[0] = 0;
    player->surfaceCueState[1] = 0;
    player->surfaceCueState[2] = 0;
    player->surfaceCueState[3] = 0;
    player->surfaceCueState[4] = 0;
    player->surfaceCueState[5] = 0;
    player->surfaceCueStep[0] = 0;
    player->surfaceCueStep[1] = 0;
    player->surfaceCueStep[2] = 0;
    player->surfaceCueStep[3] = 0;
    player->surfaceCueStep[4] = 0;
    player->surfaceCueStep[5] = 0;
}

s32 func_800832CC(RaceInputPlayer *player) {
    s32 value;
    s32 cue;
    s8 stickY;
    s8 stickX;

    cue = 0;
    if (player->unk4 != 0) {
        value = randomNextMain() & player->surfaceCueOverrideMask;
        cue = value;
        return cue;
    }

    stickY = player->stickY;
    if (stickY >= 30) {
        cue = 1;
    }
    if (stickY < -29) {
        cue = 2;
    }

    stickX = player->stickX;
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

    switch (player->characterId) {
    case 0:
        value = func_800831C0(D_800DE590, player, 0);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE5A0, player, 1);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE5AC, player, 2);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE5B4, player, 3);
        if (value != 0) {
            cue = value;
        }
        break;
    case 1:
        value = func_800831C0(D_800DE5EC, player, 3);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE5C0, player, 0);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE5D0, player, 1);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE5DC, player, 2);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE5F4, player, 4);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE5FC, player, 5);
        if (value != 0) {
            cue = value;
        }
        break;
    case 2:
        value = func_800831C0(D_800DE628, player, 2);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE60C, player, 0);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE61C, player, 1);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE630, player, 3);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE640, player, 4);
        if (value != 0) {
            cue = value;
        }
        break;
    case 3:
        value = func_800831C0(D_800DE684, player, 0);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE698, player, 1);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE6A0, player, 2);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE6B0, player, 3);
        if (value != 0) {
            cue = value;
        }
        break;
    case 4:
        value = func_800831C0(D_800DE6C8, player, 0);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE6EC, player, 1);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE6F8, player, 2);
        if (value != 0) {
            cue = value;
        }
        break;
    case 5:
        value = func_800831C0(D_800DE708, player, 0);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE718, player, 1);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE724, player, 2);
        if (value != 0) {
            cue = value;
        }
        value = func_800831C0(D_800DE768, player, 3);
        if (value != 0) {
            cue = value;
        }
        break;
    }

    return cue;
}
