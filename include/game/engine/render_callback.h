#ifndef RENDER_CALLBACK_H
#define RENDER_CALLBACK_H

#include "common.h"

typedef void (*RenderCallback)(void *);

typedef struct RenderCallbackNode {
    struct RenderCallbackNode *next;
    RenderCallback callback;
    void *arg;
} RenderCallbackNode;

void addRenderCallback(RenderCallbackNode **queue, RenderCallback callback, void *arg);
void runRenderCallbacks(RenderCallbackNode **queue);

extern RenderCallbackNode *gMenuOverlayRenderCallbackList;
extern RenderCallbackNode *gMenuForegroundRenderCallbackList;
extern RenderCallbackNode *gMenuRenderCallbackList;
extern RenderCallbackNode *gRaceOverlayRenderCallbackList;
extern RenderCallbackNode *gRaceForegroundRenderCallbackList;
extern RenderCallbackNode *gRaceObjectRenderCallbackList;
extern RenderCallbackNode *gRaceModelEffectRenderCallbackList;
extern RenderCallbackNode *gModelRenderCallbackList;
extern RenderCallbackNode *gBackdropRenderCallbackList;
extern RenderCallbackNode *gEffectRenderCallbackList;
extern RenderCallbackNode *gSceneModelRenderCallbackList;
extern RenderCallbackNode *D_80124848;
extern RenderCallbackNode *D_801248C8;
extern RenderCallbackNode *D_801248E0;
extern RenderCallbackNode *D_801248EC;

#endif
