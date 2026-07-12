#include "common.h"

#define RACE_ACTOR_COLLISION_YAW_FLIPPED 0x400
#define RACE_ACTOR_COLLISION_ACTIVE 0x800
#define RACE_ACTOR_COLLISION_ALWAYS_VALID 0x400000

#define RACE_ACTOR_COLLISION_ANGLE_MASK 0xFFF
#define RACE_ACTOR_COLLISION_HALF_TURN 0x800

typedef struct {
    /* 0x000 */ u16 unk0;
    /* 0x002 */ char pad2[2];
    /* 0x004 */ s32 unk4;
    /* 0x008 */ char pad8[0xC];
    /* 0x014 */ s8 unk14;
    /* 0x015 */ char pad15[7];
    /* 0x01C */ s32 unk1C;
    /* 0x020 */ char pad20[4];
    /* 0x024 */ s32 unk24;
    /* 0x028 */ char pad28[0x18];
    /* 0x040 */ s32 unk40;
    /* 0x044 */ char pad44[4];
    /* 0x048 */ s32 unk48;
    /* 0x04C */ char pad4C[0x10];
    /* 0x05C */ s32 unk5C;
    /* 0x060 */ char pad60[0x220];
    /* 0x280 */ s32 unk280;
    /* 0x284 */ s32 unk284;
    /* 0x288 */ char pad288[0x3A];
    /* 0x2C2 */ char pad2C2[1];
    /* 0x2C3 */ s8 unk2C3;
    /* 0x2C4 */ char pad2C4[2];
    /* 0x2C6 */ u16 unk2C6;
    /* 0x2C8 */ s32 unk2C8;
    /* 0x2CC */ s32 unk2CC;
    /* 0x2D0 */ char pad2D0[2];
    /* 0x2D2 */ s16 unk2D2;
    s16 unk2D4;
    /* 0x2D6 */ char pad2D6[8];
    /* 0x2DE */ s16 unk2DE;
    /* 0x2E0 */ s32 unk2E0;
    /* 0x2E4 */ s32 unk2E4;
    /* 0x2E8 */ char pad2E8[4];
    /* 0x2EC */ s16 yaw;
    /* 0x2EE */ char pad2EE[0xE];
    /* 0x2FC */ s32 collisionFlags;
    /* 0x300 */ s16 unk300;
    /* 0x302 */ s16 unk302;
    /* 0x304 */ s16 unk304;
    /* 0x306 */ char pad306[0xA];
    /* 0x310 */ s32 unk310;
    /* 0x314 */ char pad314[0xC];
    /* 0x320 */ s16 unk320;
    /* 0x322 */ char pad322[0xE];
    /* 0x330 */ s8 unk330;
    /* 0x331 */ s8 unk331;
    /* 0x332 */ char pad332[0x1D0];
    /* 0x502 */ s16 unk502;
    /* 0x504 */ char pad504[0x13];
    /* 0x517 */ s8 unk517;
    /* 0x518 */ char pad518[0x62];
    /* 0x57A */ s8 unk57A;
    /* 0x57B */ char pad57B[0x91];
} RaceActorCollision;

typedef struct {
    s8 active;
    char pad1[0xAF];
} Unk8011228C;

extern s32 D_80121D50;
extern s32 D_80121D54;
extern s32 D_80121D58;
extern s16 D_80121D56;
extern s32 D_80121D5C;
extern s32 D_80121D60;
extern RaceActorCollision D_80121D80[];
extern Unk8011228C D_8011228C[];

s32 func_8004908C(s32, s32);
s32 func_8004DB60(s8);
s32 func_8004E518(s16, s16, s32, s32, s32);
s32 func_80072138(s32, s32);
s32 func_80072A74(s32, s32 *, s32, s32);
s32 func_80080CC4(s16, s32, s32);
s32 func_80098C30(s64);

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

// func_80085664 best match: 86.757% (nonmatchings/func_80085664-2225551288923588688/base_3.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_actor_collision/func_80085664.s")

#ifdef NON_MATCHING
void func_80085664(RaceActorCollision *arg0) {
    s32 var_t0;
    s32 var_t1;
    s32 var_t2;
    s32 var_t3;
    s32 var_t4;
    s32 var_t4_2;
    s32 var_t8;
    s32 var_t9;
    u16 temp_a0;
    u16 temp_a0_2;
    u16 temp_v0;

    D_80121D50 = 0;
    temp_v0 = arg0->unk2C6;
    if (temp_v0 != 0) {
        if ((temp_v0 & 0x200) && (arg0->unk5C < (func_80080CC4(arg0->unk502, arg0->unk1C, arg0->unk24) + 0xB0000))) {
            arg0->unk2C6 |= 1;
        }
        D_80121D5C = arg0->unk2C8 - arg0->unk40;
        D_80121D60 = arg0->unk2CC - arg0->unk48;
        D_80121D58 = func_80098C30((s64) D_80121D5C * D_80121D5C + (s64) D_80121D60 * D_80121D60) >> 1;
        D_80121D54 = func_8004908C(D_80121D5C, D_80121D60);
        if ((arg0->unk2C6 & 1) && (func_80085238(arg0) != 0) && (func_80084F50(arg0) != 0) && (func_800852E8(arg0) != 0)) {
            func_800853A0(arg0);
        }
        if ((arg0->unk2C6 & 2) && (func_80085238(arg0) != 0)) {
            func_80084F50(arg0);
        }
        temp_a0 = arg0->unk2C6;
        var_t4 = temp_a0 & 0x400;
        if (temp_a0 & 0x800) {
            func_80085010(arg0);
            var_t4 = arg0->unk2C6 & 0x400;
        }
        if ((var_t4 != 0) && (func_80085160(arg0) != 0)) {
            func_80085160(arg0);
        }
        if ((arg0->unk2C6 & 4) && (func_80085290(arg0) != 0)) {
            func_800850D0(arg0);
        }
        temp_a0_2 = arg0->unk2C6;
        var_t3 = temp_a0_2 & 0x100;
        if (temp_a0_2 & 8) {
            func_80085118(arg0);
            var_t3 = arg0->unk2C6 & 0x100;
        }
        var_t8 = arg0->unk2C6 & 0x10;
        if (var_t3 != 0) {
            func_800851F0(arg0);
            var_t8 = arg0->unk2C6 & 0x10;
        }
        var_t9 = arg0->unk2C6 & 0x20;
        if (var_t8 != 0) {
            func_8008542C(arg0);
            var_t9 = arg0->unk2C6 & 0x20;
        }
        var_t0 = arg0->unk2C6 & 0x1000;
        if (var_t9 != 0) {
            func_80085474(arg0);
            var_t0 = arg0->unk2C6 & 0x1000;
        }
        var_t1 = arg0->unk2C6 & 0x40;
        if (var_t0 != 0) {
            func_800854E8(arg0);
            var_t1 = arg0->unk2C6 & 0x40;
        }
        var_t2 = arg0->unk2C6 & 0x2000;
        if (var_t1 != 0) {
            func_80085544(arg0);
            var_t2 = arg0->unk2C6 & 0x2000;
        }
        var_t4_2 = arg0->unk2C6 & 0x80;
        if (var_t2 != 0) {
            func_8008558C(arg0);
            var_t4_2 = arg0->unk2C6 & 0x80;
        }
        if (var_t4_2 != 0) {
            func_800851A8(arg0);
        }
    }
    arg0->unk2C6 = 0;
    if (arg0->unk330 == 3) {
        if (arg0->unk331 == 3) {
            func_800853E4(arg0);
        }
    }
    if (arg0->unk330 == 8) {
        func_800853E4(arg0);
    }
    if ((arg0->unk330 == 6) || (arg0->unk330 == 0xC) || (arg0->unk330 == 0x10)) {
        func_800855D4(arg0);
    }
    if ((arg0->unk330 == 7) || (arg0->unk330 == 0xE) || (arg0->unk330 == 0xF)) {
        func_8008561C(arg0);
    }
    if (D_80121D50 != 0) {
        if (arg0->collisionFlags & 0x800) {
            arg0->unk2C3 = 0;
        }
        arg0->unk57A = 0;
        switch (D_80121D50) {
        case 5:
        case 6:
            if (D_80121D50 == 6) {
                if (D_8011228C[arg0->unk0].active != 0) {
                    func_80072138(0x47, 0x32);
                }
            } else {
                func_8008BBB8(arg0, 2);
            }
            arg0->unk300 = 4;
            arg0->unk302 = 0;
            arg0->unk304 = 0;
            arg0->unk2DE = (s16) D_80121D54;
            arg0->unk2E0 = arg0->unk2C8;
            arg0->unk2E4 = arg0->unk2CC;
            if (arg0->unk14 == 0) {
                func_80072A74(0x14, &arg0->unk1C, 0x7F, 0x32);
                func_8004E518((s16) arg0->unk0, arg0->unk2DE, 0, arg0->unk284 / 2, arg0->unk280);
            }
            break;
        case 8:
            func_8008BBB8(arg0, 2);
            arg0->unk300 = 0xE;
            arg0->unk302 = 0;
            arg0->unk304 = 0;
            if (arg0->unk14 == 0) {
                func_80072A74(0x14, &arg0->unk1C, 0x7F, 0x32);
                func_8004E518((s16) arg0->unk0, D_80121D56, 0, arg0->unk284 / 2, arg0->unk280);
            }
            break;
        case 9:
            func_8008BBB8(arg0, 2);
            arg0->unk300 = 0xE;
            arg0->unk302 = 0;
            arg0->unk304 = 0;
            arg0->unk2DE = (s16) D_80121D54;
            arg0->unk2E0 = arg0->unk2C8;
            arg0->unk2E4 = arg0->unk2CC;
            break;
        case 2:
            func_8008BBB8(arg0, 2);
            arg0->unk300 = 3;
            arg0->unk302 = 0;
            arg0->unk304 = 0;
            arg0->unk2DE = (s16) D_80121D54;
            arg0->unk2E0 = arg0->unk2C8;
            arg0->unk2E4 = arg0->unk2CC;
            if (arg0->unk14 == 0) {
                func_80072A74(0x14, &arg0->unk1C, 0x7F, 0x32);
                func_8004E518((s16) arg0->unk0, arg0->unk2DE, 1, arg0->unk284 / 2, arg0->unk280);
            }
            break;
        case 1:
            arg0->unk310 = 0xC000;
            break;
        case 7:
            if (arg0->unk14 == 0) {
                func_80072A74(0x14, &arg0->unk1C, 0x7F, 0x32);
                func_8004E518((s16) arg0->unk0, 0, 1, arg0->unk284 / 2, 0);
            }
            /* fallthrough */
        case 10:
            func_8008BBB8(arg0, 2);
            arg0->unk300 = 0xB;
            arg0->unk302 = 0;
block_104:
            arg0->unk304 = 0;
            break;
        case 11:
            D_80121D80[arg0->unk2D2 & 3].unk517 = 1;
            if (D_80121D80[arg0->unk2D2 & 3].isCpu != 0) {
                func_8008BBB8(arg0, 2);
            } else {
                func_8008BBB8(arg0, 3);
            }
            arg0->unk300 = 0x28;
            arg0->unk302 = 0;
            goto block_104;
        case 12:
            D_80121D80[arg0->unk2D2 & 3].unk517 = 1;
            if (D_80121D80[arg0->unk2D2 & 3].isCpu != 0) {
                func_8008BBB8(arg0, 2);
            } else {
                func_8008BBB8(arg0, 3);
            }
            arg0->unk300 = 0x1A;
            arg0->unk302 = 0;
            goto block_104;
        case 13:
            D_80121D80[arg0->unk2D2 & 3].unk517 = 1;
            if (D_80121D80[arg0->unk2D2 & 3].isCpu != 0) {
                func_8008BBB8(arg0, 2);
            } else {
                func_8008BBB8(arg0, 3);
            }
            arg0->unk300 = 0x1B;
            arg0->unk302 = 0;
            goto block_104;
        case 14:
            func_8008BBB8(arg0, 4);
            arg0->unk300 = 0xA;
            arg0->unk302 = 0;
            goto block_104;
        case 15:
            D_80121D80[arg0->unk2D2 & 3].unk517 = 1;
            if (D_80121D80[arg0->unk2D2 & 3].isCpu != 0) {
                func_8008BBB8(arg0, 2);
            } else {
                func_8008BBB8(arg0, 3);
            }
            arg0->unk300 = 0x17;
            arg0->unk302 = 0;
            goto block_104;
        case 16:
        case 17:
            if (D_80121D50 == 0x11) {
                if (D_80121D80[arg0->unk2D2 & 3].isCpu != 0) {
                    func_8008BBB8(arg0, 2);
                } else {
                    func_8008BBB8(arg0, 3);
                }
            } else {
                func_8008BBB8(arg0, 2);
            }
            if (arg0->unk14 == 0) {
                func_8004E518((s16) arg0->unk0, 0, 3, arg0->unk284 / 2, 0);
            }
            arg0->unk300 = 0x18;
            arg0->unk302 = 0;
            goto block_104;
        case 3:
            D_80121D80[arg0->unk2D2 & 3].unk517 = 1;
            if (D_80121D80[arg0->unk2D2 & 3].isCpu != 0) {
                func_8008BBB8(arg0, 2);
            } else {
                func_8008BBB8(arg0, 3);
            }
            arg0->unk300 = 0x19;
            arg0->unk302 = 0;
            goto block_104;
        case 4:
            func_8008BBB8(arg0, 2);
            arg0->unk300 = 0x19;
            arg0->unk302 = 0;
            goto block_104;
        case 18:
            arg0->unk300 = 6;
            arg0->unk302 = 0;
            arg0->unk304 = 0;
            if (func_8004DB60(arg0->unk330) != 0) {
                arg0->unk300 = 0x1C;
            }
            break;
        case 19:
            arg0->unk300 = 9;
            arg0->unk302 = 0;
            goto block_104;
        }
    }
}
#endif

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
