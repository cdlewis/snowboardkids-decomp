#include "common.h"
#include "game/race/ui/race_ui_effects.h"
#include "game/engine/game_task_scheduler.h"
#include "game/race/race_state.h"
#include "game/audio/sound_manager.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/engine/system_runtime.h"
#include "game/engine/viewport_manager.h"
#include "game/math/fixed_point_math.h"
#include "game/menu/renderer/menu_screen_effects.h"
#include "game/menu/main_menu/main_menu_scene_model.h"
#include "game/menu/main_menu/main_menu_scene_model_renderer.h"
#include "game/race/player/race_player_input.h"
#include "game/engine/model_asset_symbols.h"

#define MAIN_MENU_GFX_CMD(pkt, cmd0, cmd1) \
    {                                      \
        Gfx *_g = (Gfx *)(pkt);            \
        _g->words.w0 = (cmd0);             \
        _g->words.w1 = (cmd1);             \
    }

typedef struct {
    OverlayActorWord x;
    OverlayActorWord y;
    OverlayActorWord z;
} OverlayActorTransform;

typedef struct {
    Transform3D source;
    s32 pad20;
} TitleMenuRotatingBoardScratch;

void drawPassAwardLabel(MenuScreenEffectActor *);
void drawCourseRecordBanner(MenuScreenEffectActor *);

void drawRaceStartPlayerEffectSprite(MenuScreenEffectActor *);

void drawFallingConfetti(MenuScreenEffectActor *arg0) {
    void *texture;

    if (gCurrentViewportIndex == arg0->index) {
        texture = getRelocatableHeapBlockBase(gAssetHandles[42]);
        drawMenuSprite(
            (s16)(arg0->unk18.half.hi >> 4),
            (s16)(arg0->unk18.half.lo >> 4),
            texture,
            arg0->unk1C.half.hi + 2,
            arg0->timer,
            arg0->timer,
            0,
            arg0->unk2A + 2
        );
    }
}

void updateFallingConfettiDrift(MenuScreenEffectActor *arg0) {
    s16 temp_v0;
    s16 temp_v0_2;
    u32 var_v1;

    arg0->unk1C.half.hi = (arg0->unk1C.half.hi + 1) & 3;
    if (arg0->unk24.half.hi >= 0) {
        temp_v0 = arg0->unk24.half.lo;
        if (temp_v0 != 0) {
            arg0->unk20.half.hi += 4;
            arg0->unk24.half.lo = temp_v0 - 1;
        } else {
            arg0->unk20.half.hi -= 4;
            if (arg0->unk20.half.hi == 0) {
                arg0->unk24.half.lo = (randomNextMain() & 0x1F) + 0x10;
                arg0->unk24.half.hi = -1;
            }
        }
    } else {
        temp_v0_2 = arg0->unk24.half.lo;
        if (temp_v0_2 != 0) {
            arg0->unk20.half.hi -= 4;
            arg0->unk24.half.lo = temp_v0_2 - 1;
        } else {
            arg0->unk20.half.hi += 4;
            if (arg0->unk20.half.hi == 0) {
                arg0->unk24.half.lo = (randomNextMain() & 0x1F) + 0x10;
                arg0->unk24.half.hi = 1;
            }
        }
    }

    var_v1 = arg0->unk20.half.hi;
    arg0->unk18.half.hi += var_v1;
    arg0->unk18.half.lo += arg0->unk20.half.lo;
    if (arg0->unk18.half.lo >= 0x6E1) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawFallingConfetti, (void *)arg0);
}

void updateFallingConfettiSway(MenuScreenEffectActor *arg0) {
    arg0->unk1C.half.hi = (arg0->unk1C.half.hi + 1) & 3;
    arg0->unk18.half.lo += arg0->unk20.half.lo;
    arg0->unk24.half.hi = (arg0->unk24.half.hi + 0x20) & 0xFFF;
    arg0->unk18.half.hi += (fixedSine(arg0->unk24.half.hi) * (arg0->unk1C.half.lo + 0x30)) / 4096;
    if ((arg0->unk24.half.hi == 0) || (arg0->unk24.half.hi == 0x800)) {
        arg0->unk1C.half.lo = randomNextMain() & 0xF;
    }
    if (arg0->unk18.half.lo >= 0x6E1) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawFallingConfetti, (void *)arg0);
}

void initFallingConfetti(MenuScreenEffectActor *arg0) {
    s32 temp_v1;
    s16 rand;

    temp_v1 = randomNextMain() & 1;
    switch (temp_v1) {
        case 0:
            arg0->unk18.half.hi = (randomNextMain() - 0x80) << 4;
            arg0->unk18.half.lo = -0x780;
            arg0->unk1C.half.lo = randomNextMain() & 0xF;
            rand = randomNextMain();
            arg0->unk24.half.hi = rand << 4;
            if (randomNextMain() & 1) {
                arg0->timer = 0x20;
                arg0->unk20.half.lo = (randomNextMain() & 7) + 0x40;
            } else {
                arg0->timer = 0x1B;
                arg0->unk20.half.lo = (randomNextMain() & 7) + 0x20;
            }
            arg0->unk2A = randomNextMain() % 5;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateFallingConfettiSway);
            return;
        case 1:
            arg0->unk18.half.hi = (randomNextMain() - 0x80) << 4;
            arg0->unk18.half.lo = -0x780;
            arg0->unk1C.half.lo = (randomNextMain() & 0x1F) - 0xF;
            rand = randomNextMain();
            arg0->unk24.half.hi = rand << 4;
            if (randomNextMain() & 1) {
                arg0->timer = 0x20;
                arg0->unk20.half.lo = (randomNextMain() & 7) + 0x3C;
            } else {
                arg0->timer = 0x1B;
                arg0->unk20.half.lo = (randomNextMain() & 7) + 0x22;
            }
            arg0->unk2A = randomNextMain() % 5;
            arg0->unk24.half.hi = (randomNextMain() & 1) - 1;
            if (arg0->unk24.half.hi > 0) {
                arg0->unk20.half.hi = (randomNextMain() * 8) & 0xF;
            } else {
                arg0->unk20.half.hi = (randomNextMain() * -8) & 0xF;
            }
            arg0->unk24.half.lo = randomNextMain() & 0xF;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateFallingConfettiDrift);
            return;
    }
}

void drawPassAwardPodiumModels(MenuScreenEffectActor *arg0) {
    register MenuScreenEffectActor *actor = arg0;

    if (gCurrentViewportIndex == 0) {
        gDPPipeSync(gRegionAllocPtr++);

        gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[8]));
        gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[9]));

        gSPMatrix(gRegionAllocPtr++, &gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        if (actor->index == 0) {
            gSPDisplayList(gRegionAllocPtr++, gPassAwardPodiumVariant0Dl);
        }
        if (actor->index == 1) {
            gSPDisplayList(gRegionAllocPtr++, gPassAwardPodiumVariant1Dl);
        }
        if (actor->index == 2) {
            gSPDisplayList(gRegionAllocPtr++, gPassAwardPodiumVariant2Dl);
        }
    }
}

void drawPassAwardCourseBackdrop(void *arg0) {
    if (gCurrentViewportIndex == 0) {
        gDPPipeSync(gRegionAllocPtr++);

        gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[14]));
        gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[18]));

        gSPMatrix(gRegionAllocPtr++, &gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        gSPDisplayList(gRegionAllocPtr++, gRookieMountainBackdropDl);
    }
}

void renderPassAwardPodium(MenuScreenEffectActor *arg0) {
    addRenderCallback(&gBackdropRenderCallbackList, (RenderCallback)drawPassAwardCourseBackdrop, arg0);
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)drawPassAwardPodiumModels, arg0);
}

void initPassAwardPodium(MenuScreenEffectActor *arg0) {
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)renderPassAwardPodium);
}

void stopPassAwardWinner(MenuScreenEffectActor *arg0) {
    loopMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallbackForViewport0(0);
}

void updatePassAwardWinner(MenuScreenEffectActor *arg0) {
    if (gMenuFadeAlpha == 0) {
        if (arg0->unk18.byte.b0 != 0) {
            enqueueSoundEffect(raceSetupCharacterFocusSoundIds[arg0->index], SFX_PRIORITY_NORMAL);
            arg0->unk18.byte.b0 = 0;
        }
        loopMainMenuSceneModelAnimation(0);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)stopPassAwardWinner);
        addMainMenuSceneModelDrawCallbackForViewport0(0);
        return;
    }
    addMainMenuSceneModelDrawCallback(0);
}

void initPassAwardWinner(MenuScreenEffectActor *arg0) {
    arg0->unk18.byte.b0 = 1;
    initMainMenuSceneModel(0, arg0->index);
    setMainMenuSceneModelAnimation(0, raceSetupCharacterFocusAnimationIds[arg0->index]);
    loopMainMenuSceneModelAnimation(0);
    setMainMenuSceneModelPosition(0, 0, 0, 0);
    setMainMenuSceneModelRotation(0, 0, 0, 0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updatePassAwardWinner);
}

void updatePassAwardOpponent(MenuScreenEffectActor *arg0) {
    s16 temp = arg0->unk1C.half.hi;

    if (temp == 0) {
        loopMainMenuSceneModelAnimation(arg0->unk18.word);
    } else {
        arg0->unk1C.half.hi = temp - 1;
    }
    addMainMenuSceneModelDrawCallbackForViewport(arg0->unk18.word, 1);
}

void initPassAwardOpponent(MenuScreenEffectActor *arg0) {
    s32 a = arg0->unk18.word;
    arg0->unk1C.half.hi = a * 0xF;
    initMainMenuSceneModel(a, arg0->index);
    setMainMenuSceneModelAnimation(arg0->unk18.word, passAwardOpponentAnimationIds[arg0->index]);
    loopMainMenuSceneModelAnimation(arg0->unk18.word);
    {
        s32 b = arg0->unk18.word;
        setMainMenuSceneModelPosition(b, passAwardOpponentXPositions[b], 0xFFB50000, 0xA00000);
    }
    setMainMenuSceneModelRotation(arg0->unk18.word, 0, 0x800, 0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updatePassAwardOpponent);
}

void createPassAwardOpponent(s32 arg0, s32 arg1) {
    MenuScreenEffectActor *p = createCallbackTaskWithUserId((CallbackTaskCallback)initPassAwardOpponent, 0, 0x64, arg1);
    if (p != NULL) {
        p->unk18.word = arg0;
    }
}

void drawTitleScreenStartPrompt(MenuScreenEffectActor *arg0) {
    drawMenuAsciiTextDefaultScale(-0x44, 0x38, D_800E0520, 0);
}

void updateTitleScreenStartPrompt(void *arg0) {
    if (gFrameCounter & 8) {
        addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawTitleScreenStartPrompt, 0);
    }
}

void drawRaceStartPlayerEffectSprite(MenuScreenEffectActor *arg0) {
    void *spA4;
    void *spA0;
    volatile u8 gap[0xC];
    Transform3D sp74;

    if (gRenderMatricesDirty != 0) {
        arg0->unk2F = 1;
    }

    if (arg0->unk2F != 0) {
        arg0->unk2F = 0;
        sp74 = gIdentityFixedTransform;
        sp74.translation.x = arg0->unk18.word;
        sp74.translation.y = arg0->unk1C.word;
        sp74.translation.z = arg0->unk20.word;
        arg0->unk24.word = (s32)allocFixedTransformMatrix(&sp74);
    }

    if (arg0->unk24.word != 0) {
        gSPDisplayList(gRegionAllocPtr++, gAlphaSpriteRenderModeDl);
        getAssetTableImageAndPalette(
            getRelocatableHeapBlockBase(gAssetHandles[30]),
            (u16)(arg0->unk2E + 0x24),
            &spA4,
            &spA0
        );
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xFD500000, (u32)spA4);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF5500000, 0x07080200);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF3000000, 0x070FF400);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF5400400, 0x80200);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF2000000, 0x7C07C);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)spA0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0x01020040, arg0->unk24.word);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0x01000040, (u32)gViewportMatrix);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)raceStartPlayerEffectVertices);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
    }
}

void updateRaceStartPlayerEffectActive(MenuScreenEffectActor *arg0) {
    s16 sp38[0x10];
    Vec3i sp2C;
    RacePlayer *player;
    u8 temp;

    player = &gRacePlayers[arg0->index];
    if (player->stateFlags & 0x2000) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitForRaceStartPlayerEffect);
        return;
    }

    temp = arg0->unk2E;
    if (temp != 5) {
        arg0->unk2E = temp + 1;
    } else if (player->isTargetedByAttack == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitForRaceStartPlayerEffect);
    }

    sp2C.x = 0x40000;
    sp2C.y = 0x180000;
    sp2C.z = 0;

    player = &gRacePlayers[arg0->index];
    makeFixedRotationXY(sp38, player->pitchAngle, player->facingAngle);
    transformVec3iByFixedMatrix(sp38, &sp2C, (Vec3i *)&arg0->unk18);

    player = &gRacePlayers[arg0->index];
    arg0->unk18.word += player->renderPosition.x;
    arg0->unk1C.word += player->renderPosition.y;
    arg0->unk20.word += player->renderPosition.z;

    addRenderCallback(
        &gRaceModelEffectRenderCallbackList,
        (RenderCallback)drawRaceStartPlayerEffectSprite,
        (void *)arg0
    );
}

void waitForRaceStartPlayerEffect(MenuScreenEffectActor *arg0) {
    RacePlayer *player = &gRacePlayers[arg0->index];
    if (!(player->stateFlags & 0x2000)) {
        if (player->isTargetedByAttack != 0) {
            arg0->unk2E = 0xFF;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceStartPlayerEffectActive);
        }
    }
}

void drawTitleMenuSparkle(MenuScreenEffectActor *arg0) {
    if ((gFrameCounter & 0x3E) >= 0x1F) {
        drawAssetTableSprite(
            arg0->unk18.half.hi,
            arg0->unk18.half.lo,
            getRelocatableHeapBlockBase(gAssetHandles[6]),
            1
        );
    } else {
        drawAssetTableSprite(
            arg0->unk18.half.hi,
            arg0->unk18.half.lo,
            getRelocatableHeapBlockBase(gAssetHandles[6]),
            titleMenuSparkleTileFrames[(gFrameCounter & 0x1E) >> 1]
        );
    }
}

void renderTitleMenuSparkle(MenuScreenEffectActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawTitleMenuSparkle, arg0);
}

void initTitleMenuSparkle(MenuScreenEffectActor *arg0) {
    s16 *temp = &titleMenuSparklePositions[arg0->index * 2];

    arg0->unk18.half.hi = temp[0];
    arg0->unk18.half.lo = temp[1];
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)renderTitleMenuSparkle);
}

void drawCourseRecordBanner(MenuScreenEffectActor *arg0) {

    {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 255, 255, 255, arg0->unk18.half.lo);
        drawAssetTableSprite(-0x54, -0x10, getRelocatableHeapBlockBase(gAssetHandles[28]), 0x5D);
        drawAssetTableSprite(4, -0x10, getRelocatableHeapBlockBase(gAssetHandles[28]), 0x5E);
        if (arg0) {}
        drawAssetTableSprite(
            -0x4C,
            -0xC,
            getRelocatableHeapBlockBase(gAssetHandles[29]),
            (u16)courseRecordDigitTileOffsets[gRaceCourseIndex.signedValue]
        );
        drawAssetTableSprite(
            4,
            -0xC,
            getRelocatableHeapBlockBase(gAssetHandles[29]),
            courseRecordDigitTileOffsets[gRaceCourseIndex.signedValue] + 1
        );
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

void updateCourseRecordBannerFadeOut(MenuScreenEffectActor *arg0) {
    s16 v = arg0->unk18.half.hi;
    if (v != 0) {
        arg0->unk18.half.hi = v - 1;
    } else {
        arg0->unk18.half.lo -= 0x20;
        if (arg0->unk18.half.lo <= 0) {
            removeCallbackTask(arg0);
            return;
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseRecordBanner, arg0);
}

void initCourseRecordBannerFadeOut(MenuScreenEffectActor *arg0) {
    arg0->unk18.half.hi = 0x3E;
    arg0->unk18.half.lo = 0xFF;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseRecordBannerFadeOut);
}

void drawRaceSetupNamePlate(MenuScreenEffectActor *arg0) {
    u8 *new_var;

    new_var = &gCurrentViewportIndex;
    if (arg0->index == *new_var) {
        drawAssetTableSprite(
            (s16)((arg0->unk18.word >> 1) - 0x38),
            -8,
            getRelocatableHeapBlockBase(gAssetHandles[31]),
            0x76
        );
    }
}

void updateRaceSetupNamePlateSlideOut(MenuScreenEffectActor *arg0) {
    s32 temp_v0 = arg0->unk24.word;
    s32 temp_t8 = (unsigned long long)(temp_v0 + 4);

    arg0->unk18.word += temp_v0;
    arg0->unk24.word = temp_t8;
    if (temp_t8 == 0x38) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceSetupNamePlate, (void *)arg0);
    }
}

void holdRaceSetupNamePlate(MenuScreenEffectActor *arg0) {
    u16 temp = (arg0->timer & 0xFFFFU) - 1;

    arg0->timer = temp;
    temp += 0;
    if (temp == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSetupNamePlateSlideOut);
    }
    if (1) {}
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceSetupNamePlate, (void *)arg0);
}

void updateRaceSetupNamePlateSlideIn(MenuScreenEffectActor *arg0) {
    arg0->unk18.word += arg0->unk24.word;
    arg0->unk24.word -= 4;
    if (arg0->unk24.word == 0) {
        arg0->timer = 0x1E;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)holdRaceSetupNamePlate);
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceSetupNamePlate, (void *)arg0);
}

void waitForRaceSetupNamePlate(MenuScreenEffectActor *arg0) {
    if (gViewportStates[arg0->index].active != 0) {
        arg0->unk24.word = 0x38;
        arg0->unk18.word = -0x1A4;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSetupNamePlateSlideIn);
        return;
    }
    removeCallbackTask(arg0);
}

void drawCongratulationsBannerModel(MenuScreenEffectActor *arg0) {
    void *image;
    void *palette;
    Transform3D transform;
    Mtx *matrix;

    if (gCurrentViewportIndex == 2) {
        getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[34]), 0, &image, &palette);
        makeFixedRotationZ(transform.rotation, arg0->spriteIndex);
        transform.translation.x = arg0->unk18.word;
        transform.translation.y = arg0->unk1C.word;
        transform.translation.z = arg0->unk20.word;
        matrix = allocFixedTransformMatrix(&transform);
        if (matrix != NULL) {
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)gTranslucentSpriteRenderModeDl);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xFA000000, (arg0->alpha & 0xFF) | ~0xFF);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xFD48006B, (u32)image);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF5481C00, 0x07080200);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF4000000, 0x071B0040);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF5401C00, 0x00080200);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF2000000, 0x00360040);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)palette);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0x01020040, (u32)matrix);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0x01000040, (u32)gViewportMatrix);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)congratulationsBannerModelVertices);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x00060200);
        }
    }
}

void updateCongratulationsBannerAfterimage(MenuScreenEffectActor *arg0) {
    arg0->alpha -= 0x20;
    if (arg0->alpha == 0) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)drawCongratulationsBannerModel, (void *)arg0);
}

void updateCongratulationsBannerModel(MenuScreenEffectActor *arg0) {
    MenuScreenEffectActor *actor;

    arg0->unk1C.word = ((-0x400000LL * fixedSine((s16)(arg0->unk30.word + 0x400))) / 0x1000) + 0x700000;
    arg0->unk20.word = ((-0x9F0000LL * arg0->unk30.word) / 0x400) + 0xEC0000;
    arg0->spriteIndex = (0x5000LL * fixedSine(arg0->unk30.half.lo)) / 0x1000;
    if (arg0->unk30.word < 0x3F0) {
        actor = createCallbackTask((CallbackTaskCallback)updateCongratulationsBannerAfterimage, 0, 0x65);
        *(OverlayActorTransform *)&actor->unk18 = *(OverlayActorTransform *)&arg0->unk18;
        actor->spriteIndex = arg0->spriteIndex;
        actor->alpha = 0xC0;
        arg0->unk30.word += 0x10;
    } else {
        createCallbackTask((CallbackTaskCallback)updateCongratulationsBannerSprite, 0, 0x63);
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)drawCongratulationsBannerModel, (void *)arg0);
}

void initCongratulationsBannerModel(MenuScreenEffectActor *arg0) {
    arg0->alpha = 0xFF;
    arg0->unk30.word = 0;
    arg0->unk18.word = 0;
    arg0->unk1C.word = 0x300000;
    arg0->unk20.word = 0xEC0000;
    arg0->spriteIndex = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCongratulationsBannerModel);
}

void drawCongratulationsBannerSprite(MenuScreenEffectActor *arg0) {
    if (gCurrentViewportIndex == 2) {
        drawAssetTableSpriteWithExplicitPalette(
            -0x6C,
            -0x48,
            getRelocatableHeapBlockBase(gAssetHandles[34]),
            0,
            congratulationsBannerSpriteTileOffsets[arg0->unk18.half.lo]
        );
    }
}

void updateCongratulationsBannerSprite(MenuScreenEffectActor *arg0) {
    s16 v;
    arg0->unk18.half.hi += 1;
    v = arg0->unk18.half.hi;
    if (v >= 0x3D) {
        arg0->unk18.half.hi = 0;
        v = arg0->unk18.half.hi;
    }
    if (v < 0xA) {
        arg0->unk18.half.lo = v;
    } else {
        arg0->unk18.half.lo = 0;
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawCongratulationsBannerSprite, arg0);
}

void drawPassAwardLabel(MenuScreenEffectActor *arg0) {
    gDPPipeSync(gRegionAllocPtr++);
    gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->unk18.half.lo);

    switch (arg0->index) {
        case 0:
            drawAssetTableSprite(-0x8C, 0x38, getRelocatableHeapBlockBase(gAssetHandles[34]), 1);
            drawAssetTableSpriteWithExplicitPalette(
                -0x10,
                0x38,
                getRelocatableHeapBlockBase(gAssetHandles[34]),
                2,
                passAwardLabelFlashTileOffsets[gFrameCounter & 3]
            );
            break;
        case 1:
            drawAssetTableSprite(-0x89, 0x38, getRelocatableHeapBlockBase(gAssetHandles[34]), 1);
            drawAssetTableSpriteWithExplicitPalette(
                -0xD,
                0x38,
                getRelocatableHeapBlockBase(gAssetHandles[34]),
                3,
                passAwardLabelFlashTileOffsets[gFrameCounter & 3]
            );
            break;
        case 2:
            drawAssetTableSprite(-0x7E, 0x38, getRelocatableHeapBlockBase(gAssetHandles[34]), 1);
            drawAssetTableSpriteWithExplicitPalette(
                -2,
                0x38,
                getRelocatableHeapBlockBase(gAssetHandles[34]),
                4,
                passAwardLabelFlashTileOffsets[gFrameCounter & 3]
            );
            break;
    }

    gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
}

void updatePassAwardLabelFadeIn(MenuScreenEffectActor *arg0) {
    s16 v = arg0->unk18.half.hi;
    if (v != 0) {
        arg0->unk18.half.hi = v - 1;
        return;
    }
    arg0->unk18.half.lo += 0x20;
    if (arg0->unk18.half.lo >= 0x100) {
        arg0->unk18.half.lo = 0xFF;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawPassAwardLabel, arg0);
}

void initPassAwardLabelFadeIn(MenuScreenEffectActor *arg0) {
    arg0->unk18.half.hi = 0x50;
    arg0->unk18.half.lo = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updatePassAwardLabelFadeIn);
}

void drawMainMenuRotatingBoardModel(MenuScreenEffectActor *arg0) {
    TitleMenuRotatingBoardScratch scratch;
    Mtx *matrix;

    if (gCurrentViewportIndex == 0) {
        makeFixedRotationY(scratch.source.rotation, arg0->unk18.half.hi);
        scratch.source.translation.x = 0;
        scratch.source.translation.y = 0;
        scratch.source.translation.z = 0;
        matrix = allocFixedTransformMatrix(&scratch.source);
        if (matrix != NULL) {
            gDPPipeSync(gRegionAllocPtr++);

            gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[8]));
            gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[9]));

            gSPMatrix(gRegionAllocPtr++, matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            gSPDisplayList(gRegionAllocPtr++, gMainMenuRotatingBoardDl);
        }
    }
}

void drawMainMenuStaticBoardModel(void *arg0) {
    if (gCurrentViewportIndex == 0) {
        gDPPipeSync(gRegionAllocPtr++);

        gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[8]));
        gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[9]));

        gSPMatrix(gRegionAllocPtr++, &gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        gSPDisplayList(gRegionAllocPtr++, gMainMenuStaticBoardDl);
    }
}

void renderMainMenuBoardModels(MenuScreenEffectActor *arg0) {
    addRenderCallback(&gBackdropRenderCallbackList, (RenderCallback)drawMainMenuStaticBoardModel, arg0);
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)drawMainMenuRotatingBoardModel, arg0);
}

void initMainMenuBoardModels(MenuScreenEffectActor *arg0) {
    arg0->unk18.half.hi = 0x360;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)renderMainMenuBoardModels);
}
