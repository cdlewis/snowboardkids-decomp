#include "common.h"
#include "game_audio.h"
#include "game_boot.h"

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

typedef struct AudioRomRange {
    s32 words[2];
} AudioRomRange;

typedef union GameAudioHalfArg {
    s32 word;
    struct {
        s16 unused;
        s16 value;
    } half;
} GameAudioHalfArg;

extern s16 D_8011213A;
extern s16 D_80121B50;
extern s32 D_80121850;
extern s32 D_80121858;
extern s32 D_8012185C;
extern s32 D_80121974;
extern s32 D_80121AF8;
extern s32 D_80121AFC;
extern s32 D_80121B18;
extern s32 D_80121B30;
extern GameAudioHandleNode *D_80121930;
extern GameAudioHandleNode *D_80121934;
extern s32 D_8012193C;
extern GameAudioHandleNode *D_80121940[];
extern GameAudioQueueEntry D_80121978[];
extern AudioRomRange D_800DBC5C[];
extern AudioRomRange D_800DBCAC[];
extern s32 player_bss_0048;
extern u16 D_800DBCF4[];
extern s32 D_80121B08[];
extern RacePlayerSoundView D_80121D80[];
extern u8 D_80121B00;
extern u8 D_80121B01;
extern u8 D_80121B02;
extern u8 D_80121B03;
extern u8 D_80121B04;
extern u8 D_80121B05;
extern s8 D_8015A6B8;

void func_8009D8B0(s32 arg0, s32 arg1);
void func_8009DE50(s32 arg0, s32 arg1);
void osStartThread(void *);
void osStopThread(void *);
s32 func_8009D8D8(s32 arg0);
s32 func_8009DEC4(s32 arg0);
s32 func_8009DF14(s32 arg0, s32 arg1);
s32 func_8009DC68(s32 soundId, s32 volume, s32 pan, s32 arg3, s32 priority);
s32 func_80043040(s16 arg0);
void *func_80048388(s32 arg0);
void func_800720E4(s32 arg0);
s32 func_80071B74(void);
s32 func_80071CC0(void);
s32 func_800722F0(SoundPosition *pos, s32 volume);
void func_80072518(s16 soundId, s32 mode, s16 volume, f32 pitch);

#pragma GLOBAL_ASM("asm/nonmatchings/game_audio/func_80071830.s")

void func_80071A3C(s32 arg0) {
    if (D_8012185C == 0) {
        if (D_80121858 != 0) {
            func_8009DE50(D_80121858, arg0);
        }
        D_8012185C = 1;
    }
}

s32 func_80071A8C(s32 arg0) {
    s32 size;
    AudioRomRange *range;

    if (D_80121858 == 0) {
        range = (AudioRomRange *)((arg0 * 2) + (s32 *)D_800DBC5C);
        size = range->words[1] - range->words[0];
        func_80099C44(range->words[0], func_80043040(D_8011213A), size);
        if ((D_80121858 = func_8009D8D8(func_80043040(D_8011213A))) != 0) {
            D_80121974 = arg0;
            if (range == D_800DBCAC) {
                func_8009D8B0(2, 0x7FFF);
            } else {
                func_8009D8B0(2, 0x60FF);
            }
            D_8012185C = 0;
            return 0;
        }
    } else {
        func_80071A3C(4);
    }
    return 1;
}

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

// func_80071CC0 best match: 98.108%
#pragma GLOBAL_ASM("asm/nonmatchings/game_audio/func_80071CC0.s")

#ifdef NON_MATCHING
s32 func_80071CC0(void) {
    GameAudioHandleNode *temp_v0;
    GameAudioHandleNode *temp_v0_2;
    GameAudioHandleNode *var_v1;
    s32 var_a0;

    temp_v0 = func_80071C84();
    if (temp_v0 == NULL) {
        if (D_80121B04 < D_80121934->priority) {
            return 1;
        }
        if (D_80121934->stopRequested != 0) {
            D_80121934->stopRequested = 0;
            func_8009DE50(D_80121934->handle, 0);
        }
        return 0;
    }

    var_a0 = D_80121B01;
    if ((var_a0 >= 0x70) || (var_a0 < 0)) {
        D_80121B01 = 0;
        var_a0 = 0 & 0xFF;
    }

    temp_v0->handle = func_8009DC68(var_a0, D_80121B02, D_80121B03, 0, D_80121B04);
    temp_v0->volume = D_80121B05;
    temp_v0->priority = D_80121B04;
    temp_v0->stopRequested = 1;

    if (D_80121930 == NULL) {
        temp_v0->prev = D_80121930;
        temp_v0->next = NULL;
        D_80121930 = (D_80121934 = temp_v0);
        return 1;
    }

    var_v1 = D_80121930;
    if (D_80121930 != NULL) {
        do {
            if (temp_v0->priority >= var_v1->priority) {
                temp_v0_2 = var_v1->prev;
                temp_v0->next = var_v1;
                temp_v0->prev = temp_v0_2;
                if (temp_v0_2 == NULL) {
                    D_80121930 = temp_v0;
                } else {
                    temp_v0_2->next = temp_v0;
                }
                var_v1->prev = temp_v0;
                return 1;
            }
            var_v1 = var_v1->next;
        } while (var_v1 != NULL);
    }

    temp_v0->next = D_80121934->next;
    temp_v0->prev = D_80121934;
    D_80121934->next = temp_v0;
    D_80121934 = temp_v0;
    return 1;
}
#endif

void func_80071E80(void) {
    GameAudioHandleNode *node;
    s32 *left;
    s32 *right;
    GameAudioQueueEntry *entry;
    s32 index;

    osStopThread(&D_8015A6B8);

    node = D_80121930;
    while (node != NULL) {
        if (node->handle == 0) {
            func_80071BE8(node);
            goto next_node;
        } else {
            if (func_8009DEC4(node->handle) == 0) {
                func_80071BE8(node);
            }
        }
    next_node:
        node = node->next;
    }

    right = &D_80121B18, left = D_80121B08;
    do {
        if ((*right != 0) && (func_8009DEC4(*right) == 0)) {
            *right = 0;
        }
        if ((*left != 0) && (func_8009DEC4(*left) == 0)) {
            *left = 0;
        }
        right++;
    } while (&D_80121B18 > ++left);

    if ((D_80121B30 != 0) && (func_8009DEC4(D_80121B30) == 0)) {
        D_80121B30 = 0;
    }

    if (D_80121B00 == 0) {
        index = func_80071BB0();
        if (index != -1) {
            entry = &D_80121978[index];
            D_80121B00 = entry->type;
            D_80121B01 = entry->soundId;
            D_80121B02 = entry->volume;
            D_80121B03 = entry->pan;
            D_80121B04 = entry->priority;
            D_80121B05 = entry->unk5;
        }
    }

    if ((D_80121B00 == 1) && (func_80071CC0() != 0)) {
        D_80121B00 = 0;
    }

    if ((D_80121858 != 0) && (func_8009DEC4(D_80121858) == 0)) {
        D_80121858 = 0;
        D_80121974 = -1;
    }

    if ((D_80121850 == 1) && (func_80071A8C(player_bss_0048) == 0)) {
        D_80121850 = 0;
    }

    if (D_80121850 == 2) {
        func_80071A3C(player_bss_0048);
        D_80121850 = 0;
    }

    osStartThread(&D_8015A6B8);
}

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

s32 func_80072938(void) {
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

void func_80072B24(void) {
    PositionalSoundRequest *node;
    s16 mode;

    osStopThread(&D_8015A6B8);
    node = gPendingPositionalSoundRequests;

    while (node != NULL) {
        mode = node->mode;
        switch (mode) {
            case -1:
                func_800721B8(node->soundId, func_800722F0(&node->pos, node->volume), node->priority);
                break;

            case 0:
            case 1:
            case 2:
            case 3:
                func_80072518(node->soundId, mode, node->volume, node->pitch);
                break;

            case 4:
            case 5:
            case 6:
            case 7:
                func_8007276C(node->soundId, mode - 4, node->volume, node->minVolume);
                break;

            case 10:
                func_80072844(node->soundId, &node->pos, node->volume);
                break;
        }
        node = node->next;
    }

    osStartThread(&D_8015A6B8);
}
