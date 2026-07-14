#include "game/engine/viewport_manager.h"

// resetAllViewports best match: 53.082% (nonmatchings/resetAllViewports-3357475854818838508/base_7.c)

#pragma GLOBAL_ASM("asm/nonmatchings/engine/viewport_manager/resetAllViewports.s")

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

// configureViewportWithFovAndFarClip best match: 97.634% (nonmatchings/configureViewportWithFovAndFarClip-5512657642801906896/base_3.c)

#pragma GLOBAL_ASM("asm/nonmatchings/engine/viewport_manager/configureViewportWithFovAndFarClip.s")

#ifdef NON_MATCHING
extern void guPerspective(ViewportMtx *, u16 *, f32, f32, f32, f32, f32);
extern f32 gCustomViewportOverlayFarClip;

void configureViewportWithFovAndFarClip(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u16 arg6, f32 arg7,
                                        s16 arg8, s32 arg9) {
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

    fovy = (f32)arg8;
    guPerspective(&(&gViewportStates[arg0])->projection, &(&gViewportStates[arg0])->perspectiveNorm, fovy, arg7, 10.0f,
                  (f32)arg9, 0.5f);
    guPerspective(&(&gViewportStates[arg0])->overlayProjection, &(&gViewportStates[arg0])->overlayPerspectiveNorm, fovy, arg7,
                  10.0f, gCustomViewportOverlayFarClip, 0.5f);
}
#endif

// configureRaceViewport best match: 97.137% (nonmatchings/configureRaceViewport-2870645799593382959/base_10.c)

#pragma GLOBAL_ASM("asm/nonmatchings/engine/viewport_manager/configureRaceViewport.s")

#ifdef NON_MATCHING
extern f32 gRaceViewportOverlayFarClip;
extern void guPerspective(ViewportMtx *, u16 *, f32, f32, f32, f32, f32);

void configureRaceViewport(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u16 arg6, f32 arg7) {
    ViewportState *viewport;
    s64 boundsValid;

    viewport = &gViewportStates[arg0];
    viewport->viewportTranslateX = arg1 * 4;
    viewport->active = 1;
    viewport->viewportTranslateY = arg2 * 4;
    viewport->viewportScaleX = arg5 * 2;
    viewport->viewportScaleY = arg6 * 2;

    viewport->right = (arg3 / 2) + arg1;
    viewport->left = (0, arg1 - (arg3 / 2));
    viewport->top = arg2 - (arg4 / 2);
    viewport->bottom = (arg4 / 2) + arg2;
    boundsValid = 1;
    viewport->right = viewport->right;
    viewport->left = viewport->left;
    viewport->top = viewport->top;
    viewport->bottom = viewport->bottom;
    viewport->screenBoundsValid = boundsValid;

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

    guPerspective(&viewport->projection, &viewport->perspectiveNorm, 70.0f, arg7, 10.0f, 1000.0f, 0.5f);
    guPerspective(&viewport->overlayProjection, &viewport->overlayPerspectiveNorm, 70.0f, arg7, 10.0f,
                  gRaceViewportOverlayFarClip, 0.5f);
}
#endif

// configureMenuViewport best match: 90.258%

#pragma GLOBAL_ASM("asm/nonmatchings/engine/viewport_manager/configureMenuViewport.s")

#ifdef NON_MATCHING
extern f32 gMenuViewportFarClip;
extern f32 gMenuViewportOverlayFarClip;

void configureMenuViewport(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u16 arg6, f32 arg7) {
    ViewportState *viewport;

    viewport = &gViewportStates[arg0];
    viewport->viewportTranslateX = arg1 * 4;
    viewport->active = 1;
    viewport->viewportTranslateY = arg2 * 4;
    viewport->viewportScaleX = arg5 * 2;
    viewport->viewportScaleY = arg6 * 2;

    viewport->right = (arg3 / 2) + arg1;
    viewport->left = arg1 - (arg3 / 2);
    viewport->top = arg2 - (arg4 / 2);
    viewport->bottom = (arg4 / 2) + arg2;
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

    guPerspective(&viewport->projection, &viewport->perspectiveNorm, 70.0f, arg7, 10.0f, gMenuViewportFarClip, 0.5f);
    guPerspective(&viewport->overlayProjection, &viewport->overlayPerspectiveNorm, 70.0f, arg7, 10.0f, gMenuViewportOverlayFarClip,
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
