#include "common.h"
#include "memory_allocator.h"
#include "effect_task_scheduler.h"
#include "asset_decompression.h"
#include "game_audio.h"
#include "fixed_point_math.h"
#include "fixed_point_matrix.h"

#define RACE_PLAYER_STATE_SIZE 0x60C
#define RACE_ITEM_GFX_CMD(pkt, cmd0, cmd1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (cmd0); \
    _g->words.w1 = (cmd1); \
}

typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
} Vec2s;

typedef s16 FixedMatrix3sScratch[0x10];

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ char pad12[2];
    /* 0x14 */ s32 x;
    /* 0x18 */ s32 y;
    /* 0x1C */ s32 z;
} RaceItemGfxCommandSource;

typedef union {
    Vec3i vec;
    struct {
        /* 0x18 */ s16 x;
        /* 0x1A */ s16 y;
        /* 0x1C */ u16 frame;
        /* 0x1E */ u8 pad1E[2];
        /* 0x20 */ s8 colorR;
        /* 0x21 */ s8 colorG;
        /* 0x22 */ s8 colorB;
    } sprite;
} RaceItemEffectPayload;

typedef union {
    s32 word;
    struct {
        /* 0x28 */ s8 unk28;
        /* 0x29 */ s8 phase;
        /* 0x2A */ s8 unk2A;
        /* 0x2B */ s8 unk2B;
    } bytes;
} RaceItemEffectWord28;

typedef union {
    s16 halfword;
    s8 byte;
    u8 ubyte;
    struct {
        /* 0x34 */ s8 byte0;
        /* 0x35 */ s8 matrixDirty;
    } bytes;
} RaceItemEffectState;

typedef union {
    s32 velocityX;
    s16 timer;
} RaceItemEffectWord24;

typedef union {
    struct {
        /* 0x30 */ s16 x;
        /* 0x32 */ s16 y;
    } screen;
    void *matrix;
    struct {
        /* 0x30 */ s16 drawInitialized;
        /* 0x32 */ s16 delay;
    } particle;
} RaceItemEffectShorts30;

typedef union {
    s16 width;
    s16 alpha;
} RaceItemEffectHalf38;

typedef union {
    s16 halfword;
    s8 byte;
} RaceItemEffectHeight;

typedef union {
    void *matrix;
    struct {
        /* 0x34 */ RaceItemEffectState state;
        /* 0x36 */ RaceItemEffectHeight height;
    } shorts;
} RaceItemEffectWord34;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 playerIndex;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ RaceItemEffectPayload payload;
    /* 0x24 */ RaceItemEffectWord24 unk24;
    /* 0x28 */ RaceItemEffectWord28 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ RaceItemEffectShorts30 unk30;
    /* 0x34 */ RaceItemEffectWord34 unk34;
    /* 0x38 */ RaceItemEffectHalf38 unk38;
    /* 0x3A */ u8 pad3A[2];
    /* 0x3C */ void *image;
    /* 0x40 */ void *palette;
    /* 0x44 */ u8 pad44[9];
    /* 0x4D */ u8 angleIndex;
    /* 0x4E */ u8 pad4E;
    /* 0x4F */ u8 followPlayerIndex;
    /* 0x50 */ u8 pad50[0x64 - 0x50];
    /* 0x64 */ s16 unk64;
    /* 0x66 */ u8 pad66[2];
    /* 0x68 */ u8 *unk68;
} RaceItemEffectActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 playerIndex;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ Vec3i pos1;
    /* 0x24 */ Vec3i pos2;
    /* 0x30 */ Vec3i offset1;
    /* 0x3C */ Vec3i offset2;
    /* 0x48 */ void *matrix1;
    /* 0x4C */ void *matrix2;
    /* 0x50 */ s8 dirty;
    /* 0x51 */ s8 timer;
} RaceItemFollowActor;

typedef struct RaceItemDrawNode {
    /* 0x00 */ struct RaceItemDrawNode *next;
    /* 0x04 */ Vec3i *pos;
    /* 0x08 */ u32 displayList;
    /* 0x0C */ void *matrix;
    /* 0x10 */ u8 matrixDirty;
} RaceItemDrawNode;

typedef struct {
    /* 0x00 */ RaceItemDrawNode *heads[4];
} RaceItemDrawLists;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ void *images[4];
    /* 0x28 */ void *palettes[4];
} RaceItemTextureActor;

typedef struct {
    /* 0x000 */ Vec3i pos;
    /* 0x00C */ u8 padC[RACE_PLAYER_STATE_SIZE - 0xC];
} RaceItemEffectPlayerState;

typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ Vec3i pos1C;
    /* 0x028 */ Vec3i pos;
    /* 0x034 */ u8 pad34[0x94 - 0x34];
    /* 0x094 */ u8 transform[0x14];
    /* 0x0A8 */ Vec3i posA8;
    /* 0x0B4 */ u8 padB4[0x2FC - 0xB4];
    /* 0x2FC */ s32 flags2FC;
    /* 0x300 */ u8 pad300[0x4A0 - 0x300];
    /* 0x4A0 */ Vec3i unk4A0;
    /* 0x4AC */ Vec3i unk4AC;
    /* 0x4B8 */ Vec3i unk4B8;
    /* 0x4C4 */ Vec3i unk4C4;
    /* 0x4D0 */ u8 pad4D0[RACE_PLAYER_STATE_SIZE - 0x4D0];
} RaceItemFollowPlayer;

typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ s16 itemTextureHandle;
} RaceItemEffectAssetHandles;

typedef union {
    s64 value;
    struct {
        /* 0x0 */ s32 high;
        /* 0x4 */ u32 low;
    } word;
} LongLongParts;

extern u8 *D_800D46D0[];
extern u8 D_800D4708[];
extern u16 D_800D46F8[];
extern Gfx D_800D45E0[];
extern u8 D_800D4808[];
extern u8 D_800D4814[];
extern u8 D_800D4820[];
extern u32 D_800D4830[];
extern s16 D_800D4870[];
extern Gfx D_800D4878[];
extern u32 D_800D48A8[];
extern Gfx D_800D48E8[];
extern u32 D_800D4A00[];
extern Vec2s D_800D4928[];
extern u32 D_800D49C0[];
extern s32 D_800D4968[];
extern s32 D_800D4974[];
extern u32 D_800D4980[];
extern u32 D_800D6270[];
extern Gfx D_800D9D00[];
extern Gfx D_800D9D40[];
extern RaceItemGfxCommandSource D_800DEE30;
extern RaceItemEffectAssetHandles D_80112130;
extern RaceItemDrawLists D_801121D0;
extern RaceItemDrawLists D_801121E0;
extern s16 D_80112168;
extern s16 D_8011216C;
extern s16 D_8011216E;
extern s16 D_80121B50;
extern u8 D_80121B56;
extern RaceItemFollowPlayer D_80121D80[];
extern RaceItemEffectPlayerState D_80121EE8[];
extern s32 D_80124878;
extern s32 D_801248C8;
extern s32 D_801248E0;
extern s32 D_801248EC;
extern u8 D_80156608;
extern u8 D_80156609;
extern void *D_80156614;
extern Gfx *gRegionAllocPtr;

void func_800459D4(u8 *, u16, u16, void **, void **);
void func_80045990(s32, u16, void *, void *);
void func_80046D68(s16, s16, s32, s32, s32);
void func_8004767C(s16, s16, s32, s32, s32, s32);
void func_800483FC(void *, void *, void *);
void *func_8004885C(RaceItemGfxCommandSource *);
void func_8004DB8C(RaceItemEffectActor *);
void func_8004E02C(RaceItemEffectActor *);
void func_8004E438(RaceItemEffectActor *);
/* Local 4-arg declaration; see note in effect_task_scheduler.h. */
RaceItemEffectActor *func_800716A4(void *, s32, s32, s32);
void func_8004E604(RaceItemEffectActor *);
void func_8004E960(RaceItemEffectActor *);
void func_8004EAA8(RaceItemEffectActor *);
void func_8004FB44(RaceItemFollowActor *);
void func_8004EFF8(RaceItemEffectActor *);
void func_8004F68C(RaceItemEffectActor *);
void func_8004F9CC(RaceItemEffectActor *);
void func_8005019C(RaceItemEffectActor *);
void func_80050340(RaceItemEffectActor *);
void func_80050398(RaceItemEffectActor *);
void func_80050888(RaceItemEffectActor *);
void func_8005098C(RaceItemFollowActor *);

u8 func_8004DB60(s32 arg0) {
    u8 *p = D_800D46D0[D_80121B50];
    return p[arg0];
}

// func_8004DB8C best match: 70.982% (nonmatchings/func_8004DB8C-2225551288923588688/base_4.c)

#pragma GLOBAL_ASM("asm/nonmatchings/race_item_effects/func_8004DB8C.s")

#ifdef NON_MATCHING
void func_8004DB8C(RaceItemEffectActor *arg0) {
    RaceItemDrawNode **drawList;
    u8 *cursor;
    RaceItemEffectPayload *payload;
    s16 state;
    s32 i;
    u16 playerIndex;

    if (D_80121B56 == 0) {
        arg0->unk64 = arg0->unk64 + 1;
        if (arg0->unk64 == 5) {
            func_800716E4(arg0);
            return;
        }
    }

    state = arg0->unk64;
    i = 0;
    if (state == 0) {
        arg0->unk64 = state + 1;
    }

    playerIndex = arg0->playerIndex;
    cursor = (u8 *)arg0;
    if (playerIndex >= 4) {
        playerIndex = 0;
    }

    drawList = &D_801121D0.heads[playerIndex];
    payload = &arg0->payload;

loop:
    *(RaceItemDrawNode **)(cursor + 0x3C) = *drawList;
    *drawList = (RaceItemDrawNode *)(cursor + 0x3C);
    *(RaceItemEffectPayload **)(cursor + 0x40) = payload;
    i++;
    payload += 1;
    cursor += 0x14;
    *(u8 **)(cursor + 0x30) = &D_800D4708[arg0->unk68[arg0->unk64 - 1] * 0x10];
    if (i != 2) {
        goto loop;
    }
}
#endif

void func_8004DC6C(RaceItemEffectActor *arg0) {
    arg0->unk64 = 0;
    func_8004DB8C(arg0);
    func_80071824(arg0, func_8004DB8C);
}

// func_8004DCA0 best match: 86.674% (nonmatchings/func_8004DCA0-2225551288923588688/base_6.c)

#pragma GLOBAL_ASM("asm/nonmatchings/race_item_effects/func_8004DCA0.s")

#ifdef NON_MATCHING
void func_8004DCA0(Vec3i *arg0, Vec3i *arg1, Vec3i *arg2, Vec3i *arg3, s16 arg4) {
    LongLongParts total;
    s64 distY;
    s64 distX;
    s64 distZ;
    volatile s32 high;
    s32 midAX;
    s32 midAY;
    s32 midAZ;
    s32 midBX;
    s32 midBY;
    s32 midBZ;
    s32 dx;
    s32 i;
    s32 random;
    RaceItemEffectActor *actor;
    u8 *cursor;
    s16 itemType;

    itemType = func_8004DB60(arg4);
    if (itemType != 4) {
        midAX = ((arg0->x - arg1->x) / 2) + arg1->x;
        midAY = ((arg0->y - arg1->y) / 2) + arg1->y;
        midAZ = ((arg0->z - arg1->z) / 2) + arg1->z;
        midBX = ((arg2->x - arg3->x) / 2) + arg3->x;
        midBY = ((arg2->y - arg3->y) / 2) + arg3->y;
        midBZ = ((arg2->z - arg3->z) / 2) + arg3->z;

        dx = midAY - midBY;
        distY = (s64) dx * dx;
        dx = midAX - midBX;
        distX = (s64) dx * dx;
        dx = midAZ - midBZ;
        distZ = (s64) dx * dx;
        total.value = distZ + distX + distY;
        high = total.word.high;

        if (high > 0) {
            if (high < 2) {}
            actor = func_800716A4(func_8004DC6C, 5, 0x32, itemType);
            if (actor != NULL) {
                actor->unk68 = D_800D4820;
                if (high < 0x65) {
                    if (high >= 0x64) {
                    } else {
                        actor->unk68 = D_800D4814;
                    }
                }
                if (high < 0x25) {
                    if (high >= 0x24) {
                    } else {
                        actor->unk68 = D_800D4808;
                    }
                }

                cursor = (u8 *) actor;
                for (i = 0; i != 2; i++) {
                    random = func_800430D0() & 0xF;
                    cursor += 0xC;
                    *(s32 *)(cursor + 0xC) = (((arg0->x - arg1->x) * random) / 15) + arg1->x;
                    *(s32 *)(cursor + 0x10) = (((arg0->y - arg1->y) * random) / 15) + arg1->y;
                    *(s32 *)(cursor + 0x14) = (((arg0->z - arg1->z) * random) / 15) + arg1->z;
                }
            }
        }
    }
}
#endif

void func_8004E02C(RaceItemEffectActor *arg0) {
    RaceItemGfxCommandSource sp88;
    void *sp84;
    void *sp80;
    s32 frame;

 do { if (D_80156609) { sp88 = D_800DEE30; sp88.x = arg0->payload.vec.x; sp88.y = arg0->payload.vec.y; sp88.z = arg0->payload.vec.z; arg0->unk30.matrix = func_8004885C(&sp88); } if (arg0->unk30.matrix != NULL) { if (func_80049000(&arg0->payload.vec) != 0) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xBB000001; _g->words.w1 = 0xFFFFFFFF; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xFC121824; _g->words.w1 = 0xFF33FFFF; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xB900031D; _g->words.w1 = 0x005049D8; } ; func_80045990(func_80043040(D_80112168), (frame = D_800D4870[arg0->playerIndex] + (arg0->unk24.timer >> 1), 0xFFFF & frame), &sp84, &sp80); { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xFD500000; _g->words.w1 = (u32) sp84; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xF5500000; _g->words.w1 = 0x07080200; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xE6000000; _g->words.w1 = D_800D4870[arg0->playerIndex] * 0; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xF3000000; _g->words.w1 = 0x070FF400; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xE7000000; _g->words.w1 = 0; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xF5400400; _g->words.w1 = 0x80200; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xF2000000; _g->words.w1 = 0x7C07C; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xFD100000; _g->words.w1 = (u32) sp80; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xE8000000; _g->words.w1 = 0; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xF5000100; _g->words.w1 = 0x07000000; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xE6000000; _g->words.w1 = 0; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xF0000000; _g->words.w1 = 0x0703C000; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xE7000000; _g->words.w1 = 0; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0x01020040; _g->words.w1 = (u32) arg0->unk30.matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0x01000040; _g->words.w1 = (u32) D_80156614; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0x0400103F; _g->words.w1 = (u32) D_800D4830; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = 0xB1060402; _g->words.w1 = 0x60200; } ; } } } while (0);
}

void func_8004E3BC(RaceItemEffectActor *arg0) {
    s16 temp_v0;

    if (D_80121B56 == 0) {
        arg0->unk24.timer++;
        if (arg0->unk24.timer == 8) {
            func_800716E4(arg0);
            return;
        }
    }
    temp_v0 = arg0->unk24.timer;
    if (temp_v0 < 0) {
        arg0->unk24.timer = temp_v0 + 1;
    }
    func_800483FC(&D_801248E0, func_8004E02C, arg0);
}

void func_8004E438(RaceItemEffectActor *arg0) {
    char pad2C[4];
    RaceItemFollowPlayer *sp58;
    char pad50[8];
    char sp30[0x20];
    s32 sp24[3];

    arg0->unk24.timer = -1;
    if (arg0->unk34.shorts.state.halfword == 0) {
        sp58 = &D_80121D80[arg0->unk38.width];
        func_80097C18(sp30, arg0->unk34.shorts.height.halfword);
        sp24[0] = 0;
        sp24[1] = arg0->unk28.word;
        sp24[2] = arg0->unk2C;
        func_80098590(sp30, sp24, &arg0->payload);
        arg0->payload.vec.x += sp58->pos1C.x;
        arg0->payload.vec.y += sp58->pos1C.y;
        arg0->payload.vec.z += sp58->pos1C.z;
    }
    func_8004E3BC(arg0);
    func_80071824(arg0, func_8004E3BC);
}

void func_8004E518(s16 arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4) {
    RaceItemEffectActor *p = func_800716A4(func_8004E438, 5, 0x32, arg2);

    if (p != NULL) {
        p->unk34.shorts.state.halfword = 0;
        p->unk34.shorts.height.halfword = arg1;
        p->unk38.width = arg0;
        p->unk28.word = arg3;
        p->unk2C = arg4;
    }
}

void func_8004E594(s32 arg0, s32 arg1, s32 arg2, s16 arg3) {
    RaceItemEffectActor *p = func_800716A4(func_8004E438, 5, 2, arg3);

    if (p != NULL) {
        p->payload.vec.x = arg0;
        p->payload.vec.y = arg1;
        p->payload.vec.z = arg2;
        p->unk34.shorts.state.halfword = 1;
    }
}

// func_8004E604 best match: 97.710%

#pragma GLOBAL_ASM("asm/nonmatchings/race_item_effects/func_8004E604.s")

#ifdef NON_MATCHING
void func_8004E604(RaceItemEffectActor *arg0) {
    volatile u8 padding[4];
    RaceItemGfxCommandSource sp64;
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
        arg0->unk34.shorts.state.bytes.matrixDirty = 1;
    }

    if (arg0->unk34.shorts.state.bytes.matrixDirty != 0) {
        arg0->unk34.shorts.state.bytes.matrixDirty = 0;
        sp64 = D_800DEE30;
        sp64.unk0 = arg0->unk30.screen.y << 8;
        sp64.unk8 = arg0->unk30.screen.y << 8;
        sp64.unk10 = arg0->unk30.screen.y << 8;
        sp64.x = arg0->payload.vec.x;
        sp64.y = arg0->payload.vec.y;
        sp64.z = arg0->payload.vec.z;
        arg0->unk24.velocityX = (s32)func_8004885C(&sp64);
    }

    if (arg0->unk24.velocityX != 0) {
        temp_v0 = gRegionAllocPtr++;
        temp_v0->words.w0 = 0x06000000;
        temp_v0->words.w1 = (u32)D_800D4878;
        temp_v0_2 = gRegionAllocPtr++;
        temp_v0_2->words.w0 = 0xFA000000;
        temp_v0_2->words.w1 = (arg0->unk30.screen.x & 0xFF) | ~0xFF;
        temp_v0_3 = gRegionAllocPtr++;
        temp_v0_3->words.w0 = 0xFD500000;
        temp_v0_3->words.w1 = (u32)arg0->unk2C;
        temp_v0_4 = gRegionAllocPtr++;
        temp_v0_4->words.w1 = 0x07080200;
        temp_v0_4->words.w0 = 0xF5500000;
        temp_v0_5 = gRegionAllocPtr++;
        temp_v0_5->words.w1 = 0;
        temp_v0_5->words.w0 = 0xE6000000;
        temp_v0_6 = gRegionAllocPtr++;
        temp_v0_6->words.w1 = 0x0703F800;
        temp_v0_6->words.w0 = 0xF3000000;
        temp_v0_7 = gRegionAllocPtr++;
        temp_v0_7->words.w1 = 0;
        temp_v0_7->words.w0 = 0xE7000000;
        temp_v0_8 = gRegionAllocPtr++;
        temp_v0_8->words.w1 = 0x00080200;
        temp_v0_8->words.w0 = 0xF5400200;
        temp_v0_9 = gRegionAllocPtr++;
        temp_v0_9->words.w1 = 0x0003C03C;
        temp_v0_9->words.w0 = 0xF2000000;
        temp_v0_10 = gRegionAllocPtr++;
        temp_v0_10->words.w0 = 0xFD100000;
        temp_v0_10->words.w1 = (u32)arg0->unk28.word;
        temp_v0_11 = gRegionAllocPtr++;
        temp_v0_11->words.w1 = 0;
        temp_v0_11->words.w0 = 0xE8000000;
        temp_v0_12 = gRegionAllocPtr++;
        temp_v0_12->words.w1 = 0x07000000;
        temp_v0_12->words.w0 = 0xF5000100;
        temp_v0_13 = gRegionAllocPtr++;
        temp_v0_13->words.w1 = 0;
        temp_v0_13->words.w0 = 0xE6000000;
        temp_v0_14 = gRegionAllocPtr++;
        temp_v0_14->words.w1 = 0x0703C000;
        temp_v0_14->words.w0 = 0xF0000000;
        temp_v0_15 = gRegionAllocPtr++;
        temp_v0_15->words.w1 = 0;
        temp_v0_15->words.w0 = 0xE7000000;
        temp_v0_16 = gRegionAllocPtr++;
        temp_v0_16->words.w0 = 0x01020040;
        temp_v0_16->words.w1 = (u32)arg0->unk24.velocityX;
        temp_v0_17 = gRegionAllocPtr++;
        temp_v0_17->words.w0 = 0x01000040;
        temp_v0_17->words.w1 = (u32)D_80156614;
        temp_v0_18 = gRegionAllocPtr++;
        temp_v0_18->words.w1 = (u32)D_800D45E0;
        temp_v0_18->words.w0 = 0x0400103F;
        temp_v0_19 = gRegionAllocPtr++;
        temp_v0_19->words.w1 = 0x00060200;
        temp_v0_19->words.w0 = 0xB1060402;
    }
}
#endif

void func_8004E960(RaceItemEffectActor *arg0) {
    if (D_80121B56 == 0) {
        arg0->unk30.screen.x -= 0x30;
        arg0->unk30.screen.y += 3;
        if (arg0->unk30.screen.x < 0x21) {
            func_800716E4(arg0);
            return;
        }
    }
    func_800483FC(&D_801248EC, func_8004E604, arg0);
}

void func_8004E9D0(RaceItemEffectActor *arg0) {
    arg0->unk30.screen.x = 0xF0;
    arg0->unk30.screen.y = 0x10;
    func_80045990(func_80043040(D_8011216C), arg0->unk34.shorts.state.ubyte, &arg0->unk2C, &arg0->unk28.word);
    func_80071824(arg0, func_8004E960);
}

void func_8004EA34(s32 arg0, s32 arg1, s32 arg2, s16 arg3) {
    RaceItemEffectActor *p = func_800711D0(func_8004E9D0, 0, 2);

    if (p != NULL) {
        p->unk34.shorts.state.byte = arg3;
        p->payload.vec.x = arg0;
        p->payload.vec.y = arg1;
        p->payload.vec.z = arg2;
    }
}

void func_8004EAA8(RaceItemEffectActor *arg0) {
    RaceItemGfxCommandSource sp80;
    void *sp7C;
    void *sp78;
    volatile s32 pad[2];

    if (D_80156609 != 0) {
        arg0->unk34.shorts.state.bytes.matrixDirty = 1;
    }

    if (func_80049000(&arg0->payload.vec) != 0) {
        if (arg0->unk34.shorts.state.bytes.matrixDirty != 0) {
            arg0->unk34.shorts.state.bytes.matrixDirty = 0;
            sp80 = D_800DEE30;
            sp80.x = arg0->payload.vec.x;
            sp80.y = arg0->payload.vec.y;
            sp80.z = arg0->payload.vec.z;
            arg0->unk30.matrix = func_8004885C(&sp80);
        }

        do {
            if (arg0->unk30.matrix != NULL) {
                func_80045990(func_80043040(D_80112168),
                              (u16)(((s8)arg0->unk34.shorts.height.byte >> 1) + 0x36),
                              &sp7C, &sp78);

                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)D_800D9D00);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xFD500000, (u32)sp7C);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5500000, 0x07080200);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF3000000, 0x0703F800);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5400200, 0x80200);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF2000000, 0x3C03C);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)sp78);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01020040, (u32)arg0->unk30.matrix);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01000040, (u32)D_80156614);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)D_800D48A8);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
                RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)D_800D9D40);
            }
        } while (0);
    }
}

void func_8004EE0C(RaceItemEffectActor *arg0) {
    RaceItemFollowPlayer *player;
    RaceItemEffectActor *actor = arg0;

    if (D_80121B56 == 0) {
        arg0->unk34.shorts.height.byte++;
        if (arg0->unk34.shorts.height.byte == 6) {
            func_800716E4(arg0);
            return;
        }
    }
    if (actor->unk34.shorts.height.byte < 0) {
        actor->unk34.shorts.height.byte = 0;
    }
    func_80098590(D_80121D80[actor->playerIndex].transform, &actor->unk24.velocityX, &actor->payload);
    player = &D_80121D80[actor->playerIndex];
    actor->payload.vec.x += player->posA8.x;
    actor->payload.vec.y += player->posA8.y;
    actor->payload.vec.z += player->posA8.z;
    func_800483FC(&D_801248C8, func_8004EAA8, actor);
}

void func_8004EF24(RaceItemEffectActor *arg0) {
    RaceItemFollowPlayer *player;

    arg0->unk34.shorts.height.byte = -1;
    arg0->unk2C = 0;
    arg0->unk28.word = 0x280000;
    arg0->unk24.velocityX = 0x400000;
    player = &D_80121D80[arg0->playerIndex];
    if (player->flags2FC & 0x400) {
        arg0->unk24.velocityX = -arg0->unk24.velocityX;
    }
    player = &D_80121D80[arg0->playerIndex];
    func_80072A74(9, (SoundPosition *) &player->pos1C, 0x7F, 0x32);
    func_8004EE0C(arg0);
    func_80071824(arg0, func_8004EE0C);
}

void func_8004EFF8(RaceItemEffectActor *arg0) {
    volatile s32 pad0;
    RaceItemGfxCommandSource sp74;
    volatile u8 padding[0x14];
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
    Gfx *temp_v0_17;
    Gfx *temp_v0_18;

    if (D_80156609 != 0) {
        arg0->pad4E = 1;
    }

    if (func_80049000(&arg0->payload.vec) != 0) {
        if (arg0->pad4E != 0) {
            arg0->pad4E = 0;
            sp74 = D_800DEE30;
            sp74.x = arg0->payload.vec.x;
            sp74.y = arg0->payload.vec.y;
            sp74.z = arg0->payload.vec.z;
            arg0->unk34.matrix = func_8004885C(&sp74);
        }

        do { if (arg0->unk34.matrix != NULL) { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 6) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) 0x00) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) 0) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) D_800D4878; } ; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xFA000000; temp_v0_2->words.w1 = (arg0->unk38.width & 0xFF) | ~0xFF; temp_v0_3 = gRegionAllocPtr++; temp_v0_3->words.w0 = 0xFD500000; temp_v0_3->words.w1 = (u32) arg0->image; temp_v0_4 = gRegionAllocPtr++; temp_v0_4->words.w0 = 0xF5500000; temp_v0_4->words.w1 = 0x07080200; temp_v0_5 = gRegionAllocPtr++; temp_v0_5->words.w1 = 0; temp_v0_5->words.w0 = 0xE6000000; temp_v0_6 = gRegionAllocPtr++; temp_v0_6->words.w0 = 0xF3000000; temp_v0_6->words.w1 = 0x0703F800; temp_v0_7 = gRegionAllocPtr++; temp_v0_7->words.w1 = 0; temp_v0_7->words.w0 = 0xE7000000; temp_v0_8 = gRegionAllocPtr++; temp_v0_8->words.w0 = 0xF5400200; temp_v0_8->words.w1 = 0x00080200; temp_v0_9 = gRegionAllocPtr++; temp_v0_9->words.w0 = 0xF2000000; temp_v0_9->words.w1 = 0x0003C03C; temp_v0_10 = gRegionAllocPtr++; temp_v0_10->words.w0 = 0xFD100000; temp_v0_10->words.w1 = (u32) arg0->palette; temp_v0_11 = gRegionAllocPtr++; temp_v0_11->words.w1 = 0; temp_v0_11->words.w0 = 0xE8000000; temp_v0_12 = gRegionAllocPtr++; temp_v0_12->words.w0 = 0xF5000100; temp_v0_12->words.w1 = 0x07000000; temp_v0_13 = gRegionAllocPtr++; temp_v0_13->words.w1 = 0; temp_v0_13->words.w0 = 0xE6000000; temp_v0_14 = gRegionAllocPtr++; temp_v0_14->words.w0 = 0xF0000000; temp_v0_14->words.w1 = 0x0703C000; temp_v0_17 = gRegionAllocPtr++; temp_v0_17->words.w1 = 0; temp_v0_17->words.w0 = 0xE7000000; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) arg0->unk34.matrix; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((u32) ((((u32) 1) & ((0x01 << 8) - 1)) << 24)) | ((u32) ((((u32) ((0x00 | 0x00) | 0x00)) & ((0x01 << 8) - 1)) << 16))) | ((u32) ((((u32) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0)); _g->words.w1 = (u32) D_80156614; } ; temp_v0_18 = gRegionAllocPtr++; temp_v0_18->words.w0 = 0x0400103F; temp_v0_18->words.w1 = (u32) D_800D48E8; temp_v0_2 = gRegionAllocPtr++; temp_v0_2->words.w0 = 0xB1060402; temp_v0_2->words.w1 = 0x00060200; } } while (0);
    }
}

void func_8004F33C(RaceItemEffectActor *arg0) {
    s16 temp_v0;

    if (D_80121B56 == 0) {
        temp_v0 = arg0->unk30.particle.delay;
        if (temp_v0 != 0) {
            arg0->unk30.particle.delay = temp_v0 - 1;
            return;
        }

        arg0->payload.vec.x += arg0->unk24.velocityX;
        arg0->payload.vec.y += arg0->unk28.word;
        arg0->payload.vec.z += arg0->unk2C;
        arg0->unk28.word -= 0x4000;
        arg0->unk38.alpha -= 0x10;
        if (arg0->unk38.alpha <= 0) {
            func_800716E4(arg0);
            return;
        }
    }

    if (arg0->unk30.particle.drawInitialized == 0) {
        arg0->unk30.particle.drawInitialized++;
    }
    func_800483FC(&D_801248EC, func_8004EFF8, arg0);
}

void func_8004F3FC(RaceItemEffectActor *arg0) {
    char padTail[8];
    volatile s32 pad0[1];
    FixedMatrix3sScratch sp3C;
    RaceItemEffectPayload sp30;
    volatile s32 pad1[1];
    RaceItemFollowPlayer *player;
    Vec2s *angles;

    arg0->unk38.width = 0xFF;
    arg0->unk30.screen.y = func_800430D0() & 3;
    func_80045990(func_80043040(D_80112168), D_800D46F8[arg0->playerIndex], &arg0->image, &arg0->palette);

    player = &D_80121D80[arg0->followPlayerIndex];
    arg0->payload.vec.x = player->pos.x;
    arg0->payload.vec.y = player->pos.y;
    arg0->payload.vec.z = player->pos.z;

    angles = &D_800D4928[arg0->angleIndex & 0xFF];
    func_80097FE4(sp3C, angles->x, angles->y);
    func_80098590(sp3C, D_800D4968, &sp30);

    arg0->payload.vec.x += sp30.vec.x;
    arg0->payload.vec.y += sp30.vec.y;
    arg0->payload.vec.z += sp30.vec.z;

    func_80098590(sp3C, D_800D4974, (RaceItemEffectPayload *) &arg0->unk24);
    if (arg0->playerIndex == 0) {
        arg0->payload.vec.y += 0x60000;
    }

    func_8004F33C(arg0);
    func_80071824(arg0, func_8004F33C);
}

void func_8004F55C(s16 playerIndex, s16 itemIndex) {
    RaceItemEffectActor *actor;
    void *callback;
    s32 itemType;
    s32 angleIndex;

    itemType = func_8004DB60(itemIndex);
    if (itemType != 4) {
        if (itemType == 1) {
            func_80072A74(0x12, (SoundPosition *) &D_80121D80[playerIndex].pos1C, 0x7F, 0x32);
        } else {
            func_80072A74(0x11, (SoundPosition *) &D_80121D80[playerIndex].pos1C, 0x7F, 0x32);
        }

        callback = func_8004F3FC;
        for (angleIndex = 0; angleIndex != 0x10; angleIndex++) {
            actor = func_800716A4(callback, 5, 2, itemType);
            if (actor != NULL) {
                actor->followPlayerIndex = playerIndex;
                actor->angleIndex = angleIndex;
            }
        }
    }
}

void func_8004F68C(RaceItemEffectActor *arg0) {
    RaceItemGfxCommandSource sp78;
    void *sp74;
    void *sp70;

    if (D_80156609 != 0) {
        arg0->unk28.bytes.unk28 = 1;
    }

    if (func_80049000(&arg0->payload.vec) != 0) {
        if (arg0->unk28.bytes.unk28 != 0) {
            arg0->unk28.bytes.unk28 = 0;
            sp78 = D_800DEE30;
            sp78.x = arg0->payload.vec.x;
            sp78.y = arg0->payload.vec.y;
            sp78.z = arg0->payload.vec.z;
            arg0->unk24.velocityX = (s32)func_8004885C(&sp78);
        }

        if (arg0->unk24.velocityX != 0) {
            func_80045990(func_80043040(D_80112168),
                          ((s8)arg0->unk28.bytes.phase >> 1) + 0x2F,
                          &sp74, &sp70);

            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)D_800D6270);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xFD500000, (u32)sp74);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5500000, 0x07080200);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF3000000, 0x070FF400);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5400400, 0x80200);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF2000000, 0x7C07C);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)sp70);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01020040, arg0->unk24.velocityX);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01000040, D_80156614);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)D_800D4980);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
        }
    }
}

void func_8004F9CC(RaceItemEffectActor *arg0) {
    if (D_80121B56 == 0) {
        arg0->unk28.bytes.phase++;
        if (arg0->unk28.bytes.phase == 0xC) {
            func_800716E4(arg0);
            return;
        }
    }
    if (arg0->unk28.bytes.phase < 0) {
        arg0->unk28.bytes.phase = 0;
    }
    func_800483FC(&D_801248C8, func_8004F68C, arg0);
}

void func_8004FA44(RaceItemEffectActor *arg0) {
    arg0->unk28.bytes.phase = -1;
    arg0->payload.vec.x = D_80121EE8[arg0->playerIndex].pos.x + ((func_800430D0() - 0x80) << 10);
    arg0->payload.vec.y = D_80121EE8[arg0->playerIndex].pos.y + ((func_800430D0() - 0x80) << 10);
    arg0->payload.vec.z = D_80121EE8[arg0->playerIndex].pos.z + ((func_800430D0() - 0x80) << 10);
    func_8004F9CC(arg0);
    func_80071824(arg0, func_8004F9CC);
}

void func_8004FB44(RaceItemFollowActor *arg0) {
    RaceItemGfxCommandSource sp90;
    void *sp8C;
    void *sp88;
    volatile s32 pad[2];

    if (D_80156609 != 0) {
        arg0->dirty = 1;
    }
    if (func_80049000(&arg0->pos1) != 0) {
        if (arg0->dirty != 0) {
            arg0->dirty = 0;
            sp90 = D_800DEE30;
            sp90.x = arg0->pos1.x;
            sp90.y = arg0->pos1.y;
            sp90.z = arg0->pos1.z;
            arg0->matrix1 = func_8004885C(&sp90);
            sp90.x = arg0->pos2.x;
            sp90.y = arg0->pos2.y;
            sp90.z = arg0->pos2.z;
            arg0->matrix2 = func_8004885C(&sp90);
        }
        if (arg0->matrix2 != NULL) {
            func_80045990(func_80043040(D_80112168), (u16)((((s8)arg0->timer) >> 2) + 0x39), &sp8C, &sp88);

            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)D_800D6270);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xFD500000, (u32)sp8C);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5500000, 0x07080200);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF3000000, 0x0703F800);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5400200, 0x80200);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF2000000, 0x3C03C);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)sp88);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01020040, (u32)arg0->matrix1);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01000040, (u32)D_80156614);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)D_800D49C0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01020040, (u32)arg0->matrix2);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01000040, (u32)D_80156614);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)D_800D49C0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
        }
    }
}

void func_8004FF34(RaceItemFollowActor *arg0) {
    RaceItemFollowPlayer *player;
    RaceItemFollowActor *temp_a2 = arg0;

    if (D_80121B56 == 0) {
        arg0->timer++;
        player = &D_80121D80[arg0->playerIndex];
        arg0->pos1.x = arg0->offset1.x + player->pos.x;
        arg0->pos1.y = arg0->offset1.y + player->pos.y;
        arg0->pos1.z = arg0->offset1.z + player->pos.z;
        arg0->pos2.x = arg0->offset2.x + player->pos.x;
        arg0->pos2.y = arg0->offset2.y + player->pos.y;
        arg0->pos2.z = arg0->offset2.z + player->pos.z;
        if (arg0->timer == 0x18) {
            func_800716E4(arg0);
            return;
        }
    }
    if (temp_a2->timer < 0) {
        temp_a2->timer = 0;
    }
    func_800483FC(&D_801248C8, func_8004FB44, temp_a2);
}

void func_80050030(RaceItemFollowActor *arg0) {
    RaceItemFollowPlayer *player;

    arg0->timer = -1;
    player = &D_80121D80[arg0->playerIndex];
    if (player->flags2FC & 0x400) {
        arg0->offset1.x = player->unk4A0.x - player->pos.x;
        arg0->offset1.y = player->unk4A0.y - player->pos.y;
        arg0->offset1.z = player->unk4A0.z - player->pos.z;
        arg0->offset2.x = player->unk4B8.x - player->pos.x;
        arg0->offset2.y = player->unk4B8.y - player->pos.y;
        arg0->offset2.z = player->unk4B8.z - player->pos.z;
    } else {
        arg0->offset1.x = player->unk4AC.x - player->pos.x;
        arg0->offset1.y = player->unk4AC.y - player->pos.y;
        arg0->offset1.z = player->unk4AC.z - player->pos.z;
        arg0->offset2.x = player->unk4C4.x - player->pos.x;
        arg0->offset2.y = player->unk4C4.y - player->pos.y;
        arg0->offset2.z = player->unk4C4.z - player->pos.z;
    }
    func_8004FF34(arg0);
    func_80071824(arg0, func_8004FF34);
}

void func_8005019C(RaceItemEffectActor *arg0) {
    if ((u8)arg0->payload.sprite.colorR == D_80156608) {
        if ((u8)arg0->payload.sprite.colorG == 0) {
            if ((u8)arg0->payload.sprite.colorB == 0) {
                func_80046D68(arg0->payload.sprite.x, arg0->payload.sprite.y, func_80043040(D_8011216E),
                              ((arg0->payload.sprite.frame >> 1) + 0x5C) & 0xFFFF, 0x1D);
            } else {
                func_80046D68(arg0->payload.sprite.x, arg0->payload.sprite.y, func_80043040(D_8011216E),
                              ((arg0->payload.sprite.frame >> 1) + 0x5C) & 0xFFFF, 0x1E);
            }
        } else if ((u8)arg0->payload.sprite.colorB == 0) {
            func_8004767C((s16)(arg0->payload.sprite.x - 8), (s16)(arg0->payload.sprite.y - 8),
                          func_80043040(D_8011216E), ((arg0->payload.sprite.frame >> 1) + 0x5C) & 0xFFFF,
                          0x1D, 1);
        } else {
            func_8004767C((s16)(arg0->payload.sprite.x - 8), (s16)(arg0->payload.sprite.y - 8),
                          func_80043040(D_8011216E), ((arg0->payload.sprite.frame >> 1) + 0x5C) & 0xFFFF,
                          0x1E, 1);
        }
    }
}

void func_80050340(RaceItemEffectActor *arg0) {
    u16 temp = arg0->payload.sprite.frame + 1;

    arg0->payload.sprite.frame++;
    if ((((arg0->payload.sprite.frame) + 1) - 1) >= 0x10) {
        func_800716E4(arg0);
    } else {
        func_800483FC(&D_80124878, func_8005019C, arg0);
    }
}

void func_80050398(RaceItemEffectActor *arg0) {
    arg0->payload.sprite.frame = 0xFFFF;
    func_80050340(arg0);
    func_80071824(arg0, func_80050340);
}

void func_800503D0(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4) {
    RaceItemEffectActor *temp_v0;

    temp_v0 = func_800711D0(func_80050398, 5, 3);
    if (temp_v0 != NULL) {
        temp_v0->payload.sprite.x = arg0 - 8;
        temp_v0->payload.sprite.y = arg1 - 8;
        temp_v0->payload.sprite.colorR = arg2;
        temp_v0->payload.sprite.colorG = arg3;
        temp_v0->payload.sprite.colorB = arg4;
    }
}

// func_80050458 best match: 93.825% (nonmatchings/func_80050458-7123131487808489545/base_7.c)

#pragma GLOBAL_ASM("asm/nonmatchings/race_item_effects/func_80050458.s")

#ifdef NON_MATCHING
void func_80050458(RaceItemTextureActor *arg0) {
    volatile u8 pad[0xC];
    RaceItemGfxCommandSource sp94;
    volatile u8 pad2[0x10];
    s32 i;
    RaceItemDrawNode **head;
    RaceItemDrawNode *node;
    Gfx *gfx;

    sp94 = D_800DEE30;
    if (D_80156609 != 0) {
        head = D_801121D0.heads;
        do {
            node = *head++;
            if (node != NULL) {
                do {
                    node->matrixDirty = 1;
                    node = node->next;
                } while (node != NULL);
            }
        } while (head != D_801121E0.heads);
    }

    gfx = gRegionAllocPtr++;
    gfx->words.w1 = (u32)D_800D9D00;
    gfx->words.w0 = 0x06000000;

    head = D_801121D0.heads;
    i = 0;
    do {
        node = *head;
        if (node != NULL) {
            gDPLoadTextureBlock_4b(gRegionAllocPtr++, arg0->images[i], G_IM_FMT_CI, 16, 16, 0,
                                   G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 0, 0,
                                   G_TX_NOLOD, G_TX_NOLOD);
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palettes[i]);
        }
        if (node != NULL) {
            do {
                if (func_80049000(node->pos) != 0) {
                    if (node->matrixDirty != 0) {
                        node->matrixDirty = 0;
                        sp94.x = node->pos->x;
                        sp94.y = node->pos->y;
                        sp94.z = node->pos->z;
                        node->matrix = func_8004885C(&sp94);
                    }

                    gfx = gRegionAllocPtr++;
                    gfx->words.w0 = 0x01020040;
                    gfx->words.w1 = (u32)node->matrix;

                    gfx = gRegionAllocPtr++;
                    gfx->words.w0 = 0x01000040;
                    gfx->words.w1 = (u32)D_80156614;

                    gfx = gRegionAllocPtr++;
                    gfx->words.w0 = 0x0400103F;
                    gfx->words.w1 = node->displayList;

                    gfx = gRegionAllocPtr++;
                    gfx->words.w0 = 0xB1060402;
                    gfx->words.w1 = 0x00060200;
                }
                node = node->next;
            } while (node != NULL);
        }
        head++;
        i++;
    } while (head != D_801121E0.heads);

    gfx = gRegionAllocPtr++;
    gfx->words.w0 = 0x06000000;
    gfx->words.w1 = (u32)D_800D9D40;
}
#endif

// func_80050888 best match: 66.842%

#pragma GLOBAL_ASM("asm/nonmatchings/race_item_effects/func_80050888.s")

#ifdef NON_MATCHING
void func_80050888(RaceItemEffectActor *arg0) {
    D_801121D0.heads[0] = NULL;
    D_801121D0.heads[1] = NULL;
    D_801121D0.heads[2] = NULL;
    D_801121D0.heads[3] = NULL;
    func_800483FC(&D_801248E0, func_80050458, arg0);
}
#endif

void func_800508D0(RaceItemEffectActor *arg0) {
    u16 *var_s0;
    RaceItemEffectActor *callbackActor;
    s32 var_s1;
    volatile short pad;
    s32 *var_s2;
    volatile RaceItemEffectAssetHandles *new_var2;
    s32 *var_s3;
    volatile RaceItemEffectAssetHandles *var_s4;
    s32 var_s5;
    RaceItemEffectActor *actor;
    void (*new_var)(RaceItemEffectActor *);
    RaceItemEffectActor *var_s6;

    var_s4 = &D_80112130;
    if (1 != 0) {
    }
    do { var_s0 = D_800D46F8; if (1) { } var_s6 = arg0; actor = var_s6; { } var_s1 = 0; var_s2 = &actor->payload.vec.x; var_s3 = &actor->unk28.word; if (1) { } arg0 = arg0; if (arg0 && arg0) { } var_s5 = 0x10; if (1) { } new_var = func_80050888; callbackActor = actor; do { func_80045990((0, func_80043040((new_var2 = var_s4)->itemTextureHandle)), *var_s0, var_s2, var_s3); var_s1 += 4; var_s0++; var_s2++; var_s3++; } while (var_s1 != var_s5); } while (0);

    func_80050888(callbackActor);
    func_80071824(callbackActor, new_var);
}

void func_8005098C(RaceItemFollowActor *arg0) {
    RaceItemGfxCommandSource sp98;
    void *sp94;
    void *sp90;
    volatile s32 pad[2];

    if (D_80156609 != 0) {
        arg0->dirty = 1;
    }
    if (func_80049000(&arg0->pos1) != 0) {
        if (arg0->dirty != 0) {
            arg0->dirty = 0;
            sp98 = D_800DEE30;
            sp98.x = arg0->pos1.x;
            sp98.y = arg0->pos1.y;
            sp98.z = arg0->pos1.z;
            arg0->matrix1 = func_8004885C(&sp98);
            sp98.x = arg0->pos2.x;
            sp98.y = arg0->pos2.y;
            sp98.z = arg0->pos2.z;
            arg0->matrix2 = func_8004885C(&sp98);
        }
        if (arg0->matrix2 != NULL) {
            func_800459D4((u8 *)func_80043040(D_80112168), (u16)((((s8)arg0->timer) >> 2) + 0x39), 0x12, &sp94, &sp90);

            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x06000000, (u32)D_800D6270);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xFD500000, (u32)sp94);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5500000, 0x07080200);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF3000000, 0x0703F800);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5400200, 0x80200);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF2000000, 0x3C03C);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xFD100000, (u32)sp90);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE8000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF5000100, 0x07000000);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE6000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xF0000000, 0x0703C000);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xE7000000, 0);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01020040, (u32)arg0->matrix1);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01000040, (u32)D_80156614);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)D_800D4A00);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01020040, (u32)arg0->matrix2);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x01000040, (u32)D_80156614);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0x0400103F, (u32)D_800D4A00);
            RACE_ITEM_GFX_CMD(gRegionAllocPtr++, 0xB1060402, 0x60200);
        }
    }
}

void func_80050D84(RaceItemFollowActor *arg0) {
    RaceItemFollowPlayer *player;
    RaceItemFollowActor *temp_a2 = arg0;

    if (D_80121B56 == 0) {
        arg0->timer++;
        player = &D_80121D80[arg0->playerIndex];
        arg0->pos1.x = arg0->offset1.x + player->pos.x;
        arg0->pos1.y = arg0->offset1.y + player->pos.y;
        arg0->pos1.z = arg0->offset1.z + player->pos.z;
        arg0->pos2.x = arg0->offset2.x + player->pos.x;
        arg0->pos2.y = arg0->offset2.y + player->pos.y;
        arg0->pos2.z = arg0->offset2.z + player->pos.z;
        if (arg0->timer == 0x18) {
            func_800716E4(arg0);
            return;
        }
    }
    if (temp_a2->timer < 0) {
        temp_a2->timer = 0;
    }
    func_800483FC(&D_801248C8, func_8005098C, temp_a2);
}

void func_80050E80(RaceItemFollowActor *arg0) {
    RaceItemFollowPlayer *player;

    arg0->timer = -1;
    player = &D_80121D80[arg0->playerIndex];
    if (player->flags2FC & 0x400) {
        arg0->offset1.x = player->unk4A0.x - player->pos.x;
        arg0->offset1.y = player->unk4A0.y - player->pos.y;
        arg0->offset1.z = player->unk4A0.z - player->pos.z;
        arg0->offset2.x = player->unk4B8.x - player->pos.x;
        arg0->offset2.y = player->unk4B8.y - player->pos.y;
        arg0->offset2.z = player->unk4B8.z - player->pos.z;
    } else {
        arg0->offset1.x = player->unk4AC.x - player->pos.x;
        arg0->offset1.y = player->unk4AC.y - player->pos.y;
        arg0->offset1.z = player->unk4AC.z - player->pos.z;
        arg0->offset2.x = player->unk4C4.x - player->pos.x;
        arg0->offset2.y = player->unk4C4.y - player->pos.y;
        arg0->offset2.z = player->unk4C4.z - player->pos.z;
    }
    func_80050D84(arg0);
    func_80071824(arg0, func_80050D84);
}
