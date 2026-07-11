#include "common.h"
#include "callback_task_scheduler.h"
#include "main_menu_effects.h"
#include "main_menu_scene_actor_2.h"
#include "main_menu_scene_actor_3.h"
#include "main_menu_scene_model.h"
#include "main_menu_scene_renderer.h"

struct MainMenuSceneActor2 {
    /* 0x00 */ char pad0[0x18];
    /* 0x18 */ s32 posX;
    /* 0x1C */ s32 posY;
    /* 0x20 */ s32 posZ;
    /* 0x24 */ s16 rotX;
    /* 0x26 */ s16 rotY;
    /* 0x28 */ s16 rotZ;
    /* 0x2A */ u16 timer;
};

extern u8 D_8010B1A4;
extern u16 gEndingSequencePhase;
extern u8 D_8010B1AA;

void func_800373E0(MainMenuSceneActor2 *arg0) {
    func_80041FB4(2);
    func_800428C8(2);
}

void func_8003740C(MainMenuSceneActor2 *arg0) {
    func_80041FB4(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x41) {
        setCallbackTaskCallback(arg0, func_800373E0);
        func_80041DD4(2, 0x56);
        arg0->rotY = 0xC00;
        func_800420FC(2, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
}

void func_80037484(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x40) {
        setCallbackTaskCallback(arg0, func_8003740C);
        func_80041DD4(2, 0x68);
    }
}

void func_800374DC(MainMenuSceneActor2 *arg0) {
    u16 *p;
    func_80042034(2);
    func_800428C8(2);
    p = &arg0->timer;
    (*p)++;
    if (*p == 0x1E) {
        *p = 0;
        setCallbackTaskCallback(arg0, func_80037484);
        gEndingSequencePhase = 0x3F;
    }
}

void func_80037548(MainMenuSceneActor2 *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = func_80041FB4(2);
    func_800428C8(2);
    if (sp18 == 1) {
        setCallbackTaskCallback(arg0, func_800374DC);
        func_80041DD4(2, 0x67);
    }
}

void func_800375A0(MainMenuSceneActor2 *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = func_80041FB4(2);
    func_800428C8(2);
    if (sp18 == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0x1E) {
            *p = 0;
            setCallbackTaskCallback(arg0, func_80037548);
            func_80041DD4(2, 0x66);
        }
    }
}

void func_80037620(MainMenuSceneActor2 *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = func_80041FB4(2);
    func_800428C8(2);
    if (sp18 == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0x14) {
            *p = 0;
            setCallbackTaskCallback(arg0, func_800375A0);
            func_80041DD4(2, 0x65);
            func_8003D068(-0x1F, -0x6B);
        }
    }
}

void func_800376AC(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x3E) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, func_80037620);
        func_80041DD4(2, 0x64);
    }
}

void func_80037708(MainMenuSceneActor2 *arg0) {
    u16 *p;
    func_80042034(2);
    func_800428C8(2);
    p = &arg0->timer;
    (*p)++;
    if (*p == 0x32) {
        setCallbackTaskCallback(arg0, func_800376AC);
        gEndingSequencePhase = 0x3D;
    }
}

void func_80037770(MainMenuSceneActor2 *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = func_80041FB4(2);
    func_800428C8(2);
    if (sp18 == 1) {
        setCallbackTaskCallback(arg0, func_80037708);
        func_80041DD4(2, 0x63);
    }
}

void func_800377C8(MainMenuSceneActor2 *arg0) {
    if (func_80041FB4(2) == 1) {
        setCallbackTaskCallback(arg0, func_80037770);
        func_80041DD4(2, 0x62);
    }
    func_800428C8(2);
}

void func_80037818(MainMenuSceneActor2 *arg0) {
    if (func_80041FB4(2) == 1) {
        setCallbackTaskCallback(arg0, func_800377C8);
        func_80041DD4(2, 0x23);
    }
    func_800428C8(2);
}

void func_80037868(MainMenuSceneActor2 *arg0) {
    if (func_80041FB4(2) == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0xF) {
            *p = 0;
            setCallbackTaskCallback(arg0, func_80037818);
            func_80041DD4(2, 0x22);
        }
    }
    func_800428C8(2);
}

void func_800378E0(MainMenuSceneActor2 *arg0) {
    s32 unused;
    s32 sp20;
    s32 var_v0;

    sp20 = func_80041FB4(2);
    if (arg0->timer < 5) {
        var_v0 = 1;
    } else {
        var_v0 = -1;
    }
    arg0->posX += 0x5D000;
    arg0->posY += var_v0 * 0x60000;
    arg0->timer = arg0->timer + 1;
    func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
    if (sp20 == 1) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, func_80037868);
        func_80041DD4(2, 0x21);
    }
}

void func_80037998(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    arg0->posX += 0xFFF70000;
    if (arg0->posX < (s32)0xFFA00001) {
        arg0->posX = 0xFFA00000;
        setCallbackTaskCallback(arg0, func_800378E0);
        arg0->rotY = 0xC00;
        func_800420FC(2, arg0->rotX, arg0->rotY, arg0->rotZ);
        func_80041DD4(2, 0x20);
        gEndingSequencePhase = 0x3B;
        D_8010B1AA = 1;
    }
    func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void func_80037A58(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x3A) {
        setCallbackTaskCallback(arg0, func_80037998);
    }
}

void func_80037AA4(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    arg0->posX += 0x24000;
    if (arg0->posX >= 0x100000) {
        arg0->posX = 0x100000;
        setCallbackTaskCallback(arg0, func_80037A58);
    }
    func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void func_80037B20(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x38) {
        setCallbackTaskCallback(arg0, func_80037AA4);
    }
}

void func_80037B6C(MainMenuSceneActor2 *arg0) {
    s32 var_a1;
    MainMenuSceneActor2 *new_var;

    func_80042034(2);
    new_var = arg0;
    var_a1 = (new_var->posX += -0x48000);
    if (var_a1 < -0x7FFFFF) {
        new_var->posX = -0x800000;
        setCallbackTaskCallback(new_var, func_80037B20);
        var_a1 = arg0->posX;
    } else if ((var_a1 < 0x1300001) && (gEndingSequencePhase == 0x33)) {
        gEndingSequencePhase = 0x34;
        var_a1 = arg0->posX;
    }
    func_8004209C(2, var_a1, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void func_80037C28(MainMenuSceneActor2 *arg0) {
    if (gEndingSequencePhase == 0x33) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0x14) {
            *p = 0;
            setCallbackTaskCallback(arg0, func_80037B6C);
            arg0->posX = 0x1900000;
            arg0->posZ = 0x120000;
            D_8010B1AA = 0;
            func_8003CD9C(0x18, -0x36, 2, 1);
        }
    }
}

void func_80037CAC(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    arg0->posX += 0xFFFB8000;
    if (arg0->posX < (s32)0xFE700001) {
        arg0->posX = 0xFE700000;
        setCallbackTaskCallback(arg0, func_80037C28);
        gEndingSequencePhase = 0x31;
        D_8010B1AA = 1;
    }
    func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void func_80037D48(MainMenuSceneActor2 *arg0) {
    s32 unused;
    s32 sp20;

    sp20 = func_80041FB4(2);
    arg0->posX += 0xFFF00000;
    arg0->timer += 1;
    if (arg0->timer < 3) {
        arg0->posY += 0x140000;
    } else {
        arg0->posY += 0xFFEC0000;
    }
    func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
    if (sp20 == 1) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, func_80037CAC);
        arg0->posY = 0;
        func_8004209C(2, arg0->posX, 0, arg0->posZ);
        func_80041DD4(2, 0x1B);
        D_8010B1AA = 0;
        func_8003CD9C(0x18, -0x36, 2, 1);
    }
}

void func_80037E3C(MainMenuSceneActor2 *arg0) {
    func_800428C8(2);
    if (gEndingSequencePhase == 0x2F) {
        setCallbackTaskCallback(arg0, func_80037D48);
        func_80041DD4(2, 0x4E);
        func_8003C0A4(8, -0x40, 2, 0);
    }
}

void func_80037EA0(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    arg0->posX = arg0->posX + 0x2000;
    func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x2D) {
        setCallbackTaskCallback(arg0, func_80037E3C);
    }
}

void func_80037F0C(MainMenuSceneActor2 *arg0) {
    u16 *p;
    func_80042034(2);
    func_800428C8(2);
    p = &arg0->timer;
    (*p)++;
    if (*p == 0x19) {
        gEndingSequencePhase = 0x2C;
        setCallbackTaskCallback(arg0, func_80037EA0);
        *p = 0;
    }
}

void func_80037F74(MainMenuSceneActor2 *arg0) {
    if (func_80041FB4(2) == 0) {
        arg0->posX = arg0->posX + 0xFFF60000;
    } else {
        setCallbackTaskCallback(arg0, func_80037F0C);
        func_80041DD4(2, 0x44);
        arg0->posZ = 0xFFFF0000;
        arg0->timer = 0;
    }
    func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void func_80037FF8(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    arg0->posX += 0xFFFC8000;
    arg0->posZ += 0xFFFF0000;
    if (arg0->posX < (s32)0xFFA80001) {
        arg0->posX = 0xFF780000;
        setCallbackTaskCallback(arg0, func_80037F74);
        func_80041DD4(2, 0x43);
        arg0->timer = 0;
    }
    func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void func_80038098(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x2A) {
        setCallbackTaskCallback(arg0, func_80037FF8);
        arg0->posZ = 0xFFFC0000;
    }
}

void func_800380F0(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    arg0->posX += 0x2D000;
    if (arg0->posX >= 0xF00000) {
        arg0->posX = 0xF00000;
        setCallbackTaskCallback(arg0, func_80038098);
        gEndingSequencePhase = 0x29;
        D_8010B1AA = 1;
        func_8003C420(-0x10, -0x4E, 2);
    }
    func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void func_80038194(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x28) {
        setCallbackTaskCallback(arg0, func_800380F0);
    }
}

void func_800381E0(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    arg0->posX += 0x2E000;
    if (arg0->posX >= (s32)0xFFD00000) {
        arg0->posX = 0xFFD00000;
        setCallbackTaskCallback(arg0, func_80038194);
    }
    func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void func_8003825C(MainMenuSceneActor2 *arg0) {
    u16 *p;
    func_80042034(2);
    func_800428C8(2);
    p = &arg0->timer;
    (*p)++;
    if (*p == 0xF) {
        *p = 0;
        setCallbackTaskCallback(arg0, func_800381E0);
    }
}

void func_800382BC(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    arg0->posX += 0xFFFD2000;
    if (arg0->posX < (s32)0xFF800001) {
        arg0->posX = 0xFF800000;
        setCallbackTaskCallback(arg0, func_8003825C);
    }
    func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void func_80038340(MainMenuSceneActor2 *arg0) {
    u16 *p;
    func_80042034(2);
    func_800428C8(2);
    p = &arg0->timer;
    (*p)++;
    if (*p == 0x19) {
        *p = 0;
        setCallbackTaskCallback(arg0, func_800382BC);
        gEndingSequencePhase = 0x27;
    }
}

void func_800383AC(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    arg0->posX += 0x20000;
    if (arg0->posX >= (s32)0xFFD80000) {
        arg0->posX = 0xFFD80000;
        setCallbackTaskCallback(arg0, func_80038340);
    }
    func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void func_80038424(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    func_800428C8(2);
    gEndingSequencePhase = 0x26;
    if (gEndingSequencePhase != 0) {
        setCallbackTaskCallback(arg0, func_800383AC);
    }
}

void func_80038478(MainMenuSceneActor2 *arg0) {
    s32 *temp_a2;
    s32 var_a1;

    func_80042034(2);
    var_a1 = (arg0->posX += 0xFFFD4000);
    if (var_a1 < (s32)0xFF550001) {
        arg0->posX = 0xFF550000;
        setCallbackTaskCallback(arg0, func_80038424);
        gEndingSequencePhase = 0x25;
        var_a1 = arg0->posX;
    }
    temp_a2 = &arg0->posY;
    func_8004209C(2, var_a1, *temp_a2, arg0->posZ);
    func_800428C8(2);
}

void func_80038508(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x24) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0x12) {
            *p = 0;
            setCallbackTaskCallback(arg0, func_80038478);
        }
    }
}

void func_8003857C(MainMenuSceneActor2 *arg0) {
    s32 var_a1;

    func_80042034(2);
    var_a1 = (arg0->posX -= -0x28000);
    if (var_a1 >= (s32)0xFFD00000) {
        arg0->posX = 0xFFD00000;
        setCallbackTaskCallback(arg0, func_80038508);
        var_a1 = arg0->posX;
    }
    if ((var_a1 >= (s32)0xFFA00000) && (gEndingSequencePhase == 0x21)) {
        gEndingSequencePhase = 0x22;
        var_a1 = arg0->posX;
    }
    func_8004209C(2, var_a1, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void func_80038628(MainMenuSceneActor2 *arg0) {
    if (gEndingSequencePhase == 0x21) {
        setCallbackTaskCallback(arg0, func_8003857C);
        D_8010B1AA = 0;
        func_8003CD9C(0x18, -0x36, 2, 1);
    }
}

void func_80038678(MainMenuSceneActor2 *arg0) {
    s32 *temp_a2;
    s32 var_a1;

    func_80042034(2);
    var_a1 = (arg0->posX += 0xFFFA0000);
    if (var_a1 < (s32)0xFE700001) {
        arg0->posX = 0xFE700000;
        setCallbackTaskCallback(arg0, func_80038628);
        D_8010B1AA = 1;
        var_a1 = arg0->posX;
    }
    temp_a2 = &arg0->posY;
    func_8004209C(2, var_a1, *temp_a2, arg0->posZ);
    func_800428C8(2);
}

void func_80038704(MainMenuSceneActor2 *arg0) {
    s32 var_a1;

    func_80042034(2);
    var_a1 = (arg0->posX += 0xFFFB0000);
    if (var_a1 < (s32)0xFF600001) {
        arg0->posX = 0xFF600000;
        setCallbackTaskCallback(arg0, func_80038678);
        func_80041DD4(2, 0x1B);
        arg0->rotY = 0xC00;
        func_800420FC(2, arg0->rotX, arg0->rotY, arg0->rotZ);
        gEndingSequencePhase = 0x20;
        D_8010B1AA = 0;
        func_8003CD9C(0x18, -0x36, 2, 1);
        var_a1 = arg0->posX;
    }
    func_8004209C(2, var_a1, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void func_800387D4(MainMenuSceneActor2 *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = func_80041FB4(2);
    func_800428C8(2);
    if (sp18 == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0x14) {
            *p = 0;
            setCallbackTaskCallback(arg0, func_80038704);
            func_80041DD4(2, 0x48);
        }
    }
}

void func_80038854(MainMenuSceneActor2 *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = func_80041FB4(2);
    func_800428C8(2);
    if (sp18 == 1) {
        setCallbackTaskCallback(arg0, func_800387D4);
        func_80041DD4(2, 0x47);
        func_8003C0A4(5, -0x46, 2, 0);
    }
}

void func_800388C0(MainMenuSceneActor2 *arg0) {
    if (func_80041FB4(2) == 0) {
        arg0->posX += 0xFFFE0000;
        func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    } else {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0xF) {
            *p = 0;
            setCallbackTaskCallback(arg0, func_80038854);
            func_80041DD4(2, 0x46);
        }
    }
    func_800428C8(2);
}

void func_80038954(MainMenuSceneActor2 *arg0) {
    func_800428C8(2);
    if (gEndingSequencePhase == 0x1E) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, func_800388C0);
        func_80041DD4(2, 0x45);
        func_8003CB78(0x2A, -0x57);
    }
}

void func_800389B4(MainMenuSceneActor2 *arg0) {
    s32 sp24;
    s32 sp20;

    sp20 = func_80041FB4(2);
    func_800428C8(2);
    if (sp20 == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 5) {
            *p = 0;
            setCallbackTaskCallback(arg0, func_80038954);
            gEndingSequencePhase = 0x1D;
        }
    } else {
        arg0->posX += 0xFFFB8000;
        func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    }
}

void func_80038A54(MainMenuSceneActor2 *arg0) {
    s32 sp1C;
    s32 sp18;

    sp18 = func_80041FB4(2);
    func_800428C8(2);
    if (sp18 == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0x14) {
            *p = 0;
            setCallbackTaskCallback(arg0, func_800389B4);
            func_80041DD4(2, 0x35);
            func_8003C0A4(8, -0x40, 2, 0);
        }
    }
}

void func_80038AE8(MainMenuSceneActor2 *arg0) {
    func_80041FB4(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x1C) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, func_80038A54);
        func_80041DD4(2, 0x34);
    }
}

void func_80038B44(MainMenuSceneActor2 *arg0) {
    u16 *p;
    func_800428C8(2);
    p = &arg0->timer;
    (*p)++;
    if (*p == 0x1E) {
        *p = 0;
        setCallbackTaskCallback(arg0, func_80038AE8);
        func_80041DD4(2, 0x33);
        arg0->posZ = 0xFFFF0000;
        func_8004209C(2, arg0->posX, arg0->posY, 0xFFFF0000);
    }
}

void func_80038BBC(MainMenuSceneActor2 *arg0) {
    if (func_80041FB4(2) == 1) {
        setCallbackTaskCallback(arg0, func_80038B44);
    } else {
        arg0->posZ = arg0->posZ + 0xFFFC0000;
        func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    }
    func_800428C8(2);
}

void func_80038C2C(MainMenuSceneActor2 *arg0) {
    func_800428C8(2);
    if (D_8010B1A4 == 5) {
        setCallbackTaskCallback(arg0, func_80038BBC);
        func_80041DD4(2, 0x32);
        arg0->rotY = 0x400;
        func_800420FC(2, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
}

void func_80038C9C(MainMenuSceneActor2 *arg0) {
    if (D_8010B1A4 == 2) {
        func_80041FB4(2);
        if (func_80041FB4(2) == 1) {
            u16 *p;
            D_8010B1A4++;
            func_80041DD4(2, 0x2B);
            p = &arg0->timer;
            (*p)++;
            if (*p == 0x4) {
                setCallbackTaskCallback(arg0, func_80038C2C);
                *p = 0;
            }
        }
    }
    func_800428C8(2);
}

void func_80038D38(MainMenuSceneActor2 *arg0) {
    if (D_8010B1A4 == 2) {
        func_80041FB4(2);
        if (func_80041FB4(2) == 1) {
            u16 *p;
            D_8010B1A4++;
            func_80041DD4(2, 0x2B);
            p = &arg0->timer;
            (*p)++;
            if (*p == 0x5) {
                setCallbackTaskCallback(arg0, func_80038C9C);
                *p = 0;
            }
        }
    }
    func_800428C8(2);
}

void func_80038DD4(MainMenuSceneActor2 *arg0) {
    if (D_8010B1A4 == 2) {
        if (arg0->rotY == 0xC01) {
            arg0->rotY = 0xC00;
            func_800420FC(2, arg0->rotX, arg0->rotY, arg0->rotZ);
        }
        if (func_80041FB4(2) == 1) {
            u16 *p;
            D_8010B1A4++;
            func_80041DD4(2, 0x2B);
            p = &arg0->timer;
            (*p)++;
            if (*p == 0x3) {
                setCallbackTaskCallback(arg0, func_80038D38);
                *p = 0;
            }
        }
    }
    func_800428C8(2);
}

void func_80038E90(MainMenuSceneActor2 *arg0) {
    if (D_8010B1A4 == 2) {
        if (func_80041FB4(2) == 1) {
            D_8010B1A4++;
            setCallbackTaskCallback(arg0, func_80038DD4);
            func_80041DD4(2, 0x2B);
            arg0->rotY = 0xC01;
            arg0->timer = 0;
        }
    }
    func_800428C8(2);
}

void func_80038F18(MainMenuSceneActor2 *arg0) {
    func_80041FB4(2);
    func_800428C8(2);
    arg0->posX = arg0->posX + 0x30000;
    if (gEndingSequencePhase == 0x18) {
        setCallbackTaskCallback(arg0, func_80038E90);
        func_80041DD4(2, 0x31);
        arg0->rotY = 0x400;
        func_800420FC(2, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
    func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
}

void func_80038FB4(MainMenuSceneActor2 *arg0) {
    u16 val;
    func_80042034(2);
    func_800428C8(2);
    val = arg0->timer;
    if (val < 0x2D) {
        arg0->timer = val + 1;
        if (arg0->timer == 0x2D) {
            gEndingSequencePhase = 0x16;
        }
    } else if (gEndingSequencePhase == 0x17) {
        setCallbackTaskCallback(arg0, func_80038F18);
        func_80041DD4(2, 0x30);
        arg0->rotY = 0x400;
        func_800420FC(2, arg0->rotX, arg0->rotY, arg0->rotZ);
    }
}

void func_8003905C(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    arg0->posX = arg0->posX + 0xFFFD0000;
    if (arg0->posX < 0x4E0001) {
        arg0->posX = 0x4E0000;
        setCallbackTaskCallback(arg0, func_80038FB4);
        func_80041DD4(2, 0x26);
    }
    func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
}

void func_800390E8(MainMenuSceneActor2 *arg0) {
    if (func_80041FB4(2) == 1) {
        setCallbackTaskCallback(arg0, func_8003905C);
        func_80041DD4(2, 0);
    }
    func_800428C8(2);
}

void func_80039138(MainMenuSceneActor2 *arg0) {
    if (func_80041FB4(2) == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0xF) {
            *p = 0;
            setCallbackTaskCallback(arg0, func_800390E8);
            func_80041DD4(2, 0x23);
        }
    }
    func_800428C8(2);
}

void func_800391B0(MainMenuSceneActor2 *arg0) {
    if (func_80041FB4(2) == 1) {
        u16 *p = &arg0->timer;
        (*p)++;
        if (*p == 0xF) {
            *p = 0;
            setCallbackTaskCallback(arg0, func_80039138);
            func_80041DD4(2, 0x22);
        }
    }
    func_800428C8(2);
}

void func_80039228(MainMenuSceneActor2 *arg0) {
    s32 unused;
    s32 sp20;
    s32 var_v0;

    sp20 = func_80041FB4(2);
    if (arg0->timer < 5) {
        var_v0 = 1;
    } else {
        var_v0 = -1;
    }
    arg0->posX += 0x50000;
    arg0->posY += var_v0 * 0x60000;
    arg0->timer = arg0->timer + 1;
    func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800428C8(2);
    if (sp20 == 1) {
        arg0->timer = 0;
        setCallbackTaskCallback(arg0, func_800391B0);
        func_80041DD4(2, 0x21);
    }
}

void func_800392DC(MainMenuSceneActor2 *arg0) {
    func_80042034(2);
    func_800428C8(2);
    if (gEndingSequencePhase == 0x15) {
        setCallbackTaskCallback(arg0, func_80039228);
        arg0->rotY = 0xC00;
        func_800420FC(2, arg0->rotX, arg0->rotY, arg0->rotZ);
        func_80041DD4(2, 0x20);
    }
}

void func_80039354(MainMenuSceneActor2 *arg0) {
    arg0->posX = arg0->posX + 0xFFFB8000;
    if (arg0->posX < 0x500001) {
        arg0->posX = 0x500000;
        setCallbackTaskCallback(arg0, func_800392DC);
    }
    func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    func_80042034(2);
    func_800428C8(2);
}

void func_800393D4(MainMenuSceneActor2 *arg0) {
    if (gEndingSequencePhase == 0x13) {
        setCallbackTaskCallback(arg0, func_80039354);
        func_8003B308(arg0->posX + 0x48000, 0x480000, 0xFFF30000, 2, 2, 1);
    }
}

void func_80039440(MainMenuSceneActor2 *arg0) {
    arg0->posX = 0x3248000;
    arg0->posY = 0;
    arg0->posZ = 0;
    arg0->rotX = 0;
    arg0->rotY = 0x400;
    arg0->rotZ = 0;
    func_80041D20(2, 2);
    func_80041DD4(2, 0x25);
    func_8004209C(2, arg0->posX, arg0->posY, arg0->posZ);
    func_800420FC(2, arg0->rotX, arg0->rotY, arg0->rotZ);
    setCallbackTaskCallback(arg0, func_800393D4);
}
