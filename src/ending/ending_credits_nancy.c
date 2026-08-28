#include "common.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/ending/ending_credits_effects.h"
#include "game/ending/ending_credits_flow.h"
#include "game/ending/ending_credits_nancy.h"
#include "game/menu/main_menu/main_menu_scene_model.h"
#include "game/menu/main_menu/main_menu_scene_model_renderer.h"

void updateEndingNancyFinalPose(EndingCreditsNancyActor *arg0) {
    stepMainMenuSceneModelAnimation(1);
    addMainMenuSceneModelDrawCallback(1);
}

void updateEndingNancyStartFinalPose(EndingCreditsNancyActor *arg0) {
    stepMainMenuSceneModelAnimation(1);
    addMainMenuSceneModelDrawCallback(1);
    if (gEndingCreditsSequencePhase == 0x41) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancyFinalPose);
        setMainMenuSceneModelAnimation(1, 0x5B);
        arg0->rot.y = 0xC00;
        setMainMenuSceneModelRotation(1, arg0->rot.x, arg0->rot.y, arg0->rot.z);
    }
}

void waitEndingNancyPhase40(EndingCreditsNancyActor *arg0) {
    stepMainMenuSceneModelAnimation(1);
    addMainMenuSceneModelDrawCallback(1);
    if (gEndingCreditsSequencePhase == 0x40) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancyStartFinalPose);
        setMainMenuSceneModelAnimation(1, 0x5A);
    }
}

void updateEndingNancyPhase40Prep(EndingCreditsNancyActor *arg0) {
    s32 unused;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(1);
    addMainMenuSceneModelDrawCallback(1);
    if (sp18 == 1) {
        if (++arg0->timer == 0x1A) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingNancyPhase40);
            setMainMenuSceneModelAnimation(1, 0x59);
            spawnEndingCreditsPhaseAdvanceSparkle(0x53, -0x64);
        }
    }
}

void updateEndingNancyWaitBeforePhase40Prep(EndingCreditsNancyActor *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(1);
    addMainMenuSceneModelDrawCallback(1);
    if (sp18 == 1) {
        if (++arg0->timer == 0x14) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancyPhase40Prep);
            setMainMenuSceneModelAnimation(1, 0x58);
        }
    }
}

void updateEndingNancySpinUntilPhase3F(EndingCreditsNancyActor *arg0) {
    s32 unused;
    s32 sp20;
    s32 temp_a1;

    sp20 = stepMainMenuSceneModelAnimation(1);
    addMainMenuSceneModelDrawCallback(1);
    if (sp20 == 0) {
        temp_a1 = (arg0->pos.x += 0x18000);
        setMainMenuSceneModelPosition(1, temp_a1, arg0->pos.y, arg0->pos.z);
    }
    if (gEndingCreditsSequencePhase == 0x3F) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancyWaitBeforePhase40Prep);
        setMainMenuSceneModelAnimation(1, 0x57);
    }
}

void updateEndingNancyTumbleToPhase3C(EndingCreditsNancyActor *arg0) {
    s32 unused;
    s32 sp20;
    s32 var_v0;

    sp20 = stepMainMenuSceneModelAnimation(1);
    if (arg0->timer < 5) {
        var_v0 = 1;
    } else {
        var_v0 = -1;
    }
    arg0->pos.x += 0x76000;
    arg0->pos.y += var_v0 << 19;
    arg0->timer = arg0->timer + 1;
    setMainMenuSceneModelPosition(1, arg0->pos.x, arg0->pos.y, arg0->pos.z);
    addMainMenuSceneModelDrawCallback(1);
    if (sp20 == 1) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancySpinUntilPhase3F);
        setMainMenuSceneModelAnimation(1, 0x5D);
    }
}

void updateEndingNancyExitUntilPhase3C(EndingCreditsNancyActor *arg0) {
    loopMainMenuSceneModelAnimation(1);
    arg0->pos.x += 0xFFF58000;
    if (gEndingCreditsSequencePhase == 0x3C) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancyTumbleToPhase3C);
        setMainMenuSceneModelAnimation(1, 0x5C);
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_NANCY] = 1;
    }
    setMainMenuSceneModelPosition(1, arg0->pos.x, arg0->pos.y, arg0->pos.z);
    addMainMenuSceneModelDrawCallback(1);
}

void waitEndingNancyPhase3A(EndingCreditsNancyActor *arg0) {
    loopMainMenuSceneModelAnimation(1);
    addMainMenuSceneModelDrawCallback(1);
    if (gEndingCreditsSequencePhase == 0x3A) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancyExitUntilPhase3C);
    }
}

void updateEndingNancySlideToCenter(EndingCreditsNancyActor *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(1);
    arg0->pos.x += 0x24000;
    var_a1 = arg0->pos.x;
    if (var_a1 >= 0x1200000) {
        arg0->pos.x = 0x1200000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingNancyPhase3A);
        var_a1 = arg0->pos.x;
    }
    setMainMenuSceneModelPosition(1, var_a1, arg0->pos.y, arg0->pos.z);
    addMainMenuSceneModelDrawCallback(1);
}

void waitEndingNancyPhase38(EndingCreditsNancyActor *arg0) {
    loopMainMenuSceneModelAnimation(1);
    addMainMenuSceneModelDrawCallback(1);
    if (gEndingCreditsSequencePhase == 0x38) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancySlideToCenter);
    }
}

void updateEndingNancySlideLeftSetPhase37(EndingCreditsNancyActor *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(1);
    arg0->pos.x += 0xFFFB8000;
    var_a1 = arg0->pos.x;
    if (var_a1 < 0x01000001) {
        arg0->pos.x = 0x01000000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingNancyPhase38);
        gEndingCreditsSequencePhase = 0x37;
        var_a1 = arg0->pos.x;
    }
    setMainMenuSceneModelPosition(1, var_a1, arg0->pos.y, arg0->pos.z);
    addMainMenuSceneModelDrawCallback(1);
}

void waitEndingNancyPhase36(EndingCreditsNancyActor *arg0) {
    if (gEndingCreditsSequencePhase == 0x36) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancySlideLeftSetPhase37);
        arg0->pos.x = 0x1900000;
    }
}

void updateEndingNancySlideLeftSetPhase33(EndingCreditsNancyActor *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(1);
    arg0->pos.x += 0xFFFB8000;
    var_a1 = arg0->pos.x;
    if (var_a1 < (s32)0xFE700001) {
        arg0->pos.x = -0x1900000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingNancyPhase36);
        gEndingCreditsSequencePhase = 0x33;
        var_a1 = arg0->pos.x;
    }
    setMainMenuSceneModelPosition(1, var_a1, arg0->pos.y, arg0->pos.z);
    addMainMenuSceneModelDrawCallback(1);
}

void waitEndingNancyPhase32Aura(EndingCreditsNancyActor *arg0) {
    if (stepMainMenuSceneModelAnimation(1) == 0) {
        arg0->pos.z = 0;
    }
    setMainMenuSceneModelPosition(1, arg0->pos.x, arg0->pos.y, arg0->pos.z);
    addMainMenuSceneModelDrawCallback(1);
    if (gEndingCreditsSequencePhase == 0x32) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancySlideLeftSetPhase33);
        setMainMenuSceneModelAnimation(1, 0x52);
        arg0->rot.y = 0xC00;
        setMainMenuSceneModelRotation(1, arg0->rot.x, arg0->rot.y, arg0->rot.z);
        gEndingCreditsCharacterAuraDoneFlags[ENDING_CREDITS_CHARACTER_SLASH] = 0;
        spawnEndingCreditsCharacterAura(0x13, -0x3A, 1, 1);
    }
}

void waitEndingNancyPhase30Vanish(EndingCreditsNancyActor *arg0) {
    addMainMenuSceneModelDrawCallback(1);
    if (gEndingCreditsSequencePhase == 0x30) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingNancyPhase32Aura);
        setMainMenuSceneModelAnimation(1, 0x51);
        spawnEndingCreditsCharacterVanishPoof(6, -0x40, 1, 0);
    }
}

void updateEndingNancyDashRightSetPhase2F(EndingCreditsNancyActor *arg0) {
    if (stepMainMenuSceneModelAnimation(1) == 1) {
        arg0->timer++;
        if (arg0->timer == 0xA) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingNancyPhase30Vanish);
            gEndingCreditsSequencePhase = 0x2F;
        }
    } else {
        arg0->pos.x += 0x20000;
    }
    setMainMenuSceneModelPosition(1, arg0->pos.x, arg0->pos.y, arg0->pos.z);
    addMainMenuSceneModelDrawCallback(1);
}

void waitEndingNancyPhase2E(EndingCreditsNancyActor *arg0) {
    addMainMenuSceneModelDrawCallback(1);
    if (gEndingCreditsSequencePhase == 0x2E) {
        if (++arg0->timer == 0x14) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancyDashRightSetPhase2F);
            setMainMenuSceneModelAnimation(1, 0x50);
        }
    }
}

void updateEndingNancyWaitPhase2EAnim(EndingCreditsNancyActor *arg0) {
    s32 sp1c;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(1);
    addMainMenuSceneModelDrawCallback(1);
    sp1c = (sp18 == 1);
    if (sp1c) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingNancyPhase2E);
        arg0->timer = 0;
    }
}

void updateEndingNancyDriftAfterPhase2D(EndingCreditsNancyActor *arg0) {
    s32 temp_a1;

    stepMainMenuSceneModelAnimation(1);
    temp_a1 = (arg0->pos.x += 0x2800);
    if (1) {
        setMainMenuSceneModelPosition(1, temp_a1, arg0->pos.y, arg0->pos.z);
        addMainMenuSceneModelDrawCallback(1);
    }
    if (gEndingCreditsSequencePhase == 0x2D) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancyWaitPhase2EAnim);
        setMainMenuSceneModelAnimation(1, 0x4F);
        spawnEndingCreditsCharacterVanishPoof(8, -0x40, 1, 0);
    }
}

void waitEndingNancyPhase2B(EndingCreditsNancyActor *arg0) {
    loopMainMenuSceneModelAnimation(1);
    addMainMenuSceneModelDrawCallback(1);
    if (gEndingCreditsSequencePhase == 0x2B) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancyDriftAfterPhase2D);
        setMainMenuSceneModelAnimation(1, 0x4D);
        createCallbackTask((CallbackTaskCallback)&initEndingCreditsNancySpeedLines, 0, 0x64);
    }
}

void updateEndingNancyRunRightThenPhase2A(EndingCreditsNancyActor *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(1);
    arg0->pos.x += 0x120000;
    var_a1 = arg0->pos.x;
    if (var_a1 >= -0x600000) {
        arg0->pos.x = -0x600000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingNancyPhase2B);
        gEndingCreditsSequencePhase = 0x2A;
        var_a1 = arg0->pos.x;
    }
    setMainMenuSceneModelPosition(1, var_a1, arg0->pos.y, arg0->pos.z);
    addMainMenuSceneModelDrawCallback(1);
}

void waitEndingNancyPhase29(EndingCreditsNancyActor *arg0) {
    if (gEndingCreditsSequencePhase == 0x29) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancyRunRightThenPhase2A);
        setMainMenuSceneModelAnimation(1, 0x4C);
        arg0->pos.z = 0xA0000;
    }
}

void updateEndingNancySlideFarLeftSetPhase12(EndingCreditsNancyActor *arg0) {
    arg0->pos.x += 0xFFFE0000;
    setMainMenuSceneModelPosition(1, arg0->pos.x, arg0->pos.y, arg0->pos.z);
    if (arg0->pos.x < -0x18FFFFF) {
        gEndingCreditsSequencePhase = 0x12;
        arg0->pos.x = -0x1900000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingNancyPhase29);
    }
    loopMainMenuSceneModelAnimation(1);
    addMainMenuSceneModelDrawCallback(1);
}

void updateEndingNancyWaitBeforeSlideFarLeft(EndingCreditsNancyActor *arg0) {
    loopMainMenuSceneModelAnimation(1);
    addMainMenuSceneModelDrawCallback(1);
    if (++arg0->timer == 0x32) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancySlideFarLeftSetPhase12);
    }
}

void updateEndingNancySlideRightToPose(EndingCreditsNancyActor *arg0) {
    loopMainMenuSceneModelAnimation(1);
    arg0->pos.x += 0x60000;
    setMainMenuSceneModelPosition(1, arg0->pos.x, arg0->pos.y, arg0->pos.z);
    if (arg0->pos.x >= -0x580000) {
        arg0->pos.x = -0x580000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancyWaitBeforeSlideFarLeft);
    }
    addMainMenuSceneModelDrawCallback(1);
}

void waitEndingNancyPhase11(EndingCreditsNancyActor *arg0) {
    if (gEndingCreditsSequencePhase == 0x11) {
        arg0->pos.x = 0xFE700000;
        arg0->rot.y = 0x400;
        setMainMenuSceneModelPosition(1, -0x1900000, arg0->pos.y, arg0->pos.z);
        setMainMenuSceneModelRotation(1, arg0->rot.x, arg0->rot.y, arg0->rot.z);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancySlideRightToPose);
        setMainMenuSceneModelAnimation(1, 0x1F);
    }
}

void updateEndingNancyExitRightSetPhase0F(EndingCreditsNancyActor *arg0) {
    loopMainMenuSceneModelAnimation(1);
    arg0->pos.x += 0x60000;
    setMainMenuSceneModelPosition(1, arg0->pos.x, arg0->pos.y, arg0->pos.z);
    if (arg0->pos.x >= 0x1900000) {
        arg0->pos.x = 0x1900000;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingNancyPhase11);
        gEndingCreditsSequencePhase = 0xF;
    }
    addMainMenuSceneModelDrawCallback(1);
}

void waitEndingNancyPhase0E(EndingCreditsNancyActor *arg0) {
    loopMainMenuSceneModelAnimation(1);
    addMainMenuSceneModelDrawCallback(1);
    if (gEndingCreditsSequencePhase == 0xE) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancyExitRightSetPhase0F);
        setMainMenuSceneModelAnimation(1, 0x1F);
    }
}

void updateEndingNancyStartRunDust(EndingCreditsNancyActor *arg0) {
    if (stepMainMenuSceneModelAnimation(1) == 1) {
        if (++arg0->timer == 0xC) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingNancyPhase0E);
            setMainMenuSceneModelAnimation(1, 0x15);
            createCallbackTask((CallbackTaskCallback)&initEndingCreditsNancyRunDust, 0, 0x64);
        }
    }
    addMainMenuSceneModelDrawCallback(1);
}

void updateEndingNancyRepeatAnimThenVanish(EndingCreditsNancyActor *arg0) {
    if (stepMainMenuSceneModelAnimation(1) == 1) {
        arg0->timer += 1;
        if (arg0->timer < 3) {
            setMainMenuSceneModelAnimation(1, 0x13);
        }
        if (arg0->timer == 3) {
            arg0->timer = 0;
            gEndingCreditsSequencePhase = 0xD;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancyStartRunDust);
            setMainMenuSceneModelAnimation(1, 0x14);
            spawnEndingCreditsCharacterVanishPoof(8, -0x40, 1, 0);
        }
    }
    addMainMenuSceneModelDrawCallback(1);
}

void startEndingNancyRepeatAnim(EndingCreditsNancyActor *arg0) {
    if (stepMainMenuSceneModelAnimation(1) == 1) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancyRepeatAnimThenVanish);
        setMainMenuSceneModelAnimation(1, 0x13);
    }
    addMainMenuSceneModelDrawCallback(1);
}

void updateEndingNancyWaitBeforeRepeatAnim(EndingCreditsNancyActor *arg0) {
    stepMainMenuSceneModelAnimation(1);
    arg0->timer += 1;
    if (arg0->timer == 0x46) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)startEndingNancyRepeatAnim);
        setMainMenuSceneModelAnimation(1, 0x12);
        spawnEndingCreditsDelayedSparkle(-0xB, -0x58, 1);
    }
    addMainMenuSceneModelDrawCallback(1);
}

void waitEndingNancyPhase0C(EndingCreditsNancyActor *arg0) {
    s32 temp_a3;

    if (stepMainMenuSceneModelAnimation(1) == 0) {
        temp_a3 = (arg0->pos.z += (s32)0xFFFA0000);
        setMainMenuSceneModelPosition(1, arg0->pos.x, arg0->pos.y, temp_a3);
    }
    if (gEndingCreditsSequencePhase == 0xC) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancyWaitBeforeRepeatAnim);
        setMainMenuSceneModelAnimation(1, 0x11);
    }
    addMainMenuSceneModelDrawCallback(1);
}

void updateEndingNancyEnterUntilPhase0B(EndingCreditsNancyActor *arg0) {
    arg0->pos.x += 0x39435;
    if (gEndingCreditsSequencePhase == 0xB) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingNancyPhase0C);
        setMainMenuSceneModelAnimation(1, 0x10);
        spawnEndingCreditsCharacterVanishPoof(0xA, -0x40, 1, 0);
    } else {
        setMainMenuSceneModelPosition(1, arg0->pos.x, arg0->pos.y, arg0->pos.z);
        loopMainMenuSceneModelAnimation(1);
    }
    addMainMenuSceneModelDrawCallback(1);
}

void waitEndingNancyPhase9(EndingCreditsNancyActor *arg0) {
    if (gEndingCreditsSequencePhase == 9) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingNancyEnterUntilPhase0B);
    }
}

void initEndingCreditsNancy(EndingCreditsNancyActor *arg0) {
    arg0->pos.x = 0xFE700000;
    arg0->pos.y = 0;
    arg0->pos.z = 0;
    arg0->rot.x = 0;
    arg0->rot.y = 0x400;
    arg0->rot.z = 0;
    initMainMenuSceneModel(1, 1);
    setMainMenuSceneModelAnimation(1, 0xF);
    setMainMenuSceneModelPosition(1, arg0->pos.x, arg0->pos.y, arg0->pos.z);
    setMainMenuSceneModelRotation(1, arg0->rot.x, arg0->rot.y, arg0->rot.z);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitEndingNancyPhase9);
}
