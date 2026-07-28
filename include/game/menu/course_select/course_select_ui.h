#ifndef COURSE_SELECT_UI_H
#define COURSE_SELECT_UI_H

#include "common.h"
#include "game/menu/renderer/menu_renderer.h"

typedef struct {
    MenuGlyphScript text[0x18];
} CourseBoardLevelByCourseText;

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
            /* 0x1F */ s8 timer;
            /* 0x20 */ s8 subState;
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
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ u16 playerPanelFadeAlpha[4];
} CourseSelectPlayerPanelsActor;

typedef struct CourseSelectIconListActor CourseSelectIconListActor;
typedef struct CourseSelectAnimatedActor CourseSelectAnimatedActor;

extern CourseBoardLevelByCourseText gCourseSelectBoardLevelByCourseText[4];

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
void drawCourseSelectExtraCourseIconList(CourseSelectWidgetActor *arg0);
void updateCourseSelectExtraCourseIconListIn(CourseSelectWidgetActor *arg0);
void updateCourseSelectExtraCourseIconListOut(CourseSelectWidgetActor *arg0);
void updateCourseSelectExtraCourseIconListClose(CourseSelectWidgetActor *arg0);
void updateCourseSelectExtraCourseIconList(CourseSelectWidgetActor *arg0);
void updateCourseSelectPlayerPanels(CourseSelectWidgetActor *arg0);
void drawCourseSelectCompletePanels(CourseSelectPlayerPanelsActor *actor);
void updateCourseSelectCompletePanels(CourseSelectPlayerPanelsActor *actor);
void initCourseSelectCompletePanels(CourseSelectWidgetActor *arg0);
void finishCourseSelectUiTask(s32 arg0);

#endif
