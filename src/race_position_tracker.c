#include "common.h"
#include "fixed_point_math.h"
#include "race_motion.h"
#include "race_position_tracker.h"

// Race player records are 0x60C bytes apart. This view only names the fields
// touched by this placement/progress tracking module.
#define RACE_POSITION_PLAYER_COUNT 4
#define RACE_POSITION_PLAYER_SIZE 0x60C

typedef struct RacePositionPlayer {
    /* 0x00 */ s16 pathFrame;
    /* 0x02 */ s16 eventId;
} RacePositionCheckpointEvent;

typedef struct {
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
    /* 0x52C */ u8 pad52C[RACE_POSITION_PLAYER_SIZE - 0x52C];
} RacePositionPlayer;

extern RacePositionPlayer D_80121D80[RACE_POSITION_PLAYER_COUNT];
extern RacePositionPlayer gFrameCounter;
extern RacePositionCheckpointEvent *D_800DE030[];
extern s8 *D_800DDE74[];
extern u8 D_800DE058[];
extern u8 D_800DE05C[];
extern u8 D_800DE060[];
extern u8 D_800DE064[];
extern u8 gRaceSplitscreenMode;
extern u8 gPlayerCount;
extern s16 gRaceCourseIndex;

// updateRacePositionTracker best match: 30.134% (nonmatchings/updateRacePositionTracker-5752545231564691495/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_position_tracker/func_8007B250.s")

#ifdef NON_MATCHING
#define RANK_NEAR_LIMIT 0x3800000
#define RANK_NEAR_NEG_LIMIT ((s32)0xFC800001)

#define ASSIGN_DISPLAY_RANKS(pattern) \
    rankIndex = 0; \
    player = &D_80121D80[order[0]]; \
    if (player->isActive != 0) { \
        player->rankChangeTimer = 0; \
        player->displayRank = (pattern)[rankIndex]; \
        rankIndex++; \
    } \
    player = &D_80121D80[order[1]]; \
    if (player->isActive != 0) { \
        player->rankChangeTimer = 0; \
        player->displayRank = (pattern)[rankIndex]; \
        rankIndex++; \
    } \
    player = &D_80121D80[order[2]]; \
    if (player->isActive != 0) { \
        player->rankChangeTimer = 0; \
        player->displayRank = (pattern)[rankIndex]; \
        rankIndex++; \
    } \
    player = &D_80121D80[order[3]]; \
    if (player->isActive != 0) { \
        player->rankChangeTimer = 0; \
        player->displayRank = (pattern)[rankIndex]; \
    }

void updateRacePositionTracker(void) {
    s32 order[4];
    s32 i;
    s32 temp;
    s32 candidate;
    s32 *rankSlot;
    s32 *scan;
    s32 rankIndex;
    s32 mode;
    RacePositionPlayer *player;
    RacePositionPlayer *other;
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
                if (((volatile RacePositionPlayer *)D_80121D80)[temp].raceRank < D_80121D80[temp].raceRank) {
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
                if (D_80121D80[candidate].raceRank < D_80121D80[temp].raceRank) {
                    *rankSlot = candidate;
                    *scan = temp;
                    temp = *rankSlot;
                }
                candidate = scan[1];
                if (D_80121D80[candidate].raceRank < D_80121D80[temp].raceRank) {
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
        rank = D_80121D80[0].raceRank;
        if (rank == 0) {
            ASSIGN_DISPLAY_RANKS(D_800DE058);
        }
        if (rank == 1) {
            ASSIGN_DISPLAY_RANKS(D_800DE05C);
        }
        if (rank == 2) {
            ASSIGN_DISPLAY_RANKS(D_800DE060);
        }
        if (rank == 3) {
            ASSIGN_DISPLAY_RANKS(D_800DE064);
        }
        break;
    case 2:
        if (!(D_80121D80[1].raceRank < D_80121D80[0].raceRank)) {
            if (D_80121D80[1].raceRank == 3) {
                if (D_80121D80[3].raceRank >= D_80121D80[2].raceRank) {
                    D_80121D80[2].displayRank = 2;
                    D_80121D80[3].displayRank = 1;
                } else {
                    D_80121D80[2].displayRank = 1;
                    D_80121D80[3].displayRank = 2;
                }
            } else if (D_80121D80[3].raceRank >= D_80121D80[2].raceRank) {
                D_80121D80[2].displayRank = 1;
                D_80121D80[3].displayRank = 2;
            } else {
                D_80121D80[2].displayRank = 2;
                D_80121D80[3].displayRank = 1;
            }
            D_80121D80[2].rankChangeTimer = 1;
            D_80121D80[3].rankChangeTimer = 1;
        } else {
            if (D_80121D80[0].raceRank == 3) {
                if (D_80121D80[3].raceRank >= D_80121D80[2].raceRank) {
                    D_80121D80[2].displayRank = 2;
                    D_80121D80[3].displayRank = 1;
                } else {
                    D_80121D80[2].displayRank = 1;
                    D_80121D80[3].displayRank = 2;
                }
            } else if (D_80121D80[3].raceRank >= D_80121D80[2].raceRank) {
                D_80121D80[2].displayRank = 1;
                D_80121D80[3].displayRank = 2;
            } else {
                D_80121D80[2].displayRank = 2;
                D_80121D80[3].displayRank = 1;
            }
            D_80121D80[2].rankChangeTimer = 0;
            D_80121D80[3].rankChangeTimer = 0;
        }
        break;
    case 3:
        D_80121D80[3].rankChangeTimer = 0;
        D_80121D80[3].displayRank = 1;
        rank = D_80121D80[(s8)D_80121D80[3].rankChangeTimer].raceRank;
        if (rank < D_80121D80[1].raceRank) {
            D_80121D80[3].rankChangeTimer = 1;
            rank = D_80121D80[(s8)D_80121D80[3].rankChangeTimer].raceRank;
        }
        if (rank < D_80121D80[2].raceRank) {
            D_80121D80[3].rankChangeTimer = 2;
        }
        break;
    }

    player = D_80121D80;
    do {
        if (player->isActive != 0) {
            rank = player->displayRank;
            switch (rank) {
            case 0:
                player->rankArrow = 0;
                break;
            case 1:
                player->rankArrow = 0;
                other = &D_80121D80[(s8)player->rankChangeTimer];
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
                    if (D_80121D80[0].raceRank < player->raceRank) {
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

// func_8007BB08 best match: 99.480% (nonmatchings/func_8007BB08-7273315160691878794/base_9.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_position_tracker/func_8007BB08.s")

#ifdef NON_MATCHING
void func_8007BB08(RacePositionPlayer *player) {
    s64 product;
    s32 x;
    s32 y;
    s32 z;
    s16 angle;
    RacePositionCheckpointEvent *event;
    s32 eventIndex;
    s16 pathFrame;
    s32 eventMask;
    s32 sine;
    s32 cosine;

    if (player->flags & 0x1000) {
        player->checkpointEventMask = 0;
    }

    player->checkpointHit = 0;
    event = D_800DE030[gRaceCourseIndex];
    eventIndex = 0;

    for (;;) {
        pathFrame = event->pathFrame;
        if (pathFrame == -1) {
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
            product = -(s64)sine * x;
            z = (product + (s64)cosine * z) / 0x1000;

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
#endif

s32 getSmoothedRacePlayerPathOffset(s32 playerIndex, s32 pathIndex, s32 rankSlot) {
    s32 courseIndex;
    s32 pathIndexCopy;
    s8 *entry;

    courseIndex = gRaceCourseIndex;
    entry = D_800DDE74[(courseIndex * RACE_POSITION_PLAYER_COUNT) + playerIndex];
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

    entry = D_800DDE74[(courseIndex * RACE_POSITION_PLAYER_COUNT) + playerIndex];
    pathIndex = entry[pathIndexCopy] << 0x12;
    pathIndex -= D_80121D80[rankSlot].smoothedPathOffset;

    if (pathIndex > 0x60000) {
        pathIndex = 0x60000;
    }
    if (pathIndex < -0x60000) {
        pathIndex = -0x60000;
    }

    D_80121D80[rankSlot].smoothedPathOffset += pathIndex;
    return D_80121D80[rankSlot].smoothedPathOffset;
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

    entry = D_800DDE74[(gRaceCourseIndex * RACE_POSITION_PLAYER_COUNT) + playerIndex];
    return entry[pathIndex] << 0x12;
}
