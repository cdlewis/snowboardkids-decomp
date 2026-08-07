#include "game/race/race_state.h"
#include "common.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/engine/render_callback.h"
#include "game/engine/system_runtime.h"
#include "assets.h"
#include "game/race/course/race_course_effects.h"
#include "game/race/player/race_player_input.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/math/spatial_math.h"
#include "game/math/fixed_point_math.h"
#include "game/race/motion/race_motion.h"
#include "game/race/player/race_player_movement.h"

#define COURSE_INDEX_RELOAD (*(volatile s16 *)&gRaceCourseIndex.signedValue)
#define ASSET_HANDLE(index) (gAssetHandles[(index)])
#define RACE_COURSE_EFFECTS_GFX_CMD(pkt, cmd0, cmd1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (cmd0); \
    _g->words.w1 = (cmd1); \
}

typedef struct RaceCountdownEffect {
    char pad[0x18];
    s16 step;
    u16 timer;
} RaceCountdownEffect;

typedef struct RacePlayerEffect {
    char pad[0x10];
    u16 playerIndex;
} RacePlayerEffect;

typedef struct RaceCourseRenderEffect {
    char pad0[0x18];
    Mtx *vertices;
} RaceCourseRenderEffect;

typedef struct {
    s16 displayListIndex;
    s16 pad2;
    Vec3i position;
    s16 rotation;
    s16 pad12;
} CourseRenderEntry;

typedef struct {
    s8 type;
    char pad1[3];
    Vec3i pos;
} CourseMarkerSpawnEntry;

typedef struct PatrolCourseObjectEffect {
    char pad[0x18];
    Vec3i pos;
    Vec3i startPos;
    s32 endX;
    char pad34[4];
    s32 endZ;
    s16 surfaceIndex;
    s16 angle;
    s16 unk40;
    s16 pad42;
    s16 displayListValid;
    s16 pad46;
    void *displayList;
    s16 unk4C;
    s16 unk4E;
    s32 unk50;
} PatrolCourseObjectEffect;

typedef struct {
    s32 dz;
    Vec3i *volatile pos;
} PatrolCourseObjectUpdateLocals;

typedef struct CourseGateObjectEffect {
    char pad0[0x18];
    FixedTransform source;
    Vec3i pos1;
    Vec3i pos2;
    s16 unk50;
    s16 unk52;
    s16 unk54;
    s16 unk56;
    void *sourceMatrix;
    void *pos1Matrix;
    void *pos2Matrix;
} CourseGateObjectEffect;

typedef struct RaceMovingEffect {
    char pad0[0x18];
    Vec3i velocity;
    Vec3i pos;
    FixedTransform unk30;
    s16 timer;
    s16 unk52;
    void *matrix;
} RaceMovingEffect;

typedef struct RaceCourseMarkerEffect {
    char pad0[0x10];
    u16 entryIndex;
    char pad12[6];
    void *texture;
    void *palette;
    s16 rotation;
    char pad22[2];
    Vtx *vertices;
    Vtx *baseVertices;
    s16 vertexCount;
    char pad2E[2];
    s32 texturePtr;
    s32 palettePtr;
    s32 useAltQueue;
    s32 unk3C;
} RaceCourseMarkerEffect;

typedef struct {
    s32 texturePtr;
    s32 palettePtr;
    void *baseVerticesInput;
    s16 vertexCount;
    s16 flags;
    u16 textureIndex;
    s16 unk12;
} CourseMarkerEntry;

typedef struct {
    s16 scaleX;
    s16 scaleY;
    s16 scaleZ;
    s16 unk6;
    Vec3i pos;
    s16 pitch;
    s16 yaw;
    s32 displayList;
} CourseTriggerEntry;

typedef union {
    u16 raw[32];
    CourseTriggerEntry entries[2];
} CourseTriggerEntryData;

typedef struct {
    Vec3i dest;
    Vec3i source;
    FixedTransform transform;
    s32 pad38;
} CourseTriggerScratch;

typedef struct RaceCourseTriggerEffect {
    char pad0[0x10];
    u16 entryIndex;
    char pad12[6];
    void *matrix;
    Vec3i pos1;
    Vec3i pos2;
    s16 pitch;
    s16 yaw;
    s32 scaleX;
    s32 scaleY;
    s32 scaleZ;
    s32 displayList;
} RaceCourseTriggerEffect;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s16 angle;
    s16 unkE;
} SoundParams;

typedef struct {
    char pad0[0x44];
    Vec3i transformOffset;
    char pad50[0x60];
} RaceCamera;

typedef struct RaceCourseBackdropEffect {
    char pad0[0x44];
    void *matrix;
} RaceCourseBackdropEffect;

Vtx D_800D9C40[] = {
    {{{-36, 144, 0}, 0x0000, {-16, -16}, {228, 228, 228, 255}}},
    {{{36, 144, 0}, 0x0000, {1008, -16}, {228, 228, 228, 255}}},
    {{{36, -8, 0}, 0x0000, {1008, 2032}, {228, 228, 228, 255}}},
    {{{-36, -8, 0}, 0x0000, {-16, 2032}, {228, 228, 228, 255}}},
    {{{-36, 144, 0}, 0x0000, {-16, -16}, {228, 228, 228, 255}}},
    {{{36, 144, 0}, 0x0000, {1008, -16}, {228, 228, 228, 255}}},
    {{{36, -8, 0}, 0x0000, {1008, 2032}, {228, 228, 228, 255}}},
    {{{-36, -8, 0}, 0x0000, {-16, 2032}, {228, 228, 228, 255}}},
    {{{-4, 8, 0}, 0x0000, {-16, -16}, {228, 228, 228, 255}}},
    {{{4, 8, 0}, 0x0000, {496, -16}, {228, 228, 228, 255}}},
    {{{4, 0, 0}, 0x0000, {496, 496}, {228, 228, 228, 255}}},
    {{{-4, 0, 0}, 0x0000, {-16, 496}, {228, 228, 228, 255}}},
};

Gfx gEffectRenderModeSetupDl[] = {
    gsDPPipeSync(),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_PASS2),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsSPSetGeometryMode(G_FOG),
    gsSPEndDisplayList(),
};

Gfx gEffectRenderModeCleanupDl[] = {
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_RA_ZB_OPA_SURF, G_RM_RA_ZB_OPA_SURF2),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPClearGeometryMode(G_FOG),
    gsSPEndDisplayList(),
};

CourseMarkerSpawnEntry D_800D9D68[] = {
    {0, {0}, {0xDA6797E0, 0xE8120000, 0xE8618D80}},
    {0, {0}, {0xDA5596C0, 0xE8050000, 0xE8E23CA8}},
    {0, {0}, {0xD95E35E8, 0xE7DF8000, 0xE8D1E9E8}},
    {0, {0}, {0xD953F118, 0xE7CF0000, 0xE98828D8}},
    {0, {0}, {0xD87ED180, 0xE7B28000, 0xE96589B8}},
    {0, {0}, {0xD84BAD48, 0xE7980000, 0xEA4D1788}},
    {0, {0}, {0xD73764B0, 0xE7708000, 0xEA46A970}},
    {0, {0}, {0xD6D24718, 0xE7460000, 0xEBBB5D50}},
    {0, {0}, {0xD57CF2F0, 0xE7050000, 0xEC279070}},
    {0, {0}, {0xD577DA48, 0xE6CF8000, 0xED718F20}},
    {0, {0}, {0xD4B5F828, 0xE6A18000, 0xEDB78D18}},
    {0, {0}, {0xD4DDCE30, 0xE66A0000, 0xEEB211B0}},
    {0, {0}, {0xD3CA7C40, 0xE61A0000, 0xEF1E5DB8}},
    {0, {0}, {0xD481C800, 0xE5BC0000, 0xF0859FC0}},
    {0, {0}, {0xD3CC9528, 0xE5828000, 0xF0E0F508}},
    {0, {0}, {0xD409A090, 0xE52E8000, 0xF2195078}},
    {0, {0}, {0xD3866C60, 0xE4FA0000, 0xF2BE3BE0}},
    {0, {0}, {0xD96B6500, 0xE7FB8000, 0xE7D9E7E0}},
    {0, {0}, {0xD859CD08, 0xE7D48000, 0xE7F9B050}},
    {0, {0}, {0xD7C2DDF0, 0xE7A58000, 0xE8FA3A88}},
    {0, {0}, {0xD6E1B258, 0xE7858000, 0xE92060E8}},
    {0, {0}, {0xD6A9CBB0, 0xE7668000, 0xEA017B70}},
    {0, {0}, {0xD5C498C0, 0xE7490000, 0xEA290DE0}},
    {0, {0}, {0xD56CE800, 0xE7220000, 0xEB391100}},
    {0, {0}, {0xD4870878, 0xE6F98000, 0xEBB56870}},
    {0, {0}, {0xD450CCE8, 0xE6C28000, 0xECD082E8}},
    {0, {0}, {0xD3807520, 0xE6908000, 0xED524948}},
    {0, {0}, {0xD3A568E8, 0xE6588000, 0xEE3907D8}},
    {0, {0}, {0xD2F6A828, 0xE6240000, 0xEEB42968}},
    {0, {0}, {0xD2EBB590, 0xE5C00000, 0xEFE3BC10}},
    {0, {0}, {0xD28E09F0, 0xE5400000, 0xF170DE80}},
    {0, {0}, {0xCD7E5688, 0xE3048000, 0xF5BE8888}},
    {0, {0}, {0xB041CA10, 0xC4418000, 0xDE974048}},
    {0, {0}, {0xAFE7E4D8, 0xB75D0000, 0xC9182EF0}},
    {0, {0}, {0xB00F6BB8, 0xB7400000, 0xC8C59428}},
    {0, {0}, {0xB034A4F0, 0xB71E8000, 0xC86E5ED8}},
    {0, {0}, {0xB0DF41B8, 0xB6A90000, 0xC7B85530}},
    {0, {0}, {0xB21C3F10, 0xB5E58000, 0xC72DAF10}},
    {0, {0}, {0xB3B42628, 0xB4EA0000, 0xC751B0F0}},
    {0, {0}, {0xB401A730, 0xB4CD0000, 0xC753E9F8}},
    {1, {0}, {0xCD361418, 0xE2EE8000, 0xF57850C0}},
    {1, {0}, {0xCCEE6A70, 0xE2B78000, 0xF4B062D0}},
    {1, {0}, {0xB7162340, 0xCC288000, 0xEC76E730}},
    {1, {0}, {0xB5EFCAD0, 0xCBF68000, 0xEC39E9A0}},
    {1, {0}, {0xB56C12F8, 0xCBE08000, 0xEC0BDFB0}},
    {1, {0}, {0xB0C36918, 0xC4C08000, 0xDF47EDD8}},
    {-1, {0}, {0x00000000, 0x00000000, 0x00000000}},
};

CourseMarkerSpawnEntry D_800DA058[] = {
    {2, {0}, {0x28C8B179, 0xD113BFA6, 0xC9F84D48}},
    {2, {0}, {0x29477435, 0xD084A801, 0xCC351004}},
    {2, {0}, {0x2A2C03A7, 0xD0016D55, 0xCE225B3E}},
    {2, {0}, {0x2AC2EC87, 0xCFF28C55, 0xCDE30266}},
    {2, {0}, {0x2CBF0511, 0xCF49D858, 0xCEDEA806}},
    {-1, {0}, {0x00000000, 0x00000000, 0x00000000}},
};

CourseMarkerSpawnEntry *gCourseTextureMarkerSpawnEntriesByCourse[] = {
    D_800D9D68, D_800DA058,
};

Vtx D_800DA0C0[] = {
    {{{-40, 80, -136}, 0x0000, {0, 0}, {30, 25, 35, 255}}},
    {{{-42, 82, -136}, 0x0000, {0, 0}, {30, 25, 35, 255}}},
    {{{-42, 742, -1736}, 0x0000, {0, 0}, {0, 0, 0, 255}}},
    {{{-40, 740, -1736}, 0x0000, {0, 0}, {0, 0, 0, 255}}},
    {{{40, 80, -136}, 0x0000, {0, 0}, {30, 25, 35, 255}}},
    {{{42, 82, -136}, 0x0000, {0, 0}, {30, 25, 35, 255}}},
    {{{42, 742, -1736}, 0x0000, {0, 0}, {0, 0, 0, 255}}},
    {{{40, 740, -1736}, 0x0000, {0, 0}, {0, 0, 0, 255}}},
};

Gfx D_800DA140[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsSPSetGeometryMode(G_FOG),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPVertex(D_800DA0C0, 8, 0),
    gsSP1Quadrangle(1, 2, 3, 0, 0),
    gsSP1Quadrangle(3, 2, 1, 0, 0),
    gsSP1Quadrangle(5, 6, 7, 4, 0),
    gsSP1Quadrangle(7, 6, 5, 4, 0),
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_RA_ZB_OPA_SURF, G_RM_RA_ZB_OPA_SURF2),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPClearGeometryMode(G_FOG),
    gsSPEndDisplayList(),
};

void *gRaceCourseSceneryDisplayLists[] = {
    (void *) 0x02000000,
    (void *) 0x02000160,
    (void *) 0x02000000,
    (void *) 0x020002C0,
    (void *) 0x02000410,
    (void *) 0x020028F0,
    (void *) 0x02002DB8,
    (void *) 0x02000000,
    (void *) 0x02000568,
    (void *) 0x02000910,
    (void *) 0x02000B30,
    (void *) 0x02001350,
};
Gfx *gThrownPickupModelDisplayList[] = {
    (Gfx *) 0x02001548,
    (Gfx *) 0x02001678,
    (Gfx *) 0x02001730,
    (Gfx *) 0x02001810,
    (Gfx *) 0x020018E8,
    (Gfx *) 0x020019C0,
    (Gfx *) 0x02001D00,
    (Gfx *) D_800DA140,
    (Gfx *) 0x020020A8,
    (Gfx *) 0x02002208,
    (Gfx *) 0x020023A8,
    (Gfx *) 0x02002490,
    (Gfx *) 0x02002660,
    (Gfx *) 0x02002ED0,
};

CourseRenderEntry D_800DA228[] = {
    {0, 0, {0x01D014A8, 0xFF6C4D78, 0xFB9EA3E0}, 608, 0},
    {0, 0, {0x0240BB14, 0xFF6C4D78, 0xFA935474}, 608, 0},
    {1, 0, {0xD23466CE, 0xE42B18E0, 0xF5EA0624}, 2304, 0},
    {4, 0, {0xCFB74EEE, 0xDE1498E0, 0xEAC21300}, 2304, 0},
    {8, 0, {0xF97016E4, 0x00DBFC9A, 0xFD0C374C}, 3632, 0},
    {8, 0, {0xC2E81822, 0xAC908000, 0xD1AD9DFC}, 2608, 0},
    {19, 0, {0xC2E81822, 0xAC908000, 0xD1AD9DFC}, 2608, 0},
    {17, 0, {0xCFAB11F8, 0xE3673C32, 0xF4590B3E}, 1216, 0},
    {17, 0, {0xC14A2B73, 0xACD9B80F, 0xCC23BA7C}, 2992, 0},
    {25, 0, {0xFFEC52BC, 0x0005AEED, 0x00953930}, 2608, 0},
    {25, 0, {0xC48EF729, 0xAC960000, 0xD0CC2D2F}, 5152, 0},
    {25, 0, {0xC4D7BE2D, 0xAC960000, 0xD06F5859}, 5152, 0},
    {10, 0, {0xC46056B3, 0xAC960000, 0xCE3C5977}, 3664, 0},
    {-1, 0, {0x00000000, 0x00000000, 0x00000000}, 0, 0},
};

CourseRenderEntry D_800DA340[] = {
    {8, 0, {0xF88F8010, 0x01B3C704, 0x08B7C216}, 3952, 0},
    {8, 0, {0x25AB1286, 0xB99EAFDE, 0xA418BB3C}, 736, 0},
    {19, 0, {0x25AB1286, 0xB99EAFDE, 0xA418BB3C}, 736, 0},
    {10, 0, {0xFC683074, 0x00117426, 0xFCE0F666}, 672, 0},
    {11, 0, {0xF271CB3A, 0xF24BC074, 0xE9E7C75E}, 1392, 0},
    {18, 0, {0xF5186325, 0xF35DB06F, 0xEC2A2C50}, 1424, 0},
    {18, 0, {0xF3CB2861, 0xF332306F, 0xEB1AD400}, 1424, 0},
    {-1, 0, {0x00000000, 0x00000000, 0x00000000}, 0, 0},
};

CourseRenderEntry D_800DA3E0[] = {
    {8, 0, {0xFB3C1234, 0x0166050E, 0x02974782}, 0, 0},
    {8, 0, {0x1678D7C8, 0xD0575952, 0xC957E20F}, 2192, 0},
    {19, 0, {0x1678D7C8, 0xD0575952, 0xC957E20F}, 2192, 0},
    {-1, 0, {0x00000000, 0x00000000, 0x00000000}, 0, 0},
};

CourseRenderEntry D_800DA430[] = {
    {8, 0, {0x053E07E6, 0x00874833, 0xFA36E885}, 1808, 0},
    {8, 0, {0xB3F31DAE, 0xD0500000, 0x9B0A372B}, 2000, 0},
    {19, 0, {0xB3F31DAE, 0xD0500000, 0x9B0A372B}, 2000, 0},
    {-1, 0, {0x00000000, 0x00000000, 0x00000000}, 0, 0},
};

CourseRenderEntry D_800DA480[] = {
    {11, 0, {0x05A47A7A, 0xD7FCE5DB, 0xAFF3326D}, 3712, 0},
    {11, 0, {0x040EFAF2, 0xD8D2DC67, 0xB3E951AB}, 1120, 0},
    {11, 0, {0x095DF1CC, 0xD9C54381, 0xB2FF5E19}, 4080, 0},
    {8, 0, {0x01A91FD6, 0xD2600000, 0xA317F7BF}, 1536, 0},
    {19, 0, {0x01A91FD6, 0xD2600000, 0xA317F7BF}, 1536, 0},
    {8, 0, {0xFB2519B1, 0x018E6620, 0x03090886}, 4064, 0},
    {18, 0, {0x0452B064, 0xF5C87EA5, 0xE0367FE4}, 832, 0},
    {18, 0, {0x03D5887A, 0xF66BDEA5, 0xE1592132}, 832, 0},
    {18, 0, {0x059C3BB0, 0xF65E9EA5, 0xE1E2E754}, 2912, 0},
    {18, 0, {0x05ECAF34, 0xF5C69EA5, 0xE0AD1F0E}, 2912, 0},
    {18, 0, {0x13B2B871, 0xEB826EEA, 0xDE2EA200}, 3376, 0},
    {18, 0, {0x147006D7, 0xEB860EEA, 0xDF91B288}, 3360, 0},
    {18, 0, {0x163EB5B3, 0xEA584EEA, 0xDE76D2D8}, 2352, 0},
    {18, 0, {0x15935139, 0xEA504EEA, 0xDD1127D0}, 304, 0},
    {18, 0, {0x17187BE1, 0xEA044EEA, 0xDC43F6B2}, 1312, 0},
    {18, 0, {0x17D35A51, 0xEA072EEA, 0xDD93D6BA}, 1312, 0},
    {-1, 0, {0x00000000, 0x00000000, 0x00000000}, 0, 0},
};

CourseRenderEntry D_800DA5D4[] = {
    {8, 0, {0xFCA42745, 0x0169B6DA, 0xF7136E88}, 3008, 0},
    {8, 0, {0x46B6BF2E, 0xD8100000, 0x04CBF586}, 2704, 0},
    {19, 0, {0x46B6BF2E, 0xD8100000, 0x04CBF586}, 2704, 0},
    {-1, 0, {0x00000000, 0x00000000, 0x00000000}, 0, 0},
};

CourseRenderEntry D_800DA624[] = {
    {8, 0, {0x9F401566, 0xDD5C0000, 0xE785C37D}, 3296, 0},
    {19, 0, {0x9F401566, 0xDD5C0000, 0xE785C37D}, 3296, 0},
    {8, 0, {0xF61512AA, 0x00011DE4, 0xF93B37B9}, 3264, 0},
    {-1, 0, {0x00000000, 0x00000000, 0x00000000}, 0, 0},
};

CourseRenderEntry D_800DA674[] = {
    {11, 0, {0xF3C2296E, 0xF2A20000, 0xEE5E0129}, 1792, 0},
    {-1, 0, {0x00000000, 0x00000000, 0x00000000}, 0, 0},
};

CourseRenderEntry D_800DA69C[] = {
    {19, 0, {0x19B8F477, 0xF2F60000, 0xE965AC2E}, 2432, 0},
    {8, 0, {0xFB2EE4C4, 0x013F685D, 0x035893BA}, 3968, 0},
    {-1, 0, {0x00000000, 0x00000000, 0x00000000}, 0, 0},
};

CourseRenderEntry D_800DA6D8[] = {
    {8, 0, {0x14DA6C89, 0xEEC47C98, 0xF4485684}, 3792, 0},
    {19, 0, {0x14DA6C89, 0xEEC47C98, 0xF4485684}, 3792, 0},
    {8, 0, {0x00000000, 0xFFFDFDD7, 0x0003AEE6}, 0, 0},
    {10, 0, {0x1470B7EE, 0xEECD0856, 0xF615DFCF}, 2832, 0},
    {-1, 0, {0x00000000, 0x00000000, 0x00000000}, 0, 0},
};

CourseRenderEntry *gRaceCourseSceneryEntriesByCourse[] = {
    D_800DA228,
    D_800DA340,
    D_800DA3E0,
    D_800DA430,
    D_800DA480,
    D_800DA5D4,
    D_800DA624,
    D_800DA674,
    D_800DA69C,
    D_800DA6D8,
};

SoundParams gCourseGateSoundParams[] = {
    {0xC3623C28, 0xAC940000, 0xD129BAB2, 1584, 0},
    {0x253CC0AA, 0xB9A9E000, 0xA4A99C5C, 3824, 0},
    {0x1720C991, 0xD05E70EE, 0xC940D8EA, 1168, 0},
    {0xB4967EF1, 0xD0530400, 0x9B1C046F, 928, 0},
    {0x021EC3FF, 0xD2640000, 0xA39AFD40, 512, 0},
    {0x471CC8AB, 0xD8140000, 0x044C759E, 1728, 0},
    {0x9F10F80F, 0xDD600000, 0xE6DD4687, 2288, 0},
    {0x00000000, 0x00000000, 0x00000000, 0, 0},
    {0x1A4B7210, 0xF2FA0000, 0xE9069D8E, 1408, 0},
    {0x1442F7DC, 0xEEC80E50, 0xF3F545C2, 2768, 0},
};

CourseMarkerEntry gCourseBillboardMarkerEntries[] = {
    {0x02000060, 0x02000088, (void *) 0x02000000, 6, 1, 3, 0},
    {0x0200BB90, 0x0200BBB8, (void *) 0x0200BA10, 24, 1, 3, 0},
    {0x0200BCC0, 0x0200BCE8, (void *) 0x0200BC00, 12, 1, 3, 0},
};

CourseTriggerEntryData gCourseTriggerEntries = {{
    0x0002, 0x0001, 0x0186, 0x0000, 0x14CB, 0x79E2, 0xD852, 0x20A4,
    0xD28E, 0xDA6A, 0x0F90, 0x0070, 0x0200, 0x9148, 0x0004, 0x0006,
    0x0122, 0x0000, 0xF7B4, 0x2249, 0xF305, 0xD31B, 0xEB76, 0x22AF,
    0x0FBA, 0x0400, 0x0200, 0x6988, 0x0000, 0x0000, 0x0000, 0x0000,
}};

#define gCourseTriggerEntries (gCourseTriggerEntries.entries)

extern void enqueueSoundEffect(s32, s32);
extern void enqueuePositionalSoundEffect(s32, void *, s32, s32);
extern void osWritebackDCache(void *, s32);
extern void *allocMenuRenderScratch(s32);
extern void packFixedTransformMatrix(void *, void *);
extern void *allocFixedTransformMatrix(FixedTransform *);
extern void setPackedMatrixTranslation(Mtx *, Vec3i *);
extern void *resolveAssetTableRelativePointer(void *, u32);
extern void osWritebackDCache(void *, s32);
extern s32 gMenuFlowState;
extern u8 gCurrentViewportIndex;
extern void waitForCourseGateTrigger(CourseGateObjectEffect *);
void renderCourseGateObject(CourseGateObjectEffect *);
void renderCourseTriggerVolume(RaceCourseTriggerEffect *);
void collidePlayerWithCourseTriggerVolume(RacePlayer *, RaceCourseTriggerEffect *);
void updateCourseTriggerVolume(RaceCourseTriggerEffect *);
void drawRaceCountdownReadyPrompt(RaceCountdownEffect *);
void drawRaceCountdownGoPrompt(RaceCountdownEffect *);
void updateRaceCountdownGoPromptOut(RaceCountdownEffect *);
void updateRaceCountdownGoPromptHold(RaceCountdownEffect *);
void updateRaceCountdownReadyPromptIn(RaceCountdownEffect *);
void updateRaceCountdownReadyPromptHold(RaceCountdownEffect *);
void updateRaceCountdownInitialDelay(RaceCountdownEffect *);
void updatePatrolCourseObject(PatrolCourseObjectEffect *);
void initPatrolCourseObject(PatrolCourseObjectEffect *);
void renderRaceCourseSceneryObjects(RaceCourseRenderEffect *);
void renderSpiralCourseObject(RaceMovingEffect *);
extern u8 gRaceUpdatePaused;
extern RaceCamera D_801121E0[];
extern Gfx *gRegionAllocPtr;

void drawRaceCountdownReadyPrompt(RaceCountdownEffect *arg0) {
    if (arg0->step != 0) {
        drawScaledAssetTableSprite(-0x34, -0xC, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)), 0x3F, arg0->step);
    } else {
        drawAssetTableSprite(-0x34, -0xC, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)), 0x3F);
    }
}

void drawRaceCountdownGoPrompt(RaceCountdownEffect *arg0) {
    if (arg0->step != 0) {
        drawScaledAssetTableSprite(-0x20, -0xC, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)), 0x40, arg0->step);
    } else {
        drawAssetTableSprite(-0x20, -0xC, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)), 0x40);
    }
}

void updateRaceCountdownGoPromptOut(RaceCountdownEffect *arg0) {
    arg0->step++;
    if (arg0->step == 4) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceCountdownGoPrompt, arg0);
}

void updateRaceCountdownGoPromptHold(RaceCountdownEffect *arg0) {
    RaceCountdownEffect *temp_a2 = arg0;

    if (arg0->step != 0) {
        arg0->step--;
    }
    temp_a2->timer--;
    if (temp_a2->timer == 0) {
        gMenuFlowState &= ~1;
        setCallbackTaskCallback(temp_a2, (CallbackTaskCallback)updateRaceCountdownGoPromptOut);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceCountdownGoPrompt, temp_a2);
}

void updateRaceCountdownReadyPromptIn(RaceCountdownEffect *arg0) {
    arg0->step++;
    if (arg0->step == 4) {
        enqueueSoundEffect(0x4C, 0x5A);
        arg0->timer = 0x14;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceCountdownGoPromptHold);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceCountdownReadyPrompt, arg0);
}

void updateRaceCountdownReadyPromptHold(RaceCountdownEffect *arg0) {
    if (arg0->step != 0) {
        arg0->step--;
    }
    arg0->timer--;
    if (arg0->timer == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceCountdownReadyPromptIn);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceCountdownReadyPrompt, arg0);
}

void updateRaceCountdownInitialDelay(RaceCountdownEffect *arg0) {
    if ((--arg0->timer) == 0) {
        enqueueSoundEffect(0x4B, 0x5A);
        arg0->step = 4;
        arg0->timer = 0x3C;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceCountdownReadyPromptHold);
    }
}

void initRaceCountdownPrompt(RaceCountdownEffect *arg0) {
    arg0->timer = 0x14;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceCountdownInitialDelay);
}

void renderRaceCourseModel(void *arg0) {
    volatile u8 pad[0x30];

    gDPPipeSync(gRegionAllocPtr++);

    gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0x8)));
    gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0x9)));

    gSPMatrix(gRegionAllocPtr++, &gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    switch (gRaceCourseIndex.unsignedValue) {
        case 0:
            gSPDisplayList(gRegionAllocPtr++, &_EBEC0_VRAM);
            break;
        case 1:
            gSPDisplayList(gRegionAllocPtr++, &_F57D0_VRAM);
            break;
        case 2:
            gSPDisplayList(gRegionAllocPtr++, &_FE788_VRAM);
            break;
        case 3:
            gSPDisplayList(gRegionAllocPtr++, &_10A1A0_VRAM);
            break;
        case 4:
            gSPDisplayList(gRegionAllocPtr++, &_1167F8_VRAM);
            break;
        case 5:
            gSPDisplayList(gRegionAllocPtr++, &_1246D0_VRAM);
            break;
        case 6:
            gSPDisplayList(gRegionAllocPtr++, &_131448_VRAM);
            break;
        case 7:
            gSPDisplayList(gRegionAllocPtr++, &_137EC0_VRAM);
            break;
        case 8:
            gSPDisplayList(gRegionAllocPtr++, &_13EC60_VRAM);
            break;
        case 9:
            gSPDisplayList(gRegionAllocPtr++, &_144B88_VRAM);
            break;
    }
}

void renderRaceCourseBackdrop(RaceCourseBackdropEffect *arg0) {
    s32 matrixFlags;
    void *textureBase;
    FixedTransform sp100;
    volatile u8 pad[8];

    sp100 = gIdentityFixedTransform;
    sp100.translation.x = -D_801121E0[gCurrentViewportIndex].transformOffset.x;
    sp100.translation.y = -D_801121E0[gCurrentViewportIndex].transformOffset.y;
    matrixFlags = G_MTX_NOPUSH;
    sp100.translation.z = -D_801121E0[gCurrentViewportIndex].transformOffset.z;

    arg0->matrix = allocFixedTransformMatrix(&sp100);
    if (arg0->matrix != NULL) {
        switch (gRaceCourseIndex.unsignedValue) {
            case 0:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02,
                           getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03,
                           getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->matrix,
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(gRegionAllocPtr++, &_EBFA0_VRAM);
                gSPDisplayList(gRegionAllocPtr++, &_EC310_VRAM);
                break;
            case 1:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02,
                           getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03,
                           textureBase = getRelocatableHeapBlockBase(
                               gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->matrix,
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(gRegionAllocPtr++, &_F5920_VRAM);
                break;
            case 2:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02,
                           getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03,
                           getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->matrix,
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(gRegionAllocPtr++, &_FE8F0_VRAM);
                break;
            case 3:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02,
                           getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03,
                           getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->matrix,
                          (G_MTX_NOPUSH | G_MTX_LOAD) | matrixFlags);
                gSPDisplayList(gRegionAllocPtr++, &_10A280_VRAM);
                break;
            case 4:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02,
                           getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03,
                           getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->matrix,
                          (matrixFlags | G_MTX_LOAD) | matrixFlags);
                gSPDisplayList(gRegionAllocPtr++, &_1169F0_VRAM);
                break;
            case 5:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02,
                           getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03,
                           getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->matrix,
                          (matrixFlags | G_MTX_LOAD) | matrixFlags);
                gSPDisplayList(gRegionAllocPtr++, &_1248A8_VRAM);
                gSPDisplayList(gRegionAllocPtr++, &_124E18_VRAM);
                break;
            case 6:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02,
                           getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03,
                           getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->matrix,
                          (matrixFlags | G_MTX_LOAD) | matrixFlags);
                gSPDisplayList(gRegionAllocPtr++, &_131558_VRAM);
                break;
            case 7:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02,
                           getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03,
                           getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->matrix,
                          (matrixFlags | G_MTX_LOAD) | matrixFlags);
                gSPDisplayList(gRegionAllocPtr++, &_137FD8_VRAM);
                break;
            case 8:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02,
                           getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03,
                           getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->matrix,
                          (matrixFlags | G_MTX_LOAD) | matrixFlags);
                gSPDisplayList(gRegionAllocPtr++, &_13ED30_VRAM);
                break;
            case 9:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02,
                           getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03,
                           getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->matrix,
                          (matrixFlags | G_MTX_LOAD) | matrixFlags);
                gSPDisplayList(gRegionAllocPtr++, &_144C58_VRAM);
                break;
        }
    }
}

void updateRaceCourseModelRenderTask(void *arg0) {
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderRaceCourseModel, arg0);
    addRenderCallback(&gBackdropRenderCallbackList, (RenderCallback)renderRaceCourseBackdrop, arg0);
}

void initRaceCourseModelRenderTask(void *arg0) {
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceCourseModelRenderTask);
}

void drawFinalLapPromptForViewport(RacePlayerEffect *arg0) {
    if (gCurrentViewportIndex == arg0->playerIndex) {
        drawAssetTableSprite(-0x30, -0xC, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)), 0x41);
    }
}

void updateFinalLapPrompt(void *arg0) {
    if (gMenuFlowState & 8) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawFinalLapPromptForViewport, arg0);
}

void initFinalLapPrompt(void *arg0) {
    enqueueSoundEffect(0x52, 0x5A);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateFinalLapPrompt);
}

extern void getAssetTableImagePaletteAndSize(u8 *, s32, void **, void **, s16 *, s16 *);
void renderCourseTextureMarkers(RaceCourseRenderEffect *arg0) {
    volatile u8 pad[8];
    void *image;
    void *palette;
    s16 width;
    s16 height;
    CourseMarkerSpawnEntry *entry;
    s16 textureIndex;
    s32 i;

    textureIndex = -1;
    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
    entry = gCourseTextureMarkerSpawnEntriesByCourse[gRaceCourseIndex.signedValue];
    i = 0;
    if (entry->type != -1) {
        do {
            if (isPositionNearCurrentRaceViewportCamera(&entry->pos) != 0) {
                if (entry->type != textureIndex) {
                    textureIndex = entry->type;
                    getAssetTableImagePaletteAndSize((u8 *)getRelocatableHeapBlockBase((s32)ASSET_HANDLE(0x1C)), textureIndex & 0xFFFF,
                                  &image, &palette, &width, &height);
                    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, width, height, 0,
                                            G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                                            G_TX_NOLOD, G_TX_NOLOD);
                    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
                }
                gSPMatrix(gRegionAllocPtr++, &arg0->vertices[i], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                {
                    Gfx *_g = gRegionAllocPtr++;
                    _g->words.w0 = 0x0400103F;
                    _g->words.w1 = (u32)&D_800D9C40[entry->type * 4];
                }
                RACE_COURSE_EFFECTS_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
            }
            entry++;
            i++;
        } while (-1 != entry->type);
    }
    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
}

void updateCourseTextureMarkers(void *arg0) {
    CourseMarkerSpawnEntry *entry;
    s8 type;

    entry = gCourseTextureMarkerSpawnEntriesByCourse[gRaceCourseIndex.signedValue];
    if (entry->type != -1) {
        do {
            switch (entry->type) {
                case 0:
                case 1:
                    pushRacePlayersOutOfCylinderAndApplyItemHit(&entry->pos, 0xC0000, 0x600000, 2);
                    break;
                case 2:
                    pushRacePlayersOutOfCylinderOrApplyItemHit(&entry->pos, 0x20000, 0x40000, 0x30000, 4);
                    break;
            }
            type = entry[1].type;
            entry++;
        } while (entry->type != -1);
    }

    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)renderCourseTextureMarkers, arg0);
}

void initCourseTextureMarkers(RaceCourseRenderEffect *arg0) {
    CourseMarkerSpawnEntry *entry;
    s32 count;
    s32 allocSize;
    s32 i;

    entry = gCourseTextureMarkerSpawnEntriesByCourse[gRaceCourseIndex.signedValue];
    count = 0;
    if (entry->type != -1) {
        do {
            count++;
            entry++;
        } while (entry->type != -1);
    }

    if (count != 0) {
        entry = gCourseTextureMarkerSpawnEntriesByCourse[gRaceCourseIndex.signedValue];
        allocSize = count * sizeof(Mtx);
        gAssetHandles[0x21] = allocRelocatableHeapBlock(allocSize);
        arg0->vertices = getRelocatableHeapBlockBase(gAssetHandles[0x21]);

        i = 0;
        if (count > 0) {
            do {
                arg0->vertices[i] = gIdentityMatrix;
                setPackedMatrixTranslation(&arg0->vertices[i], &entry->pos);
                i++;
                entry++;
            } while (i != count);
        }
        osWritebackDCache(arg0->vertices, allocSize);
    }

    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseTextureMarkers);
}

void renderRaceCourseSceneryObjects(RaceCourseRenderEffect *arg0) {
    CourseRenderEntry *var_s4;
    s32 var_s5;
    s32 var_s7;
    Gfx *temp_s0;
    Gfx *temp_s2;
    Gfx *temp_s3;

    var_s4 = gRaceCourseSceneryEntriesByCourse[gRaceCourseIndex.signedValue];
    var_s7 = TRUE;
    var_s5 = 0;
    if (var_s4->displayListIndex != -1) {
        do {
            if (isPositionNearCurrentRaceViewportCamera(&var_s4->position) != 0) {
                if (var_s7 != 0) {
                    gDPPipeSync(gRegionAllocPtr++);
                    temp_s2 = gRegionAllocPtr++;
                    var_s7 = FALSE;
                    gSPSegment(temp_s2, 0x02, getRelocatableHeapBlockBase(gAssetHandles[0xA]));

                    temp_s3 = gRegionAllocPtr++;
                    gSPSegment(temp_s3, 0x03, getRelocatableHeapBlockBase(gAssetHandles[0xB]));
                }

                temp_s0 = gRegionAllocPtr++;
                gDma1p(temp_s0, 1, (u32)arg0->vertices + (var_s5 << 6), 0x40, 2);

                temp_s0 = gRegionAllocPtr++;
                gSPDisplayList(temp_s0, gRaceCourseSceneryDisplayLists[var_s4->displayListIndex]);
            }
            var_s4++;
            var_s5++;
        } while (var_s4->displayListIndex != -1);
    }
}

void updateRaceCourseSceneryObjects(void *arg0) {
    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)renderRaceCourseSceneryObjects, arg0);
}

void initRaceCourseSceneryObjects(RaceCourseRenderEffect *arg0) {
    s32 size;
    CourseRenderEntry *base;
    CourseRenderEntry *entry;
    s32 i;
    FixedTransform transform;
    s32 count;

    base = gRaceCourseSceneryEntriesByCourse[gRaceCourseIndex.signedValue];
    count = 0;
    entry = base;
    if (base->displayListIndex != -1) {
        do {
            count++;
            entry++;
        } while (entry->displayListIndex != -1);
    }

    if (count != 0) {
        entry = base;
        size = count << 6;
        gAssetHandles[0x22] = allocRelocatableHeapBlock(size);
        arg0->vertices = (void *)getRelocatableHeapBlockBase(gAssetHandles[0x22]);

        for (i = 0; i < count; i++) {
            makeFixedRotationY(transform.rotation, entry->rotation);
            transform.translation.x = entry->position.x;
            transform.translation.y = entry->position.y;
            transform.translation.z = entry->position.z;
            packFixedTransformMatrix(&transform, (void *)((u32)arg0->vertices + (i << 6)));
            entry++;
        }

        osWritebackDCache(arg0->vertices, size);
    }
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceCourseSceneryObjects);
}

void renderPatrolCourseObject(PatrolCourseObjectEffect *arg0) {
    s32 sine;
    s32 doubleSine;
    FixedTransform transform;
    volatile s32 pad0[18];

    if (gRenderMatricesDirty != 0) {
        arg0->displayListValid = 0;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->displayListValid == 0) {
            arg0->displayListValid = 1;
            sine = fixedSine(arg0->unk40);
            if (1) {
                doubleSine = fixedSine((s16)(arg0->unk40 * 2));
                sine >>= 4;
                makeFixedRotationY(transform.rotation, arg0->angle + sine + 0x800);
                transform.translation.x = arg0->pos.x;
                transform.translation.y = (arg0->pos.y + (((doubleSine + 0x1000) << 2) << 2)) + 0xA4000;
            }
            transform.translation.z = arg0->pos.z;
            scaleFixedMatrix3sByQuarter(transform.rotation);
            arg0->displayList = allocFixedTransformMatrix(&transform);
        }

        if ((((&transform) && (&transform)) && (&transform)) & 0xFFFFu) {
        }

        if (arg0->displayList != NULL) {
            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
            gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
            gSPMatrix(gRegionAllocPtr++, arg0->displayList, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gRegionAllocPtr++, &_149610_VRAM);
        }
    }
}

// Matched by queueram via decomp.me scratch WguxK.
void updatePatrolCourseObject(PatrolCourseObjectEffect *arg0) {
    PatrolCourseObjectUpdateLocals local;
    s16 rand;
    s32 targetAngle;
    s32 var_v1;

    if (gRaceUpdatePaused == 0) {
        if (isPositionNearAnyRaceViewportFocus(&arg0->pos) != 0) {
            if (arg0->pad42 != 0) {
                targetAngle = calculateFixedAngleBetweenXZPoints(arg0->pos.x, arg0->pos.z, arg0->startPos.x, arg0->startPos.z);
            } else {
                targetAngle = calculateFixedAngleBetweenXZPoints(arg0->pos.x, arg0->pos.z, arg0->endX, arg0->endZ);
            }
            var_v1 = (((((targetAngle & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) - arg0->angle) & 0xFFF;
            targetAngle = -targetAngle;
            var_v1 = (s16)var_v1;
            if (var_v1 >= 0x801) {
                var_v1 = (s16)(var_v1 - 0x1000);
            }
            if (arg0->unk4C < var_v1) {
                var_v1 = (s16)(s32)arg0->unk4C;
            }
            if (var_v1 < -arg0->unk4C) {
                var_v1 = (s16)-arg0->unk4C;
            }
            arg0->angle = (s16)(arg0->angle + var_v1);
            arg0->pos.x += fixedSine(arg0->angle) * ((s32)-arg0->unk50 / 4096);
            local.dz = (arg0->pos.z += fixedCosine(arg0->angle) * ((s32)-arg0->unk50 / 4096));
            arg0->surfaceIndex =
                findRaceCourseSurfaceFromHint(arg0->surfaceIndex, arg0->pos.x, local.dz);
            arg0->pos.y =
                getRaceCourseSurfaceHeight(arg0->surfaceIndex, arg0->pos.x, arg0->pos.z);
            if (arg0->pad42 != 0) {
                targetAngle = arg0->pos.x - arg0->startPos.x;
                var_v1 = arg0->pos.z - arg0->startPos.z;
                if (targetAngle < 0) {
                    targetAngle = -targetAngle;
                }
                if (var_v1 < 0) {
                    var_v1 = -var_v1;
                }
                if (targetAngle < 0x30000) {
                    if (var_v1 < 0x30000) {
                        arg0->pad42 = 0;
                    }
                }
            } else {
                targetAngle = arg0->pos.x - arg0->endX;
                var_v1 = arg0->pos.z - arg0->endZ;
                if (targetAngle < 0) {
                    targetAngle = -targetAngle;
                }
                if (var_v1 < 0) {
                    var_v1 = -var_v1;
                }
                if ((targetAngle < 0x30000) && (var_v1 < 0x30000)) {
                    arg0->pad42 = 1;
                }
            }
            pushRacePlayersOutOfCylinderOrApplyItemHit(&arg0->pos, 0x40000, 0x50000, 0x30000, 4);
            arg0->unk40 += arg0->unk4E;
            if (arg0->unk40 == 0) {
                rand = randomNextSecondary();
                rand = rand & 3;
                if (rand == 1) {
                    arg0->unk50 = 0x10000;
                    arg0->unk4E = 0x80;
                    arg0->unk4C = 0x10;
                }
                if (rand == 2) {
                    arg0->unk50 = 0x20000;
                    arg0->unk4E = 0x100;
                    arg0->unk4C = 0x20;
                }
                if (rand == 3) {
                    arg0->unk50 = 0x40000;
                    arg0->unk4E = 0x200;
                    arg0->unk4C = 0x40;
                }
            }
        }
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderPatrolCourseObject, arg0);
}

void initPatrolCourseObject(PatrolCourseObjectEffect *arg0) {
    s16 angle;
    s16 temp4C;
    s32 temp50;
    s16 temp4E;

    if (gRaceUpdatePaused == 0) {
        angle = calculateFixedAngleBetweenXZPoints(arg0->startPos.x, arg0->startPos.z, arg0->endX, arg0->endZ);
        arg0->pos = arg0->startPos;
        temp4C = 0x10;
        temp50 = 0x10000;
        temp4E = 0x80;
        arg0->angle = angle;
        arg0->unk4C = temp4C;
        arg0->unk50 = temp50;
        arg0->unk4E = temp4E;
        arg0->pos.y = getRaceCourseSurfaceHeight(arg0->surfaceIndex, arg0->pos.x, arg0->pos.z);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updatePatrolCourseObject);
    }
}

void spawnPatrolCourseObject(s16 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    PatrolCourseObjectEffect *p = createCallbackTask((CallbackTaskCallback)initPatrolCourseObject, 0, 0x64);
    if (p != 0) {
        p->startPos.x = arg1;
        p->startPos.z = arg2;
        p->endX = arg3;
        p->endZ = arg4;
        p->surfaceIndex = arg0;
    }
}

void renderLaunchRampCourseObject(RaceMovingEffect *arg0) {
    volatile s32 unused;
    FixedTransform transform;
    volatile s32 pad[1];

    if (gRenderMatricesDirty != 0) {
        makeFixedRotationY(transform.rotation,
                           gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampCourseObjectAngle + 0x400);
        transform.translation.x = arg0->pos.x;
        transform.translation.y = arg0->pos.y;
        transform.translation.z = arg0->pos.z;
        arg0->matrix = allocFixedTransformMatrix(&transform);
    }

    if (arg0->matrix != NULL) {
        if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
            Gfx *_g;

            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
            gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
            gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            if (gRaceCourseIndex.signedValue != 8) {
                gSPDisplayList(gRegionAllocPtr++, &_148220_VRAM);
            } else {
                gSPDisplayList(gRegionAllocPtr++, &_14AB28_VRAM);
            }
        }
    }
}

void updateLaunchRampCourseObjectExit(RaceMovingEffect *arg0) {
    Vec3i sp24;
    s16 temp_v0;
    s32 temp_v1;
    RaceMovingEffect *temp_a3 = arg0;

    temp_v0 = arg0->timer;
    if (temp_v0 != 0) {
        if (gRaceUpdatePaused == 0) {
            temp_v1 = arg0->velocity.z;
            arg0->timer = temp_v0 - 1;
            if (temp_v1 >= (s32)0xFFF60001) {
                arg0->velocity.z = temp_v1 - 0x2000;
            }
            transformVec3iByFixedMatrix(arg0->unk30.rotation, &temp_a3->velocity, &sp24);
            temp_a3->pos.x += sp24.x;
            temp_a3->pos.y += sp24.y;
            temp_a3->pos.z += sp24.z;
        }
        addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderLaunchRampCourseObject, temp_a3);
        return;
    }
    removeCallbackTask(temp_a3);
}

void updateLaunchRampCourseObjectArc(RaceMovingEffect *arg0) {
    Vec3i sp2C;
    s16 *mtx;

    if (gRaceUpdatePaused == 0) {
        arg0->timer--;
        if (arg0->timer < 0x13) {
            arg0->velocity.z = 0xFFFC0000;
        }

        mtx = arg0->unk30.rotation;
        transformVec3iByFixedMatrix(mtx, &arg0->velocity, &sp2C);
        arg0->pos.x += sp2C.x;
        arg0->pos.y += sp2C.y;
        arg0->pos.z += sp2C.z;

        if (arg0->timer == 0) {
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateLaunchRampCourseObjectExit);
            makeFixedRotationXY(mtx, 0x100,
                                gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampCourseObjectAngle + 0x400);
            arg0->timer = 0x64;
        }
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderLaunchRampCourseObject, arg0);
}

void initLaunchRampCourseObject(RaceMovingEffect *arg0) {
    s16 *mtx;

    arg0->timer = 0x46;
    arg0->velocity.z = 0x680000;
    mtx = arg0->unk30.rotation;
    makeFixedRotationY(mtx,
                       gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampCourseObjectAngle + 0x400);
    transformVec3iByFixedMatrix(mtx, &arg0->velocity, &arg0->pos);
    arg0->velocity.z = 0xFFFE0000;
    arg0->pos.x += gRaceCourseStartEntries[COURSE_INDEX_RELOAD].pos.x;
    arg0->pos.y += gRaceCourseStartEntries[COURSE_INDEX_RELOAD].pos.y;
    arg0->pos.z += gRaceCourseStartEntries[COURSE_INDEX_RELOAD].pos.z;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateLaunchRampCourseObjectArc);
}

void renderSpiralCourseObject(RaceMovingEffect *arg0) {
    volatile s32 unused;
    FixedTransform transform;
    volatile s32 pad[2];

    if (gRenderMatricesDirty != 0) {
        makeFixedRotationY(transform.rotation, arg0->unk52);
        transform.translation.x = arg0->pos.x;
        transform.translation.y = arg0->pos.y;
        transform.translation.z = arg0->pos.z;
        arg0->matrix = allocFixedTransformMatrix(&transform);
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrix != NULL) {
            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
            gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
            gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            if (gRaceCourseIndex.signedValue != 8) {
                gSPDisplayList(gRegionAllocPtr++, &_148220_VRAM);
            } else {
                gSPDisplayList(gRegionAllocPtr++, &_14AB28_VRAM);
            }
        }
    }
}

void updateSpiralCourseObjectExit(RaceMovingEffect *arg0) {
    Vec3i sp24;
    s16 temp_v0;
    RaceMovingEffect *temp_a3 = arg0;

    temp_v0 = arg0->timer;
    if (temp_v0 != 0) {
        if (gRaceUpdatePaused == 0) {
            arg0->timer = temp_v0 - 1;
            transformVec3iByFixedMatrix(arg0->unk30.rotation, &temp_a3->velocity, &sp24);
            temp_a3->pos.x += sp24.x;
            temp_a3->pos.y += sp24.y;
            temp_a3->pos.z += sp24.z;
        }
        addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderSpiralCourseObject, temp_a3);
        return;
    }
    removeCallbackTask(temp_a3);
}

void updateSpiralCourseObjectTurn(RaceMovingEffect *arg0) {
    Vec3i sp2C;
    void *velocity;

    if (gRaceUpdatePaused == 0) {
        arg0->timer--;
        arg0->unk52 -= 0x2A;
        makeFixedRotationY(arg0->unk30.rotation, arg0->unk52);

        velocity = &arg0->velocity;
        transformVec3iByFixedMatrix(arg0->unk30.rotation, velocity, &sp2C);
        arg0->pos.x += sp2C.x;
        arg0->pos.y += sp2C.y;
        arg0->pos.z += sp2C.z;

        arg0->unk52 -= 0x2A;
        makeFixedRotationY(arg0->unk30.rotation, arg0->unk52);
        transformVec3iByFixedMatrix(arg0->unk30.rotation, velocity, &sp2C);
        arg0->pos.x += sp2C.x;
        arg0->pos.y += sp2C.y;
        arg0->pos.z += sp2C.z;

        if (arg0->timer == 0) {
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateSpiralCourseObjectExit);
            arg0->timer = 0x38;
        }
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderSpiralCourseObject, arg0);
}

void updateSpiralCourseObjectLaunch(RaceMovingEffect *arg0) {
    Vec3i sp1C;
    RaceMovingEffect *temp_a3 = arg0;

    if (gRaceUpdatePaused == 0) {
        arg0->timer--;
        transformVec3iByFixedMatrix(arg0->unk30.rotation, &temp_a3->velocity, &sp1C);
        temp_a3->pos.x += sp1C.x * 2;
        temp_a3->pos.y += sp1C.y * 2;
        temp_a3->pos.z += sp1C.z * 2;
        if (temp_a3->timer == 0) {
            setCallbackTaskCallback(temp_a3, (CallbackTaskCallback)updateSpiralCourseObjectTurn);
            temp_a3->timer = 0x18;
        }
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderSpiralCourseObject, temp_a3);
}

void initSpiralCourseObject(RaceMovingEffect *arg0) {
    s16 *mtx;

    arg0->timer = 0x28;
    arg0->unk52 = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].spiralCourseObjectAngle;
    arg0->velocity.x = -0x200000;
    arg0->velocity.z = 0x400000;
    mtx = arg0->unk30.rotation;
    makeFixedRotationY(mtx, arg0->unk52);
    transformVec3iByFixedMatrix(mtx, &arg0->velocity, &arg0->pos);
    arg0->velocity.x = 0;
    arg0->velocity.z = -0x20000;
    arg0->pos.x += gRaceCourseStartEntries[gRaceCourseIndex.signedValue].unk8.x;
    arg0->pos.y += gRaceCourseStartEntries[gRaceCourseIndex.signedValue].unk8.y + 0x40000;
    arg0->pos.z += gRaceCourseStartEntries[gRaceCourseIndex.signedValue].unk8.z;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateSpiralCourseObjectLaunch);
    updateSpiralCourseObjectLaunch(arg0);
}

void renderCourseGateObject(CourseGateObjectEffect *arg0) {
    FixedTransform scratch;
    volatile s32 pad[2];
    CourseGateObjectEffect *temp_s0 = arg0;
    Gfx *segment1;
    Gfx *segment2;

    if (gRenderMatricesDirty != 0) {
        temp_s0->sourceMatrix = NULL;
        temp_s0->pos1Matrix = NULL;
        temp_s0->pos2Matrix = NULL;
    }

    if (isPositionNearCurrentRaceViewportCamera((Vec3i *) &gCourseGateSoundParams[gRaceCourseIndex.signedValue]) == 0) {
        return;
    }

    if (temp_s0->sourceMatrix == NULL) {
        temp_s0->sourceMatrix = allocFixedTransformMatrix(&temp_s0->source);
    }

    if (temp_s0->sourceMatrix != NULL) {
        gDPPipeSync(gRegionAllocPtr++);
        segment1 = gRegionAllocPtr++;
        segment1->words.w0 = 0xBC000806;
        segment1->words.w1 = (u32)getRelocatableHeapBlockBase(ASSET_HANDLE(0xA));
        segment2 = gRegionAllocPtr++;
        segment2->words.w0 = 0xBC000C06;
        segment2->words.w1 = (u32)getRelocatableHeapBlockBase(ASSET_HANDLE(0xB));
        gSPMatrix(gRegionAllocPtr++, temp_s0->sourceMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, &_148F88_VRAM);
    }

    if (temp_s0->pos1Matrix == NULL) {
        makeFixedRotationZY(scratch.rotation, gCourseGateSoundParams[gRaceCourseIndex.signedValue].angle, temp_s0->unk50);
        scratch.translation.x = temp_s0->pos1.x;
        scratch.translation.y = temp_s0->pos1.y;
        scratch.translation.z = temp_s0->pos1.z;
        temp_s0->pos1Matrix = allocFixedTransformMatrix(&scratch);
    }

    if (temp_s0->pos1Matrix != NULL) {
        gSPMatrix(gRegionAllocPtr++, temp_s0->pos1Matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, &_149040_VRAM);
    }

    if (temp_s0->pos2Matrix == NULL) {
        scratch = temp_s0->source;
        scratch.translation.x = temp_s0->pos2.x;
        scratch.translation.y = temp_s0->pos2.y;
        scratch.translation.z = temp_s0->pos2.z;
        temp_s0->pos2Matrix = allocFixedTransformMatrix(&scratch);
    }

    if (temp_s0->pos2Matrix != NULL) {
        gSPMatrix(gRegionAllocPtr++, temp_s0->pos2Matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        if (temp_s0->unk56 == 0) {
            gSPDisplayList(gRegionAllocPtr++, &_149120_VRAM);
        } else {
            gSPDisplayList(gRegionAllocPtr++, &_1491F8_VRAM);
        }
    }
}

void updateCourseGateClosing(CourseGateObjectEffect *arg0) {
    if (gRaceUpdatePaused == 0) {
        if (arg0->unk50 != 0) {
            arg0->unk50 += 0x80;
        } else {
            arg0->unk56 = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitForCourseGateTrigger);
        }
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderCourseGateObject, arg0);
}

void updateCourseGateOpening(CourseGateObjectEffect *arg0) {
    CourseGateObjectEffect *temp_s0 = arg0;
    s16 temp_v0;

    if (gRaceUpdatePaused == 0) {
        temp_v0 = arg0->unk50;
        if (temp_v0 != -0x400) {
            arg0->unk50 = temp_v0 - 0x40;
        }
        temp_s0->unk54--;
        if (temp_s0->unk54 == 0) {
            enqueuePositionalSoundEffect(0x1C, &gCourseGateSoundParams[gRaceCourseIndex.signedValue], 0x7F, 0x32);
            setCallbackTaskCallback(temp_s0, (CallbackTaskCallback)updateCourseGateClosing);
        }
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderCourseGateObject, temp_s0);
}

void waitForCourseGateTrigger(CourseGateObjectEffect *arg0) {
    if ((gRaceUpdatePaused == 0) && (gMenuFlowState & 4)) {
        arg0->unk54 = 0x2D;
        gMenuFlowState &= ~4;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseGateOpening);
        arg0->unk56 = 1;
        enqueuePositionalSoundEffect(0x16, &gCourseGateSoundParams[gRaceCourseIndex.signedValue], 0x7F, 0x32);
        enqueuePositionalSoundEffect(0x1B, &gCourseGateSoundParams[gRaceCourseIndex.signedValue], 0x7F, 0x32);
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderCourseGateObject, arg0);
}

void initCourseGateObject(CourseGateObjectEffect *arg0) {
    s16 *mtx;
    Vec3i sp28;

    mtx = arg0->source.rotation;
    makeFixedRotationY(mtx, gCourseGateSoundParams[gRaceCourseIndex.signedValue].angle);
    arg0->source.translation.x = gCourseGateSoundParams[gRaceCourseIndex.signedValue].x;
    arg0->source.translation.y = gCourseGateSoundParams[gRaceCourseIndex.signedValue].y;
    arg0->source.translation.z = gCourseGateSoundParams[gRaceCourseIndex.signedValue].z;
    sp28.x = 0x18000;
    sp28.y = 0x120000;
    sp28.z = -0x80000;
    transformVec3iByFixedMatrix(mtx, &sp28, &arg0->pos1);
    arg0->pos1.x += arg0->source.translation.x;
    arg0->pos1.y += arg0->source.translation.y;
    arg0->pos1.z += arg0->source.translation.z;
    sp28.x = 0;
    sp28.y = 0x120000;
    sp28.z = 0x50000;
    transformVec3iByFixedMatrix(mtx, &sp28, &arg0->pos2);
    arg0->pos2.x += arg0->source.translation.x;
    arg0->pos2.y += arg0->source.translation.y;
    arg0->pos2.z += arg0->source.translation.z;
    arg0->unk52 = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitForCourseGateTrigger);
}

void renderCourseBillboardMarker(RaceCourseMarkerEffect *arg0) {
    Gfx *segmentGfx;
    Gfx *gfx;
    s32 i;
    s16 vertexCount;
    Gfx *newGfx;
    u8 pad[0x10];

    if (gRenderMatricesDirty != 0) {
        arg0->vertices = allocMenuRenderScratch((arg0->vertexCount * sizeof(Vtx)) + ((u32) pad & 0));
        if (arg0->vertices != NULL) {
            i = 0;
            if (arg0->vertexCount > 0) {
                do {
                    arg0->vertices[i] = arg0->baseVertices[i];
                    arg0->vertices[i].v.tc[1] += arg0->rotation;
                    arg0->vertices[i].v.tc[1] &= 0xFFFF;
                    i++;
                } while (i < arg0->vertexCount);
            }
        }
    }

    if (arg0->vertices != NULL) {
        gDPPipeSync(gRegionAllocPtr++);
        newGfx = gRegionAllocPtr++;
        segmentGfx = newGfx;
        segmentGfx->words.w0 = 0xBC000806;
        segmentGfx->words.w1 = (u32) getRelocatableHeapBlockBase(ASSET_HANDLE(0x8));
        gSPMatrix(gRegionAllocPtr++, &gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, arg0->texturePtr);
        gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->texture, G_IM_FMT_CI, 0x20, 0x40, 0, G_TX_WRAP,
                               G_TX_WRAP, 5, 6, 0, 0);
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette);
        gfx = gRegionAllocPtr++;
        vertexCount = arg0->vertexCount;
        gfx->words.w0 = (((vertexCount << 0xA) | ((vertexCount << 4) - 1)) & 0xFFFF) | 0x04000000;
        gfx->words.w1 = (u32) arg0->vertices;
        gSPDisplayList(gRegionAllocPtr++, arg0->palettePtr);
    }
}

void updateCourseBillboardMarker(RaceCourseMarkerEffect *arg0) {
    arg0->rotation -= 0x40;
    arg0->rotation &= 0x7FF;
    if (arg0->useAltQueue != 0) {
        addRenderCallback(&D_801248EC, (RenderCallback)renderCourseBillboardMarker, arg0);
    } else {
        addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderCourseBillboardMarker, arg0);
    }
}

void initCourseBillboardMarker(RaceCourseMarkerEffect *arg0) {
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)),
                  gCourseBillboardMarkerEntries[arg0->entryIndex].textureIndex,
                  &arg0->texture, &arg0->palette);
    arg0->baseVertices =
        (Vtx *) resolveAssetTableRelativePointer(getRelocatableHeapBlockBase(ASSET_HANDLE(0x8)), (s32) gCourseBillboardMarkerEntries[arg0->entryIndex].baseVerticesInput);

    {
        CourseMarkerEntry *entry = &gCourseBillboardMarkerEntries[arg0->entryIndex];

        arg0->vertexCount = entry->vertexCount;
        arg0->texturePtr = entry->texturePtr;
        arg0->palettePtr = entry->palettePtr;
        arg0->vertexCount = entry->vertexCount;
        arg0->useAltQueue = entry->flags & 1;
        arg0->unk3C = entry->flags & 2;
    }
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseBillboardMarker);
}

void renderCourseTriggerVolume(RaceCourseTriggerEffect *arg0) {
    volatile s32 unused;
    FixedTransform transform;
    Gfx *gfx;

    if (gRenderMatricesDirty != 0) {
        CourseTriggerEntry *entry = &gCourseTriggerEntries[((volatile RaceCourseTriggerEffect *) arg0)->entryIndex];

        makeFixedRotationXY(transform.rotation, entry->pitch, entry->yaw);
        transform.translation.x = gCourseTriggerEntries[arg0->entryIndex].pos.x;
        transform.translation.y = gCourseTriggerEntries[arg0->entryIndex].pos.y;
        transform.translation.z = gCourseTriggerEntries[arg0->entryIndex].pos.z;
        arg0->matrix = allocFixedTransformMatrix(&transform);
    }

    if (arg0->matrix != NULL) {
        {
            Gfx *_g = gRegionAllocPtr++;

            _g->words.w0 = 0xE7000000;
            _g->words.w1 = 0;
        }
        {
            Gfx *_g = gRegionAllocPtr++;

            _g->words.w0 = 0xBC000806;
            _g->words.w1 = (u32)getRelocatableHeapBlockBase(ASSET_HANDLE(0x8));
        }
        {
            Gfx *_g = gRegionAllocPtr++;
            volatile s32 pad[2];

            _g->words.w0 = 0xBC000C06;
            _g->words.w1 = (u32)getRelocatableHeapBlockBase(ASSET_HANDLE(0x9));
        }
        {
            Gfx *_g = gRegionAllocPtr++;

            _g->words.w0 = 0x01020040;
            _g->words.w1 = (u32) arg0->matrix;
        }
        {
            Gfx *_g = gRegionAllocPtr++;

            _g->words.w0 = 0x06000000;
            _g->words.w1 = (u32) arg0->displayList;
        }
    }
}

// collidePlayerWithCourseTriggerVolume best match: 98.574% at nonmatchings/collidePlayerWithCourseTriggerVolume-7050948565576131586/base_30.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race/course/race_course_effects/collidePlayerWithCourseTriggerVolume.s")

#ifdef NON_MATCHING
void collidePlayerWithCourseTriggerVolume(RacePlayer *arg0, RaceCourseTriggerEffect *arg1) {
    RaceCourseTriggerEffect *trigger;
    RacePlayer *player;
    Vec3i delta;
    Vec3i transformed;
    FixedMatrix3sScratch matrix;
    s64 savedPush;
    s32 zero;
    CourseTriggerEntry *entry;
    s32 push;
    s32 scaleX;
    u16 entryIndex;
    s32 collisionRadius;
    s32 limit;
    s32 positiveLimit;
    s32 negativeLimit;

    trigger = arg1;
    player = arg0;

    if ((gRaceUpdatePaused == 0) && (player->isActive != 0)) {
        makeFixedRotationYX(matrix, -gCourseTriggerEntries[trigger->entryIndex].pitch,
                            -gCourseTriggerEntries[trigger->entryIndex].yaw);

        if ((player->stateFlags & 0x2000) == 0) {
            delta.x = player->pos.x - gCourseTriggerEntries[trigger->entryIndex].pos.x;
            delta.y = player->unk5C - gCourseTriggerEntries[trigger->entryIndex].pos.y;
            delta.z = player->pos.z - gCourseTriggerEntries[trigger->entryIndex].pos.z;
            transformVec3iByFixedMatrix(matrix, &delta, &transformed);

            if ((transformed.z >= -trigger->scaleZ) && (trigger->scaleZ >= transformed.z) &&
                (transformed.x >= (-trigger->scaleX - 0x30000)) &&
                ((trigger->scaleX + 0x30000) >= transformed.x) && (transformed.y >= -0xFFFFF) &&
                (trigger->scaleY >= transformed.y)) {
                delta.y = trigger->scaleY - transformed.y;
                delta.x = -transformed.x;
                delta.z = 0;

                entry = &gCourseTriggerEntries[trigger->entryIndex];
                makeFixedRotationXY(matrix, entry->pitch, entry->yaw);
                transformVec3iByFixedMatrix(matrix, &delta, &transformed);

                player->pos.x += transformed.x;
                player->unk5C += transformed.y;
                player->pos.z += transformed.z;
                player->stateFlags |= 0x02000000;
                player->unk332 = gCourseTriggerEntries[trigger->entryIndex].yaw ^ 0;
                player->unk334 = gCourseTriggerEntries[trigger->entryIndex].pitch;
                return;
            }
        }

        if (player->unk578 == 0) {
            pushRacePlayerOutOfCylinder(&trigger->pos1, trigger->scaleX + 0x30000, 0x120000, player->playerIndex);
            pushRacePlayerOutOfCylinder(&trigger->pos2, trigger->scaleX + 0x30000, 0x100000, player->playerIndex);
        }

        delta.x = player->pos.x - gCourseTriggerEntries[trigger->entryIndex].pos.x;
        delta.y = player->unk5C - gCourseTriggerEntries[trigger->entryIndex].pos.y;
        delta.z = player->pos.z - gCourseTriggerEntries[trigger->entryIndex].pos.z;
        transformVec3iByFixedMatrix(matrix, &delta, &transformed);

        if (transformed.y <= 0) {
            if (transformed.y >= -0x160000) {
                if (transformed.z >= -trigger->scaleZ) {
                    if (1) {
                    }
                    if (1) {
                    }
                    if (1) {
                    }
                    if (1) {
                    }
                    if (trigger->scaleZ >= transformed.z) {
                        scaleX = trigger->scaleX;
                        collisionRadius = player->collisionRadius;
                        limit = scaleX + collisionRadius;
                        if (transformed.x >= ((-scaleX - collisionRadius) - 0x30000)) {
                            positiveLimit = limit + 0x30000;
                            if (trigger && trigger) {
                            }
                            if (positiveLimit >= transformed.x) {
                                push = 0;
                                savedPush = -limit;
                                negativeLimit = savedPush;
                                if (((gRaceUpdatePaused & 0xFFFF) && gRaceUpdatePaused) && gRaceUpdatePaused) {
                                }
                                if (transformed.x >= 0) {
                                    if (transformed.x < positiveLimit) {
                                        push = (limit - transformed.x) + 0x30000;
                                    }
                                } else {
                                    entry = &gCourseTriggerEntries[trigger->entryIndex];
                                    if (1) {
                                        if ((negativeLimit - 0x30000) < transformed.x) {
                                            push = (negativeLimit - transformed.x) - 0x30000;
                                        }
                                    }
                                }

                                if (push != 0) {
                                    entryIndex = trigger->entryIndex;
                                    makeFixedRotationY(matrix, gCourseTriggerEntries[entryIndex].yaw);
                                    zero = transformed.x * 0;
                                    delta.y = zero;
                                    delta.z = 0;
                                    delta.x = push;
                                    transformVec3iByFixedMatrix(matrix, &delta, &transformed);
                                    player->pos.x += transformed.x;
                                    player->pos.z += transformed.z;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
#endif

void updateCourseTriggerVolume(RaceCourseTriggerEffect *arg0) {
    if (gRacePlayers[0].isActive != 0) {
        collidePlayerWithCourseTriggerVolume(&gRacePlayers[0], arg0);
    }
    if (gRacePlayers[1].isActive != 0) {
        collidePlayerWithCourseTriggerVolume(&gRacePlayers[1], arg0);
    }
    if (gRacePlayers[2].isActive != 0) {
        collidePlayerWithCourseTriggerVolume(&gRacePlayers[2], arg0);
    }
    if (gRacePlayers[3].isActive != 0) {
        collidePlayerWithCourseTriggerVolume(&gRacePlayers[3], arg0);
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderCourseTriggerVolume, arg0);
}

void initCourseTriggerVolume(RaceCourseTriggerEffect *arg0) {
    CourseTriggerScratch scratch;
    CourseTriggerEntry *entry;

    entry = &gCourseTriggerEntries[arg0->entryIndex];
    arg0->pitch = entry->pitch;
    arg0->yaw = entry->yaw;
    arg0->scaleX = entry->scaleX << 0x10;
    arg0->scaleY = entry->scaleY << 0x10;
    arg0->scaleZ = entry->scaleZ << 0x10;
    arg0->displayList = entry->displayList;

    makeFixedRotationXY(scratch.transform.rotation, arg0->pitch, arg0->yaw);

    scratch.source.y = 0;
    scratch.source.x = 0;
    scratch.source.z = arg0->scaleZ;
    transformVec3iByFixedMatrix(scratch.transform.rotation, &scratch.source, &scratch.dest);

    entry = &gCourseTriggerEntries[arg0->entryIndex];
    arg0->pos1.x = entry->pos.x + scratch.dest.x;
    arg0->pos1.y = entry->pos.y + scratch.dest.y - 0x100000;
    arg0->pos1.z = entry->pos.z + scratch.dest.z;

    scratch.source.x = 0;
    scratch.source.y = 0;
    scratch.source.z = -arg0->scaleZ;
    transformVec3iByFixedMatrix(scratch.transform.rotation, &scratch.source, &scratch.dest);

    entry = &gCourseTriggerEntries[arg0->entryIndex];
    arg0->pos2.x = entry->pos.x + scratch.dest.x;
    arg0->pos2.y = entry->pos.y + scratch.dest.y - 0x100000;
    arg0->pos2.z = entry->pos.z + scratch.dest.z;

    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseTriggerVolume);
}
