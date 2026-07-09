#include "common.h"
#include "memory_allocator.h"
#include "effect_task_scheduler.h"
#include "character_select_flow.h"
#include "controller_pak_menu.h"
#include "controller_pak_menu_ui.h"
#include "menu_rendering.h"

#define CONTROLLER_PAK_DELETE_PANEL_HANDLE (D_80112130[0x29])

typedef struct {
    u16 x;
    u8 state;
} ControllerPakPromptTransition;

typedef struct {
    s16 pad0;
    u8 state;
    u8 pad3;
    u16 targetScale;
    u8 pad6[2];
    u8 selectedOption;
} ControllerPakConfirmTransition;

extern void func_800483FC(void *, void *, s32);
extern void func_80017168(void *, s32);
extern s32 D_80124868;
extern s32 D_80124838;
extern CharacterSelectFlowState *D_801235B8;
extern ControllerPakPromptTransition D_8010AF80;
extern ControllerPakConfirmTransition D_8010ADD0;
extern ControllerPakMenuState D_8010AF90;
extern s16 D_80112130[];
extern u8 D_8010ADD2;
extern u8 D_8010AF92;
extern u8 D_8010AF93;
extern s16 D_80112172;
extern s16 D_8011217C;
extern s16 D_80112178;
extern u8 D_800B7E60[];
extern u8 D_800B8070[];
extern u8 D_800B8090[];

void func_8002FFD0(ControllerPakTitleActor *arg0) {
    s32 i;
    s32 j;
    s32 alpha;

    func_8000F030((s16)(arg0->common.x - 4), (s16)(arg0->common.y - 4), func_80043040(D_80112130[0x29]), 2, 0x20, 0x20, 0, 0);
    func_8000F030((s16)(arg0->common.x + 0xD4), (s16)(arg0->common.y - 4), func_80043040(D_80112130[0x29]), 4, 0x20, 0x20, 0, 0);

    i = 0;
    do {
        func_8000F030((s16)(arg0->common.x + i), (s16)(arg0->common.y - 4), func_80043040(D_80112130[0x29]), 3, 0x20, 0x20, 0, 0);
        func_8000F030((s16)(arg0->common.x + i), (s16)(arg0->common.y + 0x24), func_80043040(D_80112130[0x29]), 8, 0x20, 0x20, 0, 0);
        i += 0x10;
    } while (i < 0xE0);

    func_8000F030((s16)(arg0->common.x - 4), (s16)(arg0->common.y + 0x24), func_80043040(D_80112130[0x29]), 7, 0x20, 0x20, 0, 0);
    func_8000F030((s16)(arg0->common.x + 0xD4), (s16)(arg0->common.y + 0x24), func_80043040(D_80112130[0x29]), 9, 0x20, 0x20, 0, 0);

    i = 0;
loop_outer:
    func_8000F030((s16)(arg0->common.x - 4), (s16)(arg0->common.y + i), func_80043040(D_80112130[0x29]), 5, 0x20, 0x20, 0, 0);
    func_8000F030((s16)(arg0->common.x + 0xD4), (s16)(arg0->common.y + i), func_80043040(D_80112130[0x29]), 6, 0x20, 0x20, 0, 0);
    j = 0;
    do {
        func_8000F030((s16)(arg0->common.x + j), (s16)(arg0->common.y + i), func_80043040(D_80112130[0x29]), 0xB, 0x20, 0x20, 0, 0);
        j += 0x10;
    } while (j != 0xE0);
    i += 0x10;
    if (i != 0x30) {
        // IDO register allocation nudge for matching codegen.
        if (1) {
        }
        goto loop_outer;
    }

    func_80013154((s16)(arg0->common.x + 0x30), arg0->common.y, D_800B7E60, 0, 0x100, 0);

    alpha = (arg0->selectedOption == 0) ? 0x100 : 0x60;

    func_8000F8AC((s16)(arg0->common.x + 0x4C), (s16)(arg0->common.y + 0x10), func_80043040(D_80112130[0x24]), 0x17, 0x20,
                  0x20, 0, alpha, 0);

    if (alpha == 0x100) {
        alpha = 0x60;
    } else {
        alpha = 0x100;
    }

    func_8000F8AC((s16)(arg0->common.x + 0x4C), (s16)(arg0->common.y + 0x20), func_80043040(D_80112130[0x24]), 0x18, 0x20,
                  0x20, 0, alpha, 0);
    func_8000F8AC((s16)(arg0->common.x + 0x4C), (s16)(arg0->common.y + (arg0->selectedOption * 0x10) + 0x10),
                  func_80043040(D_80112130[0x24]), 0x12, 0x20, 0x20, 0, arg0->scale, 0);
}

// func_8003048C best match: 92.821%
#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_menu_ui/func_8003048C.s")

#ifdef NON_MATCHING
void func_8003048C(ControllerPakTitleActor *arg0) {
    s32 temp_v0;
    s32 state;
    ControllerPakTitleActor *temp_a2 = arg0;

    temp_v0 = D_8010AF80.state;
    state = arg0->blinkState;
    if (state != temp_v0) {
        arg0->blinkState = temp_v0;
        state = temp_v0 & 0xFF;
        arg0->scale = D_8010AF80.x;
    }

    temp_v0 = state;
    switch (temp_v0) {
        case 1:
            temp_a2->selectedOption = D_801235B8->timer;
            if (temp_a2->timer < 0x10) {
                temp_a2->scale -= 9;
            } else {
                temp_a2->scale += 9;
            }
            temp_a2->timer = (temp_a2->timer + 1) & 0x1F;
            state = temp_a2->blinkState;
            break;
        case 0:
        case 2:
            break;
    }

    if (state == 3) {
        func_800716E4(temp_a2);
        return;
    }
    func_800483FC(&D_80124868, func_8002FFD0, (s32)temp_a2);
}
#endif

void func_80030570(ControllerPakTitleActor *arg0) {
    arg0->common.x = -0x70;
    arg0->common.y = -0x28;
    arg0->selectedOption = 0;
    arg0->blinkState = 0;
    arg0->scale = 0x100;
    arg0->timer = 0;
    func_80071824(arg0, func_8003048C);
}

// func_800305B8 best match: 99.732% (nonmatchings/func_800305B8-5272447827802519043/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_menu_ui/func_800305B8.s")

#ifdef NON_MATCHING
extern u8 D_80121B55;
extern u8 D_800EC8B4[];
extern u8 D_800B7E78[];
extern u8 D_800B8018[];
extern u8 D_800B8030[];
extern u8 D_800B8048[];

void func_800305B8(ControllerPakConfirmActor *arg0) {
    register ControllerPakConfirmActor *actor;
    s32 i;
    s32 j;
    s32 playerIndex;
    s32 playerY;
    s32 yAdjust;
    s32 alpha;
    u16 playerNumberText[5];
    u8 *message;

    actor = arg0;

    if (actor->state == 8) {
        yAdjust = 8;
    } else {
        yAdjust = 0;
    }

    func_8000F8AC((s16)(actor->common.x - 4), (s16)(actor->common.y - yAdjust - 4), func_80043040(D_80112130[0x29]), 2,
                  0x20, 0x20, 0, actor->scale, 0);
    func_8000F8AC((s16)(actor->common.x + 0xD4), (s16)(actor->common.y - yAdjust - 4), func_80043040(D_80112130[0x29]),
                  4, 0x20, 0x20, 0, actor->scale, 0);

    i = 0;
    do {
        func_8000F8AC((s16)(actor->common.x + i), (s16)(actor->common.y - yAdjust - 4), func_80043040(D_80112130[0x29]),
                      3, 0x20, 0x20, 0, actor->scale, 0);
        func_8000F8AC((s16)(actor->common.x + i), (s16)(actor->common.y + yAdjust + 0x24), func_80043040(D_80112130[0x29]),
                      8, 0x20, 0x20, 0, actor->scale, 0);
        i += 0x10;
    } while (i < 0xE0);

    func_8000F8AC((s16)(actor->common.x - 4), (s16)(actor->common.y + yAdjust + 0x24), func_80043040(D_80112130[0x29]),
                  7, 0x20, 0x20, 0, actor->scale, 0);
    func_8000F8AC((s16)(actor->common.x + 0xD4), (s16)(actor->common.y + yAdjust + 0x24), func_80043040(D_80112130[0x29]),
                  9, 0x20, 0x20, 0, actor->scale, 0);

    i = 0;
    do {
        func_8000F8AC((s16)(actor->common.x - 4), (s16)(actor->common.y + i), func_80043040(D_80112130[0x29]), 5, 0x20,
                      0x20, 0, actor->scale, 0);
        func_8000F8AC((s16)(actor->common.x + 0xD4), (s16)(actor->common.y + i), func_80043040(D_80112130[0x29]), 6, 0x20,
                      0x20, 0, actor->scale, 0);
        j = 0;
        do {
            func_8000F8AC((s16)(actor->common.x + j), (s16)(actor->common.y + i), func_80043040(D_80112130[0x29]), 0xB,
                          0x20, 0x20, 0, actor->scale, 0);
            j += 0x10;
            // IDO register allocation nudge for matching codegen.
            if (((!actor) && (!actor)) && (!actor)) {
            }
        } while (j != 0xE0);
        i += 0x10;
    } while (i < 0x30);

    if (actor->state == 8) {
        playerIndex = 0;
        playerY = 0;
        do {
            playerNumberText[0] = -4;
            playerNumberText[1] = 7;
            playerNumberText[2] = playerIndex + 1;
            playerNumberText[3] = 0x19;
            playerNumberText[4] = 0xFFFF;
            if (playerIndex < D_80121B55) {
                if (D_800EC8B4[playerIndex] == 1) {
                    message = D_800B8048;
                } else {
                    message = D_800B8018;
                }
                alpha = (u16)actor->scale;
            } else {
                message = D_800B8030;
                alpha = 0x60;
            }
            func_80013154((s16)(actor->common.x + 0x10), (s16)(actor->common.y - yAdjust + playerY),
                          (u8 *)playerNumberText, 0, alpha, 0);
            func_80013154((s16)(actor->common.x + 0x40), (s16)(actor->common.y - yAdjust + playerY), message, 0, alpha,
                          0);
            playerIndex++;
            playerY += 0x10;
        } while (playerIndex != 4);
    } else {
        func_80013154(actor->common.x, actor->common.y, &D_800B7E78[actor->targetScale * 0x68], 0, actor->scale, 0);
    }

    if (actor->state == 9) {
        alpha = 0x60;
        if (D_8010ADD0.unk9 == 0) {
            alpha = 0x100;
        }
        func_8000F8AC((s16)(actor->common.x + 0x4C), (s16)(actor->common.y + 0x10), func_80043040(D_80112130[0x21]),
                      0x17, 0x20, 0x20, 0, alpha, 0);
        if (alpha == 0x100) {
            alpha = 0x60;
        } else {
            alpha = 0x100;
        }
        func_8000F8AC((s16)(actor->common.x + 0x4C), (s16)(actor->common.y + 0x20), func_80043040(D_80112130[0x21]),
                      0x18, 0x20, 0x20, 0, alpha, 0);
        func_8000F8AC((s16)(actor->common.x + 0x4C), (s16)(actor->common.y + (D_8010ADD0.unk9 * 0x10) + 0x10),
                      func_80043040(D_80112130[0x21]), 0x12, 0x20, 0x20, 0, actor->optionScale, 0);
    }

    if ((actor->state == 1) || (actor->state == 3) || (actor->state == 8)) {
        func_8000F030((s16)(actor->common.x + 0xD0), (s16)(actor->common.y + yAdjust + 0x20), func_80043040(D_80112130[0x21]),
                      ((actor->timer >= 8) + 5) & 0xFFFF, 0x20, 0x20, 0, 0);
    }
}
#endif

void func_80030CC4(ControllerPakConfirmActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->state;
    if (state != (globalState = D_8010ADD0.state)) {
        arg0->state = globalState;
        if (1) {}
        {}
        if (1) {}
        if (1) {}
        if (1) {}
        arg0->targetScale = D_8010ADD0.targetScale;
        arg0->timer = 0;
        arg0->optionScale = 0x100; state = globalState;
    }

    switch (state) {
        case 0:
            arg0->scale += 0x28;
            if (arg0->scale >= 0x100) {
                arg0->scale = 0x100;
                if (D_8010ADD0.selectedOption == 1) {
                    arg0->state = 3;
                } else {
                    arg0->state = 1;
                }
            }
            state = arg0->state;
            break;
        case 1:
            state = arg0->state;
            arg0->timer = (arg0->timer + 1) & 0xF;
            break;
        case 2:
            state = arg0->state;
            arg0->timer = 0;
            break;
        case 3:
            state = arg0->state;
            arg0->timer = (arg0->timer + 1) & 0xF;
            break;
        case 4:
            arg0->scale -= 0x28;
            if (arg0->scale <= 0) {
                arg0->scale = 0;
                arg0->state = 5;
            }
            state = arg0->state;
            break;
        case 7:
            state = arg0->state;
            arg0->timer = 0;
            break;
        case 8:
            state = arg0->state;
            arg0->timer = (arg0->timer + 1) & 0xF;
            break;
        case 9:
            if ((s32)arg0->timer < 0x10) {
                arg0->optionScale -= 9;
            } else {
                arg0->optionScale += 9;
            }
            state = arg0->state;
            arg0->timer = (arg0->timer + 1) & 0x1F;
            break;
        case 5:
        case 6:
            break;
    }

    D_8010ADD2 = state;
    if (arg0->state == 5) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_800305B8, (s32)arg0);
}

void func_80030EA8(ControllerPakConfirmActor *arg0) {
    arg0->common.x = -0x70;
    arg0->common.y = -0x1C;
    arg0->state = 6;
    arg0->scale = 0;
    arg0->timer = 0;
    arg0->targetScale = 0;
    func_80071824(arg0, func_80030CC4);
}

void func_80030EF0(ControllerPakTitleActor *arg0) {
    u32 drawAlpha;
    u16 alpha;

    if (D_8010AF90.mainChoice == 0) {
        alpha = 0x100;
    } else {
        alpha = 0x80;
    }

    drawAlpha = alpha;
    func_8000F8AC(arg0->common.x, arg0->common.y, func_80043040(D_80112172), 1, 0x20, 0x20, 0, drawAlpha, 0);

    if (drawAlpha == 0x100) {
        alpha = 0x80;
    } else {
        alpha = 0x100;
    }

    func_8000F8AC((s16)(arg0->common.x + 0x70), arg0->common.y, func_80043040(D_80112172), 2, 0x20, 0x20, 0,
                  alpha, 0);
    func_8000F8AC((s16)(arg0->common.x + (D_8010AF90.mainChoice * 0x70)), arg0->common.y, func_80043040(D_80112172),
                  5, 0x20, 0x20, 0, arg0->scale, 0);
}

void func_80031038(ControllerPakTitleActor *arg0) {
    switch (D_8010AF93) {
        case 0:
            if (arg0->timer < 0x10) {
                arg0->scale -= 8;
            } else {
                arg0->scale += 8;
            }
            arg0->timer = (arg0->timer + 1) & 0x1F;
            break;
        case 1:
        case 2:
            arg0->scale = 0x100;
            arg0->timer = 0;
            break;
    }
    func_800483FC(&D_80124868, func_80030EF0, (s32)arg0);
}

void func_800310EC(ControllerPakTitleActor *arg0) {
    arg0->common.x = -0x68;
    arg0->common.y = -0x44;
    arg0->scale = 0x100;
    arg0->timer = 0;
    func_80071824(arg0, func_80031038);
}

void func_8003112C(ControllerPakTitleActor *arg0) {
    ControllerPakTitleActor *actor;
    u16 alpha;
    u16 otherAlpha;

    actor = arg0;

    if (D_8010AF90.state == 2) {
        otherAlpha = 0x80;
        alpha = 0x80;
        if (D_8010AF90.confirmChoice == 0) {
            alpha = 0x100;
        } else {
            otherAlpha = 0x100;
        }
    } else {
        alpha = 0x80;
        otherAlpha = 0x80;
        if (!D_8010AF92) {
        }
    }

    func_8000F8AC(arg0->common.x, arg0->common.y, func_80043040(D_80112172), 3, 0x20, 0x20, 0, alpha, 0);
    func_8000F8AC((s16)(actor->common.x + 0x70), actor->common.y, func_80043040(D_80112172), 4, 0x20, 0x20, 0,
                  otherAlpha, 0);

    if (D_8010AF93 == 2) {
        func_8000F8AC((s16)(actor->common.x + (D_8010AF92 * 0x70)), actor->common.y, func_80043040(D_80112172), 5, 0x20,
                      0x20, 0, actor->scale, 0);
    }
}

void func_80031294(ControllerPakTitleActor *arg0) {
    switch (D_8010AF93) {
        case 0:
        case 1:
            break;
        case 2:
            if (arg0->timer < 0x10) {
                arg0->scale -= 8;
            } else {
                arg0->scale += 8;
            }
            arg0->timer = (arg0->timer + 1) & 0x1F;
            break;
    }
    func_800483FC(&D_80124868, func_8003112C, (s32)arg0);
}

void func_80031330(ControllerPakTitleActor *arg0) {
    arg0->common.x = -0x68;
    arg0->common.y = 0x48;
    arg0->scale = 0x100;
    arg0->timer = 0;
    func_80071824(arg0, func_80031294);
}

#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_menu_ui/func_80031370.s")

void func_800314E0(s32 arg0) {
    func_800483FC(&D_80124868, func_80031370, arg0);
}

void func_80031510(ControllerPakTwoPointActor *arg0) {
    s32 temp_v0 = 0x34;

    arg0->common.x = -0x24;
    arg0->common.y = temp_v0;
    arg0->x2 = 0x5C;
    arg0->y2 = temp_v0;
    func_80071824(arg0, func_800314E0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_menu_ui/func_80031550.s")

void func_80031A88(ControllerPakFileListActor *arg0) {
    if (D_8010AF93 == 1) {
        if (arg0->cursorTimer < 0x10) {
            arg0->cursorScale -= 9;
        } else {
            arg0->cursorScale += 9;
        }
        arg0->cursorTimer = (arg0->cursorTimer + 1) & 0x1F;
    } else {
        arg0->cursorScale = 0x100;
        arg0->cursorTimer = 0;
    }
    func_800483FC(&D_80124868, func_80031550, (s32)arg0);
}

void func_80031B24(ControllerPakFileListActor *arg0) {
    s32 temp_v0 = -0x20;
    s32 temp_v1 = -0x84;

    arg0->positions[0].x = temp_v1;
    arg0->positions[0].y = temp_v0;
    arg0->positions[1].x = -0x6A;
    arg0->positions[1].y = temp_v0;
    arg0->positions[2].x = 0x2C;
    arg0->positions[2].y = temp_v0;
    arg0->positions[3].x = 0x54;
    arg0->positions[3].y = temp_v0;
    arg0->positions[4].x = 0x69;
    arg0->positions[4].y = temp_v0;
    arg0->positions[5].x = temp_v1;
    arg0->positions[5].y = -0x1F;
    arg0->cursorScale = 0x100;
    arg0->cursorTimer = 0;
    func_80071824(arg0, func_80031A88);
}

void func_80031BA0(ControllerPakSpriteActor *arg0) {
    func_80011D74((MenuRenderSprite *)&arg0->sprite, 0, arg0->common.x, arg0->common.y);
}

void func_80031BD4(s32 arg0) {
    func_800483FC(&D_80124838, func_80031BA0, arg0);
}

void func_80031C04(ControllerPakSpriteActor *arg0) {
    func_80017168(&arg0->sprite, func_80043040(D_8011217C));
    arg0->sprite.x = 0x10;
    arg0->sprite.y = 0x10;
    arg0->common.x = arg0->startX;
    arg0->common.y = arg0->startY;
    func_80071824(arg0, func_80031BD4);
}

void func_80031C6C(ControllerPakSpriteActor *arg0) {
    func_80011D74((MenuRenderSprite *)&arg0->sprite, 1, arg0->common.x, arg0->common.y);
}

void func_80031CA0(s32 arg0) {
    func_800483FC(&D_80124838, func_80031C6C, arg0);
}

void func_80031CD0(ControllerPakSpriteActor *arg0) {
    func_80017168(&arg0->sprite, func_80043040(D_8011217C));
    arg0->sprite.x = 0x10;
    arg0->sprite.y = 0x20;
    arg0->common.x = arg0->startX;
    arg0->common.y = arg0->startY;
    func_80071824(arg0, func_80031CA0);
}

void func_80031D3C(ControllerPakWindowActor *arg0) {
    func_8000F8AC(arg0->common.x, arg0->common.y, func_80043040(D_80112178), 0, 0x20, 0x20, 0, arg0->scale, 0);
    func_8000F8AC((s16)(arg0->common.x + 0x40), arg0->common.y, func_80043040(D_80112178), 1, 0x20, 0x20, 0, arg0->scale, 0);
    func_8000F8AC((s16)(arg0->common.x + 0x78), arg0->common.y, func_80043040(D_80112178), 1, 0x20, 0x20, 0, arg0->scale, 0);
    func_8000F8AC((s16)(arg0->common.x + 0xB0), arg0->common.y, func_80043040(D_80112178), 2, 0x20, 0x20, 0, arg0->scale, 0);
    func_80013154((s16)(arg0->common.x + 4), (s16)(arg0->common.y + 4), D_800B8090, 0, arg0->scale, 0);
    if (arg0->selectedOption == 1) {
        func_8000F030((s16)(arg0->common.x + 0xD4), (s16)(arg0->common.y + 0x24), func_80043040(D_80112178),
                      ((arg0->timer >= 8) + 5) & 0xFFFF, 0x20, 0x20, 0, 0);
    }
}

// func_80031F40 best match: 84.757% (nonmatchings/func_80031F40-4839787584499344943/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_menu_ui/func_80031F40.s")

#ifdef NON_MATCHING
void func_80031F40(ControllerPakDeletePromptActor *arg0) {
    u8 globalState;
    u8 state;
    u8 switchState;

    globalState = D_8010AF96;
    state = arg0->timer;
    switchState = state;
    if (state != globalState) {
        state = globalState;
        switchState = globalState;
        arg0->timer = globalState;
    }

    switch (switchState) {
    case 0:
        arg0->scale = (s16)arg0->scale + 0x28;
        if ((s16)arg0->scale >= 0x100) {
            arg0->scale = 0x100;
            arg0->timer = 1;
        }
        state = arg0->timer;
        break;
    case 1:
        state = arg0->timer;
        arg0->selectedOption = (arg0->selectedOption + 1) & 0xF;
        break;
    case 2:
        arg0->scale = (s16)arg0->scale - 0x28;
        if ((s16)arg0->scale <= 0) {
            arg0->scale = 0;
            arg0->timer = 3;
        }
        state = arg0->timer;
        break;
    case 3:
        break;
    }

    D_8010AF96 = state;
    if (arg0->timer == 3) {
        func_800716E4((EffectTask *)arg0);
        return;
    }

    func_800483FC(&D_80124868, func_80031D3C, (s32)arg0);
}
#endif

void func_8003205C(ControllerPakDeletePromptActor *arg0) {
    arg0->common.x = -0x70;
    arg0->common.y = -0x1C;
    arg0->timer = 0;
    arg0->scale = 0;
    arg0->selectedOption = 0;
    func_80071824(arg0, func_80031F40);
}

void func_8003209C(ControllerPakDeletePromptActor *arg0) {
    s32 i;
    s32 j;
    s32 alpha;
    s32 limit;

    func_8000F030(arg0->common.x, arg0->common.y, func_80043040(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 2, 0x20, 0x20, 0, 0);
    func_8000F030((s16)(arg0->common.x + 0x90), arg0->common.y, func_80043040(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 4, 0x20, 0x20, 0, 0);

    i = 0;
    do {
        func_8000F030((s16)(arg0->common.x + i + 0x10), arg0->common.y, func_80043040(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 3, 0x20, 0x20, 0, 0);
        func_8000F030((s16)(arg0->common.x + i + 0x10), (s16)(arg0->common.y + 0x30), func_80043040(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 8, 0x20, 0x20, 0, 0);
        i += 0x10;
    } while (i < 0x80);

    func_8000F030(arg0->common.x, (s16)(arg0->common.y + 0x30), func_80043040(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 7, 0x20, 0x20, 0, 0);
    func_8000F030((s16)(arg0->common.x + 0x90), (s16)(arg0->common.y + 0x30), func_80043040(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 9, 0x20, 0x20, 0, 0);

    i = 0;
    limit = 0x80;
loop_outer:
    func_8000F030(arg0->common.x, (s16)(arg0->common.y + i + 0x10), func_80043040(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 5, 0x20, 0x20, 0, 0);
    func_8000F030((s16)(arg0->common.x + 0x90), (s16)(arg0->common.y + i + 0x10), func_80043040(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 6, 0x20, 0x20, 0, 0);

    j = 0;
    do {
        func_8000F030((s16)(arg0->common.x + j + 0x10), (s16)(arg0->common.y + i + 0x10), func_80043040(CONTROLLER_PAK_DELETE_PANEL_HANDLE), 0xB, 0x20, 0x20, 0, 0);
        j += 0x10;
    } while (j != limit);

    i += 0x10;
    if (i != 0x20) {
        if (1) {
        }
        goto loop_outer;
    }

    func_80013154((s16)(arg0->common.x + 0x1C), (s16)(arg0->common.y + 4), D_800B8070, 1, 0x100, 0);

    alpha = (arg0->selectedOption == 0) ? 0x100 : 0x60;

    func_8000F8AC((s16)(arg0->common.x + 0x2C), (s16)(arg0->common.y + 0x18), func_80043040(D_80112130[0x24]), 0x17, 0x20, 0x20, 0, alpha, 0);

    if (alpha == 0x100) {
        alpha = 0x60;
    } else {
        alpha = 0x100;
    }

    func_8000F8AC((s16)(arg0->common.x + 0x2C), (s16)(arg0->common.y + 0x28), func_80043040(D_80112130[0x24]), 0x18, 0x20, 0x20, 0, alpha, 0);
    func_8000F8AC((s16)(arg0->common.x + 0x2C), (s16)(arg0->common.y + (arg0->selectedOption * 0x10) + 0x18), func_80043040(D_80112130[0x24]), 0x12, 0x20, 0x20, 0, arg0->scale, 0);
}

void func_80032534(ControllerPakDeletePromptActor *arg0) {
    arg0->selectedOption = D_8010AF90.confirmChoice;
    if (arg0->timer < 0x10) {
        arg0->scale -= 9;
    } else {
        arg0->scale += 9;
    }
    arg0->timer = (arg0->timer + 1) & 0x1F;
    if (D_8010AF90.state != 3) {
        func_800716E4(arg0);
    } else {
        func_800483FC(&D_80124868, func_8003209C, (s32)arg0);
    }
}

void func_800325D0(ControllerPakDeletePromptActor *arg0) {
    arg0->common.x = -0x54;
    arg0->common.y = -0x20;
    arg0->scale = 0x100;
    arg0->selectedOption = 1;
    arg0->timer = 0;
    func_80071824(arg0, func_80032534);
}
