#include "common.h"
#include "callback_task_scheduler.h"
#include "main_menu_effects.h"
#include "main_menu_scene_actor_0.h"
#include "main_menu_scene_actor_4.h"
#include "main_menu_scene_model.h"
#include "main_menu_scene_renderer.h"

struct MainMenuSceneActor {
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

extern MainMenuSceneActorShadow D_8010B1C0;

extern u16 gEndingSequencePhase;
extern u8 D_8010B1A4;
extern u8 D_8010B1A5;
extern u8 D_8010B1A8;

void func_80032A80(void) {
}

void func_80032A88(MainMenuSceneActor *arg0) {
    MainMenuSceneActor *temp = arg0;

    func_80041FB4(0);
    func_800428C8(0);
    if (temp->unk2A < 0x96) {
        temp->unk2A += 1;
        if (temp->unk2A == 0x96) {
            gEndingSequencePhase = 0x42;
        }
    }
}

void func_80032AF0(MainMenuSceneActor *arg0) {
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
            setCallbackTaskCallback(arg0, func_80032A88);
            func_80041DD4(0, 0x56);
            gEndingSequencePhase = 0x41;
            arg0->unk26 = 0xC00;
            func_800420FC(0, arg0->unk24, arg0->unk26, arg0->unk28);
        }
    }
}

void func_80032B94(MainMenuSceneActor *arg0) {
    func_80041FB4(0);
    func_800428C8(0);
    if (gEndingSequencePhase == 0x40) {
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, func_80032AF0);
        func_80041DD4(0, 0x55);
    }
}

void func_80032BF0(MainMenuSceneActor *arg0) {
    MainMenuSceneActor *temp = arg0;

    if (func_80041FB4(0) == 1) {
        temp->unk2A += 1;
        if (temp->unk2A == 0x1E) {
            temp->unk2A = 0;
            setCallbackTaskCallback(temp, func_80032B94);
            func_80041DD4(0, 0x54);
            func_8003D068(0xA, -0x64);
        }
    }
    func_800428C8(0);
}

void func_80032C74(MainMenuSceneActor *arg0) {
    struct { s32 ret; s32 pad; } l;
    l.ret = func_80041FB4(0);
    func_800428C8(0);
    if (l.ret == 1) {
        arg0->unk2A += 1;
        if (arg0->unk2A == 0x14) {
            arg0->unk2A = 0;
            setCallbackTaskCallback(arg0, func_80032BF0);
            func_80041DD4(0, 0x53);
        }
    }
}

void func_80032CF4(MainMenuSceneActor *arg0) {
    if (func_80041FB4(0) == 0) {
        arg0->unk18 += 0x18000;
        func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    }
    if (gEndingSequencePhase == 0x3F) {
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, func_80032C74);
        func_80041DD4(0, 0x22);
    }
    func_800428C8(0);
}

void func_80032D7C(MainMenuSceneActor *arg0) {
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
        setCallbackTaskCallback(arg0, func_80032CF4);
        func_80041DD4(0, 0x21);
    }
}

void func_80032E48(MainMenuSceneActor *arg0) {
    func_80042034(0);
    arg0->unk18 += 0xFFF70000;
    if (gEndingSequencePhase == 0x3B) {
        setCallbackTaskCallback(arg0, func_80032D7C);
        func_80041DD4(0, 0x20);
        arg0->unk26 = 0xC00;
        func_800420FC(0, arg0->unk24, arg0->unk26, arg0->unk28);
        D_8010B1A8 = 1;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void func_80032EF0(s32 arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (gEndingSequencePhase == 0x3A) {
        setCallbackTaskCallback(arg0, func_80032E48);
    }
}

void func_80032F3C(MainMenuSceneActor *arg0) {
    func_80042034(0);
    func_800428C8(0);
    arg0->unk2A += 1;
    if (D_8010B1A5 >= 3) {
        if (arg0->unk2A % 15 == 0) {
            D_8010B1A5--;
            if (D_8010B1A5 == 2) {
                arg0->unk2A = 0;
            }
        }
    } else if (arg0->unk2A == 0x10) {
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, func_80032EF0);
        gEndingSequencePhase = 0x39;
    }
}

void func_80032FF0(MainMenuSceneActor *arg0) {
    MainMenuSceneActor *temp = arg0;

    func_80042034(0);
    temp->unk18 += 0x24000;
    if (temp->unk18 >= 0x500000) {
        temp->unk18 = 0x500000;
        setCallbackTaskCallback(temp, func_80032F3C);
    }
    func_8004209C(0, temp->unk18, temp->unk1C, temp->unk20);
    func_800428C8(0);
}

void func_8003306C(MainMenuSceneActor *arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (gEndingSequencePhase == 0x37) {
        arg0->unk2A += 1;
        if (arg0->unk2A == 0x1E) {
            arg0->unk2A = 0;
            setCallbackTaskCallback(arg0, func_80032FF0);
            gEndingSequencePhase = 0x38;
        }
    }
}

void func_800330EC(MainMenuSceneActor *arg0) {
    s32 var_a1;
    MainMenuSceneActor *new_var;

    func_80042034(0);
    new_var = arg0;
    var_a1 = (new_var->unk18 += -0x48000);
    if (var_a1 < -0x1FFFFF) {
        new_var->unk18 = -0x200000;
        setCallbackTaskCallback(new_var, func_8003306C);
        var_a1 = arg0->unk18;
    } else if ((var_a1 < 0xD00001) && (gEndingSequencePhase == 0x34)) {
        gEndingSequencePhase = 0x35;
        var_a1 = arg0->unk18;
    }
    func_8004209C(0, var_a1, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void func_800331A8(MainMenuSceneActor *arg0) {
    if (gEndingSequencePhase == 0x34) {
        setCallbackTaskCallback(arg0, func_800330EC);
        arg0->unk18 = 0x1900000;
        arg0->unk20 = 0;
        D_8010B1A8 = 0;
        func_8003CD9C(0x1C, -0x30, 0, 1);
    }
}

void func_8003320C(MainMenuSceneActor *arg0) {
    func_80042034(0);
    arg0->unk18 += 0xFFFB8000;
    if (arg0->unk18 < (s32)0xFE700001) {
        arg0->unk18 = 0xFE700000;
        setCallbackTaskCallback(arg0, func_800331A8);
        D_8010B1A8 = 1;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void func_8003329C(MainMenuSceneActor *arg0) {
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
        setCallbackTaskCallback(arg0, func_8003320C);
        arg0->unk1C = 0;
        func_8004209C(0, arg0->unk18, 0, arg0->unk20);
        func_80041DD4(0, 0x1B);
        D_8010B1A8 = 0;
        func_8003CD9C(0x1C, -0x30, 0, 1);
    }
}

void func_800333A8(void *arg0) {
    func_800428C8(0);
    if (gEndingSequencePhase == 0x2F) {
        setCallbackTaskCallback(arg0, func_8003329C);
        func_80041DD4(0, 0x4E);
        func_8003C0A4(8, -0x40, 0, 0);
    }
}

void func_8003340C(MainMenuSceneActor *arg0) {
    func_80042034(0);
    arg0->unk18 += 0x2000;
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
    if (gEndingSequencePhase == 0x2D) {
        setCallbackTaskCallback(arg0, func_800333A8);
    }
}

void func_80033478(MainMenuSceneActor *arg0) {
    if (func_80041FB4(0) == 0) {
        arg0->unk18 += 0xFFF60000;
    } else {
        setCallbackTaskCallback(arg0, func_8003340C);
        func_80041DD4(0, 0x44);
        arg0->unk20 = 0xFFFF0000;
        gEndingSequencePhase = 0x2B;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void func_80033504(MainMenuSceneActor *arg0) {
    func_80042034(0);
    arg0->unk18 += 0xFFFC8000;
    if (arg0->unk18 < (s32)0xFFD00001) {
        arg0->unk18 = 0xFFA00000;
        setCallbackTaskCallback(arg0, func_80033478);
        func_80041DD4(0, 0x43);
        arg0->unk2A = 0;
        D_8010B1A8 = 1;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void func_800335A4(s32 arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (gEndingSequencePhase == 0x2A) {
        setCallbackTaskCallback(arg0, func_80033504);
    }
}

void func_800335F0(MainMenuSceneActor *arg0) {
    s32 var_a1;

    func_80042034(0);
    var_a1 = (arg0->unk18 += 0x2D000);
    if (var_a1 >= 0x800000) {
        arg0->unk18 = 0x800000;
        setCallbackTaskCallback(arg0, func_800335A4);
        D_8010B1A8 = 1;
        func_8003C420(-0x10, -0x4A, 0);
        var_a1 = arg0->unk18;
    }
    func_8004209C(0, var_a1, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void func_80033688(s32 arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (gEndingSequencePhase == 0x28) {
        setCallbackTaskCallback(arg0, func_800335F0);
    }
}

void func_800336D4(MainMenuSceneActor *arg0) {
    func_80042034(0);
    arg0->unk18 += 0xFFFDA000;
    if (arg0->unk18 < (s32)0xFF700001) {
        arg0->unk18 = 0xFF700000;
        setCallbackTaskCallback(arg0, func_80033688);
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void func_80033758(MainMenuSceneActor *arg0) {
    func_80042034(0);
    arg0->unk18 += 0x1D000;
    if (arg0->unk18 >= (s32)0xFFCE0000) {
        arg0->unk18 = 0xFFCE0000;
        setCallbackTaskCallback(arg0, func_800336D4);
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void func_800337D4(void *arg0) {
    func_80042034(0);
    func_800428C8(0);
    if ((gEndingSequencePhase = 0x27) != 0) {
        setCallbackTaskCallback(arg0, func_80033758);
    }
}

void func_80033828(MainMenuSceneActor *arg0) {
    func_80042034(0);
    arg0->unk18 += 0xFFFD8000;
    if (arg0->unk18 < (s32)0xFF700001) {
        arg0->unk18 = 0xFF700000;
        setCallbackTaskCallback(arg0, func_800337D4);
        gEndingSequencePhase = 0x26;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void func_800338B8(void) {
}

void func_800338C0(MainMenuSceneActor *arg0) {
    func_80042034(0);
    arg0->unk18 += 0x28000;
    arg0->unk20 += 0xFFFE0000;
    if (arg0->unk18 >= (s32)0xFFC80000) {
        arg0->unk18 = 0xFFC80000;
        setCallbackTaskCallback(arg0, func_80033828);
        gEndingSequencePhase = 0x24;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void func_80033958(MainMenuSceneActor *arg0) {
    func_80042034(0);
    func_800428C8(0);
    arg0->unk2A += 1;
    if (arg0->unk2A == 0x32) {
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, func_800338C0);
    }
}

void func_800339B8(MainMenuSceneActor *arg0) {
    func_80042034(0);
    arg0->unk18 += 0x28000;
    if (arg0->unk18 >= (s32)0xFF700000) {
        arg0->unk18 = 0xFF700000;
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, func_80033958);
        gEndingSequencePhase = 0x23;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void func_80033A44(void *arg0) {
    if (gEndingSequencePhase == 0x22) {
        setCallbackTaskCallback(arg0, func_800339B8);
        D_8010B1A8 = 0;
        func_8003CD9C(0x1C, -0x30, 0, 1);
    }
}

void func_80033A94(MainMenuSceneActor *arg0) {
    func_80042034(0);
    arg0->unk18 += 0xFFFA0000;
    if (arg0->unk18 < (s32)0xFE700001) {
        arg0->unk18 = 0xFE700000;
        setCallbackTaskCallback(arg0, func_80033A44);
        D_8010B1A8 = 1;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void func_80033B20(MainMenuSceneActor *arg0) {
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
            setCallbackTaskCallback(arg0, func_80033A94);
            func_80041DD4(0, 0x1B);
            arg0->unk26 = 0xC00;
            func_800420FC(0, arg0->unk24, arg0->unk26, arg0->unk28);
            gEndingSequencePhase = 0x1F;
            D_8010B1A8 = 0;
            func_8003CD9C(0x1C, -0x30, 0, 1);
        }
    }
}

void func_80033BE0(void *arg0) {
    func_800428C8(0);
    if (gEndingSequencePhase == 0x1E) {
        setCallbackTaskCallback(arg0, func_80033B20);
        func_80041DD4(0, 0x2D);
        func_8003C0A4(8, -0x40, 0, 0);
    }
}

void func_80033C44(MainMenuSceneActor *arg0) {
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
                setCallbackTaskCallback(arg0, func_80033BE0);
            }
        }
    } else {
        arg0->unk18 += 0xFFFF0000;
        func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    }
}

void func_80033D0C(s32 arg0) {
    struct {
        s32 ret;
        s32 pad;
    } locals;

    locals.ret = func_80041FB4(0);
    func_800428C8(0);
    if (locals.ret == 1) {
        setCallbackTaskCallback(arg0, func_80033C44);
        func_80041DD4(0, 0x2F);
    }
}

void func_80033D64(MainMenuSceneActor *arg0) {
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
            setCallbackTaskCallback(arg0, func_80033D0C);
            func_80041DD4(0, 0x2E);
        }
    }
}

void func_80033DE4(MainMenuSceneActor *arg0) {
    func_80041FB4(0);
    func_800428C8(0);
    if (gEndingSequencePhase == 0x1B) {
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, func_80033D64);
        func_80041DD4(0, 0x2D);
        func_8003C0A4(8, -0x40, 0, 0);
    }
}

void func_80033E54(void *arg0) {
    func_800428C8(0);
    if (gEndingSequencePhase == 0x1A) {
        setCallbackTaskCallback(arg0, func_80033DE4);
        func_80041DD4(0, 0x2C);
    }
}

void func_80033EA4(MainMenuSceneActor *arg0) {
    if (D_8010B1A4 == 4) {
        D_8010B1A4 = 0;
        func_80041DD4(0, 0x2B);
    }
    if (D_8010B1A4 == 0) {
        func_80041FB4(0);
        if (func_80041FB4(0) == 1) {
            D_8010B1A4 += 1;
            if (++arg0->unk2A == 5) {
                setCallbackTaskCallback(arg0, func_80033E54);
                arg0->unk2A = 0;
            }
        }
    }
    func_800428C8(0);
}

void func_80033F5C(MainMenuSceneActor *arg0) {
    if (D_8010B1A4 == 4) {
        D_8010B1A4 = 0;
        func_80041DD4(0, 0x2B);
    }
    if (D_8010B1A4 == 0) {
        func_80041FB4(0);
        if (func_80041FB4(0) == 1) {
            D_8010B1A4 += 1;
            if (++arg0->unk2A == 5) {
                setCallbackTaskCallback(arg0, func_80033EA4);
                arg0->unk2A = 0;
            }
        }
    }
    func_800428C8(0);
}

void func_80034014(MainMenuSceneActor *arg0) {
    if (D_8010B1A4 == 4) {
        D_8010B1A4 = 0;
        func_80041DD4(0, 0x2B);
    }
    if (D_8010B1A4 == 0) {
        if (func_80041FB4(0) == 1) {
            D_8010B1A4 += 1;
            if (++arg0->unk2A == 3) {
                setCallbackTaskCallback(arg0, func_80033F5C);
                arg0->unk2A = 0;
            }
        }
    }
    func_800428C8(0);
}

void func_800340D8(MainMenuSceneActor *arg0) {
    struct {
        s32 ret;
        s32 pad;
    } locals;

    locals.ret = func_80041FB4(0);
    func_800428C8(0);
    if (locals.ret == 1) {
        D_8010B1A4++;
        setCallbackTaskCallback(arg0, func_80034014);
    }
}

void func_80034138(void *arg0) {
    func_800428C8(0);
    if (gEndingSequencePhase == 0x19) {
        setCallbackTaskCallback(arg0, func_800340D8);
        func_80041DD4(0, 0x2A);
    }
}

void func_80034188(MainMenuSceneActor *arg0) {
    func_80041FB4(0);
    func_800428C8(0);
    arg0->unk18 += 0xFFFD0000;
    if (gEndingSequencePhase == 0x18) {
        setCallbackTaskCallback(arg0, func_80034138);
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
}

void func_800341FC(s32 arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (gEndingSequencePhase == 0x17) {
        setCallbackTaskCallback(arg0, func_80034188);
        func_80041DD4(0, 0x29);
    }
}

void func_80034254(MainMenuSceneActor *arg0) {
    func_80042034(0);
    arg0->unk18 += 0x30000;
    if (arg0->unk18 >= 0xD0000) {
        arg0->unk18 = 0xD0000;
        setCallbackTaskCallback(arg0, func_800341FC);
        func_80041DD4(0, 0x24);
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void func_800342D8(s32 arg0) {
    if (func_80041FB4(0) == 1) {
        setCallbackTaskCallback(arg0, func_80034254);
        func_80041DD4(0, 0);
    }
    func_800428C8(0);
}

void func_80034328(MainMenuSceneActor *arg0) {
    MainMenuSceneActor *temp = arg0;

    if (func_80041FB4(0) == 1) {
        temp->unk2A += 1;
        if (temp->unk2A == 0xF) {
            temp->unk2A = 0;
            setCallbackTaskCallback(temp, func_800342D8);
            func_80041DD4(0, 0x23);
        }
    }
    func_800428C8(0);
}

void func_800343A0(MainMenuSceneActor *arg0) {
    MainMenuSceneActor *temp = arg0;

    if (func_80041FB4(0) == 1) {
        temp->unk2A += 1;
        if (temp->unk2A == 0xF) {
            temp->unk2A = 0;
            setCallbackTaskCallback(temp, func_80034328);
            func_80041DD4(0, 0x22);
        }
    }
    func_800428C8(0);
}

void func_80034418(MainMenuSceneActor *arg0) {
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
        setCallbackTaskCallback(arg0, func_800343A0);
        func_80041DD4(0, 0x21);
    }
}

void func_800344CC(MainMenuSceneActor *arg0) {
    s32 var_a1;

    func_80042034(0);
    var_a1 = (arg0->unk18 += 0x40000);
    if (var_a1 >= 0x280000) {
        arg0->unk18 = 0x280000;
        func_80041DD4(0, 0x20);
        setCallbackTaskCallback(arg0, func_80034418);
        gEndingSequencePhase = 0x15;
        D_8010B1A8 = 1;
        func_8003CB78(0xB, -0x4C);
        var_a1 = arg0->unk18;
    }
    func_8004209C(0, var_a1, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void func_80034574(MainMenuSceneActor *arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (arg0->unk2A < 0x10) {
        arg0->unk2A += 1;
        if (arg0->unk2A == 0x10) {
            gEndingSequencePhase = 0x13;
        }
    } else if (gEndingSequencePhase == 0x14) {
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, func_800344CC);
    }
}

void func_80034600(MainMenuSceneActor *arg0) {
    arg0->unk18 += -0x20000;
    arg0->unk2A += 1;
    if ((arg0->unk2A % 13) == 0) {
        D_8010B1A5 -= 1;
    }
    if (arg0->unk18 < -0x9FFFFF) {
        D_8010B1A5 = 2;
        arg0->unk2A = 0;
        arg0->unk18 = -0xA00000;
        setCallbackTaskCallback(arg0, func_80034574);
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_80042034(0);
    func_800428C8(0);
}

void func_800346D4(MainMenuSceneActor *arg0) {
    func_80042034(0);
    func_800428C8(0);
    if (D_8010B1A5 != 0xA) {
        D_8010B1A5 -= 2;
    } else {
        arg0->unk2A += 1;
        if (arg0->unk2A == 1) {
            arg0->unk2A = 0;
            setCallbackTaskCallback(arg0, func_80034600);
        }
    }
}

void func_80034754(MainMenuSceneActor *arg0) {
    MainMenuSceneActor *temp = arg0;

    func_80042034(0);
    func_800428C8(0);
    if (gEndingSequencePhase == 0x12) {
        temp->unk2A += 1;
        if (temp->unk2A == 0x1E) {
            temp->unk2A = 0;
            setCallbackTaskCallback(temp, func_800346D4);
        }
    }
}

void func_800347C8(MainMenuSceneActor *arg0) {
    func_80042034(0);
    arg0->unk18 += 0x60000;
    arg0->unk2A += 1;
    if (arg0->unk18 >= 0x300000) {
        arg0->unk2A = 0;
        arg0->unk18 = 0x300000;
        setCallbackTaskCallback(arg0, func_80034754);
    }
    if (arg0->unk2A == 0x18) {
        gEndingSequencePhase = 0x11;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void func_80034864(MainMenuSceneActor *arg0) {
    if (gEndingSequencePhase == 0x10) {
        arg0->unk18 = 0xFE700000;
        arg0->unk26 = 0x400;
        func_8004209C(0, -0x1900000, arg0->unk1C, arg0->unk20);
        func_800420FC(0, arg0->unk24, arg0->unk26, arg0->unk28);
        setCallbackTaskCallback(arg0, func_800347C8);
        func_80041DD4(0, 0x1B);
        D_8010B1A8 = 0;
        func_8003CD9C(-0x32, -0x30, 0, 0);
    }
}

void func_80034900(MainMenuSceneActor *arg0) {
    func_80042034(0);
    arg0->unk18 += 0x60000;
    arg0->unk2A += 1;
    if (arg0->unk18 >= 0x1900000) {
        arg0->unk2A = 0;
        arg0->unk18 = 0x1900000;
        setCallbackTaskCallback(arg0, func_80034864);
        D_8010B1A8 = 1;
    }
    if (arg0->unk2A == 1) {
        gEndingSequencePhase = 0xE;
    }
    func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
    func_800428C8(0);
}

void func_800349A8(MainMenuSceneActor *arg0) {
    u16 temp;

    if (func_80041FB4(0) == 1) {
        temp = ++arg0->unk2A;
        if ((u32) temp == 0x19) {
            arg0->unk2A = 0;
            setCallbackTaskCallback(arg0, func_80034900);
            func_80041DD4(0, 0x1B);
            D_8010B1A8 = 0;
            func_8003CD9C(-0x32, -0x30, 0, 0);
        } else if (temp == 2) {
            func_8003C0A4(5, -0x40, 0, 0);
        }
    }
    func_800428C8(0);
}

void func_80034A60(s32 arg0) {
    if (func_80041FB4(0) == 1) {
        setCallbackTaskCallback(arg0, func_800349A8);
        func_80041DD4(0, 0x1A);
    }
    func_800428C8(0);
}

void func_80034AB0(MainMenuSceneActor *arg0) {
    if (func_80041FB4(0) == 1) {
        arg0->unk2A += 1;
        if (arg0->unk2A == 0x32) {
            arg0->unk2A = 0;
            setCallbackTaskCallback(arg0, func_80034A60);
            func_80041DD4(0, 0x19);
        }
    } else {
        arg0->unk2A += 1;
        if (arg0->unk2A == 0x23) {
            arg0->unk2A = 0;
            createCallbackTask(func_8003C870, 0, 0x64);
        }
    }
    func_800428C8(0);
}

void func_80034B60(MainMenuSceneActor *arg0) {
    func_800428C8(0);
    arg0->unk2A += 1;
    if (arg0->unk2A == 0x19) {
        arg0->unk2A = 0;
        setCallbackTaskCallback(arg0, func_80034AB0);
        func_80041DD4(0, 0x18);
    }
}

void func_80034BC4(MainMenuSceneActor *arg0) {
    if (func_80041FB4(0) == 1) {
        arg0->unk2A += 1;
        if (arg0->unk2A < 0xA) {
            func_80041DD4(0, 0x17);
        } else {
            arg0->unk2A = 0;
            setCallbackTaskCallback(arg0, func_80034B60);
        }
    }
    func_800428C8(0);
    func_800373AC(&D_8010B1C0);
}

void func_80034C50(s32 arg0) {
    if (func_80041FB4(0) == 1) {
        setCallbackTaskCallback(arg0, func_80034BC4);
        func_80041DD4(0, 0x17);
    }
    func_800428C8(0);
    func_800373AC(&D_8010B1C0);
}

void func_80034CAC(MainMenuSceneActor *arg0) {
    if (gEndingSequencePhase == 0xD) {
        setCallbackTaskCallback(arg0, func_80034C50);
        func_80041DD4(0, 0x16);
        arg0->unk2C = 0x16;
    }
    func_800428C8(0);
    func_800373AC(&D_8010B1C0);
}

void func_80034D10(MainMenuSceneActor *arg0) {
    if (func_80041FB4(0) == 1) {
        gEndingSequencePhase = 0xB;
        setCallbackTaskCallback(arg0, func_80034CAC);
    } else {
        arg0->unk2A += 1;
        if (arg0->unk2A == 3) {
            func_8003CB78(6, -0x42);
        }
    }
    func_800428C8(0);
    func_800373AC(&D_8010B1C0);
}

void func_80034DA0(MainMenuSceneActor *arg0) {
    MainMenuSceneActor *temp_a2;

    temp_a2 = arg0;
    arg0->unk2A++;
    if (arg0->unk2A == 6) {
        gEndingSequencePhase = 9;
    }
    if (arg0->unk2A == 0x64) {
        gEndingSequencePhase = 0xA;
        setCallbackTaskCallback(temp_a2, func_80034D10);
        func_80041DD4(0, 0xE);
        temp_a2->unk2A = 0;
        D_8010B1C0.actorId = 0;
        D_8010B1C0.unkC = 0xB;
        D_8010B1C0.posY = (s32)0xFFE80000;
        D_8010B1A8 = 1;
    } else {
        func_80042034(0);
    }
    func_800428C8(0);
}

void func_80034E60(MainMenuSceneActor *arg0) {
    s32 temp_a1;

    temp_a1 = (arg0->unk18 += 0xFFFA0000);
    if (temp_a1 < 0x580001) {
        arg0->unk18 = 0x580000;
        setCallbackTaskCallback(arg0, func_80034DA0);
        func_80041DD4(0, 0xD);
        arg0->unk26 = 0x400;
        func_800420FC(0, arg0->unk24, arg0->unk26, arg0->unk28);
        gEndingSequencePhase = 8;
        D_8010B1A8 = 0;
        func_8003CD9C(0x1C, -0x38, 0, 1);
    } else {
        func_8004209C(0, temp_a1, arg0->unk1C, arg0->unk20);
        func_80042034(0);
    }
    func_800428C8(0);
}


void func_80034F2C(MainMenuSceneActor *arg0) {
    if (func_80041FB4(0) == 1) {
        if (gEndingSequencePhase == 5) {
            arg0->unk2A += 1;
            if (arg0->unk2A == 0xA) {
                gEndingSequencePhase = 6;
            }
        }
        if (gEndingSequencePhase == 7) {
            arg0->unk2A = 0;
            setCallbackTaskCallback(arg0, func_80034E60);
            func_80041DD4(0, 0);
        }
    }
    func_800428C8(0);
}

void func_80034FC8(s32 arg0) {
    func_80041FB4(0);
    if (gEndingSequencePhase == 5) {
        setCallbackTaskCallback(arg0, func_80034F2C);
        func_80041DD4(0, 2);
        func_8003D218(0x55, -0x62, 0);
    }
    func_800428C8(0);
}

void func_80035030(MainMenuSceneActor *arg0) {
    func_80041FB4(0);
    if (gEndingSequencePhase == 3) {
        setCallbackTaskCallback(arg0, func_80034FC8);
        func_80041DD4(0, 2);
        func_8003D218(0x55, -0x62, 0);
        arg0->unk2A = 0;
    }
    func_800428C8(0);
}

void func_800350A4(MainMenuSceneActor *arg0) {
    s32 temp_a1;

    temp_a1 = (arg0->unk18 += 0xFFFD8FD8);
    if (temp_a1 < 0x900001) {
        arg0->unk18 = 0x900000;
        setCallbackTaskCallback(arg0, func_80035030);
        func_8004209C(0, arg0->unk18, arg0->unk1C, arg0->unk20);
        func_80041DD4(0, 1);
        func_8003D068(0x50, -0x68);
    } else {
        if (!temp_a1) {
        }
        func_8004209C(0, temp_a1, arg0->unk1C, arg0->unk20);
        func_80042034(0);
    }
    func_800428C8(0);
}

void func_80035150(void *arg0) {
    if (gEndingSequencePhase == 2) {
        setCallbackTaskCallback(arg0, func_800350A4);
    }
}

void func_80035184(MainMenuSceneActor *arg0) {
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
    setCallbackTaskCallback(arg0, func_80035150);
}
