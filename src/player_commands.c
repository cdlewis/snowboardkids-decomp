#include "common.h"
#include <PR/libaudio.h>
#include <PR/os_ai.h>
#include <PR/os_convert.h>
#include <PR/ucode.h>

typedef void *OSMesg;

typedef struct OSMesgQueue {
    void *mtqueue;
    void *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;

typedef struct SchedulerThread {
    u8 pad[0x1B0];
} SchedulerThread;

typedef struct SchedulerViMode {
    u8 pad[0x50];
} SchedulerViMode;

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
    u16 unkC6;
    u16 unkC8;
    u16 unkCA;
    u16 unkCC;
    u16 returnUnkC8[5];
    u16 returnUnkCA[5];
    u8 padE2[0x2];
    u8 unkE4;
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
    u8 unkEF;
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
    OSMesgQueue *queue;
    OSMesg msg;
} SchedulerTask;

typedef struct SchedulerState {
    s16 unk0;
    s16 unk2;
    OSMesgQueue messageQueue;
    OSMesg messageMsgs[16];
    OSMesgQueue eventQueue;
    OSMesg eventMsgs[16];
    OSMesgQueue retraceQueue;
    OSMesg retraceMsgs[32];
    OSMesgQueue queue14C;
    OSMesg msgs164[16];
    OSMesgQueue queue1A4;
    OSMesg msgs1BC[16];
    OSMesgQueue framebufferQueue;
    OSMesg framebufferMsgs[16];
    u8 pad254[4];
    SchedulerThread thread258;
    SchedulerThread thread408;
    u8 pad5B8[0x768 - 0x5B8];
    SchedulerClient *clients;
    SchedulerTask *curRSPTask;
    SchedulerTask *curRDPTask;
    s32 doAudio;
} SchedulerState;

typedef struct Struct800A0138 {
    u8 initialized;
    u8 pad1[3];
    ALLink *activeList;
    ALLink *readyList;
} Struct800A0138;

typedef struct Struct800A0170Node {
    ALLink node;
    s32 addr;
    u32 counter;
    void *buffer;
} Struct800A0170Node;

typedef struct OSIoMesg {
    u8 pad[0x18];
} OSIoMesg;

typedef struct PlayerCommandInit {
    s32 count;
    void *unk4;
    s32 outputRate;
    u8 *heapBase;
    s32 heapLen;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 *fxHeader;
    s32 *unk28;
    s32 maxUpdates;
    s32 maxFXBusses;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
} PlayerCommandInit;

typedef struct PlayerCommandSynConfig {
    s32 maxVVoices;
    s32 maxPVoices;
    s32 maxUpdates;
    s32 padC;
    s32 dmaproc;
    ALHeap *heap;
    s32 outputRate;
    u8 fxType;
} PlayerCommandSynConfig;

typedef struct AudioInfo {
    void *buf;
    s16 len;
} AudioInfo;

typedef struct AudioTask {
    void *outBuf;
    s16 outLen;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u32 type;
    u32 flags;
    void *ucodeBoot;
    u32 ucodeBootSize;
    void *ucode;
    u32 ucodeSize;
    void *ucodeData;
    u32 ucodeDataSize;
    void *dramStack;
    u32 dramStackSize;
    void *outputBuff;
    void *outputBuffSize;
    void *dataPtr;
    u32 dataSize;
    void *yieldDataPtr;
    u32 yieldDataSize;
    OSMesgQueue *msgQ;
    OSMesg msg;
    u8 pad60[8];
    u8 unk68;
} AudioTask;

extern s32 osSendMesg(OSMesgQueue *, OSMesg, s32);
extern s32 osSetIntMask(s32);
extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
extern void osCreateThread(SchedulerThread *, s32, void (*)(void *), void *, void *, s32);
extern void osCreateViManager(s32);
extern void osSetEventMesg(s32, OSMesgQueue *, OSMesg);
extern void osStartThread(SchedulerThread *);
extern s32 osSpTaskYielded(void *);
extern void osViBlack(u8);
extern void osViSetEvent(OSMesgQueue *, OSMesg, u32);
extern void osViSetMode(SchedulerViMode *);
extern void *osViGetCurrentFramebuffer(void);
extern void *osViGetNextFramebuffer(void);
extern void osWritebackDCacheAll(void);
extern void osSpTaskLoad(void *);
extern void osSpTaskStartGo(void *);
extern void osSpTaskYield(void);
extern u32 osAiGetLength(void);
extern u32 osVirtualToPhysical(void *);
extern s32 osPiStartDma(OSIoMesg *, s32, s32, u32, void *, u32, OSMesgQueue *);
extern Struct800A0138 D_8015C928;
extern ALLink *D_8015C964;
extern void func_8009CD18(PlayerCommandState *, u8 *);
extern void func_8009C77C(SchedulerState *);
extern void func_8009F604();
extern s32 func_8009F4C8();
extern s32 func_8009F780(PlayerCommandState *, s32, s32, s32, s32);
extern void func_8009F344(s32, s32);
extern void func_8009F810(void *, s32 *, s32, s32 *, s32, s32, s32);
extern ALMicroTime func_8009E0D4(void *);
extern s32 func_8009FF80(s32, s32, void *);
extern void func_8009C444(void *);
extern void func_8009C8DC(void *);
extern f32 sinf(f32);
extern s32 D_800DF154;
extern s32 D_800DF158;
extern s32 D_800DF298;
extern u32 D_800DF290;
extern u32 D_800DF294;
extern s32 D_800DF2A4;
extern SchedulerViMode D_800DF340[];
extern u64 D_800E1F00[];
extern u8 D_80158620[];
extern Acmd *D_8015A6A0[];
extern u64 D_8015A8A0[];
extern s32 D_8015A680;
extern s32 D_8015A684;
extern s32 D_8015A620;
extern u8 D_8015A624;
extern s32 D_8015C934;
extern s32 D_8015C938;
extern OSMesgQueue D_8015C948;
extern s32 D_8015C960;
extern OSIoMesg *D_8015C968;
extern s32 D_8015C970;
extern s32 *libmus_fxheader_current;
extern f64 D_800E1A78;
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
extern ALPlayer D_8015A630;
extern ALHeap D_8015A648;
extern u8 *D_8015A64C;
extern u8 *D_8015A65C;
extern s32 D_8015A658;
extern PlayerCommandState *D_8015A660;
extern s32 D_8015A664;
extern s32 D_8015A668;
extern s32 *D_8015A670;
extern s16 D_8015A67C;
extern s16 D_8015A67E;
extern s32 D_8015A678;
extern s32 D_8015A68C;
extern ALSynth D_8015A8D8;
extern s32 osTvType;

void func_8009C270(SchedulerState *arg0, u8 arg1, u8 arg2) {
    arg0->curRSPTask = 0;
    arg0->curRDPTask = 0;
    arg0->clients = 0;
    arg0->doAudio = 1;
    arg0->doAudio = arg0->doAudio & 0xFFFFFFFFFFFFFFFF;
    arg0->unk0 = 1;
    arg0->unk2 = 3;
    D_800DF154 = 0;
    D_800DF158 = 0;
    D_8015A620 = 0;
    D_8015A624 = 0;
    osCreateMesgQueue(&arg0->retraceQueue, arg0->retraceMsgs, 0x20);
    osCreateMesgQueue(&arg0->queue1A4, arg0->msgs1BC, 0x10);
    osCreateMesgQueue(&arg0->eventQueue, arg0->eventMsgs, 0x10);
    osCreateMesgQueue(&arg0->messageQueue, arg0->messageMsgs, 0x10);
    osCreateMesgQueue(&arg0->framebufferQueue, arg0->framebufferMsgs, 0x10);
    osCreateMesgQueue(&arg0->queue14C, arg0->msgs164, 0x10);
    osCreateViManager(0xFE);
    osViSetMode(&D_800DF340[arg1]);
    osViBlack(1);
    osViSetEvent(&arg0->retraceQueue, (OSMesg)0x29A, arg2);
    osSetEventMesg(4, &arg0->retraceQueue, (OSMesg)0x29B);
    osSetEventMesg(0xE, &arg0->retraceQueue, (OSMesg)0x29D);
    osSetEventMesg(9, &arg0->queue1A4, (OSMesg)0x29C);
    osCreateThread(&arg0->thread258, 6, func_8009C444, arg0, D_80158620, 0x78);
    osStartThread(&arg0->thread258);
    osCreateThread(&arg0->thread408, 5, func_8009C8DC, arg0, &D_8015A620, 0x64);
    osStartThread(&arg0->thread408);
}

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

void func_8009C81C(SchedulerState *arg0) {
    OSMesg msg;

    msg = 0;
    osWritebackDCacheAll();
    if (D_800DF154 == 1) {
        osSpTaskLoad(arg0->curRSPTask->list);
        osSpTaskStartGo(arg0->curRSPTask->list);
    } else if (D_800DF154 == 2) {
        D_8015A620 &= ~1;
        osSendMesg(&arg0->queue14C, &msg, 1);
    }
    osSendMesg(arg0->curRDPTask->queue, arg0->curRDPTask->msg, 1);
    D_8015A624 = 0;
}

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

// func_8009CE3C best match: 98.929% (nonmatchings/func_8009CE3C-2911448260736516995/base_21.c)
#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009CE3C.s")

#ifdef NON_MATCHING
s32 func_8009CE3C(PlayerCommandState *arg0, u8 *arg1) {
    s32 i;
    PlayerCommandState *entry;
    s32 value;
    s32 scaled;

    value = (*arg1 * 0x6000) / 120 / D_8015A678;
    scaled = 7;
    scaled = (arg0->unkB4 * value) >> scaled;
    arg1++;
    if (arg0->soundId != 0) {
        arg0->unkB8 = value;
        do {
        } while (0);
        return (s32)arg1;
    }

    i = 0;
    entry = D_8015A660;
    if (D_8015A658 > 0) {
        do {
            i++;
            if (entry->data == arg0->data) {
                entry->unkBA = value;
                entry->unkB8 = scaled;
            }
            entry++;
        } while (i < D_8015A658);
    }
    return (s32)arg1;
}
#endif

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

s32 func_8009D170(PlayerCommandState *arg0, u8 *arg1) {
    u8 depth;

    depth = arg0->returnDepth;
    arg0->returnUnk109[depth] = *arg1++;
    arg0->returnScripts[depth] = arg1;
    arg0->returnUnk60[depth] = arg0->unk60;
    arg0->returnUnk68[depth] = arg0->unk68;
    arg0->returnUnk10E[depth] = arg0->unkEF;
    arg0->returnUnk113[depth] = arg0->unkF0;
    arg0->returnUnkC8[depth] = arg0->unkC8;
    arg0->returnUnkCA[depth] = arg0->unkCA;
    arg0->returnDepth++;
    return (s32)arg1;
}

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

// func_8009D348 best match: 97.0%
#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009D348.s")

#ifdef NON_MATCHING
s32 func_8009D348(PlayerCommandState *arg0, u8 *arg1) {
    register s32 temp_v0;
    register s32 temp_v1;
    s32 temp_t9;
    u8 *new_var;
    s32 temp_t7;

    temp_v0 = arg1[2];
    temp_v1 = arg1[0];
    temp_t9 = (arg1[3] & 0xFF) & 0xFF;
    temp_v0 = temp_v0 << 8;
    arg1 += 5;
    new_var = arg1 - 5;
    temp_t7 = new_var[1];
    temp_v0 = temp_v0 + temp_t9;
    arg0->unk60 = arg0->unk64 + temp_v0;
    arg0->unkC8 = 1;
    temp_v0 = new_var[4];
    temp_t9 = new_var[5];
    temp_v0 = (temp_v0 << 8) + temp_t9;
    temp_v1 = (temp_v1 << 8) + temp_t7;
    arg0->unk68 = arg0->unk6C + temp_v0;
    arg0->unkCA = 1;
    temp_t7 = arg0->restartPos;
    return temp_t7 + temp_v1;
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
    s32 temp_v0;

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

// func_8009D5A8 best match: 99.871%

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009D5A8.s")

#ifdef NON_MATCHING
s32 func_8009D5A8(PlayerCommandInit *arg0) {
    PlayerCommandInit *config;
    volatile s16 sp92;
    s16 sp90;
    volatile s8 sp94;
    PlayerCommandSynConfig synConfig;
    s32 config2[3];
    s32 i;
    s32 stateOffset;
    s32 voiceOffset;

    config = arg0;
    D_8015A658 = config->count;
    D_8015A664 = config->unk1C;
    D_8015A668 = config->unk20;
    libmus_fxheader_current = config->fxHeader;
    D_8015A670 = config->unk28;

    if (osTvType == 0) {
        D_8015A678 = 50;
    } else {
        D_8015A678 = 60;
    }

    func_8009F748(config->heapBase, 0, config->heapLen);
    alHeapInit(&D_8015A648, config->heapBase, config->heapLen);

    D_8015A65C = alHeapDBAlloc(0, 0, &D_8015A648, 1, D_8015A658 * sizeof(ALVoice));
    D_8015A660 = alHeapDBAlloc(0, 0, &D_8015A648, 1, D_8015A658 * sizeof(PlayerCommandState));

    func_8009F748(D_8015A65C, 0, D_8015A658 * sizeof(ALVoice));
    func_8009F748(D_8015A660, 0, D_8015A658 * sizeof(PlayerCommandState));

    synConfig.maxVVoices = D_8015A658;
    synConfig.maxPVoices = D_8015A658;
    synConfig.maxUpdates = config->maxUpdates;
    synConfig.dmaproc = 0;
    synConfig.fxType = 2;
    synConfig.outputRate = 0;
    synConfig.heap = &D_8015A648;

    config2[0] = config->maxFXBusses;
    config2[1] = config->unk38;
    config2[2] = config->unk34;

    func_8009F810(config->unk4, &synConfig.maxVVoices, config->outputRate, config2,
                  config->unk3C, config->unk40, D_8015A678);
    func_8009F344(config->unk14, config->unk18);
    func_8009D8B0(3, 0x7FFF);

    D_8015A68C = 0;
    D_8015A680 = 1;
    D_8015A684 = 0x12345678;

    D_8015A630.next = 0;
    D_8015A630.handler = func_8009E0D4;
    D_8015A630.clientData = &D_8015A630;
    alSynAddPlayer(&D_8015A8D8, &D_8015A630);

    i = 0;
    stateOffset = 0;
    if (D_8015A658 > 0) {
        voiceOffset = 0;
        do {
            ((PlayerCommandState *)((u8 *)D_8015A660 + stateOffset))->unkE4 = 0;
            func_8009F604((PlayerCommandState *)((u8 *)D_8015A660 + stateOffset));

            sp94 = 0;
            sp90 = config->outputRate;
            sp92 = 0;
            alSynAllocVoice(&D_8015A8D8, (ALVoice *)(D_8015A65C + voiceOffset), (ALVoiceConfig *)&sp90);

            i++;
            stateOffset += sizeof(PlayerCommandState);
            voiceOffset += sizeof(ALVoice);
        } while (i < D_8015A658);
    }

    return D_8015A64C - D_8015A648.base;
}
#endif

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

// func_8009E76C best match: 99.609%

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009E76C.s")

#ifdef NON_MATCHING
extern void alSynSetVol(void *, void *, s16, s32);
extern void alSynSetPan(void *, void *, s32);
extern u8 *D_8015A65C;
extern s32 D_8015A678;

void func_8009E76C(PlayerCommandState *arg0, s32 arg1) {
    u32 volume;
    s32 stopping;
    u8 pan;
    u8 oldPan;

    volume = (u32)(arg0->unkEF * arg0->unkF9 * arg0->unk108 * arg0->unkB0) >> 13;
    if (volume >= 0x8000U) {
        volume = 0x7FFF;
    }

    if (arg0->unkBE == 0) {
        volume *= D_8015A67E;
    } else {
        volume *= D_8015A67C;
    }

    stopping = arg0->unk18;
    volume >>= 15;

    if (stopping != -1) {
        volume = (stopping * volume) / arg0->unk1C;
    }

    if (volume != arg0->unkB6) {
        arg0->unkB6 = volume;
        alSynSetVol(&D_8015A8D8, D_8015A65C + (arg1 * 0x1C), (s16)volume, 0xF4240 / D_8015A678);
    }

    oldPan = arg0->unkE3;
    pan = ((arg0->unkF2 * arg0->unkB2) >> 7) & 0x7F;
    if (pan != oldPan) {
        arg0->unkE3 = pan;
        alSynSetPan(&D_8015A8D8, D_8015A65C + (arg1 * 0x1C), pan & 0xFF);
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009E938.s")

// func_8009EB6C best match: 89.464%

#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009EB6C.s")

#ifdef NON_MATCHING
void func_8009EB6C(PlayerCommandState *arg0) {
    s32 max = 0x7FFFFFFF;
    u16 temp_v0;
    u8 temp_f5;
    u8 temp_f4;

    if (arg0->unkBC != 0x7FFF) {
        temp_v0 = arg0->unkC2;
        if (temp_v0 != 0) {
            arg0->unk100 = arg0->unk10 + (temp_v0 << 8);
        } else {
            arg0->unk100 = arg0->unkC - (arg0->unkC4 << 8);
        }
    } else {
        arg0->unk100 = max;
    }

    temp_f5 = arg0->unkF5;
    temp_f4 = arg0->unkF4;
    arg0->unkF8 = 1;
    arg0->unkF9 = temp_f5;
    arg0->unkFA = temp_f4;
}
#endif

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

void func_8009EF44(PlayerCommandState *arg0) {
    s32 temp_v1;
    register f32 temp_fv1;
    u8 temp_v0;

    temp_v0 = arg0->unkE9;
    if (temp_v0 != 0) {
        temp_v1 = arg0->unkC6 - arg0->unk105;
        if (temp_v1 > 0) {
            temp_fv1 = sinf((f32)((f64)((temp_v1 / (f32)temp_v0) * ((float)2.0)) * D_800E1A78)) * arg0->unk44;
            arg0->unk48 = temp_fv1;
        }
    }
}

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

// func_8009FD74 best match: 85.904% (nonmatchings/func_8009FD74-2775475442547365205/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009FD74.s")

#ifdef NON_MATCHING
s32 func_8009FD74(AudioTask *task, AudioInfo *info) {
    u32 outBuf;
    s32 cmdLen;
    Acmd *cmdListEnd;

    func_800A0170();
    outBuf = osVirtualToPhysical(task->outBuf);

    if (info != NULL) {
        osAiSetNextBuffer(info->buf, info->len * 4);
    }

    task->outLen = ((D_8015C938 - (osAiGetLength() >> 2)) + 0x68) & 0xFFF0;
    if ((u32)task->outLen < (u32)D_8015C934) {
        task->outLen = D_8015C934;
    }

    cmdListEnd = alAudioFrame(D_8015A6A0[D_800DF298], &cmdLen, (s16 *)outBuf, task->outLen);
    if (cmdLen == 0) {
        return 0;
    }

    task->unk8 = 0;
    task->msgQ = (OSMesgQueue *)D_8015A8A0;
    task->msg = (OSMesg)&task->unk68;
    task->unk10 = 0;
    task->dataPtr = D_8015A6A0[D_800DF298];
    task->dataSize = (((s32)cmdListEnd - (s32)D_8015A6A0[D_800DF298]) >> 3) << 3;

    task->type = 2;
    task->flags = 0;
    task->ucodeBoot = rspbootTextStart;
    task->ucodeBootSize = (u8 *)aspMainTextStart - (u8 *)rspbootTextStart;
    task->ucode = aspMainTextStart;
    task->ucodeData = D_800E1F00;
    task->ucodeDataSize = 0x800;
    task->dramStack = NULL;
    task->dramStackSize = 0;
    task->outputBuff = NULL;
    task->outputBuffSize = NULL;
    task->yieldDataPtr = NULL;
    task->yieldDataSize = 0;

    osSendMesg(func_8009C434(D_8015C960), &task->unk8, 1);
    D_800DF298 = D_800DF298 ^ 1;
    return 1;
}
#endif

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

// func_8009FF80 best match: 86.936%
#pragma GLOBAL_ASM("asm/nonmatchings/player_commands/func_8009FF80.s")

#ifdef NON_MATCHING
s32 func_8009FF80(s32 addr, s32 len, void *state) {
    register ALLink *first;
    register ALLink *node;
    ALLink *last;
    Struct800A0170Node *dmaNode;
    u32 aligned;
    u32 offset;
    void *buffer;

    last = NULL;
    first = D_8015C928.activeList;
    node = first;
    if (node != NULL) {
        s32 dmaLen = D_8015C970;
        do {
            dmaNode = (Struct800A0170Node *)node;
            if ((u32)addr < (u32)dmaNode->addr) {
                break;
            }
            last = node;
            if ((dmaNode->addr + dmaLen) >= (addr + len)) {
                dmaNode->counter = D_800DF290;
                return osVirtualToPhysical((u8 *)dmaNode->buffer + addr - dmaNode->addr);
            }
            node = node->next;
        } while (node != NULL);
    }

    node = D_8015C928.readyList;
    if (node == NULL) {
        return osVirtualToPhysical(first);
    }

    D_8015C928.readyList = node->next;
    alUnlink(node);
    if (last != NULL) {
        alLink(node, last);
    } else {
        first = D_8015C928.activeList;
        if (first != NULL) {
            D_8015C928.activeList = node;
            node->next = first;
            node->prev = NULL;
            first->prev = node;
        } else {
            D_8015C928.activeList = node;
            node->next = NULL;
            node->prev = NULL;
        }
    }

    offset = addr & 1;
    aligned = addr - offset;
    dmaNode = (Struct800A0170Node *)node;
    dmaNode->addr = aligned;
    dmaNode->counter = D_800DF290;
    buffer = dmaNode->buffer;
    osPiStartDma(&D_8015C968[D_800DF294++], 0, 0, aligned, buffer, D_8015C970, &D_8015C948);
    return osVirtualToPhysical(buffer) + offset;
}
#endif

ALDMAproc func_800A0138(Struct800A0138 **arg0) {
    if (D_8015C928.initialized == 0) {
        D_8015C928.activeList = 0;
        D_8015C928.readyList = D_8015C964;
        D_8015C928.initialized = 1;
    }
    *arg0 = &D_8015C928;
    return func_8009FF80;
}

void func_800A0170(void) {
    OSMesg msg[2];
    u32 i;
    Struct800A0170Node *node;
    Struct800A0170Node *next;

    i = 0;
    if (D_800DF294 != 0) {
        do {
            osRecvMesg(&D_8015C948, msg, 0);
            i++;
        } while (i < D_800DF294);
    }

    node = (Struct800A0170Node *)D_8015C928.activeList;
    if (node != NULL) {
        do {
            next = (Struct800A0170Node *)node->node.next;
            if ((node->counter + 1) < D_800DF290) {
                if ((ALLink *)node == D_8015C928.activeList) {
                    D_8015C928.activeList = &((Struct800A0170Node *)node->node.next)->node;
                }
                alUnlink(&node->node);
                if (D_8015C928.readyList != NULL) {
                    alLink(&node->node, D_8015C928.readyList);
                } else {
                    D_8015C928.readyList = &node->node;
                    node->node.next = (node->node.prev = NULL);
                }
            }
            node = next;
            i = 0;
        } while (next != NULL);
    }

    D_800DF294 = 0;
    D_800DF290++;
}
