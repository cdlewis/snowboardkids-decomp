#include "common.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/ending/ending_credits_effects.h"
#include "game/ending/ending_credits_flow.h"
#include "game/ending/ending_credits_jam.h"
#include "game/ending/ending_credits_linda.h"
#include "game/menu/main_menu/main_menu_scene_model.h"
#include "game/menu/main_menu/main_menu_scene_model_renderer.h"

void updateEndingJamFinalPose(EndingCreditsJam *arg0) {
    stepMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
}

void updateEndingJamStartFinalPose(EndingCreditsJam *arg0) {
    stepMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    if (gEndingCreditsSequencePhase == 0x41) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamFinalPose);
        setMainMenuSceneModelAnimation(2, 0x56);
        arg0->rotY = 0xC00;
        setMainMenuSceneModelRotation(2, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
}

void waitEndingJamPhase40(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    if (gEndingCreditsSequencePhase == 0x40) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamStartFinalPose);
        setMainMenuSceneModelAnimation(2, 0x68);
    }
}

void updateEndingJamWaitBeforePhase40(EndingCreditsJam *arg0) {
    u16 *p;
    loopMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    p = &arg0->timer;
    (*p)++;
    if (*p == 0x1E) {
        *p = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingJamPhase40);
        gEndingCreditsSequencePhase = 0x3F;
    }
}

void updateEndingJamPhase3FAnim3(EndingCreditsJam *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    if (sp18 == 1) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamWaitBeforePhase40);
        setMainMenuSceneModelAnimation(2, 0x67);
    }
}

void updateEndingJamPhase3FAnim2(EndingCreditsJam *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    if (sp18 == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0x1E) {
            *p = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamPhase3FAnim3);
            setMainMenuSceneModelAnimation(2, 0x66);
        }
    }
}

void updateEndingJamPhase3FAnim1(EndingCreditsJam *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    if (sp18 == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0x14) {
            *p = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamPhase3FAnim2);
            setMainMenuSceneModelAnimation(2, 0x65);
            spawnEndingCreditsPhaseAdvanceSparkle(-0x1F, -0x6B);
        }
    }
}

void waitEndingJamPhase3E(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    if (gEndingCreditsSequencePhase == 0x3E) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamPhase3FAnim1);
        setMainMenuSceneModelAnimation(2, 0x64);
    }
}

void updateEndingJamSetPhase3D(EndingCreditsJam *arg0) {
    u16 *p;
    loopMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    p = &arg0->timer;
    (*p)++;
    if (*p == 0x32) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingJamPhase3E);
        gEndingCreditsSequencePhase = 0x3D;
    }
}

void updateEndingJamPhase3DPrep(EndingCreditsJam *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    if (sp18 == 1) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamSetPhase3D);
        setMainMenuSceneModelAnimation(2, 0x63);
    }
}

void updateEndingJamWaitBeforePhase3D(EndingCreditsJam *arg0) {
    if (stepMainMenuSceneModelAnimation(2) == 1) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamPhase3DPrep);
        setMainMenuSceneModelAnimation(2, 0x62);
    }
    addMainMenuSceneModelDrawCallback(2);
}

void updateEndingJamWaitBeforeIdleAnim2(EndingCreditsJam *arg0) {
    if (stepMainMenuSceneModelAnimation(2) == 1) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamWaitBeforePhase3D);
        setMainMenuSceneModelAnimation(2, 0x23);
    }
    addMainMenuSceneModelDrawCallback(2);
}

void updateEndingJamWaitBeforeIdleAnim1(EndingCreditsJam *arg0) {
    if (stepMainMenuSceneModelAnimation(2) == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0xF) {
            *p = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamWaitBeforeIdleAnim2);
            setMainMenuSceneModelAnimation(2, 0x22);
        }
    }
    addMainMenuSceneModelDrawCallback(2);
}

void updateEndingJamHopRightToIdle(EndingCreditsJam *arg0) {
    s32 unused;
    s32 sp20;
    s32 var_v0;

    sp20 = stepMainMenuSceneModelAnimation(2);
    if (arg0->timer < 5) {
        var_v0 = 1;
    } else {
        var_v0 = -1;
    }
    arg0->posX += 0x5D000;
    arg0->posY += var_v0 * 0x60000;
    arg0->timer = arg0->timer + 1;
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
    if (sp20 == 1) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamWaitBeforeIdleAnim1);
        setMainMenuSceneModelAnimation(2, 0x21);
    }
}

void updateEndingJamSlideLeftSetPhase3B(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX += 0xFFF70000;
    if (arg0->posX < (s32)0xFFA00001) {
        arg0->posX = 0xFFA00000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamHopRightToIdle);
        arg0->rotY = 0xC00;
        setMainMenuSceneModelRotation(2, arg0->rotX, arg0->rotY, arg0->rotZ);
        setMainMenuSceneModelAnimation(2, 0x20);
        gEndingCreditsSequencePhase = 0x3B;
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_JAM] = 1;
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
}

void waitEndingJamPhase3A(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    if (gEndingCreditsSequencePhase == 0x3A) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamSlideLeftSetPhase3B);
    }
}

void updateEndingJamSlideRightToCenter(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX += 0x24000;
    if (arg0->posX >= 0x100000) {
        arg0->posX = 0x100000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingJamPhase3A);
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
}

void waitEndingJamPhase38(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    if (gEndingCreditsSequencePhase == 0x38) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamSlideRightToCenter);
    }
}

void updateEndingJamSlideLeftFromFarRight(EndingCreditsJam *arg0) {
    s32 var_a1;
    EndingCreditsJam *new_var;

    loopMainMenuSceneModelAnimation(2);
    new_var = arg0;
    var_a1 = (new_var->posX += -0x48000);
    if (var_a1 < -0x7FFFFF) {
        new_var->posX = -0x800000;
        setCallbackTaskCallback(new_var, (CallbackTaskCallback)waitEndingJamPhase38);
        var_a1 = arg0->posX;
    } else if ((var_a1 < 0x1300001) && (gEndingCreditsSequencePhase == 0x33)) {
        gEndingCreditsSequencePhase = 0x34;
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(2, var_a1, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
}

void waitEndingJamPhase33Aura(EndingCreditsJam *arg0) {
    if (gEndingCreditsSequencePhase == 0x33) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0x14) {
            *p = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamSlideLeftFromFarRight);
            arg0->posX = 0x1900000;
            arg0->posZ = 0x120000;
            gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_JAM] = 0;
            spawnEndingCreditsCharacterAura(0x18, -0x36, 2, 1);
        }
    }
}

void updateEndingJamDashOffLeftSetPhase31(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX += 0xFFFB8000;
    if (arg0->posX < (s32)0xFE700001) {
        arg0->posX = 0xFE700000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingJamPhase33Aura);
        gEndingCreditsSequencePhase = 0x31;
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_JAM] = 1;
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
}

void updateEndingJamVanishRetreat(EndingCreditsJam *arg0) {
    s32 unused;
    s32 sp20;

    sp20 = stepMainMenuSceneModelAnimation(2);
    arg0->posX += 0xFFF00000;
    arg0->timer += 1;
    if (arg0->timer < 3) {
        arg0->posY += 0x140000;
    } else {
        arg0->posY += 0xFFEC0000;
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
    if (sp20 == 1) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamDashOffLeftSetPhase31);
        arg0->posY = 0;
        setMainMenuSceneModelPosition(2, arg0->posX, 0, arg0->posZ);
        setMainMenuSceneModelAnimation(2, 0x1B);
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_JAM] = 0;
        spawnEndingCreditsCharacterAura(0x18, -0x36, 2, 1);
    }
}

void waitEndingJamPhase2F(EndingCreditsJam *arg0) {
    addMainMenuSceneModelDrawCallback(2);
    if (gEndingCreditsSequencePhase == 0x2F) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamVanishRetreat);
        setMainMenuSceneModelAnimation(2, 0x4E);
        spawnEndingCreditsCharacterVanishPoof(8, -0x40, 2, 0);
    }
}

void updateEndingJamDriftAfterPhase2D(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX = arg0->posX + 0x2000;
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
    if (gEndingCreditsSequencePhase == 0x2D) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingJamPhase2F);
    }
}

void updateEndingJamWaitThenSetPhase2C(EndingCreditsJam *arg0) {
    u16 *p;
    loopMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    p = &arg0->timer;
    (*p)++;
    if (*p == 0x19) {
        gEndingCreditsSequencePhase = 0x2C;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamDriftAfterPhase2D);
        *p = 0;
    }
}

void updateEndingJamRunLeftThenPhase2C(EndingCreditsJam *arg0) {
    if (stepMainMenuSceneModelAnimation(2) == 0) {
        arg0->posX = arg0->posX + 0xFFF60000;
    } else {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamWaitThenSetPhase2C);
        setMainMenuSceneModelAnimation(2, 0x44);
        arg0->posZ = 0xFFFF0000;
        arg0->timer = 0;
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
}

void updateEndingJamSlideFarLeft(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX += 0xFFFC8000;
    arg0->posZ += 0xFFFF0000;
    if (arg0->posX < (s32)0xFFA80001) {
        arg0->posX = 0xFF780000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamRunLeftThenPhase2C);
        setMainMenuSceneModelAnimation(2, 0x43);
        arg0->timer = 0;
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
}

void waitEndingJamPhase2A(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    if (gEndingCreditsSequencePhase == 0x2A) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamSlideFarLeft);
        arg0->posZ = 0xFFFC0000;
    }
}

void updateEndingJamSlideRightToPose(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX += 0x2D000;
    if (arg0->posX >= 0xF00000) {
        arg0->posX = 0xF00000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingJamPhase2A);
        gEndingCreditsSequencePhase = 0x29;
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_JAM] = 1;
        spawnEndingCreditsCharacterLoopingSparkle(-0x10, -0x4E, 2);
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
}

void waitEndingJamPhase28(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    if (gEndingCreditsSequencePhase == 0x28) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamSlideRightToPose);
    }
}

void updateEndingJamSlideRightToMarker(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX += 0x2E000;
    if (arg0->posX >= (s32)0xFFD00000) {
        arg0->posX = 0xFFD00000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingJamPhase28);
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
}

void waitEndingJamBeforeMarkerSlide(EndingCreditsJam *arg0) {
    u16 *p;
    loopMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    p = &arg0->timer;
    (*p)++;
    if (*p == 0xF) {
        *p = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamSlideRightToMarker);
    }
}

void updateEndingJamSlideLeftToMarker(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX += 0xFFFD2000;
    if (arg0->posX < (s32)0xFF800001) {
        arg0->posX = 0xFF800000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingJamBeforeMarkerSlide);
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
}

void updateEndingJamWaitThenSetPhase27(EndingCreditsJam *arg0) {
    u16 *p;
    loopMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    p = &arg0->timer;
    (*p)++;
    if (*p == 0x19) {
        *p = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamSlideLeftToMarker);
        gEndingCreditsSequencePhase = 0x27;
    }
}

void updateEndingJamSlideRightToPhase27Start(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX += 0x20000;
    if (arg0->posX >= (s32)0xFFD80000) {
        arg0->posX = 0xFFD80000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamWaitThenSetPhase27);
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
}

void startEndingJamPhase26Slide(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    gEndingCreditsSequencePhase = 0x26;
    if (gEndingCreditsSequencePhase != 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamSlideRightToPhase27Start);
    }
}

void updateEndingJamSlideLeftSetPhase25(EndingCreditsJam *arg0) {
    s32 *temp_a2;
    s32 var_a1;

    loopMainMenuSceneModelAnimation(2);
    var_a1 = (arg0->posX += 0xFFFD4000);
    if (var_a1 < (s32)0xFF550001) {
        arg0->posX = 0xFF550000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)startEndingJamPhase26Slide);
        gEndingCreditsSequencePhase = 0x25;
        var_a1 = arg0->posX;
    }
    temp_a2 = &arg0->posY;
    setMainMenuSceneModelPosition(2, var_a1, *temp_a2, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
}

void waitEndingJamBeforeDiagonalSlide(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    if (gEndingCreditsSequencePhase == 0x24) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0x12) {
            *p = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamSlideLeftSetPhase25);
        }
    }
}

void updateEndingJamDiagonalSlideSetPhase22(EndingCreditsJam *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(2);
    var_a1 = (arg0->posX -= -0x28000);
    if (var_a1 >= (s32)0xFFD00000) {
        arg0->posX = 0xFFD00000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingJamBeforeDiagonalSlide);
        var_a1 = arg0->posX;
    }
    if ((var_a1 >= (s32)0xFFA00000) && (gEndingCreditsSequencePhase == 0x21)) {
        gEndingCreditsSequencePhase = 0x22;
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(2, var_a1, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
}

void waitEndingJamPhase21Aura(EndingCreditsJam *arg0) {
    if (gEndingCreditsSequencePhase == 0x21) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamDiagonalSlideSetPhase22);
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_JAM] = 0;
        spawnEndingCreditsCharacterAura(0x18, -0x36, 2, 1);
    }
}

void updateEndingJamExitLeftAfterPhase21(EndingCreditsJam *arg0) {
    s32 *temp_a2;
    s32 var_a1;

    loopMainMenuSceneModelAnimation(2);
    var_a1 = (arg0->posX += 0xFFFA0000);
    if (var_a1 < (s32)0xFE700001) {
        arg0->posX = 0xFE700000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingJamPhase21Aura);
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_JAM] = 1;
        var_a1 = arg0->posX;
    }
    temp_a2 = &arg0->posY;
    setMainMenuSceneModelPosition(2, var_a1, *temp_a2, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
}

void updateEndingJamAfterVanishWait(EndingCreditsJam *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(2);
    var_a1 = (arg0->posX += 0xFFFB0000);
    if (var_a1 < (s32)0xFF600001) {
        arg0->posX = 0xFF600000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamExitLeftAfterPhase21);
        setMainMenuSceneModelAnimation(2, 0x1B);
        arg0->rotY = 0xC00;
        setMainMenuSceneModelRotation(2, arg0->rotX, arg0->rotY, arg0->rotZ);
        gEndingCreditsSequencePhase = 0x20;
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_JAM] = 0;
        spawnEndingCreditsCharacterAura(0x18, -0x36, 2, 1);
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(2, var_a1, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
}

void updateEndingJamWaitBeforeVanishReturn(EndingCreditsJam *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    if (sp18 == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0x14) {
            *p = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamAfterVanishWait);
            setMainMenuSceneModelAnimation(2, 0x48);
        }
    }
}

void startEndingJamVanishReturn(EndingCreditsJam *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    if (sp18 == 1) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamWaitBeforeVanishReturn);
        setMainMenuSceneModelAnimation(2, 0x47);
        spawnEndingCreditsCharacterVanishPoof(5, -0x46, 2, 0);
    }
}

void updateEndingJamSlideLeftThenVanish(EndingCreditsJam *arg0) {
    if (stepMainMenuSceneModelAnimation(2) == 0) {
        arg0->posX += 0xFFFE0000;
        setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    } else {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0xF) {
            *p = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)startEndingJamVanishReturn);
            setMainMenuSceneModelAnimation(2, 0x46);
        }
    }
    addMainMenuSceneModelDrawCallback(2);
}

void waitEndingJamPhase1E(EndingCreditsJam *arg0) {
    addMainMenuSceneModelDrawCallback(2);
    if (gEndingCreditsSequencePhase == 0x1E) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamSlideLeftThenVanish);
        setMainMenuSceneModelAnimation(2, 0x45);
        spawnEndingCreditsSmallBurst(0x2A, -0x57);
    }
}

void updateEndingJamExitLeftSetPhase1D(EndingCreditsJam *arg0) {
    s32 sp24;
    s32 sp20;

    sp20 = stepMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    if (sp20 == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 5) {
            *p = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingJamPhase1E);
            gEndingCreditsSequencePhase = 0x1D;
        }
    } else {
        arg0->posX += 0xFFFB8000;
        setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    }
}

void updateEndingJamRepeatAnimThenVanish(EndingCreditsJam *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    if (sp18 == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0x14) {
            *p = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamExitLeftSetPhase1D);
            setMainMenuSceneModelAnimation(2, 0x35);
            spawnEndingCreditsCharacterVanishPoof(8, -0x40, 2, 0);
        }
    }
}

void waitEndingJamPhase1C(EndingCreditsJam *arg0) {
    stepMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    if (gEndingCreditsSequencePhase == 0x1C) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamRepeatAnimThenVanish);
        setMainMenuSceneModelAnimation(2, 0x34);
    }
}

void updateEndingJamWaitBeforePhase1C(EndingCreditsJam *arg0) {
    u16 *p;
    addMainMenuSceneModelDrawCallback(2);
    p = &arg0->timer;
    (*p)++;
    if (*p == 0x1E) {
        *p = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingJamPhase1C);
        setMainMenuSceneModelAnimation(2, 0x33);
        arg0->posZ = 0xFFFF0000;
        setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, 0xFFFF0000);
    }
}

void updateEndingJamStepBackAfterHandshake(EndingCreditsJam *arg0) {
    if (stepMainMenuSceneModelAnimation(2) == 1) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamWaitBeforePhase1C);
    } else {
        arg0->posZ = arg0->posZ + 0xFFFC0000;
        setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    }
    addMainMenuSceneModelDrawCallback(2);
}

void waitEndingJamHandshakeExit(EndingCreditsJam *arg0) {
    addMainMenuSceneModelDrawCallback(2);
    if (gEndingCreditsHandshakeState == 5) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamStepBackAfterHandshake);
        setMainMenuSceneModelAnimation(2, 0x32);
        arg0->rotY = 0x400;
        setMainMenuSceneModelRotation(2, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
}

void updateEndingJamHandshakeLoopThird(EndingCreditsJam *arg0) {
    if (gEndingCreditsHandshakeState == 2) {
        stepMainMenuSceneModelAnimation(2);
        if (stepMainMenuSceneModelAnimation(2) == 1) {
            u16 *p;
            gEndingCreditsHandshakeState++;
            setMainMenuSceneModelAnimation(2, 0x2B);
            p = &arg0->timer;
            (*p)++;
            if (*p == 0x4) {
                setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingJamHandshakeExit);
                *p = 0;
            }
        }
    }
    addMainMenuSceneModelDrawCallback(2);
}

void updateEndingJamHandshakeLoopSecond(EndingCreditsJam *arg0) {
    if (gEndingCreditsHandshakeState == 2) {
        stepMainMenuSceneModelAnimation(2);
        if (stepMainMenuSceneModelAnimation(2) == 1) {
            u16 *p;
            gEndingCreditsHandshakeState++;
            setMainMenuSceneModelAnimation(2, 0x2B);
            p = &arg0->timer;
            (*p)++;
            if (*p == 0x5) {
                setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamHandshakeLoopThird);
                *p = 0;
            }
        }
    }
    addMainMenuSceneModelDrawCallback(2);
}

void updateEndingJamHandshakeLoopFirst(EndingCreditsJam *arg0) {
    if (gEndingCreditsHandshakeState == 2) {
        if (arg0->rotY == 0xC01) {
            arg0->rotY = 0xC00;
            setMainMenuSceneModelRotation(2, arg0->rotX, arg0->rotY, arg0->rotZ);
        }
        if (stepMainMenuSceneModelAnimation(2) == 1) {
            u16 *p;
            gEndingCreditsHandshakeState++;
            setMainMenuSceneModelAnimation(2, 0x2B);
            p = &arg0->timer;
            (*p)++;
            if (*p == 0x3) {
                setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamHandshakeLoopSecond);
                *p = 0;
            }
        }
    }
    addMainMenuSceneModelDrawCallback(2);
}

void startEndingJamHandshakeLoop(EndingCreditsJam *arg0) {
    if (gEndingCreditsHandshakeState == 2) {
        if (stepMainMenuSceneModelAnimation(2) == 1) {
            gEndingCreditsHandshakeState++;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamHandshakeLoopFirst);
            setMainMenuSceneModelAnimation(2, 0x2B);
            arg0->rotY = 0xC01;
            arg0->timer = 0;
        }
    }
    addMainMenuSceneModelDrawCallback(2);
}

void updateEndingJamSlideRightUntilPhase18(EndingCreditsJam *arg0) {
    stepMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    arg0->posX = arg0->posX + 0x30000;
    if (gEndingCreditsSequencePhase == 0x18) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)startEndingJamHandshakeLoop);
        setMainMenuSceneModelAnimation(2, 0x31);
        arg0->rotY = 0x400;
        setMainMenuSceneModelRotation(2, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
}

void waitEndingJamPhase17(EndingCreditsJam *arg0) {
    u16 val;
    loopMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    val = arg0->timer;
    if (val < 0x2D) {
        arg0->timer = val + 1;
        if (arg0->timer == 0x2D) {
            gEndingCreditsSequencePhase = 0x16;
        }
    } else if (gEndingCreditsSequencePhase == 0x17) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamSlideRightUntilPhase18);
        setMainMenuSceneModelAnimation(2, 0x30);
        arg0->rotY = 0x400;
        setMainMenuSceneModelRotation(2, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
}

void updateEndingJamSlideLeftToIdle(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX = arg0->posX + 0xFFFD0000;
    if (arg0->posX < 0x4E0001) {
        arg0->posX = 0x4E0000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingJamPhase17);
        setMainMenuSceneModelAnimation(2, 0x26);
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
}

void startEndingJamIdleAfterAnim(EndingCreditsJam *arg0) {
    if (stepMainMenuSceneModelAnimation(2) == 1) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamSlideLeftToIdle);
        setMainMenuSceneModelAnimation(2, 0);
    }
    addMainMenuSceneModelDrawCallback(2);
}

void updateEndingJamWaitBeforeIdleAnimB(EndingCreditsJam *arg0) {
    if (stepMainMenuSceneModelAnimation(2) == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0xF) {
            *p = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)startEndingJamIdleAfterAnim);
            setMainMenuSceneModelAnimation(2, 0x23);
        }
    }
    addMainMenuSceneModelDrawCallback(2);
}

void updateEndingJamWaitBeforeIdleAnimA(EndingCreditsJam *arg0) {
    if (stepMainMenuSceneModelAnimation(2) == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0xF) {
            *p = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamWaitBeforeIdleAnimB);
            setMainMenuSceneModelAnimation(2, 0x22);
        }
    }
    addMainMenuSceneModelDrawCallback(2);
}

void updateEndingJamHopRightToPose(EndingCreditsJam *arg0) {
    s32 unused;
    s32 sp20;
    s32 var_v0;

    sp20 = stepMainMenuSceneModelAnimation(2);
    if (arg0->timer < 5) {
        var_v0 = 1;
    } else {
        var_v0 = -1;
    }
    arg0->posX += 0x50000;
    arg0->posY += var_v0 * 0x60000;
    arg0->timer = arg0->timer + 1;
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(2);
    if (sp20 == 1) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamWaitBeforeIdleAnimA);
        setMainMenuSceneModelAnimation(2, 0x21);
    }
}

void waitEndingJamPhase15(EndingCreditsJam *arg0) {
    loopMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
    if (gEndingCreditsSequencePhase == 0x15) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamHopRightToPose);
        arg0->rotY = 0xC00;
        setMainMenuSceneModelRotation(2, arg0->rotX, arg0->rotY, arg0->rotZ);
        setMainMenuSceneModelAnimation(2, 0x20);
    }
}

void updateEndingJamSlideLeftToPhase15Wait(EndingCreditsJam *arg0) {
    arg0->posX = arg0->posX + 0xFFFB8000;
    if (arg0->posX < 0x500001) {
        arg0->posX = 0x500000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingJamPhase15);
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    loopMainMenuSceneModelAnimation(2);
    addMainMenuSceneModelDrawCallback(2);
}

void waitEndingJamPhase13(EndingCreditsJam *arg0) {
    if (gEndingCreditsSequencePhase == 0x13) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingJamSlideLeftToPhase15Wait);
        spawnEndingCreditsTumblingSnowboard(arg0->posX + 0x48000, 0x480000, 0xFFF30000, 2, 2, 1);
    }
}

void initEndingCreditsJam(EndingCreditsJam *arg0) {
    arg0->posX = 0x3248000;
    arg0->posY = 0;
    arg0->posZ = 0;
    arg0->rotX = 0;
    arg0->rotY = 0x400;
    arg0->rotZ = 0;
    initMainMenuSceneModel(2, 2);
    setMainMenuSceneModelAnimation(2, 0x25);
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    setMainMenuSceneModelRotation(2, arg0->rotX, arg0->rotY, arg0->rotZ);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingJamPhase13);
}
