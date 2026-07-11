#include "common.h"
#include "memory_allocator.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "viewport_manager.h"
#include "fixed_point_math.h"
#include "main_menu_overlay_effects.h"
#include "main_menu_scene_model.h"
#include "main_menu_scene_renderer.h"

#define MAIN_MENU_GFX_CMD(pkt, cmd0, cmd1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (cmd0); \
    _g->words.w1 = (cmd1); \
}

typedef struct {
    /* 0x000 */ u8 pad0[0x28];
    /* 0x028 */ Vec3i pos28;
    /* 0x034 */ u8 pad34[0x2EA - 0x34];
    /* 0x2EA */ s16 pitch;
    /* 0x2EC */ s16 yaw;
    /* 0x2EE */ u8 pad2EE[0x2FC - 0x2EE];
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
    /* 0x00 */ OverlayActorWord x;
    /* 0x04 */ OverlayActorWord y;
    /* 0x08 */ OverlayActorWord z;
} OverlayActorTransform;

struct MainMenuOverlayEffectActor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ OverlayActorWord unk18;
    /* 0x1C */ OverlayActorWord unk1C;
    /* 0x20 */ OverlayActorWord unk20;
    /* 0x24 */ OverlayActorWord unk24;
    /* 0x28 */ u16 timer;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ u8 pad2C[2];
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F;
    /* 0x30 */ OverlayActorWord unk30;
    /* 0x34 */ s16 spriteIndex;
    /* 0x36 */ s16 alpha;
};

typedef s16 FixedMatrix3sScratch[0x10];

typedef struct {
    /* 0x00 */ FixedMatrix3s rotation;
    /* 0x12 */ s16 pad12;
    /* 0x14 */ s32 x;
    /* 0x18 */ s32 y;
    /* 0x1C */ s32 z;
} GfxCommandSource;

typedef struct {
    /* 0x00 */ GfxCommandSource source;
    /* 0x20 */ s32 pad20;
} Func55410Scratch;

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

extern Gfx *gRegionAllocPtr;
extern void *gMenuRenderCallbackList;
extern u8 D_80124858[];
extern u8 D_80124878[];
extern u8 D_801248A4[];
extern u8 D_801248BC;
extern u8 D_801248D4[];
extern u8 D_801248F8[];
extern s16 D_800D5738[];
extern u8 D_800D5730[];
extern u8 D_800D5744[];
extern s32 D_800D5748[];
extern u8 D_800D57A0[];
extern s16 D_800D57B0[];
extern s16 D_800D57B4[];
extern u32 D_800D57C8[];
extern s16 D_800D5808[];
extern s16 D_800D581C[];
extern u32 D_800D5760[];
extern u32 D_800D6270[];
extern u32 D_800D60E0[];
extern GfxCommandSource gIdentityFixedTransform;
extern u32 gIdentityMatrix[];
extern s16 gMenuFadeAlpha;
extern u32 D_800DEFF8[];
extern s16 D_8011213C;
extern s16 D_80112140;
extern s16 D_80112142;
extern s16 D_8011214C;
extern s16 D_80112154;
extern s16 D_80112168;
extern s16 D_8011216A;
extern s16 D_8011216C;
extern s16 D_8011216E;
extern s16 D_80112174;
extern s16 D_80112184;
extern s16 D_80121B50;
extern s16 gFrameCounter;
extern RacePlayerState D_80121D80[];
extern u8 gCurrentViewportIndex;
extern u8 D_80156609;
extern GfxCommandDest *D_80156614;
extern u8 gConnectedControllerCount;
extern u32 D_2000000[];
extern u32 D_20006C8[];
extern u32 D_2000D78[];
extern u32 D_2000E48[];
extern u32 D_2000E70[];
extern u32 D_20058A8[];

void func_8000F030(s32, s32, s32, s32, s32, s32, s32, s32);
void func_80045A78(s16, s16, s32, u16);
void func_80046D68(s32, s32, s32, s32, s32);
void func_80045990(void *, u16, void **, void **);
void func_80048278(s32, s32, void *, s32);
void addRenderCallback(void *, void *, s32);
GfxCommandDest *func_8004885C(GfxCommandSource *);
void func_80053B28(MainMenuOverlayEffectActor *);
void func_80053C90(void *);
void func_80053D8C(s32);
void func_80053DFC(s32);
void func_8005408C(MainMenuOverlayEffectActor *);
void func_80054130(MainMenuOverlayEffectActor *);
void func_80054460(MainMenuOverlayEffectActor *);
void func_800545D0(MainMenuOverlayEffectActor *);
void func_80054644(MainMenuOverlayEffectActor *);
void func_8005475C(MainMenuOverlayEffectActor *);
void func_800548F4(MainMenuOverlayEffectActor *);
void func_8005499C(MainMenuOverlayEffectActor *);
void func_80054A04(MainMenuOverlayEffectActor *);
void func_80054A64(MainMenuOverlayEffectActor *);
void func_80054AC0(MainMenuOverlayEffectActor *);
void func_80054B98(MainMenuOverlayEffectActor *);
void func_80054EC4(MainMenuOverlayEffectActor *);
void func_800550E0(MainMenuOverlayEffectActor *);
void func_80055148(MainMenuOverlayEffectActor *);
void func_8005537C(MainMenuOverlayEffectActor *);
void func_80055410(MainMenuOverlayEffectActor *);
void func_80055530(void *);
void enqueueSoundEffect(s16, s32, void *);

void func_80053660(MainMenuOverlayEffectActor *arg0) {
    if (gCurrentViewportIndex == arg0->index) {
        func_8000F030((s16)(arg0->unk18.half.hi >> 4), (s16)(arg0->unk18.half.lo >> 4), func_80043040(D_80112184), (arg0->unk1C.half.hi + 2) & 0xFFFF,
                      arg0->timer, arg0->timer, 0, arg0->unk2A + 2);
    }
}

void func_800536F4(MainMenuOverlayEffectActor *arg0) {
    s16 temp_v0;
    s16 temp_v0_2;
    u32 var_v1;

    arg0->unk1C.half.hi = (arg0->unk1C.half.hi + 1) & 3;
    if (arg0->unk24.half.hi >= 0) {
        temp_v0 = arg0->unk24.half.lo;
        if (temp_v0 != 0) {
            arg0->unk20.half.hi += 4;
            arg0->unk24.half.lo = temp_v0 - 1;
        } else {
            arg0->unk20.half.hi -= 4;
            if (arg0->unk20.half.hi == 0) {
                arg0->unk24.half.lo = (randomNextMain() & 0x1F) + 0x10;
                arg0->unk24.half.hi = -1;
            }
        }
    } else {
        temp_v0_2 = arg0->unk24.half.lo;
        if (temp_v0_2 != 0) {
            arg0->unk20.half.hi -= 4;
            arg0->unk24.half.lo = temp_v0_2 - 1;
        } else {
            arg0->unk20.half.hi += 4;
            if (arg0->unk20.half.hi == 0) {
                arg0->unk24.half.lo = (randomNextMain() & 0x1F) + 0x10;
                arg0->unk24.half.hi = 1;
            }
        }
    }

    var_v1 = arg0->unk20.half.hi;
    arg0->unk18.half.hi += var_v1;
    arg0->unk18.half.lo += arg0->unk20.half.lo;
    if (arg0->unk18.half.lo >= 0x6E1) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(D_80124878, func_80053660, (s32)arg0);
}

void func_80053858(MainMenuOverlayEffectActor *arg0) {
    arg0->unk1C.half.hi = (arg0->unk1C.half.hi + 1) & 3;
    arg0->unk18.half.lo += arg0->unk20.half.lo;
    arg0->unk24.half.hi = (arg0->unk24.half.hi + 0x20) & 0xFFF;
    arg0->unk18.half.hi += (fixedSine(arg0->unk24.half.hi) * (arg0->unk1C.half.lo + 0x30)) / 4096;
    if ((arg0->unk24.half.hi == 0) || (arg0->unk24.half.hi == 0x800)) {
        arg0->unk1C.half.lo = randomNextMain() & 0xF;
    }
    if (arg0->unk18.half.lo >= 0x6E1) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(D_80124878, func_80053660, (s32)arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_overlay_effects/func_8005393C.s")

void func_80053B28(MainMenuOverlayEffectActor *arg0) {
    register MainMenuOverlayEffectActor *actor = arg0;

    if (gCurrentViewportIndex == 0) {
        gDPPipeSync(gRegionAllocPtr++);

        gSPSegment(gRegionAllocPtr++, 0x02, func_80043040(D_80112140));
        gSPSegment(gRegionAllocPtr++, 0x03, func_80043040(D_80112142));

        gSPMatrix(gRegionAllocPtr++, gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        if (actor->index == 0) {
            gSPDisplayList(gRegionAllocPtr++, D_2000000);
        }
        if (actor->index == 1) {
            gSPDisplayList(gRegionAllocPtr++, D_20006C8);
        }
        if (actor->index == 2) {
            gSPDisplayList(gRegionAllocPtr++, D_2000D78);
        }
    }
}

// func_80053C90 best match: 99.677% at nonmatchings/func_80053C90-2/output-20-1/source.c
void func_80053C90(void *arg0) {
    if (gCurrentViewportIndex == 0) {
        gDPPipeSync(gRegionAllocPtr++);

        gSPSegment(gRegionAllocPtr++, 0x02, func_80043040(D_8011214C));
        gSPSegment(gRegionAllocPtr++, 0x03, func_80043040(D_80112154));

        gSPMatrix(gRegionAllocPtr++, gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        gSPDisplayList(gRegionAllocPtr++, D_20058A8);
    }
}

void func_80053D8C(s32 arg0) {
    addRenderCallback(D_801248F8, func_80053C90, arg0);
    addRenderCallback(D_801248A4, func_80053B28, arg0);
}

void func_80053DD8(void *arg0) {
    setCallbackTaskCallback(arg0, func_80053D8C);
}

void func_80053DFC(s32 arg0) {
    func_80042034(0);
    func_80042A00(0);
}

void func_80053E28(MainMenuOverlayEffectActor *arg0) {
    if (gMenuFadeAlpha == 0) {
        if (arg0->unk18.byte.b0 != 0) {
            enqueueSoundEffect(D_800D5738[arg0->index], 0x32, arg0);
            arg0->unk18.byte.b0 = 0;
        }
        func_80042034(0);
        setCallbackTaskCallback(arg0, func_80053DFC);
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
    setCallbackTaskCallback(arg0, func_80053E28);
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
    setCallbackTaskCallback(arg0, func_80053F50);
}

void func_80054044(s32 arg0, s32 arg1) {
    MainMenuOverlayEffectActor *p = createCallbackTaskWithUserId(func_80053FA0, 0, 0x64, arg1);
    if (p != NULL) {
        p->unk18.word = arg0;
    }
}

void func_8005408C(MainMenuOverlayEffectActor *arg0) {
    if (gConnectedControllerCount != 0) {
        func_80048278(-0x44, 0x38, "Push Start Button", 0);
        return;
    }
    func_80048278(-0x5C, 0x38, "No Control Pad detected", 1);
}

void func_800540EC(void *arg0) {
    if (gFrameCounter & 8) {
        addRenderCallback(D_80124858, func_8005408C, 0);
    }
}

void func_80054130(MainMenuOverlayEffectActor *arg0) {
    void *spA4;
    void *spA0;
    volatile u8 gap[0xC];
    GfxCommandSource sp74;

    if (D_80156609 != 0) {
        arg0->unk2F = 1;
    }

    if (arg0->unk2F != 0) {
        arg0->unk2F = 0;
        sp74 = gIdentityFixedTransform;
        sp74.x = arg0->unk18.word;
        sp74.y = arg0->unk1C.word;
        sp74.z = arg0->unk20.word;
        arg0->unk24.word = (s32)func_8004885C(&sp74);
    }

    if (arg0->unk24.word != 0) {
        gSPDisplayList(gRegionAllocPtr++, D_800D6270);
        func_80045990(func_80043040(D_8011216C), (u16)(arg0->unk2E + 0x24), &spA4, &spA0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xFD500000, (u32)spA4);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF5500000, 0x07080200);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF3000000, 0x070FF400);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF5400400, 0x80200);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF2000000, 0x7C07C);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)spA0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0x01020040, arg0->unk24.word);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0x01000040, (u32)D_80156614);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)D_800D5760);
        MAIN_MENU_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
    }
}

void func_80054460(MainMenuOverlayEffectActor *arg0) {
    FixedMatrix3sScratch sp38;
    Vec3i sp2C;
    RacePlayerState *player;
    u8 temp;

    player = &D_80121D80[arg0->index];
    if (player->flags & 0x2000) {
        setCallbackTaskCallback(arg0, func_800545D0);
        return;
    }

    temp = arg0->unk2E;
    if (temp != 5) {
        arg0->unk2E = temp + 1;
    } else if (player->unk51A == 0) {
        setCallbackTaskCallback(arg0, func_800545D0);
    }

    sp2C.x = 0x40000;
    sp2C.y = 0x180000;
    sp2C.z = 0;

    player = &D_80121D80[arg0->index];
    makeFixedRotationXY(sp38, player->pitch, player->yaw);
    transformVec3iByFixedMatrix(sp38, &sp2C, (Vec3i *) &arg0->unk18);

    player = &D_80121D80[arg0->index];
    arg0->unk18.word += player->pos28.x;
    arg0->unk1C.word += player->pos28.y;
    arg0->unk20.word += player->pos28.z;

    addRenderCallback(&D_801248BC, func_80054130, (s32)arg0);
}

void func_800545D0(MainMenuOverlayEffectActor *arg0) {
    RacePlayerState *player = &D_80121D80[arg0->index];
    if (!(player->flags & 0x2000)) {
        if (player->unk51A != 0) {
            arg0->unk2E = 0xFF;
            setCallbackTaskCallback(arg0, func_80054460);
        }
    }
}

void func_80054644(MainMenuOverlayEffectActor *arg0) {
    if ((gFrameCounter & 0x3E) >= 0x1F) {
        func_80045A78(arg0->unk18.half.hi, arg0->unk18.half.lo, func_80043040(D_8011213C), 1);
    } else {
        func_80045A78(arg0->unk18.half.hi, arg0->unk18.half.lo, func_80043040(D_8011213C), D_800D57A0[(gFrameCounter & 0x1E) >> 1]);
    }
}

void func_800546E4(s32 arg0) {
    addRenderCallback(&gMenuRenderCallbackList, func_80054644, arg0);
}

void func_80054714(MainMenuOverlayEffectActor *arg0) {
    s16 *temp = &D_800D57B0[arg0->index * 2];

    arg0->unk18.half.hi = temp[0];
    arg0->unk18.half.lo = temp[1];
    setCallbackTaskCallback(arg0, func_800546E4);
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
            removeCallbackTask(arg0);
            return;
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, func_8005475C, arg0);
}

void func_80054968(MainMenuOverlayEffectActor *arg0) {
    arg0->unk18.half.hi = 0x3E;
    arg0->unk18.half.lo = 0xFF;
    setCallbackTaskCallback(arg0, func_800548F4);
}

void func_8005499C(MainMenuOverlayEffectActor *arg0) {
    u8 *new_var;

    new_var = &gCurrentViewportIndex;
    if (arg0->index == *new_var) {
        func_80045A78((s16)((arg0->unk18.word >> 1) - 0x38), -8, func_80043040(D_8011216E), 0x76);
    }
}

void func_80054A04(MainMenuOverlayEffectActor *arg0) {
    s32 temp_v0 = arg0->unk24.word;
    s32 temp_t8 = (unsigned long long)(temp_v0 + 4);

    arg0->unk18.word += temp_v0;
    arg0->unk24.word = temp_t8;
    if (temp_t8 == 0x38) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(D_80124878, func_8005499C, (s32)arg0);
    }
}

void func_80054A64(MainMenuOverlayEffectActor *arg0) {
    u16 temp = (arg0->timer & 0xFFFFU) - 1;

    arg0->timer = temp;
    temp += 0;
    if (temp == 0) {
        setCallbackTaskCallback(arg0, func_80054A04);
    }
    if (1) {
    }
    addRenderCallback(D_80124878, func_8005499C, (s32)arg0);
}

void func_80054AC0(MainMenuOverlayEffectActor *arg0) {
    arg0->unk18.word += arg0->unk24.word;
    arg0->unk24.word -= 4;
    if (arg0->unk24.word == 0) {
        arg0->timer = 0x1E;
        setCallbackTaskCallback(arg0, func_80054A64);
    }
    addRenderCallback(D_80124878, func_8005499C, (s32)arg0);
}

void func_80054B2C(MainMenuOverlayEffectActor *arg0) {
    if (D_801124B0[arg0->index].active != 0) {
        arg0->unk24.word = 0x38;
        arg0->unk18.word = -0x1A4;
        setCallbackTaskCallback(arg0, func_80054AC0);
        return;
    }
    removeCallbackTask(arg0);
}

// func_80054B98 best match: 84.797% at nonmatchings/func_80054B98-4923837976568703863/base_11.c
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_overlay_effects/func_80054B98.s")

#ifdef NON_MATCHING
void func_80054B98(MainMenuOverlayEffectActor *arg0) {
    void *image;
    void *palette;
    GfxCommandSource transform;
    volatile u8 pad[0x48];
    GfxCommandDest *matrix;
    Gfx *gfx;

    if (gCurrentViewportIndex == 2) {
        func_80045990((void *) func_80043040(D_80112174), 0, &image, &palette);
        makeFixedRotationZ(transform.rotation, arg0->spriteIndex);
        transform.x = arg0->unk18.word;
        transform.y = arg0->unk1C.word;
        transform.z = arg0->unk20.word;
        matrix = func_8004885C(&transform);
        if (matrix != NULL) {
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w0 = 0x06000000;
            gfx->words.w1 = (u32) D_800D60E0;
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w0 = 0xFA000000;
            gfx->words.w1 = (arg0->alpha & 0xFF) | ~0xFF;
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w0 = 0xFD48006B;
            gfx->words.w1 = (u32) image;
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w0 = 0xF5481C00;
            gfx->words.w1 = 0x07080200;
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w1 = 0;
            gfx->words.w0 = 0xE6000000;
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w0 = 0xF4000000;
            gfx->words.w1 = 0x071B0040;
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w1 = 0;
            gfx->words.w0 = 0xE7000000;
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w0 = 0xF5401C00;
            gfx->words.w1 = 0x00080200;
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w0 = 0xF2000000;
            gfx->words.w1 = 0x00360040;
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w0 = 0xFD100000;
            gfx->words.w1 = (u32) palette;
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w1 = 0;
            gfx->words.w0 = 0xE8000000;
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w0 = 0xF5000100;
            gfx->words.w1 = 0x07000000;
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w1 = 0;
            gfx->words.w0 = 0xE6000000;
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w0 = 0xF0000000;
            gfx->words.w1 = 0x0703C000;
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w1 = 0;
            gfx->words.w0 = 0xE7000000;
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w0 = 0x01020040;
            gfx->words.w1 = (u32) matrix;
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w0 = 0x01000040;
            gfx->words.w1 = (u32) D_80156614;
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w0 = 0x0400103F;
            gfx->words.w1 = (u32) D_800D57C8;
            gfx = gRegionAllocPtr;
            gRegionAllocPtr = gfx + 1;
            gfx->words.w0 = 0xB1060402;
            gfx->words.w1 = 0x00060200;
        }
    }
}
#endif

void func_80054E70(MainMenuOverlayEffectActor *arg0) {
    arg0->alpha -= 0x20;
    if (arg0->alpha == 0) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(D_801248D4, func_80054B98, (s32)arg0);
}

void func_80054EC4(MainMenuOverlayEffectActor *arg0) {
    MainMenuOverlayEffectActor *actor;

    arg0->unk1C.word = ((-0x400000LL * fixedSine((s16)(arg0->unk30.word + 0x400))) / 0x1000) + 0x700000;
    arg0->unk20.word = ((-0x9F0000LL * arg0->unk30.word) / 0x400) + 0xEC0000;
    arg0->spriteIndex = (0x5000LL * fixedSine(arg0->unk30.half.lo)) / 0x1000;
    if (arg0->unk30.word < 0x3F0) {
        actor = createCallbackTask(func_80054E70, 0, 0x65);
        *(OverlayActorTransform *)&actor->unk18 = *(OverlayActorTransform *)&arg0->unk18;
        actor->spriteIndex = arg0->spriteIndex;
        actor->alpha = 0xC0;
        arg0->unk30.word += 0x10;
    } else {
        createCallbackTask(func_800550E0, 0, 0x63);
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(D_801248D4, func_80054B98, (s32)arg0);
}

void func_8005502C(MainMenuOverlayEffectActor *arg0) {
    arg0->alpha = 0xFF;
    arg0->unk30.word = 0;
    arg0->unk18.word = 0;
    arg0->unk1C.word = 0x300000;
    arg0->unk20.word = 0xEC0000;
    arg0->spriteIndex = 0;
    setCallbackTaskCallback(arg0, func_80054EC4);
}

void func_80055074(MainMenuOverlayEffectActor *arg0) {
    if (gCurrentViewportIndex == 2) {
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
    addRenderCallback(D_80124878, func_80055074, arg0);
}

// func_80055148 best match: 99.220%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_overlay_effects/func_80055148.s")

#ifdef NON_MATCHING
void func_80055148(MainMenuOverlayEffectActor *arg0) {
    Gfx *temp_v0;
    Gfx *temp_v0_2;
    Gfx *temp_v0_3;
    Gfx *temp_v0_4;
    Gfx *temp_v0_5;
    u16 temp_t0;

    temp_v0 = gRegionAllocPtr;
    gRegionAllocPtr = temp_v0 + 1;
    temp_v0->words.w0 = 0xE7000000;
    temp_v0->words.w1 = 0;
    temp_v0_2 = gRegionAllocPtr;
    gRegionAllocPtr = temp_v0_2 + 1;
    temp_v0_2->words.w0 = 0xFC119623;
    temp_v0_2->words.w1 = 0xFF2FFFFF;
    temp_v0_3 = gRegionAllocPtr;
    gRegionAllocPtr = temp_v0_3 + 1; temp_v0_3->words.w0 = 0xB900031D; temp_v0_3->words.w1 = 0x00504240;
    temp_v0_4 = gRegionAllocPtr;
    gRegionAllocPtr = temp_v0_4 + 1;
    temp_v0_4->words.w0 = 0xFA000000;
    temp_v0_4->words.w1 = (arg0->unk18.half.lo & 0xFF) | ~0xFF;
    temp_t0 = arg0->index;
    switch (temp_t0) {
    case 0:
        func_80045A78(-0x8C, 0x38, func_80043040(D_80112174), 1);
        func_80046D68(-0x10, 0x38, func_80043040(D_80112174), 2, D_800D581C[gFrameCounter & 3]);
        break;
    case 1:
        func_80045A78(-0x89, 0x38, func_80043040(D_80112174), 1);
        func_80046D68(-0xD, 0x38, func_80043040(D_80112174), 3, D_800D581C[gFrameCounter & 3]);
        break;
    case 2:
        func_80045A78(-0x7E, 0x38, func_80043040(D_80112174), 1);
        func_80046D68(-2, 0x38, func_80043040(D_80112174), 4, D_800D581C[gFrameCounter & 3]);
        break;
    } temp_v0_5 = gRegionAllocPtr; gRegionAllocPtr = temp_v0_5 + 1; temp_v0_5->words.w0 = 0x06000000; temp_v0_5->words.w1 = (u32) D_800DEFF8;
}
#endif

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
    addRenderCallback(&gMenuRenderCallbackList, func_80055148, arg0);
}

void func_800553E0(MainMenuOverlayEffectActor *arg0) {
    arg0->unk18.half.hi = 0x50;
    arg0->unk18.half.lo = 0;
    setCallbackTaskCallback(arg0, func_8005537C);
}

void func_80055410(MainMenuOverlayEffectActor *arg0) {
    Func55410Scratch scratch;
    GfxCommandDest *matrix;

    if (gCurrentViewportIndex == 0) {
        makeFixedRotationY(scratch.source.rotation, arg0->unk18.half.hi);
        scratch.source.x = 0;
        scratch.source.y = 0;
        scratch.source.z = 0;
        matrix = func_8004885C(&scratch.source);
        if (matrix != NULL) {
            gDPPipeSync(gRegionAllocPtr++);

            gSPSegment(gRegionAllocPtr++, 0x02, func_80043040(D_80112140));
            gSPSegment(gRegionAllocPtr++, 0x03, func_80043040(D_80112142));

            gSPMatrix(gRegionAllocPtr++, matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            gSPDisplayList(gRegionAllocPtr++, D_2000E48);
        }
    }
}

void func_80055530(void *arg0) {
    if (gCurrentViewportIndex == 0) {
        gDPPipeSync(gRegionAllocPtr++);

        gSPSegment(gRegionAllocPtr++, 0x02, func_80043040(D_80112140));
        gSPSegment(gRegionAllocPtr++, 0x03, func_80043040(D_80112142));

        gSPMatrix(gRegionAllocPtr++, gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        gSPDisplayList(gRegionAllocPtr++, D_2000E70);
    }
}

void func_8005562C(s32 arg0) {
    addRenderCallback(D_801248F8, func_80055530, arg0);
    addRenderCallback(D_801248A4, func_80055410, arg0);
}

void func_80055678(MainMenuOverlayEffectActor *arg0) {
    arg0->unk18.half.hi = 0x360;
    setCallbackTaskCallback(arg0, func_8005562C);
}
