#include "common.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/race/player/race_player_input.h"
#include "game/math/spatial_math.h"
#include "game/race/items/race_item_projectiles.h"
#include "game/race/items/race_item_effects.h"
#include "game/race/player/race_player_pickup_effects.h"
#include "game/race/effects/snowboard_trail_effects.h"
#include "game/race/ui/race_ui_effects.h"

typedef void (*EffectCallback)(void *);

extern void enqueuePositionalSoundEffect(s32 soundId, void *pos, s32 volume, s32 distance);

extern void *createCallbackTaskWithUserIdPreservingArgs(void *, s32, s32, s32);

void updateRacePlayerItemEffectUse(RacePlayer *player) {
    s32 trigger;
    s32 deltaX;
    s32 deltaZ;
    s32 angle;
    register s32 i;

    trigger = 0;
    if (player->isCpu == 0) {
        if (player->currentInputFlags & 0x2000) {
            trigger = 1;
        }
    } else {
        if (player->itemTriggerCooldown == 0) {
            player->itemTriggerCooldown = 10;
            if (player->itemTriggerChance >= randomNextMain()) {
                trigger = 1;
            }
        } else {
            player->itemTriggerCooldown--;
        }

        if ((trigger != 0) && (player->itemEffectCount != 0)) {
            trigger = 0;
            for (i = 0; i < RACE_PLAYER_COUNT; i++) {
                if (gRacePlayers[i].isCpu == 0) {
                    deltaX = gRacePlayers[i].pos.x - player->pos.x;
                    deltaZ = gRacePlayers[i].pos.z - player->pos.z;
                    if ((deltaX < 0x6000000) && (deltaX >= -0x5FFFFFF) && (deltaZ < 0x6000000) &&
                        (deltaZ >= -0x5FFFFFF)) {
                        angle = (s16)((calculateFixedAngleFromDeltaXZ(deltaX, deltaZ) -
                                       (s16)(player->facingAngle & 0xFFFFu)) &
                                      0xFFF);
                        if ((angle >= 0xE01) ||
                            (angle <
                             (((((((((((0x200 & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) &
                                 0xFFFF) &
                                0xFFFF) &
                               0xFFFF) &
                              0xFFFF))) {
                            trigger = 1;
                            player->itemTriggerCooldown = -0x3E;
                        }
                    }
                }
            }
        }
    }

    if (trigger != 0) {
        if (player->itemEffectCount != 0) {
            if (player->itemEffectType == 1) {
                if (createCallbackTaskWithUserIdPreservingArgs(
                        initWideHomingItemProjectile,
                        (u16)(player->playerIndex + 1),
                        0x3C,
                        player->playerIndex
                    ) != NULL) {
                    createCallbackTaskWithUserIdPreservingArgs(initRacePlayerShockEffect, 5, 4, player->playerIndex);
                    player->itemEffectCount--;
                }
            }
            if (player->itemEffectType == 2) {
                if (createCallbackTaskWithUserIdPreservingArgs(
                        initLongRangeHomingItemProjectile,
                        (u16)(player->playerIndex + 1),
                        0x3C,
                        player->playerIndex
                    ) != NULL) {
                    createCallbackTaskWithUserIdPreservingArgs(initRacePlayerShockEffect, 5, 4, player->playerIndex);
                    player->itemEffectCount--;
                }
            }
            if (player->itemEffectType == 3) {
                if (createCallbackTaskWithUserIdPreservingArgs(
                        initCloseRangeHomingItemProjectile,
                        (u16)(player->playerIndex + 1),
                        0x3C,
                        player->playerIndex
                    ) != NULL) {
                    createCallbackTaskWithUserIdPreservingArgs(initRacePlayerShockEffect, 5, 4, player->playerIndex);
                    player->itemEffectCount--;
                }
            }
            if (player->itemEffectType == 4) {
                if (createCallbackTaskWithUserIdPreservingArgs(
                        initBouncingItemProjectile,
                        (u16)(player->playerIndex + 1),
                        0x3C,
                        player->playerIndex
                    ) != NULL) {
                    createCallbackTaskWithUserIdPreservingArgs(initRacePlayerShockEffect, 5, 4, player->playerIndex);
                    player->itemEffectCount--;
                }
            }
            if (player->itemEffectType == 5) {
                if (createCallbackTaskWithUserIdPreservingArgs(
                        initAreaBlastItemProjectile,
                        (u16)(player->playerIndex + 1),
                        0x3C,
                        player->playerIndex
                    ) != NULL) {
                    createCallbackTaskWithUserIdPreservingArgs(initRacePlayerShockEffect, 5, 4, player->playerIndex);
                    player->itemEffectCount--;
                }
            }
            if (player->itemEffectCount == 0) {
                player->itemEffectType = 0;
            }
        }

        if (player->shieldEffectTimer != 0) {
            if (createCallbackTaskWithUserIdPreservingArgs(
                    initShieldProjectile,
                    (u16)(player->playerIndex + 1),
                    0x3C,
                    player->playerIndex
                ) != NULL) {
                createCallbackTaskWithUserIdPreservingArgs(initRacePlayerShockEffect, 5, 4, player->playerIndex);
                player->shieldEffectTimer--;
            }
        }
    }
}

void updateRacePlayerActionEffectUse(RacePlayer *player) {
    s32 trigger;
    s32 type;

    trigger = 0;
    if (player->isCpu == 0) {
        if (player->currentInputFlags & 0x4000) {
            trigger = 1;
        }
    } else {
        if (player->actionTriggerCooldown == 0) {
            if (player->actionTriggerChance >= randomNextMain()) {
                trigger = 1;
            }
            player->actionTriggerCooldown = 10;
        } else {
            player->actionTriggerCooldown--;
        }

        if (player->actionEffectType == 6) {
            trigger = 0;
            if (player->itemTargetFlag != 0) {
                trigger = 1;
            } else {
                trigger = 0;
            }
        }
    }

    if (trigger != 0) {
        type = player->actionEffectType;
        if (type != 0) {
            if ((type == 1) && (player->trailEffectTimer == 0)) {
                startSnowboardTrailEffect(player);
                player->actionEffectType = 0;
                type = player->actionEffectType;
            }

            if (type == 2) {
                spawnRacePlayerSparkleEffect(player->playerIndex);
                player->actionEffectType = 0;
                type = player->actionEffectType;
            }

            if (type == 3) {
                spawnGhostSlowdownTargets(player->playerIndex);
                player->actionEffectType = 0;
                type = player->actionEffectType;
            }

            if (type == 4) {
                createCallbackTaskWithUserIdPreservingArgs(
                    initFallingActionProjectile,
                    0,
                    0x3C,
                    (u16)player->playerIndex
                );
                player->actionEffectType = 0;
                type = player->actionEffectType;
            }

            if (type == 5) {
                if (player->isCpu == 0) {
                    createCallbackTaskWithUserIdPreservingArgs(
                        initForwardActionProjectileEffect,
                        0,
                        3,
                        (u16)player->playerIndex
                    );
                }
                player->actionEffectType = 0;
                type = player->actionEffectType;
            }

            if ((type == 6) && (player->actionSoundTimer == 0)) {
                player->actionSoundTimer = 0xB4;
                player->actionEffectType = 0;
                enqueuePositionalSoundEffect(0x10, &player->pos.x, 0x7F, 0x32);
            }
        }
    }
}
