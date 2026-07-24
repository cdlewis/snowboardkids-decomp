#include "common.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/math/fixed_point_math.h"
#include "game/ending/ending_credits_effects.h"
#include "game/ending/ending_credits_flow.h"
#include "game/ending/ending_credits_tommy.h"
#include "game/menu/main_menu/main_menu_scene_model.h"
#include "game/menu/main_menu/main_menu_scene_model_renderer.h"
#include "game/engine/relocatable_heap.h"

#define ENDING_GFX_CMD(pkt, cmd0, cmd1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (cmd0); \
    _g->words.w1 = (cmd1); \
}

struct EndingCreditsTommy {
    /* 0x00 */ char pad0[0x18];
    /* 0x18 */ s32 posX;
    /* 0x1C */ s32 posY;
    /* 0x20 */ s32 posZ;
    /* 0x24 */ s16 rotX;
    /* 0x26 */ s16 rotY;
    /* 0x28 */ s16 rotZ;
    /* 0x2A */ u16 timer;
};

typedef struct {
    /* 0x00 */ FixedMatrix3s rotation;
    /* 0x12 */ s16 pad12;
    /* 0x14 */ s32 x;
    /* 0x18 */ s32 y;
    /* 0x1C */ s32 z;
} GfxCommandSource;

typedef struct {
    /* 0x00 */ void *palette;
    /* 0x04 */ void *image;
    /* 0x08 */ Vec3i transformed;
    /* 0x14 */ Vec3i pos;
    /* 0x20 */ s32 pad90;
    /* 0x24 */ GfxCommandSource transform;
} EndingShadowStack;

extern void getAssetTableImageAndPalette(s32 arg0, s32 arg1, void **arg2, void **arg3);
extern Mtx *allocFixedTransformMatrix(GfxCommandSource *arg0);
extern MainMenuSceneActorShadow gEndingActorShadow;
extern void addRenderCallback(void *, void *, void *);
extern s32 gModelRenderCallbackList;
extern Gfx *gRegionAllocPtr;
extern GfxCommandSource gIdentityFixedTransform;
extern u32 gAlphaSpriteRenderModeDl[];
extern Vtx D_800B8100[];
extern s16 gAssetHandles[];

void noopEndingCreditsTommy(void) {
}

void updateEndingTommyFinalPose(EndingCreditsTommy *arg0) {
    stepMainMenuSceneModelAnimation(4);
    addMainMenuSceneModelDrawCallback(4);
}

void updateEndingTommyStartFinalPose(EndingCreditsTommy *arg0) {
    stepMainMenuSceneModelAnimation(4);
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
    if (gEndingCreditsSequencePhase == 0x41) {
        setCallbackTaskCallback(arg0, updateEndingTommyFinalPose);
        setMainMenuSceneModelAnimation(4, 0x61);
    }
}

void updateEndingTommyWaitThenFinalPhase(EndingCreditsTommy *arg0) {
    s32 new_var2;
    s32 sp18;
    s32 new_var;
    u16 temp_t7;
    u16 temp_v0;

    sp18 = stepMainMenuSceneModelAnimation(4);
    new_var2 = (new_var = 4);
    addMainMenuSceneModelDrawCallback(new_var2);
    if (sp18 == 1) {
        temp_v0 = arg0->timer;
        temp_t7 = temp_v0;
        temp_t7 = temp_t7 + 1;
        if (temp_v0 < 0x1E) {
            arg0->timer = temp_t7;
            if ((temp_t7 & 0xFFFFU) == 0x1E) {
                gEndingCreditsSequencePhase = 0x3E;
            }
        }
        if (gEndingCreditsSequencePhase == 0x40) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, updateEndingTommyStartFinalPose);
            setMainMenuSceneModelAnimation(4, 0x60);
        }
    }
}

void waitEndingTommyPhase3D(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(4);
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
    if (gEndingCreditsSequencePhase == 0x3D) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingTommyWaitThenFinalPhase);
        setMainMenuSceneModelAnimation(4, 0x5F);
    }
}

void updateEndingTommyEnterForPhase3A(EndingCreditsTommy *arg0) {
    s32 limit = (s32)0xFF700000;

    loopMainMenuSceneModelAnimation(4);
    arg0->posX += 0x48000;
    if (arg0->posX >= limit) {
        arg0->posX = limit;
        gEndingCreditsSequencePhase = 0x3A;
        setCallbackTaskCallback(arg0, waitEndingTommyPhase3D);
    }
    setMainMenuSceneModelPosition(4, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void waitEndingTommyPhase39(EndingCreditsTommy *arg0) {
    u16 temp_t6;
    u16 temp_v0;

    temp_v0 = arg0->timer;
    if (temp_v0 < 0x23) {
        temp_t6 = temp_v0 + 1;
        arg0->timer = temp_t6;
        if ((temp_t6 & 0xFFFF) == 0x23) {
            gEndingCreditsSequencePhase = 0x10;
        }
    } else if (gEndingCreditsSequencePhase == 0x39) {
        arg0->posX = 0xFCA00000;
        setCallbackTaskCallback(arg0, updateEndingTommyEnterForPhase3A);
        setMainMenuSceneModelAnimation(4, 4);
        arg0->rotY = 0;
        setMainMenuSceneModelRotation(4, arg0->rotX, arg0->rotY, arg0->rotZ);
        gEndingActorShadow.unkC = 9;
        gEndingActorShadow.posX = 0xFFF20000;
        gEndingActorShadow.posY = 0xFFF20000;
        gEndingActorShadow.posZ = 0xA0000;
    }
}

void updateEndingTommySlideLeftAfterBurst(EndingCreditsTommy *arg0) {
    arg0->posX += (s32)0xFFFE8000;
    if (arg0->posX < (s32)0xFE700001) {
        arg0->posX = (s32)0xFE700000;
        arg0->posY = 0;
        setCallbackTaskCallback(arg0, waitEndingTommyPhase39);
    }
    setMainMenuSceneModelPosition(4, arg0->posX, arg0->posY, arg0->posZ);
    loopMainMenuSceneModelAnimation(4);
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void updateEndingTommyStartBurstExit(EndingCreditsTommy *arg0) {
    EndingCreditsTommy *new_var;
    s32 sp20;

    sp20 = stepMainMenuSceneModelAnimation(4);
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
    if (sp20 == 1) {
        setCallbackTaskCallback(arg0, updateEndingTommySlideLeftAfterBurst);
        setMainMenuSceneModelAnimation(4, 3);
        arg0->rotY = 0xC00;
        setMainMenuSceneModelRotation(4, arg0->rotX, (new_var = arg0)->rotY, arg0->rotZ);
        gEndingActorShadow.unkC = 9;
        gEndingActorShadow.posX = 0xFFF20000;
        gEndingActorShadow.posY = 0xFFF20000;
        gEndingActorShadow.posZ = 0;
    }
}

void updateEndingTommyWaitBeforeBurstExit(EndingCreditsTommy *arg0) {
    s32 unused;
    volatile unsigned int sp18;
    s32 var_v0;

    sp18 = stepMainMenuSceneModelAnimation(4);
    addMainMenuSceneModelDrawCallback(4);
    if (sp18 == 1) {
        arg0->timer++;
        if (arg0->timer == 0x41) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, updateEndingTommyStartBurstExit);
            setMainMenuSceneModelAnimation(4, 0x1E);
        }
    } else {
        arg0->timer++;
        var_v0 = arg0->timer;
        if (var_v0 == 0x1F) {
            gEndingActorShadow.actorId = 4;
            gEndingActorShadow.unkC = 0xB;
            gEndingActorShadow.posY = -0x180000;
            var_v0 = arg0->timer;
        }
        if (var_v0 == 0x27) {
            createCallbackTask(initEndingCreditsTommyBigBurst, 0, 0x64);
            arg0->timer = 0;
        }
    }
    if ((u8)gEndingActorShadow.actorId == 4) {
        addEndingActorShadowRenderCallback(&gEndingActorShadow);
    }
}

void updateEndingTommyWaitBeforeBurst(EndingCreditsTommy *arg0) {
    u16 temp_v0 = arg0->timer;
    EndingCreditsTommy *temp_a2 = arg0;

    if (temp_v0 < 0x1E) {
        arg0->timer = temp_v0 + 1;
    } else if (stepMainMenuSceneModelAnimation(4) == 1) {
        temp_a2->timer = 0;
        setCallbackTaskCallback(temp_a2, updateEndingTommyWaitBeforeBurstExit);
        setMainMenuSceneModelAnimation(4, 0x1D);
    }
    addMainMenuSceneModelDrawCallback(4);
}

void waitEndingTommyPhase0F(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(4);
    addMainMenuSceneModelDrawCallback(4);
    arg0->timer++;
    if (gEndingCreditsSequencePhase == 0xF) {
        arg0->timer = 0;
        setMainMenuSceneModelPosition(4, arg0->posX, arg0->posY, arg0->posZ);
        setCallbackTaskCallback(arg0, updateEndingTommyWaitBeforeBurst);
        setMainMenuSceneModelAnimation(4, 0x1C);
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_TOMMY] = 1;
    }
}

void updateEndingTommyStartPhase0CAuras(EndingCreditsTommy *arg0) {
    if (stepMainMenuSceneModelAnimation(4) == 1) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, waitEndingTommyPhase0F);
        setMainMenuSceneModelAnimation(4, 0xC);
        gEndingCreditsSequencePhase = 0xC;
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_TOMMY] = 0;
        spawnEndingCreditsCharacterAura(-0x24, -0x32, 4, 0);
        spawnEndingCreditsCharacterAura(0x10, -0x32, 4, 1);
    }
    addMainMenuSceneModelDrawCallback(4);
}

void updateEndingTommyWaitBeforePhase0CAuras(EndingCreditsTommy *arg0) {
    if (stepMainMenuSceneModelAnimation(4) == 1) {
        arg0->timer++;
        if (arg0->timer == 0x14) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, updateEndingTommyStartPhase0CAuras);
            setMainMenuSceneModelAnimation(4, 0xB);
        }
    }
    addMainMenuSceneModelDrawCallback(4);
}

void updateEndingTommySlideLeftToPhase0A(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(4);
    arg0->posX += (s32)0xFFFB8000;
    if (arg0->posX < (s32)0xFF600001) {
        arg0->posX = (s32)0xFF600000;
        setCallbackTaskCallback(arg0, updateEndingTommyWaitBeforePhase0CAuras);
        setMainMenuSceneModelAnimation(4, 0xA);
    }
    setMainMenuSceneModelPosition(4, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(4);
}

void updateEndingTommyHopLeftToPhase0A(EndingCreditsTommy *arg0) {
    if (stepMainMenuSceneModelAnimation(4) == 0) {
        s32 var_v0 = (arg0->timer < 5) ? 1 : -1;

        arg0->posY += var_v0 * 0x3E000;
        arg0->posX += (s32)0xFFF60000;
        setMainMenuSceneModelPosition(4, arg0->posX, arg0->posY, arg0->posZ);
    } else {
        arg0->posY = 0x6C000;
        setMainMenuSceneModelPosition(4, arg0->posX, 0x6C000, arg0->posZ);
        setCallbackTaskCallback(arg0, updateEndingTommySlideLeftToPhase0A);
        setMainMenuSceneModelAnimation(4, 9);
    }
    addMainMenuSceneModelDrawCallback(4);
}

void waitEndingTommyPhase0B(EndingCreditsTommy *arg0) {
    if (gEndingCreditsSequencePhase < 0xA) {
        loopMainMenuSceneModelAnimation(4);
    } else if (gEndingCreditsSequencePhase == 0xB) {
        setCallbackTaskCallback(arg0, updateEndingTommyHopLeftToPhase0A);
        setMainMenuSceneModelAnimation(4, 8);
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_TOMMY] = 1;
    }
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void waitEndingTommyPhase08Aura(EndingCreditsTommy *arg0) {
    if (gEndingCreditsSequencePhase == 8) {
        setCallbackTaskCallback(arg0, waitEndingTommyPhase0B);
        setMainMenuSceneModelAnimation(4, 7);
        gEndingActorShadow.posY = (s32)0xFFE80000;
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_TOMMY] = 0;
        spawnEndingCreditsCharacterAura(-0x24, -0x32, 4, 0);
    }
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void updateEndingTommyRepeatAnimThenPhase07(EndingCreditsTommy *arg0) {
    if (stepMainMenuSceneModelAnimation(4) == 1) {
        arg0->timer++;
        if (arg0->timer < 6) {
            setMainMenuSceneModelAnimation(4, 6);
        }
    }
    if (arg0->timer == 6) {
        setCallbackTaskCallback(arg0, waitEndingTommyPhase08Aura);
        arg0->timer = 0;
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_TOMMY] = 1;
        gEndingCreditsSequencePhase = 7;
    }
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void updateEndingTommyWaitForPhase06(EndingCreditsTommy *arg0) {
    if ((stepMainMenuSceneModelAnimation(4) == 1) && (gEndingCreditsSequencePhase == 4)) {
        gEndingCreditsSequencePhase = 5;
    }
    if (gEndingCreditsSequencePhase == 6) {
        setCallbackTaskCallback(arg0, updateEndingTommyRepeatAnimThenPhase07);
        setMainMenuSceneModelAnimation(4, 6);
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_TOMMY] = 0;
        spawnEndingCreditsCharacterAura(-0x1C, -0x3A, 4, 0);
    }
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void waitEndingTommyPhase04(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(4);
    if (gEndingCreditsSequencePhase == 4) {
        setCallbackTaskCallback(arg0, updateEndingTommyWaitForPhase06);
        setMainMenuSceneModelAnimation(4, 5);
    }
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void updateEndingTommyEnterToCenter(EndingCreditsTommy *arg0) {
    arg0->posX += 0x24000;
    if (arg0->posX >= 0x100000) {
        arg0->posX = 0x100000;
        gEndingCreditsSequencePhase = 2;
        setCallbackTaskCallback(arg0, waitEndingTommyPhase04);
        setMainMenuSceneModelPosition(4, arg0->posX, arg0->posY, arg0->posZ);
        setMainMenuSceneModelAnimation(4, 4);
    } else {
        setMainMenuSceneModelPosition(4, arg0->posX, arg0->posY, arg0->posZ);
        loopMainMenuSceneModelAnimation(4);
    }
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void waitEndingTommyPhase01(EndingCreditsTommy *arg0) {
    if (gEndingCreditsSequencePhase == 1) {
        setCallbackTaskCallback(arg0, updateEndingTommyEnterToCenter);
        createCallbackTask(&initEndingCreditsTommySnowmanEntrance, 0, 0x64);
    }
}

void initEndingCreditsTommy(EndingCreditsTommy *arg0) {
    arg0->posX = (s32)0xFE700000;
    arg0->posY = 0;
    arg0->posZ = 0;
    arg0->rotX = 0;
    arg0->rotY = 0x400;
    arg0->rotZ = 0;
    arg0->timer = 0;
    initMainMenuSceneModel(4, 4);
    setMainMenuSceneModelAnimation(4, 3);
    setMainMenuSceneModelPosition(4, arg0->posX, arg0->posY, arg0->posZ);
    setMainMenuSceneModelRotation(4, arg0->rotX, arg0->rotY, arg0->rotZ);
    gEndingActorShadow.actorId = 4;
    gEndingActorShadow.unkC = 9;
    gEndingActorShadow.posX = (s32)0xFFF20000;
    gEndingActorShadow.posY = (s32)0xFFF20000;
    gEndingActorShadow.posZ = 0;
    setCallbackTaskCallback(arg0, waitEndingTommyPhase01);
}

void drawEndingActorShadow(MainMenuSceneActorShadow *arg0) {
    Mtx *matrix;
    EndingShadowStack stack;
    MainMenuSceneModel *model;

    model = getMainMenuSceneModel((u8)arg0->actorId);
    stack.pos.x = arg0->posX;
    stack.pos.y = arg0->posY;
    stack.pos.z = arg0->posZ;
    transformVec3iByFixedMatrix(model->displayObjects[(u8)arg0->unkC].rotation, &stack.pos, &stack.transformed);
    stack.transform = gIdentityFixedTransform;
    stack.transform.x = model->displayObjects[(u8)arg0->unkC].translation.x + stack.transformed.x;
    stack.transform.y = model->displayObjects[(u8)arg0->unkC].translation.y + stack.transformed.y;
    stack.transform.z = model->displayObjects[(u8)arg0->unkC].translation.z;

    ENDING_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)gAlphaSpriteRenderModeDl);

    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x21]), 0x31, &stack.image,
                                 &stack.palette);

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
    addRenderCallback(&gModelRenderCallbackList, drawEndingActorShadow, arg0);
}
