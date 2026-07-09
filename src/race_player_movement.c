#include "common.h"
#include "asset_decompression.h"
#include "game_audio.h"
#include "race_input_history.h"

typedef struct {
    RaceVec3i worldPos;
    RaceVec3i localPos;
    Matrix4s rotationMtx;
} TransformScratch;

extern void func_8008B73C(RaceInputPlayer *, s32, s32, s32, s32, s32);
extern s32 func_8004940C(s32, s32, s32, s32);
extern void func_80097FE4(Matrix4s, s16, s16, RaceInputPlayer *);
extern void func_80098590(Matrix4s, RaceVec3i *, RaceVec3i *);
extern s16 func_80097AE8(s16);
extern s16 func_80097B48(s16);
extern s32 func_80098C30(s64);
extern s16 D_800DE84C[];
extern s16 D_800DE864[];
extern s16 D_800DE87C[];
extern s16 D_800DE894[];
extern s16 D_800DE8A0[];
extern s16 D_800DE8AC[];
extern s16 D_800DE8B8[];

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80087600.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80087AFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80087E14.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80087EFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80088294.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80088664.s")

// func_80088A1C best match: 98.333% (nonmatchings/func_80088A1C-6182772958467082306/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80088A1C.s")

#ifdef NON_MATCHING
void func_80088A1C(RaceVec3i *pos, s32 xzSize, s32 ySize, s32 arg3, s16 arg4) {
    volatile u8 pad[8];
    RaceInputPlayer *player;
    s32 yDiff;
    s32 magnitude;
    s32 xDiff;
    s32 zDiff;
    s32 yLimit;
    s32 xzLimit;
    s16 angle;
    s32 sine;
    s32 cosine;
    s32 overlap;
    s32 pushX;
    s32 pushZ;

    player = D_80121D80;
    do {
        if (player->isActive != 0) {
            yLimit = ySize;
            yDiff = pos->y - player->unk5C;
            if (yDiff < 0) {
                yDiff = -yDiff;
            } else {
                yLimit = player->unk284;
            }

            if (yDiff <= yLimit) {
                xDiff = pos->x - player->posX;
                xzLimit = player->unk280 + xzSize;
                if (xDiff < 0) {
                    xDiff = -xDiff;
                }
                if (xDiff < xzLimit) {
                    zDiff = pos->z - player->posZ;
                    if (zDiff < 0) {
                        zDiff = -zDiff;
                    }
                    if ((zDiff < xzLimit) &&
                        ((magnitude = func_80098C30((s64)xDiff * xDiff + (s64)zDiff * zDiff)) < xzLimit)) {
                        if (player->unk29C < arg3) {
                            angle = func_8004940C(pos->x, pos->z, player->posX, player->posZ);
                            sine = func_80097AE8(angle);
                            cosine = func_80097B48(angle);
                            overlap = xzLimit - magnitude;
                            pushX = (s64)-sine * -overlap / 0x1000;
                            pushZ = (s64)cosine * -overlap / 0x1000;
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
#endif

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

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80089000.s")

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
