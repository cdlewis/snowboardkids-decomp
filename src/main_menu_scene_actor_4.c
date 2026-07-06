#include "common.h"

typedef struct {
    /* 0x00 */ char pad0[0x18];
    /* 0x18 */ s32 posX;
    /* 0x1C */ s32 posY;
    /* 0x20 */ s32 posZ;
    /* 0x24 */ s16 rotX;
    /* 0x26 */ s16 rotY;
    /* 0x28 */ s16 rotZ;
    /* 0x2A */ u16 timer;
} MainMenuSceneActor4;

typedef struct {
    /* 0x00 */ s32 posX;
    /* 0x04 */ s32 posY;
    /* 0x08 */ s32 posZ;
    /* 0x0C */ s8 unkC;
    /* 0x0D */ s8 actorId;
} MainMenuSceneActorShadow;

extern void func_8003CD9C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_800373AC(MainMenuSceneActorShadow *arg0);
extern void func_80041DD4(s32 arg0, s32 arg1);
extern void func_80041D20(s32 arg0, s32 arg1);
extern s32 func_80041FB4(s32 arg0);
extern void func_80042034(s32 arg0);
extern void func_8004209C(s32, s32, s32, s32);
extern void func_800420FC(s32, s16, s16, s16);
extern void func_800428C8(s32 arg0);
extern void func_80071824(void *task, void (*callback)());
extern MainMenuSceneActorShadow D_8010B1C0;
extern u16 D_8010B1A2;
extern s8 D_8010B1AC;
extern void func_800483FC(void *, void *, void *);
extern s32 D_80124898;
extern void func_80037070(void);
extern s32 D_8010B1C4;
extern void func_80071408(void *arg0, s32 arg1, s32 arg2);
extern void func_8003D384(void);

void func_800363B4(MainMenuSceneActor4 *arg0);
void func_80036418(MainMenuSceneActor4 *arg0);
void func_800364B8(MainMenuSceneActor4 *arg0);
void func_80036520(MainMenuSceneActor4 *arg0);
void func_800365B4(MainMenuSceneActor4 *arg0);
void func_80036674(MainMenuSceneActor4 *arg0);
void func_80036704(MainMenuSceneActor4 *arg0);
void func_800367A8(MainMenuSceneActor4 *arg0);
void func_800368BC(MainMenuSceneActor4 *arg0);
void func_80036930(MainMenuSceneActor4 *arg0);
void func_800369BC(MainMenuSceneActor4 *arg0);
void func_80036A4C(MainMenuSceneActor4 *arg0);
void func_80036AC4(MainMenuSceneActor4 *arg0);
void func_80036B54(MainMenuSceneActor4 *arg0);
void func_80036C14(MainMenuSceneActor4 *arg0);
void func_80036C8C(MainMenuSceneActor4 *arg0);
void func_80036D08(MainMenuSceneActor4 *arg0);
void func_80036DAC(MainMenuSceneActor4 *arg0);
void func_80036E58(MainMenuSceneActor4 *arg0);
void func_80036EBC(MainMenuSceneActor4 *arg0);
void func_80036F6C(MainMenuSceneActor4 *arg0);

void func_80036380(void) {
}

void func_80036388(MainMenuSceneActor4 *arg0) {
    func_80041FB4(4);
    func_800428C8(4);
}

void func_800363B4(MainMenuSceneActor4 *arg0) {
    func_80041FB4(4);
    func_800428C8(4);
    func_800373AC(&D_8010B1C0);
    if (D_8010B1A2 == 0x41) {
        func_80071824(arg0, func_80036388);
        func_80041DD4(4, 0x61);
    }
}

void func_80036418(MainMenuSceneActor4 *arg0) {
    s32 new_var2;
    s32 sp18;
    s32 new_var;
    u16 temp_t7;
    u16 temp_v0;

    sp18 = func_80041FB4(4);
    new_var2 = (new_var = 4);
    func_800428C8(new_var2);
    if (sp18 == 1) {
        temp_v0 = arg0->timer;
        temp_t7 = temp_v0;
        temp_t7 = temp_t7 + 1;
        if (temp_v0 < 0x1E) {
            arg0->timer = temp_t7;
            if ((temp_t7 & 0xFFFFU) == 0x1E) {
                D_8010B1A2 = 0x3E;
            }
        }
        if (D_8010B1A2 == 0x40) {
            arg0->timer = 0;
            func_80071824(arg0, func_800363B4);
            func_80041DD4(4, 0x60);
        }
    }
}

void func_800364B8(MainMenuSceneActor4 *arg0) {
    func_80042034(4);
    func_800428C8(4);
    func_800373AC(&D_8010B1C0);
    if (D_8010B1A2 == 0x3D) {
        arg0->timer = 0;
        func_80071824(arg0, func_80036418);
        func_80041DD4(4, 0x5F);
    }
}

void func_80036520(MainMenuSceneActor4 *arg0) {
    s32 limit = (s32)0xFF700000;

    func_80042034(4);
    arg0->posX += 0x48000;
    if (arg0->posX >= limit) {
        arg0->posX = limit;
        D_8010B1A2 = 0x3A;
        func_80071824(arg0, func_800364B8);
    }
    func_8004209C(4, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(4);
    func_800373AC(&D_8010B1C0);
}

void func_800365B4(MainMenuSceneActor4 *arg0) {
    u16 temp_t6;
    u16 temp_v0;

    temp_v0 = arg0->timer;
    if (temp_v0 < 0x23) {
        temp_t6 = temp_v0 + 1;
        arg0->timer = temp_t6;
        if ((temp_t6 & 0xFFFF) == 0x23) {
            D_8010B1A2 = 0x10;
        }
    } else if (D_8010B1A2 == 0x39) {
        arg0->posX = 0xFCA00000;
        func_80071824(arg0, func_80036520);
        func_80041DD4(4, 4);
        arg0->rotY = 0;
        func_800420FC(4, arg0->rotX, arg0->rotY, arg0->rotZ);
        D_8010B1C0.unkC = 9;
        D_8010B1C0.posX = 0xFFF20000;
        D_8010B1C0.posY = 0xFFF20000;
        D_8010B1C0.posZ = 0xA0000;
    }
}

void func_80036674(MainMenuSceneActor4 *arg0) {
    arg0->posX += (s32)0xFFFE8000;
    if (arg0->posX < (s32)0xFE700001) {
        arg0->posX = (s32)0xFE700000;
        arg0->posY = 0;
        func_80071824(arg0, func_800365B4);
    }
    func_8004209C(4, arg0->posX, arg0->posY, arg0->posZ);
    func_80042034(4);
    func_800428C8(4);
    func_800373AC(&D_8010B1C0);
}

void func_80036704(MainMenuSceneActor4 *arg0) {
    MainMenuSceneActor4 *new_var;
    s32 sp20;

    sp20 = func_80041FB4(4);
    func_800428C8(4);
    func_800373AC(&D_8010B1C0);
    if (sp20 == 1) {
        func_80071824(arg0, func_80036674);
        func_80041DD4(4, 3);
        arg0->rotY = 0xC00;
        func_800420FC(4, arg0->rotX, (new_var = arg0)->rotY, arg0->rotZ);
        D_8010B1C0.unkC = 9;
        D_8010B1C0.posX = 0xFFF20000;
        D_8010B1C0.posY = 0xFFF20000;
        D_8010B1C0.posZ = 0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_actor_4/func_800367A8.s")

void func_800368BC(MainMenuSceneActor4 *arg0) {
    u16 temp_v0 = arg0->timer;
    MainMenuSceneActor4 *temp_a2 = arg0;

    if (temp_v0 < 0x1E) {
        arg0->timer = temp_v0 + 1;
    } else if (func_80041FB4(4) == 1) {
        temp_a2->timer = 0;
        func_80071824(temp_a2, func_800367A8);
        func_80041DD4(4, 0x1D);
    }
    func_800428C8(4);
}

void func_80036930(MainMenuSceneActor4 *arg0) {
    func_80042034(4);
    func_800428C8(4);
    arg0->timer++;
    if (D_8010B1A2 == 0xF) {
        arg0->timer = 0;
        func_8004209C(4, arg0->posX, arg0->posY, arg0->posZ);
        func_80071824(arg0, func_800368BC);
        func_80041DD4(4, 0x1C);
        D_8010B1AC = 1;
    }
}

void func_800369BC(MainMenuSceneActor4 *arg0) {
    if (func_80041FB4(4) == 1) {
        arg0->timer = 0;
        func_80071824(arg0, func_80036930);
        func_80041DD4(4, 0xC);
        D_8010B1A2 = 0xC;
        D_8010B1AC = 0;
        func_8003CD9C(-0x24, -0x32, 4, 0);
        func_8003CD9C(0x10, -0x32, 4, 1);
    }
    func_800428C8(4);
}

void func_80036A4C(MainMenuSceneActor4 *arg0) {
    if (func_80041FB4(4) == 1) {
        arg0->timer++;
        if (arg0->timer == 0x14) {
            arg0->timer = 0;
            func_80071824(arg0, func_800369BC);
            func_80041DD4(4, 0xB);
        }
    }
    func_800428C8(4);
}

void func_80036AC4(MainMenuSceneActor4 *arg0) {
    func_80042034(4);
    arg0->posX += (s32)0xFFFB8000;
    if (arg0->posX < (s32)0xFF600001) {
        arg0->posX = (s32)0xFF600000;
        func_80071824(arg0, func_80036A4C);
        func_80041DD4(4, 0xA);
    }
    func_8004209C(4, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(4);
}

void func_80036B54(MainMenuSceneActor4 *arg0) {
    if (func_80041FB4(4) == 0) {
        s32 var_v0 = (arg0->timer < 5) ? 1 : -1;

        arg0->posY += var_v0 * 0x3E000;
        arg0->posX += (s32)0xFFF60000;
        func_8004209C(4, arg0->posX, arg0->posY, arg0->posZ);
    } else {
        arg0->posY = 0x6C000;
        func_8004209C(4, arg0->posX, 0x6C000, arg0->posZ);
        func_80071824(arg0, func_80036AC4);
        func_80041DD4(4, 9);
    }
    func_800428C8(4);
}

void func_80036C14(MainMenuSceneActor4 *arg0) {
    if (D_8010B1A2 < 0xA) {
        func_80042034(4);
    } else if (D_8010B1A2 == 0xB) {
        func_80071824(arg0, func_80036B54);
        func_80041DD4(4, 8);
        D_8010B1AC = 1;
    }
    func_800428C8(4);
    func_800373AC(&D_8010B1C0);
}

void func_80036C8C(MainMenuSceneActor4 *arg0) {
    if (D_8010B1A2 == 8) {
        func_80071824(arg0, func_80036C14);
        func_80041DD4(4, 7);
        D_8010B1C4 = (s32)0xFFE80000;
        D_8010B1AC = 0;
        func_8003CD9C(-0x24, -0x32, 4, 0);
    }
    func_800428C8(4);
    func_800373AC(&D_8010B1C0);
}

void func_80036D08(MainMenuSceneActor4 *arg0) {
    if (func_80041FB4(4) == 1) {
        arg0->timer++;
        if (arg0->timer < 6) {
            func_80041DD4(4, 6);
        }
    }
    if (arg0->timer == 6) {
        func_80071824(arg0, func_80036C8C);
        arg0->timer = 0;
        D_8010B1AC = 1;
        D_8010B1A2 = 7;
    }
    func_800428C8(4);
    func_800373AC(&D_8010B1C0);
}

void func_80036DAC(MainMenuSceneActor4 *arg0) {
    if ((func_80041FB4(4) == 1) && (D_8010B1A2 == 4)) {
        D_8010B1A2 = 5;
    }
    if (D_8010B1A2 == 6) {
        func_80071824(arg0, func_80036D08);
        func_80041DD4(4, 6);
        D_8010B1AC = 0;
        func_8003CD9C(-0x1C, -0x3A, 4, 0);
    }
    func_800428C8(4);
    func_800373AC(&D_8010B1C0);
}

void func_80036E58(MainMenuSceneActor4 *arg0) {
    func_80042034(4);
    if (D_8010B1A2 == 4) {
        func_80071824(arg0, func_80036DAC);
        func_80041DD4(4, 5);
    }
    func_800428C8(4);
    func_800373AC(&D_8010B1C0);
}

void func_80036EBC(MainMenuSceneActor4 *arg0) {
    arg0->posX += 0x24000;
    if (arg0->posX >= 0x100000) {
        arg0->posX = 0x100000;
        D_8010B1A2 = 2;
        func_80071824(arg0, func_80036E58);
        func_8004209C(4, arg0->posX, arg0->posY, arg0->posZ);
        func_80041DD4(4, 4);
    } else {
        func_8004209C(4, arg0->posX, arg0->posY, arg0->posZ);
        func_80042034(4);
    }
    func_800428C8(4);
    func_800373AC(&D_8010B1C0);
}

void func_80036F6C(MainMenuSceneActor4 *arg0) {
    if (D_8010B1A2 == 1) {
        func_80071824(arg0, func_80036EBC);
        func_80071408(&func_8003D384, 0, 0x64);
    }
}

void func_80036FB4(MainMenuSceneActor4 *arg0) {
    arg0->posX = (s32)0xFE700000;
    arg0->posY = 0;
    arg0->posZ = 0;
    arg0->rotX = 0;
    arg0->rotY = 0x400;
    arg0->rotZ = 0;
    arg0->timer = 0;
    func_80041D20(4, 4);
    func_80041DD4(4, 3);
    func_8004209C(4, arg0->posX, arg0->posY, arg0->posZ);
    func_800420FC(4, arg0->rotX, arg0->rotY, arg0->rotZ);
    D_8010B1C0.actorId = 4;
    D_8010B1C0.unkC = 9;
    D_8010B1C0.posX = (s32)0xFFF20000;
    D_8010B1C0.posY = (s32)0xFFF20000;
    D_8010B1C0.posZ = 0;
    func_80071824(arg0, func_80036F6C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_actor_4/func_80037070.s")

void func_800373AC(MainMenuSceneActorShadow *arg0) {
    func_800483FC(&D_80124898, func_80037070, arg0);
}
