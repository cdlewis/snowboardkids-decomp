#include "common.h"

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i;

typedef struct {
    char pad[0x18];
    s16 step;
    u16 timer;
} RaceCountdownEffect;

typedef struct {
    char pad[0x10];
    u16 playerIndex;
} RacePlayerEffect;

typedef struct {
    s32 words[0x10];
} CourseRenderCommand;

typedef struct {
    char pad0[0x18];
    CourseRenderCommand *vertices;
} RaceCourseRenderEffect;

typedef struct {
    s16 displayListIndex;
    s16 pad2;
    Vec3i position;
    s32 pad10;
} CourseRenderEntry;

typedef struct {
    s8 type;
    char pad1[3];
    Vec3i pos;
} CourseMarkerSpawnEntry;

typedef struct {
    char pad0[0x13];
    s8 isActive;
    char pad14[0x5F8];
} CourseEffectPlayer;

typedef struct {
    char pad[0x18];
    Vec3i pos;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    char pad34[4];
    s32 unk38;
    s16 unk3C;
    s16 unk3E;
    s16 unk40;
    s16 pad42;
    s16 displayListValid;
    s16 pad46;
    void *displayList;
    s16 unk4C;
    s16 unk4E;
    s32 unk50;
} Struct6B760;

typedef struct {
    s16 rotation[9];
    s16 pad2A;
    Vec3i basePos;
} CourseEffectMatrixSource;

typedef struct {
    char pad0[0x18];
    CourseEffectMatrixSource source;
    Vec3i pos1;
    Vec3i pos2;
    s16 unk50;
    s16 unk52;
    s16 unk54;
    s16 unk56;
    void *sourceMatrix;
    void *pos1Matrix;
    void *pos2Matrix;
} Struct6C51C;

typedef struct {
    char pad0[0x18];
    Vec3i velocity;
    Vec3i pos;
    char unk30[0x20];
    s16 timer;
    s16 unk52;
    void *matrix;
} RaceMovingEffect;

typedef struct {
    char pad0[8];
    Vec3i unk8;
    char pad14[0xC];
    Vec3i pos;
    char pad2C[0x1C];
} CourseSpawnEntry;

typedef struct {
    s16 angle;
    s16 unk2;
    char pad4[0x44];
} CourseAngleEntry;

typedef struct {
    char pad0[0x10];
    u16 entryIndex;
    char pad12[6];
    void *texture;
    void *palette;
    s16 rotation;
    char pad22[2];
    Vtx *vertices;
    Vtx *baseVertices;
    s16 vertexCount;
    char pad2E[2];
    s32 texturePtr;
    s32 palettePtr;
    s32 useAltQueue;
    s32 unk3C;
} RaceCourseMarkerEffect;

typedef struct {
    s32 texturePtr;
    s32 palettePtr;
    void *baseVerticesInput;
    s16 vertexCount;
    s16 flags;
    u16 textureIndex;
    s16 unk12;
} CourseMarkerEntry;

typedef struct {
    void *baseVerticesInput;
    char pad4[0x10];
} CourseMarkerVertexResource;

typedef struct {
    u16 textureIndex;
    char pad2[0x12];
} CourseMarkerTextureResource;

typedef struct {
    s16 scaleX;
    s16 scaleY;
    s16 scaleZ;
    s16 unk6;
    Vec3i pos;
    s16 pitch;
    s16 yaw;
    s32 displayList;
} CourseTriggerEntry;

typedef struct {
    Vec3i dest;
    Vec3i source;
    char mtx[0x20];
    s32 pad38;
} CourseTriggerScratch;

typedef struct {
    char pad0[0x10];
    u16 entryIndex;
    char pad12[6];
    void *matrix;
    Vec3i pos1;
    Vec3i pos2;
    s16 pitch;
    s16 yaw;
    s32 scaleX;
    s32 scaleY;
    s32 scaleZ;
    s32 displayList;
} RaceCourseTriggerEffect;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s16 angle;
    s16 unkE;
} SoundParams;

typedef struct {
    s16 angle;
    char pad2[0xE];
} SoundParamAngle;

typedef struct {
    char pad0[0x14];
    s16 courseVtxHandle;
    s16 courseTextureHandle;
    char pad18[0x2A];
    s16 markerMatrixHandle;
} CourseAssetHandles;

extern void func_80071824(void *task, void (*callback)());
extern void func_800483FC(void *, void *, void *);
extern void func_800716E4(void *);
extern void func_80072138(s32, s32);
extern void func_80072A74(s32, void *, s32, s32);
extern void osWritebackDCache(void *, s32);
extern void func_8006A80C(void *);
extern void func_80069BEC(void *);
extern void func_80069E50(void);
extern s16 func_80042D58(s32);
extern s32 func_80043040(s16);
extern void *func_80048594(s32);
extern void *func_8004885C(CourseEffectMatrixSource *);
extern void func_80048C90(CourseRenderCommand *, Vec3i *);
extern s32 func_80049000(Vec3i *);
extern void func_80045990(s32, s32, void *, void *);
extern s32 func_8004597C(s32, s32);
extern void func_80047174(s32, s32, s32, s32, s32);
extern void func_80045A78(s32, s32, s32, s32);
extern void func_80088294(Vec3i *, s32, s32, u16);
extern void func_80088A1C(Vec3i *, s32, s32, s32, s32);
extern void func_80097C18(void *, s16);
extern void func_80097FE4(void *, s16, s16);
extern void func_80098174(s16 *, s16, s16);
extern void func_80098590(void *, void *, Vec3i *);
extern void func_80048D60(CourseEffectMatrixSource *);
extern s32 func_80097AE8(s16);
extern s16 func_8004940C(s32, s32, s32, s32);
extern s32 func_80080CC4(s16, s32, s32);
extern void func_80088294(Vec3i *, s32, s32, u16);
extern void func_80088A1C(Vec3i *, s32, s32, s32, s32);
extern s16 D_80112168;
extern s16 D_80112140;
extern s16 D_80112142;
extern s32 D_801235B4;
extern u8 D_80156608;
extern u8 D_80156609;
extern void func_8006C5C0(Struct6C51C *);
void func_8006C1B4(Struct6C51C *);
void func_8006CCC0(RaceCourseTriggerEffect *);
void func_8006CE68(CourseEffectPlayer *, RaceCourseTriggerEffect *);
void func_8006D2D0(RaceCourseTriggerEffect *);
void func_80069890(RaceCountdownEffect *);
void func_80069914(RaceCountdownEffect *);
void func_80069998(RaceCountdownEffect *);
void func_800699F0(RaceCountdownEffect *);
void func_80069A78(RaceCountdownEffect *);
void func_80069AF0(RaceCountdownEffect *);
void func_80069B60(RaceCountdownEffect *);
void func_8006A894(void *);
extern void func_8006C7F4(RaceCourseMarkerEffect *);
void func_8006B3E0(Struct6B760 *);
void func_8006B6C8(Struct6B760 *);
void func_8006AF48(RaceCourseRenderEffect *);
void func_8006BC68(RaceMovingEffect *);
extern Struct6B760 *func_80071408(void *, s32, s32);
extern u8 D_80121B56;
extern s16 D_80121B50;
extern CourseAssetHandles D_80112130;
extern s16 D_80112144;
extern s16 D_80112146;
extern CourseMarkerSpawnEntry *D_800DA0B8[];
extern CourseRenderEntry *D_800DA73C[];
extern void *D_800DA1C0[];
extern SoundParams D_800DA764[];
extern CourseSpawnEntry D_800B9540[];
extern CourseAngleEntry D_800B9554[];
extern CourseAngleEntry D_800B9556[];
extern CourseMarkerEntry D_800DA804[];
extern CourseMarkerVertexResource D_800DA80C[];
extern CourseMarkerTextureResource D_800DA814[];
extern CourseTriggerEntry D_800DA840[];
extern Gfx D_2001D00[];
extern SoundParamAngle D_800DA770[];
extern CourseEffectPlayer D_80121D80[];
extern CourseEffectPlayer D_8012238C[];
extern CourseEffectPlayer D_80122998[];
extern CourseEffectPlayer D_80122FA4[];
extern CourseRenderCommand D_800DEE50[];
extern s32 D_80124868;
extern s32 D_80124878;
extern s32 D_801248D4;
extern s32 D_801248EC;
extern s32 D_801248D4;
extern s32 D_801248B0;
extern s32 D_801248A4;
extern s32 D_801248F8;
extern Gfx *gRegionAllocPtr;
extern Gfx D_2001678[];
extern Gfx D_2001730[];
extern Gfx D_2001810[];
extern Gfx D_20018E8[];
extern Gfx D_2000910[];
extern Gfx D_2003218[];
extern Gfx D_20057D8[];
extern Gfx D_2006430[];
extern Gfx D_20067B0[];
extern Gfx D_2008628[];
extern Gfx D_2008900[];
extern Gfx D_2008E30[];
extern Gfx D_200B400[];
extern Gfx D_200B7B8[];
extern Gfx D_200BD48[];
extern Gfx D_200C060[];
void func_8006BE90(RaceMovingEffect *);
void func_8006B7E0(RaceMovingEffect *);
extern void func_8006A894(void *);

void func_80069890(RaceCountdownEffect *arg0) {
    if (arg0->step != 0) {
        func_80047174(-0x34, -0xC, func_80043040(D_80112168), 0x3F, arg0->step);
    } else {
        func_80045A78(-0x34, -0xC, func_80043040(D_80112168), 0x3F);
    }
}

void func_80069914(RaceCountdownEffect *arg0) {
    if (arg0->step != 0) {
        func_80047174(-0x20, -0xC, func_80043040(D_80112168), 0x40, arg0->step);
    } else {
        func_80045A78(-0x20, -0xC, func_80043040(D_80112168), 0x40);
    }
}

void func_80069998(RaceCountdownEffect *arg0) {
    arg0->step++;
    if (arg0->step == 4) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_80069914, arg0);
}

void func_800699F0(RaceCountdownEffect *arg0) {
    RaceCountdownEffect *temp_a2 = arg0;

    if (arg0->step != 0) {
        arg0->step--;
    }
    temp_a2->timer--;
    if (temp_a2->timer == 0) {
        D_801235B4 &= ~1;
        func_80071824(temp_a2, func_80069998);
    }
    func_800483FC(&D_80124868, func_80069914, temp_a2);
}

void func_80069A78(RaceCountdownEffect *arg0) {
    arg0->step++;
    if (arg0->step == 4) {
        func_80072138(0x4C, 0x5A);
        arg0->timer = 0x14;
        func_80071824(arg0, func_800699F0);
    }
    func_800483FC(&D_80124868, func_80069890, arg0);
}

void func_80069AF0(RaceCountdownEffect *arg0) {
    if (arg0->step != 0) {
        arg0->step--;
    }
    arg0->timer--;
    if (arg0->timer == 0) {
        func_80071824(arg0, func_80069A78);
    }
    func_800483FC(&D_80124868, func_80069890, arg0);
}

void func_80069B60(RaceCountdownEffect *arg0) {
    if ((--arg0->timer) == 0) {
        func_80072138(0x4B, 0x5A);
        arg0->step = 4;
        arg0->timer = 0x3C;
        func_80071824(arg0, func_80069AF0);
    }
}

void func_80069BC0(RaceCountdownEffect *arg0) {
    arg0->timer = 0x14;
    func_80071824(arg0, func_80069B60);
}

void func_80069BEC(void *arg0) {
    volatile u8 pad[0x30];

    gDPPipeSync(gRegionAllocPtr++);

    gSPSegment(gRegionAllocPtr++, 0x02, func_80043040(D_80112140));
    gSPSegment(gRegionAllocPtr++, 0x03, func_80043040(D_80112142));

    gSPMatrix(gRegionAllocPtr++, D_800DEE50, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    switch (*(u16 *)&D_80121B50) {
        case 0:
            gSPDisplayList(gRegionAllocPtr++, D_2008900);
            break;
        case 1:
            gSPDisplayList(gRegionAllocPtr++, D_2008E30);
            break;
        case 2:
            gSPDisplayList(gRegionAllocPtr++, D_2008628);
            break;
        case 3:
            gSPDisplayList(gRegionAllocPtr++, D_200B400);
            break;
        case 4:
            gSPDisplayList(gRegionAllocPtr++, D_200BD48);
            break;
        case 5:
            gSPDisplayList(gRegionAllocPtr++, D_200C060);
            break;
        case 6:
            gSPDisplayList(gRegionAllocPtr++, D_200B7B8);
            break;
        case 7:
            gSPDisplayList(gRegionAllocPtr++, D_2006430);
            break;
        case 8:
            gSPDisplayList(gRegionAllocPtr++, D_20067B0);
            break;
        case 9:
            gSPDisplayList(gRegionAllocPtr++, D_20057D8);
            break;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_80069E50.s")

void func_8006A74C(void *arg0) {
    func_800483FC(&D_801248A4, func_80069BEC, arg0);
    func_800483FC(&D_801248F8, func_80069E50, arg0);
}

void func_8006A798(void *arg0) {
    func_80071824(arg0, func_8006A74C);
}

void func_8006A7BC(RacePlayerEffect *arg0) {
    if (D_80156608 == arg0->playerIndex) {
        func_80045A78(-0x30, -0xC, func_80043040(D_80112168), 0x41);
    }
}

void func_8006A80C(void *arg0) {
    if (D_801235B4 & 8) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124878, func_8006A7BC, arg0);
}

void func_8006A85C(void *arg0) {
    func_80072138(0x52, 0x5A);
    func_80071824(arg0, func_8006A80C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006A894.s")

void func_8006ACE8(void *arg0) {
    CourseMarkerSpawnEntry *entry;
    s8 type;

    entry = D_800DA0B8[D_80121B50];
    if (entry->type != -1) {
        do {
            switch (entry->type) {
                case 0:
                case 1:
                    func_80088294(&entry->pos, 0xC0000, 0x600000, 2);
                    break;
                case 2:
                    func_80088A1C(&entry->pos, 0x20000, 0x40000, 0x30000, 4);
                    break;
            }
            type = entry[1].type;
            entry++;
        } while (entry->type != -1);
    }

    func_800483FC(&D_801248D4, func_8006A894, arg0);
}

void func_8006AE00(RaceCourseRenderEffect *arg0) {
    CourseMarkerSpawnEntry *entry;
    s32 count;
    s32 allocSize;
    s32 i;

    entry = D_800DA0B8[D_80121B50];
    count = 0;
    if (entry->type != -1) {
        do {
            count++;
            entry++;
        } while (entry->type != -1);
    }

    if (count != 0) {
        entry = D_800DA0B8[D_80121B50];
        allocSize = count * sizeof(CourseRenderCommand);
        D_80112130.markerMatrixHandle = func_80042D58(allocSize);
        arg0->vertices = func_80043040(D_80112130.markerMatrixHandle);

        i = 0;
        if (count > 0) {
            do {
                arg0->vertices[i] = D_800DEE50[0];
                func_80048C90(&arg0->vertices[i], &entry->pos);
                i++;
                entry++;
            } while (i != count);
        }
        osWritebackDCache(arg0->vertices, allocSize);
    }

    func_80071824(arg0, func_8006ACE8);
}

void func_8006AF48(RaceCourseRenderEffect *arg0) {
    CourseRenderEntry *var_s4;
    s32 var_s5;
    s32 var_s7;
    Gfx *temp_s0;
    Gfx *temp_s2;
    Gfx *temp_s3;

    var_s4 = D_800DA73C[D_80121B50];
    var_s7 = TRUE;
    var_s5 = 0;
    if (var_s4->displayListIndex != -1) {
        do {
            if (func_80049000(&var_s4->position) != 0) {
                if (var_s7 != 0) {
                    gDPPipeSync(gRegionAllocPtr++);
                    temp_s2 = gRegionAllocPtr++;
                    var_s7 = FALSE;
                    gSPSegment(temp_s2, 0x02, func_80043040(D_80112130.courseVtxHandle));

                    temp_s3 = gRegionAllocPtr++;
                    gSPSegment(temp_s3, 0x03, func_80043040(D_80112130.courseTextureHandle));
                }

                temp_s0 = gRegionAllocPtr++;
                gDma1p(temp_s0, 1, (u32)arg0->vertices + (var_s5 << 6), 0x40, 2);

                temp_s0 = gRegionAllocPtr++;
                gSPDisplayList(temp_s0, D_800DA1C0[var_s4->displayListIndex]);
            }
            var_s4++;
            var_s5++;
        } while (var_s4->displayListIndex != -1);
    }
}

void func_8006B0D8(void *arg0) {
    func_800483FC(&D_801248B0, func_8006AF48, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006B108.s")

// func_8006B228 best match: 98.727% at nonmatchings/func_8006B228-8662636370764828261/base_11.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006B228.s")

#ifdef NON_MATCHING
void func_8006B228(Struct6B760 *arg0) {
    s32 sine;
    s32 doubleSine;
    CourseEffectMatrixSource transform;
    volatile s32 pad0[16];
    Gfx *segment2;
    Gfx *gfx;

    if ((D_80156609 & 0xFF) != 0) {
        arg0->displayListValid = 0;
    }

    if (func_80049000(&arg0->pos) != 0) {
        if (arg0->displayListValid == 0) {
            arg0->displayListValid = 1;
            sine = func_80097AE8(arg0->unk40);
            if (1) {
                doubleSine = func_80097AE8((s16)(arg0->unk40 * 2));
                func_80097C18(transform.rotation, arg0->unk3E + (sine >> 4) + 0x800);
                transform.basePos.x = arg0->pos.x;
                transform.basePos.y = arg0->pos.y + ((doubleSine + 0x1000) << 4) + 0xA4000;
            }
            transform.basePos.z = arg0->pos.z;
            func_80048D60(&transform);
            arg0->displayList = func_8004885C(&transform);
        }

 do { if (arg0->displayList != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (unsigned int) ((((unsigned int) 0xe7) & ((0x01 << 8) - 1)) << 24); _g->words.w1 = 0; } ; ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((unsigned int) ((((unsigned int) ((-65) - 3)) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) (0x02 * 4)) & ((0x01 << 16) - 1)) << 8))) | ((unsigned int) ((((unsigned int) 0x06) & ((0x01 << 8) - 1)) << 0)); _g->words.w1 = (unsigned int) func_80043040(D_80112144); } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((unsigned int) ((((unsigned int) ((-65) - 3)) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) (0x03 * 4)) & ((0x01 << 16) - 1)) << 8))) | ((unsigned int) ((((unsigned int) 0x06) & ((0x01 << 8) - 1)) << 0)); _g->words.w1 = (unsigned int) func_80043040(D_80112146); } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((unsigned int) ((((unsigned int) 1) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((unsigned int) ((((unsigned int) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (unsigned int) arg0->displayList; } ; gfx = gRegionAllocPtr++; { Gfx *_g = (Gfx *) gfx; _g->words.w0 = (((unsigned int) ((((unsigned int) 6) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (unsigned int) D_2001D00; } ; } } while (0);
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006B3E0.s")

// func_8006B6C8 best match: 98.684% at nonmatchings/func_8006B6C8-5821324921387846781/base.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006B6C8.s")

#ifdef NON_MATCHING
void func_8006B6C8(Struct6B760 *arg0) {
    s32 temp24;
    s32 temp28;
    s32 temp2C;
    s16 angle;
    s16 temp4C;
    s32 temp50;
    s16 temp4E;

    if (D_80121B56 == 0) {
        angle = func_8004940C(arg0->unk24, arg0->unk2C, arg0->unk30, arg0->unk38);
        temp24 = arg0->unk24;
        temp28 = arg0->unk28;
        temp2C = arg0->unk2C;
        arg0->pos.x = temp24;
        temp4C = 0x10;
        temp50 = 0x10000;
        temp4E = 0x80;
        arg0->unk3E = angle;
        arg0->unk4C = temp4C;
        arg0->unk50 = temp50;
        arg0->unk4E = temp4E;
        arg0->pos.y = temp28;
        arg0->pos.z = temp2C;
        arg0->pos.y = func_80080CC4(arg0->unk3C, arg0->pos.x, arg0->pos.z);
        func_80071824(arg0, func_8006B3E0);
    }
}
#endif

void func_8006B760(s16 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    Struct6B760 *p = func_80071408(func_8006B6C8, 0, 0x64);
    if (p != 0) {
        p->unk24 = arg1;
        p->unk2C = arg2;
        p->unk30 = arg3;
        p->unk38 = arg4;
        p->unk3C = arg0;
    }
}

void func_8006B7E0(RaceMovingEffect *arg0) {
    volatile s32 unused;
    CourseEffectMatrixSource transform;
    volatile s32 pad[1];

    if (D_80156609 != 0) {
        func_80097C18(&transform, D_800B9556[D_80121B50].angle + 0x400);
        transform.basePos.x = arg0->pos.x;
        transform.basePos.y = arg0->pos.y;
        transform.basePos.z = arg0->pos.z;
        arg0->matrix = func_8004885C(&transform);
    }

    if (arg0->matrix != NULL) {
        if (func_80049000(&arg0->pos) != 0) {
            Gfx *_g;

            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(gRegionAllocPtr++, 0x02, func_80043040(D_80112144));
            gSPSegment(gRegionAllocPtr++, 0x03, func_80043040(D_80112146));
            gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            if (D_80121B50 != 8) {
                gSPDisplayList(gRegionAllocPtr++, D_2000910);
            } else {
                gSPDisplayList(gRegionAllocPtr++, D_2003218);
            }
        }
    }
}

void func_8006B988(RaceMovingEffect *arg0) {
    Vec3i sp24;
    s16 temp_v0;
    s32 temp_v1;
    RaceMovingEffect *temp_a3 = arg0;

    temp_v0 = arg0->timer;
    if (temp_v0 != 0) {
        if (D_80121B56 == 0) {
            temp_v1 = arg0->velocity.z;
            arg0->timer = temp_v0 - 1;
            if (temp_v1 >= (s32)0xFFF60001) {
                arg0->velocity.z = temp_v1 - 0x2000;
            }
            func_80098590(&arg0->unk30, &temp_a3->velocity, &sp24);
            temp_a3->pos.x += sp24.x;
            temp_a3->pos.y += sp24.y;
            temp_a3->pos.z += sp24.z;
        }
        func_800483FC(&D_801248A4, func_8006B7E0, temp_a3);
        return;
    }
    func_800716E4(temp_a3);
}

void func_8006BA50(RaceMovingEffect *arg0) {
    Vec3i sp2C;
    void *mtx;

    if (D_80121B56 == 0) {
        arg0->timer--;
        if (arg0->timer < 0x13) {
            arg0->velocity.z = 0xFFFC0000;
        }

        mtx = arg0->unk30;
        func_80098590(mtx, &arg0->velocity, &sp2C);
        arg0->pos.x += sp2C.x;
        arg0->pos.y += sp2C.y;
        arg0->pos.z += sp2C.z;

        if (arg0->timer == 0) {
            func_80071824(arg0, func_8006B988);
            func_80097FE4(mtx, 0x100, D_800B9556[D_80121B50].angle + 0x400);
            arg0->timer = 0x64;
        }
    }

    func_800483FC(&D_801248A4, func_8006B7E0, arg0);
}

// func_8006BB50 best match: 98.261% at nonmatchings/func_8006BB50-1404502880690620360/base_6.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006BB50.s")

#ifdef NON_MATCHING
void func_8006BB50(RaceMovingEffect *arg0) {
    u32 tempIndex;
    volatile s16 *courseIndex;
    void *mtx;

    arg0->timer = 0x46;
    courseIndex = &D_80121B50;
    arg0->velocity.z = 0x680000;
    tempIndex = D_80121B50;
    mtx = arg0->unk30;
    func_80097C18(mtx, D_800B9556[tempIndex].angle + 0x400);
    func_80098590(mtx, &arg0->velocity, &arg0->pos);
    arg0->velocity.z = 0xFFFE0000;
    arg0->pos.x += ((CourseSpawnEntry *)((u8 *)D_800B9540 + ((*(volatile s16 *)&D_80121B50) * 0x48)))->pos.x;
    arg0->pos.y += ((CourseSpawnEntry *)((u8 *)D_800B9540 + ((*(volatile s16 *)&D_80121B50) * 0x48)))->pos.y;
    arg0->pos.z += ((CourseSpawnEntry *)((u8 *)D_800B9540 + (*courseIndex * 0x48)))->pos.z;
    func_80071824(arg0, func_8006BA50);
}
#endif

void func_8006BC68(RaceMovingEffect *arg0) {
    volatile s32 unused;
    CourseEffectMatrixSource transform;
    volatile s32 pad[2];

    if (D_80156609 != 0) {
        func_80097C18(&transform, arg0->unk52);
        transform.basePos.x = arg0->pos.x;
        transform.basePos.y = arg0->pos.y;
        transform.basePos.z = arg0->pos.z;
        arg0->matrix = func_8004885C(&transform);
    }

    if (func_80049000(&arg0->pos) != 0) {
        if (arg0->matrix != NULL) {
            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(gRegionAllocPtr++, 0x02, func_80043040(D_80112144));
            gSPSegment(gRegionAllocPtr++, 0x03, func_80043040(D_80112146));
            gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            if (D_80121B50 != 8) {
                gSPDisplayList(gRegionAllocPtr++, D_2000910);
            } else {
                gSPDisplayList(gRegionAllocPtr++, D_2003218);
            }
        }
    }
}

void func_8006BDE4(RaceMovingEffect *arg0) {
    Vec3i sp24;
    s16 temp_v0;
    RaceMovingEffect *temp_a3 = arg0;

    temp_v0 = arg0->timer;
    if (temp_v0 != 0) {
        if (D_80121B56 == 0) {
            arg0->timer = temp_v0 - 1;
            func_80098590(&arg0->unk30, &temp_a3->velocity, &sp24);
            temp_a3->pos.x += sp24.x;
            temp_a3->pos.y += sp24.y;
            temp_a3->pos.z += sp24.z;
        }
        func_800483FC(&D_801248A4, func_8006BC68, temp_a3);
        return;
    }
    func_800716E4(temp_a3);
}

void func_8006BE90(RaceMovingEffect *arg0) {
    Vec3i sp2C;
    void *velocity;

    if (D_80121B56 == 0) {
        arg0->timer--;
        arg0->unk52 -= 0x2A;
        func_80097C18(arg0->unk30, arg0->unk52);

        velocity = &arg0->velocity;
        func_80098590(arg0->unk30, velocity, &sp2C);
        arg0->pos.x += sp2C.x;
        arg0->pos.y += sp2C.y;
        arg0->pos.z += sp2C.z;

        arg0->unk52 -= 0x2A;
        func_80097C18(arg0->unk30, arg0->unk52);
        func_80098590(arg0->unk30, velocity, &sp2C);
        arg0->pos.x += sp2C.x;
        arg0->pos.y += sp2C.y;
        arg0->pos.z += sp2C.z;

        if (arg0->timer == 0) {
            func_80071824(arg0, func_8006BDE4);
            arg0->timer = 0x38;
        }
    }

    func_800483FC(&D_801248A4, func_8006BC68, arg0);
}

void func_8006BFC0(RaceMovingEffect *arg0) {
    Vec3i sp1C;
    RaceMovingEffect *temp_a3 = arg0;

    if (D_80121B56 == 0) {
        arg0->timer--;
        func_80098590(&arg0->unk30, &temp_a3->velocity, &sp1C);
        temp_a3->pos.x += sp1C.x * 2;
        temp_a3->pos.y += sp1C.y * 2;
        temp_a3->pos.z += sp1C.z * 2;
        if (temp_a3->timer == 0) {
            func_80071824(temp_a3, func_8006BE90);
            temp_a3->timer = 0x18;
        }
    }
    func_800483FC(&D_801248A4, func_8006BC68, temp_a3);
}

void func_8006C088(RaceMovingEffect *arg0) {
    void *mtx;

    arg0->timer = 0x28;
    arg0->unk52 = D_800B9554[D_80121B50].angle;
    arg0->velocity.x = -0x200000;
    arg0->velocity.z = 0x400000;
    mtx = arg0->unk30;
    func_80097C18(mtx, arg0->unk52);
    func_80098590(mtx, &arg0->velocity, &arg0->pos);
    arg0->velocity.x = 0;
    arg0->velocity.z = -0x20000;
    arg0->pos.x += D_800B9540[D_80121B50].unk8.x;
    arg0->pos.y += D_800B9540[D_80121B50].unk8.y + 0x40000;
    arg0->pos.z += D_800B9540[D_80121B50].unk8.z;
    func_80071824(arg0, func_8006BFC0);
    func_8006BFC0(arg0);
}

// func_8006C1B4 best match: 99.309% (nonmatchings/func_8006C1B4-180949888360117632/base_19.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006C1B4.s")

#ifdef NON_MATCHING
void func_8006C1B4(Struct6C51C *arg0) {
    CourseEffectMatrixSource scratch;
    volatile s32 pad[2];
    void *matrix;

    if (D_80156609 != 0) {
        arg0->sourceMatrix = NULL;
        arg0->pos1Matrix = NULL;
        arg0->pos2Matrix = NULL;
    }

    if (func_80049000((Vec3i *) &D_800DA764[D_80121B50]) == 0) {
        return;
    }

    matrix = arg0->sourceMatrix;
    if (matrix == NULL) {
        arg0->sourceMatrix = func_8004885C(&arg0->source);
        matrix = arg0->sourceMatrix;
    }

    if (matrix != NULL) {
        gDPPipeSync(gRegionAllocPtr++);
        gSPSegment(gRegionAllocPtr++, 0x02, func_80043040(D_80112144));
        gSPSegment(gRegionAllocPtr++, 0x03, func_80043040(D_80112146));
        arg0++;
        arg0--;
        gSPMatrix(gRegionAllocPtr++, arg0->sourceMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_2001678);
    }

    matrix = arg0->pos1Matrix;
    if (matrix == NULL) {
        func_80098174(scratch.rotation, D_800DA770[D_80121B50].angle, arg0->unk50);
        scratch.basePos.x = arg0->pos1.x;
        scratch.basePos.y = arg0->pos1.y;
        scratch.basePos.z = arg0->pos1.z;
        arg0->pos1Matrix = func_8004885C(&scratch);
        matrix = arg0->pos1Matrix;
    }

    if (matrix != NULL) {
        gSPMatrix(gRegionAllocPtr++, arg0->pos1Matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_2001730);
    }

    matrix = arg0->pos2Matrix;
    if (matrix == NULL) {
        scratch = arg0->source;
        scratch.basePos.x = arg0->pos2.x;
        scratch.basePos.y = arg0->pos2.y;
        scratch.basePos.z = arg0->pos2.z;
        arg0->pos2Matrix = func_8004885C(&scratch);
        matrix = arg0->pos2Matrix;
    }

    if (matrix != NULL) {
        gSPMatrix(gRegionAllocPtr++, arg0->pos2Matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        if (arg0->unk56 == 0) {
            gSPDisplayList(gRegionAllocPtr++, D_2001810);
        } else {
            gSPDisplayList(gRegionAllocPtr++, D_20018E8);
        }
    }
}
#endif

void func_8006C4AC(Struct6C51C *arg0) {
    if (D_80121B56 == 0) {
        if (arg0->unk50 != 0) {
            arg0->unk50 += 0x80;
        } else {
            arg0->unk56 = 0;
            func_80071824(arg0, func_8006C5C0);
        }
    }
    func_800483FC(&D_801248A4, func_8006C1B4, arg0);
}

void func_8006C51C(Struct6C51C *arg0) {
    Struct6C51C *temp_s0 = arg0;
    s16 temp_v0;

    if (D_80121B56 == 0) {
        temp_v0 = arg0->unk50;
        if (temp_v0 != -0x400) {
            arg0->unk50 = temp_v0 - 0x40;
        }
        temp_s0->unk54--;
        if (temp_s0->unk54 == 0) {
            func_80072A74(0x1C, &D_800DA764[D_80121B50], 0x7F, 0x32);
            func_80071824(temp_s0, func_8006C4AC);
        }
    }
    func_800483FC(&D_801248A4, func_8006C1B4, temp_s0);
}

void func_8006C5C0(Struct6C51C *arg0) {
    if ((D_80121B56 == 0) && (D_801235B4 & 4)) {
        arg0->unk54 = 0x2D;
        D_801235B4 &= ~4;
        func_80071824(arg0, func_8006C51C);
        arg0->unk56 = 1;
        func_80072A74(0x16, &D_800DA764[D_80121B50], 0x7F, 0x32);
        func_80072A74(0x1B, &D_800DA764[D_80121B50], 0x7F, 0x32);
    }
    func_800483FC(&D_801248A4, func_8006C1B4, arg0);
}

void func_8006C698(Struct6C51C *arg0) {
    void *mtx;
    Vec3i sp28;

    mtx = arg0->source.rotation;
    func_80097C18(mtx, D_800DA764[D_80121B50].angle);
    arg0->source.basePos.x = D_800DA764[D_80121B50].x;
    arg0->source.basePos.y = D_800DA764[D_80121B50].y;
    arg0->source.basePos.z = D_800DA764[D_80121B50].z;
    sp28.x = 0x18000;
    sp28.y = 0x120000;
    sp28.z = -0x80000;
    func_80098590(mtx, &sp28, &arg0->pos1);
    arg0->pos1.x += arg0->source.basePos.x;
    arg0->pos1.y += arg0->source.basePos.y;
    arg0->pos1.z += arg0->source.basePos.z;
    sp28.x = 0;
    sp28.y = 0x120000;
    sp28.z = 0x50000;
    func_80098590(mtx, &sp28, &arg0->pos2);
    arg0->pos2.x += arg0->source.basePos.x;
    arg0->pos2.y += arg0->source.basePos.y;
    arg0->pos2.z += arg0->source.basePos.z;
    arg0->unk52 = 0;
    func_80071824(arg0, func_8006C5C0);
}

// func_8006C7F4 best match: 96.706% at nonmatchings/func_8006C7F4-2911448260736516995/base_10.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006C7F4.s")

#ifdef NON_MATCHING
void func_8006C7F4(RaceCourseMarkerEffect *arg0) {
    Gfx *gfx;
    s32 i;
    s16 vertexCount;
    volatile u8 pad[0x10];

    if (D_80156609 != 0) {
        arg0->vertices = func_80048594(arg0->vertexCount * sizeof(Vtx));
        if (arg0->vertices != NULL) {
            i = 0;
            if (arg0->vertexCount > 0) {
                do {
                    arg0->vertices[i] = arg0->baseVertices[i];
                    arg0->vertices[i].v.tc[1] += arg0->rotation;
                    arg0->vertices[i].v.tc[1] = arg0->vertices[i].v.tc[1];
                    i++;
                } while ((i < arg0->vertexCount) != 0);
            }
        }
    }

    if (arg0->vertices != NULL) {
        gfx = gRegionAllocPtr++;
        gfx->words.w0 = 0xE7000000;
        vertexCount = (unsigned int) ((((unsigned int) ((((1 << 11) + MAX(1, 0x20 / 16)) - 1) / MAX(1, 0x20 / 16))) & ((0x01 << 12) - 1)) << 0);
        gfx->words.w1 = 0;
        gfx = gRegionAllocPtr++;
        gfx->words.w0 = 0xBC000806;
        gfx->words.w1 = func_80043040(D_80112140);
        gfx = gRegionAllocPtr++;
        gfx->words.w0 = 0x01020040;
        gfx->words.w1 = (u32) D_800DEE50;
        gfx = gRegionAllocPtr++;
        gfx->words.w0 = 0x06000000;
        gfx->words.w1 = (u32) arg0->texturePtr;
        gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->texture, G_IM_FMT_CI, 0x20, 0x40,
                               0, G_TX_WRAP, G_TX_WRAP, 5, 6, G_TX_NOLOD, G_TX_NOLOD);
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette);
        gfx = gRegionAllocPtr++;
        vertexCount = arg0->vertexCount;
        gfx->words.w0 = (((vertexCount << 0xA) | ((vertexCount << 4) - 1)) & 0xFFFF) | 0x04000000;
        gfx->words.w1 = (u32) arg0->vertices;
        gfx = gRegionAllocPtr++;
        gfx->words.w0 = 0x06000000;
        gfx->words.w1 = (u32) arg0->palettePtr;
    }
}
#endif

void func_8006CB50(RaceCourseMarkerEffect *arg0) {
    arg0->rotation -= 0x40;
    arg0->rotation &= 0x7FF;
    if (arg0->useAltQueue != 0) {
        func_800483FC(&D_801248EC, func_8006C7F4, arg0);
    } else {
        func_800483FC(&D_801248A4, func_8006C7F4, arg0);
    }
}

void func_8006CBBC(RaceCourseMarkerEffect *arg0) {
    func_80045990(func_80043040(D_80112168),
                  D_800DA814[arg0->entryIndex].textureIndex,
                  &arg0->texture, &arg0->palette);
    arg0->baseVertices =
        (Vtx *) func_8004597C(func_80043040(D_80112140), (s32) D_800DA80C[arg0->entryIndex].baseVerticesInput);

    {
        CourseMarkerEntry *entry = &D_800DA804[arg0->entryIndex];

        arg0->vertexCount = entry->vertexCount;
        arg0->texturePtr = entry->texturePtr;
        arg0->palettePtr = entry->palettePtr;
        arg0->vertexCount = entry->vertexCount;
        arg0->useAltQueue = entry->flags & 1;
        arg0->unk3C = entry->flags & 2;
    }
    func_80071824(arg0, func_8006CB50);
}

void func_8006CCC0(RaceCourseTriggerEffect *arg0) {
    volatile s32 unused;
    CourseEffectMatrixSource transform;
    Gfx *gfx;

    if (D_80156609 != 0) {
        CourseTriggerEntry *entry = &D_800DA840[((volatile RaceCourseTriggerEffect *) arg0)->entryIndex];

        func_80097FE4(&transform, entry->pitch, entry->yaw);
        transform.basePos.x = D_800DA840[arg0->entryIndex].pos.x;
        transform.basePos.y = D_800DA840[arg0->entryIndex].pos.y;
        transform.basePos.z = D_800DA840[arg0->entryIndex].pos.z;
        arg0->matrix = func_8004885C(&transform);
    }

    if (arg0->matrix != NULL) {
        {
            Gfx *_g = gRegionAllocPtr++;

            _g->words.w0 = 0xE7000000;
            _g->words.w1 = 0;
        }
        {
            Gfx *_g = gRegionAllocPtr++;

            _g->words.w0 = 0xBC000806;
            _g->words.w1 = func_80043040(D_80112140);
        }
        {
            Gfx *_g = gRegionAllocPtr++;
            volatile s32 pad[2];

            _g->words.w0 = 0xBC000C06;
            _g->words.w1 = func_80043040(D_80112142);
        }
        {
            Gfx *_g = gRegionAllocPtr++;

            _g->words.w0 = 0x01020040;
            _g->words.w1 = (u32) arg0->matrix;
        }
        {
            Gfx *_g = gRegionAllocPtr++;

            _g->words.w0 = 0x06000000;
            _g->words.w1 = (u32) arg0->displayList;
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006CE68.s")

void func_8006D2D0(RaceCourseTriggerEffect *arg0) {
    if (D_80121D80[0].isActive != 0) {
        func_8006CE68(D_80121D80, arg0);
    }
    if (D_80121D80[1].isActive != 0) {
        func_8006CE68(D_8012238C, arg0);
    }
    if (D_80121D80[2].isActive != 0) {
        func_8006CE68(D_80122998, arg0);
    }
    if (D_80121D80[3].isActive != 0) {
        func_8006CE68(D_80122FA4, arg0);
    }
    func_800483FC(&D_801248A4, func_8006CCC0, arg0);
}

void func_8006D384(RaceCourseTriggerEffect *arg0) {
    CourseTriggerScratch scratch;
    CourseTriggerEntry *entry;

    entry = &D_800DA840[arg0->entryIndex];
    arg0->pitch = entry->pitch;
    arg0->yaw = entry->yaw;
    arg0->scaleX = entry->scaleX << 0x10;
    arg0->scaleY = entry->scaleY << 0x10;
    arg0->scaleZ = entry->scaleZ << 0x10;
    arg0->displayList = entry->displayList;

    func_80097FE4(scratch.mtx, arg0->pitch, arg0->yaw);

    scratch.source.y = 0;
    scratch.source.x = 0;
    scratch.source.z = arg0->scaleZ;
    func_80098590(scratch.mtx, &scratch.source, &scratch.dest);

    entry = &D_800DA840[arg0->entryIndex];
    arg0->pos1.x = entry->pos.x + scratch.dest.x;
    arg0->pos1.y = entry->pos.y + scratch.dest.y - 0x100000;
    arg0->pos1.z = entry->pos.z + scratch.dest.z;

    scratch.source.x = 0;
    scratch.source.y = 0;
    scratch.source.z = -arg0->scaleZ;
    func_80098590(scratch.mtx, &scratch.source, &scratch.dest);

    entry = &D_800DA840[arg0->entryIndex];
    arg0->pos2.x = entry->pos.x + scratch.dest.x;
    arg0->pos2.y = entry->pos.y + scratch.dest.y - 0x100000;
    arg0->pos2.z = entry->pos.z + scratch.dest.z;

    func_80071824(arg0, func_8006D2D0);
}
