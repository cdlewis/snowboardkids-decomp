#include "common.h"

#define RACE_PLAYER_STATE_SIZE 0x60C

typedef struct {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 z;
} Vec3i;

typedef struct {
    /* 0x000 */ u8 pad0[0x1C8];
    /* 0x1C8 */ Vec3i pos;
    /* 0x1D4 */ u8 pad1D4[0x502 - 0x1D4];
    /* 0x502 */ s16 surfaceAngle;
    /* 0x504 */ u8 pad504[RACE_PLAYER_STATE_SIZE - 0x504];
} RacePlayerState;

typedef struct {
    /* 0x000 */ s16 surfaceAngle;
    /* 0x002 */ u8 pad2[RACE_PLAYER_STATE_SIZE - 2];
} RacePlayerSurfaceState;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 playerIndex;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s32 velocityY;
    /* 0x2C */ s32 accelerationY;
    /* 0x30 */ void *image;
    /* 0x34 */ void *palette;
    /* 0x38 */ s16 timer;
    /* 0x3A */ s16 spriteIndex;
    /* 0x3C */ s16 targetAngle;
    /* 0x3E */ s16 startAngle;
    /* 0x40 */ s16 angle;
} RaceEffectActor;

extern s16 D_8011216C;
extern s16 D_80112168;
extern u8 D_80121B56;
extern RacePlayerState D_80121D80[];
extern RacePlayerSurfaceState D_80122282[];
extern s32 D_801248A4;

s32 func_80043040(s16);
void func_800483FC(void *, void *, void *);
void func_80045990(s32, s32, void **, void **);
void func_8004B5F8(RaceEffectActor *);
void func_8004CBC4(RaceEffectActor *);
void func_8004CF28(RaceEffectActor *);
void func_8004D018(RaceEffectActor *);
void func_8004E594(s32, s32, s32, s32);
void func_800716E4(RaceEffectActor *);
void func_80071824(void *task, void (*callback)());
void func_80072A74(s32, void *, s32, s32);
void *func_80071408(void *, s32, s32);
s16 func_8007D200(s16, s32, s32);
s32 func_80080CC4(s16, s32, s32);
s32 func_800891B8(Vec3i *, s32, s32, s16);

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_80049440.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_80049664.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_800499A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_80049CE0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_80049FB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004A2F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004A648.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004A91C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004AC5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004AFE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004B2B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004B5F8.s")

void func_8004B83C(RaceEffectActor *arg0) {
    arg0->timer = 0x3C;
    arg0->spriteIndex = -1;
    arg0->velocityY = 0xFFF00000;
    arg0->accelerationY = 0;
    func_80045990(func_80043040(D_8011216C), 3, &arg0->image, &arg0->palette);
    func_8004B5F8(arg0);
    func_80071824(arg0, func_8004B5F8);
}

void func_8004B8B4(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4) {
    RaceEffectActor *obj = func_80071408(func_8004B83C, 0, 0x1E);

    if (obj != NULL) {
        obj->pos.x = arg0;
        obj->pos.y = arg1;
        obj->pos.z = arg2;
        obj->targetAngle = arg4;
        obj->startAngle = arg3;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004B934.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004BC74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004BFA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004C274.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004C5B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004C8F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004CBC4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004CF28.s")

void func_8004D018(RaceEffectActor *arg0) {
    Vec3i *pos;
    s32 i;
    s32 accelerationY;
    s32 groundY;

    if (D_80121B56 == 0) {
        accelerationY = arg0->accelerationY;
        arg0->pos.y += accelerationY;
        arg0->accelerationY = accelerationY - 0x6000;

        arg0->angle = func_8007D200(arg0->angle, arg0->pos.x, arg0->pos.z);
        groundY = func_80080CC4(arg0->angle, arg0->pos.x, arg0->pos.z);
        if (arg0->pos.y < groundY + 0x30000) {
            arg0->pos.y = groundY + 0x30000;
            func_80071824(arg0, func_8004CF28);
        }

        for (i = 0; i != 4; i++) {
            pos = &arg0->pos;
            if ((i != arg0->playerIndex || arg0->timer == 0) && func_800891B8(pos, 0x30000, 0x400, i)) {
                func_80072A74(0x14, pos, 0x7F, 0x32);
                func_8004E594(arg0->pos.x, arg0->pos.y, arg0->pos.z, 1);
                func_800716E4(arg0);
                return;
            }
        }

        if (arg0->timer != 0) {
            arg0->timer--;
        }
    }

    func_800483FC(&D_801248A4, func_8004CBC4, arg0);
}

void func_8004D184(RaceEffectActor *arg0) {
    RacePlayerState *player;

    arg0->timer = 0x3C;
    arg0->spriteIndex = 0x708;
    arg0->accelerationY = 0x30000;
    player = &D_80121D80[arg0->playerIndex];
    arg0->pos.x = player->pos.x;
    arg0->pos.y = player->pos.y;
    arg0->pos.z = player->pos.z;
    func_80072A74(0x6A, &arg0->pos, 0x7F, 0x32);
    arg0->angle = D_80122282[arg0->playerIndex].surfaceAngle;
    func_80045990(func_80043040(D_80112168), 2, &arg0->image, &arg0->palette);
    func_8004D018(arg0);
    func_80071824(arg0, func_8004D018);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004D280.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004D5C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004D880.s")
