#include "common.h"
#include "asset_manager.h"
#include "game_audio.h"
#include "callback_task_scheduler.h"
#include "fixed_point_math.h"
#include "input_task_scheduler.h"
#include "main_menu_overlay_effects.h"
#include "race_camera.h"
#include "main_menu_race_setup.h"
#include "viewport_manager.h"

typedef struct {
    s32 x;
    s32 y;
} Vec2i;

typedef s16 Matrix4s[0x10];

typedef struct {
    char pad[0x18];
    s32 fade;
} MainMenuRaceSetupState;

typedef struct {
    u8 pad0[0x35];
    u8 unk35;
    u8 unk36;
    u8 unk37;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    u8 pad3B[2];
    u8 unk3D;
    u8 pad3E[0xE];
    u8 unk4C;
} RaceSetupSaveData;

typedef struct {
    char pad0[0x4];
    s16 pitch;
    s16 yaw;
    char pad8[0x10];
    Vec3i pos;
    s32 depth;
    char pad28[0x8];
    char rotationMtx[0x14];
    Vec3i cameraDelta;
} MainMenuRaceSetupObject;

typedef struct {
    Matrix4s rotationMtx;
    Vec3i worldPos;
    Vec3i localPos;
} TransformScratch;

extern void releaseMenuAssetHandles(void);
extern s8 gFramebufferSwapDelay;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern s16 gMenuFadeAlpha;

extern MainMenuRaceSetupState *gCurrentInputTask;
extern MainMenuRaceSetupObject *D_800EC9C4;
extern Vec2i D_8010B1B0;

void func_8003DDD0() {
    TransformScratch scratch;
    MainMenuRaceSetupObject *obj;

    obj = D_800EC9C4;
    makeFixedRotationYX(obj->rotationMtx, -obj->pitch, -obj->yaw);
    obj = D_800EC9C4;
    scratch.localPos.x = 0;
    scratch.localPos.y = 0;
    scratch.localPos.z = -obj->depth;
    makeFixedRotationXY(scratch.rotationMtx, obj->pitch, obj->yaw);
    transformVec3iByFixedMatrix(scratch.rotationMtx, &scratch.localPos.x, &scratch.worldPos.x);
    obj = D_800EC9C4;
    obj->cameraDelta.x = scratch.worldPos.x - obj->pos.x;
    obj = D_800EC9C4;
    obj->cameraDelta.y = scratch.worldPos.y - obj->pos.y;
    obj = D_800EC9C4;
    obj->cameraDelta.z = scratch.worldPos.z - obj->pos.z;
}

void n_alSeqpDelete(struct ALSeqPlayer *seqp) {
    func_8003DDD0(seqp);
}

void func_8003DEC8(void) {
    TransformScratch scratch;
    MainMenuRaceSetupObject *obj;

    obj = D_800EC9C4;
    makeFixedRotationYX(obj->rotationMtx, -obj->pitch, -obj->yaw);
    obj = D_800EC9C4;
    scratch.localPos.x = D_8010B1B0.x;
    scratch.localPos.y = D_8010B1B0.y;
    scratch.localPos.z = -obj->depth;
    makeFixedRotationXY(scratch.rotationMtx, obj->pitch, obj->yaw);
    transformVec3iByFixedMatrix(scratch.rotationMtx, &scratch.localPos.x, &scratch.worldPos.x);
    obj = D_800EC9C4;
    obj->cameraDelta.x = scratch.worldPos.x - obj->pos.x;
    obj = D_800EC9C4;
    obj->cameraDelta.y = scratch.worldPos.y - obj->pos.y;
    obj = D_800EC9C4;
    obj->cameraDelta.z = scratch.worldPos.z - obj->pos.z;
}

void func_8003DFB0(void) {
    func_8003DEC8();
}

// func_8003DFD0 best match: 83.044% (nonmatchings/func_8003DFD0-4923837976568703863/base_4.c)
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_race_setup/func_8003DFD0.s")

#ifdef NON_MATCHING
extern void func_80041CF0();
extern void func_80053DD8(CallbackTask *);
extern void func_80053EBC(CallbackTask *);
extern void func_80054044(s32, s32);
extern void func_8005502C(CallbackTask *);
extern void func_800553E0(CallbackTask *);
extern void initCallbackTaskScheduler(s32);
extern void *createCallbackTask(void (*)(), s32, s32);
extern u8 D_13F3B0[];
extern u8 D_145380[];
extern u8 D_1467B0[];
extern u8 D_1D3070[];
extern u8 D_1D82B0[];
extern u8 D_1DCED0[];
extern u8 D_1DE360[];
extern u8 D_1E0F70[];
extern u8 D_1E19C0[];
extern u8 D_5DAF30[];
extern u8 D_5DB9D0[];
extern u8 D_800BB810[];
extern u8 D_800BB811[];
extern u8 D_800BB812[];
extern u8 D_800BB813[];
extern s8 D_800DC4C0;
extern u8 D_800EC9C2;
extern RaceSetupSaveData D_800EC9F0;
extern u8 D_800ECA24;
extern u8 D_80121B55;
extern u8 D_80121D90;

void func_8003DFD0(s32 arg0, RaceSetupSaveData *unused) {
    RaceSetupSaveData *save;
    s32 transition;
    s32 effectArg;
    s32 allReady;
    u8 state;

    transition = 0;
    if (D_800EC9C2 == 0) {
        arg0 = 1;
        if (D_80121B55 == 1) {
            save = &D_800EC9F0;
            state = save->unk4C;
            if ((state == 2) && (save->unk3A == 1)) {
                save->unk4C = 3;
                D_800DC4C0 = 1;
                state = save->unk4C;
                transition = 3;
            }
            if ((state == 1) && (save->unk39 == 1)) {
                transition = 2;
                save->unk4C = 2;
                state = 2;
            }
            if (state == 0) {
                if (save->unk3D == 1) {
                    allReady = 1;
                    if (D_800ECA24 != 1) {
                        allReady = 0;
                    }
                    if (save->unk35 != 1) {
                        allReady = 0;
                    }
                    if (save->unk36 != 1) {
                        allReady = 0;
                    }
                    if (save->unk37 != 1) {
                        allReady = 0;
                    }
                    if (save->unk38 != 1) {
                        allReady = 0;
                    }
                    if (allReady != 0) {
                        transition = 1;
                        save->unk4C = 1;
                    }
                }
            }
        }
    }

    if (transition == 0) {
        resumeInputTask(2);
        removeInputTask(4);
        return;
    }

    func_80041CF0(1, save, transition);
    loadCompressedRomAsset(D_5DAF30, D_5DB9D0, 0x2A);
    loadCompressedRomAsset(D_1E0F70, D_1E19C0, 0x22);
    loadRawRomAsset(D_145380, D_1467B0, 8);
    loadCompressedRomAsset(D_1DCED0, D_1DE360, 9);
    loadRawRomAsset(D_13F3B0, D_145380, 0xE);
    loadCompressedRomAsset(D_1D3070, D_1D82B0, 0x12);
    initCallbackTaskScheduler(2);
    func_8006D5CC();
    func_8006D580(0, 0x1D);
    func_8006D580(1, 0x1D);
    func_8006D580(2, 0x1D);
    resetAllViewports();
    configureViewport(0, 0xA0, 0x78, 0x120, 0xC0, 0x140, 0xF0, 1.333333373f);
    configureViewport(1, 0xA0, 0x78, 0x120, 0xC0, 0x140, 0xF0, 1.333333373f);
    configureViewport(2, 0xA0, 0x78, 0x120, 0xC0, 0x140, 0xF0, 1.333333373f);
    func_80070E90(2);
    gMenuFadeAlpha = 0xFF;
    gCurrentInputTask->fade = 5;
    effectArg = transition - 1;
    createCallbackTaskWithUserId(func_80053DD8, 0, 0x64, effectArg);
    createCallbackTask(func_8005502C, 0, 0x64);
    createCallbackTaskWithUserId(func_800553E0, 0, 0x64, effectArg);
    createCallbackTaskWithUserId(func_80053EBC, 0, 0x64, D_80121D90);
    func_80054044(1, D_800BB810[D_80121D90 * 4]);
    func_80054044(2, D_800BB811[D_80121D90 * 4]);
    func_80054044(3, D_800BB812[D_80121D90 * 4]);
    func_80054044(4, D_800BB813[D_80121D90 * 4]);
    setCurrentInputTaskCallback(func_8003E3AC, 0);
}
#endif

void func_8003E3AC(void) {
    gCurrentInputTask->fade--;
    if (gCurrentInputTask->fade == 0) {
        func_800720E4(4);
        gCurrentInputTask->fade = 0x12C;
        setCurrentInputTaskCallback(func_8003E45C, 0);
    }
    createCallbackTaskWithUserId(func_8005393C, 5, 0x64, 0);
    func_8006D780(0);
    func_8006D780(1);
    func_8006D780(2);
    updateCallbackTasks();
}

void func_8003E45C(void) {
    MainMenuRaceSetupState **state;
    MainMenuRaceSetupState *currentState;

    gMenuFadeAlpha -= 8;
    if (gMenuFadeAlpha < 0) {
        gMenuFadeAlpha = 0;
    }
    state = &gCurrentInputTask;
    currentState = *state;
    currentState->fade -= 1;
    if ((*state)->fade == 0) {
        func_80072114(0x7E);
        setCurrentInputTaskCallback(func_8003E514, 0);
    }
    createCallbackTaskWithUserId(func_8005393C, 5, 0x64, 0);
    func_8006D780(0);
    func_8006D780(1);
    updateCallbackTasks();
}

void func_8003E514(void) {
    gMenuFadeAlpha += 4;
    if (gMenuFadeAlpha >= 0xFF) {
        gCurrentInputTask->fade = 0xFF;
        gFramebufferSwapHold = 1;
        setCurrentInputTaskCallback(func_8003E5A8, 0);
    }
    createCallbackTaskWithUserId(func_8005393C, 5, 0x64, 0);
    func_8006D780(0);
    func_8006D780(1);
    updateCallbackTasks();
}

void func_8003E5A8(void) {
    if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay = 0;
        resumeInputTask(2);
        removeInputTask(4);
    }
}
