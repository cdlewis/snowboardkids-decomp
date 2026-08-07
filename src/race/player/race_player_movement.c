#include "game/race/race_state.h"
#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/audio/sound_manager.h"
#include "game/race/motion/race_motion.h"
#include "game/race/camera/race_camera.h"
#include "game/race/course/race_course_effects.h"
#include "game/race/player/race_player_input.h"
#include "game/race/player/race_player_movement.h"

#define FIXED_PRODUCT(a, b) ((s64)(a) * (b) / 0x1000)
#define COLLISION_POINT(x, y, z, sizeX, sizeY, sizeZ) \
    (((s64)(x) * (sizeX) + (s64)(y) * (sizeY) + (s64)(z) * (sizeZ)) / 0x1000)
#define MIRRORED_COLLISION_POINT(x, y, z, sizeX, sizeY, sizeZ) \
    ((-(s64)(x) * (sizeX) + (s64)(y) * (sizeY) + (s64)(z) * (sizeZ)) / 0x1000)
#define RACE_PLAYER_SURFACE_DOT2(a, b, c, d) \
    ((product = (s64)(a) * (b)), ((product + (s64)(c) * (d)) / 0x1000))
#define RACE_PLAYER_SURFACE_COLLISION_POINT(x, y, z, sizeX, sizeY, sizeZ) \
    ((product = (s64)(y) * (sizeY)), ((product + (s64)(x) * (sizeX) + (s64)(z) * (sizeZ)) / 0x1000))
#define RACE_PLAYER_SURFACE_MIRRORED_COLLISION_POINT(x, y, z, sizeX, sizeY, sizeZ) \
    ((product = (s64)(y) * (sizeY)), ((product + -(s64)(x) * (sizeX) + (s64)(z) * (sizeZ)) / 0x1000))

typedef struct {
    Vec3i worldPos;
    Vec3i localPos;
    Matrix4s rotationMtx;
} TransformScratch;

typedef struct {
    s32 pad0[2];
    s32 cos;
    s32 sin;
    s32 pad1;
    s32 speed;
} MovementSpeedScratch;

typedef s16 GroundAlignmentMatrix3s[9];

typedef struct {
    GroundAlignmentMatrix3s values;
    u8 pad[14];
} MatrixScratch;

typedef struct {
    GroundAlignmentMatrix3s values;
    s16 pad;
    s32 transformedX;
    s32 transformedY;
    s32 transformedZ;
} EffectMatrixScratch;

typedef struct {
    s32 baseY;
    u8 pad4[0x10];
    s32 collisionX;
    u8 pad18[4];
    s32 collisionZ;
} GroundProbeScratch;

Vec3i gRacePlayerGroundProbeOffsets[] = {
    { 0, 0, -0x80000 },
    { 0, 0, 0x80000 },
    { 0x46000, 0, -0xC6000 },
    { -0x46000, 0, -0xC6000 },
    { 0x46000, 0, 0xC6000 },
    { -0x46000, 0, 0xC6000 },
    { 0x46000, 0, 0 },
    { -0x46000, 0, 0 },
    { 0, 0, 0xC6000 },
    { 0xA6000, 0, -0x46000 },
    { -0xA6000, 0, -0x46000 },
    { 0xA6000, 0, 0x46000 },
    { -0xA6000, 0, 0x46000 },
};

s16 gRacePlayerVoiceSoundIds0[] = {
    0x22, 0x22, 0x28, 0x28, 0x35, 0x35, 0x2E, 0x2E, 0x3B, 0x3B, 0x40, 0x40,
};

s16 gRacePlayerVoiceSoundIds1[] = {
    0x23, 0x23, 0x60, 0x29, 0x36, 0x36, 0x63, 0x2F, 0x3C, 0x3C, 0x40, 0x40,
};

s16 gRacePlayerVoiceSoundIds2[] = {
    0x24, 0x5F, 0x62, 0x2A, 0x65, 0x37, 0x64, 0x30, 0x3D, 0x3D, 0x41, 0x41,
};

s16 gRacePlayerVoiceSoundIds4[] = { 0x27, 0x2D, 0x3A, 0x33, 0x3F, 0x43 };
s16 gRacePlayerVoiceSoundIds5[] = { 0x25, 0x2C, 0x38, 0x32, 0x3E, 0x42 };
s16 gRacePlayerVoiceSoundIds6[] = { 0x26, 0x2B, 0x39, 0x31, 0x3E, 0x42 };
s16 gRacePlayerVoiceSoundIds7[] = { 0x26, 0x2B, 0x38, 0x32, 0x3E, 0x42, 0, 0, 0, 0, 0, 0 };

extern s32 calculateFixedAngleBetweenXZPoints(s32, s32, s32, s32);
extern s16 calculateFixedAngleFromDeltaXZ(s32, s32);
extern void makeFixedRotationX(Matrix4s, s16);
extern void makeFixedRotationY(Matrix4s, s16);
extern void makeFixedRotationZ(Matrix4s, s16);
extern void multiplyFixedMatrix3s(Matrix4s, Matrix4s, Matrix4s);
extern void makeFixedRotationXYZ(Matrix4s, s16, s16, s16);
extern void makeFixedRotationXY(Matrix4s, s16, s16);
extern void makeFixedRotationZX(Matrix4s, s16, s16);
extern void makeFixedRotationXZ(Matrix4s, s16, s16);
extern void makeFixedRotationZXY(Matrix4s, s16, s16, s16);
extern void makeFixedRotationZYX(Matrix4s, s16, s16, s16);
extern void transformVec3iByFixedMatrix(Matrix4s, Vec3i *, Vec3i *);
extern s16 fixedSine(s16);
extern s16 fixedCosine(s16);
extern s32 integerSquareRoot64(s64);
extern s16 gFrameCounter;
extern s8 gRacePlayerCount;
extern s32 gMenuFlowState;
extern s16 gRaceLapCount;

void getRacePlayerRankingProgress(s32 arg0, s32 *arg1, s32 *arg2) {
    RacePlayer *player;
    s32 temp;

    player = &gRacePlayers[arg0];
    *arg1 = player->coursePathIndex * 8;
    *arg2 = player->unk504;

    switch (gRaceCourseIndex.signedValue) {
    case 0:
        temp = *arg1;
        if (temp >= 0x580) {
            *arg1 = 0x598 - temp;
            *arg2 = -*arg2;
            return;
        }
        break;
    case 1:
        temp = *arg1;
        if (temp >= 0x4A8) {
            *arg1 = 0x4A8 - temp;
            *arg2 = -*arg2;
            return;
        }
        break;
    case 2:
        temp = *arg1;
        if ((temp >= 0x490) && (temp < 0x4B9)) {
            *arg1 = 0x490 - temp;
            *arg2 = -*arg2;
            return;
        }
        if ((temp >= 0x4C0) && (temp < 0x4D1)) {
            *arg1 = ((temp * 0x38) - 0x10A00) / 0x10 + 0x80;
            return;
        }
        if ((temp >= 0x4D8) && (temp < 0x5C1)) {
            *arg1 = ((temp * 0x120) - 0x57300) / 0xE8 + 0x188;
            return;
        }
        if ((temp >= 0x5C8) && (temp < 0x621)) {
            *arg1 = ((temp * 0x58) - 0x1FCC0) / 0x58 + 0x290;
            return;
        }
        break;
    case 3:
        temp = *arg1;
        if ((temp >= 0x628) && (temp < 0x649)) {
            *arg1 = 0x628 - temp;
            *arg2 = -*arg2;
            return;
        }
        if ((temp >= 0x650) && (temp < 0x7D1)) {
            *arg1 = ((temp * 0x138) - 0x7B180) / 0x180 + 0x60;
            return;
        }
        if ((temp >= 0x7D8) && (temp < 0x8E1)) {
            *arg1 = ((temp * 0xB8) - 0x5A340) / 0x108 + 0x2F0;
            return;
        }
        if ((temp >= 0x8E8) && (temp < 0x921)) {
            *arg1 = ((temp * 0x68) - 0x39E40) / 0x38 + 0x520;
            return;
        }
        break;
    case 4:
        temp = *arg1;
        if ((temp >= 0x618) && (temp < 0x641)) {
            *arg1 = 0x618 - temp;
            *arg2 = -*arg2;
            temp = *arg1;
        }
        if ((temp >= 0x648) && (temp < 0x699)) {
            *arg1 = ((temp * 0x70) - 0x2BF80) / 0x50 + 0x380;
            return;
        }
        break;
    case 5:
        temp = *arg1;
        if ((temp >= 0x6D0) && (temp < 0x6F9)) {
            *arg1 = 0x6F8 - temp;
            *arg2 = -*arg2;
            temp = *arg1;
        }
        if ((temp >= 0x700) && (temp < 0x7E9)) {
            *arg1 = ((temp * 0xD0) - 0x5B000) / 0xE8 + 0x1A0;
            return;
        }
        if ((temp >= 0x7F0) && (temp < 0xA01)) {
            *arg1 = ((temp * 0x280) - 0x13D800) / 0x210 + 0x2C8;
            return;
        }
        if ((temp >= 0xA08) && (temp < 0xA39)) {
            *arg1 = ((temp * 0x48) - 0x2D240) / 0x30 + 0x550;
            return;
        }
        break;
    case 6:
        temp = *arg1;
        if ((temp >= 0x7B8) && (temp < 0x7E1)) {
            *arg1 = 0x7F8 - temp;
            *arg2 = -*arg2;
            return;
        }
        break;
    case 8:
        temp = *arg1;
        if ((temp >= 0x1F0) && (temp < 0x219)) {
            *arg1 = 0x1F0 - temp;
            *arg2 = -*arg2;
        }
        break;
    }
}

// updateRacePlayerRankings best match: 99.722% (nonmatchings/updateRacePlayerRankings-7050948565576131586/base_49.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/player/race_player_movement/updateRacePlayerRankings.s")

#ifdef NON_MATCHING
void updateRacePlayerRankings(void) {
    s8 order[4];
    s32 progress[4];
    s32 pathOffsets[4];
    register s32 i;
    register s32 playerCount;
    register s32 next;
    register s32 lastPlayerIndex;
    register s32 j;
    s32 playerIndex;
    s32 otherPlayerIndex;

    if (gRaceSplitscreenMode != 2) {
        if (gMenuFlowState & 1) {
            gRaceOrderPlayerIds[0] = 0;
            gRaceOrderPlayerIds[1] = 1;
            gRaceOrderPlayerIds[2] = 2;
            gRaceOrderPlayerIds[3] = 3;
        } else {
            if (gRaceOrderPlayerIds) {
            }
            playerCount = gRacePlayerCount;
            order[0] = 0;
            order[1] = 1;
            order[2] = 2;
            order[3] = 3;
            i = 0;
            if (playerCount > 0) {
                for (playerIndex = 1; playerIndex; playerIndex = i < playerCount) {
                    getRacePlayerRankingProgress(i, &progress[i], &pathOffsets[i]);
                    if ((s32)(gRacePlayers[i].stateFlags << 5) < 0) {
                        progress[i] += gRacePlayers[i].unk57C;
                    }
                    i++;
                    if (1) {
                    }
                    if (1) {
                    }
                    if (1) {
                    }
                    playerCount = gRacePlayerCount;
                }
                i = 0;
            }

            lastPlayerIndex = playerCount - 1;
            if (lastPlayerIndex > 0) {
                for (playerIndex = 1; playerIndex; playerIndex = next < lastPlayerIndex) {
                    next = (j = i - -1);
                    if (next < playerCount) {
                        if (((!i) && (!i)) && (!i)) {
                        }
                        for (otherPlayerIndex = 1; otherPlayerIndex;
                             otherPlayerIndex = &order[j] < &order[playerCount]) {
                            otherPlayerIndex = order[j];
                            playerIndex = order[i];
                            if (gRacePlayers[otherPlayerIndex].rankIndex < gRacePlayers[order[i]].rankIndex) {
                                order[i] = otherPlayerIndex;
                                order[j] = playerIndex;
                            }
                            j++;
                        }
                    }
                    playerCount = gRacePlayerCount;
                    i = next;
                }
                i = 0;
            }

            if (lastPlayerIndex > 0) {
                for (playerIndex = 1; playerIndex; playerIndex = next < lastPlayerIndex) {
                    next = (j = i + 1);
                    if (next < playerCount) {
                        for (otherPlayerIndex = 1; otherPlayerIndex; otherPlayerIndex = j < playerCount) {
                            if (1) {
                                playerIndex = order[i];
                                if (!(gRacePlayers[playerIndex].stateFlags & 0x40)) {
                                    otherPlayerIndex = order[j];
                                    if (!(gRacePlayers[otherPlayerIndex].stateFlags & 0x40)) {
                                        if (gRacePlayers[playerIndex].lapDigit <
                                            gRacePlayers[otherPlayerIndex].lapDigit) {
                                            order[i] = otherPlayerIndex;
                                            order[j] = playerIndex;
                                        } else if (gRacePlayers[playerIndex].lapDigit ==
                                                   gRacePlayers[otherPlayerIndex].lapDigit) {
                                            if (progress[playerIndex] < progress[otherPlayerIndex]) {
                                                order[i] = otherPlayerIndex;
                                                order[j] = playerIndex;
                                            } else if (progress[playerIndex] == progress[otherPlayerIndex]) {
                                                if (pathOffsets[playerIndex] < pathOffsets[otherPlayerIndex]) {
                                                    order[i] = otherPlayerIndex & 0xFF;
                                                    playerCount = gRacePlayerCount;
                                                    order[j] = playerIndex;
                                                }
                                            }
                                        }
                                    }
                                }
                                j++;
                            }
                        }
                    }
                    i = next;
                }
                i = 0;
            }

            if (playerCount > 0) {
                u8 *finalRaceOrderPtr;

                finalRaceOrderPtr = gRaceOrderPlayerIds;
                otherPlayerIndex = 0;
                for (playerIndex = 1; playerIndex; playerIndex = i < playerCount) {
                    playerIndex = order[otherPlayerIndex];
                    gRacePlayers[playerIndex].rankIndex = i;
                    *finalRaceOrderPtr = gRacePlayers[playerIndex].playerIndex;
                    i++;
                    otherPlayerIndex++;
                    finalRaceOrderPtr++;
                }
            }
        }
    }
}
#endif

#if 0

#ifdef NON_MATCHING
#if 0
void updateRacePlayerRankings(void) {
    RacePlayerOrder order;
    s32 primary[4];
    s32 secondary[4];
    s32 playerCount;
    s32 i;
    s32 j;
    s32 lastPair;
    u8 *orderI;
    u8 *orderJ;
    RacePlayer *player;
    s8 right;
    s8 left;

    if ((u32)gRaceSplitscreenMode != 2) {
        if (gMenuFlowState & 1) {
            gRaceOrderPlayerIds[0] = 0;
            gRaceOrderPlayerIds[1] = 1;
            gRaceOrderPlayerIds[2] = 2;
            gRaceOrderPlayerIds[3] = 3;
            return;
        }

        playerCount = gRacePlayerCount;
        order[0] = 0;
        order[1] = 1;
        order[2] = 2;
        left = 1;
        right = 3;
        order[3] = right;
        i = 0;
        if (playerCount > 0) {
            player = gRacePlayers;
            do {
                getRacePlayerRankingProgress(i, &primary[i], &secondary[i]);
                if ((s32)(player->stateFlags << 5) < 0) {
                    primary[i] += player->unk57C;
                }
                i++;
                player++;
            } while (i < ((playerCount = gRacePlayerCount) ^ 0));
            i = 0;
        }

        lastPair = playerCount - left;
        if (lastPair > 0) {
            do {
                j = 1;
                j = i + j;
                if (j < playerCount) {
                    orderJ = &order[j];
                    orderI = &order[i];
                    playerCount++;
                    playerCount--;
                    do {
                        if (gRacePlayers[right = orderJ[0]].rankIndex <
                                gRacePlayers[orderI[0]].rankIndex) {
                            left = orderI[0] & 0xFFFFFFFFFFFFFFFFu;
                            orderI[0] = right;
                            orderJ[0] = left;
                        }
                        orderJ++;
                    } while (orderJ < &order[playerCount]);
                }
                i = j;
            } while (j < lastPair);
            i = 0;
        }

        if (lastPair > 0) {
            do {
                do {
                    j = i + 1;
                    if (j < playerCount) {
                        j = i + 1;
                        orderI = &order[i];
                        do {
                            left = orderI[0];
                            orderJ = &order[j];
                            if (!(gRacePlayers[left].stateFlags & 0x40)) {
                                right = orderJ[0];
                                if (!(gRacePlayers[right].stateFlags & 0x40)) {
                                    if (gRacePlayers[left].lapDigit < gRacePlayers[right].lapDigit) {
                                        orderI[0] = right;
                                        orderJ[0] = left;
                                    } else if (gRacePlayers[left].lapDigit ==
                                            gRacePlayers[right].lapDigit) {
                                        if (primary[left] < primary[right]) {
                                            orderI[0] = right;
                                            orderJ[0] = left;
                                        } else if ((primary[left] == primary[right]) &&
                                                (secondary[left] < secondary[right])) {
                                            orderI[0] = right;
                                            orderJ[0] = left;
                                        }
                                    }
                                }
                                if (gRacePlayers) {
                                }
                            }
                            j++;
                        } while ((j ^ 0) < playerCount);
                    }
                    j = i + 1;
                    i = j;
                } while (j < lastPair);
            } while (0);
            i = 0;
        }

        if (playerCount > 0) {
            orderI = order;
            orderJ = gRaceOrderPlayerIds;
            do {
                right = orderI[playerCount * 0];
                orderI++;
                orderJ++;
                player = &gRacePlayers[right ^ 0];
                player->rankIndex = i;
                i++;
                orderJ[-1] = player->playerIndex;
            } while (i < playerCount);
        }
    }
}
#endif
void updateRacePlayerRankings(void) {
    s8 playerIds[4];
    s32 courseProgress[4];
    s32 pathProgress[4];
    s32 lastPlayer;
    s32 next;
    s32 i;
    s32 j;
    s32 *nextPtr;
    s32 initializedPlayerId;
    RacePlayer *player;
    s8 comparedPlayerId;
    s8 playerId;
    s8 otherPlayerId;

    nextPtr = &next;
    if (gRaceSplitscreenMode != 2) {
        if (gMenuFlowState & 1) {
            gRaceOrderPlayerIds[0] = 0;
            gRaceOrderPlayerIds[1] = 1;
            gRaceOrderPlayerIds[2] = 2;
            gRaceOrderPlayerIds[3] = 3;
        } else {
            playerIds[0] = 0;
            playerIds[1] = 1;
            initializedPlayerId = 2;
            playerIds[initializedPlayerId] = initializedPlayerId;
            playerIds[3] = 3;

            i = 0;
            if (gRacePlayerCount > 0) {
                player = gRacePlayers;
                playerId = 0;
                do {
                    getRacePlayerRankingProgress(i, &courseProgress[i], &pathProgress[i]);
                    if (player->stateFlags & 0x04000000) {
                        courseProgress[i] = courseProgress[i] + player->unk57C;
                    }
                    i++;
                    player++;
                } while (i < gRacePlayerCount);
                i = playerId;
            }

            lastPlayer = (u32)(gRacePlayerCount - 1);
            if (lastPlayer > 0) {
                i++;
                i--;
                do {
                    next = i + 1;
                    j = next;
                    if (next < gRacePlayerCount) {
                        do {
                            otherPlayerId = playerIds[j];
                            playerId = playerIds[i];
                            if (gRacePlayers[otherPlayerId].rankIndex <
                                gRacePlayers[playerId].rankIndex) {
                                playerIds[i] = otherPlayerId;
                                playerIds[j] = playerId;
                            }
                            j++;
                        } while (j < gRacePlayerCount);
                    }
                    i = *nextPtr;
                } while (next < lastPlayer);
                i = 0;
            }

            if (lastPlayer > 0) {
                do {
                    next = i + 1;
                    j = next;
                    if (next < gRacePlayerCount) {
                        do {
                            playerId = playerIds[i];
                            if (!(gRacePlayers[playerId].stateFlags & 0x40)) {
                                otherPlayerId = playerIds[j];
                                if (!(gRacePlayers[otherPlayerId].stateFlags & 0x40)) {
                                    if (gRacePlayers[playerId].lapDigit <
                                        gRacePlayers[otherPlayerId].lapDigit) {
                                        playerIds[i] = otherPlayerId;
                                        playerIds[j] = playerId;
                                    } else if (gRacePlayers[playerId].lapDigit ==
                                               gRacePlayers[otherPlayerId].lapDigit) {
                                        comparedPlayerId = playerId;
                                        if (courseProgress[comparedPlayerId] <
                                            courseProgress[otherPlayerId]) {
                                            playerIds[i] = otherPlayerId;
                                            playerIds[j] = comparedPlayerId;
                                        } else if ((courseProgress[comparedPlayerId] ==
                                                    courseProgress[otherPlayerId]) &&
                                                   (pathProgress[comparedPlayerId] <
                                                    pathProgress[otherPlayerId])) {
                                            playerIds[i] = otherPlayerId & 0xFF;
                                            playerIds[j] = comparedPlayerId;
                                        }
                                    }
                                }
                            }
                            if (gRacePlayers) {
                            }
                            j++;
                        } while (j < gRacePlayerCount);
                    }
                    i = next;
                } while (next < lastPlayer);
            }
            i = 0;

            if (gRacePlayerCount > 0) {
                do {
                    playerId = playerIds[i];
                    player = &gRacePlayers[playerId];
                    player->rankIndex = i;
                    gRaceOrderPlayerIds[i] = player->playerIndex;
                    i++;
                } while (i < gRacePlayerCount);
            }
        }
    }
}
#endif

#endif

void updateRacePlayerFinalLapStatus(RacePlayer *player) {
    CallbackTask *task;
    u32 flags;

    flags = player->stateFlags;
    if (!(flags & 0x40) && (player->lapDigit >= (gRaceLapCount - 1)) &&
            (player->coursePathIndex == gRaceCourseStartEntries[gRaceCourseIndex.signedValue].pathIndex) && !(flags & 0x1000)) {
        player->stateFlags = flags | 0x40;
        if ((gRaceCameraModeChangeDisabled == 0) && (gRacePlayerHudStatuses[player->playerIndex].active != 0)) {
            task = createCallbackTask((CallbackTaskCallback)initFinalLapPrompt, 6, 0x64);
            if (task != NULL) {
                task->userId = player->playerIndex;
            }
        }
    }
}

void resolveRacePlayerBodyCollisions(void) {
    RacePlayer *playerA;
    RacePlayer *playerB;
    s32 i;
    s32 j;
    s32 temp;
    s32 xDiff;
    s32 radius;
    s16 angle;
    s32 sine;
    s32 cosine;
    s32 pushX;

    for (i = 0; i != 3; i++) {
        j = i + 1;
        if (j < 4) {
            playerA = &gRacePlayers[i];
            do {
                if (playerA->isActive != 0) {
                    playerB = &gRacePlayers[j];
                    if ((playerB->isActive != 0) && (playerA->soundDisabled == 0) &&
                        (playerB->soundDisabled == 0) && !(playerA->stateFlags & 0x200000) &&
                        !(playerB->stateFlags & 0x200000)) {
                        temp = playerA->unk5C - playerB->unk5C;
                        if ((temp < 0 ? (temp = -temp, playerA->collisionHeight) : playerB->collisionHeight) >= temp) {
                            radius = playerB->collisionRadius + playerA->collisionRadius;
                            xDiff = playerA->pos.x - playerB->pos.x;
                            if (xDiff < 0) {
                                xDiff = -xDiff;
                            }
                            if (xDiff < radius) {
                                temp = playerA->pos.z - playerB->pos.z;
                                if (temp < 0) {
                                    temp *= -1;
                                }
                                if ((temp < radius) &&
                                    ((temp = integerSquareRoot64((s64)((0, xDiff)) * xDiff +
                                                           (((s64)temp * temp) & 0xFFFFFFFFFFFFFFFF))) < radius)) {
                                    temp = ((radius - temp) * -1) / 2;
                                    angle = calculateFixedAngleBetweenXZPoints(playerA->pos.x, playerA->pos.z,
                                                          playerB->pos.x, playerB->pos.z);
                                    sine = fixedSine(angle);
                                    cosine = fixedCosine(angle);
                                    pushX = (s64)-sine * temp / 0x1000;
                                    temp = (s64)cosine * temp / 0x1000;
                                    if (playerA->stateFlags & 0x1000) {
                                        if (!(playerB->stateFlags & 0x1000)) {
                                            playerB->pos.x -= pushX * 2;
                                            playerB->pos.z += temp * 2;
                                        }
                                    } else if (playerB->stateFlags & 0x1000) {
                                        playerA->pos.x += pushX * 2;
                                        playerA->pos.z -= temp * 2;
                                    } else {
                                        playerA->pos.x += pushX;
                                        playerA->pos.z -= temp;
                                        playerB->pos.x -= pushX;
                                        playerB->pos.z += temp;
                                    }
                                }
                            }
                        }
                    }
                }
                j++;
            } while (j != 4);
        }
    }
}

void pushRacePlayersOutOfCylinderAndApplyItemHit(Vec3i *pos, s32 xzSize, s32 ySize, u16 flag) {
    volatile u8 pad[8];
    register s32 i;
    s32 temp;
    s32 xDiff;
    s32 yLimit;
    s32 xzLimit;
    s32 pushX;
    s32 pushZ;
    s32 localX;
    s32 localZ;
    s16 angle;
    s32 sine;
    s32 cosine;

    i = 0;
    do {
        if (gRacePlayers[i].isActive != 0) {
            yLimit = ySize;
            temp = pos->y - gRacePlayers[i].unk5C;
            if (temp < 0) {
                temp = -temp;
            } else {
                yLimit = gRacePlayers[i].collisionHeight;
            }

            if (temp <= yLimit) {
                xzLimit = gRacePlayers[i].collisionRadius + xzSize;
                xDiff = pos->x - gRacePlayers[i].pos.x;
                if (xDiff < 0) {
                    xDiff = -xDiff;
                }
                if (xDiff < xzLimit) {
                    temp = pos->z - gRacePlayers[i].pos.z;
                    if (temp < 0) {
                        temp = -temp;
                    }
                    if ((temp < xzLimit) &&
                        ((temp = integerSquareRoot64((s64)((0, xDiff)) * xDiff +
                                               (((s64)temp * temp) & 0xFFFFFFFFFFFFFFFF))) < xzLimit)) {
                        angle = calculateFixedAngleBetweenXZPoints(
                            pos->x, pos->z, gRacePlayers[i].pos.x, gRacePlayers[i].pos.z);
                        sine = fixedSine(angle);
                        cosine = fixedCosine(angle);
                        temp = xzLimit - temp;
                        temp *= -1;
                        pushX = (s64)-sine * (s64)((s64)temp & 0xFFFFFFFFFFFFFFFF) / 0x1000;
                        pushZ = (s64)cosine * (s64)((s64)temp & 0xFFFFFFFFFFFFFFFF) / 0x1000;
                        gRacePlayers[i].pos.x -= pushX;
                        gRacePlayers[i].pos.z += pushZ;

                        localX = ((s64)cosine * gRacePlayers[i].unk2C8 -
                                  (s64)sine * gRacePlayers[i].unk2CC) /
                                 0x1000;
                        localZ = ((s64)sine * gRacePlayers[i].unk2C8 +
                                  (s64)cosine * gRacePlayers[i].unk2CC) /
                                 0x1000;
                        if (localZ > 0) {
                            localZ = -localZ;
                        }
                        gRacePlayers[i].unk2C8 =
                            ((s64)cosine * localX + (s64)sine * localZ) / 0x1000;
                        gRacePlayers[i].unk2CC =
                            ((s64)-sine * localX + (s64)cosine * localZ) / 0x1000;
                        gRacePlayers[i].pendingItemHitFlags |= flag;
                    }
                }
            }
        }
        i++;
    } while ((RacePlayer *)&gFrameCounter != &gRacePlayers[i]);
}

void pushRacePlayerOutOfCylinderAndApplyItemHit(Vec3i *pos, s32 xzSize, s32 ySize, u16 flag, s16 playerIndex) {
    volatile int pad;
    s32 temp;
    RacePlayer *player;
    s32 xzLimit;
    s32 yLimit;
    s32 zDiff;
    s32 sine;
    s32 cosine;
    s32 pushX;
    s32 pushZ;
    s16 angle;
    s32 localX;
    s32 localZ;
    s32 xDiff;

    player = &gRacePlayers[playerIndex];
    if (player->isActive != 0) {
        yLimit = ySize;
        temp = pos->y - player->unk5C;
        if (temp < 0) {
            temp = -temp;
        } else {
            yLimit = player->collisionHeight;
        }

        if (temp <= yLimit) {
            xzLimit = player->collisionRadius + xzSize;
            xDiff = pos->x - player->pos.x;
            if (xDiff < 0) {
                xDiff = -xDiff;
            }
            if (xDiff < xzLimit) {
                zDiff = pos->z - player->pos.z;
                if (zDiff < 0) {
                    zDiff = -zDiff;
                }
                if ((zDiff < xzLimit) &&
                    ((temp = integerSquareRoot64((s64)((0, xDiff)) * xDiff +
                                           (((s64)zDiff * zDiff) & 0xFFFFFFFFFFFFFFFF))) < xzLimit)) {
                    angle = calculateFixedAngleBetweenXZPoints(pos->x, pos->z, player->pos.x, player->pos.z);
                    sine = fixedSine(angle);
                    cosine = fixedCosine(angle);
                    temp = xzLimit - temp;
                    temp *= -1;
                    pushX = (s64)-sine * temp / 0x1000;
                    pushZ = (s64)cosine * temp / 0x1000;
                    player->pos.x -= pushX;
                    player->pos.z += pushZ;

                    localX = ((s64)cosine * player->unk2C8 - (s64)sine * player->unk2CC) / 0x1000;
                    localZ = ((s64)sine * player->unk2C8 + (s64)cosine * player->unk2CC) / 0x1000;
                    if (localZ > 0) {
                        localZ = -localZ;
                    }
                    player->unk2C8 = ((s64)cosine * localX + (s64)sine * localZ) / 0x1000;
                    player->unk2CC = ((s64)-sine * localX + (s64)cosine * localZ) / 0x1000;
                    player->pendingItemHitFlags = player->pendingItemHitFlags | flag;
                }
            }
        }
    }
}

void pushRacePlayersOutOfCylinderOrApplyItemHit(Vec3i *pos, s32 xzSize, s32 ySize, s32 arg3, s16 arg4) {
    volatile u8 pad[16];
    s32 temp;
    s32 xDiff;
    s32 yLimit;
    s32 xzLimit;
    s16 angle;
    s32 sine;
    s32 cosine;
    s32 pushX;
    s32 pushZ;
    register s32 i;

    for (i = 0; i < RACE_PLAYER_COUNT; i++) {
        if (gRacePlayers[i].isActive != 0) {
            yLimit = ySize;
            temp = pos->y - gRacePlayers[i].unk5C;
            if (temp < 0) {
                temp = -temp;
            } else {
                yLimit = gRacePlayers[i].collisionHeight;
            }

            if (temp <= yLimit) {
                xzLimit = gRacePlayers[i].collisionRadius + xzSize;
                xDiff = pos->x - gRacePlayers[i].pos.x;
                if (xDiff < 0) {
                    xDiff = -xDiff;
                }
                if (xDiff < xzLimit) {
                    temp = pos->z - gRacePlayers[i].pos.z;
                    if (temp < 0) {
                        temp = -temp;
                    }
                    if ((temp < xzLimit) &&
                        ((temp = integerSquareRoot64((s64)((0, xDiff)) * xDiff +
                                               (((s64)temp * temp) & 0xFFFFFFFFFFFFFFFF))) < xzLimit)) {
                        if (gRacePlayers[i].unk29C < arg3) {
                            angle = calculateFixedAngleBetweenXZPoints(pos->x, pos->z, gRacePlayers[i].pos.x, gRacePlayers[i].pos.z);
                            sine = fixedSine(angle);
                            cosine = fixedCosine(angle);
                            temp = xzLimit - temp;
                            pushX = (s64)-sine * -temp / 0x1000;
                            pushZ = (s64)cosine * -temp / 0x1000;
                            gRacePlayers[i].pos.x -= pushX;
                            gRacePlayers[i].pos.z += pushZ;
                        } else {
                            gRacePlayers[i].pendingItemHitFlags |= arg4;
                        }
                    }
                }
            }
        }
    }
}

void pushRacePlayerOutOfCylinder(Vec3i *pos, s32 xzSize, s32 ySize, s16 playerIndex) {
    s32 distance;
    RacePlayer *player;
    s32 xzLimit;
    s32 yLimit;
    s32 zDiff;
    s32 sine;
    s32 cosine;
    s32 pushX;
    s32 pushZ;

    player = &gRacePlayers[playerIndex];
    if (player->isActive != 0) {
        s16 angle;
        s32 xDiff;

        yLimit = ySize;
        distance = pos->y - player->unk5C;
        if (distance < 0) {
            distance = -distance;
        } else {
            yLimit = player->collisionHeight;
        }

        if (distance <= yLimit) {
            xzLimit = player->collisionRadius + xzSize;
            xDiff = pos->x - player->pos.x;
            if (xDiff < 0) {
                xDiff = -xDiff;
            }
            if (xDiff < xzLimit) {
                zDiff = pos->z - player->pos.z;
                if (zDiff < 0) {
                    zDiff = -zDiff;
                }
                if ((zDiff < xzLimit) &&
                    ((distance = integerSquareRoot64((s64)((0, xDiff)) * xDiff +
                                                    (((s64)zDiff * zDiff) & 0xFFFFFFFFFFFFFFFF))) <
                     xzLimit)) {
                    angle = calculateFixedAngleBetweenXZPoints(pos->x, pos->z, player->pos.x, player->pos.z);
                    sine = fixedSine(angle);
                    cosine = fixedCosine(angle);
                    distance = xzLimit - distance;
                    distance *= -1;
                    pushX = (s64)-sine * distance / 0x1000;
                    pushZ = (s64)cosine * distance / 0x1000;
                    player->pos.x -= pushX;
                    (&gRacePlayers[playerIndex])->pos.z += pushZ;
                }
            }
        }
    }
}

s32 isRacePlayerInsideCylinder(Vec3i *pos, s32 xzSize, s32 ySize, s16 playerIndex) {
    RacePlayer *player;
    s32 yDiff;
    s32 newLimit;
    s32 yLimit;
    s32 zDiff;
    s32 xDiff;
    s32 xzLimit;
    volatile s32 stackPad[2];
    s16 result[3];

    player = &gRacePlayers[playerIndex];
    result[2] = 0;
    if (player->isActive == 0) {
        return 0;
    }

    yDiff = pos->y - player->unk5C;
    yLimit = ySize;
    if (yDiff < 0) {
        yDiff = -yDiff;
    } else {
        yLimit = player->collisionHeight;
    }

    if (yDiff < yLimit) {
        newLimit = player->collisionRadius + xzSize;
        xDiff = pos->x - player->pos.x;
        xzLimit = newLimit;
        if (xDiff <= -1) {
            xDiff = -xDiff;
        }
        if (xDiff < xzLimit) {
            zDiff = pos->z - player->pos.z;
            if (zDiff < 0) {
                zDiff = -zDiff;
            }
            if ((zDiff < xzLimit) && (integerSquareRoot64((s64)xDiff * xDiff + (s64)zDiff * zDiff) < xzLimit)) {
                result[2] = 1;
            }
        }
    }

    return result[2];
}

void applyItemHitToRacePlayersInsideSphere(Vec3i *pos, s32 xzSize, s16 flag) {
    volatile u8 pad[16];
    RacePlayer *player;
    RacePlayer *end;
    s32 radius;
    s32 dx;
    s32 dy;
    s32 dz;

    end = gRacePlayersEnd; player = gRacePlayers;
    do {
        if ((player->isActive & 0xFFFFFFFF) != 0) {
            dx = player->pos.x - pos->x;
            radius = player->collisionRadius + xzSize;
            if (dx < 0) {
                dx = -dx;
            }
            if (dx < radius) {
                dy = (player->collisionRadius + player->unk5C) - pos->y;
                if (dy < 0) {
                    dy = -dy;
                }
                if (dy < radius) {
                    dz = player->pos.z - pos->z;
                    if (dz < 0) {
                        dz = -dz;
                    }
                    if ((dz < radius) &&
                        (integerSquareRoot64((s64)dx * dx + (s64)dy * dy + (s64)dz * dz) < radius)) {
                        player->pendingItemHitFlags |= flag;
                    }
                }
            }
        }
        player++;
    } while (player != end);
}

s32 tryApplyRacePlayerItemHit(Vec3i *pos, s32 xzSize, s16 flag, s16 playerIndex) {
    volatile u8 pad[8];
    s32 radius;
    s32 dx;
    s32 dy;
    s32 dz;
    RacePlayer *player;

    player = &gRacePlayers[playerIndex];
    if (player->isActive == 0) {
        return 0;
    }
    if (player->actionSoundTimer != 0) {
        return 0;
    }

    dx = player->pos.x - pos->x;
    radius = player->collisionRadius + xzSize;
    if (dx < 0) {
        dx = -dx;
    }
    if (dx < radius) {
        dy = (player->collisionRadius + player->unk5C) - pos->y;
        if (dy < 0) {
            dy = -dy;
        }
        if (dy < radius) {
            dz = player->pos.z - pos->z;
            if (dz < 0) {
                dz = -dz;
            }
            if ((dz < radius) &&
                (integerSquareRoot64((s64)dx * dx + (s64)dy * dy + (s64)dz * dz) < radius)) {
                player->pendingItemHitFlags |= flag;
                return 1;
            }
        }
    }

    return 0;
}

// updateRacePlayerSurfaceContact best match: 99.914% (nonmatchings/updateRacePlayerSurfaceContact-2163214805492048867/base_22.c)
// The assembly references interior elements of gRacePlayerGroundProbeOffsets by fixed offsets.
#pragma GLOBAL_ASM("asm/nonmatchings/race/player/race_player_movement/updateRacePlayerSurfaceContact.s")

#ifdef NON_MATCHING

s32 updateRacePlayerSurfaceContact(RacePlayer *player) {
    MatrixScratch playerRotation;
    MatrixScratch intermediateRotation;
    MatrixScratch worldRotation;
    EffectMatrixScratch effectRotation;
    Vec3i points[6];
    s32 collisionY;
    s32 collisionZ;
    s32 collisionX;
    s32 initialSurfaceHeight;
    s32 hadCollision;
    s32 pushX;
    s32 pushZ;
    s32 normalX;
    s32 normalZ;
    s32 liftLimit;
    s32 contactHeights[6];
    s32 surfaceHeights[6];
    s32 heightDeltas[6];
    s32 longitudinalSpacing;
    s32 sideSpacing;
    s32 baseY;
    s32 verticalOffset;
    s16 terrainId;
    s16 iteration;
    s16 i;
    s32 sine;
    s32 cosine;
    s64 product;

    player->unk500 = 0;
    terrainId = findRaceCourseSurfaceFromHint(player->coursePathIndex, player->pos.x, player->pos.z);
    resolveRaceCourseSurfaceCollisionWithNormal(
        terrainId,
        player->pos.x, player->pos.z, 0x60000, 2, &pushX, &pushZ, &normalX, &normalZ);
    if ((pushX != 0) || (pushZ != 0)) {
        player->pos.x += pushX;
        player->pos.z += pushZ;
    }

    if (player->stateFlags & 0x7C000) {
        terrainId = findRaceCourseSurfaceFromHint(player->coursePathIndex, player->pos.x, player->pos.z);
        resolveRaceCourseSurfaceCollisionWithNormal(
            terrainId, player->pos.x, player->pos.z, 0xC0000, 1, &pushX, &pushZ,
            &player->unk2C8, &player->unk2CC);
        if ((pushX != 0) || (pushZ != 0)) {
            player->pos.x += pushX;
            player->pos.z += pushZ;
            player->pendingItemHitFlags |= 1;
        }

        terrainId = findRaceCourseSurfaceFromHint(player->coursePathIndex, player->pos.x, player->pos.z);
        resolveRaceCourseSurfaceCollisionWithNormal(
            terrainId, player->pos.x, player->pos.z, 0xC0000, 3, &pushX, &pushZ,
            &player->unk2C8, &player->unk2CC);
        if ((pushX != 0) || (pushZ != 0)) {
            player->pos.x += pushX;
            player->pos.z += pushZ;
            player->pendingItemHitFlags |= 0x200;
        }
    } else {
        terrainId = findRaceCourseSurfaceFromHint(player->coursePathIndex, player->pos.x, player->pos.z);
        resolveRaceCourseSurfaceCollisionWithNormal(
            terrainId, player->pos.x, player->pos.z, 0x78000, 1, &pushX, &pushZ,
            &player->unk2C8, &player->unk2CC);
        if ((pushX != 0) || (pushZ != 0)) {
            player->pendingItemHitFlags |= 1;
            player->pos.x += pushX;
            player->pos.z += pushZ;
        }

        terrainId = findRaceCourseSurfaceFromHint(player->coursePathIndex, player->pos.x, player->pos.z);
        resolveRaceCourseSurfaceCollisionWithNormal(
            terrainId, player->pos.x, player->pos.z, 0x78000, 3, &pushX, &pushZ,
            &player->unk2C8, &player->unk2CC);
        if ((pushX != 0) || (pushZ != 0)) {
            player->pendingItemHitFlags |= 0x200;
            player->pos.x += pushX;
            player->pos.z += pushZ;
        }

        sine = fixedSine(player->facingAngle);
        cosine = fixedCosine(player->facingAngle);
        hadCollision = 0;
        for (i = 0; i < 2; i++) {
            points[i].x = RACE_PLAYER_SURFACE_DOT2(gRacePlayerGroundProbeOffsets[i].x, cosine, gRacePlayerGroundProbeOffsets[i].z, sine);
            points[i].z = RACE_PLAYER_SURFACE_DOT2(gRacePlayerGroundProbeOffsets[i].x, -sine, gRacePlayerGroundProbeOffsets[i].z, cosine);
            points[i].x += player->pos.x;
            points[i].z += player->pos.z;

            terrainId = findRaceCourseSurfaceFromHint(player->coursePathIndex, points[i].x, points[i].z);
            resolveRaceCourseSurfaceCollisionWithNormal(
                terrainId, points[i].x, points[i].z, 0x4C000, 1, &pushX, &pushZ,
                &player->unk2C8, &player->unk2CC);
            if ((pushX != 0) || (pushZ != 0)) {
                points[i].x += pushX;
                points[i].z += pushZ;
                hadCollision = 1;
                player->pendingItemHitFlags |= 1;
            }

            terrainId = findRaceCourseSurfaceFromHint(player->coursePathIndex, points[i].x, points[i].z);
            resolveRaceCourseSurfaceCollisionWithNormal(
                terrainId, points[i].x, points[i].z, 0x4C000, 3, &pushX, &pushZ,
                &player->unk2C8, &player->unk2CC);
            if ((pushX != 0) || (pushZ != 0)) {
                points[i].x += pushX;
                points[i].z += pushZ;
                hadCollision = 1;
                player->pendingItemHitFlags |= 0x200;
            }
        }

        if (hadCollision != 0) {
            player->facingAngle =
                calculateFixedAngleBetweenXZPoints(points[1].x, points[1].z, points[0].x, points[0].z);
            player->pos.x = ((s64)points[0].x + points[1].x) / 2;
            player->pos.z = ((s64)points[0].z + points[1].z) / 2;

            sine = fixedSine(player->facingAngle);
            cosine = fixedCosine(player->facingAngle);
            for (i = 0; i < 2; i++) {
                points[i].x = RACE_PLAYER_SURFACE_DOT2(gRacePlayerGroundProbeOffsets[i].x, cosine, gRacePlayerGroundProbeOffsets[i].z, sine);
                points[i].z = RACE_PLAYER_SURFACE_DOT2(gRacePlayerGroundProbeOffsets[i].x, -sine, gRacePlayerGroundProbeOffsets[i].z, cosine);
                points[i].x += player->pos.x;
                points[i].z += player->pos.z;

                terrainId = findRaceCourseSurfaceFromHint(player->coursePathIndex, points[i].x, points[i].z);
                resolveRaceCourseSurfaceCollisionWithNormal(
                    terrainId, points[i].x, points[i].z, 0x4C000, 1, &pushX, &pushZ,
                    &player->unk2C8, &player->unk2CC);
                if ((pushX != 0) || (pushZ != 0)) {
                    player->pos.x += pushX;
                    player->pos.z += pushZ;
                }

                terrainId = findRaceCourseSurfaceFromHint(player->coursePathIndex, points[i].x, points[i].z);
                resolveRaceCourseSurfaceCollisionWithNormal(
                    terrainId, points[i].x, points[i].z, 0x4C000, 3, &pushX, &pushZ,
                    &player->unk2C8, &player->unk2CC);
                if ((pushX != 0) || (pushZ != 0)) {
                    player->pos.x += pushX;
                    player->pos.z += pushZ;
                }
            }
        }
    }

    sine = fixedSine(player->unk2EE);
    cosine = fixedCosine(player->unk2EE);
    sideSpacing = RACE_PLAYER_SURFACE_DOT2(gRacePlayerGroundProbeOffsets[6].x, cosine, gRacePlayerGroundProbeOffsets[6].y, -sine);

    verticalOffset = player->unk5C;
    player->coursePathIndex = findRaceCourseSurfaceFromHint(player->coursePathIndex, player->pos.x, player->pos.z);
    terrainId = player->coursePathIndex;
    initialSurfaceHeight = getRaceCourseSurfaceHeight(terrainId, player->pos.x, player->pos.z);
    if (verticalOffset < initialSurfaceHeight) {
        verticalOffset = initialSurfaceHeight;
    }
    baseY = verticalOffset;

    for (iteration = 0; iteration < 3; iteration++) {
        sine = fixedSine(player->pitchAngle);
        cosine = fixedCosine(player->pitchAngle);
        longitudinalSpacing = RACE_PLAYER_SURFACE_DOT2(gRacePlayerGroundProbeOffsets[8].y, sine, gRacePlayerGroundProbeOffsets[8].z, cosine);
        makeFixedRotationXY(playerRotation.values, player->pitchAngle, player->facingAngle);

        for (i = 0; i < 6; i++) {
            transformVec3iByFixedMatrix(playerRotation.values, &gRacePlayerGroundProbeOffsets[i + 2], &points[i]);
            points[i].x += player->pos.x;
            points[i].y += verticalOffset;
            points[i].z += player->pos.z;
            terrainId = findRaceCourseSurfaceFromHint(player->coursePathIndex, points[i].x, points[i].z);
            surfaceHeights[i] = getRaceCourseSurfaceHeight(terrainId, points[i].x, points[i].z);
            contactHeights[i] = surfaceHeights[i];
            heightDeltas[i] = surfaceHeights[i] - points[i].y;
            if (heightDeltas[i] < 0) {
                contactHeights[i] = points[i].y;
            }
        }

        surfaceHeights[4] = (surfaceHeights[0] + (s64)surfaceHeights[2]) / 2;
        surfaceHeights[5] = (surfaceHeights[1] + (s64)surfaceHeights[3]) / 2;
        if (heightDeltas[0] < heightDeltas[1]) {
            heightDeltas[0] = heightDeltas[1];
            contactHeights[0] = contactHeights[1];
            surfaceHeights[0] = surfaceHeights[1];
        }
        if (heightDeltas[2] < heightDeltas[3]) {
            heightDeltas[2] = heightDeltas[3];
            contactHeights[2] = contactHeights[3];
            surfaceHeights[2] = surfaceHeights[3];
        }
        if (heightDeltas[4] < heightDeltas[5]) {
            heightDeltas[4] = heightDeltas[5];
            contactHeights[4] = contactHeights[5];
        }

        if ((heightDeltas[0] >= 0) && (heightDeltas[2] >= 0)) {
            if (!(player->stateFlags & 4)) {
                player->pitchAngle =
                    calculateFixedAngleFromDeltaXZ(-(contactHeights[0] - contactHeights[2]),
                                                    -longitudinalSpacing * 2) &
                    0xFFF;
            }
            baseY = (contactHeights[0] + (s64)contactHeights[2]) / 2;
        } else if (heightDeltas[0] >= 0) {
            if (!(player->stateFlags & 4)) {
                player->pitchAngle =
                    calculateFixedAngleFromDeltaXZ(-(contactHeights[0] - contactHeights[4]),
                                                    -longitudinalSpacing) &
                    0xFFF;
            }
            baseY = contactHeights[4];
        } else if (heightDeltas[2] >= 0) {
            if (!(player->stateFlags & 4)) {
                player->pitchAngle =
                    calculateFixedAngleFromDeltaXZ(-(contactHeights[4] - contactHeights[2]),
                                                    -longitudinalSpacing) &
                    0xFFF;
            }
            baseY = contactHeights[4];
        }
    }

    player->unk2F0 =
        calculateFixedAngleFromDeltaXZ(-(surfaceHeights[0] - surfaceHeights[2]), -longitudinalSpacing * 2);
    player->unk2F4 =
        calculateFixedAngleFromDeltaXZ(-(surfaceHeights[4] - surfaceHeights[5]), -sideSpacing * 2);
    player->unk64 = 0;
    makeFixedRotationZXY(playerRotation.values, player->pitchAngle, player->facingAngle, player->unk2EE);
    for (i = 0; i < 4; i++) {
        transformVec3iByFixedMatrix(playerRotation.values, &gRacePlayerGroundProbeOffsets[i + 2], &points[i]);
        points[i].x += player->pos.x;
        points[i].z += player->pos.z;
        points[i].y += baseY + player->unk64;
        terrainId = findRaceCourseSurfaceFromHint(player->coursePathIndex, points[i].x, points[i].z);
        contactHeights[i] = getRaceCourseSurfaceHeight(terrainId, points[i].x, points[i].z);
        if (points[i].y < contactHeights[i]) {
            player->unk64 = (player->unk64 + contactHeights[i]) - points[i].y;
        }
    }

    player->unk58 = player->pos.y - baseY;
    if (player->unk58 < 0) {
        player->pos.y -= player->unk58;
        player->unk58 = 0;
    }

    verticalOffset = 0;
    if (player->unk58 < player->unk60) {
        verticalOffset = player->unk60 - player->unk58;
        liftLimit = player->unk260 + 0xC00;
        if (liftLimit < verticalOffset) {
            verticalOffset = liftLimit;
        }
    }
    player->unk58 += verticalOffset;
    player->pos.y += verticalOffset;

    effectRotation.transformedX = FIXED_PRODUCT(playerRotation.values[3], player->collisionCenterOffset);
    effectRotation.transformedY = FIXED_PRODUCT(playerRotation.values[4], player->collisionCenterOffset);
    effectRotation.transformedZ = FIXED_PRODUCT(playerRotation.values[5], player->collisionCenterOffset);

    if (player->stateFlags & 0x400) {
        makeFixedRotationZYX(effectRotation.values, player->unk6C, -player->unk6E, -player->unk70);
        multiplyFixedMatrix3s(effectRotation.values, playerRotation.values, worldRotation.values);
    } else {
        makeFixedRotationZYX(effectRotation.values, player->unk6C, player->unk6E, player->unk70);
        makeFixedRotationY(worldRotation.values, 0x800);
        multiplyFixedMatrix3s(worldRotation.values, playerRotation.values, intermediateRotation.values);
        multiplyFixedMatrix3s(effectRotation.values, intermediateRotation.values, worldRotation.values);
    }

    if (player->stateFlags & 0x400) {
        collisionX = RACE_PLAYER_SURFACE_MIRRORED_COLLISION_POINT(
            worldRotation.values[0], worldRotation.values[3], worldRotation.values[6],
            player->collisionSources[0].sizeX,
            player->collisionSources[0].sizeY - player->collisionCenterOffset,
            player->collisionSources[0].sizeZ);
        collisionY = RACE_PLAYER_SURFACE_MIRRORED_COLLISION_POINT(
            worldRotation.values[1], worldRotation.values[4], worldRotation.values[7],
            player->collisionSources[0].sizeX,
            player->collisionSources[0].sizeY - player->collisionCenterOffset,
            player->collisionSources[0].sizeZ);
        collisionZ = RACE_PLAYER_SURFACE_MIRRORED_COLLISION_POINT(
            worldRotation.values[2], worldRotation.values[5], worldRotation.values[8],
            player->collisionSources[0].sizeX,
            player->collisionSources[0].sizeY - player->collisionCenterOffset,
            player->collisionSources[0].sizeZ);
        makeFixedRotationXYZ(intermediateRotation.values, player->collisionSources[0].rotX,
                             -player->collisionSources[0].rotY, -player->collisionSources[0].rotZ);
    } else {
        collisionX = RACE_PLAYER_SURFACE_COLLISION_POINT(
            worldRotation.values[0], worldRotation.values[3], worldRotation.values[6],
            player->collisionSources[0].sizeX,
            player->collisionSources[0].sizeY - player->collisionCenterOffset,
            player->collisionSources[0].sizeZ);
        collisionY = RACE_PLAYER_SURFACE_COLLISION_POINT(
            worldRotation.values[1], worldRotation.values[4], worldRotation.values[7],
            player->collisionSources[0].sizeX,
            player->collisionSources[0].sizeY - player->collisionCenterOffset,
            player->collisionSources[0].sizeZ);
        collisionZ = RACE_PLAYER_SURFACE_COLLISION_POINT(
            worldRotation.values[2], worldRotation.values[5], worldRotation.values[8],
            player->collisionSources[0].sizeX,
            player->collisionSources[0].sizeY - player->collisionCenterOffset,
            player->collisionSources[0].sizeZ);
        makeFixedRotationXYZ(intermediateRotation.values, player->collisionSources[0].rotX,
                             player->collisionSources[0].rotY, player->collisionSources[0].rotZ);
    }

    collisionX += player->pos.x + effectRotation.transformedX;
    collisionY += player->pos.y - player->unk58 + player->unk64 + effectRotation.transformedY + 0xA000;
    collisionZ += player->pos.z + effectRotation.transformedZ;
    multiplyFixedMatrix3s(intermediateRotation.values, worldRotation.values, playerRotation.values);
    for (i = 0; i < 4; i++) {
        transformVec3iByFixedMatrix(playerRotation.values, &gRacePlayerGroundProbeOffsets[i + 9],
                                    &player->groundMarkerSources[i]);
        player->groundMarkerSources[i].x += collisionX;
        player->groundMarkerSources[i].y += collisionY;
        player->groundMarkerSources[i].z += collisionZ;
        player->markerPoints[i] = player->groundMarkerSources[i];
        terrainId = findRaceCourseSurfaceFromHint(player->coursePathIndex, player->markerPoints[i].x,
                                                  player->markerPoints[i].z);
        player->markerPoints[i].y =
            getRaceCourseSurfaceHeight(terrainId, player->markerPoints[i].x, player->markerPoints[i].z);
        if ((player->markerPoints[i].y + 0x2C000) >= player->groundMarkerSources[i].y) {
            player->unk500 |= 1 << i;
        }
    }

    updateRacePlayerProjectedPosition(player);
    if (player->unk58 == 0x60000) {
        player->stateFlags |= 1;
        return 1;
    }

    player->stateFlags &= ~1;
    if (player->stateFlags & 0x01000000) {
        player->unk2EE = player->unk2F4;
        player->unk2EE &= 0xFFF;
        if (player->unk2EE >= 0x801) {
            player->unk2EE -= 0x1000;
        }
    }
    return 0;
}

#endif

s32 updateRacePlayerGroundAlignment(RacePlayer *player) {
    MatrixScratch playerRotation;
    MatrixScratch intermediateRotation;
    MatrixScratch worldRotation;
    EffectMatrixScratch effectRotation;
    Vec3i points[6];
    GroundProbeScratch probeScratch;
    s32 contactHeights[6];
    s32 surfaceHeights[6];
    s32 heightDeltas[6];
    s32 longitudinalSpacing;
    s32 sideSpacing;
    s16 terrainId;
    s16 i;
    Vec3i collisionPoint;

    player->unk500 = 0;
    terrainId = player->coursePathIndex;

    makeFixedRotationZ(playerRotation.values, player->unk2EE);
    transformVec3iByFixedMatrix(playerRotation.values, &gRacePlayerGroundProbeOffsets[6], points);
    sideSpacing = points[0].x;

    makeFixedRotationX(playerRotation.values, player->pitchAngle);
    transformVec3iByFixedMatrix(playerRotation.values, &gRacePlayerGroundProbeOffsets[8], points);
    longitudinalSpacing = points[0].z;

    probeScratch.baseY = player->pos.y - 0x30000;
    makeFixedRotationXY(playerRotation.values, player->pitchAngle, player->facingAngle);

    for (i = 0; i < 6; i++) {
        transformVec3iByFixedMatrix(playerRotation.values, &gRacePlayerGroundProbeOffsets[i + 2], &points[i]);
        points[i].x += player->pos.x;
        points[i].y += probeScratch.baseY;
        points[i].z += player->pos.z;
        surfaceHeights[i] = getRaceCourseSurfaceHeight(terrainId, points[i].x, points[i].z);
        contactHeights[i] = surfaceHeights[i];
        heightDeltas[i] = surfaceHeights[i] - points[i].y;
        if (heightDeltas[i] < 0) {
            contactHeights[i] = points[i].y;
        }
    }

    surfaceHeights[4] = (surfaceHeights[0] + (s64)surfaceHeights[2]) / 2;
    surfaceHeights[5] = (surfaceHeights[1] + (s64)surfaceHeights[3]) / 2;

    if (heightDeltas[0] < heightDeltas[1]) {
        heightDeltas[0] = heightDeltas[1];
        contactHeights[0] = contactHeights[1];
        surfaceHeights[0] = surfaceHeights[1];
    }
    if (heightDeltas[2] < heightDeltas[3]) {
        heightDeltas[2] = heightDeltas[3];
        contactHeights[2] = contactHeights[3];
        surfaceHeights[2] = surfaceHeights[3];
    }
    if (heightDeltas[4] < heightDeltas[5]) {
        heightDeltas[4] = heightDeltas[5];
        contactHeights[4] = contactHeights[5];
    }

    if ((heightDeltas[0] >= 0) && (heightDeltas[2] >= 0)) {
        if (!(player->stateFlags & 4)) {
            player->pitchAngle =
                calculateFixedAngleFromDeltaXZ(-(contactHeights[0] - contactHeights[2]),
                                                -longitudinalSpacing * 2);
        }
        probeScratch.baseY = (contactHeights[0] + (s64)contactHeights[2]) / 2;
    } else if (heightDeltas[0] >= 0) {
        if (!(player->stateFlags & 4)) {
            player->pitchAngle =
                calculateFixedAngleFromDeltaXZ(-(contactHeights[0] - contactHeights[4]),
                                                -longitudinalSpacing);
        }
        probeScratch.baseY = contactHeights[4];
    } else if (heightDeltas[2] >= 0) {
        if (!(player->stateFlags & 4)) {
            player->pitchAngle =
                calculateFixedAngleFromDeltaXZ(-(contactHeights[4] - contactHeights[2]),
                                                -longitudinalSpacing);
        }
        probeScratch.baseY = contactHeights[4];
    }

    player->unk2F0 =
        calculateFixedAngleFromDeltaXZ(-(surfaceHeights[0] - surfaceHeights[2]), -longitudinalSpacing * 2);
    player->unk2F4 =
        calculateFixedAngleFromDeltaXZ(-(surfaceHeights[4] - surfaceHeights[5]), -sideSpacing * 2);

    player->unk64 = 0;
    makeFixedRotationZXY(playerRotation.values, player->pitchAngle, player->facingAngle, player->unk2EE);
    for (i = 0; i < 4; i++) {
        transformVec3iByFixedMatrix(playerRotation.values, &gRacePlayerGroundProbeOffsets[i + 2], &points[i]);
        points[i].x += player->pos.x;
        points[i].z += player->pos.z;
        points[i].y += probeScratch.baseY + player->unk64;
        contactHeights[i] = getRaceCourseSurfaceHeight(terrainId, points[i].x, points[i].z);
        if (points[i].y < contactHeights[i]) {
            player->unk64 = (player->unk64 + contactHeights[i]) - points[i].y;
        }
    }

    if (player->pos.y < probeScratch.baseY + 0x30000) {
        player->pos.y = probeScratch.baseY + 0x2FFFF;
        player->unk58 = 0x2FFFF;
    } else {
        player->pos.y = probeScratch.baseY + 0x30000;
        player->unk58 = 0x30000;
    }

    effectRotation.transformedX = FIXED_PRODUCT(playerRotation.values[3], player->collisionCenterOffset);
    effectRotation.transformedY = FIXED_PRODUCT(playerRotation.values[4], player->collisionCenterOffset);
    effectRotation.transformedZ = FIXED_PRODUCT(playerRotation.values[5], player->collisionCenterOffset);

    if (player->stateFlags & 0x400) {
        makeFixedRotationZYX(effectRotation.values, player->unk6C, -player->unk6E, -player->unk70);
        multiplyFixedMatrix3s(effectRotation.values, playerRotation.values, worldRotation.values);
    } else {
        makeFixedRotationZYX(effectRotation.values, player->unk6C, player->unk6E, player->unk70);
        makeFixedRotationY(worldRotation.values, 0x800);
        multiplyFixedMatrix3s(worldRotation.values, playerRotation.values, intermediateRotation.values);
        multiplyFixedMatrix3s(effectRotation.values, intermediateRotation.values, worldRotation.values);
    }

    if (player->stateFlags & 0x400) {
        probeScratch.collisionX =
            MIRRORED_COLLISION_POINT(worldRotation.values[0], worldRotation.values[3], worldRotation.values[6],
                                     player->collisionSources[0].sizeX,
                                     player->collisionSources[0].sizeY - player->collisionCenterOffset,
                                     player->collisionSources[0].sizeZ);
        collisionPoint.y =
            MIRRORED_COLLISION_POINT(worldRotation.values[1], worldRotation.values[4], worldRotation.values[7],
                                     player->collisionSources[0].sizeX,
                                     player->collisionSources[0].sizeY - player->collisionCenterOffset,
                                     player->collisionSources[0].sizeZ);
        probeScratch.collisionZ =
            MIRRORED_COLLISION_POINT(worldRotation.values[2], worldRotation.values[5], worldRotation.values[8],
                                     player->collisionSources[0].sizeX,
                                     player->collisionSources[0].sizeY - player->collisionCenterOffset,
                                     player->collisionSources[0].sizeZ);
        makeFixedRotationXYZ(intermediateRotation.values, player->collisionSources[0].rotX,
                             -player->collisionSources[0].rotY, -player->collisionSources[0].rotZ);
    } else {
        probeScratch.collisionX =
            COLLISION_POINT(worldRotation.values[0], worldRotation.values[3], worldRotation.values[6],
                            player->collisionSources[0].sizeX, player->collisionSources[0].sizeY - player->collisionCenterOffset,
                            player->collisionSources[0].sizeZ);
        collisionPoint.y =
            COLLISION_POINT(worldRotation.values[1], worldRotation.values[4], worldRotation.values[7],
                            player->collisionSources[0].sizeX, player->collisionSources[0].sizeY - player->collisionCenterOffset,
                            player->collisionSources[0].sizeZ);
        probeScratch.collisionZ =
            COLLISION_POINT(worldRotation.values[2], worldRotation.values[5], worldRotation.values[8],
                            player->collisionSources[0].sizeX, player->collisionSources[0].sizeY - player->collisionCenterOffset,
                            player->collisionSources[0].sizeZ);
        makeFixedRotationXYZ(intermediateRotation.values, player->collisionSources[0].rotX,
                             player->collisionSources[0].rotY, player->collisionSources[0].rotZ);
    }

    probeScratch.collisionX += player->pos.x + effectRotation.transformedX;
    probeScratch.collisionZ += player->pos.z + effectRotation.transformedZ;
    multiplyFixedMatrix3s(intermediateRotation.values, worldRotation.values, playerRotation.values);

    for (i = 0; i < 4; i++) {
        transformVec3iByFixedMatrix(playerRotation.values, &gRacePlayerGroundProbeOffsets[i + 9],
                                    &player->markerPoints[i]);
        player->markerPoints[i].x += probeScratch.collisionX;
        player->markerPoints[i].z += probeScratch.collisionZ;
        player->markerPoints[i].y =
            getRaceCourseSurfaceHeight(terrainId, player->markerPoints[i].x, player->markerPoints[i].z);
    }

    updateRacePlayerProjectedPosition(player);
    if (player->unk58 == 0x30000) {
        player->stateFlags |= 1;
        return 1;
    }

    player->stateFlags &= ~1;
    return 0;
}

s32 updateRacePlayerLeanAngle(RacePlayer *player, s32 arg1, s16 arg2) {
    s16 temp_v0;
    s16 scale;

    if (arg1 < 0) {
        arg1 = -arg1;
    }
    temp_v0 = player->unk2F6;
    arg2 = arg2 * 0x10;
    arg2 = arg2 - temp_v0;
    if (arg2 >= 0x81) {
        arg2 = 0x80;
    }
    if (arg2 < -0x80) {
        arg2 = -0x80;
    }
    player->unk2F6 = temp_v0 + arg2;
    arg2 = player->unk2F6 * player->unk2F8 / 0x3F;
    if (arg1 >= 0x40001) {
        scale = 0x117;
    } else {
        scale = (arg1 * 0x117) / 0x40000;
    }
    arg1 = player->unk2EE;
    arg2 = arg2 * scale / 0x1F0;
    temp_v0 = arg2;
    player->unk2EE = player->unk2EE + ((temp_v0 - arg1) >> 2);
    return temp_v0;
}

void clampRacePlayerVectorXZSpeed(Vec3i *vec, RacePlayer *player) {
    s32 magnitude;

    magnitude = integerSquareRoot64((s64)vec->x * vec->x + (s64)vec->z * vec->z);
    if (player->unk314 < magnitude) {
        vec->x = (s64)vec->x * player->unk314 / magnitude;
        vec->z = (s64)vec->z * player->unk314 / magnitude;
    }
}

void clampRacePlayerVectorXZHalfSpeed(Vec3i *vec, RacePlayer *player) {
    s32 magnitude;

    magnitude = integerSquareRoot64((s64)vec->x * vec->x + (s64)vec->z * vec->z);
    if ((player->unk314 / 2) < magnitude) {
        vec->x = (s64)vec->x * (player->unk314 / 2) / magnitude;
        vec->z = (s64)vec->z * (player->unk314 / 2) / magnitude;
    }
}

void updateRacePlayerLocalVelocity(RacePlayer *player, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    volatile s32 pad[8];
    TransformScratch scratch;
    MovementSpeedScratch speedScratch;
    s32 useHalfLimit;
    s32 *localPosYPtr;

    speedScratch.sin = fixedSine(-player->facingAngle);
    speedScratch.cos = fixedCosine(-player->facingAngle);

    scratch.localPos.x = ((s64)player->velocity.x * speedScratch.cos + (s64)player->velocity.z * speedScratch.sin) / 0x1000;
    scratch.localPos.z = ((s64)player->velocity.x * -speedScratch.sin + (s64)player->velocity.z * speedScratch.cos) / 0x1000;
    scratch.localPos.y = player->velocity.y;

    makeFixedRotationXZ(scratch.rotationMtx, -player->unk2F0, -player->unk2F4);
    transformVec3iByFixedMatrix(scratch.rotationMtx, &scratch.localPos, &scratch.worldPos);

    if (arg1 > 0) {
        if (scratch.worldPos.z < 0x30000) {
            scratch.worldPos.z += arg1;
        }
    } else if (scratch.worldPos.z >= -0x2FFFF) {
        scratch.worldPos.z += arg1;
    }

    scratch.worldPos.z += arg2;
    useHalfLimit = 0;
    if (scratch.worldPos.y < 0) {
        scratch.worldPos.y = 0;
    }

    if (scratch.worldPos.z < 0) {
        speedScratch.speed = arg4;
    } else {
        speedScratch.speed = arg5;
        if (arg4 != speedScratch.speed) {
            useHalfLimit = 1;
        }
    }

    if (useHalfLimit != 0) {
        clampRacePlayerVectorXZHalfSpeed(&scratch.worldPos, player);
    } else {
        clampRacePlayerVectorXZSpeed(&scratch.worldPos, player);
    }

    if (scratch.worldPos.z >= 0) {
        if (speedScratch.speed < scratch.worldPos.z) {
            scratch.worldPos.z -= speedScratch.speed;
        } else {
            scratch.worldPos.z = 0;
        }
    } else if (scratch.worldPos.z < -speedScratch.speed) {
        scratch.worldPos.z += speedScratch.speed;
    } else {
        scratch.worldPos.z = 0;
    }

    if (scratch.worldPos.x >= 0) {
        if (arg3 < scratch.worldPos.x) {
            scratch.worldPos.x -= arg3;
        } else {
            scratch.worldPos.x = 0;
        }
    } else if (scratch.worldPos.x < -arg3) {
        scratch.worldPos.x += arg3;
    } else {
        scratch.worldPos.x = 0;
    }

    player->unk258 = scratch.worldPos.x;
    player->unk254 = scratch.worldPos.z;

    makeFixedRotationZX(scratch.rotationMtx, player->unk2F0, player->unk2F4);
    localPosYPtr = &scratch.localPos.y;
    transformVec3iByFixedMatrix(scratch.rotationMtx, &scratch.worldPos, &scratch.localPos);

    player->unk74 = scratch.localPos.y + 0x1000;
    scratch.localPos.y = (scratch.localPos.y + player->velocity.y) - (*localPosYPtr);

    speedScratch.sin = fixedSine(player->facingAngle);
    speedScratch.cos = fixedCosine(player->facingAngle);

    player->velocity.x = ((s64)scratch.localPos.x * speedScratch.cos + (s64)scratch.localPos.z * speedScratch.sin) / 0x1000;
    player->velocity.z = ((s64)scratch.localPos.x * -speedScratch.sin + (s64)scratch.localPos.z * speedScratch.cos) / 0x1000;
    player->velocity.y = scratch.localPos.y;
}

void updateRacePlayerLocalVelocityNoVerticalOffset(RacePlayer *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    updateRacePlayerLocalVelocity(arg0, arg1, 0, arg2, arg3, arg4);
}

void addRacePlayerScore(RacePlayer *arg0, s32 arg1) {
    if (arg0->isActive != 0) {
        arg0->score += arg1;
        if (arg0->score >= 0x186A0) {
            arg0->score = 0x1869F;
        }
        arg0->unk56C += arg1;
        if (arg0->unk56C >= 0x186A0) {
            arg0->unk56C = 0x1869F;
        }
    }
}

void enqueueRacePlayerVoiceSound(RacePlayer *player, s16 soundType) {
    if (player->soundDisabled == 0) {
        switch (soundType) {
        case 0:
            enqueuePlayerPositionalSoundEffect(gRacePlayerVoiceSoundIds0[(randomNextMain() & 1) + (player->characterId * 2)],
                          (Vec3i *)&player->pos.x, 0x7F, 0x5A, (u16)player->playerIndex, 0);
            return;
        case 1:
            enqueuePlayerPositionalSoundEffect(gRacePlayerVoiceSoundIds1[(randomNextMain() & 1) + (player->characterId * 2)],
                          (Vec3i *)&player->pos.x, 0x7F, 0x5A, (u16)player->playerIndex, 0);
            return;
        case 2:
            enqueuePlayerPositionalSoundEffect(gRacePlayerVoiceSoundIds2[(randomNextMain() & 1) + (player->characterId * 2)],
                          (Vec3i *)&player->pos.x, 0x7F, 0x5A, (u16)player->playerIndex, 0);
            return;
        case 3:
            enqueuePlayerPositionalSoundEffect(gRacePlayerVoiceSoundIds2[(randomNextMain() & 1) + (player->characterId * 2)],
                          (Vec3i *)&player->pos.x, 0x7F, 0x5A, (u16)player->playerIndex, 0x60);
            return;
        case 4:
            enqueuePlayerPositionalSoundEffect(gRacePlayerVoiceSoundIds4[player->characterId], (Vec3i *)&player->pos.x, 0x7F, 0x5A,
                          (u16)player->playerIndex, 0);
            return;
        case 5:
            enqueuePlayerPositionalSoundEffect(gRacePlayerVoiceSoundIds5[player->characterId], (Vec3i *)&player->pos.x, 0x7F, 0x5A,
                          (u16)player->playerIndex, 0);
            return;
        case 6:
            enqueuePlayerPositionalSoundEffect(gRacePlayerVoiceSoundIds6[player->characterId], (Vec3i *)&player->pos.x, 0x7F, 0x5A,
                          (u16)player->playerIndex, 0);
            return;
        case 7:
            enqueuePlayerPositionalSoundEffect(gRacePlayerVoiceSoundIds7[player->characterId], (Vec3i *)&player->pos.x, 0x7F, 0x5A,
                          (u16)player->playerIndex, 0);
            break;
        }
    }
}

void updateRacePlayerProjectedPosition(RacePlayer *arg0) {
    TransformScratch scratch;

    makeFixedRotationXY(scratch.rotationMtx, arg0->pitchAngle, arg0->facingAngle);
    scratch.localPos.x = 0;
    scratch.localPos.y = 0xC0000;
    scratch.localPos.z = 0;
    transformVec3iByFixedMatrix(scratch.rotationMtx, &scratch.localPos, &scratch.worldPos);
    arg0->projectedPos.x = scratch.worldPos.x + arg0->pos.x;
    arg0->projectedPos.y = scratch.worldPos.y + arg0->pos.y;
    arg0->projectedPos.z = scratch.worldPos.z + arg0->pos.z;
}
