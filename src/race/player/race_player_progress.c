#include "game/race/race_state.h"
#include "common.h"
#include "game/math/fixed_point_math.h"
#include "game/race/motion/race_motion.h"
#include "game/race/player/race_player_progress.h"
#include "game/race/player/race_player_input.h"

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

// updateRacePlayerRankDisplay best match: 73.923% (nonmatchings/updateRacePlayerRankDisplay-8742002951815950717/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/player/race_player_progress/updateRacePlayerRankDisplay.s")

#ifdef NON_MATCHING
#define RANK_NEAR_LIMIT 0x3800000
#define RANK_NEAR_NEG_LIMIT ((s32)0xFC800001)
#define DISPLAY_RANK_ONE i
#define DISPLAY_RANK_TWO j
#define DISPLAY_RANK_THREE temp

#define ASSIGN_DISPLAY_RANKS(pattern) \
    i = 0; \
    if (gRacePlayers[order[0]].isCpu != 0) { \
        gRacePlayers[order[0]].displayRank = (pattern)[i]; \
        gRacePlayers[order[0]].rankChangeTimer = 0; \
        i++; \
    } \
    if (gRacePlayers[order[1]].isCpu != 0) { \
        gRacePlayers[order[1]].displayRank = (pattern)[i]; \
        gRacePlayers[order[1]].rankChangeTimer = 0; \
        i++; \
    } \
    if (gRacePlayers[order[2]].isCpu != 0) { \
        gRacePlayers[order[2]].displayRank = (pattern)[i]; \
        gRacePlayers[order[2]].rankChangeTimer = 0; \
        i++; \
    } \
    if (gRacePlayers[order[3]].isCpu != 0) { \
        gRacePlayers[order[3]].displayRank = (pattern)[i]; \
        gRacePlayers[order[3]].rankChangeTimer = 0; \
    }

void updateRacePlayerRankDisplay(void) {
    RacePlayer *player;
    s32 dx;
    s32 dz;
    s32 i;
    s32 order[4];
    s32 temp;
    s32 j;
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
        for (j = i; j < 4; j++) {
            if (gRacePlayers[order[j]].rankIndex <
                gRacePlayers[order[i]].rankIndex) {
                temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }
        }
    }

    mode = gPlayerCount;
    DISPLAY_RANK_ONE = 1;
    DISPLAY_RANK_TWO = 2;
    DISPLAY_RANK_THREE = 3;

    switch (mode) {
    case 1:
        rank = gRacePlayers[0].rankIndex;
        if (rank == 0) {
            ASSIGN_DISPLAY_RANKS(gSinglePlayerRankDisplayPatternFirst);
            rank = gRacePlayers[0].rankIndex;
            i = 0;
        }
        if (rank == 1) {
            ASSIGN_DISPLAY_RANKS(gSinglePlayerRankDisplayPatternSecond);
            rank = gRacePlayers[0].rankIndex;
            i = 0;
        }
        if (rank == 2) {
            ASSIGN_DISPLAY_RANKS(gSinglePlayerRankDisplayPatternThird);
            rank = gRacePlayers[0].rankIndex;
            i = 0;
        }
        if (rank == DISPLAY_RANK_THREE) {
            ASSIGN_DISPLAY_RANKS(gSinglePlayerRankDisplayPatternFourth);
        }
        break;
    case 2:
        if (gRacePlayers[1].rankIndex >= gRacePlayers[0].rankIndex) {
            if (gRacePlayers[1].rankIndex == DISPLAY_RANK_THREE) {
                if (gRacePlayers[3].rankIndex >= gRacePlayers[2].rankIndex) {
                    gRacePlayers[2].displayRank = DISPLAY_RANK_TWO;
                    goto case2_first_display_1;
                }
                gRacePlayers[2].displayRank = DISPLAY_RANK_ONE;
                gRacePlayers[3].displayRank = DISPLAY_RANK_TWO;
            } else if (gRacePlayers[3].rankIndex >= gRacePlayers[2].rankIndex) {
                gRacePlayers[2].displayRank = DISPLAY_RANK_ONE;
                gRacePlayers[3].displayRank = DISPLAY_RANK_TWO;
            } else {
                gRacePlayers[2].displayRank = DISPLAY_RANK_TWO;
case2_first_display_1:
                gRacePlayers[3].displayRank = DISPLAY_RANK_ONE;
            }
            gRacePlayers[2].rankChangeTimer = DISPLAY_RANK_ONE;
            gRacePlayers[3].rankChangeTimer = DISPLAY_RANK_ONE;
        } else {
            if (gRacePlayers[0].rankIndex == DISPLAY_RANK_THREE) {
                if (gRacePlayers[3].rankIndex >= gRacePlayers[2].rankIndex) {
                    gRacePlayers[2].displayRank = DISPLAY_RANK_TWO;
                    goto case2_second_display_1;
                }
                gRacePlayers[2].displayRank = DISPLAY_RANK_ONE;
                gRacePlayers[3].displayRank = DISPLAY_RANK_TWO;
            } else if (gRacePlayers[3].rankIndex >= gRacePlayers[2].rankIndex) {
                gRacePlayers[2].displayRank = DISPLAY_RANK_ONE;
                gRacePlayers[3].displayRank = DISPLAY_RANK_TWO;
            } else {
                gRacePlayers[2].displayRank = DISPLAY_RANK_TWO;
case2_second_display_1:
                gRacePlayers[3].displayRank = DISPLAY_RANK_ONE;
            }
            gRacePlayers[2].rankChangeTimer = 0;
            gRacePlayers[3].rankChangeTimer = 0;
        }
        break;
    case 3:
        gRacePlayers[3].rankChangeTimer = 0;
        gRacePlayers[3].displayRank = DISPLAY_RANK_ONE;
        rank = gRacePlayers[(s8)gRacePlayers[3].rankChangeTimer].rankIndex;
        if (rank < gRacePlayers[1].rankIndex) {
            gRacePlayers[3].rankChangeTimer = DISPLAY_RANK_ONE;
            rank = gRacePlayers[(s8)gRacePlayers[3].rankChangeTimer].rankIndex;
        }
        if (rank < gRacePlayers[2].rankIndex) {
            gRacePlayers[3].rankChangeTimer = DISPLAY_RANK_TWO;
        }
        break;
    }

    player = gRacePlayers;
    do {
        if (player->isCpu != 0) {
            rank = player->displayRank;
            switch (rank) {
            case 0:
                player->rankArrow = 0;
                break;
            case 1:
                player->rankArrow = 0;
                dx = gRacePlayers[(s8)player->rankChangeTimer].pos.x - player->pos.x;
                dz = gRacePlayers[(s8)player->rankChangeTimer].pos.z - player->pos.z;
                if ((dx >= RANK_NEAR_LIMIT) || (dx < RANK_NEAR_NEG_LIMIT) ||
                    (dz >= RANK_NEAR_LIMIT) || (dz < RANK_NEAR_NEG_LIMIT)) {
                    if (gRacePlayers[(s8)player->rankChangeTimer].rankIndex <
                        player->rankIndex) {
                        player->rankArrow = 1;
                    } else {
                        player->rankArrow = DISPLAY_RANK_TWO;
                    }
                }
                break;
            case 2:
                if (mode == 1) {
                    if (gRacePlayers[0].rankIndex < player->rankIndex) {
                        player->rankArrow = 0;
                    } else {
                        player->rankArrow = DISPLAY_RANK_THREE;
                    }
                } else {
                    player->rankArrow = DISPLAY_RANK_THREE;
                }
                break;
            }

            if (player->characterId == 5) {
                player->rankArrow = 0;
            }
        }
        player++;
    } while (player != (RacePlayer *)&gFrameCounter);
}

#undef ASSIGN_DISPLAY_RANKS
#undef DISPLAY_RANK_THREE
#undef DISPLAY_RANK_TWO
#undef DISPLAY_RANK_ONE
#undef RANK_NEAR_NEG_LIMIT
#undef RANK_NEAR_LIMIT
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
