#include "game/race/race_state.h"
#include "common.h"
#include "game/audio/sound_manager.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/engine/system_runtime.h"
#include "game/engine/viewport_manager.h"
#include "game/math/fixed_point_math.h"
#include "game/menu/renderer/menu_screen_effects.h"
#include "game/menu/main_menu/main_menu_scene_model.h"
#include "game/menu/main_menu/main_menu_scene_model_renderer.h"
#include "game/race/player/race_player_input.h"

#define MAIN_MENU_GFX_CMD(pkt, cmd0, cmd1) \
    {                                      \
        Gfx *_g = (Gfx *)(pkt);            \
        _g->words.w0 = (cmd0);             \
        _g->words.w1 = (cmd1);             \
    }

typedef struct {
    /* 0x00 */ OverlayActorWord x;
    /* 0x04 */ OverlayActorWord y;
    /* 0x08 */ OverlayActorWord z;
} OverlayActorTransform;

typedef struct {
    /* 0x00 */ Transform3D source;
    /* 0x20 */ s32 pad20;
} TitleMenuRotatingBoardScratch;

extern Gfx *gRegionAllocPtr;

u8 raceSetupCharacterFocusAnimationIds[8] = { 0, 1, 0, 1, 2, 0, 0, 0 };

s16 raceSetupCharacterFocusSoundIds[6] = {
    0x26, 0x2B, 0x39, 0x31, 0x3E, 0,
};

u8 raceSetupOpponentFocusAnimationIds[4] = { 3, 4, 3, 4 };

s32 raceSetupOpponentFocusXPositions[6] = {
    0x05000000, 0xFFA00000, 0xFFE00000, 0x00200000, 0x00600000, 0,
};

Vtx raceStartPlayerEffectVertices[4] = {
    { { { 0, 16, 0 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 16, 16, 0 }, 0, { 1008, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 16, 0, 0 }, 0, { 1008, 1008 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 0, 0 }, 0, { -16, 1008 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

u8 titleMenuSparkleTileFrames[16] = {
    2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5,
};

s16 titleMenuSparklePositions[2] = { 0x58, 0x30 };

s16 courseRecordDigitTileOffsets[10] = {
    0, 0, 3, 6, 0, 0, 6, 0, 0, 0,
};

Vtx mainMenuModeBoardTransitionVertices[4] = {
    { { { -108, 8, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 107, 8, 0 }, 0, { 6912, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 107, -7, 0 }, 0, { 6912, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -108, -7, 0 }, 0, { 0, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

s16 mainMenuModeIconFlashTileOffsets[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 1,
};

s16 mainMenuModeLabelFlashTileOffsets[538] = {
    0x000A, 0x000B, 0x000C, 0x000B, 0x000D, 0x000E, 0x000F, 0x000E, 0x0010, 0x0011, 0x0012, 0x0011, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0xFF97, 0x00D1, 0x0000, 0x0000, 0xFFF0, 0xFFF0, 0xE4E4, 0xE4FF, 0x0069, 0x00D1,
    0x0000, 0x0000, 0x05F0, 0xFFF0, 0xE4E4, 0xE4FF, 0x0069, 0xFFFF, 0x0000, 0x0000, 0x05F0, 0x05F0, 0xE4E4, 0xE4FF,
    0xFF97, 0xFFFF, 0x0000, 0x0000, 0xFFF0, 0x05F0, 0xE4E4, 0xE4FF, 0xFFDC, 0x0047, 0x0000, 0x0000, 0xFFF0, 0xFFF0,
    0xE4E4, 0xE4FF, 0x0024, 0x0047, 0x0000, 0x0000, 0x03F0, 0xFFF0, 0xE4E4, 0xE4FF, 0x0024, 0xFFFF, 0x0000, 0x0000,
    0x03F0, 0x03F0, 0xE4E4, 0xE4FF, 0xFFDC, 0xFFFF, 0x0000, 0x0000, 0xFFF0, 0x03F0, 0xE4E4, 0xE4FF, 0xFFDC, 0x0047,
    0x0000, 0x0000, 0xFFF0, 0xFFF0, 0xE4E4, 0xE4FF, 0x0024, 0x0047, 0x0000, 0x0000, 0x03F0, 0xFFF0, 0xE4E4, 0xE4FF,
    0x0024, 0xFFFF, 0x0000, 0x0000, 0x03F0, 0x03F0, 0xE4E4, 0xE4FF, 0xFFDC, 0xFFFF, 0x0000, 0x0000, 0xFFF0, 0x03F0,
    0xE4E4, 0xE4FF, 0xFF74, 0x008B, 0x0000, 0x0000, 0xFFF0, 0xFFF0, 0xE4E4, 0xE4FF, 0x008C, 0x008B, 0x0000, 0x0000,
    0x03F0, 0xFFF0, 0xE4E4, 0xE4FF, 0x008C, 0xFFFF, 0x0000, 0x0000, 0x03F0, 0x01F0, 0xE4E4, 0xE4FF, 0xFF74, 0xFFFF,
    0x0000, 0x0000, 0xFFF0, 0x01F0, 0xE4E4, 0xE4FF, 0xFFDC, 0x0047, 0x0000, 0x0000, 0xFFF0, 0xFFF0, 0xE4E4, 0xE4FF,
    0x0024, 0x0047, 0x0000, 0x0000, 0x01F0, 0xFFF0, 0xE4E4, 0xE4FF, 0x0024, 0xFFFF, 0x0000, 0x0000, 0x01F0, 0x01F0,
    0xE4E4, 0xE4FF, 0xFFDC, 0xFFFF, 0x0000, 0x0000, 0xFFF0, 0x01F0, 0xE4E4, 0xE4FF, 0xFFDC, 0x0047, 0x0000, 0x0000,
    0xFFF0, 0xFFF0, 0xE4E4, 0xE4FF, 0x0024, 0x0047, 0x0000, 0x0000, 0x01F0, 0xFFF0, 0xE4E4, 0xE4FF, 0x0024, 0xFFFF,
    0x0000, 0x0000, 0x01F0, 0x01F0, 0xE4E4, 0xE4FF, 0xFFDC, 0xFFFF, 0x0000, 0x0000, 0xFFF0, 0x01F0, 0xE4E4, 0xE4FF,
    0xFFDC, 0x0047, 0x0000, 0x0000, 0xFFF0, 0xFFF0, 0xE4E4, 0xE4FF, 0x0024, 0x0047, 0x0000, 0x0000, 0x03F0, 0xFFF0,
    0xE4E4, 0xE4FF, 0x0024, 0xFFFF, 0x0000, 0x0000, 0x03F0, 0x03F0, 0xE4E4, 0xE4FF, 0xFFDC, 0xFFFF, 0x0000, 0x0000,
    0xFFF0, 0x03F0, 0xE4E4, 0xE4FF, 0xFFDC, 0x0047, 0x0000, 0x0000, 0xFFF0, 0xFFF0, 0xE4E4, 0xE4FF, 0x0024, 0x0047,
    0x0000, 0x0000, 0x03F0, 0xFFF0, 0xE4E4, 0xE4FF, 0x0024, 0xFFFF, 0x0000, 0x0000, 0x03F0, 0x03F0, 0xE4E4, 0xE4FF,
    0xFFDC, 0xFFFF, 0x0000, 0x0000, 0xFFF0, 0x03F0, 0xE4E4, 0xE4FF, 0xFFD0, 0x0080, 0x0000, 0x0000, 0xFFF0, 0xFFF0,
    0xE4E4, 0xE4FF, 0x0030, 0x0080, 0x0000, 0x0000, 0x05F0, 0xFFF0, 0xE4E4, 0xE4FF, 0x0030, 0xFFFF, 0x0000, 0x0000,
    0x05F0, 0x07F0, 0xE4E4, 0xE4FF, 0xFFD0, 0xFFFF, 0x0000, 0x0000, 0xFFF0, 0x07F0, 0xE4E4, 0xE4FF, 0x0000, 0x0000,
    0xE3AE, 0x6EB2, 0xED09, 0x2847, 0xD2B3, 0x5DA0, 0x0100, 0x0000, 0xE6B6, 0xBF89, 0xEAAC, 0xA4AF, 0xCC8E, 0xF413,
    0x0100, 0x0000, 0xE384, 0x5545, 0xEA11, 0x24AF, 0xC7AE, 0xDA8F, 0x0100, 0x0000, 0xDE6A, 0x71B9, 0xE995, 0x84AF,
    0xCA26, 0xD9E5, 0x0000, 0x0000, 0xDEBF, 0x2944, 0xE2B1, 0xBE92, 0xB205, 0x0381, 0x0000, 0x0000, 0xDD3E, 0xD5C8,
    0xE2D8, 0x7E92, 0xB1E8, 0xADC7, 0x0000, 0x0000, 0xDA07, 0xE90C, 0xE1B2, 0x9E92, 0xAEB4, 0xC05D, 0x0100, 0x0000,
    0xDCC1, 0xC48E, 0xE24C, 0x5E92, 0xAF86, 0x7419, 0x0000, 0x0000, 0xDC4B, 0x4348, 0xDF57, 0xF901, 0xAA57, 0xE4E9,
    0x0000, 0x0000, 0xD802, 0xE3D8, 0xDDE0, 0xB901, 0xA310, 0x08D7, 0x0000, 0x0000, 0xD079, 0x4956, 0xDB87, 0xA587,
    0xA1FF, 0x7C2B, 0x0000, 0x0000, 0xCB20, 0x2456, 0xD99F, 0x4587, 0xA36B, 0x2AFB, 0x0000, 0x0000, 0xC774, 0x7C7C,
    0xD8C2, 0xC587, 0xA4FE, 0x7AB9, 0x0000, 0x0000, 0xC24F, 0x7297, 0xD731, 0x33AC, 0xAA41, 0xB882, 0x0000, 0x0000,
    0xC2C9, 0x044F, 0xD73C, 0xB3AC, 0xABD7, 0x9784, 0x0000, 0x0000, 0xC211, 0x616B, 0xD70C, 0x33AC, 0xAC9B, 0x9310,
    0x0000, 0x0000, 0xBD01, 0xA965, 0xD585, 0xB3AC, 0xAB68, 0x7008, 0x0000, 0x0000, 0xBC4F, 0x5A33, 0xD424, 0x73AC,
    0xA620, 0x714E, 0x0400, 0x0000, 0xB791, 0xB6C1, 0xD03C, 0x0F63, 0x9C7C, 0x0D14, 0x0500, 0x0000, 0xB70B, 0x2AE5,
    0xD03C, 0x0F63, 0x98EC, 0x66A0, 0x0400, 0x0000, 0xB1C6, 0x4329, 0xD03C, 0x0F63, 0x9BF7, 0x5604, 0x0300, 0x0000,
    0xBA1E, 0xAE31, 0xD03C, 0x0F63, 0x9B25, 0x9D2C, 0x0200, 0x0000, 0xE3A3, 0x59FD, 0xE5AF, 0x3F61, 0xBF2B, 0x213F,
    0xFF00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x032D, 0x6F99, 0xF97F, 0x6926,
    0xE4A5, 0xC892, 0x0000, 0x0000, 0x04D1, 0x669B, 0xFC27, 0xDD4E, 0xF0FA, 0xB893, 0x0200, 0x0000, 0xF7E4, 0xBE57,
    0xF4E3, 0x7F05, 0xD688, 0x7A09, 0x0100, 0x0000, 0xF52D, 0x87B9, 0xF4A4, 0xA1ED, 0xD7C9, 0xCCB7, 0xFF00, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};
extern u32 gAlphaSpriteRenderModeDl[];
extern s16 gMenuFadeAlpha;
extern s16 gFrameCounter;
extern u8 gCurrentViewportIndex;
extern u8 gConnectedControllerCount;
extern u32 D_2000000[];
extern u32 D_20006C8[];
extern u32 D_2000D78[];
extern u32 D_2000E48[];
extern u32 D_2000E70[];
extern u32 D_20058A8[];

void drawMenuAsciiTextDefaultScale(s32, s32, void *, s32);
void drawRaceSetupBackdropModels(MenuScreenEffectActor *);
void drawRaceSetupCourseBackdrop(void *);
void renderRaceSetupBackdrop(MenuScreenEffectActor *);
void stopRaceSetupCharacterFocus(MenuScreenEffectActor *);
void drawTitleScreenStartPrompt(MenuScreenEffectActor *);
void drawRaceStartPlayerEffectSprite(MenuScreenEffectActor *);
void updateRaceStartPlayerEffectActive(MenuScreenEffectActor *);
void waitForRaceStartPlayerEffect(MenuScreenEffectActor *);
void drawTitleMenuSparkle(MenuScreenEffectActor *);
void drawCourseRecordBanner(MenuScreenEffectActor *);
void updateCourseRecordBannerFadeOut(MenuScreenEffectActor *);
void drawRaceSetupNamePlate(MenuScreenEffectActor *);
void updateRaceSetupNamePlateSlideOut(MenuScreenEffectActor *);
void holdRaceSetupNamePlate(MenuScreenEffectActor *);
void updateRaceSetupNamePlateSlideIn(MenuScreenEffectActor *);
void drawMainMenuModeBoardTransition(MenuScreenEffectActor *);
void updateMainMenuModeBoardTransition(MenuScreenEffectActor *);
void updateMainMenuModeIconFlash(MenuScreenEffectActor *);
void drawMainMenuModeLabel(MenuScreenEffectActor *);
void updateMainMenuModeLabelFadeIn(MenuScreenEffectActor *);
void drawMainMenuRotatingBoardModel(MenuScreenEffectActor *);
void drawMainMenuStaticBoardModel(void *);

void drawFallingMenuSnowflake(MenuScreenEffectActor *arg0) {
    void *texture;

    if (gCurrentViewportIndex == arg0->index) {
        texture = getRelocatableHeapBlockBase(gAssetHandles[42]);
        drawMenuSprite(
            (s16)(arg0->unk18.half.hi >> 4),
            (s16)(arg0->unk18.half.lo >> 4),
            texture,
            arg0->unk1C.half.hi + 2,
            arg0->timer,
            arg0->timer,
            0,
            arg0->unk2A + 2
        );
    }
}

void updateFallingMenuSnowflakeDrift(MenuScreenEffectActor *arg0) {
    s16 temp_v0;
    s16 temp_v0_2;
    u32 var_v1;

    arg0->unk1C.half.hi = (arg0->unk1C.half.hi + 1) & 3;
    if (arg0->unk24.half.hi >= 0) {
        temp_v0 = arg0->unk24.half.lo;
        if (temp_v0 != 0) {
            arg0->unk20.half.hi += 4;
            arg0->unk24.half.lo = temp_v0 - 1;
        } else {
            arg0->unk20.half.hi -= 4;
            if (arg0->unk20.half.hi == 0) {
                arg0->unk24.half.lo = (randomNextMain() & 0x1F) + 0x10;
                arg0->unk24.half.hi = -1;
            }
        }
    } else {
        temp_v0_2 = arg0->unk24.half.lo;
        if (temp_v0_2 != 0) {
            arg0->unk20.half.hi -= 4;
            arg0->unk24.half.lo = temp_v0_2 - 1;
        } else {
            arg0->unk20.half.hi += 4;
            if (arg0->unk20.half.hi == 0) {
                arg0->unk24.half.lo = (randomNextMain() & 0x1F) + 0x10;
                arg0->unk24.half.hi = 1;
            }
        }
    }

    var_v1 = arg0->unk20.half.hi;
    arg0->unk18.half.hi += var_v1;
    arg0->unk18.half.lo += arg0->unk20.half.lo;
    if (arg0->unk18.half.lo >= 0x6E1) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawFallingMenuSnowflake, (void *)arg0);
}

void updateFallingMenuSnowflakeSway(MenuScreenEffectActor *arg0) {
    arg0->unk1C.half.hi = (arg0->unk1C.half.hi + 1) & 3;
    arg0->unk18.half.lo += arg0->unk20.half.lo;
    arg0->unk24.half.hi = (arg0->unk24.half.hi + 0x20) & 0xFFF;
    arg0->unk18.half.hi += (fixedSine(arg0->unk24.half.hi) * (arg0->unk1C.half.lo + 0x30)) / 4096;
    if ((arg0->unk24.half.hi == 0) || (arg0->unk24.half.hi == 0x800)) {
        arg0->unk1C.half.lo = randomNextMain() & 0xF;
    }
    if (arg0->unk18.half.lo >= 0x6E1) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawFallingMenuSnowflake, (void *)arg0);
}

void initFallingMenuSnowflake(MenuScreenEffectActor *arg0) {
    s32 temp_v1;
    s16 rand;

    temp_v1 = randomNextMain() & 1;
    switch (temp_v1) {
        case 0:
            arg0->unk18.half.hi = (randomNextMain() - 0x80) << 4;
            arg0->unk18.half.lo = -0x780;
            arg0->unk1C.half.lo = randomNextMain() & 0xF;
            rand = randomNextMain();
            arg0->unk24.half.hi = rand << 4;
            if (randomNextMain() & 1) {
                arg0->timer = 0x20;
                arg0->unk20.half.lo = (randomNextMain() & 7) + 0x40;
            } else {
                arg0->timer = 0x1B;
                arg0->unk20.half.lo = (randomNextMain() & 7) + 0x20;
            }
            arg0->unk2A = randomNextMain() % 5;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateFallingMenuSnowflakeSway);
            return;
        case 1:
            arg0->unk18.half.hi = (randomNextMain() - 0x80) << 4;
            arg0->unk18.half.lo = -0x780;
            arg0->unk1C.half.lo = (randomNextMain() & 0x1F) - 0xF;
            rand = randomNextMain();
            arg0->unk24.half.hi = rand << 4;
            if (randomNextMain() & 1) {
                arg0->timer = 0x20;
                arg0->unk20.half.lo = (randomNextMain() & 7) + 0x3C;
            } else {
                arg0->timer = 0x1B;
                arg0->unk20.half.lo = (randomNextMain() & 7) + 0x22;
            }
            arg0->unk2A = randomNextMain() % 5;
            arg0->unk24.half.hi = (randomNextMain() & 1) - 1;
            if (arg0->unk24.half.hi > 0) {
                arg0->unk20.half.hi = (randomNextMain() * 8) & 0xF;
            } else {
                arg0->unk20.half.hi = (randomNextMain() * -8) & 0xF;
            }
            arg0->unk24.half.lo = randomNextMain() & 0xF;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateFallingMenuSnowflakeDrift);
            return;
    }
}

void drawRaceSetupBackdropModels(MenuScreenEffectActor *arg0) {
    register MenuScreenEffectActor *actor = arg0;

    if (gCurrentViewportIndex == 0) {
        gDPPipeSync(gRegionAllocPtr++);

        gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[8]));
        gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[9]));

        gSPMatrix(gRegionAllocPtr++, &gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        if (actor->index == 0) {
            gSPDisplayList(gRegionAllocPtr++, D_2000000);
        }
        if (actor->index == 1) {
            gSPDisplayList(gRegionAllocPtr++, D_20006C8);
        }
        if (actor->index == 2) {
            gSPDisplayList(gRegionAllocPtr++, D_2000D78);
        }
    }
}

// drawRaceSetupCourseBackdrop best match: 99.677% at nonmatchings/drawRaceSetupCourseBackdrop-2/output-20-1/source.c
void drawRaceSetupCourseBackdrop(void *arg0) {
    if (gCurrentViewportIndex == 0) {
        gDPPipeSync(gRegionAllocPtr++);

        gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[14]));
        gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[18]));

        gSPMatrix(gRegionAllocPtr++, &gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        gSPDisplayList(gRegionAllocPtr++, D_20058A8);
    }
}

void renderRaceSetupBackdrop(MenuScreenEffectActor *arg0) {
    addRenderCallback(&gBackdropRenderCallbackList, (RenderCallback)drawRaceSetupCourseBackdrop, arg0);
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)drawRaceSetupBackdropModels, arg0);
}

void initRaceSetupBackdrop(MenuScreenEffectActor *arg0) {
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)renderRaceSetupBackdrop);
}

void stopRaceSetupCharacterFocus(MenuScreenEffectActor *arg0) {
    loopMainMenuSceneModelAnimation(0);
    addMainMenuSceneModelDrawCallbackForViewport0(0);
}

void updateRaceSetupCharacterFocus(MenuScreenEffectActor *arg0) {
    if (gMenuFadeAlpha == 0) {
        if (arg0->unk18.byte.b0 != 0) {
            enqueueSoundEffect(raceSetupCharacterFocusSoundIds[arg0->index], 0x32);
            arg0->unk18.byte.b0 = 0;
        }
        loopMainMenuSceneModelAnimation(0);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)stopRaceSetupCharacterFocus);
        addMainMenuSceneModelDrawCallbackForViewport0(0);
        return;
    }
    addMainMenuSceneModelDrawCallback(0);
}

void initRaceSetupCharacterFocus(MenuScreenEffectActor *arg0) {
    arg0->unk18.byte.b0 = 1;
    initMainMenuSceneModel(0, arg0->index);
    setMainMenuSceneModelAnimation(0, raceSetupCharacterFocusAnimationIds[arg0->index]);
    loopMainMenuSceneModelAnimation(0);
    setMainMenuSceneModelPosition(0, 0, 0, 0);
    setMainMenuSceneModelRotation(0, 0, 0, 0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSetupCharacterFocus);
}

void updateRaceSetupOpponentFocus(MenuScreenEffectActor *arg0) {
    s16 temp = arg0->unk1C.half.hi;

    if (temp == 0) {
        loopMainMenuSceneModelAnimation(arg0->unk18.word);
    } else {
        arg0->unk1C.half.hi = temp - 1;
    }
    addMainMenuSceneModelDrawCallbackForViewport(arg0->unk18.word, 1);
}

void initRaceSetupOpponentFocus(MenuScreenEffectActor *arg0) {
    s32 a = arg0->unk18.word;
    arg0->unk1C.half.hi = a * 0xF;
    initMainMenuSceneModel(a, arg0->index);
    setMainMenuSceneModelAnimation(arg0->unk18.word, raceSetupOpponentFocusAnimationIds[arg0->index]);
    loopMainMenuSceneModelAnimation(arg0->unk18.word);
    {
        s32 b = arg0->unk18.word;
        setMainMenuSceneModelPosition(b, raceSetupOpponentFocusXPositions[b], 0xFFB50000, 0xA00000);
    }
    setMainMenuSceneModelRotation(arg0->unk18.word, 0, 0x800, 0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSetupOpponentFocus);
}

void createRaceSetupOpponentFocus(s32 arg0, s32 arg1) {
    MenuScreenEffectActor *p =
        createCallbackTaskWithUserId((CallbackTaskCallback)initRaceSetupOpponentFocus, 0, 0x64, arg1);
    if (p != NULL) {
        p->unk18.word = arg0;
    }
}

void drawTitleScreenStartPrompt(MenuScreenEffectActor *arg0) {
    if (gConnectedControllerCount != 0) {
        drawMenuAsciiTextDefaultScale(-0x44, 0x38, "Push Start Button", 0);
        return;
    }
    drawMenuAsciiTextDefaultScale(-0x5C, 0x38, "No Control Pad detected", 1);
}

void updateTitleScreenStartPrompt(void *arg0) {
    if (gFrameCounter & 8) {
        addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawTitleScreenStartPrompt, 0);
    }
}

void drawRaceStartPlayerEffectSprite(MenuScreenEffectActor *arg0) {
    void *spA4;
    void *spA0;
    volatile u8 gap[0xC];
    Transform3D sp74;

    if (gRenderMatricesDirty != 0) {
        arg0->unk2F = 1;
    }

    if (arg0->unk2F != 0) {
        arg0->unk2F = 0;
        sp74 = gIdentityFixedTransform;
        sp74.translation.x = arg0->unk18.word;
        sp74.translation.y = arg0->unk1C.word;
        sp74.translation.z = arg0->unk20.word;
        arg0->unk24.word = (s32)allocFixedTransformMatrix(&sp74);
    }

    if (arg0->unk24.word != 0) {
        gSPDisplayList(gRegionAllocPtr++, gAlphaSpriteRenderModeDl);
        getAssetTableImageAndPalette(
            getRelocatableHeapBlockBase(gAssetHandles[30]),
            (u16)(arg0->unk2E + 0x24),
            &spA4,
            &spA0
        );
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xFD500000, (u32)spA4);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF5500000, 0x07080200);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF3000000, 0x070FF400);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF5400400, 0x80200);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF2000000, 0x7C07C);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)spA0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0x01020040, arg0->unk24.word);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0x01000040, (u32)gViewportMatrix);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)raceStartPlayerEffectVertices);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
    }
}

void updateRaceStartPlayerEffectActive(MenuScreenEffectActor *arg0) {
    s16 sp38[0x10];
    Vec3i sp2C;
    RacePlayer *player;
    u8 temp;

    player = &gRacePlayers[arg0->index];
    if (player->stateFlags & 0x2000) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitForRaceStartPlayerEffect);
        return;
    }

    temp = arg0->unk2E;
    if (temp != 5) {
        arg0->unk2E = temp + 1;
    } else if (player->itemTargetFlag == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)waitForRaceStartPlayerEffect);
    }

    sp2C.x = 0x40000;
    sp2C.y = 0x180000;
    sp2C.z = 0;

    player = &gRacePlayers[arg0->index];
    makeFixedRotationXY(sp38, player->pitchAngle, player->facingAngle);
    transformVec3iByFixedMatrix(sp38, &sp2C, (Vec3i *)&arg0->unk18);

    player = &gRacePlayers[arg0->index];
    arg0->unk18.word += player->unk28.x;
    arg0->unk1C.word += player->unk28.y;
    arg0->unk20.word += player->unk28.z;

    addRenderCallback(
        &gRaceModelEffectRenderCallbackList,
        (RenderCallback)drawRaceStartPlayerEffectSprite,
        (void *)arg0
    );
}

void waitForRaceStartPlayerEffect(MenuScreenEffectActor *arg0) {
    RacePlayer *player = &gRacePlayers[arg0->index];
    if (!(player->stateFlags & 0x2000)) {
        if (player->itemTargetFlag != 0) {
            arg0->unk2E = 0xFF;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceStartPlayerEffectActive);
        }
    }
}

void drawTitleMenuSparkle(MenuScreenEffectActor *arg0) {
    if ((gFrameCounter & 0x3E) >= 0x1F) {
        drawAssetTableSprite(
            arg0->unk18.half.hi,
            arg0->unk18.half.lo,
            getRelocatableHeapBlockBase(gAssetHandles[6]),
            1
        );
    } else {
        drawAssetTableSprite(
            arg0->unk18.half.hi,
            arg0->unk18.half.lo,
            getRelocatableHeapBlockBase(gAssetHandles[6]),
            titleMenuSparkleTileFrames[(gFrameCounter & 0x1E) >> 1]
        );
    }
}

void renderTitleMenuSparkle(MenuScreenEffectActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawTitleMenuSparkle, arg0);
}

void initTitleMenuSparkle(MenuScreenEffectActor *arg0) {
    s16 *temp = &titleMenuSparklePositions[arg0->index * 2];

    arg0->unk18.half.hi = temp[0];
    arg0->unk18.half.lo = temp[1];
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)renderTitleMenuSparkle);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void drawCourseRecordBanner(MenuScreenEffectActor *arg0) {
    Gfx *gfx;

    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    /* IDO scheduling for this function depends on this block staying on one line. */
    do { gfx->words.w0 = 0xE7000000; gfx->words.w1 = 0; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFC119623; gfx->words.w1 = 0xFF2FFFFF; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xB900031D; gfx->words.w1 = 0x00504240; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->unk18.half.lo & 0xFF) | (~0xFF); drawAssetTableSprite(-0x54, -0x10, getRelocatableHeapBlockBase(gAssetHandles[28]), 0x5D); drawAssetTableSprite(4, -0x10, getRelocatableHeapBlockBase(gAssetHandles[28]), 0x5E); if (arg0) { } drawAssetTableSprite(-0x4C, -0xC, getRelocatableHeapBlockBase(gAssetHandles[29]), (u16)courseRecordDigitTileOffsets[gRaceCourseIndex.signedValue]); drawAssetTableSprite(4, -0xC, getRelocatableHeapBlockBase(gAssetHandles[29]), courseRecordDigitTileOffsets[gRaceCourseIndex.signedValue] + 1); gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0x06000000; gfx->words.w1 = (u32) gMenuRenderModeResetDl; } while (0);
}
// clang-format on

void updateCourseRecordBannerFadeOut(MenuScreenEffectActor *arg0) {
    s16 v = arg0->unk18.half.hi;
    if (v != 0) {
        arg0->unk18.half.hi = v - 1;
    } else {
        arg0->unk18.half.lo -= 0x20;
        if (arg0->unk18.half.lo <= 0) {
            removeCallbackTask(arg0);
            return;
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawCourseRecordBanner, arg0);
}

void initCourseRecordBannerFadeOut(MenuScreenEffectActor *arg0) {
    arg0->unk18.half.hi = 0x3E;
    arg0->unk18.half.lo = 0xFF;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateCourseRecordBannerFadeOut);
}

void drawRaceSetupNamePlate(MenuScreenEffectActor *arg0) {
    u8 *new_var;

    new_var = &gCurrentViewportIndex;
    if (arg0->index == *new_var) {
        drawAssetTableSprite(
            (s16)((arg0->unk18.word >> 1) - 0x38),
            -8,
            getRelocatableHeapBlockBase(gAssetHandles[31]),
            0x76
        );
    }
}

void updateRaceSetupNamePlateSlideOut(MenuScreenEffectActor *arg0) {
    s32 temp_v0 = arg0->unk24.word;
    s32 temp_t8 = (unsigned long long)(temp_v0 + 4);

    arg0->unk18.word += temp_v0;
    arg0->unk24.word = temp_t8;
    if (temp_t8 == 0x38) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceSetupNamePlate, (void *)arg0);
    }
}

void holdRaceSetupNamePlate(MenuScreenEffectActor *arg0) {
    u16 temp = (arg0->timer & 0xFFFFU) - 1;

    arg0->timer = temp;
    temp += 0;
    if (temp == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSetupNamePlateSlideOut);
    }
    if (1) {}
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceSetupNamePlate, (void *)arg0);
}

void updateRaceSetupNamePlateSlideIn(MenuScreenEffectActor *arg0) {
    arg0->unk18.word += arg0->unk24.word;
    arg0->unk24.word -= 4;
    if (arg0->unk24.word == 0) {
        arg0->timer = 0x1E;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)holdRaceSetupNamePlate);
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceSetupNamePlate, (void *)arg0);
}

void waitForRaceSetupNamePlate(MenuScreenEffectActor *arg0) {
    if (gViewportStates[arg0->index].active != 0) {
        arg0->unk24.word = 0x38;
        arg0->unk18.word = -0x1A4;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSetupNamePlateSlideIn);
        return;
    }
    removeCallbackTask(arg0);
}

void drawMainMenuModeBoardTransition(MenuScreenEffectActor *arg0) {
    void *image;
    void *palette;
    Transform3D transform;
    Mtx *matrix;

    if (gCurrentViewportIndex == 2) {
        getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[34]), 0, &image, &palette);
        makeFixedRotationZ(transform.rotation, arg0->spriteIndex);
        transform.translation.x = arg0->unk18.word;
        transform.translation.y = arg0->unk1C.word;
        transform.translation.z = arg0->unk20.word;
        matrix = allocFixedTransformMatrix(&transform);
        if (matrix != NULL) {
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)gTranslucentSpriteRenderModeDl);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xFA000000, (arg0->alpha & 0xFF) | ~0xFF);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xFD48006B, (u32)image);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF5481C00, 0x07080200);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF4000000, 0x071B0040);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF5401C00, 0x00080200);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF2000000, 0x00360040);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)palette);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0x01020040, (u32)matrix);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0x01000040, (u32)gViewportMatrix);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)mainMenuModeBoardTransitionVertices);
            MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x00060200);
        }
    }
}

void updateMainMenuModeBoardAfterimage(MenuScreenEffectActor *arg0) {
    arg0->alpha -= 0x20;
    if (arg0->alpha == 0) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)drawMainMenuModeBoardTransition, (void *)arg0);
}

void updateMainMenuModeBoardTransition(MenuScreenEffectActor *arg0) {
    MenuScreenEffectActor *actor;

    arg0->unk1C.word = ((-0x400000LL * fixedSine((s16)(arg0->unk30.word + 0x400))) / 0x1000) + 0x700000;
    arg0->unk20.word = ((-0x9F0000LL * arg0->unk30.word) / 0x400) + 0xEC0000;
    arg0->spriteIndex = (0x5000LL * fixedSine(arg0->unk30.half.lo)) / 0x1000;
    if (arg0->unk30.word < 0x3F0) {
        actor = createCallbackTask((CallbackTaskCallback)updateMainMenuModeBoardAfterimage, 0, 0x65);
        *(OverlayActorTransform *)&actor->unk18 = *(OverlayActorTransform *)&arg0->unk18;
        actor->spriteIndex = arg0->spriteIndex;
        actor->alpha = 0xC0;
        arg0->unk30.word += 0x10;
    } else {
        createCallbackTask((CallbackTaskCallback)updateMainMenuModeIconFlash, 0, 0x63);
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)drawMainMenuModeBoardTransition, (void *)arg0);
}

void initMainMenuModeBoardTransition(MenuScreenEffectActor *arg0) {
    arg0->alpha = 0xFF;
    arg0->unk30.word = 0;
    arg0->unk18.word = 0;
    arg0->unk1C.word = 0x300000;
    arg0->unk20.word = 0xEC0000;
    arg0->spriteIndex = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateMainMenuModeBoardTransition);
}

void drawMainMenuModeIconFlash(MenuScreenEffectActor *arg0) {
    if (gCurrentViewportIndex == 2) {
        drawAssetTableSpriteWithExplicitPalette(
            -0x6C,
            -0x48,
            getRelocatableHeapBlockBase(gAssetHandles[34]),
            0,
            mainMenuModeIconFlashTileOffsets[arg0->unk18.half.lo]
        );
    }
}

void updateMainMenuModeIconFlash(MenuScreenEffectActor *arg0) {
    s16 v;
    arg0->unk18.half.hi += 1;
    v = arg0->unk18.half.hi;
    if (v >= 0x3D) {
        arg0->unk18.half.hi = 0;
        v = arg0->unk18.half.hi;
    }
    if (v < 0xA) {
        arg0->unk18.half.lo = v;
    } else {
        arg0->unk18.half.lo = 0;
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawMainMenuModeIconFlash, arg0);
}

void drawMainMenuModeLabel(MenuScreenEffectActor *arg0) {
    gDPPipeSync(gRegionAllocPtr++);
    gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->unk18.half.lo);

    switch (arg0->index) {
        case 0:
            drawAssetTableSprite(-0x8C, 0x38, getRelocatableHeapBlockBase(gAssetHandles[34]), 1);
            drawAssetTableSpriteWithExplicitPalette(
                -0x10,
                0x38,
                getRelocatableHeapBlockBase(gAssetHandles[34]),
                2,
                mainMenuModeLabelFlashTileOffsets[gFrameCounter & 3]
            );
            break;
        case 1:
            drawAssetTableSprite(-0x89, 0x38, getRelocatableHeapBlockBase(gAssetHandles[34]), 1);
            drawAssetTableSpriteWithExplicitPalette(
                -0xD,
                0x38,
                getRelocatableHeapBlockBase(gAssetHandles[34]),
                3,
                mainMenuModeLabelFlashTileOffsets[gFrameCounter & 3]
            );
            break;
        case 2:
            drawAssetTableSprite(-0x7E, 0x38, getRelocatableHeapBlockBase(gAssetHandles[34]), 1);
            drawAssetTableSpriteWithExplicitPalette(
                -2,
                0x38,
                getRelocatableHeapBlockBase(gAssetHandles[34]),
                4,
                mainMenuModeLabelFlashTileOffsets[gFrameCounter & 3]
            );
            break;
    }

    gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
}

void updateMainMenuModeLabelFadeIn(MenuScreenEffectActor *arg0) {
    s16 v = arg0->unk18.half.hi;
    if (v != 0) {
        arg0->unk18.half.hi = v - 1;
        return;
    }
    arg0->unk18.half.lo += 0x20;
    if (arg0->unk18.half.lo >= 0x100) {
        arg0->unk18.half.lo = 0xFF;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawMainMenuModeLabel, arg0);
}

void initMainMenuModeLabelFadeIn(MenuScreenEffectActor *arg0) {
    arg0->unk18.half.hi = 0x50;
    arg0->unk18.half.lo = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateMainMenuModeLabelFadeIn);
}

void drawMainMenuRotatingBoardModel(MenuScreenEffectActor *arg0) {
    TitleMenuRotatingBoardScratch scratch;
    Mtx *matrix;

    if (gCurrentViewportIndex == 0) {
        makeFixedRotationY(scratch.source.rotation, arg0->unk18.half.hi);
        scratch.source.translation.x = 0;
        scratch.source.translation.y = 0;
        scratch.source.translation.z = 0;
        matrix = allocFixedTransformMatrix(&scratch.source);
        if (matrix != NULL) {
            gDPPipeSync(gRegionAllocPtr++);

            gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[8]));
            gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[9]));

            gSPMatrix(gRegionAllocPtr++, matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            gSPDisplayList(gRegionAllocPtr++, D_2000E48);
        }
    }
}

void drawMainMenuStaticBoardModel(void *arg0) {
    if (gCurrentViewportIndex == 0) {
        gDPPipeSync(gRegionAllocPtr++);

        gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[8]));
        gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[9]));

        gSPMatrix(gRegionAllocPtr++, &gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        gSPDisplayList(gRegionAllocPtr++, D_2000E70);
    }
}

void renderMainMenuBoardModels(MenuScreenEffectActor *arg0) {
    addRenderCallback(&gBackdropRenderCallbackList, (RenderCallback)drawMainMenuStaticBoardModel, arg0);
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)drawMainMenuRotatingBoardModel, arg0);
}

void initMainMenuBoardModels(MenuScreenEffectActor *arg0) {
    arg0->unk18.half.hi = 0x360;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)renderMainMenuBoardModels);
}
