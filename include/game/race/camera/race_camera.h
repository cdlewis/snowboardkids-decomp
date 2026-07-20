#ifndef RACE_CAMERA_H
#define RACE_CAMERA_H

#include "common.h"

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
