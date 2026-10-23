#ifndef SOUND_IDS_H
#define SOUND_IDS_H

/* Indices into gSoundEffectCommandPointers, installed as PlayerCommandInit.fxHeader
 * and selected by MusStartEffect/MusStartEffect2 and the sound-manager queue APIs.
 * Names describe established call-site roles; effects may serve multiple roles.
 * Voice roles follow enqueueRacePlayerVoiceSound; board loops follow motion feedback.
 * Entries without C callers retain UNUSED names and, when unidentified, sample indices.
 * ALT entries are distinct programs/samples, not aliases with interchangeable IDs.
 */
typedef enum SoundEffectId {
    SFX_NONE = 0x00,
    SFX_MENU_ACCEPT = 0x01,
    SFX_RESULTS_TALLY_COMPLETE = 0x02,
    /* Default motion-feedback loop (surface/effect type 0). */
    SFX_BOARD_GLIDE_LOOP = 0x03,
    /* Fallback contact loop (types 2/4 and the airborne override). */
    SFX_BOARD_CONTACT_FALLBACK_LOOP = 0x04,
    /* Surface/effect type 3 motion-feedback loop. */
    SFX_BOARD_SURFACE_SPRAY_LOOP = 0x05,
    /* Surface/effect type 4, surface 0xD; drives rumble. */
    SFX_BOARD_SURFACE_RUMBLE_LOOP = 0x06,
    SFX_SPEED_FAN_LOOP = 0x07,
    SFX_CRASH_SLIDE_LOOP = 0x08,
    SFX_SHOCK = 0x09,
    SFX_PROJECTILE_IMPACT = 0x0A,
    SFX_SPIN_HIT = 0x0B,
    SFX_GHOST_HIT = 0x0C,
    SFX_GHOST_SLOWDOWN = 0x0D,
    SFX_SUNSET_ROCK_AMBIENCE = 0x0E,
    SFX_ITEM_STEAL_START = 0x0F,
    SFX_ITEM_USE = 0x10,
    SFX_ITEM_IMPACT = 0x11,
    SFX_ITEM_BREAK = 0x12,
    SFX_AREA_BLAST = 0x13,
    SFX_COLLISION_IMPACT = 0x14,
    SFX_ITEM_STEAL_END = 0x15,
    SFX_COURSE_GATE_OPEN_1 = 0x16,
    SFX_TRICK_START = 0x17,
    SFX_CONFIRM = 0x18,
    SFX_MENU_CURSOR = 0x19,
    SFX_COIN_TICK = 0x1A,
    SFX_COURSE_GATE_OPEN_2 = 0x1B,
    SFX_COURSE_GATE_CLOSE = 0x1C,
    /* No C caller; effect uses sample 91. */
    SFX_UNUSED_SAMPLE_91 = 0x1D,
    SFX_BREAKABLE_SURFACE_LOOP = 0x1E,
    SFX_ITEM_PICKUP = 0x1F,
    SFX_FALLING_ROCK_BOUNCE = 0x20,
    SFX_PLAYER_DESCENT = 0x21,
    SFX_SLASH_TRICK = 0x22,
    SFX_SLASH_COMPLEX_TRICK = 0x23,
    SFX_SLASH_HIT = 0x24,
    SFX_SLASH_ATTACK_SUCCESS = 0x25,
    /* Also used for the main-menu secret-code unlock. */
    SFX_SLASH_CHEER = 0x26,
    SFX_SLASH_OBSTACLE_HIT = 0x27,
    SFX_NANCY_TRICK = 0x28,
    SFX_NANCY_COMPLEX_TRICK = 0x29,
    SFX_NANCY_HIT = 0x2A,
    SFX_NANCY_CHEER = 0x2B,
    SFX_NANCY_ATTACK_SUCCESS = 0x2C,
    SFX_NANCY_OBSTACLE_HIT = 0x2D,
    SFX_LINDA_TRICK = 0x2E,
    SFX_LINDA_COMPLEX_TRICK = 0x2F,
    SFX_LINDA_HIT = 0x30,
    SFX_LINDA_CHEER = 0x31,
    SFX_LINDA_ATTACK_SUCCESS = 0x32,
    SFX_LINDA_OBSTACLE_HIT = 0x33,
    /* No C caller; sample lies in the Linda voice group. */
    SFX_LINDA_UNUSED_VOICE = 0x34,
    SFX_JAM_TRICK = 0x35,
    SFX_JAM_COMPLEX_TRICK = 0x36,
    SFX_JAM_HIT = 0x37,
    SFX_JAM_ATTACK_SUCCESS = 0x38,
    SFX_JAM_CHEER = 0x39,
    SFX_JAM_OBSTACLE_HIT = 0x3A,
    SFX_TOMMY_TRICK = 0x3B,
    SFX_TOMMY_COMPLEX_TRICK = 0x3C,
    SFX_TOMMY_HIT = 0x3D,
    SFX_TOMMY_CHEER = 0x3E,
    SFX_TOMMY_OBSTACLE_HIT = 0x3F,
    /* Also used for Shinobin course selection and complex tricks. */
    SFX_SHINOBIN_TRICK = 0x40,
    SFX_SHINOBIN_HIT = 0x41,
    SFX_SHINOBIN_CHEER = 0x42,
    SFX_SHINOBIN_OBSTACLE_HIT = 0x43,
    SFX_SHOP_GREETING = 0x44,
    SFX_COURSE_PURCHASE_PROMPT = 0x45,
    SFX_MENU_BACK = 0x46,
    SFX_WARNING = 0x47,
    /* No C caller; effect uses sample 159. */
    SFX_UNUSED_SAMPLE_159 = 0x48,
    SFX_COURSE_PURCHASE_AVAILABLE = 0x49,
    SFX_MAIN_MENU_ENTER = 0x4A,
    SFX_RACE_READY = 0x4B,
    SFX_RACE_GO = 0x4C,
    SFX_TRICK_SCORE_LOW = 0x4D,
    SFX_TRICK_SCORE_MEDIUM = 0x4E,
    SFX_TRICK_SCORE_HIGH = 0x4F,
    SFX_TRICK_SCORE_TOP = 0x50,
    SFX_TRICK_BONUS = 0x51,
    SFX_FINAL_LAP = 0x52,
    SFX_SPARKLE_RETARGET = 0x53,
    SFX_COURSE_COLUMN_RIGHT_SELECT = 0x54,
    SFX_COURSE_COLUMN_MIDDLE_SELECT = 0x55,
    SFX_COURSE_COLUMN_LEFT_SELECT = 0x56,
    SFX_SLASH_SELECT = 0x57,
    SFX_NANCY_SELECT = 0x58,
    SFX_LINDA_SELECT = 0x59,
    SFX_JAM_SELECT = 0x5A,
    SFX_TOMMY_SELECT = 0x5B,
    /* No C caller; shares the Tommy-select sample. */
    SFX_TOMMY_SELECT_ALT = 0x5C,
    SFX_SHINOBIN_SELECT = 0x5D,
    /* No C caller; alternate Slash trick sample. */
    SFX_SLASH_TRICK_ALT = 0x5E,
    SFX_SLASH_HIT_ALT = 0x5F,
    SFX_NANCY_COMPLEX_TRICK_ALT = 0x60,
    /* No C caller; sample lies in the Nancy voice group. */
    SFX_NANCY_UNUSED_VOICE = 0x61,
    SFX_NANCY_HIT_ALT = 0x62,
    SFX_LINDA_COMPLEX_TRICK_ALT = 0x63,
    SFX_LINDA_HIT_ALT = 0x64,
    SFX_JAM_HIT_ALT = 0x65,
    SFX_RAT_FACE_APPEAR = 0x66,
    SFX_TEACUP_BUMP = 0x67,
    SFX_RAT_FACE_LAND = 0x68,
    SFX_RAT_FACE_STEAL = 0x69,
    SFX_PROJECTILE_LAUNCH = 0x6A,
    /* Started by the main-menu entrance program; shares its sample. */
    SFX_MAIN_MENU_ENTER_ALT = 0x6B
} SoundEffectId;

#define SOUND_EFFECT_COUNT (SFX_MAIN_MENU_ENTER_ALT + 1)

/* Common queue priorities. Higher values take precedence over lower values. */
typedef enum SoundEffectPriority {
    SFX_PRIORITY_LOW = 49,
    SFX_PRIORITY_NORMAL = 50,
    SFX_PRIORITY_PLAYER_LOOP = 70,
    SFX_PRIORITY_HIGH = 90
} SoundEffectPriority;

/* Categories accepted by enqueueRacePlayerVoiceSound, not effect-table IDs. */
typedef enum RacePlayerVoiceCue {
    RACE_VOICE_TRICK = 0,
    RACE_VOICE_COMPLEX_TRICK = 1,
    RACE_VOICE_HIT = 2,
    RACE_VOICE_HIT_LOCAL = 3,
    RACE_VOICE_OBSTACLE_HIT = 4,
    RACE_VOICE_ATTACK_SUCCESS = 5,
    RACE_VOICE_CHEER = 6,
    RACE_VOICE_LANDING = 7
} RacePlayerVoiceCue;

#endif
