#ifndef COURSE_SELECT_SHOP_UI_H
#define COURSE_SELECT_SHOP_UI_H

#include "common.h"

typedef struct ShopMenuRowActor {
    char pad0[0x18];
    /* 0x18 */ s16 rowXPositions[5];
    /* 0x22 */ s16 baseY;
    /* 0x24 */ u8 state;
    /* 0x25 */ u8 revealTimer;
    /* 0x26 */ u8 visibleRowCount;
} ShopMenuRowActor;

typedef struct ShopMenuWidgetActor {
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
} ShopMenuWidgetActor;

void drawShopMenuModeChoiceRows(ShopMenuRowActor *arg0);
void updateShopMenuModeChoiceRows(ShopMenuRowActor *arg0);
void initShopMenuModeChoiceRows(ShopMenuRowActor *arg0);
void drawShopMenuSidePanel(ShopMenuWidgetActor *arg0);
void updateShopMenuSidePanel(ShopMenuWidgetActor *arg0);
void initShopMenuSidePanel(ShopMenuWidgetActor *arg0);
void drawShopMenuSelectedModePanel(ShopMenuWidgetActor *arg0);
void updateShopMenuSelectedModePanel(ShopMenuWidgetActor *arg0);
void initShopMenuSelectedModePanel(ShopMenuWidgetActor *arg0);
void drawShopMenuUnselectedModePanel(ShopMenuWidgetActor *arg0);
void updateShopMenuUnselectedModePanel(ShopMenuWidgetActor *arg0);
void initShopMenuUnselectedModePanel(ShopMenuWidgetActor *arg0);
void drawShopMenuCourseListPanel(ShopMenuWidgetActor *arg0);
void updateShopMenuCourseListPanel(ShopMenuWidgetActor *arg0);
void initShopMenuCourseListPanel(ShopMenuWidgetActor *arg0);
void drawShopMenuModeCursor(ShopMenuWidgetActor *arg0);
void updateShopMenuModeCursor(ShopMenuWidgetActor *arg0);
void initShopMenuModeCursor(ShopMenuWidgetActor *arg0);
void drawShopMenuDescriptionText(ShopMenuWidgetActor *arg0);
void updateShopMenuDescriptionText(ShopMenuWidgetActor *arg0);
void initShopMenuDescriptionText(ShopMenuWidgetActor *arg0);
void drawShopMenuPromptPanel(ShopMenuWidgetActor *arg0);
void updateShopMenuPromptPanel(ShopMenuWidgetActor *arg0);
void initShopMenuPromptPanel(ShopMenuWidgetActor *arg0);
void drawShopMenuMoneyPanel(ShopMenuWidgetActor *arg0);
void updateShopMenuMoneyPanel(ShopMenuWidgetActor *arg0);
void initShopMenuMoneyPanel(ShopMenuWidgetActor *arg0);
void updateShopMenuMoneyPanelForCourseSelectReturn(ShopMenuWidgetActor *arg0);
void initShopMenuMoneyPanelForCourseSelectReturn(ShopMenuWidgetActor *arg0);
void drawShopMenuSparkles(ShopMenuWidgetActor *arg0);
void updateShopMenuSparkles(ShopMenuWidgetActor *arg0);
void initShopMenuSparkles(ShopMenuWidgetActor *arg0);
void drawCourseUnlockPricePanel(ShopMenuWidgetActor *arg0);
void updateCourseUnlockPricePanel(ShopMenuWidgetActor *arg0);
void initCourseUnlockPricePanel(ShopMenuWidgetActor *arg0);
void drawCourseUnlockPurchasePrompt(ShopMenuWidgetActor *arg0);
void updateCourseUnlockPurchasePrompt(ShopMenuWidgetActor *arg0);
void initCourseUnlockPurchasePrompt(ShopMenuWidgetActor *arg0);
void drawCourseDetailsMenu(ShopMenuWidgetActor *arg0);
void updateCourseDetailsMenu(ShopMenuWidgetActor *arg0);
void initCourseDetailsMenu(ShopMenuWidgetActor *arg0);
void drawCourseDetailsPreviewTile(ShopMenuWidgetActor *arg0);
void updateCourseDetailsPreviewTile(ShopMenuWidgetActor *arg0);
void initCourseDetailsPreviewTile(ShopMenuWidgetActor *arg0);
void drawCoursePreviewCloseSparkles(ShopMenuWidgetActor *arg0);
void updateCoursePreviewCloseSparkles(ShopMenuWidgetActor *arg0);
void initCoursePreviewCloseSparkles(ShopMenuWidgetActor *arg0);

#endif
