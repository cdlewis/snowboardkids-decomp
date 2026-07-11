#include "common.h"
#include "callback_task_scheduler.h"
#include "menu_transition_effects.h"
#include "main_menu_scene_actor_1.h"
#include "main_menu_scene_model.h"
#include "main_menu_scene_renderer.h"

struct MainMenuSceneActor1 {
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
extern s8 D_8010B1A9;

void func_80035220(MainMenuSceneActor1 *arg0) {
    stepMainMenuSceneModelAnimation(1);
    func_800428C8(1);
}

void func_8003524C(MainMenuSceneActor1 *arg0) {
    stepMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (gEndingSequencePhase == 0x41) {
        setCallbackTaskCallback(arg0, func_80035220);
        setMainMenuSceneModelAnimation(1, 0x5B);
        arg0->rotY = 0xC00;
        setMainMenuSceneModelRotation(1, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
}

void func_800352C4(MainMenuSceneActor1 *arg0) {
    stepMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (gEndingSequencePhase == 0x40) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, func_8003524C);
        setMainMenuSceneModelAnimation(1, 0x5A);
    }
}

void func_80035320(MainMenuSceneActor1 *arg0) {
    s32 unused;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (sp18 == 1) {
        if (++arg0->timer == 0x1A) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, func_800352C4);
            setMainMenuSceneModelAnimation(1, 0x59);
            spawnEndingPhaseAdvanceSparkle(0x53, -0x64);
        }
    }
}

void func_800353AC(MainMenuSceneActor1 *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (sp18 == 1) {
        if (++arg0->timer == 0x14) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, func_80035320);
            setMainMenuSceneModelAnimation(1, 0x58);
        }
    }
}

void func_8003542C(MainMenuSceneActor1 *arg0) {
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
        setCallbackTaskCallback(arg0, func_800353AC);
        setMainMenuSceneModelAnimation(1, 0x57);
    }
}

void func_800354BC(MainMenuSceneActor1 *arg0) {
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
        setCallbackTaskCallback(arg0, func_8003542C);
        setMainMenuSceneModelAnimation(1, 0x5D);
    }
}

void func_8003556C(MainMenuSceneActor1 *arg0) {
    loopMainMenuSceneModelAnimation(1);
    arg0->posX += 0xFFF58000;
    if (gEndingSequencePhase == 0x3C) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, func_800354BC);
        setMainMenuSceneModelAnimation(1, 0x5C);
        D_8010B1A9 = 1;
    }
    setMainMenuSceneModelPosition(1, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(1);
}

void func_80035600(MainMenuSceneActor1 *arg0) {
    loopMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (gEndingSequencePhase == 0x3A) {
        setCallbackTaskCallback(arg0, func_8003556C);
    }
}

void func_8003564C(MainMenuSceneActor1 *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(1);
    arg0->posX += 0x24000;
    var_a1 = arg0->posX;
    if (var_a1 >= 0x1200000) {
        arg0->posX = 0x1200000;
        setCallbackTaskCallback(arg0, func_80035600);
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(1, var_a1, arg0->posY, arg0->posZ);
    func_800428C8(1);
}

void func_800356C8(MainMenuSceneActor1 *arg0) {
    loopMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (gEndingSequencePhase == 0x38) {
        setCallbackTaskCallback(arg0, func_8003564C);
    }
}

void func_80035714(MainMenuSceneActor1 *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(1);
    arg0->posX += 0xFFFB8000;
    var_a1 = arg0->posX;
    if (var_a1 < 0x01000001) {
        arg0->posX = 0x01000000;
        setCallbackTaskCallback(arg0, func_800356C8);
        gEndingSequencePhase = 0x37;
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(1, var_a1, arg0->posY, arg0->posZ);
    func_800428C8(1);
}

void func_800357A4(MainMenuSceneActor1 *arg0) {
    if (gEndingSequencePhase == 0x36) {
        setCallbackTaskCallback(arg0, func_80035714);
        arg0->posX = 0x1900000;
    }
}

void func_800357E8(MainMenuSceneActor1 *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(1);
    arg0->posX += 0xFFFB8000;
    var_a1 = arg0->posX;
    if (var_a1 < (s32)0xFE700001) {
        arg0->posX = -0x1900000;
        setCallbackTaskCallback(arg0, func_800357A4);
        gEndingSequencePhase = 0x33;
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(1, var_a1, arg0->posY, arg0->posZ);
    func_800428C8(1);
}

void func_80035878(MainMenuSceneActor1 *arg0) {
    if (stepMainMenuSceneModelAnimation(1) == 0) {
        arg0->posZ = 0;
    }
    setMainMenuSceneModelPosition(1, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(1);
    if (gEndingSequencePhase == 0x32) {
        setCallbackTaskCallback(arg0, func_800357E8);
        setMainMenuSceneModelAnimation(1, 0x52);
        arg0->rotY = 0xC00;
        setMainMenuSceneModelRotation(1, arg0->rotX, arg0->rotY, arg0->rotZ);
        gEndingCharacterEffectDone = 0;
        spawnEndingCharacterAura(0x13, -0x3A, 1, 1);
    }
}

void func_8003592C(MainMenuSceneActor1 *arg0) {
    func_800428C8(1);
    if (gEndingSequencePhase == 0x30) {
        setCallbackTaskCallback(arg0, func_80035878);
        setMainMenuSceneModelAnimation(1, 0x51);
        spawnEndingCharacterVanishPoof(6, -0x40, 1, 0);
    }
}

void func_80035990(MainMenuSceneActor1 *arg0) {
    if (stepMainMenuSceneModelAnimation(1) == 1) {
        arg0->timer++;
        if (arg0->timer == 0xA) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, func_8003592C);
            gEndingSequencePhase = 0x2F;
        }
    } else {
        arg0->posX += 0x20000;
    }
    setMainMenuSceneModelPosition(1, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(1);
}

void func_80035A2C(MainMenuSceneActor1 *arg0) {
    func_800428C8(1);
    if (gEndingSequencePhase == 0x2E) {
        if (++arg0->timer == 0x14) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, func_80035990);
            setMainMenuSceneModelAnimation(1, 0x50);
        }
    }
}

void func_80035AA4(MainMenuSceneActor1 *arg0) {
    s32 sp1c;
    s32 sp18;

    sp18 = stepMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    sp1c = (sp18 == 1);
    if (sp1c) {
        setCallbackTaskCallback(arg0, func_80035A2C);
        arg0->timer = 0;
    }
}

void func_80035AFC(MainMenuSceneActor1 *arg0) {
    s32 temp_a1;

    stepMainMenuSceneModelAnimation(1);
    temp_a1 = (arg0->posX += 0x2800);
    if (1) {
        setMainMenuSceneModelPosition(1, temp_a1, arg0->posY, arg0->posZ);
        func_800428C8(1);
    }
    if (gEndingSequencePhase == 0x2D) {
        setCallbackTaskCallback(arg0, func_80035AA4);
        setMainMenuSceneModelAnimation(1, 0x4F);
        spawnEndingCharacterVanishPoof(8, -0x40, 1, 0);
    }
}

void func_80035B88(MainMenuSceneActor1 *arg0) {
    loopMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (gEndingSequencePhase == 0x2B) {
        setCallbackTaskCallback(arg0, func_80035AFC);
        setMainMenuSceneModelAnimation(1, 0x4D);
        createCallbackTask(&initEndingSpeedLines, 0, 0x64);
    }
}

void func_80035BF4(MainMenuSceneActor1 *arg0) {
    s32 var_a1;

    loopMainMenuSceneModelAnimation(1);
    arg0->posX += 0x120000;
    var_a1 = arg0->posX;
    if (var_a1 >= -0x600000) {
        arg0->posX = -0x600000;
        setCallbackTaskCallback(arg0, func_80035B88);
        gEndingSequencePhase = 0x2A;
        var_a1 = arg0->posX;
    }
    setMainMenuSceneModelPosition(1, var_a1, arg0->posY, arg0->posZ);
    func_800428C8(1);
}

void func_80035C78(MainMenuSceneActor1 *arg0) {
    if (gEndingSequencePhase == 0x29) {
        setCallbackTaskCallback(arg0, func_80035BF4);
        setMainMenuSceneModelAnimation(1, 0x4C);
        arg0->posZ = 0xA0000;
    }
}

void func_80035CC8(MainMenuSceneActor1 *arg0) {
    arg0->posX += 0xFFFE0000;
    setMainMenuSceneModelPosition(1, arg0->posX, arg0->posY, arg0->posZ);
    if (arg0->posX < -0x18FFFFF) {
        gEndingSequencePhase = 0x12;
        arg0->posX = -0x1900000;
        setCallbackTaskCallback(arg0, func_80035C78);
    }
    loopMainMenuSceneModelAnimation(1);
    func_800428C8(1);
}

void func_80035D54(MainMenuSceneActor1 *arg0) {
    loopMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (++arg0->timer == 0x32) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, func_80035CC8);
    }
}

void func_80035DB4(MainMenuSceneActor1 *arg0) {
    loopMainMenuSceneModelAnimation(1);
    arg0->posX += 0x60000;
    setMainMenuSceneModelPosition(1, arg0->posX, arg0->posY, arg0->posZ);
    if (arg0->posX >= -0x580000) {
        arg0->posX = -0x580000;
        setCallbackTaskCallback(arg0, func_80035D54);
    }
    func_800428C8(1);
}

void func_80035E28(MainMenuSceneActor1 *arg0) {
    if (gEndingSequencePhase == 0x11) {
        arg0->posX = 0xFE700000;
        arg0->rotY = 0x400;
        setMainMenuSceneModelPosition(1, -0x1900000, arg0->posY, arg0->posZ);
        setMainMenuSceneModelRotation(1, arg0->rotX, arg0->rotY, arg0->rotZ);
        setCallbackTaskCallback(arg0, func_80035DB4);
        setMainMenuSceneModelAnimation(1, 0x1F);
    }
}

void func_80035EA8(MainMenuSceneActor1 *arg0) {
    loopMainMenuSceneModelAnimation(1);
    arg0->posX += 0x60000;
    setMainMenuSceneModelPosition(1, arg0->posX, arg0->posY, arg0->posZ);
    if (arg0->posX >= 0x1900000) {
        arg0->posX = 0x1900000;
        setCallbackTaskCallback(arg0, func_80035E28);
        gEndingSequencePhase = 0xF;
    }
    func_800428C8(1);
}

void func_80035F28(MainMenuSceneActor1 *arg0) {
    loopMainMenuSceneModelAnimation(1);
    func_800428C8(1);
    if (gEndingSequencePhase == 0xE) {
        setCallbackTaskCallback(arg0, func_80035EA8);
        setMainMenuSceneModelAnimation(1, 0x1F);
    }
}

void func_80035F80(MainMenuSceneActor1 *arg0) {
    if (stepMainMenuSceneModelAnimation(1) == 1) {
        if (++arg0->timer == 0xC) {
            arg0->timer = 0;
            setCallbackTaskCallback(arg0, func_80035F28);
            setMainMenuSceneModelAnimation(1, 0x15);
            createCallbackTask(&initEndingRunDust, 0, 0x64);
        }
    }
    func_800428C8(1);
}

void func_8003600C(MainMenuSceneActor1 *arg0) {
    if (stepMainMenuSceneModelAnimation(1) == 1) {
        arg0->timer += 1;
        if (arg0->timer < 3) {
            setMainMenuSceneModelAnimation(1, 0x13);
        }
        if (arg0->timer == 3) {
            arg0->timer = 0;
            gEndingSequencePhase = 0xD;
            setCallbackTaskCallback(arg0, func_80035F80);
            setMainMenuSceneModelAnimation(1, 0x14);
            spawnEndingCharacterVanishPoof(8, -0x40, 1, 0);
        }
    }
    func_800428C8(1);
}

void func_800360CC(MainMenuSceneActor1 *arg0) {
    if (stepMainMenuSceneModelAnimation(1) == 1) {
        setCallbackTaskCallback(arg0, func_8003600C);
        setMainMenuSceneModelAnimation(1, 0x13);
    }
    func_800428C8(1);
}

void func_8003611C(MainMenuSceneActor1 *arg0) {
    stepMainMenuSceneModelAnimation(1);
    arg0->timer += 1;
    if (arg0->timer == 0x46) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, func_800360CC);
        setMainMenuSceneModelAnimation(1, 0x12);
        spawnEndingDelayedSparkle(-0xB, -0x58, 1);
    }
    func_800428C8(1);
}

void func_80036198(MainMenuSceneActor1 *arg0) {
    s32 temp_a3;

    if (stepMainMenuSceneModelAnimation(1) == 0) {
        temp_a3 = (arg0->posZ += (s32)0xFFFA0000);
        setMainMenuSceneModelPosition(1, arg0->posX, arg0->posY, temp_a3);
    }
    if (gEndingSequencePhase == 0xC) {
        setCallbackTaskCallback(arg0, func_8003611C);
        setMainMenuSceneModelAnimation(1, 0x11);
    }
    func_800428C8(1);
}

void func_80036218(MainMenuSceneActor1 *arg0) {
    arg0->posX += 0x39435;
    if (gEndingSequencePhase == 0xB) {
        setCallbackTaskCallback(arg0, func_80036198);
        setMainMenuSceneModelAnimation(1, 0x10);
        spawnEndingCharacterVanishPoof(0xA, -0x40, 1, 0);
    } else {
        setMainMenuSceneModelPosition(1, arg0->posX, arg0->posY, arg0->posZ);
        loopMainMenuSceneModelAnimation(1);
    }
    func_800428C8(1);
}

void func_800362B4(MainMenuSceneActor1 *arg0) {
    if (gEndingSequencePhase == 9) {
        setCallbackTaskCallback(arg0, func_80036218);
    }
}

void func_800362E8(MainMenuSceneActor1 *arg0) {
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
    setCallbackTaskCallback(arg0, func_800362B4);
}
