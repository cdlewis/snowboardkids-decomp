#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/engine/render_callback.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/math/fixed_point_math.h"
#include "game/ending/ending_credits_effects.h"
#include "game/ending/ending_credits_flow.h"
#include "game/ending/ending_credits_tommy.h"
#include "game/menu/main_menu/main_menu_scene_model.h"
#include "game/menu/main_menu/main_menu_scene_model_renderer.h"
#include "game/engine/relocatable_heap.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"

#define ENDING_GFX_CMD(pkt, cmd0, cmd1) \
    {                                   \
        Gfx *_g = (Gfx *)(pkt);         \
        _g->words.w0 = (cmd0);          \
        _g->words.w1 = (cmd1);          \
    }

typedef struct {
    /* 0x00 */ void *palette;
    /* 0x04 */ void *image;
    /* 0x08 */ Vec3i transformed;
    /* 0x14 */ Vec3i pos;
    /* 0x20 */ s32 pad90;
    /* 0x24 */ Transform3D transform;
} EndingShadowStack;

extern MainMenuSceneActorShadow gEndingActorShadow;
extern Gfx *gRegionAllocPtr;
extern u32 gAlphaSpriteRenderModeDl[];
Vtx D_800B8100[4] = {
    { { { -16, 16, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 16, 16, 0 }, 0, { 480, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 16, -16, 0 }, 0, { 480, 480 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -16, -16, 0 }, 0, { 0, 480 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};
extern s16 gAssetHandles[];

void noopEndingCreditsTommy(void) {
}

void updateEndingTommyFinalPose(EndingCreditsCharacterActor *arg0) {
    stepMainMenuSceneModelAnimation(4);
    addMainMenuSceneModelDrawCallback(4);
}

void updateEndingTommyStartFinalPose(EndingCreditsCharacterActor *arg0) {
    stepMainMenuSceneModelAnimation(4);
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
    if (gEndingCreditsSequencePhase == 0x41) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingTommyFinalPose);
        setMainMenuSceneModelAnimation(4, 0x61);
    }
}

void updateEndingTommyWaitThenFinalPhase(EndingCreditsCharacterActor *arg0) {
    s32 new_var2;
    s32 sp18;
    s32 new_var;
    u16 temp_t7;
    u16 temp_v0;

    sp18 = stepMainMenuSceneModelAnimation(4);
    new_var2 = (new_var = 4);
    addMainMenuSceneModelDrawCallback(new_var2);
    if (sp18 == 1) {
        temp_v0 = arg0->stateTimer;
        temp_t7 = temp_v0;
        temp_t7 = temp_t7 + 1;
        if (temp_v0 < 0x1E) {
            arg0->stateTimer = temp_t7;
            if ((temp_t7 & 0xFFFFU) == 0x1E) {
                gEndingCreditsSequencePhase = 0x3E;
            }
        }
        if (gEndingCreditsSequencePhase == 0x40) {
            arg0->stateTimer = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingTommyStartFinalPose);
            setMainMenuSceneModelAnimation(4, 0x60);
        }
    }
}

void waitEndingTommyPhase3D(EndingCreditsCharacterActor *arg0) {
    loopMainMenuSceneModelAnimation(4);
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
    if (gEndingCreditsSequencePhase == 0x3D) {
        arg0->stateTimer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingTommyWaitThenFinalPhase);
        setMainMenuSceneModelAnimation(4, 0x5F);
    }
}

void updateEndingTommyEnterForPhase3A(EndingCreditsCharacterActor *arg0) {
    s32 limit = (s32)0xFF700000;

    loopMainMenuSceneModelAnimation(4);
    arg0->position.x += 0x48000;
    if (arg0->position.x >= limit) {
        arg0->position.x = limit;
        gEndingCreditsSequencePhase = 0x3A;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingTommyPhase3D);
    }
    setMainMenuSceneModelPosition(4, arg0->position.x, arg0->position.y, arg0->position.z);
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void waitEndingTommyPhase39(EndingCreditsCharacterActor *arg0) {
    u16 temp_t6;
    u16 temp_v0;

    temp_v0 = arg0->stateTimer;
    if (temp_v0 < 0x23) {
        temp_t6 = temp_v0 + 1;
        arg0->stateTimer = temp_t6;
        if ((temp_t6 & 0xFFFF) == 0x23) {
            gEndingCreditsSequencePhase = 0x10;
        }
    } else if (gEndingCreditsSequencePhase == 0x39) {
        arg0->position.x = 0xFCA00000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingTommyEnterForPhase3A);
        setMainMenuSceneModelAnimation(4, 4);
        arg0->rotation.y = 0;
        setMainMenuSceneModelRotation(4, arg0->rotation.x, arg0->rotation.y, arg0->rotation.z);
        gEndingActorShadow.modelPartIndex = 9;
        gEndingActorShadow.offset.x = 0xFFF20000;
        gEndingActorShadow.offset.y = 0xFFF20000;
        gEndingActorShadow.offset.z = 0xA0000;
    }
}

void updateEndingTommySlideLeftAfterBurst(EndingCreditsCharacterActor *arg0) {
    arg0->position.x += (s32)0xFFFE8000;
    if (arg0->position.x < (s32)0xFE700001) {
        arg0->position.x = (s32)0xFE700000;
        arg0->position.y = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingTommyPhase39);
    }
    setMainMenuSceneModelPosition(4, arg0->position.x, arg0->position.y, arg0->position.z);
    loopMainMenuSceneModelAnimation(4);
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void updateEndingTommyStartBurstExit(EndingCreditsCharacterActor *arg0) {
    EndingCreditsCharacterActor *new_var;
    s32 sp20;

    sp20 = stepMainMenuSceneModelAnimation(4);
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
    if (sp20 == 1) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingTommySlideLeftAfterBurst);
        setMainMenuSceneModelAnimation(4, 3);
        arg0->rotation.y = 0xC00;
        setMainMenuSceneModelRotation(4, arg0->rotation.x, (new_var = arg0)->rotation.y, arg0->rotation.z);
        gEndingActorShadow.modelPartIndex = 9;
        gEndingActorShadow.offset.x = 0xFFF20000;
        gEndingActorShadow.offset.y = 0xFFF20000;
        gEndingActorShadow.offset.z = 0;
    }
}

void updateEndingTommyWaitBeforeBurstExit(EndingCreditsCharacterActor *arg0) {
    s32 unused;
    volatile unsigned int sp18;
    s32 var_v0;

    sp18 = stepMainMenuSceneModelAnimation(4);
    addMainMenuSceneModelDrawCallback(4);
    if (sp18 == 1) {
        arg0->stateTimer++;
        if (arg0->stateTimer == 0x41) {
            arg0->stateTimer = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingTommyStartBurstExit);
            setMainMenuSceneModelAnimation(4, 0x1E);
        }
    } else {
        arg0->stateTimer++;
        var_v0 = arg0->stateTimer;
        if (var_v0 == 0x1F) {
            gEndingActorShadow.characterId = 4;
            gEndingActorShadow.modelPartIndex = 0xB;
            gEndingActorShadow.offset.y = -0x180000;
            var_v0 = arg0->stateTimer;
        }
        if (var_v0 == 0x27) {
            createCallbackTask((CallbackTaskCallback)initEndingCreditsTommyBigBurst, 0, 0x64);
            arg0->stateTimer = 0;
        }
    }
    if ((u8)gEndingActorShadow.characterId == 4) {
        addEndingActorShadowRenderCallback(&gEndingActorShadow);
    }
}

void updateEndingTommyWaitBeforeBurst(EndingCreditsCharacterActor *arg0) {
    u16 temp_v0 = arg0->stateTimer;
    EndingCreditsCharacterActor *temp_a2 = arg0;

    if (temp_v0 < 0x1E) {
        arg0->stateTimer = temp_v0 + 1;
    } else if (stepMainMenuSceneModelAnimation(4) == 1) {
        temp_a2->stateTimer = 0;
        setCallbackTaskCallback(temp_a2, (CallbackTaskCallback)updateEndingTommyWaitBeforeBurstExit);
        setMainMenuSceneModelAnimation(4, 0x1D);
    }
    addMainMenuSceneModelDrawCallback(4);
}

void waitEndingTommyPhase0F(EndingCreditsCharacterActor *arg0) {
    loopMainMenuSceneModelAnimation(4);
    addMainMenuSceneModelDrawCallback(4);
    arg0->stateTimer++;
    if (gEndingCreditsSequencePhase == 0xF) {
        arg0->stateTimer = 0;
        setMainMenuSceneModelPosition(4, arg0->position.x, arg0->position.y, arg0->position.z);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingTommyWaitBeforeBurst);
        setMainMenuSceneModelAnimation(4, 0x1C);
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_TOMMY] = 1;
    }
}

void updateEndingTommyStartPhase0CAuras(EndingCreditsCharacterActor *arg0) {
    if (stepMainMenuSceneModelAnimation(4) == 1) {
        arg0->stateTimer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingTommyPhase0F);
        setMainMenuSceneModelAnimation(4, 0xC);
        gEndingCreditsSequencePhase = 0xC;
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_TOMMY] = 0;
        spawnEndingCreditsCharacterAura(-0x24, -0x32, 4, 0);
        spawnEndingCreditsCharacterAura(0x10, -0x32, 4, 1);
    }
    addMainMenuSceneModelDrawCallback(4);
}

void updateEndingTommyWaitBeforePhase0CAuras(EndingCreditsCharacterActor *arg0) {
    if (stepMainMenuSceneModelAnimation(4) == 1) {
        arg0->stateTimer++;
        if (arg0->stateTimer == 0x14) {
            arg0->stateTimer = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingTommyStartPhase0CAuras);
            setMainMenuSceneModelAnimation(4, 0xB);
        }
    }
    addMainMenuSceneModelDrawCallback(4);
}

void updateEndingTommySlideLeftToPhase0A(EndingCreditsCharacterActor *arg0) {
    loopMainMenuSceneModelAnimation(4);
    arg0->position.x += (s32)0xFFFB8000;
    if (arg0->position.x < (s32)0xFF600001) {
        arg0->position.x = (s32)0xFF600000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingTommyWaitBeforePhase0CAuras);
        setMainMenuSceneModelAnimation(4, 0xA);
    }
    setMainMenuSceneModelPosition(4, arg0->position.x, arg0->position.y, arg0->position.z);
    addMainMenuSceneModelDrawCallback(4);
}

void updateEndingTommyHopLeftToPhase0A(EndingCreditsCharacterActor *arg0) {
    if (stepMainMenuSceneModelAnimation(4) == 0) {
        s32 var_v0 = (arg0->stateTimer < 5) ? 1 : -1;

        arg0->position.y += var_v0 * 0x3E000;
        arg0->position.x += (s32)0xFFF60000;
        setMainMenuSceneModelPosition(4, arg0->position.x, arg0->position.y, arg0->position.z);
    } else {
        arg0->position.y = 0x6C000;
        setMainMenuSceneModelPosition(4, arg0->position.x, 0x6C000, arg0->position.z);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingTommySlideLeftToPhase0A);
        setMainMenuSceneModelAnimation(4, 9);
    }
    addMainMenuSceneModelDrawCallback(4);
}

void waitEndingTommyPhase0B(EndingCreditsCharacterActor *arg0) {
    if (gEndingCreditsSequencePhase < 0xA) {
        loopMainMenuSceneModelAnimation(4);
    } else if (gEndingCreditsSequencePhase == 0xB) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingTommyHopLeftToPhase0A);
        setMainMenuSceneModelAnimation(4, 8);
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_TOMMY] = 1;
    }
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void waitEndingTommyPhase08Aura(EndingCreditsCharacterActor *arg0) {
    if (gEndingCreditsSequencePhase == 8) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingTommyPhase0B);
        setMainMenuSceneModelAnimation(4, 7);
        gEndingActorShadow.offset.y = (s32)0xFFE80000;
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_TOMMY] = 0;
        spawnEndingCreditsCharacterAura(-0x24, -0x32, 4, 0);
    }
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void updateEndingTommyRepeatAnimThenPhase07(EndingCreditsCharacterActor *arg0) {
    if (stepMainMenuSceneModelAnimation(4) == 1) {
        arg0->stateTimer++;
        if (arg0->stateTimer < 6) {
            setMainMenuSceneModelAnimation(4, 6);
        }
    }
    if (arg0->stateTimer == 6) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingTommyPhase08Aura);
        arg0->stateTimer = 0;
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_TOMMY] = 1;
        gEndingCreditsSequencePhase = 7;
    }
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void updateEndingTommyWaitForPhase06(EndingCreditsCharacterActor *arg0) {
    if ((stepMainMenuSceneModelAnimation(4) == 1) && (gEndingCreditsSequencePhase == 4)) {
        gEndingCreditsSequencePhase = 5;
    }
    if (gEndingCreditsSequencePhase == 6) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingTommyRepeatAnimThenPhase07);
        setMainMenuSceneModelAnimation(4, 6);
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_TOMMY] = 0;
        spawnEndingCreditsCharacterAura(-0x1C, -0x3A, 4, 0);
    }
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void waitEndingTommyPhase04(EndingCreditsCharacterActor *arg0) {
    loopMainMenuSceneModelAnimation(4);
    if (gEndingCreditsSequencePhase == 4) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingTommyWaitForPhase06);
        setMainMenuSceneModelAnimation(4, 5);
    }
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void updateEndingTommyEnterToCenter(EndingCreditsCharacterActor *arg0) {
    arg0->position.x += 0x24000;
    if (arg0->position.x >= 0x100000) {
        arg0->position.x = 0x100000;
        gEndingCreditsSequencePhase = 2;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingTommyPhase04);
        setMainMenuSceneModelPosition(4, arg0->position.x, arg0->position.y, arg0->position.z);
        setMainMenuSceneModelAnimation(4, 4);
    } else {
        setMainMenuSceneModelPosition(4, arg0->position.x, arg0->position.y, arg0->position.z);
        loopMainMenuSceneModelAnimation(4);
    }
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void waitEndingTommyPhase01(EndingCreditsCharacterActor *arg0) {
    if (gEndingCreditsSequencePhase == 1) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingTommyEnterToCenter);
        createCallbackTask((CallbackTaskCallback)&initEndingCreditsTommySnowmanEntrance, 0, 0x64);
    }
}

void initEndingCreditsTommy(EndingCreditsCharacterActor *arg0) {
    arg0->position.x = (s32)0xFE700000;
    arg0->position.y = 0;
    arg0->position.z = 0;
    arg0->rotation.x = 0;
    arg0->rotation.y = 0x400;
    arg0->rotation.z = 0;
    arg0->stateTimer = 0;
    initMainMenuSceneModel(4, 4);
    setMainMenuSceneModelAnimation(4, 3);
    setMainMenuSceneModelPosition(4, arg0->position.x, arg0->position.y, arg0->position.z);
    setMainMenuSceneModelRotation(4, arg0->rotation.x, arg0->rotation.y, arg0->rotation.z);
    gEndingActorShadow.characterId = 4;
    gEndingActorShadow.modelPartIndex = 9;
    gEndingActorShadow.offset.x = (s32)0xFFF20000;
    gEndingActorShadow.offset.y = (s32)0xFFF20000;
    gEndingActorShadow.offset.z = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingTommyPhase01);
}

void drawEndingActorShadow(MainMenuSceneActorShadow *arg0) {
    Mtx *matrix;
    EndingShadowStack stack;
    MainMenuSceneModel *model;

    model = getMainMenuSceneModel((u8)arg0->characterId);
    stack.pos.x = arg0->offset.x;
    stack.pos.y = arg0->offset.y;
    stack.pos.z = arg0->offset.z;
    transformVec3iByFixedMatrix(model->partTransforms[(u8)arg0->modelPartIndex].rotation, &stack.pos, &stack.transformed);
    stack.transform = gIdentityFixedTransform;
    stack.transform.translation.x = model->partTransforms[(u8)arg0->modelPartIndex].translation.x + stack.transformed.x;
    stack.transform.translation.y = model->partTransforms[(u8)arg0->modelPartIndex].translation.y + stack.transformed.y;
    stack.transform.translation.z = model->partTransforms[(u8)arg0->modelPartIndex].translation.z;

    ENDING_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)gAlphaSpriteRenderModeDl);

    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x21]), 0x31, &stack.image, &stack.palette);

    ENDING_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)stack.palette);
    ENDING_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
    ENDING_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
    ENDING_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
    ENDING_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
    ENDING_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
    matrix = allocFixedTransformMatrix(&stack.transform);
    ENDING_GFX_CMD(gRegionAllocPtr++, 0x01020040, (u32)matrix);
    ENDING_GFX_CMD(gRegionAllocPtr++, 0xFD500000, (u32)stack.image);
    ENDING_GFX_CMD(gRegionAllocPtr++, 0xF5500000, 0x07080200);
    ENDING_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
    ENDING_GFX_CMD(gRegionAllocPtr++, 0xF3000000, 0x0703F800);
    ENDING_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
    ENDING_GFX_CMD(gRegionAllocPtr++, 0xF5400200, 0x80200);
    ENDING_GFX_CMD(gRegionAllocPtr++, 0xF2000000, 0x3C03C);
    ENDING_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)D_800B8100);
    ENDING_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
}

void addEndingActorShadowRenderCallback(MainMenuSceneActorShadow *arg0) {
    addRenderCallback(&gModelRenderCallbackList, (RenderCallback)drawEndingActorShadow, arg0);
}
