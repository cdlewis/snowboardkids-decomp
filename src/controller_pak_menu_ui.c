#include "common.h"
#include "controller_pak_menu.h"

typedef struct {
    s16 x;
    s16 y;
} Vec2s;

typedef struct {
    char pad[0x18];
    s16 x;
    s16 y;
} ControllerPakMenuActor;

typedef struct {
    ControllerPakMenuActor common;
    s16 scale;
    u16 timer;
    s32 selectedOption;
    u8 blinkState;
} ControllerPakTitleActor;

typedef struct {
    ControllerPakMenuActor common;
    s16 scale;
    s16 optionScale;
    u16 timer;
    u8 state;
    u8 selectedOption;
    s16 targetScale;
} ControllerPakConfirmActor;

typedef struct {
    char pad[0x18];
    Vec2s positions[6];
    s16 cursorScale;
    u16 cursorTimer;
} ControllerPakFileListActor;

typedef struct {
    ControllerPakMenuActor common;
    s16 x2;
    s16 y2;
} ControllerPakTwoPointActor;

typedef struct {
    s16 x;
    s16 y;
    u8 unk4[4];
} ControllerPakSprite;

typedef struct {
    ControllerPakMenuActor common;
    ControllerPakSprite sprite;
    s16 startX;
    s16 startY;
} ControllerPakSpriteActor;

typedef struct {
    ControllerPakMenuActor common;
    u16 scale;
    s16 selectedOption;
    u8 timer;
} ControllerPakDeletePromptActor;

extern void func_800483FC(void *, void *, s32);
extern s32 func_80043040(s16);
extern void func_80017168(void *, s32);
extern void func_80071824(void *, void *);
extern void func_80011D74(void *, s32, s16, s16);
extern s32 D_80124868;
extern s32 D_80124838;
extern ControllerPakMenuState D_8010AF90;
extern u8 D_8010AF93;
extern s16 D_8011217C;
extern void func_8003048C(void);
extern void func_80030CC4(void);
extern void func_80030EF0(void);
void func_80031038(ControllerPakTitleActor *);
extern void func_8003112C(void);
void func_80031294(ControllerPakTitleActor *);
extern void func_80031370(void);
extern void func_80031550(void);
extern void func_80031F40(void);
extern void func_8003209C(void);
void func_80032534(ControllerPakDeletePromptActor *);
extern void func_800716E4(void *);

#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_menu_ui/func_8002FFD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_menu_ui/func_8003048C.s")

void func_80030570(ControllerPakTitleActor *arg0) {
    arg0->common.x = -0x70;
    arg0->common.y = -0x28;
    arg0->selectedOption = 0;
    arg0->blinkState = 0;
    arg0->scale = 0x100;
    arg0->timer = 0;
    func_80071824(arg0, func_8003048C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_menu_ui/func_800305B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_menu_ui/func_80030CC4.s")

void func_80030EA8(ControllerPakConfirmActor *arg0) {
    arg0->common.x = -0x70;
    arg0->common.y = -0x1C;
    arg0->state = 6;
    arg0->scale = 0;
    arg0->timer = 0;
    arg0->targetScale = 0;
    func_80071824(arg0, func_80030CC4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_menu_ui/func_80030EF0.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_menu_ui/func_8003112C.s")

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
    func_80011D74(&arg0->sprite, 0, arg0->common.x, arg0->common.y);
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
    func_80011D74(&arg0->sprite, 1, arg0->common.x, arg0->common.y);
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

#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_menu_ui/func_80031D3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_menu_ui/func_80031F40.s")

void func_8003205C(ControllerPakDeletePromptActor *arg0) {
    arg0->common.x = -0x70;
    arg0->common.y = -0x1C;
    arg0->timer = 0;
    arg0->scale = 0;
    arg0->selectedOption = 0;
    func_80071824(arg0, func_80031F40);
}

#pragma GLOBAL_ASM("asm/nonmatchings/controller_pak_menu_ui/func_8003209C.s")

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
