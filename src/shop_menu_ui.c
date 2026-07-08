#include "common.h"

typedef struct ShopMenuWidgetActor ShopMenuWidgetActor;

typedef struct {
    char pad[0x1C];
    s32 shopItemPrice;
    s32 unk20;
} MainMenuState;

typedef struct {
    char pad0[0x6];
    /* 0x06 */ u8 selectedShopItem;
    char pad7[0x1];
    /* 0x08 */ u8 shopMenuState;
    char pad9[0x3];
    /* 0x0C */ s32 money;
} ShopMenuState;

typedef struct {
    union {
        struct {
            /* 0x00 */ u8 unk0[4];
            /* 0x04 */ u8 playerOneCourseDecided;
            /* 0x05 */ u8 unk5[3];
            /* 0x08 */ u8 playerTwoCourseDecided;
            /* 0x09 */ u8 unk9[7];
            /* 0x10 */ u8 unk10[4];
        };
        struct {
            /* 0x00 */ u8 unk0Array[4];
            /* 0x04 */ u8 unk4Array[4];
            /* 0x08 */ u8 unk8Array[4];
            /* 0x0C */ u8 unkCArray[4];
            /* 0x10 */ u8 unk10Array[4];
        };
    };
    /* 0x14 */ s16 unk14[4];
    /* 0x1C */ s16 unk1C[4];
    /* 0x24 */ u8 unk24[4];
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 transitionState;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F[9];
} CourseSelectStatus;

typedef u8 ShopDescriptionText[0x8C];

typedef struct {
    char pad0[0x18];
    /* 0x18 */ s16 unk18[5];
    /* 0x22 */ s16 unk22;
    /* 0x24 */ u8 unk24;
    /* 0x25 */ u8 unk25;
    /* 0x26 */ u8 unk26;
} ShopMenuRowActor;

struct ShopMenuWidgetActor {
    char pad0[0x18];
    union {
        struct {
            union {
                struct {
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
                            /* 0x25 */ u8 timer;
                        } bytes;
                        /* 0x24 */ u8 slideState;
                    } slide;
                };
                /* 0x18 */ s16 cursorPositions[10];
            };
            /* 0x2C */ s16 targetY;
            /* 0x2E */ s16 targetX;
            union {
                struct {
                    /* 0x30 */ s16 pulseAlpha;
                    /* 0x32 */ s16 pulseTimer;
                } bytes;
                /* 0x30 */ s32 pulse;
            } prompt;
            /* 0x34 */ s16 spawnTimer;
            /* 0x36 */ s16 visibleCount;
            /* 0x38 */ u8 state;
        };
        struct {
            char pad18[6];
            /* 0x1E */ u16 counters[12];
        } sparkle;
        /* 0x18 */ s16 randomValues[15];
    };
};

extern void func_80071824(void *task, void (*callback)());
extern void func_800716E4(void *);
extern void func_800483FC(void *, void *, void *);
extern void *func_80071408(void *, s32, s32);
extern void func_8002C4E0(ShopMenuRowActor *);
extern void func_8002C9A0(ShopMenuWidgetActor *);
extern void func_8002FAB8(ShopMenuWidgetActor *);
extern void func_8002DF40(ShopMenuWidgetActor *);
extern void func_8002E32C(ShopMenuWidgetActor *);
extern void func_8002E468(ShopMenuWidgetActor *);
extern void func_8002CFAC(ShopMenuWidgetActor *);
extern void func_8002D294(ShopMenuWidgetActor *);
extern void func_8002D2E4(ShopMenuWidgetActor *);
extern void func_8002D558(ShopMenuWidgetActor *);
extern void func_8002D778(ShopMenuWidgetActor *);
extern void func_8002D9EC(ShopMenuWidgetActor *);
extern void func_8002DC14(ShopMenuWidgetActor *);
extern void func_8002F2C8(ShopMenuWidgetActor *);
extern void func_8002E9E4(ShopMenuWidgetActor *);
extern void func_8002EC5C(ShopMenuWidgetActor *);
extern void func_8002E5A4(ShopMenuWidgetActor *);
extern void func_8002FC00(ShopMenuWidgetActor *);
extern void func_8002CAD4(ShopMenuWidgetActor *);
extern void func_8002DE6C(ShopMenuWidgetActor *);
extern void func_8002E074(ShopMenuWidgetActor *);
extern void func_8002E214(ShopMenuWidgetActor *);
extern void func_8002E42C(ShopMenuWidgetActor *);
extern void func_8002E798(ShopMenuWidgetActor *);
extern void func_8001061C(s16, s16, s32, u16, s32, s32, s32, s32, s32, s32);
extern void func_8000F030(s16, s16, s32, s32, s32, s32, s32, s32);
extern void func_8000F8AC(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_80013D0C(s16, s16, void *, s32, s32);
extern void func_80013154(s32, s32, ShopDescriptionText, s32, s32, s32);
extern void func_8001BA2C(s32, s32, s32, s32);
extern int sprintf(char *, const char *, ...);
extern s32 func_80043040(s16);
extern s32 func_800430D0(void);
extern s32 D_800B34B0[];
extern u16 D_800B34E0[];
extern u16 D_800B34EC[];
extern ShopDescriptionText D_800B7A14[];
extern u8 D_800B7CD0;
extern u8 D_80121D86;
extern u8 D_80121D88;
extern s16 D_8011217E;
extern s16 D_8011214A;
extern s16 D_80112130[];
extern u8 D_800EC9E6;
extern CourseSelectStatus D_8010AF18;
extern s32 D_8010ADDC;
extern u8 D_8010AF40;
extern u8 D_8010AF70;
extern u8 D_8010AF71;
extern u8 D_8010AF72;
extern u8 D_8010AF73;
extern s16 D_8010AF76;
extern s16 D_8011217A;
extern ShopMenuState D_80121D80;
extern s32 D_80121D8C;
extern s32 D_801235B4;
extern MainMenuState *D_801235B8;
extern s16 D_800EC9D0;
extern u8 D_800ECA2F[];
extern s32 D_80123758;
extern u8 D_80124868;

const char D_800E0F60[] = "%6dG";
const char D_800E0F68[] = "%6dG";
const char D_800E0F70[] = "%5dG";
const char D_800E0F78[] = "%4dG";

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002C4E0.s")

void func_8002C624(ShopMenuRowActor *arg0) {
    s32 i;
    s32 moved;
    ShopMenuRowActor *spawnRow;
    s32 state;
    int stateByte;
    ShopMenuRowActor *row;
    ShopMenuRowActor *actor;

    stateByte = arg0->unk24;
    actor = arg0;
    state = stateByte;
    row = arg0;
    switch (state) {
    case 0:
        moved = 0;
        for (i = 0; i < row->unk26; i++) {
            if (row->unk18[i] < -0x7C) {
                row->unk18[i] += 0x10;
                moved++;
                if (row->unk18[i] >= -0x7C) {
                    row->unk18[i] = -0x7C;
                }
            }
        }
        row->unk25++;
        spawnRow = row;
        if (!(spawnRow->unk25 & 1)) {
            if (spawnRow->unk26 < 3) {
                spawnRow->unk26++;
            }
        }
        if (moved == 0) {
            spawnRow->unk24 = 1;
            func_80071408(func_8002D294, 0, 0x5F);
            func_80071408(func_8002DC14, 0, 0x61);
        }
        state = arg0->unk24;
        break;
    case 1:
        if (D_80121D88 == 1) {
            state = (u8) (arg0->unk24 = 2);
        }
        break;
    case 2:
        for (i = 0; i < 5; i++) {
            arg0->unk18[i] -= 0x20;
        }
        if (arg0->unk18[0] < -0x103) {
            arg0->unk24 = 3;
        }
        break;
    case 3:
        break;
    }

    if (arg0->unk24 == 3) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002C4E0, actor);
}

void func_8002C800(ShopMenuRowActor *arg0) {
    s32 i;

    for (i = 0; i < 5; i++) { arg0->unk18[i] = -0x104; }

    arg0->unk22 = -0x50;
    arg0->unk25 = 0;
    arg0->unk26 = 1;
    arg0->unk24 = 0;

    func_80071824(arg0, func_8002C624);
}

void func_8002C860(ShopMenuWidgetActor *arg0) {
    func_8000F030(arg0->x, arg0->y, func_80043040(D_8011217A), 3, 0x20, 0x20, 0, 0);
    func_8000F030((s16) (arg0->x + 0x40), arg0->y, func_80043040(D_8011217A), 4, 0x20, 0x20, 0, 0);
    func_8000F030(arg0->x, (s16) (arg0->y + 0x40), func_80043040(D_8011217A), 5, 0x20, 0x20, 0, 0);
    func_8000F030((s16) (arg0->x + 0x40), (s16) (arg0->y + 0x40), func_80043040(D_8011217A), 6, 0x20, 0x20, 0, 0);
}

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

void func_8002CFAC(ShopMenuWidgetActor *arg0) {
    int state;

    if ((D_800EC9E6 >= (u16) arg0->item.counter) && (arg0->item.bytes.subState != 0) && (arg0->y != -0x48)) {
        state = arg0->item.bytes.subState = 2;
    } else {
        state = arg0->item.bytes.subState;
        if ((D_800EC9E6 < (u16) arg0->item.counter) && (state != 0) && (arg0->y != -0x140)) {
            state = arg0->item.bytes.subState = 1;
        } else {
            state = arg0->item.bytes.subState;
            if ((state != 0) && (state < 4)) {
                state = arg0->item.bytes.subState = 3;
            }
        }
    }

    switch (state) {
    case 0:
        arg0->x -= 0x20;
        if (arg0->item.bytes.subTimer == 0) {
            func_80071408(func_8002CAA0, 0, 0x63);
        }
        arg0->item.bytes.subTimer++;
        if (arg0->x < -7) {
            arg0->x = -8;
            arg0->item.bytes.subState = 3;
            func_80071408(func_8002E074, 0, 0x64);
            D_8010ADDC = (s32) func_80071408(func_8002DE6C, 0, 0x64);
            func_80071408(func_8002E214, 0, 0x64);
            func_80071408(func_8002E798, 0, 0x60);
            func_80071408(func_8002E42C, 0, 0x64);
        }
        state = arg0->item.bytes.subState;
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->item.bytes.subState = 3;
        }
        state = arg0->item.bytes.subState;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->item.bytes.subState = 3;
        }
        state = arg0->item.bytes.subState;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->item.bytes.subState = 4;
        }
        state = arg0->item.bytes.subState;
        break;
    case 4:
        arg0->x += 0x20;
        if (arg0->x >= 0xA0) {
            arg0->item.bytes.subState = 5;
        }
        state = arg0->item.bytes.subState;
        break;
    case 5:
        D_80121D88 = 2;
        state = arg0->item.bytes.subState;
        break;
    }

    if ((state == 5) && (D_80121D88 == 2)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002CAD4, arg0);
}

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

void func_8002D558(ShopMenuWidgetActor *arg0) {
    int state;

    if ((D_800EC9E6 >= (u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->item.bytes.state = 2;
    } else if ((D_800EC9E6 < (u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->item.bytes.state = 1;
    } else {
        state = arg0->item.bytes.state;
        if (state < 4) {
            state = arg0->item.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
    case 5:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->item.bytes.state = 3;
        }
        state = arg0->item.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->item.bytes.state = 3;
        }
        state = arg0->item.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            if (arg0->y == -0x140) {
                arg0->item.bytes.state = 5;
            } else {
                arg0->item.bytes.state = 4;
            }
        }
        state = arg0->item.bytes.state;
        break;
    case 4:
        arg0->x += 0x20;
        if (arg0->x >= 0xA0) {
            arg0->item.bytes.state = 5;
        }
        state = arg0->item.bytes.state;
        break;
    }

    if (state == 5) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002D2E4, arg0);
}

void func_8002D734(ShopMenuWidgetActor *arg0) {
    arg0->x = -8;
    arg0->y = -0x140;
    arg0->sprite.index = 1;
    arg0->item.bytes.timer = 0;
    arg0->item.bytes.state = 0;
    func_80071824(arg0, func_8002D558);
}

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002D778.s")

void func_8002D9EC(ShopMenuWidgetActor *arg0) {
    u8 *stateField;
    int state;

    stateField = &arg0->item.bytes.state;

    if ((D_800EC9E6 == 2) && (arg0->y != -0x48) && (arg0->item.bytes.state < 6)) {
        state = arg0->item.bytes.state = 2;
    } else {
        state = arg0->item.bytes.state;
        if ((D_800EC9E6 != 2) && (arg0->y != -0x140) && (state < 6)) {
            state = arg0->item.bytes.state = 1;
        } else if (state < 4) {
            state = arg0->item.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
    case 4:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->item.bytes.state = 3;
        }
        state = arg0->item.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->item.bytes.state = 6;
            arg0->transition.counter = 8;
        }
        state = arg0->item.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->item.bytes.state = 5;
        }
        state = arg0->item.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    case 6:
        arg0->y -= arg0->transition.counter;
        arg0->item.bytes.state = 7;
        state = arg0->item.bytes.state;
        break;
    case 7:
        arg0->y += arg0->transition.counter;
        arg0->transition.counter = arg0->transition.counter / 2;
        if (arg0->transition.counter == 0) {
            arg0->item.bytes.state = 3;
        } else {
            arg0->item.bytes.state = 6;
        }
        state = arg0->item.bytes.state;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002D778, arg0);
}

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

void func_8002DCE8(ShopMenuWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = D_8010AF18.unk28)) {
        arg0->transition.bytes.state = globalState;
        /* Preserve IDO's state/globalState register allocation. */
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        state = globalState;
        arg0->sprite.index = D_8010AF18.unk2A;
        arg0->transition.bytes.timer = D_8010AF18.unk2C;
    }

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
        if ((s32)arg0->transition.bytes.timer < 0x10) {
            arg0->sprite.index -= 9;
        } else {
            arg0->sprite.index += 9;
        }
        state = arg0->transition.bytes.state;
        arg0->transition.bytes.timer = (arg0->transition.bytes.timer + 1) & 0x1F;
        break;
    case 2:
        if (D_80121D88 == 1) {
            state = arg0->transition.bytes.state = 3;
        }
        break;
    case 3:
        arg0->x -= 0x20;
        if (arg0->x < -0xEF) {
            arg0->transition.bytes.state = 4;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
    case 5:
        break;
    }

    D_8010AF40 = state;
    if (arg0->transition.bytes.state == 4) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002DC58, arg0);
}

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

void func_8002DF40(ShopMenuWidgetActor *arg0) {
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
        arg0->x -= 0x20;
        if (arg0->x < -0xFF) {
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        break;
    }
    if ((unsigned int)state == 3) {
        func_800716E4(arg0);
        if (D_8010AF70 == 0) {
            D_8010AF70 = 1;
        }
        if (D_8010AF71 == 1) {
            D_8010AF71 = 0;
        }
        return;
    }
    func_800483FC(&D_80124868, func_8002DEAC, arg0);
}

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

void func_8002E32C(ShopMenuWidgetActor *arg0) {
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
    func_800483FC(&D_80124868, func_8002E250, arg0);
}

void func_8002E42C(ShopMenuWidgetActor *arg0) {
    arg0->x = 0x30;
    arg0->y = 0x40;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8002E32C);
}

void func_8002E468(ShopMenuWidgetActor *arg0) {
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
        if (D_80121D88 == 3) {
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
    func_800483FC(&D_80124868, func_8002E250, arg0);
}

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

void func_8002E810(ShopMenuWidgetActor *arg0) {
    s16 new_var;
    s32 temp;
    char sp4C[4];
    u16 sp4A;
    u16 sp48;
    s32 palette;

    if ((s8)D_800ECA2F[D_80121D86] == -1) {
        new_var = 0x3000;
        temp = D_800B34B0[D_80121D86];
        if ((u32)temp >= 0x186A0U) {
            sp4A = 0x4000;
            sp48 = 0;
            sprintf(sp4C, D_800E0F68, arg0->item.price);
        } else if ((u32)temp >= 0x2710U) {
            sp4A = 0x3800;
            sp48 = 4;
            sprintf(sp4C, D_800E0F70, arg0->item.price);
        } else {
            sp4A = new_var;
            sp48 = 8;
            sprintf(sp4C, D_800E0F78, arg0->item.price);
        }

        func_8001BA2C((s16)(arg0->x + sp48), arg0->y, sp4A, 0x2000);

        if ((u32)D_80121D8C < (u32)D_800B34B0[D_80121D86]) {
            palette = 1;
        } else {
            palette = 0;
        }
        if (arg0->slide.slideState == 2) {
            palette = 0;
        }

        func_80013D0C((s16)(arg0->x + sp48 + 4), (s16)(arg0->y + 4), sp4C, palette & 0xFF, arg0->sprite.index);
        return;
    }

    func_8000F8AC(arg0->x, arg0->y, func_80043040(D_8011217E), 6, 0x20, 0x20, 0, arg0->sprite.index, 0);
}

// func_8002E9E4 best match: 94.119% (nonmatchings/func_8002E9E4-1404502880690620360/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002E9E4.s")

#ifdef NON_MATCHING
void func_8002E9E4(ShopMenuWidgetActor *arg0) {
    s32 amount;
    s32 price;

    switch (arg0->slide.bytes.state) {
    case 0:
        arg0->sprite.index += 0x26;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->slide.bytes.state = 1;
        }
        break;
    case 1:
        arg0->item.price = D_800B34B0[D_80121D80.selectedShopItem];
        if (D_801235B8->shopItemPrice >= 2) {
            arg0->slide.bytes.state = 2;
        }
        break;
    case 2:
        price = arg0->item.price;
        if ((u32)price < 100U) {
            amount = price & 0xFFFF;
        } else {
            amount = 500;
            if (D_80123758 & 0x8000) {
                if ((u32)price < 5000U) {
                    amount = price & 0xFFFF;
                } else {
                    amount = 5000;
                }
            }
        }
        arg0->item.price = price - amount;
        D_80121D80.money -= amount;
        if (arg0->item.price == 0) {
            arg0->slide.bytes.state = 3;
            D_800ECA2F[D_80121D80.selectedShopItem] = 9;
        }
        break;
    case 3:
        arg0->slide.bytes.timer++;
        if (arg0->slide.bytes.timer >= 0x14) {
            arg0->slide.bytes.timer = 0;
            arg0->slide.bytes.state = 4;
            D_801235B8->shopItemPrice += 2;
        }
        break;
    case 4:
        if (D_80121D80.shopMenuState == 3) {
            arg0->slide.bytes.state = 5;
        }
        /* fallthrough */
    case 5:
        arg0->x += 0x20;
        if (arg0->x >= 0x94) {
            arg0->slide.bytes.state = 6;
        }
        break;
    }
    if (((D_800EC9D0 >= 5) && (D_800EC9D0 != 9)) || (arg0->slide.bytes.state == 6)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002E810, arg0);
}
#endif

void func_8002EC04(ShopMenuWidgetActor *arg0) {
    arg0->x = 0x20;
    arg0->y = -8;
    arg0->item.price = D_800B34B0[D_80121D86];
    arg0->sprite.index = 0;
    arg0->slide.bytes.state = 0;
    func_80071824(arg0, func_8002E9E4);
}

// func_8002EC5C best match: 92.575% (nonmatchings/func_8002EC5C-7387615772158234395/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002EC5C.s")

#ifdef NON_MATCHING
void func_8002EC5C(ShopMenuWidgetActor *arg0) {
    u16 alpha;

    func_8000F030(arg0->x, arg0->y, func_80043040(D_80112130[0x24]), 0, 0x20, 0x20, 0, 0);
    func_8000F030((s16)(arg0->x + 0x40), arg0->y, func_80043040(D_80112130[0x24]), 1, 0x20, 0x20, 0, 0);
    func_8000F030((s16)(arg0->x + 0x78), arg0->y, func_80043040(D_80112130[0x24]), 1, 0x20, 0x20, 0, 0);
    func_8000F030((s16)(arg0->x + 0xB0), arg0->y, func_80043040(D_80112130[0x24]), 2, 0x20, 0x20, 0, 0);
    func_80013154((s16)(arg0->x + 0x30), (s16)(arg0->y + 4), &D_800B7CD0, 0, 0x100, 0);

    alpha = 0x60;
    if (arg0->item.price == 0) {
        alpha = 0x100;
    }

    func_8000F8AC((s16)(arg0->x + 0x50), (s16)(arg0->y + 0x14), func_80043040(D_80112130[0x24]), 0x17, 0x20,
                  0x20, 0, alpha, 0);

    if (alpha == 0x100) {
        alpha = 0x60;
    } else {
        alpha = 0x100;
    }

    func_8000F8AC((s16)(arg0->x + 0x50), (s16)(arg0->y + 0x24), func_80043040(D_80112130[0x24]), 0x18, 0x20,
                  0x20, 0, alpha, 0);
    func_8000F8AC((s16)(arg0->x + 0x50), (s16)(arg0->y + (arg0->item.price * 0x10) + 0x14),
                  func_80043040(D_80112130[0x24]), 0x12, 0x20, 0x20, 0, arg0->sprite.index, 0);
}
#endif

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

void func_8002F854(ShopMenuWidgetActor *arg0) {
    s32 i;

    for (i = 0; i < 10; i++) {
        arg0->cursorPositions[i] = -0xFC;
    }

    arg0->targetX = -0x7C;
    arg0->targetY = -0x5C;
    arg0->prompt.bytes.pulseTimer = 0;
    arg0->spawnTimer = 0;
    arg0->visibleCount = 1;
    arg0->prompt.bytes.pulseAlpha = 0;
    arg0->state = 0;
    func_80071824(arg0, func_8002F2C8);
}

void func_8002F8DC(ShopMenuWidgetActor *arg0) {
    s32 unused;
    u16 tileIndex;

    if (D_8010AF72 < 7) {
        tileIndex = D_800B34E0[D_8010AF73 * 7 + D_8010AF72];
    } else {
        tileIndex = D_800B34EC[D_8010AF73 * 7];
    }

    func_8001061C(arg0->x, arg0->y, func_80043040(D_8011214A), tileIndex, 0, 0, 0x40, 0x1C, 0x20, 0x20);
    func_8001061C((s16)(arg0->x + 0x40), arg0->y, func_80043040(D_8011214A), tileIndex, 0, 0x1C, 0x40, 0x1C, 0x20,
                  0x20);
    func_8000F030((s16)(arg0->x - 4), (s16)(arg0->y - 1), func_80043040(D_8011217E), 0xA, 0x20, 0x20, 0, 0);
    func_8000F030((s16)(arg0->x + 0x3C), (s16)(arg0->y - 1), func_80043040(D_8011217E), 0xB, 0x20, 0x20, 1, 0);
}

void func_8002FAB8(ShopMenuWidgetActor *arg0) {
    u8 state;

    if ((D_801235B8->unk20 == 3) || (D_801235B8->unk20 == 9)) {
        arg0->sprite.bytes.state = 2;
    }
    state = arg0->sprite.bytes.state;
    switch (state) {
    case 0:
        arg0->x -= 0x20;
        if (arg0->x < 5) {
            arg0->x = 4;
            arg0->sprite.bytes.state = 1;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 1:
        break;
    case 2:
        arg0->x += 0x20;
        if (arg0->x >= 0x90) {
            arg0->sprite.bytes.state = 3;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 3:
        state = arg0->sprite.bytes.state = 4;
        break;
    }
    if ((u32)state == 4) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002F8DC, arg0);
}

void func_8002FBC8(ShopMenuWidgetActor *arg0) {
    arg0->x = 0x90;
    arg0->y = 0x44;
    arg0->sprite.bytes.state = 0;
    func_80071824(arg0, func_8002FAB8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/shop_menu_ui/func_8002FC00.s")

void func_8002FD70(ShopMenuWidgetActor *arg0) {
    s32 i;

    arg0->sprite.index = D_8010AF76 / 2;
    arg0->x++;
    arg0->y--;

    for (i = 0; i < 12; i += 4) {
        arg0->sparkle.counters[i] = (arg0->sparkle.counters[i] + 1) % 6;
        arg0->sparkle.counters[i + 1] = (arg0->sparkle.counters[i + 1] + 1) % 6;
        arg0->sparkle.counters[i + 2] = (arg0->sparkle.counters[i + 2] + 1) % 6;
        arg0->sparkle.counters[i + 3] = (arg0->sparkle.counters[i + 3] + 1) % 6;
    }

    if (arg0->sprite.index == 0) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002FC00, arg0);
}

void func_8002FEF8(ShopMenuWidgetActor *arg0) {
    ShopMenuWidgetActor *new_var;
    s32 i;

    arg0->x = 5;
    arg0->y = 0x48;
    new_var = arg0;
    new_var->sprite.index = D_8010AF76 / 2;

    for (i = 0; i < 12; i++) {
        new_var->randomValues[i + 3] = func_800430D0() % 6;
    }

    func_80071824(new_var, func_8002FD70);
}
