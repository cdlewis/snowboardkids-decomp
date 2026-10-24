#include "common.h"
#include "game/race/items/race_items.h"
#include "game/engine/game_task_scheduler.h"
#include "game/race/race_state.h"
#include "game/engine/render_callback.h"
#include "game/engine/asset_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/race/player/race_player_rumble.h"
#include "game/audio/sound_manager.h"
#include "game/race/motion/race_motion.h"
#include "game/race/camera/race_camera.h"
#include "game/race/course/race_course_effects.h"
#include "game/race/player/race_player_collision.h"
#include "game/math/fixed_point_math.h"
#include "game/math/spatial_math.h"
#include "game/race/player/race_player_input.h"
#include "game/race/effects/race_visual_effects.h"
#include "game/race/player/race_player_equipment_use.h"
#include "game/menu/renderer/menu_screen_effects.h"
#include "game/race/player/race_player_movement.h"
#include "game/race/player/race_player_progress.h"
#include "game/race/player/race_player_model_renderer.h"
#include "game/race/player/race_player_update.h"
#include "game/race/player/race_player_properties.h"
#include "game/race/course/race_surface_cues.h"
#include "game/race/ui/race_ui_effects.h"
#include "game/race/effects/snowboard_trail_effects.h"
#include "game/engine/viewport_manager.h"
#include "game/race/flow/race_flow.h"

typedef struct {
    u8 matrix[0x20];
    s32 transformedX;
    s32 transformedY;
    s32 transformedZ;
    s32 sourceX;
    s32 sourceY;
    s32 sourceZ;
} PlayerTransformScratch80095A88;

void initRacePlayers(void) {
    gRacePlayers[0].playerIndex = 0;
    gRacePlayers[1].playerIndex = 1;
    gRacePlayers[2].playerIndex = 2;
    gRacePlayers[3].playerIndex = 3;
    if (gRacePlayers[0].isActive != 0) {
        initRacePlayer(&gRacePlayers[0]);
    }
    if (gRacePlayers[1].isActive != 0) {
        initRacePlayer(&gRacePlayers[1]);
    }
    if (gRacePlayers[2].isActive != 0) {
        initRacePlayer(&gRacePlayers[2]);
    }
    if (gRacePlayers[3].isActive != 0) {
        initRacePlayer(&gRacePlayers[3]);
    }
}

void applyRacePlayerProperties(RacePlayer *arg0) {
    RacePlayer *player;
    RacePlayerProperties *var_v0;
    RacePlayerProperties *var_v1;
    RacePlayerProperties *temp_a1;
    RacePlayerProperties *temp_a2;

    player = arg0;
    var_v0 = gRaceCharacterProperties;
    if ((gRaceDemoPlaybackEnabled != 0) || (var_v1 = var_v0, gMainMenuModeSelection != 0)) {
        var_v0 = gRaceDemoBoardProperties;
        var_v1 = gRaceDemoCharacterProperties;
    } else {
        var_v0 = gRaceBoardProperties;
    }
    temp_a1 = (RacePlayerProperties *)((u8 *)var_v1 + (player->characterId * sizeof(RacePlayerProperties)));
    temp_a2 = (RacePlayerProperties *)((u8 *)var_v0 + (player->snowboardIndex * sizeof(RacePlayerProperties)));
    player->baseSpeedLimit = (temp_a1->maxSpeed + temp_a2->maxSpeed) << 8;
    player->speedLimit = player->baseSpeedLimit;
    player->gravity = (temp_a1->gravity + temp_a2->gravity) << 4;
    player->aerialGravity = (temp_a1->aerialGravity + temp_a2->aerialGravity) << 4;
    player->turnStrength = temp_a1->turnStrength + temp_a2->turnStrength;
    player->lateralDeceleration = (temp_a1->lateralDeceleration + temp_a2->lateralDeceleration) << 4;
    player->turnRadiusAtFullLean = (temp_a1->turnRadiusAtFullLean + temp_a2->turnRadiusAtFullLean) << 4;
    player->turnRadiusAtHalfLean = (temp_a1->turnRadiusAtHalfLean + temp_a2->turnRadiusAtHalfLean) << 4;
    player->forwardDeceleration = (temp_a1->forwardDeceleration + temp_a2->forwardDeceleration) << 4;
    player->reverseDeceleration = (temp_a1->reverseDeceleration + temp_a2->reverseDeceleration) << 4;
}

#pragma GLOBAL_ASM("asm.jp/nonmatchings/race/player/race_player_update/initRacePlayer.s")

void updateRacePlayers(void) {
    s32 i;

    gRacePlayers[0].loopSoundVolume = 0;
    gRacePlayers[1].loopSoundVolume = 0;
    gRacePlayers[2].loopSoundVolume = 0;
    gRacePlayers[3].loopSoundVolume = 0;
    if (gRaceUpdatePaused == 0) {
        updateRacePlayerInput(&gRacePlayers[0]);
        updateRacePlayerInput(&gRacePlayers[1]);
        updateRacePlayerInput(&gRacePlayers[2]);
        updateRacePlayerInput(&gRacePlayers[3]);
        updateRacePlayerRankDisplay();
        for (i = 0; i < gRacePlayerCount; i++) {
            updateRacePlayer(&gRacePlayers[i]);
        }
        resolveRacePlayerBodyCollisions();
        updateRacePlayerRankings();
    }
}

#pragma GLOBAL_ASM("asm.jp/nonmatchings/race/player/race_player_update/updateRacePlayer.s")

void updateRacePlayerMotionFeedback(RacePlayer *player) {
    s16 angleDiff;
    s32 itemType;
    s32 deltaX;
    s32 deltaZ;

    deltaX = player->pos.x - player->previousPosition.x;
    deltaZ = player->pos.z - player->previousPosition.z;
    angleDiff = (calculateFixedAngleFromDeltaXZ(deltaX, deltaZ) - player->facingAngle) & 0xFFF;
    if (angleDiff >= 0x801) {
        angleDiff = 0x1000 - angleDiff;
    }
    if (angleDiff >= 0x401) {
        angleDiff = 0x800 - angleDiff;
    }

    player->loopSoundPitch = (f32)((f64)(f32)-angleDiff / 171.0);
    player->loopSoundVolume = player->movementSpeed >> 0xC;
    if (player->loopSoundVolume >= 0x80) {
        player->loopSoundVolume = 0x7F;
    }

    itemType = getRaceItemEffectType(player->surfaceType);
    player->loopSoundId = SFX_BOARD_GLIDE_LOOP;
    if (itemType == 1) {
        player->loopSoundId = SFX_BREAKABLE_SURFACE_LOOP;
        player->loopSoundVolume = 0x7F;
        player->loopSoundPitch = 0.0f;
    }
    if (itemType == 2) {
        player->loopSoundId = SFX_BOARD_CONTACT_FALLBACK_LOOP;
    }
    if (3 == itemType) {
        player->loopSoundId = SFX_BOARD_SURFACE_SPRAY_LOOP;
    }
    if (4 == itemType) {
        player->loopSoundId = SFX_BOARD_CONTACT_FALLBACK_LOOP;
        if (player->surfaceType == 0xD) {
            player->loopSoundId = SFX_BOARD_SURFACE_RUMBLE_LOOP;
            if (player->loopSoundVolume >= 0x51) {
                player->rumbleTimer = 2;
                player->rumblePatternId = 1;
            }
        }
        player->loopSoundPitch = 0.0f;
    }
    if (player->stateFlags & 0x2000000) {
        player->loopSoundId = SFX_BOARD_CONTACT_FALLBACK_LOOP;
    }
}

void updateRacePlayerMode00Grounded(RacePlayer *player) {
    volatile s32 pad[26];
    s64 temp;
    s32 turnTarget;
    s32 targetX;
    s32 surfaceCue;
    s32 targetZ;
    s32 bankRate;
    RaceCourseStartEntry *spawn;
    s32 lean;
    s32 speed;
    s16 steerAngle;
    s16 turn;
    s16 quickTurn;
    s32 rotation;
    s16 unused;
    s32 interpolated;

    if (player->updateState == 0) {
        player->updateState++;
        player->stateFlags &= ~0x200;
        player->stanceChangeTimer = 0;
        player->stanceTransitionFrame = 0;
        player->subState = 0;
        player->subStateTimer = 0;
        player->modeAngle = 0;
        player->unk528 = 0;
        player->localVelocityX = 0;
        if (player->unk57B != 0) {
            enqueueRacePlayerVoiceSound(player, RACE_VOICE_LANDING);
            setRaceMotionAnimation(player, 0x22);
        }
    }

    if (player->unk57A != 0) {
        player->stanceChangeTimer = 6;
    }
    if (player->targetGroundOffset == 0) {
        player->targetGroundOffset = 0x40000;
    }

    player->velocity.y -= player->gravity;
    speed = player->localVelocityZ;
    if (speed > 0) {
        if (speed < 0x4000) {
            speed = 0x4000;
        }
    } else if (speed >= -0x3FFF) {
        speed = -0x4000;
    }

    if (player->isCpu != 0) {
        getRaceCourseTargetPositionAhead(
            player->coursePathIndex,
            player->pos.x,
            player->pos.z,
            &targetX,
            &targetZ,
            (s8)player->unk17,
            (u16)player->playerIndex
        );
        turn =
            (calculateFixedAngleBetweenXZPoints(player->pos.x, player->pos.z, targetX, targetZ) - player->facingAngle) &
            0xFFF;
        if (turn >= 0x801) {
            turn -= 0x1000;
        }
        if (turn >= 0x101) {
            turn = 0x100;
        }
        if (turn < -0x100) {
            turn = -0x100;
        }

        quickTurn = turn >> 2;
        turn = (turn + 1) >> 1;
        if (turn >= 0x20) {
            turn = 0x1F;
        }
        if (turn < -0x1F) {
            turn = -0x1F;
        }

        turn = (turn - player->unk528) & 0xFFF;
        if (turn >= 0x801) {
            turn -= 0x1000;
        }
        if (turn >= 3) {
            turn = 2;
        }
        if (turn < -2) {
            turn = -2;
        }
        player->unk528 += turn;
        turn = player->unk528;
        if (turn == 1) {
            turn = 0;
        }
        if (turn == -1) {
            turn = 0;
        }
        player->leanScale = 0x3F;
    } else {
        turn = -player->stickX;
        player->leanScale = 0x20 - player->stickY;
        if (player->leanScale < 0x1F) {
            player->leanScale = 0x1F;
        }
    }

    lean = speed;
    if (player->localVelocityZ > 0) {
        lean = -speed;
        turn = -turn;
    }

    if (player->isCpu == 0) {
        spawn = &gRaceCourseStartEntries[gRaceCourseIndex.signedValue];
        if ((spawn->pathIndex == player->coursePathIndex) && !(player->stateFlags & 0x40)) {
            surfaceCue = (s16)(((calculateFixedAngleBetweenXZPoints(
                                     player->pos.x,
                                     player->pos.z,
                                     spawn->startSurfaceTargetX,
                                     spawn->startSurfaceTargetZ
                                 ) -
                                 player->facingAngle) +
                                0x400) &
                               0xFFF);
            if (surfaceCue < 0x800) {
                if (player->stateFlags & 0x400) {
                    player->stanceChangeTimer = 0;
                    player->stateFlags &= ~0x400;
                    player->stanceTransitionFrame = 6 - player->stanceTransitionFrame;
                }
            } else if (!(player->stateFlags & 0x400)) {
                player->stanceChangeTimer = 0;
                player->stateFlags |= 0x400;
                player->stanceTransitionFrame = 6 - player->stanceTransitionFrame;
            }
        } else {
            if (player->localVelocityZ < -0x8000) {
                if (player->stateFlags & 0x400) {
                    if (player->stanceChangeTimer >= 6) {
                        player->stanceChangeTimer = 0;
                        player->stateFlags &= ~0x400;
                        player->stanceTransitionFrame = 6 - player->stanceTransitionFrame;
                    } else {
                        player->stanceChangeTimer++;
                    }
                }
            }
            if (player->localVelocityZ >= 0x8001) {
                if (!(player->stateFlags & 0x400)) {
                    if (player->stanceChangeTimer >= 6) {
                        player->stanceChangeTimer = 0;
                        player->stateFlags |= 0x400;
                        player->stanceTransitionFrame = 6 - player->stanceTransitionFrame;
                    } else {
                        player->stanceChangeTimer++;
                    }
                }
            }
        }
    }

    if (!(gMenuFlowState & 1)) {
        turnTarget = updateRacePlayerLeanAngle(player, lean, turn);
        if (player->stanceTransitionFrame == 0) {
            steerAngle = player->modeAngle;
            steerAngle = turnTarget - steerAngle;
            if (steerAngle >= 0x31) {
                steerAngle = 0x30;
            }
            if (steerAngle < -0x30) {
                steerAngle = -0x30;
            }
            player->modeAngle += steerAngle;
            if (player->modeAngle == 0) {
                if (player->animationId == 0x22) {
                    if (stepRaceMotionJointAnimationUntilEnd(player) != 0) {
                        setRaceMotionAnimation(player, 1);
                    }
                } else {
                    if (player->animationId != 1) {
                        setRaceMotionAnimation(player, 1);
                    }
                    stepRaceMotionLoopingJointAnimation(player);
                }
            } else if (player->modeAngle >= 0) {
                setRaceMotionAnimation(player, 3);
                blendRaceMotionJointAnimation(player, 3, player->modeAngle, 0x118);
            } else {
                setRaceMotionAnimation(player, 2);
                blendRaceMotionJointAnimation(player, 2, -player->modeAngle, 0x118);
            }
        } else {
            player->stanceTransitionFrame--;
            steerAngle = -player->modeAngle;
            if (steerAngle >= 0x31) {
                steerAngle = 0x30;
            }
            if (steerAngle < -0x30) {
                steerAngle = -0x30;
            }
            player->modeAngle += steerAngle;
            setRaceMotionAnimation(player, player->stanceTransitionFrame + 8);
            stepRaceMotionJointAnimationUntilEnd(player);
        }

        if (player->isCpu == 0) {
            steerAngle = 0;
            bankRate = (-turn * 0x10 * player->turnStrength) / 0x100;
            interpolated = (((player->turnRadiusAtHalfLean - player->turnRadiusAtFullLean) * (0x3F - player->leanScale)) / 0x20) + player->turnRadiusAtFullLean;
            if (bankRate != 0) {
                lean = (lean - 0xFF) / 0x100;
                temp = ((s64)interpolated * lean * lean) / bankRate;
                if (temp != 0) {
                    if (temp > 0) {
                        temp += 0xC0000;
                    }
                    if (temp < 0) {
                        temp += 0xFFFFFFFFFFF40000LL;
                    }
                    if (temp > 0x10000000LL) {
                        temp = 0x10000000LL;
                    }
                    if (temp < -0x10000000LL) {
                        temp = -0x10000000LL;
                    }
                    steerAngle = calculateFixedAngleBetweenXZPoints((s32)temp, 0, 0, speed);
                    if (temp > 0) {
                        steerAngle -= 0x400;
                    }
                    if (temp < 0) {
                        steerAngle -= 0xC00;
                    }
                }
                steerAngle &= 0xFFF;
            }

            if (steerAngle < 0x801) {
                if (steerAngle >= 0x81) {
                    steerAngle = 0x80;
                }
            }
            if ((steerAngle >= 0x801) && (steerAngle < 0xF80)) {
                steerAngle = 0xF80;
            }
            player->facingAngle += steerAngle;
        } else {
            player->facingAngle += quickTurn;
        }

        rotation = 0;
        if (player->trailEffectTimer != 0) {
            rotation = -0x8000;
            if (player->snowboardTrailSide != 0) {
                rotation = 0x8000;
            }
        }
        if ((gMainMenuModeSelection != 0) && ((u8)player->unk519 == 6)) {
            rotation = -0x60000;
        }
        updateRacePlayerLocalVelocity(player, 0, rotation, player->lateralDeceleration, player->forwardDeceleration, player->reverseDeceleration);

        if (player->unk57A == 0) {
            if (player->isCpu == 0) {
                if (player->heldInput & A_BUTTON) {
                    player->mode = 2;
                    player->updateState = 0;
                    player->updateTimer = 0;
                }
            } else if (player->checkpointHit != 0) {
                player->mode = 2;
                player->updateState = 0;
                player->updateTimer = 0;
                player->unk525 = 1;
            } else if ((player->movementSpeed < 0x30000) || (player->localVelocityZ >= (s32)0xFFFD0001)) {
                player->mode = 2;
                player->updateState = 0;
                player->updateTimer = 0;
                player->unk525 = 0;
            }
            updateRacePlayerShotUse(player);
            updateRacePlayerItemUse(player);
        } else {
            player->unk57A--;
            player->stanceChangeTimer = 6;
        }

        if (player->stateFlags & 0x2000000) {
            player->mode = 0x1D;
            player->updateState = 0;
            player->updateTimer = 0;
        }
        if (player->stateFlags & 0x40) {
            player->mode = 0x1E;
            player->updateState = 0;
            player->updateTimer = 0;
        }
    } else {
        stepRaceMotionLoopingJointAnimation(player);
    }

    player->pos.x += player->velocity.x;
    player->pos.y += player->velocity.y;
    player->pos.z += player->velocity.z;
    updateRacePlayerMotionFeedback(player);
    if (player->unk517 != 0) {
        enqueueRacePlayerVoiceSound(player, RACE_VOICE_ATTACK_SUCCESS);
    }
}

void updateRacePlayerMode29Crash(RacePlayer *player) {
    s16 updateState;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateFlags &= ~0x200;
        player->modeValue = 0;
        player->unk80 = 0;
        player->subState = 0;
        player->subStateTimer = 0;
        player->modeAngle = 0;
        player->unk528 = 0;
        setRaceMotionAnimation(player, 0x1E);
    }

    player->velocity.y -= player->gravity;
    player->facingAngle = player->unk332;
    player->pitchAngle = player->unk334;
    if (player->stateFlags & 0x400) {
        player->facingAngle += 0x800;
        player->pitchAngle = -(player->pitchAngle & 0xFFF);
    }

    updateRacePlayerLeanAngle(player, 0, 0);
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    player->pos.x += player->velocity.x;
    player->pos.y += player->velocity.y;
    player->pos.z += player->velocity.z;

    if (stepRaceMotionAnimationUntilEnd(player) != 0) {
        setRaceMotionAnimation(player, 0x1F);
        stepRaceMotionAnimationUntilEnd(player);
    }

    player->unk578 = 6;
    updateRacePlayerMotionFeedback(player);
    if (player->unk517 != 0) {
        enqueueRacePlayerVoiceSound(player, RACE_VOICE_ATTACK_SUCCESS);
    }
}

void updateRacePlayerMode01JumpStart(RacePlayer *player) {
    s32 unused;
    s32 targetX;
    s32 targetZ;
    s16 angleDiff;
    s32 yVel;

    if (player->updateState == 0) {
        if (player->animationId != 4) {
            setRaceMotionAnimation(player, 4);
        }
        player->updateState++;
        player->stateFlags |= 0x200;
        player->targetGroundOffset = 0;
        resetRacePlayerTrickSubstate(player);
        player->aerialTrickId = 0;
    }

    if (player->subState == 0) {
        updateRacePlayerShotUse(player);
        if (player->isCpu == 0) {
            if (player->stickX > 0) {
                player->facingAngle -= 0x18;
            }
            if (player->stickX < 0) {
                player->facingAngle += 0x18;
            }
        } else {
            getRaceCourseTargetPositionAhead(
                player->coursePathIndex,
                unused = player->pos.x,
                player->pos.z,
                &targetX,
                &targetZ,
                (s8)player->unk17,
                (u16)player->playerIndex
            );
            angleDiff = (calculateFixedAngleBetweenXZPoints(player->pos.x, player->pos.z, targetX, targetZ) -
                         player->facingAngle) &
                        0xFFF;
            if (angleDiff >= 0x801) {
                angleDiff -= 0x1000;
            }
            if (angleDiff >= 0x19) {
                angleDiff = 0x18;
            }
            if (angleDiff < -0x18) {
                angleDiff = -0x18;
            }
            player->facingAngle += angleDiff;
        }
        stepRaceMotionAnimationUntilEnd(player);
    }

    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;
    if (player->unk517 != 0) {
        enqueueRacePlayerVoiceSound(player, RACE_VOICE_ATTACK_SUCCESS);
    }
}

void updateRacePlayerMode22Airborne(RacePlayer *player) {
    s32 unused;
    s32 targetX;
    s32 targetZ;
    s16 angleDiff;
    s32 yVel;

    if (player->updateState == 0) {
        player->updateState++;
        player->stateFlags |= 0x208;
        player->targetGroundOffset = 0;
        player->unk80 = 0;
        resetRacePlayerTrickSubstate(player);
        player->aerialTrickId = 0;
    }

    if (player->subState == 0) {
        updateRacePlayerShotUse(player);
        if (player->isCpu == 0) {
            if (player->stickX > 0) {
                player->facingAngle -= 0x18;
            }
            if (player->stickX < 0) {
                player->facingAngle += 0x18;
            }
        } else {
            getRaceCourseTargetPositionAhead(
                player->coursePathIndex,
                unused = player->pos.x,
                player->pos.z,
                &targetX,
                &targetZ,
                (s8)player->unk17,
                (u16)player->playerIndex
            );
            angleDiff = (calculateFixedAngleBetweenXZPoints(player->pos.x, player->pos.z, targetX, targetZ) -
                         player->facingAngle) &
                        0xFFF;
            if (angleDiff >= 0x801) {
                angleDiff -= 0x1000;
            }
            if (angleDiff >= 0x19) {
                angleDiff = 0x18;
            }
            if (angleDiff < -0x18) {
                angleDiff = -0x18;
            }
            player->facingAngle += angleDiff;
        }
    }

    updateRacePlayerTrickSubstate(player);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;
    if (player->unk517 != 0) {
        enqueueRacePlayerVoiceSound(player, RACE_VOICE_ATTACK_SUCCESS);
    }
}

void func_8008D91C(RacePlayer *player) {
    gRacePlayerAirborneUpdateHandlers[player->updateState](player);
}

void updateRacePlayerAirborneLaunch(RacePlayer *player) {
    RaceCourseStartEntry *spawn;
    s16 updateTimer;
    s32 velocityY;
    s32 rotation;
    s8 turnTimer;

    updateTimer = player->updateTimer;
    if (updateTimer == 0) {
        player->updateTimer = updateTimer + 1;
        player->modeValue = 0x40000;
        player->stateFlags |= 0x10;
        setRaceMotionAnimation(player, 5);
        resetRacePlayerSurfaceCueState(player);
        player->targetGroundOffset = 0x40000;
        player->aerialTrickId = 0;
        player->stanceTransitionFrame = 0;
        player->unk336 = 0;
    }

    if (player->isCpu == 0) {
        spawn = &gRaceCourseStartEntries[gRaceCourseIndex.signedValue];
        if ((spawn->pathIndex == player->coursePathIndex) && !(player->stateFlags & 0x40)) {
            velocityY = (s16)(((calculateFixedAngleBetweenXZPoints(
                                    player->pos.x,
                                    player->pos.z,
                                    spawn->startSurfaceTargetX,
                                    spawn->startSurfaceTargetZ
                                ) -
                                player->facingAngle) +
                               0x400) &
                              0xFFF);
            if (velocityY < 0x800) {
                if (player->stateFlags & 0x400) {
                    player->stateFlags &= ~0x400;
                    player->stanceTransitionFrame = 6 - player->stanceTransitionFrame;
                }
            } else if (!(player->stateFlags & 0x400)) {
                player->stateFlags |= 0x400;
                player->stanceTransitionFrame = 6 - player->stanceTransitionFrame;
            }
        } else {
            if (player->localVelocityZ < -0x8000) {
                if (player->stateFlags & 0x400) {
                    player->stateFlags &= ~0x400;
                    player->stanceTransitionFrame = 6 - player->stanceTransitionFrame;
                }
            }
            if (player->localVelocityZ >= 0x8001) {
                if (!(player->stateFlags & 0x400)) {
                    player->stateFlags |= 0x400;
                    player->stanceTransitionFrame = 6 - player->stanceTransitionFrame;
                }
            }
        }
    }

    turnTimer = player->stanceTransitionFrame;
    if (turnTimer != 0) {
        player->stanceTransitionFrame = turnTimer - 1;
        setRaceMotionAnimation(player, player->stanceTransitionFrame + 8);
    } else if (player->animationId != 5) {
        setRaceMotionAnimation(player, 5);
    }

    updateRacePlayerShotUse(player);
    stepRaceMotionJointAnimationUntilEnd(player);

    velocityY = player->targetGroundOffset;
    if (velocityY != 0) {
        if (velocityY < 0x20000) {
            player->targetGroundOffset = 0;
        } else {
            player->targetGroundOffset = velocityY - 0x20000;
        }
    } else {
        player->updateState++;
    }

    if ((player->isCpu == 0) && !(player->heldInput & A_BUTTON)) {
        player->stateFlags &= ~0x10;
    }

    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->gravity;

    rotation = 0;
    if (player->trailEffectTimer != 0) {
        rotation = -0x8000;
        if (player->snowboardTrailSide != 0) {
            rotation = 0x8000;
        }
    }

    updateRacePlayerLocalVelocity(player, 0, rotation, player->lateralDeceleration, player->forwardDeceleration, player->reverseDeceleration);
    player->pos.x += player->velocity.x;
    player->pos.y += player->velocity.y;
    player->pos.z += player->velocity.z;
    updateRacePlayerSurfaceCue(player);
    updateRacePlayerMotionFeedback(player);
    if (player->unk517 != 0) {
        enqueueRacePlayerVoiceSound(player, RACE_VOICE_ATTACK_SUCCESS);
    }
}

#pragma GLOBAL_ASM("asm.jp/nonmatchings/race/player/race_player_update/func_8008DC9C.s")

void resetRacePlayerTrickSubstate(RacePlayer *player) {
    player->subState = 0;
    player->subStateTimer = 0;
    player->subStateStep = 0;
    player->subStateParam = 0;
    player->grabInputFrames = 0;
}

void updateRacePlayerTrickSubstate(RacePlayer *player) {
    gRacePlayerTrickSubstateHandlers[player->subState](player);
}

void updateRacePlayerTrickSubstateStart(RacePlayer *player) {
    s16 inputMask;

    player->subStateTimer = 0;
    player->subStateStep = 0;
    player->grabTrickId = 0;
    player->grabHoldFrames = 0;
    if (player->isCpu == 0) {
        if (player->pressedInput & R_CBUTTONS) {
            player->subStateTimer = R_CBUTTONS;
            player->grabTrickId = 1;
        }
        if (player->pressedInput & L_CBUTTONS) {
            player->subStateTimer = L_CBUTTONS;
            player->grabTrickId = 3;
        }
        if (player->pressedInput & U_CBUTTONS) {
            player->subStateTimer = U_CBUTTONS;
            player->grabTrickId = 7;
        }
        if (player->pressedInput & D_CBUTTONS) {
            player->subStateTimer = D_CBUTTONS;
            player->grabTrickId = 5;
        }
        if (player->subStateTimer != 0) {
            if (player->grabInputFrames < 6) {
                player->grabTrickId++;
            }
            player->subState++;
            if (player->stateFlags & 0x400) {
                inputMask = player->subStateTimer;
                if (inputMask & R_CBUTTONS) {
                    setRaceMotionAnimation(player, 0x17);
                    inputMask = player->subStateTimer;
                }
                if (inputMask & L_CBUTTONS) {
                    setRaceMotionAnimation(player, 0x15);
                    inputMask = player->subStateTimer;
                }
            } else {
                inputMask = player->subStateTimer;
                if (inputMask & R_CBUTTONS) {
                    setRaceMotionAnimation(player, 0x15);
                    inputMask = player->subStateTimer;
                }
                if (inputMask & L_CBUTTONS) {
                    setRaceMotionAnimation(player, 0x17);
                    inputMask = player->subStateTimer;
                }
            }
            if (inputMask & U_CBUTTONS) {
                setRaceMotionAnimation(player, 0x19);
                inputMask = player->subStateTimer;
            }
            if (inputMask & D_CBUTTONS) {
                setRaceMotionAnimation(player, 0x1B);
            }
            player->stateFlags |= 0x800;
        }
        player->grabInputFrames++;
    }
}

void updateRacePlayerTrickSubstateHold(RacePlayer *player) {
    s16 inputMask;

    if (!(player->heldInput & player->subStateTimer)) {
        player->subStateStep = 1;
    }
    if (stepRaceMotionAnimationUntilEnd(player) != 0) {
        if (player->subStateStep != 0) {
            player->subState++;
            if (player->stateFlags & 0x400) {
                inputMask = player->subStateTimer;
                if (inputMask & R_CBUTTONS) {
                    setRaceMotionAnimation(player, 0x18);
                    inputMask = player->subStateTimer;
                }
                if (inputMask & L_CBUTTONS) {
                    setRaceMotionAnimation(player, 0x16);
                    inputMask = player->subStateTimer;
                }
            } else {
                inputMask = player->subStateTimer;
                if (inputMask & R_CBUTTONS) {
                    setRaceMotionAnimation(player, 0x16);
                    inputMask = player->subStateTimer;
                }
                if (inputMask & L_CBUTTONS) {
                    setRaceMotionAnimation(player, 0x18);
                    inputMask = player->subStateTimer;
                }
            }
            if (inputMask & U_CBUTTONS) {
                setRaceMotionAnimation(player, 0x1A);
                inputMask = player->subStateTimer;
            }
            if (inputMask & D_CBUTTONS) {
                setRaceMotionAnimation(player, 0x1C);
            }
        } else {
            player->grabHoldFrames++;
        }
    }
    player->stateFlags |= 0x800;
}

void updateRacePlayerTrickSubstateFinish(RacePlayer *player) {
    if (stepRaceMotionAnimationUntilEnd(player) != 0) {
        player->subState++;
    }
    player->stateFlags |= 0x800;
}

void updateRacePlayerTrickSubstateNoop(RacePlayer *player) {
}

void updateRacePlayerMode13AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 stateTimer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }

    if (player->subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }

    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    player->modelRotationX = (s16)((s32)(fixedSine(player->modeValue) * 0x1000) / 0x1000);
    player->modeValue += 0x20;
    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }
    stateTimer = player->modeValue;
    player->stateFlags |= 2;
    if (stateTimer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(
                initRacePlayerYellowTrickSparkles,
                5,
                2,
                (u16)player->playerIndex
            );
        }
    }
}

void updateRacePlayerMode15AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 stateTimer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }

    if (player->subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }

    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    player->modelRotationX = (s16)((s32)(fixedSine(player->modeValue) * -0x1000) / 0x1000);
    player->modeValue += 0x20;
    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }
    stateTimer = player->modeValue;
    player->stateFlags |= 2;
    if (stateTimer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(
                initRacePlayerYellowTrickSparkles,
                5,
                2,
                (u16)player->playerIndex
            );
        }
    }
}

void updateRacePlayerMode16AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }
    if (player->subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }
    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;
    if (player->stateFlags & 0x400) {
        player->modelRotationY = (s16)((s32)(fixedSine(player->modeValue) << 0xC) / 4096);
    } else {
        player->modelRotationY = (s16)((s32)(fixedSine(player->modeValue) * -0x1000) / 4096);
    }

    player->modeValue += 0x1E;
    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }
    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(
                initRacePlayerYellowTrickSparkles,
                5,
                2,
                (u16)player->playerIndex
            );
        }
    }
}

void updateRacePlayerMode17AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }
    if (player->subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }
    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;
    if (player->stateFlags & 0x400) {
        player->modelRotationY = (s16)((s32)(fixedSine(player->modeValue) * -0x1000) / 0x1000);
    } else {
        player->modelRotationY = (s16)((s32)(fixedSine(player->modeValue) << 12) / 0x1000);
    }

    player->modeValue += 0x1D;
    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }
    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(
                initRacePlayerYellowTrickSparkles,
                5,
                2,
                (u16)player->playerIndex
            );
        }
    }
}

void updateRacePlayerMode18AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }
    if (player->subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }
    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;
    player->modelRotationX = (s16)((s32)(fixedSine((s16)player->modeValue) << 12) / 0x1000);
    if (player->stateFlags & 0x400) {
        player->modelRotationY = (s16)((s32)(fixedSine((s16)player->modeValue) << 12) / 0x1000);
    } else {
        player->modelRotationY = (s16)((s32)(fixedSine((s16)player->modeValue) * -0x1000) / 0x1000);
    }

    player->modeValue += 0x1A;
    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }
    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(
                initRacePlayerYellowTrickSparkles,
                5,
                2,
                (u16)player->playerIndex
            );
        }
    }
}

void updateRacePlayerMode19AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }
    if (player->subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }
    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;
    player->modelRotationX = (s16)((s32)(fixedSine(player->modeValue) << 12) / 0x1000);
    if (player->stateFlags & 0x400) {
        player->modelRotationY = (s16)((s32)(fixedSine(player->modeValue) * -0x1000) / 0x1000);
    } else {
        player->modelRotationY = (s16)((s32)(fixedSine(player->modeValue) << 12) / 0x1000);
    }

    player->modeValue += 0x1A;
    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }
    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(
                initRacePlayerYellowTrickSparkles,
                5,
                2,
                (u16)player->playerIndex
            );
        }
    }
}

void updateRacePlayerMode20AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }
    if (player->subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }
    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;
    player->modelRotationX = (s16)((s32)(fixedSine(player->modeValue) * -0x1000) / 0x1000);
    if (player->stateFlags & 0x400) {
        player->modelRotationY = (s16)((s32)(fixedSine(player->modeValue) << 12) / 0x1000);
    } else {
        player->modelRotationY = (s16)((s32)(fixedSine(player->modeValue) * -0x1000) / 0x1000);
    }

    player->modeValue += 0x1A;
    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }
    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(
                initRacePlayerYellowTrickSparkles,
                5,
                2,
                (u16)player->playerIndex
            );
        }
    }
}

void updateRacePlayerMode21AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }
    if (player->subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }
    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;
    player->modelRotationX = (s16)((s32)(fixedSine(player->modeValue) * -0x1000) / 0x1000);
    if (player->stateFlags & 0x400) {
        player->modelRotationY = (s16)((s32)(fixedSine(player->modeValue) * -0x1000) / 0x1000);
    } else {
        player->modelRotationY = (s16)((s32)(fixedSine(player->modeValue) << 12) / 0x1000);
    }

    player->modeValue += 0x1A;
    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }
    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(
                initRacePlayerYellowTrickSparkles,
                5,
                2,
                (u16)player->playerIndex
            );
        }
    }
}

void updateRacePlayerMode31AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x15);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    if (player->stateFlags & 0x400) {
        player->modelRotationY = (fixedSine(player->modeValue) * -0x2000) / 4096;
    } else {
        player->modelRotationY = (fixedSine(player->modeValue) * 0x2000) / 4096;
    }

    player->modeValue += 0x16;
    player->updateTimer++;
    updateTimer = player->updateTimer;
    if (updateTimer == 8) {
        setRaceMotionAnimation(player, 0x16);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0xF) {
        setRaceMotionAnimation(player, 0x1B);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x1C);
    }

    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode41AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    player->modelRotationX = (fixedSine(player->modeValue) << 13) / 0x1000;

    player->modeValue += 0x14;
    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode42AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    player->modelRotationZ = (fixedSine(player->modeValue) << 13) / 0x1000;

    player->modeValue += 0x14;
    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode43AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);

    updateTimer = player->updateTimer;
    if (updateTimer < 8) {
        player->pitchAngle = -updateTimer << 7;
    }
    if (updateTimer >= 0x23) {
        player->pitchAngle = (-(-updateTimer) << 6) - 0xC80;
    }
    if (updateTimer < 0x32) {
        player->updateTimer = updateTimer + 1;
    }

    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    player->modelRotationY = (fixedSine(player->modeValue) << 14) / 0x1000;

    player->modeValue += 0x14;
    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode32AerialTrick(RacePlayer *player) {
    s16 updateState;
    s32 yVel;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x28);
        player->modeValue = 0;
        player->stateFlags |= 0x200;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
    }

    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    if (stepRaceMotionAnimationUntilEnd(player) == 0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode33AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 0x23);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    player->modelRotationX = (fixedSine(player->modeValue) * -0x3000) / 4096;
    if (player->stateFlags & 0x400) {
        player->modelRotationY = -player->updateTimer;
    } else {
        player->modelRotationY = player->updateTimer;
    }
    if (player->updateTimer != 0x1000) {
        player->updateTimer += 0x80;
    }

    player->modeValue += 0x11;
    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode34AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 0x23);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->modelRotationX = player->modeValue * 4;
    player->verticalVelocityLimit = yVel;

    if (player->stateFlags & 0x400) {
        player->modelRotationY = (fixedSine((s16)player->modeValue) * -0x4000) / 0x1000;
    } else {
        player->modelRotationY = (fixedSine((s16)player->modeValue) << 0xE) / 0x1000;
    }

    player->modeValue += 0x15;
    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode44AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x17);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    if (player->stateFlags & 0x400) {
        player->modelRotationY = (fixedSine(player->modeValue) * -0x2000) / 4096;
    } else {
        player->modelRotationY = (fixedSine(player->modeValue) * 0x2000) / 4096;
    }

    player->modeValue += 0x16;
    player->updateTimer++;
    if (player->updateTimer == 8) {
        setRaceMotionAnimation(player, 0x18);
    }
    if (player->updateTimer == 0xF) {
        setRaceMotionAnimation(player, 0x19);
    }
    if (player->updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x1A);
    }

    timer = player->modeValue;
    if (timer >= 0x401) {
        timer = 0x400;
        player->modeValue = timer;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode45AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    player->updateTimer = (fixedSine(player->modeValue) << 13) / 0x1000;
    updateTimer = player->updateTimer;
    if (updateTimer >= 0x1001) {
        player->modelRotationY = 0x2000 - updateTimer;
    } else {
        player->modelRotationY = updateTimer;
    }

    if (player->stateFlags & 0x400) {
        player->modelRotationY = -player->modelRotationY;
    }

    player->modelRotationX = (fixedSine(player->modeValue) << 13) / 0x1000;
    player->modeValue += 0x14;
    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode46AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x1B);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    if (player->stateFlags & 0x400) {
        player->modelRotationY = (fixedSine(player->modeValue) * -0x5000) / 0x1000;
    } else {
        player->modelRotationY = (fixedSine(player->modeValue) * 0x5000) / 0x1000;
    }

    player->modeValue += 0x12;
    player->updateTimer++;
    if (player->updateTimer == 0x28) {
        setRaceMotionAnimation(player, 0x1C);
    }

    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode35AerialTrick(RacePlayer *player) {
    s16 updateState;
    s32 yVel;
    s32 stateTimer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x15);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    if (player->stateFlags & 0x400) {
        player->modelRotationZ = (fixedSine(player->modeValue) * -0x2000) / 0x1000;
    } else {
        player->modelRotationZ = (fixedSine(player->modeValue) << 13) / 0x1000;
    }

    player->modeValue += 0x16;
    player->updateTimer++;
    if (player->updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x16);
    }

    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    stateTimer = player->modeValue;
    player->stateFlags |= 2;
    if (stateTimer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode36AerialTrick(RacePlayer *player) {
    s16 updateState;
    s32 yVel;
    s32 timer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x15);
        player->updateTimer = 0;
    }
    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    if (player->stateFlags & 0x400) {
        player->modelRotationY = (s16)((s32)(fixedSine(player->modeValue) * -0x3000) / 4096);
    } else {
        player->modelRotationY = (s16)((s32)(fixedSine(player->modeValue) * 0x3000) / 4096);
    }

    player->modeValue += 0x15;
    player->updateTimer++;
    if (player->updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x16);
    }

    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode47AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x19);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    if (player->stateFlags & 0x400) {
        player->modelRotationY = (fixedSine(player->modeValue) * -0x2000) / 4096;
    } else {
        player->modelRotationY = (fixedSine(player->modeValue) << 0xD) / 4096;
    }

    player->modeValue += 0x16;
    player->updateTimer++;
    updateTimer = player->updateTimer;
    if (updateTimer == 8) {
        setRaceMotionAnimation(player, 0x1A);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0xF) {
        setRaceMotionAnimation(player, 0x15);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x16);
    }

    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode48AerialTrick(RacePlayer *player) {
    s16 updateState;
    s32 yVel;
    s32 stateTimer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x15);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    if (player->stateFlags & 0x400) {
        player->modelRotationZ = (fixedSine(player->modeValue) * 0x3000) / 0x1000;
    } else {
        player->modelRotationZ = (fixedSine(player->modeValue) * -0x3000) / 0x1000;
    }

    player->modeValue += 0x16;
    if (player->updateTimer < 0x2D) {
        player->updateTimer = player->updateTimer + 1;
    }

    if (player->updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x16);
    }

    stateTimer = player->modeValue;
    player->modelRotationY = (player->updateTimer << 0xC) / 45;
    if (stateTimer >= 0x401) {
        player->modeValue = 0x400;
    }

    stateTimer = player->modeValue;
    player->stateFlags |= 2;
    if (stateTimer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode49AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    player->modelRotationX = (fixedSine(player->modeValue) * -0x3000) / 0x1000;

    player->modeValue += 0x12;
    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode37AerialTrick(RacePlayer *player) {
    s16 updateState;
    s16 updateTimer;
    s16 tilt;
    s16 *updateTimerPtr;
    s32 yVel;
    s32 timer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x23);
        player->modeValue = 0;
        player->stateFlags |= 0x200;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
        player->unk306 = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = (u32)player->velocity.y;
    player->pos.x += player->velocity.x;
    updateTimerPtr = &player->updateTimer;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    if (player->stateFlags & 0x400) {
        player->modelRotationZ = (fixedSine(player->modeValue) * -0x2000) / 4096;
    } else {
        player->modelRotationZ = (fixedSine(player->modeValue) << 0xD) / 4096;
    }

    player->modeValue += 0x16;
    player->updateTimer++;
    updateTimer = player->updateTimer;
    if (updateTimer == 0xC) {
        setRaceMotionAnimation(player, 0x15);
        updateTimer = *updateTimerPtr;
    }

    if (updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x16);
        updateTimer = player->updateTimer;
    }

    if ((updateTimer >= 9) && (updateTimer < 0x14)) {
        tilt = player->unk306;
        if (tilt != -0x400) {
            player->unk306 = tilt - 0x80;
            updateTimer = player->updateTimer;
        }
    }

    tilt = player->unk306;
    if ((updateTimer >= 0x1F) && (tilt != 0)) {
        player->unk306 = tilt + 0x80;
        tilt = player->unk306;
    }

    if (player->stateFlags & 0x400) {
        player->modelRotationY = -tilt;
    } else {
        player->modelRotationY = tilt;
    }

    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode50AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x19);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    if (player->stateFlags & 0x400) {
        player->modelRotationY = (fixedSine(player->modeValue) * -0x2000) / 4096;
    } else {
        player->modelRotationY = (fixedSine(player->modeValue) * 0x2000) / 4096;
    }

    player->modeValue += 0x16;
    player->updateTimer++;
    updateTimer = player->updateTimer;
    if (updateTimer == 8) {
        setRaceMotionAnimation(player, 0x1A);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0xF) {
        setRaceMotionAnimation(player, 0x17);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x18);
    }

    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode51AerialTrick(RacePlayer *player) {
    s16 updateState;
    s16 updateTimer;
    RacePlayer *playerAlias;
    s16 tilt;
    s32 yVel;
    s32 timer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x23);
        player->modeValue = 0;
        player->stateFlags |= 0x200;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
        player->unk306 = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    playerAlias = player;
    playerAlias->velocity.y -= playerAlias->aerialGravity;
    clampRacePlayerVectorXZSpeed(&playerAlias->velocity, player);

    yVel = player->velocity.y;
    playerAlias->pos.x += player->velocity.x;
    player->pos.y += yVel;
    playerAlias->pos.z += player->velocity.z;
    playerAlias->verticalVelocityLimit = yVel;

    if (playerAlias->stateFlags & 0x400) {
        playerAlias->modelRotationZ = (fixedSine((s16)player->modeValue) * 0x3000) / 4096;
    } else {
        player->modelRotationZ = (fixedSine((s16)player->modeValue) * -0x3000) / 4096;
    }

    playerAlias->modeValue += 0x14;
    player->updateTimer++;
    updateTimer = player->updateTimer;
    if (updateTimer == 0xC) {
        setRaceMotionAnimation(playerAlias, 0x15);
        updateTimer = playerAlias->updateTimer;
    }

    if (updateTimer == 0x1E) {
        setRaceMotionAnimation(playerAlias, 0x16);
        updateTimer = playerAlias->updateTimer;
    }

    if ((updateTimer >= 9) && (updateTimer < 0x14)) {
        tilt = player->unk306;
        if (tilt != 0x400) {
            player->unk306 = tilt + 0x80;
            updateTimer = playerAlias->updateTimer;
        }
    }

    tilt = player->unk306;
    if ((updateTimer >= 0x1F) && (tilt != 0)) {
        player->unk306 = tilt - 0x80;
        tilt = player->unk306;
    }

    if (playerAlias->stateFlags & 0x400) {
        playerAlias->modelRotationY = -tilt;
    } else {
        player->modelRotationY = tilt;
    }

    timer = player->modeValue;
    if (timer >= 0x401) {
        timer = 0x400;
        playerAlias->modeValue = timer;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        playerAlias->stateFlags |= 0x800;
        if ((playerAlias->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(
                initRacePlayerBlueTrickSparkles,
                5,
                2,
                (u16)playerAlias->playerIndex
            );
        }
    }
}

void updateRacePlayerMode52AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 stateTimer;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x1B);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    if (player->stateFlags & 0x400) {
        player->modelRotationY = (fixedSine(player->modeValue) * -0x3000) / 0x1000;
    } else {
        player->modelRotationY = (fixedSine(player->modeValue) * 0x3000) / 0x1000;
    }

    player->modeValue += 0x16;
    player->updateTimer++;
    if (player->updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x1C);
    }

    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode38AerialTrick(RacePlayer *player) {
    s16 updateState;
    s32 yVel;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x23);
        player->modeValue = 0;
        player->stateFlags |= 0x200;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
    }

    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    if (stepRaceMotionAnimationUntilEnd(player) == 0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode53AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x19);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    if (player->stateFlags & 0x400) {
        player->modelRotationY = (fixedSine(player->modeValue) * -0x1800) / 4096;
    } else {
        player->modelRotationY = ((fixedSine(player->modeValue) * 6) * 1024) / 4096;
    }

    player->modeValue += 0x16;
    player->updateTimer++;
    updateTimer = player->updateTimer;
    if (updateTimer == 8) {
        setRaceMotionAnimation(player, 0x1A);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0xF) {
        setRaceMotionAnimation(player, 0x1B);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x1C);
    }

    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode54AerialTrick(RacePlayer *player) {
    s16 updateState;
    s32 yVel;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x28);
        player->modeValue = 0;
        player->stateFlags |= 0x200;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
    }

    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    if (stepRaceMotionAnimationUntilEnd(player) == 0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode39AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    updateTimer = player->updateTimer;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    switch (updateTimer) {
        case 0:
            player->modelRotationX = (s16)((s32)(fixedSine(player->modeValue) << 12) / 4096);
            player->modeValue += 0x33;
            if (player->modeValue >= 0x401) {
                player->modeValue = 0;
                player->updateTimer++;
                player->unk306 = 0;
            }

            player->stateFlags |= 0x800;
            if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
                createCallbackTaskWithUserIdPreservingArgs(
                    initRacePlayerBlueTrickSparkles,
                    5,
                    2,
                    (u16)player->playerIndex
                );
            }
            break;
        case 1:
            if (player->stateFlags & 0x400) {
                player->modelRotationY = (s16)((s32)(fixedSine(player->modeValue) * -0x2000) / 4096);
            } else {
                player->modelRotationY = (s16)((s32)(fixedSine(player->modeValue) << 13) / 4096);
            }

            player->modelRotationX = -player->unk306;
            player->modeValue += 0x20;
            if (player->modeValue >= 0x401) {
                player->modeValue = 0x400;
            }

            if (player->unk306 != 0x1000) {
                player->unk306 += 0x80;
            }

            if (player->modeValue < 0x3D0) {
                player->stateFlags |= 0x800;
                if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
                    createCallbackTaskWithUserIdPreservingArgs(
                        initRacePlayerBlueTrickSparkles,
                        5,
                        2,
                        (u16)player->playerIndex
                    );
                }
            }
            break;
    }

    player->stateFlags |= 2;
}

void updateRacePlayerMode55AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    if (player->stateFlags & 0x400) {
        player->modelRotationY = (s16)((s32)(fixedSine(player->modeValue) * -0x2000) / 4096);
    } else {
        player->modelRotationY = (s16)((s32)(fixedSine(player->modeValue) << 0xD) / 4096);
    }

    player->modeValue += 0x16;
    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode56AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    if (player->stateFlags & 0x400) {
        player->modelRotationY = (fixedSine(player->modeValue) * -0x5000) / 0x1000;
    } else {
        player->modelRotationY = (fixedSine(player->modeValue) * 0x5000) / 0x1000;
    }

    player->modeValue += 0x14;
    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode57AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y -= player->aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->verticalVelocityLimit = yVel;

    player->modelRotationX = (fixedSine(player->modeValue) * -0x5000) / 0x1000;

    player->modeValue += 0x14;
    if (player->modeValue >= 0x401) {
        player->modeValue = 0x400;
    }

    timer = player->modeValue;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode03Nudge(RacePlayer *player) {
    RacePlayer *playerAlias;
    RaceCourseStartEntry *spawn;
    s16 updateState;
    s16 angleDiff;
    s32 clamped;
    s32 tempX;
    s32 tempZ;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x2000;
        if (((player->unk2DE - player->facingAngle) & 0xFFF) < 0x800) {
            setRaceMotionAnimation(player, 7);
        } else {
            setRaceMotionAnimation(player, 6);
        }
        player->velocity.x = player->unk2E0;
        player->velocity.z = player->unk2E4;
        player->rumbleTimer = 3;
        player->rumblePatternId = 0;
    }

    updateRacePlayerLeanAngle(player, 0x10000, 0);
    playerAlias = player;
    player->speedLimit = 0x60000;
    if (!(playerAlias->stateFlags & 1)) {
        playerAlias->stateFlags &= ~0x200;
        playerAlias->velocity.y -= 0xA000;
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, (unsigned long long)0x2000, 0x2000, 0x2000);
    } else {
        playerAlias->stateFlags |= 0x200;
        player->velocity.y -= 0x7000;
        clampRacePlayerVectorXZSpeed(&player->velocity, player);

        tempX = playerAlias->velocity.x;
        clamped = tempX;
        if (clamped >= 0x2001) {
            clamped = 0x2000;
        }
        updateState = -0x2000;
        if (clamped < -0x2000) {
            clamped = updateState;
        }
        player->velocity.x = tempX - clamped;

        tempZ = playerAlias->velocity.z;
        clamped = tempZ;
        if (clamped >= 0x2001) {
            clamped = 0x2000;
        }
        if (clamped < updateState) {
            clamped = updateState;
        }
        player->velocity.z = tempZ - clamped;
    }

    playerAlias->pos.x += playerAlias->velocity.x;
    playerAlias->pos.y += player->velocity.y;
    playerAlias->pos.z += playerAlias->velocity.z;

    spawn = &gRaceCourseStartEntries[gRaceCourseIndex.signedValue];
    if ((spawn->pathIndex == playerAlias->coursePathIndex) && !(playerAlias->stateFlags & 0x40)) {
        angleDiff = calculateFixedAngleBetweenXZPoints(
            playerAlias->pos.x,
            player->pos.z,
            spawn->startSurfaceTargetX,
            spawn->startSurfaceTargetZ
        );
        if (playerAlias->stateFlags & 0x400) {
            angleDiff += 0x800;
        }

        angleDiff = (angleDiff - player->facingAngle) & 0xFFF;
        if (angleDiff >= 0x801) {
            angleDiff -= 0x1000;
        }
        if (angleDiff >= 0x97) {
            angleDiff = 0x96;
        }
        if (angleDiff < -0x96) {
            angleDiff = -0x96;
        }
        playerAlias->facingAngle += angleDiff;
    }

    if (!(player->stateFlags & 1)) {
        updateRacePlayerMotionFeedback(player);
    }

    if (stepRaceMotionAnimationUntilEnd(player) != 0) {
        playerAlias->stateFlags &= 0xFE0C1FFB;
        if (playerAlias->stateFlags & 0x200) {
            player->mode = 1;
            playerAlias->updateState = 0;
            player->updateTimer = 0;
        } else {
            playerAlias->mode = 0;
            playerAlias->updateState = 0;
            playerAlias->updateTimer = 0;
        }
    }
}

void updateRacePlayerMode04Spinout(RacePlayer *player) {
    s16 updateState;
    s32 tempX;
    s32 tempZ;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0xE);
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x12204;
        player->pitchAngle = 0;
        updateRacePlayerLeanAngle(player, 0, 0);
        player->velocity.x = player->unk2E0;
        player->velocity.z = player->unk2E4;
        player->velocity.y = 0x20000;
        player->targetGroundOffset = 0;
        player->rumbleTimer = 3;
        player->rumblePatternId = 0;
    }

    player->speedLimit = 0x60000;
    if (player->stateFlags & 1) {
        clampRacePlayerVectorXZSpeed(&player->velocity, player);
        tempX = player->velocity.x;
        tempZ = player->velocity.z;
        player->velocity.y -= 0xA000;
        player->velocity.x = tempX - (tempX / 15);
        player->velocity.z = tempZ - (tempZ / 15);
    } else {
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x3000, 0x3000, 0x3000);
    }

    player->pos.x += player->velocity.x;
    player->pos.y += player->velocity.y;
    player->pos.z += player->velocity.z;

    if ((stepRaceMotionAnimationUntilEnd(player) != 0) && !(player->stateFlags & 1)) {
        player->mode = 5;
        player->updateState = 0;
        player->updateTimer = 0;
    }
}

void updateRacePlayerMode05SpinoutStun(RacePlayer *player) {
    s16 updateState;
    s32 stateTimer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->modeValue = 0x1E;
        if (player->stateFlags & 0x800000) {
            player->modeValue = 0x5A;
            spawnRaceUiStunOrbitingIcons(player->playerIndex);
        }
        setRaceMotionAnimation(player, 0xF);
        stateTimer = player->modeValue;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags &= ~0x200;
        player->stateFlags |= 0x1012000;
        player->targetGroundOffset = 0;
        player->modeValue = stateTimer;
        /* Rank-scaled recovery timer for humans and CPUs. With zero-based rank,
         * integer truncation gives 60/53/45/38 updates for a base of 60. */
        player->modeValue = stateTimer - ((player->modeValue * player->rankIndex) / 8);
        player->rumbleTimer = 1;
        player->rumblePatternId = 0;
    }

    updateRacePlayerLeanAngle(player, 0, 0);
    if (!(player->stateFlags & 1)) {
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x4000, 0x4000, 0x4000);
    }

    player->velocity.y -= 0xA000;
    player->pos.y += player->velocity.y;

    if (!(player->stateFlags & 1)) {
        player->modeValue = player->modeValue - getRaceInputTimerDecrementBonus(player) - 1;
        if (player->modeValue < 0) {
            player->modeValue = 0;
        }
        if (stepRaceMotionAnimationUntilEnd(player) != 0) {
            player->stateFlags &= ~0x200;
            if (player->modeValue == 0) {
                player->mode = 8;
                player->updateState = 0;
                player->updateTimer = 0;
            }
        }
    } else {
        player->stateFlags |= 0x200;
        setRaceMotionAnimation(player, 0xF);
        stepRaceMotionAnimationUntilEnd(player);
        if (player->modeValue < 0x1E) {
            player->modeValue = 0x1E;
        }
        if (!(player->stateFlags & 1)) {
            player->rumbleTimer = 1;
            player->rumblePatternId = 0;
        }
    }
}

void updateRacePlayerMode08SpinoutRecover(RacePlayer *player) {
    RaceCourseStartEntry *spawn;
    s16 updateState;
    s16 targetAngle;
    s16 facingAngle;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        if (player->stateFlags & 0x10000) {
            setRaceMotionAnimation(player, 0x11);
        } else {
            setRaceMotionAnimation(player, 0x10);
        }
        player->stateFlags = player->stateFlags & 0xFE0C1FFB;
        player->stateFlags = player->stateFlags & ~0x200;
        player->stateFlags = player->stateFlags | 0xA000;
        player->targetGroundOffset = 0x40000;
    }

    if (player->stateFlags & 1) {
        player->stateFlags |= 0x200;
    } else {
        player->stateFlags &= ~0x200;
    }

    spawn = &gRaceCourseStartEntries[gRaceCourseIndex.signedValue];
    if ((spawn->pathIndex == player->coursePathIndex) && !(player->stateFlags & 0x40)) {
        targetAngle = calculateFixedAngleBetweenXZPoints(
            player->pos.x,
            player->pos.z,
            spawn->startSurfaceTargetX,
            spawn->startSurfaceTargetZ
        );
        if (player->stateFlags & 0x400) {
            targetAngle += 0x800;
        }

        facingAngle = player->facingAngle;
        targetAngle = (targetAngle - facingAngle) & 0xFFF;
        if (targetAngle >= 0x801) {
            targetAngle -= 0x1000;
        }
        if (targetAngle >= 0x67) {
            targetAngle = 0x66;
        }
        if (targetAngle < -0x66) {
            targetAngle = -0x66;
        }
        player->facingAngle = facingAngle + targetAngle;
    }

    updateRacePlayerLeanAngle(player, 0, 0);
    player->velocity.y -= 0xA000;
    player->pos.y += player->velocity.y;

    if (stepRaceMotionAnimationUntilEnd(player) != 0) {
        player->stateFlags &= 0xFE0C1FFB;
        player->mode = 0;
        player->updateState = 0;
        player->updateTimer = 0;
    }
}

void updateRacePlayerMode06TerrainFall(RacePlayer *player) {
    if (player->updateState == 0) {
        player->updateState++;
        player->modeValue = 0x3C;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x42000;
        player->updateTimer = 0;
        setRaceCameraMode(player->playerIndex, 4);
        player->targetGroundOffset = 0;
        player->unk2E8 = player->coursePathIndex;
        if (player->soundDisabled == 0) {
            spawnRaceItemBreakParticles(player->playerIndex, player->surfaceType);
        }
        /* Rank-scaled recovery timer for humans and CPUs. With zero-based rank,
         * integer truncation gives 60/53/45/38 updates for a base of 60. */
        player->modeValue -= (player->modeValue * player->rankIndex) / 8;
        player->rumbleTimer = 2;
        player->rumblePatternId = 0;
        setRaceMotionAnimation(player, 0x20);
    }

    switch (player->updateTimer) {
        case 0:
            player->modeValue--;
            if (player->modeValue == 0) {
                player->updateTimer++;
            }
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                setRaceMotionAnimation(player, 0x21);
            }
            break;
        case 1:
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                setRaceMotionAnimation(player, 0x21);
            }
            player->stateFlags |= 0x80000;
            if (gViewportStates[(u16)player->playerIndex].overlayAlpha == 0xFF) {
                player->updateTimer++;
                player->verticalVelocityLimit = 0;
                player->coursePathIndex = player->unk2E8;
                while (1) {
                    getRaceCourseSurfaceSpawnTransform(
                        player->coursePathIndex,
                        &player->pos.x,
                        &player->pos.y,
                        &player->pos.z,
                        &player->facingAngle
                    );
                    if (isRacePlayerRespawnSurfaceValid(player) == 0) {
                        player->coursePathIndex--;
                    } else {
                        break;
                    }
                }
                player->pos.y = getRaceCourseSurfaceHeight(player->coursePathIndex, player->pos.x, player->pos.z);
                player->previousPosition = player->pos;
                player->velocity.x = 0;
                player->velocity.y = 0;
                player->velocity.z = 0;
                player->stateFlags &= ~0x400;
                player->leanAngle = 0;
                updateRacePlayerLeanAngle(player, 0, 0);
                setRaceMotionAnimation(player, 1);
                stepRaceMotionAnimationUntilEnd(player);
                setRaceCameraMode(player->playerIndex, 1);
                player->targetGroundOffset = 0x40000;
            }
            break;
        case 2:
            player->stateFlags &= 0xFFF7FFFF;
            if (gViewportStates[(u16)player->playerIndex].overlayAlpha == 0) {
                player->mode = 0;
                player->updateState = 0;
                player->updateTimer = 0;
                player->stateFlags = 0;
            }
            stepRaceMotionAnimationUntilEnd(player);
            break;
    }

    player->velocity.y -= player->gravity;
    player->pos.y += player->velocity.y;
}

void updateRacePlayerMode28TerrainFallWithItemEffect(RacePlayer *player) {
    s16 updateState;

    player->velocity.y -= player->gravity;
    player->pos.y += player->velocity.y;
    updateState = player->updateState;

    switch (updateState) {
        case 0:
            player->updateState = updateState + 1;
            player->stateFlags &= 0xFE0C1FFB;
            player->stateFlags |= 0x42000;
            setRaceMotionAnimation(player, 0xE);
            stepRaceMotionAnimationUntilEnd(player);
            player->unk80 = player->coursePathIndex + 2;
            setRaceCameraMode(player->playerIndex, 4);
            player->targetGroundOffset = 0;
            if (player->soundDisabled == 0) {
                spawnRaceItemBreakParticles(player->playerIndex, player->surfaceType);
            }
            player->unk2E8 = player->coursePathIndex;
            player->rumbleTimer = 2;
            player->rumblePatternId = 0;
            /* fallthrough */
        case 1:
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                player->updateState++;
                setRaceMotionAnimation(player, 0xF);
            }
            updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
            player->speedLimit = 0x20000;
            updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x2000, 0x2000, 0x2000);
            player->pos.x += player->velocity.x;
            player->pos.y += player->velocity.y;
            player->pos.z += player->velocity.z;
            break;
        case 2:
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                player->updateState++;
                /* Rank-scaled recovery timer for humans and CPUs. With zero-based rank,
                 * integer truncation gives 60/53/45/38 updates for a base of 60. */
                player->modeValue = 0x3C - ((player->rankIndex * 0x3C) / 8);
                setRaceMotionAnimation(player, 0x20);
            }
            updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
            player->speedLimit = 0x20000;
            updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x2000, 0x2000, 0x2000);
            player->pos.x += player->velocity.x;
            player->pos.y += player->velocity.y;
            player->pos.z += player->velocity.z;
            break;
        case 3:
            player->modeValue--;
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                setRaceMotionAnimation(player, 0x21);
            }
            if (player->modeValue == 0) {
                player->updateState++;
            }
            break;
        case 4:
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                setRaceMotionAnimation(player, 0x21);
            }
            player->stateFlags |= 0x80000;
            if (gViewportStates[(u16)player->playerIndex].overlayAlpha == 0xFF) {
                player->updateState++;
                player->coursePathIndex = player->unk2E8;
            loop:
                getRaceCourseSurfaceSpawnTransform(
                    player->coursePathIndex,
                    &player->pos.x,
                    &player->pos.y,
                    &player->pos.z,
                    &player->facingAngle
                );
                if (isRacePlayerRespawnSurfaceValid(player) == 0) {
                    player->coursePathIndex--;
                    goto loop;
                }
                player->pos.y = getRaceCourseSurfaceHeight(player->coursePathIndex, player->pos.x, player->pos.z);
                player->previousPosition = player->pos;
                player->velocity.x = 0;
                player->velocity.y = 0;
                player->velocity.z = 0;
                player->stateFlags &= ~0x400;
                player->leanAngle = 0;
                updateRacePlayerLeanAngle(player, 0, 0);
                setRaceMotionAnimation(player, 1);
                stepRaceMotionAnimationUntilEnd(player);
                setRaceCameraMode(player->playerIndex, 1);
                player->targetGroundOffset = 0x40000;
            }
            break;
        case 5:
            player->stateFlags &= 0xFFF7FFFF;
            if (gViewportStates[(u16)player->playerIndex].overlayAlpha == 0) {
                player->mode = 0;
                player->updateState = 0;
                player->updateTimer = 0;
                stepRaceMotionAnimationUntilEnd(player);
                player->stateFlags = 0;
            }
            break;
    }
}

void updateRacePlayerMode09TerrainCrash(RacePlayer *player) {
    s16 sine;
    s16 cosine;
    s16 updateState;

    player->velocity.y -= player->gravity;
    player->pos.y += player->velocity.y;
    updateState = player->updateState;

    switch (updateState) {
        case 0:
            player->updateState = updateState + 1;
            player->stateFlags &= 0xFE0C1FFB;
            player->stateFlags |= 0x42000;
            setRaceMotionAnimation(player, 0xE);
            stepRaceMotionAnimationUntilEnd(player);
            player->unk80 = player->coursePathIndex + 2;
            setRaceCameraMode((u16)player->playerIndex, 4);
            player->targetGroundOffset = 0;
            if (player->soundDisabled == 0) {
                spawnRaceItemBreakParticles(player->playerIndex, player->surfaceType);
            }
            player->unk2E8 = player->coursePathIndex;
            player->rumbleTimer = 2;
            player->rumblePatternId = 0;
            /* fallthrough */
        case 1:
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                player->updateState++;
                setRaceMotionAnimation(player, 0xF);
            }
            updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
            player->speedLimit = 0x20000;
            updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x2000, 0x2000, 0x2000);
            player->pos.x += player->velocity.x;
            player->pos.y += player->velocity.y;
            player->pos.z += player->velocity.z;
            break;
        case 2:
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                player->updateState++;
                /* Rank-scaled recovery timer for humans and CPUs. With zero-based rank,
                 * integer truncation gives 60/53/45/38 updates for a base of 60. */
                player->modeValue = 0x3C - ((player->rankIndex * 0x3C) / 8);
                setRaceMotionAnimation(player, 0x20);
            }
            updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
            player->speedLimit = 0x20000;
            updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x2000, 0x2000, 0x2000);
            player->pos.x += player->velocity.x;
            player->pos.y += player->velocity.y;
            player->pos.z += player->velocity.z;
            break;
        case 3:
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                setRaceMotionAnimation(player, 0x21);
            }
            player->modeValue--;
            if (player->modeValue == 0) {
                player->updateState++;
            }
            break;
        case 4:
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                setRaceMotionAnimation(player, 0x21);
            }
            player->stateFlags |= 0x80000;
            if (gViewportStates[(u16)player->playerIndex].overlayAlpha == 0xFF) {
                player->updateState++;
                player->coursePathIndex = player->unk2E8;
                getRaceCourseSurfaceSpawnTransform(
                    player->unk80,
                    &player->pos.x,
                    &player->pos.y,
                    &player->pos.z,
                    &player->facingAngle
                );
                player->pos.y = getRaceCourseSurfaceHeight(player->coursePathIndex, player->pos.x, player->pos.z);
                sine = fixedSine(player->facingAngle);
                cosine = fixedCosine(player->facingAngle);
                player->pos.x += ((s64)sine * -0x240000) / 0x1000;
                player->pos.z += ((s64)cosine * -0x240000) / 0x1000;
                player->previousPosition = player->pos;
                player->velocity.x = 0;
                player->velocity.y = 0;
                player->velocity.z = 0;
                player->verticalVelocityLimit = 0;
                player->stateFlags &= ~0x400;
                player->leanAngle = 0;
                updateRacePlayerLeanAngle(player, 0, 0);
                setRaceMotionAnimation(player, 1);
                stepRaceMotionAnimationUntilEnd(player);
                setRaceCameraMode((u16)player->playerIndex, 1);
                player->targetGroundOffset = 0x40000;
            }
            break;
        case 5:
            player->stateFlags &= 0xFFF7FFFF;
            if (gViewportStates[(u16)player->playerIndex].overlayAlpha == 0) {
                player->mode = 0;
                player->updateState = 0;
                player->updateTimer = 0;
                stepRaceMotionAnimationUntilEnd(player);
                player->stateFlags = 0;
            }
            break;
    }
}

void updateRacePlayerMode10TerrainCrashSlide(RacePlayer *player) {
    s16 updateState;
    s32 yVel;
    s32 velocityX;
    s32 velocityZ;
    u32 stateFlags;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateFlags &= ~0x200;
        player->stateFlags &= 0xFE0C1FFB;
        (*player).stateFlags |= 0x01006000;
        if (player->animationId != 0x12) {
            setRaceMotionAnimation(player, 0x12);
        }
        player->modeValue = 0x1E;
        player->targetGroundOffset = 0;
    }
    stepRaceMotionLoopingAnimation(player);
    updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
    player->velocity.y += 0xFFFF6000;
    player->speedLimit = 0x80000;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    stateFlags = player->stateFlags;
    if ((stateFlags & 1) == 0) {
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0, 0, 0);
        player->loopSoundVolume = 0x100;
        player->loopSoundId = SFX_CRASH_SLIDE_LOOP;
        player->loopSoundPitch = 0.0f;
        stateFlags = player->stateFlags;
    }
    if ((stateFlags & 1) != 0) {
        player->stateFlags = stateFlags | 0x200;
    } else {
        player->stateFlags = stateFlags & ~0x200;
    }
    yVel = player->velocity.y;
    velocityX = player->velocity.x;
    velocityZ = player->velocity.z;
    player->pos.y += yVel;
    player->verticalVelocityLimit = 0;
    player->pos.x += velocityX;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->facingAngle = calculateFixedAngleFromDeltaXZ(velocityX, velocityZ);
    if (player->stateFlags & 0x400) {
        player->facingAngle += 0x800;
    }
    if (player->surfaceType != 3) {
        if (--player->modeValue == 0) {
            player->stateFlags &= 0xFE0C1FFB;
            if (player->stateFlags & 1) {
                player->mode = 1;
                player->updateState = 0;
                player->updateTimer = 0;
            } else {
                player->mode = 0;
                player->updateState = 0;
                player->updateTimer = 0;
            }
        }
    } else {
        player->modeValue = 0x1E;
    }
    player->rumbleTimer = 4;
    player->rumblePatternId = 2;
}

void updateRacePlayerMode12LaunchRecover(RacePlayer *player) {
    s16 updateState;
    u32 stateFlags;
    s32 grounded;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x14);
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags &= ~0x200;
        player->stateFlags |= 0x1022000;
        player->targetGroundOffset = 0;
        player->unk80 = 0;
        /* Rank-scaled recovery timer for humans and CPUs. With zero-based rank,
         * integer truncation gives 60/53/45/38 updates for a base of 60. */
        player->modeValue = 0x1E - ((player->rankIndex * 0x1E) / 8);
        player->rumbleTimer = 1;
        player->rumblePatternId = 0;
    }

    updateRacePlayerLeanAngle(player, 0, 0);
    player->velocity.y += 0xFFFF6000;
    grounded = player->stateFlags & 1;
    if (grounded == 0) {
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x6000, 0x6000, 0x6000);
        grounded = player->stateFlags & 1;
    }

    player->pos.x += player->velocity.x;
    player->pos.y += player->velocity.y;
    player->pos.z += player->velocity.z;

    if (grounded == 0) {
        updateRacePlayerMotionFeedback(player);
        if ((player->loopSoundId != SFX_BOARD_SURFACE_RUMBLE_LOOP) && (player->loopSoundId != SFX_BREAKABLE_SURFACE_LOOP)) {
            player->loopSoundPitch = -6.0f;
        }
    }

    stateFlags = player->stateFlags;
    if (!(stateFlags & 1)) {
        player->modeValue = player->modeValue - getRaceInputTimerDecrementBonus(player) - 1;
        if (player->modeValue < 0) {
            player->modeValue = 0;
        }
        if (stepRaceMotionAnimationUntilEnd(player)) {
            player->stateFlags &= ~0x200;
            if (player->modeValue == 0) {
                player->mode = 8;
                player->updateState = 0;
                player->updateTimer = 0;
            }
        }
    } else {
        player->stateFlags = stateFlags | 0x200;
        setRaceMotionAnimation(player, 0x14);
        stepRaceMotionAnimationUntilEnd(player);
        player->modeValue = 0x1E;
        player->unk80 = 7;
        if (!(player->stateFlags & 1)) {
            player->rumbleTimer = 1;
            player->rumblePatternId = 0;
        }
    }
}

void updateRacePlayerMode11LaunchHit(RacePlayer *player) {
    s16 updateState;
    s32 tempX;
    s32 tempZ;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x13);
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x22200;
        player->pitchAngle = 0;
        updateRacePlayerLeanAngle(player, 0, 0);
        player->targetGroundOffset = 0;
        player->rumbleTimer = 3;
        player->rumblePatternId = 0;
    }

    player->speedLimit = 0x60000;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    player->velocity.y -= player->aerialGravity;
    if (!(player->stateFlags & 1)) {
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x4000, 0x4000, 0x4000);
    } else {
        tempX = player->velocity.x;
        tempZ = player->velocity.z;
        player->velocity.x = tempX - (tempX / 16);
        player->velocity.z = tempZ - (tempZ / 16);
    }

    player->pos.x += player->velocity.x;
    player->pos.y += player->velocity.y;
    player->pos.z += player->velocity.z;

    if ((stepRaceMotionAnimationUntilEnd(player) != 0) && !(player->stateFlags & 1)) {
        player->mode = 0xC;
        player->updateState = 0;
        player->updateTimer = 0;
    }
}

void updateRacePlayerMode26Tumble(RacePlayer *player) {
    s16 updateState;
    s32 timer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x12);
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x6204;
        player->pitchAngle = 0;
        updateRacePlayerLeanAngle(player, 0, 0);
        player->targetGroundOffset = 0;
        player->velocity.y = 0x80000;
        player->modeValue = 0x14;
    }

    if (gFrameCounter & 1) {
        createCallbackTaskWithUserIdPreservingArgs(initRacePlayerRecoverySparkle, 5, 2, (u16)player->playerIndex);
    }

    player->velocity.y -= 0x7000;
    player->pos.y += player->velocity.y;
    stepRaceMotionLoopingAnimation(player);

    timer = player->modeValue;
    if (timer == 0) {
        if (!(player->stateFlags & 1)) {
            player->mode = 0xC;
            player->updateState = 0;
            player->updateTimer = 0;
        }
    } else {
        player->modeValue = timer - 1;
    }

    if (player->velocity.y > 0) {
        player->rumbleTimer = 2;
        player->rumblePatternId = 3;
    }
}

void updateRacePlayerMode14PushHit(RacePlayer *player) {
    s16 updateState;
    s32 velocityX;
    s32 velocityZ;
    u32 stateFlags;
    s16 nextState;

    updateState = player->updateState;
    nextState = updateState + 1;
    if (updateState == 0) {
        player->updateState = nextState;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x01006000;
        setRaceMotionAnimation(player, 0x12);
        player->modeValue = 0xF;
        player->targetGroundOffset = 0;
    }
    stepRaceMotionLoopingAnimation(player);
    updateRacePlayerLeanAngle(player, 0x10000, 0);
    player->velocity.y += 0xFFFF6000;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    stateFlags = player->stateFlags;
    if ((stateFlags & 1) == 0) {
        player->stateFlags = stateFlags & ~0x200;
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0, 0, 0);
        player->loopSoundVolume = 0x100;
        player->loopSoundId = SFX_CRASH_SLIDE_LOOP;
        player->loopSoundPitch = 0.0f;
    } else {
        player->stateFlags = stateFlags | 0x200;
    }
    velocityX = player->velocity.x;
    player->pos.x += velocityX;
    player->pos.y += player->velocity.y;
    velocityZ = player->velocity.z;
    player->pos.z += velocityZ;
    player->facingAngle = calculateFixedAngleFromDeltaXZ(velocityX, velocityZ);
    if (player->stateFlags & 0x400) {
        player->facingAngle += 0x800;
    }
    if (--player->modeValue == 0) {
        player->mode = 0xC;
        player->updateState = 0;
        player->updateTimer = 0;
    }
    player->rumbleTimer = 4;
    player->rumblePatternId = 2;
}

void updateRacePlayerMode23ItemSteal(RacePlayer *player) {
    s16 updateState;
    s16 nextState;
    s32 timer;
    u32 stateFlags;

    updateState = player->updateState;
    nextState = updateState + 1;
    if (updateState == 0) {
        player->updateState = nextState;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x102000;
        setRaceMotionAnimation(player, 0xE);
        player->modeValue = 0x5A;
        player->targetGroundOffset = 0;
        stepRaceMotionLoopingAnimation(player);
        player->velocity.y = 0;
        createCallbackTaskWithUserIdPreservingArgs(initRaceUiItemStealTrailEffect, 0, 0x64, (u16)player->playerIndex);
        timer = player->modeValue;
        /* Rank-scaled recovery timer for humans and CPUs. With zero-based rank,
         * integer truncation gives 60/53/45/38 updates for a base of 60. */
        player->modeValue = timer - ((player->modeValue * player->rankIndex) / 8);
        player->rumbleTimer = 4;
        player->rumblePatternId = 0;
    }

    stateFlags = player->stateFlags;
    if (!(stateFlags & 1)) {
        player->stateFlags = stateFlags & ~0x200;
    } else {
        player->stateFlags = stateFlags | 0x200;
    }

    updateRacePlayerLeanAngle(player, 0, 0);
    player->velocity.y -= 0xA000;
    player->pos.y += player->velocity.y;

    player->modeValue -= getRaceInputTimerDecrementBonus(player) + 1;
    timer = player->modeValue;
    if (timer < 0) {
        player->modeValue = 0;
        timer = 0;
    }

    if (timer == 0) {
        player->mode = 4;
        player->updateState = 0;
        player->updateTimer = 0;
        player->unk2E0 = 0;
        player->unk2E4 = 0;
    }
}

void updateRacePlayerMode24HeavyKnockdown(RacePlayer *player) {
    s16 temp_v1_2;
    s16 temp_v0;
    s16 temp_2d6;
    s32 timer;
    s32 ret;
    u32 stateFlags;

    player->leanAngle = 0;
    updateRacePlayerLeanAngle(player, 0, 0);
    player->velocity.y += 0xFFFF6000;
    player->pos.y += player->velocity.y;

    stateFlags = player->stateFlags;
    if (!(stateFlags & 1)) {
        player->stateFlags = stateFlags & ~0x200;
    } else {
        player->stateFlags = stateFlags | 0x200;
    }

    temp_v1_2 = player->updateState;
    temp_v0 = temp_v1_2;
    switch (temp_v0 ^ 0) {
        case 0:
            player->updateState = temp_v1_2 + 1;
            player->stateFlags &= 0xFE0C1FFB;
            player->stateFlags |= 0x1202000;
            player->unk2D6 = 1;
            setRaceMotionAnimation(player, 1);
            player->modeValue = 0x5A;
            player->targetGroundOffset = 0;
            stepRaceMotionLoopingAnimation(player);
            timer = player->modeValue;
            /* Rank-scaled recovery timer for humans and CPUs. With zero-based rank,
             * integer truncation gives 60/53/45/38 updates for a base of 60. */
            player->modeValue = (timer ^ 0) - ((timer * player->rankIndex) / 8);
            player->velocity.y = 0;
            player->rumbleTimer = 6;
            player->rumblePatternId = 0;
            /* fallthrough */
        case 1:
            temp_2d6 = player->unk2D6;
            if (temp_2d6 != 1) {
                player->unk2D6 = temp_2d6 - 1;
            }

            ret = getRaceInputTimerDecrementBonus(player);
            if (ret != 0) {
                player->unk2D6 = 3;
            }

            player->modeValue -= ret + 1;
            timer = player->modeValue;
            if (timer < 0) {
                player->modeValue = 0;
                timer = 0;
            }

            if (timer == 0) {
                player->updateState += 1;
                player->unk2D6 = 1;
            }
            break;
        case 2:
            player->unk2D6 += 1;
            if (player->unk2D6 == 7) {
                player->unk2D6 = 2;
                player->updateState += 1;
            }
            break;
        case 3:
            player->unk2D6 += 1;
            if (player->unk2D6 == 8) {
                player->stateFlags &= 0xFE0C1FFB;
                player->mode = 0;
                player->updateState = 0;
                player->updateTimer = 0;
                player->unk2D4 = 0x3C;
            }
            break;
    }
}

void updateRacePlayerMode25SpinHit(RacePlayer *player) {
    s16 updateState;
    s32 stateTimer;
    u32 stateFlags;
    s16 nextState;

    updateState = player->updateState;
    nextState = updateState + 1;
    if (updateState == 0) {
        player->updateState = nextState;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x400000;
        player->modeValue = 0xF0;
        setRaceMotionAnimation(player, 1);
        stepRaceMotionLoopingAnimation(player);
        createCallbackTaskWithUserIdPreservingArgs(
            initRaceUiHeavyKnockdownTrailEffect,
            0,
            0x3C,
            (u16)player->playerIndex
        );
        stateTimer = player->modeValue;
        player->modeValue = stateTimer;
        /* Rank-scaled recovery timer for humans and CPUs. With zero-based rank,
         * integer truncation gives 60/53/45/38 updates for a base of 60. */
        player->modeValue = player->modeValue - ((stateTimer * player->rankIndex) / 8);
        player->rumbleTimer = 4;
        player->rumblePatternId = 0;
    }

    stateFlags = player->stateFlags;
    if (!(stateFlags & 1)) {
        player->stateFlags = stateFlags & ~0x200;
        updateRacePlayerLeanAngle(player, player->localVelocityZ, 0);
        player->velocity.y -= player->gravity;
        updateRacePlayerLocalVelocity(player, 0, 0, player->lateralDeceleration, player->forwardDeceleration, player->reverseDeceleration);
    } else {
        player->stateFlags = stateFlags | 0x200;
        player->velocity.y -= player->aerialGravity;
        clampRacePlayerVectorXZSpeed(&player->velocity, player);
    }

    player->modeValue -= getRaceInputTimerDecrementBonus(player) + 1;
    if (player->modeValue < 0) {
        player->modeValue = 0;
    }

    if (player->modeValue == 0) {
        stateFlags = player->stateFlags;
        if (!(stateFlags & 0x200)) {
            player->stateFlags = stateFlags & 0xFE0C1FFB;
            player->mode = 0;
            player->updateState = 0;
            player->updateTimer = 0;
        } else {
            player->stateFlags = stateFlags & 0xFE0C1FFB;
            player->mode = 1;
            player->updateState = 0;
            player->updateTimer = 0;
        }
    }

    player->pos.x += player->velocity.x;
    player->pos.y += player->velocity.y;
    player->pos.z += player->velocity.z;

    if (!(player->stateFlags & 1)) {
        updateRacePlayerMotionFeedback(player);
    }
}

void updateRacePlayerMode27Slide(RacePlayer *player) {
    s16 updateState;
    s16 updateTimer;
    u32 stateFlags;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x13);
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x822204;
        player->pitchAngle = 0;
        player->targetGroundOffset = 0;
        player->velocity.y = 0x80000;
        player->updateTimer = 0;
        player->rumbleTimer = 4;
        player->rumblePatternId = 0;
    }

    updateRacePlayerLeanAngle(player, 0, 0);
    if (player->velocity.y > 0) {
        player->velocity.y -= 0x7000;
    } else {
        updateTimer = player->updateTimer;
        if (updateTimer == 0) {
            player->updateTimer = updateTimer + 1;
            createCallbackTaskWithUserIdPreservingArgs(
                initRaceUiSpinHitTransitionEffect,
                0,
                3,
                (u16)player->playerIndex
            );
        }
        player->facingAngle += 0x40;
        player->velocity.y -= 0x800;
    }

    player->velocity.x -= player->velocity.x >> 6;
    player->velocity.z -= player->velocity.z >> 6;
    player->pos.x += player->velocity.x;
    player->pos.y += player->velocity.y;
    player->pos.z += player->velocity.z;

    stepRaceMotionAnimationUntilEnd(player);
    if (player->velocity.y < 0) {
        stateFlags = player->stateFlags;
        if (!(stateFlags & 1)) {
            player->mode = 8;
            player->updateState = 0;
            player->updateTimer = 0;
            player->stateFlags = stateFlags & ~0x200;
        }
    }
}

void updateRacePlayerMode40Stun(RacePlayer *player) {
    s16 updateState;
    s32 velocityX;
    s32 velocityZ;
    u32 stateFlags;
    s16 nextState;

    updateState = player->updateState;
    nextState = updateState + 1;
    if (updateState == 0) {
        player->updateState = nextState;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x01006000;
        setRaceMotionAnimation(player, 0x12);
        player->modeValue = 0x1E;
        player->modeValue += randomNextMain() >> 4;
        player->targetGroundOffset = 0;
    }
    stepRaceMotionLoopingAnimation(player);
    updateRacePlayerLeanAngle(player, 0x10000, 0);
    player->velocity.y += 0xFFFF6000;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    stateFlags = player->stateFlags;
    if ((stateFlags & 1) == 0) {
        player->stateFlags = stateFlags & ~0x200;
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0, 0, 0);
        player->stateFlags &= ~0x200;
        player->loopSoundVolume = 0x100;
        player->loopSoundId = SFX_CRASH_SLIDE_LOOP;
        player->loopSoundPitch = 0.0f;
    } else {
        player->stateFlags = stateFlags | 0x200;
    }
    velocityX = player->velocity.x;
    player->pos.x += velocityX;
    player->pos.y += player->velocity.y;
    velocityZ = player->velocity.z;
    player->pos.z += velocityZ;
    player->facingAngle = calculateFixedAngleFromDeltaXZ(velocityX, velocityZ);
    if (player->stateFlags & 0x400) {
        player->facingAngle += 0x800;
    }
    if (--player->modeValue == 0) {
        player->mode = 0xC;
        player->updateState = 0;
        player->updateTimer = 0;
    }
    player->rumbleTimer = 4;
    player->rumblePatternId = 2;
}

void func_80094C4C(RacePlayer *player) {
    gRacePlayerMode07StateHandlers[player->updateState](player);
}

void updateRacePlayerMode07AlignToLaunchRamp(RacePlayer *player) {
    s16 angleDelta;
    s16 updateTimer;
    s32 temp_a1;
    s32 tempX;
    s32 tempZ;

    updateTimer = player->updateTimer;
    if (updateTimer == 0) {
        player->updateTimer = updateTimer + 1;
        player->modeValue = 0x14;
        player->unk80 = 0xA;
        player->modeAngle = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampCourseObjectAngle;
        if (player->stateFlags & 0x400) {
            player->modeAngle += 0x800;
        }
        gMenuFlowState |= 4;
        setRaceMotionAnimation(player, 1);
    }

    stepRaceMotionLoopingAnimation(player);
    angleDelta = (player->modeAngle - player->facingAngle) & 0xFFF;
    if (angleDelta >= 0x801) {
        angleDelta -= 0x1000;
    }
    updateRacePlayerLeanAngle(player, 0x10000, 0);

    temp_a1 = player->unk80;
    if (1) {}
    if (1) {}
    if (1) {}
    if (temp_a1 != 0) {
        angleDelta /= temp_a1;
        if (gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampTargetZ) {}
        player->facingAngle += angleDelta;
        tempX = player->pos.x;
        tempZ = player->pos.z;
        if (temp_a1) {}
        if (1) {
            player->pos.x =
                tempX -
                (0 - ((gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampTargetX - tempX) / temp_a1));
        }
        player->pos.z =
            tempZ - (0 - ((gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampTargetZ - tempZ) / temp_a1));
    }

    if (--player->modeValue == 0) {
        player->pos.x = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampTargetX;
        player->pos.z = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampTargetZ;
        player->updateState++;
        player->updateTimer = 0;
        player->modeValue = 0;
        setRaceMotionAnimation(player, 5);
        setRaceCameraMode(player->playerIndex, 3);
    }
}

void updateRacePlayerMode07StartLaunchRamp(RacePlayer *player) {
    if (stepRaceMotionAnimationUntilEnd(player) != 0) {
        player->updateState++;
        player->updateTimer = 0;
        player->facingAngle = player->modeAngle;
        player->pos.x = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampTargetX;
        player->pos.z = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampTargetZ;
    }
}

void updateRacePlayerMode07LaunchRampTakeoff(RacePlayer *player) {
    s16 updateTimer;
    s32 unused;
    s32 scratch[14];

    updateTimer = player->updateTimer;
    if (updateTimer == 0) {
        player->updateTimer = updateTimer + 1;
        player->velocity.y = 0x30000;
        player->modeValue = 0x1E;
        setRaceMotionAnimation(player, 4);
        if (player->stateFlags & 0x400) {
            player->unk80 = 0x20000;
        } else {
            player->unk80 = -0x20000;
        }
    }

    makeFixedRotationY((s16 *)&scratch[2], player->facingAngle);
    scratch[13] = 0;
    scratch[14] = 0;
    scratch[15] = player->unk80;
    transformVec3iByFixedMatrix((s16 *)&scratch[2], (Vec3i *)&scratch[13], (Vec3i *)&scratch[10]);
    player->pos.x += scratch[10];
    player->pos.z += scratch[12];

    makeFixedRotationY((s16 *)&scratch[2], player->facingAngle);
    scratch[13] = 0;
    scratch[14] = 0;
    scratch[15] = player->unk80;
    transformVec3iByFixedMatrix((s16 *)&scratch[2], (Vec3i *)&scratch[13], (Vec3i *)&scratch[10]);
    player->pos.x += scratch[10];
    player->pos.z += scratch[12];
    player->velocity.y -= 0x7000;
    player->pos.y += player->velocity.y;

    if ((player->velocity.y < 0) && !(player->stateFlags & 1)) {
        if (player->animationId != 1) {
            setRaceMotionAnimation(player, 1);
        }
        stepRaceMotionLoopingAnimation(player);
        updateRacePlayerMotionFeedback(player);
    } else {
        stepRaceMotionAnimationUntilEnd(player);
    }

    if (--player->modeValue == 0) {
        player->updateState++;
        player->updateTimer = 0;
        player->modeValue = 0x20;
        createCallbackTask((CallbackTaskCallback)initLaunchRampCourseObject, 0, 0x64);
        player->stateFlags &= ~0x20;
    }
}

void updateRacePlayerMode07LaunchRampSpin(RacePlayer *player) {
    s32 scratch[15];

    player->facingAngle += 0x10;
    if (player->unk80 > 0) {
        player->unk80 -= 0x820;
    } else {
        player->unk80 += 0x820;
    }

    makeFixedRotationY((s16 *)&scratch[1], player->facingAngle);
    scratch[12] = 0;
    scratch[13] = 0;
    scratch[14] = player->unk80;
    transformVec3iByFixedMatrix((s16 *)&scratch[1], (Vec3i *)&scratch[12], (Vec3i *)&scratch[9]);
    player->pos.x += scratch[9];
    player->pos.z += scratch[11];

    player->facingAngle += 0x10;
    if (player->unk80 > 0) {
        player->unk80 -= 0x820;
    } else {
        player->unk80 += 0x820;
    }

    makeFixedRotationY((s16 *)&scratch[1], player->facingAngle);
    scratch[12] = 0;
    scratch[13] = 0;
    scratch[14] = player->unk80;
    transformVec3iByFixedMatrix((s16 *)&scratch[1], (Vec3i *)&scratch[12], (Vec3i *)&scratch[9]);
    player->pos.x += scratch[9];
    player->pos.z += scratch[11];

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerMotionFeedback(player);
    if (--player->modeValue == 0) {
        player->updateTimer = 0;
        player->updateState++;
    }
}

void updateRacePlayerMode07LaunchRampPose(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        setRaceMotionAnimation(player, 0x1D);
        player->modeValue = 0x13;
    }
    stepRaceMotionAnimationUntilEnd(player);
    player->modeValue--;
    if (player->modeValue == 0) {
        player->updateTimer = 0;
        player->updateState++;
    }
}

void updateRacePlayerMode07LaunchRampDrift(RacePlayer *player) {
    s16 updateTimer;
    s32 scratch[14];

    updateTimer = player->updateTimer;
    if (updateTimer == 0) {
        player->updateTimer = updateTimer + 1;
        player->modeValue = 0x13;
        if (player->stateFlags & 0x400) {
            player->unk80 = 0x40000;
        } else {
            player->unk80 = -0x40000;
        }
    }

    makeFixedRotationY((s16 *)&scratch[1], player->facingAngle);
    scratch[12] = 0;
    scratch[13] = 0;
    scratch[14] = player->unk80;
    transformVec3iByFixedMatrix((s16 *)&scratch[1], (Vec3i *)&scratch[12], (Vec3i *)&scratch[9]);
    player->pos.x += scratch[9];
    player->pos.z += scratch[11];

    player->modeValue--;
    if (player->modeValue == 0) {
        player->updateTimer = 0;
        player->updateState++;
    }
    stepRaceMotionAnimationUntilEnd(player);
}

void updateRacePlayerMode07LaunchRampClimb(RacePlayer *player) {
    PlayerTransformScratch80095A88 scratch;

    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->modeValue = 0x64;
    }

    if (player->unk80 > 0) {
        if (player->unk80 < 0xA0000) {
            player->unk80 += 0x2000;
        }
        makeFixedRotationXY((s16 *)scratch.matrix, -0x100, player->facingAngle);
    } else {
        if (player->unk80 >= -0x9FFFF) {
            player->unk80 -= 0x2000;
        }
        makeFixedRotationXY((s16 *)scratch.matrix, 0x100, (0, player)->facingAngle);
    }

    scratch.sourceX = 0;
    scratch.sourceY = 0;
    scratch.sourceZ = player->unk80;
    transformVec3iByFixedMatrix((s16 *)scratch.matrix, (Vec3i *)&scratch.sourceX, (Vec3i *)&scratch.transformedX);
    player->pos.x = player->pos.x + scratch.transformedX;
    player->pos.y += scratch.transformedY;
    player->pos.z += scratch.transformedZ;

    player->modeValue--;
    if (player->modeValue == 0) {
        player->updateTimer = 0;
        player->updateState++;
    }

    if (player->modeValue < 0x3C) {
        player->stateFlags |= 0x80000;
        if (gRaceCameras[(u16)player->playerIndex].initialized.signedValue != 0) {
            gRaceCameras[(u16)player->playerIndex].initialized.signedValue = 2;
        }
    }
}

void updateRacePlayerMode07SpiralExit(RacePlayer *player) {
    PlayerTransformScratch80095A88 scratch;

    if (player->updateTimer == 0) {
        player->updateTimer++;
        scratch.sourceX = -0x200000;
        scratch.sourceY = 0;
        scratch.sourceZ = 0x400000;
        makeFixedRotationY(
            (s16 *)scratch.matrix,
            gRaceCourseStartEntries[gRaceCourseIndex.signedValue].liftExitObjectAngle
        );
        transformVec3iByFixedMatrix((s16 *)scratch.matrix, (Vec3i *)&scratch.sourceX, (Vec3i *)&player->pos);
        player->pos.x += gRaceCourseStartEntries[gRaceCourseIndex.signedValue].liftExitOrigin.x;
        player->pos.y += gRaceCourseStartEntries[gRaceCourseIndex.signedValue].liftExitOrigin.y + 0x80000;
        player->pos.z += gRaceCourseStartEntries[gRaceCourseIndex.signedValue].liftExitOrigin.z;
        player->coursePathIndex = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].liftExitSurfaceIndex;
        player->lapDigit++;
        player->previousPosition = player->pos;
        player->modeValue = 0x28;
        player->stateFlags &= 0xFBFFFBFF;
        player->facingAngle = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].liftExitObjectAngle;
        player->coursePathOffset =
            -projectRaceCourseSurfaceProgress(player->coursePathIndex, player->pos.x, player->pos.z);
        setRaceCameraMode(player->playerIndex, 1);
        gRaceCameras[player->playerIndex].prevPos.x = player->pos.x;
        gRaceCameras[player->playerIndex].prevPos.y = player->pos.y;
        gRaceCameras[player->playerIndex].prevPos.z = player->pos.z;
        createCallbackTask((CallbackTaskCallback)initLiftExitCourseObject, 0, 0x64);
        if (player->lapDigit >= (gRaceLapCount - 1)) {
            createCallbackTaskWithUserIdPreservingArgs(waitForRaceSetupNamePlate, 0, 0x64, player->playerIndex);
        }
    }

    player->stateFlags &= ~0x80000;
    scratch.sourceX = 0;
    scratch.sourceY = 0;
    scratch.sourceZ = -0x40000;
    makeFixedRotationY(
        (s16 *)scratch.matrix,
        gRaceCourseStartEntries[gRaceCourseIndex.signedValue].liftExitObjectAngle
    );
    transformVec3iByFixedMatrix((s16 *)scratch.matrix, (Vec3i *)&scratch.sourceX, (Vec3i *)&scratch.transformedX);
    player->pos.x += scratch.transformedX;
    player->pos.z += scratch.transformedZ;
    player->modeValue--;

    if (player->modeValue == 0) {
        player->mode = 1;
        player->updateState = 0;
        player->updateTimer = 0;
        player->stateFlags = 0;

        /* Rank-dependent course-object exit displacement: add 0/1/2/4 times
         * current X/Z velocity for first through fourth, for humans and CPUs. */
        if (player->rankIndex == 1) {
            player->pos.x += player->velocity.x;
            player->pos.z += player->velocity.z;
        }

        if (player->rankIndex == 2) {
            player->pos.x += player->velocity.x * 2;
            player->pos.z += player->velocity.z * 2;
        }

        if (player->rankIndex == 3) {
            player->pos.x += player->velocity.x * 4;
            player->pos.z += player->velocity.z * 4;
        }

        if (gRaceCameras[player->playerIndex].initialized.signedValue != 0) {
            gRaceCameras[player->playerIndex].initialized.signedValue = 1;
        }
    }
}

s32 tryStartRacePlayerCourseObjectMode(s16 arg0) {
    s32 delta;
    s32 i;

    if (gRacePlayers[arg0].stateFlags & 0x40) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        if ((gRacePlayers[i].isActive != 0) && (arg0 != (u16)gRacePlayers[i].playerIndex) &&
            (gRacePlayers[i].stateFlags & 0x20)) {
            return 0;
        }
    }

    delta = gRacePlayers[arg0].pos.x - gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampTargetX;
    if (delta < 0) {
        delta = -delta;
    }
    if (delta > 0x180000) {
        return 0;
    }

    delta = gRacePlayers[arg0].pos.z - gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampTargetZ;
    if (delta < 0) {
        delta = -delta;
    }
    if (delta > 0x180000) {
        return 0;
    }

    gRacePlayers[arg0].stateFlags |= 0x04001020;
    gRacePlayers[arg0].unk57C = gRacePlayerAttackStartTimer;
    if (gRacePlayerAttackStartTimer != 0) {
        gRacePlayerAttackStartTimer--;
    }
    return 1;
}

void dispatchRacePlayerMode30Attack(RacePlayer *player) {
    gRacePlayerMode30StateHandlers[player->updateState](player);
    if (player->collisionRadius < 0xF0000) {
        player->collisionRadius += 0x8000;
    }
}

void updateRacePlayerMode30AttackApproach(RacePlayer *player) {
    s16 updateTimer;
    s16 facingAngle;
    s16 angleDiff;
    s16 roll;

    updateTimer = player->updateTimer;
    if (updateTimer == 0) {
        player->updateTimer = updateTimer + 1;
        player->subState = 0;
        setRaceCameraMode(player->playerIndex, 5);
        if (player->movementSpeed >= 0x20001) {
            player->unk306 = 1;
            player->unk31E = calculateFixedAngleFromDeltaXZ(player->velocity.x, player->velocity.z);
            if (player->stateFlags & 0x400) {
                player->unk31E += 0x800;
            }

            if ((s16)(angleDiff = 0x800 & (player->unk31E - player->facingAngle)) < 0x800) {
                player->unk31C = 0;
                if (player->stateFlags & 0x400) {
                    player->unk31C = 1;
                }
                player->unk31E -= 0x400;
            } else {
                player->unk31C = 1;
                if (player->stateFlags & 0x400) {
                    player->unk31C = 0;
                }
                player->unk31E += 0x400;
            }
            player->modeValue = 0;
        } else {
            player->unk306 = 0;
            setRaceMotionAnimation(player, 1);
        }
    }

    if (player->unk306 != 0) {
        facingAngle = player->facingAngle;
        angleDiff = (player->unk31E - facingAngle) & 0xFFF;
        if (angleDiff >= 0x801) {
            angleDiff -= 0x1000;
        }
        if (angleDiff >= 0x41) {
            angleDiff = 0x40;
        }
        if (angleDiff < -0x40) {
            angleDiff = -0x40;
        }
        player->facingAngle = facingAngle + angleDiff;

        if (player->unk31C != 0) {
            player->modeValue = 0x1F;
        } else {
            player->modeValue = -0x1F;
        }

        player->leanScale = 0x3F;
        angleDiff = updateRacePlayerLeanAngle(player, player->movementSpeed, player->modeValue) - player->modeAngle;
        roll = player->modeAngle;
        if (angleDiff >= 0x31) {
            angleDiff = 0x30;
        }
        if (angleDiff < -0x30) {
            angleDiff = -0x30;
        }
        player->modeAngle = player->modeAngle + angleDiff;

        if (player->modeAngle == 0) {
            if (player->animationId != 1) {
                setRaceMotionAnimation(player, 1);
            }
            stepRaceMotionLoopingJointAnimation(player);
        } else if (player->modeAngle >= 0) {
            setRaceMotionAnimation(player, 3);
            blendRaceMotionJointAnimation(player, 3, player->modeAngle, 0x118);
        } else {
            setRaceMotionAnimation(player, 2);
            blendRaceMotionJointAnimation(player, 2, -player->modeAngle, 0x118);
        }
    } else {
        updateRacePlayerLeanAngle(player, 0, 0);
        stepRaceMotionLoopingJointAnimation(player);
    }

    player->velocity.y -= player->gravity;
    updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x6000, 0x6000, 0x6000);
    player->pos.x += player->velocity.x;
    player->pos.y += player->velocity.y;
    player->pos.z += player->velocity.z;
    updateRacePlayerMotionFeedback(player);

    if ((player->velocity.x == 0) && (player->velocity.z == 0) && !(player->stateFlags & 1)) {
        if (player->soundDisabled == 0) {
            player->updateState++;
            player->modeValue = 0x1E;
            if (player->animationId != 1) {
                setRaceMotionAnimation(player, 1);
            }
        }
    }
}

void updateRacePlayerMode30AttackResolve(RacePlayer *player) {
    if (gRaceResultState != 0) {
        player->modeValue = 0;
    }
    if (player->modeValue == 0) {
        if (gRaceSplitscreenMode == 0) {
            if (player->rankIndex == 0) {
                player->updateState = 2;
                player->updateTimer = 0;
            } else {
                player->updateState = 3;
                player->updateTimer = 0;
            }
        } else if (gRaceResultState == 1) {
            player->updateState = 2;
            player->updateTimer = 0;
        } else {
            player->updateState = 3;
            player->updateTimer = 0;
        }
    }
    updateRacePlayerLeanAngle(player, 0, 0);
    stepRaceMotionLoopingJointAnimation(player);
}

void func_80095F2C(RacePlayer *player) {
    updateRacePlayerLeanAngle(player, 0, 0);
    gRacePlayerMode32CharacterHandlers[player->characterId](player);
}

void updateRacePlayerMode32Character0(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->modeValue = 0;
        setRaceMotionAnimation(player, 0x23);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->animationId == 0x24) {
            player->stateFlags |= 0x8000000;
        }
        if (player->modeValue == 0) {
            if (player->updateTimer == 1) {
                enqueueRacePlayerVoiceSound(player, RACE_VOICE_CHEER);
                player->updateTimer++;
            }
            player->modeValue = 0x14;
            setRaceMotionAnimation(player, 0x24);
        } else {
            player->modeValue--;
        }
    }
}

void updateRacePlayerMode32Character1(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->modeValue = 0;
        setRaceMotionAnimation(player, 0x24);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->animationId == 0x25) {
            player->stateFlags |= 0x8000000;
        }
        if (player->modeValue == 0) {
            if (player->updateTimer == 1) {
                enqueueRacePlayerVoiceSound(player, RACE_VOICE_CHEER);
                player->updateTimer++;
            }
            setRaceMotionAnimation(player, 0x25);
        } else {
            player->modeValue--;
        }
    }
}

void updateRacePlayerMode32Character2(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->modeValue = 0;
        setRaceMotionAnimation(player, 0x23);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->animationId == 0x24) {
            player->stateFlags |= 0x8000000;
        }
        if (player->modeValue == 0) {
            if (player->updateTimer == 1) {
                enqueueRacePlayerVoiceSound(player, RACE_VOICE_CHEER);
                player->updateTimer++;
            }
            player->modeValue = 0x14;
            setRaceMotionAnimation(player, 0x24);
        } else {
            player->modeValue--;
        }
    }
}

void updateRacePlayerMode32Character3(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->modeValue = 0;
        setRaceMotionAnimation(player, 0x24);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->animationId == 0x25) {
            player->stateFlags |= 0x8000000;
        }
        if (player->modeValue == 0) {
            if (player->updateTimer == 1) {
                enqueueRacePlayerVoiceSound(player, RACE_VOICE_CHEER);
                player->updateTimer++;
            }
            player->modeValue = 0;
            setRaceMotionAnimation(player, 0x25);
        } else {
            player->modeValue--;
        }
    }
}

void updateRacePlayerMode32Character4(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->modeValue = 0;
        setRaceMotionAnimation(player, 0x24);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->animationId == 0x25) {
            player->stateFlags |= 0x8000000;
        }
        if (player->modeValue == 0) {
            if (player->updateTimer == 1) {
                enqueueRacePlayerVoiceSound(player, RACE_VOICE_CHEER);
                player->updateTimer++;
            }
            player->modeValue = 0x32;
            setRaceMotionAnimation(player, 0x25);
        } else {
            player->modeValue--;
        }
    }
}

void updateRacePlayerMode32Character5(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        enqueueRacePlayerVoiceSound(player, RACE_VOICE_CHEER);
        player->modeValue = 0x1E;
        setRaceMotionAnimation(player, 0x23);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        player->stateFlags |= 0x8000000;
        if (player->modeValue == 0) {
            player->modeValue = 0x1E;
            setRaceMotionAnimation(player, 0x23);
        } else {
            player->modeValue--;
        }
    }
}

void func_800963D8(RacePlayer *player) {
    updateRacePlayerLeanAngle(player, 0, 0);
    gRacePlayerMode35CharacterHandlers[player->characterId](player);
}

void updateRacePlayerMode35Character0(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->modeValue = 0;
        setRaceMotionAnimation(player, 0x25);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->modeValue == 0) {
            player->stateFlags |= 0x8000000;
            player->modeValue = 0x1E;
            setRaceMotionAnimation(player, 0x26);
        } else {
            player->modeValue--;
        }
    }
}

void updateRacePlayerMode35Character1(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->modeValue = 0;
        setRaceMotionAnimation(player, 0x26);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->modeValue == 0) {
            player->stateFlags |= 0x8000000;
            player->modeValue = 0x1E;
            setRaceMotionAnimation(player, 0x27);
        } else {
            player->modeValue--;
        }
    }
}

void updateRacePlayerMode35Character2(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->modeValue = 0;
        setRaceMotionAnimation(player, 0x25);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->modeValue == 0) {
            player->modeValue = 0;
            player->stateFlags |= 0x8000000;
            setRaceMotionAnimation(player, 0x26);
        } else {
            player->modeValue--;
        }
    }
}

void func_800965C8(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->modeValue = 0;
        setRaceMotionAnimation(player, 0x26);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->modeValue == 0) {
            player->stateFlags |= 0x8000000;
            player->modeValue = 0;
            setRaceMotionAnimation(player, 0x27);
        } else {
            player->modeValue--;
        }
    }
}

#pragma GLOBAL_ASM("asm.jp/nonmatchings/race/player/race_player_update/func_80096650.s")

void updateRacePlayerMode35Character5(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->modeValue = 0;
        setRaceMotionAnimation(player, 0x24);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->modeValue == 0) {
            if (player->animationId == 0x25) {
                player->modeValue = 0;
                setRaceMotionAnimation(player, 0x26);
            } else {
                player->stateFlags |= 0x8000000;
                player->modeValue = 0x1E;
                setRaceMotionAnimation(player, 0x25);
            }
        } else {
            player->modeValue--;
        }
    }
}

void updateRacePlayersPostUpdate(void) {
    RacePlayer *player;
    Vec3i *soundPos;
    Vec3i *nextSoundPos;
    s32 i;

    if (gRaceUpdatePaused == 0) {
        i = 0;
        if (gRacePlayerCount > 0) {
            for (;;) {
                { updateRacePlayerPostUpdate(&gRacePlayers[i]); }
                i++;

                if (!(i < gRacePlayerCount)) {
                    break;
                }
            }
        }
    }

    i = 0;
    if (gRacePlayerCount > 0) {
        for (;;) {
            updateSpeedFanEffect(&gRacePlayers[i]);
            i++;

            if (!(i < gRacePlayerCount)) {
                break;
            }
        }
        i = 0;
    }

    i = 0;
    if (gRacePlayerCount > 0) {
        for (;;) {
            player = &gRacePlayers[i];
            soundPos = &gRacePlayers[i].pos;
            nextSoundPos = &gRacePlayers[i].renderPosition;
            if (player->soundDisabled == 0) {
                enqueuePlayerLoopingPositionalSoundRequest(
                    player->loopSoundId,
                    soundPos,
                    player->loopSoundVolume,
                    SFX_PRIORITY_PLAYER_LOOP,
                    player->loopSoundPitch,
                    i
                );
            } else {
                enqueuePlayerLoopingPositionalSoundRequest(
                    player->loopSoundId,
                    soundPos,
                    0,
                    SFX_PRIORITY_PLAYER_LOOP,
                    player->loopSoundPitch,
                    i
                );
            }

            *nextSoundPos = *soundPos;
            player->renderPosition.y = player->groundCorrectionY + player->renderPosition.y - player->groundOffset + 0xA000;
            if (player->soundDisabled == 0) {
                addRenderCallback(&gModelRenderCallbackQueue4Head, (RenderCallback)drawRacePlayerModel, player);
                addRenderCallback(&gModelRenderCallbackQueue7Head, (RenderCallback)drawRacePlayerGroundShadow, player);
            } else {
                addRenderCallback(&gModelRenderCallbackQueue7Head, (RenderCallback)drawRaceGhostPlayerModel, player);
            }
            i++;

            if (!(i < gRacePlayerCount)) {
                break;
            }
        }
    }
}

void updateRacePlayerPostUpdate(RacePlayer *player) {
    unsigned long long random;

    gRacePlayerModePostUpdateHandlers[player->mode](player);
    if (!(gMenuFlowState & 1) && !(player->stateFlags & 0x41000) && (player->soundDisabled == 0)) {
        if (player->groundContactMask & 3) {
            random = randomNextMain();
            spawnRaceItemTrackSparkBurst(
                &player->boardContactPoints[0],
                &player->boardContactPoints[1],
                &player->boardContactPoints[4],
                &player->boardContactPoints[5],
                random,
                player->surfaceType
            );
        }
        if (player->groundContactMask & 0xC) {
            random = randomNextMain();
            spawnRaceItemTrackSparkBurst(
                &player->boardContactPoints[2],
                &player->boardContactPoints[3],
                &player->boardContactPoints[6],
                &player->boardContactPoints[7],
                random,
                player->surfaceType
            );
        }
    }
    updateRacePlayerFinalLapStatus(player);
    resolveRacePlayerCollisionVolumes(player);
    updateRacePlayerRumble(player);
}

void updateRacePlayerPostUpdateNoop(RacePlayer *player) {
    updateRacePlayerSurfaceContact(player);
}

void updateRacePlayerPostUpdateCourseObject(RacePlayer *player) {
    if (player->updateState < 7) {
        updateRacePlayerGroundAlignment(player);
    } else {
        player->groundCorrectionY = 0;
        updateRacePlayerProjectedPosition(player);
        player->groundOffset = 0x30000;
    }
}

void updateRacePlayerPostUpdateMode00(RacePlayer *player) {
    if (updateRacePlayerSurfaceContact(player)) {
        player->mode = 0x16;
        player->updateState = 0;
        player->updateTimer = 0;
        player->stateFlags |= 0x200;
    } else if (tryStartRacePlayerCourseObjectMode(player->playerIndex)) {
        player->mode = 7;
        player->updateState = 0;
        player->updateTimer = 0;
    }
    interpolateRaceMotionJointAnimationFrame(player, 0, (0x60000 - player->groundOffset) / 0x600, 0x100);
}

void updateRacePlayerVoiceSounds(RacePlayer *player) {
    s32 soundType;
    RacePlayer *player2;
    s32 soundId;
    s32 score;
    s16 voiceType;
    s16 splitType;
    s32 bitIndex;
    u16 playerIndex;

    voiceType = player->aerialTrickId;
    player2 = player;
    soundId = 0;
    score = 0;
    if (voiceType >= 9) {
        player->unk57B = 1;
        voiceType = player2->aerialTrickId;
    }
    if ((voiceType != 0) && (player->grabTrickId != 0)) {
        player->unk57B = 1;
        voiceType = player2->aerialTrickId;
    }
    if (voiceType != 0) {
        soundId = gRacePlayerVoiceBaseSoundIds[voiceType];
        score = voiceType < 9 ? 0xA : 0x28;
        bitIndex = voiceType;
        if ((s32)(u32)bitIndex >= 0xA) {
            bitIndex = 9;
        }
        player->trickAttackCompletedTrickMask |= 1 << bitIndex;
    }
    splitType = player2->grabTrickId;
    if (splitType != 0) {
        if (player->aerialTrickId != 0) {
            soundId += gRacePlayerVoiceSplitSoundOffsets[splitType];
            if (splitType & 1) {
                score += 5;
            } else {
                score += 0xA;
            }
        } else {
            score += 1;
            soundId += gRacePlayerVoiceLeadSoundOffsets[splitType];
        }
        soundId += player2->grabHoldFrames;
    }
    if ((soundId != 0) && (gRaceSplitscreenMode == 0) && (gRaceDemoPlaybackEnabled == 0)) {
        addRacePlayerScore(player, soundId);
        playerIndex = player2->playerIndex;
        if (gRacePlayerHudStatuses[playerIndex].active != 0) {
            spawnRaceUiScorePopup((void *)soundId, (s16)playerIndex);
            soundType = SFX_TRICK_SCORE_LOW;
            if (soundId >= 0x64) {
                soundType = SFX_TRICK_SCORE_MEDIUM;
            }
            if (soundId >= 0x78) {
                soundType = SFX_TRICK_SCORE_HIGH;
            }
            if (soundId >= 0xC8) {
                soundType = SFX_TRICK_BONUS;
            }
            if (soundId >= 0x12C) {
                soundType = SFX_TRICK_SCORE_TOP;
            }
            enqueueSoundEffect((s16)soundType, SFX_PRIORITY_HIGH);
        }
    }
    if (gRaceSplitscreenMode == 1) {
        if (gRaceTypeSelection == 2) {
            if (score != 0) {
                soundType = SFX_TRICK_SCORE_LOW;
                if (score >= 0x2710) {
                    score = 0x270F;
                }
                player->trickAttackPointTotal += score;
                player->trickAttackMakeCount++;
                if (player->trickAttackMakeCount >= 0x64) {
                    player->trickAttackMakeCount = 0x63;
                }
                if (player->trickAttackBestMakeCount < player->trickAttackMakeCount) {
                    player->trickAttackBestMakeCount = player2->trickAttackMakeCount;
                }
                if (score >= 0xA) {
                    soundType = SFX_TRICK_SCORE_MEDIUM;
                }
                if (score >= 0xF) {
                    soundType = SFX_TRICK_SCORE_HIGH;
                }
                if (score >= 0x14) {
                    soundType = SFX_TRICK_BONUS;
                }
                if (score >= 0x28) {
                    soundType = SFX_TRICK_SCORE_TOP;
                }
                if (player2->trickAttackCompletedTrickMask == 0x3FE) {
                    player2->trickAttackPointTotal += 0x12C;
                    player2->trickAttackCompletedTrickMask = 0x3FF;
                    soundType = SFX_TRICK_BONUS;
                    spawnRaceUiTrickScorePopup((void *)score, 1);
                } else {
                    spawnRaceUiTrickScorePopup((void *)score, 0);
                }
                enqueueSoundEffect((s16)soundType, SFX_PRIORITY_HIGH);
                if (player2->trickAttackPointTotal >= 0x2710) {
                    player2->trickAttackPointTotal = 0x270F;
                }
            }
        }
    }
}

void updateRacePlayerLoopingSound(RacePlayer *player) {
    s32 v0 = -player->velocity.y >> 13;
    if (v0 >= 0x80) {
        v0 = 0x7F;
    }
    if (v0 < 0x46) {
        v0 = 0x46;
    }
    if (v0 >= 0x51) {
        player->rumbleTimer = 2;
        player->rumblePatternId = 0;
    } else {
        player->rumbleTimer = 1;
        player->rumblePatternId = 0;
    }
    if (player->soundDisabled == 0) {
        enqueuePositionalSoundEffect(SFX_PLAYER_DESCENT, (Vec3i *)&player->pos.x, (s16)v0, SFX_PRIORITY_NORMAL);
    }
}

void updateRacePlayerPostUpdateMode22(RacePlayer *player) {
    u32 stateFlags;

    if (updateRacePlayerSurfaceContact(player) == 0) {
        stateFlags = player->stateFlags;
        if (stateFlags & 8) {
            player->stateFlags = stateFlags & ~8;
        } else if (stateFlags & 0x800) {
            player->trickAttackMakeCount = 0;
            player->mode = 0xE;
            player->updateState = 0;
            player->updateTimer = 0;
            player->stateFlags = stateFlags & ~0x200;
            enqueueRacePlayerVoiceSound(player, RACE_VOICE_HIT);
        } else {
            player->stateFlags = stateFlags & ~0x200;
            if (tryStartRacePlayerCourseObjectMode(player->playerIndex) != 0) {
                player->mode = 7;
                player->updateState = 0;
                player->updateTimer = 0;
                updateRacePlayerVoiceSounds(player);
                updateRacePlayerLoopingSound(player);
            } else if (player->subState == 0) {
                player->mode = 0;
                player->updateState = 1;
                player->updateTimer = 0;
                player->unk57A = 2;
                updateRacePlayerLoopingSound(player);
            } else {
                player->mode = 0;
                player->updateState = 0;
                player->updateTimer = 0;
                player->unk57A = 2;
                updateRacePlayerVoiceSounds(player);
                updateRacePlayerLoopingSound(player);
            }
        }
    }
}

void updateRacePlayerPostUpdateAirborneTrick(RacePlayer *player) {
    u32 stateFlags;

    if (updateRacePlayerSurfaceContact(player) == 0) {
        stateFlags = player->stateFlags;
        if (stateFlags & 8) {
            player->stateFlags = stateFlags & ~8;
        } else if (stateFlags & 0x800) {
            player->trickAttackMakeCount = 0;
            player->mode = 0xE;
            player->updateState = 0;
            player->updateTimer = 0;
            player->stateFlags = stateFlags & ~0x200;
            enqueueRacePlayerVoiceSound(player, RACE_VOICE_HIT);
        } else {
            player->stateFlags = stateFlags & ~0x200;
            if (tryStartRacePlayerCourseObjectMode(player->playerIndex) != 0) {
                player->mode = 7;
                player->updateState = 0;
                player->updateTimer = 0;
                updateRacePlayerVoiceSounds(player);
                updateRacePlayerLoopingSound(player);
            } else {
                player->mode = 0;
                player->updateState = 0;
                player->updateTimer = 0;
                player->unk57A = 2;
                updateRacePlayerVoiceSounds(player);
                updateRacePlayerLoopingSound(player);
            }
        }
    }
}

void updateRacePlayerPostUpdateAirborneLaunch(RacePlayer *player) {
    if (updateRacePlayerSurfaceContact(player)) {
        player->mode = 0x16;
        player->updateState = 0;
        player->updateTimer = 0;
        player->stateFlags |= 0x200;
    } else if (tryStartRacePlayerCourseObjectMode(player->playerIndex)) {
        player->mode = 7;
        player->updateState = 0;
        player->updateTimer = 0;
    }
    interpolateRaceMotionJointAnimationFrame(player, 0, (0x60000 - player->groundOffset) / 0x600, 0x100);
}

void updateRacePlayerPostUpdateNudge(RacePlayer *player) {
    if (updateRacePlayerSurfaceContact(player)) {
        player->stateFlags |= 0x200;
    } else {
        player->stateFlags &= ~0x200;
    }
}

void updateRacePlayerPostUpdateMode29(RacePlayer *player) {
    u16 playerIndex;

    updateRacePlayerSurfaceContact(player);
    if (!(player->stateFlags & 0x02000000)) {
        player->mode = 1;
        player->updateState = 0;
        player->updateTimer = 0;
        if ((gRaceSplitscreenMode == 1) && (gRaceTypeSelection == 2)) {
            player->trickAttackPointTotal *= 2;
            if (player->trickAttackPointTotal >= 0x2710) {
                player->trickAttackPointTotal = 0x270F;
            }
            createCallbackTaskPreservingArgs((CallbackTaskCallback)initRaceUiCrashScorePopup, 0, 0x64);
            enqueueSoundEffect(SFX_TRICK_BONUS, SFX_PRIORITY_NORMAL);
        }
        if (gRaceSplitscreenMode == 0) {
            addRacePlayerScore(player, 0x12C);
            playerIndex = player->playerIndex;
            if (gRacePlayerHudStatuses[playerIndex].active != 0) {
                spawnRaceUiScorePopup((void *)0x12C, (s16)playerIndex);
                enqueueSoundEffect(SFX_TRICK_BONUS, SFX_PRIORITY_NORMAL);
            }
        }
    }
}

void updateRacePlayerPostUpdateAttack(RacePlayer *player) {
    updateRacePlayerSurfaceContact(player);
    if (player->updateState < 2) {
        interpolateRaceMotionJointAnimationFrame(player, 0, (0x60000 - player->groundOffset) / 0x600, 0x100);
    }
}
