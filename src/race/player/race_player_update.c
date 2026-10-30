#include "game/race/items/race_items.h"
#include "game/engine/game_task_scheduler.h"
#include "game/race/race_state.h"
#include "common.h"
#include "game/engine/render_callback.h"
#include "game/engine/asset_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/race/player/race_player_rumble.h"
#include "game/audio/sound_manager.h"
#include "game/race/motion/race_motion.h"
#include "game/race/camera/race_camera.h"
#include "game/race/course/race_course_effects.h"
#include "game/race/player/race_player_collision.h"
#include "game/math/fixed_point_math.h"
#include "game/math/spatial_math.h"
#include "game/race/player/race_player_input.h"
#include "game/race/effects/race_visual_effects.h"
#include "game/race/player/race_player_equipment_use.h"
#include "game/menu/renderer/menu_screen_effects.h"
#include "game/race/player/race_player_movement.h"
#include "game/race/player/race_player_progress.h"
#include "game/race/player/race_player_model_renderer.h"
#include "game/race/player/race_player_update.h"
#include "game/race/player/race_player_properties.h"
#include "game/race/course/race_surface_cues.h"
#include "game/race/ui/race_ui_effects.h"
#include "game/race/effects/snowboard_trail_effects.h"
#include "game/engine/viewport_manager.h"
#include "game/race/flow/race_flow.h"

typedef struct {
    Vec3i position;
    s16 surfaceHint;
    s16 facingAngle;
} RacePlayerPreviewStart;

typedef struct {
    s16 angle;
    s16 unk2;
    char pad4[0x44];
} CourseAngleEntry;

typedef union {
    s32 value;
    u64 align;
} RaceSpawnX;

typedef struct {
    /* 0x00 */ u8 matrix[0x20];
    /* 0x20 */ s32 transformedX;
    /* 0x24 */ s32 transformedY;
    /* 0x28 */ s32 transformedZ;
    /* 0x2C */ s32 sourceX;
    /* 0x30 */ s32 sourceY;
    /* 0x34 */ s32 sourceZ;
} PlayerTransformScratch80095A88;

u16 gRacePlayerVoiceBaseSoundIds[] = {
    0, 100, 100, 120, 120, 150, 150, 150, 150, 220, 250, 300, 350, 400, 500, 0,
};

u16 gRacePlayerVoiceLeadSoundOffsets[] = { 0, 20, 20, 30, 30, 20, 20, 30, 30, 0 };

u16 gRacePlayerVoiceSplitSoundOffsets[] = { 0, 10, 20, 20, 30, 10, 20, 20, 30, 0 };

RacePlayerPreviewStart gRacePlayerPreviewStartPositions[] = {
    { { 0xB368C3F4, 0xCB46B624, 0xEA08A3AE }, 0x0086, 0x0219 },
    { { 0xB2E0A118, 0xCB1BF934, 0xE976F80E }, 0x0087, 0x0219 },
    { { 0xB3DCC2C0, 0xCB76CBC7, 0xEAC479D2 }, 0x0085, 0x0219 },
    { { 0xB4588A34, 0xCB7BBDC0, 0xEA907F46 }, 0x0085, 0x0219 },
    { { 0xD063FFEB, 0xE3CDAB7F, 0xF6122B47 }, 0x0047, 0x039D },
    { { 0xCDF310C7, 0xE3196BA9, 0xF5ABF799 }, 0x004A, 0x00CB },
    { { 0xD213A6C5, 0xE447FE19, 0xF536149D }, 0x0045, 0x057C },
    { { 0xD2F7EC75, 0xE543D9F6, 0xF17E596D }, 0x0042, 0x07A6 },
    { { 0xFF006C53, 0xFE018275, 0xF6D9F959 }, 0x000E, 0x01BB },
    { { 0x00312A27, 0xFE6222E8, 0xF7E100F5 }, 0x000C, 0x01BB },
    { { 0x009D334B, 0xFEA97BBA, 0xF94BA899 }, 0x0008, 0x014F },
    { { 0x0122D61D, 0xFEF0BF18, 0xFB0C2767 }, 0x0005, 0x0007 },
    { { 0xF3146D96, 0xF65FD87C, 0xE594B2A7 }, 0x001C, 0x0112 },
    { { 0xF3DE014E, 0xF6F48DE5, 0xE6F87AAB }, 0x001B, 0x0112 },
    { { 0xF50C3D28, 0xF79DEC47, 0xE85C0A69 }, 0x001B, 0x018C },
    { { 0xF4FD20BA, 0xF7BF24E1, 0xE8D66707 }, 0x001B, 0x018C },
    { { 0xF6BC6F6A, 0xF9282D2F, 0xEAC9A71E }, 0x0019, 0x0185 },
    { { 0xF51BF890, 0xF7B512C2, 0xE89EBBC2 }, 0x001B, 0x0185 },
    { { 0xF51C3792, 0xF7A94134, 0xE876D4B8 }, 0x001B, 0x0185 },
    { { 0xF4EF8D12, 0xF7A8E16E, 0xE8952A98 }, 0x001B, 0x0185 },
    { { 0x03658EEB, 0xFDB008F5, 0xF347D9C3 }, 0x000E, 0x007A },
    { { 0x032C7FE7, 0xFDD2BA88, 0xF3B1B53B }, 0x000E, 0x00A6 },
    { { 0x039BF323, 0xFDBA013C, 0xF3FAB937 }, 0x000E, 0x00A6 },
    { { 0x03561369, 0xFDD1D31D, 0xF455FCC1 }, 0x000E, 0x00A6 },
    { { 0x152F51DA, 0xDB57148A, 0xD9C5DE9A }, 0x003D, 0xFFDA },
    { { 0x1574E1FA, 0xDA904D40, 0xD8767BDE }, 0x003E, 0xFFDA },
    { { 0x15945DC0, 0xDAFF0927, 0xD93EE950 }, 0x003D, 0xFFDA },
    { { 0x15223A28, 0xDBD20087, 0xDA9BC6D4 }, 0x003C, 0xFED2 },
    { { 0xDD2EE8D5, 0xE28F8DD8, 0xB0BE962E }, 0x007B, 0x0225 },
    { { 0xDB2A07EF, 0xE1D64190, 0xAF8EF0AA }, 0x007D, 0x0137 },
    { { 0xDB738683, 0xE1EB39CB, 0xAFA37ADA }, 0x007D, 0x0137 },
    { { 0xDBDF1787, 0xE21B7F22, 0xAFB2168E }, 0x007C, 0x01CA },
    { { 0xF72554A7, 0xEF95A3F6, 0xDDAF853E }, 0x0033, 0x0319 },
    { { 0xF430DCC7, 0xEF3B35CA, 0xDC2D95AE }, 0x0037, 0x0319 },
    { { 0xF8142BD3, 0xEFC226B6, 0xDDDB5064 }, 0x0032, 0x039D },
    { { 0xF968B2D9, 0xF01A652D, 0xDDEDE22C }, 0x0031, 0x039D },
    { { 0xFCC9F88C, 0xFC00DC7E, 0xF2167F01 }, 0x0014, 0xFE57 },
    { { 0xFE07ACCC, 0xFAC04712, 0xEFE1D5D3 }, 0x0016, 0xFE57 },
    { { 0xFFFB7EC8, 0xF9110ACA, 0xEE25DD1F }, 0x0018, 0xFE57 },
    { { 0x010B8D34, 0xF84E86C9, 0xEB0C423D }, 0x001C, 0xFD9B },
    { { 0xB14D4F91, 0xB636FD06, 0xC6EEE2EB }, 0x009F, 0xFC80 },
    { { 0xB28B6B3D, 0xB58C9448, 0xC7013571 }, 0x00A0, 0xFC80 },
    { { 0xB2A97AB1, 0xB5714EA4, 0xC686D82B }, 0x00A0, 0xFB70 },
    { { 0xAF715C51, 0xB7453607, 0xC8C47837 }, 0x009D, 0xFE95 },
};

void (*gRacePlayerModeUpdateHandlers[])(RacePlayer *) = {
    updateRacePlayerMode00Grounded,
    updateRacePlayerMode01JumpStart,
    dispatchRacePlayerAirborneMode,
    updateRacePlayerMode03Nudge,
    updateRacePlayerMode04Spinout,
    updateRacePlayerMode05SpinoutStun,
    updateRacePlayerMode06TerrainFall,
    dispatchRacePlayerMode07CourseObject,
    updateRacePlayerMode08SpinoutRecover,
    updateRacePlayerMode09TerrainCrash,
    updateRacePlayerMode10TerrainCrashSlide,
    updateRacePlayerMode11LaunchHit,
    updateRacePlayerMode12LaunchRecover,
    updateRacePlayerMode13AerialTrick,
    updateRacePlayerMode14PushHit,
    updateRacePlayerMode15AerialTrick,
    updateRacePlayerMode16AerialTrick,
    updateRacePlayerMode17AerialTrick,
    updateRacePlayerMode18AerialTrick,
    updateRacePlayerMode19AerialTrick,
    updateRacePlayerMode20AerialTrick,
    updateRacePlayerMode21AerialTrick,
    updateRacePlayerMode22Airborne,
    updateRacePlayerMode23ItemSteal,
    updateRacePlayerMode24HeavyKnockdown,
    updateRacePlayerMode25SpinHit,
    updateRacePlayerMode26Tumble,
    updateRacePlayerMode27Slide,
    updateRacePlayerMode28TerrainFallWithItemEffect,
    updateRacePlayerMode29Crash,
    dispatchRacePlayerMode30Attack,
    updateRacePlayerMode31AerialTrick,
    updateRacePlayerMode32AerialTrick,
    updateRacePlayerMode33AerialTrick,
    updateRacePlayerMode34AerialTrick,
    updateRacePlayerMode35AerialTrick,
    updateRacePlayerMode36AerialTrick,
    updateRacePlayerMode37AerialTrick,
    updateRacePlayerMode38AerialTrick,
    updateRacePlayerMode39AerialTrick,
    updateRacePlayerMode40Stun,
    updateRacePlayerMode41AerialTrick,
    updateRacePlayerMode42AerialTrick,
    updateRacePlayerMode43AerialTrick,
    updateRacePlayerMode44AerialTrick,
    updateRacePlayerMode45AerialTrick,
    updateRacePlayerMode46AerialTrick,
    updateRacePlayerMode47AerialTrick,
    updateRacePlayerMode48AerialTrick,
    updateRacePlayerMode49AerialTrick,
    updateRacePlayerMode50AerialTrick,
    updateRacePlayerMode51AerialTrick,
    updateRacePlayerMode52AerialTrick,
    updateRacePlayerMode53AerialTrick,
    updateRacePlayerMode54AerialTrick,
    updateRacePlayerMode55AerialTrick,
    updateRacePlayerMode56AerialTrick,
    updateRacePlayerMode57AerialTrick,
};

s32 gRaceRankSpeedBonuses[] = { 0, 0x8000, 0x10000, 0x20000 };

void (*gRacePlayerAirborneUpdateHandlers[])(RacePlayer *) = {
    updateRacePlayerAirborneLaunch,
    updateRacePlayerAirborneCruise,
};

void (*gRacePlayerTrickSubstateHandlers[])(RacePlayer *) = {
    updateRacePlayerTrickSubstateStart,
    updateRacePlayerTrickSubstateHold,
    updateRacePlayerTrickSubstateFinish,
    updateRacePlayerTrickSubstateNoop,
};

void (*gRacePlayerMode07StateHandlers[])(RacePlayer *) = {
    updateRacePlayerMode07AlignToLaunchRamp, updateRacePlayerMode07StartLaunchRamp,
    updateRacePlayerMode07LaunchRampTakeoff, updateRacePlayerMode07LaunchRampSpin,
    updateRacePlayerMode07LaunchRampPose,    updateRacePlayerMode07LaunchRampDrift,
    updateRacePlayerMode07LaunchRampClimb,   updateRacePlayerMode07SpiralExit,
};

void (*gRacePlayerMode30StateHandlers[])(RacePlayer *) = {
    updateRacePlayerMode30AttackApproach,
    updateRacePlayerMode30AttackResolve,
    dispatchRacePlayerMode32Character,
    dispatchRacePlayerMode35Character,
};

void (*gRacePlayerMode32CharacterHandlers[])(RacePlayer *) = {
    updateRacePlayerMode32Character0, updateRacePlayerMode32Character1, updateRacePlayerMode32Character2,
    updateRacePlayerMode32Character3, updateRacePlayerMode32Character4, updateRacePlayerMode32Character5,
};

void (*gRacePlayerMode35CharacterHandlers[])(RacePlayer *) = {
    updateRacePlayerMode35Character0, updateRacePlayerMode35Character1, updateRacePlayerMode35Character2,
    updateRacePlayerMode35Character3, updateRacePlayerMode35Character4, updateRacePlayerMode35Character5,
};

void (*gRacePlayerModePostUpdateHandlers[])(RacePlayer *) = {
    updateRacePlayerPostUpdateMode00,         updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateAirborneLaunch, updateRacePlayerPostUpdateNudge,
    updateRacePlayerPostUpdateNoop,           updateRacePlayerPostUpdateNoop,
    updateRacePlayerPostUpdateNoop,           updateRacePlayerPostUpdateCourseObject,
    updateRacePlayerPostUpdateNoop,           updateRacePlayerPostUpdateNoop,
    updateRacePlayerPostUpdateNoop,           updateRacePlayerPostUpdateNoop,
    updateRacePlayerPostUpdateNoop,           updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateNoop,           updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateAirborneTrick,  updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateAirborneTrick,  updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateAirborneTrick,  updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateMode22,         updateRacePlayerPostUpdateNoop,
    updateRacePlayerPostUpdateNoop,           updateRacePlayerPostUpdateNoop,
    updateRacePlayerPostUpdateNoop,           updateRacePlayerPostUpdateNoop,
    updateRacePlayerPostUpdateNoop,           updateRacePlayerPostUpdateMode29,
    updateRacePlayerPostUpdateAttack,         updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateAirborneTrick,  updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateAirborneTrick,  updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateAirborneTrick,  updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateAirborneTrick,  updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateNoop,           updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateAirborneTrick,  updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateAirborneTrick,  updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateAirborneTrick,  updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateAirborneTrick,  updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateAirborneTrick,  updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateAirborneTrick,  updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateAirborneTrick,  updateRacePlayerPostUpdateAirborneTrick,
    updateRacePlayerPostUpdateAirborneTrick,  updateRacePlayerPostUpdateAirborneTrick,
};
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

void applyRacePlayerProperties(RacePlayer *arg0) {
    RacePlayer *player;
    RacePlayerProperties *var_v0;
    RacePlayerProperties *var_v1;
    RacePlayerProperties *temp_a1;
    RacePlayerProperties *temp_a2;

    player = arg0;
    var_v0 = gRaceCharacterProperties;
    if ((gRaceDemoPlaybackEnabled != 0) || (var_v1 = var_v0, gMainMenuModeSelection != 0)) {
        var_v0 = gRaceDemoBoardProperties;
        var_v1 = gRaceDemoCharacterProperties;
    } else {
        var_v0 = gRaceBoardProperties;
    }
    temp_a1 = (RacePlayerProperties *)((u8 *)var_v1 + (player->characterId * sizeof(RacePlayerProperties)));
    temp_a2 = (RacePlayerProperties *)((u8 *)var_v0 + (player->snowboardIndex * sizeof(RacePlayerProperties)));
    player->state.baseSpeedLimit = (temp_a1->maxSpeed + temp_a2->maxSpeed) << 8;
    player->state.speedLimit = player->state.baseSpeedLimit;
    player->state.gravity = (temp_a1->gravity + temp_a2->gravity) << 4;
    player->state.aerialGravity = (temp_a1->aerialGravity + temp_a2->aerialGravity) << 4;
    player->state.turnStrength = temp_a1->turnStrength + temp_a2->turnStrength;
    player->state.lateralDeceleration = (temp_a1->lateralDeceleration + temp_a2->lateralDeceleration) << 4;
    player->state.turnRadiusAtFullLean = (temp_a1->turnRadiusAtFullLean + temp_a2->turnRadiusAtFullLean) << 4;
    player->state.turnRadiusAtHalfLean = (temp_a1->turnRadiusAtHalfLean + temp_a2->turnRadiusAtHalfLean) << 4;
    player->state.forwardDeceleration = (temp_a1->forwardDeceleration + temp_a2->forwardDeceleration) << 4;
    player->state.reverseDeceleration = (temp_a1->reverseDeceleration + temp_a2->reverseDeceleration) << 4;
}

void initRacePlayer(RacePlayer *player) {
    u32 i;
    char *bytes;
    u32 size;

    size = (char *)(&player->state + 1) - (char *)&player->state;
    bytes = (char *)&player->state;
    i = 0;
    while (i < size) {
        bytes[i++] = 0;
    }

    player->state.loopSoundPitch = 0.0f;
    initRaceMotionModelParts(player);
    setRaceMotionAnimation(player, 1);

    if (gRacePlayerCount == 4) {
        switch (player->playerIndex) {
            case 0:
                player->state.pos.x = -0xC0000;
                break;
            case 1:
                player->state.pos.x = 0xC0000;
                break;
            case 2:
                player->state.pos.x = -0x240000;
                break;
            case 3:
                player->state.pos.x = 0x240000;
                break;
        }
    }
    player->state.pos.z = -0x200000;

    player->state.previousPosition = player->state.pos;
    player->state.targetGroundOffset = 0x40000;
    player->state.collisionRadius = 0x80000;
    player->state.collisionHeight = 0xE0000;

    applyRacePlayerProperties(player);
    if (player->isCpu != 0) {
        player->state.lateralDeceleration = 0x10000;
    }
    if ((player->isCpu != 0) && (player->characterId == 5)) {
        player->state.lateralDeceleration = 0xC0000;
    }
    if (gRaceSplitscreenMode == 2) {
        player->state.itemType = RACE_ITEM_SPEED_FAN;
    }
    if (player->isCpu == 0) {
        player->state.unk519 = 6;
    }
    if ((gRaceSplitscreenMode == 1) && (gRaceTypeSelection == 1)) {
        player->state.shieldShotCount = 10;
    }
    if (gRaceCourseIndex.signedValue == RACE_COURSE_ROOKIE_MOUNTAIN) {
        player->state.pos.z = -0xBD0000;
    }
    player->state.collisionCenterOffset = 0xC0000;

    if ((player->isCpu == 0) && (player->soundDisabled == 0) && (gRaceCameraModeChangeDisabled == 0) &&
        (player->state.forwardDeceleration != player->state.reverseDeceleration)) {
        createCallbackTaskWithUserIdPreservingArgs(initRaceUiBoardReversePrompt, 0, 0x64, player->playerIndex);
    }

    if (gRaceDemoPlaybackEnabled == 1) {
        if (player->playerIndex == 0) {
            player->state.shotType = RACE_SHOT_FREEZE_SHOT;
            player->state.shotCount = 3;
        } else {
            player->state.shotType = RACE_SHOT_BOMB;
            player->state.shotCount = 3;
        }
        player->state.itemType = RACE_ITEM_SPEED_FAN;
    }

    switch (gTrainingCourseLesson) {
        case 1:
            player->state.facingAngle = 0x57E;
            player->state.pos.x = 0xDB870B2D;
            player->state.pos.z = 0xE72C26D8;
            player->state.coursePathIndex = 0x34;
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            player->state.facingAngle = 0xFD5;
            player->state.pos.x = 0xAE2B01F4;
            player->state.pos.z = 0xD6C4D18A;
            player->state.coursePathIndex = 0x96;
            break;
        case 6:
            player->state.facingAngle = 0x993;
            player->state.pos.x = 0xC183C5F9;
            player->state.pos.z = 0xCE51B844;
            player->state.coursePathIndex = 0xAE;
            break;
        case 7:
        case 8:
            switch (player->playerIndex) {
                case 0:
                    player->state.facingAngle = 0xF7D;
                    player->state.pos.x = 0x8FC2BA;
                    player->state.pos.z = 0xFC4A83D8;
                    player->state.coursePathIndex = 4;
                    break;
                case 2:
                    player->state.facingAngle = 0x102;
                    player->state.pos.x = 0x012FA8E0;
                    player->state.pos.z = 0xF9850FD2;
                    player->state.coursePathIndex = 8;
                    break;
                case 3:
                    player->state.facingAngle = 0x8F;
                    player->state.pos.x = 0xC2C658;
                    player->state.pos.z = 0xFA155B82;
                    player->state.coursePathIndex = 7;
                    break;
                case 1:
                    player->state.facingAngle = 0x8F;
                    player->state.pos.x = 0x017C2FC4;
                    player->state.pos.z = 0xFA1B250E;
                    player->state.coursePathIndex = 7;
                    break;
            }
            break;
        case 9:
            player->state.facingAngle = 0x235;
            player->state.pos.x = 0xFEAFF7CD;
            player->state.pos.z = 0xF62C899D;
            player->state.coursePathIndex = 0xE;
            break;
    }

    switch (gMainMenuModeSelection) {
        case 1:
            player->state.shotType = RACE_SHOT_SLAPSTICK;
            player->state.shotCount = 3;
            break;
        case 2:
            player->state.shotType = RACE_SHOT_PARACHUTE;
            player->state.shotCount = 3;
            break;
        case 3:
            player->state.shotType = RACE_SHOT_FREEZE_SHOT;
            player->state.shotCount = 3;
            break;
        case 4:
            player->state.shotType = RACE_SHOT_SNOWMAN;
            player->state.shotCount = 3;
            break;
        case 5:
            player->state.shotType = RACE_SHOT_BOMB;
            player->state.shotCount = 3;
            break;
        case 6:
            player->state.itemType = RACE_ITEM_SPEED_FAN;
            break;
        case 7:
            player->state.itemType = RACE_ITEM_GHOST;
            break;
        case 8:
            player->state.itemType = RACE_ITEM_PAN;
            break;
        case 9:
            player->state.itemType = RACE_ITEM_ROCK;
            break;
        case 10:
            player->state.itemType = RACE_ITEM_RAT_FACE;
            if (player->playerIndex != 0) {
                player->state.score = 0x1388;
            }
            break;
        case 11:
            player->state.itemType = RACE_ITEM_INVISIBLE;
            if (player->playerIndex == 2) {
                player->state.itemType = RACE_ITEM_PAN;
            }
            player->state.shotType = RACE_SHOT_SLAPSTICK;
            player->state.shotCount = 3;
            break;
    }

    if (gMainMenuModeSelection != 0) {
        player->state.pos = gRacePlayerPreviewStartPositions[(gMainMenuModeSelection - 1) * 4 + player->playerIndex].position;
        player->state.facingAngle = gRacePlayerPreviewStartPositions[(gMainMenuModeSelection - 1) * 4 + player->playerIndex].facingAngle;
        player->state.coursePathIndex = gRacePlayerPreviewStartPositions[(gMainMenuModeSelection - 1) * 4 + player->playerIndex].surfaceHint;
    }

    player->state.coursePathIndex = findRaceCourseSurfaceFromHint(player->state.coursePathIndex, player->state.pos.x, player->state.pos.z);
    player->state.pos.y = getRaceCourseSurfaceHeight(player->state.coursePathIndex, player->state.pos.x, player->state.pos.z);
    if (!gRaceDemoPlaybackEnabled) {
        player->state.pos.y += 0x40000;
    }

    player->state.previousPosition = player->state.pos;
    player->state.randomIndex = randomNextMain();
    setRaceCameraMode(player->playerIndex, 1);

    if ((gRaceDemoPlaybackEnabled == 0) && (gRaceSplitscreenMode == 0)) {
        createCallbackTaskWithUserId((CallbackTaskCallback)waitForRaceStartPlayerEffect, 0, 1, player->playerIndex);
    }
}

void updateRacePlayers(void) {
    s32 i;

    gRacePlayers[0].state.loopSoundVolume = 0;
    gRacePlayers[1].state.loopSoundVolume = 0;
    gRacePlayers[2].state.loopSoundVolume = 0;
    gRacePlayers[3].state.loopSoundVolume = 0;
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
    Transform3D mtx;
    u8 cooldown;

    player->state.anglePtr = NULL;
    player->state.previousSurfaceType = player->state.surfaceType;
    player->state.surfaceType = getRaceCourseSurfaceType(player->state.coursePathIndex, player->state.pos.x, player->state.pos.z);
    if (player->state.stateFlags & 0x02000000) {
        player->state.surfaceType = 0xB;
    }

    if (player->state.surfaceType >= 0x11 && player->state.surfaceType < 0x19 && !(player->state.stateFlags & 0x200)) {
        offset.x = 0;
        offset.y = 0;
        offset.z = -0x2000;
        getRaceCourseSurfaceSpawnTransform(player->state.coursePathIndex, &spawnX.value, &spawnY, &spawnZ, &spawnAngle);
        spawnAngle = spawnAngle - (player->state.surfaceType << 9) + 0x2200;
        makeFixedRotationY(mtx.rotation, spawnAngle);
        transformVec3iByFixedMatrix(mtx.rotation, &offset, &transformedOffset);
        player->state.pos.x += transformedOffset.x;
        player->state.pos.y += transformedOffset.y;
        player->state.pos.z += transformedOffset.z;
    }

    player->state.coursePathOffset = -projectRaceCourseSurfaceProgress(player->state.coursePathIndex, player->state.pos.x, player->state.pos.z);
    player->state.velocity.x = player->state.pos.x - player->state.previousPosition.x;
    player->state.velocity.y = player->state.pos.y - player->state.previousPosition.y;
    player->state.velocity.z = player->state.pos.z - player->state.previousPosition.z;
    if (player->state.verticalVelocityLimit < player->state.velocity.y) {
        player->state.velocity.y = player->state.verticalVelocityLimit;
    }
    verticalDelta = player->state.velocity.y;
    if (player->state.velocity.y < -0x400000) {
        player->state.velocity.y = -0x400000;
    }
    if (player->state.velocity.y > 0x400000) {
        player->state.velocity.y = 0x400000;
    }

    player->state.previousPosition = player->state.pos;
    player->state.verticalVelocityLimit = 0x7FFFFFFF;
    player->state.boardContactPoints[4] = player->state.boardContactPoints[0];
    player->state.boardContactPoints[5] = player->state.boardContactPoints[1];

    player->state.boardContactPoints[6] = player->state.boardContactPoints[2];
    player->state.boardContactPoints[7] = player->state.boardContactPoints[3];
    player->state.targetSpeedLimit = player->state.baseSpeedLimit;

#if defined(VERSION_US)
    /* Fixed CPU target-speed addition, independent of rank and course. */
    if (player->isCpu != 0) {
        player->state.targetSpeedLimit += 0x6000;
    }
#endif

    for (i = 0; i < player->state.activeSparkleEffectCount; i++) {
        player->state.targetSpeedLimit -= player->state.targetSpeedLimit >> 2;
    }

    if ((player->state.pendingItemHitFlags == 0) && (player->state.surfaceType == 1) && (player->state.targetSpeedLimit >= 0x50001)) {
        player->state.targetSpeedLimit = 0x50000;
    }

    if (player->state.trailEffectTimer != 0) {
        if ((gRaceSplitscreenMode == 1) && (gRaceTypeSelection == 0)) {
            player->state.targetSpeedLimit += 0x40000;
        } else {
            player->state.targetSpeedLimit += 0x30000;
        }
    }

    resolveRacePlayerHitReactions(player);

    if ((player->isCpu == 0) || (player->state.cpuPaceMode == RACE_CPU_PACE_RANK_BONUS)) {
        speedDelta = gRaceRankSpeedBonuses[player->state.rankIndex];
        catchupDelta = speedDelta - player->state.rankSpeedBonus;
        if (catchupDelta >= 0x21) {
            catchupDelta = 0x20;
        }
        if (catchupDelta < -0x30) {
            catchupDelta = -0x30;
        }
        player->state.rankSpeedBonus += catchupDelta;
        player->state.targetSpeedLimit += player->state.rankSpeedBonus;
    } else {
        if (player->state.cpuPaceMode == RACE_CPU_PACE_CATCH_UP) {
            player->state.targetSpeedLimit += 0x70000;
        }
        if (player->state.cpuPaceMode == RACE_CPU_PACE_SLOW_DOWN) {
            player->state.targetSpeedLimit /= 3;
        }
        if ((player->state.cpuPaceMode == RACE_CPU_PACE_YIELD) && (player->state.rankIndex != 3)) {
            player->state.targetSpeedLimit -= player->state.targetSpeedLimit >> 4;
        }
    }

    if ((player->isCpu != 0) && (player->state.lapDigit >= (gRaceLapCount - 1))) {
        if (((gRaceCourseStartEntries[gRaceCourseIndex.signedValue].pathIndex - 0x14) < player->state.coursePathIndex) &&
            (player->state.coursePathIndex < gRaceCourseStartEntries[gRaceCourseIndex.signedValue].pathIndex)) {
            player->state.targetSpeedLimit = player->state.baseSpeedLimit;
        }
    }

    speedDelta = player->state.targetSpeedLimit - player->state.speedLimit;
    if (speedDelta > 0xE00) {
        speedDelta = 0xE00;
    }
    if (speedDelta < -0xE00) {
        speedDelta = -0xE00;
    }
    player->state.speedLimit += speedDelta;
    player->state.stateFlags &= ~0x800;

    if (player->isCpu != 0) {
        updateRacePlayerCheckpointEvents(player);
    }

    if (player->state.unk578 != 0) {
        player->state.unk578--;
    }

    gRacePlayerModeUpdateHandlers[player->state.mode](player);
    player->state.unk517 = 0;
    player->state.unk57B = 0;
    player->state.isTargetedByAttack = 0;
    player->state.stateFlags &= ~0x2000000;
    if (player->state.stateFlags & 2) {
        player->state.stateFlags &= ~2;
    } else {
        player->state.modelRotationX = 0;
        player->state.modelRotationY = 0;
        player->state.modelRotationZ = 0;
    }

    if (player->state.stateFlags & 0x80000) {
        gViewportStates[player->playerIndex].overlayAlpha += 0x10;
        if (gViewportStates[player->playerIndex].overlayAlpha > 0xFF) {
            gViewportStates[player->playerIndex].overlayAlpha = 0xFF;
        }
    } else {
        gViewportStates[player->playerIndex].overlayAlpha -= 0x10;
        if (gViewportStates[player->playerIndex].overlayAlpha < 0) {
            gViewportStates[player->playerIndex].overlayAlpha = 0;
        }
    }

    deltaX = player->state.pos.x - player->state.previousPosition.x;
    deltaZ = player->state.pos.z - player->state.previousPosition.z;
    player->state.movementSpeed = integerSquareRoot64((s64)deltaX * deltaX + (s64)deltaZ * deltaZ);
    player->state.collisionVelocityX = player->state.velocity.x;
    player->state.collisionVelocityZ = player->state.velocity.z;
    player->state.collisionBottomY = player->state.pos.y - 0x60000;

    if (player->state.unk2D4 != 0) {
        player->state.unk2D4--;
    }

    if (player->state.trailEffectTimer != 0) {
        player->state.loopSoundId = SFX_SPEED_FAN_LOOP;
        player->state.loopSoundVolume = 0x7F;
        player->state.loopSoundPitch = 0.0f;
        if (player->state.trailEffectTimer >= 0xDD) {
            player->state.rumbleTimer = 2;
            player->state.rumblePatternId = 0;
        }
    }

    if (player->state.invisibilityTimer != 0) {
        player->state.invisibilityTimer--;
    }
    if (player->state.stateFlags & 0x403040) {
        player->state.invisibilityTimer = 0;
    }

    if (!(gMenuFlowState & 1)) {
        cooldown = player->state.unk519;
        if (cooldown != 0) {
            player->state.unk519 = cooldown - 1;
        }
    }
}

void updateRacePlayerMotionFeedback(RacePlayer *player) {
    s16 angleDiff;
    s32 itemType;
    s32 deltaX;
    s32 deltaZ;

    deltaX = player->state.pos.x - player->state.previousPosition.x;
    deltaZ = player->state.pos.z - player->state.previousPosition.z;
    angleDiff = (calculateFixedAngleFromDeltaXZ(deltaX, deltaZ) - player->state.facingAngle) & 0xFFF;
    if (angleDiff >= 0x801) {
        angleDiff = 0x1000 - angleDiff;
    }
    if (angleDiff >= 0x401) {
        angleDiff = 0x800 - angleDiff;
    }

    player->state.loopSoundPitch = (f32)((f64)(f32)-angleDiff / 171.0);
    player->state.loopSoundVolume = player->state.movementSpeed >> 0xC;
    if (player->state.loopSoundVolume >= 0x80) {
        player->state.loopSoundVolume = 0x7F;
    }

    itemType = getRaceItemEffectType(player->state.surfaceType);
    player->state.loopSoundId = SFX_BOARD_GLIDE_LOOP;
    if (itemType == 1) {
        player->state.loopSoundId = SFX_BREAKABLE_SURFACE_LOOP;
        player->state.loopSoundVolume = 0x7F;
        player->state.loopSoundPitch = 0.0f;
    }
    if (itemType == 2) {
        player->state.loopSoundId = SFX_BOARD_CONTACT_FALLBACK_LOOP;
    }
    if (3 == itemType) {
        player->state.loopSoundId = SFX_BOARD_SURFACE_SPRAY_LOOP;
    }
    if (4 == itemType) {
        player->state.loopSoundId = SFX_BOARD_CONTACT_FALLBACK_LOOP;
        if (player->state.surfaceType == 0xD) {
            player->state.loopSoundId = SFX_BOARD_SURFACE_RUMBLE_LOOP;
            if (player->state.loopSoundVolume >= 0x51) {
                player->state.rumbleTimer = 2;
                player->state.rumblePatternId = 1;
            }
        }
        player->state.loopSoundPitch = 0.0f;
    }
    if (player->state.stateFlags & 0x2000000) {
        player->state.loopSoundId = SFX_BOARD_CONTACT_FALLBACK_LOOP;
    }
}

/* Mode 2 supplies a push/jump: humans enter with A, CPUs also enter at low
 * speed (movementSpeed < 0x30000 or localVelocityZ >= -0x2FFFF). Its release
 * path in updateRacePlayerAirborneCruise adds a push as well as a jump.
 * Human flag 0x400 uses board orientation relative to startSurfaceTargetX/Z
 * at the course's pathIndex; elsewhere it follows signed local velocity. */
void updateRacePlayerMode00Grounded(RacePlayer *player) {
    volatile s32 pad[26];
    s64 temp;
    s32 turnTarget;
    s32 targetX;
    s32 surfaceCue;
    s32 targetZ;
    s32 bankRate;
    RaceCourseStartEntry *spawn;
    s32 lean;
    s32 speed;
    s16 steerAngle;
    s16 turn;
    s16 quickTurn;
    s32 rotation;
    s16 unused;
    s32 interpolated;

    if (player->state.updateState == 0) {
        player->state.updateState++;
        player->state.stateFlags &= ~0x200;
        player->state.stanceChangeTimer = 0;
        player->state.stanceTransitionFrame = 0;
        player->state.subState = 0;
        player->state.subStateTimer = 0;
        player->state.modeAngle = 0;
        player->state.unk528 = 0;
        player->state.localVelocityX = 0;
        if (player->state.unk57B != 0) {
            enqueueRacePlayerVoiceSound(player, RACE_VOICE_LANDING);
            setRaceMotionAnimation(player, 0x22);
        }
    }

    if (player->state.unk57A != 0) {
        player->state.stanceChangeTimer = 6;
    }
    if (player->state.targetGroundOffset == 0) {
        player->state.targetGroundOffset = 0x40000;
    }

    player->state.velocity.y -= player->state.gravity;
    speed = player->state.localVelocityZ;
    if (speed > 0) {
        if (speed < 0x4000) {
            speed = 0x4000;
        }
    } else if (speed >= -0x3FFF) {
        speed = -0x4000;
    }

    if (player->isCpu != 0) {
        getRaceCourseTargetPositionAhead(
            player->state.coursePathIndex,
            player->state.pos.x,
            player->state.pos.z,
            &targetX,
            &targetZ,
            (s8)player->unk17,
            (u16)player->playerIndex
        );
        turn =
            (calculateFixedAngleBetweenXZPoints(player->state.pos.x, player->state.pos.z, targetX, targetZ) - player->state.facingAngle) &
            0xFFF;
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

        turn = (turn - player->state.unk528) & 0xFFF;
        if (turn >= 0x801) {
            turn -= 0x1000;
        }
        if (turn >= 3) {
            turn = 2;
        }
        if (turn < -2) {
            turn = -2;
        }
        player->state.unk528 += turn;
        turn = player->state.unk528;
        if (turn == 1) {
            turn = 0;
        }
        if (turn == -1) {
            turn = 0;
        }
        player->state.leanScale = 0x3F;
    } else {
        turn = -player->state.stickX;
        player->state.leanScale = 0x20 - player->state.stickY;
        if (player->state.leanScale < 0x1F) {
            player->state.leanScale = 0x1F;
        }
    }

    lean = speed;
    if (player->state.localVelocityZ > 0) {
        lean = -speed;
        turn = -turn;
    }

    if (player->isCpu == 0) {
        spawn = &gRaceCourseStartEntries[gRaceCourseIndex.signedValue];
        if ((spawn->pathIndex == player->state.coursePathIndex) && !(player->state.stateFlags & 0x40)) {
            surfaceCue = (s16)(((calculateFixedAngleBetweenXZPoints(
                                     player->state.pos.x,
                                     player->state.pos.z,
                                     spawn->startSurfaceTargetX,
                                     spawn->startSurfaceTargetZ
                                 ) -
                                 player->state.facingAngle) +
                                0x400) &
                               0xFFF);
            if (surfaceCue < 0x800) {
                if (player->state.stateFlags & 0x400) {
                    player->state.stanceChangeTimer = 0;
                    player->state.stateFlags &= ~0x400;
                    player->state.stanceTransitionFrame = 6 - player->state.stanceTransitionFrame;
                }
            } else if (!(player->state.stateFlags & 0x400)) {
                player->state.stanceChangeTimer = 0;
                player->state.stateFlags |= 0x400;
                player->state.stanceTransitionFrame = 6 - player->state.stanceTransitionFrame;
            }
        } else {
            if (player->state.localVelocityZ < -0x8000) {
                if (player->state.stateFlags & 0x400) {
                    if (player->state.stanceChangeTimer >= 6) {
                        player->state.stanceChangeTimer = 0;
                        player->state.stateFlags &= ~0x400;
                        player->state.stanceTransitionFrame = 6 - player->state.stanceTransitionFrame;
                    } else {
                        player->state.stanceChangeTimer++;
                    }
                }
            }
            if (player->state.localVelocityZ >= 0x8001) {
                if (!(player->state.stateFlags & 0x400)) {
                    if (player->state.stanceChangeTimer >= 6) {
                        player->state.stanceChangeTimer = 0;
                        player->state.stateFlags |= 0x400;
                        player->state.stanceTransitionFrame = 6 - player->state.stanceTransitionFrame;
                    } else {
                        player->state.stanceChangeTimer++;
                    }
                }
            }
        }
    }

    if (!(gMenuFlowState & 1)) {
        turnTarget = updateRacePlayerLeanAngle(player, lean, turn);
        if (player->state.stanceTransitionFrame == 0) {
            steerAngle = player->state.modeAngle;
            steerAngle = turnTarget - steerAngle;
            if (steerAngle >= 0x31) {
                steerAngle = 0x30;
            }
            if (steerAngle < -0x30) {
                steerAngle = -0x30;
            }
            player->state.modeAngle += steerAngle;
            if (player->state.modeAngle == 0) {
                if (player->state.animationId == 0x22) {
                    if (stepRaceMotionJointAnimationUntilEnd(player) != 0) {
                        setRaceMotionAnimation(player, 1);
                    }
                } else {
                    if (player->state.animationId != 1) {
                        setRaceMotionAnimation(player, 1);
                    }
                    stepRaceMotionLoopingJointAnimation(player);
                }
            } else if (player->state.modeAngle >= 0) {
                setRaceMotionAnimation(player, 3);
                blendRaceMotionJointAnimation(player, 3, player->state.modeAngle, 0x118);
            } else {
                setRaceMotionAnimation(player, 2);
                blendRaceMotionJointAnimation(player, 2, -player->state.modeAngle, 0x118);
            }
        } else {
            player->state.stanceTransitionFrame--;
            steerAngle = -player->state.modeAngle;
            if (steerAngle >= 0x31) {
                steerAngle = 0x30;
            }
            if (steerAngle < -0x30) {
                steerAngle = -0x30;
            }
            player->state.modeAngle += steerAngle;
            setRaceMotionAnimation(player, player->state.stanceTransitionFrame + 8);
            stepRaceMotionJointAnimationUntilEnd(player);
        }

        if (player->isCpu == 0) {
            steerAngle = 0;
            bankRate = (-turn * 0x10 * player->state.turnStrength) / 0x100;
            interpolated = (((player->state.turnRadiusAtHalfLean - player->state.turnRadiusAtFullLean) * (0x3F - player->state.leanScale)) / 0x20) + player->state.turnRadiusAtFullLean;
            if (bankRate != 0) {
                lean = (lean - 0xFF) / 0x100;
                temp = ((s64)interpolated * lean * lean) / bankRate;
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
                    steerAngle = calculateFixedAngleBetweenXZPoints((s32)temp, 0, 0, speed);
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
            player->state.facingAngle += steerAngle;
        } else {
            player->state.facingAngle += quickTurn;
        }

        rotation = 0;
        if (player->state.trailEffectTimer != 0) {
            rotation = -0x8000;
            if (player->state.snowboardTrailSide != 0) {
                rotation = 0x8000;
            }
        }
        if ((gMainMenuModeSelection != 0) && ((u8)player->state.unk519 == 6)) {
            rotation = -0x60000;
        }
        updateRacePlayerLocalVelocity(player, 0, rotation, player->state.lateralDeceleration, player->state.forwardDeceleration, player->state.reverseDeceleration);

        if (player->state.unk57A == 0) {
            if (player->isCpu == 0) {
                if (player->state.heldInput & A_BUTTON) {
                    player->state.mode = 2;
                    player->state.updateState = 0;
                    player->state.updateTimer = 0;
                }
            } else if (player->state.checkpointHit != 0) {
                player->state.mode = 2;
                player->state.updateState = 0;
                player->state.updateTimer = 0;
                player->state.unk525 = 1;
            } else if ((player->state.movementSpeed < 0x30000) || (player->state.localVelocityZ >= (s32)0xFFFD0001)) {
                player->state.mode = 2;
                player->state.updateState = 0;
                player->state.updateTimer = 0;
                player->state.unk525 = 0;
            }
            updateRacePlayerShotUse(player);
            updateRacePlayerItemUse(player);
        } else {
            player->state.unk57A--;
            player->state.stanceChangeTimer = 6;
        }

        if (player->state.stateFlags & 0x2000000) {
            player->state.mode = 0x1D;
            player->state.updateState = 0;
            player->state.updateTimer = 0;
        }
        if (player->state.stateFlags & 0x40) {
            player->state.mode = 0x1E;
            player->state.updateState = 0;
            player->state.updateTimer = 0;
        }
    } else {
        stepRaceMotionLoopingJointAnimation(player);
    }

    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += player->state.velocity.y;
    player->state.pos.z += player->state.velocity.z;
    updateRacePlayerMotionFeedback(player);
    if (player->state.unk517 != 0) {
        enqueueRacePlayerVoiceSound(player, RACE_VOICE_ATTACK_SUCCESS);
    }
}

void updateRacePlayerMode29Crash(RacePlayer *player) {
    s16 updateState;

    updateState = player->state.updateState;
    if (updateState == 0) {
        player->state.updateState = updateState + 1;
        player->state.stateFlags &= ~0x200;
        player->state.modeValue = 0;
        player->state.unk80 = 0;
        player->state.subState = 0;
        player->state.subStateTimer = 0;
        player->state.modeAngle = 0;
        player->state.unk528 = 0;
        setRaceMotionAnimation(player, 0x1E);
    }

    player->state.velocity.y -= player->state.gravity;
    player->state.facingAngle = player->state.unk332;
    player->state.pitchAngle = player->state.unk334;
    if (player->state.stateFlags & 0x400) {
        player->state.facingAngle += 0x800;
        player->state.pitchAngle = -(player->state.pitchAngle & 0xFFF);
    }

    updateRacePlayerLeanAngle(player, 0, 0);
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += player->state.velocity.y;
    player->state.pos.z += player->state.velocity.z;

    if (stepRaceMotionAnimationUntilEnd(player) != 0) {
        setRaceMotionAnimation(player, 0x1F);
        stepRaceMotionAnimationUntilEnd(player);
    }

    player->state.unk578 = 6;
    updateRacePlayerMotionFeedback(player);
    if (player->state.unk517 != 0) {
        enqueueRacePlayerVoiceSound(player, RACE_VOICE_ATTACK_SUCCESS);
    }
}

void updateRacePlayerMode01JumpStart(RacePlayer *player) {
    s32 unused;
    s32 targetX;
    s32 targetZ;
    s16 angleDiff;
    s32 yVel;

    if (player->state.updateState == 0) {
        if (player->state.animationId != 4) {
            setRaceMotionAnimation(player, 4);
        }
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.targetGroundOffset = 0;
        resetRacePlayerTrickSubstate(player);
        player->state.aerialTrickId = 0;
    }

    if (player->state.subState == 0) {
        updateRacePlayerShotUse(player);
        if (player->isCpu == 0) {
            if (player->state.stickX > 0) {
                player->state.facingAngle -= 0x18;
            }
            if (player->state.stickX < 0) {
                player->state.facingAngle += 0x18;
            }
        } else {
            getRaceCourseTargetPositionAhead(
                player->state.coursePathIndex,
                unused = player->state.pos.x,
                player->state.pos.z,
                &targetX,
                &targetZ,
                (s8)player->unk17,
                (u16)player->playerIndex
            );
            angleDiff = (calculateFixedAngleBetweenXZPoints(player->state.pos.x, player->state.pos.z, targetX, targetZ) -
                         player->state.facingAngle) &
                        0xFFF;
            if (angleDiff >= 0x801) {
                angleDiff -= 0x1000;
            }
            if (angleDiff >= 0x19) {
                angleDiff = 0x18;
            }
            if (angleDiff < -0x18) {
                angleDiff = -0x18;
            }
            player->state.facingAngle += angleDiff;
        }
        stepRaceMotionAnimationUntilEnd(player);
    }

    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);
    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;
    if (player->state.unk517 != 0) {
        enqueueRacePlayerVoiceSound(player, RACE_VOICE_ATTACK_SUCCESS);
    }
}

void updateRacePlayerMode22Airborne(RacePlayer *player) {
    s32 unused;
    s32 targetX;
    s32 targetZ;
    s16 angleDiff;
    s32 yVel;

    if (player->state.updateState == 0) {
        player->state.updateState++;
        player->state.stateFlags |= 0x208;
        player->state.targetGroundOffset = 0;
        player->state.unk80 = 0;
        resetRacePlayerTrickSubstate(player);
        player->state.aerialTrickId = 0;
    }

    if (player->state.subState == 0) {
        updateRacePlayerShotUse(player);
        if (player->isCpu == 0) {
            if (player->state.stickX > 0) {
                player->state.facingAngle -= 0x18;
            }
            if (player->state.stickX < 0) {
                player->state.facingAngle += 0x18;
            }
        } else {
            getRaceCourseTargetPositionAhead(
                player->state.coursePathIndex,
                unused = player->state.pos.x,
                player->state.pos.z,
                &targetX,
                &targetZ,
                (s8)player->unk17,
                (u16)player->playerIndex
            );
            angleDiff = (calculateFixedAngleBetweenXZPoints(player->state.pos.x, player->state.pos.z, targetX, targetZ) -
                         player->state.facingAngle) &
                        0xFFF;
            if (angleDiff >= 0x801) {
                angleDiff -= 0x1000;
            }
            if (angleDiff >= 0x19) {
                angleDiff = 0x18;
            }
            if (angleDiff < -0x18) {
                angleDiff = -0x18;
            }
            player->state.facingAngle += angleDiff;
        }
    }

    updateRacePlayerTrickSubstate(player);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);
    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;
    if (player->state.unk517 != 0) {
        enqueueRacePlayerVoiceSound(player, RACE_VOICE_ATTACK_SUCCESS);
    }
}

void dispatchRacePlayerAirborneMode(RacePlayer *player) {
    gRacePlayerAirborneUpdateHandlers[player->state.updateState](player);
}

void updateRacePlayerAirborneLaunch(RacePlayer *player) {
    RaceCourseStartEntry *spawn;
    s16 updateTimer;
    s32 velocityY;
    s32 rotation;
    s8 turnTimer;

    updateTimer = player->state.updateTimer;
    if (updateTimer == 0) {
        player->state.updateTimer = updateTimer + 1;
        player->state.modeValue = 0x40000;
        player->state.stateFlags |= 0x10;
        setRaceMotionAnimation(player, 5);
        resetRacePlayerSurfaceCueState(player);
        player->state.targetGroundOffset = 0x40000;
        player->state.aerialTrickId = 0;
        player->state.stanceTransitionFrame = 0;
        player->state.unk336 = 0;
    }

    if (player->isCpu == 0) {
        spawn = &gRaceCourseStartEntries[gRaceCourseIndex.signedValue];
        if ((spawn->pathIndex == player->state.coursePathIndex) && !(player->state.stateFlags & 0x40)) {
            velocityY = (s16)(((calculateFixedAngleBetweenXZPoints(
                                    player->state.pos.x,
                                    player->state.pos.z,
                                    spawn->startSurfaceTargetX,
                                    spawn->startSurfaceTargetZ
                                ) -
                                player->state.facingAngle) +
                               0x400) &
                              0xFFF);
            if (velocityY < 0x800) {
                if (player->state.stateFlags & 0x400) {
                    player->state.stateFlags &= ~0x400;
                    player->state.stanceTransitionFrame = 6 - player->state.stanceTransitionFrame;
                }
            } else if (!(player->state.stateFlags & 0x400)) {
                player->state.stateFlags |= 0x400;
                player->state.stanceTransitionFrame = 6 - player->state.stanceTransitionFrame;
            }
        } else {
            if (player->state.localVelocityZ < -0x8000) {
                if (player->state.stateFlags & 0x400) {
                    player->state.stateFlags &= ~0x400;
                    player->state.stanceTransitionFrame = 6 - player->state.stanceTransitionFrame;
                }
            }
            if (player->state.localVelocityZ >= 0x8001) {
                if (!(player->state.stateFlags & 0x400)) {
                    player->state.stateFlags |= 0x400;
                    player->state.stanceTransitionFrame = 6 - player->state.stanceTransitionFrame;
                }
            }
        }
    }

    turnTimer = player->state.stanceTransitionFrame;
    if (turnTimer != 0) {
        player->state.stanceTransitionFrame = turnTimer - 1;
        setRaceMotionAnimation(player, player->state.stanceTransitionFrame + 8);
    } else if (player->state.animationId != 5) {
        setRaceMotionAnimation(player, 5);
    }

    updateRacePlayerShotUse(player);
    stepRaceMotionJointAnimationUntilEnd(player);

    velocityY = player->state.targetGroundOffset;
    if (velocityY != 0) {
        if (velocityY < 0x20000) {
            player->state.targetGroundOffset = 0;
        } else {
            player->state.targetGroundOffset = velocityY - 0x20000;
        }
    } else {
        player->state.updateState++;
    }

    if ((player->isCpu == 0) && !(player->state.heldInput & A_BUTTON)) {
        player->state.stateFlags &= ~0x10;
    }

    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.gravity;

    rotation = 0;
    if (player->state.trailEffectTimer != 0) {
        rotation = -0x8000;
        if (player->state.snowboardTrailSide != 0) {
            rotation = 0x8000;
        }
    }

    updateRacePlayerLocalVelocity(player, 0, rotation, player->state.lateralDeceleration, player->state.forwardDeceleration, player->state.reverseDeceleration);
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += player->state.velocity.y;
    player->state.pos.z += player->state.velocity.z;
    updateRacePlayerSurfaceCue(player);
    updateRacePlayerMotionFeedback(player);
    if (player->state.unk517 != 0) {
        enqueueRacePlayerVoiceSound(player, RACE_VOICE_ATTACK_SUCCESS);
    }
}

/* As in grounded mode, human flag 0x400 uses target-relative board orientation
 * at the start surface and signed local velocity elsewhere. */
void updateRacePlayerAirborneCruise(RacePlayer *player) {
    s32 pushAcceleration;
    s32 boostAcceleration;
    s32 surfaceCue;

    pushAcceleration = 0;
    if (player->isCpu == 0) {
        if ((player->state.coursePathIndex == gRaceCourseStartEntries[gRaceCourseIndex.signedValue].pathIndex) &&
            (!(player->state.stateFlags & 0x40))) {
            s32 stanceAngle;

            stanceAngle = calculateFixedAngleBetweenXZPoints(
                              player->state.pos.x,
                              player->state.pos.z,
                              gRaceCourseStartEntries[gRaceCourseIndex.signedValue].startSurfaceTargetX,
                              gRaceCourseStartEntries[gRaceCourseIndex.signedValue].startSurfaceTargetZ
                          ) -
                          player->state.facingAngle + 0x400;
            if ((s16)(stanceAngle & 0xFFF) < 0x800) {
                if (player->state.stateFlags & 0x400) {
                    player->state.stateFlags &= ~0x400;
                    player->state.stanceTransitionFrame = 6 - player->state.stanceTransitionFrame;
                }
            } else {
                if (!(player->state.stateFlags & 0x400)) {
                    player->state.stateFlags |= 0x400;
                    player->state.stanceTransitionFrame = 6 - player->state.stanceTransitionFrame;
                }
            }
        } else {
            if (player->state.localVelocityZ < -0x8000) {
                if (player->state.stateFlags & 0x400) {
                    player->state.stateFlags &= ~0x400;
                    player->state.stanceTransitionFrame = 6 - player->state.stanceTransitionFrame;
                }
            }
            if (player->state.localVelocityZ >= 0x8001) {
                if (!(player->state.stateFlags & 0x400)) {
                    player->state.stateFlags |= 0x400;
                    player->state.stanceTransitionFrame = 6 - player->state.stanceTransitionFrame;
                }
            }
        }
        if (gRaceDemoPlaybackEnabled == 0) {
            if (player->state.unk336 < 0x5A) {
                player->state.unk336++;
            }
        }
    }

    if (player->state.stanceTransitionFrame != 0) {
        player->state.stanceTransitionFrame--;
        setRaceMotionAnimation(player, player->state.stanceTransitionFrame + 8);
    } else if (player->state.animationId != 5) {
        setRaceMotionAnimation(player, 5);
    }

    if (player->state.stateFlags & 0x10) {
        if (player->isCpu == 0) {
            if (!(player->state.heldInput & A_BUTTON)) {
                player->state.stateFlags &= ~0x10;
            }
        } else {
            player->state.stateFlags &= ~0x10;
            if (player->state.unk525 != 0) {
                player->state.modeValue = 0x46000;
            }
        }
        if (player->state.stateFlags & 0x10) {
            if (player->state.modeValue < 0x46000) {
                player->state.modeValue += 0x2000;
            }
        }
    }
    updateRacePlayerShotUse(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.gravity;
    boostAcceleration = 0;
    if (player->state.trailEffectTimer) {
        boostAcceleration = -0x8000;
        if (player->state.snowboardTrailSide != 0) {
            boostAcceleration = 0x8000;
        }
    }
    if (!(player->state.stateFlags & 0x10)) {
        if (player->state.stateFlags & 0x400) {
            pushAcceleration = 0x30000;
            if ((u8)player->state.unk519 != 0) {
                boostAcceleration = 0x100000;
            }
            if (player->state.unk336 >= 0x3C) {
                player->state.targetSpeedLimit += 0x50000;
                player->state.speedLimit += 0x50000;
                boostAcceleration += 0x50000;
                enqueueRacePlayerVoiceSound(player, RACE_VOICE_TRICK);
            }
        } else {
            pushAcceleration = -0x30000;
            if ((u8)player->state.unk519 != 0) {
                boostAcceleration = -0x100000;
            }
            if (player->state.unk336 >= 0x3C) {
                player->state.targetSpeedLimit += 0x50000;
                player->state.speedLimit += 0x50000;
                boostAcceleration -= 0x50000;
                enqueueRacePlayerVoiceSound(player, RACE_VOICE_TRICK);
            }
        }
    }
    updateRacePlayerLocalVelocity(
        player,
        pushAcceleration,
        boostAcceleration,
        player->state.lateralDeceleration,
        player->state.forwardDeceleration,
        player->state.reverseDeceleration
    );
    surfaceCue = updateRacePlayerSurfaceCue(player);
    if (!(player->state.stateFlags & 0x10)) {
        if (player->state.modeValue < 0x46000) {
            surfaceCue = 0;
        }
        if (surfaceCue >= 9) {
            player->state.modeValue = 0x50000;
        }
        player->state.grabTrickId = 0;
        switch (surfaceCue) {
            case 1:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_TRICK);
                }
                player->state.mode = 0xD;
                player->state.aerialTrickId = 1;
                player->state.stateFlags |= 0x800;
                break;

            case 2:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_TRICK);
                }
                player->state.mode = 0xF;
                player->state.aerialTrickId = 2;
                player->state.stateFlags |= 0x800;
                break;

            case 3:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_TRICK);
                }
                player->state.mode = 0x10;
                player->state.aerialTrickId = 3;
                player->state.stateFlags |= 0x800;
                break;

            case 4:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_TRICK);
                }
                player->state.mode = 0x11;
                player->state.aerialTrickId = 4;
                player->state.stateFlags |= 0x800;
                break;

            case 5:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x12;
                player->state.aerialTrickId = 5;
                player->state.stateFlags |= 0x800;
                break;

            case 6:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x13;
                player->state.aerialTrickId = 6;
                player->state.stateFlags |= 0x800;
                break;

            case 7:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x14;
                player->state.aerialTrickId = 7;
                player->state.stateFlags |= 0x800;
                break;

            case 8:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x15;
                player->state.aerialTrickId = 8;
                player->state.stateFlags |= 0x800;
                break;

            case 9:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x1F;
                player->state.aerialTrickId = 0xB;
                player->state.stateFlags |= 0x800;
                break;

            case 10:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x20;
                player->state.aerialTrickId = 0xB;
                player->state.stateFlags |= 0x800;
                break;

            case 11:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x21;
                player->state.aerialTrickId = 0xC;
                player->state.stateFlags |= 0x800;
                break;

            case 12:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x22;
                player->state.aerialTrickId = 0xD;
                player->state.stateFlags |= 0x800;
                break;

            case 13:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x23;
                player->state.aerialTrickId = 0xA;
                player->state.stateFlags |= 0x800;
                break;

            case 14:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x24;
                player->state.aerialTrickId = 0xC;
                player->state.stateFlags |= 0x800;
                break;

            case 15:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x25;
                player->state.aerialTrickId = 0xB;
                player->state.stateFlags |= 0x800;
                break;

            case 16:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x26;
                player->state.aerialTrickId = 0xD;
                player->state.stateFlags |= 0x800;
                break;

            case 17:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x27;
                player->state.aerialTrickId = 0xE;
                player->state.stateFlags |= 0x800;
                break;

            case 18:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x29;
                player->state.aerialTrickId = 9;
                player->state.stateFlags |= 0x800;
                break;

            case 19:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x2A;
                player->state.aerialTrickId = 0xA;
                player->state.stateFlags |= 0x800;
                break;

            case 20:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x2B;
                player->state.aerialTrickId = 0xC;
                player->state.stateFlags |= 0x800;
                break;

            case 21:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x2C;
                player->state.aerialTrickId = 9;
                player->state.stateFlags |= 0x800;
                break;

            case 22:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x2D;
                player->state.aerialTrickId = 0xA;
                player->state.stateFlags |= 0x800;
                break;

            case 23:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x2E;
                player->state.aerialTrickId = 0xE;
                player->state.stateFlags |= 0x800;
                break;

            case 24:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x2F;
                player->state.aerialTrickId = 9;
                player->state.stateFlags |= 0x800;
                break;

            case 25:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x30;
                player->state.aerialTrickId = 0xB;
                player->state.stateFlags |= 0x800;
                break;

            case 26:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x31;
                player->state.aerialTrickId = 0xD;
                player->state.stateFlags |= 0x800;
                break;

            case 27:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x32;
                player->state.aerialTrickId = 9;
                player->state.stateFlags |= 0x800;
                break;

            case 28:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x33;
                player->state.aerialTrickId = 0xA;
                player->state.stateFlags |= 0x800;
                break;

            case 29:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x34;
                player->state.aerialTrickId = 0xC;
                player->state.stateFlags |= 0x800;
                break;

            case 30:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x35;
                player->state.aerialTrickId = 0xA;
                player->state.stateFlags |= 0x800;
                break;

            case 31:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x36;
                player->state.aerialTrickId = 0xC;
                player->state.stateFlags |= 0x800;
                break;

            case 32:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x37;
                player->state.aerialTrickId = 9;
                player->state.stateFlags |= 0x800;
                break;

            case 33:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x38;
                player->state.aerialTrickId = 0xA;
                player->state.stateFlags |= 0x800;
                break;

            case 34:
                if (player->soundDisabled == 0) {
                    enqueuePositionalSoundEffect(SFX_TRICK_START, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);
                    enqueueRacePlayerVoiceSound(player, RACE_VOICE_COMPLEX_TRICK);
                }
                player->state.mode = 0x39;
                player->state.aerialTrickId = 0xD;
                player->state.stateFlags |= 0x800;
                break;

            case 0:
            default:
                player->state.mode = 1;
                player->state.aerialTrickId = 0;
                break;
        }

        player->state.updateState = 0;
        player->state.updateTimer = 0;
        player->state.velocity.y += player->state.modeValue;
        player->state.pos.y += 0x60000;
        player->state.verticalVelocityLimit = player->state.velocity.y;
        player->state.stateFlags |= 0x208;
        setRaceMotionAnimation(player, 4);
    } else {
        updateRacePlayerSurfaceCue(player);
    }
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += player->state.velocity.y;
    player->state.pos.z += player->state.velocity.z;
    stepRaceMotionJointAnimationUntilEnd(player);
    updateRacePlayerMotionFeedback(player);
    if (player->state.unk517 != 0) {
        enqueueRacePlayerVoiceSound(player, RACE_VOICE_ATTACK_SUCCESS);
    }
}

void resetRacePlayerTrickSubstate(RacePlayer *player) {
    player->state.subState = 0;
    player->state.subStateTimer = 0;
    player->state.subStateStep = 0;
    player->state.subStateParam = 0;
    player->state.grabInputFrames = 0;
}

void updateRacePlayerTrickSubstate(RacePlayer *player) {
    gRacePlayerTrickSubstateHandlers[player->state.subState](player);
}

void updateRacePlayerTrickSubstateStart(RacePlayer *player) {
    s16 inputMask;

    player->state.subStateTimer = 0;
    player->state.subStateStep = 0;
    player->state.grabTrickId = 0;
    player->state.grabHoldFrames = 0;
    if (player->isCpu == 0) {
        if (player->state.pressedInput & R_CBUTTONS) {
            player->state.subStateTimer = R_CBUTTONS;
            player->state.grabTrickId = 1;
        }
        if (player->state.pressedInput & L_CBUTTONS) {
            player->state.subStateTimer = L_CBUTTONS;
            player->state.grabTrickId = 3;
        }
        if (player->state.pressedInput & U_CBUTTONS) {
            player->state.subStateTimer = U_CBUTTONS;
            player->state.grabTrickId = 7;
        }
        if (player->state.pressedInput & D_CBUTTONS) {
            player->state.subStateTimer = D_CBUTTONS;
            player->state.grabTrickId = 5;
        }
        if (player->state.subStateTimer != 0) {
            if (player->state.grabInputFrames < 6) {
                player->state.grabTrickId++;
            }
            player->state.subState++;
            if (player->state.stateFlags & 0x400) {
                inputMask = player->state.subStateTimer;
                if (inputMask & R_CBUTTONS) {
                    setRaceMotionAnimation(player, 0x17);
                    inputMask = player->state.subStateTimer;
                }
                if (inputMask & L_CBUTTONS) {
                    setRaceMotionAnimation(player, 0x15);
                    inputMask = player->state.subStateTimer;
                }
            } else {
                inputMask = player->state.subStateTimer;
                if (inputMask & R_CBUTTONS) {
                    setRaceMotionAnimation(player, 0x15);
                    inputMask = player->state.subStateTimer;
                }
                if (inputMask & L_CBUTTONS) {
                    setRaceMotionAnimation(player, 0x17);
                    inputMask = player->state.subStateTimer;
                }
            }
            if (inputMask & U_CBUTTONS) {
                setRaceMotionAnimation(player, 0x19);
                inputMask = player->state.subStateTimer;
            }
            if (inputMask & D_CBUTTONS) {
                setRaceMotionAnimation(player, 0x1B);
            }
            player->state.stateFlags |= 0x800;
        }
        player->state.grabInputFrames++;
    }
}

void updateRacePlayerTrickSubstateHold(RacePlayer *player) {
    s16 inputMask;

    if (!(player->state.heldInput & player->state.subStateTimer)) {
        player->state.subStateStep = 1;
    }
    if (stepRaceMotionAnimationUntilEnd(player) != 0) {
        if (player->state.subStateStep != 0) {
            player->state.subState++;
            if (player->state.stateFlags & 0x400) {
                inputMask = player->state.subStateTimer;
                if (inputMask & R_CBUTTONS) {
                    setRaceMotionAnimation(player, 0x18);
                    inputMask = player->state.subStateTimer;
                }
                if (inputMask & L_CBUTTONS) {
                    setRaceMotionAnimation(player, 0x16);
                    inputMask = player->state.subStateTimer;
                }
            } else {
                inputMask = player->state.subStateTimer;
                if (inputMask & R_CBUTTONS) {
                    setRaceMotionAnimation(player, 0x16);
                    inputMask = player->state.subStateTimer;
                }
                if (inputMask & L_CBUTTONS) {
                    setRaceMotionAnimation(player, 0x18);
                    inputMask = player->state.subStateTimer;
                }
            }
            if (inputMask & U_CBUTTONS) {
                setRaceMotionAnimation(player, 0x1A);
                inputMask = player->state.subStateTimer;
            }
            if (inputMask & D_CBUTTONS) {
                setRaceMotionAnimation(player, 0x1C);
            }
        } else {
            player->state.grabHoldFrames++;
        }
    }
    player->state.stateFlags |= 0x800;
}

void updateRacePlayerTrickSubstateFinish(RacePlayer *player) {
    if (stepRaceMotionAnimationUntilEnd(player) != 0) {
        player->state.subState++;
    }
    player->state.stateFlags |= 0x800;
}

void updateRacePlayerTrickSubstateNoop(RacePlayer *player) {
}

void updateRacePlayerMode13AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 stateTimer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }

    if (player->state.subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }

    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    player->state.modelRotationX = (s16)((s32)(fixedSine(player->state.modeValue) * 0x1000) / 0x1000);
    player->state.modeValue += 0x20;
    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }
    stateTimer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (stateTimer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(
                initRacePlayerYellowTrickSparkles,
                5,
                2,
                (u16)player->playerIndex
            );
        }
    }
}

void updateRacePlayerMode15AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 stateTimer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }

    if (player->state.subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }

    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    player->state.modelRotationX = (s16)((s32)(fixedSine(player->state.modeValue) * -0x1000) / 0x1000);
    player->state.modeValue += 0x20;
    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }
    stateTimer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (stateTimer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(
                initRacePlayerYellowTrickSparkles,
                5,
                2,
                (u16)player->playerIndex
            );
        }
    }
}

void updateRacePlayerMode16AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }
    if (player->state.subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }
    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);
    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;
    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = (s16)((s32)(fixedSine(player->state.modeValue) << 0xC) / 4096);
    } else {
        player->state.modelRotationY = (s16)((s32)(fixedSine(player->state.modeValue) * -0x1000) / 4096);
    }

    player->state.modeValue += 0x1E;
    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }
    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(
                initRacePlayerYellowTrickSparkles,
                5,
                2,
                (u16)player->playerIndex
            );
        }
    }
}

void updateRacePlayerMode17AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }
    if (player->state.subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }
    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);
    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;
    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = (s16)((s32)(fixedSine(player->state.modeValue) * -0x1000) / 0x1000);
    } else {
        player->state.modelRotationY = (s16)((s32)(fixedSine(player->state.modeValue) << 12) / 0x1000);
    }

    player->state.modeValue += 0x1D;
    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }
    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(
                initRacePlayerYellowTrickSparkles,
                5,
                2,
                (u16)player->playerIndex
            );
        }
    }
}

void updateRacePlayerMode18AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }
    if (player->state.subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }
    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);
    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;
    player->state.modelRotationX = (s16)((s32)(fixedSine((s16)player->state.modeValue) << 12) / 0x1000);
    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = (s16)((s32)(fixedSine((s16)player->state.modeValue) << 12) / 0x1000);
    } else {
        player->state.modelRotationY = (s16)((s32)(fixedSine((s16)player->state.modeValue) * -0x1000) / 0x1000);
    }

    player->state.modeValue += 0x1A;
    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }
    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(
                initRacePlayerYellowTrickSparkles,
                5,
                2,
                (u16)player->playerIndex
            );
        }
    }
}

void updateRacePlayerMode19AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }
    if (player->state.subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }
    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);
    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;
    player->state.modelRotationX = (s16)((s32)(fixedSine(player->state.modeValue) << 12) / 0x1000);
    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = (s16)((s32)(fixedSine(player->state.modeValue) * -0x1000) / 0x1000);
    } else {
        player->state.modelRotationY = (s16)((s32)(fixedSine(player->state.modeValue) << 12) / 0x1000);
    }

    player->state.modeValue += 0x1A;
    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }
    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(
                initRacePlayerYellowTrickSparkles,
                5,
                2,
                (u16)player->playerIndex
            );
        }
    }
}

void updateRacePlayerMode20AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }
    if (player->state.subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }
    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);
    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;
    player->state.modelRotationX = (s16)((s32)(fixedSine(player->state.modeValue) * -0x1000) / 0x1000);
    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = (s16)((s32)(fixedSine(player->state.modeValue) << 12) / 0x1000);
    } else {
        player->state.modelRotationY = (s16)((s32)(fixedSine(player->state.modeValue) * -0x1000) / 0x1000);
    }

    player->state.modeValue += 0x1A;
    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }
    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(
                initRacePlayerYellowTrickSparkles,
                5,
                2,
                (u16)player->playerIndex
            );
        }
    }
}

void updateRacePlayerMode21AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }
    if (player->state.subState == 0) {
        stepRaceMotionAnimationUntilEnd(player);
    }
    updateRacePlayerTrickSubstate(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);
    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;
    player->state.modelRotationX = (s16)((s32)(fixedSine(player->state.modeValue) * -0x1000) / 0x1000);
    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = (s16)((s32)(fixedSine(player->state.modeValue) * -0x1000) / 0x1000);
    } else {
        player->state.modelRotationY = (s16)((s32)(fixedSine(player->state.modeValue) << 12) / 0x1000);
    }

    player->state.modeValue += 0x1A;
    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }
    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(
                initRacePlayerYellowTrickSparkles,
                5,
                2,
                (u16)player->playerIndex
            );
        }
    }
}

void updateRacePlayerMode31AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x15);
        player->state.updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = (fixedSine(player->state.modeValue) * -0x2000) / 4096;
    } else {
        player->state.modelRotationY = (fixedSine(player->state.modeValue) * 0x2000) / 4096;
    }

    player->state.modeValue += 0x16;
    player->state.updateTimer++;
    updateTimer = player->state.updateTimer;
    if (updateTimer == 8) {
        setRaceMotionAnimation(player, 0x16);
        updateTimer = player->state.updateTimer;
    }
    if (updateTimer == 0xF) {
        setRaceMotionAnimation(player, 0x1B);
        updateTimer = player->state.updateTimer;
    }
    if (updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x1C);
    }

    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode41AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        player->state.updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    player->state.modelRotationX = (fixedSine(player->state.modeValue) << 13) / 0x1000;

    player->state.modeValue += 0x14;
    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode42AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        player->state.updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    player->state.modelRotationZ = (fixedSine(player->state.modeValue) << 13) / 0x1000;

    player->state.modeValue += 0x14;
    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode43AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        player->state.updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);

    updateTimer = player->state.updateTimer;
    if (updateTimer < 8) {
        player->state.pitchAngle = -updateTimer << 7;
    }
    if (updateTimer >= 0x23) {
        player->state.pitchAngle = (-(-updateTimer) << 6) - 0xC80;
    }
    if (updateTimer < 0x32) {
        player->state.updateTimer = updateTimer + 1;
    }

    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    player->state.modelRotationY = (fixedSine(player->state.modeValue) << 14) / 0x1000;

    player->state.modeValue += 0x14;
    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode32AerialTrick(RacePlayer *player) {
    s16 updateState;
    s32 yVel;

    updateState = player->state.updateState;
    if (updateState == 0) {
        player->state.updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x28);
        player->state.modeValue = 0;
        player->state.stateFlags |= 0x200;
        resetRacePlayerTrickSubstate(player);
        player->state.updateTimer = 0;
    }

    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    if (stepRaceMotionAnimationUntilEnd(player) == 0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode33AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 0x23);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        player->state.updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    player->state.modelRotationX = (fixedSine(player->state.modeValue) * -0x3000) / 4096;
    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = -player->state.updateTimer;
    } else {
        player->state.modelRotationY = player->state.updateTimer;
    }
    if (player->state.updateTimer != 0x1000) {
        player->state.updateTimer += 0x80;
    }

    player->state.modeValue += 0x11;
    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode34AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 0x23);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        player->state.updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.modelRotationX = player->state.modeValue * 4;
    player->state.verticalVelocityLimit = yVel;

    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = (fixedSine((s16)player->state.modeValue) * -0x4000) / 0x1000;
    } else {
        player->state.modelRotationY = (fixedSine((s16)player->state.modeValue) << 0xE) / 0x1000;
    }

    player->state.modeValue += 0x15;
    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode44AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x17);
        player->state.updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = (fixedSine(player->state.modeValue) * -0x2000) / 4096;
    } else {
        player->state.modelRotationY = (fixedSine(player->state.modeValue) * 0x2000) / 4096;
    }

    player->state.modeValue += 0x16;
    player->state.updateTimer++;
    if (player->state.updateTimer == 8) {
        setRaceMotionAnimation(player, 0x18);
    }
    if (player->state.updateTimer == 0xF) {
        setRaceMotionAnimation(player, 0x19);
    }
    if (player->state.updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x1A);
    }

    timer = player->state.modeValue;
    if (timer >= 0x401) {
        timer = 0x400;
        player->state.modeValue = timer;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode45AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    player->state.updateTimer = (fixedSine(player->state.modeValue) << 13) / 0x1000;
    updateTimer = player->state.updateTimer;
    if (updateTimer >= 0x1001) {
        player->state.modelRotationY = 0x2000 - updateTimer;
    } else {
        player->state.modelRotationY = updateTimer;
    }

    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = -player->state.modelRotationY;
    }

    player->state.modelRotationX = (fixedSine(player->state.modeValue) << 13) / 0x1000;
    player->state.modeValue += 0x14;
    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode46AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x1B);
        player->state.updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = (fixedSine(player->state.modeValue) * -0x5000) / 0x1000;
    } else {
        player->state.modelRotationY = (fixedSine(player->state.modeValue) * 0x5000) / 0x1000;
    }

    player->state.modeValue += 0x12;
    player->state.updateTimer++;
    if (player->state.updateTimer == 0x28) {
        setRaceMotionAnimation(player, 0x1C);
    }

    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode35AerialTrick(RacePlayer *player) {
    s16 updateState;
    s32 yVel;
    s32 stateTimer;

    updateState = player->state.updateState;
    if (updateState == 0) {
        player->state.updateState = updateState + 1;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x15);
        player->state.updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationZ = (fixedSine(player->state.modeValue) * -0x2000) / 0x1000;
    } else {
        player->state.modelRotationZ = (fixedSine(player->state.modeValue) << 13) / 0x1000;
    }

    player->state.modeValue += 0x16;
    player->state.updateTimer++;
    if (player->state.updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x16);
    }

    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    stateTimer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (stateTimer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode36AerialTrick(RacePlayer *player) {
    s16 updateState;
    s32 yVel;
    s32 timer;

    updateState = player->state.updateState;
    if (updateState == 0) {
        player->state.updateState = updateState + 1;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x15);
        player->state.updateTimer = 0;
    }
    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = (s16)((s32)(fixedSine(player->state.modeValue) * -0x3000) / 4096);
    } else {
        player->state.modelRotationY = (s16)((s32)(fixedSine(player->state.modeValue) * 0x3000) / 4096);
    }

    player->state.modeValue += 0x15;
    player->state.updateTimer++;
    if (player->state.updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x16);
    }

    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode47AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x19);
        player->state.updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = (fixedSine(player->state.modeValue) * -0x2000) / 4096;
    } else {
        player->state.modelRotationY = (fixedSine(player->state.modeValue) << 0xD) / 4096;
    }

    player->state.modeValue += 0x16;
    player->state.updateTimer++;
    updateTimer = player->state.updateTimer;
    if (updateTimer == 8) {
        setRaceMotionAnimation(player, 0x1A);
        updateTimer = player->state.updateTimer;
    }
    if (updateTimer == 0xF) {
        setRaceMotionAnimation(player, 0x15);
        updateTimer = player->state.updateTimer;
    }
    if (updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x16);
    }

    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode48AerialTrick(RacePlayer *player) {
    s16 updateState;
    s32 yVel;
    s32 stateTimer;

    updateState = player->state.updateState;
    if (updateState == 0) {
        player->state.updateState = updateState + 1;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x15);
        player->state.updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationZ = (fixedSine(player->state.modeValue) * 0x3000) / 0x1000;
    } else {
        player->state.modelRotationZ = (fixedSine(player->state.modeValue) * -0x3000) / 0x1000;
    }

    player->state.modeValue += 0x16;
    if (player->state.updateTimer < 0x2D) {
        player->state.updateTimer = player->state.updateTimer + 1;
    }

    if (player->state.updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x16);
    }

    stateTimer = player->state.modeValue;
    player->state.modelRotationY = (player->state.updateTimer << 0xC) / 45;
    if (stateTimer >= 0x401) {
        player->state.modeValue = 0x400;
    }

    stateTimer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (stateTimer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode49AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    player->state.modelRotationX = (fixedSine(player->state.modeValue) * -0x3000) / 0x1000;

    player->state.modeValue += 0x12;
    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
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

    updateState = player->state.updateState;
    if (updateState == 0) {
        player->state.updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x23);
        player->state.modeValue = 0;
        player->state.stateFlags |= 0x200;
        resetRacePlayerTrickSubstate(player);
        player->state.updateTimer = 0;
        player->state.unk306 = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = (u32)player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    updateTimerPtr = &player->state.updateTimer;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationZ = (fixedSine(player->state.modeValue) * -0x2000) / 4096;
    } else {
        player->state.modelRotationZ = (fixedSine(player->state.modeValue) << 0xD) / 4096;
    }

    player->state.modeValue += 0x16;
    player->state.updateTimer++;
    updateTimer = player->state.updateTimer;
    if (updateTimer == 0xC) {
        setRaceMotionAnimation(player, 0x15);
        updateTimer = *updateTimerPtr;
    }

    if (updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x16);
        updateTimer = player->state.updateTimer;
    }

    if ((updateTimer >= 9) && (updateTimer < 0x14)) {
        tilt = player->state.unk306;
        if (tilt != -0x400) {
            player->state.unk306 = tilt - 0x80;
            updateTimer = player->state.updateTimer;
        }
    }

    tilt = player->state.unk306;
    if ((updateTimer >= 0x1F) && (tilt != 0)) {
        player->state.unk306 = tilt + 0x80;
        tilt = player->state.unk306;
    }

    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = -tilt;
    } else {
        player->state.modelRotationY = tilt;
    }

    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode50AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x19);
        player->state.updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = (fixedSine(player->state.modeValue) * -0x2000) / 4096;
    } else {
        player->state.modelRotationY = (fixedSine(player->state.modeValue) * 0x2000) / 4096;
    }

    player->state.modeValue += 0x16;
    player->state.updateTimer++;
    updateTimer = player->state.updateTimer;
    if (updateTimer == 8) {
        setRaceMotionAnimation(player, 0x1A);
        updateTimer = player->state.updateTimer;
    }
    if (updateTimer == 0xF) {
        setRaceMotionAnimation(player, 0x17);
        updateTimer = player->state.updateTimer;
    }
    if (updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x18);
    }

    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
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

    updateState = player->state.updateState;
    if (updateState == 0) {
        player->state.updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x23);
        player->state.modeValue = 0;
        player->state.stateFlags |= 0x200;
        resetRacePlayerTrickSubstate(player);
        player->state.updateTimer = 0;
        player->state.unk306 = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    playerAlias = player;
    playerAlias->state.velocity.y -= playerAlias->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&playerAlias->state.velocity, player);

    yVel = player->state.velocity.y;
    playerAlias->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    playerAlias->state.pos.z += player->state.velocity.z;
    playerAlias->state.verticalVelocityLimit = yVel;

    if (playerAlias->state.stateFlags & 0x400) {
        playerAlias->state.modelRotationZ = (fixedSine((s16)player->state.modeValue) * 0x3000) / 4096;
    } else {
        player->state.modelRotationZ = (fixedSine((s16)player->state.modeValue) * -0x3000) / 4096;
    }

    playerAlias->state.modeValue += 0x14;
    player->state.updateTimer++;
    updateTimer = player->state.updateTimer;
    if (updateTimer == 0xC) {
        setRaceMotionAnimation(playerAlias, 0x15);
        updateTimer = playerAlias->state.updateTimer;
    }

    if (updateTimer == 0x1E) {
        setRaceMotionAnimation(playerAlias, 0x16);
        updateTimer = playerAlias->state.updateTimer;
    }

    if ((updateTimer >= 9) && (updateTimer < 0x14)) {
        tilt = player->state.unk306;
        if (tilt != 0x400) {
            player->state.unk306 = tilt + 0x80;
            updateTimer = playerAlias->state.updateTimer;
        }
    }

    tilt = player->state.unk306;
    if ((updateTimer >= 0x1F) && (tilt != 0)) {
        player->state.unk306 = tilt - 0x80;
        tilt = player->state.unk306;
    }

    if (playerAlias->state.stateFlags & 0x400) {
        playerAlias->state.modelRotationY = -tilt;
    } else {
        player->state.modelRotationY = tilt;
    }

    timer = player->state.modeValue;
    if (timer >= 0x401) {
        timer = 0x400;
        playerAlias->state.modeValue = timer;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        playerAlias->state.stateFlags |= 0x800;
        if ((playerAlias->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(
                initRacePlayerBlueTrickSparkles,
                5,
                2,
                (u16)playerAlias->playerIndex
            );
        }
    }
}

void updateRacePlayerMode52AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 stateTimer;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x1B);
        player->state.updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = (fixedSine(player->state.modeValue) * -0x3000) / 0x1000;
    } else {
        player->state.modelRotationY = (fixedSine(player->state.modeValue) * 0x3000) / 0x1000;
    }

    player->state.modeValue += 0x16;
    player->state.updateTimer++;
    if (player->state.updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x1C);
    }

    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode38AerialTrick(RacePlayer *player) {
    s16 updateState;
    s32 yVel;

    updateState = player->state.updateState;
    if (updateState == 0) {
        player->state.updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x23);
        player->state.modeValue = 0;
        player->state.stateFlags |= 0x200;
        resetRacePlayerTrickSubstate(player);
        player->state.updateTimer = 0;
    }

    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    if (stepRaceMotionAnimationUntilEnd(player) == 0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode53AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        setRaceMotionAnimation(player, 0x19);
        player->state.updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = (fixedSine(player->state.modeValue) * -0x1800) / 4096;
    } else {
        player->state.modelRotationY = ((fixedSine(player->state.modeValue) * 6) * 1024) / 4096;
    }

    player->state.modeValue += 0x16;
    player->state.updateTimer++;
    updateTimer = player->state.updateTimer;
    if (updateTimer == 8) {
        setRaceMotionAnimation(player, 0x1A);
        updateTimer = player->state.updateTimer;
    }
    if (updateTimer == 0xF) {
        setRaceMotionAnimation(player, 0x1B);
        updateTimer = player->state.updateTimer;
    }
    if (updateTimer == 0x1E) {
        setRaceMotionAnimation(player, 0x1C);
    }

    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode54AerialTrick(RacePlayer *player) {
    s16 updateState;
    s32 yVel;

    updateState = player->state.updateState;
    if (updateState == 0) {
        player->state.updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x28);
        player->state.modeValue = 0;
        player->state.stateFlags |= 0x200;
        resetRacePlayerTrickSubstate(player);
        player->state.updateTimer = 0;
    }

    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    if (stepRaceMotionAnimationUntilEnd(player) == 0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode39AerialTrick(RacePlayer *player) {
    s16 updateTimer;
    s32 yVel;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
        player->state.updateTimer = 0;
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    updateTimer = player->state.updateTimer;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    switch (updateTimer) {
        case 0:
            player->state.modelRotationX = (s16)((s32)(fixedSine(player->state.modeValue) << 12) / 4096);
            player->state.modeValue += 0x33;
            if (player->state.modeValue >= 0x401) {
                player->state.modeValue = 0;
                player->state.updateTimer++;
                player->state.unk306 = 0;
            }

            player->state.stateFlags |= 0x800;
            if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
                createCallbackTaskWithUserIdPreservingArgs(
                    initRacePlayerBlueTrickSparkles,
                    5,
                    2,
                    (u16)player->playerIndex
                );
            }
            break;
        case 1:
            if (player->state.stateFlags & 0x400) {
                player->state.modelRotationY = (s16)((s32)(fixedSine(player->state.modeValue) * -0x2000) / 4096);
            } else {
                player->state.modelRotationY = (s16)((s32)(fixedSine(player->state.modeValue) << 13) / 4096);
            }

            player->state.modelRotationX = -player->state.unk306;
            player->state.modeValue += 0x20;
            if (player->state.modeValue >= 0x401) {
                player->state.modeValue = 0x400;
            }

            if (player->state.unk306 != 0x1000) {
                player->state.unk306 += 0x80;
            }

            if (player->state.modeValue < 0x3D0) {
                player->state.stateFlags |= 0x800;
                if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
                    createCallbackTaskWithUserIdPreservingArgs(
                        initRacePlayerBlueTrickSparkles,
                        5,
                        2,
                        (u16)player->playerIndex
                    );
                }
            }
            break;
    }

    player->state.stateFlags |= 2;
}

void updateRacePlayerMode55AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = (s16)((s32)(fixedSine(player->state.modeValue) * -0x2000) / 4096);
    } else {
        player->state.modelRotationY = (s16)((s32)(fixedSine(player->state.modeValue) << 0xD) / 4096);
    }

    player->state.modeValue += 0x16;
    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode56AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    if (player->state.stateFlags & 0x400) {
        player->state.modelRotationY = (fixedSine(player->state.modeValue) * -0x5000) / 0x1000;
    } else {
        player->state.modelRotationY = (fixedSine(player->state.modeValue) * 0x5000) / 0x1000;
    }

    player->state.modeValue += 0x14;
    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode57AerialTrick(RacePlayer *player) {
    s32 yVel;
    s32 timer;

    if (player->state.updateState == 0) {
        setRaceMotionAnimation(player, 4);
        player->state.updateState++;
        player->state.stateFlags |= 0x200;
        player->state.modeValue = 0;
        resetRacePlayerTrickSubstate(player);
    }

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y -= player->state.aerialGravity;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

    yVel = player->state.velocity.y;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.verticalVelocityLimit = yVel;

    player->state.modelRotationX = (fixedSine(player->state.modeValue) * -0x5000) / 0x1000;

    player->state.modeValue += 0x14;
    if (player->state.modeValue >= 0x401) {
        player->state.modeValue = 0x400;
    }

    timer = player->state.modeValue;
    player->state.stateFlags |= 2;
    if (timer < 0x3D0) {
        player->state.stateFlags |= 0x800;
        if ((player->soundDisabled == 0) && (gFrameCounter & 1)) {
            createCallbackTaskWithUserIdPreservingArgs(initRacePlayerBlueTrickSparkles, 5, 2, (u16)player->playerIndex);
        }
    }
}

void updateRacePlayerMode03Nudge(RacePlayer *player) {
    RacePlayer *playerAlias;
    RaceCourseStartEntry *spawn;
    s16 updateState;
    s16 angleDiff;
    s32 clamped;
    s32 tempX;
    s32 tempZ;

    updateState = player->state.updateState;
    if (updateState == 0) {
        player->state.updateState = updateState + 1;
        player->state.stateFlags &= 0xFE0C1FFB;
        player->state.stateFlags |= 0x2000;
        if (((player->state.unk2DE - player->state.facingAngle) & 0xFFF) < 0x800) {
            setRaceMotionAnimation(player, 7);
        } else {
            setRaceMotionAnimation(player, 6);
        }
        player->state.velocity.x = player->state.unk2E0;
        player->state.velocity.z = player->state.unk2E4;
        player->state.rumbleTimer = 3;
        player->state.rumblePatternId = 0;
    }

    updateRacePlayerLeanAngle(player, 0x10000, 0);
    playerAlias = player;
    player->state.speedLimit = 0x60000;
    if (!(playerAlias->state.stateFlags & 1)) {
        playerAlias->state.stateFlags &= ~0x200;
        playerAlias->state.velocity.y -= 0xA000;
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, (unsigned long long)0x2000, 0x2000, 0x2000);
    } else {
        playerAlias->state.stateFlags |= 0x200;
        player->state.velocity.y -= 0x7000;
        clampRacePlayerVectorXZSpeed(&player->state.velocity, player);

        tempX = playerAlias->state.velocity.x;
        clamped = tempX;
        if (clamped >= 0x2001) {
            clamped = 0x2000;
        }
        updateState = -0x2000;
        if (clamped < -0x2000) {
            clamped = updateState;
        }
        player->state.velocity.x = tempX - clamped;

        tempZ = playerAlias->state.velocity.z;
        clamped = tempZ;
        if (clamped >= 0x2001) {
            clamped = 0x2000;
        }
        if (clamped < updateState) {
            clamped = updateState;
        }
        player->state.velocity.z = tempZ - clamped;
    }

    playerAlias->state.pos.x += playerAlias->state.velocity.x;
    playerAlias->state.pos.y += player->state.velocity.y;
    playerAlias->state.pos.z += playerAlias->state.velocity.z;

    spawn = &gRaceCourseStartEntries[gRaceCourseIndex.signedValue];
    if ((spawn->pathIndex == playerAlias->state.coursePathIndex) && !(playerAlias->state.stateFlags & 0x40)) {
        angleDiff = calculateFixedAngleBetweenXZPoints(
            playerAlias->state.pos.x,
            player->state.pos.z,
            spawn->startSurfaceTargetX,
            spawn->startSurfaceTargetZ
        );
        if (playerAlias->state.stateFlags & 0x400) {
            angleDiff += 0x800;
        }

        angleDiff = (angleDiff - player->state.facingAngle) & 0xFFF;
        if (angleDiff >= 0x801) {
            angleDiff -= 0x1000;
        }
        if (angleDiff >= 0x97) {
            angleDiff = 0x96;
        }
        if (angleDiff < -0x96) {
            angleDiff = -0x96;
        }
        playerAlias->state.facingAngle += angleDiff;
    }

    if (!(player->state.stateFlags & 1)) {
        updateRacePlayerMotionFeedback(player);
    }

    if (stepRaceMotionAnimationUntilEnd(player) != 0) {
        playerAlias->state.stateFlags &= 0xFE0C1FFB;
        if (playerAlias->state.stateFlags & 0x200) {
            player->state.mode = 1;
            playerAlias->state.updateState = 0;
            player->state.updateTimer = 0;
        } else {
            playerAlias->state.mode = 0;
            playerAlias->state.updateState = 0;
            playerAlias->state.updateTimer = 0;
        }
    }
}

void updateRacePlayerMode04Spinout(RacePlayer *player) {
    s16 updateState;
    s32 tempX;
    s32 tempZ;

    updateState = player->state.updateState;
    if (updateState == 0) {
        player->state.updateState = updateState + 1;
        setRaceMotionAnimation(player, 0xE);
        player->state.stateFlags &= 0xFE0C1FFB;
        player->state.stateFlags |= 0x12204;
        player->state.pitchAngle = 0;
        updateRacePlayerLeanAngle(player, 0, 0);
        player->state.velocity.x = player->state.unk2E0;
        player->state.velocity.z = player->state.unk2E4;
        player->state.velocity.y = 0x20000;
        player->state.targetGroundOffset = 0;
        player->state.rumbleTimer = 3;
        player->state.rumblePatternId = 0;
    }

    player->state.speedLimit = 0x60000;
    if (player->state.stateFlags & 1) {
        clampRacePlayerVectorXZSpeed(&player->state.velocity, player);
        tempX = player->state.velocity.x;
        tempZ = player->state.velocity.z;
        player->state.velocity.y -= 0xA000;
        player->state.velocity.x = tempX - (tempX / 15);
        player->state.velocity.z = tempZ - (tempZ / 15);
    } else {
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x3000, 0x3000, 0x3000);
    }

    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += player->state.velocity.y;
    player->state.pos.z += player->state.velocity.z;

    if ((stepRaceMotionAnimationUntilEnd(player) != 0) && !(player->state.stateFlags & 1)) {
        player->state.mode = 5;
        player->state.updateState = 0;
        player->state.updateTimer = 0;
    }
}

void updateRacePlayerMode05SpinoutStun(RacePlayer *player) {
    s16 updateState;
    s32 stateTimer;

    updateState = player->state.updateState;
    if (updateState == 0) {
        player->state.updateState = updateState + 1;
        player->state.modeValue = 0x1E;
        if (player->state.stateFlags & 0x800000) {
            player->state.modeValue = 0x5A;
            spawnRaceUiStunOrbitingIcons(player->playerIndex);
        }
        setRaceMotionAnimation(player, 0xF);
        stateTimer = player->state.modeValue;
        player->state.stateFlags &= 0xFE0C1FFB;
        player->state.stateFlags &= ~0x200;
        player->state.stateFlags |= 0x1012000;
        player->state.targetGroundOffset = 0;
        player->state.modeValue = stateTimer;
        /* Rank-scaled recovery timer for humans and CPUs. With zero-based rank,
         * integer truncation gives 60/53/45/38 updates for a base of 60. */
        player->state.modeValue = stateTimer - ((player->state.modeValue * player->state.rankIndex) / 8);
        player->state.rumbleTimer = 1;
        player->state.rumblePatternId = 0;
    }

    updateRacePlayerLeanAngle(player, 0, 0);
    if (!(player->state.stateFlags & 1)) {
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x4000, 0x4000, 0x4000);
    }

    player->state.velocity.y -= 0xA000;
    player->state.pos.y += player->state.velocity.y;

    if (!(player->state.stateFlags & 1)) {
        player->state.modeValue = player->state.modeValue - getRaceInputTimerDecrementBonus(player) - 1;
        if (player->state.modeValue < 0) {
            player->state.modeValue = 0;
        }
        if (stepRaceMotionAnimationUntilEnd(player) != 0) {
            player->state.stateFlags &= ~0x200;
            if (player->state.modeValue == 0) {
                player->state.mode = 8;
                player->state.updateState = 0;
                player->state.updateTimer = 0;
            }
        }
    } else {
        player->state.stateFlags |= 0x200;
        setRaceMotionAnimation(player, 0xF);
        stepRaceMotionAnimationUntilEnd(player);
        if (player->state.modeValue < 0x1E) {
            player->state.modeValue = 0x1E;
        }
        if (!(player->state.stateFlags & 1)) {
            player->state.rumbleTimer = 1;
            player->state.rumblePatternId = 0;
        }
    }
}

void updateRacePlayerMode08SpinoutRecover(RacePlayer *player) {
    RaceCourseStartEntry *spawn;
    s16 updateState;
    s16 targetAngle;
    s16 facingAngle;

    updateState = player->state.updateState;
    if (updateState == 0) {
        player->state.updateState = updateState + 1;
        if (player->state.stateFlags & 0x10000) {
            setRaceMotionAnimation(player, 0x11);
        } else {
            setRaceMotionAnimation(player, 0x10);
        }
        player->state.stateFlags = player->state.stateFlags & 0xFE0C1FFB;
        player->state.stateFlags = player->state.stateFlags & ~0x200;
        player->state.stateFlags = player->state.stateFlags | 0xA000;
        player->state.targetGroundOffset = 0x40000;
    }

    if (player->state.stateFlags & 1) {
        player->state.stateFlags |= 0x200;
    } else {
        player->state.stateFlags &= ~0x200;
    }

    spawn = &gRaceCourseStartEntries[gRaceCourseIndex.signedValue];
    if ((spawn->pathIndex == player->state.coursePathIndex) && !(player->state.stateFlags & 0x40)) {
        targetAngle = calculateFixedAngleBetweenXZPoints(
            player->state.pos.x,
            player->state.pos.z,
            spawn->startSurfaceTargetX,
            spawn->startSurfaceTargetZ
        );
        if (player->state.stateFlags & 0x400) {
            targetAngle += 0x800;
        }

        facingAngle = player->state.facingAngle;
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
        player->state.facingAngle = facingAngle + targetAngle;
    }

    updateRacePlayerLeanAngle(player, 0, 0);
    player->state.velocity.y -= 0xA000;
    player->state.pos.y += player->state.velocity.y;

    if (stepRaceMotionAnimationUntilEnd(player) != 0) {
        player->state.stateFlags &= 0xFE0C1FFB;
        player->state.mode = 0;
        player->state.updateState = 0;
        player->state.updateTimer = 0;
    }
}

void updateRacePlayerMode06TerrainFall(RacePlayer *player) {
    if (player->state.updateState == 0) {
        player->state.updateState++;
        player->state.modeValue = 0x3C;
        player->state.stateFlags &= 0xFE0C1FFB;
        player->state.stateFlags |= 0x42000;
        player->state.updateTimer = 0;
        setRaceCameraMode(player->playerIndex, 4);
        player->state.targetGroundOffset = 0;
        player->state.unk2E8 = player->state.coursePathIndex;
        if (player->soundDisabled == 0) {
            spawnRaceItemBreakParticles(player->playerIndex, player->state.surfaceType);
        }
        /* Rank-scaled recovery timer for humans and CPUs. With zero-based rank,
         * integer truncation gives 60/53/45/38 updates for a base of 60. */
        player->state.modeValue -= (player->state.modeValue * player->state.rankIndex) / 8;
        player->state.rumbleTimer = 2;
        player->state.rumblePatternId = 0;
        setRaceMotionAnimation(player, 0x20);
    }

    switch (player->state.updateTimer) {
        case 0:
            player->state.modeValue--;
            if (player->state.modeValue == 0) {
                player->state.updateTimer++;
            }
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                setRaceMotionAnimation(player, 0x21);
            }
            break;
        case 1:
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                setRaceMotionAnimation(player, 0x21);
            }
            player->state.stateFlags |= 0x80000;
            if (gViewportStates[(u16)player->playerIndex].overlayAlpha == 0xFF) {
                player->state.updateTimer++;
                player->state.verticalVelocityLimit = 0;
                player->state.coursePathIndex = player->state.unk2E8;
                do {
                    getRaceCourseSurfaceSpawnTransform(
                        player->state.coursePathIndex,
                        &player->state.pos.x,
                        &player->state.pos.y,
                        &player->state.pos.z,
                        &player->state.facingAngle
                    );
                    if (isRacePlayerRespawnSurfaceValid(player) == 0) {
                        player->state.coursePathIndex--;
                    } else {
                        break;
                    }
                } while (1);
                player->state.pos.y = getRaceCourseSurfaceHeight(player->state.coursePathIndex, player->state.pos.x, player->state.pos.z);
                player->state.previousPosition = player->state.pos;
                player->state.velocity.x = 0;
                player->state.velocity.y = 0;
                player->state.velocity.z = 0;
                player->state.stateFlags &= ~0x400;
                player->state.leanAngle = 0;
                updateRacePlayerLeanAngle(player, 0, 0);
                setRaceMotionAnimation(player, 1);
                stepRaceMotionAnimationUntilEnd(player);
                setRaceCameraMode(player->playerIndex, 1);
                player->state.targetGroundOffset = 0x40000;
            }
            break;
        case 2:
            player->state.stateFlags &= 0xFFF7FFFF;
            if (gViewportStates[(u16)player->playerIndex].overlayAlpha == 0) {
                player->state.mode = 0;
                player->state.updateState = 0;
                player->state.updateTimer = 0;
                player->state.stateFlags = 0;
            }
            stepRaceMotionAnimationUntilEnd(player);
            break;
    }

    player->state.velocity.y -= player->state.gravity;
    player->state.pos.y += player->state.velocity.y;
}

void updateRacePlayerMode28TerrainFallWithItemEffect(RacePlayer *player) {
    s16 updateState;

    player->state.velocity.y -= player->state.gravity;
    player->state.pos.y += player->state.velocity.y;
    updateState = player->state.updateState;

    switch (updateState) {
        case 0:
            player->state.updateState = updateState + 1;
            player->state.stateFlags &= 0xFE0C1FFB;
            player->state.stateFlags |= 0x42000;
            setRaceMotionAnimation(player, 0xE);
            stepRaceMotionAnimationUntilEnd(player);
            player->state.unk80 = player->state.coursePathIndex + 2;
            setRaceCameraMode(player->playerIndex, 4);
            player->state.targetGroundOffset = 0;
            if (player->soundDisabled == 0) {
                spawnRaceItemBreakParticles(player->playerIndex, player->state.surfaceType);
            }
            player->state.unk2E8 = player->state.coursePathIndex;
            player->state.rumbleTimer = 2;
            player->state.rumblePatternId = 0;
            /* fallthrough */
        case 1:
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                player->state.updateState++;
                setRaceMotionAnimation(player, 0xF);
            }
            updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
            player->state.speedLimit = 0x20000;
            updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x2000, 0x2000, 0x2000);
            player->state.pos.x += player->state.velocity.x;
            player->state.pos.y += player->state.velocity.y;
            player->state.pos.z += player->state.velocity.z;
            break;
        case 2:
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                player->state.updateState++;
                /* Rank-scaled recovery timer for humans and CPUs. With zero-based rank,
                 * integer truncation gives 60/53/45/38 updates for a base of 60. */
                player->state.modeValue = 0x3C - ((player->state.rankIndex * 0x3C) / 8);
                setRaceMotionAnimation(player, 0x20);
            }
            updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
            player->state.speedLimit = 0x20000;
            updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x2000, 0x2000, 0x2000);
            player->state.pos.x += player->state.velocity.x;
            player->state.pos.y += player->state.velocity.y;
            player->state.pos.z += player->state.velocity.z;
            break;
        case 3:
            player->state.modeValue--;
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                setRaceMotionAnimation(player, 0x21);
            }
            if (player->state.modeValue == 0) {
                player->state.updateState++;
            }
            break;
        case 4:
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                setRaceMotionAnimation(player, 0x21);
            }
            player->state.stateFlags |= 0x80000;
            if (gViewportStates[(u16)player->playerIndex].overlayAlpha == 0xFF) {
                player->state.updateState++;
                player->state.coursePathIndex = player->state.unk2E8;
            loop:
                getRaceCourseSurfaceSpawnTransform(
                    player->state.coursePathIndex,
                    &player->state.pos.x,
                    &player->state.pos.y,
                    &player->state.pos.z,
                    &player->state.facingAngle
                );
                if (isRacePlayerRespawnSurfaceValid(player) == 0) {
                    player->state.coursePathIndex--;
                    goto loop;
                }
                player->state.pos.y = getRaceCourseSurfaceHeight(player->state.coursePathIndex, player->state.pos.x, player->state.pos.z);
                player->state.previousPosition = player->state.pos;
                player->state.velocity.x = 0;
                player->state.velocity.y = 0;
                player->state.velocity.z = 0;
                player->state.stateFlags &= ~0x400;
                player->state.leanAngle = 0;
                updateRacePlayerLeanAngle(player, 0, 0);
                setRaceMotionAnimation(player, 1);
                stepRaceMotionAnimationUntilEnd(player);
                setRaceCameraMode(player->playerIndex, 1);
                player->state.targetGroundOffset = 0x40000;
            }
            break;
        case 5:
            player->state.stateFlags &= 0xFFF7FFFF;
            if (gViewportStates[(u16)player->playerIndex].overlayAlpha == 0) {
                player->state.mode = 0;
                player->state.updateState = 0;
                player->state.updateTimer = 0;
                stepRaceMotionAnimationUntilEnd(player);
                player->state.stateFlags = 0;
            }
            break;
    }
}

void updateRacePlayerMode09TerrainCrash(RacePlayer *player) {
    s16 sine;
    s16 cosine;
    s16 updateState;

    player->state.velocity.y -= player->state.gravity;
    player->state.pos.y += player->state.velocity.y;
    updateState = player->state.updateState;

    switch (updateState) {
        case 0:
            player->state.updateState = updateState + 1;
            player->state.stateFlags &= 0xFE0C1FFB;
            player->state.stateFlags |= 0x42000;
            setRaceMotionAnimation(player, 0xE);
            stepRaceMotionAnimationUntilEnd(player);
            player->state.unk80 = player->state.coursePathIndex + 2;
            setRaceCameraMode((u16)player->playerIndex, 4);
            player->state.targetGroundOffset = 0;
            if (player->soundDisabled == 0) {
                spawnRaceItemBreakParticles(player->playerIndex, player->state.surfaceType);
            }
            player->state.unk2E8 = player->state.coursePathIndex;
            player->state.rumbleTimer = 2;
            player->state.rumblePatternId = 0;
            /* fallthrough */
        case 1:
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                player->state.updateState++;
                setRaceMotionAnimation(player, 0xF);
            }
            updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
            player->state.speedLimit = 0x20000;
            updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x2000, 0x2000, 0x2000);
            player->state.pos.x += player->state.velocity.x;
            player->state.pos.y += player->state.velocity.y;
            player->state.pos.z += player->state.velocity.z;
            break;
        case 2:
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                player->state.updateState++;
                /* Rank-scaled recovery timer for humans and CPUs. With zero-based rank,
                 * integer truncation gives 60/53/45/38 updates for a base of 60. */
                player->state.modeValue = 0x3C - ((player->state.rankIndex * 0x3C) / 8);
                setRaceMotionAnimation(player, 0x20);
            }
            updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
            player->state.speedLimit = 0x20000;
            updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x2000, 0x2000, 0x2000);
            player->state.pos.x += player->state.velocity.x;
            player->state.pos.y += player->state.velocity.y;
            player->state.pos.z += player->state.velocity.z;
            break;
        case 3:
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                setRaceMotionAnimation(player, 0x21);
            }
            player->state.modeValue--;
            if (player->state.modeValue == 0) {
                player->state.updateState++;
            }
            break;
        case 4:
            if (stepRaceMotionAnimationUntilEnd(player) != 0) {
                setRaceMotionAnimation(player, 0x21);
            }
            player->state.stateFlags |= 0x80000;
            if (gViewportStates[(u16)player->playerIndex].overlayAlpha == 0xFF) {
                player->state.updateState++;
                player->state.coursePathIndex = player->state.unk2E8;
                getRaceCourseSurfaceSpawnTransform(
                    player->state.unk80,
                    &player->state.pos.x,
                    &player->state.pos.y,
                    &player->state.pos.z,
                    &player->state.facingAngle
                );
                player->state.pos.y = getRaceCourseSurfaceHeight(player->state.coursePathIndex, player->state.pos.x, player->state.pos.z);
                sine = fixedSine(player->state.facingAngle);
                cosine = fixedCosine(player->state.facingAngle);
                player->state.pos.x += ((s64)sine * -0x240000) / 0x1000;
                player->state.pos.z += ((s64)cosine * -0x240000) / 0x1000;
                player->state.previousPosition = player->state.pos;
                player->state.velocity.x = 0;
                player->state.velocity.y = 0;
                player->state.velocity.z = 0;
                player->state.verticalVelocityLimit = 0;
                player->state.stateFlags &= ~0x400;
                player->state.leanAngle = 0;
                updateRacePlayerLeanAngle(player, 0, 0);
                setRaceMotionAnimation(player, 1);
                stepRaceMotionAnimationUntilEnd(player);
                setRaceCameraMode((u16)player->playerIndex, 1);
                player->state.targetGroundOffset = 0x40000;
            }
            break;
        case 5:
            player->state.stateFlags &= 0xFFF7FFFF;
            if (gViewportStates[(u16)player->playerIndex].overlayAlpha == 0) {
                player->state.mode = 0;
                player->state.updateState = 0;
                player->state.updateTimer = 0;
                stepRaceMotionAnimationUntilEnd(player);
                player->state.stateFlags = 0;
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

    updateState = player->state.updateState;
    if (updateState == 0) {
        player->state.updateState = updateState + 1;
        player->state.stateFlags &= ~0x200;
        player->state.stateFlags &= 0xFE0C1FFB;
        (*player).state.stateFlags |= 0x01006000;
        if (player->state.animationId != 0x12) {
            setRaceMotionAnimation(player, 0x12);
        }
        player->state.modeValue = 0x1E;
        player->state.targetGroundOffset = 0;
    }
    stepRaceMotionLoopingAnimation(player);
    updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
    player->state.velocity.y += 0xFFFF6000;
    player->state.speedLimit = 0x80000;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);
    stateFlags = player->state.stateFlags;
    if ((stateFlags & 1) == 0) {
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0, 0, 0);
        player->state.loopSoundVolume = 0x100;
        player->state.loopSoundId = SFX_CRASH_SLIDE_LOOP;
        player->state.loopSoundPitch = 0.0f;
        stateFlags = player->state.stateFlags;
    }
    if ((stateFlags & 1) != 0) {
        player->state.stateFlags = stateFlags | 0x200;
    } else {
        player->state.stateFlags = stateFlags & ~0x200;
    }
    yVel = player->state.velocity.y;
    velocityX = player->state.velocity.x;
    velocityZ = player->state.velocity.z;
    player->state.pos.y += yVel;
    player->state.verticalVelocityLimit = 0;
    player->state.pos.x += velocityX;
    player->state.pos.y += yVel;
    player->state.pos.z += player->state.velocity.z;
    player->state.facingAngle = calculateFixedAngleFromDeltaXZ(velocityX, velocityZ);
    if (player->state.stateFlags & 0x400) {
        player->state.facingAngle += 0x800;
    }
    if (player->state.surfaceType != 3) {
        if (--player->state.modeValue == 0) {
            player->state.stateFlags &= 0xFE0C1FFB;
            if (player->state.stateFlags & 1) {
                player->state.mode = 1;
                player->state.updateState = 0;
                player->state.updateTimer = 0;
            } else {
                player->state.mode = 0;
                player->state.updateState = 0;
                player->state.updateTimer = 0;
            }
        }
    } else {
        player->state.modeValue = 0x1E;
    }
    player->state.rumbleTimer = 4;
    player->state.rumblePatternId = 2;
}

void updateRacePlayerMode12LaunchRecover(RacePlayer *player) {
    s16 updateState;
    u32 stateFlags;
    s32 grounded;

    updateState = player->state.updateState;
    if (updateState == 0) {
        player->state.updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x14);
        player->state.stateFlags &= 0xFE0C1FFB;
        player->state.stateFlags &= ~0x200;
        player->state.stateFlags |= 0x1022000;
        player->state.targetGroundOffset = 0;
        player->state.unk80 = 0;
        /* Rank-scaled recovery timer for humans and CPUs. With zero-based rank,
         * integer truncation gives 60/53/45/38 updates for a base of 60. */
        player->state.modeValue = 0x1E - ((player->state.rankIndex * 0x1E) / 8);
        player->state.rumbleTimer = 1;
        player->state.rumblePatternId = 0;
    }

    updateRacePlayerLeanAngle(player, 0, 0);
    player->state.velocity.y += 0xFFFF6000;
    grounded = player->state.stateFlags & 1;
    if (grounded == 0) {
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x6000, 0x6000, 0x6000);
        grounded = player->state.stateFlags & 1;
    }

    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += player->state.velocity.y;
    player->state.pos.z += player->state.velocity.z;

    if (grounded == 0) {
        updateRacePlayerMotionFeedback(player);
        if ((player->state.loopSoundId != SFX_BOARD_SURFACE_RUMBLE_LOOP) && (player->state.loopSoundId != SFX_BREAKABLE_SURFACE_LOOP)) {
            player->state.loopSoundPitch = -6.0f;
        }
    }

    stateFlags = player->state.stateFlags;
    if (!(stateFlags & 1)) {
        player->state.modeValue = player->state.modeValue - getRaceInputTimerDecrementBonus(player) - 1;
        if (player->state.modeValue < 0) {
            player->state.modeValue = 0;
        }
        if (stepRaceMotionAnimationUntilEnd(player)) {
            player->state.stateFlags &= ~0x200;
            if (player->state.modeValue == 0) {
                player->state.mode = 8;
                player->state.updateState = 0;
                player->state.updateTimer = 0;
            }
        }
    } else {
        player->state.stateFlags = stateFlags | 0x200;
        setRaceMotionAnimation(player, 0x14);
        stepRaceMotionAnimationUntilEnd(player);
        player->state.modeValue = 0x1E;
        player->state.unk80 = 7;
        if (!(player->state.stateFlags & 1)) {
            player->state.rumbleTimer = 1;
            player->state.rumblePatternId = 0;
        }
    }
}

void updateRacePlayerMode11LaunchHit(RacePlayer *player) {
    s16 updateState;
    s32 tempX;
    s32 tempZ;

    updateState = player->state.updateState;
    if (updateState == 0) {
        player->state.updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x13);
        player->state.stateFlags &= 0xFE0C1FFB;
        player->state.stateFlags |= 0x22200;
        player->state.pitchAngle = 0;
        updateRacePlayerLeanAngle(player, 0, 0);
        player->state.targetGroundOffset = 0;
        player->state.rumbleTimer = 3;
        player->state.rumblePatternId = 0;
    }

    player->state.speedLimit = 0x60000;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);
    player->state.velocity.y -= player->state.aerialGravity;
    if (!(player->state.stateFlags & 1)) {
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x4000, 0x4000, 0x4000);
    } else {
        tempX = player->state.velocity.x;
        tempZ = player->state.velocity.z;
        player->state.velocity.x = tempX - (tempX / 16);
        player->state.velocity.z = tempZ - (tempZ / 16);
    }

    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += player->state.velocity.y;
    player->state.pos.z += player->state.velocity.z;

    if ((stepRaceMotionAnimationUntilEnd(player) != 0) && !(player->state.stateFlags & 1)) {
        player->state.mode = 0xC;
        player->state.updateState = 0;
        player->state.updateTimer = 0;
    }
}

void updateRacePlayerMode26Tumble(RacePlayer *player) {
    s16 updateState;
    s32 timer;

    updateState = player->state.updateState;
    if (updateState == 0) {
        player->state.updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x12);
        player->state.stateFlags &= 0xFE0C1FFB;
        player->state.stateFlags |= 0x6204;
        player->state.pitchAngle = 0;
        updateRacePlayerLeanAngle(player, 0, 0);
        player->state.targetGroundOffset = 0;
        player->state.velocity.y = 0x80000;
        player->state.modeValue = 0x14;
    }

    if (gFrameCounter & 1) {
        createCallbackTaskWithUserIdPreservingArgs(initRacePlayerRecoverySparkle, 5, 2, (u16)player->playerIndex);
    }

    player->state.velocity.y -= 0x7000;
    player->state.pos.y += player->state.velocity.y;
    stepRaceMotionLoopingAnimation(player);

    timer = player->state.modeValue;
    if (timer == 0) {
        if (!(player->state.stateFlags & 1)) {
            player->state.mode = 0xC;
            player->state.updateState = 0;
            player->state.updateTimer = 0;
        }
    } else {
        player->state.modeValue = timer - 1;
    }

    if (player->state.velocity.y > 0) {
        player->state.rumbleTimer = 2;
        player->state.rumblePatternId = 3;
    }
}

void updateRacePlayerMode14PushHit(RacePlayer *player) {
    s16 updateState;
    s32 velocityX;
    s32 velocityZ;
    u32 stateFlags;
    s16 nextState;

    updateState = player->state.updateState;
    nextState = updateState + 1;
    if (updateState == 0) {
        player->state.updateState = nextState;
        player->state.stateFlags &= 0xFE0C1FFB;
        player->state.stateFlags |= 0x01006000;
        setRaceMotionAnimation(player, 0x12);
        player->state.modeValue = 0xF;
        player->state.targetGroundOffset = 0;
    }
    stepRaceMotionLoopingAnimation(player);
    updateRacePlayerLeanAngle(player, 0x10000, 0);
    player->state.velocity.y += 0xFFFF6000;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);
    stateFlags = player->state.stateFlags;
    if ((stateFlags & 1) == 0) {
        player->state.stateFlags = stateFlags & ~0x200;
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0, 0, 0);
        player->state.loopSoundVolume = 0x100;
        player->state.loopSoundId = SFX_CRASH_SLIDE_LOOP;
        player->state.loopSoundPitch = 0.0f;
    } else {
        player->state.stateFlags = stateFlags | 0x200;
    }
    velocityX = player->state.velocity.x;
    player->state.pos.x += velocityX;
    player->state.pos.y += player->state.velocity.y;
    velocityZ = player->state.velocity.z;
    player->state.pos.z += velocityZ;
    player->state.facingAngle = calculateFixedAngleFromDeltaXZ(velocityX, velocityZ);
    if (player->state.stateFlags & 0x400) {
        player->state.facingAngle += 0x800;
    }
    if (--player->state.modeValue == 0) {
        player->state.mode = 0xC;
        player->state.updateState = 0;
        player->state.updateTimer = 0;
    }
    player->state.rumbleTimer = 4;
    player->state.rumblePatternId = 2;
}

void updateRacePlayerMode23ItemSteal(RacePlayer *player) {
    s16 updateState;
    s16 nextState;
    s32 timer;
    u32 stateFlags;

    updateState = player->state.updateState;
    nextState = updateState + 1;
    if (updateState == 0) {
        player->state.updateState = nextState;
        player->state.stateFlags &= 0xFE0C1FFB;
        player->state.stateFlags |= 0x102000;
        setRaceMotionAnimation(player, 0xE);
        player->state.modeValue = 0x5A;
        player->state.targetGroundOffset = 0;
        stepRaceMotionLoopingAnimation(player);
        player->state.velocity.y = 0;
        createCallbackTaskWithUserIdPreservingArgs(initRaceUiItemStealTrailEffect, 0, 0x64, (u16)player->playerIndex);
        timer = player->state.modeValue;
        /* Rank-scaled recovery timer for humans and CPUs. With zero-based rank,
         * integer truncation gives 60/53/45/38 updates for a base of 60. */
        player->state.modeValue = timer - ((player->state.modeValue * player->state.rankIndex) / 8);
        player->state.rumbleTimer = 4;
        player->state.rumblePatternId = 0;
    }

    stateFlags = player->state.stateFlags;
    if (!(stateFlags & 1)) {
        player->state.stateFlags = stateFlags & ~0x200;
    } else {
        player->state.stateFlags = stateFlags | 0x200;
    }

    updateRacePlayerLeanAngle(player, 0, 0);
    player->state.velocity.y -= 0xA000;
    player->state.pos.y += player->state.velocity.y;

    player->state.modeValue -= getRaceInputTimerDecrementBonus(player) + 1;
    timer = player->state.modeValue;
    if (timer < 0) {
        player->state.modeValue = 0;
        timer = 0;
    }

    if (timer == 0) {
        player->state.mode = 4;
        player->state.updateState = 0;
        player->state.updateTimer = 0;
        player->state.unk2E0 = 0;
        player->state.unk2E4 = 0;
    }
}

void updateRacePlayerMode24HeavyKnockdown(RacePlayer *player) {
    s16 temp_v1_2;
    s16 temp_v0;
    s16 temp_2d6;
    s32 timer;
    s32 ret;
    u32 stateFlags;

    player->state.leanAngle = 0;
    updateRacePlayerLeanAngle(player, 0, 0);
    player->state.velocity.y += 0xFFFF6000;
    player->state.pos.y += player->state.velocity.y;

    stateFlags = player->state.stateFlags;
    if (!(stateFlags & 1)) {
        player->state.stateFlags = stateFlags & ~0x200;
    } else {
        player->state.stateFlags = stateFlags | 0x200;
    }

    temp_v1_2 = player->state.updateState;
    temp_v0 = temp_v1_2;
    switch (temp_v0 ^ 0) {
        case 0:
            player->state.updateState = temp_v1_2 + 1;
            player->state.stateFlags &= 0xFE0C1FFB;
            player->state.stateFlags |= 0x1202000;
            player->state.unk2D6 = 1;
            setRaceMotionAnimation(player, 1);
            player->state.modeValue = 0x5A;
            player->state.targetGroundOffset = 0;
            stepRaceMotionLoopingAnimation(player);
            timer = player->state.modeValue;
            /* Rank-scaled recovery timer for humans and CPUs. With zero-based rank,
             * integer truncation gives 60/53/45/38 updates for a base of 60. */
            player->state.modeValue = (timer ^ 0) - ((timer * player->state.rankIndex) / 8);
            player->state.velocity.y = 0;
            player->state.rumbleTimer = 6;
            player->state.rumblePatternId = 0;
            /* fallthrough */
        case 1:
            temp_2d6 = player->state.unk2D6;
            if (temp_2d6 != 1) {
                player->state.unk2D6 = temp_2d6 - 1;
            }

            ret = getRaceInputTimerDecrementBonus(player);
            if (ret != 0) {
                player->state.unk2D6 = 3;
            }

            player->state.modeValue -= ret + 1;
            timer = player->state.modeValue;
            if (timer < 0) {
                player->state.modeValue = 0;
                timer = 0;
            }

            if (timer == 0) {
                player->state.updateState += 1;
                player->state.unk2D6 = 1;
            }
            break;
        case 2:
            player->state.unk2D6 += 1;
            if (player->state.unk2D6 == 7) {
                player->state.unk2D6 = 2;
                player->state.updateState += 1;
            }
            break;
        case 3:
            player->state.unk2D6 += 1;
            if (player->state.unk2D6 == 8) {
                player->state.stateFlags &= 0xFE0C1FFB;
                player->state.mode = 0;
                player->state.updateState = 0;
                player->state.updateTimer = 0;
                player->state.unk2D4 = 0x3C;
            }
            break;
    }
}

void updateRacePlayerMode25SpinHit(RacePlayer *player) {
    s16 updateState;
    s32 stateTimer;
    u32 stateFlags;
    s16 nextState;

    updateState = player->state.updateState;
    nextState = updateState + 1;
    if (updateState == 0) {
        player->state.updateState = nextState;
        player->state.stateFlags &= 0xFE0C1FFB;
        player->state.stateFlags |= 0x400000;
        player->state.modeValue = 0xF0;
        setRaceMotionAnimation(player, 1);
        stepRaceMotionLoopingAnimation(player);
        createCallbackTaskWithUserIdPreservingArgs(
            initRaceUiHeavyKnockdownTrailEffect,
            0,
            0x3C,
            (u16)player->playerIndex
        );
        stateTimer = player->state.modeValue;
        player->state.modeValue = stateTimer;
        /* Rank-scaled recovery timer for humans and CPUs. With zero-based rank,
         * integer truncation gives 60/53/45/38 updates for a base of 60. */
        player->state.modeValue = player->state.modeValue - ((stateTimer * player->state.rankIndex) / 8);
        player->state.rumbleTimer = 4;
        player->state.rumblePatternId = 0;
    }

    stateFlags = player->state.stateFlags;
    if (!(stateFlags & 1)) {
        player->state.stateFlags = stateFlags & ~0x200;
        updateRacePlayerLeanAngle(player, player->state.localVelocityZ, 0);
        player->state.velocity.y -= player->state.gravity;
        updateRacePlayerLocalVelocity(player, 0, 0, player->state.lateralDeceleration, player->state.forwardDeceleration, player->state.reverseDeceleration);
    } else {
        player->state.stateFlags = stateFlags | 0x200;
        player->state.velocity.y -= player->state.aerialGravity;
        clampRacePlayerVectorXZSpeed(&player->state.velocity, player);
    }

    player->state.modeValue -= getRaceInputTimerDecrementBonus(player) + 1;
    if (player->state.modeValue < 0) {
        player->state.modeValue = 0;
    }

    if (player->state.modeValue == 0) {
        stateFlags = player->state.stateFlags;
        if (!(stateFlags & 0x200)) {
            player->state.stateFlags = stateFlags & 0xFE0C1FFB;
            player->state.mode = 0;
            player->state.updateState = 0;
            player->state.updateTimer = 0;
        } else {
            player->state.stateFlags = stateFlags & 0xFE0C1FFB;
            player->state.mode = 1;
            player->state.updateState = 0;
            player->state.updateTimer = 0;
        }
    }

    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += player->state.velocity.y;
    player->state.pos.z += player->state.velocity.z;

    if (!(player->state.stateFlags & 1)) {
        updateRacePlayerMotionFeedback(player);
    }
}

void updateRacePlayerMode27Slide(RacePlayer *player) {
    s16 updateState;
    s16 updateTimer;
    u32 stateFlags;

    updateState = player->state.updateState;
    if (updateState == 0) {
        player->state.updateState = updateState + 1;
        setRaceMotionAnimation(player, 0x13);
        player->state.stateFlags &= 0xFE0C1FFB;
        player->state.stateFlags |= 0x822204;
        player->state.pitchAngle = 0;
        player->state.targetGroundOffset = 0;
        player->state.velocity.y = 0x80000;
        player->state.updateTimer = 0;
        player->state.rumbleTimer = 4;
        player->state.rumblePatternId = 0;
    }

    updateRacePlayerLeanAngle(player, 0, 0);
    if (player->state.velocity.y > 0) {
        player->state.velocity.y -= 0x7000;
    } else {
        updateTimer = player->state.updateTimer;
        if (updateTimer == 0) {
            player->state.updateTimer = updateTimer + 1;
            createCallbackTaskWithUserIdPreservingArgs(
                initRaceUiSpinHitTransitionEffect,
                0,
                3,
                (u16)player->playerIndex
            );
        }
        player->state.facingAngle += 0x40;
        player->state.velocity.y -= 0x800;
    }

    player->state.velocity.x -= player->state.velocity.x >> 6;
    player->state.velocity.z -= player->state.velocity.z >> 6;
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += player->state.velocity.y;
    player->state.pos.z += player->state.velocity.z;

    stepRaceMotionAnimationUntilEnd(player);
    if (player->state.velocity.y < 0) {
        stateFlags = player->state.stateFlags;
        if (!(stateFlags & 1)) {
            player->state.mode = 8;
            player->state.updateState = 0;
            player->state.updateTimer = 0;
            player->state.stateFlags = stateFlags & ~0x200;
        }
    }
}

void updateRacePlayerMode40Stun(RacePlayer *player) {
    s16 updateState;
    s32 velocityX;
    s32 velocityZ;
    u32 stateFlags;
    s16 nextState;

    updateState = player->state.updateState;
    nextState = updateState + 1;
    if (updateState == 0) {
        player->state.updateState = nextState;
        player->state.stateFlags &= 0xFE0C1FFB;
        player->state.stateFlags |= 0x01006000;
        setRaceMotionAnimation(player, 0x12);
        player->state.modeValue = 0x1E;
        player->state.modeValue += randomNextMain() >> 4;
        player->state.targetGroundOffset = 0;
    }
    stepRaceMotionLoopingAnimation(player);
    updateRacePlayerLeanAngle(player, 0x10000, 0);
    player->state.velocity.y += 0xFFFF6000;
    clampRacePlayerVectorXZSpeed(&player->state.velocity, player);
    stateFlags = player->state.stateFlags;
    if ((stateFlags & 1) == 0) {
        player->state.stateFlags = stateFlags & ~0x200;
        updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0, 0, 0);
        player->state.stateFlags &= ~0x200;
        player->state.loopSoundVolume = 0x100;
        player->state.loopSoundId = SFX_CRASH_SLIDE_LOOP;
        player->state.loopSoundPitch = 0.0f;
    } else {
        player->state.stateFlags = stateFlags | 0x200;
    }
    velocityX = player->state.velocity.x;
    player->state.pos.x += velocityX;
    player->state.pos.y += player->state.velocity.y;
    velocityZ = player->state.velocity.z;
    player->state.pos.z += velocityZ;
    player->state.facingAngle = calculateFixedAngleFromDeltaXZ(velocityX, velocityZ);
    if (player->state.stateFlags & 0x400) {
        player->state.facingAngle += 0x800;
    }
    if (--player->state.modeValue == 0) {
        player->state.mode = 0xC;
        player->state.updateState = 0;
        player->state.updateTimer = 0;
    }
    player->state.rumbleTimer = 4;
    player->state.rumblePatternId = 2;
}

void dispatchRacePlayerMode07CourseObject(RacePlayer *player) {
    gRacePlayerMode07StateHandlers[player->state.updateState](player);
}

void updateRacePlayerMode07AlignToLaunchRamp(RacePlayer *player) {
    s16 angleDelta;
    s16 updateTimer;
    s32 temp_a1;
    s32 tempX;
    s32 tempZ;

    updateTimer = player->state.updateTimer;
    if (updateTimer == 0) {
        player->state.updateTimer = updateTimer + 1;
        player->state.modeValue = 0x14;
        player->state.unk80 = 0xA;
        player->state.modeAngle = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampCourseObjectAngle;
        if (player->state.stateFlags & 0x400) {
            player->state.modeAngle += 0x800;
        }
        gMenuFlowState |= 4;
        setRaceMotionAnimation(player, 1);
    }

    stepRaceMotionLoopingAnimation(player);
    angleDelta = (player->state.modeAngle - player->state.facingAngle) & 0xFFF;
    if (angleDelta >= 0x801) {
        angleDelta -= 0x1000;
    }
    updateRacePlayerLeanAngle(player, 0x10000, 0);

    temp_a1 = player->state.unk80;
    if (1) {}
    if (1) {}
    if (1) {}
    if (temp_a1 != 0) {
        angleDelta /= temp_a1;
        if (gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampTargetZ) {}
        player->state.facingAngle += angleDelta;
        tempX = player->state.pos.x;
        tempZ = player->state.pos.z;
        if (temp_a1) {}
        if (1) {
            player->state.pos.x =
                tempX -
                (0 - ((gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampTargetX - tempX) / temp_a1));
        }
        player->state.pos.z =
            tempZ - (0 - ((gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampTargetZ - tempZ) / temp_a1));
    }

    if (--player->state.modeValue == 0) {
        player->state.pos.x = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampTargetX;
        player->state.pos.z = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampTargetZ;
        player->state.updateState++;
        player->state.updateTimer = 0;
        player->state.modeValue = 0;
        setRaceMotionAnimation(player, 5);
        setRaceCameraMode(player->playerIndex, 3);
    }
}

void updateRacePlayerMode07StartLaunchRamp(RacePlayer *player) {
    if (stepRaceMotionAnimationUntilEnd(player) != 0) {
        player->state.updateState++;
        player->state.updateTimer = 0;
        player->state.facingAngle = player->state.modeAngle;
        player->state.pos.x = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampTargetX;
        player->state.pos.z = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampTargetZ;
    }
}

void updateRacePlayerMode07LaunchRampTakeoff(RacePlayer *player) {
    s16 updateTimer;
    s32 unused;
    s32 scratch[14];

    updateTimer = player->state.updateTimer;
    if (updateTimer == 0) {
        player->state.updateTimer = updateTimer + 1;
        player->state.velocity.y = 0x30000;
        player->state.modeValue = 0x1E;
        setRaceMotionAnimation(player, 4);
        if (player->state.stateFlags & 0x400) {
            player->state.unk80 = 0x20000;
        } else {
            player->state.unk80 = -0x20000;
        }
    }

    makeFixedRotationY((s16 *)&scratch[2], player->state.facingAngle);
    scratch[13] = 0;
    scratch[14] = 0;
    scratch[15] = player->state.unk80;
    transformVec3iByFixedMatrix((s16 *)&scratch[2], (Vec3i *)&scratch[13], (Vec3i *)&scratch[10]);
    player->state.pos.x += scratch[10];
    player->state.pos.z += scratch[12];

    makeFixedRotationY((s16 *)&scratch[2], player->state.facingAngle);
    scratch[13] = 0;
    scratch[14] = 0;
    scratch[15] = player->state.unk80;
    transformVec3iByFixedMatrix((s16 *)&scratch[2], (Vec3i *)&scratch[13], (Vec3i *)&scratch[10]);
    player->state.pos.x += scratch[10];
    player->state.pos.z += scratch[12];
    player->state.velocity.y -= 0x7000;
    player->state.pos.y += player->state.velocity.y;

    if ((player->state.velocity.y < 0) && !(player->state.stateFlags & 1)) {
        if (player->state.animationId != 1) {
            setRaceMotionAnimation(player, 1);
        }
        stepRaceMotionLoopingAnimation(player);
        updateRacePlayerMotionFeedback(player);
    } else {
        stepRaceMotionAnimationUntilEnd(player);
    }

    if (--player->state.modeValue == 0) {
        player->state.updateState++;
        player->state.updateTimer = 0;
        player->state.modeValue = 0x20;
        createCallbackTask((CallbackTaskCallback)initLaunchRampCourseObject, 0, 0x64);
        player->state.stateFlags &= ~0x20;
    }
}

void updateRacePlayerMode07LaunchRampSpin(RacePlayer *player) {
    s32 scratch[15];

    player->state.facingAngle += 0x10;
    if (player->state.unk80 > 0) {
        player->state.unk80 -= 0x820;
    } else {
        player->state.unk80 += 0x820;
    }

    makeFixedRotationY((s16 *)&scratch[1], player->state.facingAngle);
    scratch[12] = 0;
    scratch[13] = 0;
    scratch[14] = player->state.unk80;
    transformVec3iByFixedMatrix((s16 *)&scratch[1], (Vec3i *)&scratch[12], (Vec3i *)&scratch[9]);
    player->state.pos.x += scratch[9];
    player->state.pos.z += scratch[11];

    player->state.facingAngle += 0x10;
    if (player->state.unk80 > 0) {
        player->state.unk80 -= 0x820;
    } else {
        player->state.unk80 += 0x820;
    }

    makeFixedRotationY((s16 *)&scratch[1], player->state.facingAngle);
    scratch[12] = 0;
    scratch[13] = 0;
    scratch[14] = player->state.unk80;
    transformVec3iByFixedMatrix((s16 *)&scratch[1], (Vec3i *)&scratch[12], (Vec3i *)&scratch[9]);
    player->state.pos.x += scratch[9];
    player->state.pos.z += scratch[11];

    stepRaceMotionAnimationUntilEnd(player);
    updateRacePlayerMotionFeedback(player);
    if (--player->state.modeValue == 0) {
        player->state.updateTimer = 0;
        player->state.updateState++;
    }
}

void updateRacePlayerMode07LaunchRampPose(RacePlayer *player) {
    if (player->state.updateTimer == 0) {
        player->state.updateTimer++;
        setRaceMotionAnimation(player, 0x1D);
        player->state.modeValue = 0x13;
    }
    stepRaceMotionAnimationUntilEnd(player);
    player->state.modeValue--;
    if (player->state.modeValue == 0) {
        player->state.updateTimer = 0;
        player->state.updateState++;
    }
}

void updateRacePlayerMode07LaunchRampDrift(RacePlayer *player) {
    s16 updateTimer;
    s32 scratch[14];

    updateTimer = player->state.updateTimer;
    if (updateTimer == 0) {
        player->state.updateTimer = updateTimer + 1;
        player->state.modeValue = 0x13;
        if (player->state.stateFlags & 0x400) {
            player->state.unk80 = 0x40000;
        } else {
            player->state.unk80 = -0x40000;
        }
    }

    makeFixedRotationY((s16 *)&scratch[1], player->state.facingAngle);
    scratch[12] = 0;
    scratch[13] = 0;
    scratch[14] = player->state.unk80;
    transformVec3iByFixedMatrix((s16 *)&scratch[1], (Vec3i *)&scratch[12], (Vec3i *)&scratch[9]);
    player->state.pos.x += scratch[9];
    player->state.pos.z += scratch[11];

    player->state.modeValue--;
    if (player->state.modeValue == 0) {
        player->state.updateTimer = 0;
        player->state.updateState++;
    }
    stepRaceMotionAnimationUntilEnd(player);
}



void updateRacePlayerMode07LaunchRampClimb(RacePlayer *player) {
    PlayerTransformScratch80095A88 scratch;

    if (player->state.updateTimer == 0) {
        player->state.updateTimer++;
        player->state.modeValue = 0x64;
    }

    if (player->state.unk80 > 0) {
        if (player->state.unk80 < 0xA0000) {
            player->state.unk80 += 0x2000;
        }
        makeFixedRotationXY((s16 *)scratch.matrix, -0x100, player->state.facingAngle);
    } else {
        if (player->state.unk80 >= -0x9FFFF) {
            player->state.unk80 -= 0x2000;
        }
        makeFixedRotationXY((s16 *)scratch.matrix, 0x100, (0, player)->state.facingAngle);
    }

    scratch.sourceX = 0;
    scratch.sourceY = 0;
    scratch.sourceZ = player->state.unk80;
    transformVec3iByFixedMatrix((s16 *)scratch.matrix, (Vec3i *)&scratch.sourceX, (Vec3i *)&scratch.transformedX);
    player->state.pos.x = player->state.pos.x + scratch.transformedX;
    player->state.pos.y += scratch.transformedY;
    player->state.pos.z += scratch.transformedZ;

    player->state.modeValue--;
    if (player->state.modeValue == 0) {
        player->state.updateTimer = 0;
        player->state.updateState++;
    }

    if (player->state.modeValue < 0x3C) {
        player->state.stateFlags |= 0x80000;
        if (gRaceCameras[(u16)player->playerIndex].initialized.signedValue != 0) {
            gRaceCameras[(u16)player->playerIndex].initialized.signedValue = 2;
        }
    }
}

void updateRacePlayerMode07SpiralExit(RacePlayer *player) {
    PlayerTransformScratch80095A88 scratch;

    if (player->state.updateTimer == 0) {
        player->state.updateTimer++;
        scratch.sourceX = -0x200000;
        scratch.sourceY = 0;
        scratch.sourceZ = 0x400000;
        makeFixedRotationY(
            (s16 *)scratch.matrix,
            gRaceCourseStartEntries[gRaceCourseIndex.signedValue].liftExitObjectAngle
        );
        transformVec3iByFixedMatrix((s16 *)scratch.matrix, (Vec3i *)&scratch.sourceX, (Vec3i *)&player->state.pos);
        player->state.pos.x += gRaceCourseStartEntries[gRaceCourseIndex.signedValue].liftExitOrigin.x;
        player->state.pos.y += gRaceCourseStartEntries[gRaceCourseIndex.signedValue].liftExitOrigin.y + 0x80000;
        player->state.pos.z += gRaceCourseStartEntries[gRaceCourseIndex.signedValue].liftExitOrigin.z;
        player->state.coursePathIndex = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].liftExitSurfaceIndex;
        player->state.lapDigit++;
        player->state.previousPosition = player->state.pos;
        player->state.modeValue = 0x28;
        player->state.stateFlags &= 0xFBFFFBFF;
        player->state.facingAngle = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].liftExitObjectAngle;
        player->state.coursePathOffset =
            -projectRaceCourseSurfaceProgress(player->state.coursePathIndex, player->state.pos.x, player->state.pos.z);
        setRaceCameraMode(player->playerIndex, 1);
        gRaceCameras[player->playerIndex].prevPos.x = player->state.pos.x;
        gRaceCameras[player->playerIndex].prevPos.y = player->state.pos.y;
        gRaceCameras[player->playerIndex].prevPos.z = player->state.pos.z;
        createCallbackTask((CallbackTaskCallback)initLiftExitCourseObject, 0, 0x64);
        if (player->state.lapDigit >= (gRaceLapCount - 1)) {
            createCallbackTaskWithUserIdPreservingArgs(waitForRaceSetupNamePlate, 0, 0x64, player->playerIndex);
        }
    }

    player->state.stateFlags &= ~0x80000;
    scratch.sourceX = 0;
    scratch.sourceY = 0;
    scratch.sourceZ = -0x40000;
    makeFixedRotationY(
        (s16 *)scratch.matrix,
        gRaceCourseStartEntries[gRaceCourseIndex.signedValue].liftExitObjectAngle
    );
    transformVec3iByFixedMatrix((s16 *)scratch.matrix, (Vec3i *)&scratch.sourceX, (Vec3i *)&scratch.transformedX);
    player->state.pos.x += scratch.transformedX;
    player->state.pos.z += scratch.transformedZ;
    player->state.modeValue--;

    if (player->state.modeValue == 0) {
        player->state.mode = 1;
        player->state.updateState = 0;
        player->state.updateTimer = 0;
        player->state.stateFlags = 0;

        /* Rank-dependent course-object exit displacement: add 0/1/2/4 times
         * current X/Z velocity for first through fourth, for humans and CPUs. */
        if (player->state.rankIndex == 1) {
            player->state.pos.x += player->state.velocity.x;
            player->state.pos.z += player->state.velocity.z;
        }

        if (player->state.rankIndex == 2) {
            player->state.pos.x += player->state.velocity.x * 2;
            player->state.pos.z += player->state.velocity.z * 2;
        }

        if (player->state.rankIndex == 3) {
            player->state.pos.x += player->state.velocity.x * 4;
            player->state.pos.z += player->state.velocity.z * 4;
        }

        if (gRaceCameras[player->playerIndex].initialized.signedValue != 0) {
            gRaceCameras[player->playerIndex].initialized.signedValue = 1;
        }
    }
}

/* Success sets racer flags, copies gRacePlayerAttackStartTimer into unk57C,
 * and decrements that shared timer if nonzero. This is a state transition. */
s32 tryStartRacePlayerCourseObjectMode(s16 arg0) {
    s32 delta;
    s32 i;

    if (gRacePlayers[arg0].state.stateFlags & 0x40) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        if ((gRacePlayers[i].isActive != 0) && (arg0 != (u16)gRacePlayers[i].playerIndex) &&
            (gRacePlayers[i].state.stateFlags & 0x20)) {
            return 0;
        }
    }

    delta = gRacePlayers[arg0].state.pos.x - gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampTargetX;
    if (delta < 0) {
        delta = -delta;
    }
    if (delta > 0x180000) {
        return 0;
    }

    delta = gRacePlayers[arg0].state.pos.z - gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampTargetZ;
    if (delta < 0) {
        delta = -delta;
    }
    if (delta > 0x180000) {
        return 0;
    }

    gRacePlayers[arg0].state.stateFlags |= 0x04001020;
    gRacePlayers[arg0].state.unk57C = gRacePlayerAttackStartTimer;
    if (gRacePlayerAttackStartTimer != 0) {
        gRacePlayerAttackStartTimer--;
    }
    return 1;
}

void dispatchRacePlayerMode30Attack(RacePlayer *player) {
    gRacePlayerMode30StateHandlers[player->state.updateState](player);
    if (player->state.collisionRadius < 0xF0000) {
        player->state.collisionRadius += 0x8000;
    }
}

void updateRacePlayerMode30AttackApproach(RacePlayer *player) {
    s16 updateTimer;
    s16 facingAngle;
    s16 angleDiff;
    s16 roll;

    updateTimer = player->state.updateTimer;
    if (updateTimer == 0) {
        player->state.updateTimer = updateTimer + 1;
        player->state.subState = 0;
        setRaceCameraMode(player->playerIndex, 5);
        if (player->state.movementSpeed >= 0x20001) {
            player->state.unk306 = 1;
            player->state.unk31E = calculateFixedAngleFromDeltaXZ(player->state.velocity.x, player->state.velocity.z);
            if (player->state.stateFlags & 0x400) {
                player->state.unk31E += 0x800;
            }

            if ((s16)(angleDiff = 0x800 & (player->state.unk31E - player->state.facingAngle)) < 0x800) {
                player->state.unk31C = 0;
                if (player->state.stateFlags & 0x400) {
                    player->state.unk31C = 1;
                }
                player->state.unk31E -= 0x400;
            } else {
                player->state.unk31C = 1;
                if (player->state.stateFlags & 0x400) {
                    player->state.unk31C = 0;
                }
                player->state.unk31E += 0x400;
            }
            player->state.modeValue = 0;
        } else {
            player->state.unk306 = 0;
            setRaceMotionAnimation(player, 1);
        }
    }

    if (player->state.unk306 != 0) {
        facingAngle = player->state.facingAngle;
        angleDiff = (player->state.unk31E - facingAngle) & 0xFFF;
        if (angleDiff >= 0x801) {
            angleDiff -= 0x1000;
        }
        if (angleDiff >= 0x41) {
            angleDiff = 0x40;
        }
        if (angleDiff < -0x40) {
            angleDiff = -0x40;
        }
        player->state.facingAngle = facingAngle + angleDiff;

        if (player->state.unk31C != 0) {
            player->state.modeValue = 0x1F;
        } else {
            player->state.modeValue = -0x1F;
        }

        player->state.leanScale = 0x3F;
        angleDiff = updateRacePlayerLeanAngle(player, player->state.movementSpeed, player->state.modeValue) - player->state.modeAngle;
        roll = player->state.modeAngle;
        if (angleDiff >= 0x31) {
            angleDiff = 0x30;
        }
        if (angleDiff < -0x30) {
            angleDiff = -0x30;
        }
        player->state.modeAngle = player->state.modeAngle + angleDiff;

        if (player->state.modeAngle == 0) {
            if (player->state.animationId != 1) {
                setRaceMotionAnimation(player, 1);
            }
            stepRaceMotionLoopingJointAnimation(player);
        } else if (player->state.modeAngle >= 0) {
            setRaceMotionAnimation(player, 3);
            blendRaceMotionJointAnimation(player, 3, player->state.modeAngle, 0x118);
        } else {
            setRaceMotionAnimation(player, 2);
            blendRaceMotionJointAnimation(player, 2, -player->state.modeAngle, 0x118);
        }
    } else {
        updateRacePlayerLeanAngle(player, 0, 0);
        stepRaceMotionLoopingJointAnimation(player);
    }

    player->state.velocity.y -= player->state.gravity;
    updateRacePlayerLocalVelocityNoVerticalOffset(player, 0, 0x6000, 0x6000, 0x6000);
    player->state.pos.x += player->state.velocity.x;
    player->state.pos.y += player->state.velocity.y;
    player->state.pos.z += player->state.velocity.z;
    updateRacePlayerMotionFeedback(player);

    if ((player->state.velocity.x == 0) && (player->state.velocity.z == 0) && !(player->state.stateFlags & 1)) {
        if (player->soundDisabled == 0) {
            player->state.updateState++;
            player->state.modeValue = 0x1E;
            if (player->state.animationId != 1) {
                setRaceMotionAnimation(player, 1);
            }
        }
    }
}

void updateRacePlayerMode30AttackResolve(RacePlayer *player) {
    if (gRaceResultState != 0) {
        player->state.modeValue = 0;
    }
    if (player->state.modeValue == 0) {
        if (gRaceSplitscreenMode == 0) {
            if (player->state.rankIndex == 0) {
                player->state.updateState = 2;
                player->state.updateTimer = 0;
            } else {
                player->state.updateState = 3;
                player->state.updateTimer = 0;
            }
        } else if (gRaceResultState == 1) {
            player->state.updateState = 2;
            player->state.updateTimer = 0;
        } else {
            player->state.updateState = 3;
            player->state.updateTimer = 0;
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
    if (player->state.updateTimer == 0) {
        player->state.updateTimer++;
        player->state.modeValue = 0;
        setRaceMotionAnimation(player, 0x23);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->state.animationId == 0x24) {
            player->state.stateFlags |= 0x8000000;
        }
        if (player->state.modeValue == 0) {
            if (player->state.updateTimer == 1) {
                enqueueRacePlayerVoiceSound(player, RACE_VOICE_CHEER);
                player->state.updateTimer++;
            }
            player->state.modeValue = 0x14;
            setRaceMotionAnimation(player, 0x24);
        } else {
            player->state.modeValue--;
        }
    }
}

void updateRacePlayerMode32Character1(RacePlayer *player) {
    if (player->state.updateTimer == 0) {
        player->state.updateTimer++;
        player->state.modeValue = 0;
        setRaceMotionAnimation(player, 0x24);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->state.animationId == 0x25) {
            player->state.stateFlags |= 0x8000000;
        }
        if (player->state.modeValue == 0) {
            if (player->state.updateTimer == 1) {
                enqueueRacePlayerVoiceSound(player, RACE_VOICE_CHEER);
                player->state.updateTimer++;
            }
            setRaceMotionAnimation(player, 0x25);
        } else {
            player->state.modeValue--;
        }
    }
}

void updateRacePlayerMode32Character2(RacePlayer *player) {
    if (player->state.updateTimer == 0) {
        player->state.updateTimer++;
        player->state.modeValue = 0;
        setRaceMotionAnimation(player, 0x23);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->state.animationId == 0x24) {
            player->state.stateFlags |= 0x8000000;
        }
        if (player->state.modeValue == 0) {
            if (player->state.updateTimer == 1) {
                enqueueRacePlayerVoiceSound(player, RACE_VOICE_CHEER);
                player->state.updateTimer++;
            }
            player->state.modeValue = 0x14;
            setRaceMotionAnimation(player, 0x24);
        } else {
            player->state.modeValue--;
        }
    }
}

void updateRacePlayerMode32Character3(RacePlayer *player) {
    if (player->state.updateTimer == 0) {
        player->state.updateTimer++;
        player->state.modeValue = 0;
        setRaceMotionAnimation(player, 0x24);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->state.animationId == 0x25) {
            player->state.stateFlags |= 0x8000000;
        }
        if (player->state.modeValue == 0) {
            if (player->state.updateTimer == 1) {
                enqueueRacePlayerVoiceSound(player, RACE_VOICE_CHEER);
                player->state.updateTimer++;
            }
            player->state.modeValue = 0;
            setRaceMotionAnimation(player, 0x25);
        } else {
            player->state.modeValue--;
        }
    }
}

void updateRacePlayerMode32Character4(RacePlayer *player) {
    if (player->state.updateTimer == 0) {
        player->state.updateTimer++;
        player->state.modeValue = 0;
        setRaceMotionAnimation(player, 0x24);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->state.animationId == 0x25) {
            player->state.stateFlags |= 0x8000000;
        }
        if (player->state.modeValue == 0) {
            if (player->state.updateTimer == 1) {
                enqueueRacePlayerVoiceSound(player, RACE_VOICE_CHEER);
                player->state.updateTimer++;
            }
            player->state.modeValue = 0x32;
            setRaceMotionAnimation(player, 0x25);
        } else {
            player->state.modeValue--;
        }
    }
}

void updateRacePlayerMode32Character5(RacePlayer *player) {
    if (player->state.updateTimer == 0) {
        player->state.updateTimer++;
        enqueueRacePlayerVoiceSound(player, RACE_VOICE_CHEER);
        player->state.modeValue = 0x1E;
        setRaceMotionAnimation(player, 0x23);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        player->state.stateFlags |= 0x8000000;
        if (player->state.modeValue == 0) {
            player->state.modeValue = 0x1E;
            setRaceMotionAnimation(player, 0x23);
        } else {
            player->state.modeValue--;
        }
    }
}

void dispatchRacePlayerMode35Character(RacePlayer *player) {
    updateRacePlayerLeanAngle(player, 0, 0);
    gRacePlayerMode35CharacterHandlers[player->characterId](player);
}

void updateRacePlayerMode35Character0(RacePlayer *player) {
    if (player->state.updateTimer == 0) {
        player->state.updateTimer++;
        player->state.modeValue = 0;
        setRaceMotionAnimation(player, 0x25);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->state.modeValue == 0) {
            player->state.stateFlags |= 0x8000000;
            player->state.modeValue = 0x1E;
            setRaceMotionAnimation(player, 0x26);
        } else {
            player->state.modeValue--;
        }
    }
}

void updateRacePlayerMode35Character1(RacePlayer *player) {
    if (player->state.updateTimer == 0) {
        player->state.updateTimer++;
        player->state.modeValue = 0;
        setRaceMotionAnimation(player, 0x26);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->state.modeValue == 0) {
            player->state.stateFlags |= 0x8000000;
            player->state.modeValue = 0x1E;
            setRaceMotionAnimation(player, 0x27);
        } else {
            player->state.modeValue--;
        }
    }
}

void updateRacePlayerMode35Character2(RacePlayer *player) {
    if (player->state.updateTimer == 0) {
        player->state.updateTimer++;
        player->state.modeValue = 0;
        setRaceMotionAnimation(player, 0x25);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->state.modeValue == 0) {
            player->state.modeValue = 0;
            player->state.stateFlags |= 0x8000000;
            setRaceMotionAnimation(player, 0x26);
        } else {
            player->state.modeValue--;
        }
    }
}

void updateRacePlayerMode35Character3(RacePlayer *player) {
    if (player->state.updateTimer == 0) {
        player->state.updateTimer++;
        player->state.modeValue = 0;
        setRaceMotionAnimation(player, 0x26);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->state.modeValue == 0) {
            player->state.stateFlags |= 0x8000000;
            player->state.modeValue = 0;
            setRaceMotionAnimation(player, 0x27);
        } else {
            player->state.modeValue--;
        }
    }
}

void updateRacePlayerMode35Character4(RacePlayer *player) {
    if (player->state.updateTimer == 0) {
        player->state.updateTimer++;
        player->state.modeValue = 0;
        setRaceMotionAnimation(player, 0x26);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->state.modeValue == 0) {
            player->state.stateFlags |= 0x8000000;
            player->state.modeValue = 0;
            setRaceMotionAnimation(player, 0x27);
        } else {
            player->state.modeValue--;
        }
    }
}

void updateRacePlayerMode35Character5(RacePlayer *player) {
    if (player->state.updateTimer == 0) {
        player->state.updateTimer++;
        player->state.modeValue = 0;
        setRaceMotionAnimation(player, 0x24);
    }
    if (stepRaceMotionAnimationUntilEnd(player)) {
        if (player->state.modeValue == 0) {
            if (player->state.animationId == 0x25) {
                player->state.modeValue = 0;
                setRaceMotionAnimation(player, 0x26);
            } else {
                player->state.stateFlags |= 0x8000000;
                player->state.modeValue = 0x1E;
                setRaceMotionAnimation(player, 0x25);
            }
        } else {
            player->state.modeValue--;
        }
    }
}

void updateRacePlayersPostUpdate(void) {
    RacePlayer *player;
    Vec3i *soundPos;
    Vec3i *nextSoundPos;
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
            updateSpeedFanEffect(&gRacePlayers[i]);
            i++;
        } while (i < gRacePlayerCount);
        i = 0;
    }

    i = 0;
    if (gRacePlayerCount > 0) {
        do {
            player = &gRacePlayers[i];
            soundPos = &gRacePlayers[i].state.pos;
            nextSoundPos = &gRacePlayers[i].state.renderPosition;
            if (player->soundDisabled == 0) {
                enqueuePlayerLoopingPositionalSoundRequest(
                    player->state.loopSoundId,
                    soundPos,
                    player->state.loopSoundVolume,
                    SFX_PRIORITY_PLAYER_LOOP,
                    player->state.loopSoundPitch,
                    i
                );
            } else {
                enqueuePlayerLoopingPositionalSoundRequest(
                    player->state.loopSoundId,
                    soundPos,
                    0,
                    SFX_PRIORITY_PLAYER_LOOP,
                    player->state.loopSoundPitch,
                    i
                );
            }

            *nextSoundPos = *soundPos;
            player->state.renderPosition.y = player->state.groundCorrectionY + player->state.renderPosition.y - player->state.groundOffset + 0xA000;
            if (player->soundDisabled == 0) {
                addRenderCallback(&gModelRenderCallbackQueue4Head, (RenderCallback)drawRacePlayerModel, player);
                addRenderCallback(&gModelRenderCallbackQueue7Head, (RenderCallback)drawRacePlayerGroundShadow, player);
            } else {
                addRenderCallback(&gModelRenderCallbackQueue7Head, (RenderCallback)drawRaceGhostPlayerModel, player);
            }
            i++;
        } while (i < gRacePlayerCount);
    }
}

void updateRacePlayerPostUpdate(RacePlayer *player) {
    unsigned long long random;

    gRacePlayerModePostUpdateHandlers[player->state.mode](player);
    if (!(gMenuFlowState & 1) && !(player->state.stateFlags & 0x41000) && (player->soundDisabled == 0)) {
        if (player->state.groundContactMask & 3) {
            random = randomNextMain();
            spawnRaceItemTrackSparkBurst(
                &player->state.boardContactPoints[0],
                &player->state.boardContactPoints[1],
                &player->state.boardContactPoints[4],
                &player->state.boardContactPoints[5],
                random,
                player->state.surfaceType
            );
        }
        if (player->state.groundContactMask & 0xC) {
            random = randomNextMain();
            spawnRaceItemTrackSparkBurst(
                &player->state.boardContactPoints[2],
                &player->state.boardContactPoints[3],
                &player->state.boardContactPoints[6],
                &player->state.boardContactPoints[7],
                random,
                player->state.surfaceType
            );
        }
    }
    updateRacePlayerFinalLapStatus(player);
    resolveRacePlayerCollisionVolumes(player);
    updateRacePlayerRumble(player);
}

void updateRacePlayerPostUpdateNoop(RacePlayer *player) {
    updateRacePlayerSurfaceContact(player);
}

void updateRacePlayerPostUpdateCourseObject(RacePlayer *player) {
    if (player->state.updateState < 7) {
        updateRacePlayerGroundAlignment(player);
    } else {
        player->state.groundCorrectionY = 0;
        updateRacePlayerProjectedPosition(player);
        player->state.groundOffset = 0x30000;
    }
}

void updateRacePlayerPostUpdateMode00(RacePlayer *player) {
    if (updateRacePlayerSurfaceContact(player)) {
        player->state.mode = 0x16;
        player->state.updateState = 0;
        player->state.updateTimer = 0;
        player->state.stateFlags |= 0x200;
    } else if (tryStartRacePlayerCourseObjectMode(player->playerIndex)) {
        player->state.mode = 7;
        player->state.updateState = 0;
        player->state.updateTimer = 0;
    }
    interpolateRaceMotionJointAnimationFrame(player, 0, (0x60000 - player->state.groundOffset) / 0x600, 0x100);
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

    voiceType = player->state.aerialTrickId;
    player2 = player;
    soundId = 0;
    score = 0;
    if (voiceType >= 9) {
        player->state.unk57B = 1;
        voiceType = player2->state.aerialTrickId;
    }
    if ((voiceType != 0) && (player->state.grabTrickId != 0)) {
        player->state.unk57B = 1;
        voiceType = player2->state.aerialTrickId;
    }
    if (voiceType != 0) {
        soundId = gRacePlayerVoiceBaseSoundIds[voiceType];
        score = voiceType < 9 ? 0xA : 0x28;
        bitIndex = voiceType;
        if ((s32)(u32)bitIndex >= 0xA) {
            bitIndex = 9;
        }
        player->state.trickAttackCompletedTrickMask |= 1 << bitIndex;
    }
    splitType = player2->state.grabTrickId;
    if (splitType != 0) {
        if (player->state.aerialTrickId != 0) {
            soundId += gRacePlayerVoiceSplitSoundOffsets[splitType];
            if (splitType & 1) {
                score += 5;
            } else {
                score += 0xA;
            }
        } else {
            score += 1;
            soundId += gRacePlayerVoiceLeadSoundOffsets[splitType];
        }
        soundId += player2->state.grabHoldFrames;
    }
    if ((soundId != 0) && (gRaceSplitscreenMode == 0) && (gRaceDemoPlaybackEnabled == 0)) {
        addRacePlayerScore(player, soundId);
        playerIndex = player2->playerIndex;
        if (gRacePlayerHudStatuses[playerIndex].active != 0) {
            spawnRaceUiScorePopup((void *)soundId, (s16)playerIndex);
            soundType = SFX_TRICK_SCORE_LOW;
            if (soundId >= 0x64) {
                soundType = SFX_TRICK_SCORE_MEDIUM;
            }
            if (soundId >= 0x78) {
                soundType = SFX_TRICK_SCORE_HIGH;
            }
            if (soundId >= 0xC8) {
                soundType = SFX_TRICK_BONUS;
            }
            if (soundId >= 0x12C) {
                soundType = SFX_TRICK_SCORE_TOP;
            }
            enqueueSoundEffect((s16)soundType, SFX_PRIORITY_HIGH);
        }
    }
    if (gRaceSplitscreenMode == 1) {
        if (gRaceTypeSelection == 2) {
            if (score != 0) {
                soundType = SFX_TRICK_SCORE_LOW;
                if (score >= 0x2710) {
                    score = 0x270F;
                }
                player->state.trickAttackPointTotal += score;
                player->state.trickAttackMakeCount++;
                if (player->state.trickAttackMakeCount >= 0x64) {
                    player->state.trickAttackMakeCount = 0x63;
                }
                if (player->state.trickAttackBestMakeCount < player->state.trickAttackMakeCount) {
                    player->state.trickAttackBestMakeCount = player2->state.trickAttackMakeCount;
                }
                if (score >= 0xA) {
                    soundType = SFX_TRICK_SCORE_MEDIUM;
                }
                if (score >= 0xF) {
                    soundType = SFX_TRICK_SCORE_HIGH;
                }
                if (score >= 0x14) {
                    soundType = SFX_TRICK_BONUS;
                }
                if (score >= 0x28) {
                    soundType = SFX_TRICK_SCORE_TOP;
                }
                if (player2->state.trickAttackCompletedTrickMask == 0x3FE) {
                    player2->state.trickAttackPointTotal += 0x12C;
                    player2->state.trickAttackCompletedTrickMask = 0x3FF;
                    soundType = SFX_TRICK_BONUS;
                    spawnRaceUiTrickScorePopup((void *)score, 1);
                } else {
                    spawnRaceUiTrickScorePopup((void *)score, 0);
                }
                enqueueSoundEffect((s16)soundType, SFX_PRIORITY_HIGH);
                if (player2->state.trickAttackPointTotal >= 0x2710) {
                    player2->state.trickAttackPointTotal = 0x270F;
                }
            }
        }
    }
}

void updateRacePlayerLoopingSound(RacePlayer *player) {
    s32 v0 = -player->state.velocity.y >> 13;
    if (v0 >= 0x80) {
        v0 = 0x7F;
    }
    if (v0 < 0x46) {
        v0 = 0x46;
    }
    if (v0 >= 0x51) {
        player->state.rumbleTimer = 2;
        player->state.rumblePatternId = 0;
    } else {
        player->state.rumbleTimer = 1;
        player->state.rumblePatternId = 0;
    }
    if (player->soundDisabled == 0) {
        enqueuePositionalSoundEffect(SFX_PLAYER_DESCENT, (Vec3i *)&player->state.pos.x, (s16)v0, SFX_PRIORITY_NORMAL);
    }
}

void updateRacePlayerPostUpdateMode22(RacePlayer *player) {
    u32 stateFlags;

    if (updateRacePlayerSurfaceContact(player) == 0) {
        stateFlags = player->state.stateFlags;
        if (stateFlags & 8) {
            player->state.stateFlags = stateFlags & ~8;
        } else if (stateFlags & 0x800) {
            player->state.trickAttackMakeCount = 0;
            player->state.mode = 0xE;
            player->state.updateState = 0;
            player->state.updateTimer = 0;
            player->state.stateFlags = stateFlags & ~0x200;
            enqueueRacePlayerVoiceSound(player, RACE_VOICE_HIT);
        } else {
            player->state.stateFlags = stateFlags & ~0x200;
            if (tryStartRacePlayerCourseObjectMode(player->playerIndex) != 0) {
                player->state.mode = 7;
                player->state.updateState = 0;
                player->state.updateTimer = 0;
                updateRacePlayerVoiceSounds(player);
                updateRacePlayerLoopingSound(player);
            } else if (player->state.subState == 0) {
                player->state.mode = 0;
                player->state.updateState = 1;
                player->state.updateTimer = 0;
                player->state.unk57A = 2;
                updateRacePlayerLoopingSound(player);
            } else {
                player->state.mode = 0;
                player->state.updateState = 0;
                player->state.updateTimer = 0;
                player->state.unk57A = 2;
                updateRacePlayerVoiceSounds(player);
                updateRacePlayerLoopingSound(player);
            }
        }
    }
}

void updateRacePlayerPostUpdateAirborneTrick(RacePlayer *player) {
    u32 stateFlags;

    if (updateRacePlayerSurfaceContact(player) == 0) {
        stateFlags = player->state.stateFlags;
        if (stateFlags & 8) {
            player->state.stateFlags = stateFlags & ~8;
        } else if (stateFlags & 0x800) {
            player->state.trickAttackMakeCount = 0;
            player->state.mode = 0xE;
            player->state.updateState = 0;
            player->state.updateTimer = 0;
            player->state.stateFlags = stateFlags & ~0x200;
            enqueueRacePlayerVoiceSound(player, RACE_VOICE_HIT);
        } else {
            player->state.stateFlags = stateFlags & ~0x200;
            if (tryStartRacePlayerCourseObjectMode(player->playerIndex) != 0) {
                player->state.mode = 7;
                player->state.updateState = 0;
                player->state.updateTimer = 0;
                updateRacePlayerVoiceSounds(player);
                updateRacePlayerLoopingSound(player);
            } else {
                player->state.mode = 0;
                player->state.updateState = 0;
                player->state.updateTimer = 0;
                player->state.unk57A = 2;
                updateRacePlayerVoiceSounds(player);
                updateRacePlayerLoopingSound(player);
            }
        }
    }
}

void updateRacePlayerPostUpdateAirborneLaunch(RacePlayer *player) {
    if (updateRacePlayerSurfaceContact(player)) {
        player->state.mode = 0x16;
        player->state.updateState = 0;
        player->state.updateTimer = 0;
        player->state.stateFlags |= 0x200;
    } else if (tryStartRacePlayerCourseObjectMode(player->playerIndex)) {
        player->state.mode = 7;
        player->state.updateState = 0;
        player->state.updateTimer = 0;
    }
    interpolateRaceMotionJointAnimationFrame(player, 0, (0x60000 - player->state.groundOffset) / 0x600, 0x100);
}

void updateRacePlayerPostUpdateNudge(RacePlayer *player) {
    if (updateRacePlayerSurfaceContact(player)) {
        player->state.stateFlags |= 0x200;
    } else {
        player->state.stateFlags &= ~0x200;
    }
}

void updateRacePlayerPostUpdateMode29(RacePlayer *player) {
    u16 playerIndex;

    updateRacePlayerSurfaceContact(player);
    if (!(player->state.stateFlags & 0x02000000)) {
        player->state.mode = 1;
        player->state.updateState = 0;
        player->state.updateTimer = 0;
        if ((gRaceSplitscreenMode == 1) && (gRaceTypeSelection == 2)) {
            player->state.trickAttackPointTotal *= 2;
            if (player->state.trickAttackPointTotal >= 0x2710) {
                player->state.trickAttackPointTotal = 0x270F;
            }
            createCallbackTaskPreservingArgs((CallbackTaskCallback)initRaceUiCrashScorePopup, 0, 0x64);
            enqueueSoundEffect(SFX_TRICK_BONUS, SFX_PRIORITY_NORMAL);
        }
        if (gRaceSplitscreenMode == 0) {
            addRacePlayerScore(player, 0x12C);
            playerIndex = player->playerIndex;
            if (gRacePlayerHudStatuses[playerIndex].active != 0) {
                spawnRaceUiScorePopup((void *)0x12C, (s16)playerIndex);
                enqueueSoundEffect(SFX_TRICK_BONUS, SFX_PRIORITY_NORMAL);
            }
        }
    }
}

void updateRacePlayerPostUpdateAttack(RacePlayer *player) {
    updateRacePlayerSurfaceContact(player);
    if (player->state.updateState < 2) {
        interpolateRaceMotionJointAnimationFrame(player, 0, (0x60000 - player->state.groundOffset) / 0x600, 0x100);
    }
}
