#include "common.h"

typedef struct Node9CE70 {
    struct Node9CE70 *next;
    void *mq;
} Node9CE70;

typedef struct Struct800A0138 {
    u8 unk0;
    u8 pad1[3];
    s32 unk4;
    s32 unk8;
} Struct800A0138;

extern void osSendMesg(void *, s32, s32);
extern s32 osSetIntMask(s32);
extern s32 osRecvMesg(void *, void *, s32);
extern s32 osSpTaskYielded(void *);
extern void *osViGetCurrentFramebuffer(void);
extern void *osViGetNextFramebuffer(void);
extern void osWritebackDCacheAll(void);
extern void osSpTaskLoad(void *);
extern void osSpTaskStartGo(void *);
extern u32 osAiGetLength(void);
extern Struct800A0138 D_8015C928;
extern s32 D_8015C964;
extern void func_8009CD18();
extern void func_8009F604(void);
extern s8 func_8009F4C8(u8, u8 *, void *);
extern s32 func_8009F780(void *, s32, s32, s32, s32);
extern void func_8009FF80(void);
extern s32 D_800DF154;
extern s32 D_800DF158;
extern s32 D_800DF2A4;
extern s32 D_8015A680;
extern s32 D_8015A620;
extern s32 *libmus_fxheader_current;

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009C270.s")

s32 func_8009C434(s32 arg0) {
    return arg0 + 4;
}

s32 func_8009C43C(s32 arg0) {
    return arg0 + 0x5C;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009C444.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009C6DC.s")

void func_8009C77C(void *arg0) {
    if (D_800DF158 != 0) {
        D_800DF158 = 0;
        if (osSpTaskYielded((u8 *)*(s32 *)((u8 *)arg0 + 0x76C) + 0x10) != 0) {
            D_800DF154 = 1;
        } else {
            D_800DF154 = 2;
        }
    }
    D_8015A620 |= 2;
    osWritebackDCacheAll();
    osSpTaskLoad((u8 *)*(s32 *)((u8 *)arg0 + 0x770) + 0x10);
    osSpTaskStartGo((u8 *)*(s32 *)((u8 *)arg0 + 0x770) + 0x10);
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009C81C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009C8DC.s")

void func_8009CA60(void *arg0, Node9CE70 *arg1, void *arg2) {
    s32 prev = osSetIntMask(1);
    arg1->mq = arg2;
    arg1->next = *(Node9CE70 **)((u8 *)arg0 + 0x768);
    *(Node9CE70 **)((u8 *)arg0 + 0x768) = arg1;
    osSetIntMask(prev);
}

void func_8009CAB4(void *arg0, Node9CE70 *arg1) {
    Node9CE70 *node;
    Node9CE70 *prev;
    s32 mask;

    node = *(Node9CE70 **)((u8 *)arg0 + 0x768);
    prev = NULL;
    mask = osSetIntMask(1);
    if (node != NULL) {
        do {
            if (node == arg1) {
                if (prev != NULL) {
                    prev->next = arg1->next;
                } else {
                    *(Node9CE70 **)((u8 *)arg0 + 0x768) = arg1->next;
                }
                break;
            }
            prev = node;
            node = node->next;
        } while (node != NULL);
    }
    osSetIntMask(mask);
}

void func_8009CB44(void *arg0, s32 arg1) {
    Node9CE70 *node = *(Node9CE70 **)((u8 *)arg0 + 0x768);
    while (node != NULL) {
        osSendMesg(node->mq, arg1, 0);
        node = node->next;
    }
}

void func_8009CB98(void *arg0, void *arg1) {
    void *msg;
    Node9CE70 node;
    void *framebuffer;
    void *queue;

    msg = 0;
    framebuffer = *(void **)((u8 *)arg1 + 0xC);
    while ((osViGetCurrentFramebuffer() == framebuffer) || (osViGetNextFramebuffer() == framebuffer)) {
        queue = (u8 *)arg0 + 0x1FC;
        func_8009CA60(arg0, &node, queue);
        osRecvMesg(queue, &msg, 1);
        func_8009CAB4(arg0, &node);
    }
}

void func_8009CC50(void *arg0) {
    void *msg;
    Node9CE70 node;

    msg = 0;
    func_8009CA60(arg0, &node, (u8 *)arg0 + 0x1FC);
    osRecvMesg((void *)((u8 *)arg0 + 0x1FC), &msg, 1);
    func_8009CAB4(arg0, &node);
}

s32 func_8009CCA0(void *arg0, s32 arg1) {
    *(s32 *)((u8 *)arg0 + 0x60) = 0;
    *(s32 *)((u8 *)arg0 + 0x68) = 0;
    *(s32 *)((u8 *)arg0 + 0x54) = 0;
    *(u16 *)((u8 *)arg0 + 0xBE) = 0;
    *(s32 *)((u8 *)arg0 + 0x14) = 0;
    return 0;
}

void *func_8009CCC0(void *arg0, u8 *arg1) {
    u16 v;
    u8 *new_var;

    v = *arg1;
    arg1++;
    if (v & 0x80) {
        v = ((v & 0x7F) << 1) << 7;
        v |= *(new_var = arg1);
        arg1++;
    }
    *(u16 *)((u8 *)arg0 + 0xCC) = v;
    return arg1;
}

s32 func_8009CCFC(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0xEA) = *arg1;
    return (s32)(arg1 + 1);
}

s32 func_8009CD0C(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xEA) = 0;
    return arg1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009CD18.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009CE3C.s")

s32 func_8009CF1C(void *arg0, u8 *arg1) {
    u8 *ret;
    u8 *temp;

    *(u16 *)((u8 *)arg0 + 0xC4) = *arg1;
    ;
    ret = (temp = arg1) + 1;
    *(u16 *)((u8 *)arg0 + 0xC2) = 0;
    arg1++;
    arg1--;
    return (s32)ret;
}

s32 func_8009CF30(void *arg0, u8 *arg1) {
    u8 *ret;
    u8 *temp;
    u32 v;
    short new_var;

    new_var = arg1[0] << 8;
    new_var = arg1[1] | ((0, new_var));
    v = new_var;
    ret = (temp = arg1) + 2;
    *(u16 *)((u8 *)arg0 + 0xC2) = v;
    *(u16 *)((u8 *)arg0 + 0xC4) = 0;
    arg1 += 2;
    arg1 -= 2;
    return (s32)ret;
}

s32 func_8009CF50(void *arg0, u8 *arg1) {
    u32 temp_t8;
    f32 var_ft1;

    *(u8 *)((u8 *)arg0 + 0x105) = arg1[0];
    *(u8 *)((u8 *)arg0 + 0xE9) = arg1[1];
    arg1 += 2;
    temp_t8 = arg1[0];
    var_ft1 = (f32)(temp_t8 & 0xFFFF);
    *(f32 *)((u8 *)arg0 + 0x44) = (f32)(((double)var_ft1) / 50.0);
    return (s32)(arg1 + 1);
}

s32 func_8009CFAC(void *arg0, u8 *arg1) {
    u32 temp_t8;
    f32 var_ft1;

    *(u8 *)((u8 *)arg0 + 0x105) = arg1[0];
    *(u8 *)((u8 *)arg0 + 0xE9) = arg1[1];
    arg1 += 2;
    temp_t8 = arg1[0];
    var_ft1 = (f32)(temp_t8 & 0xFFFF);
    *(f32 *)((u8 *)arg0 + 0x44) = (f32)(((double)-var_ft1) / 50.0);
    return (s32)(arg1 + 1);
}

s32 func_8009D00C(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xE9) = 0;
    *(f32 *)((u8 *)arg0 + 0x48) = 0.0f;
    return arg1;
}

s32 func_8009D020(void *arg0, u8 *arg1) {
    u8 b;
    unsigned int new_var;
    s16 value;

    b = arg1[0];
    arg1++;
    if (b < 0x80) {
        *(s16 *)((u8 *)arg0 + 0xC0) = b;
    } else {
        value = ((b & 0x7F) << 6) << 2;
        *(s16 *)((u8 *)arg0 + 0xC0) = value;
        new_var = arg1[0];
        b = new_var;
        *(s16 *)((u8 *)arg0 + 0xC0) = value + b;
        arg1++;
    }
    return (s32)arg1;
}

s32 func_8009D064(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xE6) = 1;
    return arg1;
}

s32 func_8009D074(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0x118) = *arg1;
    return (s32)(arg1 + 1);
}

s32 func_8009D084(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xE7) = 1;
    return arg1;
}

s32 func_8009D094(void *arg0, u8 *arg1) {
    s32 b;
    f32 f;
    b = *arg1;
    arg1++;
    if (b & 0x80) {
        b |= 0xFFFFFF00;
    }
    f = (f32)b;
    *(f32 *)((u8 *)arg0 + 0x2C) = f / 100.0;
    return (s32)arg1;
}

s32 func_8009D0E0(void *arg0, u8 *arg1) {
    s32 var_v0;

    var_v0 = arg1[0];
    arg1 += 1;
    if (var_v0 & 0x80) {
        var_v0 = arg1[0] | ((0, (var_v0 & 0x7F) << 8));
        arg1 += 1;
    }
    func_8009CD18(arg0, (u8 *)*(s32 *)(*(s32 *)((u8 *)arg0 + 0x54) + 0x10) + (var_v0 * 7));
    return (s32)arg1;
}

s32 func_8009D138(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xE8) = 1;
    return arg1;
}

s32 func_8009D148(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xE8) = 0;
    return arg1;
}

s32 func_8009D154(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xE5) = 1;
    return arg1;
}

s32 func_8009D164(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xE5) = 0;
    return arg1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D170.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D1EC.s")

s32 func_8009D27C(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0x119) = arg1[0];
    *(u8 *)((u8 *)arg0 + 0xEB) = arg1[1];
    *(u8 *)((u8 *)arg0 + 0x106) = arg1[2];
    return (s32)(arg1 + 3);
}

s32 func_8009D2A0(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xEB) = 0;
    return arg1;
}

s32 func_8009D2AC(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xED) = 1;
    return arg1;
}

s32 func_8009D2BC(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xED) = 0;
    return arg1;
}

s32 func_8009D2C8(void *arg0, u8 *arg1) {
    s32 ret;

    ret = arg1 + 1;
    *(u8 *)((u8 *)arg0 + 0xEE) = arg1[0];
    if (ret) {
    }
    *(u8 *)((u8 *)arg0 + 0xED) = 0;
    return ret;
}

s32 func_8009D2DC(void *arg0, u8 *arg1) {
    *(s8 *)((u8 *)arg0 + 0xF2) = (s32)(u8)*arg1 / 2;
    return (s32)(arg1 + 1);
}

s32 func_8009D2FC(s32 arg0, s32 arg1) {
    return arg1 + 2;
}

void *func_8009D308(void *arg0, u8 *arg1) {
    u8 *temp_a2;
    s32 phi;

    temp_a2 = *(u8 **)((s32)arg0 + 0x54);
    phi = (s32)temp_a2 + (*(s32 **)((s32)(*(u8 **)((s32)arg0 + 0x54)) + 0x14))[*arg1];
    *(s32 *)((s32)arg0 + 0x70) = phi;
    return arg1 + 1;
}

s32 func_8009D330(void *arg0, s32 arg1) {
    *(s32 *)((s32)arg0 + 0x70) = 0;
    return arg1;
}

s32 func_8009D33C(s32 arg0, s32 arg1) {
    return arg1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D348.s")

s32 func_8009D3AC(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0xF3) = *arg1;
    return (s32)(arg1 + 1);
}

s32 func_8009D3BC(void *arg0, u8 *arg1) {
    u8 temp_a0;

    temp_a0 = *arg1;
    arg1++;
    *(s8 *)((u8 *)arg0 + 0x118) = func_8009F4C8(temp_a0, arg1, arg0);
    *(s8 *)((u8 *)arg0 + 0x118) = *arg1 + *(s8 *)((u8 *)arg0 + 0x118);
    return (s32)(arg1 + 1);
}

s32 func_8009D40C(void *arg0, u8 *arg1) {
    u8 temp_a0;
    s8 temp_v0;

    temp_a0 = *arg1;
    arg1++;
    temp_v0 = func_8009F4C8(temp_a0, arg1, arg0);
    *(s8 *)((u8 *)arg0 + 0xEF) = temp_v0;
    *(s8 *)((u8 *)arg0 + 0xEF) = *arg1 + (temp_a0 = temp_v0);
    return (s32)(arg1 + 1);
}

s32 func_8009D45C(void *arg0, u8 *arg1) {
    u8 temp_a0;
    s8 temp_v0;

    temp_a0 = *arg1;
    arg1++;
    temp_v0 = func_8009F4C8(temp_a0, arg1, arg0);
    *(s8 *)((u8 *)arg0 + 0xF2) = temp_v0;
    *(s8 *)((u8 *)arg0 + 0xF2) = *arg1 + (temp_a0 = temp_v0);
    return (s32)(arg1 + 1);
}

s32 func_8009D4AC(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0xEF) = *arg1;
    return (s32)(arg1 + 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D4BC.s")

s32 func_8009D598(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0xF1) = *arg1;
    return (s32)(arg1 + 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D5A8.s")

extern s16 D_8015A67C;
extern s16 D_8015A67E;
extern s32 D_8015A658;
extern void *D_8015A660;
extern s32 *D_8015A670;

void func_8009D8B0(s32 arg0, s32 arg1) {
    s32 temp_t7 = arg0 & 2;

    if (arg0 & 1) {
        D_8015A67C = arg1;
    }
    if (temp_t7) {
        D_8015A67E = arg1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009D8D8.s")

s32 func_8009DBE4(s32 arg0) {
    s32 i;
    void *entry;
    s32 value;

    value = D_8015A670[arg0];
    entry = D_8015A660;
    i = 0;
    if (D_8015A658 > 0) {
        do {
            i++;
            if (*(s32 *)((u8 *)entry + 0x58) == 0) {
                return func_8009F780(entry, arg0, 0x80, 0x80, value);
            }
            entry = (u8 *)entry + 0x11C;
        } while (i < D_8015A658);
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009DC68.s")

void func_8009DD5C(s32 arg0, s32 arg1) {
    s32 i;
    s32 value;
    void *entry;

    if (arg1 != 0) {
        value = arg1;
    } else {
        value = 1;
    }
    entry = D_8015A660;
    i = 0;
    entry = D_8015A660;
    if (D_8015A658 > 0) {
        do {
            ;
            i++;
            if (((*(u16 *)((u8 *)entry + 0xBE) != 0) && (arg0 & 1)) ||
                ((*(u16 *)((u8 *)entry + 0xBE) == 0) && (arg0 & 2))) {
                *(s32 *)((u8 *)entry + 0x18) = arg1;
                *(s32 *)((u8 *)entry + 0x1C) = value;
            }
            entry = (u8 *)entry + 0x11C;
        } while (i < D_8015A658);
    }
}

s32 func_8009DDE4(s32 arg0) {
    s32 i;
    s32 matches;
    void *entry;
    volatile char flags;
    u16 temp_a0;

    i = 0;
    entry = D_8015A660;
    ;
    matches = 0;
    if (D_8015A658 > 0) {
        do {
            i++;
            if (*((s32 *) ((u8 *) entry + 0x58)) != 0) {
                temp_a0 = *((u16 *) ((u8 *) entry + 0xBE));
                if (((temp_a0 != 0) && (arg0 & 1)) || ((((unsigned long) temp_a0) == 0) && (arg0 & 2))) {
                    matches++;
                }
            }
            entry = (u8 *) entry + 0x11C;
        } while (i < D_8015A658);
    }
    return matches;
}

s32 func_8009DE50(s32 arg0, s32 arg1) {
    s32 count;
    s32 i;
    s32 value;
    void *entry;

    value = arg0 == 0;
    i = 0;
    if (value) {
        return 0;
    }
    if (arg1 != 0) {
        value = arg1;
    } else {
        value = 1;
    }
    entry = D_8015A660;
    count = 0;
    if (D_8015A658 > 0) {
        do {
            i++;
            if (arg0 == *(s32 *)((u8 *)entry + 0x14)) {
                *(s32 *)((u8 *)entry + 0x18) = arg1;
                *(s32 *)((u8 *)entry + 0x1C) = value;
                count++;
            }
            entry = (u8 *)entry + 0x11C;
        } while (i < D_8015A658);
    }
    return count;
}

s32 func_8009DEC4(s32 arg0) {
    s32 i = 0;
    s32 matches;
    void *entry;

    if (arg0 == 0) {
        return 0;
    }

    entry = D_8015A660;
    matches = 0;
    if (D_8015A658 > 0) {
        do {
            i++;
            if (arg0 == *(s32 *)((u8 *)entry + 0x14)) {
                matches++;
            }
            entry = (u8 *)entry + 0x11C;
        } while (i < D_8015A658);
    }
    return matches;
}

s32 func_8009DF14(s32 arg0, s32 arg1) {
    s32 i;
    s32 matches;
    void *entry;

    if (arg0 == 0) {
        return 0;
    }

    i = 0;
    entry = D_8015A660;
    matches = 0;
    if (D_8015A658 > 0) {
        do {
            i++;
            if (arg0 == *(s32 *)((u8 *)entry + 0x14)) {
                *(s16 *)((u8 *)entry + 0xB0) = arg1;
                matches++;
            }
            entry = (u8 *)entry + 0x11C;
        } while (i < D_8015A658);
    }
    return matches;
}

s32 func_8009DF78(s32 arg0, s32 arg1) {
    s32 i;
    s32 matches;
    void *entry;

    if (arg0 == 0) {
        return 0;
    }

    i = 0;
    entry = D_8015A660;
    matches = 0;
    if (D_8015A658 > 0) {
        do {
            i++;
            if (arg0 == *(s32 *)((u8 *)entry + 0x14)) {
                *(s16 *)((u8 *)entry + 0xB2) = arg1;
                matches++;
            }
            entry = (u8 *)entry + 0x11C;
        } while (i < D_8015A658);
    }
    return matches;
}

s32 func_8009DFDC(s32 arg0, f32 arg1) {
    s32 i = 0;
    s32 matches;
    void *entry;

    if (arg0 == 0) {
        return 0;
    }

    entry = D_8015A660;
    matches = 0;
    if (D_8015A658 > 0) {
        do {
            i++;
            if (arg0 == *(s32 *)((u8 *)entry + 0x14)) {
                *(f32 *)((u8 *)entry + 0x50) = arg1;
                matches++;
            }
            entry = (u8 *)entry + 0x11C;
        } while (i < D_8015A658);
    }
    return matches;
}

s32 func_8009E040(s32 arg0, s32 arg1) {
    s32 i = 0;
    s32 matches;
    void *entry;

    if (arg0 == 0) {
        return 0;
    }

    if (arg1 <= 0) {
        arg1 = 1;
    } else if (arg1 >= 0x101) {
        arg1 = 0x100;
    }

    entry = D_8015A660;
    matches = 0;
    if (D_8015A658 > 0) {
        do {
            i++;
            if (arg0 == *(s32 *)((u8 *)entry + 0x14)) {
                *(s16 *)((u8 *)entry + 0xB4) = arg1;
                matches++;
                *(s16 *)((u8 *)entry + 0xB8) = (s32)(*(u16 *)((u8 *)entry + 0xBA) * arg1) >> 7;
            }
            entry = (u8 *)entry + 0x11C;
        } while (i < D_8015A658);
    }
    return matches;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009E0D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009E354.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009E76C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009E938.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009EB6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009EBDC.s")

void func_8009EEE8(void *arg0) {
    u8 temp_t7;
    u8 temp_v0;

    if (1) {
        temp_t7 = (*(u8 *)((u8 *)arg0 + 0x107) & 0xFFFFU) - 1;
    }
    *(u8 *)((u8 *)arg0 + 0x107) = temp_t7;
    if ((temp_t7 & 0xFF) == 0) {
        temp_v0 = *(u8 *)((u8 *)arg0 + 0xEB);
        if (temp_v0 != 0) {
            if (*(s8 *)((u8 *)arg0 + 0x11A) == 0) {
                *(u8 *)((u8 *)arg0 + 0x107) = temp_v0;
                *(s8 *)((u8 *)arg0 + 0x11A) = *(s8 *)((u8 *)arg0 + 0x119);
                return;
            }
            *(s8 *)((u8 *)arg0 + 0x11A) = 0;
            *(u8 *)((u8 *)arg0 + 0x107) = *(u8 *)((u8 *)arg0 + 0x106);
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009EF44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009EFF4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009F0C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009F194.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009F344.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009F4C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009F604.s")

s32 func_8009F6F4(s32 arg0, s32 arg1) {
    s32 i;
    void *entry;

    entry = D_8015A660;
    i = 0;
    if (D_8015A658 > 0) {
        do {
            if (*(s32 *)((u8 *)entry + 0x58) == 0) {
                return i;
            }
            i++;
            entry = (u8 *)entry + 0x11C;
        } while (i < D_8015A658);
    }
    return -1;
}

void func_8009F748(u8 *p, unsigned char c, s32 n) {
    u8 *q = p;

    while (n--) {
        *(q++) = c;
    }
}

s32 func_8009F780(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 temp_v1;

    func_8009F604();
    *(s16 *)((u8 *)arg0 + 0xBE) = arg1;
    *(s16 *)((u8 *)arg0 + 0xB0) = arg2;
    *(s16 *)((u8 *)arg0 + 0xB2) = arg3;
    *(s32 *)((u8 *)arg0 + 0x14) = D_8015A680;
    D_8015A680 += 1;
    *(s32 *)((u8 *)arg0 + 0x20) = arg4;
    temp_v1 = libmus_fxheader_current[arg1];
    *(s32 *)((u8 *)arg0 + 0x5C) = temp_v1;
    *(s32 *)((u8 *)arg0 + 0x58) = temp_v1;
    return *(s32 *)((u8 *)arg0 + 0x14);
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009F810.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009FC0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009FD74.s")

void func_8009FF40(s32 arg0) {
    s32 temp;

    if ((osAiGetLength() >> 2) == 0) {
        temp = 0;
        if (D_800DF2A4 != temp) {
            return;
        }
        D_800DF2A4 = temp;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_8009FF80.s")

void (*func_800A0138(Struct800A0138 **arg0))(void) {
    if (D_8015C928.unk0 == 0) {
        D_8015C928.unk4 = 0;
        D_8015C928.unk8 = D_8015C964;
        D_8015C928.unk0 = 1;
    }
    *arg0 = &D_8015C928;
    return func_8009FF80;
}

#pragma GLOBAL_ASM("asm/nonmatchings/9CE70/func_800A0170.s")
