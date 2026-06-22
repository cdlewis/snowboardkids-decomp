#include "common.h"

typedef struct {
    char pad[0x18];
    s32 unk18;
    s32 unk1C;
} Struct801235B8;

extern s32 func_80013F88(s32, s32, s32);
extern void func_80072138(s32, s32);
extern void func_8009956C(void *, s32);
extern void func_8007105C(void);
extern void func_80045914(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);

extern void func_8000C924(void);

extern Struct801235B8 *D_801235B8;
extern s8 D_800DEED4;
extern u8 D_800EC9D8;
extern u8 D_8010AF93;
extern u8 D_8010AF96;
extern s32 D_80123778;
extern u8 D_80123750;
extern u8 D_80123751;

#pragma GLOBAL_ASM("asm/nonmatchings/D200/func_8000C600.s")

#pragma GLOBAL_ASM("asm/nonmatchings/D200/func_8000C818.s")

#pragma GLOBAL_ASM("asm/nonmatchings/D200/func_8000C924.s")

#pragma GLOBAL_ASM("asm/nonmatchings/D200/func_8000CB08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/D200/func_8000CC5C.s")

void func_8000CE0C(void) {
    s32 state;

    state = D_8010AF96;
    if (state == 1) {
        if ((D_80123778 & 0x8000) || (D_80123778 & 0x1000)) {
            func_80072138(0x18, 0x32);
            D_8010AF96 = 2;
            state = 2;
        }
    }
    if (state == 3) {
        D_800EC9D8 = 0;
        func_8009956C(&func_8000C924, 0);
        D_8010AF93 = 1;
    }
    func_8007105C();
}

void func_8000CEA0(void) {
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
            func_80099658(0);
            func_8009954C(4);
        }
    }
}
