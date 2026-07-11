#ifndef RACE_TIMER_UI_H
#define RACE_TIMER_UI_H

#include "common.h"

typedef struct {
    /* 0x0 */ s8 minutes;
    /* 0x1 */ s8 seconds;
    /* 0x2 */ s16 fraction;
} RaceTimer;

void func_80078430(void);
void func_80078568(s32 arg0);
void func_80078974(s32 arg0);
void func_800789C0(s32 arg0);
void func_80078D3C(s32 arg0);
void func_80078D9C(s32 arg0);
void func_80079068(s32 arg0);
void func_80079154(s32 arg0);
void func_80079394(s32 arg0);
void func_80079438(s32 arg0);
void func_80079750(s32 arg0);
void func_80079758(s32 arg0);
void func_800799DC(s32 arg0);
void func_80079E48(s32 arg0);
void func_80079F04(s32 arg0);
void func_8007A108(s32 arg0);
void func_8007A278(s32 arg0);
void func_8007A2B8(void);
void func_8007A350(void);
void func_8007A3D8(s32 arg0);
void func_8007A8EC(void);
void func_8007AA50(void);
s32 func_8007B130(RaceTimer *arg0, RaceTimer *arg1, RaceTimer *arg2);

#endif
