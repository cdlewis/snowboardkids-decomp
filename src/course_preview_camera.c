#include "common.h"
#include "memory_allocator.h"
#include "effect_task_scheduler.h"
#include "asset_decompression.h"
#include "course_preview_camera.h"
#include "fixed_point_math.h"
#include "fixed_point_matrix.h"

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
} CoursePreviewGfxCommandEntry;

struct CoursePreviewGfxCommandActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ GfxCommandDest *matrices;
};

typedef struct CoursePreviewCamera CoursePreviewCamera;
typedef void (*CoursePreviewCameraCallback)(CoursePreviewCamera *);

typedef union {
    s32 word;
    struct {
        s16 yaw;
        s16 pitch;
    } half;
} PackedAngles;

struct CoursePreviewCamera {
    /* 0x00 */ char pad0[0x8];
    /* 0x08 */ CoursePreviewCameraCallback callback;
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
    /* 0x40 */ Gfx *displayList1;
    /* 0x44 */ s8 displayListValid;
};

typedef struct {
    /* 0x00 */ u8 pad0[0x4E];
    /* 0x4E */ s16 matrixHandle;
    /* 0x50 */ s16 matrixHandle2;
} CoursePreviewAssetHandles;

extern void *D_801248D4;
extern void func_800483FC(void *, void *, s32);
extern void osWritebackDCache(void *, s32);
extern s16 D_80112144;
extern s16 D_80112146;
extern s16 D_80112168;
extern s16 D_8011216A;
extern CoursePreviewAssetHandles D_80112130;
extern u8 D_80156609;
extern Gfx *gRegionAllocPtr;
extern Gfx D_20028F0[];
extern Gfx D_2002DB8[];
extern Gfx *func_8004885C(FixedTransform *arg0);
extern void func_80045990(s32 arg0, s32 arg1, s16 *arg2, s16 *arg3);
extern Vec3i D_800D5CC8[];
extern GfxCommandDest *D_800D5C50[];
extern CoursePreviewGfxCommandEntry *D_800D5C6C[];
extern CoursePreviewGfxCommandEntry *D_800D5FC8[];
extern u16 D_800D5D30[];
extern Gfx D_800D5CF0[];
extern Gfx D_800D5C88[];
extern Gfx D_800D9D00[];
extern Gfx D_800D9D40[];
extern GfxCommandDest D_800DEE50;
extern FixedTransform D_800DEE30;
extern void func_80048C90(GfxCommandDest *, s32 *);
extern void func_80045A1C(u8 *, u16, u32 *, u32 *, s16 *, s16 *);

extern s8 D_80122288;
extern s16 D_80122282;
extern s16 D_80121B50;
extern s16 D_801235B0;
extern u32 D_80156614;

// func_800556B0 best match: 99.170% (nonmatchings/func_800556B0-4923837976568703863/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_preview_camera/func_800556B0.s")

#ifdef NON_MATCHING
void func_800556B0(CoursePreviewGfxCommandActor *arg0) {
    u32 image;
    u32 palette;
    s16 width;
    s16 height;
    volatile u8 pad[0x10];
    CoursePreviewGfxCommandEntry *entry;
    GfxCommandDest *vertices;
    Gfx *gfx;
    s16 textureIndex;
    s32 i;
    s8 nextTextureIndex;

    gSPDisplayList(gRegionAllocPtr++, D_800D9D00);
    entry = D_800D5C6C[D_80121B50];
    vertices = D_800D5C50[D_80121B50];
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
                gfx = gRegionAllocPtr;
                gRegionAllocPtr = gfx + 1;
                gfx->words.w0 = 0x0400103F;
                gfx->words.w1 = (u32)&vertices[entry->textureIndex];
                gfx = gRegionAllocPtr;
                gRegionAllocPtr = gfx + 1;
                gfx->words.w0 = 0xB1060402;
                gfx->words.w1 = 0x60200;
            }
            nextTextureIndex = entry[1].textureIndex;
            entry++;
            i++;
        } while (nextTextureIndex != -1);
    }

    gSPDisplayList(gRegionAllocPtr++, D_800D9D40);
}
#endif

void func_80055B04(s32 arg0) {
    func_800483FC(&D_801248D4, func_800556B0, arg0);
}

void func_80055B34(CoursePreviewGfxCommandActor *arg0) {
    CoursePreviewGfxCommandEntry *entry;
    s32 count;
    s32 allocSize;
    s32 i;

    entry = D_800D5C6C[D_80121B50];
    count = 0;
    if (entry->textureIndex != -1) {
        do {
            count++;
            entry++;
        } while (entry->textureIndex != -1);
    }

    if (count != 0) {
        entry = D_800D5C6C[D_80121B50];
        allocSize = count * sizeof(GfxCommandDest);
        D_80112130.matrixHandle = func_80042D58(allocSize);
        arg0->matrices = func_80043040(D_80112130.matrixHandle);

        i = 0;
        if (count > 0) {
            do {
                arg0->matrices[i] = D_800DEE50;
                func_80048C90(&arg0->matrices[i], entry->command);
                i++;
                entry++;
            } while (i != count);
        }
        osWritebackDCache(arg0->matrices, allocSize);
    }

    func_80071824(arg0, func_80055B04);
}

// func_80055C7C best match: 98.168% (nonmatchings/func_80055C7C-2785870559185086986/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/course_preview_camera/func_80055C7C.s")

#ifdef NON_MATCHING
void func_80055C7C(CoursePreviewCamera *arg0) {
    FixedTransform sp70;
    Gfx *temp_v0;
    Gfx *temp_v0_2;
    Gfx *temp_v0_3;
    Gfx *temp_v0_4;
    Gfx *temp_v0_5;
    Gfx *temp_v0_6;
    Gfx *temp_v0_7;
    Gfx *temp_v0_8;
    Gfx *temp_v0_9;
    Gfx *temp_v0_10;
    Gfx *temp_v0_11;
    Gfx *temp_v0_12;
    Gfx *temp_v0_13;
    Gfx *temp_v0_14;
    Gfx *temp_v0_15;
    Gfx *temp_v0_16;
    Gfx *temp_v0_17;
    Gfx *temp_v0_18;
    Gfx *temp_v0_19;

    if (D_80156609 != 0) {
        arg0->displayListValid = 1;
    }

    if (func_80049000(&arg0->position) != 0) {
        if (arg0->displayListValid != 0) {
            sp70 = D_800DEE30;
            sp70.translation.x = arg0->position.x;
            sp70.translation.y = arg0->position.y;
            sp70.translation.z = arg0->position.z;
            arg0->displayList0 = func_8004885C(&sp70);
        }

        if (arg0->displayList0 != NULL) {
            temp_v0 = gRegionAllocPtr++;
            temp_v0->words.w1 = (u32)D_800D9D00;
            temp_v0->words.w0 = 0x06000000;
            temp_v0_2 = gRegionAllocPtr++;
            temp_v0_2->words.w0 = 0xFD500000;
            temp_v0_2->words.w1 = arg0->image;
            temp_v0_3 = gRegionAllocPtr++;
            temp_v0_3->words.w0 = 0xF5500000;
            temp_v0_3->words.w1 = 0x07080200;
            temp_v0_4 = gRegionAllocPtr++;
            temp_v0_4->words.w1 = 0;
            temp_v0_4->words.w0 = 0xE6000000;
            temp_v0_5 = gRegionAllocPtr++;
            temp_v0_5->words.w0 = 0xF3000000;
            temp_v0_5->words.w1 = 0x070FF400;
            temp_v0_6 = gRegionAllocPtr++;
            temp_v0_6->words.w1 = 0;
            temp_v0_6->words.w0 = 0xE7000000;
            temp_v0_7 = gRegionAllocPtr++;
            temp_v0_7->words.w1 = 0x00080200;
            temp_v0_7->words.w0 = 0xF5400400;
            temp_v0_8 = gRegionAllocPtr++;
            temp_v0_8->words.w0 = 0xF2000000;
            temp_v0_8->words.w1 = 0x0007C07C;
            temp_v0_9 = gRegionAllocPtr++;
            temp_v0_9->words.w0 = 0xFD100000;
            temp_v0_9->words.w1 = arg0->palette;
            temp_v0_10 = gRegionAllocPtr++;
            temp_v0_10->words.w1 = 0;
            temp_v0_10->words.w0 = 0xE8000000;
            temp_v0_11 = gRegionAllocPtr++;
            temp_v0_11->words.w0 = 0xF5000100;
            temp_v0_11->words.w1 = 0x07000000;
            temp_v0_12 = gRegionAllocPtr++;
            temp_v0_12->words.w1 = 0;
            temp_v0_12->words.w0 = 0xE6000000;
            temp_v0_13 = gRegionAllocPtr++;
            temp_v0_13->words.w0 = 0xF0000000;
            temp_v0_13->words.w1 = 0x0703C000;
            temp_v0_14 = gRegionAllocPtr++;
            temp_v0_14->words.w1 = 0;
            temp_v0_14->words.w0 = 0xE7000000;
            temp_v0_15 = gRegionAllocPtr++;
            temp_v0_15->words.w0 = 0x01020040;
            temp_v0_15->words.w1 = (u32)arg0->displayList0;
            temp_v0_16 = gRegionAllocPtr++;
            temp_v0_16->words.w0 = 0x01000040;
            temp_v0_16->words.w1 = D_80156614;
            temp_v0_17 = gRegionAllocPtr++;
            temp_v0_17->words.w1 = (u32)D_800D5C88;
            temp_v0_17->words.w0 = 0x0400103F;
            temp_v0_18 = gRegionAllocPtr++;
            temp_v0_18->words.w1 = 0x00060200;
            temp_v0_18->words.w0 = 0xB1060402;
            temp_v0_19 = gRegionAllocPtr++;
            temp_v0_19->words.w1 = (u32)D_800D9D40;
            temp_v0_19->words.w0 = 0x06000000;
        }
    }
}
#endif

void func_80055FA4(CoursePreviewCamera *arg0) {
    Vec3i sp44;
    FixedMatrix3sScratch sp24;
    CoursePreviewCamera *temp_s0 = arg0;

    arg0->timer--;
    if (arg0->timer == 0) {
        arg0->timer = func_800430D0() + 0x1E;
        func_80071824(arg0, func_80056070);
    }
    func_80097C18(sp24, 0x6D0);
    func_80098590(sp24, (Vec3i *)&temp_s0->velocityY, &sp44);
    temp_s0->position.x += sp44.x;
    temp_s0->position.y += sp44.y;
    temp_s0->position.z += sp44.z;
    temp_s0->radius -= 0x4000;
    func_800483FC(&D_801248D4, func_80055C7C, (s32) temp_s0);
}

void func_80056070(CoursePreviewCamera *arg0) {
    Vec3i *temp_t3;

    arg0->timer--;
    if (!arg0->timer) {
        arg0->timer = 0x5A;
        temp_t3 = &D_800D5CC8[arg0->index];
        arg0->position = *(&D_800D5CC8[arg0->index]);
        arg0->velocityY = 0;
        arg0->radius = 0;
        arg0->angle.word = 0xFFF00000;
        func_80071824(arg0, func_80055FA4);
    }
}

void func_800560F4(CoursePreviewCamera *arg0) {
    arg0->timer = (arg0->index * 0x1E) + 0x1E;
    func_80045990(func_80043040(D_8011216A), (arg0->index + 3) & 0xFFFF, &arg0->scale, &arg0->pitchVelocity);
    func_80071824(arg0, func_80056070);
}

void func_8005616C(CoursePreviewCamera *arg0) {
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

void func_80056348(CoursePreviewCamera *arg0, s16 arg1) {
    s16 diff = arg1 - arg0->timer;

    if (diff >= 5) {
        diff = 4;
    }
    if (diff < -4) {
        diff = -4;
    }
    arg0->timer += diff;
}

void func_8005638C(CoursePreviewCamera *arg0, s32 arg1) {
    s32 diff = arg1 - arg0->velocityY;

    if (diff >= 0x2001) {
        diff = 0x2000;
    }
    if (diff < -0x2000) {
        diff = -0x2000;
    }
    arg0->velocityY += diff;
}

#pragma GLOBAL_ASM("asm/nonmatchings/course_preview_camera/func_800563C4.s")

void func_80056444(CoursePreviewCamera *arg0, s16 arg1) {
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

void func_800564A8(CoursePreviewCamera *arg0) {
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

    func_800483FC(&D_801248D4, func_8005616C, (s32) arg0);
}

void func_800565E8(CoursePreviewCamera *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, 0);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, 0xA);
    func_800564A8(arg0);
}

void func_8005663C(CoursePreviewCamera *arg0) {
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
        func_80071824(arg0, func_800565E8);
    }
}

void func_800566C0(CoursePreviewCamera *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, -0x20000);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, -3);
    func_800564A8(arg0);
    arg0->stateTimer--;
    if (arg0->stateTimer == 0) {
        func_80071824(arg0, func_8005663C);
    }
}

void func_8005673C(CoursePreviewCamera *arg0) {
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
        func_80071824(arg0, func_800566C0);
    }
}

void func_800567C4(CoursePreviewCamera *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, 0);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, 0);
    func_800564A8(arg0);
    arg0->stateTimer--;
    if (arg0->stateTimer == 0) {
        func_80071824(arg0, func_8005673C);
    }
}

void func_80056840(CoursePreviewCamera *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, 0);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, 0x30);
    func_800564A8(arg0);
    arg0->stateTimer--;
    if (arg0->stateTimer == 0) {
        arg0->stateTimer = 0x96;
        func_80071824(arg0, func_800567C4);
    }
}

void func_800568C0(CoursePreviewCamera *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, 0x20000);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, 0);
    arg0->stateTimer--;
    func_800564A8(arg0);
    if (arg0->stateTimer == 0) {
        arg0->stateTimer = 0x2A;
        func_80071824(arg0, func_80056840);
    }
}

void func_80056940(CoursePreviewCamera *arg0) {
    func_80056348(arg0, 0x130);
    func_800564A8(arg0);
    if (D_80122282 == 0x35) {
        arg0->stateTimer = 0x6A;
        func_80071824(arg0, func_800568C0);
    }
}

void func_80056998(CoursePreviewCamera *arg0) {
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
    func_80071824(arg0, func_80056940);
}

void func_80056A04(CoursePreviewCamera *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, 0);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, 0x18);
    func_800564A8(arg0);
}

void func_80056A58(CoursePreviewCamera *arg0) {
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
    func_80071824(arg0, func_80056A04);
}

void func_80056ACC(CoursePreviewCamera *arg0) {
    func_80056348(arg0, 0x130);
    func_800564A8(arg0);
    if (D_80122288 == 2) {
        func_80071824(arg0, func_80056A58);
    }
}

void func_80056B1C(CoursePreviewCamera *arg0) {
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
    func_80071824(arg0, func_80056ACC);
}

void func_80056B88(CoursePreviewCamera *arg0) {
    func_80056348(arg0, 0x30);
    func_800564A8(arg0);
    if (D_80122288 == 1) {
        func_80071824(arg0, func_80056B1C);
    }
}

void func_80056BD8(CoursePreviewCamera *arg0) {
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
    func_80071824(arg0, func_80056B88);
}

void func_80056C44(CoursePreviewCamera *arg0) {
    s16 temp_v0 = D_80121B50;

    if (temp_v0 == 3) {
        func_80071824(arg0, func_80056998);
        temp_v0 = D_80121B50;
    }
    if (temp_v0 == 6) {
        func_80071824(arg0, func_80056BD8);
    }
}

// func_80056CA0 best match: 95.121%
#pragma GLOBAL_ASM("asm/nonmatchings/course_preview_camera/func_80056CA0.s")

#ifdef NON_MATCHING
void func_80056CA0(CoursePreviewGfxCommandActor *arg0) {
    u32 spC0;
    u32 spBC;
    s16 spBA;
    s16 spB8;
    CoursePreviewGfxCommandEntry *var_s2;
    Gfx *temp_v0_13;
    Gfx *temp_v0_14;
    s16 temp_s1;
    s16 var_s5;
    s32 var_s3;
    s8 temp_t7;

    gSPDisplayList(gRegionAllocPtr++, D_800D9D00);
    var_s2 = D_800D5FC8[D_80121B50];
    var_s5 = -1;
    var_s3 = 0;
    if (var_s2->textureIndex != -1) {
        do {
            if (func_80049000(var_s2->command) != 0) {
                temp_s1 = D_800D5D30[var_s2->textureIndex] + ((s32)(D_801235B0 & 4) / 4);
                if (temp_s1 != var_s5) {
                    var_s5 = temp_s1;
                    func_80045A1C((u8 *)func_80043040((s32)D_80112168), temp_s1 & 0xFFFF, &spC0, &spBC, &spBA, &spB8);
                    gDPLoadTextureBlock_4b(gRegionAllocPtr++, spC0, G_IM_FMT_CI, spBA, spB8, 0, G_TX_CLAMP,
                                            G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
                    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, spBC);
                }
                gSPMatrix(gRegionAllocPtr++, &arg0->matrices[var_s3], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, D_80156614, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                temp_v0_13 = gRegionAllocPtr;
                gRegionAllocPtr = temp_v0_13 + 1;
                temp_v0_13->words.w1 = (u32)D_800D5CF0;
                temp_v0_13->words.w0 = 0x0400103F;
                temp_v0_14 = gRegionAllocPtr;
                gRegionAllocPtr = temp_v0_14 + 1;
                temp_v0_14->words.w1 = 0x60200;
                temp_v0_14->words.w0 = 0xB1060402;
            }
            temp_t7 = var_s2[1].textureIndex;
            var_s2++;
            var_s3 += 1;
        } while (temp_t7 != -1);
    }
    gSPDisplayList(gRegionAllocPtr++, D_800D9D40);
}
#endif

void func_8005711C(s32 arg0) {
    func_800483FC(&D_801248D4, func_80056CA0, arg0);
}

void func_8005714C(CoursePreviewGfxCommandActor *arg0) {
    CoursePreviewGfxCommandEntry *entry;
    s32 count;
    s32 allocSize;
    s32 i;

    entry = D_800D5FC8[D_80121B50];
    count = 0;
    if (entry->textureIndex != -1) {
        do {
            count++;
            entry++;
        } while (entry->textureIndex != -1);
    }

    if (count != 0) {
        entry = D_800D5FC8[D_80121B50];
        allocSize = count * sizeof(GfxCommandDest);
        D_80112130.matrixHandle2 = func_80042D58(allocSize);
        arg0->matrices = func_80043040(D_80112130.matrixHandle2);

        i = 0;
        if (count > 0) {
            do {
                arg0->matrices[i] = D_800DEE50;
                func_80048C90(&arg0->matrices[i], entry->command);
                i++;
                entry++;
            } while (i != count);
        }
        osWritebackDCache(arg0->matrices, allocSize);
    }

    func_80071824(arg0, func_8005711C);
}
