#include "game/race/race_state.h"
#include "common.h"
#define calculateFixedAngleBetweenXZPoints calculateFixedAngleBetweenXZPoints_s32
#include "game/math/fixed_point_math.h"
#undef calculateFixedAngleBetweenXZPoints
#include "game/race/motion/race_motion.h"
#include "game/race/camera/race_camera.h"
#include "game/race/player/race_player_input.h"

#define RACE_CAMERA_COUNT 4
#define RACE_CAMERA_STATE_SIZE 0xB0
#define RACE_CAMERA_ANGLE_MASK 0xFFF
#define RACE_CAMERA_FP_ONE 0x1000
#define RACE_CAMERA_ROTATION_TRANSITION_STRIDE 0x24
#define RACE_CAMERA_ROTATION_TRANSITION ((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions + ((u16)D_801124A0->mode * stride)))
#define FIXED_MUL(a, b) (((a) * (b)) / 0x1000)
#define FIXED_MATRIX_ROWS(matrix) ((s16(*)[3])(matrix))
#define RACE_CAMERA_FP_DOT(a, b, c, d, e, f) \
    (((a) * (b)) / RACE_CAMERA_FP_ONE + ((c) * (d)) / RACE_CAMERA_FP_ONE + ((e) * (f)) / RACE_CAMERA_FP_ONE)

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
    FixedTransform transform;
    Vec3i transformed;
    Vec3i offset;
} StackD7D4;

extern void packFixedTransformMatrix(void *, void *);
extern s16 calculateFixedAngleBetweenXZPoints(s32, s32, s32, s32);
extern RaceCamera D_801121E0[RACE_CAMERA_COUNT];
extern RaceCamera *D_801124A0;
extern u8 gRaceCameraRotationTransitions[];
extern s16 gRaceCameraChaseYawOffsets[];
extern u8 gRaceCameraChaseYawPreferenceOrder[];
extern RaceCameraTransition gRaceCameraPositionTransitions[];
extern void *gRaceCameraModeUpdates[];
extern s32 gRaceCameraReplayStartX;
extern s32 gRaceCameraReplayStartY;
extern s32 gRaceCameraReplayStartZ;
extern s8 gRacePlayerCount;
extern u8 gRaceUpdatePaused;
extern s32 gMenuFlowState;

void setRaceCameraMode(u16 arg0, u16 arg1) {
    RaceCamera *temp;

    if (gRaceCameraModeChangeDisabled == 0) {
        temp = &D_801121E0[arg0];
        temp->mode = arg1;
        temp->update = gRaceCameraModeUpdates[arg1];
    }
}

void setRaceCameraModeForced(u16 arg0, u16 arg1) {
    RaceCamera *temp = &D_801121E0[arg0];

    temp->mode = arg1;
    temp->update = gRaceCameraModeUpdates[arg1];
}

void resetRaceCameras(void) {
    RaceCamera *camera;

    setRaceCameraMode(0, 0);
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

void updateRaceCameras(void) {
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

void updateRaceCamera(s32 arg0) {
    D_801124A0 = &D_801121E0[arg0];
    D_801124A0->update();
}

void updateRaceCameraTransformFromAngles(void) {
    StackD7D4 stack;

    makeFixedRotationYX(D_801124A0->rotationMatrix, -D_801124A0->pitch, -D_801124A0->yaw);
    stack.offset.x = 0;
    stack.offset.y = 0;
    stack.offset.z = -D_801124A0->distance;
    makeFixedRotationXY(stack.transform.rotation, D_801124A0->pitch, D_801124A0->yaw);
    transformVec3iByFixedMatrix(stack.transform.rotation, &stack.offset, &stack.transformed);
    D_801124A0->transformOffset.x = stack.transformed.x - D_801124A0->pos.x;
    D_801124A0->transformOffset.y = stack.transformed.y - D_801124A0->pos.y;
    D_801124A0->transformOffset.z = stack.transformed.z - D_801124A0->pos.z;
    packFixedTransformMatrix(&stack.transform, D_801124A0->transform);
}

void updateRaceCameraLookAtTransform(void) {
    s32 dx;
    s32 dy;
    s32 dz;
    s32 xzDist;
    s32 dist;
    s32 cosine;
    s32 sine;
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

    D_801124A0->pitch = calculateFixedAngleBetweenXZPoints(0, 0, xzDist, -dy);
    if (dist != 0) {
        sine = ((s64)dy * 0x1000) / dist;
        cosine = ((s64)xzDist * 0x1000) / dist;
        pitchMtx.rotation[MTX_YY] = cosine;
        pitchMtx.rotation[MTX_ZY] = -sine;
        pitchMtx.rotation[MTX_ZZ] = cosine;
        pitchMtx.rotation[MTX_YZ] = sine;
    }

    D_801124A0->yaw = -calculateFixedAngleBetweenXZPoints(0, 0, dx, dz);
    if (xzDist != 0) {
        sine = ((s64)dx * 0x1000) / xzDist;
        cosine = ((s64)dz * 0x1000) / xzDist;
        yawMtx.rotation[MTX_XX] = cosine;
        yawMtx.rotation[MTX_XZ] = sine;
        yawMtx.rotation[MTX_ZX] = -sine;
        yawMtx.rotation[MTX_ZZ] = cosine;
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            FIXED_MATRIX_ROWS(D_801124A0->rotationMatrix)[i][j] =
                FIXED_MUL(FIXED_MATRIX_ROWS(yawMtx.rotation)[i][0], FIXED_MATRIX_ROWS(pitchMtx.rotation)[0][j]) +
                FIXED_MUL(FIXED_MATRIX_ROWS(yawMtx.rotation)[i][1], FIXED_MATRIX_ROWS(pitchMtx.rotation)[1][j]) +
                FIXED_MUL(FIXED_MATRIX_ROWS(yawMtx.rotation)[i][2], FIXED_MATRIX_ROWS(pitchMtx.rotation)[2][j]);
        }
    }

    pitchMtx.rotation[MTX_YZ] *= -1;
    pitchMtx.rotation[MTX_ZY] *= -1;
    yawMtx.rotation[MTX_XZ] *= -1;
    yawMtx.rotation[MTX_ZX] *= -1;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            resultMtx[(i * 3) + j] =
                FIXED_MUL(pitchMtx.rotation[i * 3], yawMtx.rotation[j]) +
                FIXED_MUL(pitchMtx.rotation[(i * 3) + 1], yawMtx.rotation[j + 3]) +
                FIXED_MUL(pitchMtx.rotation[(i * 3) + 2], yawMtx.rotation[j + 6]);
        }
    }

    D_801124A0->transformOffset.x =
        -((((s64)resultMtx[MTX_YX] * D_801124A0->unk28) / 0x10000) + D_801124A0->pos.x);
    D_801124A0->transformOffset.y =
        -((((s64)resultMtx[MTX_YY] * D_801124A0->unk28) / 0x10000) + D_801124A0->pos.y);
    D_801124A0->transformOffset.z =
        -((((s64)resultMtx[MTX_YZ] * D_801124A0->unk28) / 0x10000) + D_801124A0->pos.z);
    packFixedTransformMatrix(resultMtx, D_801124A0->transform);
}

void updateRaceCameraAlternateLookAtTransform(void) {
    s32 unused[6];
    s32 dx;
    s32 dy;
    s32 dz;
    s32 xzDist;
    s32 dist;
    s32 cosine;
    s32 sine;
    FixedTransform pitchMtx;
    FixedTransform yawMtx;
    s32 pad[3];
    FixedMatrix3s resultMtx;
    s32 i;
    s32 j;

    initFixedTransform(&pitchMtx);
    initFixedTransform(&yawMtx);
    unused[0] = unused[0];
    dx = D_801124A0->pos.x - D_801124A0->focus.x;
    pad[0] = pad[0];

    dy = (D_801124A0->pos.y - D_801124A0->focus.y) + 0x40000;
    dz = D_801124A0->pos.z - D_801124A0->focus.z;

    xzDist = integerSquareRoot64((s64)dx * dx + (s64)dz * dz);
    dist = integerSquareRoot64((s64)xzDist * xzDist + (s64)dy * dy);

    D_801124A0->pitch = calculateFixedAngleBetweenXZPoints(0, 0, xzDist, -dy);
    if (dist != 0) {
        sine = ((s64)dy * 0x1000) / dist;
        cosine = ((s64)xzDist * 0x1000) / dist;
        pitchMtx.rotation[MTX_YY] = cosine;
        pitchMtx.rotation[MTX_ZY] = -sine;
        pitchMtx.rotation[MTX_ZZ] = cosine;
        pitchMtx.rotation[MTX_YZ] = sine;
    }

    D_801124A0->yaw = -calculateFixedAngleBetweenXZPoints(0, 0, dx, dz);
    if (xzDist != 0) {
        sine = ((s64)dx * 0x1000) / xzDist;
        cosine = ((s64)dz * 0x1000) / xzDist;
        yawMtx.rotation[MTX_XX] = cosine;
        yawMtx.rotation[MTX_XZ] = sine;
        yawMtx.rotation[MTX_ZX] = -sine;
        yawMtx.rotation[MTX_ZZ] = cosine;
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            FIXED_MATRIX_ROWS(D_801124A0->rotationMatrix)[i][j] =
                FIXED_MUL(FIXED_MATRIX_ROWS(yawMtx.rotation)[i][0], FIXED_MATRIX_ROWS(pitchMtx.rotation)[0][j]) +
                FIXED_MUL(FIXED_MATRIX_ROWS(yawMtx.rotation)[i][1], FIXED_MATRIX_ROWS(pitchMtx.rotation)[1][j]) +
                FIXED_MUL(FIXED_MATRIX_ROWS(yawMtx.rotation)[i][2], FIXED_MATRIX_ROWS(pitchMtx.rotation)[2][j]);
        }
    }

    pitchMtx.rotation[MTX_YZ] *= -1;
    pitchMtx.rotation[MTX_ZY] *= -1;
    yawMtx.rotation[MTX_XZ] *= -1;
    yawMtx.rotation[MTX_ZX] *= -1;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            resultMtx[(i * 3) + j] =
                FIXED_MUL(pitchMtx.rotation[i * 3], yawMtx.rotation[j]) +
                FIXED_MUL(pitchMtx.rotation[(i * 3) + 1], yawMtx.rotation[j + 3]) +
                FIXED_MUL(pitchMtx.rotation[(i * 3) + 2], yawMtx.rotation[j + 6]);
        }
    }

    D_801124A0->transformOffset.x =
        -((((s64)resultMtx[MTX_YX] * D_801124A0->unk28) / 0x10000) + D_801124A0->pos.x);
    D_801124A0->transformOffset.y =
        -((((s64)resultMtx[MTX_YY] * D_801124A0->unk28) / 0x10000) + D_801124A0->pos.y);
    D_801124A0->transformOffset.z =
        -((((s64)resultMtx[MTX_YZ] * D_801124A0->unk28) / 0x10000) + D_801124A0->pos.z);
    packFixedTransformMatrix(resultMtx, D_801124A0->transform);
}

void noopRaceCameraUpdate(void) {
}

void unusedRaceCameraNoop1(void) {
}

void initRaceCameraFollowPlayer(void) {
    s16 matrix[0x10];
    Vec3i offset;
    Vec3i transformedOffset;
    RacePlayer *player;

    offset.y = 0;
    offset.x = 0;
    offset.z = 0x400000;
    player = &gRacePlayers[D_801124A0->playerIndex];
    if (player->stateFlags & 0x400) {
        offset.z = 0xFFC00000;
        player = (RacePlayer *)(s32)&gRacePlayers[D_801124A0->playerIndex];
    }
    makeFixedRotationY(matrix, player->facingAngle);
    transformVec3iByFixedMatrix(matrix, &offset, &transformedOffset);
    D_801124A0->focus.x = gRacePlayers[D_801124A0->playerIndex].projectedPos.x;
    D_801124A0->focus.y = gRacePlayers[D_801124A0->playerIndex].projectedPos.y;
    D_801124A0->focus.z = gRacePlayers[D_801124A0->playerIndex].projectedPos.z;
    D_801124A0->pos.x = gRacePlayers[D_801124A0->playerIndex].projectedPos.x + transformedOffset.x;
    D_801124A0->pos.y = gRacePlayers[D_801124A0->playerIndex].projectedPos.y + transformedOffset.y;
    D_801124A0->pos.z = gRacePlayers[D_801124A0->playerIndex].projectedPos.z + transformedOffset.z;
    D_801124A0->prevPos.x = gRacePlayers[D_801124A0->playerIndex].pos.x;
    D_801124A0->prevPos.y = gRacePlayers[D_801124A0->playerIndex].pos.y;
    D_801124A0->prevPos.z = gRacePlayers[D_801124A0->playerIndex].pos.z;
    D_801124A0->distance = 0;
    D_801124A0->unk28 = 0x012C0000;
    D_801124A0->update = updateRaceCameraFollowPlayer;
    D_801124A0->update();
}

void updateRaceCameraFollowPlayer(void) {
    s32 delta;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 x;
    s32 y;
    s32 z;
    s32 distance;

    if (gRaceUpdatePaused == 0) {
        D_801124A0->focus.x +=
            (gRacePlayers[D_801124A0->playerIndex].projectedPos.x - D_801124A0->focus.x) >> 1;
        D_801124A0->focus.y +=
            (gRacePlayers[D_801124A0->playerIndex].projectedPos.y - D_801124A0->focus.y) >> 1;
        D_801124A0->focus.z +=
            (gRacePlayers[D_801124A0->playerIndex].projectedPos.z - D_801124A0->focus.z) >> 1;

        x = D_801124A0->pos.x;
        y = D_801124A0->pos.y;
        z = D_801124A0->pos.z;

        dx = D_801124A0->focus.x - D_801124A0->pos.x;
        dy = D_801124A0->focus.y - D_801124A0->pos.y;
        dz = D_801124A0->focus.z - D_801124A0->pos.z;

        distance = integerSquareRoot64(((s64) dx * dx) + ((s64) dz * dz));

        if (distance == 0) {
            distance = 1;
            dz = 1;
        }

        if (distance < 0x400000) {
            dx = ((s64) dx * 0x400000) / distance;
            dz = ((s64) dz * 0x400000) / distance;

            x = D_801124A0->focus.x - dx;
            z = D_801124A0->focus.z - dz;
        }

        distance = integerSquareRoot64((((s64) dx * dx) + ((s64) dy * dy)) + ((s64) dz * dz));

        if (distance > (0x460000 - D_801124A0->distance)) {
            dx = ((s64) dx * (0x460000 - D_801124A0->distance)) / distance;
            dy = ((s64) dy * (0x460000 - D_801124A0->distance)) / distance;
            dz = ((s64) dz * (0x460000 - D_801124A0->distance)) / distance;

            x = D_801124A0->focus.x - dx;
            y = D_801124A0->focus.y - dy;
            z = D_801124A0->focus.z - dz;
        } else if (distance < (0x458000 - D_801124A0->distance)) {
            dx = ((s64) dx * (0x458000 - D_801124A0->distance)) / distance;
            dy = ((s64) dy * (0x458000 - D_801124A0->distance)) / distance;
            if (gRacePlayers) {
            }
            dz = ((s64) dz * (0x458000 - D_801124A0->distance)) / distance;

            x = D_801124A0->focus.x - dx;
            y = D_801124A0->focus.y - dy;
            z = D_801124A0->focus.z - dz;
        }

        if (!(gRacePlayers[D_801124A0->playerIndex].stateFlags & 0x1000)) {
            distance = findRaceCourseSurfaceFromHint(
                gRacePlayers[D_801124A0->playerIndex].coursePathIndex, x, z);
            delta = getRaceCourseSurfaceHeight((s16) distance, x, z) - 0x40000;

            if (y < delta) {
                y = delta;
            }
        }

        D_801124A0->pos.x = (D_801124A0->pos.x + x) - D_801124A0->pos.x;
        D_801124A0->pos.y = (D_801124A0->pos.y + y) - D_801124A0->pos.y;
        D_801124A0->pos.z = (D_801124A0->pos.z + z) - D_801124A0->pos.z;

        delta = 0x12C0000;

        if (gRacePlayers[D_801124A0->playerIndex].stateFlags & 0x800) {
            delta = 0;
        }

        delta -= D_801124A0->unk28;

        if (delta > 0x96000) {
            delta = 0x96000;
        }

        if (delta < 0) {
            delta /= 6;
        }

        D_801124A0->unk28 += delta;

        delta = 0;

        if (gRacePlayers[D_801124A0->playerIndex].stateFlags & 0x800) {
            delta = 0x280000;
        }

        delta -= D_801124A0->distance;

        if (delta < -0x14000) {
            delta = -0x14000;
        }

        if (delta > 0) {
            delta /= 6;
        }

        D_801124A0->distance += delta;
    }

    D_801124A0->prevPos.x = gRacePlayers[D_801124A0->playerIndex].pos.x;
    D_801124A0->prevPos.y = gRacePlayers[D_801124A0->playerIndex].pos.y;
    D_801124A0->prevPos.z = gRacePlayers[D_801124A0->playerIndex].pos.z;

    updateRaceCameraAlternateLookAtTransform();
}

void noopRaceCameraDebugUpdate(void) {
}

void unusedRaceCameraNoop2(void) {
}

void initRaceCameraCourseStart(void) {
    D_801124A0->update = updateRaceCameraCourseStart;
    D_801124A0->update();
}

void updateRaceCameraCourseStart(void) {
    s32 diff;

    if (gRaceUpdatePaused == 0) {
        D_801124A0->focus.x += (gRacePlayers[D_801124A0->playerIndex].projectedPos.x - D_801124A0->focus.x) >> 1;
        D_801124A0->focus.y += (gRacePlayers[D_801124A0->playerIndex].projectedPos.y - D_801124A0->focus.y) >> 1;
        D_801124A0->focus.z += (gRacePlayers[D_801124A0->playerIndex].projectedPos.z - D_801124A0->focus.z) >> 1;

        diff = 0x640000 - D_801124A0->unk28;
        if (diff >= 0x10001) {
            diff = 0x10000;
        }
        if (diff < -0x10000) {
            diff = -0x10000;
        }
        D_801124A0->unk28 += diff;

        diff = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].cameraPos.x - D_801124A0->pos.x;
        if (diff >= 0xC001) {
            diff = 0xC000;
        }
        if (diff < -0xC000) {
            diff = -0xC000;
        }
        D_801124A0->pos.x += diff;

        diff = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].cameraPos.y - D_801124A0->pos.y;
        if (diff >= 0xC001) {
            diff = 0xC000;
        }
        if (diff < -0xC000) {
            diff = -0xC000;
        }
        D_801124A0->pos.y += diff;

        diff = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].cameraPos.z - D_801124A0->pos.z;
        if (diff >= 0xC001) {
            diff = 0xC000;
        }
        if (diff < -0xC000) {
            diff = -0xC000;
        }
        D_801124A0->pos.z += diff;
    }
    updateRaceCameraLookAtTransform();
}

void initRaceCameraFixedPositionFollow(void) {
    D_801124A0->update = updateRaceCameraFixedPositionFollow;
    D_801124A0->update();
}

void updateRaceCameraFixedPositionFollow(void) {
    if (gRaceUpdatePaused == 0) {
        D_801124A0->focus.x += (gRacePlayers[D_801124A0->playerIndex].projectedPos.x - D_801124A0->focus.x) >> 1;
        D_801124A0->focus.y += (gRacePlayers[D_801124A0->playerIndex].projectedPos.y - D_801124A0->focus.y) >> 1;
        D_801124A0->focus.z += (gRacePlayers[D_801124A0->playerIndex].projectedPos.z - D_801124A0->focus.z) >> 1;
    }
    updateRaceCameraLookAtTransform();
}

void initRaceCameraChase(void) {
    D_801124A0->timer = 0x96;
    D_801124A0->update = updateRaceCameraChase;
    D_801124A0->update();
}

void updateRaceCameraChase(void) {
    s32 i;
    s32 blockedAngles;
    s32 dx;
    s32 dz;
    s32 dist;
    s32 sine;
    s32 cosine;
    s16 angle;
    s16 targetYaw;
    s16 yaw;
    u8 *angleOrder;

    if (gRaceUpdatePaused == 0) {
        D_801124A0->focus.x += (gRacePlayers[D_801124A0->playerIndex].projectedPos.x - D_801124A0->focus.x) >> 1;
        D_801124A0->focus.y += (gRacePlayers[D_801124A0->playerIndex].projectedPos.y - D_801124A0->focus.y) >> 1;
        D_801124A0->focus.z += (gRacePlayers[D_801124A0->playerIndex].projectedPos.z - D_801124A0->focus.z) >> 1;
        targetYaw = gRacePlayers[D_801124A0->playerIndex].facingAngle - 0x300;
        blockedAngles = targetYaw * 0;
        i = 0;

        if (gRacePlayerCount > 0) {
            do {
                if (i != D_801124A0->playerIndex) {
                    dx = gRacePlayers[D_801124A0->playerIndex].pos.x - gRacePlayers[i].pos.x;
                    dz = gRacePlayers[D_801124A0->playerIndex].pos.z - gRacePlayers[i].pos.z;
                    if ((s64)dx * dx + (s64)dz * dz < 0xE1000000000LL) {
                        angle = calculateFixedAngleFromDeltaXZ(dx, dz) + 0x800;
                        blockedAngles |= 1 << (s16)((((angle - targetYaw) + 0x100) & 0xFFF) >> 9);
                    }
                }
                i++;
            } while (i < gRacePlayerCount);
            i = 0;
        }

        angleOrder = gRaceCameraChaseYawPreferenceOrder;
loop_11:
        if ((blockedAngles & (1 << *angleOrder)) != 0) {
            i++;
            angleOrder++;
            if (i == 8) {
                goto loop_11_done;
            }
            goto loop_11;
        }
loop_11_done:

        targetYaw += gRaceCameraChaseYawOffsets[i];
        targetYaw = (s16)targetYaw;
        angle = calculateFixedAngleBetweenXZPoints(D_801124A0->focus.x, D_801124A0->focus.z, D_801124A0->pos.x, D_801124A0->pos.z);
        yaw = (targetYaw - angle) & 0xFFF;
        if (yaw >= 0x801) {
            yaw -= 0x1000;
        }
        if (yaw >= 0x21) {
            yaw = 0x20;
        }
        if (yaw < -0x20) {
            yaw = -0x20;
        }
        yaw += angle;
        D_801124A0->unk92 = yaw;

        dx = D_801124A0->pos.x - D_801124A0->focus.x;
        dz = D_801124A0->pos.z - D_801124A0->focus.z;
        dist = integerSquareRoot64((s64)dx * dx + (s64)dz * dz);
        dist = ((0x300000 - dist) >> 1) + dist;

        sine = fixedSine(yaw);
        cosine = fixedCosine(yaw);
        D_801124A0->pos.x = (((s64)sine * -dist) / 0x1000) + D_801124A0->focus.x;
        D_801124A0->pos.z = (((s64)cosine * -dist) / 0x1000) + D_801124A0->focus.z;
        D_801124A0->pos.y += (D_801124A0->focus.y - D_801124A0->pos.y) >> 1;
        D_801124A0->unk28 += (0x960000 - D_801124A0->unk28) >> 1;
        if (gMenuFlowState & 0x20) {
            D_801124A0->update = updateRaceCameraFinishOrbit;
        }
    }

    D_801124A0->prevPos.x = gRacePlayers[D_801124A0->playerIndex].pos.x;
    D_801124A0->prevPos.y = gRacePlayers[D_801124A0->playerIndex].pos.y;
    D_801124A0->prevPos.z = gRacePlayers[D_801124A0->playerIndex].pos.z;
    updateRaceCameraLookAtTransform();
}

void updateRaceCameraFinishOrbit(void) {
    s32 unused[3];
    s32 dz;
    s32 unused2;
    s32 dist;
    s32 sine;
    s32 cosine;
    s32 dx;

    if (gRaceUpdatePaused == 0) {
        D_801124A0->focus.x += (gRacePlayers[D_801124A0->playerIndex].projectedPos.x - D_801124A0->focus.x) >> 1;
        D_801124A0->focus.y += (gRacePlayers[D_801124A0->playerIndex].projectedPos.y - D_801124A0->focus.y) >> 1;
        D_801124A0->focus.z += (gRacePlayers[D_801124A0->playerIndex].projectedPos.z - D_801124A0->focus.z) >> 1;

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

    D_801124A0->prevPos.x = gRacePlayers[D_801124A0->playerIndex].pos.x;
    D_801124A0->prevPos.y = gRacePlayers[D_801124A0->playerIndex].pos.y;
    D_801124A0->prevPos.z = gRacePlayers[D_801124A0->playerIndex].pos.z;
    updateRaceCameraLookAtTransform();
}

void noopRaceCameraReplayUpdate(void) {
}

void unusedRaceCameraNoop3(void) {
}

void initRaceCameraReplayPosition(void) {
    D_801124A0->pos.x = gRaceCameraReplayStartX;
    D_801124A0->prevPos.x = D_801124A0->pos.x;
    D_801124A0->pos.y = gRaceCameraReplayStartY;
    D_801124A0->prevPos.y = D_801124A0->pos.y;
    D_801124A0->pos.z = gRaceCameraReplayStartZ;
    D_801124A0->prevPos.z = D_801124A0->pos.z;
    D_801124A0->distance = 0;
    D_801124A0->unk28 = 0;
    D_801124A0->update = updateRaceCameraReplayPosition;
    D_801124A0->update();
}

void updateRaceCameraReplayPosition(void) {

    D_801124A0->focus.x = gRacePlayers[D_801124A0->playerIndex].projectedPos.x;
    D_801124A0->focus.y = gRacePlayers[D_801124A0->playerIndex].projectedPos.y;
    D_801124A0->focus.z = gRacePlayers[D_801124A0->playerIndex].projectedPos.z;
    updateRaceCameraLookAtTransform();
}

void updateRaceCameraRotationTransition(void) {
    register s32 stride;
    register s16 timer;
    register s16 diff;

    stride = RACE_CAMERA_ROTATION_TRANSITION_STRIDE;
    timer = D_801124A0->timer;
    if (RACE_CAMERA_ROTATION_TRANSITION[-8].duration < timer) {
        timer = RACE_CAMERA_ROTATION_TRANSITION[-8].duration;
    }

    D_801124A0->pos.x = (((s64)(RACE_CAMERA_ROTATION_TRANSITION[-8].endPos.x - RACE_CAMERA_ROTATION_TRANSITION[-8].startPos.x) * timer) / ((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions + ((u16) D_801124A0->mode * stride)))[-8].duration) + ((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions + ((u16) D_801124A0->mode * stride)))[-8].startPos.x;
    D_801124A0->pos.y = (((s64)(((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions + ((u16) D_801124A0->mode * stride)))[-8].endPos.y - ((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions + ((u16) D_801124A0->mode * stride)))[-8].startPos.y) * timer) / ((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions + ((u16) D_801124A0->mode * stride)))[-8].duration) + ((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions + ((u16) D_801124A0->mode * stride)))[-8].startPos.y;
    D_801124A0->pos.z = (((s64)(((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions + ((u16) D_801124A0->mode * stride)))[-8].endPos.z - ((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions + ((u16) D_801124A0->mode * stride)))[-8].startPos.z) * timer) / ((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions + ((u16) D_801124A0->mode * stride)))[-8].duration) + ((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions + ((u16) D_801124A0->mode * stride)))[-8].startPos.z;

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
    updateRaceCameraTransformFromAngles();
}

void initRaceCameraRotationTransition(void) {
    D_801124A0->roll = 0;
    D_801124A0->distance = 0;
    D_801124A0->update = updateRaceCameraRotationTransition;
    D_801124A0->timer = 0;
    D_801124A0->update();
}

void initRaceCameraPositionTransition(void) {
    D_801124A0->distance = 0;
    D_801124A0->unk28 = 0;
    D_801124A0->timer = 0;
    D_801124A0->update = updateRaceCameraPositionTransition;
    D_801124A0->update();
}

void updateRaceCameraPositionTransition(void) {
    s16 timer;
    s16 duration;

    timer = D_801124A0->timer;

    duration = gRaceCameraPositionTransitions[D_801124A0->mode - 16].duration;
    if (duration < timer) {
        timer = duration;
    }

    D_801124A0->pos.x = (((s64)(gRaceCameraPositionTransitions[D_801124A0->mode - 16].endPos.x - gRaceCameraPositionTransitions[D_801124A0->mode - 16].startPos.x) * timer) / gRaceCameraPositionTransitions[D_801124A0->mode - 16].duration) + gRaceCameraPositionTransitions[D_801124A0->mode - 16].startPos.x;
    D_801124A0->pos.y = (((s64)(gRaceCameraPositionTransitions[D_801124A0->mode - 16].endPos.y - gRaceCameraPositionTransitions[D_801124A0->mode - 16].startPos.y) * timer) / gRaceCameraPositionTransitions[D_801124A0->mode - 16].duration) + gRaceCameraPositionTransitions[D_801124A0->mode - 16].startPos.y;
    D_801124A0->pos.z = (((s64)(gRaceCameraPositionTransitions[D_801124A0->mode - 16].endPos.z - gRaceCameraPositionTransitions[D_801124A0->mode - 16].startPos.z) * timer) / gRaceCameraPositionTransitions[D_801124A0->mode - 16].duration) + gRaceCameraPositionTransitions[D_801124A0->mode - 16].startPos.z;

    D_801124A0->focus.x = gRacePlayers[gRaceCameraPositionTransitions[D_801124A0->mode - 16].playerIndex].projectedPos.x;
    D_801124A0->focus.y = gRacePlayers[gRaceCameraPositionTransitions[D_801124A0->mode - 16].playerIndex].projectedPos.y;
    D_801124A0->focus.z = gRacePlayers[gRaceCameraPositionTransitions[D_801124A0->mode - 16].playerIndex].projectedPos.z;
    D_801124A0->prevPos.x = D_801124A0->pos.x;
    D_801124A0->prevPos.y = D_801124A0->pos.y;
    D_801124A0->prevPos.z = D_801124A0->pos.z;
    D_801124A0->timer++;
    updateRaceCameraLookAtTransform();
}

void initRaceCameraStaticFollow(void) {
    D_801124A0->pitch = 0;
    D_801124A0->yaw = 0;
    D_801124A0->roll = 0;
    D_801124A0->pos.x = 0;
    D_801124A0->pos.y = 0x310000;
    D_801124A0->pos.z = 0x900000;
    D_801124A0->distance = 0x670000;
    D_801124A0->update = updateRaceCameraStaticFollow;
    D_801124A0->update();
}

void updateRaceCameraStaticFollow(void) {
    updateRaceCameraTransformFromAngles();
}

void initRaceCameraIntroPan(void) {
    RacePlayer *player = &gRacePlayers[0];

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
    D_801124A0->update = updateRaceCameraIntroPan;
    D_801124A0->update();
}

void updateRaceCameraIntroPan(void) {
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
    D_801124A0->pos.y = getRaceCourseSurfaceHeight((s16)findRaceCourseSurfaceFromHint(0, D_801124A0->pos.x, D_801124A0->pos.z), D_801124A0->pos.x, D_801124A0->pos.z) + D_801124A0->unkA8 + 0xE0000;
    updateRaceCameraLookAtTransform();
}

void initRaceCameraMenuPreview(void) {
    D_801124A0->pitch = 0x70;
    D_801124A0->yaw = 0;
    D_801124A0->roll = 0;
    D_801124A0->pos.x = 0;
    D_801124A0->pos.y = 0x490000;
    D_801124A0->pos.z = 0x900000;
    D_801124A0->distance = 0x500000;
    D_801124A0->update = updateRaceCameraMenuPreview;
    D_801124A0->update();
}

void updateRaceCameraMenuPreview(void) {
    updateRaceCameraTransformFromAngles();
}
