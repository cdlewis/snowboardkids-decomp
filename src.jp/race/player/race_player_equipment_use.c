#include "game/race/items/race_items.h"
#include "common.h"
#include "PR/os_cont.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/audio/sound_manager.h"
#include "game/race/player/race_player_input.h"
#include "game/math/spatial_math.h"
#include "game/race/items/race_projectiles.h"
#include "game/race/effects/race_visual_effects.h"
#include "game/race/player/race_player_equipment_use.h"
#include "game/race/effects/snowboard_trail_effects.h"
#include "game/race/ui/race_ui_effects.h"

#define CPU_PICKUP_USE_RETRY_COOLDOWN 10
#define CPU_SHOT_TARGETED_USE_COOLDOWN (-0x3E)
#define CPU_SHOT_TARGET_AXIS_MIN (-0x5FFFFFF)
#define CPU_SHOT_TARGET_AXIS_MAX 0x6000000
#define CPU_SHOT_TARGET_FORWARD_ANGLE_MAX 0x200
#define CPU_SHOT_TARGET_WRAPPED_ANGLE_MIN 0xE01
#define RACE_PLAYER_INVISIBILITY_DURATION 180

void updateRacePlayerShotUse(RacePlayer *player) {
    s32 shouldFireShot;
    s32 deltaX;
    s32 deltaZ;
    s32 relativeTargetAngle;
    s32 targetPlayerIndex;

    shouldFireShot = FALSE;
    if (player->isCpu == 0) {
        if (player->pressedInput & Z_TRIG) {
            shouldFireShot = TRUE;
        }
    } else {
        if (player->cpuShotUseCooldown == 0) {
            player->cpuShotUseCooldown = CPU_PICKUP_USE_RETRY_COOLDOWN;
            if (player->cpuShotUseChance >= randomNextMain()) {
                shouldFireShot = TRUE;
            }
        } else {
            player->cpuShotUseCooldown--;
        }

        if ((shouldFireShot != FALSE) && (player->shotCount != 0)) {
            shouldFireShot = FALSE;
            for (targetPlayerIndex = 0; targetPlayerIndex < RACE_PLAYER_COUNT; targetPlayerIndex++) {
                if (gRacePlayers[targetPlayerIndex].isCpu == 0) {
                    deltaX = gRacePlayers[targetPlayerIndex].pos.x - player->pos.x;
                    deltaZ = gRacePlayers[targetPlayerIndex].pos.z - player->pos.z;
                    if ((deltaX < CPU_SHOT_TARGET_AXIS_MAX) && (deltaX >= CPU_SHOT_TARGET_AXIS_MIN) &&
                        (deltaZ < CPU_SHOT_TARGET_AXIS_MAX) && (deltaZ >= CPU_SHOT_TARGET_AXIS_MIN)) {
                        relativeTargetAngle = (s16)((calculateFixedAngleFromDeltaXZ(deltaX, deltaZ) -
                                                     (s16)(player->facingAngle & 0xFFFFu)) &
                                                    0xFFF);
                        if ((relativeTargetAngle >= CPU_SHOT_TARGET_WRAPPED_ANGLE_MIN) ||
                            (relativeTargetAngle < CPU_SHOT_TARGET_FORWARD_ANGLE_MAX)) {
                            shouldFireShot = TRUE;
                            player->cpuShotUseCooldown = CPU_SHOT_TARGETED_USE_COOLDOWN;
                        }
                    }
                }
            }
        }
    }

    if (shouldFireShot != FALSE) {
        if (player->shotCount != 0) {
            if (player->shotType == RACE_SHOT_SLAPSTICK) {
                if (createCallbackTaskWithUserIdPreservingArgs(
                        initWideHomingShotProjectile,
                        (u16)(player->playerIndex + 1),
                        0x3C,
                        player->playerIndex
                    ) != NULL) {
                    createCallbackTaskWithUserIdPreservingArgs(initRacePlayerShockEffect, 5, 4, player->playerIndex);
                    player->shotCount--;
                }
            }
            if (player->shotType == RACE_SHOT_PARACHUTE) {
                if (createCallbackTaskWithUserIdPreservingArgs(
                        initLongRangeHomingShotProjectile,
                        (u16)(player->playerIndex + 1),
                        0x3C,
                        player->playerIndex
                    ) != NULL) {
                    createCallbackTaskWithUserIdPreservingArgs(initRacePlayerShockEffect, 5, 4, player->playerIndex);
                    player->shotCount--;
                }
            }
            if (player->shotType == RACE_SHOT_FREEZE_SHOT) {
                if (createCallbackTaskWithUserIdPreservingArgs(
                        initCloseRangeHomingShotProjectile,
                        (u16)(player->playerIndex + 1),
                        0x3C,
                        player->playerIndex
                    ) != NULL) {
                    createCallbackTaskWithUserIdPreservingArgs(initRacePlayerShockEffect, 5, 4, player->playerIndex);
                    player->shotCount--;
                }
            }
            if (player->shotType == RACE_SHOT_SNOWMAN) {
                if (createCallbackTaskWithUserIdPreservingArgs(
                        initBouncingShotProjectile,
                        (u16)(player->playerIndex + 1),
                        0x3C,
                        player->playerIndex
                    ) != NULL) {
                    createCallbackTaskWithUserIdPreservingArgs(initRacePlayerShockEffect, 5, 4, player->playerIndex);
                    player->shotCount--;
                }
            }
            if (player->shotType == RACE_SHOT_BOMB) {
                if (createCallbackTaskWithUserIdPreservingArgs(
                        initAreaBlastShotProjectile,
                        (u16)(player->playerIndex + 1),
                        0x3C,
                        player->playerIndex
                    ) != NULL) {
                    createCallbackTaskWithUserIdPreservingArgs(initRacePlayerShockEffect, 5, 4, player->playerIndex);
                    player->shotCount--;
                }
            }
            if (player->shotCount == 0) {
                player->shotType = RACE_SHOT_NONE;
            }
        }

        if (player->shieldShotCount != 0) {
            if (createCallbackTaskWithUserIdPreservingArgs(
                    initShieldProjectile,
                    (u16)(player->playerIndex + 1),
                    0x3C,
                    player->playerIndex
                ) != NULL) {
                createCallbackTaskWithUserIdPreservingArgs(initRacePlayerShockEffect, 5, 4, player->playerIndex);
                player->shieldShotCount--;
            }
        }
    }
}

void updateRacePlayerItemUse(RacePlayer *player) {
    s32 shouldUseItem;
    s32 itemType;

    shouldUseItem = FALSE;
    if (player->isCpu == 0) {
        if (player->pressedInput & B_BUTTON) {
            shouldUseItem = TRUE;
        }
    } else {
        if (player->cpuItemUseCooldown == 0) {
            if (player->cpuItemUseChance >= randomNextMain()) {
                shouldUseItem = TRUE;
            }
            player->cpuItemUseCooldown = CPU_PICKUP_USE_RETRY_COOLDOWN;
        } else {
            player->cpuItemUseCooldown--;
        }

        if (player->itemType == RACE_ITEM_INVISIBLE) {
            /* CPUs use invisibility reactively when targeted by an attack. */
            shouldUseItem = FALSE;
            if (player->isTargetedByAttack != 0) {
                shouldUseItem = TRUE;
            } else {
                shouldUseItem = FALSE;
            }
        }
    }

    if (shouldUseItem != FALSE) {
        itemType = player->itemType;
        if (itemType != RACE_ITEM_NONE) {
            if ((itemType == RACE_ITEM_SPEED_FAN) && (player->trailEffectTimer == 0)) {
                startSpeedFanEffect(player);
                player->itemType = RACE_ITEM_NONE;
                itemType = player->itemType;
            }

            if (itemType == RACE_ITEM_GHOST) {
                spawnGhostItemEffect(player->playerIndex);
                player->itemType = RACE_ITEM_NONE;
                itemType = player->itemType;
            }

            if (itemType == RACE_ITEM_PAN) {
                spawnPanItemTargets(player->playerIndex);
                player->itemType = RACE_ITEM_NONE;
                itemType = player->itemType;
            }

            if (itemType == RACE_ITEM_ROCK) {
                createCallbackTaskWithUserIdPreservingArgs(initRockItem, 0, 0x3C, (u16)player->playerIndex);
                player->itemType = RACE_ITEM_NONE;
                itemType = player->itemType;
            }

            if (itemType == RACE_ITEM_RAT_FACE) {
                if (player->isCpu == 0) {
                    createCallbackTaskWithUserIdPreservingArgs(initRatFaceItemEffect, 0, 3, (u16)player->playerIndex);
                }
                player->itemType = RACE_ITEM_NONE;
                itemType = player->itemType;
            }

            if ((itemType == RACE_ITEM_INVISIBLE) && (player->invisibilityTimer == 0)) {
                player->invisibilityTimer = RACE_PLAYER_INVISIBILITY_DURATION;
                player->itemType = RACE_ITEM_NONE;
                enqueuePositionalSoundEffect(SFX_ITEM_USE, &player->pos, 0x7F, SFX_PRIORITY_NORMAL);
            }
        }
    }
}
