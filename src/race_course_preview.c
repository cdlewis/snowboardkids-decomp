#include "common.h"
#include "memory_allocator.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "race_course_preview.h"
#include "fixed_point_math.h"
#include "fixed_point_matrix.h"

#define RACE_COURSE_PREVIEW_GFX_CMD(pkt, cmd0, cmd1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (cmd0); \
    _g->words.w1 = (cmd1); \
}

typedef s16 FixedMatrix3sScratch[0x10];

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
} GfxCommandDest;

typedef struct {
    /* 0x00 */ s8 textureIndex;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ s32 command[3];
} RaceCoursePreviewRenderCommandEntry;

struct RaceCoursePreviewMeshActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ GfxCommandDest *matrices;
};

typedef struct RaceCoursePreviewCamera RaceCoursePreviewCamera;
typedef void (*RaceCoursePreviewCameraCallback)(RaceCoursePreviewCamera *);

typedef union {
    s32 word;
    struct {
        s16 yaw;
        s16 pitch;
    } half;
} PackedAngles;

struct RaceCoursePreviewCamera {
    /* 0x00 */ char pad0[0x8];
    /* 0x08 */ RaceCoursePreviewCameraCallback callback;
    /* 0x0C */ char padC[0x4];
    /* 0x10 */ u16 index;
    /* 0x12 */ char pad12[0x6];
    /* 0x18 */ Vec3i position;
    /* 0x24 */ s32 velocityY;
    /* 0x28 */ s32 radius;
    /* 0x2C */ PackedAngles angle;
    union {
        struct {
            /* 0x30 */ s16 scale;
            /* 0x32 */ s16 tilt;
            /* 0x34 */ s16 pitchVelocity;
            /* 0x36 */ s16 spinVelocity;
        };
        struct {
            /* 0x30 */ u32 image;
            /* 0x34 */ u32 palette;
        };
    };
    /* 0x38 */ s16 timer;
    /* 0x3A */ s16 stateTimer;
    /* 0x3C */ Gfx *displayList0;
    union {
        /* 0x40 */ Gfx *displayList1;
        /* 0x40 */ s8 displayList0Valid;
    };
    /* 0x44 */ s8 displayListValid;
};

typedef struct {
    /* 0x00 */ u8 pad0[0x4E];
    /* 0x4E */ s16 matrixHandle;
    /* 0x50 */ s16 matrixHandle2;
} RaceCoursePreviewAssetHandles;

extern void *D_801248D4;
extern void func_800483FC(void *, void *, s32);
extern void osWritebackDCache(void *, s32);
extern s16 D_80112144;
extern s16 D_80112146;
extern s16 D_80112168;
extern s16 D_8011216A;
extern RaceCoursePreviewAssetHandles D_80112130;
extern u8 D_80156609;
extern Gfx *gRegionAllocPtr;
extern Gfx D_20028F0[];
extern Gfx D_2002DB8[];
extern Gfx *func_8004885C(FixedTransform *arg0);
extern void func_80045990(s32 arg0, s32 arg1, s16 *arg2, s16 *arg3);
extern Vec3i gRaceCoursePreviewBillboardPositions[];
extern GfxCommandDest *gRaceCoursePreviewModelVerticesByCourse[];
extern RaceCoursePreviewRenderCommandEntry *gRaceCoursePreviewModelCommandsByCourse[];
extern RaceCoursePreviewRenderCommandEntry *gRaceCoursePreviewAnimatedBillboardCommandsByCourse[];
extern u16 gRaceCoursePreviewAnimatedBillboardTextureIds[];
extern Gfx gRaceCoursePreviewAnimatedBillboardVertices[];
extern Gfx gRaceCoursePreviewBillboardVertices[];
extern Gfx gEffectRenderModeSetupDl[];
extern Gfx gEffectRenderModeCleanupDl[];
extern GfxCommandDest gIdentityMatrix;
extern FixedTransform gIdentityFixedTransform;
extern void func_80048C90(GfxCommandDest *, s32 *);
extern void func_80045A1C(u8 *, s32, u32 *, u32 *, s16 *, s16 *);

extern s8 D_80122288;
extern s16 D_80122282;
extern s16 D_80121B50;
extern s16 gFrameCounter;
extern u32 D_80156614;

// drawRaceCoursePreviewModelMeshes best match: 99.531% (nonmatchings/drawRaceCoursePreviewModelMeshes-6061209858023118177/base_12.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_course_preview/drawRaceCoursePreviewModelMeshes.s")

#ifdef NON_MATCHING
void drawRaceCoursePreviewModelMeshes(RaceCoursePreviewMeshActor *arg0) {
    volatile u8 pad[0xC];
    u32 image;
    u32 palette;
    s16 width;
    s16 height;
    RaceCoursePreviewRenderCommandEntry *entry;
    GfxCommandDest *vertices;
    Gfx *gfx;
    s16 textureIndex;
    s32 i;
    s8 nextTextureIndex;

    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
    entry = gRaceCoursePreviewModelCommandsByCourse[D_80121B50];
    vertices = gRaceCoursePreviewModelVerticesByCourse[D_80121B50];
    textureIndex = -1;
    i = 0;

    if (entry->textureIndex != -1) {
        do {
            if (func_80049000(entry->command) != 0) {
                if (textureIndex != entry->textureIndex) {
                    textureIndex = entry->textureIndex;
                    func_80045A1C((u8 *)func_80043040((s32)D_8011216A), (u16)textureIndex, &image, &palette,
                                  &width, &height);
                    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, width, height, 0, G_TX_CLAMP,
                                            G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
                    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
                }

                gSPMatrix(gRegionAllocPtr++, &arg0->matrices[i], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, D_80156614, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gfx = gRegionAllocPtr++;
                gfx->words.w0 = 0x0400103F;
                gfx->words.w1 = (u32)&vertices[entry->textureIndex];
                gfx = gRegionAllocPtr++;
                gfx->words.w0 = 0xB1060402;
                gfx->words.w1 = 0x60200;
            }
            nextTextureIndex = entry[1].textureIndex;
            entry++;
            i++;
        } while (nextTextureIndex != -1);
    }

    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
}
#endif

void enqueueDrawRaceCoursePreviewModelMeshes(s32 arg0) {
    func_800483FC(&D_801248D4, drawRaceCoursePreviewModelMeshes, arg0);
}

void initRaceCoursePreviewModelMeshes(RaceCoursePreviewMeshActor *arg0) {
    RaceCoursePreviewRenderCommandEntry *entry;
    s32 count;
    s32 allocSize;
    s32 i;

    entry = gRaceCoursePreviewModelCommandsByCourse[D_80121B50];
    count = 0;
    if (entry->textureIndex != -1) {
        do {
            count++;
            entry++;
        } while (entry->textureIndex != -1);
    }

    if (count != 0) {
        entry = gRaceCoursePreviewModelCommandsByCourse[D_80121B50];
        allocSize = count * sizeof(GfxCommandDest);
        D_80112130.matrixHandle = func_80042D58(allocSize);
        arg0->matrices = func_80043040(D_80112130.matrixHandle);

        i = 0;
        if (count > 0) {
            do {
                arg0->matrices[i] = gIdentityMatrix;
                func_80048C90(&arg0->matrices[i], entry->command);
                i++;
                entry++;
            } while (i != count);
        }
        osWritebackDCache(arg0->matrices, allocSize);
    }

    setCallbackTaskCallback(arg0, enqueueDrawRaceCoursePreviewModelMeshes);
}

void drawRaceCoursePreviewBillboard(RaceCoursePreviewCamera *arg0) {
    FixedTransform sp70;

    if (D_80156609 != 0) {
        arg0->displayList0Valid = 1;
    }

    if (func_80049000(&arg0->position) != 0) {
        if (arg0->displayList0Valid != 0) {
            sp70 = gIdentityFixedTransform;
            sp70.translation.x = arg0->position.x;
            sp70.translation.y = arg0->position.y;
            sp70.translation.z = arg0->position.z;
            arg0->displayList0 = func_8004885C(&sp70);
        }

        if (arg0->displayList0 != NULL) {
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)gEffectRenderModeSetupDl);
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0xFD500000, arg0->image);
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0xF5500000, 0x07080200);
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0xF3000000, 0x070FF400);
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0xF5400400, 0x00080200);
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0xF2000000, 0x0007C07C);
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0xFD100000, arg0->palette);
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0x01020040, (u32)arg0->displayList0);
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0x01000040, D_80156614);
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)gRaceCoursePreviewBillboardVertices);
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x00060200);
            RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)gEffectRenderModeCleanupDl);
        }
    }
}

void updateRaceCoursePreviewBillboard(RaceCoursePreviewCamera *arg0) {
    Vec3i sp44;
    FixedMatrix3sScratch sp24;
    RaceCoursePreviewCamera *temp_s0 = arg0;

    arg0->timer--;
    if (arg0->timer == 0) {
        arg0->timer = randomNextMain() + 0x1E;
        setCallbackTaskCallback(arg0, waitRaceCoursePreviewBillboardSpawn);
    }
    func_80097C18(sp24, 0x6D0);
    func_80098590(sp24, (Vec3i *)&temp_s0->velocityY, &sp44);
    temp_s0->position.x += sp44.x;
    temp_s0->position.y += sp44.y;
    temp_s0->position.z += sp44.z;
    temp_s0->radius -= 0x4000;
    func_800483FC(&D_801248D4, drawRaceCoursePreviewBillboard, (s32) temp_s0);
}

void waitRaceCoursePreviewBillboardSpawn(RaceCoursePreviewCamera *arg0) {
    Vec3i *temp_t3;

    arg0->timer--;
    if (!arg0->timer) {
        arg0->timer = 0x5A;
        temp_t3 = &gRaceCoursePreviewBillboardPositions[arg0->index];
        arg0->position = *(&gRaceCoursePreviewBillboardPositions[arg0->index]);
        arg0->velocityY = 0;
        arg0->radius = 0;
        arg0->angle.word = 0xFFF00000;
        setCallbackTaskCallback(arg0, updateRaceCoursePreviewBillboard);
    }
}

void initRaceCoursePreviewBillboard(RaceCoursePreviewCamera *arg0) {
    arg0->timer = (arg0->index * 0x1E) + 0x1E;
    func_80045990(func_80043040(D_8011216A), (arg0->index + 3) & 0xFFFF, &arg0->scale, &arg0->pitchVelocity);
    setCallbackTaskCallback(arg0, waitRaceCoursePreviewBillboardSpawn);
}

void drawRaceCoursePreviewCameraModel(RaceCoursePreviewCamera *arg0) {
    volatile s32 pad0[1];
    FixedTransform sp84;
    FixedTransform sp64;
    FixedTransform sp44;
    volatile s32 pad1[1];

    if (D_80156609 != 0) {
        arg0->displayListValid = 0;
    }

    if (func_80049000(&arg0->position) != 0) {
        if (arg0->displayListValid == 0) {
            func_80097C18(sp84.rotation, arg0->spinVelocity);
            sp84.translation.x = 0;
            sp84.translation.y = 0x600000;
            sp84.translation.z = 0;
            func_800981C8(sp64.rotation, arg0->scale, arg0->angle.half.yaw + 0x800, arg0->angle.half.pitch);
            sp64.translation.x = arg0->position.x;
            sp64.translation.y = arg0->position.y;
            sp64.translation.z = arg0->position.z;
            func_800987A0(&sp84, &sp64, &sp44);
            arg0->displayList0 = func_8004885C(&sp64);
            arg0->displayList1 = func_8004885C(&sp44);
        }

        if (arg0->displayList1 != NULL) {
            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(gRegionAllocPtr++, 0x02, func_80043040(D_80112144));
            gSPSegment(gRegionAllocPtr++, 0x03, func_80043040(D_80112146));
            gSPMatrix(gRegionAllocPtr++, arg0->displayList0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gRegionAllocPtr++, D_20028F0);
            gSPMatrix(gRegionAllocPtr++, arg0->displayList1, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gRegionAllocPtr++, D_2002DB8);
        }
    }
}

void approachRaceCoursePreviewSpinStep(RaceCoursePreviewCamera *arg0, s16 arg1) {
    s16 diff = arg1 - arg0->timer;

    if (diff >= 5) {
        diff = 4;
    }
    if (diff < -4) {
        diff = -4;
    }
    arg0->timer += diff;
}

void approachRaceCoursePreviewVerticalVelocity(RaceCoursePreviewCamera *arg0, s32 arg1) {
    s32 diff = arg1 - arg0->velocityY;

    if (diff >= 0x2001) {
        diff = 0x2000;
    }
    if (diff < -0x2000) {
        diff = -0x2000;
    }
    arg0->velocityY += diff;
}

void approachRaceCoursePreviewOrbitRadius(RaceCoursePreviewCamera *arg0, s32 arg1) {
    s32 diff = arg1 - arg0->radius;

    if (diff >= 0x2001) {
        diff = 0x2000;
    }
    if (diff < -0x2000) {
        diff = -0x2000;
    }
    if (diff >= 0 && arg0->radius != 0) {
        if (arg0->tilt < 0x800) {
            arg0->tilt += 0x28;
        }
    } else {
        if (arg0->tilt != 0) {
            arg0->tilt -= 0x28;
        }
    }
    arg0->radius += diff;
}

void approachRaceCoursePreviewPitchVelocity(RaceCoursePreviewCamera *arg0, s16 arg1) {
    s16 diff = arg1 - arg0->pitchVelocity;

    if (diff >= 5) {
        diff = 4;
    }
    if (diff < -4) {
        diff = -4;
    }

    arg0->pitchVelocity += diff;
    arg0->angle.half.pitch += (((-arg0->pitchVelocity * 2) - arg0->angle.half.pitch) >> 3);
}

void updateRaceCoursePreviewCameraMotion(RaceCoursePreviewCamera *arg0) {
    s32 sine;
    s32 cosine;

    arg0->spinVelocity += arg0->timer;
    arg0->angle.half.yaw += arg0->pitchVelocity;
    arg0->position.y += arg0->velocityY;

    sine = func_80097AE8(arg0->angle.half.yaw);
    cosine = func_80097B48(arg0->angle.half.yaw);

    arg0->position.x += ((s64) -arg0->radius * sine) / 0x1000;
    arg0->position.z += ((s64) -arg0->radius * cosine) / 0x1000;
    arg0->scale = 0x80 - ((func_80097AE8(arg0->tilt + 0x400) + 0x1000) / 0x40);

    func_800483FC(&D_801248D4, drawRaceCoursePreviewCameraModel, (s32) arg0);
}

void updateRaceCoursePreviewCameraIdle(RaceCoursePreviewCamera *arg0) {
    approachRaceCoursePreviewSpinStep(arg0, 0x130);
    approachRaceCoursePreviewVerticalVelocity(arg0, 0);
    approachRaceCoursePreviewOrbitRadius(arg0, 0x100000);
    approachRaceCoursePreviewPitchVelocity(arg0, 0xA);
    updateRaceCoursePreviewCameraMotion(arg0);
}

void initRaceCoursePreviewCameraIdle(RaceCoursePreviewCamera *arg0) {
    arg0->position.x = 0xB51A13A3;
    arg0->position.y = 0xD0E85B43;
    arg0->position.z = 0x9A645264;
    arg0->scale = 0;
    arg0->angle.half.yaw = 0x17A0;
    arg0->angle.half.pitch = 0;
    arg0->timer = 0x130;
    arg0->pitchVelocity = 0;
    arg0->velocityY = 0;
    arg0->radius = 0x100000;
    arg0->stateTimer = 0;
    if (D_80122288 == 2) {
        setCallbackTaskCallback(arg0, updateRaceCoursePreviewCameraIdle);
    }
}

void updateRaceCoursePreviewLongPanReturn(RaceCoursePreviewCamera *arg0) {
    approachRaceCoursePreviewSpinStep(arg0, 0x130);
    approachRaceCoursePreviewVerticalVelocity(arg0, -0x20000);
    approachRaceCoursePreviewOrbitRadius(arg0, 0x100000);
    approachRaceCoursePreviewPitchVelocity(arg0, -3);
    updateRaceCoursePreviewCameraMotion(arg0);
    arg0->stateTimer--;
    if (arg0->stateTimer == 0) {
        setCallbackTaskCallback(arg0, initRaceCoursePreviewCameraIdle);
    }
}

void initRaceCoursePreviewLongPanReturn(RaceCoursePreviewCamera *arg0) {
    arg0->position.x = 0x05CA84CF;
    arg0->position.y = 0x00D0C976;
    arg0->position.z = 0xFAA3DA4A;
    arg0->scale = 0;
    arg0->angle.half.yaw = 0x15E0;
    arg0->angle.half.pitch = 0;
    arg0->timer = 0x130;
    arg0->pitchVelocity = 0;
    arg0->velocityY = 0;
    arg0->radius = 0x100000;
    arg0->stateTimer = 0x154;
    if (D_80122288 == 1) {
        setCallbackTaskCallback(arg0, updateRaceCoursePreviewLongPanReturn);
    }
}

void updateRaceCoursePreviewLongPanHold(RaceCoursePreviewCamera *arg0) {
    approachRaceCoursePreviewSpinStep(arg0, 0x130);
    approachRaceCoursePreviewVerticalVelocity(arg0, 0);
    approachRaceCoursePreviewOrbitRadius(arg0, 0x100000);
    approachRaceCoursePreviewPitchVelocity(arg0, 0);
    updateRaceCoursePreviewCameraMotion(arg0);
    arg0->stateTimer--;
    if (arg0->stateTimer == 0) {
        setCallbackTaskCallback(arg0, initRaceCoursePreviewLongPanReturn);
    }
}

void updateRaceCoursePreviewLongPanPitchUp(RaceCoursePreviewCamera *arg0) {
    approachRaceCoursePreviewSpinStep(arg0, 0x130);
    approachRaceCoursePreviewVerticalVelocity(arg0, 0);
    approachRaceCoursePreviewOrbitRadius(arg0, 0x100000);
    approachRaceCoursePreviewPitchVelocity(arg0, 0x30);
    updateRaceCoursePreviewCameraMotion(arg0);
    arg0->stateTimer--;
    if (arg0->stateTimer == 0) {
        arg0->stateTimer = 0x96;
        setCallbackTaskCallback(arg0, updateRaceCoursePreviewLongPanHold);
    }
}

void updateRaceCoursePreviewLongPanRise(RaceCoursePreviewCamera *arg0) {
    approachRaceCoursePreviewSpinStep(arg0, 0x130);
    approachRaceCoursePreviewVerticalVelocity(arg0, 0x20000);
    approachRaceCoursePreviewOrbitRadius(arg0, 0x100000);
    approachRaceCoursePreviewPitchVelocity(arg0, 0);
    arg0->stateTimer--;
    updateRaceCoursePreviewCameraMotion(arg0);
    if (arg0->stateTimer == 0) {
        arg0->stateTimer = 0x2A;
        setCallbackTaskCallback(arg0, updateRaceCoursePreviewLongPanPitchUp);
    }
}

void waitRaceCoursePreviewLongPanTrigger(RaceCoursePreviewCamera *arg0) {
    approachRaceCoursePreviewSpinStep(arg0, 0x130);
    updateRaceCoursePreviewCameraMotion(arg0);
    if (D_80122282 == 0x35) {
        arg0->stateTimer = 0x6A;
        setCallbackTaskCallback(arg0, updateRaceCoursePreviewLongPanRise);
    }
}

void initRaceCoursePreviewLongPan(RaceCoursePreviewCamera *arg0) {
    arg0->position.x = 0xE6C45F50;
    arg0->position.y = 0xED3C9CFB;
    arg0->position.z = 0xD14CD682;
    arg0->scale = 0;
    arg0->angle.half.yaw = 0x1610;
    arg0->angle.half.pitch = 0;
    arg0->timer = 0;
    arg0->pitchVelocity = 0;
    arg0->velocityY = 0;
    arg0->stateTimer = 0x1E;
    setCallbackTaskCallback(arg0, waitRaceCoursePreviewLongPanTrigger);
}

void updateRaceCoursePreviewShortPanFinal(RaceCoursePreviewCamera *arg0) {
    approachRaceCoursePreviewSpinStep(arg0, 0x130);
    approachRaceCoursePreviewVerticalVelocity(arg0, 0);
    approachRaceCoursePreviewOrbitRadius(arg0, 0x100000);
    approachRaceCoursePreviewPitchVelocity(arg0, 0x18);
    updateRaceCoursePreviewCameraMotion(arg0);
}

void initRaceCoursePreviewShortPanFinal(RaceCoursePreviewCamera *arg0) {
    arg0->position.x = 0xF049BD62;
    arg0->position.y = 0xF0E87871;
    arg0->position.z = 0xDA07DE30;
    arg0->scale = 0;
    arg0->angle.half.yaw = 0x1070;
    arg0->angle.half.pitch = 0;
    arg0->timer = 0;
    arg0->pitchVelocity = 0;
    arg0->velocityY = 0;
    arg0->radius = 0x100000;
    arg0->stateTimer = 0x1E;
    setCallbackTaskCallback(arg0, updateRaceCoursePreviewShortPanFinal);
}

void waitRaceCoursePreviewShortPanFinal(RaceCoursePreviewCamera *arg0) {
    approachRaceCoursePreviewSpinStep(arg0, 0x130);
    updateRaceCoursePreviewCameraMotion(arg0);
    if (D_80122288 == 2) {
        setCallbackTaskCallback(arg0, initRaceCoursePreviewShortPanFinal);
    }
}

void initRaceCoursePreviewShortPanSecond(RaceCoursePreviewCamera *arg0) {
    arg0->position.x = 0xAB4FC576;
    arg0->position.y = 0xE13FBC73;
    arg0->position.z = 0xD2B26423;
    arg0->scale = 0;
    arg0->angle.half.yaw = 0xDF0;
    arg0->angle.half.pitch = 0;
    arg0->timer = 0;
    arg0->pitchVelocity = 0;
    arg0->velocityY = 0;
    arg0->stateTimer = 0x1E;
    setCallbackTaskCallback(arg0, waitRaceCoursePreviewShortPanFinal);
}

void waitRaceCoursePreviewShortPanSecond(RaceCoursePreviewCamera *arg0) {
    approachRaceCoursePreviewSpinStep(arg0, 0x30);
    updateRaceCoursePreviewCameraMotion(arg0);
    if (D_80122288 == 1) {
        setCallbackTaskCallback(arg0, initRaceCoursePreviewShortPanSecond);
    }
}

void initRaceCoursePreviewShortPan(RaceCoursePreviewCamera *arg0) {
    arg0->position.x = 0xAB4FC576;
    arg0->position.y = 0xE0CEDC73;
    arg0->position.z = 0xD2B26423;
    arg0->scale = 0;
    arg0->angle.half.yaw = 0xDF0;
    arg0->angle.half.pitch = 0;
    arg0->timer = 0;
    arg0->pitchVelocity = 0;
    arg0->velocityY = 0;
    arg0->stateTimer = 0x1E;
    setCallbackTaskCallback(arg0, waitRaceCoursePreviewShortPanSecond);
}

void initRaceCoursePreviewCameraCutscene(RaceCoursePreviewCamera *arg0) {
    s16 temp_v0 = D_80121B50;

    if (temp_v0 == 3) {
        setCallbackTaskCallback(arg0, initRaceCoursePreviewLongPan);
        temp_v0 = D_80121B50;
    }
    if (temp_v0 == 6) {
        setCallbackTaskCallback(arg0, initRaceCoursePreviewShortPan);
    }
}

// drawRaceCoursePreviewAnimatedBillboards best match: 99.373% (nonmatchings/drawRaceCoursePreviewAnimatedBillboards-6061209858023118177/base_10.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_course_preview/drawRaceCoursePreviewAnimatedBillboards.s")

#ifdef NON_MATCHING
void drawRaceCoursePreviewAnimatedBillboards(RaceCoursePreviewMeshActor *arg0) {
    volatile u8 pad[0xC];
    u32 image;
    u32 palette;
    s16 width;
    s16 height;
    RaceCoursePreviewRenderCommandEntry *entry;
    Gfx *gfx;
    s16 textureIndex;
    s16 loadedTextureIndex;
    s32 i;

    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
    entry = gRaceCoursePreviewAnimatedBillboardCommandsByCourse[D_80121B50];
    loadedTextureIndex = -1;
    i = 0;
    if (entry->textureIndex != -1) {
        do {
            if (func_80049000(entry->command) != 0) {
                textureIndex = gRaceCoursePreviewAnimatedBillboardTextureIds[entry->textureIndex] + ((s32)(gFrameCounter & 4) / 4);
                if (textureIndex != loadedTextureIndex) {
                    loadedTextureIndex = textureIndex;
                    func_80045A1C((u8 *)func_80043040((s32)D_80112168), textureIndex & 0xFFFF, &image, &palette,
                                  &width, &height);
                    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, width, height, 0, G_TX_CLAMP,
                                            G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
                    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
                }
                gSPMatrix(gRegionAllocPtr++, &arg0->matrices[i], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, D_80156614, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gfx = gRegionAllocPtr++;
                gfx->words.w1 = (u32)gRaceCoursePreviewAnimatedBillboardVertices;
                gfx->words.w0 = 0x0400103F;
                RACE_COURSE_PREVIEW_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
            }
            entry++;
            i++;
        } while (entry->textureIndex != -1);
    }
    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
}
#endif

void enqueueDrawRaceCoursePreviewAnimatedBillboards(s32 arg0) {
    func_800483FC(&D_801248D4, drawRaceCoursePreviewAnimatedBillboards, arg0);
}

void initRaceCoursePreviewAnimatedBillboards(RaceCoursePreviewMeshActor *arg0) {
    RaceCoursePreviewRenderCommandEntry *entry;
    s32 count;
    s32 allocSize;
    s32 i;

    entry = gRaceCoursePreviewAnimatedBillboardCommandsByCourse[D_80121B50];
    count = 0;
    if (entry->textureIndex != -1) {
        do {
            count++;
            entry++;
        } while (entry->textureIndex != -1);
    }

    if (count != 0) {
        entry = gRaceCoursePreviewAnimatedBillboardCommandsByCourse[D_80121B50];
        allocSize = count * sizeof(GfxCommandDest);
        D_80112130.matrixHandle2 = func_80042D58(allocSize);
        arg0->matrices = func_80043040(D_80112130.matrixHandle2);

        i = 0;
        if (count > 0) {
            do {
                arg0->matrices[i] = gIdentityMatrix;
                func_80048C90(&arg0->matrices[i], entry->command);
                i++;
                entry++;
            } while (i != count);
        }
        osWritebackDCache(arg0->matrices, allocSize);
    }

    setCallbackTaskCallback(arg0, enqueueDrawRaceCoursePreviewAnimatedBillboards);
}
