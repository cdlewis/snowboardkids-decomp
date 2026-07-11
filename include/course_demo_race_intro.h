#ifndef COURSE_DEMO_RACE_INTRO_H
#define COURSE_DEMO_RACE_INTRO_H

#include "common.h"

typedef void (*TaskCallback)(void);
typedef void (*ActorCallback)(void *);

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s32 fadeDelay;
    /* 0x1C */ s32 fadeStep;
    /* 0x20 */ s32 courseSegment;
    /* 0x24 */ s32 startDelay;
} RaceIntroTransitionState;

void initCourseDemoRaceIntro(void);
void waitForCourseDemoRaceIntroStart(void);
void updateCourseDemoRaceIntro(void);
void finishCourseDemoRaceIntro(void);

#endif
