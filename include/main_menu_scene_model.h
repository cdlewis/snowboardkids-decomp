#ifndef MAIN_MENU_SCENE_MODEL_H
#define MAIN_MENU_SCENE_MODEL_H

#include "common.h"

typedef struct MainMenuModelVec3s {
    s16 x;
    s16 y;
    s16 z;
} MainMenuModelVec3s;

typedef struct MainMenuModelVec3i {
    s32 x;
    s32 y;
    s32 z;
} MainMenuModelVec3i;

typedef union MainMenuModelAnimationBank {
    s32 frameOffsets[1];
    s16 frameData[1];
} MainMenuModelAnimationBank;

typedef struct MainMenuSceneModel {
    s16 actorIndex;
    s16 modelIndex;
    s16 textureId;
    s16 paletteId;
    s16 renderFrame;
    char padA[0x2];
    s16 *animationStart;
    s16 *animationCursor;
    s16 framesRemaining;
    s16 frameDuration;
    char poseData[0x11C];
    MainMenuModelVec3i pos;
    MainMenuModelVec3s rot;
    s16 unk146;
    char displayObjects[14][0x20];
} MainMenuSceneModel;

#endif
