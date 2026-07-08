#include "common.h"

#define RACE_PLAYER_STATE_SIZE 0x60C

typedef struct {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 z;
} Vec3i;

typedef s16 FixedMatrix3s[10];

typedef struct {
    s32 pad;
    Vec3i transformed;
    Vec3i offset;
} TransformScratch;

typedef struct {
    /* 0x000 */ u8 pad0[0x44];
    /* 0x044 */ s32 unk44;
    /* 0x048 */ u8 pad48[0x94 - 0x48];
    /* 0x094 */ FixedMatrix3s transform;
    union {
        /* 0x0A8 */ Vec3i posA8;
        /* 0x0A8 */ Vec3i velocity;
    };
    /* 0x0B4 */ u8 padB4[0x1C8 - 0xB4];
    /* 0x1C8 */ Vec3i pos;
    /* 0x1D4 */ u8 pad1D4[0x2EC - 0x1D4];
    union {
        /* 0x2EC */ s16 yaw;
        /* 0x2EC */ s16 unk2EC;
    };
    /* 0x2EE */ u8 pad2EE[0x2FC - 0x2EE];
    /* 0x2FC */ s32 flags;
    /* 0x300 */ u8 pad300[0x502 - 0x300];
    /* 0x502 */ s16 surfaceAngle;
    /* 0x504 */ u8 pad504[0x50C - 0x504];
    /* 0x50C */ s16 *unk50C;
    /* 0x510 */ s16 unk510;
    /* 0x512 */ u8 pad512[RACE_PLAYER_STATE_SIZE - 0x512];
} RacePlayerState;

typedef struct {
    /* 0x000 */ s16 surfaceAngle;
    /* 0x002 */ u8 pad2[RACE_PLAYER_STATE_SIZE - 2];
} RacePlayerSurfaceState;

typedef struct {
    /* 0x00 */ s16 value;
    /* 0x02 */ u8 pad2[RACE_PLAYER_STATE_SIZE - 2];
} RacePlayerHalfwordField;

typedef struct {
    /* 0x00 */ u8 value;
    /* 0x01 */ u8 pad1[RACE_PLAYER_STATE_SIZE - 1];
} RacePlayerByteField;

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
    union {
        /* 0x40 */ s16 angle;
        /* 0x40 */ s16 *anglePtr;
    };
    /* 0x44 */ Vec3i prevPos;
    /* 0x50 */ s32 radius;
    /* 0x54 */ s8 unk54;
} RaceEffectActor;

extern s16 D_8011216C;
extern s16 D_80112168;
extern u8 D_80121B56;
extern RacePlayerState D_80121D80[];
extern RacePlayerHalfwordField D_80122052[];
extern RacePlayerSurfaceState D_80122282[];
extern RacePlayerByteField D_8012229A[];
extern s32 D_801248A4;

s32 func_80043040(s16);
s16 func_8004908C(s32, s32);
void func_800483FC(void *, void *, void *);
void func_80045990(s32, s32, void **, void **);
s16 func_80049440(Vec3i *, s32, s16, s16, s16 *);
void func_80049664(RaceEffectActor *);
void func_800499A4(RaceEffectActor *);
void func_80049FB4(RaceEffectActor *);
void func_8004A2F4(RaceEffectActor *);
void func_8004B2B8(RaceEffectActor *);
void func_8004AC5C(RaceEffectActor *);
void func_8004B5F8(RaceEffectActor *);
void func_8004BC74(RaceEffectActor *);
void func_8004C5B4(RaceEffectActor *);
void func_8004CBC4(RaceEffectActor *);
void func_8004CF28(RaceEffectActor *);
void func_8004D018(RaceEffectActor *);
void func_8004D5C0(RaceEffectActor *);
void func_8004E594(s32, s32, s32, s32);
void func_8004EA34(s32, s32, s32, s16);
void func_800716E4(RaceEffectActor *);
void func_80071824(void *task, void (*callback)());
void func_80072A74(s32, void *, s32, s32);
void *func_80071408(void *, s32, s32);
s32 func_80098C30(s64);
s16 func_8007D200(s16, s32, s32);
void func_8007ECF4(s16, s32, s32, s32, s32 *, s32 *, s32 *, s32 *);
void func_8007FF88(s16, s32, s32, s32, s32 *, s32 *);
s32 func_80080CC4(s16, s32, s32);
s32 func_800891B8(Vec3i *, s32, s32, s16);
s16 func_80097AE8(s16);
s16 func_80097B48(s16);
void func_80098590(s16 *, Vec3i *, void *);
s64 __ll_mul(s64, s64);

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_80049440.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_80049664.s")

void func_800499A4(RaceEffectActor *arg0) {
    s32 sin;
    s32 xOffset;
    s32 cos;
    s32 zOffset;
    s32 pushX;
    s32 pushZ;
    s32 prevY;
    s32 y;
    s16 angleDiff;
    s32 groundY;
    Vec3i *pos;
    s32 i;
    volatile u8 padding[0x10];

    if (D_80121B56 == 0) {
        pos = &arg0->pos;
        arg0->spriteIndex = func_80049440(pos, 0x1600000, arg0->targetAngle, arg0->playerIndex, &angleDiff);

        if (arg0->spriteIndex != -1) {
            D_8012229A[arg0->spriteIndex].value = 1;
            angleDiff = (angleDiff - arg0->targetAngle) & 0xFFF;
            if (angleDiff >= 0x801) {
                angleDiff -= 0x1000;
            }

            if (angleDiff >= 0x1D) {
                angleDiff = 0x1C;
            }
            if (angleDiff < -0x1C) {
                angleDiff = -0x1C;
            }

            arg0->targetAngle += angleDiff;
        }

        sin = func_80097AE8(arg0->targetAngle);
        cos = func_80097B48(arg0->targetAngle);
        xOffset = ((s64)sin * arg0->velocityY) / 0x1000;
        zOffset = ((s64)cos * arg0->velocityY) / 0x1000;

        y = arg0->pos.y;
        prevY = y;
        arg0->pos.x += xOffset;
        arg0->pos.y = y;
        arg0->pos.y = arg0->pos.y + arg0->accelerationY;
        arg0->pos.z += zOffset;

        arg0->startAngle = func_8007D200(arg0->startAngle, arg0->pos.x, arg0->pos.z);
        groundY = func_80080CC4(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
        y = arg0->pos.y;
        if (y < groundY) {
            arg0->pos.y = groundY;
            y = groundY;
        }
        arg0->accelerationY = (y - prevY) - 0x20000;

        func_8007FF88(arg0->startAngle, arg0->pos.x, arg0->pos.z, 0x20000, &pushX, &pushZ);
        if (pushX != 0 || pushZ != 0) {
            arg0->timer = 0;
            arg0->pos.x += pushX;
            arg0->pos.z += pushZ;
        }

        for (i = 0; i < 4; i++) {
            if (i != arg0->playerIndex && func_800891B8(pos, 0x30000, 8, i)) {
                D_80122052[i].value = arg0->playerIndex;
                arg0->timer = 0;
                i = 4;
            }
        }

        if (arg0->timer == 0) {
            func_80072A74(0xA, pos, 0x7F, 0x32);
            func_8004E594(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            func_800716E4(arg0);
            return;
        }

        arg0->timer--;
        func_8004EA34(arg0->pos.x, arg0->pos.y, arg0->pos.z, 0);
    }

    func_800483FC(&D_801248A4, func_80049664, arg0);
}

void func_80049CE0(RaceEffectActor *arg0) {
    volatile s32 pad0;
    Vec3i source;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 magnitude;
    s32 var_a0;
    s64 product;

    arg0->timer = 0x12C;
    arg0->spriteIndex = -1;
    arg0->velocityY = 0x130000;
    source.z = 0;
    source.y = 0;
    source.x = 0x1000;
    if (D_80121D80[arg0->playerIndex].flags & 0x400) { source.x = -0x1000; } func_80098590(D_80121D80[arg0->playerIndex].transform, &source, &sp4C); product = __ll_mul((s64) sp4C, (s64) sp4C); magnitude = func_80098C30(product + __ll_mul((s64) sp54, (s64) sp54)); if (magnitude != 0) {
        arg0->accelerationY = (((s64) arg0->velocityY) * sp50) / magnitude;
        var_a0 = -arg0->velocityY;
    } else {
        var_a0 = -arg0->velocityY;
        arg0->accelerationY = var_a0;
    }
    arg0->accelerationY += D_80121D80[arg0->playerIndex].unk44;
    arg0->velocityY = var_a0;
    arg0->targetAngle = D_80121D80[arg0->playerIndex].unk2EC;
    source.z = 0;
    source.y = 0x280000;
    source.x = 0x100000;
    if (D_80121D80[arg0->playerIndex].flags & 0x400) {
        source.x = 0xFFF00000;
        arg0->targetAngle += 0x800;
    }
    func_80098590(D_80121D80[arg0->playerIndex].transform, &source, &arg0->pos.x);
    arg0->pos.x += D_80121D80[arg0->playerIndex].velocity.x;
    arg0->pos.y += D_80121D80[arg0->playerIndex].velocity.y;
    arg0->pos.z += D_80121D80[arg0->playerIndex].velocity.z;
    arg0->startAngle = D_80121D80[arg0->playerIndex].surfaceAngle;
    func_80045990(func_80043040(D_8011216C), 0, &arg0->image, &arg0->palette);
    func_800499A4(arg0);
    func_80071824(arg0, func_800499A4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_80049FB4.s")

// func_8004A2F4 best match: 99.742% (nonmatchings/func_8004A2F4-5635509610426229442/base_15.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004A2F4.s")

#ifdef NON_MATCHING
void func_8004A2F4(RaceEffectActor *arg0) {
    s32 sin;
    s32 cos;
    s32 xOffset;
    s32 zOffset;
    s32 pushX;
    s32 pushZ;
    volatile s32 prevY;
    s32 y;
    s16 angleDiff;
    s32 groundY;
    s32 newX;
    s32 newY;
    s32 newZ;
    Vec3i *pos;
    s32 hitPlayer;
    s32 i;

    if (D_80121B56 == 0) {
        pos = &arg0->pos;
        arg0->spriteIndex = func_80049440(pos, 0xE00000, arg0->targetAngle, arg0->playerIndex, &angleDiff);

        if (arg0->spriteIndex != -1) {
            D_8012229A[arg0->spriteIndex].value = 1;
            angleDiff = (angleDiff - arg0->targetAngle) & 0xFFF;
            if (angleDiff >= 0x801) {
                angleDiff -= 0x1000;
            }

            if (angleDiff >= 0x19) {
                angleDiff = 0x18;
            }
            if (angleDiff < -0x18) {
                angleDiff = -0x18;
            }

            arg0->targetAngle += angleDiff;
        }

        sin = func_80097AE8(arg0->targetAngle);
        cos = func_80097B48(arg0->targetAngle);
        xOffset = ((s64)sin * arg0->velocityY) / 0x1000;
        zOffset = ((s64)cos * arg0->velocityY) / 0x1000;

        prevY = y = arg0->pos.y;
        newX = (unsigned long long)(arg0->pos.x + xOffset);
        newY = y + arg0->accelerationY;
        newZ = arg0->pos.z + zOffset;
        arg0->pos.x = newX;
        arg0->pos.y = newY;
        arg0->pos.z = newZ;

        arg0->startAngle = func_8007D200(arg0->startAngle, newX, newZ);
        groundY = func_80080CC4(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
        y = arg0->pos.y;
        if (y < groundY) {
            arg0->pos.y = groundY;
            y = groundY;
        }
        arg0->accelerationY = (y - prevY) - 0x20000;

        func_8007FF88(arg0->startAngle, arg0->pos.x, arg0->pos.z, 0x20000, &pushX, &pushZ);
        hitPlayer = 0;
        if (pushX != 0 || pushZ != 0) {
            arg0->timer = 0;
            arg0->pos.x += pushX;
            arg0->pos.z += pushZ;
        }

        for (i = 0; i < 4; i++) {
            if (i != arg0->playerIndex && func_800891B8(pos, 0x30000, 0x10, i)) {
                D_80122052[i].value = arg0->playerIndex;
                hitPlayer = 1;
                arg0->timer = 0;
                i = 4;
            }
        }

        if (arg0->timer == 0) {
            if (hitPlayer == 0) {
                func_80072A74(0xA, pos, 0x7F, 0x32);
            }
            func_8004E594(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            func_800716E4(arg0);
            return;
        }

        arg0->timer--;
        func_8004EA34(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
    }

    func_800483FC(&D_801248A4, func_80049FB4, arg0);
}
#endif

void func_8004A648(RaceEffectActor *arg0) {
    volatile s32 pad0;
    Vec3i source;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 magnitude;
    s32 var_a0;
    s64 product;

    arg0->timer = 0x12C;
    arg0->spriteIndex = -1;
    arg0->velocityY = 0x150000;
    source.z = 0;
    source.y = 0;
    source.x = 0x1000;
    if (D_80121D80[arg0->playerIndex].flags & 0x400) { source.x = -0x1000; } func_80098590(D_80121D80[arg0->playerIndex].transform, &source, &sp4C); product = __ll_mul((s64) sp4C, (s64) sp4C); magnitude = func_80098C30(product + __ll_mul((s64) sp54, (s64) sp54)); if (magnitude != 0) {
        arg0->accelerationY = (((s64) arg0->velocityY) * sp50) / magnitude;
        var_a0 = -arg0->velocityY;
    } else {
        var_a0 = -arg0->velocityY;
        arg0->accelerationY = var_a0;
    }
    arg0->accelerationY += D_80121D80[arg0->playerIndex].unk44;
    arg0->velocityY = var_a0;
    arg0->targetAngle = D_80121D80[arg0->playerIndex].unk2EC;
    source.z = 0;
    source.y = 0x280000;
    source.x = 0x100000;
    if (D_80121D80[arg0->playerIndex].flags & 0x400) {
        source.x = 0xFFF00000;
        arg0->targetAngle += 0x800;
    }
    func_80098590(D_80121D80[arg0->playerIndex].transform, &source, &arg0->pos.x);
    arg0->pos.x += D_80121D80[arg0->playerIndex].velocity.x;
    arg0->pos.y += D_80121D80[arg0->playerIndex].velocity.y;
    arg0->pos.z += D_80121D80[arg0->playerIndex].velocity.z;
    arg0->startAngle = D_80121D80[arg0->playerIndex].surfaceAngle;
    func_80045990(func_80043040(D_8011216C), 2, &arg0->image, &arg0->palette);
    func_8004A2F4(arg0);
    func_80071824(arg0, func_8004A2F4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004A91C.s")

// func_8004AC5C best match: 99.779% (nonmatchings/func_8004AC5C-1315772375853892447/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004AC5C.s")

#ifdef NON_MATCHING
void func_8004AC5C(RaceEffectActor *arg0) {
    s32 sin;
    s32 xOffset;
    s32 cos;
    s32 zOffset;
    s32 pushX;
    s32 pushZ;
    s32 prevY;
    s32 y;
    s16 angleDiff;
    s32 groundY;
    Vec3i *pos;
    s32 i;
    volatile u8 padding[0x10];

    if (D_80121B56 == 0) {
        pos = &arg0->pos;
        arg0->spriteIndex = func_80049440(pos, 0x600000, arg0->targetAngle, arg0->playerIndex, &angleDiff);

        if (arg0->spriteIndex != -1) {
            D_8012229A[arg0->spriteIndex].value = 1;
            angleDiff = (angleDiff - arg0->targetAngle) & 0xFFF;
            if (angleDiff >= 0x801) {
                angleDiff -= 0x1000;
            }

            if (angleDiff >= 0x1D) {
                angleDiff = 0x1C;
            }
            if (angleDiff < -0x1C) {
                angleDiff = -0x1C;
            }

            arg0->targetAngle += angleDiff;
        }

        sin = func_80097AE8(arg0->targetAngle);
        cos = func_80097B48(arg0->targetAngle);
        xOffset = ((s64) sin * arg0->velocityY) / 0x1000;
        zOffset = ((s64) cos * arg0->velocityY) / 0x1000;

        prevY = arg0->pos.y;
        arg0->pos.x += xOffset;
        arg0->pos.y = prevY + arg0->accelerationY;
        arg0->pos.z += zOffset;

        arg0->startAngle = func_8007D200(arg0->startAngle, arg0->pos.x, arg0->pos.z);
        groundY = func_80080CC4(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
        y = arg0->pos.y;
        if (y < groundY) {
            arg0->pos.y = groundY;
            y = groundY;
        }
        arg0->accelerationY = (y - prevY) - 0x20000;

        func_8007ECF4(arg0->startAngle, arg0->pos.x, arg0->pos.z, 0x20000, &pushX, &pushZ, &xOffset, &zOffset);
        if (pushX != 0 || pushZ != 0) {
            arg0->accelerationY = 0;
            arg0->pos.x += pushX;
            arg0->pos.z += pushZ;
            arg0->targetAngle = func_8004908C(xOffset, zOffset);
            func_80072A74(0x11, pos, 0x7F, 0x32);
        }

        for (i = 0; i < 4; i++) {
            if ((i != arg0->playerIndex || arg0->timer < 0x4B) && func_800891B8(pos, 0x30000, 0x40, i)) {
                D_80122052[i].value = arg0->playerIndex;
                arg0->timer = 0;
                i = 4;
            }
        }

        if (arg0->timer == 0) {
            func_80072A74(0xA, pos, 0x7F, 0x32);
            func_8004E594(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            func_800716E4(arg0);
            return;
        }

        arg0->timer--;
        func_8004EA34(arg0->pos.x, arg0->pos.y, arg0->pos.z, 3);
    }

    func_800483FC(&D_801248A4, func_8004A91C, arg0);
}
#endif

void func_8004AFE4(RaceEffectActor *arg0) {
    volatile s32 pad0;
    Vec3i source;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 magnitude;
    s32 var_a0;
    s64 product;

    arg0->timer = 0xB4;
    arg0->spriteIndex = -1;
    arg0->velocityY = 0x170000;
    source.z = 0;
    source.y = 0;
    source.x = 0x1000;
    if (D_80121D80[arg0->playerIndex].flags & 0x400) { source.x = -0x1000; } func_80098590(D_80121D80[arg0->playerIndex].transform, &source, &sp4C); product = __ll_mul((s64) sp4C, (s64) sp4C); magnitude = func_80098C30(product + __ll_mul((s64) sp54, (s64) sp54)); if (magnitude != 0) {
        arg0->accelerationY = (((s64) arg0->velocityY) * sp50) / magnitude;
        var_a0 = -arg0->velocityY;
    } else {
        var_a0 = -arg0->velocityY;
        arg0->accelerationY = var_a0;
    }
    arg0->accelerationY += D_80121D80[arg0->playerIndex].unk44;
    arg0->velocityY = var_a0;
    arg0->targetAngle = D_80121D80[arg0->playerIndex].unk2EC;
    source.z = 0;
    source.y = 0x280000;
    source.x = 0x100000;
    if (D_80121D80[arg0->playerIndex].flags & 0x400) {
        source.x = 0xFFF00000;
        arg0->targetAngle += 0x800;
    }
    func_80098590(D_80121D80[arg0->playerIndex].transform, &source, &arg0->pos.x);
    arg0->pos.x += D_80121D80[arg0->playerIndex].velocity.x;
    arg0->pos.y += D_80121D80[arg0->playerIndex].velocity.y;
    arg0->pos.z += D_80121D80[arg0->playerIndex].velocity.z;
    arg0->startAngle = D_80121D80[arg0->playerIndex].surfaceAngle;
    func_80045990(func_80043040(D_8011216C), 3, &arg0->image, &arg0->palette);
    func_8004AC5C(arg0);
    func_80071824(arg0, func_8004AC5C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004B2B8.s")

void func_8004B5F8(RaceEffectActor *arg0) {
    s32 sin;
    s32 cos;
    s32 xOffset;
    s32 zOffset;
    s32 pushX;
    s32 pushZ;
    volatile u8 padding[8];
    s32 prevY;
    s32 y;
    s32 groundY;
    Vec3i *pos;
    s32 i;

    if (D_80121B56 == 0) {
        sin = func_80097AE8(arg0->targetAngle);
        cos = func_80097B48(arg0->targetAngle);
        xOffset = ((s64)sin * arg0->velocityY) / 0x1000;
        zOffset = ((s64)cos * arg0->velocityY) / 0x1000;

        prevY = arg0->pos.y;
        arg0->pos.x += xOffset;
        arg0->pos.y = arg0->pos.y + arg0->accelerationY;
        arg0->pos.z += zOffset;

        arg0->startAngle = func_8007D200(arg0->startAngle, arg0->pos.x, arg0->pos.z);
        groundY = func_80080CC4(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
        y = arg0->pos.y;
        if (y < groundY) {
            arg0->pos.y = groundY;
            y = groundY;
        }
        arg0->accelerationY = (y - prevY) - 0x20000;

        func_8007FF88(arg0->startAngle, arg0->pos.x, arg0->pos.z, 0x20000, &pushX, &pushZ);
        pos = &arg0->pos;
        if (pushX != 0 || pushZ != 0) {
            arg0->timer = 0;
            arg0->pos.x += pushX;
            arg0->pos.z += pushZ;
        }

        for (i = 0; i < 4; i++) {
            if (func_800891B8(pos, 0x30000, 0x2000, i)) {
                func_80072A74(0xA, pos, 0x7F, 0x32);
                func_8004E594(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
                arg0->timer = 0;
                i = 4;
            }
        }

        if (arg0->timer == 0) {
            func_800716E4(arg0);
            return;
        }

        arg0->timer--;
        func_8004EA34(arg0->pos.x, arg0->pos.y, arg0->pos.z, 3);
    }

    func_800483FC(&D_801248A4, func_8004B2B8, arg0);
}

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

void func_8004BFA0(RaceEffectActor *arg0) {
    volatile s32 pad0;
    Vec3i sp58;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 magnitude;
    s32 velocityY;
    RaceEffectActor *actor;
    s64 product;

    arg0->timer = 0x12C;
    arg0->spriteIndex = -1;
    arg0->velocityY = 0x110000;
    sp58.z = (sp58.y = 0);
    sp58.x = 0x1000;

    if (D_80121D80[arg0->playerIndex].flags & 0x400) {
        sp58.x = -0x1000;
        if (1) {
        }
    }

    actor = arg0; func_80098590(D_80121D80[arg0->playerIndex].transform, &sp58, &sp4C); product = __ll_mul((s64) sp4C, (s64) sp4C); magnitude = func_80098C30(product + __ll_mul((s64) sp54, (s64) sp54)); if (magnitude != 0) { actor->accelerationY = (s64)actor->velocityY * sp50 / magnitude; velocityY = -actor->velocityY; } else { velocityY = -actor->velocityY; actor->accelerationY = velocityY; } actor->accelerationY += D_80121D80[actor->playerIndex].unk44; actor->velocityY = velocityY; actor->targetAngle = D_80121D80[actor->playerIndex].yaw; sp58.z = 0; sp58.x = 0xFFF00000;
    sp58.y = 0x280000;
    sp58.x = 0x100000;

    if (D_80121D80[actor->playerIndex].flags & 0x400) {
        sp58.x = 0xFFF00000;
        actor->targetAngle += 0x800;
    }

    func_80098590(D_80121D80[actor->playerIndex].transform, &sp58, &actor->pos);
    actor->pos.x += ((0, D_80121D80))[actor->playerIndex].posA8.x;
    actor->pos.y += D_80121D80[actor->playerIndex].posA8.y;
    actor->pos.z += D_80121D80[actor->playerIndex].posA8.z;
    actor->startAngle = D_80121D80[actor->playerIndex].surfaceAngle;
    func_80045990(func_80043040(D_8011216C), 4, &actor->image, &actor->palette);
    func_8004BC74(actor);
    func_80071824(actor, func_8004BC74);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004C274.s")

// func_8004C5B4 best match: 99.879% (nonmatchings/func_8004C5B4-5635509610426229442/base_20.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004C5B4.s")

#ifdef NON_MATCHING
void func_8004C5B4(RaceEffectActor *arg0) {
    s32 sin;
    s32 cos;
    s32 xOffset;
    s32 zOffset;
    s32 pushX;
    s32 pushZ;
    volatile s32 prevY;
    s32 y;
    s16 angleDiff;
    s32 groundY;
    volatile u8 padding[4];
    Vec3i *pos;
    s32 i;

    if (D_80121B56 == 0) {
        pos = &arg0->pos;
        arg0->spriteIndex = func_80049440(pos, 0x1200000, arg0->targetAngle, arg0->playerIndex, &angleDiff);

        if (arg0->spriteIndex != -1) {
            D_8012229A[arg0->spriteIndex].value = 1;
            angleDiff = (angleDiff - arg0->targetAngle) & 0xFFF;
            if (angleDiff >= 0x801) {
                angleDiff -= 0x1000;
            }

            if (angleDiff >= 0x13) {
                angleDiff = 0x12;
            }
            if (angleDiff < -0x12) {
                angleDiff = -0x12;
            }

            arg0->targetAngle += angleDiff;
        }

        sin = func_80097AE8(arg0->targetAngle);
        cos = func_80097B48(arg0->targetAngle);
        xOffset = ((s64)sin * arg0->velocityY) / 0x1000;
        zOffset = ((s64)cos * arg0->velocityY) / 0x1000;

        arg0->pos.y = (prevY = arg0->pos.y);
        arg0->pos.x += xOffset;
        arg0->pos.y = arg0->pos.y + arg0->accelerationY;
        arg0->pos.z += zOffset;

        arg0->startAngle = func_8007D200(arg0->startAngle, arg0->pos.x, arg0->pos.z);
        groundY = func_80080CC4(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
        y = arg0->pos.y;
        if (y < groundY) {
            arg0->pos.y = groundY;
            y = groundY;
        }
        arg0->accelerationY = (y - prevY) - 0x20000;

        func_8007FF88(arg0->startAngle, arg0->pos.x, arg0->pos.z, 0x20000, &pushX, &pushZ);
        if (pushX != 0 || pushZ != 0) {
            arg0->timer = 0;
            arg0->pos.x += pushX;
            arg0->pos.z += pushZ;
        }

        for (i = 0; i < 4; i++) {
            if ((i != arg0->playerIndex) && func_800891B8(pos, 0x30000, 0x100, i)) {
                D_80122052[i].value = arg0->playerIndex;
                arg0->timer = 0;
                i = 4;
            }
        }

        if (arg0->timer == 0) {
            func_80072A74(0xA, pos, 0x7F, 0x32);
            func_8004E594(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            func_800716E4(arg0);
            return;
        }

        arg0->timer--;
        func_8004EA34(arg0->pos.x, arg0->pos.y, arg0->pos.z, 1);
    }

    func_800483FC(&D_801248A4, func_8004C274, arg0);
}
#endif

// func_8004C8F0 best match: 99.890%
#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004C8F0.s")

#ifdef NON_MATCHING
void func_8004C8F0(RaceEffectActor *arg0) {
    Vec3i *new_var;
    Vec3i sp58;
    Vec3i sp4C;
    s32 magnitude;
    s32 velocityY;
    RaceEffectActor *actor;

    arg0->timer = 0x12C;
    arg0->spriteIndex = -1;
    arg0->velocityY = 0x130000;
    sp58.z = (sp58.y = 0);
    sp58.x = 0x1000;

    if (D_80121D80[arg0->playerIndex].flags & 0x400) {
        sp58.x = -0x1000;
    }

    actor = arg0;
    func_80098590(D_80121D80[arg0->playerIndex].transform, &sp58, &sp4C);
    magnitude = func_80098C30((s64)sp4C.x * sp4C.x + (s64)sp4C.z * sp4C.z);

    if (magnitude != 0) {
        actor->accelerationY = (s64)actor->velocityY * sp4C.y / magnitude;
        velocityY = -actor->velocityY;
    } else {
        velocityY = -actor->velocityY;
        actor->accelerationY = velocityY;
    }

    actor->accelerationY += D_80121D80[actor->playerIndex].unk44;
    actor->velocityY = velocityY;
    actor->targetAngle = D_80121D80[actor->playerIndex].yaw;

    sp58.z = D_80121D80[actor->playerIndex].yaw * 0;
    new_var = &sp58;
    sp58.y = 0x280000;
    sp58.x = 0x100000;

    if (D_80121D80[actor->playerIndex].flags & 0x400) {
        sp58.x = 0xFFF00000;
        actor->targetAngle += 0x800;
    }

    func_80098590(D_80121D80[actor->playerIndex].transform, new_var, &actor->pos);
    actor->pos.x += D_80121D80[actor->playerIndex].posA8.x;
    actor->pos.y += D_80121D80[actor->playerIndex].posA8.y;
    actor->pos.z += D_80121D80[actor->playerIndex].posA8.z;
    actor->startAngle = D_80121D80[actor->playerIndex].surfaceAngle;
    func_80045990(func_80043040(D_8011216C), 1, &actor->image, &actor->palette);
    func_8004C5B4(actor);
    func_80071824(actor, func_8004C5B4);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004CBC4.s")

void func_8004CF28(RaceEffectActor *arg0) {
    Vec3i *pos;
    RaceEffectActor *actor;
    s32 i;
    s32 radius;
    s16 spriteIndex;

    spriteIndex = arg0->spriteIndex;
    actor = arg0;
    if (spriteIndex != 0) {
        actor->spriteIndex = spriteIndex - 1;
    } else {
        func_800716E4(arg0);
        return;
    }

    i = 0;
    pos = &actor->pos;
    radius = 0x30000;
    do {
        if (func_800891B8(pos, radius, 0x400, i)) {
            func_80072A74(0x14, pos, 0x7F, 0x32);
            func_8004E594(arg0->pos.x, actor->pos.y, actor->pos.z, 1);
            func_800716E4(arg0);
            return;
        }
        i++;
    } while (i != 4);

    func_800483FC(&D_801248A4, func_8004CBC4, actor);
}

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

// func_8004D5C0 best match: 89.875%
#pragma GLOBAL_ASM("asm/nonmatchings/race_effects/func_8004D5C0.s")

#ifdef NON_MATCHING
void func_8004D5C0(RaceEffectActor *arg0) {
    s32 sin;
    s32 cos;
    s32 xOffset;
    s32 pushX;
    s32 pushZ;
    s32 prevY;
    s32 y;
    s32 groundY;
    volatile s32 pad[4];
    RacePlayerState *player;

    if (D_80121B56 == 0) {
        if (arg0->unk54 == 0) {
            sin = func_80097AE8(arg0->targetAngle);
            cos = func_80097B48(arg0->targetAngle);
            xOffset = ((s64)sin * arg0->velocityY) / 0x1000;
            cos = ((s64)cos * arg0->velocityY) / 0x1000;
            prevY = arg0->pos.y;
            arg0->pos.x += xOffset;
            arg0->pos.y = prevY + arg0->accelerationY;
            arg0->pos.z += cos;

            arg0->startAngle = func_8007D200(arg0->startAngle, arg0->pos.x, arg0->pos.z);
            groundY = func_80080CC4(arg0->startAngle, arg0->pos.x, arg0->pos.z) + 0xA0000;
            y = arg0->pos.y;
            if (y < groundY) {
                arg0->pos.y = groundY;
                y = groundY;
            }
            arg0->accelerationY = (y - prevY) - 0x20000;

            func_8007FF88(arg0->startAngle, arg0->pos.x, arg0->pos.z, 0x20000, &pushX, &pushZ);
            if (pushX != 0 || pushZ != 0) {
                arg0->timer = 0;
                arg0->pos.x += pushX;
                arg0->pos.z += pushZ;
                func_80072A74(0xA, &arg0->pos, 0x7F, 0x32);
            }
        } else {
            arg0->timer = 0;
            func_80072A74(0x11, &arg0->pos, 0x7F, 0x32);
        }

        if (arg0->timer == 0) {
            func_8004E594(arg0->pos.x, arg0->pos.y, arg0->pos.z, 2);
            func_800716E4((RaceEffectActor *)arg0);
            player = &D_80121D80[arg0->playerIndex];
            player->unk510++;
            return;
        }

        arg0->timer--;
        func_8004EA34(arg0->pos.x, arg0->pos.y, arg0->pos.z, 5);
        arg0->prevPos.x = arg0->pos.x;
        arg0->prevPos.y = arg0->pos.y;
        arg0->prevPos.z = arg0->pos.z;
        arg0->radius = 0x30000;
        player = &D_80121D80[arg0->playerIndex];
        arg0->anglePtr = player->unk50C;
        player->unk50C = (s16 *)&arg0->anglePtr;
    }

    func_800483FC(&D_801248A4, func_8004D280, arg0);
}
#endif

void func_8004D880(RaceEffectActor *arg0) {
    volatile s32 pad;
    Vec3i source;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 magnitude;
    s32 newVelocity;
    s64 product;

    arg0->timer = 0x12C;
    arg0->spriteIndex = -1;
    arg0->velocityY = 0x120000;

    source.z = 0;
    source.y = 0;
    source.x = 0x1000;
    if (D_80121D80[arg0->playerIndex].flags & 0x400) { source.x = -0x1000; } func_80098590(D_80121D80[arg0->playerIndex].transform, &source, &sp4C); product = __ll_mul((s64) sp4C, (s64) sp4C); magnitude = func_80098C30(product + __ll_mul((s64) sp54, (s64) sp54)); if (magnitude != 0) {
        arg0->accelerationY = (s64)arg0->velocityY * sp50 / magnitude;
        newVelocity = -arg0->velocityY;
    } else {
        newVelocity = -arg0->velocityY;
        arg0->accelerationY = newVelocity;
    }

    arg0->accelerationY += D_80121D80[arg0->playerIndex].unk44;
    arg0->velocityY = newVelocity;
    arg0->targetAngle = D_80121D80[arg0->playerIndex].yaw;

    source.z = 0;
    source.y = 0x280000;
    source.x = 0x100000;

    if (D_80121D80[arg0->playerIndex].flags & 0x400) {
        source.x = -0x100000;
        arg0->targetAngle += 0x800;
    }

    func_80098590(D_80121D80[arg0->playerIndex].transform, &source, &arg0->pos);

    arg0->pos.x += D_80121D80[arg0->playerIndex].posA8.x;
    arg0->pos.y += D_80121D80[arg0->playerIndex].posA8.y;
    arg0->pos.z += D_80121D80[arg0->playerIndex].posA8.z;
    arg0->startAngle = D_80121D80[arg0->playerIndex].surfaceAngle;
    func_80045990(func_80043040(D_8011216C), 5, &arg0->image, &arg0->palette);
    arg0->unk54 = 0;
    func_8004D5C0(arg0);
    func_80071824(arg0, func_8004D5C0);
}
