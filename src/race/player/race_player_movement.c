#include "game/race/race_state.h"
#include "game/engine/game_task_scheduler.h"
#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/audio/sound_manager.h"
#include "game/race/motion/race_motion.h"
#include "game/race/camera/race_camera.h"
#include "game/race/course/race_course_effects.h"
#include "game/race/player/race_player_movement.h"
#include "game/math/fixed_point_math.h"
#include "game/math/spatial_math.h"
#include "game/race/flow/race_flow.h"

#define FIXED_PRODUCT(a, b) ((s64)(a) * (b) / 0x1000)
#define COLLISION_POINT(x, y, z, sizeX, sizeY, sizeZ) \
    (((s64)(x) * (sizeX) + (s64)(y) * (sizeY) + (s64)(z) * (sizeZ)) / 0x1000)
#define MIRRORED_COLLISION_POINT(x, y, z, sizeX, sizeY, sizeZ) \
    ((-(s64)(x) * (sizeX) + (s64)(y) * (sizeY) + (s64)(z) * (sizeZ)) / 0x1000)
#define RACE_PLAYER_SURFACE_DOT2(a, b, c, d) ((product = (s64)(a) * (b)), ((product + (s64)(c) * (d)) / 0x1000))
#define RACE_PLAYER_SURFACE_COLLISION_POINT(x, y, z, sizeX, sizeY, sizeZ) \
    (((s64)(y) * (sizeY) + ((product = (s64)(x) * (sizeX)), (product + (s64)(z) * (sizeZ)))) / 0x1000)
#define RACE_PLAYER_SURFACE_MIRRORED_COLLISION_POINT(x, y, z, sizeX, sizeY, sizeZ) \
    (((s64)(y) * (sizeY) + ((product = -(s64)(x) * (sizeX)), (product + (s64)(z) * (sizeZ)))) / 0x1000)

typedef struct {
    Vec3i worldPos;
    Vec3i localPos;
    s16 rotationMtx[0x10];
} RacePlayerTransformScratch;

typedef struct {
    s32 pad0[2];
    s32 cos;
    s32 sin;
    s32 pad1;
    s32 speed;
} RacePlayerVelocityScratch;

typedef struct {
    Mat3x3 values;
    u8 pad[14];
} RacePlayerMatrixScratch;

typedef struct {
    Mat3x3 values;
    s16 pad;
    s32 transformedX;
    s32 transformedY;
    s32 transformedZ;
} RacePlayerEffectMatrixScratch;

typedef struct {
    s32 baseY;
    u8 pad4[0x10];
    s32 collisionX;
    u8 pad18[4];
    s32 collisionZ;
} RacePlayerGroundProbeScratch;

Vec3i gRacePlayerGroundProbeOffsets[] = {
    { 0,        0, -0x80000 },
    { 0,        0, 0x80000  },
    { 0x46000,  0, -0xC6000 },
    { -0x46000, 0, -0xC6000 },
    { 0x46000,  0, 0xC6000  },
    { -0x46000, 0, 0xC6000  },
    { 0x46000,  0, 0        },
    { -0x46000, 0, 0        },
    { 0,        0, 0xC6000  },
    { 0xA6000,  0, -0x46000 },
    { -0xA6000, 0, -0x46000 },
    { 0xA6000,  0, 0x46000  },
    { -0xA6000, 0, 0x46000  },
};

s16 gRacePlayerVoiceSoundIds0[] = {
    SFX_SLASH_TRICK, SFX_SLASH_TRICK, SFX_NANCY_TRICK, SFX_NANCY_TRICK, SFX_JAM_TRICK,      SFX_JAM_TRICK,
    SFX_LINDA_TRICK, SFX_LINDA_TRICK, SFX_TOMMY_TRICK, SFX_TOMMY_TRICK, SFX_SHINOBIN_TRICK, SFX_SHINOBIN_TRICK,
};

s16 gRacePlayerVoiceSoundIds1[] = {
    SFX_SLASH_COMPLEX_TRICK, SFX_SLASH_COMPLEX_TRICK, SFX_NANCY_COMPLEX_TRICK_ALT, SFX_NANCY_COMPLEX_TRICK,
    SFX_JAM_COMPLEX_TRICK,   SFX_JAM_COMPLEX_TRICK,   SFX_LINDA_COMPLEX_TRICK_ALT, SFX_LINDA_COMPLEX_TRICK,
    SFX_TOMMY_COMPLEX_TRICK, SFX_TOMMY_COMPLEX_TRICK, SFX_SHINOBIN_TRICK,          SFX_SHINOBIN_TRICK,
};

s16 gRacePlayerVoiceSoundIds2[] = {
    SFX_SLASH_HIT,     SFX_SLASH_HIT_ALT, SFX_NANCY_HIT_ALT, SFX_NANCY_HIT, SFX_JAM_HIT_ALT,  SFX_JAM_HIT,
    SFX_LINDA_HIT_ALT, SFX_LINDA_HIT,     SFX_TOMMY_HIT,     SFX_TOMMY_HIT, SFX_SHINOBIN_HIT, SFX_SHINOBIN_HIT,
};

s16 gRacePlayerVoiceSoundIds4[] = { SFX_SLASH_OBSTACLE_HIT, SFX_NANCY_OBSTACLE_HIT, SFX_JAM_OBSTACLE_HIT,
                                    SFX_LINDA_OBSTACLE_HIT, SFX_TOMMY_OBSTACLE_HIT, SFX_SHINOBIN_OBSTACLE_HIT };
s16 gRacePlayerVoiceSoundIds5[] = { SFX_SLASH_ATTACK_SUCCESS, SFX_NANCY_ATTACK_SUCCESS, SFX_JAM_ATTACK_SUCCESS,
                                    SFX_LINDA_ATTACK_SUCCESS, SFX_TOMMY_CHEER,          SFX_SHINOBIN_CHEER };
s16 gRacePlayerVoiceSoundIds6[] = { SFX_SLASH_CHEER, SFX_NANCY_CHEER, SFX_JAM_CHEER,
                                    SFX_LINDA_CHEER, SFX_TOMMY_CHEER, SFX_SHINOBIN_CHEER };
s16 gRacePlayerVoiceSoundIds7[] = { SFX_SLASH_CHEER,
                                    SFX_NANCY_CHEER,
                                    SFX_JAM_ATTACK_SUCCESS,
                                    SFX_LINDA_ATTACK_SUCCESS,
                                    SFX_TOMMY_CHEER,
                                    SFX_SHINOBIN_CHEER,
                                    SFX_NONE,
                                    SFX_NONE,
                                    SFX_NONE,
                                    SFX_NONE,
                                    SFX_NONE,
                                    SFX_NONE };

RacePlayerOrder gRaceOrderPlayerIds;

void getRacePlayerRankingProgress(s32 arg0, s32 *arg1, s32 *arg2) {
    RacePlayer *player;
    s32 temp;

    player = &gRacePlayers[arg0];
    *arg1 = player->state.coursePathIndex * 8;
    *arg2 = player->state.coursePathOffset;

    switch (gRaceCourseIndex.signedValue) {
        case RACE_COURSE_BIG_SNOWMAN:
            temp = *arg1;
            if (temp >= 0x580) {
                *arg1 = 0x598 - temp;
                *arg2 = -*arg2;
                return;
            }
            break;
        case RACE_COURSE_SUNSET_ROCK:
            temp = *arg1;
            if (temp >= 0x4A8) {
                *arg1 = 0x4A8 - temp;
                *arg2 = -*arg2;
                return;
            }
            break;
        case RACE_COURSE_NIGHT_HIGHWAY:
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
        case RACE_COURSE_GRASS_VALLEY:
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
        case RACE_COURSE_DIZZY_LAND:
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
        case RACE_COURSE_QUICKSAND_VALLEY:
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
        case RACE_COURSE_SILVER_MOUNTAIN:
            temp = *arg1;
            if ((temp >= 0x7B8) && (temp < 0x7E1)) {
                *arg1 = 0x7F8 - temp;
                *arg2 = -*arg2;
                return;
            }
            break;
        case RACE_COURSE_NINJA_LAND:
            temp = *arg1;
            if ((temp >= 0x1F0) && (temp < 0x219)) {
                *arg1 = 0x1F0 - temp;
                *arg2 = -*arg2;
            }
            break;
    }
}

void updateRacePlayerRankings(void) {
    s8 order[4];
    s32 progress[4];
    s32 pathOffsets[4];
    s32 i;
    s32 j;
    u16 playerIndex;
    u16 swap;

    if (gRaceSplitscreenMode != 2) {
        if (gMenuFlowState & 1) {
            gRaceOrderPlayerIds[0] = 0;
            gRaceOrderPlayerIds[1] = 1;
            gRaceOrderPlayerIds[2] = 2;
            gRaceOrderPlayerIds[3] = 3;
        } else {
            order[0] = 0;
            order[1] = 1;
            order[2] = 2;
            order[3] = 3;

            for (i = 0; i < gRacePlayerCount; i++) {
                getRacePlayerRankingProgress(i, &progress[i], &pathOffsets[i]);
                if (gRacePlayers[i].state.stateFlags & 0x4000000) {
                    progress[i] += gRacePlayers[i].state.unk57C;
                }
            }

            for (i = 0; i < gRacePlayerCount - 1; i++) {
                for (j = i + 1; j < gRacePlayerCount; j++) {
                    if (gRacePlayers[order[j]].state.rankIndex < gRacePlayers[order[i]].state.rankIndex) {
                        swap = order[i];
                        order[i] = order[j];
                        order[j] = swap;
                    }
                }
            }

            for (i = 0; i < gRacePlayerCount - 1; i++) {
                for (j = i + 1; j < gRacePlayerCount; j++) {
                    playerIndex = order[i];
                    if (!(gRacePlayers[order[i]].state.stateFlags & 0x40) && !(gRacePlayers[order[j]].state.stateFlags & 0x40)) {
                        if (gRacePlayers[order[i]].state.lapDigit < gRacePlayers[order[j]].state.lapDigit) {
                            swap = order[i];
                            order[i] = order[j];
                            order[j] = swap;
                        } else {
                            if (gRacePlayers[order[i]].state.lapDigit == gRacePlayers[order[j]].state.lapDigit) {
                                if (progress[order[i]] < progress[order[j]]) {
                                    swap = order[i];
                                    order[i] = order[j];
                                    order[j] = swap;
                                } else if (progress[order[i]] == progress[order[j]]) {
                                    if (pathOffsets[order[j]] > pathOffsets[order[i]]) {
                                        swap = order[i];
                                        order[i] = order[j];
                                        order[j] = swap;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            for (i = 0; i < gRacePlayerCount; i++) {
                gRacePlayers[order[i]].state.rankIndex = i;
                gRaceOrderPlayerIds[i] = gRacePlayers[order[i]].playerIndex;
            }
        }
    }
}

void updateRacePlayerFinalLapStatus(RacePlayer *player) {
    CallbackTask *task;
    u32 flags;

    flags = player->state.stateFlags;
    if (!(flags & 0x40) && (player->state.lapDigit >= (gRaceLapCount - 1)) &&
        (player->state.coursePathIndex == gRaceCourseStartEntries[gRaceCourseIndex.signedValue].pathIndex) &&
        !(flags & 0x1000)) {
        player->state.stateFlags = flags | 0x40;
        if ((gRaceCameraModeChangeDisabled == 0) && (gRacePlayerHudStatuses[player->playerIndex].active != 0)) {
            task = createCallbackTask((CallbackTaskCallback)initFinalLapPrompt, 6, 0x64);
            if (task != NULL) {
                task->header.userId = player->playerIndex;
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
                    if ((playerB->isActive != 0) && (playerA->soundDisabled == 0) && (playerB->soundDisabled == 0) &&
                        !(playerA->state.stateFlags & RACE_PLAYER_COLLISION_SQUASHED) &&
                        !(playerB->state.stateFlags & RACE_PLAYER_COLLISION_SQUASHED)) {
                        temp = playerA->state.collisionBottomY - playerB->state.collisionBottomY;
                        if ((temp < 0 ? (temp = -temp, playerA->state.collisionHeight) : playerB->state.collisionHeight) >= temp) {
                            radius = playerB->state.collisionRadius + playerA->state.collisionRadius;
                            xDiff = playerA->state.pos.x - playerB->state.pos.x;
                            if (xDiff < 0) {
                                xDiff = -xDiff;
                            }
                            if (xDiff < radius) {
                                temp = playerA->state.pos.z - playerB->state.pos.z;
                                if (temp < 0) {
                                    temp *= -1;
                                }
                                if ((temp < radius) &&
                                    ((temp = integerSquareRoot64(
                                          (s64)(0, xDiff) * xDiff + (((s64)temp * temp) & 0xFFFFFFFFFFFFFFFF)
                                      )) < radius)) {
                                    temp = ((radius - temp) * -1) / 2;
                                    angle = calculateFixedAngleBetweenXZPoints(
                                        playerA->state.pos.x,
                                        playerA->state.pos.z,
                                        playerB->state.pos.x,
                                        playerB->state.pos.z
                                    );
                                    sine = fixedSine(angle);
                                    cosine = fixedCosine(angle);
                                    pushX = (s64)-sine * temp / 0x1000;
                                    temp = (s64)cosine * temp / 0x1000;
                                    if (playerA->state.stateFlags & 0x1000) {
                                        if (!(playerB->state.stateFlags & 0x1000)) {
                                            playerB->state.pos.x -= pushX * 2;
                                            playerB->state.pos.z += temp * 2;
                                        }
                                    } else if (playerB->state.stateFlags & 0x1000) {
                                        playerA->state.pos.x += pushX * 2;
                                        playerA->state.pos.z -= temp * 2;
                                    } else {
                                        playerA->state.pos.x += pushX;
                                        playerA->state.pos.z -= temp;
                                        playerB->state.pos.x -= pushX;
                                        playerB->state.pos.z += temp;
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
            temp = pos->y - gRacePlayers[i].state.collisionBottomY;
            if (temp < 0) {
                temp = -temp;
            } else {
                yLimit = gRacePlayers[i].state.collisionHeight;
            }

            if (temp <= yLimit) {
                xzLimit = gRacePlayers[i].state.collisionRadius + xzSize;
                xDiff = pos->x - gRacePlayers[i].state.pos.x;
                if (xDiff < 0) {
                    xDiff = -xDiff;
                }
                if (xDiff < xzLimit) {
                    temp = pos->z - gRacePlayers[i].state.pos.z;
                    if (temp < 0) {
                        temp = -temp;
                    }
                    if ((temp < xzLimit) && ((temp = integerSquareRoot64(
                                                  (s64)(0, xDiff) * xDiff + (((s64)temp * temp) & 0xFFFFFFFFFFFFFFFF)
                                              )) < xzLimit)) {
                        angle = calculateFixedAngleBetweenXZPoints(
                            pos->x,
                            pos->z,
                            gRacePlayers[i].state.pos.x,
                            gRacePlayers[i].state.pos.z
                        );
                        sine = fixedSine(angle);
                        cosine = fixedCosine(angle);
                        temp = xzLimit - temp;
                        temp *= -1;
                        pushX = (s64)-sine * (s64)((s64)temp & 0xFFFFFFFFFFFFFFFF) / 0x1000;
                        pushZ = (s64)cosine * (s64)((s64)temp & 0xFFFFFFFFFFFFFFFF) / 0x1000;
                        gRacePlayers[i].state.pos.x -= pushX;
                        gRacePlayers[i].state.pos.z += pushZ;

                        localX = ((s64)cosine * gRacePlayers[i].state.collisionVelocityX -
                                  (s64)sine * gRacePlayers[i].state.collisionVelocityZ) /
                                 0x1000;
                        localZ = ((s64)sine * gRacePlayers[i].state.collisionVelocityX +
                                  (s64)cosine * gRacePlayers[i].state.collisionVelocityZ) /
                                 0x1000;
                        if (localZ > 0) {
                            localZ = -localZ;
                        }
                        gRacePlayers[i].state.collisionVelocityX = ((s64)cosine * localX + (s64)sine * localZ) / 0x1000;
                        gRacePlayers[i].state.collisionVelocityZ = ((s64)-sine * localX + (s64)cosine * localZ) / 0x1000;
                        gRacePlayers[i].state.pendingItemHitFlags |= flag;
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
        temp = pos->y - player->state.collisionBottomY;
        if (temp < 0) {
            temp = -temp;
        } else {
            yLimit = player->state.collisionHeight;
        }

        if (temp <= yLimit) {
            xzLimit = player->state.collisionRadius + xzSize;
            xDiff = pos->x - player->state.pos.x;
            if (xDiff < 0) {
                xDiff = -xDiff;
            }
            if (xDiff < xzLimit) {
                zDiff = pos->z - player->state.pos.z;
                if (zDiff < 0) {
                    zDiff = -zDiff;
                }
                if ((zDiff < xzLimit) && ((temp = integerSquareRoot64(
                                               (s64)(0, xDiff) * xDiff + (((s64)zDiff * zDiff) & 0xFFFFFFFFFFFFFFFF)
                                           )) < xzLimit)) {
                    angle = calculateFixedAngleBetweenXZPoints(pos->x, pos->z, player->state.pos.x, player->state.pos.z);
                    sine = fixedSine(angle);
                    cosine = fixedCosine(angle);
                    temp = xzLimit - temp;
                    temp *= -1;
                    pushX = (s64)-sine * temp / 0x1000;
                    pushZ = (s64)cosine * temp / 0x1000;
                    player->state.pos.x -= pushX;
                    player->state.pos.z += pushZ;

                    localX =
                        ((s64)cosine * player->state.collisionVelocityX - (s64)sine * player->state.collisionVelocityZ) / 0x1000;
                    localZ =
                        ((s64)sine * player->state.collisionVelocityX + (s64)cosine * player->state.collisionVelocityZ) / 0x1000;
                    if (localZ > 0) {
                        localZ = -localZ;
                    }
                    player->state.collisionVelocityX = ((s64)cosine * localX + (s64)sine * localZ) / 0x1000;
                    player->state.collisionVelocityZ = ((s64)-sine * localX + (s64)cosine * localZ) / 0x1000;
                    player->state.pendingItemHitFlags = player->state.pendingItemHitFlags | flag;
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
            temp = pos->y - gRacePlayers[i].state.collisionBottomY;
            if (temp < 0) {
                temp = -temp;
            } else {
                yLimit = gRacePlayers[i].state.collisionHeight;
            }

            if (temp <= yLimit) {
                xzLimit = gRacePlayers[i].state.collisionRadius + xzSize;
                xDiff = pos->x - gRacePlayers[i].state.pos.x;
                if (xDiff < 0) {
                    xDiff = -xDiff;
                }
                if (xDiff < xzLimit) {
                    temp = pos->z - gRacePlayers[i].state.pos.z;
                    if (temp < 0) {
                        temp = -temp;
                    }
                    if ((temp < xzLimit) && ((temp = integerSquareRoot64(
                                                  (s64)(0, xDiff) * xDiff + (((s64)temp * temp) & 0xFFFFFFFFFFFFFFFF)
                                              )) < xzLimit)) {
                        if (gRacePlayers[i].state.movementSpeed < arg3) {
                            angle = calculateFixedAngleBetweenXZPoints(
                                pos->x,
                                pos->z,
                                gRacePlayers[i].state.pos.x,
                                gRacePlayers[i].state.pos.z
                            );
                            sine = fixedSine(angle);
                            cosine = fixedCosine(angle);
                            temp = xzLimit - temp;
                            pushX = (s64)-sine * -temp / 0x1000;
                            pushZ = (s64)cosine * -temp / 0x1000;
                            gRacePlayers[i].state.pos.x -= pushX;
                            gRacePlayers[i].state.pos.z += pushZ;
                        } else {
                            gRacePlayers[i].state.pendingItemHitFlags |= arg4;
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
        distance = pos->y - player->state.collisionBottomY;
        if (distance < 0) {
            distance = -distance;
        } else {
            yLimit = player->state.collisionHeight;
        }

        if (distance <= yLimit) {
            xzLimit = player->state.collisionRadius + xzSize;
            xDiff = pos->x - player->state.pos.x;
            if (xDiff < 0) {
                xDiff = -xDiff;
            }
            if (xDiff < xzLimit) {
                zDiff = pos->z - player->state.pos.z;
                if (zDiff < 0) {
                    zDiff = -zDiff;
                }
                if ((zDiff < xzLimit) && ((distance = integerSquareRoot64(
                                               (s64)(0, xDiff) * xDiff + (((s64)zDiff * zDiff) & 0xFFFFFFFFFFFFFFFF)
                                           )) < xzLimit)) {
                    angle = calculateFixedAngleBetweenXZPoints(pos->x, pos->z, player->state.pos.x, player->state.pos.z);
                    sine = fixedSine(angle);
                    cosine = fixedCosine(angle);
                    distance = xzLimit - distance;
                    distance *= -1;
                    pushX = (s64)-sine * distance / 0x1000;
                    pushZ = (s64)cosine * distance / 0x1000;
                    player->state.pos.x -= pushX;
                    (&gRacePlayers[playerIndex])->state.pos.z += pushZ;
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

    yDiff = pos->y - player->state.collisionBottomY;
    yLimit = ySize;
    if (yDiff < 0) {
        yDiff = -yDiff;
    } else {
        yLimit = player->state.collisionHeight;
    }

    if (yDiff < yLimit) {
        newLimit = player->state.collisionRadius + xzSize;
        xDiff = pos->x - player->state.pos.x;
        xzLimit = newLimit;
        if (xDiff <= -1) {
            xDiff = -xDiff;
        }
        if (xDiff < xzLimit) {
            zDiff = pos->z - player->state.pos.z;
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

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
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
            dx = player->state.pos.x - pos->x;
            radius = player->state.collisionRadius + xzSize;
            if (dx < 0) {
                dx = -dx;
            }
            if (dx < radius) {
                dy = (player->state.collisionRadius + player->state.collisionBottomY) - pos->y;
                if (dy < 0) {
                    dy = -dy;
                }
                if (dy < radius) {
                    dz = player->state.pos.z - pos->z;
                    if (dz < 0) {
                        dz = -dz;
                    }
                    if ((dz < radius) &&
                        (integerSquareRoot64((s64)dx * dx + (s64)dy * dy + (s64)dz * dz) < radius)) {
                        player->state.pendingItemHitFlags |= flag;
                    }
                }
            }
        }
        player++;
    } while (player != end);
}
// clang-format on

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
    if (player->state.invisibilityTimer != 0) {
        return 0;
    }

    dx = player->state.pos.x - pos->x;
    radius = player->state.collisionRadius + xzSize;
    if (dx < 0) {
        dx = -dx;
    }
    if (dx < radius) {
        dy = (player->state.collisionRadius + player->state.collisionBottomY) - pos->y;
        if (dy < 0) {
            dy = -dy;
        }
        if (dy < radius) {
            dz = player->state.pos.z - pos->z;
            if (dz < 0) {
                dz = -dz;
            }
            if ((dz < radius) && (integerSquareRoot64((s64)dx * dx + (s64)dy * dy + (s64)dz * dz) < radius)) {
                player->state.pendingItemHitFlags |= flag;
                return 1;
            }
        }
    }

    return 0;
}

s32 updateRacePlayerSurfaceContact(RacePlayer *player) {
    RacePlayerMatrixScratch playerRotation;
    RacePlayerMatrixScratch intermediateRotation;
    RacePlayerMatrixScratch worldRotation;
    RacePlayerEffectMatrixScratch effectRotation;
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

    player->state.groundContactMask = 0;
    terrainId = findRaceCourseSurfaceFromHint(player->state.coursePathIndex, player->state.pos.x, player->state.pos.z);
    resolveRaceCourseSurfaceCollisionWithNormal(
        terrainId,
        player->state.pos.x,
        player->state.pos.z,
        0x60000,
        2,
        &pushX,
        &pushZ,
        &normalX,
        &normalZ
    );
    if ((pushX != 0) || (pushZ != 0)) {
        player->state.pos.x += pushX;
        player->state.pos.z += pushZ;
    }

    if (player->state.stateFlags & 0x7C000) {
        terrainId = findRaceCourseSurfaceFromHint(player->state.coursePathIndex, player->state.pos.x, player->state.pos.z);
        resolveRaceCourseSurfaceCollisionWithNormal(
            terrainId,
            player->state.pos.x,
            player->state.pos.z,
            0xC0000,
            1,
            &pushX,
            &pushZ,
            &player->state.collisionVelocityX,
            &player->state.collisionVelocityZ
        );
        if ((pushX != 0) || (pushZ != 0)) {
            player->state.pos.x += pushX;
            player->state.pos.z += pushZ;
            player->state.pendingItemHitFlags |= 1;
        }

        terrainId = findRaceCourseSurfaceFromHint(player->state.coursePathIndex, player->state.pos.x, player->state.pos.z);
        resolveRaceCourseSurfaceCollisionWithNormal(
            terrainId,
            player->state.pos.x,
            player->state.pos.z,
            0xC0000,
            3,
            &pushX,
            &pushZ,
            &player->state.collisionVelocityX,
            &player->state.collisionVelocityZ
        );
        if ((pushX != 0) || (pushZ != 0)) {
            player->state.pos.x += pushX;
            player->state.pos.z += pushZ;
            player->state.pendingItemHitFlags |= 0x200;
        }
    } else {
        terrainId = findRaceCourseSurfaceFromHint(player->state.coursePathIndex, player->state.pos.x, player->state.pos.z);
        resolveRaceCourseSurfaceCollisionWithNormal(
            terrainId,
            player->state.pos.x,
            player->state.pos.z,
            0x78000,
            1,
            &pushX,
            &pushZ,
            &player->state.collisionVelocityX,
            &player->state.collisionVelocityZ
        );
        if ((pushX != 0) || (pushZ != 0)) {
            player->state.pendingItemHitFlags |= 1;
            player->state.pos.x += pushX;
            player->state.pos.z += pushZ;
        }

        terrainId = findRaceCourseSurfaceFromHint(player->state.coursePathIndex, player->state.pos.x, player->state.pos.z);
        resolveRaceCourseSurfaceCollisionWithNormal(
            terrainId,
            player->state.pos.x,
            player->state.pos.z,
            0x78000,
            3,
            &pushX,
            &pushZ,
            &player->state.collisionVelocityX,
            &player->state.collisionVelocityZ
        );
        if ((pushX != 0) || (pushZ != 0)) {
            player->state.pendingItemHitFlags |= 0x200;
            player->state.pos.x += pushX;
            player->state.pos.z += pushZ;
        }

        sine = fixedSine(player->state.facingAngle);
        cosine = fixedCosine(player->state.facingAngle);
        hadCollision = 0;
        for (i = 0; i < 2; i++) {
            points[i].x = RACE_PLAYER_SURFACE_DOT2(
                gRacePlayerGroundProbeOffsets[i].x,
                cosine,
                gRacePlayerGroundProbeOffsets[i].z,
                sine
            );
            points[i].z = RACE_PLAYER_SURFACE_DOT2(
                gRacePlayerGroundProbeOffsets[i].x,
                -sine,
                gRacePlayerGroundProbeOffsets[i].z,
                cosine
            );
            points[i].x += player->state.pos.x;
            points[i].z += player->state.pos.z;

            terrainId = findRaceCourseSurfaceFromHint(player->state.coursePathIndex, points[i].x, points[i].z);
            resolveRaceCourseSurfaceCollisionWithNormal(
                terrainId,
                points[i].x,
                points[i].z,
                0x4C000,
                1,
                &pushX,
                &pushZ,
                &player->state.collisionVelocityX,
                &player->state.collisionVelocityZ
            );
            if ((pushX != 0) || (pushZ != 0)) {
                points[i].x += pushX;
                points[i].z += pushZ;
                hadCollision = 1;
                player->state.pendingItemHitFlags |= 1;
            }

            terrainId = findRaceCourseSurfaceFromHint(player->state.coursePathIndex, points[i].x, points[i].z);
            resolveRaceCourseSurfaceCollisionWithNormal(
                terrainId,
                points[i].x,
                points[i].z,
                0x4C000,
                3,
                &pushX,
                &pushZ,
                &player->state.collisionVelocityX,
                &player->state.collisionVelocityZ
            );
            if ((pushX != 0) || (pushZ != 0)) {
                points[i].x += pushX;
                points[i].z += pushZ;
                hadCollision = 1;
                player->state.pendingItemHitFlags |= 0x200;
            }
        }

        if (hadCollision != 0) {
            player->state.facingAngle =
                calculateFixedAngleBetweenXZPoints(points[1].x, points[1].z, points[0].x, points[0].z);
            player->state.pos.x = ((s64)points[0].x + points[1].x) / 2;
            player->state.pos.z = ((s64)points[0].z + points[1].z) / 2;

            sine = fixedSine(player->state.facingAngle);
            cosine = fixedCosine(player->state.facingAngle);
            for (i = 0; i < 2; i++) {
                points[i].x = RACE_PLAYER_SURFACE_DOT2(
                    gRacePlayerGroundProbeOffsets[i].x,
                    cosine,
                    gRacePlayerGroundProbeOffsets[i].z,
                    sine
                );
                points[i].z = RACE_PLAYER_SURFACE_DOT2(
                    gRacePlayerGroundProbeOffsets[i].x,
                    -sine,
                    gRacePlayerGroundProbeOffsets[i].z,
                    cosine
                );
                points[i].x += player->state.pos.x;
                points[i].z += player->state.pos.z;

                terrainId = findRaceCourseSurfaceFromHint(player->state.coursePathIndex, points[i].x, points[i].z);
                resolveRaceCourseSurfaceCollisionWithNormal(
                    terrainId,
                    points[i].x,
                    points[i].z,
                    0x4C000,
                    1,
                    &pushX,
                    &pushZ,
                    &player->state.collisionVelocityX,
                    &player->state.collisionVelocityZ
                );
                if ((pushX != 0) || (pushZ != 0)) {
                    player->state.pos.x += pushX;
                    player->state.pos.z += pushZ;
                }

                terrainId = findRaceCourseSurfaceFromHint(player->state.coursePathIndex, points[i].x, points[i].z);
                resolveRaceCourseSurfaceCollisionWithNormal(
                    terrainId,
                    points[i].x,
                    points[i].z,
                    0x4C000,
                    3,
                    &pushX,
                    &pushZ,
                    &player->state.collisionVelocityX,
                    &player->state.collisionVelocityZ
                );
                if ((pushX != 0) || (pushZ != 0)) {
                    player->state.pos.x += pushX;
                    player->state.pos.z += pushZ;
                }
            }
        }
    }

    sine = fixedSine(player->state.leanAngle);
    cosine = fixedCosine(player->state.leanAngle);
    sideSpacing =
        RACE_PLAYER_SURFACE_DOT2(gRacePlayerGroundProbeOffsets[6].x, cosine, gRacePlayerGroundProbeOffsets[6].y, -sine);

    verticalOffset = player->state.collisionBottomY;
    player->state.coursePathIndex = findRaceCourseSurfaceFromHint(player->state.coursePathIndex, player->state.pos.x, player->state.pos.z);
    terrainId = player->state.coursePathIndex;
    initialSurfaceHeight = getRaceCourseSurfaceHeight(terrainId, player->state.pos.x, player->state.pos.z);
    if (verticalOffset < initialSurfaceHeight) {
        verticalOffset = initialSurfaceHeight;
    }
    baseY = verticalOffset;

    for (iteration = 0; iteration < 3; iteration++) {
        sine = fixedSine(player->state.pitchAngle);
        cosine = fixedCosine(player->state.pitchAngle);
        longitudinalSpacing = RACE_PLAYER_SURFACE_DOT2(
            gRacePlayerGroundProbeOffsets[8].y,
            sine,
            gRacePlayerGroundProbeOffsets[8].z,
            cosine
        );
        makeFixedRotationXY(playerRotation.values, player->state.pitchAngle, player->state.facingAngle);

        for (i = 0; i < 6; i++) {
            transformVec3iByFixedMatrix(playerRotation.values, &gRacePlayerGroundProbeOffsets[i + 2], &points[i]);
            points[i].x += player->state.pos.x;
            points[i].y += verticalOffset;
            points[i].z += player->state.pos.z;
            terrainId = findRaceCourseSurfaceFromHint(player->state.coursePathIndex, points[i].x, points[i].z);
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
            if (!(player->state.stateFlags & 4)) {
                player->state.pitchAngle =
                    calculateFixedAngleFromDeltaXZ(-(contactHeights[0] - contactHeights[2]), -longitudinalSpacing * 2) &
                    0xFFF;
            }
            baseY = (contactHeights[0] + (s64)contactHeights[2]) / 2;
        } else if (heightDeltas[0] >= 0) {
            if (!(player->state.stateFlags & 4)) {
                player->state.pitchAngle =
                    calculateFixedAngleFromDeltaXZ(-(contactHeights[0] - contactHeights[4]), -longitudinalSpacing) &
                    0xFFF;
            }
            baseY = contactHeights[4];
        } else if (heightDeltas[2] >= 0) {
            if (!(player->state.stateFlags & 4)) {
                player->state.pitchAngle =
                    calculateFixedAngleFromDeltaXZ(-(contactHeights[4] - contactHeights[2]), -longitudinalSpacing) &
                    0xFFF;
            }
            baseY = contactHeights[4];
        }
    }

    player->state.surfacePitchAngle =
        calculateFixedAngleFromDeltaXZ(-(surfaceHeights[0] - surfaceHeights[2]), -longitudinalSpacing * 2);
    player->state.surfaceRollAngle =
        calculateFixedAngleFromDeltaXZ(-(surfaceHeights[4] - surfaceHeights[5]), -sideSpacing * 2);
    player->state.groundCorrectionY = 0;
    makeFixedRotationZXY(playerRotation.values, player->state.pitchAngle, player->state.facingAngle, player->state.leanAngle);
    for (i = 0; i < 4; i++) {
        transformVec3iByFixedMatrix(playerRotation.values, &gRacePlayerGroundProbeOffsets[i + 2], &points[i]);
        points[i].x += player->state.pos.x;
        points[i].z += player->state.pos.z;
        points[i].y += baseY + player->state.groundCorrectionY;
        terrainId = findRaceCourseSurfaceFromHint(player->state.coursePathIndex, points[i].x, points[i].z);
        contactHeights[i] = getRaceCourseSurfaceHeight(terrainId, points[i].x, points[i].z);
        if (points[i].y < contactHeights[i]) {
            player->state.groundCorrectionY = (player->state.groundCorrectionY + contactHeights[i]) - points[i].y;
        }
    }

    player->state.groundOffset = player->state.pos.y - baseY;
    if (player->state.groundOffset < 0) {
        player->state.pos.y -= player->state.groundOffset;
        player->state.groundOffset = 0;
    }

    verticalOffset = 0;
    if (player->state.groundOffset < player->state.targetGroundOffset) {
        verticalOffset = player->state.targetGroundOffset - player->state.groundOffset;
        liftLimit = player->state.gravity + 0xC00;
        if (liftLimit < verticalOffset) {
            verticalOffset = liftLimit;
        }
    }
    player->state.groundOffset += verticalOffset;
    player->state.pos.y += verticalOffset;

    effectRotation.transformedX = FIXED_PRODUCT(playerRotation.values[3], player->state.collisionCenterOffset);
    effectRotation.transformedY = FIXED_PRODUCT(playerRotation.values[4], player->state.collisionCenterOffset);
    effectRotation.transformedZ = FIXED_PRODUCT(playerRotation.values[5], player->state.collisionCenterOffset);

    if (player->state.stateFlags & 0x400) {
        makeFixedRotationZYX(
            effectRotation.values,
            player->state.modelRotationX,
            -player->state.modelRotationY,
            -player->state.modelRotationZ
        );
        multiplyFixedMatrix3s(effectRotation.values, playerRotation.values, worldRotation.values);
    } else {
        makeFixedRotationZYX(
            effectRotation.values,
            player->state.modelRotationX,
            player->state.modelRotationY,
            player->state.modelRotationZ
        );
        makeFixedRotationY(worldRotation.values, 0x800);
        multiplyFixedMatrix3s(worldRotation.values, playerRotation.values, intermediateRotation.values);
        multiplyFixedMatrix3s(effectRotation.values, intermediateRotation.values, worldRotation.values);
    }

    if (player->state.stateFlags & 0x400) {
        collisionX = RACE_PLAYER_SURFACE_MIRRORED_COLLISION_POINT(
            worldRotation.values[0],
            worldRotation.values[3],
            worldRotation.values[6],
            player->state.modelParts[0].offset.x,
            player->state.modelParts[0].offset.y - player->state.collisionCenterOffset,
            player->state.modelParts[0].offset.z
        );
        collisionY = RACE_PLAYER_SURFACE_MIRRORED_COLLISION_POINT(
            worldRotation.values[1],
            worldRotation.values[4],
            worldRotation.values[7],
            player->state.modelParts[0].offset.x,
            player->state.modelParts[0].offset.y - player->state.collisionCenterOffset,
            player->state.modelParts[0].offset.z
        );
        collisionZ = RACE_PLAYER_SURFACE_MIRRORED_COLLISION_POINT(
            worldRotation.values[2],
            worldRotation.values[5],
            worldRotation.values[8],
            player->state.modelParts[0].offset.x,
            player->state.modelParts[0].offset.y - player->state.collisionCenterOffset,
            player->state.modelParts[0].offset.z
        );
        makeFixedRotationXYZ(
            intermediateRotation.values,
            player->state.modelParts[0].rotation.x,
            -player->state.modelParts[0].rotation.y,
            -player->state.modelParts[0].rotation.z
        );
    } else {
        collisionX = RACE_PLAYER_SURFACE_COLLISION_POINT(
            worldRotation.values[0],
            worldRotation.values[3],
            worldRotation.values[6],
            player->state.modelParts[0].offset.x,
            player->state.modelParts[0].offset.y - player->state.collisionCenterOffset,
            player->state.modelParts[0].offset.z
        );
        collisionY = RACE_PLAYER_SURFACE_COLLISION_POINT(
            worldRotation.values[1],
            worldRotation.values[4],
            worldRotation.values[7],
            player->state.modelParts[0].offset.x,
            player->state.modelParts[0].offset.y - player->state.collisionCenterOffset,
            player->state.modelParts[0].offset.z
        );
        collisionZ = RACE_PLAYER_SURFACE_COLLISION_POINT(
            worldRotation.values[2],
            worldRotation.values[5],
            worldRotation.values[8],
            player->state.modelParts[0].offset.x,
            player->state.modelParts[0].offset.y - player->state.collisionCenterOffset,
            player->state.modelParts[0].offset.z
        );
        makeFixedRotationXYZ(
            intermediateRotation.values,
            player->state.modelParts[0].rotation.x,
            player->state.modelParts[0].rotation.y,
            player->state.modelParts[0].rotation.z
        );
    }

    collisionX += player->state.pos.x + effectRotation.transformedX;
    collisionY +=
        player->state.pos.y - player->state.groundOffset + player->state.groundCorrectionY + effectRotation.transformedY + 0xA000;
    collisionZ += player->state.pos.z + effectRotation.transformedZ;
    multiplyFixedMatrix3s(intermediateRotation.values, worldRotation.values, playerRotation.values);
    for (i = 0; i < 4; i++) {
        transformVec3iByFixedMatrix(
            playerRotation.values,
            &gRacePlayerGroundProbeOffsets[i + 9],
            &player->state.boardContactPoints[i]
        );
        player->state.boardContactPoints[i].x += collisionX;
        player->state.boardContactPoints[i].y += collisionY;
        player->state.boardContactPoints[i].z += collisionZ;
        player->state.shadowCorners[i] = player->state.boardContactPoints[i];
        terrainId = findRaceCourseSurfaceFromHint(
            player->state.coursePathIndex,
            player->state.shadowCorners[i].x,
            player->state.shadowCorners[i].z
        );
        player->state.shadowCorners[i].y =
            getRaceCourseSurfaceHeight(terrainId, player->state.shadowCorners[i].x, player->state.shadowCorners[i].z);
        if ((player->state.shadowCorners[i].y + 0x2C000) >= player->state.boardContactPoints[i].y) {
            player->state.groundContactMask |= 1 << i;
        }
    }

    updateRacePlayerProjectedPosition(player);
    if (player->state.groundOffset == 0x60000) {
        player->state.stateFlags |= 1;
        return 1;
    }

    player->state.stateFlags &= ~1;
    if (player->state.stateFlags & 0x01000000) {
        player->state.leanAngle = player->state.surfaceRollAngle;
        player->state.leanAngle &= 0xFFF;
        if (player->state.leanAngle >= 0x801) {
            player->state.leanAngle -= 0x1000;
        }
    }
    return 0;
}

s32 updateRacePlayerGroundAlignment(RacePlayer *player) {
    RacePlayerMatrixScratch playerRotation;
    RacePlayerMatrixScratch intermediateRotation;
    RacePlayerMatrixScratch worldRotation;
    RacePlayerEffectMatrixScratch effectRotation;
    Vec3i points[6];
    RacePlayerGroundProbeScratch probeScratch;
    s32 contactHeights[6];
    s32 surfaceHeights[6];
    s32 heightDeltas[6];
    s32 longitudinalSpacing;
    s32 sideSpacing;
    s16 terrainId;
    s16 i;
    Vec3i collisionPoint;

    player->state.groundContactMask = 0;
    terrainId = player->state.coursePathIndex;

    makeFixedRotationZ(playerRotation.values, player->state.leanAngle);
    transformVec3iByFixedMatrix(playerRotation.values, &gRacePlayerGroundProbeOffsets[6], points);
    sideSpacing = points[0].x;

    makeFixedRotationX(playerRotation.values, player->state.pitchAngle);
    transformVec3iByFixedMatrix(playerRotation.values, &gRacePlayerGroundProbeOffsets[8], points);
    longitudinalSpacing = points[0].z;

    probeScratch.baseY = player->state.pos.y - 0x30000;
    makeFixedRotationXY(playerRotation.values, player->state.pitchAngle, player->state.facingAngle);

    for (i = 0; i < 6; i++) {
        transformVec3iByFixedMatrix(playerRotation.values, &gRacePlayerGroundProbeOffsets[i + 2], &points[i]);
        points[i].x += player->state.pos.x;
        points[i].y += probeScratch.baseY;
        points[i].z += player->state.pos.z;
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
        if (!(player->state.stateFlags & 4)) {
            player->state.pitchAngle =
                calculateFixedAngleFromDeltaXZ(-(contactHeights[0] - contactHeights[2]), -longitudinalSpacing * 2);
        }
        probeScratch.baseY = (contactHeights[0] + (s64)contactHeights[2]) / 2;
    } else if (heightDeltas[0] >= 0) {
        if (!(player->state.stateFlags & 4)) {
            player->state.pitchAngle =
                calculateFixedAngleFromDeltaXZ(-(contactHeights[0] - contactHeights[4]), -longitudinalSpacing);
        }
        probeScratch.baseY = contactHeights[4];
    } else if (heightDeltas[2] >= 0) {
        if (!(player->state.stateFlags & 4)) {
            player->state.pitchAngle =
                calculateFixedAngleFromDeltaXZ(-(contactHeights[4] - contactHeights[2]), -longitudinalSpacing);
        }
        probeScratch.baseY = contactHeights[4];
    }

    player->state.surfacePitchAngle =
        calculateFixedAngleFromDeltaXZ(-(surfaceHeights[0] - surfaceHeights[2]), -longitudinalSpacing * 2);
    player->state.surfaceRollAngle =
        calculateFixedAngleFromDeltaXZ(-(surfaceHeights[4] - surfaceHeights[5]), -sideSpacing * 2);

    player->state.groundCorrectionY = 0;
    makeFixedRotationZXY(playerRotation.values, player->state.pitchAngle, player->state.facingAngle, player->state.leanAngle);
    for (i = 0; i < 4; i++) {
        transformVec3iByFixedMatrix(playerRotation.values, &gRacePlayerGroundProbeOffsets[i + 2], &points[i]);
        points[i].x += player->state.pos.x;
        points[i].z += player->state.pos.z;
        points[i].y += probeScratch.baseY + player->state.groundCorrectionY;
        contactHeights[i] = getRaceCourseSurfaceHeight(terrainId, points[i].x, points[i].z);
        if (points[i].y < contactHeights[i]) {
            player->state.groundCorrectionY = (player->state.groundCorrectionY + contactHeights[i]) - points[i].y;
        }
    }

    if (player->state.pos.y < probeScratch.baseY + 0x30000) {
        player->state.pos.y = probeScratch.baseY + 0x2FFFF;
        player->state.groundOffset = 0x2FFFF;
    } else {
        player->state.pos.y = probeScratch.baseY + 0x30000;
        player->state.groundOffset = 0x30000;
    }

    effectRotation.transformedX = FIXED_PRODUCT(playerRotation.values[3], player->state.collisionCenterOffset);
    effectRotation.transformedY = FIXED_PRODUCT(playerRotation.values[4], player->state.collisionCenterOffset);
    effectRotation.transformedZ = FIXED_PRODUCT(playerRotation.values[5], player->state.collisionCenterOffset);

    if (player->state.stateFlags & 0x400) {
        makeFixedRotationZYX(
            effectRotation.values,
            player->state.modelRotationX,
            -player->state.modelRotationY,
            -player->state.modelRotationZ
        );
        multiplyFixedMatrix3s(effectRotation.values, playerRotation.values, worldRotation.values);
    } else {
        makeFixedRotationZYX(
            effectRotation.values,
            player->state.modelRotationX,
            player->state.modelRotationY,
            player->state.modelRotationZ
        );
        makeFixedRotationY(worldRotation.values, 0x800);
        multiplyFixedMatrix3s(worldRotation.values, playerRotation.values, intermediateRotation.values);
        multiplyFixedMatrix3s(effectRotation.values, intermediateRotation.values, worldRotation.values);
    }

    if (player->state.stateFlags & 0x400) {
        probeScratch.collisionX = MIRRORED_COLLISION_POINT(
            worldRotation.values[0],
            worldRotation.values[3],
            worldRotation.values[6],
            player->state.modelParts[0].offset.x,
            player->state.modelParts[0].offset.y - player->state.collisionCenterOffset,
            player->state.modelParts[0].offset.z
        );
        collisionPoint.y = MIRRORED_COLLISION_POINT(
            worldRotation.values[1],
            worldRotation.values[4],
            worldRotation.values[7],
            player->state.modelParts[0].offset.x,
            player->state.modelParts[0].offset.y - player->state.collisionCenterOffset,
            player->state.modelParts[0].offset.z
        );
        probeScratch.collisionZ = MIRRORED_COLLISION_POINT(
            worldRotation.values[2],
            worldRotation.values[5],
            worldRotation.values[8],
            player->state.modelParts[0].offset.x,
            player->state.modelParts[0].offset.y - player->state.collisionCenterOffset,
            player->state.modelParts[0].offset.z
        );
        makeFixedRotationXYZ(
            intermediateRotation.values,
            player->state.modelParts[0].rotation.x,
            -player->state.modelParts[0].rotation.y,
            -player->state.modelParts[0].rotation.z
        );
    } else {
        probeScratch.collisionX = COLLISION_POINT(
            worldRotation.values[0],
            worldRotation.values[3],
            worldRotation.values[6],
            player->state.modelParts[0].offset.x,
            player->state.modelParts[0].offset.y - player->state.collisionCenterOffset,
            player->state.modelParts[0].offset.z
        );
        collisionPoint.y = COLLISION_POINT(
            worldRotation.values[1],
            worldRotation.values[4],
            worldRotation.values[7],
            player->state.modelParts[0].offset.x,
            player->state.modelParts[0].offset.y - player->state.collisionCenterOffset,
            player->state.modelParts[0].offset.z
        );
        probeScratch.collisionZ = COLLISION_POINT(
            worldRotation.values[2],
            worldRotation.values[5],
            worldRotation.values[8],
            player->state.modelParts[0].offset.x,
            player->state.modelParts[0].offset.y - player->state.collisionCenterOffset,
            player->state.modelParts[0].offset.z
        );
        makeFixedRotationXYZ(
            intermediateRotation.values,
            player->state.modelParts[0].rotation.x,
            player->state.modelParts[0].rotation.y,
            player->state.modelParts[0].rotation.z
        );
    }

    probeScratch.collisionX += player->state.pos.x + effectRotation.transformedX;
    probeScratch.collisionZ += player->state.pos.z + effectRotation.transformedZ;
    multiplyFixedMatrix3s(intermediateRotation.values, worldRotation.values, playerRotation.values);

    for (i = 0; i < 4; i++) {
        transformVec3iByFixedMatrix(
            playerRotation.values,
            &gRacePlayerGroundProbeOffsets[i + 9],
            &player->state.shadowCorners[i]
        );
        player->state.shadowCorners[i].x += probeScratch.collisionX;
        player->state.shadowCorners[i].z += probeScratch.collisionZ;
        player->state.shadowCorners[i].y =
            getRaceCourseSurfaceHeight(terrainId, player->state.shadowCorners[i].x, player->state.shadowCorners[i].z);
    }

    updateRacePlayerProjectedPosition(player);
    if (player->state.groundOffset == 0x30000) {
        player->state.stateFlags |= 1;
        return 1;
    }

    player->state.stateFlags &= ~1;
    return 0;
}

s32 updateRacePlayerLeanAngle(RacePlayer *player, s32 arg1, s16 arg2) {
    s16 temp_v0;
    s16 scale;

    if (arg1 < 0) {
        arg1 = -arg1;
    }
    temp_v0 = player->state.leanInput;
    arg2 = arg2 * 0x10;
    arg2 = arg2 - temp_v0;
    if (arg2 >= 0x81) {
        arg2 = 0x80;
    }
    if (arg2 < -0x80) {
        arg2 = -0x80;
    }
    player->state.leanInput = temp_v0 + arg2;
    arg2 = player->state.leanInput * player->state.leanScale / 0x3F;
    if (arg1 >= 0x40001) {
        scale = 0x117;
    } else {
        scale = (arg1 * 0x117) / 0x40000;
    }
    arg1 = player->state.leanAngle;
    arg2 = arg2 * scale / 0x1F0;
    temp_v0 = arg2;
    player->state.leanAngle = player->state.leanAngle + ((temp_v0 - arg1) >> 2);
    return temp_v0;
}

void clampRacePlayerVectorXZSpeed(Vec3i *vec, RacePlayer *player) {
    s32 magnitude;

    magnitude = integerSquareRoot64((s64)vec->x * vec->x + (s64)vec->z * vec->z);
    if (player->state.speedLimit < magnitude) {
        vec->x = (s64)vec->x * player->state.speedLimit / magnitude;
        vec->z = (s64)vec->z * player->state.speedLimit / magnitude;
    }
}

void clampRacePlayerVectorXZHalfSpeed(Vec3i *vec, RacePlayer *player) {
    s32 magnitude;

    magnitude = integerSquareRoot64((s64)vec->x * vec->x + (s64)vec->z * vec->z);
    if ((player->state.speedLimit / 2) < magnitude) {
        vec->x = (s64)vec->x * (player->state.speedLimit / 2) / magnitude;
        vec->z = (s64)vec->z * (player->state.speedLimit / 2) / magnitude;
    }
}

void updateRacePlayerLocalVelocity(RacePlayer *player, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    volatile s32 pad[8];
    RacePlayerTransformScratch scratch;
    RacePlayerVelocityScratch speedScratch;
    s32 useHalfLimit;
    s32 *localPosYPtr;

    speedScratch.sin = fixedSine(-player->state.facingAngle);
    speedScratch.cos = fixedCosine(-player->state.facingAngle);

    scratch.localPos.x =
        ((s64)player->state.velocity.x * speedScratch.cos + (s64)player->state.velocity.z * speedScratch.sin) / 0x1000;
    scratch.localPos.z =
        ((s64)player->state.velocity.x * -speedScratch.sin + (s64)player->state.velocity.z * speedScratch.cos) / 0x1000;
    scratch.localPos.y = player->state.velocity.y;

    makeFixedRotationXZ(scratch.rotationMtx, -player->state.surfacePitchAngle, -player->state.surfaceRollAngle);
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

    player->state.localVelocityX = scratch.worldPos.x;
    player->state.localVelocityZ = scratch.worldPos.z;

    makeFixedRotationZX(scratch.rotationMtx, player->state.surfacePitchAngle, player->state.surfaceRollAngle);
    localPosYPtr = &scratch.localPos.y;
    transformVec3iByFixedMatrix(scratch.rotationMtx, &scratch.worldPos, &scratch.localPos);

    player->state.verticalVelocityLimit = scratch.localPos.y + 0x1000;
    scratch.localPos.y = (scratch.localPos.y + player->state.velocity.y) - (*localPosYPtr);

    speedScratch.sin = fixedSine(player->state.facingAngle);
    speedScratch.cos = fixedCosine(player->state.facingAngle);

    player->state.velocity.x =
        ((s64)scratch.localPos.x * speedScratch.cos + (s64)scratch.localPos.z * speedScratch.sin) / 0x1000;
    player->state.velocity.z =
        ((s64)scratch.localPos.x * -speedScratch.sin + (s64)scratch.localPos.z * speedScratch.cos) / 0x1000;
    player->state.velocity.y = scratch.localPos.y;
}

void updateRacePlayerLocalVelocityNoVerticalOffset(RacePlayer *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    updateRacePlayerLocalVelocity(arg0, arg1, 0, arg2, arg3, arg4);
}

void addRacePlayerScore(RacePlayer *arg0, s32 arg1) {
    if (arg0->isActive != 0) {
        arg0->state.score += arg1;
        if (arg0->state.score >= 0x186A0) {
            arg0->state.score = 0x1869F;
        }
        arg0->state.cumulativeRaceScore += arg1;
        if (arg0->state.cumulativeRaceScore >= 0x186A0) {
            arg0->state.cumulativeRaceScore = 0x1869F;
        }
    }
}

void enqueueRacePlayerVoiceSound(RacePlayer *player, s16 soundType) {
    if (player->soundDisabled == 0) {
        switch (soundType) {
            case RACE_VOICE_TRICK:
                enqueuePlayerPositionalSoundEffect(
                    gRacePlayerVoiceSoundIds0[(randomNextMain() & 1) + (player->characterId * 2)],
                    &player->state.pos,
                    0x7F,
                    SFX_PRIORITY_HIGH,
                    (u16)player->playerIndex,
                    0
                );
                return;
            case RACE_VOICE_COMPLEX_TRICK:
                enqueuePlayerPositionalSoundEffect(
                    gRacePlayerVoiceSoundIds1[(randomNextMain() & 1) + (player->characterId * 2)],
                    &player->state.pos,
                    0x7F,
                    SFX_PRIORITY_HIGH,
                    (u16)player->playerIndex,
                    0
                );
                return;
            case RACE_VOICE_HIT:
                enqueuePlayerPositionalSoundEffect(
                    gRacePlayerVoiceSoundIds2[(randomNextMain() & 1) + (player->characterId * 2)],
                    &player->state.pos,
                    0x7F,
                    SFX_PRIORITY_HIGH,
                    (u16)player->playerIndex,
                    0
                );
                return;
            case RACE_VOICE_HIT_LOCAL:
                enqueuePlayerPositionalSoundEffect(
                    gRacePlayerVoiceSoundIds2[(randomNextMain() & 1) + (player->characterId * 2)],
                    &player->state.pos,
                    0x7F,
                    SFX_PRIORITY_HIGH,
                    (u16)player->playerIndex,
                    0x60
                );
                return;
            case RACE_VOICE_OBSTACLE_HIT:
                enqueuePlayerPositionalSoundEffect(
                    gRacePlayerVoiceSoundIds4[player->characterId],
                    &player->state.pos,
                    0x7F,
                    SFX_PRIORITY_HIGH,
                    (u16)player->playerIndex,
                    0
                );
                return;
            case RACE_VOICE_ATTACK_SUCCESS:
                enqueuePlayerPositionalSoundEffect(
                    gRacePlayerVoiceSoundIds5[player->characterId],
                    &player->state.pos,
                    0x7F,
                    SFX_PRIORITY_HIGH,
                    (u16)player->playerIndex,
                    0
                );
                return;
            case RACE_VOICE_CHEER:
                enqueuePlayerPositionalSoundEffect(
                    gRacePlayerVoiceSoundIds6[player->characterId],
                    &player->state.pos,
                    0x7F,
                    SFX_PRIORITY_HIGH,
                    (u16)player->playerIndex,
                    0
                );
                return;
            case RACE_VOICE_LANDING:
                enqueuePlayerPositionalSoundEffect(
                    gRacePlayerVoiceSoundIds7[player->characterId],
                    &player->state.pos,
                    0x7F,
                    SFX_PRIORITY_HIGH,
                    (u16)player->playerIndex,
                    0
                );
                break;
        }
    }
}

void updateRacePlayerProjectedPosition(RacePlayer *arg0) {
    RacePlayerTransformScratch scratch;

    makeFixedRotationXY(scratch.rotationMtx, arg0->state.pitchAngle, arg0->state.facingAngle);
    scratch.localPos.x = 0;
    scratch.localPos.y = 0xC0000;
    scratch.localPos.z = 0;
    transformVec3iByFixedMatrix(scratch.rotationMtx, &scratch.localPos, &scratch.worldPos);
    arg0->state.projectedPos.x = scratch.worldPos.x + arg0->state.pos.x;
    arg0->state.projectedPos.y = scratch.worldPos.y + arg0->state.pos.y;
    arg0->state.projectedPos.z = scratch.worldPos.z + arg0->state.pos.z;
}
