#include "common.h"

#define RACE_ACTOR_COLLISION_YAW_FLIPPED 0x400
#define RACE_ACTOR_COLLISION_ACTIVE 0x800
#define RACE_ACTOR_COLLISION_ALWAYS_VALID 0x400000

#define RACE_ACTOR_COLLISION_ANGLE_MASK 0xFFF
#define RACE_ACTOR_COLLISION_HALF_TURN 0x800

typedef struct {
    char pad0[0x1C];
    s32 unk1C;
    char pad20[4];
    s32 unk24;
    char pad28[0x2AC];
    s16 unk2D4;
    char pad2D6[0x16];
    s16 yaw;
    char pad2EE[0xE];
    s32 collisionFlags;
    char pad300[0x20];
    s16 unk320;
    char pad322[0x1E0];
    s16 unk502;
} RaceActorCollision;

extern s32 D_80121D50;
extern s32 D_80121D54;
extern s32 D_80121D58;

s32 func_80081124(s16, s32, s32, RaceActorCollision *);

s32 func_80084F50(RaceActorCollision *arg0) {
    s32 temp_v0;
    s32 var_v1;
    s32 temp_t0;

    temp_v0 = arg0->collisionFlags;
    if (temp_v0 & 0x379000) {
        return 1;
    }
    if (!(temp_v0 & RACE_ACTOR_COLLISION_ALWAYS_VALID)) {
        var_v1 = D_80121D54 - arg0->yaw;
        var_v1 += RACE_ACTOR_COLLISION_HALF_TURN;
        temp_t0 = var_v1 & RACE_ACTOR_COLLISION_ANGLE_MASK;
        var_v1 = (s16) temp_t0;
        if (temp_v0 & RACE_ACTOR_COLLISION_YAW_FLIPPED) {
            var_v1 = (s16) (var_v1 + RACE_ACTOR_COLLISION_HALF_TURN);
        }
        var_v1 += 0x200;
        var_v1 = (s16) (var_v1 & RACE_ACTOR_COLLISION_ANGLE_MASK);
        if ((D_80121D58 < 0x30000) || (var_v1 >= 0x401)) {
            return 1;
        }
    }
    if (D_80121D50 < 5) {
        D_80121D50 = 5;
    }
    return 0;
}

s32 func_80085010(RaceActorCollision *arg0) {
    s32 temp_v0;
    s32 var_v1;
    s32 temp_t0;

    temp_v0 = arg0->collisionFlags;
    if (temp_v0 & 0x379000) {
        return 1;
    }
    if (!(temp_v0 & RACE_ACTOR_COLLISION_ALWAYS_VALID)) {
        var_v1 = D_80121D54 - arg0->yaw;
        var_v1 += RACE_ACTOR_COLLISION_HALF_TURN;
        temp_t0 = var_v1 & RACE_ACTOR_COLLISION_ANGLE_MASK;
        var_v1 = (s16) temp_t0;
        if (temp_v0 & RACE_ACTOR_COLLISION_YAW_FLIPPED) {
            var_v1 = (s16) (var_v1 + RACE_ACTOR_COLLISION_HALF_TURN);
        }
        var_v1 += 0x200;
        var_v1 = (s16) (var_v1 & RACE_ACTOR_COLLISION_ANGLE_MASK);
        if ((D_80121D58 < 0x30000) || (var_v1 >= 0x401)) {
            return 1;
        }
    }
    if (D_80121D50 < 6) {
        D_80121D50 = 6;
    }
    return 0;
}

s32 func_800850D0(RaceActorCollision *arg0) {
    if (arg0->collisionFlags & 0x37D000) {
        return 1;
    }
    if (D_80121D50 < 7) {
        D_80121D50 = 7;
    }
    return 0;
}

s32 func_80085118(RaceActorCollision *arg0) {
    if (arg0->collisionFlags & 0x77D000) {
        return 1;
    }
    if (D_80121D50 < 0xB) {
        D_80121D50 = 0xB;
    }
    return 0;
}

s32 func_80085160(RaceActorCollision *arg0) {
    if (arg0->collisionFlags & 0x77D000) {
        return 1;
    }
    if (D_80121D50 < 0xA) {
        D_80121D50 = 0xA;
    }
    return 0;
}

s32 func_800851A8(RaceActorCollision *arg0) {
    if (arg0->collisionFlags & 0x77D000) {
        return 1;
    }
    if (D_80121D50 < 0xC) {
        D_80121D50 = 0xC;
    }
    return 0;
}

s32 func_800851F0(RaceActorCollision *arg0) {
    if (arg0->collisionFlags & 0x77D000) {
        return 1;
    }
    if (D_80121D50 < 0xD) {
        D_80121D50 = 0xD;
    }
    return 0;
}

s32 func_80085238(RaceActorCollision *arg0) {
    s32 temp_v0 = arg0->collisionFlags;

    if (temp_v0 & 0x43000) {
        return 1;
    }
    if (!(temp_v0 & RACE_ACTOR_COLLISION_ACTIVE)) {
        return 1;
    }
    if (D_80121D50 < 8) {
        D_80121D50 = 8;
    }
    return 0;
}

s32 func_80085290(RaceActorCollision *arg0) {
    s32 temp_v0 = arg0->collisionFlags;

    if (temp_v0 & 0x43000) {
        return 1;
    }
    if (!(temp_v0 & RACE_ACTOR_COLLISION_ACTIVE)) {
        return 1;
    }
    if (D_80121D50 < 9) {
        D_80121D50 = 9;
    }
    return 0;
}

s32 func_800852E8(RaceActorCollision *arg0) {
    s32 temp_v0;
    s32 var_v1;
    s32 temp_t9;

    temp_v0 = arg0->collisionFlags;
    if (temp_v0 & 0x43000) {
        return 1;
    }
    var_v1 = D_80121D54 - arg0->yaw;
    var_v1 += RACE_ACTOR_COLLISION_HALF_TURN;
    temp_t9 = var_v1 & RACE_ACTOR_COLLISION_ANGLE_MASK;
    var_v1 = (s16) temp_t9;
    if (temp_v0 & RACE_ACTOR_COLLISION_YAW_FLIPPED) {
        var_v1 = (s16) (var_v1 + RACE_ACTOR_COLLISION_HALF_TURN);
    }
    var_v1 += 0x400;
    var_v1 = (s16) (var_v1 & RACE_ACTOR_COLLISION_ANGLE_MASK);
    if ((D_80121D58 < 0x14000) || (var_v1 >= 0x801)) {
        return 1;
    }
    if (D_80121D50 < 2) {
        D_80121D50 = 2;
    }
    return 0;
}

s32 func_800853A0(RaceActorCollision *arg0) {
    if (arg0->collisionFlags & 0x43000) {
        return 1;
    }
    if (D_80121D50 <= 0) {
        D_80121D50 = 1;
    }
    return 0;
}

s32 func_800853E4(RaceActorCollision *arg0) {
    if (arg0->collisionFlags & 0x345001) {
        return 1;
    }
    if (D_80121D50 < 0xE) {
        D_80121D50 = 0xE;
    }
    return 0;
}

s32 func_8008542C(RaceActorCollision *arg0) {
    if (arg0->collisionFlags & 0x775000) {
        return 1;
    }
    if (D_80121D50 < 0xF) {
        D_80121D50 = 0xF;
    }
    return 0;
}

s32 func_80085474(RaceActorCollision *arg0) {
    if (arg0->collisionFlags & 0x375000) {
        return 1;
    }
    if (arg0->unk2D4 != 0) {
        return 1;
    }
    if (arg0->unk320 != 0) {
        return 1;
    }
    if (D_80121D50 < 0x10) {
        D_80121D50 = 0x10;
    }
    return 0;
}

s32 func_800854E8(RaceActorCollision *arg0) {
    if (arg0->collisionFlags & 0x375000) {
        return 1;
    }
    if (arg0->unk320 != 0) {
        return 1;
    }
    if (D_80121D50 < 0x11) {
        D_80121D50 = 0x11;
    }
    return 0;
}

s32 func_80085544(RaceActorCollision *arg0) {
    if (arg0->collisionFlags & 0x775000) {
        return 1;
    }
    if (D_80121D50 < 3) {
        D_80121D50 = 3;
    }
    return 0;
}

s32 func_8008558C(RaceActorCollision *arg0) {
    if (arg0->collisionFlags & 0x775000) {
        return 1;
    }
    if (D_80121D50 < 4) {
        D_80121D50 = 4;
    }
    return 0;
}

s32 func_800855D4(RaceActorCollision *arg0) {
    if (arg0->collisionFlags & 0x41001) {
        return 1;
    }
    if (D_80121D50 < 0x12) {
        D_80121D50 = 0x12;
    }
    return 0;
}

s32 func_8008561C(RaceActorCollision *arg0) {
    if (arg0->collisionFlags & 0x41001) {
        return 1;
    }
    if (D_80121D50 < 0x13) {
        D_80121D50 = 0x13;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_actor_collision/func_80085664.s")

s32 func_800860A0(RaceActorCollision *arg0) {
    s32 temp_v0;

    temp_v0 = func_80081124(arg0->unk502, arg0->unk1C, arg0->unk24, arg0);
    if (temp_v0 == 3) {
        return 0;
    }
    if (temp_v0 == 6) {
        return 0;
    }
    if (temp_v0 == 7) {
        return 0;
    }
    if (temp_v0 == 8) {
        return 0;
    }
    if (temp_v0 == 0xC) {
        return 0;
    }
    if (temp_v0 == 0xE) {
        return 0;
    }
    if (temp_v0 == 0xF) {
        return 0;
    }
    if (temp_v0 == 0x10) {
        return 0;
    }
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_actor_collision/func_80086170.s")
