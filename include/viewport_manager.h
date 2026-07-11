#ifndef VIEWPORT_MANAGER_H
#define VIEWPORT_MANAGER_H

#include "common.h"

typedef union {
    s16 m[4][4][2];
    s64 force_structure_alignment;
} ViewportMtx;

typedef struct {
    /* 0x00 */ s8 active;
    /* 0x01 */ s8 screenBoundsValid;
    /* 0x02 */ s8 unk2;
    /* 0x03 */ s8 overlayActive;
    /* 0x04 */ s8 overlayR;
    /* 0x05 */ s8 overlayG;
    /* 0x06 */ s8 overlayB;
    /* 0x07 */ u8 pad7;
    /* 0x08 */ s16 overlayAlpha;
    /* 0x0A */ u8 padA[6];
    /* 0x10 */ s16 viewportScaleX;
    /* 0x12 */ s16 viewportScaleY;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 viewportTranslateX;
    /* 0x1A */ s16 viewportTranslateY;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ u16 perspectiveNorm;
    /* 0x22 */ u16 overlayPerspectiveNorm;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ ViewportMtx projection;
    /* 0x68 */ ViewportMtx overlayProjection;
    /* 0xA8 */ s16 left;
    /* 0xAA */ s16 top;
    /* 0xAC */ s16 right;
    /* 0xAE */ s16 bottom;
} ViewportState;

extern ViewportState D_801124B0[];
extern s16 D_801124B8;

void resetAllViewports(void);
void func_80070614(s32 arg0);
void configureViewport(s32 viewportIndex, s32 centerX, s32 centerY, u16 width, u16 height, u16 scaleX, u16 scaleY, f32 aspect);
void configureViewportWithFovAndFarClip(s32 viewportIndex, s32 centerX, s32 centerY, u16 width, u16 height, u16 scaleX,
                                        u16 scaleY, f32 aspect, s16 fovY, s32 farClip);
void func_80070A70(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u16 arg6, f32 arg7);
void configureMenuViewport(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u16 arg6, f32 arg7);
void func_80070E58(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_80070E90(s32 arg0);

#endif
