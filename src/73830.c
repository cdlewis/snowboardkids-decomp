#include "common.h"

typedef struct {
    char pad[0x18];
    s32 unk18;
    char pad1C[4];
    s32 unk20;
} Struct801235B8;

extern void func_8009956C(void *, s32);
extern void func_800734A0(void);
extern void func_800437F0(void *, void *, s32);
extern void func_80070EC0(s32);
extern u8 D_2427D0;
extern u8 D_243270;
extern Struct801235B8 *D_801235B8;
extern void func_80072C88(void);
extern void func_80000A40(s32);
extern void func_80072D04(void);

void func_80072C30(void) {
    func_800437F0(&D_2427D0, &D_243270, 6);
    func_80070EC0(0);
    D_801235B8->unk18 = 0xA;
    func_8009956C(&func_80072C88, 0);
}

void func_80072C88(void) {
    D_801235B8->unk18 -= 1;
    if (D_801235B8->unk18 == 0) {
        func_80000A40(0);
        func_80000A40(1);
        func_80000A40(2);
        func_80000A40(3);
        func_8009956C(&func_80072D04, 0);
    }
}

extern void func_80072D54(void);
extern void func_80072D98(void);
extern s32 D_80123758;

void func_80072D04(void) {
    if (D_80123758 & 0x1000) {
        func_8009956C(&func_80072D54, 0);
    } else {
        func_8009956C(&func_80072D98, 0);
    }
}

extern void func_8009956C(void *, s32);
extern void func_800994F4(s32, void *, s32);
extern void func_80099614(s32);
extern void func_80072D98(void);
extern void func_8000C600(void);
extern void func_8009954C(s32);
extern void func_80001C30(void);
extern void func_800062F8(void);
extern void func_800733E0(void);

void func_80072D54(void) {
    func_8009956C(&func_80072D98, 0);
    func_800994F4(4, &func_8000C600, 0x64);
    func_80099614(0);
}

extern void func_8000CF70(void);
extern void func_80072DDC(void);

void func_80072D98(void) {
    func_8009956C(&func_80072DDC, 0);
    func_800994F4(4, &func_8000CF70, 0x64);
    func_80099614(0);
}

void func_80072DDC(void) {
    func_800994F4(3, &func_80001C30, 0x64);
    func_8009954C(0);
}

extern s16 func_80042D58(s32);
extern s16 *D_800DC490[];
extern s16 D_80112188;
extern s16 D_80121B50;

void func_80072E10(void) {
    s16 *p = D_800DC490[D_80121B50];
    s32 count = 0;
loop:
    if (*p != -2) {
        *p = -1;
        p = (s16 *)((u8 *)p + 0x10);
        count += 1;
        goto loop;
    }
    if (count != 0) {
        D_80112188 = func_80042D58(count * 0x60C);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/73830/func_80072E98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/73830/func_80072FC4.s")

s32 func_800730EC(void) {
    s16 *ptr = D_800DC490[D_80121B50];

loop:
    if (*ptr != -2) {
        if (*ptr != -1) {
            return 1;
        }
        ptr = (s16 *)((u8 *)ptr + 0x10);
        goto loop;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/73830/func_80073140.s")

extern void func_80003140(void);
extern void func_8007334C(void);
extern void func_80004960(void);

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

extern void func_8007339C(void);
extern u8 D_80121B55;

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

extern void func_80073738(void);
extern void func_80073308(void);
extern s32 D_801235B4;
extern s32 D_8012207C;
extern u8 D_80121D80[];
extern s8 D_800DEF10;

void func_800733E0(void) {
    if (D_801235B4 == 0) {
        func_8009956C(&func_80073738, 0);
    } else {
        D_801235B4 = 0;
        func_8009956C(&func_80073308, 0);
    }
}

extern void func_800734E4(void);
extern void func_800735F8(void);
extern void func_80005F18(void);
extern u8 D_800EC9C2;

void func_80073434(void) {
    if (D_800EC9C2 == 1) {
        func_8009956C(&func_800735F8, 0);
    } else {
        func_8009956C(&func_800734E4, 0);
    }
    func_800994F4(4, &func_80005F18, 0x64);
    func_80099614(2);
}

extern void func_800055EC(void);
extern void func_800734E4(void);

void func_800734A0(void) {
    func_8009956C(&func_800734E4, 0);
    func_800994F4(4, &func_800055EC, 0x64);
    func_80099614(2);
}

extern void func_800735B4(void);
extern void func_80073650(void);
extern void func_80073694(void);

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

extern void func_800086EC(void);
extern void func_800735F8(void);

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

extern void func_800097E0(void);

void func_80073650(void) {
    func_8009956C(&func_800734A0, 0);
    func_800994F4(4, &func_800097E0, 0x64);
    func_80099614(2);
}

extern void func_800736E0(void);
extern void func_80008D60(void);
extern u8 D_8010ADFA;

void func_80073694(void) {
    D_8010ADFA = 0;
    func_8009956C(&func_800736E0, 0);
    func_800994F4(4, &func_80008D60, 0x64);
    func_80099614(2);
}

extern void func_800737FC(void);

void func_800736E0(void) {
    if (D_801235B4 == 1) {
        D_801235B4 = 0;
        func_8009956C(&func_800734A0, 0);
    } else {
        func_8009956C(&func_800737FC, 0);
    }
}

extern void func_80006F30(void);
extern void func_8007377C(void);

void func_80073738(void) {
    func_8009956C(&func_8007377C, 0);
    func_800994F4(4, &func_80006F30, 0x64);
    func_80099614(2);
}

extern void func_80077DA0(void);

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

extern void func_80000A40(s32);
extern void func_80072114(s32);
extern void func_800994F4(s32, void *, s32);
extern void func_8000C280(void);
extern void func_8007389C(void);

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

extern void func_8000D340(void);
extern u8 D_800DC4C0;

void func_800738F4(void) {
    func_8009956C(&func_800734A0, 0);
    if (D_800DC4C0 != 0) {
        func_800994F4(4, &func_8000D340, 0x64);
        func_80099614(2);
    }
}

extern void func_800738F4(void);
extern void func_8003DFD0(void);
extern void func_800728E0(void);
extern void func_80077C4C(void);
extern void func_800740C0(void);
extern void func_80074960(void);
extern s16 D_800DEF14;

void func_80073944(void) {
    func_8009956C(&func_800738F4, 0);
    func_800994F4(4, &func_8003DFD0, 0x64);
    func_80099614(2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/73830/func_80073988.s")

extern void func_8006D520(s32, s32);
extern void func_800720E4(s32);
extern void func_80074160(void);
extern void func_80077C94(void);
extern s16 D_801124B8;

void func_800740C0(void) {
    D_800DEF14 -= 8;
    if (D_800DEF14 < 0) {
        D_800DEF14 = 0;
        D_801235B8->unk20--;
    }
    if (D_801235B8->unk20 == 0) {
        func_8006D520(0, 0x1E);
        func_800720E4(5);
        func_8009956C(&func_80074160, 0);
    }
    func_80077C94();
    D_801124B8 = 0xFF;
}

#pragma GLOBAL_ASM("asm/nonmatchings/73830/func_80074160.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/73830/func_80074864.s")

#pragma GLOBAL_ASM("asm/nonmatchings/73830/func_80074960.s")

#pragma GLOBAL_ASM("asm/nonmatchings/73830/func_80074C5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/73830/func_80074F50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/73830/func_800751C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/73830/func_80076054.s")

#pragma GLOBAL_ASM("asm/nonmatchings/73830/func_80076490.s")

#pragma GLOBAL_ASM("asm/nonmatchings/73830/func_80077324.s")

#pragma GLOBAL_ASM("asm/nonmatchings/73830/func_80077400.s")

#pragma GLOBAL_ASM("asm/nonmatchings/73830/func_80077554.s")

#pragma GLOBAL_ASM("asm/nonmatchings/73830/func_8007797C.s")

extern void func_80077B34(void);
extern void func_80077CD4(void);
extern s8 D_800EC8B0;

void func_80077AD4(void) {
    func_80077CD4();
    D_800EC8B0 = 0;
    D_800DEF14 += 7;
    if (D_800DEF14 >= 0xFF) {
        D_800DEF14 = 0xFF;
        func_8009956C(&func_80077B34, 0);
    }
}

extern void func_80042C20(void);
extern void func_80045914(void);
extern void func_80072260(void);
extern void func_80073988(void);
extern u8 D_80123750;
extern u8 D_80123751;
extern u8 D_800DEED4;
extern u8 D_80121B57;
extern u8 D_80121B58;

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

extern void func_8006D700(void);
extern void func_800710CC(s32);
extern void func_8007115C(void);
extern void func_8007AA50(void);
extern void func_8008C704(void);
extern void func_80096E3C(void);

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
    u8 *var_a0;

    if (D_800EC9C2 == 1) {
        if (!(D_8012207C & 0x40)) {
            return 0;
        }
    } else {
        var_v0 = 0;
        if ((s32)D_80121B55 > 0) {
            var_a0 = D_80121D80;
loop:
            if (!(*(s32 *)(var_a0 + 0x2FC) & 0x40)) {
                return 0;
            }
            var_v0++;
            var_a0 += 0x60C;
            if (var_v0 < (s32)D_80121B55) {
                goto loop;
            }
        }
    }
    D_801235B4 |= 2;
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/73830/func_80077DA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/73830/func_80078078.s")

extern void func_8007105C(void);
extern void func_800781FC(void);
extern s16 D_800DEF14;

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
