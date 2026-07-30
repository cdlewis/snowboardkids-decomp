#ifndef MAIN_MENU_SCENE_MODEL_H
#define MAIN_MENU_SCENE_MODEL_H

#include "common.h"

#define MAIN_MENU_SCENE_MODEL_HANDLE_BASE 0x2D
#define MAIN_MENU_SCENE_MODEL_GEOMETRY_HANDLE_BASE 0x33
#define MAIN_MENU_SCENE_MODEL_TEXTURE_HANDLE_BASE 0x39
#define MAIN_MENU_SCENE_MODEL_ANIMATION_BANK_HANDLE 0x3F

typedef struct MainMenuModelVec3s {
    s16 x;
    s16 y;
    s16 z;
} MainMenuModelVec3s;

typedef union MainMenuModelAnimationBank {
    s32 frameOffsets[1];
    s16 frameData[1];
} MainMenuModelAnimationBank;

typedef struct MainMenuModelPart {
    s32 previousPartOffsetZ;
    s8 displayObjectIndex;
    char pad5[0x1];
    MainMenuModelVec3s rot;
    s32 offsetX;
    s32 offsetY;
} MainMenuModelPart;

typedef struct MainMenuModelTransform {
    s16 rotation[9];
    s16 pad12;
    s32 translation[3];
} MainMenuModelTransform;

typedef struct MainMenuSceneModel {
    s16 sceneModelIndex;
    s16 characterIndex;
    s16 textureId;
    s16 paletteId;
    s16 viewportIndex;
    char padA[0x2];
    s16 *animationStart;
    s16 *animationCursor;
    s16 framesRemaining;
    s16 frameDuration;
    MainMenuModelPart parts[14];
    s32 lastPartOffsetZ;
    Vec3i pos;
    MainMenuModelVec3s rot;
    s16 unk146;
    MainMenuModelTransform displayObjects[14];
} MainMenuSceneModel;

void loadMainMenuSceneModelAssets(void);
void loadMainMenuSceneModelAnimationBank(void);
void initMainMenuSceneModel(s32 sceneModelIndex, s32 characterIndex);
void setMainMenuSceneModelAnimation(s32 modelIndex, s32 animationIndex);
MainMenuSceneModel *getMainMenuSceneModel(s32 modelIndex);
void applyMainMenuSceneModelAnimationFrame(MainMenuSceneModel *model);
s32 stepMainMenuSceneModelAnimation(s32 modelIndex);
void loopMainMenuSceneModelAnimation(s32 modelIndex);
void setMainMenuSceneModelPosition(s32 modelIndex, s32 x, s32 y, s32 z);
void setMainMenuSceneModelRotation(s32 modelIndex, s16 x, s16 y, s16 z);
void updateMainMenuSceneModelTransforms(MainMenuSceneModel *model);
void initMainMenuSceneModelParts(MainMenuSceneModel *model);

#endif
