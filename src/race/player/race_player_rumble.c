#include "game/race/player/race_player_rumble.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"

#define RUMBLE_PATTERN_WRAP_MASK 0xF
#define RUMBLE_PATTERN_FAST 2
#define RUMBLE_PATTERN_FAST_LENGTH 6

typedef u8 ControllerRumblePattern[];

ControllerRumblePattern gRacePlayerRumblePatternSolid = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
};

ControllerRumblePattern gRacePlayerRumblePatternSlowPulse = {
    1, 0, 0, 0, 1, 0, 0, 0,
    1, 0, 0, 0, 1, 0, 0, 0,
};

ControllerRumblePattern gRacePlayerRumblePatternFastPulse = {
    1, 1, 0, 0, 0, 0, 0, 0,
};

ControllerRumblePattern gRacePlayerRumblePatternAlternatingPulse = {
    1, 0, 1, 0, 1, 0, 1, 0,
    1, 0, 1, 0, 1, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

void updateRacePlayerRumble(RacePlayer *player) {
    s32 patternId;
    s32 fastPatternId;
    u8 *pattern;

    if (player->unk4 != 0) {
        return;
    }
    if (player->soundDisabled != 0) {
        return;
    }
    if (player->rumbleTimer == 0) {
        player->rumblePatternIndex = 0;
        return;
    }
    player->rumbleTimer = player->rumbleTimer - 1;
    patternId = player->rumblePatternId;
    fastPatternId = RUMBLE_PATTERN_FAST;
    pattern = gRacePlayerRumblePatternSolid;
    if (patternId == 1) {
        pattern = gRacePlayerRumblePatternSlowPulse;
    }
    if (fastPatternId == patternId) {
        pattern = gRacePlayerRumblePatternFastPulse;
    }
    if (patternId == 3) {
        pattern = gRacePlayerRumblePatternAlternatingPulse;
    }
    if (fastPatternId == patternId) {
        if (!(player->rumblePatternIndex < RUMBLE_PATTERN_FAST_LENGTH)) {
            player->rumblePatternIndex = 0;
        }
    }
    if (pattern[player->rumblePatternIndex] != 0) {
        requestRumbleMotorStart(player->playerIndex);
    }
    player->rumblePatternIndex = (player->rumblePatternIndex + 1) & RUMBLE_PATTERN_WRAP_MASK;
}
