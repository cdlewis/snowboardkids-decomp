#include "common.h"
#include "font_encoding.h"
#include "game/audio/sound_manager.h"
#include "game/engine/asset_manager.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/training/training_course_ui.h"
#include "game/menu/renderer/menu_renderer.h"

extern u8 gTrainingCourseLesson;
extern TrainingCourseUiScript gTrainingCourseOpeningDialogScripts[9][0x17C];
extern TrainingCourseUiScript gTrainingCourseEndingDialogScript[0x40];

extern void redrawTrainingCourseLessonEndMenu(TrainingCourseUiActor *arg0);
extern void drawTrainingCourseLessonEndMenu(TrainingCourseUiActor *arg0);
extern void updateTrainingCourseLessonEndMenu(TrainingCourseUiActor *);
extern void drawTrainingCourseDialog(TrainingCourseUiActor *);
extern void redrawTrainingCourseDialog(TrainingCourseUiActor *);

void updateTrainingCourseDialog(TrainingCourseUiActor *);
extern void drawTrainingCourseLessonTitlePanel(TrainingCourseUiActor *);
extern void updateTrainingCourseLessonTitlePanelScrollOut(TrainingCourseUiActor *);
extern void updateTrainingCourseLessonTitlePanel(TrainingCourseUiActor *);

#pragma GLOBAL_ASM("asm.jp/nonmatchings/menu/training/training_course_ui/drawTrainingCourseLessonEndMenu.s")

void redrawTrainingCourseLessonEndMenu(TrainingCourseUiActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)redrawTrainingCourseLessonEndMenu, arg0);
}

void updateTrainingCourseLessonEndMenu(TrainingCourseUiActor *arg0) {
    s32 temp_a1;
    s32 var_a0;
    s32 var_v0;

    var_v0 = (u16)arg0->state.selectedAction;
    temp_a1 = var_v0;
    if (gPlayerInputPressed[0] & (STICK_UP | U_JPAD)) {
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
    if (gPlayerInputPressed[0] & (STICK_DOWN | D_JPAD)) {
        if (var_v0 != 2) {
            arg0->state.selectedAction = var_v0 + 1;
            var_a0 = (u16)arg0->state.selectedAction;
        }
    }
    if (var_a0 != temp_a1) {
        enqueueSoundEffect(SFX_MENU_CURSOR, SFX_PRIORITY_NORMAL);
    }
    if ((u16)arg0->highlightTimer < 0x10) {
        arg0->highlightScale -= 9;
    } else {
        arg0->highlightScale += 9;
    }
    arg0->highlightTimer = ((u16)arg0->highlightTimer + 1) & 0x1F;
    if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
        enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL);
        gMainMenuSelectionResult = (u16)arg0->state.selectedAction + 1;
        arg0->highlightScale = 0x100;
        arg0->highlightTimer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)redrawTrainingCourseLessonEndMenu);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawTrainingCourseLessonEndMenu, (void *)arg0);
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

void drawTrainingCourseDialog(TrainingCourseUiActor *arg0) {
    s32 i;
    u16 lineX;
    TrainingCourseUiScript token;
    s32 visibleIndex;
    TrainingCourseUiActor *dialog;
    TrainingCourseUiScript glyphText[2];
    u16 lineY;
    u16 reachedEnd;

    dialog = arg0;

    drawMenuSprite(
        dialog->x - 4,
        dialog->y + 0x14,
        getRelocatableHeapBlockBase(gAssetHandles[0x2A]),
        2,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        dialog->x + 0xF8,
        dialog->y + 0x14,
        getRelocatableHeapBlockBase(gAssetHandles[0x2A]),
        4,
        0x20,
        0x20,
        0,
        0
    );

    for (i = 0; i < 0xF0; i += 0x10) {
        drawMenuSprite(
            dialog->x + i + 0xC,
            dialog->y + 0x14,
            getRelocatableHeapBlockBase(gAssetHandles[0x2A]),
            3,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            dialog->x + i + 0xC,
            dialog->y + 0x4C,
            getRelocatableHeapBlockBase(gAssetHandles[0x2A]),
            8,
            0x20,
            0x20,
            0,
            0
        );
    }

    drawMenuSprite(
        dialog->x - 4,
        dialog->y + 0x4C,
        getRelocatableHeapBlockBase(gAssetHandles[0x2A]),
        7,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        dialog->x + 0xF8,
        dialog->y + 0x4C,
        getRelocatableHeapBlockBase(gAssetHandles[0x2A]),
        9,
        0x20,
        0x20,
        0,
        0
    );

    for (i = 0; i < 0x30; i += 0x10) {
        drawMenuSprite(
            dialog->x - 4,
            dialog->y + i + 0x24,
            getRelocatableHeapBlockBase(gAssetHandles[0x2A]),
            5,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            dialog->x + 0xF8,
            dialog->y + i + 0x24,
            getRelocatableHeapBlockBase(gAssetHandles[0x2A]),
            6,
            0x20,
            0x20,
            0,
            0
        );
    }

    dialog->glyphPalette = 7;
    glyphText[1] = 0xFFFF;
    reachedEnd = 0;
    lineX = 0;
    lineY = 0;
    i = 0;
    for (visibleIndex = 0; visibleIndex < dialog->state.script.visibleGlyphCount; visibleIndex++) {
        while (dialog->layout.script[i] > 0xFF00 && dialog->layout.script[i] != 0xFFFE) {
            token = dialog->layout.script[i];
            if (token == 0xFFFF) {
                dialog->scriptState = 1;
                reachedEnd = 1;
                break;
            }
            if (token == 0xFFFD) {
                lineY += 0x10;
                lineX = 0;
                i++;
                continue;
            }
            if (token == 0xFFFC) {
                if (((!i) && (!i)) && (!i)) {}
                i++;
                dialog->glyphPalette = dialog->layout.script[i];
                i++;
                continue;
            }
            if (token == 0xFFFB) {
                dialog->scriptState = 2;
                reachedEnd = 1;
                break;
            }
        }

        if (1 == reachedEnd) {
            glyphText[0] = 0xFFFE;
        } else {
            glyphText[0] = dialog->layout.script[i];
        }

        drawMenuColoredGlyphScript(
            dialog->x + lineX,
            dialog->y + lineY + 0x18,
            glyphText,
            0,
            0x100,
            dialog->glyphPalette,
            0x29
        );

        if (1 != reachedEnd) {
            i++;
            lineX += 0x10;
        }
    }

    dialog->glyphTimer++;
    if (dialog->glyphTimer == 4) {
        dialog->glyphTimer = 0;
        if (1 != reachedEnd) {
            dialog->state.script.visibleGlyphCount++;
        }
    }

    if (((dialog->scriptState == 1) || (dialog->scriptState == 2)) && (gMainMenuSelectionResult == 0)) {
        drawMenuSprite(
            dialog->x + 0xF4,
            dialog->y + 0x48,
            getRelocatableHeapBlockBase(gAssetHandles[0x2A]),
            dialog->confirmBlinkTimer >= 8,
            0x20,
            0x20,
            0,
            0
        );
    }
}

void updateTrainingCourseDialogScrollOut(TrainingCourseUiActor *arg0) {
    arg0->y += 0x10;
    if (arg0->y >= 0x79) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawTrainingCourseDialog, (void *)arg0);
    }
}

void updateTrainingCourseDialog(TrainingCourseUiActor *arg0) {
    TrainingCourseUiScript *scan;
    s32 keepScanning;
    u16 token;

    switch (arg0->scriptState) {
        case 0:
            keepScanning = 1;
            if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                scan = arg0->layout.script;
                arg0->state.script.visibleGlyphCount = 0;
                for (keepScanning = 1; keepScanning;) {
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
                }
            }
            break;
        case 1:
            arg0->confirmBlinkTimer = (arg0->confirmBlinkTimer + 1) & 0xF;
            if ((A_BUTTON & gPlayerInputPressed[0]) || (gPlayerInputPressed[0] & START_BUTTON)) {
                enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL);
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
            if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                enqueueSoundEffect(SFX_MENU_ACCEPT, SFX_PRIORITY_NORMAL);

                while (*arg0->layout.script != 0xFFFB) {
                    arg0->layout.script++;
                }

                arg0->layout.script++;
                arg0->scriptState = 0;
                arg0->state.script.visibleGlyphCount = 1;
                arg0->glyphTimer = 0;
                arg0->confirmBlinkTimer = 0;
            }
            break;
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawTrainingCourseDialog, (void *)arg0);
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
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawTrainingCourseDialog, arg0);
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

#pragma GLOBAL_ASM("asm.jp/nonmatchings/menu/training/training_course_ui/drawTrainingCourseLessonTitlePanel.s")

void updateTrainingCourseLessonTitlePanelScrollOut(TrainingCourseUiActor *arg0) {
    arg0->y -= 0xA;
    if (arg0->y < -0xC7) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawTrainingCourseLessonTitlePanel, (void *)arg0);
    }
}

void updateTrainingCourseLessonTitlePanel(TrainingCourseUiActor *arg0) {
    if (gMainMenuSelectionResult == 1) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateTrainingCourseLessonTitlePanelScrollOut);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawTrainingCourseLessonTitlePanel, (void *)arg0);
}

void initTrainingCourseLessonTitlePanel(TrainingCourseUiActor *arg0) {
    arg0->x = -0x48;
    arg0->y = -0x48;
    arg0->layout.panelHeight = 0x78;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateTrainingCourseLessonTitlePanel);
}
