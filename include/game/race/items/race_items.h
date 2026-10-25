#ifndef RACE_ITEMS_H
#define RACE_ITEMS_H

/* The instruction booklet (Equipment, p. 22) calls red-shop pickups "Shots"
 * (Z button) and blue-shop pickups "Items" (B button). Players may also know
 * these as red items and blue items; shot/item names below distinguish the
 * two inventory slots. The asset manifests call these families projectile/action.
 * Shots and items have separate, overlapping runtime ID ranges.
 * These are not course surface/effect types returned by getRaceItemEffectType,
 * HUD tile indices, or PLAYER_HITFLAG_* bit masks.
 */

/* RacePlayer.state.shotType and gShotRollTable. */
#define RACE_SHOT_NONE 0
#define RACE_SHOT_SLAPSTICK 1
#define RACE_SHOT_PARACHUTE 2
#define RACE_SHOT_FREEZE_SHOT 3
#define RACE_SHOT_SNOWMAN 4
#define RACE_SHOT_BOMB 5

/* RacePlayer.state.itemType and gItemRollTable. */
#define RACE_ITEM_NONE 0
#define RACE_ITEM_SPEED_FAN 1
#define RACE_ITEM_GHOST 2
#define RACE_ITEM_PAN 3
#define RACE_ITEM_ROCK 4
#define RACE_ITEM_RAT_FACE 5
#define RACE_ITEM_INVISIBLE 6

#endif
