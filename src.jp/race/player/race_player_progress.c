#include "common.h"
#include "game/race/race_state.h"
#include "game/race/player/race_player_progress.h"

extern s8 *D_800DD1A4[];

#pragma GLOBAL_ASM("asm.jp/nonmatchings/race/player/race_player_progress/updateRacePlayerRankDisplay.s")

#pragma GLOBAL_ASM("asm.jp/nonmatchings/race/player/race_player_progress/func_8007B46C.s")

s32 updateRacePlayerSmoothedPathOffset(s32 playerIndex, s32 pathIndex, s32 rankSlot) {
    s32 courseIndex;
    s32 pathIndexCopy;
    s8 *entry;

    courseIndex = gRaceCourseIndex.signedValue;
    entry = D_800DD1A4[(courseIndex * RACE_PLAYER_COUNT) + playerIndex];
    pathIndexCopy = pathIndex;
    if (courseIndex == RACE_COURSE_ANIMAL_LAND) {
        if (playerIndex == 0) {
            return 0xFFF40000;
        }
        if (playerIndex == 1) {
            return 0xC0000;
        }
        if (playerIndex == 2) {
            return 0xFFDC0000;
        }
        if (playerIndex == 3) {
            return 0x240000;
        }
    }

    entry = D_800DD1A4[(courseIndex * RACE_PLAYER_COUNT) + playerIndex];
    pathIndex = entry[pathIndexCopy] << 0x12;
    pathIndex -= gRacePlayers[rankSlot].smoothedPathOffset;

    if (pathIndex > 0x60000) {
        pathIndex = 0x60000;
    }
    if (pathIndex < -0x60000) {
        pathIndex = -0x60000;
    }

    gRacePlayers[rankSlot].smoothedPathOffset += pathIndex;
    return gRacePlayers[rankSlot].smoothedPathOffset;
}

s32 getRacePlayerPathOffset(s32 playerIndex, s32 pathIndex) {
    s8 *entry;

    if (gRaceCourseIndex.signedValue == RACE_COURSE_ANIMAL_LAND) {
        if (playerIndex == 0) {
            return 0xFFF40000;
        }
        if (playerIndex == 1) {
            return 0xC0000;
        }
        if (playerIndex == 2) {
            return 0xFFDC0000;
        }
        if (playerIndex == 3) {
            return 0x240000;
        }
    }

    entry = D_800DD1A4[(gRaceCourseIndex.signedValue * RACE_PLAYER_COUNT) + playerIndex];
    return entry[pathIndex] << 0x12;
}
