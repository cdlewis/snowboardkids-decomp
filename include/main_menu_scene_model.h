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

typedef struct MainMenuModelPart {
    s32 unk0;
    s8 displayObjectIndex;
    char pad5[0x1];
    MainMenuModelVec3s rot;
    s32 offsetX;
    s32 offsetY;
} MainMenuModelPart;

typedef struct MainMenuModelDisplayObject {
    char pad0[0x14];
    s32 screenX;
    s32 screenY;
    s32 screenZ;
} MainMenuModelDisplayObject;

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
    MainMenuModelPart parts[14];
    char pad130[0x4];
    MainMenuModelVec3i pos;
    MainMenuModelVec3s rot;
    s16 unk146;
    MainMenuModelDisplayObject displayObjects[14];
} MainMenuSceneModel;

void func_80041CC0(void);
void func_80041CF0(void);
void func_80041D20(s32 actorIndex, s32 modelIndex);
void func_80041DD4(s32 modelIndex, s32 animationIndex);
MainMenuSceneModel *func_80041E60(s32 modelIndex);
void func_80041E90(MainMenuSceneModel *model);
s32 func_80041FB4(s32 arg0);
void func_80042034(s32 modelIndex);
void func_8004209C(s32 modelIndex, s32 x, s32 y, s32 z);
void func_800420FC(s32 modelIndex, s16 x, s16 y, s16 z);
void func_80042AB4(MainMenuSceneModel *model);

#endif
