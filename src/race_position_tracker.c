#include "common.h"

// Race player records are 0x60C bytes apart. This view only names the fields
// touched by this placement/progress tracking module.
#define RACE_POSITION_PLAYER_COUNT 4
#define RACE_POSITION_PLAYER_SIZE 0x60C

typedef struct {
    /* 0x00 */ s16 pathFrame;
    /* 0x02 */ s16 eventId;
} RacePositionCheckpointEvent;

typedef struct {
    /* 0x000 */ s16 playerIndex;
    /* 0x002 */ u8 pad2[2];
    /* 0x004 */ u8 isActive;
    /* 0x005 */ u8 pad5[0x1C - 0x5];
    /* 0x01C */ s32 posX;
    /* 0x020 */ s32 posY;
    /* 0x024 */ s32 posZ;
    /* 0x028 */ u8 pad28[0x2FC - 0x28];
    /* 0x2FC */ s32 flags;
    /* 0x300 */ u8 pad300[0x502 - 0x300];
    /* 0x502 */ s16 pathFrame;
    /* 0x504 */ u8 pad504[0x509 - 0x504];
    /* 0x509 */ s8 raceRank;
    /* 0x50A */ u8 pad50A[0x51C - 0x50A];
    /* 0x51C */ u32 checkpointEventMask;
    /* 0x520 */ s32 smoothedPathOffset;
    /* 0x524 */ u8 checkpointHit;
    /* 0x525 */ u8 pad525[2];
    /* 0x527 */ s8 checkpointEventId;
    /* 0x528 */ u8 pad528;
    /* 0x529 */ u8 displayRank;
    /* 0x52A */ u8 pad52A;
    /* 0x52B */ u8 rankChangeTimer;
    /* 0x52C */ u8 pad52C[RACE_POSITION_PLAYER_SIZE - 0x52C];
} RacePositionPlayer;

extern RacePositionPlayer D_80121D80[RACE_POSITION_PLAYER_COUNT];
extern RacePositionCheckpointEvent *D_800DE030[];
extern s8 *D_800DDE74[];
extern s16 D_80121B50;

#pragma GLOBAL_ASM("asm/nonmatchings/race_position_tracker/func_8007B250.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_position_tracker/func_8007BB08.s")

// func_8007BCFC best match: 99.474% (nonmatchings/func_8007BCFC-1404502880690620360/base_14.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_position_tracker/func_8007BCFC.s")

#ifdef NON_MATCHING
s32 func_8007BCFC(s32 playerIndex, s32 pathIndex, s32 rankSlot) {
    s32 rankSlotCopy;
    s32 courseIndex;
    s32 pathIndexCopy;
    s8 *entry;
    RacePositionPlayer *player;
    s8 pathSample;
    s32 pathOffset;

    courseIndex = D_80121B50;
    entry = D_800DDE74[(courseIndex * RACE_POSITION_PLAYER_COUNT) + playerIndex];
    pathIndexCopy = pathIndex;
    if (courseIndex == 7) {
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

    rankSlotCopy = rankSlot;
    entry = D_800DDE74[(courseIndex * RACE_POSITION_PLAYER_COUNT) + playerIndex];
    player = &D_80121D80[rankSlotCopy];
    pathOffset = player->smoothedPathOffset;
    pathSample = entry[pathIndexCopy];
    pathIndex = (pathSample << 0x12) - pathOffset;

    if (pathIndex >= 0x60001) {
        pathIndex = 0x60000;
    }
    if (pathIndex < -0x60000) {
        pathIndex = -0x60000;
    }

    pathOffset += pathIndex;
    player->smoothedPathOffset = pathOffset;
    return pathOffset;
}
#endif

s32 func_8007BDE4(s32 playerIndex, s32 pathIndex) {
    s8 *entry;

    if (D_80121B50 == 7) {
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

    entry = D_800DDE74[(D_80121B50 * RACE_POSITION_PLAYER_COUNT) + playerIndex];
    return entry[pathIndex] << 0x12;
}
