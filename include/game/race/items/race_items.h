#ifndef RACE_ITEMS_H
#define RACE_ITEMS_H

/* Runtime IDs documented by the item asset manifests.
 * Projectile and action items have separate, overlapping ID ranges.
 * These are not course surface/effect types returned by getRaceItemEffectType,
 * HUD tile indices, or PLAYER_HITFLAG_* bit masks.
 */

/* RacePlayer.itemEffectType and gItemEffectRollTable. */
#define RACE_ITEM_NONE 0
#define RACE_ITEM_SLAPSTICK 1
#define RACE_ITEM_PARACHUTE 2
#define RACE_ITEM_FREEZE_SHOT 3
#define RACE_ITEM_SNOWMAN 4
#define RACE_ITEM_BOMB 5

/* RacePlayer.actionEffectType and gActionEffectRollTable. */
#define RACE_ACTION_NONE 0
#define RACE_ACTION_SPEED_FAN 1
#define RACE_ACTION_GHOST 2
#define RACE_ACTION_PAN 3
#define RACE_ACTION_ROCK 4
#define RACE_ACTION_RAT_FACE 5
#define RACE_ACTION_INVISIBLE 6

#endif
