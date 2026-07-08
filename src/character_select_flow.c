#include "common.h"
#include "game_audio.h"
#include "effect_task_scheduler.h"
#include "character_select_flow.h"
#include "character_select_ui.h"

typedef s16 CharacterSelectOptionList[10];

extern void func_8009956C(void *, s32);
extern s32 func_80013F88(s32, s32, s32);
extern void func_80072138(s32, s32);
extern void func_80045914(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);

extern void func_80008620(void);
extern void func_80005540(void);
extern CharacterSelectFlowState *D_801235B8;
extern s8 D_800DEED4;
extern u8 D_80121D88;
extern u8 D_80121B55;
extern s16 D_80121B50;
extern CharacterSelectOptionList *D_8010AE90;
extern s32 D_801235B4;
extern s32 D_80123758;
extern s32 D_80123778;
extern u16 D_8010ADF0;
extern u16 D_8010AE80;
extern u8 D_8010AE88;
extern u8 D_8010AE89;
extern u16 D_8010AE8C;
extern u8 D_8010ADF8;
extern u8 D_800EC9C1;
extern u8 D_800EC9DD;
extern u8 D_80123750;
extern u8 D_80123751;

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_flow/func_80005C70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_flow/func_80005F18.s")

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_flow/func_800062F8.s")

// func_800066CC best match: 70.727% (nonmatchings/func_800066CC-2911448260736516995/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_flow/func_800066CC.s")

#ifdef NON_MATCHING
void func_800066CC(void) {
    s32 temp_input;
    s32 pressed;
    s32 previousSelection;
    s32 heldInput;
    u16 repeatTimer;

    if (D_801235B8->fade != 0) {
        D_801235B8->fade = func_80013F88((s16) D_801235B8->fade, 0x24, 0);
        if (D_801235B8->fade == 0) {
            func_80071408(&func_8001C83C, 0, 0x63);
        }
    } else {
        if (D_80121D88 == 0) {
            if (D_800EC9C1 == 0) {
                if (D_8010AE88 == 1) {
                    previousSelection = D_80121B50;
                    temp_input = D_80123758;
                    pressed = temp_input & 0x10800;
                    if ((pressed == 0) && ((temp_input & 0x20400) == 0)) {
                        D_8010ADF0 = 0;
                    }

                    heldInput = D_80123778;
                    repeatTimer = D_8010ADF0;
                    if ((heldInput & 0x10800) == 0) {
                        if (((pressed != 0) && (repeatTimer >= 0xB)) && ((repeatTimer % 3) == 0)) {
                            if (repeatTimer == 0) {
                                repeatTimer++;
                            }
                            if (D_80121B50 > 0) {
                                D_80121B50--;
                            }
                            D_8010ADF0 = repeatTimer;
                        } else if (((heldInput & 0x20400) != 0) ||
                                   ((((temp_input & 0x20400) != 0) && (repeatTimer >= 0xB)) &&
                                    ((repeatTimer % 3) == 0))) {
                            D_8010ADF0 = repeatTimer;
                            if (repeatTimer == 0) {
                                D_8010ADF0 = repeatTimer + 1;
                            }
                            if ((*D_8010AE90)[D_80121B50] != -1) {
                                D_80121B50++;
                            }
                        } else {
                            D_8010ADF0 = repeatTimer;
                        }
                    } else {
                        if (repeatTimer == 0) {
                            repeatTimer++;
                        }
                        if (D_80121B50 > 0) {
                            D_80121B50--;
                        }
                        D_8010ADF0 = repeatTimer;
                    }

                    repeatTimer = D_8010ADF0;
                    if (repeatTimer != 0) {
                        repeatTimer++;
                        D_8010ADF0 = repeatTimer;
                        if (repeatTimer == 0xFFFF) {
                            D_8010ADF0 = 0xC;
                        }
                    }

                    if (previousSelection != D_80121B50) {
                        func_80072138(0x19, 0x32);
                    }

                    temp_input = D_80123778;
                    if (((temp_input & 0x1000) || (temp_input & 0x8000)) &&
                        (D_801235B4 == (D_8010AE80 + 1))) {
                        func_80072138(1, 0x32);
                        if ((*D_8010AE90)[D_80121B50] != -1) {
                            D_800EC9C1 = 1;
                            D_8010AE88 = 2;
                            D_8010AE8C = 0x100;
                        } else {
                            D_8010AE88 = 2;
                            D_8010AE8C = 0x100;
                            D_80121D88 = 7;
                            func_8009956C(&func_80006D70, 0);
                            func_80072114(8);
                        }
                    } else if ((temp_input & 0x4000) && (D_801235B4 == (D_8010AE80 + 1))) {
                        func_80072138(1, 0x32);
                        D_8010AE88 = 2;
                        D_8010AE8C = 0x100;
                        D_80121D88 = 7;
                        func_8009956C(&func_80006D70, 0);
                        func_80072114(8);
                    }
                }
            } else {
                D_800EC9C1++;
            }

            if (D_800EC9C1 == 8) {
                if ((*D_8010AE90)[D_80121B50] == -1) {
                    D_80121D88 = 2;
                    func_8009956C(&func_80006D70, 0);
                    func_80072114(8);
                } else {
                    D_80121D88 = 1;
                }
            }
        }

        if (D_8010AE89 >= 2) {
            func_8009956C(&func_80006AE8, 0);
        }
    }

    D_801235B4 = 0;
    func_8007105C();
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_flow/func_80006AE8.s")

void func_80006D70(void) {
    if (D_80121D88 == 8) {
        if (D_80121B55 >= 2) {
            func_8009956C(&func_80006E3C, 0);
            D_801235B4 = 1;
            D_8010ADF8 = 1;
        } else {
            if (D_800EC9DD == 0) {
                func_8009956C(&func_80005540, 0);
            } else {
                func_8009956C(&func_80008620, 0);
            }
            D_80121B50 = (*D_8010AE90)[D_80121B50];
            D_801235B4 = 0;
        }
    }
    func_8007105C();
}

void func_80006E3C(void) {
    if (D_801235B8->fade != 0xFF) {
        D_801235B8->fade = func_80013F88((s16) D_801235B8->fade, 0x24, 1);
        if (D_801235B8->fade == 0xFF) {
            D_80123751 = 1;
        } else {
            func_8007105C();
        }
    } else {
        if (D_80123750 == 2) {
            func_80045914();
            D_80123751 = 0;
            D_800DEED4 = 0;
            D_80121B50 = (*D_8010AE90)[D_80121B50];
            func_80099658(2);
            func_8009954C(4);
        }
    }
}
