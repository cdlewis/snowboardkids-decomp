#ifndef SYSTEM_BOOT_H
#define SYSTEM_BOOT_H

#include "common.h"

void main(void *arg);
void bootThreadMain(void *arg);
void initGameSystems(void);
void dmaReadRom(u32 devAddr, void *dramAddr, s32 size);
void resetRenderCallbackQueues(void);
void submitFramebufferRenderTask(u8 frameIndex);
void setBootFadeColor(u8 a0, u8 a1, u8 a2);
void setTitleFadeColor(u8 a0, u8 a1, u8 a2);

#endif
