#include "common.h"
#include "effect_task_scheduler.h"
#include "main_menu_effects.h"
#include "main_menu_scene_actor_3.h"
#include "main_menu_scene_model.h"
#include "main_menu_scene_renderer.h"
#include "race_position_ui.h"

typedef s16 FixedMatrix3sScratch[0x10];
typedef s32 MatrixWordCopy[8];

struct MainMenuSceneActor3 {
    char pad[0x18];
    s32 x;
    s32 y;
    s32 z;
    s16 rotX;
    s16 rotY;
    s16 rotZ;
    u16 timer;
    u16 animTimer;
    u16 textureId;
    u16 paletteId;
};

struct MainMenuSceneEffect3 {
    char pad0[0x10];
    u16 mode;
    char pad12[0x6];
    char displayObject[0x14];
    s32 x;
    s32 y;
    s32 z;
    u16 textureId;
    u16 paletteId;
    u16 timer;
};

extern void func_800483FC(void *, void *, void *);
extern s32 func_8004885C(void *);
#ifdef NON_MATCHING
extern void func_80097BAC();
extern void func_80097C84();
extern void func_80097CF0();
extern void func_8009853C();
#else
extern void func_8009853C(void *, s32, s32, void *);
#endif
extern u16 D_8010B1A2;
extern u8 D_8010B1A4;
extern s32 D_80124898;
extern void *D_8010ADE0;

void func_800394E0(s32 arg0) {
    func_80041FB4(3);
    func_800428C8(3);
}

void func_8003950C(MainMenuSceneActor3 *arg0) {
    func_80041FB4(3);
    func_800428C8(3);
    if (D_8010B1A2 == 0x41) {
        func_80071824(arg0, func_800394E0);
        func_80041DD4(3, 0x5B);
        arg0->rotY = 0xC00;
        func_800420FC(3, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
}

void func_80039584(MainMenuSceneActor3 *arg0) {
    u16 *timerPtr;
    s32 sp18;
    int unused;
    u16 temp_t8;

    sp18 = func_80041FB4(3);
    func_800428C8(3);
    if (sp18 == 1) {
        timerPtr = &arg0->timer;
        arg0->timer = temp_t8 = (*timerPtr & 0xFFFF) + 1;
        if ((temp_t8 & 0xFFFF) == 0x14) {
            timerPtr = &arg0->timer;
            *timerPtr = 0;
            func_80071824(arg0, func_8003950C);
            func_80041DD4(3, 0x5A);
            D_8010B1A2 = 0x40;
        }
    }
}

void func_80039610(MainMenuSceneActor3 *arg0) {
    u16 *timerPtr;
    s32 sp18;
    int unused;
    u16 temp_t8;

    sp18 = func_80041FB4(3);
    func_800428C8(3);
    if (sp18 == 1) {
        timerPtr = &arg0->timer;
        arg0->timer = temp_t8 = (*timerPtr & 0xFFFF) + 1;
        if ((temp_t8 & 0xFFFF) == 0x1B) {
            timerPtr = &arg0->timer;
            *timerPtr = 0;
            func_80071824(arg0, func_80039584);
            func_80041DD4(3, 0x59);
            func_8003D068(0x30, -0x5B);
        }
    }
}

void func_8003969C(MainMenuSceneActor3 *arg0) {
    u16 temp_t7;

    if (func_80041FB4(3) == 1) {
        temp_t7 = (arg0->timer & 0xFFFF) + 1;
        arg0->timer = temp_t7;
        if ((temp_t7 & 0xFFFF) == 0x14) {
            arg0->timer = 0;
            func_80071824(arg0, func_80039610);
            func_80041DD4(3, 0x58);
        }
    } else {
        arg0->z += 0x80000;
    }
    func_8004209C(3, arg0->x, arg0->y, arg0->z);
    func_800428C8(3);
}

void func_8003973C(MainMenuSceneActor3 *arg0) {
    if (func_80041FB4(3) == 0) {
        arg0->x = arg0->x + 0x18000;
        func_8004209C(3, arg0->x, arg0->y, arg0->z);
    }
    if (D_8010B1A2 == 0x3F) {
        arg0->timer = 0;
        func_80071824(arg0, func_8003969C);
        func_80041DD4(3, 0x57);
    }
    func_800428C8(3);
}

void func_800397C4(MainMenuSceneActor3 *arg0) {
    s32 unused;
    s32 sp20;
    s32 var_v0;

    sp20 = func_80041FB4(3);
    if (arg0->timer < 5) {
        var_v0 = 1;
    } else {
        var_v0 = -1;
    }
    arg0->x += 0x76000;
    arg0->y += var_v0 << 19;
    arg0->z += 0xFFFA0000;
    arg0->timer = arg0->timer + 1;
    func_8004209C(3, arg0->x, arg0->y, arg0->z);
    func_800428C8(3);
    if (sp20 == 1) {
        arg0->timer = 0;
        func_80071824(arg0, func_8003973C);
        func_80041DD4(3, 0x5D);
    }
}

void func_80039880(MainMenuSceneActor3 *arg0) {
    func_80042034(3);
    arg0->x += 0xFFF58000;
    if (D_8010B1A2 == 0x3C) {
        arg0->timer = 0;
        func_80071824(arg0, func_800397C4);
        func_80041DD4(3, 0x5C);
        arg0->rotY = 0xC00;
        func_800420FC(3, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
    func_8004209C(3, arg0->x, arg0->y, arg0->z);
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
}

void func_80039930(MainMenuSceneActor3 *arg0) {
    func_80042034(3);
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
    if (D_8010B1A2 == 0x3A) {
        func_80071824(arg0, func_80039880);
    }
}

void func_8003998C(MainMenuSceneActor3 *arg0) {
    s32 var_a1;

    func_80042034(3);
    var_a1 = (arg0->x & 0xFFFFFFFFFFFFFFFFu) + 0x24000;
    arg0->x = var_a1;
    if (var_a1 >= 0xE00000) {
        arg0->x = 0xE00000;
        func_80071824(arg0, func_80039930);
        var_a1 = arg0->x;
    }
    func_8004209C(3, var_a1, arg0->y, arg0->z);
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
}

void func_80039A14(MainMenuSceneActor3 *arg0) {
    func_80042034(3);
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
    if (D_8010B1A2 == 0x38) {
        func_80071824(arg0, func_8003998C);
    }
}

void func_80039A70(MainMenuSceneActor3 *arg0) {
    s32 var_a1;
    MainMenuSceneActor3 *new_var;

    func_80042034(3);
    new_var = arg0;
    var_a1 = (new_var->x += 0xFFFB8000);
    if (var_a1 < 0xA00001) {
        new_var->x = 0xA00000;
        func_80071824(new_var, func_80039A14);
        var_a1 = arg0->x;
    } else if ((var_a1 < 0x1300001) && (D_8010B1A2 == 0x35)) {
        D_8010B1A2 = 0x36;
        var_a1 = arg0->x;
    }
    func_8004209C(3, var_a1, arg0->y, arg0->z);
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
}

void func_80039B38(MainMenuSceneActor3 *arg0) {
    if (D_8010B1A2 == 0x35) {
        func_80071824(arg0, func_80039A70);
        arg0->x = 0x01900000;
        arg0->z = 0xFFF80000;
    }
}

// func_80039B84 best match: 99.453%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_actor_3/func_80039B84.s")

#ifdef NON_MATCHING
void func_80039B84(MainMenuSceneActor3 *arg0) {
    s32 new_var;
    s32 temp_t7;
    s32 var_a1;
    s32 temp_v0;

    func_80042034(3);
    temp_t7 = arg0->x + -0x48000;
    arg0->x = temp_t7;
    var_a1 = temp_t7;
    if ((temp_t7 < -0x400000) != 0) {
        goto block_1;
    }
    var_a1 = temp_t7;
    arg0->z += -0x48000;
    var_a1 = arg0->x;
    goto block_2;

block_1:
    temp_v0 = arg0->z;
    if (temp_v0 < 0) {
        var_a1 = arg0->x;
        arg0->z = temp_v0 + 0x70000;
    }

block_2:
    if (var_a1 < -0x1900000 + 1) {
        arg0->x = -0x1900000;
        func_80071824(arg0, func_80039B38);
        var_a1 = arg0->x;
    }
    if ((var_a1 < -0xF00000 + 1) && (D_8010B1A2 == 0x31)) {
        D_8010B1A2 = 0x32;
        var_a1 = arg0->x;
    }
    new_var = 0xB;
    func_8004209C(3, var_a1, temp_t7 = arg0->y, arg0->z);
    func_8004298C(3, arg0->textureId, arg0->paletteId, new_var);
}
#endif

void func_80039C84(MainMenuSceneActor3 *arg0) {
    func_80041FB4(3);
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
    if (D_8010B1A2 == 0x31) {
        func_80071824(arg0, func_80039B84);
        func_80041DD4(3, 0x4B);
    }
}

void func_80039CEC(MainMenuSceneActor3 *arg0) {
    s32 sp;
    s32 temp;

    sp = func_80041FB4(3);
    if (sp == 0) {
        arg0->x += 0xFFFF6000;
    } else {
        temp = (arg0->timer += 1);
        if ((temp % 28) == 0) {
            if ((temp / 28) == 3) {
                func_80071824(arg0, func_80039C84);
                D_8010B1A2 = 0x2E;
            } else {
                func_80041DD4(3, 0x4A);
            }
        }
    }
    func_8004209C(3, arg0->x, arg0->y, arg0->z);
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
}

void func_80039E08(MainMenuSceneActor3 *arg0) {
    arg0->timer++;
    if (arg0->timer == 0xF) {
        arg0->timer = 0;
        func_80071824(arg0, func_80039CEC);
        func_80041DD4(3, 0x4A);
    }
    func_800428C8(3);
}

void func_80039E5C(MainMenuSceneActor3 *arg0) {
    s32 sp;

    sp = func_80041FB4(3);
    arg0->x += 0xFFFD8000;
    if ((sp == 1) && (arg0->x < 0x800001)) {
        arg0->timer = 0;
        func_80071824(arg0, func_80039E08);
        D_8010B1A2 = 0x2D;
    } else if ((sp == 1) && (arg0->x >= 0x800001)) {
        func_80041DD4(3, 0x4B);
    }
    func_8004209C(3, arg0->x, arg0->y, arg0->z);
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
}

void func_80039F2C(MainMenuSceneActor3 *arg0) {
    if (D_8010B1A2 == 0x2C) {
        func_80071824(arg0, func_80039E5C);
        func_80041DD4(3, 0x4B);
        arg0->timer = 0;
    }
}

void func_80039F7C(MainMenuSceneActor3 *arg0) {
    s32 var_a1;

    func_80042034(3);
    var_a1 = arg0->x += 0x28000;
    if (var_a1 >= 0x1900000) {
        arg0->x = 0x1900000;
        func_80071824(arg0, func_80039F2C);
        D_8010B1A2 = 0x28;
        var_a1 = arg0->x;
    }
    func_8004209C(3, var_a1, arg0->y, arg0->z);
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
}

void func_8003A010(MainMenuSceneActor3 *arg0) {
    func_80042034(3);
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
    arg0->timer++;
    if (arg0->timer == 0x41) {
        arg0->timer = 0;
        func_80071824(arg0, func_80039F7C);
    }
}

void func_8003A078(MainMenuSceneActor3 *arg0) {
    s32 var_a1;

    func_80042034(3);
    var_a1 = arg0->x += 0xFFFD8000;
    if (var_a1 < 0x800001) {
        arg0->x = 0x800000;
        func_80071824(arg0, func_8003A010);
        var_a1 = arg0->x;
    }
    func_8004209C(3, var_a1, arg0->y, arg0->z);
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
}

void func_8003A108(MainMenuSceneActor3 *arg0) {
    s32 temp_a1;
    u16 temp_t9;

    func_80042034(3);
    temp_a1 = arg0->x += 0x20000;
    func_8004209C(3, temp_a1, arg0->y, arg0->z);
    temp_t9 = (arg0->timer & 0xFFFF) + 1;
    arg0->timer = temp_t9;
    if ((temp_t9 & 0xFFFF) == 0x28) {
        arg0->timer = 0;
        func_80071824(arg0, func_8003A078);
    }
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
}

void func_8003A190(MainMenuSceneActor3 *arg0) {
    s32 temp_a1;
    u16 temp_t9;

    func_80042034(3);
    temp_a1 = arg0->x += 0xFFFD8000;
    func_8004209C(3, temp_a1, arg0->y, arg0->z);
    temp_t9 = (arg0->timer & 0xFFFF) + 1;
    arg0->timer = temp_t9;
    if ((temp_t9 & 0xFFFF) == 0x2D) {
        arg0->timer = 0;
        func_80071824(arg0, func_8003A108);
    }
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
}

void func_8003A21C(MainMenuSceneActor3 *arg0) {
    s32 temp_a1;
    u16 temp_t9;

    func_80042034(3);
    temp_a1 = arg0->x += 0x28000;
    func_8004209C(3, temp_a1, arg0->y, arg0->z);
    temp_t9 = (arg0->timer & 0xFFFF) + 1;
    arg0->timer = temp_t9;
    if ((temp_t9 & 0xFFFF) == 0x1E) {
        arg0->timer = 0;
        func_80071824(arg0, func_8003A190);
    }
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
}

void func_8003A2A8(MainMenuSceneActor3 *arg0) {
    func_80042034(3);
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
    if (D_8010B1A2 == 0x23) {
        arg0->timer++;
        if (arg0->timer == 0x2D) {
            arg0->timer = 0;
            func_80071824(arg0, func_8003A21C);
        }
    }
}

void func_8003A324(MainMenuSceneActor3 *arg0) {
    s32 var_a1;

    func_80042034(3);
    var_a1 = arg0->x += 0x28000;
    if (var_a1 >= 0x800000) {
        arg0->timer = 0;
        arg0->x = 0x800000;
        func_80071824(arg0, func_8003A2A8);
        var_a1 = arg0->x;
    }
    if ((var_a1 >= 0x200000) && (D_8010B1A2 == 0x20)) {
        D_8010B1A2 = 0x21;
        var_a1 = arg0->x;
    }
    func_8004209C(3, var_a1, arg0->y, arg0->z);
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
}

void func_8003A3E0(MainMenuSceneActor3 *arg0) {
    s32 var_a1;

    func_80042034(3);
    var_a1 = arg0->x += 0xFFFA0000;
    if (var_a1 < -0x9FFFFF) {
        arg0->x = 0xFF600000;
        func_80071824(arg0, func_8003A324);
        var_a1 = arg0->x;
    }
    func_8004209C(3, var_a1, arg0->y, arg0->z);
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
}

void func_8003A46C(MainMenuSceneActor3 *arg0) {
    u16 temp_t8;
    s32 sp20;

    sp20 = func_80041FB4(3);
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
    if (sp20 == 0) {
        if (D_8010B1A2 == 0x1D) {
            temp_t8 = (arg0->timer & 0xFFFF) + 1;
            arg0->timer = temp_t8;
            if ((temp_t8 & 0xFFFF) == 3) {
                D_8010B1A2 = 0x1E;
                arg0->timer = 0;
            }
        }
    }
    if (sp20 == 1) {
        func_80041DD4(3, 0x4A);
    }
    if (D_8010B1A2 == 0x20) {
        arg0->timer = 0;
        func_80071824(arg0, func_8003A3E0);
        func_80041DD4(3, 0x4B);
    }
}

void func_8003A538(MainMenuSceneActor3 *arg0) {
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
    if (D_8010B1A2 == 0x1D) {
        func_80071824(arg0, func_8003A46C);
        func_80041DD4(3, 0x4A);
    }
}

void func_8003A598(MainMenuSceneActor3 *arg0) {
    s32 var_a1;

    func_80042034(3);
    var_a1 = arg0->x += 0xFFFE8000;
    if (var_a1 < 0xC80001) {
        arg0->x = 0xC80000;
        func_80071824(arg0, func_8003A538);
        D_8010B1A2 = 0x1C;
        var_a1 = arg0->x;
    }
    func_8004209C(3, var_a1, arg0->y, arg0->z);
    func_8004298C(3, arg0->textureId, arg0->paletteId, 0xB);
}

void func_8003A634(MainMenuSceneActor3 *arg0) {
    s32 pad;
    s32 sp20;

    sp20 = func_80041FB4(3);
    func_80042920(3, arg0->textureId, arg0->paletteId);
    if (sp20 == 1) {
        func_80071824(arg0, func_8003A598);
        func_80041DD4(3, 0x49);
        arg0->x += 0xFFEC0000;
    }
}

void func_8003A6A8(MainMenuSceneActor3 *arg0) {
    s32 pad;
    s32 sp18;

    sp18 = func_80041FB4(3);
    func_80042920(3, arg0->textureId, arg0->paletteId);
    if (sp18 == 1) {
        func_80071824(arg0, func_8003A634);
        func_80041DD4(3, 0x42);
    }
}

void func_8003A70C(MainMenuSceneActor3 *arg0) {
    func_800428C8(3);
    arg0->timer++;
    if (arg0->timer == 0x1E) {
        arg0->timer = 0;
        D_8010B1A2 = 0x1B;
        func_80071824(arg0, func_8003A6A8);
        func_80041DD4(3, 0x41);
    }
}

void func_8003A77C(MainMenuSceneActor3 *arg0) {
    if (func_80041FB4(3) == 1) {
        func_80071824(arg0, func_8003A70C);
    } else {
        arg0->x = arg0->x + 0x40000;
        func_8004209C(3, arg0->x, arg0->y, arg0->z);
    }
    func_800428C8(3);
}

void func_8003A7EC(MainMenuSceneActor3 *arg0) {
    s32 temp_v0;
    u16 temp_t7;

    temp_v0 = func_80041FB4(3);
    temp_t7 = (arg0->timer & 0xFFFF) + 1;
    arg0->timer = temp_t7;
    arg0->x += 0x70000;
    if ((temp_t7 & 0xFFFF) < 5) {
        arg0->y += 0x50000;
    } else {
        arg0->y += 0xFFFB0000;
    }
    if (temp_v0 == 1) {
        arg0->timer = 0;
        func_80071824(arg0, func_8003A77C);
        func_80041DD4(3, 0x40);
    }
    func_8004209C(3, arg0->x, arg0->y, arg0->z);
    func_800428C8(3);
}

void func_8003A8A4(MainMenuSceneActor3 *arg0) {
    s32 pad;
    s32 sp20;

    arg0->x += 0x100000;
    func_8004209C(3, arg0->x, arg0->y, arg0->z);
    sp20 = func_80041FB4(3);
    func_800428C8(3);
    arg0->y += 0x50000;
    if (sp20 == 1) {
        arg0->timer = 0;
        D_8010B1A2 = 0x1A;
        func_80071824(arg0, func_8003A7EC);
        func_80041DD4(3, 0x3F);
    }
}

// func_8003A944 best match: 97.333%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_actor_3/func_8003A944.s")

#ifdef NON_MATCHING
void func_8003A944(MainMenuSceneActor3 *arg0) {
    s32 pad;
    s32 sp20;
    s32 i;
    s32 count;

    count = 1;
    if (arg0->animTimer < 3) {
        count = count;
    } else {
        count = 2;
    }

    for (i = 0; i < count; i++) {
        sp20 = func_80041FB4(3);
    }

    func_800428C8(3);
    if (sp20 == 1) {
        D_8010B1A4++;
        func_80071824(arg0, func_8003AC00);
    }
}
#endif

void func_8003A9E0(MainMenuSceneActor3 *arg0) {
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
            func_80041FB4(3);
            func_800428C8(3);
            temp_t8 = (arg0->timer & 0xFFFF) + 1;
            arg0->timer = temp_t8;
            if ((temp_t8 & 0xFFFF) < 5) {
                if ((D_8010B1A4 == 1) || (D_8010B1A4 == 5)) {
                    arg0->x = (s32) (arg0->x + 0x100000);
                } else {
                    arg0->x = (s32) (arg0->x + 0xFFF00000);
                }
                if ((s32) arg0->timer < 3) {
                    arg0->y = (s32) (arg0->y + 0x90000);
                } else {
                    arg0->y = (s32) (arg0->y + 0xFFF70000);
                }
                func_8004209C(3, arg0->x, arg0->y, arg0->z);
                var_s1 += 1;
                if ((arg0->animTimer == 0xD) && (D_8010B1A4 == 1) && (arg0->timer == 3)) {
                    D_8010B1A4 = 5;
                }
                continue;
            } else {
                arg0->timer = 0U;
                arg0->y = 0;
                func_8004209C(3, arg0->x, 0, arg0->z);
                func_80071824(arg0, func_8003A944);
                if ((D_8010B1A4 == 1) || (D_8010B1A4 == 5)) {
                    func_80041DD4(3, 0x3A);
                } else {
                    func_80041DD4(3, 0x3C);
                    arg0->animTimer = (u16) (arg0->animTimer + 1);
                }
                if (arg0->animTimer == 0xD) {
                    if (D_8010B1A4 == 5) {
                        func_80071824(arg0, func_8003A8A4);
                        func_80041DD4(3, 0x3E);
                        arg0->animTimer = 0U;
                    }
                }
                return;
            }
        } while (var_s1 != var_s4);
    }
}

void func_8003AC00(MainMenuSceneActor3 *arg0) {
    func_800428C8(3);
    if ((D_8010B1A4 == 1) || (D_8010B1A4 == 3)) {
        func_80071824(arg0, func_8003A9E0);
        if (D_8010B1A4 == 1) {
            arg0->rotY = 0x400;
            if (arg0->timer == 0xF) {
                func_80041DD4(3, 0x39);
                arg0->timer = 2;
                arg0->y = 0x120000;
            } else {
                func_80041DD4(3, 0x3D);
            }
        } else {
            arg0->rotY = 0x400;
            func_80041DD4(3, 0x3B);
        }
        func_800420FC(3, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
}

void func_8003ACD0(MainMenuSceneActor3 *arg0) {
    func_800428C8(3);
    arg0->timer++;
    if (arg0->timer == 0xF) {
        D_8010B1A2 = 0x19;
        func_80071824(arg0, func_8003AC00);
        func_8004209C(3, arg0->x, arg0->y, arg0->z);
    }
}

void func_8003AD40(MainMenuSceneActor3 *arg0) {
    void (*new_var)(MainMenuSceneActor3 *);
    s32 sp18;
    u16 temp_t8;

    sp18 = func_80041FB4(3);
    func_800428C8(3);
    if (sp18 == 1) {
        temp_t8 = (arg0->timer & 0xFFFF) + 1;
        arg0->timer = temp_t8;
        if ((temp_t8 & 0xFFFF) == 0xA) {
            arg0->timer = 0;
            new_var = func_8003ACD0;
            D_8010B1A2 = 0x18;
            func_80071824(arg0, new_var);
        }
    }
}

void func_8003ADC0(MainMenuSceneActor3 *arg0) {
    s32 var_a1;

    func_80042034(3);
    func_800428C8(3);
    var_a1 = arg0->x += (s32)0xFFFD0000;
    arg0->z = arg0->z + (s32)0xFFFF0000;
    if (var_a1 < 0x2D0001) {
        arg0->x = 0x2D0000;
        arg0->z = (s32)0xFFFE0000;
        func_80071824(arg0, func_8003AD40);
        func_80041DD4(3, 0x38);
        D_8010B1A2 = 0x17;
        arg0->rotY = 0x400;
        func_800420FC(3, arg0->rotX, arg0->rotY, arg0->rotZ);
        var_a1 = arg0->x;
    }
    func_8004209C(3, var_a1, arg0->y, arg0->z);
}

void func_8003AE8C(MainMenuSceneActor3 *arg0) {
    u16 temp;
    s32 sp20;

    sp20 = func_80041FB4(3);
    if (arg0->animTimer == 0) {
        func_800428C8(3);
    } else {
        func_80042A00(3);
    }
    temp = (arg0->animTimer & 0xFFFF) + 1;
    arg0->animTimer = temp;
    if (sp20 == 1) {
        arg0->animTimer = 0;
        temp = (arg0->timer & 0xFFFF) + 1;
        arg0->timer = temp;
        if ((temp & 0xFFFF) < 2) {
            func_80041DD4(3, 0x37);
        } else {
            arg0->timer = 0;
            func_80071824(arg0, func_8003ADC0);
            func_80041DD4(3, 0xF);
            arg0->rotY = 0xC00;
            func_800420FC(3, arg0->rotX, arg0->rotY, arg0->rotZ);
        }
    }
}

void func_8003AF6C(MainMenuSceneActor3 *arg0) {
    if (func_80041FB4(3) == 1) {
        arg0->animTimer = 0;
        func_80071824(arg0, func_8003AE8C);
        func_80041DD4(3, 0x37);
    }
    func_800428C8(3);
}

void func_8003AFC0(MainMenuSceneActor3 *arg0) {
    u16 temp_t9;

    if (func_80041FB4(3) == 0) {
        arg0->x = arg0->x + 0x48000;
        func_8004209C(3, arg0->x, arg0->y, arg0->z);
        temp_t9 = (arg0->timer & 0xFFFF) + 1;
        arg0->timer = temp_t9;
        if ((temp_t9 & 0xFFFF) == 2) {
            func_8003C0A4(-0x14, -0x40, 3, 1);
        }
    }
    if (D_8010B1A2 == 0x16) {
        arg0->timer = 0;
        func_80071824(arg0, func_8003AF6C);
        func_80041DD4(3, 0x36);
    }
    func_800428C8(3);
}

void func_8003B074(MainMenuSceneActor3 *arg0) {
    u16 temp_t0;
    s32 sp20;

    sp20 = func_80041FB4(3);
    func_800428C8(3);
    if (D_8010B1A2 == 0x15) {
        arg0->timer = 0;
        func_80071824(arg0, func_8003AFC0);
        arg0->rotY = 0x400;
        func_800420FC(3, arg0->rotX, arg0->rotY, arg0->rotZ);
        func_80041DD4(3, 0x28);
    } else if (sp20 == 1) {
        temp_t0 = (arg0->timer & 0xFFFF) + 1;
        arg0->timer = temp_t0;
        if ((temp_t0 & 0xFFFF) == 0x10) {
            arg0->timer = 0;
            func_80041DD4(3, 0x27);
        }
    }
}

void func_8003B134(MainMenuSceneActor3 *arg0) {
    s32 temp_v0;
    u16 temp_t2;

    temp_v0 = func_80041FB4(3);
    arg0->x += 0xFFFB8000;
    if (arg0->x < 0xE00001) {
        arg0->x = 0xE00000;
        func_80071824(arg0, func_8003B074);
        D_8010B1A2 = 0x14;
    } else if (temp_v0 == 1) {
        temp_t2 = (arg0->timer & 0xFFFF) + 1;
        arg0->timer = temp_t2;
        if ((temp_t2 & 0xFFFF) == 0x10) {
            arg0->timer = 0;
            func_80041DD4(3, 0x27);
        }
    }
    func_8004209C(3, arg0->x, arg0->y, arg0->z);
    func_800428C8(3);
}

void func_8003B1F8(MainMenuSceneActor3 *arg0) {
    if (D_8010B1A2 == 0x13) {
        func_80071824(arg0, func_8003B134);
        func_8003B308(arg0->x + 0x48000, 0x480000, 0xFFF70000, arg0->textureId, arg0->paletteId, 2);
    }
}

void func_8003B264(MainMenuSceneActor3 *arg0) {
    arg0->x = 0x03B48000;
    arg0->y = 0;
    arg0->z = 0;
    arg0->rotX = 0;
    arg0->rotY = 0;
    arg0->rotZ = 0;
    arg0->timer = 0;
    arg0->animTimer = 0;
    arg0->textureId = 0;
    arg0->paletteId = 3;
    func_80041D20(3, 3);
    func_80041DD4(3, 0x27);
    func_8004209C(3, arg0->x, arg0->y, arg0->z);
    func_800420FC(3, arg0->rotX, arg0->rotY, arg0->rotZ);
    func_80071824(arg0, func_8003B1F8);
}

void func_8003B308(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u8 arg5) {
    MainMenuSceneEffect3 *sp1C;
    MainMenuSceneEffect3 *temp_v0;

    temp_v0 = func_80071664((void (*)(void *))func_8003B740, 0, 0x64, arg5);
    D_8010ADE0 = temp_v0;
    temp_v0->x = arg0;
    temp_v0->y = arg1;
    temp_v0->z = arg2;
    sp1C = temp_v0;
    func_8009853C(&temp_v0->displayObject, 0x400, 0x400, &D_8010ADE0);
    sp1C->textureId = arg3;
    sp1C->paletteId = arg4;
}

void func_8003B39C(MainMenuSceneEffect3 *arg0) {
    s32 temp = func_8004885C(&arg0->displayObject);
    if (temp != 0) {
        func_8007C130((void *)temp, arg0->textureId, arg0->paletteId);
    }
}

void func_8003B3E0(MainMenuSceneEffect3 *arg0) {
    if (D_8010B1A2 == 0x1B) {
        func_800716E4(arg0);
    } else {
        func_800483FC(&D_80124898, func_8003B39C, arg0);
    }
}

// func_8003B430 best match: 91.989% (base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_actor_3/func_8003B430.s")

#ifdef NON_MATCHING
void func_8003B430(MainMenuSceneEffect3 *arg0) {
    FixedMatrix3sScratch sp48;
    FixedMatrix3sScratch sp28;
    void *sp24[1];
    s16 temp_t7;
    s32 timer;
    u16 timer2;

    temp_t7 = arg0->timer + 1;
    arg0->timer = temp_t7;
    if (arg0->mode == 1) {
        timer = temp_t7 & 0xFFFF;
        if (timer < 6) {
            arg0->y = (arg0->y - (timer * 0x15000)) + 0x80000;
        } else {
            arg0->y = (arg0->y - (timer * 0x15000)) + 0x124000;
        }
        arg0->x += 0xC0000;
    } else {
        timer2 = arg0->timer;
        if ((s32) timer2 < 5) {
            arg0->y = (arg0->y - (timer2 << 0xF)) + 0x40000;
        } else {
            arg0->y = (arg0->y - (timer2 * 0x3800)) + 0xFFFE8800;
        }
        arg0->x += 0x1D000;
    }

    func_8009853C(sp48, 0x400, 0x400);
    if (arg0->mode == 1) {
        func_80097C84(sp28, 0x1000 - (arg0->timer << 6));
    } else {
        func_80097C84(sp28, 0x1000 - (arg0->timer * 0x28));
    }

    sp24[0] = &arg0->displayObject;
    func_80097CF0(sp48, sp28, sp24[0]);

    if (arg0->mode == 1) {
        if (arg0->x >= 0x1900000) {
            arg0->timer = 0;
            func_80071824(arg0, func_8003B3E0);
        }
    } else if (arg0->timer == 0x14) {
        arg0->timer = 0;
        arg0->y = 0x8BD1E;
        func_80071824(arg0, func_8003B3E0);
        func_8009853C(sp48, 0x400, 0x400);
        func_80097C84(sp28, 0xC00);
        func_80097CF0(sp48, sp28, sp24[0]);
        func_80097BAC(sp48, 0x300);
        ((MatrixWordCopy *) sp28)[0][0] = ((MatrixWordCopy *) sp24[0])[0][0];
        ((MatrixWordCopy *) sp28)[0][1] = ((MatrixWordCopy *) sp24[0])[0][1];
        ((MatrixWordCopy *) sp28)[0][2] = ((MatrixWordCopy *) sp24[0])[0][2];
        ((MatrixWordCopy *) sp28)[0][3] = ((MatrixWordCopy *) sp24[0])[0][3];
        ((MatrixWordCopy *) sp28)[0][4] = ((MatrixWordCopy *) sp24[0])[0][4];
        ((MatrixWordCopy *) sp28)[0][5] = ((MatrixWordCopy *) sp24[0])[0][5];
        ((MatrixWordCopy *) sp28)[0][6] = ((MatrixWordCopy *) sp24[0])[0][6];
        ((MatrixWordCopy *) sp28)[0][7] = ((MatrixWordCopy *) sp24[0])[0][7];
        func_80097CF0(sp28, sp48, sp24[0]);
    }

    func_800483FC(&D_80124898, func_8003B39C, arg0);
}
#endif

void func_8003B6F0(MainMenuSceneEffect3 *arg0) {
    if (D_8010B1A2 == 0x15) {
        func_80071824(arg0, func_8003B430);
    }
    func_800483FC(&D_80124898, func_8003B39C, arg0);
}

void func_8003B740(MainMenuSceneEffect3 *arg0) {
    arg0->x = arg0->x + 0xFFFB8000;
    arg0->timer++;
    if (arg0->timer == 0xA2) {
        arg0->timer = 0;
        func_80071824(arg0, func_8003B6F0);
    }
    func_800483FC(&D_80124898, (void *)func_8003B39C, arg0);
}
