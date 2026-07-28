#ifndef CHARACTER_SELECT_COURSE_UI_H
#define CHARACTER_SELECT_COURSE_UI_H

#include "common.h"

typedef union CharacterSelectCourseCursorState {
    u8 bytes[8];
    struct {
        /* 0x00 */ u8 state;
        /* 0x01 */ u8 otherState;
        /* 0x02 */ u8 pad2[2];
        /* 0x04 */ s16 spriteIndex;
        /* 0x06 */ u8 timer;
        /* 0x07 */ u8 otherTimer;
    } fields;
} CharacterSelectCourseCursorState;

typedef struct CharacterSelectCourseWidgetActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ union {
        /* 0x1C */ s16 index;
        /* 0x1C */ u16 unsignedIndex;
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
        /* 0x1E */ u16 alpha;
        /* 0x1E */ s16 signedAlpha;
    } transition;
    union {
        struct {
            /* 0x20 */ u8 rowSpacing;
            /* 0x21 */ u8 pad21;
        } bytes;
        /* 0x20 */ s16 counter;
        /* 0x20 */ u16 unsignedCounter;
    } selection;
    union {
        struct {
            /* 0x22 */ u8 subState;
            /* 0x23 */ u8 subTimer;
        } bytes;
        /* 0x22 */ s16 value;
    } row;
} CharacterSelectCourseWidgetActor;

typedef struct CharacterSelectCourseMenuFrameActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x[11];
    /* 0x2E */ s16 y[11];
    /* 0x44 */ s16 baseY;
    /* 0x46 */ u8 state;
    /* 0x47 */ u8 timer;
    /* 0x48 */ u8 itemCount;
    /* 0x49 */ u8 itemSpacing;
} CharacterSelectCourseMenuFrameActor;

typedef struct CharacterSelectCourseRecordTime {
    /* 0x00 */ s8 minutes;
    /* 0x01 */ s8 seconds;
    /* 0x02 */ s16 centiseconds;
} CharacterSelectCourseRecordTime;

extern CharacterSelectCourseCursorState gCharacterSelectCourseCursorState;

void drawCharacterSelectCourseListOptions(CharacterSelectCourseMenuFrameActor *arg0);
void updateCharacterSelectUnlockedCourseList(CharacterSelectCourseMenuFrameActor *arg0);
void initCharacterSelectUnlockedCourseList(CharacterSelectCourseMenuFrameActor *arg0);
void drawCharacterSelectLimitedCourseListOptions(CharacterSelectCourseMenuFrameActor *arg0);
void updateCharacterSelectLimitedCourseList(CharacterSelectCourseMenuFrameActor *arg0);
void initCharacterSelectLimitedCourseList(CharacterSelectCourseMenuFrameActor *arg0);
void drawCharacterSelectCoursePreviewFrame(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCoursePreviewFrame(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCoursePreviewFrame(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCoursePreviewPanel1(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCoursePreviewPanel1(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCoursePreviewPanel1(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCoursePreviewPanel2(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCoursePreviewPanel2(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCoursePreviewPanel2(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCoursePreviewPanel3(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCoursePreviewPanel3(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCoursePreviewPanel3(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCoursePreviewPanel4(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCoursePreviewPanel4(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCoursePreviewPanel4(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCoursePreviewPanel5(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCoursePreviewPanel5(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCoursePreviewPanel5(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCoursePreviewPanel6(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCoursePreviewPanel6(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCoursePreviewPanel6(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCoursePreviewPanel7(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCoursePreviewPanel7(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCoursePreviewPanel7(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCoursePreviewPanel8(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCoursePreviewPanel8(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCoursePreviewPanel8(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCoursePreviewPanel9(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCoursePreviewPanel9(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCoursePreviewPanel9(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCourseExitPreviewPanel(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCourseExitPreviewPanel(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCourseExitPreviewPanel(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCourseListCursor(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCourseListCursor(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCourseListCursor(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCourseTitleCursor(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCourseTitleCursor(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCourseTitleCursor(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCourseStatsBadge(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCourseStatsBadge(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCourseStatsBadge(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCoursePlayerStatsPanel(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCoursePlayerStatsPanel(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCoursePlayerStatsPanel(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCourseSubmenuFrame(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCourseSubmenuFrame(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCourseSubmenuFrame(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCourseRecordsFrame(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCourseRecordsFrame(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCourseRecordsFrame(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCourseNamePopup(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCourseNamePopup(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCourseNamePopup(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCourseDescriptionPopup(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCourseDescriptionPopup(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCourseDescriptionPopup(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCourseConfirmCursor(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCourseConfirmCursor(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCourseConfirmCursor(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCourseRecordsPopup(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCourseRecordsPopup(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCourseRecordsPopup(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCourseExitPopup(CharacterSelectCourseWidgetActor *arg0);
void updateCharacterSelectCourseExitPopup(CharacterSelectCourseWidgetActor *arg0);
void initCharacterSelectCourseExitPopup(CharacterSelectCourseWidgetActor *arg0);
void drawCharacterSelectCourseRecordTime(CharacterSelectCourseRecordTime *arg0, s32 x, s32 y, s32 alpha);

#endif
