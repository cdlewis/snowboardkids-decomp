#include "common.h"
#include "menu_rendering.h"

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
        /* 0x1E */ u16 step;
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

typedef struct {
    s16 alpha;
    s8 state;
} PlayerCountSelectMenuCursor;

typedef struct {
    /* 0x00 */ u8 pad0[0x7E];
    /* 0x7E */ u16 center[16];
    /* 0x9E */ u16 right[2];
    /* 0xA2 */ u16 bottom[2];
    /* 0xA6 */ u16 corner;
} PlayerCountSelectFrameTileMap;

typedef struct {
    /* 0x00 */ u8 pad0[0x42];
    /* 0x42 */ s16 textureHandle;
} PlayerCountSelectAssetHandles;

extern void func_80071824(void *task, void (*callback)());
extern void *func_80071408(void *, s32, s32);
extern void func_800716E4(void *);
extern void func_800483FC(void *, void *, void *);
extern void func_80029200(PlayerCountSelectRowActor *);
extern void func_800296D8(PlayerCountSelectWidgetActor *);
extern void func_8002980C(PlayerCountSelectWidgetActor *);
extern void func_80029FB8(PlayerCountSelectWidgetActor *);
extern void func_8002A458(PlayerCountSelectWidgetActor *);
extern void func_8002A008(PlayerCountSelectWidgetActor *);
extern void func_8002A27C(PlayerCountSelectWidgetActor *);
extern void func_8002A49C(PlayerCountSelectWidgetActor *);
extern void func_8002A710(PlayerCountSelectWidgetActor *);
extern void func_8002A8EC(PlayerCountSelectWidgetActor *);
extern void func_8002AB24(PlayerCountSelectWidgetActor *);
extern void func_8002AD74(PlayerCountSelectWidgetActor *);
extern void func_8002AE3C(PlayerCountSelectWidgetActor *);
extern void func_8002AFB8(PlayerCountSelectWidgetActor *);
extern void func_8002B05C(PlayerCountSelectWidgetActor *);
extern void func_8002B15C(PlayerCountSelectWidgetActor *);
extern void func_8002B1FC(PlayerCountSelectWidgetActor *);
extern void func_8002B2FC(PlayerCountSelectWidgetActor *);
extern void func_8002B424(PlayerCountSelectWidgetActor *);
extern void func_8002B524(PlayerCountSelectWidgetActor *);
extern void func_800112F4(s16, s16, s32, u16, u16, u16, s32, s32);
extern void func_8000F8AC(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_80013154(s32, s32, PlayerCountPortrait, s32, s32, s32);
extern void func_8001BA2C(s32, s32, s32, s32);
extern s32 func_80043040(s16);
extern int sprintf(char *, const char *, ...);
extern PlayerCountSelectFrameTileMap D_800B70F0;
extern u16 D_800B7196;
extern PlayerCountPortrait D_800B7198[];
extern PlayerCountSelectAssetHandles D_80112130;
extern s16 D_80112172;
extern PlayerCountSelectMenuCursor D_8010AF50;
extern u8 D_8010AF52;
extern s32 D_8010ADDC;
extern u8 D_80121B5E;
extern u8 D_80121D88;
extern s32 D_80121D8C;
extern s32 D_801235B4;
extern void *D_80124868;

const char D_800E0EA0[] = "%6dG";

#pragma GLOBAL_ASM("asm/nonmatchings/player_count_select_ui/func_80029200.s")

void func_80029344(PlayerCountSelectRowActor *arg0) {
    s32 i;
    s32 moved;
    s32 state;
    int stateByte;
    PlayerCountSelectRowActor *row;
    PlayerCountSelectRowActor *actor;

    stateByte = arg0->state;
    actor = arg0;
    state = stateByte;
    row = arg0;
    switch (state) {
    case 0:
        moved = 0;
        for (i = 0; i < row->playerCount; i++) {
            if (row->iconX[i] < -0x7C) {
                row->iconX[i] += 0x10;
                moved++;
                if (row->iconX[i] >= -0x7C) {
                    row->iconX[i] = -0x7C;
                }
            }
        }

        row->spawnTimer++;
        if (!(row->spawnTimer & 1)) {
            if (row->playerCount < 4) {
                row->playerCount++;
            }
        }

        if (moved == 0) {
            row->state = 1;
            func_80071408(func_80029FB8, 0, 0x5F);
            func_80071408(func_8002A458, 0, 0x60);
            func_80071408(func_8002A8EC, 0, 0x61);
            func_80071408(func_8002AD74, 0, 0x63);
        }
        state = arg0->state;
        break;
    case 1:
        if (D_80121D88 == 1) {
            state = (u8) (arg0->state = 2);
        }
        break;
    case 2:
        for (i = 0; i < 5; i++) {
            arg0->iconX[i] -= 0x20;
        }
        if (arg0->iconX[0] < -0x103) {
            arg0->state = 3;
        }
        break;
    case 3:
        break;
    }

    if (arg0->state == 3) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_80029200, actor);
}

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

void func_80029598(PlayerCountSelectWidgetActor *arg0) {
    func_8000F030(arg0->x, arg0->y, func_80043040(D_80112172), 3, 0x20, 0x20, 0, 0);
    func_8000F030((s16) (arg0->x + 0x40), arg0->y, func_80043040(D_80112172), 4, 0x20, 0x20, 0, 0);
    func_8000F030(arg0->x, (s16) (arg0->y + 0x40), func_80043040(D_80112172), 5, 0x20, 0x20, 0, 0);
    func_8000F030((s16) (arg0->x + 0x40), (s16) (arg0->y + 0x40), func_80043040(D_80112172), 6, 0x20, 0x20, 0, 0);
}

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

void func_80029CE4(PlayerCountSelectWidgetActor *arg0) {
    int state;

    if ((D_80121B5E >= (u16) arg0->widget.counter) && (arg0->row.bytes.subState != 0) && (arg0->y != -0x48)) {
        state = arg0->row.bytes.subState = 2;
    } else {
        state = arg0->row.bytes.subState;
        if ((D_80121B5E < (u16) arg0->widget.counter) && (state != 0) && (arg0->y != -0x140)) {
            state = arg0->row.bytes.subState = 1;
        } else {
            state = arg0->row.bytes.subState;
            if ((state != 0) && (state < 4)) {
                state = arg0->row.bytes.subState = 3;
            }
        }
    }

    switch (state) {
    case 0:
        arg0->x -= 0x20;
        if (arg0->row.bytes.subTimer == 0) {
            func_80071408(func_800297D8, 0, 0x63);
        }
        arg0->row.bytes.subTimer++;
        if (arg0->x < -7) {
            arg0->x = -8;
            arg0->row.bytes.subState = 3;
            D_8010ADDC = (s32) func_80071408(func_8002AFB8, 0, 0x64);
            func_80071408(func_8002B15C, 0, 0x64);
            func_80071408(func_8002B2FC, 0, 0x64);
            func_80071408(func_8002B524, 0, 0x64);
        }
        state = arg0->row.bytes.subState;
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->row.bytes.subState = 3;
        }
        state = arg0->row.bytes.subState;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->row.bytes.subState = 3;
        }
        state = arg0->row.bytes.subState;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->row.bytes.subState = 4;
        }
        state = arg0->row.bytes.subState;
        break;
    case 4:
        arg0->x += 0x20;
        if (arg0->x >= 0xA0) {
            arg0->row.bytes.subState = 5;
        }
        state = arg0->row.bytes.subState;
        break;
    case 5:
        D_80121D88 = 2;
        state = arg0->row.bytes.subState;
        break;
    }

    if ((state == 5) && (D_80121D88 == 2)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002980C, arg0);
}

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

void func_8002A27C(PlayerCountSelectWidgetActor *arg0) {
    int state;

    if ((D_80121B5E >= (u16) arg0->sprite.spriteIndex) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B5E < (u16) arg0->sprite.spriteIndex) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = arg0->transition.bytes.state;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
    case 5:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            if (arg0->y == -0x140) {
                arg0->transition.bytes.state = 5;
            } else {
                arg0->transition.bytes.state = 4;
            }
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        arg0->x += 0x20;
        if (arg0->x >= 0xA0) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    }

    if (state == 5) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002A008, arg0);
}

void func_8002A458(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.spriteIndex = 1;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8002A27C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_count_select_ui/func_8002A49C.s")

void func_8002A710(PlayerCountSelectWidgetActor *arg0) {
    int state;

    if ((D_80121B5E >= (u16) arg0->sprite.spriteIndex) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B5E < (u16) arg0->sprite.spriteIndex) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = arg0->transition.bytes.state;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
    case 5:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            if (arg0->y == -0x140) {
                arg0->transition.bytes.state = 5;
            } else {
                arg0->transition.bytes.state = 4;
            }
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        arg0->x += 0x20;
        if (arg0->x >= 0xA0) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    }

    if (state == 5) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002A49C, arg0);
}

void func_8002A8EC(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.spriteIndex = 2;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8002A710);
}

void func_8002A930(PlayerCountSelectWidgetActor *arg0) {
    PlayerCountSelectFrameTileMap *tileMap;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileMap = &D_800B70F0;
    tileOffset = 0;
    for (i = 0; i < 16; i++, tileOffset++) {
        func_800112F4(arg0->x + ((i & 3) << 5), arg0->y + ((i / 4) << 5), func_80043040(D_80112130.textureHandle),
                      tileMap->center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    tileMap = &D_800B70F0;
    tileOffset = 0;
    offset = 0; i = 0x80; do {
        func_800112F4(arg0->x + 0x80, arg0->y + offset, func_80043040(D_80112130.textureHandle),
                      tileMap->right[tileOffset], 0, 0x100, 0xA0, 0x49);
        func_800112F4(arg0->x + offset, arg0->y + 0x80, func_80043040(D_80112130.textureHandle),
                      tileMap->bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    func_800112F4(arg0->x + 0x80, arg0->y + 0x80, func_80043040(D_80112130.textureHandle),
                  D_800B7196, 0, 0x100, 0xA0, 0x49);
}

void func_8002AB24(PlayerCountSelectWidgetActor *arg0) {
    int state;

    if ((D_80121B5E == 3) && (arg0->y != -0x48) && ((s32) arg0->widget.bytes.state < 6)) {
        state = arg0->widget.bytes.state = 2;
    } else {
        state = (s32) arg0->widget.bytes.state;
        if ((D_80121B5E != 3) && (arg0->y != -0x140) && (state < 6)) {
            state = arg0->widget.bytes.state = 1;
        } else {
            state = arg0->widget.bytes.state;
            if (state < 4) {
                state = arg0->widget.bytes.state = 3;
            }
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->widget.bytes.state = 3;
        }
        state = arg0->widget.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->widget.bytes.state = 6;
            arg0->transition.step = 8;
        }
        state = arg0->widget.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            if (arg0->y == -0x140) {
                arg0->widget.bytes.state = 5;
            } else {
                arg0->widget.bytes.state = 4;
            }
        }
        state = arg0->widget.bytes.state;
        break;
    case 4:
        arg0->x += 0x20;
        if (arg0->x >= 0xA0) {
            arg0->widget.bytes.state = 5;
        }
        state = arg0->widget.bytes.state;
        break;
    case 6:
        arg0->y -= arg0->transition.step;
        state = arg0->widget.bytes.state = 7;
        break;
    case 7:
        arg0->y += arg0->transition.step;
        arg0->transition.step /= 2;
        if (arg0->transition.step == 0) {
            arg0->widget.bytes.state = 3;
        } else {
            arg0->widget.bytes.state = 6;
        }
        state = arg0->widget.bytes.state;
        break;
    }

    if (state == 5) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002A930, arg0);
}

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

void func_8002AE3C(PlayerCountSelectWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = D_8010AF50.state)) {
        arg0->transition.bytes.state = globalState;
        /* Preserve IDO's state/globalState register allocation. */
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        state = globalState;
        arg0->sprite.spriteIndex = D_8010AF50.alpha;
    }

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
        if ((s32) arg0->transition.bytes.timer < 0x10) {
            arg0->sprite.spriteIndex -= 9;
        } else {
            arg0->sprite.spriteIndex += 9;
        }
        state = arg0->transition.bytes.state;
        arg0->transition.bytes.timer = (arg0->transition.bytes.timer + 1) & 0x1F;
        break;
    case 2:
        if (D_80121D88 == 1) {
            state = arg0->transition.bytes.state = 3;
        }
        break;
    case 3:
        arg0->x -= 0x20;
        if (arg0->x < -0xEF) {
            arg0->transition.bytes.state = 4;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        break;
    }

    D_8010AF52 = state;
    if (arg0->transition.bytes.state == 4) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002ADB8, arg0);
}

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

void func_8002B1FC(PlayerCountSelectWidgetActor *arg0) {
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
    func_800483FC(&D_80124868, func_8002B198, arg0);
}

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

void func_8002B424(PlayerCountSelectWidgetActor *arg0) {
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
    func_800483FC(&D_80124868, func_8002B338, arg0);
}

void func_8002B524(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = 0x30;
    arg0->y = 0x40;
    arg0->sprite.spriteIndex = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8002B424);
}
