#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/race/course/race_surface_cues.h"

extern RaceSurfaceCue gRaceSurfaceCueSlashSequence0[];
extern RaceSurfaceCue gRaceSurfaceCueSlashSequence1[];
extern RaceSurfaceCue gRaceSurfaceCueSlashSequence2[];
extern RaceSurfaceCue gRaceSurfaceCueSlashSequence3[];
extern RaceSurfaceCue gRaceSurfaceCueNancySequence0[];
extern RaceSurfaceCue gRaceSurfaceCueNancySequence1[];
extern RaceSurfaceCue gRaceSurfaceCueNancySequence2[];
extern RaceSurfaceCue gRaceSurfaceCueNancySequence3[];
extern RaceSurfaceCue gRaceSurfaceCueNancySequence4[];
extern RaceSurfaceCue gRaceSurfaceCueNancySequence5[];
extern RaceSurfaceCue gRaceSurfaceCueJamSequence0[];
extern RaceSurfaceCue gRaceSurfaceCueJamSequence1[];
extern RaceSurfaceCue gRaceSurfaceCueJamSequence2[];
extern RaceSurfaceCue gRaceSurfaceCueJamSequence3[];
extern RaceSurfaceCue gRaceSurfaceCueJamSequence4[];
extern RaceSurfaceCue gRaceSurfaceCueLindaSequence0[];
extern RaceSurfaceCue gRaceSurfaceCueLindaSequence1[];
extern RaceSurfaceCue gRaceSurfaceCueLindaSequence2[];
extern RaceSurfaceCue gRaceSurfaceCueLindaSequence3[];
extern RaceSurfaceCue gRaceSurfaceCueTommySequence0[];
extern RaceSurfaceCue gRaceSurfaceCueTommySequence1[];
extern RaceSurfaceCue gRaceSurfaceCueTommySequence2[];
extern RaceSurfaceCue gRaceSurfaceCueShinobinSequence0[];
extern RaceSurfaceCue gRaceSurfaceCueShinobinSequence1[];
extern RaceSurfaceCue gRaceSurfaceCueShinobinSequence2[];
extern RaceSurfaceCue gRaceSurfaceCueShinobinSequence3[];

s32 advanceRaceSurfaceCueSequence(RaceSurfaceCue *cues, RacePlayer *player, s16 cueIndex) {
    RaceSurfaceCue *cue;
    s16 step;
    s16 cueValue;
    u32 surfaceType;

    surfaceType = (player->inputFlags & 0xF0000) >> 0x10;
    switch (player->surfaceCueState[cueIndex]) {
    case 0:
        cue = cues;
        cue += player->surfaceCueStep[cueIndex];
        if (surfaceType != cue->surfaceType) {
            player->surfaceCueStep[cueIndex] = 0;
            return 0;
        }
        cueValue = cue->cueValue;
        if (cueValue != 0) {
            return cueValue;
        }
        player->surfaceCueState[cueIndex] = player->surfaceCueState[cueIndex] + 1;
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

void resetRacePlayerSurfaceCueState(RacePlayer *player) {
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

s32 updateRacePlayerSurfaceCue(RacePlayer *player) {
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
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueSlashSequence0, player, 0);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueSlashSequence1, player, 1);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueSlashSequence2, player, 2);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueSlashSequence3, player, 3);
        if (value != 0) {
            cue = value;
        }
        break;
    case 1:
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueNancySequence3, player, 3);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueNancySequence0, player, 0);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueNancySequence1, player, 1);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueNancySequence2, player, 2);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueNancySequence4, player, 4);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueNancySequence5, player, 5);
        if (value != 0) {
            cue = value;
        }
        break;
    case 2:
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueJamSequence2, player, 2);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueJamSequence0, player, 0);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueJamSequence1, player, 1);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueJamSequence3, player, 3);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueJamSequence4, player, 4);
        if (value != 0) {
            cue = value;
        }
        break;
    case 3:
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueLindaSequence0, player, 0);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueLindaSequence1, player, 1);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueLindaSequence2, player, 2);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueLindaSequence3, player, 3);
        if (value != 0) {
            cue = value;
        }
        break;
    case 4:
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueTommySequence0, player, 0);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueTommySequence1, player, 1);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueTommySequence2, player, 2);
        if (value != 0) {
            cue = value;
        }
        break;
    case 5:
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueShinobinSequence0, player, 0);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueShinobinSequence1, player, 1);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueShinobinSequence2, player, 2);
        if (value != 0) {
            cue = value;
        }
        value = advanceRaceSurfaceCueSequence(gRaceSurfaceCueShinobinSequence3, player, 3);
        if (value != 0) {
            cue = value;
        }
        break;
    }

    return cue;
}
