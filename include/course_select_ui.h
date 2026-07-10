#ifndef COURSE_SELECT_UI_H
#define COURSE_SELECT_UI_H

#include "common.h"

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

void func_80023A68(void *arg0);
void func_80024050(void *arg0);
void func_80024380(void);
void func_80024968(void *arg0);
void func_80025BE4(CourseSelectWidgetActor *arg0);
void func_80025E6C(CourseSelectWidgetActor *arg0);
void func_80025FA8(CourseSelectWidgetActor *arg0);
void func_800260E8(CourseSelectWidgetActor *arg0);
void func_800263A4(CourseSelectWidgetActor *arg0);
void func_800263D8(CourseSelectWidgetActor *arg0);
void func_80026A54(CourseSelectWidgetActor *arg0);
void func_80026B88(CourseSelectWidgetActor *arg0);
void func_800271CC(CourseSelectWidgetActor *arg0);
void func_800273C4(CourseSelectWidgetActor *arg0);
void func_80027408(CourseSelectWidgetActor *arg0);
void func_80027498(CourseSelectWidgetActor *arg0);
void func_800275A4(CourseSelectWidgetActor *arg0);
void func_800275E0(CourseSelectWidgetActor *arg0);
void func_8002784C(CourseSelectWidgetActor *arg0);
void func_80027914(CourseSelectWidgetActor *arg0);
void func_80027A08(CourseSelectWidgetActor *arg0);
void func_80027AF8(CourseSelectWidgetActor *arg0);
void func_800287EC(CourseSelectWidgetActor *arg0);
void func_80028C68(CourseSelectPlayerPanelsActor *actor);
void func_80028FF0(CourseSelectWidgetActor *arg0);
void func_8002916C(CourseSelectWidgetActor *arg0);
void func_800291F0(s32 arg0);

#endif
