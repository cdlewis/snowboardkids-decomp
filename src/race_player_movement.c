#include "common.h"
#include "race_input_history.h"

typedef struct {
    RaceVec3i worldPos;
    RaceVec3i localPos;
    Matrix4s rotationMtx;
} TransformScratch;

extern void func_8008B73C(RaceInputPlayer *, s32, s32, s32, s32, s32);
extern void func_80097FE4(Matrix4s, s16, s16, RaceInputPlayer *);
extern void func_80098590(Matrix4s, RaceVec3i *, RaceVec3i *);

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

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_movement/func_8008B508.s")

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
