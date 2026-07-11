#include "common.h"
#include "callback_task_scheduler.h"
#include "ending_sequence_actors.h"
#include "ending_credits_slash.h"
#include "ending_credits_tommy.h"
#include "main_menu_scene_model.h"
#include "main_menu_scene_model_renderer.h"

struct EndingCreditsSlash {
    char pad0[0x18];
    /* 0x18 */ s32 posX;
    /* 0x1C */ s32 posY;
    /* 0x20 */ s32 posZ;
    /* 0x24 */ s16 rotX;
    /* 0x26 */ s16 rotY;
    /* 0x28 */ s16 rotZ;
    /* 0x2A */ u16 timer;
    /* 0x2C */ s16 unk2C;
};

extern MainMenuSceneActorShadow gEndingActorShadow;

extern u16 gEndingSequencePhase;
extern u8 gEndingActorHandshakeState;
extern u8 gEndingTransitionRotationStep;
extern u8 gEndingCharacterEffectDoneFlags[];

void noopEndingCreditsSlash(void) {
}

void updateEndingSlashFinalPose(EndingCreditsSlash *arg0) {
    EndingCreditsSlash *temp = arg0;

    stepMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if (temp->timer < 0x96) {
        temp->timer += 1;
        if (temp->timer == 0x96) {
            gEndingSequencePhase = 0x42;
        }
    }
}

void updateEndingSlashStartFinalPose(EndingCreditsSlash *arg0) {
    struct {
        s32 ret;
        s32 pad;
    } l;

    l.ret = stepMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if (l.ret == 1) {
        arg0->timer += 1;
        if (arg0->timer == 0x41) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, updateEndingSlashFinalPose);
            setMainMenuSceneModelAnimation(0, 0x56);
            gEndingSequencePhase = 0x41;
            arg0->rotY = 0xC00;
            setMainMenuSceneModelRotation(0, arg0->rotX, arg0->rotY, arg0->rotZ);
        }
    }
}

void waitEndingSlashPhase40(EndingCreditsSlash *arg0) {
    stepMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if (gEndingSequencePhase == 0x40) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingSlashStartFinalPose);
        setMainMenuSceneModelAnimation(0, 0x55);
    }
}

void updateEndingSlashPhase40Prep(EndingCreditsSlash *arg0) {
    EndingCreditsSlash *temp = arg0;

    if (stepMainMenuSceneModelAnimation(0) == 1) {
        temp->timer += 1;
        if (temp->timer == 0x1E) {
            temp->timer = 0;
            setCallbackTaskCallback(temp, waitEndingSlashPhase40);
            setMainMenuSceneModelAnimation(0, 0x54);
            spawnEndingPhaseAdvanceSparkle(0xA, -0x64);
        }
    }
    addMainMenuSceneModelDrawCallback(0);
}

void updateEndingSlashWaitPhase3FAnim(EndingCreditsSlash *arg0) {
    struct { s32 ret; s32 pad; } l;
    l.ret = stepMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if (l.ret == 1) {
        arg0->timer += 1;
        if (arg0->timer == 0x14) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, updateEndingSlashPhase40Prep);
            setMainMenuSceneModelAnimation(0, 0x53);
        }
    }
}

void updateEndingSlashSpinUntilPhase3F(EndingCreditsSlash *arg0) {
    if (stepMainMenuSceneModelAnimation(0) == 0) {
        arg0->posX += 0x18000;
        setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    }
    if (gEndingSequencePhase == 0x3F) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingSlashWaitPhase3FAnim);
        setMainMenuSceneModelAnimation(0, 0x22);
    }
    addMainMenuSceneModelDrawCallback(0);
}

void updateEndingSlashTumbleToPhase3C(EndingCreditsSlash *arg0) {
    s32 unused;
    s32 sp20;
    s32 var_v0;

    sp20 = stepMainMenuSceneModelAnimation(0);
    if (arg0->timer < 5) {
        var_v0 = 1;
    } else {
        var_v0 = -1;
    }
    arg0->posX += 0x80000;
    arg0->posY += var_v0 * 0x80000;
    arg0->timer = arg0->timer + 1;
    if (arg0->timer == 2) {
        gEndingSequencePhase = 0x3C;
    }
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
    if (sp20 == 1) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingSlashSpinUntilPhase3F);
        setMainMenuSceneModelAnimation(0, 0x21);
    }
}

void updateEndingSlashExitUntilPhase3B(EndingCreditsSlash *arg0) {
    loopMainMenuSceneModelAnimation(0);
    arg0->posX += 0xFFF70000;
    if (gEndingSequencePhase == 0x3B) {
        setCallbackTaskCallback(arg0, updateEndingSlashTumbleToPhase3C);
        setMainMenuSceneModelAnimation(0, 0x20);
        arg0->rotY = 0xC00;
        setMainMenuSceneModelRotation(0, arg0->rotX, arg0->rotY, arg0->rotZ);
        gEndingCharacterEffectDoneFlags[0] = 1;
    }
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
}

void waitEndingSlashPhase3A(s32 arg0) {
    loopMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if (gEndingSequencePhase == 0x3A) {
        setCallbackTaskCallback(arg0, updateEndingSlashExitUntilPhase3B);
    }
}

void updateEndingSlashSlowRotationWipe(EndingCreditsSlash *arg0) {
    loopMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    arg0->timer += 1;
    if (gEndingTransitionRotationStep >= 3) {
        if (arg0->timer % 15 == 0) {
            gEndingTransitionRotationStep--;
            if (gEndingTransitionRotationStep == 2) {
                arg0->timer = 0;
            }
        }
    } else if (arg0->timer == 0x10) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, waitEndingSlashPhase3A);
        gEndingSequencePhase = 0x39;
    }
}

void updateEndingSlashSlideToCenter(EndingCreditsSlash *arg0) {
    EndingCreditsSlash *temp = arg0;

    loopMainMenuSceneModelAnimation(0);
    temp->posX += 0x24000;
    if (temp->posX >= 0x500000) {
        temp->posX = 0x500000;
        setCallbackTaskCallback(temp, updateEndingSlashSlowRotationWipe);
    }
    setMainMenuSceneModelPosition(0, temp->posX, temp->posY, temp->posZ);
    addMainMenuSceneModelDrawCallback(0);
}

void waitEndingSlashPhase37(EndingCreditsSlash *arg0) {
    loopMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if (gEndingSequencePhase == 0x37) {
        arg0->timer += 1;
        if (arg0->timer == 0x1E) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, updateEndingSlashSlideToCenter);
            gEndingSequencePhase = 0x38;
        }
    }
}

void updateEndingSlashSlideLeftFromFarRight(EndingCreditsSlash *arg0) {
    s32 var_a1;
    EndingCreditsSlash *new_var;

    loopMainMenuSceneModelAnimation(0);
    new_var = arg0;
    var_a1 = (new_var->posX += -0x48000);
    if (var_a1 < -0x1FFFFF) {
        new_var->posX = -0x200000;
        setCallbackTaskCallback(new_var, waitEndingSlashPhase37);
        var_a1 = arg0->posX;
    } else if ((var_a1 < 0xD00001) && (gEndingSequencePhase == 0x34)) {
        gEndingSequencePhase = 0x35;
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(0, var_a1, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
}

void waitEndingSlashPhase34(EndingCreditsSlash *arg0) {
    if (gEndingSequencePhase == 0x34) {
        setCallbackTaskCallback(arg0, updateEndingSlashSlideLeftFromFarRight);
        arg0->posX = 0x1900000;
        arg0->posZ = 0;
        gEndingCharacterEffectDoneFlags[0] = 0;
        spawnEndingCharacterAura(0x1C, -0x30, 0, 1);
    }
}

void updateEndingSlashDashOffLeft(EndingCreditsSlash *arg0) {
    loopMainMenuSceneModelAnimation(0);
    arg0->posX += 0xFFFB8000;
    if (arg0->posX < (s32)0xFE700001) {
        arg0->posX = 0xFE700000;
        setCallbackTaskCallback(arg0, waitEndingSlashPhase34);
        gEndingCharacterEffectDoneFlags[0] = 1;
    }
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
}

void updateEndingSlashVanishRetreat(EndingCreditsSlash *arg0) {
    s32 unused;
    s32 sp20;

    sp20 = stepMainMenuSceneModelAnimation(0);
    arg0->posX += 0xFFF00000;
    arg0->timer += 1;
    if (arg0->timer < 3) {
        arg0->posY += 0x140000;
    } else {
        arg0->posY += 0xFFEC0000;
    }
    if (arg0->timer == 2) {
        gEndingSequencePhase = 0x30;
    }
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
    if (sp20 == 1) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingSlashDashOffLeft);
        arg0->posY = 0;
        setMainMenuSceneModelPosition(0, arg0->posX, 0, arg0->posZ);
        setMainMenuSceneModelAnimation(0, 0x1B);
        gEndingCharacterEffectDoneFlags[0] = 0;
        spawnEndingCharacterAura(0x1C, -0x30, 0, 1);
    }
}

void waitEndingSlashPhase2F(void *arg0) {
    addMainMenuSceneModelDrawCallback(0);
    if (gEndingSequencePhase == 0x2F) {
        setCallbackTaskCallback(arg0, updateEndingSlashVanishRetreat);
        setMainMenuSceneModelAnimation(0, 0x4E);
        spawnEndingCharacterVanishPoof(8, -0x40, 0, 0);
    }
}

void updateEndingSlashDriftAfterPhase2D(EndingCreditsSlash *arg0) {
    loopMainMenuSceneModelAnimation(0);
    arg0->posX += 0x2000;
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
    if (gEndingSequencePhase == 0x2D) {
        setCallbackTaskCallback(arg0, waitEndingSlashPhase2F);
    }
}

void updateEndingSlashRunLeftThenPhase2B(EndingCreditsSlash *arg0) {
    if (stepMainMenuSceneModelAnimation(0) == 0) {
        arg0->posX += 0xFFF60000;
    } else {
        setCallbackTaskCallback(arg0, updateEndingSlashDriftAfterPhase2D);
        setMainMenuSceneModelAnimation(0, 0x44);
        arg0->posZ = 0xFFFF0000;
        gEndingSequencePhase = 0x2B;
    }
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
}

void updateEndingSlashSlideFarLeft(EndingCreditsSlash *arg0) {
    loopMainMenuSceneModelAnimation(0);
    arg0->posX += 0xFFFC8000;
    if (arg0->posX < (s32)0xFFD00001) {
        arg0->posX = 0xFFA00000;
        setCallbackTaskCallback(arg0, updateEndingSlashRunLeftThenPhase2B);
        setMainMenuSceneModelAnimation(0, 0x43);
        arg0->timer = 0;
        gEndingCharacterEffectDoneFlags[0] = 1;
    }
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
}

void waitEndingSlashPhase2A(s32 arg0) {
    loopMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if (gEndingSequencePhase == 0x2A) {
        setCallbackTaskCallback(arg0, updateEndingSlashSlideFarLeft);
    }
}

void updateEndingSlashSlideRightToPose(EndingCreditsSlash *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(0);
    var_a1 = (arg0->posX += 0x2D000);
    if (var_a1 >= 0x800000) {
        arg0->posX = 0x800000;
        setCallbackTaskCallback(arg0, waitEndingSlashPhase2A);
        gEndingCharacterEffectDoneFlags[0] = 1;
        spawnEndingCharacterLoopingSparkle(-0x10, -0x4A, 0);
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(0, var_a1, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
}

void waitEndingSlashPhase28(s32 arg0) {
    loopMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if (gEndingSequencePhase == 0x28) {
        setCallbackTaskCallback(arg0, updateEndingSlashSlideRightToPose);
    }
}

void updateEndingSlashSlideLeftToMarker(EndingCreditsSlash *arg0) {
    loopMainMenuSceneModelAnimation(0);
    arg0->posX += 0xFFFDA000;
    if (arg0->posX < (s32)0xFF700001) {
        arg0->posX = 0xFF700000;
        setCallbackTaskCallback(arg0, waitEndingSlashPhase28);
    }
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
}

void updateEndingSlashSlideRightToMarker(EndingCreditsSlash *arg0) {
    loopMainMenuSceneModelAnimation(0);
    arg0->posX += 0x1D000;
    if (arg0->posX >= (s32)0xFFCE0000) {
        arg0->posX = 0xFFCE0000;
        setCallbackTaskCallback(arg0, updateEndingSlashSlideLeftToMarker);
    }
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
}

void startEndingSlashPhase27Slide(void *arg0) {
    loopMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if ((gEndingSequencePhase = 0x27) != 0) {
        setCallbackTaskCallback(arg0, updateEndingSlashSlideRightToMarker);
    }
}

void updateEndingSlashSlideLeftSetPhase26(EndingCreditsSlash *arg0) {
    loopMainMenuSceneModelAnimation(0);
    arg0->posX += 0xFFFD8000;
    if (arg0->posX < (s32)0xFF700001) {
        arg0->posX = 0xFF700000;
        setCallbackTaskCallback(arg0, startEndingSlashPhase27Slide);
        gEndingSequencePhase = 0x26;
    }
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
}

void noopEndingCreditsSlashAlt(void) {
}

void updateEndingSlashDiagonalSlideSetPhase24(EndingCreditsSlash *arg0) {
    loopMainMenuSceneModelAnimation(0);
    arg0->posX += 0x28000;
    arg0->posZ += 0xFFFE0000;
    if (arg0->posX >= (s32)0xFFC80000) {
        arg0->posX = 0xFFC80000;
        setCallbackTaskCallback(arg0, updateEndingSlashSlideLeftSetPhase26);
        gEndingSequencePhase = 0x24;
    }
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
}

void waitEndingSlashBeforeDiagonalSlide(EndingCreditsSlash *arg0) {
    loopMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    arg0->timer += 1;
    if (arg0->timer == 0x32) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingSlashDiagonalSlideSetPhase24);
    }
}

void updateEndingSlashSlideRightSetPhase23(EndingCreditsSlash *arg0) {
    loopMainMenuSceneModelAnimation(0);
    arg0->posX += 0x28000;
    if (arg0->posX >= (s32)0xFF700000) {
        arg0->posX = 0xFF700000;
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, waitEndingSlashBeforeDiagonalSlide);
        gEndingSequencePhase = 0x23;
    }
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
}

void waitEndingSlashPhase22Aura(void *arg0) {
    if (gEndingSequencePhase == 0x22) {
        setCallbackTaskCallback(arg0, updateEndingSlashSlideRightSetPhase23);
        gEndingCharacterEffectDoneFlags[0] = 0;
        spawnEndingCharacterAura(0x1C, -0x30, 0, 1);
    }
}

void updateEndingSlashExitLeftAfterPhase22(EndingCreditsSlash *arg0) {
    loopMainMenuSceneModelAnimation(0);
    arg0->posX += 0xFFFA0000;
    if (arg0->posX < (s32)0xFE700001) {
        arg0->posX = 0xFE700000;
        setCallbackTaskCallback(arg0, waitEndingSlashPhase22Aura);
        gEndingCharacterEffectDoneFlags[0] = 1;
    }
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
}

void updateEndingSlashAfterVanishWait(EndingCreditsSlash *arg0) {
    struct {
        s32 ret;
        s32 pad;
    } l;

    l.ret = stepMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if (l.ret == 1) {
        arg0->timer += 1;
        if (arg0->timer == 0x14) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, updateEndingSlashExitLeftAfterPhase22);
            setMainMenuSceneModelAnimation(0, 0x1B);
            arg0->rotY = 0xC00;
            setMainMenuSceneModelRotation(0, arg0->rotX, arg0->rotY, arg0->rotZ);
            gEndingSequencePhase = 0x1F;
            gEndingCharacterEffectDoneFlags[0] = 0;
            spawnEndingCharacterAura(0x1C, -0x30, 0, 1);
        }
    }
}

void waitEndingSlashPhase1E(void *arg0) {
    addMainMenuSceneModelDrawCallback(0);
    if (gEndingSequencePhase == 0x1E) {
        setCallbackTaskCallback(arg0, updateEndingSlashAfterVanishWait);
        setMainMenuSceneModelAnimation(0, 0x2D);
        spawnEndingCharacterVanishPoof(8, -0x40, 0, 0);
    }
}

void updateEndingSlashRepeatAnimThenVanish(EndingCreditsSlash *arg0) {
    s32 sp24;
    s32 sp20;

    sp20 = stepMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if (sp20 == 1) {
        arg0->timer++;
        if ((arg0->timer % 20) == 0) {
            setMainMenuSceneModelAnimation(0, 0x2F);
            if ((arg0->timer / 20) == 3) {
                arg0->timer = 0;
                setCallbackTaskCallback(arg0, waitEndingSlashPhase1E);
            }
        }
    } else {
        arg0->posX += 0xFFFF0000;
        setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    }
}

void startEndingSlashRepeatAnim(s32 arg0) {
    struct {
        s32 ret;
        s32 pad;
    } locals;

    locals.ret = stepMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if (locals.ret == 1) {
        setCallbackTaskCallback(arg0, updateEndingSlashRepeatAnimThenVanish);
        setMainMenuSceneModelAnimation(0, 0x2F);
    }
}

void updateEndingSlashWaitBeforeRepeatAnim(EndingCreditsSlash *arg0) {
    struct {
        s32 ret;
        s32 pad;
    } locals;

    locals.ret = stepMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if (locals.ret == 1) {
        arg0->timer += 1;
        if (arg0->timer == 0x1E) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, startEndingSlashRepeatAnim);
            setMainMenuSceneModelAnimation(0, 0x2E);
        }
    }
}

void waitEndingSlashPhase1B(EndingCreditsSlash *arg0) {
    stepMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if (gEndingSequencePhase == 0x1B) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingSlashWaitBeforeRepeatAnim);
        setMainMenuSceneModelAnimation(0, 0x2D);
        spawnEndingCharacterVanishPoof(8, -0x40, 0, 0);
    }
}

void waitEndingSlashPhase1A(void *arg0) {
    addMainMenuSceneModelDrawCallback(0);
    if (gEndingSequencePhase == 0x1A) {
        setCallbackTaskCallback(arg0, waitEndingSlashPhase1B);
        setMainMenuSceneModelAnimation(0, 0x2C);
    }
}

void updateEndingSlashHandshakeLoopThird(EndingCreditsSlash *arg0) {
    if (gEndingActorHandshakeState == 4) {
        gEndingActorHandshakeState = 0;
        setMainMenuSceneModelAnimation(0, 0x2B);
    }
    if (gEndingActorHandshakeState == 0) {
        stepMainMenuSceneModelAnimation(0);
        if (stepMainMenuSceneModelAnimation(0) == 1) {
            gEndingActorHandshakeState += 1;
            if (++arg0->timer == 5) {
                setCallbackTaskCallback(arg0, waitEndingSlashPhase1A);
                arg0->timer = 0;
            }
        }
    }
    addMainMenuSceneModelDrawCallback(0);
}

void updateEndingSlashHandshakeLoopSecond(EndingCreditsSlash *arg0) {
    if (gEndingActorHandshakeState == 4) {
        gEndingActorHandshakeState = 0;
        setMainMenuSceneModelAnimation(0, 0x2B);
    }
    if (gEndingActorHandshakeState == 0) {
        stepMainMenuSceneModelAnimation(0);
        if (stepMainMenuSceneModelAnimation(0) == 1) {
            gEndingActorHandshakeState += 1;
            if (++arg0->timer == 5) {
                setCallbackTaskCallback(arg0, updateEndingSlashHandshakeLoopThird);
                arg0->timer = 0;
            }
        }
    }
    addMainMenuSceneModelDrawCallback(0);
}

void updateEndingSlashHandshakeLoopFirst(EndingCreditsSlash *arg0) {
    if (gEndingActorHandshakeState == 4) {
        gEndingActorHandshakeState = 0;
        setMainMenuSceneModelAnimation(0, 0x2B);
    }
    if (gEndingActorHandshakeState == 0) {
        if (stepMainMenuSceneModelAnimation(0) == 1) {
            gEndingActorHandshakeState += 1;
            if (++arg0->timer == 3) {
                setCallbackTaskCallback(arg0, updateEndingSlashHandshakeLoopSecond);
                arg0->timer = 0;
            }
        }
    }
    addMainMenuSceneModelDrawCallback(0);
}

void startEndingSlashHandshakeLoop(EndingCreditsSlash *arg0) {
    struct {
        s32 ret;
        s32 pad;
    } locals;

    locals.ret = stepMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if (locals.ret == 1) {
        gEndingActorHandshakeState++;
        setCallbackTaskCallback(arg0, updateEndingSlashHandshakeLoopFirst);
    }
}

void waitEndingSlashPhase19(void *arg0) {
    addMainMenuSceneModelDrawCallback(0);
    if (gEndingSequencePhase == 0x19) {
        setCallbackTaskCallback(arg0, startEndingSlashHandshakeLoop);
        setMainMenuSceneModelAnimation(0, 0x2A);
    }
}

void updateEndingSlashSlideLeftUntilPhase18(EndingCreditsSlash *arg0) {
    stepMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    arg0->posX += 0xFFFD0000;
    if (gEndingSequencePhase == 0x18) {
        setCallbackTaskCallback(arg0, waitEndingSlashPhase19);
    }
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
}

void waitEndingSlashPhase17(s32 arg0) {
    loopMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if (gEndingSequencePhase == 0x17) {
        setCallbackTaskCallback(arg0, updateEndingSlashSlideLeftUntilPhase18);
        setMainMenuSceneModelAnimation(0, 0x29);
    }
}

void updateEndingSlashSlideRightToIdle(EndingCreditsSlash *arg0) {
    loopMainMenuSceneModelAnimation(0);
    arg0->posX += 0x30000;
    if (arg0->posX >= 0xD0000) {
        arg0->posX = 0xD0000;
        setCallbackTaskCallback(arg0, waitEndingSlashPhase17);
        setMainMenuSceneModelAnimation(0, 0x24);
    }
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
}

void startEndingSlashIdleAfterAnim(s32 arg0) {
    if (stepMainMenuSceneModelAnimation(0) == 1) {
        setCallbackTaskCallback(arg0, updateEndingSlashSlideRightToIdle);
        setMainMenuSceneModelAnimation(0, 0);
    }
    addMainMenuSceneModelDrawCallback(0);
}

void updateEndingSlashWaitBeforeIdleAnim2(EndingCreditsSlash *arg0) {
    EndingCreditsSlash *temp = arg0;

    if (stepMainMenuSceneModelAnimation(0) == 1) {
        temp->timer += 1;
        if (temp->timer == 0xF) {
            temp->timer = 0;
            setCallbackTaskCallback(temp, startEndingSlashIdleAfterAnim);
            setMainMenuSceneModelAnimation(0, 0x23);
        }
    }
    addMainMenuSceneModelDrawCallback(0);
}

void updateEndingSlashWaitBeforeIdleAnim1(EndingCreditsSlash *arg0) {
    EndingCreditsSlash *temp = arg0;

    if (stepMainMenuSceneModelAnimation(0) == 1) {
        temp->timer += 1;
        if (temp->timer == 0xF) {
            temp->timer = 0;
            setCallbackTaskCallback(temp, updateEndingSlashWaitBeforeIdleAnim2);
            setMainMenuSceneModelAnimation(0, 0x22);
        }
    }
    addMainMenuSceneModelDrawCallback(0);
}

void updateEndingSlashHopLeftAnim(EndingCreditsSlash *arg0) {
    s32 unused;
    s32 sp20;
    s32 var_v0;

    sp20 = stepMainMenuSceneModelAnimation(0);
    if (arg0->timer < 5) {
        var_v0 = 1;
    } else {
        var_v0 = -1;
    }
    arg0->posX += 0xFFF80000;
    arg0->posY += var_v0 * 0x60000;
    arg0->timer = arg0->timer + 1;
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
    if (sp20 == 1) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingSlashWaitBeforeIdleAnim1);
        setMainMenuSceneModelAnimation(0, 0x21);
    }
}

void updateEndingSlashSlideRightBurst(EndingCreditsSlash *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(0);
    var_a1 = (arg0->posX += 0x40000);
    if (var_a1 >= 0x280000) {
        arg0->posX = 0x280000;
        setMainMenuSceneModelAnimation(0, 0x20);
        setCallbackTaskCallback(arg0, updateEndingSlashHopLeftAnim);
        gEndingSequencePhase = 0x15;
        gEndingCharacterEffectDoneFlags[0] = 1;
        spawnEndingSmallBurst(0xB, -0x4C);
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(0, var_a1, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
}

void waitEndingSlashRotationPhase14(EndingCreditsSlash *arg0) {
    loopMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if (arg0->timer < 0x10) {
        arg0->timer += 1;
        if (arg0->timer == 0x10) {
            gEndingSequencePhase = 0x13;
        }
    } else if (gEndingSequencePhase == 0x14) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingSlashSlideRightBurst);
    }
}

void updateEndingSlashSlideLeftSlowRotation(EndingCreditsSlash *arg0) {
    arg0->posX += -0x20000;
    arg0->timer += 1;
    if ((arg0->timer % 13) == 0) {
        gEndingTransitionRotationStep -= 1;
    }
    if (arg0->posX < -0x9FFFFF) {
        gEndingTransitionRotationStep = 2;
        arg0->timer = 0;
        arg0->posX = -0xA00000;
        setCallbackTaskCallback(arg0, waitEndingSlashRotationPhase14);
    }
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    loopMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
}

void updateEndingSlashWaitRotationStepTen(EndingCreditsSlash *arg0) {
    loopMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if (gEndingTransitionRotationStep != 0xA) {
        gEndingTransitionRotationStep -= 2;
    } else {
        arg0->timer += 1;
        if (arg0->timer == 1) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, updateEndingSlashSlideLeftSlowRotation);
        }
    }
}

void waitEndingSlashPhase12(EndingCreditsSlash *arg0) {
    EndingCreditsSlash *temp = arg0;

    loopMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallback(0);
    if (gEndingSequencePhase == 0x12) {
        temp->timer += 1;
        if (temp->timer == 0x1E) {
            temp->timer = 0;
            setCallbackTaskCallback(temp, updateEndingSlashWaitRotationStepTen);
        }
    }
}

void updateEndingSlashSlideRightSetPhase11(EndingCreditsSlash *arg0) {
    loopMainMenuSceneModelAnimation(0);
    arg0->posX += 0x60000;
    arg0->timer += 1;
    if (arg0->posX >= 0x300000) {
        arg0->timer = 0;
        arg0->posX = 0x300000;
        setCallbackTaskCallback(arg0, waitEndingSlashPhase12);
    }
    if (arg0->timer == 0x18) {
        gEndingSequencePhase = 0x11;
    }
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
}

void waitEndingSlashPhase10Aura(EndingCreditsSlash *arg0) {
    if (gEndingSequencePhase == 0x10) {
        arg0->posX = 0xFE700000;
        arg0->rotY = 0x400;
        setMainMenuSceneModelPosition(0, -0x1900000, arg0->posY, arg0->posZ);
        setMainMenuSceneModelRotation(0, arg0->rotX, arg0->rotY, arg0->rotZ);
        setCallbackTaskCallback(arg0, updateEndingSlashSlideRightSetPhase11);
        setMainMenuSceneModelAnimation(0, 0x1B);
        gEndingCharacterEffectDoneFlags[0] = 0;
        spawnEndingCharacterAura(-0x32, -0x30, 0, 0);
    }
}

void updateEndingSlashExitRightSetPhase0E(EndingCreditsSlash *arg0) {
    loopMainMenuSceneModelAnimation(0);
    arg0->posX += 0x60000;
    arg0->timer += 1;
    if (arg0->posX >= 0x1900000) {
        arg0->timer = 0;
        arg0->posX = 0x1900000;
        setCallbackTaskCallback(arg0, waitEndingSlashPhase10Aura);
        gEndingCharacterEffectDoneFlags[0] = 1;
    }
    if (arg0->timer == 1) {
        gEndingSequencePhase = 0xE;
    }
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(0);
}

void updateEndingSlashVanishBeforeExitRight(EndingCreditsSlash *arg0) {
    u16 temp;

    if (stepMainMenuSceneModelAnimation(0) == 1) {
        temp = ++arg0->timer;
        if ((u32) temp == 0x19) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, updateEndingSlashExitRightSetPhase0E);
            setMainMenuSceneModelAnimation(0, 0x1B);
            gEndingCharacterEffectDoneFlags[0] = 0;
            spawnEndingCharacterAura(-0x32, -0x30, 0, 0);
        } else if (temp == 2) {
            spawnEndingCharacterVanishPoof(5, -0x40, 0, 0);
        }
    }
    addMainMenuSceneModelDrawCallback(0);
}

void startEndingSlashVanishBeforeExitRight(s32 arg0) {
    if (stepMainMenuSceneModelAnimation(0) == 1) {
        setCallbackTaskCallback(arg0, updateEndingSlashVanishBeforeExitRight);
        setMainMenuSceneModelAnimation(0, 0x1A);
    }
    addMainMenuSceneModelDrawCallback(0);
}

void updateEndingSlashWaitRisingStar(EndingCreditsSlash *arg0) {
    if (stepMainMenuSceneModelAnimation(0) == 1) {
        arg0->timer += 1;
        if (arg0->timer == 0x32) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, startEndingSlashVanishBeforeExitRight);
            setMainMenuSceneModelAnimation(0, 0x19);
        }
    } else {
        arg0->timer += 1;
        if (arg0->timer == 0x23) {
            arg0->timer = 0;
            createCallbackTask(initEndingSlashRisingStar, 0, 0x64);
        }
    }
    addMainMenuSceneModelDrawCallback(0);
}

void waitEndingSlashBeforeRisingStar(EndingCreditsSlash *arg0) {
    addMainMenuSceneModelDrawCallback(0);
    arg0->timer += 1;
    if (arg0->timer == 0x19) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingSlashWaitRisingStar);
        setMainMenuSceneModelAnimation(0, 0x18);
    }
}

void updateEndingSlashRepeatAnim17(EndingCreditsSlash *arg0) {
    if (stepMainMenuSceneModelAnimation(0) == 1) {
        arg0->timer += 1;
        if (arg0->timer < 0xA) {
            setMainMenuSceneModelAnimation(0, 0x17);
        } else {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, waitEndingSlashBeforeRisingStar);
        }
    }
    addMainMenuSceneModelDrawCallback(0);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void startEndingSlashRepeatAnim17(s32 arg0) {
    if (stepMainMenuSceneModelAnimation(0) == 1) {
        setCallbackTaskCallback(arg0, updateEndingSlashRepeatAnim17);
        setMainMenuSceneModelAnimation(0, 0x17);
    }
    addMainMenuSceneModelDrawCallback(0);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void waitEndingSlashPhase0D(EndingCreditsSlash *arg0) {
    if (gEndingSequencePhase == 0xD) {
        setCallbackTaskCallback(arg0, startEndingSlashRepeatAnim17);
        setMainMenuSceneModelAnimation(0, 0x16);
        arg0->unk2C = 0x16;
    }
    addMainMenuSceneModelDrawCallback(0);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void updateEndingSlashAfterPhase0DAnim(EndingCreditsSlash *arg0) {
    if (stepMainMenuSceneModelAnimation(0) == 1) {
        gEndingSequencePhase = 0xB;
        setCallbackTaskCallback(arg0, waitEndingSlashPhase0D);
    } else {
        arg0->timer += 1;
        if (arg0->timer == 3) {
            spawnEndingSmallBurst(6, -0x42);
        }
    }
    addMainMenuSceneModelDrawCallback(0);
    addEndingActorShadowRenderCallback(&gEndingActorShadow);
}

void updateEndingSlashLongWaitSetShadow(EndingCreditsSlash *arg0) {
    EndingCreditsSlash *temp_a2;

    temp_a2 = arg0;
    arg0->timer++;
    if (arg0->timer == 6) {
        gEndingSequencePhase = 9;
    }
    if (arg0->timer == 0x64) {
        gEndingSequencePhase = 0xA;
        setCallbackTaskCallback(temp_a2, updateEndingSlashAfterPhase0DAnim);
        setMainMenuSceneModelAnimation(0, 0xE);
        temp_a2->timer = 0;
        gEndingActorShadow.actorId = 0;
        gEndingActorShadow.unkC = 0xB;
        gEndingActorShadow.posY = (s32)0xFFE80000;
        gEndingCharacterEffectDoneFlags[0] = 1;
    } else {
        loopMainMenuSceneModelAnimation(0);
    }
    addMainMenuSceneModelDrawCallback(0);
}

void updateEndingSlashSlideLeftToAura(EndingCreditsSlash *arg0) {
    s32 temp_a1;

    temp_a1 = (arg0->posX += 0xFFFA0000);
    if (temp_a1 < 0x580001) {
        arg0->posX = 0x580000;
        setCallbackTaskCallback(arg0, updateEndingSlashLongWaitSetShadow);
        setMainMenuSceneModelAnimation(0, 0xD);
        arg0->rotY = 0x400;
        setMainMenuSceneModelRotation(0, arg0->rotX, arg0->rotY, arg0->rotZ);
        gEndingSequencePhase = 8;
        gEndingCharacterEffectDoneFlags[0] = 0;
        spawnEndingCharacterAura(0x1C, -0x38, 0, 1);
    } else {
        setMainMenuSceneModelPosition(0, temp_a1, arg0->posY, arg0->posZ);
        loopMainMenuSceneModelAnimation(0);
    }
    addMainMenuSceneModelDrawCallback(0);
}


void updateEndingSlashWaitPhase5To7(EndingCreditsSlash *arg0) {
    if (stepMainMenuSceneModelAnimation(0) == 1) {
        if (gEndingSequencePhase == 5) {
            arg0->timer += 1;
            if (arg0->timer == 0xA) {
                gEndingSequencePhase = 6;
            }
        }
        if (gEndingSequencePhase == 7) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, updateEndingSlashSlideLeftToAura);
            setMainMenuSceneModelAnimation(0, 0);
        }
    }
    addMainMenuSceneModelDrawCallback(0);
}

void waitEndingSlashPhase5(s32 arg0) {
    stepMainMenuSceneModelAnimation(0);
    if (gEndingSequencePhase == 5) {
        setCallbackTaskCallback(arg0, updateEndingSlashWaitPhase5To7);
        setMainMenuSceneModelAnimation(0, 2);
        spawnEndingDelayedSparkle(0x55, -0x62, 0);
    }
    addMainMenuSceneModelDrawCallback(0);
}

void waitEndingSlashPhase3(EndingCreditsSlash *arg0) {
    stepMainMenuSceneModelAnimation(0);
    if (gEndingSequencePhase == 3) {
        setCallbackTaskCallback(arg0, waitEndingSlashPhase5);
        setMainMenuSceneModelAnimation(0, 2);
        spawnEndingDelayedSparkle(0x55, -0x62, 0);
        arg0->timer = 0;
    }
    addMainMenuSceneModelDrawCallback(0);
}

void updateEndingSlashEnterFromRight(EndingCreditsSlash *arg0) {
    s32 temp_a1;

    temp_a1 = (arg0->posX += 0xFFFD8FD8);
    if (temp_a1 < 0x900001) {
        arg0->posX = 0x900000;
        setCallbackTaskCallback(arg0, waitEndingSlashPhase3);
        setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
        setMainMenuSceneModelAnimation(0, 1);
        spawnEndingPhaseAdvanceSparkle(0x50, -0x68);
    } else {
        if (!temp_a1) {
        }
        setMainMenuSceneModelPosition(0, temp_a1, arg0->posY, arg0->posZ);
        loopMainMenuSceneModelAnimation(0);
    }
    addMainMenuSceneModelDrawCallback(0);
}

void waitEndingSlashPhase2(void *arg0) {
    if (gEndingSequencePhase == 2) {
        setCallbackTaskCallback(arg0, updateEndingSlashEnterFromRight);
    }
}

void initEndingCreditsSlash(EndingCreditsSlash *arg0) {
    arg0->posX = 0x1900000;
    arg0->posY = 0;
    arg0->posZ = 0;
    arg0->rotX = 0;
    arg0->rotY = 0xC00;
    arg0->rotZ = 0;
    initMainMenuSceneModel(0, 0);
    setMainMenuSceneModelAnimation(0, 0);
    setMainMenuSceneModelPosition(0, arg0->posX, arg0->posY, arg0->posZ);
    setMainMenuSceneModelRotation(0, arg0->rotX, arg0->rotY, arg0->rotZ);
    setCallbackTaskCallback(arg0, waitEndingSlashPhase2);
}
