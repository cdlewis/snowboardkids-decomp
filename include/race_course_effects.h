#ifndef RACE_COURSE_EFFECTS_H
#define RACE_COURSE_EFFECTS_H

#include "common.h"

struct CourseEffectPlayer;
struct RaceCountdownEffect;
struct RaceCourseBackdropEffect;
struct RaceCourseMarkerEffect;
struct RaceCourseRenderEffect;
struct RaceCourseTriggerEffect;
struct RaceMovingEffect;
struct RacePlayerEffect;
struct Struct6B760;
struct Struct6C51C;

void func_80069890(struct RaceCountdownEffect *arg0);
void func_80069914(struct RaceCountdownEffect *arg0);
void func_80069998(struct RaceCountdownEffect *arg0);
void func_800699F0(struct RaceCountdownEffect *arg0);
void func_80069A78(struct RaceCountdownEffect *arg0);
void func_80069AF0(struct RaceCountdownEffect *arg0);
void func_80069B60(struct RaceCountdownEffect *arg0);
void func_80069BC0(struct RaceCountdownEffect *arg0);
void func_80069BEC(void *arg0);
void func_80069E50(struct RaceCourseBackdropEffect *arg0);
void func_8006A74C(void *arg0);
void func_8006A798(void *arg0);
void func_8006A7BC(struct RacePlayerEffect *arg0);
void func_8006A80C(void *arg0);
void func_8006A85C(void *arg0);
void func_8006A894(struct RaceCourseRenderEffect *arg0);
void func_8006ACE8(void *arg0);
void func_8006AE00(struct RaceCourseRenderEffect *arg0);
void func_8006AF48(struct RaceCourseRenderEffect *arg0);
void func_8006B0D8(void *arg0);
void func_8006B108(struct RaceCourseRenderEffect *arg0);
void func_8006B228(struct Struct6B760 *arg0);
void func_8006B3E0(struct Struct6B760 *arg0);
void func_8006B6C8(struct Struct6B760 *arg0);
void func_8006B760(s16 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_8006B7E0(struct RaceMovingEffect *arg0);
void func_8006B988(struct RaceMovingEffect *arg0);
void func_8006BA50(struct RaceMovingEffect *arg0);
void func_8006BB50(struct RaceMovingEffect *arg0);
void func_8006BC68(struct RaceMovingEffect *arg0);
void func_8006BDE4(struct RaceMovingEffect *arg0);
void func_8006BE90(struct RaceMovingEffect *arg0);
void func_8006BFC0(struct RaceMovingEffect *arg0);
void func_8006C088(struct RaceMovingEffect *arg0);
void func_8006C1B4(struct Struct6C51C *arg0);
void func_8006C4AC(struct Struct6C51C *arg0);
void func_8006C51C(struct Struct6C51C *arg0);
void func_8006C5C0(struct Struct6C51C *arg0);
void func_8006C698(struct Struct6C51C *arg0);
void func_8006C7F4(struct RaceCourseMarkerEffect *arg0);
void func_8006CB50(struct RaceCourseMarkerEffect *arg0);
void func_8006CBBC(struct RaceCourseMarkerEffect *arg0);
void func_8006CCC0(struct RaceCourseTriggerEffect *arg0);
void func_8006CE68(struct CourseEffectPlayer *player, struct RaceCourseTriggerEffect *trigger);
void func_8006D2D0(struct RaceCourseTriggerEffect *arg0);
void func_8006D384(struct RaceCourseTriggerEffect *arg0);

#endif
