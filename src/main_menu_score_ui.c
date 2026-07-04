#include "common.h"

typedef struct MainMenuScoreTask MainMenuScoreTask;

extern void func_80071824(MainMenuScoreTask *, void *);
extern void func_800129DC(s16, s16, u8 *, s32, s32);
extern void *func_80071408(void *, s32, s32);
extern void func_800716E4(void *);
extern void func_800483FC(void *, void *, void *);
extern u8 D_800B79AC[];
extern s16 D_8010AF62;
extern void *D_80124868;
extern void *D_8010ADE0;
extern s16 D_800EC9C8;
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
            s16 slideOffset;
            u16 selection;
            u16 alpha;
            u16 alphaTimer;
        } w;
    } state;
};

extern void func_8002B8B4(MainMenuScoreTask *);
extern void func_8002BB24(MainMenuScoreTask *);
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

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_score_ui/func_8002BA38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_score_ui/func_8002BB24.s")

void func_8002BC60(MainMenuScoreTask *arg0) {
    arg0->x = -0x44;
    arg0->y = -0x1E;
    arg0->unk1C.scale = 0;
    arg0->state.b.unk1F = 0;
    func_80071824(arg0, func_8002BB24);
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_score_ui/func_8002BC9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_score_ui/func_8002BDAC.s")

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
