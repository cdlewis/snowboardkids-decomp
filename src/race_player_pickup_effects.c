#include "common.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "race_player_input.h"
#include "spatial_math.h"
#include "race_item_projectiles.h"
#include "race_item_effects.h"
#include "race_player_pickup_effects.h"
#include "snowboard_trail_effects.h"
#include "race_ui_effects.h"

// updateRacePlayerItemEffectUse best match: 98.537% (nonmatchings/func_800849E0-731940616440357983/base_13.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_player_pickup_effects/updateRacePlayerItemEffectUse.s")

typedef void (*EffectCallback)(void *);

extern void enqueuePositionalSoundEffect(s32 soundId, void *pos, s32 volume, s32 distance);

extern void *createCallbackTaskWithUserIdPreservingArgs(void *, s32, s32, s32);
extern s32 gFrameCounter;

#define gRaceInputPlayers D_80121D80
#define gRaceInputPlayersEnd ((RaceInputPlayer *)&gFrameCounter)

#ifdef NON_MATCHING
void updateRacePlayerItemEffectUse(RaceInputPlayer *player) {
    struct TriggerSlot { s32 pad0; s32 pad1; s32 pad2; s32 trigger; } triggerSlot;
    volatile s32 dummy;
#define trigger triggerSlot.trigger
    RaceInputPlayer *otherPlayer;
    s32 deltaX;
    s32 deltaZ;
    s32 angle;

    trigger = 0;
    if (player->unk4 == 0) {
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
            otherPlayer = gRaceInputPlayers;
            do {
                if (otherPlayer->unk4 == 0) {
                    deltaX = otherPlayer->posX - player->posX;
                    deltaZ = otherPlayer->posZ - player->posZ;
                    if ((deltaX < 0x6000000) && (deltaX >= -0x5FFFFFF) &&
                        (deltaZ < 0x6000000) && (deltaZ >= -0x5FFFFFF)) {
                        angle = (s16) ((calculateFixedAngleFromDeltaXZ(deltaX, deltaZ) - player->facingAngle) & 0xFFF);
                        if ((angle >= 0xE01) || (angle < 0x200)) {
                            trigger = 1;
                            player->itemTriggerCooldown = -0x3E;
                        }
                    }
                }
                otherPlayer++;
            } while (otherPlayer != gRaceInputPlayersEnd);
        }
    }

    if (trigger != 0) {
        if (player->itemEffectCount != 0) {
            if (player->itemEffectType == 1) {
                if (createCallbackTaskWithUserIdPreservingArgs(initWideHomingItemProjectile, (u16) (player->playerIndexU + 1), 0x3C, player->playerIndexU) != NULL) {
                    createCallbackTaskWithUserIdPreservingArgs(initRacePlayerShockEffect, 5, 4, player->playerIndexU);
                    player->itemEffectCount--;
                }
            }
            if (player->itemEffectType == 2) {
                if (createCallbackTaskWithUserIdPreservingArgs(initLongRangeHomingItemProjectile, (u16) (player->playerIndexU + 1), 0x3C, player->playerIndexU) != NULL) {
                    createCallbackTaskWithUserIdPreservingArgs(initRacePlayerShockEffect, 5, 4, player->playerIndexU);
                    player->itemEffectCount--;
                }
            }
            if (player->itemEffectType == 3) {
                if (createCallbackTaskWithUserIdPreservingArgs(initCloseRangeHomingItemProjectile, (u16) (player->playerIndexU + 1), 0x3C, player->playerIndexU) != NULL) {
                    createCallbackTaskWithUserIdPreservingArgs(initRacePlayerShockEffect, 5, 4, player->playerIndexU);
                    player->itemEffectCount--;
                }
            }
            if (player->itemEffectType == 4) {
                if (createCallbackTaskWithUserIdPreservingArgs(initBouncingItemProjectile, (u16) (player->playerIndexU + 1), 0x3C, player->playerIndexU) != NULL) {
                    createCallbackTaskWithUserIdPreservingArgs(initRacePlayerShockEffect, 5, 4, player->playerIndexU);
                    player->itemEffectCount--;
                }
            }
            if (player->itemEffectType == 5) {
                if (createCallbackTaskWithUserIdPreservingArgs(initAreaBlastItemProjectile, (u16) (player->playerIndexU + 1), 0x3C, player->playerIndexU) != NULL) {
                    createCallbackTaskWithUserIdPreservingArgs(initRacePlayerShockEffect, 5, 4, player->playerIndexU);
                    player->itemEffectCount--;
                }
            }
            if (player->itemEffectCount == 0) {
                player->itemEffectType = 0;
            }
        }

        if (player->shieldEffectTimer != 0) {
            if (createCallbackTaskWithUserIdPreservingArgs(initShieldProjectile, (u16) (player->playerIndexU + 1), 0x3C, player->playerIndexU) != NULL) {
                createCallbackTaskWithUserIdPreservingArgs(initRacePlayerShockEffect, 5, 4, player->playerIndexU);
                player->shieldEffectTimer--;
            }
        }
    }
#undef trigger
}
#endif

void updateRacePlayerActionEffectUse(RaceInputPlayer *player) {
    s32 trigger;
    s32 type;

    trigger = 0;
    if (player->unk4 == 0) {
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
            if (player->actionEffectEnabled != 0) {
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
                createCallbackTaskWithUserIdPreservingArgs(initFallingActionProjectile, 0, 0x3C, (u16) player->playerIndex);
                player->actionEffectType = 0;
                type = player->actionEffectType;
            }

            if (type == 5) {
                if (player->unk4 == 0) {
                    createCallbackTaskWithUserIdPreservingArgs(initForwardActionProjectileEffect, 0, 3, (u16) player->playerIndex);
                }
                player->actionEffectType = 0;
                type = player->actionEffectType;
            }

            if ((type == 6) && (player->actionSoundTimer == 0)) {
                player->actionSoundTimer = 0xB4;
                player->actionEffectType = 0;
                enqueuePositionalSoundEffect(0x10, &player->posX, 0x7F, 0x32);
            }
        }
    }
}
