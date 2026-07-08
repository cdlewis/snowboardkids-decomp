#include "common.h"
#include "memory_allocator.h"
#include "effect_task_scheduler.h"
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
    char pad0[0x42];
    /* 0x42 */ s16 textureHandle;
} PlayerSelectAssetHandles;

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 iconX[5];
    /* 0x22 */ s16 iconY;
    /* 0x24 */ u8 state;
    /* 0x25 */ u8 spawnTimer;
    /* 0x26 */ u8 playerCount;
} PlayerSelectRowActor;

typedef struct {
    char pad0[0x18];
    s16 unk18[5];
    s16 unk22;
    s8 unk24;
    s8 unk25;
    s8 unk26;
} Struct1952C;

typedef struct {
    u8 state;
    char pad1[1];
    s16 nextState;
} PlayerSelectCursorState;

typedef u8 PlayerPortrait[0x8C];

typedef struct {
    /* 0x00 */ u16 centerTiles[16];
    /* 0x20 */ u16 rightEdgeTiles[2];
    /* 0x24 */ u16 bottomEdgeTiles[2];
    /* 0x28 */ u16 cornerTile;
} PlayerSelectFrameTiles;

extern void func_800483FC(void *, void *, void *);
extern void func_80011264(s16, s16, s32, u16, u16, u16);
extern void func_800112F4(s16, s16, s32, u16, u16, u16, s32, s32);
extern void func_80019800(PlayerSelectWidgetActor *);
extern void func_800196CC(PlayerSelectWidgetActor *);
extern void func_80019FAC(PlayerSelectWidgetActor *);
extern void func_8001A44C(PlayerSelectWidgetActor *);
extern void func_8001A490(PlayerSelectWidgetActor *);
extern void func_8001A8E0(PlayerSelectWidgetActor *);
extern void func_8001A704(PlayerSelectWidgetActor *);
extern void func_8001A924(PlayerSelectWidgetActor *);
extern void func_8001AD74(PlayerSelectWidgetActor *);
extern void func_8001AB98(PlayerSelectWidgetActor *);
extern void func_8001B210(PlayerSelectWidgetActor *);
extern void func_8001B02C(PlayerSelectWidgetActor *);
extern void func_8001B454(PlayerSelectWidgetActor *);
extern void func_8001B520(PlayerSelectWidgetActor *);
extern void func_8001B638(PlayerSelectWidgetActor *);
extern void func_8001B6D8(PlayerSelectWidgetActor *);
extern void func_8001B7D8(PlayerSelectWidgetActor *);
extern void func_8001B8F0(PlayerSelectWidgetActor *);
extern void func_8001B9F0(PlayerSelectWidgetActor *);
extern void func_8001BA2C(s32, s32, s32, s32);
extern int sprintf(char *, const char *, ...);
extern PlayerSelectFrameTiles D_800B5B50[];
extern PlayerPortrait D_800B5C24[];
extern u8 D_800EC9C1;
extern u8 D_800EC9C2;
extern u8 D_80121D85;
extern u8 D_80121D88;
extern void *D_80124868;
extern PlayerSelectAssetHandles D_80112130;
extern PlayerSelectCursorState D_8010AE70;
extern u8 D_8010AE70_state;
extern s16 D_8010AE74;
extern s32 D_8010ADDC;
extern u8 D_8010ADF8;
extern s16 D_80112172;
extern s32 D_80121D8C;
extern s32 D_801235B4;

const char D_800E0AE0[] = "%6dG";

void func_800191D0(PlayerSelectRowActor *arg0) {
    PlayerSelectRowActor *sp54;
    PlayerSelectRowActor *var_s3;
    s32 var_s0;
    s32 var_s1;
    s32 var_s2;

    sp54 = arg0;
    var_s0 = 0;
    if ((s32)arg0->playerCount > 0) {
        do { var_s2 = 0; var_s3 = arg0; do { var_s1 = 0; if (((((D_800EC9C1 != 0) && (((s32)D_800EC9C1) < 8)) && (D_8010ADF8 == 0)) && (var_s0 == D_800EC9C2)) && (D_800EC9C1 & 1)) { var_s1 = 0xFF; } func_8000F030(var_s3->iconX[0], (s16)(arg0->iconY + var_s2), func_80043040(D_80112130.textureHandle), (var_s0 + 8) & 0xFFFF, 0x20, 0x20, 0, var_s1); var_s0 += 1; var_s2 += 0x14; var_s3 = (PlayerSelectRowActor *)((u8 *)var_s3 + 2); if (arg0->playerCount) {} } while (var_s0 < ((s32)sp54->playerCount)); } while (0);
    }
}

void func_80019314(PlayerSelectRowActor *arg0) {
    s32 i;
    s32 moved;
    void (*callback)(PlayerSelectWidgetActor *);
    s32 state;
    int stateByte;
    PlayerSelectRowActor *row;
    PlayerSelectRowActor *actor;

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
            if (row->playerCount < 5) {
                row->playerCount++;
            }
        }

        if (moved == 0) {
            row->state = 1;
            func_80071408(func_80019FAC, 0, 0x5F);
            callback = func_8001A44C;
            func_80071408(callback, 0, 0x60);
            func_80071408(func_8001A8E0, 0, 0x61);
            func_80071408(func_8001AD74, 0, 0x62);
            func_80071408(func_8001B210, 0, 0x63);
            state++;
            state--;
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
    func_800483FC(&D_80124868, func_800191D0, actor);
}

void func_8001952C(Struct1952C *arg0) {
    s32 i;

    for (i = 0; i < 5; i++) { arg0->unk18[i] = -0x104; }

    arg0->unk22 = -0x60;
    arg0->unk25 = 0;
    arg0->unk26 = 1;
    arg0->unk24 = 0;

    func_80071824(arg0, func_80019314);
}

void func_8001958C(PlayerSelectWidgetActor *arg0) {
    func_8000F030(arg0->x, arg0->y, func_80043040(D_80112172), 3, 0x20, 0x20, 0, 0);
    func_8000F030((s16) (arg0->x + 0x40), arg0->y, func_80043040(D_80112172), 4, 0x20, 0x20, 0, 0);
    func_8000F030(arg0->x, (s16) (arg0->y + 0x40), func_80043040(D_80112172), 5, 0x20, 0x20, 0, 0);
    func_8000F030((s16) (arg0->x + 0x40), (s16) (arg0->y + 0x40), func_80043040(D_80112172), 6, 0x20, 0x20, 0, 0);
}

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

// func_80019800 best match: 96.706%
#pragma GLOBAL_ASM("asm/nonmatchings/player_select_ui/func_80019800.s")

#ifdef NON_MATCHING
void func_80019800(PlayerSelectWidgetActor *arg0) {
    s32 i;
    s32 tileIndex;
    s32 offset;

    tileIndex = 0;
    for (i = 0; i < 16; i++, tileIndex++) {
        func_80011264((s16)(arg0->x + ((i & 3) << 5)), (s16)(arg0->y + ((i / 4) << 5)),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5B50[(u16)arg0->counter].centerTiles[tileIndex], 0, 0x100);
    }

    tileIndex = 0;
    offset = 0;
    do {
        func_80011264((s16)(arg0->x + 0x80), (s16)(arg0->y + offset), func_80043040(D_80112130.textureHandle),
                      D_800B5B50[(u16)arg0->counter].rightEdgeTiles[tileIndex], 0, 0x100);
        func_80011264((s16)(arg0->x + offset), (s16)(arg0->y + 0x80), func_80043040(D_80112130.textureHandle),
                      D_800B5B50[(u16)arg0->counter].bottomEdgeTiles[tileIndex], 0, 0x100);
        offset += 0x40;
        tileIndex++;
    } while (offset < 0x80);

    func_80011264((s16)(arg0->x + 0x80), (s16)(arg0->y + 0x80), func_80043040(D_80112130.textureHandle),
                  D_800B5B50[(u16)arg0->counter].cornerTile, 0, 0x100);

    func_8000F030((s16)(arg0->x - 4), (s16)(arg0->y - 4), func_80043040(D_80112130.textureHandle), 0x33, 0x20,
                  0x20, 0, 0);
    func_8000F030((s16)(arg0->x - 4), (s16)(arg0->y + 0x8C), func_80043040(D_80112130.textureHandle), 0x38, 0x20,
                  0x20, 0, 0);
    func_8000F030((s16)(arg0->x + 0x8C), (s16)(arg0->y - 4), func_80043040(D_80112130.textureHandle), 0x35, 0x20,
                  0x20, 0, 0);
    func_8000F030((s16)(arg0->x + 0x8C), (s16)(arg0->y + 0x8C), func_80043040(D_80112130.textureHandle), 0x3A, 0x20,
                  0x20, 0, 0);

    offset = 0;
    do {
        func_8000F030((s16)(arg0->x + offset + 0xC), (s16)(arg0->y - 4),
                      func_80043040(D_80112130.textureHandle), 0x34, 0x20, 0x20, 0, 0);
        func_8000F030((s16)(arg0->x + offset + 0xC), (s16)(arg0->y + 0x8C),
                      func_80043040(D_80112130.textureHandle), 0x39, 0x20, 0x20, 0, 0);
        func_8000F030((s16)(arg0->x - 4), (s16)(arg0->y + offset + 0xC),
                      func_80043040(D_80112130.textureHandle), 0x36, 0x20, 0x20, 0, 0);
        func_8000F030((s16)(arg0->x + 0x8C), (s16)(arg0->y + offset + 0xC),
                      func_80043040(D_80112130.textureHandle), 0x37, 0x20, 0x20, 0, 0);
        offset += 0x10;
    } while (offset != 0x80);
}
#endif

void func_80019CD8(PlayerSelectWidgetActor *arg0) {
    int state;

    if ((D_800EC9C2 >= (u16) arg0->counter) && (arg0->row.bytes.subState != 0) && (arg0->y != -0x48)) {
        state = arg0->row.bytes.subState = 2;
    } else {
        state = arg0->row.bytes.subState;
        if ((D_800EC9C2 < (u16) arg0->counter) && (state != 0) && (arg0->y != -0x140)) {
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
            func_80071408(func_800197CC, 0, 0x63);
        }
        arg0->row.bytes.subTimer++;
        if (arg0->x < -7) {
            arg0->x = -8;
            arg0->row.bytes.subState = 3;
            D_8010ADDC = (s32) func_80071408(func_8001B454, 0, 0x64);
            func_80071408(func_8001B638, 0, 0x64);
            func_80071408(func_8001B7D8, 0, 0x64);
            func_80071408(func_8001B9F0, 0, 0x64);
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
    func_800483FC(&D_80124868, func_80019800, arg0);
}

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

void func_80019FFC(PlayerSelectWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        func_800112F4((s16)(arg0->x + ((i & 3) << 5)), (s16)(arg0->y + ((i / 4) << 5)),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5B50[(u16)arg0->sprite.spriteIndex].centerTiles[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        func_800112F4((s16)(arg0->x + 0x80), (s16)(arg0->y + offset),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5B50[(u16)arg0->sprite.spriteIndex].rightEdgeTiles[tileOffset], 0, 0x100, 0xA0, 0x49);
        func_800112F4((s16)(arg0->x + offset), (s16)(arg0->y + 0x80),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5B50[(u16)arg0->sprite.spriteIndex].bottomEdgeTiles[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    func_800112F4((s16)(arg0->x + 0x80), (s16)(arg0->y + 0x80), func_80043040(D_80112130.textureHandle),
                  D_800B5B50[(u16)arg0->sprite.spriteIndex].cornerTile, 0, 0x100, 0xA0, 0x49);
}

void func_8001A270(PlayerSelectWidgetActor *arg0) {
    int state;

    if ((D_800EC9C2 >= (u16) arg0->sprite.spriteIndex) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_800EC9C2 < (u16) arg0->sprite.spriteIndex) && (arg0->y != -0x140)) {
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
    func_800483FC(&D_80124868, func_80019FFC, arg0);
}

void func_8001A44C(PlayerSelectWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.spriteIndex = 1;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001A270);
}

void func_8001A490(PlayerSelectWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileIndex;
    s32 offset;

    tileIndex = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileIndex++) {
        func_800112F4((s16)(arg0->x + ((i & 3) << 5)), (s16)(arg0->y + ((i / 4) << 5)),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5B50[(u16)arg0->sprite.spriteIndex].centerTiles[tileIndex], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileIndex = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        func_800112F4((s16)(arg0->x + 0x80), (s16)(arg0->y + offset), func_80043040(D_80112130.textureHandle),
                      D_800B5B50[(u16)arg0->sprite.spriteIndex].rightEdgeTiles[tileIndex], 0, 0x100, 0xA0, 0x49);
        func_800112F4((s16)(arg0->x + offset), (s16)(arg0->y + 0x80), func_80043040(D_80112130.textureHandle),
                      D_800B5B50[(u16)arg0->sprite.spriteIndex].bottomEdgeTiles[tileIndex], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileIndex++;
    } while (offset != i);
    i++;
    i--;

    func_800112F4((s16)(arg0->x + 0x80), (s16)(arg0->y + 0x80), func_80043040(D_80112130.textureHandle),
                  D_800B5B50[(u16)arg0->sprite.spriteIndex].cornerTile, 0, 0x100, 0xA0, 0x49);
}

void func_8001A704(PlayerSelectWidgetActor *arg0) {
    int state;

    if ((D_800EC9C2 >= (u16) arg0->sprite.spriteIndex) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_800EC9C2 < (u16) arg0->sprite.spriteIndex) && (arg0->y != -0x140)) {
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
    func_800483FC(&D_80124868, func_8001A490, arg0);
}

void func_8001A8E0(PlayerSelectWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.spriteIndex = 2;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001A704);
}

void func_8001A924(PlayerSelectWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        func_800112F4((s16)(arg0->x + ((i & 3) << 5)), (s16)(arg0->y + ((i / 4) << 5)),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5B50[(u16)arg0->sprite.spriteIndex].centerTiles[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        func_800112F4((s16)(arg0->x + 0x80), (s16)(arg0->y + offset),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5B50[(u16)arg0->sprite.spriteIndex].rightEdgeTiles[tileOffset], 0, 0x100, 0xA0, 0x49);
        func_800112F4((s16)(arg0->x + offset), (s16)(arg0->y + 0x80),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5B50[(u16)arg0->sprite.spriteIndex].bottomEdgeTiles[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    func_800112F4((s16)(arg0->x + 0x80), (s16)(arg0->y + 0x80), func_80043040(D_80112130.textureHandle),
                  D_800B5B50[(u16)arg0->sprite.spriteIndex].cornerTile, 0, 0x100, 0xA0, 0x49);
}

void func_8001AB98(PlayerSelectWidgetActor *arg0) {
    int state;

    if ((D_800EC9C2 >= (u16) arg0->sprite.spriteIndex) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_800EC9C2 < (u16) arg0->sprite.spriteIndex) && (arg0->y != -0x140)) {
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
    func_800483FC(&D_80124868, func_8001A924, arg0);
}

void func_8001AD74(PlayerSelectWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.spriteIndex = 3;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001AB98);
}

void func_8001ADB8(PlayerSelectWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileIndex;
    s32 offset;

    tileIndex = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileIndex++) {
        func_800112F4((s16)(arg0->x + ((i & 3) << 5)), (s16)(arg0->y + ((i / 4) << 5)),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5B50[(u16)arg0->sprite.spriteIndex].centerTiles[tileIndex], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileIndex = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        func_800112F4((s16)(arg0->x + 0x80), (s16)(arg0->y + offset), func_80043040(D_80112130.textureHandle),
                      D_800B5B50[(u16)arg0->sprite.spriteIndex].rightEdgeTiles[tileIndex], 0, 0x100, 0xA0, 0x49);
        func_800112F4((s16)(arg0->x + offset), (s16)(arg0->y + 0x80), func_80043040(D_80112130.textureHandle),
                      D_800B5B50[(u16)arg0->sprite.spriteIndex].bottomEdgeTiles[tileIndex], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileIndex++;
    } while (offset != i);
    i++;
    i--;

    func_800112F4((s16)(arg0->x + 0x80), (s16)(arg0->y + 0x80), func_80043040(D_80112130.textureHandle),
                  D_800B5B50[(u16)arg0->sprite.spriteIndex].cornerTile, 0, 0x100, 0xA0, 0x49);
}

void func_8001B02C(PlayerSelectWidgetActor *arg0) {
    int state;

    D_801235B4 = 0;
    if ((D_800EC9C2 >= (u16) arg0->sprite.spriteIndex) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_800EC9C2 < (u16) arg0->sprite.spriteIndex) && (arg0->y != -0x140)) {
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
    func_800483FC(&D_80124868, func_8001ADB8, arg0);
}

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

void func_8001B2D8(PlayerSelectWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = D_8010AE70.state)) {
        arg0->transition.bytes.state = globalState;
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        state = globalState;
        arg0->sprite.spriteIndex = D_8010AE70.nextState;
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

    D_8010AE70_state = state;
    if (arg0->transition.bytes.state == 4) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001B254, arg0);
}

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

void func_8001B520(PlayerSelectWidgetActor *arg0) {
    u8 state = arg0->transition.bytes.state;

    switch (state) {
    case 0:
        arg0->sprite.spriteIndex += 0x26;
        if (arg0->sprite.spriteIndex >= 0x100) {
            arg0->sprite.spriteIndex = 0x100;
            arg0->transition.bytes.state = 1;
        }
        break;
    case 1:
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 2;
        }
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0xFF) {
            arg0->transition.bytes.state = 3;
        }
        break;
    case 3:
        break;
    }
    D_8010AE74 = arg0->sprite.spriteIndex;
    if (arg0->transition.bytes.state == 3) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001B494, arg0);
}

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
