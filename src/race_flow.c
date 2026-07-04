#include "common.h"

#define COURSE_GRID_ENTRY_FREE -1
#define COURSE_GRID_ENTRY_END -2
#define RACE_PLAYER_STATE_SIZE 0x60C
#define RACE_PLAYER_READY_FLAG 0x40

typedef struct {
    /* 0x00 */ s16 status;
    /* 0x02 */ u8 pad2[0x10 - 0x2];
} CourseGridEntry;

typedef struct {
    /* 0x000 */ u8 pad0[0x2FC];
    /* 0x2FC */ s32 flags;
    /* 0x300 */ u8 pad300[RACE_PLAYER_STATE_SIZE - 0x300];
} RacePlayerState;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 fadeTimer;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 countdown;
} RaceFlowState;

extern CourseGridEntry *D_800DC490[];
extern RaceFlowState *D_801235B8;
extern RacePlayerState D_80121D80[];
extern s32 D_801235B4;
extern s32 D_80123758;
extern s32 D_8012207C;
extern s32 D_80123778;
extern s16 D_80112188;
extern s16 D_801124B8;
extern s16 D_80121B50;
extern s16 D_80121B52;
extern s16 D_800DEF14;
extern s8 D_800DEF10;
extern s8 D_800EC8B0;
extern u8 D_2427D0[];
extern u8 D_243270[];
extern u8 D_80121B55;
extern u8 D_80121B57;
extern u8 D_80121B58;
extern u8 D_80121B60;
extern u8 D_80123750;
extern u8 D_80123751;
extern u8 D_800DEED4;
extern u8 D_800EC9C2;
extern u8 D_8010ADFA;
extern u8 D_800DC4C0;

extern s16 func_80042D58(s32);
extern void func_80000A40(s32);
extern void func_80001C30(void);
extern void func_80003140(void);
extern void func_80004960(void);
extern void func_800055EC(void);
extern void func_80005F18(void);
extern void func_800062F8(void);
extern void func_80006F30(void);
extern void func_800086EC(void);
extern void func_80008D60(void);
extern void func_800097E0(void);
extern void func_8000C280(void);
extern void func_8000C600(void);
extern void func_8000CF70(void);
extern void func_8000D340(void);
extern void func_8003DFD0(void);
extern void func_80042C20(void);
extern void func_800437F0(void *, void *, s32);
extern void func_80045914(void);
extern void func_8006D520(s32, s32);
extern void func_8006D700(void);
extern void func_8007105C(void);
extern void func_800710CC(s32);
extern void func_8007115C(void);
extern void func_80071664(void *, s32, s32, s32);
extern void func_8005393C(void);
extern void func_800720E4(s32);
extern void func_80072114(s32);
extern void func_80072138(s32, s32);
extern void func_80072260(void);
extern void func_800728E0(void);
extern s32 func_80072938(void);
extern void func_80070EC0(s32);
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
extern void func_80074960(void);
extern void func_80077B34(void);
extern void func_80077C4C(void);
extern void func_80077400(void);
extern void func_80077C94(void);
extern void func_80077CD4(void);
extern void func_80077DA0(void);
extern void func_80078078(void);
extern void func_80078198(void);
extern void func_800781FC(void);
extern void func_8007AA50(void);
extern void func_8008C704(void);
extern void func_80096E3C(void);
extern void func_800994F4(s32, void *, s32);
extern void func_8009954C(s32);
extern void func_8009956C(void *, s32);
extern void func_80099614(s32);

void func_80072C88(void);
void func_80072D04(void);
void func_80072D54(void);
void func_80072D98(void);
void func_80072DDC(void);
void func_8007334C(void);

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
    CourseGridEntry *entry = D_800DC490[D_80121B50];
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

#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_80072E98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_80072FC4.s")

s32 func_800730EC(void) {
    CourseGridEntry *entry = D_800DC490[D_80121B50];

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

#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_80073140.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_80074160.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_80074864.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_80074960.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_80074C5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_80074F50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_800751C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_80076054.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_80077400.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_80077554.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_8007797C.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/race_flow/func_80077DA0.s")

void func_80078078(void) {
    s32 temp_v0;

    D_800DEF14 -= 0x10;
    if (D_800DEF14 < 0) {
        D_800DEF14 = 0;
        temp_v0 = D_80123778;
        if ((temp_v0 & 0x20400) && (D_80121B52 != 9)) {
            func_80072138(0x19, 0x32);
            D_80121B52 += 1;
            temp_v0 = D_80123778;
        }
        if ((temp_v0 & 0x10800) && (D_80121B52 != 1)) {
            func_80072138(0x19, 0x32);
            D_80121B52 -= 1;
            temp_v0 = D_80123778;
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
