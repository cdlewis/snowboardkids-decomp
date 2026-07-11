#ifndef PLAYER_COMMANDS_H
#define PLAYER_COMMANDS_H

#include "common.h"
#include <PR/libaudio.h>

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

typedef struct AudioThreadLocals {
    SchedulerClient client;
    OSMesg msg;
    s32 pad;
} AudioThreadLocals;

typedef struct PlayerCommandData {
    s32 trackCount;
    s32 *sequenceOffsets;
    s32 *unk8;
    s32 *unkC;
    u8 *commands;
    s32 *jumpTable;
} PlayerCommandData;

typedef struct PlayerCommandClearBlock {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} PlayerCommandClearBlock;

typedef struct PlayerCommandState {
    u32 unk0;
    u32 unk4;
    u8 pad8[0x4];
    u32 unkC;
    u32 unk10;
    s32 id;
    s32 fadeTarget;
    s32 fadeTime;
    s32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
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
    u16 unkB6;
    s16 unkB8;
    u16 unkBA;
    u16 unkBC;
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
    u8 unkE2;
    u8 unkE3;
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
    u8 unkF4;
    u8 unkF5;
    u8 unkF6;
    u8 unkF7;
    u8 unkF8;
    u8 unkF9;
    u8 unkFA;
    u8 unkFB;
    u8 unkFC;
    s8 unkFD;
    u8 unkFE;
    u8 unkFF;
    u32 unk100;
    u8 unk104;
    u8 unk105;
    u8 unk106;
    u8 unk107;
    u8 unk108;
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
    s16 retrace;
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

typedef struct PlayerCommandBank {
    u8 pad0[0x10];
    s32 count;
    s32 offsets[1];
} PlayerCommandBank;

typedef struct PlayerCommandRelocEntry {
    s32 unk0;
    u8 pad4[4];
    u8 unk8;
    u8 relocated;
    u8 padA[2];
    s32 unkC;
    s32 unk10;
} PlayerCommandRelocEntry;

typedef struct PlayerCommandInit {
    s32 count;
    void *unk4;
    s32 outputRate;
    u8 *heapBase;
    s32 heapLen;
    PlayerCommandBank *unk14;
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
    u8 pad1D[4];
} PlayerCommandSynConfig;

typedef struct AudioInfo {
    void *buf;
    s16 len;
} AudioInfo;

typedef struct AudioFrameMessage {
    s16 type;
    s16 unk2;
    AudioInfo *info;
} AudioFrameMessage;

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

void func_8009C270(SchedulerState *arg0, u8 arg1, u8 arg2);
s32 func_8009C434(s32 arg0);
s32 func_8009C43C(s32 arg0);
void func_8009C444(SchedulerState *arg0);
void func_8009C6DC(SchedulerState *arg0);
void func_8009C77C(SchedulerState *arg0);
void func_8009C81C(SchedulerState *arg0);
void func_8009C8DC(SchedulerState *arg0);
void func_8009CA60(SchedulerState *arg0, SchedulerClient *arg1, OSMesgQueue *arg2);
void func_8009CAB4(SchedulerState *arg0, SchedulerClient *arg1);
void func_8009CB44(SchedulerState *arg0, s32 arg1);
void func_8009CB98(SchedulerState *arg0, SchedulerTask *arg1);
void func_8009CC50(SchedulerState *arg0);
s32 func_8009CCA0(PlayerCommandState *arg0, s32 arg1);
void *func_8009CCC0(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009CCFC(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009CD0C(PlayerCommandState *arg0, s32 arg1);
s32 func_8009CD18(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009CE3C(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009CF1C(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009CF30(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009CF50(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009CFAC(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009D00C(PlayerCommandState *arg0, s32 arg1);
s32 func_8009D020(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009D064(PlayerCommandState *arg0, s32 arg1);
s32 func_8009D074(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009D084(PlayerCommandState *arg0, s32 arg1);
s32 func_8009D094(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009D0E0(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009D138(PlayerCommandState *arg0, s32 arg1);
s32 func_8009D148(PlayerCommandState *arg0, s32 arg1);
s32 func_8009D154(PlayerCommandState *arg0, s32 arg1);
s32 func_8009D164(PlayerCommandState *arg0, s32 arg1);
s32 func_8009D170(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009D1EC(PlayerCommandState *arg0, s32 arg1);
s32 func_8009D27C(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009D2A0(PlayerCommandState *arg0, s32 arg1);
s32 func_8009D2AC(PlayerCommandState *arg0, s32 arg1);
s32 func_8009D2BC(PlayerCommandState *arg0, s32 arg1);
s32 func_8009D2C8(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009D2DC(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009D2FC(s32 arg0, s32 arg1);
void *func_8009D308(void *arg0, u8 *arg1);
s32 func_8009D330(PlayerCommandState *arg0, s32 arg1);
s32 func_8009D33C(s32 arg0, s32 arg1);
s32 func_8009D348(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009D3AC(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009D3BC(void *arg0, u8 *arg1);
s32 func_8009D40C(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009D45C(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009D4AC(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009D4BC(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009D598(PlayerCommandState *arg0, u8 *arg1);
s32 func_8009D5A8(PlayerCommandInit *arg0);
void func_8009D8B0(s32 arg0, s32 arg1);
s32 func_8009D8D8(PlayerCommandData *arg0);
s32 func_8009DBE4(s32 arg0);
s32 func_8009DC68(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_8009DD5C(s32 arg0, s32 arg1);
s32 func_8009DDE4(s32 arg0);
s32 func_8009DE50(s32 arg0, s32 arg1);
s32 func_8009DEC4(s32 arg0);
s32 func_8009DF14(s32 arg0, s32 arg1);
s32 func_8009DF78(s32 arg0, s32 arg1);
s32 func_8009DFDC(s32 arg0, f32 arg1);
s32 func_8009E040(s32 arg0, s32 arg1);
ALMicroTime func_8009E0D4(void *arg0);
void func_8009E354(PlayerCommandState *arg0, s32 arg1);
void func_8009E76C(PlayerCommandState *arg0, s32 arg1);
void func_8009E938(PlayerCommandState *arg0, s32 arg1);
void func_8009EB6C(PlayerCommandState *arg0);
void func_8009EBDC(PlayerCommandState *arg0);
void func_8009EEE8(PlayerCommandState *arg0);
void func_8009EF44(PlayerCommandState *arg0);
void func_8009EFF4(PlayerCommandState *arg0);
void func_8009F0C4(PlayerCommandState *arg0);
f32 func_8009F194(f32 arg0);
void func_8009F344(PlayerCommandBank *arg0, s32 arg1);
s32 func_8009F4C8();
void func_8009F604(PlayerCommandState *arg0);
s32 func_8009F6F4(s32 arg0, s32 arg1);
void func_8009F748(u8 *p, unsigned char c, s32 n);
s32 func_8009F780(PlayerCommandState *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_8009F810(void *arg0, ALSynConfig *arg1, s32 arg2, s32 *arg3, s32 arg4, s32 arg5, s32 arg6);
void func_8009FC0C(s32 arg0);
s32 func_8009FD74(AudioTask *task, AudioInfo *info);
void func_8009FF40(s32 arg0);
s32 func_8009FF80(s32 addr, s32 len, void *state);
ALDMAproc func_800A0138(Struct800A0138 **arg0);
void func_800A0170(void);

#endif
