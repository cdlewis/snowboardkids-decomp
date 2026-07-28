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

// updateRacePlayerRankDisplay best match: 55.599% (nonmatchings/updateRacePlayerRankDisplay-3357475854818838508/base_11.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/player/race_player_progress/updateRacePlayerRankDisplay.s")

#ifdef NON_MATCHING
#define RANK_NEAR_LIMIT 0x3800000
#define RANK_NEAR_NEG_LIMIT ((s32)0xFC800001)

#define ASSIGN_DISPLAY_RANKS(pattern) \
    rankIndex = 0; \
    player = &gRacePlayers[order[0]]; \
    if (player->progressActive != 0) { \
        player->rankChangeTimer = 0; \
        player->displayRank = (pattern)[rankIndex]; \
        rankIndex++; \
    } \
    player = &gRacePlayers[order[1]]; \
    if (player->progressActive != 0) { \
        player->rankChangeTimer = 0; \
        player->displayRank = (pattern)[rankIndex]; \
        rankIndex++; \
    } \
    player = &gRacePlayers[order[2]]; \
    if (player->progressActive != 0) { \
        player->rankChangeTimer = 0; \
        player->displayRank = (pattern)[rankIndex]; \
        rankIndex++; \
    } \
    player = &gRacePlayers[order[3]]; \
    if (player->progressActive != 0) { \
        player->rankChangeTimer = 0; \
        player->displayRank = (pattern)[rankIndex]; \
    }

void updateRacePlayerRankDisplay(void) {
    s32 order[4];
    s32 i;
    s32 temp;
    s32 candidate;
    s32 *rankSlot;
    s32 *scan;
    s32 rankIndex;
    u8 mode;
    RacePlayer *player;
    RacePlayer *other;
    s32 dx;
    s32 dz;
    s8 rank;

    if (gRaceSplitscreenMode != 0) {
        return;
    }

    order[0] = 0;
    order[1] = 1;
    order[2] = 2;
    order[3] = 3;

    i = 0;
    do {
        if (i < 4) {
            rankSlot = &order[i];
            candidate = i;
            if ((4 - i) & 1) {
                temp = *rankSlot;
                candidate = i + 1;
                if (((volatile RacePlayer *)gRacePlayers)[temp].rankIndex < gRacePlayers[temp].rankIndex) {
                    *((volatile s32 *)rankSlot) = temp;
                    *((volatile s32 *)rankSlot) = temp;
                }
                if (candidate == 4) {
                    goto sort_next;
                }
            }
            scan = &order[candidate];
            do {
                temp = *rankSlot;
                candidate = *scan;
                if (gRacePlayers[candidate].rankIndex < gRacePlayers[temp].rankIndex) {
                    *rankSlot = candidate;
                    *scan = temp;
                    temp = *rankSlot;
                }
                candidate = scan[1];
                if (gRacePlayers[candidate].rankIndex < gRacePlayers[temp].rankIndex) {
                    *rankSlot = candidate;
                    scan[1] = temp;
                }
                scan += 2;
            } while (scan != &order[4]);
        }
sort_next:
        i++;
    } while (i < 3);

    mode = gPlayerCount;

    switch (mode) {
    case 1:
        rank = gRacePlayers[0].rankIndex;
        if (rank == 0) {
            ASSIGN_DISPLAY_RANKS(gSinglePlayerRankDisplayPatternFirst);
            rank = gRacePlayers[0].rankIndex;
            rankIndex = 0;
        }
        if (rank == 1) {
            RacePlayer *player2;
            RacePlayer *player3;

            rankIndex = 0;
            player2 = &gRacePlayers[order[2]];
            player3 = &gRacePlayers[order[3]];
            player = &gRacePlayers[order[0]];
            other = &gRacePlayers[order[1]];
            if (player->progressActive != 0) {
                player->rankChangeTimer = 0;
                player->displayRank = gSinglePlayerRankDisplayPatternSecond[rankIndex];
                rankIndex++;
            }
            if (other->progressActive != 0) {
                other->rankChangeTimer = 0;
                other->displayRank = gSinglePlayerRankDisplayPatternSecond[rankIndex];
                rankIndex++;
            }
            if (player2->progressActive != 0) {
                player2->rankChangeTimer = 0;
                player2->displayRank = gSinglePlayerRankDisplayPatternSecond[rankIndex];
                rankIndex++;
            }
            if (player3->progressActive != 0) {
                player3->rankChangeTimer = 0;
                player3->displayRank = gSinglePlayerRankDisplayPatternSecond[rankIndex];
            }
            rank = gRacePlayers[0].rankIndex;
            rankIndex = 0;
        }
        if (rank == 2) {
            RacePlayer *player2;
            RacePlayer *player3;

            rankIndex = 0;
            player2 = &gRacePlayers[order[2]];
            player3 = &gRacePlayers[order[3]];
            player = &gRacePlayers[order[0]];
            other = &gRacePlayers[order[1]];
            if (player->progressActive != 0) {
                player->rankChangeTimer = 0;
                player->displayRank = gSinglePlayerRankDisplayPatternThird[rankIndex];
                rankIndex++;
            }
            if (other->progressActive != 0) {
                other->rankChangeTimer = 0;
                other->displayRank = gSinglePlayerRankDisplayPatternThird[rankIndex];
                rankIndex++;
            }
            if (player2->progressActive != 0) {
                player2->rankChangeTimer = 0;
                player2->displayRank = gSinglePlayerRankDisplayPatternThird[rankIndex];
                rankIndex++;
            }
            if (player3->progressActive != 0) {
                player3->rankChangeTimer = 0;
                player3->displayRank = gSinglePlayerRankDisplayPatternThird[rankIndex];
            }
            rank = gRacePlayers[0].rankIndex;
            rankIndex = 0;
        }
        if (rank == 3) {
            RacePlayer *player2;
            RacePlayer *player3;

            rankIndex = 0;
            player2 = &gRacePlayers[order[2]];
            player3 = &gRacePlayers[order[3]];
            player = &gRacePlayers[order[0]];
            other = &gRacePlayers[order[1]];
            if (player->progressActive != 0) {
                player->rankChangeTimer = 0;
                player->displayRank = gSinglePlayerRankDisplayPatternFourth[rankIndex];
                rankIndex++;
            }
            if (other->progressActive != 0) {
                other->rankChangeTimer = 0;
                other->displayRank = gSinglePlayerRankDisplayPatternFourth[rankIndex];
                rankIndex++;
            }
            if (player2->progressActive != 0) {
                player2->rankChangeTimer = 0;
                player2->displayRank = gSinglePlayerRankDisplayPatternFourth[rankIndex];
                rankIndex++;
            }
            if (player3->progressActive != 0) {
                player3->rankChangeTimer = 0;
                player3->displayRank = gSinglePlayerRankDisplayPatternFourth[rankIndex];
            }
        }
        break;
    case 2:
        if (gRacePlayers[1].rankIndex >= gRacePlayers[0].rankIndex) {
            if (gRacePlayers[1].rankIndex == 3) {
                if (gRacePlayers[3].rankIndex >= gRacePlayers[2].rankIndex) {
                    gRacePlayers[2].displayRank = 2;
                    goto case2_first_display_1;
                }
                gRacePlayers[2].displayRank = 1;
                gRacePlayers[3].displayRank = 2;
            } else if (gRacePlayers[3].rankIndex >= gRacePlayers[2].rankIndex) {
                gRacePlayers[2].displayRank = 1;
                gRacePlayers[3].displayRank = 2;
            } else {
                gRacePlayers[2].displayRank = 2;
case2_first_display_1:
                gRacePlayers[3].displayRank = 1;
            }
            gRacePlayers[2].rankChangeTimer = 1;
            gRacePlayers[3].rankChangeTimer = 1;
        } else {
            if (gRacePlayers[0].rankIndex == 3) {
                if (gRacePlayers[3].rankIndex >= gRacePlayers[2].rankIndex) {
                    gRacePlayers[2].displayRank = 2;
                    goto case2_second_display_1;
                }
                gRacePlayers[2].displayRank = 1;
                gRacePlayers[3].displayRank = 2;
            } else if (gRacePlayers[3].rankIndex >= gRacePlayers[2].rankIndex) {
                gRacePlayers[2].displayRank = 1;
                gRacePlayers[3].displayRank = 2;
            } else {
                gRacePlayers[2].displayRank = 2;
case2_second_display_1:
                gRacePlayers[3].displayRank = 1;
            }
            gRacePlayers[2].rankChangeTimer = 0;
            gRacePlayers[3].rankChangeTimer = 0;
        }
        break;
    case 3:
        gRacePlayers[3].rankChangeTimer = 0;
        gRacePlayers[3].displayRank = 1;
        rank = gRacePlayers[(s8)gRacePlayers[3].rankChangeTimer].rankIndex;
        if (rank < gRacePlayers[1].rankIndex) {
            gRacePlayers[3].rankChangeTimer = 1;
            rank = gRacePlayers[(s8)gRacePlayers[3].rankChangeTimer].rankIndex;
        }
        if (rank < gRacePlayers[2].rankIndex) {
            gRacePlayers[3].rankChangeTimer = 2;
        }
        break;
    }

    player = gRacePlayers;
    do {
        if (player->progressActive != 0) {
            rank = player->displayRank;
            switch (rank) {
            case 0:
                player->rankArrow = 0;
                break;
            case 1:
                player->rankArrow = 0;
                other = &gRacePlayers[(s8)player->rankChangeTimer];
                dx = other->posX - player->posX;
                dz = other->posZ - player->posZ;
                if ((dx >= RANK_NEAR_LIMIT) || (dx < RANK_NEAR_NEG_LIMIT) || (dz >= RANK_NEAR_LIMIT) ||
                        (dz < RANK_NEAR_NEG_LIMIT)) {
                    if (other->rankIndex < player->rankIndex) {
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

            if (player->progressState == 5) {
                player->rankArrow = 0;
            }
        }
        player++;
    } while (player != gRacePlayersEnd);
}

#undef ASSIGN_DISPLAY_RANKS
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

        if (!(player->checkpointEventMask & eventMask) && (player->surfaceAngle <= pathFrame) &&
                (player->surfaceAngle > pathFrame - 3)) {
            getRaceCourseSurfaceSpawnTransform(pathFrame, &x, &y, &z, &angle);

            x = player->posX - x;
            y = player->posY - y;
            z = player->posZ - z;

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
                player->checkpointEventId = event->eventId;
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
