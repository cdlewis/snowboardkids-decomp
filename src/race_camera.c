#include "common.h"
#include "fixed_point_matrix.h"

#define RACE_CAMERA_COUNT 4
#define RACE_CAMERA_STATE_SIZE 0xB0
#define RACE_PLAYER_STATE_SIZE 0x60C

typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ Vec3i pos;
    /* 0x028 */ u8 pad28[0x290 - 0x28];
    /* 0x290 */ Vec3i cameraPos;
    /* 0x29C */ u8 pad29C[0x2EC - 0x29C];
    /* 0x2EC */ s16 yaw;
    /* 0x2EE */ u8 pad2EE[0x2FC - 0x2EE];
    /* 0x2FC */ u32 flags2FC;
    /* 0x300 */ u8 pad300[RACE_PLAYER_STATE_SIZE - 0x300];
} RacePlayerState;

typedef union {
    RacePlayerState state;
    u8 bytes[RACE_PLAYER_STATE_SIZE];
} RacePlayerSlot;

typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ Vec3i cameraPos;
    /* 0x38 */ u8 pad38[0x48 - 0x38];
} CourseSpawnEntry;

typedef struct {
    /* 0x00 */ u16 playerIndex;
    /* 0x02 */ s16 mode;
    /* 0x04 */ s16 pitch;
    /* 0x06 */ s16 yaw;
    /* 0x08 */ s16 roll;
    /* 0x0A */ u8 padA[2];
    /* 0x0C */ Vec3i focus;
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ s32 distance;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ void (*update)(void);
    /* 0x30 */ u8 rotationMatrix[0x14];
    /* 0x44 */ Vec3i transformOffset;
    /* 0x50 */ u8 transform[0x42];
    /* 0x92 */ s16 unk92;
    /* 0x94 */ Vec3i prevPos;
    /* 0xA0 */ s16 timer;
    /* 0xA2 */ u8 padA2[2];
    /* 0xA4 */ s32 velocity;
    /* 0xA8 */ s32 unkA8;
    /* 0xAC */ u8 initialized;
    /* 0xAD */ u8 padAD[3];
} RaceCamera;

typedef struct {
    /* 0x00 */ s16 playerIndex;
    /* 0x02 */ s16 duration;
    /* 0x04 */ Vec3i startPos;
    /* 0x10 */ Vec3i endPos;
} RaceCameraTransition;

typedef struct StackD7D4 {
    char sp28[0x20];
    s32 sp48;
    s32 sp4C;
    s32 sp50;
    s32 sp54;
    s32 sp58;
    s32 sp5C;
} StackD7D4;

extern void func_8006D8B4();
void func_8006E534(void);
extern void func_8006ECBC(void);
extern void func_8006EF1C(void);
extern void func_8006F048(void);
extern void func_8006F984(void);
extern void func_8006FA20(void);
void func_8006FE88(void);
extern void func_8007022C(void);
void func_8007031C(void);
extern void func_800704C4(void);
extern void func_800486BC(void *, void *);
extern s16 func_8007D200(s32, s32, s32);
extern s32 func_80080CC4(s16, s32, s32);
extern RaceCamera D_801121E0[RACE_CAMERA_COUNT];
extern RaceCamera *D_801124A0;
extern RacePlayerSlot D_80121D80[];
extern CourseSpawnEntry D_800B9540[];
extern u8 D_800DAA3C[];
extern void *D_800DA880[];
extern s32 D_80121B40;
extern s32 D_80121B44;
extern s32 D_80121B48;
extern s16 D_80121B50;
extern s8 D_80121B54;
extern u8 D_80121B56;
extern u8 D_80121B58;

void func_8006D520(u16 arg0, u16 arg1) {
    RaceCamera *temp;

    if (D_80121B58 == 0) {
        temp = &D_801121E0[arg0];
        temp->mode = arg1;
        temp->update = D_800DA880[arg1];
    }
}

void func_8006D580(u16 arg0, u16 arg1) {
    RaceCamera *temp = &D_801121E0[arg0];

    temp->mode = arg1;
    temp->update = D_800DA880[arg1];
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006D5CC.s")

void func_8006D700(void) {
    RaceCamera *camera;
    s32 index;

    index = 0;
    if (D_80121B54 > 0) {
 camera = D_801121E0; do {
            (D_801124A0 = camera)->update();
            index += 1;
            camera += 1;
        } while (index < D_80121B54);
    }
}

void func_8006D780(s32 arg0) {
    D_801124A0 = &D_801121E0[arg0];
    D_801124A0->update();
}

void func_8006D7D4(void) {
    StackD7D4 stack;

    func_8009853C(D_801124A0->rotationMatrix, -D_801124A0->pitch, -D_801124A0->yaw);
    stack.sp54 = 0;
    stack.sp58 = 0;
    stack.sp5C = -D_801124A0->distance;
    func_80097FE4(stack.sp28, D_801124A0->pitch, D_801124A0->yaw);
    func_80098590(stack.sp28, &stack.sp54, &stack.sp48);
    D_801124A0->transformOffset.x = stack.sp48 - D_801124A0->pos.x;
    D_801124A0->transformOffset.y = stack.sp4C - D_801124A0->pos.y;
    D_801124A0->transformOffset.z = stack.sp50 - D_801124A0->pos.z;
    func_800486BC(stack.sp28, D_801124A0->transform);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006D8B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006DDB4.s")

void func_8006E2B4(void) {
}

void func_8006E2BC(void) {
}

void func_8006E2C4(void) {
    s16 matrix[0x10];
    Vec3i offset;
    Vec3i transformedOffset;
    RacePlayerSlot *player;

    offset.y = 0;
    offset.x = 0;
    offset.z = 0x400000;
    player = &D_80121D80[D_801124A0->playerIndex];
    if (player->state.flags2FC & 0x400) {
        offset.z = 0xFFC00000;
        player = (RacePlayerSlot *) ((u8 *) D_80121D80 + (D_801124A0->playerIndex * RACE_PLAYER_STATE_SIZE));
    }
    func_80097C18(matrix, player->state.yaw);
    func_80098590(matrix, &offset, &transformedOffset);
    D_801124A0->focus.x = D_80121D80[D_801124A0->playerIndex].state.cameraPos.x;
    D_801124A0->focus.y = D_80121D80[D_801124A0->playerIndex].state.cameraPos.y;
    D_801124A0->focus.z = D_80121D80[D_801124A0->playerIndex].state.cameraPos.z;
    D_801124A0->pos.x = D_80121D80[D_801124A0->playerIndex].state.cameraPos.x + transformedOffset.x;
    D_801124A0->pos.y = D_80121D80[D_801124A0->playerIndex].state.cameraPos.y + transformedOffset.y;
    D_801124A0->pos.z = D_80121D80[D_801124A0->playerIndex].state.cameraPos.z + transformedOffset.z;
    D_801124A0->prevPos.x = D_80121D80[D_801124A0->playerIndex].state.pos.x;
    D_801124A0->prevPos.y = D_80121D80[D_801124A0->playerIndex].state.pos.y;
    D_801124A0->prevPos.z = D_80121D80[D_801124A0->playerIndex].state.pos.z;
    D_801124A0->distance = 0;
    D_801124A0->unk28 = 0x012C0000;
    D_801124A0->update = func_8006E534;
    D_801124A0->update();
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006E534.s")

void func_8006EC64(void) {
}

void func_8006EC6C(void) {
}

void func_8006EC74(void) {
    D_801124A0->update = func_8006ECBC;
    D_801124A0->update();
}

void func_8006ECBC(void) {
    s32 diff;

    if (D_80121B56 == 0) {
        D_801124A0->focus.x += (D_80121D80[D_801124A0->playerIndex].state.cameraPos.x - D_801124A0->focus.x) >> 1;
        D_801124A0->focus.y += (D_80121D80[D_801124A0->playerIndex].state.cameraPos.y - D_801124A0->focus.y) >> 1;
        D_801124A0->focus.z += (D_80121D80[D_801124A0->playerIndex].state.cameraPos.z - D_801124A0->focus.z) >> 1;

        diff = 0x640000 - D_801124A0->unk28;
        if (diff >= 0x10001) {
            diff = 0x10000;
        }
        if (diff < -0x10000) {
            diff = -0x10000;
        }
        D_801124A0->unk28 += diff;

        diff = D_800B9540[D_80121B50].cameraPos.x - D_801124A0->pos.x;
        if (diff >= 0xC001) {
            diff = 0xC000;
        }
        if (diff < -0xC000) {
            diff = -0xC000;
        }
        D_801124A0->pos.x += diff;

        diff = D_800B9540[D_80121B50].cameraPos.y - D_801124A0->pos.y;
        if (diff >= 0xC001) {
            diff = 0xC000;
        }
        if (diff < -0xC000) {
            diff = -0xC000;
        }
        D_801124A0->pos.y += diff;

        diff = D_800B9540[D_80121B50].cameraPos.z - D_801124A0->pos.z;
        if (diff >= 0xC001) {
            diff = 0xC000;
        }
        if (diff < -0xC000) {
            diff = -0xC000;
        }
        D_801124A0->pos.z += diff;
    }
    func_8006D8B4();
}

void func_8006EED4(void) {
    D_801124A0->update = func_8006EF1C;
    D_801124A0->update();
}

void func_8006EF1C(void) {
    if (D_80121B56 == 0) {
        D_801124A0->focus.x += (D_80121D80[D_801124A0->playerIndex].state.cameraPos.x - D_801124A0->focus.x) >> 1;
        D_801124A0->focus.y += (D_80121D80[D_801124A0->playerIndex].state.cameraPos.y - D_801124A0->focus.y) >> 1;
        D_801124A0->focus.z += (D_80121D80[D_801124A0->playerIndex].state.cameraPos.z - D_801124A0->focus.z) >> 1;
    }
    func_8006D8B4();
}

void func_8006EFF4(void) {
    D_801124A0->timer = 0x96;
    D_801124A0->update = func_8006F048;
    D_801124A0->update();
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006F048.s")

void func_8006F5B0(void) {
    s32 unused[3];
    s32 dz;
    s32 unused2;
    s32 dist;
    s32 sine;
    s32 cosine;
    s32 dx;

    if (D_80121B56 == 0) {
        D_801124A0->focus.x += (D_80121D80[D_801124A0->playerIndex].state.cameraPos.x - D_801124A0->focus.x) >> 1;
        D_801124A0->focus.y += (D_80121D80[D_801124A0->playerIndex].state.cameraPos.y - D_801124A0->focus.y) >> 1;
        D_801124A0->focus.z += (D_80121D80[D_801124A0->playerIndex].state.cameraPos.z - D_801124A0->focus.z) >> 1;

        dx = D_801124A0->pos.x - D_801124A0->focus.x;
        dz = D_801124A0->pos.z - D_801124A0->focus.z;
        dist = func_80098C30((s64)dx * dx + (s64)dz * dz);
        dist = ((0x400000 - dist) >> 4) + dist;

        D_801124A0->unk92 += 8;
        sine = func_80097AE8(D_801124A0->unk92);
        cosine = func_80097B48(D_801124A0->unk92);

        D_801124A0->pos.x = (((s64)sine * -dist) / 0x1000) + D_801124A0->focus.x;
        D_801124A0->pos.z = (((s64)cosine * -dist) / 0x1000) + D_801124A0->focus.z;
        D_801124A0->pos.y += ((D_801124A0->focus.y - D_801124A0->pos.y) + 0x100000) >> 4;
        D_801124A0->unk28 += (0x960000 - D_801124A0->unk28) >> 1;
    }

    D_801124A0->prevPos.x = D_80121D80[D_801124A0->playerIndex].state.pos.x;
    D_801124A0->prevPos.y = D_80121D80[D_801124A0->playerIndex].state.pos.y;
    D_801124A0->prevPos.z = D_80121D80[D_801124A0->playerIndex].state.pos.z;
    func_8006D8B4();
}

void func_8006F8AC(void) {
}

void func_8006F8B4(void) {
}

void func_8006F8BC(void) {
    D_801124A0->pos.x = D_80121B40;
    D_801124A0->prevPos.x = D_801124A0->pos.x;
    D_801124A0->pos.y = D_80121B44;
    D_801124A0->prevPos.y = D_801124A0->pos.y;
    D_801124A0->pos.z = D_80121B48;
    D_801124A0->prevPos.z = D_801124A0->pos.z;
    D_801124A0->distance = 0;
    D_801124A0->unk28 = 0;
    D_801124A0->update = func_8006F984;
    D_801124A0->update();
}

void func_8006F984(void) {
    s32 stride = RACE_PLAYER_STATE_SIZE;

    D_801124A0->focus.x = ((RacePlayerState *)((u8 *)D_80121D80 + D_801124A0->playerIndex * stride))->cameraPos.x;
    D_801124A0->focus.y = ((RacePlayerState *)((u8 *)D_80121D80 + D_801124A0->playerIndex * stride))->cameraPos.y;
    D_801124A0->focus.z = ((RacePlayerState *)((u8 *)D_80121D80 + D_801124A0->playerIndex * stride))->cameraPos.z;
    func_8006D8B4(D_80121D80, stride);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006FA20.s")

void func_8006FDC0(void) {
    D_801124A0->roll = 0;
    D_801124A0->distance = 0;
    D_801124A0->update = func_8006FA20;
    D_801124A0->timer = 0;
    D_801124A0->update();
}

void func_8006FE24(void) {
    D_801124A0->distance = 0;
    D_801124A0->unk28 = 0;
    D_801124A0->timer = 0;
    D_801124A0->update = func_8006FE88;
    D_801124A0->update();
}

// func_8006FE88 best match: 99.490%
#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006FE88.s")

#ifdef NON_MATCHING
void func_8006FE88(void) {
    register s32 stride;
    register s32 transitionBase;
    RaceCameraTransition *transition;
    RaceCamera *camera;
    s16 timer;
    s16 duration;

    transitionBase = (s32) D_800DAA3C;
    stride = sizeof(RaceCameraTransition);
    camera = D_801124A0;
    timer = camera->timer;
    transition = (RaceCameraTransition *)((u8 *)transitionBase + ((u16) camera->mode * stride));
    duration = transition[-16].duration;
    if (duration < timer) {
        timer = duration;
    }

#define TRANSITION (((RaceCameraTransition *)((u8 *)transitionBase + ((u16) D_801124A0->mode * stride)))[-16])

    D_801124A0->pos.x = (((s64)(transition[-16].endPos.x - transition[-16].startPos.x) * timer) / TRANSITION.duration) + TRANSITION.startPos.x;
    D_801124A0->pos.y = (((s64)(TRANSITION.endPos.y - TRANSITION.startPos.y) * timer) / TRANSITION.duration) + TRANSITION.startPos.y;
    D_801124A0->pos.z = (((s64)(TRANSITION.endPos.z - TRANSITION.startPos.z) * timer) / TRANSITION.duration) + TRANSITION.startPos.z;

    D_801124A0->focus.x = D_80121D80[TRANSITION.playerIndex].state.cameraPos.x;
    D_801124A0->focus.y = D_80121D80[TRANSITION.playerIndex].state.cameraPos.y;
    D_801124A0->focus.z = D_80121D80[TRANSITION.playerIndex].state.cameraPos.z;
    D_801124A0->prevPos.x = D_801124A0->pos.x;
    D_801124A0->prevPos.y = D_801124A0->pos.y;
    D_801124A0->prevPos.z = D_801124A0->pos.z;
    D_801124A0->timer++;
    func_8006D8B4();

#undef TRANSITION
}
#endif

void func_80070198(void) {
    D_801124A0->pitch = 0;
    D_801124A0->yaw = 0;
    D_801124A0->roll = 0;
    D_801124A0->pos.x = 0;
    D_801124A0->pos.y = 0x310000;
    D_801124A0->pos.z = 0x900000;
    D_801124A0->distance = 0x670000;
    D_801124A0->update = func_8007022C;
    D_801124A0->update();
}

void func_8007022C(void) {
    func_8006D7D4();
}

void func_8007024C(void) {
    RacePlayerState *player = &D_80121D80[0].state;

    D_801124A0->pos.x = 0;
    D_801124A0->pos.y = 0;
    D_801124A0->pos.z = player->pos.z + 0xFE600000;
    D_801124A0->focus.x = player->pos.x;
    D_801124A0->focus.y = player->pos.y + 0xF0000;
    D_801124A0->focus.z = player->pos.z;
    D_801124A0->timer = 0;
    D_801124A0->distance = 0;
    D_801124A0->unk28 = 0;
    D_801124A0->velocity = 0x10000;
    D_801124A0->update = func_8007031C;
    D_801124A0->update();
}

void func_8007031C(void) {
    s16 angle;
    s32 velocity;

    angle = D_801124A0->timer;
    if (angle < 0x400) {
        D_801124A0->timer = angle + 0x10;
        angle = D_801124A0->timer;
    }
    D_801124A0->unkA8 = (-func_80097AE8(angle) * 0xC00) + 0xC00000;

    velocity = D_801124A0->velocity;
    if (velocity < 0x80000) {
        D_801124A0->velocity = velocity + 0x8000;
        velocity = D_801124A0->velocity;
    }

    D_801124A0->pos.z += velocity;
    D_801124A0->pos.y = func_80080CC4(func_8007D200(0, D_801124A0->pos.x, D_801124A0->pos.z), D_801124A0->pos.x, D_801124A0->pos.z) + D_801124A0->unkA8 + 0xE0000;
    func_8006D8B4();
}

void func_8007042C(void) {
    D_801124A0->pitch = 0x70;
    D_801124A0->yaw = 0;
    D_801124A0->roll = 0;
    D_801124A0->pos.x = 0;
    D_801124A0->pos.y = 0x490000;
    D_801124A0->pos.z = 0x900000;
    D_801124A0->distance = 0x500000;
    D_801124A0->update = func_800704C4;
    D_801124A0->update();
}

void func_800704C4(void) {
    func_8006D7D4();
}
