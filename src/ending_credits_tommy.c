#include "common.h"
#include "callback_task_scheduler.h"
#include "menu_transition_effects.h"
#include "ending_credits_tommy.h"
#include "main_menu_scene_actor_3.h"
#include "main_menu_scene_model.h"
#include "main_menu_scene_renderer.h"

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

extern u8 gEndingActorHandshakeState;
extern u16 gEndingSequencePhase;
extern u8 gEndingTommyEffectDone;

void updateEndingTommyFinalPose(EndingCreditsTommy *arg0) {
    stepMainMenuSceneModelAnimation(2);
    func_800428C8(2);
}

void updateEndingTommyStartFinalPose(EndingCreditsTommy *arg0) {
    stepMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x41) {
        setCallbackTaskCallback(arg0, updateEndingTommyFinalPose);
        setMainMenuSceneModelAnimation(2, 0x56);
        arg0->rotY = 0xC00;
        setMainMenuSceneModelRotation(2, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
}

void waitEndingTommyPhase40(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x40) {
        setCallbackTaskCallback(arg0, updateEndingTommyStartFinalPose);
        setMainMenuSceneModelAnimation(2, 0x68);
    }
}

void updateEndingTommyWaitBeforePhase40(EndingCreditsTommy *arg0) {
    u16 *p;
    loopMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    p = &arg0->timer;
    (*p)++;
    if (*p == 0x1E) {
        *p = 0;
        setCallbackTaskCallback(arg0, waitEndingTommyPhase40);
        gEndingSequencePhase = 0x3F;
    }
}

void updateEndingTommyPhase3FAnim3(EndingCreditsTommy *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    if (sp18 == 1) {
        setCallbackTaskCallback(arg0, updateEndingTommyWaitBeforePhase40);
        setMainMenuSceneModelAnimation(2, 0x67);
    }
}

void updateEndingTommyPhase3FAnim2(EndingCreditsTommy *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    if (sp18 == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0x1E) {
            *p = 0;
            setCallbackTaskCallback(arg0, updateEndingTommyPhase3FAnim3);
            setMainMenuSceneModelAnimation(2, 0x66);
        }
    }
}

void updateEndingTommyPhase3FAnim1(EndingCreditsTommy *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    if (sp18 == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0x14) {
            *p = 0;
            setCallbackTaskCallback(arg0, updateEndingTommyPhase3FAnim2);
            setMainMenuSceneModelAnimation(2, 0x65);
            spawnEndingPhaseAdvanceSparkle(-0x1F, -0x6B);
        }
    }
}

void waitEndingTommyPhase3E(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x3E) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingTommyPhase3FAnim1);
        setMainMenuSceneModelAnimation(2, 0x64);
    }
}

void updateEndingTommySetPhase3D(EndingCreditsTommy *arg0) {
    u16 *p;
    loopMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    p = &arg0->timer;
    (*p)++;
    if (*p == 0x32) {
        setCallbackTaskCallback(arg0, waitEndingTommyPhase3E);
        gEndingSequencePhase = 0x3D;
    }
}

void updateEndingTommyPhase3DPrep(EndingCreditsTommy *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    if (sp18 == 1) {
        setCallbackTaskCallback(arg0, updateEndingTommySetPhase3D);
        setMainMenuSceneModelAnimation(2, 0x63);
    }
}

void updateEndingTommyWaitBeforePhase3D(EndingCreditsTommy *arg0) {
    if (stepMainMenuSceneModelAnimation(2) == 1) {
        setCallbackTaskCallback(arg0, updateEndingTommyPhase3DPrep);
        setMainMenuSceneModelAnimation(2, 0x62);
    }
    func_800428C8(2);
}

void updateEndingTommyWaitBeforeIdleAnim2(EndingCreditsTommy *arg0) {
    if (stepMainMenuSceneModelAnimation(2) == 1) {
        setCallbackTaskCallback(arg0, updateEndingTommyWaitBeforePhase3D);
        setMainMenuSceneModelAnimation(2, 0x23);
    }
    func_800428C8(2);
}

void updateEndingTommyWaitBeforeIdleAnim1(EndingCreditsTommy *arg0) {
    if (stepMainMenuSceneModelAnimation(2) == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0xF) {
            *p = 0;
            setCallbackTaskCallback(arg0, updateEndingTommyWaitBeforeIdleAnim2);
            setMainMenuSceneModelAnimation(2, 0x22);
        }
    }
    func_800428C8(2);
}

void updateEndingTommyHopRightToIdle(EndingCreditsTommy *arg0) {
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
    func_800428C8(2);
    if (sp20 == 1) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingTommyWaitBeforeIdleAnim1);
        setMainMenuSceneModelAnimation(2, 0x21);
    }
}

void updateEndingTommySlideLeftSetPhase3B(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX += 0xFFF70000;
    if (arg0->posX < (s32)0xFFA00001) {
        arg0->posX = 0xFFA00000;
        setCallbackTaskCallback(arg0, updateEndingTommyHopRightToIdle);
        arg0->rotY = 0xC00;
        setMainMenuSceneModelRotation(2, arg0->rotX, arg0->rotY, arg0->rotZ);
        setMainMenuSceneModelAnimation(2, 0x20);
        gEndingSequencePhase = 0x3B;
        gEndingTommyEffectDone = 1;
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void waitEndingTommyPhase3A(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x3A) {
        setCallbackTaskCallback(arg0, updateEndingTommySlideLeftSetPhase3B);
    }
}

void updateEndingTommySlideRightToCenter(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX += 0x24000;
    if (arg0->posX >= 0x100000) {
        arg0->posX = 0x100000;
        setCallbackTaskCallback(arg0, waitEndingTommyPhase3A);
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void waitEndingTommyPhase38(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x38) {
        setCallbackTaskCallback(arg0, updateEndingTommySlideRightToCenter);
    }
}

void updateEndingTommySlideLeftFromFarRight(EndingCreditsTommy *arg0) {
    s32 var_a1;
    EndingCreditsTommy *new_var;

    loopMainMenuSceneModelAnimation(2);
    new_var = arg0;
    var_a1 = (new_var->posX += -0x48000);
    if (var_a1 < -0x7FFFFF) {
        new_var->posX = -0x800000;
        setCallbackTaskCallback(new_var, waitEndingTommyPhase38);
        var_a1 = arg0->posX;
    } else if ((var_a1 < 0x1300001) && (gEndingSequencePhase == 0x33)) {
        gEndingSequencePhase = 0x34;
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(2, var_a1, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void waitEndingTommyPhase33Aura(EndingCreditsTommy *arg0) {
    if (gEndingSequencePhase == 0x33) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0x14) {
            *p = 0;
            setCallbackTaskCallback(arg0, updateEndingTommySlideLeftFromFarRight);
            arg0->posX = 0x1900000;
            arg0->posZ = 0x120000;
            gEndingTommyEffectDone = 0;
            spawnEndingCharacterAura(0x18, -0x36, 2, 1);
        }
    }
}

void updateEndingTommyDashOffLeftSetPhase31(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX += 0xFFFB8000;
    if (arg0->posX < (s32)0xFE700001) {
        arg0->posX = 0xFE700000;
        setCallbackTaskCallback(arg0, waitEndingTommyPhase33Aura);
        gEndingSequencePhase = 0x31;
        gEndingTommyEffectDone = 1;
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void updateEndingTommyVanishRetreat(EndingCreditsTommy *arg0) {
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
    func_800428C8(2);
    if (sp20 == 1) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingTommyDashOffLeftSetPhase31);
        arg0->posY = 0;
        setMainMenuSceneModelPosition(2, arg0->posX, 0, arg0->posZ);
        setMainMenuSceneModelAnimation(2, 0x1B);
        gEndingTommyEffectDone = 0;
        spawnEndingCharacterAura(0x18, -0x36, 2, 1);
    }
}

void waitEndingTommyPhase2F(EndingCreditsTommy *arg0) {
    func_800428C8(2);
    if (gEndingSequencePhase == 0x2F) {
        setCallbackTaskCallback(arg0, updateEndingTommyVanishRetreat);
        setMainMenuSceneModelAnimation(2, 0x4E);
        spawnEndingCharacterVanishPoof(8, -0x40, 2, 0);
    }
}

void updateEndingTommyDriftAfterPhase2D(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX = arg0->posX + 0x2000;
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x2D) {
        setCallbackTaskCallback(arg0, waitEndingTommyPhase2F);
    }
}

void updateEndingTommyWaitThenSetPhase2C(EndingCreditsTommy *arg0) {
    u16 *p;
    loopMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    p = &arg0->timer;
    (*p)++;
    if (*p == 0x19) {
        gEndingSequencePhase = 0x2C;
        setCallbackTaskCallback(arg0, updateEndingTommyDriftAfterPhase2D);
        *p = 0;
    }
}

void updateEndingTommyRunLeftThenPhase2C(EndingCreditsTommy *arg0) {
    if (stepMainMenuSceneModelAnimation(2) == 0) {
        arg0->posX = arg0->posX + 0xFFF60000;
    } else {
        setCallbackTaskCallback(arg0, updateEndingTommyWaitThenSetPhase2C);
        setMainMenuSceneModelAnimation(2, 0x44);
        arg0->posZ = 0xFFFF0000;
        arg0->timer = 0;
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void updateEndingTommySlideFarLeft(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX += 0xFFFC8000;
    arg0->posZ += 0xFFFF0000;
    if (arg0->posX < (s32)0xFFA80001) {
        arg0->posX = 0xFF780000;
        setCallbackTaskCallback(arg0, updateEndingTommyRunLeftThenPhase2C);
        setMainMenuSceneModelAnimation(2, 0x43);
        arg0->timer = 0;
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void waitEndingTommyPhase2A(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x2A) {
        setCallbackTaskCallback(arg0, updateEndingTommySlideFarLeft);
        arg0->posZ = 0xFFFC0000;
    }
}

void updateEndingTommySlideRightToPose(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX += 0x2D000;
    if (arg0->posX >= 0xF00000) {
        arg0->posX = 0xF00000;
        setCallbackTaskCallback(arg0, waitEndingTommyPhase2A);
        gEndingSequencePhase = 0x29;
        gEndingTommyEffectDone = 1;
        spawnEndingCharacterLoopingSparkle(-0x10, -0x4E, 2);
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void waitEndingTommyPhase28(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x28) {
        setCallbackTaskCallback(arg0, updateEndingTommySlideRightToPose);
    }
}

void updateEndingTommySlideRightToMarker(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX += 0x2E000;
    if (arg0->posX >= (s32)0xFFD00000) {
        arg0->posX = 0xFFD00000;
        setCallbackTaskCallback(arg0, waitEndingTommyPhase28);
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void waitEndingTommyBeforeMarkerSlide(EndingCreditsTommy *arg0) {
    u16 *p;
    loopMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    p = &arg0->timer;
    (*p)++;
    if (*p == 0xF) {
        *p = 0;
        setCallbackTaskCallback(arg0, updateEndingTommySlideRightToMarker);
    }
}

void updateEndingTommySlideLeftToMarker(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX += 0xFFFD2000;
    if (arg0->posX < (s32)0xFF800001) {
        arg0->posX = 0xFF800000;
        setCallbackTaskCallback(arg0, waitEndingTommyBeforeMarkerSlide);
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void updateEndingTommyWaitThenSetPhase27(EndingCreditsTommy *arg0) {
    u16 *p;
    loopMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    p = &arg0->timer;
    (*p)++;
    if (*p == 0x19) {
        *p = 0;
        setCallbackTaskCallback(arg0, updateEndingTommySlideLeftToMarker);
        gEndingSequencePhase = 0x27;
    }
}

void updateEndingTommySlideRightToPhase27Start(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX += 0x20000;
    if (arg0->posX >= (s32)0xFFD80000) {
        arg0->posX = 0xFFD80000;
        setCallbackTaskCallback(arg0, updateEndingTommyWaitThenSetPhase27);
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void startEndingTommyPhase26Slide(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    gEndingSequencePhase = 0x26;
    if (gEndingSequencePhase != 0) {
        setCallbackTaskCallback(arg0, updateEndingTommySlideRightToPhase27Start);
    }
}

void updateEndingTommySlideLeftSetPhase25(EndingCreditsTommy *arg0) {
    s32 *temp_a2;
    s32 var_a1;

    loopMainMenuSceneModelAnimation(2);
    var_a1 = (arg0->posX += 0xFFFD4000);
    if (var_a1 < (s32)0xFF550001) {
        arg0->posX = 0xFF550000;
        setCallbackTaskCallback(arg0, startEndingTommyPhase26Slide);
        gEndingSequencePhase = 0x25;
        var_a1 = arg0->posX;
    }
    temp_a2 = &arg0->posY;
    setMainMenuSceneModelPosition(2, var_a1, *temp_a2, arg0->posZ);
    func_800428C8(2);
}

void waitEndingTommyBeforeDiagonalSlide(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x24) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0x12) {
            *p = 0;
            setCallbackTaskCallback(arg0, updateEndingTommySlideLeftSetPhase25);
        }
    }
}

void updateEndingTommyDiagonalSlideSetPhase22(EndingCreditsTommy *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(2);
    var_a1 = (arg0->posX -= -0x28000);
    if (var_a1 >= (s32)0xFFD00000) {
        arg0->posX = 0xFFD00000;
        setCallbackTaskCallback(arg0, waitEndingTommyBeforeDiagonalSlide);
        var_a1 = arg0->posX;
    }
    if ((var_a1 >= (s32)0xFFA00000) && (gEndingSequencePhase == 0x21)) {
        gEndingSequencePhase = 0x22;
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(2, var_a1, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void waitEndingTommyPhase21Aura(EndingCreditsTommy *arg0) {
    if (gEndingSequencePhase == 0x21) {
        setCallbackTaskCallback(arg0, updateEndingTommyDiagonalSlideSetPhase22);
        gEndingTommyEffectDone = 0;
        spawnEndingCharacterAura(0x18, -0x36, 2, 1);
    }
}

void updateEndingTommyExitLeftAfterPhase21(EndingCreditsTommy *arg0) {
    s32 *temp_a2;
    s32 var_a1;

    loopMainMenuSceneModelAnimation(2);
    var_a1 = (arg0->posX += 0xFFFA0000);
    if (var_a1 < (s32)0xFE700001) {
        arg0->posX = 0xFE700000;
        setCallbackTaskCallback(arg0, waitEndingTommyPhase21Aura);
        gEndingTommyEffectDone = 1;
        var_a1 = arg0->posX;
    }
    temp_a2 = &arg0->posY;
    setMainMenuSceneModelPosition(2, var_a1, *temp_a2, arg0->posZ);
    func_800428C8(2);
}

void updateEndingTommyAfterVanishWait(EndingCreditsTommy *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(2);
    var_a1 = (arg0->posX += 0xFFFB0000);
    if (var_a1 < (s32)0xFF600001) {
        arg0->posX = 0xFF600000;
        setCallbackTaskCallback(arg0, updateEndingTommyExitLeftAfterPhase21);
        setMainMenuSceneModelAnimation(2, 0x1B);
        arg0->rotY = 0xC00;
        setMainMenuSceneModelRotation(2, arg0->rotX, arg0->rotY, arg0->rotZ);
        gEndingSequencePhase = 0x20;
        gEndingTommyEffectDone = 0;
        spawnEndingCharacterAura(0x18, -0x36, 2, 1);
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(2, var_a1, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void updateEndingTommyWaitBeforeVanishReturn(EndingCreditsTommy *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    if (sp18 == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0x14) {
            *p = 0;
            setCallbackTaskCallback(arg0, updateEndingTommyAfterVanishWait);
            setMainMenuSceneModelAnimation(2, 0x48);
        }
    }
}

void startEndingTommyVanishReturn(EndingCreditsTommy *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    if (sp18 == 1) {
        setCallbackTaskCallback(arg0, updateEndingTommyWaitBeforeVanishReturn);
        setMainMenuSceneModelAnimation(2, 0x47);
        spawnEndingCharacterVanishPoof(5, -0x46, 2, 0);
    }
}

void updateEndingTommySlideLeftThenVanish(EndingCreditsTommy *arg0) {
    if (stepMainMenuSceneModelAnimation(2) == 0) {
        arg0->posX += 0xFFFE0000;
        setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    } else {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0xF) {
            *p = 0;
            setCallbackTaskCallback(arg0, startEndingTommyVanishReturn);
            setMainMenuSceneModelAnimation(2, 0x46);
        }
    }
    func_800428C8(2);
}

void waitEndingTommyPhase1E(EndingCreditsTommy *arg0) {
    func_800428C8(2);
    if (gEndingSequencePhase == 0x1E) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingTommySlideLeftThenVanish);
        setMainMenuSceneModelAnimation(2, 0x45);
        spawnEndingSmallBurst(0x2A, -0x57);
    }
}

void updateEndingTommyExitLeftSetPhase1D(EndingCreditsTommy *arg0) {
    s32 sp24;
    s32 sp20;

    sp20 = stepMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    if (sp20 == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 5) {
            *p = 0;
            setCallbackTaskCallback(arg0, waitEndingTommyPhase1E);
            gEndingSequencePhase = 0x1D;
        }
    } else {
        arg0->posX += 0xFFFB8000;
        setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    }
}

void updateEndingTommyRepeatAnimThenVanish(EndingCreditsTommy *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    if (sp18 == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0x14) {
            *p = 0;
            setCallbackTaskCallback(arg0, updateEndingTommyExitLeftSetPhase1D);
            setMainMenuSceneModelAnimation(2, 0x35);
            spawnEndingCharacterVanishPoof(8, -0x40, 2, 0);
        }
    }
}

void waitEndingTommyPhase1C(EndingCreditsTommy *arg0) {
    stepMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x1C) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingTommyRepeatAnimThenVanish);
        setMainMenuSceneModelAnimation(2, 0x34);
    }
}

void updateEndingTommyWaitBeforePhase1C(EndingCreditsTommy *arg0) {
    u16 *p;
    func_800428C8(2);
    p = &arg0->timer;
    (*p)++;
    if (*p == 0x1E) {
        *p = 0;
        setCallbackTaskCallback(arg0, waitEndingTommyPhase1C);
        setMainMenuSceneModelAnimation(2, 0x33);
        arg0->posZ = 0xFFFF0000;
        setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, 0xFFFF0000);
    }
}

void updateEndingTommyStepBackAfterHandshake(EndingCreditsTommy *arg0) {
    if (stepMainMenuSceneModelAnimation(2) == 1) {
        setCallbackTaskCallback(arg0, updateEndingTommyWaitBeforePhase1C);
    } else {
        arg0->posZ = arg0->posZ + 0xFFFC0000;
        setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    }
    func_800428C8(2);
}

void waitEndingTommyHandshakeExit(EndingCreditsTommy *arg0) {
    func_800428C8(2);
    if (gEndingActorHandshakeState == 5) {
        setCallbackTaskCallback(arg0, updateEndingTommyStepBackAfterHandshake);
        setMainMenuSceneModelAnimation(2, 0x32);
        arg0->rotY = 0x400;
        setMainMenuSceneModelRotation(2, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
}

void updateEndingTommyHandshakeLoopThird(EndingCreditsTommy *arg0) {
    if (gEndingActorHandshakeState == 2) {
        stepMainMenuSceneModelAnimation(2);
        if (stepMainMenuSceneModelAnimation(2) == 1) {
            u16 *p;
            gEndingActorHandshakeState++;
            setMainMenuSceneModelAnimation(2, 0x2B);
            p = &arg0->timer;
            (*p)++;
            if (*p == 0x4) {
                setCallbackTaskCallback(arg0, waitEndingTommyHandshakeExit);
                *p = 0;
            }
        }
    }
    func_800428C8(2);
}

void updateEndingTommyHandshakeLoopSecond(EndingCreditsTommy *arg0) {
    if (gEndingActorHandshakeState == 2) {
        stepMainMenuSceneModelAnimation(2);
        if (stepMainMenuSceneModelAnimation(2) == 1) {
            u16 *p;
            gEndingActorHandshakeState++;
            setMainMenuSceneModelAnimation(2, 0x2B);
            p = &arg0->timer;
            (*p)++;
            if (*p == 0x5) {
                setCallbackTaskCallback(arg0, updateEndingTommyHandshakeLoopThird);
                *p = 0;
            }
        }
    }
    func_800428C8(2);
}

void updateEndingTommyHandshakeLoopFirst(EndingCreditsTommy *arg0) {
    if (gEndingActorHandshakeState == 2) {
        if (arg0->rotY == 0xC01) {
            arg0->rotY = 0xC00;
            setMainMenuSceneModelRotation(2, arg0->rotX, arg0->rotY, arg0->rotZ);
        }
        if (stepMainMenuSceneModelAnimation(2) == 1) {
            u16 *p;
            gEndingActorHandshakeState++;
            setMainMenuSceneModelAnimation(2, 0x2B);
            p = &arg0->timer;
            (*p)++;
            if (*p == 0x3) {
                setCallbackTaskCallback(arg0, updateEndingTommyHandshakeLoopSecond);
                *p = 0;
            }
        }
    }
    func_800428C8(2);
}

void startEndingTommyHandshakeLoop(EndingCreditsTommy *arg0) {
    if (gEndingActorHandshakeState == 2) {
        if (stepMainMenuSceneModelAnimation(2) == 1) {
            gEndingActorHandshakeState++;
            setCallbackTaskCallback(arg0, updateEndingTommyHandshakeLoopFirst);
            setMainMenuSceneModelAnimation(2, 0x2B);
            arg0->rotY = 0xC01;
            arg0->timer = 0;
        }
    }
    func_800428C8(2);
}

void updateEndingTommySlideRightUntilPhase18(EndingCreditsTommy *arg0) {
    stepMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    arg0->posX = arg0->posX + 0x30000;
    if (gEndingSequencePhase == 0x18) {
        setCallbackTaskCallback(arg0, startEndingTommyHandshakeLoop);
        setMainMenuSceneModelAnimation(2, 0x31);
        arg0->rotY = 0x400;
        setMainMenuSceneModelRotation(2, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
}

void waitEndingTommyPhase17(EndingCreditsTommy *arg0) {
    u16 val;
    loopMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    val = arg0->timer;
    if (val < 0x2D) {
        arg0->timer = val + 1;
        if (arg0->timer == 0x2D) {
            gEndingSequencePhase = 0x16;
        }
    } else if (gEndingSequencePhase == 0x17) {
        setCallbackTaskCallback(arg0, updateEndingTommySlideRightUntilPhase18);
        setMainMenuSceneModelAnimation(2, 0x30);
        arg0->rotY = 0x400;
        setMainMenuSceneModelRotation(2, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
}

void updateEndingTommySlideLeftToIdle(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    arg0->posX = arg0->posX + 0xFFFD0000;
    if (arg0->posX < 0x4E0001) {
        arg0->posX = 0x4E0000;
        setCallbackTaskCallback(arg0, waitEndingTommyPhase17);
        setMainMenuSceneModelAnimation(2, 0x26);
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void startEndingTommyIdleAfterAnim(EndingCreditsTommy *arg0) {
    if (stepMainMenuSceneModelAnimation(2) == 1) {
        setCallbackTaskCallback(arg0, updateEndingTommySlideLeftToIdle);
        setMainMenuSceneModelAnimation(2, 0);
    }
    func_800428C8(2);
}

void updateEndingTommyWaitBeforeIdleAnimB(EndingCreditsTommy *arg0) {
    if (stepMainMenuSceneModelAnimation(2) == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0xF) {
            *p = 0;
            setCallbackTaskCallback(arg0, startEndingTommyIdleAfterAnim);
            setMainMenuSceneModelAnimation(2, 0x23);
        }
    }
    func_800428C8(2);
}

void updateEndingTommyWaitBeforeIdleAnimA(EndingCreditsTommy *arg0) {
    if (stepMainMenuSceneModelAnimation(2) == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0xF) {
            *p = 0;
            setCallbackTaskCallback(arg0, updateEndingTommyWaitBeforeIdleAnimB);
            setMainMenuSceneModelAnimation(2, 0x22);
        }
    }
    func_800428C8(2);
}

void updateEndingTommyHopRightToPose(EndingCreditsTommy *arg0) {
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
    func_800428C8(2);
    if (sp20 == 1) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingTommyWaitBeforeIdleAnimA);
        setMainMenuSceneModelAnimation(2, 0x21);
    }
}

void waitEndingTommyPhase15(EndingCreditsTommy *arg0) {
    loopMainMenuSceneModelAnimation(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x15) {
        setCallbackTaskCallback(arg0, updateEndingTommyHopRightToPose);
        arg0->rotY = 0xC00;
        setMainMenuSceneModelRotation(2, arg0->rotX, arg0->rotY, arg0->rotZ);
        setMainMenuSceneModelAnimation(2, 0x20);
    }
}

void updateEndingTommySlideLeftToPhase15Wait(EndingCreditsTommy *arg0) {
    arg0->posX = arg0->posX + 0xFFFB8000;
    if (arg0->posX < 0x500001) {
        arg0->posX = 0x500000;
        setCallbackTaskCallback(arg0, waitEndingTommyPhase15);
    }
    setMainMenuSceneModelPosition(2, arg0->posX, arg0->posY, arg0->posZ);
    loopMainMenuSceneModelAnimation(2);
    func_800428C8(2);
}

void waitEndingTommyPhase13(EndingCreditsTommy *arg0) {
    if (gEndingSequencePhase == 0x13) {
        setCallbackTaskCallback(arg0, updateEndingTommySlideLeftToPhase15Wait);
        func_8003B308(arg0->posX + 0x48000, 0x480000, 0xFFF30000, 2, 2, 1);
    }
}

void initEndingCreditsTommy(EndingCreditsTommy *arg0) {
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
    setCallbackTaskCallback(arg0, waitEndingTommyPhase13);
}
