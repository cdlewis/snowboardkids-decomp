#ifndef COURSE_SELECT_MENU_H
#define COURSE_SELECT_MENU_H

#include "common.h"

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

extern CourseSelectStatus gCourseSelectStatus;
extern u8 gCourseDetailsMenuState;
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
