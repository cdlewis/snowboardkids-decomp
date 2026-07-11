#include "main_menu_scene_model.h"
#include "memory_allocator.h"
#include "main_menu_scene_renderer.h"
#include "race_position_ui.h"

extern void addRenderCallback(void *, void (*)(MainMenuSceneModel *), MainMenuSceneModel *);
extern s16 D_8011218A[];
extern s32 D_801248B0;
extern void func_80042574(MainMenuSceneModel *);
extern void func_8004270C(MainMenuSceneModel *);

// func_80042560 best match: 25.000% at nonmatchings/func_80042560-4839787584499344943/base_1.c.
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_renderer/func_80042560.s")

#ifdef NON_MATCHING
void func_80042560(void) {
}
#endif

// func_80042574 best source-tree match: 99.706% at nonmatchings/func_80042574-4139837607000619032/base_6.c.
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_renderer/func_80042574.s")

#ifdef NON_MATCHING
typedef struct MainMenuModelAssetHandles {
    u8 pad0[0x66];
    s16 modelAssetSlots[6];
    s16 animationAssetSlots[6];
} MainMenuModelAssetHandles;

extern Mtx *func_8004885C(MainMenuModelDisplayObject *);
extern Gfx *gRegionAllocPtr;
extern u8 gCurrentViewportIndex;
extern MainMenuModelAssetHandles D_80112130;
extern Gfx *D_800D3CB0[];

void func_80042574(MainMenuSceneModel *arg0) {
    MainMenuSceneModel *model;
    MainMenuModelDisplayObject *displayObject;
    Gfx **displayLists;
    Mtx *matrix;
    s32 i;
    s32 end;
    s32 stride;

 do { if ((u16)arg0->renderFrame == gCurrentViewportIndex) { model = arg0; gDPPipeSync(gRegionAllocPtr++); gSPSegment(gRegionAllocPtr++, 0x02, func_80043040(D_80112130.modelAssetSlots[(u16)model->actorIndex])); gSPSegment(gRegionAllocPtr++, 0x03, func_80043040(D_80112130.animationAssetSlots[(u16)model->actorIndex])); i = 1; displayObject = &model->displayObjects[1]; end = 14; stride = 13; displayLists = D_800D3CB0; do { matrix = func_8004885C(displayObject); if (matrix != NULL) { gSPMatrix(gRegionAllocPtr++, matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW); gSPDisplayList(gRegionAllocPtr++, displayLists[((u16)model->modelIndex * stride) + i - 1]); } i++; displayObject++; } while (i != end); } } while (0);
}
#endif

// func_8004270C best match: 99.595% at nonmatchings/func_8004270C-4139837607000619032/base.c.
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_renderer/func_8004270C.s")

#ifdef NON_MATCHING
void func_8004270C(MainMenuSceneModel *arg0) {
    MainMenuSceneModel *model;
    MainMenuModelDisplayObject *displayObject;
    Gfx **displayLists;
    Mtx *matrix;
    s32 i;
    s32 end;
    s32 stride;

    do { if ((u16)arg0->renderFrame == gCurrentViewportIndex) { matrix = func_8004885C(arg0->displayObjects); model = arg0; if (matrix != NULL) { func_8007C130(matrix, model->textureId, model->paletteId); } gDPPipeSync(gRegionAllocPtr++); gSPSegment(gRegionAllocPtr++, 0x02, func_80043040(D_80112130.modelAssetSlots[(u16)model->actorIndex])); gSPSegment(gRegionAllocPtr++, 0x03, func_80043040(D_80112130.animationAssetSlots[(u16)model->actorIndex])); i = 1; displayObject = &model->displayObjects[1]; end = 14; stride = 13; displayLists = D_800D3CB0; do { matrix = func_8004885C(displayObject); if (matrix != NULL) { gSPMatrix(gRegionAllocPtr++, matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW); gSPDisplayList(gRegionAllocPtr++, displayLists[((u16)model->modelIndex * stride) + i - 1]); } i++; displayObject++; } while (i != end); } } while (0);
}
#endif

void func_800428C8(s32 modelIndex) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)func_80043040(D_8011218A[modelIndex]);
    func_8004215C(model);
    model->renderFrame = 0;
    addRenderCallback(&D_801248B0, func_80042574, model);
}

void func_80042920(s32 modelIndex, s32 textureId, s32 paletteId) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)func_80043040(D_8011218A[modelIndex]);
    func_8004215C(model);
    model->renderFrame = 0;
    model->textureId = (s16)textureId;
    model->paletteId = (s16)paletteId;
    addRenderCallback(&D_801248B0, func_8004270C, model);
}

void func_8004298C(s32 modelIndex, s32 textureId, s32 paletteId, s32 arg3) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)func_80043040(D_8011218A[modelIndex]);
    func_8004215C(model);
    model->renderFrame = 0;
    model->textureId = (s16)textureId;
    model->paletteId = (s16)paletteId;
    addRenderCallback(&D_801248B0, func_8004270C, model);
}

void func_80042A00(s32 modelIndex) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)func_80043040(D_8011218A[modelIndex]);
    func_8004215C(model);
    model->renderFrame = 0;
    addRenderCallback(&D_801248B0, func_80042574, model);
}

void func_80042A58(s32 modelIndex, s32 renderFrame) {
    MainMenuSceneModel *model;

    model = (MainMenuSceneModel *)func_80043040(D_8011218A[modelIndex]);
    func_8004215C(model);
    model->renderFrame = (s16)renderFrame;
    addRenderCallback(&D_801248B0, func_80042574, model);
}

// func_80042AB4 best match: 99.077% at nonmatchings/func_80042AB4-2225551288923588688/base_10.c.
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_renderer/func_80042AB4.s")

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

extern s16 *D_800D3EE4[];

void func_80042AB4(MainMenuSceneModel *model) {
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

    cursor = D_800D3EE4[model->modelIndex];
    end = 14; i = 0; writePart = (MainMenuInitPartPair *)model; loop: writePart->half22 = 0;
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
    if (i != end) {
        goto loop;
    }
}
#endif
