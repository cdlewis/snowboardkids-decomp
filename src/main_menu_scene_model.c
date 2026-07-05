#include "main_menu_scene_model.h"

/* Frame offsets are halfword-relative to the bank start; this form preserves target addu order. */
#define MAIN_MENU_ANIMATION_FRAME_DATA(bank, index) \
    ((s16 *)(((bank)->frameOffsets[(index)] * sizeof(s16)) + (s32)(bank)))

extern MainMenuSceneModel *func_80043040(s16);
extern s16 D_8011218A[];
extern s16 D_801121AE;
extern void func_80041E90(MainMenuSceneModel *arg0);
extern void func_800437F0(void *, void *, s32);
extern u8 D_5E34A0;
extern u8 D_608560;
extern u8 D_215BE0;
extern u8 D_21D9D0;

typedef struct {
    /* 0x0 */ u8 *start;
    /* 0x4 */ u8 *end;
} RomAssetRange;

extern RomAssetRange D_800D4020[];
extern RomAssetRange D_800D4050[];
extern s16 D_80112130[];
extern s16 func_80042D58(s32);
extern void func_800438EC(s32, s32, s32);
extern void func_80042AB4(MainMenuSceneModel *arg0);

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_model/func_80040C80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_model/func_80040D94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_model/func_80041A20.s")

void func_80041CC0(void) {
    func_800437F0(&D_5E34A0, &D_608560, 0x3F);
}

void func_80041CF0(void) {
    func_800437F0(&D_215BE0, &D_21D9D0, 0x3F);
}

// func_80041D20 best match: 95.34% at nonmatchings/func_80041D20-3174110973063422312/base_5.c.
#ifdef NON_MATCHING
void func_80041D20(s32 actorIndex, s32 modelIndex) {
    MainMenuSceneModel *model;

    func_800438EC((s32) D_800D4020[modelIndex].start, (s32) D_800D4020[modelIndex].end, actorIndex + 0x33);
    func_800437F0(D_800D4050[modelIndex].start, D_800D4050[modelIndex].end, actorIndex + 0x39);
    D_80112130[actorIndex + 0x2D] = func_80042D58(0x308);
    model = func_80043040(D_80112130[actorIndex + 0x2D]);
    model->actorIndex = actorIndex;
    model->modelIndex = modelIndex;
    func_80042AB4(model);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_model/func_80041D20.s")
#endif

void func_80041DD4(s32 modelIndex, s32 animationIndex) {
    MainMenuModelAnimationBank *animationBank;
    s16 *frameData;
    MainMenuSceneModel *model;
    s16 frameDuration;

    animationBank = (MainMenuModelAnimationBank *)func_80043040(D_801121AE);
    frameData = MAIN_MENU_ANIMATION_FRAME_DATA(animationBank, animationIndex);
    model = func_80043040(D_8011218A[modelIndex]);
    frameDuration = *frameData++;
    model->framesRemaining = frameDuration;
    model->animationStart = frameData;
    model->framesRemaining++;
    model->animationCursor = frameData;
    model->frameDuration = frameDuration;
}

void func_80041E60(s32 modelIndex) {
    func_80043040(D_8011218A[modelIndex]);
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_model/func_80041E90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_model/func_80041FB4.s")

void func_80042034(s32 modelIndex) {
    MainMenuSceneModel *model;

    model = func_80043040(D_8011218A[modelIndex]);
    model->framesRemaining--;
    if (model->framesRemaining <= 0) {
        model->framesRemaining = model->frameDuration;
        model->animationCursor = model->animationStart;
    }
    func_80041E90(model);
}

void func_8004209C(s32 modelIndex, s32 x, s32 y, s32 z) {
    MainMenuSceneModel *model;

    model = func_80043040(D_8011218A[modelIndex]);
    model->pos.x = x;
    model->pos.y = y;
    model->pos.z = z;
}

void func_800420FC(s32 modelIndex, s16 x, s16 y, s16 z) {
    MainMenuSceneModel *model;

    model = func_80043040(D_8011218A[modelIndex]);
    model->rot.x = x;
    model->rot.y = y;
    model->rot.z = z;
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_scene_model/func_8004215C.s")
