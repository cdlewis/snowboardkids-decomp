#ifndef COURSE_SELECT_MENU_H
#define COURSE_SELECT_MENU_H

#include "common.h"

#define COURSE_UNLOCK_PRICE_COUNT 12

#define COURSE_SELECT_PLAYER_COUNT 4
#define COURSE_SELECT_COLUMN_COUNT 4
#define COURSE_SELECT_EXTRA_COURSE_COUNT 3

typedef union {
    struct {
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
    };
    struct {
        /* 0x00 */ u8 previewModelState[4];
        /* 0x04 */ u8 incomingPreviewModelState[4];
        /* 0x08 */ u8 outgoingPreviewModelState[4];
        /* 0x0C */ u8 incomingPreviewModelTimer[4];
        /* 0x10 */ u8 outgoingPreviewModelTimer[4];
        /* 0x14 */ u16 incomingPreviewModelAngle[4];
        union {
            /* 0x1C */ u16 outgoingPreviewModelAngle[4];
            /* 0x1C */ u8 outgoingPreviewModelAngleBytes[8];
        };
        /* 0x24 */ u8 extraCourseCounts[4];
    };
    /* 0x00 */ u8 bytes[0x28];
} CourseSelectStatus;

typedef struct {
    CourseSelectStatus core;
    u8 cursorState;
    u8 submenuState;
    s16 cursorValue;
    union {
        s16 purchaseMessageState;
        u16 purchaseMessageStateUnsigned;
    };
    u8 extraCourseColumnState;
} CourseSelectStatusLayout;

#define COURSE_SELECT_STATUS_LAYOUT (*(CourseSelectStatusLayout *)&gCourseSelectStatus)

extern CourseSelectStatus gCourseSelectStatus;
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
