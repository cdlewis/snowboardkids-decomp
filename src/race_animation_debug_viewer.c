#include "common.h"
#include "race_camera.h"
#include "race_motion.h"
#include "race_animation_debug_viewer.h"
#include "race_position_ui.h"

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
} RaceAnimationDebugViewerState;

extern RaceAnimationDebugViewerState D_80121D80;
extern s32 gPlayerInputHeld;
extern s32 gPlayerInputPressed;
extern void *D_80124858;
extern void *gModelRenderCallbackList;
extern void sprintf(char *, const char *, ...);
extern void drawMenuAsciiTextDefaultScale(s32, s32, char *, s32);
extern void addRenderCallback(void *, void *, void *);

const char gRaceAnimationDebugViewerMotionNumberFormat[] = "MOTION NO %3.3i";

void initRaceAnimationDebugViewer(void) {
    RaceAnimationDebugViewerState *state;
    s16 temp_v0;

    setRaceCameraMode(0, 2);
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
    initRaceMotionModelParts((RaceMotionInitState *)state);
}

void drawRaceAnimationDebugViewerMotionNumber(s32 arg0) {
    char buf[0x64];

    sprintf(buf, gRaceAnimationDebugViewerMotionNumberFormat, D_80121D80.motionIndex);
    drawMenuAsciiTextDefaultScale(0x28, 0x28, buf, 1);
}

void updateRaceAnimationDebugViewer(void) {
    s32 buttons;

    if (D_80121D80.initialized == 0) {
        D_80121D80.initialized = 1;
        setRaceMotionAnimation((RaceMotionState *)&D_80121D80, D_80121D80.motionIndex);
        stepRaceMotionLoopingAnimation((RaceMotionState *)&D_80121D80);
    } else {
        buttons = gPlayerInputPressed;
        if (buttons & 0x2000) {
            if (D_80121D80.renderMode != 0) {
                D_80121D80.renderMode = 0;
            } else {
                D_80121D80.renderMode = 0x400;
            }
        }
        if ((buttons & 8) && (D_80121D80.motionIndex != 0)) {
            D_80121D80.motionIndex--;
            D_80121D80.initialized = 0;
        }
        if (buttons & 4) {
            D_80121D80.initialized = 0;
            D_80121D80.motionIndex++;
        }
        if (gPlayerInputHeld & 2) {
            stepRaceMotionLoopingAnimation((RaceMotionState *)&D_80121D80);
            buttons = gPlayerInputPressed;
        }
        if (buttons & 1) {
            stepRaceMotionLoopingAnimation((RaceMotionState *)&D_80121D80);
        }
    }
    addRenderCallback(&gModelRenderCallbackList, func_8007C5E8, (RacePositionUiPlayer *)&D_80121D80);
    addRenderCallback(&D_80124858, drawRaceAnimationDebugViewerMotionNumber, NULL);
}
