#include "common.h"

extern s32 D_80121D50;

#pragma GLOBAL_ASM("asm/nonmatchings/85B50/func_80084F50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/85B50/func_80085010.s")

s32 func_800850D0(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x2FC) & 0x37D000) {
        return 1;
    }
    if (D_80121D50 < 7) {
        D_80121D50 = 7;
    }
    return 0;
}

s32 func_80085118(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x2FC) & 0x77D000) {
        return 1;
    }
    if (D_80121D50 < 0xB) {
        D_80121D50 = 0xB;
    }
    return 0;
}

s32 func_80085160(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x2FC) & 0x77D000) {
        return 1;
    }
    if (D_80121D50 < 0xA) {
        D_80121D50 = 0xA;
    }
    return 0;
}

s32 func_800851A8(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x2FC) & 0x77D000) {
        return 1;
    }
    if (D_80121D50 < 0xC) {
        D_80121D50 = 0xC;
    }
    return 0;
}

s32 func_800851F0(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x2FC) & 0x77D000) {
        return 1;
    }
    if (D_80121D50 < 0xD) {
        D_80121D50 = 0xD;
    }
    return 0;
}

s32 func_80085238(void *arg0) {
    s32 temp_v0 = *(s32 *)((u8 *)arg0 + 0x2FC);

    if (temp_v0 & 0x43000) {
        return 1;
    }
    if (!(temp_v0 & 0x800)) {
        return 1;
    }
    if (D_80121D50 < 8) {
        D_80121D50 = 8;
    }
    return 0;
}

s32 func_80085290(void *arg0) {
    s32 temp_v0 = *(s32 *)((u8 *)arg0 + 0x2FC);

    if (temp_v0 & 0x43000) {
        return 1;
    }
    if (!(temp_v0 & 0x800)) {
        return 1;
    }
    if (D_80121D50 < 9) {
        D_80121D50 = 9;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/85B50/func_800852E8.s")

s32 func_800853A0(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x2FC) & 0x43000) {
        return 1;
    }
    if (D_80121D50 <= 0) {
        D_80121D50 = 1;
    }
    return 0;
}

s32 func_800853E4(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x2FC) & 0x345001) {
        return 1;
    }
    if (D_80121D50 < 0xE) {
        D_80121D50 = 0xE;
    }
    return 0;
}

s32 func_8008542C(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x2FC) & 0x775000) {
        return 1;
    }
    if (D_80121D50 < 0xF) {
        D_80121D50 = 0xF;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/85B50/func_80085474.s")

#pragma GLOBAL_ASM("asm/nonmatchings/85B50/func_800854E8.s")

s32 func_80085544(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x2FC) & 0x775000) {
        return 1;
    }
    if (D_80121D50 < 3) {
        D_80121D50 = 3;
    }
    return 0;
}

s32 func_8008558C(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x2FC) & 0x775000) {
        return 1;
    }
    if (D_80121D50 < 4) {
        D_80121D50 = 4;
    }
    return 0;
}

s32 func_800855D4(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x2FC) & 0x41001) {
        return 1;
    }
    if (D_80121D50 < 0x12) {
        D_80121D50 = 0x12;
    }
    return 0;
}

s32 func_8008561C(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x2FC) & 0x41001) {
        return 1;
    }
    if (D_80121D50 < 0x13) {
        D_80121D50 = 0x13;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/85B50/func_80085664.s")

#pragma GLOBAL_ASM("asm/nonmatchings/85B50/func_800860A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/85B50/func_80086170.s")
