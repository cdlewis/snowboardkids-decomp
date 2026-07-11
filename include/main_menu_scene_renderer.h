#ifndef MAIN_MENU_SCENE_RENDERER_H
#define MAIN_MENU_SCENE_RENDERER_H

#include "common.h"

void addMainMenuSceneModelRenderCallback(s32 modelIndex);
void func_80042920(s32 modelIndex, s32 textureId, s32 paletteId);
void func_8004298C(s32 modelIndex, s32 textureId, s32 paletteId, s32 arg3);
void func_80042A00(s32 modelIndex);
void func_80042A58(s32 modelIndex, s32 renderFrame);

#endif
