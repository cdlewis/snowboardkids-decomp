#include "common.h"

typedef struct {
    u8 pad0[0x10];
    s16 unk10;
} Struct71AC0;

typedef struct Struct8007105C {
    struct Struct8007105C *unk0;
    struct Struct8007105C *unk4;
    void (*unk8)(struct Struct8007105C *);
    u8 padC[0x8];
    s16 unk14;
} Struct8007105C;

extern Struct8007105C *D_80121848;

void *func_800711D0(void *, s32, s32);
void *func_80071408(void *, s32, s32);

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_80070EC0.s")

extern Struct8007105C *D_80112784;

// Mirrors the linked list head D_80112784 into the global cursor D_80121848,
// then for each node clears unk14 and invokes its unk8 callback with a pointer
// to itself, advancing via unk4. Assigning through the captured store
// (`s0 = (D_80121848 = D_80112784)`) and clearing unk14 via a temp steers IDO's
// register allocator to match the target's temp-register choice (t6/t7).
void func_8007105C(void) {
    Struct8007105C *s0;
    s32 new_var;

    s0 = (D_80121848 = D_80112784);
    if (D_80121848 != NULL) {
        do {
            new_var = 0;
            D_80121848->unk14 = new_var;
            D_80121848->unk8(D_80121848);
            s0 = D_80121848->unk4;
            D_80121848 = s0;
        } while (D_80121848 != NULL);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_800710CC.s")

// Drains the global cursor list (D_80121848): for each node, clears unk14 and
// invokes its unk8 callback with a pointer to itself, then advances via unk4.
// The empty `if ((s0 && s0) && s0) {}` is a dead condition that IDO 5.3 folds to
// zero instructions, but which steers the allocator into keeping the cursor in
// $s0 and emitting `move $a0, $s0` for the callback argument (matching the
// target). Without it IDO folds the reload straight into $a0.
void func_8007115C(void) {
    Struct8007105C *s0 = D_80121848;

    if (s0 != NULL) {
        do {
            s0->unk14 = 0;
            s0 = D_80121848;
            if ((s0 && s0) && s0) {}
            s0->unk8(s0);
            s0 = D_80121848->unk4;
            D_80121848 = s0;
        } while (s0 != NULL);
    }
}

void func_800711C8(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_800711D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_80071408.s")

void *func_80071664(void *arg0, s32 arg1, s32 arg2, s32 arg3){ void *t=func_80071408(arg0,arg1&0xFFFF,arg2); if(t!=NULL){((Struct71AC0 *)t)->unk10=arg3;} return t;}

void *func_800716A4(void *arg0, s32 arg1, s32 arg2, s32 arg3){ void *t=func_800711D0(arg0,arg1&0xFFFF,arg2); if(t!=NULL){((Struct71AC0 *)t)->unk10=arg3;} return t;}

#pragma GLOBAL_ASM("asm/nonmatchings/71AC0/func_800716E4.s")

void func_80071824(void *arg0, void (*arg1)(void)) {
    *(void (**)(void))((u8 *)arg0 + 8) = arg1;
}
