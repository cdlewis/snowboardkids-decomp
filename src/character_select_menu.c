#include "common.h"
#include "effect_task_scheduler.h"
#include "character_select_menu.h"

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
    s16 characterId;
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
extern void func_80099658(s32);
extern void func_8009954C(s32);
extern void func_80072138(s32, s32);
extern void func_8009956C(void (*)(), s32);
extern void func_80004CC8(void);
extern void func_80005468(void);

extern CharacterSelectMenuState *D_801235B8;
extern CharacterSelectState D_8010AE50;
extern CharacterId D_800B3400[];
extern CharacterSelectPlayer D_80121D80[];
extern s8 D_800DEED4;
extern s32 D_801235B4;
extern s32 D_80123778;
extern u8 D_80121B55;
extern u8 D_80123750;
extern u8 D_80123751;

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_menu/func_80004960.s")

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_menu/func_80004CC8.s")

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
