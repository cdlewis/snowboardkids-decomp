#include "common.h"
#include "callback_task_scheduler.h"
#include "menu_transition_effects.h"
#include "ending_credits_actor_0.h"
#include "main_menu_scene_actor_4.h"
#include "main_menu_scene_model.h"
#include "main_menu_scene_renderer.h"

struct EndingCreditsActor0 {
    char pad0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u16 unk2A;
    /* 0x2C */ s16 unk2C;
};

extern MainMenuSceneActorShadow gEndingActorShadow;

extern u16 gEndingSequencePhase;
extern u8 gEndingActorHandshakeState;
extern u8 gMenuTransitionRotationStep;
extern u8 gEndingCharacterEffectDone;

void noopEndingCreditsActor0(void) {
}

void updateEndingActor0FinalPose(EndingCreditsActor0 *arg0) {
    EndingCreditsActor0 *temp = arg0;

    func_80041FB4(0);
    func_800428C8(0);
    if (temp->unk2A < 0x96) {
        temp->unk2A += 1;
        if (temp->unk2A == 0x96) {
            gEndingSequencePhase = 0x42;
        }
    }
}

void updateEndingActor0StartFinalPose(EndingCreditsActor0 *arg0) {
    struct {
        s32 ret;
        s32 pad;
    } l;

    l.ret = func_80041FB4(0);
    func_800428C8(0);
    if (l.ret == 1) {
        arg0->unk2A += 1;
        if (arg0->unk2A == 0x41) {
            arg0->unk2A = 0;
            setCallbackTaskCallback(arg0, updateEndingActor0FinalPose);
            func_80041DD4(0, 0x56);
            gEndingSequencePhase = 0x41;
            arg0->unk26 = 0xC00;
            func_800420FC(0, arg0->unk24, arg0->unk26, arg0->unk28);
        }
    }
}

void waitEndingActor0Phase40(EndingCreditsActor0 *arg0) {
    func_80041FB4(0);
    func_800428C8(0);
    if (gEndingSequencePhase == 0x40) {
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, updateEndingActor0StartFinalPose);
        func_80041DD4(0, 0x55);
    }
}

void updateEndingActor0Phase40Prep(EndingCreditsActor0 *arg0) {
    EndingCreditsActor0 *temp = arg0;

    if (func_80041FB4(0) == 1) {
        temp->unk2A += 1;
        if (temp->unk2A == 0x1E) {
            temp->unk2A = 0;
            setCallbackTaskCallback(temp, waitEndingActor0Phase40);
            func_80041DD4(0, 0x54);
            spawnEndingPhaseAdvanceSparkle(0xA, -0x64);
        }
    }
    func_800428C8(0);
}

void updateEndingActor0WaitPhase3FAnim(EndingCreditsActor0 *arg0) {
    struct { s32 ret; s32 pad; } l;
    l.ret = func_80041FB4(0);
    func_800428C8(0);
    if (l.ret == 1) {
        arg0->unk2A += 1;
        if (arg0->unk2A == 0x14) {
            arg0->unk2A = 0;
            setCallbackTaskCallback(arg0, updateEndingActor0Phase40Prep);
            func_80041DD4(0, 0x53);
        }
    }
}

void updateEndingActor0SpinUntilPhase3F(EndingCreditsActor0 *arg0) {
    if (func_80041FB4(0) == 0) {
        arg0->unk18 += 0x18000;
        func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    }
    if (gEndingSequencePhase == 0x3F) {
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, updateEndingActor0WaitPhase3FAnim);
        func_80041DD4(0, 0x22);
    }
    func_800428C8(0);
}

void updateEndingActor0TumbleToPhase3C(EndingCreditsActor0 *arg0) {
    s32 unused;
    s32 sp20;
    s32 var_v0;

    sp20 = func_80041FB4(0);
    if (arg0->unk2A < 5) {
        var_v0 = 1;
    } else {
        var_v0 = -1;
    }
    arg0->unk18 += 0x80000;
    arg0->unk1C += var_v0 * 0x80000;
    arg0->unk2A = arg0->unk2A + 1;
    if (arg0->unk2A == 2) {
        gEndingSequencePhase = 0x3C;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
    if (sp20 == 1) {
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, updateEndingActor0SpinUntilPhase3F);
        func_80041DD4(0, 0x21);
    }
}

void updateEndingActor0ExitUntilPhase3B(EndingCreditsActor0 *arg0) {
    func_80042034(0);
    arg0->unk18 += 0xFFF70000;
    if (gEndingSequencePhase == 0x3B) {
        setCallbackTaskCallback(arg0, updateEndingActor0TumbleToPhase3C);
        func_80041DD4(0, 0x20);
        arg0->unk26 = 0xC00;
        func_800420FC(0, arg0->unk24, arg0->unk26, arg0->unk28);
        gEndingCharacterEffectDone = 1;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void waitEndingActor0Phase3A(s32 arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (gEndingSequencePhase == 0x3A) {
        setCallbackTaskCallback(arg0, updateEndingActor0ExitUntilPhase3B);
    }
}

void updateEndingActor0SlowRotationWipe(EndingCreditsActor0 *arg0) {
    func_80042034(0);
    func_800428C8(0);
    arg0->unk2A += 1;
    if (gMenuTransitionRotationStep >= 3) {
        if (arg0->unk2A % 15 == 0) {
            gMenuTransitionRotationStep--;
            if (gMenuTransitionRotationStep == 2) {
                arg0->unk2A = 0;
            }
        }
    } else if (arg0->unk2A == 0x10) {
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, waitEndingActor0Phase3A);
        gEndingSequencePhase = 0x39;
    }
}

void updateEndingActor0SlideToCenter(EndingCreditsActor0 *arg0) {
    EndingCreditsActor0 *temp = arg0;

    func_80042034(0);
    temp->unk18 += 0x24000;
    if (temp->unk18 >= 0x500000) {
        temp->unk18 = 0x500000;
        setCallbackTaskCallback(temp, updateEndingActor0SlowRotationWipe);
    }
    func_8004209C(0, temp->unk18, temp->unk1C, temp->unk20);
    func_800428C8(0);
}

void waitEndingActor0Phase37(EndingCreditsActor0 *arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (gEndingSequencePhase == 0x37) {
        arg0->unk2A += 1;
        if (arg0->unk2A == 0x1E) {
            arg0->unk2A = 0;
            setCallbackTaskCallback(arg0, updateEndingActor0SlideToCenter);
            gEndingSequencePhase = 0x38;
        }
    }
}

void updateEndingActor0SlideLeftFromFarRight(EndingCreditsActor0 *arg0) {
    s32 var_a1;
    EndingCreditsActor0 *new_var;

    func_80042034(0);
    new_var = arg0;
    var_a1 = (new_var->unk18 += -0x48000);
    if (var_a1 < -0x1FFFFF) {
        new_var->unk18 = -0x200000;
        setCallbackTaskCallback(new_var, waitEndingActor0Phase37);
        var_a1 = arg0->unk18;
    } else if ((var_a1 < 0xD00001) && (gEndingSequencePhase == 0x34)) {
        gEndingSequencePhase = 0x35;
        var_a1 = arg0->unk18;
    }
    func_8004209C(0, var_a1, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void waitEndingActor0Phase34(EndingCreditsActor0 *arg0) {
    if (gEndingSequencePhase == 0x34) {
        setCallbackTaskCallback(arg0, updateEndingActor0SlideLeftFromFarRight);
        arg0->unk18 = 0x1900000;
        arg0->unk20 = 0;
        gEndingCharacterEffectDone = 0;
        spawnEndingCharacterAura(0x1C, -0x30, 0, 1);
    }
}

void updateEndingActor0DashOffLeft(EndingCreditsActor0 *arg0) {
    func_80042034(0);
    arg0->unk18 += 0xFFFB8000;
    if (arg0->unk18 < (s32)0xFE700001) {
        arg0->unk18 = 0xFE700000;
        setCallbackTaskCallback(arg0, waitEndingActor0Phase34);
        gEndingCharacterEffectDone = 1;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void updateEndingActor0VanishRetreat(EndingCreditsActor0 *arg0) {
    s32 unused;
    s32 sp20;

    sp20 = func_80041FB4(0);
    arg0->unk18 += 0xFFF00000;
    arg0->unk2A += 1;
    if (arg0->unk2A < 3) {
        arg0->unk1C += 0x140000;
    } else {
        arg0->unk1C += 0xFFEC0000;
    }
    if (arg0->unk2A == 2) {
        gEndingSequencePhase = 0x30;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
    if (sp20 == 1) {
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, updateEndingActor0DashOffLeft);
        arg0->unk1C = 0;
        func_8004209C(0, arg0->unk18, 0, arg0->unk20);
        func_80041DD4(0, 0x1B);
        gEndingCharacterEffectDone = 0;
        spawnEndingCharacterAura(0x1C, -0x30, 0, 1);
    }
}

void waitEndingActor0Phase2F(void *arg0) {
    func_800428C8(0);
    if (gEndingSequencePhase == 0x2F) {
        setCallbackTaskCallback(arg0, updateEndingActor0VanishRetreat);
        func_80041DD4(0, 0x4E);
        spawnEndingCharacterVanishPoof(8, -0x40, 0, 0);
    }
}

void updateEndingActor0DriftAfterPhase2D(EndingCreditsActor0 *arg0) {
    func_80042034(0);
    arg0->unk18 += 0x2000;
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
    if (gEndingSequencePhase == 0x2D) {
        setCallbackTaskCallback(arg0, waitEndingActor0Phase2F);
    }
}

void updateEndingActor0RunLeftThenPhase2B(EndingCreditsActor0 *arg0) {
    if (func_80041FB4(0) == 0) {
        arg0->unk18 += 0xFFF60000;
    } else {
        setCallbackTaskCallback(arg0, updateEndingActor0DriftAfterPhase2D);
        func_80041DD4(0, 0x44);
        arg0->unk20 = 0xFFFF0000;
        gEndingSequencePhase = 0x2B;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void updateEndingActor0SlideFarLeft(EndingCreditsActor0 *arg0) {
    func_80042034(0);
    arg0->unk18 += 0xFFFC8000;
    if (arg0->unk18 < (s32)0xFFD00001) {
        arg0->unk18 = 0xFFA00000;
        setCallbackTaskCallback(arg0, updateEndingActor0RunLeftThenPhase2B);
        func_80041DD4(0, 0x43);
        arg0->unk2A = 0;
        gEndingCharacterEffectDone = 1;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void waitEndingActor0Phase2A(s32 arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (gEndingSequencePhase == 0x2A) {
        setCallbackTaskCallback(arg0, updateEndingActor0SlideFarLeft);
    }
}

void updateEndingActor0SlideRightToPose(EndingCreditsActor0 *arg0) {
    s32 var_a1;

    func_80042034(0);
    var_a1 = (arg0->unk18 += 0x2D000);
    if (var_a1 >= 0x800000) {
        arg0->unk18 = 0x800000;
        setCallbackTaskCallback(arg0, waitEndingActor0Phase2A);
        gEndingCharacterEffectDone = 1;
        spawnEndingCharacterLoopingSparkle(-0x10, -0x4A, 0);
        var_a1 = arg0->unk18;
    }
    func_8004209C(0, var_a1, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void waitEndingActor0Phase28(s32 arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (gEndingSequencePhase == 0x28) {
        setCallbackTaskCallback(arg0, updateEndingActor0SlideRightToPose);
    }
}

void updateEndingActor0SlideLeftToMarker(EndingCreditsActor0 *arg0) {
    func_80042034(0);
    arg0->unk18 += 0xFFFDA000;
    if (arg0->unk18 < (s32)0xFF700001) {
        arg0->unk18 = 0xFF700000;
        setCallbackTaskCallback(arg0, waitEndingActor0Phase28);
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void updateEndingActor0SlideRightToMarker(EndingCreditsActor0 *arg0) {
    func_80042034(0);
    arg0->unk18 += 0x1D000;
    if (arg0->unk18 >= (s32)0xFFCE0000) {
        arg0->unk18 = 0xFFCE0000;
        setCallbackTaskCallback(arg0, updateEndingActor0SlideLeftToMarker);
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void startEndingActor0Phase27Slide(void *arg0) {
    func_80042034(0);
    func_800428C8(0);
    if ((gEndingSequencePhase = 0x27) != 0) {
        setCallbackTaskCallback(arg0, updateEndingActor0SlideRightToMarker);
    }
}

void updateEndingActor0SlideLeftSetPhase26(EndingCreditsActor0 *arg0) {
    func_80042034(0);
    arg0->unk18 += 0xFFFD8000;
    if (arg0->unk18 < (s32)0xFF700001) {
        arg0->unk18 = 0xFF700000;
        setCallbackTaskCallback(arg0, startEndingActor0Phase27Slide);
        gEndingSequencePhase = 0x26;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void noopEndingCreditsActor0Alt(void) {
}

void updateEndingActor0DiagonalSlideSetPhase24(EndingCreditsActor0 *arg0) {
    func_80042034(0);
    arg0->unk18 += 0x28000;
    arg0->unk20 += 0xFFFE0000;
    if (arg0->unk18 >= (s32)0xFFC80000) {
        arg0->unk18 = 0xFFC80000;
        setCallbackTaskCallback(arg0, updateEndingActor0SlideLeftSetPhase26);
        gEndingSequencePhase = 0x24;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void waitEndingActor0BeforeDiagonalSlide(EndingCreditsActor0 *arg0) {
    func_80042034(0);
    func_800428C8(0);
    arg0->unk2A += 1;
    if (arg0->unk2A == 0x32) {
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, updateEndingActor0DiagonalSlideSetPhase24);
    }
}

void updateEndingActor0SlideRightSetPhase23(EndingCreditsActor0 *arg0) {
    func_80042034(0);
    arg0->unk18 += 0x28000;
    if (arg0->unk18 >= (s32)0xFF700000) {
        arg0->unk18 = 0xFF700000;
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, waitEndingActor0BeforeDiagonalSlide);
        gEndingSequencePhase = 0x23;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void waitEndingActor0Phase22Aura(void *arg0) {
    if (gEndingSequencePhase == 0x22) {
        setCallbackTaskCallback(arg0, updateEndingActor0SlideRightSetPhase23);
        gEndingCharacterEffectDone = 0;
        spawnEndingCharacterAura(0x1C, -0x30, 0, 1);
    }
}

void updateEndingActor0ExitLeftAfterPhase22(EndingCreditsActor0 *arg0) {
    func_80042034(0);
    arg0->unk18 += 0xFFFA0000;
    if (arg0->unk18 < (s32)0xFE700001) {
        arg0->unk18 = 0xFE700000;
        setCallbackTaskCallback(arg0, waitEndingActor0Phase22Aura);
        gEndingCharacterEffectDone = 1;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void updateEndingActor0AfterVanishWait(EndingCreditsActor0 *arg0) {
    struct {
        s32 ret;
        s32 pad;
    } l;

    l.ret = func_80041FB4(0);
    func_800428C8(0);
    if (l.ret == 1) {
        arg0->unk2A += 1;
        if (arg0->unk2A == 0x14) {
            arg0->unk2A = 0;
            setCallbackTaskCallback(arg0, updateEndingActor0ExitLeftAfterPhase22);
            func_80041DD4(0, 0x1B);
            arg0->unk26 = 0xC00;
            func_800420FC(0, arg0->unk24, arg0->unk26, arg0->unk28);
            gEndingSequencePhase = 0x1F;
            gEndingCharacterEffectDone = 0;
            spawnEndingCharacterAura(0x1C, -0x30, 0, 1);
        }
    }
}

void waitEndingActor0Phase1E(void *arg0) {
    func_800428C8(0);
    if (gEndingSequencePhase == 0x1E) {
        setCallbackTaskCallback(arg0, updateEndingActor0AfterVanishWait);
        func_80041DD4(0, 0x2D);
        spawnEndingCharacterVanishPoof(8, -0x40, 0, 0);
    }
}

void updateEndingActor0RepeatAnimThenVanish(EndingCreditsActor0 *arg0) {
    s32 sp24;
    s32 sp20;

    sp20 = func_80041FB4(0);
    func_800428C8(0);
    if (sp20 == 1) {
        arg0->unk2A++;
        if ((arg0->unk2A % 20) == 0) {
            func_80041DD4(0, 0x2F);
            if ((arg0->unk2A / 20) == 3) {
                arg0->unk2A = 0;
                setCallbackTaskCallback(arg0, waitEndingActor0Phase1E);
            }
        }
    } else {
        arg0->unk18 += 0xFFFF0000;
        func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    }
}

void startEndingActor0RepeatAnim(s32 arg0) {
    struct {
        s32 ret;
        s32 pad;
    } locals;

    locals.ret = func_80041FB4(0);
    func_800428C8(0);
    if (locals.ret == 1) {
        setCallbackTaskCallback(arg0, updateEndingActor0RepeatAnimThenVanish);
        func_80041DD4(0, 0x2F);
    }
}

void updateEndingActor0WaitBeforeRepeatAnim(EndingCreditsActor0 *arg0) {
    struct {
        s32 ret;
        s32 pad;
    } locals;

    locals.ret = func_80041FB4(0);
    func_800428C8(0);
    if (locals.ret == 1) {
        arg0->unk2A += 1;
        if (arg0->unk2A == 0x1E) {
            arg0->unk2A = 0;
            setCallbackTaskCallback(arg0, startEndingActor0RepeatAnim);
            func_80041DD4(0, 0x2E);
        }
    }
}

void waitEndingActor0Phase1B(EndingCreditsActor0 *arg0) {
    func_80041FB4(0);
    func_800428C8(0);
    if (gEndingSequencePhase == 0x1B) {
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, updateEndingActor0WaitBeforeRepeatAnim);
        func_80041DD4(0, 0x2D);
        spawnEndingCharacterVanishPoof(8, -0x40, 0, 0);
    }
}

void waitEndingActor0Phase1A(void *arg0) {
    func_800428C8(0);
    if (gEndingSequencePhase == 0x1A) {
        setCallbackTaskCallback(arg0, waitEndingActor0Phase1B);
        func_80041DD4(0, 0x2C);
    }
}

void updateEndingActor0HandshakeLoopThird(EndingCreditsActor0 *arg0) {
    if (gEndingActorHandshakeState == 4) {
        gEndingActorHandshakeState = 0;
        func_80041DD4(0, 0x2B);
    }
    if (gEndingActorHandshakeState == 0) {
        func_80041FB4(0);
        if (func_80041FB4(0) == 1) {
            gEndingActorHandshakeState += 1;
            if (++arg0->unk2A == 5) {
                setCallbackTaskCallback(arg0, waitEndingActor0Phase1A);
                arg0->unk2A = 0;
            }
        }
    }
    func_800428C8(0);
}

void updateEndingActor0HandshakeLoopSecond(EndingCreditsActor0 *arg0) {
    if (gEndingActorHandshakeState == 4) {
        gEndingActorHandshakeState = 0;
        func_80041DD4(0, 0x2B);
    }
    if (gEndingActorHandshakeState == 0) {
        func_80041FB4(0);
        if (func_80041FB4(0) == 1) {
            gEndingActorHandshakeState += 1;
            if (++arg0->unk2A == 5) {
                setCallbackTaskCallback(arg0, updateEndingActor0HandshakeLoopThird);
                arg0->unk2A = 0;
            }
        }
    }
    func_800428C8(0);
}

void updateEndingActor0HandshakeLoopFirst(EndingCreditsActor0 *arg0) {
    if (gEndingActorHandshakeState == 4) {
        gEndingActorHandshakeState = 0;
        func_80041DD4(0, 0x2B);
    }
    if (gEndingActorHandshakeState == 0) {
        if (func_80041FB4(0) == 1) {
            gEndingActorHandshakeState += 1;
            if (++arg0->unk2A == 3) {
                setCallbackTaskCallback(arg0, updateEndingActor0HandshakeLoopSecond);
                arg0->unk2A = 0;
            }
        }
    }
    func_800428C8(0);
}

void startEndingActor0HandshakeLoop(EndingCreditsActor0 *arg0) {
    struct {
        s32 ret;
        s32 pad;
    } locals;

    locals.ret = func_80041FB4(0);
    func_800428C8(0);
    if (locals.ret == 1) {
        gEndingActorHandshakeState++;
        setCallbackTaskCallback(arg0, updateEndingActor0HandshakeLoopFirst);
    }
}

void waitEndingActor0Phase19(void *arg0) {
    func_800428C8(0);
    if (gEndingSequencePhase == 0x19) {
        setCallbackTaskCallback(arg0, startEndingActor0HandshakeLoop);
        func_80041DD4(0, 0x2A);
    }
}

void updateEndingActor0SlideLeftUntilPhase18(EndingCreditsActor0 *arg0) {
    func_80041FB4(0);
    func_800428C8(0);
    arg0->unk18 += 0xFFFD0000;
    if (gEndingSequencePhase == 0x18) {
        setCallbackTaskCallback(arg0, waitEndingActor0Phase19);
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
}

void waitEndingActor0Phase17(s32 arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (gEndingSequencePhase == 0x17) {
        setCallbackTaskCallback(arg0, updateEndingActor0SlideLeftUntilPhase18);
        func_80041DD4(0, 0x29);
    }
}

void updateEndingActor0SlideRightToIdle(EndingCreditsActor0 *arg0) {
    func_80042034(0);
    arg0->unk18 += 0x30000;
    if (arg0->unk18 >= 0xD0000) {
        arg0->unk18 = 0xD0000;
        setCallbackTaskCallback(arg0, waitEndingActor0Phase17);
        func_80041DD4(0, 0x24);
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void startEndingActor0IdleAfterAnim(s32 arg0) {
    if (func_80041FB4(0) == 1) {
        setCallbackTaskCallback(arg0, updateEndingActor0SlideRightToIdle);
        func_80041DD4(0, 0);
    }
    func_800428C8(0);
}

void updateEndingActor0WaitBeforeIdleAnim2(EndingCreditsActor0 *arg0) {
    EndingCreditsActor0 *temp = arg0;

    if (func_80041FB4(0) == 1) {
        temp->unk2A += 1;
        if (temp->unk2A == 0xF) {
            temp->unk2A = 0;
            setCallbackTaskCallback(temp, startEndingActor0IdleAfterAnim);
            func_80041DD4(0, 0x23);
        }
    }
    func_800428C8(0);
}

void updateEndingActor0WaitBeforeIdleAnim1(EndingCreditsActor0 *arg0) {
    EndingCreditsActor0 *temp = arg0;

    if (func_80041FB4(0) == 1) {
        temp->unk2A += 1;
        if (temp->unk2A == 0xF) {
            temp->unk2A = 0;
            setCallbackTaskCallback(temp, updateEndingActor0WaitBeforeIdleAnim2);
            func_80041DD4(0, 0x22);
        }
    }
    func_800428C8(0);
}

void updateEndingActor0HopLeftAnim(EndingCreditsActor0 *arg0) {
    s32 unused;
    s32 sp20;
    s32 var_v0;

    sp20 = func_80041FB4(0);
    if (arg0->unk2A < 5) {
        var_v0 = 1;
    } else {
        var_v0 = -1;
    }
    arg0->unk18 += 0xFFF80000;
    arg0->unk1C += var_v0 * 0x60000;
    arg0->unk2A = arg0->unk2A + 1;
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
    if (sp20 == 1) {
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, updateEndingActor0WaitBeforeIdleAnim1);
        func_80041DD4(0, 0x21);
    }
}

void updateEndingActor0SlideRightBurst(EndingCreditsActor0 *arg0) {
    s32 var_a1;

    func_80042034(0);
    var_a1 = (arg0->unk18 += 0x40000);
    if (var_a1 >= 0x280000) {
        arg0->unk18 = 0x280000;
        func_80041DD4(0, 0x20);
        setCallbackTaskCallback(arg0, updateEndingActor0HopLeftAnim);
        gEndingSequencePhase = 0x15;
        gEndingCharacterEffectDone = 1;
        spawnEndingSmallBurst(0xB, -0x4C);
        var_a1 = arg0->unk18;
    }
    func_8004209C(0, var_a1, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void waitEndingActor0RotationPhase14(EndingCreditsActor0 *arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (arg0->unk2A < 0x10) {
        arg0->unk2A += 1;
        if (arg0->unk2A == 0x10) {
            gEndingSequencePhase = 0x13;
        }
    } else if (gEndingSequencePhase == 0x14) {
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, updateEndingActor0SlideRightBurst);
    }
}

void updateEndingActor0SlideLeftSlowRotation(EndingCreditsActor0 *arg0) {
    arg0->unk18 += -0x20000;
    arg0->unk2A += 1;
    if ((arg0->unk2A % 13) == 0) {
        gMenuTransitionRotationStep -= 1;
    }
    if (arg0->unk18 < -0x9FFFFF) {
        gMenuTransitionRotationStep = 2;
        arg0->unk2A = 0;
        arg0->unk18 = -0xA00000;
        setCallbackTaskCallback(arg0, waitEndingActor0RotationPhase14);
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_80042034(0);
    func_800428C8(0);
}

void updateEndingActor0WaitRotationStepTen(EndingCreditsActor0 *arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (gMenuTransitionRotationStep != 0xA) {
        gMenuTransitionRotationStep -= 2;
    } else {
        arg0->unk2A += 1;
        if (arg0->unk2A == 1) {
            arg0->unk2A = 0;
            setCallbackTaskCallback(arg0, updateEndingActor0SlideLeftSlowRotation);
        }
    }
}

void waitEndingActor0Phase12(EndingCreditsActor0 *arg0) {
    EndingCreditsActor0 *temp = arg0;

    func_80042034(0);
    func_800428C8(0);
    if (gEndingSequencePhase == 0x12) {
        temp->unk2A += 1;
        if (temp->unk2A == 0x1E) {
            temp->unk2A = 0;
            setCallbackTaskCallback(temp, updateEndingActor0WaitRotationStepTen);
        }
    }
}

void updateEndingActor0SlideRightSetPhase11(EndingCreditsActor0 *arg0) {
    func_80042034(0);
    arg0->unk18 += 0x60000;
    arg0->unk2A += 1;
    if (arg0->unk18 >= 0x300000) {
        arg0->unk2A = 0;
        arg0->unk18 = 0x300000;
        setCallbackTaskCallback(arg0, waitEndingActor0Phase12);
    }
    if (arg0->unk2A == 0x18) {
        gEndingSequencePhase = 0x11;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void waitEndingActor0Phase10Aura(EndingCreditsActor0 *arg0) {
    if (gEndingSequencePhase == 0x10) {
        arg0->unk18 = 0xFE700000;
        arg0->unk26 = 0x400;
        func_8004209C(0, -0x1900000, arg0->unk1C, arg0->unk20);
        func_800420FC(0, arg0->unk24, arg0->unk26, arg0->unk28);
        setCallbackTaskCallback(arg0, updateEndingActor0SlideRightSetPhase11);
        func_80041DD4(0, 0x1B);
        gEndingCharacterEffectDone = 0;
        spawnEndingCharacterAura(-0x32, -0x30, 0, 0);
    }
}

void updateEndingActor0ExitRightSetPhase0E(EndingCreditsActor0 *arg0) {
    func_80042034(0);
    arg0->unk18 += 0x60000;
    arg0->unk2A += 1;
    if (arg0->unk18 >= 0x1900000) {
        arg0->unk2A = 0;
        arg0->unk18 = 0x1900000;
        setCallbackTaskCallback(arg0, waitEndingActor0Phase10Aura);
        gEndingCharacterEffectDone = 1;
    }
    if (arg0->unk2A == 1) {
        gEndingSequencePhase = 0xE;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void updateEndingActor0VanishBeforeExitRight(EndingCreditsActor0 *arg0) {
    u16 temp;

    if (func_80041FB4(0) == 1) {
        temp = ++arg0->unk2A;
        if ((u32) temp == 0x19) {
            arg0->unk2A = 0;
            setCallbackTaskCallback(arg0, updateEndingActor0ExitRightSetPhase0E);
            func_80041DD4(0, 0x1B);
            gEndingCharacterEffectDone = 0;
            spawnEndingCharacterAura(-0x32, -0x30, 0, 0);
        } else if (temp == 2) {
            spawnEndingCharacterVanishPoof(5, -0x40, 0, 0);
        }
    }
    func_800428C8(0);
}

void startEndingActor0VanishBeforeExitRight(s32 arg0) {
    if (func_80041FB4(0) == 1) {
        setCallbackTaskCallback(arg0, updateEndingActor0VanishBeforeExitRight);
        func_80041DD4(0, 0x1A);
    }
    func_800428C8(0);
}

void updateEndingActor0WaitRisingStar(EndingCreditsActor0 *arg0) {
    if (func_80041FB4(0) == 1) {
        arg0->unk2A += 1;
        if (arg0->unk2A == 0x32) {
            arg0->unk2A = 0;
            setCallbackTaskCallback(arg0, startEndingActor0VanishBeforeExitRight);
            func_80041DD4(0, 0x19);
        }
    } else {
        arg0->unk2A += 1;
        if (arg0->unk2A == 0x23) {
            arg0->unk2A = 0;
            createCallbackTask(initEndingRisingStar, 0, 0x64);
        }
    }
    func_800428C8(0);
}

void waitEndingActor0BeforeRisingStar(EndingCreditsActor0 *arg0) {
    func_800428C8(0);
    arg0->unk2A += 1;
    if (arg0->unk2A == 0x19) {
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, updateEndingActor0WaitRisingStar);
        func_80041DD4(0, 0x18);
    }
}

void updateEndingActor0RepeatAnim17(EndingCreditsActor0 *arg0) {
    if (func_80041FB4(0) == 1) {
        arg0->unk2A += 1;
        if (arg0->unk2A < 0xA) {
            func_80041DD4(0, 0x17);
        } else {
            arg0->unk2A = 0;
            setCallbackTaskCallback(arg0, waitEndingActor0BeforeRisingStar);
        }
    }
    func_800428C8(0);
    func_800373AC(&gEndingActorShadow);
}

void startEndingActor0RepeatAnim17(s32 arg0) {
    if (func_80041FB4(0) == 1) {
        setCallbackTaskCallback(arg0, updateEndingActor0RepeatAnim17);
        func_80041DD4(0, 0x17);
    }
    func_800428C8(0);
    func_800373AC(&gEndingActorShadow);
}

void waitEndingActor0Phase0D(EndingCreditsActor0 *arg0) {
    if (gEndingSequencePhase == 0xD) {
        setCallbackTaskCallback(arg0, startEndingActor0RepeatAnim17);
        func_80041DD4(0, 0x16);
        arg0->unk2C = 0x16;
    }
    func_800428C8(0);
    func_800373AC(&gEndingActorShadow);
}

void updateEndingActor0AfterPhase0DAnim(EndingCreditsActor0 *arg0) {
    if (func_80041FB4(0) == 1) {
        gEndingSequencePhase = 0xB;
        setCallbackTaskCallback(arg0, waitEndingActor0Phase0D);
    } else {
        arg0->unk2A += 1;
        if (arg0->unk2A == 3) {
            spawnEndingSmallBurst(6, -0x42);
        }
    }
    func_800428C8(0);
    func_800373AC(&gEndingActorShadow);
}

void updateEndingActor0LongWaitSetShadow(EndingCreditsActor0 *arg0) {
    EndingCreditsActor0 *temp_a2;

    temp_a2 = arg0;
    arg0->unk2A++;
    if (arg0->unk2A == 6) {
        gEndingSequencePhase = 9;
    }
    if (arg0->unk2A == 0x64) {
        gEndingSequencePhase = 0xA;
        setCallbackTaskCallback(temp_a2, updateEndingActor0AfterPhase0DAnim);
        func_80041DD4(0, 0xE);
        temp_a2->unk2A = 0;
        gEndingActorShadow.actorId = 0;
        gEndingActorShadow.unkC = 0xB;
        gEndingActorShadow.posY = (s32)0xFFE80000;
        gEndingCharacterEffectDone = 1;
    } else {
        func_80042034(0);
    }
    func_800428C8(0);
}

void updateEndingActor0SlideLeftToAura(EndingCreditsActor0 *arg0) {
    s32 temp_a1;

    temp_a1 = (arg0->unk18 += 0xFFFA0000);
    if (temp_a1 < 0x580001) {
        arg0->unk18 = 0x580000;
        setCallbackTaskCallback(arg0, updateEndingActor0LongWaitSetShadow);
        func_80041DD4(0, 0xD);
        arg0->unk26 = 0x400;
        func_800420FC(0, arg0->unk24, arg0->unk26, arg0->unk28);
        gEndingSequencePhase = 8;
        gEndingCharacterEffectDone = 0;
        spawnEndingCharacterAura(0x1C, -0x38, 0, 1);
    } else {
        func_8004209C(0, temp_a1, arg0->unk1C, arg0->unk20);
        func_80042034(0);
    }
    func_800428C8(0);
}


void updateEndingActor0WaitPhase5To7(EndingCreditsActor0 *arg0) {
    if (func_80041FB4(0) == 1) {
        if (gEndingSequencePhase == 5) {
            arg0->unk2A += 1;
            if (arg0->unk2A == 0xA) {
                gEndingSequencePhase = 6;
            }
        }
        if (gEndingSequencePhase == 7) {
            arg0->unk2A = 0;
            setCallbackTaskCallback(arg0, updateEndingActor0SlideLeftToAura);
            func_80041DD4(0, 0);
        }
    }
    func_800428C8(0);
}

void waitEndingActor0Phase5(s32 arg0) {
    func_80041FB4(0);
    if (gEndingSequencePhase == 5) {
        setCallbackTaskCallback(arg0, updateEndingActor0WaitPhase5To7);
        func_80041DD4(0, 2);
        spawnEndingDelayedSparkle(0x55, -0x62, 0);
    }
    func_800428C8(0);
}

void waitEndingActor0Phase3(EndingCreditsActor0 *arg0) {
    func_80041FB4(0);
    if (gEndingSequencePhase == 3) {
        setCallbackTaskCallback(arg0, waitEndingActor0Phase5);
        func_80041DD4(0, 2);
        spawnEndingDelayedSparkle(0x55, -0x62, 0);
        arg0->unk2A = 0;
    }
    func_800428C8(0);
}

void updateEndingActor0EnterFromRight(EndingCreditsActor0 *arg0) {
    s32 temp_a1;

    temp_a1 = (arg0->unk18 += 0xFFFD8FD8);
    if (temp_a1 < 0x900001) {
        arg0->unk18 = 0x900000;
        setCallbackTaskCallback(arg0, waitEndingActor0Phase3);
        func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
        func_80041DD4(0, 1);
        spawnEndingPhaseAdvanceSparkle(0x50, -0x68);
    } else {
        if (!temp_a1) {
        }
        func_8004209C(0, temp_a1, arg0->unk1C, arg0->unk20);
        func_80042034(0);
    }
    func_800428C8(0);
}

void waitEndingActor0Phase2(void *arg0) {
    if (gEndingSequencePhase == 2) {
        setCallbackTaskCallback(arg0, updateEndingActor0EnterFromRight);
    }
}

void initEndingCreditsActor0(EndingCreditsActor0 *arg0) {
    arg0->unk18 = 0x1900000;
    arg0->unk1C = 0;
    arg0->unk20 = 0;
    arg0->unk24 = 0;
    arg0->unk26 = 0xC00;
    arg0->unk28 = 0;
    func_80041D20(0, 0);
    func_80041DD4(0, 0);
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800420FC(0, arg0->unk24, arg0->unk26, arg0->unk28);
    setCallbackTaskCallback(arg0, waitEndingActor0Phase2);
}
