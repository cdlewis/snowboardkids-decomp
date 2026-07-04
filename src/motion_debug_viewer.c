#include "common.h"

typedef struct {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 z;
} Vec3i;

typedef struct {
    /* 0x000 */ s16 objectId;
    /* 0x002 */ u8 pad2[0x1A];
    /* 0x01C */ Vec3i pos;
    /* 0x028 */ u8 pad28[0x30];
    /* 0x058 */ s32 unk58;
    /* 0x05C */ u8 pad5C[8];
    /* 0x064 */ s32 zOffset;
    /* 0x068 */ u8 pad68[0x282];
    /* 0x2EA */ s16 startFrame;
    /* 0x2EC */ s16 endFrame;
    /* 0x2EE */ s16 frame;
    /* 0x2F0 */ u8 pad2F0[0xC];
    /* 0x2FC */ s32 renderMode;
    /* 0x300 */ s16 initialized;
    /* 0x302 */ u8 pad302[0x150];
    /* 0x452 */ s16 motionIndex;
} MotionDebugViewerState;

extern s16 D_801221D2;
extern MotionDebugViewerState D_80121D80;
extern char D_800E1700[];
extern void sprintf(char *, const char *, ...);
extern void func_80048278(s32, s32, char *, s32);
extern void func_8006D520(s32, s32);
extern void func_80082FC8(MotionDebugViewerState *);

void func_80078250(void) {
    MotionDebugViewerState *state;
    s16 temp_v0;

    func_8006D520(0, 2);
    state = &D_80121D80;
    state->frame = 0;
    temp_v0 = state->frame;
    state->pos.x = 0;
    state->pos.y = 0;
    state->pos.z = 0;
    state->motionIndex = 0;
    state->zOffset = 0xFFFA0000;
    state->unk58 = 0;
    state->initialized = 0;
    state->objectId = 0;
    state->endFrame = temp_v0;
    state->startFrame = temp_v0;
    func_80082FC8(state);
}

void func_800782B4(s32 arg0) {
    char buf[0x64];

    sprintf(buf, D_800E1700, D_801221D2);
    func_80048278(0x28, 0x28, buf, 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/motion_debug_viewer/func_800782FC.s")
