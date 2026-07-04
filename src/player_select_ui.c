#include "common.h"

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    union {
        /* 0x1C */ s16 spriteIndex;
        struct {
            /* 0x1C */ u8 state;
            /* 0x1D */ u8 pad1D;
        } bytes;
    } sprite;
    union {
        struct {
            /* 0x1E */ u8 state;
            /* 0x1F */ u8 timer;
        } bytes;
        /* 0x1E */ s16 alpha;
    } transition;
    /* 0x20 */ s16 counter;
    union {
        struct {
            /* 0x22 */ u8 subState;
            /* 0x23 */ u8 subTimer;
        } bytes;
        /* 0x22 */ s16 rowY;
    } row;
} PlayerSelectWidgetActor;

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 iconX[5];
    /* 0x22 */ s16 iconY;
    /* 0x24 */ u8 state;
    /* 0x25 */ u8 spawnTimer;
    /* 0x26 */ u8 playerCount;
} PlayerSelectRowActor;

typedef u8 PlayerPortrait[0x8C];

extern void func_80071824(void *, void *);
extern void func_800716E4(void *);
extern void func_800483FC(void *, void *, void *);
extern void func_8001958C(PlayerSelectWidgetActor *);
extern void func_800196CC(PlayerSelectWidgetActor *);
extern void func_80019CD8(PlayerSelectWidgetActor *);
extern void func_8001A270(PlayerSelectWidgetActor *);
extern void func_8001A704(PlayerSelectWidgetActor *);
extern void func_8001AB98(PlayerSelectWidgetActor *);
extern void func_8001B02C(PlayerSelectWidgetActor *);
extern void func_8001B2D8(PlayerSelectWidgetActor *);
extern void func_8001B520(PlayerSelectWidgetActor *);
extern void func_8001B6D8(PlayerSelectWidgetActor *);
extern void func_8001B8F0(PlayerSelectWidgetActor *);
extern void func_8000F8AC(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_80013D0C(s32, s32, char *, s32, s32);
extern void func_80013154(s32, s32, PlayerPortrait, s32, s32, s32);
extern void func_8001BA2C(s32, s32, s32, s32);
extern s32 func_80043040(s16);
extern int sprintf(char *, const char *, ...);
extern PlayerPortrait D_800B5C24[];
extern u8 D_800EC9C2;
extern u8 D_80121D85;
extern u8 D_80121D88;
extern void *D_80124868;
extern s16 D_80112172;
extern s32 D_80121D8C;

const char D_800E0AE0[] = "%6dG";

#pragma GLOBAL_ASM("asm/nonmatchings/player_select_ui/func_800191D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_select_ui/func_80019314.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_select_ui/func_8001952C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_select_ui/func_8001958C.s")

void func_800196CC(PlayerSelectWidgetActor *arg0) {
    u8 state = arg0->sprite.bytes.state;

    switch (state) {
    case 0:
        arg0->x += 0x20;
        if (arg0->x >= -0x88) {
            arg0->x = -0x88;
            arg0->sprite.bytes.state = 1;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 1:
        if (D_80121D88 == 1) {
            state = arg0->sprite.bytes.state = 2;
        }
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0x10D) {
            arg0->sprite.bytes.state = 3;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 3:
        break;
    }
    if ((unsigned int)state == 3) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001958C, arg0);
}

void func_800197CC(PlayerSelectWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = 8;
    func_80071824(arg0, func_800196CC);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_select_ui/func_80019800.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_select_ui/func_80019CD8.s")

void func_80019FAC(PlayerSelectWidgetActor *arg0) {
    arg0->x = 0x94;
    arg0->y = -0x48;
    arg0->sprite.spriteIndex = -8;
    arg0->transition.alpha = -0x74;
    arg0->counter = 0;
    arg0->row.bytes.subTimer = 0;
    arg0->row.bytes.subState = 0;
    func_80071824(arg0, func_80019CD8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_select_ui/func_80019FFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_select_ui/func_8001A270.s")

void func_8001A44C(PlayerSelectWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.spriteIndex = 1;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001A270);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_select_ui/func_8001A490.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_select_ui/func_8001A704.s")

void func_8001A8E0(PlayerSelectWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.spriteIndex = 2;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001A704);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_select_ui/func_8001A924.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_select_ui/func_8001AB98.s")

void func_8001AD74(PlayerSelectWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.spriteIndex = 3;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001AB98);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_select_ui/func_8001ADB8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_select_ui/func_8001B02C.s")

void func_8001B210(PlayerSelectWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.spriteIndex = 4;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001B02C);
}

void func_8001B254(PlayerSelectWidgetActor *arg0) {
    func_8000F8AC(arg0->x, (s16)(arg0->y + (D_800EC9C2 * 0x14)), func_80043040(D_80112172), 7, 0x20, 0x20, 0, arg0->sprite.spriteIndex, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_select_ui/func_8001B2D8.s")

void func_8001B454(PlayerSelectWidgetActor *arg0) {
    arg0->x = -0x7C;
    arg0->y = -0x60;
    arg0->sprite.spriteIndex = 0;
    arg0->transition.bytes.state = 0;
    arg0->transition.bytes.timer = 0;
    func_80071824(arg0, func_8001B2D8);
}

void func_8001B494(PlayerSelectWidgetActor *arg0) {
    s32 portraitIndex;

    if (D_800EC9C2 == 3) {
        portraitIndex = D_800EC9C2 & 0xFF;
        if (D_80121D85 == 5) {
            portraitIndex = 5;
        }
    } else {
        portraitIndex = D_800EC9C2 & 0xFF;
    }
    func_80013154(arg0->x, arg0->y, D_800B5C24[portraitIndex], 1, arg0->sprite.spriteIndex, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_select_ui/func_8001B520.s")

void func_8001B638(PlayerSelectWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = 0xC;
    arg0->sprite.spriteIndex = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001B520);
}

void func_8001B674(PlayerSelectWidgetActor *arg0) {
    func_8000F8AC(arg0->x, arg0->y, func_80043040(D_80112172), 0, 0x20, 0x20, 0, arg0->sprite.spriteIndex, 0);
}

void func_8001B6D8(PlayerSelectWidgetActor *arg0) {
    u8 state = arg0->transition.bytes.state;

    switch (state) {
    case 0:
        arg0->sprite.spriteIndex += 0x26;
        if (arg0->sprite.spriteIndex >= 0x100) {
            arg0->sprite.spriteIndex = 0x100;
            arg0->transition.bytes.state = 1;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        if (D_80121D88 == 1) {
            state = arg0->transition.bytes.state = 2;
        }
        break;
    case 2:
        arg0->x += 0x20;
        if (arg0->x >= 0xA0) {
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        break;
    }
    if ((unsigned int)state == 3) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001B674, arg0);
}

void func_8001B7D8(PlayerSelectWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x5C;
    arg0->sprite.spriteIndex = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001B6D8);
}

void func_8001B814(PlayerSelectWidgetActor *arg0) {
    char sp40[0x18];

    func_8001BA2C(arg0->x, arg0->y, 0x5000, 0x4000);
    func_8000F8AC((s16)(arg0->x + 8), (s16)(arg0->y + 4), func_80043040(D_80112172), 0x11, 0x20, 0x20, 0, arg0->sprite.spriteIndex, 0);
    sprintf(sp40, D_800E0AE0, D_80121D8C);
    func_80013D0C((s16)(arg0->x + 0x10), (s16)(arg0->y + 0x10), sp40, 0, arg0->sprite.spriteIndex);
}

void func_8001B8F0(PlayerSelectWidgetActor *arg0) {
    u8 state = arg0->transition.bytes.state;

    switch (state) {
    case 0:
        arg0->sprite.spriteIndex += 0x26;
        if (arg0->sprite.spriteIndex >= 0x100) {
            arg0->sprite.spriteIndex = 0x100;
            arg0->transition.bytes.state = 1;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        if (D_80121D88 == 1) {
            state = arg0->transition.bytes.state = 2;
        }
        break;
    case 2:
        arg0->x += 0x20;
        if (arg0->x >= 0xA0) {
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        break;
    }
    if ((unsigned int)state == 3) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001B814, arg0);
}

void func_8001B9F0(PlayerSelectWidgetActor *arg0) {
    arg0->x = 0x30;
    arg0->y = 0x40;
    arg0->sprite.spriteIndex = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001B8F0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_select_ui/func_8001BA2C.s")
