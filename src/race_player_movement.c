#include "common.h"
#include "asset_manager.h"
#include "callback_task_scheduler.h"
#include "game_audio.h"
#include "model_animation.h"
#include "race_course_effects.h"
#include "race_input_history.h"
#include "race_player_movement.h"

typedef struct {
    RaceVec3i worldPos;
    RaceVec3i localPos;
    Matrix4s rotationMtx;
} TransformScratch;

typedef struct {
    s16 unk0;
    char pad2[0x46];
} RaceCourseStartEntry;

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
    s32 pad[3];
    s32 speed;
} MovementSpeedScratch;

extern s32 calculateAngleBetweenXZPoints(s32, s32, s32, s32);
extern s16 calculateAngleFromDeltaXZ(s32, s32);
extern void makeFixedRotationX(Matrix4s, s16);
extern void makeFixedRotationY(Matrix4s, s16);
extern void makeFixedRotationZ(Matrix4s, s16);
extern void multiplyFixedMatrix3s(Matrix4s, Matrix4s, Matrix4s);
extern void makeFixedRotationXYZ(Matrix4s, s16, s16, s16);
extern void makeFixedRotationXY(Matrix4s, s16, s16, RaceInputPlayer *);
extern void makeFixedRotationZX(Matrix4s, s16, s16);
extern void makeFixedRotationXZ(Matrix4s, s16, s16);
extern void makeFixedRotationZXY(Matrix4s, s16, s16, s16);
extern void makeFixedRotationZYX(Matrix4s, s16, s16, s16);
extern void transformVec3iByFixedMatrix(Matrix4s, RaceVec3i *, RaceVec3i *);
extern s16 fixedSine(s16);
extern s16 fixedCosine(s16);
extern s32 integerSquareRoot64(s64);
extern u8 D_800EC9C2;
extern s8 gRacePlayerCount;
extern s8 gRaceOrderPlayerIds[];
extern RaceInputPlayer gFrameCounter;
extern s32 D_801235B4;
extern RaceVec3i D_800DE7B0[];
extern s16 D_800DE84C[];
extern s16 D_800DE864[];
extern s16 D_800DE87C[];
extern s16 D_800DE894[];
extern s16 D_800DE8A0[];
extern s16 D_800DE8AC[];
extern s16 D_800DE8B8[];
extern RaceCourseStartEntry D_800B9540[];
extern Unk8011228C D_8011228C[];
extern s16 D_80121B50;
extern s16 D_80121B52;
extern u8 D_80121B58;

void func_80087600(s32 arg0, s32 *arg1, s32 *arg2) {
    RaceInputPlayer *player;
    s32 temp;

    player = &D_80121D80[arg0];
    *arg1 = player->unk502 * 8;
    *arg2 = player->unk504;

    switch (D_80121B50) {
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

// func_80087AFC best match: 82.773% (nonmatchings/func_80087AFC-4923837976568703863/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80087AFC.s")

#ifdef NON_MATCHING
void func_80087AFC(void) {
    PlayerOrder order;
    s32 primary[4];
    s32 secondary[4];
    RaceInputPlayer *player;
    RaceInputPlayer *player2;
    RaceInputPlayer *base;
    s8 *orderI;
    s8 *orderJ;
    s8 *orderEnd;
    s8 *rankPtr;
    s32 playerCount;
    s32 lastPair;
    s32 i;
    s32 j;
    s8 right;
    s8 left;

    if (D_800EC9C2 != 2) {
        if (D_801235B4 & 1) {
            gRaceOrderPlayerIds[0] = 0;
            gRaceOrderPlayerIds[1] = 1;
            gRaceOrderPlayerIds[2] = 2;
            gRaceOrderPlayerIds[3] = 3;
            return;
        }

        playerCount = gRacePlayerCount;
        order.order0 = 0;
        order.order1 = 1;
        order.order2 = 2;
        order.order3 = 3;
        i = 0;
        if (playerCount > 0) {
            player = D_80121D80;
            do {
                func_80087600(i, &primary[i], &secondary[i]);
                i++;
                if ((s32)(player->stateFlags << 5) < 0) {
                    primary[i - 1] += player->unk57C;
                }
                player++;
            } while (i < gRacePlayerCount);
            i = 0;
        }

        lastPair = playerCount - 1;
        if (lastPair > 0) {
            base = D_80121D80;
            do {
                j = i + 1;
                if (j < playerCount) {
                    orderJ = &(&order.order0)[j];
                    orderI = &(&order.order0)[i];
                    orderEnd = &(&order.order0)[playerCount];
                    do {
                        right = orderJ[0];
                        left = orderI[0];
                        player = &base[right];
                        player2 = &base[left];
                        if (player->rankIndex < player2->rankIndex) {
                            orderI[0] = right;
                            orderJ[0] = left;
                        }
                        orderJ++;
                    } while (orderJ < orderEnd);
                }
                j = i + 1;
                i = j;
            } while (j < lastPair);
            i = 0;
        }

        base = D_80121D80;
        if (lastPair > 0) {
            do {
                j = i + 1;
                if (j < playerCount) {
                    orderI = &(&order.order0)[i];
                    do {
                        left = orderI[0];
                        orderJ = &(&order.order0)[j];
                        player = &base[left];
                        if (!(player->stateFlags & 0x40)) {
                            right = orderJ[0];
                            player2 = &base[right];
                            if (!(player2->stateFlags & 0x40)) {
                                if (player->unk508 < player2->unk508) {
                                    orderI[0] = right;
                                    orderJ[0] = left;
                                } else if (player->unk508 == player2->unk508) {
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
                        }
                        j++;
                    } while (j < playerCount);
                }
                j = i + 1;
                i = j;
            } while (j < lastPair);
            i = 0;
        }

        if (playerCount > 0) {
            rankPtr = gRaceOrderPlayerIds;
            orderI = &order.order0;
            do {
                right = orderI[0];
                orderI++;
                rankPtr++;
                player = &base[right];
                player->rankIndex = i;
                i++;
                rankPtr[-1] = player->playerIndex;
            } while (i < playerCount);
        }
    }
}
#endif

void func_80087E14(RaceInputPlayer *player) {
    CallbackTask *task;
    u32 flags;

    flags = player->stateFlags;
    if (!(flags & 0x40) && (player->unk508 >= (D_80121B52 - 1)) &&
            (player->unk502 == D_800B9540[D_80121B50].unk0) && !(flags & 0x1000)) {
        player->stateFlags = flags | 0x40;
        if ((D_80121B58 == 0) && (D_8011228C[player->playerIndexU16].active != 0)) {
            task = createCallbackTask(func_8006A85C, 6, 0x64);
            if (task != NULL) {
                task->userId = player->playerIndexU16;
            }
        }
    }
}

// func_80087EFC best match: 96.174% (nonmatchings/func_80087EFC-2225551288923588688/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80087EFC.s")

#ifdef NON_MATCHING
void func_80087EFC(void) {
    RaceInputPlayer *playerA;
    RaceInputPlayer *playerB;
    s32 i;
    s32 j;
    s32 nextI;
    s32 temp;
    s32 xDiff;
    s32 yLimit;
    s32 radius;
    s16 angle;
    s32 sine;
    s32 cosine;
    s32 pushX;
    s32 pushZ;

    i = 0;
    do {
        j = i + 1;
        nextI = j;
        if (j < 4) {
            playerA = &D_80121D80[i];
            do {
                if (playerA->isActive != 0) {
                    playerB = &D_80121D80[j];
                    if ((playerB->isActive != 0) && (playerA->soundDisabled == 0) &&
                        (playerB->soundDisabled == 0) && !(playerA->stateFlags & 0x200000) &&
                        !(playerB->stateFlags & 0x200000)) {
                        temp = playerA->unk5C - playerB->unk5C;
                        if (temp < 0) {
                            yLimit = playerA->unk284;
                            temp = -temp;
                        } else {
                            yLimit = playerB->unk284;
                        }

                        if (temp <= yLimit) {
                            xDiff = playerA->posX - playerB->posX;
                            radius = playerB->unk280 + playerA->unk280;
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
                                    angle = calculateAngleBetweenXZPoints(playerA->posX, playerA->posZ,
                                                          playerB->posX, playerB->posZ);
                                    sine = fixedSine(angle);
                                    cosine = fixedCosine(angle);
                                    pushX = (s64)-sine * temp / 0x1000;
                                    pushZ = (s64)cosine * temp / 0x1000;
                                    if (playerA->stateFlags & 0x1000) {
                                        if (!(playerB->stateFlags & 0x1000)) {
                                            playerB->posX -= pushX * 2;
                                            playerB->posZ += pushZ * 2;
                                        }
                                    } else if (playerB->stateFlags & 0x1000) {
                                        playerA->posX += pushX * 2;
                                        playerA->posZ -= pushZ * 2;
                                    } else {
                                        playerA->posX += pushX;
                                        playerA->posZ -= pushZ;
                                        playerB->posX -= pushX;
                                        playerB->posZ += pushZ;
                                    }
                                }
                            }
                        }
                    }
                }
                j++;
            } while (j != 4);
        }
        i = nextI;
    } while (i != 3);
}
#endif

// func_80088294 best match: 97.245% (nonmatchings/func_80088294-8207005055717715604/base_9.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80088294.s")

#ifdef NON_MATCHING
void func_80088294(RaceVec3i *pos, s32 xzSize, s32 ySize, u16 flag) {
    volatile u8 pad[8];
    RaceInputPlayer *player;
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

    player = D_80121D80;
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
                        angle = calculateAngleBetweenXZPoints(pos->x, pos->z, player->posX, player->posZ);
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
                        player->pendingItemHitFlags |= flag;
                    }
                }
            }
        }
        player++;
    } while (player != &gFrameCounter);
}
#endif

// func_80088664 best match: 99.643% (nonmatchings/func_80088664-6113366811127043669/base_order_18.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80088664.s")

#ifdef NON_MATCHING
void func_80088664(RaceVec3i *pos, s32 xzSize, s32 ySize, u16 flag, s16 playerIndex) {
    volatile u8 pad[8];
    RaceInputPlayer *player;
    s32 temp;
    s32 xDiff;
    s32 yLimit;
    s32 sine;
    s32 xzLimit;
    s32 pushX;
    s32 pushZ;
    s16 angle;
    s32 localX;
    s32 localZ;
    s32 cosine;

    player = &D_80121D80[playerIndex];
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
                    angle = calculateAngleBetweenXZPoints(pos->x, pos->z, player->posX, player->posZ);
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
                    player->pendingItemHitFlags |= flag;
                }
            }
        }
    }
}
#endif

void func_80088A1C(RaceVec3i *pos, s32 xzSize, s32 ySize, s32 arg3, s16 arg4) {
    volatile u8 pad[16];
    RaceInputPlayer *player;
    s32 temp;
    s32 xDiff;
    s32 yLimit;
    s32 xzLimit;
    s16 angle;
    s32 sine;
    s32 cosine;
    s32 pushX;
    s32 pushZ;

    player = D_80121D80;
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
                        if (player->unk29C < arg3) {
                            angle = calculateAngleBetweenXZPoints(pos->x, pos->z, player->posX, player->posZ);
                            sine = fixedSine(angle);
                            cosine = fixedCosine(angle);
                            temp = xzLimit - temp;
                            pushX = (s64)-sine * -temp / 0x1000;
                            pushZ = (s64)cosine * -temp / 0x1000;
                            player->posX -= pushX;
                            player->posZ += pushZ;
                        } else {
                            player->pendingItemHitFlags |= arg4;
                        }
                    }
                }
            }
        }
        player++;
    } while (player != &gFrameCounter);
}

// func_80088C80 best match: 99.776% (nonmatchings/func_80088C80-7273315160691878794/base_13.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80088C80.s")

#ifdef NON_MATCHING
void func_80088C80(RaceVec3i *pos, s32 xzSize, s32 ySize, s16 playerIndex) {
    RaceInputPlayer *player;
    s32 temp;
    s32 xzLimit;
    s32 xDiff;
    s32 yLimit;
    s32 sine;
    s32 cosine;
    s32 pushX;
    s32 pushZ;

    player = &D_80121D80[playerIndex];
    if (player->isActive != 0) {
        s16 angle;

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
                volatile u8 pad[8];
                temp = pos->z - player->posZ;
                if (temp < 0) {
                    temp = -temp;
                }
                if ((temp < xzLimit) &&
                    ((temp = integerSquareRoot64((s64)((0, xDiff)) * xDiff +
                                           (((s64)temp * temp) & 0xFFFFFFFFFFFFFFFF))) < xzLimit)) {
                    angle = calculateAngleBetweenXZPoints(pos->x, pos->z, player->posX, player->posZ);
                    sine = fixedSine(angle);
                    cosine = fixedCosine(angle);
                    temp = xzLimit - temp;
                    pushX = (s64)-sine * -temp / 0x1000;
                    pushZ = (s64)cosine * -temp / 0x1000;
                    player->posX -= pushX;
                    (&D_80121D80[playerIndex])->posZ += pushZ;
                }
            }
        }
    }
}
#endif

// func_80088E98 best match: 99.719% (nonmatchings/func_80088E98-6688367443449623229/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80088E98.s")

#ifdef NON_MATCHING
s32 func_80088E98(RaceVec3i *pos, s32 xzSize, s32 ySize, s16 playerIndex) {
    RaceInputPlayer *player;
    s32 yDiff;
    s32 newLimit;
    s32 yLimit;
    s32 zDiff;
    s32 xDiff;
    s32 xzLimit;
    s16 result;
    volatile s32 stackPad[4];

    player = &D_80121D80[playerIndex];
    result = 0;
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
                result = 1;
            }
        }
    }

    return result;
}
#endif

void func_80089000(RaceVec3i *pos, s32 xzSize, s16 flag) {
    volatile u8 pad[16];
    RaceInputPlayer *player;
    RaceInputPlayer *end;
    s32 radius;
    s32 dx;
    s32 dy;
    s32 dz;

    end = &gFrameCounter; player = D_80121D80;
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

s32 func_800891B8(RaceVec3i *pos, s32 xzSize, s16 flag, s16 playerIndex) {
    volatile u8 pad[8];
    s32 radius;
    s32 dx;
    s32 dy;
    s32 dz;
    RaceInputPlayer *player;

    player = &D_80121D80[playerIndex];
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

// func_80089374 best match: 63.781% (nonmatchings/func_80089374-731940616440357983/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80089374.s")

// func_8008A940 best match: 76.717% (nonmatchings/func_8008A940-5752545231564691495/base_9.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_8008A940.s")

#ifdef NON_MATCHING
void func_8008A940(RaceInputPlayer *player) {
    volatile u8 pad[16];
    Matrix4s mtx;
    Matrix4s tiltMtx;
    Matrix4s baseMtx;
    Matrix4s effectMtx;
    RaceVec3i points[6];
    s32 heightDiffs[6];
    s32 groundHeights[6];
    s32 transformedX;
    s32 transformedY;
    s32 transformedZ;
    s32 frontMidGround;
    s32 backMidGround;
    s32 baseY;
    s32 pitchSpan;
    s32 rollSpan;
    s32 frontHeightDiff;
    s32 backHeightDiff;
    s32 sideHeightDiff;
    s16 i;
    s32 terrainId;
    s32 stateFlags;
    RaceInputPlayer *temp_s2;
    RaceVec3i *point;
    volatile s32 extraPad[6];

    temp_s2 = player;
    temp_s2->unk500 = 0;
    terrainId = temp_s2->unk502;

    makeFixedRotationZ(mtx, temp_s2->unk2EE);
    transformVec3iByFixedMatrix(mtx, &D_800DE7F8, points);
    pitchSpan = points[0].x;

    makeFixedRotationX(mtx, temp_s2->pitchAngle);
    transformVec3iByFixedMatrix(mtx, &D_800DE810, points);
    rollSpan = points[0].z;

    baseY = temp_s2->posY - 0x30000;
    makeFixedRotationXY(mtx, temp_s2->pitchAngle, temp_s2->facingAngle, temp_s2);

    i = 0;
    do {
        point = &points[i];
        transformVec3iByFixedMatrix(mtx, &D_800DE7B0[i + 2], point);
        point->x += temp_s2->posX;
        point->y += baseY;
        point->z += temp_s2->posZ;
        groundHeights[i] = func_80080CC4(terrainId, point->x, point->z);
        heightDiffs[i] = groundHeights[i] - point->y;
        if (heightDiffs[i] < 0) {
            groundHeights[i] = point->y;
        }
        i++;
    } while (i < 6);

    frontMidGround = (s64)(groundHeights[0] + groundHeights[2]) / 2;
    backMidGround = (s64)(groundHeights[1] + groundHeights[3]) / 2;

    frontHeightDiff = heightDiffs[0];
    if (frontHeightDiff < heightDiffs[1]) {
        frontHeightDiff = heightDiffs[1];
        groundHeights[0] = groundHeights[1];
        points[0].y = points[1].y;
    }
    if (heightDiffs[2] < heightDiffs[3]) {
        heightDiffs[2] = heightDiffs[3];
        groundHeights[2] = groundHeights[3];
        points[2].y = points[3].y;
    }
    if (heightDiffs[4] < heightDiffs[5]) {
        heightDiffs[4] = heightDiffs[5];
        groundHeights[4] = groundHeights[5];
    }

    if ((frontHeightDiff >= 0) && (heightDiffs[2] >= 0)) {
        heightDiffs[0] = frontHeightDiff;
        if (!(temp_s2->stateFlags & 4)) {
            temp_s2->pitchAngle = calculateAngleFromDeltaXZ(-(groundHeights[0] - groundHeights[2]), -rollSpan * 2);
        }
        baseY = (s64)(groundHeights[2] + groundHeights[0]) / 2;
    } else {
        heightDiffs[0] = frontHeightDiff;
        if (frontHeightDiff >= 0) {
            if (!(temp_s2->stateFlags & 4)) {
                temp_s2->pitchAngle = calculateAngleFromDeltaXZ(-(groundHeights[0] - groundHeights[4]), -rollSpan);
            }
            baseY = groundHeights[4];
        } else if (heightDiffs[2] >= 0) {
            if (!(temp_s2->stateFlags & 4)) {
                temp_s2->pitchAngle = calculateAngleFromDeltaXZ(-(groundHeights[4] - groundHeights[2]), -rollSpan);
            }
            baseY = groundHeights[4];
        }
    }

    temp_s2->unk2F0 = calculateAngleFromDeltaXZ(-(points[0].y - points[2].y), -rollSpan * 2);
    temp_s2->unk2F4 = calculateAngleFromDeltaXZ(-(frontMidGround - backMidGround), -pitchSpan * 2);
    temp_s2->unk64 = 0;

    makeFixedRotationZXY(mtx, temp_s2->pitchAngle, temp_s2->facingAngle, temp_s2->unk2EE);
    i = 0;
    do {
        point = &points[i];
        transformVec3iByFixedMatrix(mtx, &D_800DE7B0[i + 2], point);
        point->x += temp_s2->posX;
        point->z += temp_s2->posZ;
        point->y += baseY + temp_s2->unk64;
        groundHeights[i] = func_80080CC4(terrainId, point->x, point->z);
        if (point->y < groundHeights[i]) {
            temp_s2->unk64 += groundHeights[i] - point->y;
        }
        i++;
    } while (i < 4);

    if (temp_s2->posY < baseY + 0x30000) {
        temp_s2->posY = baseY + 0x2FFFF;
        temp_s2->unk58 = 0x2FFFF;
    } else {
        temp_s2->posY = baseY + 0x30000;
        temp_s2->unk58 = 0x30000;
    }

    transformedX = (s64)mtx[3] * temp_s2->unk68 / 0x1000;
    transformedY = (s64)mtx[4] * temp_s2->unk68 / 0x1000;
    transformedZ = (s64)mtx[5] * temp_s2->unk68 / 0x1000;

    if (temp_s2->stateFlags & 0x400) {
        makeFixedRotationZYX(effectMtx, temp_s2->unk6C, -temp_s2->unk6E, -temp_s2->unk70);
        multiplyFixedMatrix3s(effectMtx, mtx, baseMtx);
    } else {
        makeFixedRotationZYX(effectMtx, temp_s2->unk6C, temp_s2->unk6E, temp_s2->unk70);
        makeFixedRotationY(baseMtx, 0x800);
        multiplyFixedMatrix3s(baseMtx, mtx, tiltMtx);
        multiplyFixedMatrix3s(effectMtx, tiltMtx, baseMtx);
    }

    stateFlags = temp_s2->stateFlags;
    if (stateFlags & 0x400) {
        sideHeightDiff = ((s64)baseMtx[3] * (temp_s2->unk344 - temp_s2->unk68) +
                          (s64)-baseMtx[0] * temp_s2->unk340 + (s64)baseMtx[6] * temp_s2->unk348) /
                         0x1000;
        ((s64)baseMtx[4] * (temp_s2->unk344 - temp_s2->unk68) + (s64)-baseMtx[1] * temp_s2->unk340 +
         (s64)baseMtx[7] * temp_s2->unk348) /
            0x1000;
        backHeightDiff = ((s64)baseMtx[5] * (temp_s2->unk344 - temp_s2->unk68) +
                          (s64)-baseMtx[2] * temp_s2->unk340 + (s64)baseMtx[8] * temp_s2->unk348) /
                         0x1000;
        makeFixedRotationXYZ(tiltMtx, temp_s2->unk33A, -temp_s2->unk33C, -temp_s2->unk33E);
    } else {
        sideHeightDiff = ((s64)baseMtx[3] * (temp_s2->unk344 - temp_s2->unk68) +
                          (s64)baseMtx[0] * temp_s2->unk340 + (s64)baseMtx[6] * temp_s2->unk348) /
                         0x1000;
        ((s64)baseMtx[4] * (temp_s2->unk344 - temp_s2->unk68) + (s64)baseMtx[1] * temp_s2->unk340 +
         (s64)baseMtx[7] * temp_s2->unk348) /
            0x1000;
        backHeightDiff = ((s64)baseMtx[5] * (temp_s2->unk344 - temp_s2->unk68) +
                          (s64)baseMtx[2] * temp_s2->unk340 + (s64)baseMtx[8] * temp_s2->unk348) /
                         0x1000;
        makeFixedRotationXYZ(tiltMtx, temp_s2->unk33A, temp_s2->unk33C, temp_s2->unk33E);
    }

    sideHeightDiff += temp_s2->posX + transformedX;
    backHeightDiff += temp_s2->posZ + transformedZ;
    multiplyFixedMatrix3s(tiltMtx, baseMtx, mtx);

    i = 0;
    do {
        transformVec3iByFixedMatrix(mtx, &D_800DE7B0[i + 9], &temp_s2->markerPoints[i]);
        temp_s2->markerPoints[i].x += sideHeightDiff;
        temp_s2->markerPoints[i].z += backHeightDiff;
        temp_s2->markerPoints[i].y =
            func_80080CC4(terrainId, temp_s2->markerPoints[i].x, temp_s2->markerPoints[i].z);
        i++;
    } while (i < 4);

    func_8008BE1C(temp_s2);
    if (temp_s2->unk58 == 0x30000) {
        temp_s2->stateFlags |= 1;
        return;
    }
    temp_s2->stateFlags &= ~1;
}
#endif

// func_8008B408 best match: 95.985% (nonmatchings/func_8008B408-4/output-255-1/source.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_8008B408.s")

#ifdef NON_MATCHING
s32 func_8008B408(RaceInputPlayer *player, s32 arg1, s16 arg2) {
    s16 temp_v0;
    s32 threshold;
    s32 scale;
    s32 temp;

    if (arg1 < 0) {
        arg1 = -arg1;
    }

    temp_v0 = player->unk2F6;
    arg2 = (arg2 * 0x10) - temp_v0;
    scale = (arg1 << 2) << 1;
    if (arg2 >= 0x81) {
        arg2 = 0x80;
    }
    scale += arg1;
    if (arg2 < -0x80) {
        arg2 = -0x80;
    }
    player->unk2F6 = (unsigned long long)(temp_v0 + arg2);

    arg2 = player->unk2F6 * player->unk2F8 / 0x3F;
    scale <<= 2;
    scale -= arg1;
    scale <<= 3;
    scale -= arg1;
    if (arg1 >= (threshold = 0x40001)) {
        scale = 0x117;
    } else {
        temp = scale >> 18;
        if (scale <= -1) {
            scale++;
            scale--;
            temp = (unsigned long long)((scale + 0x3FFFF) >> 18);
        }
        scale = (s16)((unsigned long long)temp);
    }

    arg1 = player->unk2EE;
    temp_v0 = arg2 * scale / 0x1F0;
    player->unk2EE = player->unk2EE + ((temp_v0 - arg1) >> 2);
    return temp_v0;
}
#endif

void func_8008B508(RaceVec3i *vec, RaceInputPlayer *player) {
    s32 magnitude;

    magnitude = integerSquareRoot64((s64)vec->x * vec->x + (s64)vec->z * vec->z);
    if (player->unk314 < magnitude) {
        vec->x = (s64)vec->x * player->unk314 / magnitude;
        vec->z = (s64)vec->z * player->unk314 / magnitude;
    }
}

void func_8008B60C(RaceVec3i *vec, RaceInputPlayer *player) {
    s32 magnitude;

    magnitude = integerSquareRoot64((s64)vec->x * vec->x + (s64)vec->z * vec->z);
    if ((player->unk314 / 2) < magnitude) {
        vec->x = (s64)vec->x * (player->unk314 / 2) / magnitude;
        vec->z = (s64)vec->z * (player->unk314 / 2) / magnitude;
    }
}

// func_8008B73C best match: 99.739% (nonmatchings/func_8008B73C-2225551288923588688/base_16.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_8008B73C.s")

#ifdef NON_MATCHING
void func_8008B73C(RaceInputPlayer *player, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    volatile s32 pad[8];
    TransformScratch scratch;
    MovementSpeedScratch speedScratch;
    s32 sin;
    s32 cos;
    s32 useHalfLimit;

    sin = fixedSine(-player->facingAngle);
    cos = fixedCosine(-player->facingAngle);

    scratch.localPos.x = ((s64)player->unk40.x * cos + (s64)player->unk40.z * sin) / 0x1000;
    scratch.localPos.z = ((s64)player->unk40.x * -sin + (s64)player->unk40.z * cos) / 0x1000;
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
        func_8008B60C(&scratch.worldPos, player);
    } else {
        func_8008B508(&scratch.worldPos, player);
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
    transformVec3iByFixedMatrix(scratch.rotationMtx, &scratch.worldPos, &scratch.localPos);

    player->unk74 = scratch.localPos.y + 0x1000;
    scratch.localPos.y = (scratch.localPos.y + player->unk40.y) - scratch.localPos.y;

    sin = fixedSine(player->facingAngle);
    cos = fixedCosine(player->facingAngle);

    player->unk40.x = ((s64)scratch.localPos.x * cos + (s64)scratch.localPos.z * sin) / 0x1000;
    player->unk40.z = ((s64)scratch.localPos.x * -sin + (s64)scratch.localPos.z * cos) / 0x1000;
    player->unk40.y = scratch.localPos.y;
}
#endif

void func_8008BB20(RaceInputPlayer *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_8008B73C(arg0, arg1, 0, arg2, arg3, arg4);
}

void func_8008BB5C(RaceInputPlayer *arg0, s32 arg1) {
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

void func_8008BBB8(RaceInputPlayer *player, s16 soundType) {
    if (player->soundDisabled == 0) {
        switch (soundType) {
        case 0:
            func_80072AC8(D_800DE84C[(randomNextMain() & 1) + (player->characterId * 2)],
                          (SoundPosition *)&player->posX, 0x7F, 0x5A, (u16)player->playerIndex, 0);
            return;
        case 1:
            func_80072AC8(D_800DE864[(randomNextMain() & 1) + (player->characterId * 2)],
                          (SoundPosition *)&player->posX, 0x7F, 0x5A, (u16)player->playerIndex, 0);
            return;
        case 2:
            func_80072AC8(D_800DE87C[(randomNextMain() & 1) + (player->characterId * 2)],
                          (SoundPosition *)&player->posX, 0x7F, 0x5A, (u16)player->playerIndex, 0);
            return;
        case 3:
            func_80072AC8(D_800DE87C[(randomNextMain() & 1) + (player->characterId * 2)],
                          (SoundPosition *)&player->posX, 0x7F, 0x5A, (u16)player->playerIndex, 0x60);
            return;
        case 4:
            func_80072AC8(D_800DE894[player->characterId], (SoundPosition *)&player->posX, 0x7F, 0x5A,
                          (u16)player->playerIndex, 0);
            return;
        case 5:
            func_80072AC8(D_800DE8A0[player->characterId], (SoundPosition *)&player->posX, 0x7F, 0x5A,
                          (u16)player->playerIndex, 0);
            return;
        case 6:
            func_80072AC8(D_800DE8AC[player->characterId], (SoundPosition *)&player->posX, 0x7F, 0x5A,
                          (u16)player->playerIndex, 0);
            return;
        case 7:
            func_80072AC8(D_800DE8B8[player->characterId], (SoundPosition *)&player->posX, 0x7F, 0x5A,
                          (u16)player->playerIndex, 0);
            break;
        }
    }
}

void func_8008BE1C(RaceInputPlayer *arg0) {
    TransformScratch scratch;

    makeFixedRotationXY(scratch.rotationMtx, arg0->pitchAngle, arg0->facingAngle, arg0);
    scratch.localPos.x = 0;
    scratch.localPos.y = 0xC0000;
    scratch.localPos.z = 0;
    transformVec3iByFixedMatrix(scratch.rotationMtx, &scratch.localPos, &scratch.worldPos);
    arg0->projectedPos.x = scratch.worldPos.x + arg0->posX;
    arg0->projectedPos.y = scratch.worldPos.y + arg0->posY;
    arg0->projectedPos.z = scratch.worldPos.z + arg0->posZ;
}
