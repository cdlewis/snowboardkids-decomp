#include "common.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/training/training_course_ui.h"
#include "game/menu/renderer/menu_renderer.h"

#define TRAINING_COURSE_UI_TEXTURE_HANDLE (gAssetHandles.textureHandle)

typedef MenuGlyphScript TrainingCourseUiScript;

struct TrainingCourseUiActor {
    /* 0x00 */ struct TrainingCourseUiActor *prev;
    /* 0x04 */ struct TrainingCourseUiActor *next;
    /* 0x08 */ void (*callback)(struct TrainingCourseUiActor *);
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
        /* 0x1C */ TrainingCourseUiScript *script;
        /* 0x1C */ s32 panelHeight;
    } layout;
    /* 0x20 */ s16 highlightScale;
    /* 0x22 */ s16 highlightTimer;
    union {
        /* 0x24 */ s16 selectedAction;
        struct {
            /* 0x24 */ u8 visibleGlyphCount;
            /* 0x25 */ u8 pad25;
        } script;
    } state;
    /* 0x26 */ s16 glyphPalette;
    /* 0x28 */ u16 glyphTimer;
    /* 0x2A */ u8 scriptState;
    /* 0x2B */ u8 confirmBlinkTimer;
    /* 0x2C */ u8 keepVisibleAfterConfirm;
};

typedef struct TrainingCourseAssetHandles {
    /* 0x00 */ char pad0[0x54];
    /* 0x54 */ s16 textureHandle;
} TrainingCourseAssetHandles;


extern TrainingCourseAssetHandles gAssetHandles;
extern u8 gMainMenuSelectionResult;
extern s32 gPlayerInputPressed;
extern TrainingCourseUiScript gTrainingCourseOpeningDialogScripts[][0x17C];
extern TrainingCourseUiScript gTrainingCourseEndingDialogScript[];
extern TrainingCourseUiScript gTrainingCourseLessonTitleLabelTemplate[];
extern TrainingCourseUiScript gTrainingCourseLessonTitleText[][0x14];
extern s16 gTrainingCourseLessonTitleXPositions[];
extern u8 gTrainingCourseLesson;
extern void enqueueSoundEffect(s32, s32, TrainingCourseUiActor *);
void drawTrainingCourseDialog(TrainingCourseUiActor *);
void updateTrainingCourseDialog(TrainingCourseUiActor *);
void redrawTrainingCourseDialog(TrainingCourseUiActor *);
void drawTrainingCourseLessonTitlePanel(TrainingCourseUiActor *);
void updateTrainingCourseLessonTitlePanelScrollOut(TrainingCourseUiActor *);
void updateTrainingCourseLessonTitlePanel(TrainingCourseUiActor *);
void updateTrainingCourseLessonEndMenu(TrainingCourseUiActor *);

void drawTrainingCourseLessonEndMenu(TrainingCourseUiActor *arg0) {
    s32 i;
    s32 j;
    s32 alpha;
    s32 limit;
    s32 selected;
    u16 tileIndex;
    void *texture;

    drawMenuSprite(arg0->x, arg0->y, getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 2, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->x + 0x50), arg0->y, getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 4, 0x20, 0x20, 0, 0);

    i = 0;
    do {
        drawMenuSprite((s16)(arg0->x + i + 0x10), arg0->y, getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)(arg0->x + i + 0x10), (s16)(arg0->y + 0x40), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 8, 0x20, 0x20, 0, 0);
        i += 0x10;
    } while (i < 0x40);

    drawMenuSprite(arg0->x, (s16)(arg0->y + 0x40), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->x + 0x50), (s16)(arg0->y + 0x40), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, 0);

    i = 0;
    limit = 0x40;
    do {
        drawMenuSprite(arg0->x, (s16)(arg0->y + i + 0x10), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 5, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)(arg0->x + 0x50), (s16)(arg0->y + i + 0x10), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 6, 0x20, 0x20, 0, 0);
        j = 0;
        do {
            drawMenuSprite((s16)(arg0->x + j + 0x10), (s16)(arg0->y + i + 0x10), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, 0);
            j += 0x10;
        } while (j != limit);
        i += 0x10;
    } while (i < 0x30);

    drawMenuSprite((s16)(arg0->x + 8), (s16)(arg0->y + 4), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 0x15, 0x20, 0x20, 0, 0);
    texture = getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE);
    drawMenuSprite(arg0->x + 0x44, arg0->y + 4, texture, gTrainingCourseLesson + 0xB, 0x20, 0x20, 0, 0);

    if (gTrainingCourseLesson != 9) {
        j = 0;
        i = 0;
        do {
            alpha = (j == (u16)arg0->state.selectedAction) ? 0x100 : 0x60;
            tileIndex = j + 0x16;
            drawMenuSpriteWithAlpha((s16)(arg0->x + 8), (s16)(arg0->y + i + 0x18), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), tileIndex, 0x20, 0x20, 0, alpha, 0);
            j++;
            i += 0x10;
        } while (j != 3);
    } else {
        j = 0;
        i = 0;
        do {
            alpha = ((j + 1) == (u16)arg0->state.selectedAction) ? 0x100 : 0x60;
            tileIndex = j + 0x17;
            drawMenuSpriteWithAlpha((s16)(arg0->x + 8), (s16)(arg0->y + i + 0x20), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), tileIndex, 0x20, 0x20, 0, alpha, 0);
            j++;
            i += 0x10;
        } while (j != 2);
    }

    if (gTrainingCourseLesson != 9) {
        selected = (u16)arg0->state.selectedAction;
    } else {
        selected = (u16)arg0->state.selectedAction - 1;
    }
    drawMenuSpriteWithAlpha(arg0->layout.textOffset.x, (s16)(arg0->layout.textOffset.y + (selected * 0x10)), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 0xA, 0x20, 0x20, 0, arg0->highlightScale, 0);
}

void redrawTrainingCourseLessonEndMenu(TrainingCourseUiActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList,(RenderCallback)redrawTrainingCourseLessonEndMenu, arg0);
}

void updateTrainingCourseLessonEndMenu(TrainingCourseUiActor *arg0) {
    s32 temp_a1;
    s32 var_a0;
    s32 var_v0;

    var_v0 = (u16)arg0->state.selectedAction;
    temp_a1 = var_v0;
    if (gPlayerInputPressed & (STICK_UP | U_JPAD)) {
        if (gTrainingCourseLesson != 9) {
            if (var_v0 != 0) {
                arg0->state.selectedAction = var_v0 - 1;
                var_v0 = (u16)arg0->state.selectedAction;
            }
        } else {
            if (var_v0 != 1) {
                arg0->state.selectedAction = var_v0 - 1;
                var_v0 = (u16)arg0->state.selectedAction;
            }
        }
    }
    var_a0 = var_v0;
    if (gPlayerInputPressed & (STICK_DOWN | D_JPAD)) {
        if (var_v0 != 2) {
            arg0->state.selectedAction = var_v0 + 1;
            var_a0 = (u16)arg0->state.selectedAction;
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
    if ((gPlayerInputPressed & A_BUTTON) || (gPlayerInputPressed & START_BUTTON)) {
        enqueueSoundEffect(1, 0x32, arg0);
        gMainMenuSelectionResult = (u16)arg0->state.selectedAction + 1;
        arg0->highlightScale = 0x100;
        arg0->highlightTimer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)redrawTrainingCourseLessonEndMenu);
    }
    addRenderCallback(&gMenuRenderCallbackList,(RenderCallback)drawTrainingCourseLessonEndMenu, (void *)arg0);
}

void initTrainingCourseLessonEndMenu(TrainingCourseUiActor *arg0) {
    arg0->x = -0x30;
    arg0->y = -0x30;
    arg0->layout.textOffset.x = -0x2C;
    if (gTrainingCourseLesson != 9) {
        arg0->layout.textOffset.y = -0x1A;
        arg0->state.selectedAction = 0;
    } else {
        arg0->layout.textOffset.y = -0x12;
        arg0->state.selectedAction = 2;
    }
    arg0->highlightScale = 0x100;
    arg0->highlightTimer = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateTrainingCourseLessonEndMenu);
}

// drawTrainingCourseDialog best match: 96.113% (nonmatchings/drawTrainingCourseDialog-8699393380584516020/base_28.c)
#pragma GLOBAL_ASM("asm/nonmatchings/menu/training/training_course_ui/drawTrainingCourseDialog.s")

#ifdef NON_MATCHING
void drawTrainingCourseDialog(TrainingCourseUiActor *arg0) {
    TrainingCourseUiScript *script;
    u16 xOffset;
    unsigned short stopped;
    int i;
    unsigned int new_var;
    TrainingCourseUiScript glyph[2];
    u16 token;
    u16 drawToken;
    s32 glyphIndex;
    s32 scriptOffset;
    s32 lineOffset;
    unsigned int pad;

    drawMenuSprite((s16)(arg0->x - 4), (s16)(arg0->y + 0x14), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 2, 0x20, 0x20, 0, 0);
    drawMenuSprite((s16)(arg0->x + 0xF8), (s16)((*arg0).y + 0x14), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 4, 0x20, 0x20, 0, 0);

    i = 0;
    new_var = 0;
    do {
        drawMenuSprite((s16)(arg0->x + i + 0xC), (s16)(arg0->y + 0x14), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 3, 0x20, 0x20, new_var, new_var);
        drawMenuSprite((s16)(arg0->x + i + 0xC), (s16)(arg0->y + 0x4C), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 8, 0x20, 0x20, 0, new_var);
        i += 0x10;
    } while (i < 0xF0);

    drawMenuSprite((s16)(arg0->x - 4), (s16)(arg0->y + 0x4C), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, new_var);
    drawMenuSprite((s16)(arg0->x + 0xF8), (s16)(arg0->y + 0x4C), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 9, 0x20, 0x20, new_var, 0);

    i = new_var;
    do {
        drawMenuSprite((s16)(arg0->x - 4), (s16)(arg0->y + i + 0x24), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 5, 0x20, 0x20, new_var, 0);
        drawMenuSprite((s16)(arg0->x + 0xF8), (s16)(arg0->y + i + 0x24), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), 6, 0x20, 0x20, new_var, 0);
        i += 0x10;
    } while (i < 0x30);

    arg0->glyphPalette = 7;
    glyph[1] = 0xFFFF;
    stopped = 0;
    xOffset = 0;
    glyphIndex = new_var;
    if ((s32)arg0->state.script.visibleGlyphCount > ((arg0->scriptState == 1) * 0)) {
        scriptOffset = 0;
        lineOffset = 0;
        do {
            scriptOffset += 0;
            script = (TrainingCourseUiScript *)((u8 *)arg0->layout.script + scriptOffset);
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
                        xOffset = 0;
                        scriptOffset += 2;
                        script++;
                        lineOffset = (lineOffset + 0x10) & 0xFFFF;
                        drawToken = token;
                    } else if (token == 0xFFFC) {
                        arg0->glyphPalette = script[1];
                        token = script[2];
                        scriptOffset += 4;
                        script += 2;
                        drawToken = token;
                    } else if (token == 0xFFFB) {
                        arg0->scriptState = 2;
 do { if (1) { do { } while (0); do { stopped = 1; break; } while (new_var); } } while (0);
                    }
                    if (drawToken < 0xFF01) {
                        break;
                    }
                    if (((TrainingCourseUiScript *)((u8 *)arg0->layout.script + scriptOffset))[0] == 0xFFFE) {
                        break;
                    }
                    token = drawToken;
                } while (1);
            }

            if (stopped == 1) {
                glyph[0] = 0xFFFE;
            } else {
                glyph[0] = *((TrainingCourseUiScript *)((u8 *)arg0->layout.script + scriptOffset));
            }
            drawMenuColoredGlyphScript((s16)(arg0->x + xOffset), (s16)(arg0->y + lineOffset + 0x18), glyph, 0, 0x100, ((u16)arg0->glyphPalette) ^ 0, 0x29);
            if (stopped != 1) {
                scriptOffset += 2;
                xOffset = xOffset + 0x10;
            }
            glyphIndex++;
        } while (glyphIndex < (s32)arg0->state.script.visibleGlyphCount);
    }

    arg0->glyphTimer++;
    if ((u16)(*arg0).glyphTimer == 4) {
        arg0->glyphTimer = 0;
        if (stopped != 1) {
            arg0->state.script.visibleGlyphCount++;
        }
    }

    if (((arg0->scriptState == 1) || (arg0->scriptState == 2)) && (gMainMenuSelectionResult == new_var)) {
        drawMenuSprite((s16)(arg0->x + 0xF4), (s16)(arg0->y + 0x48), getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE), ((s32)arg0->confirmBlinkTimer >= 8) & 0xFFFF, 0x20, 0x20, 0, 0);
    }
}
#endif

void updateTrainingCourseDialogScrollOut(TrainingCourseUiActor *arg0) {
    arg0->y += 0x10;
    if (arg0->y >= 0x79) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gMenuRenderCallbackList,(RenderCallback)drawTrainingCourseDialog, (void *)arg0);
    }
}

void updateTrainingCourseDialog(TrainingCourseUiActor *arg0) {
    TrainingCourseUiScript *scan;
    s32 keepScanning;
    u16 token;

    switch (arg0->scriptState) {
    case 0:
        keepScanning = 1;
        if ((gPlayerInputPressed & A_BUTTON) || (gPlayerInputPressed & START_BUTTON)) {
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
        if ((A_BUTTON & gPlayerInputPressed) || (gPlayerInputPressed & START_BUTTON)) {
            enqueueSoundEffect(1, 0x32, arg0);
            arg0->confirmBlinkTimer = 0;
            gMainMenuSelectionResult = 1;
            if (arg0->keepVisibleAfterConfirm == 0) {
                setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateTrainingCourseDialogScrollOut);
            } else {
                setCallbackTaskCallback(arg0, (CallbackTaskCallback)redrawTrainingCourseDialog);
            }
        }
        break;
    case 2:
        arg0->confirmBlinkTimer = (arg0->confirmBlinkTimer + 1) & 0xF;
        if ((gPlayerInputPressed & A_BUTTON) || (gPlayerInputPressed & START_BUTTON)) {
            enqueueSoundEffect(1, 0x32, arg0);
            if (*arg0->layout.script != 0xFFFB) {
                do {
                    arg0->layout.script++;
                } while (*arg0->layout.script != 0xFFFB);
            }
            arg0->layout.script++;
            arg0->scriptState = 0;
            arg0->state.script.visibleGlyphCount = 1;
            arg0->glyphTimer = 0;
            arg0->confirmBlinkTimer = 0;
        }
        break;
    }

    addRenderCallback(&gMenuRenderCallbackList,(RenderCallback)drawTrainingCourseDialog, (void *)arg0);
}

void initTrainingCourseOpeningDialog(TrainingCourseUiActor *arg0) {
    arg0->x = -0x80;
    arg0->y = 4;
    arg0->scriptState = 0;
    arg0->state.script.visibleGlyphCount = 1;
    arg0->glyphPalette = 0;
    arg0->layout.script = gTrainingCourseOpeningDialogScripts[gTrainingCourseLesson - 1];
    arg0->keepVisibleAfterConfirm = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateTrainingCourseDialog);
}

void redrawTrainingCourseDialog(TrainingCourseUiActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList,(RenderCallback)drawTrainingCourseDialog, arg0);
}

void initTrainingCourseEndingDialog(TrainingCourseUiActor *arg0) {
    arg0->x = -0x80;
    arg0->y = -0x24;
    arg0->scriptState = 0;
    arg0->state.script.visibleGlyphCount = 1;
    arg0->glyphPalette = 7;
    arg0->layout.script = gTrainingCourseEndingDialogScript;
    arg0->keepVisibleAfterConfirm = 1;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateTrainingCourseDialog);
}

void drawTrainingCourseLessonTitlePanel(TrainingCourseUiActor *arg0) {
    gTrainingCourseLessonTitleLabelTemplate[9] = gTrainingCourseLesson;
    drawMenuColoredGlyphScript((s16)(arg0->x - 0x10), arg0->y, gTrainingCourseLessonTitleLabelTemplate, 0, 0x100, 5, 0x29);
    drawMenuColoredGlyphScript(gTrainingCourseLessonTitleXPositions[gTrainingCourseLesson], (s16)(arg0->y + 0x18), gTrainingCourseLessonTitleText[gTrainingCourseLesson - 1], 0, 0x100, 4, 0x29);
}

void updateTrainingCourseLessonTitlePanelScrollOut(TrainingCourseUiActor *arg0) {
    arg0->y -= 0xA;
    if (arg0->y < -0xC7) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gMenuRenderCallbackList,(RenderCallback)drawTrainingCourseLessonTitlePanel, (void *)arg0);
    }
}

void updateTrainingCourseLessonTitlePanel(TrainingCourseUiActor *arg0) {
    if (gMainMenuSelectionResult == 1) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateTrainingCourseLessonTitlePanelScrollOut);
    }
    addRenderCallback(&gMenuRenderCallbackList,(RenderCallback)drawTrainingCourseLessonTitlePanel, (void *)arg0);
}

void initTrainingCourseLessonTitlePanel(TrainingCourseUiActor *arg0) {
    arg0->x = -0x48;
    arg0->y = -0x48;
    arg0->layout.panelHeight = 0x78;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateTrainingCourseLessonTitlePanel);
}
