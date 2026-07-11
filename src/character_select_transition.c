#include "common.h"
#include "effect_task_scheduler.h"
#include "character_select_flow.h"
#include "character_select_transition.h"
#include "input_task_scheduler.h"
#include "title_menu.h"

extern s32 func_80013F88(s32, s32, s32);
extern void func_80045914(void);

extern CharacterSelectFlowState *D_801235B8;
extern s8 D_800DEED4;
extern u8 D_80123750;
extern u8 D_80123751;

typedef struct {
    char pad0[0x3F];
    s8 characterState[12];
    char pad4B[0x78AD];
} CharacterSelectTransitionSaveData;

typedef struct {
    char pad0[5];
    u8 mode;
    u8 selection;
    char pad7[0x605];
} CharacterSelectTransitionPlayer;

typedef struct {
    u8 unk0[4];
    u8 unk4[4];
    u8 unk8[4];
    u8 unkC[4];
    u8 unk10[4];
    s16 unk14[4];
    s16 unk1C[4];
    u8 unk24[4];
    u8 unk28;
    char pad29;
    s16 unk2A;
    s16 unk2C;
    u8 unk2E;
} CharacterSelectTransitionState2;

extern s16 func_80042D58(s32);
extern s32 func_80043040(s16);
extern void func_800437F0(void *, void *, s32);
extern void func_8006D5CC(void);
extern void func_800704F0(void);
extern void func_8007066C(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_800720E4(s32);
extern void func_80099C44(void *, void *, s32);
extern void n_alSeqpDelete(void);

extern u8 D_14B450[];
extern u8 D_1502A0[];
extern u8 D_1EF530[];
extern u8 D_1F1A90[];
extern u8 D_245A80[];
extern u8 D_24C8E0[];
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_59AAA0[];
extern u8 D_59DFE0[];
extern u8 D_59E7F0[];
extern u8 D_5A1ED0[];
extern u8 D_5C5320[];
extern u8 D_5CBA80[];
extern u8 D_5CCD40[];
extern s16 D_800DEF14;
extern s8 D_800EC9C0;
extern u8 D_800EC9C2;
extern s16 D_800EC9D0[];
extern s8 D_800EC9E5;
extern s8 D_800EC9E6;
extern CharacterSelectTransitionSaveData D_800EC9F0[];
extern s32 D_8010ADDC;
extern s32 D_8010ADE0;
extern s32 D_8010ADE4;
extern s16 D_8010ADF0[];
extern s8 D_8010AE64[];
extern u8 D_8010AEA0[];
extern s8 D_8010AEA4[];
extern s8 D_8010AEA8;
extern s8 D_8010AEAC[];
extern s8 D_8010AEB0;
extern s8 D_8010AEB8[][3];
extern u8 D_8010AEC8[];
extern s8 D_8010AECC[];
extern s16 D_8010AED0;
extern s32 D_8010AED8[];
extern s32 D_8010AEE8[];
extern u8 D_8010AEF8[][4];
extern s8 D_8010AEFB[];
extern u8 D_8010AF08[][3];
extern CharacterSelectTransitionState2 D_8010AF18;
extern s16 D_80112130[];
extern s32 D_80112204;
extern void (*D_8011220C)(void);
extern s32 D_801122B4;
extern void (*D_801122BC)(void);
extern s32 D_80112364;
extern void (*D_8011236C)(void);
extern s32 D_80112414;
extern void (*D_8011241C)(void);
extern u8 D_80121B55;
extern CharacterSelectTransitionPlayer D_80121D80[];
extern CharacterSelectTransitionPlayer D_801235B0;
extern s32 D_801235B4;

// func_80006F30 best match: 78.174% (nonmatchings/func_80006F30-7273315160691878794/base_1.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_transition/func_80006F30.s")

#ifdef NON_MATCHING
void func_80006F30(void) {
    s32 size;
    s32 i;
    s32 j;
    s32 k;
    s32 sum;
    s32 selected;
    s32 screenBase;

    if (D_800EC9C2 == 1) {
        func_800720E4(2);
    }
    func_8006D5CC();
    func_800704F0();

    switch (D_80121B55) {
    case 1:
        func_8007066C(0, 0xE8, 0x78, 0x90, 0xD0, 0xA0, 0xF0, 0.6666666865f);
        screenBase = 0xA40000;
        break;
    case 2:
        func_8007066C(0, 0xE4, 0x4A, 0x84, 0x54, 0xA8, 0x74, 1.448275805f);
        func_8007066C(1, 0xE4, 0xAE, 0x84, 0x54, 0xA8, 0x74, 1.448275805f);
        screenBase = 0x894000;
        break;
    case 3:
    case 4:
        func_8007066C(0, 0x78, 0x44, 0x44, 0x40, 0x60, 0x60, 1.333333373f);
        func_8007066C(1, 0x78, 0xA8, 0x44, 0x40, 0x60, 0x60, 1.333333373f);
        func_8007066C(2, 0x104, 0x44, 0x44, 0x40, 0x60, 0x60, 1.333333373f);
        func_8007066C(3, 0x104, 0xA8, 0x44, 0x40, 0x60, 0x60, 1.333333373f);
        screenBase = 0x894000;
        break;
    default:
        break;
    }

    D_8011220C = n_alSeqpDelete;
    D_80112204 = screenBase;
    D_8010AEA0[0] = 0;
    D_8010AEAC[0] = 0;
    D_801122BC = n_alSeqpDelete;
    D_801122B4 = screenBase;
    D_8010AEA0[1] = 0;
    D_8010AEAC[1] = 0;
    D_8011236C = n_alSeqpDelete;
    D_80112364 = screenBase;
    D_8010AEA0[2] = 0;
    D_8010AEAC[2] = 0;
    D_8011241C = n_alSeqpDelete;
    D_80112414 = screenBase;
    D_8010AEA0[3] = 0;
    D_8010AEAC[3] = 0;
    D_800DEED4 = 0;
    D_800EC9E5 = 0;
    D_8010AEA8 = 0;
    D_8010AEB0 = 0;
    D_801235B8->fade = 0xFF;
    D_800DEF14 = D_801235B8->fade;

    func_800437F0(D_5CBA80, D_5CCD40, 0x21);
    func_800437F0(D_593D10, D_598A70, 0x22);
    func_800437F0(D_598A70, D_59AAA0, 0x23);
    func_800437F0(D_59AAA0, D_59DFE0, 0x24);
    func_800437F0(D_5A1ED0, D_5C5320, 0x25);
    func_800437F0(D_59DFE0, D_59E7F0, 0x26);

    size = D_1502A0 - D_14B450;
    D_80112130[0xC] = func_80042D58(size);
    func_80099C44(D_14B450, (void *)func_80043040(D_80112130[0xC]), size);
    func_800437F0(D_1EF530, D_1F1A90, 0xD);
    func_800437F0(D_245A80, D_24C8E0, 0x1F);
    func_80070EC0(0);
    func_80071408((void (*)(EffectTask *))func_8001710C, 0, 0x63);

    D_800EC9C0 = 0;
    D_8010ADDC = 0;
    D_8010ADE0 = 0;
    D_8010ADE4 = 0;
    D_801235B4 = 0;
    D_8010AED0 = 0;
    D_800EC9E6 = 0;
    D_801235B8->timer = 0;
    D_801235B8->unk20 = 0;
    func_8009956C(func_80007840, 0);

    for (i = 0; &D_80121D80[i] < &D_801235B0; i++) {
        D_80121D80[i].mode = 0;
    }

    for (i = 0; i < D_80121B55; i++) {
        D_8010AEA4[i] = 0;
        D_8010AEC8[i] = 0;
        D_8010AECC[i] = 0;
        D_800EC9D0[i] = 0;
        D_8010ADF0[i] = 0;
        D_8010AED8[i] = 0;
        D_8010AEE8[i] = 0;

        for (j = 0; j < 3; j++) {
            sum = 0;
            for (k = 0; k < 3; k++) {
                sum += D_800EC9F0[i].characterState[j + k * 3];
            }
            D_8010AEB8[i][j] = (sum != -3);
        }

        sum = 0;
        for (j = 9; j < 12; j++) {
            sum += D_800EC9F0[i].characterState[j];
        }
        if (sum >= -2) {
            D_8010AEC8[i] = 4;
        }
        if (D_8010AEC8[i] == 4) {
            D_8010AEA0[i] = 1;
        }

        for (j = 0; j < 3; j++) {
            D_8010AEF8[i][j] = j;
        }

        if (D_8010AEA0[i] == 1) {
            for (j = 9; j < 12; j++) {
                if (D_800EC9F0[i].characterState[j] != -1) {
                    D_8010AEFB[i * 4] = j;
                    break;
                }
            }

            k = 0;
            for (j = 9; j < 12; j++) {
                D_8010AF08[i][k] = 0;
                if (D_800EC9F0[i].characterState[j] != -1) {
                    D_8010AF08[i][k] = j;
                    k++;
                }
            }
        } else {
            D_8010AEFB[i * 4] = 0;
        }
    }

    for (i = 0; i < D_80121B55; i++) {
        selected = D_80121D80[i].selection;
        if ((selected >= 9) && (selected < 12)) {
            j = 3;
            if (D_80121D80[i].mode == 5) {
                j = 0;
            }
            k = D_8010AF18.unk2E;
        } else {
            k = D_8010AF18.unk2E;
            if (k == 1) {
                j = 0;
                D_8010AF18.unk2E = 0;
                k = 0;
            } else {
                j = selected % 3;
            }
        }

        D_8010AE64[i] = j;
        if (k == 1) {
            j--;
        }

        if (D_80121D80[i].mode == 5) {
            D_8010AEAC[i] = 0;
        } else if (selected < 9) {
            D_8010AEAC[i] = selected / 3;
        } else {
            if (selected >= 12) {
                D_8010AEAC[i] = 0;
            }
            if (selected >= 9 && selected < 12) {
                for (k = 0; k < 3; k++) {
                    if (D_8010AF08[i][k] == selected) {
                        D_8010AEAC[i] = k;
                        break;
                    }
                }
            }
        }

        D_80121D80[i].selection = D_8010AEF8[i][j];
        D_8010AF18.unk0[i] = 0;
        D_8010AF18.unk4[i] = 0;
        D_8010AF18.unk8[i] = 0;
        D_8010AF18.unkC[i] = 0;
        D_8010AF18.unk10[i] = 0;
        D_8010AF18.unk14[i] = 0;
        D_8010AF18.unk1C[i] = 0;
        D_8010AF18.unk24[i] = 0;
    }

    D_8010AF18.unk28 = 0;
    D_8010AF18.unk2A = 0;
    D_8010AF18.unk2C = 0;
}
#endif

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
