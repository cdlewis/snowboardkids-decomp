#include "common.h"
#include "memory_allocator.h"

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

extern s16 D_8011213E;
extern s16 D_801121B0;

#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_800458E0.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80045A78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80045E84.s")

#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80046358.s")

#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80046748.s")

#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80046970.s")

#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80046D68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80047174.s")

#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_8004767C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80047B84.s")

extern s16 D_8011213C;
extern s16 D_801121B2;
extern void *D_801121B4;

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

extern s16 D_80112130;
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

#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80048D60.s")

void func_80048E3C(void *arg0, void *arg1, void *arg2) {
}

void func_80048E4C(void *arg0, void *arg1, void *arg2, void *arg3) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/render_asset_utils/func_80048E60.s")
