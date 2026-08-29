#ifndef MAIN_MENU_SCENE_MODEL_RENDERER_H
#define MAIN_MENU_SCENE_MODEL_RENDERER_H

#include "common.h"

void addMainMenuSceneModelDrawCallback(s32 modelIndex);
void addMainMenuSceneModelTexturedDrawCallback(
    s32 modelIndex,
    s32 snowboardDisplayListIndex,
    s32 snowboardTextureIndex
);
void addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(
    s32 modelIndex,
    s32 snowboardDisplayListIndex,
    s32 snowboardTextureIndex,
    s32 unusedArg
);
void addMainMenuSceneModelDrawCallbackForViewport0(s32 modelIndex);
void addMainMenuSceneModelDrawCallbackForViewport(s32 modelIndex, s32 viewportIndex);

#endif
