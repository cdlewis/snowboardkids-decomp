#include "common.h"
#include "memory_allocator.h"
#include "fixed_point_matrix.h"

#define FONT_GFX_CMD(pkt, cmd0, cmd1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (cmd0); \
    _g->words.w1 = (cmd1); \
}

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 entryCount;
    /* 0x8 */ char entriesStart[1];
} AssetTable;

typedef struct {
    /* 0x0 */ s32 imageOffset;
    /* 0x4 */ u16 textureIndex;
    /* 0x6 */ u8 width;
    /* 0x7 */ u8 height;
} AssetTableEntry;

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 imageOffset;
    /* 0x0C */ u8 padC[2];
    /* 0x0E */ u8 width;
    /* 0x0F */ u8 height;
} FontTexture;

typedef struct RenderCallbackNode {
    struct RenderCallbackNode *next;
    void (*callback)(s32);
    s32 arg;
} RenderCallbackNode;

typedef struct {
    s32 words[16];
} GfxCommandBlock;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} GfxCommandTriple;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ char pad12[2];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
} GfxCommandSource;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
} GfxCommandDest;

extern u8 D_800EC9C2;
extern s32 D_801121F8;
extern s32 D_80112200;
extern s32 D_801122A8;
extern s32 D_801122B0;
extern s32 D_80112358;
extern s32 D_80112360;
extern s32 D_80112408;
extern s32 D_80112410;
extern s16 D_80112130;
extern s16 D_8011213E;
extern s16 D_801121B0;
extern s16 D_8015660A;
extern s16 D_8015660C;
extern s16 D_8015660E;
extern s16 D_80156610;
extern s32 gRegionAllocPtr;
extern s8 D_801124B0;
extern s8 D_80112560;
extern s8 D_80112610;
extern s8 D_801126C0;
extern void *D_801121B4;

// func_800458E0 best match: 97.917%
#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_800458E0.s")

#ifdef NON_MATCHING
void func_800458E0(void)
{
    s16 **new_var;
    s16 *end;
    s16 *handle;
    s16 *new_var2;

    end = &D_801121B0;
    do {
        ;
        if (1) {
            handle = &D_80112130;
        }
        end = (new_var2 = &D_801121B0);
        do {
            *handle++ = -1;
            new_var2 = &D_801121B0;
            *handle++ = -((0, 1));
            *handle++ = -1;
            if (!end) {
            }
dummy_label_220868:
            ;
            *handle++ = -1;
            end = new_var2;
        } while (*(&handle) != end);
    } while (0 != 0);
}
#endif

void func_80045914(void)
{
 do { s16 *handle = &D_8011213E; do { if ((*handle) != (-1)) { *handle = func_80042EE4(*handle); } handle++; } while (handle != (&D_801121B0)); } while (0);
}

void *func_8004597C(void *arg0, u32 arg1) {
    return (void *)((u8 *)arg0 + (arg1 & 0xFFFFFF));
}

void func_80045990(u8 *arg0, u16 arg1, void **arg2, void **arg3) {
    AssetTableEntry *temp_v1;
    u8 *temp_v0;
    short idx;

    temp_v1 = (AssetTableEntry *)((s32)arg0 + (arg1 * sizeof(AssetTableEntry)));
    temp_v0 = arg0 + (((AssetTable *)arg0)->entryCount * sizeof(AssetTableEntry));
    idx = 1;
    *arg2 = (void *)(arg0 + temp_v1[idx].imageOffset);
    temp_v0 += 8;
    *arg3 = (void *)((temp_v1[idx].textureIndex << 5) + temp_v0);
}

void func_800459D4(u8 *arg0, u16 arg1, u16 arg2, void **arg3, void **arg4) {
    u8 *temp_v0;
    AssetTableEntry *temp_v1;

    temp_v1 = (AssetTableEntry *)((s32)arg0 + (arg1 * sizeof(AssetTableEntry)));
    temp_v0 = arg0 + (((AssetTable *)arg0)->entryCount * sizeof(AssetTableEntry));
    *arg3 = (void *)(arg0 + temp_v1[1].imageOffset);
    temp_v0 += 8;
    *arg4 = (void *)((arg2 << 5) + temp_v0);
}

void func_80045A1C(u8 *arg0, u16 arg1, void **arg2, void **arg3, s16 *arg4, s16 *arg5) {
    AssetTableEntry *temp_v1;
    u8 *temp_v0;
    short idx;

    temp_v1 = (AssetTableEntry *)((s32)arg0 + (arg1 * sizeof(AssetTableEntry)));
    temp_v0 = arg0 + (((AssetTable *)arg0)->entryCount * sizeof(AssetTableEntry));
    idx = 1;
    *arg2 = (void *)(arg0 + temp_v1[idx].imageOffset);
    temp_v0 += 8;
    *arg3 = (void *)((temp_v1[idx].textureIndex << 5) + temp_v0);
    *arg4 = temp_v1[idx].width;
    *arg5 = temp_v1[idx].height;
}

// func_80045A78 best match: 96.718% (nonmatchings/func_80045A78-8207005055717715604/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80045A78.s")

#ifdef NON_MATCHING
void func_80045A78(s16 arg0, s16 arg1, AssetTable *arg2, u16 arg3) {
    volatile char pad[0x18];
    u8 *textureBase;
    s32 clipU;
    s32 clipV;
    s32 rightClip;
    s32 bottomClip;
    s32 leftClip;
    s32 topClip;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    AssetTableEntry *rawEntry;
    AssetTableEntry *entry;

    textureBase = (((u8 *)arg2) + (arg2->entryCount * sizeof(AssetTableEntry))) + sizeof(AssetTableEntry);
    rawEntry = (AssetTableEntry *)(((u8 *)arg2) + (arg3 * sizeof(AssetTableEntry)));
    x0 = arg0 + D_8015660E;
    y0 = arg1 + D_80156610;
    ;
    clipU = 0;
    clipV = 0;
    x1 = rawEntry[1].width + x0;
    y1 = rawEntry[1].height + y0;
    rightClip = D_8015660E + (D_8015660A / 2);
    if (x0 < rightClip) {
        leftClip = D_8015660E - (D_8015660A / 2);
        bottomClip = D_80156610 + (D_8015660C / 2);
        if (y0 < bottomClip) {
            topClip = D_80156610 - (D_8015660C / 2);
            if ((x1 >= leftClip) && (y1 >= topClip)) {
                if (x0 < leftClip) {
                    clipU = leftClip - x0;
                    x0 = leftClip;
                }
                if (y0 < topClip) {
                    clipV = topClip - y0;
                    y0 = topClip;
                }
                if (x1 >= rightClip) {
                    x1 = rightClip;
                }
                if (y1 >= bottomClip) {
                    y1 = bottomClip;
                }
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = ((((rawEntry + 1)->width >> 1) - 1) & 0xFFF) | 0xFD480000;
                    _g->words.w1 = (u32)(((u8 *)arg2) + (rawEntry + 1)->imageOffset);
                }
                ;
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = (((((((rawEntry + 1)->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5480000;
                    _g->words.w1 = 0x07080200;
                }
                ;
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xE6000000;
                    _g->words.w1 = 0;
                }
                ;
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xF4000000;
                    _g->words.w1 = (((((rawEntry + 1)->width * 2) & 0xFFF) << 12) | 0x07000000) | (((rawEntry + 1)->height * 4) & 0xFFF);
                }
                ;
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xE7000000;
                    _g->words.w1 = 0;
                }
                ;
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = (((((((rawEntry + 1)->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5400000;
                    _g->words.w1 = 0x00080200;
                }
                ;
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xF2000000;
                    _g->words.w1 = ((((rawEntry + 1)->width * 4) & 0xFFF) << 12) | (((rawEntry + 1)->height * 4) & 0xFFF);
                }
                ;
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xFD100000;
                    _g->words.w1 = (u32)(textureBase + ((rawEntry + 1)->textureIndex << 5));
                }
                ;
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xE8000000;
                    _g->words.w1 = 0;
                }
                ;
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xF5000100;
                    _g->words.w1 = 0x07000000;
                }
                ;
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xE6000000;
                    _g->words.w1 = 0;
                }
                ;
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xF0000000;
                    _g->words.w1 = 0x0703C000;
                }
                ;
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xE7000000;
                    _g->words.w1 = 0;
                }
                ;
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = ((((x1 * 4) & 0xFFF) << 12) | 0xE4000000) | ((y1 * 4) & 0xFFF);
                    _g->words.w1 = (((x0 * 4) & 0xFFF) << 12) | ((y0 * 4) & 0xFFF);
                }
                ;
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xB4000000;
                    leftClip = clipU;
                    _g->words.w1 = (leftClip << 21) | ((clipV << 5) & 0xFFFF);
                }
                ;
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xB3000000;
                    _g->words.w1 = 0x04000400;
                }
                ;
            }
        }
    }
}
#endif

// func_80045E84 best match: 76.854% (nonmatchings/func_80045E84-2225551288923588688/base.c)
#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80045E84.s")

#ifdef NON_MATCHING
extern s32 gRegionAllocPtr;
extern u32 D_800DEFF8[];
extern s16 D_801235B0;
extern s16 D_8015660A;
extern s16 D_8015660C;
extern s16 D_8015660E;
extern s16 D_80156610;

void func_80045E84(s16 arg0, s16 arg1, AssetTable *arg2, s32 arg3) {
    volatile u8 pad[0x18];
    u8 *volatile sp24;
    volatile s32 sp18;
    volatile s32 sp14;
    volatile s32 sp10;
    volatile s32 spC;
    s32 temp_a0;
    s32 temp_a3;
    s32 temp_t1;
    s32 temp_t2;
    s32 temp_t6;
    s32 temp_t7;
    s32 temp_t8;
    s32 temp_t8_2;
    Gfx *temp_gfx;
    s32 var_a0;
    s32 var_a0_2;
    register s32 var_s0;
    s32 var_t5;
    AssetTableEntry *temp_t4;
    AssetTableEntry *temp_t4_2;

    temp_t4 = (AssetTableEntry *)((u8 *)arg2 + ((arg3 & 0xFFFF) * sizeof(AssetTableEntry)));
    sp24 = (u8 *)arg2 + (arg2->entryCount * sizeof(AssetTableEntry)) + sizeof(AssetTableEntry);
    var_t5 = arg0 + D_8015660E;
    var_s0 = arg1 + D_80156610;
    temp_t4_2 = temp_t4 + 1;
    sp14 = temp_t4_2->height + var_s0;
    sp10 = 0;
    spC = 0;
    temp_t2 = temp_t4_2->width + var_t5;
    temp_t7 = D_8015660A / 2;
    temp_t1 = D_8015660E + temp_t7;
    if (var_t5 < temp_t1) {
        temp_a3 = D_8015660E - temp_t7;
        temp_t8 = D_8015660C / 2;
        temp_a0 = D_80156610 + temp_t8;
        if (var_s0 < temp_a0) {
            sp18 = temp_t2;
            if (temp_t2 >= temp_a3) {
                s32 temp_v0 = D_80156610 - temp_t8;
                if (sp14 >= temp_v0) {
                    if (var_t5 < temp_a3) {
                        sp10 = temp_a3 - var_t5;
                        var_t5 = temp_a3;
                    }
                    if (var_s0 < temp_v0) {
                        spC = temp_v0 - var_s0;
                        var_s0 = temp_v0;
                    }
                    if (sp18 >= temp_t1) {
                        sp18 = temp_t1;
                    }
                    if (sp14 >= temp_a0) {
                        sp14 = temp_a0;
                    }
                    temp_t8_2 = D_801235B0 & 0x1F;
                    var_a0 = temp_t8_2;
                    if (temp_t8_2 >= 0x11) {
                        var_a0 = 0x20 - temp_t8_2;
                    }
                    temp_t6 = var_a0 * 0x10;
                    var_a0_2 = temp_t6;
                    if (temp_t6 >= 0x100) {
                        var_a0_2 = 0xFF;
                    }
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w1 = 0;
                    temp_gfx->words.w0 = 0xE7000000;
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w1 = 0xFF2FFFFF;
                    temp_gfx->words.w0 = 0xFC119623;
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w0 = 0xFA000000;
                    temp_gfx->words.w1 = (((var_a0_2 & 0xFF) << 8) | 0xFFFF0000 | 0xFF);
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w0 = ((((s32)temp_t4_2->width >> 1) - 1) & 0xFFF) | 0xFD480000;
                    temp_gfx->words.w1 = (u32)((u8 *)arg2 + temp_t4_2->imageOffset);
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w0 = (((((s32)(((s32)(temp_t4_2->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5480000);
                    temp_gfx->words.w1 = 0x07080200;
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w1 = 0;
                    temp_gfx->words.w0 = 0xE6000000;
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w0 = 0xF4000000;
                    temp_gfx->words.w1 = (((temp_t4_2->width * 2) & 0xFFF) << 12) | 0x07000000 | ((temp_t4_2->height * 4) & 0xFFF);
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w1 = 0;
                    temp_gfx->words.w0 = 0xE7000000;
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w0 = (((((s32)(((s32)(temp_t4_2->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5400000);
                    temp_gfx->words.w1 = 0x00080200;
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w0 = 0xF2000000;
                    temp_gfx->words.w1 = (((temp_t4_2->width * 4) & 0xFFF) << 12) | ((temp_t4_2->height * 4) & 0xFFF);
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w0 = 0xFD100000;
                    temp_gfx->words.w1 = (u32)((temp_t4_2->textureIndex << 5) + sp24);
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w1 = 0;
                    temp_gfx->words.w0 = 0xE8000000;
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w1 = 0x07000000;
                    temp_gfx->words.w0 = 0xF5000100;
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w1 = 0;
                    temp_gfx->words.w0 = 0xE6000000;
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w0 = 0xF0000000;
                    temp_gfx->words.w1 = 0x0703C000;
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w1 = 0;
                    temp_gfx->words.w0 = 0xE7000000;
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w0 = (((sp18 * 4) & 0xFFF) << 12) | 0xE4000000 | ((sp14 * 4) & 0xFFF);
                    temp_gfx->words.w1 = (((var_t5 * 4) & 0xFFF) << 12) | ((var_s0 * 4) & 0xFFF);
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w0 = 0xB4000000;
                    temp_gfx->words.w1 = (sp10 << 21) | ((spC << 5) & 0xFFFF);
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w0 = 0xB3000000;
                    temp_gfx->words.w1 = 0x04000400;
                    temp_gfx = (Gfx *)gRegionAllocPtr;
                    gRegionAllocPtr = (s32)(temp_gfx + 1);
                    temp_gfx->words.w0 = 0x06000000;
                    temp_gfx->words.w1 = (u32)D_800DEFF8;
                }
            }
        }
    }
}
#endif

// func_80046358 best match: 88.099% (nonmatchings/func_80046358-8207005055717715604/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80046358.s")

#ifdef NON_MATCHING
extern Gfx *gRegionAllocPtr;
extern u16 D_800D40B0[];

void func_80046358(s16 arg0, s16 arg1, u8 *arg2, u16 arg3) {
    volatile char pad[0x20];
    s32 clipU;
    s32 clipV;
    s32 rightClip;
    s32 bottomClip;
    s32 leftClip;
    s32 topClip;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    AssetTableEntry *rawEntry;

    rawEntry = (AssetTableEntry *)(arg2 + (arg3 * sizeof(AssetTableEntry)));
    x0 = arg0 + D_8015660E;
    y0 = arg1 + D_80156610;
    clipU = 0;
    clipV = 0;
    x1 = rawEntry[1].width + x0;
    y1 = rawEntry[1].height + y0;
    rightClip = D_8015660E + (D_8015660A / 2);
    if (x0 < rightClip) {
        leftClip = D_8015660E - (D_8015660A / 2);
        bottomClip = D_80156610 + (D_8015660C / 2);
        if (y0 < bottomClip) {
            topClip = D_80156610 - (D_8015660C / 2);
            if ((x1 >= leftClip) && (y1 >= topClip)) {
                if (x0 < leftClip) {
                    clipU = leftClip - x0;
                    x0 = leftClip;
                }
                if (y0 < topClip) {
                    clipV = topClip - y0;
                    y0 = topClip;
                }
                if (x1 >= rightClip) {
                    x1 = rightClip;
                }
                if (y1 >= bottomClip) {
                    y1 = bottomClip;
                }
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = ((((rawEntry + 1)->width >> 1) - 1) & 0xFFF) | 0xFD480000;
                    _g->words.w1 = (u32)(arg2 + (rawEntry + 1)->imageOffset);
                }
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = (((((((rawEntry + 1)->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5480000;
                    _g->words.w1 = 0x07080200;
                }
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xE6000000;
                    _g->words.w1 = 0;
                }
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xF4000000;
                    _g->words.w1 = (((((rawEntry + 1)->width * 2) & 0xFFF) << 12) | 0x07000000) | (((rawEntry + 1)->height * 4) & 0xFFF);
                }
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xE7000000;
                    _g->words.w1 = 0;
                }
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = (((((((rawEntry + 1)->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5400000;
                    _g->words.w1 = 0x00080200;
                }
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xF2000000;
                    _g->words.w1 = ((((rawEntry + 1)->width * 4) & 0xFFF) << 12) | (((rawEntry + 1)->height * 4) & 0xFFF);
                }
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xFD100000;
                    _g->words.w1 = (u32)D_800D40B0;
                }
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xE8000000;
                    _g->words.w1 = 0;
                }
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xF5000100;
                    _g->words.w1 = 0x07000000;
                }
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xE6000000;
                    _g->words.w1 = 0;
                }
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xF0000000;
                    _g->words.w1 = 0x0703C000;
                }
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xE7000000;
                    _g->words.w1 = 0;
                }
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = ((((x1 * 4) & 0xFFF) << 12) | 0xE4000000) | ((y1 * 4) & 0xFFF);
                    _g->words.w1 = (((x0 * 4) & 0xFFF) << 12) | ((y0 * 4) & 0xFFF);
                }
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xB4000000;
                    leftClip = clipU;
                    _g->words.w1 = (leftClip << 21) | ((clipV << 5) & 0xFFFF);
                }
                {
                    Gfx *_g = (Gfx *)(gRegionAllocPtr++);
                    _g->words.w0 = 0xB3000000;
                    _g->words.w1 = 0x04000400;
                }
            }
        }
    }
}
#endif

// func_80046748 best match: 89.835% (nonmatchings/func_80046748-8207005055717715604/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80046748.s")

#ifdef NON_MATCHING
extern Gfx D_800DEFF8[];

void func_80046748(s16 x, s16 y, s16 width, s16 height, u8 red, u8 green, u8 blue) {
    s32 rightClip;
    s32 bottomClip;
    s32 leftClip;
    s32 topClip;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 halfWidth;
    s32 halfHeight;
    s32 color;

    x0 = x + D_8015660E;
    y0 = y + D_80156610;
    x1 = width + x0;
    y1 = height + y0;

    halfWidth = D_8015660A / 2;
    rightClip = D_8015660E + halfWidth;
    if (x0 < rightClip) {
        halfHeight = D_8015660C / 2;
        bottomClip = D_80156610 + halfHeight;
        leftClip = D_8015660E - halfWidth;
        if (y0 < bottomClip) {
            topClip = D_80156610 - halfHeight;
            if ((x1 >= leftClip) && (y1 >= topClip)) {
                if (x0 < leftClip) {
                    x0 = leftClip;
                }
                if (y0 < topClip) {
                    y0 = topClip;
                }
                if (x1 >= rightClip) {
                    x1 = rightClip;
                }
                if (y1 >= bottomClip) {
                    y1 = bottomClip;
                }

                color = ((red << 8) & 0xF800) | ((green << 3) & 0x7C0) | ((blue >> 2) & 0x3E) | 1;

                gDPPipeSync(gRegionAllocPtr++);
                gDPSetCycleType(gRegionAllocPtr++, G_CYC_FILL);
                gDPSetRenderMode(gRegionAllocPtr++, G_RM_NOOP, G_RM_NOOP2);
                gDPSetFillColor(gRegionAllocPtr++, (color << 16) | color);
                gDPFillRectangle(gRegionAllocPtr++, x0, y0, x1 - 1, y1 - 1);
                gSPDisplayList(gRegionAllocPtr++, D_800DEFF8);
            }
        }
    }
}
#endif

// func_80046970 best match: 74.764% (nonmatchings/func_80046970-4923837976568703863/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80046970.s")

#ifdef NON_MATCHING
void func_80046970(s16 arg0, s16 arg1, AssetTable *arg2, s32 arg3) {
    u8 *sp2C;
    s32 sp18;
    s32 sp14;
    s32 temp_a1;
    s32 temp_a3;
    s32 temp_t1;
    s32 temp_t6;
    s32 temp_t7;
    Gfx *temp_v0_2;
    Gfx *temp_v0_3;
    Gfx *temp_v0_4;
    Gfx *temp_v0_5;
    Gfx *temp_v0_6;
    Gfx *temp_v0_7;
    Gfx *temp_v0_8;
    Gfx *temp_v0_9;
    Gfx *temp_v0_10;
    Gfx *temp_v0_11;
    Gfx *temp_v0_12;
    Gfx *temp_v0_13;
    Gfx *temp_v0_14;
    Gfx *temp_v0_15;
    Gfx *temp_v0_16;
    Gfx *temp_v0_17;
    s32 var_at;
    s32 var_s0;
    s32 var_s1;
    s32 var_s2;
    s32 var_t5;
    AssetTableEntry *temp_t2;
    AssetTableEntry *temp_t2_2;

    sp2C = (u8 *)arg2 + (arg2->entryCount * sizeof(AssetTableEntry)) + 8;
    temp_t2 = (AssetTableEntry *)((u8 *)arg2 + ((arg3 & 0xFFFF) * sizeof(AssetTableEntry)));
    var_t5 = arg0 + D_8015660E;
    var_s0 = arg1 + D_80156610;
    temp_t2_2 = temp_t2 + 1;
    sp18 = 0;
    sp14 = 0;
    var_s1 = temp_t2_2->width + var_t5;
    var_s2 = temp_t2_2->height + var_s0;
    temp_t6 = D_8015660A / 2;
    temp_t1 = D_8015660E + temp_t6;
    if (var_t5 < temp_t1) {
        temp_a3 = D_8015660E - temp_t6;
        temp_t7 = D_8015660C / 2;
        temp_a1 = D_80156610 + temp_t7;
        if (var_s0 < temp_a1) {
            temp_v0_2 = (Gfx *)(D_80156610 - temp_t7);
            if ((var_s1 >= temp_a3) && (var_s2 >= (s32)temp_v0_2)) {
                if (var_t5 < temp_a3) {
                    sp18 = temp_a3 - var_t5;
                    var_t5 = temp_a3;
                }
                if (var_s0 < (s32)temp_v0_2) {
                    sp14 = (s32)temp_v0_2 - var_s0;
                    var_s0 = (s32)temp_v0_2;
                }
                var_at = var_s2 < temp_a1;
                if (var_s1 >= temp_t1) {
                    var_s1 = temp_t1;
                    var_at = var_s2 < temp_a1;
                }
                if (var_at == 0) {
                    var_s2 = temp_a1;
                }
                temp_v0_2 = (Gfx *)gRegionAllocPtr;
                gRegionAllocPtr = (s32)(temp_v0_2 + 1);
                temp_v0_2->words.w0 = ((temp_t2_2->width - 1) & 0xFFF) | 0xFD480000;
                temp_v0_2->words.w1 = temp_t2_2->imageOffset + (s32)arg2;
                temp_v0_3 = (Gfx *)gRegionAllocPtr;
                gRegionAllocPtr = (s32)(temp_v0_3 + 1);
                temp_v0_3->words.w0 = ((((s32)(temp_t2_2->width + 8) >> 3) & 0x1FF) << 9) | 0xF5480000;
                temp_v0_3->words.w1 = 0x07080200;
                temp_v0_4 = (Gfx *)gRegionAllocPtr;
                gRegionAllocPtr = (s32)(temp_v0_4 + 1);
                temp_v0_4->words.w1 = 0;
                temp_v0_4->words.w0 = 0xE6000000;
                temp_v0_5 = (Gfx *)gRegionAllocPtr;
                gRegionAllocPtr = (s32)(temp_v0_5 + 1);
                temp_v0_5->words.w0 = 0xF4000000;
                temp_v0_5->words.w1 = (((temp_t2_2->width * 4) & 0xFFF) << 12) | 0x07000000 | ((temp_t2_2->height * 4) & 0xFFF);
                temp_v0_6 = (Gfx *)gRegionAllocPtr;
                gRegionAllocPtr = (s32)(temp_v0_6 + 1);
                temp_v0_6->words.w1 = 0;
                temp_v0_6->words.w0 = 0xE7000000;
                temp_v0_7 = (Gfx *)gRegionAllocPtr;
                gRegionAllocPtr = (s32)(temp_v0_7 + 1);
                temp_v0_7->words.w0 = ((((s32)(temp_t2_2->width + 8) >> 3) & 0x1FF) << 9) | 0xF5480000;
                temp_v0_7->words.w1 = 0x80200;
                temp_v0_8 = (Gfx *)gRegionAllocPtr;
                gRegionAllocPtr = (s32)(temp_v0_8 + 1);
                temp_v0_8->words.w0 = 0xF2000000;
                temp_v0_8->words.w1 = (((temp_t2_2->width * 4) & 0xFFF) << 12) | ((temp_t2_2->height * 4) & 0xFFF);
                temp_v0_9 = (Gfx *)gRegionAllocPtr;
                gRegionAllocPtr = (s32)(temp_v0_9 + 1);
                temp_v0_9->words.w0 = 0xFD100000;
                temp_v0_9->words.w1 = (temp_t2_2->textureIndex << 5) + (s32)sp2C;
                temp_v0_10 = (Gfx *)gRegionAllocPtr;
                gRegionAllocPtr = (s32)(temp_v0_10 + 1);
                temp_v0_10->words.w1 = 0;
                temp_v0_10->words.w0 = 0xE8000000;
                temp_v0_11 = (Gfx *)gRegionAllocPtr;
                gRegionAllocPtr = (s32)(temp_v0_11 + 1);
                temp_v0_11->words.w1 = 0x07000000;
                temp_v0_11->words.w0 = 0xF5000100;
                temp_v0_12 = (Gfx *)gRegionAllocPtr;
                gRegionAllocPtr = (s32)(temp_v0_12 + 1);
                temp_v0_12->words.w1 = 0;
                temp_v0_12->words.w0 = 0xE6000000;
                temp_v0_13 = (Gfx *)gRegionAllocPtr;
                gRegionAllocPtr = (s32)(temp_v0_13 + 1);
                temp_v0_13->words.w0 = 0xF0000000;
                temp_v0_13->words.w1 = 0x073FC000;
                temp_v0_14 = (Gfx *)gRegionAllocPtr;
                gRegionAllocPtr = (s32)(temp_v0_14 + 1);
                temp_v0_14->words.w1 = 0;
                temp_v0_14->words.w0 = 0xE7000000;
                temp_v0_15 = (Gfx *)gRegionAllocPtr;
                gRegionAllocPtr = (s32)(temp_v0_15 + 1);
                temp_v0_15->words.w0 = (((var_s1 * 4) & 0xFFF) << 12) | 0xE4000000 | ((var_s2 * 4) & 0xFFF);
                temp_v0_15->words.w1 = (((var_t5 * 4) & 0xFFF) << 12) | ((var_s0 * 4) & 0xFFF);
                temp_v0_16 = (Gfx *)gRegionAllocPtr;
                gRegionAllocPtr = (s32)(temp_v0_16 + 1);
                temp_v0_16->words.w0 = 0xB4000000;
                temp_v0_16->words.w1 = (sp18 << 21) | ((sp14 << 5) & 0xFFFF);
                temp_v0_17 = (Gfx *)gRegionAllocPtr;
                gRegionAllocPtr = (s32)(temp_v0_17 + 1);
                temp_v0_17->words.w0 = 0xB3000000;
                temp_v0_17->words.w1 = 0x04000400;
            }
        }
    }
}
#endif

// func_80046D68 best match: 80.448% (nonmatchings/func_80046D68-4923837976568703863/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80046D68.s")

#ifdef NON_MATCHING
void func_80046D68(s16 arg0, s16 arg1, AssetTable *arg2, s32 arg3, u16 arg4) {
    volatile u8 padding[0x10];
    u8 *sp2C;
    s32 sp18;
    s32 sp14;
    s32 temp_a1;
    s32 temp_a3;
    s32 temp_t1;
    s32 temp_t6;
    s32 temp_t7;
    Gfx *temp_v0_2;
    Gfx *temp_v0_3;
    Gfx *temp_v0_4;
    Gfx *temp_v0_5;
    Gfx *temp_v0_6;
    Gfx *temp_v0_7;
    Gfx *temp_v0_8;
    Gfx *temp_v0_9;
    Gfx *temp_v0_10;
    Gfx *temp_v0_11;
    Gfx *temp_v0_12;
    Gfx *temp_v0_13;
    Gfx *temp_v0_14;
    Gfx *temp_v0_15;
    Gfx *temp_v0_16;
    Gfx *temp_v0_17;
    s32 var_at;
    s32 var_s0;
    s32 var_s1;
    s32 var_s2;
    s32 var_t5;
    AssetTableEntry *temp_t2;
    AssetTableEntry *temp_t2_2;

    sp2C = ((u8 *)arg2 + (arg2->entryCount * sizeof(AssetTableEntry))) + 8;
    temp_t2 = (AssetTableEntry *)((u8 *)arg2 + ((arg3 & 0xFFFF) * sizeof(AssetTableEntry)));
    var_t5 = arg0 + D_8015660E;
    var_s0 = arg1 + D_80156610;
    sp18 = 0;
    sp14 = 0;
    var_s1 = temp_t2[1].width + var_t5;
    var_s2 = temp_t2[1].height + var_s0;
    temp_t2_2 = temp_t2 + 1;
    temp_t6 = D_8015660A / 2;
    temp_t1 = D_8015660E + temp_t6;
    if (var_t5 < temp_t1) {
        temp_a3 = D_8015660E - temp_t6;
        temp_t7 = D_8015660C / 2;
        temp_a1 = D_80156610 + temp_t7;
        if (var_s0 < temp_a1) {
            temp_v0_2 = (Gfx *)(D_80156610 - temp_t7);
            if ((var_s1 >= temp_a3) && (var_s2 >= (s32)temp_v0_2)) {
                if (var_t5 < temp_a3) {
                    sp18 = temp_a3 - var_t5;
                    var_t5 = temp_a3;
                }
                if (var_s0 < (s32)temp_v0_2) {
                    sp14 = (s32)temp_v0_2 - var_s0;
                    var_s0 = (s32)temp_v0_2;
                }
                var_at = var_s2 < temp_a1;
                if (var_s1 >= temp_t1) {
                    var_s1 = temp_t1;
                    var_at = var_s2 < temp_a1;
                }
                if (var_at == 0) {
                    var_s2 = temp_a1;
                }
                temp_v0_2 = gRegionAllocPtr++;
                temp_v0_2->words.w0 = (((temp_t2_2->width >> 1) - 1) & 0xFFF) | 0xFD480000;
                temp_v0_2->words.w1 = temp_t2_2->imageOffset + (s32)arg2;
                temp_v0_3 = gRegionAllocPtr++;
                temp_v0_3->words.w0 = (((((temp_t2_2->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9 | 0xF5480000;
                temp_v0_3->words.w1 = 0x07080200;
                temp_v0_4 = gRegionAllocPtr++;
                temp_v0_4->words.w1 = 0;
                temp_v0_4->words.w0 = 0xE6000000;
                temp_v0_5 = gRegionAllocPtr++;
                temp_v0_5->words.w0 = 0xF4000000;
                temp_v0_5->words.w1 = (((temp_t2_2->width * 2) & 0xFFF) << 12) | 0x07000000 | ((temp_t2_2->height * 4) & 0xFFF);
                temp_v0_6 = gRegionAllocPtr++;
                temp_v0_6->words.w1 = 0;
                temp_v0_6->words.w0 = 0xE7000000;
                temp_v0_7 = gRegionAllocPtr++;
                temp_v0_7->words.w0 = (((((temp_t2_2->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9 | 0xF5400000;
                temp_v0_7->words.w1 = 0x80200;
                temp_v0_8 = gRegionAllocPtr++;
                temp_v0_8->words.w0 = 0xF2000000;
                temp_v0_8->words.w1 = (((temp_t2_2->width * 4) & 0xFFF) << 12) | ((temp_t2_2->height * 4) & 0xFFF);
                temp_v0_9 = gRegionAllocPtr++;
                temp_v0_9->words.w0 = 0xFD100000;
                temp_v0_9->words.w1 = (arg4 << 5) + (s32)sp2C;
                temp_v0_10 = gRegionAllocPtr++;
                temp_v0_10->words.w1 = 0;
                temp_v0_10->words.w0 = 0xE8000000;
                temp_v0_11 = gRegionAllocPtr++;
                temp_v0_11->words.w1 = 0x07000000;
                temp_v0_11->words.w0 = 0xF5000100;
                temp_v0_12 = gRegionAllocPtr++;
                temp_v0_12->words.w1 = 0;
                temp_v0_12->words.w0 = 0xE6000000;
                temp_v0_13 = gRegionAllocPtr++;
                temp_v0_13->words.w0 = 0xF0000000;
                temp_v0_13->words.w1 = 0x0703C000;
                temp_v0_14 = gRegionAllocPtr++;
                temp_v0_14->words.w1 = 0;
                temp_v0_14->words.w0 = 0xE7000000;
                temp_v0_15 = gRegionAllocPtr++;
                temp_v0_15->words.w0 = (((var_s1 * 4) & 0xFFF) << 12) | 0xE4000000 | ((var_s2 * 4) & 0xFFF);
                temp_v0_15->words.w1 = (((var_t5 * 4) & 0xFFF) << 12) | ((var_s0 * 4) & 0xFFF);
                temp_v0_16 = gRegionAllocPtr++;
                temp_v0_16->words.w0 = 0xB4000000;
                temp_v0_16->words.w1 = (sp18 << 21) | ((sp14 << 5) & 0xFFFF);
                temp_v0_17 = gRegionAllocPtr++;
                temp_v0_17->words.w0 = 0xB3000000;
                temp_v0_17->words.w1 = 0x04000400;
            }
        }
    }
}
#endif

// func_80047174 best match: 73.385% (nonmatchings/func_80047174-4923837976568703863/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80047174.s")

#ifdef NON_MATCHING
#define RENDER_EMIT_GFX(cmd0, cmd1)       \
    do {                                  \
        Gfx *gfx = (Gfx *)gRegionAllocPtr; \
        gRegionAllocPtr = (s32)(gfx + 1); \
        gfx->words.w0 = (cmd0);           \
        gfx->words.w1 = (cmd1);           \
    } while (0)

void func_80047174(s16 arg0, s16 arg1, AssetTable *arg2, u16 arg3, u16 arg4)
{
    u8 *paletteBase;
    s32 right;
    s32 bottom;
    s32 s;
    s32 t;
    s32 halfWidth;
    s32 halfHeight;
    s32 clipRight;
    s32 clipBottom;
    s32 clipLeft;
    s32 clipTop;
    s32 scaledWidth;
    s32 scaledHeight;
    s32 x;
    s32 y;
    s32 value;
    AssetTableEntry *entry;

    if ((s32)arg4 >= 0) {
        paletteBase = (u8 *)arg2 + (arg2->entryCount * sizeof(AssetTableEntry)) + 8;
        entry = (AssetTableEntry *)((u8 *)arg2 + (arg3 * sizeof(AssetTableEntry))) + 1;
        scaledWidth = entry->width >> arg4;
        scaledHeight = entry->height >> arg4;
        x = arg0 + D_8015660E + ((entry->width - scaledWidth) / 2);
        y = arg1 + D_80156610 + ((entry->height - scaledHeight) / 2);
        right = scaledWidth + x;
        bottom = scaledHeight + y;
        s = 0;
        t = 0;

        halfWidth = D_8015660A / 2;
        clipRight = D_8015660E + halfWidth;
        if (x < clipRight) {
            halfHeight = D_8015660C / 2;
            clipBottom = D_80156610 + halfHeight;
            if (y < clipBottom) {
                clipLeft = D_8015660E - halfWidth;
                if (right >= clipLeft) {
                    clipTop = D_80156610 - halfHeight;
                    if (bottom >= clipTop) {
                        if (x < clipLeft) {
                            s = clipLeft - x;
                            x = clipLeft;
                        }
                        if (y < clipTop) {
                            t = clipTop - y;
                            y = clipTop;
                        }
                        if (right >= clipRight) {
                            right = clipRight;
                        }
                        if (bottom >= clipBottom) {
                            bottom = clipBottom;
                        }

                        RENDER_EMIT_GFX(0xE7000000, 0);
                        RENDER_EMIT_GFX(0xBA000C02, 0x3000);
                        RENDER_EMIT_GFX((((entry->width >> 1) - 1) & 0xFFF) | 0xFD480000,
                                        entry->imageOffset + (s32)arg2);
                        RENDER_EMIT_GFX(((((entry->width + 1) >> 1) + 7) >> 3 & 0x1FF) << 9 | 0xF5480000,
                                        0x07080200);
                        RENDER_EMIT_GFX(0xE6000000, 0);
                        RENDER_EMIT_GFX(0xF4000000,
                                        (((entry->width * 2) & 0xFFF) << 12) | 0x07000000 |
                                            ((entry->height * 4) & 0xFFF));
                        RENDER_EMIT_GFX(0xE7000000, 0);
                        RENDER_EMIT_GFX(((((entry->width + 1) >> 1) + 7) >> 3 & 0x1FF) << 9 | 0xF5400000,
                                        0x00080200);
                        RENDER_EMIT_GFX(0xF2000000,
                                        (((entry->width * 4) & 0xFFF) << 12) | ((entry->height * 4) & 0xFFF));
                        RENDER_EMIT_GFX(0xFD100000, (entry->textureIndex << 5) + (s32)paletteBase);
                        RENDER_EMIT_GFX(0xE8000000, 0);
                        RENDER_EMIT_GFX(0xF5000100, 0x07000000);
                        RENDER_EMIT_GFX(0xE6000000, 0);
                        RENDER_EMIT_GFX(0xF0000000, 0x0703C000);
                        RENDER_EMIT_GFX(0xE7000000, 0);
                        RENDER_EMIT_GFX((((right * 4) & 0xFFF) << 12) | 0xE4000000 | ((bottom * 4) & 0xFFF),
                                        (((x * 4) & 0xFFF) << 12) | ((y * 4) & 0xFFF));
                        RENDER_EMIT_GFX(0xB4000000,
                                        (((s << 5) + 0x10) << 16) | (((t << 5) + 0x10) & 0xFFFF));
                        value = (1 << (arg4 + 10)) & 0xFFFF;
                        RENDER_EMIT_GFX(0xB3000000, (value << 16) | value);
                        RENDER_EMIT_GFX(0xE7000000, 0);
                        RENDER_EMIT_GFX(0xBA000C02, 0);
                        RENDER_EMIT_GFX(0xE7000000, 0);
                    }
                }
            }
        }
    }
}

#undef RENDER_EMIT_GFX
#endif

// func_8004767C best match: 74.481% (nonmatchings/func_8004767C-7273315160691878794/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_8004767C.s")

#ifdef NON_MATCHING
void func_8004767C(s16 x, s16 y, AssetTable *asset, u16 entryIndex, u16 paletteIndex, u16 scale) {
    u8 *paletteBase;
    AssetTableEntry *entry;
    AssetTableEntry *entry2;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 clipS;
    s32 clipT;
    s32 viewHalfWidth;
    s32 viewHalfHeight;
    s32 maxX;
    s32 maxY;
    s32 minX;
    s32 minY;
    s32 width;
    s32 height;
    s32 drawWidth;
    s32 drawHeight;
    s32 dsdx;
    Gfx *gfx;

    if ((s32)scale >= 0) {
        paletteBase = (u8 *)asset + (asset->entryCount * sizeof(AssetTableEntry)) + sizeof(AssetTableEntry);
        entry = (AssetTableEntry *)((u8 *)asset + (entryIndex * sizeof(AssetTableEntry)));
        entry2 = entry + 1;
        width = entry2->width;
        height = entry2->height;
        drawWidth = width >> scale;
        drawHeight = height >> scale;

        x0 = x + D_8015660E + ((width - drawWidth) / 2);
        y0 = y + D_80156610 + ((height - drawHeight) / 2);
        x1 = x0 + drawWidth;
        y1 = y0 + drawHeight;
        clipS = 0;
        clipT = 0;

        viewHalfWidth = D_8015660A / 2;
        maxX = D_8015660E + viewHalfWidth;
        if (x0 < maxX) {
            viewHalfHeight = D_8015660C / 2;
            maxY = D_80156610 + viewHalfHeight;
            if (y0 < maxY) {
                minX = D_8015660E - viewHalfWidth;
                if (x1 >= minX) {
                    minY = D_80156610 - viewHalfHeight;
                    if (y1 >= minY) {
                        if (x0 < minX) {
                            clipS = minX - x0;
                            x0 = minX;
                        }
                        if (y0 < minY) {
                            clipT = minY - y0;
                            y0 = minY;
                        }
                        if (x1 >= maxX) {
                            x1 = maxX;
                        }
                        if (y1 >= maxY) {
                            y1 = maxY;
                        }

                        FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xBA000C02, 0x3000);
                        FONT_GFX_CMD(gRegionAllocPtr++, (((entry2->width >> 1) - 1) & 0xFFF) | 0xFD480000,
                                     (u32)((u8 *)asset + entry2->imageOffset));
                        FONT_GFX_CMD(gRegionAllocPtr++, (((((entry2->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9 | 0xF5480000,
                                     0x07080200);
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xF4000000,
                                     (((entry2->width * 2) & 0xFFF) << 12) | 0x07000000 | ((entry2->height * 4) & 0xFFF));
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                        FONT_GFX_CMD(gRegionAllocPtr++, (((((entry2->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9 | 0xF5400000,
                                     0x00080200);
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000,
                                     (((entry2->width * 4) & 0xFFF) << 12) | ((entry2->height * 4) & 0xFFF));

                        FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)(paletteBase + (paletteIndex << 5)));
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
                        gfx = gRegionAllocPtr++;
                        gfx->words.w0 = 0xE7000000;
                        gfx->words.w1 = 0;

                        FONT_GFX_CMD(gRegionAllocPtr++, (((x1 * 4) & 0xFFF) << 12) | 0xE4000000 | ((y1 * 4) & 0xFFF),
                                     (((x0 * 4) & 0xFFF) << 12) | ((y0 * 4) & 0xFFF));
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xB4000000,
                                     (((clipS << 5) + 0x10) << 16) | (((clipT << 5) + 0x10) & 0xFFFF));
                        dsdx = (1 << (scale + 10)) & 0xFFFF;
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xB3000000, (dsdx << 16) | dsdx);
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xBA000C02, 0);
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                    }
                }
            }
        }
    }
}
#endif

// func_80047B84 best match: 87.572% (nonmatchings/func_80047B84-7273315160691878794/base_11.c)
#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80047B84.s")

#ifdef NON_MATCHING
void func_80047B84(s16 x, s16 y, u16 s, u16 t, u16 paletteIndex) {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    volatile char pad[0x48];
    s32 maxX;
    s32 maxY;
    s32 minX;
    s32 minY;
    s32 clipS;
    s32 clipT;
    s32 viewHalfWidth;
    s32 viewHalfHeight;

    x0 = x + D_8015660E;
    y0 = y + D_80156610;
    x1 = x0 + 8;
    y1 = y0 + 8;
    clipS = 0;
    clipT = 0;

    viewHalfWidth = D_8015660A / 2;
    maxX = D_8015660E + viewHalfWidth;
    if (x0 < maxX) {
        minX = D_8015660E - viewHalfWidth;
        viewHalfHeight = D_8015660C / 2;
        maxY = D_80156610 + viewHalfHeight;
        if (y0 < maxY) {
            minY = D_80156610 - viewHalfHeight;
            if ((x1 >= minX) && (y1 >= minY)) {
                if (x0 < minX) {
                    clipS = minX - x0;
                    x0 = minX;
                }
                if (y0 < minY) {
                    clipT = minY - y0;
                    y0 = minY;
                }
                if (x1 >= maxX) {
                    x1 = maxX - 1;
                }
                if (y1 >= maxY) {
                    y1 = maxY - 1;
                }
                clipS += s;
                clipT += t;

                if (paletteIndex != D_801121B0) {
                    D_801121B0 = paletteIndex;
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)D_801121B4 + (paletteIndex << 5));
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                }

                FONT_GFX_CMD(gRegionAllocPtr++, (((x1 * 4) & 0xFFF) << 12) | 0xE4000000 | ((y1 * 4) & 0xFFF),
                             (((x0 * 4) & 0xFFF) << 12) | ((y0 * 4) & 0xFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xB4000000,
                             (clipS << 21) | ((clipT << 5) & 0xFFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xB3000000, 0x04000400);
            }
        }
    }
}
#endif

extern s16 D_8011213C;
extern s16 D_801121B2;

void func_80047E38(void) {
    s32 v0 = func_80043040(D_8011213C);
    AssetTable *assetTable = (AssetTable *)v0;

    D_801121B4 = (void *)((assetTable->entryCount * sizeof(AssetTableEntry)) + (u8 *)assetTable + sizeof(AssetTableEntry));
    D_801121B2 = -1;
    D_801121B0 = -1;
}

// func_80047E88 best match: 97.351% (nonmatchings/func_80047E88-2785870559185086986/base_11.c)
#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80047E88.s")

#ifdef NON_MATCHING
void func_80047E88(s16 x, s16 y, s32 ch, u16 arg3) {
    volatile char pad[0x48];
    volatile s32 *chPtr;
    s32 chByte;
    s32 tile;
    FontTexture *font;

    chByte = ch & 0xFF;
    chPtr = &ch;
    if ((chByte >= 'a') && (chByte < '{')) {
        tile = chByte - 0x40;

        if (D_801121B2 != 0) {
            font = (FontTexture *)func_80043040(D_8011213C);

            FONT_GFX_CMD(gRegionAllocPtr++, (((font->width >> 1) - 1) & 0xFFF) | 0xFD480000,
                         (u32)(font->imageOffset + (u8 *)font));
            FONT_GFX_CMD(gRegionAllocPtr++, (((((font->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9 | 0xF5480000,
                         0x07080200);
            FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            FONT_GFX_CMD(gRegionAllocPtr++, 0xF4000000,
                         (((font->width * 2) & 0xFFF) << 12) | 0x07000000 | ((font->height * 4) & 0xFFF));
            FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            FONT_GFX_CMD(gRegionAllocPtr++, (((((font->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9 | 0xF5400000,
                         0x00080200);
            FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000,
                         (((font->width * 4) & 0xFFF) << 12) | ((font->height * 4) & 0xFFF));

            D_801121B2 = 0;
            D_801121B0 = -1;
            tile = chByte - 0x40;
        }

        func_80047B84(x, y, ((tile & 7) * 8) & 0xFFFF, tile & 0x38, arg3);
        return;
    }

    tile = chByte - 0x20;
    if (D_801121B2 != 0) {
        font = (FontTexture *)func_80043040(D_8011213C);

        FONT_GFX_CMD(gRegionAllocPtr++, (((font->width >> 1) - 1) & 0xFFF) | 0xFD480000,
                     (u32)(font->imageOffset + (u8 *)font));
        FONT_GFX_CMD(gRegionAllocPtr++, (((((font->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9 | 0xF5480000,
                     0x07080200);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xF4000000,
                     (((font->width * 2) & 0xFFF) << 12) | 0x07000000 | ((font->height * 4) & 0xFFF));
        FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
        FONT_GFX_CMD(gRegionAllocPtr++, (((((font->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9 | 0xF5400000,
                     0x00080200);
        FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000,
                     (((font->width * 4) & 0xFFF) << 12) | ((font->height * 4) & 0xFFF));

        D_801121B2 = 0;
        D_801121B0 = -1;
        tile = chByte - 0x20;
    }

    if ((u32)tile < 0x40) {
        func_80047B84(x, y, ((tile & 7) * 8) & 0xFFFF, tile & 0x38, arg3);
    }
}
#endif

#ifndef NON_MATCHING
extern void func_80047E88(s16 x, s16 y, volatile s32 ch, u16 arg3);
#endif

void func_80048278(s16 arg0, s16 arg1, u8 *arg2, u16 arg3) {
    s32 var_s0;
    u8 *var_s1;
    char mask;
    s32 var_s2;
    s32 temp_s4;
    s32 temp_t9;
    s32 var_a2;
    s32 newline;

    mask = 0xFFFFFFFFFFFFFFFFu;
    temp_t9 = *arg2 & mask;
    var_s0 = arg0;
    var_s1 = arg2;
    var_s2 = arg1;
    temp_s4 = var_s0;
    if ((0, temp_t9) != 0) {
        var_a2 = temp_t9 & 0xFF;
        arg0 = arg0;
        newline = 0xA;
        do {
            if (newline == var_a2) {
                var_s0 = temp_s4;
                var_s2 += 8;
            } else {
                func_80047E88(var_s0, var_s2, var_a2, arg3);
                var_s0 += 8;
            }
            var_a2 = var_s1[1];
            var_s1++;
        } while (var_a2 != 0);
    }
}

extern s32 D_801121B8;

void func_80048338(void) {
    D_80112130 = func_80042D58(0x4000);
}

void func_8004835C(void) {
    D_801121B8 = func_80043040(D_80112130);
}

void *func_80048388(s32 arg0) {
    s32 *new_var2;
    u32 new_var;
    s32 sp1C;
    s32 base;
    s32 temp_a0;

    sp1C = D_801121B8;
    base = func_80043040(D_80112130);
    new_var2 = &temp_a0;
    temp_a0 = D_801121B8 + ((((u32)(arg0 + 3)) >> 2) * 4);
    new_var = (u32)(*new_var2 - base);
    if (new_var >= 0x4001U) {
        return NULL;
    }
    D_801121B8 = *new_var2;
    return (void *)sp1C;
}

void func_800483FC(RenderCallbackNode **arg0, void (*arg1)(s32), s32 arg2) {
    RenderCallbackNode *temp_v0 = func_80048388(sizeof(RenderCallbackNode));

    if (temp_v0 != NULL) {
        temp_v0->next = *arg0;
        temp_v0->callback = arg1;
        temp_v0->arg = arg2;
        *arg0 = temp_v0;
    }
}

extern s32 gRegionAllocPtr;
extern s32 D_80124904;

void func_8004845C(RenderCallbackNode **arg0) {
    RenderCallbackNode *s0 = *arg0;
    if (s0 != NULL) {
loop:
        if ((u32)((gRegionAllocPtr - D_80124904) - 0x5B8) < 0x14181U) {
            s0->callback(s0->arg);
            s0 = s0->next;
            if (s0 != NULL) {
                goto loop;
            }
        }
    }
}

extern s16 D_80112132;
extern s16 D_80112134;

void func_800484F0(void) {
    D_80112132 = func_80042D58(0x8000);
    D_80112134 = func_80042D58(0x8000);
}

extern u32 D_80123754;
extern u8 *D_801121C0;
extern u8 *D_801121BC;

void func_80048524(s32 arg0) {
    D_80123754 = 0;
    if (arg0 == 0) {
        D_801121BC = D_801121C0 = func_80043040(D_80112132);
    } else {
        D_801121BC = D_801121C0 = func_80043040(D_80112134);
    }
}

void *func_80048594(s32 size) {
    u8 *oldPtr = D_801121BC;
    u8 *newPtr = ((0, oldPtr)) + ((((u32)(size + 3)) >> 2) * 4);
    s32 new_var2;

    new_var2 = newPtr - D_801121C0;
    newPtr++;
    newPtr--;

    if (D_801121BC) {
    }

    if ((u32)(newPtr - D_801121C0) >= 0x8000) {
        return 0;
    }

    D_801121BC = newPtr;
    D_80123754 = new_var2;

    return oldPtr;
}

extern void osWritebackDCache(void *, s32);

void func_800485E8(s32 arg0) {
    if (arg0 == 0) {
        osWritebackDCache((void *)func_80043040(D_80112132), D_80123754);
    } else {
        osWritebackDCache((void *)func_80043040(D_80112134), D_80123754);
    }
}

extern void *func_80048594(s32);

void *func_8004864C(GfxCommandBlock *arg0) {
    GfxCommandBlock *p = func_80048594(sizeof(GfxCommandBlock));
    if (p == NULL) {
        return NULL;
    }
    *p = *arg0;
    return p;
}

void func_800486BC(void *arg0, void *arg1) {
    GfxCommandSource *src = arg0;
    GfxCommandDest *dst = arg1;

    dst->unk0 = ((src->unk2 >> 12) & 0xFFFF) | ((src->unk0 << 4) & 0xFFFF0000);
    dst->unk4 = (src->unk4 << 4) & 0xFFFF0000;
    dst->unk8 = ((src->unk8 >> 12) & 0xFFFF) | ((src->unk6 << 4) & 0xFFFF0000);
    dst->unkC = (src->unkA << 4) & 0xFFFF0000;
    dst->unk10 = ((src->unkE >> 12) & 0xFFFF) | ((src->unkC << 4) & 0xFFFF0000);
    dst->unk14 = (src->unk10 << 4) & 0xFFFF0000;
    dst->unk18 = ((src->unk18 >> 16) & 0xFFFF) | (src->unk14 & 0xFFFF0000);
    dst->unk1C = (src->unk1C & 0xFFFF0000) | 1;
    dst->unk20 = ((src->unk2 << 4) & 0xFFFF) | ((src->unk0 << 20) & 0xFFFF0000);
    dst->unk24 = (src->unk4 << 20) & 0xFFFF0000;
    dst->unk28 = ((src->unk8 << 4) & 0xFFFF) | ((src->unk6 << 20) & 0xFFFF0000);
    dst->unk2C = (src->unkA << 20) & 0xFFFF0000;
    dst->unk30 = ((src->unkE << 4) & 0xFFFF) | ((src->unkC << 20) & 0xFFFF0000);
    dst->unk34 = (src->unk10 << 20) & 0xFFFF0000;
    dst->unk38 = (src->unk18 & 0xFFFF) | ((src->unk14 << 16) & 0xFFFF0000);
    dst->unk3C = (src->unk1C << 16) & 0xFFFF0000;
}

GfxCommandDest *func_8004885C(GfxCommandSource *arg0) {
    GfxCommandDest *dst = func_80048594(sizeof(GfxCommandDest));

    if (dst == NULL) {
        return NULL;
    }

    dst->unk0 = ((arg0->unk2 >> 12) & 0xFFFF) | ((arg0->unk0 << 4) & 0xFFFF0000);
    dst->unk4 = (arg0->unk4 << 4) & 0xFFFF0000;
    dst->unk8 = ((arg0->unk8 >> 12) & 0xFFFF) | ((arg0->unk6 << 4) & 0xFFFF0000);
    dst->unkC = (arg0->unkA << 4) & 0xFFFF0000;
    dst->unk10 = ((arg0->unkE >> 12) & 0xFFFF) | ((arg0->unkC << 4) & 0xFFFF0000);
    dst->unk14 = (arg0->unk10 << 4) & 0xFFFF0000;
    dst->unk18 = ((arg0->unk18 >> 16) & 0xFFFF) | (arg0->unk14 & 0xFFFF0000);
    dst->unk1C = (arg0->unk1C & 0xFFFF0000) | 1;
    dst->unk20 = ((arg0->unk2 << 4) & 0xFFFF) | ((arg0->unk0 << 20) & 0xFFFF0000);
    dst->unk24 = (arg0->unk4 << 20) & 0xFFFF0000;
    dst->unk28 = ((arg0->unk8 << 4) & 0xFFFF) | ((arg0->unk6 << 20) & 0xFFFF0000);
    dst->unk2C = (arg0->unkA << 20) & 0xFFFF0000;
    dst->unk30 = ((arg0->unkE << 4) & 0xFFFF) | ((arg0->unkC << 20) & 0xFFFF0000);
    dst->unk34 = (arg0->unk10 << 20) & 0xFFFF0000;
    dst->unk38 = (arg0->unk18 & 0xFFFF) | ((arg0->unk14 << 16) & 0xFFFF0000);
    dst->unk3C = (arg0->unk1C << 16) & 0xFFFF0000;
    return dst;
}

GfxCommandDest *func_80048A38(GfxCommandSource *arg0) {
    GfxCommandDest *dst = func_80048594(sizeof(GfxCommandDest));

    if (dst == NULL) {
        return NULL;
    }

    dst->unk0 = ((arg0->unk2 >> 12) & 0xFFFF) | ((arg0->unk0 << 4) & 0xFFFF0000);
    dst->unk4 = (arg0->unk4 << 4) & 0xFFFF0000;
    dst->unk8 = ((arg0->unk8 >> 12) & 0xFFFF) | ((arg0->unk6 << 4) & 0xFFFF0000);
    dst->unkC = (arg0->unkA << 4) & 0xFFFF0000;
    dst->unk10 = ((arg0->unkE >> 12) & 0xFFFF) | ((arg0->unkC << 4) & 0xFFFF0000);
    dst->unk14 = (arg0->unk10 << 4) & 0xFFFF0000;
    dst->unk18 = 0;
    dst->unk1C = 1;
    dst->unk20 = ((arg0->unk2 << 4) & 0xFFFF) | ((arg0->unk0 << 20) & 0xFFFF0000);
    dst->unk24 = (arg0->unk4 << 20) & 0xFFFF0000;
    dst->unk28 = ((arg0->unk8 << 4) & 0xFFFF) | ((arg0->unk6 << 20) & 0xFFFF0000);
    dst->unk2C = (arg0->unkA << 20) & 0xFFFF0000;
    dst->unk30 = ((arg0->unkE << 4) & 0xFFFF) | ((arg0->unkC << 20) & 0xFFFF0000);
    dst->unk34 = (arg0->unk10 << 20) & 0xFFFF0000;
    dst->unk38 = 0;
    dst->unk3C = 0;
    return dst;
}

GfxCommandDest *func_80048BBC(GfxCommandDest *arg0) {
    GfxCommandDest *dst = func_80048594(sizeof(GfxCommandDest));

    if (dst == NULL) {
        return NULL;
    }

    dst->unk0 = 0x10000;
    dst->unk4 = 0;
    dst->unk8 = 1;
    dst->unkC = 0;
    dst->unk10 = 0;
    dst->unk14 = 0x10000;
    dst->unk18 = ((arg0->unk18 >> 16) & 0xFFFF) | (arg0->unk14 & 0xFFFF0000);
    dst->unk1C = (arg0->unk1C & 0xFFFF0000) | 1;
    dst->unk20 = 0;
    dst->unk24 = 0;
    dst->unk28 = 0;
    dst->unk2C = 0;
    dst->unk30 = 0;
    dst->unk34 = 0;
    dst->unk38 = (arg0->unk18 & 0xFFFF) | ((arg0->unk14 << 16) & 0xFFFF0000);
    dst->unk3C = (arg0->unk1C << 16) & 0xFFFF0000;
    return dst;
}

void func_80048C90(GfxCommandDest *arg0, GfxCommandTriple *arg1) {
    arg0->unk18 = (s32) ((arg1->unk0 & 0xFFFF0000) | (((s32) arg1->unk4 >> 0x10) & 0xFFFF));
    arg0->unk1C = (s32) ((arg1->unk8 & 0xFFFF0000) | 1);
    arg0->unk38 = (s32) (((arg1->unk0 << 0x10) & 0xFFFF0000) | (arg1->unk4 & 0xFFFF));
    arg0->unk3C = (s32) ((arg1->unk8 << 0x10) & 0xFFFF0000);
}

void func_80048CF8(GfxCommandBlock *arg0, GfxCommandBlock *arg1) {
    arg1->words[6] = (arg0->words[5] & 0xFFFF0000) | ((arg0->words[6] >> 0x10) & 0xFFFF);
    arg1->words[7] = (arg0->words[7] & 0xFFFF0000) | 1;
    arg1->words[14] = ((arg0->words[5] << 0x10) & 0xFFFF0000) | (arg0->words[6] & 0xFFFF);
    arg1->words[15] = (arg0->words[7] << 0x10) & 0xFFFF0000;
}

void func_80048D60(FixedMatrix3s arg0) {
    arg0[0] = arg0[0] / 4;
    arg0[1] = arg0[1] / 4;
    arg0[2] = arg0[2] / 4;
    arg0[3] = arg0[3] / 4;
    arg0[4] = arg0[4] / 4;
    arg0[5] = arg0[5] / 4;
    arg0[6] = arg0[6] / 4;
    arg0[7] = arg0[7] / 4;
    arg0[8] = arg0[8] / 4;
}

void func_80048E3C(void *arg0, void *arg1, void *arg2) {
}

void func_80048E4C(void *arg0, void *arg1, void *arg2, void *arg3) {
}

// func_80048E60 best match: 99.126%

#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80048E60.s")

#ifdef NON_MATCHING
s32 func_80048E60(Vec3i *pos) {
    Vec3i *posAlias;
    s32 tempZ;
    s32 *zPtr;
    s32 diffX;
    s32 temp;
    s32 lowerCheck;
    s32 lower;
    s32 diffZ;

    if (D_800EC9C2 == 2) {
        return 1;
    }

    if (D_801124B0 != 0) {
        diffX = D_801121F8;
        diffX = diffX - pos->x;
        if ((diffX < 0x6000000) && (diffX >= (s32)0xFA000001)) {
            zPtr = &pos->z;
            tempZ = D_80112200;
            diffZ = tempZ - *zPtr;
            if ((diffZ < 0x6000000) && (diffZ >= (s32)0xFA000001)) {
                return 1;
            }
        }
    }

    posAlias = pos;
    if (D_80112560 != 0) {
        diffX = D_801122A8 - pos->x;
        temp = D_801122B0;
        diffZ = temp - posAlias->z;
        lowerCheck = diffX >= (s32)0xFA000001;
        if ((diffX < 0x6000000) && lowerCheck && (diffZ < 0x6000000) && (diffZ >= (s32)0xFA000001)) {
            return 1;
        }
    }

    if (D_80112610 != 0) {
        diffX = D_80112358 - posAlias->x;
        diffZ = D_80112360 - pos->z;
        if ((diffX < 0x6000000) && (diffX >= (s32)0xFA000001) && (diffZ < 0x6000000) && (diffZ >= (s32)0xFA000001)) {
            return 1;
        }
    }

    if (D_801126C0 != 0) {
        diffX = D_80112408 - posAlias->x;
        diffZ = D_80112410 - posAlias->z;
        lower = 0xFA000001;
        if ((diffX < 0x6000000) && (diffX >= lower) && (diffZ < 0x6000000) && (diffZ >= lower)) {
            return 1;
        }
    }

    return 0;
}
#endif
