#include "common.h"
#include "game/math/spatial_math.h"
#define calculateFixedAngleBetweenXZPoints calculateFixedAngleBetweenXZPoints_s32
#include "game/math/fixed_point_math.h"
#undef calculateFixedAngleBetweenXZPoints
#include "game/audio/sound_manager.h"
#include "game/race/motion/race_motion.h"
#include "game/race/race_state.h"
#include "game/race/player/race_player_collision.h"
#include "game/race/items/race_item_effects.h"
#include "game/race/items/race_item_hit_flags.h"

#define RACE_PLAYER_COLLISION_YAW_FLIPPED 0x400
#define RACE_PLAYER_COLLISION_ACTIVE 0x800
#define RACE_PLAYER_COLLISION_SQUASHED 0x200000
#define RACE_PLAYER_COLLISION_ALWAYS_VALID 0x400000

#define RACE_PLAYER_COLLISION_ANGLE_MASK 0xFFF
#define RACE_PLAYER_COLLISION_HALF_TURN 0x800

extern s32 gRacePlayerHitCueId;
extern s32 gRacePlayerHitAngle;
extern s32 gRacePlayerHitDistance;
extern s16 gRacePlayerHitEffectAngle;
extern s32 gRacePlayerHitDeltaX;
extern s32 gRacePlayerHitDeltaZ;
s32 enqueueSoundEffect(s16, s16);
void enqueueRacePlayerVoiceSound(RacePlayer *, s32);

s32 tryQueueRacePlayerHitCueSpinout(RacePlayer *arg0) {
    s32 temp_v0;
    s32 var_v1;
    s32 temp_t0;

    temp_v0 = arg0->stateFlags;
    if (temp_v0 & 0x379000) {
        return 1;
    }
    if (!(temp_v0 & RACE_PLAYER_COLLISION_ALWAYS_VALID)) {
        var_v1 = gRacePlayerHitAngle - arg0->facingAngle;
        var_v1 += RACE_PLAYER_COLLISION_HALF_TURN;
        temp_t0 = var_v1 & RACE_PLAYER_COLLISION_ANGLE_MASK;
        var_v1 = (s16)temp_t0;
        if (temp_v0 & RACE_PLAYER_COLLISION_YAW_FLIPPED) {
            var_v1 = (s16)(var_v1 + RACE_PLAYER_COLLISION_HALF_TURN);
        }
        var_v1 += 0x200;
        var_v1 = (s16)(var_v1 & RACE_PLAYER_COLLISION_ANGLE_MASK);
        if ((gRacePlayerHitDistance < 0x30000) || (var_v1 >= 0x401)) {
            return 1;
        }
    }
    if (gRacePlayerHitCueId < 5) {
        gRacePlayerHitCueId = 5;
    }
    return 0;
}

s32 tryQueueRacePlayerHitCueGhostSpinout(RacePlayer *arg0) {
    s32 temp_v0;
    s32 var_v1;
    s32 temp_t0;

    temp_v0 = arg0->stateFlags;
    if (temp_v0 & 0x379000) {
        return 1;
    }
    if (!(temp_v0 & RACE_PLAYER_COLLISION_ALWAYS_VALID)) {
        var_v1 = gRacePlayerHitAngle - arg0->facingAngle;
        var_v1 += RACE_PLAYER_COLLISION_HALF_TURN;
        temp_t0 = var_v1 & RACE_PLAYER_COLLISION_ANGLE_MASK;
        var_v1 = (s16)temp_t0;
        if (temp_v0 & RACE_PLAYER_COLLISION_YAW_FLIPPED) {
            var_v1 = (s16)(var_v1 + RACE_PLAYER_COLLISION_HALF_TURN);
        }
        var_v1 += 0x200;
        var_v1 = (s16)(var_v1 & RACE_PLAYER_COLLISION_ANGLE_MASK);
        if ((gRacePlayerHitDistance < 0x30000) || (var_v1 >= 0x401)) {
            return 1;
        }
    }
    if (gRacePlayerHitCueId < 6) {
        gRacePlayerHitCueId = 6;
    }
    return 0;
}

s32 tryQueueRacePlayerHitCueLaunch(RacePlayer *arg0) {
    if (arg0->stateFlags & 0x37D000) {
        return 1;
    }
    if (gRacePlayerHitCueId < 7) {
        gRacePlayerHitCueId = 7;
    }
    return 0;
}

s32 tryQueueRacePlayerHitCueStun(RacePlayer *arg0) {
    if (arg0->stateFlags & 0x77D000) {
        return 1;
    }
    if (gRacePlayerHitCueId < 0xB) {
        gRacePlayerHitCueId = 0xB;
    }
    return 0;
}

s32 tryQueueRacePlayerHitCueTrip(RacePlayer *arg0) {
    if (arg0->stateFlags & 0x77D000) {
        return 1;
    }
    if (gRacePlayerHitCueId < 0xA) {
        gRacePlayerHitCueId = 0xA;
    }
    return 0;
}

s32 tryQueueRacePlayerHitCueTumble(RacePlayer *arg0) {
    if (arg0->stateFlags & 0x77D000) {
        return 1;
    }
    if (gRacePlayerHitCueId < 0xC) {
        gRacePlayerHitCueId = 0xC;
    }
    return 0;
}

s32 tryQueueRacePlayerHitCueSlide(RacePlayer *arg0) {
    if (arg0->stateFlags & 0x77D000) {
        return 1;
    }
    if (gRacePlayerHitCueId < 0xD) {
        gRacePlayerHitCueId = 0xD;
    }
    return 0;
}

s32 tryQueueRacePlayerHitCuePushForward(RacePlayer *arg0) {
    s32 temp_v0 = arg0->stateFlags;

    if (temp_v0 & 0x43000) {
        return 1;
    }
    if (!(temp_v0 & RACE_PLAYER_COLLISION_ACTIVE)) {
        return 1;
    }
    if (gRacePlayerHitCueId < 8) {
        gRacePlayerHitCueId = 8;
    }
    return 0;
}

s32 tryQueueRacePlayerHitCuePushBack(RacePlayer *arg0) {
    s32 temp_v0 = arg0->stateFlags;

    if (temp_v0 & 0x43000) {
        return 1;
    }
    if (!(temp_v0 & RACE_PLAYER_COLLISION_ACTIVE)) {
        return 1;
    }
    if (gRacePlayerHitCueId < 9) {
        gRacePlayerHitCueId = 9;
    }
    return 0;
}

s32 tryQueueRacePlayerHitCueNudge(RacePlayer *arg0) {
    s32 temp_v0;
    s32 var_v1;
    s32 temp_t9;

    temp_v0 = arg0->stateFlags;
    if (temp_v0 & 0x43000) {
        return 1;
    }
    var_v1 = gRacePlayerHitAngle - arg0->facingAngle;
    var_v1 += RACE_PLAYER_COLLISION_HALF_TURN;
    temp_t9 = var_v1 & RACE_PLAYER_COLLISION_ANGLE_MASK;
    var_v1 = (s16)temp_t9;
    if (temp_v0 & RACE_PLAYER_COLLISION_YAW_FLIPPED) {
        var_v1 = (s16)(var_v1 + RACE_PLAYER_COLLISION_HALF_TURN);
    }
    var_v1 += 0x400;
    var_v1 = (s16)(var_v1 & RACE_PLAYER_COLLISION_ANGLE_MASK);
    if ((gRacePlayerHitDistance < 0x14000) || (var_v1 >= 0x801)) {
        return 1;
    }
    if (gRacePlayerHitCueId < 2) {
        gRacePlayerHitCueId = 2;
    }
    return 0;
}

s32 tryQueueRacePlayerHitCueSlowdown(RacePlayer *arg0) {
    if (arg0->stateFlags & 0x43000) {
        return 1;
    }
    if (gRacePlayerHitCueId <= 0) {
        gRacePlayerHitCueId = 1;
    }
    return 0;
}

s32 tryQueueRacePlayerHitCueObstacle(RacePlayer *arg0) {
    if (arg0->stateFlags & 0x345001) {
        return 1;
    }
    if (gRacePlayerHitCueId < 0xE) {
        gRacePlayerHitCueId = 0xE;
    }
    return 0;
}

s32 tryQueueRacePlayerHitCueHeavyKnockdown(RacePlayer *arg0) {
    if (arg0->stateFlags & 0x775000) {
        return 1;
    }
    if (gRacePlayerHitCueId < 0xF) {
        gRacePlayerHitCueId = 0xF;
    }
    return 0;
}

s32 tryQueueRacePlayerHitCueItemSteal(RacePlayer *arg0) {
    if (arg0->stateFlags & 0x375000) {
        return 1;
    }
    if (arg0->unk2D4 != 0) {
        return 1;
    }
    if (arg0->actionSoundTimer != 0) {
        return 1;
    }
    if (gRacePlayerHitCueId < 0x10) {
        gRacePlayerHitCueId = 0x10;
    }
    return 0;
}

s32 tryQueueRacePlayerHitCueGhostSlowdown(RacePlayer *arg0) {
    if (arg0->stateFlags & 0x375000) {
        return 1;
    }
    if (arg0->actionSoundTimer != 0) {
        return 1;
    }
    if (gRacePlayerHitCueId < 0x11) {
        gRacePlayerHitCueId = 0x11;
    }
    return 0;
}

s32 tryQueueRacePlayerHitCueSpin(RacePlayer *arg0) {
    if (arg0->stateFlags & 0x775000) {
        return 1;
    }
    if (gRacePlayerHitCueId < 3) {
        gRacePlayerHitCueId = 3;
    }
    return 0;
}

s32 tryQueueRacePlayerHitCueReverseSpin(RacePlayer *arg0) {
    if (arg0->stateFlags & 0x775000) {
        return 1;
    }
    if (gRacePlayerHitCueId < 4) {
        gRacePlayerHitCueId = 4;
    }
    return 0;
}

s32 tryQueueRacePlayerHitCueTerrainFall(RacePlayer *arg0) {
    if (arg0->stateFlags & 0x41001) {
        return 1;
    }
    if (gRacePlayerHitCueId < 0x12) {
        gRacePlayerHitCueId = 0x12;
    }
    return 0;
}

s32 tryQueueRacePlayerHitCueTerrainCrash(RacePlayer *arg0) {
    if (arg0->stateFlags & 0x41001) {
        return 1;
    }
    if (gRacePlayerHitCueId < 0x13) {
        gRacePlayerHitCueId = 0x13;
    }
    return 0;
}

void resolveRacePlayerHitReactions(RacePlayer *arg0) {
    u16 temp_v0;

    gRacePlayerHitCueId = 0;
    temp_v0 = arg0->pendingItemHitFlags;
    if (temp_v0 != 0) {
        if ((temp_v0 & 0x200) &&
            (arg0->unk5C < (getRaceCourseSurfaceHeight(arg0->coursePathIndex, arg0->pos.x, arg0->pos.z) + 0xB0000))) {
            arg0->pendingItemHitFlags |= 1;
        }
        gRacePlayerHitDeltaX = arg0->unk2C8 - arg0->velocity.x;
        gRacePlayerHitDeltaZ = arg0->unk2CC - arg0->velocity.z;
        gRacePlayerHitDistance =
            (u32)integerSquareRoot64(
                (s64)gRacePlayerHitDeltaX * gRacePlayerHitDeltaX + (s64)gRacePlayerHitDeltaZ * gRacePlayerHitDeltaZ
            ) >>
            1;
        gRacePlayerHitAngle = calculateFixedAngleFromDeltaXZ(gRacePlayerHitDeltaX, gRacePlayerHitDeltaZ);
        if ((arg0->pendingItemHitFlags & 1) && (tryQueueRacePlayerHitCuePushForward(arg0) != 0) &&
            (tryQueueRacePlayerHitCueSpinout(arg0) != 0) && (tryQueueRacePlayerHitCueNudge(arg0) != 0)) {
            tryQueueRacePlayerHitCueSlowdown(arg0);
        }
        if ((arg0->pendingItemHitFlags & 2) && (tryQueueRacePlayerHitCuePushForward(arg0) != 0)) {
            tryQueueRacePlayerHitCueSpinout(arg0);
        }
        if (arg0->pendingItemHitFlags & 0x800) {
            tryQueueRacePlayerHitCueGhostSpinout(arg0);
        }
        if ((arg0->pendingItemHitFlags & 0x400) && (tryQueueRacePlayerHitCueTrip(arg0) != 0)) {
            tryQueueRacePlayerHitCueTrip(arg0);
        }
        if ((arg0->pendingItemHitFlags & 4) && (tryQueueRacePlayerHitCuePushBack(arg0) != 0)) {
            tryQueueRacePlayerHitCueLaunch(arg0);
        }
        if (arg0->pendingItemHitFlags & 8) {
            tryQueueRacePlayerHitCueStun(arg0);
        }
        if (arg0->pendingItemHitFlags & 0x100) {
            tryQueueRacePlayerHitCueSlide(arg0);
        }
        if (arg0->pendingItemHitFlags & 0x10) {
            tryQueueRacePlayerHitCueHeavyKnockdown(arg0);
        }
        if (arg0->pendingItemHitFlags & 0x20) {
            tryQueueRacePlayerHitCueItemSteal(arg0);
        }
        if (arg0->pendingItemHitFlags & PLAYER_HITFLAG_GHOST_SLOWDOWN) {
            tryQueueRacePlayerHitCueGhostSlowdown(arg0);
        }
        if (arg0->pendingItemHitFlags & 0x40) {
            tryQueueRacePlayerHitCueSpin(arg0);
        }
        if (arg0->pendingItemHitFlags & 0x2000) {
            tryQueueRacePlayerHitCueReverseSpin(arg0);
        }
        if (arg0->pendingItemHitFlags & 0x80) {
            tryQueueRacePlayerHitCueTumble(arg0);
        }
    }
    arg0->pendingItemHitFlags = 0;
    if (arg0->unk330 == 3) {
        if (arg0->unk331 == 3) {
            tryQueueRacePlayerHitCueObstacle(arg0);
        }
    }
    if (arg0->unk330 == 8) {
        tryQueueRacePlayerHitCueObstacle(arg0);
    }
    if ((arg0->unk330 == 6) || (arg0->unk330 == 0xC) || (arg0->unk330 == 0x10)) {
        tryQueueRacePlayerHitCueTerrainFall(arg0);
    }
    if ((arg0->unk330 == 7) || (arg0->unk330 == 0xE) || (arg0->unk330 == 0xF)) {
        tryQueueRacePlayerHitCueTerrainCrash(arg0);
    }
    if (gRacePlayerHitCueId != 0) {
        if (arg0->stateFlags & 0x800) {
            arg0->unk2C3 = 0;
        }
        arg0->unk57A = 0;
        switch (gRacePlayerHitCueId) {
            case 5:
            case 6:
                if (gRacePlayerHitCueId == 6) {
                    if (gRacePlayerHudStatuses[arg0->playerIndex].active != 0) {
                        enqueueSoundEffect(0x47, 0x32);
                    }
                } else {
                    enqueueRacePlayerVoiceSound(arg0, 2);
                }
                arg0->mode = 4;
                arg0->updateState = 0;
                arg0->updateTimer = 0;
                arg0->unk2DE = (s16)gRacePlayerHitAngle;
                arg0->unk2E0 = arg0->unk2C8;
                arg0->unk2E4 = arg0->unk2CC;
                if (arg0->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(0x14, &arg0->pos, 0x7F, 0x32);
                    spawnRacePlayerHitEffect(
                        (s16)arg0->playerIndex,
                        arg0->unk2DE,
                        0,
                        arg0->collisionHeight / 2,
                        arg0->collisionRadius
                    );
                }
                break;
            case 8:
                enqueueRacePlayerVoiceSound(arg0, 2);
                arg0->mode = 0xE;
                arg0->updateState = 0;
                arg0->updateTimer = 0;
                if (arg0->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(0x14, &arg0->pos, 0x7F, 0x32);
                    spawnRacePlayerHitEffect(
                        (s16)arg0->playerIndex,
                        gRacePlayerHitEffectAngle,
                        0,
                        arg0->collisionHeight / 2,
                        arg0->collisionRadius
                    );
                }
                break;
            case 9:
                enqueueRacePlayerVoiceSound(arg0, 2);
                arg0->mode = 0xE;
                arg0->updateState = 0;
                arg0->updateTimer = 0;
                arg0->unk2DE = (s16)gRacePlayerHitAngle;
                arg0->unk2E0 = arg0->unk2C8;
                arg0->unk2E4 = arg0->unk2CC;
                break;
            case 2:
                enqueueRacePlayerVoiceSound(arg0, 2);
                arg0->mode = 3;
                arg0->updateState = 0;
                arg0->updateTimer = 0;
                arg0->unk2DE = (s16)gRacePlayerHitAngle;
                arg0->unk2E0 = arg0->unk2C8;
                arg0->unk2E4 = arg0->unk2CC;
                if (arg0->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(0x14, &arg0->pos, 0x7F, 0x32);
                    spawnRacePlayerHitEffect(
                        (s16)arg0->playerIndex,
                        arg0->unk2DE,
                        1,
                        arg0->collisionHeight / 2,
                        arg0->collisionRadius
                    );
                }
                break;
            case 1:
                arg0->unk310 = 0xC000;
                break;
            case 7:
                if (arg0->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(0x14, &arg0->pos, 0x7F, 0x32);
                    spawnRacePlayerHitEffect((s16)arg0->playerIndex, 0, 1, arg0->collisionHeight / 2, 0);
                }
                /* fallthrough */
            case 10:
                enqueueRacePlayerVoiceSound(arg0, 2);
                arg0->mode = 0xB;
                arg0->updateState = 0;
                arg0->updateTimer = 0;
                break;
            case 11:
                gRacePlayers[arg0->hitSourcePlayerIndex & 3].unk517 = 1;
                if (gRacePlayers[arg0->hitSourcePlayerIndex & 3].isCpu != 0) {
                    enqueueRacePlayerVoiceSound(arg0, 2);
                } else {
                    enqueueRacePlayerVoiceSound(arg0, 3);
                }
                arg0->mode = 0x28;
                arg0->updateState = 0;
                arg0->updateTimer = 0;
                break;
            case 12:
                gRacePlayers[arg0->hitSourcePlayerIndex & 3].unk517 = 1;
                if (gRacePlayers[arg0->hitSourcePlayerIndex & 3].isCpu != 0) {
                    enqueueRacePlayerVoiceSound(arg0, 2);
                } else {
                    enqueueRacePlayerVoiceSound(arg0, 3);
                }
                arg0->mode = 0x1A;
                arg0->updateState = 0;
                arg0->updateTimer = 0;
                break;
            case 13:
                gRacePlayers[arg0->hitSourcePlayerIndex & 3].unk517 = 1;
                if (gRacePlayers[arg0->hitSourcePlayerIndex & 3].isCpu != 0) {
                    enqueueRacePlayerVoiceSound(arg0, 2);
                } else {
                    enqueueRacePlayerVoiceSound(arg0, 3);
                }
                arg0->mode = 0x1B;
                arg0->updateState = 0;
                arg0->updateTimer = 0;
                break;
            case 14:
                enqueueRacePlayerVoiceSound(arg0, 4);
                arg0->mode = 0xA;
                arg0->updateState = 0;
                arg0->updateTimer = 0;
                break;
            case 15:
                gRacePlayers[arg0->hitSourcePlayerIndex & 3].unk517 = 1;
                if (gRacePlayers[arg0->hitSourcePlayerIndex & 3].isCpu != 0) {
                    enqueueRacePlayerVoiceSound(arg0, 2);
                } else {
                    enqueueRacePlayerVoiceSound(arg0, 3);
                }
                arg0->mode = 0x17;
                arg0->updateState = 0;
                arg0->updateTimer = 0;
                break;
            case 16:
            case 17:
                if (gRacePlayerHitCueId == 0x11) {
                    if (gRacePlayers[arg0->hitSourcePlayerIndex & 3].isCpu != 0) {
                        enqueueRacePlayerVoiceSound(arg0, 2);
                    } else {
                        enqueueRacePlayerVoiceSound(arg0, 3);
                    }
                } else {
                    enqueueRacePlayerVoiceSound(arg0, 2);
                }
                if (arg0->soundDisabled == 0) {
                    spawnRacePlayerHitEffect((s16)arg0->playerIndex, 0, 3, arg0->collisionHeight / 2, 0);
                }
                arg0->mode = 0x18;
                arg0->updateState = 0;
                arg0->updateTimer = 0;
                break;
            case 3:
                gRacePlayers[arg0->hitSourcePlayerIndex & 3].unk517 = 1;
                if (gRacePlayers[arg0->hitSourcePlayerIndex & 3].isCpu != 0) {
                    enqueueRacePlayerVoiceSound(arg0, 2);
                } else {
                    enqueueRacePlayerVoiceSound(arg0, 3);
                }
                arg0->mode = 0x19;
                arg0->updateState = 0;
                arg0->updateTimer = 0;
                break;
            case 4:
                enqueueRacePlayerVoiceSound(arg0, 2);
                arg0->mode = 0x19;
                arg0->updateState = 0;
                arg0->updateTimer = 0;
                break;
            case 18:
                arg0->mode = 6;
                arg0->updateState = 0;
                arg0->updateTimer = 0;
                if (getRaceItemEffectType(arg0->unk330) != 0) {
                    arg0->mode = 0x1C;
                }
                break;
            case 19:
                arg0->mode = 9;
                arg0->updateState = 0;
                arg0->updateTimer = 0;
                break;
        }
    }
}
s32 isRacePlayerRespawnSurfaceValid(RacePlayer *arg0) {
    s32 temp_v0;

    temp_v0 = getRaceCourseSurfaceType(arg0->coursePathIndex, arg0->pos.x, arg0->pos.z);
    if (temp_v0 == 3) {
        return 0;
    }
    if (temp_v0 == 6) {
        return 0;
    }
    if (temp_v0 == 7) {
        return 0;
    }
    if (temp_v0 == 8) {
        return 0;
    }
    if (temp_v0 == 0xC) {
        return 0;
    }
    if (temp_v0 == 0xE) {
        return 0;
    }
    if (temp_v0 == 0xF) {
        return 0;
    }
    if (temp_v0 == 0x10) {
        return 0;
    }
    return 1;
}

void resolveRacePlayerCollisionVolumes(RacePlayer* player) {
    RacePlayerCollisionSource* source;
    RacePlayer* volumeView;
    RacePlayer* parentView;
    RacePlayer* sourceView;
    s16 localAxes[14][16];
    s32 i;
    s32 j;
    s32 k;
    Mat3x3 tempMtx;
    s32 sinX;
    s32 cosX;
    s32 sinY;
    Mat3x3 playerMtx;
    s32 cosY;
    s32 sinZ;
    s32 cosZ;
    Mat3x3 baseMtx;
    s32 offset[3];
    Mat3x3 effectMtx;
    s16 parentAxisY;
    s32 temp;
    RacePlayerCollisionSource* sourceIterator;
    s32 sourceIndex;
    s32 parentIndex;

    player->unk288 = player->pitchAngle;
    player->unk28A = player->facingAngle;
    player->unk28C = player->unk2EE;
    if (player->stateFlags & 0x400) {
        for (i = 0; i < player->collisionVolumeCount; i++) {
            sinX = fixedSine(player->collisionSources[player->collisionSources[i].mirroredParentIndex].rotX);
            cosX = fixedCosine(player->collisionSources[player->collisionSources[i].mirroredParentIndex].rotX);
            sinY = fixedSine(-player->collisionSources[player->collisionSources[i].mirroredParentIndex].rotY);
            cosY = fixedCosine(-player->collisionSources[player->collisionSources[i].mirroredParentIndex].rotY);
            sinZ = fixedSine(-player->collisionSources[player->collisionSources[i].mirroredParentIndex].rotZ);
            cosZ = fixedCosine(-player->collisionSources[player->collisionSources[i].mirroredParentIndex].rotZ);
            localAxes[i][0] = (((s32) cosY) * cosZ) / 0x1000;
            localAxes[i][1] = (((s32) cosY) * sinZ) / 0x1000;
            localAxes[i][2] = -sinY;
            temp = (((s32) sinX) * sinY) / 0x1000;
            localAxes[i][3] = ((((s32) temp) * cosZ) / 0x1000) + ((((s32) cosX) * (-sinZ)) / 0x1000);
            localAxes[i][4] = ((((s32) temp) * sinZ) / 0x1000) + ((((s32) cosX) * cosZ) / 0x1000);
            localAxes[i][5] = (((s32) sinX) * cosY) / 0x1000;
            temp = (((s32) cosX) * sinY) / 0x1000;
            localAxes[i][6] = ((((s32) temp) * cosZ) / 0x1000) + ((((s32) sinX) * sinZ) / 0x1000);
            localAxes[i][7] = ((((s32) temp) * sinZ) / 0x1000) + ((((s32) (-sinX)) * cosZ) / 0x1000);
            localAxes[i][8] = (((s32) cosX) * cosY) / 0x1000;
            if (player->stateFlags & 0x200000) {
                localAxes[i][1] = (localAxes[i][1] * player->unk2D6) / 8;
                localAxes[i][4] = (localAxes[i][4] * player->unk2D6) / 8;
                localAxes[i][7] = (localAxes[i][7] * player->unk2D6) / 8;
            }
        }

        makeFixedRotationZYX(effectMtx, player->unk6C, -player->unk6E, -player->unk70);
        makeFixedRotationZXY(playerMtx, player->unk288, player->unk28A, player->unk28C);
        offset[0] = (((s64) playerMtx[3]) * player->collisionCenterOffset) / 0x1000;
        offset[1] = (((s64) playerMtx[4]) * player->collisionCenterOffset) / 0x1000;
        offset[2] = (((s64) playerMtx[5]) * player->collisionCenterOffset) / 0x1000;
        multiplyFixedMatrix3s(effectMtx, playerMtx, baseMtx);
        for (k = 0; k < player->collisionVolumeCount; k++) {

            for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) {
                    player->collisionVolumes[k].axis[(i * 3) + j] =
                        (((localAxes[k][(i * 3)] * baseMtx[j]) + (localAxes[k][(i * 3) + 1] * baseMtx[j + 3])) +
                         (localAxes[k][(i * 3) + 2] * baseMtx[j + 6])) /
                        0x1000;
                }
            }
        }

        player->collisionVolumes[0].point[0] =
            ((((-((s64) baseMtx[0])) * player->collisionSources[0].sizeX) +
              (((s64) baseMtx[3]) * (player->collisionSources[0].sizeY - player->collisionCenterOffset))) +
             (((s64) baseMtx[6]) * player->collisionSources[0].sizeZ)) /
            0x1000;
        player->collisionVolumes[0].point[1] =
            ((((-((s64) baseMtx[1])) * player->collisionSources[0].sizeX) +
              (((s64) baseMtx[4]) * (player->collisionSources[0].sizeY - player->collisionCenterOffset))) +
             (((s64) baseMtx[7]) * player->collisionSources[0].sizeZ)) /
            0x1000;
        player->collisionVolumes[0].point[2] =
            ((((-((s64) baseMtx[2])) * player->collisionSources[0].sizeX) +
              (((s64) baseMtx[5]) * (player->collisionSources[0].sizeY - player->collisionCenterOffset))) +
             (((s64) baseMtx[8]) * player->collisionSources[0].sizeZ)) /
            0x1000;
        player->collisionVolumes[1].point[0] =
            ((((-((s64) player->collisionVolumes[0].axis[0])) * player->collisionSources[1].sizeX) +
              (((s64) player->collisionVolumes[0].axis[3]) * player->collisionSources[1].sizeY)) +
             (((s64) player->collisionVolumes[0].axis[6]) * player->collisionSources[1].sizeZ)) /
            0x1000;
        player->collisionVolumes[1].point[1] =
            ((((-((s64) player->collisionVolumes[0].axis[1])) * player->collisionSources[1].sizeX) +
              (((s64) player->collisionVolumes[0].axis[4]) * player->collisionSources[1].sizeY)) +
             (((s64) player->collisionVolumes[0].axis[7]) * player->collisionSources[1].sizeZ)) /
            0x1000;
        player->collisionVolumes[1].point[2] =
            ((((-((s64) player->collisionVolumes[0].axis[2])) * player->collisionSources[1].sizeX) +
              (((s64) player->collisionVolumes[0].axis[5]) * player->collisionSources[1].sizeY)) +
             (((s64) player->collisionVolumes[0].axis[8]) * player->collisionSources[1].sizeZ)) /
            0x1000;
    } else {
        for (i = 0; i < player->collisionVolumeCount; i++) {
            sinX = fixedSine(player->collisionSources[i].rotX);
            cosX = fixedCosine(player->collisionSources[i].rotX);
            sinY = fixedSine(player->collisionSources[i].rotY);
            cosY = fixedCosine(player->collisionSources[i].rotY);
            sinZ = fixedSine(player->collisionSources[i].rotZ);
            cosZ = fixedCosine(player->collisionSources[i].rotZ);
            localAxes[i][0] = (((s32) cosY) * cosZ) / 0x1000;
            localAxes[i][1] = (((s32) cosY) * sinZ) / 0x1000;
            localAxes[i][2] = -sinY;
            temp = (((s32) sinX) * sinY) / 0x1000;
            localAxes[i][3] = ((((s32) temp) * cosZ) / 0x1000) + ((((s32) cosX) * (-sinZ)) / 0x1000);
            localAxes[i][4] = ((((s32) temp) * sinZ) / 0x1000) + ((((s32) cosX) * cosZ) / 0x1000);
            localAxes[i][5] = (((s32) sinX) * cosY) / 0x1000;
            temp = (((s32) cosX) * sinY) / 0x1000;
            localAxes[i][6] = ((((s32) temp) * cosZ) / 0x1000) + ((((s32) sinX) * sinZ) / 0x1000);
            localAxes[i][7] = ((((s32) temp) * sinZ) / 0x1000) + ((((s32) (-sinX)) * cosZ) / 0x1000);
            localAxes[i][8] = (((s32) cosX) * cosY) / 0x1000;
            if (player->stateFlags & 0x200000) {
                localAxes[i][1] = (localAxes[i][1] * player->unk2D6) / 8;
                localAxes[i][4] = (localAxes[i][4] * player->unk2D6) / 8;
                localAxes[i][7] = (localAxes[i][7] * player->unk2D6) / 8;
            }
        }

        makeFixedRotationZYX(effectMtx, player->unk6C, player->unk6E, player->unk70);
        makeFixedRotationZXY(playerMtx, player->unk288, player->unk28A, player->unk28C);
        offset[0] = (((s64) playerMtx[3]) * player->collisionCenterOffset) / 0x1000;
        offset[1] = (((s64) playerMtx[4]) * player->collisionCenterOffset) / 0x1000;
        offset[2] = (((s64) playerMtx[5]) * player->collisionCenterOffset) / 0x1000;
        makeFixedRotationY(baseMtx, 0x800);
        multiplyFixedMatrix3s(baseMtx, playerMtx, tempMtx);
        multiplyFixedMatrix3s(effectMtx, tempMtx, baseMtx);
        for (k = 0; k < player->collisionVolumeCount; k++) {
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) {
                    player->collisionVolumes[k].axis[(i * 3) + j] =
                        (((localAxes[k][(i * 3)] * baseMtx[j]) + (localAxes[k][(i * 3) + 1] * baseMtx[j + 3])) +
                         (localAxes[k][(i * 3) + 2] * baseMtx[j + 6])) /
                        0x1000;
                }
            }
        }

        player->collisionVolumes[0].point[0] =
            (((((s64) baseMtx[0]) * player->collisionSources[0].sizeX) +
              (((s64) baseMtx[3]) * (player->collisionSources[0].sizeY - player->collisionCenterOffset))) +
             (((s64) baseMtx[6]) * player->collisionSources[0].sizeZ)) /
            0x1000;
        player->collisionVolumes[0].point[1] =
            (((((s64) baseMtx[1]) * player->collisionSources[0].sizeX) +
              (((s64) baseMtx[4]) * (player->collisionSources[0].sizeY - player->collisionCenterOffset))) +
             (((s64) baseMtx[7]) * player->collisionSources[0].sizeZ)) /
            0x1000;
        player->collisionVolumes[0].point[2] =
            (((((s64) baseMtx[2]) * player->collisionSources[0].sizeX) +
              (((s64) baseMtx[5]) * (player->collisionSources[0].sizeY - player->collisionCenterOffset))) +
             (((s64) baseMtx[8]) * player->collisionSources[0].sizeZ)) /
            0x1000;
        player->collisionVolumes[1].point[0] =
            (((((s64) player->collisionVolumes[0].axis[0]) * player->collisionSources[1].sizeX) +
              (((s64) player->collisionVolumes[0].axis[3]) * player->collisionSources[1].sizeY)) +
             (((s64) player->collisionVolumes[0].axis[6]) * player->collisionSources[1].sizeZ)) /
            0x1000;
        player->collisionVolumes[1].point[1] =
            (((((s64) player->collisionVolumes[0].axis[1]) * player->collisionSources[1].sizeX) +
              (((s64) player->collisionVolumes[0].axis[4]) * player->collisionSources[1].sizeY)) +
             (((s64) player->collisionVolumes[0].axis[7]) * player->collisionSources[1].sizeZ)) /
            0x1000;
        player->collisionVolumes[1].point[2] =
            (((((s64) player->collisionVolumes[0].axis[2]) * player->collisionSources[1].sizeX) +
              (((s64) player->collisionVolumes[0].axis[5]) * player->collisionSources[1].sizeY)) +
             (((s64) player->collisionVolumes[0].axis[8]) * player->collisionSources[1].sizeZ)) /
            0x1000;
    }
    player->collisionVolumes[0].point[0] += player->pos.x + offset[0];
    player->collisionVolumes[0].point[1] += (((player->pos.y - player->unk58) + player->unk64) + offset[1]) + 0xA000;
    player->collisionVolumes[0].point[2] += player->pos.z + offset[2];
    player->collisionVolumes[1].point[0] += player->collisionVolumes[0].point[0];
    player->collisionVolumes[1].point[1] += player->collisionVolumes[0].point[1];
    player->collisionVolumes[1].point[2] += player->collisionVolumes[0].point[2];
    for (k = 2; k < player->collisionVolumeCount; k++) {
        parentIndex = player->collisionSources[k].parentIndex;
        for (i = 0; i < 3; i++) {
            player->collisionVolumes[k].point[i] =
                (((((s64) player->collisionVolumes[parentIndex].axis[i]) * player->collisionSources[k].sizeX) +
                  (((s64) player->collisionVolumes[parentIndex].axis[i + 3]) * player->collisionSources[k].sizeY)) +
                 (((s64) player->collisionVolumes[parentIndex].axis[i + 6]) * player->collisionSources[k].sizeZ)) /
                0x1000;
            player->collisionVolumes[k].point[i] += player->collisionVolumes[parentIndex].point[i];
        }
    }
    for (i = 0; i < player->collisionVolumeCount; i++) {

        player->collisionVolumes[i].axis[0] /= 4;
        player->collisionVolumes[i].axis[1] /= 4;
        player->collisionVolumes[i].axis[2] /= 4;
        player->collisionVolumes[i].axis[3] /= 4;
        player->collisionVolumes[i].axis[4] /= 4;
        player->collisionVolumes[i].axis[5] /= 4;
        player->collisionVolumes[i].axis[6] /= 4;
        player->collisionVolumes[i].axis[7] /= 4;
        player->collisionVolumes[i].axis[8] /= 4;
    }
}
