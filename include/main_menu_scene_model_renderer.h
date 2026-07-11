#ifndef MAIN_MENU_SCENE_MODEL_RENDERER_H
#define MAIN_MENU_SCENE_MODEL_RENDERER_H

#include "common.h"

void addMainMenuSceneModelDrawCallback(s32 modelIndex);
void addMainMenuSceneModelTexturedDrawCallback(s32 modelIndex, s32 textureId, s32 paletteId);
void addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(s32 modelIndex, s32 textureId, s32 paletteId, s32 unusedArg);
void addMainMenuSceneModelDrawCallbackViewport0(s32 modelIndex);
void addMainMenuSceneModelDrawCallbackForViewport(s32 modelIndex, s32 renderFrame);

#endif
