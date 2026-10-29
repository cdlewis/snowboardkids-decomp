#include "common.h"
#include "game/race/race_state.h"
#include "game/race/player/race_player_progress.h"
#include "game/math/fixed_point_math.h"
#include "game/race/motion/race_motion.h"

#define RANK_ARROW_DISTANCE 0x04000000
#define RANK_ARROW_NEGATIVE_LIMIT ((s32)0xFC000001)

extern u8 gSinglePlayerRankDisplayPatternFirst[];
extern u8 gSinglePlayerRankDisplayPatternSecond[];
extern u8 gSinglePlayerRankDisplayPatternThird[];
extern u8 gSinglePlayerRankDisplayPatternFourth[];
extern s8 *gRaceCoursePlayerPathOffsetTables[];
extern RacePlayerCheckpointEvent *gRaceCourseCheckpointEventLists[];

void updateRacePlayerRankDisplay(void) {
    s32 deltaX;
    s32 deltaZ;
    s32 i;
    s32 order[RACE_PLAYER_COUNT];
    s32 j;
    s32 temp;

    if (gRaceSplitscreenMode != 0) {
        return;
    }

    order[0] = 0;
    order[1] = 1;
    order[2] = 2;
    order[3] = 3;

    for (i = 0; i < 3; i++) {
        for (j = i; j < RACE_PLAYER_COUNT; j++) {
            if (gRacePlayers[order[j]].state.rankIndex < gRacePlayers[order[i]].state.rankIndex) {
                temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }
        }
    }

    switch (gPlayerCount) {
        case 1:
            i = 0;
            if (gRacePlayers[0].state.rankIndex == 0) {
                for (j = 0; j < RACE_PLAYER_COUNT; j++) {
                    if (gRacePlayers[j].isCpu) {
                        gRacePlayers[j].state.displayRank = gSinglePlayerRankDisplayPatternFirst[i];
                        gRacePlayers[j].state.rankTargetPlayer = 0;
                        i++;
                    }
                }
            }

            i = 0;
            if (gRacePlayers[0].state.rankIndex == 1) {
                for (j = 0; j < RACE_PLAYER_COUNT; j++) {
                    if (gRacePlayers[j].isCpu) {
                        gRacePlayers[j].state.displayRank = gSinglePlayerRankDisplayPatternSecond[i];
                        gRacePlayers[j].state.rankTargetPlayer = 0;
                        i++;
                    }
                }
            }

            i = 0;
            if (gRacePlayers[0].state.rankIndex == 2) {
                for (j = 0; j < RACE_PLAYER_COUNT; j++) {
                    if (gRacePlayers[j].isCpu) {
                        gRacePlayers[j].state.displayRank = gSinglePlayerRankDisplayPatternThird[i];
                        gRacePlayers[j].state.rankTargetPlayer = 0;
                        i++;
                    }
                }
            }

            i = 0;
            if (gRacePlayers[0].state.rankIndex == 3) {
                for (j = 0; j < RACE_PLAYER_COUNT; j++) {
                    if (gRacePlayers[j].isCpu) {
                        gRacePlayers[j].state.displayRank = gSinglePlayerRankDisplayPatternFourth[i];
                        gRacePlayers[j].state.rankTargetPlayer = 0;
                        i++;
                    }
                }
            }
            break;

        case 2:
            if (gRacePlayers[1].state.rankIndex >= gRacePlayers[0].state.rankIndex) {
                if (gRacePlayers[1].state.rankIndex == 3) {
                    if (gRacePlayers[3].state.rankIndex >= gRacePlayers[2].state.rankIndex) {
                        gRacePlayers[2].state.displayRank = 2;
                        gRacePlayers[3].state.displayRank = 1;
                    } else {
                        gRacePlayers[2].state.displayRank = 1;
                        gRacePlayers[3].state.displayRank = 2;
                    }
                } else if (gRacePlayers[3].state.rankIndex >= gRacePlayers[2].state.rankIndex) {
                    gRacePlayers[2].state.displayRank = 1;
                    gRacePlayers[3].state.displayRank = 2;
                } else {
                    gRacePlayers[2].state.displayRank = 2;
                    gRacePlayers[3].state.displayRank = 1;
                }
                gRacePlayers[2].state.rankTargetPlayer = 1;
                gRacePlayers[3].state.rankTargetPlayer = 1;
            } else {
                if (gRacePlayers[0].state.rankIndex == 3) {
                    if (gRacePlayers[3].state.rankIndex >= gRacePlayers[2].state.rankIndex) {
                        gRacePlayers[2].state.displayRank = 2;
                        gRacePlayers[3].state.displayRank = 1;
                    } else {
                        gRacePlayers[2].state.displayRank = 1;
                        gRacePlayers[3].state.displayRank = 2;
                    }
                } else if (gRacePlayers[3].state.rankIndex >= gRacePlayers[2].state.rankIndex) {
                    gRacePlayers[2].state.displayRank = 1;
                    gRacePlayers[3].state.displayRank = 2;
                } else {
                    gRacePlayers[2].state.displayRank = 2;
                    gRacePlayers[3].state.displayRank = 1;
                }
                gRacePlayers[2].state.rankTargetPlayer = 0;
                gRacePlayers[3].state.rankTargetPlayer = 0;
            }
            break;

        case 3:
            gRacePlayers[3].state.rankTargetPlayer = 0;
            gRacePlayers[3].state.displayRank = 1;
            if (gRacePlayers[gRacePlayers[3].state.rankTargetPlayer].state.rankIndex < gRacePlayers[1].state.rankIndex) {
                gRacePlayers[3].state.rankTargetPlayer = 1;
            }
            if (gRacePlayers[gRacePlayers[3].state.rankTargetPlayer].state.rankIndex < gRacePlayers[2].state.rankIndex) {
                gRacePlayers[3].state.rankTargetPlayer = 2;
            }
            break;
    }

    for (i = 0; i < RACE_PLAYER_COUNT; i++) {
        if (gRacePlayers[i].isCpu) {
            switch (gRacePlayers[i].state.displayRank) {
                case 0:
                    gRacePlayers[i].state.cpuPaceMode = RACE_CPU_PACE_RANK_BONUS;
                    break;

                case 1:
                    gRacePlayers[i].state.cpuPaceMode = RACE_CPU_PACE_RANK_BONUS;
                    deltaX = gRacePlayers[gRacePlayers[i].state.rankTargetPlayer].state.pos.x - gRacePlayers[i].state.pos.x;
                    deltaZ = gRacePlayers[gRacePlayers[i].state.rankTargetPlayer].state.pos.z - gRacePlayers[i].state.pos.z;
                    if ((deltaX >= RANK_ARROW_DISTANCE) || (deltaX < RANK_ARROW_NEGATIVE_LIMIT) ||
                        (deltaZ >= RANK_ARROW_DISTANCE) || (deltaZ < RANK_ARROW_NEGATIVE_LIMIT)) {
                        if (gRacePlayers[gRacePlayers[i].state.rankTargetPlayer].state.rankIndex < gRacePlayers[i].state.rankIndex) {
                            gRacePlayers[i].state.cpuPaceMode = RACE_CPU_PACE_CATCH_UP;
                        } else {
                            gRacePlayers[i].state.cpuPaceMode = RACE_CPU_PACE_SLOW_DOWN;
                        }
                    }
                    break;

                case 2:
                    gRacePlayers[i].state.cpuPaceMode = RACE_CPU_PACE_YIELD;
                    break;
            }
            if (gRacePlayers[i].characterId == 5) {
                gRacePlayers[i].state.cpuPaceMode = RACE_CPU_PACE_RANK_BONUS;
            }
        }
    }
}

void updateRacePlayerCheckpointEvents(RacePlayer *player) {
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

    if (player->state.stateFlags & 0x1000) {
        player->state.checkpointEventMask = 0;
    }

    player->state.checkpointHit = 0;
    eventList = gRaceCourseCheckpointEventLists[gRaceCourseIndex.signedValue];
    event = eventList;
    eventIndex = 0;

    for (;;) {
        pathFrame = event->pathFrame;
        if (pathFrame == -(u32)1) {
            break;
        }

        eventMask = 1 << eventIndex;

        if (!(player->state.checkpointEventMask & eventMask) && (player->state.coursePathIndex <= pathFrame) &&
            (player->state.coursePathIndex > pathFrame - 3)) {
            getRaceCourseSurfaceSpawnTransform(pathFrame, &x, &y, &z, &angle);

            x = player->state.pos.x - x;
            y = player->state.pos.y - y;
            z = player->state.pos.z - z;

            sine = fixedSine(-angle);
            cosine = fixedCosine(-angle);
            {
                s64 innerProduct;

                innerProduct = -(s64)sine * x;
                z = (innerProduct + (s64)cosine * z) / 0x1000;
            }

            if (z < 0x600000 && z > 0) {
                player->state.checkpointEventMask |= eventMask;
                player->state.checkpointHit = 1;
                player->state.surfaceCueOverrideMask = event->surfaceCueMask;
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
    entry = gRaceCoursePlayerPathOffsetTables[(courseIndex * RACE_PLAYER_COUNT) + playerIndex];
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

    entry = gRaceCoursePlayerPathOffsetTables[(courseIndex * RACE_PLAYER_COUNT) + playerIndex];
    pathIndex = entry[pathIndexCopy] << 0x12;
    pathIndex -= gRacePlayers[rankSlot].state.smoothedPathOffset;

    if (pathIndex > 0x60000) {
        pathIndex = 0x60000;
    }
    if (pathIndex < -0x60000) {
        pathIndex = -0x60000;
    }

    gRacePlayers[rankSlot].state.smoothedPathOffset += pathIndex;
    return gRacePlayers[rankSlot].state.smoothedPathOffset;
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

    entry = gRaceCoursePlayerPathOffsetTables[(gRaceCourseIndex.signedValue * RACE_PLAYER_COUNT) + playerIndex];
    return entry[pathIndex] << 0x12;
}
