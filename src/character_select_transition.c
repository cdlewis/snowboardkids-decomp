#include "common.h"
#include "effect_task_scheduler.h"
#include "character_select_flow.h"
#include "character_select_transition.h"
#include "input_task_scheduler.h"

extern s32 func_80013F88(s32, s32, s32);
extern void func_80045914(void);

extern CharacterSelectFlowState *D_801235B8;
extern s8 D_800DEED4;
extern u8 D_80123750;
extern u8 D_80123751;

#pragma GLOBAL_ASM("asm/nonmatchings/character_select_transition/func_80006F30.s")

// func_80007840 best match: 10.769% (nonmatchings/func_80007840-8207005055717715604/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_transition/func_80007840.s")

#ifdef NON_MATCHING
typedef struct {
    /* 0x00 */ u8 pad0[5];
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ s8 unk7;
    /* 0x08 */ u8 state;
    /* 0x09 */ u8 pad9[0x603];
} CharacterSelectTransitionEntry;

typedef struct CharacterSelectTransitionObject {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ void (*update)(void);
    /* 0x30 */ u8 pad30[0x80];
} CharacterSelectTransitionObject;

extern void func_800257F0(EffectTask *);
extern void func_80028194(EffectTask *);
extern void func_80028B0C(EffectTask *);
extern void func_8002916C(EffectTask *);
extern void *func_80071408(void (*)(EffectTask *), s32, s32);
extern void func_80072114(s32);
extern void func_8009956C(void (*)(void), s32);

extern void *D_8010ADE8;
extern s32 D_8010AEE8[];
extern CharacterSelectTransitionObject *D_800EC9C4;
extern CharacterSelectTransitionObject D_801121E0[];
extern CharacterSelectTransitionObject D_801124A0;
extern s16 D_800EC9D0[];
extern u8 D_800EC9C0;
extern CharacterSelectTransitionEntry D_80121D80[];
extern s32 D_801235B4;

void func_80007840(void) {
    CharacterSelectTransitionObject *obj;
    CharacterSelectTransitionEntry *entry;
    s32 i;
    s32 count;
    s32 ready;
    s32 selectionCount;
    u8 state;

    if (D_801235B8->fade != 0) {
        D_801235B8->fade = func_80013F88((s16) D_801235B8->fade, 0x24, 0);
        if (D_801235B8->fade == 0) {
            if (D_80121B55 == 1) {
                func_80071408(func_800257F0, 0, 0x63);
            } else {
                func_80071408(func_80028B0C, 0, 0x62);
                func_80071408(func_8002916C, 0, 0x63);
            }
            D_8010ADE8 = func_80071408(func_80028194, 0, 0x61);
        }
    } else {
        count = D_80121B55;
        ready = 0;
        entry = D_80121D80;
        for (i = 0; i < count; i++, entry++) {
            if (entry->state == 9) {
                ready++;
            }
        }

        if (count == ready) {
            selectionCount = 0;
            for (i = 0; i < count; i++) {
                state = D_800EC9D0[i];
                if ((D_8010AEE8[i] != 0) || (state == 1) || (state == 9) || (state >= 5)) {
                    selectionCount++;
                }
            }

            if (selectionCount == 0) {
                selectionCount = count < 3 ? count : 4;
                for (i = 0; i < selectionCount; i++) {
                    D_80121D80[i].state = 3;
                }
                D_800EC9C0 = 0xF;
                D_801235B4 = 1;
            }
        } else if (D_800EC9C0 != 0) {
            D_800EC9C0++;
        }
    }

    if (D_800EC9C0 == 0x19) {
        func_8009956C(func_8000854C, 0);
        if (D_801235B4 == 0) {
            func_80072114(8);
        }
    }

    obj = D_801121E0;
    do {
        D_800EC9C4 = obj;
        obj->update();
        obj++;
    } while (obj != &D_801124A0);
    func_8007105C();
}
#endif

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
