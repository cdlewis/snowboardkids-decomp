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
    char pad[0x18];
    volatile s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    char pad34[4];
    s32 unk38;
    s16 unk3C;
    s16 unk3E;
    char pad40[0xC];
    s16 unk4C;
    s16 unk4E;
    s32 unk50;
} Struct6B760;

typedef struct {
    char pad0[0x18];
    char mtx[0x14];
    Vec3i basePos;
    Vec3i pos1;
    Vec3i pos2;
    s16 unk50;
    s16 unk52;
    s16 unk54;
    s16 unk56;
} Struct6C51C;

typedef struct {
    char pad0[0x18];
    Vec3i velocity;
    Vec3i pos;
    char unk30[0x20];
    s16 timer;
    s16 unk52;
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
    char pad2[0x46];
} CourseAngleEntry;

typedef struct {
    char pad0[0x10];
    u16 entryIndex;
    char pad12[6];
    void *texture;
    void *palette;
    s16 rotation;
    char pad22[2];
    void *vertices;
    void *baseVertices;
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
    void *vertices;
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

extern void func_80071824(void *task, void (*callback)());
extern void func_800483FC(void *, void *, void *);
extern void func_800716E4(void *);
extern void func_80072138(s32, s32);
extern void func_80072A74(s32, void *, s32, s32);
extern void func_8006AF48(void);
extern void func_8006A80C(void *);
extern void func_80069BEC(void);
extern void func_80069E50(void);
extern s32 func_80043040(s16);
extern void func_80045990(s32, s32, void *, void *);
extern s32 func_8004597C(s32, s32);
extern void func_80047174(s32, s32, s32, s32, s32);
extern void func_80045A78(s32, s32, s32, s32);
extern void func_80097C18(void *, s16);
extern void func_80097FE4(void *, s16, s16);
extern void func_80098590(void *, void *, Vec3i *);
extern s16 func_8004940C(s32, s32, s32, s32);
extern s32 func_80080CC4(s16, s32, s32);
extern s16 D_80112168;
extern s16 D_80112140;
extern s32 D_801235B4;
extern u8 D_80156608;
extern void func_8006C5C0(Struct6C51C *);
extern void func_8006C1B4(void);
extern void func_8006D2D0(RaceCourseTriggerEffect *);
void func_80069890(RaceCountdownEffect *);
void func_80069914(RaceCountdownEffect *);
void func_80069998(RaceCountdownEffect *);
void func_800699F0(RaceCountdownEffect *);
void func_80069A78(RaceCountdownEffect *);
void func_80069AF0(RaceCountdownEffect *);
void func_80069B60(RaceCountdownEffect *);
extern void func_8006C7F4(void);
void func_8006B3E0(Struct6B760 *);
void func_8006B6C8(Struct6B760 *);
extern Struct6B760 *func_80071408(void *, s32, s32);
extern u8 D_80121B56;
extern s16 D_80121B50;
extern SoundParams D_800DA764[];
extern CourseSpawnEntry D_800B9540[];
extern CourseAngleEntry D_800B9554[];
extern s16 D_800B9556[];
extern CourseMarkerEntry D_800DA804[];
extern CourseMarkerVertexResource D_800DA80C[];
extern CourseMarkerTextureResource D_800DA814[];
extern CourseTriggerEntry D_800DA840[];
extern s32 D_80124868;
extern s32 D_80124878;
extern s32 D_801248EC;
extern s32 D_801248B0;
extern s32 D_801248A4;
extern s32 D_801248F8;
extern void func_8006BC68(void *);
extern void func_8006BE90(void);
extern void func_8006B7E0(void);

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

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_80069BEC.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006ACE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006AE00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006AF48.s")

void func_8006B0D8(void *arg0) {
    func_800483FC(&D_801248B0, func_8006AF48, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006B108.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006B228.s")

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
        arg0->unk18 = temp24;
        temp4C = 0x10;
        temp50 = 0x10000;
        temp4E = 0x80;
        arg0->unk3E = angle;
        arg0->unk4C = temp4C;
        arg0->unk50 = temp50;
        arg0->unk4E = temp4E;
        arg0->unk1C = temp28;
        arg0->unk20 = temp2C;
        arg0->unk1C = func_80080CC4(arg0->unk3C, arg0->unk18, arg0->unk20);
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

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006B7E0.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006BA50.s")

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
    func_80097C18(mtx, D_800B9556[tempIndex * 0x24] + 0x400);
    func_80098590(mtx, &arg0->velocity, &arg0->pos);
    arg0->velocity.z = 0xFFFE0000;
    arg0->pos.x += ((CourseSpawnEntry *)((u8 *)D_800B9540 + ((*(volatile s16 *)&D_80121B50) * 0x48)))->pos.x;
    arg0->pos.y += ((CourseSpawnEntry *)((u8 *)D_800B9540 + ((*(volatile s16 *)&D_80121B50) * 0x48)))->pos.y;
    arg0->pos.z += ((CourseSpawnEntry *)((u8 *)D_800B9540 + (*courseIndex * 0x48)))->pos.z;
    func_80071824(arg0, func_8006BA50);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006BC68.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006BE90.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006C1B4.s")

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

    mtx = arg0->mtx;
    func_80097C18(mtx, D_800DA764[D_80121B50].angle);
    arg0->basePos.x = D_800DA764[D_80121B50].x;
    arg0->basePos.y = D_800DA764[D_80121B50].y;
    arg0->basePos.z = D_800DA764[D_80121B50].z;
    sp28.x = 0x18000;
    sp28.y = 0x120000;
    sp28.z = -0x80000;
    func_80098590(mtx, &sp28, &arg0->pos1);
    arg0->pos1.x += arg0->basePos.x;
    arg0->pos1.y += arg0->basePos.y;
    arg0->pos1.z += arg0->basePos.z;
    sp28.x = 0;
    sp28.y = 0x120000;
    sp28.z = 0x50000;
    func_80098590(mtx, &sp28, &arg0->pos2);
    arg0->pos2.x += arg0->basePos.x;
    arg0->pos2.y += arg0->basePos.y;
    arg0->pos2.z += arg0->basePos.z;
    arg0->unk52 = 0;
    func_80071824(arg0, func_8006C5C0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006C7F4.s")

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
        func_8004597C(func_80043040(D_80112140), (s32) D_800DA80C[arg0->entryIndex].baseVerticesInput);

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

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006CCC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006CE68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/func_8006D2D0.s")

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
