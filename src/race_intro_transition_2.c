#include "race_intro_transition.h"

typedef struct {
    /* 0x000 */ s16 playerIndex;
    /* 0x002 */ char pad2[2];
    /* 0x004 */ u8 unk4;
    /* 0x005 */ char pad5[0xB];
    /* 0x010 */ u8 characterId;
    /* 0x011 */ u8 unk11;
    /* 0x012 */ u8 unk12;
    /* 0x013 */ s8 isActive;
    /* 0x014 */ s8 soundDisabled;
    /* 0x015 */ s8 unk15;
    /* 0x016 */ s8 replayInputSource;
    /* 0x017 */ char pad17[0x5F5];
} RaceIntroPlayer;

extern RaceIntroTransitionState *D_801235B8;
extern RaceIntroPlayer D_80121D80[];
extern s16 D_800DEF14;
extern u8 D_80123750;
extern s8 D_800EC8B0;
extern s8 D_800EC9C2;
extern s8 D_800DEED4;
extern u8 D_8011228C;
extern s8 D_8010B1E0;
extern s8 D_8010B1E1;
extern s16 D_80121B50;
extern s16 D_80121B52;
extern s8 D_80121B54;
extern u8 D_80121B55;
extern u8 D_80121B56;
extern u8 D_80121B58;
extern u8 D_80121B59;
extern u8 D_80121B5A;
extern u8 D_80121B5B;
extern s16 D_80121B5C;
extern u8 D_80121B5F;
extern s8 D_80121D94;
extern s8 D_80121D95;
extern s8 D_80121D96;
extern s8 D_801223A0;
extern s8 D_801223A1;
extern s8 D_801223A2;
extern s8 D_801229AC;
extern s8 D_801229AD;
extern s8 D_801229AE;
extern s8 D_80122FB8;
extern s8 D_80122FB9;
extern s8 D_80122FBA;
extern u8 D_80123751;
extern s32 D_801235B4;
extern u8 D_24C8E0;
extern u8 D_24DBE0;
extern void func_800437F0(void *, void *, s32);
extern void func_80043950(void);
extern void func_800440F4(void);
extern void func_80044294(void);
extern void func_8006D5CC(void);
extern void func_8007066C(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_80070EC0(s32);
extern void func_800720E4(s32);
extern void func_80078430(void);
extern void func_8008BEB0(void);
extern void func_8009956C(TaskCallback, s32);
extern void func_80045914(void);
extern void func_80072260(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);
extern void func_8003F00C(void);

// func_8003ED00 best match: 99.874% (nonmatchings/func_8003ED00-5635509610426229442/base_2.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_intro_transition_2/func_8003ED00.s")

#ifdef NON_MATCHING
void func_8003ED00(void) {
    RaceIntroPlayer *players;
    register s32 one = 1;
    s32 active = 1;
    s32 two = 2;
    s32 three = 3;
    s32 four = 4;
    s32 six = 6;
    s32 five0 = 5;
    s32 five1 = 5;
    s32 five2 = 5;
    s32 five3 = 5;

    D_80121B50 = 0;
    D_80121B56 = 0;
    D_80121B58 = 0;
    D_80121B5F = 0;
    D_80121B59 = one;
    D_80121B55 = four;
    D_800EC9C2 = 0;
    D_80121B5A = 0;
    D_80121B5B = 0;

    players = D_80121D80;
    players[0].unk4 = 0;
    players[1].unk4 = 0;
    players[2].unk4 = 0;
    players[3].unk4 = 0;
    players[0].characterId = 0;
    players[1].characterId = one;
    players[2].characterId = two;
    players[3].characterId = three;
    players[0].unk11 = six;
    players[1].unk11 = six;
    players[2].unk11 = six;
    players[3].unk11 = six;
    players[0].unk12 = one;
    players[1].unk12 = two;
    players[2].unk12 = three;
    players[3].unk12 = four;
    players[0].isActive = active;
    players[1].isActive = active;
    players[2].isActive = active;
    players[3].isActive = active;

    D_80121B54 = four;
    D_80121B52 = two;
    D_80121B5C = 0x64;
    func_80070EC0(1);

    D_80121D95 = 0;
    D_80121D94 = 0;
    D_80121D96 = five0;
    D_801223A1 = 0;
    D_801223A0 = 0;
    D_801223A2 = five1;
    D_801229AD = 0;
    D_801229AC = 0;
    D_801229AE = five2;
    D_80122FB9 = 0;
    D_80122FB8 = 0;
    players = D_80121D80;
    D_80122FBA = five3;

    if (players[0].replayInputSource == 5) {
        func_800437F0(&D_24C8E0, &D_24DBE0, 0x2B);
    }
    func_80043950();
    func_800440F4();
    D_801235B4 = 0;
    func_8006D5CC();
    func_800704F0();
    func_8007066C(0, 0xA0, 0x78, 0x120, 0xB0, 0x140, 0xF0, D_800E10C0);
    D_8011228C = 1;
    D_800DEED4 = 0;
    func_8008BEB0();
    func_80078430();
    func_80044294();
    func_8009956C(func_8003EF7C, 0);
    D_800DEF14 = 0xFF;
    D_800EC8B0 = 0;
    D_801235B8->fadeDelay = 0x4A1;
    D_801235B8->fadeStep = 0;
    D_801235B8->courseSegment = 0;
    D_801235B8->startDelay = 0x14;
    D_8010B1E1 = 0;
    D_8010B1E0 = 0;
}
#endif

void func_8003EF7C(void) {
    D_801235B8->startDelay--;
    if (D_801235B8->startDelay == 0) {
        func_8007066C(0, 0xA0, 0x78, 0x120, 0, 0x140, 0xF0, 1.333333373f);
        D_800DEF14 = 0;
        func_800720E4(0);
        func_8009956C(func_8003F00C, 0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_intro_transition_2/func_8003F00C.s")

void func_8003F4B4(void) {
    if (D_80123750 == 2) {
        D_800EC8B0 = 0;
        D_80121B58 = 0;
        func_80045914();
        D_80123751 = 0;
        D_800DEED4 = 0;
        func_80072260();
        D_801235B4 = 0;
        func_80099658(3);
        func_8009954C(4);
    }
}
