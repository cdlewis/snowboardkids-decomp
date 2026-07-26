#include "common.h"
#include "game/engine/render_callback.h"
#include "game/race/camera/race_camera.h"
#include "game/race/motion/race_motion_animation_debug_viewer.h"
#include "game/race/motion/race_motion.h"
#include "game/race/player/race_player_model_renderer.h"

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
} RaceMotionAnimationDebugViewerState;

extern RaceMotionAnimationDebugViewerState gRacePlayers;
extern s32 gPlayerInputHeld;
extern s32 gPlayerInputPressed;
extern void drawMenuAsciiTextDefaultScale(s32, s32, char *, s32);

#define gRaceMotionAnimationDebugViewerState gRacePlayers

const char gRaceMotionAnimationDebugViewerMotionNumberFormat[] = "MOTION NO %3.3i";

void initRaceMotionAnimationDebugViewer(void) {
    RaceMotionAnimationDebugViewerState *state;
    s16 temp_v0;

    setRaceCameraMode(0, 2);
    state = &gRaceMotionAnimationDebugViewerState;
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

void drawRaceMotionAnimationDebugViewerMotionNumber(void *unused) {
    char buf[0x64];

    sprintf(buf, gRaceMotionAnimationDebugViewerMotionNumberFormat,
            gRaceMotionAnimationDebugViewerState.animationIndex);
    drawMenuAsciiTextDefaultScale(0x28, 0x28, buf, 1);
}

void updateRaceMotionAnimationDebugViewer(void) {
    s32 buttons;

    if (gRaceMotionAnimationDebugViewerState.animationLoaded == 0) {
        gRaceMotionAnimationDebugViewerState.animationLoaded = 1;
        setRaceMotionAnimation((RaceMotionState *)&gRaceMotionAnimationDebugViewerState,
                               gRaceMotionAnimationDebugViewerState.animationIndex);
        stepRaceMotionLoopingAnimation((RaceMotionState *)&gRaceMotionAnimationDebugViewerState);
    } else {
        buttons = gPlayerInputPressed;
        if (buttons & Z_TRIG) {
            if (gRaceMotionAnimationDebugViewerState.modelFlags != 0) {
                gRaceMotionAnimationDebugViewerState.modelFlags = 0;
            } else {
                gRaceMotionAnimationDebugViewerState.modelFlags = 0x400;
            }
        }
        if ((buttons & U_CBUTTONS) && (gRaceMotionAnimationDebugViewerState.animationIndex != 0)) {
            gRaceMotionAnimationDebugViewerState.animationIndex--;
            gRaceMotionAnimationDebugViewerState.animationLoaded = 0;
        }
        if (buttons & D_CBUTTONS) {
            gRaceMotionAnimationDebugViewerState.animationLoaded = 0;
            gRaceMotionAnimationDebugViewerState.animationIndex++;
        }
        if (gPlayerInputHeld & L_CBUTTONS) {
            stepRaceMotionLoopingAnimation((RaceMotionState *)&gRaceMotionAnimationDebugViewerState);
            buttons = gPlayerInputPressed;
        }
        if (buttons & R_CBUTTONS) {
            stepRaceMotionLoopingAnimation((RaceMotionState *)&gRaceMotionAnimationDebugViewerState);
        }
    }
    addRenderCallback(&gModelRenderCallbackList, (RenderCallback)drawRacePlayerModel,
                      (RacePlayerModelRenderState *)&gRaceMotionAnimationDebugViewerState);
    addRenderCallback(&gMenuForegroundRenderCallbackList,
                      drawRaceMotionAnimationDebugViewerMotionNumber, NULL);
}
