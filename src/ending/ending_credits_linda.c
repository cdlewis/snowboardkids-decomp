#include "common.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/ending/ending_credits_effects.h"
#include "game/ending/ending_credits_flow.h"
#include "game/ending/ending_credits_linda.h"
#include "game/menu/main_menu/main_menu_scene_model.h"
#include "game/menu/main_menu/main_menu_scene_model_renderer.h"
#include "game/race/player/race_player_model_renderer.h"
#include "game/math/fixed_matrix_multiply.h"

typedef s16 FixedMatrix3sScratch[0x10];
typedef struct MatrixWordCopy {
    s32 words[8];
} MatrixWordCopy;

struct EndingCreditsLinda {
    char pad[0x18];
    s32 posX;
    s32 posY;
    s32 posZ;
    s16 rotX;
    s16 rotY;
    s16 rotZ;
    u16 timer;
    u16 animTimer;
    u16 textureId;
    u16 paletteId;
};

struct EndingCreditsTumblingSnowboard {
    char pad0[0x10];
    u16 mode;
    char pad12[0x6];
    char displayObject[0x14];
    s32 posX;
    s32 posY;
    s32 posZ;
    u16 textureId;
    u16 paletteId;
    u16 timer;
};

extern void addRenderCallback(void *, void *, void *);
extern s32 allocFixedTransformMatrix(void *);
extern void makeFixedRotationX(void *, s16);
extern void makeFixedRotationZ(void *, s16);
extern void makeFixedRotationYX(void *, s16, s16, ...);
extern s32 gModelRenderCallbackList;
extern void *D_8010ADE0;

void updateEndingLindaFinalPose(s32 arg0) {
    stepMainMenuSceneModelAnimation(3);
    addMainMenuSceneModelDrawCallback(3);
}

void updateEndingLindaStartFinalPose(EndingCreditsLinda *arg0) {
    stepMainMenuSceneModelAnimation(3);
    addMainMenuSceneModelDrawCallback(3);
    if (gEndingCreditsSequencePhase == 0x41) {
        setCallbackTaskCallback(arg0, updateEndingLindaFinalPose);
        setMainMenuSceneModelAnimation(3, 0x5B);
        arg0->rotY = 0xC00;
        setMainMenuSceneModelRotation(3, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
}

void updateEndingLindaPhase40Prep(EndingCreditsLinda *arg0) {
    u16 *timerPtr;
    s32 sp18;
    int unused;
    u16 temp_t8;

    sp18 = stepMainMenuSceneModelAnimation(3);
    addMainMenuSceneModelDrawCallback(3);
    if (sp18 == 1) {
        timerPtr = &arg0->timer;
        arg0->timer = temp_t8 = (*timerPtr & 0xFFFF) + 1;
        if ((temp_t8 & 0xFFFF) == 0x14) {
            timerPtr = &arg0->timer;
            *timerPtr = 0;
            setCallbackTaskCallback(arg0, updateEndingLindaStartFinalPose);
            setMainMenuSceneModelAnimation(3, 0x5A);
            gEndingCreditsSequencePhase = 0x40;
        }
    }
}

void updateEndingLindaWaitBeforePhase40Prep(EndingCreditsLinda *arg0) {
    u16 *timerPtr;
    s32 sp18;
    int unused;
    u16 temp_t8;

    sp18 = stepMainMenuSceneModelAnimation(3);
    addMainMenuSceneModelDrawCallback(3);
    if (sp18 == 1) {
        timerPtr = &arg0->timer;
        arg0->timer = temp_t8 = (*timerPtr & 0xFFFF) + 1;
        if ((temp_t8 & 0xFFFF) == 0x1B) {
            timerPtr = &arg0->timer;
            *timerPtr = 0;
            setCallbackTaskCallback(arg0, updateEndingLindaPhase40Prep);
            setMainMenuSceneModelAnimation(3, 0x59);
            spawnEndingCreditsPhaseAdvanceSparkle(0x30, -0x5B);
        }
    }
}

void updateEndingLindaWaitPhase3FAnim(EndingCreditsLinda *arg0) {
    u16 temp_t7;

    if (stepMainMenuSceneModelAnimation(3) == 1) {
        temp_t7 = (arg0->timer & 0xFFFF) + 1;
        arg0->timer = temp_t7;
        if ((temp_t7 & 0xFFFF) == 0x14) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, updateEndingLindaWaitBeforePhase40Prep);
            setMainMenuSceneModelAnimation(3, 0x58);
        }
    } else {
        arg0->posZ += 0x80000;
    }
    setMainMenuSceneModelPosition(3, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(3);
}

void updateEndingLindaSpinUntilPhase3F(EndingCreditsLinda *arg0) {
    if (stepMainMenuSceneModelAnimation(3) == 0) {
        arg0->posX = arg0->posX + 0x18000;
        setMainMenuSceneModelPosition(3, arg0->posX, arg0->posY, arg0->posZ);
    }
    if (gEndingCreditsSequencePhase == 0x3F) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingLindaWaitPhase3FAnim);
        setMainMenuSceneModelAnimation(3, 0x57);
    }
    addMainMenuSceneModelDrawCallback(3);
}

void updateEndingLindaTumbleToPhase3C(EndingCreditsLinda *arg0) {
    s32 unused;
    s32 sp20;
    s32 var_v0;

    sp20 = stepMainMenuSceneModelAnimation(3);
    if (arg0->timer < 5) {
        var_v0 = 1;
    } else {
        var_v0 = -1;
    }
    arg0->posX += 0x76000;
    arg0->posY += var_v0 << 19;
    arg0->posZ += 0xFFFA0000;
    arg0->timer = arg0->timer + 1;
    setMainMenuSceneModelPosition(3, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(3);
    if (sp20 == 1) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingLindaSpinUntilPhase3F);
        setMainMenuSceneModelAnimation(3, 0x5D);
    }
}

void updateEndingLindaExitUntilPhase3C(EndingCreditsLinda *arg0) {
    loopMainMenuSceneModelAnimation(3);
    arg0->posX += 0xFFF58000;
    if (gEndingCreditsSequencePhase == 0x3C) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingLindaTumbleToPhase3C);
        setMainMenuSceneModelAnimation(3, 0x5C);
        arg0->rotY = 0xC00;
        setMainMenuSceneModelRotation(3, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
    setMainMenuSceneModelPosition(3, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
}

void waitEndingLindaPhase3A(EndingCreditsLinda *arg0) {
    loopMainMenuSceneModelAnimation(3);
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
    if (gEndingCreditsSequencePhase == 0x3A) {
        setCallbackTaskCallback(arg0, updateEndingLindaExitUntilPhase3C);
    }
}

void updateEndingLindaSlideToCenter(EndingCreditsLinda *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(3);
    var_a1 = (arg0->posX & 0xFFFFFFFFFFFFFFFFu) + 0x24000;
    arg0->posX = var_a1;
    if (var_a1 >= 0xE00000) {
        arg0->posX = 0xE00000;
        setCallbackTaskCallback(arg0, waitEndingLindaPhase3A);
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(3, var_a1, arg0->posY, arg0->posZ);
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
}

void waitEndingLindaPhase38(EndingCreditsLinda *arg0) {
    loopMainMenuSceneModelAnimation(3);
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
    if (gEndingCreditsSequencePhase == 0x38) {
        setCallbackTaskCallback(arg0, updateEndingLindaSlideToCenter);
    }
}

void updateEndingLindaSlideLeftSetPhase36(EndingCreditsLinda *arg0) {
    s32 var_a1;
    EndingCreditsLinda *new_var;

    loopMainMenuSceneModelAnimation(3);
    new_var = arg0;
    var_a1 = (new_var->posX += 0xFFFB8000);
    if (var_a1 < 0xA00001) {
        new_var->posX = 0xA00000;
        setCallbackTaskCallback(new_var, waitEndingLindaPhase38);
        var_a1 = arg0->posX;
    } else if ((var_a1 < 0x1300001) && (gEndingCreditsSequencePhase == 0x35)) {
        gEndingCreditsSequencePhase = 0x36;
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(3, var_a1, arg0->posY, arg0->posZ);
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
}

void waitEndingLindaPhase35(EndingCreditsLinda *arg0) {
    if (gEndingCreditsSequencePhase == 0x35) {
        setCallbackTaskCallback(arg0, updateEndingLindaSlideLeftSetPhase36);
        arg0->posX = 0x01900000;
        arg0->posZ = 0xFFF80000;
    }
}

void updateEndingLindaSlideLeftFromFarRight(EndingCreditsLinda *arg0) {
    loopMainMenuSceneModelAnimation(3);
    arg0->posX += -0x48000;
    if (arg0->posX >= -0x400000) {
        arg0->posZ += -0x48000;
    } else if (arg0->posZ < 0) {
        arg0->posZ += 0x70000;
    }

    if (arg0->posX <= -0x1900000) {
        arg0->posX = -0x1900000;
        setCallbackTaskCallback(arg0, waitEndingLindaPhase35);
    }

    if (arg0->posX <= -0xF00000 && gEndingCreditsSequencePhase == 0x31) {
        gEndingCreditsSequencePhase = 0x32;
    }

    setMainMenuSceneModelPosition(3, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
}

void waitEndingLindaPhase31(EndingCreditsLinda *arg0) {
    stepMainMenuSceneModelAnimation(3);
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
    if (gEndingCreditsSequencePhase == 0x31) {
        setCallbackTaskCallback(arg0, updateEndingLindaSlideLeftFromFarRight);
        setMainMenuSceneModelAnimation(3, 0x4B);
    }
}

void updateEndingLindaSlideLeftSetPhase2E(EndingCreditsLinda *arg0) {
    s32 sp;
    s32 temp;

    sp = stepMainMenuSceneModelAnimation(3);
    if (sp == 0) {
        arg0->posX += 0xFFFF6000;
    } else {
        temp = (arg0->timer += 1);
        if ((temp % 28) == 0) {
            if ((temp / 28) == 3) {
                setCallbackTaskCallback(arg0, waitEndingLindaPhase31);
                gEndingCreditsSequencePhase = 0x2E;
            } else {
                setMainMenuSceneModelAnimation(3, 0x4A);
            }
        }
    }
    setMainMenuSceneModelPosition(3, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
}

void updateEndingLindaWaitBeforePhase2E(EndingCreditsLinda *arg0) {
    arg0->timer++;
    if (arg0->timer == 0xF) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingLindaSlideLeftSetPhase2E);
        setMainMenuSceneModelAnimation(3, 0x4A);
    }
    addMainMenuSceneModelDrawCallback(3);
}

void updateEndingLindaRunLeftSetPhase2D(EndingCreditsLinda *arg0) {
    s32 sp;

    sp = stepMainMenuSceneModelAnimation(3);
    arg0->posX += 0xFFFD8000;
    if ((sp == 1) && (arg0->posX < 0x800001)) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingLindaWaitBeforePhase2E);
        gEndingCreditsSequencePhase = 0x2D;
    } else if ((sp == 1) && (arg0->posX >= 0x800001)) {
        setMainMenuSceneModelAnimation(3, 0x4B);
    }
    setMainMenuSceneModelPosition(3, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
}

void waitEndingLindaPhase2C(EndingCreditsLinda *arg0) {
    if (gEndingCreditsSequencePhase == 0x2C) {
        setCallbackTaskCallback(arg0, updateEndingLindaRunLeftSetPhase2D);
        setMainMenuSceneModelAnimation(3, 0x4B);
        arg0->timer = 0;
    }
}

void updateEndingLindaSlideRightSetPhase28(EndingCreditsLinda *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(3);
    var_a1 = arg0->posX += 0x28000;
    if (var_a1 >= 0x1900000) {
        arg0->posX = 0x1900000;
        setCallbackTaskCallback(arg0, waitEndingLindaPhase2C);
        gEndingCreditsSequencePhase = 0x28;
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(3, var_a1, arg0->posY, arg0->posZ);
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
}

void updateEndingLindaWaitBeforeSlideRight(EndingCreditsLinda *arg0) {
    loopMainMenuSceneModelAnimation(3);
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
    arg0->timer++;
    if (arg0->timer == 0x41) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingLindaSlideRightSetPhase28);
    }
}

void updateEndingLindaSlideLeftToPose(EndingCreditsLinda *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(3);
    var_a1 = arg0->posX += 0xFFFD8000;
    if (var_a1 < 0x800001) {
        arg0->posX = 0x800000;
        setCallbackTaskCallback(arg0, updateEndingLindaWaitBeforeSlideRight);
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(3, var_a1, arg0->posY, arg0->posZ);
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
}

void updateEndingLindaSlideRightAfterPause(EndingCreditsLinda *arg0) {
    s32 temp_a1;
    u16 temp_t9;

    loopMainMenuSceneModelAnimation(3);
    temp_a1 = arg0->posX += 0x20000;
    setMainMenuSceneModelPosition(3, temp_a1, arg0->posY, arg0->posZ);
    temp_t9 = (arg0->timer & 0xFFFF) + 1;
    arg0->timer = temp_t9;
    if ((temp_t9 & 0xFFFF) == 0x28) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingLindaSlideLeftToPose);
    }
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
}

void updateEndingLindaSlideLeftAfterPause(EndingCreditsLinda *arg0) {
    s32 temp_a1;
    u16 temp_t9;

    loopMainMenuSceneModelAnimation(3);
    temp_a1 = arg0->posX += 0xFFFD8000;
    setMainMenuSceneModelPosition(3, temp_a1, arg0->posY, arg0->posZ);
    temp_t9 = (arg0->timer & 0xFFFF) + 1;
    arg0->timer = temp_t9;
    if ((temp_t9 & 0xFFFF) == 0x2D) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingLindaSlideRightAfterPause);
    }
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
}

void updateEndingLindaSlideRightToPhase23Wait(EndingCreditsLinda *arg0) {
    s32 temp_a1;
    u16 temp_t9;

    loopMainMenuSceneModelAnimation(3);
    temp_a1 = arg0->posX += 0x28000;
    setMainMenuSceneModelPosition(3, temp_a1, arg0->posY, arg0->posZ);
    temp_t9 = (arg0->timer & 0xFFFF) + 1;
    arg0->timer = temp_t9;
    if ((temp_t9 & 0xFFFF) == 0x1E) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingLindaSlideLeftAfterPause);
    }
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
}

void waitEndingLindaPhase23(EndingCreditsLinda *arg0) {
    loopMainMenuSceneModelAnimation(3);
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
    if (gEndingCreditsSequencePhase == 0x23) {
        arg0->timer++;
        if (arg0->timer == 0x2D) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, updateEndingLindaSlideRightToPhase23Wait);
        }
    }
}

void updateEndingLindaSlideRightSetPhase21(EndingCreditsLinda *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(3);
    var_a1 = arg0->posX += 0x28000;
    if (var_a1 >= 0x800000) {
        arg0->timer = 0;
        arg0->posX = 0x800000;
        setCallbackTaskCallback(arg0, waitEndingLindaPhase23);
        var_a1 = arg0->posX;
    }
    if ((var_a1 >= 0x200000) && (gEndingCreditsSequencePhase == 0x20)) {
        gEndingCreditsSequencePhase = 0x21;
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(3, var_a1, arg0->posY, arg0->posZ);
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
}

void updateEndingLindaSlideLeftToFarLeft(EndingCreditsLinda *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(3);
    var_a1 = arg0->posX += 0xFFFA0000;
    if (var_a1 < -0x9FFFFF) {
        arg0->posX = 0xFF600000;
        setCallbackTaskCallback(arg0, updateEndingLindaSlideRightSetPhase21);
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(3, var_a1, arg0->posY, arg0->posZ);
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
}

void updateEndingLindaRepeatAnimUntilPhase20(EndingCreditsLinda *arg0) {
    u16 temp_t8;
    s32 sp20;

    sp20 = stepMainMenuSceneModelAnimation(3);
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
    if (sp20 == 0) {
        if (gEndingCreditsSequencePhase == 0x1D) {
            temp_t8 = (arg0->timer & 0xFFFF) + 1;
            arg0->timer = temp_t8;
            if ((temp_t8 & 0xFFFF) == 3) {
                gEndingCreditsSequencePhase = 0x1E;
                arg0->timer = 0;
            }
        }
    }
    if (sp20 == 1) {
        setMainMenuSceneModelAnimation(3, 0x4A);
    }
    if (gEndingCreditsSequencePhase == 0x20) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingLindaSlideLeftToFarLeft);
        setMainMenuSceneModelAnimation(3, 0x4B);
    }
}

void waitEndingLindaPhase1D(EndingCreditsLinda *arg0) {
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
    if (gEndingCreditsSequencePhase == 0x1D) {
        setCallbackTaskCallback(arg0, updateEndingLindaRepeatAnimUntilPhase20);
        setMainMenuSceneModelAnimation(3, 0x4A);
    }
}

void updateEndingLindaSlideLeftSetPhase1C(EndingCreditsLinda *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(3);
    var_a1 = arg0->posX += 0xFFFE8000;
    if (var_a1 < 0xC80001) {
        arg0->posX = 0xC80000;
        setCallbackTaskCallback(arg0, waitEndingLindaPhase1D);
        gEndingCreditsSequencePhase = 0x1C;
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(3, var_a1, arg0->posY, arg0->posZ);
    addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(3, arg0->textureId, arg0->paletteId, 0xB);
}

void updateEndingLindaAfterIntroAnim2(EndingCreditsLinda *arg0) {
    s32 pad;
    s32 sp20;

    sp20 = stepMainMenuSceneModelAnimation(3);
    addMainMenuSceneModelTexturedDrawCallback(3, arg0->textureId, arg0->paletteId);
    if (sp20 == 1) {
        setCallbackTaskCallback(arg0, updateEndingLindaSlideLeftSetPhase1C);
        setMainMenuSceneModelAnimation(3, 0x49);
        arg0->posX += 0xFFEC0000;
    }
}

void updateEndingLindaAfterIntroAnim1(EndingCreditsLinda *arg0) {
    s32 pad;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(3);
    addMainMenuSceneModelTexturedDrawCallback(3, arg0->textureId, arg0->paletteId);
    if (sp18 == 1) {
        setCallbackTaskCallback(arg0, updateEndingLindaAfterIntroAnim2);
        setMainMenuSceneModelAnimation(3, 0x42);
    }
}

void updateEndingLindaWaitBeforePhase1B(EndingCreditsLinda *arg0) {
    addMainMenuSceneModelDrawCallback(3);
    arg0->timer++;
    if (arg0->timer == 0x1E) {
        arg0->timer = 0;
        gEndingCreditsSequencePhase = 0x1B;
        setCallbackTaskCallback(arg0, updateEndingLindaAfterIntroAnim1);
        setMainMenuSceneModelAnimation(3, 0x41);
    }
}

void updateEndingLindaHopRightToIdle(EndingCreditsLinda *arg0) {
    if (stepMainMenuSceneModelAnimation(3) == 1) {
        setCallbackTaskCallback(arg0, updateEndingLindaWaitBeforePhase1B);
    } else {
        arg0->posX = arg0->posX + 0x40000;
        setMainMenuSceneModelPosition(3, arg0->posX, arg0->posY, arg0->posZ);
    }
    addMainMenuSceneModelDrawCallback(3);
}

void updateEndingLindaHopRightToPose(EndingCreditsLinda *arg0) {
    s32 temp_v0;
    u16 temp_t7;

    temp_v0 = stepMainMenuSceneModelAnimation(3);
    temp_t7 = (arg0->timer & 0xFFFF) + 1;
    arg0->timer = temp_t7;
    arg0->posX += 0x70000;
    if ((temp_t7 & 0xFFFF) < 5) {
        arg0->posY += 0x50000;
    } else {
        arg0->posY += 0xFFFB0000;
    }
    if (temp_v0 == 1) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingLindaHopRightToIdle);
        setMainMenuSceneModelAnimation(3, 0x40);
    }
    setMainMenuSceneModelPosition(3, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(3);
}

void updateEndingLindaSlideRightSetPhase1A(EndingCreditsLinda *arg0) {
    s32 pad;
    s32 sp20;

    arg0->posX += 0x100000;
    setMainMenuSceneModelPosition(3, arg0->posX, arg0->posY, arg0->posZ);
    sp20 = stepMainMenuSceneModelAnimation(3);
    addMainMenuSceneModelDrawCallback(3);
    arg0->posY += 0x50000;
    if (sp20 == 1) {
        arg0->timer = 0;
        gEndingCreditsSequencePhase = 0x1A;
        setCallbackTaskCallback(arg0, updateEndingLindaHopRightToPose);
        setMainMenuSceneModelAnimation(3, 0x3F);
    }
}

void updateEndingLindaHandshakeAnimComplete(EndingCreditsLinda *arg0) {
    s32 pad;
    s32 sp20;
    s32 i;
    s32 count;

    if (arg0->animTimer >= 3) {
        count = 2;
    } else {
        count = 1;
    }
    for (i = 0; i < count; i++) {
        sp20 = stepMainMenuSceneModelAnimation(3);
    }

    addMainMenuSceneModelDrawCallback(3);
    if (sp20 == 1) {
        gEndingCreditsHandshakeState++;
        setCallbackTaskCallback(arg0, startEndingLindaHandshakeLoop);
    }
}

void updateEndingLindaHandshakeLoop(EndingCreditsLinda *arg0) {
    s32 var_s1;
    s32 var_s4;
    u16 temp_t8;

    if (arg0->animTimer >= 3) {
        var_s4 = 2;
    } else {
        var_s4 = 1;
    }
    var_s1 = 0;
    if (var_s4 > 0) {
        do {
            stepMainMenuSceneModelAnimation(3);
            addMainMenuSceneModelDrawCallback(3);
            temp_t8 = (arg0->timer & 0xFFFF) + 1;
            arg0->timer = temp_t8;
            if ((temp_t8 & 0xFFFF) < 5) {
                if ((gEndingCreditsHandshakeState == 1) || (gEndingCreditsHandshakeState == 5)) {
                    arg0->posX = (s32) (arg0->posX + 0x100000);
                } else {
                    arg0->posX = (s32) (arg0->posX + 0xFFF00000);
                }
                if ((s32) arg0->timer < 3) {
                    arg0->posY = (s32) (arg0->posY + 0x90000);
                } else {
                    arg0->posY = (s32) (arg0->posY + 0xFFF70000);
                }
                setMainMenuSceneModelPosition(3, arg0->posX, arg0->posY, arg0->posZ);
                var_s1 += 1;
                if ((arg0->animTimer == 0xD) && (gEndingCreditsHandshakeState == 1) && (arg0->timer == 3)) {
                    gEndingCreditsHandshakeState = 5;
                }
                continue;
            } else {
                arg0->timer = 0U;
                arg0->posY = 0;
                setMainMenuSceneModelPosition(3, arg0->posX, 0, arg0->posZ);
                setCallbackTaskCallback(arg0, updateEndingLindaHandshakeAnimComplete);
                if ((gEndingCreditsHandshakeState == 1) || (gEndingCreditsHandshakeState == 5)) {
                    setMainMenuSceneModelAnimation(3, 0x3A);
                } else {
                    setMainMenuSceneModelAnimation(3, 0x3C);
                    arg0->animTimer = (u16) (arg0->animTimer + 1);
                }
                if (arg0->animTimer == 0xD) {
                    if (gEndingCreditsHandshakeState == 5) {
                        setCallbackTaskCallback(arg0, updateEndingLindaSlideRightSetPhase1A);
                        setMainMenuSceneModelAnimation(3, 0x3E);
                        arg0->animTimer = 0U;
                    }
                }
                return;
            }
        } while (var_s1 != var_s4);
    }
}

void startEndingLindaHandshakeLoop(EndingCreditsLinda *arg0) {
    addMainMenuSceneModelDrawCallback(3);
    if ((gEndingCreditsHandshakeState == 1) || (gEndingCreditsHandshakeState == 3)) {
        setCallbackTaskCallback(arg0, updateEndingLindaHandshakeLoop);
        if (gEndingCreditsHandshakeState == 1) {
            arg0->rotY = 0x400;
            if (arg0->timer == 0xF) {
                setMainMenuSceneModelAnimation(3, 0x39);
                arg0->timer = 2;
                arg0->posY = 0x120000;
            } else {
                setMainMenuSceneModelAnimation(3, 0x3D);
            }
        } else {
            arg0->rotY = 0x400;
            setMainMenuSceneModelAnimation(3, 0x3B);
        }
        setMainMenuSceneModelRotation(3, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
}

void waitEndingLindaPhase19(EndingCreditsLinda *arg0) {
    addMainMenuSceneModelDrawCallback(3);
    arg0->timer++;
    if (arg0->timer == 0xF) {
        gEndingCreditsSequencePhase = 0x19;
        setCallbackTaskCallback(arg0, startEndingLindaHandshakeLoop);
        setMainMenuSceneModelPosition(3, arg0->posX, arg0->posY, arg0->posZ);
    }
}

void updateEndingLindaWaitThenSetPhase18(EndingCreditsLinda *arg0) {
    void (*new_var)(EndingCreditsLinda *);
    s32 sp18;
    u16 temp_t8;

    sp18 = stepMainMenuSceneModelAnimation(3);
    addMainMenuSceneModelDrawCallback(3);
    if (sp18 == 1) {
        temp_t8 = (arg0->timer & 0xFFFF) + 1;
        arg0->timer = temp_t8;
        if ((temp_t8 & 0xFFFF) == 0xA) {
            arg0->timer = 0;
            new_var = waitEndingLindaPhase19;
            gEndingCreditsSequencePhase = 0x18;
            setCallbackTaskCallback(arg0, new_var);
        }
    }
}

void updateEndingLindaSlideLeftUntilPhase17(EndingCreditsLinda *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(3);
    addMainMenuSceneModelDrawCallback(3);
    var_a1 = arg0->posX += (s32)0xFFFD0000;
    arg0->posZ = arg0->posZ + (s32)0xFFFF0000;
    if (var_a1 < 0x2D0001) {
        arg0->posX = 0x2D0000;
        arg0->posZ = (s32)0xFFFE0000;
        setCallbackTaskCallback(arg0, updateEndingLindaWaitThenSetPhase18);
        setMainMenuSceneModelAnimation(3, 0x38);
        gEndingCreditsSequencePhase = 0x17;
        arg0->rotY = 0x400;
        setMainMenuSceneModelRotation(3, arg0->rotX, arg0->rotY, arg0->rotZ);
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(3, var_a1, arg0->posY, arg0->posZ);
}

void updateEndingLindaBlinkThenSlideLeft(EndingCreditsLinda *arg0) {
    u16 temp;
    s32 sp20;

    sp20 = stepMainMenuSceneModelAnimation(3);
    if (arg0->animTimer == 0) {
        addMainMenuSceneModelDrawCallback(3);
    } else {
        addMainMenuSceneModelDrawCallbackForViewport0(3);
    }
    temp = (arg0->animTimer & 0xFFFF) + 1;
    arg0->animTimer = temp;
    if (sp20 == 1) {
        arg0->animTimer = 0;
        temp = (arg0->timer & 0xFFFF) + 1;
        arg0->timer = temp;
        if ((temp & 0xFFFF) < 2) {
            setMainMenuSceneModelAnimation(3, 0x37);
        } else {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, updateEndingLindaSlideLeftUntilPhase17);
            setMainMenuSceneModelAnimation(3, 0xF);
            arg0->rotY = 0xC00;
            setMainMenuSceneModelRotation(3, arg0->rotX, arg0->rotY, arg0->rotZ);
        }
    }
}

void updateEndingLindaStartBlinkLoop(EndingCreditsLinda *arg0) {
    if (stepMainMenuSceneModelAnimation(3) == 1) {
        arg0->animTimer = 0;
        setCallbackTaskCallback(arg0, updateEndingLindaBlinkThenSlideLeft);
        setMainMenuSceneModelAnimation(3, 0x37);
    }
    addMainMenuSceneModelDrawCallback(3);
}

void updateEndingLindaSlideRightUntilPhase16(EndingCreditsLinda *arg0) {
    u16 temp_t9;

    if (stepMainMenuSceneModelAnimation(3) == 0) {
        arg0->posX = arg0->posX + 0x48000;
        setMainMenuSceneModelPosition(3, arg0->posX, arg0->posY, arg0->posZ);
        temp_t9 = (arg0->timer & 0xFFFF) + 1;
        arg0->timer = temp_t9;
        if ((temp_t9 & 0xFFFF) == 2) {
            spawnEndingCreditsCharacterVanishPoof(-0x14, -0x40, 3, 1);
        }
    }
    if (gEndingCreditsSequencePhase == 0x16) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingLindaStartBlinkLoop);
        setMainMenuSceneModelAnimation(3, 0x36);
    }
    addMainMenuSceneModelDrawCallback(3);
}

void waitEndingLindaPhase15(EndingCreditsLinda *arg0) {
    u16 temp_t0;
    s32 sp20;

    sp20 = stepMainMenuSceneModelAnimation(3);
    addMainMenuSceneModelDrawCallback(3);
    if (gEndingCreditsSequencePhase == 0x15) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, updateEndingLindaSlideRightUntilPhase16);
        arg0->rotY = 0x400;
        setMainMenuSceneModelRotation(3, arg0->rotX, arg0->rotY, arg0->rotZ);
        setMainMenuSceneModelAnimation(3, 0x28);
    } else if (sp20 == 1) {
        temp_t0 = (arg0->timer & 0xFFFF) + 1;
        arg0->timer = temp_t0;
        if ((temp_t0 & 0xFFFF) == 0x10) {
            arg0->timer = 0;
            setMainMenuSceneModelAnimation(3, 0x27);
        }
    }
}

void updateEndingLindaSlideLeftSetPhase14(EndingCreditsLinda *arg0) {
    s32 temp_v0;
    u16 temp_t2;

    temp_v0 = stepMainMenuSceneModelAnimation(3);
    arg0->posX += 0xFFFB8000;
    if (arg0->posX < 0xE00001) {
        arg0->posX = 0xE00000;
        setCallbackTaskCallback(arg0, waitEndingLindaPhase15);
        gEndingCreditsSequencePhase = 0x14;
    } else if (temp_v0 == 1) {
        temp_t2 = (arg0->timer & 0xFFFF) + 1;
        arg0->timer = temp_t2;
        if ((temp_t2 & 0xFFFF) == 0x10) {
            arg0->timer = 0;
            setMainMenuSceneModelAnimation(3, 0x27);
        }
    }
    setMainMenuSceneModelPosition(3, arg0->posX, arg0->posY, arg0->posZ);
    addMainMenuSceneModelDrawCallback(3);
}

void waitEndingLindaPhase13(EndingCreditsLinda *arg0) {
    if (gEndingCreditsSequencePhase == 0x13) {
        setCallbackTaskCallback(arg0, updateEndingLindaSlideLeftSetPhase14);
        spawnEndingCreditsTumblingSnowboard(arg0->posX + 0x48000, 0x480000, 0xFFF70000, arg0->textureId, arg0->paletteId, 2);
    }
}

void initEndingCreditsLinda(EndingCreditsLinda *arg0) {
    arg0->posX = 0x03B48000;
    arg0->posY = 0;
    arg0->posZ = 0;
    arg0->rotX = 0;
    arg0->rotY = 0;
    arg0->rotZ = 0;
    arg0->timer = 0;
    arg0->animTimer = 0;
    arg0->textureId = 0;
    arg0->paletteId = 3;
    initMainMenuSceneModel(3, 3);
    setMainMenuSceneModelAnimation(3, 0x27);
    setMainMenuSceneModelPosition(3, arg0->posX, arg0->posY, arg0->posZ);
    setMainMenuSceneModelRotation(3, arg0->rotX, arg0->rotY, arg0->rotZ);
    setCallbackTaskCallback(arg0, waitEndingLindaPhase13);
}

void spawnEndingCreditsTumblingSnowboard(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u8 arg5) {
    EndingCreditsTumblingSnowboard *sp1C;
    EndingCreditsTumblingSnowboard *temp_v0;

    temp_v0 = createCallbackTaskWithUserId((void (*)(void *))updateEndingCreditsTumblingSnowboardSlideIn, 0, 0x64, arg5);
    D_8010ADE0 = temp_v0;
    temp_v0->posX = arg0;
    temp_v0->posY = arg1;
    temp_v0->posZ = arg2;
    sp1C = temp_v0;
    makeFixedRotationYX(&temp_v0->displayObject, 0x400, 0x400, &D_8010ADE0);
    sp1C->textureId = arg3;
    sp1C->paletteId = arg4;
}

void drawEndingCreditsTumblingSnowboard(EndingCreditsTumblingSnowboard *arg0) {
    s32 temp = allocFixedTransformMatrix(&arg0->displayObject);
    if (temp != 0) {
        drawRacePlayerModelRootPart((void *)temp, arg0->textureId, arg0->paletteId);
    }
}

void updateEndingCreditsTumblingSnowboardWaitForRemove(EndingCreditsTumblingSnowboard *arg0) {
    if (gEndingCreditsSequencePhase == 0x1B) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gModelRenderCallbackList, drawEndingCreditsTumblingSnowboard, arg0);
    }
}

void updateEndingCreditsTumblingSnowboardBounce(EndingCreditsTumblingSnowboard *arg0) {
    volatile s32 pad[2];
    FixedMatrix3sScratch sp48;
    FixedMatrix3sScratch sp28;
    void *sp24[1];
    s32 timer;

    arg0->timer += 1;
    if (arg0->mode == 1) {
        timer = arg0->timer;
        if (timer < 6) {
            arg0->posY = (arg0->posY - (timer * 0x15000)) + 0x80000;
        } else {
            arg0->posY = (arg0->posY - (timer * 0x15000)) + 0x124000;
        }
        arg0->posX += 0xC0000;
    } else {
        timer = arg0->timer;
        if (timer < 5) {
            arg0->posY = (arg0->posY - (timer << 0xF)) + 0x40000;
        } else {
            arg0->posY = (arg0->posY - (timer * 0x3800)) + 0xFFFE8800;
        }
        arg0->posX += 0x1D000;
    }

    makeFixedRotationYX(sp48, 0x400, 0x400);
    if (arg0->mode == 1) {
        makeFixedRotationZ(sp28, 0x1000 - (arg0->timer << 6));
    } else {
        makeFixedRotationZ(sp28, 0x1000 - (arg0->timer * 0x28));
    }

    sp24[0] = &arg0->displayObject;
    multiplyFixedMatrix3s(sp48, sp28, sp24[0]);

    if (arg0->mode == 1) {
        if (arg0->posX >= 0x1900000) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, updateEndingCreditsTumblingSnowboardWaitForRemove);
        }
    } else if (arg0->timer == 0x14) {
        arg0->timer = 0;
        arg0->posY = 0x8BD1E;
        setCallbackTaskCallback(arg0, updateEndingCreditsTumblingSnowboardWaitForRemove);
        makeFixedRotationYX(sp48, 0x400, 0x400);
        makeFixedRotationZ(sp28, 0xC00);
        multiplyFixedMatrix3s(sp48, sp28, sp24[0]);
        makeFixedRotationX(sp48, 0x300);
        *(MatrixWordCopy *) sp28 = *(MatrixWordCopy *) sp24[0];
        multiplyFixedMatrix3s(sp28, sp48, sp24[0]);
    }

    addRenderCallback(&gModelRenderCallbackList, drawEndingCreditsTumblingSnowboard, arg0);
}

void waitEndingCreditsTumblingSnowboardPhase15(EndingCreditsTumblingSnowboard *arg0) {
    if (gEndingCreditsSequencePhase == 0x15) {
        setCallbackTaskCallback(arg0, updateEndingCreditsTumblingSnowboardBounce);
    }
    addRenderCallback(&gModelRenderCallbackList, drawEndingCreditsTumblingSnowboard, arg0);
}

void updateEndingCreditsTumblingSnowboardSlideIn(EndingCreditsTumblingSnowboard *arg0) {
    arg0->posX = arg0->posX + 0xFFFB8000;
    arg0->timer++;
    if (arg0->timer == 0xA2) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, waitEndingCreditsTumblingSnowboardPhase15);
    }
    addRenderCallback(&gModelRenderCallbackList, (void *)drawEndingCreditsTumblingSnowboard, arg0);
}
