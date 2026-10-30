#include "ctx.c"
void updateRacePlayerAirborneCruise(RacePlayer *player) {
    s32 sp2C;
    s32 var_a2;
    s32 var_v1;

    sp2C = 0;
    if (!player->isCpu) {
        if ((player->state.coursePathIndex == gRaceCourseStartEntries[gRaceCourseIndex.signedValue].pathIndex) &&
            (!(player->state.stateFlags & 0x40))) {
            var_v1 = calculateFixedAngleBetweenXZPoints(
                         player->state.pos.x,
                         player->state.pos.z,
                         gRaceCourseStartEntries[gRaceCourseIndex.signedValue].startSurfaceTargetX,
                         gRaceCourseStartEntries[gRaceCourseIndex.signedValue].startSurfaceTargetZ
                     ) -
                     player->state.facingAngle + 0x400;
            if ((s16)(var_v1 & 0xFFF) < 0x800) {
                if (player->state.stateFlags & 0x400) {
                    player->state.stateFlags &= ~0x400;
                    player->state.stanceTransitionFrame = 6 - player->state.stanceTransitionFrame;
                }
            } else {
                if (!(player->state.stateFlags & 0x400)) {
                    player->state.stateFlags |= 0x400;
                    player->state.stanceTransitionFrame = 6 - player->state.stanceTransitionFrame;
                }
            }
        } else {
            if (player->state.localVelocityZ < (-0x8000)) {
                if (player->state.stateFlags & 0x400) {
                    player->state.stateFlags &= ~0x400;
                    player->state.stanceTransitionFrame = 6 - player->state.stanceTransitionFrame;
                }
            }
            if (player->state.localVelocityZ >= 0x8001) {
                if (!(player->state.stateFlags & 0x400)) {
                    player->state.stateFlags |= 0x400;
                    player->state.stanceTransitionFrame = 6 - player->state.stanceTransitionFrame;
                }
            }
        }
        if (!gRaceDemoPlaybackEnabled) {
            if (player->state.unk336 < 0x5A) {
                player->state.unk336++;
            }
        }
    }

    if (player->state.stanceTransitionFrame) {
        setRaceMotionAnimation(player, player->state.stanceTransitionFrame-- + 8);
    } else if (player->state.animationId != 5) {
        setRaceMotionAnimation(player, 5);
    }

    if (player->state.stateFlags & 0x10) {
        if (!player->isCpu) {
            if (!(player->state.heldInput & A_BUTTON)) {
                player->state.stateFlags &= ~0x10;
            }
        } else {
            player->state.stateFlags &= ~0x10;
            if (player->state.unk525) {
                player->state.modeValue = 0x46000;
            }
        }
        if (player->state.stateFlags & 0x10) {
            if (player->state.modeValue < 0x46000) {
                player->state.modeValue += 0x2000;
            }
        }
    }
    updateRacePlayerShotUse(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.gravity;
    var_a2 = 0;
    if (player->state.trailEffectTimer) {
        var_a2 = -0x8000;
        if (player->state.snowboardTrailSide) {
            var_a2 = 0x8000;
        }
    }
    if (!(player->state.stateFlags & 0x10)) {
        if (player->state.stateFlags & 0x400) {
            sp2C = 0x30000;
            if ((u8)player->state.unk519) {
                var_a2 = 0x100000;
            }
            if (player->state.unk336 >= 0x3C) {
                player->state.targetSpeedLimit += 0x50000;
                player->state.speedLimit += 0x50000;
                var_a2 += 0x50000;
                enqueueRacePlayerVoiceSound(player, RACE_VOICE_TRICK);
            }
        } else {
            sp2C = -0x30000;
            if ((u8)player->state.unk519) {
                var_a2 = -0x100000;
            }
            if (player->state.unk336 >= 0x3C) {
                player->state.targetSpeedLimit += 0x50000;
                player->state.speedLimit += 0x50000;
                var_a2 += 0xFFFB0000;
                enqueueRacePlayerVoiceSound(player, RACE_VOICE_TRICK);
            }
        }
    }
    updateRacePlayerLocalVelocity(
        player,
        sp2C,
        var_a2,
        player->state.lateralDeceleration,
        player->state.forwardDeceleration,
        player->state.reverseDeceleration
    );
    var_v1 = updateRacePlayerSurfaceCue(player);
    if (!(player->state.stateFlags & 0x10)) {
        if (player->state.modeValue < 0x46000) {
            var_v1 = 0;
        }
        if (var_v1 >= 9) {
            player->state.modeValue = 0x50000;
        }
        player->state.grabTrickId = 0;
        switch (var_v1) {
            case 1:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_TRICK);
                }
                player->state.mode = 0xD;
                player->state.aerialTrickId = 1;
                player->state.stateFlags |= 0x800;
                break;

            case 2:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_TRICK);
                }
                player->state.mode = 0xF;
                player->state.aerialTrickId = 2;
                player->state.stateFlags |= 0x800;
                break;

            case 3:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_TRICK);
                }
                player->state.mode = 0x10;
                player->state.aerialTrickId = 3;
                player->state.stateFlags |= 0x800;
                break;

            case 4:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_TRICK);
                }
                player->state.mode = 0x11;
                player->state.aerialTrickId = 4;
                player->state.stateFlags |= 0x800;
                break;

            case 5:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x12;
                player->state.aerialTrickId = 5;
                player->state.stateFlags |= 0x800;
                break;

            case 6:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x13;
                player->state.aerialTrickId = 6;
                player->state.stateFlags |= 0x800;
                break;

            case 7:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x14;
                player->state.aerialTrickId = 7;
                player->state.stateFlags |= 0x800;
                break;

            case 8:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x15;
                player->state.aerialTrickId = 8;
                player->state.stateFlags |= 0x800;
                break;

            case 9:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x1F;
                player->state.aerialTrickId = 0xB;
                player->state.stateFlags |= 0x800;
                break;

            case 10:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x20;
                player->state.aerialTrickId = 0xB;
                player->state.stateFlags |= 0x800;
                break;

            case 11:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x21;
                player->state.aerialTrickId = 0xC;
                player->state.stateFlags |= 0x800;
                break;

            case 12:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x22;
                player->state.aerialTrickId = 0xD;
                player->state.stateFlags |= 0x800;
                break;

            case 13:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x23;
                player->state.aerialTrickId = 0xA;
                player->state.stateFlags |= 0x800;
                break;

            case 14:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x24;
                player->state.aerialTrickId = 0xC;
                player->state.stateFlags |= 0x800;
                break;

            case 15:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x25;
                player->state.aerialTrickId = 0xB;
                player->state.stateFlags |= 0x800;
                break;

            case 16:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x26;
                player->state.aerialTrickId = 0xD;
                player->state.stateFlags |= 0x800;
                break;

            case 17:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x27;
                player->state.aerialTrickId = 0xE;
                player->state.stateFlags |= 0x800;
                break;

            case 18:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x29;
                player->state.aerialTrickId = 9;
                player->state.stateFlags |= 0x800;
                break;

            case 19:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x2A;
                player->state.aerialTrickId = 0xA;
                player->state.stateFlags |= 0x800;
                break;

            case 20:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x2B;
                player->state.aerialTrickId = 0xC;
                player->state.stateFlags |= 0x800;
                break;

            case 21:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x2C;
                player->state.aerialTrickId = 9;
                player->state.stateFlags |= 0x800;
                break;

            case 22:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x2D;
                player->state.aerialTrickId = 0xA;
                player->state.stateFlags |= 0x800;
                break;

            case 23:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x2E;
                player->state.aerialTrickId = 0xE;
                player->state.stateFlags |= 0x800;
                break;

            case 24:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x2F;
                player->state.aerialTrickId = 9;
                player->state.stateFlags |= 0x800;
                break;

            case 25:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x30;
                player->state.aerialTrickId = 0xB;
                player->state.stateFlags |= 0x800;
                break;

            case 26:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x31;
                player->state.aerialTrickId = 0xD;
                player->state.stateFlags |= 0x800;
                break;

            case 27:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x32;
                player->state.aerialTrickId = 9;
                player->state.stateFlags |= 0x800;
                break;

            case 28:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x33;
                player->state.aerialTrickId = 0xA;
                player->state.stateFlags |= 0x800;
                break;

            case 29:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x34;
                player->state.aerialTrickId = 0xC;
                player->state.stateFlags |= 0x800;
                break;

            case 30:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x35;
                player->state.aerialTrickId = 0xA;
                player->state.stateFlags |= 0x800;
                break;

            case 31:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x36;
                player->state.aerialTrickId = 0xC;
                player->state.stateFlags |= 0x800;
                break;

            case 32:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x37;
                player->state.aerialTrickId = 9;
                player->state.stateFlags |= 0x800;
                break;

            case 33:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x38;
                player->state.aerialTrickId = 0xA;
                player->state.stateFlags |= 0x800;
                break;

            case 34:
                if (!player->soundDisabled) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, (Vec3i *)(&player->state.pos), 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x39;
                player->state.aerialTrickId = 0xD;
                player->state.stateFlags |= 0x800;
                break;

            case 0:

            default:
                player->state.mode = 1;
                player->state.aerialTrickId = 0;
                break;
        }

        player->state.updateState = 0;
        player->state.updateTimer = 0;
        player->state.velocity.y += player->state.modeValue;
        player->state.pos.y += 0x60000;
        player->state.verticalVelocityLimit = player->state.velocity.y;
        player->state.stateFlags |= 0x208;
        setRaceMotionAnimation(player, 4);
    } else {
        updateRacePlayerSurfaceCue(player);
    }
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += player->state.velocity.y;
    player->state.pos.z += player->state.velocity.z;
    stepRaceMotionJointAnimationUntilEnd(player);
    updateRacePlayerMotionFeedback(player);
    if (player->state.unk517) {
        enqueueRacePlayerVoiceSound(player, RACE_VOICE_ATTACK_SUCCESS);
    }
}
