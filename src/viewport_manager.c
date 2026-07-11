#include "viewport_manager.h"

// func_800704F0 best match: 50.342%

#pragma GLOBAL_ASM("asm/nonmatchings/viewport_manager/func_800704F0.s")

#ifdef NON_MATCHING
void func_800704F0(void) {
    volatile ViewportState *viewport = &D_801124B0[0];

    viewport->active = 0;
    viewport->screenBoundsValid = 0;
    viewport->unk2 = 0;
    viewport->overlayActive = 0;
    viewport->overlayR = 0;
    viewport->overlayG = 0;
    viewport->overlayB = 0;
    viewport->overlayAlpha = 0;
    viewport->unk14 = 0x1FF;
    viewport->unk16 = 0;
    viewport->unk1C = 0x1FF;
    viewport->unk1E = 0;

    viewport = &D_801124B0[1];
    viewport->active = 0;
    viewport->screenBoundsValid = 0;
    viewport->unk2 = 0;
    viewport->overlayActive = 0;
    viewport->overlayR = 0;
    viewport->overlayG = 0;
    viewport->overlayB = 0;
    viewport->overlayAlpha = 0;
    viewport->unk14 = 0x1FF;
    viewport->unk16 = 0;
    viewport->unk1C = 0x1FF;
    viewport->unk1E = 0;

    viewport = &D_801124B0[2];
    viewport->active = 0;
    viewport->screenBoundsValid = 0;
    viewport->unk2 = 0;
    viewport->overlayActive = 0;
    viewport->overlayR = 0;
    viewport->overlayG = 0;
    viewport->overlayB = 0;
    viewport->overlayAlpha = 0;
    viewport->unk14 = 0x1FF;
    viewport->unk16 = 0;
    viewport->unk1C = 0x1FF;
    viewport->unk1E = 0;

    viewport = &D_801124B0[3];
    viewport->active = 0;
    viewport->screenBoundsValid = 0;
    viewport->unk2 = 0;
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
#endif

void func_80070614(s32 arg0) {
    ViewportState *viewport = &D_801124B0[arg0];

    viewport->active = 0;
    viewport->screenBoundsValid = 0;
    viewport->unk2 = 0;
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

// func_8007066C best match: 88.911%

#pragma GLOBAL_ASM("asm/nonmatchings/viewport_manager/func_8007066C.s")

#ifdef NON_MATCHING
extern void guPerspective(ViewportMtx *, u16 *, f32, f32, f32, f32, f32);

void func_8007066C(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u16 arg6, f32 arg7) {
    ViewportState *viewport;
    s32 halfHeight;
    s32 halfWidth;
    f32 fovy;
    u16 *arg3Ptr;

    viewport = &D_801124B0[arg0];
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

// func_80070860 best match: 90.168%

#pragma GLOBAL_ASM("asm/nonmatchings/viewport_manager/func_80070860.s")

#ifdef NON_MATCHING
extern void guPerspective(ViewportMtx *, u16 *, f32, f32, f32, f32, f32);

void func_80070860(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u16 arg6, f32 arg7, s16 arg8, s32 arg9) {
    ViewportState *viewport;
    s32 halfHeight;
    s32 halfWidth;
    f32 fovy;

    viewport = &D_801124B0[arg0];
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

    fovy = (f32) arg8;
    guPerspective(&viewport->projection, &viewport->perspectiveNorm, fovy, arg7, 10.0f, (f32) arg9, 0.5f);
    guPerspective(&viewport->overlayProjection, &viewport->overlayPerspectiveNorm, fovy, arg7, 10.0f, 15000.0f, 0.5f);
}
#endif

// func_80070A70 best match: 88.992%

#pragma GLOBAL_ASM("asm/nonmatchings/viewport_manager/func_80070A70.s")

#ifdef NON_MATCHING
extern f32 D_800E1538;

void func_80070A70(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u16 arg6, f32 arg7) {
    ViewportState *viewport;
    s32 halfHeight;
    s32 halfWidth;
    f32 fovy;
    u16 *arg3Ptr;

    viewport = &D_801124B0[arg0];
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
    guPerspective(&viewport->overlayProjection, &viewport->overlayPerspectiveNorm, fovy, arg7, 10.0f, D_800E1538, 0.5f);
}
#endif

// func_80070C64 best match: 90.016%

#pragma GLOBAL_ASM("asm/nonmatchings/viewport_manager/func_80070C64.s")

#ifdef NON_MATCHING
extern f32 D_800E153C;
extern f32 D_800E1540;

void func_80070C64(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u16 arg6, f32 arg7) {
    ViewportState *viewport;
    ViewportState *savedViewport;
    s32 halfHeight;
    s32 halfWidth;

    viewport = &D_801124B0[arg0];
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
    guPerspective(&viewport->projection, &viewport->perspectiveNorm, 70.0f, arg7, 10.0f, D_800E153C, 0.5f);
    guPerspective(&savedViewport->overlayProjection, &savedViewport->overlayPerspectiveNorm, 70.0f, arg7, 10.0f, D_800E1540, 0.5f);
}
#endif

void func_80070E58(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    ViewportState *viewport = &D_801124B0[arg0];

    viewport->overlayActive = 1;
    viewport->overlayR = arg1;
    viewport->overlayG = arg2;
    viewport->overlayB = arg3;
}

void func_80070E90(s32 arg0) {
    D_801124B0[arg0].unk2 = 1;
}
