#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/controller_input.h"
#include "game/ending/ending_credits_ui.h"
#include "game/ending/ending_credits_flow.h"
#include "font_encoding.h"
/*
 * drawEndingCreditsPageText only matches when this call is compiled against
 * the original promoted argument types. Suppress the narrow prototype here so
 * the legacy declaration below remains local to this translation unit.
 */
#define MENU_RENDERER_OMIT_DRAW_MENU_GLYPH_SCRIPT
#include "game/menu/renderer/menu_renderer.h"
#undef MENU_RENDERER_OMIT_DRAW_MENU_GLYPH_SCRIPT

extern void drawMenuGlyphScript(s32 x, s32 y, u8 *text, s32 palette, s32 scale, s32 colorMode);

#define ENDING_CREDITS_TEXT_FADE_MAX 0x100
#define ENDING_CREDITS_PAGE_FADE_STEP 0xA
#define ENDING_CREDITS_PAGE_RESET_DELAY 0x20
#define ENDING_CREDITS_PAGE_VISIBLE_FRAMES 0x96
#define ENDING_CREDITS_PAGE_COUNT 0x19

struct EndingCreditsPageTextActor {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s16 x;
    /* 0x1E */ s16 y;
    /* 0x20 */ u16 pageIndex;
    /* 0x22 */ u16 timer;
    /* 0x24 */ s16 alpha;
    /* 0x26 */ u8 state;
};

typedef struct EndingObjectSpriteDebugViewerActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 spriteId;
    /* 0x1E */ u8 enabled;
    /* 0x1F */ u8 palette;
} EndingObjectSpriteDebugViewerActor;

typedef struct EndingCreditsPageTextPosition {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
} EndingCreditsPageTextPosition;

typedef struct EndingCreditsPageTextLineLayout {
    /* 0x00 */ s16 count;
    /* 0x02 */ EndingCreditsPageTextPosition positions[5];
    /* 0x16 */ s16 pad16;
} EndingCreditsPageTextLineLayout;

MenuGlyphScript gEndingCreditsPageTextScripts[ENDING_CREDITS_PAGE_COUNT][0x5A] = {
    // textconv requires these _() invocations to retain their original line layout.
    // clang-format off
    { _("{COLOR:0}SNOWBOARD KIDS{END}{COLOR:0}STAFF") },
    { _("{COLOR:2}VOICE ACTRESS{END}{COLOR:0}LANI MINELLA") },
    { _("{COLOR:2}VOICE ACTOR{END}{COLOR:0}JUNICHI SUWABE{END}{COLOR:0}KOUICHI TOOCHIKA") },
    { _("{COLOR:2}PLANNER{END}{COLOR:0}YUMIKO HATTORI") },
    { _("{COLOR:2}ASSISTANT{END}{COLOR:2}  PLANNER{END}{COLOR:0}SIGERU MAEKAWA") },
    { _("{COLOR:2}PROGRAMMER{END}{COLOR:0}KID") },
    { _("{COLOR:2}ASSISTANT{END}{COLOR:2}  PROGRAMMER{END}{COLOR:0}TAKASI HASIZUME") },
    { _("{COLOR:2}GRAPHIC{END}{COLOR:2}  DESIGNER{END}{COLOR:0}MIKI MIKAMI") },
    { _("{COLOR:0}KOUICHI KIZATO") },
    { _("{COLOR:0}SHUUHEI FUJINAGA") },
    { _("{COLOR:2}MOTION DESIGNER{END}{COLOR:0}YASUHIRO TAMAKI") },
    { _("{COLOR:2}SOUND PRODUCER{END}{COLOR:0}ISAO KASAI{END}{COLOR:0}TOMOHIKO SATOU") },
    { _("{COLOR:2}TRANSLATE STAFF{END}{COLOR:0}JOHN YAMAMOTO{END}{COLOR:0}BRIAN GLAZEBROOK{END}{COLOR:0}GAIL SALAMANCA") },
    { _("{COLOR:0}PETE STERN{END}{COLOR:0}SUSAN ABESIMA{END}{COLOR:0}MICHIYO SUZUKI") },
    { _("{COLOR:2}TEST PLAYER{END}{COLOR:0}JUNICHI ODA{END}{COLOR:2}{CREDITS_MANAGER_LEFT}MANAGER{CREDITS_MANAGER_RIGHT}") },
    { _("{COLOR:2}TEST PLAYER{END}{COLOR:0}TETUYA IIOKA{END}{COLOR:0}HIROYUKI SUZUKI{END}{COLOR:0}YUUICHI HASEGAWA{END}{COLOR:0}DAISUKE HOSINO") },
    { _("{COLOR:2}SPECIAL THANKS{END}{COLOR:0}YUKA NAKAI{END}{COLOR:0}HIRONORI KOBAYASI{END}{COLOR:0}HIROAKI KANAMARU") },
    { _("{COLOR:0}NAZO{END}{COLOR:0}TEIKOKU{END}{COLOR:0}OKACHIU{END}{COLOR:0}SUGURU ONO") },
    { _("{COLOR:0}TURUTA{END}{COLOR:0}OFFSPRING{END}{COLOR:0}MAKO96{END}{COLOR:0}TAMASU") },
    { _("{COLOR:0}WARUNOUCHI{END}{COLOR:0}HIROKAZU{END}{COLOR:0}HIP{END}{COLOR:0}BONNIE") },
    { _("{COLOR:0}CAROL{END}{COLOR:0}YASUMURA{END}{COLOR:0}MITANI{END}{COLOR:0}KEN9999") },
    { _("{COLOR:2}PROJECT MANAGER{END}{COLOR:0}MIKI MIKAMI") },
    { _("{COLOR:2}DIRECTOR{END}{COLOR:0}HIDEKI YAYAMA{END}{COLOR:0}TAKESI MURATA") },
    { _("{COLOR:2}PRODUCER{END}{COLOR:0}HIROYUKI TANAKA") },
    { _("{COLOR:2}EXECUTIVE{END}{COLOR:2}  PRODUCER{END}{COLOR:0}HIDEYUKI YOKOYAMA") },
    // clang-format on
};

EndingCreditsPageTextLineLayout gEndingCreditsPageTextLineLayouts[ENDING_CREDITS_PAGE_COUNT] = {
    { 2, { { -112, 24 }, { -40, 48 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },            0 },
    { 2, { { -112, 0 }, { -96, 44 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },             0 },
    { 3, { { -112, 0 }, { -112, 36 }, { -128, 60 }, { 0, 0 }, { 0, 0 } },        0 },
    { 2, { { -112, 0 }, { -112, 44 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },            0 },
    { 3, { { -112, 0 }, { -112, 16 }, { -112, 56 }, { 0, 0 }, { 0, 0 } },        0 },
    { 2, { { -112, 0 }, { -24, 44 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },             0 },
    { 3, { { -112, 0 }, { -112, 16 }, { -120, 52 }, { 0, 0 }, { 0, 0 } },        0 },
    { 3, { { -112, 0 }, { -112, 16 }, { -88, 52 }, { 0, 0 }, { 0, 0 } },         0 },
    { 1, { { -112, 30 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },               0 },
    { 1, { { -128, 30 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },               0 },
    { 2, { { -112, 0 }, { -120, 44 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },            0 },
    { 3, { { -112, 0 }, { -80, 36 }, { -112, 60 }, { 0, 0 }, { 0, 0 } },         0 },
    { 4, { { -112, 0 }, { -104, 24 }, { -128, 48 }, { -112, 72 }, { 0, 0 } },    0 },
    { 3, { { -80, 2 }, { -112, 26 }, { -112, 50 }, { 0, 0 }, { 0, 0 } },         0 },
    { 3, { { -112, 0 }, { -88, 52 }, { -88, 68 }, { 0, 0 }, { 0, 0 } },          0 },
    { 5, { { -112, 0 }, { -96, 24 }, { -120, 42 }, { -120, 60 }, { -112, 78 } }, 0 },
    { 4, { { -112, 0 }, { -80, 24 }, { -136, 48 }, { -128, 72 }, { 0, 0 } },     0 },
    { 4, { { -32, 6 }, { -56, 30 }, { -56, 54 }, { -80, 78 }, { 0, 0 } },        0 },
    { 4, { { -48, 6 }, { -72, 30 }, { -48, 54 }, { -48, 78 }, { 0, 0 } },        0 },
    { 4, { { -80, 6 }, { -64, 30 }, { -24, 54 }, { -48, 78 }, { 0, 0 } },        0 },
    { 4, { { -40, 6 }, { -64, 30 }, { -48, 54 }, { -56, 78 }, { 0, 0 } },        0 },
    { 2, { { -112, 0 }, { -88, 44 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },             0 },
    { 3, { { -112, 0 }, { -104, 36 }, { -104, 60 }, { 0, 0 }, { 0, 0 } },        0 },
    { 2, { { -112, 0 }, { -120, 44 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },            0 },
    { 3, { { -112, 0 }, { -112, 16 }, { -136, 52 }, { 0, 0 }, { 0, 0 } },        0 },
};

extern int rmonPrintf(const char *, ...);
extern int sprintf(char *, const char *, ...);

void drawEndingCreditsPageText(EndingCreditsPageTextActor *arg0);
void updateEndingCreditsTheEndTextFadeIn(EndingCreditsPageTextActor *arg0);
void updateEndingCreditsPageText(EndingCreditsPageTextActor *arg0);
void updateEndingObjectSpriteDebugViewer(EndingObjectSpriteDebugViewerActor *arg0);

CLANG_DIAGNOSTIC_PUSH
CLANG_DIAGNOSTIC_IGNORE_UNINITIALIZED
void drawEndingCreditsPageText(EndingCreditsPageTextActor *arg0) {
    register s32 lineCount;
    s32 lineIndex;
    s32 scriptIndex;
    s32 lineLength;
    s32 positionIndex;
    s16 alpha;
    s16 x;
    s32 y;
    u16 lineText[0x1C];
    u16 pad[6];
    EndingCreditsPageTextPosition *position;
    EndingCreditsPageTextLineLayout *layout;
    volatile u16 colorMode;
    u16 glyph;

    lineCount = (&gEndingCreditsPageTextLineLayouts[arg0->pageIndex])->count;
    lineIndex = 0;
    if (!position->y) {}
    if (lineCount > 0) {
        scriptIndex = 0;
        positionIndex = 0;
        do {
            layout = &gEndingCreditsPageTextLineLayouts[y = arg0->pageIndex];
            position = &layout->positions[positionIndex];
            x = position->x;
            glyph = gEndingCreditsPageTextScripts[arg0->pageIndex][scriptIndex];
            y = position->y;
            lineLength = 0;
            if (gEndingCreditsPageTextScripts[arg0->pageIndex][scriptIndex] != 0xFFFF) {
                do {
                    lineText[lineLength] = gEndingCreditsPageTextScripts[arg0->pageIndex][scriptIndex];
                    scriptIndex++;
                    glyph = gEndingCreditsPageTextScripts[arg0->pageIndex][scriptIndex];
                    glyph = (((((glyph & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu;
                    if ((y && y) && y) {}
                    lineLength++;
                } while (gEndingCreditsPageTextScripts[arg0->pageIndex][scriptIndex] != 0xFFFF);
                if (!x) {}
                if (!lineCount) {}
            }
            lineText[lineLength] = 0xFFFF;
            alpha = 0xFFFF;
            scriptIndex++;
            drawMenuGlyphScript(
                (x << 16) >> 16,
                y,
                (u8 *)lineText,
                (((0 & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu,
                arg0->alpha,
                colorMode & alpha
            );
            if (((!arg0) && (!arg0)) && (!arg0)) {}
            lineIndex++;
            positionIndex++;
        } while (lineIndex != lineCount);
        if (x) {}
    }
}
CLANG_DIAGNOSTIC_POP

void drawEndingCreditsTheEndText(EndingCreditsPageTextActor *arg0) {
    drawMenuSpriteWithAlpha(
        arg0->x,
        arg0->y,
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
        0x35,
        0x20,
        0x20,
        0,
        arg0->alpha,
        0
    );
    drawMenuSpriteWithAlpha(
        (s16)(arg0->x + 0x40),
        arg0->y,
        getRelocatableHeapBlockBase(gAssetHandles[0x21]),
        0x36,
        0x20,
        0x20,
        0,
        arg0->alpha,
        0
    );
}

void updateEndingCreditsTheEndTextFadeIn(EndingCreditsPageTextActor *arg0) {
    s32 v1 = ENDING_CREDITS_TEXT_FADE_MAX;
    s32 v0;

    if (gEndingCreditsSequencePhase == 0x43) {
        v0 = arg0->alpha;
        if (v1 != v0) {
            arg0->alpha = v0 + 0x10;
            if (arg0->alpha >= ENDING_CREDITS_TEXT_FADE_MAX) {
                arg0->alpha = v1;
            }
        }
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingCreditsTheEndText, arg0);
    }
}

void updateEndingCreditsPageText(EndingCreditsPageTextActor *arg0) {
    switch (arg0->state) {
        case 0:
            arg0->alpha += ENDING_CREDITS_PAGE_FADE_STEP;
            if (!(arg0->alpha < ENDING_CREDITS_TEXT_FADE_MAX)) {
                arg0->alpha = ENDING_CREDITS_TEXT_FADE_MAX;
                arg0->state = 1;
                arg0->timer = 0;
            }
            break;
        case 1:
            arg0->timer = arg0->timer + 1;
            if (arg0->timer == ENDING_CREDITS_PAGE_VISIBLE_FRAMES) {
                arg0->timer = 0;
                arg0->state = 2;
            }
            break;
        case 2:
            arg0->alpha -= ENDING_CREDITS_PAGE_FADE_STEP;
            if (!(arg0->alpha > 0)) {
                arg0->alpha = 0;
                arg0->state = 3;
                arg0->pageIndex = arg0->pageIndex + 1;
                if (arg0->pageIndex == ENDING_CREDITS_PAGE_COUNT) {
                    arg0->pageIndex = 0;
                    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsTheEndTextFadeIn);
                }
                if (gEndingCreditsSequencePhase == 0) {
                    gEndingCreditsSequencePhase = 1;
                }
            }
            break;
        case 3:
            arg0->timer = arg0->timer + 1;
            if (!(arg0->timer < ENDING_CREDITS_PAGE_RESET_DELAY)) {
                arg0->timer = 0;
                arg0->state = 0;
            }
            break;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingCreditsPageText, arg0);
}

void initEndingCreditsPageTextActor(EndingCreditsPageTextActor *arg0) {
    arg0->state = 3;
    arg0->pageIndex = 0;
    arg0->x = -0x40;
    arg0->y = 0x10;
    arg0->alpha = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingCreditsPageText);
}

void drawEndingObjectSpriteDebugViewer(EndingObjectSpriteDebugViewerActor *arg0) {
    char sp38[0x10];

    if (arg0->enabled == 1) {
        drawMenuSprite(
            arg0->x,
            arg0->y,
            getRelocatableHeapBlockBase(gAssetHandles[0x21]),
            (u16)arg0->spriteId,
            0x20,
            0x20,
            arg0->palette,
            0
        );
        sprintf(sp38, "ENDOBJ %2d \n", arg0->spriteId);
        drawMenuAsciiText(0x40, -0x66, sp38, 0, 0x100);
    }
}

void updateEndingObjectSpriteDebugViewer(EndingObjectSpriteDebugViewerActor *arg0) {
    s16 temp_a1;
    s16 temp_a2;
    s16 oldY;

    if (gPlayerInputPressed[0] & Z_TRIG) {
        if (arg0->enabled == 1) {
            arg0->enabled = 0;
        } else {
            arg0->enabled = 1;
        }
    }
    if (arg0->enabled == 1) {
        temp_a2 = arg0->y;
        temp_a1 = arg0->x;
        oldY = temp_a2;
        if ((gPlayerInputHeld[0] & (STICK_UP | U_JPAD)) && (temp_a2 >= -0x73)) {
            arg0->y = temp_a2 - 1;
        }
        if (gPlayerInputHeld[0] & (STICK_DOWN | D_JPAD)) {
            temp_a2 = arg0->y;
            if (temp_a2 < 0x68) {
                arg0->y = temp_a2 + 1;
            }
        }
        if (gPlayerInputHeld[0] & (STICK_LEFT | L_JPAD)) {
            if (arg0->x >= -0x67) {
                arg0->x = arg0->x - 1;
            }
        }
        if (gPlayerInputHeld[0] & (STICK_RIGHT | R_JPAD)) {
            if (arg0->x < 0x68) {
                arg0->x = arg0->x + 1;
            }
        }
        if (gPlayerInputPressed[0] & A_BUTTON) {
            arg0->spriteId += 1;
            if (arg0->spriteId == 0x35) {
                arg0->spriteId = 0;
            }
        }
        if (gPlayerInputPressed[0] & B_BUTTON) {
            arg0->spriteId = arg0->spriteId - 1;
            if (arg0->spriteId < 0) {
                arg0->spriteId = 0x34;
            }
        }
        if (gPlayerInputPressed[0] & U_CBUTTONS) {
            arg0->palette = (arg0->palette + 1) & 1;
        }
        temp_a2 = arg0->y;
        if ((temp_a1 != arg0->x) || (oldY != temp_a2)) {
            rmonPrintf("x = %d  y = %d \n", arg0->x, temp_a2);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawEndingObjectSpriteDebugViewer, arg0);
}

void initEndingObjectSpriteDebugViewerActor(EndingObjectSpriteDebugViewerActor *arg0) {
    arg0->x = 0;
    arg0->y = 0;
    arg0->spriteId = 0;
    arg0->enabled = 0;
    arg0->palette = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateEndingObjectSpriteDebugViewer);
}
