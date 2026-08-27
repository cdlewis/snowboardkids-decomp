#ifndef VIEWPORT_MANAGER_H
#define VIEWPORT_MANAGER_H

#include "common.h"
#include "game/engine/runtime_matrix.h"

typedef struct {
    /* 0x00 */ s8 active;
    /* 0x01 */ u8 screenBoundsValid;
    /* 0x02 */ u8 clearFramebuffer;
    /* 0x03 */ u8 overlayActive;
    /* 0x04 */ s8 overlayR;
    /* 0x05 */ s8 overlayG;
    /* 0x06 */ s8 overlayB;
    /* 0x07 */ u8 pad7;
    /* 0x08 */ s16 overlayAlpha;
    /* 0x0A */ u8 padA[6];
    /* 0x10 */ union {
        struct {
            /* 0x10 */ s16 viewportScaleX;
            /* 0x12 */ s16 viewportScaleY;
            /* 0x14 */ s16 viewportScaleZ;
            /* 0x16 */ s16 viewportScaleW;
            /* 0x18 */ s16 viewportTranslateX;
            /* 0x1A */ s16 viewportTranslateY;
            /* 0x1C */ s16 viewportTranslateZ;
            /* 0x1E */ s16 viewportTranslateW;
        };
        Vp viewport;
    };
    /* 0x20 */ u16 perspectiveNorm;
    /* 0x22 */ u16 overlayPerspectiveNorm;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ union {
        Mtx projectionMatrix;
        RuntimeMtx projection;
    };
    /* 0x68 */ union {
        Mtx overlayProjectionMatrix;
        RuntimeMtx overlayProjection;
    };
    /* 0xA8 */ s16 left;
    /* 0xAA */ s16 top;
    /* 0xAC */ s16 right;
    /* 0xAE */ s16 bottom;
} ViewportState;

typedef char ViewportStateSizeCheck[(sizeof(ViewportState) == 0xB0) ? 1 : -1];

extern ViewportState gViewportStates[];

void resetAllViewports(void);
void resetViewport(s32 arg0);
void configureViewport(
    s32 viewportIndex,
    s32 centerX,
    s32 centerY,
    u16 width,
    u16 height,
    u16 scaleX,
    u16 scaleY,
    f32 aspect
);
void configureViewportWithFovAndFarClip(
    s32 viewportIndex,
    s32 centerX,
    s32 centerY,
    u16 width,
    u16 height,
    u16 scaleX,
    u16 scaleY,
    f32 aspect,
    s16 fovY,
    s32 farClip
);
void configureRaceViewport(
    s32 viewportIndex,
    s32 centerX,
    s32 centerY,
    u16 width,
    u16 height,
    u16 scaleX,
    u16 scaleY,
    f32 aspect
);
void configureMenuViewport(
    s32 viewportIndex,
    s32 centerX,
    s32 centerY,
    u16 width,
    u16 height,
    u16 scaleX,
    u16 scaleY,
    f32 aspect
);
void setViewportOverlayColor(s32 viewportIndex, s32 r, s32 g, s32 b);
void enableViewportClear(s32 viewportIndex);

#endif
