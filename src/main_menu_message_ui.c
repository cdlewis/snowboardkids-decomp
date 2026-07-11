#include "common.h"
#include "memory_allocator.h"
#include "callback_task_scheduler.h"
#include "main_menu_message_ui.h"
#include "menu_rendering.h"

#define MAIN_MENU_MESSAGE_TEXTURE_HANDLE (D_80112130.textureHandle)

typedef MenuGlyphScript MainMenuMessageScript;

struct MainMenuMessageActor {
    /* 0x00 */ struct MainMenuMessageActor *prev;
    /* 0x04 */ struct MainMenuMessageActor *next;
    /* 0x08 */ void (*callback)(struct MainMenuMessageActor *);
    /* 0x0C */ u16 type;
    /* 0x0E */ u16 priority;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 callbackTimer;
    /* 0x16 */ s16 isActive;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    union {
        struct {
            /* 0x1C */ s16 x;
            /* 0x1E */ s16 y;
        } textOffset;
        /* 0x1C */ MainMenuMessageScript *script;
        /* 0x1C */ s32 panelHeight;
    } layout;
    /* 0x20 */ s16 highlightScale;
    /* 0x22 */ s16 highlightTimer;
    union {
        /* 0x24 */ s16 selectedChoice;
        struct {
            /* 0x24 */ u8 visibleGlyphCount;
            /* 0x25 */ u8 pad25;
        } script;
    } state;
    /* 0x26 */ s16 glyphPalette;
    /* 0x28 */ s16 glyphTimer;
    /* 0x2A */ u8 scriptState;
    /* 0x2B */ u8 confirmBlinkTimer;
    /* 0x2C */ u8 returnToSummary;
};

typedef struct MainMenuMessageAssetHandles {
    /* 0x00 */ char pad0[0x54];
    /* 0x54 */ s16 textureHandle;
} MainMenuMessageAssetHandles;

typedef struct RenderCallbackNode RenderCallbackNode;
typedef void (*RenderCallback)(s32);

extern void addRenderCallback(RenderCallbackNode **, RenderCallback, s32);
extern RenderCallbackNode *gMenuRenderCallbackList;
extern MainMenuMessageAssetHandles D_80112130;
extern u8 D_8010B1F0;
extern s32 gPlayerInputPressed;
extern MainMenuMessageScript D_800B3500[][0x17C];
extern MainMenuMessageScript D_800B4FB8[];
extern MainMenuMessageScript D_800B5038[];
extern MainMenuMessageScript D_800B5050[][0x14];
extern s16 D_800B51B6[];
extern u8 D_80121B5A;
extern void enqueueSoundEffect(s32, s32, MainMenuMessageActor *);
void func_8000DF9C(MainMenuMessageActor *);
void func_8000E5A0(MainMenuMessageActor *);
void func_8000E8CC(MainMenuMessageActor *);
void func_8000E99C(MainMenuMessageActor *);
void func_8000E9F4(MainMenuMessageActor *);
void func_8000DDA4(MainMenuMessageActor *);

void func_8000D7F0(MainMenuMessageActor *arg0) {
    s32 i;
    s32 j;
    s32 alpha;
    s32 limit;
    s32 selected;

    drawMenuSprite(arg0->x, arg0->y, getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 2, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->x + 0x50), arg0->y, getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 4, 0x20, 0x20, 0, 0);

    i = 0;
    do {
        drawMenuSprite((s16)(arg0->x + i + 0x10), arg0->y, getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)(arg0->x + i + 0x10), (s16)(arg0->y + 0x40), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 8, 0x20, 0x20, 0, 0);
        i += 0x10;
    } while (i < 0x40);

    drawMenuSprite(arg0->x, (s16)(arg0->y + 0x40), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->x + 0x50), (s16)(arg0->y + 0x40), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, 0);

    i = 0;
    limit = 0x40;
    do {
        drawMenuSprite(arg0->x, (s16)(arg0->y + i + 0x10), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 5, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)(arg0->x + 0x50), (s16)(arg0->y + i + 0x10), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 6, 0x20, 0x20, 0, 0);
        j = 0;
        do {
            drawMenuSprite((s16)(arg0->x + j + 0x10), (s16)(arg0->y + i + 0x10), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, 0);
            j += 0x10;
        } while (j != limit);
        i += 0x10;
    } while (i < 0x30);

    drawMenuSprite((s16)(arg0->x + 8), (s16)(arg0->y + 4), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 0x15, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->x + 0x44), (s16)(arg0->y + 4), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), (D_80121B5A + 0xB) & 0xFFFF, 0x20, 0x20, 0, 0);

    if (D_80121B5A != 9) {
        j = 0;
        i = 0;
        do {
            alpha = (j == (u16)arg0->state.selectedChoice) ? 0x100 : 0x60;
            drawMenuSpriteWithAlpha((s16)(arg0->x + 8), (s16)(arg0->y + i + 0x18), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), (j + 0x16) & 0xFFFF, 0x20, 0x20, 0, alpha, 0);
            j++;
            i += 0x10;
        } while (j != 3);
    } else {
        j = 0;
        i = 0;
        do {
            alpha = ((j + 1) == (u16)arg0->state.selectedChoice) ? 0x100 : 0x60;
            drawMenuSpriteWithAlpha((s16)(arg0->x + 8), (s16)(arg0->y + i + 0x20), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), (j + 0x17) & 0xFFFF, 0x20, 0x20, 0, alpha, 0);
            j++;
            i += 0x10;
        } while (j != 2);
    }

    if (D_80121B5A != 9) {
        selected = (u16)arg0->state.selectedChoice;
    } else {
        selected = (u16)arg0->state.selectedChoice - 1;
    }
    drawMenuSpriteWithAlpha(arg0->layout.textOffset.x, (s16)(arg0->layout.textOffset.y + (selected * 0x10)), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 0xA, 0x20, 0x20, 0, arg0->highlightScale, 0);
}

void func_8000DD74(s32 arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8000DD74, arg0);
}

// func_8000DDA4 best match: 99.562%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_message_ui/func_8000DDA4.s")

#ifdef NON_MATCHING
void func_8000DDA4(MainMenuMessageActor *arg0) {
    s32 temp_t1;
    s32 temp_t8;
    s32 temp_t9;
    s32 temp_a1;
    s32 var_a0;
    s32 var_v0;

    var_v0 = (u16)arg0->state.selectedChoice;
    temp_a1 = var_v0;
    if (gPlayerInputPressed & 0x10800) {
        if (D_80121B5A != 9) {
            temp_t8 = var_v0 - 1;
            if (var_v0 != 0) {
                arg0->state.selectedChoice = temp_t8;
                var_v0 = temp_t8 & 0xFFFF;
            }
        } else {
            temp_t9 = var_v0 - 1;
            if (var_v0 != 1) {
                arg0->state.selectedChoice = temp_t9;
                var_v0 = temp_t9 & 0xFFFF;
            }
        }
    }
    var_a0 = var_v0;
    if (gPlayerInputPressed & 0x20400) {
        temp_t1 = var_v0 + 1;
        if (var_v0 != 2) {
            arg0->state.selectedChoice = ((temp_t1 & 0xFFFFu) & 0xFFFFu) & 0xFFFFu;
            do {
            } while (0);
            var_a0 = temp_t1 & 0xFFFF;
        }
    }
    if (var_a0 != temp_a1) {
        enqueueSoundEffect(0x19, 0x32, arg0);
    }
    if ((u16)arg0->highlightTimer < 0x10) {
        arg0->highlightScale -= 9;
    } else {
        arg0->highlightScale += 9;
    }
    arg0->highlightTimer = ((u16)arg0->highlightTimer + 1) & 0x1F;
    if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000)) {
        enqueueSoundEffect(1, 0x32, arg0);
        D_8010B1F0 = (u16)arg0->state.selectedChoice + 1;
        arg0->highlightScale = 0x100;
        arg0->highlightTimer = 0;
        setCallbackTaskCallback(arg0, func_8000DD74);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8000D7F0, (s32)arg0);
}
#endif

void func_8000DF28(MainMenuMessageActor *arg0) {
    arg0->x = -0x30;
    arg0->y = -0x30;
    arg0->layout.textOffset.x = -0x2C;
    if (D_80121B5A != 9) {
        arg0->layout.textOffset.y = -0x1A;
        arg0->state.selectedChoice = 0;
    } else {
        arg0->layout.textOffset.y = -0x12;
        arg0->state.selectedChoice = 2;
    }
    arg0->highlightScale = 0x100;
    arg0->highlightTimer = 0;
    setCallbackTaskCallback(arg0, func_8000DDA4);
}

// func_8000DF9C best match: 95.343%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_message_ui/func_8000DF9C.s")

#ifdef NON_MATCHING
void func_8000DF9C(MainMenuMessageActor *arg0) {
    unsigned int new_var;
    MainMenuMessageScript glyph;
    MainMenuMessageScript *script;
    u16 xOffset;
    s32 i;
    s32 glyphIndex;
    s32 lineOffset;
    s32 scriptOffset;
    unsigned short stopped;
    u16 token;
    u16 drawToken;
    unsigned int pad;

    drawMenuSprite((s16)(arg0->x - 4), (s16)(arg0->y + 0x14), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 2, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->x + 0xF8), (s16)((*arg0).y + 0x14), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 4, 0x20, 0x20, 0, 0);

    i = 0;
    new_var = 0;
    do {
        drawMenuSprite((s16)(arg0->x + i + 0xC), (s16)(arg0->y + 0x14), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 3, 0x20, 0x20, new_var, new_var);
        drawMenuSprite((s16)(arg0->x + i + 0xC), (s16)(arg0->y + 0x4C), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 8, 0x20, 0x20, 0, new_var);
        i += 0x10;
    } while (i < 0xF0);

    drawMenuSprite((s16)(arg0->x - 4), (s16)(arg0->y + 0x4C), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, new_var);
    drawMenuSprite((s16)(arg0->x + 0xF8), (s16)(arg0->y + 0x4C), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 9, 0x20, 0x20, new_var, 0);

    i = new_var;
    do {
        drawMenuSprite((s16)(arg0->x - 4), (s16)(arg0->y + i + 0x24), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 5, 0x20, 0x20, new_var, 0);
        drawMenuSprite((s16)(arg0->x + 0xF8), (s16)(arg0->y + i + 0x24), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), 6, 0x20, 0x20, new_var, 0);
        i += 0x10;
    } while (i < 0x30);

    arg0->glyphPalette = 7;
    glyph = 0xFFFF;
    stopped = 0;
    xOffset = 0;
    glyphIndex = new_var;
    if ((s32)arg0->state.script.visibleGlyphCount > ((arg0->scriptState == 1) * 0)) {
        scriptOffset = 0;
        lineOffset = 0;
        do {
            scriptOffset += 0;
            script = (MainMenuMessageScript *)((u8 *)arg0->layout.script + scriptOffset);
            token = script[new_var];
            if ((token >= 0xFF01) && (token != 0xFFFE)) {
                drawToken = token;
                do {
                    if (token == 0xFFFF) {
                        arg0->scriptState = 1;
                        drawToken++;
                        drawToken--;
                        stopped = 1;
                        break;
                    }
                    if (token == 0xFFFD) {
                        token = script[1];
                        lineOffset = (lineOffset + 0x10) & 0xFFFF;
                        xOffset = 0;
                        scriptOffset += 2;
                        script++;
                        drawToken = token;
                    } else if (token == 0xFFFC) {
                        arg0->glyphPalette = script[1];
                        token = script[2];
                        scriptOffset += 4;
                        script += 2;
                        drawToken = token;
                    } else if (token == 0xFFFB) {
                        arg0->scriptState = 2;
                        if (1) {
                            do {
                                stopped = 1;
                                break;
                            } while (new_var);
                        }
                    }
                    if (drawToken < 0xFF01) {
                        break;
                    }
                    if (((MainMenuMessageScript *)((u8 *)arg0->layout.script + scriptOffset))[0] == 0xFFFE) {
                        break;
                    }
                    token = drawToken;
                } while (1);
            }

            if (stopped == 1) {
                glyph = 0xFFFE;
            } else {
                glyph = *((MainMenuMessageScript *)((u8 *)arg0->layout.script + scriptOffset));
            }
            func_8001303C((s16)(arg0->x + xOffset), (s16)(arg0->y + lineOffset + 0x18), &glyph, 0, 0x100, (u16)arg0->glyphPalette, 0x29);
            if (stopped != 1) {
                scriptOffset += 2;
                xOffset = xOffset + 0x10;
            }
            glyphIndex++;
        } while (glyphIndex < (s32)arg0->state.script.visibleGlyphCount);
    }

    arg0->glyphTimer++;
    if ((u16)arg0->glyphTimer == 4) {
        arg0->glyphTimer = 0;
        if (stopped != 1) {
            arg0->state.script.visibleGlyphCount++;
        }
    }

    if (((arg0->scriptState == 1) || (arg0->scriptState == 2)) && (D_8010B1F0 == new_var)) {
        drawMenuSprite((s16)(arg0->x + 0xF4), (s16)(arg0->y + 0x48), getMemoryBlockBase(MAIN_MENU_MESSAGE_TEXTURE_HANDLE), ((s32)arg0->confirmBlinkTimer >= 8) & 0xFFFF, 0x20, 0x20, 0, 0);
    }
}
#endif

void func_8000E548(MainMenuMessageActor *arg0) {
    arg0->y += 0x10;
    if (arg0->y >= 0x79) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8000DF9C, (s32)arg0);
    }
}

// func_8000E5A0 best match: 99.775%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_message_ui/func_8000E5A0.s")

#ifdef NON_MATCHING
void func_8000E5A0(MainMenuMessageActor *arg0) {
    MainMenuMessageScript *scan;
    MainMenuMessageScript *script;
    MainMenuMessageScript *next;
    s32 keepScanning;
    u16 token;
    u16 marker;

    switch (arg0->scriptState) {
    case 0:
        keepScanning = 1;
        if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000)) {
            scan = arg0->layout.script;
            arg0->state.script.visibleGlyphCount = 0;
            do {
                token = *scan;
                if (token < 0xFF00) {
                    scan++;
                    arg0->state.script.visibleGlyphCount++;
                } else {
                    if (token == 0xFFFD) {
                        scan++;
                    }
                    if (token == 0xFFFE) {
                        scan++;
                        arg0->state.script.visibleGlyphCount++;
                    }
                    if (token == 0xFFFC) {
                        scan += 2;
                    }
                    if (token == 0xFFFB) {
                        keepScanning = 0;
                    }
                    if (token == 0xFFFF) {
                        keepScanning = 0;
                    }
                }
            } while (keepScanning);
        }
        break;
    case 1:
        arg0->confirmBlinkTimer = (arg0->confirmBlinkTimer + 1) & 0xF;
        if ((0x8000 & gPlayerInputPressed) || (gPlayerInputPressed & 0x1000)) {
            enqueueSoundEffect(1, 0x32, arg0);
            arg0->confirmBlinkTimer = 0;
            D_8010B1F0 = 1;
            if (arg0->returnToSummary == 0) {
                setCallbackTaskCallback(arg0, func_8000E548);
            } else {
                setCallbackTaskCallback(arg0, func_8000E844);
            }
        }
        break;
    case 2:
        arg0->confirmBlinkTimer = (arg0->confirmBlinkTimer + 1) & 0xF;
        if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000)) {
            enqueueSoundEffect(1, 0x32, arg0);
            script = arg0->layout.script;
            if (*script != 0xFFFB) {
                do {
                    next = script + 1;
                    arg0->layout.script = next;
                    script = next;
                    marker = (*script) & 0xFFFF;
                    script++;
                    script--;
                } while (marker != 0xFFFB);
            }
            arg0->layout.script = script + 1;
            arg0->scriptState = 0;
            arg0->state.script.visibleGlyphCount = 1;
            arg0->glyphTimer = 0;
            arg0->confirmBlinkTimer = 0;
        }
        break;
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8000DF9C, (s32)arg0);
}
#endif

void func_8000E7CC(MainMenuMessageActor *arg0) {
    arg0->x = -0x80;
    arg0->y = 4;
    arg0->scriptState = 0;
    arg0->state.script.visibleGlyphCount = 1;
    arg0->glyphPalette = 0;
    arg0->layout.script = D_800B3500[D_80121B5A - 1];
    arg0->returnToSummary = 0;
    setCallbackTaskCallback(arg0, func_8000E5A0);
}

void func_8000E844(s32 arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8000DF9C, arg0);
}

void func_8000E874(MainMenuMessageActor *arg0) {
    arg0->x = -0x80;
    arg0->y = -0x24;
    arg0->scriptState = 0;
    arg0->state.script.visibleGlyphCount = 1;
    arg0->glyphPalette = 7;
    arg0->layout.script = D_800B4FB8;
    arg0->returnToSummary = 1;
    setCallbackTaskCallback(arg0, func_8000E5A0);
}

void func_8000E8CC(MainMenuMessageActor *arg0) {
    D_800B5038[9] = D_80121B5A;
    func_8001303C((s16)(arg0->x - 0x10), arg0->y, D_800B5038, 0, 0x100, 5, 0x29);
    func_8001303C(D_800B51B6[D_80121B5A], (s16)(arg0->y + 0x18), D_800B5050[D_80121B5A - 1], 0, 0x100, 4, 0x29);
}

void func_8000E99C(MainMenuMessageActor *arg0) {
    arg0->y -= 0xA;
    if (arg0->y < -0xC7) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8000E8CC, (s32)arg0);
    }
}

void func_8000E9F4(MainMenuMessageActor *arg0) {
    if (D_8010B1F0 == 1) {
        setCallbackTaskCallback(arg0, func_8000E99C);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)func_8000E8CC, (s32)arg0);
}

void func_8000EA44(MainMenuMessageActor *arg0) {
    arg0->x = -0x48;
    arg0->y = -0x48;
    arg0->layout.panelHeight = 0x78;
    setCallbackTaskCallback(arg0, func_8000E9F4);
}
