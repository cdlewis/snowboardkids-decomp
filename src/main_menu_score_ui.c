#include "common.h"

typedef struct MainMenuScoreTask MainMenuScoreTask;

extern void func_80071824(MainMenuScoreTask *, void *);
extern void func_800129DC(s16, s16, u8 *, s32, s32);
extern u8 D_800B79AC[];
extern s16 D_8010AF62;

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

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_score_ui/func_8002B560.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_score_ui/func_8002B8B4.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_score_ui/func_8002C390.s")

void func_8002C498(MainMenuScoreTask *arg0) {
    arg0->x = -0x24;
    arg0->y = -0x38;
    arg0->unk1C.scale = 0;
    arg0->state.b.unk1E = 0;
    func_80071824(arg0, func_8002C390);
}
