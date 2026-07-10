#include "common.h"
#include "asset_decompression.h"
#include "effect_task_scheduler.h"
#include "game_audio.h"
#include "race_input_history.h"

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

extern void func_8008B73C(RaceInputPlayer *, s32, s32, s32, s32, s32);
extern s32 func_8004940C(s32, s32, s32, s32);
extern void func_80097FE4(Matrix4s, s16, s16, RaceInputPlayer *);
extern void func_80098590(Matrix4s, RaceVec3i *, RaceVec3i *);
extern s16 func_80097AE8(s16);
extern s16 func_80097B48(s16);
extern s32 func_80098C30(s64);
extern void func_80087600(s32, s32 *, s32 *);
extern u8 D_800EC9C2;
extern s8 D_80121B54;
extern s8 D_80121D70[];
extern RaceInputPlayer D_801235B0;
extern s32 D_801235B4;
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

void func_8006A85C(EffectTask *task);

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80087600.s")

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
            D_80121D70[0] = 0;
            D_80121D70[1] = 1;
            D_80121D70[2] = 2;
            D_80121D70[3] = 3;
            return;
        }

        playerCount = D_80121B54;
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
            } while (i < D_80121B54);
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
                        if (player->unk509 < player2->unk509) {
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
            rankPtr = D_80121D70;
            orderI = &order.order0;
            do {
                right = orderI[0];
                orderI++;
                rankPtr++;
                player = &base[right];
                player->unk509 = i;
                i++;
                rankPtr[-1] = player->playerIndex;
            } while (i < playerCount);
        }
    }
}
#endif

void func_80087E14(RaceInputPlayer *player) {
    EffectTask *task;
    u32 flags;

    flags = player->stateFlags;
    if (!(flags & 0x40) && (player->unk508 >= (D_80121B52 - 1)) &&
            (player->unk502 == D_800B9540[D_80121B50].unk0) && !(flags & 0x1000)) {
        player->stateFlags = flags | 0x40;
        if ((D_80121B58 == 0) && (D_8011228C[player->playerIndexU16].active != 0)) {
            task = func_80071408(func_8006A85C, 6, 0x64);
            if (task != NULL) {
                task->unk10 = player->playerIndexU16;
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80087EFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80088294.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80088664.s")

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
                        ((temp = func_80098C30((s64)((0, xDiff)) * xDiff +
                                               (((s64)temp * temp) & 0xFFFFFFFFFFFFFFFF))) < xzLimit)) {
                        if (player->unk29C < arg3) {
                            angle = func_8004940C(pos->x, pos->z, player->posX, player->posZ);
                            sine = func_80097AE8(angle);
                            cosine = func_80097B48(angle);
                            temp = xzLimit - temp;
                            pushX = (s64)-sine * -temp / 0x1000;
                            pushZ = (s64)cosine * -temp / 0x1000;
                            player->posX -= pushX;
                            player->posZ += pushZ;
                        } else {
                            player->unk2C6 |= arg4;
                        }
                    }
                }
            }
        }
        player++;
    } while (player != &D_801235B0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80088C80.s")

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
            if ((zDiff < xzLimit) && (func_80098C30((s64)xDiff * xDiff + (s64)zDiff * zDiff) < xzLimit)) {
                result = 1;
            }
        }
    }

    return result;
}
#endif

// func_80089000 best match: 99.818% (nonmatchings/func_80089000-6182772958467082306/base_12.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80089000.s")

#ifdef NON_MATCHING
void func_80089000(RaceVec3i *pos, s32 xzSize, s16 flag) {
    volatile u8 pad[16];
    RaceInputPlayer *player;
    RaceInputPlayer *end;
    s32 radius;
    s32 dx;
    s32 dy;
    s32 dz;

    end = &D_801235B0;
    player = D_80121D80;
    do {
        if (player->isActive != 0) {
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
                        (func_80098C30((s64)dx * dx + (s64)dy * dy + (s64)dz * dz) < radius)) {
                        player->unk2C6 |= flag;
                    }
                }
            }
        }
        player++;
    } while (player != end);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_800891B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80089374.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_8008A940.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_8008B408.s")

void func_8008B508(RaceVec3i *vec, RaceInputPlayer *player) {
    s32 magnitude;

    magnitude = func_80098C30((s64)vec->x * vec->x + (s64)vec->z * vec->z);
    if (player->unk314 < magnitude) {
        vec->x = (s64)vec->x * player->unk314 / magnitude;
        vec->z = (s64)vec->z * player->unk314 / magnitude;
    }
}

void func_8008B60C(RaceVec3i *vec, RaceInputPlayer *player) {
    s32 magnitude;

    magnitude = func_80098C30((s64)vec->x * vec->x + (s64)vec->z * vec->z);
    if ((player->unk314 / 2) < magnitude) {
        vec->x = (s64)vec->x * (player->unk314 / 2) / magnitude;
        vec->z = (s64)vec->z * (player->unk314 / 2) / magnitude;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_8008B73C.s")

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
            func_80072AC8(D_800DE84C[(func_800430D0() & 1) + (player->characterId * 2)],
                          (SoundPosition *)&player->posX, 0x7F, 0x5A, (u16)player->playerIndex, 0);
            return;
        case 1:
            func_80072AC8(D_800DE864[(func_800430D0() & 1) + (player->characterId * 2)],
                          (SoundPosition *)&player->posX, 0x7F, 0x5A, (u16)player->playerIndex, 0);
            return;
        case 2:
            func_80072AC8(D_800DE87C[(func_800430D0() & 1) + (player->characterId * 2)],
                          (SoundPosition *)&player->posX, 0x7F, 0x5A, (u16)player->playerIndex, 0);
            return;
        case 3:
            func_80072AC8(D_800DE87C[(func_800430D0() & 1) + (player->characterId * 2)],
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

    func_80097FE4(scratch.rotationMtx, arg0->pitchAngle, arg0->facingAngle, arg0);
    scratch.localPos.x = 0;
    scratch.localPos.y = 0xC0000;
    scratch.localPos.z = 0;
    func_80098590(scratch.rotationMtx, &scratch.localPos, &scratch.worldPos);
    arg0->projectedPos.x = scratch.worldPos.x + arg0->posX;
    arg0->projectedPos.y = scratch.worldPos.y + arg0->posY;
    arg0->projectedPos.z = scratch.worldPos.z + arg0->posZ;
}
