#include "game/race/race_state.h"
#include "common.h"
#include "game/math/fixed_point_math.h"
#include "game/race/motion/race_motion.h"
#include "game/race/player/race_player_progress.h"
#include "game/race/player/race_player_input.h"

#define RANK_ARROW_DISTANCE 0x03800000
#define RANK_ARROW_NEGATIVE_LIMIT ((s32)0xFC800001)

typedef struct RacePlayerCheckpointEvent {
    /* 0x00 */ s16 pathFrame;
    /* 0x02 */ s16 eventId;
} RacePlayerCheckpointEvent;

extern u32 D_800DC9A8[];
extern s16 gFrameCounter;

char D_800DDE70[4] = "\t\t\t";

s8 *gRaceCoursePlayerPathOffsetTables[40] = {
    (s8 *)&D_800DC9A8[0x002], (s8 *)&D_800DC9A8[0x002], (s8 *)&D_800DC9A8[0x030], (s8 *)&D_800DC9A8[0x05E],
    (s8 *)&D_800DC9A8[0x08C], (s8 *)&D_800DC9A8[0x08C], (s8 *)&D_800DC9A8[0x0B4], (s8 *)&D_800DC9A8[0x0DC],
    (s8 *)&D_800DC9A8[0x104], (s8 *)&D_800DC9A8[0x104], (s8 *)&D_800DC9A8[0x136], (s8 *)&D_800DC9A8[0x168],
    (s8 *)&D_800DC9A8[0x19A], (s8 *)&D_800DC9A8[0x19A], (s8 *)&D_800DC9A8[0x1E4], (s8 *)&D_800DC9A8[0x22E],
    (s8 *)&D_800DC9A8[0x278], (s8 *)&D_800DC9A8[0x278], (s8 *)&D_800DC9A8[0x2AD], (s8 *)&D_800DC9A8[0x2E2],
    (s8 *)&D_800DC9A8[0x317], (s8 *)&D_800DC9A8[0x317], (s8 *)&D_800DC9A8[0x369], (s8 *)&D_800DC9A8[0x3BB],
    (s8 *)&D_800DC9A8[0x40D], (s8 *)&D_800DC9A8[0x40D], (s8 *)&D_800DC9A8[0x44D], (s8 *)&D_800DC9A8[0x48D],
    (s8 *)&D_800DC9A8[0x002], (s8 *)&D_800DC9A8[0x002], (s8 *)&D_800DC9A8[0x030], (s8 *)&D_800DC9A8[0x05E],
    (s8 *)&D_800DC9A8[0x4CD], (s8 *)&D_800DC9A8[0x4CD], (s8 *)&D_800DC9A8[0x4DE], (s8 *)&D_800DC9A8[0x4EF],
    (s8 *)&D_800DC9A8[0x500], (s8 *)&D_800DC9A8[0x500], (s8 *)&D_800DC9A8[0x511], (s8 *)&D_800DC9A8[0x522],
};

RacePlayerCheckpointEvent D_800DDF14[8] = {
    { 0x31, 7 },
    { 0x52, 7 },
    { 0x66, 3 },
    { 0x8B, 1 },
    { 0x8E, 7 },
    { 0x99, 7 },
    { 0xA6, 7 },
    { -1,   0 },
};

RacePlayerCheckpointEvent D_800DDF34[4] = {
    { 0x27, 7 },
    { 0x34, 7 },
    { 0x67, 7 },
    { -1,   0 },
};

RacePlayerCheckpointEvent D_800DDF44[10] = {
    { 0x06, 7 },
    { 0x21, 1 },
    { 0x2C, 1 },
    { 0x42, 1 },
    { 0x4F, 1 },
    { 0x6A, 1 },
    { 0x79, 1 },
    { 0x88, 1 },
    { 0x8C, 1 },
    { -1,   0 },
};

RacePlayerCheckpointEvent D_800DDF6C[14] = {
    { 0x10,  7 },
    { 0x13,  7 },
    { 0x22,  3 },
    { 0x48,  1 },
    { 0x81,  3 },
    { 0x8C,  1 },
    { 0x9D,  1 },
    { 0xB7,  3 },
    { 0x110, 0 },
    { 0x101, 0 },
    { 0xF6,  1 },
    { 0xEA,  1 },
    { 0xE2,  0 },
    { -1,    7 },
};

RacePlayerCheckpointEvent D_800DDFA4[9] = {
    { 0x22, 1 },
    { 0x46, 1 },
    { 0x56, 2 },
    { 0x69, 2 },
    { 0x89, 3 },
    { 0x96, 0 },
    { 0x9C, 3 },
    { 0xBA, 3 },
    { -1,   7 },
};

RacePlayerCheckpointEvent D_800DDFC8[10] = {
    { 0x1E,  3 },
    { 0x2E,  3 },
    { 0x77,  0 },
    { 0x91,  0 },
    { 0xB4,  3 },
    { 0xB7,  3 },
    { 0x105, 3 },
    { 0x126, 3 },
    { 0x12F, 3 },
    { -1,    7 },
};

RacePlayerCheckpointEvent D_800DDFF0[8] = {
    { 0x1A, 3 },
    { 0x29, 0 },
    { 0x4C, 3 },
    { 0x61, 7 },
    { 0x84, 3 },
    { 0xAF, 3 },
    { 0xE0, 3 },
    { -1,   7 },
};

RacePlayerCheckpointEvent D_800DE010[4] = {
    { 0x15, 7 },
    { 0x1F, 7 },
    { 0x32, 0 },
    { -1,   7 },
};

RacePlayerCheckpointEvent D_800DE020[3] = {
    { 0x17, 7 },
    { 0x38, 7 },
    { -1,   7 },
};

RacePlayerCheckpointEvent D_800DE02C[1] = {
    { -1, 7 }
};

RacePlayerCheckpointEvent *gRaceCourseCheckpointEventLists[10] = {
    D_800DDF14, D_800DDF34, D_800DDF44, D_800DDF6C, D_800DDFA4,
    D_800DDFC8, D_800DDFF0, D_800DE02C, D_800DE010, D_800DE020,
};

u8 gSinglePlayerRankDisplayPatternFirst[4] = { 1, 0, 2, 0 };
u8 gSinglePlayerRankDisplayPatternSecond[4] = { 0, 1, 2, 0 };
u8 gSinglePlayerRankDisplayPatternThird[4] = { 0, 1, 2, 0 };
u8 gSinglePlayerRankDisplayPatternFourth[12] = { 0, 2, 1, 0 };

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
            if (gRacePlayers[order[j]].rankIndex < gRacePlayers[order[i]].rankIndex) {
                temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }
        }
    }

    switch (gPlayerCount) {
        case 1:
            i = 0;
            if (gRacePlayers[0].rankIndex == 0) {
                for (j = 0; j < RACE_PLAYER_COUNT; j++) {
                    if (gRacePlayers[order[j]].isCpu != 0) {
                        gRacePlayers[order[j]].displayRank = gSinglePlayerRankDisplayPatternFirst[i];
                        gRacePlayers[order[j]].rankTargetPlayer = 0;
                        i++;
                    }
                }
            }

            i = 0;
            if (gRacePlayers[0].rankIndex == 1) {
                for (j = 0; j < RACE_PLAYER_COUNT; j++) {
                    if (gRacePlayers[order[j]].isCpu != 0) {
                        gRacePlayers[order[j]].displayRank = gSinglePlayerRankDisplayPatternSecond[i];
                        gRacePlayers[order[j]].rankTargetPlayer = 0;
                        i++;
                    }
                }
            }

            i = 0;
            if (gRacePlayers[0].rankIndex == 2) {
                for (j = 0; j < RACE_PLAYER_COUNT; j++) {
                    if (gRacePlayers[order[j]].isCpu != 0) {
                        gRacePlayers[order[j]].displayRank = gSinglePlayerRankDisplayPatternThird[i];
                        gRacePlayers[order[j]].rankTargetPlayer = 0;
                        i++;
                    }
                }
            }

            i = 0;
            if (gRacePlayers[0].rankIndex == 3) {
                for (j = 0; j < RACE_PLAYER_COUNT; j++) {
                    if (gRacePlayers[order[j]].isCpu != 0) {
                        gRacePlayers[order[j]].displayRank = gSinglePlayerRankDisplayPatternFourth[i];
                        gRacePlayers[order[j]].rankTargetPlayer = 0;
                        i++;
                    }
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
            if (gRacePlayers[gRacePlayers[3].rankTargetPlayer].rankIndex < gRacePlayers[1].rankIndex) {
                gRacePlayers[3].rankTargetPlayer = 1;
            }
            if (gRacePlayers[gRacePlayers[3].rankTargetPlayer].rankIndex < gRacePlayers[2].rankIndex) {
                gRacePlayers[3].rankTargetPlayer = 2;
            }
            break;
    }

    for (i = 0; i < RACE_PLAYER_COUNT; i++) {
        if (gRacePlayers[i].isCpu != 0) {
            switch (gRacePlayers[i].displayRank) {
                case 0:
                    gRacePlayers[i].rankArrow = 0;
                    break;

                case 1:
                    gRacePlayers[i].rankArrow = 0;
                    deltaX = gRacePlayers[gRacePlayers[i].rankTargetPlayer].pos.x - gRacePlayers[i].pos.x;
                    deltaZ = gRacePlayers[gRacePlayers[i].rankTargetPlayer].pos.z - gRacePlayers[i].pos.z;
                    if ((deltaX >= RANK_ARROW_DISTANCE) || (deltaX < RANK_ARROW_NEGATIVE_LIMIT) ||
                        (deltaZ >= RANK_ARROW_DISTANCE) || (deltaZ < RANK_ARROW_NEGATIVE_LIMIT)) {
                        if (gRacePlayers[gRacePlayers[i].rankTargetPlayer].rankIndex < gRacePlayers[i].rankIndex) {
                            gRacePlayers[i].rankArrow = 1;
                        } else {
                            gRacePlayers[i].rankArrow = 2;
                        }
                    }
                    break;

                case 2:
                    if (gPlayerCount == 1) {
                        if (gRacePlayers[0].rankIndex < gRacePlayers[i].rankIndex) {
                            gRacePlayers[i].rankArrow = 0;
                        } else {
                            gRacePlayers[i].rankArrow = 3;
                        }
                    } else {
                        gRacePlayers[i].rankArrow = 3;
                    }
                    break;
            }
            if (gRacePlayers[i].characterId == 5) {
                gRacePlayers[i].rankArrow = 0;
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
