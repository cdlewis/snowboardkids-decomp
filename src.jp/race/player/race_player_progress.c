#include "common.h"
#include "game/race/race_state.h"
#include "game/race/player/race_player_progress.h"
#include "game/math/fixed_point_math.h"
#include "game/race/motion/race_motion.h"

extern s8 *D_800DD1A4[];
extern RacePlayerCheckpointEvent *D_800DD360[];

#pragma GLOBAL_ASM("asm.jp/nonmatchings/race/player/race_player_progress/updateRacePlayerRankDisplay.s")

void func_8007B46C(RacePlayer *player) {
    RacePlayerCheckpointEvent *eventList;
    RacePlayerCheckpointEvent *event;
    s32 x;
    s32 y;
    s32 z;
    s32 eventIndex;
    s32 eventMask;
    s16 angle;
    s16 pathFrame;
    s32 sine;
    s32 cosine;

    if (player->stateFlags & 0x1000) {
        player->checkpointEventMask = 0;
    }

    player->checkpointHit = 0;
    eventList = D_800DD360[gRaceCourseIndex.signedValue];
    event = eventList;
    eventIndex = 0;

    for (;;) {
        pathFrame = event->pathFrame;
        if (pathFrame == -(u32)1) {
            break;
        }

        eventMask = 1 << eventIndex;

        if (!(player->checkpointEventMask & eventMask) && (player->coursePathIndex <= pathFrame) &&
            (player->coursePathIndex > pathFrame - 3)) {
            getRaceCourseSurfaceSpawnTransform(pathFrame, &x, &y, &z, &angle);

            x = player->pos.x - x;
            y = player->pos.y - y;
            z = player->pos.z - z;

            sine = fixedSine(-angle);
            cosine = fixedCosine(-angle);
            {
                s64 innerProduct;

                innerProduct = -(s64)sine * x;
                z = (innerProduct + (s64)cosine * z) / 0x1000;
            }

            if (z < 0x600000 && z > 0) {
                player->checkpointEventMask |= eventMask;
                player->checkpointHit = 1;
                player->surfaceCueOverrideMask = event->surfaceCueMask;
                break;
            }
        }

        eventIndex++;
        event++;
    }
}

s32 updateRacePlayerSmoothedPathOffset(s32 playerIndex, s32 pathIndex, s32 rankSlot) {
    s32 courseIndex;
    s32 pathIndexCopy;
    s8 *entry;

    courseIndex = gRaceCourseIndex.signedValue;
    entry = D_800DD1A4[(courseIndex * RACE_PLAYER_COUNT) + playerIndex];
    pathIndexCopy = pathIndex;
    if (courseIndex == RACE_COURSE_ANIMAL_LAND) {
        if (playerIndex == 0) {
            return 0xFFF40000;
        }
        if (playerIndex == 1) {
            return 0xC0000;
        }
        if (playerIndex == 2) {
            return 0xFFDC0000;
        }
        if (playerIndex == 3) {
            return 0x240000;
        }
    }

    entry = D_800DD1A4[(courseIndex * RACE_PLAYER_COUNT) + playerIndex];
    pathIndex = entry[pathIndexCopy] << 0x12;
    pathIndex -= gRacePlayers[rankSlot].smoothedPathOffset;

    if (pathIndex > 0x60000) {
        pathIndex = 0x60000;
    }
    if (pathIndex < -0x60000) {
        pathIndex = -0x60000;
    }

    gRacePlayers[rankSlot].smoothedPathOffset += pathIndex;
    return gRacePlayers[rankSlot].smoothedPathOffset;
}

s32 getRacePlayerPathOffset(s32 playerIndex, s32 pathIndex) {
    s8 *entry;

    if (gRaceCourseIndex.signedValue == RACE_COURSE_ANIMAL_LAND) {
        if (playerIndex == 0) {
            return 0xFFF40000;
        }
        if (playerIndex == 1) {
            return 0xC0000;
        }
        if (playerIndex == 2) {
            return 0xFFDC0000;
        }
        if (playerIndex == 3) {
            return 0x240000;
        }
    }

    entry = D_800DD1A4[(gRaceCourseIndex.signedValue * RACE_PLAYER_COUNT) + playerIndex];
    return entry[pathIndex] << 0x12;
}
