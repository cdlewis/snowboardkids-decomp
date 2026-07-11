#ifndef RACE_ITEM_EFFECTS_H
#define RACE_ITEM_EFFECTS_H

#include "common.h"
#include "spatial_math.h"

typedef struct RaceItemEffectActor RaceItemEffectActor;
typedef struct RaceItemFollowActor RaceItemFollowActor;
typedef struct RaceItemTextureActor RaceItemTextureActor;

s32 func_8004DB60(s32 arg0);
void func_8004DB8C(RaceItemEffectActor *arg0);
void func_8004DC6C(RaceItemEffectActor *arg0);
void func_8004DCA0(Vec3i *arg0, Vec3i *arg1, Vec3i *arg2, Vec3i *arg3, s32 arg4, s16 arg5);
void func_8004E02C(RaceItemEffectActor *arg0);
void func_8004E3BC(RaceItemEffectActor *arg0);
void func_8004E438(RaceItemEffectActor *arg0);
void func_8004E518(s16 arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4);
void func_8004E594(s32 arg0, s32 arg1, s32 arg2, s16 arg3);
void func_8004E604(RaceItemEffectActor *arg0);
void func_8004E960(RaceItemEffectActor *arg0);
void func_8004E9D0(RaceItemEffectActor *arg0);
void func_8004EA34(s32 arg0, s32 arg1, s32 arg2, s16 arg3);
void func_8004EAA8(RaceItemEffectActor *arg0);
void func_8004EE0C(RaceItemEffectActor *arg0);
void func_8004EF24(RaceItemEffectActor *arg0);
void func_8004EFF8(RaceItemEffectActor *arg0);
void func_8004F33C(RaceItemEffectActor *arg0);
void func_8004F3FC(RaceItemEffectActor *arg0);
void func_8004F55C(s16 playerIndex, s16 itemIndex);
void func_8004F68C(RaceItemEffectActor *arg0);
void func_8004F9CC(RaceItemEffectActor *arg0);
void func_8004FA44(RaceItemEffectActor *arg0);
void func_8004FB44(RaceItemFollowActor *arg0);
void func_8004FF34(RaceItemFollowActor *arg0);
void func_80050030(RaceItemFollowActor *arg0);
void func_8005019C(RaceItemEffectActor *arg0);
void func_80050340(RaceItemEffectActor *arg0);
void func_80050398(RaceItemEffectActor *arg0);
void func_800503D0(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4);
void func_80050458(RaceItemTextureActor *arg0);
void func_80050888(RaceItemEffectActor *arg0);
void func_800508D0(RaceItemEffectActor *arg0);
void func_8005098C(RaceItemFollowActor *arg0);
void func_80050D84(RaceItemFollowActor *arg0);
void func_80050E80(RaceItemFollowActor *arg0);

#endif
