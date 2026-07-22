#include "game/engine/viewport_manager.h"

ViewportState gViewportStates[4];

extern f32 gRaceViewportOverlayFarClip;
extern void guPerspective(ViewportMtx *, u16 *, f32, f32, f32, f32, f32);

void resetAllViewports(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        gViewportStates[i].active = 0;
        gViewportStates[i].screenBoundsValid = 0;
        gViewportStates[i].clearFramebuffer = 0;
        gViewportStates[i].overlayActive = 0;
        gViewportStates[i].overlayR = 0;
        gViewportStates[i].overlayG = 0;
        gViewportStates[i].overlayB = 0;
        gViewportStates[i].overlayAlpha = 0;
        gViewportStates[i].unk14 = 0x1FF;
        gViewportStates[i].unk16 = 0;
        gViewportStates[i].unk1C = 0x1FF;
        gViewportStates[i].unk1E = 0;
    }
}

void resetViewport(s32 arg0) {
    ViewportState *viewport = &gViewportStates[arg0];

    viewport->active = 0;
    viewport->screenBoundsValid = 0;
    viewport->clearFramebuffer = 0;
    viewport->overlayActive = 0;
    viewport->overlayR = 0;
    viewport->overlayG = 0;
    viewport->overlayB = 0;
    viewport->overlayAlpha = 0;
    viewport->unk14 = 0x1FF;
    viewport->unk16 = 0;
    viewport->unk1C = 0x1FF;
    viewport->unk1E = 0;
}

// configureViewport best match: 97.944% (nonmatchings/configureViewport-6934502587000073416/base_2.c)

#pragma GLOBAL_ASM("asm/nonmatchings/engine/viewport_manager/configureViewport.s")

#ifdef NON_MATCHING
extern f32 gDefaultViewportOverlayFarClip;

void configureViewport(s32 viewportIndex, s32 centerX, s32 centerY, u16 width, u16 height, u16 scaleX, u16 scaleY,
                       f32 aspect) {
    gViewportStates[viewportIndex].viewportTranslateX = centerX * 4;
    gViewportStates[viewportIndex].active = 1;
    gViewportStates[viewportIndex].viewportTranslateY = centerY * 4;
    gViewportStates[viewportIndex].viewportScaleX = scaleX * 2;
    gViewportStates[viewportIndex].viewportScaleY = scaleY * 2;

    gViewportStates[viewportIndex].left = centerX - (width / 2);
    gViewportStates[viewportIndex].right = (width / 2) + centerX;
    gViewportStates[viewportIndex].top = centerY - (height / 2);
    gViewportStates[viewportIndex].bottom = (height / 2) + centerY;
    gViewportStates[viewportIndex].left = gViewportStates[viewportIndex].left;
    gViewportStates[viewportIndex].top = gViewportStates[viewportIndex].top;
    gViewportStates[viewportIndex].right = gViewportStates[viewportIndex].right;
    gViewportStates[viewportIndex].bottom = gViewportStates[viewportIndex].bottom;
    gViewportStates[viewportIndex].screenBoundsValid = 1;

    if (gViewportStates[viewportIndex].right < 0) {
        gViewportStates[viewportIndex].screenBoundsValid = 0;
    }
    if (gViewportStates[viewportIndex].bottom < 0) {
        gViewportStates[viewportIndex].screenBoundsValid = 0;
    }
    if (gViewportStates[viewportIndex].left >= 0x140) {
        gViewportStates[viewportIndex].screenBoundsValid = 0;
    }
    if (gViewportStates[viewportIndex].top >= 0xF0) {
        gViewportStates[viewportIndex].screenBoundsValid = 0;
    }
    if (gViewportStates[viewportIndex].left < 0) {
        gViewportStates[viewportIndex].left = 0;
    }
    if (gViewportStates[viewportIndex].top < 0) {
        gViewportStates[viewportIndex].top = 0;
    }
    if (gViewportStates[viewportIndex].right >= 0x140) {
        gViewportStates[viewportIndex].right = 0x13F;
    }
    if (gViewportStates[viewportIndex].bottom >= 0xF0) {
        gViewportStates[viewportIndex].bottom = 0xEF;
    }

    guPerspective(&gViewportStates[viewportIndex].projection, &gViewportStates[viewportIndex].perspectiveNorm, 70.0f,
                  aspect, 10.0f, 2800.0f, 0.5f);
    guPerspective(&gViewportStates[viewportIndex].overlayProjection,
                  &gViewportStates[viewportIndex].overlayPerspectiveNorm, 70.0f, aspect, 10.0f,
                  gDefaultViewportOverlayFarClip, 0.5f);
}
#endif

void configureViewportWithFovAndFarClip(s32 viewportIndex, s32 centerX, s32 centerY, u16 width, u16 height, u16 scaleX,
                                        u16 scaleY, f32 aspect, s16 fovY, s32 farClip) {
    gViewportStates[viewportIndex].active = 1;
    gViewportStates[viewportIndex].viewportTranslateX = centerX * 4;
    gViewportStates[viewportIndex].viewportTranslateY = centerY * 4;
    gViewportStates[viewportIndex].viewportScaleX = scaleX * 2;
    gViewportStates[viewportIndex].viewportScaleY = scaleY * 2;

    gViewportStates[viewportIndex].left = centerX - (width / 2);
    gViewportStates[viewportIndex].top = centerY - (height / 2);
    gViewportStates[viewportIndex].right = (width / 2) + centerX;
    gViewportStates[viewportIndex].bottom = (height / 2) + centerY;
    gViewportStates[viewportIndex].left = gViewportStates[viewportIndex].left;
    gViewportStates[viewportIndex].top = gViewportStates[viewportIndex].top;
    gViewportStates[viewportIndex].right = gViewportStates[viewportIndex].right;
    gViewportStates[viewportIndex].bottom = gViewportStates[viewportIndex].bottom;
    gViewportStates[viewportIndex].screenBoundsValid = 1;

    if (gViewportStates[viewportIndex].right < 0) {
        gViewportStates[viewportIndex].screenBoundsValid = 0;
    }
    if (gViewportStates[viewportIndex].bottom < 0) {
        gViewportStates[viewportIndex].screenBoundsValid = 0;
    }
    if (gViewportStates[viewportIndex].left >= 0x140) {
        gViewportStates[viewportIndex].screenBoundsValid = 0;
    }
    if (gViewportStates[viewportIndex].top >= 0xF0) {
        gViewportStates[viewportIndex].screenBoundsValid = 0;
    }
    if (gViewportStates[viewportIndex].left < 0) {
        gViewportStates[viewportIndex].left = 0;
    }
    if (gViewportStates[viewportIndex].top < 0) {
        gViewportStates[viewportIndex].top = 0;
    }
    if (gViewportStates[viewportIndex].right >= 0x140) {
        gViewportStates[viewportIndex].right = 0x13F;
    }
    if (gViewportStates[viewportIndex].bottom >= 0xF0) {
        gViewportStates[viewportIndex].bottom = 0xEF;
    }

    guPerspective(&gViewportStates[viewportIndex].projection, &gViewportStates[viewportIndex].perspectiveNorm, (f32)fovY,
                  aspect, 10.0f, (f32)farClip, 0.5f);
    guPerspective(&gViewportStates[viewportIndex].overlayProjection,
                  &gViewportStates[viewportIndex].overlayPerspectiveNorm, (f32)fovY, aspect, 10.0f, 15000.0f, 0.5f);
}

void configureRaceViewport(s32 viewportIndex, s32 centerX, s32 centerY, u16 width, u16 height, u16 scaleX, u16 scaleY,
                           f32 aspect) {
    gViewportStates[viewportIndex].active = 1;
    gViewportStates[viewportIndex].viewportTranslateX = centerX * 4;
    gViewportStates[viewportIndex].viewportTranslateY = centerY * 4;
    gViewportStates[viewportIndex].viewportScaleX = scaleX * 2;
    gViewportStates[viewportIndex].viewportScaleY = scaleY * 2;

    gViewportStates[viewportIndex].left = centerX - (width / 2);
    gViewportStates[viewportIndex].top = centerY - (height / 2);
    gViewportStates[viewportIndex].right = (width / 2) + centerX;
    gViewportStates[viewportIndex].bottom = (height / 2) + centerY;
    gViewportStates[viewportIndex].left = gViewportStates[viewportIndex].left;
    gViewportStates[viewportIndex].top = gViewportStates[viewportIndex].top;
    gViewportStates[viewportIndex].right = gViewportStates[viewportIndex].right;
    gViewportStates[viewportIndex].bottom = gViewportStates[viewportIndex].bottom;
    gViewportStates[viewportIndex].screenBoundsValid = 1;

    if (gViewportStates[viewportIndex].right < 0) {
        gViewportStates[viewportIndex].screenBoundsValid = 0;
    }
    if (gViewportStates[viewportIndex].bottom < 0) {
        gViewportStates[viewportIndex].screenBoundsValid = 0;
    }
    if (gViewportStates[viewportIndex].left >= 0x140) {
        gViewportStates[viewportIndex].screenBoundsValid = 0;
    }
    if (gViewportStates[viewportIndex].top >= 0xF0) {
        gViewportStates[viewportIndex].screenBoundsValid = 0;
    }
    if (gViewportStates[viewportIndex].left < 0) {
        gViewportStates[viewportIndex].left = 0;
    }
    if (gViewportStates[viewportIndex].top < 0) {
        gViewportStates[viewportIndex].top = 0;
    }
    if (gViewportStates[viewportIndex].right >= 0x140) {
        gViewportStates[viewportIndex].right = 0x13F;
    }
    if (gViewportStates[viewportIndex].bottom >= 0xF0) {
        gViewportStates[viewportIndex].bottom = 0xEF;
    }

    guPerspective(&gViewportStates[viewportIndex].projection, &gViewportStates[viewportIndex].perspectiveNorm, 70.0f,
                  aspect, 10.0f, 1000.0f, 0.5f);
    guPerspective(&gViewportStates[viewportIndex].overlayProjection,
                  &gViewportStates[viewportIndex].overlayPerspectiveNorm, 70.0f, aspect, 10.0f,
                  gRaceViewportOverlayFarClip, 0.5f);
}

// configureMenuViewport best match: 99.597% (nonmatchings/configureMenuViewport-6934502587000073416/base_17.c)

#pragma GLOBAL_ASM("asm/nonmatchings/engine/viewport_manager/configureMenuViewport.s")

#ifdef NON_MATCHING
extern f32 gMenuViewportFarClip;
extern f32 gMenuViewportOverlayFarClip;

void configureMenuViewport(s32 viewportIndex, s32 centerX, s32 centerY, u16 width, u16 height, u16 scaleX, u16 scaleY,
                           f32 aspect) {
    s32 halfHeight;

    gViewportStates[viewportIndex].viewportTranslateX = centerX * 4;
    gViewportStates[viewportIndex].active = 1;
    gViewportStates[viewportIndex].viewportTranslateY = centerY * 4;
    gViewportStates[viewportIndex].viewportScaleX = scaleX * 2;
    gViewportStates[viewportIndex].viewportScaleY = scaleY * 2;
    gViewportStates[viewportIndex].left = centerX - (width / 2);
    halfHeight = height / 2;
    gViewportStates[viewportIndex].right = (width / 2) + centerX;
    gViewportStates[viewportIndex].top = centerY - (height / 2);
    gViewportStates[viewportIndex].bottom = halfHeight + centerY;
    gViewportStates[viewportIndex].left = gViewportStates[viewportIndex].left;
    gViewportStates[viewportIndex].top = gViewportStates[viewportIndex].top;
    gViewportStates[viewportIndex].right = gViewportStates[viewportIndex].right;
    gViewportStates[viewportIndex].bottom = gViewportStates[viewportIndex].bottom;
    gViewportStates[viewportIndex].screenBoundsValid = 1;

    if (gViewportStates[viewportIndex].right < 0) {
        gViewportStates[viewportIndex].screenBoundsValid = 0;
    }
    if (gViewportStates[viewportIndex].bottom < 0) {
        gViewportStates[viewportIndex].screenBoundsValid = 0;
    }
    if (gViewportStates[viewportIndex].left >= 0x140) {
        gViewportStates[viewportIndex].screenBoundsValid = 0;
    }
    if (gViewportStates[viewportIndex].top >= 0xF0) {
        gViewportStates[viewportIndex].screenBoundsValid = 0;
    }
    if (gViewportStates[viewportIndex].left < 0) {
        gViewportStates[viewportIndex].left = 0;
    }
    if (gViewportStates[viewportIndex].top < 0) {
        halfHeight++;
        halfHeight--;
        gViewportStates[viewportIndex].top = 0;
    }
    if (gViewportStates[viewportIndex].right >= 0x140) {
        gViewportStates[viewportIndex].right = 0x13F;
    }
    if (gViewportStates[viewportIndex].bottom >= 0xF0) {
        gViewportStates[viewportIndex].bottom = 0xEF;
    }

    guPerspective(&gViewportStates[viewportIndex].projection, &gViewportStates[viewportIndex].perspectiveNorm, 70.0f,
                  aspect, 10.0f, gMenuViewportFarClip, 0.5f);
    guPerspective(&gViewportStates[viewportIndex].overlayProjection,
                  &gViewportStates[viewportIndex].overlayPerspectiveNorm, 70.0f, aspect, 10.0f,
                  gMenuViewportOverlayFarClip, 0.5f);
}
#endif

void setViewportOverlayColor(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    ViewportState *viewport = &gViewportStates[arg0];

    viewport->overlayActive = 1;
    viewport->overlayR = arg1;
    viewport->overlayG = arg2;
    viewport->overlayB = arg3;
}

void enableViewportClear(s32 arg0) {
    gViewportStates[arg0].clearFramebuffer = 1;
}
