#include "common.h"
#include "callback_task_scheduler.h"
#include "menu_transition_effects.h"
#include "ending_credits_character_1.h"
#include "main_menu_scene_model.h"
#include "main_menu_scene_renderer.h"

struct EndingCreditsCharacter1 {
    /* 0x00 */ char pad0[0x18];
    /* 0x18 */ s32 posX;
    /* 0x1C */ s32 posY;
    /* 0x20 */ s32 posZ;
    /* 0x24 */ s16 rotX;
    /* 0x26 */ s16 rotY;
    /* 0x28 */ s16 rotZ;
    /* 0x2A */ u16 timer;
};

extern u16 gEndingSequencePhase;
extern s8 gEndingCharacterEffectDone;
extern s8 gEndingCharacter1EffectDone;

void updateEndingCharacter1FinalPose(EndingCreditsCharacter1 *arg0) {
    stepMainMenuSceneModelAnimation(1);
    func_800428C8(1);
}

void updateEndingCharacter1StartFinalPose(EndingCreditsCharacter1 *arg0) {
    stepMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (gEndingSequencePhase == 0x41) {
        setCallbackTaskCallback(arg0, updateEndingCharacter1FinalPose);
        setMainMenuSceneModelAnimation(1, 0x5B);
        arg0->rotY = 0xC00;
        setMainMenuSceneModelRotation(1, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
}

void waitEndingCharacter1Phase40(EndingCreditsCharacter1 *arg0) {
    stepMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (gEndingSequencePhase == 0x40) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingCharacter1StartFinalPose);
        setMainMenuSceneModelAnimation(1, 0x5A);
    }
}

void updateEndingCharacter1Phase40Prep(EndingCreditsCharacter1 *arg0) {
    s32 unused;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (sp18 == 1) {
        if (++arg0->timer == 0x1A) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, waitEndingCharacter1Phase40);
            setMainMenuSceneModelAnimation(1, 0x59);
            spawnEndingPhaseAdvanceSparkle(0x53, -0x64);
        }
    }
}

void updateEndingCharacter1WaitBeforePhase40Prep(EndingCreditsCharacter1 *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (sp18 == 1) {
        if (++arg0->timer == 0x14) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, updateEndingCharacter1Phase40Prep);
            setMainMenuSceneModelAnimation(1, 0x58);
        }
    }
}

void updateEndingCharacter1SpinUntilPhase3F(EndingCreditsCharacter1 *arg0) {
    s32 unused;
    s32 sp20;
    s32 temp_a1;

    sp20 = stepMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (sp20 == 0) {
        temp_a1 = (arg0->posX += 0x18000);
        setMainMenuSceneModelPosition(1, temp_a1, arg0->posY, arg0->posZ);
    }
    if (gEndingSequencePhase == 0x3F) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingCharacter1WaitBeforePhase40Prep);
        setMainMenuSceneModelAnimation(1, 0x57);
    }
}

void updateEndingCharacter1TumbleToPhase3C(EndingCreditsCharacter1 *arg0) {
    s32 unused;
    s32 sp20;
    s32 var_v0;

    sp20 = stepMainMenuSceneModelAnimation(1);
    if (arg0->timer < 5) {
        var_v0 = 1;
    } else {
        var_v0 = -1;
    }
    arg0->posX += 0x76000;
    arg0->posY += var_v0 << 19;
    arg0->timer = arg0->timer + 1;
    setMainMenuSceneModelPosition(1, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(1);
    if (sp20 == 1) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingCharacter1SpinUntilPhase3F);
        setMainMenuSceneModelAnimation(1, 0x5D);
    }
}

void updateEndingCharacter1ExitUntilPhase3C(EndingCreditsCharacter1 *arg0) {
    loopMainMenuSceneModelAnimation(1);
    arg0->posX += 0xFFF58000;
    if (gEndingSequencePhase == 0x3C) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingCharacter1TumbleToPhase3C);
        setMainMenuSceneModelAnimation(1, 0x5C);
        gEndingCharacter1EffectDone = 1;
    }
    setMainMenuSceneModelPosition(1, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(1);
}

void waitEndingCharacter1Phase3A(EndingCreditsCharacter1 *arg0) {
    loopMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (gEndingSequencePhase == 0x3A) {
        setCallbackTaskCallback(arg0, updateEndingCharacter1ExitUntilPhase3C);
    }
}

void updateEndingCharacter1SlideToCenter(EndingCreditsCharacter1 *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(1);
    arg0->posX += 0x24000;
    var_a1 = arg0->posX;
    if (var_a1 >= 0x1200000) {
        arg0->posX = 0x1200000;
        setCallbackTaskCallback(arg0, waitEndingCharacter1Phase3A);
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(1, var_a1, arg0->posY, arg0->posZ);
    func_800428C8(1);
}

void waitEndingCharacter1Phase38(EndingCreditsCharacter1 *arg0) {
    loopMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (gEndingSequencePhase == 0x38) {
        setCallbackTaskCallback(arg0, updateEndingCharacter1SlideToCenter);
    }
}

void updateEndingCharacter1SlideLeftSetPhase37(EndingCreditsCharacter1 *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(1);
    arg0->posX += 0xFFFB8000;
    var_a1 = arg0->posX;
    if (var_a1 < 0x01000001) {
        arg0->posX = 0x01000000;
        setCallbackTaskCallback(arg0, waitEndingCharacter1Phase38);
        gEndingSequencePhase = 0x37;
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(1, var_a1, arg0->posY, arg0->posZ);
    func_800428C8(1);
}

void waitEndingCharacter1Phase36(EndingCreditsCharacter1 *arg0) {
    if (gEndingSequencePhase == 0x36) {
        setCallbackTaskCallback(arg0, updateEndingCharacter1SlideLeftSetPhase37);
        arg0->posX = 0x1900000;
    }
}

void updateEndingCharacter1SlideLeftSetPhase33(EndingCreditsCharacter1 *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(1);
    arg0->posX += 0xFFFB8000;
    var_a1 = arg0->posX;
    if (var_a1 < (s32)0xFE700001) {
        arg0->posX = -0x1900000;
        setCallbackTaskCallback(arg0, waitEndingCharacter1Phase36);
        gEndingSequencePhase = 0x33;
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(1, var_a1, arg0->posY, arg0->posZ);
    func_800428C8(1);
}

void waitEndingCharacter1Phase32Aura(EndingCreditsCharacter1 *arg0) {
    if (stepMainMenuSceneModelAnimation(1) == 0) {
        arg0->posZ = 0;
    }
    setMainMenuSceneModelPosition(1, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(1);
    if (gEndingSequencePhase == 0x32) {
        setCallbackTaskCallback(arg0, updateEndingCharacter1SlideLeftSetPhase33);
        setMainMenuSceneModelAnimation(1, 0x52);
        arg0->rotY = 0xC00;
        setMainMenuSceneModelRotation(1, arg0->rotX, arg0->rotY, arg0->rotZ);
        gEndingCharacterEffectDone = 0;
        spawnEndingCharacterAura(0x13, -0x3A, 1, 1);
    }
}

void waitEndingCharacter1Phase30Vanish(EndingCreditsCharacter1 *arg0) {
    func_800428C8(1);
    if (gEndingSequencePhase == 0x30) {
        setCallbackTaskCallback(arg0, waitEndingCharacter1Phase32Aura);
        setMainMenuSceneModelAnimation(1, 0x51);
        spawnEndingCharacterVanishPoof(6, -0x40, 1, 0);
    }
}

void updateEndingCharacter1DashRightSetPhase2F(EndingCreditsCharacter1 *arg0) {
    if (stepMainMenuSceneModelAnimation(1) == 1) {
        arg0->timer++;
        if (arg0->timer == 0xA) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, waitEndingCharacter1Phase30Vanish);
            gEndingSequencePhase = 0x2F;
        }
    } else {
        arg0->posX += 0x20000;
    }
    setMainMenuSceneModelPosition(1, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(1);
}

void waitEndingCharacter1Phase2E(EndingCreditsCharacter1 *arg0) {
    func_800428C8(1);
    if (gEndingSequencePhase == 0x2E) {
        if (++arg0->timer == 0x14) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, updateEndingCharacter1DashRightSetPhase2F);
            setMainMenuSceneModelAnimation(1, 0x50);
        }
    }
}

void updateEndingCharacter1WaitPhase2EAnim(EndingCreditsCharacter1 *arg0) {
    s32 sp1c;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    sp1c = (sp18 == 1);
    if (sp1c) {
        setCallbackTaskCallback(arg0, waitEndingCharacter1Phase2E);
        arg0->timer = 0;
    }
}

void updateEndingCharacter1DriftAfterPhase2D(EndingCreditsCharacter1 *arg0) {
    s32 temp_a1;

    stepMainMenuSceneModelAnimation(1);
    temp_a1 = (arg0->posX += 0x2800);
    if (1) {
        setMainMenuSceneModelPosition(1, temp_a1, arg0->posY, arg0->posZ);
        func_800428C8(1);
    }
    if (gEndingSequencePhase == 0x2D) {
        setCallbackTaskCallback(arg0, updateEndingCharacter1WaitPhase2EAnim);
        setMainMenuSceneModelAnimation(1, 0x4F);
        spawnEndingCharacterVanishPoof(8, -0x40, 1, 0);
    }
}

void waitEndingCharacter1Phase2B(EndingCreditsCharacter1 *arg0) {
    loopMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (gEndingSequencePhase == 0x2B) {
        setCallbackTaskCallback(arg0, updateEndingCharacter1DriftAfterPhase2D);
        setMainMenuSceneModelAnimation(1, 0x4D);
        createCallbackTask(&initEndingSpeedLines, 0, 0x64);
    }
}

void updateEndingCharacter1RunRightThenPhase2A(EndingCreditsCharacter1 *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(1);
    arg0->posX += 0x120000;
    var_a1 = arg0->posX;
    if (var_a1 >= -0x600000) {
        arg0->posX = -0x600000;
        setCallbackTaskCallback(arg0, waitEndingCharacter1Phase2B);
        gEndingSequencePhase = 0x2A;
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(1, var_a1, arg0->posY, arg0->posZ);
    func_800428C8(1);
}

void waitEndingCharacter1Phase29(EndingCreditsCharacter1 *arg0) {
    if (gEndingSequencePhase == 0x29) {
        setCallbackTaskCallback(arg0, updateEndingCharacter1RunRightThenPhase2A);
        setMainMenuSceneModelAnimation(1, 0x4C);
        arg0->posZ = 0xA0000;
    }
}

void updateEndingCharacter1SlideFarLeftSetPhase12(EndingCreditsCharacter1 *arg0) {
    arg0->posX += 0xFFFE0000;
    setMainMenuSceneModelPosition(1, arg0->posX, arg0->posY, arg0->posZ);
    if (arg0->posX < -0x18FFFFF) {
        gEndingSequencePhase = 0x12;
        arg0->posX = -0x1900000;
        setCallbackTaskCallback(arg0, waitEndingCharacter1Phase29);
    }
    loopMainMenuSceneModelAnimation(1);
    func_800428C8(1);
}

void updateEndingCharacter1WaitBeforeSlideFarLeft(EndingCreditsCharacter1 *arg0) {
    loopMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (++arg0->timer == 0x32) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingCharacter1SlideFarLeftSetPhase12);
    }
}

void updateEndingCharacter1SlideRightToPose(EndingCreditsCharacter1 *arg0) {
    loopMainMenuSceneModelAnimation(1);
    arg0->posX += 0x60000;
    setMainMenuSceneModelPosition(1, arg0->posX, arg0->posY, arg0->posZ);
    if (arg0->posX >= -0x580000) {
        arg0->posX = -0x580000;
        setCallbackTaskCallback(arg0, updateEndingCharacter1WaitBeforeSlideFarLeft);
    }
    func_800428C8(1);
}

void waitEndingCharacter1Phase11(EndingCreditsCharacter1 *arg0) {
    if (gEndingSequencePhase == 0x11) {
        arg0->posX = 0xFE700000;
        arg0->rotY = 0x400;
        setMainMenuSceneModelPosition(1, -0x1900000, arg0->posY, arg0->posZ);
        setMainMenuSceneModelRotation(1, arg0->rotX, arg0->rotY, arg0->rotZ);
        setCallbackTaskCallback(arg0, updateEndingCharacter1SlideRightToPose);
        setMainMenuSceneModelAnimation(1, 0x1F);
    }
}

void updateEndingCharacter1ExitRightSetPhase0F(EndingCreditsCharacter1 *arg0) {
    loopMainMenuSceneModelAnimation(1);
    arg0->posX += 0x60000;
    setMainMenuSceneModelPosition(1, arg0->posX, arg0->posY, arg0->posZ);
    if (arg0->posX >= 0x1900000) {
        arg0->posX = 0x1900000;
        setCallbackTaskCallback(arg0, waitEndingCharacter1Phase11);
        gEndingSequencePhase = 0xF;
    }
    func_800428C8(1);
}

void waitEndingCharacter1Phase0E(EndingCreditsCharacter1 *arg0) {
    loopMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (gEndingSequencePhase == 0xE) {
        setCallbackTaskCallback(arg0, updateEndingCharacter1ExitRightSetPhase0F);
        setMainMenuSceneModelAnimation(1, 0x1F);
    }
}

void updateEndingCharacter1StartRunDust(EndingCreditsCharacter1 *arg0) {
    if (stepMainMenuSceneModelAnimation(1) == 1) {
        if (++arg0->timer == 0xC) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, waitEndingCharacter1Phase0E);
            setMainMenuSceneModelAnimation(1, 0x15);
            createCallbackTask(&initEndingRunDust, 0, 0x64);
        }
    }
    func_800428C8(1);
}

void updateEndingCharacter1RepeatAnimThenVanish(EndingCreditsCharacter1 *arg0) {
    if (stepMainMenuSceneModelAnimation(1) == 1) {
        arg0->timer += 1;
        if (arg0->timer < 3) {
            setMainMenuSceneModelAnimation(1, 0x13);
        }
        if (arg0->timer == 3) {
            arg0->timer = 0;
            gEndingSequencePhase = 0xD;
            setCallbackTaskCallback(arg0, updateEndingCharacter1StartRunDust);
            setMainMenuSceneModelAnimation(1, 0x14);
            spawnEndingCharacterVanishPoof(8, -0x40, 1, 0);
        }
    }
    func_800428C8(1);
}

void startEndingCharacter1RepeatAnim(EndingCreditsCharacter1 *arg0) {
    if (stepMainMenuSceneModelAnimation(1) == 1) {
        setCallbackTaskCallback(arg0, updateEndingCharacter1RepeatAnimThenVanish);
        setMainMenuSceneModelAnimation(1, 0x13);
    }
    func_800428C8(1);
}

void updateEndingCharacter1WaitBeforeRepeatAnim(EndingCreditsCharacter1 *arg0) {
    stepMainMenuSceneModelAnimation(1);
    arg0->timer += 1;
    if (arg0->timer == 0x46) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, startEndingCharacter1RepeatAnim);
        setMainMenuSceneModelAnimation(1, 0x12);
        spawnEndingDelayedSparkle(-0xB, -0x58, 1);
    }
    func_800428C8(1);
}

void waitEndingCharacter1Phase0C(EndingCreditsCharacter1 *arg0) {
    s32 temp_a3;

    if (stepMainMenuSceneModelAnimation(1) == 0) {
        temp_a3 = (arg0->posZ += (s32)0xFFFA0000);
        setMainMenuSceneModelPosition(1, arg0->posX, arg0->posY, temp_a3);
    }
    if (gEndingSequencePhase == 0xC) {
        setCallbackTaskCallback(arg0, updateEndingCharacter1WaitBeforeRepeatAnim);
        setMainMenuSceneModelAnimation(1, 0x11);
    }
    func_800428C8(1);
}

void updateEndingCharacter1EnterUntilPhase0B(EndingCreditsCharacter1 *arg0) {
    arg0->posX += 0x39435;
    if (gEndingSequencePhase == 0xB) {
        setCallbackTaskCallback(arg0, waitEndingCharacter1Phase0C);
        setMainMenuSceneModelAnimation(1, 0x10);
        spawnEndingCharacterVanishPoof(0xA, -0x40, 1, 0);
    } else {
        setMainMenuSceneModelPosition(1, arg0->posX, arg0->posY, arg0->posZ);
        loopMainMenuSceneModelAnimation(1);
    }
    func_800428C8(1);
}

void waitEndingCharacter1Phase9(EndingCreditsCharacter1 *arg0) {
    if (gEndingSequencePhase == 9) {
        setCallbackTaskCallback(arg0, updateEndingCharacter1EnterUntilPhase0B);
    }
}

void initEndingCreditsCharacter1(EndingCreditsCharacter1 *arg0) {
    arg0->posX = 0xFE700000;
    arg0->posY = 0;
    arg0->posZ = 0;
    arg0->rotX = 0;
    arg0->rotY = 0x400;
    arg0->rotZ = 0;
    initMainMenuSceneModel(1, 1);
    setMainMenuSceneModelAnimation(1, 0xF);
    setMainMenuSceneModelPosition(1, arg0->posX, arg0->posY, arg0->posZ);
    setMainMenuSceneModelRotation(1, arg0->rotX, arg0->rotY, arg0->rotZ);
    setCallbackTaskCallback(arg0, waitEndingCharacter1Phase9);
}
