#include "common.h"

#define RACE_MODEL_BUFFER_HANDLE (*(s16 *)&D_80112130[0x48])

typedef struct {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 z;
} Vec3i;

typedef struct {
    /* 0x0 */ s16 enabled;
    /* 0x2 */ s16 modelIndex;
    /* 0x4 */ Vec3i transform;
} RaceModelEntry;

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

typedef union {
    /* 0x00 */ s32 words[8];
    /* 0x00 */ s16 halfwords[0x10];
} GfxCommandSource;

typedef s16 FixedMatrix3sScratch[0x12];

typedef struct {
    /* 0x00 */ s16 modelIndex;
    char pad2[2];
    /* 0x04 */ Vec3i pos;
    /* 0x10 */ s16 assetIndex;
    char pad12[2];
} RaceOverlayModelEntry;

typedef struct {
    char pad0[0x10];
    /* 0x10 */ u16 modelListIndex;
    char pad12[6];
    /* 0x18 */ GfxCommandDest *modelBuffer;
    char pad1C[2];
    /* 0x1E */ s16 modelCount;
} RaceModelListActor;

typedef struct {
    char pad0[0x18];
    /* 0x18 */ GfxCommandDest *matrix;
    /* 0x1C */ Vec3i pos;
    /* 0x28 */ s16 pitch;
    /* 0x2A */ s16 yaw;
    char pad2C[2];
    /* 0x2E */ s8 matrixDirty;
} RaceOverlayTransformActor;

typedef struct {
    char pad0[0x1C];
    /* 0x1C */ Vec3i pos;
    char pad28[2];
    /* 0x2A */ s16 modelIndex;
    /* 0x2C */ s16 unk2C;
    char pad2E[2];
    /* 0x30 */ Vec3i transformedPos;
    /* 0x3C */ Vec3i velocity;
    char pad48[2];
    /* 0x4A */ s16 timer;
} RaceThrownModelActor;

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 timer;
    char pad1A[2];
    /* 0x1C */ Vec3i pos;
    /* 0x28 */ Vec3i drawPos;
    char pad34[0x18];
    /* 0x4C */ s32 unk4C;
    char pad50[0x10];
    /* 0x60 */ s32 velY;
} RaceOverlayModelActor;

typedef struct {
    char pad0[0x10];
    /* 0x10 */ u16 spawnOffsetIndex;
    char pad12[6];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Vec3i velocity;
    /* 0x30 */ s16 rotX;
    /* 0x32 */ s16 rotY;
    /* 0x34 */ s16 rotZ;
    /* 0x36 */ s16 rotVelX;
    /* 0x38 */ s16 rotVelY;
    /* 0x3A */ s16 rotVelZ;
    /* 0x3C */ s16 timer;
    char pad3E[2];
    /* 0x40 */ void *displayList;
    /* 0x44 */ s8 transformDirty;
    char pad45[3];
    /* 0x48 */ void *image;
    /* 0x4C */ void *palette;
} RaceParticleActor;

extern s16 func_80042D58(s32);
extern s32 func_80043040(s16);
extern s32 func_800430D0(void);
extern void func_80045990(s32, s32, void *, void *);
extern GfxCommandDest *func_8004885C(GfxCommandSource *);
extern void func_80048C90(GfxCommandDest *, Vec3i *);
extern s32 func_80049000(Vec3i *);
extern s32 func_80048E60(void *);
extern void func_80071824(void *task, void (*callback)());
extern void func_800716E4(void *);
extern void osWritebackDCache(void *, s32);
extern void func_80097C18(void *, s16);
extern void func_80097FE4(FixedMatrix3sScratch, s16, s16);
extern void func_80098590(void *, void *, void *);
extern void func_80088C80(void *, s32, s32, s32);
extern void func_80088294(void *, s32, s32, s32);
extern void *func_800711D0(void *, s32, s32);
extern void *func_80071408(void *, s32, s32);
extern void func_800483FC(void *, void *, void *);
extern Vec3i D_800D9BD8[];
extern RaceOverlayModelEntry *D_800D7754[];
extern void *D_800DA1C0[];
extern Gfx *D_800DA1F0;
extern s32 D_801248D4;
extern s32 D_801248B0;
extern s16 D_80112144;
extern s16 D_80112146;
extern s16 D_80112168;
extern u8 D_80156609;
extern u8 D_80121B56;
extern u8 D_80112130[];
extern RaceModelEntry *D_800D91E8[];
extern u32 D_800D9210[];
extern Gfx D_800D9D00[];
extern Gfx D_800D9D40[];
extern GfxCommandDest D_800DEE50;
extern Gfx *gRegionAllocPtr;
extern void *D_80156614;
extern s16 D_80121B50;
extern void func_80066E10(void);
extern void func_80067034(RaceModelListActor *);
extern void func_80067364(RaceThrownModelActor *);
extern void func_800674B4(RaceThrownModelActor *);
extern void func_800681A4(RaceOverlayModelActor *);
extern void func_80068CD4(RaceOverlayModelActor *);
extern void func_800684E4(RaceOverlayModelActor *);
extern void func_80068EA0(RaceOverlayModelActor *);
extern void func_8006935C(RaceParticleActor *);
extern void func_80069678(RaceParticleActor *);

typedef struct Scratch674B4 {
    char scratch[0x28];
    s32 pad;
} Scratch674B4;

void func_80066760(RaceModelListActor *arg0) {
    RaceOverlayModelEntry *var_s4;
    s32 var_s5;
    s32 var_s7;
    Gfx *temp_s0;
    Gfx *temp_s2;
    Gfx *temp_s3;

    var_s4 = D_800D7754[arg0->modelListIndex];
    var_s7 = TRUE;
    var_s5 = 0;
    if (var_s4->modelIndex != -1) {
        do {
            if (func_80049000(&var_s4->pos) != 0) {
                if (var_s7 != 0) {
                    gDPPipeSync(gRegionAllocPtr++);
                    temp_s2 = gRegionAllocPtr++;
                    var_s7 = FALSE;
                    gSPSegment(temp_s2, 0x02, func_80043040(*(s16 *)&D_80112130[0x14]));

                    temp_s3 = gRegionAllocPtr++;
                    gSPSegment(temp_s3, 0x03, func_80043040(*(s16 *)&D_80112130[0x16]));
                }

                temp_s0 = gRegionAllocPtr++;
                gDma1p(temp_s0, 1, (u32)arg0->modelBuffer + (var_s5 << 6), 0x40, 2);

                temp_s0 = gRegionAllocPtr++;
                gSPDisplayList(temp_s0, D_800DA1C0[var_s4->modelIndex]);
            }
            var_s4++;
            var_s5++;
        } while (var_s4->modelIndex != -1);
    }
}

void func_800668EC(RaceModelListActor *arg0) {
    RaceOverlayModelEntry *entry;
    void *pos;

    entry = D_800D7754[arg0->modelListIndex];
    if (entry->modelIndex != -1) {
        pos = &entry->pos;
        do {
            if (func_80048E60(pos) != 0) {
                func_80088294(pos, 0x1C0000, 0x480000, 2);
            }
            entry++;
            pos = &entry->pos;
        } while (entry->modelIndex != -1);
    }
    func_800483FC(&D_801248B0, func_80066760, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_overlay_effects/func_800669A0.s")

// func_80066ABC best match: 96.831%
#pragma GLOBAL_ASM("asm/nonmatchings/race_overlay_effects/func_80066ABC.s")

#ifdef NON_MATCHING
void func_80066ABC(RaceModelListActor *arg0) {
    RaceModelEntry *entry;
    s16 modelIndex;
    s32 i;
    register RaceModelListActor *actor;
    Gfx *temp_v0;
    void *spA0;
    void *sp9C;
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

    actor = arg0;
    temp_v0 = gRegionAllocPtr++;
    temp_v0->words.w1 = (u32)D_800D9D00;
    temp_v0->words.w0 = 0x06000000;

    modelIndex = -1;
    entry = D_800D91E8[D_80121B50];
    i = 0;
    if (entry->modelIndex != -1) {
        do {
            if ((entry->enabled != 0) && (func_80049000(&entry->transform) != 0)) {
                if (modelIndex != entry->modelIndex + actor->modelIndexOffset) {
                    modelIndex = entry->modelIndex + actor->modelIndexOffset;
                    func_80045990(func_80043040(D_80112168), (modelIndex + 4) & 0xFFFF, &spA0, &sp9C);

                    temp_v0_2 = gRegionAllocPtr++;
                    temp_v0_2->words.w0 = 0xFD500000;
                    temp_v0_2->words.w1 = (u32)spA0;

                    temp_v0_3 = gRegionAllocPtr++;
                    temp_v0_3->words.w1 = 0x07080200;
                    temp_v0_3->words.w0 = 0xF5500000;

                    temp_v0_4 = gRegionAllocPtr++;
                    temp_v0_4->words.w1 = 0;
                    temp_v0_4->words.w0 = 0xE6000000;

                    temp_v0_5 = gRegionAllocPtr++;
                    temp_v0_5->words.w1 = 0x0703F800;
                    temp_v0_5->words.w0 = 0xF3000000;

                    temp_v0_6 = gRegionAllocPtr++;
                    temp_v0_6->words.w1 = 0;
                    temp_v0_6->words.w0 = 0xE7000000;

                    temp_v0_7 = gRegionAllocPtr++;
                    temp_v0_7->words.w0 = 0xF5400200;
                    temp_v0_7->words.w1 = 0x00080200;

                    temp_v0_8 = gRegionAllocPtr++;
                    temp_v0_8->words.w0 = 0xF2000000;
                    temp_v0_8->words.w1 = 0x0003C03C;

                    temp_v0_9 = gRegionAllocPtr++;
                    temp_v0_9->words.w0 = 0xFD100000;
                    temp_v0_9->words.w1 = (u32)sp9C;

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
                    temp_v0_13->words.w1 = 0x0703C000;
                    temp_v0_13->words.w0 = 0xF0000000;

                    temp_v0_14 = gRegionAllocPtr++;
                    temp_v0_14->words.w1 = 0;
                    temp_v0_14->words.w0 = 0xE7000000;
                }

                gSPMatrix(gRegionAllocPtr++, &actor->modelBuffer[i], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, D_80156614, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

                temp_v0_15 = gRegionAllocPtr++;
                temp_v0_15->words.w1 = (u32)D_800D9210;
                temp_v0_15->words.w0 = 0x0400103F;

                temp_v0_16 = gRegionAllocPtr++;
                temp_v0_16->words.w1 = 0x00060200;
                temp_v0_16->words.w0 = 0xB1060402;
            }
            entry++;
            i++;
        } while (entry->modelIndex != -1);
    }

    temp_v0_19 = gRegionAllocPtr++;
    temp_v0_19->words.w1 = (u32)D_800D9D40;
    temp_v0_19->words.w0 = 0x06000000;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/race_overlay_effects/func_80066E10.s")

void func_80067034(RaceModelListActor *arg0) {
    register RaceModelListActor *actor1;
    register RaceModelListActor *actor2;
    register RaceModelEntry *script;
    register s32 i;
    register s32 offset;
    register s32 one;

    script = D_800D91E8[D_80121B50];
    actor1 = arg0;
    actor2 = arg0;
    i = 0;
    if (actor1->modelCount > 0) {
        register GfxCommandDest *template;

        template = &D_800DEE50;
        offset = 0;
        one = 1;
        do {
            script->enabled = one;
            actor1->modelBuffer[i] = *template;
            func_80048C90(&actor1->modelBuffer[i], &script->transform);
            i++;
            offset += sizeof(GfxCommandDest);
            script++;
        } while (i < actor2->modelCount);
    }
    osWritebackDCache(actor1->modelBuffer, actor1->modelCount * sizeof(GfxCommandDest));
}

void func_8006713C(RaceModelListActor *arg0) {
    RaceModelListActor *new_var;
    RaceModelEntry *var_v0;

    new_var = arg0;
    new_var->modelCount = 0;
    var_v0 = D_800D91E8[D_80121B50];
    if (var_v0->modelIndex != -1) {
        do {
            new_var->modelCount += 1;
            var_v0++;
        } while (var_v0->modelIndex != -1);
    }
    if (new_var->modelCount != 0) {
        RACE_MODEL_BUFFER_HANDLE = func_80042D58(new_var->modelCount << 6);
        new_var->modelBuffer = (void *) func_80043040(RACE_MODEL_BUFFER_HANDLE);
        func_80067034(new_var);
        func_80071824(new_var, func_80066E10);
    }
}

void func_800671F4(RaceOverlayTransformActor *arg0) {
    FixedMatrix3sScratch scratch;

    if (D_80156609 != 0) {
        arg0->matrixDirty = 1;
    }

    if (func_80049000(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            func_80097FE4(scratch, arg0->pitch, arg0->yaw);
            ((GfxCommandSource *)scratch)->words[5] = arg0->pos.x;
            ((GfxCommandSource *)scratch)->words[6] = arg0->pos.y + 0x190000;
            ((GfxCommandSource *)scratch)->words[7] = arg0->pos.z;
            arg0->matrix = func_8004885C((GfxCommandSource *)scratch);
            arg0->matrixDirty = 0;
        }

        if (arg0->matrix != NULL) {
            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(gRegionAllocPtr++, 0x02, func_80043040(D_80112144));
            gSPSegment(gRegionAllocPtr++, 0x03, func_80043040(D_80112146));
            gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gRegionAllocPtr++, D_800DA1F0);
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_overlay_effects/func_80067364.s")

void func_800674B4(RaceThrownModelActor *arg0) {
    Scratch674B4 sp1C;
    RaceThrownModelActor *temp_a3 = arg0;

    if (D_80121B56 == 0) {
        func_80097C18(sp1C.scratch, temp_a3->modelIndex);
        temp_a3->timer = 0x32;
        temp_a3->velocity.x = 0;
        temp_a3->velocity.y = 0xB0000;
        temp_a3->velocity.z = 0xFFF90000;
        func_80098590(sp1C.scratch, &temp_a3->velocity, &temp_a3->transformedPos);
        func_80071824(temp_a3, func_80067364);
    }
}

void func_8006752C(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4) {
    RaceThrownModelActor *temp = func_80071408(func_800674B4, 0, 0x64);

    if (temp != NULL) {
        temp->pos.x = arg0;
        temp->pos.y = arg1;
        temp->pos.z = arg2;
        temp->modelIndex = arg3;
        temp->unk2C = arg4;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_overlay_effects/func_800675AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_overlay_effects/func_80067830.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_overlay_effects/func_800681A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_overlay_effects/func_800684E4.s")

void func_80068BF0(RaceOverlayModelActor *arg0) {
    s16 temp_v0;
    s32 temp_v1;
    RaceOverlayModelActor *temp_s0 = arg0;
    void *temp_s1;

    if (D_80121B56 == 0) {
        arg0->timer--;
        temp_v0 = arg0->timer;
        temp_v1 = arg0->pos.y - (temp_v0 * 0x14000);
        arg0->drawPos.y = temp_v1 + 0x140000;
        arg0->unk4C = temp_v1;
        if (temp_v0 == 0) {
            func_80071824(arg0, func_80068EA0);
        }
        temp_s1 = &temp_s0->pos;
        func_80088C80(temp_s1, 0xC0000, 0x180000, 0);
        func_80088C80(temp_s1, 0xC0000, 0x180000, 1);
        func_80088C80(temp_s1, 0xC0000, 0x180000, 2);
        func_80088C80(temp_s1, 0xC0000, 0x180000, 3);
    }
    func_800483FC(&D_801248D4, func_800684E4, temp_s0);
}

void func_80068CD4(RaceOverlayModelActor *arg0) {
    if (D_80121B56 == 0) {
        arg0->drawPos.y += arg0->velY;
        arg0->velY += 0xFFFF0000;

        if (arg0->drawPos.y < arg0->pos.y) {
            arg0->drawPos.y = arg0->pos.y;
            arg0->timer = 0x10;
            func_80071824(arg0, func_80068BF0);
        }

        func_80088C80(&arg0->pos, 0xC0000, 0x180000, 0);
        func_80088C80(&arg0->pos, 0xC0000, 0x180000, 1);
        func_80088C80(&arg0->pos, 0xC0000, 0x180000, 2);
        func_80088C80(&arg0->pos, 0xC0000, 0x180000, 3);
    }
    func_800483FC(&D_801248D4, func_800681A4, arg0);
}

void func_80068DB4(RaceOverlayModelActor *arg0) {
    s32 temp_a2;
    s32 temp_v0;
    s32 var_v1;
    void *temp_s1;

    if (D_80121B56 == 0) {
        temp_v0 = arg0->velY;
        temp_a2 = arg0->pos.y;
        var_v1 = (arg0->drawPos.y += temp_v0);
        arg0->velY = temp_v0 + 0xFFFF0000;
        if (var_v1 < temp_a2) {
            arg0->drawPos.y = temp_a2;
            arg0->velY = 0x30000;
            func_80071824(arg0, func_80068CD4);
            var_v1 = arg0->drawPos.y;
        }
        temp_s1 = &arg0->drawPos;
        if (var_v1 < 0) {
            func_80088C80(temp_s1, 0xC0000, 0x180000, 0);
            func_80088C80(temp_s1, 0xC0000, 0x180000, 1);
            func_80088C80(temp_s1, 0xC0000, 0x180000, 2);
            func_80088C80(temp_s1, 0xC0000, 0x180000, 3);
        }
    }
    func_800483FC(&D_801248D4, func_800681A4, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_overlay_effects/func_80068EA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_overlay_effects/func_800691C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_overlay_effects/func_8006935C.s")

void func_80069678(RaceParticleActor *arg0) {
    s32 temp_v0;
    RaceParticleActor *temp_a2 = arg0;

    if (D_80121B56 == 0) {
        arg0->timer--;
    }
    if (temp_a2->timer != 0) {
        if (D_80121B56 == 0) {
            temp_v0 = temp_a2->velocity.y;
            temp_a2->pos.x += temp_a2->velocity.x;
            temp_a2->pos.y += temp_v0;
            temp_a2->pos.z += temp_a2->velocity.z;
            temp_a2->velocity.y = temp_v0 + 0xFFFF0000;
            temp_a2->rotX += temp_a2->rotVelX;
            temp_a2->rotY += temp_a2->rotVelY;
            temp_a2->rotZ += temp_a2->rotVelZ;
        }
        func_800483FC(&D_801248D4, func_8006935C, temp_a2);
        return;
    }
    func_800716E4(temp_a2);
}

void func_80069754(RaceParticleActor *arg0) {
    char sp28[0x20];

    arg0->timer = 0xA;
    arg0->rotVelX = func_800430D0() - 0x80;
    arg0->rotVelY = func_800430D0() - 0x80;
    arg0->rotVelZ = func_800430D0() - 0x80;
    func_80097C18(sp28, arg0->rotY);
    func_80098590(sp28, &D_800D9BD8[arg0->spawnOffsetIndex], &arg0->velocity);
    func_80045990(func_80043040(D_80112168), 0x22, &arg0->palette, &arg0->image);
    func_80071824(arg0, func_80069678);
}

void func_80069808(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4) {
    RaceParticleActor *temp = func_800711D0(func_80069754, 5, 0x3B);

    if (temp != NULL) {
        temp->spawnOffsetIndex = arg4;
        temp->pos.x = arg0;
        temp->pos.y = arg1;
        temp->pos.z = arg2;
        temp->rotY = arg3;
    }
}
