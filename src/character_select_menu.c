#include "common.h"
#include "effect_task_scheduler.h"
#include "asset_decompression.h"
#include "character_select_menu.h"
#include "input_task_scheduler.h"

typedef struct {
    char pad[0x18];
    s32 fade;
    s32 timer;
} CharacterSelectMenuState;

typedef struct {
    u8 phase;
    u8 exitMode;
    u8 readyCount;
    u8 pad3;
    char pad4[4];
    s16 fade;
    s16 unkA;
    u8 confirmSelection;
    u8 unkD;
    u8 blockDecrease;
    u8 blockIncrease;
    s16 cursorX;
    s16 cursorY;
    s8 playerSelections[4];
} CharacterSelectState;

typedef struct {
    u16 characterId;
} CharacterId;

typedef struct {
    char pad0[5];
    u8 characterId;
    char pad6[2];
    u8 isCpu;
    char pad9[0x603];
} CharacterSelectPlayer;

extern s32 func_80013F88(s32, s32, s32);
extern void func_80045914(void);
extern void func_80072138(s32, s32);
extern void func_80004CC8(void);
extern void func_80005290(void);
extern void func_80005468(void);
extern void func_800704F0(void);
extern void func_8007066C(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_800720E4(s32);
extern void func_800179D4(EffectTask *);
extern void func_80017D08(EffectTask *);
extern void func_80018060(EffectTask *);
extern void func_800183DC(EffectTask *);
extern void func_80018B6C(EffectTask *);
extern void func_800191A0(EffectTask *);

extern CharacterSelectMenuState *D_801235B8;
extern CharacterSelectState D_8010AE50;
extern CharacterId D_800B3400[];
extern CharacterSelectPlayer D_80121D80[];
extern EffectTask *D_8010ADDC;
extern EffectTask *D_8010ADE0;
extern EffectTask *D_8010ADE4;
extern EffectTask *D_8010ADE8;
extern EffectTask *D_8010ADEC;
extern f32 D_800E0980;
extern s16 D_800DEF14;
extern s16 D_8010ADF0[];
extern s8 D_800DEED4;
extern s8 D_800EC9C1;
extern s16 D_800B340C[];
extern s16 D_800EC9C8;
extern s8 D_8010AE53;
extern s8 D_8010AE54;
extern s8 D_8010AE55;
extern s8 D_8010AE56;
extern s8 D_8010AE64[];
extern s32 D_801235B4;
extern s32 D_80123778;
extern u8 D_8010AE5E;
extern u8 D_8010AE5F;
extern u8 D_245A80[];
extern u8 D_24C8E0[];
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_59AAA0[];
extern u8 D_59DFE0[];
extern u8 D_60F1A0[];
extern u8 D_60F990[];
extern u8 D_800B3190;
extern u8 D_800EC9E5;
extern u8 D_8010ADF8;
extern u8 D_8010AE68;
extern u8 D_80121B55;
extern u8 D_80123750;
extern u8 D_80123751;

// func_80004960 best match: 90.714%
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_menu/func_80004960.s")

#ifdef NON_MATCHING
void func_80004960(void) {
    CharacterSelectPlayer *player;
    CharacterId *character;
    CharacterSelectState *state;
    s32 i;
    s32 j;
    s32 playerCount;
    u8 *selectionPtr;
    s16 *timerPtr;
    s16 *timerEnd;

    if ((D_800EC9E5 == 0) || (D_8010ADF8 == 1)) {
        func_800720E4(1);
        func_800704F0();
        func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, D_800E0980);
        D_800DEED4 = 0;
        D_800EC9C1 = 0;
        D_8010ADF8 = 0;
        D_801235B8->fade = 0xFF;
        D_801235B8->timer = 0;
        D_800DEF14 = D_801235B8->fade;
        func_800437F0(D_59AAA0, D_59DFE0, 0x21);
        func_800437F0(D_245A80, D_24C8E0, 0x1F);
        func_800437F0(D_593D10, D_598A70, 0x22);
        func_800437F0(D_598A70, D_59AAA0, 0x23);
        func_800437F0(D_60F1A0, D_60F990, 0x29);
        func_80070EC0(0);

        playerCount = D_80121B55;
        i = 0;
        if (playerCount > 0) {
            player = D_80121D80;
            do {
                character = D_800B3400;
                j = 0;
loop_1:
                if (player->characterId == character->characterId) {
                    D_8010AE64[i] = j;
                } else {
                    j++;
                    character++;
                    if (j != 6) {
                        goto loop_1;
                    }
                }
                i++;
                player++;
            } while (i < playerCount);
        }
    } else {
        D_801235B8->fade = 1;
        D_800DEF14 = 1;
        playerCount = D_80121B55;
        i = 0;
        if (playerCount > 0) {
            selectionPtr = (u8 *) D_8010AE64;
            player = D_80121D80;
            do {
                j = i + 1;
                player->characterId = i;
                i = j;
                player++;
                selectionPtr++;
                selectionPtr[-1] = j;
            } while (j < playerCount);
        }
    }

    D_8010ADDC = func_80071408(func_800179D4, 0, 0x64);
    D_8010ADEC = func_80071408(func_800191A0, 0, 0x64);
    D_8010ADE8 = func_80071408(func_800183DC, 0, 0x64);
    D_8010ADE0 = func_80071408(func_80018060, 0, 0x64);
    func_80071408(func_80018B6C, 0, 0x64);
    D_8010ADE4 = func_80071408(func_80017D08, 0, 0x63);
    func_8009956C(func_80004CC8, 0);

    playerCount = D_80121B55;
    if (playerCount > 0) {
        timerPtr = D_8010ADF0;
        timerEnd = timerPtr + playerCount;
        player = D_80121D80;
        do {
            timerPtr++;
            player++;
            player[-1].isCpu = 0;
            timerPtr[-1] = 0;
        } while ((u32) timerPtr < (u32) timerEnd);
    }

    state = &D_8010AE50;
    state->phase = 0;
    state->exitMode = 0;
    state->readyCount = 0;
    state->fade = 0;
    state->unkA = 0;
    state->confirmSelection = 0;
    state->unkD = 0;
    state->blockDecrease = 0;
    state->blockIncrease = 0;
    state->cursorX = 0;
    state->cursorY = 0;
    D_8010AE53 = 0;
    D_8010AE54 = 0;
    D_8010AE55 = 0;
    D_8010AE56 = 0;
    D_8010AE68 = 0;
    if (D_800B3190 != 0) {
        D_8010AE68 = 1;
    }
    func_8007105C();
}
#endif

// func_80004CC8 best match: 66.871% (nonmatchings/func_80004CC8/base_final.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_menu/func_80004CC8.s")

#ifdef NON_MATCHING
void func_80004CC8(void) {
    u8 sp6A;
    u8 sp68;
    s16 sp64;
    void *sp54;
    u8 *readyBytePtr;
    s32 sp50;
    u8 cachedIsCpu;
    s32 sp48;
    s32 sp40;
    s16 *charSfxTable;
    CharacterSelectPlayer *var_s4;
    CharacterSelectState *var_v1_3;
    s16 *temp_t1;
    s16 *var_v0;
    s16 temp_t7;
    s16 temp_t8;
    s16 temp_v1;
    s16 var_ra;
    volatile int savedRa;
    s32 *var_s7;
    s32 temp_a1;
    s32 temp_s6;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a2;
    s32 var_s0;
    s32 var_s1;
    s32 var_t1;
    s32 var_v0_2;
    s32 var_v0_3;
    s32 var_v1_2;
    u8 *temp_fp;
    s8 temp_t2;
    s8 temp_v0_4;
    u16 temp_v0_3;
    u16 temp_v1_2;
    u16 var_v1;
    u8 temp_a3;
    u8 temp_t6;
    u8 var_t5;
    void *temp_v1_3;
    extern s32 D_80123758[];

    temp_v0 = D_801235B8->fade;
    if (temp_v0 != 0) {
        D_801235B8->fade = func_80013F88((s32) (s16) temp_v0, 0x24, 0);
        if (D_801235B8->fade == 0) {
            D_8010AE50.phase = 1;
        }
    } else if ((s32) D_8010AE50.phase < 3) {
        if ((D_8010AE50.phase == 1) && (D_8010AE50.fade == 0x100) && ((D_80123778 & 0x8000) || (D_80123778 & 0x1000))) {
            D_8010AE50.phase = 2;
            func_80072138(1, 0x32);
            var_v0 = &D_800EC9C8;
            if ((s32) D_80121B55 > 0) {
                do {
                    temp_v1 = *var_v0;
                    if ((temp_v1 == 0x12) || (temp_v1 == 5)) {
                        *var_v0 = 0x13;
                    }
                    var_v0 += 1;
                } while ((u32) var_v0 < (u32) (&D_800EC9C8 + D_80121B55));
            }
        }
    } else if (D_8010AE50.readyCount == 1) {
        var_s1 = 1;
        var_ra = 5;
        if (D_8010AE50.blockDecrease != 0) {
            var_s1 = 0;
        }
        var_s4 = D_80121D80;
        if (D_8010AE50.blockIncrease != 0) {
            var_ra = 6;
        }
        temp_a3 = D_80121B55;
        var_s0 = 0;
        var_a0 = 0;
        if ((s32) D_80121B55 > 0) {
            var_t5 = sp6A;
            var_s7 = (s32 *) &D_80123778;
            do {
                cachedIsCpu = var_s4->isCpu;
                sp48 = var_a0;
                if (cachedIsCpu == 0) {
                    temp_fp = (u8 *) &D_8010AE64[var_s0];
                    temp_t2 = *temp_fp;
                    temp_v0_2 = D_80123758[var_s0];
                    temp_a1 = temp_v0_2 & 0x40100;
                    temp_t1 = &D_8010ADF0[var_s0];
                    var_a0_2 = temp_t2 & 0xFF;
                    sp50 = (s32) temp_t2;
                    charSfxTable = D_800B340C;
                    if ((temp_a1 == 0) && !(temp_v0_2 & 0x80200)) {
                        *temp_t1 = 0;
                    }
                    temp_s6 = *var_s7;
                    temp_v0_3 = (u16) *temp_t1;
                    if ((temp_s6 & 0x80200) || ((temp_v0_2 & 0x80200) && ((s32) temp_v0_3 >= 0xB) && (temp_v0_3 & 1))) {
                        temp_v1_2 = (u16) *temp_t1;
                        if (temp_v1_2 == 0) {
                            *temp_t1 = temp_v1_2 + 1;
                        }
                        if (var_s1 < var_a0_2) {
                            var_a0_2 = (var_a0_2 - 1) & 0xFF;
                            var_t5 = 1;
                        }
                    }
                    var_v1 = (u16) *temp_t1;
                    if ((temp_s6 & 0x40100) || ((temp_a1 != 0) && ((s32) var_v1 >= 0xB) && (var_v1 & 1))) {
                        if (var_v1 == 0) {
                            temp_t8 = var_v1 + 1;
                            *temp_t1 = temp_t8;
                            var_v1 = temp_t8 & 0xFFFF;
                        }
                        if (var_a0_2 < var_ra) {
                            var_a0_2 = (var_a0_2 + 1) & 0xFF;
                            var_t5 = 2;
                        }
                    }
                    temp_t7 = var_v1 + 1;
                    if (var_v1 != 0) {
                        *temp_t1 = temp_t7;
                        if ((temp_t7 & 0xFFFF) == 0xFFFF) {
                            *temp_t1 = 0xC;
                        }
                    }
                    if (D_8010AE68 == 0) {
                        var_t1 = 0;
                        var_v1_2 = 0;
loop_48:
                        var_v0_2 = 0;
                        if ((s32) D_80121B55 > 0) {
                            do {
                                if ((var_v0_2 != var_s0) && (var_a0_2 == D_8010AE64[var_v0_2])) {
                                    var_v1_2 = (var_v1_2 + 1) & 0xFF;
                                }
                                var_v0_2 += 1;
                            } while (var_v0_2 < (s32) D_80121B55);
                        }
                        var_t1 += 1;
                        if ((D_8010AE5E == 0) && (var_a0_2 == 0)) {
                            var_v1_2 = (var_v1_2 + 1) & 0xFF;
                        }
                        if ((D_8010AE5F == 0) && (var_a0_2 == 6)) {
                            var_v1_2 = (var_v1_2 + 1) & 0xFF;
                        }
                        if (var_v1_2 != 0) {
                            if (var_t5 == 1) {
                                var_a0_2 = (var_a0_2 - 1) & 0xFF;
                            } else {
                                var_a0_2 = (var_a0_2 + 1) & 0xFF;
                            }
                            var_a2 = var_a0_2;
                            if (var_a0_2 < var_s1) {
                                var_a0_2 = temp_t2 & 0xFF;
                                var_a2 = var_a0_2;
                            }
                            if (var_ra < var_a2) {
                                var_a0_2 = temp_t2 & 0xFF;
                            }
                        }
                        if (var_v1_2 != 0) {
                            var_v1_2 = 0;
                            if (var_t1 == 7) {

                            } else {
                                goto loop_48;
                            }
                        }
                    }
                    if (var_a0_2 != sp50) {
                        sp64 = var_ra;
                        sp40 = var_a0_2;
                        sp6A = var_t5;
                        func_80072138(0x19, 0x32);
                        var_ra = (s16) (u16) sp64;
                    }
                    *temp_fp = (s8) var_a0_2;
                    temp_v0_4 = *temp_fp;
                    var_s4->characterId = (u8) D_800B3400[temp_v0_4].characterId;
                    if (((*var_s7 & 0x8000) || (*var_s7 & 0x1000)) && (var_s4->isCpu == 0)) {
                        temp_v1_3 = (u8 *) &D_8010AE50 + var_s0;
                        sp64 = var_ra;
                        savedRa = (s16) (u16) sp64;
                        sp54 = temp_v1_3;
                        sp6A = var_t5;
                        func_80072138((s32) charSfxTable[temp_v0_4], 0x32);
                        var_ra = savedRa;
                        var_s4->isCpu = 1;
                        ((u8 *) temp_v1_3)[3] = 1;
                    }
                }
                var_a0 = sp48 + 4;
                if ((*var_s7 & 0x4000) && (cachedIsCpu != 0)) {
                    var_s4->isCpu = 0;
                    (&D_8010AE53)[var_s0] = 0;
                }
                var_s0 += 1;
                var_s4 = (CharacterSelectPlayer *) ((u8 *) var_s4 + 0x60C);
                var_s7 += 1;
            } while (var_s0 < (s32) D_80121B55);
            sp6A = var_t5;
            sp68 = var_s4[-1].isCpu & 1;
        }
        if (sp68 != 0) {
            var_v0_3 = 0;
            if ((s32) D_80121B55 > 0) {
                var_v1_3 = &D_8010AE50;
                do {
                    readyBytePtr = (u8 *) var_v1_3;
                    temp_t6 = readyBytePtr[3];
                    var_v1_3 = (CharacterSelectState *) ((u8 *) var_v1_3 + 1);
                    var_v0_3 += temp_t6;
                } while ((u32) var_v1_3 < (u32) ((u8 *) &D_8010AE50 + D_80121B55));
            }
            if (var_v0_3 == (D_80121B55 * 3)) {
                D_8010AE50.phase = 4;
                D_8010AE50.confirmSelection = 0;
                D_8010AE50.unkD = 0;
                D_8010AE50.unkA = 0x100;
                func_8009956C(func_80005290, 0);
            }
        }
    }
    func_8007105C();
}
#endif

// func_80005290 best match: 85.264%
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_menu/func_80005290.s")

#ifdef NON_MATCHING
void func_80005290(void) {
    CharacterSelectState *state;
    CharacterSelectPlayer *player;
    u8 *readyPtr;
    u8 *readyEnd;
    s32 oldSelection;
    s32 buttons;
    s32 selection;
    u8 playerCount;

    state = &D_8010AE50;
    if (state->fade == 0x100) {
        buttons = D_80123778;
        selection = state->confirmSelection;
        oldSelection = selection;

        if ((buttons & 0x10800) && (selection != 0)) {
            state->confirmSelection = selection - 1;
            selection = (u8) (selection - 1);
        } else if (buttons & 0x20400) {
            if (selection == 0) {
                state->confirmSelection = selection + 1;
                selection = (u8) (selection + 1);
            }
        }

        if (selection != oldSelection) {
            func_80072138(0x19, 0x32);
            buttons = D_80123778;
        }

        if ((buttons & 0x8000) || (buttons & 0x1000)) {
            func_80072138(0x18, 0x32);
            state = &D_8010AE50;
            if (state->confirmSelection == 0) {
                func_8009956C(func_80005468, 0);
                state = &D_8010AE50;
                state->cursorX = 0x8C;
                state->cursorY = 0x44;
                state->exitMode = 3;
            } else {
                playerCount = D_80121B55;
                player = D_80121D80;
                if ((s32) playerCount > 0) {
                    readyPtr = (u8 *) &D_8010AE50;
                    readyEnd = playerCount + readyPtr;
                    do {
                        readyPtr++;
                        player++;
                        player[-1].isCpu = 0;
                        readyPtr[2] = 0;
                    } while ((u32) readyPtr < (u32) readyEnd);
                }
                func_8009956C(func_80004CC8, 0);
                state = &D_8010AE50;
                state->phase = 3;
                state->fade = 0;
            }
        } else if (buttons & 0x4000) {
            func_80072138(0x18, 0x32);
            playerCount = D_80121B55;
            player = D_80121D80;
            if ((s32) playerCount > 0) {
                readyPtr = (u8 *) &D_8010AE50;
                readyEnd = playerCount + readyPtr;
                do {
                    readyPtr++;
                    player++;
                    player[-1].isCpu = 0;
                    readyPtr[2] = 0;
                } while ((u32) readyPtr < (u32) readyEnd);
            }
            func_8009956C(func_80004CC8, 0);
            state = &D_8010AE50;
            state->phase = 3;
            state->fade = 0;
        }
    }
    func_8007105C();
}
#endif

void func_80005468(void) {
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
            D_801235B4 = 0;
            func_80099658(2);
            func_8009954C(4);
        }
    }
}
