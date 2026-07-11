#include "common.h"
#include "game_audio.h"
#include "effect_task_scheduler.h"
#include "memory_allocator.h"
#include "asset_decompression.h"
#include "character_select_flow.h"
#include "character_select_menu.h"
#include "character_select_transition.h"
#include "controller_pak_delete_flow.h"
#include "controller_pak_message_flow.h"
#include "controller_pak_menu_flow.h"
#include "course_select_menu.h"
#include "game_setup_menu.h"
#include "input_task_scheduler.h"
#include "main_menu.h"
#include "main_menu_overlay_effects.h"
#include "main_menu_panel_ui.h"
#include "player_count_select_menu.h"
#include "player_select_menu.h"
#include "race_camera.h"
#include "race_course_effects.h"
#include "title_menu.h"

#define COURSE_GRID_ENTRY_FREE -1
#define COURSE_GRID_ENTRY_END -2
#define COURSE_TIME_MINUTE 0x177000
#define COURSE_TIME_SECOND 0x6400
#define RACE_PLAYER_STATE_SIZE 0x60C
#define RACE_PLAYER_READY_FLAG 0x40
#ifdef NON_MATCHING
#define RACE_PLAYER_RESULT_VALUE(index) (*(s8 *)((u8 *)D_80121D80 + ((((((((index) << 2) - (index)) << 5) + (index)) << 2) - (index)) << 2) + 0x509))
#endif

typedef struct {
    /* 0x00 */ s16 status;
    /* 0x02 */ s16 courseId;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
} CourseGridEntry;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ u8 pad2[0x48 - 0x2];
} CourseSelectTableEntry;

typedef struct {
    /* 0x00 */ s8 minutes;
    /* 0x01 */ s8 seconds;
    /* 0x02 */ s16 fraction;
} RaceTime;

typedef union {
    s16 s;
    u16 u;
} SignedUnsignedShort;

typedef struct {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ s16 unk2;
} RaceFlowInitEntry;

typedef struct {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 pad2;
} RaceFlowResultEntry;

typedef struct {
    /* 0x0000 */ u8 pad0[0x4];
    /* 0x0004 */ s32 unk4[12];
    /* 0x0034 */ u8 pad34[0x4E - 0x34];
    /* 0x004E */ RaceFlowInitEntry unk4E[11][5];
    /* 0x012A */ RaceFlowInitEntry unk12A[11];
    /* 0x0156 */ RaceFlowInitEntry unk156[11][5];
    /* 0x0232 */ RaceFlowResultEntry unk232[9];
    /* 0x0256 */ u8 pad256[0x7756 - 0x256];
    /* 0x7756 */ s16 unk7756[11][5];
    /* 0x77C4 */ u8 unk77C4[11][5];
    /* 0x77FB */ u8 unk77FB[11][5];
    /* 0x7832 */ u8 unk7832[11][5];
    /* 0x7869 */ u8 unk7869[11][5];
    /* 0x78A0 */ u8 unk78A0[11][5];
} RaceFlowInitScratch;

typedef struct {
    /* 0x0000 */ u8 pad0[0x4E];
    /* 0x004E */ RaceTime timeTrial[5];
    /* 0x0062 */ u8 pad62[0x156 - 0x62];
    /* 0x0156 */ RaceTime raceTimes[5];
} TimeCourseView;

typedef struct {
    /* 0x0000 */ u8 pad0[0x7756];
    /* 0x7756 */ u16 values[5];
} TrickCourseView;

typedef struct {
    /* 0x0000 */ u8 pad0[0x7832];
    /* 0x7832 */ u8 values[5];
} ScoreCourseView;

typedef struct {
    /* 0x000 */ u8 pad0[0x04];
    /* 0x004 */ u8 unk4;
    /* 0x005 */ u8 unk5;
    /* 0x006 */ u8 unk6;
    /* 0x007 */ u8 unk7;
    /* 0x008 */ u8 pad8[0xC - 0x8];
    /* 0x00C */ s32 unkC;
    /* 0x010 */ u8 pad10[0x13 - 0x10];
    /* 0x013 */ u8 unk13;
    /* 0x014 */ s8 unk14;
    /* 0x015 */ u8 unk15;
    /* 0x016 */ u8 unk16;
    /* 0x017 */ u8 pad17[0x18 - 0x17];
    /* 0x018 */ s16 unk18;
    /* 0x01A */ u8 pad1A[0x2FC - 0x1A];
    /* 0x2FC */ s32 flags;
    /* 0x300 */ u8 pad300[0x502 - 0x300];
    /* 0x502 */ s16 courseId;
    /* 0x504 */ u8 pad504[0x509 - 0x504];
    /* 0x509 */ s8 result;
    /* 0x50A */ u8 pad50A[RACE_PLAYER_STATE_SIZE - 0x50A];
} RacePlayerState;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 fadeTimer;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 countdown;
} RaceFlowState;

typedef struct {
    /* 0x00 */ u8 pad0[0xAC];
    /* 0xAC */ u8 active;
    /* 0xAD */ u8 padAD[0xB0 - 0xAD];
} ViewportSlot;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad4[0x8 - 0x4];
    /* 0x08 */ s8 unk8;
} Unk80043040;

void func_800732C4(void);

extern CourseGridEntry *D_800DC490[];
extern u16 D_800DC5C0[];
extern CourseSelectTableEntry D_800B9542[];
extern RaceFlowState *D_801235B8;
extern RacePlayerState D_80121D80[];
extern s32 D_80121B40;
extern s32 D_80121B44;
extern s32 D_80121B48;
extern s32 D_801235B4;
extern s32 D_80123758;
extern s32 D_8012207C;
extern s32 D_80123778[];
extern void *D_80124868;
extern s16 D_80112130[];
extern s16 D_80112186;
extern s16 D_80112188;
extern s16 D_8011216E;
extern s16 D_801124B8;
extern SignedUnsignedShort D_80121B50;
extern s16 D_80121B52;
extern s16 D_800DEF14;
extern s8 D_800DEF10;
extern s8 D_800EC8B0;
extern RaceFlowInitScratch D_800EC9F0;
extern u8 D_80121B5E;
#ifdef NON_MATCHING
extern u8 D_8011228C;
extern u8 D_8011233C;
extern f32 D_800E16CC;
extern f32 D_800E16D0;
extern u8 D_59AAA0[];
extern s16 D_80121B5C;
extern u8 D_80121B5F;
extern u8 D_80121D94;
extern u8 D_80121D95;
extern u8 D_80121D96;
extern u8 D_801223A0;
extern u8 D_801223A1;
extern u8 D_801223A2;
extern u8 D_801229AC;
extern u8 D_801229AD;
extern u8 D_801229AE;
extern u8 D_80122FB8;
extern u8 D_80122FB9;
extern u8 D_80122FBA;
#endif
extern u8 D_2427D0[];
extern u8 D_243270[];
extern u8 D_80121B55;
extern u8 D_80121B57;
extern u8 D_80121B58;
extern u8 D_80121B5E;
extern s8 D_80121B5F;
extern u8 D_80121B60;
extern s8 D_80121B61;
extern RaceTime D_80121B74;
extern u8 D_80121B81;
extern u8 D_8012482A;
extern u8 D_80123750;
extern u8 D_80123751;
extern s8 D_800DEED4;
extern u8 D_800EC9C2;
extern u8 D_8010ADFA;
extern u8 D_800DC4C0;
extern u8 D_80121B56;
extern ViewportSlot D_801121E0[];
extern u8 D_1E74E0[];
extern u8 D_1EC0F0[];
extern u8 D_1EC4A0[];
extern u8 D_1EC810[];
extern u8 D_1ECD00[];
extern u8 D_1ED910[];
extern u8 D_1EDC80[];
extern u8 D_1EE060[];
extern u8 D_1EEEA0[];
extern u8 D_1EF1D0[];
extern u8 D_1EF530[];
extern u8 D_1F1A90[];
extern u8 D_1F2220[];
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_59DFE0[];
extern u8 D_59E7F0[];
extern u8 D_60F1A0[];
extern u8 D_60F990[];
extern s16 D_80122040;
extern s16 D_801222F4;

extern void func_8000C280(void);
extern void func_8000D340(void);
extern void func_8003DFD0(void);
extern s32 func_80040D94(void);
extern void func_80044294(void);
extern void func_80045914(void);
extern void func_80045A78(s32, s32, s32, s32);
extern void func_80046D68(s32, s32, s32, s32, s32);
extern void func_80057E60(EffectTask *);
extern void func_80065E90(EffectTask *);
extern void func_800483FC(void *, void (*)(s32), s32);
extern void func_8005E68C(void *);
extern void func_800704F0(void);
extern void func_8007066C(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_80070A70(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_80070614(s32);
extern void func_80072138(s32, s32);
extern void func_800733E0(void);
extern void func_800734A0(void);
extern void func_80073308(void);
extern void func_8007339C(void);
extern void func_80073434(void);
extern void func_800734E4(void);
extern void func_800735B4(void);
extern void func_800735F8(void);
extern void func_80073650(void);
extern void func_80073694(void);
extern void func_800736E0(void);
extern void func_80073738(void);
extern void func_8007377C(void);
extern void func_800737FC(void);
extern void func_8007389C(void);
extern void func_800738F4(void);
extern void func_80073944(void);
extern void func_80073988(void);
extern void func_800740C0(void);
extern void func_80074160(void);
extern void func_800747E8(void);
extern void func_80074960(void);
extern void func_80074C5C(void);
extern void func_80074F50(s32, s32, s32, s32, s32, s32, s32, f32, s32, s32, s32, s32, s32, s32, f32);
extern void func_80076054(void);
extern void func_80077AD4(void);
extern void func_80077B34(void);
extern void func_80078430(void);
extern void func_80077C4C(void);
extern void func_80077400(void);
extern void func_80077554(void);
extern void func_80077C94(void);
extern void func_80077CD4(void);
extern void func_80077DA0(void);
extern void func_80078078(void);
extern void func_80078198(void);
extern void func_800781FC(void);
extern void func_8007AA50(void);
extern void func_8008BEB0(void);
extern void func_8008C704(void);
extern void func_80096E3C(void);

void func_80072C88(void);
void func_80072D04(void);
void func_80072D54(void);
void func_80072D98(void);
void func_80072DDC(void);
void func_8007334C(void);
s32 func_80077D14(void);

void func_80072C30(void) {
    func_800437F0(D_2427D0, D_243270, 6);
    func_80070EC0(0);
    D_801235B8->fadeTimer = 0xA;
    func_8009956C(&func_80072C88, 0);
}

void func_80072C88(void) {
    D_801235B8->fadeTimer -= 1;
    if (D_801235B8->fadeTimer == 0) {
        func_80000A40(0);
        func_80000A40(1);
        func_80000A40(2);
        func_80000A40(3);
        func_8009956C(&func_80072D04, 0);
    }
}

void func_80072D04(void) {
    if (D_80123758 & 0x1000) {
        func_8009956C(&func_80072D54, 0);
    } else {
        func_8009956C(&func_80072D98, 0);
    }
}

void func_80072D54(void) {
    func_8009956C(&func_80072D98, 0);
    func_800994F4(4, &func_8000C600, 0x64);
    func_80099614(0);
}

void func_80072D98(void) {
    func_8009956C(&func_80072DDC, 0);
    func_800994F4(4, &func_8000CF70, 0x64);
    func_80099614(0);
}

void func_80072DDC(void) {
    func_800994F4(3, &func_80001C30, 0x64);
    func_8009954C(0);
}

void func_80072E10(void) {
    CourseGridEntry *entry = D_800DC490[D_80121B50.s];
    s32 count = 0;
loop:
    if (entry->status != COURSE_GRID_ENTRY_END) {
        entry->status = COURSE_GRID_ENTRY_FREE;
        entry++;
        count += 1;
        goto loop;
    }
    if (count != 0) {
        D_80112188 = func_80042D58(count * sizeof(RacePlayerState));
    }
}

void func_80072E98(void) {
    CourseGridEntry *entry = D_800DC490[D_80121B50.s];
    s32 i = 0;

loop:
    if (entry->status != COURSE_GRID_ENTRY_END) {
        if ((entry->status == COURSE_GRID_ENTRY_FREE) && (entry->courseId == D_80121D80[0].courseId)) {
            s32 *status = (s32 *)func_80043040(D_80112130[0x2B]);
            if (*status < 0x1194) {
                entry->status = *status;
                ((RacePlayerState *) func_80043040(D_80112130[0x2C]))[i] = D_80121D80[0];
            }
        }
        entry++;
        i++;
        goto loop;
    }
}

s32 func_80072FC4(void) {
    s32 count;
    CourseGridEntry *entry;

    entry = D_800DC490[D_80121B50.s];
    count = 0;
loop:
    if (entry->status != COURSE_GRID_ENTRY_END) {
        if (entry->status != COURSE_GRID_ENTRY_FREE) {
            D_80121D80[0] = ((RacePlayerState *)func_80043040(D_80112188))[count];
            *(s32 *)func_80043040(D_80112186) = entry->status;
            entry->status = COURSE_GRID_ENTRY_FREE;
            D_80121B40 = entry->unk4;
            D_80121B44 = entry->unk8;
            D_80121B48 = entry->unkC;
            func_8006D580(0, 7);
            return 1;
        }
        entry++;
        count += 1;
        goto loop;
    }
    return 0;
}

s32 func_800730EC(void) {
    CourseGridEntry *entry = D_800DC490[D_80121B50.s];

loop:
    if (entry->status != COURSE_GRID_ENTRY_END) {
        if (entry->status != COURSE_GRID_ENTRY_FREE) {
            return 1;
        }
        entry++;
        goto loop;
    }
    return 0;
}

void func_80073140(void) {
    s32 course;
    s32 one;
    s32 player;

    for (course = 0; course < 11; course++) {
        one = 1;
        for (player = 0; player < 5; player++) {
            D_800EC9F0.unk4E[course][player].unk0 = 3;
            D_800EC9F0.unk4E[course][player].unk1 = 0x3B;
            D_800EC9F0.unk4E[course][player].unk2 = 0;
            D_800EC9F0.unk156[course][player].unk0 = 3;
            D_800EC9F0.unk156[course][player].unk1 = 0;
            D_800EC9F0.unk156[course][player].unk2 = 0;
            D_800EC9F0.unk77FB[course][player] = player;
            D_800EC9F0.unk7832[course][player] = 0;
            D_800EC9F0.unk7869[course][player] = player;
            D_800EC9F0.unk78A0[course][player] = player;
            D_800EC9F0.unk7756[course][player] = 0;
            D_800EC9F0.unk77C4[course][player] = player;
        }
        D_800EC9F0.unk12A[course].unk0 = one;
        D_800EC9F0.unk12A[course].unk1 = 0x18;
        D_800EC9F0.unk12A[course].unk2 = 0;
        D_800EC9F0.unk4[course + 1] = 0;
    }

    D_800EC9F0.unk232[0].unk0 = 0;
    D_800EC9F0.unk232[1].unk0 = 0;
    D_800EC9F0.unk232[2].unk0 = 0;
    D_800EC9F0.unk232[3].unk0 = 0;
    D_800EC9F0.unk232[4].unk0 = 0;
    D_800EC9F0.unk232[5].unk0 = (u8)0;
    D_800EC9F0.unk232[6].unk0 = 0;
    D_800EC9F0.unk232[7].unk0 = 0;
    D_800EC9F0.unk232[8].unk0 = 0;
    D_80121B55 = 1;
    D_80121B5E = 0;
    D_80121D80[0].unk5 = 0;
    D_80121D80[1].unk5 = 0;
    D_80121D80[2].unk5 = 0;
    D_80121D80[3].unk5 = 0;
    D_80121D80[0].unk6 = 0;
    D_80121D80[1].unk6 = 0;
    D_80121D80[2].unk6 = 0;
    D_80121D80[3].unk6 = 0;
    D_80121D80[0].unk7 = 0;
    D_80121D80[1].unk7 = 0;
    D_80121D80[2].unk7 = 0;
    D_80121D80[3].unk7 = 0;
    D_80121D80[0].unkC = 0;
    D_80121D80[1].unkC = 0;
    D_80121D80[2].unkC = 0;
    D_80121D80[3].unkC = 0;
    D_80121D80[0].unk18 = 0;
    D_80121D80[1].unk18 = 0;
    D_80121D80[2].unk18 = 0;
    D_80121D80[3].unk18 = 0;
    D_80121B50.s = 0;
    D_800EC9C2 = 0;
    D_80121B58 = 0;
    func_8009956C(func_800732C4, 0);
}

void func_800732C4(void) {
    func_8009956C(&func_8007334C, 0);
    func_800994F4(4, &func_80003140, 0x64);
    func_80099614(2);
}

void func_80073308(void) {
    func_8009956C(&func_8007334C, 0);
    func_800994F4(4, &func_80004960, 0x64);
    func_80099614(2);
}

void func_8007334C(void) {
    if (D_80121B55 >= 2) {
        func_8009956C(&func_8007339C, 0);
    } else {
        func_8009956C(&func_800734A0, 0);
    }
}

void func_8007339C(void) {
    func_8009956C(&func_800733E0, 0);
    func_800994F4(4, &func_800062F8, 0x64);
    func_80099614(2);
}

void func_800733E0(void) {
    if (D_801235B4 == 0) {
        func_8009956C(&func_80073738, 0);
    } else {
        D_801235B4 = 0;
        func_8009956C(&func_80073308, 0);
    }
}

void func_80073434(void) {
    if (D_800EC9C2 == 1) {
        func_8009956C(&func_800735F8, 0);
    } else {
        func_8009956C(&func_800734E4, 0);
    }
    func_800994F4(4, &func_80005F18, 0x64);
    func_80099614(2);
}

void func_800734A0(void) {
    func_8009956C(&func_800734E4, 0);
    func_800994F4(4, &func_800055EC, 0x64);
    func_80099614(2);
}

void func_800734E4(void) {
    if (D_801235B4 == 1) {
        D_801235B4 = 0;
        func_8009956C(&func_80073308, 0);
        return;
    }
    switch (D_800EC9C2) {
    case 0:
    case 2:
        func_8009956C(&func_80073738, 0);
        return;
    case 1:
        func_8009956C(&func_800735B4, 0);
        return;
    case 3:
        func_8009956C(&func_80073650, 0);
        return;
    case 4:
        func_8009956C(&func_80073694, 0);
        /* fallthrough */
    default:
        return;
    }
}

void func_800735B4(void) {
    func_8009956C(&func_800735F8, 0);
    func_800994F4(4, &func_800086EC, 0x64);
    func_80099614(2);
}

void func_800735F8(void) {
    if (D_801235B4 == 1) {
        D_801235B4 = 0;
        func_8009956C(&func_800734A0, 0);
    } else {
        func_8009956C(&func_80073738, 0);
    }
}

void func_80073650(void) {
    func_8009956C(&func_800734A0, 0);
    func_800994F4(4, &func_800097E0, 0x64);
    func_80099614(2);
}

void func_80073694(void) {
    D_8010ADFA = 0;
    func_8009956C(&func_800736E0, 0);
    func_800994F4(4, &func_80008D60, 0x64);
    func_80099614(2);
}

void func_800736E0(void) {
    if (D_801235B4 == 1) {
        D_801235B4 = 0;
        func_8009956C(&func_800734A0, 0);
    } else {
        func_8009956C(&func_800737FC, 0);
    }
}

void func_80073738(void) {
    func_8009956C(&func_8007377C, 0);
    func_800994F4(4, &func_80006F30, 0x64);
    func_80099614(2);
}

void func_8007377C(void) {
    if (D_801235B4 == 1) {
        D_801235B4 = 0;
        if (D_80121B55 >= 2) {
            func_8009956C(&func_8007339C, 0);
        } else {
            func_8009956C(&func_80073434, 0);
        }
    } else {
        func_8009956C(&func_80077DA0, 0);
    }
}

void func_800737FC(void) {
    func_80000A40(0);
    func_80000A40(1);
    func_80000A40(2);
    func_80000A40(3);
    func_80072114(0);
    func_800994F4(3, &func_80001C30, 0x64);
    func_8009954C(2);
}

void func_80073858(void) {
    func_8009956C(&func_8007389C, 0);
    func_800994F4(4, &func_8000C280, 0x64);
    func_80099614(2);
}

void func_8007389C(void) {
    if (D_801235B4 == 1) {
        D_801235B4 = 0;
        func_8009956C(&func_800737FC, 0);
    } else {
        func_8009956C(&func_80073308, 0);
    }
}

void func_800738F4(void) {
    func_8009956C(&func_800734A0, 0);
    if (D_800DC4C0 != 0) {
        func_800994F4(4, &func_8000D340, 0x64);
        func_80099614(2);
    }
}

void func_80073944(void) {
    func_8009956C(&func_800738F4, 0);
    func_800994F4(4, &func_8003DFD0, 0x64);
    func_80099614(2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_80073988.s")

void func_800740C0(void) {
    D_800DEF14 -= 8;
    if (D_800DEF14 < 0) {
        D_800DEF14 = 0;
        D_801235B8->countdown--;
    }
    if (D_801235B8->countdown == 0) {
        func_8006D520(0, 0x1E);
        func_800720E4(5);
        func_8009956C(&func_80074160, 0);
    }
    func_80077C94();
    D_801124B8 = 0xFF;
}

void func_80074160(void) {
    s32 temp_a0;
    RaceFlowState *state;

    state = D_801235B8;
    temp_a0 = state->unk1C;
    if (temp_a0 >= 0x32) {
        D_800DEF10 = 1;
        D_800DEF14 += 0x16;
        if (D_800DEF14 >= 0x100) {
            func_8006D520(0, 1);
            D_800DEF14 = 0xFF;
            func_80071408(func_80069BC0, 6, 0x64);
            func_80071408(func_80065E90, 6, 0x64);
            switch (D_80121B55 & 0xFFFFFFFF) {
            case 1:
                if (D_80121B50.s != 6) {
                    func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
                } else {
                    func_80070A70(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
                }
                D_801121E0[0].active = 1;
                D_800DEED4 = 0;
                break;
            case 2:
                if (D_80121B50.s != 6) {
                    func_8007066C(0, 0xA0, 0x43, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
                    func_8007066C(1, 0xA0, 0xAD, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
                } else {
                    func_80070A70(0, 0xA0, 0x43, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
                    func_80070A70(1, 0xA0, 0xAD, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
                }
                D_801121E0[0].active = 1;
                D_801121E0[1].active = 1;
                D_800DEED4 = 1;
                break;
            case 3:
                if (D_80121B50.s != 6) {
                    func_8007066C(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    func_8007066C(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    func_8007066C(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                } else {
                    func_80070A70(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    func_80070A70(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    func_80070A70(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                }
                D_801121E0[0].active = 1;
                D_801121E0[1].active = 1;
                D_801121E0[2].active = 1;
                D_800DEED4 = 1;
                break;
            case 4:
                if (D_80121B50.s != 6) {
                    func_8007066C(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    func_8007066C(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    func_8007066C(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    func_8007066C(3, 0xE9, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                } else {
                    func_80070A70(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    func_80070A70(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    func_80070A70(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                    func_80070A70(3, 0xE9, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
                }
                D_801121E0[0].active = 1;
                D_801121E0[1].active = 1;
                D_801121E0[2].active = 1;
                D_801121E0[3].active = 1;
                D_800DEED4 = 1;
                break;
            }
            func_8009956C(func_800747E8, 0);
        }
    } else {
        state->unk1C = temp_a0 + 1;
        D_800DEF14 -= 0x10;
        if (D_800DEF14 < 0) {
            D_800DEF14 = 0;
        }
    }
    func_80077C94();
}

void func_800747E8(void) {
    if (D_800DEF14 != 0) {
        D_800DEF14 -= 0x10;
    }
    if (D_800DEF14 < 0) {
        D_800DEF14 = 0;
    }
    func_80077C4C();
    if (!(D_801235B4 & 1)) {
        D_800DEF10 = 0;
        func_800728E0();
        func_8009956C(func_80074960, 0);
    }
}

void func_80074864(s32 arg0) {
    s32 color;

    func_80045A78(-0x14, -0x10, func_80043040(D_8011216E), 0x57);
    color = 0x1A;
    if (D_80121B57 == 0) {
        color = 0x1B;
    }
    func_80046D68(-0x1C, 0, func_80043040(D_8011216E), 0x58, color);
    color = 0x1A;
    if (D_80121B57 == 1) {
        color = 0x1B;
    }
    func_80046D68(-0x1C, 0xA, func_80043040(D_8011216E), 0x59, color);
    color = 0x1A;
    if (D_80121B57 == 2) {
        color = 0x1B;
    }
    func_80046D68(-0x1C, 0x14, func_80043040(D_8011216E), 0x5A, color);
}

void func_80074960(void) {
    RacePlayerState *player;
    s32 opened;
    s32 i;
    s32 input;
    s32 selection;
    s32 valueTwo;

    opened = 0; i = 0; if (D_80121B55 > 0) { player = D_80121D80; do { if (((player->unk14 == 0) && (D_80121B56 == 0)) && (D_80123778[i] & 0x1000)) { D_80121B57 = 0; D_80121B56 = 1; opened = 1; func_80072138(1, 0x32); func_80000A40(0); func_80000A40(1); func_80000A40(2); func_80000A40(3); } i++; player++; } while (i < D_80121B55); } valueTwo = 2; if ((D_80121B56 != 0) && (opened == 0)) { i = 0; if (D_80121B55 > 0) { player = D_80121D80; do { if (player->unk14 == 0) { input = D_80123778[i]; if (input & 0x10800) { selection = D_80121B57; if (selection != 0) { D_80121B57 = selection - 1; func_80072138(1, 0x32); input = D_80123778[i]; }
                    }
                    if (input & 0x20400) {
                        selection = D_80121B57;
                        if (valueTwo != selection) {
                            D_80121B57 = selection + 1;
                            func_80072138(1, 0x32);
                            input = D_80123778[i];
                        }
                    }
                    if (input & 0x1000) {
                        input = D_80121B57;
                        if (input == 0) {
                            D_80121B56 = 0;
                            func_80072138(1, 0x32);
                            input = D_80121B57;
                        }
                        if (input == 1) {
                            D_800EC8B0 = 0;
                            D_80121B56 = 0;
                            D_80123751 = 1;
                            func_80072138(1, 0x32);
                            func_8009956C(func_80077B34, 0);
                            return;
                        }
                        if (valueTwo == input) {
                            D_800EC8B0 = 0;
                            D_80121B56 = 0;
                            D_80123751 = 1;
                            func_80072138(1, 0x32);
                            func_8009956C(func_80077B34, 0);
                            return;
                        }
                    }
                }
                i++;
                player++;
            } while (i < D_80121B55);
        }
        func_800483FC(&D_80124868, func_80074864, 0);
    }

    if (D_800EC9C2 == valueTwo) {
        func_80072E98();
    }
    func_80077C4C();
    if (func_80077D14() != 0) {
        D_801235B8->fadeTimer = 0x3C;
        D_800EC8B0 = 0;
        func_8009956C(func_80074C5C, 0);
    }
}

// func_80074C5C best match: 88.931% (nonmatchings/func_80074C5C-3236181511606361864/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_80074C5C.s")

#ifdef NON_MATCHING
void func_80074C5C(void) {
    s32 bestPlayer;
    s8 value;

    D_80121B57 = 0;
    func_80077C4C();
    D_801235B8->fadeTimer--;
    if (D_801235B8->fadeTimer == 0) {
        D_801235B4 |= 8;
        switch (D_80121B55) {
        case 3:
            D_801235B8->unk1C = 0;
            bestPlayer = D_801235B8->unk1C;
            value = RACE_PLAYER_RESULT_VALUE(bestPlayer);
            if (RACE_PLAYER_RESULT_VALUE(1) < value) {
                D_801235B8->unk1C = 1;
                bestPlayer = D_801235B8->unk1C;
                value = RACE_PLAYER_RESULT_VALUE(bestPlayer);
            }
            if (RACE_PLAYER_RESULT_VALUE(2) < value) {
                D_801235B8->unk1C = 2;
                bestPlayer = D_801235B8->unk1C;
            }
            D_801235B8->unk1C = bestPlayer + 2;
            func_8009956C(func_800751C4, 0);
            break;
        case 4:
            D_801235B8->unk1C = 0;
            bestPlayer = D_801235B8->unk1C;
            value = RACE_PLAYER_RESULT_VALUE(bestPlayer);
            if (RACE_PLAYER_RESULT_VALUE(1) < value) {
                D_801235B8->unk1C = 1;
                bestPlayer = D_801235B8->unk1C;
                value = RACE_PLAYER_RESULT_VALUE(bestPlayer);
            }
            if (RACE_PLAYER_RESULT_VALUE(2) < value) {
                D_801235B8->unk1C = 2;
                bestPlayer = D_801235B8->unk1C;
                value = RACE_PLAYER_RESULT_VALUE(bestPlayer);
            }
            if (RACE_PLAYER_RESULT_VALUE(3) < value) {
                D_801235B8->unk1C = 3;
                bestPlayer = D_801235B8->unk1C;
            }
            D_801235B8->unk1C = bestPlayer + 5;
            func_8009956C(func_800751C4, 0);
            break;
        case 2:
            if (RACE_PLAYER_RESULT_VALUE(0) < RACE_PLAYER_RESULT_VALUE(1)) {
                D_801235B8->unk1C = 0;
                D_8011233C = 0;
            } else {
                D_801235B8->unk1C = 1;
                D_8011228C = 0;
            }
            D_801235B8->fadeTimer = 0;
            func_8009956C(func_800751C4, 0);
            break;
        case 1:
            D_801235B8->fadeTimer = 0;
            func_8009956C(func_80076054, 0);
            break;
        }
    }
}
#endif

// func_80074F50 best match: 60.289%
#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_80074F50.s")

#ifdef NON_MATCHING
extern void func_8007066C(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_80070A70(s32, s32, s32, s32, s32, s32, s32, f32);

void func_80074F50(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6,
                  f32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11, s32 arg12, s32 arg13,
                  f32 arg14) {
    volatile s32 sp2C;
    volatile s32 sp28;
    volatile f32 temp_fv0;
    volatile s32 temp_t0;
    volatile s32 temp_t1;
    volatile s32 temp_t2;
    volatile s32 temp_t3;
    s32 temp_v0;

    temp_v0 = D_801235B8->fadeTimer;
    sp2C = (((arg8 - arg1) * temp_v0) / 15) + arg1;
    sp28 = (((arg9 - arg2) * temp_v0) / 15) + arg2;
    temp_t0 = (((arg10 - arg3) * temp_v0) / 15) + arg3;
    temp_t1 = (((arg11 - arg4) * temp_v0) / 15) + arg4;
    temp_t2 = (((arg12 - arg5) * temp_v0) / 15) + arg5;
    temp_fv0 = (((arg14 - arg7) * (f32) temp_v0) / 15.0f) + arg7;
    temp_t3 = (((arg13 - arg6) * temp_v0) / 15) + arg6;
    if (D_80121B50.s != 6) {
        func_8007066C(arg7, sp2C, sp28, temp_t0 & 0xFFFF, temp_t1, temp_t2, temp_t3, temp_fv0);
    } else {
        func_80070A70(arg7, sp2C, sp28, temp_t0 & 0xFFFF, temp_t1, temp_t2, temp_t3, temp_fv0);
    }
}
#endif

void func_800751C4(void) {
    D_801235B8->fadeTimer += 1;
    switch (D_801235B8->unk1C) {
    case 0:
        func_80074F50(0, 0xA0, 0x43, 0x120, 0x68, 0x140, 0x78, 2.6666667f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        func_80074F50(1, 0xA0, 0xAD, 0x120, 0x68, 0x140, 0x78, 2.6666667f, 0xA0, 0x115, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
        if (D_801235B8->fadeTimer == 0xF) {
            func_80070614(1);
            D_801121E0[1].active = 0;
        }
        break;
    case 1:
        func_80074F50(1, 0xA0, 0xAD, 0x120, 0x68, 0x140, 0x78, 2.6666667f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        func_80074F50(0, 0xA0, 0x43, 0x120, 0x68, 0x140, 0x78, 2.6666667f, 0xA0, -0x25, 0x120, 0x68, 0x140, 0x78, 2.6666667f);
        if (D_801235B8->fadeTimer == 0xF) {
            func_80070614(0);
            D_801121E0[0].active = 0;
        }
        break;
    case 2:
        func_80074F50(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        func_80074F50(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x57, 0x115, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        func_80074F50(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x179, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        if (D_801235B8->fadeTimer == 0xF) {
            D_801121E0[1].active = 0;
            D_801121E0[2].active = 0;
            func_80070614(1);
            func_80070614(2);
        }
        break;
    case 3:
        func_80074F50(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        func_80074F50(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x57, -0x25, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        func_80074F50(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x179, -0x25, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        if (D_801235B8->fadeTimer == 0xF) {
            D_801121E0[0].active = 0;
            D_801121E0[2].active = 0;
            func_80070614(0);
            func_80070614(2);
        }
        break;
    case 4:
        func_80074F50(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        func_80074F50(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, -0x39, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        func_80074F50(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, -0x39, 0x115, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        if (D_801235B8->fadeTimer == 0xF) {
            D_801121E0[0].active = 0;
            D_801121E0[1].active = 0;
            func_80070614(0);
            func_80070614(1);
        }
        break;
    case 5:
        func_80074F50(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        func_80074F50(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x57, 0x115, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        func_80074F50(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x179, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        func_80074F50(3, 0xE9, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x179, 0x115, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        if (D_801235B8->fadeTimer == 0xF) {
            func_80070614(1);
            func_80070614(2);
            func_80070614(3);
            D_801121E0[1].active = 0;
            D_801121E0[2].active = 0;
            D_801121E0[3].active = 0;
        }
        break;
    case 6:
        func_80074F50(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        func_80074F50(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x57, -0x25, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        func_80074F50(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x179, -0x25, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        func_80074F50(3, 0xE9, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0x179, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        if (D_801235B8->fadeTimer == 0xF) {
            D_801121E0[0].active = 0;
            D_801121E0[2].active = 0;
            D_801121E0[3].active = 0;
            func_80070614(0);
            func_80070614(2);
            func_80070614(3);
        }
        break;
    case 7:
        func_80074F50(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        func_80074F50(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, -0x39, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        func_80074F50(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, -0x39, 0x115, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        func_80074F50(3, 0xE9, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xE9, 0x115, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        if (D_801235B8->fadeTimer == 0xF) {
            D_801121E0[0].active = 0;
            D_801121E0[1].active = 0;
            D_801121E0[3].active = 0;
            func_80070614(0);
            func_80070614(1);
            func_80070614(3);
        }
        break;
    case 8:
        func_80074F50(3, 0xE9, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        func_80074F50(0, 0x57, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, -0x39, -0x25, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        func_80074F50(1, 0x57, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, -0x39, 0xAD, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        func_80074F50(2, 0xE9, 0x43, 0x90, 0x68, 0xA0, 0x78, 1.3333334f, 0xE9, -0x25, 0x90, 0x68, 0xA0, 0x78, 1.3333334f);
        if (D_801235B8->fadeTimer == 0xF) {
            D_801121E0[0].active = 0;
            D_801121E0[1].active = 0;
            D_801121E0[2].active = 0;
            func_80070614(0);
            func_80070614(1);
            func_80070614(2);
        }
        break;
    }
    func_80077C94();
    if (D_801235B8->fadeTimer == 0xF) {
        func_8009956C(func_80076054, 0);
    }
}

// func_80076054 best match: 81.749% (nonmatchings/func_80076054-7273315160691878794/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_80076054.s")

#ifdef NON_MATCHING
void func_80076054(void) {
    RacePlayerState *player;
    TimeCourseView *timeCourse;
    TrickCourseView *trickCourse;
    ScoreCourseView *scoreCourse;
    RaceTime *time;
    u16 *trickScore;
    u8 *scoreValue;
    s32 i;
    s32 index;
    s32 currentTime;
    s32 recordTime;
    s32 courseOffset;

    D_800DC4C0 = 0;
    D_800DEED4 = 0;
    D_80121B60 = 0;
    D_80121B61 = 0;

    switch (D_800EC9C2) {
    case 0:
        D_80121B5F = 1;
        i = 0;
        if (D_80121B55 > 0) {
            player = D_80121D80;
            do {
                if (player->result == 0) {
                    D_80121B60 = i + 1;
                }
                i++;
                player++;
            } while (i < D_80121B55);
        }
        break;

    case 2:
        currentTime = (D_80121B74.seconds * COURSE_TIME_SECOND) + D_80121B74.fraction + (D_80121B74.minutes * COURSE_TIME_MINUTE);
        courseOffset = (D_80121B50.s << 2) + D_80121B50.s;
        courseOffset <<= 2;
        timeCourse = (TimeCourseView *)((u8 *)&D_800EC9F0 + courseOffset);
        index = 0;
        time = timeCourse->timeTrial;
        do {
            recordTime = (time->minutes * COURSE_TIME_MINUTE) + time->fraction + (time->seconds * COURSE_TIME_SECOND);
            if (currentTime < recordTime) {
                break;
            }
            index += 4;
            time++;
        } while (index != 0x14);
        if (index < 0x14) {
            D_80121B5F = 1;
            D_80121B60 = 1;
            if (index == 0) {
                D_80121B61 = 1;
            }
        } else {
            D_80121B5F = 2;
        }
        break;

    case 1:
        if (D_80121B5E != 0) {
            i = 0;
            if (D_80121B5E != 1) {
                if (D_80121B5E == 2) {
                    courseOffset = (D_80121B50.s << 2) + D_80121B50.s;
                    courseOffset <<= 1;
                    trickCourse = (TrickCourseView *)((u8 *)&D_800EC9F0 + courseOffset);
                    trickScore = trickCourse->values;
                    do {
                        if (*trickScore < D_80122040) {
                            break;
                        }
                        i++;
                        trickScore++;
                    } while (i < 5);
                    if (D_80121B81 != 0) {
                        i = 5;
                    }
                    if (i < 5) {
                        D_80121B5F = 1;
                        D_80121B60 = 1;
                    } else {
                        D_80121B5F = 2;
                    }
                }
            } else {
                courseOffset = (D_80121B50.s << 2) + D_80121B50.s;
                scoreCourse = (ScoreCourseView *)((u8 *)&D_800EC9F0 + courseOffset);
                scoreValue = scoreCourse->values;
                do {
                    if (*scoreValue < D_801222F4) {
                        break;
                    }
                    i++;
                    scoreValue++;
                } while (i < 5);
                if (D_80121B81 != 0) {
                    i = 5;
                }
                if (i < 5) {
                    D_80121B5F = 1;
                    D_80121B60 = 1;
                } else {
                    D_80121B5F = 2;
                }
            }
        } else {
            currentTime = (D_80121B74.seconds * COURSE_TIME_SECOND) + D_80121B74.fraction + (D_80121B74.minutes * COURSE_TIME_MINUTE);
            courseOffset = (D_80121B50.s << 2) + D_80121B50.s;
            courseOffset <<= 2;
            timeCourse = (TimeCourseView *)((u8 *)&D_800EC9F0 + courseOffset);
            i = 0;
            time = timeCourse->raceTimes;
            do {
                recordTime = (time->minutes * COURSE_TIME_MINUTE) + time->fraction + (time->seconds * COURSE_TIME_SECOND);
                if (currentTime < recordTime) {
                    break;
                }
                i++;
                time++;
            } while (i < 5);
            if (D_80121B81 != 0) {
                i = 5;
            }
            if (i < 5) {
                D_80121B5F = 1;
                D_80121B60 = 1;
            } else {
                D_80121B5F = 2;
            }
        }
        break;
    }

    D_801235B8->unk1C = 0x3C;
    func_8009956C(func_80076490, 0);
    if (D_80121B60 != 0) {
        func_800720E4(6);
        if ((D_80121B55 == 1) && (D_800EC9C2 == 0)) {
            func_80071664(func_8005E68C, 6, 0x64, 0xA9);
        }
        func_80071664(func_8005393C, 5, 0x64, D_80121B60 - 1);
    }
    func_80077C94();
}
#endif

// func_80076490 best match: 88.124% at nonmatchings/func_80076490-7273315160691878794/base_6.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_80076490.s")

void func_80077324(void) {
    if (func_80072938() == 0) {
        func_800720E4(7);
        D_801235B8->unk1C = 0x3C;
        D_801235B4 |= 0x20;
    }
    if (D_80121B60 != 0) {
        func_80071664(&func_8005393C, 5, 0x64, D_80121B60 - 1);
    }
    func_80077C94();
    if (D_801235B4 & 0x10) {
        D_801235B8->fadeTimer -= 1;
        if (D_801235B8->fadeTimer == 0) {
            D_801235B8->fadeTimer = 4;
            func_8009956C(&func_80077400, 0);
            func_80072114(0x14);
        }
    }
}

void func_80077400(void) {
    D_800DEED4 = 0;
    if (D_80121B60 != 0) {
        func_80071664(func_8005393C, 5, 0x64, D_80121B60 - 1);
    }
    func_80077C94();
    D_800EC8B0 = 0;
    D_800DEF14 += 0x10;
    if (D_800DEF14 >= 0xFF) {
        D_800DEF14 = 0xFF;
        D_801235B8->fadeTimer -= 1;
        if (D_801235B8->fadeTimer == 0) {
            func_80072260();
            if ((D_800EC9C2 == 2) && (((Unk80043040 *)func_80043040(D_80112186))->unk8 != 0) && (D_80121B61 != 0) &&
                (func_80040D94() != 0)) {
                D_80121B61 = -1;
            }
            if (D_800EC9C2 == 2) {
                func_8009956C(&func_80077554, 0);
            } else {
                D_80123751 = 1;
                func_8009956C(func_80077B34, 0);
            }
        }
    }
}

// func_80077554 best match: 99.464%
#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_80077554.s")

#ifdef NON_MATCHING
void func_80077554(void) {
    s32 one;
    s32 asset;

    D_80121B56 = 0;
    D_80121B58 = 1;
    D_80121B5F = 0;
    asset = func_80043040(D_80112130[0x2B]);
    if (((Unk80043040 *)asset)->unk8 == 0) {
        D_80123751 = 1;
        func_8009956C(func_80077B34, 0);
        return;
    }
    if (func_800730EC() == 0) {
        D_80123751 = 1;
        func_8009956C(func_80077B34, 0);
        return;
    }
    ((Unk80043040 *)asset)->unk0 = 0;
    if (D_80112130[0x21] != -1) {
        D_80112130[0x21] = func_80042EE4(D_80112130[0x21]);
    }
    if (D_80112130[0x22] != -1) {
        D_80112130[0x22] = func_80042EE4(D_80112130[0x22]);
    }
    if (D_80112130[0x23] != -1) {
        D_80112130[0x23] = func_80042EE4(D_80112130[0x23]);
    }
    if (D_80112130[0x24] != -1) {
        D_80112130[0x24] = func_80042EE4(D_80112130[0x24]);
    }
    if (D_80112130[0x25] != -1) {
        D_80112130[0x25] = func_80042EE4(D_80112130[0x25]);
    }
    if (D_80112130[0x26] != -1) {
        D_80112130[0x26] = func_80042EE4(D_80112130[0x26]);
    }
    if (D_80112130[0x27] != -1) {
        D_80112130[0x27] = func_80042EE4(D_80112130[0x27]);
    }
    if (D_80112130[0x28] != -1) {
        D_80112130[0x28] = func_80042EE4(D_80112130[0x28]);
    }
    if (D_80112130[0x1F] != -1) {
        D_80112130[0x1F] = func_80042EE4(D_80112130[0x1F]);
    }
    ((Unk80043040 *)asset)->unk0 = 0;
    one = 1;
    D_80121D80[0].unk4 = 0;
    D_80121D80[1].unk4 = one;
    D_80121D80[2].unk4 = one;
    D_80121D80[3].unk4 = one;
    D_80121D80[0].unk13 = one;
    D_80121D80[1].unk13 = 0;
    D_80121D80[2].unk13 = 0;
    D_80121D80[3].unk13 = 0;
    D_80121B54 = 1;
    *(u16 *)&D_80121B52 = 1;
    D_80121B5C = 0x64;
    func_80070EC0(2);
    D_80121D95 = 0;
    D_80121D96 = 0;
    D_80121D94 = 0;
    D_801223A1 = 0;
    D_801223A2 = 0;
    D_801223A0 = 0;
    D_801229AD = 0;
    D_801229AE = 0;
    D_801229AC = 0;
    D_80122FB9 = 0;
    D_80122FBA = 0;
    D_80122FB8 = 0;
    D_80121D80[0].unk16 = 2;
    D_801235B4 = 0;
    func_8006D5CC();
    func_800704F0();
    if (D_80121B50.s != 6) {
        func_8007066C(0, 0xA0, 0x78, 0x100, 0xB0, 0x120, 0xD0, D_800E16CC);
    } else {
        func_80070A70(0, 0xA0, 0x78, 0x100, 0xB0, 0x120, 0xD0, D_800E16D0);
    }
    func_800437F0(D_598A70, D_59AAA0, 0x29);
    D_8011228C = 1;
    D_800DEED4 = 0;
    func_80043154();
    func_8008BEB0();
    func_80078430();
    func_80044294();
    D_800DEF14 = 0xFF;
    func_80042C20();
    func_800720E4(0);
    D_801235B8->fadeTimer = 0;
    D_801235B8->unk1C = 0;
    func_80071408(func_80057E60, 6, 0x64);
    if (D_80121B61 == -1) {
        func_80071408((void (*)(EffectTask *))func_80052520, 6, 0x64);
    }
    func_8009956C(func_8007797C, 0);
}
#endif

void func_8007797C(void) {
    void *sp18;

    if (D_801235B8->fadeTimer == D_800DC5C0[D_801235B8->unk1C]) {
        if (func_80072FC4() != 0) {
            if (D_801235B8->unk1C != 0xB) {
                D_801235B8->unk1C++;
                D_80121D80[0].unk16 = 2;
                D_80121D80[0].unk15 = 0;
                D_801124B8 = 0;
            } else {
                func_80072114(0x48);
                func_8009956C(func_80077AD4, 0);
            }
        } else {
            func_80072114(0x48);
            func_8009956C(func_80077AD4, 0);
        }
    }
    if (D_801235B8->fadeTimer == 1) {
        D_800DEF14 = 0;
    }
    if (D_80121B56 == 0) {
        D_801235B8->fadeTimer++;
    }
    func_80077CD4();
    if (D_80123778[0] & 0x1000) {
        sp18 = func_80077AD4;
        func_80072114(0x48);
        func_8009956C(sp18, 0);
    }
}

void func_80077AD4(void) {
    func_80077CD4();
    D_800EC8B0 = 0;
    D_800DEF14 += 7;
    if (D_800DEF14 >= 0xFF) {
        D_800DEF14 = 0xFF;
        func_8009956C(&func_80077B34, 0);
    }
}

void func_80077B34(void) {
    D_80123751 = 1;
    if (D_80123750 == 2) {
        D_800EC8B0 = 0;
        D_80121B58 = 0;
        func_80042C20();
        func_80045914();
        D_80123751 = 0;
        D_800DEED4 = 0;
        func_80072260();
        func_80000A40(0);
        func_80000A40(1);
        func_80000A40(2);
        func_80000A40(3);
        D_801235B4 = 0;
        if (D_80121B57 == 2) {
            func_8009956C(func_80073988, 0);
        } else if (D_80121B55 == 1) {
            if (D_800EC9C2 == 1) {
                func_8009956C(func_800735B4, 0);
            } else {
                func_8009956C(func_80073944, 0);
            }
        } else {
            func_8009956C(func_80073858, 0);
        }
        func_80072114(0);
    }
}

void func_80077C4C(void) {
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    func_8007AA50();
}

void func_80077C94(void) {
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
}

void func_80077CD4(void) {
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
}

s32 func_80077D14(void) {
    s32 var_v0;
    RacePlayerState *player;

    if (D_800EC9C2 == 1) {
        if (!(D_8012207C & 0x40)) {
            return 0;
        }
    } else {
        var_v0 = 0;
        if ((s32)D_80121B55 > 0) {
            player = D_80121D80;
loop:
            if (!(player->flags & RACE_PLAYER_READY_FLAG)) {
                return 0;
            }
            var_v0++;
            player++;
            if (var_v0 < (s32)D_80121B55) {
                goto loop;
            }
        }
    }
    D_801235B4 |= 2;
    return 1;
}

void func_80077DA0(void) {
    D_80121B52 = D_800B9542[D_80121B50.s].unk0;
    if (D_800EC9C2 != 0) {
        func_8009956C(func_80073988, 0);
        return;
    }
    if (D_80121B55 == 1) {
        func_8009956C(func_80073988, 0);
        return;
    }
    if (D_8012482A == 0) {
        func_8009956C(func_80073988, 0);
        return;
    }
    func_800704F0();
    D_800DEED4 = 0;
    func_800437F0(D_593D10, D_598A70, 0x29);
    func_800437F0(D_60F1A0, D_60F990, 0x2A);
    func_800437F0(D_59DFE0, D_59E7F0, 0x26);
    func_800437F0(D_1F1A90, D_1F2220, 0x28);
    switch (D_80121B50.u) {
    case 0:
        func_800437F0(D_1EC0F0, D_1EC4A0, 0x1D);
        break;
    case 1:
        func_800437F0(D_1EC4A0, D_1EC810, 0x1D);
        break;
    case 2:
        func_800437F0(D_1EC810, D_1ECD00, 0x1D);
        break;
    case 3:
        func_800437F0(D_1ECD00, D_1ED910, 0x1D);
        break;
    case 4:
        func_800437F0(D_1ED910, D_1EDC80, 0x1D);
        break;
    case 5:
        func_800437F0(D_1EDC80, D_1EE060, 0x1D);
        break;
    case 6:
        func_800437F0(D_1EE060, D_1EEEA0, 0x1D);
        break;
    case 8:
        func_800437F0(D_1EEEA0, D_1EF1D0, 0x1D);
        break;
    case 9:
        func_800437F0(D_1EF1D0, D_1EF530, 0x1D);
        break;
    }
    func_800437F0(D_1E74E0, D_1EC0F0, 0x1C);
    func_80070EC0(0);
    D_801235B4 = 0;
    func_80071664(func_80053634, 0, 0x64, 0);
    func_80071408((void (*)(EffectTask *))func_8001710C, 0, 0x5E);
    func_8009956C(func_80078078, 0);
    func_800720E4(7);
}

void func_80078078(void) {
    s32 temp_v0;

    D_800DEF14 -= 0x10;
    if (D_800DEF14 < 0) {
        D_800DEF14 = 0;
        temp_v0 = D_80123778[0];
        if ((temp_v0 & 0x20400) && (D_80121B52 != 9)) {
            func_80072138(0x19, 0x32);
            D_80121B52 += 1;
            temp_v0 = D_80123778[0];
        }
        if ((temp_v0 & 0x10800) && (D_80121B52 != 1)) {
            func_80072138(0x19, 0x32);
            D_80121B52 -= 1;
            temp_v0 = D_80123778[0];
        }
        if (temp_v0 & 0x9000) {
            D_801235B4 = 1;
            func_80072138(0x18, 0x32);
            func_80072114(0x3C);
            func_8009956C(func_80078198, 0);
        }
    }
    func_8007105C();
}

void func_80078198(void) {
    D_800DEF14 += 0x10;
    if (D_800DEF14 >= 0x100) {
        D_800DEF14 = 0xFF;
        D_80123751 = 1;
        func_8009956C(&func_800781FC, 0);
    }
    func_8007105C();
}

void func_800781FC(void) {
    if (D_80123750 == 2) {
        func_80045914();
        D_80123751 = 0;
        D_800DEED4 = 0;
        func_8009956C(&func_80073988, 0);
    }
}
