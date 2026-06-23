#include "common.h"

extern void func_8008B408(void *, s32, s32);
extern void func_8008A940(void);
extern void func_8008BE1C(void *);
extern void func_80082664(void *, s32, s32, s32);
extern void func_80081E40(void *, s32);
extern s32 func_80082EC0(void *);
extern void func_8008BBB8(void *, s32);
extern s32 func_80095F90(s32);
extern void func_80072A74(s32, void *, s32, s32);

extern void (*D_800DECD0[])(void *);
extern void (*D_800DECD8[])(void *);
extern void (*D_800DECE8[])(void *);
extern void (*D_800DED08[])(void *);
extern void (*D_800DED18[])(void *);
extern void (*D_800DED30[])(void *);

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008BEB0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008BF5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008C098.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008C704.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008C7D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008CF10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008D09C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008DAF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008DC2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008DE1C.s")

void func_8008DFD0(void *arg0) {
    D_800DECD0[*(s16 *)((u8 *)arg0 + 0x302)](arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008E008.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008E350.s")

void func_8008F1B4(void *arg0) {
    *(u16 *)((u8 *)arg0 + 0x308) = 0;
    *(u16 *)((u8 *)arg0 + 0x30A) = 0;
    *(u16 *)((u8 *)arg0 + 0x30C) = 0;
    *(u16 *)((u8 *)arg0 + 0x30E) = 0;
    *(u16 *)((u8 *)arg0 + 0x2A0) = 0;
}

void func_8008F1CC(void *arg0) {
    D_800DECD8[*(s16 *)((u8 *)arg0 + 0x308)](arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008F204.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008F3C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008F514.s")

void func_8008F560(s32 arg0) {

}

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008F568.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008F6C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008F82C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008F9CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008FB6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008FD2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8008FEEC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_800900B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80090274.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80090470.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_800905BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80090708.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80090898.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80090998.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80090B30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80090CD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80090ECC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8009107C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80091250.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80091400.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_800915C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_800917BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_800919A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80091AF8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80091D40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80091F3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80092194.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80092368.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80092468.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80092674.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80092774.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_800929E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80092B6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80092D04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80092E58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80093144.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80093304.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_800934EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_800936D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_800939E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80093E0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80094288.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80094480.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8009469C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80094808.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8009491C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80094A94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80094BEC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80094DF8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80094FF4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80095164.s")

void func_80095300(void *arg0) {
    D_800DECE8[*(s16 *)((u8 *)arg0 + 0x302)](arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80095338.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_800955C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80095650.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80095804.s")

void func_80095940(void *arg0) {
    if (*(s16 *)((u8 *)arg0 + 0x304) == 0) {
        *(s16 *)((u8 *)arg0 + 0x304) = *(s16 *)((u8 *)arg0 + 0x304) + 1;
        func_80081E40(arg0, 0x1D);
        *(s32 *)((u8 *)arg0 + 0x7C) = 0x13;
    }
    func_80082EC0(arg0);
    *(s32 *)((u8 *)arg0 + 0x7C) = *(s32 *)((u8 *)arg0 + 0x7C) - 1;
    if (*(s32 *)((u8 *)arg0 + 0x7C) == 0) {
        *(s16 *)((u8 *)arg0 + 0x304) = 0;
        *(s16 *)((u8 *)arg0 + 0x302) = *(s16 *)((u8 *)arg0 + 0x302) + 1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_800959B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80095A88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80095BE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80095F90.s")

void func_8009617C(void *arg0) {
    D_800DED08[*(s16 *)((u8 *)arg0 + 0x302)](arg0);
    if (*(s32 *)((u8 *)arg0 + 0x280) < 0xF0000) {
        *(s32 *)((u8 *)arg0 + 0x280) += 0x8000;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_800961DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8009652C.s")

void func_800965E0(void *arg0) {
    func_8008B408(arg0, 0, 0);
    D_800DED18[*(u8 *)((u8 *)arg0 + 0x10)](arg0);
}

void func_80096630(void *arg0) {
    if (*(s16 *)((u8 *)arg0 + 0x304) == 0) {
        *(s16 *)((u8 *)arg0 + 0x304) = *(s16 *)((u8 *)arg0 + 0x304) + 1;
        *(s32 *)((u8 *)arg0 + 0x7C) = 0;
        func_80081E40(arg0, 0x23);
    }
    if (func_80082EC0(arg0)) {
        if (*(s16 *)((u8 *)arg0 + 0x452) == 0x24) {
            *(s32 *)((u8 *)arg0 + 0x2FC) |= 0x8000000;
        }
        if (*(s32 *)((u8 *)arg0 + 0x7C) == 0) {
            if (*(s16 *)((u8 *)arg0 + 0x304) == 1) {
                func_8008BBB8(arg0, 6);
                *(s16 *)((u8 *)arg0 + 0x304) = *(s16 *)((u8 *)arg0 + 0x304) + 1;
            }
            *(s32 *)((u8 *)arg0 + 0x7C) = 0x14;
            func_80081E40(arg0, 0x24);
        } else {
            *(s32 *)((u8 *)arg0 + 0x7C) = *(s32 *)((u8 *)arg0 + 0x7C) - 1;
        }
    }
}

void func_800966F4(void *arg0) {
    if (*(s16 *)((u8 *)arg0 + 0x304) == 0) {
        *(s16 *)((u8 *)arg0 + 0x304) = *(s16 *)((u8 *)arg0 + 0x304) + 1;
        *(s32 *)((u8 *)arg0 + 0x7C) = 0;
        func_80081E40(arg0, 0x24);
    }
    if (func_80082EC0(arg0)) {
        if (*(s16 *)((u8 *)arg0 + 0x452) == 0x25) {
            *(s32 *)((u8 *)arg0 + 0x2FC) |= 0x8000000;
        }
        if (*(s32 *)((u8 *)arg0 + 0x7C) == 0) {
            if (*(s16 *)((u8 *)arg0 + 0x304) == 1) {
                func_8008BBB8(arg0, 6);
                *(s16 *)((u8 *)arg0 + 0x304) = *(s16 *)((u8 *)arg0 + 0x304) + 1;
            }
            func_80081E40(arg0, 0x25);
        } else {
            *(s32 *)((u8 *)arg0 + 0x7C) = *(s32 *)((u8 *)arg0 + 0x7C) - 1;
        }
    }
}

void func_800967B0(void *arg0) {
    if (*(s16 *)((u8 *)arg0 + 0x304) == 0) {
        *(s16 *)((u8 *)arg0 + 0x304) = *(s16 *)((u8 *)arg0 + 0x304) + 1;
        *(s32 *)((u8 *)arg0 + 0x7C) = 0;
        func_80081E40(arg0, 0x23);
    }
    if (func_80082EC0(arg0)) {
        if (*(s16 *)((u8 *)arg0 + 0x452) == 0x24) {
            *(s32 *)((u8 *)arg0 + 0x2FC) |= 0x8000000;
        }
        if (*(s32 *)((u8 *)arg0 + 0x7C) == 0) {
            if (*(s16 *)((u8 *)arg0 + 0x304) == 1) {
                func_8008BBB8(arg0, 6);
                *(s16 *)((u8 *)arg0 + 0x304) = *(s16 *)((u8 *)arg0 + 0x304) + 1;
            }
            *(s32 *)((u8 *)arg0 + 0x7C) = 0x14;
            func_80081E40(arg0, 0x24);
        } else {
            *(s32 *)((u8 *)arg0 + 0x7C) = *(s32 *)((u8 *)arg0 + 0x7C) - 1;
        }
    }
}

void func_80096874(void *arg0) {
    if (*(s16 *)((u8 *)arg0 + 0x304) == 0) {
        *(s16 *)((u8 *)arg0 + 0x304) = *(s16 *)((u8 *)arg0 + 0x304) + 1;
        *(s32 *)((u8 *)arg0 + 0x7C) = 0;
        func_80081E40(arg0, 0x24);
    }
    if (func_80082EC0(arg0)) {
        if (*(s16 *)((u8 *)arg0 + 0x452) == 0x25) {
            *(s32 *)((u8 *)arg0 + 0x2FC) |= 0x8000000;
        }
        if (*(s32 *)((u8 *)arg0 + 0x7C) == 0) {
            if (*(s16 *)((u8 *)arg0 + 0x304) == 1) {
                func_8008BBB8(arg0, 6);
                *(s16 *)((u8 *)arg0 + 0x304) = *(s16 *)((u8 *)arg0 + 0x304) + 1;
            }
            *(s32 *)((u8 *)arg0 + 0x7C) = 0;
            func_80081E40(arg0, 0x25);
        } else {
            *(s32 *)((u8 *)arg0 + 0x7C) = *(s32 *)((u8 *)arg0 + 0x7C) - 1;
        }
    }
}

void func_80096934(void *arg0) {
    if (*(s16 *)((u8 *)arg0 + 0x304) == 0) {
        *(s16 *)((u8 *)arg0 + 0x304) = *(s16 *)((u8 *)arg0 + 0x304) + 1;
        *(s32 *)((u8 *)arg0 + 0x7C) = 0;
        func_80081E40(arg0, 0x24);
    }
    if (func_80082EC0(arg0)) {
        if (*(s16 *)((u8 *)arg0 + 0x452) == 0x25) {
            *(s32 *)((u8 *)arg0 + 0x2FC) |= 0x8000000;
        }
        if (*(s32 *)((u8 *)arg0 + 0x7C) == 0) {
            if (*(s16 *)((u8 *)arg0 + 0x304) == 1) {
                func_8008BBB8(arg0, 6);
                *(s16 *)((u8 *)arg0 + 0x304) = *(s16 *)((u8 *)arg0 + 0x304) + 1;
            }
            *(s32 *)((u8 *)arg0 + 0x7C) = 0x32;
            func_80081E40(arg0, 0x25);
        } else {
            *(s32 *)((u8 *)arg0 + 0x7C) = *(s32 *)((u8 *)arg0 + 0x7C) - 1;
        }
    }
}

void func_800969F8(void *arg0) {
    if (*(s16 *)((u8 *)arg0 + 0x304) == 0) {
        *(s16 *)((u8 *)arg0 + 0x304) = *(s16 *)((u8 *)arg0 + 0x304) + 1;
        func_8008BBB8(arg0, 6);
        *(s32 *)((u8 *)arg0 + 0x7C) = 0x1E;
        func_80081E40(arg0, 0x23);
    }
    if (func_80082EC0(arg0)) {
        *(s32 *)((u8 *)arg0 + 0x2FC) |= 0x8000000;
        if (*(s32 *)((u8 *)arg0 + 0x7C) == 0) {
            *(s32 *)((u8 *)arg0 + 0x7C) = 0x1E;
            func_80081E40(arg0, 0x23);
        } else {
            *(s32 *)((u8 *)arg0 + 0x7C) = *(s32 *)((u8 *)arg0 + 0x7C) - 1;
        }
    }
}

void func_80096A8C(void *arg0) {
    func_8008B408(arg0, 0, 0);
    D_800DED30[*(u8 *)((u8 *)arg0 + 0x10)](arg0);
}

void func_80096ADC(void *arg0) {
    if (*(s16 *)((u8 *)arg0 + 0x304) == 0) {
        *(s16 *)((u8 *)arg0 + 0x304) = *(s16 *)((u8 *)arg0 + 0x304) + 1;
        *(s32 *)((u8 *)arg0 + 0x7C) = 0;
        func_80081E40(arg0, 0x25);
    }
    if (func_80082EC0(arg0)) {
        if (*(s32 *)((u8 *)arg0 + 0x7C) == 0) {
            *(s32 *)((u8 *)arg0 + 0x2FC) |= 0x8000000;
            *(s32 *)((u8 *)arg0 + 0x7C) = 0x1E;
            func_80081E40(arg0, 0x26);
        } else {
            *(s32 *)((u8 *)arg0 + 0x7C) = *(s32 *)((u8 *)arg0 + 0x7C) - 1;
        }
    }
}

void func_80096B68(void *arg0) {
    if (*(s16 *)((u8 *)arg0 + 0x304) == 0) {
        *(s16 *)((u8 *)arg0 + 0x304) = *(s16 *)((u8 *)arg0 + 0x304) + 1;
        *(s32 *)((u8 *)arg0 + 0x7C) = 0;
        func_80081E40(arg0, 0x26);
    }
    if (func_80082EC0(arg0)) {
        if (*(s32 *)((u8 *)arg0 + 0x7C) == 0) {
            *(s32 *)((u8 *)arg0 + 0x2FC) |= 0x8000000;
            *(s32 *)((u8 *)arg0 + 0x7C) = 0x1E;
            func_80081E40(arg0, 0x27);
        } else {
            *(s32 *)((u8 *)arg0 + 0x7C) = *(s32 *)((u8 *)arg0 + 0x7C) - 1;
        }
    }
}

void func_80096BF4(void *arg0) {
    if (*(s16 *)((u8 *)arg0 + 0x304) == 0) {
        *(s16 *)((u8 *)arg0 + 0x304) = *(s16 *)((u8 *)arg0 + 0x304) + 1;
        *(s32 *)((u8 *)arg0 + 0x7C) = 0;
        func_80081E40(arg0, 0x25);
    }
    if (func_80082EC0(arg0)) {
        if (*(s32 *)((u8 *)arg0 + 0x7C) == 0) {
            *(s32 *)((u8 *)arg0 + 0x7C) = 0;
            *(s32 *)((u8 *)arg0 + 0x2FC) |= 0x8000000;
            func_80081E40(arg0, 0x26);
        } else {
            *(s32 *)((u8 *)arg0 + 0x7C) = *(s32 *)((u8 *)arg0 + 0x7C) - 1;
        }
    }
}

void func_80096C7C(void *arg0) {
    if (*(s16 *)((u8 *)arg0 + 0x304) == 0) {
        *(s16 *)((u8 *)arg0 + 0x304) = *(s16 *)((u8 *)arg0 + 0x304) + 1;
        *(s32 *)((u8 *)arg0 + 0x7C) = 0;
        func_80081E40(arg0, 0x26);
    }
    if (func_80082EC0(arg0)) {
        if (*(s32 *)((u8 *)arg0 + 0x7C) == 0) {
            *(s32 *)((u8 *)arg0 + 0x2FC) |= 0x8000000;
            *(s32 *)((u8 *)arg0 + 0x7C) = 0;
            func_80081E40(arg0, 0x27);
        } else {
            *(s32 *)((u8 *)arg0 + 0x7C) = *(s32 *)((u8 *)arg0 + 0x7C) - 1;
        }
    }
}

void func_80096D04(void *arg0) {
    if (*(s16 *)((u8 *)arg0 + 0x304) == 0) {
        *(s16 *)((u8 *)arg0 + 0x304) = *(s16 *)((u8 *)arg0 + 0x304) + 1;
        *(s32 *)((u8 *)arg0 + 0x7C) = 0;
        func_80081E40(arg0, 0x26);
    }
    if (func_80082EC0(arg0)) {
        if (*(s32 *)((u8 *)arg0 + 0x7C) == 0) {
            *(s32 *)((u8 *)arg0 + 0x2FC) |= 0x8000000;
            *(s32 *)((u8 *)arg0 + 0x7C) = 0;
            func_80081E40(arg0, 0x27);
        } else {
            *(s32 *)((u8 *)arg0 + 0x7C) = *(s32 *)((u8 *)arg0 + 0x7C) - 1;
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80096D8C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80096E3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80097038.s")

extern s32 func_80089374(void);

void func_80097144(void) {
    func_80089374();
}

void func_80097164(void *arg0) {
    if (*(s16 *)((u8 *)arg0 + 0x302) < 7) {
        func_8008A940();
    } else {
        *(s32 *)((u8 *)arg0 + 0x64) = 0;
        func_8008BE1C(arg0);
        *(s32 *)((u8 *)arg0 + 0x58) = 0x30000;
    }
}

void func_800971B8(void *arg0) {
    if (func_80089374()) {
        *(s16 *)((u8 *)arg0 + 0x300) = 0x16;
        *(s16 *)((u8 *)arg0 + 0x302) = 0;
        *(s16 *)((u8 *)arg0 + 0x304) = 0;
        *(s32 *)((u8 *)arg0 + 0x2FC) |= 0x200;
    } else if (func_80095F90(*(s16 *)arg0)) {
        *(s16 *)((u8 *)arg0 + 0x300) = 7;
        *(s16 *)((u8 *)arg0 + 0x302) = 0;
        *(s16 *)((u8 *)arg0 + 0x304) = 0;
    }
    func_80082664(arg0, 0, (0x60000 - *(s32 *)((u8 *)arg0 + 0x58)) / 0x600, 0x100);
}

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8009724C.s")

void func_8009759C(void *arg0) {
    s32 v0 = -*(s32 *)((u8 *)arg0 + 0x44) >> 13;
    if (v0 >= 0x80) {
        v0 = 0x7F;
    }
    if (v0 < 0x46) {
        v0 = 0x46;
    }
    if (v0 >= 0x51) {
        *(s16 *)((u8 *)arg0 + 0x604) = 2;
        *(s8 *)((u8 *)arg0 + 0x608) = 0;
    } else {
        *(s16 *)((u8 *)arg0 + 0x604) = 1;
        *(s8 *)((u8 *)arg0 + 0x608) = 0;
    }
    if (*(s8 *)((u8 *)arg0 + 0x14) == 0) {
        func_80072A74(0x21, (u8 *)arg0 + 0x1C, (s16)v0, 0x32);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_8009762C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80097744.s")

void func_8009782C(void *arg0) {
    if (func_80089374()) {
        *(s16 *)((u8 *)arg0 + 0x300) = 0x16;
        *(s16 *)((u8 *)arg0 + 0x302) = 0;
        *(s16 *)((u8 *)arg0 + 0x304) = 0;
        *(s32 *)((u8 *)arg0 + 0x2FC) |= 0x200;
    } else if (func_80095F90(*(s16 *)arg0)) {
        *(s16 *)((u8 *)arg0 + 0x300) = 7;
        *(s16 *)((u8 *)arg0 + 0x302) = 0;
        *(s16 *)((u8 *)arg0 + 0x304) = 0;
    }
    func_80082664(arg0, 0, (0x60000 - *(s32 *)((u8 *)arg0 + 0x58)) / 0x600, 0x100);
}

void func_800978C0(void *arg0) {
    if (func_80089374()) {
        *(s32 *)((u8 *)arg0 + 0x2FC) |= 0x200;
    } else {
        *(s32 *)((u8 *)arg0 + 0x2FC) &= ~0x200;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/8CAB0/func_80097910.s")

void func_80097A14(void *arg0) {
    func_80089374();
    if (*(s16 *)((u8 *)arg0 + 0x302) < 2) {
        func_80082664(arg0, 0, (0x60000 - *(s32 *)((u8 *)arg0 + 0x58)) / 0x600, 0x100);
    }
}
