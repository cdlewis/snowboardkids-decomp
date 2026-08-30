#ifndef COURSE_SELECT_UI_H
#define COURSE_SELECT_UI_H

#include "common.h"
#include "game/engine/callback_task_scheduler.h"
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
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x[4];
    /* 0x20 */ s16 y[4];
    /* 0x28 */ s16 alpha[4];
    /* 0x30 */ u8 state[4];
    /* 0x34 */ u8 pulseTimer[4];
    /* 0x38 */ s8 horizontalSpacing[4];
} CourseSelectCourseCursorsActor;

typedef struct {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ u8 state;
} CourseSelectCourseListBackdropActor;

typedef struct {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x[4];
    /* 0x20 */ s16 y[4];
    /* 0x28 */ s16 alpha;
    /* 0x2A */ u8 state;
} CourseSelectCourseStatsActor;

typedef struct {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 alpha;
    /* 0x1E */ u8 state;
    /* 0x1F */ u8 selectedBoardLevel;
    /* 0x20 */ u8 selectionLocked;
} CourseSelectCourseDescriptionActor;

typedef struct {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 alpha;
    /* 0x1E */ s8 state;
} CourseSelectExtraCourseBadgeActor;

typedef struct {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x[4];
    /* 0x20 */ s16 y[4];
    /* 0x28 */ s16 targetX[4];
    /* 0x30 */ s16 deltaX[4];
    /* 0x38 */ s16 deltaY[4];
    /* 0x40 */ s16 exitTargetX;
    /* 0x42 */ u16 period[4];
    /* 0x4A */ u16 speed[4];
    /* 0x52 */ u16 timer[4];
    /* 0x5A */ u8 state[4];
} CourseSelectPlayerPanelListActor;

typedef struct {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ u16 playerPanelFadeAlpha[4];
} CourseSelectCompletePanelsActor;

typedef struct CourseSelectIconListActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 y[4][5];
    /* 0x40 */ s16 x[4][5];
    /* 0x68 */ s16 baseX[4];
    /* 0x70 */ s16 direction[4];
    /* 0x78 */ s16 startY[4];
    /* 0x80 */ s16 targetY[4];
    /* 0x88 */ s16 targetX[4];
    /* 0x90 */ u8 state[4];
    /* 0x94 */ u8 timer[4];
    /* 0x98 */ u8 itemCounts[4];
    /* 0x9C */ u8 speed[4];
    /* 0xA0 */ s16 clipLeft;
    /* 0xA2 */ s16 clipRight;
    /* 0xA4 */ s16 clipTop;
    /* 0xA6 */ s16 clipBottom;
} CourseSelectIconListActor;

typedef struct CourseSelectExtraCourseIconListActor {
    /* 0x00 */ CallbackTaskHeader task;
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
} CourseSelectExtraCourseIconListActor;

typedef struct CourseSelectAnimatedActor {
    /* 0x000 */ CallbackTaskHeader task;
    /* 0x018 */ Mtx *renderMatrix;
    /* 0x01C */ Transform3D viewTransform;
    /* 0x03C */ Transform3D modelTransforms[4];
    /* 0x0BC */ Vec3i modelOffsets[4];
    /* 0x0EC */ u16 rotationAngle[4];
    /* 0x0F4 */ s16 targetCourseIndex[4];
    /* 0x0FC */ u8 transitionTimer[4];
    /* 0x100 */ u8 transitionState[4];
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
void initCourseSelectExtraCourseIconList(CourseSelectExtraCourseIconListActor *arg0);
void updateCourseSelectCourseCursors(CourseSelectCourseCursorsActor *arg0);
void initCourseSelectCourseCursors(CourseSelectCourseCursorsActor *arg0);
void drawCourseSelectCourseListBackdrop(CourseSelectCourseListBackdropActor *arg0);
void updateCourseSelectCourseListBackdrop(CourseSelectCourseListBackdropActor *arg0);
void initCourseSelectCourseListBackdrop(CourseSelectCourseListBackdropActor *arg0);
void drawCourseSelectCourseStats(CourseSelectCourseStatsActor *arg0);
void updateCourseSelectCourseStats(CourseSelectCourseStatsActor *arg0);
void initCourseSelectCourseStats(CourseSelectCourseStatsActor *arg0);
void drawCourseSelectCourseDescription(CourseSelectCourseDescriptionActor *arg0);
void updateCourseSelectCourseDescription(CourseSelectCourseDescriptionActor *arg0);
void initCourseSelectCourseDescription(CourseSelectCourseDescriptionActor *arg0);
void drawCourseSelectExtraCourseBadge(CourseSelectExtraCourseBadgeActor *arg0);
void updateCourseSelectExtraCourseBadge(CourseSelectExtraCourseBadgeActor *arg0);
void initCourseSelectExtraCourseBadge(CourseSelectExtraCourseBadgeActor *arg0);
void drawCourseSelectExtraCourseIconList(CourseSelectExtraCourseIconListActor *arg0);
void updateCourseSelectExtraCourseIconListIn(CourseSelectExtraCourseIconListActor *arg0);
void updateCourseSelectExtraCourseIconListOut(CourseSelectExtraCourseIconListActor *arg0);
void updateCourseSelectExtraCourseIconListClose(CourseSelectExtraCourseIconListActor *arg0);
void updateCourseSelectExtraCourseIconList(CourseSelectExtraCourseIconListActor *arg0);
void drawCourseSelectPlayerPanels(CourseSelectPlayerPanelListActor *arg0);
void updateCourseSelectPlayerPanels(CourseSelectPlayerPanelListActor *arg0);
void initCourseSelectPlayerPanels(CourseSelectPlayerPanelListActor *arg0);
void drawCourseSelectCompletePanels(CourseSelectCompletePanelsActor *actor);
void updateCourseSelectCompletePanels(CourseSelectCompletePanelsActor *actor);
void initCourseSelectCompletePanels(CourseSelectCompletePanelsActor *arg0);
void finishCourseSelectUiTask(s32 arg0);

#endif
