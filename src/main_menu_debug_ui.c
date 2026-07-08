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

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
} EndObjTextPosition;

typedef struct {
    /* 0x00 */ s16 count;
    /* 0x02 */ EndObjTextPosition positions[5];
    /* 0x16 */ s16 pad;
} EndObjTextLayout;

extern u16 D_8010B1A2;
extern s16 D_80112172;
extern void *D_80124868;
extern char D_800E1060[];
extern char D_800E1070[];
extern s32 D_80123758;
extern s32 D_80123778[];
extern u16 D_800B8140[][0x5A];
extern EndObjTextLayout D_800B92D4[];
extern s32 func_80043040(s16);
extern void func_8000F8AC(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_8000F030(s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_80013154(s32, s32, u8 *, s32, s32, s32);
extern void func_80013D0C(s32, s32, void *, s32, s32);
extern void func_800483FC(void *, void *, void *);
extern void func_80071824(void *task, void (*callback)());
extern int rmonPrintf(const char *, ...);
extern int sprintf(char *, const char *, ...);

void func_8003B7C0(EndObjTask *arg0);
void func_8003B9F8(EndObjTask *arg0);
void func_8003BA64(EndObjTask *arg0);
void func_8003BC9C(DebugObjectPositionTask *arg0);

// func_8003B7C0 best match: 82.541%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_debug_ui/func_8003B7C0.s")

#ifdef NON_MATCHING
void func_8003B7C0(EndObjTask *arg0) {
    s16 count;
    s32 i;
    s32 scriptIndex;
    s32 lineLength;
    s32 layoutOffset;
    s16 x;
    s16 y;
    u16 glyph;
    volatile u16 colorMode;
    u16 text[0x1E];
    u16 pad[0xE];
    EndObjTextLayout *layout;

    layout = &D_800B92D4[arg0->cycleCount];
    count = layout->count;
    i = 0;
    if (count > 0) {
        scriptIndex = 0;
        layoutOffset = 0;
        do {
            layout = &D_800B92D4[arg0->cycleCount];
            glyph = D_800B8140[arg0->cycleCount][scriptIndex];
            x = *(s16 *)((u8 *)layout->positions + layoutOffset);
            y = *(s16 *)((u8 *)layout->positions + layoutOffset + 2);
            lineLength = 0;
            if (glyph != 0xFFFF) {
                do {
                    text[lineLength] = glyph;
                    scriptIndex++;
                    glyph = D_800B8140[arg0->cycleCount][scriptIndex];
                    lineLength++;
                } while (glyph != 0xFFFF);
            }
            text[lineLength] = 0xFFFF;
            scriptIndex++;
            func_80013154((s16)x, y, (u8 *)text, 0, arg0->alpha, colorMode);
            i++;
            layoutOffset += 4;
        } while (i != count);
    }
}
#endif

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

void func_8003BC9C(DebugObjectPositionTask *arg0) {
    s16 temp_a1;
    s16 temp_a2;
    s16 oldY;

    if (D_80123778[0] & 0x2000) {
        if (arg0->enabled == 1) {
            arg0->enabled = 0;
        } else {
            arg0->enabled = 1;
        }
    }
    if (arg0->enabled == 1) {
        temp_a2 = arg0->y;
        temp_a1 = arg0->x;
        oldY = temp_a2;
        if ((D_80123758 & 0x10800) && (temp_a2 >= -0x73)) {
            arg0->y = temp_a2 - 1;
        }
        if (D_80123758 & 0x20400) {
            temp_a2 = arg0->y;
            if (temp_a2 < 0x68) {
                arg0->y = temp_a2 + 1;
            }
        }
        if (D_80123758 & 0x80200) {
            if (arg0->x >= -0x67) {
                arg0->x = arg0->x - 1;
            }
        }
        if (D_80123758 & 0x40100) {
            if (arg0->x < 0x68) {
                arg0->x = arg0->x + 1;
            }
        }
        if (D_80123778[0] & 0x8000) {
            arg0->objectId += 1;
            if (arg0->objectId == 0x35) {
                arg0->objectId = 0;
            }
        }
        if (D_80123778[0] & 0x4000) {
            arg0->objectId = arg0->objectId - 1;
            if (arg0->objectId < 0) {
                arg0->objectId = 0x34;
            }
        }
        if (D_80123778[0] & 8) {
            arg0->palette = (arg0->palette + 1) & 1;
        }
        temp_a2 = arg0->y;
        if ((temp_a1 != arg0->x) || (oldY != temp_a2)) {
            rmonPrintf(D_800E1070, arg0->x, temp_a2);
        }
    }
    func_800483FC(&D_80124868, func_8003BC00, arg0);
}

void func_8003BEB4(DebugObjectPositionTask *arg0) {
    arg0->x = 0;
    arg0->y = 0;
    arg0->objectId = 0;
    arg0->enabled = 0;
    arg0->palette = 0;
    func_80071824(arg0, func_8003BC9C);
}
