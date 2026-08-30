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
#include "game/race/camera/race_camera.h"
#include "game/audio/sound_manager.h"
#include "PR/os_cache.h"

#define COURSE_INDEX_RELOAD (*(volatile s16 *)&gRaceCourseIndex.signedValue)
#define ASSET_HANDLE(index) (gAssetHandles[(index)])
#define RACE_COURSE_EFFECTS_GFX_CMD(pkt, cmd0, cmd1) \
    {                                                \
        Gfx *_g = (Gfx *)(pkt);                      \
        _g->words.w0 = (cmd0);                       \
        _g->words.w1 = (cmd1);                       \
    }

typedef struct {
    s32 dz;
    Vec3i *volatile unusedPosition;
} PatrolCourseObjectUpdateLocals;

typedef struct {
    Vec3i dest;
    Vec3i source;
    Transform3D transform;
    s32 padding38;
} CourseTriggerScratch;

Vtx D_800D9C40[] = {
    { { { -36, 144, 0 }, 0x0000, { -16, -16 }, { 228, 228, 228, 255 } } },
    { { { 36, 144, 0 }, 0x0000, { 1008, -16 }, { 228, 228, 228, 255 } } },
    { { { 36, -8, 0 }, 0x0000, { 1008, 2032 }, { 228, 228, 228, 255 } } },
    { { { -36, -8, 0 }, 0x0000, { -16, 2032 }, { 228, 228, 228, 255 } } },
    { { { -36, 144, 0 }, 0x0000, { -16, -16 }, { 228, 228, 228, 255 } } },
    { { { 36, 144, 0 }, 0x0000, { 1008, -16 }, { 228, 228, 228, 255 } } },
    { { { 36, -8, 0 }, 0x0000, { 1008, 2032 }, { 228, 228, 228, 255 } } },
    { { { -36, -8, 0 }, 0x0000, { -16, 2032 }, { 228, 228, 228, 255 } } },
    { { { -4, 8, 0 }, 0x0000, { -16, -16 }, { 228, 228, 228, 255 } } },
    { { { 4, 8, 0 }, 0x0000, { 496, -16 }, { 228, 228, 228, 255 } } },
    { { { 4, 0, 0 }, 0x0000, { 496, 496 }, { 228, 228, 228, 255 } } },
    { { { -4, 0, 0 }, 0x0000, { -16, 496 }, { 228, 228, 228, 255 } } },
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

RaceCourseTextureMarkerEntry D_800D9D68[] = {
    { 0,  { 0 }, { 0xDA6797E0, 0xE8120000, 0xE8618D80 } },
    { 0,  { 0 }, { 0xDA5596C0, 0xE8050000, 0xE8E23CA8 } },
    { 0,  { 0 }, { 0xD95E35E8, 0xE7DF8000, 0xE8D1E9E8 } },
    { 0,  { 0 }, { 0xD953F118, 0xE7CF0000, 0xE98828D8 } },
    { 0,  { 0 }, { 0xD87ED180, 0xE7B28000, 0xE96589B8 } },
    { 0,  { 0 }, { 0xD84BAD48, 0xE7980000, 0xEA4D1788 } },
    { 0,  { 0 }, { 0xD73764B0, 0xE7708000, 0xEA46A970 } },
    { 0,  { 0 }, { 0xD6D24718, 0xE7460000, 0xEBBB5D50 } },
    { 0,  { 0 }, { 0xD57CF2F0, 0xE7050000, 0xEC279070 } },
    { 0,  { 0 }, { 0xD577DA48, 0xE6CF8000, 0xED718F20 } },
    { 0,  { 0 }, { 0xD4B5F828, 0xE6A18000, 0xEDB78D18 } },
    { 0,  { 0 }, { 0xD4DDCE30, 0xE66A0000, 0xEEB211B0 } },
    { 0,  { 0 }, { 0xD3CA7C40, 0xE61A0000, 0xEF1E5DB8 } },
    { 0,  { 0 }, { 0xD481C800, 0xE5BC0000, 0xF0859FC0 } },
    { 0,  { 0 }, { 0xD3CC9528, 0xE5828000, 0xF0E0F508 } },
    { 0,  { 0 }, { 0xD409A090, 0xE52E8000, 0xF2195078 } },
    { 0,  { 0 }, { 0xD3866C60, 0xE4FA0000, 0xF2BE3BE0 } },
    { 0,  { 0 }, { 0xD96B6500, 0xE7FB8000, 0xE7D9E7E0 } },
    { 0,  { 0 }, { 0xD859CD08, 0xE7D48000, 0xE7F9B050 } },
    { 0,  { 0 }, { 0xD7C2DDF0, 0xE7A58000, 0xE8FA3A88 } },
    { 0,  { 0 }, { 0xD6E1B258, 0xE7858000, 0xE92060E8 } },
    { 0,  { 0 }, { 0xD6A9CBB0, 0xE7668000, 0xEA017B70 } },
    { 0,  { 0 }, { 0xD5C498C0, 0xE7490000, 0xEA290DE0 } },
    { 0,  { 0 }, { 0xD56CE800, 0xE7220000, 0xEB391100 } },
    { 0,  { 0 }, { 0xD4870878, 0xE6F98000, 0xEBB56870 } },
    { 0,  { 0 }, { 0xD450CCE8, 0xE6C28000, 0xECD082E8 } },
    { 0,  { 0 }, { 0xD3807520, 0xE6908000, 0xED524948 } },
    { 0,  { 0 }, { 0xD3A568E8, 0xE6588000, 0xEE3907D8 } },
    { 0,  { 0 }, { 0xD2F6A828, 0xE6240000, 0xEEB42968 } },
    { 0,  { 0 }, { 0xD2EBB590, 0xE5C00000, 0xEFE3BC10 } },
    { 0,  { 0 }, { 0xD28E09F0, 0xE5400000, 0xF170DE80 } },
    { 0,  { 0 }, { 0xCD7E5688, 0xE3048000, 0xF5BE8888 } },
    { 0,  { 0 }, { 0xB041CA10, 0xC4418000, 0xDE974048 } },
    { 0,  { 0 }, { 0xAFE7E4D8, 0xB75D0000, 0xC9182EF0 } },
    { 0,  { 0 }, { 0xB00F6BB8, 0xB7400000, 0xC8C59428 } },
    { 0,  { 0 }, { 0xB034A4F0, 0xB71E8000, 0xC86E5ED8 } },
    { 0,  { 0 }, { 0xB0DF41B8, 0xB6A90000, 0xC7B85530 } },
    { 0,  { 0 }, { 0xB21C3F10, 0xB5E58000, 0xC72DAF10 } },
    { 0,  { 0 }, { 0xB3B42628, 0xB4EA0000, 0xC751B0F0 } },
    { 0,  { 0 }, { 0xB401A730, 0xB4CD0000, 0xC753E9F8 } },
    { 1,  { 0 }, { 0xCD361418, 0xE2EE8000, 0xF57850C0 } },
    { 1,  { 0 }, { 0xCCEE6A70, 0xE2B78000, 0xF4B062D0 } },
    { 1,  { 0 }, { 0xB7162340, 0xCC288000, 0xEC76E730 } },
    { 1,  { 0 }, { 0xB5EFCAD0, 0xCBF68000, 0xEC39E9A0 } },
    { 1,  { 0 }, { 0xB56C12F8, 0xCBE08000, 0xEC0BDFB0 } },
    { 1,  { 0 }, { 0xB0C36918, 0xC4C08000, 0xDF47EDD8 } },
    { -1, { 0 }, { 0x00000000, 0x00000000, 0x00000000 } },
};

RaceCourseTextureMarkerEntry D_800DA058[] = {
    { 2,  { 0 }, { 0x28C8B179, 0xD113BFA6, 0xC9F84D48 } },
    { 2,  { 0 }, { 0x29477435, 0xD084A801, 0xCC351004 } },
    { 2,  { 0 }, { 0x2A2C03A7, 0xD0016D55, 0xCE225B3E } },
    { 2,  { 0 }, { 0x2AC2EC87, 0xCFF28C55, 0xCDE30266 } },
    { 2,  { 0 }, { 0x2CBF0511, 0xCF49D858, 0xCEDEA806 } },
    { -1, { 0 }, { 0x00000000, 0x00000000, 0x00000000 } },
};

RaceCourseTextureMarkerEntry *gCourseTextureMarkerSpawnEntriesByCourse[] = {
    D_800D9D68,
    D_800DA058,
};

Vtx gRaceCourseSceneryFogPanelVertices[] = {
    { { { -40, 80, -136 }, 0x0000, { 0, 0 }, { 30, 25, 35, 255 } } },
    { { { -42, 82, -136 }, 0x0000, { 0, 0 }, { 30, 25, 35, 255 } } },
    { { { -42, 742, -1736 }, 0x0000, { 0, 0 }, { 0, 0, 0, 255 } } },
    { { { -40, 740, -1736 }, 0x0000, { 0, 0 }, { 0, 0, 0, 255 } } },
    { { { 40, 80, -136 }, 0x0000, { 0, 0 }, { 30, 25, 35, 255 } } },
    { { { 42, 82, -136 }, 0x0000, { 0, 0 }, { 30, 25, 35, 255 } } },
    { { { 42, 742, -1736 }, 0x0000, { 0, 0 }, { 0, 0, 0, 255 } } },
    { { { 40, 740, -1736 }, 0x0000, { 0, 0 }, { 0, 0, 0, 255 } } },
};

Gfx gRaceCourseSceneryFogPanelDisplayList[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsSPSetGeometryMode(G_FOG),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPVertex(gRaceCourseSceneryFogPanelVertices, 8, 0),
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

Gfx *gRaceCourseObjectDisplayLists[] = {
    (void *)0x02000000, (void *)0x02000160, (void *)0x02000000, (void *)0x020002C0,
    (void *)0x02000410, (void *)0x020028F0, (void *)0x02002DB8, (void *)0x02000000,
    (void *)0x02000568, (void *)0x02000910, (void *)0x02000B30, (void *)0x02001350,
    (void *)0x02001548, (void *)0x02001678, (void *)0x02001730, (void *)0x02001810, (void *)0x020018E8,
    (void *)0x020019C0, (void *)0x02001D00, gRaceCourseSceneryFogPanelDisplayList, (void *)0x020020A8,
    (void *)0x02002208, (void *)0x020023A8, (void *)0x02002490, (void *)0x02002660, (void *)0x02002ED0,
};

RaceCourseSceneryEntry gBigSnowmanCourseSceneryEntries[] = {
    { 0,  0, { 0x01D014A8, 0xFF6C4D78, 0xFB9EA3E0 }, 608,  0 },
    { 0,  0, { 0x0240BB14, 0xFF6C4D78, 0xFA935474 }, 608,  0 },
    { 1,  0, { 0xD23466CE, 0xE42B18E0, 0xF5EA0624 }, 2304, 0 },
    { 4,  0, { 0xCFB74EEE, 0xDE1498E0, 0xEAC21300 }, 2304, 0 },
    { 8,  0, { 0xF97016E4, 0x00DBFC9A, 0xFD0C374C }, 3632, 0 },
    { 8,  0, { 0xC2E81822, 0xAC908000, 0xD1AD9DFC }, 2608, 0 },
    { 19, 0, { 0xC2E81822, 0xAC908000, 0xD1AD9DFC }, 2608, 0 },
    { 17, 0, { 0xCFAB11F8, 0xE3673C32, 0xF4590B3E }, 1216, 0 },
    { 17, 0, { 0xC14A2B73, 0xACD9B80F, 0xCC23BA7C }, 2992, 0 },
    { 25, 0, { 0xFFEC52BC, 0x0005AEED, 0x00953930 }, 2608, 0 },
    { 25, 0, { 0xC48EF729, 0xAC960000, 0xD0CC2D2F }, 5152, 0 },
    { 25, 0, { 0xC4D7BE2D, 0xAC960000, 0xD06F5859 }, 5152, 0 },
    { 10, 0, { 0xC46056B3, 0xAC960000, 0xCE3C5977 }, 3664, 0 },
    { -1, 0, { 0x00000000, 0x00000000, 0x00000000 }, 0,    0 },
};

RaceCourseSceneryEntry gSunsetRockCourseSceneryEntries[] = {
    { 8,  0, { 0xF88F8010, 0x01B3C704, 0x08B7C216 }, 3952, 0 },
    { 8,  0, { 0x25AB1286, 0xB99EAFDE, 0xA418BB3C }, 736,  0 },
    { 19, 0, { 0x25AB1286, 0xB99EAFDE, 0xA418BB3C }, 736,  0 },
    { 10, 0, { 0xFC683074, 0x00117426, 0xFCE0F666 }, 672,  0 },
    { 11, 0, { 0xF271CB3A, 0xF24BC074, 0xE9E7C75E }, 1392, 0 },
    { 18, 0, { 0xF5186325, 0xF35DB06F, 0xEC2A2C50 }, 1424, 0 },
    { 18, 0, { 0xF3CB2861, 0xF332306F, 0xEB1AD400 }, 1424, 0 },
    { -1, 0, { 0x00000000, 0x00000000, 0x00000000 }, 0,    0 },
};

RaceCourseSceneryEntry gNightHighwayCourseSceneryEntries[] = {
    { 8,  0, { 0xFB3C1234, 0x0166050E, 0x02974782 }, 0,    0 },
    { 8,  0, { 0x1678D7C8, 0xD0575952, 0xC957E20F }, 2192, 0 },
    { 19, 0, { 0x1678D7C8, 0xD0575952, 0xC957E20F }, 2192, 0 },
    { -1, 0, { 0x00000000, 0x00000000, 0x00000000 }, 0,    0 },
};

RaceCourseSceneryEntry gGrassValleyCourseSceneryEntries[] = {
    { 8,  0, { 0x053E07E6, 0x00874833, 0xFA36E885 }, 1808, 0 },
    { 8,  0, { 0xB3F31DAE, 0xD0500000, 0x9B0A372B }, 2000, 0 },
    { 19, 0, { 0xB3F31DAE, 0xD0500000, 0x9B0A372B }, 2000, 0 },
    { -1, 0, { 0x00000000, 0x00000000, 0x00000000 }, 0,    0 },
};

RaceCourseSceneryEntry gDizzyLandCourseSceneryEntries[] = {
    { 11, 0, { 0x05A47A7A, 0xD7FCE5DB, 0xAFF3326D }, 3712, 0 },
    { 11, 0, { 0x040EFAF2, 0xD8D2DC67, 0xB3E951AB }, 1120, 0 },
    { 11, 0, { 0x095DF1CC, 0xD9C54381, 0xB2FF5E19 }, 4080, 0 },
    { 8,  0, { 0x01A91FD6, 0xD2600000, 0xA317F7BF }, 1536, 0 },
    { 19, 0, { 0x01A91FD6, 0xD2600000, 0xA317F7BF }, 1536, 0 },
    { 8,  0, { 0xFB2519B1, 0x018E6620, 0x03090886 }, 4064, 0 },
    { 18, 0, { 0x0452B064, 0xF5C87EA5, 0xE0367FE4 }, 832,  0 },
    { 18, 0, { 0x03D5887A, 0xF66BDEA5, 0xE1592132 }, 832,  0 },
    { 18, 0, { 0x059C3BB0, 0xF65E9EA5, 0xE1E2E754 }, 2912, 0 },
    { 18, 0, { 0x05ECAF34, 0xF5C69EA5, 0xE0AD1F0E }, 2912, 0 },
    { 18, 0, { 0x13B2B871, 0xEB826EEA, 0xDE2EA200 }, 3376, 0 },
    { 18, 0, { 0x147006D7, 0xEB860EEA, 0xDF91B288 }, 3360, 0 },
    { 18, 0, { 0x163EB5B3, 0xEA584EEA, 0xDE76D2D8 }, 2352, 0 },
    { 18, 0, { 0x15935139, 0xEA504EEA, 0xDD1127D0 }, 304,  0 },
    { 18, 0, { 0x17187BE1, 0xEA044EEA, 0xDC43F6B2 }, 1312, 0 },
    { 18, 0, { 0x17D35A51, 0xEA072EEA, 0xDD93D6BA }, 1312, 0 },
    { -1, 0, { 0x00000000, 0x00000000, 0x00000000 }, 0,    0 },
};

RaceCourseSceneryEntry gQuicksandValleyCourseSceneryEntries[] = {
    { 8,  0, { 0xFCA42745, 0x0169B6DA, 0xF7136E88 }, 3008, 0 },
    { 8,  0, { 0x46B6BF2E, 0xD8100000, 0x04CBF586 }, 2704, 0 },
    { 19, 0, { 0x46B6BF2E, 0xD8100000, 0x04CBF586 }, 2704, 0 },
    { -1, 0, { 0x00000000, 0x00000000, 0x00000000 }, 0,    0 },
};

RaceCourseSceneryEntry gSilverMountainCourseSceneryEntries[] = {
    { 8,  0, { 0x9F401566, 0xDD5C0000, 0xE785C37D }, 3296, 0 },
    { 19, 0, { 0x9F401566, 0xDD5C0000, 0xE785C37D }, 3296, 0 },
    { 8,  0, { 0xF61512AA, 0x00011DE4, 0xF93B37B9 }, 3264, 0 },
    { -1, 0, { 0x00000000, 0x00000000, 0x00000000 }, 0,    0 },
};

RaceCourseSceneryEntry gAnimalLandCourseSceneryEntries[] = {
    { 11, 0, { 0xF3C2296E, 0xF2A20000, 0xEE5E0129 }, 1792, 0 },
    { -1, 0, { 0x00000000, 0x00000000, 0x00000000 }, 0,    0 },
};

RaceCourseSceneryEntry gNinjaLandCourseSceneryEntries[] = {
    { 19, 0, { 0x19B8F477, 0xF2F60000, 0xE965AC2E }, 2432, 0 },
    { 8,  0, { 0xFB2EE4C4, 0x013F685D, 0x035893BA }, 3968, 0 },
    { -1, 0, { 0x00000000, 0x00000000, 0x00000000 }, 0,    0 },
};

RaceCourseSceneryEntry gRookieMountainCourseSceneryEntries[] = {
    { 8,  0, { 0x14DA6C89, 0xEEC47C98, 0xF4485684 }, 3792, 0 },
    { 19, 0, { 0x14DA6C89, 0xEEC47C98, 0xF4485684 }, 3792, 0 },
    { 8,  0, { 0x00000000, 0xFFFDFDD7, 0x0003AEE6 }, 0,    0 },
    { 10, 0, { 0x1470B7EE, 0xEECD0856, 0xF615DFCF }, 2832, 0 },
    { -1, 0, { 0x00000000, 0x00000000, 0x00000000 }, 0,    0 },
};

RaceCourseSceneryEntry *gRaceCourseSceneryEntriesByCourse[] = {
    gBigSnowmanCourseSceneryEntries,
    gSunsetRockCourseSceneryEntries,
    gNightHighwayCourseSceneryEntries,
    gGrassValleyCourseSceneryEntries,
    gDizzyLandCourseSceneryEntries,
    gQuicksandValleyCourseSceneryEntries,
    gSilverMountainCourseSceneryEntries,
    gAnimalLandCourseSceneryEntries,
    gNinjaLandCourseSceneryEntries,
    gRookieMountainCourseSceneryEntries,
};

RaceCourseGateEntry gCourseGateSoundParams[] = {
    { { 0xC3623C28, 0xAC940000, 0xD129BAB2 }, 1584, 0 },
    { { 0x253CC0AA, 0xB9A9E000, 0xA4A99C5C }, 3824, 0 },
    { { 0x1720C991, 0xD05E70EE, 0xC940D8EA }, 1168, 0 },
    { { 0xB4967EF1, 0xD0530400, 0x9B1C046F }, 928,  0 },
    { { 0x021EC3FF, 0xD2640000, 0xA39AFD40 }, 512,  0 },
    { { 0x471CC8AB, 0xD8140000, 0x044C759E }, 1728, 0 },
    { { 0x9F10F80F, 0xDD600000, 0xE6DD4687 }, 2288, 0 },
    { { 0x00000000, 0x00000000, 0x00000000 }, 0,    0 },
    { { 0x1A4B7210, 0xF2FA0000, 0xE9069D8E }, 1408, 0 },
    { { 0x1442F7DC, 0xEEC80E50, 0xF3F545C2 }, 2768, 0 },
};

RaceCourseBillboardEntry gCourseBillboardMarkerEntries[] = {
    { 0x02000060, 0x02000088, 0x02000000, 6,  1, 3, 0 },
    { 0x0200BB90, 0x0200BBB8, 0x0200BA10, 24, 1, 3, 0 },
    { 0x0200BCC0, 0x0200BCE8, 0x0200BC00, 12, 1, 3, 0 },
};

RaceCourseTriggerEntry gCourseTriggerEntries[] = {
    { 2, 1, 0x186, 0, { 0x14CB79E2, 0xD85220A4, 0xD28EDA6A }, 0xF90, 0x70, 0x02009148 },
    { 4, 6, 0x122, 0, { 0xF7B42249, 0xF305D31B, 0xEB7622AF }, 0xFBA, 0x400, 0x02006988 },
};

extern s32 gMenuFlowState;
extern u8 gCurrentViewportIndex;
extern u8 gRaceUpdatePaused;
extern RaceCamera D_801121E0[];
extern Gfx *gRegionAllocPtr;

void drawRaceCountdownReadyPrompt(RaceCountdownEffect *arg0) {
    if (arg0->scaleStep != 0) {
        drawScaledAssetTableSprite(-0x34, -0xC, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)), 0x3F, arg0->scaleStep);
    } else {
        drawAssetTableSprite(-0x34, -0xC, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)), 0x3F);
    }
}

void drawRaceCountdownGoPrompt(RaceCountdownEffect *arg0) {
    if (arg0->scaleStep != 0) {
        drawScaledAssetTableSprite(-0x20, -0xC, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)), 0x40, arg0->scaleStep);
    } else {
        drawAssetTableSprite(-0x20, -0xC, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)), 0x40);
    }
}

void updateRaceCountdownGoPromptOut(RaceCountdownEffect *arg0) {
    arg0->scaleStep++;
    if (arg0->scaleStep == 4) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceCountdownGoPrompt, arg0);
}

void updateRaceCountdownGoPromptHold(RaceCountdownEffect *arg0) {
    RaceCountdownEffect *temp_a2 = arg0;

    if (arg0->scaleStep != 0) {
        arg0->scaleStep--;
    }
    temp_a2->timer--;
    if (temp_a2->timer == 0) {
        gMenuFlowState &= ~1;
        setCallbackTaskCallback(temp_a2, (CallbackTaskCallback)updateRaceCountdownGoPromptOut);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceCountdownGoPrompt, temp_a2);
}

void updateRaceCountdownReadyPromptIn(RaceCountdownEffect *arg0) {
    arg0->scaleStep++;
    if (arg0->scaleStep == 4) {
        enqueueSoundEffect(0x4C, 0x5A);
        arg0->timer = 0x14;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceCountdownGoPromptHold);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceCountdownReadyPrompt, arg0);
}

void updateRaceCountdownReadyPromptHold(RaceCountdownEffect *arg0) {
    if (arg0->scaleStep != 0) {
        arg0->scaleStep--;
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
        arg0->scaleStep = 4;
        arg0->timer = 0x3C;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceCountdownReadyPromptHold);
    }
}

void initRaceCountdownPrompt(RaceCountdownEffect *arg0) {
    arg0->timer = 0x14;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceCountdownInitialDelay);
}

void renderRaceCourseModel(RaceCourseModelRenderEffect *arg0) {
    volatile u8 pad[0x30];

    gDPPipeSync(gRegionAllocPtr++);

    gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0x8)));
    gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0x9)));

    gSPMatrix(gRegionAllocPtr++, &gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    switch (gRaceCourseIndex.unsignedValue) {
        case 0:
            gSPDisplayList(gRegionAllocPtr++, &BIG_SNOWMAN_COURSE_DISPLAY_LIST_VRAM);
            break;
        case 1:
            gSPDisplayList(gRegionAllocPtr++, &SUNSET_ROCK_COURSE_DISPLAY_LIST_VRAM);
            break;
        case 2:
            gSPDisplayList(gRegionAllocPtr++, &NIGHT_HIGHWAY_COURSE_DISPLAY_LIST_VRAM);
            break;
        case 3:
            gSPDisplayList(gRegionAllocPtr++, &GRASS_VALLEY_COURSE_DISPLAY_LIST_VRAM);
            break;
        case 4:
            gSPDisplayList(gRegionAllocPtr++, &DIZZY_LAND_COURSE_DISPLAY_LIST_VRAM);
            break;
        case 5:
            gSPDisplayList(gRegionAllocPtr++, &QUICKSAND_VALLEY_COURSE_DISPLAY_LIST_VRAM);
            break;
        case 6:
            gSPDisplayList(gRegionAllocPtr++, &SILVER_MOUNTAIN_COURSE_DISPLAY_LIST_VRAM);
            break;
        case 7:
            gSPDisplayList(gRegionAllocPtr++, &ANIMAL_LAND_COURSE_DISPLAY_LIST_VRAM);
            break;
        case 8:
            gSPDisplayList(gRegionAllocPtr++, &NINJA_LAND_COURSE_DISPLAY_LIST_VRAM);
            break;
        case 9:
            gSPDisplayList(gRegionAllocPtr++, &ROOKIE_MOUNTAIN_COURSE_DISPLAY_LIST_VRAM);
            break;
    }
}

void renderRaceCourseBackdrop(RaceCourseModelRenderEffect *arg0) {
    s32 matrixFlags;
    void *textureBase;
    Transform3D sp100;
    volatile u8 pad[8];

    sp100 = gIdentityFixedTransform;
    sp100.translation.x = -D_801121E0[gCurrentViewportIndex].cameraTransform.translation.x;
    sp100.translation.y = -D_801121E0[gCurrentViewportIndex].cameraTransform.translation.y;
    matrixFlags = G_MTX_NOPUSH;
    sp100.translation.z = -D_801121E0[gCurrentViewportIndex].cameraTransform.translation.z;

    arg0->backdropMatrix = allocFixedTransformMatrix(&sp100);
    if (arg0->backdropMatrix != NULL) {
        switch (gRaceCourseIndex.unsignedValue) {
            case 0:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->backdropMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(gRegionAllocPtr++, &BIG_SNOWMAN_COURSE_BACKDROP_DISPLAY_LIST_VRAM);
                gSPDisplayList(gRegionAllocPtr++, &BIG_SNOWMAN_COURSE_SECONDARY_BACKDROP_DISPLAY_LIST_VRAM);
                break;
            case 1:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03, textureBase = getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->backdropMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(gRegionAllocPtr++, &SUNSET_ROCK_COURSE_BACKDROP_DISPLAY_LIST_VRAM);
                break;
            case 2:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->backdropMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(gRegionAllocPtr++, &NIGHT_HIGHWAY_COURSE_BACKDROP_DISPLAY_LIST_VRAM);
                break;
            case 3:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->backdropMatrix, (G_MTX_NOPUSH | G_MTX_LOAD) | matrixFlags);
                gSPDisplayList(gRegionAllocPtr++, &GRASS_VALLEY_COURSE_BACKDROP_DISPLAY_LIST_VRAM);
                break;
            case 4:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->backdropMatrix, (matrixFlags | G_MTX_LOAD) | matrixFlags);
                gSPDisplayList(gRegionAllocPtr++, &DIZZY_LAND_COURSE_BACKDROP_DISPLAY_LIST_VRAM);
                break;
            case 5:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->backdropMatrix, (matrixFlags | G_MTX_LOAD) | matrixFlags);
                gSPDisplayList(gRegionAllocPtr++, &QUICKSAND_VALLEY_COURSE_BACKDROP_DISPLAY_LIST_VRAM);
                gSPDisplayList(gRegionAllocPtr++, &QUICKSAND_VALLEY_COURSE_SECONDARY_BACKDROP_DISPLAY_LIST_VRAM);
                break;
            case 6:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->backdropMatrix, (matrixFlags | G_MTX_LOAD) | matrixFlags);
                gSPDisplayList(gRegionAllocPtr++, &SILVER_MOUNTAIN_COURSE_BACKDROP_DISPLAY_LIST_VRAM);
                break;
            case 7:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->backdropMatrix, (matrixFlags | G_MTX_LOAD) | matrixFlags);
                gSPDisplayList(gRegionAllocPtr++, &ANIMAL_LAND_COURSE_BACKDROP_DISPLAY_LIST_VRAM);
                break;
            case 8:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->backdropMatrix, (matrixFlags | G_MTX_LOAD) | matrixFlags);
                gSPDisplayList(gRegionAllocPtr++, &NINJA_LAND_COURSE_BACKDROP_DISPLAY_LIST_VRAM);
                break;
            case 9:
                gDPPipeSync(gRegionAllocPtr++);
                gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[8]));
                gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[9]));
                gSPMatrix(gRegionAllocPtr++, arg0->backdropMatrix, (matrixFlags | G_MTX_LOAD) | matrixFlags);
                gSPDisplayList(gRegionAllocPtr++, &ROOKIE_MOUNTAIN_COURSE_BACKDROP_DISPLAY_LIST_VRAM);
                break;
        }
    }
}

void updateRaceCourseModelRenderTask(RaceCourseModelRenderEffect *arg0) {
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderRaceCourseModel, arg0);
    addRenderCallback(&gBackdropRenderCallbackList, (RenderCallback)renderRaceCourseBackdrop, arg0);
}

void initRaceCourseModelRenderTask(RaceCourseModelRenderEffect *arg0) {
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceCourseModelRenderTask);
}

void drawFinalLapPromptForViewport(CallbackTask *arg0) {
    if (gCurrentViewportIndex == arg0->header.userId) {
        drawAssetTableSprite(-0x30, -0xC, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)), 0x41);
    }
}

void updateFinalLapPrompt(CallbackTask *arg0) {
    if (gMenuFlowState & 8) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawFinalLapPromptForViewport, arg0);
}

void initFinalLapPrompt(CallbackTask *arg0) {
    enqueueSoundEffect(0x52, 0x5A);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateFinalLapPrompt);
}

void renderCourseTextureMarkers(RaceCourseObjectMatrixEffect *arg0) {
    volatile u8 pad[8];
    void *image;
    void *palette;
    s16 width;
    s16 height;
    RaceCourseTextureMarkerEntry *entry;
    s16 textureIndex;
    s32 i;

    textureIndex = -1;
    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
    entry = gCourseTextureMarkerSpawnEntriesByCourse[gRaceCourseIndex.signedValue];
    i = 0;
    if (entry->type != -1) {
        do {
            if (isPositionNearCurrentRaceViewportCamera(&entry->position) != 0) {
                if (entry->type != textureIndex) {
                    textureIndex = entry->type;
                    getAssetTableImagePaletteAndSize(
                        (u8 *)getRelocatableHeapBlockBase((s32)ASSET_HANDLE(0x1C)),
                        textureIndex,
                        &image,
                        &palette,
                        &width,
                        &height
                    );
                    gDPLoadTextureBlock_4b(
                        gRegionAllocPtr++,
                        image,
                        G_IM_FMT_CI,
                        width,
                        height,
                        0,
                        G_TX_CLAMP,
                        G_TX_CLAMP,
                        G_TX_NOMASK,
                        G_TX_NOMASK,
                        G_TX_NOLOD,
                        G_TX_NOLOD
                    );
                    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
                }
                gSPMatrix(gRegionAllocPtr++, &arg0->matrices[i], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
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

void updateCourseTextureMarkers(RaceCourseObjectMatrixEffect *arg0) {
    RaceCourseTextureMarkerEntry *entry;
    s8 type;

    entry = gCourseTextureMarkerSpawnEntriesByCourse[gRaceCourseIndex.signedValue];
    if (entry->type != -1) {
        do {
            switch (entry->type) {
                case 0:
                case 1:
                    pushRacePlayersOutOfCylinderAndApplyItemHit(&entry->position, 0xC0000, 0x600000, 2);
                    break;
                case 2:
                    pushRacePlayersOutOfCylinderOrApplyItemHit(&entry->position, 0x20000, 0x40000, 0x30000, 4);
                    break;
            }
            type = entry[1].type;
            entry++;
        } while (entry->type != -1);
    }

    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)renderCourseTextureMarkers, arg0);
}

void initCourseTextureMarkers(RaceCourseObjectMatrixEffect *arg0) {
    RaceCourseTextureMarkerEntry *entry;
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
        arg0->matrices = getRelocatableHeapBlockBase(gAssetHandles[0x21]);

        i = 0;
        if (count > 0) {
            do {
                arg0->matrices[i] = gIdentityMatrix;
                setPackedMatrixTranslation(&arg0->matrices[i], &entry->position);
                i++;
                entry++;
            } while (i != count);
        }
        osWritebackDCache(arg0->matrices, allocSize);
    }

    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseTextureMarkers);
}

void renderRaceCourseSceneryObjects(RaceCourseObjectMatrixEffect *arg0) {
    RaceCourseSceneryEntry *var_s4;
    s32 var_s5;
    s32 var_s7;
    Gfx *temp_s0;
    Gfx *temp_s2;
    Gfx *temp_s3;

    var_s4 = gRaceCourseSceneryEntriesByCourse[gRaceCourseIndex.signedValue];
    var_s7 = TRUE;
    var_s5 = 0;
    if (var_s4->modelIndex != -1) {
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
                gDma1p(temp_s0, 1, (u32)arg0->matrices + (var_s5 << 6), 0x40, 2);

                temp_s0 = gRegionAllocPtr++;
                gSPDisplayList(temp_s0, gRaceCourseObjectDisplayLists[var_s4->modelIndex]);
            }
            var_s4++;
            var_s5++;
        } while (var_s4->modelIndex != -1);
    }
}

void updateRaceCourseSceneryObjects(RaceCourseObjectMatrixEffect *arg0) {
    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)renderRaceCourseSceneryObjects, arg0);
}

void initRaceCourseSceneryObjects(RaceCourseObjectMatrixEffect *arg0) {
    s32 size;
    RaceCourseSceneryEntry *base;
    RaceCourseSceneryEntry *entry;
    s32 i;
    Transform3D transform;
    s32 count;

    base = gRaceCourseSceneryEntriesByCourse[gRaceCourseIndex.signedValue];
    count = 0;
    entry = base;
    if (base->modelIndex != -1) {
        do {
            count++;
            entry++;
        } while (entry->modelIndex != -1);
    }

    if (count != 0) {
        entry = base;
        size = count << 6;
        gAssetHandles[0x22] = allocRelocatableHeapBlock(size);
        arg0->matrices = getRelocatableHeapBlockBase(gAssetHandles[0x22]);

        for (i = 0; i < count; i++) {
            makeFixedRotationY(transform.rotation, entry->rotation);
            transform.translation.x = entry->position.x;
            transform.translation.y = entry->position.y;
            transform.translation.z = entry->position.z;
            packFixedTransformMatrix(&transform, &arg0->matrices[i]);
            entry++;
        }

        osWritebackDCache(arg0->matrices, size);
    }
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceCourseSceneryObjects);
}

void renderPatrolCourseObject(PatrolCourseObjectEffect *arg0) {
    s32 sine;
    s32 doubleSine;
    Transform3D transform;
    volatile s32 pad0[18];

    if (gRenderMatricesDirty != 0) {
        arg0->matrixValid = 0;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->position) != 0) {
        if (arg0->matrixValid == 0) {
            arg0->matrixValid = 1;
            sine = fixedSine(arg0->animationPhase);
            if (1) {
                doubleSine = fixedSine((s16)(arg0->animationPhase * 2));
                sine >>= 4;
                makeFixedRotationY(transform.rotation, arg0->angle + sine + 0x800);
                transform.translation.x = arg0->position.x;
                transform.translation.y = (arg0->position.y + (((doubleSine + 0x1000) << 2) << 2)) + 0xA4000;
            }
            transform.translation.z = arg0->position.z;
            scaleFixedMatrix3sByQuarter(transform.rotation);
            arg0->matrix = allocFixedTransformMatrix(&transform);
        }

        if ((((&transform) && (&transform)) && (&transform)) & 0xFFFFu) {}

        if (arg0->matrix != NULL) {
            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
            gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
            gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
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
        if (isPositionNearAnyRaceViewportFocus(&arg0->position) != 0) {
            if (arg0->movingTowardStart != 0) {
                targetAngle =
                    calculateFixedAngleBetweenXZPoints(arg0->position.x, arg0->position.z, arg0->startPosition.x, arg0->startPosition.z);
            } else {
                targetAngle = calculateFixedAngleBetweenXZPoints(arg0->position.x, arg0->position.z, arg0->endPosition.x, arg0->endPosition.z);
            }
            var_v1 = (((((targetAngle & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) - arg0->angle) & 0xFFF;
            targetAngle = -targetAngle;
            var_v1 = (s16)var_v1;
            if (var_v1 >= 0x801) {
                var_v1 = (s16)(var_v1 - 0x1000);
            }
            if (arg0->maxTurnStep < var_v1) {
                var_v1 = (s16)(s32)arg0->maxTurnStep;
            }
            if (var_v1 < -arg0->maxTurnStep) {
                var_v1 = (s16)-arg0->maxTurnStep;
            }
            arg0->angle = (s16)(arg0->angle + var_v1);
            arg0->position.x += fixedSine(arg0->angle) * ((s32)-arg0->speed / 4096);
            local.dz = (arg0->position.z += fixedCosine(arg0->angle) * ((s32)-arg0->speed / 4096));
            arg0->surfaceIndex = findRaceCourseSurfaceFromHint(arg0->surfaceIndex, arg0->position.x, local.dz);
            arg0->position.y = getRaceCourseSurfaceHeight(arg0->surfaceIndex, arg0->position.x, arg0->position.z);
            if (arg0->movingTowardStart != 0) {
                targetAngle = arg0->position.x - arg0->startPosition.x;
                var_v1 = arg0->position.z - arg0->startPosition.z;
                if (targetAngle < 0) {
                    targetAngle = -targetAngle;
                }
                if (var_v1 < 0) {
                    var_v1 = -var_v1;
                }
                if (targetAngle < 0x30000) {
                    if (var_v1 < 0x30000) {
                        arg0->movingTowardStart = 0;
                    }
                }
            } else {
                targetAngle = arg0->position.x - arg0->endPosition.x;
                var_v1 = arg0->position.z - arg0->endPosition.z;
                if (targetAngle < 0) {
                    targetAngle = -targetAngle;
                }
                if (var_v1 < 0) {
                    var_v1 = -var_v1;
                }
                if ((targetAngle < 0x30000) && (var_v1 < 0x30000)) {
                    arg0->movingTowardStart = 1;
                }
            }
            pushRacePlayersOutOfCylinderOrApplyItemHit(&arg0->position, 0x40000, 0x50000, 0x30000, 4);
            arg0->animationPhase += arg0->animationPhaseStep;
            if (arg0->animationPhase == 0) {
                rand = randomNextSecondary();
                rand = rand & 3;
                if (rand == 1) {
                    arg0->speed = 0x10000;
                    arg0->animationPhaseStep = 0x80;
                    arg0->maxTurnStep = 0x10;
                }
                if (rand == 2) {
                    arg0->speed = 0x20000;
                    arg0->animationPhaseStep = 0x100;
                    arg0->maxTurnStep = 0x20;
                }
                if (rand == 3) {
                    arg0->speed = 0x40000;
                    arg0->animationPhaseStep = 0x200;
                    arg0->maxTurnStep = 0x40;
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
        angle = calculateFixedAngleBetweenXZPoints(arg0->startPosition.x, arg0->startPosition.z, arg0->endPosition.x, arg0->endPosition.z);
        arg0->position = arg0->startPosition;
        temp4C = 0x10;
        temp50 = 0x10000;
        temp4E = 0x80;
        arg0->angle = angle;
        arg0->maxTurnStep = temp4C;
        arg0->speed = temp50;
        arg0->animationPhaseStep = temp4E;
        arg0->position.y = getRaceCourseSurfaceHeight(arg0->surfaceIndex, arg0->position.x, arg0->position.z);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updatePatrolCourseObject);
    }
}

void spawnPatrolCourseObject(s16 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    PatrolCourseObjectEffect *p = createCallbackTask((CallbackTaskCallback)initPatrolCourseObject, 0, 0x64);
    if (p != 0) {
        p->startPosition.x = arg1;
        p->startPosition.z = arg2;
        p->endPosition.x = arg3;
        p->endPosition.z = arg4;
        p->surfaceIndex = arg0;
    }
}

void renderLaunchRampCourseObject(RaceMovingCourseObjectEffect *arg0) {
    volatile s32 unused;
    Transform3D transform;
    volatile s32 pad[1];

    if (gRenderMatricesDirty != 0) {
        makeFixedRotationY(
            transform.rotation,
            gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampCourseObjectAngle + 0x400
        );
        transform.translation.x = arg0->position.x;
        transform.translation.y = arg0->position.y;
        transform.translation.z = arg0->position.z;
        arg0->matrix = allocFixedTransformMatrix(&transform);
    }

    if (arg0->matrix != NULL) {
        if (isPositionNearCurrentRaceViewportCamera(&arg0->position) != 0) {
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

void updateLaunchRampCourseObjectExit(RaceMovingCourseObjectEffect *arg0) {
    Vec3i sp24;
    s16 temp_v0;
    s32 temp_v1;
    RaceMovingCourseObjectEffect *temp_a3 = arg0;

    temp_v0 = arg0->timer;
    if (temp_v0 != 0) {
        if (gRaceUpdatePaused == 0) {
            temp_v1 = arg0->velocity.z;
            arg0->timer = temp_v0 - 1;
            if (temp_v1 >= (s32)0xFFF60001) {
                arg0->velocity.z = temp_v1 - 0x2000;
            }
            transformVec3iByFixedMatrix(arg0->movementTransform.rotation, &temp_a3->velocity, &sp24);
            temp_a3->position.x += sp24.x;
            temp_a3->position.y += sp24.y;
            temp_a3->position.z += sp24.z;
        }
        addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderLaunchRampCourseObject, temp_a3);
        return;
    }
    removeCallbackTask(temp_a3);
}

void updateLaunchRampCourseObjectArc(RaceMovingCourseObjectEffect *arg0) {
    Vec3i sp2C;
    s16 *mtx;

    if (gRaceUpdatePaused == 0) {
        arg0->timer--;
        if (arg0->timer < 0x13) {
            arg0->velocity.z = 0xFFFC0000;
        }

        mtx = arg0->movementTransform.rotation;
        transformVec3iByFixedMatrix(mtx, &arg0->velocity, &sp2C);
        arg0->position.x += sp2C.x;
        arg0->position.y += sp2C.y;
        arg0->position.z += sp2C.z;

        if (arg0->timer == 0) {
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateLaunchRampCourseObjectExit);
            makeFixedRotationXY(
                mtx,
                0x100,
                gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampCourseObjectAngle + 0x400
            );
            arg0->timer = 0x64;
        }
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderLaunchRampCourseObject, arg0);
}

void initLaunchRampCourseObject(RaceMovingCourseObjectEffect *arg0) {
    s16 *mtx;

    arg0->timer = 0x46;
    arg0->velocity.z = 0x680000;
    mtx = arg0->movementTransform.rotation;
    makeFixedRotationY(mtx, gRaceCourseStartEntries[gRaceCourseIndex.signedValue].launchRampCourseObjectAngle + 0x400);
    transformVec3iByFixedMatrix(mtx, &arg0->velocity, &arg0->position);
    arg0->velocity.z = 0xFFFE0000;
    arg0->position.x += gRaceCourseStartEntries[COURSE_INDEX_RELOAD].launchRampOrigin.x;
    arg0->position.y += gRaceCourseStartEntries[COURSE_INDEX_RELOAD].launchRampOrigin.y;
    arg0->position.z += gRaceCourseStartEntries[COURSE_INDEX_RELOAD].launchRampOrigin.z;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateLaunchRampCourseObjectArc);
}

void renderSpiralCourseObject(RaceMovingCourseObjectEffect *arg0) {
    volatile s32 unused;
    Transform3D transform;
    volatile s32 pad[2];

    if (gRenderMatricesDirty != 0) {
        makeFixedRotationY(transform.rotation, arg0->yaw);
        transform.translation.x = arg0->position.x;
        transform.translation.y = arg0->position.y;
        transform.translation.z = arg0->position.z;
        arg0->matrix = allocFixedTransformMatrix(&transform);
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->position) != 0) {
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

void updateSpiralCourseObjectExit(RaceMovingCourseObjectEffect *arg0) {
    Vec3i sp24;
    s16 temp_v0;
    RaceMovingCourseObjectEffect *temp_a3 = arg0;

    temp_v0 = arg0->timer;
    if (temp_v0 != 0) {
        if (gRaceUpdatePaused == 0) {
            arg0->timer = temp_v0 - 1;
            transformVec3iByFixedMatrix(arg0->movementTransform.rotation, &temp_a3->velocity, &sp24);
            temp_a3->position.x += sp24.x;
            temp_a3->position.y += sp24.y;
            temp_a3->position.z += sp24.z;
        }
        addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderSpiralCourseObject, temp_a3);
        return;
    }
    removeCallbackTask(temp_a3);
}

void updateSpiralCourseObjectTurn(RaceMovingCourseObjectEffect *arg0) {
    Vec3i sp2C;
    void *velocity;

    if (gRaceUpdatePaused == 0) {
        arg0->timer--;
        arg0->yaw -= 0x2A;
        makeFixedRotationY(arg0->movementTransform.rotation, arg0->yaw);

        velocity = &arg0->velocity;
        transformVec3iByFixedMatrix(arg0->movementTransform.rotation, velocity, &sp2C);
        arg0->position.x += sp2C.x;
        arg0->position.y += sp2C.y;
        arg0->position.z += sp2C.z;

        arg0->yaw -= 0x2A;
        makeFixedRotationY(arg0->movementTransform.rotation, arg0->yaw);
        transformVec3iByFixedMatrix(arg0->movementTransform.rotation, velocity, &sp2C);
        arg0->position.x += sp2C.x;
        arg0->position.y += sp2C.y;
        arg0->position.z += sp2C.z;

        if (arg0->timer == 0) {
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateSpiralCourseObjectExit);
            arg0->timer = 0x38;
        }
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderSpiralCourseObject, arg0);
}

void updateSpiralCourseObjectLaunch(RaceMovingCourseObjectEffect *arg0) {
    Vec3i sp1C;
    RaceMovingCourseObjectEffect *temp_a3 = arg0;

    if (gRaceUpdatePaused == 0) {
        arg0->timer--;
        transformVec3iByFixedMatrix(arg0->movementTransform.rotation, &temp_a3->velocity, &sp1C);
        temp_a3->position.x += sp1C.x * 2;
        temp_a3->position.y += sp1C.y * 2;
        temp_a3->position.z += sp1C.z * 2;
        if (temp_a3->timer == 0) {
            setCallbackTaskCallback(temp_a3, (CallbackTaskCallback)updateSpiralCourseObjectTurn);
            temp_a3->timer = 0x18;
        }
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderSpiralCourseObject, temp_a3);
}

void initSpiralCourseObject(RaceMovingCourseObjectEffect *arg0) {
    s16 *mtx;

    arg0->timer = 0x28;
    arg0->yaw = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].spiralCourseObjectAngle;
    arg0->velocity.x = -0x200000;
    arg0->velocity.z = 0x400000;
    mtx = arg0->movementTransform.rotation;
    makeFixedRotationY(mtx, arg0->yaw);
    transformVec3iByFixedMatrix(mtx, &arg0->velocity, &arg0->position);
    arg0->velocity.x = 0;
    arg0->velocity.z = -0x20000;
    arg0->position.x += gRaceCourseStartEntries[gRaceCourseIndex.signedValue].spiralOrigin.x;
    arg0->position.y += gRaceCourseStartEntries[gRaceCourseIndex.signedValue].spiralOrigin.y + 0x40000;
    arg0->position.z += gRaceCourseStartEntries[gRaceCourseIndex.signedValue].spiralOrigin.z;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateSpiralCourseObjectLaunch);
    updateSpiralCourseObjectLaunch(arg0);
}

void renderCourseGateObject(RaceCourseGateEffect *arg0) {
    Transform3D scratch;
    volatile s32 pad[2];
    RaceCourseGateEffect *temp_s0 = arg0;
    Gfx *segment1;
    Gfx *segment2;

    if (gRenderMatricesDirty != 0) {
        temp_s0->baseMatrix = NULL;
        temp_s0->firstPanelMatrix = NULL;
        temp_s0->secondPanelMatrix = NULL;
    }

    if (isPositionNearCurrentRaceViewportCamera(&gCourseGateSoundParams[gRaceCourseIndex.signedValue].position) == 0) {
        return;
    }

    if (temp_s0->baseMatrix == NULL) {
        temp_s0->baseMatrix = allocFixedTransformMatrix(&temp_s0->baseTransform);
    }

    if (temp_s0->baseMatrix != NULL) {
        gDPPipeSync(gRegionAllocPtr++);
        segment1 = gRegionAllocPtr++;
        segment1->words.w0 = 0xBC000806;
        segment1->words.w1 = (u32)getRelocatableHeapBlockBase(ASSET_HANDLE(0xA));
        segment2 = gRegionAllocPtr++;
        segment2->words.w0 = 0xBC000C06;
        segment2->words.w1 = (u32)getRelocatableHeapBlockBase(ASSET_HANDLE(0xB));
        gSPMatrix(gRegionAllocPtr++, temp_s0->baseMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, &_148F88_VRAM);
    }

    if (temp_s0->firstPanelMatrix == NULL) {
        makeFixedRotationZY(
            scratch.rotation,
            gCourseGateSoundParams[gRaceCourseIndex.signedValue].angle,
            temp_s0->firstPanelAngle
        );
        scratch.translation.x = temp_s0->firstPanelPosition.x;
        scratch.translation.y = temp_s0->firstPanelPosition.y;
        scratch.translation.z = temp_s0->firstPanelPosition.z;
        temp_s0->firstPanelMatrix = allocFixedTransformMatrix(&scratch);
    }

    if (temp_s0->firstPanelMatrix != NULL) {
        gSPMatrix(gRegionAllocPtr++, temp_s0->firstPanelMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, &_149040_VRAM);
    }

    if (temp_s0->secondPanelMatrix == NULL) {
        scratch = temp_s0->baseTransform;
        scratch.translation.x = temp_s0->secondPanelPosition.x;
        scratch.translation.y = temp_s0->secondPanelPosition.y;
        scratch.translation.z = temp_s0->secondPanelPosition.z;
        temp_s0->secondPanelMatrix = allocFixedTransformMatrix(&scratch);
    }

    if (temp_s0->secondPanelMatrix != NULL) {
        gSPMatrix(gRegionAllocPtr++, temp_s0->secondPanelMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        if (temp_s0->isOpen == 0) {
            gSPDisplayList(gRegionAllocPtr++, &_149120_VRAM);
        } else {
            gSPDisplayList(gRegionAllocPtr++, &_1491F8_VRAM);
        }
    }
}

void updateCourseGateClosing(RaceCourseGateEffect *arg0) {
    if (gRaceUpdatePaused == 0) {
        if (arg0->firstPanelAngle != 0) {
            arg0->firstPanelAngle += 0x80;
        } else {
            arg0->isOpen = 0;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitForCourseGateTrigger);
        }
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderCourseGateObject, arg0);
}

void updateCourseGateOpening(RaceCourseGateEffect *arg0) {
    RaceCourseGateEffect *temp_s0 = arg0;
    s16 temp_v0;

    if (gRaceUpdatePaused == 0) {
        temp_v0 = arg0->firstPanelAngle;
        if (temp_v0 != -0x400) {
            arg0->firstPanelAngle = temp_v0 - 0x40;
        }
        temp_s0->openTimer--;
        if (temp_s0->openTimer == 0) {
            enqueuePositionalSoundEffect(
                0x1C, &gCourseGateSoundParams[gRaceCourseIndex.signedValue].position, 0x7F, 0x32
            );
            setCallbackTaskCallback(temp_s0, (CallbackTaskCallback)updateCourseGateClosing);
        }
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderCourseGateObject, temp_s0);
}

void waitForCourseGateTrigger(RaceCourseGateEffect *arg0) {
    if ((gRaceUpdatePaused == 0) && (gMenuFlowState & 4)) {
        arg0->openTimer = 0x2D;
        gMenuFlowState &= ~4;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseGateOpening);
        arg0->isOpen = 1;
        enqueuePositionalSoundEffect(0x16, &gCourseGateSoundParams[gRaceCourseIndex.signedValue].position, 0x7F, 0x32);
        enqueuePositionalSoundEffect(0x1B, &gCourseGateSoundParams[gRaceCourseIndex.signedValue].position, 0x7F, 0x32);
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderCourseGateObject, arg0);
}

void initCourseGateObject(RaceCourseGateEffect *arg0) {
    s16 *mtx;
    Vec3i sp28;

    mtx = arg0->baseTransform.rotation;
    makeFixedRotationY(mtx, gCourseGateSoundParams[gRaceCourseIndex.signedValue].angle);
    arg0->baseTransform.translation.x = gCourseGateSoundParams[gRaceCourseIndex.signedValue].position.x;
    arg0->baseTransform.translation.y = gCourseGateSoundParams[gRaceCourseIndex.signedValue].position.y;
    arg0->baseTransform.translation.z = gCourseGateSoundParams[gRaceCourseIndex.signedValue].position.z;
    sp28.x = 0x18000;
    sp28.y = 0x120000;
    sp28.z = -0x80000;
    transformVec3iByFixedMatrix(mtx, &sp28, &arg0->firstPanelPosition);
    arg0->firstPanelPosition.x += arg0->baseTransform.translation.x;
    arg0->firstPanelPosition.y += arg0->baseTransform.translation.y;
    arg0->firstPanelPosition.z += arg0->baseTransform.translation.z;
    sp28.x = 0;
    sp28.y = 0x120000;
    sp28.z = 0x50000;
    transformVec3iByFixedMatrix(mtx, &sp28, &arg0->secondPanelPosition);
    arg0->secondPanelPosition.x += arg0->baseTransform.translation.x;
    arg0->secondPanelPosition.y += arg0->baseTransform.translation.y;
    arg0->secondPanelPosition.z += arg0->baseTransform.translation.z;
    arg0->unusedAngle = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitForCourseGateTrigger);
}

void renderCourseBillboardMarker(RaceCourseBillboardEffect *arg0) {
    Gfx *segmentGfx;
    Gfx *gfx;
    s32 i;
    s16 vertexCount;
    Gfx *newGfx;
    u8 pad[0x10];

    if (gRenderMatricesDirty != 0) {
        arg0->vertices = allocMenuRenderScratch((arg0->vertexCount * sizeof(Vtx)) + ((u32)pad & 0));
        if (arg0->vertices != NULL) {
            i = 0;
            if (arg0->vertexCount > 0) {
                do {
                    arg0->vertices[i] = arg0->baseVertices[i];
                    arg0->vertices[i].v.tc[1] += arg0->textureScroll;
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
        segmentGfx->words.w1 = (u32)getRelocatableHeapBlockBase(ASSET_HANDLE(0x8));
        gSPMatrix(gRegionAllocPtr++, &gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, arg0->renderSetupDisplayListAddress);
        gDPLoadTextureBlock_4b(
            gRegionAllocPtr++,
            arg0->texture,
            G_IM_FMT_CI,
            0x20,
            0x40,
            0,
            G_TX_WRAP,
            G_TX_WRAP,
            5,
            6,
            0,
            0
        );
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette);
        gfx = gRegionAllocPtr++;
        vertexCount = arg0->vertexCount;
        gfx->words.w0 = (((vertexCount << 0xA) | ((vertexCount << 4) - 1)) & 0xFFFF) | 0x04000000;
        gfx->words.w1 = (u32)arg0->vertices;
        gSPDisplayList(gRegionAllocPtr++, arg0->geometryDisplayListAddress);
    }
}

void updateCourseBillboardMarker(RaceCourseBillboardEffect *arg0) {
    arg0->textureScroll -= 0x40;
    arg0->textureScroll &= 0x7FF;
    if (arg0->useAlternateRenderQueue != 0) {
        addRenderCallback(&D_801248EC, (RenderCallback)renderCourseBillboardMarker, arg0);
    } else {
        addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderCourseBillboardMarker, arg0);
    }
}

void initCourseBillboardMarker(RaceCourseBillboardEffect *arg0) {
    getAssetTableImageAndPalette(
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)),
        gCourseBillboardMarkerEntries[arg0->task.userId].textureIndex,
        &arg0->texture,
        &arg0->palette
    );
    arg0->baseVertices = resolveAssetTableRelativePointer(
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x8)),
        gCourseBillboardMarkerEntries[arg0->task.userId].baseVerticesAddress
    );

    {
        RaceCourseBillboardEntry *entry = &gCourseBillboardMarkerEntries[arg0->task.userId];

        arg0->vertexCount = entry->vertexCount;
        arg0->renderSetupDisplayListAddress = entry->renderSetupDisplayListAddress;
        arg0->geometryDisplayListAddress = entry->geometryDisplayListAddress;
        arg0->vertexCount = entry->vertexCount;
        arg0->useAlternateRenderQueue = entry->flags & 1;
        arg0->renderFlags = entry->flags & 2;
    }
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseBillboardMarker);
}

void renderCourseTriggerVolume(RaceCourseTriggerEffect *arg0) {
    volatile s32 unused;
    Transform3D transform;
    volatile RaceCourseTriggerEffect *effect;

    effect = arg0;

    if (gRenderMatricesDirty != 0) {
        RaceCourseTriggerEntry *entry = &gCourseTriggerEntries[effect->task.userId];

        makeFixedRotationXY(transform.rotation, entry->pitch, entry->yaw);
        transform.translation.x = gCourseTriggerEntries[arg0->task.userId].position.x;
        transform.translation.y = gCourseTriggerEntries[arg0->task.userId].position.y;
        transform.translation.z = gCourseTriggerEntries[arg0->task.userId].position.z;
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
            _g->words.w1 = (u32)arg0->matrix;
        }
        {
            Gfx *_g = gRegionAllocPtr++;

            _g->words.w0 = 0x06000000;
            _g->words.w1 = arg0->displayListAddress;
        }
    }
}

void collidePlayerWithCourseTriggerVolume(RacePlayer *player, RaceCourseTriggerEffect *trigger) {
    Vec3i delta;
    Vec3i transformed;
    s16 matrix[0x10];
    s64 savedPush;
    RaceCourseTriggerEntry *entry;
    s32 pad;
    s32 push;
    s32 scaleX;
    u16 entryIndex;
    s32 collisionRadius;
    s32 limit;
    s32 positiveLimit;
    s32 negativeLimit;

    if (trigger) {}
    if (trigger) {}
    if (trigger) {}
    if (trigger) {}

    if ((gRaceUpdatePaused != 0) || (player->isActive == 0)) {
        return;
    }

    makeFixedRotationYX(
        matrix,
        -gCourseTriggerEntries[trigger->task.userId].pitch,
        -gCourseTriggerEntries[trigger->task.userId].yaw
    );

    if ((player->stateFlags & 0x2000) == 0) {
        delta.x = player->pos.x - gCourseTriggerEntries[trigger->task.userId].position.x;
        delta.y = player->collisionBottomY - gCourseTriggerEntries[trigger->task.userId].position.y;
        delta.z = player->pos.z - gCourseTriggerEntries[trigger->task.userId].position.z;
        transformVec3iByFixedMatrix(matrix, &delta, &transformed);

        if ((transformed.z >= -trigger->halfDepth) && (trigger->halfDepth >= transformed.z) &&
            (transformed.x >= (-trigger->halfWidth - 0x30000)) &&
            ((trigger->halfWidth + 0x30000) >= transformed.x) &&
            (transformed.y >= -0xFFFFF) && (trigger->height >= transformed.y)) {
            delta.y = trigger->height - transformed.y;
            delta.x = -transformed.x;
            delta.z = 0;

            entry = &gCourseTriggerEntries[trigger->task.userId];
            makeFixedRotationXY(matrix, entry->pitch, entry->yaw);
            transformVec3iByFixedMatrix(matrix, &delta, &transformed);

            player->pos.x += transformed.x;
            player->collisionBottomY += transformed.y;
            player->pos.z += transformed.z;
            player->stateFlags |= 0x02000000;
            player->unk332 = gCourseTriggerEntries[trigger->task.userId].yaw ^ 0;
            player->unk334 = gCourseTriggerEntries[trigger->task.userId].pitch;
            return;
        }
    }

    if (player->unk578 == 0) {
        pushRacePlayerOutOfCylinder(&trigger->positiveDepthCollisionCenter, trigger->halfWidth + 0x30000, 0x120000, player->playerIndex);
        pushRacePlayerOutOfCylinder(&trigger->negativeDepthCollisionCenter, trigger->halfWidth + 0x30000, 0x100000, player->playerIndex);
    }

    delta.x = player->pos.x - gCourseTriggerEntries[trigger->task.userId].position.x;
    delta.y = player->collisionBottomY - gCourseTriggerEntries[trigger->task.userId].position.y;
    delta.z = player->pos.z - gCourseTriggerEntries[trigger->task.userId].position.z;
    transformVec3iByFixedMatrix(matrix, &delta, &transformed);

    if (transformed.y > 0 || -0x160000 > transformed.y || -trigger->halfDepth > transformed.z) {
        return;
    }
    if (trigger->halfDepth < transformed.z ||
        transformed.x < ((-trigger->halfWidth - player->collisionRadius) - 0x30000)) {
        return;
    }

    limit = trigger->halfWidth + player->collisionRadius;
    if ((limit + 0x30000) < transformed.x) {
        return;
    }

    push = 0;
    if (transformed.x >= 0) {
        entry = &gCourseTriggerEntries[trigger->task.userId];
        if (transformed.x < (limit + 0x30000)) {
            push = (limit - transformed.x) + 0x30000;
        }
    } else {
        entry = &gCourseTriggerEntries[trigger->task.userId];
        if ((-limit - 0x30000) < transformed.x) {
            push = (-limit - transformed.x) - 0x30000;
        }
    }

    if (push != 0) {
        makeFixedRotationY(
            matrix,
            gCourseTriggerEntries[trigger->task.userId].yaw
        );
        delta.x = push;
        delta.y = 0;
        delta.z = 0;
        transformVec3iByFixedMatrix(matrix, &delta, &transformed);
        player->pos.x += transformed.x;
        player->pos.z += transformed.z;
    }
}

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
    RaceCourseTriggerEntry *entry;

    entry = &gCourseTriggerEntries[arg0->task.userId];
    arg0->pitch = entry->pitch;
    arg0->yaw = entry->yaw;
    arg0->halfWidth = entry->halfWidth << 0x10;
    arg0->height = entry->height << 0x10;
    arg0->halfDepth = entry->halfDepth << 0x10;
    arg0->displayListAddress = entry->displayListAddress;

    makeFixedRotationXY(scratch.transform.rotation, arg0->pitch, arg0->yaw);

    scratch.source.y = 0;
    scratch.source.x = 0;
    scratch.source.z = arg0->halfDepth;
    transformVec3iByFixedMatrix(scratch.transform.rotation, &scratch.source, &scratch.dest);

    entry = &gCourseTriggerEntries[arg0->task.userId];
    arg0->positiveDepthCollisionCenter.x = entry->position.x + scratch.dest.x;
    arg0->positiveDepthCollisionCenter.y = entry->position.y + scratch.dest.y - 0x100000;
    arg0->positiveDepthCollisionCenter.z = entry->position.z + scratch.dest.z;

    scratch.source.x = 0;
    scratch.source.y = 0;
    scratch.source.z = -arg0->halfDepth;
    transformVec3iByFixedMatrix(scratch.transform.rotation, &scratch.source, &scratch.dest);

    entry = &gCourseTriggerEntries[arg0->task.userId];
    arg0->negativeDepthCollisionCenter.x = entry->position.x + scratch.dest.x;
    arg0->negativeDepthCollisionCenter.y = entry->position.y + scratch.dest.y - 0x100000;
    arg0->negativeDepthCollisionCenter.z = entry->position.z + scratch.dest.z;

    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseTriggerVolume);
}
