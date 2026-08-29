#ifndef MAIN_MENU_SCENE_MODEL_H
#define MAIN_MENU_SCENE_MODEL_H

#include "common.h"
#include "game/race/player/character_model.h"

#define MAIN_MENU_SCENE_MODEL_HANDLE_BASE 0x2D
#define MAIN_MENU_SCENE_MODEL_GEOMETRY_HANDLE_BASE 0x33
#define MAIN_MENU_SCENE_MODEL_TEXTURE_HANDLE_BASE 0x39
#define MAIN_MENU_SCENE_MODEL_ANIMATION_BANK_HANDLE 0x3F
#define MAIN_MENU_SCENE_MODEL_PART_COUNT 14

typedef struct MainMenuAnimationBank {
    s32 frameOffsets[1];
} MainMenuAnimationBank;

typedef struct MainMenuSceneModel {
    /* 0x000 */ s16 sceneModelIndex;
    /* 0x002 */ s16 characterIndex;
    /* 0x004 */ s16 snowboardDisplayListIndex;
    /* 0x006 */ s16 snowboardTextureIndex;
    /* 0x008 */ s16 viewportIndex;
    /* 0x00A */ char padA[2];
    /* 0x00C */ s16 *animationStart;
    /* 0x010 */ s16 *animationCursor;
    /* 0x014 */ s16 framesRemaining;
    /* 0x016 */ s16 frameCount;
    /* 0x018 */ s32 unk18;
    /* 0x01C */ CharacterModelPart parts[MAIN_MENU_SCENE_MODEL_PART_COUNT];
    /* 0x134 */ Vec3i position;
    /* 0x140 */ Vec3s rotation;
    /* 0x146 */ s16 pad146;
    /* 0x148 */ Transform3D partTransforms[MAIN_MENU_SCENE_MODEL_PART_COUNT];
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
