#include "common.h"

typedef void *OSMesg;

typedef struct OSMesgQueue {
    void *mtqueue;
    void *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;

typedef struct SchedulerClient {
    struct SchedulerClient *next;
    OSMesgQueue *queue;
} SchedulerClient;

typedef struct PlayerCommandData {
    u8 pad0[0x10];
    u8 *commands;
    s32 *jumpTable;
} PlayerCommandData;

typedef struct PlayerCommandState {
    u8 pad0[0x14];
    s32 id;
    s32 fadeTarget;
    s32 fadeTime;
    s32 unk20;
    u8 pad24[0x8];
    f32 unk2C;
    u8 pad30[0x8];
    f32 unk38;
    u8 pad3C[0x8];
    f32 unk44;
    f32 unk48;
    u8 pad4C[0x4];
    f32 unk50;
    PlayerCommandData *data;
    s32 sequencePos;
    s32 restartPos;
    s32 unk60;
    s32 unk64;
    s32 unk68;
    s32 unk6C;
    s32 jumpTarget;
    u8 *returnScripts[5];
    s32 returnUnk60[5];
    s32 returnUnk68[5];
    s16 unkB0;
    s16 unkB2;
    s16 unkB4;
    u8 padB6[0x2];
    s16 unkB8;
    u16 unkBA;
    u8 padBC[0x2];
    u16 soundId;
    s16 unkC0;
    u16 unkC2;
    u16 unkC4;
    u8 padC6[0x2];
    u16 unkC8;
    u16 unkCA;
    u16 unkCC;
    u16 returnUnkC8[5];
    u16 returnUnkCA[5];
    u8 padE2[0x3];
    u8 flagE5;
    u8 flagE6;
    u8 flagE7;
    u8 flagE8;
    u8 unkE9;
    u8 unkEA;
    u8 unkEB;
    u8 returnDepth;
    u8 unkED;
    u8 unkEE;
    s8 unkEF;
    u8 unkF0;
    u8 unkF1;
    s8 unkF2;
    u8 unkF3;
    u8 padF4[0x9];
    s8 unkFD;
    u8 padFE[0x7];
    u8 unk105;
    u8 unk106;
    u8 unk107;
    u8 pad108;
    u8 returnUnk109[5];
    u8 returnUnk10E[5];
    u8 returnUnk113[5];
    s8 unk118;
    s8 unk119;
    s8 unk11A;
    u8 pad11B;
} PlayerCommandState;

typedef struct SchedulerTask {
    struct SchedulerTask *next;
    u32 state;
    u32 flags;
    void *framebuffer;
    u8 list[0x40];
} SchedulerTask;

typedef struct SchedulerState {
    s32 unk0;
    OSMesgQueue messageQueue;
    u8 pad1C[0x1FC - 0x1C];
    OSMesgQueue framebufferQueue;
    OSMesg framebufferMsgs[16];
    u8 pad254[0x768 - 0x254];
    SchedulerClient *clients;
    SchedulerTask *curRSPTask;
    SchedulerTask *curRDPTask;
    s32 doAudio;
} SchedulerState;

typedef struct Struct800A0138 {
    u8 unk0;
    u8 pad1[3];
    s32 unk4;
    s32 unk8;
} Struct800A0138;

extern s32 osSendMesg(OSMesgQueue *, OSMesg, s32);
extern s32 osSetIntMask(s32);
extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
extern s32 osSpTaskYielded(void *);
extern void *osViGetCurrentFramebuffer(void);
extern void *osViGetNextFramebuffer(void);
extern void osWritebackDCacheAll(void);
extern void osSpTaskLoad(void *);
extern void osSpTaskStartGo(void *);
extern void osSpTaskYield(void);
extern u32 osAiGetLength(void);
extern Struct800A0138 D_8015C928;
extern s32 D_8015C964;
extern void func_8009CD18(PlayerCommandState *, u8 *);
extern void func_8009C77C(SchedulerState *);
extern void func_8009F604(void);
extern s32 func_8009F4C8();
extern s32 func_8009F780(PlayerCommandState *, s32, s32, s32, s32);
extern void func_8009FF80(void);
extern s32 D_800DF154;
extern s32 D_800DF158;
extern s32 D_800DF2A4;
extern s32 D_8015A680;
extern s32 D_8015A684;
extern s32 D_8015A620;
extern u8 D_8015A624;
extern s32 *libmus_fxheader_current;
extern f64 D_800E1A80;
extern f64 D_800E1A88;
extern f64 D_800E1A90;
extern f64 D_800E1A98;
extern f64 D_800E1AA0;
extern f64 D_800E1AA8;
extern f64 D_800E1AB0;
extern f64 D_800E1AB8;
extern f64 D_800E1AC0;
extern f64 D_800E1AC8;
extern f64 D_800E1AD0;
extern f64 D_800E1AD8;

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009C270.s")

s32 func_8009C434(s32 arg0) {
    return arg0 + 4;
}

s32 func_8009C43C(s32 arg0) {
    return arg0 + 0x5C;
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009C444.s")

void func_8009C6DC(SchedulerState *arg0) {
    if (D_8015A624 == 0) {
        if (osRecvMesg(&arg0->messageQueue, (OSMesg *)&arg0->curRDPTask, 0) != -1) {
            osWritebackDCacheAll();
            D_8015A624 = 1;
            D_800DF154 = 0;
            *(volatile unsigned int *)&D_800DF158 = 0;
            if (D_8015A620 & 1) {
                *(volatile unsigned int *)&D_800DF158 = 1;
                osSpTaskYield();
            } else {
                func_8009C77C(arg0);
            }
        }
    }
}

void func_8009C77C(SchedulerState *arg0) {
    if (D_800DF158 != 0) {
        D_800DF158 = 0;
        if (osSpTaskYielded(&arg0->curRSPTask->list) != 0) {
            D_800DF154 = 1;
        } else {
            D_800DF154 = 2;
        }
    }
    D_8015A620 |= 2;
    osWritebackDCacheAll();
    osSpTaskLoad(&arg0->curRDPTask->list);
    osSpTaskStartGo(&arg0->curRDPTask->list);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009C81C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009C8DC.s")

void func_8009CA60(SchedulerState *arg0, SchedulerClient *arg1, OSMesgQueue *arg2) {
    s32 prev = osSetIntMask(1);
    arg1->queue = arg2;
    arg1->next = arg0->clients;
    arg0->clients = arg1;
    osSetIntMask(prev);
}

void func_8009CAB4(SchedulerState *arg0, SchedulerClient *arg1) {
    SchedulerClient *node;
    SchedulerClient *prev;
    s32 mask;

    node = arg0->clients;
    prev = NULL;
    mask = osSetIntMask(1);
    if (node != NULL) {
        do {
            if (node == arg1) {
                if (prev != NULL) {
                    prev->next = arg1->next;
                } else {
                    arg0->clients = arg1->next;
                }
                break;
            }
            prev = node;
            node = node->next;
        } while (node != NULL);
    }
    osSetIntMask(mask);
}

void func_8009CB44(SchedulerState *arg0, s32 arg1) {
    SchedulerClient *node = arg0->clients;
    while (node != NULL) {
        osSendMesg(node->queue, (OSMesg)arg1, 0);
        node = node->next;
    }
}

void func_8009CB98(SchedulerState *arg0, SchedulerTask *arg1) {
    OSMesg msg;
    SchedulerClient node;
    void *framebuffer;
    OSMesgQueue *queue;

    msg = 0;
    framebuffer = arg1->framebuffer;
    while ((osViGetCurrentFramebuffer() == framebuffer) || (osViGetNextFramebuffer() == framebuffer)) {
        queue = &arg0->framebufferQueue;
        func_8009CA60(arg0, &node, queue);
        osRecvMesg(queue, &msg, 1);
        func_8009CAB4(arg0, &node);
    }
}

void func_8009CC50(SchedulerState *arg0) {
    OSMesg msg;
    SchedulerClient node;

    msg = 0;
    func_8009CA60(arg0, &node, &arg0->framebufferQueue);
    osRecvMesg(&arg0->framebufferQueue, &msg, 1);
    func_8009CAB4(arg0, &node);
}

s32 func_8009CCA0(PlayerCommandState *arg0, s32 arg1) {
    arg0->unk60 = 0;
    arg0->unk68 = 0;
    arg0->data = 0;
    arg0->soundId = 0;
    arg0->id = 0;
    return 0;
}

void *func_8009CCC0(PlayerCommandState *arg0, u8 *arg1) {
    u16 v;
    u8 *new_var;

    v = *arg1;
    arg1++;
    if (v & 0x80) {
        v = ((v & 0x7F) << 1) << 7;
        v |= *(new_var = arg1);
        arg1++;
    }
    arg0->unkCC = v;
    return arg1;
}

s32 func_8009CCFC(PlayerCommandState *arg0, u8 *arg1) {
    arg0->unkEA = *arg1;
    return (s32)(arg1 + 1);
}

s32 func_8009CD0C(PlayerCommandState *arg0, s32 arg1) {
    arg0->unkEA = 0;
    return arg1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009CD18.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009CE3C.s")

s32 func_8009CF1C(PlayerCommandState *arg0, u8 *arg1) {
    u8 *ret;
    u8 *temp;

    arg0->unkC4 = *arg1;
    ;
    ret = (temp = arg1) + 1;
    arg0->unkC2 = 0;
    arg1++;
    arg1--;
    return (s32)ret;
}

s32 func_8009CF30(PlayerCommandState *arg0, u8 *arg1) {
    u8 *ret;
    u8 *temp;
    u32 v;
    short new_var;

    new_var = arg1[0] << 8;
    new_var = arg1[1] | ((0, new_var));
    v = new_var;
    ret = (temp = arg1) + 2;
    arg0->unkC2 = v;
    arg0->unkC4 = 0;
    arg1 += 2;
    arg1 -= 2;
    return (s32)ret;
}

s32 func_8009CF50(PlayerCommandState *arg0, u8 *arg1) {
    u32 temp_t8;
    f32 var_ft1;

    arg0->unk105 = arg1[0];
    arg0->unkE9 = arg1[1];
    arg1 += 2;
    temp_t8 = arg1[0];
    var_ft1 = (f32)(temp_t8 & 0xFFFF);
    arg0->unk44 = (f32)(((double)var_ft1) / 50.0);
    return (s32)(arg1 + 1);
}

s32 func_8009CFAC(PlayerCommandState *arg0, u8 *arg1) {
    u32 temp_t8;
    f32 var_ft1;

    arg0->unk105 = arg1[0];
    arg0->unkE9 = arg1[1];
    arg1 += 2;
    temp_t8 = arg1[0];
    var_ft1 = (f32)(temp_t8 & 0xFFFF);
    arg0->unk44 = (f32)(((double)-var_ft1) / 50.0);
    return (s32)(arg1 + 1);
}

s32 func_8009D00C(PlayerCommandState *arg0, s32 arg1) {
    arg0->unkE9 = 0;
    arg0->unk48 = 0.0f;
    return arg1;
}

s32 func_8009D020(PlayerCommandState *arg0, u8 *arg1) {
    u8 b;
    unsigned int new_var;
    s16 value;

    b = arg1[0];
    arg1++;
    if (b < 0x80) {
        arg0->unkC0 = b;
    } else {
        value = ((b & 0x7F) << 6) << 2;
        arg0->unkC0 = value;
        new_var = arg1[0];
        b = new_var;
        arg0->unkC0 = value + b;
        arg1++;
    }
    return (s32)arg1;
}

s32 func_8009D064(PlayerCommandState *arg0, s32 arg1) {
    arg0->flagE6 = 1;
    return arg1;
}

s32 func_8009D074(PlayerCommandState *arg0, u8 *arg1) {
    arg0->unk118 = *arg1;
    return (s32)(arg1 + 1);
}

s32 func_8009D084(PlayerCommandState *arg0, s32 arg1) {
    arg0->flagE7 = 1;
    return arg1;
}

s32 func_8009D094(PlayerCommandState *arg0, u8 *arg1) {
    s32 b;
    f32 f;
    b = *arg1;
    arg1++;
    if (b & 0x80) {
        b |= 0xFFFFFF00;
    }
    f = (f32)b;
    arg0->unk2C = f / 100.0;
    return (s32)arg1;
}

s32 func_8009D0E0(PlayerCommandState *arg0, u8 *arg1) {
    s32 var_v0;

    var_v0 = arg1[0];
    arg1 += 1;
    if (var_v0 & 0x80) {
        var_v0 = arg1[0] | ((0, (var_v0 & 0x7F) << 8));
        arg1 += 1;
    }
    func_8009CD18(arg0, arg0->data->commands + (var_v0 * 7));
    return (s32)arg1;
}

s32 func_8009D138(PlayerCommandState *arg0, s32 arg1) {
    arg0->flagE8 = 1;
    return arg1;
}

s32 func_8009D148(PlayerCommandState *arg0, s32 arg1) {
    arg0->flagE8 = 0;
    return arg1;
}

s32 func_8009D154(PlayerCommandState *arg0, s32 arg1) {
    arg0->flagE5 = 1;
    return arg1;
}

s32 func_8009D164(PlayerCommandState *arg0, s32 arg1) {
    arg0->flagE5 = 0;
    return arg1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009D170.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009D1EC.s")

s32 func_8009D27C(PlayerCommandState *arg0, u8 *arg1) {
    arg0->unk119 = arg1[0];
    arg0->unkEB = arg1[1];
    arg0->unk106 = arg1[2];
    return (s32)(arg1 + 3);
}

s32 func_8009D2A0(PlayerCommandState *arg0, s32 arg1) {
    arg0->unkEB = 0;
    return arg1;
}

s32 func_8009D2AC(PlayerCommandState *arg0, s32 arg1) {
    arg0->unkED = 1;
    return arg1;
}

s32 func_8009D2BC(PlayerCommandState *arg0, s32 arg1) {
    arg0->unkED = 0;
    return arg1;
}

s32 func_8009D2C8(PlayerCommandState *arg0, u8 *arg1) {
    s32 ret;

    ret = arg1 + 1;
    arg0->unkEE = arg1[0];
    if (ret) {
    }
    arg0->unkED = 0;
    return ret;
}

s32 func_8009D2DC(PlayerCommandState *arg0, u8 *arg1) {
    arg0->unkF2 = (s32)(u8)*arg1 / 2;
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

s32 func_8009D330(PlayerCommandState *arg0, s32 arg1) {
    arg0->jumpTarget = 0;
    return arg1;
}

s32 func_8009D33C(s32 arg0, s32 arg1) {
    return arg1;
}

// func_8009D348 best match: 74.0%
#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009D348.s")

#ifdef NON_MATCHING
s32 func_8009D348(PlayerCommandState *arg0, u8 *arg1) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_t7;

    temp_v0 = (arg1[2] << 8) + arg1[3];
    temp_v1 = arg1[0];
    temp_t7 = arg1[1];
    arg0->unk60 = arg0->unk64 + temp_v0;
    arg0->unkC8 = 1;
    temp_v0 = (arg1[4] << 8) + arg1[5];
    temp_v1 = (temp_v1 << 8) + temp_t7;
    arg0->unk68 = arg0->unk6C + temp_v0;
    arg1 += 5;
    arg0->unkCA = 1;
    return arg0->restartPos + temp_v1;
}
#endif

s32 func_8009D3AC(PlayerCommandState *arg0, u8 *arg1) {
    arg0->unkF3 = *arg1;
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

s32 func_8009D40C(PlayerCommandState *arg0, u8 *arg1) {
    u8 temp_a0;
    s8 temp_v0;

    temp_a0 = *arg1;
    arg1++;
    temp_v0 = func_8009F4C8(temp_a0, arg1, arg0);
    arg0->unkEF = temp_v0;
    arg0->unkEF = *arg1 + (temp_a0 = temp_v0);
    return (s32)(arg1 + 1);
}

s32 func_8009D45C(PlayerCommandState *arg0, u8 *arg1) {
    u8 temp_a0;
    s8 temp_v0;

    temp_a0 = *arg1;
    arg1++;
    temp_v0 = func_8009F4C8(temp_a0, arg1, arg0);
    arg0->unkF2 = temp_v0;
    arg0->unkF2 = *arg1 + (temp_a0 = temp_v0);
    return (s32)(arg1 + 1);
}

s32 func_8009D4AC(PlayerCommandState *arg0, u8 *arg1) {
    arg0->unkEF = *arg1;
    return (s32)(arg1 + 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009D4BC.s")

s32 func_8009D598(PlayerCommandState *arg0, u8 *arg1) {
    arg0->unkF1 = *arg1;
    return (s32)(arg1 + 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009D5A8.s")

extern s16 D_8015A67C;
extern s16 D_8015A67E;
extern s32 D_8015A658;
extern PlayerCommandState *D_8015A660;
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

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009D8D8.s")

s32 func_8009DBE4(s32 arg0) {
    s32 i;
    PlayerCommandState *entry;
    s32 value;

    value = D_8015A670[arg0];
    entry = D_8015A660;
    i = 0;
    if (D_8015A658 > 0) {
        do {
            i++;
            if (entry->sequencePos == 0) {
                return func_8009F780(entry, arg0, 0x80, 0x80, value);
            }
            entry++;
        } while (i < D_8015A658);
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009DC68.s")

void func_8009DD5C(s32 arg0, s32 arg1) {
    s32 i;
    s32 value;
    PlayerCommandState *entry;

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
            if (((entry->soundId != 0) && (arg0 & 1)) ||
                ((entry->soundId == 0) && (arg0 & 2))) {
                entry->fadeTarget = arg1;
                entry->fadeTime = value;
            }
            entry++;
        } while (i < D_8015A658);
    }
}

s32 func_8009DDE4(s32 arg0) {
    s32 i;
    s32 matches;
    PlayerCommandState *entry;
    volatile char flags;
    u16 temp_a0;

    i = 0;
    entry = D_8015A660;
    ;
    matches = 0;
    if (D_8015A658 > 0) {
        do {
            i++;
            if (entry->sequencePos != 0) {
                temp_a0 = entry->soundId;
                if (((temp_a0 != 0) && (arg0 & 1)) || ((((unsigned long) temp_a0) == 0) && (arg0 & 2))) {
                    matches++;
                }
            }
            entry++;
        } while (i < D_8015A658);
    }
    return matches;
}

s32 func_8009DE50(s32 arg0, s32 arg1) {
    s32 count;
    s32 i;
    s32 value;
    PlayerCommandState *entry;

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
            if (arg0 == entry->id) {
                entry->fadeTarget = arg1;
                entry->fadeTime = value;
                count++;
            }
            entry++;
        } while (i < D_8015A658);
    }
    return count;
}

s32 func_8009DEC4(s32 arg0) {
    s32 i = 0;
    s32 matches;
    PlayerCommandState *entry;

    if (arg0 == 0) {
        return 0;
    }

    entry = D_8015A660;
    matches = 0;
    if (D_8015A658 > 0) {
        do {
            i++;
            if (arg0 == entry->id) {
                matches++;
            }
            entry++;
        } while (i < D_8015A658);
    }
    return matches;
}

s32 func_8009DF14(s32 arg0, s32 arg1) {
    s32 i;
    s32 matches;
    PlayerCommandState *entry;

    if (arg0 == 0) {
        return 0;
    }

    i = 0;
    entry = D_8015A660;
    matches = 0;
    if (D_8015A658 > 0) {
        do {
            i++;
            if (arg0 == entry->id) {
                entry->unkB0 = arg1;
                matches++;
            }
            entry++;
        } while (i < D_8015A658);
    }
    return matches;
}

s32 func_8009DF78(s32 arg0, s32 arg1) {
    s32 i;
    s32 matches;
    PlayerCommandState *entry;

    if (arg0 == 0) {
        return 0;
    }

    i = 0;
    entry = D_8015A660;
    matches = 0;
    if (D_8015A658 > 0) {
        do {
            i++;
            if (arg0 == entry->id) {
                entry->unkB2 = arg1;
                matches++;
            }
            entry++;
        } while (i < D_8015A658);
    }
    return matches;
}

s32 func_8009DFDC(s32 arg0, f32 arg1) {
    s32 i = 0;
    s32 matches;
    PlayerCommandState *entry;

    if (arg0 == 0) {
        return 0;
    }

    entry = D_8015A660;
    matches = 0;
    if (D_8015A658 > 0) {
        do {
            i++;
            if (arg0 == entry->id) {
                entry->unk50 = arg1;
                matches++;
            }
            entry++;
        } while (i < D_8015A658);
    }
    return matches;
}

s32 func_8009E040(s32 arg0, s32 arg1) {
    s32 i = 0;
    s32 matches;
    PlayerCommandState *entry;

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
            if (arg0 == entry->id) {
                entry->unkB4 = arg1;
                matches++;
                entry->unkB8 = (s32)(entry->unkBA * arg1) >> 7;
            }
            entry++;
        } while (i < D_8015A658);
    }
    return matches;
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009E0D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009E354.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009E76C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009E938.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009EB6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009EBDC.s")

void func_8009EEE8(PlayerCommandState *arg0) {
    u8 temp_t7;
    u8 temp_v0;

    if (1) {
        temp_t7 = (arg0->unk107 & 0xFFFFU) - 1;
    }
    arg0->unk107 = temp_t7;
    if ((temp_t7 & 0xFF) == 0) {
        temp_v0 = arg0->unkEB;
        if (temp_v0 != 0) {
            if (arg0->unk11A == 0) {
                arg0->unk107 = temp_v0;
                arg0->unk11A = arg0->unk119;
                return;
            }
            arg0->unk11A = 0;
            arg0->unk107 = arg0->unk106;
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009EF44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009EFF4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009F0C4.s")

f32 func_8009F194(f32 arg0) {
    f32 square;
    f32 fourth;

    if (arg0 == 0.0f) {
        return 1.0f;
    }

    if (arg0 > 0.0f) {
        square = arg0 * arg0;
        fourth = square * square;
        return (f32)(((f64)arg0 * D_800E1A80) + 1.0 +
            ((f64)square * D_800E1A88) +
            ((f64)(square * arg0) * D_800E1A90) +
            ((f64)fourth * D_800E1A98) +
            ((f64)(fourth * arg0) * D_800E1AA0) +
            ((f64)(fourth * square) * D_800E1AA8));
    }

    arg0 = -arg0;
    square = arg0 * arg0;
    fourth = square * square;
    return (f32)(1.0 / (((f64)arg0 * D_800E1AB0) + 1.0 +
        ((f64)square * D_800E1AB8) +
        ((f64)(square * arg0) * D_800E1AC0) +
        ((f64)fourth * D_800E1AC8) +
        ((f64)(fourth * arg0) * D_800E1AD0) +
        ((f64)(fourth * square) * D_800E1AD8)));
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009F344.s")

s32 func_8009F4C8(s32 arg0) {
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 var_v0;
    f32 temp_f0;
    f32 temp_f10;

    for (var_v0 = 0; var_v0 != 8;) {
        var_v0 += 4;
        temp_v1 = D_8015A684 & 0x48000000;
        D_8015A684 *= 2;
        if ((temp_v1 == 0x48000000) || (temp_v1 == 0x08000000)) {
            D_8015A684 |= 1;
        }
        temp_v1_2 = D_8015A684 & 0x48000000;
        D_8015A684 *= 2;
        if ((temp_v1_2 == 0x48000000) || (temp_v1_2 == 0x08000000)) {
            D_8015A684 |= 1;
        }
        temp_v1_3 = D_8015A684 & 0x48000000;
        D_8015A684 *= 2;
        if ((temp_v1_3 == 0x48000000) || (temp_v1_3 == 0x08000000)) {
            D_8015A684 |= 1;
        }
        temp_v1_4 = D_8015A684 & 0x48000000;
        D_8015A684 *= 2;
        if ((temp_v1_4 == 0x48000000) || (temp_v1_4 == 0x08000000)) {
            D_8015A684 |= 1;
        }
    }

    temp_f0 = (f32)D_8015A684 / 65536.0f;
    temp_f10 = (f32)arg0;
    temp_f0 = temp_f0 / 65536.0f;
    return (s32)(temp_f10 * temp_f0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009F604.s")

s32 func_8009F6F4(s32 arg0, s32 arg1) {
    s32 i;
    PlayerCommandState *entry;

    entry = D_8015A660;
    i = 0;
    if (D_8015A658 > 0) {
        do {
            if (entry->sequencePos == 0) {
                return i;
            }
            i++;
            entry++;
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

s32 func_8009F780(PlayerCommandState *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 temp_v1;

    func_8009F604();
    arg0->soundId = arg1;
    arg0->unkB0 = arg2;
    arg0->unkB2 = arg3;
    arg0->id = D_8015A680;
    D_8015A680 += 1;
    arg0->unk20 = arg4;
    temp_v1 = libmus_fxheader_current[arg1];
    arg0->restartPos = temp_v1;
    arg0->sequencePos = temp_v1;
    return arg0->id;
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009F810.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009FC0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009FD74.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009FF80.s")

void (*func_800A0138(Struct800A0138 **arg0))(void) {
    if (D_8015C928.unk0 == 0) {
        D_8015C928.unk4 = 0;
        D_8015C928.unk8 = D_8015C964;
        D_8015C928.unk0 = 1;
    }
    *arg0 = &D_8015C928;
    return func_8009FF80;
}

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_800A0170.s")
