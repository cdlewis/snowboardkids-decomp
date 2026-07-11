#include "common.h"
#include "memory_allocator.h"
#include "effect_task_scheduler.h"
#include "character_select_ui.h"
#include "player_select_ui.h"
#define MENU_RENDERING_BROAD_PROTOTYPES
#include "menu_rendering.h"

#define CHARACTER_SELECT_FRAME_TEXTURE_HANDLE (D_80112130.textureHandle)

typedef u8 CharacterSelectText[0x94];

typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ s16 value;
    /* 0x01A */ u8 pad1A[0x5F2];
} CharacterSelectPlayerRecord;

typedef struct {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ s16 popupFontHandle;
    /* 0x40 */ u8 pad40[2];
    /* 0x42 */ s16 textureHandle;
    /* 0x44 */ u8 pad44[4];
    /* 0x48 */ s16 iconTextureHandle;
} CharacterSelectAssetHandles;

typedef struct {
    /* 0x00 */ u16 center[16];
    /* 0x20 */ u16 right[2];
    /* 0x24 */ u16 bottom[2];
    /* 0x28 */ u16 corner;
} CharacterSelectFrameTileMap;

typedef struct {
    /* 0x00 */ u16 corner;
    /* 0x02 */ u8 pad2[0x28];
} CharacterSelectFrameCornerTileMap;

typedef union {
    u8 bytes[8];
    struct {
        /* 0x00 */ u8 state;
        /* 0x01 */ u8 otherState;
        /* 0x02 */ u8 pad2[2];
        /* 0x04 */ s16 spriteIndex;
        /* 0x06 */ u8 timer;
        /* 0x07 */ u8 otherTimer;
    } fields;
} CharacterSelectCursorState;

extern void func_80046D68(s16, s16, s32, u16, s32);
extern int sprintf(char *, const char *, ...);
extern CharacterSelectAssetHandles D_80112130;
extern s16 D_80112172;
extern s16 D_80112178;
extern s16 D_800B3420[][11];
extern CharacterSelectFrameTileMap D_800B5FC0[];
extern CharacterSelectFrameCornerTileMap D_800B5FE8[];
extern u16 D_800B618C;
extern s16 D_800B3478[];
extern s16 D_800B3480[];
extern u16 D_800B61AC[];
extern u8 D_800B61C0[];
extern u8 D_800B61CC[];
extern u8 D_800B6B88[];
extern CharacterSelectText D_800B6210[];
extern u8 D_800B67D8[][0x74];
extern u8 D_800B6934[][0x60];
extern u8 D_800B6A54[][0x70];
extern u8 D_800B6B34[];
extern u8 D_800ECA24[];
extern s32 D_800EC9F8[];
extern s16 D_80121B50;
extern s32 D_8010ADDC;
extern u16 D_8010AE80;
extern void *D_8010ADE0;
extern void *D_8010ADE4;
extern CharacterSelectCursorState D_8010AE88;
extern u8 D_8010AE88_state;
extern u8 D_8010AE89;
extern u8 D_8010AE8A;
extern u8 D_8010AE8F;
extern u8 D_8010ADF9;
extern u8 D_80121D88;
extern u8 D_800EC9C1;
extern s16 D_800EC9D0;
extern u8 D_800EC9DD;
extern u8 D_800EC9C2;
extern u8 D_80121B5E;
extern void *D_80124868;
extern void func_800483FC(void *, void *, void *);
extern u8 D_80121B55;
extern u8 D_80121D80[];
extern CharacterSelectPlayerRecord D_801235B0;
extern s32 D_801235B4;

// func_8001BD70 best match: 95.048%
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001BD70.s")

#ifdef NON_MATCHING
void func_8001BD70(CharacterSelectMenuFrameActor *arg0) {
    CharacterSelectMenuFrameActor *base;
    s16 *characterIds;
    s16 *characterId;
    s16 *pos;
    s32 i;
    s32 alpha;
    s32 characterIdOffset;

    base = arg0;
    if ((D_800EC9C2 == 0) || (D_800EC9C2 == 2)) {
        characterIds = D_800B3420[D_8010ADF9];
    } else if (D_80121B5E >= 3) {
        characterIds = D_800B3478;
    } else {
        characterIds = D_800B3480;
    }

    i = 0;
    characterIdOffset = 0;
    if (base->itemCount > 0) {
        pos = (s16 *)base;
        do {
            alpha = 0;
            if (i == 0) {
                if ((i == D_80121B50) && (D_800EC9C1 > 0) && (D_800EC9C1 < 8) && (D_800EC9C1 & 1)) {
                    alpha = 0xFF;
                }
                func_8000F030(pos[12], pos[23], func_80043040(D_80112130.textureHandle), 0x1C, 0x20, 0x20, 0, alpha);
            } else if (i == D_8010AE80) {
                if ((D_8010AE80 == D_80121B50) && (D_800EC9C1 > 0) && (D_800EC9C1 < 8) && (D_800EC9C1 & 1)) {
                    alpha = 0xFF;
                }
                if ((D_80121B55 - 1) == 0) {
                    func_8000F030(pos[12], pos[23], func_80043040(D_80112130.textureHandle), 0x1B, 0x20, 0x20, 0, alpha);
                } else {
                    func_8000F030(pos[12], pos[23], func_80043040(D_80112130.textureHandle), 0x1E, 0x20, 0x20, 0, alpha);
                }
            } else {
                if ((i == D_80121B50) && (D_800EC9C1 > 0) && (D_800EC9C1 < 8) && (D_800EC9C1 & 1)) {
                    alpha = 0xFF;
                }
                func_8000F030(pos[12], pos[23], func_80043040(D_80112130.textureHandle), (i + 0x12) & 0xFFFF, 0x20, 0x20, 0, alpha);
            }

            if (i != D_8010AE80) {
                characterId = (s16 *)((u8 *)characterIds + characterIdOffset);
                if (((D_80121B55 - 1) == 0) && (D_800ECA24[*characterId] != 0)) {
                    func_8000F030((s16)(pos[12] - 0x10), pos[23], func_80043040(D_80112130.iconTextureHandle), (i + 0x1A) & 0xFFFF, 0x20, 0x20, 0, D_800ECA24[*characterId] + 6);
                } else {
                    func_8000F030((s16)(pos[12] - 0x10), pos[23], func_80043040(D_80112130.textureHandle), (i + 0x29) & 0xFFFF, 0x20, 0x20, 0, 0);
                }
            }
            i++;
            characterIdOffset += 2;
            pos++;
        } while (i < base->itemCount);
    }
}
#endif

// func_8001C158 best match: 99.352% (nonmatchings/func_8001C158-4923837976568703863/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001C158.s")

#ifdef NON_MATCHING
void func_8001C158(CharacterSelectMenuFrameActor *arg0) {
    s32 anyMoving;
    s32 i;
    s32 limit;
    s32 targetY;
    void (*finalCallback)(EffectTask *);
    CharacterSelectMenuFrameActor *base;
    CharacterSelectMenuFrameActor *ptr;
    s16 *x;

    base = arg0;
    switch (arg0->state) {
    case 0:
        anyMoving = 0;
        i = 0;
        if (base->itemCount > 0) {
            ptr = arg0;
            do {
                if (ptr->x[0] < -0x7C) {
                    ptr->x[0] += 0x10;
                    anyMoving++;
                    if (ptr->x[0] >= -0x7C) {
                        ptr->x[0] = -0x7C;
                    }
                }
                i++;
                ptr = (CharacterSelectMenuFrameActor *)((s16 *)ptr + 1);
            } while (i < base->itemCount);
        }

        arg0->timer++;
        if (!(arg0->timer & 1)) {
            if (arg0->itemCount < (D_8010AE80 + 1)) {
                arg0->itemCount++;
                if (arg0->itemCount == (D_8010AE80 + 1)) {
                    D_8010ADE4 = func_80071408((void (*)(EffectTask *))func_8001DACC, 0, 0x58);
                    func_80071408((void (*)(EffectTask *))func_8001DFA0, 0, 0x59);
                    func_80071408((void (*)(EffectTask *))func_8001E468, 0, 0x5A);
                    finalCallback = (void (*)(EffectTask *))func_80020AA0;
                    func_80071408((void (*)(EffectTask *))func_8001E930, 0, 0x5B);
                    func_80071408((void (*)(EffectTask *))func_8001EDF8, 0, 0x5C);
                    func_80071408((void (*)(EffectTask *))func_8001F2C0, 0, 0x5D);
                    if (D_8010ADF9 != 0) {
                        func_80071408((void (*)(EffectTask *))func_8001F788, 0, 0x5E);
                        if (D_8010ADF9 >= 2) {
                            func_80071408((void (*)(EffectTask *))func_8001FC50, 0, 0x5F);
                            if (D_8010ADF9 >= 3) {
                                func_80071408((void (*)(EffectTask *))func_80020118, 0, 0x60);
                            }
                        }
                    }
                    func_80071408(finalCallback, 0, 0x61);
                }
            }
        }
        if (anyMoving == 0) {
            arg0->state = 1;
        }
        break;
    case 1:
        if (D_80121D88 == 1) {
            arg0->state = 2;
        }
        if (D_80121D88 == 7) {
            arg0->state = 7;
        }
        break;
    case 2:
        if (D_80121B50 != 0) {
            arg0->x[0] -= 0x20;
        }
        if (D_80121B50 != 1) {
            arg0->x[1] -= 0x20;
        }
        if (D_80121B50 != 2) {
            arg0->x[2] -= 0x20;
        }
        i = 3;
        do {
            x = (s16 *)arg0;
            if (i != D_80121B50) {
                x[i + 12] -= 0x20;
            }
            if ((i + 1) != D_80121B50) {
                x[i + 13] -= 0x20;
            }
            if ((i + 2) != D_80121B50) {
                x[i + 14] -= 0x20;
            }
            if ((i + 3) != D_80121B50) {
                x[i + 15] -= 0x20;
            }
            i += 4;
        } while (i != 0xB);

        if (D_80121B50 != 0) {
            if (arg0->x[0] < -0x103) {
                arg0->state = 3;
            }
        } else if (arg0->x[1] < -0x103) {
            arg0->state = 3;
        }
        break;
    case 3:
        arg0->y[D_80121B50] -= arg0->itemSpacing;
        if (arg0->y[D_80121B50] < -0x5F) {
            arg0->y[D_80121B50] = -0x60;
            arg0->state = 4;
            if (D_800EC9C2 == 2) {
                D_8010ADE0 = func_80071408((void (*)(EffectTask *))func_800235E0, 0, 0x62);
                func_80071408((void (*)(EffectTask *))func_800232F4, 0, 0x63);
            } else {
                func_80071408((void (*)(EffectTask *))func_8002186C, 0, 0x61);
                func_80071408((void (*)(EffectTask *))func_80021B20, 0, 0x62);
            }
        }
        break;
    case 4:
        if (D_80121D88 == 4) {
            arg0->state = 5;
        }
        if (D_80121D88 == 7) {
            arg0->state = 7;
        }
        break;
    case 5:
        arg0->y[D_80121B50] += arg0->itemSpacing;
        targetY = (D_80121B50 * arg0->itemSpacing) + arg0->baseY;
        if (arg0->y[D_80121B50] >= targetY) {
            arg0->y[D_80121B50] = targetY;
            arg0->state = 6;
        }
        break;
    case 6:
        limit = D_8010AE80 + 1;
        i = 0;
        if (limit > 0) {
            do {
                if (i != D_80121B50) {
                    arg0->x[i] += 0x20;
                    limit = D_8010AE80 + 1;
                }
                i++;
            } while (i < (D_8010AE80 + 1));
        }
        if (D_80121B50 != 0) {
            if (arg0->x[0] >= -0x7C) {
                arg0->state = 1;
            }
        } else if (arg0->x[1] >= -0x7C) {
            arg0->state = 1;
        }
        if (arg0->state == 1) {
            D_80121D88 = 5;
        }
        break;
    case 7:
        i = 0;
        if ((D_8010AE80 + 1) > 0) {
            ptr = arg0;
            do {
                i++;
                ptr->x[0] -= 0x20;
                ptr = (CharacterSelectMenuFrameActor *)((s16 *)ptr + 1);
            } while (i < (D_8010AE80 + 1));
        }
        break;
    }

    if ((arg0->state == 7) && (arg0->x[0] < -0x103)) {
        func_800716E4((EffectTask *)arg0);
        return;
    }

    func_800483FC(&D_80124868, func_8001BD70, arg0);
}
#endif

// func_8001C83C best match: 96.776% (nonmatchings/func_8001C83C-3836525038718587862/base_9.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001C83C.s")

#ifdef NON_MATCHING
void func_8001C83C(CharacterSelectMenuFrameActor *arg0) {
    u8 spacing;
    s16 baseY;
    s32 i;
    int mode;
    unsigned int limit;

    mode = D_8010ADF9;
    if (mode == 0) {
        if (1) {}
        spacing = 0x18;
        baseY = -0x50;
    } else if (mode == 1) {
        baseY = -0x54;
        spacing = 0x16;
    } else if (mode == 2) {
        spacing = 0x16;
        baseY = -0x60;
    } else {
        baseY = -0x60;
        spacing = 0x13;
    }

    arg0->baseY = baseY;
    arg0->itemSpacing = spacing;
    arg0->y[1] = baseY + spacing;
    arg0->y[2] = baseY + (spacing * 2);
    arg0->x[0] = -0x104;
    arg0->x[1] = -0x104;
    arg0->x[2] = -0x104;
    arg0->x[2] = -0x104;
    limit = 0xB;
    arg0->y[0] = baseY;

    i = 3;
    do {
        arg0->x[i + 1] = -0x104;
        arg0->x[i + 2] = -0x104;
        arg0->x[i + 3] = -0x104;
        arg0->x[i] = -0x104;
        arg0->y[i] = baseY + (i * spacing);
        arg0->y[i + 1] = baseY + ((i + 1) * spacing);
        arg0->y[i + 2] = baseY + ((i + 2) * spacing);
        arg0->y[i + 3] = baseY + ((i + 3) * spacing);
        i += 4;
    } while (i != limit);

    arg0->timer = 0;
    arg0->itemCount = 1;
    arg0->state = 0;
    func_80071824(arg0, func_8001C158);
}
#endif

// func_8001C96C best match: 99.556%
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001C96C.s")

#ifdef NON_MATCHING
void func_8001C96C(CharacterSelectMenuFrameActor *arg0) {
    register CharacterSelectMenuFrameActor *actor;
    s32 arrowBase;
    s32 lastArrowIndex;
    s32 i;
    s32 alpha;
    u16 tile;

    actor = arg0;
    if (D_80121B5E < 2) {
        arrowBase = 2;
    } else {
        arrowBase = 0;
    }
    i = 0;
    lastArrowIndex = arrowBase + 1;
    if (actor->itemCount > 0) {
        do {
            alpha = 0;
            if (i == 0) {
                if (D_80121B5E == 2) {
                    goto use_alt_tile;
                }
                tile = 0x1C;
                goto tile_selected;
use_alt_tile:
                tile = 0x1D;
tile_selected:
                if ((i == D_80121B50) && (D_800EC9C1 > 0) && (D_800EC9C1 < 8) && (D_800EC9C1 & 1)) {
                    alpha = 0xFF;
                }
                func_8000F030(actor->x[i], actor->y[i], func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), tile, 0x20, 0x20, 0, alpha);
            } else if (i == lastArrowIndex) {
                if ((i == D_80121B50) && (D_800EC9C1 > 0) && (D_800EC9C1 < 8) && (D_800EC9C1 & 1)) {
                    alpha = 0xFF;
                }
                func_8000F030(actor->x[i], actor->y[i], func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), 0x20, 0x20, 0x20, 0, alpha);
            } else {
                if ((i == D_80121B50) && (D_800EC9C1 > 0) && (D_800EC9C1 < 8) && (D_800EC9C1 & 1)) {
                    alpha = 0xFF;
                }
                func_8000F030(actor->x[i], actor->y[i], func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), (i + 0x12) & 0xFFFF, 0x20, 0x20, 0, alpha);
            }

            if (i != lastArrowIndex) {
                func_8000F030((s16)(actor->x[i] - 0x10), actor->y[i], func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), (i + 0x29) & 0xFFFF, 0x20, 0x20, 0, 0);
            }
            i++;
        } while (i < actor->itemCount);
    }
}
#endif

// func_8001CC10 best match: 84.118% (nonmatchings/func_8001CC10-7273315160691878794/base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001CC10.s")

#ifdef NON_MATCHING
void func_8001CC10(CharacterSelectMenuFrameActor *arg0) {
    s32 movedCount;
    s32 visibleCount;
    volatile s32 state6Count;
    s32 i;
    s32 remainder;
    s32 selected;
    s32 modeOffset;
    s32 targetY;
    u8 nextItemCount;
    CharacterSelectMenuFrameActor *actor;
    CharacterSelectMenuFrameActor *actorCopy;

    actorCopy = arg0;

    if (D_80121B5E < 2) {
        modeOffset = 2;
    } else {
        modeOffset = 0;
    }

    switch (arg0->state) {
    case 0:
        movedCount = 0;
        i = 0;
        if (arg0->itemCount > 0) {
            actor = arg0;
            do {
                if (actor->x[0] < -0x7C) {
                    actor->x[0] += 0x10;
                    movedCount++;
                    if (actor->x[0] >= -0x7C) {
                        actor->x[0] = -0x7C;
                    }
                }
                i++;
                actor = (CharacterSelectMenuFrameActor *)((s16 *)actor + 1);
            } while (i < actorCopy->itemCount);
        }

        arg0->timer++;
        if ((arg0->timer & 1) == 0) {
            visibleCount = modeOffset + 2;
            if (arg0->itemCount < visibleCount) {
                nextItemCount = arg0->itemCount + 1;
                arg0->itemCount = nextItemCount;
                if (visibleCount == nextItemCount) {
                    D_8010ADE4 = func_80071408(func_8001DACC, 0, 0x59);
                    if (D_80121B5E < 2) {
                        func_80071408(func_8001DFA0, 0, 0x5A);
                        func_80071408(func_8001E468, 0, 0x5B);
                    }
                    func_80071408(func_80020AA0, 0, 0x5C);
                    func_80071408(func_80021E70, 0, 0x62);
                }
            }
        }

        if (movedCount == 0) {
            arg0->state = 1;
        }
        break;

    case 1:
        if (D_80121D88 == 7) {
            arg0->state = 7;
        }
        if (D_80121D88 == 1) {
            arg0->state = 2;
        }
        break;

    case 2:
        i = 0;
        if (arg0->itemCount > 0) {
            do {
                if (i != D_80121B50) {
                    arg0->x[i] -= 0x20;
                }
                i++;
            } while (i < actorCopy->itemCount);
        }

        if (D_80121B50 != 0) {
            if (arg0->x[0] < -0x103) {
                arg0->state = 3;
            }
        } else if (arg0->x[1] < -0x103) {
            arg0->state = 3;
        }
        break;

    case 3:
        arg0->y[D_80121B50] -= arg0->itemSpacing;
        if (arg0->y[D_80121B50] < -0x57) {
            arg0->y[D_80121B50] = -0x58;
            arg0->state = 4;
            func_80071408(func_800232F4, 0, 0x62);
            D_8010ADE0 = func_80071408(func_800235E0, 0, 0x63);
        }
        break;

    case 4:
        if (D_80121D88 == 4) {
            arg0->state = 5;
        }
        if (D_80121D88 == 7) {
            arg0->state = 10;
        }
        break;

    case 5:
        arg0->y[D_80121B50] += arg0->itemSpacing;
        targetY = arg0->baseY + (D_80121B50 * arg0->itemSpacing);
        if (arg0->y[D_80121B50] >= targetY) {
            arg0->y[D_80121B50] = targetY;
            arg0->state = 6;
        }
        break;

    case 6:
        state6Count = modeOffset + 2;
        i = 0;
        if (state6Count > 0) {
            remainder = state6Count & 3;
            if (remainder != 0) {
                do {
                    if (i != D_80121B50) {
                        arg0->x[i] += 0x20;
                    }
                    i++;
                } while (remainder != i);
                if (i == state6Count) {
                    goto state_6_done;
                }
            }
            do {
                selected = D_80121B50;
                if (i != selected) {
                    arg0->x[i] += 0x20;
                    selected = D_80121B50;
                }
                if (i + 1 != selected) {
                    arg0->x[i + 1] += 0x20;
                    selected = D_80121B50;
                }
                if (i + 2 != selected) {
                    arg0->x[i + 2] += 0x20;
                    selected = D_80121B50;
                }
                if (i + 3 != selected) {
                    arg0->x[i + 3] += 0x20;
                }
                i += 4;
            } while (i != state6Count);
        }
state_6_done:
        if (D_80121B50 != 0) {
            if (arg0->x[0] >= -0x7C) {
                arg0->state = 1;
            }
        } else if (arg0->x[1] >= -0x7C) {
            arg0->state = 1;
        }
        if (arg0->state == 1) {
            D_80121D88 = 5;
        }
        break;

    case 7:
        i = 0;
        actor = arg0;
        if (arg0->itemCount > 0) {
            do {
                i++;
                actor->x[0] -= 0x20;
                actor = (CharacterSelectMenuFrameActor *)((s16 *)actor + 1);
            } while (i < arg0->itemCount);
        }
        if (arg0->x[0] < -0x103) {
            arg0->state = 8;
        }
        break;

    case 8:
        arg0->state = 9;
        break;

    case 10:
        arg0->x[D_80121B50] -= 0x20;
        if (arg0->x[D_80121B50] < -0x103) {
            arg0->state = 8;
        }
        break;
    }

    if (arg0->state == 9) {
        func_800716E4((EffectTask *)arg0);
    } else {
        func_800483FC(&D_80124868, func_8001C96C, arg0);
    }
}
#endif

// func_8001D254 best match: 52.632% (nonmatchings/func_8001D254-786318006044585456/base_3.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001D254.s")

#ifdef NON_MATCHING
void func_8001D254(CharacterSelectMenuFrameActor *arg0) {
    s32 temp_t6;
    u8 temp_t7;
    s32 temp_t8;
    s32 temp_t9;
    s32 temp_t0;
    s32 temp_t3;
    s32 temp_t2;
    s32 temp_t4;
    s32 temp_t5;
    s32 temp_v1;

    temp_t6 = -0x58;
    temp_t7 = 0x18;
    arg0->baseY = temp_t6;
    temp_t8 = arg0->baseY;
    temp_t9 = arg0->baseY;
    arg0->itemSpacing = temp_t7;
    temp_t0 = temp_t7;
    temp_t3 = temp_t7;
    temp_t7 = temp_t7;
    arg0->y[0] = temp_t8;
    temp_t2 = arg0->baseY;
    temp_t6 = arg0->baseY;
    temp_t8 = temp_t7 << 2;
    temp_v1 = -0x104;
    temp_t0 = temp_t9 + temp_t0;
    temp_t8 -= temp_t7;
    temp_t4 = temp_t3 << 1;
    temp_t3 = 1;
    temp_t5 = temp_t2 + temp_t4;
    temp_t9 = temp_t6 + temp_t8;
    arg0->y[3] = temp_t9;
    arg0->y[2] = temp_t5;
    arg0->y[1] = temp_t0;
    arg0->x[1] = temp_v1;
    arg0->x[2] = temp_v1;
    arg0->x[3] = temp_v1;
    arg0->x[0] = temp_v1;
    arg0->timer = 0;
    arg0->itemCount = temp_t3;
    arg0->state = 0;
    func_80071824(arg0, func_8001CC10);
}
#endif

// func_8001D2F0 best match: 98.863% (nonmatchings/func_8001D2F0-3836525038718587862/base_7.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001D2F0.s")

#ifdef NON_MATCHING
void func_8001D2F0(CharacterSelectWidgetActor *arg0) {
    volatile s32 padTop[2];
    volatile u16 frameIndex;
    volatile s32 padAfter[1];
    CharacterSelectFrameTileMap *savedFrame;
    volatile s32 padBefore[2];
    s32 yOffset;
    register CharacterSelectFrameTileMap *tileMap;
    register s32 i;
    register s32 offset;
    register CharacterSelectWidgetActor *actor;

    actor = arg0;
    if ((D_800EC9C2 == 1) && (D_80121B5E == 2)) {
        do {
            frameIndex = 9;
        } while (0);
    } else {
 frameIndex = actor->sprite.unsignedIndex; } savedFrame = (tileMap = &D_800B5FC0[frameIndex]); i = (offset = 0); if (arg0 == 0) { } do { func_80011264((s16) (actor->x + ((i & 3) << 5)), (s16) (actor->y + ((i / 4) << 5)), func_80043040(D_80112130.textureHandle), tileMap->center[0], 0, 0x100); i += 1; tileMap = (CharacterSelectFrameTileMap *) (((u16 *) tileMap) + 1); } while (i < 0x10); tileMap = savedFrame; yOffset = offset; do { func_80011264((s16) (actor->x + 0x80), (s16) (actor->y + yOffset), func_80043040(D_80112130.textureHandle), tileMap->right[0], 0, 0x100); func_80011264((s16) (actor->x + offset), (s16) (actor->y + 0x80), func_80043040(D_80112130.textureHandle), tileMap->bottom[0], 0, 0x100); offset += 0x40; tileMap = (CharacterSelectFrameTileMap *) (((u16 *) tileMap) + 1); } while (offset < 0x80); func_80011264((s16) (actor->x + 0x80), (s16) (actor->y + 0x80), func_80043040(D_80112130.textureHandle), D_800B5FE8[frameIndex].corner, 0, 0x100);

    func_8000F030((s16)(actor->x - 4), (s16)(actor->y - 4), func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), 0x33,
                  0x20, 0x20, 0, 0);
    func_8000F030((s16)(actor->x - 4), (s16)(actor->y + 0x8C), func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                  0x38, 0x20, 0x20, 0, 0);
    func_8000F030((s16)(actor->x + 0x8C), (s16)(actor->y - 4), func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                  0x35, 0x20, 0x20, 0, 0);
    func_8000F030((s16)(actor->x + 0x8C), (s16)(actor->y + 0x8C), func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                  0x3A, 0x20, 0x20, 0, 0);

    offset = 0;
    do {
        func_8000F030((s16)(actor->x + offset + 0xC), (s16)(actor->y - 4),
                      func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), 0x34, 0x20, 0x20, 0, 0);
        func_8000F030((s16)(actor->x + offset + 0xC), (s16)(actor->y + 0x8C),
                      func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), 0x39, 0x20, 0x20, 0, 0);
        func_8000F030((s16)(actor->x - 4), (s16)(actor->y + offset + 0xC),
                      func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), 0x36, 0x20, 0x20, 0, 0);
        func_8000F030((s16)(actor->x + 0x8C), (s16)(actor->y + offset + 0xC),
                      func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE), 0x37, 0x20, 0x20, 0, 0);
        offset += 0x10;
    } while (offset != 0x80);
}
#endif

// func_8001D7B8 best match: 91.995%
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_8001D7B8.s")

#ifdef NON_MATCHING
void func_8001D7B8(CharacterSelectWidgetActor *arg0) {
    int state;
    int stateValue;
    u16 selectedSpriteIndex;
    s32 selectedIndex;
    u16 spriteIndex;

    selectedIndex = D_80121B50;
    spriteIndex = (u16)arg0->sprite.index;
    selectedSpriteIndex = spriteIndex;
    if ((selectedIndex >= (s32)selectedSpriteIndex) && (arg0->transition.bytes.state != 0) && (arg0->y != -0x48)) {
        stateValue = 2;
        state = stateValue & 0xFF;
        arg0->transition.bytes.state = stateValue;
    } else {
        state = arg0->transition.bytes.state;
        if ((selectedIndex < (s32)selectedSpriteIndex) && (state != 0) && (arg0->y != -0x140)) {
            stateValue = 1;
            state = stateValue & 0xFF;
            arg0->transition.bytes.state = stateValue;
        } else if ((state != 0) && (state < 4)) {
            stateValue = 3;
            state = stateValue & 0xFF;
            arg0->transition.bytes.state = stateValue;
        }
    }

    if (state != D_8010AE8A) {
        state = D_8010AE8A & 0xFF;
        arg0->transition.bytes.state = D_8010AE8A;
    }

    switch (state) {
    case 0:
        arg0->x -= 0x26;
        if (arg0->x < -7) {
            arg0->x = -8;
            arg0->transition.bytes.state = 3;
            D_8010ADDC = (s32)func_80071408(func_80020CEC, 0, 0x64);
            func_80071408(func_80020F44, 0, 0x62);
            func_80071408(func_800213D4, 0, 0x62);
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        state = D_80121D80[8];
        if (state == 1) {
            arg0->transition.bytes.state = 4;
            state = D_80121D88;
        }
        if (state == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        state = D_80121D80[8];
        if (state == 5) {
            arg0->transition.bytes.state = 3;
            D_80121D80[8] = 6;
            state = 6 & 0xFF;
        }
        if (state == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        if (arg0->x >= 0x94) {
            arg0->transition.bytes.state = 6;
        }
        state = arg0->transition.bytes.state;
        break;
    case 6:
        arg0->transition.bytes.state = 7;
        D_80121D80[8] = 8;
        state = arg0->transition.bytes.state;
        break;
    }

    D_8010AE8A = state;
    if (arg0->transition.bytes.state == 7) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001D2F0, arg0);
}
#endif

void func_8001DACC(CharacterSelectWidgetActor *arg0) {
    arg0->x = 0x96;
    arg0->y = -0x48;
    arg0->sprite.index = 0;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001D7B8);
}

void func_8001DB0C(CharacterSelectWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        func_800112F4((s16)(arg0->x + ((i & 3) << 5)), (s16)(arg0->y + ((i / 4) << 5)),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        func_800112F4((s16)(arg0->x + 0x80), (s16)(arg0->y + offset),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        func_800112F4((s16)(arg0->x + offset), (s16)(arg0->y + 0x80),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    func_800112F4((s16)(arg0->x + 0x80), (s16)(arg0->y + 0x80), func_80043040(D_80112130.textureHandle),
                  D_800B5FC0[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void func_8001DD80(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((D_80121B50 >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B50 < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    case 6:
        state = arg0->transition.bytes.state = 7;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001DB0C, arg0);
}

void func_8001DFA0(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x1;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001DD80);
}

void func_8001DFE4(CharacterSelectWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        func_800112F4(arg0->x + ((i & 3) << 5), arg0->y + ((i / 4) << 5), func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        func_800112F4(arg0->x + 0x80, arg0->y + offset, func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        func_800112F4(arg0->x + offset, arg0->y + 0x80, func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    func_800112F4(arg0->x + 0x80, arg0->y + 0x80, func_80043040(D_80112130.textureHandle),
                  D_800B5FC0[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void func_8001E258(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((D_80121B50 >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B50 < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001DFE4, arg0);
}

void func_8001E468(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x2;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001E258);
}

void func_8001E4AC(CharacterSelectWidgetActor *arg0) {
    s32 i;
    s32 tileOffset;

    tileOffset = 0;
    for (i = 0; i < 16; i++, tileOffset++) {
        func_800112F4(arg0->x + ((i & 3) << 5), arg0->y + ((i / 4) << 5), func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    tileOffset = 0;
    for (i = 0; i < 2; i++) {
        func_800112F4(arg0->x + 0x80, arg0->y + i * 0x40, func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        func_800112F4(arg0->x + i * 0x40, arg0->y + 0x80, func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        tileOffset++;
    }

    func_800112F4(arg0->x + 0x80, arg0->y + 0x80, func_80043040(D_80112130.textureHandle),
                  D_800B5FC0[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void func_8001E720(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((D_80121B50 >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B50 < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001E4AC, arg0);
}

void func_8001E930(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x3;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001E720);
}

void func_8001E974(CharacterSelectWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        func_800112F4((s16)(arg0->x + ((i & 3) << 5)), (s16)(arg0->y + ((i / 4) << 5)),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        func_800112F4((s16)(arg0->x + 0x80), (s16)(arg0->y + offset),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        func_800112F4((s16)(arg0->x + offset), (s16)(arg0->y + 0x80),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    func_800112F4((s16)(arg0->x + 0x80), (s16)(arg0->y + 0x80), func_80043040(D_80112130.textureHandle),
                  D_800B5FC0[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void func_8001EBE8(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((D_80121B50 >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B50 < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001E974, arg0);
}

void func_8001EDF8(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x4;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001EBE8);
}

void func_8001EE3C(CharacterSelectWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        func_800112F4(arg0->x + ((i & 3) << 5), arg0->y + ((i / 4) << 5), func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        func_800112F4(arg0->x + 0x80, arg0->y + offset, func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        func_800112F4(arg0->x + offset, arg0->y + 0x80, func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    func_800112F4(arg0->x + 0x80, arg0->y + 0x80, func_80043040(D_80112130.textureHandle),
                  D_800B5FC0[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void func_8001F0B0(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((D_80121B50 >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B50 < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001EE3C, arg0);
}

void func_8001F2C0(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x5;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001F0B0);
}

void func_8001F304(CharacterSelectWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        func_800112F4((s16)(arg0->x + ((i & 3) << 5)), (s16)(arg0->y + ((i / 4) << 5)),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        func_800112F4((s16)(arg0->x + 0x80), (s16)(arg0->y + offset),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        func_800112F4((s16)(arg0->x + offset), (s16)(arg0->y + 0x80),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    func_800112F4((s16)(arg0->x + 0x80), (s16)(arg0->y + 0x80), func_80043040(D_80112130.textureHandle),
                  D_800B5FC0[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void func_8001F578(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((D_80121B50 >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B50 < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001F304, arg0);
}

void func_8001F788(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x6;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001F578);
}

void func_8001F7CC(CharacterSelectWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        func_800112F4(arg0->x + ((i & 3) << 5), arg0->y + ((i / 4) << 5), func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        func_800112F4(arg0->x + 0x80, arg0->y + offset, func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        func_800112F4(arg0->x + offset, arg0->y + 0x80, func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    func_800112F4(arg0->x + 0x80, arg0->y + 0x80, func_80043040(D_80112130.textureHandle),
                  D_800B5FC0[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void func_8001FA40(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((D_80121B50 >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B50 < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001F7CC, arg0);
}

void func_8001FC50(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x7;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001FA40);
}

void func_8001FC94(CharacterSelectWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileIndex;
    s32 offset;

    tileIndex = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileIndex++) {
        func_800112F4((s16)(arg0->x + ((i & 3) << 5)), (s16)(arg0->y + ((i / 4) << 5)),
                      func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].center[tileIndex], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileIndex = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        func_800112F4((s16)(arg0->x + 0x80), (s16)(arg0->y + offset), func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].right[tileIndex], 0, 0x100, 0xA0, 0x49);
        func_800112F4((s16)(arg0->x + offset), (s16)(arg0->y + 0x80), func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].bottom[tileIndex], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileIndex++;
    } while (offset != i);
    i++;
    i--;

    func_800112F4((s16)(arg0->x + 0x80), (s16)(arg0->y + 0x80), func_80043040(D_80112130.textureHandle),
                  D_800B5FC0[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void func_8001FF08(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((D_80121B50 >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B50 < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8001FC94, arg0);
}

void func_80020118(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x8;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8001FF08);
}

void func_8002015C(CharacterSelectWidgetActor *arg0) {
    s32 shouldDraw;
    s32 i;
    s32 tileOffset;
    register s32 offset;

    tileOffset = 0;
    shouldDraw = 1;
    for (i = 0; i < 16; i++, tileOffset++) {
        func_800112F4(arg0->x + ((i & 3) << 5), arg0->y + ((i / 4) << 5), func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    if (shouldDraw) {
        tileOffset = 0;
        i = 0x80;
    }
    offset = 0;
    do {
        func_800112F4(arg0->x + 0x80, arg0->y + offset, func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        func_800112F4(arg0->x + offset, arg0->y + 0x80, func_80043040(D_80112130.textureHandle),
                      D_800B5FC0[(u16)arg0->sprite.index].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);
    i++;
    i--;

    func_800112F4(arg0->x + 0x80, arg0->y + 0x80, func_80043040(D_80112130.textureHandle),
                  D_800B5FC0[(u16)arg0->sprite.index].corner, 0, 0x100, 0xA0, 0x49);
}

void func_800203D0(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->transition.bytes.state;

    if ((D_80121B50 >= (s32)(u16) arg0->sprite.index) && (arg0->y != -0x48)) {
        state = arg0->transition.bytes.state = 2;
    } else if ((D_80121B50 < (s32)(u16) arg0->sprite.index) && (arg0->y != -0x140)) {
        state = arg0->transition.bytes.state = 1;
    } else {
        state = *stateField;
        if (state < 4) {
            state = arg0->transition.bytes.state = 3;
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 4;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8002015C, arg0);
}

void func_800205E0(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0x9;
    arg0->transition.bytes.timer = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_800203D0);
}

void func_80020624(CharacterSelectWidgetActor *arg0) {
    s32 i;
    s32 tileOffset;
    s32 offset;

    tileOffset = 0;
    for (i = 0; i < 16; i++, tileOffset++) {
        func_800112F4(arg0->x + ((i & 3) << 5), arg0->y + ((i / 4) << 5), func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                      D_800B5FC0[10].center[tileOffset], 0, 0x100, 0xA0, 0x49);
    }

    tileOffset = 0; offset = 0; i = 0x80; do {
        func_800112F4(arg0->x + 0x80, arg0->y + offset, func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                      D_800B5FC0[10].right[tileOffset], 0, 0x100, 0xA0, 0x49);
        func_800112F4(arg0->x + offset, arg0->y + 0x80, func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                      D_800B5FC0[10].bottom[tileOffset], 0, 0x100, 0xA0, 0x49);
        i = 0x80;
        offset += 0x40;
        tileOffset++;
    } while (offset != i);

    i++;
    i--;

    func_800112F4(arg0->x + 0x80, arg0->y + 0x80, func_80043040(CHARACTER_SELECT_FRAME_TEXTURE_HANDLE),
                  D_800B618C, 0, 0x100, 0xA0, 0x49);
}

void func_80020818(CharacterSelectWidgetActor *arg0) {
    int state;
    u8 *stateField = &arg0->row.bytes.subState;

    if ((D_80121B50 == D_8010AE80) && (arg0->y != -0x48) && ((s32) arg0->row.bytes.subState < 6)) {
        state = arg0->row.bytes.subState = 2;
    } else {
        state = (s32) arg0->row.bytes.subState;
        if ((D_80121B50 != D_8010AE80) && (arg0->y != -0x140) && (state < 6)) {
            state = arg0->row.bytes.subState = 1;
        } else {
            state = arg0->row.bytes.subState;
            if (state < 4) {
                state = arg0->row.bytes.subState = 3;
            }
        }
    }

    switch (state) {
    case 0:
        break;
    case 1:
        arg0->y -= 0x24;
        if (arg0->y < -0x13F) {
            arg0->y = -0x140;
            arg0->row.bytes.subState = 3;
        }
        state = arg0->row.bytes.subState;
        break;
    case 2:
        arg0->y += 0x24;
        if (arg0->y >= -0x48) {
            arg0->y = -0x48;
            arg0->row.bytes.subState = 6;
            arg0->transition.alpha = 8;
        }
        state = arg0->row.bytes.subState;
        break;
    case 3:
        D_801235B4 += 1;
        if (D_80121D88 == 1) {
            arg0->row.bytes.subState = 4;
        }
        if (D_80121D88 == 7) {
            arg0->row.bytes.subState = 5;
        }
        state = arg0->row.bytes.subState;
        break;
    case 4:
        if (D_80121D88 == 5) {
            arg0->row.bytes.subState = 3;
        }
        if (D_80121D88 == 7) {
            arg0->row.bytes.subState = 5;
        }
        state = arg0->row.bytes.subState;
        break;
    case 5:
        arg0->x += 0x20;
        state = *stateField;
        break;
    case 6:
        arg0->y -= arg0->transition.alpha;
        state = arg0->row.bytes.subState = 7;
        break;
    case 7:
        arg0->y += arg0->transition.alpha;
        arg0->transition.alpha /= 2;
        if (arg0->transition.alpha == 0) {
            arg0->row.bytes.subState = 3;
        } else {
            arg0->row.bytes.subState = 6;
        }
        state = arg0->row.bytes.subState;
        break;
    }

    if ((state == 5) && (arg0->x >= 0x94)) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_80020624, arg0);
}

void func_80020AA0(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x140;
    arg0->sprite.index = 0;
    arg0->row.bytes.subTimer = 0;
    arg0->row.bytes.subState = 0;
    func_80071824(arg0, func_80020818);
}

void func_80020AE0(CharacterSelectWidgetActor *arg0) {
    if (arg0->transition.bytes.state < 3) {
        func_8000F8AC(
            arg0->x,
            (s16)(arg0->y + D_80121B50 * arg0->selection.bytes.selectedCharacterRow),
            func_80043040(D_80112172),
            0x12,
            0x20,
            0x20,
            0,
            arg0->sprite.index,
            0);
    }
}

void func_80020B70(CharacterSelectWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = D_8010AE88.fields.state)) {
        arg0->transition.bytes.state = globalState;
        state = globalState;
        arg0->sprite.index = D_8010AE88.fields.spriteIndex;
        arg0->transition.bytes.timer = D_8010AE88.fields.timer;
    }

    /* Preserve IDO's state selector register allocation. */
    switch (state ^ 0) {
    case 0:
        arg0->sprite.index += 0x26;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 1;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        if ((s32) arg0->transition.bytes.timer < 0x10) {
            arg0->sprite.index -= 9;
        } else {
            arg0->sprite.index += 9;
        }
        state = arg0->transition.bytes.state;
        arg0->transition.bytes.timer = (arg0->transition.bytes.timer + 1) & 0x1F;
        break;
    case 2:
        if (D_80121D88 == 1) {
            state = arg0->transition.bytes.state = 3;
        }
        break;
    case 3:
        break;
    }

    D_8010AE88_state = state;
    if (D_80121D88 == 7) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_80020AE0, arg0);
}

void func_80020CEC(CharacterSelectWidgetActor *arg0) {
    u8 selectedCharacterRow;
    s16 y;
    int mode;

    mode = D_8010ADF9;
    if (mode == 0) {
        if (1) {}
        selectedCharacterRow = 0x18;
        y = -0x50;
    } else if (mode == 1) {
        y = -0x54;
        selectedCharacterRow = 0x16;
    } else if (mode == 2) {
        selectedCharacterRow = 0x16;
        y = -0x60;
    } else {
        y = -0x60;
        selectedCharacterRow = 0x13;
    }

    if (D_800EC9DD != 0) {
        y = -0x58;
        selectedCharacterRow = 0x18;
    }

    arg0->x = -0x7C;
    arg0->y = y;
    arg0->selection.bytes.selectedCharacterRow = selectedCharacterRow;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    arg0->transition.bytes.timer = 0;
    func_80071824(arg0, func_80020B70);
}

void func_80020D88(CharacterSelectWidgetActor *arg0) {
    func_8000F8AC(arg0->x, arg0->y, func_80043040(D_80112172), 1, 0x20, 0x20, 0, arg0->sprite.index, 0);
}

void func_80020DEC(CharacterSelectWidgetActor *arg0) {
    u8 state = arg0->transition.bytes.state;

    switch (state) {
    case 0:
        arg0->sprite.index += 0x26;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 1;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 2;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 1;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        arg0->x += 0x20;
        if (arg0->x >= 0x94) {
            arg0->transition.bytes.state = 4;
        }
        state = arg0->transition.bytes.state;
        break;
    }
    if ((unsigned int)state == 4) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_80020D88, arg0);
}

void func_80020F44(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x8;
    arg0->y = -0x5C;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_80020DEC);
}

const char D_800E0B80[] = "%4d";

void func_80020F80(CharacterSelectWidgetActor *arg0) {
    char pad[8];
    char buf[4];
    s32 temp_v0;
    s32 xOffset;
    s32 i;
    s32 var_t8;

    if (D_80121B50 != D_8010AE80) {
        func_8001BA2C(arg0->x, arg0->y, 0x3800, 0x5800);
        func_8000F8AC((s16)(arg0->x + 4), (s16)(arg0->y + 4), func_80043040(D_80112130.textureHandle),
                      0x23, 0x20, 0x20, 0, arg0->sprite.index, 0);
        sprintf(buf - 4, D_800E0B80, D_800B61AC[D_80121B50]);
        func_80013D0C((s16)(arg0->x + 8), (s16)(arg0->y + 0xC), buf - 4, 0, arg0->sprite.index);
        func_8000F8AC((s16)(arg0->x + 0x28), (s16)(arg0->y + 0xC), func_80043040(D_80112130.textureHandle),
                      0x24, 0x20, 0x20, 0, arg0->sprite.index, 0);
        func_8000F8AC(arg0->x, (s16)(arg0->y + 0x14), func_80043040(D_80112130.textureHandle), 0x25, 0x20,
                      0x20, 0, arg0->sprite.index, 0);

        temp_v0 = D_800B61C0[D_80121B50];
        xOffset = 0;
        i = 0;
        var_t8 = temp_v0 & 1;
        if ((temp_v0 / 2) > 0) {
            do {
                func_8000F8AC((s16)(arg0->x + xOffset + 4), (s16)(arg0->y + 0x1C),
                              func_80043040(D_80112130.iconTextureHandle), 0x25, 0x20, 0x20, 0,
                              arg0->sprite.index, 0);
                i++;
                temp_v0 = D_800B61C0[D_80121B50];
                xOffset += 0xC;
            } while (i < (temp_v0 / 2));
        }
        var_t8 = temp_v0 & 1;
        if (var_t8 != 0) {
            func_8000F8AC((s16)(arg0->x + xOffset + 4), (s16)(arg0->y + 0x1C),
                          func_80043040(D_80112130.iconTextureHandle), 0x26, 0x20, 0x20, 0, arg0->sprite.index,
                          0);
        }
    }
}

void func_8002127C(CharacterSelectWidgetActor *arg0) {
    u8 state = arg0->transition.bytes.state;

    switch (state) {
    case 0:
        arg0->sprite.index += 0x26;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 1;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        if (D_80121D88 == 1) {
            arg0->transition.bytes.state = 2;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        if (D_80121D88 == 5) {
            arg0->transition.bytes.state = 1;
        }
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 3;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        arg0->x += 0x20;
        if (arg0->x >= 0x94) {
            arg0->transition.bytes.state = 4;
        }
        state = arg0->transition.bytes.state;
        break;
    }
    if ((unsigned int)state == 4) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_80020F80, arg0);
}

void func_800213D4(CharacterSelectWidgetActor *arg0) {
    arg0->x = 0x48;
    arg0->y = 0x34;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_8002127C);
}

const char D_800E0B84[] = "%5d";
const char D_800E0B88[] = "%5d";
const char D_800E0B8C[] = "%2d";

void func_80021410(CharacterSelectWidgetActor *arg0) {
    char buf[4];
    s16 *characterIds;
    s32 yOffset;
    s32 valueOffset;
    s32 three;
    CharacterSelectPlayerRecord *player;
    do { characterIds = D_800B3420[D_8010ADF9]; if (D_80121B55 == 1) { func_8000F030(arg0->x, arg0->y, func_80043040(D_80112130.textureHandle), 0x21, 0x20, 0x20, 0, 0); func_8000F030((s16)(arg0->x + 0x30), arg0->y, func_80043040(D_80112130.textureHandle), 0x22, 0x20, 0x20, 0, 0); sprintf(buf - 0x10, D_800E0B84, D_800EC9F8[characterIds[*(&D_80121B50)]]); func_80013D0C((s16)(arg0->x + 0x14), (s16)(arg0->y + 0x2A), buf - 0x10, 0, 0x100); valueOffset = 0; yOffset = 0; three = 3; do { sprintf(buf - 0x10, D_800E0B88, *((u16 *)(&D_800B61CC[(((*(&D_80121B50)) * three) * 2) + valueOffset]))); func_80013D0C((s16)(arg0->x + 0x28), (s16)((arg0->y + yOffset) + 9), buf - 0x10, 0, 0x100); yOffset += 8; valueOffset += 2; } while (yOffset != 0x18); } else { func_8000F030(arg0->x, arg0->y, func_80043040(D_80112130.textureHandle), 0x26, 0x20, 0x20, 0, 0); func_8000F030(arg0->x, (s16)(arg0->y + 0x10), func_80043040(D_80112130.textureHandle), 0x27, 0x20, 0x20, 0, 0); player = (CharacterSelectPlayerRecord *)D_80121D80; yOffset = 0; do { sprintf(buf - 0x10, D_800E0B8C, player->value); func_80013D0C((s16)(arg0->x + 0x40), (s16)((arg0->y + yOffset) + 0x10), buf - 0x10, 0, 0x100); player++; yOffset += 8; } while (player != &D_801235B0); } } while (0);
}

void func_8002172C(CharacterSelectWidgetActor *arg0) {
    u8 state = arg0->sprite.bytes.state;
    s16 target;

    switch (state) {
    case 0:
        if (D_80121B55 == 1) {
            target = -0x78;
        } else {
            target = -0x80;
        }
        arg0->x += 0x20;
        if (arg0->x >= target) {
            arg0->x = target;
            arg0->sprite.bytes.state = 1;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 1:
        if ((D_80121D88 == 3) || (D_80121D88 == 7)) {
            state = arg0->sprite.bytes.state = 2;
        }
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0x107) {
            arg0->sprite.bytes.state = 3;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 3:
        break;
    }
    if ((unsigned int)state == 3) {
        func_800716E4(arg0);
        if (D_80121D80[8] == 3) {
            D_80121D80[8] = 4;
        }
    } else {
        func_800483FC(&D_80124868, func_80021410, arg0);
    }
}

void func_8002186C(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = -0x50;
    arg0->sprite.bytes.state = 0;
    func_80071824(arg0, func_8002172C);
}

void func_800218A4(CharacterSelectWidgetActor *arg0) {
    func_8000F030(arg0->x, arg0->y, func_80043040(D_80112172), 3, 0x20, 0x20, 0, 0);
    func_8000F030((s16) (arg0->x + 0x40), arg0->y, func_80043040(D_80112172), 4, 0x20, 0x20, 0, 0);
    func_8000F030(arg0->x, (s16) (arg0->y + 0x40), func_80043040(D_80112172), 5, 0x20, 0x20, 0, 0);
    func_8000F030((s16) (arg0->x + 0x40), (s16) (arg0->y + 0x40), func_80043040(D_80112172), 6, 0x20,
                  0x20, 0, 0);
}

void func_800219E4(CharacterSelectWidgetActor *arg0) {
    u8 state = arg0->sprite.bytes.state;

    switch (state) {
    case 0:
        arg0->x += 0x20;
        if (arg0->x >= -0x88) {
            arg0->x = -0x88;
            arg0->sprite.bytes.state = 1;
            D_8010ADE0 = func_80071408(func_8002215C, 0, 0x63);
        }
        state = arg0->sprite.bytes.state;
        break;
    case 1:
        if ((D_80121D88 == 3) || (D_80121D88 == 7)) {
            state = arg0->sprite.bytes.state = 2;
        }
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0x107) {
            arg0->sprite.bytes.state = 3;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 3:
        state = arg0->sprite.bytes.state = 4;
        break;
    }
    state = arg0->sprite.bytes.state;
    if (state == 4) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_800218A4, arg0);
}

void func_80021B20(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = -0x18;
    arg0->sprite.bytes.state = 0;
    func_80071824(arg0, func_800219E4);
}

void func_80021B58(CharacterSelectWidgetActor *arg0) {
    func_8000F030(arg0->x, arg0->y, func_80043040(D_80112172), 3, 0x20, 0x20, 0, 0);
    func_8000F030((s16) (arg0->x + 0x40), arg0->y, func_80043040(D_80112172), 4, 0x20, 0x20, 0, 0);
    func_8000F030(arg0->x, (s16) (arg0->y + 0x40), func_80043040(D_80112172), 5, 0x20, 0x20, 0, 0);
    func_8000F030((s16) (arg0->x + 0x40), (s16) (arg0->y + 0x40), func_80043040(D_80112172), 6, 0x20, 0x20, 0, 0);
}

void func_80021C98(CharacterSelectWidgetActor *arg0) {
    u32 state = arg0->sprite.bytes.state;

    switch (state) {
    case 0:
        arg0->x += 0x20;
        if (arg0->x >= -0x88) {
            arg0->x = -0x88;
            arg0->sprite.bytes.state = 1;
            func_80071408(func_80022464, 0, 0x63);
        }
        state = arg0->sprite.bytes.state;
        break;
    case 1:
        if (D_80121D88 == 7) {
            arg0->sprite.bytes.state = 5;
        }
        if (D_80121D88 == 1) {
            arg0->sprite.bytes.state = 2;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0x127) {
            arg0->sprite.bytes.state = 3;
            arg0->x = -0x128;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 3:
        if (D_80121D88 == 5) {
            state = arg0->sprite.bytes.state = 4;
        }
        break;
    case 4:
        arg0->x += 0x20;
        if (arg0->x >= -0x88) {
            arg0->x = -0x88;
            arg0->sprite.bytes.state = 1;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 5:
        arg0->x -= 0x20;
        if (arg0->x < -0x107) {
            arg0->sprite.bytes.state = 6;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 6:
        state = arg0->sprite.bytes.state = 7;
        break;
    }
    if (state == 7) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_80021B58, arg0);
}

void func_80021E70(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x128;
    arg0->y = 0x8;
    arg0->sprite.bytes.state = 0;
    func_80071824(arg0, func_80021C98);
}

void func_80021EA8(CharacterSelectWidgetActor *arg0) {
    CharacterSelectText *texts;
    u8 *text;

    texts = D_800B6210;
    text = texts[D_80121B50];
    func_80013154(arg0->x, arg0->y, text, 1, arg0->sprite.index, 0);
    if (arg0->transition.bytes.state == 2) {
        func_8000F030(
            (s16)(arg0->x + 0x60),
            (s16)(arg0->y + 0x40),
            func_80043040(D_80112178),
            (((s32)arg0->transition.bytes.timer >= 8) + 5) & 0xFFFF,
            0x20,
            0x20,
            0,
            0);
    }
}

void func_80021F80(CharacterSelectWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = D_8010AE88.bytes[1])) {
        arg0->transition.bytes.state = globalState;
        state = globalState;
        arg0->transition.bytes.timer = D_8010AE88.bytes[7];
    }

    switch (state) {
    case 0:
        if ((++arg0->transition.bytes.timer) == 2) {
            arg0->transition.bytes.state = 1;
            arg0->transition.bytes.timer = state * 0;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        arg0->sprite.index += 0x30;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 2;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->transition.bytes.timer = (arg0->transition.bytes.timer + 1) & 0xF;
        if (D_80121D88 == 3) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        state = arg0->transition.bytes.state;
        arg0->transition.bytes.timer = 0;
        break;
    case 4:
        arg0->transition.bytes.timer++;
        if ((D_80121D88 == 3) || (D_80121D88 == 7)) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x -= 0x20;
        if (arg0->x < -0xFF) {
            arg0->transition.bytes.state = 6;
        }
        state = arg0->transition.bytes.state;
        break;
    case 6:
        break;
    }

    D_8010AE89 = state;
    D_8010AE8F = arg0->transition.bytes.timer;

    if (arg0->transition.bytes.state == 6) {
        func_800716E4(arg0);
        D_8010ADE0 = NULL;
        D_8010AE88.bytes[1] = 0;
        D_8010AE88.bytes[7] = 0;
        return;
    }
    func_800483FC(&D_80124868, func_80021EA8, arg0);
}

void func_8002215C(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = -0x14;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_80021F80);
}

void func_80022198(CharacterSelectWidgetActor *arg0) {
    u8 *text;
    s32 threshold;
    u16 idx;

    if (D_80121B5E < 2) {
        threshold = 2;
    } else {
        threshold = 0;
    }
    if (threshold >= D_80121B50) {
        idx = (u16)D_80121B50;
        if (D_80121B5E == 0) {
            text = D_800B67D8[idx];
        } else if (D_80121B5E == 1) {
            text = D_800B6934[idx];
        } else {
            text = D_800B6A54[idx];
        }
    } else {
        text = D_800B6B34;
    }
    func_80013154(arg0->x, arg0->y, text, 1, arg0->sprite.index, 0);
}

void func_80022274(CharacterSelectWidgetActor *arg0) {
    u32 state = arg0->transition.bytes.state;

    switch (state) {
    case 0:
        if (++arg0->transition.bytes.timer == 2) {
            arg0->transition.bytes.state = 1;
            arg0->transition.bytes.timer = 0;
        }
        state = arg0->transition.bytes.state;
        break;
    case 1:
        arg0->sprite.index += 0x30;
        if (arg0->sprite.index >= 0x100) {
            arg0->sprite.index = 0x100;
            arg0->transition.bytes.state = 2;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        if ((D_80121D88 == 7) || (D_80121D88 == 1)) {
            state = arg0->transition.bytes.state = 3;
        }
        break;
    case 3:
        arg0->x -= 0x20;
        if (arg0->x < -0x11F) {
            if (D_80121D88 == 7) {
                arg0->transition.bytes.state = 7;
            } else {
                arg0->transition.bytes.state = 4;
            }
        }
        state = arg0->transition.bytes.state;
        break;
    case 4:
        if (D_80121D88 == 5) {
            state = arg0->transition.bytes.state = 5;
        }
        break;
    case 5:
        arg0->x += 0x20;
        if (arg0->x >= -0x84) {
            arg0->transition.bytes.state = 2;
        }
        state = arg0->transition.bytes.state;
        break;
    case 6:
        if (D_80121D88 == 7) {
            arg0->transition.bytes.state = 3;
        }
        if (D_80121D88 == 0) {
            arg0->transition.bytes.state = 2;
        }
        state = arg0->transition.bytes.state;
        break;
    case 7:
        state = arg0->transition.bytes.state = 8;
        break;
    }
    if (state == 8) {
        func_800716E4(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_80022198, arg0);
}

void func_80022464(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x84;
    arg0->y = 0xC;
    arg0->sprite.index = 0;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_80022274);
}

void func_800224A0(CharacterSelectWidgetActor *arg0) {
    u32 drawAlpha;
    int isEvenState;
    u16 alpha;
    s16 state;

    state = D_800EC9D0;
    if (state != 0) {
        isEvenState = !(state & 1);
        if (isEvenState) {
            alpha = 0x100;
        } else {
            alpha = 0x60;
        }

        drawAlpha = alpha;
        func_8000F8AC(arg0->x, arg0->sprite.index, func_80043040(D_80112178), 0x18, 0x20, 0x20, 0, drawAlpha, 0);

        if (drawAlpha == 0x100) {
            alpha = 0x60;
        } else {
            alpha = 0x100;
        }

        func_8000F8AC(arg0->x, arg0->y, func_80043040(D_80112178), 0x17, 0x20, 0x20, 0, alpha, 0);

        state = D_800EC9D0;
        if ((state == 3) || (state == 4)) {
            func_8000F8AC(arg0->x, (s16)(((D_800EC9D0 * 0x10) + arg0->y) - 0x30), func_80043040(D_80112178), 0x12,
                          0x20, 0x20, 0, (u16)arg0->selection.counter, 0);
            state = D_800EC9D0;
        }
    }
    if ((state >= 5) && (((s16)arg0->transition.alpha) == 0)) {
        func_800716E4(arg0);
    }
}

void func_8002262C(CharacterSelectWidgetActor *arg0) {
    s32 step;

    if ((D_800EC9D0 != 0) && (D_800EC9D0 != 3) && (D_800EC9D0 != 4)) {
        step = ((D_800EC9D0 < 5) ? 1 : -1) * 8;
        arg0->transition.signedAlpha += step;

        if (arg0->transition.signedAlpha == 0x20) {
            D_800EC9D0 += 2;
            arg0->row.value = 0;
            arg0->selection.counter = 0x100;
        }

        if (step == 8) {
            if (arg0->transition.signedAlpha < 0x18) {
                arg0->y += 8;
            }
            arg0->sprite.index += 8;
        } else {
            if (arg0->transition.signedAlpha < 0x10) {
                arg0->y -= 8;
            }
            arg0->sprite.index -= 8;
        }
    }

    if ((D_800EC9D0 == 3) || (D_800EC9D0 == 4)) {
        if ((u16)arg0->row.value < 0x10) {
            arg0->selection.counter = (u16)arg0->selection.counter - 9;
        } else {
            arg0->selection.counter = (u16)arg0->selection.counter + 9;
        }
        arg0->row.value = ((u16)arg0->row.value + 1) & 0x1F;
    }

    func_800483FC(&D_80124868, func_800224A0, arg0);
}

void func_800227A0(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x74;
    arg0->y = 0x2F;
    arg0->sprite.index = 0x2F;
    func_80071824(arg0, func_8002262C);
}

// func_800227D8 best match: 90.478% (nonmatchings/func_800227D8-2225551288923588688/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_800227D8.s")

#ifdef NON_MATCHING
typedef struct {
    /* 0x0000 */ u8 pad0[0x4E];
    /* 0x004E */ u8 courseStatsNames[10][5][4];
} CharacterSelectCourseStatsNameData;

typedef struct {
    /* 0x0000 */ u8 pad0[0x156];
    /* 0x0156 */ u8 resultNames[10][5][4];
} CharacterSelectResultNameData;

typedef struct {
    /* 0x0000 */ u8 pad0[0x7756];
    /* 0x7756 */ u16 trickValues[10][5];
} CharacterSelectStatsData;

extern u8 D_800EC9F0[];
extern u8 D_800F41EB[];
extern u8 D_800F4222[];
extern u8 D_800F4259[];
extern u8 D_800F4290[];

void func_800227D8(CharacterSelectWidgetActor *arg0) {
    volatile u8 padding[0x40];
    s16 *courseIds;
    u16 sp72;
    u16 sp70;
    volatile u8 color;
    u8 *trickIcon;
    s16 *selectedCoursePtr;
    s32 quotient;
    s32 var_s1;
    s32 var_s4;
    s32 var_s4_2;
    s32 var_s5;
    s32 var_s7;
    u16 var_s0;

    if (D_800EC9C2 == 2) {
        courseIds = D_800B3420[D_8010ADF9];
    } else if ((s32)D_80121B5E < 2) {
        courseIds = D_800B3478;
    } else {
        courseIds = D_800B3480;
    }

    var_s7 = 0;
    var_s5 = 0;
    do {
        if (var_s7 < 3) {
            color = 6;
        } else {
            color = 4;
        }

        func_8000F030(arg0->x, (s16)(arg0->y + var_s5), func_80043040(D_80112130.popupFontHandle),
                      (var_s7 + 0x77) & 0xFFFF, 0x20, 0x20, 0, 0);

        if (D_80121B55 == 1) {
            if (D_800EC9C2 != 2) {
                if (D_80121B5E == 0) {
                    if (var_s7 < 3) {
                        color = 0xC;
                    } else {
                        color = 0xD;
                    }
                    func_80023618((CharacterSelectTime *)((CharacterSelectResultNameData *)D_800EC9F0)->resultNames[courseIds[D_80121B50]][var_s7],
                                  arg0->x + 0x14, arg0->y + var_s5, color);
                    func_8000F030((s16)(arg0->x + 0x54), (s16)(arg0->y + var_s5),
                                  func_80043040(D_80112130.popupFontHandle),
                                  ((D_800F4290[(courseIds[D_80121B50] * 5) + var_s7] & 7) + 0x51) & 0xFFFF, 0x20, 0x20, 0, 0);
                    func_8000F030((s16)(arg0->x + 0x65), (s16)(arg0->y + var_s5),
                                  func_80043040(D_80112130.popupFontHandle),
                                  ((D_800F4290[(courseIds[D_80121B50] * 5) + var_s7] >> 3) + 0x7C) & 0xFFFF, 0x20, 0x20, 0, 0);
                } else if (D_80121B5E == 1) {
                    if (var_s7 < 3) {
                        var_s4 = 0xC;
                    } else {
                        var_s4 = 0xD;
                    }
                    selectedCoursePtr = &courseIds[D_80121B50];
                    quotient = D_800F4222[((*selectedCoursePtr * 5) + var_s7)] / 10;
                    sp70 = quotient;
                    if (sp70 != 0) {
                        func_8000F030((s16)(arg0->x + 0x18), (s16)(arg0->y + var_s5),
                                      func_80043040(D_80112130.popupFontHandle), (sp70 + 0x2B) & 0xFFFF, 0x20, 0x20, 0,
                                      var_s4 + 1);
                        selectedCoursePtr = &courseIds[D_80121B50];
                    }
                    sp72 = D_800F4222[((*selectedCoursePtr * 5) + var_s7)] % 10;
                    func_8000F030((s16)(arg0->x + 0x20), (s16)(arg0->y + var_s5), func_80043040(D_80112130.popupFontHandle),
                                  (sp72 + 0x2B) & 0xFFFF, 0x20, 0x20, 0, (var_s4 + 1) & 0xFF);
                    func_80013D0C((s16)(arg0->x + 0x28), (s16)(arg0->y + var_s5 + 8), "HIT", color, 0x100);
                    func_8000F030((s16)(arg0->x + 0x48), (s16)(arg0->y + var_s5),
                                  func_80043040(D_80112130.popupFontHandle),
                                  ((D_800F4259[(courseIds[D_80121B50] * 5) + var_s7] & 7) + 0x51) & 0xFFFF, 0x20, 0x20, 0, 0);
                    func_8000F030((s16)(arg0->x + 0x60), (s16)(arg0->y + var_s5),
                                  func_80043040(D_80112130.popupFontHandle),
                                  ((D_800F4259[(courseIds[D_80121B50] * 5) + var_s7] >> 3) + 0x7C) & 0xFFFF, 0x20, 0x20, 0, 0);
                } else {
                    trickIcon = &D_800EC9F0[var_s7];
                    if (var_s7 < 3) {
                        var_s4_2 = 0xC;
                    } else {
                        var_s4_2 = 0xD;
                    }
                    var_s0 = ((CharacterSelectStatsData *)D_800EC9F0)->trickValues[0][var_s7];
                    var_s1 = 0;
                    do {
                        func_8000F030((s16)((arg0->x - var_s1) + 0x38), (s16)(arg0->y + var_s5),
                                      func_80043040(D_80112130.popupFontHandle), ((var_s0 % 10) + 0x2B) & 0xFFFF, 0x20, 0x20, 0,
                                      (var_s4_2 + 1) & 0xFF);
                        var_s1 += 8;
                        var_s0 = var_s0 / 10;
                    } while (var_s0 != 0);
                    func_80013D0C((s16)(arg0->x + 0x40), (s16)(arg0->y + var_s5 + 8), "P", color, 0x100);
                    func_8000F030((s16)(arg0->x + 0x4C), (s16)(arg0->y + var_s5), func_80043040(D_80112130.popupFontHandle),
                                  ((trickIcon[0x77E7] & 7) + 0x51) & 0xFFFF, 0x20, 0x20, 0, 0);
                    func_8000F030((s16)(arg0->x + 0x60), (s16)(arg0->y + var_s5), func_80043040(D_80112130.popupFontHandle),
                                  ((trickIcon[0x77E7] >> 3) + 0x7C) & 0xFFFF, 0x20, 0x20, 0, 0);
                }
            } else {
                if (var_s7 < 3) {
                    color = 0xC;
                } else {
                    color = 0xD;
                }
                func_80023618((CharacterSelectTime *)((CharacterSelectCourseStatsNameData *)D_800EC9F0)->courseStatsNames[courseIds[D_80121B50]][var_s7],
                              arg0->x + 0x14, arg0->y + var_s5, color);
                func_8000F030((s16)(arg0->x + 0x54), (s16)(arg0->y + var_s5), func_80043040(D_80112130.popupFontHandle),
                              ((D_800F41EB[(courseIds[D_80121B50] * 5) + var_s7] & 7) + 0x51) & 0xFFFF, 0x20, 0x20, 0, 0);
                func_8000F030((s16)(arg0->x + 0x65), (s16)(arg0->y + var_s5), func_80043040(D_80112130.popupFontHandle),
                              ((D_800F41EB[(courseIds[D_80121B50] * 5) + var_s7] >> 3) + 0x7C) & 0xFFFF, 0x20, 0x20, 0, 0);
            }
        }
        var_s7++;
        var_s5 += 0x14;
    } while (var_s7 != 5);
}
#endif

void func_80023198(CharacterSelectWidgetActor *arg0) {
    u8 state = arg0->sprite.bytes.state;
    u8 unk;
    s16 target;

    switch (state) {
    case 0:
        unk = D_80121B5E;
        if ((D_800EC9C2 == 2) || ((D_800EC9C2 == 1) && (unk == 0))) {
            target = -0x8A;
        } else {
            target = -0x88;
        }
        arg0->x += 0x20;
        if (arg0->x >= target) {
            arg0->x = target;
            arg0->sprite.bytes.state = 1;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 1:
        if ((D_80121D88 == 3) || (D_80121D88 == 7)) {
            state = arg0->sprite.bytes.state = 2;
        }
        break;
    case 2:
        arg0->x -= 0x20;
        if (arg0->x < -0xEF) {
            arg0->x = -0xF0;
            arg0->sprite.bytes.state = 3;
        }
        state = arg0->sprite.bytes.state;
        break;
    case 3:
        state = arg0->sprite.bytes.state = 4;
        break;
    case 4:
        break;
    }
    state = arg0->sprite.bytes.state;
    if (state == 4) {
        func_800716E4(arg0);
        D_80121D88 = 4;
        return;
    }
    func_800483FC(&D_80124868, func_800227D8, arg0);
}

void func_800232F4(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0xF0;
    arg0->y = -0x40;
    arg0->sprite.bytes.state = 0;
    func_80071824(arg0, func_80023198);
}

void func_8002332C(CharacterSelectWidgetActor *arg0) {
    func_8000F030(arg0->x, arg0->y, func_80043040(D_80112172), 0x28, 0x20, 0x20, 0, 0);
    if ((s32) arg0->transition.bytes.state > 0) {
        func_80013154((s16) (arg0->x + 4), (s16) (arg0->y + 4), D_800B6B88, 1, 0x100, 0);
        func_8000F030(
            (s16) (arg0->x + 0x64),
            (s16) (arg0->y + 4),
            func_80043040(D_80112178),
            (u16) ((arg0->transition.bytes.timer >= 8) + 5),
            0x20,
            0x20,
            0,
            0);
    }
}

void func_80023434(CharacterSelectWidgetActor *arg0) {
    u8 state;
    u8 globalState;

    state = arg0->transition.bytes.state;
    if (state != (globalState = D_8010AE88.bytes[1])) {
        arg0->transition.bytes.state = globalState;
        /* Preserve IDO's state/globalState register allocation. */
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        if (1) {}
        state = globalState;
        arg0->transition.bytes.timer = D_8010AE88.bytes[7];
    }

    switch (state) {
    case 0:
        arg0->x += 0x20;
        if (arg0->x >= -0x88) {
            arg0->x = -0x88;
            arg0->transition.bytes.state = 2;
        }
        state = arg0->transition.bytes.state;
        break;
    case 2:
        arg0->transition.bytes.timer = (arg0->transition.bytes.timer + 1) & 0xF;
        if (D_80121D88 == 3) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 3:
        state = arg0->transition.bytes.state;
        arg0->transition.bytes.timer = 0;
        break;
    case 4:
        arg0->transition.bytes.timer++;
        if ((D_80121D88 == 3) || (D_80121D88 == 7)) {
            arg0->transition.bytes.state = 5;
        }
        state = arg0->transition.bytes.state;
        break;
    case 5:
        arg0->x -= 0x20;
        if (arg0->x < -0x107) {
            arg0->transition.bytes.state = 6;
        }
        state = arg0->transition.bytes.state;
        break;
    }

    D_8010AE89 = state;
    D_8010AE8F = arg0->transition.bytes.timer;

    if (arg0->transition.bytes.state == 6) {
        func_800716E4(arg0);
        D_8010ADE0 = NULL;
        D_8010AE88.bytes[1] = 0;
        D_8010AE88.bytes[7] = 0;
        return;
    }
    func_800483FC(&D_80124868, func_8002332C, arg0);
}

void func_800235E0(CharacterSelectWidgetActor *arg0) {
    arg0->x = -0x108;
    arg0->y = 0x28;
    arg0->transition.bytes.state = 0;
    func_80071824(arg0, func_80023434);
}

// func_80023618 best match: 81.667%
#pragma GLOBAL_ASM("asm/nonmatchings/character_select_ui/func_80023618.s")

#ifdef NON_MATCHING
void func_80023618(CharacterSelectTime *arg0, s32 x, s32 y, s32 alpha) {
    char buffer[8];
    char *it;
    char *end;
    s32 tempY;
    s32 tempAlpha;
    s32 drawY;
    s32 drawAlpha;

    tempY = y;
    tempAlpha = alpha;
    sprintf(buffer, "%2.2d", arg0->minutes);
    drawY = (s16)tempY;
    drawAlpha = tempAlpha & 0xFFFF;
    it = buffer;
    end = buffer + 2;
    do {
        func_80046D68((s16)x, drawY, func_80043040(D_80112130.popupFontHandle), ((u8)*it - 5) & 0xFFFF,
                      drawAlpha);
        it++;
        x += 8;
    } while ((u32)it < (u32)end);
    func_80046D68((s16)x, drawY, func_80043040(D_80112130.popupFontHandle), 0x36, drawAlpha);

    if ((D_800EC9C2 == 2) || ((D_800EC9C2 == 1) && (D_80121B5E == 0))) {
        x += 6;
    } else {
        x += 8;
    }

    sprintf(buffer, "%2.2d", arg0->seconds);
    it = buffer;
    end = buffer + 2;
    do {
        func_80046D68((s16)x, drawY, func_80043040(D_80112130.popupFontHandle), ((u8)*it - 5) & 0xFFFF,
                      drawAlpha);
        it++;
        x += 8;
    } while ((u32)it < (u32)end);
    func_80046D68((s16)x, drawY, func_80043040(D_80112130.popupFontHandle), 0x35, drawAlpha);

    x += 8;
    sprintf(buffer, "%2.2d", arg0->centiseconds >> 8);
    it = buffer;
    end = buffer + 2;
    do {
        func_80046D68((s16)x, drawY, func_80043040(D_80112130.popupFontHandle), ((u8)*it - 5) & 0xFFFF,
                      drawAlpha);
        it++;
        x += 8;
    } while (it != end);
}
#endif
