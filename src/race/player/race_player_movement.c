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

typedef struct {
    Vec3i worldPos;
    Vec3i localPos;
    Matrix4s rotationMtx;
} TransformScratch;

typedef struct {
    s8 active;
    char pad1[0xAF];
} Unk8011228C;

typedef struct {
    s8 order0;
    s8 order1;
    s8 order2;
    s8 order3;
} PlayerOrder;

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
extern u8 gRaceSplitscreenMode;
extern s8 gRacePlayerCount;
extern u8 gRaceOrderPlayerIds[];
extern s32 gMenuFlowState;
extern Vec3i gRacePlayerGroundProbeOffsets[];
extern Vec3i D_800DE7F8;
extern s32 D_800DE7FC;
extern Vec3i D_800DE810;
extern s32 D_800DE814;
extern s32 D_800DE818;
extern s16 gRacePlayerVoiceSoundIds0[];
extern s16 gRacePlayerVoiceSoundIds1[];
extern s16 gRacePlayerVoiceSoundIds2[];
extern s16 gRacePlayerVoiceSoundIds4[];
extern s16 gRacePlayerVoiceSoundIds5[];
extern s16 gRacePlayerVoiceSoundIds6[];
extern s16 gRacePlayerVoiceSoundIds7[];
extern Unk8011228C gRacePlayerHudStatuses[];
extern s16 gRaceLapCount;

void getRacePlayerRankingProgress(s32 arg0, s32 *arg1, s32 *arg2) {
    RacePlayer *player;
    s32 temp;

    player = &gRacePlayers[arg0];
    *arg1 = player->unk502 * 8;
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

// updateRacePlayerRankings best match: 96.672% (nonmatchings/updateRacePlayerRankings-5176680205357669729/base_20.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/player/race_player_movement/updateRacePlayerRankings.s")

#ifdef NON_MATCHING
#if 0
void updateRacePlayerRankings(void) {
    PlayerOrder order;
    s32 primary[4];
    s32 secondary[4];
    s32 playerCount;
    s32 i;
    s32 j;
    s32 lastPair;
    s8 *orderI;
    s8 *orderJ;
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
        (&order.order0)[0] = 0;
        (&order.order0)[1] = 1;
        (&order.order0)[2] = 2;
        left = 1;
        right = 3;
        (&order.order0)[3] = right;
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
                    orderJ = &(&order.order0)[j];
                    orderI = &(&order.order0)[i];
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
                    } while (orderJ < &(&order.order0)[playerCount]);
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
                        orderI = &(&order.order0)[i];
                        do {
                            left = orderI[0];
                            orderJ = &(&order.order0)[j];
                            if (!(gRacePlayers[left].stateFlags & 0x40)) {
                                right = orderJ[0];
                                if (!(gRacePlayers[right].stateFlags & 0x40)) {
                                    if (gRacePlayers[left].unk508 < gRacePlayers[right].unk508) {
                                        orderI[0] = right;
                                        orderJ[0] = left;
                                    } else if (gRacePlayers[left].unk508 ==
                                            gRacePlayers[right].unk508) {
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
            orderI = &order.order0;
            orderJ = gRaceOrderPlayerIds;
            do {
                right = orderI[playerCount * 0];
                orderI++;
                orderJ++;
                player = &gRacePlayers[right ^ 0];
                player->rankIndex = i;
                i++;
                orderJ[-1] = player->playerIndexU16;
            } while (i < playerCount);
        }
    }
}
#endif
void updateRacePlayerRankings(void) {
    s8 playerIds[4];
    s32 courseProgress[4];
    s32 *nextPtr;
    s32 initializedPlayerId;
    s32 pathProgress[4];
    RacePlayer *player;
    s32 lastPlayer;
    s32 next;
    s32 i;
    s32 j;
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
                do {
                    getRacePlayerRankingProgress(i, &courseProgress[i], &pathProgress[i]);
                    if (player->stateFlags & 0x04000000) {
                        courseProgress[i] = courseProgress[i] + player->unk57C;
                    }
                    i++;
                    player++;
                } while (i < gRacePlayerCount);
                i = 0;
            }

            lastPlayer = (u32)(gRacePlayerCount - 1);
            if (lastPlayer > 0) {
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
                                            playerIds[i] = otherPlayerId;
                                            playerIds[j] = comparedPlayerId;
                                        }
                                    }
                                }
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

void updateRacePlayerFinalLapStatus(RacePlayer *player) {
    CallbackTask *task;
    u32 flags;

    flags = player->stateFlags;
    if (!(flags & 0x40) && (player->unk508 >= (gRaceLapCount - 1)) &&
            (player->unk502 == gRaceCourseStartEntries[gRaceCourseIndex.signedValue].pathIndex) && !(flags & 0x1000)) {
        player->stateFlags = flags | 0x40;
        if ((gRaceCameraModeChangeDisabled == 0) && (gRacePlayerHudStatuses[player->playerIndexU16].active != 0)) {
            task = createCallbackTask((CallbackTaskCallback)initFinalLapPrompt, 6, 0x64);
            if (task != NULL) {
                task->userId = player->playerIndexU16;
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
                        if ((temp < 0 ? (temp = -temp, playerA->unk284) : playerB->unk284) >= temp) {
                            radius = playerB->unk280 + playerA->unk280;
                            xDiff = playerA->posX - playerB->posX;
                            if (xDiff < 0) {
                                xDiff = -xDiff;
                            }
                            if (xDiff < radius) {
                                temp = playerA->posZ - playerB->posZ;
                                if (temp < 0) {
                                    temp *= -1;
                                }
                                if ((temp < radius) &&
                                    ((temp = integerSquareRoot64((s64)((0, xDiff)) * xDiff +
                                                           (((s64)temp * temp) & 0xFFFFFFFFFFFFFFFF))) < radius)) {
                                    temp = ((radius - temp) * -1) / 2;
                                    angle = calculateFixedAngleBetweenXZPoints(playerA->posX, playerA->posZ,
                                                          playerB->posX, playerB->posZ);
                                    sine = fixedSine(angle);
                                    cosine = fixedCosine(angle);
                                    pushX = (s64)-sine * temp / 0x1000;
                                    temp = (s64)cosine * temp / 0x1000;
                                    if (playerA->stateFlags & 0x1000) {
                                        if (!(playerB->stateFlags & 0x1000)) {
                                            playerB->posX -= pushX * 2;
                                            playerB->posZ += temp * 2;
                                        }
                                    } else if (playerB->stateFlags & 0x1000) {
                                        playerA->posX += pushX * 2;
                                        playerA->posZ -= temp * 2;
                                    } else {
                                        playerA->posX += pushX;
                                        playerA->posZ -= temp;
                                        playerB->posX -= pushX;
                                        playerB->posZ += temp;
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

// pushRacePlayersOutOfCylinderAndApplyItemHit best match: 97.776% (nonmatchings/pushRacePlayersOutOfCylinderAndApplyItemHit-2341155904261615822/base_15.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/player/race_player_movement/pushRacePlayersOutOfCylinderAndApplyItemHit.s")

#ifdef NON_MATCHING
void pushRacePlayersOutOfCylinderAndApplyItemHit(Vec3i *pos, s32 xzSize, s32 ySize, u16 flag) {
    volatile u8 pad[8];
    RacePlayer *player;
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

    player = gRacePlayers;
    do {
        if (player->isActive != 0) {
            yLimit = ySize;
            temp = pos->y - player->unk5C;
            if (temp < 0) {
                temp = -temp;
            } else {
                yLimit = player->unk284;
            }

            if (temp <= yLimit) {
                xzLimit = player->unk280 + xzSize;
                xDiff = pos->x - player->posX;
                if (xDiff < 0) {
                    xDiff = -xDiff;
                }
                if (xDiff < xzLimit) {
                    temp = pos->z - player->posZ;
                    if (temp < 0) {
                        temp = -temp;
                    }
                    if ((temp < xzLimit) &&
                        ((temp = integerSquareRoot64((s64)((0, xDiff)) * xDiff +
                                               (((s64)temp * temp) & 0xFFFFFFFFFFFFFFFF))) < xzLimit)) {
                        angle = calculateFixedAngleBetweenXZPoints(pos->x, pos->z, player->posX, player->posZ);
                        sine = fixedSine(angle);
                        cosine = fixedCosine(angle);
                        temp = xzLimit - temp;
                        pushX = (s64)-sine * -temp / 0x1000;
                        pushZ = (s64)cosine * -temp / 0x1000;
                        player->posX -= pushX;
                        player->posZ += pushZ;

                        localX = ((s64)cosine * player->unk2C8 - (s64)sine * player->unk2CC) / 0x1000;
                        localZ = ((s64)sine * player->unk2C8 + (s64)cosine * player->unk2CC) / 0x1000;
                        if (localZ > 0) {
                            localZ = -localZ;
                        }
                        player->unk2C8 = ((s64)cosine * localX + (s64)sine * localZ) / 0x1000;
                        if ((player && player) && player) {
                        }
                        player->unk2CC = ((s64)-sine * localX + (s64)cosine * localZ) / 0x1000;
                        player->pendingItemHitFlags |= flag;
                    }
                }
            }
        }
        player++;
    } while (player != gRacePlayersEnd);
}
#endif

// pushRacePlayerOutOfCylinderAndApplyItemHit best match: 99.832% (nonmatchings/pushRacePlayerOutOfCylinderAndApplyItemHit-6887713755923057488/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/player/race_player_movement/pushRacePlayerOutOfCylinderAndApplyItemHit.s")

#ifdef NON_MATCHING
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

    player = &gRacePlayers[playerIndex];
    if (player->isActive != 0) {
        s32 xDiff;

        yLimit = ySize;
        temp = pos->y - player->unk5C;
        if (temp < 0) {
            temp = -temp;
        } else {
            yLimit = player->unk284;
        }

        if (temp <= yLimit) {
            xzLimit = player->unk280 + xzSize;
            xDiff = pos->x - player->posX;
            if (xDiff < 0) {
                xDiff = -xDiff;
            }
            if (xDiff < xzLimit) {
                zDiff = pos->z - player->posZ;
                if (zDiff < 0) {
                    zDiff = -zDiff;
                }
                if ((zDiff < xzLimit) &&
                    ((temp = integerSquareRoot64((s64)((0, xDiff)) * xDiff +
                                           (((s64)zDiff * zDiff) & 0xFFFFFFFFFFFFFFFF))) < xzLimit)) {
                    angle = calculateFixedAngleBetweenXZPoints(pos->x, pos->z, player->posX, player->posZ);
                    sine = fixedSine(angle);
                    cosine = fixedCosine(angle);
                    temp = xzLimit - temp;
                    pushX = (s64)-sine * -temp / 0x1000;
                    pushZ = (s64)cosine * -temp / 0x1000;
                    player->posX -= pushX;
                    player->posZ += pushZ;

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
#endif

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
                yLimit = gRacePlayers[i].unk284;
            }

            if (temp <= yLimit) {
                xzLimit = gRacePlayers[i].unk280 + xzSize;
                xDiff = pos->x - gRacePlayers[i].posX;
                if (xDiff < 0) {
                    xDiff = -xDiff;
                }
                if (xDiff < xzLimit) {
                    temp = pos->z - gRacePlayers[i].posZ;
                    if (temp < 0) {
                        temp = -temp;
                    }
                    if ((temp < xzLimit) &&
                        ((temp = integerSquareRoot64((s64)((0, xDiff)) * xDiff +
                                               (((s64)temp * temp) & 0xFFFFFFFFFFFFFFFF))) < xzLimit)) {
                        if (gRacePlayers[i].unk29C < arg3) {
                            angle = calculateFixedAngleBetweenXZPoints(pos->x, pos->z, gRacePlayers[i].posX, gRacePlayers[i].posZ);
                            sine = fixedSine(angle);
                            cosine = fixedCosine(angle);
                            temp = xzLimit - temp;
                            pushX = (s64)-sine * -temp / 0x1000;
                            pushZ = (s64)cosine * -temp / 0x1000;
                            gRacePlayers[i].posX -= pushX;
                            gRacePlayers[i].posZ += pushZ;
                        } else {
                            gRacePlayers[i].pendingItemHitFlags |= arg4;
                        }
                    }
                }
            }
        }
    }
}

// pushRacePlayerOutOfCylinder best match: 99.925% (nonmatchings/pushRacePlayerOutOfCylinder-1189375296343516052/base_11.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/player/race_player_movement/pushRacePlayerOutOfCylinder.s")

#ifdef NON_MATCHING
void pushRacePlayerOutOfCylinder(Vec3i *pos, s32 xzSize, s32 ySize, s16 playerIndex) {
    s32 temp;
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
        temp = pos->y - player->unk5C;
        if (temp < 0) {
            temp = -temp;
        } else {
            yLimit = player->unk284;
        }

        if (temp <= yLimit) {
            xzLimit = player->unk280 + xzSize;
            xDiff = pos->x - player->posX;
            if (xDiff < 0) {
                xDiff = -xDiff;
            }
            if (xDiff < xzLimit) {
                zDiff = pos->z - player->posZ;
                if (zDiff < 0) {
                    zDiff = -zDiff;
                }
                if ((zDiff < xzLimit) &&
                    ((temp = integerSquareRoot64((s64)((0, xDiff)) * xDiff +
                                           (((s64)zDiff * zDiff) & 0xFFFFFFFFFFFFFFFF))) < xzLimit)) {
                    angle = calculateFixedAngleBetweenXZPoints(pos->x, pos->z, player->posX, player->posZ);
                    sine = fixedSine(angle);
                    cosine = fixedCosine(angle);
                    temp = xzLimit - temp;
                    pushX = (s64)-sine * -temp / 0x1000;
                    pushZ = (s64)cosine * -temp / 0x1000;
                    player->posX -= pushX;
                    (&gRacePlayers[playerIndex])->posZ += pushZ;
                }
            }
        }
    }
}
#endif

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
        yLimit = player->unk284;
    }

    if (yDiff < yLimit) {
        newLimit = player->unk280 + xzSize;
        xDiff = pos->x - player->posX;
        xzLimit = newLimit;
        if (xDiff <= -1) {
            xDiff = -xDiff;
        }
        if (xDiff < xzLimit) {
            zDiff = pos->z - player->posZ;
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
            dx = player->posX - pos->x;
            radius = player->unk280 + xzSize;
            if (dx < 0) {
                dx = -dx;
            }
            if (dx < radius) {
                dy = (player->unk280 + player->unk5C) - pos->y;
                if (dy < 0) {
                    dy = -dy;
                }
                if (dy < radius) {
                    dz = player->posZ - pos->z;
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

    dx = player->posX - pos->x;
    radius = player->unk280 + xzSize;
    if (dx < 0) {
        dx = -dx;
    }
    if (dx < radius) {
        dy = (player->unk280 + player->unk5C) - pos->y;
        if (dy < 0) {
            dy = -dy;
        }
        if (dy < radius) {
            dz = player->posZ - pos->z;
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

// updateRacePlayerSurfaceContact best match: 99.251% (nonmatchings/updateRacePlayerSurfaceContact-2167615756788266096/base_22.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/player/race_player_movement/updateRacePlayerSurfaceContact.s")

#ifdef NON_MATCHING

s32 updateRacePlayerSurfaceContact(RacePlayer *player) {
    MatrixScratch playerRotation;
    MatrixScratch intermediateRotation;
    MatrixScratch worldRotation;
    EffectMatrixScratch effectRotation;
    Vec3i points[6];
    s32 collisionX;
    s32 pushX;
    s32 pushZ;
    s32 normalX;
    s32 normalZ;
    s32 contactHeights[6];
    s32 surfaceHeights[6];
    s32 heightDeltas[6];
    s32 sideSpacing;
    s32 longitudinalSpacing;
    s32 probeBaseY;
    s32 baseY;
    s32 collisionY;
    s32 collisionZ;
    s32 lift;
    s32 hadCollision;
    s16 terrainId;
    s16 iteration;
    s16 i;
    s32 sine;
    s32 cosine;

    player->unk500 = 0;
    terrainId = findRaceCourseSurfaceFromHint(player->unk502, player->pos.x, player->pos.z);
    resolveRaceCourseSurfaceCollisionWithNormal(
        terrainId,
        player->pos.x, player->pos.z, 0x60000, 2, &pushX, &pushZ, &normalX, &normalZ);
    if ((pushX != 0) || (pushZ != 0)) {
        player->pos.x += pushX;
        player->pos.z += pushZ;
    }

    if (player->stateFlags & 0x7C000) {
        terrainId = findRaceCourseSurfaceFromHint(player->unk502, player->pos.x, player->pos.z);
        resolveRaceCourseSurfaceCollisionWithNormal(
            terrainId, player->pos.x, player->pos.z, 0xC0000, 1, &pushX, &pushZ,
            &player->unk2C8, &player->unk2CC);
        if ((pushX != 0) || (pushZ != 0)) {
            player->pos.x += pushX;
            player->pos.z += pushZ;
            player->pendingItemHitFlags |= 1;
        }

        terrainId = findRaceCourseSurfaceFromHint(player->unk502, player->pos.x, player->pos.z);
        resolveRaceCourseSurfaceCollisionWithNormal(
            terrainId, player->pos.x, player->pos.z, 0xC0000, 3, &pushX, &pushZ,
            &player->unk2C8, &player->unk2CC);
        if ((pushX != 0) || (pushZ != 0)) {
            player->pos.x += pushX;
            player->pos.z += pushZ;
            player->pendingItemHitFlags |= 0x200;
        }
    } else {
        terrainId = findRaceCourseSurfaceFromHint(player->unk502, player->pos.x, player->pos.z);
        resolveRaceCourseSurfaceCollisionWithNormal(
            terrainId, player->pos.x, player->pos.z, 0x78000, 1, &pushX, &pushZ,
            &player->unk2C8, &player->unk2CC);
        if ((pushX != 0) || (pushZ != 0)) {
            player->pendingItemHitFlags |= 1;
            player->pos.x += pushX;
            player->pos.z += pushZ;
        }

        terrainId = findRaceCourseSurfaceFromHint(player->unk502, player->pos.x, player->pos.z);
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
            points[i].x = ((s64)gRacePlayerGroundProbeOffsets[i].x * cosine +
                           (s64)gRacePlayerGroundProbeOffsets[i].z * sine) /
                          0x1000;
            points[i].z = ((s64)gRacePlayerGroundProbeOffsets[i].x * -sine +
                           (s64)gRacePlayerGroundProbeOffsets[i].z * cosine) /
                          0x1000;
            points[i].x += player->pos.x;
            points[i].z += player->pos.z;

            terrainId = findRaceCourseSurfaceFromHint(player->unk502, points[i].x, points[i].z);
            resolveRaceCourseSurfaceCollisionWithNormal(
                terrainId, points[i].x, points[i].z, 0x4C000, 1, &pushX, &pushZ,
                &player->unk2C8, &player->unk2CC);
            if ((pushX != 0) || (pushZ != 0)) {
                points[i].x += pushX;
                points[i].z += pushZ;
                hadCollision = 1;
                player->pendingItemHitFlags |= 1;
            }

            terrainId = findRaceCourseSurfaceFromHint(player->unk502, points[i].x, points[i].z);
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
                points[i].x = ((s64)gRacePlayerGroundProbeOffsets[i].x * cosine +
                               (s64)gRacePlayerGroundProbeOffsets[i].z * sine) /
                              0x1000;
                points[i].z = ((s64)gRacePlayerGroundProbeOffsets[i].x * -sine +
                               (s64)gRacePlayerGroundProbeOffsets[i].z * cosine) /
                              0x1000;
                points[i].x += player->pos.x;
                points[i].z += player->pos.z;

                terrainId = findRaceCourseSurfaceFromHint(player->unk502, points[i].x, points[i].z);
                resolveRaceCourseSurfaceCollisionWithNormal(
                    terrainId, points[i].x, points[i].z, 0x4C000, 1, &pushX, &pushZ,
                    &player->unk2C8, &player->unk2CC);
                if ((pushX != 0) || (pushZ != 0)) {
                    player->pos.x += pushX;
                    player->pos.z += pushZ;
                }

                terrainId = findRaceCourseSurfaceFromHint(player->unk502, points[i].x, points[i].z);
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
    sideSpacing = ((s64)D_800DE7F8.x * cosine + (s64)D_800DE7FC * -sine) / 0x1000;

    probeBaseY = player->unk5C;
    player->unk502 = findRaceCourseSurfaceFromHint(player->unk502, player->pos.x, player->pos.z);
    terrainId = player->unk502;
    collisionX = getRaceCourseSurfaceHeight(terrainId, player->pos.x, player->pos.z);
    if (probeBaseY < collisionX) {
        probeBaseY = collisionX;
    }
    baseY = probeBaseY;

    for (iteration = 0; iteration < 3; iteration++) {
        sine = fixedSine(player->pitchAngle);
        cosine = fixedCosine(player->pitchAngle);
        longitudinalSpacing = ((s64)D_800DE814 * sine + (s64)D_800DE818 * cosine) / 0x1000;
        makeFixedRotationXY(playerRotation.values, player->pitchAngle, player->facingAngle);

        for (i = 0; i < 6; i++) {
            transformVec3iByFixedMatrix(playerRotation.values, &gRacePlayerGroundProbeOffsets[i + 2], &points[i]);
            points[i].x += player->pos.x;
            points[i].y += probeBaseY;
            points[i].z += player->pos.z;
            terrainId = findRaceCourseSurfaceFromHint(player->unk502, points[i].x, points[i].z);
            surfaceHeights[i] = getRaceCourseSurfaceHeight(terrainId, points[i].x, points[i].z);
            contactHeights[i] = surfaceHeights[i];
            heightDeltas[i] = surfaceHeights[i] - points[i].y;
            if (heightDeltas[i] < 0) {
                contactHeights[i] = points[i].y;
            }
        }

        surfaceHeights[4] = ((s64)surfaceHeights[2] + surfaceHeights[0]) / 2;
        surfaceHeights[5] = ((s64)surfaceHeights[3] + surfaceHeights[1]) / 2;
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
            baseY = ((s64)contactHeights[2] + contactHeights[0]) / 2;
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
        terrainId = findRaceCourseSurfaceFromHint(player->unk502, points[i].x, points[i].z);
        collisionY = getRaceCourseSurfaceHeight(terrainId, points[i].x, points[i].z);
        collisionZ = points[i].y;
        contactHeights[i] = collisionY;
        if (collisionZ < collisionY) {
            player->unk64 = (player->unk64 + collisionY) - collisionZ;
        }
    }

    player->unk58 = player->pos.y - baseY;
    if (player->unk58 < 0) {
        player->pos.y -= player->unk58;
        player->unk58 = 0;
    }

    lift = 0;
    if (player->unk58 < player->unk60) {
        lift = player->unk60 - player->unk58;
        if ((player->unk260 + 0xC00) < lift) {
            lift = player->unk260 + 0xC00;
        }
    }
    player->unk58 += lift;
    player->pos.y += lift;
    if (0) {
    }

    effectRotation.transformedX = FIXED_PRODUCT(playerRotation.values[3], player->unk68);
    effectRotation.transformedY = FIXED_PRODUCT(playerRotation.values[4], player->unk68);
    effectRotation.transformedZ = FIXED_PRODUCT(playerRotation.values[5], player->unk68);

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
        collisionX = MIRRORED_COLLISION_POINT(worldRotation.values[0], worldRotation.values[3], worldRotation.values[6],
                                              player->collisionSources[0].sizeX,
                                              player->collisionSources[0].sizeY - player->unk68,
                                              player->collisionSources[0].sizeZ);
        collisionY = MIRRORED_COLLISION_POINT(worldRotation.values[1], worldRotation.values[4], worldRotation.values[7],
                                              player->collisionSources[0].sizeX,
                                              player->collisionSources[0].sizeY - player->unk68,
                                              player->collisionSources[0].sizeZ);
        collisionZ = MIRRORED_COLLISION_POINT(worldRotation.values[2], worldRotation.values[5], worldRotation.values[8],
                                              player->collisionSources[0].sizeX,
                                              player->collisionSources[0].sizeY - player->unk68,
                                              player->collisionSources[0].sizeZ);
        makeFixedRotationXYZ(intermediateRotation.values, player->collisionSources[0].rotX,
                             -player->collisionSources[0].rotY, -player->collisionSources[0].rotZ);
    } else {
        collisionX = COLLISION_POINT(worldRotation.values[0], worldRotation.values[3], worldRotation.values[6],
                                     player->collisionSources[0].sizeX,
                                     player->collisionSources[0].sizeY - player->unk68,
                                     player->collisionSources[0].sizeZ);
        collisionY = COLLISION_POINT(worldRotation.values[1], worldRotation.values[4], worldRotation.values[7],
                                     player->collisionSources[0].sizeX,
                                     player->collisionSources[0].sizeY - player->unk68,
                                     player->collisionSources[0].sizeZ);
        collisionZ = COLLISION_POINT(worldRotation.values[2], worldRotation.values[5], worldRotation.values[8],
                                     player->collisionSources[0].sizeX,
                                     player->collisionSources[0].sizeY - player->unk68,
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
        terrainId = findRaceCourseSurfaceFromHint(player->unk502, player->markerPoints[i].x,
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
    terrainId = player->unk502;

    makeFixedRotationZ(playerRotation.values, player->unk2EE);
    transformVec3iByFixedMatrix(playerRotation.values, &D_800DE7F8, points);
    sideSpacing = points[0].x;

    makeFixedRotationX(playerRotation.values, player->pitchAngle);
    transformVec3iByFixedMatrix(playerRotation.values, &D_800DE810, points);
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

    effectRotation.transformedX = FIXED_PRODUCT(playerRotation.values[3], player->unk68);
    effectRotation.transformedY = FIXED_PRODUCT(playerRotation.values[4], player->unk68);
    effectRotation.transformedZ = FIXED_PRODUCT(playerRotation.values[5], player->unk68);

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
                                     player->collisionSources[0].sizeY - player->unk68,
                                     player->collisionSources[0].sizeZ);
        collisionPoint.y =
            MIRRORED_COLLISION_POINT(worldRotation.values[1], worldRotation.values[4], worldRotation.values[7],
                                     player->collisionSources[0].sizeX,
                                     player->collisionSources[0].sizeY - player->unk68,
                                     player->collisionSources[0].sizeZ);
        probeScratch.collisionZ =
            MIRRORED_COLLISION_POINT(worldRotation.values[2], worldRotation.values[5], worldRotation.values[8],
                                     player->collisionSources[0].sizeX,
                                     player->collisionSources[0].sizeY - player->unk68,
                                     player->collisionSources[0].sizeZ);
        makeFixedRotationXYZ(intermediateRotation.values, player->collisionSources[0].rotX,
                             -player->collisionSources[0].rotY, -player->collisionSources[0].rotZ);
    } else {
        probeScratch.collisionX =
            COLLISION_POINT(worldRotation.values[0], worldRotation.values[3], worldRotation.values[6],
                            player->collisionSources[0].sizeX, player->collisionSources[0].sizeY - player->unk68,
                            player->collisionSources[0].sizeZ);
        collisionPoint.y =
            COLLISION_POINT(worldRotation.values[1], worldRotation.values[4], worldRotation.values[7],
                            player->collisionSources[0].sizeX, player->collisionSources[0].sizeY - player->unk68,
                            player->collisionSources[0].sizeZ);
        probeScratch.collisionZ =
            COLLISION_POINT(worldRotation.values[2], worldRotation.values[5], worldRotation.values[8],
                            player->collisionSources[0].sizeX, player->collisionSources[0].sizeY - player->unk68,
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

    scratch.localPos.x = ((s64)player->unk40.x * speedScratch.cos + (s64)player->unk40.z * speedScratch.sin) / 0x1000;
    scratch.localPos.z = ((s64)player->unk40.x * -speedScratch.sin + (s64)player->unk40.z * speedScratch.cos) / 0x1000;
    scratch.localPos.y = player->unk40.y;

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
    scratch.localPos.y = (scratch.localPos.y + player->unk40.y) - (*localPosYPtr);

    speedScratch.sin = fixedSine(player->facingAngle);
    speedScratch.cos = fixedCosine(player->facingAngle);

    player->unk40.x = ((s64)scratch.localPos.x * speedScratch.cos + (s64)scratch.localPos.z * speedScratch.sin) / 0x1000;
    player->unk40.z = ((s64)scratch.localPos.x * -speedScratch.sin + (s64)scratch.localPos.z * speedScratch.cos) / 0x1000;
    player->unk40.y = scratch.localPos.y;
}

void updateRacePlayerLocalVelocityNoVerticalOffset(RacePlayer *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    updateRacePlayerLocalVelocity(arg0, arg1, 0, arg2, arg3, arg4);
}

void addRacePlayerScore(RacePlayer *arg0, s32 arg1) {
    if (arg0->isActive != 0) {
        arg0->unk568 += arg1;
        if (arg0->unk568 >= 0x186A0) {
            arg0->unk568 = 0x1869F;
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
                          (Vec3i *)&player->posX, 0x7F, 0x5A, (u16)player->playerIndex, 0);
            return;
        case 1:
            enqueuePlayerPositionalSoundEffect(gRacePlayerVoiceSoundIds1[(randomNextMain() & 1) + (player->characterId * 2)],
                          (Vec3i *)&player->posX, 0x7F, 0x5A, (u16)player->playerIndex, 0);
            return;
        case 2:
            enqueuePlayerPositionalSoundEffect(gRacePlayerVoiceSoundIds2[(randomNextMain() & 1) + (player->characterId * 2)],
                          (Vec3i *)&player->posX, 0x7F, 0x5A, (u16)player->playerIndex, 0);
            return;
        case 3:
            enqueuePlayerPositionalSoundEffect(gRacePlayerVoiceSoundIds2[(randomNextMain() & 1) + (player->characterId * 2)],
                          (Vec3i *)&player->posX, 0x7F, 0x5A, (u16)player->playerIndex, 0x60);
            return;
        case 4:
            enqueuePlayerPositionalSoundEffect(gRacePlayerVoiceSoundIds4[player->characterId], (Vec3i *)&player->posX, 0x7F, 0x5A,
                          (u16)player->playerIndex, 0);
            return;
        case 5:
            enqueuePlayerPositionalSoundEffect(gRacePlayerVoiceSoundIds5[player->characterId], (Vec3i *)&player->posX, 0x7F, 0x5A,
                          (u16)player->playerIndex, 0);
            return;
        case 6:
            enqueuePlayerPositionalSoundEffect(gRacePlayerVoiceSoundIds6[player->characterId], (Vec3i *)&player->posX, 0x7F, 0x5A,
                          (u16)player->playerIndex, 0);
            return;
        case 7:
            enqueuePlayerPositionalSoundEffect(gRacePlayerVoiceSoundIds7[player->characterId], (Vec3i *)&player->posX, 0x7F, 0x5A,
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
    arg0->projectedPos.x = scratch.worldPos.x + arg0->posX;
    arg0->projectedPos.y = scratch.worldPos.y + arg0->posY;
    arg0->projectedPos.z = scratch.worldPos.z + arg0->posZ;
}
