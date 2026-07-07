#include "common.h"
#include "viewport_manager.h"

typedef struct {
    s32 word;
    u16 half;
    u8 b6;
    u8 b7;
} RaceUiSpriteInit;

typedef struct {
    s32 a;
    s32 b;
    s32 c;
} Vec3i;

typedef s16 FixedMatrix3sScratch[0x12];

typedef union {
    s32 word;
    struct {
        s16 hi;
        s16 lo;
    } half;
} SplitWord;

typedef struct {
    /* 0x00 */ u16 assetId;
    /* 0x02 */ u8 pad2[6];
} RaceUiAssetEntry;

typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ Vec3i pos1C;
    /* 0x028 */ Vec3i pos28;
    /* 0x034 */ u8 pad34[0x2C0 - 0x34];
    /* 0x2C0 */ s16 unk2C0;
    /* 0x2C2 */ s8 pad2C2;
    /* 0x2C3 */ s8 unk2C3;
    /* 0x2C4 */ u8 pad2C4[0x2EA - 0x2C4];
    /* 0x2EA */ s16 pitch;
    /* 0x2EC */ s16 yaw;
    /* 0x2EE */ u8 pad2EE[0x574 - 0x2EE];
    /* 0x574 */ s16 score;
    /* 0x576 */ s16 targetScore;
    /* 0x578 */ u8 pad578[0x60C - 0x578];
} RacePlayerState;

typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ s16 mainFontHandle;
    /* 0x3A */ u8 pad3A[0x48 - 0x3A];
    /* 0x48 */ s16 resultTextHandle;
    /* 0x4A */ s16 pad4A;
    /* 0x4C */ s16 rankTextHandle;
} RaceUiAssetHandles;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ u8 pad18[0x24 - 0x18];
    /* 0x24 */ s16 transform[0x22];
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s16 unk6C;
    /* 0x6E */ s16 unk6E;
} RaceUiTransitionActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ s16 timer;
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ s32 x;
    /* 0x20 */ s16 angle;
    /* 0x22 */ u8 pad22[2];
    /* 0x24 */ s32 value;
    /* 0x28 */ s32 velocity;
    /* 0x2C */ s32 soundIndex;
} RaceUiSlideActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ s16 alpha;
    /* 0x1A */ s16 timer;
    /* 0x1C */ s16 state;
    /* 0x1E */ s16 value;
    /* 0x20 */ s16 bonus;
    /* 0x22 */ s16 target;
    /* 0x24 */ s16 flag;
} RaceUiCounterActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ s16 angle;
    /* 0x26 */ s16 angle2;
    /* 0x28 */ s16 frame;
    /* 0x2A */ s16 frameTarget;
    /* 0x2C */ s16 rotX;
    /* 0x2E */ s16 rotY;
    /* 0x30 */ s16 scale;
    /* 0x32 */ s16 scaleStep;
} RaceUiModelActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ void *ptr18;
    /* 0x1C */ void *ptr1C;
    /* 0x20 */ void *ptr20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ Vec3i pos;
    /* 0x34 */ s16 frame;
    /* 0x36 */ s16 frameEnd;
    /* 0x38 */ Vec3i assetPos;
    /* 0x44 */ s32 assetX;
    /* 0x48 */ Vec3i vec48;
    /* 0x54 */ s16 state;
    /* 0x56 */ s16 timer;
    /* 0x58 */ s32 velX;
    /* 0x5C */ s32 velY;
    /* 0x60 */ s32 velZ;
    /* 0x64 */ u8 pad64[4];
    /* 0x68 */ s16 unk68;
} RaceUiEffectActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ Vec3i velocity;
    /* 0x30 */ u8 pad30[0x36 - 0x30];
    /* 0x36 */ s16 flags;
    /* 0x38 */ u8 pad38[0x50 - 0x38];
    /* 0x50 */ s32 verticalVelocity;
    /* 0x54 */ s32 verticalAcceleration;
} RaceUiProjectileActor;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
} RaceUiEffectParticle;

typedef struct {
    /* 0x00 */ s32 words[8];
} RaceUiTrailCopyBlock;

typedef struct {
    /* 0x00 */ s16 state;
    /* 0x02 */ u8 pad02[0x1C - 0x02];
    /* 0x1C */ Vec3i worldPos;
    /* 0x28 */ RaceUiTrailCopyBlock copyBlock;
    /* 0x48 */ u8 pad48[0x6A - 0x48];
    /* 0x6A */ s16 spinYaw;
} RaceUiSnowboardTrailState;

typedef struct {
    /* 0x000 */ u16 playerIndex;
    /* 0x002 */ u8 pad002[0x58C - 0x002];
    /* 0x58C */ RaceUiSnowboardTrailState trail;
} RaceUiSnowboardTrailPlayer;

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ Vec3i worldPos;
    /* 0x3C */ u8 pad3C[4];
    /* 0x40 */ RaceUiTrailCopyBlock copyBlock;
    /* 0x60 */ u8 pad60[0x80 - 0x60];
    /* 0x80 */ u16 playerIndex;
    /* 0x82 */ u8 pad82[2];
    /* 0x84 */ s16 spinYaw;
} RaceUiSnowboardTrailActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ RaceUiEffectParticle *particles;
    /* 0x1C */ u8 pad1C[8];
    /* 0x24 */ s16 count;
} RaceUiEffectParticleActor;

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
} RaceUiGfxCommandDest;

typedef struct {
    /* 0x00 */ s16 active;
    /* 0x02 */ s16 sentinel;
    /* 0x04 */ s32 command[3];
} RaceUiGfxCommandScriptEntry;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ RaceUiGfxCommandDest *particles;
    /* 0x1C */ u8 pad1C[2];
    /* 0x1E */ s16 count;
} RaceUiGfxCommandActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ u16 timer;
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ s32 x;
    /* 0x20 */ SplitWord y;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s32 velocity;
    /* 0x2C */ void *parent;
    /* 0x30 */ s8 playerIndex;
} RaceUiPopupActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 index;
    /* 0x12 */ u8 pad12[0x18 - 0x12];
    /* 0x18 */ u32 timer;
    /* 0x1C */ s16 y;
} RaceUiPromptActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s16 alpha;
} RaceUiAlphaActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x1A];
    /* 0x1A */ s16 alpha;
} RaceUiAlpha1AActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 alpha;
} RaceUiAlpha18Actor;

typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u8 pad4[0x608];
} RacePlayerFlags;

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ u16 *script;
    /* 0x34 */ s16 timer;
    /* 0x36 */ s16 value;
    /* 0x38 */ u8 pad38[8];
    /* 0x40 */ s32 assetY;
    /* 0x44 */ s32 assetX;
} RaceUiScriptActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 x;
    /* 0x1C */ s32 y;
    /* 0x20 */ s32 z;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s32 velocity;
    /* 0x2C */ u8 pad2C[4];
    /* 0x30 */ s16 timer;
    /* 0x32 */ u8 pad32[6];
    /* 0x38 */ void *image3A;
    /* 0x3C */ void *palette3A;
    /* 0x40 */ void *image3B;
    /* 0x44 */ void *palette3B;
} RaceUiOverlayActor;

extern RaceUiSpriteInit D_800D5FF0[];

extern Vec3i D_800D61C0[];
extern Vec3i D_800D6340[];
extern Vec3i D_800D6220[];
extern Vec3i D_800D6330[];
extern Vec3i D_800D62AC[];
extern s16 D_800D633C[];

extern void *D_80124868;
extern void *D_80124858;
extern void *D_80124888;
extern void *D_801248B0;
extern void *D_801248BC;
extern void *D_801248C8;
extern u8 D_80121B55;
extern u8 D_80121B56;
extern s8 D_80121B54;
extern u8 D_80121B81;
extern u8 D_80121D90;
extern s32 D_80121DA4;
extern u8 D_80156608;
extern s16 D_80156612;
extern s16 D_8011216E;
extern s16 D_80112168;
extern s8 D_80122289;
extern s16 D_801235B0;
extern s16 D_8011216C;
extern RaceUiAssetHandles D_80112130;
extern s16 D_801222F6;
extern s16 D_801222F2;
extern void *D_80124878;
extern void *D_801248A4;
extern void *D_801248EC;
extern s16 D_800D6050[];
extern Vec3i D_800D6030[];
extern s16 *D_800D761C[];
extern RaceUiGfxCommandScriptEntry *D_800D693C[];
extern RaceUiGfxCommandDest D_800DEE50;
extern void func_80071824(void *task, void (*callback)());
extern void *func_800711D0(void *, s32, s32);
extern void *func_80071408(void *, s32, s32);
extern void func_800483FC(void *, void *, s32);
extern void func_80048C90(RaceUiGfxCommandDest *, s32 *);
extern void func_80048D60(void *);
extern void osWritebackDCache(void *, s32);
extern s32 func_80043040(s16);
extern s32 func_800430D0(void);
extern s16 func_80042D58(s32);
extern void func_80045A78(s32, s32, s32, s32);
extern void func_80045990(s32, s32, void *, void *);
extern void func_80097BAC(s16 *, s16);
extern void func_80097C18(void *, s32);
extern void func_80097FE4(FixedMatrix3sScratch, s16, s16);
extern void func_80098590(FixedMatrix3sScratch, Vec3i *, Vec3i *);
extern void func_8005F448(void *);
extern void func_8005B14C(void *);
extern void func_8005C64C(void *);
extern void func_8005DE6C(void *);
extern void func_8005CB74(void *);
extern void func_800623E8(void *);
extern s32 func_8007D200(s32, s32, s32);
extern s32 func_80080CC4(s32, s32, s32);
extern void func_800716E4(void *);
extern void *func_800716A4(void *, s32, s32);
extern void func_80072A74(s32, void *, s32, s32);
extern void func_80072A20(s32, void *, s32, s32, f32, s32);
extern s32 func_8007B130(void *, void *, void *, void *);
extern RacePlayerState D_80121D80[];
extern RacePlayerFlags D_8012207C[];
extern u8 D_800EC9F0[];
extern void *D_80121B74;
extern void func_80072138(s32, s32);
extern void func_80048278(s32, s32, void *, s32);
extern void func_80059A04(void *, s32, s32, s32);
extern void *D_800E1220;
extern void *D_800E1230;
extern void *D_800E1368;
extern void *D_800E136C;
extern void *D_800E1370;
extern void *D_800E1374;
extern void *D_800E1378;
extern void *D_800E14B8;
extern void *D_800E1474;
extern void *D_800E1484;
extern void *D_800E1494;
extern void *D_800E14A8;
extern void *D_800E14BC;
extern void *D_800E14C0;
extern void *D_800E14C4;
extern void *D_800E14C8;
extern u32 D_800DEFF8[];
extern u32 D_800E12D4[];
extern u32 D_800E1390[];
extern Gfx *gRegionAllocPtr;
extern RaceUiAssetEntry D_800D5FF4[];
extern u16 D_800D6520[];
extern s32 D_80123778;
extern s32 D_801235B4;
extern void func_80057E10(void *);
extern void func_800615BC(void);
extern void func_800640D8(void);
extern void func_80057710(RaceUiPromptActor *);
extern void func_80057B60(RaceUiPopupActor *);
extern void func_80057D68(RaceUiPopupActor *);
extern void func_80058C00(void);
extern void func_8005905C(void);
extern void func_80059518(void *);
extern void func_80059950(void *);
extern void func_8005A288(void *);
extern void func_8005E5B4(void *);
extern void func_8005F174(void);
extern void func_80061088(void);
extern void func_80062F6C(void);
extern void func_80058B20(void *);
extern void func_800628DC(void);
extern void func_80060FA4(void *);
extern void func_80061CA8(void);
extern void func_800634C8(void);
extern void func_80064470(RaceUiProjectileActor *);
extern void func_80064914(RaceUiProjectileActor *);
extern void func_80064B28(RaceUiProjectileActor *);
extern void func_80057AA4(RaceUiPopupActor *);
extern void func_80057CAC(RaceUiPopupActor *);
extern void func_80060E7C(void *);
extern void func_80060D10(void);
extern void func_8006501C(void *);
extern void func_80064F4C(void *);
extern void func_80059E5C(RaceUiAlpha1AActor *);
extern void func_80059C34(void);
extern void func_8005BE68(void);
extern void func_80061984(void);
extern void func_80063220(void);
extern void func_80057854(void);
extern void func_800621DC(void *);
extern void func_8005A1FC(void *);
extern void func_8005A31C(void);
extern void func_8005A884(void);
extern void func_8005AAE4(void);
extern void func_8005AE1C(void *);
extern void func_8005A4BC(void);
extern void func_8005AC44(void);
extern void func_8005B9F8(void);
extern void func_8005C14C(void);
extern void func_8005A0E0(void *);
extern void func_80061F38(void);
extern void func_8005F828(void);
extern void func_8005FBA8(void *);
extern void func_8005FED0(void *);
extern void func_8005CF60(void);
extern void func_8005D558(void);
extern void func_8005D860(void);
extern void func_8005DB3C(void *);
extern void func_8005D1CC(void);
extern void func_8005D9B4(void);
extern void func_800601F8(void *);
extern void func_800602BC(void *);
extern void func_800589F4(void *);
extern void func_80057E90(void);
extern void func_80058360(void);
extern void func_80065D24(void);
extern void func_80065808(void);
extern void func_80065508(void);
extern void func_80066158(void *);
extern void func_800663C8(void *);
extern void func_80059854(void *);
extern void func_8005804C(void);
extern void func_8005812C(void);
extern void func_8005827C(void);
extern void func_800572A0(void *);
extern void func_80057548(RaceUiSlideActor *);
extern void func_8005B6F8(void *);
extern void func_8005CD10(void *);
extern void func_8005E3F8(void *);
extern void func_80064D88(RaceUiProjectileActor *);
extern void func_80062D34(void);
extern s32 func_80043120(void);
extern void func_80063A9C(void);
extern void func_8005B61C(void *);
extern void func_8005CC54(void *);
extern void func_8005E33C(void *);
extern void func_8005893C(void *);
extern void func_80060544(void);
extern void func_80060914(void);
extern void func_80058610(void *);
extern void func_80058880(void *);
extern s16 D_80121B50;
extern void *D_801248D4;

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_800572A0.s")

void func_80057548(RaceUiSlideActor *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->soundIndex;
    arg0->angle = arg0->angle + 2;
    arg0->angle = arg0->angle & 0x3F;
    if (temp_v0 != 0) {
        func_80072A20(0xE, &D_800D6030[temp_v0], 0x7F, 0x32, 0.0f, 0xA);
    }
    if (arg0->velocity != 0) {
        func_800483FC(&D_801248EC, func_800572A0, arg0);
        return;
    }
    func_800483FC(&D_801248A4, func_800572A0, arg0);
}

void func_80057600(RaceUiSlideActor *arg0) {
    arg0->angle = 0;
    func_80045990(func_80043040(D_80112168), D_800D5FF4[arg0->index].assetId, &arg0->timer, &arg0->x);
    arg0->value = D_800D5FF0[arg0->index].word;
    arg0->velocity = D_800D5FF0[arg0->index].b6;
    arg0->soundIndex = D_800D5FF0[arg0->index].b7;
    func_80071824(arg0, func_80057548);
}

void func_80057694(RaceUiPromptActor *arg0) {
    if (D_80156608 == arg0->index) {
        if (D_80156612 & 1) {
            func_80048278(-0x34, arg0->y, &D_800E1220, 0);
            return;
        }
        func_80048278(-0x34, arg0->y, &D_800E1230, 1);
    }
}

void func_80057710(RaceUiPromptActor *arg0) {
    if (D_8012207C[arg0->index].flags & 0x400) {
        if (arg0->timer < 0x1E) {
            arg0->timer++;
        }
    } else {
        arg0->timer = 0;
    }
    if (D_8012207C[arg0->index].flags & 0x1040) {
        arg0->timer = 0;
    }
    if (arg0->timer >= 0x1E) {
        func_800483FC(&D_80124888, func_80057694, (s32) arg0);
    }
}


void func_80057810(RaceUiPromptActor *arg0) {
    arg0->y = -0x10;
    if (D_80121B55 != 1) {
        arg0->y = -8;
    }
    func_80071824(arg0, func_80057710);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80057854.s")

void func_80057AA4(RaceUiPopupActor *arg0) {
    arg0->x += arg0->velocity;
    arg0->velocity += 4;
    if (arg0->velocity == 0x38) {
        func_800716E4(arg0);
    } else {
        func_800483FC(&D_80124878, func_80057854, arg0);
    }
}

void func_80057B04(RaceUiPopupActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        func_80071824(arg0, func_80057AA4);
    }
    func_800483FC(&D_80124878, func_80057854, arg0);
}

void func_80057B60(RaceUiPopupActor *arg0) {
    arg0->x -= arg0->velocity;
    arg0->velocity -= 4;
    if (arg0->velocity == 0) {
        arg0->timer = 0x2D;
        func_80071824(arg0, func_80057B04);
    }
    func_800483FC(&D_80124878, func_80057854, arg0);
}

void func_80057BCC(RaceUiPopupActor *arg0) {
    arg0->y.word = -0x2C;
    arg0->velocity = 0x38;
    arg0->x = 0x1A4;
    func_80071824(arg0, func_80057B60);
}

void func_80057C08(void *arg0, s16 arg1) {
    RaceUiPopupActor *temp = func_80071408(func_80057BCC, 0, 0x64);
    if (temp != NULL) {
        temp->parent = arg0;
        temp->playerIndex = arg1;
    }
}

void func_80057C5C(RaceUiPopupActor *arg0) {
    if (D_80156608 == 0) {
        func_80045A78(-0x3C, arg0->y.half.lo, func_80043040(D_8011216E), 0x8F);
    }
}

void func_80057CAC(RaceUiPopupActor *arg0) {
    arg0->x += arg0->velocity;
    arg0->velocity += 4;
    if (arg0->velocity == 0x38) {
        func_800716E4(arg0);
    } else {
        func_800483FC(&D_80124878, func_80057C5C, arg0);
    }
}

void func_80057D0C(RaceUiPopupActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        func_80071824(arg0, func_80057CAC);
    }
    func_800483FC(&D_80124878, func_80057C5C, arg0);
}

void func_80057D68(RaceUiPopupActor *arg0) {
    arg0->x -= arg0->velocity;
    arg0->velocity -= 4;
    if (arg0->velocity == 0) {
        arg0->timer = 0x2D;
        func_80071824(arg0, func_80057D0C);
    }
    func_800483FC(&D_80124878, func_80057C5C, arg0);
}

void func_80057DD4(RaceUiPopupActor *arg0) {
    arg0->y.word = 0xC;
    arg0->velocity = 0x38;
    arg0->x = 0x1A4;
    func_80071824(arg0, func_80057D68);
}

void func_80057E10(void *arg0) {
    if (D_80156612 & 8) {
        func_80045A78(0x20, 0x4A, func_80043040(D_8011216E), 0x5B);
    }
}

void func_80057E60(s32 arg0) {
    func_800483FC(&D_80124868, func_80057E10, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80057E90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005804C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005812C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005827C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80058360.s")

void func_800584A0(void *arg0) {
    func_800483FC(&D_80124868, func_80057E90, arg0);
    func_800483FC(&D_80124868, func_8005804C, arg0);
    func_800483FC(&D_80124868, func_8005812C, arg0);
    func_800483FC(&D_80124868, func_8005827C, arg0);
    func_800483FC(&D_80124858, func_80058360, arg0);
}

void func_80058538(void *arg0) {
    if (D_80123778 & 0x8000) {
        func_80072138(0x18, 0x32);
        D_801235B4 |= 0x10;
        func_80071824(arg0, func_800584A0);
    }
    func_800483FC(&D_80124868, func_80057E90, (s32)arg0);
    func_800483FC(&D_80124868, func_8005804C, (s32)arg0);
    func_800483FC(&D_80124868, func_8005812C, (s32)arg0);
    func_800483FC(&D_80124868, func_8005827C, (s32)arg0);
    func_800483FC(&D_80124858, func_80058360, (s32)arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80058610.s")

void func_80058880(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0;
        func_80071824(arg0, func_80058610);
    }
    func_800483FC(&D_80124868, func_80057E90, arg0);
    func_800483FC(&D_80124868, func_8005804C, arg0);
    func_800483FC(&D_80124868, func_8005812C, arg0);
    func_800483FC(&D_80124868, func_8005827C, arg0);
    func_800483FC(&D_80124858, func_80058360, arg0);
}

void func_8005893C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        func_80072138(0x1A, 0x32);
        func_80071824(arg0, func_80058880);
    }
    func_800483FC(&D_80124868, func_80057E90, arg0);
    func_800483FC(&D_80124868, func_8005804C, arg0);
    func_800483FC(&D_80124868, func_8005812C, arg0);
    func_800483FC(&D_80124858, func_80058360, arg0);
}

void func_800589F4(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        func_80072138(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        func_80071824(arg0, func_8005893C);
    }
    func_800483FC(&D_80124868, func_80057E90, arg0);
    func_800483FC(&D_80124868, func_8005804C, arg0);
    func_800483FC(&D_80124858, func_80058360, arg0);
}

void func_80058A98(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        func_80072138(0x1A, 0x32);
        func_80071824(arg0, func_800589F4);
    }
    func_800483FC(&D_80124868, func_80057E90, arg0);
    func_800483FC(&D_80124858, func_80058360, arg0);
}

void func_80058B20(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(s16 *)((u8 *)arg0 + 0x18) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x18) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        *(s16 *)((u8 *)arg0 + 0x18) = 0xFF;
        func_80071824(arg0, func_80058A98);
    }
    func_800483FC(&D_80124868, func_80057E90, arg0);
    func_800483FC(&D_80124858, func_80058360, arg0);
}

void func_80058BAC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = 0;
    *(s16 *)((u8 *)arg0 + 0x1C) = D_800D6050[D_80122289 + D_80121B50 * 4];
    func_80071824(arg0, func_80058B20);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80058C00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005905C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80059518.s")

void func_80059804(void *arg0) {
    func_80059518(arg0);
    func_800483FC(&D_80124868, func_80058C00, (s32)arg0);
    func_800483FC(&D_80124858, func_8005905C, (s32)arg0);
}

void func_80059854(void *arg0) {
    s32 *input;
    s32 i;
    void *actor;

    actor = arg0;
    i = 0;
    if ((s32)D_80121B55 > 0) {
        input = &D_80123778; do {
            if ((*input & 0x8000) && !(D_801235B4 & 0x10)) {
                D_801235B4 |= 0x10;
                func_80072138(0x18, 0x32);
                func_80071824(actor, func_80059804);
            }
            i++;
            input++;
        } while (i < (s32)D_80121B55);
    }
    func_80059518(actor);
    func_800483FC(&D_80124868, func_80058C00, (s32)actor);
    func_800483FC(&D_80124858, func_8005905C, (s32)actor);
}

void func_80059950(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(s16 *)((u8 *)arg0 + 0x18) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x18) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x96;
        *(s16 *)((u8 *)arg0 + 0x18) = 0xFF;
        func_80071824(arg0, func_80059854);
    }
    func_80059518(arg0);
    func_800483FC(&D_80124868, func_80058C00, arg0);
    func_800483FC(&D_80124858, func_8005905C, arg0);
}

void func_800599DC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = 0;
    func_80071824(arg0, func_80059950);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80059A04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80059C34.s")

void func_80059E5C(RaceUiAlpha1AActor *arg0) {
    Gfx *gfx;
    s32 w0;
    s32 w1;

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->alpha & 0xFF) | (~0xFF); } func_80059A04(&D_80121B74, -0x6C, -0x30, 0xC); if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) D_800DEFF8; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; } } while (0);
}

void func_80059F6C(RaceUiAlpha18Actor *arg0) {
    Gfx *gfx;
    s32 w0;
    s32 w1;

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->alpha & 0xFF) | (~0xFF); } func_80048278(-0x74, -0x3C, D_800E12D4, 5); if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) D_800DEFF8; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; } } while (0);
}

void func_8005A07C(void *arg0) {
    func_800483FC(&D_80124868, func_80059E5C, arg0);
    func_800483FC(&D_80124868, func_80059C34, arg0);
    func_800483FC(&D_80124858, func_80059F6C, arg0);
}

void func_8005A0E0(void *arg0) {
    if (D_80123778 & 0x8000) {
        D_801235B4 |= 0x10;
        func_80072138(0x18, 0x32);
        func_80071824(arg0, func_8005A07C);
    }
    func_800483FC(&D_80124868, func_80059E5C, arg0);
    func_800483FC(&D_80124868, func_80059C34, arg0);
    func_800483FC(&D_80124858, func_80059F6C, arg0);
}

void func_8005A184(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = *(s16 *)((u8 *)arg0 + 0x1C) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1C) == 0) {
        func_80071824(arg0, func_8005A0E0);
    }
    func_800483FC(&D_80124868, func_80059E5C, arg0);
    func_800483FC(&D_80124858, func_80059F6C, arg0);
}

void func_8005A1FC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x1A) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1C) = 0x1E;
        *(s16 *)((u8 *)arg0 + 0x1A) = 0xFF;
        func_80071824(arg0, func_8005A184);
    }
    func_800483FC(&D_80124868, func_80059E5C, arg0);
    func_800483FC(&D_80124858, func_80059F6C, arg0);
}

void func_8005A288(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(s16 *)((u8 *)arg0 + 0x18) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x18) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x18) = 0xFF;
        func_80071824(arg0, func_8005A1FC);
    }
    func_800483FC(&D_80124858, func_80059F6C, arg0);
}

void func_8005A2F0(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = 0;
    *(s16 *)((u8 *)arg0 + 0x1A) = 0;
    func_80071824(arg0, func_8005A288);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005A31C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005A4BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005A884.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005AAE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005AC44.s")

void func_8005AE1C(void *arg0) {
    func_80048278(0x20, -0x48, &D_800E1368, 6);
    func_80048278(0x20, -0x28, &D_800E136C, 6);
    func_80048278(0x20, -8, &D_800E1370, 6);
    func_80048278(0x20, 0x18, &D_800E1374, 4);
    func_80048278(0x20, 0x38, &D_800E1378, 4);
}

void func_8005AEB0(void *arg0) {
    func_800483FC(&D_80124868, func_8005A31C, arg0);
    func_800483FC(&D_80124868, func_8005A884, arg0);
    func_800483FC(&D_80124858, func_8005AAE4, arg0);
    func_800483FC(&D_80124858, func_8005AE1C, arg0);
}

void func_8005AF2C(void *arg0) {
    if (D_80123778 & 0x8000) {
        func_80072138(0x18, 0x32);
        D_801235B4 |= 0x10;
        func_80071824(arg0, func_8005AEB0);
    }
    func_800483FC(&D_80124868, func_8005A31C, arg0);
    func_800483FC(&D_80124868, func_8005A884, arg0);
    func_800483FC(&D_80124858, func_8005AAE4, arg0);
    func_800483FC(&D_80124858, func_8005AE1C, arg0);
}

void func_8005AFEC(void *arg0) {
    func_800483FC(&D_80124868, func_8005A31C, arg0);
    func_800483FC(&D_80124868, func_8005A4BC, arg0);
    func_800483FC(&D_80124858, func_8005AAE4, arg0);
    func_800483FC(&D_80124858, func_8005AC44, arg0);
}

void func_8005B068(void *arg0) {
    if (D_80123778 & 0x8000) {
        func_80072138(0x18, 0x32);
        if (*(u16 *)((u8 *)arg0 + 0x10) < 5) {
            func_80071824(arg0, func_8005AF2C);
        } else {
            D_801235B4 |= 0x10;
            func_80071824(arg0, func_8005AFEC);
        }
    }
    func_800483FC(&D_80124868, func_8005A31C, (s32)arg0);
    func_800483FC(&D_80124868, func_8005A4BC, (s32)arg0);
    func_800483FC(&D_80124858, func_8005AAE4, (s32)arg0);
    func_800483FC(&D_80124858, func_8005AC44, (s32)arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005B14C.s")

void func_8005B344(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = 3;
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        func_80071824(arg0, func_8005B14C);
    }
    func_800483FC(&D_80124868, func_8005A31C, arg0);
    func_800483FC(&D_80124868, func_8005A4BC, arg0);
    func_800483FC(&D_80124858, func_8005AAE4, arg0);
    func_800483FC(&D_80124858, func_8005AC44, arg0);
}

void func_8005B3EC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = 3;
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0xA;
        func_80071824(arg0, func_8005B344);
    }
    func_800483FC(&D_80124868, func_8005A31C, arg0);
    func_800483FC(&D_80124868, func_8005A4BC, arg0);
    func_800483FC(&D_80124858, func_8005AAE4, arg0);
    func_800483FC(&D_80124858, func_8005AC44, arg0);
}

void func_8005B49C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = 2;
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        func_80072138(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        func_80071824(arg0, func_8005B3EC);
    }
    func_800483FC(&D_80124868, func_8005A31C, arg0);
    func_800483FC(&D_80124868, func_8005A4BC, arg0);
    func_800483FC(&D_80124858, func_8005AAE4, arg0);
    func_800483FC(&D_80124858, func_8005AC44, arg0);
}

void func_8005B55C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = 1;
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        func_80072138(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        func_80071824(arg0, func_8005B49C);
    }
    func_800483FC(&D_80124868, func_8005A31C, arg0);
    func_800483FC(&D_80124868, func_8005A4BC, arg0);
    func_800483FC(&D_80124858, func_8005AAE4, arg0);
    func_800483FC(&D_80124858, func_8005AC44, arg0);
}

void func_8005B61C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = 0;
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        func_80072138(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        if (*(s16 *)((u8 *)arg0 + 0x24) != 0) {
            func_80071824(arg0, func_8005B55C);
        } else {
            func_80071824(arg0, func_8005B49C);
        }
    }
    func_800483FC(&D_80124868, func_8005A31C, arg0);
    func_800483FC(&D_80124868, func_8005A4BC, arg0);
    func_800483FC(&D_80124858, func_8005AAE4, arg0);
    func_800483FC(&D_80124858, func_8005AC44, arg0);
}

void func_8005B6F8(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        func_80072138(0x1A, 0x32);
        func_80071824(arg0, func_8005B61C);
    }
    func_800483FC(&D_80124868, func_8005A31C, arg0);
    func_800483FC(&D_80124858, func_8005AAE4, arg0);
    func_800483FC(&D_80124858, func_8005AC44, arg0);
}

void func_8005B798(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(s16 *)((u8 *)arg0 + 0x18) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x18) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        *(s16 *)((u8 *)arg0 + 0x18) = 0xFF;
        func_80071824(arg0, func_8005B6F8);
    }
    func_800483FC(&D_80124868, func_8005A31C, arg0);
    func_800483FC(&D_80124858, func_8005AAE4, arg0);
    func_800483FC(&D_80124858, func_8005AC44, arg0);
}

void func_8005B834(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = 0;
    *(s16 *)((u8 *)arg0 + 0x24) = 0;
    *(s16 *)((u8 *)arg0 + 0x20) = 0;
    *(s16 *)((u8 *)arg0 + 0x22) = 0x12C;
    if (D_80121B50 == 9) {
        *(s16 *)((u8 *)arg0 + 0x22) = 0x64;
    }
    if (D_80121B81 != 0) {
        *(s16 *)((u8 *)arg0 + 0x1E) = D_80121D80[0].score;
    } else {
        *(s16 *)((u8 *)arg0 + 0x1E) = D_80121D80[0].score * 0x32;
        if (D_80121D80[0].score == D_80121D80[0].targetScore) {
            *(s16 *)((u8 *)arg0 + 0x24) = 1;
            *(s16 *)((u8 *)arg0 + 0x20) = 0x3E8;
        }
    }
    func_80071824(arg0, func_8005B798);
}

void func_8005B8E8(RaceUiAlphaActor *arg0) {
    Gfx *gfx;
    s32 w0;
    s32 w1;

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->alpha & 0xFF) | (~0xFF); } func_80059A04(&D_80121B74, -0x68, -0x37, 0xC); if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) D_800DEFF8; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; } } while (0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005B9F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005BE68.s")

void func_8005C03C(RaceUiAlphaActor *arg0) {
    Gfx *gfx;
    s32 w0;
    s32 w1;

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = (arg0->alpha & 0xFF) | (~0xFF); } func_80048278(-0x70, -0x40, D_800E1390, 5); if (arg0->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) D_800DEFF8; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; } } while (0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005C14C.s")

void func_8005C3E4(void *arg0) {
    func_800483FC(&D_80124868, func_8005B8E8, arg0);
    func_800483FC(&D_80124868, func_8005BE68, arg0);
    func_800483FC(&D_80124858, func_8005C03C, arg0);
}

void func_8005C448(void *arg0) {
    if (D_80123778 & 0x8000) {
        func_80072138(0x18, 0x32);
        D_801235B4 |= 0x10;
        func_80071824(arg0, func_8005C3E4);
    }
    func_800483FC(&D_80124868, func_8005B8E8, arg0);
    func_800483FC(&D_80124868, func_8005BE68, arg0);
    func_800483FC(&D_80124858, func_8005C03C, arg0);
}

void func_8005C4EC(void *arg0) {
    func_800483FC(&D_80124868, func_8005B8E8, arg0);
    func_800483FC(&D_80124868, func_8005B9F8, arg0);
    func_800483FC(&D_80124858, func_8005C03C, arg0);
    func_800483FC(&D_80124858, func_8005C14C, arg0);
}

void func_8005C568(void *arg0) {
    if (D_80123778 & 0x8000) {
        func_80072138(0x18, 0x32);
        if (*(u16 *)((u8 *)arg0 + 0x10) < 5) {
            func_80071824(arg0, func_8005C448);
        } else {
            D_801235B4 |= 0x10;
            func_80071824(arg0, func_8005C4EC);
        }
    }
    func_800483FC(&D_80124868, func_8005B8E8, (s32)arg0);
    func_800483FC(&D_80124868, func_8005B9F8, (s32)arg0);
    func_800483FC(&D_80124858, func_8005C03C, (s32)arg0);
    func_800483FC(&D_80124858, func_8005C14C, (s32)arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005C64C.s")

void func_8005C89C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x20) = 4;
    *(s16 *)((u8 *)arg0 + 0x1E) = *(s16 *)((u8 *)arg0 + 0x1E) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1E) == 0) {
        func_80071824(arg0, func_8005C64C);
    }
    func_800483FC(&D_80124868, func_8005B8E8, arg0);
    func_800483FC(&D_80124868, func_8005B9F8, arg0);
    func_800483FC(&D_80124858, func_8005C03C, arg0);
    func_800483FC(&D_80124858, func_8005C14C, arg0);
}

void func_8005C944(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x20) = 4;
    *(s16 *)((u8 *)arg0 + 0x1E) = *(s16 *)((u8 *)arg0 + 0x1E) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1E) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1E) = 0xA;
        func_80071824(arg0, func_8005C89C);
    }
    func_800483FC(&D_80124868, func_8005B8E8, arg0);
    func_800483FC(&D_80124868, func_8005B9F8, arg0);
    func_800483FC(&D_80124858, func_8005C03C, arg0);
    func_800483FC(&D_80124858, func_8005C14C, arg0);
}

void func_8005C9F4(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x20) = 3;
    *(s16 *)((u8 *)arg0 + 0x1E) = *(s16 *)((u8 *)arg0 + 0x1E) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1E) == 0) {
        func_80072138(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1E) = 0x14;
        func_80071824(arg0, func_8005C944);
    }
    func_800483FC(&D_80124868, func_8005B8E8, arg0);
    func_800483FC(&D_80124868, func_8005B9F8, arg0);
    func_800483FC(&D_80124858, func_8005C03C, arg0);
    func_800483FC(&D_80124858, func_8005C14C, arg0);
}

void func_8005CAB4(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x20) = 2;
    *(s16 *)((u8 *)arg0 + 0x1E) = *(s16 *)((u8 *)arg0 + 0x1E) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1E) == 0) {
        func_80072138(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1E) = 0x14;
        func_80071824(arg0, func_8005C9F4);
    }
    func_800483FC(&D_80124868, func_8005B8E8, arg0);
    func_800483FC(&D_80124868, func_8005B9F8, arg0);
    func_800483FC(&D_80124858, func_8005C03C, arg0);
    func_800483FC(&D_80124858, func_8005C14C, arg0);
}

void func_8005CB74(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x20) = 1;
    *(s16 *)((u8 *)arg0 + 0x1E) = *(s16 *)((u8 *)arg0 + 0x1E) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1E) == 0) {
        func_80072138(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1E) = 0x14;
        if (*(s16 *)((u8 *)arg0 + 0x2A) != 0) {
            func_80071824(arg0, func_8005CAB4);
        } else {
            func_80071824(arg0, func_8005C9F4);
        }
    }
    func_800483FC(&D_80124868, func_8005B8E8, arg0);
    func_800483FC(&D_80124868, func_8005B9F8, arg0);
    func_800483FC(&D_80124858, func_8005C03C, arg0);
    func_800483FC(&D_80124858, func_8005C14C, arg0);
}

void func_8005CC54(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x20) = 0;
    *(s16 *)((u8 *)arg0 + 0x1E) = *(s16 *)((u8 *)arg0 + 0x1E) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1E) == 0) {
        func_80072138(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1E) = 0x14;
        func_80071824(arg0, func_8005CB74);
    }
    func_800483FC(&D_80124868, func_8005B8E8, arg0);
    func_800483FC(&D_80124868, func_8005B9F8, arg0);
    func_800483FC(&D_80124858, func_8005C03C, arg0);
    func_800483FC(&D_80124858, func_8005C14C, arg0);
}

void func_8005CD10(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1E) = *(s16 *)((u8 *)arg0 + 0x1E) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1E) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1E) = 0x14;
        func_80072138(0x1A, 0x32);
        func_80071824(arg0, func_8005CC54);
    }
    func_800483FC(&D_80124868, func_8005B8E8, arg0);
    func_800483FC(&D_80124858, func_8005C03C, arg0);
    func_800483FC(&D_80124858, func_8005C14C, arg0);
}

void func_8005CDB0(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = *(s16 *)((u8 *)arg0 + 0x1C) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x1C) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1E) = 0x14;
        *(s16 *)((u8 *)arg0 + 0x1C) = 0xFF;
        func_80071824(arg0, func_8005CD10);
    }
    func_800483FC(&D_80124868, func_8005B8E8, arg0);
    func_800483FC(&D_80124858, func_8005C03C, arg0);
    func_800483FC(&D_80124858, func_8005C14C, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005CE4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005CF60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005D1CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005D558.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005D860.s")

// func_8005D9B4 best match: 88.102% (nonmatchings/func_8005D9B4-1197934324348345530/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005D9B4.s")

#ifdef NON_MATCHING
void func_8005D9B4(RaceUiAlpha18Actor *arg0) {
    Gfx *gfx;
    s32 w0;
    s32 w1;
    s16 y;
    RaceUiAlpha18Actor **arg0p = &arg0;

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { if ((*arg0p)->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w1 = 0; gfx->words.w0 = 0xE7000000; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xFA000000; gfx->words.w1 = ((*arg0p)->alpha & 0xFF) | (~0xFF); } func_80048278(-0xC, -0x50, &D_800E1474, 5); y = -0x28; func_80048278(-8, y, &D_800E1484, 5); y += 0x28; func_80048278(-8, y, &D_800E1494, 5); y += 0x28; func_80048278(-0xC, y, &D_800E14A8, 5); if ((*arg0p)->alpha != 0xFF) { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) D_800DEFF8; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; } } while (0);
}
#endif

void func_8005DB3C(void *arg0) {
    func_80048278(0x30, -0x48, &D_800E14B8, 6);
    func_80048278(0x30, -0x28, &D_800E14BC, 6);
    func_80048278(0x30, -8, &D_800E14C0, 6);
    func_80048278(0x30, 0x18, &D_800E14C4, 4);
    func_80048278(0x30, 0x38, &D_800E14C8, 4);
}

void func_8005DBD0(void *arg0) {
    func_800483FC(&D_80124868, func_8005CF60, arg0);
    func_800483FC(&D_80124868, func_8005D558, arg0);
    func_800483FC(&D_80124858, func_8005D860, arg0);
    func_800483FC(&D_80124858, func_8005DB3C, arg0);
}

void func_8005DC4C(void *arg0) {
    if (D_80123778 & 0x8000) {
        func_80072138(0x18, 0x32);
        D_801235B4 |= 0x10;
        func_80071824(arg0, func_8005DBD0);
    }
    func_800483FC(&D_80124868, func_8005CF60, arg0);
    func_800483FC(&D_80124868, func_8005D558, arg0);
    func_800483FC(&D_80124858, func_8005D860, arg0);
    func_800483FC(&D_80124858, func_8005DB3C, arg0);
}

void func_8005DD0C(void *arg0) {
    func_800483FC(&D_80124868, func_8005CF60, arg0);
    func_800483FC(&D_80124868, func_8005D1CC, arg0);
    func_800483FC(&D_80124858, func_8005D860, arg0);
    func_800483FC(&D_80124858, func_8005D9B4, arg0);
}

void func_8005DD88(void *arg0) {
    if (D_80123778 & 0x8000) {
        func_80072138(0x18, 0x32);
        if (*(u16 *)((u8 *)arg0 + 0x10) < 5) {
            func_80071824(arg0, func_8005DC4C);
        } else {
            D_801235B4 |= 0x10;
            func_80071824(arg0, func_8005DD0C);
        }
    }
    func_800483FC(&D_80124868, func_8005CF60, (s32)arg0);
    func_800483FC(&D_80124868, func_8005D1CC, (s32)arg0);
    func_800483FC(&D_80124858, func_8005D860, (s32)arg0);
    func_800483FC(&D_80124858, func_8005D9B4, (s32)arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005DE6C.s")

void func_8005E064(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = 3;
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        func_80071824(arg0, func_8005DE6C);
    }
    func_800483FC(&D_80124868, func_8005CF60, arg0);
    func_800483FC(&D_80124868, func_8005D1CC, arg0);
    func_800483FC(&D_80124858, func_8005D860, arg0);
    func_800483FC(&D_80124858, func_8005D9B4, arg0);
}

void func_8005E10C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = 3;
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0xA;
        func_80071824(arg0, func_8005E064);
    }
    func_800483FC(&D_80124868, func_8005CF60, arg0);
    func_800483FC(&D_80124868, func_8005D1CC, arg0);
    func_800483FC(&D_80124858, func_8005D860, arg0);
    func_800483FC(&D_80124858, func_8005D9B4, arg0);
}

void func_8005E1BC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = 2;
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        func_80072138(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        func_80071824(arg0, func_8005E10C);
    }
    func_800483FC(&D_80124868, func_8005CF60, arg0);
    func_800483FC(&D_80124868, func_8005D1CC, arg0);
    func_800483FC(&D_80124858, func_8005D860, arg0);
    func_800483FC(&D_80124858, func_8005D9B4, arg0);
}

void func_8005E27C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1C) = 1;
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        func_80072138(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        func_80071824(arg0, func_8005E1BC);
    }
    func_800483FC(&D_80124868, func_8005CF60, arg0);
    func_800483FC(&D_80124868, func_8005D1CC, arg0);
    func_800483FC(&D_80124858, func_8005D860, arg0);
    func_800483FC(&D_80124858, func_8005D9B4, arg0);
}

void func_8005E33C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    *(s16 *)((u8 *)arg0 + 0x1C) = 0;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        func_80072138(0x1A, 0x32);
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        func_80071824(arg0, func_8005E27C);
    }
    func_800483FC(&D_80124868, func_8005CF60, arg0);
    func_800483FC(&D_80124868, func_8005D1CC, arg0);
    func_800483FC(&D_80124858, func_8005D860, arg0);
    func_800483FC(&D_80124858, func_8005D9B4, arg0);
}

void func_8005E3F8(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x1A) = *(s16 *)((u8 *)arg0 + 0x1A) - 1;
    if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        func_80072138(0x1A, 0x32);
        func_80071824(arg0, func_8005E33C);
    }
    func_800483FC(&D_80124868, func_8005CF60, arg0);
    func_800483FC(&D_80124858, func_8005D860, arg0);
    func_800483FC(&D_80124858, func_8005D9B4, arg0);
}

void func_8005E498(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(s16 *)((u8 *)arg0 + 0x18) + 0x10;
    if (*(s16 *)((u8 *)arg0 + 0x18) >= 0x100) {
        *(s16 *)((u8 *)arg0 + 0x1A) = 0x14;
        *(s16 *)((u8 *)arg0 + 0x18) = 0xFF;
        func_80071824(arg0, func_8005E3F8);
    }
    func_800483FC(&D_80124868, func_8005CF60, arg0);
    func_800483FC(&D_80124858, func_8005D860, arg0);
    func_800483FC(&D_80124858, func_8005D9B4, arg0);
}

void func_8005E534(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = 0;
    *(s16 *)((u8 *)arg0 + 0x20) = 0;
    *(s16 *)((u8 *)arg0 + 0x22) = 0x12C;
    if (D_80121B81 != 0) {
        *(s16 *)((u8 *)arg0 + 0x1E) = 0;
        *(volatile s16 *)((u8 *)arg0 + 0x20) = 0;
    } else {
        *(s16 *)((u8 *)arg0 + 0x1E) = D_80121D80[0].unk2C0 * 3;
        *(s16 *)((u8 *)arg0 + 0x20) = D_80121D80[0].unk2C3 * 10;
    }
    func_80071824(arg0, func_8005E498);
}

void func_8005E5B4(void *arg0) {
    func_80045A78(-0x68, -0x2C, func_80043040(D_8011216E), 0x4D);
    func_80045A78(-0x42, -0xC, func_80043040(D_8011216E), D_80122289 & 0xFFFF);
    func_80045A78(-0x22, 4, func_80043040(D_8011216E), (D_80122289 + 0x48) & 0xFFFF);
    func_80045A78(-0x68, 6, func_80043040(D_8011216E), (D_80121D90 + 0x91) & 0xFFFF);
}

void func_8005E68C(void *arg0) {
    if (*(u16 *)((u8 *)arg0 + 0x10) != 0) {
        *(s16 *)((u8 *)arg0 + 0x10) = *(u16 *)((u8 *)arg0 + 0x10) - 1;
    } else {
        func_800483FC(&D_80124868, func_8005E5B4, (s32)arg0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005E6D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005EA4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005ECA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005EFFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005F174.s")

void func_8005F298(s16 arg0) {
    void *temp = func_800711D0(func_8005F174, 0, 0x62);
    if (temp != NULL) {
        *(s8 *)((u8 *)temp + 0x59) = arg0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005F2DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005F448.s")

void func_8005F56C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x90) = 1;
    *(s32 *)((u8 *)arg0 + 0x24) = 0xF0000;
    *(s32 *)((u8 *)arg0 + 0x28) = 0x230000;
    *(s32 *)((u8 *)arg0 + 0x2C) = 0;
    *(s16 *)((u8 *)arg0 + 0x92) = 0xF;
    *(s32 *)((u8 *)arg0 + 0x3C) = 0x20000;
    func_8005F448(arg0);
    func_80071824(arg0, func_8005F448);
}

void func_8005F5C8(RaceUiSnowboardTrailPlayer *player) {
    RaceUiSnowboardTrailActor *actor = func_800711D0(func_8005F56C, 0, 0x62);

    if (actor != NULL) {
        actor->playerIndex = player->playerIndex;
        actor->worldPos = player->trail.worldPos;
        actor->copyBlock = player->trail.copyBlock;
        actor->spinYaw = player->trail.spinYaw;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005F6A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005F828.s")

void func_8005FB30(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x48) = 2;
    func_80072A74(0xF, &D_80121D80[*(u16 *)((u8 *)arg0 + 0x10)].pos1C, 0x7F, 0x32);
    func_80071824(arg0, func_8005F828);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005FBA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8005FED0.s")

void func_800601F8(void *arg0) {
    s32 temp;

    if (D_80121B56 == 0) {
        temp = *(s32 *)((u8 *)arg0 + 0x4C);
        *(s32 *)((u8 *)arg0 + 0x18) += *(s32 *)((u8 *)arg0 + 0x48);
        *(s32 *)((u8 *)arg0 + 0x1C) += temp;
        *(s32 *)((u8 *)arg0 + 0x20) += *(s32 *)((u8 *)arg0 + 0x50);
        *(s32 *)((u8 *)arg0 + 0x4C) = temp - 0x8000;
        *(s16 *)((u8 *)arg0 + 0x54) = *(s16 *)((u8 *)arg0 + 0x54) + 1;
        if (*(s16 *)((u8 *)arg0 + 0x54) >= 0x10) {
            func_800716E4(arg0);
            return;
        }
    }
    if (*(u8 *)((u8 *)arg0 + 0x57) == 0) {
        func_800483FC(&D_801248EC, func_8005FBA8, arg0);
        return;
    }
    func_800483FC(&D_801248B0, func_8005FED0, arg0);
}

void func_800602BC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x54) = 0;
    *(Vec3i *)((u8 *)arg0 + 0x48) = D_800D61C0[*(u16 *)((u8 *)arg0 + 0x10)];
    if (*(u8 *)((u8 *)arg0 + 0x57) == 0) {
        func_80045990(func_80043040(D_80112130.mainFontHandle), 0x27, (u8 *)arg0 + 0x38, (u8 *)arg0 + 0x28);
        func_80045990(func_80043040(D_80112130.mainFontHandle), 0x28, (u8 *)arg0 + 0x3C, (u8 *)arg0 + 0x2C);
        func_80045990(func_80043040(D_80112130.mainFontHandle), 0x29, (u8 *)arg0 + 0x40, (u8 *)arg0 + 0x30);
        func_80045990(func_80043040(D_80112130.mainFontHandle), 0x2A, (u8 *)arg0 + 0x44, (u8 *)arg0 + 0x34);
    } else {
        func_80045990(func_80043040(D_80112130.mainFontHandle), 0x2B, (u8 *)arg0 + 0x38, (u8 *)arg0 + 0x28);
        func_80045990(func_80043040(D_80112130.mainFontHandle), 0x2C, (u8 *)arg0 + 0x3C, (u8 *)arg0 + 0x2C);
        func_80045990(func_80043040(D_80112130.mainFontHandle), 0x2D, (u8 *)arg0 + 0x40, (u8 *)arg0 + 0x30);
        func_80045990(func_80043040(D_80112130.mainFontHandle), 0x2E, (u8 *)arg0 + 0x44, (u8 *)arg0 + 0x34);
    }
    func_800601F8(arg0);
    func_80071824(arg0, func_800601F8);
}

void func_80060454(void *arg0, void *arg1, void *arg2, s16 arg3) {
    void *temp = func_800711D0(func_800602BC, 5, 0x1E);
    if (temp != NULL) {
        *(s8 *)((u8 *)temp + 0x57) = 0;
        *(s16 *)((u8 *)temp + 0x10) = arg3;
        *(void **)((u8 *)temp + 0x18) = arg0;
        *(void **)((u8 *)temp + 0x1C) = arg1;
        *(void **)((u8 *)temp + 0x20) = arg2;
    }
}

void func_800604CC(void *arg0, void *arg1, void *arg2, s16 arg3) {
    void *temp = func_800711D0(func_800602BC, 5, 0x1E);
    if (temp != NULL) {
        *(s8 *)((u8 *)temp + 0x57) = 1;
        *(s16 *)((u8 *)temp + 0x10) = arg3;
        *(void **)((u8 *)temp + 0x18) = arg0;
        *(void **)((u8 *)temp + 0x1C) = arg1;
        *(void **)((u8 *)temp + 0x20) = arg2;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80060544.s")

void func_8006069C(void *arg0) {
    if (D_80121B56 == 0) {
        *(s32 *)((u8 *)arg0 + 0x4C) += -0x10000;
        *(s32 *)((u8 *)arg0 + 0x18) += 0x20000;
        *(s32 *)((u8 *)arg0 + 0x1C) += *(s32 *)((u8 *)arg0 + 0x4C);
        *(s32 *)((u8 *)arg0 + 0x20) += 0x10000;
        *(s16 *)((u8 *)arg0 + 0x56) = *(s16 *)((u8 *)arg0 + 0x56) - 1;
    }
    if (*(s16 *)((u8 *)arg0 + 0x56) == 0) {
        func_800716E4(arg0);
    } else {
        func_800483FC(&D_801248B0, func_80060544, arg0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80060738.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80060914.s")

void func_80060BC4(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x54) = 1;
    *(s32 *)((u8 *)arg0 + 0x48) = 0;
    *(s32 *)((u8 *)arg0 + 0x4C) = 0;
    *(u8 *)((u8 *)arg0 + 0x59) = 1;
    func_80072A74(0xD, &D_80121D80[*(s16 *)((u8 *)arg0 + 0x50)].pos1C, 0x7F, 0x32);
    func_80071824(arg0, func_80060914);
}

void func_80060C4C(s16 arg0) {
    s32 var_s1;
    s32 var_s0;
    void *temp_v0;

    var_s1 = 0;
    var_s0 = 0;
    if (D_80121B54 > 0) {
        do {
            if (arg0 != var_s0) {
                temp_v0 = func_800711D0(func_80060BC4, 0, 0x1E);
                if (temp_v0 != NULL) {
                    *(s16 *)((u8 *)temp_v0 + 0x50) = arg0;
                    *(s16 *)((u8 *)temp_v0 + 0x52) = var_s0;
                    *(s16 *)((u8 *)temp_v0 + 0x56) = var_s1 * 3;
                    var_s1 += 1;
                }
            }
            var_s0 += 1;
        } while (var_s0 < D_80121B54);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80060D10.s")

void func_80060E7C(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x1C) += *(s32 *)((u8 *)arg0 + 0x28);
    *(s32 *)((u8 *)arg0 + 0x28) += 4;
    if (*(s32 *)((u8 *)arg0 + 0x28) == 0x38) {
        func_800716E4(arg0);
    } else {
        func_800483FC(&D_80124878, func_80060D10, arg0);
    }
}

void func_80060EDC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(u16 *)((u8 *)arg0 + 0x18) - 1;
    if (*(u16 *)((u8 *)arg0 + 0x18) == 0) {
        func_80071824(arg0, func_80060E7C);
    }
    func_800483FC(&D_80124878, func_80060D10, arg0);
}

void func_80060F38(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x1C) -= *(s32 *)((u8 *)arg0 + 0x28);
    *(s32 *)((u8 *)arg0 + 0x28) -= 4;
    if (*(s32 *)((u8 *)arg0 + 0x28) == 0) {
        *(s16 *)((u8 *)arg0 + 0x18) = 0x2D;
        func_80071824(arg0, func_80060EDC);
    }
    func_800483FC(&D_80124878, func_80060D10, arg0);
}

void func_80060FA4(void *arg0) {
    if (D_801124B0[*(s8 *)((u8 *)arg0 + 0x30)].active != 0) {
        if (D_80121B55 == 1) {
            *(s32 *)((u8 *)arg0 + 0x20) = -0x24;
        } else {
            *(s32 *)((u8 *)arg0 + 0x20) = -0x16;
        }
        *(s32 *)((u8 *)arg0 + 0x28) = 0x38;
        *(s32 *)((u8 *)arg0 + 0x1C) = 0x1A4;
        func_80071824(arg0, func_80060F38);
    } else {
        func_800716E4(arg0);
    }
}

void func_80061034(void *arg0, s16 arg1) {
    void *temp = func_80071408(func_80060FA4, 0, 0x64);
    if (temp != NULL) {
        *(void **)((u8 *)temp + 0x2C) = arg0;
        *(s8 *)((u8 *)temp + 0x30) = arg1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80061088.s")

void func_800613EC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x30) = *(s16 *)((u8 *)arg0 + 0x30) + 4;
    func_800483FC(&D_801248B0, func_80061088, (s32)arg0);
}

void func_80061428(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x30) = 0;
    *(Vec3i *)((u8 *)arg0 + 0x18) = D_800D6220[*(u16 *)((u8 *)arg0 + 0x10)];
    func_80071824(arg0, func_800613EC);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80061484.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_800615BC.s")

void func_800617C8(void *arg0) {
    func_80071824(arg0, func_800615BC);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_800617EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80061984.s")

void func_80061A98(void *arg0) {
    s32 temp;
    *(s16 *)((u8 *)arg0 + 0x34) = 0xF;
    temp = func_80080CC4(*(s16 *)((u8 *)arg0 + 0x24), *(s32 *)((u8 *)arg0 + 0x18), *(s32 *)((u8 *)arg0 + 0x20));
    *(s16 *)((u8 *)arg0 + 0x2A) = 8;
    *(s32 *)((u8 *)arg0 + 0x1C) = temp;
    *(s16 *)((u8 *)arg0 + 0x28) = *(s16 *)((u8 *)arg0 + 0x2A);
    func_80071824(arg0, func_80061984);
}

void func_80061AF4(s16 arg0, void *arg1, void *arg2, s16 arg3) {
    void *temp = func_80071408(func_80061A98, 0, 0x64);
    if (temp != NULL) {
        *(void **)((u8 *)temp + 0x18) = arg1;
        *(void **)((u8 *)temp + 0x20) = arg2;
        *(s16 *)((u8 *)temp + 0x26) = (arg3 + 0x800) & 0xFFF;
        *(s16 *)((u8 *)temp + 0x24) = arg0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80061B70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80061CA8.s")

void func_80061D90(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x68) = 0x3C;
    func_80097C18((u8 *)arg0 + 0x44, 0x800);
    *(s32 *)((u8 *)arg0 + 0x58) = 0;
    *(s32 *)((u8 *)arg0 + 0x5C) = 0x100000;
    *(s32 *)((u8 *)arg0 + 0x60) = 0xFFE00000;
    func_80071824(arg0, func_80061CA8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80061DE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80061F38.s")

void func_800621DC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x4C) = 4;
    *(s16 *)((u8 *)arg0 + 0x4A) = 0xFF;
    *(s16 *)((u8 *)arg0 + 0x48) = 0;
    *(s32 *)((u8 *)arg0 + 0x2C) = *(s32 *)((u8 *)arg0 + 0x38);
    *(s32 *)((u8 *)arg0 + 0x30) = *(s32 *)((u8 *)arg0 + 0x3C);
    *(s32 *)((u8 *)arg0 + 0x34) = *(s32 *)((u8 *)arg0 + 0x40);
    func_80072A74(0x13, (u8 *)arg0 + 0x38, 0x7F, 0x32);
    func_80071824(arg0, func_80061F38);
}

void func_8006224C(void *arg0, void *arg1, void *arg2) {
    void *temp = func_800716A4(func_800621DC, 0, 3);
    if (temp != NULL) {
        *(void **)((u8 *)temp + 0x38) = arg0;
        *(void **)((u8 *)temp + 0x3C) = arg1;
        *(void **)((u8 *)temp + 0x40) = arg2;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_800622B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_800623E8.s")

void func_80062530(RaceUiTransitionActor *arg0) {
    s16 *transform = arg0->transform;
    volatile u8 padding[0x20];

    arg0->unk68 = 0;
    func_80097BAC(transform, 0x400);
    func_80048D60(transform);
    arg0->unk6C = 1;
    arg0->unk6E = 0xF;
    func_80072A74(0xB, &D_80121D80[arg0->index].pos1C, 0x7F, 0x32);
    func_800623E8(arg0);
    func_80071824(arg0, func_800623E8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_800625D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_800628DC.s")

void func_80062A10(void *arg0) {
    func_80045990(func_80043040(D_80112168), 0x35, (u8 *)arg0 + 0x30, (u8 *)arg0 + 0x2C);
    func_80071824(arg0, func_800628DC);
}

void func_80062A64(s16 arg0) {
    void *temp;
    temp = func_800711D0(func_80062A10, 0, 4);
    if (temp != NULL) {
        *(s8 *)((u8 *)temp + 0x35) = arg0;
        *(s16 *)((u8 *)temp + 0x28) = 0;
    }
    temp = func_800711D0(func_80062A10, 0, 4);
    if (temp != NULL) {
        *(s8 *)((u8 *)temp + 0x35) = arg0;
        *(s16 *)((u8 *)temp + 0x28) = 0x555;
    }
    temp = func_800711D0(func_80062A10, 0, 4);
    if (temp != NULL) {
        *(s8 *)((u8 *)temp + 0x35) = arg0;
        *(s16 *)((u8 *)temp + 0x28) = 0xAAA;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80062AF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80062D34.s")

void func_80062ED4(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x2C) = func_80043120() << 4;
    if (func_80043120() & 1) {
        *(s16 *)((u8 *)arg0 + 0x2E) = 0x40;
    } else {
        *(s16 *)((u8 *)arg0 + 0x2E) = -0x40;
    }
    *(s16 *)((u8 *)arg0 + 0x24) = 0x1000;
    *(Vec3i *)((u8 *)arg0 + 0x18) = D_800D62AC[*(u16 *)((u8 *)arg0 + 0x10)];
    func_80071824(arg0, func_80062D34);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80062F6C.s")

void func_80063164(void *arg0) {
    if (D_80121B56 == 0) {
        *(s16 *)((u8 *)arg0 + 0x2E) = *(s16 *)((u8 *)arg0 + 0x2E) + 8;
    }
    func_800483FC(&D_801248BC, func_80062F6C, (s32)arg0);
}

void func_800631B0(void *arg0) {
    *(Vec3i *)((u8 *)arg0 + 0x18) = D_800D6330[*(u16 *)((u8 *)arg0 + 0x10)];
    *(s16 *)((u8 *)arg0 + 0x2C) = D_800D633C[*(u16 *)((u8 *)arg0 + 0x10)];
    func_80071824(arg0, func_80063164);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80063220.s")

void func_80063410(void *arg0) {
    if (D_80121B56 == 0) {
        *(s16 *)((u8 *)arg0 + 0x2E) = *(s16 *)((u8 *)arg0 + 0x2E) + 0x60;
        *(s16 *)((u8 *)arg0 + 0x30) = *(s16 *)((u8 *)arg0 + 0x30) + 0x10;
        *(s16 *)((u8 *)arg0 + 0x32) = *(s16 *)((u8 *)arg0 + 0x32) + 4;
    }
    func_800483FC(&D_801248BC, func_80063220, arg0);
}

void func_80063470(void *arg0) {
    *(Vec3i *)((u8 *)arg0 + 0x18) = D_800D6340[*(u16 *)((u8 *)arg0 + 0x10)];
    func_80071824(arg0, func_80063410);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_800634C8.s")

void func_8006392C(void *arg0) {
    if ((D_801235B0 & 7) == 0) {
        *(s16 *)((u8 *)arg0 + 0x4C) = (*(s16 *)((u8 *)arg0 + 0x4C) + 4) & 0x3F;
    }
    func_800483FC(&D_801248C8, func_800634C8, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80063980.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80063A9C.s")

void func_80063E70(RaceUiEffectParticleActor *arg0) {
    register RaceUiEffectParticleActor *actor;
    register s32 i;

    actor = arg0;
    for (i = 0; i < actor->count; i++) {
        actor->particles[i].unk0 += 0xA0000;
        if (actor->particles[i].unk0 >= 0x1000000) {
            actor->particles[i].unk8 = func_800430D0() << 0x10;
            actor->particles[i].unk4 = func_800430D0() << 0x10;
            actor->particles[i].unk0 = 0;
        }
        actor->particles[i].unk4 += 0xFFFD0000;
        actor->particles[i].unk4 &= 0xFFFFFF;
    }
    func_800483FC(&D_801248D4, func_80063A9C, (s32)actor);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80063FC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_800640D8.s")

void func_8006426C(s32 arg0) {
    func_800483FC(&D_801248C8, func_800640D8, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_8006429C.s")

void func_800643B4(void *arg0, u16 *arg1) {
    *(u16 **)((u8 *)arg0 + 0x30) = arg1;
    *(s16 *)((u8 *)arg0 + 0x34) = arg1[0];
    *(s16 *)((u8 *)arg0 + 0x36) = arg1[1];
    func_80045990(func_80043040(D_8011216C), arg1[2], (u8 *)arg0 + 0x44, (u8 *)arg0 + 0x40);
}

s32 func_80064414(void *arg0) {
    s16 temp_v0;

    temp_v0 = *(s16 *)((u8 *)arg0 + 0x34);
    if (!(temp_v0 & 0x7FFF)) {
        if (temp_v0 & 0x8000) {
            return 1;
        }
        func_800643B4(arg0, *(u16 **)((u8 *)arg0 + 0x30) + 3);
        goto block_5;
    }
    *(s16 *)((u8 *)arg0 + 0x34) = temp_v0 - 1;
block_5:
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80064470.s")

void func_800647E0(RaceUiProjectileActor *arg0) {
    struct {
        s16 mtx[0x10];
        s32 pad;
    } sp24;
    RaceUiProjectileActor *actor;
    s32 temp;

    actor = arg0;
    if (!D_80121B56) {
        func_80097FE4(sp24.mtx, D_80121D80[actor->index].pitch, D_80121D80[actor->index].yaw);
        func_80098590(sp24.mtx, &actor->velocity, &actor->pos);
        actor->pos.a += D_80121D80[actor->index].pos28.a;
        actor->pos.b += D_80121D80[actor->index].pos28.b + actor->verticalVelocity;
        actor->pos.c += D_80121D80[actor->index].pos28.c;
        temp = (actor->verticalVelocity += actor->verticalAcceleration);
        actor->verticalAcceleration += 0x10000;
        if (temp >= 0x300001) {
            func_800716E4(actor);
            return;
        }
    }
    func_800483FC(&D_801248C8, func_80064470, actor);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80064914.s")

void func_80064B28(RaceUiProjectileActor *arg0) {
    FixedMatrix3sScratch sp2C;
    RacePlayerState *player;
    RaceUiProjectileActor *actor;

    actor = arg0;
    if (D_80121B56 == 0) {
        player = &D_80121D80[actor->index];
        arg0 += 0;
        if (((!actor) && (!actor)) && (!actor)) {
        }
        player = &D_80121D80[actor->index];
        func_80097FE4(sp2C, player->pitch, player->yaw);
        func_80098590(sp2C, &actor->velocity, &actor->pos);

        player = &D_80121D80[actor->index];
        actor->pos.a += player->pos28.a;
        actor->pos.b += player->pos28.b + actor->verticalVelocity;
        actor->pos.c += player->pos28.c;
        actor->verticalAcceleration += 0xFFFF0000;
        actor->verticalVelocity += actor->verticalAcceleration;
        if (actor->verticalVelocity <= 0) {
            actor->verticalVelocity = 0;
            func_80072A74(0x68, &player->pos28, 0x7F, 0x32);
            func_80064414(actor);
            func_80071824(actor, func_80064914);
        }
    }
    func_800483FC(&D_801248C8, func_80064470, actor);
}

void func_80064C68(RaceUiProjectileActor *arg0) {
    FixedMatrix3sScratch sp2C;
    RacePlayerState *player;
    RaceUiProjectileActor *actor;

    actor = arg0;
    if (D_80121B56 == 0) {
        func_80097FE4(sp2C, D_80121D80[actor->index].pitch, D_80121D80[actor->index].yaw);
        func_80098590(sp2C, &actor->velocity, &actor->pos);

        player = &D_80121D80[actor->index];
        actor->pos.a += player->pos28.a;
        actor->pos.b += player->pos28.b;
        actor->pos.c += player->pos28.c;
        func_80064414(actor);
        if (actor->flags != 0) {
            func_80071824(actor, func_80064B28);
            actor->verticalAcceleration = 0x50000;
            actor->verticalVelocity += 0x50000;
        }
    }
    func_800483FC(&D_801248C8, func_80064470, actor);
}

void func_80064D88(RaceUiProjectileActor *arg0) {
    FixedMatrix3sScratch sp2C;
    RacePlayerState *player;
    RaceUiProjectileActor *actor;

    actor = arg0;
    if (D_80121B56 == 0) {
        func_80097FE4(sp2C, D_80121D80[actor->index].pitch, D_80121D80[actor->index].yaw);
        func_80098590(sp2C, &actor->velocity, &actor->pos);

        player = &D_80121D80[actor->index];
        actor->pos.a += player->pos28.a;
        actor->pos.b += player->pos28.b + actor->verticalVelocity;
        actor->pos.c += player->pos28.c;
        actor->verticalVelocity += actor->verticalAcceleration;
        actor->verticalAcceleration += 0xFFFF0000;
        if (actor->verticalVelocity <= 0) {
            actor->verticalVelocity = 0;
            func_80071824(actor, func_80064C68);
        }
    }
    func_800483FC(&D_801248C8, func_80064470, actor);
}

void func_80064EAC(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x24) = 0;
    *(s32 *)((u8 *)arg0 + 0x28) = 0x280000;
    *(s32 *)((u8 *)arg0 + 0x2C) = 0;
    *(s16 *)((u8 *)arg0 + 0x3C) = 0;
    *(s32 *)((u8 *)arg0 + 0x4C) = 0;
    *(s32 *)((u8 *)arg0 + 0x50) = 0x300000;
    *(s32 *)((u8 *)arg0 + 0x54) = 0;
    func_800643B4(arg0, D_800D6520);
    func_80072A74(0x66, &D_80121D80[*(u16 *)((u8 *)arg0 + 0x10)].pos28, 0x7F, 0x32);
    func_80071824(arg0, func_80064D88);
}

void func_80064F4C(void *arg0) {
    void *temp_s0;

    temp_s0 = arg0;
    if (D_80156608 == 0) {
        if (*(s8 *)((u8 *)arg0 + 0x30) != 0) {
            func_80045A78((s16)(*(s32 *)((u8 *)temp_s0 + 0x1C) - 0x66), *(s16 *)((u8 *)temp_s0 + 0x22), func_80043040(D_8011216E), 0x98);
            func_80059A04((u8 *)temp_s0 + 0x2C, *(s32 *)((u8 *)temp_s0 + 0x1C) + 0x26, *(s32 *)((u8 *)temp_s0 + 0x20), 0xC);
            return;
        }
        func_80045A78((s16)(*(s32 *)((u8 *)temp_s0 + 0x1C) - 0x62), *(s16 *)((u8 *)temp_s0 + 0x22), func_80043040(D_8011216E), 0x97);
        func_80059A04((u8 *)temp_s0 + 0x2C, *(s32 *)((u8 *)temp_s0 + 0x1C) + 0x22, *(s32 *)((u8 *)temp_s0 + 0x20), 0x10);
    }
}

void func_8006501C(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x1C) -= *(s32 *)((u8 *)arg0 + 0x28);
    *(s32 *)((u8 *)arg0 + 0x28) += 4;
    if (*(s32 *)((u8 *)arg0 + 0x28) == 0x38) {
        func_800716E4(arg0);
    } else {
        func_800483FC(&D_80124878, func_80064F4C, arg0);
    }
}

void func_8006507C(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = *(u16 *)((u8 *)arg0 + 0x18) - 1;
    if (*(u16 *)((u8 *)arg0 + 0x18) == 0) {
        func_80071824(arg0, func_8006501C);
    }
    func_800483FC(&D_80124878, func_80064F4C, arg0);
}

void func_800650D8(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x1C) -= *(s32 *)((u8 *)arg0 + 0x28);
    *(s32 *)((u8 *)arg0 + 0x28) -= 4;
    if (*(s32 *)((u8 *)arg0 + 0x28) == 0) {
        *(s16 *)((u8 *)arg0 + 0x18) = 0x5A;
        func_80071824(arg0, func_8006507C);
    }
    func_800483FC(&D_80124878, func_80064F4C, arg0);
}

void func_80065144(void *arg0) {
    s32 v0;
    *(s32 *)((u8 *)arg0 + 0x20) = -0x2C;
    *(s32 *)((u8 *)arg0 + 0x1C) = 0x1A4;
    *(s32 *)((u8 *)arg0 + 0x28) = 0x38;
    v0 = func_8007B130(&D_80121B74, &D_800EC9F0[D_80121B50 * 4 + 0x12A], (u8 *)arg0 + 0x2C, arg0);
    *(s8 *)((u8 *)arg0 + 0x30) = v0;
    func_80071824(arg0, func_800650D8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_800651BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80065508.s")

void func_8006565C(RaceUiGfxCommandActor *arg0) {
    register RaceUiGfxCommandActor *actor1;
    register RaceUiGfxCommandActor *actor2;
    register RaceUiGfxCommandScriptEntry *script;
    register s32 i;
    register s32 offset;
    register s32 one;

    script = D_800D693C[D_80121B50];
    actor1 = arg0;
    actor2 = arg0;
    i = 0;
    if (actor1->count > 0) {
        register RaceUiGfxCommandDest *template;

        template = &D_800DEE50;
        offset = 0;
        one = 1;
        do {
            script->active = one;
            actor1->particles[i] = *template;
            func_80048C90(&actor1->particles[i], script->command);
            i++;
            offset += sizeof(RaceUiGfxCommandDest);
            script++;
        } while (i < actor2->count);
    }
    osWritebackDCache(actor1->particles, actor1->count * sizeof(RaceUiGfxCommandDest));
}

void func_80065764(void *arg0) {
    RaceUiGfxCommandScriptEntry *var_v0;
    s32 var_v1;

    var_v0 = D_800D693C[D_80121B50];
    var_v1 = 0;
    if (var_v0->sentinel != -1) {
        do {
            var_v1++;
            var_v0++;
        } while (var_v0->sentinel != -1);
    }
    D_801222F2 = var_v1;
    *(s16 *)((u8 *)arg0 + 0x1E) = var_v1;
    if (var_v1 != 0) {
        D_80112130.resultTextHandle = func_80042D58(var_v1 << 6);
        *(s32 *)((u8 *)arg0 + 0x18) = func_80043040(D_80112130.resultTextHandle);
        func_8006565C(arg0);
        func_80071824(arg0, func_80065508);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80065808.s")

void func_80065CB8(void *arg0) {
    s16 temp_v1;
    void *temp_a2;

    temp_v1 = *(s16 *)((u8 *)arg0 + 0x30);
    temp_a2 = arg0;
    if (temp_v1 != 0) {
        *(s32 *)((u8 *)arg0 + 0x1C) += *(s32 *)((u8 *)arg0 + 0x28);
        *(s32 *)((u8 *)arg0 + 0x28) += 0x2000;
        *(s16 *)((u8 *)arg0 + 0x30) = temp_v1 - 1;
    } else {
        func_800716E4(temp_a2);
        return;
    }
    func_800483FC(&D_801248BC, func_80065808, temp_a2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80065D24.s")

void func_80065E0C(void *arg0) {
    s16 v1 = *(s16 *)((u8 *)arg0 + 0x30);
    if (v1 != 0) {
        *(s32 *)((u8 *)arg0 + 0x1C) -= *(s32 *)((u8 *)arg0 + 0x28);
        *(s32 *)((u8 *)arg0 + 0x28) -= 0x2000;
        *(s16 *)((u8 *)arg0 + 0x30) = v1 - 1;
    } else {
        *(s16 *)((u8 *)arg0 + 0x30) = 0x14;
        *(s16 *)((u8 *)arg0 + 0x32) = 0;
        func_80071824(arg0, func_80065D24);
    }
    func_800483FC(&D_801248BC, func_80065808, arg0);
}

void func_80065E90(RaceUiOverlayActor *arg0) {
    s32 i;

    arg0->x = 0;
    arg0->z = D_80121DA4 + 0xFF900000;
    arg0->velocity = 0;
    arg0->y = func_80080CC4(func_8007D200(0, 0, arg0->z), arg0->x, arg0->z) + 0x200000;

    for (i = 0; i < 0x40; i++) {
        arg0->velocity += 0x2000;
        arg0->y += arg0->velocity;
        arg0->timer++;
    }

    func_80045990(func_80043040(D_8011216E), 0x3A, &arg0->palette3A, &arg0->image3A);
    func_80045990(func_80043040(D_8011216E), 0x3B, &arg0->palette3B, &arg0->image3B);
    func_80071824(arg0, func_80065E0C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80065FD8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_80066158.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_ui_effects/func_800663C8.s")

void func_800666B0(void *arg0) {
    s16 *var_v0;
    s32 var_v1;

    var_v0 = D_800D761C[D_80121B50];
    var_v1 = 0;
    if (*var_v0 != -1) {
        do {
            var_v1++;
            var_v0 += 0xA;
        } while (*var_v0 != -1);
    }
    D_801222F6 = var_v1;
    *(s16 *)((u8 *)arg0 + 0x1C) = var_v1;
    if (var_v1 != 0) {
        D_80112130.rankTextHandle = func_80042D58(var_v1 << 6);
        *(s32 *)((u8 *)arg0 + 0x18) = func_80043040(D_80112130.rankTextHandle);
        func_800663C8(arg0);
        func_80071824(arg0, func_80066158);
    }
}
