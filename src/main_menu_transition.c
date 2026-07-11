#include "common.h"
#include "game_audio.h"
#include "effect_task_scheduler.h"
#include "asset_manager.h"
#include "course_asset_loader.h"
#include "race_input_history.h"
#include "input_task_scheduler.h"
#include "main_menu_message_ui.h"
#include "main_menu_overlay_effects.h"
#include "main_menu_panel_ui.h"
#include "race_camera.h"
#include "main_menu_transition.h"
#include "race_player_state.h"
#include "race_timer_ui.h"
#include "viewport_manager.h"

#define MAIN_MENU_TRANSITION_FRAMES 0x10

typedef struct {
    /* 0x00 */ char pad[0x18];
    /* 0x18 */ s32 transitionTimer;
    /* 0x1C */ s32 unk1C;
} MainMenuTransitionState;

typedef struct {
    /* 0x0 */ u8 courseIndex;
    /* 0x1 */ u8 pad1[3];
    /* 0x4 */ u8 *romStart;
    /* 0x8 */ u8 *romEnd;
} MainMenuCourseAsset;


extern u8 D_1467B0[];
extern u8 D_147910[];
extern u8 D_1DE360[];
extern u8 D_1E0F70[];
extern u8 D_1F1A90[];
extern u8 D_1F2220[];
extern u8 D_245A80[];
extern u8 D_24C8E0[];
extern MainMenuTransitionState *gCurrentInputTask;
extern u8 D_8010B1F0;
extern u8 D_8011228C;
extern s16 D_801124B8;
extern s16 D_80121B50;
extern s16 D_80121B52;
extern s8 gRacePlayerCount;
extern u8 D_80121B55;
extern u8 gRaceUpdatePaused;
extern s16 gMenuFadeAlpha;
extern u8 gFramebufferSwapHold;
extern s32 D_801235B4;
extern u8 D_80121B58;
extern u8 D_80121B59;
extern u8 D_80121B5B;
extern s16 D_80121B5C;
extern u8 D_80121B5E;
extern u8 D_80121B5F;
extern s16 D_800D3C90[];

extern void releaseMenuAssetHandles(void);
extern u8 gPendingFramebufferSwapCount;
extern s8 gFramebufferSwapDelay;
extern u8 D_80121B5A;
extern u8 D_800EC8B0;
extern u8 D_800EC9C2;
extern u8 D_80121D94;
extern u8 D_80121D95;
extern u8 D_80121D96;
extern u8 D_801223A0;
extern u8 D_801223A1;
extern u8 D_801223A2;
extern u8 D_801229AC;
extern u8 D_801229AD;
extern u8 D_801229AE;
extern u8 D_80122FB8;
extern u8 D_80122FB9;
extern u8 D_80122FBA;
extern MainMenuCourseAsset D_800D3C00[];
extern f32 D_800E10D4;
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_60F1A0[];
extern u8 D_60F990[];
extern s16 D_80122282;

void func_8003F520(void) {
    D_80121B5B = 1;
    setCurrentInputTaskCallback(func_8003F554, 0);
}

void func_8003F554(void) {
    loadCompressedRomAsset(D_1F1A90, D_1F2220, 0x28);
    loadCompressedRomAsset(D_593D10, D_598A70, 0x29);
    loadCompressedRomAsset(D_60F1A0, D_60F990, 0x2A);
    loadCompressedRomAsset(D_245A80, D_24C8E0, 0x1F);
    loadRawRomAsset(D_1467B0, D_147910, 8);
    loadCompressedRomAsset(D_1DE360, D_1E0F70, 9);
    D_8010B1F0 = 0;
    gFramebufferSwapDelay = 0;
    gMenuFadeAlpha = 0xFF;
    resetAllViewports();
    D_801124B8 = 0x80;
    func_80070EC0(0);
    func_80070C64(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    func_8006D5CC();
    func_8006D520(0, 0x1F);
    func_80071664(&func_80055678, 0, 0x64, 0);
    createEffectTask(&func_80051854, 0, 0x64);
    setCurrentInputTaskCallback(&func_8003F6C0, 0);
    func_800720E4(7);
}

void func_8003F6C0(void) {
    gMenuFadeAlpha -= 0x10;
    if (gMenuFadeAlpha <= 0) {
        gMenuFadeAlpha = 0;
        setCurrentInputTaskCallback(&func_8003F718, 0);
    }
    func_8006D780(0);
    updateEffectTasks();
}

void func_8003F718(void) {
    if (D_8010B1F0 != 0) {
        if (D_80121B5B == 0xC) {
            func_80072114(0x3C);
        }
        setCurrentInputTaskCallback(&func_8003F778, 0);
    }
    func_8006D780(0);
    updateEffectTasks();
}

void func_8003F778(void) {
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0xFF) {
        gMenuFadeAlpha = 0xFF;
        gFramebufferSwapHold = 1;
        setCurrentInputTaskCallback(&func_8003F7E4, 0);
    }
    func_8006D780(0);
    updateEffectTasks();
}

void func_8003F7E4(void) {
    if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay = 0;
        if (D_80121B5B != 0xC) {
            setCurrentInputTaskCallback(&func_8003F864, 0);
        } else {
            D_801235B4 = 0;
            resumeInputTask(3);
            removeInputTask(4);
        }
    }
}

// func_8003F864 best match: 94.795% (nonmatchings/func_8003F864-8662636370764828261/base_5.c)

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_transition/func_8003F864.s")

#ifdef NON_MATCHING
void func_8003F864(void) {
    RaceInputPlayer *players;
    MainMenuCourseAsset *courseAsset;

    D_80121B50 = D_800D3C00[D_80121B5B].courseIndex;
    gRaceUpdatePaused = 0;
    D_80121B58 = 0;
    D_80121B5F = 0;
    D_80121B59 = 0;
    D_80121B5A = 0;
    D_800EC9C2 = 0;
    D_80121B5E = 0;
    resetGameplayRng();

    players = D_80121D80;
    players[0].unk4 = 0;
    players[1].unk4 = 0;
    players[2].unk4 = 0;
    players[3].unk4 = 0;
    players[0].characterId = 0;
    players[1].characterId = 1;
    players[2].characterId = 2;
    players[3].characterId = 3;
    players[0].unk11 = 0;
    players[1].unk11 = 0;
    players[2].unk11 = 0;
    players[3].unk11 = 0;
    players[0].unk12 = 3;
    players[1].unk12 = 5;
    players[2].unk12 = 6;
    players[3].unk12 = 7;
    players[0].unk17 = 0;
    players[1].unk17 = 1;
    players[2].unk17 = 2;
    players[3].unk17 = 3;
    D_80121B55 = 4;
    players[0].isActive = 1;
    players[1].isActive = 1;
    players[2].isActive = 1;
    players[3].isActive = 1;
    gRacePlayerCount = 4;
    D_80121B52 = 5;
    D_80121B5C = 0x64;
    func_80070EC0(1);

    players[0].unk15 = 0;
    players[0].soundDisabled = 0;
    players[0].replayInputSource = 5;
    players[1].unk15 = 0;
    players[1].soundDisabled = 0;
    players[1].replayInputSource = 5;
    players[2].unk15 = 0;
    players[2].soundDisabled = 0;
    players[2].replayInputSource = 5;
    players[3].unk15 = 0;
    players[3].soundDisabled = 0;
    players[3].replayInputSource = 5;

    courseAsset = &D_800D3C00[D_80121B5B];
    loadCompressedRomAsset(courseAsset->romStart, courseAsset->romEnd, 0x2B);
    func_80043950();
    func_800440F4();
    func_8006D5CC();
    resetAllViewports();
    gFramebufferSwapDelay = 0;
    func_8008BEB0();
    D_80121B55 = 1;
    func_80078430();
    D_80121B55 = 4;
    configureViewport(0, 0xA0, 0x50, 0x108, 0x78, 0x140, 0x8C, D_800E10D4);
    D_8011228C = 1;
    gFramebufferSwapDelay = 0;
    func_80044294();
    gMenuFadeAlpha = 0xFF;
    D_800EC8B0 = 0;
    D_801235B4 = 1;
    loadCompressedRomAsset(D_593D10, D_598A70, 0x29);
    loadCompressedRomAsset(D_60F1A0, D_60F990, 0x2A);
    D_8010B1F0 = 0;
    createEffectTask(func_80051FDC, 0, 0x64);
    createEffectTask(func_800524B0, 0, 0x64);
    setCurrentInputTaskCallback(func_8003FB70, 0);
    func_800720E4(7);
}
#endif

void func_8003FB70(void) {
    gMenuFadeAlpha -= 0x10;
    if (gMenuFadeAlpha < 0) {
        gMenuFadeAlpha = 0;
        setCurrentInputTaskCallback(&func_8003FBE8, 0);
    }
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    D_801124B8 = 0x80;
}

void func_8003FBE8(void) {
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    if (D_8010B1F0 != 0) {
        gCurrentInputTask->transitionTimer = 0;
        func_80072114(0x20);
        setCurrentInputTaskCallback(&func_8003FC60, 0);
    }
    D_801124B8 = 0x80;
}

// func_8003FC60 best match: 75.250% (nonmatchings/func_8003FC60-1197934324348345530/base_3.c)

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_transition/func_8003FC60.s")

#ifdef NON_MATCHING
void func_8003FC60(void) {
    s32 timer;
    s32 x2;
    s32 x1;
    s32 y1;
    s32 y2;
    f64 denominator = MAIN_MENU_TRANSITION_FRAMES;
    f32 aspect;

    gCurrentInputTask->transitionTimer += 1;
    timer = gCurrentInputTask->transitionTimer;

    x2 = timer << 2;
    x2 -= timer;
    x2 <<= 3;
    x2 = (s16) ((x2 / MAIN_MENU_TRANSITION_FRAMES) + 0x108);

    x1 = (s16) (((timer * 0x28) / MAIN_MENU_TRANSITION_FRAMES) + 0x50);
    y1 = (s16) (((timer * 0x58) / MAIN_MENU_TRANSITION_FRAMES) + 0x78);
    y2 = (s16) (((timer * 0x64) / MAIN_MENU_TRANSITION_FRAMES) + 0x8C);
    aspect = (f32) ((((f64) timer * -0.9523809523809523) / denominator) + 2.2857142857142856);

    configureViewport(0, 0xA0, x1, x2 & 0xFFFF, y1, 0x140, y2, aspect);
    if (gCurrentInputTask->transitionTimer == MAIN_MENU_TRANSITION_FRAMES) {
        func_800720E4(0xF);
        configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        gCurrentInputTask->transitionTimer = 0;
        setCurrentInputTaskCallback(func_8003FE54, 0);
    }
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
}
#endif

void func_8003FE54(void) {
    D_801235B4 = 0;
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    func_8007AA50();
    gCurrentInputTask->transitionTimer += 1;
    if (gCurrentInputTask->transitionTimer == D_800D3C90[D_80121B5B]) {
        setCurrentInputTaskCallback(func_8003FEF4, 0);
        func_80072114(0x40);
    }
}

void func_8003FEF4(void) {
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    func_8007AA50();
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0xFF) {
        gFramebufferSwapHold = 1;
        {
            void (*func_ptr)(void) = func_8003FF78;
            setCurrentInputTaskCallback(func_ptr, 0);
        }
    }
}

void func_8003FF78(void) {
    if (gPendingFramebufferSwapCount == 2) {
        func_80072260();
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay = 0;
        setCurrentInputTaskCallback(&func_8003F554, 0);
    }
}

void func_8003FFD0(void) {
    D_80121B5A = 1;
    setCurrentInputTaskCallback(&func_80040004, 0);
}

void func_80040004(void) {
    setCurrentInputTaskCallback(&func_8004002C, 0);
}

// func_8004002C best match: 98.186% (nonmatchings/func_8004002C-6061209858023118177/base.c)

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_transition/func_8004002C.s")

#ifdef NON_MATCHING
void func_8004002C(void) {
    u64 characterId;

    switch (D_80121B5A) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            D_80121B50 = 0;
            break;
        default:
            D_80121B50 = 9;
            break;
    }

    gRaceUpdatePaused = 0;
    D_80121B58 = 0;
    D_80121B5F = 0;
    D_80121B59 = 0;
    D_80121B5B = 0;
    D_800EC9C2 = 0;
    D_80121B5E = 0;

    D_80121D80[0].unk4 = 0;
    D_80121D80[1].unk4 = 1;
    D_80121D80[2].unk4 = 1;
    D_80121D80[3].unk4 = 1;
    D_80121D80[0].characterId = (D_80121D80[0].unk11 = 0);
    D_80121D80[1].characterId = 1;
    characterId = 2;
    D_80121D80[2].characterId = characterId;
    D_80121D80[3].characterId = 3;
    D_80121D80[1].unk11 = 0;
    D_80121D80[2].unk11 = 0;
    D_80121D80[3].unk11 = 0;
    D_80121D80[0].unk12 = 3;
    D_80121D80[0].isActive = 1;
    D_80121D80[1].unk12 = 5;
    D_80121D80[2].unk12 = 6;
    if (!D_80121B5A) {
    }
    D_80121D80[3].unk12 = 7;
    D_80121D80[0].unk17 = 0;
    D_80121D80[1].unk17 = 1;
    D_80121D80[2].unk17 = 2;
    D_80121D80[3].unk17 = 3;
    D_80121B55 = 1;

    switch (D_80121B5A) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 9:
            D_80121D80[0].isActive = 1;
            D_80121D80[1].isActive = 0;
            D_80121D80[2].isActive = 0;
            D_80121D80[3].isActive = 0;
            gRacePlayerCount = 1;
            break;
        default:
            D_80121D80[0].isActive = 1;
            D_80121D80[1].isActive = 1;
            D_80121D80[2].isActive = 1;
            D_80121D80[3].isActive = 1;
            gRacePlayerCount = 4;
            break;
    }

    D_80121B52 = 5;
    D_80121B5C = 0x64;
    func_80070EC0(1);
    D_80121D95 = 0;
    D_80121D96 = 0;
    D_80121D94 = 0;
    D_801223A1 = 0;
    D_801223A2 = 0;
    D_801223A0 = 0;
    D_801229AD = 0;
    D_801229AE = 0;
    D_801229AC = 0;
    D_80122FB9 = 0;
    D_80122FBA = 0;
    D_80122FB8 = 0;
    func_80043950();
    func_800440F4();
    func_8006D5CC();
    resetAllViewports();
    D_8011228C = 1;
    gFramebufferSwapDelay = 0;
    func_8008BEB0();
    func_80078430();
    configureViewport(0, 0xA0, 0x50, 0x108, 0x78, 0x140, 0x8C, 2.285714388f);
    D_8011228C = 1;
    gFramebufferSwapDelay = 0;
    func_80044294();
    gMenuFadeAlpha = 0xFF;
    D_800EC8B0 = 0;
    D_801235B4 = 1;
    loadCompressedRomAsset(D_593D10, D_598A70, 0x29);
    loadCompressedRomAsset(D_60F1A0, D_60F990, 0x2A);
    D_8010B1F0 = 0;
    createEffectTask(func_8000E7CC, 0, 0x64);
    createEffectTask(func_8000EA44, 0, 0x63);
    createEffectTask(func_800524B0, 0, 0x64);
    setCurrentInputTaskCallback(func_80040360, 0);
    func_800720E4(7);
}
#endif

void func_80040360(void) {
    gMenuFadeAlpha -= 0x10;
    if (gMenuFadeAlpha < 0) {
        gMenuFadeAlpha = 0;
        setCurrentInputTaskCallback(func_800403D8, 0);
    }
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    D_801124B8 = 0x80;
}

void func_800403D8(void) {
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    if (D_8010B1F0 != 0) {
        gCurrentInputTask->transitionTimer = 0;
        func_80072114(0x20);
        setCurrentInputTaskCallback(&func_80040450, 0);
    }
    D_801124B8 = 0x80;
}

// func_80040450 best match: 74.802% (nonmatchings/func_80040450-1197934324348345530/base_8.c)

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_transition/func_80040450.s")

#ifdef NON_MATCHING
void func_80040450(void) {
    s32 timer;
    s32 x2;
    s32 x1;
    s32 y1;
    s32 y2;
    f64 denominator = MAIN_MENU_TRANSITION_FRAMES;
    f32 aspect;

    gCurrentInputTask->transitionTimer += 1;
    timer = gCurrentInputTask->transitionTimer;

    x2 = timer << 2;
    x2 -= timer;
    x2 <<= 3;
    x2 = (s16) ((x2 / MAIN_MENU_TRANSITION_FRAMES) + 0x108);

    x1 = (s16) (((timer * 0x28) / MAIN_MENU_TRANSITION_FRAMES) + 0x50);
    y1 = (s16) (((timer * 0x58) / MAIN_MENU_TRANSITION_FRAMES) + 0x78);
    y2 = (s16) (((timer * 0x64) / MAIN_MENU_TRANSITION_FRAMES) + 0x8C);
    aspect = (f32) ((((f64) timer * -0.9523809523809523) / denominator) + 2.2857142857142856);

    configureViewport(0, 0xA0, x1, x2 & 0xFFFF, y1, 0x140, y2, aspect);
    if (gCurrentInputTask->transitionTimer == MAIN_MENU_TRANSITION_FRAMES) {
        func_800720E4(0xF);
        configureViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
        setCurrentInputTaskCallback(func_80040638, 0);
    }
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
}
#endif

void func_80040638(void) {
    D_801235B4 = 0;
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    func_8007AA50();
    gCurrentInputTask->transitionTimer = 0;
    switch (D_80121B5A) {
        case 1:
            if (D_80122282 == 0x50) {
                setCurrentInputTaskCallback(func_800407AC, 0);
                func_80072114(0x40);
                return;
            }
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            if (D_80122282 == 0x9C) {
                setCurrentInputTaskCallback(func_800407AC, 0);
                func_80072114(0x40);
                return;
            }
            break;
        case 6:
            if (D_80122282 == 0xB4) {
                setCurrentInputTaskCallback(func_800407AC, 0);
                func_80072114(0x40);
                return;
            }
            break;
        case 7:
        case 8:
            if (D_80122282 == 0x16) {
                setCurrentInputTaskCallback(func_800407AC, 0);
                func_80072114(0x40);
                return;
            }
            break;
        case 9:
            if (D_80122282 == 0x36) {
                setCurrentInputTaskCallback(func_800407AC, 0);
                func_80072114(0x40);
            }
            break;
    }
}

void func_800407AC(void) {
    s32 temp_v1;

    gRaceUpdatePaused = 1;
    D_8010B1F0 = 0;
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    func_8007AA50();
    gCurrentInputTask->transitionTimer += 0x10;
    temp_v1 = gCurrentInputTask->transitionTimer;
    if (temp_v1 == 0x80) {
        createEffectTask(func_8000DF28, 0, 0x64);
        setCurrentInputTaskCallback(func_8004086C, 0);
    }
    temp_v1 = gCurrentInputTask->transitionTimer;
    D_801124B8 = temp_v1;
}

void func_8004086C(void) {
    gRaceUpdatePaused = 1;
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    func_8007AA50();
    D_801124B8 = 0x80;
    if (D_8010B1F0 != 0) {
        setCurrentInputTaskCallback(&func_800408E4, 0);
    }
}

void func_800408E4(void) {
    gRaceUpdatePaused = 1;
    func_8008C704();
    func_800710CC(0x63);
    func_80096E3C();
    func_8007115C();
    func_8006D700();
    func_8007AA50();
    D_801124B8 = 0x80;
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0xFF) {
        gFramebufferSwapHold = 1;
        setCurrentInputTaskCallback(&func_8004097C, 0);
    }
}

void func_8004097C(void) {
    if (gPendingFramebufferSwapCount == 2) {
        func_80072260();
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay = 0;
        if (D_8010B1F0 == 1) {
            if (D_80121B5A < 9) {
                D_80121B5A += 1;
            }
            setCurrentInputTaskCallback(func_80040004, 0);
        }
        if (D_8010B1F0 == 2) {
            setCurrentInputTaskCallback(func_8004002C, 0);
        }
        if (D_8010B1F0 == 3) {
            setCurrentInputTaskCallback(func_80040A48, 0);
        }
    }
}

void func_80040A48(void) {
    if (D_80121B5A != 9) {
        setCurrentInputTaskCallback(func_80040C44, 0);
        return;
    }
    loadCompressedRomAsset(D_593D10, D_598A70, 0x29);
    loadCompressedRomAsset(D_60F1A0, D_60F990, 0x2A);
    D_8010B1F0 = 0;
    gFramebufferSwapDelay = 0;
    gMenuFadeAlpha = 0xFF;
    resetAllViewports();
    func_80070EC0(0);
    createEffectTask(&func_8000E874, 0, 0x64);
    setCurrentInputTaskCallback(func_80040B04, 0);
}

void func_80040B04(void) {
    gMenuFadeAlpha -= 0x10;
    if (gMenuFadeAlpha <= 0) {
        gMenuFadeAlpha = 0;
        setCurrentInputTaskCallback(&func_80040B54, 0);
    }
    updateEffectTasks();
}

void func_80040B54(void) {
    if (D_8010B1F0 != 0) {
        setCurrentInputTaskCallback(&func_80040B90, 0);
    }
    updateEffectTasks();
}

void func_80040B90(void) {
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0xFF) {
        gMenuFadeAlpha = 0xFF;
        gFramebufferSwapHold = 1;
        setCurrentInputTaskCallback(&func_80040BF4, 0);
    }
    updateEffectTasks();
}

void func_80040BF4(void) {
    if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay = 0;
        setCurrentInputTaskCallback(&func_80040C44, 0);
    }
}

void func_80040C44(void) {
    D_801235B4 = 0;
    resumeInputTask(3);
    removeInputTask(4);
}
