#include "common.h"
#include "game_audio.h"

#define GAME_AUDIO_FREE_HANDLE_COUNT 13
#define GAME_AUDIO_QUEUE_CAPACITY 64
#define GAME_AUDIO_QUEUE_MASK (GAME_AUDIO_QUEUE_CAPACITY - 1)

typedef struct GameAudioHandleNode {
    struct GameAudioHandleNode *prev;
    struct GameAudioHandleNode *next;
    s32 handle;
    u16 priority;
    u8 stopRequested;
    u8 volume;
} GameAudioHandleNode;

typedef struct GameAudioQueueEntry {
    u8 type;
    u8 soundId;
    u8 volume;
    u8 pan;
    u8 priority;
    u8 unk5;
} GameAudioQueueEntry;

typedef struct PositionalSoundRequest {
    struct PositionalSoundRequest *next;
    SoundPosition pos;
    s16 soundId;
    s16 volume;
    s16 minVolume;
    s16 priority;
    s16 mode;
    f32 pitch;
} PositionalSoundRequest;

typedef struct RacePlayerSoundView {
    u8 pad0[0x1C];
    SoundPosition pos;
    u8 pad28[0x60C - 0x28];
} RacePlayerSoundView;

typedef union GameAudioHalfArg {
    s32 word;
    struct {
        s16 unused;
        s16 value;
    } half;
} GameAudioHalfArg;

extern s16 D_80121B50;
extern s32 D_80121850;
extern s32 D_80121858;
extern s32 D_8012185C;
extern s32 D_80121974;
extern s32 D_80121AF8;
extern s32 D_80121AFC;
extern s32 D_80121B30;
extern GameAudioHandleNode *D_80121930;
extern GameAudioHandleNode *D_80121934;
extern s32 D_8012193C;
extern GameAudioHandleNode *D_80121940[];
extern GameAudioQueueEntry D_80121978[];
extern s32 player_bss_0048;
extern u16 D_800DBCF4[];
extern s32 D_80121B08[];
extern RacePlayerSoundView D_80121D80[];

void func_8009DE50(s32 arg0, s32 arg1);
s32 func_8009DF14(s32 arg0, s32 arg1);
s32 func_8009DC68(s32 soundId, s32 volume, s32 pan, s32 arg3, s32 priority);
void *func_80048388(s32 arg0);
void func_800720E4(s32 arg0);
s32 func_80071B74(void);
s32 func_800722F0(SoundPosition *pos, s32 volume);

#pragma GLOBAL_ASM("asm/nonmatchings/game_audio/func_80071830.s")

void func_80071A3C(s32 arg0) {
    if (D_8012185C == 0) {
        if (D_80121858 != 0) {
            func_8009DE50(D_80121858, arg0);
        }
        D_8012185C = 1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_audio/func_80071A8C.s")

s32 func_80071B74(void) {
    u32 ret;
    s32 temp_v1;

    temp_v1 = D_80121AF8;
    ret = -1;
    if (D_80121AFC == ((temp_v1 + 1) & GAME_AUDIO_QUEUE_MASK)) {
        return ret;
    }
    (&D_80121AF8)[(D_80121AFC == ((temp_v1 + 1) & GAME_AUDIO_QUEUE_MASK)) * 0] =
        (temp_v1 + 1) & GAME_AUDIO_QUEUE_MASK;
    return temp_v1;
}

s32 func_80071BB0(void) {
    u32 ret;
    s32 temp_v1;

    temp_v1 = D_80121AFC;
    ret = -1;
    if (temp_v1 == D_80121AF8) {
        return ret;
    }
    (&D_80121AFC)[(temp_v1 == D_80121AF8) * 0] = (temp_v1 + 1) & GAME_AUDIO_QUEUE_MASK;
    return temp_v1;
}

void func_80071BE8(GameAudioHandleNode *arg0) {
    GameAudioHandleNode *temp_v0;
    GameAudioHandleNode *temp_v1;

    temp_v0 = arg0->prev;
    if (temp_v0 == NULL) {
        D_80121930 = arg0->next;
        temp_v1 = arg0->next;
        if (temp_v1 != NULL) {
            temp_v1->prev = arg0->prev;
        } else {
            D_80121934 = NULL;
        }
    } else {
        temp_v0->next = arg0->next;
        temp_v1 = arg0->next;
        if (temp_v1 != NULL) {
            temp_v1->prev = arg0->prev;
        } else {
            D_80121934 = arg0->prev;
        }
    }
    D_80121940[D_8012193C] = arg0;
    D_8012193C++;
}

GameAudioHandleNode *func_80071C84(void) {
    s32 count;

    count = D_8012193C;
    if (count == 0) {
        return NULL;
    }
    D_8012193C--;
    return D_80121940[D_8012193C];
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_audio/func_80071CC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_audio/func_80071E80.s")

void func_800720E4(s32 arg0) {
    if (arg0 != D_80121974) {
        D_80121974 = arg0;
        D_80121850 = 1;
        player_bss_0048 = arg0;
    }
}

void func_80072114(s32 arg0) {
    D_80121850 = 2;
    player_bss_0048 = arg0;
    D_80121974 = -1;
}

s32 func_80072138(s16 arg0, s16 arg1) {
    s32 temp_v0 = func_80071B74();
    GameAudioQueueEntry *temp_v1;

    if (temp_v0 == -1) {
        return 1;
    }
    temp_v1 = &D_80121978[temp_v0];
    temp_v1->type = 1;
    temp_v1->soundId = arg0;
    temp_v1->volume = 0xFF;
    temp_v1->pan = 0x80;
    temp_v1->unk5 = 0;
    temp_v1->priority = arg1;
    return 0;
}

s32 func_800721B8(s16 arg0, s16 arg1, s16 arg2) {
    s32 temp_v0;
    GameAudioQueueEntry *temp_v1;

    if (arg1 <= 0) {
        return 0;
    }
    temp_v0 = func_80071B74();
    if (temp_v0 == -1) {
        return 1;
    }
    if (arg1 >= 0x100) {
        arg1 = 0xFF;
    }
    temp_v1 = &D_80121978[temp_v0];
    temp_v1->type = 1;
    temp_v1->soundId = (u8)arg0;
    temp_v1->volume = (u8)arg1;
    temp_v1->pan = 0x80;
    temp_v1->unk5 = 0;
    temp_v1->priority = (u8)arg2;
    return 0;
}

extern void func_8009DD5C(s32 arg0, s32 arg1);
extern void func_8009DDE4(s32 arg0);
extern void osStartThread(void *);
extern void osStopThread(void *);
extern s8 D_8015A6B8;
extern s8 D_80121B00;

void func_80072260(void) {
    osStopThread(&D_8015A6B8);
    D_80121B00 = 0;
    D_80121AF8 = 0;
    D_80121AFC = 0;
    func_8009DD5C(1, 0);
    osStartThread(&D_8015A6B8);
}

void func_800722B4(void) {
    osStopThread(&D_8015A6B8);
    func_8009DD5C(3, 0x14);
    osStartThread(&D_8015A6B8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_audio/func_800722F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_audio/func_80072518.s")

void func_8007276C(s32 soundId, s32 playerIndex, s32 volume, s32 minVolume) {
    s32 adjustedVolume;

    adjustedVolume = func_800722F0(&D_80121D80[playerIndex].pos, volume);
    if (adjustedVolume < minVolume) {
        adjustedVolume = minVolume;
    }
    if (adjustedVolume != 0) {
        if (D_80121B08[playerIndex] != 0) {
            func_8009DE50(D_80121B08[playerIndex], 0);
            D_80121B08[playerIndex] = 0;
        }
        D_80121B08[playerIndex] = func_8009DC68(soundId, adjustedVolume, 0x80, 0, 0x5A);
    }
}

void func_80072844(s16 soundId, SoundPosition *pos, s16 volume) {
    s32 adjustedVolume;

    adjustedVolume = func_800722F0(pos, volume);
    if (adjustedVolume == 0) {
        if (D_80121B30 != 0) {
            func_8009DE50(D_80121B30, 0);
            D_80121B30 = 0;
        }
    } else if (D_80121B30 == 0) {
        D_80121B30 = func_8009DC68(soundId, adjustedVolume, 0x80, 0, 0x32);
    } else {
        func_8009DF14(D_80121B30, adjustedVolume);
    }
}

void func_800728E0(void) {
    func_800720E4(D_800DBCF4[D_80121B50]);
}

void func_80072918(void) {
    func_8009DDE4(3);
}

void func_80072938(void) {
    func_8009DDE4(2);
}

extern PositionalSoundRequest *gPendingPositionalSoundRequests;
extern void enqueuePositionalSoundRequest(s32, SoundPosition *, s32, s32, f32, s32, s32);

void clearPendingPositionalSoundRequests(void) {
    gPendingPositionalSoundRequests = NULL;
}

void enqueuePositionalSoundRequest(s32 soundId, SoundPosition *pos, s32 volume, s32 priority, f32 pitch, s32 mode,
                                   s32 minVolume) {
    PositionalSoundRequest *node;

    node = func_80048388(sizeof(PositionalSoundRequest));
    if (node != NULL) {
        node->next = gPendingPositionalSoundRequests;
        node->pos = *pos;
        node->soundId = ((GameAudioHalfArg *)&soundId)->half.value;
        node->volume = ((GameAudioHalfArg *)&volume)->half.value;
        node->minVolume = ((GameAudioHalfArg *)&minVolume)->half.value;
        node->priority = ((GameAudioHalfArg *)&priority)->half.value;
        node->mode = ((GameAudioHalfArg *)&mode)->half.value;
        node->pitch = pitch;
        gPendingPositionalSoundRequests = node;
    }
}

void func_80072A20(s32 arg0, SoundPosition *arg1, s32 arg2, s32 arg3, f32 arg4, s16 mode) {
    s32 temp_a0 = arg0 << 16;
    s32 temp_a2 = arg2 << 16;
    s32 temp_a3 = arg3 << 16;

    enqueuePositionalSoundRequest(temp_a0 >> 16, arg1, temp_a2 >> 16, temp_a3 >> 16, arg4, mode, 0);
}

void func_80072A74(s16 arg0, SoundPosition *arg1, s16 arg2, s16 arg3) {
    enqueuePositionalSoundRequest(arg0, arg1, arg2, arg3, 0.0f, -1, 0);
}

void func_80072AC8(s16 arg0, SoundPosition *arg1, s16 arg2, s16 arg3, s16 arg4, s16 minVolume) {
    enqueuePositionalSoundRequest(arg0, arg1, arg2, arg3, 0.0f, arg4 + 4, minVolume);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_audio/func_80072B24.s")
