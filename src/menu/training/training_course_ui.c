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

#define TRAINING_COURSE_UI_TEXTURE_HANDLE (gAssetHandles[0x2A])

typedef struct {
    TrainingCourseUiScript lessonTitleText[0xB3];
    s16 lessonTitleXPositions[0xD];
} TrainingCourseLessonTitleLayout;

typedef union {
    TrainingCourseLessonTitleLayout layout;
    TrainingCourseUiScript lessonTitleText[9][0x14];
} TrainingCourseLessonTitleData;

TrainingCourseUiScript gTrainingCourseOpeningDialogScripts[9][0x17C] = {
    {
        // textconv requires these _() invocations to retain their original line layout.
        // clang-format off
        _("{COLOR:6}SNOWBOARD KIDS{COLOR:7}\nIS A RACING\nGAME WITHOUT\nBRAKES.{PAGE_BREAK}WHEN MOVING\nTHE {COLOR:6}JOYSTICK{COLOR:7},\nTHE PLAYER WILL\n{COLOR:6}MOVE THERE{COLOR:7}.{PAGE_BREAK}{COLOR:6}BOTTOM LEFT\n{COLOR:7}AND {COLOR:6}BOTTOM\nRIGHT {COLOR:7}ALLOW\n{COLOR:6}SHARP TURNS{COLOR:7}.{PAGE_BREAK}NOW, USE THE\n{COLOR:6}CONTROL STICK\n{COLOR:7}AND GO AROUND\nTHE CORNER."),
    },
    {
        _("{COLOR:7}BY PRESSING\nTHE {COLOR:6}A BUTTON{COLOR:7},\nYOU CAN {COLOR:6}JUMP{COLOR:7}.{PAGE_BREAK}WHILE THE {COLOR:6}A\nBUTTON {COLOR:7}IS\nPRESSED, {COLOR:6}YOU\nCANNOT TURN{COLOR:7}.{PAGE_BREAK}BY {COLOR:6}TAPPING THE\nA BUTTON{COLOR:7}, YOU\nHOP, {COLOR:6}HOLD IT\nDOWN{COLOR:7}, YOU JUMP.{PAGE_BREAK}YOU CAN {COLOR:6}JUMP\nOVER ROCKS {COLOR:7}AND\n{COLOR:6}JUMP HIGHER\nOFF OF RAMPS{COLOR:7}.{PAGE_BREAK}ALSO, IF YOUR\nTIMING IS GOOD\n{COLOR:6}YOU CAN JUMP\nOVER ATTACKS{COLOR:7}.{PAGE_BREAK}NOW TRY AND\nJUMP OFF OF\nTHE RAMP BY\n{COLOR:6}PRESSING A."),
    },
    {
        _("{COLOR:6}TRICKS {COLOR:7}ARE\nMOVES THAT\nLOOK COOL.{PAGE_BREAK}YOU CAN GET\n{COLOR:6}MONEY {COLOR:7}FOR\nDOING {COLOR:6}TRICKS{COLOR:7}.{PAGE_BREAK}{COLOR:6}HOLD DOWN A\n{COLOR:7}AND MOVE TO\n{COLOR:6}ONE DIRECTION{COLOR:7}.\nNOW {COLOR:6}RELEASE A{COLOR:7}.{PAGE_BREAK}NOW, DO A\n{COLOR:6}TRICK OFF{COLOR:7} OF\nTHE RAMP.{PAGE_BREAK}AFTER PRESSING\nSTART, {COLOR:6}HOLD A{COLOR:7},\n{COLOR:6}CHOOSE A SIDE\n{COLOR:7}AND {COLOR:6}LET A GO{COLOR:7}."),
    },
    {
        _("YOU CAN ALSO\n{COLOR:6}GRAB YOUR\nBOARD {COLOR:7}DURING\nA JUMP.{PAGE_BREAK}{COLOR:6}WHILE JUMPING{COLOR:7},\nSELECT ONE OF\nTHE {COLOR:6}C BUTTONS{COLOR:7}.{PAGE_BREAK}LET GO OF YOUR\nBOARD {COLOR:6}WHEN YOU\nLAND{COLOR:7}, OR {COLOR:6}YOU\nWILL FALL{COLOR:7}.{PAGE_BREAK}GRAB THE BOARD\nWHILE DOING A\n{COLOR:6}TRICK{COLOR:7}.{PAGE_BREAK}NOW, TRY TO\n{COLOR:6}GRAB YOUR\nBOARD {COLOR:7}WHILE\nIN THE AIR."),
    },
    {
        _("BESIDES THESE,\nEACH CHARACTER\nHAS THEIR OWN\n{COLOR:6}SPECIAL MOVES{COLOR:7}.{PAGE_BREAK}TO DO A\n{COLOR:6}SPECIAL TRICK\n{COLOR:7}IS THE SAME AS\nA NORMAL TRICK.{PAGE_BREAK}BUT YOU MUST\nDO THE MOVE\nIN {COLOR:6}A CERTAIN\nPATTERN{COLOR:7}.{PAGE_BREAK}NOW LETS DO A\nSPECIAL TRICK.\n{COLOR:6}YOU CAN'T GRAB\nYOUR BOARD{COLOR:7}.{PAGE_BREAK}{COLOR:6}HOLD A DOWN\n{COLOR:7}AND {COLOR:6}MOVE {CONTROL_STICK_ICON_LEFT}{CONTROL_STICK_ICON_RIGHT}\n{COLOR:7}AND {COLOR:6}HOLD{COLOR:7}.\nRELEASE A."),
    },
    {
        _("IN ORDER TO GO\nTHROUGH ANOTHER\nLAP, {COLOR:6}GET ON THE\nLIFT{COLOR:7}.{PAGE_BREAK}{COLOR:6}JUST STAND IN\nFRONT OF THE\nLIFT {COLOR:7}AND YOU\nCAN GET ON.{PAGE_BREAK}NOW GET ON THE\nLIFT.  {COLOR:6}JUST\nFOLLOW THE\nSIGN{COLOR:7}."),
    },
    {
        _("THERE ARE\n{COLOR:6}ITEMS {COLOR:7}THAT YOU\nCAN USE IN THE\nGAME.{PAGE_BREAK}{COLOR:6}ITEMS {COLOR:7}ARE ON\nTHE RIGHT\nWINDOW.  {COLOR:6}USE\nB TO ACTIVATE{COLOR:7}.{PAGE_BREAK}IN ORDER TO\nGET THE {COLOR:6}ITEMS{COLOR:7},\nYOU MUST GO TO\nA {COLOR:6}BLUE STAND{COLOR:7}.{PAGE_BREAK}YOU NEED TO\nEITHER {COLOR:6}DO\nTRICKS {COLOR:7}OR {COLOR:6}GET\nCOINS{COLOR:7}.{PAGE_BREAK}IF YOU HAVE\n{COLOR:6}100G{COLOR:7}, YOU CAN\nOBTAIN AN {COLOR:6}ITEM{COLOR:7}.{PAGE_BREAK} NOW LETS GET\nAN ITEM.  {COLOR:6}GET\nA COIN {COLOR:7}AND {COLOR:6}GET\nAN ITEM{COLOR:7}."),
    },
    {
        _("THERE ARE\n{COLOR:6}WEAPONS{COLOR:7} THAT\nYOU CAN GET\nIN THE GAME.{PAGE_BREAK}{COLOR:6}THE WEAPON{COLOR:7}\nWILL BE IN THE\nLEFT WINDOW.\n{COLOR:6}PRESS Z TO USE{COLOR:7}.{PAGE_BREAK}YOU CAN SHOOT\nUP TO {COLOR:6}3 TIMES{COLOR:7}\nWITH {COLOR:6}1 WEAPON{COLOR:7}.{PAGE_BREAK}YOU NEED TO GO\nTO A {COLOR:6}RED\nSTAND {COLOR:7}AND\nHAVE {COLOR:6}100G{COLOR:7}.{PAGE_BREAK}NOW {COLOR:6}SHOOT YOUR\nOPPONENT {COLOR:7}WITH\nTHE WEAPON.{PAGE_BREAK}PICK UP A {COLOR:6}COIN\n{COLOR:7}AND GO THOUGH\nTHE {COLOR:6}RED STAND{COLOR:7}\nAND {COLOR:6}PRESS Z{COLOR:7}."),
    },
    {
        _("YOU CAN ALSO\nPERFORM {COLOR:6}TRICKS\n{COLOR:7}ON THE NORMAL\nCOURSE.{PAGE_BREAK}WHEN THERE ARE\n{COLOR:6}BUMPS ON THE\nCOURSE{COLOR:7}, YOU\nCAN DO {COLOR:6}TRICKS{COLOR:7}.{PAGE_BREAK}BUT IT MAY BE\n{COLOR:6}DIFFICULT{COLOR:7} TO\nPERFORM HARD\nTRICKS.{PAGE_BREAK}IT'S EASIER TO\nFLIP {COLOR:6}UP/DOWN\n{COLOR:7}INSTEAD OF\n{COLOR:6}LEFT/RIGHT{COLOR:7}.{PAGE_BREAK}NOW, {COLOR:6}LET'S DO\nA TRICK {COLOR:7}WHILE\nGOING OVER THE\nBUMPS.{PAGE_BREAK}PRESS START AND\n{COLOR:6}HOLD DOWN A{COLOR:7} AND\nGO OVER THE BUMP\n{COLOR:6}RELEASE A{COLOR:7}."),
    },
};

TrainingCourseUiScript gTrainingCourseEndingDialogScript[0x40] = {
    _("THE TRAINING\nMODE IS OVER.{PAGE_BREAK}NOW YOU CAN\nPLAY THE\n{COLOR:6}REAL GAME{COLOR:7}."),
};

TrainingCourseUiScript gTrainingCourseLessonTitleLabelTemplate[0xC] = {
    _("{TRAINING_LABEL_BORDER}TRAINING0{TRAINING_LABEL_BORDER}"),
};

TrainingCourseLessonTitleData gTrainingCourseLessonTitleText = {
    {
        {
                _("{TITLE_LEFT}CORNERING{TITLE_RIGHT}"), 0, 0, 0, 0, 0, 0, 0, 0, _("{TITLE_LEFT}JUMP{TITLE_RIGHT}"), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _("{TITLE_LEFT}TRICKS PT1{TITLE_RIGHT}"), 0, 0, 0, 0, 0, 0, 0, _("{TITLE_LEFT}TRICKS PT2{TITLE_RIGHT}"), 0, 0, 0, 0, 0, 0, 0, _("{TITLE_LEFT}TRICKS PT3{TITLE_RIGHT}"), 0, 0, 0, 0, 0, 0, 0, _("{TITLE_LEFT}GOING ON LIFT{TITLE_RIGHT}"), 0, 0, 0, 0, _("{TITLE_LEFT}ITEM{TITLE_RIGHT}"), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _("{TITLE_LEFT}SHOT WEAPON{TITLE_RIGHT}"), 0, 0, 0, 0, 0, 0, _("{TITLE_LEFT}HIGH TECHNIQUE{TITLE_RIGHT}"),
            // clang-format on
        },
        {
            0x0000,
            0xFFA8,
            0xFFD0,
            0xFFA0,
            0xFFA0,
            0xFFA0,
            0xFF88,
            0xFFD0,
            0xFF98,
            0xFF80,
            0x0000,
            0x0000,
            0x0000,
        },
    },
};
/* The final title-text slot and the first X position share the halfword at 0x800B51B6. */
extern s16 gTrainingCourseLessonTitleXPositions[];
extern u8 gTrainingCourseLesson;
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

    drawMenuSprite(
        arg0->x,
        arg0->y,
        getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE),
        2,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0x50),
        arg0->y,
        getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE),
        4,
        0x20,
        0x20,
        0,
        0
    );

    i = 0;
    do {
        drawMenuSprite(
            (s16)(arg0->x + i + 0x10),
            arg0->y,
            getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE),
            3,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            (s16)(arg0->x + i + 0x10),
            (s16)(arg0->y + 0x40),
            getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE),
            8,
            0x20,
            0x20,
            0,
            0
        );
        i += 0x10;
    } while (i < 0x40);

    drawMenuSprite(
        arg0->x,
        (s16)(arg0->y + 0x40),
        getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE),
        7,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0x50),
        (s16)(arg0->y + 0x40),
        getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE),
        9,
        0x20,
        0x20,
        0,
        0
    );

    i = 0;
    limit = 0x40;
    do {
        drawMenuSprite(
            arg0->x,
            (s16)(arg0->y + i + 0x10),
            getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE),
            5,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            (s16)(arg0->x + 0x50),
            (s16)(arg0->y + i + 0x10),
            getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE),
            6,
            0x20,
            0x20,
            0,
            0
        );
        j = 0;
        do {
            drawMenuSprite(
                (s16)(arg0->x + j + 0x10),
                (s16)(arg0->y + i + 0x10),
                getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE),
                0xB,
                0x20,
                0x20,
                0,
                0
            );
            j += 0x10;
        } while (j != limit);
        i += 0x10;
    } while (i < 0x30);

    drawMenuSprite(
        (s16)(arg0->x + 8),
        (s16)(arg0->y + 4),
        getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE),
        0x15,
        0x20,
        0x20,
        0,
        0
    );
    texture = getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE);
    drawMenuSprite(arg0->x + 0x44, arg0->y + 4, texture, gTrainingCourseLesson + 0xB, 0x20, 0x20, 0, 0);

    if (gTrainingCourseLesson != 9) {
        j = 0;
        i = 0;
        do {
            alpha = (j == (u16)arg0->state.selectedAction) ? 0x100 : 0x60;
            tileIndex = j + 0x16;
            drawMenuSpriteWithAlpha(
                (s16)(arg0->x + 8),
                (s16)(arg0->y + i + 0x18),
                getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE),
                tileIndex,
                0x20,
                0x20,
                0,
                alpha,
                0
            );
            j++;
            i += 0x10;
        } while (j != 3);
    } else {
        j = 0;
        i = 0;
        do {
            alpha = ((j + 1) == (u16)arg0->state.selectedAction) ? 0x100 : 0x60;
            tileIndex = j + 0x17;
            drawMenuSpriteWithAlpha(
                (s16)(arg0->x + 8),
                (s16)(arg0->y + i + 0x20),
                getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE),
                tileIndex,
                0x20,
                0x20,
                0,
                alpha,
                0
            );
            j++;
            i += 0x10;
        } while (j != 2);
    }

    if (gTrainingCourseLesson != 9) {
        selected = (u16)arg0->state.selectedAction;
    } else {
        selected = (u16)arg0->state.selectedAction - 1;
    }
    drawMenuSpriteWithAlpha(
        arg0->layout.textOffset.x,
        (s16)(arg0->layout.textOffset.y + (selected * 0x10)),
        getRelocatableHeapBlockBase(TRAINING_COURSE_UI_TEXTURE_HANDLE),
        0xA,
        0x20,
        0x20,
        0,
        arg0->highlightScale,
        0
    );
}

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
        enqueueSoundEffect(0x19, 0x32);
    }
    if ((u16)arg0->highlightTimer < 0x10) {
        arg0->highlightScale -= 9;
    } else {
        arg0->highlightScale += 9;
    }
    arg0->highlightTimer = ((u16)arg0->highlightTimer + 1) & 0x1F;
    if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
        enqueueSoundEffect(1, 0x32);
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
            if ((A_BUTTON & gPlayerInputPressed[0]) || (gPlayerInputPressed[0] & START_BUTTON)) {
                enqueueSoundEffect(1, 0x32);
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
                enqueueSoundEffect(1, 0x32);
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

void drawTrainingCourseLessonTitlePanel(TrainingCourseUiActor *arg0) {
    gTrainingCourseLessonTitleLabelTemplate[9] = gTrainingCourseLesson;
    drawMenuColoredGlyphScript(
        (s16)(arg0->x - 0x10),
        arg0->y,
        gTrainingCourseLessonTitleLabelTemplate,
        0,
        0x100,
        5,
        0x29
    );
    drawMenuColoredGlyphScript(
        gTrainingCourseLessonTitleXPositions[gTrainingCourseLesson],
        (s16)(arg0->y + 0x18),
        gTrainingCourseLessonTitleText.lessonTitleText[gTrainingCourseLesson - 1],
        0,
        0x100,
        4,
        0x29
    );
}

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
