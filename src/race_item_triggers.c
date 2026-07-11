#include "common.h"
#include "effect_task_scheduler.h"
#include "asset_manager.h"
#include "race_input_history.h"
#include "fixed_point_math.h"
#include "race_effects.h"
#include "race_item_effects.h"
#include "race_item_triggers.h"
#include "snowboard_trail_effects.h"
#include "race_ui_effects.h"

// updateItemUseTrigger best match: 98.537% (nonmatchings/func_800849E0-731940616440357983/base_13.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_item_triggers/updateItemUseTrigger.s")

typedef void (*EffectCallback)(void *);

extern void func_80072A74(s32 soundId, void *pos, s32 volume, s32 distance);

extern void *func_800716A4(void *, s32, s32, s32);
extern RaceInputPlayer gFrameCounter;

#ifdef NON_MATCHING
void updateItemUseTrigger(RaceInputPlayer *player) {
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
            otherPlayer = D_80121D80;
            do {
                if (otherPlayer->unk4 == 0) {
                    deltaX = otherPlayer->posX - player->posX;
                    deltaZ = otherPlayer->posZ - player->posZ;
                    if ((deltaX < 0x6000000) && (deltaX >= -0x5FFFFFF) &&
                        (deltaZ < 0x6000000) && (deltaZ >= -0x5FFFFFF)) {
                        angle = (s16) ((func_8004908C(deltaX, deltaZ) - player->facingAngle) & 0xFFF);
                        if ((angle >= 0xE01) || (angle < 0x200)) {
                            trigger = 1;
                            player->itemTriggerCooldown = -0x3E;
                        }
                    }
                }
                otherPlayer++;
            } while (otherPlayer != &gFrameCounter);
        }
    }

    if (trigger != 0) {
        if (player->itemEffectCount != 0) {
            if (player->itemEffectType == 1) {
                if (func_800716A4(func_80049CE0, (u16) (player->playerIndexU + 1), 0x3C, player->playerIndexU) != NULL) {
                    func_800716A4(func_8004EF24, 5, 4, player->playerIndexU);
                    player->itemEffectCount--;
                }
            }
            if (player->itemEffectType == 2) {
                if (func_800716A4(func_8004C8F0, (u16) (player->playerIndexU + 1), 0x3C, player->playerIndexU) != NULL) {
                    func_800716A4(func_8004EF24, 5, 4, player->playerIndexU);
                    player->itemEffectCount--;
                }
            }
            if (player->itemEffectType == 3) {
                if (func_800716A4(func_8004A648, (u16) (player->playerIndexU + 1), 0x3C, player->playerIndexU) != NULL) {
                    func_800716A4(func_8004EF24, 5, 4, player->playerIndexU);
                    player->itemEffectCount--;
                }
            }
            if (player->itemEffectType == 4) {
                if (func_800716A4(func_8004AFE4, (u16) (player->playerIndexU + 1), 0x3C, player->playerIndexU) != NULL) {
                    func_800716A4(func_8004EF24, 5, 4, player->playerIndexU);
                    player->itemEffectCount--;
                }
            }
            if (player->itemEffectType == 5) {
                if (func_800716A4(func_8004BFA0, (u16) (player->playerIndexU + 1), 0x3C, player->playerIndexU) != NULL) {
                    func_800716A4(func_8004EF24, 5, 4, player->playerIndexU);
                    player->itemEffectCount--;
                }
            }
            if (player->itemEffectCount == 0) {
                player->itemEffectType = 0;
            }
        }

        if (player->shieldEffectTimer != 0) {
            if (func_800716A4(func_8004D880, (u16) (player->playerIndexU + 1), 0x3C, player->playerIndexU) != NULL) {
                func_800716A4(func_8004EF24, 5, 4, player->playerIndexU);
                player->shieldEffectTimer--;
            }
        }
    }
#undef trigger
}
#endif

void updateActionUseTrigger(RaceInputPlayer *player) {
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
                func_80083CFC(player);
                player->actionEffectType = 0;
                type = player->actionEffectType;
            }

            if (type == 2) {
                func_8005F298(player->playerIndex);
                player->actionEffectType = 0;
                type = player->actionEffectType;
            }

            if (type == 3) {
                spawnGhostSlowdownTargets(player->playerIndex);
                player->actionEffectType = 0;
                type = player->actionEffectType;
            }

            if (type == 4) {
                func_800716A4(func_8004D184, 0, 0x3C, (u16) player->playerIndex);
                player->actionEffectType = 0;
                type = player->actionEffectType;
            }

            if (type == 5) {
                if (player->unk4 == 0) {
                    func_800716A4(func_80064EAC, 0, 3, (u16) player->playerIndex);
                }
                player->actionEffectType = 0;
                type = player->actionEffectType;
            }

            if ((type == 6) && (player->actionSoundTimer == 0)) {
                player->actionSoundTimer = 0xB4;
                player->actionEffectType = 0;
                func_80072A74(0x10, &player->posX, 0x7F, 0x32);
            }
        }
    }
}
