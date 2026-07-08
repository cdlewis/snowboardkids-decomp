#include "common.h"
#include "asset_decompression.h"
#include "character_select_flow.h"
#include "controller_pak_menu.h"

extern s32 func_80013F88(s32, s32, s32);
extern void func_80072138(s32, s32);
extern void func_8009956C(void *, s32);
extern void func_8007105C(void);
extern void func_80045914(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);

extern void func_80071408(void *, s32, s32);
extern void func_80000C48(s32);
extern void func_800704F0(void);
extern void func_8007066C(s32, s32, s32, s32, s32, s32, s32, f32);
extern void func_80070EC0(s32);
extern void func_80001618(void);
extern void func_800016D8(u16);
extern void func_80001858(void);
extern void func_8000C818(void);
extern void func_8000C924(void);
extern void func_8000CB08(void);
extern void func_8000CC5C(void);
extern void func_8000CEA0(void);
extern void func_8000CE0C(void);
extern void func_8003205C(void *);
extern u8 func_800325D0;
extern u8 func_800310EC;
extern u8 func_80031330;
extern u8 func_80031510;
extern u8 func_80031B24;
extern u8 func_80031C04;

extern ControllerPakMenuState D_8010AF90;
extern ControllerPakFileEntry D_8010AFA0[];
extern CharacterSelectFlowState *D_801235B8;
extern s8 D_800DEED4;
extern u8 D_800EC9D8;
extern u8 D_8010AF93;
extern u8 D_8010AF96;
extern s32 D_80123778;
extern u8 D_80123750;
extern u8 D_80123751;
extern s16 D_800DEF14;
extern s16 D_800EC9C8;
extern s8 D_800EC8B0;
extern s32 D_8010B198;
extern s32 D_8010B19C;
extern s32 D_801235B4;
extern u8 D_593D10;
extern u8 D_598A70;
extern u8 D_59AAA0;
extern u8 D_59DFE0;
extern u8 D_5DFDD0;
extern u8 D_5E0350;
extern u8 D_5E0E40;
extern u8 D_60F1A0;
extern u8 D_60F990;

void func_8000C600(void) {
    func_80000C48(0);
    if (D_800EC9C8 != 1) {
        func_8009956C(func_8000CEA0, 0);
        return;
    }
    func_800704F0();
    func_8007066C(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.333333373f);
    D_800DEED4 = 0;
    D_801235B8->fade = 0;
    D_801235B8->timer = 0;
    D_801235B4 = 0;
    D_800EC9D8 = 0;
    D_8010B198 = 0;
    D_8010B19C = 0;
    D_800EC8B0 = 0;
    D_800DEF14 = D_801235B8->fade;
    func_800437F0(&D_5DFDD0, &D_5E0350, 0x21);
    func_800437F0(&D_593D10, &D_598A70, 0x22);
    func_800437F0(&D_598A70, &D_59AAA0, 0x23);
    func_800437F0(&D_59AAA0, &D_59DFE0, 0x24);
    func_800437F0(&D_5E0350, &D_5E0E40, 0x26);
    func_800437F0(&D_60F1A0, &D_60F990, 0x29);
    func_80070EC0(0);
    func_80071408(&func_800310EC, 0, 0x63);
    func_80071408(&func_80031330, 0, 0x63);
    func_80071408(&func_80031510, 0, 0x63);
    func_80071408(&func_80031B24, 0, 0x63);
    func_80071408(&func_80031C04, 0, 0x5E);
    D_8010AF90.mainChoice = 0;
    D_8010AF90.fileIndex = 0;
    D_8010AF90.confirmChoice = 0;
    D_8010AF90.state = 0;
    D_8010AF90.visibleFileIndex = 0;
    D_8010AF90.isEdgeScroll = 0;
    func_80001618();
    func_80001858();
    func_8009956C(func_8000C818, 0);
}

void func_8000C818(void) {
    if ((D_80123778 & 0x40100) && (D_8010AF90.mainChoice != 1)) {
        D_8010AF90.mainChoice = 1;
        func_80072138(0x19, 0x32);
    } else if ((D_80123778 & 0x80200) && (D_8010AF90.mainChoice != 0)) {
        D_8010AF90.mainChoice = 0;
        func_80072138(0x19, 0x32);
    }
    if ((D_80123778 & 0x8000) || (D_80123778 & 0x1000)) {
        func_80072138(0x18, 0x32);
        if (D_8010AF90.mainChoice == 1) {
            func_8009956C(func_8000CEA0, 0);
        } else {
            func_8009956C(func_8000C924, 0);
            D_8010AF90.state = 1;
            D_8010AF90.isEdgeScroll = 0;
        }
    }
    func_8007105C();
}

void func_8000C924(void) {
    u8 direction = 0;

    if ((D_80123778 & 0x10800) && (D_8010AF90.fileIndex != 0)) {
        D_8010AF90.fileIndex--;
        direction = 1;
    } else if ((D_80123778 & 0x20400) && (D_8010AF90.fileIndex != 0xF)) {
        D_8010AF90.fileIndex++;
        direction = 2;
    }
    if (direction == 0) {
        D_8010AF90.isEdgeScroll = 0;
    } else {
        func_80072138(0x19, 0x32);
        if ((D_8010AF90.visibleFileIndex != 0) && (D_8010AF90.visibleFileIndex != 4)) {
            D_8010AF90.isEdgeScroll = 0;
        } else if (D_8010AF90.visibleFileIndex == 0 && direction == 2) {
            D_8010AF90.isEdgeScroll = 0;
        } else {
            if ((D_8010AF90.visibleFileIndex == 4) && (direction == 1)) {
                D_8010AF90.isEdgeScroll = 0;
            } else {
                D_8010AF90.isEdgeScroll = 1;
            }
        }

        if (D_8010AF90.isEdgeScroll == 0) {
            if (direction == 1) {
                D_8010AF90.visibleFileIndex--;
            } else {
                D_8010AF90.visibleFileIndex++;
            }
        }
    }

    if (((D_80123778 & 0x8000) || (D_80123778 & 0x4000, ((D_80123778 & 0x1000) != 0))) &&
        (D_80123778 & 0x4000, (D_8010AFA0[D_8010AF90.fileIndex].exists != 0))) {
        func_80072138(0x18, 0x32);
        func_8009956C(func_8000CB08, 0);
        D_8010AF90.state = 2;
        D_8010AF90.confirmChoice = 1;
    } else if ((D_80123778 & 0x4000) != 0) {
        func_80072138(0x18, 0x32);
        func_8009956C(func_8000C818, 0);
        D_8010AF90.state = 0;
    }
    func_8007105C();
}

void func_8000CB08(void) {
    if ((D_80123778 & 0x40100) && (D_8010AF90.confirmChoice != 1)) {
        D_8010AF90.confirmChoice = 1;
        func_80072138(0x19, 0x32);
    } else if ((D_80123778 & 0x80200) && (D_8010AF90.confirmChoice != 0)) {
        D_8010AF90.confirmChoice = 0;
        func_80072138(0x19, 0x32);
    }
    if ((D_80123778 & 0x8000) || (D_80123778 & 0x1000)) {
        func_80072138(0x18, 0x32);
        if (D_8010AF90.confirmChoice == 0) {
            func_80071408(&func_800325D0, 0, 0x64);
            func_8009956C(func_8000CC5C, 0);
            D_8010AF90.state = 3;
            D_8010AF90.confirmChoice = 1;
        } else {
            func_8009956C(func_8000C924, 0);
            D_8010AF90.state = 1;
        }
    } else if (D_80123778 & 0x4000) {
        func_80072138(0x18, 0x32);
        func_8009956C(func_8000C924, 0);
        D_8010AF90.state = 1;
    }
    func_8007105C();
}

void func_8000CC5C(void) {
    if ((D_80123778 & 0x10800) && (D_8010AF90.confirmChoice != 0)) {
        D_8010AF90.confirmChoice = 0;
        func_80072138(0x19, 0x32);
    } else if ((D_80123778 & 0x20400) && (D_8010AF90.confirmChoice != 1)) {
        D_8010AF90.confirmChoice = 1;
        func_80072138(0x19, 0x32);
    }
    if ((D_80123778 & 0x8000) || (D_80123778 & 0x1000)) {
        func_80072138(0x18, 0x32);
        if (D_8010AF90.confirmChoice == 0) {
            func_800016D8(D_8010AF90.fileIndex);
            if (D_800EC9D8 == 0) {
                D_8010AFA0[D_8010AF90.fileIndex].exists = 0;
                func_80001618();
                func_80001858();
                func_8009956C(func_8000C924, 0);
                D_8010AF90.state = 1;
            } else {
                func_80071408(func_8003205C, 0, 0x64);
                D_8010AF90.unk6 = 0;
                D_8010AF90.state = 4;
                func_8009956C(func_8000CE0C, 0);
            }
        } else {
            func_8009956C(func_8000CB08, 0);
            D_8010AF90.state = 2;
            D_8010AF90.confirmChoice = 0;
        }
    } else if (D_80123778 & 0x4000) {
        func_80072138(0x18, 0x32);
        func_8009956C(func_8000CB08, 0);
        D_8010AF90.state = 2;
        D_8010AF90.confirmChoice = 0;
    }
    func_8007105C();
}

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
            func_80099658(0);
            func_8009954C(4);
        }
    }
}
