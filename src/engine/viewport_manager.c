#include "game/engine/viewport_manager.h"

ViewportState gViewportStates[4];

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

// configureViewport best match: 97.621% (nonmatchings/configureViewport-2870645799593382959/base_18.c)

#pragma GLOBAL_ASM("asm/nonmatchings/engine/viewport_manager/configureViewport.s")

#ifdef NON_MATCHING
extern f32 gDefaultViewportOverlayFarClip;
extern void guPerspective(ViewportMtx *, u16 *, f32, f32, f32, f32, f32);

void configureViewport(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u16 arg6, f32 arg7) {
    ViewportState *viewport;
    s32 halfHeight;
    s32 boundsInvalid;
    u32 boundsValid;
    f32 fovy;
    s32 halfWidth;

    boundsValid = (halfHeight = 1);
    (&gViewportStates[arg0])->viewportTranslateX = arg1 * 4;
    (&gViewportStates[arg0])->active = halfHeight;
    (&gViewportStates[arg0])->viewportTranslateY = arg2 * 4;
    (&gViewportStates[arg0])->viewportScaleX = arg5 * 2;
    (&gViewportStates[arg0])->viewportScaleY = arg6 * 2;
    halfWidth = arg3 / 2;
    (&gViewportStates[arg0])->left = arg1 - halfWidth;
    (&gViewportStates[arg0])->right = halfWidth + arg1;
    halfHeight = arg4 / 2;
    (&gViewportStates[arg0])->top = arg2 - halfHeight;
    (&gViewportStates[arg0])->bottom = halfHeight + arg2;
    (&gViewportStates[arg0])->left = (&gViewportStates[arg0])->left;
    (&gViewportStates[arg0])->top = (&gViewportStates[arg0])->top;
    (&gViewportStates[arg0])->right = (&gViewportStates[arg0])->right;
    (&gViewportStates[arg0])->bottom = (&gViewportStates[arg0])->bottom;
    (&gViewportStates[arg0])->screenBoundsValid = boundsValid;
    boundsInvalid = 0;

    if ((&gViewportStates[arg0])->right < 0) {
        (&gViewportStates[arg0])->screenBoundsValid = 0;
    }
    if ((&gViewportStates[arg0])->bottom < 0) {
        (&gViewportStates[arg0])->screenBoundsValid = boundsInvalid;
    }
    if ((&gViewportStates[arg0])->left >= 0x140) {
        (&gViewportStates[arg0])->screenBoundsValid = 0;
    }
    if ((&gViewportStates[arg0])->top >= 0xF0) {
        (&gViewportStates[arg0])->screenBoundsValid = 0;
    }
    if ((&gViewportStates[arg0])->left < 0) {
        (&gViewportStates[arg0])->left = 0;
    }
    if ((&gViewportStates[arg0])->top < 0) {
        (&gViewportStates[arg0])->top = 0;
    }
    if ((&gViewportStates[arg0])->right >= 0x140) {
        (&gViewportStates[arg0])->right = 0x13F;
    }
    if ((&gViewportStates[arg0])->bottom >= 0xF0) {
        (&gViewportStates[arg0])->bottom = 0xEF;
    }

    fovy = 70.0f;
    guPerspective(&(&gViewportStates[arg0])->projection, &(&gViewportStates[arg0])->perspectiveNorm, fovy, arg7, 10.0f,
                  2800.0f, 0.5f);
    guPerspective(&(&gViewportStates[arg0])->overlayProjection, &(&gViewportStates[arg0])->overlayPerspectiveNorm, fovy,
                  arg7, 10.0f, gDefaultViewportOverlayFarClip, 0.5f);
}
#endif

// configureViewportWithFovAndFarClip best match: 98.053%
// (nonmatchings/configureViewportWithFovAndFarClip-6887713755923057488/base_5.c)

#pragma GLOBAL_ASM("asm/nonmatchings/engine/viewport_manager/configureViewportWithFovAndFarClip.s")

#ifdef NON_MATCHING
extern void guPerspective(ViewportMtx *, u16 *, f32, f32, f32, f32, f32);

void configureViewportWithFovAndFarClip(s32 viewportIndex, s32 centerX, s32 centerY, u16 width, u16 height, u16 scaleX,
                                        u16 scaleY, f32 aspect, s16 fovY, s32 farClip) {
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

    guPerspective(&gViewportStates[viewportIndex].projection, &gViewportStates[viewportIndex].perspectiveNorm, (f32)fovY,
                  aspect, 10.0f, (f32)farClip, 0.5f);
    guPerspective(&gViewportStates[viewportIndex].overlayProjection,
                  &gViewportStates[viewportIndex].overlayPerspectiveNorm, (f32)fovY, aspect, 10.0f, 15000.0f, 0.5f);
}
#endif

// configureRaceViewport best match: 97.621% (nonmatchings/configureRaceViewport-5313856277864964686/base_10.c)

#pragma GLOBAL_ASM("asm/nonmatchings/engine/viewport_manager/configureRaceViewport.s")

#ifdef NON_MATCHING
extern f32 gRaceViewportOverlayFarClip;
extern void guPerspective(ViewportMtx *, u16 *, f32, f32, f32, f32, f32);

void configureRaceViewport(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u16 arg6, f32 arg7) {
    ViewportState *viewport;
    s32 halfHeight;
    s32 boundsInvalid;
    u32 boundsValid;
    f32 fovy;
    s32 halfWidth;

    boundsValid = (halfHeight = 1);
    (&gViewportStates[arg0])->viewportTranslateX = arg1 * 4;
    (&gViewportStates[arg0])->active = halfHeight;
    (&gViewportStates[arg0])->viewportTranslateY = arg2 * 4;
    (&gViewportStates[arg0])->viewportScaleX = arg5 * 2;
    (&gViewportStates[arg0])->viewportScaleY = arg6 * 2;

    halfWidth = arg3 / 2;
    (&gViewportStates[arg0])->left = arg1 - halfWidth;
    (&gViewportStates[arg0])->right = halfWidth + arg1;
    halfHeight = arg4 / 2;
    (&gViewportStates[arg0])->top = arg2 - halfHeight;
    (&gViewportStates[arg0])->bottom = halfHeight + arg2;
    (&gViewportStates[arg0])->left = (&gViewportStates[arg0])->left;
    (&gViewportStates[arg0])->top = (&gViewportStates[arg0])->top;
    (&gViewportStates[arg0])->right = (&gViewportStates[arg0])->right;
    (&gViewportStates[arg0])->bottom = (&gViewportStates[arg0])->bottom;
    (&gViewportStates[arg0])->screenBoundsValid = boundsValid;
    boundsInvalid = 0;

    if ((&gViewportStates[arg0])->right < 0) {
        (&gViewportStates[arg0])->screenBoundsValid = 0;
    }
    if ((&gViewportStates[arg0])->bottom < 0) {
        (&gViewportStates[arg0])->screenBoundsValid = boundsInvalid;
    }
    if ((&gViewportStates[arg0])->left >= 0x140) {
        (&gViewportStates[arg0])->screenBoundsValid = 0;
    }
    if ((&gViewportStates[arg0])->top >= 0xF0) {
        (&gViewportStates[arg0])->screenBoundsValid = 0;
    }
    if ((&gViewportStates[arg0])->left < 0) {
        (&gViewportStates[arg0])->left = 0;
    }
    if ((&gViewportStates[arg0])->top < 0) {
        (&gViewportStates[arg0])->top = 0;
    }
    if ((&gViewportStates[arg0])->right >= 0x140) {
        (&gViewportStates[arg0])->right = 0x13F;
    }
    if ((&gViewportStates[arg0])->bottom >= 0xF0) {
        (&gViewportStates[arg0])->bottom = 0xEF;
    }

    fovy = 70.0f;
    guPerspective(&(&gViewportStates[arg0])->projection, &(&gViewportStates[arg0])->perspectiveNorm, fovy, arg7, 10.0f,
                  1000.0f, 0.5f);
    guPerspective(&(&gViewportStates[arg0])->overlayProjection, &(&gViewportStates[arg0])->overlayPerspectiveNorm, fovy,
                  arg7, 10.0f,
                  gRaceViewportOverlayFarClip, 0.5f);
}
#endif

// configureMenuViewport best match: 97.920% (nonmatchings/configureMenuViewport-8075865578671233833/base_14.c)

#pragma GLOBAL_ASM("asm/nonmatchings/engine/viewport_manager/configureMenuViewport.s")

#ifdef NON_MATCHING
extern f32 gMenuViewportFarClip;
extern f32 gMenuViewportOverlayFarClip;

void configureMenuViewport(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u16 arg6, f32 arg7) {
    ViewportState *viewport;
    s32 halfHeight;
    s32 boundsInvalid;
    u32 boundsValid;
    f32 fovy;
    s32 halfWidth;

    boundsValid = (halfHeight = 1);
    (&gViewportStates[arg0])->viewportTranslateX = arg1 * 4;
    (&gViewportStates[arg0])->active = halfHeight;
    arg1++;
    arg1--;
    (&gViewportStates[arg0])->viewportTranslateY = arg2 * 4;
    (&gViewportStates[arg0])->viewportScaleX = arg5 * 2;
    (&gViewportStates[arg0])->viewportScaleY = arg6 * 2;

    halfWidth = arg3 / 2;
    (&gViewportStates[arg0])->left = arg1 - halfWidth;
    (&gViewportStates[arg0])->right = halfWidth + arg1;
    halfHeight = arg4 / 2;
    (&gViewportStates[arg0])->top = arg2 - halfHeight;
    (&gViewportStates[arg0])->bottom = halfHeight + arg2;
    (&gViewportStates[arg0])->right = (&gViewportStates[arg0])->right;
    (&gViewportStates[arg0])->screenBoundsValid = boundsValid;
    (&gViewportStates[arg0])->left = (&gViewportStates[arg0])->left;
    (&gViewportStates[arg0])->top = (&gViewportStates[arg0])->top;
    (&gViewportStates[arg0])->bottom = (&gViewportStates[arg0])->bottom;
    boundsInvalid = 0;
    (&gViewportStates[arg0])->screenBoundsValid = boundsValid;

    if ((&gViewportStates[arg0])->right < 0) {
        (&gViewportStates[arg0])->screenBoundsValid = 0;
    }
    if ((&gViewportStates[arg0])->bottom < 0) {
        (&gViewportStates[arg0])->screenBoundsValid = boundsInvalid;
    }
    if ((&gViewportStates[arg0])->left >= 0x140) {
        (&gViewportStates[arg0])->screenBoundsValid = 0;
    }
    if ((&gViewportStates[arg0])->top >= 0xF0) {
        (&gViewportStates[arg0])->screenBoundsValid = 0;
    }
    if ((&gViewportStates[arg0])->left < 0) {
        (&gViewportStates[arg0])->left = 0;
    }
    if ((&gViewportStates[arg0])->top < 0) {
        (&gViewportStates[arg0])->top = 0;
    }
    if ((&gViewportStates[arg0])->right >= 0x140) {
        (&gViewportStates[arg0])->right = 0x13F;
    }
    if ((&gViewportStates[arg0])->bottom >= 0xF0) {
        (&gViewportStates[arg0])->bottom = 0xEF;
    }

    fovy = 70.0f;
    guPerspective(&(&gViewportStates[arg0])->projection, &(&gViewportStates[arg0])->perspectiveNorm, fovy, arg7, 10.0f,
                  gMenuViewportFarClip, 0.5f);
    guPerspective(&(&gViewportStates[arg0])->overlayProjection, &(&gViewportStates[arg0])->overlayPerspectiveNorm, fovy,
                  arg7, 10.0f, gMenuViewportOverlayFarClip, 0.5f);
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
