#include "common.h"

typedef struct {
    /* 0x000 */ u16 unk0;
    /* 0x002 */ char pad2[0x2];
    /* 0x004 */ u8 unk4;
    /* 0x005 */ char pad5[0xF];
    /* 0x014 */ s8 unk14;
    /* 0x015 */ char pad15[0x5EF];
    /* 0x604 */ s16 unk604;
    /* 0x606 */ s16 unk606;
    /* 0x608 */ s8 unk608;
} Struct800830C0;

extern u8 D_800DE550[];
extern u8 D_800DE560[];
extern u8 D_800DE570[];
extern u8 D_800DE578[];
extern void func_80000C00(u16);

void func_800830C0(Struct800830C0 *arg0) {
    s32 v0;
    s32 v1;
    u8 *p;

    if (arg0->unk4 != 0) {
        return;
    }
    if (arg0->unk14 != 0) {
        return;
    }
    if (arg0->unk604 == 0) {
        arg0->unk606 = 0;
        return;
    }
    arg0->unk604 = arg0->unk604 - 1;
    v0 = arg0->unk608;
    v1 = 2;
    p = D_800DE550;
    if (v0 == 1) {
        p = D_800DE560;
    }
    if (v1 == v0) {
        p = D_800DE570;
    }
    if (v0 == 3) {
        p = D_800DE578;
    }
    if (v1 == v0) {
        if (!(arg0->unk606 < 6)) {
            arg0->unk606 = 0;
        }
    }
    if (p[arg0->unk606] != 0) {
        func_80000C00(arg0->unk0);
    }
    arg0->unk606 = (arg0->unk606 + 1) & 0xF;
}
