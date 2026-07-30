#include "game/menu/main_menu/main_menu_scene_model.h"
#include "game/engine/asset_manager.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/render_callback.h"
#include "game/menu/main_menu/main_menu_scene_model_renderer.h"
#include "game/race/player/race_player_model_renderer.h"

#define ASSET_HANDLE(index) (gAssetHandles[(index)])

extern void drawMainMenuSceneModel(MainMenuSceneModel *);
extern void drawTexturedMainMenuSceneModel(MainMenuSceneModel *);

void initMainMenuSceneModelRenderer(void) {
}

void initMainMenuSceneModelRenderer_pad(void) {
}

// drawMainMenuSceneModel best match: 99.706% at
// nonmatchings/drawMainMenuSceneModel-2641000770066553983/base_15.c.
#pragma GLOBAL_ASM("asm/nonmatchings/menu/main_menu/main_menu_scene_model_renderer/drawMainMenuSceneModel.s")

#ifdef NON_MATCHING
extern Mtx *allocFixedTransformMatrix(MainMenuModelTransform *);
extern Gfx *gRegionAllocPtr;
extern u8 gCurrentViewportIndex;
extern Gfx *gMainMenuSceneModelPartDisplayLists[];

void drawMainMenuSceneModel(MainMenuSceneModel *arg0) {
    MainMenuSceneModel *model;
    MainMenuModelTransform *displayObject;
    Gfx **displayLists;
    Mtx *matrix;
    s32 i;
    s32 end;
    s32 stride;
    u32 matrixParams;

    if ((u16)arg0->viewportIndex == gCurrentViewportIndex) {
        model = arg0;
        gDPPipeSync(gRegionAllocPtr++);
        gSPSegment(gRegionAllocPtr++, 0x02,
                   getRelocatableHeapBlockBase(
                       ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_GEOMETRY_HANDLE_BASE +
                                    (u16)model->sceneModelIndex)));
        gSPSegment(gRegionAllocPtr++, 0x03,
                   getRelocatableHeapBlockBase(
                       ASSET_HANDLE(MAIN_MENU_SCENE_MODEL_TEXTURE_HANDLE_BASE +
                                    (u16)model->sceneModelIndex)));
        stride = 13;
        matrixParams = G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW;
        displayLists = gMainMenuSceneModelPartDisplayLists;
        for (i = 1, displayObject = &model->displayObjects[1], end = 14;
             i != end;
             displayObject++, i++) {
            matrix = allocFixedTransformMatrix(displayObject);
            if (matrix != NULL) {
                gDma1p(gRegionAllocPtr++, G_MTX, matrix, sizeof(Mtx), matrixParams);
                gSPDisplayList(gRegionAllocPtr++,
                               displayLists[((u16)model->characterIndex * stride) + i - 1]);
            }
        }
    }
}
#endif

// drawTexturedMainMenuSceneModel best match: 99.595% at nonmatchings/drawTexturedMainMenuSceneModel-4139837607000619032/base.c.
#pragma GLOBAL_ASM("asm/nonmatchings/menu/main_menu/main_menu_scene_model_renderer/drawTexturedMainMenuSceneModel.s")

#ifdef NON_MATCHING
void drawTexturedMainMenuSceneModel(MainMenuSceneModel *arg0) {
    MainMenuSceneModel *model;
    MainMenuModelTransform *displayObject;
    Gfx **displayLists;
    Mtx *matrix;
    s32 i;
    s32 end;
    s32 stride;

    do { if ((u16)arg0->viewportIndex == gCurrentViewportIndex) { matrix = allocFixedTransformMatrix(arg0->displayObjects); model = arg0; if (matrix != NULL) { drawRacePlayerModelRootPart(matrix, model->textureId, model->paletteId); } gDPPipeSync(gRegionAllocPtr++); gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(gAssetHandles[MAIN_MENU_SCENE_MODEL_GEOMETRY_HANDLE_BASE + (u16)model->sceneModelIndex])); gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(gAssetHandles[MAIN_MENU_SCENE_MODEL_TEXTURE_HANDLE_BASE + (u16)model->sceneModelIndex])); i = 1; displayObject = &model->displayObjects[1]; end = 14; stride = 13; displayLists = gMainMenuSceneModelPartDisplayLists; do { matrix = allocFixedTransformMatrix(displayObject); if (matrix != NULL) { gSPMatrix(gRegionAllocPtr++, matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW); gSPDisplayList(gRegionAllocPtr++, displayLists[((u16)model->characterIndex * stride) + i - 1]); } i++; displayObject++; } while (i != end); } } while (0);
}
#endif

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

extern s16 *gMainMenuSceneModelPartInitDataByModel[];

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
