#include "common.h"
#include "game_audio.h"
#include "race_input_history.h"

typedef struct {
    RaceVec3i worldPos;
    RaceVec3i localPos;
    Matrix4s rotationMtx;
} TransformScratch;

extern void func_8008B73C(RaceInputPlayer *, s32, s32, s32, s32, s32);
extern u8 func_800430D0(void);
extern void func_80097FE4(Matrix4s, s16, s16, RaceInputPlayer *);
extern void func_80098590(Matrix4s, RaceVec3i *, RaceVec3i *);
extern s32 func_80098C30(s64);
extern s16 D_800DE84C[][2];
extern s16 D_800DE864[][2];
extern s16 D_800DE87C[][2];
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

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80088A1C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80088C80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_80088E98.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_8008B60C.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_8008BBB8.s")

#ifdef NON_MATCHING
void func_8008BBB8(RaceInputPlayer *player, s32 soundType) {
    s16 playerIndex;
    s16 soundTypeShort = soundType;

    if (player->soundDisabled != 0 || soundTypeShort >= 8U) {
        return;
    }

    playerIndex = player->playerIndex;
    switch (soundTypeShort) {
        case 0:
            func_80072AC8(D_800DE84C[player->characterId][func_800430D0() & 1],
                          (SoundPosition *)&player->posX, 0x7F, 0x5A, playerIndex, 0);
            break;
        case 1:
            func_80072AC8(D_800DE864[player->characterId][func_800430D0() & 1],
                          (SoundPosition *)&player->posX, 0x7F, 0x5A, playerIndex, 0);
            break;
        case 2:
            func_80072AC8(D_800DE87C[player->characterId][func_800430D0() & 1],
                          (SoundPosition *)&player->posX, 0x7F, 0x5A, playerIndex, 0);
            break;
        case 3:
            func_80072AC8(D_800DE87C[player->characterId][func_800430D0() & 1],
                          (SoundPosition *)&player->posX, 0x7F, 0x5A, playerIndex, 0x60);
            break;
        case 4:
            func_80072AC8(D_800DE894[player->characterId], (SoundPosition *)&player->posX, 0x7F, 0x5A, playerIndex, 0);
            break;
        case 5:
            func_80072AC8(D_800DE8A0[player->characterId], (SoundPosition *)&player->posX, 0x7F, 0x5A, playerIndex, 0);
            break;
        case 6:
            func_80072AC8(D_800DE8AC[player->characterId], (SoundPosition *)&player->posX, 0x7F, 0x5A, playerIndex, 0);
            break;
        case 7:
            func_80072AC8(D_800DE8B8[player->characterId], (SoundPosition *)&player->posX, 0x7F, 0x5A, playerIndex, 0);
            break;
    }
}
#endif

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
