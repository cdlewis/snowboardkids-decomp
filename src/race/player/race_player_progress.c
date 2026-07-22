#include "common.h"
#include "game/math/fixed_point_math.h"
#include "game/race/motion/race_motion.h"
#include "game/race/player/race_player_progress.h"

#define RACE_PLAYER_PROGRESS_COUNT 4
#define RACE_PLAYER_PROGRESS_STATE_SIZE 0x60C

typedef struct RacePlayerCheckpointEvent {
    /* 0x00 */ s16 pathFrame;
    /* 0x02 */ s16 eventId;
} RacePlayerCheckpointEvent;

typedef struct RacePlayerProgressState {
    /* 0x000 */ s16 playerIndex;
    /* 0x002 */ u8 pad2[2];
    /* 0x004 */ u8 isActive;
    /* 0x005 */ u8 pad5[0x10 - 0x5];
    /* 0x010 */ u8 state;
    /* 0x011 */ u8 pad11[0x1C - 0x11];
    /* 0x01C */ s32 posX;
    /* 0x020 */ s32 posY;
    /* 0x024 */ s32 posZ;
    /* 0x028 */ u8 pad28[0x2FC - 0x28];
    /* 0x2FC */ s32 flags;
    /* 0x300 */ u8 pad300[0x502 - 0x300];
    /* 0x502 */ s16 pathFrame;
    /* 0x504 */ u8 pad504[0x509 - 0x504];
    /* 0x509 */ s8 raceRank;
    /* 0x50A */ u8 pad50A[0x51C - 0x50A];
    /* 0x51C */ u32 checkpointEventMask;
    /* 0x520 */ s32 smoothedPathOffset;
    /* 0x524 */ u8 checkpointHit;
    /* 0x525 */ u8 pad525[2];
    /* 0x527 */ s8 checkpointEventId;
    /* 0x528 */ u8 pad528;
    /* 0x529 */ u8 displayRank;
    /* 0x52A */ u8 rankArrow;
    /* 0x52B */ u8 rankChangeTimer;
    /* 0x52C */ u8 pad52C[RACE_PLAYER_PROGRESS_STATE_SIZE - 0x52C];
} RacePlayerProgressState;

extern RacePlayerProgressState gRacePlayers[RACE_PLAYER_PROGRESS_COUNT];
extern RacePlayerProgressState gFrameCounter;
extern RacePlayerCheckpointEvent *gRaceCourseCheckpointEventLists[];
extern s8 *gRaceCoursePlayerPathOffsetTables[];
extern u8 gSinglePlayerRankDisplayPatternFirst[];
extern u8 gSinglePlayerRankDisplayPatternSecond[];
extern u8 gSinglePlayerRankDisplayPatternThird[];
extern u8 gSinglePlayerRankDisplayPatternFourth[];
extern u8 gRaceSplitscreenMode;
extern u8 gPlayerCount;
extern s16 gRaceCourseIndex;

#define gRacePlayerProgressStates gRacePlayers

// updateRacePlayerRankDisplay best match: 55.599% (nonmatchings/updateRacePlayerRankDisplay-3357475854818838508/base_11.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/player/race_player_progress/updateRacePlayerRankDisplay.s")

#ifdef NON_MATCHING
#define RANK_NEAR_LIMIT 0x3800000
#define RANK_NEAR_NEG_LIMIT ((s32)0xFC800001)

#define ASSIGN_DISPLAY_RANKS(pattern) \
    rankIndex = 0; \
    player = &gRacePlayerProgressStates[order[0]]; \
    if (player->isActive != 0) { \
        player->rankChangeTimer = 0; \
        player->displayRank = (pattern)[rankIndex]; \
        rankIndex++; \
    } \
    player = &gRacePlayerProgressStates[order[1]]; \
    if (player->isActive != 0) { \
        player->rankChangeTimer = 0; \
        player->displayRank = (pattern)[rankIndex]; \
        rankIndex++; \
    } \
    player = &gRacePlayerProgressStates[order[2]]; \
    if (player->isActive != 0) { \
        player->rankChangeTimer = 0; \
        player->displayRank = (pattern)[rankIndex]; \
        rankIndex++; \
    } \
    player = &gRacePlayerProgressStates[order[3]]; \
    if (player->isActive != 0) { \
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
    RacePlayerProgressState *player;
    RacePlayerProgressState *other;
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
                if (((volatile RacePlayerProgressState *)gRacePlayerProgressStates)[temp].raceRank < gRacePlayerProgressStates[temp].raceRank) {
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
                if (gRacePlayerProgressStates[candidate].raceRank < gRacePlayerProgressStates[temp].raceRank) {
                    *rankSlot = candidate;
                    *scan = temp;
                    temp = *rankSlot;
                }
                candidate = scan[1];
                if (gRacePlayerProgressStates[candidate].raceRank < gRacePlayerProgressStates[temp].raceRank) {
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
        rank = gRacePlayerProgressStates[0].raceRank;
        if (rank == 0) {
            ASSIGN_DISPLAY_RANKS(gSinglePlayerRankDisplayPatternFirst);
            rank = gRacePlayerProgressStates[0].raceRank;
            rankIndex = 0;
        }
        if (rank == 1) {
            RacePlayerProgressState *player2;
            RacePlayerProgressState *player3;

            rankIndex = 0;
            player2 = &gRacePlayerProgressStates[order[2]];
            player3 = &gRacePlayerProgressStates[order[3]];
            player = &gRacePlayerProgressStates[order[0]];
            other = &gRacePlayerProgressStates[order[1]];
            if (player->isActive != 0) {
                player->rankChangeTimer = 0;
                player->displayRank = gSinglePlayerRankDisplayPatternSecond[rankIndex];
                rankIndex++;
            }
            if (other->isActive != 0) {
                other->rankChangeTimer = 0;
                other->displayRank = gSinglePlayerRankDisplayPatternSecond[rankIndex];
                rankIndex++;
            }
            if (player2->isActive != 0) {
                player2->rankChangeTimer = 0;
                player2->displayRank = gSinglePlayerRankDisplayPatternSecond[rankIndex];
                rankIndex++;
            }
            if (player3->isActive != 0) {
                player3->rankChangeTimer = 0;
                player3->displayRank = gSinglePlayerRankDisplayPatternSecond[rankIndex];
            }
            rank = gRacePlayerProgressStates[0].raceRank;
            rankIndex = 0;
        }
        if (rank == 2) {
            RacePlayerProgressState *player2;
            RacePlayerProgressState *player3;

            rankIndex = 0;
            player2 = &gRacePlayerProgressStates[order[2]];
            player3 = &gRacePlayerProgressStates[order[3]];
            player = &gRacePlayerProgressStates[order[0]];
            other = &gRacePlayerProgressStates[order[1]];
            if (player->isActive != 0) {
                player->rankChangeTimer = 0;
                player->displayRank = gSinglePlayerRankDisplayPatternThird[rankIndex];
                rankIndex++;
            }
            if (other->isActive != 0) {
                other->rankChangeTimer = 0;
                other->displayRank = gSinglePlayerRankDisplayPatternThird[rankIndex];
                rankIndex++;
            }
            if (player2->isActive != 0) {
                player2->rankChangeTimer = 0;
                player2->displayRank = gSinglePlayerRankDisplayPatternThird[rankIndex];
                rankIndex++;
            }
            if (player3->isActive != 0) {
                player3->rankChangeTimer = 0;
                player3->displayRank = gSinglePlayerRankDisplayPatternThird[rankIndex];
            }
            rank = gRacePlayerProgressStates[0].raceRank;
            rankIndex = 0;
        }
        if (rank == 3) {
            RacePlayerProgressState *player2;
            RacePlayerProgressState *player3;

            rankIndex = 0;
            player2 = &gRacePlayerProgressStates[order[2]];
            player3 = &gRacePlayerProgressStates[order[3]];
            player = &gRacePlayerProgressStates[order[0]];
            other = &gRacePlayerProgressStates[order[1]];
            if (player->isActive != 0) {
                player->rankChangeTimer = 0;
                player->displayRank = gSinglePlayerRankDisplayPatternFourth[rankIndex];
                rankIndex++;
            }
            if (other->isActive != 0) {
                other->rankChangeTimer = 0;
                other->displayRank = gSinglePlayerRankDisplayPatternFourth[rankIndex];
                rankIndex++;
            }
            if (player2->isActive != 0) {
                player2->rankChangeTimer = 0;
                player2->displayRank = gSinglePlayerRankDisplayPatternFourth[rankIndex];
                rankIndex++;
            }
            if (player3->isActive != 0) {
                player3->rankChangeTimer = 0;
                player3->displayRank = gSinglePlayerRankDisplayPatternFourth[rankIndex];
            }
        }
        break;
    case 2:
        if (gRacePlayerProgressStates[1].raceRank >= gRacePlayerProgressStates[0].raceRank) {
            if (gRacePlayerProgressStates[1].raceRank == 3) {
                if (gRacePlayerProgressStates[3].raceRank >= gRacePlayerProgressStates[2].raceRank) {
                    gRacePlayerProgressStates[2].displayRank = 2;
                    goto case2_first_display_1;
                }
                gRacePlayerProgressStates[2].displayRank = 1;
                gRacePlayerProgressStates[3].displayRank = 2;
            } else if (gRacePlayerProgressStates[3].raceRank >= gRacePlayerProgressStates[2].raceRank) {
                gRacePlayerProgressStates[2].displayRank = 1;
                gRacePlayerProgressStates[3].displayRank = 2;
            } else {
                gRacePlayerProgressStates[2].displayRank = 2;
case2_first_display_1:
                gRacePlayerProgressStates[3].displayRank = 1;
            }
            gRacePlayerProgressStates[2].rankChangeTimer = 1;
            gRacePlayerProgressStates[3].rankChangeTimer = 1;
        } else {
            if (gRacePlayerProgressStates[0].raceRank == 3) {
                if (gRacePlayerProgressStates[3].raceRank >= gRacePlayerProgressStates[2].raceRank) {
                    gRacePlayerProgressStates[2].displayRank = 2;
                    goto case2_second_display_1;
                }
                gRacePlayerProgressStates[2].displayRank = 1;
                gRacePlayerProgressStates[3].displayRank = 2;
            } else if (gRacePlayerProgressStates[3].raceRank >= gRacePlayerProgressStates[2].raceRank) {
                gRacePlayerProgressStates[2].displayRank = 1;
                gRacePlayerProgressStates[3].displayRank = 2;
            } else {
                gRacePlayerProgressStates[2].displayRank = 2;
case2_second_display_1:
                gRacePlayerProgressStates[3].displayRank = 1;
            }
            gRacePlayerProgressStates[2].rankChangeTimer = 0;
            gRacePlayerProgressStates[3].rankChangeTimer = 0;
        }
        break;
    case 3:
        gRacePlayerProgressStates[3].rankChangeTimer = 0;
        gRacePlayerProgressStates[3].displayRank = 1;
        rank = gRacePlayerProgressStates[(s8)gRacePlayerProgressStates[3].rankChangeTimer].raceRank;
        if (rank < gRacePlayerProgressStates[1].raceRank) {
            gRacePlayerProgressStates[3].rankChangeTimer = 1;
            rank = gRacePlayerProgressStates[(s8)gRacePlayerProgressStates[3].rankChangeTimer].raceRank;
        }
        if (rank < gRacePlayerProgressStates[2].raceRank) {
            gRacePlayerProgressStates[3].rankChangeTimer = 2;
        }
        break;
    }

    player = gRacePlayerProgressStates;
    do {
        if (player->isActive != 0) {
            rank = player->displayRank;
            switch (rank) {
            case 0:
                player->rankArrow = 0;
                break;
            case 1:
                player->rankArrow = 0;
                other = &gRacePlayerProgressStates[(s8)player->rankChangeTimer];
                dx = other->posX - player->posX;
                dz = other->posZ - player->posZ;
                if ((dx >= RANK_NEAR_LIMIT) || (dx < RANK_NEAR_NEG_LIMIT) || (dz >= RANK_NEAR_LIMIT) ||
                        (dz < RANK_NEAR_NEG_LIMIT)) {
                    if (other->raceRank < player->raceRank) {
                        player->rankArrow = 1;
                    } else {
                        player->rankArrow = 2;
                    }
                }
                break;
            case 2:
                if (mode == 1) {
                    if (gRacePlayerProgressStates[0].raceRank < player->raceRank) {
                        player->rankArrow = 0;
                    } else {
                        player->rankArrow = 3;
                    }
                } else {
                    player->rankArrow = 3;
                }
                break;
            }

            if (player->state == 5) {
                player->rankArrow = 0;
            }
        }
        player++;
    } while (player != &gFrameCounter);
}

#undef ASSIGN_DISPLAY_RANKS
#undef RANK_NEAR_NEG_LIMIT
#undef RANK_NEAR_LIMIT
#endif

void updateRacePlayerCheckpointEvents(RacePlayerProgressState *player) {
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

    if (player->flags & 0x1000) {
        player->checkpointEventMask = 0;
    }

    player->checkpointHit = 0;
    eventList = gRaceCourseCheckpointEventLists[gRaceCourseIndex];
    event = eventList;
    eventIndex = 0;

    for (;;) {
        pathFrame = event->pathFrame;
        if (pathFrame == -(u32)1) {
            break;
        }

        eventMask = 1 << eventIndex;

        if (!(player->checkpointEventMask & eventMask) && (player->pathFrame <= pathFrame) &&
                (player->pathFrame > pathFrame - 3)) {
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

    courseIndex = gRaceCourseIndex;
    entry = gRaceCoursePlayerPathOffsetTables[(courseIndex * RACE_PLAYER_PROGRESS_COUNT) + playerIndex];
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

    entry = gRaceCoursePlayerPathOffsetTables[(courseIndex * RACE_PLAYER_PROGRESS_COUNT) + playerIndex];
    pathIndex = entry[pathIndexCopy] << 0x12;
    pathIndex -= gRacePlayerProgressStates[rankSlot].smoothedPathOffset;

    if (pathIndex > 0x60000) {
        pathIndex = 0x60000;
    }
    if (pathIndex < -0x60000) {
        pathIndex = -0x60000;
    }

    gRacePlayerProgressStates[rankSlot].smoothedPathOffset += pathIndex;
    return gRacePlayerProgressStates[rankSlot].smoothedPathOffset;
}

s32 getRacePlayerPathOffset(s32 playerIndex, s32 pathIndex) {
    s8 *entry;

    if (gRaceCourseIndex == 7) {
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

    entry = gRaceCoursePlayerPathOffsetTables[(gRaceCourseIndex * RACE_PLAYER_PROGRESS_COUNT) + playerIndex];
    return entry[pathIndex] << 0x12;
}
