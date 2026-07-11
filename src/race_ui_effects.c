#include "common.h"
#include "race_ui_effects.h"
#include "snowboard_trail_effects.h"
#include "relocatable_heap.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "race_item_hit_flags.h"
#include "viewport_manager.h"
#include "spatial_math.h"
#include "fixed_point_math.h"
#include "model_animation.h"
#include "race_effects.h"
#include "race_player_movement.h"
#include "race_timer_ui.h"

/* Local 3-arg declaration; see note in callback_task_scheduler.h. */
extern void *createCallbackTaskWithUserIdPreservingArgs(void *, s32, s32);

#define RACE_UI_TRAIL_GFX_ALLOC_PTR (*(RaceUiDisplayCommand **)&gRegionAllocPtr)
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

typedef s16 FixedMatrix3sScratch[0x12];

typedef union {
    /* 0x00 */ s32 words[8];
    /* 0x00 */ s16 halfwords[0x10];
    /* 0x00 */ FixedTransform transform;
} RaceUiTrailCopyBlock;

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

typedef struct {
    /* 0x000 */ u16 playerIndex;
    /* 0x002 */ u8 pad002[0x0C - 0x02];
    /* 0x00C */ s32 unk00C;
    /* 0x010 */ u8 pad010[0x13 - 0x10];
    /* 0x013 */ s8 isActive;
    /* 0x014 */ u8 pad014[0x1C - 0x14];
    /* 0x01C */ Vec3i pos1C;
    /* 0x028 */ Vec3i pos28;
    /* 0x034 */ u8 pad34[0x94 - 0x34];
    /* 0x094 */ RaceUiTrailCopyBlock copyBlock94;
    /* 0x0B4 */ u8 padB4[0x174 - 0xB4];
    /* 0x174 */ FixedTransform transform;
    /* 0x194 */ u8 pad194[0x2C0 - 0x194];
    /* 0x2C0 */ s16 unk2C0;
    /* 0x2C2 */ s8 pad2C2;
    /* 0x2C3 */ s8 unk2C3;
    /* 0x2C4 */ u8 pad2C4[0x2C6 - 0x2C4];
    /* 0x2C6 */ u16 pendingItemHitFlags;
    /* 0x2C8 */ u8 pad2C8[0x2D8 - 0x2C8];
    /* 0x2D8 */ s16 unk2D8;
    /* 0x2DA */ s16 unk2DA;
    /* 0x2DC */ u8 pad2DC[0x2EA - 0x2DC];
    /* 0x2EA */ s16 pitch;
    /* 0x2EC */ s16 yaw;
    /* 0x2EE */ u8 pad2EE[0x2FC - 0x2EE];
    /* 0x2FC */ s32 flags;
    /* 0x300 */ u8 pad300[0x509 - 0x300];
    /* 0x509 */ s8 mode;
    /* 0x50A */ u8 pad50A[0x568 - 0x50A];
    /* 0x568 */ s32 unk568;
    /* 0x56C */ u8 pad56C[0x570 - 0x56C];
    /* 0x570 */ s16 unk570;
    /* 0x572 */ s16 unk572;
    /* 0x574 */ s16 score;
    /* 0x576 */ s16 targetScore;
    /* 0x578 */ u8 pad578[0x60C - 0x578];
} RacePlayerState;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s16 modelVtxHandle;
    /* 0x16 */ s16 modelTextureHandle;
    /* 0x18 */ u8 pad18[0x38 - 0x18];
    /* 0x38 */ s16 mainFontHandle;
    /* 0x3A */ u8 pad3A[0x3E - 0x3A];
    /* 0x3E */ s16 popupFontHandle;
    /* 0x40 */ u8 pad40[0x48 - 0x40];
    /* 0x48 */ s16 resultTextHandle;
    /* 0x4A */ s16 pad4A;
    /* 0x4C */ s16 rankTextHandle;
} RaceUiAssetHandles;

typedef struct RaceUiGfxCommandDest RaceUiGfxCommandDest;

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
    /* 0x1E */ s16 value;
    /* 0x20 */ s16 bonus;
    /* 0x22 */ s16 target;
    /* 0x24 */ s16 flag;
} RaceUiCounterActor;

typedef struct RaceUiDualCounterActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s8 row;
    /* 0x19 */ s8 column;
    /* 0x1A */ u8 pad1A[2];
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
    /* 0x30 */ u8 pad30[0x36 - 0x30];
    /* 0x36 */ s16 flags;
    /* 0x38 */ RaceUiGfxCommandDest *matrix;
    /* 0x3C */ u8 pad3C[4];
    /* 0x40 */ void *palette;
    /* 0x44 */ void *image;
    /* 0x48 */ u8 pad48[0x50 - 0x48];
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

struct RaceUiGfxCommandDest {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
};

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
    /* 0x24 */ Vec3i sourcePos;
    /* 0x30 */ Vec3i worldPos;
    /* 0x3C */ s32 velocityY;
    /* 0x40 */ RaceUiTrailCopyBlock copyBlock;
    /* 0x60 */ RaceUiTrailCopyBlock transformedCopyBlock;
    /* 0x80 */ s16 playerIndex;
    /* 0x82 */ u8 pad82[2];
    /* 0x84 */ s16 spinYaw;
    /* 0x86 */ u8 pad86[2];
    /* 0x88 */ RaceUiGfxCommandDest *matrix0;
    /* 0x8C */ RaceUiGfxCommandDest *matrix1;
    /* 0x90 */ s16 unk90;
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

typedef struct {
    /* 0x00 */ s16 pathIndex;
    /* 0x02 */ u8 pad2[0x48 - 0x02];
} RaceUiCourseSpawnEntry;

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

typedef struct {
    /* 0x000 */ s16 value;
    /* 0x002 */ u8 pad2[0x60C - 0x002];
} RacePlayerHalfwordField;

typedef struct {
    /* 0x000 */ s8 value;
    /* 0x001 */ u8 pad1[0x60C - 0x001];
} RacePlayerByteField;

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
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ RaceUiGfxCommandDest *matrix0;
    /* 0x28 */ RaceUiGfxCommandDest *matrix1;
    /* 0x2C */ s16 rotY;
    /* 0x2E */ s16 rotX;
    /* 0x30 */ u8 matrixDirty;
} RaceUiTrailingParticleActor;

typedef struct RaceUiTripleParticleActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ RaceUiGfxCommandDest *matrix0;
    /* 0x28 */ RaceUiGfxCommandDest *matrix1;
    /* 0x2C */ RaceUiGfxCommandDest *matrix2;
    /* 0x30 */ s16 rotY;
    /* 0x32 */ u8 matrixDirty;
} RaceUiTripleParticleActor;

typedef struct RaceUiSpinningParticleActor {
    /* 0x00 */ u8 pad0[0x18];
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
    /* 0x00 */ u8 pad0[0x18];
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
    /* 0x44 */ FixedTransform sourceTransform;
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
    /* 0x04 */ s32 command[3];
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

typedef struct {
    /* 0x0000 */ u8 pad0[0x156];
    /* 0x0156 */ u8 resultNames[10][5][4];
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
    /* 0x12 */ u8 pad12[0x1A - 0x12];
    /* 0x1A */ s16 timer;
    /* 0x1C */ s16 visibleRows;
    /* 0x1E */ s16 pendingFirstValue;
    /* 0x20 */ s16 pendingSecondValue;
    /* 0x22 */ s16 pendingThirdValue;
} RaceUiCourseStatsActor;

typedef struct {
    /* 0x0000 */ u8 pad0[0x4E];
    /* 0x004E */ u8 courseStatsNames[10][5][4];
} RaceUiCourseStatsNameData;

typedef struct {
    /* 0x0 */ s8 first;
    /* 0x1 */ s8 second;
    /* 0x2 */ s16 third;
} RaceUiPackedTime;

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

typedef struct {
    /* 0x000 */ s8 placement;
    /* 0x001 */ u8 pad1[0x60C - 0x001];
} RacePlayerPlacement;

typedef struct {
    /* 0x000 */ u8 placement;
    /* 0x001 */ u8 pad1[0x60C - 0x001];
} RacePlayerUnsignedPlacement;

typedef struct {
    /* 0x000 */ s16 value;
    /* 0x002 */ u8 pad2[0x60C - 0x002];
} RacePlayerValue;

typedef struct RaceUiAlpha18Actor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 alpha;
    /* 0x1A */ s16 timer;
    /* 0x1C */ s16 score;
} RaceUiAlpha18Actor;

typedef struct RaceUiResultsBannerActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 alpha;
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ s8 player0;
    /* 0x1D */ s8 player1;
    /* 0x1E */ s8 player2;
    /* 0x1F */ s8 player3;
} RaceUiResultsBannerActor;

typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u8 pad4[0x608];
} RacePlayerFlags;

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
    /* 0x35 */ u8 index;
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
    /* 0x00 */ s16 pathIndex;
    /* 0x02 */ u8 pad2[0x48 - 0x2];
} CourseSpawnEntry;

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

extern RaceUiSpriteInit D_800D5FF0[];

extern CourseSpawnEntry D_800B9540[];
extern Vec3i D_800D61C0[];
extern Vec3i D_800D6340[];
extern Vtx D_800D6350[][4];
extern Vec3i D_800D6220[];
extern Vec3i D_800D62A0;
extern Vec3i D_800D6324;
extern Vec3i D_800D6330[];
extern Vec3i D_800D62AC[];
extern s16 D_800D633C[];
extern s32 D_800DC900[];

extern void *gMenuRenderCallbackList;
extern void *D_80124858;
extern void *D_80124888;
extern void *D_801248B0;
extern void *D_801248BC;
extern void *D_801248C8;
extern u8 gPlayerCount;
extern u8 gRaceUpdatePaused;
extern s8 gRacePlayerCount;
extern u8 D_80121B81;
extern RacePlayerUnsignedPlacement D_80121D90[];
extern s32 D_80121DA4;
extern u8 gCurrentViewportIndex;
extern u8 D_80156609;
extern s16 gUiBlinkTimer;
extern s16 D_80112140;
extern s16 D_80112142;
extern s16 D_80112144;
extern s16 D_80112146;
extern s16 D_8011216A;
extern s16 D_8011216E;
extern s16 D_80112168;
extern s16 gRaceLapCount;
extern s32 D_80121D8C;
extern RacePlayerPlacement D_80122288[];
extern s8 D_80122289;
extern RacePlayerByteField D_80121D94[];
extern RacePlayerHalfwordField gPlayerHitSource[];
extern RacePlayerHalfwordField D_8012265E[];
extern RacePlayerHalfwordField D_80122C6A[];
extern RacePlayerHalfwordField D_80123276[];
extern RacePlayerByteField D_8012229A[];
extern s16 gFrameCounter;
extern s16 D_8011216C;
extern RaceUiAssetHandles D_80112130;
extern s16 D_801222F6;
extern s16 D_801222F2;
extern s16 D_801222F0;
extern RaceUiRankTrigger *D_8012228C;
extern void *D_80124878;
extern void *D_801248A4;
extern void *D_801248EC;
extern s16 D_800D6050[];
extern Vec3i D_800D6030[];
extern u32 *D_800D6400[];
extern RaceUiRankTextRenderEntry *D_800D761C[];
extern RaceUiGfxCommandScriptEntry *D_800D693C[];
extern FixedTransform gIdentityFixedTransform;
extern RaceUiGfxCommandDest gIdentityMatrix;
extern RaceUiCameraTransformSource D_801121E0[];
extern Gfx D_800D6120[];
extern Gfx D_800D6160[];
extern Gfx D_800D6968[];
extern u32 D_800D6230[];
extern u32 D_800D6270[];
extern u32 D_800D63D0[];
extern RaceUiProjectileVertexBlock D_800D64A0[];
extern u32 D_800D69A8[];
extern Gfx gEffectRenderModeSetupDl[];
extern Gfx gEffectRenderModeCleanupDl[];
extern u32 D_20019C0[];
extern u32 D_2002208[];
extern u32 D_20023A8[];
extern u32 D_2002660[];
extern u32 D_2002490[];
extern u32 D_2003870[];
extern u32 D_2003538[];
extern u32 D_20035F8[];
extern u32 D_200CC20[];
extern u32 D_200C910[];
extern u32 D_200C1C8[];
extern u32 D_200C6A0[];
extern u32 D_200C7D8[];
extern u32 D_200CE48[];
extern u32 D_200CFB0[];
extern u32 D_200D3A8[];
extern void addRenderCallback(void *, void *, s32);
extern RaceUiGfxCommandDest *allocFixedTransformMatrix(RaceUiTrailCopyBlock *);
extern void func_80048C90(RaceUiGfxCommandDest *, s32 *);
extern void osWritebackDCache(void *, s32);
extern void func_80045A78(s16, s16, s32, s32);
extern void func_80046D68(s16, s16, s32, s32, s32);
extern void func_80045990(s32, s32, void *, void *);
extern int sprintf(char *, const char *, ...);
extern void enqueuePositionalSoundEffect(s32, void *, s32, s32);
extern void enqueuePlayerLoopingPositionalSoundRequest(s32, void *, s32, s32, f32, s32);
extern RacePlayerState D_80121D80[];
extern RacePlayerValue D_80121D98[];
extern s16 D_8012206C[][0x306];
extern RacePlayerFlags D_8012207C[];
extern u8 gGameSaveDataBuffer[];
extern RaceTimer D_80121B74;
extern void enqueueSoundEffect(s32, s32);
extern void func_80048278(s32, s32, void *, s32);
extern char D_800E12F4[];
extern char D_800E128C[];
extern char D_800E1290[];
extern char D_800E1294[];
extern char D_800E1298[];
extern char D_800E129C[];
extern char D_800E12A0[];
extern char D_800E12A4[];
extern char D_800E12A8[];
extern char D_800E12AC[];
extern char D_800E12B0[];
extern char D_800E12B4[];
extern char D_800E12B8[];
extern const char D_800E1474[];
extern const char D_800E1484[];
extern const char D_800E1494[];
extern const char D_800E14A8[];
extern Gfx D_800D60A0[];
extern Gfx D_800D60E0[];
extern Gfx D_800D6120[];
extern Gfx D_800D6190[];
extern u32 D_800DEFF8[];
extern Gfx *gRegionAllocPtr;
extern RaceUiAssetEntry D_800D5FF4[];
extern Vec3i D_800D6110;
extern u16 D_800D6520[];
extern s32 gPlayerInputPressed;
extern s32 D_801235B4;
extern u32 D_80156614;

const char D_800E1220[0x10] = "Board Reverse";
const char D_800E1230[0x10] = "Board Reverse";
const char D_800E1240[0x4] = "%2d";
const char D_800E1244[0xC] = "%3d";
const char D_800E1250[0x4] = "%5d";

extern s32 func_80048E60(Vec3i *);
extern void func_800486BC(void *, void *);
extern s16 gRaceCourseIndex;
extern s16 D_801222F4;
extern void *D_801248D4;

void func_800572A0(RaceUiSlideActor *arg0) {
    volatile u8 pad[8];
    Gfx *gfx;

    gDPPipeSync(gRegionAllocPtr++);
    gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(D_80112140));
    gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(D_80112142));
    gSPMatrix(gRegionAllocPtr++, &gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->image, G_IM_FMT_CI, 0x20, 0x40, 0, G_TX_WRAP, G_TX_WRAP,
                           5, 6, G_TX_NOLOD, G_TX_NOLOD);

    gfx = gRegionAllocPtr++;
    gfx->words.w0 = ((arg0->angle * 4) & 0xFFF) | 0xF2000000;
    gfx->words.w1 = ((arg0->angle + 0x40) << 2) & 0xFFF;

    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette);
    gSPDisplayList(gRegionAllocPtr++, arg0->displayList);
}

void func_80057548(RaceUiSlideActor *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->soundIndex;
    arg0->angle = arg0->angle + 2;
    arg0->angle = arg0->angle & 0x3F;
    if (temp_v0 != 0) {
        enqueuePlayerLoopingPositionalSoundRequest(0xE, &D_800D6030[temp_v0], 0x7F, 0x32, 0.0f, 0xA);
    }
    if (arg0->velocity != 0) {
        addRenderCallback(&D_801248EC, func_800572A0, arg0);
        return;
    }
    addRenderCallback(&D_801248A4, func_800572A0, arg0);
}

void func_80057600(RaceUiSlideActor *arg0) {
    arg0->angle = 0;
    func_80045990(getRelocatableHeapBlockBase(D_80112168), D_800D5FF4[arg0->index].assetId, &arg0->image, &arg0->palette);
    arg0->displayList = (Gfx *) D_800D5FF0[arg0->index].word;
    arg0->velocity = D_800D5FF0[arg0->index].b6;
    arg0->soundIndex = D_800D5FF0[arg0->index].b7;
    setCallbackTaskCallback(arg0, func_80057548);
}

void func_80057694(RaceUiPromptActor *arg0) {
    if (gCurrentViewportIndex == arg0->index) {
        if (gUiBlinkTimer & 1) {
            func_80048278(-0x34, arg0->y, &D_800E1220, 0);
            return;
        }
        func_80048278(-0x34, arg0->y, &D_800E1230, 1);
    }
}

void func_80057710(RaceUiPromptActor *arg0) {
    if (D_8012207C[arg0->index].flags & 0x400) {
        if (arg0->timer < 0x1E) {
            arg0->timer++;
        }
    } else {
        arg0->timer = 0;
    }
    if (D_8012207C[arg0->index].flags & 0x1040) {
        arg0->timer = 0;
    }
    if (arg0->timer >= 0x1E) {
        addRenderCallback(&D_80124888, func_80057694, (s32) arg0);
    }
}


void func_80057810(RaceUiPromptActor *arg0) {
    arg0->y = -0x10;
    if (gPlayerCount != 1) {
        arg0->y = -8;
    }
    setCallbackTaskCallback(arg0, func_80057710);
}

void func_80057854(RaceUiPopupActor *arg0) {
    volatile u8 padding[0x20];
    char buffer[8];
    s32 i;

    if (gCurrentViewportIndex == 0) {
        func_80045A78((s16)(arg0->x - 0x3C), arg0->y.half.lo, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x8B);
        func_80045A78((s16)(arg0->x + 0x14), arg0->y.half.lo, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x8E);
        sprintf(buffer, D_800E1240, arg0->parent);
        for (i = 0; i < 2; i++) {
            if (buffer[i] != ' ') {
                func_80046D68((s16)(arg0->x + (i * 8) + 4), arg0->y.half.lo,
                              getRelocatableHeapBlockBase(D_80112130.popupFontHandle), (buffer[i] - 5) & 0xFFFF, 0xE);
            }
        }

        if (arg0->playerIndex != 0) {
            func_80045A78((s16)(arg0->x - 0x48), (s16)(arg0->y.word + 0x12),
                          getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x8D);
            func_80045A78((s16)(arg0->x + 0x20), (s16)(arg0->y.word + 0x12),
                          getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x8E);
            sprintf(buffer, D_800E1244, 0x12C);
            for (i = 0; i != 3; i++) {
                if (buffer[i] != ' ') {
                    func_80046D68((s16)(arg0->x + (i * 8) + 8), (s16)(arg0->y.word + 0x12),
                                  getRelocatableHeapBlockBase(D_80112130.popupFontHandle), (buffer[i] - 5) & 0xFFFF, 0xE);
                }
            }
        }
    }
}

void func_80057AA4(RaceUiPopupActor *arg0) {
    arg0->x += arg0->velocity;
    arg0->velocity += 4;
    if (arg0->velocity == 0x38) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&D_80124878, func_80057854, arg0);
    }
}

void func_80057B04(RaceUiPopupActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        setCallbackTaskCallback(arg0, func_80057AA4);
    }
    addRenderCallback(&D_80124878, func_80057854, arg0);
}

void func_80057B60(RaceUiPopupActor *arg0) {
    arg0->x -= arg0->velocity;
    arg0->velocity -= 4;
    if (arg0->velocity == 0) {
        arg0->timer = 0x2D;
        setCallbackTaskCallback(arg0, func_80057B04);
    }
    addRenderCallback(&D_80124878, func_80057854, arg0);
}

void func_80057BCC(RaceUiPopupActor *arg0) {
    arg0->y.word = -0x2C;
    arg0->velocity = 0x38;
    arg0->x = 0x1A4;
    setCallbackTaskCallback(arg0, func_80057B60);
}

void func_80057C08(void *arg0, s16 arg1) {
    RaceUiPopupActor *temp = createCallbackTask(func_80057BCC, 0, 0x64);
    if (temp != NULL) {
        temp->parent = arg0;
        temp->playerIndex = arg1;
    }
}

void func_80057C5C(RaceUiPopupActor *arg0) {
    if (gCurrentViewportIndex == 0) {
        func_80045A78(-0x3C, arg0->y.half.lo, getRelocatableHeapBlockBase(D_8011216E), 0x8F);
    }
}

void func_80057CAC(RaceUiPopupActor *arg0) {
    arg0->x += arg0->velocity;
    arg0->velocity += 4;
    if (arg0->velocity == 0x38) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&D_80124878, func_80057C5C, arg0);
    }
}

void func_80057D0C(RaceUiPopupActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        setCallbackTaskCallback(arg0, func_80057CAC);
    }
    addRenderCallback(&D_80124878, func_80057C5C, arg0);
}

void func_80057D68(RaceUiPopupActor *arg0) {
    arg0->x -= arg0->velocity;
    arg0->velocity -= 4;
    if (arg0->velocity == 0) {
        arg0->timer = 0x2D;
        setCallbackTaskCallback(arg0, func_80057D0C);
    }
    addRenderCallback(&D_80124878, func_80057C5C, arg0);
}

void func_80057DD4(RaceUiPopupActor *arg0) {
    arg0->y.word = 0xC;
    arg0->velocity = 0x38;
    arg0->x = 0x1A4;
    setCallbackTaskCallback(arg0, func_80057D68);
}

void func_80057E10(void *arg0) {
    if (gUiBlinkTimer & 8) {
        func_80045A78(0x20, 0x4A, getRelocatableHeapBlockBase(D_8011216E), 0x5B);
    }
}

void func_80057E60(s32 arg0) {
    addRenderCallback(&gMenuRenderCallbackList, func_80057E10, arg0);
}

void func_80057E90(RaceUiAlpha18Actor *arg0) {
    if (D_80122289 != 0) {
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
        func_80045A78(-0x68, -0x2C, getRelocatableHeapBlockBase(D_8011216E), 0x4D);
        func_80045A78(-0x42, -0xC, getRelocatableHeapBlockBase(D_8011216E), D_80122289 & 0xFFFF);
        func_80045A78(-0x22, 4, getRelocatableHeapBlockBase(D_8011216E), (D_80122289 + 0x48) & 0xFFFF);
        func_80045A78(-0x68, 6, getRelocatableHeapBlockBase(D_8011216E), (D_80121D90[0].placement + 0x91) & 0xFFFF);
        if (arg0->alpha != 0xFF) {
            gSPDisplayList(gRegionAllocPtr++, D_800DEFF8);
        }
    }
}

void func_8005804C(RaceUiAlpha18Actor *arg0) {
    volatile u8 padding[0x18];
    char buffer[8];
    s32 x;
    u8 *ptr;
    volatile RaceUiAssetHandles *handles;
    s32 space;

    sprintf(buffer + -8, D_800E1250, arg0->score);
    x = 0x20;
    handles = &D_80112130;
 ptr = (u8 *) (((s32) padding) - 0x10); if (!ptr) { } space = ' '; loop: if ((*ptr) != 0) {
        if (space != *ptr) {
            func_80045A78(x, -0x3F, getRelocatableHeapBlockBase(handles->popupFontHandle), (*ptr - 5) & 0xFFFF);
        }
        x += 8;
        ptr++;
        goto loop;
    }

    func_80045A78(x, -0x3F, getRelocatableHeapBlockBase(handles->popupFontHandle), 0x37);
}

// func_8005812C best match: 99.881% (nonmatchings/func_8005812C-4033633224288138541/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005812C.s")

#ifdef NON_MATCHING
void func_8005812C(void *arg0) {
    char buffer[0x14];
    s32 x;
    s32 i;
    s32 space;
    s32 colorValue;
    u16 color;
    RaceUiAssetHandles *assets;

    i = 0;
    do { sprintf(&buffer[i], D_800E1254, D_801222E8); x = 0x20; if (D_80122289 == 3) { colorValue = 0xD; } else { colorValue = 0xC; } do { assets = (RaceUiAssetHandles *)((u8 *)&D_80112130 + (u8)i); space = ' '; color = colorValue; if (space) { }
loop:
    if (buffer[i] != '\0') { if (buffer[i] != space) { func_80046D68((s16)x, -0xF, getRelocatableHeapBlockBase(assets->popupFontHandle), (buffer[i] - 5) & 0xFFFF, color); } x += 8; i++; goto loop; } } while (0); func_80046D68((s16)x, -0xF, getRelocatableHeapBlockBase(assets->popupFontHandle), 0x37, color); } while (0);
    if (D_80122289 == 3) {
        func_80045A78(0x18, -0xB, getRelocatableHeapBlockBase(assets->popupFontHandle), 0x4E);
    }
}
#endif

const char D_800E1258[0x4] = "%6d";
const char D_800E125C[0x10] = "-Rank Prize-";
const char D_800E126C[0x10] = "  -Get Coin-";
const char D_800E127C[0x10] = "-Total Money-";

void func_8005827C(void *arg0) {
    volatile u8 padding[0x18];
    char buffer[8];
    s32 x;
    u8 *ptr;
    volatile RaceUiAssetHandles *handles;
    s32 space;

    sprintf(buffer + -8, D_800E1258, D_80121D8C);
    x = 0x18;
    handles = &D_80112130;
 ptr = (u8 *) (((s32) padding) - 0x10); if (!ptr) { } space = ' '; loop: if ((*ptr) != 0) {
        if (space != *ptr) {
            func_80045A78(x, 0x21, getRelocatableHeapBlockBase(handles->popupFontHandle), (*ptr - 5) & 0xFFFF);
        }
        x += 8;
        ptr++;
        goto loop;
    }

    func_80045A78(x, 0x21, getRelocatableHeapBlockBase(handles->popupFontHandle), 0x37);
}

void func_80058360(RaceUiAlpha18Actor *arg0) {
    if (arg0->alpha != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->alpha);
    }
    func_80048278(8, -0x48, &D_800E125C, 5);
    func_80048278(0, -0x18, &D_800E126C, 5);
    func_80048278(4, 0x18, &D_800E127C, 5);
    if (arg0->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, D_800DEFF8);
    }
}

void func_800584A0(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, func_80057E90, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005804C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005812C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005827C, arg0);
    addRenderCallback(&D_80124858, func_80058360, arg0);
}

void func_80058538(void *arg0) {
    if (gPlayerInputPressed & 0x8000) {
        enqueueSoundEffect(0x18, 0x32);
        D_801235B4 |= 0x10;
        setCallbackTaskCallback(arg0, func_800584A0);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_80057E90, (s32)arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005804C, (s32)arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005812C, (s32)arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005827C, (s32)arg0);
    addRenderCallback(&D_80124858, func_80058360, (s32)arg0);
}

void func_80058610(RaceUiAlpha18Actor *arg0) {
    s32 unused;
    s32 shouldPlaySound;
    s32 value;
    RacePlayerState *player;

    shouldPlaySound = 0;
    value = arg0->score;
    if (value != 0) {
        shouldPlaySound = 1;
    }
    player = (RacePlayerState *) (s32) D_80121D80;
    if (value >= 9) {
        player->unk00C += 8;
        arg0->score -= 8;
    } else {
        D_80121D8C = player->unk00C + value;
        arg0->score = 0;
    }

    if (player->mode != 3) {
        value = player->unk568;
        if (value != 0) {
            shouldPlaySound = 1;
        }
        if (value >= 9) {
            player->unk00C += 8;
            player->unk568 = value - 8;
        } else {
            player->unk568 = 0;
            player->unk00C += value;
        }
    }

    if (gPlayerInputPressed & 0x8000) {
        D_80121D8C = player->unk00C + arg0->score;
        if (player->mode != 3) {
            player->unk00C += player->unk568;
            player->unk568 = 0;
        }
        arg0->score = 0;
    }

    if (player->unk00C >= 1000000) {
        player->unk00C = 999999;
    }

    if (player->mode != 3) {
        if ((arg0->score == 0) && (player->unk568 == 0)) {
            enqueueSoundEffect(2, 0x32);
            setCallbackTaskCallback(arg0, func_80058538);
        }
    } else if (arg0->score == 0) {
        shouldPlaySound = 0;
        enqueueSoundEffect(2, 0x32);
        setCallbackTaskCallback(arg0, func_80058538);
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

    addRenderCallback(&gMenuRenderCallbackList, func_80057E90, (s32)arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005804C, (s32)arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005812C, (s32)arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005827C, (s32)arg0);
    addRenderCallback(&D_80124858, func_80058360, (s32)arg0);
}

void func_80058880(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0;
        setCallbackTaskCallback(arg0, func_80058610);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_80057E90, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005804C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005812C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005827C, arg0);
    addRenderCallback(&D_80124858, func_80058360, arg0);
}

void func_8005893C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        enqueueSoundEffect(0x1A, 0x32);
        setCallbackTaskCallback(arg0, func_80058880);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_80057E90, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005804C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005812C, arg0);
    addRenderCallback(&D_80124858, func_80058360, arg0);
}

void func_800589F4(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        setCallbackTaskCallback(arg0, func_8005893C);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_80057E90, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005804C, arg0);
    addRenderCallback(&D_80124858, func_80058360, arg0);
}

void func_80058A98(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        enqueueSoundEffect(0x1A, 0x32);
        setCallbackTaskCallback(arg0, func_800589F4);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_80057E90, arg0);
    addRenderCallback(&D_80124858, func_80058360, arg0);
}

void func_80058B20(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(s16 *)((u8 *)arg0 + 0x18) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x18) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        *(s16 *)((u8 *)arg0 + 0x18) = 0xFF;
        setCallbackTaskCallback(arg0, func_80058A98);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_80057E90, arg0);
    addRenderCallback(&D_80124858, func_80058360, arg0);
}

void func_80058BAC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = 0;
    *(s16 *)((u8 *)arg0 + 0x1C) = D_800D6050[D_80122289 + gRaceCourseIndex * 4];
    setCallbackTaskCallback(arg0, func_80058B20);
}

void func_80058C00(RaceUiResultsBannerActor *arg0) {
    if (arg0->alpha != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->alpha);
    }

    if (arg0->player0 != -1) {
        func_80045A78(-0x88, -0x50, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0);
        func_80045A78(-0x68, -0x40, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x48);
        func_80046970(-0x54, -0x50, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                      (D_80121D90[arg0->player0].placement + 0x41) & 0xFFFF);
        func_80045A78(-0x30, -0x40, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x4C);
    }

    if (arg0->player1 != -1) {
        func_80045A78(8, -0x30, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 1);
        func_80045A78(0x28, -0x20, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x49);
        func_80046970(0x3C, -0x30, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                      (D_80121D90[arg0->player1].placement + 0x41) & 0xFFFF);
        func_80045A78(0x60, -0x20, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x4C);
    }

    if (arg0->player2 != -1) {
        func_80045A78(-0x88, -0x10, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 2);
        func_80045A78(-0x68, 0, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x4A);
        func_80046970(-0x54, -0x10, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                      (D_80121D90[arg0->player2].placement + 0x41) & 0xFFFF);
        func_80045A78(-0x30, 0, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x4C);
    }

    if (arg0->player3 != -1) {
        func_80045A78(8, 0x10, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 3);
        func_80045A78(0x28, 0x20, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x4B);
        func_80046970(0x3C, 0x10, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                      (D_80121D90[arg0->player3].placement + 0x41) & 0xFFFF);
        func_80045A78(0x60, 0x20, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x4C);
    }

    if (arg0->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, D_800DEFF8);
    }
}

// func_8005905C best match: 99.488% (nonmatchings/func_8005905C-2785870559185086986/base_11.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005905C.s")

#ifdef NON_MATCHING
void func_8005905C(void *arg0) {
    char buffer[0x58];
    RacePlayerState *player;
    s8 playerIndex;
    RaceUiResultsBannerActor *actor;
    char *text;

    actor = arg0;
    text = buffer - 0x10;
    if (!actor) {}
    if (actor->alpha != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, actor->alpha);
    }

    playerIndex = actor->player0;
    if (playerIndex != -1) {
        sprintf(text, D_800E128C, D_80121D98[playerIndex].value);
        if (gUiBlinkTimer & 1) {
            func_80048278(-0x20, -0x38, text, 6);
        } else {
            func_80048278(-0x20, -0x38, text, 0);
        }

        player = &D_80121D80[actor->player0];
        if (player->pad002[2] == 0) {
            sprintf(text, D_800E1290, player->playerIndex + 1);
            func_80048278(-0x30, -0x48, text, 5);
        } else {
            func_80048278(-0x30, -0x48, D_800E1294, 4);
        }
    }

    playerIndex = actor->player1;
    if (playerIndex != -1) {
        sprintf(text, D_800E1298, D_80121D98[playerIndex].value);
        func_80048278(0x70, -0x18, text, 6);

        player = &D_80121D80[actor->player1];
        if (player->pad002[2] == 0) {
            sprintf(text, D_800E129C, player->playerIndex + 1);
            func_80048278(0x60, -0x28, text, 5);
        } else {
            func_80048278(0x60, -0x28, D_800E12A0, 4);
        }
    }

    playerIndex = actor->player2;
    if (playerIndex != -1) {
        sprintf(text, D_800E12A4, D_80121D98[playerIndex].value);
        func_80048278(-0x20, 8, text, 6);

        player = &D_80121D80[actor->player2];
        if (player->pad002[2] == 0) {
            sprintf(text, D_800E12A8, player->playerIndex + 1);
            func_80048278(-0x30, -8, text, 5);
        } else {
            func_80048278(-0x30, -8, D_800E12AC, 4);
        }
    }

    playerIndex = actor->player3;
    if (playerIndex != -1) {
        sprintf(text, D_800E12B0, D_80121D98[playerIndex].value);
        func_80048278(0x70, 0x28, text, 6);

        player = &D_80121D80[actor->player3];
        if (player->pad002[2] == 0) {
            sprintf(text, D_800E12B4, player->playerIndex + 1);
            func_80048278(0x60, 0x18, text, 5);
        } else {
            func_80048278(0x60, 0x18, D_800E12B8, 4);
        }
    }

    if (actor->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, D_800DEFF8);
    }
}
#endif

// func_80059518 best match: 89.968% (nonmatchings/func_80059518-2225551288923588688/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80059518.s")

#ifdef NON_MATCHING
void func_80059518(void *arg0) {
    RaceUiResultsBannerActor *actor;
    s8 *players;
    s32 i;
    s32 j;
    s8 temp;

    actor = arg0;
    players = &actor->player0;
    for (i = 0; i < 4; i++) {
        players[i] = i;
    }

    for (i = 0; i < 3; i++) {
        for (j = i + 1; j < 4; j++) {
            if (D_80121D80[players[j]].mode < D_80121D80[players[i]].mode) {
                temp = players[j];
                players[j] = players[i];
                players[i] = temp;
            }
        }
    }

    if (!(D_80121D80[players[0]].flags & 0x40)) {
        players[0] = -1;
    }
    if (!(D_80121D80[players[1]].flags & 0x40)) {
        players[1] = -1;
    }
    if (!(D_80121D80[players[2]].flags & 0x40)) {
        players[2] = -1;
    }
    if (!(D_80121D80[players[3]].flags & 0x40)) {
        players[3] = -1;
    }
}
#endif

void func_80059804(void *arg0) {
    func_80059518(arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_80058C00, (s32)arg0);
    addRenderCallback(&D_80124858, func_8005905C, (s32)arg0);
}

void func_80059854(void *arg0) {
    s32 *input;
    s32 i;
    void *actor;

    actor = arg0;
    i = 0;
    if ((s32)gPlayerCount > 0) {
        input = &gPlayerInputPressed; do {
            if ((*input & 0x8000) && !(D_801235B4 & 0x10)) {
                D_801235B4 |= 0x10;
                enqueueSoundEffect(0x18, 0x32);
                setCallbackTaskCallback(actor, func_80059804);
            }
            i++;
            input++;
        } while (i < (s32)gPlayerCount);
    }
    func_80059518(actor);
    addRenderCallback(&gMenuRenderCallbackList, func_80058C00, (s32)actor);
    addRenderCallback(&D_80124858, func_8005905C, (s32)actor);
}

void func_80059950(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(s16 *)((u8 *)arg0 + 0x18) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x18) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x96;
        *(s16 *)((u8 *)arg0 + 0x18) = 0xFF;
        setCallbackTaskCallback(arg0, func_80059854);
    }
    func_80059518(arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_80058C00, arg0);
    addRenderCallback(&D_80124858, func_8005905C, arg0);
}

void func_800599DC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = 0;
    setCallbackTaskCallback(arg0, func_80059950);
}

// func_80059A04 best match: 96.300% (nonmatchings/func_80059A04-5272447827802519043/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80059A04.s")

#ifdef NON_MATCHING
void func_80059A04(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    char buffer[0x58];
    RaceUiPackedTime *record;
    s32 x;
    s16 y;
    unsigned int color;
    char *ptr;
    char *end;

    record = arg0;
    x = arg1;
    sprintf(buffer - 0x10, D_800E12BC, record->first);
    y = arg2;
    color = arg3;
    end = &buffer[-0xE];
    ptr = buffer - 0x10; do { func_80046D68((s16)x, y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), ((u8)*ptr - 5) & 0xFFFF, color);
        ptr++;
        x += 8;
    } while (ptr < end);

    func_80046D68((s16)x, y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x36, color);
    x += 8;
    sprintf(buffer - 0x10, D_800E12C4, record->second);
    ptr = buffer - 0x10;
    end = &buffer[-0xE];
    do {
        func_80046D68((s16)x, y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), ((u8)*ptr - 5) & 0xFFFF, color);
        ptr++;
        x += 8;
    } while (ptr < end);

    func_80046D68((s16)x, y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x35, color);
    x += 8;
    sprintf(buffer - 0x10, D_800E12CC, record->third >> 8);
    ptr = buffer - 0x10;
    end = &buffer[-0xE];
    do {
        func_80046D68((s16)x, y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), ((u8)*ptr - 5) & 0xFFFF, color);
        do {
            ptr++;
            x += 8;
        } while (0);
    } while (ptr != end);
}
#endif

void func_80059C34(RaceUiCourseStatsActor *arg0) {
    s32 y;
    s32 row;
    s32 offset;
    RaceUiCourseStatsActor *actor;

    actor = arg0;
    y = -0x50;
    for (row = 0, offset = 0; row != 5; row++, offset += 4, y += 0x20) {
        func_80045A78(-8, (s16)y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), (row + 0x77) & 0xFFFF);
        if ((row == actor->index) && (gUiBlinkTimer & 1)) {
            func_80059A04(((RaceUiCourseStatsNameData *)gGameSaveDataBuffer)->courseStatsNames[gRaceCourseIndex][row],
                          0x10, y, 0x10);
        } else if (row < 3) {
            func_80059A04(((RaceUiCourseStatsNameData *)gGameSaveDataBuffer)->courseStatsNames[gRaceCourseIndex][row],
                          0x10, y, 0xC);
        } else {
            func_80059A04(((RaceUiCourseStatsNameData *)gGameSaveDataBuffer)->courseStatsNames[gRaceCourseIndex][row],
                          0x10, y, 0xD);
        }

        func_80045A78(0x58, (s16)y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                      ((((RaceUiCourseStatsIconData *)gGameSaveDataBuffer)->courseStatsIcons[gRaceCourseIndex][row] & 7) +
                       0x51) &
                          0xFFFF);
        func_80045A78(0x6C, (s16)y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                      (((*((RaceUiCourseStatsIconData *)gGameSaveDataBuffer)).courseStatsIcons[gRaceCourseIndex][row] >> 3) +
                       0x7C) &
                          0xFFFF);
    }
}

void func_80059E5C(RaceUiAlpha1AActor *arg0) {
    Gfx *gfx;
    s32 w0;
    s32 w1;

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->alpha & 0xFF) | (~0xFF); } func_80059A04(&D_80121B74, -0x6C, -0x30, 0xC); if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) D_800DEFF8; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; } } while (0);
}

const char D_800E12D4[0xC] = "-Lap Time-";

void func_80059F6C(RaceUiAlpha18Actor *arg0) {
    Gfx *gfx;
    s32 w0;
    s32 w1;

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->alpha & 0xFF) | (~0xFF); } func_80048278(-0x74, -0x3C, D_800E12D4, 5); if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) D_800DEFF8; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; } } while (0);
}

void func_8005A07C(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, func_80059E5C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_80059C34, arg0);
    addRenderCallback(&D_80124858, func_80059F6C, arg0);
}

void func_8005A0E0(void *arg0) {
    if (gPlayerInputPressed & 0x8000) {
        D_801235B4 |= 0x10;
        enqueueSoundEffect(0x18, 0x32);
        setCallbackTaskCallback(arg0, func_8005A07C);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_80059E5C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_80059C34, arg0);
    addRenderCallback(&D_80124858, func_80059F6C, arg0);
}

void func_8005A184(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = *(s16 *)((u8 *)arg0 + 0x1C) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1C) == 0) {
        setCallbackTaskCallback(arg0, func_8005A0E0);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_80059E5C, arg0);
    addRenderCallback(&D_80124858, func_80059F6C, arg0);
}

void func_8005A1FC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x1A) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1C) = 0x1E;
        *(s16 *)((u8 *)arg0 + 0x1A) = 0xFF;
        setCallbackTaskCallback(arg0, func_8005A184);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_80059E5C, arg0);
    addRenderCallback(&D_80124858, func_80059F6C, arg0);
}

void func_8005A288(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(s16 *)((u8 *)arg0 + 0x18) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x18) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x18) = 0xFF;
        setCallbackTaskCallback(arg0, func_8005A1FC);
    }
    addRenderCallback(&D_80124858, func_80059F6C, arg0);
}

void func_8005A2F0(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = 0;
    *(s16 *)((u8 *)arg0 + 0x1A) = 0;
    setCallbackTaskCallback(arg0, func_8005A288);
}

const char D_800E12E0[4] = "%2d";

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

    func_80045A78(-0x6C, -0x24, getRelocatableHeapBlockBase(D_8011216E), 0x20);
    sprintf(buffer + -8, D_800E12E0, D_801222F4);

    if (buffer[-8] != ' ') {
        func_80045A78(-0x4C, -0x1C, getRelocatableHeapBlockBase(D_8011216E), (buffer[-8] - 5) & 0xFFFF);
    }

    x = -0x44;
    func_80045A78(((SplitWord *)&x)->half.lo, -0x1C, getRelocatableHeapBlockBase(D_8011216E), (buffer[-7] - 5) & 0xFFFF);

    if (actor->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, D_800DEFF8);
    }
}

// func_8005A4BC best match: 54.798% (nonmatchings/func_8005A4BC-7273315160691878794/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005A4BC.s")

#ifdef NON_MATCHING
const char D_800E12E4[0x4] = "%5d";
const char D_800E12E8[0x4] = "%5d";
const char D_800E12EC[0x4] = "%5d";
const char D_800E12F0[0x4] = "%6d";

void func_8005A4BC(RaceUiCounterActor *arg0) {
    volatile u8 pad[4];
    SplitWord y;
    char buffer[0x24];
    char *ptr;
    s32 x;
    s16 rowY;
    s16 visibleRows;

    y.word = -0x47;
    if (arg0->flag == 0) {
        y.word = -0x33;
    }
    visibleRows = arg0->state;
    x = 0x20;
    if (visibleRows >= 0) {
        sprintf(buffer, D_800E12E4, arg0->value);
        rowY = y.half.lo;
        ptr = buffer;
loop1:
        if (*ptr != 0) {
            if (*ptr != ' ') {
                func_80045A78((s16)x, rowY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), (*ptr - 5) & 0xFFFF);
            }
            x += 8;
            ptr++;
            goto loop1;
        }
        func_80045A78((s16)x, rowY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x37);
        y.word += 0x28;
        visibleRows = arg0->state;
    }

    if (visibleRows > 0) {
        x = 0x20;
        if (arg0->flag != 0) {
            sprintf(buffer, D_800E12E8, arg0->bonus);
            rowY = y.half.lo;
            ptr = buffer;
loop2:
            if (*ptr != 0) {
                if (*ptr != ' ') {
                    func_80045A78((s16)x, rowY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), (*ptr - 5) & 0xFFFF);
                }
                x += 8;
                ptr++;
                goto loop2;
            }
            func_80045A78((s16)x, rowY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x37);
            y.word += 0x28;
            visibleRows = arg0->state;
        }
    }

    if (visibleRows >= 2) {
        x = 0x20;
        sprintf(buffer, D_800E12EC, arg0->target);
        rowY = y.half.lo;
        ptr = buffer;
loop3:
        if (*ptr != 0) {
            if (*ptr != ' ') {
                func_80045A78((s16)x, rowY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), (*ptr - 5) & 0xFFFF);
            }
            x += 8;
            ptr++;
            goto loop3;
        }
        func_80045A78((s16)x, rowY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x37);
        y.word += 0x28;
        visibleRows = arg0->state;
    }

    if (visibleRows >= 3) {
        x = 0x18;
        sprintf(buffer, D_800E12F0, D_80121D8C);
        rowY = y.half.lo;
        ptr = buffer;
loop4:
        if (*ptr != 0) {
            if (*ptr != ' ') {
                func_80045A78((s16)x, rowY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), (*ptr - 5) & 0xFFFF);
            }
            x += 8;
            ptr++;
            goto loop4;
        }
        func_80045A78((s16)x, rowY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x37);
    }
}
#endif

// func_8005A884 best match: 87.770% (nonmatchings/func_8005A884-4139837607000619032/base_11.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005A884.s")

#ifdef NON_MATCHING
void func_8005A884(RaceUiPopupActor *arg0) {
    volatile u8 padding[0x20];
    char buffer[8];
    s32 i;
    s32 color;
    s32 y;
    s32 space;

    y = -0x50;
    i = 0;
    space = ' ';
    do {
        func_80045A78(-8, (s16)y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), (i + 0x77) & 0xFFFF);

        if ((i == arg0->index) && (gUiBlinkTimer & 1)) {
            color = 0x10;
        } else if (i < 3) {
            color = 0xC;
        } else {
            color = 0xD;
        }

        sprintf(buffer, D_800E12F4, ((RaceUiCourseValueData *)gGameSaveDataBuffer)->values[gRaceCourseIndex][i]);

        if ((u8)buffer[0] != space) {
            func_80046D68(0x10, (s16)y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                          (buffer[0] - 5) & 0xFFFF, color);
        }

        if ((u8)buffer[1] != space) {
            func_80046D68(0x18, (s16)y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                          (buffer[1] - 5) & 0xFFFF, color & 0xFFFF);
        }

        func_80045A78(0x46, (s16)y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                      ((((RaceUiCourseValueData *)gGameSaveDataBuffer)->icons[gRaceCourseIndex][i] & 7) + 0x51) & 0xFFFF);
        func_80045A78(0x58, (s16)y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                      ((((RaceUiCourseValueData *)gGameSaveDataBuffer)->icons[gRaceCourseIndex][i] >> 3) + 0x7C) & 0xFFFF);

        i++;
        y += 0x20;
    } while (i != 5);
}
#endif

const char D_800E12F8[0x4] = "/%d";
const char D_800E12FC[0x4] = "*";
const char D_800E1300[0xC] = "Time Out";

void func_8005AAE4(RaceUiAlpha18Actor *arg0) {
    Gfx *gfx;
    s32 w0;
    s32 w1;
    char unused[8];
    char sp20[0xC];

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; if ((gRegionAllocPtr && gRegionAllocPtr) && gRegionAllocPtr) { } w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->alpha & 0xFF) | (~0xFF); }
    sprintf(&sp20[0x14], D_800E12F8, D_801222F6); func_80048278(-0x54, -0x14, D_800E12FC, 6); func_80048278(-0x3C, -0x14, &sp20[0x14], 6); if (D_80121B81 != 0) { func_80048278(-0x68, -4, D_800E1300, 4); } if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) D_800DEFF8; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; } } while (0);
}

const char D_800E130C[0x14] = "-Hit Prize(*1G)-";
const char D_800E1320[0x14] = "-Hit Prize(*50G)-";
const char D_800E1334[0x10] = "  -Perfect Hit-";
const char D_800E1344[0x14] = "-Complete Bonus-";
const char D_800E1358[0x10] = "-Total Money-";

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

    if (D_80121B81 != 0) {
        func_80048278(-8, (s16)y, &D_800E130C, 5);
    } else {
        func_80048278(-0xC, (s16)y, &D_800E1320, 5);
    }

    y += 0x28;
    if (arg0->flag != 0) {
        func_80048278(-0xC, (s16)y, &D_800E1334, 5);
        y += 0x28;
    }

    func_80048278(-8, (s16)y, &D_800E1344, 5);
    y += 0x28;
    func_80048278(4, (s16)y, &D_800E1358, 5);

    if (arg0->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, D_800DEFF8);
    }
}

const char D_800E1368[0x4] = "Hit";
const char D_800E136C[0x4] = "Hit";
const char D_800E1370[0x4] = "Hit";
const char D_800E1374[0x4] = "Hit";
const char D_800E1378[0x4] = "Hit";

void func_8005AE1C(void *arg0) {
    func_80048278(0x20, -0x48, &D_800E1368, 6);
    func_80048278(0x20, -0x28, &D_800E136C, 6);
    func_80048278(0x20, -8, &D_800E1370, 6);
    func_80048278(0x20, 0x18, &D_800E1374, 4);
    func_80048278(0x20, 0x38, &D_800E1378, 4);
}

void func_8005AEB0(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, func_8005A31C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005A884, arg0);
    addRenderCallback(&D_80124858, func_8005AAE4, arg0);
    addRenderCallback(&D_80124858, func_8005AE1C, arg0);
}

void func_8005AF2C(void *arg0) {
    if (gPlayerInputPressed & 0x8000) {
        enqueueSoundEffect(0x18, 0x32);
        D_801235B4 |= 0x10;
        setCallbackTaskCallback(arg0, func_8005AEB0);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005A31C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005A884, arg0);
    addRenderCallback(&D_80124858, func_8005AAE4, arg0);
    addRenderCallback(&D_80124858, func_8005AE1C, arg0);
}

void func_8005AFEC(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, func_8005A31C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005A4BC, arg0);
    addRenderCallback(&D_80124858, func_8005AAE4, arg0);
    addRenderCallback(&D_80124858, func_8005AC44, arg0);
}

void func_8005B068(void *arg0) {
    if (gPlayerInputPressed & 0x8000) {
        enqueueSoundEffect(0x18, 0x32);
        if (*(u16 *)((u8 *)arg0 + 0x10) < 5) {
            setCallbackTaskCallback(arg0, func_8005AF2C);
        } else {
            D_801235B4 |= 0x10;
            setCallbackTaskCallback(arg0, func_8005AFEC);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005A31C, (s32)arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005A4BC, (s32)arg0);
    addRenderCallback(&D_80124858, func_8005AAE4, (s32)arg0);
    addRenderCallback(&D_80124858, func_8005AC44, (s32)arg0);
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
    if ((value >= 8) && !(gPlayerInputPressed & 0x8000)) {
        value = 8;
    }
    /* IDO register allocation depends on this always-zero index. */
    D_80121D80[gPlayerInputPressed * 0].unk00C += value;
    arg0->value -= value;

    value = arg0->bonus;
    if ((value >= 8) && !(gPlayerInputPressed & 0x8000)) {
        value = 8;
    }
    D_80121D80[0].unk00C += value;
    arg0->bonus -= value;

    value = arg0->target;
    if ((value >= 8) && !(gPlayerInputPressed & 0x8000)) {
        value = 8;
    }
    D_80121D80[0].unk00C += value;
    arg0->target -= value;

    if (D_80121D80[0].unk00C >= 0xF4240) {
        D_80121D80[0].unk00C = 0xF423F;
    }

    if ((arg0->value == 0) && (arg0->bonus == 0) && (arg0->target == 0)) {
        enqueueSoundEffect(2, 0x32);
        arg0->timer = 0x14;
        setCallbackTaskCallback(arg0, func_8005B068);
    }

    addRenderCallback(&gMenuRenderCallbackList, func_8005A31C, (s32)arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005A4BC, (s32)arg0);
    addRenderCallback(&D_80124858, func_8005AAE4, (s32)arg0);
    addRenderCallback(&D_80124858, func_8005AC44, (s32)arg0);
}

void func_8005B344(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = 3;
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        setCallbackTaskCallback(arg0, func_8005B14C);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005A31C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005A4BC, arg0);
    addRenderCallback(&D_80124858, func_8005AAE4, arg0);
    addRenderCallback(&D_80124858, func_8005AC44, arg0);
}

void func_8005B3EC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = 3;
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0xA;
        setCallbackTaskCallback(arg0, func_8005B344);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005A31C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005A4BC, arg0);
    addRenderCallback(&D_80124858, func_8005AAE4, arg0);
    addRenderCallback(&D_80124858, func_8005AC44, arg0);
}

void func_8005B49C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = 2;
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        setCallbackTaskCallback(arg0, func_8005B3EC);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005A31C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005A4BC, arg0);
    addRenderCallback(&D_80124858, func_8005AAE4, arg0);
    addRenderCallback(&D_80124858, func_8005AC44, arg0);
}

void func_8005B55C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = 1;
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        setCallbackTaskCallback(arg0, func_8005B49C);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005A31C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005A4BC, arg0);
    addRenderCallback(&D_80124858, func_8005AAE4, arg0);
    addRenderCallback(&D_80124858, func_8005AC44, arg0);
}

void func_8005B61C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = 0;
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        if (*(s16 *)((u8 *)arg0 + 0x24) != 0) {
            setCallbackTaskCallback(arg0, func_8005B55C);
        } else {
            setCallbackTaskCallback(arg0, func_8005B49C);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005A31C, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005A4BC, arg0);
    addRenderCallback(&D_80124858, func_8005AAE4, arg0);
    addRenderCallback(&D_80124858, func_8005AC44, arg0);
}

void func_8005B6F8(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        enqueueSoundEffect(0x1A, 0x32);
        setCallbackTaskCallback(arg0, func_8005B61C);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005A31C, arg0);
    addRenderCallback(&D_80124858, func_8005AAE4, arg0);
    addRenderCallback(&D_80124858, func_8005AC44, arg0);
}

void func_8005B798(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(s16 *)((u8 *)arg0 + 0x18) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x18) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        *(s16 *)((u8 *)arg0 + 0x18) = 0xFF;
        setCallbackTaskCallback(arg0, func_8005B6F8);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005A31C, arg0);
    addRenderCallback(&D_80124858, func_8005AAE4, arg0);
    addRenderCallback(&D_80124858, func_8005AC44, arg0);
}

void func_8005B834(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = 0;
    *(s16 *)((u8 *)arg0 + 0x24) = 0;
    *(s16 *)((u8 *)arg0 + 0x20) = 0;
    *(s16 *)((u8 *)arg0 + 0x22) = 0x12C;
    if (gRaceCourseIndex == 9) {
        *(s16 *)((u8 *)arg0 + 0x22) = 0x64;
    }
    if (D_80121B81 != 0) {
        *(s16 *)((u8 *)arg0 + 0x1E) = D_80121D80[0].score;
    } else {
        *(s16 *)((u8 *)arg0 + 0x1E) = D_80121D80[0].score * 0x32;
        if (D_80121D80[0].score == D_80121D80[0].targetScore) {
            *(s16 *)((u8 *)arg0 + 0x24) = 1;
            *(s16 *)((u8 *)arg0 + 0x20) = 0x3E8;
        }
    }
    setCallbackTaskCallback(arg0, func_8005B798);
}

void func_8005B8E8(RaceUiAlphaActor *arg0) {
    Gfx *gfx;
    s32 w0;
    s32 w1;

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->alpha & 0xFF) | (~0xFF); } func_80059A04(&D_80121B74, -0x68, -0x37, 0xC); if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) D_800DEFF8; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; } } while (0);
}

// func_8005B9F8 best match: 86.310% at nonmatchings/func_8005B9F8-6061209858023118177/base_8.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005B9F8.s")

#ifdef NON_MATCHING
const char D_800E137C[] = "%5d";
const char D_800E1380[] = "%5d";
const char D_800E1384[] = "%5d";
const char D_800E1388[] = "%5d";
const char D_800E138C[] = "%6d";

void func_8005B9F8(RaceUiDualCounterActor *arg0) {
    volatile u8 pad[4];
    SplitWord y;
    char buf[0x24];
    char *ptr;
    s32 x;
    s16 tempY;

    y.word = -0x47;
    x = 0x20;
    if (arg0->state >= 0) {
        sprintf(buf - 4, D_800E137C, arg0->leftValue);
        ptr = buf - 4;
        while (1) {
            if (*ptr == 0) {
                break;
            }
            if (*ptr != ' ') {
                func_80045A78((s16)x, -0x47, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                              (((u8)*ptr) - 5) & 0xFFFF);
            }
            x += 8;
            ptr++;
        }
        func_80045A78((s16)x, -0x47, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x37);
        y.word = -0x27;
    }

    if (arg0->state > 0) {
        x = 0x20;
        sprintf(buf - 4, D_800E1380, arg0->rightValue);
        tempY = y.half.lo;
        ptr = buf - 4;
        while (1) {
            if (*ptr == 0) {
                break;
            }
            if (*ptr != ' ') {
                func_80045A78((s16)x, tempY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                              (((u8)*ptr) - 5) & 0xFFFF);
            }
            x += 8;
            ptr++;
        }
        func_80045A78((s16)x, tempY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x37);
        y.word += 0x20;
    }

    if (arg0->state >= 2) {
        x = 0x20;
        if (arg0->flag != 0) {
            sprintf(buf - 4, D_800E1384, arg0->bonus);
            tempY = y.half.lo;
            ptr = buf - 4;
            while (1) {
                if (*ptr == 0) {
                    break;
                }
                if (*ptr != ' ') {
                    func_80045A78((s16)x, tempY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                                  (((u8)*ptr) - 5) & 0xFFFF);
                }
                x += 8;
                ptr++;
            }
            func_80045A78((s16)x, tempY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x37);
            y.word += 0x20;
        }
    }

    if (arg0->state >= 3) {
        x = 0x20;
        sprintf(buf - 4, D_800E1388, arg0->leftTarget);
        tempY = y.half.lo;
        ptr = buf - 4;
        while (1) {
            if (*ptr == 0) {
                break;
            }
            if (*ptr != ' ') {
                func_80045A78((s16)x, tempY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                              (((u8)*ptr) - 5) & 0xFFFF);
            }
            x += 8;
            ptr++;
        }
        func_80045A78((s16)x, tempY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x37);
        y.word += 0x20;
    }

    if (arg0->state >= 4) {
        x = 0x18;
        sprintf(buf - 4, D_800E138C, D_80121D8C);
        tempY = y.half.lo;
        ptr = buf - 4;
        while (1) {
            if (*ptr == 0) {
                break;
            }
            if (*ptr != ' ') {
                func_80045A78((s16)x, tempY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                              (((u8)*ptr) - 5) & 0xFFFF);
            }
            x += 8;
            ptr++;
        }
        func_80045A78((s16)x, tempY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x37);
    }
}
#endif

void func_8005BE68(RaceUiPopupActor *arg0) {
    s32 y;
    s32 color;
    s32 i;
    s32 offset;

    y = -0x50;
    /* IDO scheduling for this function depends on this initialization staying on one line. */
    i = 0; offset = 0; while (1) {
        func_80045A78(-8, (s16)y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), (i + 0x77) & 0xFFFF);
        if ((i == arg0->index) && (gUiBlinkTimer & 1)) {
            color = 0x10;
        } else if (i < 3) {
            color = 0xC;
        } else {
            color = 0xD;
        }
        func_80059A04(((RaceUiResultNameData *)gGameSaveDataBuffer)->resultNames[gRaceCourseIndex][i], 0x10, y, color);
        func_80045A78(0x58, (s16)y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                      ((((RaceUiResultIconData *)gGameSaveDataBuffer)->resultIcons[gRaceCourseIndex][i] & 7) + 0x51) & 0xFFFF);
        func_80045A78(0x6C, (s16)y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                      (((*((RaceUiResultIconData *)gGameSaveDataBuffer)).resultIcons[gRaceCourseIndex][i] >> 3) + 0x7C) & 0xFFFF);
        i++;
        offset += 4;
        y += 0x20;
        if (i == 5) {
            break;
        }
    }
}

const char D_800E1390[0xC] = "-Lap Time-";

void func_8005C03C(RaceUiAlphaActor *arg0) {
    Gfx *gfx;
    s32 w0;
    s32 w1;

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->alpha & 0xFF) | (~0xFF); } func_80048278(-0x70, -0x40, D_800E1390, 5); if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) D_800DEFF8; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; } } while (0);
}

// func_8005C14C best match: 93.873% (nonmatchings/func_8005C14C-4139837607000619032/base_13.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005C14C.s")

#ifdef NON_MATCHING
typedef struct RaceUiCourseRecordActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s8 unk18;
    /* 0x19 */ s8 unk19;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 alpha;
    /* 0x1E */ u8 pad1E[0x2A - 0x1E];
    /* 0x2A */ s16 flag;
} RaceUiCourseRecordActor;

typedef struct {
    /* 0x00 */ s8 x;
    /* 0x01 */ s8 y;
    /* 0x02 */ s16 z;
} RaceUiCoursePosition;

extern RaceUiCoursePosition D_800DC900[];
void func_8005C14C(RaceUiCourseRecordActor *arg0) {
    char buf[0x24];
    char *bufp;
    Gfx *gfx;
    s32 w0;
    s32 w1;
    s16 y;
    u32 new_var;
    RaceUiCoursePosition *pos;

    do {
        if (arg0->alpha != 0xFF) {
            gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000;
            gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1;
            gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1;
            gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->alpha & 0xFF) | (~0xFF);
        }
        if (D_80121B81 == 0) {
            func_80048278(-0x68, -0x1F, &D_800E139C, 7);
            pos = &D_800DC900[gRaceCourseIndex];
            bufp = buf;
            sprintf(bufp, &D_800E13A8, pos->x, pos->y, pos->z >> 8);
            func_80048278(-0x68, -0x16, bufp, 7);
            func_80048278(-0x68, -8, &D_800E13BC, 7);
            sprintf(bufp, &D_800E13C4, arg0->unk18, arg0->unk19, arg0->unk1A >> 8);
            func_80048278(-0x6C, 1, bufp, 7);
        } else {
            func_80048278(-0x68, -0x24, &D_800E13D8, 4);
        }
        func_80048278(-8, -0x50, &D_800E13E4, 5);
        y = -0x30;
        func_80048278(-0xC, y, &D_800E13F4, 5);
        y += 0x20;
        if (arg0->flag != 0) {
            func_80048278(-0xC, y, &D_800E1408, 5);
            y += 0x20;
        }
        func_80048278(-8, y, &D_800E1418, 5);
        y += 0x20;
        new_var = y;
        func_80048278(-0xC, new_var, &D_800E142C, 5);
        if (arg0->alpha != 0xFF) {
            gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) D_800DEFF8; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1;
        }
    } while (0);
}
#endif

void func_8005C3E4(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, func_8005B8E8, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005BE68, arg0);
    addRenderCallback(&D_80124858, func_8005C03C, arg0);
}

void func_8005C448(void *arg0) {
    if (gPlayerInputPressed & 0x8000) {
        enqueueSoundEffect(0x18, 0x32);
        D_801235B4 |= 0x10;
        setCallbackTaskCallback(arg0, func_8005C3E4);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005B8E8, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005BE68, arg0);
    addRenderCallback(&D_80124858, func_8005C03C, arg0);
}

void func_8005C4EC(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, func_8005B8E8, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005B9F8, arg0);
    addRenderCallback(&D_80124858, func_8005C03C, arg0);
    addRenderCallback(&D_80124858, func_8005C14C, arg0);
}

void func_8005C568(void *arg0) {
    if (gPlayerInputPressed & 0x8000) {
        enqueueSoundEffect(0x18, 0x32);
        if (*(u16 *)((u8 *)arg0 + 0x10) < 5) {
            setCallbackTaskCallback(arg0, func_8005C448);
        } else {
            D_801235B4 |= 0x10;
            setCallbackTaskCallback(arg0, func_8005C4EC);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005B8E8, (s32)arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005B9F8, (s32)arg0);
    addRenderCallback(&D_80124858, func_8005C03C, (s32)arg0);
    addRenderCallback(&D_80124858, func_8005C14C, (s32)arg0);
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
    if ((value >= 8) && !(gPlayerInputPressed & 0x8000)) {
        value = 8;
    }
    /* IDO register allocation depends on this always-zero index. */
    D_80121D80[gPlayerInputPressed * 0].unk00C += value;
    arg0->leftValue -= value;

    value = arg0->rightValue;
    if ((value >= 8) && !(gPlayerInputPressed & 0x8000)) {
        value = 8;
    }
    D_80121D80[0].unk00C += value;
    arg0->rightValue -= value;

    value = arg0->bonus;
    if ((value >= 8) && !(gPlayerInputPressed & 0x8000)) {
        value = 8;
    }
    D_80121D80[0].unk00C += value;
    arg0->bonus -= value;

    value = arg0->leftTarget;
    if ((value >= 8) && !(gPlayerInputPressed & 0x8000)) {
        value = 8;
    }
    D_80121D80[0].unk00C += value;
    arg0->leftTarget -= value;

    if (D_80121D80[0].unk00C >= 0xF4240) {
        D_80121D80[0].unk00C = 0xF423F;
    }

    if ((arg0->rightValue == 0) && (arg0->leftValue == 0) && (arg0->bonus == 0) && (arg0->leftTarget == 0)) {
        enqueueSoundEffect(2, 0x32);
        arg0->timer = 0x14;
        setCallbackTaskCallback(arg0, func_8005C568);
    }

    addRenderCallback(&gMenuRenderCallbackList, func_8005B8E8, (s32)arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005B9F8, (s32)arg0);
    addRenderCallback(&D_80124858, func_8005C03C, (s32)arg0);
    addRenderCallback(&D_80124858, func_8005C14C, (s32)arg0);
}

void func_8005C89C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x20) = 4;
    *(s16 *)((u8 *)arg0 + 0x1E) = *(s16 *)((u8 *)arg0 + 0x1E) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1E) == 0) {
        setCallbackTaskCallback(arg0, func_8005C64C);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005B8E8, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005B9F8, arg0);
    addRenderCallback(&D_80124858, func_8005C03C, arg0);
    addRenderCallback(&D_80124858, func_8005C14C, arg0);
}

void func_8005C944(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x20) = 4;
    *(s16 *)((u8 *)arg0 + 0x1E) = *(s16 *)((u8 *)arg0 + 0x1E) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1E) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1E) = 0xA;
        setCallbackTaskCallback(arg0, func_8005C89C);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005B8E8, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005B9F8, arg0);
    addRenderCallback(&D_80124858, func_8005C03C, arg0);
    addRenderCallback(&D_80124858, func_8005C14C, arg0);
}

void func_8005C9F4(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x20) = 3;
    *(s16 *)((u8 *)arg0 + 0x1E) = *(s16 *)((u8 *)arg0 + 0x1E) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1E) == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1E) = 0x14;
        setCallbackTaskCallback(arg0, func_8005C944);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005B8E8, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005B9F8, arg0);
    addRenderCallback(&D_80124858, func_8005C03C, arg0);
    addRenderCallback(&D_80124858, func_8005C14C, arg0);
}

void func_8005CAB4(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x20) = 2;
    *(s16 *)((u8 *)arg0 + 0x1E) = *(s16 *)((u8 *)arg0 + 0x1E) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1E) == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1E) = 0x14;
        setCallbackTaskCallback(arg0, func_8005C9F4);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005B8E8, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005B9F8, arg0);
    addRenderCallback(&D_80124858, func_8005C03C, arg0);
    addRenderCallback(&D_80124858, func_8005C14C, arg0);
}

void func_8005CB74(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x20) = 1;
    *(s16 *)((u8 *)arg0 + 0x1E) = *(s16 *)((u8 *)arg0 + 0x1E) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1E) == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1E) = 0x14;
        if (*(s16 *)((u8 *)arg0 + 0x2A) != 0) {
            setCallbackTaskCallback(arg0, func_8005CAB4);
        } else {
            setCallbackTaskCallback(arg0, func_8005C9F4);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005B8E8, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005B9F8, arg0);
    addRenderCallback(&D_80124858, func_8005C03C, arg0);
    addRenderCallback(&D_80124858, func_8005C14C, arg0);
}

void func_8005CC54(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x20) = 0;
    *(s16 *)((u8 *)arg0 + 0x1E) = *(s16 *)((u8 *)arg0 + 0x1E) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1E) == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1E) = 0x14;
        setCallbackTaskCallback(arg0, func_8005CB74);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005B8E8, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005B9F8, arg0);
    addRenderCallback(&D_80124858, func_8005C03C, arg0);
    addRenderCallback(&D_80124858, func_8005C14C, arg0);
}

void func_8005CD10(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1E) = *(s16 *)((u8 *)arg0 + 0x1E) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1E) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1E) = 0x14;
        enqueueSoundEffect(0x1A, 0x32);
        setCallbackTaskCallback(arg0, func_8005CC54);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005B8E8, arg0);
    addRenderCallback(&D_80124858, func_8005C03C, arg0);
    addRenderCallback(&D_80124858, func_8005C14C, arg0);
}

void func_8005CDB0(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = *(s16 *)((u8 *)arg0 + 0x1C) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x1C) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1E) = 0x14;
        *(s16 *)((u8 *)arg0 + 0x1C) = 0xFF;
        setCallbackTaskCallback(arg0, func_8005CD10);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005B8E8, arg0);
    addRenderCallback(&D_80124858, func_8005C03C, arg0);
    addRenderCallback(&D_80124858, func_8005C14C, arg0);
}

void func_8005CE4C(RaceUiDualCounterActor *arg0) {
    s16 temp_v0;

    arg0->alpha = 0;
    arg0->flag = 0;
    arg0->bonus = 0;
    arg0->leftTarget = 0x12C;
    if (gRaceCourseIndex == 9) {
        arg0->leftTarget = 0x64;
    }
    func_8007B130((RaceTimer *)&D_800DC900[gRaceCourseIndex], &D_80121B74, (RaceTimer *)&arg0->row);
    if (D_80121B81 != 0) {
        arg0->leftValue = 0;
        temp_v0 = D_801222F0;
    } else {
        arg0->leftValue = ((arg0->row * 0x3C) + arg0->column) * 0x14;
        if (D_80121D80[0].unk572 == (temp_v0 = D_80121D80[0].unk570)) {
            arg0->flag = 1;
            arg0->bonus = 0x12C;
            temp_v0 = D_801222F0;
        }
    }
    arg0->rightValue = temp_v0 * 0xA;
    setCallbackTaskCallback(arg0, func_8005CDB0);
}

// func_8005CF60 best match: 93.690% (nonmatchings/func_8005CF60-7123131487808489545/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005CF60.s")

#ifdef NON_MATCHING
const char D_800E143C[4] = "%d";
const char D_800E1440[4] = "%d";

void func_8005CF60(RaceUiDualCounterActor *arg0) {
    char buffer[0x2C];
    char *ptr;
    char *ptr2;
    Gfx *gfx;
    Gfx **gfxp;
    s32 w0;
    s32 w1;
    s32 x;

    gfxp = &gRegionAllocPtr;
    if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->alpha & 0xFF) | (~0xFF); }

    sprintf(buffer, D_800E143C, D_80122040);
    x = -0x50;
    ptr = buffer;
    if (D_80122040 >= 10) {
        x = -0x54;
    }
    if (D_80122040 >= 100) {
        x = -0x58;
    }
    if (D_80122040 >= 1000) {
        x = -0x5C;
    }

loop1:
    if (*ptr != '\0') {
        goto body1;
    }
    goto done1;

body1:
    if (*ptr != ' ') {
        func_80045A78(x, -0x37, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), (*ptr - 5) & 0xFFFF);
    }
    ptr++;
    x += 8;
    goto loop1;

done1:
    x = -0x50;
    sprintf(buffer, D_800E1440, D_80122043);
    ptr2 = buffer;
    if (D_80122043 >= 10) {
        x = -0x54;
    }

loop2:
    if (*ptr2 != '\0') {
        goto body2;
    }
    goto done2;

body2:
    if (*ptr2 != ' ') {
        func_80046D68(x, -0x13, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), (*ptr2 - 5) & 0xFFFF, 0xE);
    }
    ptr2++;
    x += 8;
    goto loop2;

done2:
    if (gfxp == NULL) {
    }
    if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) D_800DEFF8; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; }
}
#endif

// func_8005D1CC best match: 96.634% (nonmatchings/func_8005D1CC-6061209858023118177/base_18.c, permuter artifact)
#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005D1CC.s")

#ifdef NON_MATCHING
const char D_800E1444[0x4] = "%5d";
const char D_800E1448[0x4] = "%5d";
const char D_800E144C[0x4] = "%5d";
const char D_800E1450[0x4] = "%6d";

void func_8005D1CC(RaceUiCourseStatsActor *arg0) {
    volatile u8 pad[4];
    SplitWord y;
    char buffer[0x24];
    char *ptr;
    s32 x;
    s16 rowY;
    s16 visibleRows;

    y.word = -0x47;
    visibleRows = arg0->visibleRows;
    x = 0x20;
    if (visibleRows >= 0) {
        sprintf(buffer - 4, D_800E1444, arg0->pendingFirstValue);
        ptr = buffer - 4;
loop1:
        if (*ptr != 0) {
            if (*ptr != ' ') {
                func_80045A78(x, -0x47, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), (u16)(*ptr - 5));
            }
            x += 8;
            ptr++;
            goto loop1;
        }
        func_80045A78(x, -0x47, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x37);
        y.word = -0x1F;
        visibleRows = arg0->visibleRows;
    }

    if (visibleRows > 0) {
        x = 0x20;
        ptr = 0;
        sprintf(buffer - 4, D_800E1448, arg0->pendingSecondValue);
        rowY = y.half.lo;
        ptr = buffer;
        ptr = ptr - 4;
loop2:
        if (*ptr != 0) {
            if (*ptr != ' ') {
                func_80045A78(x, rowY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), (u16)(*ptr - 5));
            }
            x += 8;
            ptr++;
            goto loop2;
        }
        func_80045A78(x, rowY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x37);
        y.word += 0x28;
        visibleRows = arg0->visibleRows;
    }

    if (visibleRows >= 2) {
        x = 0x20;
        ptr = 0;
        sprintf(buffer - 4, D_800E144C, rowY = arg0->pendingThirdValue);
        rowY = y.half.lo;
        ptr = buffer - 4;
loop3:
        if (*ptr != 0) {
            if (*ptr != ' ') {
                func_80045A78(x, rowY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), (u16)(*ptr - 5));
            }
            x += 8;
            ptr++;
            goto loop3;
        }
        func_80045A78(x, rowY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x37);
        y.word += 0x28;
        visibleRows = arg0->visibleRows;
    }

    if (visibleRows >= 3) {
        x = 0x18;
        ptr = 0;
        sprintf(buffer - 4, D_800E1450, D_80121D8C);
        rowY = y.half.lo;
        ptr = buffer - 4;
loop4:
        if (*ptr != 0) {
            if (*ptr != ' ') {
                func_80045A78(x, rowY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), (u16)(*ptr - 5));
            }
            x += 8;
            ptr++;
            goto loop4;
        }
        func_80045A78(x, rowY, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x37);
    }
}
#endif

// func_8005D558 best match: 98.194% (nonmatchings/func_8005D558-6061209858023118177/base_9.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005D558.s")

#ifdef NON_MATCHING
const char D_800E1454[0x4] = "%4d";

void func_8005D558(RaceUiCourseStatsActor *arg0) {
    volatile u8 padding[0x18];
    char new_var;
    s32 i;
    u16 color;
    s32 y;
    s32 space;

    i = 0;
    y = -0x50;
    space = ' ';
    do {
        char buffer[8];

        func_80045A78(-8, (s16)y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle), (i + 0x77) & 0xFFFF);
        if ((i == arg0->index) && (gUiBlinkTimer & 1)) {
            color = 0x10;
        } else if (i < 3) {
            color = 0xC;
        } else {
            color = 0xD;
        }

        sprintf(buffer, D_800E1454, ((RaceUiTrickValueData *)gGameSaveDataBuffer)->values[gRaceCourseIndex][i]);

        new_var = buffer[0];
        if ((u8)new_var != space) {
            func_80046D68(0x10, (s16)y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                          (buffer[(color & 0xFFFF) * 0] - 5) & 0xFFFF, color);
        }
        if ((u8)buffer[1] != space) {
            func_80046D68(0x18, (s16)y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                          (buffer[1] - 5) & 0xFFFF, color & 0xFFFF);
        }
        if ((u8)buffer[2] != space) {
            func_80046D68(0x20, (s16)y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                          (buffer[2] - 5) & 0xFFFF, color & 0xFFFF);
        }
        if ((u8)buffer[3] != space) {
            func_80046D68(0x28, (s16)y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                          (buffer[3] - 5) & 0xFFFF, color & 0xFFFF);
        }

        func_80045A78(0x40, (s16)y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                      ((((RaceUiTrickIconData *)gGameSaveDataBuffer)->icons[gRaceCourseIndex][i] & 7) + 0x51) & 0xFFFF);
        func_80045A78(0x54, (s16)y, getRelocatableHeapBlockBase(D_80112130.popupFontHandle),
                      ((((RaceUiTrickIconData *)gGameSaveDataBuffer)->icons[gRaceCourseIndex][i] >> 3) + 0x7C) & 0xFFFF);

        i++;
        y += 0x20;
    } while (i != 5);
}
#endif

const char D_800E1458[0x8] = "-Trick-";
const char D_800E1460[0x8] = "-Make-";
const char D_800E1468[0xC] = "Time Out";
const char D_800E1474[0x10] = "  -Trick Prize-";
const char D_800E1484[0x10] = "  -Make Bonus-";
const char D_800E1494[0x14] = "-Complete Bonus-";
const char D_800E14A8[0x10] = "  -Total Money-";

void func_8005D860(RaceUiAlpha18Actor *arg0) {
    Gfx *gfx;
    void **new_var2;
    s32 w0;
    s32 w1;
    s32 new_var;

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; if (!gfx) { } gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; if ((gRegionAllocPtr && gRegionAllocPtr) && gRegionAllocPtr) { } w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->alpha & 0xFF) | (~0xFF); } func_80048278(-0x68, -0x40, new_var2 = &D_800E1458, 6); func_80048278(-0x64, -0x1C, &D_800E1460, 5); w0 = 8; if (D_80121B81 != 0) { new_var = w0; func_80048278(-0x6C, new_var, &D_800E1468, 4); } if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) D_800DEFF8; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; } } while (0);
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
    func_80048278(-0xC, (s16)y, &D_800E1474, 5);
    y += 0x28;
    func_80048278(-8, (s16)y, &D_800E1484, 5);
    y += 0x28;
    func_80048278(-8, (s16)y, &D_800E1494, 5);
    y += 0x28;
    func_80048278(-0xC, (s16)y, &D_800E14A8, 5);

    if ((*arg0p)->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, D_800DEFF8);
    }
    } while (0);
}

const char D_800E14B8[0x4] = "P";
const char D_800E14BC[0x4] = "P";
const char D_800E14C0[0x4] = "P";
const char D_800E14C4[0x4] = "P";
const char D_800E14C8[0x8] = "P";

void func_8005DB3C(void *arg0) {
    func_80048278(0x30, -0x48, &D_800E14B8, 6);
    func_80048278(0x30, -0x28, &D_800E14BC, 6);
    func_80048278(0x30, -8, &D_800E14C0, 6);
    func_80048278(0x30, 0x18, &D_800E14C4, 4);
    func_80048278(0x30, 0x38, &D_800E14C8, 4);
}

void func_8005DBD0(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, func_8005CF60, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005D558, arg0);
    addRenderCallback(&D_80124858, func_8005D860, arg0);
    addRenderCallback(&D_80124858, func_8005DB3C, arg0);
}

void func_8005DC4C(void *arg0) {
    if (gPlayerInputPressed & 0x8000) {
        enqueueSoundEffect(0x18, 0x32);
        D_801235B4 |= 0x10;
        setCallbackTaskCallback(arg0, func_8005DBD0);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005CF60, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005D558, arg0);
    addRenderCallback(&D_80124858, func_8005D860, arg0);
    addRenderCallback(&D_80124858, func_8005DB3C, arg0);
}

void func_8005DD0C(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, func_8005CF60, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005D1CC, arg0);
    addRenderCallback(&D_80124858, func_8005D860, arg0);
    addRenderCallback(&D_80124858, func_8005D9B4, arg0);
}

void func_8005DD88(void *arg0) {
    if (gPlayerInputPressed & 0x8000) {
        enqueueSoundEffect(0x18, 0x32);
        if (*(u16 *)((u8 *)arg0 + 0x10) < 5) {
            setCallbackTaskCallback(arg0, func_8005DC4C);
        } else {
            D_801235B4 |= 0x10;
            setCallbackTaskCallback(arg0, func_8005DD0C);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005CF60, (s32)arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005D1CC, (s32)arg0);
    addRenderCallback(&D_80124858, func_8005D860, (s32)arg0);
    addRenderCallback(&D_80124858, func_8005D9B4, (s32)arg0);
}

void func_8005DE6C(RaceUiCourseStatsActor *arg0) {
    s16 value;

    if (arg0->timer == 0) {
        arg0->timer = 2;
        enqueueSoundEffect(0x1A, 0x32);
    } else {
        arg0->timer--;
    }

    value = arg0->pendingFirstValue;
    if ((value >= 8) && !(gPlayerInputPressed & 0x8000)) {
        value = 8;
    }
    /* IDO register allocation depends on this always-zero index. */
    D_80121D80[gPlayerInputPressed * 0].unk00C += value;
    arg0->pendingFirstValue -= value;

    value = arg0->pendingSecondValue;
    if ((value >= 8) && !(gPlayerInputPressed & 0x8000)) {
        value = 8;
    }
    D_80121D80[0].unk00C += value;
    arg0->pendingSecondValue -= value;

    value = arg0->pendingThirdValue;
    if ((value >= 8) && !(gPlayerInputPressed & 0x8000)) {
        value = 8;
    }
    D_80121D80[0].unk00C += value;
    arg0->pendingThirdValue -= value;

    if (D_80121D80[0].unk00C >= 0xF4240) {
        D_80121D80[0].unk00C = 0xF423F;
    }

    if ((arg0->pendingFirstValue == 0) && (arg0->pendingSecondValue == 0) && (arg0->pendingThirdValue == 0)) {
        enqueueSoundEffect(2, 0x32);
        arg0->timer = 0x14;
        setCallbackTaskCallback(arg0, func_8005DD88);
    }

    addRenderCallback(&gMenuRenderCallbackList, func_8005CF60, (s32)arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005D1CC, (s32)arg0);
    addRenderCallback(&D_80124858, func_8005D860, (s32)arg0);
    addRenderCallback(&D_80124858, func_8005D9B4, (s32)arg0);
}

void func_8005E064(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = 3;
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        setCallbackTaskCallback(arg0, func_8005DE6C);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005CF60, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005D1CC, arg0);
    addRenderCallback(&D_80124858, func_8005D860, arg0);
    addRenderCallback(&D_80124858, func_8005D9B4, arg0);
}

void func_8005E10C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = 3;
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0xA;
        setCallbackTaskCallback(arg0, func_8005E064);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005CF60, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005D1CC, arg0);
    addRenderCallback(&D_80124858, func_8005D860, arg0);
    addRenderCallback(&D_80124858, func_8005D9B4, arg0);
}

void func_8005E1BC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = 2;
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        setCallbackTaskCallback(arg0, func_8005E10C);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005CF60, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005D1CC, arg0);
    addRenderCallback(&D_80124858, func_8005D860, arg0);
    addRenderCallback(&D_80124858, func_8005D9B4, arg0);
}

void func_8005E27C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = 1;
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        setCallbackTaskCallback(arg0, func_8005E1BC);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005CF60, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005D1CC, arg0);
    addRenderCallback(&D_80124858, func_8005D860, arg0);
    addRenderCallback(&D_80124858, func_8005D9B4, arg0);
}

void func_8005E33C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    *(s16 *)((u8 *)arg0 + 0x1C) = 0;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        enqueueSoundEffect(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        setCallbackTaskCallback(arg0, func_8005E27C);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005CF60, arg0);
    addRenderCallback(&gMenuRenderCallbackList, func_8005D1CC, arg0);
    addRenderCallback(&D_80124858, func_8005D860, arg0);
    addRenderCallback(&D_80124858, func_8005D9B4, arg0);
}

void func_8005E3F8(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        enqueueSoundEffect(0x1A, 0x32);
        setCallbackTaskCallback(arg0, func_8005E33C);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005CF60, arg0);
    addRenderCallback(&D_80124858, func_8005D860, arg0);
    addRenderCallback(&D_80124858, func_8005D9B4, arg0);
}

void func_8005E498(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(s16 *)((u8 *)arg0 + 0x18) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x18) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        *(s16 *)((u8 *)arg0 + 0x18) = 0xFF;
        setCallbackTaskCallback(arg0, func_8005E3F8);
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005CF60, arg0);
    addRenderCallback(&D_80124858, func_8005D860, arg0);
    addRenderCallback(&D_80124858, func_8005D9B4, arg0);
}

void func_8005E534(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = 0;
    *(s16 *)((u8 *)arg0 + 0x20) = 0;
    *(s16 *)((u8 *)arg0 + 0x22) = 0x12C;
    if (D_80121B81 != 0) {
        *(s16 *)((u8 *)arg0 + 0x1E) = 0;
        *(volatile s16 *)((u8 *)arg0 + 0x20) = 0;
    } else {
        *(s16 *)((u8 *)arg0 + 0x1E) = D_80121D80[0].unk2C0 * 3;
        *(s16 *)((u8 *)arg0 + 0x20) = D_80121D80[0].unk2C3 * 10;
    }
    setCallbackTaskCallback(arg0, func_8005E498);
}

void func_8005E5B4(void *arg0) {
    func_80045A78(-0x68, -0x2C, getRelocatableHeapBlockBase(D_8011216E), 0x4D);
    func_80045A78(-0x42, -0xC, getRelocatableHeapBlockBase(D_8011216E), D_80122289 & 0xFFFF);
    func_80045A78(-0x22, 4, getRelocatableHeapBlockBase(D_8011216E), (D_80122289 + 0x48) & 0xFFFF);
    func_80045A78(-0x68, 6, getRelocatableHeapBlockBase(D_8011216E), (D_80121D90[0].placement + 0x91) & 0xFFFF);
}

void func_8005E68C(void *arg0) {
    if (*(u16 *)((u8 *)arg0 + 0x10) != 0) {
        *(s16 *)((u8 *)arg0 + 0x10) = *(u16 *)((u8 *)arg0 + 0x10) - 1;
    } else {
        addRenderCallback(&gMenuRenderCallbackList, func_8005E5B4, (s32)arg0);
    }
}

void func_8005E6D0(RaceUiSparkleActor *arg0) {
    RaceUiSparkleActor *arg1;
    s32 i;
    s32 j;
    RaceUiTrailCopyBlock sp6C;

    arg1 = arg0;
    if (D_80156609 != 0) {
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
        gSPDisplayList(gRegionAllocPtr++, D_800D60E0);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg1->alpha);
        gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg1->images[arg1->frame], G_IM_FMT_CI, 0x20, 0x20,
                               0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg1->palettes[arg1->frame]);
        gSPMatrix(gRegionAllocPtr++, arg1->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(gRegionAllocPtr++, D_80156614, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        RACE_UI_GSP_VERTEX_F3DEX(gRegionAllocPtr++, D_800D60A0, 4, 0);
        RACE_UI_GSP1QUADRANGLE_F3DEX(gRegionAllocPtr++, 2, 1, 0, 3, 3);
    }
}

void func_8005EA4C(RaceUiSparkleActor *arg0) {
    RaceUiSparkleTransformScratch stack;
    RacePlayerState *player;
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
                player = &D_80121D80[arg0->playerIndex];
                player->unk2D8--;
                removeCallbackTask((CallbackTask *) arg0);
                return;
            }
        }

        arg0->unk28 += 0x60;
        angle = arg0->unk28 - D_8012206C[arg0->playerIndex][0];
        makeFixedRotationY(stack.matrix, angle);
        transformVec3iByFixedMatrix(stack.matrix, &D_800D6110, &stack.vec);

        player = &D_80121D80[arg0->playerIndex];
        makeFixedRotationXY(stack.matrix, player->pitch, player->yaw);
        transformVec3iByFixedMatrix(stack.matrix, &stack.vec, &arg0->pos);

        player = &D_80121D80[arg0->playerIndex];
        arg0->pos.x += player->pos28.x;
        arg0->pos.y += player->pos28.y;
        arg0->pos.z += player->pos28.z;
    }

    addRenderCallback(&D_801248EC, func_8005E6D0, (s32) arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005ECA8.s")

void func_8005EFFC(RaceUiSparkleActor *arg0) {
    s16 sp38[0x10];
    Vec3i sp2C;
    volatile u8 pad[8];
    RacePlayerState *player;

    if (gRaceUpdatePaused == 0) {
        if (gFrameCounter & 1) {
            arg0->frame = (arg0->frame + 1) & 3;
        }

        sp2C.x = 0;
        sp2C.y = arg0->zOffset;
        sp2C.z = 0;
        player = &D_80121D80[arg0->playerIndex];
        makeFixedRotationXY(sp38, player->pitch, player->yaw);
        transformVec3iByFixedMatrix(sp38, &sp2C, &arg0->pos);

        player = &D_80121D80[arg0->playerIndex];
        arg0->pos.x += player->pos28.x;
        arg0->pos.y += player->pos28.y;
        arg0->pos.z += player->pos28.z;
        arg0->alpha += arg0->alphaStep;
        if (arg0->alpha >= 0x100) {
            arg0->alpha = 0xFF;
            arg0->alphaStep = -0x20;
            arg0->timer = 0x14;
            setCallbackTaskCallback(arg0, func_8005ECA8);
        }
        arg0->zOffset += 0x20000;
    }

    addRenderCallback(&D_801248EC, func_8005E6D0, arg0);
}

void func_8005F174(RaceUiSparkleActor *arg0) {
    arg0->zOffset = 0x20000;
    arg0->alpha = 0;
    arg0->alphaStep = 0x10;
    arg0->frame = 0;
    arg0->scale = 0x1000;
    arg0->unk28 = 0;

    func_80045990(getRelocatableHeapBlockBase(D_80112168), 0x23, &arg0->images[0], &arg0->palettes[0]);
    func_80045990(getRelocatableHeapBlockBase(D_80112168), 0x24, &arg0->images[1], &arg0->palettes[1]);
    func_80045990(getRelocatableHeapBlockBase(D_80112168), 0x25, &arg0->images[2], &arg0->palettes[2]);
    func_80045990(getRelocatableHeapBlockBase(D_80112168), 0x26, &arg0->images[3], &arg0->palettes[3]);
    enqueuePositionalSoundEffect(0x10, &D_80121D80[arg0->playerIndex].pos1C, 0x7F, 0x32);
    setCallbackTaskCallback(arg0, func_8005EFFC);
}

void func_8005F298(s16 arg0) {
    RaceUiSparkleActor *temp = createCallbackTaskPreservingArgs(func_8005F174, 0, 0x62);
    if (temp != NULL) {
        temp->playerIndex = arg0;
    }
}

void func_8005F2DC(RaceUiSnowboardTrailActor *arg0) {
    volatile u8 pad[0x20];
    RaceUiDisplayCommand *unused;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        arg0->matrix0 = allocFixedTransformMatrix(&arg0->copyBlock);
        arg0->matrix1 = allocFixedTransformMatrix(&arg0->transformedCopyBlock);
    }

    if (arg0->matrix0 != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(D_80112144));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(D_80112146));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, D_2002208);
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix1, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, D_20023A8);
    }
}

void func_8005F448(RaceUiSnowboardTrailActor *arg0) {
    FixedTransform sp30;
    volatile u8 pad[0x10];
    RaceUiSnowboardTrailActor *actor;

    actor = arg0;
    actor->spinYaw += 0x240;
    if (gRaceUpdatePaused == 0) {
        actor->worldPos.y += actor->velocityY;
        actor->velocityY -= 0x8000;
    }

    actor->copyBlock.transform.translation.x = actor->worldPos.x;
    actor->copyBlock.transform.translation.y = actor->worldPos.y;
    actor->copyBlock.transform.translation.z = actor->worldPos.z;

    makeFixedRotationX(sp30.rotation, actor->spinYaw);
    sp30.translation.x = actor->sourcePos.x;
    sp30.translation.y = actor->sourcePos.y;
    sp30.translation.z = actor->sourcePos.z;
    composeFixedTransforms(&sp30, &actor->copyBlock.transform, &actor->transformedCopyBlock.transform);

    actor->timer--;
    if (actor->timer == 0) {
        removeCallbackTask(actor);
        return;
    }

    if (D_80121D94[actor->playerIndex].value == 0) {
        addRenderCallback(&D_801248B0, func_8005F2DC, arg0);
    }
}

void func_8005F56C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x90) = 1;
    *(s32 *)((u8 *)arg0 + 0x24) = 0xF0000;
    *(s32 *)((u8 *)arg0 + 0x28) = 0x230000;
    *(s32 *)((u8 *)arg0 + 0x2C) = 0;
    *(s16 *)((u8 *)arg0 + 0x92) = 0xF;
    *(s32 *)((u8 *)arg0 + 0x3C) = 0x20000;
    func_8005F448(arg0);
    setCallbackTaskCallback(arg0, func_8005F448);
}

void func_8005F5C8(RaceUiSnowboardTrailPlayer *player) {
    RaceUiSnowboardTrailActor *actor = createCallbackTaskPreservingArgs(func_8005F56C, 0, 0x62);

    if (actor != NULL) {
        actor->playerIndex = player->playerIndex;
        actor->worldPos = player->trail.worldPos;
        actor->copyBlock = player->trail.copyBlock;
        actor->spinYaw = player->trail.spinYaw;
    }
}

void func_8005F6A4(RaceUiRankTrailActor *arg0) {
    volatile u8 pad[0x20];
    RacePlayerState *player;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        player = &D_80121D80[arg0->playerIndex];
        arg0->copyBlock.words[5] = player->pos28.x;
        arg0->copyBlock.words[6] = player->pos28.y + 0x100000;
        arg0->copyBlock.words[7] = player->pos28.z;
        arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
    }

    if (arg0->matrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(D_80112144));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(D_80112146));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, D_2003870);
    }
}

void func_8005F828(RaceUiRankTrailActor *arg0) {
    RacePlayerState *player;
    s16 scale;
    s32 i;

    arg0->copyBlock.transform = gIdentityFixedTransform;
    player = &D_80121D80[arg0->playerIndex];
    arg0->pos.x = player->pos28.x;
    arg0->pos.y = player->pos28.y;
    arg0->pos.z = player->pos28.z;

    for (i = 0; i < 4; i++) {
        if (i != arg0->playerIndex) {
            func_80088664(&arg0->pos, 0xE0000, 0xB0000, 2, i);
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

    if (D_8012207C[arg0->playerIndex].flags & 0x100000) {
        addRenderCallback(&D_801248EC, func_8005F6A4, (s32)arg0);
        return;
    }

    for (i = 0; i < 8; i++) {
        func_80060454((void *)arg0->pos.x, (void *)arg0->pos.y, (void *)arg0->pos.z, i);
    }

    enqueuePositionalSoundEffect(0x15, &D_80121D80[arg0->playerIndex].pos1C, 0x7F, 0x32);
    removeCallbackTask(arg0);
}

void func_8005FB30(RaceUiRankTrailActor *arg0) {
    arg0->scale = 2;
    enqueuePositionalSoundEffect(0xF, &D_80121D80[arg0->playerIndex].pos1C, 0x7F, 0x32);
    setCallbackTaskCallback(arg0, func_8005F828);
}

void func_8005FBA8(RaceUiAnimatedTextActor *arg0) {
    volatile u8 padding[4];
    RaceUiTrailCopyBlock sp64;

    if (D_80156609 != 0) {
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
        gSPMatrix(gRegionAllocPtr++, D_80156614, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        RACE_UI_GSP_VERTEX_F3DEX(gRegionAllocPtr++, D_800D6120, 4, 0);
        RACE_UI_GSP1QUADRANGLE_F3DEX(gRegionAllocPtr++, 2, 1, 0, 3, 3);
    }
}

void func_8005FED0(RaceUiTextParticleActor *arg0) {
    volatile u8 padding[4];
    RaceUiTrailCopyBlock sp64;

    if (D_80156609 != 0) {
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
        gSPMatrix(gRegionAllocPtr++, D_80156614, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        RACE_UI_GSP_VERTEX_F3DEX(gRegionAllocPtr++, D_800D6120, 4, 0);
        RACE_UI_GSP1QUADRANGLE_F3DEX(gRegionAllocPtr++, 2, 1, 0, 3, 3);
    }
}

void func_800601F8(void *arg0) {
    s32 temp;

    if (gRaceUpdatePaused == 0) {
        temp = *(s32 *)((u8 *)arg0 + 0x4C);
        *(s32 *)((u8 *)arg0 + 0x18) += *(s32 *)((u8 *)arg0 + 0x48);
        *(s32 *)((u8 *)arg0 + 0x1C) += temp;
        *(s32 *)((u8 *)arg0 + 0x20) += *(s32 *)((u8 *)arg0 + 0x50);
        *(s32 *)((u8 *)arg0 + 0x4C) = temp - 0x8000;
        *(s16 *)((u8 *)arg0 + 0x54) = *(s16 *)((u8 *)arg0 + 0x54) + 1;
        if (*(s16 *)((u8 *)arg0 + 0x54) >= 0x10) {
            removeCallbackTask(arg0);
            return;
        }
    }
    if (*(u8 *)((u8 *)arg0 + 0x57) == 0) {
        addRenderCallback(&D_801248EC, func_8005FBA8, arg0);
        return;
    }
    addRenderCallback(&D_801248B0, func_8005FED0, arg0);
}

void func_800602BC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x54) = 0;
    *(Vec3i *)((u8 *)arg0 + 0x48) = D_800D61C0[*(u16 *)((u8 *)arg0 + 0x10)];
    if (*(u8 *)((u8 *)arg0 + 0x57) == 0) {
        func_80045990(getRelocatableHeapBlockBase(D_80112130.mainFontHandle), 0x27, (u8 *)arg0 + 0x38, (u8 *)arg0 + 0x28);
        func_80045990(getRelocatableHeapBlockBase(D_80112130.mainFontHandle), 0x28, (u8 *)arg0 + 0x3C, (u8 *)arg0 + 0x2C);
        func_80045990(getRelocatableHeapBlockBase(D_80112130.mainFontHandle), 0x29, (u8 *)arg0 + 0x40, (u8 *)arg0 + 0x30);
        func_80045990(getRelocatableHeapBlockBase(D_80112130.mainFontHandle), 0x2A, (u8 *)arg0 + 0x44, (u8 *)arg0 + 0x34);
    } else {
        func_80045990(getRelocatableHeapBlockBase(D_80112130.mainFontHandle), 0x2B, (u8 *)arg0 + 0x38, (u8 *)arg0 + 0x28);
        func_80045990(getRelocatableHeapBlockBase(D_80112130.mainFontHandle), 0x2C, (u8 *)arg0 + 0x3C, (u8 *)arg0 + 0x2C);
        func_80045990(getRelocatableHeapBlockBase(D_80112130.mainFontHandle), 0x2D, (u8 *)arg0 + 0x40, (u8 *)arg0 + 0x30);
        func_80045990(getRelocatableHeapBlockBase(D_80112130.mainFontHandle), 0x2E, (u8 *)arg0 + 0x44, (u8 *)arg0 + 0x34);
    }
    func_800601F8(arg0);
    setCallbackTaskCallback(arg0, func_800601F8);
}

void func_80060454(void *arg0, void *arg1, void *arg2, s16 arg3) {
    void *temp = createCallbackTaskPreservingArgs(func_800602BC, 5, 0x1E);
    if (temp != NULL) {
        *(s8 *)((u8 *)temp + 0x57) = 0;
        *(s16 *)((u8 *)temp + 0x10) = arg3;
        *(void **)((u8 *)temp + 0x18) = arg0;
        *(void **)((u8 *)temp + 0x1C) = arg1;
        *(void **)((u8 *)temp + 0x20) = arg2;
    }
}

void func_800604CC(void *arg0, void *arg1, void *arg2, s16 arg3) {
    void *temp = createCallbackTaskPreservingArgs(func_800602BC, 5, 0x1E);
    if (temp != NULL) {
        *(s8 *)((u8 *)temp + 0x57) = 1;
        *(s16 *)((u8 *)temp + 0x10) = arg3;
        *(void **)((u8 *)temp + 0x18) = arg0;
        *(void **)((u8 *)temp + 0x1C) = arg1;
        *(void **)((u8 *)temp + 0x20) = arg2;
    }
}

void func_80060544(RaceUiPodiumTrailActor *arg0) {
    volatile u8 pad[0x20];
    volatile RaceUiDisplayCommand *unused;

    if (D_80156609 != 0) {
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
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(D_80112144));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(D_80112146));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, D_2002660);
    }
}

void func_8006069C(void *arg0) {
    if (gRaceUpdatePaused == 0) {
        *(s32 *)((u8 *)arg0 + 0x4C) += -0x10000;
        *(s32 *)((u8 *)arg0 + 0x18) += 0x20000;
        *(s32 *)((u8 *)arg0 + 0x1C) += *(s32 *)((u8 *)arg0 + 0x4C);
        *(s32 *)((u8 *)arg0 + 0x20) += 0x10000;
        *(s16 *)((u8 *)arg0 + 0x56) = *(s16 *)((u8 *)arg0 + 0x56) - 1;
    }
    if (*(s16 *)((u8 *)arg0 + 0x56) == 0) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&D_801248B0, func_80060544, arg0);
    }
}

void updateGhostSlowdownImpact(RaceUiPodiumTrailActor *arg0) {
    RacePlayerState *player;
    s32 height;

    if (gRaceUpdatePaused == 0) {
        arg0->velocity += -0x10000;
        height = arg0->height + arg0->velocity;
        arg0->height = height;
        if ((height < 0x200000) && ((arg0->playImpactSound & 0xFF) != 0)) {
            enqueuePositionalSoundEffect(0xC, &D_80121D80[arg0->targetPlayerIndex].pos1C, 0x7F, 0x32);
            D_80121D80[arg0->targetPlayerIndex].pendingItemHitFlags |= PLAYER_HITFLAG_GHOST_SLOWDOWN;
            gPlayerHitSource[arg0->targetPlayerIndex].value = arg0->playerIndex;
            height = arg0->height;
            arg0->playImpactSound = 0;
        }

        if (height < 0x100000) {
            arg0->timer = 0x14;
            arg0->velocity = 0x80000;
            setCallbackTaskCallback(arg0, func_8006069C);
            height = arg0->height;
        }

        player = &D_80121D80[arg0->targetPlayerIndex];
        arg0->pos.x = player->pos28.x;
        arg0->pos.y = player->pos28.y + height;
        arg0->pos.z = player->pos28.z;
    }

    D_8012229A[arg0->targetPlayerIndex].value = 1;
    addRenderCallback(&D_801248BC, func_80060544, arg0);
}

// updateGhostSlowdownRise best match: 83.297% (nonmatchings/updateGhostSlowdownRise-8662636370764828261/base_15.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/updateGhostSlowdownRise.s")

#ifdef NON_MATCHING
void updateGhostSlowdownRise(RaceUiPodiumTrailActor *arg0) {
    RacePlayerState *player;
    s16 playerIndex;
    s16 scale;
    s32 height;

    if (arg0->timer == 0) {
        if (gRaceUpdatePaused == 0) {
            arg0->velocity += 0x10000;
            arg0->height += arg0->velocity;
            height = arg0->height;
            arg0->copyBlock.transform = gIdentityFixedTransform;
            playerIndex = arg0->playerIndex;
            player = &D_80121D80[playerIndex];
            arg0->pos.x = player->pos28.x;
            arg0->pos.y = player->pos28.y + height;
            arg0->pos.z = player->pos28.z;
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
                setCallbackTaskCallback(arg0, updateGhostSlowdownImpact);
            }
        }
        D_8012229A[arg0->targetPlayerIndex].value = 1;
        addRenderCallback(&D_801248BC, func_80060544, (s32)arg0);
    } else if (gRaceUpdatePaused == 0) {
        arg0->timer--;
    }
}
#endif

void initGhostSlowdownActor(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x54) = 1;
    *(s32 *)((u8 *)arg0 + 0x48) = 0;
    *(s32 *)((u8 *)arg0 + 0x4C) = 0;
    *(u8 *)((u8 *)arg0 + 0x59) = 1;
    enqueuePositionalSoundEffect(0xD, &D_80121D80[*(s16 *)((u8 *)arg0 + 0x50)].pos1C, 0x7F, 0x32);
    setCallbackTaskCallback(arg0, updateGhostSlowdownRise);
}

void spawnGhostSlowdownTargets(s16 arg0) {
    s32 var_s1;
    s32 var_s0;
    void *temp_v0;

    var_s1 = 0;
    var_s0 = 0;
    if (gRacePlayerCount > 0) {
        do {
            if (arg0 != var_s0) {
                temp_v0 = createCallbackTaskPreservingArgs(initGhostSlowdownActor, 0, 0x1E);
                if (temp_v0 != NULL) {
                    *(s16 *)((u8 *)temp_v0 + 0x50) = arg0;
                    *(s16 *)((u8 *)temp_v0 + 0x52) = var_s0;
                    *(s16 *)((u8 *)temp_v0 + 0x56) = var_s1 * 3;
                    var_s1 += 1;
                }
            }
            var_s0 += 1;
        } while (var_s0 < gRacePlayerCount);
    }
}

const char D_800E14D0[0x10] = "%4d";

void func_80060D10(RaceUiPopupActor *arg0) {
    volatile u8 padding[0x20];
    char buffer[8];
    s32 i;

    if (gCurrentViewportIndex == arg0->playerIndex) {
        func_80045A78((s16)(arg0->x - 0x34), (s16)(arg0->y.word - 8), getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x8B);
        sprintf(buffer, D_800E14D0, arg0->parent);
        for (i = 0; i != 4; i++) {
            if (buffer[i] != ' ') {
                func_80046D68((s16)(arg0->x + (i * 8) + 4), (s16)(arg0->y.word - 8),
                              getRelocatableHeapBlockBase(D_80112130.popupFontHandle), (buffer[i] - 5) & 0xFFFF, 0xE);
            }
        }
        func_80046D68((s16)(arg0->x + (i * 8) + 4), (s16)(arg0->y.word - 8),
                      getRelocatableHeapBlockBase(D_80112130.popupFontHandle), 0x37, 0xE);
    }
}

void func_80060E7C(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x1C) += *(s32 *)((u8 *)arg0 + 0x28);
    *(s32 *)((u8 *)arg0 + 0x28) += 4;
    if (*(s32 *)((u8 *)arg0 + 0x28) == 0x38) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&D_80124878, func_80060D10, arg0);
    }
}

void func_80060EDC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(u16 *)((u8 *)arg0 + 0x18) - 1;
    if (*(u16 *)((u8 *)arg0 + 0x18) == 0) {
        setCallbackTaskCallback(arg0, func_80060E7C);
    }
    addRenderCallback(&D_80124878, func_80060D10, arg0);
}

void func_80060F38(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x1C) -= *(s32 *)((u8 *)arg0 + 0x28);
    *(s32 *)((u8 *)arg0 + 0x28) -= 4;
    if (*(s32 *)((u8 *)arg0 + 0x28) == 0) {
        *(s16 *)((u8 *)arg0 + 0x18) = 0x2D;
        setCallbackTaskCallback(arg0, func_80060EDC);
    }
    addRenderCallback(&D_80124878, func_80060D10, arg0);
}

void func_80060FA4(void *arg0) {
    if (D_801124B0[*(s8 *)((u8 *)arg0 + 0x30)].active != 0) {
        if (gPlayerCount == 1) {
            *(s32 *)((u8 *)arg0 + 0x20) = -0x24;
        } else {
            *(s32 *)((u8 *)arg0 + 0x20) = -0x16;
        }
        *(s32 *)((u8 *)arg0 + 0x28) = 0x38;
        *(s32 *)((u8 *)arg0 + 0x1C) = 0x1A4;
        setCallbackTaskCallback(arg0, func_80060F38);
    } else {
        removeCallbackTask(arg0);
    }
}

void func_80061034(void *arg0, s16 arg1) {
    void *temp = createCallbackTask(func_80060FA4, 0, 0x64);
    if (temp != NULL) {
        *(void **)((u8 *)temp + 0x2C) = arg0;
        *(s8 *)((u8 *)temp + 0x30) = arg1;
    }
}

void func_80061088(RaceUiTripleParticleActor *arg0) {
    s16 unused;
    RaceUiTrailCopyBlock spAC;
    RaceUiTrailCopyBlock sp8C;
    RaceUiTrailCopyBlock sp6C;
    s32 sine;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
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
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(D_80112140));
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(D_80112142));
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, D_200C1C8);
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix1, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, D_200C6A0);
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix2, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, D_200C7D8);
        }
    }
}

void func_800613EC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x30) = *(s16 *)((u8 *)arg0 + 0x30) + 4;
    addRenderCallback(&D_801248B0, func_80061088, (s32)arg0);
}

void func_80061428(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x30) = 0;
    *(Vec3i *)((u8 *)arg0 + 0x18) = D_800D6220[*(u16 *)((u8 *)arg0 + 0x10)];
    setCallbackTaskCallback(arg0, func_800613EC);
}

void func_80061484(RaceUiRankTrailActor *arg0) {
    volatile u8 pad[0x20];
    RaceUiDisplayCommand *unused;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
    }

    if (arg0->matrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(D_80112144));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(D_80112146));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, D_20019C0);
    }
}

void func_800615BC(RaceUiRankTrailActor *arg0) {
    RacePlayerState *player;
    s32 i;

    arg0->copyBlock = (&D_80121D80[arg0->playerIndex])->copyBlock94;
    arg0->copyBlock.halfwords[0] = arg0->copyBlock.halfwords[0] / 4;
    arg0->copyBlock.halfwords[1] = arg0->copyBlock.halfwords[1] / 4;
    arg0->copyBlock.halfwords[2] = arg0->copyBlock.halfwords[2] / 4;
    arg0->copyBlock.halfwords[3] = arg0->copyBlock.halfwords[3] / 4;
    arg0->copyBlock.halfwords[4] = arg0->copyBlock.halfwords[4] / 4;
    arg0->copyBlock.halfwords[5] = arg0->copyBlock.halfwords[5] / 4;
    arg0->copyBlock.halfwords[6] = arg0->copyBlock.halfwords[6] / 4;
    arg0->copyBlock.halfwords[7] = arg0->copyBlock.halfwords[7] / 4;
    arg0->copyBlock.halfwords[8] = arg0->copyBlock.halfwords[8] / 4;

    player = &D_80121D80[arg0->playerIndex];
    if (player->flags & 0x400000) {
        addRenderCallback(&D_801248BC, func_80061484, (s32)arg0);
        return;
    }

    enqueuePositionalSoundEffect(0x11, &player->pos1C, 0x7F, 0x32);

    for (i = 0; i < 8; i++) {
        func_800604CC((void *)arg0->copyBlock.words[5], (void *)arg0->copyBlock.words[6], (void *)arg0->copyBlock.words[7], i);
    }

    removeCallbackTask(arg0);
}

void func_800617C8(void *arg0) {
    setCallbackTaskCallback(arg0, func_800615BC);
}

void func_800617EC(RaceUiRisingTrailActor *arg0) {
    volatile s32 padlow;
    s32 sine;
    RaceUiTrailCopyBlock sp80;
    volatile s32 pad[0x12];

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            sine = fixedSine(arg0->sineAngle);
            makeFixedRotationY(&sp80, (s16)(arg0->angle + 0x800));
            sp80.words[5] = arg0->pos.x;
            sp80.words[6] = arg0->pos.y + ((sine + 0x1000) << 5) + 0x10000;
            sp80.words[7] = arg0->pos.z;
            scaleFixedMatrix3sByQuarter(&sp80);
            arg0->matrix = allocFixedTransformMatrix(&sp80);
        }

        if (arg0->matrix != NULL) {
            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(D_80112144));
            gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(D_80112146));
            gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gRegionAllocPtr++, D_20019C0);
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
        if (func_80048E60(pos) != 0) {
            func_80088294(pos, 0x1A0000, 0x600000, 2);
            if (arg0->soundTimer == 0) {
                enqueuePositionalSoundEffect(0xD, pos, 0x7F, 0x31);
                func_8004B8B4(arg0->pos.x, arg0->pos.y + 0x700000, arg0->pos.z, arg0->surface, arg0->angle);
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
    addRenderCallback(&D_801248A4, func_800617EC, (s32)arg0);
}

void func_80061A98(RaceUiThrownTrailActor *arg0) {
    arg0->soundTimer = 0xF;
    arg0->pos.y = func_80080CC4(arg0->surface, arg0->pos.x, arg0->pos.z);
    arg0->angleStep = 8;
    arg0->angle = arg0->angleStep;
    setCallbackTaskCallback(arg0, func_80061984);
}

void func_80061AF4(s16 arg0, void *arg1, void *arg2, s16 arg3) {
    RaceUiThrownTrailActor *temp = createCallbackTask(func_80061A98, 0, 0x64);
    if (temp != NULL) {
        temp->pos.x = (s32)arg1;
        temp->pos.z = (s32)arg2;
        temp->targetAngle = (arg3 + 0x800) & 0xFFF;
        temp->surface = arg0;
    }
}

void func_80061B70(RaceUiSingleTrailActor *arg0) {
    volatile u8 pad[0x20];
    RaceUiDisplayCommand *unused;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
    }

    if (arg0->matrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(D_80112144));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(D_80112146));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, D_2003538);
    }
}

void func_80061CA8(RaceUiSingleTrailActor *arg0) {
    composeFixedTransforms(&arg0->sourceTransform, &D_80121D80[arg0->playerIndex].transform, &arg0->copyBlock.transform);

    if (gRaceUpdatePaused == 0) {
        arg0->timer--;
        if (!(D_8012207C[arg0->playerIndex].flags & 0x2000)) {
            arg0->timer = 0;
        }
    }

    if (arg0->timer == 0) {
        removeCallbackTask(arg0);
        return;
    }

    addRenderCallback(&D_801248BC, func_80061B70, (s32)arg0);
}

void func_80061D90(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x68) = 0x3C;
    makeFixedRotationY((u8 *)arg0 + 0x44, 0x800);
    *(s32 *)((u8 *)arg0 + 0x58) = 0;
    *(s32 *)((u8 *)arg0 + 0x5C) = 0x100000;
    *(s32 *)((u8 *)arg0 + 0x60) = 0xFFE00000;
    setCallbackTaskCallback(arg0, func_80061CA8);
}

void func_80061DE8(RaceUiFadingTrailActor *arg0) {
    volatile u8 pad[0x20];
    RaceUiDisplayCommand *unused;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
    }

    if (arg0->matrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(D_80112144));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(D_80112146));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetPrimColor(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->alpha);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, D_20035F8);
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
        makeFixedRotationY(&arg0->copyBlock, arg0->angle);

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
            if (func_800891B8(sp24, sp2C, 0x80, 0) != 0) {
                gPlayerHitSource[0].value = new_var->playerIndex;
            }
            if (func_800891B8(sp24, sp2C, 0x80, 1) != 0) {
                D_8012265E[0].value = new_var->playerIndex;
            }
            if (func_800891B8(sp24, sp2C, 0x80, 2) != 0) {
                D_80122C6A[0].value = arg0->playerIndex;
            }
            if (func_800891B8(sp24, sp2C, 0x80, 3) != 0) {
                D_80123276[0].value = arg0->playerIndex;
            }
        }
    }

    if (arg0->alpha <= 0) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&D_801248EC, func_80061DE8, (s32)arg0);
    }
}

void func_800621DC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x4C) = 4;
    *(s16 *)((u8 *)arg0 + 0x4A) = 0xFF;
    *(s16 *)((u8 *)arg0 + 0x48) = 0;
    *(s32 *)((u8 *)arg0 + 0x2C) = *(s32 *)((u8 *)arg0 + 0x38);
    *(s32 *)((u8 *)arg0 + 0x30) = *(s32 *)((u8 *)arg0 + 0x3C);
    *(s32 *)((u8 *)arg0 + 0x34) = *(s32 *)((u8 *)arg0 + 0x40);
    enqueuePositionalSoundEffect(0x13, (u8 *)arg0 + 0x38, 0x7F, 0x32);
    setCallbackTaskCallback(arg0, func_80061F38);
}

void func_8006224C(void *arg0, void *arg1, void *arg2) {
    void *temp = createCallbackTaskWithUserIdPreservingArgs(func_800621DC, 0, 3);
    if (temp != NULL) {
        *(void **)((u8 *)temp + 0x38) = arg0;
        *(void **)((u8 *)temp + 0x3C) = arg1;
        *(void **)((u8 *)temp + 0x40) = arg2;
    }
}

void func_800622B0(RaceUiTransitionRenderActor *arg0) {
    volatile u8 pad[0x20];
    RaceUiDisplayCommand *unused;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
    }

    if (arg0->matrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(D_80112144));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(D_80112146));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, D_2002490);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_800623E8.s")

void func_80062530(RaceUiTransitionActor *arg0) {
    s16 *transform = arg0->transformSource.source.halfwords;
    volatile u8 padding[0x20];

    arg0->unk68 = 0;
    makeFixedRotationX(transform, 0x400);
    scaleFixedMatrix3sByQuarter(transform);
    arg0->unk6C = 1;
    arg0->unk6E = 0xF;
    enqueuePositionalSoundEffect(0xB, &D_80121D80[arg0->index].pos1C, 0x7F, 0x32);
    func_800623E8(arg0);
    setCallbackTaskCallback(arg0, func_800623E8);
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

    if (D_80156609 != 0) {
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

 do { if (arg0->matrix != NULL) { temp_v0 = gRegionAllocPtr++; temp_v0->words.w0 = 0x06000000; temp_v0->words.w1 = (u32) D_800D6270; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFD500000; temp_v0_2->words.w1 = (u32) arg0->image; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x0703F800; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400200; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0003C03C; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; temp_v0_15 = gRegionAllocPtr++; temp_v0_15->words.w0 = 0x01020040; temp_v0_15->words.w1 = (u32) arg0->matrix; temp_v0_16 = gRegionAllocPtr++; temp_v0_16->words.w0 = 0x01000040; temp_v0_16->words.w1 = D_80156614; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x0400103F; temp_v0_17->words.w1 = (u32) D_800D6230; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; } } while (0);
}

void func_800628DC(RaceUiOrbitingSpriteActor *arg0) {
    RacePlayerState *player;

    player = &D_80121D80[arg0->index];
    if (!(player->flags & 0x10000)) {
        removeCallbackTask(arg0);
        return;
    }

    transformVec3iByFixedMatrix(player->transform.rotation, &D_800D62A0, &arg0->pos);
    player = &D_80121D80[arg0->index];
    arg0->pos.x += player->transform.translation.x;
    arg0->pos.y += player->transform.translation.y + 0x80000;
    arg0->pos.z += player->transform.translation.z;
    arg0->angle += 0xC0;
    arg0->pos.x -= fixedSine(arg0->angle) << 7;
    arg0->pos.z += fixedCosine(arg0->angle) << 7;
    addRenderCallback(&D_801248EC, func_800625D8, (s32)arg0);
}

void func_80062A10(void *arg0) {
    func_80045990(getRelocatableHeapBlockBase(D_80112168), 0x35, (u8 *)arg0 + 0x30, (u8 *)arg0 + 0x2C);
    setCallbackTaskCallback(arg0, func_800628DC);
}

void func_80062A64(s16 arg0) {
    void *temp;
    temp = createCallbackTaskPreservingArgs(func_80062A10, 0, 4);
    if (temp != NULL) {
        *(s8 *)((u8 *)temp + 0x35) = arg0;
        *(s16 *)((u8 *)temp + 0x28) = 0;
    }
    temp = createCallbackTaskPreservingArgs(func_80062A10, 0, 4);
    if (temp != NULL) {
        *(s8 *)((u8 *)temp + 0x35) = arg0;
        *(s16 *)((u8 *)temp + 0x28) = 0x555;
    }
    temp = createCallbackTaskPreservingArgs(func_80062A10, 0, 4);
    if (temp != NULL) {
        *(s8 *)((u8 *)temp + 0x35) = arg0;
        *(s16 *)((u8 *)temp + 0x28) = 0xAAA;
    }
}

void func_80062AF0(RaceUiScaledParticleActor *arg0) {
    FixedMatrix3sScratch scratch;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
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
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(D_80112140));
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(D_80112142));
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, D_200C910);
        }
    }
}

void func_80062D34(RaceUiScaledParticleActor *arg0) {
    RaceUiScaledParticleActor *actor;
    Vec3i *pos;
    s16 scale;

    actor = arg0;
    if (gRaceUpdatePaused == 0) {
        pos = &actor->pos;
        if (func_80048E60(pos) != 0) {
            actor->rotY += actor->rotYStep;

            if (func_80088E98(pos, 0x200000, 0xF0000, 0) != 0) {
                actor->scale = 0x1800;
                enqueuePositionalSoundEffect(0x67, pos, 0x7F, 0x32);
                func_80088C80(pos, 0x2C0000, 0xF0000, 0);
            }
            if (func_80088E98(pos, 0x200000, 0xF0000, 1) != 0) {
                actor->scale = 0x1800;
                enqueuePositionalSoundEffect(0x67, pos, 0x7F, 0x32);
                func_80088C80(pos, 0x2C0000, 0xF0000, 1);
            }
            if (func_80088E98(pos, 0x200000, 0xF0000, 2) != 0) {
                actor->scale = 0x1800;
                enqueuePositionalSoundEffect(0x67, pos, 0x7F, 0x32);
                func_80088C80(pos, 0x2C0000, 0xF0000, 2);
            }
            if (func_80088E98(pos, 0x200000, 0xF0000, 3) != 0) {
                actor->scale = 0x1800;
                enqueuePositionalSoundEffect(0x67, pos, 0x7F, 0x32);
                func_80088C80(pos, 0x2C0000, 0xF0000, 3);
            }

            scale = actor->scale;
            if (scale != 0x1000) {
                actor->scale = scale - 0x100;
            }
        }
    }
    addRenderCallback(&D_801248BC, func_80062AF0, actor);
}

void func_80062ED4(RaceUiScaledParticleActor *arg0) {
    arg0->rotY = randomNextSecondary() << 4;
    if (randomNextSecondary() & 1) {
        arg0->rotYStep = 0x40;
    } else {
        arg0->rotYStep = -0x40;
    }
    arg0->scale = 0x1000;
    arg0->pos = D_800D62AC[*(u16 *)((u8 *)arg0 + 0x10)];
    setCallbackTaskCallback(arg0, func_80062D34);
}

void func_80062F6C(RaceUiTrailingParticleActor *arg0) {
    FixedMatrix3sScratch scratch;
    Vec3i transformedOffset;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
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
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(D_80112140));
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(D_80112142));
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, D_200CE48);
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix1, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, D_200CC20);
        }
    }
}

void func_80063164(void *arg0) {
    if (gRaceUpdatePaused == 0) {
        *(s16 *)((u8 *)arg0 + 0x2E) = *(s16 *)((u8 *)arg0 + 0x2E) + 8;
    }
    addRenderCallback(&D_801248BC, func_80062F6C, (s32)arg0);
}

void func_800631B0(void *arg0) {
    *(Vec3i *)((u8 *)arg0 + 0x18) = D_800D6330[*(u16 *)((u8 *)arg0 + 0x10)];
    *(s16 *)((u8 *)arg0 + 0x2C) = D_800D633C[*(u16 *)((u8 *)arg0 + 0x10)];
    setCallbackTaskCallback(arg0, func_80063164);
}

void func_80063220(RaceUiSpinningParticleActor *arg0) {
    FixedMatrix3sScratch scratch;
    s32 temp2;
    s32 pad;
    s32 pad2;
    SplitWord temp;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentViewport(&arg0->pos) == 0) {
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
        gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(D_80112140));
        gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(D_80112142));
        gSPMatrix(gRegionAllocPtr++, arg0->matrix0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_200D3A8);
        gSPMatrix(gRegionAllocPtr++, arg0->matrix1, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_200CFB0);
    }
}

void func_80063410(void *arg0) {
    if (gRaceUpdatePaused == 0) {
        *(s16 *)((u8 *)arg0 + 0x2E) = *(s16 *)((u8 *)arg0 + 0x2E) + 0x60;
        *(s16 *)((u8 *)arg0 + 0x30) = *(s16 *)((u8 *)arg0 + 0x30) + 0x10;
        *(s16 *)((u8 *)arg0 + 0x32) = *(s16 *)((u8 *)arg0 + 0x32) + 4;
    }
    addRenderCallback(&D_801248BC, func_80063220, arg0);
}

void func_80063470(void *arg0) {
    *(Vec3i *)((u8 *)arg0 + 0x18) = D_800D6340[*(u16 *)((u8 *)arg0 + 0x10)];
    setCallbackTaskCallback(arg0, func_80063410);
}

// func_800634C8 best match: 99.268% (nonmatchings/func_800634C8-7273315160691878794/base_9.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_800634C8.s")

#ifdef NON_MATCHING
void func_800634C8(RaceUiCourseSpriteActor *arg0) {
    volatile u8 padding[4];
    s32 imageIndex;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentViewport((Vec3i *)&arg0->x) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            arg0->vertices = func_80048594(0x40);
            if (arg0->vertices != NULL) {
                arg0->vertices[0] = D_800D6350[arg0->index][0];
                arg0->vertices[0].v.tc[0] += arg0->angle << 5;
                arg0->vertices[1] = D_800D6350[arg0->index][1];
                arg0->vertices[1].v.tc[0] += arg0->angle << 5;
                arg0->vertices[2] = D_800D6350[arg0->index][2];
                arg0->vertices[2].v.tc[0] += arg0->angle << 5;
                arg0->vertices[3] = D_800D6350[arg0->index][3];
                arg0->vertices[3].v.tc[0] += arg0->angle << 5;
            }
        }

        if (arg0->vertices != NULL) {
            imageIndex = 0;
            if ((D_80122288[gCurrentViewportIndex].placement + 1) >= gRaceLapCount) {
                imageIndex = 1;
            }

            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, D_800D6270);
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gDPSetTextureImage(RACE_UI_TRAIL_GFX_ALLOC_PTR++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, arg0->palettes[imageIndex]);
            gDPTileSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
            gDPSetTile(RACE_UI_TRAIL_GFX_ALLOC_PTR++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0x100, G_TX_LOADTILE, 0,
                       G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD,
                       G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
            gDPLoadSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
            gDPLoadTLUTCmd(RACE_UI_TRAIL_GFX_ALLOC_PTR++, G_TX_LOADTILE, 0xF);
            gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
            gDPSetTextureImage(RACE_UI_TRAIL_GFX_ALLOC_PTR++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, arg0->images[imageIndex]);
            gDPSetTile(RACE_UI_TRAIL_GFX_ALLOC_PTR++, G_IM_FMT_CI, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0,
                       G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD,
                       G_TX_NOMIRROR | G_TX_WRAP, 6, G_TX_NOLOD);
            gDPLoadSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
            gDPLoadBlock(RACE_UI_TRAIL_GFX_ALLOC_PTR++, G_TX_LOADTILE, 0, 0, 0xFF, 0x200);
            gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
            gDPSetTile(RACE_UI_TRAIL_GFX_ALLOC_PTR++, G_IM_FMT_CI, G_IM_SIZ_4b, 4, 0, G_TX_RENDERTILE, 0,
                       G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD,
                       G_TX_NOMIRROR | G_TX_WRAP, 6, G_TX_NOLOD);
            gDPSetTileSize(RACE_UI_TRAIL_GFX_ALLOC_PTR++, G_TX_RENDERTILE, 0, 0, 0x3F << 2, 0xF << 2);
            RACE_UI_GSP_VERTEX_F3DEX(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->vertices, 4, 0);
            RACE_UI_GSP1QUADRANGLE_F3DEX(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 3, 2, 1, 0, 0);
            RACE_UI_GSP1QUADRANGLE_F3DEX(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 1, 2, 3, 0, 0);
        }
    }
}
#endif

void func_8006392C(void *arg0) {
    if ((gFrameCounter & 7) == 0) {
        *(s16 *)((u8 *)arg0 + 0x4C) = (*(s16 *)((u8 *)arg0 + 0x4C) + 4) & 0x3F;
    }
    addRenderCallback(&D_801248C8, func_800634C8, arg0);
}

void func_80063980(RaceUiCourseSpriteActor *actor) {
    s32 unused;
    s16 angle;

    switch (actor->index) {
        case 0:
            func_80081508(0, &actor->x, &actor->y, &actor->z, &angle);
            func_80045990(getRelocatableHeapBlockBase(D_8011216A), 0, &actor->images[0], &actor->palettes[0]);
            actor->images[1] = actor->images[0];
            actor->palettes[1] = actor->palettes[0];
            break;
        case 1:
            func_80081508(D_800B9540[gRaceCourseIndex].pathIndex, &actor->x, &actor->y, &actor->z, &angle);
            func_80045990(getRelocatableHeapBlockBase(D_8011216A), 1, &actor->images[0], &actor->palettes[0]);
            func_80045990(getRelocatableHeapBlockBase(D_8011216A), 2, &actor->images[1], &actor->palettes[1]);
            break;
    }
    unused = 0;
    setCallbackTaskCallback(actor, func_8006392C);
}

// func_80063A9C best match: 98.816% (nonmatchings/func_80063A9C-6061209858023118177/base_9.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80063A9C.s")

#ifdef NON_MATCHING
#define RACE_UI_EFFECT_EMIT_GFX(word0, word1) \
    {                                         \
        Gfx *_g;                              \
        _g = gRegionAllocPtr++;               \
        _g->words.w0 = (word0);               \
        _g->words.w1 = (word1);               \
    }

void func_80063A9C(RaceUiEffectParticleActor *arg0) {
    RaceUiTrailCopyBlock spA0;
    Gfx *new_var;
    RaceUiGfxCommandDest *matrix;
    RaceUiCameraTransformSource *camera;
    s32 negX;
    s32 negY;
    s32 negZ;
    s32 maskedNegX;
    s32 maskedNegY;
    s32 maskedNegZ;
    s32 i;
    Gfx *new_var2;
    s32 mask;
    RaceUiEffectParticleActor *actor;
    RaceUiEffectParticleActor *actor2;

    RACE_UI_EFFECT_EMIT_GFX(0xFD500000, (u32)arg0->unk20);
    RACE_UI_EFFECT_EMIT_GFX(0xF5500000, 0x07080200);
    RACE_UI_EFFECT_EMIT_GFX(0xE6000000, 0);
    negX = 0x070FF400;
    RACE_UI_EFFECT_EMIT_GFX(0xF3000000, negX);
    RACE_UI_EFFECT_EMIT_GFX(0xE7000000, 0);
    RACE_UI_EFFECT_EMIT_GFX(0xF5400400, 0x00080200);
    RACE_UI_EFFECT_EMIT_GFX(0xF2000000, 0x0007C07C);
    RACE_UI_EFFECT_EMIT_GFX(0xFD100000, (u32)arg0->unk1C);
    RACE_UI_EFFECT_EMIT_GFX(0xE8000000, 0);
    RACE_UI_EFFECT_EMIT_GFX(0xF5000100, 0x07000000);
    {
        Gfx *_g;

        _g = gRegionAllocPtr++;
        _g->words.w0 = 0xE6000000;
        new_var2 = _g;
        new_var2->words.w1 = 0;
    }
    RACE_UI_EFFECT_EMIT_GFX(0xF0000000, 0x0703C000);
    RACE_UI_EFFECT_EMIT_GFX(0xE7000000, 0);
    RACE_UI_EFFECT_EMIT_GFX(0x06000000, (u32)D_800D6270);

    spA0.transform = gIdentityFixedTransform;

    actor = arg0;
    actor2 = arg0;
    camera = &D_801121E0[gCurrentViewportIndex];
    negX = -camera->transformOffset.x;
    negY = -camera->transformOffset.y;
    negZ = -camera->transformOffset.z;

    i = 0;
    if (actor->count > 0) {
        mask = 0xFFFFFF;
        maskedNegX = negX & mask;
        maskedNegY = negY & mask;
        maskedNegZ = negZ & mask;
        do {
            spA0.transform.translation.x = ((actor->particles[i].unk0 - maskedNegX) & mask) + negX + 0xFF800000;
            spA0.transform.translation.y = ((actor->particles[i].unk4 - maskedNegY) & mask) + negY + 0xFF800000;
            spA0.transform.translation.z = ((actor->particles[i].unk8 - maskedNegZ) & mask) + negZ + 0xFF800000;
            matrix = allocFixedTransformMatrix(&spA0);
            if (matrix != NULL) {
                RACE_UI_EFFECT_EMIT_GFX(0x01020040, (u32)matrix);
                RACE_UI_EFFECT_EMIT_GFX(0x01000040, D_80156614);
                RACE_UI_EFFECT_EMIT_GFX(0x04000C2F, (u32)D_800D63D0);
                {
                    Gfx *_g;

                    _g = gRegionAllocPtr++;
                    new_var = _g;
                    new_var->words.w0 = 0xBF000000;
                    _g->words.w1 = 0x402;
                }
            }
            i++;
        } while (i < actor2->count);
    }
}

#undef RACE_UI_EFFECT_EMIT_GFX
#endif

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
    addRenderCallback(&D_801248D4, func_80063A9C, (s32)actor);
}

void func_80063FC0(RaceUiEffectParticleActor *actor) {
    s32 i;

    if (gPlayerCount != 0) {
        actor->count = 0xF;
    } else {
        actor->count = 0x3C;
    }

    func_80045990(getRelocatableHeapBlockBase(D_80112130.mainFontHandle), 0x42, &actor->unk20, &actor->unk1C);
    D_80112130.pad4A = allocRelocatableHeapBlock(actor->count * sizeof(RaceUiEffectParticle));
    actor->particles = (RaceUiEffectParticle *)getRelocatableHeapBlockBase(D_80112130.pad4A);

    for (i = 0; i < actor->count; i++) {
        actor->particles[i].unk0 = randomNextMain() << 0x10;
        actor->particles[i].unk4 = randomNextMain() << 0x10;
        actor->particles[i].unk8 = randomNextMain() << 0x10;
    }

    setCallbackTaskCallback(actor, func_80063E70);
}

void func_800640D8(RaceUiRankParticleActor *arg0) {
    RaceUiDisplayCommand *unused;
    s32 displayListIndex;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
        }

        if (arg0->matrix != NULL) {
            displayListIndex = 0;
            if (gRaceLapCount <= D_80122288[gCurrentViewportIndex].placement + 1) {
                displayListIndex = 1;
            }

            gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(D_80112140));
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(D_80112142));
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->displayLists[displayListIndex]);
        }
    }
}

void func_8006426C(s32 arg0) {
    addRenderCallback(&D_801248C8, func_800640D8, arg0);
}

void func_8006429C(RaceUiRankParticleActor *actor) {
    struct {
        s16 pad;
        s16 angle;
        s32 unused;
    } local;

    switch (actor->index) {
    case 0:
        func_80081508(1, &actor->pos.x, &actor->pos.y, &actor->pos.z, &local.angle);
        break;
    case 1:
        func_80081508(D_800B9540[gRaceCourseIndex].pathIndex, &actor->pos.x, &actor->pos.y, &actor->pos.z, &local.angle);
        break;
    }

    makeFixedRotationY(actor->copyBlock.halfwords, local.angle);
    actor->copyBlock.transform.translation.x = actor->pos.x;
    actor->copyBlock.transform.translation.y = actor->pos.y;
    actor->copyBlock.transform.translation.z = actor->pos.z;
    actor->displayLists[0] = D_800D6400[(actor->index * 2) + (gRaceCourseIndex * 4)];
    actor->displayLists[1] = D_800D6400[(actor->index * 2) + (gRaceCourseIndex * 4) + 1];
    setCallbackTaskCallback(actor, func_8006426C);
}

void func_800643B4(void *arg0, u16 *arg1) {
    *(u16 **)((u8 *)arg0 + 0x30) = arg1;
    *(s16 *)((u8 *)arg0 + 0x34) = arg1[0];
    *(s16 *)((u8 *)arg0 + 0x36) = arg1[1];
    func_80045990(getRelocatableHeapBlockBase(D_8011216C), arg1[2], (u8 *)arg0 + 0x44, (u8 *)arg0 + 0x40);
}

s32 func_80064414(void *arg0) {
    s16 temp_v0;

    temp_v0 = *(s16 *)((u8 *)arg0 + 0x34);
    if (!(temp_v0 & 0x7FFF)) {
        if (temp_v0 & 0x8000) {
            return 1;
        }
        func_800643B4(arg0, *(u16 **)((u8 *)arg0 + 0x30) + 3);
        goto block_5;
    }
    *(s16 *)((u8 *)arg0 + 0x34) = temp_v0 - 1;
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

    if (D_80156609 != 0) {
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
        do { temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0x06000000; temp_v0_2->words.w1 = (u32) D_800D6270; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xFD500000; temp_v0_3->words.w1 = (u32) arg0->image; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w0 = 0xF5500000; temp_v0_4->words.w1 = 0x07080200; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xE6000000; temp_v0_5->words.w1 = 0; temp_t4 = gRegionAllocPtr++; temp_t4->words.w0 = 0xF3000000; temp_t0 = (((var_ra << 5) + 3) >> 2) - 1; if (temp_t0 < 0x7FF) { var_t5 = temp_t0; } else { var_t5 = 0x7FF; } temp_t4->words.w1 = (((var_t5 & 0xFFF) << 0xC) | 0x07000000) | 0x400; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w0 = 0xF5400400; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = (((var_ra - 1) << 2) & 0xFFF) | 0x0007C000; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->palette; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w0 = 0x01020040; temp_v0_17->words.w1 = (u32) arg0->matrix; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0x01000040; temp_v0_18->words.w1 = (u32) D_80156614; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0x0400103F; temp_v0_2->words.w1 = (u32) (&D_800D64A0[sp74]); temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xB1060402; temp_v0_3->words.w1 = 0x00060200; } while (0);
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
        makeFixedRotationXY(sp24.mtx, D_80121D80[actor->index].pitch, D_80121D80[actor->index].yaw);
        transformVec3iByFixedMatrix(sp24.mtx, &actor->velocity, &actor->pos);
        actor->pos.x += D_80121D80[actor->index].pos28.x;
        actor->pos.y += D_80121D80[actor->index].pos28.y + actor->verticalVelocity;
        actor->pos.z += D_80121D80[actor->index].pos28.z;
        temp = (actor->verticalVelocity += actor->verticalAcceleration);
        actor->verticalAcceleration += 0x10000;
        if (temp >= 0x300001) {
            removeCallbackTask(actor);
            return;
        }
    }
    addRenderCallback(&D_801248C8, func_80064470, actor);
}

// func_80064914 best match: 96.414% (nonmatchings/func_80064914-5272447827802519043/base_13.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80064914.s")

#ifdef NON_MATCHING
void func_80064914(RaceUiProjectileActor *arg0) {
    FixedMatrix3sScratch sp44;
    RacePlayerState *player;
    RacePlayerState *otherPlayer;
    s32 value;
    s32 amount;
    s16 flags;

    if (gRaceUpdatePaused == 0) {
        player = &D_80121D80[arg0->index];
        makeFixedRotationXY(sp44, D_80121D80[arg0->index].pitch, D_80121D80[arg0->index].yaw);
        transformVec3iByFixedMatrix(sp44, &arg0->velocity, &arg0->pos);

        player = &D_80121D80[arg0->index];
        arg0->pos.x += player->pos28.x;
        arg0->pos.y += player->pos28.y;
        arg0->pos.z += player->pos28.z;
        func_80064414(arg0);

        otherPlayer = D_80121D80;
        do {
            if ((otherPlayer->isActive != 0) && (otherPlayer->playerIndex != arg0->index)) {
                value = otherPlayer->unk568;
                amount = value;
                if (value >= 0xA6) {
                    amount = 0xA6;
                }
                otherPlayer->unk568 = value - amount;
                func_8008BB5C((struct RaceInputPlayer *)&D_80121D80[arg0->index], amount);
            }
            otherPlayer++;
        } while (otherPlayer != &gFrameCounter);

        flags = arg0->flags;
        amount = flags & 1;
        if (flags & 8) {
            enqueuePositionalSoundEffect(0x69, &D_80121D80[arg0->index].pos28, 0x7F, 0x32);
            arg0->flags &= ~8;
            amount = (arg0->flags & 1) & 0xFFFF;
        }
        if (amount != 0) {
            arg0->verticalAcceleration = 0;
            arg0->verticalVelocity = 0;
            enqueuePositionalSoundEffect(0x6A, &D_80121D80[arg0->index].pos28, 0x7F, 0x32);
            setCallbackTaskCallback(arg0, func_800647E0);
        }
    }

    addRenderCallback(&D_801248C8, func_80064470, arg0);
}
#endif

void func_80064B28(RaceUiProjectileActor *arg0) {
    FixedMatrix3sScratch sp2C;
    RacePlayerState *player;
    RaceUiProjectileActor *actor;

    actor = arg0;
    if (gRaceUpdatePaused == 0) {
        player = &D_80121D80[actor->index];
        arg0 += 0;
        if (((!actor) && (!actor)) && (!actor)) {
        }
        player = &D_80121D80[actor->index];
        makeFixedRotationXY(sp2C, player->pitch, player->yaw);
        transformVec3iByFixedMatrix(sp2C, &actor->velocity, &actor->pos);

        player = &D_80121D80[actor->index];
        actor->pos.x += player->pos28.x;
        actor->pos.y += player->pos28.y + actor->verticalVelocity;
        actor->pos.z += player->pos28.z;
        actor->verticalAcceleration += 0xFFFF0000;
        actor->verticalVelocity += actor->verticalAcceleration;
        if (actor->verticalVelocity <= 0) {
            actor->verticalVelocity = 0;
            enqueuePositionalSoundEffect(0x68, &player->pos28, 0x7F, 0x32);
            func_80064414(actor);
            setCallbackTaskCallback(actor, func_80064914);
        }
    }
    addRenderCallback(&D_801248C8, func_80064470, actor);
}

void func_80064C68(RaceUiProjectileActor *arg0) {
    FixedMatrix3sScratch sp2C;
    RacePlayerState *player;
    RaceUiProjectileActor *actor;

    actor = arg0;
    if (gRaceUpdatePaused == 0) {
        makeFixedRotationXY(sp2C, D_80121D80[actor->index].pitch, D_80121D80[actor->index].yaw);
        transformVec3iByFixedMatrix(sp2C, &actor->velocity, &actor->pos);

        player = &D_80121D80[actor->index];
        actor->pos.x += player->pos28.x;
        actor->pos.y += player->pos28.y;
        actor->pos.z += player->pos28.z;
        func_80064414(actor);
        if (actor->flags != 0) {
            setCallbackTaskCallback(actor, func_80064B28);
            actor->verticalAcceleration = 0x50000;
            actor->verticalVelocity += 0x50000;
        }
    }
    addRenderCallback(&D_801248C8, func_80064470, actor);
}

void func_80064D88(RaceUiProjectileActor *arg0) {
    FixedMatrix3sScratch sp2C;
    RacePlayerState *player;
    RaceUiProjectileActor *actor;

    actor = arg0;
    if (gRaceUpdatePaused == 0) {
        makeFixedRotationXY(sp2C, D_80121D80[actor->index].pitch, D_80121D80[actor->index].yaw);
        transformVec3iByFixedMatrix(sp2C, &actor->velocity, &actor->pos);

        player = &D_80121D80[actor->index];
        actor->pos.x += player->pos28.x;
        actor->pos.y += player->pos28.y + actor->verticalVelocity;
        actor->pos.z += player->pos28.z;
        actor->verticalVelocity += actor->verticalAcceleration;
        actor->verticalAcceleration += 0xFFFF0000;
        if (actor->verticalVelocity <= 0) {
            actor->verticalVelocity = 0;
            setCallbackTaskCallback(actor, func_80064C68);
        }
    }
    addRenderCallback(&D_801248C8, func_80064470, actor);
}

void func_80064EAC(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x24) = 0;
    *(s32 *)((u8 *)arg0 + 0x28) = 0x280000;
    *(s32 *)((u8 *)arg0 + 0x2C) = 0;
    *(s16 *)((u8 *)arg0 + 0x3C) = 0;
    *(s32 *)((u8 *)arg0 + 0x4C) = 0;
    *(s32 *)((u8 *)arg0 + 0x50) = 0x300000;
    *(s32 *)((u8 *)arg0 + 0x54) = 0;
    func_800643B4(arg0, D_800D6520);
    enqueuePositionalSoundEffect(0x66, &D_80121D80[*(u16 *)((u8 *)arg0 + 0x10)].pos28, 0x7F, 0x32);
    setCallbackTaskCallback(arg0, func_80064D88);
}

void func_80064F4C(void *arg0) {
    void *temp_s0;

    temp_s0 = arg0;
    if (gCurrentViewportIndex == 0) {
        if (*(s8 *)((u8 *)arg0 + 0x30) != 0) {
            func_80045A78((s16)(*(s32 *)((u8 *)temp_s0 + 0x1C) - 0x66), *(s16 *)((u8 *)temp_s0 + 0x22), getRelocatableHeapBlockBase(D_8011216E), 0x98);
            func_80059A04((u8 *)temp_s0 + 0x2C, *(s32 *)((u8 *)temp_s0 + 0x1C) + 0x26, *(s32 *)((u8 *)temp_s0 + 0x20), 0xC);
            return;
        }
        func_80045A78((s16)(*(s32 *)((u8 *)temp_s0 + 0x1C) - 0x62), *(s16 *)((u8 *)temp_s0 + 0x22), getRelocatableHeapBlockBase(D_8011216E), 0x97);
        func_80059A04((u8 *)temp_s0 + 0x2C, *(s32 *)((u8 *)temp_s0 + 0x1C) + 0x22, *(s32 *)((u8 *)temp_s0 + 0x20), 0x10);
    }
}

void func_8006501C(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x1C) -= *(s32 *)((u8 *)arg0 + 0x28);
    *(s32 *)((u8 *)arg0 + 0x28) += 4;
    if (*(s32 *)((u8 *)arg0 + 0x28) == 0x38) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&D_80124878, func_80064F4C, arg0);
    }
}

void func_8006507C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(u16 *)((u8 *)arg0 + 0x18) - 1;
    if (*(u16 *)((u8 *)arg0 + 0x18) == 0) {
        setCallbackTaskCallback(arg0, func_8006501C);
    }
    addRenderCallback(&D_80124878, func_80064F4C, arg0);
}

void func_800650D8(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x1C) -= *(s32 *)((u8 *)arg0 + 0x28);
    *(s32 *)((u8 *)arg0 + 0x28) -= 4;
    if (*(s32 *)((u8 *)arg0 + 0x28) == 0) {
        *(s16 *)((u8 *)arg0 + 0x18) = 0x5A;
        setCallbackTaskCallback(arg0, func_8006507C);
    }
    addRenderCallback(&D_80124878, func_80064F4C, arg0);
}

void func_80065144(void *arg0) {
    s32 v0;
    *(s32 *)((u8 *)arg0 + 0x20) = -0x2C;
    *(s32 *)((u8 *)arg0 + 0x1C) = 0x1A4;
    *(s32 *)((u8 *)arg0 + 0x28) = 0x38;
    v0 = func_8007B130(&D_80121B74, (RaceTimer *)&gGameSaveDataBuffer[gRaceCourseIndex * 4 + 0x12A],
                       (RaceTimer *)((u8 *)arg0 + 0x2C));
    *(s8 *)((u8 *)arg0 + 0x30) = v0;
    setCallbackTaskCallback(arg0, func_800650D8);
}

// func_800651BC best match: 353 asm-differ differences
#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_800651BC.s")

#ifdef NON_MATCHING
void func_800651BC(RaceUiGfxCommandActor *arg0) {
    RaceUiGfxCommandScriptEntry *entry;
    s16 textureIndex;
    s32 i;
    register RaceUiGfxCommandActor *actor;
    void *spA0;
    void *sp9C;
    Gfx *temp_v0_3;
    Gfx *temp_v0_4;
    Gfx *temp_v0_2;
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
    unsigned int new_var2;
    int new_var;

    actor = arg0;
    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);

    textureIndex = -1;
    entry = D_800D693C[gRaceCourseIndex];
    i = 0;
    if (entry->sentinel != -1) {
        do {
            if ((entry->active != 0) && (isPositionNearCurrentViewport(&entry->command) != 0)) {
                if (textureIndex != entry->sentinel + actor->textureOffset) {
                    textureIndex = entry->sentinel + actor->textureOffset;
                    func_80045990(getRelocatableHeapBlockBase(D_8011216E), 0x14, &spA0, &sp9C);
                    temp_v0_2 = gRegionAllocPtr++;
                    temp_v0_2->words.w0 = 0xFD500000;
                    temp_v0_2->words.w1 = (u32)spA0;
                    temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xF5500000; temp_v0_3->words.w1 = 0x07080200;
                    temp_v0_4 = gRegionAllocPtr++;
                    temp_v0_4->words.w1 = 0;
                    temp_v0_4->words.w0 = 0xE6000000;
                    temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w0 = 0xF3000000; temp_v0_5->words.w1 = 0x070FF400;
                    temp_v0_6 = gRegionAllocPtr++;
                    temp_v0_6->words.w1 = 0;
                    temp_v0_6->words.w0 = 0xE7000000;
                    temp_v0_7 = gRegionAllocPtr++;
                    temp_v0_7->words.w1 = 0x00080200;
                    temp_v0_7->words.w0 = 0xF5400400;
                    temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x0007C07C;
                    temp_v0_9 = gRegionAllocPtr++;
                    temp_v0_9->words.w0 = 0xFD100000;
                    temp_v0_9->words.w1 = (u32)sp9C;
                    temp_v0_10 = gRegionAllocPtr++;
                    temp_v0_10->words.w1 = 0;
                    temp_v0_10->words.w0 = 0xE8000000;
                    temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w1 = 0x07000000; temp_v0_11->words.w0 = 0xF5000100;
                    temp_v0_12 = gRegionAllocPtr++;
                    temp_v0_12->words.w1 = 0;
                    temp_v0_12->words.w0 = 0xE6000000;
                    temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w0 = (new_var = 0xF0000000); temp_v0_13->words.w1 = 0x0703C000;
                    temp_v0_14 = gRegionAllocPtr++;
                    temp_v0_14->words.w1 = 0;
                    temp_v0_14->words.w0 = 0xE7000000;
                }
                gSPMatrix(gRegionAllocPtr++, (u32)actor->particles + (i << 6), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                {
                    Gfx *_g = gRegionAllocPtr++;
                    new_var2 = 0x01;
                    _g->words.w0 = (((unsigned int)((((unsigned int)1) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int)((((unsigned int)((0x00 | 0x00) | 0x00)) & ((new_var2 << 8) - 1)) << 16))) | ((((unsigned int)(sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0);
                    _g->words.w1 = (unsigned int)D_80156614;
                }

                temp_v0_15 = gRegionAllocPtr++;
                temp_v0_15->words.w1 = (u32)D_800D6968;
                temp_v0_15->words.w0 = 0x0400103F;

                temp_v0_16 = gRegionAllocPtr++;
                temp_v0_16->words.w1 = 0x00060200;
                temp_v0_16->words.w0 = 0xB1060402;
            }
            entry++;
            i++;
        } while (entry->sentinel != -1);
    }

    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
}
#endif

void func_80065508(RaceUiGfxCommandActor *arg0) {
    RaceUiGfxCommandScriptEntry *entry;
    Vec3i *pos;
    RaceUiGfxCommandActor *actor;
    register s32 xzSize;
    register s32 ySize;
    register s32 sentinel;

    entry = D_800D693C[gRaceCourseIndex];
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
    pos = &entry->command;

loop:
    if (entry->active != 0) {
        if (func_80048E60(pos) != 0) {
            if (func_80088E98(pos, xzSize, ySize, 0) != 0) {
                entry->active = 0;
                enqueuePositionalSoundEffect(0x18, pos, 0x7F, 0x32);
                D_80121D80[0].unk570++;
                if (D_80121D80[0].unk2DA != 0) {
                    D_80121D80[0].unk2DA = 0xF0;
                } else {
                    func_80083CFC((struct RaceInputPlayer *)D_80121D80);
                }
                if (D_80121D80[0].unk2DA) {
                }
            }
        }
    }

    entry++;
    pos = &entry->command;
    if (entry->sentinel != sentinel) {
        goto loop;
    }

done:
    addRenderCallback(&D_801248D4, func_800651BC, actor);
}

void func_8006565C(RaceUiGfxCommandActor *arg0) {
    register RaceUiGfxCommandActor *actor1;
    register RaceUiGfxCommandActor *actor2;
    register RaceUiGfxCommandScriptEntry *script;
    register s32 i;
    register s32 offset;
    register s32 one;

    script = D_800D693C[gRaceCourseIndex];
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
            func_80048C90(&actor1->particles[i], script->command);
            i++;
            offset += sizeof(RaceUiGfxCommandDest);
            script++;
        } while (i < actor2->count);
    }
    osWritebackDCache(actor1->particles, actor1->count * sizeof(RaceUiGfxCommandDest));
}

void func_80065764(void *arg0) {
    RaceUiGfxCommandScriptEntry *var_v0;
    s32 var_v1;

    var_v0 = D_800D693C[gRaceCourseIndex];
    var_v1 = 0;
    if (var_v0->sentinel != -1) {
        do {
            var_v1++;
            var_v0++;
        } while (var_v0->sentinel != -1);
    }
    D_801222F2 = var_v1;
    *(s16 *)((u8 *)arg0 + 0x1E) = var_v1;
    if (var_v1 != 0) {
        D_80112130.resultTextHandle = allocRelocatableHeapBlock(var_v1 << 6);
        *(s32 *)((u8 *)arg0 + 0x18) = getRelocatableHeapBlockBase(D_80112130.resultTextHandle);
        func_8006565C(arg0);
        setCallbackTaskCallback(arg0, func_80065508);
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
        if (D_80156609 != 0) {
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
 do { if (arg0->matrix != NULL) { temp_v0 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0->words.w0 = 0x06000000; temp_v0->words.w1 = (u32) D_800D6270; temp_v0_2 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_2->words.w0 = 0xFD480017; temp_v0_2->words.w1 = (u32) arg0->palette3A; temp_v0_3 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_3->words.w0 = 0xF5480600; temp_v0_3->words.w1 = 0x07080200; temp_v0_4 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_4->words.w1 = 0; temp_v0_4->words.w0 = 0xE6000000; temp_v0_5 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_5->words.w0 = 0xF4000000; temp_v0_5->words.w1 = 0x070600A0; temp_v0_6 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_6->words.w1 = 0; temp_v0_6->words.w0 = 0xE7000000; temp_v0_7 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_7->words.w0 = 0xF5400600; temp_v0_7->words.w1 = 0x00080200; temp_v0_8 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_8->words.w0 = 0xF2000000; temp_v0_8->words.w1 = 0x000C00A0; temp_v0_9 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_9->words.w0 = 0xFD100000; temp_v0_9->words.w1 = (u32) arg0->image3A; temp_v0_10 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_10->words.w1 = 0; temp_v0_10->words.w0 = 0xE8000000; temp_v0_11 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_11->words.w0 = 0xF5000100; temp_v0_11->words.w1 = 0x07000000; temp_v0_12 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_12->words.w1 = 0; temp_v0_12->words.w0 = 0xE6000000; temp_v0_13 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_13->words.w0 = 0xF0000000; temp_v0_13->words.w1 = 0x0703C000; temp_v0_14 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_14->words.w1 = 0; temp_v0_14->words.w0 = 0xE7000000; temp_v0_15 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_15->words.w0 = 0x01020040; temp_v0_15->words.w1 = (u32) arg0->matrix; temp_v0_16 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_16->words.w0 = 0x01000040; temp_v0_16->words.w1 = D_80156614; temp_v0_17 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_17->words.w0 = 0x0400207F; temp_v0_17->words.w1 = (u32) D_800D69A8; temp_v0_18 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_18->words.w0 = 0xB1060402; temp_v0_18->words.w1 = 0x00060200; temp_v0_19 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_19->words.w0 = 0xFD480017; temp_v0_19->words.w1 = (u32) arg0->palette3B; temp_v0_20 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_20->words.w0 = 0xF5480600; temp_v0_20->words.w1 = 0x07080200; temp_v0_21 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_21->words.w1 = 0; temp_v0_21->words.w0 = 0xE6000000; temp_v0_22 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_22->words.w0 = 0xF4000000; temp_v0_22->words.w1 = 0x070600A0; temp_v0_23 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_23->words.w1 = 0; temp_v0_23->words.w0 = 0xE7000000; temp_v0_24 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_24->words.w0 = 0xF5400600; temp_v0_24->words.w1 = 0x00080200; temp_v0_25 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_25->words.w0 = 0xF2000000; temp_v0_25->words.w1 = 0x000C00A0; temp_v0_26 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_26->words.w0 = 0xFD100000; temp_v0_26->words.w1 = (u32) arg0->image3B; temp_v0_27 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_27->words.w1 = 0; temp_v0_27->words.w0 = 0xE8000000; temp_v0_28 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_28->words.w0 = 0xF5000100; temp_v0_28->words.w1 = 0x07000000; temp_v0_29 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_29->words.w1 = 0; temp_v0_29->words.w0 = 0xE6000000; temp_v0_30 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_30->words.w0 = 0xF0000000; temp_v0_30->words.w1 = 0x0703C000; temp_v0_31 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_31->words.w1 = 0; temp_v0_31->words.w0 = 0xE7000000; temp_v0_32 = (*((RaceUiDisplayCommand **) (&gRegionAllocPtr)))++; temp_v0_32->words.w0 = 0xB10E0C0A; temp_v0_32->words.w1 = 0x000E0A08; } } while (0);
        } while (0);
    } while (0);
}

void func_80065CB8(void *arg0) {
    s16 temp_v1;
    void *temp_a2;

    temp_v1 = *(s16 *)((u8 *)arg0 + 0x30);
    temp_a2 = arg0;
    if (temp_v1 != 0) {
        *(s32 *)((u8 *)arg0 + 0x1C) += *(s32 *)((u8 *)arg0 + 0x28);
        *(s32 *)((u8 *)arg0 + 0x28) += 0x2000;
        *(s16 *)((u8 *)arg0 + 0x30) = temp_v1 - 1;
    } else {
        removeCallbackTask(temp_a2);
        return;
    }
    addRenderCallback(&D_801248BC, func_80065808, temp_a2);
}

void func_80065D24(RaceUiOverlayActor *arg0) {
    s16 temp_v0;
    s32 temp_v0_2;
    s32 temp_a1;
    void *temp_v0_3;
    RaceUiOverlayActor *temp_s0;

    temp_v0 = arg0->timer;
    temp_s0 = arg0;
    if (temp_v0 == 0) {
        if (arg0->assetTimer == 0) {
            enqueuePositionalSoundEffect(9, &temp_s0->x, 0x7F, 0x32);
        }
        temp_v0_3 = getRelocatableHeapBlockBase(D_8011216E);
        temp_a1 = temp_s0->assetTimer;
        temp_a1 = (temp_a1 >> 1) + 0x3B;
        temp_a1 = (u16)temp_a1;
        func_80045990(temp_v0_3, temp_a1, &temp_s0->palette3B, &temp_s0->image3B);
        temp_s0->assetTimer++;
        if (temp_s0->assetTimer >= 0xB) {
            temp_s0->timer = 0x3C;
            setCallbackTaskCallback(temp_s0, func_80065CB8);
        }
    } else {
        temp_s0->timer = temp_v0 - 1;
    }

    temp_v0_2 = temp_s0->velocity;
    temp_s0->y += temp_v0_2;
    temp_s0->velocity = temp_v0_2 + 0xC00;
    addRenderCallback(&D_801248BC, func_80065808, temp_s0);
}

void func_80065E0C(void *arg0) {
    s16 v1 = *(s16 *)((u8 *)arg0 + 0x30);
    if (v1 != 0) {
        *(s32 *)((u8 *)arg0 + 0x1C) -= *(s32 *)((u8 *)arg0 + 0x28);
        *(s32 *)((u8 *)arg0 + 0x28) -= 0x2000;
        *(s16 *)((u8 *)arg0 + 0x30) = v1 - 1;
    } else {
        *(s16 *)((u8 *)arg0 + 0x30) = 0x14;
        *(s16 *)((u8 *)arg0 + 0x32) = 0;
        setCallbackTaskCallback(arg0, func_80065D24);
    }
    addRenderCallback(&D_801248BC, func_80065808, arg0);
}

void func_80065E90(RaceUiOverlayActor *arg0) {
    s32 i;

    arg0->x = 0;
    arg0->z = D_80121DA4 + 0xFF900000;
    arg0->velocity = 0;
    arg0->y = func_80080CC4(func_8007D200(0, 0, arg0->z), arg0->x, arg0->z) + 0x200000;

    for (i = 0; i < 0x40; i++) {
        arg0->velocity += 0x2000;
        arg0->y += arg0->velocity;
        arg0->timer++;
    }

    func_80045990(getRelocatableHeapBlockBase(D_8011216E), 0x3A, &arg0->palette3A, &arg0->image3A);
    func_80045990(getRelocatableHeapBlockBase(D_8011216E), 0x3B, &arg0->palette3B, &arg0->image3B);
    setCallbackTaskCallback(arg0, func_80065E0C);
}

void func_80065FD8(RaceUiRankTextRenderActor *arg0) {
    RaceUiRankTextRenderEntry *var_s4;
    s32 var_s6;
    s32 var_fp;
    Gfx *temp_s0;
    Gfx *temp_s2;
    Gfx *temp_s3;

    var_s4 = D_800D761C[gRaceCourseIndex];
    var_fp = TRUE;
    var_s6 = 0;
    if (var_s4->active != -1) {
        do {
            if ((var_s4->active != 0) && (isPositionNearCurrentViewport(&var_s4->position) != 0)) {
                if (var_fp != 0) {
                    gDPPipeSync(gRegionAllocPtr++);

                    temp_s2 = gRegionAllocPtr++;
                    var_fp = FALSE;
                    gSPSegment(temp_s2, 0x02, getRelocatableHeapBlockBase(D_80112130.modelVtxHandle));

                    temp_s3 = gRegionAllocPtr++;
                    gSPSegment(temp_s3, 0x03, getRelocatableHeapBlockBase(D_80112130.modelTextureHandle));
                }

                temp_s0 = gRegionAllocPtr++;
                gSPMatrix(temp_s0, (u32)arg0->matrices + (var_s6 << 6), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                temp_s0 = gRegionAllocPtr++;
                gSPDisplayList(temp_s0, D_20019C0);
            }
            var_s4++;
            var_s6++;
        } while (var_s4->active != -1);
    }
}

void func_80066158(void *arg0) {
    volatile u8 pad[8];
    RaceUiRankTextRenderEntry *entry;
    RaceUiRankTrigger *trigger;
    s32 radius;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 i;

    entry = D_800D761C[gRaceCourseIndex];
    if (entry->active != -1) {
        do {
            if (entry->active != 0) {
                func_80088294(&entry->position, 0xE0000, 0x100000, 2);
                trigger = D_8012228C;
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
                                            func_800604CC((void *)entry->position.x, (void *)(entry->position.y + 0x70000),
                                                          (void *)entry->position.z, i);
                                            i++;
                                        } while (i != 8);
                                        entry->active = 0;
                                        D_801222F4++;
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

    addRenderCallback(&D_801248B0, func_80065FD8, arg0);
}

void func_800663C8(RaceUiRankTextRenderActor *arg0) {
    s16 scratch[0x10];
    RaceUiRankTextRenderEntry *entry;
    RaceUiRankTextRenderActor *actor1;
    RaceUiRankTextRenderActor *actor2;
    s32 i;
    s32 active;
    s16 count;

    actor1 = arg0;
    actor2 = arg0;
    entry = D_800D761C[gRaceCourseIndex];
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
            func_800486BC(scratch, (void *)((u32)actor1->matrices + (i << 6)));
            entry->active = active;
            i++;
            entry++;
        } while (i < actor2->count);
        count = actor1->count;
    }
    osWritebackDCache(actor1->matrices, count << 6);
}

void func_800666B0(RaceUiRankTextRenderActor *arg0) {
    RaceUiRankTextRenderEntry *var_v0;
    s32 var_v1;

    var_v0 = D_800D761C[gRaceCourseIndex];
    var_v1 = 0;
    if (var_v0->active != -1) {
        do {
            var_v1++;
            var_v0++;
        } while (var_v0->active != -1);
    }
    D_801222F6 = var_v1;
    arg0->count = var_v1;
    if (var_v1 != 0) {
        D_80112130.rankTextHandle = allocRelocatableHeapBlock(var_v1 << 6);
        arg0->matrices = (void *)getRelocatableHeapBlockBase(D_80112130.rankTextHandle);
        func_800663C8(arg0);
        setCallbackTaskCallback(arg0, func_80066158);
    }
}
