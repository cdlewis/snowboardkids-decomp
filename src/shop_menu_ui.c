#include "common.h"

typedef struct ShopMenuWidgetActor ShopMenuWidgetActor;

typedef struct {
    char pad[0x1C];
    s32 shopItemPrice;
} MainMenuState;

typedef u8 ShopDescriptionText[0x8C];

struct ShopMenuWidgetActor {
    char pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    union {
        /* 0x1C */ s16 index;
        struct {
            /* 0x1C */ u8 state;
            /* 0x1D */ u8 pad1D;
        } bytes;
    } sprite;
    union {
        struct {
            /* 0x1E */ u8 state;
            /* 0x1F */ u8 timer;
        } bytes;
        /* 0x1E */ u16 counter;
        /* 0x1E */ s16 alpha;
    } transition;
    union {
        struct {
            /* 0x20 */ u8 state;
            /* 0x21 */ u8 timer;
            /* 0x22 */ u8 subState;
            /* 0x23 */ u8 subTimer;
        } bytes;
        /* 0x20 */ s16 counter;
        /* 0x20 */ s32 price;
    } item;
    union {
        struct {
            /* 0x24 */ u8 state;
        } bytes;
        /* 0x24 */ u8 slideState;
    } slide;
};

extern void func_80071824(ShopMenuWidgetActor *, void *);
extern void func_800716E4(ShopMenuWidgetActor *);
extern void func_800483FC(void *, void *, ShopMenuWidgetActor *);
extern void func_8002C9A0(ShopMenuWidgetActor *);
extern void func_8002C860(ShopMenuWidgetActor *);
extern void func_8002FAB8(void);
extern void func_8002DF40(void);
extern void func_8002E32C(void);
extern void func_8002E468(void);
extern void func_8002CFAC(void);
extern void func_8002D558(void);
extern void func_8002D9EC(void);
extern void func_8002DCE8(void);
extern void func_8002E9E4(void);
extern void func_8002EC5C(void);
extern void func_8002E5A4(ShopMenuWidgetActor *);
extern void func_8000F8AC(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_80013154(s32, s32, ShopDescriptionText, s32, s32, s32);
extern void func_80013D0C(s32, s32, char *, s32, s32);
extern void func_8001BA2C(s32, s32, s32, s32);
extern int sprintf(char *, const char *, ...);
extern s32 func_80043040(s16);
extern s32 D_800B34B0[];
extern ShopDescriptionText D_800B7A14[];
extern u8 D_80121D86;
extern u8 D_80121D88;
extern s16 D_8011217E;
extern u8 D_800EC9E6;
extern u8 D_8010AF70;
extern u8 D_8010AF71;
extern s16 D_8011217A;
extern s32 D_80121D8C;
extern s32 D_801235B4;
extern MainMenuState *D_801235B8;
extern u8 D_80124868;

const char D_800E0F60[] = "%6dG";

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002C4E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002C624.s")

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002C800.s")

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002C860.s")

void func_8002C9A0(ShopMenuWidgetActor *arg0) {
    u8 state = arg0->sprite.bytes.state;

    switch (state) {
    case 0:
        arg0->x += 0x20;
        if (arg0->x >= -0x88) {
            arg0->x = -0x88;
            arg0->sprite.bytes.state = 1;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 1:
        if (D_80121D88 == 1) {
            state = arg0->sprite.bytes.state = 2;
        }
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0x10D) {
            arg0->sprite.bytes.state = 3;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 3:
        break;
    }
    if ((unsigned int)state == 3) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002C860, arg0);
}

void func_8002CAA0(ShopMenuWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = 8;
    func_80071824(arg0, func_8002C9A0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002CAD4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002CFAC.s")

void func_8002D294(ShopMenuWidgetActor *arg0) {
    arg0->x = 0x94;
    arg0->y = -0x48;
    arg0->sprite.index = -8;
    arg0->transition.alpha = -0x74;
    arg0->item.counter = 0;
    arg0->item.bytes.subTimer = 0;
    arg0->item.bytes.subState = 0;
    func_80071824(arg0, func_8002CFAC);
}

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002D2E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002D558.s")

void func_8002D734(ShopMenuWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.index = 1;
    arg0->item.bytes.timer = 0;
    arg0->item.bytes.state = 0;
    func_80071824(arg0, func_8002D558);
}

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002D778.s")

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002D9EC.s")

void func_8002DC14(ShopMenuWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.index = 2;
    arg0->item.bytes.timer = 0;
    arg0->item.bytes.state = 0;
    func_80071824(arg0, func_8002D9EC);
}

void func_8002DC58(ShopMenuWidgetActor *arg0) {
    if (arg0->transition.bytes.state != 5) {
        func_8000F8AC(arg0->x, (s16)(arg0->y + (D_800EC9E6 * 0x1C)), func_80043040(D_8011217A), 7, 0x20, 0x20, 0, arg0->sprite.index, 0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002DCE8.s")

void func_8002DE6C(ShopMenuWidgetActor *arg0) {
    arg0->x = -0x7C;
    arg0->y = -0x50;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    arg0->transition.bytes.timer = 0;
    func_80071824(arg0, func_8002DCE8);
}

void func_8002DEAC(ShopMenuWidgetActor *arg0) {
    u8 var_v0;

    if (D_8010AF70 == 0) {
        var_v0 = 3;
    } else if (D_8010AF71 == 1) {
        var_v0 = 4;
    } else {
        var_v0 = D_800EC9E6;
    }
    func_80013154(arg0->x, arg0->y, D_800B7A14[var_v0], 1, arg0->sprite.index, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002DF40.s")

void func_8002E074(ShopMenuWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = 0xC;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8002DF40);
}

void func_8002E0B0(ShopMenuWidgetActor *arg0) {
    func_8000F8AC(arg0->x, arg0->y, func_80043040(D_8011217E), 0, 0x20, 0x20, 0, arg0->sprite.index, 0);
}

void func_8002E114(ShopMenuWidgetActor *arg0) {
    u8 state = arg0->transition.bytes.state;

    switch (state) {
    case 0:
        arg0->sprite.index += 0x26;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 1;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        if (D_80121D88 == 1) {
            state = arg0->transition.bytes.state = 2;
        }
        break;
    case 2:
        arg0->x += 0x20;
        if (arg0->x >= 0xA0) {
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        break;
    }
    if ((unsigned int)state == 3) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002E0B0, arg0);
}

void func_8002E214(ShopMenuWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x5C;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8002E114);
}

void func_8002E250(ShopMenuWidgetActor *arg0) {
    char sp40[0x18];

    func_8001BA2C(arg0->x, arg0->y, 0x5000, 0x4000);
    func_8000F8AC((s16)(arg0->x + 8), (s16)(arg0->y + 4), func_80043040(D_8011217A), 0x11, 0x20, 0x20, 0, arg0->sprite.index, 0);
    sprintf(sp40, D_800E0F60, D_80121D8C);
    func_80013D0C((s16)(arg0->x + 0x10), (s16)(arg0->y + 0x10), sp40, 0, arg0->sprite.index);
}

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002E32C.s")

void func_8002E42C(ShopMenuWidgetActor *arg0) {
    arg0->x = 0x30;
    arg0->y = 0x40;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8002E32C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002E468.s")

void func_8002E568(ShopMenuWidgetActor *arg0) {
    arg0->x = 0x30;
    arg0->y = 0x40;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8002E468);
}

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002E5A4.s")

void func_8002E6E4(ShopMenuWidgetActor *arg0) {
    s16 temp_v0;
    u8 var_v0;

    var_v0 = arg0->slide.slideState;
    switch (var_v0) {
    case 0:
        temp_v0 = arg0->sprite.index;
        arg0->x -= 0x20;
        if (temp_v0 >= arg0->x) {
            arg0->x = temp_v0;
            arg0->slide.slideState = 1U;
        }
        var_v0 = arg0->slide.slideState;
        break;
    case 1:
        if (D_80121D88 == 1) {
            arg0->slide.slideState = 2U;
            var_v0 = 2 & 0xFF;
        }
        break;
    }
    var_v0 = arg0->slide.slideState;
    if (var_v0 == 2) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002E5A4, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002E798.s")

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002E810.s")

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002E9E4.s")

void func_8002EC04(ShopMenuWidgetActor *arg0) {
    arg0->x = 0x20;
    arg0->y = -8;
    arg0->item.price = D_800B34B0[D_80121D86];
    arg0->sprite.index = 0;
    arg0->slide.bytes.state = 0;
    func_80071824(arg0, func_8002E9E4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002EC5C.s")

void func_8002EF14(ShopMenuWidgetActor *arg0) {
    ShopMenuWidgetActor *temp_a2;

    arg0->item.price = D_801235B8->shopItemPrice;
    temp_a2 = arg0;
    if (arg0->transition.counter < 0x10) {
        arg0->sprite.index -= 9;
    } else {
        temp_a2->sprite.index += 9;
    }
    temp_a2->transition.counter = (temp_a2->transition.counter + 1) & 0x1F;
    if (D_801235B4 == 1) {
        func_800716E4(temp_a2);
        D_801235B4 = 0;
    } else {
        func_800483FC(&D_80124868, func_8002EC5C, temp_a2);
    }
}

void func_8002EFB8(ShopMenuWidgetActor *arg0) {
    arg0->x = -0x76;
    arg0->y = -0x40;
    arg0->item.price = 0;
    arg0->sprite.index = 0x100;
    arg0->transition.counter = 0;
    func_80071824(arg0, func_8002EF14);
}

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002EFFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002F2C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002F854.s")

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002F8DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002FAB8.s")

void func_8002FBC8(ShopMenuWidgetActor *arg0) {
    arg0->x = 0x90;
    arg0->y = 0x44;
    arg0->sprite.bytes.state = 0;
    func_80071824(arg0, func_8002FAB8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002FC00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002FD70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002FEF8.s")
