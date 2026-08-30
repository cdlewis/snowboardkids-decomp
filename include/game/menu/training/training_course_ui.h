#ifndef TRAINING_COURSE_UI_H
#define TRAINING_COURSE_UI_H

#include "common.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/renderer/menu_renderer.h"

typedef MenuGlyphScript TrainingCourseUiScript;

typedef struct TrainingCourseUiActor {
    /* 0x00 */ CallbackTaskHeader task;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    union {
        struct {
            /* 0x1C */ s16 x;
            /* 0x1E */ s16 y;
        } textOffset;
        /* 0x1C */ TrainingCourseUiScript *script;
        /* 0x1C */ s32 panelHeight;
    } layout;
    /* 0x20 */ s16 highlightScale;
    /* 0x22 */ s16 highlightTimer;
    union {
        /* 0x24 */ s16 selectedAction;
        struct {
            /* 0x24 */ u8 visibleGlyphCount;
            /* 0x25 */ u8 pad25;
        } script;
    } state;
    /* 0x26 */ u16 glyphPalette;
    /* 0x28 */ u16 glyphTimer;
    /* 0x2A */ u8 scriptState;
    /* 0x2B */ u8 confirmBlinkTimer;
    /* 0x2C */ u8 keepVisibleAfterConfirm;
} TrainingCourseUiActor;

void initTrainingCourseLessonEndMenu(TrainingCourseUiActor *arg0);
void initTrainingCourseOpeningDialog(TrainingCourseUiActor *arg0);
void initTrainingCourseEndingDialog(TrainingCourseUiActor *arg0);
void initTrainingCourseLessonTitlePanel(TrainingCourseUiActor *arg0);

#endif
