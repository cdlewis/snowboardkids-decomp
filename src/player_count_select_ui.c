#include "common.h"
#include "memory_allocator.h"
#include "callback_task_scheduler.h"
#include "player_count_select_ui.h"
#include "player_select_ui.h"
#define MENU_RENDERING_BROAD_PROTOTYPES
#include "menu_rendering.h"

typedef u8 PlayerCountPortrait[0x8C];

typedef struct {
    s16 alpha;
    s8 state;
} PlayerCountSelectMenuCursor;

typedef struct {
    /* 0x00 */ u16 center[16];
    /* 0x20 */ u16 right[2];
    /* 0x24 */ u16 bottom[2];
    /* 0x28 */ u16 corner;
} PlayerCountSelectFrameTileMap;

typedef union {
    /* 0x00 */ PlayerCountSelectFrameTileMap entries[3];
    /* 0x00 */ PlayerCountSelectFrameTileMap frames[3];
    struct {
        /* 0x00 */ u8 pad0[0x7E];
        /* 0x7E */ PlayerCountSelectFrameTileMap selected;
    } padded;
} PlayerCountSelectFrameTileMapTable;

typedef struct {
    /* 0x00 */ u8 pad0[0x7E];
    /* 0x7E */ PlayerCountSelectFrameTileMap selected;
} PlayerCountSelectPaddedFrameTileMapTable;

typedef struct {
    /* 0x00 */ u8 pad0[0x42];
    /* 0x42 */ s16 textureHandle;
    /* 0x44 */ u8 pad44[0x6];
    /* 0x4A */ s16 frameTextureHandle;
} PlayerCountSelectAssetHandles;

extern void addRenderCallback(void *, void *, void *);
extern int sprintf(char *, const char *, ...);
extern u8 D_800EC9C1;
extern PlayerCountSelectFrameTileMapTable D_800B70F0;
extern u16 D_800B7196;
extern PlayerCountPortrait D_800B7198[];
extern PlayerCountSelectAssetHandles D_80112130;
extern s16 gMenuCommonSpritesAssetHandle;
extern PlayerCountSelectMenuCursor D_8010AF50;
extern u8 D_8010AF52;
extern s32 gActiveMenuTask;
extern u8 D_8010ADF8;
extern u8 D_80121B5E;
extern u8 D_80121D88;
extern s32 D_80121D8C;
extern s32 D_801235B4;
extern void *gMenuRenderCallbackList;

const char D_800E0EA0[] = "%6dG";

void func_80029200(PlayerCountSelectRowActor *arg0) {
    PlayerCountSelectRowActor *savedArg;
    s32 i;
    s32 alpha;
    s32 yOffset;
    PlayerCountSelectRowActor *row;

    savedArg = arg0;
 do { i = 0; if (arg0->playerCount > 0) { yOffset = 0; row = arg0; do { alpha = 0; if (((((D_800EC9C1 > 0) && (D_800EC9C1 < 8)) && (D_8010ADF8 == 0)) && (i == D_80121B5E)) && (D_800EC9C1 & 1)) { if ((!savedArg->playerCount) && (!savedArg->playerCount)) { } alpha = 0xFF; } drawMenuSprite(row->iconX[0], (s16) (arg0->iconY + yOffset), getMemoryBlockBase(D_80112130.textureHandle), (i + 0xD) & 0xFFFF, 0x20, 0x20, 0, alpha); i++; yOffset += 0x18; row = (PlayerCountSelectRowActor *) (((u8 *) row) + 2); } while (i < savedArg->playerCount); } } while (0);
}

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
            createCallbackTask(func_80029FB8, 0, 0x5F);
            createCallbackTask(func_8002A458, 0, 0x60);
            createCallbackTask(func_8002A8EC, 0, 0x61);
            createCallbackTask(func_8002AD74, 0, 0x63);
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
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, func_80029200, actor);
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
    setCallbackTaskCallback(arg0, func_80029344);
}

void func_80029598(PlayerCountSelectWidgetActor *arg0) {
    drawMenuSprite(arg0->x, arg0->y, getMemoryBlockBase(gMenuCommonSpritesAssetHandle), 3, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16) (arg0->x + 0x40), arg0->y, getMemoryBlockBase(gMenuCommonSpritesAssetHandle), 4, 0x20, 0x20, 0, 0);
    drawMenuSprite(arg0->x, (s16) (arg0->y + 0x40), getMemoryBlockBase(gMenuCommonSpritesAssetHandle), 5, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16) (arg0->x + 0x40), (s16) (arg0->y + 0x40), getMemoryBlockBase(gMenuCommonSpritesAssetHandle), 6, 0x20, 0x20, 0, 0);
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
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, func_80029598, arg0);
}

void func_800297D8(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = 8;
    setCallbackTaskCallback(arg0, func_800296D8);
}

// func_8002980C best match: 98.565% (nonmatchings/func_8002980C-6061209858023118177/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/player_count_select_ui/func_8002980C.s")

#ifdef NON_MATCHING
void func_8002980C(PlayerCountSelectWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        func_80011264((s16)(arg0->x + ((i & 3) << 5)), (s16)(arg0->y + ((i / 4) << 5)),
                      getMemoryBlockBase(D_80112130.frameTextureHandle),
                      D_800B70F0.frames[(u16)arg0->widget.counter].center[tileOffset], 0, 0x100);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        func_80011264((s16)(arg0->x + 0x80), (s16)(arg0->y + offset),
                      getMemoryBlockBase(D_80112130.frameTextureHandle),
                      D_800B70F0.frames[(u16)arg0->widget.counter].right[tileOffset], 0, 0x100);
        i = 0x80;
        func_80011264((s16)(arg0->x + offset), (s16)(arg0->y + 0x80),
                      getMemoryBlockBase(D_80112130.frameTextureHandle),
                      D_800B70F0.frames[(u16)arg0->widget.counter].bottom[tileOffset], 0, 0x100);
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    func_80011264((s16)(arg0->x + 0x80), (s16)(arg0->y + 0x80), getMemoryBlockBase(D_80112130.frameTextureHandle),
                  D_800B70F0.frames[(u16)arg0->widget.counter].corner, 0, 0x100);

    drawMenuSprite((s16)(arg0->x - 4), (s16)(arg0->y - 4), getMemoryBlockBase(D_80112130.textureHandle), 0x33, 0x20,
                  0x20, 0, 0);
    drawMenuSprite((s16)(arg0->x - 4), (s16)(arg0->y + 0x8C), getMemoryBlockBase(D_80112130.textureHandle), 0x38, 0x20,
                  0x20, 0, 0);
    drawMenuSprite((s16)(arg0->x + 0x8C), (s16)(arg0->y - 4), getMemoryBlockBase(D_80112130.textureHandle), 0x35, 0x20,
                  0x20, 0, 0);
    if (offset && offset) {
    }
    drawMenuSprite((s16)(arg0->x + 0x8C), (s16)(arg0->y + 0x8C), getMemoryBlockBase(D_80112130.textureHandle), 0x3A,
                  0x20, 0x20, 0, 0);

    for (offset = 0; (offset ^ 0) != 0x80; offset += 0x10) {
        drawMenuSprite((s16)((arg0->x + offset) + 0xC), (s16)(arg0->y - 4), getMemoryBlockBase(D_80112130.textureHandle),
                      0x34, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)((arg0->x + offset) + 0xC), (s16)(arg0->y + 0x8C), getMemoryBlockBase(D_80112130.textureHandle),
                      0x39, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)(arg0->x - 4), (s16)((arg0->y + offset) + 0xC), getMemoryBlockBase(D_80112130.textureHandle),
                      0x36, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)(arg0->x + 0x8C), (s16)((arg0->y + offset) + 0xC), getMemoryBlockBase(D_80112130.textureHandle),
                      0x37, 0x20, 0x20, 0, 0);
    }
}
#endif

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
            createCallbackTask(func_800297D8, 0, 0x63);
        }
        arg0->row.bytes.subTimer++;
        if (arg0->x < -7) {
            arg0->x = -8;
            arg0->row.bytes.subState = 3;
            gActiveMenuTask = (s32) createCallbackTask(func_8002AFB8, 0, 0x64);
            createCallbackTask(func_8002B15C, 0, 0x64);
            createCallbackTask(func_8002B2FC, 0, 0x64);
            createCallbackTask(func_8002B524, 0, 0x64);
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
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8002980C, arg0);
}

void func_80029FB8(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = 0x94;
    arg0->y = -0x48;
    arg0->sprite.spriteIndex = -8;
    arg0->transition.alpha = -0x74;
    arg0->widget.counter = 0;
    arg0->row.bytes.subTimer = 0;
    arg0->row.bytes.subState = 0;
    setCallbackTaskCallback(arg0, func_80029CE4);
}

void func_8002A008(PlayerCountSelectWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        func_800112F4((s16)(arg0->x + ((i & 3) << 5)), (s16)(arg0->y + ((i / 4) << 5)),
                      getMemoryBlockBase(D_80112130.frameTextureHandle),
                      D_800B70F0.frames[(u16)arg0->sprite.spriteIndex].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        func_800112F4((s16)(arg0->x + 0x80), (s16)(arg0->y + offset), getMemoryBlockBase(D_80112130.frameTextureHandle),
                      D_800B70F0.frames[(u16)arg0->sprite.spriteIndex].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        func_800112F4((s16)(arg0->x + offset), (s16)(arg0->y + 0x80), getMemoryBlockBase(D_80112130.frameTextureHandle),
                      D_800B70F0.frames[(u16)arg0->sprite.spriteIndex].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    func_800112F4((s16)(arg0->x + 0x80), (s16)(arg0->y + 0x80), getMemoryBlockBase(D_80112130.frameTextureHandle),
                  D_800B70F0.frames[(u16)arg0->sprite.spriteIndex].corner, 0, 0x100, 0xA0, 0x49);
}

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
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8002A008, arg0);
}

void func_8002A458(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.spriteIndex = 1;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, func_8002A27C);
}

void func_8002A49C(PlayerCountSelectWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        func_800112F4(arg0->x + ((i & 3) << 5), arg0->y + ((i / 4) << 5),
                      getMemoryBlockBase(D_80112130.frameTextureHandle),
                      D_800B70F0.entries[(u16)arg0->sprite.spriteIndex].center[tileOffset], 0, 0x100, 0xA0,
                      0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        func_800112F4(arg0->x + 0x80, arg0->y + offset,
                      getMemoryBlockBase(D_80112130.frameTextureHandle),
                      D_800B70F0.entries[(u16)arg0->sprite.spriteIndex].right[tileOffset], 0, 0x100, 0xA0,
                      0x49);
        func_800112F4(arg0->x + offset, arg0->y + 0x80,
                      getMemoryBlockBase(D_80112130.frameTextureHandle),
                      D_800B70F0.entries[(u16)arg0->sprite.spriteIndex].bottom[tileOffset], 0, 0x100, 0xA0,
                      0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    func_800112F4(arg0->x + 0x80, arg0->y + 0x80,
                  getMemoryBlockBase(D_80112130.frameTextureHandle),
                  D_800B70F0.entries[(u16)arg0->sprite.spriteIndex].corner, 0, 0x100, 0xA0, 0x49);
}

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
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8002A49C, arg0);
}

void func_8002A8EC(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.spriteIndex = 2;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, func_8002A710);
}

void func_8002A930(PlayerCountSelectWidgetActor *arg0) {
    PlayerCountSelectPaddedFrameTileMapTable *tileMap;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileMap = (PlayerCountSelectPaddedFrameTileMapTable *)&D_800B70F0;
    tileOffset = 0;
    for (i = 0; i < 16; i++, tileOffset++) {
        func_800112F4(arg0->x + ((i & 3) << 5), arg0->y + ((i / 4) << 5), getMemoryBlockBase(D_80112130.textureHandle),
                      tileMap->selected.center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    tileMap = (PlayerCountSelectPaddedFrameTileMapTable *)&D_800B70F0;
    tileOffset = 0;
    offset = 0; i = 0x80; do {
        func_800112F4(arg0->x + 0x80, arg0->y + offset, getMemoryBlockBase(D_80112130.textureHandle),
                      tileMap->selected.right[tileOffset], 0, 0x100, 0xA0, 0x49);
        func_800112F4(arg0->x + offset, arg0->y + 0x80, getMemoryBlockBase(D_80112130.textureHandle),
                      tileMap->selected.bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    func_800112F4(arg0->x + 0x80, arg0->y + 0x80, getMemoryBlockBase(D_80112130.textureHandle),
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
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8002A930, arg0);
}

void func_8002AD74(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.spriteIndex = 2;
    arg0->widget.bytes.timer = 0;
    arg0->widget.bytes.state = 0;
    setCallbackTaskCallback(arg0, func_8002AB24);
}

void func_8002ADB8(PlayerCountSelectWidgetActor *arg0) {
    drawMenuSpriteWithAlpha(arg0->x, (s16)(arg0->y + (D_80121B5E * 0x18)), getMemoryBlockBase(gMenuCommonSpritesAssetHandle), 7, 0x20, 0x20, 0, arg0->sprite.spriteIndex, 0);
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
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8002ADB8, arg0);
}

void func_8002AFB8(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = -0x7C;
    arg0->y = -0x58;
    arg0->sprite.spriteIndex = 0;
    arg0->transition.bytes.state = 0;
    arg0->transition.bytes.timer = 0;
    setCallbackTaskCallback(arg0, func_8002AE3C);
}

void func_8002AFF8(PlayerCountSelectWidgetActor *arg0) {
    PlayerCountPortrait *portrait = &D_800B7198[D_80121B5E];

    drawMenuGlyphScript(arg0->x, arg0->y, *portrait, 1, arg0->sprite.spriteIndex, 0);
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
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8002AFF8, arg0);
}

void func_8002B15C(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = 0xC;
    arg0->sprite.spriteIndex = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, func_8002B05C);
}

void func_8002B198(PlayerCountSelectWidgetActor *arg0) {
    drawMenuSpriteWithAlpha(arg0->x, arg0->y, getMemoryBlockBase(gMenuCommonSpritesAssetHandle), 2, 0x20, 0x20, 0, arg0->sprite.spriteIndex, 0);
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
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8002B198, arg0);
}

void func_8002B2FC(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x5C;
    arg0->sprite.spriteIndex = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, func_8002B1FC);
}

void func_8002B338(PlayerCountSelectWidgetActor *arg0) {
    char sp40[0x18];

    if (D_80121B5E != 3) {
        func_8001BA2C(arg0->x, arg0->y, 0x5000, 0x4000);
        drawMenuSpriteWithAlpha((s16)(arg0->x + 8), (s16)(arg0->y + 4), getMemoryBlockBase(gMenuCommonSpritesAssetHandle), 0x11, 0x20, 0x20, 0, arg0->sprite.spriteIndex, 0);
        sprintf(sp40, D_800E0EA0, D_80121D8C);
        drawMenuAsciiText((s16)(arg0->x + 0x10), (s16)(arg0->y + 0x10), sp40, 0, arg0->sprite.spriteIndex);
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
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8002B338, arg0);
}

void func_8002B524(PlayerCountSelectWidgetActor *arg0) {
    arg0->x = 0x30;
    arg0->y = 0x40;
    arg0->sprite.spriteIndex = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, func_8002B424);
}
