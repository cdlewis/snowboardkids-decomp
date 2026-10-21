#include "common.h"
#include <PR/os_libc.h>
#include "game/engine/controller_input.h"
#include "game/engine/render_callback.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/race/camera/race_camera.h"
#include "game/race/motion/race_motion_animation_debug_viewer.h"
#include "game/race/motion/race_motion.h"
#include "game/race/player/race_player_model_renderer.h"
#include "game/race/player/race_player_input.h"

const char gRaceMotionAnimationDebugViewerMotionNumberFormat[] = "MOTION NO %3.3i";

void initRaceMotionAnimationDebugViewer(void) {
    RacePlayer *state;
    s16 temp_v0;

    setRaceCameraMode(0, 2);
    state = &gRacePlayers[0];
    state->leanAngle = 0;
    temp_v0 = state->leanAngle;
    state->pos.x = 0;
    state->pos.y = 0;
    state->pos.z = 0;
    state->animationId = 0;
    state->groundCorrectionY = 0xFFFA0000;
    state->groundOffset = 0;
    state->mode = 0;
    state->playerIndex = 0;
    state->facingAngle = temp_v0;
    state->pitchAngle = temp_v0;
    initRaceMotionModelParts(state);
}

void drawRaceMotionAnimationDebugViewerMotionNumber(void *unused) {
    char buf[0x64];

    sprintf(buf, gRaceMotionAnimationDebugViewerMotionNumberFormat, gRacePlayers[0].animationId);
    drawMenuAsciiTextDefaultScale(0x28, 0x28, buf, 1);
}

void updateRaceMotionAnimationDebugViewer(void) {
    s32 pressedInput;

    if (gRacePlayers[0].mode == 0) {
        gRacePlayers[0].mode = 1;
        setRaceMotionAnimation((RaceMotionState *)&gRacePlayers[0], gRacePlayers[0].animationId);
        stepRaceMotionLoopingAnimation((RaceMotionState *)&gRacePlayers[0]);
    } else {
        pressedInput = gPlayerInputPressed[0];
        if (pressedInput & Z_TRIG) {
            if (gRacePlayers[0].stateFlags != 0) {
                gRacePlayers[0].stateFlags = 0;
            } else {
                gRacePlayers[0].stateFlags = 0x400;
            }
        }
        if ((pressedInput & U_CBUTTONS) && (gRacePlayers[0].animationId != 0)) {
            gRacePlayers[0].animationId--;
            gRacePlayers[0].mode = 0;
        }
        if (pressedInput & D_CBUTTONS) {
            gRacePlayers[0].mode = 0;
            gRacePlayers[0].animationId++;
        }
        if (gPlayerInputHeld[0] & L_CBUTTONS) {
            stepRaceMotionLoopingAnimation((RaceMotionState *)&gRacePlayers[0]);
            pressedInput = gPlayerInputPressed[0];
        }
        if (pressedInput & R_CBUTTONS) {
            stepRaceMotionLoopingAnimation((RaceMotionState *)&gRacePlayers[0]);
        }
    }
    addRenderCallback(
        &gModelRenderCallbackList,
        (RenderCallback)drawRacePlayerModel,
        &gRacePlayers[0]
    );
    addRenderCallback(&gMenuForegroundRenderCallbackList, drawRaceMotionAnimationDebugViewerMotionNumber, NULL);
}
