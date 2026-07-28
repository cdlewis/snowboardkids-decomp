#include "common.h"
#include "game/engine/controller_input.h"
#include "game/engine/render_callback.h"
#include "game/race/camera/race_camera.h"
#include "game/race/motion/race_motion_animation_debug_viewer.h"
#include "game/race/motion/race_motion.h"
#include "game/race/player/race_player_model_renderer.h"
#include "game/race/player/race_player_input.h"

extern void drawMenuAsciiTextDefaultScale(s32, s32, char *, s32);

const char gRaceMotionAnimationDebugViewerMotionNumberFormat[] = "MOTION NO %3.3i";

void initRaceMotionAnimationDebugViewer(void) {
    RacePlayer *state;
    s16 temp_v0;

    setRaceCameraMode(0, 2);
    state = &gRacePlayers[0];
    state->unk2EE = 0;
    temp_v0 = state->unk2EE;
    state->pos.x = 0;
    state->pos.y = 0;
    state->pos.z = 0;
    state->animationId = 0;
    state->unk64 = 0xFFFA0000;
    state->unk58 = 0;
    state->mode = 0;
    state->playerIndex = 0;
    state->facingAngle = temp_v0;
    state->pitchAngle = temp_v0;
    initRaceMotionModelParts((RaceMotionInitState *)state);
}

void drawRaceMotionAnimationDebugViewerMotionNumber(void *unused) {
    char buf[0x64];

    sprintf(buf, gRaceMotionAnimationDebugViewerMotionNumberFormat,
            gRacePlayers[0].animationId);
    drawMenuAsciiTextDefaultScale(0x28, 0x28, buf, 1);
}

void updateRaceMotionAnimationDebugViewer(void) {
    s32 buttons;

    if (gRacePlayers[0].mode == 0) {
        gRacePlayers[0].mode = 1;
        setRaceMotionAnimation((RaceMotionState *)&gRacePlayers[0],
                               gRacePlayers[0].animationId);
        stepRaceMotionLoopingAnimation((RaceMotionState *)&gRacePlayers[0]);
    } else {
        buttons = gPlayerInputPressed[0];
        if (buttons & Z_TRIG) {
            if (gRacePlayers[0].stateFlags != 0) {
                gRacePlayers[0].stateFlags = 0;
            } else {
                gRacePlayers[0].stateFlags = 0x400;
            }
        }
        if ((buttons & U_CBUTTONS) && (gRacePlayers[0].animationId != 0)) {
            gRacePlayers[0].animationId--;
            gRacePlayers[0].mode = 0;
        }
        if (buttons & D_CBUTTONS) {
            gRacePlayers[0].mode = 0;
            gRacePlayers[0].animationId++;
        }
        if (gPlayerInputHeld[0] & L_CBUTTONS) {
            stepRaceMotionLoopingAnimation((RaceMotionState *)&gRacePlayers[0]);
            buttons = gPlayerInputPressed[0];
        }
        if (buttons & R_CBUTTONS) {
            stepRaceMotionLoopingAnimation((RaceMotionState *)&gRacePlayers[0]);
        }
    }
    addRenderCallback(&gModelRenderCallbackList, (RenderCallback)drawRacePlayerModel,
                      (RacePlayerModelRenderState *)&gRacePlayers[0]);
    addRenderCallback(&gMenuForegroundRenderCallbackList,
                      drawRaceMotionAnimationDebugViewerMotionNumber, NULL);
}
