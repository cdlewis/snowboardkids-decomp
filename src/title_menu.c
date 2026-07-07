#include "common.h"

#define TITLE_MENU_SECONDARY_TEXTURE_HANDLE (*(s16 *)&D_80112130[0x3E])
#define TITLE_MENU_FRAME_TEXTURE_HANDLE (*(s16 *)&D_80112130[0x42])
#define RACE_PLAYER_STATE_SIZE 0x60C

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 alpha;
    /* 0x1E */ u8 state;
    /* 0x1F */ u8 timer;
    /* 0x20 */ struct MenuItemActor *child;
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
    /* 0x18 */ Rect rects[2];
    /* 0x28 */ u16 stepLimit;
    /* 0x2A */ u16 stepIncrement;
    /* 0x2C */ u16 stepAccumulator;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ u8 frame;
} RectListActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x[4];
    /* 0x20 */ s16 y[4];
    /* 0x28 */ s16 topY[4];
    /* 0x30 */ s16 slideOffset[4];
    /* 0x38 */ u16 selection[4];
    /* 0x40 */ u16 alpha[4];
    /* 0x48 */ s16 alphaTimer[4];
} TitleMenuTransitionActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x[4];
    /* 0x20 */ s16 y[4];
    /* 0x28 */ s16 alpha[4];
    /* 0x30 */ u8 frame;
    /* 0x31 */ u8 unk31[4];
} TitleMenuWidgetActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ u8 pad1A[0x6];
    /* 0x20 */ s16 y;
} TitleMenuWidgetItemView;

typedef struct {
    /* 0x00 */ u8 state;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ s16 alpha;
} TitleIntroTransitionState;

extern s32 func_80011D74(void *, s32, s16, s16);
extern void func_8000F8AC(s16, s16, s32, s32, s32, s32, s32, s32, s32);
extern void func_80010074(s16, s16, s32, s32, s32);
extern void func_800129DC(s16, s16, u16 *, s32, s32);
extern s32 func_80043040(s16);
extern void func_80017168(void *, s32);
extern void func_80071824(void *task, void (*callback)());
extern void func_80014600(MenuIntroActor *);
extern void func_80014AA4(MenuIntroActor *);
extern void func_80014EF0(MenuItemActor *);
extern void func_80014CB8(void *);
extern void func_80015054(void *);
extern void func_8001508C(void *);
extern void func_800152D0(MenuItemActor *);
extern void func_80015680(MenuItemActor *);
extern void func_800157B4(void *);
extern void func_80015404(void *);
extern void func_80015A30(void *);
extern void func_80015B20(void *);
extern void func_80015BD8(void *);
extern void func_80015F4C(RectListActor *);
extern void func_80016284(void);
extern void func_80016948(TitleMenuWidgetActor *);
extern void func_80016E40(void);
extern void func_800170AC(void *);
extern void func_800483FC(void *, void *, s32);
extern void *func_80071408(void *, s32, s32);
extern void func_800716E4(void *);
extern s32 func_80072138(s32, s32);
extern void func_80015C84(void *);
#ifdef NON_MATCHING
extern void func_8000F030(s16, s16, s32, s32, s32, s32, s32, s32);
extern void func_80013154(s16, s16, u8 *, s32, s32, s32);
extern void func_80013D0C(s16, s16, u8 *, u16, u16);
extern u8 D_800B5458[][0x4C];
extern u8 D_800B5A14[];
extern s32 D_80121D8C;
#endif
extern MenuIntroActor *D_8010ADDC;
extern s16 D_800EC9C8[];
extern s16 D_800EC9D0[];
extern TitleIntroTransitionState D_8010AE00;
extern s16 D_8010AE02;
extern u16 D_8010AE06[];
extern u8 D_8010AE0E[];
extern u16 D_8010AE12[];
extern s16 D_8010AE38;
extern s16 D_8010AE3A;
extern s16 D_8010AE3C;
extern s16 D_8010AE3E;
extern s16 D_8010AE40;
extern s16 D_8010AE42;
extern s16 D_8010AE44;
extern s16 D_8010AE46;
extern u16 D_800B5408[];
extern u16 D_800B541C[];
extern u16 D_800B5430[];
extern u16 D_800B5444[];
extern u8 D_80112130[];
extern s16 D_8011216E;
extern s16 D_80112172;
extern s16 D_8011217C;
extern u8 D_80121B55;
extern u8 D_800B5A2E[];
extern u8 D_800B5A2F[];
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

// func_80014AA4 best match: 99.573%
#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80014AA4.s")

#ifdef NON_MATCHING
void func_80014AA4(MenuIntroActor *arg0) {
    TitleIntroTransitionState *global;
    MenuIntroActor *actor;
    s32 globalState;
    s16 alpha;
    u8 state;
    s32 step;
    u32 stateCopy;
    u8 timer;

    global = &D_8010AE00;
    state = arg0->state;
    globalState = global->state;
    actor = arg0;
    stateCopy = state;
    if (stateCopy != globalState) {
        arg0->state = globalState;
        state = globalState;
        arg0->alpha = D_8010AE00.alpha;
    }
    step = 0x10;

    alpha = actor->alpha;
    if (alpha != 0x100) {
        if (state == 0) {
            actor->alpha = alpha + 0x20;
            alpha = actor->alpha;
            if (alpha == 0x100) {
                alpha = ((volatile MenuIntroActor *)actor)->alpha;
                actor->state = 1;
            }
        } else {
            actor->alpha = alpha - 0x30;
            alpha = actor->alpha;
            if (alpha <= 0) {
                actor->alpha = 0;
                alpha = actor->alpha;
            }
        }
    } else {
        switch (state) {
        case 1:
        case 6:
            timer = actor->timer;
            alpha = ((volatile MenuIntroActor *)actor)->alpha;
            actor->timer = ((timer + 1) & 0xFFFFu) & 0xF;
            break;
        case 2:
            actor->y -= 0x10;
            if (actor->y == -0x5C) {
                actor->child = func_80071408(func_80015054, 0, 0x63);
                func_80072138(1, 0x32);
                actor->state = 3;
            }
            alpha = actor->alpha;
            break;
        case 3:
            if (D_801235B8->unk1C == 2) {
                alpha = ((volatile MenuIntroActor *)actor)->alpha;
                actor->state = 4;
            }
            break;
        case 4:
            actor->y += step;
            if (actor->y == -0x1C) {
                actor->state = 5;
            }
            alpha = actor->alpha;
            break;
        case 5:
        case 7:
        case 8:
            break;
        }
    }

    if (alpha == 0) {
        actor->state = 8;
    }
    D_8010AE00.state = actor->state;
    D_8010AE02 = actor->alpha;
    if (actor->state != 8) {
        func_800483FC(&D_80124868, func_80014600, (s32)actor);
    }
}
#endif

void func_80014C7C(void *arg0) {
    MenuIntroActor *actor = arg0;

    actor->x = -0x70;
    actor->y = -0x1C;
    actor->alpha = 0;
    actor->state = 0;
    func_80071824(arg0, func_80014AA4);
}

void func_80014CB8(void *arg0) {
    MenuItemActor *actor = arg0;
    s16 unused;
    s16 width;
    s16 xOffset;
    s16 yOffset;
    s32 alpha;
    s32 temp;

    if (actor->state == 0) {
        alpha = 0x100;
        width = 0x18;
        xOffset = 0x10;
        yOffset = 0;
    } else if (D_80121B55 == 1) {
        alpha = 0x100;
        width = 0x20;
        xOffset = 0xC;
        yOffset = 8;
    } else {
        alpha = 0x60;
        width = 0x10;
        xOffset = 0x14;
        yOffset = -4;
    }

    if ((D_800EC9C1 == 0) || (D_800EC9C1 & 1) || (D_80121B55 != 1)) {
        func_8000F8AC(actor->x, actor->y, func_80043040(D_80112172), 3, 0x20, 0x20, 0, alpha, 0);
        if (width == 0x20) {
            temp = width & 0xFFFF;
            func_8000F8AC((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), func_80043040(D_8011216E), 0, temp, temp, 0, alpha, 0);
        } else {
            func_80010074((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), func_80043040(D_8011216E), 0, alpha);
        }
        func_800129DC((s16)(actor->x + 0x30), (s16)(actor->y + 4), D_800B5408, 1, alpha);
    }

    func_8000F8AC((s16)(actor->x + 0x80), actor->y, func_80043040(D_80112172), 7, 0x20, 0x20, 0, alpha, 0);
}

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

void func_8001508C(void *arg0) {
    MenuItemActor *actor = arg0;
    s16 unused;
    s16 width;
    s16 xOffset;
    s16 yOffset;
    s32 alpha;
    s32 temp;
    s32 i;

    if (actor->state == 0) {
        alpha = 0x100;
        width = 0x18;
        xOffset = 0x10;
        yOffset = 0;
    } else if (D_80121B55 == 2) {
        alpha = 0x100;
        width = 0x20;
        xOffset = 0xC;
        yOffset = 8;
    } else {
        alpha = 0x60;
        width = 0x10;
        xOffset = 0x14;
        yOffset = -4;
    }

    if ((D_800EC9C1 == 0) || (D_800EC9C1 & 1) || (D_80121B55 != 2)) {
        func_8000F8AC(actor->x, actor->y, func_80043040(TITLE_MENU_FRAME_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, alpha, 2);
        if (width == 0x20) {
            temp = width & 0xFFFF;
            func_8000F8AC((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), func_80043040(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 1, temp, temp, 0, alpha, 0);
        } else {
            func_80010074((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), func_80043040(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 1, alpha);
        }
        func_800129DC((s16)(actor->x + 0x30), (s16)(actor->y + 4), D_800B541C, 1, alpha);
    }

    for (i = 0; i != 0x28; i += 0x14) {
        func_8000F8AC((s16)(actor->x + i + 0x80), actor->y, func_80043040(TITLE_MENU_FRAME_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, alpha, 0);
    }
}

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

void func_8001543C(void *arg0) {
    MenuItemActor *actor = arg0;
    s16 unused;
    s16 width;
    s16 xOffset;
    s16 yOffset;
    s32 alpha;
    s32 temp;
    s32 i;

    if (actor->state == 0) {
        alpha = 0x100;
        width = 0x18;
        xOffset = 0x10;
        yOffset = 0;
    } else if (D_80121B55 == 3) {
        alpha = 0x100;
        width = 0x20;
        xOffset = 0xC;
        yOffset = 8;
    } else {
        alpha = 0x60;
        width = 0x10;
        xOffset = 0x14;
        yOffset = -4;
    }

    if ((D_800EC9C1 == 0) || (D_800EC9C1 & 1) || (D_80121B55 != 3)) {
        func_8000F8AC(actor->x, actor->y, func_80043040(TITLE_MENU_FRAME_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, alpha, 3);
        if (width == 0x20) {
            temp = width & 0xFFFF;
            func_8000F8AC((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), func_80043040(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 2, temp, temp, 0, alpha, 0);
        } else {
            func_80010074((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), func_80043040(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 2, alpha);
        }
        func_800129DC((s16)(actor->x + 0x30), (s16)(actor->y + 4), D_800B5430, 1, alpha);
    }

    for (i = 0; i != 0x3C; i += 0x14) {
        func_8000F8AC((s16)(actor->x + i + 0x80), actor->y, func_80043040(TITLE_MENU_FRAME_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, alpha, 0);
    }
}

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

void func_800157EC(void *arg0) {
    MenuItemActor *actor = arg0;
    s16 unused;
    s16 width;
    s16 xOffset;
    s16 yOffset;
    s32 alpha;
    s32 temp;
    s32 i;

    if (actor->state == 0) {
        alpha = 0x100;
        width = 0x18;
        xOffset = 0x10;
        yOffset = 0;
    } else if (D_80121B55 == 4) {
        alpha = 0x100;
        width = 0x20;
        xOffset = 0xC;
        yOffset = 8;
    } else {
        alpha = 0x60;
        width = 0x10;
        xOffset = 0x14;
        yOffset = -4;
    }

    if ((D_800EC9C1 == 0) || (D_800EC9C1 & 1) || (D_80121B55 != 4)) {
        func_8000F8AC(actor->x, actor->y, func_80043040(TITLE_MENU_FRAME_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, alpha, 4);
        if (width == 0x20) {
            temp = width & 0xFFFF;
            func_8000F8AC((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), func_80043040(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 3, temp, temp, 0, alpha, 0);
        } else {
            func_80010074((s16)(actor->x + xOffset), (s16)(actor->y - yOffset), func_80043040(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 3, alpha);
        }
        func_800129DC((s16)(actor->x + 0x30), (s16)(actor->y + 4), D_800B5444, 1, alpha);
    }

    for (i = 0; i != 0x50; i += 0x14) {
        func_8000F8AC((s16)(actor->x + i + 0x80), actor->y, func_80043040(TITLE_MENU_FRAME_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, alpha, 0);
    }
}

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

// func_80015CBC best match: 92.661% (nonmatchings/func_80015CBC-690418013071298896/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80015CBC.s")

#ifdef NON_MATCHING
void func_80015CBC(TitleMenuWidgetItemView *arg0) {
    char text[0x1C];
    s32 alpha;
    s32 scale;
    s32 next;
    s32 i;
    TitleMenuWidgetItemView *item;
    u8 *selected;
    u8 *textures;

    textures = D_80112130;
    selected = &D_80121B55;
    item = arg0;
    i = 0;
    do {
        alpha = 0x100;
        if (((s32)*selected - 1) < i) {
            alpha = 0x50;
        }

        func_8000F8AC(item->x, item->y, func_80043040(TITLE_MENU_FRAME_TEXTURE_HANDLE), 0xA, 0x20, 0x20, 0, alpha, (i + 1) & 0xFF);
        func_8000F8AC((s16)(item->x + 0x40), item->y, func_80043040(TITLE_MENU_FRAME_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, alpha, (i + 1) & 0xFF);
        func_8000F8AC((s16)(item->x + 0x80), item->y, func_80043040(TITLE_MENU_FRAME_TEXTURE_HANDLE), 0xC, 0x20, 0x20, 0, alpha, (i + 1) & 0xFF);

        next = i + 1;
        scale = next & 0xFF;
        sprintf(text, "%d", next);
        func_80013D0C((s16)(item->x + 0x32), (s16)(item->y + 2), text, 0, alpha);
        if (alpha == 0x100) {
            sprintf(text, "%6d", *(s32 *)((u8 *)&D_80121D8C + (i * RACE_PLAYER_STATE_SIZE)));
            func_80013D0C((s16)(item->x + 0x44), (s16)(item->y + 0x1B), text, 0, alpha);
        } else {
            func_8000F8AC((s16)(item->x + 2), (s16)(item->y + 0x14), func_80043040(TITLE_MENU_SECONDARY_TEXTURE_HANDLE), 0x90, 0x20, 0x20, 0, 0xF0, 0);
        }
        i = next;
        item = (TitleMenuWidgetItemView *)((s16 *)item + 1);
    } while (next != 4);
}
#endif

// func_80015F4C best match: 98.817% (nonmatchings/func_80015F4C-6516277587347797853/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80015F4C.s")

#ifdef NON_MATCHING
void func_80015F4C(RectListActor *arg0) {
    u8 state;
    s32 outer;
    s32 i;
    s16 delta0;
    s16 delta1;
    s16 *coords;

    outer = D_8010AE04;
    state = arg0->frame;
    if (state != D_8010AE04) {
        state = outer;
        arg0->frame = outer;
    }

    switch (state) {
    case 0:
        outer = 0;
        do {
            i = 0;
            coords = &arg0->rects[0].x0;
            do {
                if (i < 2) {
                    delta0 = 1;
                } else {
                    delta0 = -1;
                }
                if (i & 1) {
                    delta1 = -1;
                } else {
                    delta1 = 1;
                }

                coords[0] += delta0;

                if (i == 0) {
                    arg0->stepAccumulator += arg0->stepIncrement;
                }
                if (arg0->stepAccumulator >= arg0->stepLimit) {
                    coords[4] += delta1;
                    if (i == 3) {
                        arg0->stepAccumulator -= arg0->stepLimit;
                    }
                }

                if (i <= 0) {
                    delta0 = 1;
                } else {
                    delta0 = -1;
                }
                if ((i + 1) & 1) {
                    delta1 = -1;
                } else {
                    delta1 = 1;
                }

                coords[1] += delta0;

                if (i == -1) {
                    arg0->stepAccumulator += arg0->stepIncrement;
                }
                if (arg0->stepAccumulator >= arg0->stepLimit) {
                    coords[5] += delta1;
                    if (i == 2) {
                        arg0->stepAccumulator -= arg0->stepLimit;
                    }
                }

                i += 2;
                coords += 2;
            } while (i != 4);

            outer++;
            if (arg0->rects[0].x0 == -0x88) {
                arg0->frame = 1;
                break;
            }
        } while (outer != 0x10);
        state = arg0->frame;
        break;
    case 2:
        arg0->rects[1].y0 += 0x10;
        arg0->rects[1].y1 += 0x10;
        if (arg0->rects[1].y0 == 0x24) {
            arg0->frame = 3;
        }
        state = arg0->frame;
        break;
    case 1:
    case 3:
    default:
        break;
    }

    D_8010AE04 = state;
    D_8010AE38 = arg0->rects[0].x0;
    D_8010AE40 = arg0->rects[1].x0;
    D_8010AE3A = arg0->rects[0].y0;
    D_8010AE42 = arg0->rects[1].y0;
    D_8010AE3C = arg0->rects[0].x1;
    D_8010AE44 = arg0->rects[1].x1;
    D_8010AE3E = arg0->rects[0].y1;
    D_8010AE46 = arg0->rects[1].y1;

    if (D_801235B4 == 0x63) {
        func_800716E4(arg0);
        D_801235B4 = 0;
        return;
    }

    func_800483FC(&D_80124868, func_80015CBC, (s32)arg0);
}
#endif

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
    actor->stepLimit = 0x8C;
    actor->stepIncrement = 0x44;
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
    actor->stepLimit = 4;
    actor->stepIncrement = 0x11;
    actor->stepAccumulator = 2;
    actor->unk2E = 0x27;
    func_80071824(arg0, func_80016560);
}

// func_80016664 best match: 99.892% (nonmatchings/func_80016664-1197934324348345530/base_13.c)
#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80016664.s")

#ifdef NON_MATCHING
void func_80016664(TitleMenuWidgetActor *arg0) {
    volatile s32 unused;
    MenuIntroActor *intro;
    register TitleMenuWidgetActor *actor;
    u8 *text;
    s32 i;
    s16 state;
    s32 alpha;

    actor = arg0;
    intro = D_8010ADDC;
    for (i = 0; i < D_80121B55; i++) {
        state = D_800EC9C8[i];
        if (state != 8) {
            if (state == 5) {
                alpha = (u16)actor->alpha[i];
            } else {
                alpha = 0x100;
            }

            func_8000F8AC((s16)(actor->x[i] - 2), (s16)(actor->y[i] + 0xC), func_80043040(TITLE_MENU_FRAME_TEXTURE_HANDLE), 8,
                          0x20, 0x20, 0, alpha, 0);
            func_8000F8AC((s16)(actor->x[i] + 0x3E), (s16)(actor->y[i] + 0xC), func_80043040(TITLE_MENU_FRAME_TEXTURE_HANDLE), 9,
                          0x20, 0x20, 0, alpha, 0);

            state = D_800EC9C8[i];
            if (state == 0xA) {
                func_80013D0C((s16)(actor->x[i] + 2), (s16)(actor->y[i] + 0x10), D_800B5A14, 7, actor->alpha[i]);
            } else {
                text = D_800B5458[state];
                state = 2;
                func_80013154((s16)(actor->x[i] + state), (s16)(actor->y[i] + 0x10), text, 1, actor->alpha[i], 0);
            }

            if ((actor->alpha[i] == 0x100) && (intro->state == 8)) {
                state = D_800EC9C8[i];
                if ((state == 4) || ((state >= 7) && (state != 0x12) && (D_800EC9D0[i] == 0))) {
                    func_8000F030((s16)(actor->x[i] + 0x70), (s16)(actor->y[i] + 0x20), func_80043040(TITLE_MENU_FRAME_TEXTURE_HANDLE),
                                  (((s32)actor->frame >= 8) + 5) & 0xFFFF, 0x20, 0x20, 0, 0);
                }
            }
        }
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80016948.s")

void func_80016B54(TitleMenuWidgetActor *arg0) {
    s32 i;
    TitleMenuWidgetActor *new_var;

    arg0->x[0] = -0x114;
    arg0->y[0] = -0xA4;
    arg0->x[1] = -0x114;
    arg0->y[1] = 0x48;
    arg0->x[2] = 0x90;
    new_var = arg0;
    new_var->y[2] = -0xA4;
    new_var->x[3] = 0x90;
    new_var->y[3] = 0x48;

    for (i = 0; i < D_80121B55; i++) {
        new_var->alpha[i] = 0x100;
        new_var->unk31[i] = 0;
    }

    func_80071824(new_var, func_80016948);
}

// func_80016BE8 best match: 89.510% (nonmatchings/func_80016BE8-7387615772158234395/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/title_menu/func_80016BE8.s")

#ifdef NON_MATCHING
void func_80016BE8(TitleMenuTransitionActor *arg0) {
    s32 i;
    s32 alpha;
    s16 state;

    if (D_8010ADDC->state == 8) {
        for (i = 0; i < D_80121B55; i++) {
            if (D_8010AE06[i] != arg0->selection[i]) {
                arg0->selection[i] = D_8010AE06[i];
            }

            state = D_800EC9D0[i];
            if (state != 0) {
                if (!(state & 1)) {
                    alpha = 0x100;
                } else {
                    alpha = 0x60;
                }

                func_8000F8AC(arg0->x[i], arg0->topY[i], func_80043040(TITLE_MENU_FRAME_TEXTURE_HANDLE),
                              D_800B5A2F[D_800EC9C8[i] * 2], 0x20, 0x20, 0, alpha, 0);

                if (alpha == 0x100) {
                    alpha = 0x60;
                } else {
                    alpha = 0x100;
                }

                func_8000F8AC(arg0->x[i], arg0->y[i], func_80043040(TITLE_MENU_FRAME_TEXTURE_HANDLE),
                              D_800B5A2E[D_800EC9C8[i] * 2], 0x20, 0x20, 0, alpha, 0);

                state = D_800EC9D0[i];
                if ((state == 3) || (state == 4)) {
                    func_8000F8AC(arg0->x[i], (s16)(((D_800EC9D0[i] * 0x10) + arg0->y[i]) - 0x30),
                                  func_80043040(TITLE_MENU_FRAME_TEXTURE_HANDLE), 0x12, 0x20, 0x20, 0, arg0->alpha[i],
                                  i + 7);
                    state = D_800EC9D0[i];
                }
            }

            if ((state >= 5) && (arg0->slideOffset[i] == 0)) {
                D_8010AE12[i] = arg0->selection[i];
                D_8010AE0E[i] = 2;
                D_800EC9D0[i] = 0;
            }
        }
    }
}
#endif

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
