#include "common.h"
#include "callback_task_scheduler.h"
#include "fixed_point_math.h"
#include "ending_sequence_actors.h"
#include "ending_credits_tommy.h"
#include "main_menu_scene_model.h"
#include "main_menu_scene_model_renderer.h"
#include "relocatable_heap.h"

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

extern void getAssetTableImageAndPalette(s32 arg0, s32 arg1, void **arg2, void **arg3);
extern Mtx *allocFixedTransformMatrix(GfxCommandSource *arg0);
extern MainMenuSceneActorShadow gEndingActorShadow;
extern u16 gEndingSequencePhase;
extern s8 gEndingTommyEffectDone;
extern void addRenderCallback(void *, void *, void *);
extern s32 gModelRenderCallbackList;
extern Gfx *gRegionAllocPtr;
extern GfxCommandSource gIdentityFixedTransform;
extern u32 gAlphaSpriteRenderModeDl[];
extern Vtx D_800B8100[];
extern s16 gMenuCommonSpritesAssetHandle;
extern void drawEndingActorShadow(MainMenuSceneActorShadow *arg0);

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
    if (gEndingSequencePhase == 0x41) {
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
                gEndingSequencePhase = 0x3E;
            }
        }
        if (gEndingSequencePhase == 0x40) {
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
    if (gEndingSequencePhase == 0x3D) {
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
        gEndingSequencePhase = 0x3A;
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
            gEndingSequencePhase = 0x10;
        }
    } else if (gEndingSequencePhase == 0x39) {
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
            createCallbackTask(initEndingTommyBigBurst, 0, 0x64);
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
    if (gEndingSequencePhase == 0xF) {
        arg0->timer = 0;
        setMainMenuSceneModelPosition(4, arg0->posX, arg0->posY, arg0->posZ);
        setCallbackTaskCallback(arg0, updateEndingTommyWaitBeforeBurst);
        setMainMenuSceneModelAnimation(4, 0x1C);
        gEndingTommyEffectDone = 1;
    }
}

void updateEndingTommyStartPhase0CAuras(EndingCreditsTommy *arg0) {
    if (stepMainMenuSceneModelAnimation(4) == 1) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, waitEndingTommyPhase0F);
        setMainMenuSceneModelAnimation(4, 0xC);
        gEndingSequencePhase = 0xC;
        gEndingTommyEffectDone = 0;
        spawnEndingCharacterAura(-0x24, -0x32, 4, 0);
        spawnEndingCharacterAura(0x10, -0x32, 4, 1);
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
    if (gEndingSequencePhase < 0xA) {
        loopMainMenuSceneModelAnimation(4);
    } else if (gEndingSequencePhase == 0xB) {
        setCallbackTaskCallback(arg0, updateEndingTommyHopLeftToPhase0A);
        setMainMenuSceneModelAnimation(4, 8);
        gEndingTommyEffectDone = 1;
    }
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void waitEndingTommyPhase08Aura(EndingCreditsTommy *arg0) {
    if (gEndingSequencePhase == 8) {
        setCallbackTaskCallback(arg0, waitEndingTommyPhase0B);
        setMainMenuSceneModelAnimation(4, 7);
        gEndingActorShadow.posY = (s32)0xFFE80000;
        gEndingTommyEffectDone = 0;
        spawnEndingCharacterAura(-0x24, -0x32, 4, 0);
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
        gEndingTommyEffectDone = 1;
        gEndingSequencePhase = 7;
    }
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void updateEndingTommyWaitForPhase06(EndingCreditsTommy *arg0) {
    if ((stepMainMenuSceneModelAnimation(4) == 1) && (gEndingSequencePhase == 4)) {
        gEndingSequencePhase = 5;
    }
    if (gEndingSequencePhase == 6) {
        setCallbackTaskCallback(arg0, updateEndingTommyRepeatAnimThenPhase07);
        setMainMenuSceneModelAnimation(4, 6);
        gEndingTommyEffectDone = 0;
        spawnEndingCharacterAura(-0x1C, -0x3A, 4, 0);
    }
    addMainMenuSceneModelDrawCallback(4);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void waitEndingTommyPhase04(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(4);
    if (gEndingSequencePhase == 4) {
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
        gEndingSequencePhase = 2;
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
    if (gEndingSequencePhase == 1) {
        setCallbackTaskCallback(arg0, updateEndingTommyEnterToCenter);
        createCallbackTask(&initEndingTommySnowmanEntrance, 0, 0x64);
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

// drawEndingActorShadow best match: 74.234% at nonmatchings/drawEndingActorShadow-4061930211835852828/base_4.c.
#pragma GLOBAL_ASM("asm/nonmatchings/ending_credits_tommy/drawEndingActorShadow.s")

#ifdef NON_MATCHING
void drawEndingActorShadow(MainMenuSceneActorShadow *arg0) {
    void *spB0;
    s32 spAC;
    s32 spA8;
    GfxCommandSource sp94;
    Vec3i sp84;
    Vec3i sp78;
    void *sp74;
    void *sp70;
    MainMenuSceneModel *sp6C;
    MainMenuSceneModel *model;
    Gfx *gfx;
    volatile u8 pad[0x38];

    model = getMainMenuSceneModel(arg0->actorId);
    sp84.x = arg0->posX;
    sp84.y = arg0->posY;
    sp84.z = arg0->posZ;
    transformVec3iByFixedMatrix(model->displayObjects[arg0->unkC].pad0, &sp84, &sp78);
    sp6C = model;
    sp94 = gIdentityFixedTransform;
    spA8 = sp6C->displayObjects[arg0->unkC].screenX + sp78.x;
    spAC = sp6C->displayObjects[arg0->unkC].screenY + sp78.y;
    spB0 = (void *)sp6C->displayObjects[arg0->unkC].screenZ;

    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w0 = 0x06000000;
    gfx->words.w1 = (u32)gAlphaSpriteRenderModeDl;

    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gMenuCommonSpritesAssetHandle), 0x31, &sp74, &sp70);

    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w0 = 0xFD100000;
    gfx->words.w1 = (u32)sp70;
    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w1 = 0;
    gfx->words.w0 = 0xE8000000;
    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w1 = 0x07000000;
    gfx->words.w0 = 0xF5000100;
    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w1 = 0;
    gfx->words.w0 = 0xE6000000;
    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w0 = 0xF0000000;
    gfx->words.w1 = 0x0703C000;
    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w1 = 0;
    gfx->words.w0 = 0xE7000000;

    sp94.x = spA8;
    sp94.y = spAC;
    sp94.z = (s32)spB0;

    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w0 = 0x01020040;
    gfx->words.w1 = (u32)allocFixedTransformMatrix(&sp94);

    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w0 = 0xFD500000;
    gfx->words.w1 = (u32)sp74;
    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w0 = 0xF5500000;
    gfx->words.w1 = 0x07080200;
    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w1 = 0;
    gfx->words.w0 = 0xE6000000;
    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w0 = 0xF3000000;
    gfx->words.w1 = 0x0703F800;
    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w1 = 0;
    gfx->words.w0 = 0xE7000000;
    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w1 = 0x80200;
    gfx->words.w0 = 0xF5400200;
    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w0 = 0xF2000000;
    gfx->words.w1 = 0x3C03C;
    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w1 = (u32)D_800B8100;
    gfx->words.w0 = 0x0400103F;
    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w1 = 0x60200;
    gfx->words.w0 = 0xB1060402;
}
#endif

void addEndingActorShadowRenderCallback(MainMenuSceneActorShadow *arg0) {
    addRenderCallback(&gModelRenderCallbackList, drawEndingActorShadow, arg0);
}
