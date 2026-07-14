#include "common.h"
#include "game/engine/relocatable_heap.h"
#include "game/math/fixed_point_math.h"
#include "game/engine/viewport_manager.h"

#define FONT_GFX_CMD(pkt, cmd0, cmd1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (cmd0); \
    _g->words.w1 = (cmd1); \
}

typedef struct {
    /* 0x0 */ s32 imageOffset;
    /* 0x4 */ u16 textureIndex;
    /* 0x6 */ u8 width;
    /* 0x7 */ u8 height;
} AssetTableEntry;

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 entryCount;
    /* 0x8 */ AssetTableEntry entries[1];
} AssetTable;

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

extern u8 gRaceSplitscreenMode;
extern s32 D_801121F8;
extern s32 D_80112200;
extern s32 D_801122A8;
extern s32 D_801122B0;
extern s32 D_80112358;
extern s32 D_80112360;
extern s32 D_80112408;
extern s32 D_80112410;
extern s16 gAssetHandles[];
extern s16 gCurrentRaceRecordReplayHandle;
extern s16 gMenuAsciiFontPaletteIndex;
extern s16 gMenuViewportWidth;
extern s16 gMenuViewportHeight;
extern s16 gMenuViewportCenterX;
extern s16 gMenuViewportCenterY;
extern Gfx *gRegionAllocPtr;
extern void *gMenuAsciiFontPaletteBase;

void initMenuAssetHandles(void)
{
    s16 *end;
    s16 *handle;
    end = &gMenuAsciiFontPaletteIndex;
    do { handle = gAssetHandles; do { end = &gMenuAsciiFontPaletteIndex;
        *handle++ = -1;
        *handle++ = -1;
        *handle++ = -1;
        *handle++ = -1;
    }
    while (handle != end);
    }
    while (0);
    end++;
    end--;
}

void releaseMenuAssetHandles(void)
{
 do { s16 *handle = &gCurrentRaceRecordReplayHandle; do { if ((*handle) != (-1)) { *handle = freeRelocatableHeapBlock(*handle); } handle++; } while (handle != (&gMenuAsciiFontPaletteIndex)); } while (0);
}

void *resolveAssetTableRelativePointer(void *arg0, u32 arg1) {
    return (void *)((u8 *)arg0 + (arg1 & 0xFFFFFF));
}

void getAssetTableImageAndPalette(u8 *arg0, u16 arg1, void **arg2, void **arg3) {
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

void getAssetTableImageAndExplicitPalette(u8 *arg0, u16 arg1, u16 arg2, void **arg3, void **arg4) {
    u8 *temp_v0;
    AssetTableEntry *temp_v1;

    temp_v1 = (AssetTableEntry *)((s32)arg0 + (arg1 * sizeof(AssetTableEntry)));
    temp_v0 = arg0 + (((AssetTable *)arg0)->entryCount * sizeof(AssetTableEntry));
    *arg3 = (void *)(arg0 + temp_v1[1].imageOffset);
    temp_v0 += 8;
    *arg4 = (void *)((arg2 << 5) + temp_v0);
}

void getAssetTableImagePaletteAndSize(u8 *arg0, u16 arg1, void **arg2, void **arg3, s16 *arg4, s16 *arg5) {
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

// drawAssetTableSprite best match: 97.050% (nonmatchings/drawAssetTableSprite/output-775-1/source.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_render_utils/drawAssetTableSprite.s")

#ifdef NON_MATCHING
void drawAssetTableSprite(s16 arg0, s16 arg1, AssetTable *arg2, u16 arg3) {
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

    textureBase = (((u8 *)arg2) + (arg2->entryCount * sizeof(AssetTableEntry))) + sizeof(AssetTableEntry);
    rawEntry = (AssetTableEntry *)(((u8 *)arg2) + (arg3 * sizeof(AssetTableEntry)));
    x0 = arg0 + gMenuViewportCenterX;
    y0 = arg1 + gMenuViewportCenterY;
    x1 = rawEntry[1].width + x0;
    do { y1 = rawEntry[1].height + y0; rawEntry++; clipU = 0; clipV = 0; rightClip = gMenuViewportCenterX + (gMenuViewportWidth / 2); if (x0 < rightClip) { leftClip = gMenuViewportCenterX - (gMenuViewportWidth / 2); bottomClip = gMenuViewportCenterY + (gMenuViewportHeight / 2); if (y0 < bottomClip) { topClip = gMenuViewportCenterY - (gMenuViewportHeight / 2); if ((x1 >= leftClip) && (y1 >= topClip)) { if (x0 < leftClip) { clipU = leftClip - x0; x0 = leftClip; } if (y0 < topClip) { clipV = topClip - y0; y0 = topClip; } if (rightClip <= x1) { x1 = rightClip; } if (y1 >= bottomClip) { y1 = bottomClip; } { Gfx *_g = (Gfx *)(gRegionAllocPtr++); _g->words.w0 = (((rawEntry->width >> 1) - 1) & 0xFFF) | 0xFD480000; _g->words.w1 = (u32)(((u8 *)arg2) + rawEntry->imageOffset); } { Gfx *_g = (Gfx *)(gRegionAllocPtr++); _g->words.w0 = ((((((rawEntry->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5480000; _g->words.w1 = 0x07080200; } { Gfx *_g = (Gfx *)(gRegionAllocPtr++); _g->words.w0 = 0xE6000000; _g->words.w1 = 0; } { Gfx *_g = (Gfx *)(gRegionAllocPtr++); _g->words.w0 = 0xF4000000; _g->words.w1 = ((((rawEntry->width * 2) & 0xFFF) << 12) | 0x07000000) | ((rawEntry->height * 4) & 0xFFF); } { Gfx *_g = (Gfx *)(gRegionAllocPtr++); _g->words.w0 = 0xE7000000; _g->words.w1 = 0; } { Gfx *_g = (Gfx *)(gRegionAllocPtr++); _g->words.w0 = ((((((rawEntry->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5400000; _g->words.w1 = 0x00080200; } { Gfx *_g = (Gfx *)(gRegionAllocPtr++); do { _g->words.w0 = 0xF2000000; } while (0); _g->words.w1 = (((rawEntry->width * 4) & 0xFFF) << 12) | ((rawEntry->height * 4) & 0xFFF); } { Gfx *_g = (Gfx *)(gRegionAllocPtr++); _g->words.w0 = 0xFD100000; _g->words.w1 = (u32)(textureBase + (rawEntry->textureIndex << 5)); } { Gfx *_g = (Gfx *)(gRegionAllocPtr++); _g->words.w0 = 0xE8000000; _g->words.w1 = 0; } { Gfx *_g = (Gfx *)(gRegionAllocPtr++); _g->words.w0 = 0xF5000100; _g->words.w1 = 0x07000000; } { Gfx *_g = (Gfx *)(gRegionAllocPtr++); _g->words.w0 = 0xE6000000; _g->words.w1 = 0; } { Gfx *_g = (Gfx *)(gRegionAllocPtr++); _g->words.w0 = 0xF0000000; _g->words.w1 = 0x0703C000; } { Gfx *_g = (Gfx *)(gRegionAllocPtr++); _g->words.w0 = 0xE7000000; _g->words.w1 = 0; } { Gfx *_g = (Gfx *)(gRegionAllocPtr++); _g->words.w0 = ((((x1 * 4) & 0xFFF) << 12) | 0xE4000000) | ((y1 * 4) & 0xFFF); _g->words.w1 = (((x0 * 4) & 0xFFF) << 12) | ((y0 * 4) & 0xFFF); } { Gfx *_g = (Gfx *)(gRegionAllocPtr++); _g->words.w0 = 0xB4000000; leftClip = clipU; _g->words.w1 = (leftClip << 21) | ((clipV << 5) & 0xFFFF); } { Gfx *_g = (Gfx *)(gRegionAllocPtr++); _g->words.w0 = 0xB3000000; _g->words.w1 = 0x04000400; } } } } } while (0);
}
#endif

// drawPulsingAssetTableSprite best match: 82.650% (nonmatchings/drawPulsingAssetTableSprite-2870645799593382959/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_render_utils/drawPulsingAssetTableSprite.s")

#ifdef NON_MATCHING
extern Gfx *gRegionAllocPtr;
extern u32 gMenuRenderModeResetDl[];
extern s16 gFrameCounter;
extern s16 gMenuViewportWidth;
extern s16 gMenuViewportHeight;
extern s16 gMenuViewportCenterX;
extern s16 gMenuViewportCenterY;

void drawPulsingAssetTableSprite(s16 arg0, s16 arg1, AssetTable *arg2, u16 arg3) {
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
    s32 pulse;
    AssetTableEntry *rawEntry;

    textureBase = (((u8 *)arg2) + (arg2->entryCount * sizeof(AssetTableEntry))) + sizeof(AssetTableEntry);
    rawEntry = (AssetTableEntry *)(((u8 *)arg2) + (arg3 * sizeof(AssetTableEntry)));
    x0 = arg0 + gMenuViewportCenterX;
    y0 = arg1 + gMenuViewportCenterY;
    x1 = rawEntry[1].width + x0;
    do { y1 = rawEntry[1].height + y0; rawEntry++; clipU = 0; clipV = 0; rightClip = gMenuViewportCenterX + (gMenuViewportWidth / 2); if (x0 < rightClip) { leftClip = gMenuViewportCenterX - (gMenuViewportWidth / 2); bottomClip = gMenuViewportCenterY + (gMenuViewportHeight / 2); if (y0 < bottomClip) { topClip = gMenuViewportCenterY - (gMenuViewportHeight / 2); if ((x1 >= leftClip) && (y1 >= topClip)) { if (x0 < leftClip) { clipU = leftClip - x0; x0 = leftClip; } if (y0 < topClip) { clipV = topClip - y0; y0 = topClip; } if (rightClip <= x1) { x1 = rightClip; } if (y1 >= bottomClip) { y1 = bottomClip; } pulse = gFrameCounter & 0x1F; if (pulse >= 0x11) { pulse = 0x20 - pulse; } pulse *= 0x10; if (pulse >= 0x100) { pulse = 0xFF; } FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0); FONT_GFX_CMD(gRegionAllocPtr++, 0xFC119623, 0xFF2FFFFF); FONT_GFX_CMD(gRegionAllocPtr++, 0xFA000000, ((pulse & 0xFF) << 8) | 0xFFFF00FF); FONT_GFX_CMD(gRegionAllocPtr++, (((rawEntry->width >> 1) - 1) & 0xFFF) | 0xFD480000, (u32)(((u8 *)arg2) + rawEntry->imageOffset)); FONT_GFX_CMD(gRegionAllocPtr++, ((((((rawEntry->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5480000, 0x07080200); FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0); FONT_GFX_CMD(gRegionAllocPtr++, 0xF4000000, ((((rawEntry->width * 2) & 0xFFF) << 12) | 0x07000000) | ((rawEntry->height * 4) & 0xFFF)); FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0); FONT_GFX_CMD(gRegionAllocPtr++, ((((((rawEntry->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5400000, 0x00080200); FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000, (((rawEntry->width * 4) & 0xFFF) << 12) | ((rawEntry->height * 4) & 0xFFF)); FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)(textureBase + (rawEntry->textureIndex << 5))); FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0); FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000); FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0); FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000); FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0); FONT_GFX_CMD(gRegionAllocPtr++, ((((x1 * 4) & 0xFFF) << 12) | 0xE4000000) | ((y1 * 4) & 0xFFF), (((x0 * 4) & 0xFFF) << 12) | ((y0 * 4) & 0xFFF)); FONT_GFX_CMD(gRegionAllocPtr++, 0xB4000000, (clipU << 21) | ((clipV << 5) & 0xFFFF)); FONT_GFX_CMD(gRegionAllocPtr++, 0xB3000000, 0x04000400); FONT_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)gMenuRenderModeResetDl); } } } } while (0);
}
#endif

// drawAssetTableSpriteWithDefaultPalette best match: 88.254% (nonmatchings/drawAssetTableSpriteWithDefaultPalette-8331816093655448999/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_render_utils/drawAssetTableSpriteWithDefaultPalette.s")

#ifdef NON_MATCHING
extern Gfx *gRegionAllocPtr;
extern u16 D_800D40B0[];

void drawAssetTableSpriteWithDefaultPalette(s32 arg0, s32 arg1, u8 *arg2, s32 arg3) {
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

    arg0 = (s16)arg0;
    arg1 = (s16)arg1;
    rawEntry = (AssetTableEntry *)(arg2 + ((arg3 & 0xFFFF) * sizeof(AssetTableEntry)));
    x0 = arg0 + gMenuViewportCenterX;
    y0 = arg1 + gMenuViewportCenterY;
    clipU = 0;
    clipV = 0;
    x1 = rawEntry[1].width + x0;
    y1 = rawEntry[1].height + y0;
    rightClip = gMenuViewportCenterX + (gMenuViewportWidth / 2);
    if (x0 < rightClip) {
        leftClip = gMenuViewportCenterX - (gMenuViewportWidth / 2);
        bottomClip = gMenuViewportCenterY + (gMenuViewportHeight / 2);
        if (y0 < bottomClip) {
            topClip = gMenuViewportCenterY - (gMenuViewportHeight / 2);
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

// drawMenuFillRectangle best match: 90.158% (nonmatchings/drawMenuFillRectangle-3357475854818838508/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_render_utils/drawMenuFillRectangle.s")

#ifdef NON_MATCHING
extern Gfx gMenuRenderModeResetDl[];

void drawMenuFillRectangle(s16 x, s16 y, s16 width, s16 height, u8 red, u8 green, u8 blue) {
    volatile char pad[0x18];
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

    x0 = x + gMenuViewportCenterX;
    y0 = y + gMenuViewportCenterY;
    x1 = width + x0;
    y1 = height + y0;

    halfWidth = gMenuViewportWidth / 2;
    rightClip = gMenuViewportCenterX + halfWidth;
    if (x0 < rightClip) {
        halfHeight = gMenuViewportHeight / 2;
        bottomClip = gMenuViewportCenterY + halfHeight;
        leftClip = gMenuViewportCenterX - halfWidth;
        if (y0 < bottomClip) {
            topClip = gMenuViewportCenterY - halfHeight;
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
                gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
            }
        }
    }
}
#endif

// drawAssetTableSprite8bpp best match: 97.189% (nonmatchings/drawAssetTableSprite8bpp-2694253543240320626/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_render_utils/drawAssetTableSprite8bpp.s")

#ifdef NON_MATCHING
void drawAssetTableSprite8bpp(s16 arg0, s16 arg1, AssetTable *arg2, u16 arg3)
{
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

    textureBase = (((u8 *)arg2) + (arg2->entryCount * sizeof(AssetTableEntry))) + sizeof(AssetTableEntry);
    rawEntry = (AssetTableEntry *)(((u8 *)arg2) + (arg3 * sizeof(AssetTableEntry)));
    x0 = arg0 + gMenuViewportCenterX;
    y0 = arg1 + gMenuViewportCenterY;
    x1 = rawEntry[1].width + x0;
    do {
        y1 = rawEntry[1].height + y0;
        rawEntry++;
        clipU = 0;
        clipV = 0;
        rightClip = gMenuViewportCenterX + (gMenuViewportWidth / 2);
        if (x0 < rightClip) {
            leftClip = gMenuViewportCenterX - (gMenuViewportWidth / 2);
            bottomClip = gMenuViewportCenterY + (gMenuViewportHeight / 2);
            if (y0 < bottomClip) {
                topClip = gMenuViewportCenterY - (gMenuViewportHeight / 2);
                if ((x1 >= leftClip) && (y1 >= topClip)) {
                    if (x0 < leftClip) {
                        clipU = leftClip - x0;
                        x0 = leftClip;
                    }
                    do {
                        if (y0 < topClip) {
                            clipV = topClip - y0;
                            y0 = topClip;
                        }
                        if (rightClip <= x1) {
                            x1 = rightClip;
                        }
                        if (y1 >= bottomClip) {
                            y1 = bottomClip;
                        }
                        FONT_GFX_CMD(gRegionAllocPtr++, ((rawEntry->width - 1) & 0xFFF) | 0xFD480000,
                                     (u32)(((u8 *)arg2) + rawEntry->imageOffset));
                        FONT_GFX_CMD(gRegionAllocPtr++, ((((rawEntry->width + 8) >> 3) & 0x1FF) << 9) | 0xF5480000,
                                     0x07080200);
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xF4000000,
                                     ((((rawEntry->width * 4) & 0xFFF) << 12) | 0x07000000) |
                                         ((rawEntry->height * 4) & 0xFFF));
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                        FONT_GFX_CMD(gRegionAllocPtr++, ((((rawEntry->width + 8) >> 3) & 0x1FF) << 9) | 0xF5480000,
                                     0x00080200);
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000,
                                     (((rawEntry->width * 4) & 0xFFF) << 12) | ((rawEntry->height * 4) & 0xFFF));
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)(textureBase + (rawEntry->textureIndex << 5)));
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x073FC000);
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                        FONT_GFX_CMD(gRegionAllocPtr++, ((((x1 * 4) & 0xFFF) << 12) | 0xE4000000) |
                                                         ((y1 * 4) & 0xFFF),
                                     (((x0 * 4) & 0xFFF) << 12) | ((y0 * 4) & 0xFFF));
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xB4000000, (clipU << 21) | ((clipV << 5) & 0xFFFF));
                        FONT_GFX_CMD(gRegionAllocPtr++, 0xB3000000, 0x04000400);
                    } while (0);
                }
            }
        }
    } while (0);
}
#endif

// drawAssetTableSpriteWithExplicitPalette cleaned NON_MATCHING reference: 92.124% (nonmatchings/drawAssetTableSpriteWithExplicitPalette/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_render_utils/drawAssetTableSpriteWithExplicitPalette.s")

#ifdef NON_MATCHING
void drawAssetTableSpriteWithExplicitPalette(s16 x, s16 y, AssetTable *asset, s32 entryIndex, u16 paletteIndex) {
    volatile char pad[0x18];
    u8 *paletteBase;
    s32 srcS;
    s32 srcT;
    s32 viewportRight;
    s32 viewportBottom;
    s32 viewportLeft;
    s32 viewportTop;
    s32 screenLeft;
    s32 screenTop;
    s32 screenRight;
    s32 screenBottom;
    AssetTableEntry *sprite;

    paletteBase = (u8 *)&asset->entries[asset->entryCount];
    sprite = &asset->entries[entryIndex & 0xFFFF];
    screenLeft = x + gMenuViewportCenterX;
    screenTop = y + gMenuViewportCenterY;
    srcS = 0;
    srcT = 0;
    screenRight = sprite->width + screenLeft;
    screenBottom = sprite->height + screenTop;
    viewportRight = gMenuViewportCenterX + (gMenuViewportWidth / 2);
    if (screenLeft < viewportRight) {
        viewportLeft = gMenuViewportCenterX - (gMenuViewportWidth / 2);
        viewportBottom = gMenuViewportCenterY + (gMenuViewportHeight / 2);
        if (screenTop < viewportBottom) {
            viewportTop = gMenuViewportCenterY - (gMenuViewportHeight / 2);
            if ((screenRight >= viewportLeft) && (screenBottom >= viewportTop)) {
                if (screenLeft < viewportLeft) {
                    srcS = viewportLeft - screenLeft;
                    screenLeft = viewportLeft;
                }
                if (screenTop < viewportTop) {
                    srcT = viewportTop - screenTop;
                    screenTop = viewportTop;
                }
                if (screenRight >= viewportRight) {
                    screenRight = viewportRight;
                }
                if (screenBottom >= viewportBottom) {
                    screenBottom = viewportBottom;
                }
                FONT_GFX_CMD(gRegionAllocPtr++, (((sprite->width >> 1) - 1) & 0xFFF) | 0xFD480000,
                             (u32)((u8 *)asset + sprite->imageOffset));
                FONT_GFX_CMD(gRegionAllocPtr++, (((((sprite->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9 | 0xF5480000,
                             0x07080200);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF4000000,
                             (((sprite->width * 2) & 0xFFF) << 12) | 0x07000000 | ((sprite->height * 4) & 0xFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, (((((sprite->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9 | 0xF5400000,
                             0x00080200);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000,
                             (((sprite->width * 4) & 0xFFF) << 12) | ((sprite->height * 4) & 0xFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)(paletteBase + (paletteIndex << 5)));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
                FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                FONT_GFX_CMD(gRegionAllocPtr++, (((screenRight * 4) & 0xFFF) << 12) | 0xE4000000 | ((screenBottom * 4) & 0xFFF),
                             (((screenLeft * 4) & 0xFFF) << 12) | ((screenTop * 4) & 0xFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xB4000000, (srcS << 21) | ((srcT << 5) & 0xFFFF));
                FONT_GFX_CMD(gRegionAllocPtr++, 0xB3000000, 0x04000400);
            }
        }
    }
}
#endif

// drawScaledAssetTableSprite best match: 74.481% (nonmatchings/drawScaledAssetTableSprite-8331816093655448999/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_render_utils/drawScaledAssetTableSprite.s")

#ifdef NON_MATCHING
void drawScaledAssetTableSprite(s16 x, s16 y, AssetTable *asset, u16 entryIndex, u16 scale) {
    u8 *paletteBase;
    AssetTableEntry *sprite;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 clippedS;
    s32 clippedT;
    s32 viewHalfWidth;
    s32 viewHalfHeight;
    s32 clipRight;
    s32 clipBottom;
    s32 clipLeft;
    s32 clipTop;
    s32 spriteWidth;
    s32 spriteHeight;
    s32 scaledWidth;
    s32 scaledHeight;
    s32 dsdx;
    Gfx *gfx;

    paletteBase = (u8 *)&asset->entries[asset->entryCount];
    sprite = &asset->entries[entryIndex];
    spriteWidth = sprite->width;
    spriteHeight = sprite->height;
    scaledWidth = spriteWidth >> scale;
    scaledHeight = spriteHeight >> scale;

    x0 = x + gMenuViewportCenterX + ((spriteWidth - scaledWidth) / 2);
    y0 = y + gMenuViewportCenterY + ((spriteHeight - scaledHeight) / 2);
    x1 = x0 + scaledWidth;
    y1 = y0 + scaledHeight;
    clippedS = 0;
    clippedT = 0;

    viewHalfWidth = gMenuViewportWidth / 2;
    clipRight = gMenuViewportCenterX + viewHalfWidth;
    if (x0 < clipRight) {
        viewHalfHeight = gMenuViewportHeight / 2;
        clipBottom = gMenuViewportCenterY + viewHalfHeight;
        if (y0 < clipBottom) {
            clipLeft = gMenuViewportCenterX - viewHalfWidth;
            if (x1 >= clipLeft) {
                clipTop = gMenuViewportCenterY - viewHalfHeight;
                if (y1 >= clipTop) {
                    if (x0 < clipLeft) {
                        clippedS = clipLeft - x0;
                        x0 = clipLeft;
                    }
                    if (y0 < clipTop) {
                        clippedT = clipTop - y0;
                        y0 = clipTop;
                    }
                    if (x1 >= clipRight) {
                        x1 = clipRight;
                    }
                    if (y1 >= clipBottom) {
                        y1 = clipBottom;
                    }

                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xBA000C02, 0x3000);
                    FONT_GFX_CMD(gRegionAllocPtr++, (((sprite->width >> 1) - 1) & 0xFFF) | 0xFD480000,
                                 (u32)((u8 *)asset + sprite->imageOffset));
                    FONT_GFX_CMD(gRegionAllocPtr++, (((((sprite->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9 | 0xF5480000,
                                 0x07080200);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xF4000000,
                                 (((sprite->width * 2) & 0xFFF) << 12) | 0x07000000 | ((sprite->height * 4) & 0xFFF));
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, (((((sprite->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9 | 0xF5400000,
                                 0x00080200);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000,
                                 (((sprite->width * 4) & 0xFFF) << 12) | ((sprite->height * 4) & 0xFFF));
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)(paletteBase + (sprite->textureIndex << 5)));
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
                                 (((clippedS << 5) + 0x10) << 16) | (((clippedT << 5) + 0x10) & 0xFFFF));
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
#endif

// drawScaledAssetTableSpriteWithExplicitPalette best match: 74.637% (nonmatchings/drawScaledAssetTableSpriteWithExplicitPalette-8331816093655448999/base_3.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_render_utils/drawScaledAssetTableSpriteWithExplicitPalette.s")

#ifdef NON_MATCHING
void drawScaledAssetTableSpriteWithExplicitPalette(s16 x, s16 y, AssetTable *asset, u16 entryIndex, u16 paletteIndex, u16 scale) {
    u8 *paletteBase;
    AssetTableEntry *sprite;
    s32 screenLeft;
    s32 screenTop;
    s32 screenRight;
    s32 screenBottom;
    s32 srcS;
    s32 srcT;
    s32 viewHalfWidth;
    s32 viewHalfHeight;
    s32 viewportRight;
    s32 viewportBottom;
    s32 viewportLeft;
    s32 viewportTop;
    s32 spriteWidth;
    s32 spriteHeight;
    s32 scaledWidth;
    s32 scaledHeight;
    s32 textureStep;
    Gfx *gfx;

    paletteBase = (u8 *)&asset->entries[asset->entryCount];
    sprite = &asset->entries[entryIndex];
    spriteWidth = sprite->width;
    spriteHeight = sprite->height;
    scaledWidth = spriteWidth >> scale;
    scaledHeight = spriteHeight >> scale;

    screenLeft = x + gMenuViewportCenterX + ((spriteWidth - scaledWidth) / 2);
    screenTop = y + gMenuViewportCenterY + ((spriteHeight - scaledHeight) / 2);
    screenRight = screenLeft + scaledWidth;
    screenBottom = screenTop + scaledHeight;
    srcS = 0;
    srcT = 0;

    viewHalfWidth = gMenuViewportWidth / 2;
    viewportRight = gMenuViewportCenterX + viewHalfWidth;
    if (screenLeft < viewportRight) {
        viewHalfHeight = gMenuViewportHeight / 2;
        viewportBottom = gMenuViewportCenterY + viewHalfHeight;
        if (screenTop < viewportBottom) {
            viewportLeft = gMenuViewportCenterX - viewHalfWidth;
            if (screenRight >= viewportLeft) {
                viewportTop = gMenuViewportCenterY - viewHalfHeight;
                if (screenBottom >= viewportTop) {
                    if (screenLeft < viewportLeft) {
                        srcS = viewportLeft - screenLeft;
                        screenLeft = viewportLeft;
                    }
                    if (screenTop < viewportTop) {
                        srcT = viewportTop - screenTop;
                        screenTop = viewportTop;
                    }
                    if (screenRight >= viewportRight) {
                        screenRight = viewportRight;
                    }
                    if (screenBottom >= viewportBottom) {
                        screenBottom = viewportBottom;
                    }

                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xBA000C02, 0x3000);
                    FONT_GFX_CMD(gRegionAllocPtr++, (((sprite->width >> 1) - 1) & 0xFFF) | 0xFD480000,
                                 (u32)((u8 *)asset + sprite->imageOffset));
                    FONT_GFX_CMD(gRegionAllocPtr++, (((((sprite->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9 | 0xF5480000,
                                 0x07080200);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xF4000000,
                                 (((sprite->width * 2) & 0xFFF) << 12) | 0x07000000 | ((sprite->height * 4) & 0xFFF));
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, (((((sprite->width + 1) >> 1) + 7) >> 3) & 0x1FF) << 9 | 0xF5400000,
                                 0x00080200);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xF2000000,
                                 (((sprite->width * 4) & 0xFFF) << 12) | ((sprite->height * 4) & 0xFFF));

                    FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)(paletteBase + (paletteIndex << 5)));
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
                    gfx = gRegionAllocPtr++;
                    gfx->words.w0 = 0xE7000000;
                    gfx->words.w1 = 0;

                    FONT_GFX_CMD(gRegionAllocPtr++, (((screenRight * 4) & 0xFFF) << 12) | 0xE4000000 | ((screenBottom * 4) & 0xFFF),
                                 (((screenLeft * 4) & 0xFFF) << 12) | ((screenTop * 4) & 0xFFF));
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xB4000000,
                                 (((srcS << 5) + 0x10) << 16) | (((srcT << 5) + 0x10) & 0xFFFF));
                    textureStep = (1 << (scale + 10)) & 0xFFFF;
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xB3000000, (textureStep << 16) | textureStep);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xBA000C02, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                }
            }
        }
    }
}
#endif

// drawMenuAsciiFontTile best match: 89.942% (nonmatchings/drawMenuAsciiFontTile-3357475854818838508/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_render_utils/drawMenuAsciiFontTile.s")

#ifdef NON_MATCHING
void drawMenuAsciiFontTile(s16 x, s16 y, u16 s, u16 t, u16 paletteIndex) {
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

    x0 = x + gMenuViewportCenterX;
    y0 = y + gMenuViewportCenterY;
    x1 = x0 + 8;
    y1 = y0 + 8;
    clipS = 0;
    clipT = 0;

    viewHalfWidth = gMenuViewportWidth / 2;
    maxX = gMenuViewportCenterX + viewHalfWidth;
    if (x0 < maxX) {
        minX = gMenuViewportCenterX - viewHalfWidth;
        viewHalfHeight = gMenuViewportHeight / 2;
        maxY = gMenuViewportCenterY + viewHalfHeight;
        if (y0 < maxY) {
            minY = gMenuViewportCenterY - viewHalfHeight;
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

                if (paletteIndex != gMenuAsciiFontPaletteIndex) {
                    gMenuAsciiFontPaletteIndex = paletteIndex;
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)gMenuAsciiFontPaletteBase + (paletteIndex << 5));
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
                    FONT_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                }

                gSPTextureRectangle(gRegionAllocPtr++, x0 * 4, y0 * 4, x1 * 4, y1 * 4, 0,
                                    clipS << 5, clipT << 5, 0x400, 0x400);
            }
        }
    }
}
#endif

extern s16 gMenuAsciiFontAssetHandle;
extern s16 gMenuAsciiFontTextureNeedsLoad;

void initMenuAsciiFontTexture(void) {
    s32 v0 = getRelocatableHeapBlockBase(gMenuAsciiFontAssetHandle);
    AssetTable *assetTable = (AssetTable *)v0;

    gMenuAsciiFontPaletteBase = (void *)((assetTable->entryCount * sizeof(AssetTableEntry)) + (u8 *)assetTable + sizeof(AssetTableEntry));
    gMenuAsciiFontTextureNeedsLoad = -1;
    gMenuAsciiFontPaletteIndex = -1;
}

void drawMenuAsciiChar(x, y, ch, arg3)
s16 x;
s16 y;
u8 ch;
u16 arg3;
{
    char pad[8];
    u32 tile;
    u16 s;
    FontTexture *font;

    if ((ch >= 'a') && (ch <= 'z')) {
        if (gMenuAsciiFontTextureNeedsLoad) {
            font = (FontTexture *)getRelocatableHeapBlockBase(gMenuAsciiFontAssetHandle);

            gDPLoadTextureTile_4b(gRegionAllocPtr++, font->imageOffset + (u8 *)font, G_IM_FMT_CI,
                                  font->width, font->height, 0, 0, font->width, font->height, 0,
                                  G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

            gMenuAsciiFontTextureNeedsLoad = 0;
            gMenuAsciiFontPaletteIndex = -1;
        }
        tile = ch - 0x40;
        s = ((tile & 7) << 3) & 0xFFFF & 0xFFFF;
        drawMenuAsciiFontTile(x, y, s, tile & 0x38, arg3 & 0xFFFF & 0xFFFF & 0xFFFF);
    } else {
        if (gMenuAsciiFontTextureNeedsLoad != 0) {
            font = (FontTexture *)getRelocatableHeapBlockBase(gMenuAsciiFontAssetHandle);

            gDPLoadTextureTile_4b(gRegionAllocPtr++, font->imageOffset + (u8 *)font, G_IM_FMT_CI,
                                  font->width, font->height, 0, 0, font->width, font->height, 0,
                                  G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

            gMenuAsciiFontTextureNeedsLoad = 0;
            gMenuAsciiFontPaletteIndex = -1;
        }
        tile = ch - 0x20;
        if (tile < 0x40) {
            s = ((tile & 7) << 3) & 0xFFFF & 0xFFFF;
            drawMenuAsciiFontTile(x, y, s, tile & 0x38, arg3 & 0xFFFF & 0xFFFF & 0xFFFF);
        }
    }
}

extern void drawMenuAsciiChar(s16 x, s16 y, volatile s32 ch, u16 arg3);

void drawMenuAsciiTextDefaultScale(s16 arg0, s16 arg1, u8 *arg2, u16 arg3) {
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
                drawMenuAsciiChar(var_s0, var_s2, var_a2, arg3);
                var_s0 += 8;
            }
            var_a2 = var_s1[1];
            var_s1++;
        } while (var_a2 != 0);
    }
}

extern s32 gRenderCallbackScratchPtr;

void allocRenderCallbackScratchBuffer(void) {
    gAssetHandles[0] = allocRelocatableHeapBlock(0x4000);
}

void resetRenderScratchAllocator(void) {
    gRenderCallbackScratchPtr = getRelocatableHeapBlockBase(gAssetHandles[0]);
}

void *allocRenderCallbackScratch(s32 arg0) {
    s32 *new_var2;
    u32 new_var;
    s32 sp1C;
    s32 base;
    s32 temp_a0;

    sp1C = gRenderCallbackScratchPtr;
    base = getRelocatableHeapBlockBase(gAssetHandles[0]);
    new_var2 = &temp_a0;
    temp_a0 = gRenderCallbackScratchPtr + ((((u32)(arg0 + 3)) >> 2) * 4);
    new_var = (u32)(*new_var2 - base);
    if (new_var >= 0x4001U) {
        return NULL;
    }
    gRenderCallbackScratchPtr = *new_var2;
    return (void *)sp1C;
}

void addRenderCallback(RenderCallbackNode **arg0, void (*arg1)(s32), s32 arg2) {
    RenderCallbackNode *temp_v0 = allocRenderCallbackScratch(sizeof(RenderCallbackNode));

    if (temp_v0 != NULL) {
        temp_v0->next = *arg0;
        temp_v0->callback = arg1;
        temp_v0->arg = arg2;
        *arg0 = temp_v0;
    }
}

extern Gfx *gCurrentTaskDisplayListStart;

void runRenderCallbacks(RenderCallbackNode **arg0) {
    RenderCallbackNode *s0 = *arg0;
    if (s0 != NULL) {
loop:
        if ((u32)(((u8 *)gRegionAllocPtr - (u8 *)gCurrentTaskDisplayListStart) - 0x5B8) < 0x14181U) {
            s0->callback(s0->arg);
            s0 = s0->next;
            if (s0 != NULL) {
                goto loop;
            }
        }
    }
}

void allocMenuRenderScratchBuffers(void) {
    gAssetHandles[1] = allocRelocatableHeapBlock(0x8000);
    gAssetHandles[2] = allocRelocatableHeapBlock(0x8000);
}

extern u32 gMenuRenderScratchUsedSize;
extern u8 *gMenuRenderScratchStart;
extern u8 *gMenuRenderScratchPtr;

void selectMenuRenderScratchBuffer(s32 arg0) {
    gMenuRenderScratchUsedSize = 0;
    if (arg0 == 0) {
        gMenuRenderScratchPtr = gMenuRenderScratchStart = getRelocatableHeapBlockBase(gAssetHandles[1]);
    } else {
        gMenuRenderScratchPtr = gMenuRenderScratchStart = getRelocatableHeapBlockBase(gAssetHandles[2]);
    }
}

void *allocMenuRenderScratch(s32 size) {
    u8 *oldPtr = gMenuRenderScratchPtr;
    u8 *newPtr = ((0, oldPtr)) + ((((u32)(size + 3)) >> 2) * 4);
    s32 new_var2;

    new_var2 = newPtr - gMenuRenderScratchStart;
    newPtr++;
    newPtr--;

    if (gMenuRenderScratchPtr) {
    }

    if ((u32)(newPtr - gMenuRenderScratchStart) >= 0x8000) {
        return 0;
    }

    gMenuRenderScratchPtr = newPtr;
    gMenuRenderScratchUsedSize = new_var2;

    return oldPtr;
}

extern void osWritebackDCache(void *, s32);

void writebackMenuRenderScratchBuffer(s32 arg0) {
    if (arg0 == 0) {
        osWritebackDCache((void *)getRelocatableHeapBlockBase(gAssetHandles[1]), gMenuRenderScratchUsedSize);
    } else {
        osWritebackDCache((void *)getRelocatableHeapBlockBase(gAssetHandles[2]), gMenuRenderScratchUsedSize);
    }
}

extern void *allocMenuRenderScratch(s32);

void *copyGfxCommandBlockToScratch(GfxCommandBlock *arg0) {
    GfxCommandBlock *p = allocMenuRenderScratch(sizeof(GfxCommandBlock));
    if (p == NULL) {
        return NULL;
    }
    *p = *arg0;
    return p;
}

void packFixedTransformMatrix(void *arg0, void *arg1) {
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

GfxCommandDest *allocFixedTransformMatrix(GfxCommandSource *arg0) {
    GfxCommandDest *dst = allocMenuRenderScratch(sizeof(GfxCommandDest));

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

GfxCommandDest *allocFixedRotationMatrix(GfxCommandSource *arg0) {
    GfxCommandDest *dst = allocMenuRenderScratch(sizeof(GfxCommandDest));

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

GfxCommandDest *allocTranslationOnlyFixedMatrix(GfxCommandDest *arg0) {
    GfxCommandDest *dst = allocMenuRenderScratch(sizeof(GfxCommandDest));

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

void setPackedMatrixTranslation(GfxCommandDest *arg0, GfxCommandTriple *arg1) {
    arg0->unk18 = (s32) ((arg1->unk0 & 0xFFFF0000) | (((s32) arg1->unk4 >> 0x10) & 0xFFFF));
    arg0->unk1C = (s32) ((arg1->unk8 & 0xFFFF0000) | 1);
    arg0->unk38 = (s32) (((arg1->unk0 << 0x10) & 0xFFFF0000) | (arg1->unk4 & 0xFFFF));
    arg0->unk3C = (s32) ((arg1->unk8 << 0x10) & 0xFFFF0000);
}

void copyPackedMatrixTranslation(GfxCommandBlock *arg0, GfxCommandBlock *arg1) {
    arg1->words[6] = (arg0->words[5] & 0xFFFF0000) | ((arg0->words[6] >> 0x10) & 0xFFFF);
    arg1->words[7] = (arg0->words[7] & 0xFFFF0000) | 1;
    arg1->words[14] = ((arg0->words[5] << 0x10) & 0xFFFF0000) | (arg0->words[6] & 0xFFFF);
    arg1->words[15] = (arg0->words[7] << 0x10) & 0xFFFF0000;
}

void scaleFixedMatrix3sByQuarter(FixedMatrix3s arg0) {
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

void noopThreeArgs(void *arg0, void *arg1, void *arg2) {
}

void noopFourArgs(void *arg0, void *arg1, void *arg2, void *arg3) {
}

// isPositionNearAnyRaceViewportFocus best match: 99.175% at nonmatchings/isPositionNearAnyRaceViewportFocus-8331816093655448999/base_9.c.

#pragma GLOBAL_ASM("asm/nonmatchings/menu/renderer/menu_render_utils/isPositionNearAnyRaceViewportFocus.s")

#ifdef NON_MATCHING
extern s8 gViewportStatesViewport1Active;
extern s8 gViewportStatesViewport2Active;
extern s8 gViewportStatesViewport3Active;

s32 isPositionNearAnyRaceViewportFocus(Vec3i *pos) {
    Vec3i *posAlias;
    s32 tempZ;
    s32 *zPtr;
    s32 diffX;
    s32 temp;
    s32 lowerCheck;
    s32 lower;
    s32 diffZ;

    if (gRaceSplitscreenMode == 2) {
        return 1;
    }

    if (gViewportStates[0].active != 0) {
        diffX = D_801121F8;
        if (((diffX - pos->x) < 0x6000000) && ((diffX - pos->x) >= (s32)0xFA000001)) {
            zPtr = &pos->z;
            if (D_801121F8 && D_801121F8) {
            }
            tempZ = D_80112200;
            diffZ = tempZ - *zPtr;
            if ((diffZ < 0x6000000) && (diffZ >= (s32)0xFA000001)) {
                return 1;
            }
        }
    }

    posAlias = pos;
    if (gViewportStatesViewport1Active != 0) {
        diffX = D_801122A8 - pos->x;
        temp = D_801122B0;
        diffZ = temp - posAlias->z;
        lowerCheck = diffX >= (s32)0xFA000001;
        if ((diffX < 0x6000000) && lowerCheck && (diffZ < 0x6000000) && (diffZ >= (s32)0xFA000001)) {
            return 1;
        }
    }

    if (gViewportStatesViewport2Active != 0) {
        diffX = D_80112358 - posAlias->x;
        diffZ = D_80112360 - pos->z;
        if ((diffX < 0x6000000) && (diffX >= (s32)0xFA000001) && (diffZ < 0x6000000) && (diffZ >= (s32)0xFA000001)) {
            return 1;
        }
    }

    if (gViewportStatesViewport3Active != 0) {
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
