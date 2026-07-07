#include "common.h"
#include "viewport_manager.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x2FC];
    /* 0x2FC */ s32 flags;
    /* 0x300 */ u8 pad300[0x51A - 0x300];
    /* 0x51A */ u8 unk51A;
    /* 0x51B */ u8 pad51B[0x60C - 0x51B];
} RacePlayerState;

typedef union {
    s32 word;
    struct {
        s16 hi;
        s16 lo;
    } half;
    struct {
        u8 b0;
        u8 b1;
        u8 b2;
        u8 b3;
    } byte;
} OverlayActorWord;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ OverlayActorWord unk18;
    /* 0x1C */ OverlayActorWord unk1C;
    /* 0x20 */ OverlayActorWord unk20;
    /* 0x24 */ s32 velocity;
    /* 0x28 */ u16 timer;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ u8 pad2C[2];
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F;
    /* 0x30 */ OverlayActorWord unk30;
    /* 0x34 */ s16 spriteIndex;
    /* 0x36 */ s16 alpha;
} MainMenuOverlayEffectActor;

extern Gfx *gRegionAllocPtr;
extern void *D_80124868;
extern u8 D_80124858[];
extern u8 D_80124878[];
extern u8 D_801248A4[];
extern u8 D_801248D4[];
extern u8 D_801248F8[];
extern s16 D_800D5738[];
extern u8 D_800D5730[];
extern u8 D_800D5744[];
extern s32 D_800D5748[];
extern u8 D_800D57A0[];
extern s16 D_800D57B0[];
extern s16 D_800D57B4[];
extern s16 D_800D5808[];
extern u32 D_800DEE50[];
extern s16 D_800DEF14;
extern u32 D_800DEFF8[];
extern s16 D_8011213C;
extern s16 D_80112140;
extern s16 D_80112142;
extern s16 D_8011214C;
extern s16 D_80112154;
extern s16 D_80112168;
extern s16 D_8011216A;
extern s16 D_8011216E;
extern s16 D_80112174;
extern s16 D_80112184;
extern s16 D_80121B50;
extern s16 D_801235B0;
extern RacePlayerState D_80121D80[];
extern u8 D_80156608;
extern u8 D_800E29C0;
extern u8 D_800E11F0[];
extern u8 D_800E1204[];
extern u32 D_2000E70[];
extern u32 D_20058A8[];

s32 func_80043040(s16);
void func_8000F030(s32, s32, s32, s32, s32, s32, s32, s32);
void func_80041D20(s32, u16);
void func_80041DD4(s32, u8);
void func_80042034(s32);
void func_8004209C(s32, s32, s32, s32);
void func_800420FC(s32, s32, s32, s32);
void func_800428C8(s32);
void func_80042A00(s32);
void func_80042A58(s32, s32);
void func_80045A78(s16, s16, s32, u16);
void func_80046D68(s32, s32, s32, s32, s32);
void func_80048278(s32, s32, void *, s32);
void func_800483FC(void *, void *, s32);
void func_80053B28(void);
void func_80053C90(void *);
void func_80053D8C(s32);
void func_80053DFC(s32);
void func_8005408C(MainMenuOverlayEffectActor *);
void func_80054460(void);
void func_80054644(MainMenuOverlayEffectActor *);
void func_8005475C(MainMenuOverlayEffectActor *);
void func_800548F4(MainMenuOverlayEffectActor *);
void func_8005499C(MainMenuOverlayEffectActor *);
void func_80054A04(MainMenuOverlayEffectActor *);
void func_80054A64(MainMenuOverlayEffectActor *);
void func_80054AC0(MainMenuOverlayEffectActor *);
void func_80054B98(void);
void func_80054EC4(MainMenuOverlayEffectActor *);
void func_80055148(void);
void func_8005537C(MainMenuOverlayEffectActor *);
void func_80055410(void);
void func_80055530(void *);
s32 func_80097AE8(s16);
MainMenuOverlayEffectActor *func_80071408(void (*callback)(MainMenuOverlayEffectActor *), s32 type, s32 priority);
void func_800716E4(void *);
void *func_80071664(void *, s32, s32, s32);
void func_80071824(void *task, void (*callback)());
void func_80072138(s16, s32, void *);

void func_80053660(MainMenuOverlayEffectActor *arg0) {
    if (D_80156608 == arg0->index) {
        func_8000F030((s16)(arg0->unk18.half.hi >> 4), (s16)(arg0->unk18.half.lo >> 4), func_80043040(D_80112184), (arg0->unk1C.half.hi + 2) & 0xFFFF,
                      arg0->timer, arg0->timer, 0, arg0->unk2A + 2);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_overlay_effects/func_800536F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_overlay_effects/func_80053858.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_overlay_effects/func_8005393C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_overlay_effects/func_80053B28.s")

// func_80053C90 best match: 99.677% at nonmatchings/func_80053C90-2/output-20-1/source.c
void func_80053C90(void *arg0) {
    if (D_80156608 == 0) {
        gDPPipeSync(gRegionAllocPtr++);

        gSPSegment(gRegionAllocPtr++, 0x02, func_80043040(D_8011214C));
        gSPSegment(gRegionAllocPtr++, 0x03, func_80043040(D_80112154));

        gSPMatrix(gRegionAllocPtr++, D_800DEE50, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        gSPDisplayList(gRegionAllocPtr++, D_20058A8);
    }
}

void func_80053D8C(s32 arg0) {
    func_800483FC(D_801248F8, func_80053C90, arg0);
    func_800483FC(D_801248A4, func_80053B28, arg0);
}

void func_80053DD8(void *arg0) {
    func_80071824(arg0, func_80053D8C);
}

void func_80053DFC(s32 arg0) {
    func_80042034(0);
    func_80042A00(0);
}

void func_80053E28(MainMenuOverlayEffectActor *arg0) {
    if (D_800DEF14 == 0) {
        if (arg0->unk18.byte.b0 != 0) {
            func_80072138(D_800D5738[arg0->index], 0x32, arg0);
            arg0->unk18.byte.b0 = 0;
        }
        func_80042034(0);
        func_80071824(arg0, func_80053DFC);
        func_80042A00(0);
        return;
    }
    func_800428C8(0);
}

void func_80053EBC(MainMenuOverlayEffectActor *arg0) {
    arg0->unk18.byte.b0 = 1;
    func_80041D20(0, arg0->index);
    func_80041DD4(0, D_800D5730[arg0->index]);
    func_80042034(0);
    func_8004209C(0, 0, 0, 0);
    func_800420FC(0, 0, 0, 0);
    func_80071824(arg0, func_80053E28);
}

void func_80053F50(MainMenuOverlayEffectActor *arg0) {
    s16 temp = arg0->unk1C.half.hi;

    if (temp == 0) {
        func_80042034(arg0->unk18.word);
    } else {
        arg0->unk1C.half.hi = temp - 1;
    }
    func_80042A58(arg0->unk18.word, 1);
}

void func_80053FA0(MainMenuOverlayEffectActor *arg0) {
    s32 a = arg0->unk18.word;
    arg0->unk1C.half.hi = a * 0xF;
    func_80041D20(a, arg0->index);
    func_80041DD4(arg0->unk18.word, D_800D5744[arg0->index]);
    func_80042034(arg0->unk18.word);
    {
        s32 b = arg0->unk18.word;
        func_8004209C(b, D_800D5748[b], 0xFFB50000, 0xA00000);
    }
    func_800420FC(arg0->unk18.word, 0, 0x800, 0);
    func_80071824(arg0, func_80053F50);
}

void func_80054044(s32 arg0, s32 arg1) {
    MainMenuOverlayEffectActor *p = func_80071664(func_80053FA0, 0, 0x64, arg1);
    if (p != NULL) {
        p->unk18.word = arg0;
    }
}

void func_8005408C(MainMenuOverlayEffectActor *arg0) {
    if (D_800E29C0 != 0) {
        func_80048278(-0x44, 0x38, D_800E11F0, 0);
        return;
    }
    func_80048278(-0x5C, 0x38, D_800E1204, 1);
}

void func_800540EC(void *arg0) {
    if (D_801235B0 & 8) {
        func_800483FC(D_80124858, func_8005408C, 0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_overlay_effects/func_80054130.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_overlay_effects/func_80054460.s")

void func_800545D0(MainMenuOverlayEffectActor *arg0) {
    RacePlayerState *player = &D_80121D80[arg0->index];
    if (!(player->flags & 0x2000)) {
        if (player->unk51A != 0) {
            arg0->unk2E = 0xFF;
            func_80071824(arg0, func_80054460);
        }
    }
}

void func_80054644(MainMenuOverlayEffectActor *arg0) {
    if ((D_801235B0 & 0x3E) >= 0x1F) {
        func_80045A78(arg0->unk18.half.hi, arg0->unk18.half.lo, func_80043040(D_8011213C), 1);
    } else {
        func_80045A78(arg0->unk18.half.hi, arg0->unk18.half.lo, func_80043040(D_8011213C), D_800D57A0[(D_801235B0 & 0x1E) >> 1]);
    }
}

void func_800546E4(s32 arg0) {
    func_800483FC(&D_80124868, func_80054644, arg0);
}

void func_80054714(MainMenuOverlayEffectActor *arg0) {
    s16 *temp = &D_800D57B0[arg0->index * 2];

    arg0->unk18.half.hi = temp[0];
    arg0->unk18.half.lo = temp[1];
    func_80071824(arg0, func_800546E4);
}

void func_8005475C(MainMenuOverlayEffectActor *arg0) {
    Gfx *gfx;

    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    /* IDO scheduling for this function depends on this block staying on one line. */
    do { gfx->words.w0 = 0xE7000000; gfx->words.w1 = 0; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFC119623; gfx->words.w1 = 0xFF2FFFFF; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xB900031D; gfx->words.w1 = 0x00504240; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->unk18.half.lo & 0xFF) | (~0xFF); func_80045A78(-0x54, -0x10, func_80043040(D_80112168), 0x5D); func_80045A78(4, -0x10, func_80043040(D_80112168), 0x5E); if (arg0) { } func_80045A78(-0x4C, -0xC, func_80043040(D_8011216A), D_800D57B4[D_80121B50]); func_80045A78(4, -0xC, func_80043040(D_8011216A), D_800D57B4[D_80121B50] + 1); gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0x06000000; gfx->words.w1 = (u32) D_800DEFF8; } while (0);
}

void func_800548F4(MainMenuOverlayEffectActor *arg0) {
    s16 v = arg0->unk18.half.hi;
    if (v != 0) {
        arg0->unk18.half.hi = v - 1;
    } else {
        arg0->unk18.half.lo -= 0x20;
        if (arg0->unk18.half.lo <= 0) {
            func_800716E4(arg0);
            return;
        }
    }
    func_800483FC(&D_80124868, func_8005475C, arg0);
}

void func_80054968(MainMenuOverlayEffectActor *arg0) {
    arg0->unk18.half.hi = 0x3E;
    arg0->unk18.half.lo = 0xFF;
    func_80071824(arg0, func_800548F4);
}

void func_8005499C(MainMenuOverlayEffectActor *arg0) {
    u8 *new_var;

    new_var = &D_80156608;
    if (arg0->index == *new_var) {
        func_80045A78((s16)((arg0->unk18.word >> 1) - 0x38), -8, func_80043040(D_8011216E), 0x76);
    }
}

void func_80054A04(MainMenuOverlayEffectActor *arg0) {
    s32 temp_v0 = arg0->velocity;
    s32 temp_t8 = (unsigned long long)(temp_v0 + 4);

    arg0->unk18.word += temp_v0;
    arg0->velocity = temp_t8;
    if (temp_t8 == 0x38) {
        func_800716E4(arg0);
    } else {
        func_800483FC(D_80124878, func_8005499C, (s32)arg0);
    }
}

void func_80054A64(MainMenuOverlayEffectActor *arg0) {
    u16 temp = (arg0->timer & 0xFFFFU) - 1;

    arg0->timer = temp;
    temp += 0;
    if (temp == 0) {
        func_80071824(arg0, func_80054A04);
    }
    if (1) {
    }
    func_800483FC(D_80124878, func_8005499C, (s32)arg0);
}

void func_80054AC0(MainMenuOverlayEffectActor *arg0) {
    arg0->unk18.word += arg0->velocity;
    arg0->velocity -= 4;
    if (arg0->velocity == 0) {
        arg0->timer = 0x1E;
        func_80071824(arg0, func_80054A64);
    }
    func_800483FC(D_80124878, func_8005499C, (s32)arg0);
}

void func_80054B2C(MainMenuOverlayEffectActor *arg0) {
    if (D_801124B0[arg0->index].active != 0) {
        arg0->velocity = 0x38;
        arg0->unk18.word = -0x1A4;
        func_80071824(arg0, func_80054AC0);
        return;
    }
    func_800716E4(arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_overlay_effects/func_80054B98.s")

void func_80054E70(MainMenuOverlayEffectActor *arg0) {
    arg0->alpha -= 0x20;
    if (arg0->alpha == 0) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(D_801248D4, func_80054B98, (s32)arg0);
}

// func_80054EC4 best match: 99.444%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_overlay_effects/func_80054EC4.s")

#ifdef NON_MATCHING
void func_80054EC4(MainMenuOverlayEffectActor *arg0) {
    MainMenuOverlayEffectActor *actor;

    arg0->unk1C.word = ((-0x400000LL * func_80097AE8((s16)(arg0->unk30.word + 0x400))) / 0x1000) + 0x700000;
    arg0->unk20.word = ((-0x9F0000LL * arg0->unk30.word) / 0x400) + 0xEC0000;
    arg0->spriteIndex = (0x5000LL * func_80097AE8(arg0->unk30.half.lo)) / 0x1000;
    if (arg0->unk30.word < 0x3F0) {
        actor = func_80071408(func_80054E70, 0, 0x65);
        actor->unk18 = arg0->unk18;
        actor->unk1C = arg0->unk1C;
        actor->unk20 = arg0->unk20;
        actor->spriteIndex = arg0->spriteIndex;
        actor->alpha = 0xC0;
        arg0->unk30.word += 0x10;
    } else {
        func_80071408(func_800550E0, 0, 0x63);
        func_800716E4(arg0);
        return;
    }
    func_800483FC(D_801248D4, func_80054B98, (s32)arg0);
}
#endif

void func_8005502C(MainMenuOverlayEffectActor *arg0) {
    arg0->alpha = 0xFF;
    arg0->unk30.word = 0;
    arg0->unk18.word = 0;
    arg0->unk1C.word = 0x300000;
    arg0->unk20.word = 0xEC0000;
    arg0->spriteIndex = 0;
    func_80071824(arg0, func_80054EC4);
}

void func_80055074(MainMenuOverlayEffectActor *arg0) {
    if (D_80156608 == 2) {
        func_80046D68(-0x6C, -0x48, func_80043040(D_80112174), 0, D_800D5808[arg0->unk18.half.lo]);
    }
}

void func_800550E0(MainMenuOverlayEffectActor *arg0) {
    s16 v;
    arg0->unk18.half.hi += 1;
    v = arg0->unk18.half.hi;
    if (v >= 0x3D) {
        arg0->unk18.half.hi = 0;
        v = arg0->unk18.half.hi;
    }
    if (v < 0xA) {
        arg0->unk18.half.lo = v;
    } else {
        arg0->unk18.half.lo = 0;
    }
    func_800483FC(D_80124878, func_80055074, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_overlay_effects/func_80055148.s")

void func_8005537C(MainMenuOverlayEffectActor *arg0) {
    s16 v = arg0->unk18.half.hi;
    if (v != 0) {
        arg0->unk18.half.hi = v - 1;
        return;
    }
    arg0->unk18.half.lo += 0x20;
    if (arg0->unk18.half.lo >= 0x100) {
        arg0->unk18.half.lo = 0xFF;
    }
    func_800483FC(&D_80124868, func_80055148, arg0);
}

void func_800553E0(MainMenuOverlayEffectActor *arg0) {
    arg0->unk18.half.hi = 0x50;
    arg0->unk18.half.lo = 0;
    func_80071824(arg0, func_8005537C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_overlay_effects/func_80055410.s")

void func_80055530(void *arg0) {
    if (D_80156608 == 0) {
        gDPPipeSync(gRegionAllocPtr++);

        gSPSegment(gRegionAllocPtr++, 0x02, func_80043040(D_80112140));
        gSPSegment(gRegionAllocPtr++, 0x03, func_80043040(D_80112142));

        gSPMatrix(gRegionAllocPtr++, D_800DEE50, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        gSPDisplayList(gRegionAllocPtr++, D_2000E70);
    }
}

void func_8005562C(s32 arg0) {
    func_800483FC(D_801248F8, func_80055530, arg0);
    func_800483FC(D_801248A4, func_80055410, arg0);
}

void func_80055678(MainMenuOverlayEffectActor *arg0) {
    arg0->unk18.half.hi = 0x360;
    func_80071824(arg0, func_8005562C);
}
