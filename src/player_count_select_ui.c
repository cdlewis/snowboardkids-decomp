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
    union {
        struct {
            /* 0x20 */ u8 state;
            /* 0x21 */ u8 timer;
        } bytes;
        /* 0x20 */ s16 counter;
    } widget;
    union {
        struct {
            /* 0x22 */ u8 subState;
            /* 0x23 */ u8 subTimer;
        } bytes;
        /* 0x22 */ s16 rowY;
    } row;
} PlayerCountSelectWidgetActor;

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 iconX[5];
    /* 0x22 */ s16 iconY;
    /* 0x24 */ u8 state;
    /* 0x25 */ u8 spawnTimer;
    /* 0x26 */ u8 playerCount;
} PlayerCountSelectRowActor;

typedef u8 PlayerCountPortrait[0x8C];

extern void func_80071824(void *, void *);
extern void func_800716E4(void *);
extern void func_800483FC(void *, void *, void *);
extern void func_80029344(PlayerCountSelectRowActor *);
extern void func_80029598(PlayerCountSelectWidgetActor *);
extern void func_800296D8(PlayerCountSelectWidgetActor *);
extern void func_80029CE4(PlayerCountSelectWidgetActor *);
extern void func_8002A27C(PlayerCountSelectWidgetActor *);
extern void func_8002A710(PlayerCountSelectWidgetActor *);
extern void func_8002AB24(PlayerCountSelectWidgetActor *);
extern void func_8002AE3C(PlayerCountSelectWidgetActor *);
extern void func_8002B05C(PlayerCountSelectWidgetActor *);
extern void func_8002B1FC(PlayerCountSelectWidgetActor *);
extern void func_8002B424(PlayerCountSelectWidgetActor *);
extern void func_8000F8AC(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_80013154(s32, s32, PlayerCountPortrait, s32, s32, s32);
extern void func_8001BA2C(s32, s32, s32, s32);
extern void func_80013D0C(s32, s32, char *, s32, s32);
extern s32 func_80043040(s16);
extern int sprintf(char *, const char *, ...);
extern PlayerCountPortrait D_800B7198[];
extern s16 D_80112172;
extern u8 D_80121B5E;
extern u8 D_80121D88;
extern s32 D_80121D8C;
extern void *D_80124868;

const char D_800E0EA0[] = "%6dG";

#pragma GLOBAL_ASM("asm/nonmatchings/player_count_select_ui/func_80029200.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_count_select_ui/func_80029344.s")

void func_80029548(PlayerCountSelectRowActor *arg0) {
    s32 temp_v1 = -0x104;
    long long zero;
    s32 temp_t6 = -0x58;
    s32 temp_t7 = 1;

    arg0->iconX[0] = temp_v1;
    arg0->iconX[1] = temp_v1;
    arg0->iconX[2] = temp_v1;
    arg0->iconX[3] = temp_v1;
    arg0->iconY = temp_t6;
    zero = 0;
    arg0->spawnTimer = zero;
    arg0->playerCount = temp_t7;
    arg0->state = zero;
    func_80071824(arg0, func_80029344);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_count_select_ui/func_80029598.s")

void func_800296D8(PlayerCountSelectWidgetActor *arg0) {
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
    func_800483FC(&D_80124868, func_80029598, arg0);
}

void func_800297D8(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = 8;
    func_80071824(arg0, func_800296D8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_count_select_ui/func_8002980C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_count_select_ui/func_80029CE4.s")

void func_80029FB8(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = 0x94;
    arg0->y = -0x48;
    arg0->sprite.spriteIndex = -8;
    arg0->transition.alpha = -0x74;
    arg0->widget.counter = 0;
    arg0->row.bytes.subTimer = 0;
    arg0->row.bytes.subState = 0;
    func_80071824(arg0, func_80029CE4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_count_select_ui/func_8002A008.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_count_select_ui/func_8002A27C.s")

void func_8002A458(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.spriteIndex = 1;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8002A27C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_count_select_ui/func_8002A49C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_count_select_ui/func_8002A710.s")

void func_8002A8EC(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.spriteIndex = 2;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8002A710);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_count_select_ui/func_8002A930.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_count_select_ui/func_8002AB24.s")

void func_8002AD74(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.spriteIndex = 2;
    arg0->widget.bytes.timer = 0;
    arg0->widget.bytes.state = 0;
    func_80071824(arg0, func_8002AB24);
}

void func_8002ADB8(PlayerCountSelectWidgetActor *arg0) {
    func_8000F8AC(arg0->x, (s16)(arg0->y + (D_80121B5E * 0x18)), func_80043040(D_80112172), 7, 0x20, 0x20, 0, arg0->sprite.spriteIndex, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_count_select_ui/func_8002AE3C.s")

void func_8002AFB8(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = -0x7C;
    arg0->y = -0x58;
    arg0->sprite.spriteIndex = 0;
    arg0->transition.bytes.state = 0;
    arg0->transition.bytes.timer = 0;
    func_80071824(arg0, func_8002AE3C);
}

void func_8002AFF8(PlayerCountSelectWidgetActor *arg0) {
    PlayerCountPortrait *portrait = &D_800B7198[D_80121B5E];

    func_80013154(arg0->x, arg0->y, *portrait, 1, arg0->sprite.spriteIndex, 0);
}

void func_8002B05C(PlayerCountSelectWidgetActor *arg0) {
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
        arg0->x -= 0x20;
        if (arg0->x < -0xFF) {
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
    func_800483FC(&D_80124868, func_8002AFF8, arg0);
}

void func_8002B15C(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = 0xC;
    arg0->sprite.spriteIndex = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8002B05C);
}

void func_8002B198(PlayerCountSelectWidgetActor *arg0) {
    func_8000F8AC(arg0->x, arg0->y, func_80043040(D_80112172), 2, 0x20, 0x20, 0, arg0->sprite.spriteIndex, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_count_select_ui/func_8002B1FC.s")

void func_8002B2FC(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x5C;
    arg0->sprite.spriteIndex = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8002B1FC);
}

void func_8002B338(PlayerCountSelectWidgetActor *arg0) {
    char sp40[0x18];

    if (D_80121B5E != 3) {
        func_8001BA2C(arg0->x, arg0->y, 0x5000, 0x4000);
        func_8000F8AC((s16)(arg0->x + 8), (s16)(arg0->y + 4), func_80043040(D_80112172), 0x11, 0x20, 0x20, 0, arg0->sprite.spriteIndex, 0);
        sprintf(sp40, D_800E0EA0, D_80121D8C);
        func_80013D0C((s16)(arg0->x + 0x10), (s16)(arg0->y + 0x10), sp40, 0, arg0->sprite.spriteIndex);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_count_select_ui/func_8002B424.s")

void func_8002B524(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = 0x30;
    arg0->y = 0x40;
    arg0->sprite.spriteIndex = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8002B424);
}
