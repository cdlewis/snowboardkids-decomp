#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s8 unk1E;
} MenuIntroActor;

typedef struct MenuItemActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ u8 state;
    /* 0x1D */ u8 pad1D[3];
    /* 0x20 */ struct MenuItemActor *child;
} MenuItemActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 alpha;
} FadeItemActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
} Struct801235B8;

typedef struct {
    /* 0x00 */ s16 x0;
    /* 0x02 */ s16 y0;
    /* 0x04 */ s16 x1;
    /* 0x06 */ s16 y1;
} Rect;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Rect rects[3];
    /* 0x30 */ u8 frame;
} RectListActor;

extern s32 func_80011D74(void *, s32, s16, s16);
extern void func_8000F8AC(s16, s16, s32, s32, s32, s32, s32, s32, s32);
extern s32 func_80043040(s16);
extern void func_80017168(void *, s32);
extern void func_80071824(void *, void *);
extern void func_80014AA4(void);
extern void func_80014EF0(MenuItemActor *);
extern void func_80014CB8(void *);
extern void func_8001508C(void *);
extern void func_800152D0(MenuItemActor *);
extern void func_80015680(MenuItemActor *);
extern void func_800157B4(void *);
extern void func_80015404(void *);
extern void func_80015A30(void *);
extern void func_80015B20(void *);
extern void func_80015BD8(void *);
extern void func_80015F4C(void);
extern void func_80016284(void);
extern void func_80016E40(void);
extern void func_800170AC(void *);
extern void func_800483FC(void *, void *, s32);
extern void *func_80071408(void *, s32, s32);
extern void func_800716E4(void *);
extern s32 func_80072138(s32, s32);
extern void func_800157EC(void *);
extern void func_80015C84(void *);
extern s16 D_8010AE38;
extern s16 D_8010AE3A;
extern s16 D_8010AE3C;
extern s16 D_8010AE3E;
extern s16 D_8010AE40;
extern s16 D_8010AE42;
extern s16 D_8010AE44;
extern s16 D_8010AE46;
extern s16 D_80112172;
extern s16 D_8011217C;
extern u8 D_80121B55;
extern u8 D_800EC9C1;
extern s32 D_801235B4;
extern Struct801235B8 *D_801235B8;
extern s32 D_80124838;
extern s32 D_80124868;

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ u16 unkA;
    /* 0x0C */ u16 unkC;
    /* 0x0E */ u16 unkE;
    /* 0x10 */ u8 unk10[0x4];
} SrcStruct_80017168;

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
    /* 0x12 */ s16 unk12;
    /* 0x14 */ u8 *unk14;
    /* 0x18 */ u8 *unk18;
    /* 0x1C */ u8 *unk1C;
    /* 0x20 */ u8 *unk20;
    /* 0x24 */ s16 unk24;
} DstStruct_80017168;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ DstStruct_80017168 sprite;
} SpriteActor;

#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80014600.s")

#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80014AA4.s")

void func_80014C7C(void *arg0) {
    MenuIntroActor *actor = arg0;

    actor->x = -0x70;
    actor->y = -0x1C;
    actor->unk1C = 0;
    actor->unk1E = 0;
    func_80071824(arg0, func_80014AA4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80014CB8.s")

void func_80014EF0(MenuItemActor *arg0) {
    MenuItemActor *child;

    switch (arg0->state) {
    case 0:
        arg0->x -= 0x20;
        if (arg0->x == 0x70) {
            arg0->child = func_80071408(func_80015404, 0, 0x63);
            func_80072138(1, 0x32);
        }
        if (arg0->x == -0x50) {
            arg0->state = 1;
        }
        break;
    case 1:
        if (D_800EC9C1 == 0x13) {
            arg0->state = 2;
            if (D_80121B55 == 1) {
                D_801235B4 = 1;
            }
        }
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x == -0x70) {
            child = arg0->child;
            child->state = 2;
            if (D_80121B55 == 2) {
                D_801235B4 = 1;
            }
        }
        break;
    default:
        break;
    }
    if (arg0->x < -0x108) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_80014CB8, (s32)arg0);
}

void func_80015054(void *arg0) {
    MenuItemActor *actor = arg0;

    actor->x = 0x90;
    actor->y = -0x18;
    actor->state = 0;
    func_80071824(arg0, func_80014EF0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_8001508C.s")

void func_800152D0(MenuItemActor *arg0) {
    MenuItemActor *child;

    switch (arg0->state) {
    case 0:
        arg0->x -= 0x20;
        if (arg0->x == 0x70) {
            arg0->child = func_80071408(func_800157B4, 0, 0x63);
            func_80072138(1, 0x32);
        }
        if (arg0->x == -0x50) {
            arg0->state = 1;
        }
        break;
    case 1:
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x == -0x70) {
            child = arg0->child;
            child->state = 2;
            if (D_80121B55 == 3) {
                D_801235B4 = 1;
            }
        }
        break;
    default:
        break;
    }
    if (arg0->x < -0x108) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001508C, (s32)arg0);
}

void func_80015404(void *arg0) {
    MenuItemActor *actor = arg0;

    actor->x = 0x90;
    actor->y = 8;
    actor->state = 0;
    func_80071824(arg0, func_800152D0);
}

extern void func_8001543C(void *);

#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_8001543C.s")

void func_80015680(MenuItemActor *arg0) {
    MenuItemActor *child;

    switch (arg0->state) {
    case 0:
        arg0->x -= 0x20;
        if (arg0->x == 0x70) {
            arg0->child = func_80071408(func_80015B20, 0, 0x63);
            func_80072138(1, 0x32);
        }
        if (arg0->x == -0x50) {
            arg0->state = 1;
        }
        break;
    case 1:
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x == -0x70) {
            child = arg0->child;
            child->state = 2;
            if (D_80121B55 == 4) {
                D_801235B4 = 1;
            }
        }
        break;
    default:
        break;
    }
    if (arg0->x < -0x108) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001543C, (s32)arg0);
}

void func_800157B4(void *arg0) {
    MenuItemActor *actor = arg0;

    actor->x = 0x90;
    actor->y = 0x28;
    actor->state = 0;
    func_80071824(arg0, func_80015680);
}

#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_800157EC.s")

void func_80015A30(void *arg0) {
    MenuItemActor *actor = arg0;
    s16 x;

    switch (actor->state) {
    case 0:
        actor->x -= 0x20;
        x = actor->x;
        if (x == -0x50) {
            actor->state = 1;
            func_80071408(func_80015C84, 0, 0x63);
            x = actor->x;
        }
        break;
    case 1:
        x = actor->x;
        break;
    case 2:
        actor->x -= 0x20;
        x = actor->x;
        break;
    default:
        x = actor->x;
        break;
    }
    if (x < -0x108) {
        func_800716E4(actor);
        D_801235B8->unk1C = 2;
        return;
    }
    func_800483FC(&D_80124868, func_800157EC, (s32)actor);
}

void func_80015B20(void *arg0) {
    MenuItemActor *actor = arg0;

    actor->x = 0x90;
    actor->y = 0x48;
    actor->state = 0;
    func_80071824(arg0, func_80015A30);
}

void func_80015B58(void *arg0) {
    FadeItemActor *actor = arg0;

    func_8000F8AC(
        actor->x,
        (s16)((actor->y + (D_80121B55 << 5)) - 0x20),
        func_80043040(D_80112172),
        4,
        0x20,
        0x20,
        0,
        (u16)actor->alpha,
        0);
}

void func_80015BD8(void *arg0) {
    FadeItemActor *actor = arg0;
    u16 temp_v0;
    s16 temp_t6;
    FadeItemActor *temp_a2;

    temp_v0 = actor->alpha;
    temp_a2 = actor;
    if ((((((((((temp_v0 & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) != 0x100) {
        temp_t6 = temp_v0 + 0x28;
        actor->alpha = temp_t6;
        if ((temp_t6 & 0xFFFF) >= 0x100) {
            actor->alpha = 0x100;
            D_801235B8->unk1C = 1;
        }
    } else if (D_801235B4 != 0) {
        temp_a2->x = temp_a2->x - 0x20;
    }
    if (temp_a2->x < -0x90) {
        func_800716E4(temp_a2);
    } else {
        func_800483FC(&D_80124868, func_80015B58, (s32)temp_a2);
    }
}

void func_80015C84(void *arg0) {
    FadeItemActor *actor = arg0;

    actor->x = -0x68;
    actor->y = -0x14;
    actor->alpha = 0;
    func_80071824(arg0, func_80015BD8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80015CBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80015F4C.s")

void func_8001621C(void *arg0) {
    RectListActor *actor = arg0;

    actor->rects[0].x0 = -0x114;
    actor->rects[1].x0 = -0xA4;
    actor->rects[0].y0 = -0x114;
    actor->rects[1].y0 = 0x48;
    actor->rects[0].x1 = 0x90;
    actor->rects[1].x1 = -0xA4;
    actor->rects[0].y1 = 0x90;
    actor->rects[1].y1 = 0x48;
    actor->rects[2].x0 = 0x8C;
    actor->rects[2].y0 = 0x44;
    actor->frame = 0;
    func_80071824(arg0, func_80015F4C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80016284.s")

void func_80016560(void *arg0) {
    RectListActor *temp_a2;
    RectListActor *actor = arg0;

    actor->rects[0].x0 = D_8010AE38;
    actor->rects[1].x0 = D_8010AE40;
    actor->rects[0].y0 = D_8010AE3A;
    actor->rects[1].y0 = D_8010AE42;
    actor->rects[0].x1 = D_8010AE3C;
    actor->rects[1].x1 = D_8010AE44;
    actor->rects[0].y1 = D_8010AE3E;
    temp_a2 = actor;
    actor->rects[1].y1 = D_8010AE46;
    func_800483FC(&D_80124868, func_80016284, (s32)temp_a2);
}

void func_800165F0(void *arg0) {
    RectListActor *actor = arg0;

    actor->rects[0].x0 = -0x114;
    actor->rects[1].x0 = -0xA4;
    actor->rects[0].y0 = -0x114;
    actor->rects[1].y0 = 0x48;
    actor->rects[0].x1 = 0x90;
    actor->rects[1].x1 = -0xA4;
    actor->rects[0].y1 = 0x90;
    actor->rects[1].y1 = 0x48;
    actor->rects[2].x0 = 4;
    actor->rects[2].y0 = 0x11;
    actor->rects[2].x1 = 2;
    actor->rects[2].y1 = 0x27;
    func_80071824(arg0, func_80016560);
}

#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80016664.s")

#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80016948.s")

#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80016B54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80016BE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80016E40.s")

void func_80017014(void *arg0) {
    RectListActor *actor = arg0;

    actor->rects[0].x0 = -0x70;
    actor->rects[1].x0 = -0x34;
    actor->rects[2].x0 = -0x34;
    actor->rects[0].y0 = -0x70;
    actor->rects[1].y0 = 0x30;
    actor->rects[2].y0 = 0x30;
    actor->rects[0].x1 = 0x1C;
    actor->rects[1].x1 = -0x34;
    actor->rects[2].x1 = -0x34;
    actor->rects[0].y1 = 0x1C;
    actor->rects[1].y1 = 0x30;
    actor->rects[2].y1 = 0x30;
    func_80071824(arg0, func_80016E40);
}

void func_80017078(void *arg0) {
    SpriteActor *actor = arg0;

    func_80011D74(&actor->sprite, 0, actor->x, actor->y);
}

void func_800170AC(void *arg0) {
    SpriteActor *temp_a2;
    DstStruct_80017168 *temp_v0;
    SpriteActor *actor = arg0;

    temp_a2 = actor;
    actor->sprite.unk0--;
    temp_v0 = &temp_a2->sprite;
    if (actor->sprite.unk0 < 0) {
        actor->sprite.unk0 = 0x2FF;
    }
    temp_v0->unk2 = (temp_v0->unk2 + 1) & 0x1FF;
    func_800483FC(&D_80124838, func_80017078, (s32)temp_a2);
}

void func_8001710C(void *arg0) {
    SpriteActor *temp_a2 = arg0;

    func_80017168(&temp_a2->sprite, func_80043040(D_8011217C));
    temp_a2->x = temp_a2->sprite.unk8;
    temp_a2->y = temp_a2->sprite.unkA;
    func_80071824(temp_a2, func_800170AC);
}

void func_80017168(void *arg0, s32 arg1) {
    DstStruct_80017168 *dst = arg0;
    SrcStruct_80017168 *src = (SrcStruct_80017168 *)arg1;

    dst->unk0 = 0;
    dst->unk2 = 0;
    dst->unk8 = src->unk0;
    dst->unkA = src->unk2;
    dst->unk4 = src->unk4;
    dst->unk6 = src->unk6;
    dst->unkC = -0x90;
    dst->unkE = -0x68;
    dst->unk10 = 0x120;
    dst->unk12 = 0xD0;
    dst->unk18 = src->unkA + (u8 *)src;
    dst->unk14 = src->unkE + (u8 *)src;
    dst->unk1C = src->unk10;
    dst->unk20 = src->unkC + (u8 *)src;
    dst->unk24 = -1;
}

void n_alSynFreeFX(s32 arg0) {

}
