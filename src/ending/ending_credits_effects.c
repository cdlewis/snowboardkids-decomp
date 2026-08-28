#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/system_runtime.h"
#include "game/ending/ending_credits_effects.h"
#include "game/ending/ending_credits_flow.h"
#include "game/menu/main_menu/main_menu_scene_model.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/race_setup/race_setup_ui.h"

extern Gfx *gRegionAllocPtr;
extern s16 gMenuFadeAlpha;
extern u16 gEndingCreditsTransitionSnowboardIconExpandFrameCounter;
u16 gEndingCreditsNancyRunDustFrameOffsets[4] = { 0, 1, 2, 1 };
u16 gEndingCreditsTommySnowmanEntranceFrameOffsets[4] = { 0, 1, 2, 1 };

void updateEndingCreditsCharacterVanishPoof(EndingCreditsAttachedEffectActor *arg0);
void updateEndingCreditsNancySpeedLines(EndingCreditsNancySpeedLinesActor *arg0);
void drawEndingCreditsTommyBigBurst(EndingCreditsTommyBigBurstActor *arg0);
void updateEndingCreditsTommyBigBurst(EndingCreditsTommyBigBurstActor *arg0);
void updateEndingCreditsSlashRisingStar(EndingCreditsAnimatedEffectActor *arg0);
void updateEndingCreditsIdleSparkle(EndingCreditsAnimatedEffectActor *arg0);
void updateEndingCreditsTommySnowmanEntrance(EndingCreditsAnimatedEffectActor *arg0);
void updateEndingCreditsTransitionSnowboardIconSlowSpin(EndingCreditsTransitionSpriteActor *arg0);
void updateEndingCreditsTransitionSnowboardIconWaitSecondSpin(EndingCreditsTransitionSpriteActor *arg0);
void updateEndingCreditsTransitionSnowboardIconSecondSpin(EndingCreditsTransitionSpriteActor *arg0);
void drawEndingCreditsTransitionSnowboardIcon(EndingCreditsTransitionSpriteActor *arg0);
void updateEndingCreditsTransitionSnowboardIconFastSpin(EndingCreditsTransitionSpriteActor *arg0);
void updateEndingCreditsTransitionSnowboardIconWaitStart(EndingCreditsTransitionSpriteActor *arg0);
void updateEndingCreditsTransitionSnowflakeIconIdle(EndingCreditsTransitionSpriteActor *arg0);
void drawEndingCreditsTransitionLogoWipe(EndingCreditsTransitionSpriteActor *arg0);
void updateEndingCreditsTransitionLogoWipeOpen(EndingCreditsTransitionSpriteActor *arg0);
void updateEndingCreditsTransitionLogoWipeClose(EndingCreditsTransitionSpriteActor *arg0);
void updateEndingCreditsCharacterLoopingSparkle(EndingCreditsAttachedEffectActor *arg0);
void updateEndingCreditsNancyRunDust(EndingCreditsAnimatedEffectActor *arg0);
void updateEndingCreditsSmallBurst(EndingCreditsAnimatedEffectActor *arg0);
void updateEndingCreditsCharacterAura(EndingCreditsAttachedEffectActor *arg0);
void updateEndingCreditsPhaseAdvanceSparkle(EndingCreditsAnimatedEffectActor *arg0);
void updateEndingCreditsDelayedSparkle(EndingCreditsDelayedSparkleActor *arg0);

void drawEndingCreditsCharacterVanishPoof(EndingCreditsAttachedEffectActor *arg0) {
    void *texture;

    texture = getRelocatableHeapBlockBase(gAssetHandles[0x21]);
    drawMenuSprite(
        arg0->common.x + arg0->offsetX,
        arg0->common.y + arg0->offsetY,
        texture,
        arg0->frame + 0x32,
        0x20,
        0x20,
        arg0->palette,
        0
    );
}

void updateEndingCreditsCharacterVanishPoof(EndingCreditsAttachedEffectActor *arg0) {
    MainMenuSceneModel *model;
    s32 divisor;

    model = getMainMenuSceneModel(arg0->characterId);
    divisor = 0x24000;
    if (0) {}
    arg0->common.x = model->partTransforms[7].translation.x / divisor;
    arg0->common.y = -(model->partTransforms[7].translation.y / divisor);
    if ((arg0->frameTimer += 1) == 3) {
        arg0->frameTimer = 0;
        arg0->frame++;
    }
    if (arg0->frame == 3) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingCreditsCharacterVanishPoof, arg0);
}

void initEndingCreditsCharacterVanishPoof(EndingCreditsAttachedEffectActor *arg0) {
    arg0->frameTimer = 0;
    arg0->frame = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsCharacterVanishPoof);
}

void spawnEndingCreditsCharacterVanishPoof(s16 arg0, s16 arg1, u8 arg2, u8 arg3) {
    EndingCreditsAttachedEffectActor *temp_v0 =
        (EndingCreditsAttachedEffectActor *)(gActiveMenuTask = createCallbackTask(
                                         (CallbackTaskCallback)initEndingCreditsCharacterVanishPoof,
                                         0,
                                         0x63
                                     ));

    temp_v0->offsetX = arg0;
    temp_v0->offsetY = arg1;
    temp_v0->characterId = arg2;
    temp_v0->palette = arg3;
}

void drawEndingCreditsNancySpeedLine(EndingCreditsNancySpeedLinesActor *arg0) {
    void *temp = getRelocatableHeapBlockBase(gAssetHandles[0x21]);

    drawMenuSprite(
        arg0->xPositions[arg0->lineIndex],
        arg0->yPositions[arg0->lineIndex],
        temp,
        0xF,
        0x20,
        0x20,
        0,
        0
    );
}

void updateEndingCreditsNancySpeedLines(EndingCreditsNancySpeedLinesActor *arg0) {
    arg0->timer++;
    if ((arg0->timer % 30) == 0) {
        if (arg0->lineIndex < 2) {
            arg0->lineIndex++;
        }
    }
    if (arg0->timer == 0x69) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingCreditsNancySpeedLine, arg0);
}

void initEndingCreditsNancySpeedLines(EndingCreditsNancySpeedLinesActor *arg0) {
    arg0->xPositions[2] = -0x38;
    arg0->yPositions[2] = -0x5A;
    arg0->xPositions[0] = -0x30;
    arg0->yPositions[0] = -0x5D;
    arg0->xPositions[1] = -0x28;
    arg0->yPositions[1] = -0x56;
    arg0->timer = 0;
    arg0->lineIndex = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsNancySpeedLines);
}

void drawEndingCreditsCharacterLoopingSparkle(EndingCreditsAttachedEffectActor *arg0) {
    void *texture;

    texture = getRelocatableHeapBlockBase(gAssetHandles[0x21]);
    drawMenuSprite(
        arg0->common.x + arg0->offsetX,
        arg0->common.y + arg0->offsetY,
        texture,
        arg0->frame + 0x10,
        0x20,
        0x20,
        0,
        0
    );
}

void updateEndingCreditsCharacterLoopingSparkle(EndingCreditsAttachedEffectActor *arg0) {
    MainMenuSceneModel *model;
    s32 divisor;

    model = getMainMenuSceneModel(arg0->characterId);
    divisor = 0x24000;
    if (0) {}
    arg0->common.x = model->partTransforms[7].translation.x / divisor;
    arg0->common.y = -(model->partTransforms[7].translation.y / divisor);
    if ((arg0->frameTimer += 1) == 8) {
        arg0->frameTimer = 0;
        arg0->frame++;
        arg0->frame &= 3;
    }
    if (gEndingCreditsSequencePhase == 0x2F) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingCreditsCharacterLoopingSparkle, arg0);
}

void initEndingCreditsCharacterLoopingSparkle(EndingCreditsAttachedEffectActor *arg0) {
    arg0->frameTimer = 0;
    arg0->frame = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsCharacterLoopingSparkle);
}

void spawnEndingCreditsCharacterLoopingSparkle(s16 arg0, s16 arg1, u8 arg2) {
    EndingCreditsAttachedEffectActor *temp_v0 =
        (EndingCreditsAttachedEffectActor *)(gActiveMenuTask = createCallbackTask(
                                         (CallbackTaskCallback)initEndingCreditsCharacterLoopingSparkle,
                                         0,
                                         0x63
                                     ));

    temp_v0->offsetX = arg0;
    temp_v0->offsetY = arg1;
    temp_v0->characterId = arg2;
}

void drawEndingCreditsTommyBigBurst(EndingCreditsTommyBigBurstActor *arg0) {
    void *texture;

    gDPPipeSync(gRegionAllocPtr++);
    gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, 0xA0);

    if (arg0->frame != 0) {
        texture = getRelocatableHeapBlockBase(gAssetHandles[0x21]);
        drawMenuSprite(arg0->burstX, arg0->burstY, texture, arg0->frame + 0x24, 0x30, 0x20, 0, 0);
    }

    gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);

    if (arg0->frame < 0xC) {
        texture = getRelocatableHeapBlockBase(gAssetHandles[0x21]);
        drawMenuSprite(arg0->burstX, arg0->burstY, texture, arg0->frame + 0x25, 0x30, 0x20, 0, 0);
    }

    texture = getRelocatableHeapBlockBase(gAssetHandles[0x21]);
    drawMenuSprite(arg0->common.x, arg0->common.y, texture, 0x31, 0x20, 0x20, 0, 0);
}

void updateEndingCreditsTommyBigBurst(EndingCreditsTommyBigBurstActor *arg0) {
    arg0->frameTimer++;
    if (arg0->frameTimer == 3) {
        arg0->frameTimer = 0;
        arg0->frame++;
        switch (arg0->phase) {
            case 0:
                if (arg0->frame == 4) {
                    arg0->phase = arg0->phase + 1;
                }
                break;
            case 1:
                if (arg0->frame == 8) {
                    arg0->loopCount++;
                    if (arg0->loopCount == 3) {
                        arg0->phase++;
                    } else {
                        arg0->frame = 4;
                    }
                }
                break;
            case 2:
                break;
        }
    }

    if (arg0->frame == 0xD) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingCreditsTommyBigBurst, arg0);
}

void initEndingCreditsTommyBigBurst(EndingCreditsTommyBigBurstActor *arg0) {
    arg0->common.x = -0x3D;
    arg0->common.y = -0x63;
    arg0->burstX = -0x4E;
    arg0->burstY = -0x6C;
    arg0->frameTimer = 0;
    arg0->loopCount = 0;
    arg0->frame = 0;
    arg0->phase = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsTommyBigBurst);
}

void drawEndingCreditsSlashRisingStar(EndingCreditsAnimatedEffectActor *arg0) {
    void *temp;

    temp = getRelocatableHeapBlockBase(gAssetHandles[0x21]);
    drawMenuSprite(arg0->common.x, arg0->common.y, temp, arg0->frame + 9, 0x20, 0x20, 0, 0);
}

void updateEndingCreditsSlashRisingStar(EndingCreditsAnimatedEffectActor *arg0) {
    EndingCreditsAnimatedEffectActor *temp_a2 = arg0;

    arg0->common.x++;
    arg0->common.y--;
    arg0->frameTimer++;
    if (arg0->frameTimer == 3) {
        arg0->frameTimer = 0;
        arg0->frame++;
    }
    if (temp_a2->frame == 6) {
        removeCallbackTask(temp_a2);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingCreditsSlashRisingStar, temp_a2);
    }
}

void initEndingCreditsSlashRisingStar(EndingCreditsAnimatedEffectActor *arg0) {
    arg0->common.x = 0x3B;
    arg0->common.y = -0x38;
    arg0->frameTimer = 0;
    arg0->frame = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsSlashRisingStar);
}

void drawEndingCreditsNancyRunDust(EndingCreditsAnimatedEffectActor *arg0) {
    void *temp = getRelocatableHeapBlockBase(gAssetHandles[0x21]);
    drawMenuSprite(
        arg0->common.x - 0xE,
        arg0->common.y - 0x50,
        temp,
        (u16)(gEndingCreditsNancyRunDustFrameOffsets[arg0->frame] + 6),
        0x20,
        0x20,
        0,
        0
    );
}

void updateEndingCreditsNancyRunDust(EndingCreditsAnimatedEffectActor *arg0) {
    MainMenuSceneModel *model;
    s32 divisor;

    model = getMainMenuSceneModel(1);
    divisor = 0x24000;
    if (0) {}
    arg0->common.x = model->partTransforms[7].translation.x / divisor;
    arg0->common.y = -(model->partTransforms[7].translation.y / divisor);
    if ((arg0->frameTimer += 1) == 3) {
        arg0->frameTimer = 0;
        arg0->frame++;
        arg0->frame &= 3;
    }
    if (gEndingCreditsSequencePhase == 0x12) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingCreditsNancyRunDust, arg0);
}

void initEndingCreditsNancyRunDust(EndingCreditsAnimatedEffectActor *arg0) {
    arg0->frameTimer = 0;
    arg0->frame = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsNancyRunDust);
}

void drawEndingCreditsSmallBurst(EndingCreditsAnimatedEffectActor *arg0) {
    void *temp;

    temp = getRelocatableHeapBlockBase(gAssetHandles[0x21]);
    drawMenuSprite(arg0->common.x, arg0->common.y, temp, arg0->frame + 0x14, 0x20, 0x20, 0, 0);
}

void updateEndingCreditsSmallBurst(EndingCreditsAnimatedEffectActor *arg0) {
    EndingCreditsAnimatedEffectActor *temp_a2 = arg0;

    arg0->frameTimer++;
    if (arg0->frameTimer == 2) {
        arg0->frameTimer = 0;
        arg0->frame++;
    }
    if (temp_a2->frame == 5) {
        removeCallbackTask(temp_a2);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingCreditsSmallBurst, temp_a2);
    }
}

void initEndingCreditsSmallBurst(EndingCreditsAnimatedEffectActor *arg0) {
    arg0->frameTimer = 0;
    arg0->frame = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsSmallBurst);
}

void spawnEndingCreditsSmallBurst(s16 arg0, s16 arg1) {
    EndingCreditsAnimatedEffectActor *temp_v0 =
        (EndingCreditsAnimatedEffectActor
             *)(gActiveMenuTask = createCallbackTask((CallbackTaskCallback)initEndingCreditsSmallBurst, 0, 0x64));

    temp_v0->common.x = arg0;
    temp_v0->common.y = arg1;
}

void drawEndingCreditsCharacterAura(EndingCreditsAttachedEffectActor *arg0) {
    void *texture;

    texture = getRelocatableHeapBlockBase(gAssetHandles[0x21]);
    drawMenuSprite(
        arg0->common.x + arg0->offsetX,
        arg0->common.y + arg0->offsetY,
        texture,
        arg0->frame + 0x20,
        0x20,
        0x20,
        arg0->palette,
        0
    );
}

void updateEndingCreditsCharacterAura(EndingCreditsAttachedEffectActor *arg0) {
    MainMenuSceneModel *model;
    s32 divisor;

    model = getMainMenuSceneModel(arg0->characterId);
    divisor = 0x24000;
    if (0) {}
    arg0->common.x = model->partTransforms[7].translation.x / divisor;
    arg0->common.y = -(model->partTransforms[7].translation.y / divisor);
    if ((arg0->frameTimer += 1) == 4) {
        arg0->frameTimer = 0;
        if ((arg0->frame += 1) == 5) {
            arg0->frame = 0;
        }
    }
    if (gEndingCreditsCharacterAuraDoneFlags[arg0->characterId] == 0) {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingCreditsCharacterAura, arg0);
        return;
    }
    removeCallbackTask(arg0);
}

void initEndingCreditsCharacterAura(EndingCreditsAttachedEffectActor *arg0) {
    arg0->frameTimer = 0;
    arg0->frame = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsCharacterAura);
}

void spawnEndingCreditsCharacterAura(s16 arg0, s16 arg1, u8 arg2, u8 arg3) {
    EndingCreditsAttachedEffectActor *temp_v0 =
        (EndingCreditsAttachedEffectActor
             *)(gActiveMenuTask = createCallbackTask((CallbackTaskCallback)initEndingCreditsCharacterAura, 0, 0x64));

    temp_v0->offsetX = arg0;
    temp_v0->offsetY = arg1;
    temp_v0->characterId = arg2;
    temp_v0->palette = arg3;
}

void drawEndingCreditsIdleSparkle(EndingCreditsAnimatedEffectActor *arg0) {
    void *temp;

    temp = getRelocatableHeapBlockBase(gAssetHandles[0x21]);
    drawMenuSprite(arg0->common.x, arg0->common.y, temp, arg0->frame + 0x20, 0x20, 0x20, 0, 0);
}

void updateEndingCreditsIdleSparkle(EndingCreditsAnimatedEffectActor *arg0) {
    EndingCreditsAnimatedEffectActor *temp_a2 = arg0;

    arg0->frameTimer++;
    if (arg0->frameTimer == 4) {
        arg0->frameTimer = 0;
        arg0->frame++;
        if (arg0->frame == 5) {
            arg0->frame = 0;
        }
    }
    if (gEndingCreditsSequencePhase != 7) {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingCreditsIdleSparkle, temp_a2);
    } else {
        removeCallbackTask(temp_a2);
    }
}

void initEndingCreditsIdleSparkle(EndingCreditsAnimatedEffectActor *arg0) {
    arg0->common.x = -0x14;
    arg0->common.y = -0x59;
    arg0->frameTimer = 0;
    arg0->frame = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsIdleSparkle);
}

void drawEndingCreditsPhaseAdvanceSparkle(EndingCreditsAnimatedEffectActor *arg0) {
    void *temp = getRelocatableHeapBlockBase(gAssetHandles[0x21]);
    drawMenuSprite(arg0->common.x, arg0->common.y, temp, arg0->frame, 0x20, 0x20, 0, 0);
}

void updateEndingCreditsPhaseAdvanceSparkle(EndingCreditsAnimatedEffectActor *arg0) {
    EndingCreditsAnimatedEffectActor *temp_a2 = arg0;

    if (arg0->frame < 5) {
        arg0->frameTimer++;
        if (arg0->frameTimer == 1) {
            arg0->frameTimer = 0;
            arg0->frame++;
        }
    } else {
        arg0->frameTimer++;
    }
    if (arg0->frameTimer < 0xF) {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingCreditsPhaseAdvanceSparkle, temp_a2);
        return;
    }
    removeCallbackTask(temp_a2);
    gEndingCreditsSequencePhase = 3;
}

void initEndingCreditsPhaseAdvanceSparkle(EndingCreditsAnimatedEffectActor *arg0) {
    arg0->frame = 0;
    arg0->frameTimer = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsPhaseAdvanceSparkle);
}

void spawnEndingCreditsPhaseAdvanceSparkle(s16 arg0, s16 arg1) {
    EndingCreditsAnimatedEffectActor *temp =
        (EndingCreditsAnimatedEffectActor *)(gActiveMenuTask = createCallbackTask(
                                         (CallbackTaskCallback)initEndingCreditsPhaseAdvanceSparkle,
                                         0,
                                         0x64
                                     ));

    temp->common.x = arg0;
    temp->common.y = arg1;
}

void drawEndingCreditsDelayedSparkle(EndingCreditsDelayedSparkleActor *arg0) {
    void *temp;

    temp = getRelocatableHeapBlockBase(gAssetHandles[0x21]);
    drawMenuSprite(arg0->x, arg0->y, temp, arg0->frame + 0x1C, 0x20, 0x20, 0, 0);
}

void updateEndingCreditsDelayedSparkle(EndingCreditsDelayedSparkleActor *arg0) {
    s32 var_v1;
    EndingCreditsDelayedSparkleActor *temp_a2 = arg0;

    if (arg0->durationMode == 0) {
        var_v1 = 0x12;
    } else {
        var_v1 = 0xB4;
    }
    if (arg0->frame < 3) {
        arg0->frameTimer++;
        if (arg0->frameTimer == 2) {
            arg0->frameTimer = 0;
            arg0->frame++;
        }
    } else {
        arg0->frameTimer++;
    }
    if (arg0->frameTimer < var_v1) {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingCreditsDelayedSparkle, temp_a2);
        return;
    }
    if (gEndingCreditsSequencePhase == 3) {
        gEndingCreditsSequencePhase = 4;
    }
    removeCallbackTask(temp_a2);
}

void initEndingCreditsDelayedSparkle(EndingCreditsDelayedSparkleActor *arg0) {
    arg0->frame = 0;
    arg0->frameTimer = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsDelayedSparkle);
}

void spawnEndingCreditsDelayedSparkle(s16 arg0, s16 arg1, u8 arg2) {
    EndingCreditsDelayedSparkleActor *temp_v0 =
        (EndingCreditsDelayedSparkleActor
             *)(gActiveMenuTask = createCallbackTask((CallbackTaskCallback)initEndingCreditsDelayedSparkle, 0, 0x64));

    temp_v0->x = arg0;
    temp_v0->y = arg1;
    temp_v0->durationMode = arg2;
}

void drawEndingCreditsTommySnowmanEntrance(EndingCreditsAnimatedEffectActor *arg0) {
    void *temp;

    temp = getRelocatableHeapBlockBase(gAssetHandles[0x21]);
    drawMenuSprite(
        arg0->common.x,
        arg0->common.y,
        temp,
        gEndingCreditsTommySnowmanEntranceFrameOffsets[arg0->frame] + 0x19,
        0x20,
        0x20,
        0,
        0
    );
}

void updateEndingCreditsTommySnowmanEntrance(EndingCreditsAnimatedEffectActor *arg0) {
    EndingCreditsAnimatedEffectActor *temp_a2 = arg0;

    arg0->frameTimer++;
    if (arg0->frameTimer == 7) {
        arg0->frameTimer = 0;
        arg0->frame = (arg0->frame + 1) & 3;
    }
    if (temp_a2->common.x < 0x12) {
        temp_a2->common.x++;
    }
    if (gEndingCreditsSequencePhase == 4) {
        removeCallbackTask(temp_a2);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingCreditsTommySnowmanEntrance, temp_a2);
    }
}

void initEndingCreditsTommySnowmanEntrance(EndingCreditsAnimatedEffectActor *arg0) {
    arg0->common.x = -0xA7;
    arg0->common.y = -0x61;
    arg0->frameTimer = 0;
    arg0->frame = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsTommySnowmanEntrance);
}

void drawEndingCreditsTransitionSnowboardIcon(EndingCreditsTransitionSpriteActor *arg0) {
    drawMenuTilemapSprite(&arg0->sprite.render, MENU_TILEMAP_TEXEL_8B, arg0->common.x, arg0->common.y);
}

void updateEndingCreditsTransitionSnowboardIconIdle(EndingCreditsTransitionSpriteActor *arg0) {
    addRenderCallback(&gMenuOverlayRenderCallbackList, (RenderCallback)drawEndingCreditsTransitionSnowboardIcon, arg0);
}

void updateEndingCreditsTransitionSnowboardIconFinalSpin(EndingCreditsTransitionSpriteActor *arg0) {
    s16 *temp_v0;
    s16 temp_v1;
    EndingCreditsTransitionSpriteActor *temp_a2 = arg0;

    temp_v0 = &arg0->sprite.render.scrollX;
    if (*temp_v0 > 0) {
        *temp_v0 -= gEndingCreditsTransitionSnowboardIconSpinStep;
        gEndingCreditsTransitionSnowboardIconAngle = *temp_v0;
        temp_v1 = *temp_v0;
        if (temp_v1 <= 0) {
            *temp_v0 = 0x140 - temp_v1;
        }
    }
    addRenderCallback(
        &gMenuOverlayRenderCallbackList,
        (RenderCallback)drawEndingCreditsTransitionSnowboardIcon,
        temp_a2
    );
    if (gEndingCreditsSequencePhase == 0x3A) {
        setCallbackTaskCallback(temp_a2, (CallbackTaskCallback)updateEndingCreditsTransitionSnowboardIconIdle);
    }
}

void updateEndingCreditsTransitionSnowboardIconSecondSpin(EndingCreditsTransitionSpriteActor *arg0) {
    MenuRenderSprite *render;
    EndingCreditsTransitionSpriteActor *temp_a2 = arg0;

    render = &arg0->sprite.render;
    if (render->scrollX > 0) {
        render->scrollX -= gEndingCreditsTransitionSnowboardIconSpinStep;
        gEndingCreditsTransitionSnowboardIconAngle = render->scrollX;
        if (render->scrollX <= 0) {
            render->scrollX = 0x140 - render->scrollX;
        }
    }
    addRenderCallback(
        &gMenuOverlayRenderCallbackList,
        (RenderCallback)drawEndingCreditsTransitionSnowboardIcon,
        temp_a2
    );
    if (gEndingCreditsSequencePhase == 0x39) {
        render = &temp_a2->sprite.render;
        setCallbackTaskCallback(temp_a2, (CallbackTaskCallback)updateEndingCreditsTransitionSnowboardIconFinalSpin);
        temp_a2->common.x = 0x28;
        render->scrollY = 0x10;
    }
}

void updateEndingCreditsTransitionSnowboardIconWaitSecondSpin(EndingCreditsTransitionSpriteActor *arg0) {
    s16 *temp_v0;

    addRenderCallback(&gMenuOverlayRenderCallbackList, (RenderCallback)drawEndingCreditsTransitionSnowboardIcon, arg0);
    temp_v0 = &arg0->sprite.render.scrollX;
    if (gEndingCreditsSequencePhase == 0x33) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsTransitionSnowboardIconSecondSpin);
        gEndingCreditsTransitionSnowboardIconSpinStep = 8;
        if (*temp_v0 == 0) {
            *temp_v0 = 0x140;
        }
    }
}

void updateEndingCreditsTransitionSnowboardIconSlowSpin(EndingCreditsTransitionSpriteActor *arg0) {
    s16 *temp_v0;
    EndingCreditsTransitionSpriteActor *temp_a2 = arg0;

    temp_v0 = &arg0->sprite.render.scrollX;
    if (*temp_v0 > 0) {
        *temp_v0 -= 8;
        gEndingCreditsTransitionSnowboardIconAngle = *temp_v0;
        if (*temp_v0 == 0) {
            *temp_v0 = 0x140;
        }
    }
    addRenderCallback(
        &gMenuOverlayRenderCallbackList,
        (RenderCallback)drawEndingCreditsTransitionSnowboardIcon,
        temp_a2
    );
    if (gEndingCreditsSequencePhase == 0x2A) {
        setCallbackTaskCallback(
            temp_a2,
            (CallbackTaskCallback)updateEndingCreditsTransitionSnowboardIconWaitSecondSpin
        );
    }
}

void updateEndingCreditsTransitionSnowboardIconResetSpin(EndingCreditsTransitionSpriteActor *arg0) {
    s16 *sp1C;
    EndingCreditsTransitionSpriteActor *temp_a2 = arg0;

    sp1C = &arg0->sprite.render.scrollX;
    if (*sp1C > 0) {
        *sp1C -= 8;
        gEndingCreditsTransitionSnowboardIconAngle = *sp1C;
    }
    addRenderCallback(
        &gMenuOverlayRenderCallbackList,
        (RenderCallback)drawEndingCreditsTransitionSnowboardIcon,
        temp_a2
    );
    if (*sp1C == 0) {
        temp_a2->common.x = 0x14;
        *sp1C = 0x140;
        setCallbackTaskCallback(temp_a2, (CallbackTaskCallback)updateEndingCreditsTransitionSnowboardIconSlowSpin);
    }
}

void updateEndingCreditsTransitionSnowboardIconExpandSpin(EndingCreditsTransitionSpriteActor *arg0) {
    s16 *temp_v0;
    u16 *temp_v1;
    EndingCreditsTransitionSpriteActor *temp_a2 = arg0;

    temp_v0 = &arg0->sprite.render.scrollX;
    if (*temp_v0 < 0x140) {
        *temp_v0 += gEndingCreditsTransitionSnowboardIconSpinStep;
        gEndingCreditsTransitionSnowboardIconAngle = *temp_v0;
        temp_v1 = &gEndingCreditsTransitionSnowboardIconExpandFrameCounter;
        *temp_v1 = *temp_v1 + 1;
    }
    addRenderCallback(
        &gMenuOverlayRenderCallbackList,
        (RenderCallback)drawEndingCreditsTransitionSnowboardIcon,
        temp_a2
    );
    if (gEndingCreditsSequencePhase == 0x20) {
        setCallbackTaskCallback(temp_a2, (CallbackTaskCallback)updateEndingCreditsTransitionSnowboardIconResetSpin);
    }
}

void updateEndingCreditsTransitionSnowboardIconFastSpin(EndingCreditsTransitionSpriteActor *arg0) {
    s16 *sp1C;
    EndingCreditsTransitionSpriteActor *temp_a2 = arg0;

    sp1C = &arg0->sprite.render.scrollX;
    *sp1C += gEndingCreditsTransitionSnowboardIconSpinStep;
    if (*sp1C >= 0x140) {
        *sp1C -= 0x140;
    }
    gEndingCreditsTransitionSnowboardIconAngle = *sp1C;
    addRenderCallback(
        &gMenuOverlayRenderCallbackList,
        (RenderCallback)drawEndingCreditsTransitionSnowboardIcon,
        temp_a2
    );
    if (gEndingCreditsSequencePhase == 0x13) {
        setCallbackTaskCallback(temp_a2, (CallbackTaskCallback)updateEndingCreditsTransitionSnowboardIconExpandSpin);
        temp_a2->common.x = 0x28;
        *sp1C = 0;
        gEndingCreditsTransitionSnowboardIconAngle = *sp1C;
    }
}

void updateEndingCreditsTransitionSnowboardIconWaitStart(EndingCreditsTransitionSpriteActor *arg0) {
    MenuRenderSprite *render;

    render = &arg0->sprite.render;
    addRenderCallback(&gMenuOverlayRenderCallbackList, (RenderCallback)drawEndingCreditsTransitionSnowboardIcon, arg0);
    if (gEndingCreditsSequencePhase == 0x10) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsTransitionSnowboardIconFastSpin);
        arg0->common.x = 0x14;
        render->scrollX = 0;
        render->scrollY = 0x100;
        gEndingCreditsTransitionSnowboardIconSpinStep = 0x14;
    }
}

void initEndingCreditsTransitionSnowboardIcon(EndingCreditsTransitionSpriteActor *arg0) {
    initMenuTilemapSprite(&arg0->sprite, getRelocatableHeapBlockBase(gAssetHandles[0x26]));
    arg0->sprite.render.scrollY = 0x10;
    arg0->common.x = arg0->sprite.render.tilemapWidth;
    arg0->common.y = arg0->sprite.render.tilemapHeight;
    gEndingCreditsTransitionSnowboardIconExpandFrameCounter = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsTransitionSnowboardIconWaitStart);
}

void drawEndingCreditsTransitionSnowflakeIcon(EndingCreditsTransitionSpriteActor *arg0) {
    drawMenuTilemapSprite(&arg0->sprite.render, MENU_TILEMAP_TEXEL_8B, arg0->common.x, arg0->common.y);
}

void updateEndingCreditsTransitionSnowflakeIconReverseSpin(EndingCreditsTransitionSpriteActor *arg0) {
    arg0->sprite.render.scrollX--;
    if (arg0->sprite.render.scrollX < 0) {
        arg0->sprite.render.scrollX = 0x140;
    }
    addRenderCallback(&gMenuOverlayRenderCallbackList, (RenderCallback)drawEndingCreditsTransitionSnowflakeIcon, arg0);
    if ((gEndingCreditsSequencePhase == 0x2A) || (gEndingCreditsSequencePhase == 0x3A)) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsTransitionSnowflakeIconIdle);
    }
}

void updateEndingCreditsTransitionSnowflakeIconForwardSpin(EndingCreditsTransitionSpriteActor *arg0) {
    arg0->sprite.render.scrollX++;
    if (arg0->sprite.render.scrollX == 0x140) {
        arg0->sprite.render.scrollX = 0;
    }
    addRenderCallback(&gMenuOverlayRenderCallbackList, (RenderCallback)drawEndingCreditsTransitionSnowflakeIcon, arg0);
    if (gEndingCreditsSequencePhase == 0x14) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsTransitionSnowflakeIconIdle);
    }
}

void updateEndingCreditsTransitionSnowflakeIconIdle(EndingCreditsTransitionSpriteActor *arg0) {
    addRenderCallback(&gMenuOverlayRenderCallbackList, (RenderCallback)drawEndingCreditsTransitionSnowflakeIcon, arg0);
    if (gEndingCreditsSequencePhase == 0x10) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsTransitionSnowflakeIconForwardSpin);
    }
    if ((gEndingCreditsSequencePhase == 0x20) || (gEndingCreditsSequencePhase == 0x33)) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsTransitionSnowflakeIconReverseSpin);
    }
}

void initEndingCreditsTransitionSnowflakeIcon(EndingCreditsTransitionSpriteActor *arg0) {
    initMenuTilemapSprite(&arg0->sprite, getRelocatableHeapBlockBase(gAssetHandles[0x26]));
    arg0->sprite.render.scrollX = 0;
    arg0->sprite.render.scrollY = 0x1F0;
    arg0->common.x = 0x14;
    arg0->common.y = arg0->sprite.render.tilemapHeight;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsTransitionSnowflakeIconIdle);
}

void drawEndingCreditsTransitionLogoWipe(EndingCreditsTransitionSpriteActor *arg0) {
    drawMenuTilemapSprite(&arg0->sprite.render, MENU_TILEMAP_TEXEL_4B, arg0->common.x, arg0->common.y);
}

void updateEndingCreditsTransitionLogoWipeIdle(EndingCreditsTransitionSpriteActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingCreditsTransitionLogoWipe, arg0);
}

void updateEndingCreditsTransitionLogoWipeClose(EndingCreditsTransitionSpriteActor *arg0) {
    MenuRenderSprite *render;

    arg0->wipeTimer = arg0->wipeTimer + 1;
    render = &arg0->sprite.render;
    arg0->wipeTimer &= 1;
    if (arg0->wipeTimer == 0) {
        render->scrollY -= 1;
        if (render->scrollY == 0x10) {
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsTransitionLogoWipeIdle);
            gEndingCreditsSequencePhase = 0x43;
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingCreditsTransitionLogoWipe, arg0);
}

void updateEndingCreditsTransitionLogoWipeWaitClose(EndingCreditsTransitionSpriteActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingCreditsTransitionLogoWipe, arg0);
    if (gEndingCreditsSequencePhase == 0x42) {
        arg0->wipeTimer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsTransitionLogoWipeClose);
    }
}

void updateEndingCreditsTransitionLogoWipeOpen(EndingCreditsTransitionSpriteActor *arg0) {
    MenuRenderSprite *render;

    if (gMenuFadeAlpha == 0) {
        arg0->wipeTimer++;
        render = &arg0->sprite.render;
        if (arg0->wipeTimer >= 0x78) {
            if (!(arg0->wipeTimer & 1)) {
                render->scrollY++;
                if (render->scrollY == 0x60) {
                    arg0->wipeTimer = 0;
                    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsTransitionLogoWipeWaitClose);
                }
            }
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingCreditsTransitionLogoWipe, arg0);
}

void initEndingCreditsTransitionLogoWipe(EndingCreditsTransitionSpriteActor *arg0) {
    initMenuTilemapSprite(&arg0->sprite, getRelocatableHeapBlockBase(gAssetHandles[0x27]));
    arg0->sprite.render.scrollX = 0;
    arg0->sprite.render.scrollY = 0x10;
    arg0->common.x = arg0->sprite.render.tilemapWidth;
    arg0->common.y = arg0->sprite.render.tilemapHeight;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsTransitionLogoWipeOpen);
}
