#include "common.h"
#include "race_camera.h"
#include "race_motion_test_viewer.h"
#include "race_motion.h"
#include "race_position_ui.h"

typedef struct {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 z;
} Vec3i;

typedef struct {
    /* 0x000 */ s16 modelId;
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
    /* 0x2FC */ s32 modelFlags;
    /* 0x300 */ s16 animationLoaded;
    /* 0x302 */ u8 pad302[0x150];
    /* 0x452 */ s16 animationIndex;
} RaceMotionTestViewerState;

extern RaceMotionTestViewerState D_80121D80;
extern s32 gPlayerInputHeld;
extern s32 gPlayerInputPressed;
extern void *gMenuForegroundRenderCallbackList;
extern void *gModelRenderCallbackList;
extern void sprintf(char *, const char *, ...);
extern void drawMenuAsciiTextDefaultScale(s32, s32, char *, s32);
extern void addRenderCallback(void *, void *, void *);

#define gRaceMotionTestViewerState D_80121D80

const char gRaceMotionTestViewerMotionNumberFormat[] = "MOTION NO %3.3i";

void initRaceMotionTestViewer(void) {
    RaceMotionTestViewerState *state;
    s16 temp_v0;

    setRaceCameraMode(0, 2);
    state = &gRaceMotionTestViewerState;
    state->frame = 0;
    temp_v0 = state->frame;
    state->pos.x = 0;
    state->pos.y = 0;
    state->pos.z = 0;
    state->animationIndex = 0;
    state->zOffset = 0xFFFA0000;
    state->unk58 = 0;
    state->animationLoaded = 0;
    state->modelId = 0;
    state->endFrame = temp_v0;
    state->startFrame = temp_v0;
    initRaceMotionModelParts((RaceMotionInitState *)state);
}

void drawRaceMotionTestViewerMotionNumber(s32 unused) {
    char buf[0x64];

    sprintf(buf, gRaceMotionTestViewerMotionNumberFormat,
            gRaceMotionTestViewerState.animationIndex);
    drawMenuAsciiTextDefaultScale(0x28, 0x28, buf, 1);
}

void updateRaceMotionTestViewer(void) {
    s32 buttons;

    if (gRaceMotionTestViewerState.animationLoaded == 0) {
        gRaceMotionTestViewerState.animationLoaded = 1;
        setRaceMotionAnimation((RaceMotionState *)&gRaceMotionTestViewerState,
                               gRaceMotionTestViewerState.animationIndex);
        stepRaceMotionLoopingAnimation((RaceMotionState *)&gRaceMotionTestViewerState);
    } else {
        buttons = gPlayerInputPressed;
        if (buttons & 0x2000) {
            if (gRaceMotionTestViewerState.modelFlags != 0) {
                gRaceMotionTestViewerState.modelFlags = 0;
            } else {
                gRaceMotionTestViewerState.modelFlags = 0x400;
            }
        }
        if ((buttons & 8) && (gRaceMotionTestViewerState.animationIndex != 0)) {
            gRaceMotionTestViewerState.animationIndex--;
            gRaceMotionTestViewerState.animationLoaded = 0;
        }
        if (buttons & 4) {
            gRaceMotionTestViewerState.animationLoaded = 0;
            gRaceMotionTestViewerState.animationIndex++;
        }
        if (gPlayerInputHeld & 2) {
            stepRaceMotionLoopingAnimation((RaceMotionState *)&gRaceMotionTestViewerState);
            buttons = gPlayerInputPressed;
        }
        if (buttons & 1) {
            stepRaceMotionLoopingAnimation((RaceMotionState *)&gRaceMotionTestViewerState);
        }
    }
    addRenderCallback(&gModelRenderCallbackList, drawRacePositionUiPlayerModel,
                      (RacePositionUiPlayer *)&gRaceMotionTestViewerState);
    addRenderCallback(&gMenuForegroundRenderCallbackList,
                      drawRaceMotionTestViewerMotionNumber, NULL);
}
