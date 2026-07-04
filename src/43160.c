#include "main_menu_scene_model.h"

extern MainMenuSceneModel *func_80043040(s16);
extern void func_8004215C(MainMenuSceneModel *);
extern void func_800483FC(void *, void *, void *);
extern s16 D_8011218A[];
extern s32 D_801248B0;
extern void func_80042574(void);
extern void func_8004270C(void);

#pragma GLOBAL_ASM("asm/nonmatchings/43160/func_80042560.s")

#pragma GLOBAL_ASM("asm/nonmatchings/43160/func_80042574.s")

#pragma GLOBAL_ASM("asm/nonmatchings/43160/func_8004270C.s")

void func_800428C8(s32 arg0) {
    MainMenuSceneModel *model;

    model = func_80043040(D_8011218A[arg0]);
    func_8004215C(model);
    model->renderFrame = 0;
    func_800483FC(&D_801248B0, func_80042574, model);
}

void func_80042920(s32 arg0, s32 arg1, s32 arg2) {
    MainMenuSceneModel *model;

    model = func_80043040(D_8011218A[arg0]);
    func_8004215C(model);
    model->renderFrame = 0;
    model->textureId = (s16)arg1;
    model->paletteId = (s16)arg2;
    func_800483FC(&D_801248B0, func_8004270C, model);
}

void func_8004298C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    MainMenuSceneModel *model;

    model = func_80043040(D_8011218A[arg0]);
    func_8004215C(model);
    model->renderFrame = 0;
    model->textureId = (s16)arg1;
    model->paletteId = (s16)arg2;
    func_800483FC(&D_801248B0, func_8004270C, model);
}

void func_80042A00(s32 arg0) {
    MainMenuSceneModel *model;

    model = func_80043040(D_8011218A[arg0]);
    func_8004215C(model);
    model->renderFrame = 0;
    func_800483FC(&D_801248B0, func_80042574, model);
}

void func_80042A58(s32 arg0, s32 arg1) {
    MainMenuSceneModel *model;

    model = func_80043040(D_8011218A[arg0]);
    func_8004215C(model);
    model->renderFrame = (s16)arg1;
    func_800483FC(&D_801248B0, func_80042574, model);
}

#pragma GLOBAL_ASM("asm/nonmatchings/43160/func_80042AB4.s")
