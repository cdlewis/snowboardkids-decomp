#include "common.h"

#define END_OBJ_FADE_MAX 0x100
#define END_OBJ_FADE_STEP 0xA
#define END_OBJ_RESET_DELAY 0x20
#define END_OBJ_VISIBLE_FRAMES 0x96
#define END_OBJ_HOLD_CYCLES 0x19

typedef struct {
    /* 0x00 */ char pad[0x1C];
    /* 0x1C */ s16 x;
    /* 0x1E */ s16 y;
    /* 0x20 */ u16 cycleCount;
    /* 0x22 */ u16 timer;
    /* 0x24 */ s16 alpha;
    /* 0x26 */ u8 state;
} EndObjTask;

typedef struct {
    /* 0x00 */ char pad[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 objectId;
    /* 0x1E */ u8 enabled;
    /* 0x1F */ u8 palette;
} DebugObjectPositionTask;

extern u16 D_8010B1A2;
extern s16 D_80112172;
extern void *D_80124868;
extern char D_800E1060[];
extern s32 func_80043040(s16);
extern void func_8000F8AC(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_8000F030(s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_80013D0C(s32, s32, void *, s32, s32);
extern void func_800483FC(void *, void *, void *);
extern void func_80071824(void *task, void (*callback)());
extern int sprintf(char *, const char *, ...);

void func_8003B7C0(EndObjTask *arg0);
void func_8003B9F8(EndObjTask *arg0);
void func_8003BA64(EndObjTask *arg0);
void func_8003BC9C(DebugObjectPositionTask *arg0);

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_debug_ui/func_8003B7C0.s")

void func_8003B944(EndObjTask *arg0) {
    func_8000F8AC(arg0->x, arg0->y, func_80043040(D_80112172), 0x35, 0x20, 0x20, 0, arg0->alpha, 0);
    func_8000F8AC((s16)(arg0->x + 0x40), arg0->y, func_80043040(D_80112172), 0x36, 0x20, 0x20, 0,
                  arg0->alpha, 0);
}

void func_8003B9F8(EndObjTask *arg0) {
    s32 v1 = END_OBJ_FADE_MAX;
    s32 v0;

    if (D_8010B1A2 == 0x43) {
        v0 = arg0->alpha;
        if (v1 != v0) {
            arg0->alpha = v0 + 0x10;
            if (arg0->alpha >= END_OBJ_FADE_MAX) {
                arg0->alpha = v1;
            }
        }
        func_800483FC(&D_80124868, func_8003B944, arg0);
    }
}

void func_8003BA64(EndObjTask *arg0) {
    switch (arg0->state) {
    case 0:
        arg0->alpha += END_OBJ_FADE_STEP;
        if (!(arg0->alpha < END_OBJ_FADE_MAX)) {
            arg0->alpha = END_OBJ_FADE_MAX;
            arg0->state = 1;
            arg0->timer = 0;
        }
        break;
    case 1:
        arg0->timer = arg0->timer + 1;
        if (arg0->timer == END_OBJ_VISIBLE_FRAMES) {
            arg0->timer = 0;
            arg0->state = 2;
        }
        break;
    case 2:
        arg0->alpha -= END_OBJ_FADE_STEP;
        if (!(arg0->alpha > 0)) {
            arg0->alpha = 0;
            arg0->state = 3;
            arg0->cycleCount = arg0->cycleCount + 1;
            if (arg0->cycleCount == END_OBJ_HOLD_CYCLES) {
                arg0->cycleCount = 0;
                func_80071824(arg0, func_8003B9F8);
            }
            if (D_8010B1A2 == 0) {
                D_8010B1A2 = 1;
            }
        }
        break;
    case 3:
        arg0->timer = arg0->timer + 1;
        if (!(arg0->timer < END_OBJ_RESET_DELAY)) {
            arg0->timer = 0;
            arg0->state = 0;
        }
        break;
    }
    func_800483FC(&D_80124868, func_8003B7C0, arg0);
}

void func_8003BBBC(EndObjTask *arg0) {
    arg0->state = 3;
    arg0->cycleCount = 0;
    arg0->x = -0x40;
    arg0->y = 0x10;
    arg0->alpha = 0;
    func_80071824(arg0, func_8003BA64);
}

void func_8003BC00(DebugObjectPositionTask *arg0) {
    char sp38[0x10];

    if (arg0->enabled == 1) {
        func_8000F030(arg0->x, arg0->y, func_80043040(D_80112172), (u16)arg0->objectId, 0x20, 0x20, arg0->palette,
                      0);
        sprintf(sp38, D_800E1060, arg0->objectId);
        func_80013D0C(0x40, -0x66, sp38, 0, 0x100);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_debug_ui/func_8003BC9C.s")

void func_8003BEB4(DebugObjectPositionTask *arg0) {
    arg0->x = 0;
    arg0->y = 0;
    arg0->objectId = 0;
    arg0->enabled = 0;
    arg0->palette = 0;
    func_80071824(arg0, func_8003BC9C);
}
