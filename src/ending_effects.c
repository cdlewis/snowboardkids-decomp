#include "common.h"
#include "relocatable_heap.h"
#include "callback_task_scheduler.h"
#include "ending_effects.h"
#include "main_menu_scene_model.h"
#define MENU_RENDERER_BROAD_PROTOTYPES
#include "menu_renderer.h"
#include "title_menu.h"

struct EndingEffectActor {
    char pad0[0x10];
    /* 0x10 */ s16 delay;
    /* 0x12 */ char pad12[6];
    union {
        struct {
            /* 0x18 */ s16 x;
            /* 0x1A */ s16 y;
            union {
                /* 0x1C */ s16 offsetX;
                /* 0x1C */ u16 animFrame;
            };
            union {
                /* 0x1E */ s16 offsetY;
                /* 0x1E */ u16 animTimer;
            };
            union {
                /* 0x20 */ s16 spriteState[2];
                struct {
                    /* 0x20 */ s16 angle;
                    /* 0x22 */ s16 angleVelocity;
                };
                struct {
                    /* 0x20 */ u16 effectFrame;
                    /* 0x22 */ u16 effectTimer;
                };
            };
        };
        /* 0x18 */ s16 linePositions[6];
    };
    union {
        /* 0x24 */ s16 frameIndex;
        /* 0x24 */ u16 uFrameIndex;
        struct {
            /* 0x24 */ u8 frameIndexHi;
            /* 0x25 */ u8 palette;
        };
    };
    union {
        /* 0x26 */ s16 introTimer;
        struct {
            /* 0x26 */ u8 characterId;
            /* 0x27 */ u8 pad27;
        };
    };
    /* 0x28 */ s16 startX;
    /* 0x2A */ s16 startY;
};

extern void addRenderCallback(void *, void *, void *);
extern Gfx *gRegionAllocPtr;
extern s32 gMenuOverlayRenderCallbackList;
extern s32 gMenuRenderCallbackList;
extern Gfx gMenuRenderModeResetDl[];
extern s16 gMenuFadeAlpha;
extern s16 gMenuCommonSpritesAssetHandle;
extern s16 gMenuIconTilemapAssetHandle;
extern s16 gMenuPanelTilemapAssetHandle;
extern u16 gEndingSequencePhase;
extern u8 gEndingTransitionRotationStep;
extern s16 gEndingTransitionSnowboardAngle;
extern u8 gEndingCharacterEffectDoneFlags[];
extern u16 gEndingTransitionRotationFrameCount;
extern u16 gEndingCharacterRunDustFrameOffsets[];
extern u16 gEndingSnowmanEntranceFrameOffsets[];
extern s32 gActiveMenuTask;

void updateEndingCharacterVanishPoof(EndingEffectActor *arg0);
void updateEndingNancySpeedLines(EndingEffectActor *arg0);
void drawEndingTommyBigBurst(EndingEffectActor *arg0);
void updateEndingTommyBigBurst(EndingEffectActor *arg0);
void updateEndingSlashRisingStar(EndingEffectActor *arg0);
void updateEndingIdleSparkle(EndingEffectActor *arg0);
void updateEndingTommySnowmanEntrance(EndingEffectActor *arg0);
void updateRaceToMainMenuSnowboardIconSlowSpin(EndingEffectActor *arg0);
void updateRaceToMainMenuSnowboardIconWaitSecondSpin(EndingEffectActor *arg0);
void updateRaceToMainMenuSnowboardIconSecondSpin(EndingEffectActor *arg0);
void drawRaceToMainMenuSnowboardIcon(EndingEffectActor *arg0);
void updateRaceToMainMenuSnowboardIconFastSpin(EndingEffectActor *arg0);
void updateRaceToMainMenuSnowboardIconWaitStart(EndingEffectActor *arg0);
void updateRaceToMainMenuSnowflakeIconIdle(EndingEffectActor *arg0);
void drawRaceToMainMenuLogoWipe(EndingEffectActor *arg0);
void updateRaceToMainMenuLogoWipeOpen(EndingEffectActor *arg0);
void updateRaceToMainMenuLogoWipeClose(EndingEffectActor *arg0);
void updateEndingCharacterLoopingSparkle(EndingEffectActor *arg0);
void updateEndingNancyRunDust(EndingEffectActor *arg0);
void updateEndingSmallBurst(EndingEffectActor *arg0);
void updateEndingCharacterAura(EndingEffectActor *arg0);
void updateEndingPhaseAdvanceSparkle(EndingEffectActor *arg0);
void updateEndingDelayedSparkle(EndingEffectActor *arg0);

void drawEndingCharacterVanishPoof(EndingEffectActor *arg0) {
    drawMenuSprite(arg0->x + arg0->offsetX, arg0->y + arg0->offsetY, getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), ((u16) arg0->angle + 0x32) & 0xFFFF, 0x20, 0x20, arg0->palette, 0);
}

void updateEndingCharacterVanishPoof(EndingEffectActor *arg0) {
    MainMenuSceneModel *model;
    s32 divisor;

    model = getMainMenuSceneModel(arg0->characterId); divisor = 0x24000;
    if (0) {}
    arg0->x = model->displayObjects[7].screenX / divisor;
    arg0->y = -(model->displayObjects[7].screenY / divisor);
    if ((arg0->effectTimer += 1) == 3) {
        arg0->effectTimer = 0;
        arg0->effectFrame++;
    }
    if (arg0->effectFrame == 3) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawEndingCharacterVanishPoof, arg0);
}

void initEndingCharacterVanishPoof(EndingEffectActor *arg0) {
    arg0->angleVelocity = 0;
    arg0->angle = 0;
    setCallbackTaskCallback(arg0, updateEndingCharacterVanishPoof);
}

void spawnEndingCharacterVanishPoof(s16 arg0, s16 arg1, u8 arg2, u8 arg3) {
    EndingEffectActor *temp_v0 = (EndingEffectActor *)(gActiveMenuTask = (s32)createCallbackTask(initEndingCharacterVanishPoof, 0, 0x63));

    temp_v0->offsetX = arg0;
    temp_v0->offsetY = arg1;
    temp_v0->characterId = arg2;
    temp_v0->palette = arg3;
}

void drawEndingNancySpeedLine(EndingEffectActor *arg0) {
    s32 temp = getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle);
    EndingEffectActor *linePosition =
        (EndingEffectActor *)((s32)arg0 + arg0->uFrameIndex * sizeof(arg0->linePositions[0]));

    drawMenuSprite(linePosition->x, linePosition->offsetY, temp, 0xF, 0x20, 0x20, 0, 0);
}

void updateEndingNancySpeedLines(EndingEffectActor *arg0) {
    arg0->introTimer = (u16) arg0->introTimer + 1;
    if ((((u16) arg0->introTimer) % 30) == 0) {
        if (arg0->uFrameIndex < 2) {
            arg0->uFrameIndex++;
        }
    }
    if (((u16) arg0->introTimer) == 0x69) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawEndingNancySpeedLine, arg0);
}

void initEndingNancySpeedLines(EndingEffectActor *arg0) {
    arg0->linePositions[2] = -0x38;
    arg0->linePositions[5] = -0x5A;
    arg0->linePositions[0] = -0x30;
    arg0->linePositions[3] = -0x5D;
    arg0->linePositions[1] = -0x28;
    arg0->linePositions[4] = -0x56;
    arg0->introTimer = 0;
    arg0->frameIndex = 0;
    setCallbackTaskCallback(arg0, updateEndingNancySpeedLines);
}

void drawEndingCharacterLoopingSparkle(EndingEffectActor *arg0) {
    drawMenuSprite(arg0->x + arg0->offsetX, arg0->y + arg0->offsetY, getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), ((u16) arg0->angle + 0x10) & 0xFFFF, 0x20, 0x20, 0, 0);
}

void updateEndingCharacterLoopingSparkle(EndingEffectActor *arg0) {
    MainMenuSceneModel *model;
    s32 divisor;

    model = getMainMenuSceneModel(arg0->characterId); divisor = 0x24000;
    if (0) {}
    arg0->x = model->displayObjects[7].screenX / divisor;
    arg0->y = -(model->displayObjects[7].screenY / divisor);
    if ((arg0->effectTimer += 1) == 8) {
        arg0->effectTimer = 0;
        arg0->effectFrame++;
        arg0->effectFrame &= 3;
    }
    if (gEndingSequencePhase == 0x2F) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawEndingCharacterLoopingSparkle, arg0);
}

void initEndingCharacterLoopingSparkle(EndingEffectActor *arg0) {
    arg0->angleVelocity = 0;
    arg0->angle = 0;
    setCallbackTaskCallback(arg0, updateEndingCharacterLoopingSparkle);
}

void spawnEndingCharacterLoopingSparkle(s16 arg0, s16 arg1, u8 arg2) {
    EndingEffectActor *temp_v0 = (EndingEffectActor *)(gActiveMenuTask = (s32)createCallbackTask(initEndingCharacterLoopingSparkle, 0, 0x63));

    temp_v0->offsetX = arg0;
    temp_v0->offsetY = arg1;
    temp_v0->characterId = arg2;
}

// drawEndingTommyBigBurst best match: 99.074%
#pragma GLOBAL_ASM("asm/nonmatchings/ending_effects/drawEndingTommyBigBurst.s")

#ifdef NON_MATCHING
void drawEndingTommyBigBurst(EndingEffectActor *arg0) {
    Gfx *gfx;
    Gfx *new_var;
    s32 w0;
    s32 w1;

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xE7000000; gfx->words.w1 = 0; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; do { w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; new_var = gfx; new_var->words.w0 = 0xFA000000; new_var->words.w1 = -0x60; } while (0); if (arg0->effectFrame != 0) { drawMenuSprite(arg0->offsetX, arg0->offsetY, getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), (((0, arg0->effectFrame)) + 0x24) & 0xFFFF, 0x30, 0x20, 0, 0); } gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) gMenuRenderModeResetDl; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; if (arg0->effectFrame < 0xC) { drawMenuSprite(arg0->offsetX, arg0->offsetY, getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), (arg0->effectFrame + 0x25) & 0xFFFF, 0x30, 0x20, 0, 0); } drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), 0x31, 0x20, 0x20, 0, 0); } while (0);
}
#endif

void updateEndingTommyBigBurst(EndingEffectActor *arg0) {
    arg0->effectTimer++;
    if (arg0->effectTimer == 3) {
        arg0->effectTimer = 0;
        arg0->effectFrame++;
        switch (arg0->characterId) {
        case 0:
            if (arg0->effectFrame == 4) {
                arg0->characterId = arg0->characterId + 1;
            }
            break;
        case 1:
            if (arg0->effectFrame == 8) {
                arg0->uFrameIndex++;
                if (arg0->uFrameIndex == 3) {
                    arg0->characterId++;
                } else {
                    arg0->effectFrame = 4;
                }
            }
            break;
        case 2:
            break;
        }
    }

    if (arg0->effectFrame == 0xD) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawEndingTommyBigBurst, arg0);
}

void initEndingTommyBigBurst(EndingEffectActor *arg0) {
    arg0->x = -0x3D;
    arg0->y = -0x63;
    arg0->offsetX = -0x4E;
    arg0->offsetY = -0x6C;
    arg0->angleVelocity = 0;
    arg0->frameIndex = 0;
    arg0->angle = 0;
    arg0->characterId = 0;
    setCallbackTaskCallback(arg0, updateEndingTommyBigBurst);
}

void drawEndingSlashRisingStar(EndingEffectActor *arg0) {
    s32 temp;

    temp = getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle);
    drawMenuSprite(arg0->x, arg0->y, temp, (arg0->animFrame + 9) & 0xFFFF, 0x20, 0x20, 0, 0);
}

void updateEndingSlashRisingStar(EndingEffectActor *arg0) {
    EndingEffectActor *temp_a2 = arg0;

    arg0->x++;
    arg0->y--;
    arg0->animTimer++;
    if (arg0->animTimer == 3) {
        arg0->animTimer = 0;
        arg0->animFrame++;
    }
    if (temp_a2->animFrame == 6) {
        removeCallbackTask(temp_a2);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, drawEndingSlashRisingStar, temp_a2);
    }
}

void initEndingSlashRisingStar(EndingEffectActor *arg0) {
    arg0->x = 0x3B;
    arg0->y = -0x38;
    arg0->animTimer = 0;
    arg0->animFrame = 0;
    setCallbackTaskCallback(arg0, updateEndingSlashRisingStar);
}

void drawEndingNancyRunDust(EndingEffectActor *arg0) {
    s32 temp = getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle);
    drawMenuSprite(arg0->x - 0xE, arg0->y - 0x50, temp, (u16)(gEndingCharacterRunDustFrameOffsets[arg0->animFrame] + 6), 0x20, 0x20, 0, 0);
}

void updateEndingNancyRunDust(EndingEffectActor *arg0) {
    MainMenuSceneModel *model;
    s32 divisor;

    model = getMainMenuSceneModel(1); divisor = 0x24000;
    if (0) {}
    arg0->x = model->displayObjects[7].screenX / divisor;
    arg0->y = -(model->displayObjects[7].screenY / divisor);
    if ((arg0->animTimer += 1) == 3) {
        arg0->animTimer = 0;
        arg0->animFrame++;
        arg0->animFrame &= 3;
    }
    if (gEndingSequencePhase == 0x12) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawEndingNancyRunDust, arg0);
}

void initEndingNancyRunDust(EndingEffectActor *arg0) {
    arg0->animTimer = 0;
    arg0->animFrame = 0;
    setCallbackTaskCallback(arg0, updateEndingNancyRunDust);
}

void drawEndingSmallBurst(EndingEffectActor *arg0) {
    s32 temp;

    temp = getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle);
    drawMenuSprite(arg0->x, arg0->y, temp, (arg0->animFrame + 0x14) & 0xFFFF, 0x20, 0x20, 0, 0);
}

void updateEndingSmallBurst(EndingEffectActor *arg0) {
    EndingEffectActor *temp_a2 = arg0;

    arg0->animTimer++;
    if (arg0->animTimer == 2) {
        arg0->animTimer = 0;
        arg0->animFrame++;
    }
    if (temp_a2->animFrame == 5) {
        removeCallbackTask(temp_a2);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, drawEndingSmallBurst, temp_a2);
    }
}

void initEndingSmallBurst(EndingEffectActor *arg0) {
    arg0->animTimer = 0;
    arg0->animFrame = 0;
    setCallbackTaskCallback(arg0, updateEndingSmallBurst);
}

void spawnEndingSmallBurst(s16 arg0, s16 arg1) {
    EndingEffectActor *temp_v0 = (EndingEffectActor *)(gActiveMenuTask = (s32)createCallbackTask(initEndingSmallBurst, 0, 0x64));

    temp_v0->x = arg0;
    temp_v0->y = arg1;
}

void drawEndingCharacterAura(EndingEffectActor *arg0) {
    drawMenuSprite(arg0->x + arg0->offsetX, arg0->y + arg0->offsetY, getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), ((u16) arg0->angle + 0x20) & 0xFFFF, 0x20, 0x20, arg0->palette, 0);
}

void updateEndingCharacterAura(EndingEffectActor *arg0) {
    MainMenuSceneModel *model;
    s32 divisor;

    model = getMainMenuSceneModel(arg0->characterId); divisor = 0x24000;
    if (0) {}
    arg0->x = model->displayObjects[7].screenX / divisor;
    arg0->y = -(model->displayObjects[7].screenY / divisor);
    if ((arg0->effectTimer += 1) == 4) {
        arg0->effectTimer = 0;
        if ((arg0->effectFrame += 1) == 5) {
            arg0->effectFrame = 0;
        }
    }
    if (gEndingCharacterEffectDoneFlags[arg0->characterId] == 0) {
        addRenderCallback(&gMenuRenderCallbackList, drawEndingCharacterAura, arg0);
        return;
    }
    removeCallbackTask(arg0);
}

void initEndingCharacterAura(EndingEffectActor *arg0) {
    arg0->angleVelocity = 0;
    arg0->angle = 0;
    setCallbackTaskCallback(arg0, updateEndingCharacterAura);
}

void spawnEndingCharacterAura(s16 arg0, s16 arg1, u8 arg2, u8 arg3) {
    EndingEffectActor *temp_v0 = (EndingEffectActor *)(gActiveMenuTask = (s32)createCallbackTask(initEndingCharacterAura, 0, 0x64));

    temp_v0->offsetX = arg0;
    temp_v0->offsetY = arg1;
    temp_v0->characterId = arg2;
    temp_v0->palette = arg3;
}

void drawEndingIdleSparkle(EndingEffectActor *arg0) {
    s32 temp;

    temp = getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle);
    drawMenuSprite(arg0->x, arg0->y, temp, (arg0->animFrame + 0x20) & 0xFFFF, 0x20, 0x20, 0, 0);
}

void updateEndingIdleSparkle(EndingEffectActor *arg0) {
    EndingEffectActor *temp_a2 = arg0;

    arg0->animTimer++;
    if (arg0->animTimer == 4) {
        arg0->animTimer = 0;
        arg0->animFrame++;
        if (arg0->animFrame == 5) {
            arg0->animFrame = 0;
        }
    }
    if (gEndingSequencePhase != 7) {
        addRenderCallback(&gMenuRenderCallbackList, drawEndingIdleSparkle, temp_a2);
    } else {
        removeCallbackTask(temp_a2);
    }
}

void initEndingIdleSparkle(EndingEffectActor *arg0) {
    arg0->x = -0x14;
    arg0->y = -0x59;
    arg0->animTimer = 0;
    arg0->animFrame = 0;
    setCallbackTaskCallback(arg0, updateEndingIdleSparkle);
}

void drawEndingPhaseAdvanceSparkle(EndingEffectActor *arg0) {
    s32 temp = getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle);
    drawMenuSprite(arg0->x, arg0->y, temp, arg0->animFrame, 0x20, 0x20, 0, 0);
}

void updateEndingPhaseAdvanceSparkle(EndingEffectActor *arg0) {
    EndingEffectActor *temp_a2 = arg0;

    if (arg0->animFrame < 5) {
        arg0->animTimer++;
        if (arg0->animTimer == 1) {
            arg0->animTimer = 0;
            arg0->animFrame++;
        }
    } else {
        arg0->animTimer++;
    }
    if (arg0->animTimer < 0xF) {
        addRenderCallback(&gMenuRenderCallbackList, drawEndingPhaseAdvanceSparkle, temp_a2);
        return;
    }
    removeCallbackTask(temp_a2);
    gEndingSequencePhase = 3;
}

void initEndingPhaseAdvanceSparkle(EndingEffectActor *arg0) {
    arg0->animFrame = 0;
    arg0->animTimer = 0;
    setCallbackTaskCallback(arg0, updateEndingPhaseAdvanceSparkle);
}

void spawnEndingPhaseAdvanceSparkle(s16 arg0, s16 arg1) {
    EndingEffectActor *temp = (EndingEffectActor *)(gActiveMenuTask = (s32)createCallbackTask(initEndingPhaseAdvanceSparkle, 0, 0x64));

    temp->x = arg0;
    temp->y = arg1;
}

void drawEndingDelayedSparkle(EndingEffectActor *arg0) {
    s32 temp;

    temp = getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle);
    drawMenuSprite(arg0->x, arg0->y, temp, (arg0->animFrame + 0x1C) & 0xFFFF, 0x20, 0x20, 0, 0);
}

// updateEndingDelayedSparkle best match: 97.653%
#pragma GLOBAL_ASM("asm/nonmatchings/ending_effects/updateEndingDelayedSparkle.s")

#ifdef NON_MATCHING
void updateEndingDelayedSparkle(EndingEffectActor *arg0) {
    s32 var_v1;
    EndingEffectActor *temp_a2 = arg0;

    var_v1 = 0xB4;
    if ((u16) arg0->delay == 0) {
        var_v1 = 0x12;
    }
    if (arg0->animFrame < 3) {
        arg0->animTimer++;
        if (arg0->animTimer == 2) {
            arg0->animTimer = 0;
            arg0->animFrame++;
        }
    } else {
        arg0->animTimer++;
    }
    if (arg0->animTimer < var_v1) {
        addRenderCallback(&gMenuRenderCallbackList, drawEndingDelayedSparkle, temp_a2);
        return;
    }
    if (gEndingSequencePhase == 3) {
        gEndingSequencePhase = 4;
    }
    removeCallbackTask(temp_a2);
}
#endif

void initEndingDelayedSparkle(EndingEffectActor *arg0) {
    arg0->animFrame = 0;
    arg0->animTimer = 0;
    setCallbackTaskCallback(arg0, updateEndingDelayedSparkle);
}

void spawnEndingDelayedSparkle(s16 arg0, s16 arg1, u8 arg2) {
    EndingEffectActor *temp_v0 = (EndingEffectActor *)(gActiveMenuTask = (s32)createCallbackTask(initEndingDelayedSparkle, 0, 0x64));

    temp_v0->x = arg0;
    temp_v0->y = arg1;
    temp_v0->delay = arg2;
}

void drawEndingTommySnowmanEntrance(EndingEffectActor *arg0) {
    s32 temp;

    temp = getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle);
    drawMenuSprite(arg0->x, arg0->y, temp, (gEndingSnowmanEntranceFrameOffsets[arg0->animFrame] + 0x19) & 0xFFFF, 0x20, 0x20, 0, 0);
}

void updateEndingTommySnowmanEntrance(EndingEffectActor *arg0) {
    EndingEffectActor *temp_a2 = arg0;

    arg0->animTimer++;
    if (arg0->animTimer == 7) {
        arg0->animTimer = 0;
        arg0->animFrame = (arg0->animFrame + 1) & 3;
    }
    if (temp_a2->x < 0x12) {
        temp_a2->x++;
    }
    if (gEndingSequencePhase == 4) {
        removeCallbackTask(temp_a2);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, drawEndingTommySnowmanEntrance, temp_a2);
    }
}

void initEndingTommySnowmanEntrance(EndingEffectActor *arg0) {
    arg0->x = -0xA7;
    arg0->y = -0x61;
    arg0->animTimer = 0;
    arg0->animFrame = 0;
    setCallbackTaskCallback(arg0, updateEndingTommySnowmanEntrance);
}

void drawRaceToMainMenuSnowboardIcon(EndingEffectActor *arg0) {
    drawMenuTilemapSprite(&arg0->angle, 1, arg0->x, arg0->y);
}

void updateRaceToMainMenuSnowboardIconIdle(EndingEffectActor *arg0) {
    addRenderCallback(&gMenuOverlayRenderCallbackList, drawRaceToMainMenuSnowboardIcon, arg0);
}

void updateRaceToMainMenuSnowboardIconFinalSpin(EndingEffectActor *arg0) {
    s16 *temp_v0;
    s16 temp_v1;
    EndingEffectActor *temp_a2 = arg0;

    temp_v0 = &arg0->angle;
    if (*temp_v0 > 0) {
        *temp_v0 -= gEndingTransitionRotationStep;
        gEndingTransitionSnowboardAngle = *temp_v0;
        temp_v1 = *temp_v0;
        if (temp_v1 <= 0) {
            *temp_v0 = 0x140 - temp_v1;
        }
    }
    addRenderCallback(&gMenuOverlayRenderCallbackList, drawRaceToMainMenuSnowboardIcon, temp_a2);
    if (gEndingSequencePhase == 0x3A) {
        setCallbackTaskCallback(temp_a2, updateRaceToMainMenuSnowboardIconIdle);
    }
}

void updateRaceToMainMenuSnowboardIconSecondSpin(EndingEffectActor *arg0) {
    s16 *temp_v0;
    EndingEffectActor *temp_a2 = arg0;

    temp_v0 = &arg0->angle;
    if (*temp_v0 > 0) {
        *temp_v0 -= gEndingTransitionRotationStep;
        gEndingTransitionSnowboardAngle = *temp_v0;
        if (*temp_v0 <= 0) {
            *temp_v0 = 0x140 - *temp_v0;
        }
    }
    addRenderCallback(&gMenuOverlayRenderCallbackList, drawRaceToMainMenuSnowboardIcon, temp_a2);
    if (gEndingSequencePhase == 0x39) {
        temp_v0 = &temp_a2->angle;
        setCallbackTaskCallback(temp_a2, updateRaceToMainMenuSnowboardIconFinalSpin);
        temp_a2->x = 0x28;
        temp_v0[1] = 0x10;
    }
}

void updateRaceToMainMenuSnowboardIconWaitSecondSpin(EndingEffectActor *arg0) {
    s16 *temp_v0;

    addRenderCallback(&gMenuOverlayRenderCallbackList, drawRaceToMainMenuSnowboardIcon, arg0);
    temp_v0 = &arg0->angle;
    if (gEndingSequencePhase == 0x33) {
        setCallbackTaskCallback(arg0, updateRaceToMainMenuSnowboardIconSecondSpin);
        gEndingTransitionRotationStep = 8;
        if (*temp_v0 == 0) {
            *temp_v0 = 0x140;
        }
    }
}

void updateRaceToMainMenuSnowboardIconSlowSpin(EndingEffectActor *arg0) {
    s16 *temp_v0;
    EndingEffectActor *temp_a2 = arg0;

    temp_v0 = &arg0->angle;
    if (*temp_v0 > 0) {
        *temp_v0 -= 8;
        gEndingTransitionSnowboardAngle = *temp_v0;
        if (*temp_v0 == 0) {
            *temp_v0 = 0x140;
        }
    }
    addRenderCallback(&gMenuOverlayRenderCallbackList, drawRaceToMainMenuSnowboardIcon, temp_a2);
    if (gEndingSequencePhase == 0x2A) {
        setCallbackTaskCallback(temp_a2, updateRaceToMainMenuSnowboardIconWaitSecondSpin);
    }
}

void updateRaceToMainMenuSnowboardIconResetSpin(EndingEffectActor *arg0) {
    s16 *sp1C;
    EndingEffectActor *temp_a2 = arg0;

    sp1C = &arg0->angle;
    if (*sp1C > 0) {
        *sp1C -= 8;
        gEndingTransitionSnowboardAngle = *sp1C;
    }
    addRenderCallback(&gMenuOverlayRenderCallbackList, drawRaceToMainMenuSnowboardIcon, temp_a2);
    if (*sp1C == 0) {
        temp_a2->x = 0x14;
        *sp1C = 0x140;
        setCallbackTaskCallback(temp_a2, updateRaceToMainMenuSnowboardIconSlowSpin);
    }
}

void updateRaceToMainMenuSnowboardIconExpandSpin(EndingEffectActor *arg0) {
    s16 *temp_v0;
    u16 *temp_v1;
    EndingEffectActor *temp_a2 = arg0;

    temp_v0 = &arg0->angle;
    if (*temp_v0 < 0x140) {
        *temp_v0 += gEndingTransitionRotationStep;
        gEndingTransitionSnowboardAngle = *temp_v0;
        temp_v1 = &gEndingTransitionRotationFrameCount;
        *temp_v1 = *temp_v1 + 1;
    }
    addRenderCallback(&gMenuOverlayRenderCallbackList, drawRaceToMainMenuSnowboardIcon, temp_a2);
    if (gEndingSequencePhase == 0x20) {
        setCallbackTaskCallback(temp_a2, updateRaceToMainMenuSnowboardIconResetSpin);
    }
}

void updateRaceToMainMenuSnowboardIconFastSpin(EndingEffectActor *arg0) {
    s16 *sp1C;
    EndingEffectActor *temp_a2 = arg0;

    sp1C = &arg0->angle;
    *sp1C += gEndingTransitionRotationStep;
    if (*sp1C >= 0x140) {
        *sp1C -= 0x140;
    }
    gEndingTransitionSnowboardAngle = *sp1C;
    addRenderCallback(&gMenuOverlayRenderCallbackList, drawRaceToMainMenuSnowboardIcon, temp_a2);
    if (gEndingSequencePhase == 0x13) {
        setCallbackTaskCallback(temp_a2, updateRaceToMainMenuSnowboardIconExpandSpin);
        temp_a2->x = 0x28;
        *sp1C = 0;
        gEndingTransitionSnowboardAngle = *sp1C;
    }
}

void updateRaceToMainMenuSnowboardIconWaitStart(EndingEffectActor *arg0) {
    s16 *temp_v0;

    temp_v0 = &arg0->angle;
    addRenderCallback(&gMenuOverlayRenderCallbackList, drawRaceToMainMenuSnowboardIcon, arg0);
    if (gEndingSequencePhase == 0x10) {
        setCallbackTaskCallback(arg0, updateRaceToMainMenuSnowboardIconFastSpin);
        arg0->x = 0x14;
        temp_v0[0] = 0;
        temp_v0[1] = 0x100;
        gEndingTransitionRotationStep = 0x14;
    }
}

void initRaceToMainMenuSnowboardIcon(EndingEffectActor *arg0) {
    initMenuTilemapSprite((MenuTilemapSprite *)arg0->spriteState, getRelocatableHeapBlockBase(gMenuIconTilemapAssetHandle));
    arg0->angleVelocity = 0x10;
    arg0->x = arg0->startX;
    arg0->y = arg0->startY;
    gEndingTransitionRotationFrameCount = 0;
    setCallbackTaskCallback(arg0, updateRaceToMainMenuSnowboardIconWaitStart);
}

void drawRaceToMainMenuSnowflakeIcon(EndingEffectActor *arg0) {
    drawMenuTilemapSprite(&arg0->angle, 1, arg0->x, arg0->y);
}

void updateRaceToMainMenuSnowflakeIconReverseSpin(EndingEffectActor *arg0) {
    arg0->angle--;
    if (arg0->angle < 0) {
        arg0->angle = 0x140;
    }
    addRenderCallback(&gMenuOverlayRenderCallbackList, drawRaceToMainMenuSnowflakeIcon, arg0);
    if ((gEndingSequencePhase == 0x2A) || (gEndingSequencePhase == 0x3A)) {
        setCallbackTaskCallback(arg0, updateRaceToMainMenuSnowflakeIconIdle);
    }
}

void updateRaceToMainMenuSnowflakeIconForwardSpin(EndingEffectActor *arg0) {
    arg0->angle++;
    if (arg0->angle == 0x140) {
        arg0->angle = 0;
    }
    addRenderCallback(&gMenuOverlayRenderCallbackList, drawRaceToMainMenuSnowflakeIcon, arg0);
    if (gEndingSequencePhase == 0x14) {
        setCallbackTaskCallback(arg0, updateRaceToMainMenuSnowflakeIconIdle);
    }
}

void updateRaceToMainMenuSnowflakeIconIdle(EndingEffectActor *arg0) {
    addRenderCallback(&gMenuOverlayRenderCallbackList, drawRaceToMainMenuSnowflakeIcon, arg0);
    if (gEndingSequencePhase == 0x10) {
        setCallbackTaskCallback(arg0, updateRaceToMainMenuSnowflakeIconForwardSpin);
    }
    if ((gEndingSequencePhase == 0x20) || (gEndingSequencePhase == 0x33)) {
        setCallbackTaskCallback(arg0, updateRaceToMainMenuSnowflakeIconReverseSpin);
    }
}

void initRaceToMainMenuSnowflakeIcon(EndingEffectActor *arg0) {
    initMenuTilemapSprite((MenuTilemapSprite *)arg0->spriteState, getRelocatableHeapBlockBase(gMenuIconTilemapAssetHandle));
    arg0->angle = 0;
    arg0->angleVelocity = 0x1F0;
    arg0->x = 0x14;
    arg0->y = arg0->startY;
    setCallbackTaskCallback(arg0, updateRaceToMainMenuSnowflakeIconIdle);
}

void drawRaceToMainMenuLogoWipe(EndingEffectActor *arg0) {
    drawMenuTilemapSprite(&arg0->angle, 0, arg0->x, arg0->y);
}

void updateRaceToMainMenuLogoWipeIdle(EndingEffectActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, drawRaceToMainMenuLogoWipe, arg0);
}

void updateRaceToMainMenuLogoWipeClose(EndingEffectActor *arg0) {
    s16 *temp_v0;

    arg0->animFrame = arg0->animFrame + 1;
    temp_v0 = &arg0->angle;
    arg0->animFrame &= 1;
    if (arg0->animFrame == 0) {
        temp_v0[1] -= 1;
        if (temp_v0[1] == 0x10) {
            setCallbackTaskCallback(arg0, updateRaceToMainMenuLogoWipeIdle);
            gEndingSequencePhase = 0x43;
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, drawRaceToMainMenuLogoWipe, arg0);
}

void updateRaceToMainMenuLogoWipeWaitClose(EndingEffectActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, drawRaceToMainMenuLogoWipe, arg0);
    if (gEndingSequencePhase == 0x42) {
        arg0->animFrame = 0;
        setCallbackTaskCallback(arg0, updateRaceToMainMenuLogoWipeClose);
    }
}

void updateRaceToMainMenuLogoWipeOpen(EndingEffectActor *arg0) {
    s16 *temp_v0;

    if (gMenuFadeAlpha == 0) {
        arg0->animFrame = (u16)arg0->animFrame + 1;
        temp_v0 = &arg0->angle;
        if ((u16)arg0->animFrame >= 0x78) {
            if (!(((u16)arg0->animFrame) & 1)) {
                temp_v0[1]++;
                if (temp_v0[1] == 0x60) {
                    arg0->animFrame = 0;
                    setCallbackTaskCallback(arg0, updateRaceToMainMenuLogoWipeWaitClose);
                }
            }
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, drawRaceToMainMenuLogoWipe, arg0);
}

void initRaceToMainMenuLogoWipe(EndingEffectActor *arg0) {
    initMenuTilemapSprite((MenuTilemapSprite *)arg0->spriteState, getRelocatableHeapBlockBase(gMenuPanelTilemapAssetHandle));
    arg0->angle = 0;
    arg0->angleVelocity = 0x10;
    arg0->x = arg0->startX;
    arg0->y = arg0->startY;
    setCallbackTaskCallback(arg0, updateRaceToMainMenuLogoWipeOpen);
}
