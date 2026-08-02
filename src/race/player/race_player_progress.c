#include "game/race/race_state.h"
#include "common.h"
#include "game/math/fixed_point_math.h"
#include "game/race/motion/race_motion.h"
#include "game/race/player/race_player_progress.h"
#include "game/race/player/race_player_input.h"

#define RANK_ARROW_DISTANCE 0x03800000
#define RANK_ARROW_NEGATIVE_LIMIT ((s32) 0xFC800001)

typedef struct RacePlayerCheckpointEvent {
    /* 0x00 */ s16 pathFrame;
    /* 0x02 */ s16 eventId;
} RacePlayerCheckpointEvent;

extern RacePlayerCheckpointEvent *gRaceCourseCheckpointEventLists[];
extern s8 *gRaceCoursePlayerPathOffsetTables[];
extern u8 gSinglePlayerRankDisplayPatternFirst[];
extern u8 gSinglePlayerRankDisplayPatternSecond[];
extern u8 gSinglePlayerRankDisplayPatternThird[];
extern u8 gSinglePlayerRankDisplayPatternFourth[];
extern s16 gFrameCounter;

// updateRacePlayerRankDisplay best match: 81.165% (nonmatchings/updateRacePlayerRankDisplay-5793478266135801235/base_30.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/player/race_player_progress/updateRacePlayerRankDisplay.s")

#ifdef NON_MATCHING
void updateRacePlayerRankDisplay(void) {
    RacePlayer *player;
    s32 deltaX;
    s32 deltaZ;
    s32 i;
    s32 j;
    s32 order[RACE_PLAYER_COUNT];
    s32 temp;
    u8 mode;
    s8 rank;

    if (gRaceSplitscreenMode != 0) {
        return;
    }

    order[0] = 0;
    order[1] = 1;
    order[2] = 2;
    order[3] = 3;

    for (i = 0; i < 3; i++) {
        for (j = i; j < RACE_PLAYER_COUNT; j++) {
            if (gRacePlayers[order[j]].rankIndex < gRacePlayers[order[i]].rankIndex) {
                temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }
        }
    }

    mode = gPlayerCount;
    switch (mode) {
    case 1:
        rank = gRacePlayers[0].rankIndex;
        i = 0;
        if (rank == 0) {
            if (gRacePlayers[order[0]].isCpu != 0) {
                gRacePlayers[order[0]].displayRank = gSinglePlayerRankDisplayPatternFirst[i];
                gRacePlayers[order[0]].rankTargetPlayer = 0;
                i++;
            }
            if (gRacePlayers[order[1]].isCpu != 0) {
                gRacePlayers[order[1]].displayRank = gSinglePlayerRankDisplayPatternFirst[i];
                gRacePlayers[order[1]].rankTargetPlayer = 0;
                i++;
            }
            if (gRacePlayers[order[2]].isCpu != 0) {
                gRacePlayers[order[2]].displayRank = gSinglePlayerRankDisplayPatternFirst[i];
                gRacePlayers[order[2]].rankTargetPlayer = 0;
                i++;
            }
            if (gRacePlayers[order[3]].isCpu != 0) {
                gRacePlayers[order[3]].displayRank = gSinglePlayerRankDisplayPatternFirst[i];
                gRacePlayers[order[3]].rankTargetPlayer = 0;
                i++;
            }
        }

        rank = gRacePlayers[0].rankIndex;
        i = 0;
        if (rank == 1) {
            if (gRacePlayers[order[0]].isCpu != 0) {
                gRacePlayers[order[0]].displayRank = gSinglePlayerRankDisplayPatternSecond[i];
                gRacePlayers[order[0]].rankTargetPlayer = 0;
                i++;
            }
            if (gRacePlayers[order[1]].isCpu != 0) {
                gRacePlayers[order[1]].displayRank = gSinglePlayerRankDisplayPatternSecond[i];
                gRacePlayers[order[1]].rankTargetPlayer = 0;
                i++;
            }
            if (gRacePlayers[order[2]].isCpu != 0) {
                gRacePlayers[order[2]].displayRank = gSinglePlayerRankDisplayPatternSecond[i];
                gRacePlayers[order[2]].rankTargetPlayer = 0;
                i++;
            }
            if (gRacePlayers[order[3]].isCpu != 0) {
                gRacePlayers[order[3]].displayRank = gSinglePlayerRankDisplayPatternSecond[i];
                gRacePlayers[order[3]].rankTargetPlayer = 0;
                i++;
            }
        }

        rank = gRacePlayers[0].rankIndex;
        i = 0;
        if (rank == 2) {
            if (gRacePlayers[order[0]].isCpu != 0) {
                gRacePlayers[order[0]].displayRank = gSinglePlayerRankDisplayPatternThird[i];
                gRacePlayers[order[0]].rankTargetPlayer = 0;
                i++;
            }
            if (gRacePlayers[order[1]].isCpu != 0) {
                gRacePlayers[order[1]].displayRank = gSinglePlayerRankDisplayPatternThird[i];
                gRacePlayers[order[1]].rankTargetPlayer = 0;
                i++;
            }
            if (gRacePlayers[order[2]].isCpu != 0) {
                gRacePlayers[order[2]].displayRank = gSinglePlayerRankDisplayPatternThird[i];
                gRacePlayers[order[2]].rankTargetPlayer = 0;
                i++;
            }
            if (gRacePlayers[order[3]].isCpu != 0) {
                gRacePlayers[order[3]].displayRank = gSinglePlayerRankDisplayPatternThird[i];
                gRacePlayers[order[3]].rankTargetPlayer = 0;
                i++;
            }
        }

        rank = gRacePlayers[0].rankIndex;
        i = 0;
        if (rank == 3) {
            if (gRacePlayers[order[0]].isCpu != 0) {
                gRacePlayers[order[0]].displayRank = gSinglePlayerRankDisplayPatternFourth[i];
                gRacePlayers[order[0]].rankTargetPlayer = 0;
                i++;
            }
            if (gRacePlayers[order[1]].isCpu != 0) {
                gRacePlayers[order[1]].displayRank = gSinglePlayerRankDisplayPatternFourth[i];
                gRacePlayers[order[1]].rankTargetPlayer = 0;
                i++;
            }
            if (gRacePlayers[order[2]].isCpu != 0) {
                gRacePlayers[order[2]].displayRank = gSinglePlayerRankDisplayPatternFourth[i];
                gRacePlayers[order[2]].rankTargetPlayer = 0;
                i++;
            }
            if (gRacePlayers[order[3]].isCpu != 0) {
                gRacePlayers[order[3]].displayRank = gSinglePlayerRankDisplayPatternFourth[i];
                gRacePlayers[order[3]].rankTargetPlayer = 0;
                i++;
            }
        }
        break;

    case 2:
        if (gRacePlayers[1].rankIndex >= gRacePlayers[0].rankIndex) {
            if (gRacePlayers[1].rankIndex == 3) {
                if (gRacePlayers[3].rankIndex >= gRacePlayers[2].rankIndex) {
                    gRacePlayers[2].displayRank = 2;
                    gRacePlayers[3].displayRank = 1;
                } else {
                    gRacePlayers[2].displayRank = 1;
                    gRacePlayers[3].displayRank = 2;
                }
            } else if (gRacePlayers[3].rankIndex >= gRacePlayers[2].rankIndex) {
                gRacePlayers[2].displayRank = 1;
                gRacePlayers[3].displayRank = 2;
            } else {
                gRacePlayers[2].displayRank = 2;
                gRacePlayers[3].displayRank = 1;
            }
            gRacePlayers[2].rankTargetPlayer = 1;
            gRacePlayers[3].rankTargetPlayer = 1;
        } else {
            if (gRacePlayers[0].rankIndex == 3) {
                if (gRacePlayers[3].rankIndex >= gRacePlayers[2].rankIndex) {
                    gRacePlayers[2].displayRank = 2;
                    gRacePlayers[3].displayRank = 1;
                } else {
                    gRacePlayers[2].displayRank = 1;
                    gRacePlayers[3].displayRank = 2;
                }
            } else if (gRacePlayers[3].rankIndex >= gRacePlayers[2].rankIndex) {
                gRacePlayers[2].displayRank = 1;
                gRacePlayers[3].displayRank = 2;
            } else {
                gRacePlayers[2].displayRank = 2;
                gRacePlayers[3].displayRank = 1;
            }
            gRacePlayers[2].rankTargetPlayer = 0;
            gRacePlayers[3].rankTargetPlayer = 0;
        }
        break;

    case 3:
        gRacePlayers[3].rankTargetPlayer = 0;
        gRacePlayers[3].displayRank = 1;
        rank = gRacePlayers[gRacePlayers[3].rankTargetPlayer].rankIndex;
        if (rank < gRacePlayers[1].rankIndex) {
            gRacePlayers[3].rankTargetPlayer = 1;
            rank = gRacePlayers[gRacePlayers[3].rankTargetPlayer].rankIndex;
        }
        if (rank < gRacePlayers[2].rankIndex) {
            gRacePlayers[3].rankTargetPlayer = 2;
        }
        break;
    }

    player = gRacePlayers;
    do {
        if (player->isCpu != 0) {
            switch (player->displayRank) {
            case 0:
                player->rankArrow = 0;
                break;

            case 1:
                player->rankArrow = 0;
                deltaX = gRacePlayers[player->rankTargetPlayer].pos.x - player->pos.x;
                deltaZ = gRacePlayers[player->rankTargetPlayer].pos.z - player->pos.z;
                if ((deltaX >= RANK_ARROW_DISTANCE) ||
                    (deltaX < RANK_ARROW_NEGATIVE_LIMIT) ||
                    (deltaZ >= RANK_ARROW_DISTANCE) ||
                    (deltaZ < RANK_ARROW_NEGATIVE_LIMIT)) {
                    if (gRacePlayers[player->rankTargetPlayer].rankIndex < player->rankIndex) {
                        player->rankArrow = 1;
                    } else {
                        player->rankArrow = 2;
                    }
                }
                break;

            case 2:
                if (mode == 1) {
                    if (gRacePlayers[0].rankIndex < player->rankIndex) {
                        player->rankArrow = 0;
                    } else {
                        player->rankArrow = 3;
                    }
                } else {
                    player->rankArrow = 3;
                }
                break;
            }
            if (player->characterId == 5) {
                player->rankArrow = 0;
            }
        }
        player++;
    } while (player != (RacePlayer *) &gFrameCounter);
}

#endif

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

    if (player->stateFlags & 0x1000) {
        player->checkpointEventMask = 0;
    }

    player->checkpointHit = 0;
    eventList = gRaceCourseCheckpointEventLists[gRaceCourseIndex.signedValue];
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
                player->surfaceCueOverrideMask = event->eventId;
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
    if (courseIndex == 7) {
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

    if (gRaceCourseIndex.signedValue == 7) {
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
