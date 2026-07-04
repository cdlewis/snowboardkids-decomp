#include "main_menu_scene_model.h"

extern MainMenuSceneModel *func_80043040(s16);
extern void func_8004215C(MainMenuSceneModel *);
extern void func_800483FC(void *, void (*)(MainMenuSceneModel *), MainMenuSceneModel *);
extern s16 D_8011218A[];
extern s32 D_801248B0;
extern void func_80042574(MainMenuSceneModel *);
extern void func_8004270C(MainMenuSceneModel *);

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_renderer/func_80042560.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_renderer/func_80042574.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_renderer/func_8004270C.s")

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
