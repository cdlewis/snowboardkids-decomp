#include "common.h"
#include "memory_allocator.h"
#include "effect_task_scheduler.h"
#include "main_menu_score_ui.h"
#define MENU_RENDERING_BROAD_PROTOTYPES
#define MENU_RENDERING_F8AC_U16_ALPHA_PROTOTYPE
#include "menu_rendering.h"

typedef struct {
    u8 state;
    u8 pad1;
    s16 scale;
    u16 slideOffset;
} MainMenuScoreTransition;

typedef struct {
    /* 0x0 */ u8 step;
    /* 0x1 */ char pad1[0x1];
    /* 0x2 */ s16 timer;
    /* 0x4 */ u16 targetState;
    /* 0x6 */ s16 nextTimer;
} ControllerPakHighScoreSaveFlow;

extern void func_800483FC(void *, void *, void *);
extern u8 D_800B73F0[][0x4C];
extern u8 D_800B7986[];
extern u8 D_800B7987[];
extern u8 D_800B79AC[];
extern s16 D_80112172;
extern s16 gControllerPakHighScoreSaveFlowTimer;
extern ControllerPakHighScoreSaveFlow gControllerPakHighScoreSaveFlow;
extern u16 gControllerPakHighScoreSaveFlowTargetState;
extern u16 gControllerPakHighScoreSaveFlowNextTimer;
extern u8 gControllerPakHighScoreSaveFlowStep;
extern void *D_80124868;
extern void *D_8010ADE0;
extern void *D_8010ADE4;
extern s16 D_800EC9C8;
extern s16 D_800EC9D0;
extern u8 D_800EC9C1;

#ifdef NON_MATCHING
#define MAIN_MENU_SCORE_TEXTURE_HANDLE (*(s16 *)&D_80112130[0x42])

typedef struct {
    char pad[0x34];
    u8 badgeIds[0x18];
    u8 iconCount;
} MainMenuScorePlayerView;

extern s16 D_800B3462[];
extern MainMenuScorePlayerView D_800EC9F0;
extern u8 D_80112130[];
extern s32 D_80121D8C;
#endif

struct MainMenuScoreTask {
    char pad[0x18];
    s16 x;
    s16 y;
    union {
        s8 state;
        s16 scale;
    } unk1C;
    union {
        struct {
            s8 unk1E;
            s8 unk1F;
            u8 unk20;
            u8 frame;
            u16 alpha;
            u16 alphaTimer;
        } b;
        struct {
            s16 slideOffset;
            u16 selection;
            u16 alpha;
            u16 alphaTimer;
        } w;
        struct {
            u16 slideOffset;
            u16 selection;
            u16 alpha;
            u16 alphaTimer;
        } wu;
    } state;
};

// func_8002B560 best match: 99.235% (nonmatchings/func_8002B560-6688367443449623229/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_score_ui/func_8002B560.s")

#ifdef NON_MATCHING
const char D_800E0F30[] = "%6d";

void func_8002B560(MainMenuScoreTask *arg0) {
    s16 *new_var;
    s16 *badgeIndex;
    s32 xOffset;
    s32 tile;
    s32 alpha;
    s32 i;
    char text[8];
    s32 next;
    s32 count;

    func_8000F030(arg0->x, arg0->y, func_80043040(MAIN_MENU_SCORE_TEXTURE_HANDLE), 0xA, 0x20, 0x20, 0, 0);
    func_8000F030((s16)(arg0->x + 0x40), arg0->y, func_80043040(MAIN_MENU_SCORE_TEXTURE_HANDLE), 0xB, 0x20,
                  0x20, 0, 0);
    func_8000F030((s16)(arg0->x + 0x80), arg0->y, func_80043040(MAIN_MENU_SCORE_TEXTURE_HANDLE), 0xC, 0x20,
                  0x20, 0, 0);

    text[0] = '1';
    text[1] = 0;
    func_80013D0C((s16)(arg0->x + 0x32), (s16)((unsigned long)(arg0->y + 2)), text, 0, 0x100);

    sprintf(text, D_800E0F30, D_80121D8C);
    func_80013D0C((s16)(arg0->x + 0x44), (s16)(arg0->y + 0x1B), text, 0, 0x100);

    if (D_800EC9F0.iconCount == 3) {
        count = 3;
    } else {
        count = D_800EC9F0.iconCount + 1;
    }

    i = 0;
    if (count > 0) {
        xOffset = 0;
        do {
            next = 1;
            next = i + next;
            alpha = 0x100;
            if (D_800EC9F0.iconCount < next) {
                alpha = 0x70;
            }

            func_8000F8AC((s16)(arg0->x + xOffset + 4), (s16)(arg0->y + 0x11),
                          func_80043040(MAIN_MENU_SCORE_TEXTURE_HANDLE), 0x19, 0x20, 0x20, 0, alpha, 9 - i);
            i = next;
            xOffset += 0x10;
        } while (next != count);
        i = 0;
    }

    count = 6;
    if (D_800EC9F0.iconCount == 1) {
        count = 7;
    } else if (D_800EC9F0.iconCount == 2) {
        count = 8;
    } else {
        if (D_800EC9F0.iconCount == 3) {
            count = 9;
        }
        i = 0;
    }

    if (count > 0) {
        xOffset = 0;
        new_var = D_800B3462;
        badgeIndex = new_var;
        do {
            tile = D_800EC9F0.badgeIds[*badgeIndex];
            alpha = 0x70;
            if (tile != 0) {
                alpha = 0x100;
                tile += 6;
            } else {
                tile = 9;
                if ((!i) && (!i)) {
                }
            }
            func_8000F8AC((s16)(arg0->x + xOffset + 4), (s16)(arg0->y + 0x27),
                          func_80043040(MAIN_MENU_SCORE_TEXTURE_HANDLE), (i + 0x1A) & 0xFFFF, 0x20, 0x20, 0,
                          alpha, tile);
            i++;
            xOffset += 0xE;
            badgeIndex++;
        } while (i != count);
    }
}
#endif

void func_8002B8B4(MainMenuScoreTask *arg0) {
    u8 state = arg0->unk1C.state;

    switch (state) {
    case 0:
        arg0->x -= 0x20;
        if (arg0->x < -0x43) {
            arg0->x = -0x44;
            arg0->unk1C.state = 1;
            D_8010ADE0 = createEffectTask(func_8002BC60, 0, 0x62);
            createEffectTask(func_8002C498, 0, 0x63);
        }
        state = arg0->unk1C.state;
        break;
    case 1:
        if (D_800EC9C8 != 0) {
            if (D_800EC9C1 == 0x14) {
                state = arg0->unk1C.state = 2;
            }
        }
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0x117) {
            arg0->unk1C.state = 3;
        }
        state = arg0->unk1C.state;
        break;
    case 3:
        break;
    }
    state = arg0->unk1C.state;
    if (state == 3) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002B560, arg0);
}

void func_8002BA00(MainMenuScoreTask *arg0) {
    arg0->x = 0x90;
    arg0->y = -0x20;
    arg0->unk1C.state = 0;
    func_80071824(arg0, func_8002B8B4);
}

void func_8002BA38(MainMenuScoreTask *arg0) {
    s32 zero = 0;
    u16 alpha;

    if (D_800EC9C8 != 8) {
        alpha = arg0->unk1C.scale;
        func_8000F8AC((s16)(arg0->x - 2), (s16)(arg0->y + 0xC), func_80043040(D_80112172), 8, 0x20, 0x20,
                      zero, alpha, zero);
        func_8000F8AC((s16)(arg0->x + 0x3E), (s16)(arg0->y + 0xC), func_80043040(D_80112172), 9, 0x20,
                      0x20, zero, alpha, zero);
    }
}

void func_8002BB24(MainMenuScoreTask *arg0) {
    u8 state = arg0->state.b.unk1F;

    switch (state) {
    case 0:
        arg0->unk1C.scale += 0x26;
        if (arg0->unk1C.scale >= 0x100) {
            arg0->unk1C.scale = 0x100;
            arg0->state.b.unk1F = 1;
            D_8010ADE4 = createEffectTask(func_8002BF54, 0, 0x63);
        }
        state = arg0->state.b.unk1F;
        break;
    case 1:
        state = arg0->state.b.unk1F = 2;
        break;
    case 2:
        if ((D_800EC9C8 == 5) && (D_800EC9C1 == 0x14)) {
            state = arg0->state.b.unk1F = 3;
        }
        break;
    case 3:
        arg0->x -= 0x20;
        if (arg0->x < -0x117) {
            arg0->state.b.unk1F = 4;
        }
        state = arg0->state.b.unk1F;
        break;
    case 4:
        break;
    }
    if ((unsigned int)state == 4) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002BA38, arg0);
}

void func_8002BC60(MainMenuScoreTask *arg0) {
    arg0->x = -0x44;
    arg0->y = -0x1E;
    arg0->unk1C.scale = 0;
    arg0->state.b.unk1F = 0;
    func_80071824(arg0, func_8002BB24);
}

void func_8002BC9C(MainMenuScoreTask *arg0) {
    u8 *text;

    if (D_800EC9C8 != 8) {
        text = D_800B73F0[D_800EC9C8];
        func_80013154(arg0->x, arg0->y, text, 1, arg0->unk1C.scale, 0);
        if (((D_800EC9C8 == 4) || (D_800EC9C8 >= 7)) && (D_800EC9D0 == 0)) {
            if (arg0->unk1C.scale == 0x100) {
                func_8000F030((s16)(arg0->x + 0x70), (s16)(arg0->y + 0x10), func_80043040(D_80112172),
                              (((s32)arg0->state.b.frame >= 8) + 5) & 0xFFFF, 0x20, 0x20, 0, 0);
            }
        }
    }
}

void func_8002BDAC(MainMenuScoreTask *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->state.b.unk20;
    if (state != (globalState = gControllerPakHighScoreSaveFlow.step)) {
        arg0->state.b.unk20 = globalState;
        state = globalState;
        arg0->unk1C.scale = gControllerPakHighScoreSaveFlow.timer;
        arg0->state.wu.slideOffset = gControllerPakHighScoreSaveFlow.targetState;
    }

    switch (state) {
    case 0:
        arg0->state.b.unk20 = 1;
        state = arg0->state.b.unk20;
        break;
    case 1:
        if ((D_800EC9C8 == 5) && (D_800EC9C1 == 0x14)) {
            arg0->state.b.unk20 = 4;
            state = arg0->state.b.unk20;
        }
        break;
    case 2:
        arg0->unk1C.scale += 0x26;
        if (arg0->unk1C.scale >= 0x100) {
            arg0->unk1C.scale = 0x100;
            arg0->state.b.unk20 = 1;
        }
        state = arg0->state.b.unk20;
        break;
    case 3:
        arg0->unk1C.scale -= 0x26;
        if (arg0->unk1C.scale <= (state * 0)) {
            arg0->unk1C.scale = 0;
            arg0->state.b.unk20 = 2;
            D_800EC9D0 = 0;
            D_800EC9C8 = arg0->state.wu.slideOffset;
        }
        state = arg0->state.b.unk20;
        break;
    case 4:
        arg0->x -= 0x20;
        if (arg0->x < -0x117) {
            arg0->state.b.unk20 = 5;
        }
        state = arg0->state.b.unk20;
        break;
    }

    arg0->state.b.frame = (arg0->state.b.frame + 1) & 0xF;
    gControllerPakHighScoreSaveFlowStep = state;
    if ((u8)arg0->state.b.unk20 == 5) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002BC9C, arg0);
}

void func_8002BF54(MainMenuScoreTask *arg0) {
    arg0->x = -0x42;
    arg0->y = -0xE;
    arg0->unk1C.scale = 0x100;
    gControllerPakHighScoreSaveFlowTimer = 0x100;
    arg0->state.b.unk20 = 0;
    func_80071824(arg0, func_8002BDAC);
}

void func_8002BF9C(MainMenuScoreTask *arg0) {
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
        func_8000F8AC(arg0->x, arg0->unk1C.scale, func_80043040(D_80112172), D_800B7987[D_800EC9C8 * 2], 0x20, 0x20,
                      0, drawAlpha, 0);

        if (drawAlpha == 0x100) {
            alpha = 0x60;
        } else {
            alpha = 0x100;
        }

        func_8000F8AC(arg0->x, arg0->y, func_80043040(D_80112172), D_800B7986[D_800EC9C8 * 2], 0x20, 0x20, 0, alpha,
                      0);

        state = D_800EC9D0;
        if ((state == 3) || (state == 4)) {
            func_8000F8AC(arg0->x, (s16)(((D_800EC9D0 * 0x10) + arg0->y) - 0x30), func_80043040(D_80112172), 0x12,
                          0x20, 0x20, 0, (u16)arg0->state.w.alpha, 7);
            state = D_800EC9D0;
        }
    }
    if ((state >= 5) && (arg0->state.w.slideOffset == 0)) {
        gControllerPakHighScoreSaveFlowTargetState = arg0->state.w.selection;
        gControllerPakHighScoreSaveFlow.step = 3;
        D_800EC9D0 = 0;
        if (arg0->state.w.selection == 0) {
            gControllerPakHighScoreSaveFlow.timer = 0x100;
            gControllerPakHighScoreSaveFlow.step = 1;
            D_800EC9C8 = arg0->state.w.selection;
        }
    }
}

void func_8002C18C(MainMenuScoreTask *arg0) {
    s32 temp_a0;
    s32 var_v1;

    if (gControllerPakHighScoreSaveFlowNextTimer != arg0->state.w.selection) {
        arg0->state.w.selection = gControllerPakHighScoreSaveFlowNextTimer;
    }
    if ((D_800EC9D0 != 0) && (D_800EC9D0 != 3) && (D_800EC9D0 != 4)) {
        if (D_800EC9D0 < 5) {
            var_v1 = 1;
        } else {
            var_v1 = -1;
        }
        temp_a0 = var_v1 * 8;
        arg0->state.w.slideOffset += temp_a0;
        if (arg0->state.w.slideOffset == 0x20) {
            D_800EC9D0 += 2;
            arg0->state.b.alphaTimer = 0;
            arg0->state.b.alpha = 0x100;
        }
        if (temp_a0 == 8) {
            if (arg0->state.w.slideOffset < 0x18) {
                arg0->y += 8;
            }
            arg0->unk1C.scale += 8;
        } else {
            if (arg0->state.w.slideOffset < 0x10) {
                arg0->y -= 8;
            }
            arg0->unk1C.scale -= 8;
        }
    }
    if ((D_800EC9D0 == 3) || (D_800EC9D0 == 4)) {
        if ((s32)arg0->state.b.alphaTimer < 0x10) {
            arg0->state.b.alpha -= 9;
        } else {
            arg0->state.b.alpha += 9;
        }
        arg0->state.b.alphaTimer = (arg0->state.b.alphaTimer + 1) & 0x1F;
    }
    func_800483FC(&D_80124868, func_8002BF9C, arg0);
}

void func_8002C318(MainMenuScoreTask *arg0) {
    arg0->x = -0x28;
    arg0->y = 0xC;
    arg0->unk1C.scale = 0xC;
    func_80071824(arg0, func_8002C18C);
}

void func_8002C350(MainMenuScoreTask *arg0) {
    func_800129DC(arg0->x, arg0->y, D_800B79AC, 1, arg0->unk1C.scale);
}

void func_8002C390(MainMenuScoreTask *arg0) {
    u8 state = arg0->state.b.unk1E;

    switch (state) {
    case 0:
        arg0->unk1C.scale += 0x26;
        if (arg0->unk1C.scale >= 0x100) {
            arg0->unk1C.scale = 0x100;
            arg0->state.b.unk1E = 1;
        }
        state = arg0->state.b.unk1E;
        break;
    case 1:
        if ((D_800EC9C8 == 5) && (D_800EC9C1 == 0x14)) {
            state = arg0->state.b.unk1E = 2;
        }
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0xD7) {
            arg0->state.b.unk1E = 4;
        }
        state = arg0->state.b.unk1E;
        break;
    }
    if ((unsigned int)state == 4) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002C350, arg0);
}

void func_8002C498(MainMenuScoreTask *arg0) {
    arg0->x = -0x24;
    arg0->y = -0x38;
    arg0->unk1C.scale = 0;
    arg0->state.b.unk1E = 0;
    func_80071824(arg0, func_8002C390);
}
