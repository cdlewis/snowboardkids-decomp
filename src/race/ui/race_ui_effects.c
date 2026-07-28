#include "game/race/race_state.h"
#include "common.h"
#include "game/save_data.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/engine/render_callback.h"
#include "game/engine/system_runtime.h"
#include "game/engine/controller_input.h"
#include "assets.h"
#include "game/race/ui/race_ui_effects.h"
#include "game/race/effects/snowboard_trail_effects.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/race/items/race_item_hit_flags.h"
#include "game/engine/viewport_manager.h"
#include "game/math/spatial_math.h"
#include "game/math/fixed_point_math.h"
#include "game/race/motion/race_motion.h"
#include "game/race/items/race_item_projectiles.h"
#include "game/race/player/race_player_movement.h"
#include "game/race/player/race_player_input.h"
#include "game/race/ui/race_hud.h"

/* Local 3-arg declaration; see note in callback_task_scheduler.h. */
extern void *createCallbackTaskWithUserIdPreservingArgs(void *, s32, s32);
extern void drawAssetTableSpriteWideIndex(s16 x, s16 y, AssetTable *table, s32 entryIndex);
extern void drawAssetTableSpriteWithExplicitPaletteWideIndex(s16 x, s16 y, AssetTable *table, s32 entryIndex,
                                                             u16 paletteIndex);

#define RACE_UI_TRAIL_GFX_ALLOC_PTR (*(RaceUiDisplayCommand **)&gRegionAllocPtr)
#define ASSET_HANDLE(index) (gAssetHandles[(index)])
#define RACE_UI_SNOWBOARD_TRAIL_SCALE_X 0xF0000
#define RACE_UI_SNOWBOARD_TRAIL_SCALE_Y 0x230000
#define RACE_UI_SNOWBOARD_TRAIL_SCALE_Z 0
#define RACE_UI_SNOWBOARD_TRAIL_TIMER 0xF
#define RACE_UI_SNOWBOARD_TRAIL_INITIAL_VELOCITY_Y 0x20000
#define RACE_UI_SINGLE_TRAIL_TIMER 0x3C
#define RACE_UI_SINGLE_TRAIL_LOCAL_ROT_Y 0x800
#define RACE_UI_SINGLE_TRAIL_LOCAL_X 0
#define RACE_UI_SINGLE_TRAIL_LOCAL_Y 0x100000
#define RACE_UI_SINGLE_TRAIL_LOCAL_Z -0x200000
#define ICE_COURSE_BUMPER_BASE_SCALE 0x1000
#define ICE_COURSE_BUMPER_ROTATION_STEP 0x40
#define RACE_UI_RESULTS_FADE_STEP 0x10
#define RACE_UI_RESULTS_FULL_ALPHA 0xFF
#define RACE_UI_RESULTS_REVEAL_TIMER 0x14
#define RACE_UI_PRIZE_PAYOUT_REVEAL_TIMER 0x14
#define RACE_UI_COURSE_RECORD_REVEAL_TIME_PRIZE_STATE 0
#define RACE_UI_COURSE_RECORD_REVEAL_SPEED_FAN_GET_STATE 1
#define RACE_UI_COURSE_RECORD_REVEAL_PERFECT_GET_STATE 2
#define RACE_UI_COURSE_RECORD_FINAL_MONEY_STATE 3
#define RACE_UI_COURSE_RECORD_HOLD_FINAL_MONEY_STATE 4
#define RACE_UI_HIT_PRIZE_SCORE_RATE 0x12C
#define RACE_UI_HIT_PRIZE_QUICKSAND_VALLEY_SCORE_RATE 0x64
#define RACE_UI_HIT_PRIZE_PERFECT_HIT_BONUS 0x3E8
#define RACE_UI_HIT_PRIZE_SHOW_HIT_PRIZE 0
#define RACE_UI_HIT_PRIZE_SHOW_PERFECT_HIT 1
#define RACE_UI_HIT_PRIZE_SHOW_COMPLETE_BONUS 2
#define RACE_UI_HIT_PRIZE_SHOW_TOTAL_MONEY 3
#define RACE_UI_TRICK_PRIZE_REVEAL_DELAY 0x14
#define RACE_UI_TRICK_PRIZE_SHOW_COMPLETE_BONUS 2
#define RACE_UI_TRICK_PRIZE_SHOW_TOTAL_MONEY 3
#define RACE_UI_STUN_ORBIT_ANGLE_0 0
#define RACE_UI_STUN_ORBIT_ANGLE_1 0x555
#define RACE_UI_STUN_ORBIT_ANGLE_2 0xAAA
#define SCALE_MATRIX_COMPONENT(value, scale) ((value * scale) / 0x1000)
#define RACE_UI_SP_TRIANGLE_WORD(v0, v1, v2) (_SHIFTL((v0) * 2, 16, 8) | _SHIFTL((v1) * 2, 8, 8) | _SHIFTL((v2) * 2, 0, 8))
#define RACE_UI_SP_QUADRANGLE_WORD0(v0, v1, v2, v3, flag) \
    (((flag) == 0) ? RACE_UI_SP_TRIANGLE_WORD(v0, v1, v2) : \
     ((flag) == 1) ? RACE_UI_SP_TRIANGLE_WORD(v1, v2, v3) : \
     ((flag) == 2) ? RACE_UI_SP_TRIANGLE_WORD(v2, v3, v0) : \
                     RACE_UI_SP_TRIANGLE_WORD(v3, v0, v1))
#define RACE_UI_SP_QUADRANGLE_WORD1(v0, v1, v2, v3, flag) \
    (((flag) == 0) ? RACE_UI_SP_TRIANGLE_WORD(v0, v2, v3) : \
     ((flag) == 1) ? RACE_UI_SP_TRIANGLE_WORD(v1, v3, v0) : \
     ((flag) == 2) ? RACE_UI_SP_TRIANGLE_WORD(v2, v0, v1) : \
                     RACE_UI_SP_TRIANGLE_WORD(v3, v1, v2))
#define COURSE_START_FINISH_SCROLL_FRAME_MASK 7
#define COURSE_START_FINISH_SCROLL_STEP 4
#define COURSE_START_FINISH_SCROLL_MASK 0x3F
#define TIME_TRIAL_RECORD_DELTA_POPUP_START_X 0x1A4
#define TIME_TRIAL_RECORD_DELTA_POPUP_START_Y -0x2C
#define TIME_TRIAL_RECORD_DELTA_POPUP_START_VELOCITY 0x38
#define TIME_TRIAL_RECORD_DELTA_POPUP_HOLD_TIMER 0x5A
#define RACE_UI_TIME_TRIAL_SAVE_DATA ((RaceUiTimeTrialSaveData *)&gGameSaveDataBuffer)
#define RACE_UI_GSP_VERTEX_F3DEX(pkt, v, n, v0) \
    gDma1p((pkt), G_VTX, (v), ((n) << 10) | (sizeof(Vtx) * (n) - 1), (v0) * 2)
#define RACE_UI_GSP1QUADRANGLE_F3DEX(pkt, v0, v1, v2, v3, flag) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(0xB1, 24, 8) | RACE_UI_SP_QUADRANGLE_WORD0(v0, v1, v2, v3, flag); \
    _g->words.w1 = RACE_UI_SP_QUADRANGLE_WORD1(v0, v1, v2, v3, flag); \
}

typedef struct {
    s32 word;
    u16 half;
    u8 b6;
    u8 b7;
} RaceUiSpriteInit;

typedef RacePlayerTransformBlock RaceUiTrailCopyBlock;

typedef union {
    /* 0x00 */ RaceUiTrailCopyBlock source;
    /* 0x00 */ s64 forceAlignment;
} RaceUiAlignedTrailCopyBlock;

typedef union {
    s32 word;
    struct {
        s16 hi;
        s16 lo;
    } half;
} SplitWord;

typedef struct {
    /* 0x00 */ u16 assetId;
    /* 0x02 */ u8 pad2[6];
} RaceUiAssetEntry;

typedef Mtx RaceUiGfxCommandDest;

typedef struct {
    /* 0x00 */ RaceUiTrailCopyBlock source;
    /* 0x20 */ u8 pad20[0x40 - 0x20];
} RaceUiTransitionTransformSource;

typedef struct RaceUiTransitionActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ u8 pad18[0x24 - 0x18];
    /* 0x24 */ RaceUiTransitionTransformSource transformSource;
    /* 0x64 */ RaceUiGfxCommandDest *matrix;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s16 unk6C;
    /* 0x6E */ s16 unk6E;
    /* 0x70 */ u8 pad70[2];
    /* 0x72 */ u8 matrixDirty;
} RaceUiTransitionActor;

typedef struct RaceUiSlideActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ void *image;
    /* 0x1C */ void *palette;
    /* 0x20 */ s16 angle;
    /* 0x22 */ u8 pad22[2];
    /* 0x24 */ Gfx *displayList;
    /* 0x28 */ s32 velocity;
    /* 0x2C */ s32 soundIndex;
} RaceUiSlideActor;

typedef struct RaceUiCounterActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ s16 alpha;
    /* 0x1A */ s16 timer;
    /* 0x1C */ s16 state;
    union {
        /* 0x1E */ s16 value;
        /* 0x1E */ s16 pendingHitPrize;
    };
    union {
        /* 0x20 */ s16 bonus;
        /* 0x20 */ s16 pendingPerfectHitBonus;
    };
    union {
        /* 0x22 */ s16 target;
        /* 0x22 */ s16 pendingCompleteBonus;
    };
    union {
        /* 0x24 */ s16 flag;
        /* 0x24 */ s16 hasPerfectHitBonus;
    };
} RaceUiCounterActor;

typedef struct RaceUiDualCounterActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    union {
        struct {
            /* 0x18 */ s8 row;
            /* 0x19 */ s8 column;
        };
        /* 0x18 */ s16 alpha18;
    };
    /* 0x1A */ s16 timeFraction;
    /* 0x1C */ s16 alpha;
    /* 0x1E */ s16 timer;
    /* 0x20 */ s16 state;
    /* 0x22 */ s16 leftValue;
    /* 0x24 */ s16 bonus;
    /* 0x26 */ s16 leftTarget;
    /* 0x28 */ s16 rightValue;
    /* 0x2A */ s16 flag;
} RaceUiDualCounterActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ s16 angle;
    /* 0x26 */ s16 angle2;
    /* 0x28 */ s16 frame;
    /* 0x2A */ s16 frameTarget;
    /* 0x2C */ s16 rotX;
    /* 0x2E */ s16 rotY;
    /* 0x30 */ s16 scale;
    /* 0x32 */ s16 scaleStep;
} RaceUiModelActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ void *ptr18;
    /* 0x1C */ void *ptr1C;
    /* 0x20 */ void *ptr20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ Vec3i pos;
    /* 0x34 */ s16 frame;
    /* 0x36 */ s16 frameEnd;
    /* 0x38 */ Vec3i assetPos;
    /* 0x44 */ s32 assetX;
    /* 0x48 */ Vec3i vec48;
    /* 0x54 */ s16 state;
    /* 0x56 */ s16 timer;
    /* 0x58 */ s32 velX;
    /* 0x5C */ s32 velY;
    /* 0x60 */ s32 velZ;
    /* 0x64 */ u8 pad64[4];
    /* 0x68 */ s16 unk68;
} RaceUiEffectActor;

typedef struct RaceUiProjectileActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Vec3i velocity;
    /* 0x30 */ u16 *animationScript;
    /* 0x34 */ s16 frameTimer;
    /* 0x36 */ s16 flags;
    /* 0x38 */ RaceUiGfxCommandDest *matrix;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ u8 pad3E[2];
    /* 0x40 */ void *palette;
    /* 0x44 */ void *image;
    /* 0x48 */ u8 pad48[4];
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 verticalVelocity;
    /* 0x54 */ s32 verticalAcceleration;
    /* 0x58 */ u8 matrixDirty;
} RaceUiProjectileActor;

typedef struct {
    /* 0x00 */ u8 pad[0x10];
} RaceUiProjectileVertexBlock;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
} RaceUiEffectParticle;

typedef struct {
    struct {
        u32 w0;
        u32 w1;
    } words;
} RaceUiDisplayCommand;

typedef struct {
    /* 0x00 */ s16 state;
    /* 0x02 */ u8 pad02[0x1C - 0x02];
    /* 0x1C */ Vec3i worldPos;
    /* 0x28 */ RaceUiTrailCopyBlock copyBlock;
    /* 0x48 */ u8 pad48[0x6A - 0x48];
    /* 0x6A */ s16 spinYaw;
} RaceUiSnowboardTrailState;

typedef struct RaceUiSnowboardTrailPlayer {
    /* 0x000 */ u16 playerIndex;
    /* 0x002 */ u8 pad002[0x58C - 0x002];
    /* 0x58C */ RaceUiSnowboardTrailState trail;
} RaceUiSnowboardTrailPlayer;

typedef struct RaceUiSnowboardTrailActor {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ Vec3i scale;
    /* 0x30 */ Vec3i worldPos;
    /* 0x3C */ s32 velocityY;
    /* 0x40 */ RaceUiTrailCopyBlock frontTransform;
    /* 0x60 */ RaceUiTrailCopyBlock backTransform;
    /* 0x80 */ s16 playerIndex;
    /* 0x82 */ u8 pad82[2];
    /* 0x84 */ s16 spinYaw;
    /* 0x86 */ u8 pad86[2];
    /* 0x88 */ RaceUiGfxCommandDest *frontMatrix;
    /* 0x8C */ RaceUiGfxCommandDest *backMatrix;
    /* 0x90 */ s16 scaleStep;
    /* 0x92 */ s16 timer;
    /* 0x94 */ u8 matrixDirty;
} RaceUiSnowboardTrailActor;

typedef struct RaceUiRankParticleActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ RaceUiTrailCopyBlock copyBlock;
    /* 0x44 */ void *matrix;
    /* 0x48 */ u32 *displayLists[2];
    /* 0x50 */ u8 matrixDirty;
} RaceUiRankParticleActor;

typedef struct RaceUiPodiumTrailActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ RaceUiTrailCopyBlock copyBlock;
    /* 0x44 */ RaceUiGfxCommandDest *matrix;
    /* 0x48 */ s32 height;
    /* 0x4C */ s32 velocity;
    /* 0x50 */ s16 playerIndex;
    /* 0x52 */ s16 targetPlayerIndex;
    /* 0x54 */ s16 state;
    /* 0x56 */ s16 timer;
    /* 0x58 */ u8 matrixDirty;
    /* 0x59 */ u8 playImpactSound;
} RaceUiPodiumTrailActor;

typedef struct RaceUiRankTrailActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 playerIndex;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ RaceUiTrailCopyBlock copyBlock;
    /* 0x44 */ RaceUiGfxCommandDest *matrix;
    /* 0x48 */ s16 scale;
    /* 0x4A */ u8 matrixDirty;
} RaceUiRankTrailActor;

typedef struct RaceUiTextParticleActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ RaceUiGfxCommandDest *matrix;
    /* 0x28 */ void *palettes[4];
    /* 0x38 */ void *images[4];
    /* 0x48 */ Vec3i velocity;
    /* 0x54 */ s16 timer;
    /* 0x56 */ u8 matrixDirty;
    /* 0x57 */ u8 useAltTextures;
} RaceUiTextParticleActor;

typedef struct RaceUiAnimatedTextActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ RaceUiGfxCommandDest *matrix;
    /* 0x28 */ void *palettes[4];
    /* 0x38 */ void *images[4];
    /* 0x48 */ Vec3i velocity;
    /* 0x54 */ s16 frame;
    /* 0x56 */ u8 matrixDirty;
} RaceUiAnimatedTextActor;

typedef struct RaceUiTrailingParticleActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ RaceUiGfxCommandDest *matrix0;
    /* 0x28 */ RaceUiGfxCommandDest *matrix1;
    /* 0x2C */ s16 rotY;
    /* 0x2E */ s16 rotX;
    /* 0x30 */ u8 matrixDirty;
} RaceUiTrailingParticleActor;

typedef struct RaceUiTripleParticleActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ RaceUiGfxCommandDest *matrix0;
    /* 0x28 */ RaceUiGfxCommandDest *matrix1;
    /* 0x2C */ RaceUiGfxCommandDest *matrix2;
    /* 0x30 */ s16 rotY;
    /* 0x32 */ u8 matrixDirty;
} RaceUiTripleParticleActor;

typedef struct RaceUiSpinningParticleActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ RaceUiGfxCommandDest *matrix0;
    /* 0x28 */ RaceUiGfxCommandDest *matrix1;
    /* 0x2C */ s16 rotY;
    /* 0x2E */ s16 rotZ;
    /* 0x30 */ s16 rotX;
    /* 0x32 */ s16 rotX2;
    /* 0x34 */ u8 matrixDirty;
} RaceUiSpinningParticleActor;

typedef struct RaceUiScaledParticleActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ s16 scale;
    /* 0x26 */ u8 pad26[2];
    /* 0x28 */ RaceUiGfxCommandDest *matrix;
    /* 0x2C */ s16 rotY;
    /* 0x2E */ s16 rotYStep;
    /* 0x30 */ u8 matrixDirty;
} RaceUiScaledParticleActor;

typedef struct RaceUiRisingTrailActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s16 angle;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ s16 sineAngle;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ RaceUiGfxCommandDest *matrix;
    /* 0x34 */ u8 pad34[2];
    /* 0x36 */ u8 matrixDirty;
} RaceUiRisingTrailActor;

typedef struct RaceUiThrownTrailActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ s16 surface;
    /* 0x26 */ s16 targetAngle;
    /* 0x28 */ s16 angle;
    /* 0x2A */ s16 angleStep;
    /* 0x2C */ s16 spin;
    /* 0x2E */ u8 pad2E[6];
    /* 0x34 */ s16 soundTimer;
} RaceUiThrownTrailActor;

typedef struct RaceUiSingleTrailActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 playerIndex;
    /* 0x12 */ u8 pad12[0x24 - 0x12];
    /* 0x24 */ RaceUiTrailCopyBlock copyBlock;
    /* 0x44 */ FixedTransform localTransform;
    /* 0x64 */ RaceUiGfxCommandDest *matrix;
    /* 0x68 */ s16 timer;
    /* 0x6A */ u8 matrixDirty;
} RaceUiSingleTrailActor;

typedef struct RaceUiFadingTrailActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ RaceUiTrailCopyBlock copyBlock;
    /* 0x38 */ Vec3i pos;
    /* 0x44 */ RaceUiGfxCommandDest *matrix;
    /* 0x48 */ u8 pad48[2];
    /* 0x4A */ s16 alpha;
    /* 0x4C */ u8 pad4C[2];
    /* 0x4E */ u8 matrixDirty;
} RaceUiFadingTrailActor;

typedef struct RaceUiFadingImpactActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 playerIndex;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    /* 0x18 */ RaceUiTrailCopyBlock copyBlock;
    /* 0x38 */ Vec3i pos;
    /* 0x44 */ RaceUiGfxCommandDest *matrix;
    /* 0x48 */ s16 angle;
    /* 0x4A */ s16 alpha;
    /* 0x4C */ s16 scale;
    /* 0x4E */ u8 matrixDirty;
} RaceUiFadingImpactActor;

typedef struct RaceUiTransitionRenderActor {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ RaceUiTrailCopyBlock copyBlock;
    /* 0x44 */ u8 pad44[0x64 - 0x44];
    /* 0x64 */ RaceUiGfxCommandDest *matrix;
    /* 0x68 */ u8 pad68[0x72 - 0x68];
    /* 0x72 */ u8 matrixDirty;
} RaceUiTransitionRenderActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ RaceUiTrailCopyBlock transformSource;
    /* 0x38 */ Vec3i sourcePos;
    /* 0x44 */ RaceUiGfxCommandDest *matrix;
    /* 0x48 */ s16 angle;
    /* 0x4A */ s16 alpha;
    /* 0x4C */ s16 scale;
    /* 0x4E */ u8 matrixDirty;
} RaceUiFragmentActor;

typedef struct RaceUiEffectParticleActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ RaceUiEffectParticle *particles;
    /* 0x1C */ void *unk1C;
    /* 0x20 */ void *unk20;
    /* 0x24 */ s16 count;
} RaceUiEffectParticleActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ Vec3i transformOffset;
    /* 0x50 */ u8 pad50[0xB0 - 0x50];
} RaceUiCameraTransformSource;

typedef struct {
    /* 0x00 */ s16 active;
    /* 0x02 */ s16 sentinel;
    /* 0x04 */ Vec3i position;
} RaceUiGfxCommandScriptEntry;

typedef struct RaceUiGfxCommandActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ RaceUiGfxCommandDest *particles;
    /* 0x1C */ s16 textureOffset;
    /* 0x1E */ s16 count;
} RaceUiGfxCommandActor;

typedef struct RaceUiRankTextRenderActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ void *matrices;
    /* 0x1C */ s16 count;
} RaceUiRankTextRenderActor;

typedef struct {
    /* 0x00 */ s16 active;
    /* 0x02 */ s16 pad2;
    /* 0x04 */ Vec3i position;
    /* 0x10 */ s16 angle;
    /* 0x12 */ u8 pad12[2];
} RaceUiRankTextRenderEntry;

typedef struct RaceUiRankTrigger {
    /* 0x00 */ struct RaceUiRankTrigger *next;
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 y;
    /* 0x0C */ s32 z;
    /* 0x10 */ s32 radius;
    /* 0x14 */ s8 triggered;
} RaceUiRankTrigger;

typedef struct RaceUiPopupActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ u16 timer;
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ s32 x;
    /* 0x20 */ SplitWord y;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s32 velocity;
    /* 0x2C */ void *parent;
    /* 0x30 */ s8 playerIndex;
} RaceUiPopupActor;

typedef struct RaceUiTimeTrialRecordDeltaPopupActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ u16 timer;
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ s32 x;
    /* 0x20 */ SplitWord y;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s32 velocity;
    /* 0x2C */ RaceTimer delta;
    /* 0x30 */ s8 isSlowerThanRecord;
} RaceUiTimeTrialRecordDeltaPopupActor;

typedef struct {
    /* 0x000 */ u8 pad0[0x12A];
    /* 0x12A */ RaceTimer timeTrialRecordSplitTimes[11];
} RaceUiTimeTrialSaveData;

typedef struct {
    /* 0x0000 */ u8 pad0[0x156];
    /* 0x0156 */ RaceTimer resultNames[10][5];
} RaceUiResultNameData;

typedef struct {
    /* 0x0000 */ u8 pad0[0x78A0];
    /* 0x78A0 */ u8 resultIcons[10][5];
} RaceUiResultIconData;

typedef struct {
    /* 0x0000 */ u8 pad0[0x7832];
    /* 0x7832 */ u8 values[10][5];
    /* 0x7864 */ u8 pad7864[0x7869 - 0x7864];
    /* 0x7869 */ u8 icons[10][5];
} RaceUiCourseValueData;

typedef struct RaceUiPromptActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    /* 0x18 */ u32 timer;
    /* 0x1C */ s16 y;
} RaceUiPromptActor;

typedef struct RaceUiCourseStatsActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    union {
        /* 0x18 */ s16 alpha;
        /* 0x18 */ s16 titleAlpha;
    };
    union {
        /* 0x1A */ s16 timer;
        /* 0x1A */ s16 statsAlpha;
    };
    union {
        /* 0x1C */ s16 visibleRows;
        /* 0x1C */ s16 holdTimer;
    };
    /* 0x1E */ s16 pendingTrickPrize;
    /* 0x20 */ s16 pendingMakeBonus;
    /* 0x22 */ s16 pendingCompleteBonus;
} RaceUiCourseStatsActor;

typedef struct {
    /* 0x0000 */ u8 pad0[0x4E];
    /* 0x004E */ RaceTimer courseStatsNames[10][5];
} RaceUiCourseStatsNameData;

typedef struct {
    /* 0x0000 */ u8 pad0[0x77FB];
    /* 0x77FB */ u8 courseStatsIcons[10][5];
} RaceUiCourseStatsIconData;

typedef struct {
    /* 0x0000 */ u8 pad0[0x7756];
    /* 0x7756 */ u16 values[10][5];
} RaceUiTrickValueData;

typedef struct {
    /* 0x0000 */ u8 pad0[0x77C4];
    /* 0x77C4 */ u8 icons[10][5];
} RaceUiTrickIconData;

typedef struct RaceUiAlphaActor {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s16 alpha;
} RaceUiAlphaActor;

typedef struct RaceUiAlpha1AActor {
    /* 0x00 */ u8 pad0[0x1A];
    /* 0x1A */ s16 alpha;
} RaceUiAlpha1AActor;


typedef struct RaceUiAlpha18Actor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 alpha;
    /* 0x1A */ s16 timer;
    /* 0x1C */ s16 score;
} RaceUiAlpha18Actor;

typedef struct RaceUiPrizePayoutActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 alpha;
    /* 0x1A */ s16 timer;
    /* 0x1C */ s16 prizeAmount;
} RaceUiPrizePayoutActor;

typedef struct RaceUiResultsBannerActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 alpha;
    /* 0x1A */ s16 timer;
    union {
        struct {
            /* 0x1C */ s8 player0;
            /* 0x1D */ s8 player1;
            /* 0x1E */ s8 player2;
            /* 0x1F */ s8 player3;
        };
        /* 0x1C */ s8 playerIndices[4];
    };
} RaceUiResultsBannerActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ u16 *script;
    /* 0x34 */ s16 timer;
    /* 0x36 */ s16 value;
    /* 0x38 */ u8 pad38[8];
    /* 0x40 */ s32 assetY;
    /* 0x44 */ s32 assetX;
} RaceUiScriptActor;

typedef struct RaceUiOverlayActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 x;
    /* 0x1C */ s32 y;
    /* 0x20 */ s32 z;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s32 velocity;
    /* 0x2C */ u8 pad2C[4];
    /* 0x30 */ s16 timer;
    /* 0x32 */ s16 assetTimer;
    /* 0x34 */ RaceUiGfxCommandDest *matrix;
    /* 0x38 */ void *image3A;
    /* 0x3C */ void *palette3A;
    /* 0x40 */ void *image3B;
    /* 0x44 */ void *palette3B;
    /* 0x48 */ u8 matrixDirty;
} RaceUiOverlayActor;

typedef struct RaceUiOrbitingSpriteActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ RaceUiGfxCommandDest *matrix;
    /* 0x28 */ s16 angle;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ void *palette;
    /* 0x30 */ void *image;
    /* 0x34 */ u8 matrixDirty;
    /* 0x35 */ u8 playerIndex;
} RaceUiOrbitingSpriteActor;

typedef struct RaceUiSparkleActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ RaceUiGfxCommandDest *matrix;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ void *palettes[4];
    /* 0x3C */ void *images[4];
    /* 0x4C */ s32 zOffset;
    /* 0x50 */ s16 alpha;
    /* 0x52 */ s16 alphaStep;
    /* 0x54 */ s16 timer;
    /* 0x56 */ s16 scale;
    /* 0x58 */ u8 matrixDirty;
    /* 0x59 */ u8 playerIndex;
    /* 0x5A */ u8 frame;
} RaceUiSparkleActor;

typedef struct {
    /* 0x00 */ u8 pad30[4];
    /* 0x04 */ Vec3i vec;
    /* 0x10 */ FixedMatrix3s matrix;
    /* 0x22 */ u8 pad52[0xC];
} RaceUiSparkleTransformScratch;

typedef struct {
    /* 0x00 */ u8 pad30[4];
    /* 0x04 */ Vec3i vec;
    /* 0x10 */ FixedMatrix3s matrix;
    /* 0x22 */ u8 pad52[4];
} RaceUiSparkleRetargetScratch;

typedef struct RaceUiCourseSpriteActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    /* 0x18 */ s32 x;
    /* 0x1C */ s32 y;
    /* 0x20 */ s32 z;
    /* 0x24 */ u8 pad24[0x48 - 0x24];
    /* 0x48 */ Vtx *vertices;
    /* 0x4C */ s16 angle;
    /* 0x4E */ u8 pad4E[0x50 - 0x4E];
    /* 0x50 */ void *images[2];
    /* 0x58 */ void *palettes[2];
    /* 0x60 */ u8 pad60[0x62 - 0x60];
    /* 0x62 */ u8 matrixDirty;
} RaceUiCourseSpriteActor;

extern const char gRaceUiPendingTrickPrizeLabel[];
extern const char gRaceUiPendingMakeBonusLabel[];
extern const char gRaceUiPendingCompleteBonusLabel[];
extern const char gRaceUiPendingMoneyLabel[];
extern Gfx *gRegionAllocPtr;
extern Gfx D_800D60A0[];
extern Gfx D_800D6120[];
extern Gfx D_800D6120[];
extern Gfx D_800D6160[];
extern Gfx D_800D6190[];
extern Gfx D_800D6968[];
extern Gfx gEffectRenderModeCleanupDl[];
extern Gfx gEffectRenderModeSetupDl[];
extern Gfx gTranslucentSpriteRenderModeDl[];
extern int sprintf(char *, const char *, ...);
extern RaceUiAssetEntry D_800D5FF4[];
extern RaceUiCameraTransformSource D_801121E0[];
extern RaceUiGfxCommandDest *allocFixedTransformMatrix(RaceUiTrailCopyBlock *);
extern RaceUiGfxCommandScriptEntry *D_800D693C[];
extern RaceUiProjectileVertexBlock D_800D64A0[];
extern RaceUiRankTextRenderEntry *D_800D761C[];
extern RaceUiSpriteInit D_800D5FF0[];
extern s16 D_800D633C[];
extern s16 gFrameCounter;
extern s16 gRaceLapCount;
extern s16 gRacePrizeAmountsByCourseAndRank[][4];
extern s16 gUiBlinkTimer;
extern s32 gMenuFlowState;
extern s32 isPositionNearAnyRaceViewportFocus(Vec3i *);
extern s8 gRacePlayerCount;
extern u16 D_800D6520[];
extern u32 *D_800D6400[];

extern u32 D_800D6230[];
extern u32 D_800D63D0[];
extern u32 D_800D69A8[];
extern Vec3i D_800D6030[];
extern Vec3i D_800D6110;
extern Vec3i D_800D61C0[];
extern Vec3i D_800D6220[];
extern Vec3i D_800D62A0;
extern Vec3i D_800D6324;
extern Vec3i D_800D6330[];
extern Vec3i D_800D6340[];
extern Vtx D_800D6350[];
extern u32 gAlphaSpriteRenderModeDl[];
extern u32 gSnowboardTrailBackDisplayList[];
extern u32 gSnowboardTrailFrontDisplayList[];
extern u32 gViewportMatrix;
extern u8 gCurrentViewportIndex;
extern u8 gRaceChallengeFailed;
extern u8 gRaceUpdatePaused;
extern Vec3i gIceCourseBumperPositions[];

extern void drawAssetTableSprite8bpp(s16, s16, void *, s32);
extern void drawMenuAsciiTextDefaultScale(s32, s32, const void *, s32);
extern void enqueuePlayerLoopingPositionalSoundRequest(s32, void *, s32, s32, f32, s32);
extern void enqueuePositionalSoundEffect(s32, void *, s32, s32);
extern void enqueueSoundEffect(s32, s32);
extern void osWritebackDCache(void *, s32);
extern void *allocMenuRenderScratch(s32 size);
extern void packFixedTransformMatrix(void *, void *);
extern void setPackedMatrixTranslation(RaceUiGfxCommandDest *, Vec3i *);

const char gRaceUiBoardReversePromptLabelBlinkOn[0x10] = "Board Reverse";
const char gRaceUiBoardReversePromptLabelBlinkOff[0x10] = "Board Reverse";
const char gRaceUiTrickScorePopupPointsFormat[0x4] = "%2d";
const char gRaceUiTrickScorePopupBonusFormat[0xC] = "%3d";
const char gRaceUiRankPrizeCounterFormat[0x4] = "%5d";
const char gRaceUiCoinCounterFormat[0x4] = "%5d";

void renderRaceCourseSlideSprite(RaceUiSlideActor *arg0) {
    volatile u8 pad[8];
    Gfx *gfx;

    gDPPipeSync(gRegionAllocPtr++);
    gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0x8)));
    gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0x9)));
    gSPMatrix(gRegionAllocPtr++, &gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->image, G_IM_FMT_CI, 0x20, 0x40, 0, G_TX_WRAP, G_TX_WRAP,
                           5, 6, G_TX_NOLOD, G_TX_NOLOD);

    gfx = gRegionAllocPtr++;
    gfx->words.w0 = ((arg0->angle * 4) & 0xFFF) | 0xF2000000;
    gfx->words.w1 = ((arg0->angle + 0x40) << 2) & 0xFFF;

    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette);
    gSPDisplayList(gRegionAllocPtr++, arg0->displayList);
}

void updateRaceCourseSlideSprite(RaceUiSlideActor *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->soundIndex;
    arg0->angle = arg0->angle + 2;
    arg0->angle = arg0->angle & 0x3F;
    if (temp_v0 != 0) {
        enqueuePlayerLoopingPositionalSoundRequest(0xE, &D_800D6030[temp_v0], 0x7F, 0x32, 0.0f, 0xA);
    }
    if (arg0->velocity != 0) {
        addRenderCallback(&D_801248EC, (RenderCallback)renderRaceCourseSlideSprite, arg0);
        return;
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderRaceCourseSlideSprite, arg0);
}

void initRaceCourseSlideSprite(RaceUiSlideActor *arg0) {
    arg0->angle = 0;
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)), D_800D5FF4[arg0->index].assetId, &arg0->image, &arg0->palette);
    arg0->displayList = (Gfx *) D_800D5FF0[arg0->index].word;
    arg0->velocity = D_800D5FF0[arg0->index].b6;
    arg0->soundIndex = D_800D5FF0[arg0->index].b7;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceCourseSlideSprite);
}

void drawRaceUiBoardReversePrompt(RaceUiPromptActor *arg0) {
    if (gCurrentViewportIndex == arg0->index) {
        if (gUiBlinkTimer & 1) {
            drawMenuAsciiTextDefaultScale(-0x34, arg0->y, &gRaceUiBoardReversePromptLabelBlinkOn, 0);
            return;
        }
        drawMenuAsciiTextDefaultScale(-0x34, arg0->y, &gRaceUiBoardReversePromptLabelBlinkOff, 1);
    }
}

void updateRaceUiBoardReversePrompt(RaceUiPromptActor *arg0) {
    if (gRacePlayers[arg0->index].stateFlags & 0x400) {
        if (arg0->timer < 0x1E) {
            arg0->timer++;
        }
    } else {
        arg0->timer = 0;
    }
    if (gRacePlayers[arg0->index].stateFlags & 0x1040) {
        arg0->timer = 0;
    }
    if (arg0->timer >= 0x1E) {
        addRenderCallback(&gRaceForegroundRenderCallbackList, (RenderCallback)drawRaceUiBoardReversePrompt, (void *)arg0);
    }
}


void initRaceUiBoardReversePrompt(RaceUiPromptActor *arg0) {
    arg0->y = -0x10;
    if (gPlayerCount != 1) {
        arg0->y = -8;
    }
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiBoardReversePrompt);
}

void drawRaceUiTrickScorePopup(RaceUiPopupActor *arg0) {
    volatile u8 padding[0x20];
    char buffer[8];
    s32 i;

    if (gCurrentViewportIndex == 0) {
        drawAssetTableSprite((s16)(arg0->x - 0x3C), arg0->y.half.lo, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x8B);
        drawAssetTableSprite((s16)(arg0->x + 0x14), arg0->y.half.lo, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x8E);
        sprintf(buffer, gRaceUiTrickScorePopupPointsFormat, arg0->parent);
        for (i = 0; i < 2; i++) {
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWithExplicitPalette((s16)(arg0->x + (i * 8) + 4), arg0->y.half.lo,
                              getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                              buffer[i] - 5, 0xE);
            }
        }

        if (arg0->playerIndex != 0) {
            drawAssetTableSprite((s16)(arg0->x - 0x48), (s16)(arg0->y.word + 0x12),
                          getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x8D);
            drawAssetTableSprite((s16)(arg0->x + 0x20), (s16)(arg0->y.word + 0x12),
                          getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x8E);
            sprintf(buffer, gRaceUiTrickScorePopupBonusFormat, 0x12C);
            for (i = 0; i != 3; i++) {
                if (buffer[i] != ' ') {
                    drawAssetTableSpriteWithExplicitPalette((s16)(arg0->x + (i * 8) + 8), (s16)(arg0->y.word + 0x12),
                                  getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                                  buffer[i] - 5, 0xE);
                }
            }
        }
    }
}

void updateRaceUiTrickScorePopupSlideOut(RaceUiPopupActor *arg0) {
    arg0->x += arg0->velocity;
    arg0->velocity += 4;
    if (arg0->velocity == 0x38) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceUiTrickScorePopup, arg0);
    }
}

void updateRaceUiTrickScorePopupHold(RaceUiPopupActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickScorePopupSlideOut);
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceUiTrickScorePopup, arg0);
}

void updateRaceUiTrickScorePopupSlideIn(RaceUiPopupActor *arg0) {
    arg0->x -= arg0->velocity;
    arg0->velocity -= 4;
    if (arg0->velocity == 0) {
        arg0->timer = 0x2D;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickScorePopupHold);
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceUiTrickScorePopup, arg0);
}

void initRaceUiTrickScorePopup(RaceUiPopupActor *arg0) {
    arg0->y.word = -0x2C;
    arg0->velocity = 0x38;
    arg0->x = 0x1A4;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickScorePopupSlideIn);
}

void spawnRaceUiTrickScorePopup(void *arg0, s16 arg1) {
    RaceUiPopupActor *temp = createCallbackTask((CallbackTaskCallback)initRaceUiTrickScorePopup, 0, 0x64);
    if (temp != NULL) {
        temp->parent = arg0;
        temp->playerIndex = arg1;
    }
}

void drawRaceUiCrashScorePopup(RaceUiPopupActor *arg0) {
    if (gCurrentViewportIndex == 0) {
        drawAssetTableSprite(-0x3C, arg0->y.half.lo, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), 0x8F);
    }
}

void updateRaceUiCrashScorePopupSlideOut(RaceUiPopupActor *arg0) {
    arg0->x += arg0->velocity;
    arg0->velocity += 4;
    if (arg0->velocity == 0x38) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceUiCrashScorePopup, arg0);
    }
}

void updateRaceUiCrashScorePopupHold(RaceUiPopupActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiCrashScorePopupSlideOut);
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceUiCrashScorePopup, arg0);
}

void updateRaceUiCrashScorePopupSlideIn(RaceUiPopupActor *arg0) {
    arg0->x -= arg0->velocity;
    arg0->velocity -= 4;
    if (arg0->velocity == 0) {
        arg0->timer = 0x2D;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiCrashScorePopupHold);
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceUiCrashScorePopup, arg0);
}

void initRaceUiCrashScorePopup(RaceUiPopupActor *arg0) {
    arg0->y.word = 0xC;
    arg0->velocity = 0x38;
    arg0->x = 0x1A4;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiCrashScorePopupSlideIn);
}

void func_80057E10(void *arg0) {
    if (gUiBlinkTimer & 8) {
        drawAssetTableSprite(0x20, 0x4A, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), 0x5B);
    }
}

void func_80057E60(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, func_80057E10, arg0);
}

void func_80057E90(RaceUiAlpha18Actor *arg0) {
    if (gRacePlayers[0].rankIndex != 0) {
        if (arg0->alpha != 0xFF) {
            gDPPipeSync(gRegionAllocPtr++);
            gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
            gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
            {
                Gfx *gfx = gRegionAllocPtr++;

                gfx->words.w0 = 0xFA000000;
                gfx->words.w1 = (arg0->alpha & 0xFF) | ~0xFF;
            }
        }
        drawAssetTableSprite(-0x68, -0x2C, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), 0x4D);
        drawAssetTableSprite(-0x42, -0xC, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)),
                             gRacePlayers[0].rankIndex);
        drawAssetTableSprite(-0x22, 4, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)),
                             gRacePlayers[0].rankIndex + 0x48);
        drawAssetTableSprite(-0x68, 6, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)),
                             gRacePlayers[0].characterId + 0x91);
        if (arg0->alpha != 0xFF) {
            gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
        }
    }
}

void func_8005804C(RaceUiAlpha18Actor *arg0) {
    volatile u8 padding[0x18];
    char buffer[8];
    s32 x;
    u8 *ptr;
    volatile s16 *handles;
    s32 space;

    sprintf(buffer + -8, gRaceUiRankPrizeCounterFormat, arg0->score);
    x = 0x20;
    handles = gAssetHandles;
 ptr = (u8 *) (((s32) padding) - 0x10); if (!ptr) { } space = ' '; loop: if ((*ptr) != 0) {
        if (space != *ptr) {
            drawAssetTableSprite(x, -0x3F, getRelocatableHeapBlockBase(handles[0x1F]),
                                 *ptr - 5);
        }
        x += 8;
        ptr++;
        goto loop;
    }

    drawAssetTableSprite(x, -0x3F, getRelocatableHeapBlockBase(handles[0x1F]), 0x37);
}

void func_8005812C(void *arg0)
{
    s32 x;
    s32 i;
    s32 space;
    s32 colorValue;
    u16 color;
    s16 *assets;
    char buffer[0x14];

    i = 0;
    do
    {
        sprintf(&buffer[i], gRaceUiCoinCounterFormat, gRacePlayers[0].unk568);
        x = 0x20;
        if (gRacePlayers[0].rankIndex == 3)
        {
 colorValue = 0xD; } else { colorValue = 0xC; } do { assets = &gAssetHandles[i]; space = ' '; color = colorValue; if (space != 0U) { } loop: if (buffer[i] != '\0') { if (buffer[i] != space) { drawAssetTableSpriteWithExplicitPalette((s16) x, -0xF, getRelocatableHeapBlockBase(assets[0x1F]), buffer[i] - 5, color); }
            x += 8;
            i++;
            goto loop;
        }
        } while (0);
        drawAssetTableSpriteWithExplicitPalette((s16) x, -0xF, getRelocatableHeapBlockBase(assets[0x1F]), 0x37, color);
    }
    while (0);
    if (gRacePlayers[0].rankIndex == 3)
    {
        drawAssetTableSprite(0x18, -0xB, getRelocatableHeapBlockBase(assets[0x1F]), 0x4E);
    }
}

const char gRaceUiTotalMoneyCounterFormat[0x4] = "%6d";
const char gRaceUiRankPrizeLabel[0x10] = "-Rank Prize-";
const char gRaceUiGetCoinLabel[0x10] = "  -Get Coin-";
const char gRaceUiTotalMoneyLabel[0x10] = "-Total Money-";

void func_8005827C(void *arg0) {
    volatile u8 padding[0x18];
    char buffer[8];
    s32 x;
    u8 *ptr;
    volatile s16 *handles;
    s32 space;

    sprintf(buffer + -8, gRaceUiTotalMoneyCounterFormat, gRacePlayers[0].money);
    x = 0x18;
    handles = gAssetHandles;
 ptr = (u8 *) (((s32) padding) - 0x10); if (!ptr) { } space = ' '; loop: if ((*ptr) != 0) {
        if (space != *ptr) {
            drawAssetTableSprite(x, 0x21, getRelocatableHeapBlockBase(handles[0x1F]),
                                 *ptr - 5);
        }
        x += 8;
        ptr++;
        goto loop;
    }

    drawAssetTableSprite(x, 0x21, getRelocatableHeapBlockBase(handles[0x1F]), 0x37);
}

void func_80058360(RaceUiAlpha18Actor *arg0) {
    if (arg0->alpha != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->alpha);
    }
    drawMenuAsciiTextDefaultScale(8, -0x48, &gRaceUiRankPrizeLabel, 5);
    drawMenuAsciiTextDefaultScale(0, -0x18, &gRaceUiGetCoinLabel, 5);
    drawMenuAsciiTextDefaultScale(4, 0x18, &gRaceUiTotalMoneyLabel, 5);
    if (arg0->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

void func_800584A0(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_80057E90, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005804C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005812C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005827C, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_80058360, arg0);
}

void func_80058538(void *arg0) {
    if (gPlayerInputPressed[0] & A_BUTTON) {
        enqueueSoundEffect(0x18, 0x32);
        gMenuFlowState |= 0x10;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_800584A0);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_80057E90, (void *)arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005804C, (void *)arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005812C, (void *)arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005827C, (void *)arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_80058360, (void *)arg0);
}

void func_80058610(RaceUiAlpha18Actor *arg0) {
    s32 unused;
    s32 shouldPlaySound;
    s32 value;
    RacePlayer *player;

    shouldPlaySound = 0;
    value = arg0->score;
    if (value != 0) {
        shouldPlaySound = 1;
    }
    player = (RacePlayer *) (s32) &gRacePlayers[0];
    if (value >= 9) {
        player->money += 8;
        arg0->score -= 8;
    } else {
        gRacePlayers[0].money = player->money + value;
        arg0->score = 0;
    }

    if (player->rankIndex != 3) {
        value = player->unk568;
        if (value != 0) {
            shouldPlaySound = 1;
        }
        if (value >= 9) {
            player->money += 8;
            player->unk568 = value - 8;
        } else {
            player->unk568 = 0;
            player->money += value;
        }
    }

    if (gPlayerInputPressed[0] & A_BUTTON) {
        gRacePlayers[0].money = player->money + arg0->score;
        if (player->rankIndex != 3) {
            player->money += player->unk568;
            player->unk568 = 0;
        }
        arg0->score = 0;
    }

    if (player->money >= 1000000) {
        player->money = 999999;
    }

    if (player->rankIndex != 3) {
        if ((arg0->score == 0) && (player->unk568 == 0)) {
            enqueueSoundEffect(2, 0x32);
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_80058538);
        }
    } else if (arg0->score == 0) {
        shouldPlaySound = 0;
        enqueueSoundEffect(2, 0x32);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_80058538);
    }

    if (shouldPlaySound != 0) {
        s16 timer = arg0->timer;
        if (timer == 0) {
            arg0->timer = 2;
            enqueueSoundEffect(0x1A, 0x32);
        } else {
            arg0->timer = timer - 1;
        }
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_80057E90, (void *)arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005804C, (void *)arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005812C, (void *)arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005827C, (void *)arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_80058360, (void *)arg0);
}

void updateRaceUiPrizePayoutWaitBeforeMoneyCount(RaceUiPrizePayoutActor *actor) {
    actor->timer = actor->timer - 1;
    if (actor->timer == 0) {
        actor->timer = 0;
        setCallbackTaskCallback(actor, (CallbackTaskCallback)func_80058610);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_80057E90, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005804C, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005812C, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005827C, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_80058360, actor);
}

void updateRaceUiPrizePayoutRevealGetCoin(RaceUiPrizePayoutActor *actor) {
    actor->timer--;
    if (actor->timer == 0) {
        actor->timer = RACE_UI_PRIZE_PAYOUT_REVEAL_TIMER;
        enqueueSoundEffect(0x1A, 0x32);
        setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiPrizePayoutWaitBeforeMoneyCount);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_80057E90, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005804C, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005812C, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_80058360, actor);
}

void updateRaceUiPrizePayoutRevealRankPrize(RaceUiPrizePayoutActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        arg0->timer = RACE_UI_PRIZE_PAYOUT_REVEAL_TIMER;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiPrizePayoutRevealGetCoin);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_80057E90, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005804C, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_80058360, arg0);
}

void updateRaceUiPrizePayoutShowRankPrize(RaceUiPrizePayoutActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        arg0->timer = RACE_UI_PRIZE_PAYOUT_REVEAL_TIMER;
        enqueueSoundEffect(0x1A, 0x32);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiPrizePayoutRevealRankPrize);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_80057E90, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_80058360, arg0);
}

void updateRaceUiPrizePayoutFadeIn(RaceUiPrizePayoutActor *arg0) {
    arg0->alpha += 0x10;
    if (arg0->alpha >= 0x100) {
        arg0->timer = RACE_UI_PRIZE_PAYOUT_REVEAL_TIMER;
        arg0->alpha = 0xFF;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiPrizePayoutShowRankPrize);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_80057E90, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_80058360, arg0);
}

void initRaceUiPrizePayout(RaceUiPrizePayoutActor *arg0) {
    arg0->alpha = 0;
    arg0->prizeAmount = gRacePrizeAmountsByCourseAndRank[0][gRacePlayers[0].rankIndex + gRaceCourseIndex.signedValue * 4];
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiPrizePayoutFadeIn);
}

void func_80058C00(RaceUiResultsBannerActor *arg0) {
    if (arg0->alpha != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->alpha);
    }

    if (arg0->player0 != -1) {
        drawAssetTableSprite(-0x88, -0x50, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0);
        drawAssetTableSprite(-0x68, -0x40, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x48);
        drawAssetTableSprite8bpp(-0x54, -0x50, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                      (gRacePlayers[arg0->player0].characterId + 0x41) & 0xFFFF);
        drawAssetTableSprite(-0x30, -0x40, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x4C);
    }

    if (arg0->player1 != -1) {
        drawAssetTableSprite(8, -0x30, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 1);
        drawAssetTableSprite(0x28, -0x20, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x49);
        drawAssetTableSprite8bpp(0x3C, -0x30, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                      (gRacePlayers[arg0->player1].characterId + 0x41) & 0xFFFF);
        drawAssetTableSprite(0x60, -0x20, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x4C);
    }

    if (arg0->player2 != -1) {
        drawAssetTableSprite(-0x88, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 2);
        drawAssetTableSprite(-0x68, 0, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x4A);
        drawAssetTableSprite8bpp(-0x54, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                      (gRacePlayers[arg0->player2].characterId + 0x41) & 0xFFFF);
        drawAssetTableSprite(-0x30, 0, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x4C);
    }

    if (arg0->player3 != -1) {
        drawAssetTableSprite(8, 0x10, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 3);
        drawAssetTableSprite(0x28, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x4B);
        drawAssetTableSprite8bpp(0x3C, 0x10, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                      (gRacePlayers[arg0->player3].characterId + 0x41) & 0xFFFF);
        drawAssetTableSprite(0x60, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x4C);
    }

    if (arg0->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

const char D_800E128C[4] = "*%d";
const char D_800E1290[4] = "%dP";
const char D_800E1294[4] = "COM";
const char D_800E1298[4] = "*%d";
const char D_800E129C[4] = "%dP";
const char D_800E12A0[4] = "COM";
const char D_800E12A4[4] = "*%d";
const char D_800E12A8[4] = "%dP";
const char D_800E12AC[4] = "COM";
const char D_800E12B0[4] = "*%d";
const char D_800E12B4[4] = "%dP";
const char D_800E12B8[4] = "COM";

void func_8005905C(RaceUiResultsBannerActor *arg0) {
    RaceUiResultsBannerActor *actor;
    char text[0x64];

    actor = arg0;
    if (actor->alpha != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, actor->alpha);
    }

    if (actor->player0 != -1) {
        sprintf(text, D_800E128C, gRacePlayers[actor->player0].unk18);
        if (gUiBlinkTimer & 1) {
            drawMenuAsciiTextDefaultScale(-0x20, -0x38, text, 6);
        } else {
            drawMenuAsciiTextDefaultScale(-0x20, -0x38, text, 0);
        }
        if (gRacePlayers[actor->player0].isCpu == 0) {
            sprintf(text, D_800E1290, gRacePlayers[actor->player0].playerIndexU + 1);
            drawMenuAsciiTextDefaultScale(-0x30, -0x48, text, 5);
        } else {
            drawMenuAsciiTextDefaultScale(-0x30, -0x48, D_800E1294, 4);
        }
    }

    if (actor->player1 != -1) {
        sprintf(text, D_800E1298, gRacePlayers[actor->player1].unk18);
        drawMenuAsciiTextDefaultScale(0x70, -0x18, text, 6);
        if (gRacePlayers[actor->player1].isCpu == 0) {
            sprintf(text, D_800E129C, gRacePlayers[actor->player1].playerIndexU + 1);
            drawMenuAsciiTextDefaultScale(0x60, -0x28, text, 5);
        } else {
            drawMenuAsciiTextDefaultScale(0x60, -0x28, D_800E12A0, 4);
        }
    }

    if (actor->player2 != -1) {
        sprintf(text, D_800E12A4, gRacePlayers[actor->player2].unk18);
        drawMenuAsciiTextDefaultScale(-0x20, 8, text, 6);
        if (gRacePlayers[actor->player2].isCpu == 0) {
            sprintf(text, D_800E12A8, gRacePlayers[actor->player2].playerIndexU + 1);
            drawMenuAsciiTextDefaultScale(-0x30, -8, text, 5);
        } else {
            drawMenuAsciiTextDefaultScale(-0x30, -8, D_800E12AC, 4);
        }
    }

    if (actor->player3 != -1) {
        sprintf(text, D_800E12B0, gRacePlayers[actor->player3].unk18);
        drawMenuAsciiTextDefaultScale(0x70, 0x28, text, 6);
        if (gRacePlayers[actor->player3].isCpu == 0) {
            sprintf(text, D_800E12B4, gRacePlayers[actor->player3].playerIndexU + 1);
            drawMenuAsciiTextDefaultScale(0x60, 0x18, text, 5);
        } else {
            drawMenuAsciiTextDefaultScale(0x60, 0x18, D_800E12B8, 4);
        }
    }

    if (actor->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

void func_80059518(RaceUiResultsBannerActor *arg0) {
    RaceUiResultsBannerActor *actor;
    s32 i;
    s32 j;
    s32 next;
    s32 temp;

    actor = arg0;
    i = 0;
    do {
        actor->playerIndices[i] = i;
        i++;
    } while (i < 4);

    i = 0; do { next = i + 1; j = next;
        if (next < 4) {
            do {
                if (gRacePlayers[actor->playerIndices[j]].rankIndex < gRacePlayers[actor->playerIndices[i]].rankIndex) {
                    temp = actor->playerIndices[i];
                    actor->playerIndices[i] = actor->playerIndices[j];
                    actor->playerIndices[j] = temp;
                }
                j++;
            } while (j < 4);
        }
        next = i + 1;
        i = next;
    } while (next < 3);

    i = 0;
    do {
        if (!(gRacePlayers[actor->playerIndices[i]].stateFlags & 0x40)) {
            actor->playerIndices[i] = -1;
        }
        i++;
    } while (i < 4);
}

void updateRaceUiResultsBannerConfirmed(RaceUiResultsBannerActor *actor) {
    func_80059518(actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_80058C00, (void *)actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005905C, (void *)actor);
}

void updateRaceUiResultsBannerWaitForInput(RaceUiResultsBannerActor *arg0) {
    s32 *input;
    s32 i;
    RaceUiResultsBannerActor *actor;

    actor = arg0;
    i = 0;
    if ((s32)gPlayerCount > 0) {
        input = gPlayerInputPressed; do {
            if ((*input & A_BUTTON) && !(gMenuFlowState & 0x10)) {
                gMenuFlowState |= 0x10;
                enqueueSoundEffect(0x18, 0x32);
                setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiResultsBannerConfirmed);
            }
            i++;
            input++;
        } while (i < (s32)gPlayerCount);
    }
    func_80059518(actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_80058C00, (void *)actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005905C, (void *)actor);
}

void updateRaceUiResultsBannerFadeIn(RaceUiResultsBannerActor *actor) {
    actor->alpha += 0x10;
    if (actor->alpha >= 0x100) {
        actor->timer = 0x96;
        actor->alpha = 0xFF;
        setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiResultsBannerWaitForInput);
    }
    func_80059518(actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_80058C00, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005905C, actor);
}

void initRaceUiResultsBanner(RaceUiResultsBannerActor *actor) {
    actor->alpha = 0;
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiResultsBannerFadeIn);
}

const char gRaceUiPackedTimeMinutesFormat[] = "%2.2d";
const char gRaceUiPackedTimeSecondsFormat[] = "%2.2d";
const char gRaceUiPackedTimeFractionFormat[] = "%2.2d";

void func_80059A04(RaceTimer *arg0, s32 x, s32 arg2, s32 arg3)
{
  char buffer[0x58];
  RaceTimer *record;
  s16 y;
  u16 color;
  s32 i;

  record = arg0;
  y = arg2;
  color = arg3;
  sprintf(&buffer[-0x10], gRaceUiPackedTimeMinutesFormat, record->minutes);
  i = -0x10;
  do
  {
    /* IDO register allocation for this function depends on this empty loop. */
    do { } while (0);
    drawAssetTableSpriteWithExplicitPaletteWideIndex((s16) x, y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), (((u8) buffer[i]) - 5) & 0xFFFF, color);
    i++;
    x += 8;
  }
  while (i < (-0xE));
  drawAssetTableSpriteWithExplicitPaletteWideIndex((s16) x, y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x36, color);
  x += 8;
  sprintf(&buffer[-0x10], gRaceUiPackedTimeSecondsFormat, record->seconds);
  i = -0x10;
  do
  {
    drawAssetTableSpriteWithExplicitPaletteWideIndex((s16) x, y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), (((u8) buffer[i]) - 5) & 0xFFFF, color);
    i++;
    x += 8;
  }
  while (i < (-0xE));
  drawAssetTableSpriteWithExplicitPaletteWideIndex((s16) x, y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x35, color);
  x += 8;
  sprintf(&buffer[-0x10], gRaceUiPackedTimeFractionFormat, record->fraction >> 8);
  i = -0x10;
  do
  {
    drawAssetTableSpriteWithExplicitPaletteWideIndex((s16) x, y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), (((u8) buffer[i]) - 5) & 0xFFFF, color);
    i++;
    x += 8;
  }
  while (i != (-0xE));
}

void func_80059C34(RaceUiCourseStatsActor *arg0) {
    s32 y;
    s32 row;
    s32 offset;
    RaceUiCourseStatsActor *actor;

    actor = arg0;
    y = -0x50;
    for (row = 0, offset = 0; row != 5; row++, offset += 4, y += 0x20) {
        drawAssetTableSprite(-8, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                             row + 0x77);
        if ((row == actor->index) && (gUiBlinkTimer & 1)) {
            func_80059A04(&((RaceUiCourseStatsNameData *)&gGameSaveDataBuffer)->courseStatsNames[gRaceCourseIndex.signedValue][row],
                          0x10, y, 0x10);
        } else if (row < 3) {
            func_80059A04(&((RaceUiCourseStatsNameData *)&gGameSaveDataBuffer)->courseStatsNames[gRaceCourseIndex.signedValue][row],
                          0x10, y, 0xC);
        } else {
            func_80059A04(&((RaceUiCourseStatsNameData *)&gGameSaveDataBuffer)->courseStatsNames[gRaceCourseIndex.signedValue][row],
                          0x10, y, 0xD);
        }

        drawAssetTableSprite(0x58, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                      ((((RaceUiCourseStatsIconData *)&gGameSaveDataBuffer)
                            ->courseStatsIcons[gRaceCourseIndex.signedValue][row] &
                        7) +
                       0x51));
        drawAssetTableSprite(0x6C, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                      (((*((RaceUiCourseStatsIconData *)&gGameSaveDataBuffer))
                             .courseStatsIcons[gRaceCourseIndex.signedValue][row] >>
                         3) +
                        0x7C));
    }
}

void func_80059E5C(RaceUiAlpha1AActor *arg0) {
    Gfx *gfx;
    s32 w0;
    s32 w1;

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->alpha & 0xFF) | (~0xFF); } func_80059A04(&gRaceElapsedTimer, -0x6C, -0x30, 0xC); if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) gMenuRenderModeResetDl; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; } } while (0);
}

const char gRaceUiLapTimeLabel[0xC] = "-Lap Time-";

void func_80059F6C(RaceUiAlpha18Actor *arg0) {
    Gfx *gfx;
    s32 w0;
    s32 w1;

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->alpha & 0xFF) | (~0xFF); } drawMenuAsciiTextDefaultScale(-0x74, -0x3C, gRaceUiLapTimeLabel, 5); if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) gMenuRenderModeResetDl; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; } } while (0);
}

void func_8005A07C(RaceUiCourseStatsActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_80059E5C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_80059C34, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_80059F6C, arg0);
}

void func_8005A0E0(RaceUiCourseStatsActor *arg0) {
    if (gPlayerInputPressed[0] & A_BUTTON) {
        gMenuFlowState |= 0x10;
        enqueueSoundEffect(0x18, 0x32);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005A07C);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_80059E5C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_80059C34, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_80059F6C, arg0);
}

void func_8005A184(RaceUiCourseStatsActor *arg0) {
    arg0->holdTimer--;
    if (arg0->holdTimer == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005A0E0);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_80059E5C, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_80059F6C, arg0);
}

void func_8005A1FC(RaceUiCourseStatsActor *arg0) {
    arg0->statsAlpha += 0x10;
    if (arg0->statsAlpha >= 0x100) {
        arg0->holdTimer = 0x1E;
        arg0->statsAlpha = 0xFF;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005A184);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_80059E5C, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_80059F6C, arg0);
}

void func_8005A288(RaceUiCourseStatsActor *arg0) {
    arg0->titleAlpha += 0x10;
    if (arg0->titleAlpha >= 0x100) {
        arg0->titleAlpha = 0xFF;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005A1FC);
    }
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_80059F6C, arg0);
}

void func_8005A2F0(RaceUiCourseStatsActor *arg0) {
    arg0->titleAlpha = 0;
    arg0->statsAlpha = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005A288);
}

const char gRaceUiScoreAttackPointTotalFormat[4] = "%2d";

void func_8005A31C(RaceUiAlpha18Actor *arg0) {
    volatile s32 x;
    char buffer[0x20];
    RaceUiAlpha18Actor *actor;

    actor = arg0;
    if (actor->alpha != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, actor->alpha);
    }

    drawAssetTableSprite(-0x6C, -0x24, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), 0x20);
    sprintf(buffer + -8, gRaceUiScoreAttackPointTotalFormat, gRacePlayers[0].scoreAttackPointTotal);

    if (buffer[-8] != ' ') {
        drawAssetTableSprite(-0x4C, -0x1C, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), buffer[-8] - 5);
    }

    x = -0x44;
    drawAssetTableSprite(((SplitWord *)&x)->half.lo, -0x1C, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), buffer[-7] - 5);

    if (actor->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

const char gRaceUiCounterHitPrizeFormat[0x4] = "%5d";
const char gRaceUiCounterPerfectHitBonusFormat[0x4] = "%5d";
const char gRaceUiCounterCompleteBonusFormat[0x4] = "%5d";
const char gRaceUiCounterMoneyFormat[0x4] = "%6d";

void func_8005A4BC(RaceUiCounterActor *arg0)
{
    volatile u8 pad[4];
    SplitWord y;
    s32 i;
    s32 x;
    char buffer[0x20];
    s16 rowY;
    s16 visibleRows;

    y.word = -0x47;
    if (arg0->flag == 0) {
        y.word = -0x33;
    }
    visibleRows = arg0->state;
    if (visibleRows >= 0) {
        x = 0x20;
        sprintf(buffer, gRaceUiCounterHitPrizeFormat, arg0->value, arg0);
        i = 0;
        rowY = y.half.lo;
loop1:
        if (buffer[i] != 0) {
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                                     ((u8)buffer[i] - 5) & 0xFFFF);
            }
            x += 8;
            i++;
            goto loop1;
        }
        drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
        y.word += 0x28;
        visibleRows = arg0->state;
    }

    if (visibleRows > 0) {
        if (arg0->flag != 0) {
            x = 0x20;
            sprintf(buffer, gRaceUiCounterPerfectHitBonusFormat, arg0->bonus, arg0); i = 0;
            rowY = y.half.lo;
            i = 0;
loop2:
            if (buffer[i] != 0) {
                if (buffer[i] != ' ') {
                    drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                                         ((u8)buffer[i] - 5) & 0xFFFF);
                }
                x += 8;
                i++;
                goto loop2;
            }
            drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
            y.word += 0x28;
            visibleRows = arg0->state;
        }
    }

    if (visibleRows >= 2) {
        x = 0x20;
        sprintf(buffer, gRaceUiCounterCompleteBonusFormat, arg0->target, arg0); i = 0;
        rowY = y.half.lo;
        i = 0;
loop3:
        if (buffer[i] != 0) {
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                                     ((u8)buffer[i] - 5) & 0xFFFF);
            }
            x += 8;
            i++;
            goto loop3;
        }
        drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
        y.word += 0x28;
        visibleRows = arg0->state;
    }

    if (arg0) { }
    if (visibleRows >= 3) {
        x = 0x18;
        sprintf(buffer, gRaceUiCounterMoneyFormat, gRacePlayers[0].money); i = 0;
        rowY = y.half.lo;
        i = 0;
loop4:
        if (buffer[i] != 0) {
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                                     ((u8)buffer[i] - 5) & 0xFFFF);
            }
            x += 8;
            i++;
            goto loop4;
        }
        drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
    }
}

const char gRaceUiCourseValueFormat[4] = "%2d";

void func_8005A884(RaceUiPopupActor *arg0) {
    volatile u8 padding[0x18];
    char new_var;
    s32 i;
    s32 color;
    s32 y;
    s32 space;

    y = -0x50;
    /* IDO scheduling for this function depends on these statements staying on one line. */
    i = 0; space = ' '; do { char buffer[8];

        drawAssetTableSprite(-8, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), i + 0x77);

        if ((i == arg0->index) && (gUiBlinkTimer & 1)) {
            color = 0x10;
        } else if (i < 3) {
            color = 0xC;
        } else {
            color = 0xD;
        }

        sprintf(buffer, gRaceUiCourseValueFormat, ((RaceUiCourseValueData *)&gGameSaveDataBuffer)->values[gRaceCourseIndex.signedValue][i]);

        if ((u8)buffer[0] != space) {
            drawAssetTableSpriteWithExplicitPalette(0x10, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                          buffer[0] - 5, color);
            do { } while (0);
        }

        if ((u8)buffer[1] != space) {
            drawAssetTableSpriteWithExplicitPalette(0x18, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                          buffer[1] - 5, (u16)color);
        }

        drawAssetTableSprite(0x46, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                      (((RaceUiCourseValueData *)&gGameSaveDataBuffer)->icons[gRaceCourseIndex.signedValue][i] & 7) + 0x51);
        drawAssetTableSprite(0x58, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                      (((RaceUiCourseValueData *)&gGameSaveDataBuffer)->icons[gRaceCourseIndex.signedValue][i] >> 3) + 0x7C);

        i++;
        y += 0x20;
    } while (i != 5);
}

const char gRaceUiScoreAttackTargetFormat[0x4] = "/%d";
const char gRaceUiScoreAttackAsteriskLabel[0x4] = "*";
const char gRaceUiScoreAttackTimeOutLabel[0xC] = "Time Out";

void func_8005AAE4(RaceUiAlpha18Actor *arg0) {
    Gfx *gfx;
    s32 w0;
    s32 w1;
    char unused[8];
    char sp20[0xC];

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; if ((gRegionAllocPtr && gRegionAllocPtr) && gRegionAllocPtr) { } w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->alpha & 0xFF) | (~0xFF); }
    sprintf(&sp20[0x14], gRaceUiScoreAttackTargetFormat, gRacePlayers[0].scoreAttackPointTarget); drawMenuAsciiTextDefaultScale(-0x54, -0x14, gRaceUiScoreAttackAsteriskLabel, 6); drawMenuAsciiTextDefaultScale(-0x3C, -0x14, &sp20[0x14], 6); if (gRaceChallengeFailed != 0) { drawMenuAsciiTextDefaultScale(-0x68, -4, gRaceUiScoreAttackTimeOutLabel, 4); } if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) gMenuRenderModeResetDl; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; } } while (0);
}

const char gRaceUiHitPrizeX1Label[0x14] = "-Hit Prize(*1G)-";
const char gRaceUiHitPrizeX50Label[0x14] = "-Hit Prize(*50G)-";
const char gRaceUiPerfectHitLabel[0x10] = "  -Perfect Hit-";
const char gRaceUiCompleteBonusLabel[0x14] = "-Complete Bonus-";
const char gRaceUiHitPrizeTotalMoneyLabel[0x10] = "-Total Money-";

void func_8005AC44(RaceUiCounterActor *arg0) {
    s32 y;

    if (arg0->alpha != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->alpha);
    }

    y = -0x50;
    if (arg0->flag == 0) {
        y = -0x3C;
    }

    if (gRaceChallengeFailed != 0) {
        drawMenuAsciiTextDefaultScale(-8, (s16)y, &gRaceUiHitPrizeX1Label, 5);
    } else {
        drawMenuAsciiTextDefaultScale(-0xC, (s16)y, &gRaceUiHitPrizeX50Label, 5);
    }

    y += 0x28;
    if (arg0->flag != 0) {
        drawMenuAsciiTextDefaultScale(-0xC, (s16)y, &gRaceUiPerfectHitLabel, 5);
        y += 0x28;
    }

    drawMenuAsciiTextDefaultScale(-8, (s16)y, &gRaceUiCompleteBonusLabel, 5);
    y += 0x28;
    drawMenuAsciiTextDefaultScale(4, (s16)y, &gRaceUiHitPrizeTotalMoneyLabel, 5);

    if (arg0->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

const char gRaceUiCourseValueHitLabelRow1[0x4] = "Hit";
const char gRaceUiCourseValueHitLabelRow2[0x4] = "Hit";
const char gRaceUiCourseValueHitLabelRow3[0x4] = "Hit";
const char gRaceUiCourseValueHitLabelRow4[0x4] = "Hit";
const char gRaceUiCourseValueHitLabelRow5[0x4] = "Hit";

void func_8005AE1C(void *arg0) {
    drawMenuAsciiTextDefaultScale(0x20, -0x48, &gRaceUiCourseValueHitLabelRow1, 6);
    drawMenuAsciiTextDefaultScale(0x20, -0x28, &gRaceUiCourseValueHitLabelRow2, 6);
    drawMenuAsciiTextDefaultScale(0x20, -8, &gRaceUiCourseValueHitLabelRow3, 6);
    drawMenuAsciiTextDefaultScale(0x20, 0x18, &gRaceUiCourseValueHitLabelRow4, 4);
    drawMenuAsciiTextDefaultScale(0x20, 0x38, &gRaceUiCourseValueHitLabelRow5, 4);
}

void func_8005AEB0(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A31C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A884, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AAE4, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AE1C, arg0);
}

void func_8005AF2C(void *arg0) {
    if (gPlayerInputPressed[0] & A_BUTTON) {
        enqueueSoundEffect(0x18, 0x32);
        gMenuFlowState |= 0x10;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005AEB0);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A31C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A884, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AAE4, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AE1C, arg0);
}

void func_8005AFEC(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A31C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A4BC, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AAE4, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AC44, arg0);
}

void func_8005B068(RaceUiCounterActor *actor) {
    if (gPlayerInputPressed[0] & A_BUTTON) {
        enqueueSoundEffect(0x18, 0x32);
        if (actor->index < 5) {
            setCallbackTaskCallback(actor, (CallbackTaskCallback)func_8005AF2C);
        } else {
            gMenuFlowState |= 0x10;
            setCallbackTaskCallback(actor, (CallbackTaskCallback)func_8005AFEC);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A31C, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A4BC, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AAE4, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AC44, actor);
}

void func_8005B14C(RaceUiCounterActor *arg0) {
    s16 value;

    if (arg0->timer == 0) {
        arg0->timer = 2;
        enqueueSoundEffect(0x1A, 0x32);
    } else {
        arg0->timer--;
    }

    value = arg0->value;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    /* IDO register allocation depends on this always-zero index. */
    gRacePlayers[gPlayerInputPressed[0] * 0].unkC += value;
    arg0->value -= value;

    value = arg0->bonus;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    gRacePlayers[0].unkC += value;
    arg0->bonus -= value;

    value = arg0->target;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    gRacePlayers[0].unkC += value;
    arg0->target -= value;

    if (gRacePlayers[0].unkC >= 0xF4240) {
        gRacePlayers[0].unkC = 0xF423F;
    }

    if ((arg0->value == 0) && (arg0->bonus == 0) && (arg0->target == 0)) {
        enqueueSoundEffect(2, 0x32);
        arg0->timer = 0x14;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005B068);
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A31C, (void *)arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A4BC, (void *)arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AAE4, (void *)arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AC44, (void *)arg0);
}

void updateRaceUiHitPrizeWaitBeforePayout(RaceUiCounterActor *arg0) {
    arg0->state = RACE_UI_HIT_PRIZE_SHOW_TOTAL_MONEY;
    arg0->timer--;
    if (arg0->timer == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005B14C);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A31C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A4BC, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AAE4, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AC44, arg0);
}

void updateRaceUiHitPrizeRevealTotalMoney(RaceUiCounterActor *arg0) {
    arg0->state = RACE_UI_HIT_PRIZE_SHOW_TOTAL_MONEY;
    arg0->timer--;
    if (arg0->timer == 0) {
        arg0->timer = 0xA;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiHitPrizeWaitBeforePayout);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A31C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A4BC, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AAE4, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AC44, arg0);
}

void func_8005B49C(RaceUiCounterActor *arg0) {
    arg0->state = RACE_UI_HIT_PRIZE_SHOW_COMPLETE_BONUS;
    arg0->timer--;
    if (arg0->timer == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        arg0->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiHitPrizeRevealTotalMoney);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A31C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A4BC, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AAE4, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AC44, arg0);
}

void updateRaceUiHitPrizeRevealPerfectHit(RaceUiCounterActor *arg0) {
    arg0->state = RACE_UI_HIT_PRIZE_SHOW_PERFECT_HIT;
    arg0->timer--;
    if (arg0->timer == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        arg0->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005B49C);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A31C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A4BC, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AAE4, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AC44, arg0);
}

void updateRaceUiHitPrizeHoldHitPrize(RaceUiCounterActor *arg0) {
    arg0->state = RACE_UI_HIT_PRIZE_SHOW_HIT_PRIZE;
    arg0->timer--;
    if (arg0->timer == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        arg0->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        if (arg0->hasPerfectHitBonus != 0) {
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiHitPrizeRevealPerfectHit);
        } else {
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005B49C);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A31C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A4BC, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AAE4, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AC44, arg0);
}

void updateRaceUiHitPrizeWaitBeforeHitPrize(RaceUiCounterActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        arg0->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        enqueueSoundEffect(0x1A, 0x32);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiHitPrizeHoldHitPrize);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A31C, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AAE4, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AC44, arg0);
}

void updateRaceUiHitPrizeFadeIn(RaceUiCounterActor *arg0) {
    arg0->alpha += RACE_UI_RESULTS_FADE_STEP;
    if (arg0->alpha >= 0x100) {
        arg0->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        arg0->alpha = RACE_UI_RESULTS_FULL_ALPHA;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiHitPrizeWaitBeforeHitPrize);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005A31C, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AAE4, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005AC44, arg0);
}

void initRaceUiHitPrizePayout(RaceUiCounterActor *arg0) {
    arg0->alpha = 0;
    arg0->hasPerfectHitBonus = FALSE;
    arg0->pendingPerfectHitBonus = 0;
    arg0->pendingCompleteBonus = RACE_UI_HIT_PRIZE_SCORE_RATE;
    if (gRaceCourseIndex.signedValue == 9) {
        arg0->pendingCompleteBonus = RACE_UI_HIT_PRIZE_QUICKSAND_VALLEY_SCORE_RATE;
    }
    if (gRaceChallengeFailed != 0) {
        arg0->pendingHitPrize = gRacePlayers[0].scoreAttackPointTotal;
    } else {
        arg0->pendingHitPrize = gRacePlayers[0].scoreAttackPointTotal * 0x32;
        if (gRacePlayers[0].scoreAttackPointTotal == gRacePlayers[0].scoreAttackPointTarget) {
            arg0->hasPerfectHitBonus = TRUE;
            arg0->pendingPerfectHitBonus = RACE_UI_HIT_PRIZE_PERFECT_HIT_BONUS;
        }
    }
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiHitPrizeFadeIn);
}

void func_8005B8E8(RaceUiAlphaActor *arg0) {
    Gfx *gfx;
    s32 w0;
    s32 w1;

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->alpha & 0xFF) | (~0xFF); } func_80059A04(&gRaceElapsedTimer, -0x68, -0x37, 0xC); if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) gMenuRenderModeResetDl; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; } } while (0);
}

const char gRaceUiTargetTimeChallengePrizeCounterFormat[] = "%5d";
const char gRaceUiTargetTimeChallengeSpeedFanGetCounterFormat[] = "%5d";
const char gRaceUiTargetTimeChallengePerfectGetCounterFormat[] = "%5d";
const char gRaceUiTargetTimeChallengeCompleteBonusCounterFormat[] = "%5d";
const char gRaceUiTargetTimeChallengeMoneyCounterFormat[] = "%6d";

void func_8005B9F8(RaceUiDualCounterActor *arg0) {
    volatile u8 pad[4];
    SplitWord y;
    s32 i;
    s32 x;
    char buffer[0x20];
    s16 tempY;

    y.word = -0x47;
    if (arg0->state >= 0) {
        x = 0x20;
        sprintf(buffer, gRaceUiTargetTimeChallengePrizeCounterFormat, arg0->leftValue);
        i = 0; while (1) {
            if (buffer[i] == 0) {
                break;
            }
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex((s16)x, -0x47, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                              (((u8)buffer[i]) - 5) & 0xFFFF);
            }
            x += 8;
            i++;
        }
        drawAssetTableSpriteWideIndex((s16)x, -0x47, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
        y.word = -0x27;
    }

    if (arg0->state > 0) {
        x = 0x20;
        sprintf(buffer, gRaceUiTargetTimeChallengeSpeedFanGetCounterFormat, arg0->rightValue); i = 0;
        tempY = y.half.lo;
        i = 0;
        while (1) {
            if (buffer[i] == 0) {
                break;
            }
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex((s16)x, tempY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                              (((u8)buffer[i]) - 5) & 0xFFFF);
            }
            x += 8;
            i++;
        }
        drawAssetTableSpriteWideIndex((s16)x, tempY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
        y.word += 0x20;
    }

    if (arg0->state >= 2) {
        x = 0x20;
        if (arg0->flag != 0) {
            sprintf(buffer, gRaceUiTargetTimeChallengePerfectGetCounterFormat, arg0->bonus); i = 0;
            tempY = y.half.lo;
            i = 0;
            while (1) {
                if (buffer[i] == 0) {
                    break;
                }
                if (buffer[i] != ' ') {
                    drawAssetTableSpriteWideIndex((s16)x, tempY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                                  (((u8)buffer[i]) - 5) & 0xFFFF);
                }
                x += 8;
                i++;
            }
            drawAssetTableSpriteWideIndex((s16)x, tempY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
            y.word += 0x20;
        }
    }

    if (arg0->state >= 3) {
        x = 0x20;
        sprintf(buffer, gRaceUiTargetTimeChallengeCompleteBonusCounterFormat, arg0->leftTarget); i = 0;
        tempY = y.half.lo;
        i = 0;
        while (1) {
            if (buffer[i] == 0) {
                break;
            }
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex((s16)x, tempY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                              (((u8)buffer[i]) - 5) & 0xFFFF);
            }
            x += 8;
            i++;
        }
        drawAssetTableSpriteWideIndex((s16)x, tempY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
        y.word += 0x20;
    }

    if (arg0->state >= 4) {
        x = 0x18;
        sprintf(buffer, gRaceUiTargetTimeChallengeMoneyCounterFormat, gRacePlayers[0].money); i = 0;
        tempY = y.half.lo;
        i = 0;
        while (1) {
            if (buffer[i] == 0) {
                break;
            }
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex((s16)x, tempY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                              (((u8)buffer[i]) - 5) & 0xFFFF);
            }
            x += 8;
            i++;
        }
        drawAssetTableSpriteWideIndex((s16)x, tempY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
    }
}

void func_8005BE68(RaceUiPopupActor *arg0) {
    s32 y;
    s32 color;
    s32 i;
    s32 offset;

    y = -0x50;
    /* IDO scheduling for this function depends on this initialization staying on one line. */
    i = 0; offset = 0; while (1) {
        drawAssetTableSprite(-8, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), i + 0x77);
        if ((i == arg0->index) && (gUiBlinkTimer & 1)) {
            color = 0x10;
        } else if (i < 3) {
            color = 0xC;
        } else {
            color = 0xD;
        }
        func_80059A04(&((RaceUiResultNameData *)&gGameSaveDataBuffer)->resultNames[gRaceCourseIndex.signedValue][i], 0x10, y, color);
        drawAssetTableSprite(0x58, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                      (((RaceUiResultIconData *)&gGameSaveDataBuffer)->resultIcons[gRaceCourseIndex.signedValue][i] & 7) + 0x51);
        drawAssetTableSprite(0x6C, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                      ((*((RaceUiResultIconData *)&gGameSaveDataBuffer)).resultIcons[gRaceCourseIndex.signedValue][i] >> 3) + 0x7C);
        i++;
        offset += 4;
        y += 0x20;
        if (i == 5) {
            break;
        }
    }
}

const char gRaceUiTargetTimeChallengeLapTimeLabel[0xC] = "-Lap Time-";

void func_8005C03C(RaceUiAlphaActor *arg0) {
    Gfx *gfx;
    s32 w0;
    s32 w1;

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->alpha & 0xFF) | (~0xFF); } drawMenuAsciiTextDefaultScale(-0x70, -0x40, gRaceUiTargetTimeChallengeLapTimeLabel, 5); if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) gMenuRenderModeResetDl; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; } } while (0);
}

const char gRaceUiTargetTimeChallengeTargetLabel[0xC] = "-Target-";
const char gRaceUiTargetTimeChallengeTargetTimeFormat[0x14] = "%2.2d'%2.2d\"%2.2d";
const char gRaceUiTargetTimeChallengeFastLabel[0x8] = " -Fast-";
const char gRaceUiTargetTimeChallengeDeltaTimeFormat[0x14] = "-%2.2d'%2.2d\"%2.2d";
const char gRaceUiTargetTimeChallengeTimeOutLabel[0xC] = "Time Out";
const char gRaceUiTargetTimeChallengePrizeLabel[0x10] = "  -Time Prize-";
const char gRaceUiTargetTimeChallengeSpeedFanGetLabel[0x14] = " -Speed Fan Get-";
const char gRaceUiTargetTimeChallengePerfectGetLabel[0x10] = "  -Perfect Get-";
const char gRaceUiTargetTimeChallengeCompleteBonusLabel[0x14] = "-Complete Bonus-";
const char gRaceUiTargetTimeChallengeMoneyLabel[0x10] = "  -Total Money-";

void func_8005C14C(RaceUiDualCounterActor *arg0) {
    char buf[0x18];
    char *bufp;
    s32 y;
    u32 textY;
    RaceTimer *targetTime;

    if (arg0->alpha != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->alpha);
    }

    if (gRaceChallengeFailed == 0) {
        drawMenuAsciiTextDefaultScale(-0x68, -0x1F, gRaceUiTargetTimeChallengeTargetLabel, 7);
        targetTime = &gRaceCourseTargetTimes[gRaceCourseIndex.signedValue];
        bufp = buf - 0xC;
        sprintf(bufp, gRaceUiTargetTimeChallengeTargetTimeFormat, targetTime->minutes, targetTime->seconds, targetTime->fraction >> 8);
        drawMenuAsciiTextDefaultScale(-0x68, -0x16, bufp, 7);
        drawMenuAsciiTextDefaultScale(-0x68, -8, gRaceUiTargetTimeChallengeFastLabel, 7);
        sprintf(bufp, gRaceUiTargetTimeChallengeDeltaTimeFormat, arg0->row, arg0->column, arg0->timeFraction >> 8);
        drawMenuAsciiTextDefaultScale(-0x6C, 1, bufp, 7);
    } else {
        drawMenuAsciiTextDefaultScale(-0x68, -0x24, gRaceUiTargetTimeChallengeTimeOutLabel, 4);
    }

    y = -0x50;
    drawMenuAsciiTextDefaultScale(-8, (s16)y, gRaceUiTargetTimeChallengePrizeLabel, 5);
    y += 0x20;
    drawMenuAsciiTextDefaultScale(-0xC, (s16)y, gRaceUiTargetTimeChallengeSpeedFanGetLabel, 5);
    y += 0x20;
    if (arg0->flag != 0) {
        drawMenuAsciiTextDefaultScale(-0xC, (s16)y, gRaceUiTargetTimeChallengePerfectGetLabel, 5);
        y += 0x20;
    }
    drawMenuAsciiTextDefaultScale(-8, (s16)y, gRaceUiTargetTimeChallengeCompleteBonusLabel, 5);
    y += 0x20;
    textY = (s16)y;
    drawMenuAsciiTextDefaultScale(-0xC, textY, gRaceUiTargetTimeChallengeMoneyLabel, 5);

    if (arg0->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

void func_8005C3E4(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B8E8, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005BE68, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C03C, arg0);
}

void func_8005C448(void *arg0) {
    if (gPlayerInputPressed[0] & A_BUTTON) {
        enqueueSoundEffect(0x18, 0x32);
        gMenuFlowState |= 0x10;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005C3E4);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B8E8, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005BE68, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C03C, arg0);
}

void func_8005C4EC(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B8E8, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B9F8, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C03C, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C14C, arg0);
}

void func_8005C568(RaceUiDualCounterActor *actor) {
    if (gPlayerInputPressed[0] & A_BUTTON) {
        enqueueSoundEffect(0x18, 0x32);
        if (actor->index < 5) {
            setCallbackTaskCallback(actor, (CallbackTaskCallback)func_8005C448);
        } else {
            gMenuFlowState |= 0x10;
            setCallbackTaskCallback(actor, (CallbackTaskCallback)func_8005C4EC);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B8E8, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B9F8, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C03C, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C14C, actor);
}

void func_8005C64C(RaceUiDualCounterActor *arg0) {
    s16 value;

    if (arg0->timer == 0) {
        arg0->timer = 2;
        enqueueSoundEffect(0x1A, 0x32);
    } else {
        arg0->timer--;
    }

    value = arg0->leftValue;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    /* IDO register allocation depends on this always-zero index. */
    gRacePlayers[gPlayerInputPressed[0] * 0].unkC += value;
    arg0->leftValue -= value;

    value = arg0->rightValue;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    gRacePlayers[0].unkC += value;
    arg0->rightValue -= value;

    value = arg0->bonus;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    gRacePlayers[0].unkC += value;
    arg0->bonus -= value;

    value = arg0->leftTarget;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    gRacePlayers[0].unkC += value;
    arg0->leftTarget -= value;

    if (gRacePlayers[0].unkC >= 0xF4240) {
        gRacePlayers[0].unkC = 0xF423F;
    }

    if ((arg0->rightValue == 0) && (arg0->leftValue == 0) && (arg0->bonus == 0) && (arg0->leftTarget == 0)) {
        enqueueSoundEffect(2, 0x32);
        arg0->timer = 0x14;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005C568);
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B8E8, (void *)arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B9F8, (void *)arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C03C, (void *)arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C14C, (void *)arg0);
}

void func_8005C89C(RaceUiDualCounterActor *actor) {
    actor->state = RACE_UI_COURSE_RECORD_HOLD_FINAL_MONEY_STATE;
    actor->timer--;
    if (actor->timer == 0) {
        setCallbackTaskCallback(actor, (CallbackTaskCallback)func_8005C64C);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B8E8, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B9F8, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C03C, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C14C, actor);
}

void updateRaceUiCourseRecordHoldFinalMoney(RaceUiDualCounterActor *actor) {
    actor->state = RACE_UI_COURSE_RECORD_HOLD_FINAL_MONEY_STATE;
    actor->timer--;
    if (actor->timer == 0) {
        actor->timer = 0xA;
        setCallbackTaskCallback(actor, (CallbackTaskCallback)func_8005C89C);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B8E8, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B9F8, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C03C, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C14C, actor);
}

void updateRaceUiCourseRecordRevealFinalMoney(RaceUiDualCounterActor *actor) {
    actor->state = RACE_UI_COURSE_RECORD_FINAL_MONEY_STATE;
    actor->timer--;
    if (actor->timer == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        actor->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiCourseRecordHoldFinalMoney);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B8E8, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B9F8, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C03C, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C14C, actor);
}

void updateRaceUiCourseRecordRevealPerfectGet(RaceUiDualCounterActor *actor) {
    actor->state = RACE_UI_COURSE_RECORD_REVEAL_PERFECT_GET_STATE;
    actor->timer--;
    if (actor->timer == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        actor->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiCourseRecordRevealFinalMoney);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B8E8, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B9F8, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C03C, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C14C, actor);
}

void func_8005CB74(RaceUiDualCounterActor *actor) {
    actor->state = RACE_UI_COURSE_RECORD_REVEAL_SPEED_FAN_GET_STATE;
    actor->timer--;
    if (actor->timer == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        actor->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        if (actor->flag != 0) {
            setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiCourseRecordRevealPerfectGet);
        } else {
            setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiCourseRecordRevealFinalMoney);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B8E8, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B9F8, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C03C, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C14C, actor);
}

void func_8005CC54(RaceUiDualCounterActor *actor) {
    actor->state = RACE_UI_COURSE_RECORD_REVEAL_TIME_PRIZE_STATE;
    actor->timer--;
    if (actor->timer == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        actor->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        setCallbackTaskCallback(actor, (CallbackTaskCallback)func_8005CB74);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B8E8, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B9F8, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C03C, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C14C, actor);
}

void func_8005CD10(RaceUiDualCounterActor *actor) {
    actor->timer--;
    if (actor->timer == 0) {
        actor->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        enqueueSoundEffect(0x1A, 0x32);
        setCallbackTaskCallback(actor, (CallbackTaskCallback)func_8005CC54);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B8E8, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C03C, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C14C, actor);
}

void func_8005CDB0(RaceUiDualCounterActor *arg0) {
    arg0->alpha += RACE_UI_RESULTS_FADE_STEP;
    if (arg0->alpha >= 0x100) {
        arg0->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        arg0->alpha = RACE_UI_RESULTS_FULL_ALPHA;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005CD10);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005B8E8, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C03C, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005C14C, arg0);
}

void func_8005CE4C(RaceUiDualCounterActor *arg0) {
    u32 temp_v0;
    s16 *new_var;

    arg0->alpha = 0;
    arg0->flag = 0;
    arg0->bonus = 0;
    arg0->leftTarget = 0x12C;
    if (gRaceCourseIndex.signedValue == 9) {
        arg0->leftTarget = 0x64;
    }
    calculateRaceTimerDelta(&gRaceCourseTargetTimes[gRaceCourseIndex.signedValue], &gRaceElapsedTimer,
                            (RaceTimer *)&arg0->row);
    new_var = &gRacePlayers[0].unk570;
    if (gRaceChallengeFailed != 0) {
        arg0->leftValue = 0;
        temp_v0 = *new_var;
    } else {
        arg0->leftValue = ((arg0->row * 0x3C) + arg0->column) * 0x14;
        if (gRacePlayers[0].unk572 == (temp_v0 = gRacePlayers[0].unk570)) {
            arg0->flag = 1;
            arg0->bonus = 0x12C;
            temp_v0 = *new_var;
        }
    }
    arg0->rightValue = temp_v0 * 0xA;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005CDB0);
}

// func_8005CF60 best match: 98.840% (nonmatchings/func_8005CF60-8498672362023432715/base_9.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/ui/race_ui_effects/func_8005CF60.s")

#ifdef NON_MATCHING

const char gRaceUiTrickAttackPointTotalFormat[4] = "%d";
const char gRaceUiTrickAttackSecondaryCounterFormat[4] = "%d";

void func_8005CF60(RaceUiDualCounterActor *arg0) {
    char *ptr;
    char *ptr2;
    char buffer[0x24];
    Gfx **gfxp;
    s32 x;

    gfxp = &gRegionAllocPtr;
    if (arg0->alpha18 != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->alpha18);
    }

    sprintf(buffer, gRaceUiTrickAttackPointTotalFormat, gRacePlayers[0].trickAttackPointTotal);
    x = -0x50;
    ptr = buffer;
    if (gRacePlayers[0].trickAttackPointTotal >= 10) {
        x = -0x54;
    }
    if (gRacePlayers[0].trickAttackPointTotal >= 100) {
        x = -0x58;
    }
    if (gRacePlayers[0].trickAttackPointTotal >= 1000) {
        x = -0x5C;
    }

loop1:
    if (*ptr != '\0') {
        goto body1;
    }
    goto done1;

body1:
    if (*ptr != ' ') {
        drawAssetTableSprite(x, -0x37, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), *ptr - 5);
    }
    ptr++;
    x += 8;
    goto loop1;

done1:
    x = -0x50;
    sprintf(buffer, gRaceUiTrickAttackSecondaryCounterFormat, gRacePlayers[0].unk2C3);
    ptr2 = buffer;
    if (gRacePlayers[0].unk2C3 >= 10) {
        x = -0x54;
    }

loop2:
    if (*ptr2 != '\0') {
        goto body2;
    }
    goto done2;

body2:
    if (*ptr2 != ' ') {
        drawAssetTableSpriteWithExplicitPalette(x, -0x13, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), *ptr2 - 5, 0xE);
    }
    ptr2++;
    x += 8;
    goto loop2;
    x = (unsigned int) gMenuRenderModeResetDl;

done2:
    if (gfxp == NULL) {
    }
    if (arg0->alpha18 != 0xFF) {
        gRacePlayers[0].trickAttackPointTotal += 0;
        if (gfxp != NULL) {
            gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
        }
    }
}
#endif

const char gRaceUiPendingTrickPrizeFormat[0x4] = "%5d";
const char gRaceUiPendingMakeBonusFormat[0x4] = "%5d";
const char gRaceUiPendingCompleteBonusFormat[0x4] = "%5d";
const char gRaceUiPendingMoneyFormat[0x4] = "%6d";

void func_8005D1CC(RaceUiCourseStatsActor *arg0) {
    volatile u8 pad[4];
    SplitWord y;
    s32 i;
    s32 x;
    char buffer[0x20];
    s16 rowY;
    s16 visibleRows;

    y.word = -0x47;
    visibleRows = arg0->visibleRows;
    x = 0x20;
    if (visibleRows >= 0) {
        sprintf(buffer, gRaceUiPendingTrickPrizeFormat, arg0->pendingTrickPrize);
        i = 0;
loop1:
        if (buffer[i] != 0) {
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex(x, -0x47, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                                     ((u8)buffer[i] - 5) & 0xFFFF);
            }
            x += 8;
            i++;
            goto loop1;
        }
        drawAssetTableSpriteWideIndex(x, -0x47, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
        y.word = -0x1F;
        visibleRows = arg0->visibleRows;
    }

    if (visibleRows > 0) {
        x = 0x20;
        sprintf(buffer, gRaceUiPendingMakeBonusFormat, arg0->pendingMakeBonus); i = 0;
        rowY = y.half.lo;
        i = 0;
loop2:
        if (buffer[i] != 0) {
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                                     ((u8)buffer[i] - 5) & 0xFFFF);
            }
            x += 8;
            i++;
            goto loop2;
        }
        drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
        y.word += 0x28;
        visibleRows = arg0->visibleRows;
    }

    if (visibleRows >= 2) {
        x = 0x20;
        sprintf(buffer, gRaceUiPendingCompleteBonusFormat, arg0->pendingCompleteBonus); i = 0;
        rowY = y.half.lo;
        i = 0;
loop3:
        if (buffer[i] != 0) {
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                                     ((u8)buffer[i] - 5) & 0xFFFF);
            }
            x += 8;
            i++;
            goto loop3;
        }
        drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
        y.word += 0x28;
        visibleRows = arg0->visibleRows;
    }

    if (visibleRows >= 3) {
        x = 0x18;
        sprintf(buffer, gRaceUiPendingMoneyFormat, gRacePlayers[0].money); i = 0;
        rowY = y.half.lo;
        i = 0;
loop4:
        if (buffer[i] != 0) {
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                                     ((u8)buffer[i] - 5) & 0xFFFF);
            }
            x += 8;
            i++;
            goto loop4;
        }
        drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
    }
}

const char gRaceUiTrickValueFormat[0x4] = "%4d";

void func_8005D558(RaceUiCourseStatsActor *arg0) {
    volatile u8 padding[0x18];
    char new_var;
    s32 i;
    s32 color;
    s32 y;
    s32 space;

    y = -0x50;
    i = 0; space = ' '; do { char buffer[8]; drawAssetTableSprite(-8, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), i + 0x77); if ((i == arg0->index) && (gUiBlinkTimer & 1)) { color = 0x10; } else if (i < 3) { color = 0xC; } else {
            color = 0xD;
        }

        sprintf(buffer, gRaceUiTrickValueFormat, ((RaceUiTrickValueData *)&gGameSaveDataBuffer)->values[gRaceCourseIndex.signedValue][i]);

        if ((u8)buffer[0] != space) {
            drawAssetTableSpriteWithExplicitPalette(0x10, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                          buffer[0] - 5, color);
            do { } while (0);
        }
        if ((u8)buffer[1] != space) {
            drawAssetTableSpriteWithExplicitPalette(0x18, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                          buffer[1] - 5, (u16)color);
        }
        if ((u8)buffer[2] != space) {
            drawAssetTableSpriteWithExplicitPalette(0x20, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                          buffer[2] - 5, (u16)color);
        }
        if ((u8)buffer[3] != space) {
            drawAssetTableSpriteWithExplicitPalette(0x28, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                          buffer[3] - 5, (u16)color);
        }

        drawAssetTableSprite(0x40, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                      (((RaceUiTrickIconData *)&gGameSaveDataBuffer)->icons[gRaceCourseIndex.signedValue][i] & 7) + 0x51);
        drawAssetTableSprite(0x54, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                      (((RaceUiTrickIconData *)&gGameSaveDataBuffer)->icons[gRaceCourseIndex.signedValue][i] >> 3) + 0x7C);

        i++;
        y += 0x20;
    } while (i != 5);
}

const char gRaceUiTrickAttackResultsTrickLabel[0x8] = "-Trick-";
const char gRaceUiTrickAttackResultsMakeLabel[0x8] = "-Make-";
const char gRaceUiTrickAttackResultsTimeOutLabel[0xC] = "Time Out";
const char gRaceUiPendingTrickPrizeLabel[0x10] = "  -Trick Prize-";
const char gRaceUiPendingMakeBonusLabel[0x10] = "  -Make Bonus-";
const char gRaceUiPendingCompleteBonusLabel[0x14] = "-Complete Bonus-";
const char gRaceUiPendingMoneyLabel[0x10] = "  -Total Money-";

void func_8005D860(RaceUiAlpha18Actor *arg0) {
    Gfx *gfx;
    const char *new_var2;
    s32 w0;
    s32 w1;
    s32 new_var;

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; if (!gfx) { } gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; if ((gRegionAllocPtr && gRegionAllocPtr) && gRegionAllocPtr) { } w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->alpha & 0xFF) | (~0xFF); } drawMenuAsciiTextDefaultScale(-0x68, -0x40, new_var2 = gRaceUiTrickAttackResultsTrickLabel, 6); drawMenuAsciiTextDefaultScale(-0x64, -0x1C, gRaceUiTrickAttackResultsMakeLabel, 5); w0 = 8; if (gRaceChallengeFailed != 0) { new_var = w0; drawMenuAsciiTextDefaultScale(-0x6C, new_var, gRaceUiTrickAttackResultsTimeOutLabel, 4); } if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) gMenuRenderModeResetDl; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; } } while (0);
}

void func_8005D9B4(RaceUiAlpha18Actor *arg0) {
    Gfx *gfx;
    s32 w0;
    s32 w1;
    s32 y;
    RaceUiAlpha18Actor **arg0p = &arg0;

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { if ((*arg0p)->alpha != 0xFF) {
        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        gfx->words.w1 = 0;
        gfx->words.w0 = 0xE7000000;

        gfx = gRegionAllocPtr;
        gRegionAllocPtr = gfx + 1;
        w1 = 0xFF2FFFFF;
        w0 = 0xFC119623;
        gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000;
        gfx->words.w1 = ((*arg0p)->alpha & 0xFF) | ~0xFF;
    }

    y = -0x50;
    drawMenuAsciiTextDefaultScale(-0xC, (s16)y, &gRaceUiPendingTrickPrizeLabel, 5);
    y += 0x28;
    drawMenuAsciiTextDefaultScale(-8, (s16)y, &gRaceUiPendingMakeBonusLabel, 5);
    y += 0x28;
    drawMenuAsciiTextDefaultScale(-8, (s16)y, &gRaceUiPendingCompleteBonusLabel, 5);
    y += 0x28;
    drawMenuAsciiTextDefaultScale(-0xC, (s16)y, &gRaceUiPendingMoneyLabel, 5);

    if ((*arg0p)->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
    } while (0);
}

const char gRaceUiTrickValuePointLabelRow1[0x4] = "P";
const char gRaceUiTrickValuePointLabelRow2[0x4] = "P";
const char gRaceUiTrickValuePointLabelRow3[0x4] = "P";
const char gRaceUiTrickValuePointLabelRow4[0x4] = "P";
const char gRaceUiTrickValuePointLabelRow5[0x8] = "P";

void func_8005DB3C(void *arg0) {
    drawMenuAsciiTextDefaultScale(0x30, -0x48, &gRaceUiTrickValuePointLabelRow1, 6);
    drawMenuAsciiTextDefaultScale(0x30, -0x28, &gRaceUiTrickValuePointLabelRow2, 6);
    drawMenuAsciiTextDefaultScale(0x30, -8, &gRaceUiTrickValuePointLabelRow3, 6);
    drawMenuAsciiTextDefaultScale(0x30, 0x18, &gRaceUiTrickValuePointLabelRow4, 4);
    drawMenuAsciiTextDefaultScale(0x30, 0x38, &gRaceUiTrickValuePointLabelRow5, 4);
}

void func_8005DBD0(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005CF60, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005D558, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D860, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005DB3C, arg0);
}

void func_8005DC4C(void *arg0) {
    if (gPlayerInputPressed[0] & A_BUTTON) {
        enqueueSoundEffect(0x18, 0x32);
        gMenuFlowState |= 0x10;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005DBD0);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005CF60, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005D558, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D860, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005DB3C, arg0);
}

void func_8005DD0C(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005CF60, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005D1CC, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D860, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D9B4, arg0);
}

void updateRaceUiTrickPrizePayoutWaitForConfirm(RaceUiCourseStatsActor *actor) {
    if (gPlayerInputPressed[0] & A_BUTTON) {
        enqueueSoundEffect(0x18, 0x32);
        if (actor->index < 5) {
            setCallbackTaskCallback(actor, (CallbackTaskCallback)func_8005DC4C);
        } else {
            gMenuFlowState |= 0x10;
            setCallbackTaskCallback(actor, (CallbackTaskCallback)func_8005DD0C);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005CF60, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005D1CC, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D860, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D9B4, actor);
}

void func_8005DE6C(RaceUiCourseStatsActor *arg0) {
    s16 value;

    if (arg0->timer == 0) {
        arg0->timer = 2;
        enqueueSoundEffect(0x1A, 0x32);
    } else {
        arg0->timer--;
    }

    value = arg0->pendingTrickPrize;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    /* IDO register allocation depends on this always-zero index. */
    gRacePlayers[gPlayerInputPressed[0] * 0].unkC += value;
    arg0->pendingTrickPrize -= value;

    value = arg0->pendingMakeBonus;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    gRacePlayers[0].unkC += value;
    arg0->pendingMakeBonus -= value;

    value = arg0->pendingCompleteBonus;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    gRacePlayers[0].unkC += value;
    arg0->pendingCompleteBonus -= value;

    if (gRacePlayers[0].unkC >= 0xF4240) {
        gRacePlayers[0].unkC = 0xF423F;
    }

    if ((arg0->pendingTrickPrize == 0) && (arg0->pendingMakeBonus == 0) && (arg0->pendingCompleteBonus == 0)) {
        enqueueSoundEffect(2, 0x32);
        arg0->timer = 0x14;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickPrizePayoutWaitForConfirm);
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005CF60, (void *)arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005D1CC, (void *)arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D860, (void *)arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D9B4, (void *)arg0);
}

void updateRaceUiTrickPrizePayoutHoldMoneyRow(RaceUiCourseStatsActor *arg0) {
    arg0->visibleRows = RACE_UI_TRICK_PRIZE_SHOW_TOTAL_MONEY;
    arg0->timer--;
    if (arg0->timer == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005DE6C);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005CF60, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005D1CC, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D860, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D9B4, arg0);
}

void updateRaceUiTrickPrizePayoutRevealMoneyRow(RaceUiCourseStatsActor *arg0) {
    arg0->visibleRows = 3;
    arg0->timer--;
    if (arg0->timer == 0) {
        arg0->timer = 0xA;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickPrizePayoutHoldMoneyRow);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005CF60, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005D1CC, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D860, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D9B4, arg0);
}

void updateRaceUiTrickPrizePayoutRevealCompleteBonus(RaceUiCourseStatsActor *arg0) {
    arg0->visibleRows = RACE_UI_TRICK_PRIZE_SHOW_COMPLETE_BONUS;
    arg0->timer--;
    if (arg0->timer == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        arg0->timer = RACE_UI_TRICK_PRIZE_REVEAL_DELAY;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickPrizePayoutRevealMoneyRow);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005CF60, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005D1CC, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D860, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D9B4, arg0);
}

void updateRaceUiTrickPrizePayoutRevealMakeBonus(RaceUiCourseStatsActor *arg0) {
    arg0->visibleRows = 1;
    arg0->timer--;
    if (arg0->timer == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        arg0->timer = RACE_UI_TRICK_PRIZE_REVEAL_DELAY;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickPrizePayoutRevealCompleteBonus);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005CF60, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005D1CC, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D860, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D9B4, arg0);
}

void updateRaceUiTrickPrizePayoutRevealTrickPrize(RaceUiCourseStatsActor *arg0) {
    arg0->timer--;
    arg0->visibleRows = 0;
    if (arg0->timer == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        arg0->timer = RACE_UI_TRICK_PRIZE_REVEAL_DELAY;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickPrizePayoutRevealMakeBonus);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005CF60, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005D1CC, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D860, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D9B4, arg0);
}

void updateRaceUiTrickPrizePayoutWaitBeforeReveal(RaceUiCourseStatsActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        arg0->timer = RACE_UI_TRICK_PRIZE_REVEAL_DELAY;
        enqueueSoundEffect(0x1A, 0x32);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickPrizePayoutRevealTrickPrize);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005CF60, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D860, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D9B4, arg0);
}

void updateRaceUiTrickPrizePayoutFadeIn(RaceUiCourseStatsActor *arg0) {
    arg0->titleAlpha += 0x10;
    if (arg0->titleAlpha >= 0x100) {
        arg0->timer = 0x14;
        arg0->titleAlpha = 0xFF;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickPrizePayoutWaitBeforeReveal);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8005CF60, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D860, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)func_8005D9B4, arg0);
}

void initRaceUiTrickPrizePayout(RaceUiCourseStatsActor *arg0) {
    arg0->titleAlpha = 0;
    arg0->pendingMakeBonus = 0;
    arg0->pendingCompleteBonus = 0x12C;
    if (gRaceChallengeFailed != 0) {
        arg0->pendingTrickPrize = 0;
        *(volatile s16 *)&arg0->pendingMakeBonus = 0;
    } else {
        arg0->pendingTrickPrize = gRacePlayers[0].unk2C0 * 3;
        arg0->pendingMakeBonus = gRacePlayers[0].unk2C3 * 10;
    }
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickPrizePayoutFadeIn);
}

void drawRaceUiResultAwardBadge(CallbackTask *task) {
    drawAssetTableSprite(-0x68, -0x2C, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), 0x4D);
    drawAssetTableSprite(-0x42, -0xC, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), gRacePlayers[0].rankIndex);
    drawAssetTableSprite(-0x22, 4, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), gRacePlayers[0].rankIndex + 0x48);
    drawAssetTableSprite(-0x68, 6, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), gRacePlayers[0].characterId + 0x91);
}

void updateRaceUiResultAwardBadgeDelay(CallbackTask *task) {
    if ((u16)task->userId != 0) {
        task->userId = (u16)task->userId - 1;
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiResultAwardBadge, (void *)task);
    }
}

void func_8005E6D0(RaceUiSparkleActor *arg0) {
    RaceUiSparkleActor *arg1;
    s32 i;
    s32 j;
    RaceUiTrailCopyBlock sp6C;

    arg1 = arg0;
    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg1->matrixDirty != 0) {
        arg1->matrixDirty = 0;
        sp6C.transform = gIdentityFixedTransform;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                sp6C.halfwords[(i * 3) + j] = (sp6C.halfwords[(i * 3) + j] * arg1->scale) / 0x1000;
            }
        }
        sp6C.transform.translation.x = arg1->pos.x;
        sp6C.transform.translation.y = arg1->pos.y;
        sp6C.transform.translation.z = arg1->pos.z;
        arg1->matrix = allocFixedTransformMatrix(&sp6C);
    }

    if (arg1->matrix != NULL) {
        gSPDisplayList(gRegionAllocPtr++, gTranslucentSpriteRenderModeDl);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg1->alpha);
        gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg1->images[arg1->frame], G_IM_FMT_CI, 0x20, 0x20,
                               0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg1->palettes[arg1->frame]);
        gSPMatrix(gRegionAllocPtr++, arg1->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        RACE_UI_GSP_VERTEX_F3DEX(gRegionAllocPtr++, D_800D60A0, 4, 0);
        RACE_UI_GSP1QUADRANGLE_F3DEX(gRegionAllocPtr++, 2, 1, 0, 3, 3);
    }
}

void func_8005EA4C(RaceUiSparkleActor *arg0) {
    RaceUiSparkleTransformScratch stack;
    RacePlayer *player;
    s16 timer;
    s32 angle;

    if (gRaceUpdatePaused == 0) {
        arg0->scale += 0x100;
        if (arg0->scale >= 0x1001) {
            arg0->scale = 0x1000;
        }
        if (gFrameCounter & 1) {
            arg0->frame = (arg0->frame + 1) & 3;
        }

        timer = arg0->timer;
        if (timer != 0) {
            arg0->timer = timer - 1;
            arg0->alpha += arg0->alphaStep;
            if (arg0->alpha >= 0xE0) {
                arg0->alphaStep = -0x10;
            }
            if (arg0->alpha < 0x41) {
                arg0->alphaStep = 0x10;
            }
        } else {
            arg0->alpha -= 0x10;
            if (arg0->alpha <= 0) {
                player = &gRacePlayers[arg0->playerIndex];
                player->unk2D8--;
                removeCallbackTask((CallbackTask *) arg0);
                return;
            }
        }

        arg0->unk28 += 0x60;
        angle = arg0->unk28 - gRacePlayers[arg0->playerIndex].facingAngle;
        makeFixedRotationY(stack.matrix, angle);
        transformVec3iByFixedMatrix(stack.matrix, &D_800D6110, &stack.vec);

        player = &gRacePlayers[arg0->playerIndex];
        makeFixedRotationXY(stack.matrix, player->pitchAngle, player->facingAngle);
        transformVec3iByFixedMatrix(stack.matrix, &stack.vec, &arg0->pos);

        player = &gRacePlayers[arg0->playerIndex];
        arg0->pos.x += player->unk28.x;
        arg0->pos.y += player->unk28.y;
        arg0->pos.z += player->unk28.z;
    }

    addRenderCallback(&D_801248EC, (RenderCallback)func_8005E6D0, (void *)arg0);
}

void func_8005ECA8(RaceUiSparkleActor *arg0) {
    RacePlayer *player;
    s16 timer;
    s32 selectedPlayerIndex;
    s32 activeCount;
    RaceUiSparkleRetargetScratch scratch;

    if (gRaceUpdatePaused == 0) {
        if (gFrameCounter & 1) {
            arg0->frame = (arg0->frame + 1) & 3;
        }

        scratch.vec.x = 0;
        scratch.vec.y = arg0->zOffset;
        scratch.vec.z = 0;

        player = &gRacePlayers[arg0->playerIndex];
        if (arg0) {
        }
        makeFixedRotationXY(scratch.matrix, player->pitchAngle, player->facingAngle);
        transformVec3iByFixedMatrix(scratch.matrix, &scratch.vec, &arg0->pos);

        player = &gRacePlayers[arg0->playerIndex];
        arg0->pos.x += player->unk28.x;
        arg0->pos.y += player->unk28.y;
        arg0->pos.z += player->unk28.z;
        selectedPlayerIndex = gRaceOrderPlayerIds[1];

        timer = arg0->timer;
        if (timer == 0) {
            arg0->alpha += arg0->alphaStep;
            arg0->scale -= 0x200;
            if (arg0->scale < 0) {
                arg0->scale = 0;
            }
            if (arg0->alpha < 0) {
                arg0->alpha = 0;
                arg0->alphaStep = 0x10;
                activeCount = randomNextMain() ^ 0;
                if (activeCount > 0) {
                    selectedPlayerIndex = 0;
                    if ((gRacePlayerCount >= 4) && (arg0->playerIndex != gRaceOrderPlayerIds[3]) &&
                        (activeCount > 0)) {
                        selectedPlayerIndex = gRaceOrderPlayerIds[3];
                    }
                    if ((gRacePlayerCount >= 3) && (arg0->playerIndex != gRaceOrderPlayerIds[2]) &&
                        (activeCount >= 2) && !(gRacePlayers[gRaceOrderPlayerIds[2]].stateFlags & 0x40)) {
                        selectedPlayerIndex = gRaceOrderPlayerIds[2];
                    }
                    if ((gRacePlayerCount >= 2) && (arg0->playerIndex != gRaceOrderPlayerIds[1]) &&
                        (activeCount >= 3) && !(gRacePlayers[gRaceOrderPlayerIds[1]].stateFlags & 0x40)) {
                        selectedPlayerIndex = gRaceOrderPlayerIds[1];
                    }
                    if ((gRacePlayerCount > 0) && (arg0->playerIndex != gRaceOrderPlayerIds[0]) &&
                        (activeCount >= 4) && !(gRacePlayers[gRaceOrderPlayerIds[0]].stateFlags & 0x40)) {
                        selectedPlayerIndex = gRaceOrderPlayerIds[0];
                    }
                    arg0->playerIndex = selectedPlayerIndex & 3;
                }

                player = &gRacePlayers[arg0->playerIndex];
                player->unk2D8++;
                if (gRacePlayerHudStatuses[arg0->playerIndex].active != 0) {
                    enqueueSoundEffect(0x53, 0x5A);
                }
                enqueuePositionalSoundEffect(0x10, &gRacePlayers[arg0->playerIndex].pos, 0x7F, 0x32);
                arg0->timer = 0x12C;
                setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005EA4C);
            }
        } else {
            arg0->timer = timer - 1;
        }
    }

    addRenderCallback(&D_801248EC, (RenderCallback)func_8005E6D0, (void *)arg0);
}

void func_8005EFFC(RaceUiSparkleActor *arg0) {
    s16 sp38[0x10];
    Vec3i sp2C;
    volatile u8 pad[8];
    RacePlayer *player;

    if (gRaceUpdatePaused == 0) {
        if (gFrameCounter & 1) {
            arg0->frame = (arg0->frame + 1) & 3;
        }

        sp2C.x = 0;
        sp2C.y = arg0->zOffset;
        sp2C.z = 0;
        player = &gRacePlayers[arg0->playerIndex];
        makeFixedRotationXY(sp38, player->pitchAngle, player->facingAngle);
        transformVec3iByFixedMatrix(sp38, &sp2C, &arg0->pos);

        player = &gRacePlayers[arg0->playerIndex];
        arg0->pos.x += player->unk28.x;
        arg0->pos.y += player->unk28.y;
        arg0->pos.z += player->unk28.z;
        arg0->alpha += arg0->alphaStep;
        if (arg0->alpha >= 0x100) {
            arg0->alpha = 0xFF;
            arg0->alphaStep = -0x20;
            arg0->timer = 0x14;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005ECA8);
        }
        arg0->zOffset += 0x20000;
    }

    addRenderCallback(&D_801248EC, (RenderCallback)func_8005E6D0, arg0);
}

void func_8005F174(RaceUiSparkleActor *arg0) {
    arg0->zOffset = 0x20000;
    arg0->alpha = 0;
    arg0->alphaStep = 0x10;
    arg0->frame = 0;
    arg0->scale = 0x1000;
    arg0->unk28 = 0;

    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)), 0x23, &arg0->images[0], &arg0->palettes[0]);
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)), 0x24, &arg0->images[1], &arg0->palettes[1]);
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)), 0x25, &arg0->images[2], &arg0->palettes[2]);
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)), 0x26, &arg0->images[3], &arg0->palettes[3]);
    enqueuePositionalSoundEffect(0x10, &gRacePlayers[arg0->playerIndex].pos, 0x7F, 0x32);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005EFFC);
}

void spawnRacePlayerSparkleEffect(s16 arg0) {
    RaceUiSparkleActor *temp = createCallbackTaskPreservingArgs((CallbackTaskCallback)func_8005F174, 0, 0x62);
    if (temp != NULL) {
        temp->playerIndex = arg0;
    }
}

void renderRaceUiSnowboardTrailEffect(RaceUiSnowboardTrailActor *arg0) {
    volatile u8 pad[0x20];
    RaceUiDisplayCommand *unused;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        arg0->frontMatrix = allocFixedTransformMatrix(&arg0->frontTransform);
        arg0->backMatrix = allocFixedTransformMatrix(&arg0->backTransform);
    }

    if (arg0->frontMatrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->frontMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, gSnowboardTrailFrontDisplayList);
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->backMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, gSnowboardTrailBackDisplayList);
    }
}

void updateRaceUiSnowboardTrailEffect(RaceUiSnowboardTrailActor *arg0) {
    FixedTransform sp30;
    volatile u8 pad[0x10];
    RaceUiSnowboardTrailActor *actor;

    actor = arg0;
    actor->spinYaw += 0x240;
    if (gRaceUpdatePaused == 0) {
        actor->worldPos.y += actor->velocityY;
        actor->velocityY -= 0x8000;
    }

    actor->frontTransform.transform.translation.x = actor->worldPos.x;
    actor->frontTransform.transform.translation.y = actor->worldPos.y;
    actor->frontTransform.transform.translation.z = actor->worldPos.z;

    makeFixedRotationX(sp30.rotation, actor->spinYaw);
    sp30.translation.x = actor->scale.x;
    sp30.translation.y = actor->scale.y;
    sp30.translation.z = actor->scale.z;
    composeFixedTransforms(&sp30, &actor->frontTransform.transform, &actor->backTransform.transform);

    actor->timer--;
    if (actor->timer == 0) {
        removeCallbackTask(actor);
        return;
    }

    if (gRacePlayers[actor->playerIndex].soundDisabled == 0) {
        addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)renderRaceUiSnowboardTrailEffect, arg0);
    }
}

void initRaceUiSnowboardTrailEffect(RaceUiSnowboardTrailActor *actor) {
    actor->scaleStep = 1;
    actor->scale.x = RACE_UI_SNOWBOARD_TRAIL_SCALE_X;
    actor->scale.y = RACE_UI_SNOWBOARD_TRAIL_SCALE_Y;
    actor->scale.z = RACE_UI_SNOWBOARD_TRAIL_SCALE_Z;
    actor->timer = RACE_UI_SNOWBOARD_TRAIL_TIMER;
    actor->velocityY = RACE_UI_SNOWBOARD_TRAIL_INITIAL_VELOCITY_Y;
    updateRaceUiSnowboardTrailEffect(actor);
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiSnowboardTrailEffect);
}

void spawnRaceUiSnowboardTrailEffect(RaceUiSnowboardTrailPlayer *player) {
    RaceUiSnowboardTrailActor *actor = createCallbackTaskPreservingArgs((CallbackTaskCallback)initRaceUiSnowboardTrailEffect, 0, 0x62);

    if (actor != NULL) {
        actor->playerIndex = player->playerIndex;
        actor->worldPos = player->trail.worldPos;
        actor->frontTransform = player->trail.copyBlock;
        actor->spinYaw = player->trail.spinYaw;
    }
}

void func_8005F6A4(RaceUiRankTrailActor *arg0) {
    volatile u8 pad[0x20];
    RacePlayer *player;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        player = &gRacePlayers[arg0->playerIndex];
        arg0->copyBlock.words[5] = player->unk28.x;
        arg0->copyBlock.words[6] = player->unk28.y + 0x100000;
        arg0->copyBlock.words[7] = player->unk28.z;
        arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
    }

    if (arg0->matrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &_ADDR_2003870_VRAM);
    }
}

void func_8005F828(RaceUiRankTrailActor *arg0) {
    RacePlayer *player;
    s16 scale;
    s32 i;

    arg0->copyBlock.transform = gIdentityFixedTransform;
    player = &gRacePlayers[arg0->playerIndex];
    arg0->pos.x = player->unk28.x;
    arg0->pos.y = player->unk28.y;
    arg0->pos.z = player->unk28.z;

    for (i = 0; i < 4; i++) {
        if (i != arg0->playerIndex) {
            pushRacePlayerOutOfCylinderAndApplyItemHit(&arg0->pos, 0xE0000, 0xB0000, 2, i);
        }
    }

    scale = arg0->scale;
    arg0->copyBlock.halfwords[0] = (arg0->copyBlock.halfwords[0] * scale) / 64;
    arg0->copyBlock.halfwords[1] = (arg0->copyBlock.halfwords[1] * scale) / 64;
    arg0->copyBlock.halfwords[2] = (arg0->copyBlock.halfwords[2] * scale) / 64;
    arg0->copyBlock.halfwords[3] = (arg0->copyBlock.halfwords[3] * scale) / 64;
    arg0->copyBlock.halfwords[4] = (arg0->copyBlock.halfwords[4] * scale) / 64;
    arg0->copyBlock.halfwords[5] = (arg0->copyBlock.halfwords[5] * scale) / 64;
    arg0->copyBlock.halfwords[6] = (arg0->copyBlock.halfwords[6] * scale) / 64;
    arg0->copyBlock.halfwords[7] = (arg0->copyBlock.halfwords[7] * scale) / 64;
    arg0->copyBlock.halfwords[8] = (arg0->copyBlock.halfwords[8] * scale) / 64;

    if (scale != 0x10) {
        arg0->scale = scale + 2;
    }

    if (gRacePlayers[arg0->playerIndex].stateFlags & 0x100000) {
        addRenderCallback(&D_801248EC, (RenderCallback)func_8005F6A4, (void *)arg0);
        return;
    }

    for (i = 0; i < 8; i++) {
        spawnRaceUiBurstTextParticle((void *)arg0->pos.x, (void *)arg0->pos.y, (void *)arg0->pos.z, i);
    }

    enqueuePositionalSoundEffect(0x15, &gRacePlayers[arg0->playerIndex].pos, 0x7F, 0x32);
    removeCallbackTask(arg0);
}

void initRaceUiItemStealTrailEffect(RaceUiRankTrailActor *arg0) {
    arg0->scale = 2;
    enqueuePositionalSoundEffect(0xF, &gRacePlayers[arg0->playerIndex].pos, 0x7F, 0x32);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_8005F828);
}

void func_8005FBA8(RaceUiAnimatedTextActor *arg0) {
    volatile u8 padding[4];
    RaceUiTrailCopyBlock sp64;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        sp64.transform = gIdentityFixedTransform;
        sp64.transform.translation.x = arg0->pos.x;
        sp64.transform.translation.y = arg0->pos.y;
        sp64.transform.translation.z = arg0->pos.z;
        arg0->matrix = allocFixedTransformMatrix(&sp64);
    }

    if (arg0->matrix != NULL) {
        gSPDisplayList(gRegionAllocPtr++, D_800D6160);
        gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->images[arg0->frame >> 2], G_IM_FMT_CI, 0x20, 0x20,
                               0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palettes[arg0->frame >> 2]);
        gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        RACE_UI_GSP_VERTEX_F3DEX(gRegionAllocPtr++, D_800D6120, 4, 0);
        RACE_UI_GSP1QUADRANGLE_F3DEX(gRegionAllocPtr++, 2, 1, 0, 3, 3);
    }
}

void func_8005FED0(RaceUiTextParticleActor *arg0) {
    volatile u8 padding[4];
    RaceUiTrailCopyBlock sp64;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        sp64.transform = gIdentityFixedTransform;
        sp64.transform.translation.x = arg0->pos.x;
        sp64.transform.translation.y = arg0->pos.y;
        sp64.transform.translation.z = arg0->pos.z;
        arg0->matrix = allocFixedTransformMatrix(&sp64);
    }

    if (arg0->matrix != NULL) {
        gSPDisplayList(gRegionAllocPtr++, D_800D6190);
        gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->images[arg0->timer >> 2], G_IM_FMT_CI, 0x20, 0x20,
                               0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palettes[arg0->timer >> 2]);
        gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        RACE_UI_GSP_VERTEX_F3DEX(gRegionAllocPtr++, D_800D6120, 4, 0);
        RACE_UI_GSP1QUADRANGLE_F3DEX(gRegionAllocPtr++, 2, 1, 0, 3, 3);
    }
}

void updateRaceUiBurstTextParticle(RaceUiTextParticleActor *arg0) {
    s32 velocityY;

    if (gRaceUpdatePaused == 0) {
        velocityY = arg0->velocity.y;
        arg0->pos.x += arg0->velocity.x;
        arg0->pos.y += velocityY;
        arg0->pos.z += arg0->velocity.z;
        arg0->velocity.y = velocityY - 0x8000;
        arg0->timer++;
        if (arg0->timer >= 0x10) {
            removeCallbackTask(arg0);
            return;
        }
    }
    if (arg0->useAltTextures == 0) {
        addRenderCallback(&D_801248EC, (RenderCallback)func_8005FBA8, arg0);
        return;
    }
    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)func_8005FED0, arg0);
}

void initRaceUiBurstTextParticle(RaceUiTextParticleActor *arg0) {
    arg0->timer = 0;
    arg0->velocity = D_800D61C0[arg0->index];
    if (arg0->useAltTextures == 0) {
        getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x27, &arg0->images[0], &arg0->palettes[0]);
        getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x28, &arg0->images[1], &arg0->palettes[1]);
        getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x29, &arg0->images[2], &arg0->palettes[2]);
        getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x2A, &arg0->images[3], &arg0->palettes[3]);
    } else {
        getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x2B, &arg0->images[0], &arg0->palettes[0]);
        getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x2C, &arg0->images[1], &arg0->palettes[1]);
        getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x2D, &arg0->images[2], &arg0->palettes[2]);
        getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x2E, &arg0->images[3], &arg0->palettes[3]);
    }
    updateRaceUiBurstTextParticle(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiBurstTextParticle);
}

void spawnRaceUiBurstTextParticle(void *arg0, void *arg1, void *arg2, s16 arg3) {
    RaceUiTextParticleActor *actor = createCallbackTaskPreservingArgs((CallbackTaskCallback)initRaceUiBurstTextParticle, 5, 0x1E);
    if (actor != NULL) {
        actor->useAltTextures = 0;
        actor->index = arg3;
        actor->pos.x = (s32)arg0;
        actor->pos.y = (s32)arg1;
        actor->pos.z = (s32)arg2;
    }
}

void spawnRaceUiAltBurstTextParticle(void *arg0, void *arg1, void *arg2, s16 arg3) {
    RaceUiTextParticleActor *actor = createCallbackTaskPreservingArgs((CallbackTaskCallback)initRaceUiBurstTextParticle, 5, 0x1E);
    if (actor != NULL) {
        actor->useAltTextures = 1;
        actor->index = arg3;
        actor->pos.x = (s32)arg0;
        actor->pos.y = (s32)arg1;
        actor->pos.z = (s32)arg2;
    }
}

void func_80060544(RaceUiPodiumTrailActor *arg0) {
    volatile u8 pad[0x20];
    volatile RaceUiDisplayCommand *unused;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        arg0->copyBlock.words[5] = arg0->pos.x;
        arg0->copyBlock.words[6] = arg0->pos.y + 0x38000;
        arg0->copyBlock.words[7] = arg0->pos.z;
        arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
    }

    if (arg0->matrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &_ADDR_2002660_VRAM);
    }
}

void updateGhostSlowdownDriftAway(RaceUiPodiumTrailActor *arg0) {
    if (gRaceUpdatePaused == 0) {
        arg0->velocity -= 0x10000;
        arg0->pos.x += 0x20000;
        arg0->pos.y += arg0->velocity;
        arg0->pos.z += 0x10000;
        arg0->timer--;
    }

    if (arg0->timer == 0) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)func_80060544, arg0);
    }
}

void updateGhostSlowdownImpact(RaceUiPodiumTrailActor *arg0) {
    RacePlayer *player;
    s32 height;

    if (gRaceUpdatePaused == 0) {
        arg0->velocity += -0x10000;
        height = arg0->height + arg0->velocity;
        arg0->height = height;
        if ((height < 0x200000) && ((arg0->playImpactSound & 0xFF) != 0)) {
            enqueuePositionalSoundEffect(0xC, &gRacePlayers[arg0->targetPlayerIndex].pos, 0x7F, 0x32);
            gRacePlayers[arg0->targetPlayerIndex].pendingItemHitFlags |= PLAYER_HITFLAG_GHOST_SLOWDOWN;
            gRacePlayers[arg0->targetPlayerIndex].hitSourcePlayerIndex = arg0->playerIndex;
            height = arg0->height;
            arg0->playImpactSound = 0;
        }

        if (height < 0x100000) {
            arg0->timer = 0x14;
            arg0->velocity = 0x80000;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateGhostSlowdownDriftAway);
            height = arg0->height;
        }

        player = &gRacePlayers[arg0->targetPlayerIndex];
        arg0->pos.x = player->unk28.x;
        arg0->pos.y = player->unk28.y + height;
        arg0->pos.z = player->unk28.z;
    }

    gRacePlayers[arg0->targetPlayerIndex].itemTargetFlag = 1;
    addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)func_80060544, arg0);
}

void updateGhostSlowdownRise(RaceUiPodiumTrailActor *arg0) {
    RacePlayer *player;
    s16 scale;

    if (arg0->timer == 0) {
        if (gRaceUpdatePaused == 0) {
            arg0->velocity += 0x10000;
            arg0->height += arg0->velocity;
            arg0->copyBlock.transform = gIdentityFixedTransform;
            player = &gRacePlayers[arg0->playerIndex];
            arg0->pos.x = player->unk28.x;
            arg0->pos.y = player->unk28.y + arg0->height;
            arg0->pos.z = player->unk28.z;

            scale = arg0->state;
            arg0->copyBlock.halfwords[0] = (arg0->copyBlock.halfwords[0] * scale) / 64;
            arg0->copyBlock.halfwords[1] = (arg0->copyBlock.halfwords[1] * scale) / 64;
            arg0->copyBlock.halfwords[2] = (arg0->copyBlock.halfwords[2] * scale) / 64;
            arg0->copyBlock.halfwords[3] = (arg0->copyBlock.halfwords[3] * scale) / 64;
            arg0->copyBlock.halfwords[4] = (arg0->copyBlock.halfwords[4] * scale) / 64;
            arg0->copyBlock.halfwords[5] = (arg0->copyBlock.halfwords[5] * scale) / 64;
            arg0->copyBlock.halfwords[6] = (arg0->copyBlock.halfwords[6] * scale) / 64;
            arg0->copyBlock.halfwords[7] = (arg0->copyBlock.halfwords[7] * scale) / 64;
            arg0->copyBlock.halfwords[8] = (arg0->copyBlock.halfwords[8] * scale) / 64;

            if (scale != 0x10) {
                arg0->state = scale + 1;
            }

            if (arg0->height >= 0xA00001) {
                arg0->velocity = -0x10000;
                setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateGhostSlowdownImpact);
            }
        }
        gRacePlayers[arg0->targetPlayerIndex].itemTargetFlag = 1;
        addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)func_80060544, (void *)arg0);
    } else if (gRaceUpdatePaused == 0) {
        arg0->timer--;
    }
}

void initGhostSlowdownActor(RaceUiPodiumTrailActor *arg0) {
    arg0->state = 1;
    arg0->height = 0;
    arg0->velocity = 0;
    arg0->playImpactSound = 1;
    enqueuePositionalSoundEffect(0xD, &gRacePlayers[arg0->playerIndex].pos, 0x7F, 0x32);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateGhostSlowdownRise);
}

void spawnGhostSlowdownTargets(s16 arg0) {
    s32 spawnIndex;
    s32 playerIndex;
    RaceUiPodiumTrailActor *actor;

    spawnIndex = 0;
    playerIndex = 0;
    if (gRacePlayerCount > 0) {
        do {
            if (arg0 != playerIndex) {
                actor = createCallbackTaskPreservingArgs((CallbackTaskCallback)initGhostSlowdownActor, 0, 0x1E);
                if (actor != NULL) {
                    actor->playerIndex = arg0;
                    actor->targetPlayerIndex = playerIndex;
                    actor->timer = spawnIndex * 3;
                    spawnIndex += 1;
                }
            }
            playerIndex += 1;
        } while (playerIndex < gRacePlayerCount);
    }
}

const char gRaceUiScorePopupPointsFormat[0x10] = "%4d";

void drawRaceUiScorePopup(RaceUiPopupActor *arg0) {
    volatile u8 padding[0x20];
    char buffer[8];
    s32 i;

    if (gCurrentViewportIndex == arg0->playerIndex) {
        drawAssetTableSprite((s16)(arg0->x - 0x34), (s16)(arg0->y.word - 8), getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x8B);
        sprintf(buffer, gRaceUiScorePopupPointsFormat, arg0->parent);
        for (i = 0; i != 4; i++) {
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWithExplicitPalette((s16)(arg0->x + (i * 8) + 4), (s16)(arg0->y.word - 8),
                              getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                              buffer[i] - 5, 0xE);
            }
        }
        drawAssetTableSpriteWithExplicitPalette((s16)(arg0->x + (i * 8) + 4), (s16)(arg0->y.word - 8),
                      getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37, 0xE);
    }
}

void updateRaceUiScorePopupSlideOut(RaceUiPopupActor *arg0) {
    arg0->x += arg0->velocity;
    arg0->velocity += 4;
    if (arg0->velocity == 0x38) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceUiScorePopup, arg0);
    }
}

void updateRaceUiScorePopupHold(RaceUiPopupActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiScorePopupSlideOut);
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceUiScorePopup, arg0);
}

void updateRaceUiScorePopupSlideIn(RaceUiPopupActor *arg0) {
    arg0->x -= arg0->velocity;
    arg0->velocity -= 4;
    if (arg0->velocity == 0) {
        arg0->timer = 0x2D;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiScorePopupHold);
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceUiScorePopup, arg0);
}

void initRaceUiScorePopup(RaceUiPopupActor *arg0) {
    if (gViewportStates[arg0->playerIndex].active != 0) {
        if (gPlayerCount == 1) {
            arg0->y.word = -0x24;
        } else {
            arg0->y.word = -0x16;
        }
        arg0->velocity = 0x38;
        arg0->x = 0x1A4;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiScorePopupSlideIn);
    } else {
        removeCallbackTask(arg0);
    }
}

void spawnRaceUiScorePopup(void *arg0, s16 arg1) {
    RaceUiPopupActor *temp = createCallbackTask((CallbackTaskCallback)initRaceUiScorePopup, 0, 0x64);
    if (temp != NULL) {
        temp->parent = arg0;
        temp->playerIndex = arg1;
    }
}

void renderRaceCourseTripleParticle(RaceUiTripleParticleActor *arg0) {
    s16 unused;
    RaceUiTrailCopyBlock spAC;
    RaceUiTrailCopyBlock sp8C;
    RaceUiTrailCopyBlock sp6C;
    s32 sine;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            makeFixedRotationY(spAC.halfwords, arg0->rotY);
            spAC.words[5] = arg0->pos.x;
            spAC.words[6] = arg0->pos.y;
            spAC.words[7] = arg0->pos.z;

            sp8C = sp6C = spAC;

            sine = fixedSine((s16)(arg0->rotY << 4)) << 7;
            sp8C.words[6] = (sp8C.words[6] - sine) + 0x80000;
            sp6C.words[6] += sine + 0x80000;

            arg0->matrix0 = allocFixedTransformMatrix(&spAC);
            arg0->matrix1 = allocFixedTransformMatrix(&sp8C);
            arg0->matrix2 = allocFixedTransformMatrix(&sp6C);
        }

        if ((arg0->matrix0 != NULL) && (arg0->matrix1 != NULL) && (arg0->matrix2 != NULL)) {
            gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0x8)));
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0x9)));
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &_ADDR_200C1C8_VRAM);
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix1, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &_ADDR_200C6A0_VRAM);
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix2, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &_ADDR_200C7D8_VRAM);
        }
    }
}

void updateRaceCourseTripleParticle(RaceUiTripleParticleActor *actor) {
    actor->rotY += 4;
    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)renderRaceCourseTripleParticle, (void *)actor);
}

void initRaceCourseTripleParticle(RaceUiTripleParticleActor *actor) {
    actor->rotY = 0;
    actor->pos = D_800D6220[actor->index];
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceCourseTripleParticle);
}

void func_80061484(RaceUiRankTrailActor *arg0) {
    volatile u8 pad[0x20];
    RaceUiDisplayCommand *unused;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
    }

    if (arg0->matrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &_ADDR_20019C0_VRAM);
    }
}

void func_800615BC(RaceUiRankTrailActor *arg0) {
    RacePlayer *player;
    s32 i;

    arg0->copyBlock = (&gRacePlayers[arg0->playerIndex])->copyBlock94;
    arg0->copyBlock.halfwords[0] = arg0->copyBlock.halfwords[0] / 4;
    arg0->copyBlock.halfwords[1] = arg0->copyBlock.halfwords[1] / 4;
    arg0->copyBlock.halfwords[2] = arg0->copyBlock.halfwords[2] / 4;
    arg0->copyBlock.halfwords[3] = arg0->copyBlock.halfwords[3] / 4;
    arg0->copyBlock.halfwords[4] = arg0->copyBlock.halfwords[4] / 4;
    arg0->copyBlock.halfwords[5] = arg0->copyBlock.halfwords[5] / 4;
    arg0->copyBlock.halfwords[6] = arg0->copyBlock.halfwords[6] / 4;
    arg0->copyBlock.halfwords[7] = arg0->copyBlock.halfwords[7] / 4;
    arg0->copyBlock.halfwords[8] = arg0->copyBlock.halfwords[8] / 4;

    player = &gRacePlayers[arg0->playerIndex];
    if (player->stateFlags & 0x400000) {
        addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)func_80061484, (void *)arg0);
        return;
    }

    enqueuePositionalSoundEffect(0x11, &player->pos, 0x7F, 0x32);

    for (i = 0; i < 8; i++) {
        spawnRaceUiAltBurstTextParticle((void *)arg0->copyBlock.words[5], (void *)arg0->copyBlock.words[6],
                                        (void *)arg0->copyBlock.words[7], i);
    }

    removeCallbackTask(arg0);
}

void initRaceUiHeavyKnockdownTrailEffect(void *arg0) {
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_800615BC);
}

void func_800617EC(RaceUiRisingTrailActor *arg0) {
    volatile s32 padlow;
    s32 sine;
    RaceUiTrailCopyBlock sp80;
    volatile s32 pad[0x12];

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            sine = fixedSine(arg0->sineAngle);
            makeFixedRotationY(sp80.halfwords, (s16)(arg0->angle + 0x800));
            sp80.words[5] = arg0->pos.x;
            sp80.words[6] = arg0->pos.y + ((sine + 0x1000) << 5) + 0x10000;
            sp80.words[7] = arg0->pos.z;
            scaleFixedMatrix3sByQuarter(sp80.halfwords);
            arg0->matrix = allocFixedTransformMatrix(&sp80);
        }

        if (arg0->matrix != NULL) {
            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
            gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
            gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gRegionAllocPtr++, &_ADDR_20019C0_VRAM);
        }
    }
}

void func_80061984(RaceUiThrownTrailActor *arg0) {
    volatile s32 pad[4];
    volatile s16 unused0;
    volatile s16 unused1;
    Vec3i *pos;

    if (gRaceUpdatePaused == 0) {
        pos = &arg0->pos;
        if (isPositionNearAnyRaceViewportFocus(pos) != 0) {
            pushRacePlayersOutOfCylinderAndApplyItemHit(pos, 0x1A0000, 0x600000, 2);
            if (arg0->soundTimer == 0) {
                enqueuePositionalSoundEffect(0xD, pos, 0x7F, 0x31);
                createThrownTrailImpactProjectile(arg0->pos.x, arg0->pos.y + 0x700000, arg0->pos.z, arg0->surface, arg0->angle);
                arg0->soundTimer = 0xF;
            } else {
                arg0->soundTimer--;
            }
        }

        arg0->angle += arg0->angleStep;
        if (arg0->targetAngle + 0x140 < arg0->angle) {
            arg0->angleStep = -8;
        }
        if (arg0->angle < arg0->targetAngle - 0x140) {
            arg0->angleStep = 8;
        }
        arg0->spin += 0x40;
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)func_800617EC, (void *)arg0);
}

void func_80061A98(RaceUiThrownTrailActor *arg0) {
    arg0->soundTimer = 0xF;
    arg0->pos.y = getRaceCourseSurfaceHeight(arg0->surface, arg0->pos.x, arg0->pos.z);
    arg0->angleStep = 8;
    arg0->angle = arg0->angleStep;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_80061984);
}

void func_80061AF4(s16 arg0, void *arg1, void *arg2, s16 arg3) {
    RaceUiThrownTrailActor *temp = createCallbackTask((CallbackTaskCallback)func_80061A98, 0, 0x64);
    if (temp != NULL) {
        temp->pos.x = (s32)arg1;
        temp->pos.z = (s32)arg2;
        temp->targetAngle = (arg3 + 0x800) & 0xFFF;
        temp->surface = arg0;
    }
}

void renderRaceUiSingleTrailEffect(RaceUiSingleTrailActor *arg0) {
    volatile u8 pad[0x20];
    RaceUiDisplayCommand *unused;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
    }

    if (arg0->matrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &_ADDR_2003538_VRAM);
    }
}

void updateRaceUiSingleTrailEffect(RaceUiSingleTrailActor *arg0) {
    composeFixedTransforms(&arg0->localTransform, &gRacePlayers[arg0->playerIndex].renderTransform, &arg0->copyBlock.transform);

    if (gRaceUpdatePaused == 0) {
        arg0->timer--;
        if (!(gRacePlayers[arg0->playerIndex].stateFlags & 0x2000)) {
            arg0->timer = 0;
        }
    }

    if (arg0->timer == 0) {
        removeCallbackTask(arg0);
        return;
    }

    addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)renderRaceUiSingleTrailEffect, (void *)arg0);
}

void initRaceUiSingleTrailEffect(RaceUiSingleTrailActor *arg0) {
    arg0->timer = RACE_UI_SINGLE_TRAIL_TIMER;
    makeFixedRotationY(arg0->localTransform.rotation, RACE_UI_SINGLE_TRAIL_LOCAL_ROT_Y);
    arg0->localTransform.translation.x = RACE_UI_SINGLE_TRAIL_LOCAL_X;
    arg0->localTransform.translation.y = RACE_UI_SINGLE_TRAIL_LOCAL_Y;
    arg0->localTransform.translation.z = RACE_UI_SINGLE_TRAIL_LOCAL_Z;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiSingleTrailEffect);
}

void func_80061DE8(RaceUiFadingTrailActor *arg0) {
    volatile u8 pad[0x20];
    RaceUiDisplayCommand *unused;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
    }

    if (arg0->matrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetPrimColor(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->alpha);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &_ADDR_20035F8_VRAM);
    }
}

void func_80061F38(RaceUiFadingImpactActor *arg0) {
    volatile u8 pad[0x28];
    s16 scale;
    Vec3i *sp24;
    s32 sp2C;
    RaceUiFadingImpactActor *new_var;

    if (gRaceUpdatePaused == 0) {
        arg0->angle += 0x100;
        makeFixedRotationY(arg0->copyBlock.halfwords, arg0->angle);

        scale = arg0->scale;
        arg0->copyBlock.halfwords[0] = (arg0->copyBlock.halfwords[0] * scale) / 64;
        arg0->copyBlock.halfwords[1] = (arg0->copyBlock.halfwords[1] * scale) / 64;
        arg0->copyBlock.halfwords[2] = (arg0->copyBlock.halfwords[2] * scale) / 64;
        new_var = arg0;
        arg0->copyBlock.halfwords[3] = (arg0->copyBlock.halfwords[3] * scale) / 64;
        new_var->copyBlock.halfwords[4] = (new_var->copyBlock.halfwords[4] * scale) / 64;
        arg0->copyBlock.halfwords[5] = (new_var->copyBlock.halfwords[5] * scale) / 64;
        new_var->copyBlock.halfwords[6] = (new_var->copyBlock.halfwords[6] * scale) / 64;
        arg0->copyBlock.halfwords[7] = (arg0->copyBlock.halfwords[7] * scale) / 64;
        new_var->copyBlock.halfwords[8] = (arg0->copyBlock.halfwords[8] * scale) / 64;

        if (scale != 0x34) {
            new_var->scale = scale + 4;
            scale = new_var->scale;
        }

        if (scale >= 0x21) {
            arg0->alpha -= 0x10;
        }

        if (arg0->alpha >= 0x71) {
            sp24 = &arg0->pos;
            sp2C = (new_var->scale * 0x480000) / 64;
            if (tryApplyRacePlayerItemHit(sp24, sp2C, 0x80, 0) != 0) {
                gRacePlayers[0].hitSourcePlayerIndex = new_var->playerIndex;
            }
            if (tryApplyRacePlayerItemHit(sp24, sp2C, 0x80, 1) != 0) {
                gRacePlayers[1].hitSourcePlayerIndex = new_var->playerIndex;
            }
            if (tryApplyRacePlayerItemHit(sp24, sp2C, 0x80, 2) != 0) {
                gRacePlayers[2].hitSourcePlayerIndex = arg0->playerIndex;
            }
            if (tryApplyRacePlayerItemHit(sp24, sp2C, 0x80, 3) != 0) {
                gRacePlayers[3].hitSourcePlayerIndex = arg0->playerIndex;
            }
        }
    }

    if (arg0->alpha <= 0) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&D_801248EC, (RenderCallback)func_80061DE8, (void *)arg0);
    }
}

void initRaceUiFadingImpact(RaceUiFadingImpactActor *actor) {
    actor->scale = 4;
    actor->alpha = 0xFF;
    actor->angle = 0;
    actor->copyBlock.words[5] = actor->pos.x;
    actor->copyBlock.words[6] = actor->pos.y;
    actor->copyBlock.words[7] = actor->pos.z;
    enqueuePositionalSoundEffect(0x13, &actor->pos, 0x7F, 0x32);
    setCallbackTaskCallback(actor, (CallbackTaskCallback)func_80061F38);
}

void spawnRaceUiFadingImpact(void *x, void *y, void *z) {
    RaceUiFadingImpactActor *actor = createCallbackTaskWithUserIdPreservingArgs(initRaceUiFadingImpact, 0, 3);
    if (actor != NULL) {
        actor->pos.x = (s32)x;
        actor->pos.y = (s32)y;
        actor->pos.z = (s32)z;
    }
}

void func_800622B0(RaceUiTransitionRenderActor *arg0) {
    volatile u8 pad[0x20];
    RaceUiDisplayCommand *unused;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
    }

    if (arg0->matrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &_ADDR_2002490_VRAM);
    }
}

void func_800623E8(RaceUiTransitionActor *arg0) {
    RacePlayer *player;
    s16 temp_v0;
    s16 temp_v1;
    s16 temp_a0;

    if (gRaceUpdatePaused == 0) {
        if (!(gRacePlayers[arg0->index].stateFlags & 0x800000)) {
            arg0->unk6C = 0;
        }
        if (arg0->unk6C != 0) {
            player = &gRacePlayers[arg0->index];
            arg0->transformSource.source.words[5] = player->posC8.x;
            arg0->transformSource.source.words[6] = player->posC8.y;
            arg0->transformSource.source.words[7] = player->posC8.z;
        } else {
            if (arg0->unk6E == 0) {
                removeCallbackTask(arg0);
                return;
            }
            temp_v0 = arg0->transformSource.source.halfwords[1];
            temp_v1 = arg0->transformSource.source.halfwords[4];
            temp_a0 = arg0->transformSource.source.halfwords[7];
            arg0->unk68 -= 0x1000;
            arg0->transformSource.source.words[6] += arg0->unk68;
            arg0->transformSource.source.halfwords[1] = temp_v0 - (temp_v0 / 16);
            arg0->transformSource.source.halfwords[4] = temp_v1 - (temp_v1 / 16);
            arg0->transformSource.source.halfwords[7] = temp_a0 - (temp_a0 / 16);
        }
    }
    addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)func_800622B0, arg0);
}

void initRaceUiSpinHitTransitionEffect(RaceUiTransitionActor *arg0) {
    s16 *transform = arg0->transformSource.source.halfwords;
    volatile u8 padding[0x20];

    arg0->unk68 = 0;
    makeFixedRotationX(transform, 0x400);
    scaleFixedMatrix3sByQuarter(transform);
    arg0->unk6C = 1;
    arg0->unk6E = 0xF;
    enqueuePositionalSoundEffect(0xB, &gRacePlayers[arg0->index].pos, 0x7F, 0x32);
    func_800623E8(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_800623E8);
}

void func_800625D8(RaceUiOrbitingSpriteActor *arg0) {
    volatile u8 padding[4];
    RaceUiTrailCopyBlock sp64;
    Gfx *temp_v0;
    Gfx *temp_v0_2;
    Gfx *temp_v0_3;
    Gfx *temp_v0_4;
    Gfx *temp_v0_5;
    Gfx *temp_v0_6;
    Gfx *temp_v0_7;
    Gfx *temp_v0_8;
    Gfx *temp_v0_9;
    Gfx *temp_v0_10;
    Gfx *temp_v0_11;
    Gfx *temp_v0_12;
    Gfx *temp_v0_13;
    Gfx *temp_v0_14;
    Gfx *temp_v0_15;
    Gfx *temp_v0_16;
    Gfx *temp_v0_17;
    Gfx *temp_v0_18;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        sp64.transform = gIdentityFixedTransform;
        sp64.transform.translation.x = arg0->pos.x;
        sp64.transform.translation.y = arg0->pos.y;
        sp64.transform.translation.z = arg0->pos.z;
        arg0->matrix = allocFixedTransformMatrix(&sp64);
    }

 do { if (arg0->matrix != NULL) { temp_v0 = gRegionAllocPtr++; temp_v0->words.w0 = 0x06000000; temp_v0->words.w1 = (u32) gAlphaSpriteRenderModeDl; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x0703F800; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400200; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0003C03C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; temp_v0_15 = gRegionAllocPtr++; temp_v0_15->words.w0 = 0x01020040; temp_v0_15->words.w1 = (u32) arg0->matrix; temp_v0_16 = gRegionAllocPtr++; temp_v0_16->words.w0 = 0x01000040; temp_v0_16->words.w1 = gViewportMatrix; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400103F; temp_v0_17->words.w1 = (u32) D_800D6230; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; } } while (0);
}

void func_800628DC(RaceUiOrbitingSpriteActor *arg0) {
    RacePlayer *player;

    player = &gRacePlayers[arg0->playerIndex];
    if (!(player->stateFlags & 0x10000)) {
        removeCallbackTask(arg0);
        return;
    }

    transformVec3iByFixedMatrix(player->renderTransform.rotation, &D_800D62A0, &arg0->pos);
    player = &gRacePlayers[arg0->playerIndex];
    arg0->pos.x += player->renderTransform.translation.x;
    arg0->pos.y += player->renderTransform.translation.y + 0x80000;
    arg0->pos.z += player->renderTransform.translation.z;
    arg0->angle += 0xC0;
    arg0->pos.x -= fixedSine(arg0->angle) << 7;
    arg0->pos.z += fixedCosine(arg0->angle) << 7;
    addRenderCallback(&D_801248EC, (RenderCallback)func_800625D8, (void *)arg0);
}

void func_80062A10(RaceUiOrbitingSpriteActor *arg0) {
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)), 0x35, &arg0->image, &arg0->palette);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_800628DC);
}

void spawnRaceUiStunOrbitingIcons(s16 playerIndex) {
    RaceUiOrbitingSpriteActor *icon;

    icon = createCallbackTaskPreservingArgs((CallbackTaskCallback)func_80062A10, 0, 4);
    if (icon != NULL) {
        icon->playerIndex = playerIndex;
        icon->angle = RACE_UI_STUN_ORBIT_ANGLE_0;
    }
    icon = createCallbackTaskPreservingArgs((CallbackTaskCallback)func_80062A10, 0, 4);
    if (icon != NULL) {
        icon->playerIndex = playerIndex;
        icon->angle = RACE_UI_STUN_ORBIT_ANGLE_1;
    }
    icon = createCallbackTaskPreservingArgs((CallbackTaskCallback)func_80062A10, 0, 4);
    if (icon != NULL) {
        icon->playerIndex = playerIndex;
        icon->angle = RACE_UI_STUN_ORBIT_ANGLE_2;
    }
}

void renderIceCourseBumper(RaceUiScaledParticleActor *arg0) {
    FixedMatrix3sWideScratch scratch;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            makeFixedRotationY(scratch, arg0->rotY);
            scratch[0] = SCALE_MATRIX_COMPONENT(scratch[0], arg0->scale);
            scratch[3] = SCALE_MATRIX_COMPONENT(scratch[3], arg0->scale);
            scratch[6] = SCALE_MATRIX_COMPONENT(scratch[6], arg0->scale);
            scratch[2] = SCALE_MATRIX_COMPONENT(scratch[2], arg0->scale);
            scratch[5] = SCALE_MATRIX_COMPONENT(scratch[5], arg0->scale);
            scratch[8] = SCALE_MATRIX_COMPONENT(scratch[8], arg0->scale);
            ((RaceUiTrailCopyBlock *)scratch)->words[5] = arg0->pos.x;
            ((RaceUiTrailCopyBlock *)scratch)->words[6] = arg0->pos.y;
            ((RaceUiTrailCopyBlock *)scratch)->words[7] = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix((RaceUiTrailCopyBlock *)scratch);
        }

        if (arg0->matrix != NULL) {
            gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0x8)));
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0x9)));
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &_ADDR_200C910_VRAM);
        }
    }
}

void updateIceCourseBumper(RaceUiScaledParticleActor *arg0) {
    RaceUiScaledParticleActor *actor;
    Vec3i *pos;
    s16 scale;

    actor = arg0;
    if (gRaceUpdatePaused == 0) {
        pos = &actor->pos;
        if (isPositionNearAnyRaceViewportFocus(pos) != 0) {
            actor->rotY += actor->rotYStep;

            if (isRacePlayerInsideCylinder(pos, 0x200000, 0xF0000, 0) != 0) {
                actor->scale = 0x1800;
                enqueuePositionalSoundEffect(0x67, pos, 0x7F, 0x32);
                pushRacePlayerOutOfCylinder(pos, 0x2C0000, 0xF0000, 0);
            }
            if (isRacePlayerInsideCylinder(pos, 0x200000, 0xF0000, 1) != 0) {
                actor->scale = 0x1800;
                enqueuePositionalSoundEffect(0x67, pos, 0x7F, 0x32);
                pushRacePlayerOutOfCylinder(pos, 0x2C0000, 0xF0000, 1);
            }
            if (isRacePlayerInsideCylinder(pos, 0x200000, 0xF0000, 2) != 0) {
                actor->scale = 0x1800;
                enqueuePositionalSoundEffect(0x67, pos, 0x7F, 0x32);
                pushRacePlayerOutOfCylinder(pos, 0x2C0000, 0xF0000, 2);
            }
            if (isRacePlayerInsideCylinder(pos, 0x200000, 0xF0000, 3) != 0) {
                actor->scale = 0x1800;
                enqueuePositionalSoundEffect(0x67, pos, 0x7F, 0x32);
                pushRacePlayerOutOfCylinder(pos, 0x2C0000, 0xF0000, 3);
            }

            scale = actor->scale;
            if (scale != 0x1000) {
                actor->scale = scale - 0x100;
            }
        }
    }
    addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)renderIceCourseBumper, actor);
}

void initIceCourseBumper(RaceUiScaledParticleActor *bumper) {
    bumper->rotY = randomNextSecondary() << 4;
    if (randomNextSecondary() & 1) {
        bumper->rotYStep = ICE_COURSE_BUMPER_ROTATION_STEP;
    } else {
        bumper->rotYStep = -ICE_COURSE_BUMPER_ROTATION_STEP;
    }
    bumper->scale = ICE_COURSE_BUMPER_BASE_SCALE;
    bumper->pos = gIceCourseBumperPositions[bumper->index];
    setCallbackTaskCallback(bumper, (CallbackTaskCallback)updateIceCourseBumper);
}

void func_80062F6C(RaceUiTrailingParticleActor *arg0) {
    FixedMatrix3sWideScratch scratch;
    Vec3i transformedOffset;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            makeFixedRotationY(scratch, arg0->rotY);
            ((RaceUiTrailCopyBlock *)scratch)->words[5] = arg0->pos.x;
            ((RaceUiTrailCopyBlock *)scratch)->words[6] = arg0->pos.y;
            ((RaceUiTrailCopyBlock *)scratch)->words[7] = arg0->pos.z;
            arg0->matrix0 = allocFixedTransformMatrix((RaceUiTrailCopyBlock *)scratch);

            transformVec3iByFixedMatrix(scratch, &D_800D6324, &transformedOffset);
            ((RaceUiTrailCopyBlock *)scratch)->words[5] += transformedOffset.x;
            ((RaceUiTrailCopyBlock *)scratch)->words[6] += transformedOffset.y;
            ((RaceUiTrailCopyBlock *)scratch)->words[7] += transformedOffset.z;
            makeFixedRotationZY(scratch, arg0->rotY, arg0->rotX);
            arg0->matrix1 = allocFixedTransformMatrix((RaceUiTrailCopyBlock *)scratch);
        }

        if (arg0->matrix1 != NULL) {
            gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0x8)));
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0x9)));
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &_ADDR_200CE48_VRAM);
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix1, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &_ADDR_200CC20_VRAM);
        }
    }
}

void func_80063164(RaceUiTrailingParticleActor *arg0) {
    if (gRaceUpdatePaused == 0) {
        arg0->rotX += 8;
    }
    addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)func_80062F6C, (void *)arg0);
}

void func_800631B0(RaceUiTrailingParticleActor *arg0) {
    arg0->pos = D_800D6330[arg0->index];
    arg0->rotY = D_800D633C[arg0->index];
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_80063164);
}

void renderRaceCourseSpinningObject(RaceUiSpinningParticleActor *arg0) {
    FixedMatrix3sWideScratch scratch;
    s32 temp2;
    s32 pad;
    s32 pad2;
    SplitWord temp;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) == 0) {
        return;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        makeFixedRotationY(scratch, arg0->rotY);
        ((RaceUiTrailCopyBlock *)scratch)->words[5] = arg0->pos.x;
        ((RaceUiTrailCopyBlock *)scratch)->words[6] = arg0->pos.y;
        ((RaceUiTrailCopyBlock *)scratch)->words[7] = arg0->pos.z;
        arg0->matrix0 = allocFixedTransformMatrix((RaceUiTrailCopyBlock *)scratch);

        ((RaceUiTrailCopyBlock *)scratch)->words[6] += 0x01000000;
        temp.half.lo = fixedSine(arg0->rotX) >> 5;
        temp2 = fixedSine(arg0->rotX2) >> 5;
        makeFixedRotationYZX(scratch, temp.half.lo, arg0->rotZ, temp2);
        arg0->matrix1 = allocFixedTransformMatrix((RaceUiTrailCopyBlock *)scratch);
    }

    if (arg0->matrix1 != NULL) {
        gDPPipeSync(gRegionAllocPtr++);
        gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0x8)));
        gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0x9)));
        gSPMatrix(gRegionAllocPtr++, arg0->matrix0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, &_ADDR_200D3A8_VRAM);
        gSPMatrix(gRegionAllocPtr++, arg0->matrix1, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, &_ADDR_200CFB0_VRAM);
    }
}

void updateRaceCourseSpinningObject(RaceUiSpinningParticleActor *arg0) {
    if (gRaceUpdatePaused == 0) {
        arg0->rotZ += 0x60;
        arg0->rotX += 0x10;
        arg0->rotX2 += 4;
    }
    addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)renderRaceCourseSpinningObject, arg0);
}

void initRaceCourseSpinningObject(RaceUiSpinningParticleActor *arg0) {
    arg0->pos = D_800D6340[arg0->index];
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceCourseSpinningObject);
}

void renderCourseStartFinishSprite(RaceUiCourseSpriteActor *arg0) {
    volatile u8 padding[4];
    s32 imageIndex;
    RaceUiCourseSpriteActor *new_var;

    new_var = arg0;

    if (gRenderMatricesDirty != 0) {
        new_var->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera((Vec3i *)&new_var->x) != 0) {
        if (arg0->matrixDirty != 0) {
            new_var->matrixDirty = 0;
            new_var->vertices = allocMenuRenderScratch(0x40);
            if (new_var->vertices != NULL) {
                new_var->vertices[0] = D_800D6350[arg0->index * 4];
                arg0->vertices[0].v.tc[0] += arg0->angle << 5;
                new_var->vertices[1] = D_800D6350[new_var->index * 4 + 1];
                arg0->vertices[1].v.tc[0] += new_var->angle << 5;
                new_var->vertices[2] = D_800D6350[new_var->index * 4 + 2];
                arg0->vertices[2].v.tc[0] += arg0->angle << 5;
                new_var->vertices[3] = D_800D6350[new_var->index * 4 + 3];
                arg0->vertices[3].v.tc[0] += new_var->angle << 5;
            }
        }

        if (new_var->vertices != NULL) {
            imageIndex = 0;
            if ((gRacePlayers[gCurrentViewportIndex].lapDigit + 1) >= gRaceLapCount) {
                imageIndex = 1;
            }

            gSPDisplayList(gRegionAllocPtr++, gAlphaSpriteRenderModeDl);
            gSPMatrix(gRegionAllocPtr++, &gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, new_var->palettes[imageIndex]);
            gDPLoadTextureBlock_4b(gRegionAllocPtr++, new_var->images[imageIndex], G_IM_FMT_CI, 64, 16, 0,
                                   G_TX_WRAP, G_TX_CLAMP, 6, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            gSPVertex(gRegionAllocPtr++, new_var->vertices, 4, 0);
            gSP1Quadrangle(gRegionAllocPtr++, 3, 2, 1, 0, 0);
            gSP1Quadrangle(gRegionAllocPtr++, 1, 2, 3, 0, 0);
        }
    }
}

void updateCourseStartFinishSprite(RaceUiCourseSpriteActor *actor) {
    if ((gFrameCounter & COURSE_START_FINISH_SCROLL_FRAME_MASK) == 0) {
        actor->angle = (actor->angle + COURSE_START_FINISH_SCROLL_STEP) & COURSE_START_FINISH_SCROLL_MASK;
    }
    addRenderCallback(&D_801248C8, (RenderCallback)renderCourseStartFinishSprite, actor);
}

void initCourseStartFinishSprite(RaceUiCourseSpriteActor *actor) {
    s32 unused;
    s16 angle;

    switch (actor->index) {
        case 0:
            getRaceCourseSurfaceSpawnTransform(0, &actor->x, &actor->y, &actor->z, &angle);
            getAssetTableImageAndPalette(getRelocatableHeapBlockBase(ASSET_HANDLE(0x1D)), 0, &actor->images[0], &actor->palettes[0]);
            actor->images[1] = actor->images[0];
            actor->palettes[1] = actor->palettes[0];
            break;
        case 1:
            getRaceCourseSurfaceSpawnTransform(gRaceCourseStartEntries[gRaceCourseIndex.signedValue].pathIndex, &actor->x, &actor->y, &actor->z, &angle);
            getAssetTableImageAndPalette(getRelocatableHeapBlockBase(ASSET_HANDLE(0x1D)), 1, &actor->images[0], &actor->palettes[0]);
            getAssetTableImageAndPalette(getRelocatableHeapBlockBase(ASSET_HANDLE(0x1D)), 2, &actor->images[1], &actor->palettes[1]);
            break;
    }
    unused = 0;
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateCourseStartFinishSprite);
}

void func_80063A9C(RaceUiEffectParticleActor *arg0) {
    RaceUiEffectParticleActor *actor;
    s32 cameraX;
    s32 cameraY;
    s32 cameraZ;
    RaceUiAlignedTrailCopyBlock transform;
    s32 i;
    RaceUiGfxCommandDest *matrix;

    actor = arg0;
    gDPLoadTextureBlock_4b(gRegionAllocPtr++, actor->unk20, G_IM_FMT_CI, 32, 32, 0,
                           G_TX_CLAMP, G_TX_CLAMP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, actor->unk1C);
    gSPDisplayList(gRegionAllocPtr++, gAlphaSpriteRenderModeDl);

    transform.source.transform = gIdentityFixedTransform;
    cameraX = -D_801121E0[gCurrentViewportIndex].transformOffset.x;
    cameraY = -D_801121E0[gCurrentViewportIndex].transformOffset.y;
    cameraZ = -D_801121E0[gCurrentViewportIndex].transformOffset.z;

    i = 0;
    if (actor->count > 0) {
        do {
            transform.source.transform.translation.x =
                ((actor->particles[i].unk0 - (cameraX & 0xFFFFFF)) & 0xFFFFFF) + cameraX + 0xFF800000;
            transform.source.transform.translation.y =
                ((actor->particles[i].unk4 - (cameraY & 0xFFFFFF)) & 0xFFFFFF) + cameraY + 0xFF800000;
            transform.source.transform.translation.z =
                ((actor->particles[i].unk8 - (cameraZ & 0xFFFFFF)) & 0xFFFFFF) + cameraZ + 0xFF800000;
            matrix = allocFixedTransformMatrix(&transform.source);
            if (matrix != NULL) {
                gSPMatrix(gRegionAllocPtr++, matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPVertex(gRegionAllocPtr++, D_800D63D0, 3, 0);
                gSP1Triangle(gRegionAllocPtr++, 0, 2, 1, 0);
                actor = arg0;
            }
            i++;
        } while (i < actor->count);
    }
}

void func_80063E70(RaceUiEffectParticleActor *arg0) {
    register RaceUiEffectParticleActor *actor;
    register s32 i;

    actor = arg0;
    for (i = 0; i < actor->count; i++) {
        actor->particles[i].unk0 += 0xA0000;
        if (actor->particles[i].unk0 >= 0x1000000) {
            actor->particles[i].unk8 = randomNextMain() << 0x10;
            actor->particles[i].unk4 = randomNextMain() << 0x10;
            actor->particles[i].unk0 = 0;
        }
        actor->particles[i].unk4 += 0xFFFD0000;
        actor->particles[i].unk4 &= 0xFFFFFF;
    }
    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)func_80063A9C, (void *)actor);
}

void func_80063FC0(RaceUiEffectParticleActor *actor) {
    s32 i;

    if (gPlayerCount != 0) {
        actor->count = 0xF;
    } else {
        actor->count = 0x3C;
    }

    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x42, &actor->unk20, &actor->unk1C);
    gAssetHandles[0x25] = allocRelocatableHeapBlock(actor->count * sizeof(RaceUiEffectParticle));
    actor->particles = (RaceUiEffectParticle *)getRelocatableHeapBlockBase(gAssetHandles[0x25]);

    for (i = 0; i < actor->count; i++) {
        actor->particles[i].unk0 = randomNextMain() << 0x10;
        actor->particles[i].unk4 = randomNextMain() << 0x10;
        actor->particles[i].unk8 = randomNextMain() << 0x10;
    }

    setCallbackTaskCallback(actor, (CallbackTaskCallback)func_80063E70);
}

void renderRaceCourseRankModel(RaceUiRankParticleActor *arg0) {
    RaceUiDisplayCommand *unused;
    s32 displayListIndex;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
        }

        if (arg0->matrix != NULL) {
            displayListIndex = 0;
            if (gRaceLapCount <= gRacePlayers[gCurrentViewportIndex].lapDigit + 1) {
                displayListIndex = 1;
            }

            gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0x8)));
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0x9)));
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->displayLists[displayListIndex]);
        }
    }
}

void updateRaceCourseRankModel(RaceUiRankParticleActor *arg0) {
    addRenderCallback(&D_801248C8, (RenderCallback)renderRaceCourseRankModel, arg0);
}

void initRaceCourseRankModel(RaceUiRankParticleActor *actor) {
    struct {
        s16 pad;
        s16 angle;
        s32 unused;
    } local;

    switch (actor->index) {
    case 0:
        getRaceCourseSurfaceSpawnTransform(1, &actor->pos.x, &actor->pos.y, &actor->pos.z, &local.angle);
        break;
    case 1:
        getRaceCourseSurfaceSpawnTransform(gRaceCourseStartEntries[gRaceCourseIndex.signedValue].pathIndex, &actor->pos.x, &actor->pos.y, &actor->pos.z, &local.angle);
        break;
    }

    makeFixedRotationY(actor->copyBlock.halfwords, local.angle);
    actor->copyBlock.transform.translation.x = actor->pos.x;
    actor->copyBlock.transform.translation.y = actor->pos.y;
    actor->copyBlock.transform.translation.z = actor->pos.z;
    actor->displayLists[0] = D_800D6400[(actor->index * 2) + (gRaceCourseIndex.signedValue * 4)];
    actor->displayLists[1] = D_800D6400[(actor->index * 2) + (gRaceCourseIndex.signedValue * 4) + 1];
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceCourseRankModel);
}

void func_800643B4(RaceUiProjectileActor *arg0, u16 *arg1) {
    arg0->animationScript = arg1;
    arg0->frameTimer = arg1[0];
    arg0->flags = arg1[1];
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(ASSET_HANDLE(0x1E)), arg1[2], &arg0->image, &arg0->palette);
}

s32 func_80064414(RaceUiProjectileActor *arg0) {
    s16 temp_v0;

    temp_v0 = arg0->frameTimer;
    if (!(temp_v0 & 0x7FFF)) {
        if (temp_v0 & 0x8000) {
            return 1;
        }
        func_800643B4(arg0, arg0->animationScript + 3);
        goto block_5;
    }
    arg0->frameTimer = temp_v0 - 1;
block_5:
    return 0;
}

void func_80064470(RaceUiProjectileActor *arg0) {
    volatile s32 pad0;
    RaceUiTrailCopyBlock sp7C;
    volatile u8 padding[4];
    s32 sp74;
    Gfx *temp_v0_2;
    Gfx *temp_v0_3;
    Gfx *temp_v0_4;
    Gfx *temp_v0_5;
    Gfx *temp_v0_6;
    Gfx *temp_v0_7;
    Gfx *temp_v0_8;
    Gfx *temp_v0_9;
    Gfx *temp_v0_10;
    Gfx *temp_v0_11;
    Gfx *temp_v0_12;
    Gfx *temp_v0_13;
    Gfx *temp_v0_14;
    Gfx *temp_v0_17;
    Gfx *temp_v0_18;
    Gfx *temp_t4;
    s32 temp_t0;
    s32 var_ra;
    s32 var_t5;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = (float) 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        sp7C.transform = gIdentityFixedTransform;
        sp7C.transform.translation.x = arg0->pos.x;
        sp7C.transform.translation.y = arg0->pos.y;
        sp7C.transform.translation.z = arg0->pos.z;
        arg0->matrix = allocFixedTransformMatrix(&sp7C);
    }

    if (arg0->matrix != NULL) {
        var_ra = 0x20;
        if (arg0->flags & 4) {
            var_ra = 0x40;
            sp74 = 4;
        } else {
            sp74 = 0;
            do { } while (0);
        }
        do { temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0x06000000; temp_v0_2->words.w1 = (u32) gAlphaSpriteRenderModeDl; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xFD500000; temp_v0_3->words.w1 = (u32) arg0->image; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w0 = 0xF5500000; temp_v0_4->words.w1 = 0x07080200; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xE6000000; temp_v0_5->words.w1 = 0; temp_t4 = gRegionAllocPtr++; temp_t4->words.w0 = 0xF3000000; temp_t0 = (((var_ra << 5) + 3) >> 2) - 1; if (temp_t0 < 0x7FF) { var_t5 = temp_t0; } else { var_t5 = 0x7FF; } temp_t4->words.w1 = (((var_t5 & 0xFFF) << 0xC) | 0x07000000) | 0x400; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400400; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = (((var_ra - 1) << 2) & 0xFFF) | 0x0007C000; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x01020040; temp_v0_17->words.w1 = (u32) arg0->matrix; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0x01000040; temp_v0_18->words.w1 = (u32) gViewportMatrix; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0x0400103F; temp_v0_2->words.w1 = (u32) (&D_800D64A0[sp74]); temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xB1060402; temp_v0_3->words.w1 = 0x00060200; } while (0);
    }
}

void func_800647E0(RaceUiProjectileActor *arg0) {
    struct {
        s16 mtx[0x10];
        s32 pad;
    } sp24;
    RaceUiProjectileActor *actor;
    s32 temp;

    actor = arg0;
    if (!gRaceUpdatePaused) {
        makeFixedRotationXY(sp24.mtx, gRacePlayers[actor->index].pitchAngle, gRacePlayers[actor->index].facingAngle);
        transformVec3iByFixedMatrix(sp24.mtx, &actor->velocity, &actor->pos);
        actor->pos.x += gRacePlayers[actor->index].unk28.x;
        actor->pos.y += gRacePlayers[actor->index].unk28.y + actor->verticalVelocity;
        actor->pos.z += gRacePlayers[actor->index].unk28.z;
        temp = (actor->verticalVelocity += actor->verticalAcceleration);
        actor->verticalAcceleration += 0x10000;
        if (temp >= 0x300001) {
            removeCallbackTask(actor);
            return;
        }
    }
    addRenderCallback(&D_801248C8, (RenderCallback)func_80064470, actor);
}

void func_80064914(RaceUiProjectileActor *arg0) {
    RacePlayer *player;
    RacePlayer *otherPlayer;
    s16 sp44[0xE];
    s32 value;
    s32 amount;
    s32 i;
    s16 flags;

    if (gRaceUpdatePaused == 0) {
        player = &gRacePlayers[arg0->index];
        makeFixedRotationXY(sp44, gRacePlayers[arg0->index].pitchAngle, gRacePlayers[arg0->index].facingAngle);
        transformVec3iByFixedMatrix(sp44, &arg0->velocity, &arg0->pos);

        player = &gRacePlayers[arg0->index];
        arg0->pos.x += player->unk28.x;
        arg0->pos.y += player->unk28.y;
        arg0->pos.z += player->unk28.z;
        func_80064414(arg0);

        i = 0;
        do {
            otherPlayer = &gRacePlayers[i];
            if ((otherPlayer->isActive != 0) && (otherPlayer->playerIndexU != arg0->index)) {
                value = otherPlayer->unk568;
                amount = (value >= 0xA6) ? 0xA6 : value;
                otherPlayer->unk568 = value - amount;
                addRacePlayerScore(&gRacePlayers[arg0->index], amount);
            }
            i++;
        } while (i < RACE_PLAYER_COUNT);

        flags = arg0->flags;
        amount = flags & 1;
        if (flags & 8) {
            enqueuePositionalSoundEffect(0x69, &gRacePlayers[arg0->index].unk28, 0x7F, 0x32);
            arg0->flags &= ~8;
            amount = 1;
            amount = (flags = arg0->flags) & amount;
        }
        if (amount) {
            arg0->verticalAcceleration = 0;
            arg0->verticalVelocity = 0;
            enqueuePositionalSoundEffect(0x6A, &gRacePlayers[arg0->index].unk28, 0x7F, 0x32);
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_800647E0);
        }
    }

    addRenderCallback(&D_801248C8, (RenderCallback)func_80064470, arg0);
}

void func_80064B28(RaceUiProjectileActor *arg0) {
    FixedMatrix3sWideScratch sp2C;
    RacePlayer *player;
    RaceUiProjectileActor *actor;

    actor = arg0;
    if (gRaceUpdatePaused == 0) {
        player = &gRacePlayers[actor->index];
        arg0 += 0;
        if (((!actor) && (!actor)) && (!actor)) {
        }
        player = &gRacePlayers[actor->index];
        makeFixedRotationXY(sp2C, player->pitchAngle, player->facingAngle);
        transformVec3iByFixedMatrix(sp2C, &actor->velocity, &actor->pos);

        player = &gRacePlayers[actor->index];
        actor->pos.x += player->unk28.x;
        actor->pos.y += player->unk28.y + actor->verticalVelocity;
        actor->pos.z += player->unk28.z;
        actor->verticalAcceleration += 0xFFFF0000;
        actor->verticalVelocity += actor->verticalAcceleration;
        if (actor->verticalVelocity <= 0) {
            actor->verticalVelocity = 0;
            enqueuePositionalSoundEffect(0x68, &player->unk28, 0x7F, 0x32);
            func_80064414(actor);
            setCallbackTaskCallback(actor, (CallbackTaskCallback)func_80064914);
        }
    }
    addRenderCallback(&D_801248C8, (RenderCallback)func_80064470, actor);
}

void func_80064C68(RaceUiProjectileActor *arg0) {
    FixedMatrix3sWideScratch sp2C;
    RacePlayer *player;
    RaceUiProjectileActor *actor;

    actor = arg0;
    if (gRaceUpdatePaused == 0) {
        makeFixedRotationXY(sp2C, gRacePlayers[actor->index].pitchAngle, gRacePlayers[actor->index].facingAngle);
        transformVec3iByFixedMatrix(sp2C, &actor->velocity, &actor->pos);

        player = &gRacePlayers[actor->index];
        actor->pos.x += player->unk28.x;
        actor->pos.y += player->unk28.y;
        actor->pos.z += player->unk28.z;
        func_80064414(actor);
        if (actor->flags != 0) {
            setCallbackTaskCallback(actor, (CallbackTaskCallback)func_80064B28);
            actor->verticalAcceleration = 0x50000;
            actor->verticalVelocity += 0x50000;
        }
    }
    addRenderCallback(&D_801248C8, (RenderCallback)func_80064470, actor);
}

void func_80064D88(RaceUiProjectileActor *arg0) {
    FixedMatrix3sWideScratch sp2C;
    RacePlayer *player;
    RaceUiProjectileActor *actor;

    actor = arg0;
    if (gRaceUpdatePaused == 0) {
        makeFixedRotationXY(sp2C, gRacePlayers[actor->index].pitchAngle, gRacePlayers[actor->index].facingAngle);
        transformVec3iByFixedMatrix(sp2C, &actor->velocity, &actor->pos);

        player = &gRacePlayers[actor->index];
        actor->pos.x += player->unk28.x;
        actor->pos.y += player->unk28.y + actor->verticalVelocity;
        actor->pos.z += player->unk28.z;
        actor->verticalVelocity += actor->verticalAcceleration;
        actor->verticalAcceleration += 0xFFFF0000;
        if (actor->verticalVelocity <= 0) {
            actor->verticalVelocity = 0;
            setCallbackTaskCallback(actor, (CallbackTaskCallback)func_80064C68);
        }
    }
    addRenderCallback(&D_801248C8, (RenderCallback)func_80064470, actor);
}

void initForwardActionProjectileEffect(RaceUiProjectileActor *arg0) {
    arg0->velocity.x = 0;
    arg0->velocity.y = 0x280000;
    arg0->velocity.z = 0;
    arg0->unk3C = 0;
    arg0->unk4C = 0;
    arg0->verticalVelocity = 0x300000;
    arg0->verticalAcceleration = 0;
    func_800643B4(arg0, D_800D6520);
    enqueuePositionalSoundEffect(0x66, &gRacePlayers[arg0->index].unk28, 0x7F, 0x32);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_80064D88);
}

void drawTimeTrialRecordDeltaPopup(RaceUiTimeTrialRecordDeltaPopupActor *arg0) {
    if (gCurrentViewportIndex == 0) {
        if (arg0->isSlowerThanRecord != 0) {
            drawAssetTableSprite((s16)(arg0->x - 0x66), arg0->y.half.lo, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), 0x98);
            func_80059A04(&arg0->delta, arg0->x + 0x26, arg0->y.word, 0xC);
            return;
        }
        drawAssetTableSprite((s16)(arg0->x - 0x62), arg0->y.half.lo, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), 0x97);
        func_80059A04(&arg0->delta, arg0->x + 0x22, arg0->y.word, 0x10);
    }
}

void updateTimeTrialRecordDeltaPopupSlideOut(RaceUiTimeTrialRecordDeltaPopupActor *arg0) {
    arg0->x -= arg0->velocity;
    arg0->velocity += 4;
    if (arg0->velocity == 0x38) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawTimeTrialRecordDeltaPopup, arg0);
    }
}

void updateTimeTrialRecordDeltaPopupHold(RaceUiTimeTrialRecordDeltaPopupActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateTimeTrialRecordDeltaPopupSlideOut);
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawTimeTrialRecordDeltaPopup, arg0);
}

void updateTimeTrialRecordDeltaPopupSlideIn(RaceUiTimeTrialRecordDeltaPopupActor *arg0) {
    arg0->x -= arg0->velocity;
    arg0->velocity -= 4;
    if (arg0->velocity == 0) {
        arg0->timer = TIME_TRIAL_RECORD_DELTA_POPUP_HOLD_TIMER;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateTimeTrialRecordDeltaPopupHold);
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawTimeTrialRecordDeltaPopup, arg0);
}

void initTimeTrialRecordDeltaPopup(RaceUiTimeTrialRecordDeltaPopupActor *popup) {
    popup->y.word = TIME_TRIAL_RECORD_DELTA_POPUP_START_Y;
    popup->x = TIME_TRIAL_RECORD_DELTA_POPUP_START_X;
    popup->velocity = TIME_TRIAL_RECORD_DELTA_POPUP_START_VELOCITY;
    popup->isSlowerThanRecord =
        calculateRaceTimerDelta(&gRaceElapsedTimer, &RACE_UI_TIME_TRIAL_SAVE_DATA->timeTrialRecordSplitTimes[gRaceCourseIndex.signedValue], &popup->delta);
    setCallbackTaskCallback(popup, (CallbackTaskCallback)updateTimeTrialRecordDeltaPopupSlideIn);
}

void func_800651BC(RaceUiGfxCommandActor *arg0) {
    RaceUiGfxCommandScriptEntry *entry;
    s16 textureIndex;
    s32 i;
    register RaceUiGfxCommandActor *actor;
    u8 padding[1];
    void *image;
    void *palette;

    textureIndex = -1;
    actor = arg0;
    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);

    entry = D_800D693C[gRaceCourseIndex.signedValue];
    i = 0;
    if (entry->sentinel != -1) {
        do {
            if ((entry->active != 0) && (isPositionNearCurrentRaceViewportCamera(&entry->position) != 0)) {
                if (textureIndex != entry->sentinel + actor->textureOffset) {
                    textureIndex = entry->sentinel + actor->textureOffset;
                    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), 0x14, &image, &palette);

                    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, 32, 32, 0, G_TX_CLAMP,
                                           G_TX_CLAMP, 0, 0, 0, 0);
                    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
                }

                gSPMatrix(gRegionAllocPtr++, &actor->particles[i], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPVertex(gRegionAllocPtr++, D_800D6968, 4, 0);
                gSP2Triangles(gRegionAllocPtr++, 3, 2, 1, 0, 3, 1, 0, 0);
            }
            entry++;
            i++;
        } while (entry->sentinel != -1);
    }

    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
}

void updateRaceCourseCoinMarkers(RaceUiGfxCommandActor *arg0) {
    RaceUiGfxCommandScriptEntry *entry;
    Vec3i *pos;
    RaceUiGfxCommandActor *actor;
    register s32 xzSize;
    register s32 ySize;
    register s32 sentinel;

    entry = D_800D693C[gRaceCourseIndex.signedValue];
    actor = arg0;
    if (gFrameCounter & 1) {
        actor->textureOffset++;
    }
    if (actor->textureOffset >= 6) {
        actor->textureOffset = 0;
    }

    sentinel = -1;
    ySize = 0xC0000;
    if (entry->sentinel == sentinel) {
        goto done;
    }
    xzSize = 0x70000;
    pos = &entry->position;

loop:
    if (entry->active != 0) {
        if (isPositionNearAnyRaceViewportFocus(pos) != 0) {
            if (isRacePlayerInsideCylinder(pos, xzSize, ySize, 0) != 0) {
                entry->active = 0;
                enqueuePositionalSoundEffect(0x18, pos, 0x7F, 0x32);
                gRacePlayers[0].unk570++;
                if (gRacePlayers[0].trailEffectTimer != 0) {
                    gRacePlayers[0].trailEffectTimer = 0xF0;
                } else {
                    startSnowboardTrailEffect((struct RacePlayer *)gRacePlayers);
                }
                if (gRacePlayers[0].trailEffectTimer) {
                }
            }
        }
    }

    entry++;
    pos = &entry->position;
    if (entry->sentinel != sentinel) {
        goto loop;
    }

done:
    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)func_800651BC, actor);
}

void initRaceCourseCoinMarkerMatrices(RaceUiGfxCommandActor *arg0) {
    register RaceUiGfxCommandActor *actor1;
    register RaceUiGfxCommandActor *actor2;
    register RaceUiGfxCommandScriptEntry *script;
    register s32 i;
    register s32 offset;
    register s32 one;

    script = D_800D693C[gRaceCourseIndex.signedValue];
    actor1 = arg0;
    actor2 = arg0;
    i = 0;
    if (actor1->count > 0) {
        register RaceUiGfxCommandDest *template;

        template = &gIdentityMatrix;
        offset = 0;
        one = 1;
        do {
            script->active = one;
            actor1->particles[i] = *template;
            setPackedMatrixTranslation(&actor1->particles[i], &script->position);
            i++;
            offset += sizeof(RaceUiGfxCommandDest);
            script++;
        } while (i < actor2->count);
    }
    osWritebackDCache(actor1->particles, actor1->count * sizeof(RaceUiGfxCommandDest));
}

void initRaceCourseCoinMarkers(RaceUiGfxCommandActor *actor) {
    RaceUiGfxCommandScriptEntry *marker;
    s32 markerCount;

    marker = D_800D693C[gRaceCourseIndex.signedValue];
    markerCount = 0;
    if (marker->sentinel != -1) {
        do {
            markerCount++;
            marker++;
        } while (marker->sentinel != -1);
    }
    gRacePlayers[0].courseCoinMarkerCount = markerCount;
    actor->count = markerCount;
    if (markerCount != 0) {
        gAssetHandles[0x24] = allocRelocatableHeapBlock(markerCount * sizeof(RaceUiGfxCommandDest));
        actor->particles = getRelocatableHeapBlockBase(gAssetHandles[0x24]);
        initRaceCourseCoinMarkerMatrices(actor);
        setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceCourseCoinMarkers);
    }
}

void func_80065808(RaceUiOverlayActor *arg0) {
    volatile u8 pad2[0xC];
    RaceUiTrailCopyBlock sp9C;
    RaceUiDisplayCommand *temp_v0;
    RaceUiDisplayCommand *temp_v0_2;
    RaceUiDisplayCommand *temp_v0_3;
    RaceUiDisplayCommand *temp_v0_4;
    RaceUiDisplayCommand *temp_v0_5;
    RaceUiDisplayCommand *temp_v0_6;
    RaceUiDisplayCommand *temp_v0_7;
    RaceUiDisplayCommand *temp_v0_8;
    RaceUiDisplayCommand *temp_v0_9;
    RaceUiDisplayCommand *temp_v0_10;
    RaceUiDisplayCommand *temp_v0_11;
    RaceUiDisplayCommand *temp_v0_12;
    RaceUiDisplayCommand *temp_v0_13;
    RaceUiDisplayCommand *temp_v0_14;
    RaceUiDisplayCommand *temp_v0_15;
    RaceUiDisplayCommand *temp_v0_16;
    RaceUiDisplayCommand *temp_v0_17;
    RaceUiDisplayCommand *temp_v0_18;
    RaceUiDisplayCommand *temp_v0_19;
    RaceUiDisplayCommand *temp_v0_20;
    RaceUiDisplayCommand *temp_v0_21;
    RaceUiDisplayCommand *temp_v0_22;
    RaceUiDisplayCommand *temp_v0_23;
    RaceUiDisplayCommand *temp_v0_24;
    RaceUiDisplayCommand *temp_v0_25;
    RaceUiDisplayCommand *temp_v0_26;
    RaceUiDisplayCommand *temp_v0_27;
    RaceUiDisplayCommand *temp_v0_28;
    RaceUiDisplayCommand *temp_v0_29;
    RaceUiDisplayCommand *temp_v0_30;
    RaceUiDisplayCommand *temp_v0_31;
    RaceUiDisplayCommand *temp_v0_32;

    do {
        if (gRenderMatricesDirty != 0) {
            arg0->matrixDirty = 1;
        }
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            sp9C.transform = gIdentityFixedTransform;
            sp9C.words[5] = arg0->x;
            sp9C.words[6] = arg0->y;
            sp9C.words[7] = arg0->z;
            arg0->matrix = allocFixedTransformMatrix(&sp9C);
        }
        do {
 do { if (arg0->matrix != NULL) { temp_v0 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0->words.w0 = 0x06000000; temp_v0->words.w1 = (u32) gAlphaSpriteRenderModeDl; temp_v0_2 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_2->words.w0 = 0xFD480017; temp_v0_2->words.w1 = (u32) arg0->palette3A; temp_v0_3 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_3->words.w0 = 0xF5480600; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_5->words.w0 = 0xF4000000; temp_v0_5->words.w1 = 0x070600A0; temp_v0_6 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_7->words.w0 = 0xF5400600; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x000C00A0; temp_v0_9 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->image3A; temp_v0_10 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; temp_v0_15 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_15->words.w0 = 0x01020040; temp_v0_15->words.w1 = (u32) arg0->matrix; temp_v0_16 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_16->words.w0 = 0x01000040; temp_v0_16->words.w1 = gViewportMatrix; temp_v0_17 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_17->words.w0 = 0x0400207F; temp_v0_17->words.w1 = (u32) D_800D69A8; temp_v0_18 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; temp_v0_19 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_19->words.w0 = 0xFD480017; temp_v0_19->words.w1 = (u32) arg0->palette3B; temp_v0_20 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_20->words.w0 = 0xF5480600; temp_v0_20->words.w1 = 0x07080200; temp_v0_21 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_21->words.w1 = 0; temp_v0_21->words.w0 = 0xE6000000; temp_v0_22 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_22->words.w0 = 0xF4000000; temp_v0_22->words.w1 = 0x070600A0; temp_v0_23 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_23->words.w1 = 0; temp_v0_23->words.w0 = 0xE7000000; temp_v0_24 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_24->words.w0 = 0xF5400600; temp_v0_24->words.w1 = 0x00080200; temp_v0_25 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_25->words.w0 = 0xF2000000; temp_v0_25->words.w1 = 0x000C00A0; temp_v0_26 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_26->words.w0 = 0xFD100000; temp_v0_26->words.w1 = (u32) arg0->image3B; temp_v0_27 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_27->words.w1 = 0; temp_v0_27->words.w0 = 0xE8000000; temp_v0_28 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_28->words.w0 = 0xF5000100; temp_v0_28->words.w1 = 0x07000000; temp_v0_29 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_29->words.w1 = 0; temp_v0_29->words.w0 = 0xE6000000; temp_v0_30 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_30->words.w0 = 0xF0000000; temp_v0_30->words.w1 = 0x0703C000; temp_v0_31 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_31->words.w1 = 0; temp_v0_31->words.w0 = 0xE7000000; temp_v0_32 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_32->words.w0 = 0xB10E0C0A; temp_v0_32->words.w1 = 0x000E0A08; } } while (0);
        } while (0);
    } while (0);
}

void updateRaceStartOverlayFallOut(RaceUiOverlayActor *arg0) {
    s16 timer;
    RaceUiOverlayActor *actor;

    timer = arg0->timer;
    actor = arg0;
    if (timer != 0) {
        arg0->y += arg0->velocity;
        arg0->velocity += 0x2000;
        arg0->timer = timer - 1;
    } else {
        removeCallbackTask(actor);
        return;
    }
    addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)func_80065808, actor);
}

void func_80065D24(RaceUiOverlayActor *arg0) {
    s16 temp_v0;
    s32 temp_v0_2;
    void *temp_v0_3;
    RaceUiOverlayActor *temp_s0;

    temp_v0 = arg0->timer;
    temp_s0 = arg0;
    if (temp_v0 == 0) {
        if (arg0->assetTimer == 0) {
            enqueuePositionalSoundEffect(9, &temp_s0->x, 0x7F, 0x32);
        }
        temp_v0_3 = getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F));
        getAssetTableImageAndPalette(temp_v0_3, (temp_s0->assetTimer >> 1) + 0x3B,
                                     &temp_s0->palette3B, &temp_s0->image3B);
        temp_s0->assetTimer++;
        if (temp_s0->assetTimer >= 0xB) {
            temp_s0->timer = 0x3C;
            setCallbackTaskCallback(temp_s0, (CallbackTaskCallback)updateRaceStartOverlayFallOut);
        }
    } else {
        temp_s0->timer = temp_v0 - 1;
    }

    temp_v0_2 = temp_s0->velocity;
    temp_s0->y += temp_v0_2;
    temp_s0->velocity = temp_v0_2 + 0xC00;
    addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)func_80065808, temp_s0);
}

void func_80065E0C(RaceUiOverlayActor *arg0) {
    s16 timer = arg0->timer;

    if (timer != 0) {
        arg0->y -= arg0->velocity;
        arg0->velocity -= 0x2000;
        arg0->timer = timer - 1;
    } else {
        arg0->timer = 0x14;
        arg0->assetTimer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_80065D24);
    }

    addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)func_80065808, arg0);
}

void func_80065E90(RaceUiOverlayActor *arg0) {
    s32 i;

    arg0->x = 0;
    arg0->z = gRacePlayers[0].pos.z + 0xFF900000;
    arg0->velocity = 0;
    arg0->y = getRaceCourseSurfaceHeight(findRaceCourseSurfaceFromHint(0, 0, arg0->z), arg0->x, arg0->z) + 0x200000;

    for (i = 0; i < 0x40; i++) {
        arg0->velocity += 0x2000;
        arg0->y += arg0->velocity;
        arg0->timer++;
    }

    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), 0x3A, &arg0->palette3A, &arg0->image3A);
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), 0x3B, &arg0->palette3B, &arg0->image3B);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)func_80065E0C);
}

void renderRaceScoreAttackRings(RaceUiRankTextRenderActor *arg0) {
    RaceUiRankTextRenderEntry *var_s4;
    s32 var_s6;
    s32 var_fp;
    Gfx *temp_s0;
    Gfx *temp_s2;
    Gfx *temp_s3;

    var_s4 = D_800D761C[gRaceCourseIndex.signedValue];
    var_fp = TRUE;
    var_s6 = 0;
    if (var_s4->active != -1) {
        do {
            if ((var_s4->active != 0) && (isPositionNearCurrentRaceViewportCamera(&var_s4->position) != 0)) {
                if (var_fp != 0) {
                    gDPPipeSync(gRegionAllocPtr++);

                    temp_s2 = gRegionAllocPtr++;
                    var_fp = FALSE;
                    gSPSegment(temp_s2, 0x02, getRelocatableHeapBlockBase(gAssetHandles[0xA]));

                    temp_s3 = gRegionAllocPtr++;
                    gSPSegment(temp_s3, 0x03, getRelocatableHeapBlockBase(gAssetHandles[0xB]));
                }

                temp_s0 = gRegionAllocPtr++;
                gSPMatrix(temp_s0, (u32)arg0->matrices + (var_s6 << 6), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                temp_s0 = gRegionAllocPtr++;
                gSPDisplayList(temp_s0, &_ADDR_20019C0_VRAM);
            }
            var_s4++;
            var_s6++;
        } while (var_s4->active != -1);
    }
}

void updateRaceScoreAttackRings(void *arg0) {
    volatile u8 pad[8];
    RaceUiRankTextRenderEntry *entry;
    RaceUiRankTrigger *trigger;
    s32 radius;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 i;

    entry = D_800D761C[gRaceCourseIndex.signedValue];
    if (entry->active != -1) {
        do {
            if (entry->active != 0) {
                pushRacePlayersOutOfCylinderAndApplyItemHit(&entry->position, 0xE0000, 0x100000, 2);
                trigger = gRacePlayers[0].scoreAttackRingTriggerList;
                if (trigger != NULL) {
                    do {
                        if (trigger->triggered == 0) {
                            radius = trigger->radius + 0xE0000;
                            dx = trigger->x - entry->position.x;
                            if ((dx < radius) && (-radius < dx)) {
                                dy = (trigger->y - entry->position.y) + 0xFFF20000;
                                if ((dy < radius) && (-radius < dy)) {
                                    dz = trigger->z - entry->position.z;
                                    if ((dz < radius) && (-radius < dz) &&
                                        (integerSquareRoot64((s64)dx * dx + (s64)dy * dy + (s64)dz * dz) < radius)) {
                                        trigger->triggered = 1;
                                        i = 0;
                                        do {
                                            spawnRaceUiAltBurstTextParticle((void *)entry->position.x,
                                                                           (void *)(entry->position.y + 0x70000),
                                                                           (void *)entry->position.z, i);
                                            i++;
                                        } while (i != 8);
                                        entry->active = 0;
                                        gRacePlayers[0].scoreAttackPointTotal++;
                                        break;
                                    }
                                }
                            }
                        }
                        trigger = trigger->next;
                    } while (trigger != NULL);
                }
            }
            entry++;
        } while (entry->active != -1);
    }

    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)renderRaceScoreAttackRings, arg0);
}

void initRaceScoreAttackRingMatrices(RaceUiRankTextRenderActor *arg0) {
    s16 scratch[0x10];
    RaceUiRankTextRenderEntry *entry;
    RaceUiRankTextRenderActor *actor1;
    RaceUiRankTextRenderActor *actor2;
    s32 i;
    s32 active;
    s16 count;

    actor1 = arg0;
    actor2 = arg0;
    entry = D_800D761C[gRaceCourseIndex.signedValue];
    count = actor1->count;
    i = 0;
    if (count > 0) {
        active = 1;
        do {
            makeFixedRotationY(scratch, entry->angle);
            scratch[0] = scratch[0] / 12;
            scratch[1] = scratch[1] / 12;
            scratch[2] = scratch[2] / 12;
            if ((!i) && (!i)) {
            }
            scratch[3] = scratch[3] / 12;
            scratch[4] = scratch[4] / 12;
            scratch[5] = scratch[5] / 12;
            scratch[6] = scratch[6] / 12;
            scratch[7] = scratch[7] / 12;
            scratch[8] = scratch[8] / 12;
            ((Vec3i *)&scratch[10])->x = entry->position.x;
            ((Vec3i *)&scratch[10])->y = entry->position.y;
            ((Vec3i *)&scratch[10])->z = entry->position.z;
            packFixedTransformMatrix(scratch, (void *)((u32)actor1->matrices + (i << 6)));
            entry->active = active;
            i++;
            entry++;
        } while (i < actor2->count);
        count = actor1->count;
    }
    osWritebackDCache(actor1->matrices, count << 6);
}

void initRaceScoreAttackRings(RaceUiRankTextRenderActor *arg0) {
    RaceUiRankTextRenderEntry *var_v0;
    s32 var_v1;

    var_v0 = D_800D761C[gRaceCourseIndex.signedValue];
    var_v1 = 0;
    if (var_v0->active != -1) {
        do {
            var_v1++;
            var_v0++;
        } while (var_v0->active != -1);
    }
    gRacePlayers[0].scoreAttackPointTarget = var_v1;
    arg0->count = var_v1;
    if (var_v1 != 0) {
        gAssetHandles[0x26] = allocRelocatableHeapBlock(var_v1 << 6);
        arg0->matrices = (void *)getRelocatableHeapBlockBase(gAssetHandles[0x26]);
        initRaceScoreAttackRingMatrices(arg0);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceScoreAttackRings);
    }
}
