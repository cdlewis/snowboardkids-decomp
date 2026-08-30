#ifndef COURSE_SELECT_UI_H
#define COURSE_SELECT_UI_H

#include "common.h"
#include "game/menu/renderer/menu_renderer.h"

typedef struct {
    MenuGlyphScript text[0x18];
} CourseBoardLevelByCourseText;

typedef struct {
    MenuGlyphScript text[0x30];
} CourseExtraBoardLevelText;

typedef struct {
    MenuGlyphScript text[0x23];
} CourseModeDescriptionText;

typedef struct {
    MenuGlyphScript text[0x19];
} CoursePurchaseMessageText;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    union {
        struct {
            /* 0x18 */ s16 x;
            /* 0x1A */ s16 y;
            union {
                /* 0x1C */ s16 spriteIndex;
                /* 0x1C */ u16 alpha;
            };
            /* 0x1E */ s8 state;
            /* 0x1F */ u8 timer;
            /* 0x20 */ u8 subState;
        };
        struct {
            /* 0x18 */ u8 pad18[0x10];
            /* 0x28 */ s16 transitionOffset;
            /* 0x2A */ u8 transitionState;
        };
        struct {
            /* 0x18 */ u8 pad18_2[0x18];
            /* 0x30 */ u8 unk30;
            /* 0x31 */ u8 pad31[3];
            /* 0x34 */ u8 unk34;
            /* 0x35 */ u8 pad35[3];
            /* 0x38 */ s8 unk38;
        };
        struct {
            /* 0x18 */ s16 courseCursorX[4];
            /* 0x20 */ s16 courseCursorY[4];
            /* 0x28 */ s16 courseCursorAlpha[4];
            /* 0x30 */ u8 courseCursorState[4];
            /* 0x34 */ u8 courseCursorTimer[4];
            /* 0x38 */ s8 courseCursorBobOffset[4];
        };
        struct {
            /* 0x18 */ s16 courseStatsX[4];
            /* 0x20 */ s16 courseStatsY[4];
            /* 0x28 */ s16 courseStatsAlpha;
        };
        /* 0x18 */ s16 coordinates[0x22];
        /* 0x18 */ s16 coordinateRows[4][3];
    };
    union {
        /* 0x5C */ u8 itemCount;
        /* 0x5C */ u8 itemCounts[4];
    };
} CourseSelectWidgetActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    union {
        struct {
            /* 0x18 */ s16 unk18;
            /* 0x1A */ s16 unk1A;
            /* 0x1C */ s16 unk1C;
            /* 0x1E */ s16 unk1E;
            /* 0x20 */ s16 unk20;
            /* 0x22 */ s16 unk22;
            /* 0x24 */ s16 unk24;
            /* 0x26 */ s16 unk26;
            /* 0x28 */ s16 unk28;
            /* 0x2A */ s16 unk2A;
            /* 0x2C */ s16 unk2C;
            /* 0x2E */ s16 unk2E;
            /* 0x30 */ s16 unk30;
            /* 0x32 */ s16 unk32;
            /* 0x34 */ s16 unk34;
            /* 0x36 */ s16 unk36;
            /* 0x38 */ s16 unk38;
            /* 0x3A */ s16 unk3A;
            /* 0x3C */ s16 unk3C;
            /* 0x3E */ s16 unk3E;
            /* 0x40 */ s16 unk40;
            /* 0x42 */ s16 unk42;
            /* 0x44 */ s16 unk44;
            /* 0x46 */ s16 unk46;
            /* 0x48 */ s16 unk48;
            /* 0x4A */ s16 unk4A;
            /* 0x4C */ s16 unk4C;
            /* 0x4E */ s16 unk4E;
            /* 0x50 */ s16 unk50;
            /* 0x52 */ s16 unk52;
            /* 0x54 */ s16 unk54;
            /* 0x56 */ s16 unk56;
            /* 0x58 */ s16 unk58;
            /* 0x5A */ u8 unk5A;
            /* 0x5B */ u8 unk5B;
            /* 0x5C */ u8 unk5C;
            /* 0x5D */ u8 unk5D;
        };
        struct {
            /* 0x18 */ s16 x[4];
            /* 0x20 */ s16 y[4];
            /* 0x28 */ s16 targetX[4];
            /* 0x30 */ s16 deltaX[4];
            /* 0x38 */ s16 deltaY[4];
            union {
                /* 0x40 */ s16 exitTargetX;
                /* 0x40 */ s16 exitTargetXs[1];
            };
            /* 0x42 */ u16 period[4];
            /* 0x4A */ u16 speed[4];
            /* 0x52 */ u16 timer[4];
            /* 0x5A */ u8 state[4];
        };
    };
} CourseSelectWidgetInitActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ u16 playerPanelFadeAlpha[4];
} CourseSelectPlayerPanelsActor;

typedef struct CourseSelectIconListActor {
    /* 0x00 */ u8 pad0[0x18];
    union {
        /* 0x18 */ s16 coordinates[0x40];
        struct {
            /* 0x18 */ s16 y[4][5];
            /* 0x40 */ s16 x[4][5];
            /* 0x68 */ s16 baseX[4];
            /* 0x70 */ s16 direction[4];
            /* 0x78 */ s16 startY[4];
            /* 0x80 */ s16 targetY[4];
            /* 0x88 */ s16 targetX[4];
            /* 0x90 */ u8 state[4];
            /* 0x94 */ u8 timer[4];
        };
    };
    /* 0x98 */ u8 itemCounts[4];
    /* 0x9C */ u8 speed[4];
    /* 0xA0 */ s16 clipLeft;
    /* 0xA2 */ s16 clipRight;
    /* 0xA4 */ s16 clipTop;
    /* 0xA6 */ s16 clipBottom;
} CourseSelectIconListActor;

typedef union CourseSelectExtraCourseIconListActor {
    struct {
        /* 0x00 */ u8 pad0[0x18];
        /* 0x18 */ s16 iconX[4][3];
        /* 0x30 */ s16 iconY[4][3];
        /* 0x48 */ s16 clipLeft;
        /* 0x4A */ s16 clipRight;
        /* 0x4C */ s16 clipTop;
        /* 0x4E */ s16 clipBottom;
        /* 0x50 */ u8 rowSpacing[4];
        /* 0x54 */ u8 pulseTimer[4];
        /* 0x58 */ u8 revealTimer[4];
        /* 0x5C */ u8 itemCounts[4];
        /* 0x60 */ u16 alpha[4];
        /* 0x68 */ u16 tileIndices[4][3];
    };
    u8 bytes[0x80];
} CourseSelectExtraCourseIconListActor;

typedef struct CourseSelectAnimatedActor {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ Mtx *matrix;
    /* 0x01C */ Transform3D sourceTransform;
    /* 0x03C */ Transform3D playerTransforms[4];
    /* 0x0BC */ Vec3i vecs[4];
    /* 0x0EC */ u16 angle[4];
    /* 0x0F4 */ s16 targetCourse[4];
    /* 0x0FC */ u8 timer[4];
    /* 0x100 */ u8 state[4];
} CourseSelectAnimatedActor;

extern CourseBoardLevelByCourseText gCourseSelectBoardLevelByCourseText[4];
extern CourseExtraBoardLevelText gCourseSelectExtraCourseBoardLevelText[3];
extern CourseModeDescriptionText gCourseSelectModeDescriptionText[7];
extern CoursePurchaseMessageText gCourseSelectPurchaseMessageText[2];
extern MenuGlyphScript gCourseSelectBoardLevelText[];

void updateCourseSelectPreviewModelIn(CourseSelectAnimatedActor *arg0);
void initCourseSelectPreviewModelIn(void *arg0);
void updateCourseSelectPreviewModelOut(CourseSelectAnimatedActor *arg0);
void initCourseSelectPreviewModelOut(void *arg0);
void updateCourseSelectCourseIconList(CourseSelectIconListActor *arg0);
void initCourseSelectCourseIconList(CourseSelectIconListActor *arg0);
void initCourseSelectExtraCourseIconList(CourseSelectWidgetActor *arg0);
void updateCourseSelectCourseCursors(CourseSelectWidgetActor *arg0);
void initCourseSelectCourseCursors(CourseSelectWidgetActor *arg0);
void drawCourseSelectCourseListBackdrop(CourseSelectWidgetActor *arg0);
void updateCourseSelectCourseListBackdrop(CourseSelectWidgetActor *arg0);
void initCourseSelectCourseListBackdrop(CourseSelectWidgetActor *arg0);
void drawCourseSelectCourseStats(CourseSelectWidgetActor *arg0);
void updateCourseSelectCourseStats(CourseSelectWidgetActor *arg0);
void initCourseSelectCourseStats(CourseSelectWidgetActor *arg0);
void drawCourseSelectCourseDescription(CourseSelectWidgetActor *arg0);
void updateCourseSelectCourseDescription(CourseSelectWidgetActor *arg0);
void initCourseSelectCourseDescription(CourseSelectWidgetActor *arg0);
void drawCourseSelectExtraCourseBadge(CourseSelectWidgetActor *arg0);
void updateCourseSelectExtraCourseBadge(CourseSelectWidgetActor *arg0);
void initCourseSelectExtraCourseBadge(CourseSelectWidgetActor *arg0);
void drawCourseSelectExtraCourseIconList(CourseSelectExtraCourseIconListActor *arg0);
void updateCourseSelectExtraCourseIconListIn(CourseSelectWidgetActor *arg0);
void updateCourseSelectExtraCourseIconListOut(CourseSelectWidgetActor *arg0);
void updateCourseSelectExtraCourseIconListClose(CourseSelectWidgetActor *arg0);
void updateCourseSelectExtraCourseIconList(CourseSelectWidgetActor *arg0);
void drawCourseSelectPlayerPanels(CourseSelectWidgetActor *arg0);
void updateCourseSelectPlayerPanels(CourseSelectWidgetActor *arg0);
void initCourseSelectPlayerPanels(CourseSelectWidgetInitActor *arg0);
void drawCourseSelectCompletePanels(CourseSelectPlayerPanelsActor *actor);
void updateCourseSelectCompletePanels(CourseSelectPlayerPanelsActor *actor);
void initCourseSelectCompletePanels(CourseSelectWidgetActor *arg0);
void finishCourseSelectUiTask(s32 arg0);

#endif
