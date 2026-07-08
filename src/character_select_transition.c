#include "common.h"
#include "effect_task_scheduler.h"
#include "character_select_flow.h"
#include "character_select_transition.h"

extern s32 func_80013F88(s32, s32, s32);
extern void func_80045914(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);

extern CharacterSelectFlowState *D_801235B8;
extern s8 D_800DEED4;
extern u8 D_80123750;
extern u8 D_80123751;

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_transition/func_80006F30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_transition/func_80007840.s")

void func_8000854C(void) {
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
            func_80099658(2);
            func_8009954C(4);
        }
    }
}
