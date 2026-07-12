#include "viewport_manager.h"

// resetAllViewports best match: 53.082% (nonmatchings/resetAllViewports-3357475854818838508/base_7.c)

#pragma GLOBAL_ASM("asm/nonmatchings/viewport_manager/resetAllViewports.s")

#ifdef NON_MATCHING
#define RESET_VIEWPORT_FIELD(index, field, value)     \
    do {                                              \
        volatile ViewportState *viewport;             \
        viewport = &gViewportStates[index];           \
        viewport->field = value;                      \
    } while (0)

void resetAllViewports(void) {
    RESET_VIEWPORT_FIELD(0, active, 0);
    RESET_VIEWPORT_FIELD(0, screenBoundsValid, 0);
    RESET_VIEWPORT_FIELD(0, clearFramebuffer, 0);
    RESET_VIEWPORT_FIELD(0, overlayActive, 0);
    RESET_VIEWPORT_FIELD(0, overlayR, 0);
    RESET_VIEWPORT_FIELD(0, overlayG, 0);
    RESET_VIEWPORT_FIELD(0, overlayB, 0);
    RESET_VIEWPORT_FIELD(0, overlayAlpha, 0);
    RESET_VIEWPORT_FIELD(0, unk14, 0x1FF);
    RESET_VIEWPORT_FIELD(0, unk16, 0);
    RESET_VIEWPORT_FIELD(0, unk1C, 0x1FF);
    RESET_VIEWPORT_FIELD(0, unk1E, 0);

    RESET_VIEWPORT_FIELD(1, active, 0);
    RESET_VIEWPORT_FIELD(1, screenBoundsValid, 0);
    RESET_VIEWPORT_FIELD(1, clearFramebuffer, 0);
    RESET_VIEWPORT_FIELD(1, overlayActive, 0);
    RESET_VIEWPORT_FIELD(1, overlayR, 0);
    RESET_VIEWPORT_FIELD(1, overlayG, 0);
    RESET_VIEWPORT_FIELD(1, overlayB, 0);
    RESET_VIEWPORT_FIELD(1, overlayAlpha, 0);
    RESET_VIEWPORT_FIELD(1, unk14, 0x1FF);
    RESET_VIEWPORT_FIELD(1, unk16, 0);
    RESET_VIEWPORT_FIELD(1, unk1C, 0x1FF);
    RESET_VIEWPORT_FIELD(1, unk1E, 0);

    RESET_VIEWPORT_FIELD(2, active, 0);
    RESET_VIEWPORT_FIELD(2, screenBoundsValid, 0);
    RESET_VIEWPORT_FIELD(2, clearFramebuffer, 0);
    RESET_VIEWPORT_FIELD(2, overlayActive, 0);
    RESET_VIEWPORT_FIELD(2, overlayR, 0);
    RESET_VIEWPORT_FIELD(2, overlayG, 0);
    RESET_VIEWPORT_FIELD(2, overlayB, 0);
    RESET_VIEWPORT_FIELD(2, overlayAlpha, 0);
    RESET_VIEWPORT_FIELD(2, unk14, 0x1FF);
    RESET_VIEWPORT_FIELD(2, unk16, 0);
    RESET_VIEWPORT_FIELD(2, unk1C, 0x1FF);
    RESET_VIEWPORT_FIELD(2, unk1E, 0);

    RESET_VIEWPORT_FIELD(3, active, 0);
    RESET_VIEWPORT_FIELD(3, screenBoundsValid, 0);
    RESET_VIEWPORT_FIELD(3, clearFramebuffer, 0);
    RESET_VIEWPORT_FIELD(3, overlayActive, 0);
    RESET_VIEWPORT_FIELD(3, overlayR, 0);
    RESET_VIEWPORT_FIELD(3, overlayG, 0);
    RESET_VIEWPORT_FIELD(3, overlayB, 0);
    RESET_VIEWPORT_FIELD(3, overlayAlpha, 0);
    RESET_VIEWPORT_FIELD(3, unk14, 0x1FF);
    RESET_VIEWPORT_FIELD(3, unk16, 0);
    RESET_VIEWPORT_FIELD(3, unk1C, 0x1FF);
    RESET_VIEWPORT_FIELD(3, unk1E, 0);
}

#undef RESET_VIEWPORT_FIELD
#endif

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

// configureViewport best match: 88.911%

#pragma GLOBAL_ASM("asm/nonmatchings/viewport_manager/configureViewport.s")

#ifdef NON_MATCHING
extern void guPerspective(ViewportMtx *, u16 *, f32, f32, f32, f32, f32);

void configureViewport(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u16 arg6, f32 arg7) {
    ViewportState *viewport;
    s32 halfHeight;
    s32 halfWidth;
    f32 fovy;
    u16 *arg3Ptr;

    viewport = &gViewportStates[arg0];
    viewport->viewportTranslateX = arg1 * 4;
    viewport->active = 1;
    viewport->viewportTranslateY = arg2 * 4;
    viewport->viewportScaleX = arg5 * 2;
    viewport->viewportScaleY = arg6 * 2;

    arg3Ptr = &arg3;
    arg3 = *arg3Ptr & 0xFFFF;
    halfWidth = arg3 / 2;
    viewport->right = halfWidth + arg1;
    viewport->left = arg1 - halfWidth;
    halfHeight = arg4 / 2;
    viewport->top = arg2 - halfHeight;
    viewport->bottom = halfHeight + arg2;
    viewport->right = viewport->right;
    viewport->screenBoundsValid = 1;
    viewport->left = viewport->left;
    viewport->top = viewport->top;
    viewport->bottom = viewport->bottom;

    if (viewport->right < 0) {
        viewport->screenBoundsValid = 0;
    }
    if (viewport->bottom < 0) {
        viewport->screenBoundsValid = 0;
    }
    if (viewport->left >= 0x140) {
        viewport->screenBoundsValid = 0;
    }
    if (viewport->top >= 0xF0) {
        viewport->screenBoundsValid = 0;
    }
    if (viewport->left < 0) {
        viewport->left = 0;
    }
    if (viewport->top < 0) {
        viewport->top = 0;
    }
    if (viewport->right >= 0x140) {
        viewport->right = 0x13F;
    }
    if (viewport->bottom >= 0xF0) {
        viewport->bottom = 0xEF;
    }

    fovy = 70.0f;
    guPerspective(&viewport->projection, &viewport->perspectiveNorm, fovy, arg7, 10.0f, 2800.0f, 0.5f);
    guPerspective(&viewport->overlayProjection, &viewport->overlayPerspectiveNorm, fovy, arg7, 10.0f, 15000.0f, 0.5f);
}
#endif

// configureViewportWithFovAndFarClip best match: 90.168%

#pragma GLOBAL_ASM("asm/nonmatchings/viewport_manager/configureViewportWithFovAndFarClip.s")

#ifdef NON_MATCHING
extern void guPerspective(ViewportMtx *, u16 *, f32, f32, f32, f32, f32);

void configureViewportWithFovAndFarClip(s32 viewportIndex, s32 centerX, s32 centerY, u16 width, u16 height, u16 scaleX, u16 scaleY,
                                        f32 aspect, s16 fovY, s32 farClip) {
    ViewportState *viewport;
    s32 halfHeight;
    s32 halfWidth;
    f32 fovy;

    viewport = &gViewportStates[viewportIndex];
    viewport->viewportTranslateX = centerX * 4;
    viewport->active = 1;
    viewport->viewportTranslateY = centerY * 4;
    viewport->viewportScaleX = scaleX * 2;
    viewport->viewportScaleY = scaleY * 2;

    halfWidth = width / 2;
    viewport->right = halfWidth + centerX;
    viewport->left = centerX - halfWidth;
    halfHeight = height / 2;
    viewport->top = centerY - halfHeight;
    viewport->bottom = halfHeight + centerY;
    viewport->right = viewport->right;
    viewport->screenBoundsValid = 1;
    viewport->left = viewport->left;
    viewport->top = viewport->top;
    viewport->bottom = viewport->bottom;

    if (viewport->right < 0) {
        viewport->screenBoundsValid = 0;
    }
    if (viewport->bottom < 0) {
        viewport->screenBoundsValid = 0;
    }
    if (viewport->left >= 0x140) {
        viewport->screenBoundsValid = 0;
    }
    if (viewport->top >= 0xF0) {
        viewport->screenBoundsValid = 0;
    }
    if (viewport->left < 0) {
        viewport->left = 0;
    }
    if (viewport->top < 0) {
        viewport->top = 0;
    }
    if (viewport->right >= 0x140) {
        viewport->right = 0x13F;
    }
    if (viewport->bottom >= 0xF0) {
        viewport->bottom = 0xEF;
    }

    fovy = (f32) fovY;
    guPerspective(&viewport->projection, &viewport->perspectiveNorm, fovy, aspect, 10.0f, (f32) farClip, 0.5f);
    guPerspective(&viewport->overlayProjection, &viewport->overlayPerspectiveNorm, fovy, aspect, 10.0f, 15000.0f, 0.5f);
}
#endif

// configureRaceViewport best match: 88.992%

#pragma GLOBAL_ASM("asm/nonmatchings/viewport_manager/configureRaceViewport.s")

#ifdef NON_MATCHING
extern f32 gRaceViewportOverlayFarClip;

void configureRaceViewport(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u16 arg6, f32 arg7) {
    ViewportState *viewport;
    s32 halfHeight;
    s32 halfWidth;
    f32 fovy;
    u16 *arg3Ptr;

    viewport = &gViewportStates[arg0];
    viewport->viewportTranslateX = arg1 * 4;
    viewport->active = 1;
    viewport->viewportTranslateY = arg2 * 4;
    viewport->viewportScaleX = arg5 * 2;
    viewport->viewportScaleY = arg6 * 2;

    arg3Ptr = &arg3;
    arg3 = *arg3Ptr & 0xFFFF;
    halfWidth = arg3 / 2;
    viewport->right = halfWidth + arg1;
    viewport->left = arg1 - halfWidth;
    halfHeight = arg4 / 2;
    viewport->top = arg2 - halfHeight;
    viewport->bottom = halfHeight + arg2;
    viewport->right = viewport->right;
    viewport->screenBoundsValid = 1;
    viewport->left = viewport->left;
    viewport->top = viewport->top;
    viewport->bottom = viewport->bottom;

    if (viewport->right < 0) {
        viewport->screenBoundsValid = 0;
    }
    if (viewport->bottom < 0) {
        viewport->screenBoundsValid = 0;
    }
    if (viewport->left >= 0x140) {
        viewport->screenBoundsValid = 0;
    }
    if (viewport->top >= 0xF0) {
        viewport->screenBoundsValid = 0;
    }
    if (viewport->left < 0) {
        viewport->left = 0;
    }
    if (viewport->top < 0) {
        viewport->top = 0;
    }
    if (viewport->right >= 0x140) {
        viewport->right = 0x13F;
    }
    if (viewport->bottom >= 0xF0) {
        viewport->bottom = 0xEF;
    }

    fovy = 70.0f;
    guPerspective(&viewport->projection, &viewport->perspectiveNorm, fovy, arg7, 10.0f, 1000.0f, 0.5f);
    guPerspective(&viewport->overlayProjection, &viewport->overlayPerspectiveNorm, fovy, arg7, 10.0f, gRaceViewportOverlayFarClip, 0.5f);
}
#endif

// configureMenuViewport best match: 90.016%

#pragma GLOBAL_ASM("asm/nonmatchings/viewport_manager/configureMenuViewport.s")

#ifdef NON_MATCHING
extern f32 gMenuViewportFarClip;
extern f32 gMenuViewportOverlayFarClip;

void configureMenuViewport(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u16 arg6, f32 arg7) {
    ViewportState *viewport;
    ViewportState *savedViewport;
    s32 halfHeight;
    s32 halfWidth;

    viewport = &gViewportStates[arg0];
    viewport->viewportTranslateX = arg1 * 4;
    viewport->active = 1;
    viewport->viewportTranslateY = arg2 * 4;
    viewport->viewportScaleX = arg5 * 2;
    viewport->viewportScaleY = arg6 * 2;

    halfWidth = arg3 / 2;
    viewport->right = halfWidth + arg1;
    viewport->left = arg1 - halfWidth;
    halfHeight = arg4 / 2;
    viewport->top = arg2 - halfHeight;
    viewport->bottom = halfHeight + arg2;
    viewport->right = viewport->right;
    viewport->screenBoundsValid = 1;
    viewport->left = viewport->left;
    viewport->top = viewport->top;
    viewport->bottom = viewport->bottom;

    if (viewport->right < 0) {
        viewport->screenBoundsValid = 0;
    }
    if (viewport->bottom < 0) {
        viewport->screenBoundsValid = 0;
    }
    if (viewport->left >= 0x140) {
        viewport->screenBoundsValid = 0;
    }
    if (viewport->top >= 0xF0) {
        viewport->screenBoundsValid = 0;
    }
    if (viewport->left < 0) {
        viewport->left = 0;
    }
    if (viewport->top < 0) {
        viewport->top = 0;
    }
    if (viewport->right >= 0x140) {
        viewport->right = 0x13F;
    }
    if (viewport->bottom >= 0xF0) {
        viewport->bottom = 0xEF;
    }

    savedViewport = viewport;
    guPerspective(&viewport->projection, &viewport->perspectiveNorm, 70.0f, arg7, 10.0f, gMenuViewportFarClip, 0.5f);
    guPerspective(&savedViewport->overlayProjection, &savedViewport->overlayPerspectiveNorm, 70.0f, arg7, 10.0f, gMenuViewportOverlayFarClip,
                  0.5f);
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
