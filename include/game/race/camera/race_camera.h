#ifndef RACE_CAMERA_H
#define RACE_CAMERA_H

#include "common.h"
#include "game/math/geometry.h"

#define RACE_CAMERA_COUNT 4

typedef struct RaceCamera {
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
    /* 0x30 */ Transform3D cameraTransform;
    /* 0x50 */ Mtx packedTransform;
    /* 0x90 */ s16 unk90;
    /* 0x92 */ s16 unk92;
    /* 0x94 */ Vec3i prevPos;
    /* 0xA0 */ s16 timer;
    /* 0xA2 */ u8 padA2[2];
    /* 0xA4 */ s32 velocity;
    /* 0xA8 */ s32 unkA8;
    /* 0xAC */ u8 initialized;
    /* 0xAD */ u8 padAD[3];
} RaceCamera;

extern RaceCamera *gCurrentMenuCameraObject;
extern u8 gRaceCameraModeChangeDisabled;

void setRaceCameraMode(u16 arg0, u16 arg1);
void setRaceCameraModeForced(u16 arg0, u16 arg1);
void resetRaceCameras(void);
void updateRaceCameras(void);
void updateRaceCamera(s32 arg0);
void updateRaceCameraTransformFromAngles(void);
void updateRaceCameraLookAtTransform(void);
void updateRaceCameraAlternateLookAtTransform(void);
void noopRaceCameraUpdate(void);
void unusedRaceCameraNoop1(void);
void initRaceCameraFollowPlayer(void);
void updateRaceCameraFollowPlayer(void);
void noopRaceCameraDebugUpdate(void);
void unusedRaceCameraNoop2(void);
void initRaceCameraCourseStart(void);
void updateRaceCameraCourseStart(void);
void initRaceCameraFixedPositionFollow(void);
void updateRaceCameraFixedPositionFollow(void);
void initRaceCameraChase(void);
void updateRaceCameraChase(void);
void updateRaceCameraFinishOrbit(void);
void noopRaceCameraReplayUpdate(void);
void unusedRaceCameraNoop3(void);
void initRaceCameraReplayPosition(void);
void updateRaceCameraReplayPosition(void);
void updateRaceCameraRotationTransition(void);
void initRaceCameraRotationTransition(void);
void initRaceCameraPositionTransition(void);
void updateRaceCameraPositionTransition(void);
void initRaceCameraStaticFollow(void);
void updateRaceCameraStaticFollow(void);
void initRaceCameraIntroPan(void);
void updateRaceCameraIntroPan(void);
void initRaceCameraMenuPreview(void);
void updateRaceCameraMenuPreview(void);

#endif
