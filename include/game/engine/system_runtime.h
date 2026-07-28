#ifndef SYSTEM_RUNTIME_H
#define SYSTEM_RUNTIME_H

#include "common.h"

#define FRAMEBUFFER_RENDER_TASK_STRIDE 0x18620

typedef struct FramebufferRenderTaskStatus {
    /* 0x00000 */ u8 status;
    /* 0x00001 */ u8 pad1[FRAMEBUFFER_RENDER_TASK_STRIDE - 1];
} FramebufferRenderTaskStatus;

extern FramebufferRenderTaskStatus gFramebufferRenderTask0Statuses[];
extern FramebufferRenderTaskStatus gFramebufferRenderTask1Statuses[];
extern Gfx gMenuRenderModeResetDl[];
extern Gfx gTranslucentSpriteRenderModeDl[];
extern Mtx *gViewportMatrix;
extern u8 gMenuFadeOverlayActive;
extern u8 gRenderMatricesDirty;
extern u8 gRaceRecordSettingsEnabled;
extern u8 gRaceCourseModelEffectsDisabled;
extern u8 gRaceCourseOverlayEffectsDisabled;

void main(void *arg);
void bootThreadMain(void *arg);
void gameThreadMain(void *arg);
void initGameSystems(void);
void dmaReadRom(u32 romOffset, void *ramAddress, s32 size);
void appendViewportDisplayLists(u8 frameIndex);
void resetRenderCallbackQueues(void);
void submitFramebufferRenderTask(u8 frameIndex);
void setBootFadeColor(u8 a0, u8 a1, u8 a2);
void setTitleFadeColor(u8 a0, u8 a1, u8 a2);

#define USE_ASSET(id)           \
    extern u32 *id##_ROM_START; \
    extern u32 *id##_ROM_END;   \
    extern u32 *id##_VRAM;      \
    extern u32 *id##_VRAM_END

#endif
