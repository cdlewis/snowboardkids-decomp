#include "common.h"
#include "effect_task_scheduler.h"
#include "main_menu_effects.h"
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

extern u16 D_8010B1A2;
extern s8 D_8010B1A8;
extern s8 D_8010B1A9;

void func_80035220(MainMenuSceneActor1 *arg0) {
    func_80041FB4(1);
    func_800428C8(1);
}

void func_8003524C(MainMenuSceneActor1 *arg0) {
    func_80041FB4(1);
    func_800428C8(1);
    if (D_8010B1A2 == 0x41) {
        func_80071824(arg0, func_80035220);
        func_80041DD4(1, 0x5B);
        arg0->rotY = 0xC00;
        func_800420FC(1, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
}

void func_800352C4(MainMenuSceneActor1 *arg0) {
    func_80041FB4(1);
    func_800428C8(1);
    if (D_8010B1A2 == 0x40) {
        arg0->timer = 0;
        func_80071824(arg0, func_8003524C);
        func_80041DD4(1, 0x5A);
    }
}

void func_80035320(MainMenuSceneActor1 *arg0) {
    s32 unused;
    s32 sp18;

    sp18 = func_80041FB4(1);
    func_800428C8(1);
    if (sp18 == 1) {
        if (++arg0->timer == 0x1A) {
            arg0->timer = 0;
            func_80071824(arg0, func_800352C4);
            func_80041DD4(1, 0x59);
            func_8003D068(0x53, -0x64);
        }
    }
}

void func_800353AC(MainMenuSceneActor1 *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = func_80041FB4(1);
    func_800428C8(1);
    if (sp18 == 1) {
        if (++arg0->timer == 0x14) {
            arg0->timer = 0;
            func_80071824(arg0, func_80035320);
            func_80041DD4(1, 0x58);
        }
    }
}

void func_8003542C(MainMenuSceneActor1 *arg0) {
    s32 unused;
    s32 sp20;
    s32 temp_a1;

    sp20 = func_80041FB4(1);
    func_800428C8(1);
    if (sp20 == 0) {
        temp_a1 = (arg0->posX += 0x18000);
        func_8004209C(1, temp_a1, arg0->posY, arg0->posZ);
    }
    if (D_8010B1A2 == 0x3F) {
        arg0->timer = 0;
        func_80071824(arg0, func_800353AC);
        func_80041DD4(1, 0x57);
    }
}

void func_800354BC(MainMenuSceneActor1 *arg0) {
    s32 unused;
    s32 sp20;
    s32 var_v0;

    sp20 = func_80041FB4(1);
    if (arg0->timer < 5) {
        var_v0 = 1;
    } else {
        var_v0 = -1;
    }
    arg0->posX += 0x76000;
    arg0->posY += var_v0 << 19;
    arg0->timer = arg0->timer + 1;
    func_8004209C(1, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(1);
    if (sp20 == 1) {
        arg0->timer = 0;
        func_80071824(arg0, func_8003542C);
        func_80041DD4(1, 0x5D);
    }
}

void func_8003556C(MainMenuSceneActor1 *arg0) {
    func_80042034(1);
    arg0->posX += 0xFFF58000;
    if (D_8010B1A2 == 0x3C) {
        arg0->timer = 0;
        func_80071824(arg0, func_800354BC);
        func_80041DD4(1, 0x5C);
        D_8010B1A9 = 1;
    }
    func_8004209C(1, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(1);
}

void func_80035600(MainMenuSceneActor1 *arg0) {
    func_80042034(1);
    func_800428C8(1);
    if (D_8010B1A2 == 0x3A) {
        func_80071824(arg0, func_8003556C);
    }
}

void func_8003564C(MainMenuSceneActor1 *arg0) {
    s32 var_a1;

    func_80042034(1);
    arg0->posX += 0x24000;
    var_a1 = arg0->posX;
    if (var_a1 >= 0x1200000) {
        arg0->posX = 0x1200000;
        func_80071824(arg0, func_80035600);
        var_a1 = arg0->posX;
    }
    func_8004209C(1, var_a1, arg0->posY, arg0->posZ);
    func_800428C8(1);
}

void func_800356C8(MainMenuSceneActor1 *arg0) {
    func_80042034(1);
    func_800428C8(1);
    if (D_8010B1A2 == 0x38) {
        func_80071824(arg0, func_8003564C);
    }
}

void func_80035714(MainMenuSceneActor1 *arg0) {
    s32 var_a1;

    func_80042034(1);
    arg0->posX += 0xFFFB8000;
    var_a1 = arg0->posX;
    if (var_a1 < 0x01000001) {
        arg0->posX = 0x01000000;
        func_80071824(arg0, func_800356C8);
        D_8010B1A2 = 0x37;
        var_a1 = arg0->posX;
    }
    func_8004209C(1, var_a1, arg0->posY, arg0->posZ);
    func_800428C8(1);
}

void func_800357A4(MainMenuSceneActor1 *arg0) {
    if (D_8010B1A2 == 0x36) {
        func_80071824(arg0, func_80035714);
        arg0->posX = 0x1900000;
    }
}

void func_800357E8(MainMenuSceneActor1 *arg0) {
    s32 var_a1;

    func_80042034(1);
    arg0->posX += 0xFFFB8000;
    var_a1 = arg0->posX;
    if (var_a1 < (s32)0xFE700001) {
        arg0->posX = -0x1900000;
        func_80071824(arg0, func_800357A4);
        D_8010B1A2 = 0x33;
        var_a1 = arg0->posX;
    }
    func_8004209C(1, var_a1, arg0->posY, arg0->posZ);
    func_800428C8(1);
}

void func_80035878(MainMenuSceneActor1 *arg0) {
    if (func_80041FB4(1) == 0) {
        arg0->posZ = 0;
    }
    func_8004209C(1, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(1);
    if (D_8010B1A2 == 0x32) {
        func_80071824(arg0, func_800357E8);
        func_80041DD4(1, 0x52);
        arg0->rotY = 0xC00;
        func_800420FC(1, arg0->rotX, arg0->rotY, arg0->rotZ);
        D_8010B1A8 = 0;
        func_8003CD9C(0x13, -0x3A, 1, 1);
    }
}

void func_8003592C(MainMenuSceneActor1 *arg0) {
    func_800428C8(1);
    if (D_8010B1A2 == 0x30) {
        func_80071824(arg0, func_80035878);
        func_80041DD4(1, 0x51);
        func_8003C0A4(6, -0x40, 1, 0);
    }
}

void func_80035990(MainMenuSceneActor1 *arg0) {
    if (func_80041FB4(1) == 1) {
        arg0->timer++;
        if (arg0->timer == 0xA) {
            arg0->timer = 0;
            func_80071824(arg0, func_8003592C);
            D_8010B1A2 = 0x2F;
        }
    } else {
        arg0->posX += 0x20000;
    }
    func_8004209C(1, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(1);
}

void func_80035A2C(MainMenuSceneActor1 *arg0) {
    func_800428C8(1);
    if (D_8010B1A2 == 0x2E) {
        if (++arg0->timer == 0x14) {
            arg0->timer = 0;
            func_80071824(arg0, func_80035990);
            func_80041DD4(1, 0x50);
        }
    }
}

void func_80035AA4(MainMenuSceneActor1 *arg0) {
    s32 sp1c;
    s32 sp18;

    sp18 = func_80041FB4(1);
    func_800428C8(1);
    sp1c = (sp18 == 1);
    if (sp1c) {
        func_80071824(arg0, func_80035A2C);
        arg0->timer = 0;
    }
}

void func_80035AFC(MainMenuSceneActor1 *arg0) {
    s32 temp_a1;

    func_80041FB4(1);
    temp_a1 = (arg0->posX += 0x2800);
    if (1) {
        func_8004209C(1, temp_a1, arg0->posY, arg0->posZ);
        func_800428C8(1);
    }
    if (D_8010B1A2 == 0x2D) {
        func_80071824(arg0, func_80035AA4);
        func_80041DD4(1, 0x4F);
        func_8003C0A4(8, -0x40, 1, 0);
    }
}

void func_80035B88(MainMenuSceneActor1 *arg0) {
    func_80042034(1);
    func_800428C8(1);
    if (D_8010B1A2 == 0x2B) {
        func_80071824(arg0, func_80035AFC);
        func_80041DD4(1, 0x4D);
        createEffectTask(&func_8003C208, 0, 0x64);
    }
}

void func_80035BF4(MainMenuSceneActor1 *arg0) {
    s32 var_a1;

    func_80042034(1);
    arg0->posX += 0x120000;
    var_a1 = arg0->posX;
    if (var_a1 >= -0x600000) {
        arg0->posX = -0x600000;
        func_80071824(arg0, func_80035B88);
        D_8010B1A2 = 0x2A;
        var_a1 = arg0->posX;
    }
    func_8004209C(1, var_a1, arg0->posY, arg0->posZ);
    func_800428C8(1);
}

void func_80035C78(MainMenuSceneActor1 *arg0) {
    if (D_8010B1A2 == 0x29) {
        func_80071824(arg0, func_80035BF4);
        func_80041DD4(1, 0x4C);
        arg0->posZ = 0xA0000;
    }
}

void func_80035CC8(MainMenuSceneActor1 *arg0) {
    arg0->posX += 0xFFFE0000;
    func_8004209C(1, arg0->posX, arg0->posY, arg0->posZ);
    if (arg0->posX < -0x18FFFFF) {
        D_8010B1A2 = 0x12;
        arg0->posX = -0x1900000;
        func_80071824(arg0, func_80035C78);
    }
    func_80042034(1);
    func_800428C8(1);
}

void func_80035D54(MainMenuSceneActor1 *arg0) {
    func_80042034(1);
    func_800428C8(1);
    if (++arg0->timer == 0x32) {
        arg0->timer = 0;
        func_80071824(arg0, func_80035CC8);
    }
}

void func_80035DB4(MainMenuSceneActor1 *arg0) {
    func_80042034(1);
    arg0->posX += 0x60000;
    func_8004209C(1, arg0->posX, arg0->posY, arg0->posZ);
    if (arg0->posX >= -0x580000) {
        arg0->posX = -0x580000;
        func_80071824(arg0, func_80035D54);
    }
    func_800428C8(1);
}

void func_80035E28(MainMenuSceneActor1 *arg0) {
    if (D_8010B1A2 == 0x11) {
        arg0->posX = 0xFE700000;
        arg0->rotY = 0x400;
        func_8004209C(1, -0x1900000, arg0->posY, arg0->posZ);
        func_800420FC(1, arg0->rotX, arg0->rotY, arg0->rotZ);
        func_80071824(arg0, func_80035DB4);
        func_80041DD4(1, 0x1F);
    }
}

void func_80035EA8(MainMenuSceneActor1 *arg0) {
    func_80042034(1);
    arg0->posX += 0x60000;
    func_8004209C(1, arg0->posX, arg0->posY, arg0->posZ);
    if (arg0->posX >= 0x1900000) {
        arg0->posX = 0x1900000;
        func_80071824(arg0, func_80035E28);
        D_8010B1A2 = 0xF;
    }
    func_800428C8(1);
}

void func_80035F28(MainMenuSceneActor1 *arg0) {
    func_80042034(1);
    func_800428C8(1);
    if (D_8010B1A2 == 0xE) {
        func_80071824(arg0, func_80035EA8);
        func_80041DD4(1, 0x1F);
    }
}

void func_80035F80(MainMenuSceneActor1 *arg0) {
    if (func_80041FB4(1) == 1) {
        if (++arg0->timer == 0xC) {
            arg0->timer = 0;
            func_80071824(arg0, func_80035F28);
            func_80041DD4(1, 0x15);
            createEffectTask(&func_8003CA44, 0, 0x64);
        }
    }
    func_800428C8(1);
}

void func_8003600C(MainMenuSceneActor1 *arg0) {
    if (func_80041FB4(1) == 1) {
        arg0->timer += 1;
        if (arg0->timer < 3) {
            func_80041DD4(1, 0x13);
        }
        if (arg0->timer == 3) {
            arg0->timer = 0;
            D_8010B1A2 = 0xD;
            func_80071824(arg0, func_80035F80);
            func_80041DD4(1, 0x14);
            func_8003C0A4(8, -0x40, 1, 0);
        }
    }
    func_800428C8(1);
}

void func_800360CC(MainMenuSceneActor1 *arg0) {
    if (func_80041FB4(1) == 1) {
        func_80071824(arg0, func_8003600C);
        func_80041DD4(1, 0x13);
    }
    func_800428C8(1);
}

void func_8003611C(MainMenuSceneActor1 *arg0) {
    func_80041FB4(1);
    arg0->timer += 1;
    if (arg0->timer == 0x46) {
        arg0->timer = 0;
        func_80071824(arg0, func_800360CC);
        func_80041DD4(1, 0x12);
        func_8003D218(-0xB, -0x58, 1);
    }
    func_800428C8(1);
}

void func_80036198(MainMenuSceneActor1 *arg0) {
    s32 temp_a3;

    if (func_80041FB4(1) == 0) {
        temp_a3 = (arg0->posZ += (s32)0xFFFA0000);
        func_8004209C(1, arg0->posX, arg0->posY, temp_a3);
    }
    if (D_8010B1A2 == 0xC) {
        func_80071824(arg0, func_8003611C);
        func_80041DD4(1, 0x11);
    }
    func_800428C8(1);
}

void func_80036218(MainMenuSceneActor1 *arg0) {
    arg0->posX += 0x39435;
    if (D_8010B1A2 == 0xB) {
        func_80071824(arg0, func_80036198);
        func_80041DD4(1, 0x10);
        func_8003C0A4(0xA, -0x40, 1, 0);
    } else {
        func_8004209C(1, arg0->posX, arg0->posY, arg0->posZ);
        func_80042034(1);
    }
    func_800428C8(1);
}

void func_800362B4(MainMenuSceneActor1 *arg0) {
    if (D_8010B1A2 == 9) {
        func_80071824(arg0, func_80036218);
    }
}

void func_800362E8(MainMenuSceneActor1 *arg0) {
    arg0->posX = 0xFE700000;
    arg0->posY = 0;
    arg0->posZ = 0;
    arg0->rotX = 0;
    arg0->rotY = 0x400;
    arg0->rotZ = 0;
    func_80041D20(1, 1);
    func_80041DD4(1, 0xF);
    func_8004209C(1, arg0->posX, arg0->posY, arg0->posZ);
    func_800420FC(1, arg0->rotX, arg0->rotY, arg0->rotZ);
    func_80071824(arg0, func_800362B4);
}
