#include "main_menu_scene_model.h"
#include "main_menu_scene_renderer.h"

extern MainMenuSceneModel *func_80043040(s16);
extern void func_8004215C(MainMenuSceneModel *);
extern void func_800483FC(void *, void (*)(MainMenuSceneModel *), MainMenuSceneModel *);
extern s16 D_8011218A[];
extern s32 D_801248B0;
extern void func_80042574(MainMenuSceneModel *);
extern void func_8004270C(MainMenuSceneModel *);

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_renderer/func_80042560.s")

// func_80042574 best source-tree match: 99.706% at nonmatchings/func_80042574-4139837607000619032/base_6.c.
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_renderer/func_80042574.s")

#ifdef NON_MATCHING
typedef struct MainMenuModelAssetHandles {
    u8 pad0[0x66];
    s16 modelAssetSlots[6];
    s16 animationAssetSlots[6];
} MainMenuModelAssetHandles;

extern Mtx *func_8004885C(MainMenuModelDisplayObject *);
extern void func_8007C130(Mtx *, s16, s16);
extern Gfx *gRegionAllocPtr;
extern u8 D_80156608;
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

 do { if ((u16)arg0->renderFrame == D_80156608) { model = arg0; gDPPipeSync(gRegionAllocPtr++); gSPSegment(gRegionAllocPtr++, 0x02, func_80043040(D_80112130.modelAssetSlots[(u16)model->actorIndex])); gSPSegment(gRegionAllocPtr++, 0x03, func_80043040(D_80112130.animationAssetSlots[(u16)model->actorIndex])); i = 1; displayObject = &model->displayObjects[1]; end = 14; stride = 13; displayLists = D_800D3CB0; do { matrix = func_8004885C(displayObject); if (matrix != NULL) { gSPMatrix(gRegionAllocPtr++, matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW); gSPDisplayList(gRegionAllocPtr++, displayLists[((u16)model->modelIndex * stride) + i - 1]); } i++; displayObject++; } while (i != end); } } while (0);
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

    do { if ((u16)arg0->renderFrame == D_80156608) { matrix = func_8004885C(arg0->displayObjects); model = arg0; if (matrix != NULL) { func_8007C130(matrix, model->textureId, model->paletteId); } gDPPipeSync(gRegionAllocPtr++); gSPSegment(gRegionAllocPtr++, 0x02, func_80043040(D_80112130.modelAssetSlots[(u16)model->actorIndex])); gSPSegment(gRegionAllocPtr++, 0x03, func_80043040(D_80112130.animationAssetSlots[(u16)model->actorIndex])); i = 1; displayObject = &model->displayObjects[1]; end = 14; stride = 13; displayLists = D_800D3CB0; do { matrix = func_8004885C(displayObject); if (matrix != NULL) { gSPMatrix(gRegionAllocPtr++, matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW); gSPDisplayList(gRegionAllocPtr++, displayLists[((u16)model->modelIndex * stride) + i - 1]); } i++; displayObject++; } while (i != end); } } while (0);
}
#endif

void func_800428C8(s32 modelIndex) {
    MainMenuSceneModel *model;

    model = func_80043040(D_8011218A[modelIndex]);
    func_8004215C(model);
    model->renderFrame = 0;
    func_800483FC(&D_801248B0, func_80042574, model);
}

void func_80042920(s32 modelIndex, s32 textureId, s32 paletteId) {
    MainMenuSceneModel *model;

    model = func_80043040(D_8011218A[modelIndex]);
    func_8004215C(model);
    model->renderFrame = 0;
    model->textureId = (s16)textureId;
    model->paletteId = (s16)paletteId;
    func_800483FC(&D_801248B0, func_8004270C, model);
}

void func_8004298C(s32 modelIndex, s32 textureId, s32 paletteId, s32 arg3) {
    MainMenuSceneModel *model;

    model = func_80043040(D_8011218A[modelIndex]);
    func_8004215C(model);
    model->renderFrame = 0;
    model->textureId = (s16)textureId;
    model->paletteId = (s16)paletteId;
    func_800483FC(&D_801248B0, func_8004270C, model);
}

void func_80042A00(s32 modelIndex) {
    MainMenuSceneModel *model;

    model = func_80043040(D_8011218A[modelIndex]);
    func_8004215C(model);
    model->renderFrame = 0;
    func_800483FC(&D_801248B0, func_80042574, model);
}

void func_80042A58(s32 modelIndex, s32 renderFrame) {
    MainMenuSceneModel *model;

    model = func_80043040(D_8011218A[modelIndex]);
    func_8004215C(model);
    model->renderFrame = (s16)renderFrame;
    func_800483FC(&D_801248B0, func_80042574, model);
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_renderer/func_80042AB4.s")
