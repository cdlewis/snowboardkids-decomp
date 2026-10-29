#include "game/audio/sound_wave_ids.h"
#include "libmus/sequence_commands.h"
#include "common.h"
#include "game/race/camera/race_camera.h"
#include "game/race/race_state.h"
#include "assets.h"
#include "game/engine/asset_manager.h"
#include "game/math/fixed_point_math.h"
#include "game/engine/relocatable_heap.h"
#include "game/audio/sound_manager.h"
#include "game/engine/system_runtime.h"
#include "game/engine/runtime_memory.h"
#include "libmus/libmus.h"
#include "game/race/player/race_player_input.h"
#include "game/audio/audio_engine_internal.h"

#define SOUND_MANAGER_QUEUE_MASK (SOUND_MANAGER_QUEUE_CAPACITY - 1)
#define SOUND_MANAGER_QUEUE_CAPACITY 64

typedef union SoundHalfArg {
    s32 word;
    struct {
        s16 unused;
        s16 value;
    } half;
} SoundHalfArg;

void *allocRenderCallbackScratch(s32 size);

u8 gSoundBankWaveTuning[SOUND_BANK_WAVE_COUNT + 1] = {
    0xDF, 0xD5, 0xE5, 0xF9, 0xD5, 0xE4, 0xE2, 0xD5, 0xE1, 0xEA, 0xED, 0xE1, 0xD5, 0xD5, 0xE1, 0xD5, 0xE0, 0xC9,
    0xE1, 0xED, 0xD8, 0xC9, 0xCC, 0xCA, 0xF9, 0xD1, 0xD5, 0xE1, 0xC9, 0xED, 0xCF, 0xED, 0xE1, 0xED, 0xE3, 0xD5,
    0xE1, 0xF9, 0xED, 0xE1, 0xE1, 0xD5, 0xE1, 0xD5, 0xF9, 0xED, 0xF9, 0xED, 0xE1, 0xD5, 0xC9, 0xF9, 0xED, 0xE1,
    0xD5, 0xED, 0xE1, 0xD5, 0xC9, 0xF9, 0xED, 0xE4, 0xE3, 0xF9, 0xED, 0xF3, 0xE1, 0xFC, 0xFB, 0xF4, 0xF7, 0xF9,
    0xFA, 0xF9, 0xE1, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9,
    0xC9, 0xD4, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xD4, 0xD5, 0xD5, 0xD4,
    0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9,
    0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9,
    0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9,
    0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9, 0xC9,
    0xC9, 0xC9, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xE1,
    0xD5, 0xC9, 0xBD, 0xE1, 0xD5, 0xC9, 0xBD, 0xE1, 0xD5, 0xC9, 0xBD, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5,
    0xD5, 0xE1, 0xD5, 0xC9, 0xBD, 0xD5, 0xC9, 0xE1, 0xD5, 0xD5, 0xC9, 0xE1, 0xD5, 0xF9, 0xED, 0xE1, 0xD5, 0xC9,
    0xE1, 0xD5, 0xC9, 0xBD, 0xD5, 0xC9, 0xE1, 0xD5, 0xC9, 0xBD, 0xE1, 0xD5, 0xC9, 0xBD, 0xF9, 0xED, 0xD5, 0x00,
};

f32 gSoundBankWavePitchOffsets[SOUND_BANK_WAVE_COUNT] = { 0.0f };

u8 gSoundEffectNoneCommands[] = {
    MUS_STOP(),
};

u8 gSoundEffectMenuAcceptCommands[] = {
    MUS_WAVE(SOUND_WAVE_MENU_ACCEPT),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 34),
    MUS_STOP(),
};

u8 gSoundEffectResultsTallyCompleteCommands[] = {
    MUS_WAVE(SOUND_WAVE_RESULTS_TALLY_COMPLETE),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(105),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectBoardGlideLoopCommands[] = {
    MUS_WAVE(SOUND_WAVE_BOARD_GLIDE_LOOP),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(100),
    MUS_NOTE_LONG(48, MUS_DURATION_INFINITE),
    MUS_STOP(),
};

u8 gSoundEffectBoardContactFallbackLoopCommands[] = {
    MUS_WAVE(SOUND_WAVE_BOARD_CONTACT_FALLBACK_LOOP),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(100),
    MUS_NOTE_LONG(48, MUS_DURATION_INFINITE),
    MUS_STOP(),
};

u8 gSoundEffectBoardSurfaceSprayLoopCommands[] = {
    MUS_WAVE(SOUND_WAVE_BOARD_SURFACE_SPRAY_LOOP),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(100),
    MUS_NOTE_LONG(48, MUS_DURATION_INFINITE),
    MUS_STOP(),
};

u8 gSoundEffectBoardSurfaceRumbleLoopCommands[] = {
    MUS_WAVE(SOUND_WAVE_BOARD_SURFACE_RUMBLE_LOOP),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE_LONG(48, MUS_DURATION_INFINITE),
    MUS_STOP(),
};

u8 gSoundEffectSpeedFanLoopCommands[] = {
    MUS_WAVE(SOUND_WAVE_SPEED_FAN_LOOP),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE_LONG(48, MUS_DURATION_INFINITE),
    MUS_STOP(),
};

u8 gSoundEffectCrashSlideLoopCommands[] = {
    MUS_WAVE(SOUND_WAVE_CRASH_SLIDE_LOOP),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE_LONG(48, MUS_DURATION_INFINITE),
    MUS_STOP(),
};

u8 gSoundEffectShockCommands[] = {
    MUS_WAVE(SOUND_WAVE_SHOCK),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 65),
    MUS_STOP(),
};

u8 gSoundEffectProjectileImpactCommands[] = {
    MUS_WAVE(SOUND_WAVE_PROJECTILE_IMPACT),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 18),
    MUS_STOP(),
};

u8 gSoundEffectSpinHitCommands[] = {
    MUS_WAVE(SOUND_WAVE_SPIN_HIT),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 48),
    MUS_STOP(),
};

u8 gSoundEffectGhostHitCommands[] = {
    MUS_WAVE(SOUND_WAVE_GHOST_HIT),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 48),
    MUS_STOP(),
};

u8 gSoundEffectGhostSlowdownCommands[] = {
    MUS_WAVE(SOUND_WAVE_GHOST_SLOWDOWN),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 127),
    MUS_STOP(),
};

u8 gSoundEffectSunsetRockAmbienceCommands[] = {
    MUS_WAVE(SOUND_WAVE_SUNSET_ROCK_AMBIENCE),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE_LONG(48, MUS_DURATION_INFINITE),
    MUS_STOP(),
};

u8 gSoundEffectItemStealStartCommands[] = {
    MUS_WAVE(SOUND_WAVE_ITEM_STEAL_START),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 48),
    MUS_STOP(),
};

u8 gSoundEffectItemUseCommands[] = {
    MUS_WAVE(SOUND_WAVE_ITEM_USE),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectItemImpactCommands[] = {
    MUS_WAVE(SOUND_WAVE_ITEM_IMPACT),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 18),
    MUS_STOP(),
};

u8 gSoundEffectItemBreakCommands[] = {
    MUS_WAVE(SOUND_WAVE_ITEM_BREAK),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE_LONG(48, 158),
    MUS_STOP(),
};

u8 gSoundEffectAreaBlastCommands[] = {
    MUS_WAVE(SOUND_WAVE_AREA_BLAST),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE_LONG(48, 173),
    MUS_STOP(),
};

u8 gSoundEffectCollisionImpactCommands[] = {
    MUS_WAVE(SOUND_WAVE_COLLISION_IMPACT),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 34),
    MUS_STOP(),
};

u8 gSoundEffectItemStealEndCommands[] = {
    MUS_WAVE(SOUND_WAVE_ITEM_STEAL_END),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 48),
    MUS_STOP(),
};

u8 gSoundEffectCourseGateOpen1Commands[] = {
    MUS_WAVE(SOUND_WAVE_COURSE_GATE_OPEN_1),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 111),
    MUS_STOP(),
};

u8 gSoundEffectTrickStartCommands[] = {
    MUS_WAVE(SOUND_WAVE_TRICK_START),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE_LONG(48, 142),
    MUS_STOP(),
};

u8 gSoundEffectConfirmCommands[] = {
    MUS_WAVE(SOUND_WAVE_CONFIRM),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 34),
    MUS_STOP(),
};

u8 gSoundEffectMenuCursorCommands[] = {
    MUS_WAVE(SOUND_WAVE_MENU_CURSOR),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 34),
    MUS_STOP(),
};

u8 gSoundEffectCoinTickCommands[] = {
    MUS_WAVE(SOUND_WAVE_COIN_TICK),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 49),
    MUS_STOP(),
};

u8 gSoundEffectCourseGateOpen2Commands[] = {
    MUS_WAVE(SOUND_WAVE_COURSE_GATE_OPEN_2),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 65),
    MUS_STOP(),
};

u8 gSoundEffectCourseGateCloseCommands[] = {
    MUS_WAVE(SOUND_WAVE_COURSE_GATE_CLOSE),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 48),
    MUS_STOP(),
};

u8 gSoundEffectUnusedSample91Commands[] = {
    MUS_WAVE(SOUND_WAVE_UNUSED_SAMPLE_91),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(36, 18),
    MUS_STOP(),
};

u8 gSoundEffectBreakableSurfaceLoopCommands[] = {
    MUS_WAVE(SOUND_WAVE_BREAKABLE_SURFACE_LOOP),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(110),
    MUS_NOTE_LONG(36, MUS_DURATION_INFINITE),
    MUS_STOP(),
};

u8 gSoundEffectItemPickupCommands[] = {
    MUS_WAVE(SOUND_WAVE_ITEM_PICKUP),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(36, 49),
    MUS_STOP(),
};

u8 gSoundEffectFallingRockBounceCommands[] = {
    MUS_WAVE(SOUND_WAVE_FALLING_ROCK_BOUNCE),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(36, 111),
    MUS_STOP(),
};

u8 gSoundEffectPlayerDescentCommands[] = {
    MUS_WAVE(SOUND_WAVE_PLAYER_DESCENT),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(36, 28),
    MUS_STOP(),
};

u8 gSoundEffectSlashTrickCommands[] = {
    MUS_WAVE(113),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 32),
    MUS_STOP(),
};

u8 gSoundEffectSlashComplexTrickCommands[] = {
    MUS_WAVE(115),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 90),
    MUS_STOP(),
};

u8 gSoundEffectSlashHitCommands[] = {
    MUS_WAVE(116),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 24),
    MUS_STOP(),
};

u8 gSoundEffectSlashAttackSuccessCommands[] = {
    MUS_WAVE(118),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 64),
    MUS_STOP(),
};

u8 gSoundEffectSlashCheerCommands[] = {
    MUS_WAVE(SOUND_WAVE_SLASH_CHEER),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectSlashObstacleHitCommands[] = {
    MUS_WAVE(120),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectNancyTrickCommands[] = {
    MUS_WAVE(121),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 24),
    MUS_STOP(),
};

u8 gSoundEffectNancyComplexTrickCommands[] = {
    MUS_WAVE(123),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 48),
    MUS_STOP(),
};

u8 gSoundEffectNancyHitCommands[] = {
    MUS_WAVE(SOUND_WAVE_NANCY_HIT),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 48),
    MUS_STOP(),
};

u8 gSoundEffectNancyCheerCommands[] = {
    MUS_WAVE(127),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 64),
    MUS_STOP(),
};

u8 gSoundEffectNancyAttackSuccessCommands[] = {
    MUS_WAVE_LONG(SOUND_WAVE_NANCY_ATTACK_SUCCESS),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 64),
    MUS_STOP(),
};

u8 gSoundEffectNancyObstacleHitCommands[] = {
    MUS_WAVE_LONG(129),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectLindaTrickCommands[] = {
    MUS_WAVE_LONG(130),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 24),
    MUS_STOP(),
};

u8 gSoundEffectLindaComplexTrickCommands[] = {
    MUS_WAVE_LONG(131),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 48),
    MUS_STOP(),
};

u8 gSoundEffectLindaHitCommands[] = {
    MUS_WAVE_LONG(SOUND_WAVE_LINDA_HIT),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 48),
    MUS_STOP(),
};

u8 gSoundEffectLindaCheerCommands[] = {
    MUS_WAVE_LONG(135),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 72),
    MUS_STOP(),
};

u8 gSoundEffectLindaAttackSuccessCommands[] = {
    MUS_WAVE_LONG(SOUND_WAVE_LINDA_ATTACK_SUCCESS),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectLindaObstacleHitCommands[] = {
    MUS_WAVE_LONG(137),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 72),
    MUS_STOP(),
};

u8 gSoundEffectLindaUnusedVoiceCommands[] = {
    MUS_WAVE_LONG(138),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 24),
    MUS_STOP(),
};

u8 gSoundEffectJamTrickCommands[] = {
    MUS_WAVE_LONG(139),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 48),
    MUS_STOP(),
};

u8 gSoundEffectJamComplexTrickCommands[] = {
    MUS_WAVE_LONG(140),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 32),
    MUS_STOP(),
};

u8 gSoundEffectJamHitCommands[] = {
    MUS_WAVE_LONG(142),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 48),
    MUS_STOP(),
};

u8 gSoundEffectJamAttackSuccessCommands[] = {
    MUS_WAVE_LONG(143),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 64),
    MUS_STOP(),
};

u8 gSoundEffectJamCheerCommands[] = {
    MUS_WAVE_LONG(144),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 64),
    MUS_STOP(),
};

u8 gSoundEffectJamObstacleHitCommands[] = {
    MUS_WAVE_LONG(145),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectTommyTrickCommands[] = {
    MUS_WAVE_LONG(146),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 48),
    MUS_STOP(),
};

u8 gSoundEffectTommyComplexTrickCommands[] = {
    MUS_WAVE_LONG(147),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectTommyHitCommands[] = {
    MUS_WAVE_LONG(148),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectTommyCheerCommands[] = {
    MUS_WAVE_LONG(149),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 64),
    MUS_STOP(),
};

u8 gSoundEffectTommyObstacleHitCommands[] = {
    MUS_WAVE_LONG(150),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectShinobinTrickCommands[] = {
    MUS_WAVE_LONG(151),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 24),
    MUS_STOP(),
};

u8 gSoundEffectShinobinHitCommands[] = {
    MUS_WAVE_LONG(152),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 24),
    MUS_STOP(),
};

u8 gSoundEffectShinobinCheerCommands[] = {
    MUS_WAVE_LONG(153),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 127),
    MUS_STOP(),
};

u8 gSoundEffectShinobinObstacleHitCommands[] = {
    MUS_WAVE_LONG(154),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 72),
    MUS_STOP(),
};

u8 gSoundEffectShopGreetingCommands[] = {
    MUS_WAVE_LONG(155),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 72),
    MUS_STOP(),
};

u8 gSoundEffectCoursePurchasePromptCommands[] = {
    MUS_WAVE_LONG(156),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectMenuBackCommands[] = {
    MUS_WAVE_LONG(157),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectWarningCommands[] = {
    MUS_WAVE_LONG(158),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE_LONG(48, 160),
    MUS_STOP(),
};

u8 gSoundEffectUnusedSample159Commands[] = {
    MUS_WAVE_LONG(159),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE_LONG(48, 180),
    MUS_STOP(),
};

u8 gSoundEffectCoursePurchaseAvailableCommands[] = {
    MUS_WAVE_LONG(160),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 80),
    MUS_STOP(),
};

u8 gSoundEffectMainMenuEnterCommands[] = {
    MUS_START_EFFECT(107),
    MUS_WAVE_LONG(181),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(0),
    MUS_VOLUME(120),
    MUS_NOTE_LONG(48, 255),
    MUS_STOP(),
};

u8 gSoundEffectMainMenuEnterAltCommands[] = {
    MUS_WAVE_LONG(181),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(255),
    MUS_VOLUME(120),
    MUS_NOTE_LONG(47, 255),
    MUS_STOP(),
};

u8 gSoundEffectRaceReadyCommands[] = {
    MUS_WAVE_LONG(SOUND_WAVE_RACE_READY),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 127),
    MUS_STOP(),
};

u8 gSoundEffectRaceGoCommands[] = {
    MUS_WAVE_LONG(162),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 64),
    MUS_STOP(),
};

u8 gSoundEffectTrickScoreLowCommands[] = {
    MUS_WAVE_LONG(164),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(95),
    MUS_NOTE(48, 64),
    MUS_STOP(),
};

u8 gSoundEffectTrickScoreMediumCommands[] = {
    MUS_WAVE_LONG(165),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 64),
    MUS_STOP(),
};

u8 gSoundEffectTrickScoreHighCommands[] = {
    MUS_WAVE_LONG(166),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectTrickScoreTopCommands[] = {
    MUS_WAVE_LONG(167),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(115),
    MUS_NOTE(48, 72),
    MUS_STOP(),
};

u8 gSoundEffectTrickBonusCommands[] = {
    MUS_WAVE_LONG(168),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 64),
    MUS_STOP(),
};

u8 gSoundEffectFinalLapCommands[] = {
    MUS_WAVE_LONG(169),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(115),
    MUS_NOTE(48, 127),
    MUS_STOP(),
};

u8 gSoundEffectSparkleRetargetCommands[] = {
    MUS_WAVE_LONG(170),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(120),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectCourseColumnRightSelectCommands[] = {
    MUS_WAVE_LONG(SOUND_WAVE_COURSE_COLUMN_RIGHT_SELECT),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectCourseColumnMiddleSelectCommands[] = {
    MUS_WAVE_LONG(172),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectCourseColumnLeftSelectCommands[] = {
    MUS_WAVE_LONG(173),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(110),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectSlashSelectCommands[] = {
    MUS_WAVE_LONG(SOUND_WAVE_SLASH_SELECT),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectNancySelectCommands[] = {
    MUS_WAVE_LONG(SOUND_WAVE_NANCY_SELECT),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectLindaSelectCommands[] = {
    MUS_WAVE_LONG(176),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(85),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectJamSelectCommands[] = {
    MUS_WAVE_LONG(177),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(110),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectTommySelectCommands[] = {
    MUS_WAVE_LONG(178),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(110),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectTommySelectAltCommands[] = {
    MUS_WAVE_LONG(179),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(115),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectShinobinSelectCommands[] = {
    MUS_WAVE_LONG(180),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(115),
    MUS_NOTE(48, 96),
    MUS_STOP(),
};

u8 gSoundEffectSlashTrickAltCommands[] = {
    MUS_WAVE(114),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(115),
    MUS_NOTE(48, 64),
    MUS_STOP(),
};

u8 gSoundEffectSlashHitAltCommands[] = {
    MUS_WAVE(117),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(115),
    MUS_NOTE(48, 64),
    MUS_STOP(),
};

u8 gSoundEffectNancyComplexTrickAltCommands[] = {
    MUS_WAVE(122),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 24),
    MUS_STOP(),
};

u8 gSoundEffectNancyUnusedVoiceCommands[] = {
    MUS_WAVE(124),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(110),
    MUS_NOTE(48, 24),
    MUS_STOP(),
};

u8 gSoundEffectNancyHitAltCommands[] = {
    MUS_WAVE(125),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(115),
    MUS_NOTE(48, 32),
    MUS_STOP(),
};

u8 gSoundEffectLindaComplexTrickAltCommands[] = {
    MUS_WAVE_LONG(132),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(110),
    MUS_NOTE(48, 127),
    MUS_STOP(),
};

u8 gSoundEffectLindaHitAltCommands[] = {
    MUS_WAVE_LONG(133),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 48),
    MUS_STOP(),
};

u8 gSoundEffectJamHitAltCommands[] = {
    MUS_WAVE_LONG(141),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 24),
    MUS_STOP(),
};

u8 gSoundEffectRatFaceAppearCommands[] = {
    MUS_WAVE(SOUND_WAVE_RAT_FACE_APPEAR),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 48),
    MUS_STOP(),
};

u8 gSoundEffectTeacupBumpCommands[] = {
    MUS_WAVE(SOUND_WAVE_TEACUP_BUMP),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE_LONG(48, 128),
    MUS_STOP(),
};

u8 gSoundEffectRatFaceLandCommands[] = {
    MUS_WAVE(110),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE_LONG(48, 224),
    MUS_STOP(),
};

u8 gSoundEffectRatFaceStealCommands[] = {
    MUS_WAVE(SOUND_WAVE_RAT_FACE_STEAL),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 48),
    MUS_STOP(),
};

u8 gSoundEffectProjectileLaunchCommands[] = {
    MUS_WAVE(SOUND_WAVE_PROJECTILE_LAUNCH),
    MUS_ENVELOPE_DEFAULT(1, 1, 1, 127, 255, 127, 1),
    MUS_PAN(127),
    MUS_VOLUME(127),
    MUS_NOTE(48, 48),
    MUS_STOP(),
};

u8 *gSoundEffectCommandPointers[SOUND_EFFECT_COUNT] = {
    gSoundEffectNoneCommands,
    gSoundEffectMenuAcceptCommands,
    gSoundEffectResultsTallyCompleteCommands,
    gSoundEffectBoardGlideLoopCommands,
    gSoundEffectBoardContactFallbackLoopCommands,
    gSoundEffectBoardSurfaceSprayLoopCommands,
    gSoundEffectBoardSurfaceRumbleLoopCommands,
    gSoundEffectSpeedFanLoopCommands,
    gSoundEffectCrashSlideLoopCommands,
    gSoundEffectShockCommands,
    gSoundEffectProjectileImpactCommands,
    gSoundEffectSpinHitCommands,
    gSoundEffectGhostHitCommands,
    gSoundEffectGhostSlowdownCommands,
    gSoundEffectSunsetRockAmbienceCommands,
    gSoundEffectItemStealStartCommands,
    gSoundEffectItemUseCommands,
    gSoundEffectItemImpactCommands,
    gSoundEffectItemBreakCommands,
    gSoundEffectAreaBlastCommands,
    gSoundEffectCollisionImpactCommands,
    gSoundEffectItemStealEndCommands,
    gSoundEffectCourseGateOpen1Commands,
    gSoundEffectTrickStartCommands,
    gSoundEffectConfirmCommands,
    gSoundEffectMenuCursorCommands,
    gSoundEffectCoinTickCommands,
    gSoundEffectCourseGateOpen2Commands,
    gSoundEffectCourseGateCloseCommands,
    gSoundEffectUnusedSample91Commands,
    gSoundEffectBreakableSurfaceLoopCommands,
    gSoundEffectItemPickupCommands,
    gSoundEffectFallingRockBounceCommands,
    gSoundEffectPlayerDescentCommands,
    gSoundEffectSlashTrickCommands,
    gSoundEffectSlashComplexTrickCommands,
    gSoundEffectSlashHitCommands,
    gSoundEffectSlashAttackSuccessCommands,
    gSoundEffectSlashCheerCommands,
    gSoundEffectSlashObstacleHitCommands,
    gSoundEffectNancyTrickCommands,
    gSoundEffectNancyComplexTrickCommands,
    gSoundEffectNancyHitCommands,
    gSoundEffectNancyCheerCommands,
    gSoundEffectNancyAttackSuccessCommands,
    gSoundEffectNancyObstacleHitCommands,
    gSoundEffectLindaTrickCommands,
    gSoundEffectLindaComplexTrickCommands,
    gSoundEffectLindaHitCommands,
    gSoundEffectLindaCheerCommands,
    gSoundEffectLindaAttackSuccessCommands,
    gSoundEffectLindaObstacleHitCommands,
    gSoundEffectLindaUnusedVoiceCommands,
    gSoundEffectJamTrickCommands,
    gSoundEffectJamComplexTrickCommands,
    gSoundEffectJamHitCommands,
    gSoundEffectJamAttackSuccessCommands,
    gSoundEffectJamCheerCommands,
    gSoundEffectJamObstacleHitCommands,
    gSoundEffectTommyTrickCommands,
    gSoundEffectTommyComplexTrickCommands,
    gSoundEffectTommyHitCommands,
    gSoundEffectTommyCheerCommands,
    gSoundEffectTommyObstacleHitCommands,
    gSoundEffectShinobinTrickCommands,
    gSoundEffectShinobinHitCommands,
    gSoundEffectShinobinCheerCommands,
    gSoundEffectShinobinObstacleHitCommands,
    gSoundEffectShopGreetingCommands,
    gSoundEffectCoursePurchasePromptCommands,
    gSoundEffectMenuBackCommands,
    gSoundEffectWarningCommands,
    gSoundEffectUnusedSample159Commands,
    gSoundEffectCoursePurchaseAvailableCommands,
    gSoundEffectMainMenuEnterCommands,
    gSoundEffectRaceReadyCommands,
    gSoundEffectRaceGoCommands,
    gSoundEffectTrickScoreLowCommands,
    gSoundEffectTrickScoreMediumCommands,
    gSoundEffectTrickScoreHighCommands,
    gSoundEffectTrickScoreTopCommands,
    gSoundEffectTrickBonusCommands,
    gSoundEffectFinalLapCommands,
    gSoundEffectSparkleRetargetCommands,
    gSoundEffectCourseColumnRightSelectCommands,
    gSoundEffectCourseColumnMiddleSelectCommands,
    gSoundEffectCourseColumnLeftSelectCommands,
    gSoundEffectSlashSelectCommands,
    gSoundEffectNancySelectCommands,
    gSoundEffectLindaSelectCommands,
    gSoundEffectJamSelectCommands,
    gSoundEffectTommySelectCommands,
    gSoundEffectTommySelectAltCommands,
    gSoundEffectShinobinSelectCommands,
    gSoundEffectSlashTrickAltCommands,
    gSoundEffectSlashHitAltCommands,
    gSoundEffectNancyComplexTrickAltCommands,
    gSoundEffectNancyUnusedVoiceCommands,
    gSoundEffectNancyHitAltCommands,
    gSoundEffectLindaComplexTrickAltCommands,
    gSoundEffectLindaHitAltCommands,
    gSoundEffectJamHitAltCommands,
    gSoundEffectRatFaceAppearCommands,
    gSoundEffectTeacupBumpCommands,
    gSoundEffectRatFaceLandCommands,
    gSoundEffectRatFaceStealCommands,
    gSoundEffectProjectileLaunchCommands,
    gSoundEffectMainMenuEnterAltCommands,
};

s32 gSoundEffectDefaultPriorities[SOUND_EFFECT_COUNT] = {
    0x00000000, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064,
    0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064,
    0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064,
    0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064,
    0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064,
    0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064,
    0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064,
    0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064,
    0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064,
    0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064,
    0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064,
    0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064, 0x00000064,
};

SoundRomRange gMusicSequenceRomRanges[10] = {
    { 0x00536380, 0x00539DE0 },
    { 0x00539DE0, 0x0053AF70 },
    { 0x0053AF70, 0x0053B510 },
    { 0x0053B510, 0x0053CDD0 },
    { 0x0053CDD0, 0x0053DC80 },
    { 0x0053DC80, 0x0053E0D0 },
    { 0x0053E0D0, 0x0053E520 },
    { 0x0053E520, 0x0053F610 },
    { 0x0053AF70, 0x0053B510 },
    { 0x0053AF70, 0x0053B510 },
};

SoundRomRange gRaceMusicSequenceRomRanges[9] = {
    { 0x0053F610, 0x00546C70 },
    { 0x00546C70, 0x00550650 },
    { 0x00550650, 0x00558550 },
    { 0x00558550, 0x00561620 },
    { 0x00561620, 0x00569DE0 },
    { 0x00569DE0, 0x00572980 },
    { 0x00572980, 0x00579BA0 },
    { 0x00579BA0, 0x00583CE0 },
    { 0x00583CE0, 0x00588340 },
};

u16 gCourseMusicSequenceBanks[14] = {
    0x000B, 0x000C, 0x000E, 0x000D, 0x0012, 0x000F, 0x0011, 0x0010, 0x0010, 0x0010, 0x0000, 0x0000, 0x0000, 0x0000,
};

u32 gRaceReplayCourseGridEntries[0x1E0] = {
    0x00000001, 0x00604A86, 0xFF196E38, 0xFA7F8A1E, 0x00000018, 0xF85B96A2, 0xFBAEE83B, 0xED360636, 0x00000021,
    0xEFB42C00, 0xF4B96C7C, 0xE0B58BE2, 0x0000002B, 0xE8D9F648, 0xF1D3B5D4, 0xDFF9C790, 0x00000030, 0xE21120DC,
    0xED4868D2, 0xE3BA5630, 0x00000037, 0xD78F9F12, 0xE779AEB1, 0xEB881F7B, 0x00000047, 0xCDBC86C0, 0xE3267ABB,
    0xF5D949B0, 0x00000051, 0xCEB47AA6, 0xE03166D9, 0xEE1E11D2, 0x00000060, 0xD1D65D53, 0xDACE1D1C, 0xE53935E9,
    0x0000006E, 0xC1D4D12E, 0xD21DC0CB, 0xE474B056, 0x000000A7, 0xB8A8C252, 0xB16D7682, 0xC818AD7E, 0xFFFE0000,
    0x00000000, 0x00000000, 0x00000000, 0x00000003, 0x000BDC22, 0xFD8B7F8D, 0xF8D839F1, 0x0000000C, 0x01F516F7,
    0xF90DEBE9, 0xECD601A6, 0x00000024, 0xF9F75B40, 0xE945D5D1, 0xDB9619CE, 0x00000033, 0x0C3EE2D0, 0xE160263E,
    0xDAE3B066, 0x0000003A, 0x13F73966, 0xDCBFFEC8, 0xDAE4E5B4, 0x00000041, 0x156B95E9, 0xD8A84447, 0xD2BD1CD9,
    0x00000041, 0x14D70F5F, 0xD86FC447, 0xD2CF0EB5, 0x00000062, 0x2D98988F, 0xCF6744CF, 0xCF1DD474, 0x0000006B,
    0x33A02765, 0xC992DA1E, 0xC5F3AF9F, 0x00000079, 0x250C8F1F, 0xC38CC2EA, 0xBE27B6B5, 0x0000008B, 0x201D9B56,
    0xBD66C28D, 0xB1BA6D93, 0xFFFE0000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x007A58D3, 0xFEB7CD45,
    0xF871C05A, 0x00000012, 0xF88E8E8E, 0xFA4BFA1E, 0xF3C7755A, 0x00000099, 0xFA4B6016, 0xF984AF02, 0xF07EFB5A,
    0x0000002B, 0xFB29A924, 0xF17EBAEB, 0xDE38E59B, 0x0000003D, 0xECFDF00B, 0xEE973557, 0xDA5BC7EC, 0x000000A0,
    0xF80DB490, 0xEEE09ADD, 0xD8B52C56, 0x0000004E, 0xF75D5F95, 0xE8D7983D, 0xD0972A70, 0x000000A8, 0xFE46F1DF,
    0xEB8110C3, 0xD8721643, 0x00000069, 0xFDF0D9CE, 0xE0F11BCD, 0xC4668598, 0x00000069, 0xFDF793D8, 0xE15CBBCD,
    0xC46AF046, 0x00000069, 0xFDC35CAA, 0xE1133766, 0xC3E5CD7A, 0x00000072, 0x06A1E1BB, 0xDDC54EE9, 0xCA1ABBDE,
    0x00000076, 0x05B9E40A, 0xDCFC13AF, 0xCF28AB68, 0x00000087, 0x10772D05, 0xD65E46C8, 0xC5DEA4DF, 0xFFFE0000,
    0x00000000, 0x00000000, 0x00000000, 0x0000000A, 0xFA93F382, 0xFE60E69A, 0xF76F8667, 0x0000000F, 0xFACCFE95,
    0xFB181469, 0xF2F60FAB, 0x000000CA, 0xF9025B38, 0xFDB4551F, 0xF63C4237, 0x00000027, 0xF224BF0B, 0xF08E7C29,
    0xDFBD18C3, 0x000000E9, 0xEC8DBFEB, 0xF8E78A55, 0xEC3B3CD8, 0x00000035, 0xED03E7B3, 0xEF2456A8, 0xD7EE0019,
    0x00000047, 0xE5404814, 0xEB20C920, 0xCE6F6F69, 0x00000067, 0xD8A94E7A, 0xE6C93E15, 0xBDA9CB0A, 0x00000106,
    0xE2B8ED77, 0xE5CDCD72, 0xBEE51AAE, 0x00000078, 0xDE6F54B1, 0xE2CB57D4, 0xB1EAFBDD, 0x00000080, 0xDAF43410,
    0xE0F448A1, 0xAB6C2C2F, 0x00000086, 0xD7AE6032, 0xDE637895, 0xA3DF940C, 0x000000A0, 0xC4FB57ED, 0xD7F04E34,
    0xA6608F5F, 0x000000B6, 0xBB14F54B, 0xD3228A85, 0xA252CB05, 0xFFFE0000, 0x00000000, 0x00000000, 0x00000000,
    0x00000003, 0x00C2BEC6, 0xFF59B265, 0xFA226693, 0x00000021, 0x02D0D4D9, 0xF81B459D, 0xE66AD1D2, 0x00000026,
    0x03E201C1, 0xF3C7E29D, 0xDCA7C0A4, 0x00000048, 0x0DB85A98, 0xED11073F, 0xDB384EE0, 0x00000055, 0x14874F86,
    0xEAAE5DA2, 0xDEB5D8B3, 0x00000062, 0x159F63A9, 0xE83726F6, 0xD6659323, 0x00000068, 0x186D7159, 0xE627C4F7,
    0xCE106E60, 0x00000074, 0x187E36A5, 0xE44A0E31, 0xC59BA8BA, 0x000000CA, 0x16C08F0E, 0xE4D11B40, 0xC95DC40B,
    0x00000088, 0x11B34F3C, 0xE0F69048, 0xC1902114, 0x00000094, 0x113474D9, 0xDDBBCDCB, 0xB88A6567, 0x000000B9,
    0x01B8EE01, 0xD6AB0720, 0xAC405602, 0xFFFE0000, 0x00000000, 0x00000000, 0x00000000, 0x0000000C, 0x05A04A0D,
    0xFF3818D6, 0xF9F0A8C3, 0x00000026, 0xFADC2E39, 0xFBF4513A, 0xED927065, 0x0000002D, 0xFEC76761, 0xFAB76E24,
    0xEB4A8DEA, 0x0000002F, 0x00F9C0A1, 0xF8BB4B42, 0xE9C6A600, 0x00000030, 0x04393B91, 0xF69C22BF, 0xE770B20A,
    0x00000040, 0x04B317CB, 0xF4759BE9, 0xDE06849E, 0x000000EC, 0x0DCC321C, 0xF45BFE30, 0xE5435973, 0x0000004F,
    0x1081F60A, 0xF1B233C2, 0xDA801A17, 0x00000060, 0x197497A1, 0xEE9EBB2B, 0xCE9E7B40, 0x00000104, 0x17F55267,
    0xEFDF3237, 0xD57764FA, 0x00000090, 0x2EA61601, 0xE8461520, 0xD0E252E0, 0x00000119, 0x2723BAF3, 0xEB19146F,
    0xDA5784A1, 0x000000AF, 0x3A4A57E2, 0xE297AE59, 0xE87E24F7, 0x00000145, 0x3A5805C9, 0xE2914DEF, 0xE87B1FBB,
    0x000000D4, 0x42D407B2, 0xD8F212BD, 0xFFC4EE9C, 0xFFFE0000, 0x00000000, 0x00000000, 0x00000000, 0x00000006,
    0xFE704973, 0xFE98FCCD, 0xF9207165, 0x00000028, 0x04742E4D, 0xF969F279, 0xE49603F2, 0x00000032, 0x06DE0638,
    0xF8D535F9, 0xE12C095C, 0x00000054, 0xF879F060, 0xF507D43C, 0xD65A5E4E, 0x0000005F, 0xF3EE3080, 0xF3F62E70,
    0xD976F44B, 0x0000008E, 0xD8C7CFC3, 0xEAEED40C, 0xDDDA5B8C, 0x000000A2, 0xC7BBB4F0, 0xE7083AA8, 0xD748392A,
    0x000000C9, 0xAB122549, 0xE0F14F5B, 0xD2467B57, 0x000000DF, 0xA269D362, 0xE06435E0, 0xD6965E0C, 0x000000DF,
    0xA1B46F44, 0xDF9DB5E0, 0xD81D75BE, 0x000000F5, 0x9EE94459, 0xDD67D009, 0xE510784E, 0xFFFE0000, 0x00000000,
    0x00000000, 0x00000000, 0x00000001, 0x000F8664, 0xFF9C1EE0, 0xFD8692C8, 0x00000006, 0x005B9325, 0xFEC859E2,
    0xF95AB148, 0x0000000F, 0xFC922F19, 0xFC76A2B0, 0xF3749ADE, 0x00000016, 0xFE71DD3C, 0xFABEF1D1, 0xF000D0C6,
    0x00000016, 0xFEB1B41C, 0xFA3B91D1, 0xEF9AFE1E, 0x00000016, 0xFE149704, 0xFACAB1D1, 0xEFC513B8, 0x00000018,
    0x00270AFC, 0xF904737D, 0xEDBC1DE4, 0x0000001C, 0x02462A6B, 0xF81918A0, 0xEA5EB08F, 0x00000029, 0x09B88B6F,
    0xF5CC5FEB, 0xE414AB95, 0x00000037, 0x0E1CF586, 0xF205A94B, 0xECF89640, 0x0000003B, 0x1058B285, 0xEF87BD28,
    0xF0E32D4D, 0xFFFE0000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x001A6265, 0xFF39D937, 0xFB996063,
    0x00000007, 0x02CD9678, 0xFEC469B5, 0xFA4FE5FF, 0x00000010, 0x07505580, 0xFD4EC142, 0xF8CEE97D, 0x00000014,
    0x099295ED, 0xFC45F237, 0xF8316B63, 0x00000017, 0x0E7A8E1E, 0xF992B5F7, 0xF70B6393, 0x00000018, 0x10349155,
    0xF943DD04, 0xF640DBCF, 0x0000001D, 0x139822F0, 0xF80F888E, 0xF4F9BBC5, 0x00000025, 0x18C7CD02, 0xF62C20E8,
    0xF340DDDC, 0x0000002D, 0x1AFE11C9, 0xF55766AD, 0xF26F28D2, 0x00000031, 0x1C5C43B3, 0xF4CAD52C, 0xF0A88D23,
    0x00000037, 0x1C7104F5, 0xF338F76D, 0xEC0B9DC7, 0xFFFE0000, 0x00000000, 0x00000000, 0x00000000, 0xFFFE0000,
    0x00000000, 0x00000000, 0x00000000,
};

void initSoundManager(void) {
    PlayerCommandInit init;
    void *nullPtr;
    s32 pad0;
    s32 pad1;
    SoundHandleNode **freeList;
    SoundHandleNode *node;

    gPendingMusicCommand = 0;
    gPendingMusicCommandArg = 0;
    gMusicSequenceStopped = 1;
    nullPtr = NULL;
    gCurrentMusicSequenceHandle = 0;
    gCurrentMusicSequenceBank = -1;

    // clang-format off
    { \
        freeList = gFreeSoundHandleStack; \
        node = gSoundHandleNodes; \
        for (;;) { \
            *freeList = node; \
            node++; \
            freeList++; \

        if (!(node < (SoundHandleNode *)&gActiveSoundHandleListHead)) { break; }
} \
        gFreeSoundHandleCount = 13; \
        gCurrentQueuedSoundType = 0; \
    }
    // clang-format on

    gSoundQueueWriteIndex = 0;
    gSoundQueueReadIndex = 0;
    gActiveSoundHandleListHead = nullPtr;
    gActiveSoundHandleListTail = nullPtr;
    gPlayerPositionalSoundHandle0[0] = 0;
    gPlayerLoopingSoundHandle0 = 0;
    gPlayerLoopingSoundId0 = 0;
    gPlayerPositionalSoundHandle1 = 0;
    gPlayerLoopingSoundHandle1 = 0;
    gPlayerLoopingSoundId1 = 0;
    gPlayerPositionalSoundHandle2 = 0;
    gPlayerLoopingSoundHandle2 = 0;
    gPlayerLoopingSoundId2 = 0;
    gPlayerPositionalSoundHandle3 = 0;
    gPlayerLoopingSoundHandle3 = 0;
    gPlayerLoopingSoundId3 = 0;
    gSharedLoopingPositionalSoundHandle = 0;

    gAssetHandles[3] = allocRelocatableHeapBlock(SOUND_HEAP_SIZE);
    LOAD_ASSET(SOUND_POINTER_BANK, 4);
    gAssetHandles[5] = allocRelocatableHeapBlock(MUSIC_SEQUENCE_BUFFER_SIZE);

    init.count = 0x18;
    init.unk4 = &gSchedulerState;
    init.threadPriority = 0x6E;
    init.heapBase = (u8 *)getRelocatableHeapBlockBase(gAssetHandles[3]);
    init.heapLen = SOUND_HEAP_SIZE;
    init.soundBank = (PlayerCommandBank *)getRelocatableHeapBlockBase(gAssetHandles[4]);
    init.sampleBaseOffset = (s32)&_27E290_ROM_START;
    init.tuningTable = gSoundBankWaveTuning;
    init.pitchOffsetTable = gSoundBankWavePitchOffsets;
    init.fxHeader = gSoundEffectCommandPointers;
    init.unk28 = gSoundEffectDefaultPriorities;
    init.maxUpdates = 0x100;
    init.outputRate = 0x5622;
    init.commandListCapacity = 0x2000;
    init.frameRateMultiplier = 1;
    init.dmaBufferCount = 0x20;
    init.dmaBufferSize = 0x1000;
    MusInitialize(&init);
    MusSetMasterVolume(1, 0x7FFF);
}

void stopCurrentMusicSequence(s32 arg0) {
    if (gMusicSequenceStopped == 0) {
        if (gCurrentMusicSequenceHandle != 0) {
            MusHandleStop(gCurrentMusicSequenceHandle, arg0);
        }
        gMusicSequenceStopped = 1;
    }
}

s32 loadMusicSequenceBank(s32 arg0) {
    s32 size;
    SoundRomRange *range;

    if (gCurrentMusicSequenceHandle == 0) {
        range = (SoundRomRange *)((arg0 * 2) + (s32 *)gMusicSequenceRomRanges);
        size = range->romEnd - range->romStart;
        dmaReadRom(range->romStart, getRelocatableHeapBlockBase(gAssetHandles[5]), size);
        if ((gCurrentMusicSequenceHandle =
                 MusStartSong((PlayerCommandData *)getRelocatableHeapBlockBase(gAssetHandles[5]))) != 0) {
            gCurrentMusicSequenceBank = arg0;
            if (range == gRaceMusicSequenceRomRanges) {
                MusSetMasterVolume(2, 0x7FFF);
            } else {
                MusSetMasterVolume(2, 0x60FF);
            }
            gMusicSequenceStopped = 0;
            return 0;
        }
    } else {
        stopCurrentMusicSequence(4);
    }
    return 1;
}

s32 reserveSoundEffectQueueWriteIndex(void) {
    u32 ret;
    s32 temp_v1;

    temp_v1 = gSoundQueueWriteIndex;
    ret = -1;
    if (gSoundQueueReadIndex == ((temp_v1 + 1) & SOUND_MANAGER_QUEUE_MASK)) {
        return ret;
    }
    (&gSoundQueueWriteIndex)[(gSoundQueueReadIndex == ((temp_v1 + 1) & SOUND_MANAGER_QUEUE_MASK)) * 0] =
        (temp_v1 + 1) & SOUND_MANAGER_QUEUE_MASK;
    return temp_v1;
}

s32 reserveSoundEffectQueueReadIndex(void) {
    u32 ret;
    s32 temp_v1;

    temp_v1 = gSoundQueueReadIndex;
    ret = -1;
    if (temp_v1 == gSoundQueueWriteIndex) {
        return ret;
    }
    (&gSoundQueueReadIndex)[(temp_v1 == gSoundQueueWriteIndex) * 0] = (temp_v1 + 1) & SOUND_MANAGER_QUEUE_MASK;
    return temp_v1;
}

void releaseSoundEffectHandleNode(SoundHandleNode *arg0) {
    SoundHandleNode *temp_v0;
    SoundHandleNode *temp_v1;

    temp_v0 = arg0->prev;
    if (temp_v0 == NULL) {
        gActiveSoundHandleListHead = arg0->next;
        temp_v1 = arg0->next;
        if (temp_v1 != NULL) {
            temp_v1->prev = arg0->prev;
        } else {
            gActiveSoundHandleListTail = NULL;
        }
    } else {
        temp_v0->next = arg0->next;
        temp_v1 = arg0->next;
        if (temp_v1 != NULL) {
            temp_v1->prev = arg0->prev;
        } else {
            gActiveSoundHandleListTail = arg0->prev;
        }
    }
    gFreeSoundHandleStack[gFreeSoundHandleCount] = arg0;
    gFreeSoundHandleCount++;
}

SoundHandleNode *acquireSoundEffectHandleNode(void) {
    s32 count;

    count = gFreeSoundHandleCount;
    if (count == 0) {
        return NULL;
    }
    gFreeSoundHandleCount--;
    return gFreeSoundHandleStack[gFreeSoundHandleCount];
}

s32 startCurrentQueuedSoundEffect(void) {
    SoundHandleNode *temp_v0;
    SoundHandleNode *temp_v0_2;
    SoundHandleNode *var_v1;
    s32 var_a0;

    temp_v0 = acquireSoundEffectHandleNode();
    if (temp_v0 == NULL) {
        if (gCurrentQueuedSoundPriority < gActiveSoundHandleListTail->priority) {
            return 1;
        }
        if (gActiveSoundHandleListTail->stopRequested != 0) {
            gActiveSoundHandleListTail->stopRequested = 0;
            MusHandleStop(gActiveSoundHandleListTail->handle, 0);
        }
        return 0;
    }

    var_a0 = gCurrentQueuedSoundId;
    if ((var_a0 >= 0x70) || (var_a0 < 0)) {
        var_a0 = (gCurrentQueuedSoundId = 0);
    }

    temp_v0->handle =
        MusStartEffect2(var_a0, gCurrentQueuedSoundVolume, gCurrentQueuedSoundPan, 0, gCurrentQueuedSoundPriority);
    temp_v0->volume = gCurrentQueuedSoundAux;
    temp_v0->priority = gCurrentQueuedSoundPriority;
    temp_v0->stopRequested = 1;

    if (gActiveSoundHandleListHead == NULL) {
        temp_v0->prev = gActiveSoundHandleListHead;
        temp_v0->next = NULL;
        gActiveSoundHandleListHead = (gActiveSoundHandleListTail = temp_v0);
        return 1;
    }

    var_v1 = gActiveSoundHandleListHead;
    if (gActiveSoundHandleListHead != NULL) {
        for (;;) {
            if (temp_v0->priority >=
                (s32)((u64)((((var_v1->priority & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFF) & 0xFFFFULL)) {
                temp_v0_2 = var_v1->prev;
                temp_v0->next = var_v1;
                temp_v0->prev = temp_v0_2;
                if (temp_v0_2 == NULL) {
                    gActiveSoundHandleListHead = temp_v0;
                } else {
                    temp_v0_2->next = temp_v0;
                }
                var_v1->prev = temp_v0;
                return 1;
            }
            var_v1 = var_v1->next;

            if (!(var_v1 != NULL)) {
                break;
            }
        }
    }

    temp_v0->next = gActiveSoundHandleListTail->next;
    temp_v0->prev = gActiveSoundHandleListTail;
    gActiveSoundHandleListTail->next = temp_v0;
    gActiveSoundHandleListTail = temp_v0;
    return 1;
}

void updateSoundManager(void) {
    SoundHandleNode *node;
    s32 *left;
    s32 *right;
    SoundQueueEntry *entry;
    s32 index;

    osStopThread(&gAudioThread);

    node = gActiveSoundHandleListHead;
    while (node != NULL) {
        if (node->handle == 0) {
            releaseSoundEffectHandleNode(node);
            goto next_node;
        } else {
            if (MusHandleAsk(node->handle) == 0) {
                releaseSoundEffectHandleNode(node);
            }
        }
    next_node:
        node = node->next;
    }

    right = &gPlayerLoopingSoundHandle0, left = gPlayerPositionalSoundHandle0;
    for (;;) {
        if ((*right != 0) && (MusHandleAsk(*right) == 0)) {
            *right = 0;
        }
        if ((*left != 0) && (MusHandleAsk(*left) == 0)) {
            *left = 0;
        }
        right++;

        if (!(&gPlayerLoopingSoundHandle0 > ++left)) {
            break;
        }
    }

    if ((gSharedLoopingPositionalSoundHandle != 0) && (MusHandleAsk(gSharedLoopingPositionalSoundHandle) == 0)) {
        gSharedLoopingPositionalSoundHandle = 0;
    }

    if (gCurrentQueuedSoundType == 0) {
        index = reserveSoundEffectQueueReadIndex();
        if (index != -1) {
            entry = &gSoundQueue[index];
            gCurrentQueuedSoundType = entry->type;
            gCurrentQueuedSoundId = entry->soundId;
            gCurrentQueuedSoundVolume = entry->volume;
            gCurrentQueuedSoundPan = entry->pan;
            gCurrentQueuedSoundPriority = entry->priority;
            gCurrentQueuedSoundAux = entry->unk5;
        }
    }

    if ((gCurrentQueuedSoundType == 1) && (startCurrentQueuedSoundEffect() != 0)) {
        gCurrentQueuedSoundType = 0;
    }

    if ((gCurrentMusicSequenceHandle != 0) && (MusHandleAsk(gCurrentMusicSequenceHandle) == 0)) {
        gCurrentMusicSequenceHandle = 0;
        gCurrentMusicSequenceBank = -1;
    }

    if ((gPendingMusicCommand == 1) && (loadMusicSequenceBank(gPendingMusicCommandArg) == 0)) {
        gPendingMusicCommand = 0;
    }

    if (gPendingMusicCommand == 2) {
        stopCurrentMusicSequence(gPendingMusicCommandArg);
        gPendingMusicCommand = 0;
    }

    osStartThread(&gAudioThread);
}

void requestMusicSequenceBank(s32 arg0) {
    if (arg0 != gCurrentMusicSequenceBank) {
        gCurrentMusicSequenceBank = arg0;
        gPendingMusicCommand = 1;
        gPendingMusicCommandArg = arg0;
    }
}

void requestMusicSequenceStop(s32 arg0) {
    gPendingMusicCommand = 2;
    gPendingMusicCommandArg = arg0;
    gCurrentMusicSequenceBank = -1;
}

s32 enqueueSoundEffect(s16 arg0, s16 arg1) {
    s32 temp_v0 = reserveSoundEffectQueueWriteIndex();
    SoundQueueEntry *temp_v1;

    if (temp_v0 == -1) {
        return 1;
    }
    temp_v1 = &gSoundQueue[temp_v0];
    temp_v1->type = 1;
    temp_v1->soundId = arg0;
    temp_v1->volume = 0xFF;
    temp_v1->pan = 0x80;
    temp_v1->unk5 = 0;
    temp_v1->priority = arg1;
    return 0;
}

s32 enqueueSoundEffectWithVolume(s16 arg0, s16 arg1, s16 arg2) {
    s32 temp_v0;
    SoundQueueEntry *temp_v1;

    if (arg1 <= 0) {
        return 0;
    }
    temp_v0 = reserveSoundEffectQueueWriteIndex();
    if (temp_v0 == -1) {
        return 1;
    }
    if (arg1 >= 0x100) {
        arg1 = 0xFF;
    }
    temp_v1 = &gSoundQueue[temp_v0];
    temp_v1->type = 1;
    temp_v1->soundId = (u8)arg0;
    temp_v1->volume = (u8)arg1;
    temp_v1->pan = 0x80;
    temp_v1->unk5 = 0;
    temp_v1->priority = (u8)arg2;
    return 0;
}

void stopSoundEffects(void) {
    osStopThread(&gAudioThread);
    gCurrentQueuedSoundType = 0;
    gSoundQueueWriteIndex = 0;
    gSoundQueueReadIndex = 0;
    MusStop(1, 0);
    osStartThread(&gAudioThread);
}

void fadeOutAllMusicSequences(void) {
    osStopThread(&gAudioThread);
    MusStop(3, 0x14);
    osStartThread(&gAudioThread);
}

s32 calculatePositionalSoundVolume(Vec3i *pos, s32 volume) {
    RaceCamera *camera;
    s32 distance;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 attenuation;
    s32 adjustedVolume;

    distance = 0x4000000;
    if (volume <= 0) {
        return 0;
    }

    if (volume >= 0x80) {
        volume = 0x7F;
    }

    camera = gRaceCameras;
    do {
        if (camera->initialized.signedValue == 1) {
            dx = camera->prevPos.x - pos->x;
            if ((dx >= -0x4000000) && (dx < 0x4000001)) {
                dy = camera->prevPos.y - pos->y;
                if (((camera->prevPos.y - pos->y) >= -0x4000000) && (dy < 0x4000001)) {
                    dz = camera->prevPos.z - pos->z;
                    if ((dz >= -0x4000000) && (dz < 0x4000001)) {
                        adjustedVolume = integerSquareRoot64((s64)dx * dx + (s64)dy * dy + (s64)dz * dz);
                        if (adjustedVolume < distance) {
                            distance = adjustedVolume;
                        }
                    }
                }
            }
        }
        camera++;
    } while (camera != gRaceCamerasEnd);

    attenuation = 0x1000 - fixedSine(distance / 0x10000);
    adjustedVolume = (volume * attenuation) / 0x1000;
    if (adjustedVolume < 0) {
        adjustedVolume = 0;
    }
    return adjustedVolume;
}

void updatePlayerLoopingPositionalSound(s32 soundId, s32 mode, s32 volume, f32 pitch) {
    s32 adjustedVolume;
    s32 activeCameras;

    if ((f64)pitch > 6.0) {
        pitch = 6.0f;
    }
    if ((f64)pitch < -6.0) {
        pitch = -6.0f;
    }

    adjustedVolume = calculatePositionalSoundVolume(&gRacePlayers[mode].state.pos, volume);

    activeCameras = 0;
    if (gRaceCameras[0].initialized.signedValue != 0) {
        activeCameras = 1;
    }
    if (gRaceCameras[1].initialized.signedValue != 0) {
        activeCameras += 1;
    }
    if (gRaceCameras[2].initialized.signedValue != 0) {
        activeCameras += 1;
    }
    if (gRaceCameras[3].initialized.signedValue != 0) {
        activeCameras += 1;
    }
    if (gRaceDemoPlaybackEnabled != 0) {
        activeCameras = 4;
    }

    adjustedVolume = adjustedVolume - ((((activeCameras * 10) - 1) * adjustedVolume) / 100);

    if (adjustedVolume == 0) {
        if (*(&gPlayerLoopingSoundHandle0 + mode) != 0) {
            MusHandleStop(*(&gPlayerLoopingSoundHandle0 + mode), 0);
            *(&gPlayerLoopingSoundHandle0 + mode) = 0;
        }
    } else {
        if ((*(&gPlayerLoopingSoundHandle0 + mode) != 0) && (soundId != *(&gPlayerLoopingSoundId0 + mode))) {
            MusHandleStop(*(&gPlayerLoopingSoundHandle0 + mode), 0);
            *(&gPlayerLoopingSoundHandle0 + mode) = 0;
        }

        if (*(&gPlayerLoopingSoundHandle0 + mode) == 0) {
            *(&gPlayerLoopingSoundId0 + mode) = soundId;
            *(&gPlayerLoopingSoundHandle0 + mode) =
                MusStartEffect2(soundId, adjustedVolume, 0x80, 0, SFX_PRIORITY_PLAYER_LOOP);
            MusHandleSetFreqOffset(*(&gPlayerLoopingSoundHandle0 + mode), pitch);
        } else {
            MusHandleSetVolume(*(&gPlayerLoopingSoundHandle0 + mode), adjustedVolume);
            MusHandleSetFreqOffset(*(&gPlayerLoopingSoundHandle0 + mode), pitch);
        }
    }
}

void playPlayerPositionalSound(s32 soundId, s32 playerIndex, s32 volume, s32 minVolume) {
    s32 adjustedVolume;

    adjustedVolume = calculatePositionalSoundVolume(&gRacePlayers[playerIndex].state.pos, volume);
    if (adjustedVolume < minVolume) {
        adjustedVolume = minVolume;
    }
    if (adjustedVolume != 0) {
        if (gPlayerPositionalSoundHandle0[playerIndex] != 0) {
            MusHandleStop(gPlayerPositionalSoundHandle0[playerIndex], 0);
            gPlayerPositionalSoundHandle0[playerIndex] = 0;
        }
        gPlayerPositionalSoundHandle0[playerIndex] =
            MusStartEffect2(soundId, adjustedVolume, 0x80, 0, SFX_PRIORITY_HIGH);
    }
}

void updateSingleLoopingPositionalSound(s16 soundId, Vec3i *pos, s16 volume) {
    s32 adjustedVolume;

    adjustedVolume = calculatePositionalSoundVolume(pos, volume);
    if (adjustedVolume == 0) {
        if (gSharedLoopingPositionalSoundHandle != 0) {
            MusHandleStop(gSharedLoopingPositionalSoundHandle, 0);
            gSharedLoopingPositionalSoundHandle = 0;
        }
    } else if (gSharedLoopingPositionalSoundHandle == 0) {
        gSharedLoopingPositionalSoundHandle = MusStartEffect2(soundId, adjustedVolume, 0x80, 0, SFX_PRIORITY_NORMAL);
    } else {
        MusHandleSetVolume(gSharedLoopingPositionalSoundHandle, adjustedVolume);
    }
}

void requestCourseMusicSequence(void) {
    requestMusicSequenceBank(gCourseMusicSequenceBanks[gRaceCourseIndex.signedValue]);
}

void countActiveSoundPlayers(void) {
    MusAsk(3);
}

s32 countActiveMusicSequences(void) {
    return MusAsk(2);
}

void clearPendingPositionalSoundRequests(void) {
    gPendingPositionalSoundRequests = NULL;
}

void enqueuePositionalSoundRequest(
    s32 soundId,
    Vec3i *pos,
    s32 volume,
    s32 priority,
    f32 pitch,
    s32 mode,
    s32 minVolume
) {
    PositionalSoundRequest *node;

    node = allocRenderCallbackScratch(sizeof(PositionalSoundRequest));
    if (node != NULL) {
        node->next = gPendingPositionalSoundRequests;
        node->pos = *pos;
        node->soundId = ((SoundHalfArg *)&soundId)->half.value;
        node->volume = ((SoundHalfArg *)&volume)->half.value;
        node->minVolume = ((SoundHalfArg *)&minVolume)->half.value;
        node->priority = ((SoundHalfArg *)&priority)->half.value;
        node->mode = ((SoundHalfArg *)&mode)->half.value;
        node->pitch = pitch;
        gPendingPositionalSoundRequests = node;
    }
}

void enqueuePlayerLoopingPositionalSoundRequest(s32 arg0, Vec3i *arg1, s32 arg2, s32 arg3, f32 arg4, s16 mode) {
    s32 temp_a0 = arg0 << 16;
    s32 temp_a2 = arg2 << 16;
    s32 temp_a3 = arg3 << 16;

    enqueuePositionalSoundRequest(temp_a0 >> 16, arg1, temp_a2 >> 16, temp_a3 >> 16, arg4, mode, 0);
}

void enqueuePositionalSoundEffect(s16 arg0, Vec3i *arg1, s16 arg2, s16 arg3) {
    enqueuePositionalSoundRequest(arg0, arg1, arg2, arg3, 0.0f, -1, 0);
}

void enqueuePlayerPositionalSoundEffect(s16 arg0, Vec3i *arg1, s16 arg2, s16 arg3, s16 arg4, s16 minVolume) {
    enqueuePositionalSoundRequest(arg0, arg1, arg2, arg3, 0.0f, arg4 + 4, minVolume);
}

void playPendingPositionalSoundRequests(void) {
    PositionalSoundRequest *node;
    s16 mode;

    osStopThread(&gAudioThread);
    node = gPendingPositionalSoundRequests;

    while (node != NULL) {
        mode = node->mode;
        switch (mode) {
            case -1:
                enqueueSoundEffectWithVolume(
                    node->soundId,
                    calculatePositionalSoundVolume(&node->pos, node->volume),
                    node->priority
                );
                break;

            case 0:
            case 1:
            case 2:
            case 3:
                updatePlayerLoopingPositionalSound(node->soundId, mode, node->volume, node->pitch);
                break;

            case 4:
            case 5:
            case 6:
            case 7:
                playPlayerPositionalSound(node->soundId, mode - 4, node->volume, node->minVolume);
                break;

            case 10:
                updateSingleLoopingPositionalSound(node->soundId, &node->pos, node->volume);
                break;
        }
        node = node->next;
    }

    osStartThread(&gAudioThread);
}
