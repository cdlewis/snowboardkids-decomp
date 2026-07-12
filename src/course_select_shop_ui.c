#include "common.h"
#include "relocatable_heap.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "race_splitscreen_select_ui.h"
#include "course_select_shop_ui.h"
#define MENU_RENDERER_BROAD_PROTOTYPES
#include "menu_renderer.h"

typedef struct {
    char pad[0x1C];
    s32 shopItemPrice;
    s32 unk20;
} MainMenuState;

typedef struct {
    char pad0[0x6];
    /* 0x06 */ u8 selectedShopItem;
    char pad7[0x1];
    /* 0x08 */ u8 shopMenuState;
    char pad9[0x3];
    /* 0x0C */ s32 money;
} ShopMenuState;

typedef struct {
    union {
        struct {
            /* 0x00 */ u8 unk0[4];
            /* 0x04 */ u8 playerOneCourseDecided;
            /* 0x05 */ u8 unk5[3];
            /* 0x08 */ u8 playerTwoCourseDecided;
            /* 0x09 */ u8 unk9[7];
            /* 0x10 */ u8 unk10[4];
        };
        struct {
            /* 0x00 */ u8 unk0Array[4];
            /* 0x04 */ u8 unk4Array[4];
            /* 0x08 */ u8 unk8Array[4];
            /* 0x0C */ u8 unkCArray[4];
            /* 0x10 */ u8 unk10Array[4];
        };
    };
    /* 0x14 */ s16 unk14[4];
    /* 0x1C */ s16 unk1C[4];
    /* 0x24 */ u8 unk24[4];
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 transitionState;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F[9];
} CourseSelectStatus;

typedef u8 ShopDescriptionText[0x8C];

typedef struct {
    /* 0x00 */ u16 center[16];
    /* 0x20 */ u16 right[2];
    /* 0x24 */ u16 bottom[2];
    /* 0x28 */ u16 corner;
} ShopMenuFrameTileMap;

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
} ShopMenuSparkleOffset;

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 tileBase;
} ShopMenuCursorIconInit;

typedef ShopMenuSparkleOffset ShopMenuSparklePattern[13];

struct ShopMenuRowActor {
    char pad0[0x18];
    /* 0x18 */ s16 unk18[5];
    /* 0x22 */ s16 unk22;
    /* 0x24 */ u8 unk24;
    /* 0x25 */ u8 unk25;
    /* 0x26 */ u8 unk26;
};

struct ShopMenuWidgetActor {
    char pad0[0x18];
    union {
        struct {
            union {
                struct {
                    /* 0x18 */ s16 x;
                    /* 0x1A */ s16 y;
                    union {
                        /* 0x1C */ s16 index;
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
                        /* 0x1E */ u16 counter;
                        /* 0x1E */ s16 alpha;
                    } transition;
                    union {
                        struct {
                            /* 0x20 */ u8 state;
                            /* 0x21 */ u8 timer;
                            /* 0x22 */ u8 subState;
                            /* 0x23 */ u8 subTimer;
                        } bytes;
                        /* 0x20 */ s16 counter;
                        /* 0x20 */ s32 price;
                    } item;
                    union {
                        struct {
                            /* 0x24 */ u8 state;
                            /* 0x25 */ u8 timer;
                        } bytes;
                        /* 0x24 */ u8 slideState;
                    } slide;
                };
                /* 0x18 */ s16 cursorPositions[10];
            };
            /* 0x2C */ s16 targetY;
            /* 0x2E */ s16 targetX;
            union {
                struct {
                    /* 0x30 */ s16 pulseAlpha;
                    /* 0x32 */ u16 pulseTimer;
                } bytes;
                /* 0x30 */ s32 pulse;
            } prompt;
            /* 0x34 */ u16 spawnTimer;
            /* 0x36 */ s16 visibleCount;
            /* 0x38 */ u8 state;
        };
        struct {
            char pad18[6];
            union {
                /* 0x1E */ u16 counters[12];
                struct {
                    /* 0x1E */ u16 patternIndex;
                    /* 0x20 */ u16 tileBase;
                    /* 0x22 */ s16 alpha;
                };
            };
        } sparkle;
        /* 0x18 */ s16 randomValues[15];
    };
};

extern void addRenderCallback(void *, void *, void *);
extern s16 gAssetHandles[];
extern u8 gCourseSelectModeSelection;
extern u8 gMenuSelectionConfirmTimer;
extern u8 gMenuExitSelection;

void drawShopMenuModeChoiceRows(ShopMenuRowActor *arg0) {
    ShopMenuRowActor *sp54;
    ShopMenuRowActor *var_s3;
    s32 var_s0;
    s32 var_s1;
    s32 var_s2;

    sp54 = arg0;
    var_s0 = 0;
    do {
        if (arg0->unk26 > 0) {
            var_s2 = 0; var_s3 = arg0; do { var_s1 = 0;
                if ((gMenuSelectionConfirmTimer > 0) && (gMenuSelectionConfirmTimer < 8) && (gMenuExitSelection == 0) && (var_s0 == gCourseSelectModeSelection) &&
                    (gMenuSelectionConfirmTimer & 1)) {
                    var_s1 = 0xFF;
                }
                drawMenuSprite(var_s3->unk18[0], (s16)(arg0->unk22 + var_s2), getRelocatableHeapBlockBase(gAssetHandles[0x27]),
                              (var_s0 + 2) & 0xFFFF, 0x20, 0x20, 0, var_s1);
                if (1) {
                }
                var_s0 += 1;
                var_s2 += 0x1C;
                var_s3 = (ShopMenuRowActor *)((s16 *)var_s3 + 1);
            } while (var_s0 < sp54->unk26);
        }
    } while (0);
}

extern int sprintf(char *, const char *, ...);
extern s32 gCourseUnlockPrices[];
extern u16 gCourseDetailsPreviewCourseTiles[];
extern u16 gCourseDetailsPreviewExtraTiles[];
extern ShopMenuFrameTileMap gShopMenuPanelFrameTilemaps[];
extern ShopMenuCursorIconInit gShopMenuSparkleInitTable[];
extern ShopMenuSparkleOffset gCoursePreviewCloseSparkleOffsets[];
extern ShopMenuSparkleOffset gCoursePreviewCloseSparkleMirrorStart[];
extern ShopMenuSparkleOffset gCoursePreviewCloseSparkleOffsetsEnd[];
extern ShopMenuSparklePattern gShopMenuSparklePatterns[];
extern ShopDescriptionText gShopMenuModeDescriptionText[];
extern u8 gCourseUnlockPurchasePromptText;
extern u16 gCourseDetailsMenuEntryTiles[];
extern u8 gCourseSelectSelectedCourseId;
extern u8 gMenuTransitionState;
extern u8 gMenuSelectionVariant;
extern s16 gMenuPanelTilemapAssetHandle;
extern s16 gShopMenuTextureAssetHandle;
extern CourseSelectStatus gCourseSelectStatus;
extern s32 gActiveMenuTask;
extern u8 gShopMenuModeCursorState;
extern u8 gShopMenuDescriptionSeen;
extern u8 gShopMenuShowNewCoursesMessage;
extern u8 gCourseDetailsMenuSelection;
extern u8 gCourseDetailsPreviewPage;
extern s16 gCoursePreviewViewportHeight;
extern s16 gMenuUiSpritesAssetHandle;
extern ShopMenuState D_80121D80;
extern s32 gPlayer1Money;
extern s32 gMenuFlowState;
extern MainMenuState *gCurrentGameTask;
extern s16 gMenuChoicePromptState;
extern u8 gCourseUnlockSaveSlots[];
extern s32 gPlayerInputHeld;
extern u8 gMenuRenderCallbackList;

const char gShopMenuMoneyFormat[] = "%6dG";
const char gCourseUnlockPriceFormat6[] = "%6dG";
const char gCourseUnlockPriceFormat5[] = "%5dG";
const char gCourseUnlockPriceFormat4[] = "%4dG";

void updateShopMenuModeChoiceRows(ShopMenuRowActor *arg0) {
    s32 i;
    s32 moved;
    ShopMenuRowActor *spawnRow;
    s32 state;
    int stateByte;
    ShopMenuRowActor *row;
    ShopMenuRowActor *actor;

    stateByte = arg0->unk24;
    actor = arg0;
    state = stateByte;
    row = arg0;
    switch (state) {
    case 0:
        moved = 0;
        for (i = 0; i < row->unk26; i++) {
            if (row->unk18[i] < -0x7C) {
                row->unk18[i] += 0x10;
                moved++;
                if (row->unk18[i] >= -0x7C) {
                    row->unk18[i] = -0x7C;
                }
            }
        }
        row->unk25++;
        spawnRow = row;
        if (!(spawnRow->unk25 & 1)) {
            if (spawnRow->unk26 < 3) {
                spawnRow->unk26++;
            }
        }
        if (moved == 0) {
            spawnRow->unk24 = 1;
            createCallbackTask(initShopMenuSelectedModePanel, 0, 0x5F);
            createCallbackTask(initShopMenuCourseListPanel, 0, 0x61);
        }
        state = arg0->unk24;
        break;
    case 1:
        if (gMenuTransitionState == 1) {
            state = (u8) (arg0->unk24 = 2);
        }
        break;
    case 2:
        for (i = 0; i < 5; i++) {
            arg0->unk18[i] -= 0x20;
        }
        if (arg0->unk18[0] < -0x103) {
            arg0->unk24 = 3;
        }
        break;
    case 3:
        break;
    }

    if (arg0->unk24 == 3) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawShopMenuModeChoiceRows, actor);
}

void initShopMenuModeChoiceRows(ShopMenuRowActor *arg0) {
    s32 i;

    for (i = 0; i < 5; i++) { arg0->unk18[i] = -0x104; }

    arg0->unk22 = -0x50;
    arg0->unk25 = 0;
    arg0->unk26 = 1;
    arg0->unk24 = 0;

    setCallbackTaskCallback(arg0, updateShopMenuModeChoiceRows);
}

void drawShopMenuSidePanel(ShopMenuWidgetActor *arg0) {
    drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(gMenuUiSpritesAssetHandle), 3, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16) (arg0->x + 0x40), arg0->y, getRelocatableHeapBlockBase(gMenuUiSpritesAssetHandle), 4, 0x20, 0x20, 0, 0);
    drawMenuSprite(arg0->x, (s16) (arg0->y + 0x40), getRelocatableHeapBlockBase(gMenuUiSpritesAssetHandle), 5, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16) (arg0->x + 0x40), (s16) (arg0->y + 0x40), getRelocatableHeapBlockBase(gMenuUiSpritesAssetHandle), 6, 0x20, 0x20, 0, 0);
}

void updateShopMenuSidePanel(ShopMenuWidgetActor *arg0) {
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
        if (gMenuTransitionState == 1) {
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
    addRenderCallback(&gMenuRenderCallbackList, drawShopMenuSidePanel, arg0);
}

void initShopMenuSidePanel(ShopMenuWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = 8;
    setCallbackTaskCallback(arg0, updateShopMenuSidePanel);
}

// drawShopMenuSelectedModePanel best match: 99.855% (nonmatchings/drawShopMenuSelectedModePanel-8331816093655448999/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_shop_ui/drawShopMenuSelectedModePanel.s")

#ifdef NON_MATCHING
void drawShopMenuSelectedModePanel(ShopMenuWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTile((s16)(arg0->x + ((i & 3) << 5)), (s16)(arg0->y + ((i / 4) << 5)),
                      getRelocatableHeapBlockBase(gAssetHandles[0x27]), gShopMenuPanelFrameTilemaps[(u16)arg0->item.counter].center[tileOffset], 0, 0x100);
    }

    if (shouldDraw) {
        tileOffset = 0;
        if (1) {
            i = 0x80;
        }
    }
    offset = 0;
    if ((i && i) && i) {
    }
    do {
        drawMenuSpriteTile((s16)(arg0->x + 0x80), (s16)(arg0->y + offset), getRelocatableHeapBlockBase(gAssetHandles[0x27]),
                      gShopMenuPanelFrameTilemaps[(u16)arg0->item.counter].right[tileOffset], 0, 0x100);
        i = 0x80;
        drawMenuSpriteTile((s16)(arg0->x + offset), (s16)(arg0->y + 0x80), getRelocatableHeapBlockBase(gAssetHandles[0x27]),
                      gShopMenuPanelFrameTilemaps[(u16)arg0->item.counter].bottom[tileOffset], 0, 0x100);
        offset += 0x40;
        tileOffset++;
    } while (offset < 0x80);
    i++;
    i--;

    drawMenuSpriteTile((s16)(arg0->x + 0x80), (s16)(arg0->y + 0x80), getRelocatableHeapBlockBase(gAssetHandles[0x27]),
                  gShopMenuPanelFrameTilemaps[(u16)arg0->item.counter].corner, 0, 0x100);

    drawMenuSprite(arg0->x - 4, (s16)(arg0->y - 4), getRelocatableHeapBlockBase(gAssetHandles[0x25]), 0x33, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->x - 4), (s16)(arg0->y + 0x8C), getRelocatableHeapBlockBase(gAssetHandles[0x25]), 0x38, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->x + 0x8C), (s16)(arg0->y - 4), getRelocatableHeapBlockBase(gAssetHandles[0x25]), 0x35, 0x20, 0x20, 0, 0);
    if (offset && offset) {}
    drawMenuSprite((s16)(arg0->x + 0x8C), (s16)(arg0->y + 0x8C), getRelocatableHeapBlockBase(gAssetHandles[0x25]), 0x3A, 0x20, 0x20, 0, 0);

    for (offset = 0; (offset ^ 0) != 0x80; offset += 0x10) {
        drawMenuSprite((s16)((arg0->x + offset) + 0xC), (s16)(arg0->y - 4), getRelocatableHeapBlockBase(gAssetHandles[0x25]), 0x34, 0x20, 0x20, 0, 0);
        do {
            drawMenuSprite((s16)((arg0->x + offset) + 0xC), (s16)(arg0->y + 0x8C), getRelocatableHeapBlockBase(gAssetHandles[0x25]), 0x39, 0x20, 0x20, 0, 0);
        } while (0);
        drawMenuSprite((s16)(arg0->x - 4), (s16)((arg0->y + offset) + 0xC), getRelocatableHeapBlockBase(gAssetHandles[0x25]), 0x36, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)(arg0->x + 0x8C), (s16)((arg0->y + offset) + 0xC), getRelocatableHeapBlockBase(gAssetHandles[0x25]), 0x37, 0x20, 0x20, 0, 0);
    }
}
#endif

void updateShopMenuSelectedModePanel(ShopMenuWidgetActor *arg0) {
    int state;

    if ((gCourseSelectModeSelection >= (u16) arg0->item.counter) && (arg0->item.bytes.subState != 0) && (arg0->y != -0x48)) {
        state = arg0->item.bytes.subState = 2;
    } else {
        state = arg0->item.bytes.subState;
        if ((gCourseSelectModeSelection < (u16) arg0->item.counter) && (state != 0) && (arg0->y != -0x140)) {
            state = arg0->item.bytes.subState = 1;
        } else {
            state = arg0->item.bytes.subState;
            if ((state != 0) && (state < 4)) {
                state = arg0->item.bytes.subState = 3;
            }
        }
    }

    switch (state) {
    case 0:
        arg0->x -= 0x20;
        if (arg0->item.bytes.subTimer == 0) {
            createCallbackTask(initShopMenuSidePanel, 0, 0x63);
        }
        arg0->item.bytes.subTimer++;
        if (arg0->x < -7) {
            arg0->x = -8;
            arg0->item.bytes.subState = 3;
            createCallbackTask(initShopMenuDescriptionText, 0, 0x64);
            gActiveMenuTask = (s32) createCallbackTask(initShopMenuModeCursor, 0, 0x64);
            createCallbackTask(initShopMenuPromptPanel, 0, 0x64);
            createCallbackTask(initShopMenuSparkles, 0, 0x60);
            createCallbackTask(initShopMenuMoneyPanel, 0, 0x64);
        }
        state = arg0->item.bytes.subState;
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->item.bytes.subState = 3;
        }
        state = arg0->item.bytes.subState;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->item.bytes.subState = 3;
        }
        state = arg0->item.bytes.subState;
        break;
    case 3:
        gMenuFlowState += 1;
        if (gMenuTransitionState == 1) {
            arg0->item.bytes.subState = 4;
        }
        state = arg0->item.bytes.subState;
        break;
    case 4:
        arg0->x += 0x20;
        if (arg0->x >= 0xA0) {
            arg0->item.bytes.subState = 5;
        }
        state = arg0->item.bytes.subState;
        break;
    case 5:
        gMenuTransitionState = 2;
        state = arg0->item.bytes.subState;
        break;
    }

    if ((state == 5) && (gMenuTransitionState == 2)) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawShopMenuSelectedModePanel, arg0);
}

void initShopMenuSelectedModePanel(ShopMenuWidgetActor *arg0) {
    arg0->x = 0x94;
    arg0->y = -0x48;
    arg0->sprite.index = -8;
    arg0->transition.alpha = -0x74;
    arg0->item.counter = 0;
    arg0->item.bytes.subTimer = 0;
    arg0->item.bytes.subState = 0;
    setCallbackTaskCallback(arg0, updateShopMenuSelectedModePanel);
}

void drawShopMenuUnselectedModePanel(ShopMenuWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(arg0->x + ((i & 3) << 5), arg0->y + ((i / 4) << 5), getRelocatableHeapBlockBase(gAssetHandles[0x25]),
                      gShopMenuPanelFrameTilemaps[(u16)arg0->sprite.index].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + offset, getRelocatableHeapBlockBase(gAssetHandles[0x25]),
                      gShopMenuPanelFrameTilemaps[(u16)arg0->sprite.index].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        drawMenuSpriteTileClipped(arg0->x + offset, arg0->y + 0x80, getRelocatableHeapBlockBase(gAssetHandles[0x25]),
                      gShopMenuPanelFrameTilemaps[(u16)arg0->sprite.index].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + 0x80, getRelocatableHeapBlockBase(gAssetHandles[0x25]),
                  gShopMenuPanelFrameTilemaps[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void updateShopMenuUnselectedModePanel(ShopMenuWidgetActor *arg0) {
    int state;

    if ((gCourseSelectModeSelection >= (u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->item.bytes.state = 2;
    } else if ((gCourseSelectModeSelection < (u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->item.bytes.state = 1;
    } else {
        state = arg0->item.bytes.state;
        if (state < 4) {
            state = arg0->item.bytes.state = 3;
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
            arg0->item.bytes.state = 3;
        }
        state = arg0->item.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->item.bytes.state = 3;
        }
        state = arg0->item.bytes.state;
        break;
    case 3:
        gMenuFlowState += 1;
        if (gMenuTransitionState == 1) {
            if (arg0->y == -0x140) {
                arg0->item.bytes.state = 5;
            } else {
                arg0->item.bytes.state = 4;
            }
        }
        state = arg0->item.bytes.state;
        break;
    case 4:
        arg0->x += 0x20;
        if (arg0->x >= 0xA0) {
            arg0->item.bytes.state = 5;
        }
        state = arg0->item.bytes.state;
        break;
    }

    if (state == 5) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawShopMenuUnselectedModePanel, arg0);
}

void initShopMenuUnselectedModePanel(ShopMenuWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.index = 1;
    arg0->item.bytes.timer = 0;
    arg0->item.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateShopMenuUnselectedModePanel);
}

void drawShopMenuCourseListPanel(ShopMenuWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        drawMenuSpriteTileClipped(arg0->x + ((i & 3) << 5), arg0->y + ((i / 4) << 5), getRelocatableHeapBlockBase(gAssetHandles[0x25]),
                      gShopMenuPanelFrameTilemaps[(u16)arg0->sprite.index - 1].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + offset, getRelocatableHeapBlockBase(gAssetHandles[0x25]),
                      gShopMenuPanelFrameTilemaps[(u16)arg0->sprite.index - 1].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        drawMenuSpriteTileClipped(arg0->x + offset, arg0->y + 0x80, getRelocatableHeapBlockBase(gAssetHandles[0x25]),
                      gShopMenuPanelFrameTilemaps[(u16)arg0->sprite.index - 1].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    drawMenuSpriteTileClipped(arg0->x + 0x80, arg0->y + 0x80, getRelocatableHeapBlockBase(gAssetHandles[0x25]),
                  gShopMenuPanelFrameTilemaps[(u16)arg0->sprite.index - 1].corner, 0, 0x100, 0xA0, 0x49);
}

void updateShopMenuCourseListPanel(ShopMenuWidgetActor *arg0) {
    u8 *stateField;
    int state;

    stateField = &arg0->item.bytes.state;

    if ((gCourseSelectModeSelection == 2) && (arg0->y != -0x48) && (arg0->item.bytes.state < 6)) {
        state = arg0->item.bytes.state = 2;
    } else {
        state = arg0->item.bytes.state;
        if ((gCourseSelectModeSelection != 2) && (arg0->y != -0x140) && (state < 6)) {
            state = arg0->item.bytes.state = 1;
        } else if (state < 4) {
            state = arg0->item.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
    case 4:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->item.bytes.state = 3;
        }
        state = arg0->item.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->item.bytes.state = 6;
            arg0->transition.counter = 8;
        }
        state = arg0->item.bytes.state;
        break;
    case 3:
        gMenuFlowState += 1;
        if (gMenuTransitionState == 1) {
            arg0->item.bytes.state = 5;
        }
        state = arg0->item.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    case 6:
        arg0->y -= arg0->transition.counter;
        arg0->item.bytes.state = 7;
        state = arg0->item.bytes.state;
        break;
    case 7:
        arg0->y += arg0->transition.counter;
        arg0->transition.counter = arg0->transition.counter / 2;
        if (arg0->transition.counter == 0) {
            arg0->item.bytes.state = 3;
        } else {
            arg0->item.bytes.state = 6;
        }
        state = arg0->item.bytes.state;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawShopMenuCourseListPanel, arg0);
}

void initShopMenuCourseListPanel(ShopMenuWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.index = 2;
    arg0->item.bytes.timer = 0;
    arg0->item.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateShopMenuCourseListPanel);
}

void drawShopMenuModeCursor(ShopMenuWidgetActor *arg0) {
    if (arg0->transition.bytes.state != 5) {
        drawMenuSpriteWithAlpha(arg0->x, (s16)(arg0->y + (gCourseSelectModeSelection * 0x1C)), getRelocatableHeapBlockBase(gMenuUiSpritesAssetHandle), 7, 0x20, 0x20, 0, arg0->sprite.index, 0);
    }
}

void updateShopMenuModeCursor(ShopMenuWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = gCourseSelectStatus.unk28)) {
        arg0->transition.bytes.state = globalState;
        /* Preserve IDO's state/globalState register allocation. */
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        state = globalState;
        arg0->sprite.index = gCourseSelectStatus.unk2A;
        arg0->transition.bytes.timer = gCourseSelectStatus.unk2C;
    }

    switch (state) {
    case 0:
        arg0->sprite.index += 0x26;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 1;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        if ((s32)arg0->transition.bytes.timer < 0x10) {
            arg0->sprite.index -= 9;
        } else {
            arg0->sprite.index += 9;
        }
        state = arg0->transition.bytes.state;
        arg0->transition.bytes.timer = (arg0->transition.bytes.timer + 1) & 0x1F;
        break;
    case 2:
        if (gMenuTransitionState == 1) {
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
    case 5:
        break;
    }

    gShopMenuModeCursorState = state;
    if (arg0->transition.bytes.state == 4) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawShopMenuModeCursor, arg0);
}

void initShopMenuModeCursor(ShopMenuWidgetActor *arg0) {
    arg0->x = -0x7C;
    arg0->y = -0x50;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    arg0->transition.bytes.timer = 0;
    setCallbackTaskCallback(arg0, updateShopMenuModeCursor);
}

void drawShopMenuDescriptionText(ShopMenuWidgetActor *arg0) {
    u8 var_v0;

    if (gShopMenuDescriptionSeen == 0) {
        var_v0 = 3;
    } else if (gShopMenuShowNewCoursesMessage == 1) {
        var_v0 = 4;
    } else {
        var_v0 = gCourseSelectModeSelection;
    }
    drawMenuGlyphScript(arg0->x, arg0->y, gShopMenuModeDescriptionText[var_v0], 1, arg0->sprite.index, 0);
}

void updateShopMenuDescriptionText(ShopMenuWidgetActor *arg0) {
    u8 state = arg0->transition.bytes.state;

    switch (state) {
    case 0:
        arg0->sprite.index += 0x26;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 1;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        if (gMenuTransitionState == 1) {
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
        if (gShopMenuDescriptionSeen == 0) {
            gShopMenuDescriptionSeen = 1;
        }
        if (gShopMenuShowNewCoursesMessage == 1) {
            gShopMenuShowNewCoursesMessage = 0;
        }
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawShopMenuDescriptionText, arg0);
}

void initShopMenuDescriptionText(ShopMenuWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = 0xC;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateShopMenuDescriptionText);
}

void drawShopMenuPromptPanel(ShopMenuWidgetActor *arg0) {
    drawMenuSpriteWithAlpha(arg0->x, arg0->y, getRelocatableHeapBlockBase(gMenuPanelTilemapAssetHandle), 0, 0x20, 0x20, 0, arg0->sprite.index, 0);
}

void updateShopMenuPromptPanel(ShopMenuWidgetActor *arg0) {
    u8 state = arg0->transition.bytes.state;

    switch (state) {
    case 0:
        arg0->sprite.index += 0x26;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 1;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        if (gMenuTransitionState == 1) {
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
    addRenderCallback(&gMenuRenderCallbackList, drawShopMenuPromptPanel, arg0);
}

void initShopMenuPromptPanel(ShopMenuWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x5C;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateShopMenuPromptPanel);
}

void drawShopMenuMoneyPanel(ShopMenuWidgetActor *arg0) {
    char sp40[0x18];

    drawMenuPanelBackdrop(arg0->x, arg0->y, 0x5000, 0x4000);
    drawMenuSpriteWithAlpha((s16)(arg0->x + 8), (s16)(arg0->y + 4), getRelocatableHeapBlockBase(gMenuUiSpritesAssetHandle), 0x11, 0x20, 0x20, 0, arg0->sprite.index, 0);
    sprintf(sp40, gShopMenuMoneyFormat, gPlayer1Money);
    drawMenuAsciiText((s16)(arg0->x + 0x10), (s16)(arg0->y + 0x10), sp40, 0, arg0->sprite.index);
}

void updateShopMenuMoneyPanel(ShopMenuWidgetActor *arg0) {
    u8 state = arg0->transition.bytes.state;

    switch (state) {
    case 0:
        arg0->sprite.index += 0x26;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 1;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        if (gMenuTransitionState == 1) {
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
    addRenderCallback(&gMenuRenderCallbackList, drawShopMenuMoneyPanel, arg0);
}

void initShopMenuMoneyPanel(ShopMenuWidgetActor *arg0) {
    arg0->x = 0x30;
    arg0->y = 0x40;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateShopMenuMoneyPanel);
}

void updateShopMenuMoneyPanelForCourseSelectReturn(ShopMenuWidgetActor *arg0) {
    u8 state = arg0->transition.bytes.state;

    switch (state) {
    case 0:
        arg0->sprite.index += 0x26;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 1;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        if (gMenuTransitionState == 3) {
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
    addRenderCallback(&gMenuRenderCallbackList, drawShopMenuMoneyPanel, arg0);
}

void initShopMenuMoneyPanelForCourseSelectReturn(ShopMenuWidgetActor *arg0) {
    arg0->x = 0x30;
    arg0->y = 0x40;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateShopMenuMoneyPanelForCourseSelectReturn);
}

// drawShopMenuSparkles best match: 97.125% (nonmatchings/drawShopMenuSparkles-3/output-230-1/source.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_shop_ui/drawShopMenuSparkles.s")

#ifdef NON_MATCHING
void drawShopMenuSparkles(ShopMenuWidgetActor *arg0) {
    s32 tileOffset;
    s16 y;
    s16 alpha;
    s16 *randomValues;
    s32 i;
    ShopMenuWidgetActor *actor;
    ShopMenuSparkleOffset *entry;
    ShopMenuSparkleOffset *offset;
    s32 texture;
    s32 tile;

    actor = arg0;
    i = 0;
    if (gShopMenuSparklePatterns[actor->sparkle.patternIndex][0].x != -1) {
        tileOffset = 0;
        do {
            texture = getRelocatableHeapBlockBase(gAssetHandles[0x27]);
            randomValues = actor->randomValues;
            alpha = actor->sparkle.alpha;
            offset = &gShopMenuSparklePatterns[actor->sparkle.patternIndex][tileOffset];
            tile = actor->sparkle.tileBase + i;
            entry = offset;
            y = randomValues[1];
            drawMenuSpriteTile((s16)(entry->x + randomValues[0]), (s16)(y + entry->y), texture, tile & 0xFFFF, 0,
                          alpha & 0xFFFFFFFFFFFFFFFF);
            tileOffset++;
            i++;
        } while (gShopMenuSparklePatterns[actor->sparkle.patternIndex][tileOffset].x != -1);
    }
}
#endif

void updateShopMenuSparkles(ShopMenuWidgetActor *arg0) {
    s16 temp_v0;
    u8 var_v0;

    var_v0 = arg0->slide.slideState;
    switch (var_v0) {
    case 0:
        temp_v0 = arg0->sprite.index;
        arg0->x -= 0x20;
        if (temp_v0 >= arg0->x) {
            arg0->x = temp_v0;
            arg0->slide.slideState = 1U;
        }
        var_v0 = arg0->slide.slideState;
        break;
    case 1:
        if (gMenuTransitionState == 1) {
            arg0->slide.slideState = 2U;
            var_v0 = 2 & 0xFF;
        }
        break;
    }
    var_v0 = arg0->slide.slideState;
    if (var_v0 == 2) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawShopMenuSparkles, arg0);
}

// initShopMenuSparkles best match: 98.621% (nonmatchings/initShopMenuSparkles-8331816093655448999/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_shop_ui/initShopMenuSparkles.s")

#ifdef NON_MATCHING
void initShopMenuSparkles(ShopMenuWidgetActor *arg0) {
    s32 index;
    ShopMenuCursorIconInit *entry;
    s16 new_var;

    index = gMenuSelectionVariant;
    entry = &gShopMenuSparkleInitTable[0xFFFF & (u16)index];
    arg0->sparkle.patternIndex = index;
    new_var = entry->x ^ 0;
    arg0->sprite.index = new_var;
    arg0->x = 0x94;
    arg0->y = entry->y;
    arg0->sparkle.tileBase = entry->tileBase;
    arg0->sparkle.alpha = 0x100;
    arg0->slide.slideState = 0;
    setCallbackTaskCallback(arg0, updateShopMenuSparkles);
}
#endif

void drawCourseUnlockPricePanel(ShopMenuWidgetActor *arg0) {
    s16 new_var;
    s32 temp;
    char sp4C[4];
    u16 sp4A;
    u16 sp48;
    s32 palette;

    if ((s8)gCourseUnlockSaveSlots[gCourseSelectSelectedCourseId] == -1) {
        new_var = 0x3000;
        temp = gCourseUnlockPrices[gCourseSelectSelectedCourseId];
        if ((u32)temp >= 0x186A0U) {
            sp4A = 0x4000;
            sp48 = 0;
            sprintf(sp4C, gCourseUnlockPriceFormat6, arg0->item.price);
        } else if ((u32)temp >= 0x2710U) {
            sp4A = 0x3800;
            sp48 = 4;
            sprintf(sp4C, gCourseUnlockPriceFormat5, arg0->item.price);
        } else {
            sp4A = new_var;
            sp48 = 8;
            sprintf(sp4C, gCourseUnlockPriceFormat4, arg0->item.price);
        }

        drawMenuPanelBackdrop((s16)(arg0->x + sp48), arg0->y, sp4A, 0x2000);

        if ((u32)gPlayer1Money < (u32)gCourseUnlockPrices[gCourseSelectSelectedCourseId]) {
            palette = 1;
        } else {
            palette = 0;
        }
        if (arg0->slide.slideState == 2) {
            palette = 0;
        }

        drawMenuAsciiText((s16)(arg0->x + sp48 + 4), (s16)(arg0->y + 4), sp4C, palette & 0xFF, arg0->sprite.index);
        return;
    }

    drawMenuSpriteWithAlpha(arg0->x, arg0->y, getRelocatableHeapBlockBase(gMenuPanelTilemapAssetHandle), 6, 0x20, 0x20, 0, arg0->sprite.index, 0);
}

// updateCourseUnlockPricePanel best match: 94.119% (nonmatchings/updateCourseUnlockPricePanel-1404502880690620360/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_shop_ui/updateCourseUnlockPricePanel.s")

#ifdef NON_MATCHING
void updateCourseUnlockPricePanel(ShopMenuWidgetActor *arg0) {
    s32 amount;
    s32 price;

    switch (arg0->slide.bytes.state) {
    case 0:
        arg0->sprite.index += 0x26;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->slide.bytes.state = 1;
        }
        break;
    case 1:
        arg0->item.price = gCourseUnlockPrices[D_80121D80.selectedShopItem];
        if (gCurrentGameTask->shopItemPrice >= 2) {
            arg0->slide.bytes.state = 2;
        }
        break;
    case 2:
        price = arg0->item.price;
        if ((u32)price < 100U) {
            amount = price & 0xFFFF;
        } else {
            amount = 500;
            if (gPlayerInputHeld & 0x8000) {
                if ((u32)price < 5000U) {
                    amount = price & 0xFFFF;
                } else {
                    amount = 5000;
                }
            }
        }
        arg0->item.price = price - amount;
        D_80121D80.money -= amount;
        if (arg0->item.price == 0) {
            arg0->slide.bytes.state = 3;
            gCourseUnlockSaveSlots[D_80121D80.selectedShopItem] = 9;
        }
        break;
    case 3:
        arg0->slide.bytes.timer++;
        if (arg0->slide.bytes.timer >= 0x14) {
            arg0->slide.bytes.timer = 0;
            arg0->slide.bytes.state = 4;
            gCurrentGameTask->shopItemPrice += 2;
        }
        break;
    case 4:
        if (D_80121D80.shopMenuState == 3) {
            arg0->slide.bytes.state = 5;
        }
        /* fallthrough */
    case 5:
        arg0->x += 0x20;
        if (arg0->x >= 0x94) {
            arg0->slide.bytes.state = 6;
        }
        break;
    }
    if (((gMenuChoicePromptState >= 5) && (gMenuChoicePromptState != 9)) || (arg0->slide.bytes.state == 6)) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCourseUnlockPricePanel, arg0);
}
#endif

void initCourseUnlockPricePanel(ShopMenuWidgetActor *arg0) {
    arg0->x = 0x20;
    arg0->y = -8;
    arg0->item.price = gCourseUnlockPrices[gCourseSelectSelectedCourseId];
    arg0->sprite.index = 0;
    arg0->slide.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCourseUnlockPricePanel);
}

// drawCourseUnlockPurchasePrompt best match: 98.264% (nonmatchings/drawCourseUnlockPurchasePrompt-6061209858023118177/base_15.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_shop_ui/drawCourseUnlockPurchasePrompt.s")

#ifdef NON_MATCHING
void drawCourseUnlockPurchasePrompt(ShopMenuWidgetActor *arg0) {
    s32 size;
    volatile u16 drawAlpha;
    s32 zero;
    s16 alpha;

    drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->x + 0x40), arg0->y, getRelocatableHeapBlockBase(gAssetHandles[0x24]), 1, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->x + 0x78), arg0->y, getRelocatableHeapBlockBase(gAssetHandles[0x24]), 1, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->x + 0xB0), arg0->y, getRelocatableHeapBlockBase(gAssetHandles[0x24]), 2, 0x20, 0x20, 0, 0);
    drawMenuGlyphScript((s16)(arg0->x + 0x30), (s16)(arg0->y + 4), &gCourseUnlockPurchasePromptText, 0, 0x100, 0);

    alpha = 0x60;
    if (arg0->item.price == 0) {
        alpha = 0x100;
    }

    drawAlpha = alpha;
    drawMenuSpriteWithAlpha((s16)(arg0->x + 0x50), (s16)(arg0->y + 0x14), getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0x17, 0x20,
                  0x20, 0, drawAlpha, 0);

    size = 0x20;
    zero = drawAlpha;
    if (zero == 0x100) {
        alpha = 0x60;
    } else {
        alpha = 0x100;
    }

    zero = 0;
    drawAlpha = alpha;
    drawMenuSpriteWithAlpha((s16)(arg0->x + 0x50), (s16)(arg0->y + 0x24), getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0x18, size,
                  size, zero, drawAlpha, zero);
    drawMenuSpriteWithAlpha((s16)(arg0->x + 0x50), (s16)(arg0->y + (arg0->item.price * 0x10) + 0x14),
                  getRelocatableHeapBlockBase(gAssetHandles[0x24]), 0x12, size, size, zero, arg0->sprite.index, zero);
}
#endif

void updateCourseUnlockPurchasePrompt(ShopMenuWidgetActor *arg0) {
    ShopMenuWidgetActor *temp_a2;

    arg0->item.price = gCurrentGameTask->shopItemPrice;
    temp_a2 = arg0;
    if (arg0->transition.counter < 0x10) {
        arg0->sprite.index -= 9;
    } else {
        temp_a2->sprite.index += 9;
    }
    temp_a2->transition.counter = (temp_a2->transition.counter + 1) & 0x1F;
    if (gMenuFlowState == 1) {
        removeCallbackTask(temp_a2);
        gMenuFlowState = 0;
    } else {
        addRenderCallback(&gMenuRenderCallbackList, drawCourseUnlockPurchasePrompt, temp_a2);
    }
}

void initCourseUnlockPurchasePrompt(ShopMenuWidgetActor *arg0) {
    arg0->x = -0x76;
    arg0->y = -0x40;
    arg0->item.price = 0;
    arg0->sprite.index = 0x100;
    arg0->transition.counter = 0;
    setCallbackTaskCallback(arg0, updateCourseUnlockPurchasePrompt);
}

// drawCourseDetailsMenu best match: 99.749% (nonmatchings/drawCourseDetailsMenu-5802343343535905907/base_22.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_shop_ui/drawCourseDetailsMenu.s")

#ifdef NON_MATCHING
void drawCourseDetailsMenu(ShopMenuWidgetActor *arg0) {
    volatile s32 unused;
    u16 script[3];
    u16 *tiles;
    s32 i;
    s32 yOffset;
    s16 *cursor;
    s32 value;
    s32 tens;
    s16 cursorX;

 tiles = gCourseDetailsMenuEntryTiles; yOffset = 0; cursor = (s16 *) arg0; for (i = 0; i < 7; i++) { drawMenuSprite(cursor[12], (s16) (arg0->targetY + yOffset), getRelocatableHeapBlockBase(gAssetHandles[0x27]), tiles[gCourseDetailsPreviewPage * 7], 0x20, 0x20, 0, 0); value = ((gCourseDetailsPreviewPage * 7) + i) + 1; if (arg0 && arg0) { } tens = value / 10; if (tens == 0) { script[0] = -2;
        } else {
            script[0] = tens;
        }
        script[2] = -1;
        script[1] = value % 10;
        drawMenuGlyphScriptDefaultFont((s16)(cursor[12] - 0x12), (s16)(arg0->targetY + yOffset), script, 1, 0x100);

        yOffset += 0x13;
        tiles++;
        cursor++;
    }

    i = 7;
    yOffset = 0x85;
    cursor = &((s16 *)arg0)[7];
    do {
        drawMenuSprite(cursor[12], (s16)(arg0->targetY + yOffset), getRelocatableHeapBlockBase(gAssetHandles[0x27]), (u16)i, 0x20,
                      0x20, 0, 0);

        i++;
        yOffset += 0x13;
        cursor++;
    } while (i != 10);

    if ((arg0->state != 0) && (gCourseDetailsCloseFromBack == 0)) {
        if (arg0->state < 5) {
            cursorX = arg0->targetX;
        } else {
            cursorX = arg0->cursorPositions[0];
        }
        drawMenuSpriteWithAlpha(cursorX, (s16)(arg0->targetY + (gCourseDetailsMenuSelection * 0x13)), getRelocatableHeapBlockBase(gAssetHandles[0x25]),
                      0x12, 0x20, 0x20, 0, arg0->prompt.bytes.pulseAlpha, 0);
    }
}
#endif

// updateCourseDetailsMenu best match: 85.277% (nonmatchings/updateCourseDetailsMenu-3357475854818838508/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_select_shop_ui/updateCourseDetailsMenu.s")

#ifdef NON_MATCHING
void updateCourseDetailsMenu(ShopMenuWidgetActor *arg0) {
    s32 i;
    s32 moved;
    u8 globalState;
    u8 state;
    ShopMenuWidgetActor *actor;
    ShopMenuWidgetActor *shifted;
    s16 target;
    s16 temp;
    u16 count;

    actor = arg0;
    shifted = arg0;
    globalState = gCourseDetailsMenuState;
    state = arg0->state;
    if (state != globalState) {
        state = globalState & 0xFF;
        arg0->state = globalState;
    }

    if (((gCurrentGameTask->unk20 == 3) || (gCurrentGameTask->unk20 == 9)) && (state < 5)) {
        arg0->state = 5;
        state = 5 & 0xFF;
        arg0->prompt.bytes.pulseAlpha = 0x100;
    }

    switch (state) {
    case 0:
        moved = 0;
        for (i = 0; i < (u16)arg0->visibleCount; i++) {
            if (arg0->randomValues[i] < arg0->targetX) {
                arg0->randomValues[i] += 0x10;
                moved++;
                if (arg0->randomValues[i] >= arg0->targetX) {
                    arg0->randomValues[i] = arg0->targetX;
                }
            }
        }
        arg0->spawnTimer++;
        if (!(arg0->spawnTimer & 1)) {
            count = shifted->visibleCount;
            if ((s32)count < 10) {
                arg0->visibleCount = count + 1;
                if ((u16)arg0->visibleCount == 10) {
                    createCallbackTask(initCourseDetailsPreviewTile, 0, 0x63);
                }
            }
        }
        if (moved == 0) {
            arg0->state = 1;
        }
        state = arg0->state;
        break;
    case 1:
        arg0->prompt.bytes.pulseAlpha += 0x26;
        if (arg0->prompt.bytes.pulseAlpha >= 0x100) {
            arg0->prompt.bytes.pulseAlpha = 0x100;
            arg0->state = 2;
        }
        state = arg0->state;
        break;
    case 3:
        actor = arg0;
        i = 3;
        actor->randomValues[0] -= 0x20;
        if (actor->randomValues[0] < -0x117) {
            actor->randomValues[0] = -0x118;
        }
        actor->randomValues[1] -= 0x20;
        shifted = (ShopMenuWidgetActor *)((s16 *)arg0 + i);
        if (actor->randomValues[1] < -0x117) {
            actor->randomValues[1] = -0x118;
        }
        actor->randomValues[2] -= 0x20;
        if (actor->randomValues[2] < -0x117) {
            actor->randomValues[2] = -0x118;
        }
        shifted->randomValues[0] -= 0x20;
        if (shifted->randomValues[0] < -0x117) {
            shifted->randomValues[0] = -0x118;
        }
        shifted->randomValues[1] -= 0x20;
        if (shifted->randomValues[1] < -0x117) {
            shifted->randomValues[1] = -0x118;
        }
        shifted->randomValues[2] -= 0x20;
        if (shifted->randomValues[2] < -0x117) {
            shifted->randomValues[2] = -0x118;
        }
        shifted->randomValues[3] -= 0x20;
        if (shifted->randomValues[3] < -0x117) {
            shifted->randomValues[3] = -0x118;
        }
        if (actor->randomValues[0] == -0x118) {
            arg0->state = 4;
            gCourseDetailsPreviewPage = (gCourseDetailsPreviewPage + 1) % 2;
        }
        state = arg0->state;
        break;
    case 4:
        actor = arg0;
        i = 3;
        target = arg0->targetX;
        actor->randomValues[0] += 0x20;
        if (actor->randomValues[0] >= target) {
            actor->randomValues[0] = target;
        }
        actor->randomValues[1] += 0x20;
        target = arg0->targetX;
        shifted = (ShopMenuWidgetActor *)((s16 *)arg0 + i);
        if (actor->randomValues[1] >= target) {
            actor->randomValues[1] = target;
        }
        actor->randomValues[2] += 0x20;
        target = arg0->targetX;
        if (actor->randomValues[2] >= target) {
            actor->randomValues[2] = target;
        }
        shifted->randomValues[0] += 0x20;
        target = arg0->targetX;
        if (shifted->randomValues[0] >= target) {
            shifted->randomValues[0] = target;
        }
        shifted->randomValues[1] += 0x20;
        target = arg0->targetX;
        if (shifted->randomValues[1] >= target) {
            shifted->randomValues[1] = target;
        }
        shifted->randomValues[2] += 0x20;
        target = arg0->targetX;
        if (shifted->randomValues[2] >= target) {
            shifted->randomValues[2] = target;
        }
        shifted->randomValues[3] += 0x20;
        target = arg0->targetX;
        if (shifted->randomValues[3] >= target) {
            shifted->randomValues[3] = target;
        }
        if (arg0->targetX == actor->randomValues[0]) {
            arg0->state = 2;
        }
        state = arg0->state;
        break;
    case 5:
        temp = arg0->randomValues[0] - 0x20;
        arg0->randomValues[1] -= 0x20;
        arg0->randomValues[0] = temp;
        shifted = (ShopMenuWidgetActor *)((char *)arg0 + 4);
        i = 2;
        do {
            shifted->randomValues[3] -= 0x20;
            shifted->randomValues[2] -= 0x20;
            shifted->randomValues[1] -= 0x20;
            temp = shifted->randomValues[0] - 0x20;
            i += 4;
            shifted = (ShopMenuWidgetActor *)((char *)shifted + 8);
            shifted->randomValues[-4] = temp;
        } while (i != 10);
        if (arg0->randomValues[0] < -0x117) {
            arg0->state = 7;
        }
        state = arg0->state;
        break;
    case 6:
        arg0->prompt.bytes.pulseAlpha = 0x100;
        arg0->prompt.bytes.pulseTimer = 0;
        state = arg0->state;
        break;
    case 7:
        break;
    }

    if ((state >= 2) && (state < 5)) {
        if ((s32)(u16)arg0->prompt.bytes.pulseTimer < 0x10) {
            arg0->prompt.bytes.pulseAlpha -= 9;
        } else {
            arg0->prompt.bytes.pulseAlpha += 9;
        }
        arg0->prompt.bytes.pulseTimer = (arg0->prompt.bytes.pulseTimer + 1) & 0x1F;
        state = arg0->state;
    }

    gCourseDetailsMenuState = state;
    if (arg0->state == 7) {
        removeCallbackTask((CallbackTask *)arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCourseDetailsMenu, actor);
}
#endif

void initCourseDetailsMenu(ShopMenuWidgetActor *arg0) {
    s32 i;

    for (i = 0; i < 10; i++) {
        arg0->cursorPositions[i] = -0xFC;
    }

    arg0->targetX = -0x7C;
    arg0->targetY = -0x5C;
    arg0->prompt.bytes.pulseTimer = 0;
    arg0->spawnTimer = 0;
    arg0->visibleCount = 1;
    arg0->prompt.bytes.pulseAlpha = 0;
    arg0->state = 0;
    setCallbackTaskCallback(arg0, updateCourseDetailsMenu);
}

void drawCourseDetailsPreviewTile(ShopMenuWidgetActor *arg0) {
    s32 unused;
    u16 tileIndex;

    if (gCourseDetailsMenuSelection < 7) {
        tileIndex = gCourseDetailsPreviewCourseTiles[gCourseDetailsPreviewPage * 7 + gCourseDetailsMenuSelection];
    } else {
        tileIndex = gCourseDetailsPreviewExtraTiles[gCourseDetailsPreviewPage * 7];
    }

    drawMenuSpriteSubrect(arg0->x, arg0->y, getRelocatableHeapBlockBase(gShopMenuTextureAssetHandle), tileIndex, 0, 0, 0x40, 0x1C, 0x20, 0x20);
    drawMenuSpriteSubrect((s16)(arg0->x + 0x40), arg0->y, getRelocatableHeapBlockBase(gShopMenuTextureAssetHandle), tileIndex, 0, 0x1C, 0x40, 0x1C, 0x20,
                  0x20);
    drawMenuSprite((s16)(arg0->x - 4), (s16)(arg0->y - 1), getRelocatableHeapBlockBase(gMenuPanelTilemapAssetHandle), 0xA, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->x + 0x3C), (s16)(arg0->y - 1), getRelocatableHeapBlockBase(gMenuPanelTilemapAssetHandle), 0xB, 0x20, 0x20, 1, 0);
}

void updateCourseDetailsPreviewTile(ShopMenuWidgetActor *arg0) {
    u8 state;

    if ((gCurrentGameTask->unk20 == 3) || (gCurrentGameTask->unk20 == 9)) {
        arg0->sprite.bytes.state = 2;
    }
    state = arg0->sprite.bytes.state;
    switch (state) {
    case 0:
        arg0->x -= 0x20;
        if (arg0->x < 5) {
            arg0->x = 4;
            arg0->sprite.bytes.state = 1;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 1:
        break;
    case 2:
        arg0->x += 0x20;
        if (arg0->x >= 0x90) {
            arg0->sprite.bytes.state = 3;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 3:
        state = arg0->sprite.bytes.state = 4;
        break;
    }
    if ((u32)state == 4) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCourseDetailsPreviewTile, arg0);
}

void initCourseDetailsPreviewTile(ShopMenuWidgetActor *arg0) {
    arg0->x = 0x90;
    arg0->y = 0x44;
    arg0->sprite.bytes.state = 0;
    setCallbackTaskCallback(arg0, updateCourseDetailsPreviewTile);
}

void drawCoursePreviewCloseSparkles(ShopMenuWidgetActor *arg0) {
    ShopMenuSparkleOffset *offset;
    ShopMenuWidgetActor *actor;
    ShopMenuWidgetActor *counterActor;

    actor = arg0; offset = gCoursePreviewCloseSparkleOffsets; counterActor = arg0; do {
        if (offset < gCoursePreviewCloseSparkleMirrorStart) {
            drawMenuSprite((s16)(offset->x + actor->x), (s16)(offset->y + actor->sprite.index - 8),
                          getRelocatableHeapBlockBase(gAssetHandles[0x1C]), (counterActor->transition.counter + 0x39) & 0xFFFF,
                          0x20, 0x20, 0, 0);
        } else {
            drawMenuSprite((s16)(offset[-6].x + actor->y), (s16)((offset[-6].y + actor->sprite.index) * -1 - 8),
                          getRelocatableHeapBlockBase(gAssetHandles[0x1C]), (counterActor->transition.counter + 0x39) & 0xFFFF,
                          0x20, 0x20, 0, 0);
        }
        offset++;
        counterActor = (ShopMenuWidgetActor *)&counterActor->pad0[2];
    } while (offset != gCoursePreviewCloseSparkleOffsetsEnd);
}

void updateCoursePreviewCloseSparkles(ShopMenuWidgetActor *arg0) {
    s32 i;

    arg0->sprite.index = gCoursePreviewViewportHeight / 2;
    arg0->x++;
    arg0->y--;

    for (i = 0; i < 12; i += 4) {
        arg0->sparkle.counters[i] = (arg0->sparkle.counters[i] + 1) % 6;
        arg0->sparkle.counters[i + 1] = (arg0->sparkle.counters[i + 1] + 1) % 6;
        arg0->sparkle.counters[i + 2] = (arg0->sparkle.counters[i + 2] + 1) % 6;
        arg0->sparkle.counters[i + 3] = (arg0->sparkle.counters[i + 3] + 1) % 6;
    }

    if (arg0->sprite.index == 0) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawCoursePreviewCloseSparkles, arg0);
}

void initCoursePreviewCloseSparkles(ShopMenuWidgetActor *arg0) {
    ShopMenuWidgetActor *new_var;
    s32 i;

    arg0->x = 5;
    arg0->y = 0x48;
    new_var = arg0;
    new_var->sprite.index = gCoursePreviewViewportHeight / 2;

    for (i = 0; i < 12; i++) {
        new_var->randomValues[i + 3] = randomNextMain() % 6;
    }

    setCallbackTaskCallback(new_var, updateCoursePreviewCloseSparkles);
}
