#include "common.h"
#include "asset_decompression.h"
#include "effect_task_scheduler.h"
#include "controller_rumble.h"
#include "game_audio.h"
#define func_8004940C func_8004940C_s32
#include "fixed_point_matrix.h"
#undef func_8004940C
#include "race_input_history.h"

typedef struct {
    /* 0x00 */ s8 active;
    /* 0x01 */ char pad1[0xAF];
} Unk8011228C;

typedef struct {
    /* 0x000 */ SoundPosition pos;
    /* 0x00C */ char padC[0x600];
} RacePlayerSoundPosition;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
} PlayerTuningRow;

typedef struct {
    s16 unk0;
    char pad2[0x16];
    s32 unk18;
    s32 unk1C;
    char pad20[0x20];
    s32 unk40;
    s32 unk44;
} Struct800955C0;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s16 unkC;
    s16 angle;
} CourseStartPosition;

extern void func_8008C098(RaceInputPlayer *);
extern void func_8008C7D0(RaceInputPlayer *);
extern void func_8008CF10(RaceInputPlayer *);
extern void func_80082664(RaceInputPlayer *, s32, s32, s32);
extern void func_80082DD0(RaceInputPlayer *);
extern void func_80082E48(RaceInputPlayer *);
extern void func_80082F44(RaceInputPlayer *);
extern void func_80081E40(RaceInputPlayer *, s32);
extern s32 func_80082EC0(RaceInputPlayer *);
extern void func_80083298(RaceInputPlayer *);
extern void func_800832CC(RaceInputPlayer *);
extern void func_800849E0(RaceInputPlayer *);
extern s32 func_80095F90(s32);
extern void func_8008BB20(RaceInputPlayer *, s32, s32, s32, s32);
extern void func_8008BB5C(RaceInputPlayer *, s32);
extern void func_8008B73C(RaceInputPlayer *, s32, s32, s32, s32, s32);
extern s32 func_80072138(s32, s32);
extern void func_80072A20(s32, SoundPosition *, s32, s32, f32, s16);
extern void func_8008393C(RaceInputPlayer *);
extern void func_80086170(RaceInputPlayer *);
extern void func_80061034(s32, s16);
extern void func_80057C08(s32, s16);
extern void func_80057DD4(void *);
extern void func_8007BE80(void *);
extern void func_8007C5E8(void *);
extern void func_8007CBC0(void *);
extern void func_80097038(RaceInputPlayer *);
extern void func_8008F1B4(RaceInputPlayer *);
extern void func_8008F1CC(RaceInputPlayer *);
extern void func_800815D4(s16, s32, s32, s32 *, s32 *, s32, s32);
extern void func_800483FC(void *, void (*)(void *), void *);
extern void func_8004FA44(void *);
extern void func_8005FB30(void *);
extern void func_800617C8(void *);
extern void func_80050030(void *);
extern void func_8004DCA0(RaceVec3i *, RaceVec3i *, RaceVec3i *, RaceVec3i *, s32, s32);
extern volatile s16 func_8004940C(s32, s32, s32, s32);
extern void *func_800716A4(void *, s32, s32, s32);
extern void func_80050E80(void *);
extern void func_80062530(void *);
extern s16 func_80097AE8(s16);
extern void func_800545D0(EffectTask *);
extern void func_80057810(void *);

extern void (*D_800DECD0[])(RaceInputPlayer *);
extern void (*D_800DECD8[])(RaceInputPlayer *);
extern void (*D_800DECE8[])(RaceInputPlayer *);
extern void (*D_800DED08[])(RaceInputPlayer *);
extern void (*D_800DED18[])(RaceInputPlayer *);
extern void (*D_800DED30[])(RaceInputPlayer *);
extern void (*D_800DED48[])(RaceInputPlayer *);
extern CourseStartPosition D_800DE918[][4];
extern u16 D_800DE8D0;
extern u16 D_800DE8F0;
extern u16 D_800DE904;
extern PlayerTuningRow D_800DC5E0[];
extern PlayerTuningRow D_800DC6F0[];
extern PlayerTuningRow D_800DC770[];
extern PlayerTuningRow D_800DC880[];
extern Struct800955C0 D_800B9540[];
extern u8 D_80121B5F;
extern u8 D_800EC9C2;
extern s8 D_80121B54;
extern u8 D_80121B56;
extern u8 D_80121B5E;
extern u8 D_80121B59;
extern u8 D_80121B58;
extern u8 D_80121B5A;
extern u8 D_80121B5B;
extern s16 D_80121B50;
extern s32 D_801235B4;
extern s16 D_801235B0;
extern Unk8011228C D_8011228C[];
extern void func_8007B250(void);
extern void func_80087AFC(void);
extern void func_80087EFC(void);
extern RacePlayerSoundPosition D_80121D9C[];
extern RacePlayerSoundPosition D_80121DA8[];
extern void *D_801248C8;
extern void *D_801248EC;
extern Struct800955C0 D_800B9540[];
extern s16 D_80121B50;

void func_8008BEB0(void) {
    D_80121D80[0].playerIndex = 0;
    D_80121D80[1].playerIndex = 1;
    D_80121D80[2].playerIndex = 2;
    D_80121D80[3].playerIndex = 3;
    if (D_80121D80[0].isActive != 0) {
        func_8008C098(D_80121D80);
    }
    if (D_80121D80[1].isActive != 0) {
        func_8008C098(D_8012238C);
    }
    if (D_80121D80[2].isActive != 0) {
        func_8008C098(D_80122998);
    }
    if (D_80121D80[3].isActive != 0) {
        func_8008C098(D_80122FA4);
    }
}

// func_8008BF5C best match: 99.241%
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_8008BF5C.s")

#ifdef NON_MATCHING
void func_8008BF5C(RaceInputPlayer *arg0) {
    RaceInputPlayer *player;
    PlayerTuningRow *var_v0;
    s32 new_var;
    PlayerTuningRow *var_v1;
    PlayerTuningRow *temp_a1;
    PlayerTuningRow *temp_a2;
    s32 temp_t5;

    player = arg0;
    var_v0 = D_800DC6F0;
    if ((D_80121B59 != 0) || (var_v1 = var_v0, D_80121B5B != 0)) {
        var_v0 = D_800DC770;
        var_v1 = D_800DC880;
    } else {
        var_v0 = D_800DC5E0;
    }
    temp_a1 = (PlayerTuningRow *)((u8 *)var_v1 + (player->characterId * sizeof(PlayerTuningRow)));
    temp_a2 = (PlayerTuningRow *)((u8 *)var_v0 + (player->unk11 * sizeof(PlayerTuningRow)));
    if (1) {
        temp_t5 = ((temp_a1->unk0 + temp_a2->unk0) << 3) << 5;
        player->unk25C = temp_t5;
        player->unk314 = temp_t5;
        player->unk260 = (temp_a1->unk2 + temp_a2->unk2) << 4;
        player->unk264 = (new_var = (((temp_a1->unk4 + temp_a2->unk4) << 2) << 1) << 1);
        player->unk268 = temp_a1->unk6 + temp_a2->unk6;
    }
    player->unk274 = (temp_a1->unk8 + temp_a2->unk8) << 4;
    player->unk26C = (temp_a1->unkA + temp_a2->unkA) << 4;
    player->unk270 = (temp_a1->unkC + temp_a2->unkC) << 4;
    player->unk278 = (temp_a1->unkE + temp_a2->unkE) << 4;
    player->unk27C = (temp_a1->unk10 + temp_a2->unk10) << 4;
}
#endif

// func_8008C098 best match: 82.068% (base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_8008C098.s")

#ifdef NON_MATCHING
void func_8008C098(RaceInputPlayer *player) {
    RaceVec3i *pos;
    RaceVec3i *unk34;
    CourseStartPosition *start;
    u32 size;
    u32 i;
    char *clearPtr;
    s32 groundY;

    size = ((s32)player * 0) + 0x5F0;
    i = 0;
    pos = &player->pos;
    if (size != 0) {
        clearPtr = (char *)pos;
        do {
            i++;
            *clearPtr = 0;
            clearPtr++;
        } while (i < size);
    }
    player->unk588 = 0.0f;
    func_80082FC8(player);
    func_80081E40(player, 1);
    if (D_80121B54 == 4) {
        switch (player->playerIndexU16) {
        case 0:
            player->posX = -0xC0000;
            break;
        case 1:
            player->posX = 0xC0000;
            break;
        case 2:
            player->posX = -0x240000;
            break;
        case 3:
            player->posX = 0x240000;
            break;
        }
    }
    player->posZ = -0x200000;
    unk34 = &player->unk34;
    unk34->x = pos->x;
    unk34->y = pos->y;
    unk34->z = pos->z;
    player->unk60 = 0x40000;
    player->unk280 = 0x80000;
    player->unk284 = 0xE0000;
    func_8008BF5C(player);
    if (player->unk4 != 0) {
        player->unk274 = 0x10000;
    }
    if ((player->unk4 != 0) && (player->characterId == 5)) {
        player->unk274 = 0xC0000;
    }
    if (D_800EC9C2 == 2) {
        player->actionEffectType = 1;
    }
    if (player->unk4 == 0) {
        player->unk519 = 6;
    }
    if ((D_800EC9C2 == 1) && (D_80121B5E == 1)) {
        player->shieldEffectTimer = 0xA;
    }
    if (D_80121B50 == 9) {
        player->posZ = -0xBD0000;
    }
    player->unk68 = 0xC0000;
    if ((player->unk4 == 0) && (player->soundDisabled == 0) && (D_80121B58 == 0) &&
        (player->unk27C != player->unk278)) {
        func_800716A4(func_80057810, 0, 0x64, player->playerIndexU16);
    }
    if (D_80121B59 == 1) {
        if (player->playerIndexU16 == 0) {
            player->itemEffectType = 3;
        } else {
            player->itemEffectType = 5;
        }
        player->itemEffectCount = 3;
        player->actionEffectType = 1;
    }
    switch (D_80121B5A) {
    case 1:
        player->facingAngle = 0x57E;
        player->posX = 0xDB870B2D;
        player->posZ = 0xE72C26D8;
        player->unk502 = 0x34;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        player->facingAngle = 0xFD5;
        player->posX = 0xAE2B01F4;
        player->posZ = 0xD6C4D18A;
        player->unk502 = 0x96;
        break;
    case 6:
        player->facingAngle = 0x993;
        player->posX = 0xC183C5F9;
        player->posZ = 0xCE51B844;
        player->unk502 = 0xAE;
        break;
    case 7:
    case 8:
        switch (player->playerIndexU16) {
        case 0:
            player->facingAngle = 0xF7D;
            player->posX = 0x8FC2BA;
            player->posZ = 0xFC4A83D8;
            player->unk502 = 4;
            break;
        case 2:
            player->facingAngle = 0x102;
            player->posX = 0x012FA8E0;
            player->posZ = 0xF9850FD2;
            player->unk502 = 8;
            break;
        case 3:
            player->facingAngle = 0x8F;
            player->posX = 0xC2C658;
            player->posZ = 0xFA155B82;
            player->unk502 = 7;
            break;
        case 1:
            player->facingAngle = 0x8F;
            player->posX = 0x017C2FC4;
            player->posZ = 0xFA1B250E;
            player->unk502 = 7;
            break;
        }
        break;
    case 9:
        player->facingAngle = 0x235;
        player->posX = 0xFEAFF7CD;
        player->posZ = 0xF62C899D;
        player->unk502 = 0xE;
        break;
    }
    switch (D_80121B5B) {
    case 1:
        player->itemEffectType = 1;
        player->itemEffectCount = 3;
        break;
    case 2:
        player->itemEffectType = 2;
        player->itemEffectCount = 3;
        break;
    case 3:
        player->itemEffectType = 3;
        player->itemEffectCount = 3;
        break;
    case 4:
        player->itemEffectType = 4;
        player->itemEffectCount = 3;
        break;
    case 5:
        player->itemEffectType = 5;
        player->itemEffectCount = 3;
        break;
    case 6:
        player->actionEffectType = 1;
        break;
    case 7:
        player->actionEffectType = 2;
        break;
    case 8:
        player->actionEffectType = 3;
        break;
    case 9:
        player->actionEffectType = 4;
        break;
    case 10:
        player->actionEffectType = 5;
        if (player->playerIndexU16 != 0) {
            player->unk568 = 0x1388;
        }
        break;
    case 11:
        player->actionEffectType = 6;
        if (player->playerIndexU16 == 2) {
            player->actionEffectType = 3;
        }
        player->itemEffectType = 1;
        player->itemEffectCount = 3;
        break;
    }
    if (D_80121B5B != 0) {
        start = &D_800DE918[D_80121B5B - 1][player->playerIndexU16];
        pos->x = start->x;
        pos->y = start->y;
        pos->z = start->z;
        player->facingAngle = start->angle;
        player->unk502 = start->unkC;
    }
    player->unk502 = func_8007D200(player->unk502, player->posX, player->posZ);
    groundY = func_80080CC4(player->unk502, player->posX, player->posZ);
    player->posY = groundY;
    if (D_80121B59 == 0) {
        player->posY = groundY + 0x40000;
    }
    unk34->x = pos->x;
    unk34->y = pos->y;
    unk34->z = pos->z;
    player->unk518 = func_800430D0();
    func_8006D520(player->playerIndexU16, 1);
    if (D_80121B59 == 0) {
        if (D_800EC9C2 == 0) {
            func_80071664(func_800545D0, 0, 1, player->playerIndexU16);
        }
    }
}
#endif

void func_8008C704(void) {
    s32 i;

    D_80121D80[0].unk582 = 0;
    D_80121D80[1].unk582 = 0;
    D_80121D80[2].unk582 = 0;
    D_80121D80[3].unk582 = 0;
    if (D_80121B56 == 0) {
        func_80084730(D_80121D80);
        func_80084730(D_8012238C);
        func_80084730(D_80122998);
        func_80084730(D_80122FA4);
        func_8007B250();
        for (i = 0; i < D_80121B54; i++) {
            func_8008C7D0(&D_80121D80[i]);
        }
        func_80087EFC();
        func_80087AFC();
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_8008C7D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_8008CF10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_8008D09C.s")

void func_8008DAF0(RaceInputPlayer *player) {
    s16 updateState;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateFlags &= ~0x200;
        player->stateTimer = 0;
        player->unk80 = 0;
        player->subState = 0;
        player->subStateTimer = 0;
        player->unk2FA = 0;
        player->unk528 = 0;
        func_80081E40(player, 0x1E);
    }

    player->unk40.y -= player->unk260;
    player->facingAngle = player->unk332;
    player->pitchAngle = player->unk334;
    if (player->stateFlags & 0x400) {
        player->facingAngle += 0x800;
        player->pitchAngle = -(player->pitchAngle & 0xFFF);
    }

    func_8008B408(player, 0, 0);
    func_8008B508(&player->unk40, player);
    player->posX += player->unk40.x;
    player->posY += player->unk40.y;
    player->posZ += player->unk40.z;

    if (func_80082EC0(player) != 0) {
        func_80081E40(player, 0x1F);
        func_80082EC0(player);
    }

    player->unk578 = 6;
    func_8008CF10(player);
    if (player->unk517 != 0) {
        func_8008BBB8(player, 5);
    }
}

void func_8008DC2C(RaceInputPlayer *player) {
    s32 unused;
    s32 targetX;
    s32 targetZ;
    s16 angleDiff;
    s32 yVel;

    if (player->updateState == 0) {
        if (player->animationId != 4) {
            func_80081E40(player, 4);
        }
        player->updateState++;
        player->stateFlags |= 0x200;
        player->unk60 = 0;
        func_8008F1B4(player);
        player->unk2A6 = 0;
    }

    if (player->subState == 0) {
        func_800849E0(player);
        if (player->unk4 == 0) {
            if (player->stickX > 0) {
                player->facingAngle -= 0x18;
            }
            if (player->stickX < 0) {
                player->facingAngle += 0x18;
            }
        } else {
            func_800815D4(player->unk502, unused = player->posX, player->posZ, &targetX, &targetZ,
                          (s8) player->unk17, (u16) player->playerIndex);
            angleDiff = (func_8004940C(player->posX, player->posZ, targetX, targetZ) - player->facingAngle) & 0xFFF;
            if (angleDiff >= 0x801) {
                angleDiff -= 0x1000;
            }
            if (angleDiff >= 0x19) {
                angleDiff = 0x18;
            }
            if (angleDiff < -0x18) {
                angleDiff = -0x18;
            }
            player->facingAngle += angleDiff;
        }
        func_80082EC0(player);
    }

    func_8008F1CC(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);
    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;
    if (player->unk517 != 0) {
        func_8008BBB8(player, 5);
    }
}

void func_8008DE1C(RaceInputPlayer *player) {
    s32 unused;
    s32 targetX;
    s32 targetZ;
    s16 angleDiff;
    s32 yVel;

    if (player->updateState == 0) {
        player->updateState++;
        player->stateFlags |= 0x208;
        player->unk60 = 0;
        player->unk80 = 0;
        func_8008F1B4(player);
        player->unk2A6 = 0;
    }

    if (player->subState == 0) {
        func_800849E0(player);
        if (player->unk4 == 0) {
            if (player->stickX > 0) {
                player->facingAngle -= 0x18;
            }
            if (player->stickX < 0) {
                player->facingAngle += 0x18;
            }
        } else {
            func_800815D4(player->unk502, unused = player->posX, player->posZ, &targetX, &targetZ,
                          (s8) player->unk17, (u16) player->playerIndex);
            angleDiff = (func_8004940C(player->posX, player->posZ, targetX, targetZ) - player->facingAngle) & 0xFFF;
            if (angleDiff >= 0x801) {
                angleDiff -= 0x1000;
            }
            if (angleDiff >= 0x19) {
                angleDiff = 0x18;
            }
            if (angleDiff < -0x18) {
                angleDiff = -0x18;
            }
            player->facingAngle += angleDiff;
        }
    }

    func_8008F1CC(player);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);
    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;
    if (player->unk517 != 0) {
        func_8008BBB8(player, 5);
    }
}

void func_8008DFD0(RaceInputPlayer *player) {
    D_800DECD0[player->updateState](player);
}

void func_8008E008(RaceInputPlayer *player) {
    Struct800955C0 *spawn;
    s16 updateTimer;
    s32 velocityY;
    s32 rotation;
    s8 turnTimer;

    updateTimer = player->updateTimer;
    if (updateTimer == 0) {
        player->updateTimer = updateTimer + 1;
        player->stateTimer = 0x40000;
        player->stateFlags |= 0x10;
        func_80081E40(player, 5);
        func_80083298(player);
        player->unk60 = 0x40000;
        player->unk2A6 = 0;
        player->unk93 = 0;
        player->unk336 = 0;
    }

    if (player->unk4 == 0) {
        spawn = &D_800B9540[D_80121B50];
        if ((spawn->unk0 == player->unk502) && !(player->stateFlags & 0x40)) {
            velocityY = (s16) (((func_8004940C(player->posX, player->posZ, spawn->unk40, spawn->unk44) -
                                  player->facingAngle) +
                                 0x400) &
                                0xFFF);
            if (velocityY < 0x800) {
                if (player->stateFlags & 0x400) {
                    player->stateFlags &= ~0x400;
                    player->unk93 = 6 - player->unk93;
                }
            } else if (!(player->stateFlags & 0x400)) {
                player->stateFlags |= 0x400;
                player->unk93 = 6 - player->unk93;
            }
        } else {
            if (player->unk254 < -0x8000) {
                if (player->stateFlags & 0x400) {
                    player->stateFlags &= ~0x400;
                    player->unk93 = 6 - player->unk93;
                }
            }
            if (player->unk254 >= 0x8001) {
                if (!(player->stateFlags & 0x400)) {
                    player->stateFlags |= 0x400;
                    player->unk93 = 6 - player->unk93;
                }
            }
        }
    }

    turnTimer = player->unk93;
    if (turnTimer != 0) {
        player->unk93 = turnTimer - 1;
        func_80081E40(player, player->unk93 + 8);
    } else if (player->animationId != 5) {
        func_80081E40(player, 5);
    }

    func_800849E0(player);
    func_80082F44(player);

    velocityY = player->unk60;
    if (velocityY != 0) {
        if (velocityY < 0x20000) {
            player->unk60 = 0;
        } else {
            player->unk60 = velocityY - 0x20000;
        }
    } else {
        player->updateState++;
    }

    if ((player->unk4 == 0) && !(player->inputFlags & 0x8000)) {
        player->stateFlags &= ~0x10;
    }

    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk260;

    rotation = 0;
    if (player->trailEffectTimer != 0) {
        rotation = -0x8000;
        if (player->unk2DC != 0) {
            rotation = 0x8000;
        }
    }

    func_8008B73C(player, 0, rotation, player->unk274, player->unk278, player->unk27C);
    player->posX += player->unk40.x;
    player->posY += player->unk40.y;
    player->posZ += player->unk40.z;
    func_800832CC(player);
    func_8008CF10(player);
    if (player->unk517 != 0) {
        func_8008BBB8(player, 5);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_8008E350.s")

void func_8008F1B4(RaceInputPlayer *player) {
    player->subState = 0;
    player->subStateTimer = 0;
    player->subStateStep = 0;
    player->subStateParam = 0;
    player->unk2A0 = 0;
}

void func_8008F1CC(RaceInputPlayer *player) {
    D_800DECD8[player->subState](player);
}

void func_8008F204(RaceInputPlayer *player) {
    s16 inputMask;

    player->subStateTimer = 0;
    player->subStateStep = 0;
    player->unk2A2 = 0;
    player->unk2A4 = 0;
    if (player->unk4 == 0) {
        if (player->currentInputFlags & 1) {
            player->subStateTimer = 1;
            player->unk2A2 = 1;
        }
        if (player->currentInputFlags & 2) {
            player->subStateTimer = 2;
            player->unk2A2 = 3;
        }
        if (player->currentInputFlags & 8) {
            player->subStateTimer = 8;
            player->unk2A2 = 7;
        }
        if (player->currentInputFlags & 4) {
            player->subStateTimer = 4;
            player->unk2A2 = 5;
        }
        if (player->subStateTimer != 0) {
            if (player->unk2A0 < 6) {
                player->unk2A2++;
            }
            player->subState++;
            if (player->stateFlags & 0x400) {
                inputMask = player->subStateTimer;
                if (inputMask & 1) {
                    func_80081E40(player, 0x17);
                    inputMask = player->subStateTimer;
                }
                if (inputMask & 2) {
                    func_80081E40(player, 0x15);
                    inputMask = player->subStateTimer;
                }
            } else {
                inputMask = player->subStateTimer;
                if (inputMask & 1) {
                    func_80081E40(player, 0x15);
                    inputMask = player->subStateTimer;
                }
                if (inputMask & 2) {
                    func_80081E40(player, 0x17);
                    inputMask = player->subStateTimer;
                }
            }
            if (inputMask & 8) {
                func_80081E40(player, 0x19);
                inputMask = player->subStateTimer;
            }
            if (inputMask & 4) {
                func_80081E40(player, 0x1B);
            }
            player->stateFlags |= 0x800;
        }
        player->unk2A0++;
    }
}

void func_8008F3C8(RaceInputPlayer *player) {
    s16 inputMask;

    if (!(player->inputFlags & player->subStateTimer)) {
        player->subStateStep = 1;
    }
    if (func_80082EC0(player) != 0) {
        if (player->subStateStep != 0) {
            player->subState++;
            if (player->stateFlags & 0x400) {
                inputMask = player->subStateTimer;
                if (inputMask & 1) {
                    func_80081E40(player, 0x18);
                    inputMask = player->subStateTimer;
                }
                if (inputMask & 2) {
                    func_80081E40(player, 0x16);
                    inputMask = player->subStateTimer;
                }
            } else {
                inputMask = player->subStateTimer;
                if (inputMask & 1) {
                    func_80081E40(player, 0x16);
                    inputMask = player->subStateTimer;
                }
                if (inputMask & 2) {
                    func_80081E40(player, 0x18);
                    inputMask = player->subStateTimer;
                }
            }
            if (inputMask & 8) {
                func_80081E40(player, 0x1A);
                inputMask = player->subStateTimer;
            }
            if (inputMask & 4) {
                func_80081E40(player, 0x1C);
            }
        } else {
            player->unk2A4++;
        }
    }
    player->stateFlags |= 0x800;
}

void func_8008F514(RaceInputPlayer *player) {
    if (func_80082EC0(player) != 0) {
        player->subState++;
    }
    player->stateFlags |= 0x800;
}

void func_8008F560(s32 arg0) {

}

// func_8008F568 best match: 99.716%
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_8008F568.s")

#ifdef NON_MATCHING
void func_8008F568(RaceInputPlayer *player) {
    s32 yVel;
    s32 sine;
    s32 stateTimer;
    u32 stateFlags;
    long long clampedTimer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
    }

    if (player->subState == 0) {
        func_80082EC0(player);
    }

    func_8008F1CC(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    sine = func_80097AE8(player->stateTimerLow) << 0xC;
    player->unk6C = sine / 0x1000;
    stateTimer = (player->stateTimer & 0xFFFFFFFFFFFFFFFF) + 0x20;
    player->stateTimer = stateTimer;
    if (stateTimer >= 0x401) {
        clampedTimer = 0x400;
        stateTimer = clampedTimer;
        player->stateTimer = stateTimer;
    }

    stateFlags = player->stateFlags | 2;
    player->stateFlags = stateFlags;
    if (stateTimer < 0x3D0) {
        player->stateFlags = stateFlags | 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050030, 5, 2, (u16) player->playerIndex);
        }
    }
}
#endif

// func_8008F6C8 best match: 99.607% (nonmatchings/func_8008F6C8-6688367443449623229/base_13.c)

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_8008F6C8.s")

#ifdef NON_MATCHING
void func_8008F6C8(RaceInputPlayer *player) {
    s32 yVel;
    s32 stateTimer;
    u32 stateFlags;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
    }

    if (player->subState == 0) {
        func_80082EC0(player);
    }

    func_8008F1CC(player);
    func_8008B408(player, player->unk254, 0);
    stateTimer = 0x400;
    player->velocity.y -= player->unk264;
    func_8008B508(&player->velocity, player);

    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;

    player->unk6C = (s16) ((s32) (func_80097AE8(player->unk7E) * -0x1000) / 0x1000);
    stateTimer = (unsigned long long) (player->stateTimer + 0x20);
    player->stateTimer = stateTimer;
    if (stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    stateFlags = player->stateFlags | 2;
    player->stateFlags = stateFlags;
    if (stateTimer < 0x3D0) {
        player->stateFlags = stateFlags | 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050030, 5, 2, (u16) player->playerIndex);
        }
    }
}
#endif

void func_8008F82C(RaceInputPlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
    }
    if (player->subState == 0) {
        func_80082EC0(player);
    }
    func_8008F1CC(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);
    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;
    if (player->stateFlags & 0x400) {
        player->unk6E = (s16) ((s32) (func_80097AE8(player->stateTimerLow) << 0xC) / 4096);
    } else {
        player->unk6E = (s16) ((s32) (func_80097AE8(player->stateTimerLow) * -0x1000) / 4096);
    }

    player->stateTimer += 0x1E;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }
    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050030, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_8008F9CC(RaceInputPlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
    }
    if (player->subState == 0) {
        func_80082EC0(player);
    }
    func_8008F1CC(player);
    func_8008B408(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    func_8008B508(&player->velocity, player);
    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;
    if (player->stateFlags & 0x400) {
        player->unk6E = (s16) ((s32) (func_80097AE8(player->unk7E) * -0x1000) / 0x1000);
    } else {
        player->unk6E = (s16) ((s32) (func_80097AE8(player->unk7E) << 12) / 0x1000);
    }

    player->stateTimer += 0x1D;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }
    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050030, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_8008FB6C(RaceInputPlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
    }
    if (player->subState == 0) {
        func_80082EC0(player);
    }
    func_8008F1CC(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);
    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;
    player->unk6C = (s16) ((s32) (func_80097AE8((s16) player->stateTimer) << 12) / 0x1000);
    if (player->stateFlags & 0x400) {
        player->unk6E = (s16) ((s32) (func_80097AE8((s16) player->stateTimer) << 12) / 0x1000);
    } else {
        player->unk6E = (s16) ((s32) (func_80097AE8((s16) player->stateTimer) * -0x1000) / 0x1000);
    }

    player->stateTimer += 0x1A;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }
    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050030, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_8008FD2C(RaceInputPlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
    }
    if (player->subState == 0) {
        func_80082EC0(player);
    }
    func_8008F1CC(player);
    func_8008B408(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    func_8008B508(&player->velocity, player);
    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;
    player->unk6C = (s16) ((s32) (func_80097AE8(player->unk7E) << 12) / 0x1000);
    if (player->stateFlags & 0x400) {
        player->unk6E = (s16) ((s32) (func_80097AE8(player->unk7E) * -0x1000) / 0x1000);
    } else {
        player->unk6E = (s16) ((s32) (func_80097AE8(player->unk7E) << 12) / 0x1000);
    }

    player->stateTimer += 0x1A;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }
    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050030, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_8008FEEC(RaceInputPlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
    }
    if (player->subState == 0) {
        func_80082EC0(player);
    }
    func_8008F1CC(player);
    func_8008B408(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    func_8008B508(&player->velocity, player);
    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;
    player->unk6C = (s16) ((s32) (func_80097AE8(player->unk7E) * -0x1000) / 0x1000);
    if (player->stateFlags & 0x400) {
        player->unk6E = (s16) ((s32) (func_80097AE8(player->unk7E) << 12) / 0x1000);
    } else {
        player->unk6E = (s16) ((s32) (func_80097AE8(player->unk7E) * -0x1000) / 0x1000);
    }

    player->stateTimer += 0x1A;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }
    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050030, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_800900B0(RaceInputPlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
    }
    if (player->subState == 0) {
        func_80082EC0(player);
    }
    func_8008F1CC(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);
    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;
    player->unk6C = (s16) ((s32) (func_80097AE8(player->stateTimerLow) * -0x1000) / 0x1000);
    if (player->stateFlags & 0x400) {
        player->unk6E = (s16) ((s32) (func_80097AE8(player->stateTimerLow) * -0x1000) / 0x1000);
    } else {
        player->unk6E = (s16) ((s32) (func_80097AE8(player->stateTimerLow) << 12) / 0x1000);
    }

    player->stateTimer += 0x1A;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }
    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050030, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_80090274(RaceInputPlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
        func_80081E40(player, 0x15);
        player->updateTimer = 0;
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (func_80097AE8(player->stateTimerLow) * -0x2000) / 4096;
    } else {
        player->unk6E = (func_80097AE8(player->stateTimerLow) * 0x2000) / 4096;
    }

    player->stateTimer += 0x16;
    player->updateTimer++;
    updateTimer = player->updateTimer;
    if (updateTimer == 8) {
        func_80081E40(player, 0x16);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0xF) {
        func_80081E40(player, 0x1B);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0x1E) {
        func_80081E40(player, 0x1C);
    }

    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_80090470(RaceInputPlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
        player->updateTimer = 0;
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    func_8008B508(&player->velocity, player);

    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;

    player->unk6C = (func_80097AE8(player->unk7E) << 13) / 0x1000;

    player->stateTimer += 0x14;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_800905BC(RaceInputPlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
        player->updateTimer = 0;
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    func_8008B508(&player->velocity, player);

    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;

    player->unk70 = (func_80097AE8(player->unk7E) << 13) / 0x1000;

    player->stateTimer += 0x14;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

// func_80090708 best match: 99.900%

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_80090708.s")

#ifdef NON_MATCHING
void func_80090708(RaceInputPlayer *player) {
    s16 updateTimer;
    RaceInputPlayer *playerAlias;
    s32 allBitsSet;
    u64 clampValue;
    s32 yVel;
    void (*soundFunc)(void *);
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
        player->updateTimer = 0;
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);

    updateTimer = player->updateTimer;
    if (updateTimer < 8) {
        player->pitchAngle = -updateTimer << 7;
    }
    if (updateTimer >= 0x23) {
        player->pitchAngle = (-(-updateTimer) << 6) - 0xC80;
    }
    if (updateTimer < 0x32) {
        player->updateTimer = updateTimer + 1;
    }

    player->velocity.y -= player->unk264;
    func_8008B508(&player->velocity, player);

    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    soundFunc = func_80050E80;
    playerAlias = player;
    player->posZ += player->velocity.z;
    playerAlias->unk74 = yVel;

    playerAlias->unk6E = (((0, func_80097AE8(player->unk7E))) << 14) / 0x1000;

    timer = playerAlias->stateTimer + 0x14;
    playerAlias->stateTimer = timer;
    allBitsSet = 0xFFFFFFFFFFFFFFFF;
    if (timer >= 0x401) {
        clampValue = 0x400;
        player->stateTimer = (timer = clampValue) & 0xFFFFFFFFFFFFFFFFu;
    }

    (*playerAlias).stateFlags |= 2;
    if (timer < 0x3D0) {
        playerAlias->stateFlags |= 0x800 & allBitsSet;
        if ((playerAlias->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(soundFunc, 5, 2, (u16) player->playerIndex);
        }
    }
}
#endif

void func_80090898(RaceInputPlayer *player) {
    s16 updateState;
    s32 yVel;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        func_80081E40(player, 0x28);
        player->stateTimer = 0;
        player->stateFlags |= 0x200;
        func_8008F1B4(player);
        player->updateTimer = 0;
    }

    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (func_80082EC0(player) == 0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_80090998(RaceInputPlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 0x23);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
        player->updateTimer = 0;
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    player->unk6C = (func_80097AE8(player->stateTimerLow) * -0x3000) / 4096;
    if (player->stateFlags & 0x400) {
        player->unk6E = -player->updateTimer;
    } else {
        player->unk6E = player->updateTimer;
    }
    if (player->updateTimer != 0x1000) {
        player->updateTimer += 0x80;
    }

    player->stateTimer += 0x11;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

// func_80090B30 best match: 99.760% (nonmatchings/func_80090B30-3426750233777855594/base_17.c)

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_80090B30.s")

#ifdef NON_MATCHING
void func_80090B30(RaceInputPlayer *player) {
    s32 yVel;
    RaceInputPlayer *player2;
    s32 stateTimer;
    u32 stateFlags;

    if (player->updateState == 0) {
        func_80081E40(player, 0x23);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
        player->updateTimer = 0;
    }

    func_80082EC0(player);
    player2 = player;
    func_8008B408(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    func_8008B508(&player2->velocity, player2);

    yVel = player2->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player2->posZ += player->velocity.z;
    stateTimer = player2->stateTimer;
    player->unk6C = stateTimer * 4;
    player2->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player2->unk6E = (func_80097AE8((s16) stateTimer) * -0x4000) / 0x1000;
    } else {
        player2->unk6E = (func_80097AE8((s16) stateTimer) << 14) / 0x1000;
    }

    stateTimer = (unsigned long long) (player2->stateTimer + 0x15);
    player2->stateTimer = stateTimer;
    if (stateTimer >= 0x401) {
        player->stateTimer = (stateTimer = 0x400);
        stateTimer = player2->stateTimer;
    }

    stateFlags = player->stateFlags | 2;
    player2->stateFlags = stateFlags;
    if (stateTimer < 0x3D0) {
        player2->stateFlags = stateFlags | 0x800;
        if ((player2->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}
#endif

void func_80090CD0(RaceInputPlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
        func_80081E40(player, 0x17);
        player->updateTimer = 0;
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (func_80097AE8(player->stateTimerLow) * -0x2000) / 4096;
    } else {
        player->unk6E = (func_80097AE8(player->stateTimerLow) * 0x2000) / 4096;
    }

    player->stateTimer += 0x16;
    player->updateTimer++;
    if (player->updateTimer == 8) {
        func_80081E40(player, 0x18);
    }
    if (player->updateTimer == 0xF) {
        func_80081E40(player, 0x19);
    }
    if (player->updateTimer == 0x1E) {
        func_80081E40(player, 0x1A);
    }

    timer = player->stateTimer;
    if (timer >= 0x401) {
        timer = 0x400;
        player->stateTimer = timer;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

// func_80090ECC best match: 99.769% (nonmatchings/func_80090ECC-6688367443449623229/base_23.c)

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_80090ECC.s")

#ifdef NON_MATCHING
void func_80090ECC(RaceInputPlayer *player) {
    s16 updateTimer;
    unsigned long long timerConstant;
    s32 yVel;
    s32 stateTimer;
    u32 stateFlags;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    func_8008B508(&player->velocity, player);

    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;

    player->updateTimer = (s16) ((s32) (func_80097AE8(player->unk7E) << 13) / 0x1000);
    updateTimer = player->updateTimer;
    if (updateTimer >= 0x1001) {
        player->unk6E = 0x2000 - updateTimer;
    } else {
        player->unk6E = updateTimer;
    }

    if (player->stateFlags & 0x400) {
        player->unk6E = -player->unk6E;
    }

    player->unk6C = (s16) ((s32) (func_80097AE8(player->unk7E) << 13) / 0x1000);
    timerConstant = 0x14;
    stateTimer = player->stateTimer + timerConstant;
    timerConstant = 0x400;
    player->stateTimer = stateTimer;
    if (stateTimer >= 0x401) {
        stateTimer = timerConstant;
        player->stateTimer = stateTimer;
    }

    stateFlags = player->stateFlags | 2;
    player->stateFlags = stateFlags;
    if (stateTimer < 0x3D0) {
        player->stateFlags = stateFlags | 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}
#endif

void func_8009107C(RaceInputPlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
        func_80081E40(player, 0x1B);
        player->updateTimer = 0;
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    func_8008B508(&player->velocity, player);

    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (func_80097AE8(player->unk7E) * -0x5000) / 0x1000;
    } else {
        player->unk6E = (func_80097AE8(player->unk7E) * 0x5000) / 0x1000;
    }

    player->stateTimer += 0x12;
    player->updateTimer++;
    if (player->updateTimer == 0x28) {
        func_80081E40(player, 0x1C);
    }

    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_80091250(RaceInputPlayer *player) {
    s16 updateState;
    s32 yVel;
    s32 stateTimer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
        func_80081E40(player, 0x15);
        player->updateTimer = 0;
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    func_8008B508(&player->velocity, player);

    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk70 = (func_80097AE8(player->unk7E) * -0x2000) / 0x1000;
    } else {
        player->unk70 = (func_80097AE8(player->unk7E) << 13) / 0x1000;
    }

    player->stateTimer += 0x16;
    player->updateTimer++;
    if (player->updateTimer == 0x1E) {
        func_80081E40(player, 0x16);
    }

    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    stateTimer = player->stateTimer;
    player->stateFlags |= 2;
    if (stateTimer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_80091400(RaceInputPlayer *player) {
    s16 updateState;
    s32 yVel;
    s32 timer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
        func_80081E40(player, 0x15);
        player->updateTimer = 0;
    }
    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (s16) ((s32) (func_80097AE8(player->stateTimerLow) * -0x3000) / 4096);
    } else {
        player->unk6E = (s16) ((s32) (func_80097AE8(player->stateTimerLow) * 0x3000) / 4096);
    }

    player->stateTimer += 0x15;
    player->updateTimer++;
    if (player->updateTimer == 0x1E) {
        func_80081E40(player, 0x16);
    }

    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_800915C0(RaceInputPlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
        func_80081E40(player, 0x19);
        player->updateTimer = 0;
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (func_80097AE8(player->stateTimerLow) * -0x2000) / 4096;
    } else {
        player->unk6E = (func_80097AE8(player->stateTimerLow) << 0xD) / 4096;
    }

    player->stateTimer += 0x16;
    player->updateTimer++;
    updateTimer = player->updateTimer;
    if (updateTimer == 8) {
        func_80081E40(player, 0x1A);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0xF) {
        func_80081E40(player, 0x15);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0x1E) {
        func_80081E40(player, 0x16);
    }

    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

// func_800917BC best match: 99.795%
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_800917BC.s")

#ifdef NON_MATCHING
void func_800917BC(RaceInputPlayer *player) {
    s16 updateState;
    s32 yVel;
    s32 stateTimer;
    u32 stateFlags;
    long long clampedTimer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
        func_80081E40(player, 0x15);
        player->updateTimer = 0;
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk70 = (func_80097AE8(player->stateTimerLow) * 0x3000) / 0x1000;
    } else {
        player->unk70 = (func_80097AE8(player->stateTimerLow) * -0x3000) / 0x1000;
    }

    player->stateTimer += 0x16;
    if (player->updateTimer < 0x2D) {
        player->updateTimer = player->updateTimer + 1;
    }

    if (player->updateTimer == 0x1E) {
        func_80081E40(player, 0x16);
    }

    stateTimer = player->stateTimer;
    player->unk6E = (player->updateTimer << 0xC) / 45;
    if (stateTimer >= 0x401) {
        clampedTimer = 0x400;
        stateTimer = clampedTimer;
        clampedTimer = stateTimer;
        player->stateTimer = clampedTimer;
    }

    stateFlags = player->stateFlags | 2;
    player->stateFlags = stateFlags;
    if (stateTimer < 0x3D0) {
        player->stateFlags = stateFlags | 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}
#endif

void func_800919A4(RaceInputPlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    player->unk6C = (func_80097AE8(player->stateTimerLow) * -0x3000) / 0x1000;

    player->stateTimer += 0x12;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_80091AF8(RaceInputPlayer *player) {
    s16 updateState;
    s16 updateTimer;
    s16 tilt;
    s16 *updateTimerPtr;
    s32 yVel;
    s32 timer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        func_80081E40(player, 0x23);
        player->stateTimer = 0;
        player->stateFlags |= 0x200;
        func_8008F1B4(player);
        player->updateTimer = 0;
        player->unk306 = 0;
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);

    yVel = (u32) player->unk40.y;
    player->posX += player->unk40.x;
    updateTimerPtr = &player->updateTimer;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk70 = (func_80097AE8(player->stateTimerLow) * -0x2000) / 4096;
    } else {
        player->unk70 = (func_80097AE8(player->stateTimerLow) << 0xD) / 4096;
    }

    player->stateTimer += 0x16;
    player->updateTimer++;
    updateTimer = player->updateTimer;
    if (updateTimer == 0xC) {
        func_80081E40(player, 0x15);
        updateTimer = *updateTimerPtr;
    }

    if (updateTimer == 0x1E) {
        func_80081E40(player, 0x16);
        updateTimer = player->updateTimer;
    }

    if ((updateTimer >= 9) && (updateTimer < 0x14)) {
        tilt = player->unk306;
        if (tilt != -0x400) {
            player->unk306 = tilt - 0x80;
            updateTimer = player->updateTimer;
        }
    }

    tilt = player->unk306;
    if ((updateTimer >= 0x1F) && (tilt != 0)) {
        player->unk306 = tilt + 0x80;
        tilt = player->unk306;
    }

    if (player->stateFlags & 0x400) {
        player->unk6E = -tilt;
    } else {
        player->unk6E = tilt;
    }

    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_80091D40(RaceInputPlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
        func_80081E40(player, 0x19);
        player->updateTimer = 0;
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (func_80097AE8(player->stateTimerLow) * -0x2000) / 4096;
    } else {
        player->unk6E = (func_80097AE8(player->stateTimerLow) * 0x2000) / 4096;
    }

    player->stateTimer += 0x16;
    player->updateTimer++;
    updateTimer = player->updateTimer;
    if (updateTimer == 8) {
        func_80081E40(player, 0x1A);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0xF) {
        func_80081E40(player, 0x17);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0x1E) {
        func_80081E40(player, 0x18);
    }

    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_80091F3C(RaceInputPlayer *player) {
    s16 updateState;
    s16 updateTimer;
    RaceInputPlayer *playerAlias;
    s16 tilt;
    s32 yVel;
    s32 timer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        func_80081E40(player, 0x23);
        player->stateTimer = 0;
        player->stateFlags |= 0x200;
        func_8008F1B4(player);
        player->updateTimer = 0;
        player->unk306 = 0;
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    playerAlias = player;
    playerAlias->unk40.y -= playerAlias->unk264;
    func_8008B508(&playerAlias->unk40, player);

    yVel = player->unk40.y;
    playerAlias->posX += player->unk40.x;
    player->posY += yVel;
    playerAlias->posZ += player->unk40.z;
    playerAlias->unk74 = yVel;

    if (playerAlias->stateFlags & 0x400) {
        playerAlias->unk70 = (func_80097AE8((s16) player->stateTimer) * 0x3000) / 4096;
    } else {
        player->unk70 = (func_80097AE8((s16) player->stateTimer) * -0x3000) / 4096;
    }

    playerAlias->stateTimer += 0x14;
    player->updateTimer++;
    updateTimer = player->updateTimer;
    if (updateTimer == 0xC) {
        func_80081E40(playerAlias, 0x15);
        updateTimer = playerAlias->updateTimer;
    }

    if (updateTimer == 0x1E) {
        func_80081E40(playerAlias, 0x16);
        updateTimer = playerAlias->updateTimer;
    }

    if ((updateTimer >= 9) && (updateTimer < 0x14)) {
        tilt = player->unk306;
        if (tilt != 0x400) {
            player->unk306 = tilt + 0x80;
            updateTimer = playerAlias->updateTimer;
        }
    }

    tilt = player->unk306;
    if ((updateTimer >= 0x1F) && (tilt != 0)) {
        player->unk306 = tilt - 0x80;
        tilt = player->unk306;
    }

    if (playerAlias->stateFlags & 0x400) {
        playerAlias->unk6E = -tilt;
    } else {
        player->unk6E = tilt;
    }

    timer = player->stateTimer;
    if (timer >= 0x401) {
        timer = 0x400;
        playerAlias->stateTimer = timer;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        playerAlias->stateFlags |= 0x800;
        if ((playerAlias->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) playerAlias->playerIndex);
        }
    }
}

void func_80092194(RaceInputPlayer *player) {
    s32 yVel;
    s32 stateTimer;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
        func_80081E40(player, 0x1B);
        player->updateTimer = 0;
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (func_80097AE8(player->stateTimerLow) * -0x3000) / 0x1000;
    } else {
        player->unk6E = (func_80097AE8(player->stateTimerLow) * 0x3000) / 0x1000;
    }

    player->stateTimer += 0x16;
    player->updateTimer++;
    if (player->updateTimer == 0x1E) {
        func_80081E40(player, 0x1C);
    }

    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_80092368(RaceInputPlayer *player) {
    s16 updateState;
    s32 yVel;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        func_80081E40(player, 0x23);
        player->stateTimer = 0;
        player->stateFlags |= 0x200;
        func_8008F1B4(player);
        player->updateTimer = 0;
    }

    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (func_80082EC0(player) == 0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_80092468(RaceInputPlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
        func_80081E40(player, 0x19);
        player->updateTimer = 0;
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (func_80097AE8(player->stateTimerLow) * -0x1800) / 4096;
    } else {
        player->unk6E = ((func_80097AE8(player->stateTimerLow) * 6) * 1024) / 4096;
    }

    player->stateTimer += 0x16;
    player->updateTimer++;
    updateTimer = player->updateTimer;
    if (updateTimer == 8) {
        func_80081E40(player, 0x1A);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0xF) {
        func_80081E40(player, 0x1B);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0x1E) {
        func_80081E40(player, 0x1C);
    }

    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_80092674(RaceInputPlayer *player) {
    s16 updateState;
    s32 yVel;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        func_80081E40(player, 0x28);
        player->stateTimer = 0;
        player->stateFlags |= 0x200;
        func_8008F1B4(player);
        player->updateTimer = 0;
    }

    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (func_80082EC0(player) == 0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

// func_80092774 best match: 98.910% (nonmatchings/func_80092774-2127290767680699791/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_80092774.s")

#ifdef NON_MATCHING
void func_80092774(RaceInputPlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
        player->updateTimer = 0;
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);

    yVel = player->unk40.y;
    updateTimer = player->updateTimer;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    switch (updateTimer) {
    case 0:
        player->unk6C = (s16) ((s32) (func_80097AE8(player->stateTimerLow) << 12) / 4096);
        player->stateTimer += 0x33;
        if (player->stateTimer >= 0x401) {
            player->stateTimer = 0;
            player->updateTimer++;
            player->unk306 = 0;
        }

        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
        break;
    case 1:
        if (player->stateFlags & 0x400) {
            player->unk6E = (s16) ((s32) (func_80097AE8(player->stateTimerLow) * -0x2000) / 4096);
        } else {
            player->unk6E = (s16) ((s32) (func_80097AE8(player->stateTimerLow) << 13) / 4096);
        }

        timer = player->stateTimer + 0x20;
        player->unk6C = -player->unk306;
        player->stateTimer = timer;
        if (timer >= 0x401) {
            player->stateTimer = 0x400;
        }

        if (player->unk306 != 0x1000) {
            player->unk306 += 0x80;
        }

        if (player->stateTimer < 0x3D0) {
            player->stateFlags |= 0x800;
            if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
                func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
            }
        }
        break;
    }

    player->stateFlags |= 2;
}
#endif

void func_800929E4(RaceInputPlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (s16) ((s32) (func_80097AE8(player->stateTimerLow) * -0x2000) / 4096);
    } else {
        player->unk6E = (s16) ((s32) (func_80097AE8(player->stateTimerLow) << 0xD) / 4096);
    }

    player->stateTimer += 0x16;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_80092B6C(RaceInputPlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    func_8008B508(&player->velocity, player);

    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (func_80097AE8(player->unk7E) * -0x5000) / 0x1000;
    } else {
        player->unk6E = (func_80097AE8(player->unk7E) * 0x5000) / 0x1000;
    }

    player->stateTimer += 0x14;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_80092D04(RaceInputPlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        func_80081E40(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        func_8008F1B4(player);
    }

    func_80082EC0(player);
    func_8008B408(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    func_8008B508(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    player->unk6C = (func_80097AE8(player->stateTimer) * -0x5000) / 0x1000;

    player->stateTimer += 0x14;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (D_801235B0 & 1)) {
            func_800716A4(func_80050E80, 5, 2, (u16) player->playerIndex);
        }
    }
}

void func_80092E58(RaceInputPlayer *player) {
    RaceInputPlayer *playerAlias;
    Struct800955C0 *spawn;
    s16 updateState;
    s16 angleDiff;
    s32 clamped;
    s32 tempX;
    s32 tempZ;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x2000;
        if (((player->unk2DE - player->facingAngle) & 0xFFF) < 0x800) {
            func_80081E40(player, 7);
        } else {
            func_80081E40(player, 6);
        }
        player->unk40.x = player->unk2E0;
        player->unk40.z = player->unk2E4;
        player->actionEffectLevel = 3;
        player->actionEffectFrame = 0;
    }

    func_8008B408(player, 0x10000, 0);
    playerAlias = player;
    player->unk314 = 0x60000;
    if (!(playerAlias->stateFlags & 1)) {
        playerAlias->stateFlags &= ~0x200;
        playerAlias->unk40.y -= 0xA000;
        func_8008BB20(player, 0, (unsigned long long) 0x2000, 0x2000, 0x2000);
    } else {
        playerAlias->stateFlags |= 0x200;
        player->unk40.y -= 0x7000;
        func_8008B508(&player->unk40, player);

        tempX = playerAlias->unk40.x;
        clamped = tempX;
        if (clamped >= 0x2001) {
            clamped = 0x2000;
        }
        updateState = -0x2000;
        if (clamped < -0x2000) {
            clamped = updateState;
        }
        player->unk40.x = tempX - clamped;

        tempZ = playerAlias->unk40.z;
        clamped = tempZ;
        if (clamped >= 0x2001) {
            clamped = 0x2000;
        }
        if (clamped < updateState) {
            clamped = updateState;
        }
        player->unk40.z = tempZ - clamped;
    }

    playerAlias->posX += playerAlias->unk40.x;
    playerAlias->posY += player->unk40.y;
    playerAlias->posZ += playerAlias->unk40.z;

    spawn = &D_800B9540[D_80121B50];
    if ((spawn->unk0 == playerAlias->unk502) && !(playerAlias->stateFlags & 0x40)) {
        angleDiff = func_8004940C(playerAlias->posX, player->posZ, spawn->unk40, spawn->unk44);
        if (playerAlias->stateFlags & 0x400) {
            angleDiff += 0x800;
        }

        angleDiff = (angleDiff - player->facingAngle) & 0xFFF;
        if (angleDiff >= 0x801) {
            angleDiff -= 0x1000;
        }
        if (angleDiff >= 0x97) {
            angleDiff = 0x96;
        }
        if (angleDiff < -0x96) {
            angleDiff = -0x96;
        }
        playerAlias->facingAngle += angleDiff;
    }

    if (!(player->stateFlags & 1)) {
        func_8008CF10(player);
    }

    if (func_80082EC0(player) != 0) {
        playerAlias->stateFlags &= 0xFE0C1FFB;
        if (playerAlias->stateFlags & 0x200) {
            player->mode = 1;
            playerAlias->updateState = 0;
            player->updateTimer = 0;
        } else {
            playerAlias->mode = 0;
            playerAlias->updateState = 0;
            playerAlias->updateTimer = 0;
        }
    }
}

void func_80093144(RaceInputPlayer *player) {
    s16 updateState;
    s32 tempX;
    s32 tempZ;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        func_80081E40(player, 0xE);
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x12204;
        player->pitchAngle = 0;
        func_8008B408(player, 0, 0);
        player->unk40.x = player->unk2E0;
        player->unk40.z = player->unk2E4;
        player->unk40.y = 0x20000;
        player->unk60 = 0;
        player->actionEffectLevel = 3;
        player->actionEffectFrame = 0;
    }

    player->unk314 = 0x60000;
    if (player->stateFlags & 1) {
        func_8008B508(&player->unk40, player);
        tempX = player->unk40.x;
        tempZ = player->unk40.z;
        player->unk40.y -= 0xA000;
        player->unk40.x = tempX - (tempX / 15);
        player->unk40.z = tempZ - (tempZ / 15);
    } else {
        func_8008BB20(player, 0, 0x3000, 0x3000, 0x3000);
    }

    player->posX += player->unk40.x;
    player->posY += player->unk40.y;
    player->posZ += player->unk40.z;

    if ((func_80082EC0(player) != 0) && !(player->stateFlags & 1)) {
        player->mode = 5;
        player->updateState = 0;
        player->updateTimer = 0;
    }
}

// func_80093304 best match: 99.344%

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_80093304.s")

#ifdef NON_MATCHING
extern void func_80062A64(s16);

void func_80093304(RaceInputPlayer *player) {
    s16 updateState;
    long long new_var;
    s32 stateTimer;
    s32 yVel;
    s32 temp;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateTimer = 0x1E;
        if (player->stateFlags & 0x800000) {
            player->stateTimer = 0x5A;
            func_80062A64(player->playerIndex);
        }
        func_80081E40(player, 0xF);
        stateTimer = player->stateTimer;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags &= ~0x200;
        player->stateFlags |= 0x1012000;
        player->unk60 = 0;
        player->stateTimer = stateTimer;
        player->stateTimer = stateTimer - ((player->stateTimer * player->unk509) / 8);
        player->actionEffectLevel = 1;
        player->actionEffectFrame = 0;
    }

    func_8008B408(player, 0, 0);
    if (!(player->stateFlags & 1)) {
        func_8008BB20(player, 0, 0x4000, 0x4000, 0x4000);
    }

    yVel = player->unk40.y - 0xA000;
    player->unk40.y = yVel;
    player->posY += yVel;

    if (!(player->stateFlags & 1)) {
        new_var = func_80084958(player);
        temp = player->stateTimer - new_var;
        stateTimer = temp - 1;
        player->stateTimer = stateTimer;
        if (stateTimer < 0) {
            player->stateTimer = 0;
        }
        if (func_80082EC0(player) != 0) {
            player->stateFlags &= ~0x200;
            if (player->stateTimer == 0) {
                player->mode = 8;
                player->updateState = 0;
                player->updateTimer = 0;
            }
        }
    } else {
        player->stateFlags |= 0x200;
        func_80081E40(player, 0xF);
        func_80082EC0(player);
        if (player->stateTimer < 0x1E) {
            player->stateTimer = 0x1E;
        }
        if (!(player->stateFlags & 1)) {
            player->actionEffectLevel = 1;
            player->actionEffectFrame = 0;
        }
    }
}
#endif

// func_800934EC best match: 99.262%

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_800934EC.s")

#ifdef NON_MATCHING
void func_800934EC(RaceInputPlayer *player) {
    Struct800955C0 *spawn;
    s16 updateState;
    s16 targetAngle;
    s32 tooLow;
    s16 angleDiff;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        if (player->stateFlags & 0x10000) {
            func_80081E40(player, 0x11);
        } else {
            func_80081E40(player, 0x10);
        }
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags &= ~0x200;
        player->stateFlags |= 0xA000;
        player->unk60 = 0x40000;
    }

    if (player->stateFlags & 1) {
        player->stateFlags |= 0x200;
    } else {
        player->stateFlags &= ~0x200;
    }

    spawn = &D_800B9540[D_80121B50];
    if ((spawn->unk0 == player->unk502) && !(player->stateFlags & 0x40)) {
        targetAngle = func_8004940C(player->posX, player->posZ, spawn->unk40, spawn->unk44);
        if (player->stateFlags & 0x400) {
            targetAngle += 0x800;
        }

        angleDiff = (targetAngle - player->facingAngle) & 0xFFF;
        if ((targetAngle - player->facingAngle) >= 0x801) {
            angleDiff -= 0x1000;
        }
        if (angleDiff >= 0x67) {
            angleDiff = 0x66;
        }
        tooLow = angleDiff < -0x66;
        if (tooLow) {
            angleDiff = -0x66;
        }
        player->facingAngle += angleDiff;
    }

    func_8008B408(player, 0, 0);
    player->unk40.y -= 0xA000;
    player->posY += player->unk40.y;

    if (func_80082EC0(player) != 0) {
        player->stateFlags &= 0xFE0C1FFB;
        player->mode = 0;
        player->updateState = 0;
        player->updateTimer = 0;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_800936D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_800939E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_80093E0C.s")

// func_80094288 best match: 94.992%

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_80094288.s")

#ifdef NON_MATCHING
void func_80094288(RaceInputPlayer *player) {
    s16 updateState;
    s32 yVel;
    s32 velocityX;
    s32 velocityZ;
    s32 stateTimer;
    u32 stateFlags;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateFlags &= ~0x200;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x01006000;
        if (player->animationId != 0x12) {
            func_80081E40(player, 0x12);
        }
        player->stateTimer = 0x1E;
        player->unk60 = 0;
    }
    func_80082DD0(player);
    func_8008B408(player, player->unk254, 0);
    player->velocity.y += 0xFFFF6000;
    player->unk314 = 0x80000;
    func_8008B508(&player->velocity, player);
    stateFlags = player->stateFlags;
    if ((stateFlags & 1) == 0) {
        func_8008BB20(player, 0, 0, 0, 0);
        player->unk582 = 0x100;
        player->unk584 = 8;
        player->unk588 = 0.0f;
        stateFlags = player->stateFlags;
    }
    if (stateFlags & 1) {
        player->stateFlags = stateFlags | 0x200;
    } else {
        player->stateFlags = stateFlags & ~0x200;
    }
    yVel = player->velocity.y;
    velocityX = player->velocity.x;
    velocityZ = player->velocity.z;
    player->posY += yVel;
    player->unk74 = 0;
    player->posX += velocityX;
    player->posY += yVel;
    player->posZ += velocityZ;
    player->facingAngle = func_8004908C(velocityX, velocityZ);
    if (player->stateFlags & 0x400) {
        player->facingAngle += 0x800;
    }
    if (player->unk330 != 3) {
        stateTimer = player->stateTimer - 1;
        player->stateTimer = stateTimer;
        if (stateTimer == 0) {
            stateFlags = player->stateFlags & 0xFE0C1FFB;
            player->stateFlags = stateFlags;
            if (stateFlags & 1) {
                player->mode = 1;
                player->updateState = 0;
                player->updateTimer = 0;
            } else {
                player->mode = 0;
                player->updateState = 0;
                player->updateTimer = 0;
            }
        }
    } else {
        player->stateTimer = 0x1E;
    }
    player->actionEffectLevel = 4;
    player->actionEffectFrame = 2;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_80094480.s")

void func_8009469C(RaceInputPlayer *player) {
    s16 updateState;
    s32 tempX;
    s32 tempZ;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        func_80081E40(player, 0x13);
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x22200;
        player->pitchAngle = 0;
        func_8008B408(player, 0, 0);
        player->unk60 = 0;
        player->actionEffectLevel = 3;
        player->actionEffectFrame = 0;
    }

    player->unk314 = 0x60000;
    func_8008B508(&player->unk40, player);
    player->unk40.y -= player->unk264;
    if (!(player->stateFlags & 1)) {
        func_8008BB20(player, 0, 0x4000, 0x4000, 0x4000);
    } else {
        tempX = player->unk40.x;
        tempZ = player->unk40.z;
        player->unk40.x = tempX - (tempX / 16);
        player->unk40.z = tempZ - (tempZ / 16);
    }

    player->posX += player->unk40.x;
    player->posY += player->unk40.y;
    player->posZ += player->unk40.z;

    if ((func_80082EC0(player) != 0) && !(player->stateFlags & 1)) {
        player->mode = 0xC;
        player->updateState = 0;
        player->updateTimer = 0;
    }
}

void func_80094808(RaceInputPlayer *player) {
    s16 updateState;
    s32 timer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        func_80081E40(player, 0x12);
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x6204;
        player->pitchAngle = 0;
        func_8008B408(player, 0, 0);
        player->unk60 = 0;
        player->unk40.y = 0x80000;
        player->stateTimer = 0x14;
    }

    if (D_801235B0 & 1) {
        func_800716A4(func_8004FA44, 5, 2, (u16) player->playerIndex);
    }

    player->unk40.y -= 0x7000;
    player->posY += player->unk40.y;
    func_80082DD0(player);

    timer = player->stateTimer;
    if (timer == 0) {
        if (!(player->stateFlags & 1)) {
            player->mode = 0xC;
            player->updateState = 0;
            player->updateTimer = 0;
        }
    } else {
        player->stateTimer = timer - 1;
    }

    if (player->unk40.y > 0) {
        player->actionEffectLevel = 2;
        player->actionEffectFrame = 3;
    }
}

void func_8009491C(RaceInputPlayer *player) {
    s16 updateState;
    s32 velocityX;
    s32 velocityZ;
    u32 stateFlags;
    s16 nextState;

    updateState = player->updateState;
    nextState = updateState + 1;
    if (updateState == 0) {
        player->updateState = nextState;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x01006000;
        func_80081E40(player, 0x12);
        player->stateTimer = 0xF;
        player->unk60 = 0;
    }
    func_80082DD0(player);
    func_8008B408(player, 0x10000, 0);
    player->velocity.y += 0xFFFF6000;
    func_8008B508(&player->velocity, player);
    stateFlags = player->stateFlags;
    if ((stateFlags & 1) == 0) {
        player->stateFlags = stateFlags & ~0x200;
        func_8008BB20(player, 0, 0, 0, 0);
        player->unk582 = 0x100;
        player->unk584 = 8;
        player->unk588 = 0.0f;
    } else {
        player->stateFlags = stateFlags | 0x200;
    }
    velocityX = player->velocity.x;
    player->posX += velocityX;
    player->posY += player->velocity.y;
    velocityZ = player->velocity.z;
    player->posZ += velocityZ;
    player->facingAngle = func_8004908C(velocityX, velocityZ);
    if (player->stateFlags & 0x400) {
        player->facingAngle += 0x800;
    }
    if (--player->stateTimer == 0) {
        player->mode = 0xC;
        player->updateState = 0;
        player->updateTimer = 0;
    }
    player->actionEffectLevel = 4;
    player->actionEffectFrame = 2;
}

// func_80094A94 best match: 99.884%

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_80094A94.s")

#ifdef NON_MATCHING
void func_80094A94(RaceInputPlayer *player) {
    s16 updateState;
    s16 nextState;
    s32 timer;
    u32 stateFlags;

    updateState = player->updateState;
    nextState = updateState + 1;
    if (updateState == 0) {
        player->updateState = nextState;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x102000;
        func_80081E40(player, 0xE);
        player->stateTimer = 0x5A;
        player->unk60 = 0;
        func_80082DD0(player);
        player->unk40.y = 0;
        func_800716A4(func_8005FB30, 0, 0x64, (u16) player->playerIndex);
        timer = player->stateTimer;
        player->stateTimer = timer - ((player->stateTimer * player->unk509) / 8);
        player->actionEffectLevel = 4;
        player->actionEffectFrame = 0;
    }

    stateFlags = player->stateFlags;
    if (!(stateFlags & 1)) {
        player->stateFlags = stateFlags & ~0x200;
    } else {
        player->stateFlags = stateFlags | 0x200;
    }

    func_8008B408(player, 0, 0);
    player->unk40.y -= 0xA000;
    player->posY += player->unk40.y;

    timer = (player->stateTimer - func_80084958(player) - 1) & 0xFFFFFFFFFFFFFFFFu;
    player->stateTimer = timer;
    if (timer < 0) {
        player->stateTimer = 0;
        timer = 0;
    }

    if (timer == 0) {
        player->mode = 4;
        player->updateState = 0;
        player->updateTimer = 0;
        player->unk2E0 = 0;
        player->unk2E4 = 0;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_80094BEC.s")

// func_80094DF8 best match: 99.921% (nonmatchings/func_80094DF8-4923837976568703863/base_14.c)

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_80094DF8.s")

#ifdef NON_MATCHING
void func_80094DF8(RaceInputPlayer *player) {
    s16 updateState;
    s32 stateTimer;
    u32 stateFlags;
    s16 nextState;

    updateState = player->updateState;
    nextState = updateState + 1;
    if (updateState == 0) {
        player->updateState = nextState;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x400000;
        player->stateTimer = 0xF0;
        func_80081E40(player, 1);
        func_80082DD0(player);
        func_800716A4(func_800617C8, 0, 0x3C, (u16) player->playerIndex);
        stateTimer = player->stateTimer;
        player->stateTimer = stateTimer;
        player->stateTimer = player->stateTimer - ((stateTimer * player->unk509) / 8);
        player->actionEffectLevel = 4;
        player->actionEffectFrame = 0;
    }

    stateFlags = player->stateFlags;
    if (!(stateFlags & 1)) {
        player->stateFlags = stateFlags & ~0x200;
        func_8008B408(player, player->unk254, 0);
        player->unk40.y -= player->unk260;
        func_8008B73C(player, 0, 0, player->unk274, player->unk278, player->unk27C);
    } else {
        player->stateFlags = stateFlags | 0x200;
        player->unk40.y -= player->unk264;
        func_8008B508(&player->unk40, player);
    }

    stateTimer = player->stateTimer - func_80084958(player) - 1;
    player->stateTimer = stateTimer;
    if (stateTimer < 0) {
        player->stateTimer = 0;
        stateTimer = 0;
    }

    if (stateTimer == 0) {
        stateFlags = player->stateFlags;
        updateState = stateFlags;
        if (!(updateState & 0x200)) {
            player->stateFlags = stateFlags & 0xFE0C1FFB;
            player->mode = 0;
            player->updateState = 0;
            player->updateTimer = 0;
        } else {
            player->stateFlags = stateFlags & 0xFE0C1FFB;
            player->mode = 1;
            player->updateState = 0;
            player->updateTimer = 0;
        }
    }

    player->posX += player->unk40.x;
    player->posY += player->unk40.y;
    player->posZ += player->unk40.z;

    if (!(player->stateFlags & 1)) {
        func_8008CF10(player);
    }
}
#endif

void func_80094FF4(RaceInputPlayer *player) {
    s16 updateState;
    s16 updateTimer;
    u32 stateFlags;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        func_80081E40(player, 0x13);
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x822204;
        player->pitchAngle = 0;
        player->unk60 = 0;
        player->unk40.y = 0x80000;
        player->updateTimer = 0;
        player->actionEffectLevel = 4;
        player->actionEffectFrame = 0;
    }

    func_8008B408(player, 0, 0);
    if (player->unk40.y > 0) {
        player->unk40.y -= 0x7000;
    } else {
        updateTimer = player->updateTimer;
        if (updateTimer == 0) {
            player->updateTimer = updateTimer + 1;
            func_800716A4(func_80062530, 0, 3, (u16) player->playerIndex);
        }
        player->facingAngle += 0x40;
        player->unk40.y -= 0x800;
    }

    player->unk40.x -= player->unk40.x >> 6;
    player->unk40.z -= player->unk40.z >> 6;
    player->posX += player->unk40.x;
    player->posY += player->unk40.y;
    player->posZ += player->unk40.z;

    func_80082EC0(player);
    if (player->unk40.y < 0) {
        stateFlags = player->stateFlags;
        if (!(stateFlags & 1)) {
            player->mode = 8;
            player->updateState = 0;
            player->updateTimer = 0;
            player->stateFlags = stateFlags & ~0x200;
        }
    }
}

void func_80095164(RaceInputPlayer *player) {
    s16 updateState;
    s32 velocityX;
    s32 velocityZ;
    u32 stateFlags;
    s16 nextState;

    updateState = player->updateState;
    nextState = updateState + 1;
    if (updateState == 0) {
        player->updateState = nextState;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x01006000;
        func_80081E40(player, 0x12);
        player->stateTimer = 0x1E;
        player->stateTimer += func_800430D0() >> 4;
        player->unk60 = 0;
    }
    func_80082DD0(player);
    func_8008B408(player, 0x10000, 0);
    player->velocity.y += 0xFFFF6000;
    func_8008B508(&player->velocity, player);
    stateFlags = player->stateFlags;
    if ((stateFlags & 1) == 0) {
        player->stateFlags = stateFlags & ~0x200;
        func_8008BB20(player, 0, 0, 0, 0);
        player->stateFlags &= ~0x200;
        player->unk582 = 0x100;
        player->unk584 = 8;
        player->unk588 = 0.0f;
    } else {
        player->stateFlags = stateFlags | 0x200;
    }
    velocityX = player->velocity.x;
    player->posX += velocityX;
    player->posY += player->velocity.y;
    velocityZ = player->velocity.z;
    player->posZ += velocityZ;
    player->facingAngle = func_8004908C(velocityX, velocityZ);
    if (player->stateFlags & 0x400) {
        player->facingAngle += 0x800;
    }
    if (--player->stateTimer == 0) {
        player->mode = 0xC;
        player->updateState = 0;
        player->updateTimer = 0;
    }
    player->actionEffectLevel = 4;
    player->actionEffectFrame = 2;
}

void func_80095300(RaceInputPlayer *player) {
    D_800DECE8[player->updateState](player);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_80095338.s")

void func_800955C0(RaceInputPlayer *player) {
    if (func_80082EC0(player) != 0) {
        player->updateState++;
        player->updateTimer = 0;
        player->facingAngle = player->unk2FA;
        player->posX = D_800B9540[D_80121B50].unk18;
        player->posZ = D_800B9540[D_80121B50].unk1C;
    }
}

void func_80095650(RaceInputPlayer *player) {
    s16 updateTimer;
    s32 unused;
    s32 scratch[14];

    extern void func_8006BB50(EffectTask *);

    updateTimer = player->updateTimer;
    if (updateTimer == 0) {
        player->updateTimer = updateTimer + 1;
        player->unk40.y = 0x30000;
        player->stateTimer = 0x1E;
        func_80081E40(player, 4);
        if (player->stateFlags & 0x400) {
            player->unk80 = 0x20000;
        } else {
            player->unk80 = -0x20000;
        }
    }

    func_80097C18((s16 *)&scratch[2], player->facingAngle);
    scratch[13] = 0;
    scratch[14] = 0;
    scratch[15] = player->unk80;
    func_80098590((s16 *)&scratch[2], (Vec3i *)&scratch[13], (Vec3i *)&scratch[10]);
    player->posX += scratch[10];
    player->posZ += scratch[12];

    func_80097C18((s16 *)&scratch[2], player->facingAngle);
    scratch[13] = 0;
    scratch[14] = 0;
    scratch[15] = player->unk80;
    func_80098590((s16 *)&scratch[2], (Vec3i *)&scratch[13], (Vec3i *)&scratch[10]);
    player->posX += scratch[10];
    player->posZ += scratch[12];
    player->unk40.y -= 0x7000;
    player->posY += player->unk40.y;

    if ((player->unk40.y < 0) && !(player->stateFlags & 1)) {
        if (player->animationId != 1) {
            func_80081E40(player, 1);
        }
        func_80082DD0(player);
        func_8008CF10(player);
    } else {
        func_80082EC0(player);
    }

    if (--player->stateTimer == 0) {
        player->updateState++;
        player->updateTimer = 0;
        player->stateTimer = 0x20;
        func_80071408(func_8006BB50, 0, 0x64);
        player->stateFlags &= ~0x20;
    }
}

void func_80095804(RaceInputPlayer *player) {
    s32 scratch[15];

    player->facingAngle += 0x10;
    if (player->unk80 > 0) {
        player->unk80 -= 0x820;
    } else {
        player->unk80 += 0x820;
    }

    func_80097C18((s16 *)&scratch[1], player->facingAngle);
    scratch[12] = 0;
    scratch[13] = 0;
    scratch[14] = player->unk80;
    func_80098590((s16 *)&scratch[1], (Vec3i *)&scratch[12], (Vec3i *)&scratch[9]);
    player->posX += scratch[9];
    player->posZ += scratch[11];

    player->facingAngle += 0x10;
    if (player->unk80 > 0) {
        player->unk80 -= 0x820;
    } else {
        player->unk80 += 0x820;
    }

    func_80097C18((s16 *)&scratch[1], player->facingAngle);
    scratch[12] = 0;
    scratch[13] = 0;
    scratch[14] = player->unk80;
    func_80098590((s16 *)&scratch[1], (Vec3i *)&scratch[12], (Vec3i *)&scratch[9]);
    player->posX += scratch[9];
    player->posZ += scratch[11];

    func_80082EC0(player);
    func_8008CF10(player);
    if (--player->stateTimer == 0) {
        player->updateTimer = 0;
        player->updateState++;
    }
}

void func_80095940(RaceInputPlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        func_80081E40(player, 0x1D);
        player->stateTimer = 0x13;
    }
    func_80082EC0(player);
    player->stateTimer--;
    if (player->stateTimer == 0) {
        player->updateTimer = 0;
        player->updateState++;
    }
}

void func_800959B4(RaceInputPlayer *player) {
    s16 updateTimer;
    s32 scratch[14];

    updateTimer = player->updateTimer;
    if (updateTimer == 0) {
        player->updateTimer = updateTimer + 1;
        player->stateTimer = 0x13;
        if (player->stateFlags & 0x400) {
            player->unk80 = 0x40000;
        } else {
            player->unk80 = -0x40000;
        }
    }

    func_80097C18((s16 *)&scratch[1], player->facingAngle);
    scratch[12] = 0;
    scratch[13] = 0;
    scratch[14] = player->unk80;
    func_80098590((s16 *)&scratch[1], (Vec3i *)&scratch[12], (Vec3i *)&scratch[9]);
    player->posX += scratch[9];
    player->posZ += scratch[11];

    player->stateTimer--;
    if (player->stateTimer == 0) {
        player->updateTimer = 0;
        player->updateState++;
    }
    func_80082EC0(player);
}

typedef struct {
    /* 0x00 */ char pad0[0xAC];
    /* 0xAC */ s8 active;
    /* 0xAD */ char padAD[3];
} Unk801121E0;

typedef struct {
    /* 0x00 */ u8 matrix[0x20];
    /* 0x20 */ s32 transformedX;
    /* 0x24 */ s32 transformedY;
    /* 0x28 */ s32 transformedZ;
    /* 0x2C */ s32 sourceX;
    /* 0x30 */ s32 sourceY;
    /* 0x34 */ s32 sourceZ;
} PlayerTransformScratch80095A88;

extern Unk801121E0 D_801121E0[];

void func_80095A88(RaceInputPlayer *player) {
    PlayerTransformScratch80095A88 scratch;

    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0x64;
    }

    if (player->unk80 > 0) {
        if (player->unk80 < 0xA0000) {
            player->unk80 += 0x2000;
        }
        func_80097FE4((s16 *) scratch.matrix, -0x100, player->facingAngle);
    } else {
        if (player->unk80 >= -0x9FFFF) {
            player->unk80 -= 0x2000;
        }
        func_80097FE4((s16 *) scratch.matrix, 0x100, ((0, player))->facingAngle);
    }

    scratch.sourceX = 0;
    scratch.sourceY = 0;
    scratch.sourceZ = player->unk80;
    func_80098590((s16 *) scratch.matrix, (Vec3i *) &scratch.sourceX, (Vec3i *) &scratch.transformedX);
    player->posX = player->posX + scratch.transformedX;
    player->posY += scratch.transformedY;
    player->posZ += scratch.transformedZ;

    player->stateTimer--;
    if (player->stateTimer == 0) {
        player->updateTimer = 0;
        player->updateState++;
    }

    if (player->stateTimer < 0x3C) {
        player->stateFlags |= 0x80000;
        if (D_801121E0[(u16) player->playerIndex].active != 0) {
            D_801121E0[(u16) player->playerIndex].active = 2;
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_80095BE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_80095F90.s")

void func_8009617C(RaceInputPlayer *player) {
    D_800DED08[player->updateState](player);
    if (player->unk280 < 0xF0000) {
        player->unk280 += 0x8000;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_800961DC.s")

void func_8009652C(RaceInputPlayer *player) {
    if (D_80121B5F != 0) {
        player->stateTimer = 0;
    }
    if (player->stateTimer == 0) {
        if (D_800EC9C2 == 0) {
            if (player->unk509 == 0) {
                player->updateState = 2;
                player->updateTimer = 0;
            } else {
                player->updateState = 3;
                player->updateTimer = 0;
            }
        } else if (D_80121B5F == 1) {
            player->updateState = 2;
            player->updateTimer = 0;
        } else {
            player->updateState = 3;
            player->updateTimer = 0;
        }
    }
    func_8008B408(player, 0, 0);
    func_80082E48(player);
}

void func_800965E0(RaceInputPlayer *player) {
    func_8008B408(player, 0, 0);
    D_800DED18[player->characterId](player);
}

void func_80096630(RaceInputPlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        func_80081E40(player, 0x23);
    }
    if (func_80082EC0(player)) {
        if (player->animationId == 0x24) {
            player->stateFlags |= 0x8000000;
        }
        if (player->stateTimer == 0) {
            if (player->updateTimer == 1) {
                func_8008BBB8(player, 6);
                player->updateTimer++;
            }
            player->stateTimer = 0x14;
            func_80081E40(player, 0x24);
        } else {
            player->stateTimer--;
        }
    }
}

void func_800966F4(RaceInputPlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        func_80081E40(player, 0x24);
    }
    if (func_80082EC0(player)) {
        if (player->animationId == 0x25) {
            player->stateFlags |= 0x8000000;
        }
        if (player->stateTimer == 0) {
            if (player->updateTimer == 1) {
                func_8008BBB8(player, 6);
                player->updateTimer++;
            }
            func_80081E40(player, 0x25);
        } else {
            player->stateTimer--;
        }
    }
}

void func_800967B0(RaceInputPlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        func_80081E40(player, 0x23);
    }
    if (func_80082EC0(player)) {
        if (player->animationId == 0x24) {
            player->stateFlags |= 0x8000000;
        }
        if (player->stateTimer == 0) {
            if (player->updateTimer == 1) {
                func_8008BBB8(player, 6);
                player->updateTimer++;
            }
            player->stateTimer = 0x14;
            func_80081E40(player, 0x24);
        } else {
            player->stateTimer--;
        }
    }
}

void func_80096874(RaceInputPlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        func_80081E40(player, 0x24);
    }
    if (func_80082EC0(player)) {
        if (player->animationId == 0x25) {
            player->stateFlags |= 0x8000000;
        }
        if (player->stateTimer == 0) {
            if (player->updateTimer == 1) {
                func_8008BBB8(player, 6);
                player->updateTimer++;
            }
            player->stateTimer = 0;
            func_80081E40(player, 0x25);
        } else {
            player->stateTimer--;
        }
    }
}

void func_80096934(RaceInputPlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        func_80081E40(player, 0x24);
    }
    if (func_80082EC0(player)) {
        if (player->animationId == 0x25) {
            player->stateFlags |= 0x8000000;
        }
        if (player->stateTimer == 0) {
            if (player->updateTimer == 1) {
                func_8008BBB8(player, 6);
                player->updateTimer++;
            }
            player->stateTimer = 0x32;
            func_80081E40(player, 0x25);
        } else {
            player->stateTimer--;
        }
    }
}

void func_800969F8(RaceInputPlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        func_8008BBB8(player, 6);
        player->stateTimer = 0x1E;
        func_80081E40(player, 0x23);
    }
    if (func_80082EC0(player)) {
        player->stateFlags |= 0x8000000;
        if (player->stateTimer == 0) {
            player->stateTimer = 0x1E;
            func_80081E40(player, 0x23);
        } else {
            player->stateTimer--;
        }
    }
}

void func_80096A8C(RaceInputPlayer *player) {
    func_8008B408(player, 0, 0);
    D_800DED30[player->characterId](player);
}

void func_80096ADC(RaceInputPlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        func_80081E40(player, 0x25);
    }
    if (func_80082EC0(player)) {
        if (player->stateTimer == 0) {
            player->stateFlags |= 0x8000000;
            player->stateTimer = 0x1E;
            func_80081E40(player, 0x26);
        } else {
            player->stateTimer--;
        }
    }
}

void func_80096B68(RaceInputPlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        func_80081E40(player, 0x26);
    }
    if (func_80082EC0(player)) {
        if (player->stateTimer == 0) {
            player->stateFlags |= 0x8000000;
            player->stateTimer = 0x1E;
            func_80081E40(player, 0x27);
        } else {
            player->stateTimer--;
        }
    }
}

void func_80096BF4(RaceInputPlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        func_80081E40(player, 0x25);
    }
    if (func_80082EC0(player)) {
        if (player->stateTimer == 0) {
            player->stateTimer = 0;
            player->stateFlags |= 0x8000000;
            func_80081E40(player, 0x26);
        } else {
            player->stateTimer--;
        }
    }
}

void func_80096C7C(RaceInputPlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        func_80081E40(player, 0x26);
    }
    if (func_80082EC0(player)) {
        if (player->stateTimer == 0) {
            player->stateFlags |= 0x8000000;
            player->stateTimer = 0;
            func_80081E40(player, 0x27);
        } else {
            player->stateTimer--;
        }
    }
}

void func_80096D04(RaceInputPlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        func_80081E40(player, 0x26);
    }
    if (func_80082EC0(player)) {
        if (player->stateTimer == 0) {
            player->stateFlags |= 0x8000000;
            player->stateTimer = 0;
            func_80081E40(player, 0x27);
        } else {
            player->stateTimer--;
        }
    }
}

void func_80096D8C(RaceInputPlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        func_80081E40(player, 0x24);
    }
    if (func_80082EC0(player)) {
        if (player->stateTimer == 0) {
            if (player->animationId == 0x25) {
                player->stateTimer = 0;
                func_80081E40(player, 0x26);
            } else {
                player->stateFlags |= 0x8000000;
                player->stateTimer = 0x1E;
                func_80081E40(player, 0x25);
            }
        } else {
            player->stateTimer--;
        }
    }
}

// func_80096E3C best match: 99.055% (nonmatchings/func_80096E3C-6182772958467082306/base_12.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_80096E3C.s")

#ifdef NON_MATCHING
void func_80096E3C(void) {
    RaceInputPlayer *player;
    RacePlayerSoundPosition *soundPos;
    RacePlayerSoundPosition *nextSoundPos;
    s32 i;

    if (D_80121B56 == 0) {
        i = 0;
        if (D_80121B54 > 0) {
            do {
                do {
                    func_80097038(&D_80121D80[i]);
                } while (0);
                i++;
            } while (i < D_80121B54);
        }
    }

    i = 0;
    if (D_80121B54 > 0) {
        do {
            func_8008393C(&D_80121D80[i]);
            i++;
        } while (i < D_80121B54);
        i = 0;
    }

    if (D_80121B54 > 0) {
        player = D_80121D80; soundPos = D_80121D9C; nextSoundPos = D_80121DA8; do {
            if (player->soundDisabled == 0) {
                func_80072A20(player->unk584, &soundPos->pos, player->unk582, 0x46, player->unk588, i);
            } else {
                func_80072A20(player->unk584, &soundPos->pos, 0, 0x46, player->unk588, i);
            }

            nextSoundPos->pos = soundPos->pos;
            player->unk2C = player->unk64 + player->unk2C - player->unk58 + 0xA000;
            if (player->soundDisabled == 0) {
                func_800483FC(&D_801248C8, func_8007C5E8, player);
                func_800483FC(&D_801248EC, func_8007BE80, player);
            } else {
                func_800483FC(&D_801248EC, func_8007CBC0, player);
            }
            i++;
            player++;
            soundPos++;
            nextSoundPos++;
        } while (i < D_80121B54);
    }
}
#endif

void func_80097038(RaceInputPlayer *player) {
    unsigned long long random;

    D_800DED48[player->mode](player);
    if (!(D_801235B4 & 1) && !(player->stateFlags & 0x41000) && (player->soundDisabled == 0)) {
        if (player->unk500 & 3) {
            random = func_800430D0();
            func_8004DCA0(&player->unk4A0, &player->unk4AC, &player->unk4D0, &player->unk4DC, random, player->unk330);
        }
        if (player->unk500 & 0xC) {
            random = func_800430D0();
            func_8004DCA0(&player->unk4B8, &player->unk4C4, &player->unk4E8, &player->unk4F4, random, player->unk330);
        }
    }
    func_80087E14(player);
    func_80086170(player);
    func_800830C0(player);
}

extern s32 func_80089374(void);

void func_80097144(void) {
    func_80089374();
}

void func_80097164(RaceInputPlayer *player) {
    if (player->updateState < 7) {
        func_8008A940();
    } else {
        player->unk64 = 0;
        func_8008BE1C(player);
        player->unk58 = 0x30000;
    }
}

void func_800971B8(RaceInputPlayer *player) {
    if (func_80089374()) {
        player->mode = 0x16;
        player->updateState = 0;
        player->updateTimer = 0;
        player->stateFlags |= 0x200;
    } else if (func_80095F90(player->playerIndex)) {
        player->mode = 7;
        player->updateState = 0;
        player->updateTimer = 0;
    }
    func_80082664(player, 0, (0x60000 - player->unk58) / 0x600, 0x100);
}

// func_8009724C best match: 74.373%
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_state/func_8009724C.s")

#ifdef NON_MATCHING
void func_8009724C(RaceInputPlayer *arg0) {
    s32 sp2C;
    s32 sp24;
    s16 temp_v0;
    s16 var_v0;
    s16 var_v1;
    s32 var_a0;
    s32 var_a3_2;
    s32 var_at;
    s32 var_at_2;
    s32 var_s0;
    s32 var_v1_2;
    s32 var_v1_3;
    s32 var_v1_4;
    u16 temp_v0_2;
    s32 var_a3;

    var_v0 = arg0->unk2A6;
    var_a3 = 0;
    var_s0 = 0;
    if (var_v0 >= 9) {
        var_v0 = arg0->unk2A6;
        arg0->unk57B = 1;
    }
    if ((var_v0 != 0) && (arg0->unk2A2 != 0)) {
        var_v0 = arg0->unk2A6;
        arg0->unk57B = 1;
    }
    if (var_v0 != 0) {
        var_a3 = *(&D_800DE8D0 + var_v0);
        var_s0 = 0x28;
        if (var_v0 < 9) {
            var_s0 = 0xA;
        }
        var_v1 = var_v0;
        if (var_v0 >= 0xA) {
            var_v1 = 9;
        }
        arg0->unk2C4 |= 1 << var_v1;
    }
    temp_v0 = arg0->unk2A2;
    if (temp_v0 != 0) {
        if (arg0->unk2A6 != 0) {
            var_a3_2 = var_a3 + *(&D_800DE904 + temp_v0);
            if (temp_v0 & 1) {
                var_s0 += 5;
            } else {
                var_s0 += 0xA;
            }
        } else {
            var_s0 += 1;
            var_a3_2 = var_a3 + *(&D_800DE8F0 + temp_v0);
        }
        var_a3 = var_a3_2 + arg0->unk2A4;
    }
    if ((var_a3 != 0) && (D_800EC9C2 == 0) && (D_80121B59 == 0)) {
        sp24 = (s32)var_a3;
        func_8008BB5C(arg0, (s32)var_a3);
        temp_v0_2 = (u16)arg0->playerIndex;
        if (D_8011228C[temp_v0_2].active != 0) {
            sp24 = (s32)var_a3;
            func_80061034((s32)var_a3, (s16)temp_v0_2);
            var_v1_2 = 0x4D;
            if ((s32)var_a3 >= 0x64) {
                var_v1_2 = 0x4E;
            }
            if ((s32)var_a3 >= 0x78) {
                var_v1_2 = 0x4F;
            }
            var_at = (s32)var_a3 < 0x12C;
            if ((s32)var_a3 >= 0xC8) {
                var_v1_2 = 0x51;
                var_at = (s32)var_a3 < 0x12C;
            }
            var_a0 = var_v1_2 << 0x10;
            if (var_at == 0) {
                var_a0 = 0x50 << 0x10;
            }
            func_80072138(var_a0 >> 0x10, 0x5A);
        }
    }
    if (D_800EC9C2 == 1) {
        if (D_80121B5E == 2) {
            if (var_s0 != 0) {
                var_v1_3 = 0x4D;
                if (var_s0 >= 0x2710) {
                    var_s0 = 0x270F;
                }
                arg0->unk2C3 += 1;
                arg0->unk2C0 += var_s0;
                if (arg0->unk2C3 >= 0x64) {
                    arg0->unk2C3 = 0x63;
                }
                var_at_2 = var_s0 < 0xA;
                if (arg0->unk2C2 < arg0->unk2C3) {
                    arg0->unk2C2 = arg0->unk2C3;
                    var_at_2 = var_s0 < 0xA;
                }
                if (var_at_2 == 0) {
                    var_v1_3 = 0x4E;
                }
                if (var_s0 >= 0xF) {
                    var_v1_3 = 0x4F;
                }
                if (var_s0 >= 0x14) {
                    var_v1_3 = 0x51;
                }
                if (var_s0 >= 0x28) {
                    var_v1_3 = 0x50;
                }
                if (arg0->unk2C4 == 0x3FE) {
                    arg0->unk2C4 = 0x3FF;
                    arg0->unk2C0 += 0x12C;
                    sp2C = 0x51;
                    func_80057C08(var_s0, 1);
                    var_v1_4 = 0x51;
                } else {
                    sp2C = var_v1_3;
                    func_80057C08(var_s0, 0);
                    var_v1_4 = var_v1_3;
                }
                arg0 = arg0;
                func_80072138((s32)(s16)var_v1_4, 0x5A);
                if (arg0->unk2C0 >= 0x2710) {
                    arg0->unk2C0 = 0x270F;
                }
            }
        }
    }
}
#endif

void func_8009759C(RaceInputPlayer *player) {
    s32 v0 = -player->unk44 >> 13;
    if (v0 >= 0x80) {
        v0 = 0x7F;
    }
    if (v0 < 0x46) {
        v0 = 0x46;
    }
    if (v0 >= 0x51) {
        player->rumbleTimer = 2;
        player->rumblePatternId = 0;
    } else {
        player->rumbleTimer = 1;
        player->rumblePatternId = 0;
    }
    if (player->soundDisabled == 0) {
        func_80072A74(0x21, (SoundPosition *)&player->posX, (s16)v0, 0x32);
    }
}

void func_8009762C(RaceInputPlayer *player) {
    u32 stateFlags;

    if (func_80089374() == 0) {
        stateFlags = player->stateFlags;
        if (stateFlags & 8) {
            player->stateFlags = stateFlags & ~8;
        } else if (stateFlags & 0x800) {
            player->unk2C3 = 0;
            player->mode = 0xE;
            player->updateState = 0;
            player->updateTimer = 0;
            player->stateFlags = stateFlags & ~0x200;
            func_8008BBB8(player, 2);
        } else {
            player->stateFlags = stateFlags & ~0x200;
            if (func_80095F90(player->playerIndex) != 0) {
                player->mode = 7;
                player->updateState = 0;
                player->updateTimer = 0;
                func_8009724C(player);
                func_8009759C(player);
            } else if (player->subState == 0) {
                player->mode = 0;
                player->updateState = 1;
                player->updateTimer = 0;
                player->unk57A = 2;
                func_8009759C(player);
            } else {
                player->mode = 0;
                player->updateState = 0;
                player->updateTimer = 0;
                player->unk57A = 2;
                func_8009724C(player);
                func_8009759C(player);
            }
        }
    }
}

void func_80097744(RaceInputPlayer *player) {
    u32 stateFlags;

    if (func_80089374() == 0) {
        stateFlags = player->stateFlags;
        if (stateFlags & 8) {
            player->stateFlags = stateFlags & ~8;
        } else if (stateFlags & 0x800) {
            player->unk2C3 = 0;
            player->mode = 0xE;
            player->updateState = 0;
            player->updateTimer = 0;
            player->stateFlags = stateFlags & ~0x200;
            func_8008BBB8(player, 2);
        } else {
            player->stateFlags = stateFlags & ~0x200;
            if (func_80095F90(player->playerIndex) != 0) {
                player->mode = 7;
                player->updateState = 0;
                player->updateTimer = 0;
                func_8009724C(player);
                func_8009759C(player);
            } else {
                player->mode = 0;
                player->updateState = 0;
                player->updateTimer = 0;
                player->unk57A = 2;
                func_8009724C(player);
                func_8009759C(player);
            }
        }
    }
}

void func_8009782C(RaceInputPlayer *player) {
    if (func_80089374()) {
        player->mode = 0x16;
        player->updateState = 0;
        player->updateTimer = 0;
        player->stateFlags |= 0x200;
    } else if (func_80095F90(player->playerIndex)) {
        player->mode = 7;
        player->updateState = 0;
        player->updateTimer = 0;
    }
    func_80082664(player, 0, (0x60000 - player->unk58) / 0x600, 0x100);
}

void func_800978C0(RaceInputPlayer *player) {
    if (func_80089374()) {
        player->stateFlags |= 0x200;
    } else {
        player->stateFlags &= ~0x200;
    }
}

void func_80097910(RaceInputPlayer *player) {
    u16 playerIndex;

    func_80089374();
    if (!(player->stateFlags & 0x02000000)) {
        player->mode = 1;
        player->updateState = 0;
        player->updateTimer = 0;
        if ((D_800EC9C2 == 1) && (D_80121B5E == 2)) {
            player->unk2C0 *= 2;
            if (player->unk2C0 >= 0x2710) {
                player->unk2C0 = 0x270F;
            }
            func_800711D0(func_80057DD4, 0, 0x64);
            func_80072138(0x51, 0x32);
        }
        if (D_800EC9C2 == 0) {
            func_8008BB5C(player, 0x12C);
            playerIndex = player->playerIndex;
            if (D_8011228C[playerIndex].active != 0) {
                func_80061034(0x12C, (s16)playerIndex);
                func_80072138(0x51, 0x32);
            }
        }
    }
}

void func_80097A14(RaceInputPlayer *player) {
    func_80089374();
    if (player->updateState < 2) {
        func_80082664(player, 0, (0x60000 - player->unk58) / 0x600, 0x100);
    }
}
