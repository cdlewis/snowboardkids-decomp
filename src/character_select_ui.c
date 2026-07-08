#include "common.h"

#define CHARACTER_SELECT_FRAME_TEXTURE_HANDLE (*(s16 *)&D_80112130[0x42])

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ union {
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
        /* 0x1E */ u16 alpha;
    } transition;
    union {
        struct {
            /* 0x20 */ u8 selectedCharacterRow;
            /* 0x21 */ u8 pad21;
        } bytes;
        /* 0x20 */ s16 counter;
    } selection;
    union {
        struct {
            /* 0x22 */ u8 subState;
            /* 0x23 */ u8 subTimer;
        } bytes;
        /* 0x22 */ s16 value;
    } row;
} CharacterSelectWidgetActor;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x[11];
    /* 0x2E */ s16 y[13];
    /* 0x48 */ u8 itemCount;
} CharacterSelectMenuFrameActor;

typedef u8 CharacterSelectText[0x94];

typedef union {
    u8 bytes[8];
    struct {
        /* 0x00 */ u8 state;
        /* 0x01 */ u8 otherState;
        /* 0x02 */ u8 pad2[2];
        /* 0x04 */ s16 spriteIndex;
        /* 0x06 */ u8 timer;
        /* 0x07 */ u8 otherTimer;
    } fields;
} CharacterSelectCursorState;

extern void func_80071824(void *task, void (*callback)());
extern void func_8000F030(s16, s16, s32, s32, s32, s32, s32, s32);
extern void func_8000F8AC(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_80013154(s32, s32, u8 *, s32, s32, s32);
extern s32 func_80043040(s16);
extern s16 D_80112172;
extern s16 D_80112178;
extern u8 D_800B6B88[];
extern CharacterSelectText D_800B6210[];
extern u8 D_800B67D8[][0x74];
extern u8 D_800B6934[][0x60];
extern u8 D_800B6A54[][0x70];
extern u8 D_800B6B34[];
extern s16 D_80121B50;
extern u8 D_80112130[];
extern s32 D_8010ADDC;
extern u16 D_8010AE80;
extern void *D_8010ADE0;
extern CharacterSelectCursorState D_8010AE88;
extern u8 D_8010AE88_state;
extern u8 D_8010AE89;
extern u8 D_8010AE8A;
extern u8 D_8010AE8F;
extern u8 D_8010ADF9;
extern u8 D_80121D88;
extern u8 D_800EC9C1;
extern s16 D_800EC9D0;
extern u8 D_800EC9DD;
extern u8 D_800EC9C2;
extern u8 D_80121B5E;
extern void *D_80124868;
extern void *func_80071408(void *, s32, s32);
extern void func_800716E4(void *);
extern void func_800483FC(void *, void *, void *);
extern u8 D_80121B55;
extern u8 D_80121D80[];
extern s32 D_801235B4;
extern void func_80021410(void *);
extern void func_8001DB0C(CharacterSelectWidgetActor *);
extern void func_8001DFE4(CharacterSelectWidgetActor *);
extern void func_8001E4AC(CharacterSelectWidgetActor *);
extern void func_8001E974(CharacterSelectWidgetActor *);
extern void func_8001EE3C(CharacterSelectWidgetActor *);
extern void func_8001F304(CharacterSelectWidgetActor *);

void func_8002172C(CharacterSelectWidgetActor *arg0);
void func_800218A4(CharacterSelectWidgetActor *arg0);
void func_800219E4(CharacterSelectWidgetActor *arg0);
void func_80021B58(CharacterSelectWidgetActor *arg0);
void func_80021C98(CharacterSelectWidgetActor *arg0);
void func_80023198(CharacterSelectWidgetActor *arg0);
void func_800227D8(CharacterSelectWidgetActor *arg0);
void func_8002262C(CharacterSelectWidgetActor *arg0);
void func_80023434(CharacterSelectWidgetActor *arg0);
void func_80020DEC(CharacterSelectWidgetActor *arg0);
void func_80020CEC(CharacterSelectWidgetActor *arg0);
void func_80020F80(CharacterSelectWidgetActor *arg0);
void func_80020F44(CharacterSelectWidgetActor *arg0);
void func_8002127C(CharacterSelectWidgetActor *arg0);
void func_800213D4(CharacterSelectWidgetActor *arg0);
void func_80021F80(CharacterSelectWidgetActor *arg0);
void func_8002215C(CharacterSelectWidgetActor *arg0);
void func_80022198(CharacterSelectWidgetActor *arg0);
void func_80022274(CharacterSelectWidgetActor *arg0);
void func_8001D7B8(CharacterSelectWidgetActor *arg0);
void func_8001DD80(CharacterSelectWidgetActor *arg0);
void func_8001E258(CharacterSelectWidgetActor *arg0);
void func_8001E720(CharacterSelectWidgetActor *arg0);
void func_8001EBE8(CharacterSelectWidgetActor *arg0);
void func_8001F0B0(CharacterSelectWidgetActor *arg0);
void func_8001F578(CharacterSelectWidgetActor *arg0);
void func_8001F7CC(CharacterSelectWidgetActor *arg0);
void func_8001FA40(CharacterSelectWidgetActor *arg0);
void func_8001FC94(CharacterSelectWidgetActor *arg0);
void func_8001FF08(CharacterSelectWidgetActor *arg0);
void func_8002015C(CharacterSelectWidgetActor *arg0);
void func_80020624(CharacterSelectWidgetActor *arg0);
void func_800203D0(CharacterSelectWidgetActor *arg0);
void func_80020818(CharacterSelectWidgetActor *arg0);
void func_80022464(CharacterSelectWidgetActor *arg0);
void func_80020B70(CharacterSelectWidgetActor *arg0);

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001BD70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001C158.s")

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001C83C.s")

// func_8001C96C best match: 94.533%
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001C96C.s")

#ifdef NON_MATCHING
void func_8001C96C(CharacterSelectMenuFrameActor *arg0) {
    s32 lastArrowIndex;
    s32 i;
    s32 alpha;
    u16 tile;

    if (D_80121B5E < 2) {
        lastArrowIndex = 2;
    } else {
        lastArrowIndex = 0;
    }
    i = 0;
    lastArrowIndex += 1;
    if (arg0->itemCount > 0) {
        do {
            alpha = 0;
            if (i == 0) {
                tile = 0x1C;
                if (D_80121B5E == 2) {
                    tile = 0x1D;
                }
                if ((i == D_80121B50) && (D_800EC9C1 > 0) && (D_800EC9C1 < 8) && (D_800EC9C1 & 1)) {
                    alpha = 0xFF;
                }
                func_8000F030(arg0->x[i], arg0->y[i], func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), tile, 0x20, 0x20, 0, alpha);
            } else if (i == lastArrowIndex) {
                if ((i == D_80121B50) && (D_800EC9C1 > 0) && (D_800EC9C1 < 8) && (D_800EC9C1 & 1)) {
                    alpha = 0xFF;
                }
                func_8000F030(arg0->x[i], arg0->y[i], func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), 0x20, 0x20, 0x20, 0, alpha);
            } else {
                if ((i == D_80121B50) && (D_800EC9C1 > 0) && (D_800EC9C1 < 8) && (D_800EC9C1 & 1)) {
                    alpha = 0xFF;
                }
                func_8000F030(arg0->x[i], arg0->y[i], func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), (i + 0x12) & 0xFFFF, 0x20, 0x20, 0, alpha);
            }

            if (i != lastArrowIndex) {
                func_8000F030((s16)(arg0->x[i] - 0x10), arg0->y[i], func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), (i + 0x29) & 0xFFFF, 0x20, 0x20, 0, 0);
            }
            i++;
        } while (i < arg0->itemCount);
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001CC10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001D254.s")

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001D2F0.s")

// func_8001D7B8 best match: 91.918%
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001D7B8.s")

#ifdef NON_MATCHING
void func_8001D7B8(CharacterSelectWidgetActor *arg0) {
    int state;
    int stateValue;
    s32 selectedIndex;
    u16 spriteIndex;

    selectedIndex = D_80121B50;
    spriteIndex = (u16)arg0->sprite.index;
    if ((selectedIndex >= (s32)spriteIndex) && (arg0->transition.bytes.state != 0) && (arg0->y != -0x48)) {
        stateValue = 2;
        state = stateValue & 0xFF;
        arg0->transition.bytes.state = stateValue;
    } else {
        state = arg0->transition.bytes.state;
        if ((selectedIndex < (s32)spriteIndex) && (state != 0) && (arg0->y != -0x140)) {
            stateValue = 1;
            state = stateValue & 0xFF;
            arg0->transition.bytes.state = stateValue;
        } else if ((state != 0) && (state < 4)) {
            stateValue = 3;
            state = stateValue & 0xFF;
            arg0->transition.bytes.state = stateValue;
        }
    }

    if (state != D_8010AE8A) {
        state = D_8010AE8A & 0xFF;
        arg0->transition.bytes.state = D_8010AE8A;
    }

    switch (state) {
    case 0:
        arg0->x -= 0x26;
        if (arg0->x < -7) {
            arg0->x = -8;
            arg0->transition.bytes.state = 3;
            D_8010ADDC = (s32)func_80071408(func_80020CEC, 0, 0x64);
            func_80071408(func_80020F44, 0, 0x62);
            func_80071408(func_800213D4, 0, 0x62);
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        state = D_80121D80[8];
        if (state == 1) {
            arg0->transition.bytes.state = 4;
            state = D_80121D88;
        }
        if (state == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        state = D_80121D80[8];
        if (state == 5) {
            arg0->transition.bytes.state = 3;
            D_80121D80[8] = 6;
            state = 6 & 0xFF;
        }
        if (state == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        if (arg0->x >= 0x94) {
            arg0->transition.bytes.state = 6;
        }
        state = arg0->transition.bytes.state;
        break;
    case 6:
        arg0->transition.bytes.state = 7;
        D_80121D80[8] = 8;
        state = arg0->transition.bytes.state;
        break;
    }

    D_8010AE8A = state;
    if (arg0->transition.bytes.state == 7) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001D2F0, arg0);
}
#endif

void func_8001DACC(CharacterSelectWidgetActor *arg0) {
    arg0->x = 0x96;
    arg0->y = -0x48;
    arg0->sprite.index = 0;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001D7B8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001DB0C.s")

void func_8001DD80(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((D_80121B50 >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B50 < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    case 6:
        state = arg0->transition.bytes.state = 7;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001DB0C, arg0);
}

void func_8001DFA0(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x1;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001DD80);
}

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001DFE4.s")

void func_8001E258(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((D_80121B50 >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B50 < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001DFE4, arg0);
}

void func_8001E468(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x2;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001E258);
}

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001E4AC.s")

void func_8001E720(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((D_80121B50 >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B50 < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001E4AC, arg0);
}

void func_8001E930(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x3;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001E720);
}

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001E974.s")

void func_8001EBE8(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((D_80121B50 >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B50 < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001E974, arg0);
}

void func_8001EDF8(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x4;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001EBE8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001EE3C.s")

void func_8001F0B0(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((D_80121B50 >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B50 < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001EE3C, arg0);
}

void func_8001F2C0(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x5;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001F0B0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001F304.s")

void func_8001F578(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((D_80121B50 >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B50 < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001F304, arg0);
}

void func_8001F788(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x6;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001F578);
}

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001F7CC.s")

void func_8001FA40(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((D_80121B50 >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B50 < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001F7CC, arg0);
}

void func_8001FC50(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x7;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001FA40);
}

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001FC94.s")

void func_8001FF08(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((D_80121B50 >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B50 < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001FC94, arg0);
}

void func_80020118(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x8;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001FF08);
}

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8002015C.s")

void func_800203D0(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((D_80121B50 >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B50 < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002015C, arg0);
}

void func_800205E0(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x9;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_800203D0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_80020624.s")

void func_80020818(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->row.bytes.subState;

    if ((D_80121B50 == D_8010AE80) && (arg0->y != -0x48) && ((s32) arg0->row.bytes.subState < 6)) {
        state = arg0->row.bytes.subState = 2;
    } else {
        state = (s32) arg0->row.bytes.subState;
        if ((D_80121B50 != D_8010AE80) && (arg0->y != -0x140) && (state < 6)) {
            state = arg0->row.bytes.subState = 1;
        } else {
            state = arg0->row.bytes.subState;
            if (state < 4) {
                state = arg0->row.bytes.subState = 3;
            }
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->row.bytes.subState = 3;
        }
        state = arg0->row.bytes.subState;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->row.bytes.subState = 6;
            arg0->transition.alpha = 8;
        }
        state = arg0->row.bytes.subState;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->row.bytes.subState = 4;
        }
        if (D_80121D88 == 7) {
            arg0->row.bytes.subState = 5;
        }
        state = arg0->row.bytes.subState;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->row.bytes.subState = 3;
        }
        if (D_80121D88 == 7) {
            arg0->row.bytes.subState = 5;
        }
        state = arg0->row.bytes.subState;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    case 6:
        arg0->y -= arg0->transition.alpha;
        state = arg0->row.bytes.subState = 7;
        break;
    case 7:
        arg0->y += arg0->transition.alpha;
        arg0->transition.alpha /= 2;
        if (arg0->transition.alpha == 0) {
            arg0->row.bytes.subState = 3;
        } else {
            arg0->row.bytes.subState = 6;
        }
        state = arg0->row.bytes.subState;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_80020624, arg0);
}

void func_80020AA0(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0;
    arg0->row.bytes.subTimer = 0;
    arg0->row.bytes.subState = 0;
    func_80071824(arg0, func_80020818);
}

void func_80020AE0(CharacterSelectWidgetActor *arg0) {
    if (arg0->transition.bytes.state < 3) {
        func_8000F8AC(
            arg0->x,
            (s16)(arg0->y + D_80121B50 * arg0->selection.bytes.selectedCharacterRow),
            func_80043040(D_80112172),
            0x12,
            0x20,
            0x20,
            0,
            arg0->sprite.index,
            0);
    }
}

void func_80020B70(CharacterSelectWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = D_8010AE88.fields.state)) {
        arg0->transition.bytes.state = globalState;
        state = globalState;
        arg0->sprite.index = D_8010AE88.fields.spriteIndex;
        arg0->transition.bytes.timer = D_8010AE88.fields.timer;
    }

    /* Preserve IDO's state selector register allocation. */
    switch (state ^ 0) {
    case 0:
        arg0->sprite.index += 0x26;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 1;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        if ((s32) arg0->transition.bytes.timer < 0x10) {
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
        break;
    }

    D_8010AE88_state = state;
    if (D_80121D88 == 7) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_80020AE0, arg0);
}

void func_80020CEC(CharacterSelectWidgetActor *arg0) {
    u8 selectedCharacterRow;
    s16 y;
    int mode;

    mode = D_8010ADF9;
    if (mode == 0) {
        if (1) {}
        selectedCharacterRow = 0x18;
        y = -0x50;
    } else if (mode == 1) {
        y = -0x54;
        selectedCharacterRow = 0x16;
    } else if (mode == 2) {
        selectedCharacterRow = 0x16;
        y = -0x60;
    } else {
        y = -0x60;
        selectedCharacterRow = 0x13;
    }

    if (D_800EC9DD != 0) {
        y = -0x58;
        selectedCharacterRow = 0x18;
    }

    arg0->x = -0x7C;
    arg0->y = y;
    arg0->selection.bytes.selectedCharacterRow = selectedCharacterRow;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    arg0->transition.bytes.timer = 0;
    func_80071824(arg0, func_80020B70);
}

void func_80020D88(CharacterSelectWidgetActor *arg0) {
    func_8000F8AC(arg0->x, arg0->y, func_80043040(D_80112172), 1, 0x20, 0x20, 0, arg0->sprite.index, 0);
}

void func_80020DEC(CharacterSelectWidgetActor *arg0) {
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
            arg0->transition.bytes.state = 2;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 1;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        arg0->x += 0x20;
        if (arg0->x >= 0x94) {
            arg0->transition.bytes.state = 4;
        }
        state = arg0->transition.bytes.state;
        break;
    }
    if ((unsigned int)state == 4) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_80020D88, arg0);
}

void func_80020F44(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x5C;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_80020DEC);
}

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_80020F80.s")

void func_8002127C(CharacterSelectWidgetActor *arg0) {
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
            arg0->transition.bytes.state = 2;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 1;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        arg0->x += 0x20;
        if (arg0->x >= 0x94) {
            arg0->transition.bytes.state = 4;
        }
        state = arg0->transition.bytes.state;
        break;
    }
    if ((unsigned int)state == 4) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_80020F80, arg0);
}

void func_800213D4(CharacterSelectWidgetActor *arg0) {
    arg0->x = 0x48;
    arg0->y = 0x34;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8002127C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_80021410.s")

void func_8002172C(CharacterSelectWidgetActor *arg0) {
    u8 state = arg0->sprite.bytes.state;
    s16 target;

    switch (state) {
    case 0:
        if (D_80121B55 == 1) {
            target = -0x78;
        } else {
            target = -0x80;
        }
        arg0->x += 0x20;
        if (arg0->x >= target) {
            arg0->x = target;
            arg0->sprite.bytes.state = 1;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 1:
        if ((D_80121D88 == 3) || (D_80121D88 == 7)) {
            state = arg0->sprite.bytes.state = 2;
        }
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0x107) {
            arg0->sprite.bytes.state = 3;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 3:
        break;
    }
    if ((unsigned int)state == 3) {
        func_800716E4(arg0);
        if (D_80121D80[8] == 3) {
            D_80121D80[8] = 4;
        }
    } else {
        func_800483FC(&D_80124868, func_80021410, arg0);
    }
}

void func_8002186C(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = -0x50;
    arg0->sprite.bytes.state = 0;
    func_80071824(arg0, func_8002172C);
}

void func_800218A4(CharacterSelectWidgetActor *arg0) {
    func_8000F030(arg0->x, arg0->y, func_80043040(D_80112172), 3, 0x20, 0x20, 0, 0);
    func_8000F030((s16) (arg0->x + 0x40), arg0->y, func_80043040(D_80112172), 4, 0x20, 0x20, 0, 0);
    func_8000F030(arg0->x, (s16) (arg0->y + 0x40), func_80043040(D_80112172), 5, 0x20, 0x20, 0, 0);
    func_8000F030((s16) (arg0->x + 0x40), (s16) (arg0->y + 0x40), func_80043040(D_80112172), 6, 0x20,
                  0x20, 0, 0);
}

void func_800219E4(CharacterSelectWidgetActor *arg0) {
    u8 state = arg0->sprite.bytes.state;

    switch (state) {
    case 0:
        arg0->x += 0x20;
        if (arg0->x >= -0x88) {
            arg0->x = -0x88;
            arg0->sprite.bytes.state = 1;
            D_8010ADE0 = func_80071408(func_8002215C, 0, 0x63);
        }
        state = arg0->sprite.bytes.state;
        break;
    case 1:
        if ((D_80121D88 == 3) || (D_80121D88 == 7)) {
            state = arg0->sprite.bytes.state = 2;
        }
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0x107) {
            arg0->sprite.bytes.state = 3;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 3:
        state = arg0->sprite.bytes.state = 4;
        break;
    }
    state = arg0->sprite.bytes.state;
    if (state == 4) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_800218A4, arg0);
}

void func_80021B20(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = -0x18;
    arg0->sprite.bytes.state = 0;
    func_80071824(arg0, func_800219E4);
}

void func_80021B58(CharacterSelectWidgetActor *arg0) {
    func_8000F030(arg0->x, arg0->y, func_80043040(D_80112172), 3, 0x20, 0x20, 0, 0);
    func_8000F030((s16) (arg0->x + 0x40), arg0->y, func_80043040(D_80112172), 4, 0x20, 0x20, 0, 0);
    func_8000F030(arg0->x, (s16) (arg0->y + 0x40), func_80043040(D_80112172), 5, 0x20, 0x20, 0, 0);
    func_8000F030((s16) (arg0->x + 0x40), (s16) (arg0->y + 0x40), func_80043040(D_80112172), 6, 0x20, 0x20, 0, 0);
}

void func_80021C98(CharacterSelectWidgetActor *arg0) {
    u32 state = arg0->sprite.bytes.state;

    switch (state) {
    case 0:
        arg0->x += 0x20;
        if (arg0->x >= -0x88) {
            arg0->x = -0x88;
            arg0->sprite.bytes.state = 1;
            func_80071408(func_80022464, 0, 0x63);
        }
        state = arg0->sprite.bytes.state;
        break;
    case 1:
        if (D_80121D88 == 7) {
            arg0->sprite.bytes.state = 5;
        }
        if (D_80121D88 == 1) {
            arg0->sprite.bytes.state = 2;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0x127) {
            arg0->sprite.bytes.state = 3;
            arg0->x = -0x128;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 3:
        if (D_80121D88 == 5) {
            state = arg0->sprite.bytes.state = 4;
        }
        break;
    case 4:
        arg0->x += 0x20;
        if (arg0->x >= -0x88) {
            arg0->x = -0x88;
            arg0->sprite.bytes.state = 1;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 5:
        arg0->x -= 0x20;
        if (arg0->x < -0x107) {
            arg0->sprite.bytes.state = 6;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 6:
        state = arg0->sprite.bytes.state = 7;
        break;
    }
    if (state == 7) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_80021B58, arg0);
}

void func_80021E70(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x128;
    arg0->y = 0x8;
    arg0->sprite.bytes.state = 0;
    func_80071824(arg0, func_80021C98);
}

void func_80021EA8(CharacterSelectWidgetActor *arg0) {
    CharacterSelectText *texts;
    u8 *text;

    texts = D_800B6210;
    text = texts[D_80121B50];
    func_80013154(arg0->x, arg0->y, text, 1, arg0->sprite.index, 0);
    if (arg0->transition.bytes.state == 2) {
        func_8000F030(
            (s16)(arg0->x + 0x60),
            (s16)(arg0->y + 0x40),
            func_80043040(D_80112178),
            (((s32)arg0->transition.bytes.timer >= 8) + 5) & 0xFFFF,
            0x20,
            0x20,
            0,
            0);
    }
}

void func_80021F80(CharacterSelectWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = D_8010AE88.bytes[1])) {
        arg0->transition.bytes.state = globalState;
        state = globalState;
        arg0->transition.bytes.timer = D_8010AE88.bytes[7];
    }

    switch (state) {
    case 0:
        if ((++arg0->transition.bytes.timer) == 2) {
            arg0->transition.bytes.state = 1;
            arg0->transition.bytes.timer = state * 0;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        arg0->sprite.index += 0x30;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 2;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->transition.bytes.timer = (arg0->transition.bytes.timer + 1) & 0xF;
        if (D_80121D88 == 3) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        state = arg0->transition.bytes.state;
        arg0->transition.bytes.timer = 0;
        break;
    case 4:
        arg0->transition.bytes.timer++;
        if ((D_80121D88 == 3) || (D_80121D88 == 7)) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x -= 0x20;
        if (arg0->x < -0xFF) {
            arg0->transition.bytes.state = 6;
        }
        state = arg0->transition.bytes.state;
        break;
    case 6:
        break;
    }

    D_8010AE89 = state;
    D_8010AE8F = arg0->transition.bytes.timer;

    if (arg0->transition.bytes.state == 6) {
        func_800716E4(arg0);
        D_8010ADE0 = NULL;
        D_8010AE88.bytes[1] = 0;
        D_8010AE88.bytes[7] = 0;
        return;
    }
    func_800483FC(&D_80124868, func_80021EA8, arg0);
}

void func_8002215C(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = -0x14;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_80021F80);
}

void func_80022198(CharacterSelectWidgetActor *arg0) {
    u8 *text;
    s32 threshold;
    u16 idx;

    if (D_80121B5E < 2) {
        threshold = 2;
    } else {
        threshold = 0;
    }
    if (threshold >= D_80121B50) {
        idx = (u16)D_80121B50;
        if (D_80121B5E == 0) {
            text = D_800B67D8[idx];
        } else if (D_80121B5E == 1) {
            text = D_800B6934[idx];
        } else {
            text = D_800B6A54[idx];
        }
    } else {
        text = D_800B6B34;
    }
    func_80013154(arg0->x, arg0->y, text, 1, arg0->sprite.index, 0);
}

void func_80022274(CharacterSelectWidgetActor *arg0) {
    u32 state = arg0->transition.bytes.state;

    switch (state) {
    case 0:
        if (++arg0->transition.bytes.timer == 2) {
            arg0->transition.bytes.state = 1;
            arg0->transition.bytes.timer = 0;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        arg0->sprite.index += 0x30;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 2;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        if ((D_80121D88 == 7) || (D_80121D88 == 1)) {
            state = arg0->transition.bytes.state = 3;
        }
        break;
    case 3:
        arg0->x -= 0x20;
        if (arg0->x < -0x11F) {
            if (D_80121D88 == 7) {
                arg0->transition.bytes.state = 7;
            } else {
                arg0->transition.bytes.state = 4;
            }
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            state = arg0->transition.bytes.state = 5;
        }
        break;
    case 5:
        arg0->x += 0x20;
        if (arg0->x >= -0x84) {
            arg0->transition.bytes.state = 2;
        }
        state = arg0->transition.bytes.state;
        break;
    case 6:
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 0) {
            arg0->transition.bytes.state = 2;
        }
        state = arg0->transition.bytes.state;
        break;
    case 7:
        state = arg0->transition.bytes.state = 8;
        break;
    }
    if (state == 8) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_80022198, arg0);
}

void func_80022464(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = 0xC;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_80022274);
}

void func_800224A0(CharacterSelectWidgetActor *arg0) {
    u32 drawAlpha;
    int isEvenState;
    u16 alpha;
    s16 state;

    state = D_800EC9D0;
    if (state != 0) {
        isEvenState = !(state & 1);
        if (isEvenState) {
            alpha = 0x100;
        } else {
            alpha = 0x60;
        }

        drawAlpha = alpha;
        func_8000F8AC(arg0->x, arg0->sprite.index, func_80043040(D_80112178), 0x18, 0x20, 0x20, 0, drawAlpha, 0);

        if (drawAlpha == 0x100) {
            alpha = 0x60;
        } else {
            alpha = 0x100;
        }

        func_8000F8AC(arg0->x, arg0->y, func_80043040(D_80112178), 0x17, 0x20, 0x20, 0, alpha, 0);

        state = D_800EC9D0;
        if ((state == 3) || (state == 4)) {
            func_8000F8AC(arg0->x, (s16)(((D_800EC9D0 * 0x10) + arg0->y) - 0x30), func_80043040(D_80112178), 0x12,
                          0x20, 0x20, 0, (u16)arg0->selection.counter, 0);
            state = D_800EC9D0;
        }
    }
    if ((state >= 5) && (((s16)arg0->transition.alpha) == 0)) {
        func_800716E4(arg0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8002262C.s")

void func_800227A0(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x74;
    arg0->y = 0x2F;
    arg0->sprite.index = 0x2F;
    func_80071824(arg0, func_8002262C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_800227D8.s")

void func_80023198(CharacterSelectWidgetActor *arg0) {
    u8 state = arg0->sprite.bytes.state;
    u8 unk;
    s16 target;

    switch (state) {
    case 0:
        unk = D_80121B5E;
        if ((D_800EC9C2 == 2) || ((D_800EC9C2 == 1) && (unk == 0))) {
            target = -0x8A;
        } else {
            target = -0x88;
        }
        arg0->x += 0x20;
        if (arg0->x >= target) {
            arg0->x = target;
            arg0->sprite.bytes.state = 1;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 1:
        if ((D_80121D88 == 3) || (D_80121D88 == 7)) {
            state = arg0->sprite.bytes.state = 2;
        }
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0xEF) {
            arg0->x = -0xF0;
            arg0->sprite.bytes.state = 3;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 3:
        state = arg0->sprite.bytes.state = 4;
        break;
    case 4:
        break;
    }
    state = arg0->sprite.bytes.state;
    if (state == 4) {
        func_800716E4(arg0);
        D_80121D88 = 4;
        return;
    }
    func_800483FC(&D_80124868, func_800227D8, arg0);
}

void func_800232F4(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0xF0;
    arg0->y = -0x40;
    arg0->sprite.bytes.state = 0;
    func_80071824(arg0, func_80023198);
}

void func_8002332C(CharacterSelectWidgetActor *arg0) {
    func_8000F030(arg0->x, arg0->y, func_80043040(D_80112172), 0x28, 0x20, 0x20, 0, 0);
    if ((s32) arg0->transition.bytes.state > 0) {
        func_80013154((s16) (arg0->x + 4), (s16) (arg0->y + 4), D_800B6B88, 1, 0x100, 0);
        func_8000F030(
            (s16) (arg0->x + 0x64),
            (s16) (arg0->y + 4),
            func_80043040(D_80112178),
            (u16) ((arg0->transition.bytes.timer >= 8) + 5),
            0x20,
            0x20,
            0,
            0);
    }
}

void func_80023434(CharacterSelectWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = D_8010AE88.bytes[1])) {
        arg0->transition.bytes.state = globalState;
        /* Preserve IDO's state/globalState register allocation. */
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        state = globalState;
        arg0->transition.bytes.timer = D_8010AE88.bytes[7];
    }

    switch (state) {
    case 0:
        arg0->x += 0x20;
        if (arg0->x >= -0x88) {
            arg0->x = -0x88;
            arg0->transition.bytes.state = 2;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->transition.bytes.timer = (arg0->transition.bytes.timer + 1) & 0xF;
        if (D_80121D88 == 3) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        state = arg0->transition.bytes.state;
        arg0->transition.bytes.timer = 0;
        break;
    case 4:
        arg0->transition.bytes.timer++;
        if ((D_80121D88 == 3) || (D_80121D88 == 7)) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x -= 0x20;
        if (arg0->x < -0x107) {
            arg0->transition.bytes.state = 6;
        }
        state = arg0->transition.bytes.state;
        break;
    }

    D_8010AE89 = state;
    D_8010AE8F = arg0->transition.bytes.timer;

    if (arg0->transition.bytes.state == 6) {
        func_800716E4(arg0);
        D_8010ADE0 = NULL;
        D_8010AE88.bytes[1] = 0;
        D_8010AE88.bytes[7] = 0;
        return;
    }
    func_800483FC(&D_80124868, func_8002332C, arg0);
}

void func_800235E0(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = 0x28;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_80023434);
}

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_80023618.s")
