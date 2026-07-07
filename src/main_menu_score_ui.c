#include "common.h"

typedef struct MainMenuScoreTask MainMenuScoreTask;

typedef struct {
    u8 state;
    u8 pad1;
    s16 scale;
    u16 slideOffset;
} MainMenuScoreTransition;

extern void func_80071824(void *task, void (*callback)());
extern void func_800129DC(s16, s16, u8 *, s32, s32);
extern void func_80013154(s32, s32, u8 *, s32, s32, s32);
extern void func_8000F030(s16, s16, s32, s32, s32, s32, s32, s32);
extern void func_8000F8AC(s32, s32, s32, s32, s32, s32, s32, u16, s32);
extern s32 func_80043040(s16);
extern void *func_80071408(void *, s32, s32);
extern void func_800716E4(void *);
extern void func_800483FC(void *, void *, void *);
extern u8 D_800B73F0[][0x4C];
extern u8 D_800B79AC[];
extern s16 D_80112172;
extern s16 D_8010AF62;
extern MainMenuScoreTransition D_8010AF60;
extern u8 D_8010AF60_state;
extern void *D_80124868;
extern void *D_8010ADE0;
extern void *D_8010ADE4;
extern s16 D_800EC9C8;
extern s16 D_800EC9D0;
extern u8 D_800EC9C1;

struct MainMenuScoreTask {
    char pad[0x18];
    s16 x;
    s16 y;
    union {
        s8 state;
        s16 scale;
    } unk1C;
    union {
        struct {
            s8 unk1E;
            s8 unk1F;
            s8 unk20;
            u8 frame;
            u16 alpha;
            u16 alphaTimer;
        } b;
        struct {
            u16 slideOffset;
            u16 selection;
            u16 alpha;
            u16 alphaTimer;
        } w;
    } state;
};

extern void func_8002B8B4(MainMenuScoreTask *);
extern void func_8002BA38(MainMenuScoreTask *);
extern void func_8002BB24(MainMenuScoreTask *);
extern void func_8002BC9C(MainMenuScoreTask *);
extern void func_8002BF54(MainMenuScoreTask *);
extern void func_8002BDAC(MainMenuScoreTask *);
extern void func_8002C18C(MainMenuScoreTask *);
extern void func_8002C390(MainMenuScoreTask *);
void func_8002B560(MainMenuScoreTask *arg0);
void func_8002BC60(MainMenuScoreTask *arg0);
void func_8002C498(MainMenuScoreTask *arg0);

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_score_ui/func_8002B560.s")

void func_8002B8B4(MainMenuScoreTask *arg0) {
    u8 state = arg0->unk1C.state;

    switch (state) {
    case 0:
        arg0->x -= 0x20;
        if (arg0->x < -0x43) {
            arg0->x = -0x44;
            arg0->unk1C.state = 1;
            D_8010ADE0 = func_80071408(func_8002BC60, 0, 0x62);
            func_80071408(func_8002C498, 0, 0x63);
        }
        state = arg0->unk1C.state;
        break;
    case 1:
        if (D_800EC9C8 != 0) {
            if (D_800EC9C1 == 0x14) {
                state = arg0->unk1C.state = 2;
            }
        }
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0x117) {
            arg0->unk1C.state = 3;
        }
        state = arg0->unk1C.state;
        break;
    case 3:
        break;
    }
    state = arg0->unk1C.state;
    if (state == 3) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002B560, arg0);
}

void func_8002BA00(MainMenuScoreTask *arg0) {
    arg0->x = 0x90;
    arg0->y = -0x20;
    arg0->unk1C.state = 0;
    func_80071824(arg0, func_8002B8B4);
}

void func_8002BA38(MainMenuScoreTask *arg0) {
    s32 zero = 0;
    u16 alpha;

    if (D_800EC9C8 != 8) {
        alpha = arg0->unk1C.scale;
        func_8000F8AC((s16)(arg0->x - 2), (s16)(arg0->y + 0xC), func_80043040(D_80112172), 8, 0x20, 0x20,
                      zero, alpha, zero);
        func_8000F8AC((s16)(arg0->x + 0x3E), (s16)(arg0->y + 0xC), func_80043040(D_80112172), 9, 0x20,
                      0x20, zero, alpha, zero);
    }
}

void func_8002BB24(MainMenuScoreTask *arg0) {
    u8 state = arg0->state.b.unk1F;

    switch (state) {
    case 0:
        arg0->unk1C.scale += 0x26;
        if (arg0->unk1C.scale >= 0x100) {
            arg0->unk1C.scale = 0x100;
            arg0->state.b.unk1F = 1;
            D_8010ADE4 = func_80071408(func_8002BF54, 0, 0x63);
        }
        state = arg0->state.b.unk1F;
        break;
    case 1:
        state = arg0->state.b.unk1F = 2;
        break;
    case 2:
        if ((D_800EC9C8 == 5) && (D_800EC9C1 == 0x14)) {
            state = arg0->state.b.unk1F = 3;
        }
        break;
    case 3:
        arg0->x -= 0x20;
        if (arg0->x < -0x117) {
            arg0->state.b.unk1F = 4;
        }
        state = arg0->state.b.unk1F;
        break;
    case 4:
        break;
    }
    if ((unsigned int)state == 4) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002BA38, arg0);
}

void func_8002BC60(MainMenuScoreTask *arg0) {
    arg0->x = -0x44;
    arg0->y = -0x1E;
    arg0->unk1C.scale = 0;
    arg0->state.b.unk1F = 0;
    func_80071824(arg0, func_8002BB24);
}

void func_8002BC9C(MainMenuScoreTask *arg0) {
    u8 *text;

    if (D_800EC9C8 != 8) {
        text = D_800B73F0[D_800EC9C8];
        func_80013154(arg0->x, arg0->y, text, 1, arg0->unk1C.scale, 0);
        if (((D_800EC9C8 == 4) || (D_800EC9C8 >= 7)) && (D_800EC9D0 == 0)) {
            if (arg0->unk1C.scale == 0x100) {
                func_8000F030((s16)(arg0->x + 0x70), (s16)(arg0->y + 0x10), func_80043040(D_80112172),
                              (((s32)arg0->state.b.frame >= 8) + 5) & 0xFFFF, 0x20, 0x20, 0, 0);
            }
        }
    }
}

// func_8002BDAC best match: 99.714%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_score_ui/func_8002BDAC.s")

#ifdef NON_MATCHING
void func_8002BDAC(MainMenuScoreTask *arg0) {
    u8 state;
    unsigned long globalState;

    state = arg0->state.b.unk20;
    if (state != (globalState = D_8010AF60.state)) {
        arg0->state.b.unk20 = globalState;
        state = globalState;
        arg0->unk1C.scale = D_8010AF60.scale;
        arg0->state.w.slideOffset = D_8010AF60.slideOffset;
    }

    switch (state) {
    case 0:
        arg0->state.b.unk20 = 1;
        state = arg0->state.b.unk20;
        break;
    case 1:
        if ((D_800EC9C8 == 5) && (D_800EC9C1 == 0x14)) {
            arg0->state.b.unk20 = 4;
            state = arg0->state.b.unk20;
        }
        break;
    case 2:
        arg0->unk1C.scale += 0x26;
        if (arg0->unk1C.scale >= 0x100) {
            arg0->unk1C.scale = 0x100;
            arg0->state.b.unk20 = 1;
        }
        state = arg0->state.b.unk20;
        break;
    case 3:
        arg0->unk1C.scale -= 0x26;
        if (arg0->unk1C.scale <= (state * 0)) {
            arg0->unk1C.scale = 0;
            arg0->state.b.unk20 = 2;
            D_800EC9D0 = 0;
            D_800EC9C8 = (u16)arg0->state.w.slideOffset;
        }
        state = arg0->state.b.unk20;
        break;
    case 4:
        arg0->x -= 0x20;
        if (arg0->x < -0x117) {
            arg0->state.b.unk20 = 5;
        }
        state = arg0->state.b.unk20;
        break;
    }

    arg0->state.b.frame = (arg0->state.b.frame + 1) & 0xF;
    D_8010AF60_state = state;
    if ((u8)arg0->state.b.unk20 == 5) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002BC9C, arg0);
}
#endif

void func_8002BF54(MainMenuScoreTask *arg0) {
    arg0->x = -0x42;
    arg0->y = -0xE;
    arg0->unk1C.scale = 0x100;
    D_8010AF62 = 0x100;
    arg0->state.b.unk20 = 0;
    func_80071824(arg0, func_8002BDAC);
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_score_ui/func_8002BF9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_score_ui/func_8002C18C.s")

void func_8002C318(MainMenuScoreTask *arg0) {
    arg0->x = -0x28;
    arg0->y = 0xC;
    arg0->unk1C.scale = 0xC;
    func_80071824(arg0, func_8002C18C);
}

void func_8002C350(MainMenuScoreTask *arg0) {
    func_800129DC(arg0->x, arg0->y, D_800B79AC, 1, arg0->unk1C.scale);
}

void func_8002C390(MainMenuScoreTask *arg0) {
    u8 state = arg0->state.b.unk1E;

    switch (state) {
    case 0:
        arg0->unk1C.scale += 0x26;
        if (arg0->unk1C.scale >= 0x100) {
            arg0->unk1C.scale = 0x100;
            arg0->state.b.unk1E = 1;
        }
        state = arg0->state.b.unk1E;
        break;
    case 1:
        if ((D_800EC9C8 == 5) && (D_800EC9C1 == 0x14)) {
            state = arg0->state.b.unk1E = 2;
        }
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0xD7) {
            arg0->state.b.unk1E = 4;
        }
        state = arg0->state.b.unk1E;
        break;
    }
    if ((unsigned int)state == 4) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002C350, arg0);
}

void func_8002C498(MainMenuScoreTask *arg0) {
    arg0->x = -0x24;
    arg0->y = -0x38;
    arg0->unk1C.scale = 0;
    arg0->state.b.unk1E = 0;
    func_80071824(arg0, func_8002C390);
}
