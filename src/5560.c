#include "common.h"

typedef struct {
    char pad[0x18];
    s32 unk18;
    s32 unk1C;
} Struct801235B8;

extern s32 func_80013F88(s32, s32, s32);
extern void func_8007105C(void);
extern void func_80045914(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);

extern Struct801235B8 *D_801235B8;
extern s8 D_800DEED4;
extern s32 D_801235B4;
extern u8 D_80123750;
extern u8 D_80123751;

#pragma GLOBAL_ASM("asm/nonmatchings/5560/func_80004960.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5560/func_80004CC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5560/func_80005290.s")

void func_80005468(void) {
    if (D_801235B8->unk18 != 0xFF) {
        D_801235B8->unk18 = func_80013F88((s16) D_801235B8->unk18, 0x24, 1);
        if (D_801235B8->unk18 == 0xFF) {
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
