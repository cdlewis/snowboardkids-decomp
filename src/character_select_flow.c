#include "common.h"
#include "game_audio.h"
#include "effect_task_scheduler.h"
#include "asset_decompression.h"
#include "character_select_flow.h"
#include "character_select_ui.h"
#include "input_task_scheduler.h"
#include "menu_rendering.h"
#include "player_count_select_menu.h"
#include "title_menu.h"

typedef s16 CharacterSelectOptionList[10];
typedef s16 CharacterSelectCourseUnlockList[11];

typedef struct {
    char pad0[0x4C];
    u8 highestCourse;
    char pad4D[0x78AB];
} CharacterSelectSaveData;

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

extern void func_80072138(s32, s32);
extern void func_80045914(void);
extern void func_800704F0(void);
extern void func_8007066C(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_80005540(void);
extern u8 D_245A80[];
extern u8 D_24C8E0[];
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_59AAA0[];
extern u8 D_59DFE0[];
extern u8 D_59E7F0[];
extern u8 D_5A1ED0[];
extern u8 D_5C5320[];
extern u8 D_5CCD40[];
extern u8 D_5D4280[];
extern CharacterSelectCourseUnlockList D_800B3420[];
extern s16 D_800B3478[];
extern s16 D_800B3480[];
extern CharacterSelectFlowState *D_801235B8;
extern s8 D_800DEED4;
extern f32 D_800E09A4;
extern s16 D_800DEF14;
extern CharacterSelectSaveData D_800EC9F0[];
extern u8 D_80121D80[];
extern u8 D_80121D88;
extern u8 D_80121B55;
extern u8 D_80121B5E;
extern s16 D_80121B50;
extern CharacterSelectOptionList *D_8010AE90;
extern s32 D_8010ADDC;
extern s32 D_8010ADE0;
extern s32 D_8010ADE4;
extern s32 D_801235B4;
extern s32 D_80123758;
extern s32 D_80123778;
#ifndef NON_MATCHING
extern u16 D_8010ADF0;
#else
extern volatile u16 D_8010ADF0;
#endif
extern u16 D_8010AE80;
extern CharacterSelectCursorState D_8010AE88;
extern u8 D_8010AE89;
extern u8 D_8010ADF8;
extern u8 D_8010ADF9;
extern u8 D_800EC9C1;
extern u8 D_800EC9C2;
extern s16 D_800EC9D0;
extern u8 D_800EC9DD;
extern u8 D_80123750;
extern u8 D_80123751;

// func_80005C70 best match: 79.600% (nonmatchings/func_80005C70-8207005055717715604/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_flow/func_80005C70.s")

#ifdef NON_MATCHING
void func_80005C70(void) {
    s32 sp1C;
    s16 *var_v0_2;
    s16 *var_v1_2;
    s32 var_v1;
    s32 var_v0_3;
    u8 temp_v1;
    CharacterSelectSaveData *var_v0;
    CharacterSelectSaveData *temp_a0;

    D_801235B8->fade = 0;
    func_800720E4(2);
    func_80071408((void (*)(EffectTask *)) func_8001D254, 0, 0x63);
    D_801235B8->timer = 0;
    D_800EC9C1 = 0;
    D_8010ADF8 = 0;
    D_80121D88 = 0;
    D_8010ADF0 = 0;
    D_800EC9D0 = 0;
    D_800DEF14 = D_801235B8->fade;
    var_v1 = 0;
    if (D_80121B55 > 0) {
        var_v0 = D_800EC9F0;
        temp_a0 = &D_800EC9F0[D_80121B55];
        do {
            temp_v1 = var_v0->highestCourse;
            var_v0 += 1;
            if (D_8010ADF9 < temp_v1) {
                D_8010ADF9 = temp_v1;
            }
        } while (var_v0 < temp_a0);
        var_v1 = 0;
    }

    D_8010ADDC = 0;
    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    sp1C = var_v1;
    func_8009956C(func_800066CC, 0);
    func_8007105C();
    var_v1 = sp1C;
    if (D_800EC9DD == 1) {
        if (D_80121B5E < 2) {
            D_8010AE90 = (CharacterSelectOptionList *) D_800B3478;
            if ((D_80121B50 != 9) && (D_80121B50 != 0) && (D_80121B50 != 1)) {
                D_80121B50 = 9;
            }
        } else {
            D_8010AE90 = (CharacterSelectOptionList *) D_800B3480;
            D_80121B50 = 7;
        }
    } else {
        D_8010AE90 = (CharacterSelectOptionList *) D_800B3420[D_8010ADF9];
        if (D_80121B50 == -1) {
            D_80121B50 = 9;
        }
    }

    var_v0_2 = *D_8010AE90;
loop_20:
    if (D_80121B50 == *var_v0_2) {
        D_80121B50 = var_v1;
    } else {
        var_v1 += 1;
        var_v0_2 += 1;
        if (var_v1 != 0xA) {
            goto loop_20;
        }
    }

    D_8010AE80 = 0;
    var_v0_3 = 0;
    var_v1_2 = *D_8010AE90;
loop_24:
    var_v0_3 += 2;
    if (*var_v1_2 != -1) {
        var_v1_2 += 1;
        D_8010AE80 += 1;
        if (var_v0_3 != 0x14) {
            goto loop_24;
        }
    }

    D_8010AE88.bytes[0] = 0;
    D_8010AE88.bytes[1] = 0;
    D_8010AE88.bytes[2] = 0;
    D_8010AE88.fields.spriteIndex = 0;
    D_8010AE88.bytes[6] = 0;
    D_8010AE88.bytes[7] = 0;
}
#endif

// func_80005F18 best match: 84.441% (nonmatchings/func_80005F18-8207005055717715604/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_flow/func_80005F18.s")

#ifdef NON_MATCHING
void func_80005F18(void) {
    s32 sp2C;
    s16 *var_v0_2;
    s16 *var_v1_2;
    s32 var_v1;
    s32 var_v0_3;
    u8 temp_v1;
    CharacterSelectSaveData *var_v0;
    CharacterSelectSaveData *temp_a0;

    func_800720E4(2);
    func_800704F0();
    func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
    D_800DEED4 = 0;
    func_800437F0(D_5A1ED0, D_5C5320, 0x21);
    func_800437F0(D_593D10, D_598A70, 0x22);
    func_800437F0(D_598A70, D_59AAA0, 0x23);
    func_800437F0(D_59AAA0, D_59DFE0, 0x24);
    func_800437F0(D_59DFE0, D_59E7F0, 0x26);
    func_800437F0(D_245A80, D_24C8E0, 0x1F);
    func_80070EC0(0);
    func_80071408((void (*)(EffectTask *))func_8001710C, 0, 0x5E);

    if (D_800EC9C2 == 1) {
        func_800437F0(D_5CCD40, D_5D4280, 0x25);
        func_80071408((void (*)(EffectTask *)) func_8001D254, 0, 0x63);
        D_801235B8->fade = 0;
    } else {
        D_801235B8->fade = 0xFF;
    }

    D_801235B8->timer = 0;
    D_800EC9C1 = 0;
    D_8010ADF8 = 0;
    D_801235B4 = 0;
    D_80121D88 = 0;
    D_8010ADF0 = 0;
    D_800EC9D0 = 0;
    D_800DEF14 = D_801235B8->fade;
    var_v1 = 0;
    if (D_80121B55 > 0) {
        var_v0 = D_800EC9F0;
        temp_a0 = &D_800EC9F0[D_80121B55];
        do {
            temp_v1 = var_v0->highestCourse;
            var_v0 += 1;
            if (D_8010ADF9 < temp_v1) {
                D_8010ADF9 = temp_v1;
            }
        } while (var_v0 < temp_a0);
    }

    D_8010ADDC = 0;
    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    sp2C = var_v1;
    func_8009956C(func_800066CC, 0);
    var_v1 = sp2C;
    if (D_800EC9DD == 1) {
        if (D_80121B5E < 2) {
            D_8010AE90 = (CharacterSelectOptionList *) D_800B3478;
            if ((D_80121B50 != 9) && (D_80121B50 != 0) && (D_80121B50 != 1)) {
                D_80121B50 = 9;
            }
        } else {
            D_8010AE90 = (CharacterSelectOptionList *) D_800B3480;
            D_80121B50 = 7;
        }
    } else {
        D_8010AE90 = (CharacterSelectOptionList *) D_800B3420[D_8010ADF9];
        if (D_80121B50 == -1) {
            D_80121B50 = 9;
        }
    }

    var_v0_2 = *D_8010AE90;
loop_20:
    if (D_80121B50 == *var_v0_2) {
        D_80121B50 = var_v1;
    } else {
        var_v1 += 1;
        var_v0_2 += 1;
        if (var_v1 != 0xA) {
            goto loop_20;
        }
    }

    D_8010AE80 = 0;
    var_v0_3 = 0;
    var_v1_2 = *D_8010AE90;
loop_24:
    var_v0_3 += 2;
    if (*var_v1_2 != -1) {
        var_v1_2 += 1;
        D_8010AE80 += 1;
        if (var_v0_3 != 0x14) {
            goto loop_24;
        }
    }

    D_8010AE88.bytes[0] = 0;
    D_8010AE88.bytes[1] = 0;
    D_8010AE88.bytes[2] = 0;
    D_8010AE88.fields.spriteIndex = 0;
    D_8010AE88.bytes[6] = 0;
    D_8010AE88.bytes[7] = 0;
    func_8007105C();
}
#endif

// func_800062F8 best match: 83.266% (nonmatchings/func_800062F8-2127290767680699791/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_flow/func_800062F8.s")

#ifdef NON_MATCHING
void func_800062F8(void) {
    s32 sp2C;
    s16 *var_v0_2;
    s16 *var_v1_2;
    s32 var_v1;
    s32 var_v0_3;
    u8 temp_v1;
    CharacterSelectSaveData *var_v0;
    CharacterSelectSaveData *temp_a0;

    func_800720E4(2);
    if (D_80121B55 >= 2) {
        func_800704F0();
        func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, D_800E09A4);
        D_800DEED4 = 0;
        func_800437F0(D_5A1ED0, D_5C5320, 0x21);
        func_800437F0(D_593D10, D_598A70, 0x22);
        func_800437F0(D_598A70, D_59AAA0, 0x23);
        func_800437F0(D_59AAA0, D_59DFE0, 0x24);
        func_800437F0(D_59DFE0, D_59E7F0, 0x26);
        func_800437F0(D_245A80, D_24C8E0, 0x1F);
        func_80070EC0(0);
        func_80071408((void (*)(EffectTask *))func_8001710C, 0, 0x5E);
        D_801235B8->fade = 0xFF;
    } else {
        D_801235B8->fade = 0;
        func_80071408(func_8001C83C, 0, 0x63);
    }

    D_801235B8->timer = 0;
    D_800EC9C1 = 0;
    D_8010ADF8 = 0;
    D_801235B4 = 0;
    D_80121D88 = 0;
    D_8010ADF0 = 0;
    D_800EC9D0 = 0;
    D_800DEF14 = D_801235B8->fade;
    D_8010ADDC = 0;
    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    var_v1 = 0;
    if (D_80121B55 > 0) {
        var_v0 = D_800EC9F0;
        temp_a0 = &D_800EC9F0[D_80121B55];
        do {
            temp_v1 = var_v0->highestCourse;
            var_v0 += 1;
            if (D_8010ADF9 < temp_v1) {
                D_8010ADF9 = temp_v1;
            }
        } while (var_v0 < temp_a0);
    }

    sp2C = var_v1;
    func_8009956C(func_800066CC, 0);
    var_v1 = sp2C;
    if (D_800EC9DD == 1) {
        if (D_80121B5E < 2) {
            D_8010AE90 = (CharacterSelectOptionList *) D_800B3478;
            if ((D_80121B50 != 9) && (D_80121B50 != 0) && (D_80121B50 != 1)) {
                D_80121B50 = 9;
            }
        } else {
            D_8010AE90 = (CharacterSelectOptionList *) D_800B3480;
            D_80121B50 = 7;
        }
    } else {
        D_8010AE90 = (CharacterSelectOptionList *) D_800B3420[D_8010ADF9];
        if (D_80121B50 == -1) {
            D_80121B50 = 9;
        }
    }

    var_v0_2 = *D_8010AE90;
loop_20:
    if (D_80121B50 == *var_v0_2) {
        D_80121B50 = var_v1;
    } else {
        var_v1 += 1;
        var_v0_2 += 1;
        if (var_v1 != 0xA) {
            goto loop_20;
        }
    }

    D_8010AE80 = 0;
    var_v0_3 = 0;
    var_v1_2 = *D_8010AE90;
loop_24:
    var_v0_3 += 2;
    if (*var_v1_2 != -1) {
        var_v1_2 += 1;
        D_8010AE80 += 1;
        if (var_v0_3 != 0x14) {
            goto loop_24;
        }
    }

    D_8010AE88.bytes[0] = 0;
    D_8010AE88.bytes[1] = 0;
    D_8010AE88.bytes[2] = 0;
    D_8010AE88.fields.spriteIndex = 0;
    D_8010AE88.bytes[6] = 0;
    D_8010AE88.bytes[7] = 0;
    func_8007105C();
}
#endif

// func_800066CC best match: 72.688% (nonmatchings/func_800066CC-2785870559185086986/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_flow/func_800066CC.s")

#ifdef NON_MATCHING
void func_800066CC(void) {
    s32 temp_input;
    s32 pressed;
    s32 previousSelection;
    s32 heldInput;
    s32 selection;
    u16 repeatTimer;

    if (D_801235B8->fade != 0) {
        D_801235B8->fade = func_80013F88((s16) D_801235B8->fade, 0x24, 0);
        if (D_801235B8->fade == 0) {
            func_80071408(&func_8001C83C, 0, 0x63);
        }
    } else {
        if (D_80121D88 == 0) {
            if (D_800EC9C1 == 0) {
                if (D_8010AE88.fields.state == 1) {
                    selection = D_80121B50;
                    previousSelection = (s16) selection;
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
                            if (selection > 0) {
                                D_80121B50 = selection - 1;
                                selection = D_80121B50;
                            }
                            D_8010ADF0 = repeatTimer;
                        } else if (((heldInput & 0x20400) != 0) ||
                                   ((((temp_input & 0x20400) != 0) && (repeatTimer >= 0xB)) &&
                                    ((repeatTimer % 3) == 0))) {
                            D_8010ADF0 = repeatTimer;
                            if (repeatTimer == 0) {
                                D_8010ADF0 = repeatTimer + 1;
                            }
                            if ((*D_8010AE90)[selection] != -1) {
                                D_80121B50 = selection + 1;
                                selection = D_80121B50;
                            }
                        } else {
                            D_8010ADF0 = repeatTimer;
                        }
                    } else {
                        if (repeatTimer == 0) {
                            repeatTimer++;
                        }
                        if (selection > 0) {
                            D_80121B50 = selection - 1;
                            selection = D_80121B50;
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

                    if (previousSelection != selection) {
                        func_80072138(0x19, 0x32);
                    }

                    temp_input = D_80123778;
                    if (((temp_input & 0x1000) || (temp_input & 0x8000)) &&
                        (D_801235B4 == (D_8010AE80 + 1))) {
                        func_80072138(1, 0x32);
                        if ((*D_8010AE90)[D_80121B50] != -1) {
                            D_800EC9C1 = 1;
                            D_8010AE88.fields.state = 2;
                            D_8010AE88.fields.spriteIndex = 0x100;
                        } else {
                            D_8010AE88.fields.state = 2;
                            D_8010AE88.fields.spriteIndex = 0x100;
                            D_80121D88 = 7;
                            func_8009956C(&func_80006D70, 0);
                            func_80072114(8);
                        }
                    } else if ((temp_input & 0x4000) && (D_801235B4 == (D_8010AE80 + 1))) {
                        func_80072138(1, 0x32);
                        D_8010AE88.fields.state = 2;
                        D_8010AE88.fields.spriteIndex = 0x100;
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

void func_80006AE8(void) {
    s32 input;
    int state;

    state = D_80121D80[8];
    if (state < 3) {
        switch (D_8010AE88.fields.otherState) {
        case 2:
            input = D_80123778;
            if (input & 0x4000) {
                D_80121D80[8] = 3;
                func_80072138(1, 0x32);
            } else if ((input & 0x8000) || (input & 0x1000)) {
                func_80072138(1, 0x32);
                D_800EC9D0 = 1;
                D_8010AE88.fields.otherState = 3;
                func_80071408(func_800227A0, 0, 0x61);
            }
            break;
        case 3:
            if (D_800EC9D0 >= 3) {
                input = D_80123778;
                if ((input & 0x10800) && (D_800EC9D0 != 3)) {
                    D_800EC9D0--;
                    func_80072138(0x19, 0x32);
                    input = D_80123778;
                } else if ((input & 0x20400) && (D_800EC9D0 != 4)) {
                    D_800EC9D0++;
                    func_80072138(0x19, 0x32);
                    input = D_80123778;
                }

                if ((input & 0x8000) || (input & 0x1000)) {
                    func_80072138(0x18, 0x32);
                    D_800EC9D0 += 2;
                    D_8010AE88.fields.otherState = 4;
                } else if (input & 0x4000) {
                    func_80072138(1, 0x32);
                    D_800EC9D0 = 6;
                    D_8010AE88.fields.otherState = 4;
                }
            }
            break;
        case 4:
            if (D_8010AE88.fields.otherTimer == 4) {
                if (D_800EC9D0 == 6) {
                    D_80121D80[8] = 3;
                } else {
                    D_80121D80[8] = 7;
                }
                D_800EC9D0 = 0;
            }
            break;
        }
    } else {
        if (state == 6) {
            D_80121D80[8] = 0;
            D_800EC9C1 = 0;
            func_8009956C(func_800066CC, 0);
            D_8010AE88.fields.state = 1;
            D_8010AE88.fields.spriteIndex = 0x100;
            D_8010AE88.fields.timer = 0;
            state = D_80121D88;
        }
        if (state == 8) {
            func_8009956C(func_80006E3C, 0);
        }
    }
    func_8007105C();
}

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
