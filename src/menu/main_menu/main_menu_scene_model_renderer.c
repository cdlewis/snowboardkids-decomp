#include "game/menu/main_menu/main_menu_scene_model.h"
#include "game/engine/asset_manager.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/render_callback.h"
#include "game/menu/main_menu/main_menu_scene_model_renderer.h"
#include "game/race/player/race_player_model_renderer.h"

#define ASSET_HANDLE(index) (gAssetHandles[(index)])

Gfx *gMainMenuSceneModelPartDisplayLists[] = {
    (Gfx *)0x02000000, (Gfx *)0x02000068, (Gfx *)0x020000D8,
    (Gfx *)0x02000168, (Gfx *)0x020001D8, (Gfx *)0x02000268,
    (Gfx *)0x02000540, (Gfx *)0x02000A28, (Gfx *)0x02000AA0,
    (Gfx *)0x02000C48, (Gfx *)0x02000CC0, (Gfx *)0x02000E68,
    (Gfx *)0x020010C0,
    (Gfx *)0x02000000, (Gfx *)0x02000128, (Gfx *)0x02000218,
    (Gfx *)0x02000308, (Gfx *)0x020003F8, (Gfx *)0x020004F0,
    (Gfx *)0x02000850, (Gfx *)0x02000BE8, (Gfx *)0x02000D10,
    (Gfx *)0x02000FD0, (Gfx *)0x02001128, (Gfx *)0x020013E8,
    (Gfx *)0x02001630,
    (Gfx *)0x02000000, (Gfx *)0x020000E0, (Gfx *)0x020001D0,
    (Gfx *)0x02000338, (Gfx *)0x02000428, (Gfx *)0x020004F0,
    (Gfx *)0x020008A0, (Gfx *)0x02000DD8, (Gfx *)0x02000F50,
    (Gfx *)0x02001178, (Gfx *)0x020012F0, (Gfx *)0x02001508,
    (Gfx *)0x02001760,
    (Gfx *)0x02000000, (Gfx *)0x020001C8, (Gfx *)0x02000378,
    (Gfx *)0x02000528, (Gfx *)0x020006D8, (Gfx *)0x02000888,
    (Gfx *)0x02000CD8, (Gfx *)0x02001148, (Gfx *)0x02001210,
    (Gfx *)0x020014D0, (Gfx *)0x02001590, (Gfx *)0x02001850,
    (Gfx *)0x02001A98,
    (Gfx *)0x02000000, (Gfx *)0x020000E0, (Gfx *)0x020002D0,
    (Gfx *)0x020004E0, (Gfx *)0x020006B0, (Gfx *)0x020008C0,
    (Gfx *)0x02000CE0, (Gfx *)0x02001298, (Gfx *)0x02001408,
    (Gfx *)0x02001608, (Gfx *)0x02001778, (Gfx *)0x02001978,
    (Gfx *)0x02001BA0,
    (Gfx *)0x02000000, (Gfx *)0x020002A0, (Gfx *)0x02000330,
    (Gfx *)0x02000518, (Gfx *)0x020005B8, (Gfx *)0x020007B0,
    (Gfx *)0x02000CF0, (Gfx *)0x020010E8, (Gfx *)0x02001170,
    (Gfx *)0x02001410, (Gfx *)0x02001498, (Gfx *)0x02001738,
    (Gfx *)0x020017C8,
};

s16 gMainMenuSceneModelPartInitDataA[] = {
    0, 0, 0, 0, 0x25, 0, -6, -2, 0, 0, -12, 0, 6, -2,
    0, 0, -12, 0, 0, 0, 0, 0, 0x10, 0, -10, 0xE, 0, -8, -15, 0,
    0xA, 0xE, 0, 8, -15, 0, 0, -12, -5, 0, -12, -5,
};

s16 gMainMenuSceneModelPartInitDataB[] = {
    0, 0, 0, 0, 0x2B, 0, -6, -5, 0, 0, -16, 0, 6, -5,
    0, 0, -16, 0, 0, 0, 0, 0, 9, 0, -10, 8, 0, -10, -16, 0,
    0xA, 8, 0, 0xA, -16, 0, 0, -10, -3, 0, -10, -3,
};

s16 gMainMenuSceneModelPartInitDataC[] = {
    0, 0, 0, 0, 0x33, 0, -8, -3, 0, 0, -18, -1, 8, -3,
    0, 1, -18, -1, 0, 5, -5, 0, 0x20, 2, -23, 0x17, 5, -11,
    -21, 0, 0x17, 0x17, 5, 0xB, -21, 0, -1, -19, -7, 2, -19, -7,
};

s16 *gMainMenuSceneModelPartInitDataByModel[] = {
    gMainMenuSceneModelPartInitDataA,
    gMainMenuSceneModelPartInitDataB,
    gMainMenuSceneModelPartInitDataA,
    gMainMenuSceneModelPartInitDataB,
    gMainMenuSceneModelPartInitDataC,
    gMainMenuSceneModelPartInitDataA,
    NULL,
};

extern Mtx *allocFixedTransformMatrix(MainMenuModelTransform *);
extern u8 gCurrentViewportIndex;
extern Gfx *gRegionAllocPtr;

void initMainMenuSceneModelRenderer(void) {
}

void initMainMenuSceneModelRenderer_pad(void) {
}

void drawMainMenuSceneModel(MainMenuSceneModel *arg0) {
    MainMenuModelTransform *transform;
    Mtx *matrix;
    s32 partIndex;
    s32 displayListCount;

    if ((u16)arg0->viewportIndex == gCurrentViewportIndex) {
        gDPPipeSync(gRegionAllocPtr++);
        gSPSegment(gRegionAllocPtr++, 0x02,
                   getRelocatableHeapBlockBase(
                       ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_GEOMETRY_HANDLE_BASE +
                                    (u16)arg0->sceneModelIndex)));
        gSPSegment(gRegionAllocPtr++, 0x03,
                   getRelocatableHeapBlockBase(
                       ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_TEXTURE_HANDLE_BASE +
                                    (u16)arg0->sceneModelIndex)));

        displayListCount = 13;
        partIndex = 1; transform = &arg0->displayObjects[1]; do {
            matrix = allocFixedTransformMatrix(transform);
            if (matrix != NULL) {
                gSPMatrix(gRegionAllocPtr++, matrix,
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(
                    gRegionAllocPtr++,
                    gMainMenuSceneModelPartDisplayLists[
                        ((u16)arg0->characterIndex * displayListCount) +
                        partIndex - 1]);
            }
            partIndex++;
            transform++;
        } while (partIndex != 14);

        /* Keep arg0 live through the loop exit for the original register allocation. */
        if (arg0 == NULL) {
        }
    }
}

void drawTexturedMainMenuSceneModel(MainMenuSceneModel *arg0) {
    MainMenuSceneModel *model;
    Gfx **displayLists;
    Mtx *matrix;
    s32 i;
    s32 stride;

    do {
        if ((u16)arg0->viewportIndex == gCurrentViewportIndex) {
            matrix = allocFixedTransformMatrix(arg0->displayObjects);
            model = arg0;
            if (matrix != NULL) {
                drawRacePlayerModelRootPart(matrix, model->textureId, model->paletteId);
            }

            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(
                gRegionAllocPtr++, 0x02,
                getRelocatableHeapBlockBase(
                    gAssetHandles[MAIN_MENU_SCENE_MODEL_GEOMETRY_HANDLE_BASE +
                                  (u16)model->sceneModelIndex]));
            gSPSegment(
                gRegionAllocPtr++, 0x03,
                getRelocatableHeapBlockBase(
                    gAssetHandles[MAIN_MENU_SCENE_MODEL_TEXTURE_HANDLE_BASE +
                                  (u16)model->sceneModelIndex]));

            stride = 13;
            displayLists = gMainMenuSceneModelPartDisplayLists;
            for (i = 1; i < 14; i++) {
                matrix = allocFixedTransformMatrix(&model->displayObjects[i]);
                if (matrix != NULL) {
                    gSPMatrix(gRegionAllocPtr++, matrix,
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                    gSPDisplayList(
                        gRegionAllocPtr++,
                        displayLists[
                            ((u16)model->characterIndex * stride) + i - 1]);
                }
            }
        }
    } while (0);
}

void addMainMenuSceneModelDrawCallback(s32 modelIndex) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(
        ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
    updateMainMenuSceneModelTransforms(model);
    model->viewportIndex = 0;
    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)drawMainMenuSceneModel, model);
}

void addMainMenuSceneModelTexturedDrawCallback(s32 modelIndex, s32 textureId, s32 paletteId) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(
        ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
    updateMainMenuSceneModelTransforms(model);
    model->viewportIndex = 0;
    model->textureId = (s16)textureId;
    model->paletteId = (s16)paletteId;
    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)drawTexturedMainMenuSceneModel, model);
}

void addMainMenuSceneModelTexturedDrawCallbackWithUnusedArg(s32 modelIndex, s32 textureId, s32 paletteId, s32 unusedArg) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(
        ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
    updateMainMenuSceneModelTransforms(model);
    model->viewportIndex = 0;
    model->textureId = (s16)textureId;
    model->paletteId = (s16)paletteId;
    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)drawTexturedMainMenuSceneModel, model);
}

void addMainMenuSceneModelDrawCallbackForViewport0(s32 modelIndex) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(
        ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
    updateMainMenuSceneModelTransforms(model);
    model->viewportIndex = 0;
    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)drawMainMenuSceneModel, model);
}

void addMainMenuSceneModelDrawCallbackForViewport(s32 modelIndex, s32 viewportIndex) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)getRelocatableHeapBlockBase(
        ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_HANDLE_BASE + modelIndex));
    updateMainMenuSceneModelTransforms(model);
    model->viewportIndex = (s16)viewportIndex;
    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)drawMainMenuSceneModel, model);
}

// initMainMenuSceneModelParts best match: 99.077% at nonmatchings/initMainMenuSceneModelParts-5802343343535905907/base_4.c.
#ifdef NON_MATCHING
typedef struct MainMenuInitPartPair {
    s32 word0;
    s32 word4;
    char pad8[0x2];
    s16 halfA;
    s16 halfC;
    s16 halfE;
    s32 word10;
    s32 word14;
    s32 word18;
    s16 half1C;
    s16 half1E;
    s16 half20;
    s16 half22;
    s32 word24;
} MainMenuInitPartPair;

void initMainMenuSceneModelParts(MainMenuSceneModel *model) {
    s16 *cursor;
    s32 i;
    s32 end;
    s16 zero;
    volatile MainMenuInitPartPair *writePart;

    model->parts[0].displayObjectIndex = -1;
    model->parts[1].displayObjectIndex = -1;
    model->parts[2].displayObjectIndex = 1;
    model->parts[3].displayObjectIndex = 2;
    model->parts[4].displayObjectIndex = 1;
    model->parts[5].displayObjectIndex = 4;
    model->parts[6].displayObjectIndex = 1;
    model->parts[7].displayObjectIndex = 6;
    model->parts[8].displayObjectIndex = 6;
    model->parts[9].displayObjectIndex = 8;
    model->parts[10].displayObjectIndex = 6;
    model->parts[11].displayObjectIndex = 10;
    model->parts[12].displayObjectIndex = 3;
    model->parts[13].displayObjectIndex = 5;

    cursor = gMainMenuSceneModelPartInitDataByModel[(u16)model->characterIndex];
    for (end = 14, i = 0, writePart = (MainMenuInitPartPair *)model;;) {
        writePart->half22 = 0;
        zero = writePart->half22;
        i += 2;
        writePart->half20 = zero;
        writePart->half1E = zero;
        writePart->word24 = cursor[0] << 16;
        cursor += 6;
        writePart++;
        writePart->word0 = cursor[-5] << 16;
        writePart->halfE = 0;
        zero = writePart->halfE;
        writePart->word4 = cursor[-4] << 16;
        writePart->halfC = zero;
        writePart->halfA = zero;
        writePart->word10 = cursor[-3] << 16;
        writePart->word14 = cursor[-2] << 16;
        writePart->word18 = cursor[-1] << 16;
        if (i == end) {
            break;
        }
    }
}
#endif
