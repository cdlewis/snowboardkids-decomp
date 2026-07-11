#include "common.h"
#include "fixed_point_math.h"
#include "model_animation.h"
#include "race_camera.h"

#define RACE_CAMERA_COUNT 4
#define RACE_CAMERA_STATE_SIZE 0xB0
#define RACE_CAMERA_ANGLE_MASK 0xFFF
#define RACE_CAMERA_FP_ONE 0x1000
#define RACE_CAMERA_ROTATION_TRANSITION_STRIDE 0x24
#define RACE_CAMERA_ROTATION_TRANSITION ((RaceCameraRotationTransition *)((u8 *)D_800DA91C + ((u16)D_801124A0->mode * stride)))
#define RACE_PLAYER_STATE_SIZE 0x60C
#define FIXED_MUL(a, b) (((a) * (b)) / 0x1000)
#define RACE_CAMERA_FP_DOT(a, b, c, d, e, f) \
    (((a) * (b)) / RACE_CAMERA_FP_ONE + ((c) * (d)) / RACE_CAMERA_FP_ONE + ((e) * (f)) / RACE_CAMERA_FP_ONE)

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
    /* 0x02 */ u16 mode;
    /* 0x04 */ s16 pitch;
    /* 0x06 */ s16 yaw;
    /* 0x08 */ s16 roll;
    /* 0x0A */ u8 padA[2];
    /* 0x0C */ Vec3i focus;
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ s32 distance;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ void (*update)(void);
    /* 0x30 */ FixedMatrix3s rotationMatrix;
    /* 0x42 */ s16 pad42;
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

typedef struct {
    /* 0x00 */ s16 duration;
    /* 0x02 */ s16 pad2;
    /* 0x04 */ Vec3i startPos;
    /* 0x10 */ s16 startPitch;
    /* 0x12 */ s16 startYaw;
    /* 0x14 */ Vec3i endPos;
    /* 0x20 */ s16 endPitch;
    /* 0x22 */ s16 endYaw;
} RaceCameraRotationTransition;

typedef struct StackD7D4 {
    char sp28[0x20];
    s32 sp48;
    s32 sp4C;
    s32 sp50;
    s32 sp54;
    s32 sp58;
    s32 sp5C;
} StackD7D4;

extern void func_800486BC(void *, void *);
extern RaceCamera D_801121E0[RACE_CAMERA_COUNT];
extern RaceCamera *D_801124A0;
extern RacePlayerSlot D_80121D80[];
extern CourseSpawnEntry D_800B9540[];
extern u8 D_800DA91C[];
extern s16 D_800DA900[];
extern u8 D_800DA914[];
extern RaceCameraTransition D_800DAA3C[];
extern void *D_800DA880[];
extern s32 D_80121B40;
extern s32 D_80121B44;
extern s32 D_80121B48;
extern s16 D_80121B50;
extern s8 gRacePlayerCount;
extern u8 gRaceUpdatePaused;
extern u8 D_80121B58;
extern s32 D_801235B4;

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

void func_8006D5CC(void) {
    RaceCamera *camera;

    func_8006D520(0, 0);
    camera = D_801121E0;
    camera->initialized = 0;
    camera->pitch = 0;
    camera->yaw = 0;
    camera->roll = 0;
    camera->pos.x = 0;
    camera->pos.y = 0;
    camera->pos.z = 0;
    camera->distance = 0;
    camera->unk28 = 0;
    D_801121E0[1] = D_801121E0[0];
    D_801121E0[2] = D_801121E0[0];
    D_801121E0[3] = D_801121E0[0];
    D_801121E0[0].playerIndex = 0;
    D_801121E0[1].playerIndex = 1;
    D_801121E0[2].playerIndex = 2;
    D_801121E0[3].playerIndex = 3;
}

void func_8006D700(void) {
    RaceCamera *camera;
    s32 index;

    index = 0;
    if (gRacePlayerCount > 0) {
 camera = D_801121E0; do {
            (D_801124A0 = camera)->update();
            index += 1;
            camera += 1;
        } while (index < gRacePlayerCount);
    }
}

void func_8006D780(s32 arg0) {
    D_801124A0 = &D_801121E0[arg0];
    D_801124A0->update();
}

void func_8006D7D4(void) {
    StackD7D4 stack;

    makeFixedRotationYX(D_801124A0->rotationMatrix, -D_801124A0->pitch, -D_801124A0->yaw);
    stack.sp54 = 0;
    stack.sp58 = 0;
    stack.sp5C = -D_801124A0->distance;
    makeFixedRotationXY(stack.sp28, D_801124A0->pitch, D_801124A0->yaw);
    transformVec3iByFixedMatrix(stack.sp28, &stack.sp54, &stack.sp48);
    D_801124A0->transformOffset.x = stack.sp48 - D_801124A0->pos.x;
    D_801124A0->transformOffset.y = stack.sp4C - D_801124A0->pos.y;
    D_801124A0->transformOffset.z = stack.sp50 - D_801124A0->pos.z;
    func_800486BC(stack.sp28, D_801124A0->transform);
}

// func_8006D8B4 best match: 98.219% (nonmatchings/func_8006D8B4-6061209858023118177/base_14.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006D8B4.s")

#ifdef NON_MATCHING
void func_8006D8B4(void) {
    s32 dx;
    s32 dy;
    s32 dz;
    s32 xzDist;
    s32 dist;
    s32 sine;
    s32 cosine;
    FixedTransform pitchMtx;
    FixedTransform yawMtx;
    s32 pad[3];
    FixedMatrix3s resultMtx;
    s32 i;
    s32 j;

    initFixedTransform(&pitchMtx);
    initFixedTransform(&yawMtx);
    dx = D_801124A0->pos.x - D_801124A0->focus.x;
    pad[0] = pad[0];

    dy = (D_801124A0->pos.y - D_801124A0->focus.y) + 0x40000;
    dz = D_801124A0->pos.z - D_801124A0->focus.z;

    xzDist = integerSquareRoot64((s64)dx * dx + (s64)dz * dz);
    dist = integerSquareRoot64((s64)xzDist * xzDist + (s64)dy * dy);

    D_801124A0->pitch = calculateAngleBetweenXZPoints(0, 0, xzDist, -dy);
    if (dist != 0) {
        sine = ((s64)dy * 0x1000) / dist;
        cosine = ((s64)xzDist * 0x1000) / dist;
        pitchMtx.rotation[MTX_YY] = cosine;
        pitchMtx.rotation[MTX_ZY] = -sine;
        pitchMtx.rotation[MTX_ZZ] = cosine;
        pitchMtx.rotation[MTX_YZ] = sine;
    }

    D_801124A0->yaw = -calculateAngleBetweenXZPoints(0, 0, dx, dz);
    if (xzDist != 0) {
        sine = ((s64)dx * 0x1000) / xzDist;
        cosine = ((s64)dz * 0x1000) / xzDist;
        yawMtx.rotation[MTX_XX] = cosine;
        yawMtx.rotation[MTX_XZ] = sine;
        yawMtx.rotation[MTX_ZX] = -sine;
        yawMtx.rotation[MTX_ZZ] = cosine;
    }

    for (i = 0; i + 1 <= 3; i++) {
        for (j = 0; j < 3; j++) {
            D_801124A0->rotationMatrix[(i * 3) + j] =
                FIXED_MUL(pitchMtx.rotation[j], yawMtx.rotation[i * 3]) +
                FIXED_MUL(pitchMtx.rotation[j + 3], yawMtx.rotation[(i * 3) + 1]) +
                FIXED_MUL(pitchMtx.rotation[j + 6], yawMtx.rotation[(i * 3) + 2]);
        }
    }

    pitchMtx.rotation[MTX_YZ] *= -1;
    pitchMtx.rotation[MTX_ZY] *= -1;
    yawMtx.rotation[MTX_XZ] *= -1;
    yawMtx.rotation[MTX_ZX] *= -1;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            resultMtx[(i * 3) + j] =
                FIXED_MUL(yawMtx.rotation[j], pitchMtx.rotation[i * 3]) +
                FIXED_MUL(yawMtx.rotation[j + 3], pitchMtx.rotation[(i * 3) + 1]) +
                FIXED_MUL(yawMtx.rotation[j + 6], pitchMtx.rotation[(i * 3) + 2]);
        }
    }

    D_801124A0->transformOffset.x =
        -((((s64)resultMtx[MTX_YX] * D_801124A0->unk28) / 0x10000) + D_801124A0->pos.x);
    D_801124A0->transformOffset.y =
        -((((s64)resultMtx[MTX_YY] * D_801124A0->unk28) / 0x10000) + D_801124A0->pos.y);
    D_801124A0->transformOffset.z =
        -((((s64)resultMtx[MTX_YZ] * D_801124A0->unk28) / 0x10000) + D_801124A0->pos.z);
    func_800486BC(resultMtx, D_801124A0->transform);
}
#endif

// func_8006DDB4 best match: 92.325% (base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006DDB4.s")

#ifdef NON_MATCHING
void func_8006DDB4(void) {
    s32 dx;
    s32 dy;
    s32 dz;
    s32 xzLen;
    s32 fullLen;
    s32 sinPitch;
    s32 cosPitch;
    s32 sinYaw;
    s32 cosYaw;
    s32 padBefore[4];
    FixedTransform pitchMtx;
    FixedTransform yawMtx;
    FixedMatrix3s transformMtx;
    s32 padAfter[4];
    s32 row;
    s32 col;

    initFixedTransform(&pitchMtx);
    initFixedTransform(&yawMtx);

    dy = (D_801124A0->pos.y - D_801124A0->focus.y) + 0x40000;
    dx = D_801124A0->pos.x - D_801124A0->focus.x;
    dz = D_801124A0->pos.z - D_801124A0->focus.z;

    xzLen = integerSquareRoot64((s64)dx * dx + (s64)dz * dz);
    fullLen = integerSquareRoot64((s64)xzLen * xzLen + (s64)dy * dy);

    D_801124A0->pitch = calculateAngleBetweenXZPoints(0, 0, xzLen, -dy);
    if (fullLen != 0) {
        sinPitch = ((s64)dy * RACE_CAMERA_FP_ONE) / fullLen;
        cosPitch = ((s64)xzLen * RACE_CAMERA_FP_ONE) / fullLen;
        pitchMtx.rotation[MTX_YY] = cosPitch;
        pitchMtx.rotation[MTX_ZY] = -sinPitch;
        pitchMtx.rotation[MTX_ZZ] = cosPitch;
        pitchMtx.rotation[MTX_YZ] = sinPitch;
    }

    D_801124A0->yaw = -calculateAngleBetweenXZPoints(0, 0, dx, dz);
    if (xzLen != 0) {
        sinYaw = ((s64)dx * RACE_CAMERA_FP_ONE) / xzLen;
        cosYaw = ((s64)dz * RACE_CAMERA_FP_ONE) / xzLen;
        yawMtx.rotation[MTX_XX] = cosYaw;
        yawMtx.rotation[MTX_ZX] = -sinYaw;
        yawMtx.rotation[MTX_ZZ] = cosYaw;
        yawMtx.rotation[MTX_XZ] = sinYaw;
    }

    for (row = 0; row < 3; row++) {
        for (col = 0; col < 3; col++) {
            D_801124A0->rotationMatrix[row * 3 + col] =
                RACE_CAMERA_FP_DOT(yawMtx.rotation[row * 3 + 0], pitchMtx.rotation[col + 0],
                                   yawMtx.rotation[row * 3 + 1], pitchMtx.rotation[col + 3],
                                   yawMtx.rotation[row * 3 + 2], pitchMtx.rotation[col + 6]);
        }
    }

    pitchMtx.rotation[MTX_YZ] *= -1;
    pitchMtx.rotation[MTX_ZY] *= -1;
    yawMtx.rotation[MTX_XZ] *= -1;
    yawMtx.rotation[MTX_ZX] *= -1;

    for (row = 0; row < 3; row++) {
        for (col = 0; col < 3; col++) {
            transformMtx[row * 3 + col] =
                RACE_CAMERA_FP_DOT(pitchMtx.rotation[row * 3 + 0], yawMtx.rotation[col + 0],
                                   pitchMtx.rotation[row * 3 + 1], yawMtx.rotation[col + 3],
                                   pitchMtx.rotation[row * 3 + 2], yawMtx.rotation[col + 6]);
        }
    }

    D_801124A0->transformOffset.x = -((((s64)transformMtx[MTX_XZ] * D_801124A0->unk28) / 0x10000) + D_801124A0->pos.x);
    D_801124A0->transformOffset.y = -((((s64)transformMtx[MTX_YZ] * D_801124A0->unk28) / 0x10000) + D_801124A0->pos.y);
    D_801124A0->transformOffset.z = -((((s64)transformMtx[MTX_ZZ] * D_801124A0->unk28) / 0x10000) + D_801124A0->pos.z);
    func_800486BC(transformMtx, D_801124A0->transform);
}
#endif

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
    makeFixedRotationY(matrix, player->state.yaw);
    transformVec3iByFixedMatrix(matrix, &offset, &transformedOffset);
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

    if (gRaceUpdatePaused == 0) {
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
    if (gRaceUpdatePaused == 0) {
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

// func_8006F048 best match: 79.490% (nonmatchings/func_8006F048-6113366811127043669/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006F048.s")

#ifdef NON_MATCHING
void func_8006F048(void) {
    RaceCamera *camera;
    RacePlayerSlot *player;
    RacePlayerSlot *otherPlayer;
    RacePlayerSlot *players;
    s8 *playerCount;
    s32 stride;
    s32 otherStride;
    s32 i;
    s32 blockedAngles;
    s32 dx;
    s32 dz;
    s64 distSq;
    s32 dist;
    s32 sine;
    s32 cosine;
    s16 targetYaw;
    s16 yaw;
    s16 diff;
    u8 *angleOrder;

    if (gRaceUpdatePaused == 0) {
        stride = RACE_PLAYER_STATE_SIZE;
        players = D_80121D80;
        playerCount = &gRacePlayerCount;
        player = (RacePlayerSlot *)((u8 *)players + (D_801124A0->playerIndex * stride));

        D_801124A0->focus.x += (player->state.cameraPos.x - D_801124A0->focus.x) >> 1;
        player = (RacePlayerSlot *)((u8 *)players + (D_801124A0->playerIndex * stride));
        D_801124A0->focus.y += (player->state.cameraPos.y - D_801124A0->focus.y) >> 1;
        player = (RacePlayerSlot *)((u8 *)players + (D_801124A0->playerIndex * stride));
        D_801124A0->focus.z += (player->state.cameraPos.z - D_801124A0->focus.z) >> 1;

        player = (RacePlayerSlot *)((u8 *)players + (D_801124A0->playerIndex * stride));
        targetYaw = player->state.yaw - 0x300;
        blockedAngles = 0;
        i = 0;

        if (*playerCount > 0) {
            otherStride = RACE_PLAYER_STATE_SIZE;
            do {
                if (i != D_801124A0->playerIndex) {
                    player = (RacePlayerSlot *)((u8 *)players + (D_801124A0->playerIndex * stride));
                    otherPlayer = (RacePlayerSlot *)((u8 *)players + (i * otherStride));
                    dx = player->state.pos.x - otherPlayer->state.pos.x;
                    dz = player->state.pos.z - otherPlayer->state.pos.z;
                    distSq = (s64)dx * dx + (s64)dz * dz;

                    if (distSq < 0xE1000000000LL) {
                        if (distSq < 0xE0F00000000LL) {
                            blockedAngles |= 1 << (s16)((((calculateAngleFromDeltaXZ(dx, dz) + 0x800) - targetYaw) + 0x100) & 0xFFF) >> 9;
                        } else {
                            break;
                        }
                    }
                }
                i++;
            } while (i < *playerCount);
            i = 0;
        }

        angleOrder = D_800DA914;
        while ((blockedAngles & (1 << *angleOrder)) != 0) {
            i++;
            angleOrder++;
            if (i == 8) {
                break;
            }
        }

        yaw = targetYaw + D_800DA900[i];
        yaw = (s16)yaw;
        camera = D_801124A0;
        sine = calculateAngleBetweenXZPoints(camera->focus.x, camera->focus.z, camera->pos.x, camera->pos.z);
        diff = (yaw - sine) & 0xFFF;
        if (diff >= 0x801) {
            diff -= 0x1000;
        }
        if (diff >= 0x21) {
            diff = 0x20;
        }
        if (diff < -0x20) {
            diff = -0x20;
        }
        yaw = diff + sine;
        D_801124A0->unk92 = yaw;

        camera = D_801124A0;
        dx = camera->pos.x - camera->focus.x;
        dz = camera->pos.z - camera->focus.z;
        dist = integerSquareRoot64((s64)dx * dx + (s64)dz * dz);
        dist = ((0x300000 - dist) >> 1) + dist;

        sine = fixedSine(yaw);
        cosine = fixedCosine(yaw);
        D_801124A0->pos.x = (((s64)sine * -dist) / 0x1000) + D_801124A0->focus.x;
        D_801124A0->pos.z = (((s64)cosine * -dist) / 0x1000) + D_801124A0->focus.z;

        D_801124A0->pos.y += (D_801124A0->focus.y - D_801124A0->pos.y) >> 1;

        D_801124A0->unk28 += (0x960000 - D_801124A0->unk28) >> 1;

        if (D_801235B4 & 0x20) {
            D_801124A0->update = func_8006F5B0;
        }
    }

    D_801124A0->prevPos.x = D_80121D80[D_801124A0->playerIndex].state.pos.x;
    D_801124A0->prevPos.y = D_80121D80[D_801124A0->playerIndex].state.pos.y;
    D_801124A0->prevPos.z = D_80121D80[D_801124A0->playerIndex].state.pos.z;
    func_8006D8B4();
}
#endif

void func_8006F5B0(void) {
    s32 unused[3];
    s32 dz;
    s32 unused2;
    s32 dist;
    s32 sine;
    s32 cosine;
    s32 dx;

    if (gRaceUpdatePaused == 0) {
        D_801124A0->focus.x += (D_80121D80[D_801124A0->playerIndex].state.cameraPos.x - D_801124A0->focus.x) >> 1;
        D_801124A0->focus.y += (D_80121D80[D_801124A0->playerIndex].state.cameraPos.y - D_801124A0->focus.y) >> 1;
        D_801124A0->focus.z += (D_80121D80[D_801124A0->playerIndex].state.cameraPos.z - D_801124A0->focus.z) >> 1;

        dx = D_801124A0->pos.x - D_801124A0->focus.x;
        dz = D_801124A0->pos.z - D_801124A0->focus.z;
        dist = integerSquareRoot64((s64)dx * dx + (s64)dz * dz);
        dist = ((0x400000 - dist) >> 4) + dist;

        D_801124A0->unk92 += 8;
        sine = fixedSine(D_801124A0->unk92);
        cosine = fixedCosine(D_801124A0->unk92);

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

void func_8006FA20(void) {
    register s32 stride;
    register s16 timer;
    register s16 diff;

    stride = RACE_CAMERA_ROTATION_TRANSITION_STRIDE;
    timer = D_801124A0->timer;
    if (RACE_CAMERA_ROTATION_TRANSITION[-8].duration < timer) {
        timer = RACE_CAMERA_ROTATION_TRANSITION[-8].duration;
    }

    D_801124A0->pos.x = (((s64)(RACE_CAMERA_ROTATION_TRANSITION[-8].endPos.x - RACE_CAMERA_ROTATION_TRANSITION[-8].startPos.x) * timer) / ((RaceCameraRotationTransition *)((u8 *)D_800DA91C + ((u16) D_801124A0->mode * stride)))[-8].duration) + ((RaceCameraRotationTransition *)((u8 *)D_800DA91C + ((u16) D_801124A0->mode * stride)))[-8].startPos.x;
    D_801124A0->pos.y = (((s64)(((RaceCameraRotationTransition *)((u8 *)D_800DA91C + ((u16) D_801124A0->mode * stride)))[-8].endPos.y - ((RaceCameraRotationTransition *)((u8 *)D_800DA91C + ((u16) D_801124A0->mode * stride)))[-8].startPos.y) * timer) / ((RaceCameraRotationTransition *)((u8 *)D_800DA91C + ((u16) D_801124A0->mode * stride)))[-8].duration) + ((RaceCameraRotationTransition *)((u8 *)D_800DA91C + ((u16) D_801124A0->mode * stride)))[-8].startPos.y;
    D_801124A0->pos.z = (((s64)(((RaceCameraRotationTransition *)((u8 *)D_800DA91C + ((u16) D_801124A0->mode * stride)))[-8].endPos.z - ((RaceCameraRotationTransition *)((u8 *)D_800DA91C + ((u16) D_801124A0->mode * stride)))[-8].startPos.z) * timer) / ((RaceCameraRotationTransition *)((u8 *)D_800DA91C + ((u16) D_801124A0->mode * stride)))[-8].duration) + ((RaceCameraRotationTransition *)((u8 *)D_800DA91C + ((u16) D_801124A0->mode * stride)))[-8].startPos.z;

    diff = (RACE_CAMERA_ROTATION_TRANSITION[-8].endPitch - RACE_CAMERA_ROTATION_TRANSITION[-8].startPitch) & RACE_CAMERA_ANGLE_MASK;
    if (diff >= 0x801) {
        diff -= 0x1000;
    }
    diff = (diff * timer) / RACE_CAMERA_ROTATION_TRANSITION[-8].duration;
    D_801124A0->pitch = RACE_CAMERA_ROTATION_TRANSITION[-8].startPitch + (s16)diff;

    diff = (RACE_CAMERA_ROTATION_TRANSITION[-8].endYaw - RACE_CAMERA_ROTATION_TRANSITION[-8].startYaw) & RACE_CAMERA_ANGLE_MASK;
    if (diff >= 0x801) {
        diff -= 0x1000;
    }
    diff = (diff * timer) / RACE_CAMERA_ROTATION_TRANSITION[-8].duration;
    D_801124A0->yaw = RACE_CAMERA_ROTATION_TRANSITION[-8].startYaw + diff;

    D_801124A0->prevPos.x = D_801124A0->pos.x;
    D_801124A0->prevPos.y = D_801124A0->pos.y;
    D_801124A0->prevPos.z = D_801124A0->pos.z;
    D_801124A0->timer++;
    func_8006D7D4();
}

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

void func_8006FE88(void) {
    s16 timer;
    s16 duration;

    timer = D_801124A0->timer;

    duration = D_800DAA3C[D_801124A0->mode - 16].duration;
    if (duration < timer) {
        timer = duration;
    }

    D_801124A0->pos.x = (((s64)(D_800DAA3C[D_801124A0->mode - 16].endPos.x - D_800DAA3C[D_801124A0->mode - 16].startPos.x) * timer) / D_800DAA3C[D_801124A0->mode - 16].duration) + D_800DAA3C[D_801124A0->mode - 16].startPos.x;
    D_801124A0->pos.y = (((s64)(D_800DAA3C[D_801124A0->mode - 16].endPos.y - D_800DAA3C[D_801124A0->mode - 16].startPos.y) * timer) / D_800DAA3C[D_801124A0->mode - 16].duration) + D_800DAA3C[D_801124A0->mode - 16].startPos.y;
    D_801124A0->pos.z = (((s64)(D_800DAA3C[D_801124A0->mode - 16].endPos.z - D_800DAA3C[D_801124A0->mode - 16].startPos.z) * timer) / D_800DAA3C[D_801124A0->mode - 16].duration) + D_800DAA3C[D_801124A0->mode - 16].startPos.z;

    D_801124A0->focus.x = D_80121D80[D_800DAA3C[D_801124A0->mode - 16].playerIndex].state.cameraPos.x;
    D_801124A0->focus.y = D_80121D80[D_800DAA3C[D_801124A0->mode - 16].playerIndex].state.cameraPos.y;
    D_801124A0->focus.z = D_80121D80[D_800DAA3C[D_801124A0->mode - 16].playerIndex].state.cameraPos.z;
    D_801124A0->prevPos.x = D_801124A0->pos.x;
    D_801124A0->prevPos.y = D_801124A0->pos.y;
    D_801124A0->prevPos.z = D_801124A0->pos.z;
    D_801124A0->timer++;
    func_8006D8B4();
}

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
    D_801124A0->unkA8 = (-fixedSine(angle) * 0xC00) + 0xC00000;

    velocity = D_801124A0->velocity;
    if (velocity < 0x80000) {
        D_801124A0->velocity = velocity + 0x8000;
        velocity = D_801124A0->velocity;
    }

    D_801124A0->pos.z += velocity;
    D_801124A0->pos.y = func_80080CC4((s16)func_8007D200(0, D_801124A0->pos.x, D_801124A0->pos.z), D_801124A0->pos.x, D_801124A0->pos.z) + D_801124A0->unkA8 + 0xE0000;
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
