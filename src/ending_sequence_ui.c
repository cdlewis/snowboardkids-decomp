#include "common.h"
#include "memory_allocator.h"
#include "callback_task_scheduler.h"
#include "ending_sequence_ui.h"
#define MENU_RENDERING_BROAD_PROTOTYPES
#include "menu_rendering.h"

#define ENDING_SEQUENCE_FADE_MAX 0x100
#define ENDING_SEQUENCE_MESSAGE_FADE_STEP 0xA
#define ENDING_SEQUENCE_MESSAGE_RESET_DELAY 0x20
#define ENDING_SEQUENCE_MESSAGE_VISIBLE_FRAMES 0x96
#define ENDING_SEQUENCE_MESSAGE_COUNT 0x19

struct EndingSequenceMessageTask {
    /* 0x00 */ char pad[0x1C];
    /* 0x1C */ s16 x;
    /* 0x1E */ s16 y;
    /* 0x20 */ u16 cycleCount;
    /* 0x22 */ u16 timer;
    /* 0x24 */ s16 alpha;
    /* 0x26 */ u8 state;
};

struct EndingObjectDebugViewerTask {
    /* 0x00 */ char pad[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 objectId;
    /* 0x1E */ u8 enabled;
    /* 0x1F */ u8 palette;
};

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
} EndingSequenceMessagePosition;

typedef struct {
    /* 0x00 */ s16 count;
    /* 0x02 */ EndingSequenceMessagePosition positions[5];
    /* 0x16 */ s16 pad;
} EndingSequenceMessageLayout;

extern u16 gEndingSequencePhase;
extern s16 gMenuCommonSpritesAssetHandle;
extern void *gMenuRenderCallbackList;
extern s32 gPlayerInputHeld;
extern s32 gPlayerInputPressed[];
extern u16 gEndingSequenceMessageScripts[][0x5A];
extern EndingSequenceMessageLayout gEndingSequenceMessageLayouts[];
extern void addRenderCallback(void *, void *, void *);
extern int rmonPrintf(const char *, ...);
extern int sprintf(char *, const char *, ...);

void drawEndingSequenceMessagePage(EndingSequenceMessageTask *arg0);
void updateEndingSequenceFinalSprites(EndingSequenceMessageTask *arg0);
void updateEndingSequenceMessages(EndingSequenceMessageTask *arg0);
void updateEndingObjectDebugViewer(EndingObjectDebugViewerTask *arg0);

// drawEndingSequenceMessagePage best match: 93.371%
#pragma GLOBAL_ASM("asm/nonmatchings/ending_sequence_ui/drawEndingSequenceMessagePage.s")

#ifdef NON_MATCHING
void drawEndingSequenceMessagePage(EndingSequenceMessageTask *arg0) {
    register s32 count;
    s32 i;
    s32 scriptIndex;
    s32 lineLength;
    s32 layoutOffset;
    register s32 x;
    s32 y;
    u16 text[0x1A];
    u16 pad[12];
    volatile u16 colorMode;
    u16 glyph;
    EndingSequenceMessageLayout *layout;

    layout = &gEndingSequenceMessageLayouts[arg0->cycleCount];
    count = layout->count;
    i = 0;
    if (layout->count > 0) {
        scriptIndex = 0;
        layoutOffset = 0;
        do {
            layout = &gEndingSequenceMessageLayouts[arg0->cycleCount];
            glyph = gEndingSequenceMessageScripts[arg0->cycleCount][scriptIndex];
            x = *(s16 *)((u8 *)layout + layoutOffset + 2);
            y = *(s16 *)((u8 *)layout + layoutOffset + 4);
            lineLength = 0;
            if (gEndingSequenceMessageScripts[arg0->cycleCount][scriptIndex] != 0xFFFF) {
                do {
                    text[lineLength] = gEndingSequenceMessageScripts[arg0->cycleCount][scriptIndex];
                    scriptIndex++;
                    glyph = gEndingSequenceMessageScripts[arg0->cycleCount][scriptIndex];
                    lineLength++;
                } while (gEndingSequenceMessageScripts[arg0->cycleCount][scriptIndex] != 0xFFFF);
            }
            text[lineLength] = 0xFFFF;
            scriptIndex++;
            func_80013154((s16)x, y, (u8 *)text, 0, lineLength = arg0->alpha, colorMode);
            i++;
            layoutOffset += 4;
        } while (i != layout->count);
    }
}
#endif

void drawEndingSequenceFinalSprites(EndingSequenceMessageTask *arg0) {
    func_8000F8AC(arg0->x, arg0->y, func_80043040(gMenuCommonSpritesAssetHandle), 0x35, 0x20, 0x20, 0, arg0->alpha, 0);
    func_8000F8AC((s16)(arg0->x + 0x40), arg0->y, func_80043040(gMenuCommonSpritesAssetHandle), 0x36, 0x20, 0x20, 0,
                  arg0->alpha, 0);
}

void updateEndingSequenceFinalSprites(EndingSequenceMessageTask *arg0) {
    s32 v1 = ENDING_SEQUENCE_FADE_MAX;
    s32 v0;

    if (gEndingSequencePhase == 0x43) {
        v0 = arg0->alpha;
        if (v1 != v0) {
            arg0->alpha = v0 + 0x10;
            if (arg0->alpha >= ENDING_SEQUENCE_FADE_MAX) {
                arg0->alpha = v1;
            }
        }
        addRenderCallback(&gMenuRenderCallbackList, drawEndingSequenceFinalSprites, arg0);
    }
}

void updateEndingSequenceMessages(EndingSequenceMessageTask *arg0) {
    switch (arg0->state) {
    case 0:
        arg0->alpha += ENDING_SEQUENCE_MESSAGE_FADE_STEP;
        if (!(arg0->alpha < ENDING_SEQUENCE_FADE_MAX)) {
            arg0->alpha = ENDING_SEQUENCE_FADE_MAX;
            arg0->state = 1;
            arg0->timer = 0;
        }
        break;
    case 1:
        arg0->timer = arg0->timer + 1;
        if (arg0->timer == ENDING_SEQUENCE_MESSAGE_VISIBLE_FRAMES) {
            arg0->timer = 0;
            arg0->state = 2;
        }
        break;
    case 2:
        arg0->alpha -= ENDING_SEQUENCE_MESSAGE_FADE_STEP;
        if (!(arg0->alpha > 0)) {
            arg0->alpha = 0;
            arg0->state = 3;
            arg0->cycleCount = arg0->cycleCount + 1;
            if (arg0->cycleCount == ENDING_SEQUENCE_MESSAGE_COUNT) {
                arg0->cycleCount = 0;
                setCallbackTaskCallback(arg0, updateEndingSequenceFinalSprites);
            }
            if (gEndingSequencePhase == 0) {
                gEndingSequencePhase = 1;
            }
        }
        break;
    case 3:
        arg0->timer = arg0->timer + 1;
        if (!(arg0->timer < ENDING_SEQUENCE_MESSAGE_RESET_DELAY)) {
            arg0->timer = 0;
            arg0->state = 0;
        }
        break;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawEndingSequenceMessagePage, arg0);
}

void initEndingSequenceMessageTask(EndingSequenceMessageTask *arg0) {
    arg0->state = 3;
    arg0->cycleCount = 0;
    arg0->x = -0x40;
    arg0->y = 0x10;
    arg0->alpha = 0;
    setCallbackTaskCallback(arg0, updateEndingSequenceMessages);
}

void drawEndingObjectDebugViewer(EndingObjectDebugViewerTask *arg0) {
    char sp38[0x10];

    if (arg0->enabled == 1) {
        func_8000F030(arg0->x, arg0->y, func_80043040(gMenuCommonSpritesAssetHandle), (u16)arg0->objectId, 0x20, 0x20, arg0->palette,
                      0);
        sprintf(sp38, "ENDOBJ %2d \n", arg0->objectId);
        func_80013D0C(0x40, -0x66, sp38, 0, 0x100);
    }
}

void updateEndingObjectDebugViewer(EndingObjectDebugViewerTask *arg0) {
    s16 temp_a1;
    s16 temp_a2;
    s16 oldY;

    if (gPlayerInputPressed[0] & 0x2000) {
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
        if ((gPlayerInputHeld & 0x10800) && (temp_a2 >= -0x73)) {
            arg0->y = temp_a2 - 1;
        }
        if (gPlayerInputHeld & 0x20400) {
            temp_a2 = arg0->y;
            if (temp_a2 < 0x68) {
                arg0->y = temp_a2 + 1;
            }
        }
        if (gPlayerInputHeld & 0x80200) {
            if (arg0->x >= -0x67) {
                arg0->x = arg0->x - 1;
            }
        }
        if (gPlayerInputHeld & 0x40100) {
            if (arg0->x < 0x68) {
                arg0->x = arg0->x + 1;
            }
        }
        if (gPlayerInputPressed[0] & 0x8000) {
            arg0->objectId += 1;
            if (arg0->objectId == 0x35) {
                arg0->objectId = 0;
            }
        }
        if (gPlayerInputPressed[0] & 0x4000) {
            arg0->objectId = arg0->objectId - 1;
            if (arg0->objectId < 0) {
                arg0->objectId = 0x34;
            }
        }
        if (gPlayerInputPressed[0] & 8) {
            arg0->palette = (arg0->palette + 1) & 1;
        }
        temp_a2 = arg0->y;
        if ((temp_a1 != arg0->x) || (oldY != temp_a2)) {
            rmonPrintf("x = %d  y = %d \n", arg0->x, temp_a2);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, drawEndingObjectDebugViewer, arg0);
}

void initEndingObjectDebugViewerTask(EndingObjectDebugViewerTask *arg0) {
    arg0->x = 0;
    arg0->y = 0;
    arg0->objectId = 0;
    arg0->enabled = 0;
    arg0->palette = 0;
    setCallbackTaskCallback(arg0, updateEndingObjectDebugViewer);
}
