#include "common.h"

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
extern void func_8007105C(void);
extern void func_80045914(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);

extern CharacterSelectMenuState *D_801235B8;
extern CharacterSelectState D_8010AE50;
extern CharacterId D_800B3400[];
extern CharacterSelectPlayer D_80121D80[];
extern s8 D_800DEED4;
extern s32 D_801235B4;
extern u8 D_80123750;
extern u8 D_80123751;

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_menu/func_80004960.s")

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_menu/func_80004CC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_menu/func_80005290.s")

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
