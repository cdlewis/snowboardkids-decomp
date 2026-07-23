#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/race/player/race_player_rumble.h"
#include "game/audio/sound_manager.h"
#include "game/race/motion/race_motion.h"
#include "game/race/camera/race_camera.h"
#include "game/race/course/race_course_effects.h"
#include "game/race/player/race_player_collision.h"
#define calculateFixedAngleBetweenXZPoints calculateFixedAngleBetweenXZPoints_s32
#include "game/math/fixed_point_math.h"
#undef calculateFixedAngleBetweenXZPoints
#include "game/race/player/race_player_input.h"
#include "game/race/items/race_item_effects.h"
#include "game/race/player/race_player_pickup_effects.h"
#include "game/menu/renderer/menu_screen_effects.h"
#include "game/race/player/race_player_movement.h"
#include "game/race/player/race_player_progress.h"
#include "game/race/player/race_player_model_renderer.h"
#include "game/race/player/race_player_update.h"
#include "game/race/course/race_surface_cues.h"
#include "game/race/ui/race_ui_effects.h"
#include "game/race/effects/snowboard_trail_effects.h"
#include "game/engine/viewport_manager.h"

#define RACE_MOTION_STATE(player) ((RaceMotionState *)(player))
#define RACE_MOTION_INIT_STATE(player) ((RaceMotionInitState *)(player))
#define setRaceMotionAnimation(player, animIndex) setRaceMotionAnimation(RACE_MOTION_STATE(player), (animIndex))
#define interpolateRaceMotionJointAnimationFrame(player, animIndex, frameTimer, frameTimerReset) \
    interpolateRaceMotionJointAnimationFrame(RACE_MOTION_STATE(player), (animIndex), (frameTimer), (frameTimerReset))
#define blendRaceMotionJointAnimation(player, animIndex, timer, duration) \
    blendRaceMotionJointAnimation(RACE_MOTION_STATE(player), (animIndex), (timer), (duration))
#define stepRaceMotionLoopingAnimation(player) stepRaceMotionLoopingAnimation(RACE_MOTION_STATE(player))
#define stepRaceMotionLoopingJointAnimation(player) stepRaceMotionLoopingJointAnimation(RACE_MOTION_STATE(player))
#define stepRaceMotionAnimationUntilEnd(player) stepRaceMotionAnimationUntilEnd(RACE_MOTION_STATE(player))
#define stepRaceMotionJointAnimationUntilEnd(player) stepRaceMotionJointAnimationUntilEnd(RACE_MOTION_STATE(player))
#define initRaceMotionModelParts(player) initRaceMotionModelParts(RACE_MOTION_INIT_STATE(player))

typedef struct {
    /* 0x00 */ s8 active;
    /* 0x01 */ char pad1[0xAF];
} Unk8011228C;

typedef struct {
    /* 0x000 */ SoundPosition pos;
    /* 0x00C */ char padC[0x600];
} RacePlayerSoundPosition;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
} PlayerTuningRow;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ char pad2[2];
    /* 0x04 */ s16 unk4;
    /* 0x06 */ char pad6[2];
    /* 0x08 */ RaceVec3i unk8;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ char pad16[2];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    char pad20[0x20];
    s32 unk40;
    s32 unk44;
} Struct800955C0;

typedef struct {
    RaceVec3i pos;
    s16 unkC;
    s16 angle;
} CourseStartPosition;

typedef struct {
    s16 angle;
    s16 unk2;
    char pad4[0x44];
} CourseAngleEntry;

typedef union {
    s32 value;
    u64 align;
} RaceSpawnX;

extern s32 enqueueSoundEffect(s32, s32);
extern s16 calculateFixedAngleBetweenXZPoints(s32, s32, s32, s32);
extern void enqueuePlayerLoopingPositionalSoundRequest(s32, SoundPosition *, s32, s32, f32, s16);
extern void addRenderCallback(void *, void (*)(void *), void *);
extern void *createCallbackTaskWithUserIdPreservingArgs(void *, s32, s32, s32);
extern void (*gRacePlayerAirborneUpdateHandlers[])(RacePlayer *);
extern void (*gRacePlayerTrickSubstateHandlers[])(RacePlayer *);
extern void (*gRacePlayerMode07StateHandlers[])(RacePlayer *);
extern void (*gRacePlayerMode30StateHandlers[])(RacePlayer *);
extern void (*gRacePlayerMode32CharacterHandlers[])(RacePlayer *);
extern void (*gRacePlayerMode35CharacterHandlers[])(RacePlayer *);
extern void (*gRacePlayerModePostUpdateHandlers[])(RacePlayer *);
extern void (*gRacePlayerModeUpdateHandlers[])(RacePlayer *);
extern s32 D_800DECC0[];
extern CourseStartPosition gRacePlayerPreviewStartPositions[][4];
extern u16 gRacePlayerVoiceBaseSoundIds;
extern u16 gRacePlayerVoiceLeadSoundOffsets;
extern u16 gRacePlayerVoiceSplitSoundOffsets;
extern PlayerTuningRow gRacePlayerBoardTuningRows[];
extern PlayerTuningRow gRacePlayerCharacterTuningRows[];
extern PlayerTuningRow gRacePlayerDemoBoardTuningRows[];
extern PlayerTuningRow gRacePlayerDemoCharacterTuningRows[];
extern Struct800955C0 gRaceCourseStartEntries[];
extern CourseAngleEntry gSpiralCourseObjectAngles[];
extern CourseAngleEntry gLaunchRampCourseObjectAngles[];
extern u8 gRaceResultState;
extern u8 gMainMenuModeSelection;
extern u8 gRaceSplitscreenMode;
extern s8 gRacePlayerCount;
extern u8 gRaceUpdatePaused;
extern u8 gRaceDemoPlaybackEnabled;
extern u8 gRaceTypeSelection;
extern u8 gRaceCameraModeChangeDisabled;
extern u8 gTrainingCourseLesson;
extern s16 gRaceCourseIndex;
extern s32 gMenuFlowState;
extern s16 gRacePlayerAttackStartTimer;
extern s16 gRaceLapCount;
extern s16 gFrameCounter;
extern Unk8011228C gRacePlayerHudStatuses[];
extern void *D_801248C8;
extern void *D_801248EC;

void initRacePlayers(void) {
    gRacePlayers[0].playerIndex = 0;
    gRacePlayers[1].playerIndex = 1;
    gRacePlayers[2].playerIndex = 2;
    gRacePlayers[3].playerIndex = 3;
    if (gRacePlayers[0].isActive != 0) {
        initRacePlayer(&gRacePlayers[0]);
    }
    if (gRacePlayers[1].isActive != 0) {
        initRacePlayer(&gRacePlayers[1]);
    }
    if (gRacePlayers[2].isActive != 0) {
        initRacePlayer(&gRacePlayers[2]);
    }
    if (gRacePlayers[3].isActive != 0) {
        initRacePlayer(&gRacePlayers[3]);
    }
}

// applyRacePlayerTuning best match: 99.747% (base_13.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/player/race_player_update/applyRacePlayerTuning.s")

#ifdef NON_MATCHING
void applyRacePlayerTuning(RacePlayer *arg0) {
    RacePlayer *player;
    PlayerTuningRow *var_v0;
    s32 new_var;
    PlayerTuningRow *var_v1;
    PlayerTuningRow *temp_a1;
    PlayerTuningRow *temp_a2;
    s32 temp_t5;

    player = arg0;
    var_v0 = gRacePlayerCharacterTuningRows;
    if ((gRaceDemoPlaybackEnabled != 0) || (var_v1 = var_v0, gMainMenuModeSelection != 0)) {
        var_v0 = gRacePlayerDemoBoardTuningRows;
        var_v1 = gRacePlayerDemoCharacterTuningRows;
    } else {
        var_v0 = gRacePlayerBoardTuningRows;
    }
    temp_a1 = (PlayerTuningRow *)((u8 *)var_v1 + (player->characterId * sizeof(PlayerTuningRow)));
    temp_a2 = (PlayerTuningRow *)((u8 *)var_v0 + (player->unk11 * sizeof(PlayerTuningRow)));
    if (1) {
        temp_t5 = ((temp_a1->unk0 + temp_a2->unk0) << 3) << 5;
        player->unk25C = temp_t5;
        player->unk314 = temp_t5;
        player->unk264 = (new_var = (((temp_a1->unk4 + temp_a2->unk4) << 2) << 1) << 1);
        player->unk260 = (temp_a1->unk2 + temp_a2->unk2) << 4;
        player->unk268 = temp_a1->unk6 + temp_a2->unk6;
    }
    player->unk274 = (temp_a1->unk8 + temp_a2->unk8) << 4;
    player->unk26C = (temp_a1->unkA + temp_a2->unkA) << 4;
    player->unk270 = (temp_a1->unkC + temp_a2->unkC) << 4;
    player->unk278 = (temp_a1->unkE + temp_a2->unkE) << 4;
    player->unk27C = (temp_a1->unk10 + temp_a2->unk10) << 4;
}
#endif

// initRacePlayer best match: 98.796% (nonmatchings/initRacePlayer-3379532139742180785/base_31.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/player/race_player_update/initRacePlayer.s")

#ifdef NON_MATCHING
void initRacePlayer(RacePlayer *player) {
    RaceVec3i *pos;
    RaceVec3i *unk34;
    CourseStartPosition *start;
    u32 size;
    u32 i;
    char *clearPtr;
    s32 groundY;

    size = (s32)(player + 1) - (s32)&player->pos;
    i = 0;
    if (size != 0) {
        clearPtr = (char *)&player->pos;
        do {
            i++;
            *clearPtr = 0;
            clearPtr++;
        } while (i < size);
    }
    player->unk588 = 0.0f;
    pos = &player->pos;
    initRaceMotionModelParts(player);
    setRaceMotionAnimation(player, 1);
    if (gRacePlayerCount == 4) {
        switch (player->playerIndexU16) {
        case 0:
            player->posX = -0xC0000;
            break;
        case 1:
            player->posX = 0xC0000;
            break;
        case 2:
            player->posX = -0x240000;
            break;
        case 3:
            player->posX = 0x240000;
            break;
        }
    }
    player->posZ = -0x200000;
    unk34 = &player->unk34;
    *unk34 = *pos;
    player->unk60 = 0x40000;
    player->unk280 = 0x80000;
    player->unk284 = 0xE0000;
    applyRacePlayerTuning(player);
    if (player->unk4 != 0) {
        player->unk274 = 0x10000;
    }
    if ((player->unk4 != 0) && (player->characterId == 5)) {
        player->unk274 = 0xC0000;
    }
    if (gRaceSplitscreenMode == 2) {
        player->actionEffectType = 1;
    }
    if (player->unk4 == 0) {
        player->unk519 = 6;
    }
    if ((gRaceSplitscreenMode == 1) && (gRaceTypeSelection == 1)) {
        player->shieldEffectTimer = 0xA;
    }
    if (gRaceCourseIndex == 9) {
        player->posZ = -0xBD0000;
    }
    player->unk68 = 0xC0000;
    if ((player->unk4 == 0) && (player->soundDisabled == 0) && (gRaceCameraModeChangeDisabled == 0) &&
        (player->unk278 != player->unk27C)) {
        createCallbackTaskWithUserIdPreservingArgs(initRaceUiBoardReversePrompt, 0, 0x64, player->playerIndexU16);
    }
    if (gRaceDemoPlaybackEnabled == 1) {
        if (player->playerIndexU16 == 0) {
            player->itemEffectType = 3;
            player->itemEffectCount = 3;
        } else {
            player->itemEffectType = 5;
            player->itemEffectCount = 3;
        }
        player->actionEffectType = 1;
    }
    switch (gTrainingCourseLesson) {
    case 1:
        player->facingAngle = 0x57E;
        player->posX = 0xDB870B2D;
        player->posZ = 0xE72C26D8;
        player->unk502 = 0x34;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        player->facingAngle = 0xFD5;
        player->posX = 0xAE2B01F4;
        player->posZ = 0xD6C4D18A;
        player->unk502 = 0x96;
        break;
    case 6:
        player->facingAngle = 0x993;
        player->posX = 0xC183C5F9;
        player->posZ = 0xCE51B844;
        player->unk502 = 0xAE;
        break;
    case 7:
    case 8:
        switch (player->playerIndexU16) {
        case 0:
            player->facingAngle = 0xF7D;
            player->posX = 0x8FC2BA;
            player->posZ = 0xFC4A83D8;
            player->unk502 = 4;
            break;
        case 2:
            player->facingAngle = 0x102;
            player->posX = 0x012FA8E0;
            player->posZ = 0xF9850FD2;
            player->unk502 = 8;
            break;
        case 3:
            player->facingAngle = 0x8F;
            player->posX = 0xC2C658;
            player->posZ = 0xFA155B82;
            player->unk502 = 7;
            break;
        case 1:
            player->facingAngle = 0x8F;
            player->posX = 0x017C2FC4;
            player->posZ = 0xFA1B250E;
            player->unk502 = 7;
            break;
        }
        break;
    case 9:
        player->facingAngle = 0x235;
        player->posX = 0xFEAFF7CD;
        player->posZ = 0xF62C899D;
        player->unk502 = 0xE;
        break;
    }
    switch (gMainMenuModeSelection) {
    case 1:
        player->itemEffectType = 1;
        player->itemEffectCount = 3;
        break;
    case 2:
        player->itemEffectType = 2;
        player->itemEffectCount = 3;
        break;
    case 3:
        player->itemEffectType = 3;
        player->itemEffectCount = 3;
        break;
    case 4:
        player->itemEffectType = 4;
        player->itemEffectCount = 3;
        break;
    case 5:
        player->itemEffectType = 5;
        player->itemEffectCount = 3;
        break;
    case 6:
        player->actionEffectType = 1;
        break;
    case 7:
        player->actionEffectType = 2;
        break;
    case 8:
        player->actionEffectType = 3;
        break;
    case 9:
        player->actionEffectType = 4;
        break;
    case 10:
        player->actionEffectType = 5;
        if (player->playerIndexU16 != 0) {
            player->unk568 = 0x1388;
        }
        break;
    case 11:
        player->actionEffectType = 6;
        if (player->playerIndexU16 == 2) {
            player->actionEffectType = 3;
        }
        player->itemEffectType = 1;
        player->itemEffectCount = 3;
        break;
    }
    groundY = gMainMenuModeSelection;
    if (gMainMenuModeSelection != 0) {
        start = &gRacePlayerPreviewStartPositions[gMainMenuModeSelection - 1][player->playerIndexU16];
        *pos = gRacePlayerPreviewStartPositions[groundY - 1][player->playerIndexU16].pos;
        player->facingAngle =
            gRacePlayerPreviewStartPositions[*(volatile u8 *)&gMainMenuModeSelection - 1][player->playerIndexU16]
                .angle;
        groundY = gMainMenuModeSelection;
        player->unk502 = gRacePlayerPreviewStartPositions[groundY - 1][player->playerIndexU16].unkC;
    }
    player->unk502 = findRaceCourseSurfaceFromHint(player->unk502, player->posX, player->posZ);
    groundY = getRaceCourseSurfaceHeight(player->unk502, player->posX, player->posZ);
    player->posY = groundY;
    if (gRaceDemoPlaybackEnabled == 0) {
        player->posY = groundY + 0x40000;
    }
    *unk34 = *pos;
    player->randomIndex = randomNextMain();
    setRaceCameraMode(player->playerIndexU16, 1);
    if (gRaceDemoPlaybackEnabled == 0) {
        if (gRaceSplitscreenMode == 0) {
            createCallbackTaskWithUserId(waitForRaceStartPlayerEffect, 0, 1, player->playerIndexU16);
        }
    }
}
#endif

void updateRacePlayers(void) {
    s32 i;

    gRacePlayers[0].unk582 = 0;
    gRacePlayers[1].unk582 = 0;
    gRacePlayers[2].unk582 = 0;
    gRacePlayers[3].unk582 = 0;
    if (gRaceUpdatePaused == 0) {
        updateRacePlayerInput(&gRacePlayers[0]);
        updateRacePlayerInput(&gRacePlayers[1]);
        updateRacePlayerInput(&gRacePlayers[2]);
        updateRacePlayerInput(&gRacePlayers[3]);
        updateRacePlayerRankDisplay();
        for (i = 0; i < gRacePlayerCount; i++) {
            updateRacePlayer(&gRacePlayers[i]);
        }
        resolveRacePlayerBodyCollisions();
        updateRacePlayerRankings();
    }
}

void updateRacePlayer(RacePlayer *player) {
    s32 i;
    s32 catchupDelta;
    RaceSpawnX spawnX;
    s32 spawnY;
    s32 spawnZ;
    s32 deltaX;
    s32 speedDelta;
    s32 verticalDelta;
    s32 deltaZ;
    s16 spawnAngle;
    Vec3i offset;
    Vec3i transformedOffset;
    FixedTransform mtx;
    u8 cooldown;

    player->unk50C = 0;
    player->unk331 = player->unk330;
    player->unk330 = getRaceCourseSurfaceType(player->unk502, player->pos.x, player->pos.z);
    if (player->stateFlags & 0x02000000) {
        player->unk330 = 0xB;
    }

    if (player->unk330 >= 0x11 && player->unk330 < 0x19 && !(player->stateFlags & 0x200)) {
        offset.x = 0;
        offset.y = 0;
        offset.z = -0x2000;
        getRaceCourseSurfaceSpawnTransform(player->unk502, &spawnX.value, &spawnY, &spawnZ, &spawnAngle);
        spawnAngle = spawnAngle - (player->unk330 << 9) + 0x2200;
        makeFixedRotationY(&mtx.rotation, spawnAngle);
        transformVec3iByFixedMatrix(&mtx, &offset, &transformedOffset);
        player->pos.x += transformedOffset.x;
        player->pos.y += transformedOffset.y;
        player->pos.z += transformedOffset.z;
    }

    player->unk504 = -projectRaceCourseSurfaceProgress(player->unk502, player->pos.x, player->pos.z);
    player->unk40.x = player->pos.x - player->unk34.x;
    player->unk40.y = player->pos.y - player->unk34.y;
    player->unk40.z = player->pos.z - player->unk34.z;
    if (player->unk74 < player->unk40.y) {
        player->unk40.y = player->unk74;
    }
    verticalDelta = player->unk40.y;
    if (player->unk40.y < -0x400000) {
        player->unk40.y = -0x400000;
    }
    if (player->unk40.y > 0x400000) {
        player->unk40.y = 0x400000;
    }

    player->unk34 = player->pos;
    player->unk74 = 0x7FFFFFFF;
    player->unk4D0 = player->unk4A0;
    player->unk4DC = player->unk4AC;

    player->unk4E8 = player->unk4B8;
    player->unk4F4 = player->unk4C4;
    player->unk310 = player->unk25C;

    if (player->unk4 != 0) {
        player->unk310 += 0x6000;
    }

    for (i = 0; i < player->unk2D8; i++) {
        player->unk310 -= player->unk310 >> 2;
    }

    if ((player->pendingItemHitFlags == 0) && (player->unk330 == 1) && (player->unk310 >= 0x50001)) {
        player->unk310 = 0x50000;
    }

    if (player->trailEffectTimer != 0) {
        if ((gRaceSplitscreenMode == 1) && (gRaceTypeSelection == 0)) {
            player->unk310 += 0x40000;
        } else {
            player->unk310 += 0x30000;
        }
    }

    resolveRacePlayerHitReactions(player);

    if ((player->unk4 == 0) || (player->unk52A == 0)) {
        speedDelta = D_800DECC0[player->rankIndex];
        catchupDelta = speedDelta - player->unk318;
        if (catchupDelta >= 0x21) {
            catchupDelta = 0x20;
        }
        if (catchupDelta < -0x30) {
            catchupDelta = -0x30;
        }
        player->unk318 += catchupDelta;
        player->unk310 += player->unk318;
    } else {
        if (player->unk52A == 1) {
            player->unk310 += 0x70000;
        }
        if (player->unk52A == 2) {
            player->unk310 /= 3;
        }
        if ((player->unk52A == 3) && (player->rankIndex != 3)) {
            player->unk310 -= player->unk310 >> 4;
        }
    }

    if ((player->unk4 != 0) && (player->unk508 >= (gRaceLapCount - 1))) {
        if (((gRaceCourseStartEntries[gRaceCourseIndex].unk0 - 0x14) < player->unk502) &&
            (player->unk502 < gRaceCourseStartEntries[gRaceCourseIndex].unk0)) {
            player->unk310 = player->unk25C;
        }
    }

    speedDelta = player->unk310 - player->unk314;
    if (speedDelta > 0xE00) {
        speedDelta = 0xE00;
    }
    if (speedDelta < -0xE00) {
        speedDelta = -0xE00;
    }
    player->unk314 += speedDelta;
    player->stateFlags &= ~0x800;

    if (player->unk4 != 0) {
        updateRacePlayerCheckpointEvents((struct RacePlayerProgressState *) player);
    }

    if (player->unk578 != 0) {
        player->unk578--;
    }

    gRacePlayerModeUpdateHandlers[player->mode](player);
    player->unk517 = 0;
    player->unk57B = 0;
    player->actionEffectEnabled = 0;
    player->stateFlags &= ~0x2000000;
    if (player->stateFlags & 2) {
        player->stateFlags &= ~2;
    } else {
        player->unk6C = 0;
        player->unk6E = 0;
        player->unk70 = 0;
    }

    if (player->stateFlags & 0x80000) {
        gViewportStates[player->playerIndexU16].overlayAlpha += 0x10;
        if (gViewportStates[player->playerIndexU16].overlayAlpha > 0xFF) {
            gViewportStates[player->playerIndexU16].overlayAlpha = 0xFF;
        }
    } else {
        gViewportStates[player->playerIndexU16].overlayAlpha -= 0x10;
        if (gViewportStates[player->playerIndexU16].overlayAlpha < 0) {
            gViewportStates[player->playerIndexU16].overlayAlpha = 0;
        }
    }

    deltaX = player->pos.x - player->unk34.x;
    deltaZ = player->pos.z - player->unk34.z;
    player->unk29C = integerSquareRoot64((s64) deltaX * deltaX + (s64) deltaZ * deltaZ);
    player->unk2C8 = player->unk40.x;
    player->unk2CC = player->unk40.z;
    player->unk5C = player->pos.y - 0x60000;

    if (player->unk2D4 != 0) {
        player->unk2D4--;
    }

    if (player->trailEffectTimer != 0) {
        player->unk584 = 7;
        player->unk582 = 0x7F;
        player->unk588 = 0.0f;
        if (player->trailEffectTimer >= 0xDD) {
            player->actionEffectLevel = 2;
            player->actionEffectFrame = 0;
        }
    }

    if (player->actionSoundTimer != 0) {
        player->actionSoundTimer--;
    }
    if (player->stateFlags & 0x403040) {
        player->actionSoundTimer = 0;
    }

    if (!(gMenuFlowState & 1)) {
        cooldown = player->unk519;
        if (cooldown != 0) {
            player->unk519 = cooldown - 1;
        }
    }
}

void updateRacePlayerMotionFeedback(RacePlayer *player) {
    s16 angleDiff;
    s32 itemType;
    s32 deltaX;
    s32 deltaZ;

    deltaX = player->posX - player->unk34.x;
    deltaZ = player->posZ - player->unk34.z;
    angleDiff = (calculateFixedAngleFromDeltaXZ(deltaX, deltaZ) - player->facingAngle) & 0xFFF;
    if (angleDiff >= 0x801) {
        angleDiff = 0x1000 - angleDiff;
    }
    if (angleDiff >= 0x401) {
        angleDiff = 0x800 - angleDiff;
    }

    player->unk588 = (f32) ((f64) (f32) -angleDiff / 171.0);
    player->unk582 = player->unk29C >> 0xC;
    if (player->unk582 >= 0x80) {
        player->unk582 = 0x7F;
    }

    itemType = getRaceItemEffectType(player->unk330);
    player->unk584 = 3;
    if (itemType == 1) {
        player->unk584 = 0x1E;
        player->unk582 = 0x7F;
        player->unk588 = 0.0f;
    }
    if (itemType == 2) {
        player->unk584 = 4;
    }
    if (3 == itemType) {
        player->unk584 = 5;
    }
    if (4 == itemType) {
        player->unk584 = 4;
        if (player->unk330 == 0xD) {
            player->unk584 = 6;
            if (player->unk582 >= 0x51) {
                player->actionEffectLevel = 2;
                player->actionEffectFrame = 1;
            }
        }
        player->unk588 = 0.0f;
    }
    if (player->stateFlags & 0x2000000) {
        player->unk584 = 4;
    }
}

void updateRacePlayerMode00Grounded(RacePlayer *player) {
    volatile s32 pad[26];
    s64 temp;
    s32 turnTarget;
    s32 targetX;
    s32 surfaceCue;
    s32 targetZ;
    s32 bankRate;
    Struct800955C0 *spawn;
    s32 lean;
    s32 speed;
    s16 steerAngle;
    s16 turn;
    s16 quickTurn;
    s32 rotation;
    s16 unused;
    s32 interpolated;

    if (player->updateState == 0) {
        player->updateState++;
        player->stateFlags &= ~0x200;
        player->unk92 = 0;
        player->unk93 = 0;
        player->subState = 0;
        player->subStateTimer = 0;
        player->unk2FA = 0;
        player->unk528 = 0;
        player->unk258 = 0;
        if (player->unk57B != 0) {
            enqueueRacePlayerVoiceSound(player, 7);
            setRaceMotionAnimation(player, 0x22);
        }
    }

    if (player->unk57A != 0) {
        player->unk92 = 6;
    }
    if (player->unk60 == 0) {
        player->unk60 = 0x40000;
    }

    player->unk40.y -= player->unk260;
    speed = player->unk254;
    if (speed > 0) {
        if (speed < 0x4000) {
            speed = 0x4000;
        }
    } else if (speed >= -0x3FFF) {
        speed = -0x4000;
    }

    if (player->unk4 != 0) {
        getRaceCourseTargetPositionAhead(player->unk502, player->posX, player->posZ, &targetX, &targetZ, (s8) player->unk17,
                      (u16) player->playerIndex);
        turn = (calculateFixedAngleBetweenXZPoints(player->posX, player->posZ, targetX, targetZ) - player->facingAngle) & 0xFFF;
        if (turn >= 0x801) {
            turn -= 0x1000;
        }
        if (turn >= 0x101) {
            turn = 0x100;
        }
        if (turn < -0x100) {
            turn = -0x100;
        }

        quickTurn = turn >> 2;
        turn = (turn + 1) >> 1;
        if (turn >= 0x20) {
            turn = 0x1F;
        }
        if (turn < -0x1F) {
            turn = -0x1F;
        }

        turn = (turn - player->unk528) & 0xFFF;
        if (turn >= 0x801) {
            turn -= 0x1000;
        }
        if (turn >= 3) {
            turn = 2;
        }
        if (turn < -2) {
            turn = -2;
        }
        player->unk528 += turn;
        turn = player->unk528;
        if (turn == 1) {
            turn = 0;
        }
        if (turn == -1) {
            turn = 0;
        }
        player->unk2F8 = 0x3F;
    } else {
        turn = -player->stickX;
        player->unk2F8 = 0x20 - player->stickY;
        if (player->unk2F8 < 0x1F) {
            player->unk2F8 = 0x1F;
        }
    }

    lean = speed;
    if (player->unk254 > 0) {
        lean = -speed;
        turn = -turn;
    }

    if (player->unk4 == 0) {
        spawn = &gRaceCourseStartEntries[gRaceCourseIndex];
        if ((spawn->unk0 == player->unk502) && !(player->stateFlags & 0x40)) {
            surfaceCue = (s16)(((calculateFixedAngleBetweenXZPoints(player->posX, player->posZ, spawn->unk40, spawn->unk44) -
                                  player->facingAngle) +
                                 0x400) &
                                0xFFF);
            if (surfaceCue < 0x800) {
                if (player->stateFlags & 0x400) {
                    player->unk92 = 0;
                    player->stateFlags &= ~0x400;
                    player->unk93 = 6 - player->unk93;
                }
            } else if (!(player->stateFlags & 0x400)) {
                player->unk92 = 0;
                player->stateFlags |= 0x400;
                player->unk93 = 6 - player->unk93;
            }
        } else {
            if (player->unk254 < -0x8000) {
                if (player->stateFlags & 0x400) {
                    if (player->unk92 >= 6) {
                        player->unk92 = 0;
                        player->stateFlags &= ~0x400;
                        player->unk93 = 6 - player->unk93;
                    } else {
                        player->unk92++;
                    }
                }
            }
            if (player->unk254 >= 0x8001) {
                if (!(player->stateFlags & 0x400)) {
                    if (player->unk92 >= 6) {
                        player->unk92 = 0;
                        player->stateFlags |= 0x400;
                        player->unk93 = 6 - player->unk93;
                    } else {
                        player->unk92++;
                    }
                }
            }
        }
    }

    if (!(gMenuFlowState & 1)) {
        turnTarget = updateRacePlayerLeanAngle(player, lean, turn);
        if (player->unk93 == 0) {
            steerAngle = player->unk2FA;
            steerAngle = turnTarget - steerAngle;
            if (steerAngle >= 0x31) {
                steerAngle = 0x30;
            }
            if (steerAngle < -0x30) {
                steerAngle = -0x30;
            }
            player->unk2FA += steerAngle;
            if (player->unk2FA == 0) {
                if (player->animationId == 0x22) {
                    if (stepRaceMotionJointAnimationUntilEnd(player) != 0) {
                        setRaceMotionAnimation(player, 1);
                    }
                } else {
                    if (player->animationId != 1) {
                        setRaceMotionAnimation(player, 1);
                    }
                    stepRaceMotionLoopingJointAnimation(player);
                }
            } else if (player->unk2FA >= 0) {
                setRaceMotionAnimation(player, 3);
                blendRaceMotionJointAnimation(player, 3, player->unk2FA, 0x118);
            } else {
                setRaceMotionAnimation(player, 2);
                blendRaceMotionJointAnimation(player, 2, -player->unk2FA, 0x118);
            }
        } else {
            player->unk93--;
            steerAngle = -player->unk2FA;
            if (steerAngle >= 0x31) {
                steerAngle = 0x30;
            }
            if (steerAngle < -0x30) {
                steerAngle = -0x30;
            }
            player->unk2FA += steerAngle;
            setRaceMotionAnimation(player, player->unk93 + 8);
            stepRaceMotionJointAnimationUntilEnd(player);
        }

        if (player->unk4 == 0) {
            steerAngle = 0;
            bankRate = (-turn * 0x10 * player->unk268) / 0x100;
            interpolated = (((player->unk270 - player->unk26C) * (0x3F - player->unk2F8)) / 0x20) + player->unk26C;
            if (bankRate != 0) {
                lean = (lean - 0xFF) / 0x100;
                temp = ((s64) interpolated * lean * lean) / bankRate;
                if (temp != 0) {
                    if (temp > 0) {
                        temp += 0xC0000;
                    }
                    if (temp < 0) {
                        temp += 0xFFFFFFFFFFF40000LL;
                    }
                    if (temp > 0x10000000LL) {
                        temp = 0x10000000LL;
                    }
                    if (temp < -0x10000000LL) {
                        temp = -0x10000000LL;
                    }
                    steerAngle = calculateFixedAngleBetweenXZPoints((s32) temp, 0, 0, speed);
                    if (temp > 0) {
                        steerAngle -= 0x400;
                    }
                    if (temp < 0) {
                        steerAngle -= 0xC00;
                    }
                }
                steerAngle &= 0xFFF;
            }

            if (steerAngle < 0x801) {
                if (steerAngle >= 0x81) {
                    steerAngle = 0x80;
                }
            }
            if ((steerAngle >= 0x801) && (steerAngle < 0xF80)) {
                steerAngle = 0xF80;
            }
            player->facingAngle += steerAngle;
        } else {
            player->facingAngle += quickTurn;
        }

        rotation = 0;
        if (player->trailEffectTimer != 0) {
            rotation = -0x8000;
            if (player->unk2DC != 0) {
                rotation = 0x8000;
            }
        }
        if ((gMainMenuModeSelection != 0) && ((u8) player->unk519 == 6)) {
            rotation = -0x60000;
        }
        updateRacePlayerLocalVelocity(player, 0, rotation, player->unk274, player->unk278, player->unk27C);

        if (player->unk57A == 0) {
            if (player->unk4 == 0) {
                if (player->inputFlags & 0x8000) {
                    player->mode = 2;
                    player->updateState = 0;
                    player->updateTimer = 0;
                }
            } else if (player->unk524 != 0) {
                player->mode = 2;
                player->updateState = 0;
                player->updateTimer = 0;
                player->unk525 = 1;
            } else if ((player->unk29C < 0x30000) || (player->unk254 >= (s32) 0xFFFD0001)) {
                player->mode = 2;
                player->updateState = 0;
                player->updateTimer = 0;
                player->unk525 = 0;
            }
            updateRacePlayerItemEffectUse(player);
            updateRacePlayerActionEffectUse(player);
        } else {
            player->unk57A--;
            player->unk92 = 6;
        }

        if (player->stateFlags & 0x2000000) {
            player->mode = 0x1D;
            player->updateState = 0;
            player->updateTimer = 0;
        }
        if (player->stateFlags & 0x40) {
            player->mode = 0x1E;
            player->updateState = 0;
            player->updateTimer = 0;
        }
    } else {
        stepRaceMotionLoopingJointAnimation(player);
    }

    player->posX += player->unk40.x;
    player->posY += player->unk40.y;
    player->posZ += player->unk40.z;
    updateRacePlayerMotionFeedback(player);
    if (player->unk517 != 0) {
        enqueueRacePlayerVoiceSound(player, 5);
    }
}

void updateRacePlayerMode29Crash(RacePlayer *player) {
    s16 updateState;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateFlags &= ~0x200;
        player->stateTimer = 0;
        player->unk80 = 0;
        player->subState = 0;
        player->subStateTimer = 0;
        player->unk2FA = 0;
        player->unk528 = 0;
        setRaceMotionAnimation(player, 0x1E);
    }

    player->unk40.y -= player->unk260;
    player->facingAngle = player->unk332;
    player->pitchAngle = player->unk334;
    if (player->stateFlags & 0x400) {
        player->facingAngle += 0x800;
        player->pitchAngle = -(player->pitchAngle & 0xFFF);
    }

    updateRacePlayerLeanAngle(player, 0, 0);
    clampRacePlayerVectorXZSpeed(&player->unk40, player);
    player->posX += player->unk40.x;
    player->posY += player->unk40.y;
    player->posZ += player->unk40.z;

    if (stepRaceMotionAnimationUntilEnd(player) != 0) {
        setRaceMotionAnimation(player, 0x1F);
        stepRaceMotionAnimationUntilEnd(player);
    }

    player->unk578 = 6;
    updateRacePlayerMotionFeedback(player);
    if (player->unk517 != 0) {
        enqueueRacePlayerVoiceSound(player, 5);
    }
}

void updateRacePlayerMode01JumpStart(RacePlayer *player) {
    s32 unused;
    s32 targetX;
    s32 targetZ;
    s16 angleDiff;
    s32 yVel;

    if (player->updateState == 0) {
        if (player->animationId != 4) {
            setRaceMotionAnimation(player, 4);
        }
        player->updateState++;
        player->stateFlags |= 0x200;
        player->unk60 = 0;
        resetRacePlayerTrickSubstate(player);
        player->unk2A6 = 0;
    }

    if (player->subState == 0) {
        updateRacePlayerItemEffectUse(player);
        if (player->unk4 == 0) {
            if (player->stickX > 0) {
                player->facingAngle -= 0x18;
            }
            if (player->stickX < 0) {
                player->facingAngle += 0x18;
            }
        } else {
            getRaceCourseTargetPositionAhead(player->unk502, unused = player->posX, player->posZ, &targetX, &targetZ,
                          (s8) player->unk17, (u16) player->playerIndex);
            angleDiff = (calculateFixedAngleBetweenXZPoints(player->posX, player->posZ, targetX, targetZ) - player->facingAngle) & 0xFFF;
            if (angleDiff >= 0x801) {
                angleDiff -= 0x1000;
            }
            if (angleDiff >= 0x19) {
                angleDiff = 0x18;
            }
            if (angleDiff < -0x18) {
                angleDiff = -0x18;
            }
            player->facingAngle += angleDiff;
        }
        stepRaceMotionAnimationUntilEnd(player);
    }

    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);
    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;
    if (player->unk517 != 0) {
        enqueueRacePlayerVoiceSound(player, 5);
    }
}

void updateRacePlayerMode22Airborne(RacePlayer *player) {
    s32 unused;
    s32 targetX;
    s32 targetZ;
    s16 angleDiff;
    s32 yVel;

    if (player->updateState == 0) {
        player->updateState++;
        player->stateFlags |= 0x208;
        player->unk60 = 0;
        player->unk80 = 0;
        resetRacePlayerTrickSubstate(player);
        player->unk2A6 = 0;
    }

    if (player->subState == 0) {
        updateRacePlayerItemEffectUse(player);
        if (player->unk4 == 0) {
            if (player->stickX > 0) {
                player->facingAngle -= 0x18;
            }
            if (player->stickX < 0) {
                player->facingAngle += 0x18;
            }
        } else {
            getRaceCourseTargetPositionAhead(player->unk502, unused = player->posX, player->posZ, &targetX, &targetZ,
                          (s8) player->unk17, (u16) player->playerIndex);
            angleDiff = (calculateFixedAngleBetweenXZPoints(player->posX, player->posZ, targetX, targetZ) - player->facingAngle) & 0xFFF;
            if (angleDiff >= 0x801) {
                angleDiff -= 0x1000;
            }
            if (angleDiff >= 0x19) {
                angleDiff = 0x18;
            }
            if (angleDiff < -0x18) {
                angleDiff = -0x18;
            }
            player->facingAngle += angleDiff;
        }
    }

    updateRacePlayerTrickSubstate(player);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);
    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;
    if (player->unk517 != 0) {
        enqueueRacePlayerVoiceSound(player, 5);
    }
}

void dispatchRacePlayerAirborneMode(RacePlayer *player) {
    gRacePlayerAirborneUpdateHandlers[player->updateState](player);
}

void updateRacePlayerAirborneLaunch(RacePlayer *player) {
    Struct800955C0 *spawn;
    s16 updateTimer;
    s32 velocityY;
    s32 rotation;
    s8 turnTimer;

    updateTimer = player->updateTimer;
    if (updateTimer == 0) {
        player->updateTimer = updateTimer + 1;
        player->stateTimer = 0x40000;
        player->stateFlags |= 0x10;
        setRaceMotionAnimation(player, 5);
        resetRacePlayerSurfaceCueState(player);
        player->unk60 = 0x40000;
        player->unk2A6 = 0;
        player->unk93 = 0;
        player->unk336 = 0;
    }

    if (player->unk4 == 0) {
        spawn = &gRaceCourseStartEntries[gRaceCourseIndex];
        if ((spawn->unk0 == player->unk502) && !(player->stateFlags & 0x40)) {
            velocityY = (s16) (((calculateFixedAngleBetweenXZPoints(player->posX, player->posZ, spawn->unk40, spawn->unk44) -
                                  player->facingAngle) +
                                 0x400) &
                                0xFFF);
            if (velocityY < 0x800) {
                if (player->stateFlags & 0x400) {
                    player->stateFlags &= ~0x400;
                    player->unk93 = 6 - player->unk93;
                }
            } else if (!(player->stateFlags & 0x400)) {
                player->stateFlags |= 0x400;
                player->unk93 = 6 - player->unk93;
            }
        } else {
            if (player->unk254 < -0x8000) {
                if (player->stateFlags & 0x400) {
                    player->stateFlags &= ~0x400;
                    player->unk93 = 6 - player->unk93;
                }
            }
            if (player->unk254 >= 0x8001) {
                if (!(player->stateFlags & 0x400)) {
                    player->stateFlags |= 0x400;
                    player->unk93 = 6 - player->unk93;
                }
            }
        }
    }

    turnTimer = player->unk93;
    if (turnTimer != 0) {
        player->unk93 = turnTimer - 1;
        setRaceMotionAnimation(player, player->unk93 + 8);
    } else if (player->animationId != 5) {
        setRaceMotionAnimation(player, 5);
    }

    updateRacePlayerItemEffectUse(player);
    stepRaceMotionJointAnimationUntilEnd(player);

    velocityY = player->unk60;
    if (velocityY != 0) {
        if (velocityY < 0x20000) {
            player->unk60 = 0;
        } else {
            player->unk60 = velocityY - 0x20000;
        }
    } else {
        player->updateState++;
    }

    if ((player->unk4 == 0) && !(player->inputFlags & 0x8000)) {
        player->stateFlags &= ~0x10;
    }

    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk260;

    rotation = 0;
    if (player->trailEffectTimer != 0) {
        rotation = -0x8000;
        if (player->unk2DC != 0) {
            rotation = 0x8000;
        }
    }

    updateRacePlayerLocalVelocity(player, 0, rotation, player->unk274, player->unk278, player->unk27C);
    player->posX += player->unk40.x;
    player->posY += player->unk40.y;
    player->posZ += player->unk40.z;
    updateRacePlayerSurfaceCue(player);
    updateRacePlayerMotionFeedback(player);
    if (player->unk517 != 0) {
        enqueueRacePlayerVoiceSound(player, 5);
    }
}

// updateRacePlayerAirborneCruise best match: 99.550% (nonmatchings/updateRacePlayerAirborneCruise-3379532139742180785/base_10.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/player/race_player_update/updateRacePlayerAirborneCruise.s")

#ifdef NON_MATCHING
void updateRacePlayerAirborneCruise(RacePlayer *player)
{
  s32 sp2C;
  s32 var_a2;
  s32 var_v1;
  s8 turnTimer;

  sp2C = 0;
  if (player->unk4 == 0)
  {
    if ((player->unk502 == gRaceCourseStartEntries[gRaceCourseIndex].unk0) && (!(player->stateFlags & 0x40)))
    {
      var_v1 = calculateFixedAngleBetweenXZPoints(player->pos.x, player->pos.z, gRaceCourseStartEntries[gRaceCourseIndex].unk40, gRaceCourseStartEntries[gRaceCourseIndex].unk44) - player->facingAngle + 0x400;
      if ((s16) (var_v1 & 0xFFF) < 0x800)
      {
        if (player->stateFlags & 0x400)
        {
          player->stateFlags &= ~0x400;
          player->unk93 = 6 - player->unk93;
        }
      }
      else
      {
        if (!(player->stateFlags & 0x400))
        {
          player->stateFlags |= 0x400;
          player->unk93 = 6 - player->unk93;
        }
      }
    }
    else
    {
      if (player->unk254 < (-0x8000))
      {
        if (player->stateFlags & 0x400)
        {
          player->stateFlags &= ~0x400;
          player->unk93 = 6 - player->unk93;
        }
      }
      if (player->unk254 >= 0x8001)
      {
        if (!(player->stateFlags & 0x400))
        {
          player->stateFlags |= 0x400;
          player->unk93 = 6 - player->unk93;
        }
      }
    }
    if (gRaceDemoPlaybackEnabled == 0)
    {
      if (player->unk336 < 0x5A)
      {
        player->unk336++;
      }
    }
  }

  turnTimer = player->unk93;
  if (turnTimer != 0)
  {
    player->unk93 = turnTimer - 1;
    setRaceMotionAnimation(player, player->unk93 + 8);
  }
  else if (player->animationId != 5)
  {
    setRaceMotionAnimation(player, 5);
  }

  if (player->stateFlags & 0x10)
  {
    if (player->unk4 == 0)
    {
      if (!(player->inputFlags & 0x8000))
      {
        player->stateFlags &= ~0x10;
      }
    }
    else
    {
      player->stateFlags &= ~0x10;
      if (player->unk525 != 0)
      {
        player->stateTimer = 0x46000;
      }
    }
    if (player->stateFlags & 0x10)
    {
      if (player->stateTimer < 0x46000)
      {
        player->stateTimer += 0x2000;
      }
    }
  }
  updateRacePlayerItemEffectUse(player);
  updateRacePlayerLeanAngle(player, player->unk254, 0);
  player->unk40.y -= player->unk260;
  var_a2 = 0;
  if (player->trailEffectTimer)
  {
    var_a2 = -0x8000;
    if (player->unk2DC != 0)
    {
      var_a2 = 0x8000;
    }
  }
  if (!(player->stateFlags & 0x10))
  {
    if (player->stateFlags & 0x400)
    {
      sp2C = 0x30000;
      if (((u8) player->unk519) != 0)
      {
        var_a2 = 0x100000;
      }
      if (player->unk336 >= 0x3C)
      {
        player->unk310 += 0x50000;
        player->unk314 += 0x50000;
        var_a2 += 0x50000;
        enqueueRacePlayerVoiceSound(player, 0);
      }
    }
    else
    {
      sp2C = -0x30000;
      if (((u8) player->unk519) != 0)
      {
        var_a2 = -0x100000;
      }
      if (player->unk336 >= 0x3C)
      {
        player->unk310 += 0x50000;
        player->unk314 += 0x50000;
        var_a2 += 0xFFFB0000;
        enqueueRacePlayerVoiceSound(player, 0);
      }
    }
  }
  updateRacePlayerLocalVelocity(player, sp2C, var_a2, player->unk274, player->unk278, player->unk27C);
  var_v1 = updateRacePlayerSurfaceCue(player);
  if (!(player->stateFlags & 0x10))
  {
    if (player->stateTimer < 0x46000)
    {
      var_v1 = 0;
    }
    if (var_v1 >= 9)
    {
      player->stateTimer = 0x50000;
    }
    player->unk2A2 = 0;
    switch (var_v1)
    {
      case 1:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 0);
      }
        player->mode = 0xD;
        player->unk2A6 = 1;
        player->stateFlags |= 0x800;
        break;

      case 2:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 0);
      }
        player->mode = 0xF;
        player->unk2A6 = 2;
        player->stateFlags |= 0x800;
        break;

      case 3:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 0);
      }
        player->mode = 0x10;
        player->unk2A6 = 3;
        player->stateFlags |= 0x800;
        break;

      case 4:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 0);
      }
        player->mode = 0x11;
        player->unk2A6 = 4;
        player->stateFlags |= 0x800;
        break;

      case 5:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x12;
        player->unk2A6 = 5;
        player->stateFlags |= 0x800;
        break;

      case 6:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x13;
        player->unk2A6 = 6;
        player->stateFlags |= 0x800;
        break;

      case 7:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x14;
        player->unk2A6 = 7;
        player->stateFlags |= 0x800;
        break;

      case 8:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x15;
        player->unk2A6 = 8;
        player->stateFlags |= 0x800;
        break;

      case 9:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x1F;
        player->unk2A6 = 0xB;
        player->stateFlags |= 0x800;
        break;

      case 10:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x20;
        player->unk2A6 = 0xB;
        player->stateFlags |= 0x800;
        break;

      case 11:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x21;
        player->unk2A6 = 0xC;
        player->stateFlags |= 0x800;
        break;

      case 12:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x22;
        player->unk2A6 = 0xD;
        player->stateFlags |= 0x800;
        break;

      case 13:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x23;
        player->unk2A6 = 0xA;
        player->stateFlags |= 0x800;
        break;

      case 14:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x24;
        player->unk2A6 = 0xC;
        player->stateFlags |= 0x800;
        break;

      case 15:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x25;
        player->unk2A6 = 0xB;
        player->stateFlags |= 0x800;
        break;

      case 16:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x26;
        player->unk2A6 = 0xD;
        player->stateFlags |= 0x800;
        break;

      case 17:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x27;
        player->unk2A6 = 0xE;
        player->stateFlags |= 0x800;
        break;

      case 18:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x29;
        player->unk2A6 = 9;
        player->stateFlags |= 0x800;
        break;

      case 19:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x2A;
        player->unk2A6 = 0xA;
        player->stateFlags |= 0x800;
        break;

      case 20:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x2B;
        player->unk2A6 = 0xC;
        player->stateFlags |= 0x800;
        break;

      case 21:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x2C;
        player->unk2A6 = 9;
        player->stateFlags |= 0x800;
        break;

      case 22:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x2D;
        player->unk2A6 = 0xA;
        player->stateFlags |= 0x800;
        break;

      case 23:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x2E;
        player->unk2A6 = 0xE;
        player->stateFlags |= 0x800;
        break;

      case 24:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x2F;
        player->unk2A6 = 9;
        player->stateFlags |= 0x800;
        break;

      case 25:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x30;
        player->unk2A6 = 0xB;
        player->stateFlags |= 0x800;
        break;

      case 26:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x31;
        player->unk2A6 = 0xD;
        player->stateFlags |= 0x800;
        break;

      case 27:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x32;
        player->unk2A6 = 9;
        player->stateFlags |= 0x800;
        break;

      case 28:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x33;
        player->unk2A6 = 0xA;
        player->stateFlags |= 0x800;
        break;

      case 29:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x34;
        player->unk2A6 = 0xC;
        player->stateFlags |= 0x800;
        break;

      case 30:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x35;
        player->unk2A6 = 0xA;
        player->stateFlags |= 0x800;
        break;

      case 31:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x36;
        player->unk2A6 = 0xC;
        player->stateFlags |= 0x800;
        break;

      case 32:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x37;
        player->unk2A6 = 9;
        player->stateFlags |= 0x800;
        break;

      case 33:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x38;
        player->unk2A6 = 0xA;
        player->stateFlags |= 0x800;
        break;

      case 34:
        if (player->soundDisabled == 0)
      {
        enqueuePositionalSoundEffect(0x17, (SoundPosition *) (&player->pos), 0x7F, 0x32);
        enqueueRacePlayerVoiceSound(player, 1);
      }
        player->mode = 0x39;
        player->unk2A6 = 0xD;
        player->stateFlags |= 0x800;
        break;

      case 0:

      default:
        player->mode = 1;
        player->unk2A6 = 0;
        break;

    }

    player->updateState = 0;
    player->updateTimer = 0;
    player->unk40.y += player->stateTimer;
    player->pos.y += 0x60000;
    player->unk74 = player->unk40.y;
    player->stateFlags |= 0x208;
    setRaceMotionAnimation(player, 4);
  }
  else
  {
    updateRacePlayerSurfaceCue(player);
  }
  player->pos.x += player->unk40.x;
  player->pos.y += player->unk40.y;
  player->pos.z += player->unk40.z;
  stepRaceMotionJointAnimationUntilEnd(player);
  updateRacePlayerMotionFeedback(player);
  if (player->unk517 != 0)
  {
    enqueueRacePlayerVoiceSound(player, 5);
  }
}
#endif

void resetRacePlayerTrickSubstate(RacePlayer *player) {
    player->subState = 0;
    player->subStateTimer = 0;
    player->subStateStep = 0;
    player->subStateParam = 0;
    player->unk2A0 = 0;
}

void updateRacePlayerTrickSubstate(RacePlayer *player) {
    gRacePlayerTrickSubstateHandlers[player->subState](player);
}

void updateRacePlayerTrickSubstateStart(RacePlayer *player) {
    s16 inputMask;

    player->subStateTimer = 0;
    player->subStateStep = 0;
    player->unk2A2 = 0;
    player->unk2A4 = 0;
    if (player->unk4 == 0) {
        if (player->currentInputFlags & 1) {
            player->subStateTimer = 1;
            player->unk2A2 = 1;
        }
        if (player->currentInputFlags & 2) {
            player->subStateTimer = 2;
            player->unk2A2 = 3;
        }
        if (player->currentInputFlags & 8) {
            player->subStateTimer = 8;
            player->unk2A2 = 7;
        }
        if (player->currentInputFlags & 4) {
            player->subStateTimer = 4;
            player->unk2A2 = 5;
        }
        if (player->subStateTimer != 0) {
            if (player->unk2A0 < 6) {
                player->unk2A2++;
            }
            player->subState++;
            if (player->stateFlags & 0x400) {
                inputMask = player->subStateTimer;
                if (inputMask & 1) {
                    setRaceMotionAnimation(player, 0x17);
                    inputMask = player->subStateTimer;
                }
                if (inputMask & 2) {
                    setRaceMotionAnimation(player, 0x15);
                    inputMask = player->subStateTimer;
                }
            } else {
                inputMask = player->subStateTimer;
                if (inputMask & 1) {
                    setRaceMotionAnimation(player, 0x15);
                    inputMask = player->subStateTimer;
                }
                if (inputMask & 2) {
                    setRaceMotionAnimation(player, 0x17);
                    inputMask = player->subStateTimer;
                }
            }
            if (inputMask & 8) {
                setRaceMotionAnimation(player, 0x19);
                inputMask = player->subStateTimer;
            }
            if (inputMask & 4) {
                setRaceMotionAnimation(player, 0x1B);
            }
            player->stateFlags |= 0x800;
        }
        player->unk2A0++;
    }
}

void updateRacePlayerTrickSubstateHold(RacePlayer *player) {
    s16 inputMask;

    if (!(player->inputFlags & player->subStateTimer)) {
        player->subStateStep = 1;
    }
    if (stepRaceMotionAnimationUntilEnd(player) != 0) {
        if (player->subStateStep != 0) {
            player->subState++;
            if (player->stateFlags & 0x400) {
                inputMask = player->subStateTimer;
                if (inputMask & 1) {
                    setRaceMotionAnimation(player, 0x18);
                    inputMask = player->subStateTimer;
                }
                if (inputMask & 2) {
                    setRaceMotionAnimation(player, 0x16);
                    inputMask = player->subStateTimer;
                }
            } else {
                inputMask = player->subStateTimer;
                if (inputMask & 1) {
                    setRaceMotionAnimation(player, 0x16);
                    inputMask = player->subStateTimer;
                }
                if (inputMask & 2) {
                    setRaceMotionAnimation(player, 0x18);
                    inputMask = player->subStateTimer;
                }
            }
            if (inputMask & 8) {
                setRaceMotionAnimation(player, 0x1A);
                inputMask = player->subStateTimer;
            }
            if (inputMask & 4) {
                setRaceMotionAnimation(player, 0x1C);
            }
        } else {
            player->unk2A4++;
        }
    }
    player->stateFlags |= 0x800;
}

void updateRacePlayerTrickSubstateFinish(RacePlayer *player) {
    if (stepRaceMotionAnimationUntilEnd(player) != 0) {
        player->subState++;
    }
    player->stateFlags |= 0x800;
}

void updateRacePlayerTrickSubstateNoop(s32 arg0) {

}

void updateRacePlayerMode13AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 stateTimer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
    }

    if (player->subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }

    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    player->unk6C = (s16) ((s32) (fixedSine(player->stateTimerLow) * 0x1000) / 0x1000);
    player->stateTimer += 0x20;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }
    stateTimer = player->stateTimer;
    player->stateFlags |= 2;
    if (stateTimer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode15AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 stateTimer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
    }

    if (player->subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }

    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;

    player->unk6C = (s16) ((s32) (fixedSine(player->unk7E) * -0x1000) / 0x1000);
    player->stateTimer += 0x20;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }
    stateTimer = player->stateTimer;
    player->stateFlags |= 2;
    if (stateTimer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode16AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
    }
    if (player->subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }
    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);
    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;
    if (player->stateFlags & 0x400) {
        player->unk6E = (s16) ((s32) (fixedSine(player->stateTimerLow) << 0xC) / 4096);
    } else {
        player->unk6E = (s16) ((s32) (fixedSine(player->stateTimerLow) * -0x1000) / 4096);
    }

    player->stateTimer += 0x1E;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }
    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode17AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
    }
    if (player->subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }
    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;
    if (player->stateFlags & 0x400) {
        player->unk6E = (s16) ((s32) (fixedSine(player->unk7E) * -0x1000) / 0x1000);
    } else {
        player->unk6E = (s16) ((s32) (fixedSine(player->unk7E) << 12) / 0x1000);
    }

    player->stateTimer += 0x1D;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }
    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode18AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
    }
    if (player->subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }
    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);
    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;
    player->unk6C = (s16) ((s32) (fixedSine((s16) player->stateTimer) << 12) / 0x1000);
    if (player->stateFlags & 0x400) {
        player->unk6E = (s16) ((s32) (fixedSine((s16) player->stateTimer) << 12) / 0x1000);
    } else {
        player->unk6E = (s16) ((s32) (fixedSine((s16) player->stateTimer) * -0x1000) / 0x1000);
    }

    player->stateTimer += 0x1A;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }
    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode19AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
    }
    if (player->subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }
    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;
    player->unk6C = (s16) ((s32) (fixedSine(player->unk7E) << 12) / 0x1000);
    if (player->stateFlags & 0x400) {
        player->unk6E = (s16) ((s32) (fixedSine(player->unk7E) * -0x1000) / 0x1000);
    } else {
        player->unk6E = (s16) ((s32) (fixedSine(player->unk7E) << 12) / 0x1000);
    }

    player->stateTimer += 0x1A;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }
    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode20AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
    }
    if (player->subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }
    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;
    player->unk6C = (s16) ((s32) (fixedSine(player->unk7E) * -0x1000) / 0x1000);
    if (player->stateFlags & 0x400) {
        player->unk6E = (s16) ((s32) (fixedSine(player->unk7E) << 12) / 0x1000);
    } else {
        player->unk6E = (s16) ((s32) (fixedSine(player->unk7E) * -0x1000) / 0x1000);
    }

    player->stateTimer += 0x1A;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }
    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode21AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
    }
    if (player->subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }
    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);
    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;
    player->unk6C = (s16) ((s32) (fixedSine(player->stateTimerLow) * -0x1000) / 0x1000);
    if (player->stateFlags & 0x400) {
        player->unk6E = (s16) ((s32) (fixedSine(player->stateTimerLow) * -0x1000) / 0x1000);
    } else {
        player->unk6E = (s16) ((s32) (fixedSine(player->stateTimerLow) << 12) / 0x1000);
    }

    player->stateTimer += 0x1A;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }
    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode31AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x15);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (fixedSine(player->stateTimerLow) * -0x2000) / 4096;
    } else {
        player->unk6E = (fixedSine(player->stateTimerLow) * 0x2000) / 4096;
    }

    player->stateTimer += 0x16;
    player->updateTimer++;
    updateTimer = player->updateTimer;
    if (updateTimer == 8) {
        setRaceMotionAnimation(player, 0x16);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0xF) {
        setRaceMotionAnimation(player, 0x1B);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x1C);
    }

    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode41AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;

    player->unk6C = (fixedSine(player->unk7E) << 13) / 0x1000;

    player->stateTimer += 0x14;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode42AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;

    player->unk70 = (fixedSine(player->unk7E) << 13) / 0x1000;

    player->stateTimer += 0x14;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode43AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);

    updateTimer = player->updateTimer;
    if (updateTimer < 8) {
        player->pitchAngle = -updateTimer << 7;
    }
    if (updateTimer >= 0x23) {
        player->pitchAngle = (-(-updateTimer) << 6) - 0xC80;
    }
    if (updateTimer < 0x32) {
        player->updateTimer = updateTimer + 1;
    }

    player->velocity.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->pos.x += player->velocity.x;
    player->pos.y += yVel;
    player->pos.z += player->velocity.z;
    player->unk74 = yVel;

    player->unk6E = (fixedSine(player->unk7E) << 14) / 0x1000;

    player->stateTimer += 0x14;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode32AerialTrick(RacePlayer *player) {
    s16 updateState;
    s32 yVel;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x28);
        player->stateTimer = 0;
        player->stateFlags |= 0x200;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
    }

    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (stepRaceMotionAnimationUntilEnd(player) == 0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode33AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 0x23);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    player->unk6C = (fixedSine(player->stateTimerLow) * -0x3000) / 4096;
    if (player->stateFlags & 0x400) {
        player->unk6E = -player->updateTimer;
    } else {
        player->unk6E = player->updateTimer;
    }
    if (player->updateTimer != 0x1000) {
        player->updateTimer += 0x80;
    }

    player->stateTimer += 0x11;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode34AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 0x23);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = player->unk40.y;
    player->pos.x += player->unk40.x;
    player->pos.y += yVel;
    player->pos.z += player->unk40.z;
    player->unk6C = player->stateTimer * 4;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (fixedSine((s16)player->stateTimer) * -0x4000) / 0x1000;
    } else {
        player->unk6E = (fixedSine((s16)player->stateTimer) << 0xE) / 0x1000;
    }

    player->stateTimer += 0x15;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode44AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x17);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (fixedSine(player->stateTimerLow) * -0x2000) / 4096;
    } else {
        player->unk6E = (fixedSine(player->stateTimerLow) * 0x2000) / 4096;
    }

    player->stateTimer += 0x16;
    player->updateTimer++;
    if (player->updateTimer == 8) {
        setRaceMotionAnimation(player, 0x18);
    }
    if (player->updateTimer == 0xF) {
        setRaceMotionAnimation(player, 0x19);
    }
    if (player->updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x1A);
    }

    timer = player->stateTimer;
    if (timer >= 0x401) {
        timer = 0x400;
        player->stateTimer = timer;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode45AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;

    player->updateTimer = (fixedSine(player->unk7E) << 13) / 0x1000;
    updateTimer = player->updateTimer;
    if (updateTimer >= 0x1001) {
        player->unk6E = 0x2000 - updateTimer;
    } else {
        player->unk6E = updateTimer;
    }

    if (player->stateFlags & 0x400) {
        player->unk6E = -player->unk6E;
    }

    player->unk6C = (fixedSine(player->unk7E) << 13) / 0x1000;
    player->stateTimer += 0x14;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode46AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x1B);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (fixedSine(player->unk7E) * -0x5000) / 0x1000;
    } else {
        player->unk6E = (fixedSine(player->unk7E) * 0x5000) / 0x1000;
    }

    player->stateTimer += 0x12;
    player->updateTimer++;
    if (player->updateTimer == 0x28) {
        setRaceMotionAnimation(player, 0x1C);
    }

    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode35AerialTrick(RacePlayer *player) {
    s16 updateState;
    s32 yVel;
    s32 stateTimer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x15);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk70 = (fixedSine(player->unk7E) * -0x2000) / 0x1000;
    } else {
        player->unk70 = (fixedSine(player->unk7E) << 13) / 0x1000;
    }

    player->stateTimer += 0x16;
    player->updateTimer++;
    if (player->updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x16);
    }

    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    stateTimer = player->stateTimer;
    player->stateFlags |= 2;
    if (stateTimer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode36AerialTrick(RacePlayer *player) {
    s16 updateState;
    s32 yVel;
    s32 timer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x15);
        player->updateTimer = 0;
    }
    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (s16) ((s32) (fixedSine(player->stateTimerLow) * -0x3000) / 4096);
    } else {
        player->unk6E = (s16) ((s32) (fixedSine(player->stateTimerLow) * 0x3000) / 4096);
    }

    player->stateTimer += 0x15;
    player->updateTimer++;
    if (player->updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x16);
    }

    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode47AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x19);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (fixedSine(player->stateTimerLow) * -0x2000) / 4096;
    } else {
        player->unk6E = (fixedSine(player->stateTimerLow) << 0xD) / 4096;
    }

    player->stateTimer += 0x16;
    player->updateTimer++;
    updateTimer = player->updateTimer;
    if (updateTimer == 8) {
        setRaceMotionAnimation(player, 0x1A);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0xF) {
        setRaceMotionAnimation(player, 0x15);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x16);
    }

    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode48AerialTrick(RacePlayer *player) {
    s16 updateState;
    s32 yVel;
    s32 stateTimer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x15);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk70 = (fixedSine(player->stateTimerLow) * 0x3000) / 0x1000;
    } else {
        player->unk70 = (fixedSine(player->stateTimerLow) * -0x3000) / 0x1000;
    }

    player->stateTimer += 0x16;
    if (player->updateTimer < 0x2D) {
        player->updateTimer = player->updateTimer + 1;
    }

    if (player->updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x16);
    }

    stateTimer = player->stateTimer;
    player->unk6E = (player->updateTimer << 0xC) / 45;
    if (stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    stateTimer = player->stateTimer;
    player->stateFlags |= 2;
    if (stateTimer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode49AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    player->unk6C = (fixedSine(player->stateTimerLow) * -0x3000) / 0x1000;

    player->stateTimer += 0x12;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode37AerialTrick(RacePlayer *player) {
    s16 updateState;
    s16 updateTimer;
    s16 tilt;
    s16 *updateTimerPtr;
    s32 yVel;
    s32 timer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x23);
        player->stateTimer = 0;
        player->stateFlags |= 0x200;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
        player->unk306 = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = (u32) player->unk40.y;
    player->posX += player->unk40.x;
    updateTimerPtr = &player->updateTimer;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk70 = (fixedSine(player->stateTimerLow) * -0x2000) / 4096;
    } else {
        player->unk70 = (fixedSine(player->stateTimerLow) << 0xD) / 4096;
    }

    player->stateTimer += 0x16;
    player->updateTimer++;
    updateTimer = player->updateTimer;
    if (updateTimer == 0xC) {
        setRaceMotionAnimation(player, 0x15);
        updateTimer = *updateTimerPtr;
    }

    if (updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x16);
        updateTimer = player->updateTimer;
    }

    if ((updateTimer >= 9) && (updateTimer < 0x14)) {
        tilt = player->unk306;
        if (tilt != -0x400) {
            player->unk306 = tilt - 0x80;
            updateTimer = player->updateTimer;
        }
    }

    tilt = player->unk306;
    if ((updateTimer >= 0x1F) && (tilt != 0)) {
        player->unk306 = tilt + 0x80;
        tilt = player->unk306;
    }

    if (player->stateFlags & 0x400) {
        player->unk6E = -tilt;
    } else {
        player->unk6E = tilt;
    }

    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode50AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x19);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (fixedSine(player->stateTimerLow) * -0x2000) / 4096;
    } else {
        player->unk6E = (fixedSine(player->stateTimerLow) * 0x2000) / 4096;
    }

    player->stateTimer += 0x16;
    player->updateTimer++;
    updateTimer = player->updateTimer;
    if (updateTimer == 8) {
        setRaceMotionAnimation(player, 0x1A);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0xF) {
        setRaceMotionAnimation(player, 0x17);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x18);
    }

    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode51AerialTrick(RacePlayer *player) {
    s16 updateState;
    s16 updateTimer;
    RacePlayer *playerAlias;
    s16 tilt;
    s32 yVel;
    s32 timer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x23);
        player->stateTimer = 0;
        player->stateFlags |= 0x200;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
        player->unk306 = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    playerAlias = player;
    playerAlias->unk40.y -= playerAlias->unk264;
    clampRacePlayerVectorXZSpeed(&playerAlias->unk40, player);

    yVel = player->unk40.y;
    playerAlias->posX += player->unk40.x;
    player->posY += yVel;
    playerAlias->posZ += player->unk40.z;
    playerAlias->unk74 = yVel;

    if (playerAlias->stateFlags & 0x400) {
        playerAlias->unk70 = (fixedSine((s16) player->stateTimer) * 0x3000) / 4096;
    } else {
        player->unk70 = (fixedSine((s16) player->stateTimer) * -0x3000) / 4096;
    }

    playerAlias->stateTimer += 0x14;
    player->updateTimer++;
    updateTimer = player->updateTimer;
    if (updateTimer == 0xC) {
        setRaceMotionAnimation(playerAlias, 0x15);
        updateTimer = playerAlias->updateTimer;
    }

    if (updateTimer == 0x1E) {
        setRaceMotionAnimation(playerAlias, 0x16);
        updateTimer = playerAlias->updateTimer;
    }

    if ((updateTimer >= 9) && (updateTimer < 0x14)) {
        tilt = player->unk306;
        if (tilt != 0x400) {
            player->unk306 = tilt + 0x80;
            updateTimer = playerAlias->updateTimer;
        }
    }

    tilt = player->unk306;
    if ((updateTimer >= 0x1F) && (tilt != 0)) {
        player->unk306 = tilt - 0x80;
        tilt = player->unk306;
    }

    if (playerAlias->stateFlags & 0x400) {
        playerAlias->unk6E = -tilt;
    } else {
        player->unk6E = tilt;
    }

    timer = player->stateTimer;
    if (timer >= 0x401) {
        timer = 0x400;
        playerAlias->stateTimer = timer;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        playerAlias->stateFlags |= 0x800;
        if ((playerAlias->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) playerAlias->playerIndex);
        }
    }
}

void updateRacePlayerMode52AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 stateTimer;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x1B);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (fixedSine(player->stateTimerLow) * -0x3000) / 0x1000;
    } else {
        player->unk6E = (fixedSine(player->stateTimerLow) * 0x3000) / 0x1000;
    }

    player->stateTimer += 0x16;
    player->updateTimer++;
    if (player->updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x1C);
    }

    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode38AerialTrick(RacePlayer *player) {
    s16 updateState;
    s32 yVel;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x23);
        player->stateTimer = 0;
        player->stateFlags |= 0x200;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
    }

    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (stepRaceMotionAnimationUntilEnd(player) == 0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode53AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x19);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (fixedSine(player->stateTimerLow) * -0x1800) / 4096;
    } else {
        player->unk6E = ((fixedSine(player->stateTimerLow) * 6) * 1024) / 4096;
    }

    player->stateTimer += 0x16;
    player->updateTimer++;
    updateTimer = player->updateTimer;
    if (updateTimer == 8) {
        setRaceMotionAnimation(player, 0x1A);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0xF) {
        setRaceMotionAnimation(player, 0x1B);
        updateTimer = player->updateTimer;
    }
    if (updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x1C);
    }

    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode54AerialTrick(RacePlayer *player) {
    s16 updateState;
    s32 yVel;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x28);
        player->stateTimer = 0;
        player->stateFlags |= 0x200;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
    }

    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (stepRaceMotionAnimationUntilEnd(player) == 0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode39AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
        player->updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = player->unk40.y;
    updateTimer = player->updateTimer;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    switch (updateTimer) {
    case 0:
        player->unk6C = (s16) ((s32) (fixedSine(player->stateTimerLow) << 12) / 4096);
        player->stateTimer += 0x33;
        if (player->stateTimer >= 0x401) {
            player->stateTimer = 0;
            player->updateTimer++;
            player->unk306 = 0;
        }

        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
        break;
    case 1:
        if (player->stateFlags & 0x400) {
            player->unk6E = (s16) ((s32) (fixedSine(player->stateTimerLow) * -0x2000) / 4096);
        } else {
            player->unk6E = (s16) ((s32) (fixedSine(player->stateTimerLow) << 13) / 4096);
        }

        player->unk6C = -player->unk306;
        player->stateTimer += 0x20;
        if (player->stateTimer >= 0x401) {
            player->stateTimer = 0x400;
        }

        if (player->unk306 != 0x1000) {
            player->unk306 += 0x80;
        }

        if (player->stateTimer < 0x3D0) {
            player->stateFlags |= 0x800;
            if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
                createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
            }
        }
        break;
    }

    player->stateFlags |= 2;
}

void updateRacePlayerMode55AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (s16) ((s32) (fixedSine(player->stateTimerLow) * -0x2000) / 4096);
    } else {
        player->unk6E = (s16) ((s32) (fixedSine(player->stateTimerLow) << 0xD) / 4096);
    }

    player->stateTimer += 0x16;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode56AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->velocity.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);

    yVel = player->velocity.y;
    player->posX += player->velocity.x;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->unk74 = yVel;

    if (player->stateFlags & 0x400) {
        player->unk6E = (fixedSine(player->unk7E) * -0x5000) / 0x1000;
    } else {
        player->unk6E = (fixedSine(player->unk7E) * 0x5000) / 0x1000;
    }

    player->stateTimer += 0x14;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode57AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->updateState++;
        player->stateFlags |= 0x200;
        player->stateTimer = 0;
        resetRacePlayerTrickSubstate(player);
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->unk40.y -= player->unk264;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);

    yVel = player->unk40.y;
    player->posX += player->unk40.x;
    player->posY += yVel;
    player->posZ += player->unk40.z;
    player->unk74 = yVel;

    player->unk6C = (fixedSine(player->stateTimer) * -0x5000) / 0x1000;

    player->stateTimer += 0x14;
    if (player->stateTimer >= 0x401) {
        player->stateTimer = 0x400;
    }

    timer = player->stateTimer;
    player->stateFlags |= 2;
    if (timer < 0x3D0) {
        player->stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerLandingSnowSpray, 5, 2, (u16) player->playerIndex);
        }
    }
}

void updateRacePlayerMode03Nudge(RacePlayer *player) {
    RacePlayer *playerAlias;
    Struct800955C0 *spawn;
    s16 updateState;
    s16 angleDiff;
    s32 clamped;
    s32 tempX;
    s32 tempZ;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x2000;
        if (((player->unk2DE - player->facingAngle) & 0xFFF) < 0x800) {
            setRaceMotionAnimation(player, 7);
        } else {
            setRaceMotionAnimation(player, 6);
        }
        player->unk40.x = player->unk2E0;
        player->unk40.z = player->unk2E4;
        player->actionEffectLevel = 3;
        player->actionEffectFrame = 0;
    }

    updateRacePlayerLeanAngle(player, 0x10000, 0);
    playerAlias = player;
    player->unk314 = 0x60000;
    if (!(playerAlias->stateFlags & 1)) {
        playerAlias->stateFlags &= ~0x200;
        playerAlias->unk40.y -= 0xA000;
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, (unsigned long long) 0x2000, 0x2000, 0x2000);
    } else {
        playerAlias->stateFlags |= 0x200;
        player->unk40.y -= 0x7000;
        clampRacePlayerVectorXZSpeed(&player->unk40, player);

        tempX = playerAlias->unk40.x;
        clamped = tempX;
        if (clamped >= 0x2001) {
            clamped = 0x2000;
        }
        updateState = -0x2000;
        if (clamped < -0x2000) {
            clamped = updateState;
        }
        player->unk40.x = tempX - clamped;

        tempZ = playerAlias->unk40.z;
        clamped = tempZ;
        if (clamped >= 0x2001) {
            clamped = 0x2000;
        }
        if (clamped < updateState) {
            clamped = updateState;
        }
        player->unk40.z = tempZ - clamped;
    }

    playerAlias->posX += playerAlias->unk40.x;
    playerAlias->posY += player->unk40.y;
    playerAlias->posZ += playerAlias->unk40.z;

    spawn = &gRaceCourseStartEntries[gRaceCourseIndex];
    if ((spawn->unk0 == playerAlias->unk502) && !(playerAlias->stateFlags & 0x40)) {
        angleDiff = calculateFixedAngleBetweenXZPoints(playerAlias->posX, player->posZ, spawn->unk40, spawn->unk44);
        if (playerAlias->stateFlags & 0x400) {
            angleDiff += 0x800;
        }

        angleDiff = (angleDiff - player->facingAngle) & 0xFFF;
        if (angleDiff >= 0x801) {
            angleDiff -= 0x1000;
        }
        if (angleDiff >= 0x97) {
            angleDiff = 0x96;
        }
        if (angleDiff < -0x96) {
            angleDiff = -0x96;
        }
        playerAlias->facingAngle += angleDiff;
    }

    if (!(player->stateFlags & 1)) {
        updateRacePlayerMotionFeedback(player);
    }

    if (stepRaceMotionAnimationUntilEnd(player) != 0) {
        playerAlias->stateFlags &= 0xFE0C1FFB;
        if (playerAlias->stateFlags & 0x200) {
            player->mode = 1;
            playerAlias->updateState = 0;
            player->updateTimer = 0;
        } else {
            playerAlias->mode = 0;
            playerAlias->updateState = 0;
            playerAlias->updateTimer = 0;
        }
    }
}

void updateRacePlayerMode04Spinout(RacePlayer *player) {
    s16 updateState;
    s32 tempX;
    s32 tempZ;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0xE);
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x12204;
        player->pitchAngle = 0;
        updateRacePlayerLeanAngle(player, 0, 0);
        player->unk40.x = player->unk2E0;
        player->unk40.z = player->unk2E4;
        player->unk40.y = 0x20000;
        player->unk60 = 0;
        player->actionEffectLevel = 3;
        player->actionEffectFrame = 0;
    }

    player->unk314 = 0x60000;
    if (player->stateFlags & 1) {
        clampRacePlayerVectorXZSpeed(&player->unk40, player);
        tempX = player->unk40.x;
        tempZ = player->unk40.z;
        player->unk40.y -= 0xA000;
        player->unk40.x = tempX - (tempX / 15);
        player->unk40.z = tempZ - (tempZ / 15);
    } else {
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x3000, 0x3000, 0x3000);
    }

    player->posX += player->unk40.x;
    player->posY += player->unk40.y;
    player->posZ += player->unk40.z;

    if ((stepRaceMotionAnimationUntilEnd(player) != 0) && !(player->stateFlags & 1)) {
        player->mode = 5;
        player->updateState = 0;
        player->updateTimer = 0;
    }
}

void updateRacePlayerMode05SpinoutStun(RacePlayer *player) {
    s16 updateState;
    s32 stateTimer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateTimer = 0x1E;
        if (player->stateFlags & 0x800000) {
            player->stateTimer = 0x5A;
            spawnRaceUiStunOrbitingIcons(player->playerIndex);
        }
        setRaceMotionAnimation(player, 0xF);
        stateTimer = player->stateTimer;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags &= ~0x200;
        player->stateFlags |= 0x1012000;
        player->unk60 = 0;
        player->stateTimer = stateTimer;
        player->stateTimer = stateTimer - ((player->stateTimer * player->rankIndex) / 8);
        player->actionEffectLevel = 1;
        player->actionEffectFrame = 0;
    }

    updateRacePlayerLeanAngle(player, 0, 0);
    if (!(player->stateFlags & 1)) {
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x4000, 0x4000, 0x4000);
    }

    player->unk40.y -= 0xA000;
    player->posY += player->unk40.y;

    if (!(player->stateFlags & 1)) {
        player->stateTimer = player->stateTimer - getRaceInputTimerDecrementBonus(player) - 1;
        if (player->stateTimer < 0) {
            player->stateTimer = 0;
        }
        if (stepRaceMotionAnimationUntilEnd(player) != 0) {
            player->stateFlags &= ~0x200;
            if (player->stateTimer == 0) {
                player->mode = 8;
                player->updateState = 0;
                player->updateTimer = 0;
            }
        }
    } else {
        player->stateFlags |= 0x200;
        setRaceMotionAnimation(player, 0xF);
        stepRaceMotionAnimationUntilEnd(player);
        if (player->stateTimer < 0x1E) {
            player->stateTimer = 0x1E;
        }
        if (!(player->stateFlags & 1)) {
            player->actionEffectLevel = 1;
            player->actionEffectFrame = 0;
        }
    }
}

void updateRacePlayerMode08SpinoutRecover(RacePlayer *player) {
    Struct800955C0 *spawn;
    s16 updateState;
    s16 targetAngle;
    s16 facingAngle;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        if (player->stateFlags & 0x10000) {
            setRaceMotionAnimation(player, 0x11);
        } else {
            setRaceMotionAnimation(player, 0x10);
        }
        player->stateFlags = player->stateFlags & 0xFE0C1FFB;
        player->stateFlags = player->stateFlags & ~0x200;
        player->stateFlags = player->stateFlags | 0xA000;
        player->unk60 = 0x40000;
    }

    if (player->stateFlags & 1) {
        player->stateFlags |= 0x200;
    } else {
        player->stateFlags &= ~0x200;
    }

    spawn = &gRaceCourseStartEntries[gRaceCourseIndex];
    if ((spawn->unk0 == player->unk502) && !(player->stateFlags & 0x40)) {
        targetAngle = calculateFixedAngleBetweenXZPoints(player->posX, player->posZ, spawn->unk40, spawn->unk44);
        if (player->stateFlags & 0x400) {
            targetAngle += 0x800;
        }

        facingAngle = player->facingAngle;
        targetAngle = (targetAngle - facingAngle) & 0xFFF;
        if (targetAngle >= 0x801) {
            targetAngle -= 0x1000;
        }
        if (targetAngle >= 0x67) {
            targetAngle = 0x66;
        }
        if (targetAngle < -0x66) {
            targetAngle = -0x66;
        }
        player->facingAngle = facingAngle + targetAngle;
    }

    updateRacePlayerLeanAngle(player, 0, 0);
    player->unk40.y -= 0xA000;
    player->posY += player->unk40.y;

    if (stepRaceMotionAnimationUntilEnd(player) != 0) {
        player->stateFlags &= 0xFE0C1FFB;
        player->mode = 0;
        player->updateState = 0;
        player->updateTimer = 0;
    }
}

void updateRacePlayerMode06TerrainFall(RacePlayer *player) {
    if (player->updateState == 0) {
        player->updateState++;
        player->stateTimer = 0x3C;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x42000;
        player->updateTimer = 0;
        setRaceCameraMode(player->playerIndexU16, 4);
        player->unk60 = 0;
        player->unk2E8 = player->unk502;
        if (player->soundDisabled == 0) {
            spawnRaceItemBreakParticles(player->playerIndex, player->unk330);
        }
        player->stateTimer -= (player->stateTimer * player->rankIndex) / 8;
        player->actionEffectLevel = 2;
        player->actionEffectFrame = 0;
        setRaceMotionAnimation(player, 0x20);
    }

    switch (player->updateTimer) {
    case 0:
        player->stateTimer--;
        if (player->stateTimer == 0) {
            player->updateTimer++;
        }
        if (stepRaceMotionAnimationUntilEnd(player) != 0) {
            setRaceMotionAnimation(player, 0x21);
        }
        break;
    case 1:
        if (stepRaceMotionAnimationUntilEnd(player) != 0) {
            setRaceMotionAnimation(player, 0x21);
        }
        player->stateFlags |= 0x80000;
        if (gViewportStates[(u16) player->playerIndex].overlayAlpha == 0xFF) {
            player->updateTimer++;
            player->unk74 = 0;
            player->unk502 = player->unk2E8;
            do {
                getRaceCourseSurfaceSpawnTransform(player->unk502, &player->posX, &player->posY, &player->posZ,
                                                   &player->facingAngle);
                if (isRacePlayerRespawnSurfaceValid(player) == 0) {
                    player->unk502--;
                } else {
                    break;
                }
            } while (1);
            player->posY = getRaceCourseSurfaceHeight(player->unk502, player->posX, player->posZ);
            player->unk34 = player->pos;
            player->velocity.x = 0;
            player->velocity.y = 0;
            player->velocity.z = 0;
            player->stateFlags &= ~0x400;
            player->unk2EE = 0;
            updateRacePlayerLeanAngle(player, 0, 0);
            setRaceMotionAnimation(player, 1);
            stepRaceMotionAnimationUntilEnd(player);
            setRaceCameraMode(player->playerIndexU16, 1);
            player->unk60 = 0x40000;
        }
        break;
    case 2:
        player->stateFlags &= 0xFFF7FFFF;
        if (gViewportStates[(u16) player->playerIndex].overlayAlpha == 0) {
            player->mode = 0;
            player->updateState = 0;
            player->updateTimer = 0;
            player->stateFlags = 0;
        }
        stepRaceMotionAnimationUntilEnd(player);
        break;
    }

    player->velocity.y -= player->unk260;
    player->posY += player->velocity.y;
}

void updateRacePlayerMode28TerrainFallWithItemEffect(RacePlayer *player) {
    s16 updateState;

    player->unk40.y -= player->unk260;
    player->posY += player->unk40.y;
    updateState = player->updateState;

    switch (updateState) {
    case 0:
        player->updateState = updateState + 1;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x42000;
        setRaceMotionAnimation(player, 0xE);
        stepRaceMotionAnimationUntilEnd(player);
        player->unk80 = player->unk502 + 2;
        setRaceCameraMode(player->playerIndex, 4);
        player->unk60 = 0;
        if (player->soundDisabled == 0) {
            spawnRaceItemBreakParticles(player->playerIndex, player->unk330);
        }
        player->unk2E8 = player->unk502;
        player->actionEffectLevel = 2;
        player->actionEffectFrame = 0;
        /* fallthrough */
    case 1:
        if (stepRaceMotionAnimationUntilEnd(player) != 0) {
            player->updateState++;
            setRaceMotionAnimation(player, 0xF);
        }
        updateRacePlayerLeanAngle(player, player->unk254, 0);
        player->unk314 = 0x20000;
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x2000, 0x2000, 0x2000);
        player->posX += player->unk40.x;
        player->posY += player->unk40.y;
        player->posZ += player->unk40.z;
        break;
    case 2:
        if (stepRaceMotionAnimationUntilEnd(player) != 0) {
            player->updateState++;
            player->stateTimer = 0x3C - ((player->rankIndex * 0x3C) / 8);
            setRaceMotionAnimation(player, 0x20);
        }
        updateRacePlayerLeanAngle(player, player->unk254, 0);
        player->unk314 = 0x20000;
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x2000, 0x2000, 0x2000);
        player->posX += player->unk40.x;
        player->posY += player->unk40.y;
        player->posZ += player->unk40.z;
        break;
    case 3:
        player->stateTimer--;
        if (stepRaceMotionAnimationUntilEnd(player) != 0) {
            setRaceMotionAnimation(player, 0x21);
        }
        if (player->stateTimer == 0) {
            player->updateState++;
        }
        break;
    case 4:
        if (stepRaceMotionAnimationUntilEnd(player) != 0) {
            setRaceMotionAnimation(player, 0x21);
        }
        player->stateFlags |= 0x80000;
        if (gViewportStates[(u16) player->playerIndex].overlayAlpha == 0xFF) {
            player->updateState++;
            player->unk502 = player->unk2E8;
loop:
            getRaceCourseSurfaceSpawnTransform(player->unk502, &player->posX, &player->posY, &player->posZ, &player->facingAngle);
            if (isRacePlayerRespawnSurfaceValid(player) == 0) {
                player->unk502--;
                goto loop;
            }
            player->posY = getRaceCourseSurfaceHeight(player->unk502, player->posX, player->posZ);
            player->unk34 = player->pos;
            player->unk40.x = 0;
            player->unk40.y = 0;
            player->unk40.z = 0;
            player->stateFlags &= ~0x400;
            player->unk2EE = 0;
            updateRacePlayerLeanAngle(player, 0, 0);
            setRaceMotionAnimation(player, 1);
            stepRaceMotionAnimationUntilEnd(player);
            setRaceCameraMode(player->playerIndex, 1);
            player->unk60 = 0x40000;
        }
        break;
    case 5:
        player->stateFlags &= 0xFFF7FFFF;
        if (gViewportStates[(u16) player->playerIndex].overlayAlpha == 0) {
            player->mode = 0;
            player->updateState = 0;
            player->updateTimer = 0;
            stepRaceMotionAnimationUntilEnd(player);
            player->stateFlags = 0;
        }
        break;
    }
}

void updateRacePlayerMode09TerrainCrash(RacePlayer *player) {
    s16 sine;
    s16 cosine;
    s16 updateState;

    player->unk40.y -= player->unk260;
    player->posY += player->unk40.y;
    updateState = player->updateState;

    switch (updateState) {
    case 0:
        player->updateState = updateState + 1;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x42000;
        setRaceMotionAnimation(player, 0xE);
        stepRaceMotionAnimationUntilEnd(player);
        player->unk80 = player->unk502 + 2;
        setRaceCameraMode((u16)player->playerIndex, 4);
        player->unk60 = 0;
        if (player->soundDisabled == 0) {
            spawnRaceItemBreakParticles(player->playerIndex, player->unk330);
        }
        player->unk2E8 = player->unk502;
        player->actionEffectLevel = 2;
        player->actionEffectFrame = 0;
        /* fallthrough */
    case 1:
        if (stepRaceMotionAnimationUntilEnd(player) != 0) {
            player->updateState++;
            setRaceMotionAnimation(player, 0xF);
        }
        updateRacePlayerLeanAngle(player, player->unk254, 0);
        player->unk314 = 0x20000;
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x2000, 0x2000, 0x2000);
        player->posX += player->unk40.x;
        player->posY += player->unk40.y;
        player->posZ += player->unk40.z;
        break;
    case 2:
        if (stepRaceMotionAnimationUntilEnd(player) != 0) {
            player->updateState++;
            player->stateTimer = 0x3C - ((player->rankIndex * 0x3C) / 8);
            setRaceMotionAnimation(player, 0x20);
        }
        updateRacePlayerLeanAngle(player, player->unk254, 0);
        player->unk314 = 0x20000;
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x2000, 0x2000, 0x2000);
        player->posX += player->unk40.x;
        player->posY += player->unk40.y;
        player->posZ += player->unk40.z;
        break;
    case 3:
        if (stepRaceMotionAnimationUntilEnd(player) != 0) {
            setRaceMotionAnimation(player, 0x21);
        }
        player->stateTimer--;
        if (player->stateTimer == 0) {
            player->updateState++;
        }
        break;
    case 4:
        if (stepRaceMotionAnimationUntilEnd(player) != 0) {
            setRaceMotionAnimation(player, 0x21);
        }
        player->stateFlags |= 0x80000;
        if (gViewportStates[(u16)player->playerIndex].overlayAlpha == 0xFF) {
            player->updateState++;
            player->unk502 = player->unk2E8;
            getRaceCourseSurfaceSpawnTransform(player->unk80, &player->posX, &player->posY, &player->posZ, &player->facingAngle);
            player->posY = getRaceCourseSurfaceHeight(player->unk502, player->posX, player->posZ);
            sine = fixedSine(player->facingAngle);
            cosine = fixedCosine(player->facingAngle);
            player->posX += ((s64)sine * -0x240000) / 0x1000;
            player->posZ += ((s64)cosine * -0x240000) / 0x1000;
            player->unk34 = player->pos;
            player->unk40.x = 0;
            player->unk40.y = 0;
            player->unk40.z = 0;
            player->unk74 = 0;
            player->stateFlags &= ~0x400;
            player->unk2EE = 0;
            updateRacePlayerLeanAngle(player, 0, 0);
            setRaceMotionAnimation(player, 1);
            stepRaceMotionAnimationUntilEnd(player);
            setRaceCameraMode((u16)player->playerIndex, 1);
            player->unk60 = 0x40000;
        }
        break;
    case 5:
        player->stateFlags &= 0xFFF7FFFF;
        if (gViewportStates[(u16)player->playerIndex].overlayAlpha == 0) {
            player->mode = 0;
            player->updateState = 0;
            player->updateTimer = 0;
            stepRaceMotionAnimationUntilEnd(player);
            player->stateFlags = 0;
        }
        break;
    }
}

void updateRacePlayerMode10TerrainCrashSlide(RacePlayer *player) {
    s16 updateState;
    s32 yVel;
    s32 velocityX;
    s32 velocityZ;
    u32 stateFlags;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        player->stateFlags &= ~0x200;
        player->stateFlags &= 0xFE0C1FFB;
        (*player).stateFlags |= 0x01006000;
        if (player->animationId != 0x12) {
            setRaceMotionAnimation(player, 0x12);
        }
        player->stateTimer = 0x1E;
        player->unk60 = 0;
    }
    stepRaceMotionLoopingAnimation(player);
    updateRacePlayerLeanAngle(player, player->unk254, 0);
    player->velocity.y += 0xFFFF6000;
    player->unk314 = 0x80000;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    stateFlags = player->stateFlags;
    if ((stateFlags & 1) == 0) {
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0, 0, 0);
        player->unk582 = 0x100;
        player->unk584 = 8;
        player->unk588 = 0.0f;
        stateFlags = player->stateFlags;
    }
    if ((stateFlags & 1) != 0) {
        player->stateFlags = stateFlags | 0x200;
    } else {
        player->stateFlags = stateFlags & ~0x200;
    }
    yVel = player->velocity.y;
    velocityX = player->velocity.x;
    velocityZ = player->velocity.z;
    player->posY += yVel;
    player->unk74 = 0;
    player->posX += velocityX;
    player->posY += yVel;
    player->posZ += player->velocity.z;
    player->facingAngle = calculateFixedAngleFromDeltaXZ(velocityX, velocityZ);
    if (player->stateFlags & 0x400) {
        player->facingAngle += 0x800;
    }
    if (player->unk330 != 3) {
        if (--player->stateTimer == 0) {
            player->stateFlags &= 0xFE0C1FFB;
            if (player->stateFlags & 1) {
                player->mode = 1;
                player->updateState = 0;
                player->updateTimer = 0;
            } else {
                player->mode = 0;
                player->updateState = 0;
                player->updateTimer = 0;
            }
        }
    } else {
        player->stateTimer = 0x1E;
    }
    player->actionEffectLevel = 4;
    player->actionEffectFrame = 2;
}

void updateRacePlayerMode12LaunchRecover(RacePlayer *player) {
    s16 updateState;
    u32 stateFlags;
    s32 grounded;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x14);
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags &= ~0x200;
        player->stateFlags |= 0x1022000;
        player->unk60 = 0;
        player->unk80 = 0;
        player->stateTimer = 0x1E - ((player->rankIndex * 0x1E) / 8);
        player->actionEffectLevel = 1;
        player->actionEffectFrame = 0;
    }

    updateRacePlayerLeanAngle(player, 0, 0);
    player->unk40.y += 0xFFFF6000;
    grounded = player->stateFlags & 1;
    if (grounded == 0) {
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x6000, 0x6000, 0x6000);
        grounded = player->stateFlags & 1;
    }

    player->posX += player->unk40.x;
    player->posY += player->unk40.y;
    player->posZ += player->unk40.z;

    if (grounded == 0) {
        updateRacePlayerMotionFeedback(player);
        if ((player->unk584 != 6) && (player->unk584 != 0x1E)) {
            player->unk588 = -6.0f;
        }
    }

    stateFlags = player->stateFlags;
    if (!(stateFlags & 1)) {
        player->stateTimer = player->stateTimer - getRaceInputTimerDecrementBonus(player) - 1;
        if (player->stateTimer < 0) {
            player->stateTimer = 0;
        }
        if (stepRaceMotionAnimationUntilEnd(player)) {
            player->stateFlags &= ~0x200;
            if (player->stateTimer == 0) {
                player->mode = 8;
                player->updateState = 0;
                player->updateTimer = 0;
            }
        }
    } else {
        player->stateFlags = stateFlags | 0x200;
        setRaceMotionAnimation(player, 0x14);
        stepRaceMotionAnimationUntilEnd(player);
        player->stateTimer = 0x1E;
        player->unk80 = 7;
        if (!(player->stateFlags & 1)) {
            player->actionEffectLevel = 1;
            player->actionEffectFrame = 0;
        }
    }
}

void updateRacePlayerMode11LaunchHit(RacePlayer *player) {
    s16 updateState;
    s32 tempX;
    s32 tempZ;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x13);
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x22200;
        player->pitchAngle = 0;
        updateRacePlayerLeanAngle(player, 0, 0);
        player->unk60 = 0;
        player->actionEffectLevel = 3;
        player->actionEffectFrame = 0;
    }

    player->unk314 = 0x60000;
    clampRacePlayerVectorXZSpeed(&player->unk40, player);
    player->unk40.y -= player->unk264;
    if (!(player->stateFlags & 1)) {
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x4000, 0x4000, 0x4000);
    } else {
        tempX = player->unk40.x;
        tempZ = player->unk40.z;
        player->unk40.x = tempX - (tempX / 16);
        player->unk40.z = tempZ - (tempZ / 16);
    }

    player->posX += player->unk40.x;
    player->posY += player->unk40.y;
    player->posZ += player->unk40.z;

    if ((stepRaceMotionAnimationUntilEnd(player) != 0) && !(player->stateFlags & 1)) {
        player->mode = 0xC;
        player->updateState = 0;
        player->updateTimer = 0;
    }
}

void updateRacePlayerMode26Tumble(RacePlayer *player) {
    s16 updateState;
    s32 timer;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x12);
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x6204;
        player->pitchAngle = 0;
        updateRacePlayerLeanAngle(player, 0, 0);
        player->unk60 = 0;
        player->unk40.y = 0x80000;
        player->stateTimer = 0x14;
    }

    if (gFrameCounter & 1) {
        createCallbackTaskWithUserIdPreservingArgs(initRacePlayerRecoverySparkle, 5, 2, (u16) player->playerIndex);
    }

    player->unk40.y -= 0x7000;
    player->posY += player->unk40.y;
    stepRaceMotionLoopingAnimation(player);

    timer = player->stateTimer;
    if (timer == 0) {
        if (!(player->stateFlags & 1)) {
            player->mode = 0xC;
            player->updateState = 0;
            player->updateTimer = 0;
        }
    } else {
        player->stateTimer = timer - 1;
    }

    if (player->unk40.y > 0) {
        player->actionEffectLevel = 2;
        player->actionEffectFrame = 3;
    }
}

void updateRacePlayerMode14PushHit(RacePlayer *player) {
    s16 updateState;
    s32 velocityX;
    s32 velocityZ;
    u32 stateFlags;
    s16 nextState;

    updateState = player->updateState;
    nextState = updateState + 1;
    if (updateState == 0) {
        player->updateState = nextState;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x01006000;
        setRaceMotionAnimation(player, 0x12);
        player->stateTimer = 0xF;
        player->unk60 = 0;
    }
    stepRaceMotionLoopingAnimation(player);
    updateRacePlayerLeanAngle(player, 0x10000, 0);
    player->velocity.y += 0xFFFF6000;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    stateFlags = player->stateFlags;
    if ((stateFlags & 1) == 0) {
        player->stateFlags = stateFlags & ~0x200;
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0, 0, 0);
        player->unk582 = 0x100;
        player->unk584 = 8;
        player->unk588 = 0.0f;
    } else {
        player->stateFlags = stateFlags | 0x200;
    }
    velocityX = player->velocity.x;
    player->posX += velocityX;
    player->posY += player->velocity.y;
    velocityZ = player->velocity.z;
    player->posZ += velocityZ;
    player->facingAngle = calculateFixedAngleFromDeltaXZ(velocityX, velocityZ);
    if (player->stateFlags & 0x400) {
        player->facingAngle += 0x800;
    }
    if (--player->stateTimer == 0) {
        player->mode = 0xC;
        player->updateState = 0;
        player->updateTimer = 0;
    }
    player->actionEffectLevel = 4;
    player->actionEffectFrame = 2;
}

void updateRacePlayerMode23ItemSteal(RacePlayer *player) {
    s16 updateState;
    s16 nextState;
    s32 timer;
    u32 stateFlags;

    updateState = player->updateState;
    nextState = updateState + 1;
    if (updateState == 0) {
        player->updateState = nextState;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x102000;
        setRaceMotionAnimation(player, 0xE);
        player->stateTimer = 0x5A;
        player->unk60 = 0;
        stepRaceMotionLoopingAnimation(player);
        player->unk40.y = 0;
        createCallbackTaskWithUserIdPreservingArgs(initRaceUiItemStealTrailEffect, 0, 0x64, (u16) player->playerIndex);
        timer = player->stateTimer;
        player->stateTimer = timer - ((player->stateTimer * player->rankIndex) / 8);
        player->actionEffectLevel = 4;
        player->actionEffectFrame = 0;
    }

    stateFlags = player->stateFlags;
    if (!(stateFlags & 1)) {
        player->stateFlags = stateFlags & ~0x200;
    } else {
        player->stateFlags = stateFlags | 0x200;
    }

    updateRacePlayerLeanAngle(player, 0, 0);
    player->unk40.y -= 0xA000;
    player->posY += player->unk40.y;

    player->stateTimer -= getRaceInputTimerDecrementBonus(player) + 1;
    timer = player->stateTimer;
    if (timer < 0) {
        player->stateTimer = 0;
        timer = 0;
    }

    if (timer == 0) {
        player->mode = 4;
        player->updateState = 0;
        player->updateTimer = 0;
        player->unk2E0 = 0;
        player->unk2E4 = 0;
    }
}

void updateRacePlayerMode24HeavyKnockdown(RacePlayer *player) {
    s16 temp_v1_2;
    s16 temp_v0;
    s16 temp_2d6;
    s32 timer;
    s32 ret;
    u32 stateFlags;

    player->unk2EE = 0;
    updateRacePlayerLeanAngle(player, 0, 0);
    player->unk40.y += 0xFFFF6000;
    player->posY += player->unk40.y;

    stateFlags = player->stateFlags;
    if (!(stateFlags & 1)) {
        player->stateFlags = stateFlags & ~0x200;
    } else {
        player->stateFlags = stateFlags | 0x200;
    }

    temp_v1_2 = player->updateState;
    temp_v0 = temp_v1_2;
    switch (temp_v0 ^ 0) {
    case 0:
        player->updateState = temp_v1_2 + 1;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x1202000;
        player->unk2D6 = 1;
        setRaceMotionAnimation(player, 1);
        player->stateTimer = 0x5A;
        player->unk60 = 0;
        stepRaceMotionLoopingAnimation(player);
        timer = player->stateTimer;
        player->stateTimer = (timer ^ 0) - ((timer * player->rankIndex) / 8);
        player->unk40.y = 0;
        player->actionEffectLevel = 6;
        player->actionEffectFrame = 0;
        /* fallthrough */
    case 1:
        temp_2d6 = player->unk2D6;
        if (temp_2d6 != 1) {
            player->unk2D6 = temp_2d6 - 1;
        }

        ret = getRaceInputTimerDecrementBonus(player);
        if (ret != 0) {
            player->unk2D6 = 3;
        }

        player->stateTimer -= ret + 1;
        timer = player->stateTimer;
        if (timer < 0) {
            player->stateTimer = 0;
            timer = 0;
        }

        if (timer == 0) {
            player->updateState += 1;
            player->unk2D6 = 1;
        }
        break;
    case 2:
        player->unk2D6 += 1;
        if (player->unk2D6 == 7) {
            player->unk2D6 = 2;
            player->updateState += 1;
        }
        break;
    case 3:
        player->unk2D6 += 1;
        if (player->unk2D6 == 8) {
            player->stateFlags &= 0xFE0C1FFB;
            player->mode = 0;
            player->updateState = 0;
            player->updateTimer = 0;
            player->unk2D4 = 0x3C;
        }
        break;
    }
}

void updateRacePlayerMode25SpinHit(RacePlayer *player) {
    s16 updateState;
    s32 stateTimer;
    u32 stateFlags;
    s16 nextState;

    updateState = player->updateState;
    nextState = updateState + 1;
    if (updateState == 0) {
        player->updateState = nextState;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x400000;
        player->stateTimer = 0xF0;
        setRaceMotionAnimation(player, 1);
        stepRaceMotionLoopingAnimation(player);
        createCallbackTaskWithUserIdPreservingArgs(initRaceUiHeavyKnockdownTrailEffect, 0, 0x3C, (u16) player->playerIndex);
        stateTimer = player->stateTimer;
        player->stateTimer = stateTimer;
        player->stateTimer = player->stateTimer - ((stateTimer * player->rankIndex) / 8);
        player->actionEffectLevel = 4;
        player->actionEffectFrame = 0;
    }

    stateFlags = player->stateFlags;
    if (!(stateFlags & 1)) {
        player->stateFlags = stateFlags & ~0x200;
        updateRacePlayerLeanAngle(player, player->unk254, 0);
        player->unk40.y -= player->unk260;
        updateRacePlayerLocalVelocity(player, 0, 0, player->unk274, player->unk278, player->unk27C);
    } else {
        player->stateFlags = stateFlags | 0x200;
        player->unk40.y -= player->unk264;
        clampRacePlayerVectorXZSpeed(&player->unk40, player);
    }

    player->stateTimer -= getRaceInputTimerDecrementBonus(player) + 1;
    if (player->stateTimer < 0) {
        player->stateTimer = 0;
    }

    if (player->stateTimer == 0) {
        stateFlags = player->stateFlags;
        if (!(stateFlags & 0x200)) {
            player->stateFlags = stateFlags & 0xFE0C1FFB;
            player->mode = 0;
            player->updateState = 0;
            player->updateTimer = 0;
        } else {
            player->stateFlags = stateFlags & 0xFE0C1FFB;
            player->mode = 1;
            player->updateState = 0;
            player->updateTimer = 0;
        }
    }

    player->posX += player->unk40.x;
    player->posY += player->unk40.y;
    player->posZ += player->unk40.z;

    if (!(player->stateFlags & 1)) {
        updateRacePlayerMotionFeedback(player);
    }
}

void updateRacePlayerMode27Slide(RacePlayer *player) {
    s16 updateState;
    s16 updateTimer;
    u32 stateFlags;

    updateState = player->updateState;
    if (updateState == 0) {
        player->updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x13);
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x822204;
        player->pitchAngle = 0;
        player->unk60 = 0;
        player->unk40.y = 0x80000;
        player->updateTimer = 0;
        player->actionEffectLevel = 4;
        player->actionEffectFrame = 0;
    }

    updateRacePlayerLeanAngle(player, 0, 0);
    if (player->unk40.y > 0) {
        player->unk40.y -= 0x7000;
    } else {
        updateTimer = player->updateTimer;
        if (updateTimer == 0) {
            player->updateTimer = updateTimer + 1;
            createCallbackTaskWithUserIdPreservingArgs(initRaceUiSpinHitTransitionEffect, 0, 3, (u16) player->playerIndex);
        }
        player->facingAngle += 0x40;
        player->unk40.y -= 0x800;
    }

    player->unk40.x -= player->unk40.x >> 6;
    player->unk40.z -= player->unk40.z >> 6;
    player->posX += player->unk40.x;
    player->posY += player->unk40.y;
    player->posZ += player->unk40.z;

    stepRaceMotionAnimationUntilEnd(player);
    if (player->unk40.y < 0) {
        stateFlags = player->stateFlags;
        if (!(stateFlags & 1)) {
            player->mode = 8;
            player->updateState = 0;
            player->updateTimer = 0;
            player->stateFlags = stateFlags & ~0x200;
        }
    }
}

void updateRacePlayerMode40Stun(RacePlayer *player) {
    s16 updateState;
    s32 velocityX;
    s32 velocityZ;
    u32 stateFlags;
    s16 nextState;

    updateState = player->updateState;
    nextState = updateState + 1;
    if (updateState == 0) {
        player->updateState = nextState;
        player->stateFlags &= 0xFE0C1FFB;
        player->stateFlags |= 0x01006000;
        setRaceMotionAnimation(player, 0x12);
        player->stateTimer = 0x1E;
        player->stateTimer += randomNextMain() >> 4;
        player->unk60 = 0;
    }
    stepRaceMotionLoopingAnimation(player);
    updateRacePlayerLeanAngle(player, 0x10000, 0);
    player->velocity.y += 0xFFFF6000;
    clampRacePlayerVectorXZSpeed(&player->velocity, player);
    stateFlags = player->stateFlags;
    if ((stateFlags & 1) == 0) {
        player->stateFlags = stateFlags & ~0x200;
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0, 0, 0);
        player->stateFlags &= ~0x200;
        player->unk582 = 0x100;
        player->unk584 = 8;
        player->unk588 = 0.0f;
    } else {
        player->stateFlags = stateFlags | 0x200;
    }
    velocityX = player->velocity.x;
    player->posX += velocityX;
    player->posY += player->velocity.y;
    velocityZ = player->velocity.z;
    player->posZ += velocityZ;
    player->facingAngle = calculateFixedAngleFromDeltaXZ(velocityX, velocityZ);
    if (player->stateFlags & 0x400) {
        player->facingAngle += 0x800;
    }
    if (--player->stateTimer == 0) {
        player->mode = 0xC;
        player->updateState = 0;
        player->updateTimer = 0;
    }
    player->actionEffectLevel = 4;
    player->actionEffectFrame = 2;
}

void dispatchRacePlayerMode07CourseObject(RacePlayer *player) {
    gRacePlayerMode07StateHandlers[player->updateState](player);
}

void updateRacePlayerMode07AlignToLaunchRamp(RacePlayer *player) {
    s16 angleDelta;
    s16 updateTimer;
    s32 temp_a1;
    s32 tempX;
    s32 tempZ;

    updateTimer = player->updateTimer;
    if (updateTimer == 0) {
        player->updateTimer = updateTimer + 1;
        player->stateTimer = 0x14;
        player->unk80 = 0xA;
        player->unk2FA = gLaunchRampCourseObjectAngles[gRaceCourseIndex].angle;
        if (player->stateFlags & 0x400) {
            player->unk2FA += 0x800;
        }
        gMenuFlowState |= 4;
        setRaceMotionAnimation(player, 1);
    }

    stepRaceMotionLoopingAnimation(player);
    angleDelta = (player->unk2FA - player->facingAngle) & 0xFFF;
    if (angleDelta >= 0x801) {
        angleDelta -= 0x1000;
    }
    updateRacePlayerLeanAngle(player, 0x10000, 0);

    temp_a1 = player->unk80;
    if (1) {
    }
    if (1) {
    }
    if (1) {
    }
    if (temp_a1 != 0) {
        angleDelta /= temp_a1;
        if (gRaceCourseStartEntries[gRaceCourseIndex].unk1C) {
        }
        player->facingAngle += angleDelta;
        tempX = player->posX;
        tempZ = player->posZ;
        if (temp_a1) {
        }
        if (1) {
            player->posX = tempX - (0 - ((gRaceCourseStartEntries[gRaceCourseIndex].unk18 - tempX) / temp_a1));
        }
        player->posZ = tempZ - (0 - ((gRaceCourseStartEntries[gRaceCourseIndex].unk1C - tempZ) / temp_a1));
    }

    if (--player->stateTimer == 0) {
        player->posX = gRaceCourseStartEntries[gRaceCourseIndex].unk18;
        player->posZ = gRaceCourseStartEntries[gRaceCourseIndex].unk1C;
        player->updateState++;
        player->updateTimer = 0;
        player->stateTimer = 0;
        setRaceMotionAnimation(player, 5);
        setRaceCameraMode(player->playerIndex, 3);
    }
}

void updateRacePlayerMode07StartLaunchRamp(RacePlayer *player) {
    if (stepRaceMotionAnimationUntilEnd(player) != 0) {
        player->updateState++;
        player->updateTimer = 0;
        player->facingAngle = player->unk2FA;
        player->posX = gRaceCourseStartEntries[gRaceCourseIndex].unk18;
        player->posZ = gRaceCourseStartEntries[gRaceCourseIndex].unk1C;
    }
}

void updateRacePlayerMode07LaunchRampTakeoff(RacePlayer *player) {
    s16 updateTimer;
    s32 unused;
    s32 scratch[14];

    updateTimer = player->updateTimer;
    if (updateTimer == 0) {
        player->updateTimer = updateTimer + 1;
        player->unk40.y = 0x30000;
        player->stateTimer = 0x1E;
        setRaceMotionAnimation(player, 4);
        if (player->stateFlags & 0x400) {
            player->unk80 = 0x20000;
        } else {
            player->unk80 = -0x20000;
        }
    }

    makeFixedRotationY((s16 *)&scratch[2], player->facingAngle);
    scratch[13] = 0;
    scratch[14] = 0;
    scratch[15] = player->unk80;
    transformVec3iByFixedMatrix((s16 *)&scratch[2], (Vec3i *)&scratch[13], (Vec3i *)&scratch[10]);
    player->posX += scratch[10];
    player->posZ += scratch[12];

    makeFixedRotationY((s16 *)&scratch[2], player->facingAngle);
    scratch[13] = 0;
    scratch[14] = 0;
    scratch[15] = player->unk80;
    transformVec3iByFixedMatrix((s16 *)&scratch[2], (Vec3i *)&scratch[13], (Vec3i *)&scratch[10]);
    player->posX += scratch[10];
    player->posZ += scratch[12];
    player->unk40.y -= 0x7000;
    player->posY += player->unk40.y;

    if ((player->unk40.y < 0) && !(player->stateFlags & 1)) {
        if (player->animationId != 1) {
            setRaceMotionAnimation(player, 1);
        }
        stepRaceMotionLoopingAnimation(player);
        updateRacePlayerMotionFeedback(player);
    } else {
        stepRaceMotionAnimationUntilEnd(player);
    }

    if (--player->stateTimer == 0) {
        player->updateState++;
        player->updateTimer = 0;
        player->stateTimer = 0x20;
        createCallbackTask(initLaunchRampCourseObject, 0, 0x64);
        player->stateFlags &= ~0x20;
    }
}

void updateRacePlayerMode07LaunchRampSpin(RacePlayer *player) {
    s32 scratch[15];

    player->facingAngle += 0x10;
    if (player->unk80 > 0) {
        player->unk80 -= 0x820;
    } else {
        player->unk80 += 0x820;
    }

    makeFixedRotationY((s16 *)&scratch[1], player->facingAngle);
    scratch[12] = 0;
    scratch[13] = 0;
    scratch[14] = player->unk80;
    transformVec3iByFixedMatrix((s16 *)&scratch[1], (Vec3i *)&scratch[12], (Vec3i *)&scratch[9]);
    player->posX += scratch[9];
    player->posZ += scratch[11];

    player->facingAngle += 0x10;
    if (player->unk80 > 0) {
        player->unk80 -= 0x820;
    } else {
        player->unk80 += 0x820;
    }

    makeFixedRotationY((s16 *)&scratch[1], player->facingAngle);
    scratch[12] = 0;
    scratch[13] = 0;
    scratch[14] = player->unk80;
    transformVec3iByFixedMatrix((s16 *)&scratch[1], (Vec3i *)&scratch[12], (Vec3i *)&scratch[9]);
    player->posX += scratch[9];
    player->posZ += scratch[11];

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerMotionFeedback(player);
    if (--player->stateTimer == 0) {
        player->updateTimer = 0;
        player->updateState++;
    }
}

void updateRacePlayerMode07LaunchRampPose(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        setRaceMotionAnimation(player, 0x1D);
        player->stateTimer = 0x13;
    }
    stepRaceMotionAnimationUntilEnd(player);
    player->stateTimer--;
    if (player->stateTimer == 0) {
        player->updateTimer = 0;
        player->updateState++;
    }
}

void updateRacePlayerMode07LaunchRampDrift(RacePlayer *player) {
    s16 updateTimer;
    s32 scratch[14];

    updateTimer = player->updateTimer;
    if (updateTimer == 0) {
        player->updateTimer = updateTimer + 1;
        player->stateTimer = 0x13;
        if (player->stateFlags & 0x400) {
            player->unk80 = 0x40000;
        } else {
            player->unk80 = -0x40000;
        }
    }

    makeFixedRotationY((s16 *)&scratch[1], player->facingAngle);
    scratch[12] = 0;
    scratch[13] = 0;
    scratch[14] = player->unk80;
    transformVec3iByFixedMatrix((s16 *)&scratch[1], (Vec3i *)&scratch[12], (Vec3i *)&scratch[9]);
    player->posX += scratch[9];
    player->posZ += scratch[11];

    player->stateTimer--;
    if (player->stateTimer == 0) {
        player->updateTimer = 0;
        player->updateState++;
    }
    stepRaceMotionAnimationUntilEnd(player);
}

typedef struct {
    /* 0x00 */ char pad0[0x94];
    /* 0x94 */ s32 unk94;
    /* 0x98 */ s32 unk98;
    /* 0x9C */ s32 unk9C;
    /* 0xA0 */ char padA0[0xC];
    /* 0xAC */ s8 active;
    /* 0xAD */ char padAD[3];
} Unk801121E0;

typedef struct {
    /* 0x00 */ u8 matrix[0x20];
    /* 0x20 */ s32 transformedX;
    /* 0x24 */ s32 transformedY;
    /* 0x28 */ s32 transformedZ;
    /* 0x2C */ s32 sourceX;
    /* 0x30 */ s32 sourceY;
    /* 0x34 */ s32 sourceZ;
} PlayerTransformScratch80095A88;

extern Unk801121E0 D_801121E0[];

void updateRacePlayerMode07LaunchRampClimb(RacePlayer *player) {
    PlayerTransformScratch80095A88 scratch;

    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0x64;
    }

    if (player->unk80 > 0) {
        if (player->unk80 < 0xA0000) {
            player->unk80 += 0x2000;
        }
        makeFixedRotationXY((s16 *) scratch.matrix, -0x100, player->facingAngle);
    } else {
        if (player->unk80 >= -0x9FFFF) {
            player->unk80 -= 0x2000;
        }
        makeFixedRotationXY((s16 *) scratch.matrix, 0x100, ((0, player))->facingAngle);
    }

    scratch.sourceX = 0;
    scratch.sourceY = 0;
    scratch.sourceZ = player->unk80;
    transformVec3iByFixedMatrix((s16 *) scratch.matrix, (Vec3i *) &scratch.sourceX, (Vec3i *) &scratch.transformedX);
    player->posX = player->posX + scratch.transformedX;
    player->posY += scratch.transformedY;
    player->posZ += scratch.transformedZ;

    player->stateTimer--;
    if (player->stateTimer == 0) {
        player->updateTimer = 0;
        player->updateState++;
    }

    if (player->stateTimer < 0x3C) {
        player->stateFlags |= 0x80000;
        if (D_801121E0[(u16) player->playerIndex].active != 0) {
            D_801121E0[(u16) player->playerIndex].active = 2;
        }
    }
}

void updateRacePlayerMode07SpiralExit(RacePlayer *player) {
    PlayerTransformScratch80095A88 scratch;

    if (player->updateTimer == 0) {
        player->updateTimer++;
        scratch.sourceX = -0x200000;
        scratch.sourceY = 0;
        scratch.sourceZ = 0x400000;
        makeFixedRotationY((s16 *)scratch.matrix, gSpiralCourseObjectAngles[gRaceCourseIndex].angle);
        transformVec3iByFixedMatrix((s16 *)scratch.matrix, (Vec3i *) &scratch.sourceX, (Vec3i *) &player->pos);
        player->posX += gRaceCourseStartEntries[gRaceCourseIndex].unk8.x;
        player->posY += gRaceCourseStartEntries[gRaceCourseIndex].unk8.y + 0x80000;
        player->posZ += gRaceCourseStartEntries[gRaceCourseIndex].unk8.z;
        player->unk502 = gRaceCourseStartEntries[gRaceCourseIndex].unk4;
        player->unk508++;
        player->unk34 = player->pos;
        player->stateTimer = 0x28;
        player->stateFlags &= 0xFBFFFBFF;
        player->facingAngle = gRaceCourseStartEntries[gRaceCourseIndex].unk14;
        player->unk504 = -projectRaceCourseSurfaceProgress(player->unk502, player->posX, player->posZ);
        setRaceCameraMode(player->playerIndex, 1);
        D_801121E0[player->playerIndexU16].unk94 = player->posX;
        D_801121E0[player->playerIndexU16].unk98 = player->posY;
        D_801121E0[player->playerIndexU16].unk9C = player->posZ;
        createCallbackTask(initSpiralCourseObject, 0, 0x64);
        if (player->unk508 >= (gRaceLapCount - 1)) {
            createCallbackTaskWithUserIdPreservingArgs(waitForRaceSetupNamePlate, 0, 0x64, player->playerIndexU16);
        }
    }

    player->stateFlags &= ~0x80000;
    scratch.sourceX = 0;
    scratch.sourceY = 0;
    scratch.sourceZ = -0x40000;
    makeFixedRotationY((s16 *)scratch.matrix, gSpiralCourseObjectAngles[gRaceCourseIndex].angle);
    transformVec3iByFixedMatrix((s16 *)scratch.matrix, (Vec3i *) &scratch.sourceX, (Vec3i *) &scratch.transformedX);
    player->posX += scratch.transformedX;
    player->posZ += scratch.transformedZ;
    player->stateTimer--;

    if (player->stateTimer == 0) {
        player->mode = 1;
        player->updateState = 0;
        player->updateTimer = 0;
        player->stateFlags = 0;

        if (player->rankIndex == 1) {
            player->posX += player->unk40.x;
            player->posZ += player->unk40.z;
        }

        if (player->rankIndex == 2) {
            player->posX += player->unk40.x * 2;
            player->posZ += player->unk40.z * 2;
        }

        if (player->rankIndex == 3) {
            player->posX += player->unk40.x * 4;
            player->posZ += player->unk40.z * 4;
        }

        if (D_801121E0[player->playerIndexU16].active != 0) {
            D_801121E0[player->playerIndexU16].active = 1;
        }
    }
}

// tryStartRacePlayerCourseObjectMode best match: 99.836% (nonmatchings/tryStartRacePlayerCourseObjectMode-5787290371232622032/base_2.c)

#pragma GLOBAL_ASM("asm/nonmatchings/race/player/race_player_update/tryStartRacePlayerCourseObjectMode.s")

#ifdef NON_MATCHING
s32 tryStartRacePlayerCourseObjectMode(s16 arg0) {
    RacePlayer *player;
    Struct800955C0 *courseEntry;
    s32 deltaOrFlags;
    s32 delta;
    RacePlayer *firstPlayer;
    RacePlayer *players;
    u32 stateFlags;

    player = &gRacePlayers[arg0];
    stateFlags = player->stateFlags;
    players = gRacePlayers;
    if (stateFlags & 0x40) {
        return 0;
    }

    firstPlayer = players;
    if (((gRacePlayers[0].isActive != 0) && (arg0 != firstPlayer->playerIndex)) && (gRacePlayers[0].stateFlags & 0x20)) {
        return 0;
    }
    if (((gRacePlayers[1].isActive != 0) && (arg0 != gRacePlayers[1].playerIndex)) && (gRacePlayers[1].stateFlags & 0x20)) {
        return 0;
    }
    if (((gRacePlayers[2].isActive != 0) && (arg0 != gRacePlayers[2].playerIndex)) && (gRacePlayers[2].stateFlags & 0x20)) {
        delta = 0;
        return delta;
    }
    if (((gRacePlayers[3].isActive != 0) && (arg0 != gRacePlayers[3].playerIndex)) && (gRacePlayers[3].stateFlags & 0x20)) {
        return 0;
    }

    courseEntry = &gRaceCourseStartEntries[gRaceCourseIndex];
    deltaOrFlags = player->posX - courseEntry->unk18;
    if (deltaOrFlags < 0) {
        deltaOrFlags = -deltaOrFlags;
    }
    if (deltaOrFlags >= 0x180001) {
        return 0;
    }

    delta = player->posZ - courseEntry->unk1C;
    if (delta < 0) {
        delta = -delta;
    }
    if (delta >= 0x180001) {
        return 0;
    }

    delta = gRacePlayerAttackStartTimer;
    deltaOrFlags = 0x04001020;
    player->stateFlags = stateFlags | deltaOrFlags;
    gRacePlayers[arg0].unk57C = delta;
    if (delta != 0) {
        gRacePlayerAttackStartTimer = delta - 1;
    }
    return 1;
}
#endif

void dispatchRacePlayerMode30Attack(RacePlayer *player) {
    gRacePlayerMode30StateHandlers[player->updateState](player);
    if (player->unk280 < 0xF0000) {
        player->unk280 += 0x8000;
    }
}

void updateRacePlayerMode30AttackApproach(RacePlayer *player) {
    s16 updateTimer;
    s16 facingAngle;
    s16 angleDiff;
    s16 roll;

    updateTimer = player->updateTimer;
    if (updateTimer == 0) {
        player->updateTimer = updateTimer + 1;
        player->subState = 0;
        setRaceCameraMode(player->playerIndex, 5);
        if (player->unk29C >= 0x20001) {
            player->unk306 = 1;
            player->unk31E = calculateFixedAngleFromDeltaXZ(player->unk40.x, player->unk40.z);
            if (player->stateFlags & 0x400) {
                player->unk31E += 0x800;
            }

            if ((s16) (angleDiff = 0x800 & (player->unk31E - player->facingAngle)) < 0x800) {
                player->unk31C = 0;
                if (player->stateFlags & 0x400) {
                    player->unk31C = 1;
                }
                player->unk31E -= 0x400;
            } else {
                player->unk31C = 1;
                if (player->stateFlags & 0x400) {
                    player->unk31C = 0;
                }
                player->unk31E += 0x400;
            }
            player->stateTimer = 0;
        } else {
            player->unk306 = 0;
            setRaceMotionAnimation(player, 1);
        }
    }

    if (player->unk306 != 0) {
        facingAngle = player->facingAngle;
        angleDiff = (player->unk31E - facingAngle) & 0xFFF;
        if (angleDiff >= 0x801) {
            angleDiff -= 0x1000;
        }
        if (angleDiff >= 0x41) {
            angleDiff = 0x40;
        }
        if (angleDiff < -0x40) {
            angleDiff = -0x40;
        }
        player->facingAngle = facingAngle + angleDiff;

        if (player->unk31C != 0) {
            player->stateTimer = 0x1F;
        } else {
            player->stateTimer = -0x1F;
        }

        player->unk2F8 = 0x3F;
        angleDiff = updateRacePlayerLeanAngle(player, player->unk29C, player->stateTimerLow) - player->unk2FA;
        roll = player->unk2FA;
        if (angleDiff >= 0x31) {
            angleDiff = 0x30;
        }
        if (angleDiff < -0x30) {
            angleDiff = -0x30;
        }
        player->unk2FA = player->unk2FA + angleDiff;

        if (player->unk2FA == 0) {
            if (player->animationId != 1) {
                setRaceMotionAnimation(player, 1);
            }
            stepRaceMotionLoopingJointAnimation(player);
        } else if (player->unk2FA >= 0) {
            setRaceMotionAnimation(player, 3);
            blendRaceMotionJointAnimation(player, 3, player->unk2FA, 0x118);
        } else {
            setRaceMotionAnimation(player, 2);
            blendRaceMotionJointAnimation(player, 2, -player->unk2FA, 0x118);
        }
    } else {
        updateRacePlayerLeanAngle(player, 0, 0);
        stepRaceMotionLoopingJointAnimation(player);
    }

    player->unk40.y -= player->unk260;
    updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x6000, 0x6000, 0x6000);
    player->posX += player->unk40.x;
    player->posY += player->unk40.y;
    player->posZ += player->unk40.z;
    updateRacePlayerMotionFeedback(player);

    if ((player->unk40.x == 0) && (player->unk40.z == 0) && !(player->stateFlags & 1)) {
        if (player->soundDisabled == 0) {
            player->updateState++;
            player->stateTimer = 0x1E;
            if (player->animationId != 1) {
                setRaceMotionAnimation(player, 1);
            }
        }
    }
}

void updateRacePlayerMode30AttackResolve(RacePlayer *player) {
    if (gRaceResultState != 0) {
        player->stateTimer = 0;
    }
    if (player->stateTimer == 0) {
        if (gRaceSplitscreenMode == 0) {
            if (player->rankIndex == 0) {
                player->updateState = 2;
                player->updateTimer = 0;
            } else {
                player->updateState = 3;
                player->updateTimer = 0;
            }
        } else if (gRaceResultState == 1) {
            player->updateState = 2;
            player->updateTimer = 0;
        } else {
            player->updateState = 3;
            player->updateTimer = 0;
        }
    }
    updateRacePlayerLeanAngle(player, 0, 0);
    stepRaceMotionLoopingJointAnimation(player);
}

void dispatchRacePlayerMode32Character(RacePlayer *player) {
    updateRacePlayerLeanAngle(player, 0, 0);
    gRacePlayerMode32CharacterHandlers[player->characterId](player);
}

void updateRacePlayerMode32Character0(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        setRaceMotionAnimation(player, 0x23);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->animationId == 0x24) {
            player->stateFlags |= 0x8000000;
        }
        if (player->stateTimer == 0) {
            if (player->updateTimer == 1) {
                enqueueRacePlayerVoiceSound(player, 6);
                player->updateTimer++;
            }
            player->stateTimer = 0x14;
            setRaceMotionAnimation(player, 0x24);
        } else {
            player->stateTimer--;
        }
    }
}

void updateRacePlayerMode32Character1(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        setRaceMotionAnimation(player, 0x24);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->animationId == 0x25) {
            player->stateFlags |= 0x8000000;
        }
        if (player->stateTimer == 0) {
            if (player->updateTimer == 1) {
                enqueueRacePlayerVoiceSound(player, 6);
                player->updateTimer++;
            }
            setRaceMotionAnimation(player, 0x25);
        } else {
            player->stateTimer--;
        }
    }
}

void updateRacePlayerMode32Character2(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        setRaceMotionAnimation(player, 0x23);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->animationId == 0x24) {
            player->stateFlags |= 0x8000000;
        }
        if (player->stateTimer == 0) {
            if (player->updateTimer == 1) {
                enqueueRacePlayerVoiceSound(player, 6);
                player->updateTimer++;
            }
            player->stateTimer = 0x14;
            setRaceMotionAnimation(player, 0x24);
        } else {
            player->stateTimer--;
        }
    }
}

void updateRacePlayerMode32Character3(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        setRaceMotionAnimation(player, 0x24);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->animationId == 0x25) {
            player->stateFlags |= 0x8000000;
        }
        if (player->stateTimer == 0) {
            if (player->updateTimer == 1) {
                enqueueRacePlayerVoiceSound(player, 6);
                player->updateTimer++;
            }
            player->stateTimer = 0;
            setRaceMotionAnimation(player, 0x25);
        } else {
            player->stateTimer--;
        }
    }
}

void updateRacePlayerMode32Character4(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        setRaceMotionAnimation(player, 0x24);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->animationId == 0x25) {
            player->stateFlags |= 0x8000000;
        }
        if (player->stateTimer == 0) {
            if (player->updateTimer == 1) {
                enqueueRacePlayerVoiceSound(player, 6);
                player->updateTimer++;
            }
            player->stateTimer = 0x32;
            setRaceMotionAnimation(player, 0x25);
        } else {
            player->stateTimer--;
        }
    }
}

void updateRacePlayerMode32Character5(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        enqueueRacePlayerVoiceSound(player, 6);
        player->stateTimer = 0x1E;
        setRaceMotionAnimation(player, 0x23);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        player->stateFlags |= 0x8000000;
        if (player->stateTimer == 0) {
            player->stateTimer = 0x1E;
            setRaceMotionAnimation(player, 0x23);
        } else {
            player->stateTimer--;
        }
    }
}

void dispatchRacePlayerMode35Character(RacePlayer *player) {
    updateRacePlayerLeanAngle(player, 0, 0);
    gRacePlayerMode35CharacterHandlers[player->characterId](player);
}

void updateRacePlayerMode35Character0(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        setRaceMotionAnimation(player, 0x25);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->stateTimer == 0) {
            player->stateFlags |= 0x8000000;
            player->stateTimer = 0x1E;
            setRaceMotionAnimation(player, 0x26);
        } else {
            player->stateTimer--;
        }
    }
}

void updateRacePlayerMode35Character1(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        setRaceMotionAnimation(player, 0x26);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->stateTimer == 0) {
            player->stateFlags |= 0x8000000;
            player->stateTimer = 0x1E;
            setRaceMotionAnimation(player, 0x27);
        } else {
            player->stateTimer--;
        }
    }
}

void updateRacePlayerMode35Character2(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        setRaceMotionAnimation(player, 0x25);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->stateTimer == 0) {
            player->stateTimer = 0;
            player->stateFlags |= 0x8000000;
            setRaceMotionAnimation(player, 0x26);
        } else {
            player->stateTimer--;
        }
    }
}

void updateRacePlayerMode35Character3(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        setRaceMotionAnimation(player, 0x26);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->stateTimer == 0) {
            player->stateFlags |= 0x8000000;
            player->stateTimer = 0;
            setRaceMotionAnimation(player, 0x27);
        } else {
            player->stateTimer--;
        }
    }
}

void updateRacePlayerMode35Character4(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        setRaceMotionAnimation(player, 0x26);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->stateTimer == 0) {
            player->stateFlags |= 0x8000000;
            player->stateTimer = 0;
            setRaceMotionAnimation(player, 0x27);
        } else {
            player->stateTimer--;
        }
    }
}

void updateRacePlayerMode35Character5(RacePlayer *player) {
    if (player->updateTimer == 0) {
        player->updateTimer++;
        player->stateTimer = 0;
        setRaceMotionAnimation(player, 0x24);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->stateTimer == 0) {
            if (player->animationId == 0x25) {
                player->stateTimer = 0;
                setRaceMotionAnimation(player, 0x26);
            } else {
                player->stateFlags |= 0x8000000;
                player->stateTimer = 0x1E;
                setRaceMotionAnimation(player, 0x25);
            }
        } else {
            player->stateTimer--;
        }
    }
}

// updateRacePlayersPostUpdate best match: 99.055% (nonmatchings/updateRacePlayersPostUpdate-6182772958467082306/base_12.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/player/race_player_update/updateRacePlayersPostUpdate.s")

#ifdef NON_MATCHING
void updateRacePlayersPostUpdate(void) {
    RacePlayer *player;
    RacePlayerSoundPosition *soundPos;
    RacePlayerSoundPosition *nextSoundPos;
    s32 i;

    if (gRaceUpdatePaused == 0) {
        i = 0;
        if (gRacePlayerCount > 0) {
            do {
                do {
                    updateRacePlayerPostUpdate(&gRacePlayers[i]);
                } while (0);
                i++;
            } while (i < gRacePlayerCount);
        }
    }

    i = 0;
    if (gRacePlayerCount > 0) {
        do {
            updateSnowboardTrailEffect(&gRacePlayers[i]);
            i++;
        } while (i < gRacePlayerCount);
        i = 0;
    }

    if (gRacePlayerCount > 0) {
        player = gRacePlayers; soundPos = (RacePlayerSoundPosition *)&gRacePlayers[0].pos; nextSoundPos = (RacePlayerSoundPosition *)&gRacePlayers[0].unk28.y; do {
            if (player->soundDisabled == 0) {
                enqueuePlayerLoopingPositionalSoundRequest(player->unk584, &soundPos->pos, player->unk582, 0x46, player->unk588, i);
            } else {
                enqueuePlayerLoopingPositionalSoundRequest(player->unk584, &soundPos->pos, 0, 0x46, player->unk588, i);
            }

            nextSoundPos->pos = soundPos->pos;
            player->unk28.y = player->unk64 + player->unk28.y - player->unk58 + 0xA000;
            if (player->soundDisabled == 0) {
                addRenderCallback(&D_801248C8, (void (*)(void *))drawRacePlayerModel, (RacePlayerModelRenderState *)player);
                addRenderCallback(&D_801248EC, (void (*)(void *))drawRacePlayerGroundShadow, (RacePlayerModelRenderState *)player);
            } else {
                addRenderCallback(&D_801248EC, (void (*)(void *))drawRaceGhostPlayerModel, (RacePlayerModelRenderState *)player);
            }
            i++;
            player++;
            soundPos++;
            nextSoundPos++;
        } while (i < gRacePlayerCount);
    }
}
#endif

void updateRacePlayerPostUpdate(RacePlayer *player) {
    unsigned long long random;

    gRacePlayerModePostUpdateHandlers[player->mode](player);
    if (!(gMenuFlowState & 1) && !(player->stateFlags & 0x41000) && (player->soundDisabled == 0)) {
        if (player->unk500 & 3) {
            random = randomNextMain();
            spawnRaceItemTrackSparkBurst(&player->unk4A0, &player->unk4AC, &player->unk4D0, &player->unk4DC, random, player->unk330);
        }
        if (player->unk500 & 0xC) {
            random = randomNextMain();
            spawnRaceItemTrackSparkBurst(&player->unk4B8, &player->unk4C4, &player->unk4E8, &player->unk4F4, random, player->unk330);
        }
    }
    updateRacePlayerFinalLapStatus(player);
    resolveRacePlayerCollisionVolumes(player);
    updateRacePlayerRumble(player);
}

void updateRacePlayerPostUpdateNoop(void) {
    updateRacePlayerSurfaceContact();
}

void updateRacePlayerPostUpdateCourseObject(RacePlayer *player) {
    if (player->updateState < 7) {
        updateRacePlayerGroundAlignment(player);
    } else {
        player->unk64 = 0;
        updateRacePlayerProjectedPosition(player);
        player->unk58 = 0x30000;
    }
}

void updateRacePlayerPostUpdateMode00(RacePlayer *player) {
    if (updateRacePlayerSurfaceContact()) {
        player->mode = 0x16;
        player->updateState = 0;
        player->updateTimer = 0;
        player->stateFlags |= 0x200;
    } else if (tryStartRacePlayerCourseObjectMode(player->playerIndex)) {
        player->mode = 7;
        player->updateState = 0;
        player->updateTimer = 0;
    }
    interpolateRaceMotionJointAnimationFrame(player, 0, (0x60000 - player->unk58) / 0x600, 0x100);
}

void updateRacePlayerVoiceSounds(RacePlayer *player) {
    s32 soundType;
    RacePlayer *player2;
    s32 soundId;
    s32 score;
    s16 voiceType;
    s16 splitType;
    s32 bitIndex;
    u16 playerIndex;

    voiceType = player->unk2A6;
    player2 = player;
    soundId = 0;
    score = 0;
    if (voiceType >= 9) {
        player->unk57B = 1;
        voiceType = player2->unk2A6;
    }
    if ((voiceType != 0) && (player->unk2A2 != 0)) {
        player->unk57B = 1;
        voiceType = player2->unk2A6;
    }
    if (voiceType != 0) {
        soundId = (&gRacePlayerVoiceBaseSoundIds)[voiceType];
        score = voiceType < 9 ? 0xA : 0x28;
        bitIndex = voiceType;
        if ((s32)(u32)bitIndex >= 0xA) {
            bitIndex = 9;
        }
        player->unk2C4 |= 1 << bitIndex;
    }
    splitType = player2->unk2A2;
    if (splitType != 0) {
        if (player->unk2A6 != 0) {
            soundId += (&gRacePlayerVoiceSplitSoundOffsets)[splitType];
            if (splitType & 1) {
                score += 5;
            } else {
                score += 0xA;
            }
        } else {
            score += 1;
            soundId += (&gRacePlayerVoiceLeadSoundOffsets)[splitType];
        }
        soundId += player2->unk2A4;
    }
    if ((soundId != 0) && (gRaceSplitscreenMode == 0) && (gRaceDemoPlaybackEnabled == 0)) {
        addRacePlayerScore(player, soundId);
        playerIndex = player2->playerIndex;
        if (gRacePlayerHudStatuses[playerIndex].active != 0) {
            spawnRaceUiScorePopup((void *)soundId, (s16)playerIndex);
            soundType = 0x4D;
            if (soundId >= 0x64) {
                soundType = 0x4E;
            }
            if (soundId >= 0x78) {
                soundType = 0x4F;
            }
            if (soundId >= 0xC8) {
                soundType = 0x51;
            }
            if (soundId >= 0x12C) {
                soundType = 0x50;
            }
            enqueueSoundEffect((s16)soundType, 0x5A);
        }
    }
    if (gRaceSplitscreenMode == 1) {
        if (gRaceTypeSelection == 2) {
            if (score != 0) {
                soundType = 0x4D;
                if (score >= 0x2710) {
                    score = 0x270F;
                }
                player->unk2C0 += score;
                player->unk2C3++;
                if (player->unk2C3 >= 0x64) {
                    player->unk2C3 = 0x63;
                }
                if (player->unk2C2 < player->unk2C3) {
                    player->unk2C2 = player2->unk2C3;
                }
                if (score >= 0xA) {
                    soundType = 0x4E;
                }
                if (score >= 0xF) {
                    soundType = 0x4F;
                }
                if (score >= 0x14) {
                    soundType = 0x51;
                }
                if (score >= 0x28) {
                    soundType = 0x50;
                }
                if (player2->unk2C4 == 0x3FE) {
                    player2->unk2C0 += 0x12C;
                    player2->unk2C4 = 0x3FF;
                    soundType = 0x51;
                    spawnRaceUiTrickScorePopup((void *)score, 1);
                } else {
                    spawnRaceUiTrickScorePopup((void *)score, 0);
                }
                enqueueSoundEffect((s16)soundType, 0x5A);
                if (player2->unk2C0 >= 0x2710) {
                    player2->unk2C0 = 0x270F;
                }
            }
        }
    }
}

void updateRacePlayerLoopingSound(RacePlayer *player) {
    s32 v0 = -player->unk44 >> 13;
    if (v0 >= 0x80) {
        v0 = 0x7F;
    }
    if (v0 < 0x46) {
        v0 = 0x46;
    }
    if (v0 >= 0x51) {
        player->rumbleTimer = 2;
        player->rumblePatternId = 0;
    } else {
        player->rumbleTimer = 1;
        player->rumblePatternId = 0;
    }
    if (player->soundDisabled == 0) {
        enqueuePositionalSoundEffect(0x21, (SoundPosition *)&player->posX, (s16)v0, 0x32);
    }
}

void updateRacePlayerPostUpdateMode22(RacePlayer *player) {
    u32 stateFlags;

    if (updateRacePlayerSurfaceContact() == 0) {
        stateFlags = player->stateFlags;
        if (stateFlags & 8) {
            player->stateFlags = stateFlags & ~8;
        } else if (stateFlags & 0x800) {
            player->unk2C3 = 0;
            player->mode = 0xE;
            player->updateState = 0;
            player->updateTimer = 0;
            player->stateFlags = stateFlags & ~0x200;
            enqueueRacePlayerVoiceSound(player, 2);
        } else {
            player->stateFlags = stateFlags & ~0x200;
            if (tryStartRacePlayerCourseObjectMode(player->playerIndex) != 0) {
                player->mode = 7;
                player->updateState = 0;
                player->updateTimer = 0;
                updateRacePlayerVoiceSounds(player);
                updateRacePlayerLoopingSound(player);
            } else if (player->subState == 0) {
                player->mode = 0;
                player->updateState = 1;
                player->updateTimer = 0;
                player->unk57A = 2;
                updateRacePlayerLoopingSound(player);
            } else {
                player->mode = 0;
                player->updateState = 0;
                player->updateTimer = 0;
                player->unk57A = 2;
                updateRacePlayerVoiceSounds(player);
                updateRacePlayerLoopingSound(player);
            }
        }
    }
}

void updateRacePlayerPostUpdateAirborneTrick(RacePlayer *player) {
    u32 stateFlags;

    if (updateRacePlayerSurfaceContact() == 0) {
        stateFlags = player->stateFlags;
        if (stateFlags & 8) {
            player->stateFlags = stateFlags & ~8;
        } else if (stateFlags & 0x800) {
            player->unk2C3 = 0;
            player->mode = 0xE;
            player->updateState = 0;
            player->updateTimer = 0;
            player->stateFlags = stateFlags & ~0x200;
            enqueueRacePlayerVoiceSound(player, 2);
        } else {
            player->stateFlags = stateFlags & ~0x200;
            if (tryStartRacePlayerCourseObjectMode(player->playerIndex) != 0) {
                player->mode = 7;
                player->updateState = 0;
                player->updateTimer = 0;
                updateRacePlayerVoiceSounds(player);
                updateRacePlayerLoopingSound(player);
            } else {
                player->mode = 0;
                player->updateState = 0;
                player->updateTimer = 0;
                player->unk57A = 2;
                updateRacePlayerVoiceSounds(player);
                updateRacePlayerLoopingSound(player);
            }
        }
    }
}

void updateRacePlayerPostUpdateAirborneLaunch(RacePlayer *player) {
    if (updateRacePlayerSurfaceContact()) {
        player->mode = 0x16;
        player->updateState = 0;
        player->updateTimer = 0;
        player->stateFlags |= 0x200;
    } else if (tryStartRacePlayerCourseObjectMode(player->playerIndex)) {
        player->mode = 7;
        player->updateState = 0;
        player->updateTimer = 0;
    }
    interpolateRaceMotionJointAnimationFrame(player, 0, (0x60000 - player->unk58) / 0x600, 0x100);
}

void updateRacePlayerPostUpdateNudge(RacePlayer *player) {
    if (updateRacePlayerSurfaceContact()) {
        player->stateFlags |= 0x200;
    } else {
        player->stateFlags &= ~0x200;
    }
}

void updateRacePlayerPostUpdateMode29(RacePlayer *player) {
    u16 playerIndex;

    updateRacePlayerSurfaceContact();
    if (!(player->stateFlags & 0x02000000)) {
        player->mode = 1;
        player->updateState = 0;
        player->updateTimer = 0;
        if ((gRaceSplitscreenMode == 1) && (gRaceTypeSelection == 2)) {
            player->unk2C0 *= 2;
            if (player->unk2C0 >= 0x2710) {
                player->unk2C0 = 0x270F;
            }
            createCallbackTaskPreservingArgs(initRaceUiCrashScorePopup, 0, 0x64);
            enqueueSoundEffect(0x51, 0x32);
        }
        if (gRaceSplitscreenMode == 0) {
            addRacePlayerScore(player, 0x12C);
            playerIndex = player->playerIndex;
            if (gRacePlayerHudStatuses[playerIndex].active != 0) {
                spawnRaceUiScorePopup(0x12C, (s16)playerIndex);
                enqueueSoundEffect(0x51, 0x32);
            }
        }
    }
}

void updateRacePlayerPostUpdateAttack(RacePlayer *player) {
    updateRacePlayerSurfaceContact();
    if (player->updateState < 2) {
        interpolateRaceMotionJointAnimationFrame(player, 0, (0x60000 - player->unk58) / 0x600, 0x100);
    }
}
