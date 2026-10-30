#include "game/race/race_state.h"
#include "common.h"
#include "game/math/fixed_point_math.h"
#include "game/math/spatial_math.h"
#include "game/race/motion/race_motion.h"
#include "game/race/camera/race_camera.h"
#include "game/race/player/race_player_input.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/engine/game_task_scheduler.h"
#include "game/race/flow/race_flow.h"

#define RACE_CAMERA_STATE_SIZE 0xB0
#define RACE_CAMERA_ANGLE_MASK 0xFFF
#define RACE_CAMERA_FP_ONE 0x1000
#define RACE_CAMERA_ROTATION_TRANSITION_STRIDE 0x24
#define RACE_CAMERA_ROTATION_TRANSITION \
    ((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions + ((u16)gCurrentRaceCamera->mode * stride)))
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
    Transform3D transform;
    Vec3i transformed;
    Vec3i offset;
} StackD7D4;

void (*gRaceCameraModeUpdates[])(void) = {
    noopRaceCameraUpdate,
    initRaceCameraFollowPlayer,
    noopRaceCameraDebugUpdate,
    initRaceCameraCourseStart,
    initRaceCameraFixedPositionFollow,
    initRaceCameraChase,
    noopRaceCameraReplayUpdate,
    initRaceCameraReplayPosition,
    initRaceCameraRotationTransition,
    initRaceCameraRotationTransition,
    initRaceCameraRotationTransition,
    initRaceCameraRotationTransition,
    initRaceCameraRotationTransition,
    initRaceCameraRotationTransition,
    initRaceCameraRotationTransition,
    initRaceCameraRotationTransition,
    initRaceCameraPositionTransition,
    initRaceCameraPositionTransition,
    initRaceCameraPositionTransition,
    initRaceCameraPositionTransition,
    initRaceCameraPositionTransition,
    initRaceCameraPositionTransition,
    initRaceCameraPositionTransition,
    initRaceCameraPositionTransition,
    initRaceCameraPositionTransition,
    initRaceCameraPositionTransition,
    initRaceCameraPositionTransition,
    initRaceCameraPositionTransition,
    initRaceCameraPositionTransition,
    initRaceCameraStaticFollow,
    initRaceCameraIntroPan,
    initRaceCameraMenuPreview,
};

s16 gRaceCameraChaseYawOffsets[] = {
    0x0000, 0x0200, 0x0E00, 0x0400, 0x0C00, 0x0600, 0x0A00, 0x0800, 0x0000, 0x0000,
};

u8 gRaceCameraChaseYawPreferenceOrder[] = {
    0, 1, 7, 2, 6, 3, 5, 4,
};

RaceCameraRotationTransition gRaceCameraRotationTransitions[] = {
    { 0x38,
     0, { 0x00EA5501, 0xFE950245, 0xF6D81077 },
     0x0000, 0x05C6,
     { 0xFF5795ED, 0xFDE30245, 0xF4AB048D },
     0x0000, 0x0605 },
    { 0x32,
     0, { 0xFFBA05F5, 0xFE1C2666, 0xF509F544 },
     0x0EA5, 0x05B6,
     { 0xFE2A501B, 0xFD742666, 0xF3213D8E },
     0x0FBD, 0x06BC },
    { 0x34,
     0, { 0xF60AE4E1, 0xF885A3BB, 0xEA1B898C },
     0x0F9D, 0x0E59,
     { 0xF4B76D11, 0xF785C3BB, 0xE80CAB6E },
     0x0FE2, 0x0CDC },
    { 0x30,
     0, { 0xF5457AAF, 0xF7C00949, 0xE87C4914 },
     0x007C, 0x0556,
     { 0xF3C6BE35, 0xF6EB4949, 0xE6B604A6 },
     0x002F, 0x067B },
    { 0x24,
     0, { 0xDC78FF4D, 0xE882BDA6, 0xE74E451E },
     0x0028, 0x0163,
     { 0xDB1C7AC3, 0xE8459DA6, 0xE887C3DC },
     0x0F11, 0x021B },
    { 0x13,
     0, { 0xD617111A, 0xE7402278, 0xEBA1D9F7 },
     0x0FFF, 0x0C2E,
     { 0xD5A3CB94, 0xE710884A, 0xEC8A99AA },
     0x001D, 0x0CFC },
    { 0x1D,
     0, { 0xD5B494BC, 0xE7124CD0, 0xEC877DEE },
     0x0F78, 0x05DD,
     { 0xD45AB7A2, 0xE6AF8CD0, 0xEDCE829A },
     0x0EF0, 0x05DA },
    { 0x2C,
     0, { 0xD4C0433C, 0xE6B3540D, 0xEDDE8529 },
     0x0EE3, 0x06CC,
     { 0xD314E96C, 0xE558D40D, 0xF1602719 },
     0x0F5C, 0x06CC },
};

RaceCameraTransition gRaceCameraPositionTransitions[] = {
    { 0, 0x34, { 0xF2D97CE2, 0xF67DD3B6, 0xE5BDA99D }, { 0xF205259E, 0xF60253B6, 0xE447B7FD } },
    { 1, 0x30, { 0xFCB5285D, 0xFD17E274, 0xF129A024 }, { 0xFBEF8C11, 0xFCDD2274, 0xF0541664 } },
    { 2, 0x60, { 0xDA58478E, 0xE82482BE, 0xE8D93289 }, { 0xD698BBC8, 0xE78122BE, 0xEAF102B3 } },
    { 0, 0x11, { 0xD2E8C6B6, 0xE5349196, 0xF2192B16 }, { 0xD2D3E0DE, 0xE51BF196, 0xF245BB32 } },
    { 1, 0x34, { 0xD4B9ADC6, 0xE704FD7E, 0xEFF512C0 }, { 0xD3777B24, 0xE5DA5D7E, 0xF2082C88 } },
    { 3, 0x30, { 0xD3257B4F, 0xE61DCBD2, 0xEF04C065 }, { 0xD230D197, 0xE580CBD2, 0xF0CEB75F } },
    { 2, 0x33, { 0xF8607235, 0xFBC2AF59, 0xED508CA1 }, { 0xF8607235, 0xFBC2AF59, 0xED508CA1 } },
    { 1, 0x31, { 0xF8F7A953, 0xFC3490A8, 0xEE024FE0 }, { 0xF7B0D353, 0xFACDB0A8, 0xEC1E9F12 } },
    { 0, 0x34, { 0xEED72CC9, 0xF43C7D0E, 0xDFB9A879 }, { 0xED2A8B23, 0xF3B11D0E, 0xDF6E175D } },
    { 2, 0x31, { 0xEA3CF1A5, 0xF290C452, 0xDF241B6E }, { 0xE95F2D03, 0xF2516452, 0xDF65BEC8 } },
    { 1, 0x33, { 0xEF6F618E, 0xF467FB39, 0xDFDBBE9C }, { 0xEE6188E0, 0xF40E5B39, 0xDF7491B6 } },
    { 3, 0x3E, { 0xEEED56A1, 0xF451496A, 0xE00D77B0 }, { 0xECC6D0D7, 0xF405096A, 0xDF21D656 } },
    { 0, 0x0F, { 0xE21D1637, 0xED2A73FF, 0xE3E16721 }, { 0xE21D1637, 0xED3BB3FF, 0xE3E16721 } },
};

RaceCamera gRaceCameras[RACE_CAMERA_COUNT];
RaceCamera *gCurrentRaceCamera;

void setRaceCameraMode(u16 arg0, u16 arg1) {
    RaceCamera *temp;

    if (gRaceCameraModeChangeDisabled == 0) {
        temp = &gRaceCameras[arg0];
        temp->mode = arg1;
        temp->update = gRaceCameraModeUpdates[arg1];
    }
}

void setRaceCameraModeForced(u16 arg0, u16 arg1) {
    RaceCamera *temp = &gRaceCameras[arg0];

    temp->mode = arg1;
    temp->update = gRaceCameraModeUpdates[arg1];
}

void resetRaceCameras(void) {
    RaceCamera *camera;

    setRaceCameraMode(0, 0);
    camera = gRaceCameras;
    camera->initialized.value = 0;
    camera->pitch = 0;
    camera->yaw = 0;
    camera->roll = 0;
    camera->pos.x = 0;
    camera->pos.y = 0;
    camera->pos.z = 0;
    camera->distance = 0;
    camera->unk28 = 0;
    gRaceCameras[1] = gRaceCameras[0];
    gRaceCameras[2] = gRaceCameras[0];
    gRaceCameras[3] = gRaceCameras[0];
    gRaceCameras[0].playerIndex = 0;
    gRaceCameras[1].playerIndex = 1;
    gRaceCameras[2].playerIndex = 2;
    gRaceCameras[3].playerIndex = 3;
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void updateRaceCameras(void) {
    RaceCamera *camera;
    s32 index;

    index = 0;
    if (gRacePlayerCount > 0) {
 camera = gRaceCameras; do {
            (gCurrentRaceCamera = camera)->update();
            index += 1;
            camera += 1;
        } while (index < gRacePlayerCount);
    }
}
// clang-format on

void updateRaceCamera(s32 arg0) {
    gCurrentRaceCamera = &gRaceCameras[arg0];
    gCurrentRaceCamera->update();
}

void updateRaceCameraTransformFromAngles(void) {
    StackD7D4 stack;

    makeFixedRotationYX(
        gCurrentRaceCamera->cameraTransform.rotation,
        -gCurrentRaceCamera->pitch,
        -gCurrentRaceCamera->yaw
    );
    stack.offset.x = 0;
    stack.offset.y = 0;
    stack.offset.z = -gCurrentRaceCamera->distance;
    makeFixedRotationXY(stack.transform.rotation, gCurrentRaceCamera->pitch, gCurrentRaceCamera->yaw);
    transformVec3iByFixedMatrix(stack.transform.rotation, &stack.offset, &stack.transformed);
    gCurrentRaceCamera->cameraTransform.translation.x = stack.transformed.x - gCurrentRaceCamera->pos.x;
    gCurrentRaceCamera->cameraTransform.translation.y = stack.transformed.y - gCurrentRaceCamera->pos.y;
    gCurrentRaceCamera->cameraTransform.translation.z = stack.transformed.z - gCurrentRaceCamera->pos.z;
    packFixedTransformMatrix(&stack.transform, &gCurrentRaceCamera->packedTransform);
}

void updateRaceCameraLookAtTransform(void) {
    s32 dx;
    s32 dy;
    s32 dz;
    s32 xzDist;
    s32 dist;
    s32 cosine;
    s32 sine;
    Transform3D pitchMtx;
    Transform3D yawMtx;
    Transform3D resultTransform;
    s32 pad[1];
    s32 i;
    s32 j;

    initFixedTransform(&pitchMtx);
    initFixedTransform(&yawMtx);
    dx = gCurrentRaceCamera->pos.x - gCurrentRaceCamera->focus.x;
    pad[0] = pad[0];

    dy = (gCurrentRaceCamera->pos.y - gCurrentRaceCamera->focus.y) + 0x40000;
    dz = gCurrentRaceCamera->pos.z - gCurrentRaceCamera->focus.z;

    xzDist = integerSquareRoot64((s64)dx * dx + (s64)dz * dz);
    dist = integerSquareRoot64((s64)xzDist * xzDist + (s64)dy * dy);

    gCurrentRaceCamera->pitch = calculateFixedAngleBetweenXZPoints(0, 0, xzDist, -dy);
    if (dist != 0) {
        sine = ((s64)dy * 0x1000) / dist;
        cosine = ((s64)xzDist * 0x1000) / dist;
        pitchMtx.rotation[MTX_YY] = cosine;
        pitchMtx.rotation[MTX_ZY] = -sine;
        pitchMtx.rotation[MTX_ZZ] = cosine;
        pitchMtx.rotation[MTX_YZ] = sine;
    }

    gCurrentRaceCamera->yaw = -calculateFixedAngleBetweenXZPoints(0, 0, dx, dz);
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
            FIXED_MATRIX_ROWS(gCurrentRaceCamera->cameraTransform.rotation)
            [i][j] = FIXED_MUL(FIXED_MATRIX_ROWS(yawMtx.rotation)[i][0], FIXED_MATRIX_ROWS(pitchMtx.rotation)[0][j]) +
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
            resultTransform.rotation[(i * 3) + j] = FIXED_MUL(pitchMtx.rotation[i * 3], yawMtx.rotation[j]) +
                                                    FIXED_MUL(pitchMtx.rotation[(i * 3) + 1], yawMtx.rotation[j + 3]) +
                                                    FIXED_MUL(pitchMtx.rotation[(i * 3) + 2], yawMtx.rotation[j + 6]);
        }
    }

    gCurrentRaceCamera->cameraTransform.translation.x =
        -((((s64)resultTransform.rotation[MTX_YX] * gCurrentRaceCamera->unk28) / 0x10000) + gCurrentRaceCamera->pos.x);
    gCurrentRaceCamera->cameraTransform.translation.y =
        -((((s64)resultTransform.rotation[MTX_YY] * gCurrentRaceCamera->unk28) / 0x10000) + gCurrentRaceCamera->pos.y);
    gCurrentRaceCamera->cameraTransform.translation.z =
        -((((s64)resultTransform.rotation[MTX_YZ] * gCurrentRaceCamera->unk28) / 0x10000) + gCurrentRaceCamera->pos.z);
    packFixedTransformMatrix(&resultTransform, &gCurrentRaceCamera->packedTransform);
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
    Transform3D pitchMtx;
    Transform3D yawMtx;
    Transform3D resultTransform;
    s32 pad[1];
    s32 i;
    s32 j;

    initFixedTransform(&pitchMtx);
    initFixedTransform(&yawMtx);
    unused[0] = unused[0];
    dx = gCurrentRaceCamera->pos.x - gCurrentRaceCamera->focus.x;
    pad[0] = pad[0];

    dy = (gCurrentRaceCamera->pos.y - gCurrentRaceCamera->focus.y) + 0x40000;
    dz = gCurrentRaceCamera->pos.z - gCurrentRaceCamera->focus.z;

    xzDist = integerSquareRoot64((s64)dx * dx + (s64)dz * dz);
    dist = integerSquareRoot64((s64)xzDist * xzDist + (s64)dy * dy);

    gCurrentRaceCamera->pitch = calculateFixedAngleBetweenXZPoints(0, 0, xzDist, -dy);
    if (dist != 0) {
        sine = ((s64)dy * 0x1000) / dist;
        cosine = ((s64)xzDist * 0x1000) / dist;
        pitchMtx.rotation[MTX_YY] = cosine;
        pitchMtx.rotation[MTX_ZY] = -sine;
        pitchMtx.rotation[MTX_ZZ] = cosine;
        pitchMtx.rotation[MTX_YZ] = sine;
    }

    gCurrentRaceCamera->yaw = -calculateFixedAngleBetweenXZPoints(0, 0, dx, dz);
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
            FIXED_MATRIX_ROWS(gCurrentRaceCamera->cameraTransform.rotation)
            [i][j] = FIXED_MUL(FIXED_MATRIX_ROWS(yawMtx.rotation)[i][0], FIXED_MATRIX_ROWS(pitchMtx.rotation)[0][j]) +
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
            resultTransform.rotation[(i * 3) + j] = FIXED_MUL(pitchMtx.rotation[i * 3], yawMtx.rotation[j]) +
                                                    FIXED_MUL(pitchMtx.rotation[(i * 3) + 1], yawMtx.rotation[j + 3]) +
                                                    FIXED_MUL(pitchMtx.rotation[(i * 3) + 2], yawMtx.rotation[j + 6]);
        }
    }

    gCurrentRaceCamera->cameraTransform.translation.x =
        -((((s64)resultTransform.rotation[MTX_YX] * gCurrentRaceCamera->unk28) / 0x10000) + gCurrentRaceCamera->pos.x);
    gCurrentRaceCamera->cameraTransform.translation.y =
        -((((s64)resultTransform.rotation[MTX_YY] * gCurrentRaceCamera->unk28) / 0x10000) + gCurrentRaceCamera->pos.y);
    gCurrentRaceCamera->cameraTransform.translation.z =
        -((((s64)resultTransform.rotation[MTX_YZ] * gCurrentRaceCamera->unk28) / 0x10000) + gCurrentRaceCamera->pos.z);
    packFixedTransformMatrix(&resultTransform, &gCurrentRaceCamera->packedTransform);
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
    player = &gRacePlayers[gCurrentRaceCamera->playerIndex];
    if (player->state.stateFlags & 0x400) {
        offset.z = 0xFFC00000;
        player = (RacePlayer *)(s32)&gRacePlayers[gCurrentRaceCamera->playerIndex];
    }
    makeFixedRotationY(matrix, player->state.facingAngle);
    transformVec3iByFixedMatrix(matrix, &offset, &transformedOffset);
    gCurrentRaceCamera->focus.x = gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.x;
    gCurrentRaceCamera->focus.y = gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.y;
    gCurrentRaceCamera->focus.z = gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.z;
    gCurrentRaceCamera->pos.x = gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.x + transformedOffset.x;
    gCurrentRaceCamera->pos.y = gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.y + transformedOffset.y;
    gCurrentRaceCamera->pos.z = gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.z + transformedOffset.z;
    gCurrentRaceCamera->prevPos.x = gRacePlayers[gCurrentRaceCamera->playerIndex].state.pos.x;
    gCurrentRaceCamera->prevPos.y = gRacePlayers[gCurrentRaceCamera->playerIndex].state.pos.y;
    gCurrentRaceCamera->prevPos.z = gRacePlayers[gCurrentRaceCamera->playerIndex].state.pos.z;
    gCurrentRaceCamera->distance = 0;
    gCurrentRaceCamera->unk28 = 0x012C0000;
    gCurrentRaceCamera->update = updateRaceCameraFollowPlayer;
    gCurrentRaceCamera->update();
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
        gCurrentRaceCamera->focus.x +=
            (gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.x - gCurrentRaceCamera->focus.x) >> 1;
        gCurrentRaceCamera->focus.y +=
            (gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.y - gCurrentRaceCamera->focus.y) >> 1;
        gCurrentRaceCamera->focus.z +=
            (gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.z - gCurrentRaceCamera->focus.z) >> 1;

        x = gCurrentRaceCamera->pos.x;
        y = gCurrentRaceCamera->pos.y;
        z = gCurrentRaceCamera->pos.z;

        dx = gCurrentRaceCamera->focus.x - gCurrentRaceCamera->pos.x;
        dy = gCurrentRaceCamera->focus.y - gCurrentRaceCamera->pos.y;
        dz = gCurrentRaceCamera->focus.z - gCurrentRaceCamera->pos.z;

        distance = integerSquareRoot64(((s64)dx * dx) + ((s64)dz * dz));

        if (distance == 0) {
            distance = 1;
            dz = 1;
        }

        if (distance < 0x400000) {
            dx = ((s64)dx * 0x400000) / distance;
            dz = ((s64)dz * 0x400000) / distance;

            x = gCurrentRaceCamera->focus.x - dx;
            z = gCurrentRaceCamera->focus.z - dz;
        }

        distance = integerSquareRoot64((((s64)dx * dx) + ((s64)dy * dy)) + ((s64)dz * dz));

        if (distance > (0x460000 - gCurrentRaceCamera->distance)) {
            dx = ((s64)dx * (0x460000 - gCurrentRaceCamera->distance)) / distance;
            dy = ((s64)dy * (0x460000 - gCurrentRaceCamera->distance)) / distance;
            dz = ((s64)dz * (0x460000 - gCurrentRaceCamera->distance)) / distance;

            x = gCurrentRaceCamera->focus.x - dx;
            y = gCurrentRaceCamera->focus.y - dy;
            z = gCurrentRaceCamera->focus.z - dz;
        } else if (distance < (0x458000 - gCurrentRaceCamera->distance)) {
            dx = ((s64)dx * (0x458000 - gCurrentRaceCamera->distance)) / distance;
            dy = ((s64)dy * (0x458000 - gCurrentRaceCamera->distance)) / distance;
            if (gRacePlayers) {}
            dz = ((s64)dz * (0x458000 - gCurrentRaceCamera->distance)) / distance;

            x = gCurrentRaceCamera->focus.x - dx;
            y = gCurrentRaceCamera->focus.y - dy;
            z = gCurrentRaceCamera->focus.z - dz;
        }

        if (!(gRacePlayers[gCurrentRaceCamera->playerIndex].state.stateFlags & 0x1000)) {
            distance =
                findRaceCourseSurfaceFromHint(gRacePlayers[gCurrentRaceCamera->playerIndex].state.coursePathIndex, x, z);
            delta = getRaceCourseSurfaceHeight((s16)distance, x, z) - 0x40000;

            if (y < delta) {
                y = delta;
            }
        }

        gCurrentRaceCamera->pos.x = (gCurrentRaceCamera->pos.x + x) - gCurrentRaceCamera->pos.x;
        gCurrentRaceCamera->pos.y = (gCurrentRaceCamera->pos.y + y) - gCurrentRaceCamera->pos.y;
        gCurrentRaceCamera->pos.z = (gCurrentRaceCamera->pos.z + z) - gCurrentRaceCamera->pos.z;

        delta = 0x12C0000;

        if (gRacePlayers[gCurrentRaceCamera->playerIndex].state.stateFlags & 0x800) {
            delta = 0;
        }

        delta -= gCurrentRaceCamera->unk28;

        if (delta > 0x96000) {
            delta = 0x96000;
        }

        if (delta < 0) {
            delta /= 6;
        }

        gCurrentRaceCamera->unk28 += delta;

        delta = 0;

        if (gRacePlayers[gCurrentRaceCamera->playerIndex].state.stateFlags & 0x800) {
            delta = 0x280000;
        }

        delta -= gCurrentRaceCamera->distance;

        if (delta < -0x14000) {
            delta = -0x14000;
        }

        if (delta > 0) {
            delta /= 6;
        }

        gCurrentRaceCamera->distance += delta;
    }

    gCurrentRaceCamera->prevPos.x = gRacePlayers[gCurrentRaceCamera->playerIndex].state.pos.x;
    gCurrentRaceCamera->prevPos.y = gRacePlayers[gCurrentRaceCamera->playerIndex].state.pos.y;
    gCurrentRaceCamera->prevPos.z = gRacePlayers[gCurrentRaceCamera->playerIndex].state.pos.z;

    updateRaceCameraAlternateLookAtTransform();
}

void noopRaceCameraDebugUpdate(void) {
}

void unusedRaceCameraNoop2(void) {
}

void initRaceCameraCourseStart(void) {
    gCurrentRaceCamera->update = updateRaceCameraCourseStart;
    gCurrentRaceCamera->update();
}

void updateRaceCameraCourseStart(void) {
    s32 diff;

    if (gRaceUpdatePaused == 0) {
        gCurrentRaceCamera->focus.x +=
            (gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.x - gCurrentRaceCamera->focus.x) >> 1;
        gCurrentRaceCamera->focus.y +=
            (gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.y - gCurrentRaceCamera->focus.y) >> 1;
        gCurrentRaceCamera->focus.z +=
            (gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.z - gCurrentRaceCamera->focus.z) >> 1;

        diff = 0x640000 - gCurrentRaceCamera->unk28;
        if (diff >= 0x10001) {
            diff = 0x10000;
        }
        if (diff < -0x10000) {
            diff = -0x10000;
        }
        gCurrentRaceCamera->unk28 += diff;

        diff = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].courseStartCameraPosition.x -
               gCurrentRaceCamera->pos.x;
        if (diff >= 0xC001) {
            diff = 0xC000;
        }
        if (diff < -0xC000) {
            diff = -0xC000;
        }
        gCurrentRaceCamera->pos.x += diff;

        diff = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].courseStartCameraPosition.y -
               gCurrentRaceCamera->pos.y;
        if (diff >= 0xC001) {
            diff = 0xC000;
        }
        if (diff < -0xC000) {
            diff = -0xC000;
        }
        gCurrentRaceCamera->pos.y += diff;

        diff = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].courseStartCameraPosition.z -
               gCurrentRaceCamera->pos.z;
        if (diff >= 0xC001) {
            diff = 0xC000;
        }
        if (diff < -0xC000) {
            diff = -0xC000;
        }
        gCurrentRaceCamera->pos.z += diff;
    }
    updateRaceCameraLookAtTransform();
}

void initRaceCameraFixedPositionFollow(void) {
    gCurrentRaceCamera->update = updateRaceCameraFixedPositionFollow;
    gCurrentRaceCamera->update();
}

void updateRaceCameraFixedPositionFollow(void) {
    if (gRaceUpdatePaused == 0) {
        gCurrentRaceCamera->focus.x +=
            (gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.x - gCurrentRaceCamera->focus.x) >> 1;
        gCurrentRaceCamera->focus.y +=
            (gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.y - gCurrentRaceCamera->focus.y) >> 1;
        gCurrentRaceCamera->focus.z +=
            (gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.z - gCurrentRaceCamera->focus.z) >> 1;
    }
    updateRaceCameraLookAtTransform();
}

void initRaceCameraChase(void) {
    gCurrentRaceCamera->timer = 0x96;
    gCurrentRaceCamera->update = updateRaceCameraChase;
    gCurrentRaceCamera->update();
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
        gCurrentRaceCamera->focus.x +=
            (gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.x - gCurrentRaceCamera->focus.x) >> 1;
        gCurrentRaceCamera->focus.y +=
            (gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.y - gCurrentRaceCamera->focus.y) >> 1;
        gCurrentRaceCamera->focus.z +=
            (gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.z - gCurrentRaceCamera->focus.z) >> 1;
        targetYaw = gRacePlayers[gCurrentRaceCamera->playerIndex].state.facingAngle - 0x300;
        blockedAngles = targetYaw * 0;
        i = 0;

        if (gRacePlayerCount > 0) {
            do {
                if (i != gCurrentRaceCamera->playerIndex) {
                    dx = gRacePlayers[gCurrentRaceCamera->playerIndex].state.pos.x - gRacePlayers[i].state.pos.x;
                    dz = gRacePlayers[gCurrentRaceCamera->playerIndex].state.pos.z - gRacePlayers[i].state.pos.z;
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
        angle = calculateFixedAngleBetweenXZPoints(
            gCurrentRaceCamera->focus.x,
            gCurrentRaceCamera->focus.z,
            gCurrentRaceCamera->pos.x,
            gCurrentRaceCamera->pos.z
        );
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
        gCurrentRaceCamera->unk92 = yaw;

        dx = gCurrentRaceCamera->pos.x - gCurrentRaceCamera->focus.x;
        dz = gCurrentRaceCamera->pos.z - gCurrentRaceCamera->focus.z;
        dist = integerSquareRoot64((s64)dx * dx + (s64)dz * dz);
        dist = ((0x300000 - dist) >> 1) + dist;

        sine = fixedSine(yaw);
        cosine = fixedCosine(yaw);
        gCurrentRaceCamera->pos.x = (((s64)sine * -dist) / 0x1000) + gCurrentRaceCamera->focus.x;
        gCurrentRaceCamera->pos.z = (((s64)cosine * -dist) / 0x1000) + gCurrentRaceCamera->focus.z;
        gCurrentRaceCamera->pos.y += (gCurrentRaceCamera->focus.y - gCurrentRaceCamera->pos.y) >> 1;
        gCurrentRaceCamera->unk28 += (0x960000 - gCurrentRaceCamera->unk28) >> 1;
        if (gMenuFlowState & 0x20) {
            gCurrentRaceCamera->update = updateRaceCameraFinishOrbit;
        }
    }

    gCurrentRaceCamera->prevPos.x = gRacePlayers[gCurrentRaceCamera->playerIndex].state.pos.x;
    gCurrentRaceCamera->prevPos.y = gRacePlayers[gCurrentRaceCamera->playerIndex].state.pos.y;
    gCurrentRaceCamera->prevPos.z = gRacePlayers[gCurrentRaceCamera->playerIndex].state.pos.z;
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
        gCurrentRaceCamera->focus.x +=
            (gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.x - gCurrentRaceCamera->focus.x) >> 1;
        gCurrentRaceCamera->focus.y +=
            (gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.y - gCurrentRaceCamera->focus.y) >> 1;
        gCurrentRaceCamera->focus.z +=
            (gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.z - gCurrentRaceCamera->focus.z) >> 1;

        dx = gCurrentRaceCamera->pos.x - gCurrentRaceCamera->focus.x;
        dz = gCurrentRaceCamera->pos.z - gCurrentRaceCamera->focus.z;
        dist = integerSquareRoot64((s64)dx * dx + (s64)dz * dz);
        dist = ((0x400000 - dist) >> 4) + dist;

        gCurrentRaceCamera->unk92 += 8;
        sine = fixedSine(gCurrentRaceCamera->unk92);
        cosine = fixedCosine(gCurrentRaceCamera->unk92);

        gCurrentRaceCamera->pos.x = (((s64)sine * -dist) / 0x1000) + gCurrentRaceCamera->focus.x;
        gCurrentRaceCamera->pos.z = (((s64)cosine * -dist) / 0x1000) + gCurrentRaceCamera->focus.z;
        gCurrentRaceCamera->pos.y += ((gCurrentRaceCamera->focus.y - gCurrentRaceCamera->pos.y) + 0x100000) >> 4;
        gCurrentRaceCamera->unk28 += (0x960000 - gCurrentRaceCamera->unk28) >> 1;
    }

    gCurrentRaceCamera->prevPos.x = gRacePlayers[gCurrentRaceCamera->playerIndex].state.pos.x;
    gCurrentRaceCamera->prevPos.y = gRacePlayers[gCurrentRaceCamera->playerIndex].state.pos.y;
    gCurrentRaceCamera->prevPos.z = gRacePlayers[gCurrentRaceCamera->playerIndex].state.pos.z;
    updateRaceCameraLookAtTransform();
}

void noopRaceCameraReplayUpdate(void) {
}

void unusedRaceCameraNoop3(void) {
}

void initRaceCameraReplayPosition(void) {
    gCurrentRaceCamera->pos.x = gRaceCameraReplayStartX;
    gCurrentRaceCamera->prevPos.x = gCurrentRaceCamera->pos.x;
    gCurrentRaceCamera->pos.y = gRaceCameraReplayStartY;
    gCurrentRaceCamera->prevPos.y = gCurrentRaceCamera->pos.y;
    gCurrentRaceCamera->pos.z = gRaceCameraReplayStartZ;
    gCurrentRaceCamera->prevPos.z = gCurrentRaceCamera->pos.z;
    gCurrentRaceCamera->distance = 0;
    gCurrentRaceCamera->unk28 = 0;
    gCurrentRaceCamera->update = updateRaceCameraReplayPosition;
    gCurrentRaceCamera->update();
}

void updateRaceCameraReplayPosition(void) {

    gCurrentRaceCamera->focus.x = gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.x;
    gCurrentRaceCamera->focus.y = gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.y;
    gCurrentRaceCamera->focus.z = gRacePlayers[gCurrentRaceCamera->playerIndex].state.projectedPos.z;
    updateRaceCameraLookAtTransform();
}

void updateRaceCameraRotationTransition(void) {
    register s32 stride;
    register s16 timer;
    register s16 diff;

    stride = RACE_CAMERA_ROTATION_TRANSITION_STRIDE;
    timer = gCurrentRaceCamera->timer;
    if (RACE_CAMERA_ROTATION_TRANSITION[-8].duration < timer) {
        timer = RACE_CAMERA_ROTATION_TRANSITION[-8].duration;
    }

    gCurrentRaceCamera->pos.x =
        (((s64)(RACE_CAMERA_ROTATION_TRANSITION[-8].endPos.x - RACE_CAMERA_ROTATION_TRANSITION[-8].startPos.x) *
          timer) /
         ((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions +
                                           ((u16)gCurrentRaceCamera->mode * stride)))[-8]
             .duration) +
        ((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions +
                                          ((u16)gCurrentRaceCamera->mode * stride)))[-8]
            .startPos.x;
    gCurrentRaceCamera->pos.y = (((s64)(((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions +
                                                                          ((u16)gCurrentRaceCamera->mode * stride)))[-8]
                                            .endPos.y -
                                        ((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions +
                                                                          ((u16)gCurrentRaceCamera->mode * stride)))[-8]
                                            .startPos.y) *
                                  timer) /
                                 ((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions +
                                                                   ((u16)gCurrentRaceCamera->mode * stride)))[-8]
                                     .duration) +
                                ((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions +
                                                                  ((u16)gCurrentRaceCamera->mode * stride)))[-8]
                                    .startPos.y;
    gCurrentRaceCamera->pos.z = (((s64)(((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions +
                                                                          ((u16)gCurrentRaceCamera->mode * stride)))[-8]
                                            .endPos.z -
                                        ((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions +
                                                                          ((u16)gCurrentRaceCamera->mode * stride)))[-8]
                                            .startPos.z) *
                                  timer) /
                                 ((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions +
                                                                   ((u16)gCurrentRaceCamera->mode * stride)))[-8]
                                     .duration) +
                                ((RaceCameraRotationTransition *)((u8 *)gRaceCameraRotationTransitions +
                                                                  ((u16)gCurrentRaceCamera->mode * stride)))[-8]
                                    .startPos.z;

    diff = (RACE_CAMERA_ROTATION_TRANSITION[-8].endPitch - RACE_CAMERA_ROTATION_TRANSITION[-8].startPitch) &
           RACE_CAMERA_ANGLE_MASK;
    if (diff >= 0x801) {
        diff -= 0x1000;
    }
    diff = (diff * timer) / RACE_CAMERA_ROTATION_TRANSITION[-8].duration;
    gCurrentRaceCamera->pitch = RACE_CAMERA_ROTATION_TRANSITION[-8].startPitch + (s16)diff;

    diff = (RACE_CAMERA_ROTATION_TRANSITION[-8].endYaw - RACE_CAMERA_ROTATION_TRANSITION[-8].startYaw) &
           RACE_CAMERA_ANGLE_MASK;
    if (diff >= 0x801) {
        diff -= 0x1000;
    }
    diff = (diff * timer) / RACE_CAMERA_ROTATION_TRANSITION[-8].duration;
    gCurrentRaceCamera->yaw = RACE_CAMERA_ROTATION_TRANSITION[-8].startYaw + diff;

    gCurrentRaceCamera->prevPos.x = gCurrentRaceCamera->pos.x;
    gCurrentRaceCamera->prevPos.y = gCurrentRaceCamera->pos.y;
    gCurrentRaceCamera->prevPos.z = gCurrentRaceCamera->pos.z;
    gCurrentRaceCamera->timer++;
    updateRaceCameraTransformFromAngles();
}

void initRaceCameraRotationTransition(void) {
    gCurrentRaceCamera->roll = 0;
    gCurrentRaceCamera->distance = 0;
    gCurrentRaceCamera->update = updateRaceCameraRotationTransition;
    gCurrentRaceCamera->timer = 0;
    gCurrentRaceCamera->update();
}

void initRaceCameraPositionTransition(void) {
    gCurrentRaceCamera->distance = 0;
    gCurrentRaceCamera->unk28 = 0;
    gCurrentRaceCamera->timer = 0;
    gCurrentRaceCamera->update = updateRaceCameraPositionTransition;
    gCurrentRaceCamera->update();
}

/* Modes 16..28 interpolate position while looking at the selected player.
 * Passing near the camera can turn the look-at sharply without a shot change;
 * title-demo snapshot restores and camera-mode changes are separate events. */
void updateRaceCameraPositionTransition(void) {
    s16 timer;
    s16 duration;

    timer = gCurrentRaceCamera->timer;

    duration = gRaceCameraPositionTransitions[gCurrentRaceCamera->mode - 16].duration;
    if (duration < timer) {
        timer = duration;
    }

    gCurrentRaceCamera->pos.x = (((s64)(gRaceCameraPositionTransitions[gCurrentRaceCamera->mode - 16].endPos.x -
                                        gRaceCameraPositionTransitions[gCurrentRaceCamera->mode - 16].startPos.x) *
                                  timer) /
                                 gRaceCameraPositionTransitions[gCurrentRaceCamera->mode - 16].duration) +
                                gRaceCameraPositionTransitions[gCurrentRaceCamera->mode - 16].startPos.x;
    gCurrentRaceCamera->pos.y = (((s64)(gRaceCameraPositionTransitions[gCurrentRaceCamera->mode - 16].endPos.y -
                                        gRaceCameraPositionTransitions[gCurrentRaceCamera->mode - 16].startPos.y) *
                                  timer) /
                                 gRaceCameraPositionTransitions[gCurrentRaceCamera->mode - 16].duration) +
                                gRaceCameraPositionTransitions[gCurrentRaceCamera->mode - 16].startPos.y;
    gCurrentRaceCamera->pos.z = (((s64)(gRaceCameraPositionTransitions[gCurrentRaceCamera->mode - 16].endPos.z -
                                        gRaceCameraPositionTransitions[gCurrentRaceCamera->mode - 16].startPos.z) *
                                  timer) /
                                 gRaceCameraPositionTransitions[gCurrentRaceCamera->mode - 16].duration) +
                                gRaceCameraPositionTransitions[gCurrentRaceCamera->mode - 16].startPos.z;

    gCurrentRaceCamera->focus.x =
        gRacePlayers[gRaceCameraPositionTransitions[gCurrentRaceCamera->mode - 16].playerIndex].state.projectedPos.x;
    gCurrentRaceCamera->focus.y =
        gRacePlayers[gRaceCameraPositionTransitions[gCurrentRaceCamera->mode - 16].playerIndex].state.projectedPos.y;
    gCurrentRaceCamera->focus.z =
        gRacePlayers[gRaceCameraPositionTransitions[gCurrentRaceCamera->mode - 16].playerIndex].state.projectedPos.z;
    gCurrentRaceCamera->prevPos.x = gCurrentRaceCamera->pos.x;
    gCurrentRaceCamera->prevPos.y = gCurrentRaceCamera->pos.y;
    gCurrentRaceCamera->prevPos.z = gCurrentRaceCamera->pos.z;
    gCurrentRaceCamera->timer++;
    updateRaceCameraLookAtTransform();
}

void initRaceCameraStaticFollow(void) {
    gCurrentRaceCamera->pitch = 0;
    gCurrentRaceCamera->yaw = 0;
    gCurrentRaceCamera->roll = 0;
    gCurrentRaceCamera->pos.x = 0;
    gCurrentRaceCamera->pos.y = 0x310000;
    gCurrentRaceCamera->pos.z = 0x900000;
    gCurrentRaceCamera->distance = 0x670000;
    gCurrentRaceCamera->update = updateRaceCameraStaticFollow;
    gCurrentRaceCamera->update();
}

void updateRaceCameraStaticFollow(void) {
    updateRaceCameraTransformFromAngles();
}

/* Mode 0x1E: capture a fixed focus for the continuous race-start flyby. */
void initRaceCameraIntroPan(void) {
    RacePlayer *player = &gRacePlayers[0];

    gCurrentRaceCamera->pos.x = 0;
    gCurrentRaceCamera->pos.y = 0;
    gCurrentRaceCamera->pos.z = player->state.pos.z + 0xFE600000;
    gCurrentRaceCamera->focus.x = player->state.pos.x;
    gCurrentRaceCamera->focus.y = player->state.pos.y + 0xF0000;
    gCurrentRaceCamera->focus.z = player->state.pos.z;
    gCurrentRaceCamera->timer = 0;
    gCurrentRaceCamera->distance = 0;
    gCurrentRaceCamera->unk28 = 0;
    gCurrentRaceCamera->velocity = 0x10000;
    gCurrentRaceCamera->update = updateRaceCameraIntroPan;
    gCurrentRaceCamera->update();
}

/* Advance Z and derive Y from course height plus a sine-driven offset.
 * The turn near the fixed focus can be rapid without a camera discontinuity. */
void updateRaceCameraIntroPan(void) {
    s16 angle;
    s32 velocity;

    angle = gCurrentRaceCamera->timer;
    if (angle < 0x400) {
        gCurrentRaceCamera->timer = angle + 0x10;
        angle = gCurrentRaceCamera->timer;
    }
    gCurrentRaceCamera->unkA8 = (-fixedSine(angle) * 0xC00) + 0xC00000;

    velocity = gCurrentRaceCamera->velocity;
    if (velocity < 0x80000) {
        gCurrentRaceCamera->velocity = velocity + 0x8000;
        velocity = gCurrentRaceCamera->velocity;
    }

    gCurrentRaceCamera->pos.z += velocity;
    gCurrentRaceCamera->pos.y =
        getRaceCourseSurfaceHeight(
            (s16)findRaceCourseSurfaceFromHint(0, gCurrentRaceCamera->pos.x, gCurrentRaceCamera->pos.z),
            gCurrentRaceCamera->pos.x,
            gCurrentRaceCamera->pos.z
        ) +
        gCurrentRaceCamera->unkA8 + 0xE0000;
    updateRaceCameraLookAtTransform();
}

void initRaceCameraMenuPreview(void) {
    gCurrentRaceCamera->pitch = 0x70;
    gCurrentRaceCamera->yaw = 0;
    gCurrentRaceCamera->roll = 0;
    gCurrentRaceCamera->pos.x = 0;
    gCurrentRaceCamera->pos.y = 0x490000;
    gCurrentRaceCamera->pos.z = 0x900000;
    gCurrentRaceCamera->distance = 0x500000;
    gCurrentRaceCamera->update = updateRaceCameraMenuPreview;
    gCurrentRaceCamera->update();
}

void updateRaceCameraMenuPreview(void) {
    updateRaceCameraTransformFromAngles();
}
