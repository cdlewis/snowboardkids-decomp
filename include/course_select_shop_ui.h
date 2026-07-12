#ifndef COURSE_SELECT_SHOP_UI_H
#define COURSE_SELECT_SHOP_UI_H

#include "common.h"

typedef struct ShopMenuRowActor ShopMenuRowActor;
typedef struct ShopMenuWidgetActor ShopMenuWidgetActor;

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
