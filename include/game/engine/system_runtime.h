#ifndef SYSTEM_RUNTIME_H
#define SYSTEM_RUNTIME_H

#include "common.h"
#include "game/engine/system_render.h"
extern Gfx gMenuRenderModeResetDl[];
extern Gfx gTranslucentSpriteRenderModeDl[];
extern Mtx *gViewportMatrix;
extern u8 gMenuFadeOverlayActive;
extern u8 gRenderMatricesDirty;
extern u8 gRaceRecordSettingsEnabled;
extern u8 gRaceCourseModelEffectsDisabled;
extern u8 gRaceCourseOverlayEffectsDisabled;

void dmaReadRom(u32 romOffset, void *ramAddress, s32 size);
void resetRenderCallbackQueues(void);
void setBootFadeColor(u8 a0, u8 a1, u8 a2);
void setTitleFadeColor(u8 a0, u8 a1, u8 a2);

#define USE_ASSET(id)           \
    extern u32 *id##_ROM_START; \
    extern u32 *id##_ROM_END;   \
    extern u32 *id##_VRAM;      \
    extern u32 *id##_VRAM_END

#endif
