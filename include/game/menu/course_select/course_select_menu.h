#ifndef COURSE_SELECT_MENU_H
#define COURSE_SELECT_MENU_H

#include "common.h"

#define COURSE_UNLOCK_PRICE_COUNT 12

#define COURSE_SELECT_PLAYER_COUNT 4
#define COURSE_SELECT_COLUMN_COUNT 4
#define COURSE_SELECT_EXTRA_COURSE_COUNT 3

typedef struct {
    /* 0x00 */ u8 previewModelState[COURSE_SELECT_PLAYER_COUNT];
    /* 0x04 */ u8 incomingPreviewModelState[COURSE_SELECT_PLAYER_COUNT];
    /* 0x08 */ u8 outgoingPreviewModelState[COURSE_SELECT_PLAYER_COUNT];
    /* 0x0C */ u8 incomingPreviewModelTimer[COURSE_SELECT_PLAYER_COUNT];
    /* 0x10 */ u8 outgoingPreviewModelTimer[COURSE_SELECT_PLAYER_COUNT];
    /* 0x14 */ u16 incomingPreviewModelAngle[COURSE_SELECT_PLAYER_COUNT];
    /* 0x1C */ u16 outgoingPreviewModelAngle[COURSE_SELECT_PLAYER_COUNT];
    /* 0x24 */ u8 extraCourseCounts[COURSE_SELECT_PLAYER_COUNT];
} CourseSelectStatus;

typedef struct {
    CourseSelectStatus core;
    u8 cursorState;
    u8 submenuState;
    s16 cursorValue;
    u16 purchaseMessageState;
    u8 extraCourseColumnState;
} CourseSelectStatusLayout;

#define COURSE_SELECT_STATUS_LAYOUT (*(CourseSelectStatusLayout *)&gCourseSelectStatus)

extern u8 gCourseSelectPreviewModelState0;
extern u8 gCourseSelectPreviewModelState1;
extern u8 gCourseSelectPreviewModelState2;
extern u8 gCourseSelectPreviewModelState3;
extern u8 gCourseSelectIncomingModelState0;
extern u8 gCourseSelectIncomingModelState1;
extern u8 gCourseSelectIncomingModelState2;
extern u8 gCourseSelectIncomingModelState3;
extern u8 gCourseSelectOutgoingModelState0;
extern u8 gCourseSelectOutgoingModelState1;
extern u8 gCourseSelectOutgoingModelState2;
extern u8 gCourseSelectOutgoingModelState3;
extern u8 gCourseSelectIncomingModelTimer0;
extern u8 gCourseSelectIncomingModelTimer1;
extern u8 gCourseSelectIncomingModelTimer2;
extern u8 gCourseSelectIncomingModelTimer3;
extern u8 gCourseSelectOutgoingModelTimer0;
extern u8 gCourseSelectOutgoingModelTimer1;
extern u8 gCourseSelectOutgoingModelTimer2;
extern u8 gCourseSelectOutgoingModelTimer3;
extern s16 gCourseSelectIncomingModelAngle0;
extern s16 gCourseSelectIncomingModelAngle1;
extern s16 gCourseSelectIncomingModelAngle2;
extern s16 gCourseSelectIncomingModelAngle3;
extern s16 gCourseSelectOutgoingModelAngle0;
extern s16 gCourseSelectOutgoingModelAngle1;
extern s16 gCourseSelectOutgoingModelAngle2;
extern s16 gCourseSelectOutgoingModelAngle3;
extern u8 gCourseSelectExtraCourseCount0;
extern u8 gCourseSelectExtraCourseCount1;
extern u8 gCourseSelectExtraCourseCount2;
extern u8 gCourseSelectExtraCourseCount3;

#define gCourseSelectStatus (*(CourseSelectStatus *)&gCourseSelectPreviewModelState0)
extern u8 gCourseSelectHasExtraCourse[COURSE_SELECT_PLAYER_COUNT];
extern u8 gCourseSelectSelectionTimers[COURSE_SELECT_PLAYER_COUNT];
extern u8 gCourseSelectPurchaseFlowActive;
extern u8 gCourseSelectSelectedRows[COURSE_SELECT_PLAYER_COUNT];
extern u8 gCourseSelectInputLocked;
extern s8 gCourseSelectColumnUnlocked[COURSE_SELECT_PLAYER_COUNT][COURSE_SELECT_EXTRA_COURSE_COUNT];
extern u8 gCourseSelectColumnCounts[COURSE_SELECT_PLAYER_COUNT];
extern u8 gCourseSelectSlideStates[COURSE_SELECT_PLAYER_COUNT];
extern u16 gCourseSelectViewportSyncState;
extern s32 gCourseSelectVerticalOffsets[COURSE_SELECT_PLAYER_COUNT];
extern s32 gCourseSelectHorizontalOffsets[COURSE_SELECT_PLAYER_COUNT];
extern u8 gCourseSelectCourseIds[COURSE_SELECT_PLAYER_COUNT][COURSE_SELECT_COLUMN_COUNT];
extern u8 gCourseSelectExtraCourseIds[COURSE_SELECT_PLAYER_COUNT][COURSE_SELECT_EXTRA_COURSE_COUNT];
extern u8 gCourseSelectCursorState;
extern u8 gCourseSelectSubmenuState;
extern s16 gCourseSelectCursorValue;
extern u16 gCourseSelectPurchaseMessageState;
extern u8 gCourseSelectExtraCourseColumnState;
extern u32 gCourseUnlockPrices[COURSE_UNLOCK_PRICE_COUNT];
extern s32 gMenuInputHeld;
extern s8 gCourseSelectHighlightedColumn;
extern u8 gCourseSelectSelectedCourseId;
extern u8 gCourseSelectModeSelection;
extern u8 gCourseDetailsMenuSelection;
extern u8 gCourseDetailsPreviewPage;
extern u8 gCourseDetailsCloseFromBack;

void initCourseSelectMenu(void);
void updateCourseSelectModeMenu(void);
void initCourseSelectCourseList(void);
void updateCourseSelectCourseList(void);
void updateCourseSelectPurchasePrompt(void);
void updateCourseSelectUnlockCourseList(void);
void initCourseSelectCourseDetailsMenu(void);
void updateCourseSelectCourseDetailsMenu(void);
void waitCourseSelectRecordsClose(void);
void returnToCourseSelectUnlockCourseList(void);
void returnToCourseSelectModeMenu(void);
void initCourseSelectPreview(void);
void updateCourseSelectPreviewClose(void);
void exitCourseSelectMenu(void);

#endif
