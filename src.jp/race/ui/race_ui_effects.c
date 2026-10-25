#include "common.h"
#include "game/engine/model_asset_symbols.h"
#include "game/race/course/race_course_effects.h"
#include "game/engine/game_task_scheduler.h"
#include "game/race/race_state.h"
#include <PR/os_libc.h>
#include "game/save_data.h"
#include "game/audio/sound_manager.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/engine/render_callback.h"
#include "game/engine/system_runtime.h"
#include "game/engine/controller_input.h"
#include "assets.h"
#include "game/race/ui/race_ui_effects.h"
#include "game/race/effects/snowboard_trail_effects.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/race/items/race_item_hit_flags.h"
#include "game/engine/viewport_manager.h"
#include "game/math/spatial_math.h"
#include "game/math/fixed_point_math.h"
#include "game/race/motion/race_motion.h"
#include "game/race/items/race_projectiles.h"
#include "game/race/player/race_player_movement.h"
#include "game/race/player/race_player_input.h"
#include "game/race/camera/race_camera.h"
#include "game/race/ui/race_hud.h"
#include <PR/os_cache.h>
#include "game/race/flow/race_flow.h"

#define RACE_UI_HIT_PRIZE_SCORE_RATE 0x12C
#define RACE_UI_HIT_PRIZE_QUICKSAND_VALLEY_SCORE_RATE 0x64
#define RACE_UI_HIT_PRIZE_PERFECT_HIT_BONUS 0x3E8
#define ASSET_HANDLE(index) (gAssetHandles[(index)])
#define RACE_UI_PRIZE_PAYOUT_REVEAL_TIMER 0x14
#define RACE_UI_HIT_PRIZE_SHOW_TOTAL_MONEY 3
#define RACE_UI_RESULTS_REVEAL_TIMER 0x14
#define RACE_UI_HIT_PRIZE_SHOW_COMPLETE_BONUS 2
#define RACE_UI_HIT_PRIZE_SHOW_PERFECT_HIT 1
#define RACE_UI_HIT_PRIZE_SHOW_HIT_PRIZE 0
#define RACE_UI_RESULTS_FADE_STEP 0x10
#define RACE_UI_RESULTS_FULL_ALPHA 0xFF
#define RACE_UI_COURSE_RECORD_HOLD_FINAL_MONEY_STATE 4
#define RACE_UI_COURSE_RECORD_FINAL_MONEY_STATE 3
#define RACE_UI_COURSE_RECORD_REVEAL_PERFECT_GET_STATE 2
#define RACE_UI_COURSE_RECORD_REVEAL_SPEED_FAN_GET_STATE 1
#define RACE_UI_COURSE_RECORD_REVEAL_TIME_PRIZE_STATE 0
#define RACE_UI_TRICK_PRIZE_SHOW_TOTAL_MONEY 3
#define RACE_UI_TRICK_PRIZE_REVEAL_DELAY 0x14
#define RACE_UI_TRICK_PRIZE_SHOW_COMPLETE_BONUS 2
#define RACE_UI_TRAIL_GFX_ALLOC_PTR (*(RaceUiDisplayCommand **)&gRegionAllocPtr)
#define RACE_UI_SNOWBOARD_TRAIL_BACK_OFFSET_X 0xF0000
#define RACE_UI_SNOWBOARD_TRAIL_BACK_OFFSET_Y 0x230000
#define RACE_UI_SNOWBOARD_TRAIL_BACK_OFFSET_Z 0
#define RACE_UI_SNOWBOARD_TRAIL_TIMER 0xF
#define RACE_UI_SNOWBOARD_TRAIL_INITIAL_VELOCITY_Y 0x20000
#define RACE_UI_SINGLE_TRAIL_TIMER 0x3C
#define RACE_UI_SINGLE_TRAIL_LOCAL_ROT_Y 0x800
#define RACE_UI_SINGLE_TRAIL_LOCAL_X 0
#define RACE_UI_SINGLE_TRAIL_LOCAL_Y 0x100000
#define RACE_UI_SINGLE_TRAIL_LOCAL_Z -0x200000
#define RACE_UI_STUN_ORBIT_ANGLE_0 0
#define RACE_UI_STUN_ORBIT_ANGLE_1 0x555
#define RACE_UI_STUN_ORBIT_ANGLE_2 0xAAA
#define SCALE_MATRIX_COMPONENT(value, scale) ((value * scale) / 0x1000)
#define COURSE_START_FINISH_SCROLL_FRAME_MASK 7
#define COURSE_START_FINISH_SCROLL_STEP 4
#define COURSE_START_FINISH_SCROLL_MASK 0x3F
#define TIME_TRIAL_RECORD_DELTA_POPUP_HOLD_TIMER 0x5A
#define TIME_TRIAL_RECORD_DELTA_POPUP_START_X 0x1A4
#define TIME_TRIAL_RECORD_DELTA_POPUP_START_Y -0x2C
#define TIME_TRIAL_RECORD_DELTA_POPUP_START_VELOCITY 0x38
#define RACE_UI_TIME_TRIAL_SAVE_DATA ((RaceUiTimeTrialSaveData *)&gGameSaveDataBuffer[0])

typedef struct {
    u8 pad0[0x7756];
    u16 values[10][5];
} RaceUiTrickValueData;

typedef struct {
    u8 pad0[0x77C4];
    u8 icons[10][5];
} RaceUiTrickIconData;

typedef struct {
    u8 pad0[0x7832];
    u8 values[10][5];
    u8 pad7864[0x7869 - 0x7864];
    u8 icons[10][5];
} RaceUiCourseValueData;

typedef struct {
    u8 pad30[4];
    Vec3i vec;
    Mat3x3 matrix;
    u8 pad52[4];
} RaceUiSparkleRetargetScratch;

typedef struct {
    u8 pad30[4];
    Vec3i vec;
    Mat3x3 matrix;
    u8 pad52[0xC];
} RaceUiSparkleTransformScratch;

typedef struct {
    u8 pad0[0x4E];
    RaceTimer courseStatsNames[10][5];
} RaceUiCourseStatsNameData;

typedef struct {
    u8 pad0[0x77FB];
    u8 courseStatsIcons[10][5];
} RaceUiCourseStatsIconData;

typedef struct {
    u8 pad0[0x156];
    RaceTimer resultNames[10][5];
} RaceUiResultNameData;

typedef struct {
    u8 pad0[0x78A0];
    u8 resultIcons[10][5];
} RaceUiResultIconData;

typedef struct {
    struct {
        u32 w0;
        u32 w1;
    } words;
} RaceUiDisplayCommand;

typedef struct {
    u8 pad0[0x12A];
    RaceTimer timeTrialRecordSplitTimes[11];
} RaceUiTimeTrialSaveData;

extern const char D_800E14D0[];

void renderRaceCourseScrollingTexture(RaceCourseScrollingTextureActor *arg0) {
    volatile u8 pad[8];
    Gfx *unused;

    gDPPipeSync(gRegionAllocPtr++);
    gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0x8)));
    gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0x9)));
    gSPMatrix(gRegionAllocPtr++, &gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPLoadTextureBlock_4b(
        gRegionAllocPtr++,
        arg0->texture,
        G_IM_FMT_CI,
        0x20,
        0x40,
        0,
        G_TX_WRAP,
        G_TX_WRAP,
        5,
        6,
        G_TX_NOLOD,
        G_TX_NOLOD
    );

    gDPSetTileSize(
        gRegionAllocPtr++,
        G_TX_RENDERTILE,
        0,
        arg0->textureScrollOffset << G_TEXTURE_IMAGE_FRAC,
        0,
        (arg0->textureScrollOffset + 0x40) << G_TEXTURE_IMAGE_FRAC
    );

    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette);
    gSPDisplayList(gRegionAllocPtr++, arg0->displayList);
}

void updateRaceCourseScrollingTexture(RaceCourseScrollingTextureActor *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->soundSourceIndex;
    arg0->textureScrollOffset = arg0->textureScrollOffset + 2;
    arg0->textureScrollOffset = arg0->textureScrollOffset & 0x3F;
    if (temp_v0 != 0) {
        enqueuePlayerLoopingPositionalSoundRequest(
            SFX_SUNSET_ROCK_AMBIENCE,
            &D_800D6030[temp_v0],
            0x7F,
            SFX_PRIORITY_NORMAL,
            0.0f,
            0xA
        );
    }
    if (arg0->useOverlayQueue != 0) {
        addRenderCallback(&gModelRenderCallbackQueue7Head, (RenderCallback)renderRaceCourseScrollingTexture, arg0);
        return;
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderRaceCourseScrollingTexture, arg0);
}

void initRaceCourseScrollingTexture(RaceCourseScrollingTextureActor *arg0) {
    arg0->textureScrollOffset = 0;
    getAssetTableImageAndPalette(
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)),
        gRaceCourseScrollingTextureInit[arg0->task.userId].textureAssetId,
        &arg0->texture,
        &arg0->palette
    );
    arg0->displayList = (Gfx *)gRaceCourseScrollingTextureInit[arg0->task.userId].displayListAddress;
    arg0->useOverlayQueue = gRaceCourseScrollingTextureInit[arg0->task.userId].useOverlayQueue;
    arg0->soundSourceIndex = gRaceCourseScrollingTextureInit[arg0->task.userId].soundSourceIndex;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceCourseScrollingTexture);
}

void drawRaceUiBoardReversePrompt(RaceUiPromptActor *arg0) {
    if (gCurrentViewportIndex == arg0->task.userId) {
        if (gUiBlinkTimer & 1) {
            drawMenuAsciiTextDefaultScale(-0x34, arg0->y, gRaceUiBoardReversePromptLabelBlinkOn, 0);
            return;
        }
        drawMenuAsciiTextDefaultScale(-0x34, arg0->y, gRaceUiBoardReversePromptLabelBlinkOff, 1);
    }
}

void updateRaceUiBoardReversePrompt(RaceUiPromptActor *arg0) {
    if (gRacePlayers[arg0->task.userId].state.stateFlags & 0x400) {
        if (arg0->timer < 0x1E) {
            arg0->timer++;
        }
    } else {
        arg0->timer = 0;
    }
    if (gRacePlayers[arg0->task.userId].state.stateFlags & 0x1040) {
        arg0->timer = 0;
    }
    if (arg0->timer >= 0x1E) {
        addRenderCallback(
            &gRaceForegroundRenderCallbackList,
            (RenderCallback)drawRaceUiBoardReversePrompt,
            (void *)arg0
        );
    }
}

void initRaceUiBoardReversePrompt(RaceUiPromptActor *arg0) {
    arg0->y = -0x10;
    if (gPlayerCount != 1) {
        arg0->y = -8;
    }
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiBoardReversePrompt);
}

void drawRaceUiTrickScorePopup(RaceUiPopupActor *arg0) {
    volatile u8 padding[0x20];
    char buffer[8];
    s32 i;

    if (gCurrentViewportIndex == 0) {
        drawAssetTableSprite(
            (s16)(arg0->x - 0x3C),
            arg0->y.half.lo,
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            0x8B
        );
        drawAssetTableSprite(
            (s16)(arg0->x + 0x14),
            arg0->y.half.lo,
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            0x8E
        );
        sprintf(buffer, gRaceUiTrickScorePopupPointsFormat, arg0->parent);
        for (i = 0; i < 2; i++) {
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWithExplicitPalette(
                    (s16)(arg0->x + (i * 8) + 4),
                    arg0->y.half.lo,
                    getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                    buffer[i] - 5,
                    0xE
                );
            }
        }

        if (arg0->playerIndex != 0) {
            drawAssetTableSprite(
                (s16)(arg0->x - 0x48),
                (s16)(arg0->y.word + 0x12),
                getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                0x8D
            );
            drawAssetTableSprite(
                (s16)(arg0->x + 0x20),
                (s16)(arg0->y.word + 0x12),
                getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                0x8E
            );
            sprintf(buffer, gRaceUiTrickScorePopupBonusFormat, 0x12C);
            for (i = 0; i != 3; i++) {
                if (buffer[i] != ' ') {
                    drawAssetTableSpriteWithExplicitPalette(
                        (s16)(arg0->x + (i * 8) + 8),
                        (s16)(arg0->y.word + 0x12),
                        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                        buffer[i] - 5,
                        0xE
                    );
                }
            }
        }
    }
}

void updateRaceUiTrickScorePopupSlideOut(RaceUiPopupActor *arg0) {
    arg0->x += arg0->velocity;
    arg0->velocity += 4;
    if (arg0->velocity == 0x38) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceUiTrickScorePopup, arg0);
    }
}

void updateRaceUiTrickScorePopupHold(RaceUiPopupActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickScorePopupSlideOut);
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceUiTrickScorePopup, arg0);
}

void updateRaceUiTrickScorePopupSlideIn(RaceUiPopupActor *arg0) {
    arg0->x -= arg0->velocity;
    arg0->velocity -= 4;
    if (arg0->velocity == 0) {
        arg0->timer = 0x2D;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickScorePopupHold);
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceUiTrickScorePopup, arg0);
}

void initRaceUiTrickScorePopup(RaceUiPopupActor *arg0) {
    arg0->y.word = -0x2C;
    arg0->velocity = 0x38;
    arg0->x = 0x1A4;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickScorePopupSlideIn);
}

void spawnRaceUiTrickScorePopup(void *arg0, s16 arg1) {
    RaceUiPopupActor *temp = createCallbackTask((CallbackTaskCallback)initRaceUiTrickScorePopup, 0, 0x64);
    if (temp != NULL) {
        temp->parent = arg0;
        temp->playerIndex = arg1;
    }
}

void drawRaceUiCrashScorePopup(RaceUiPopupActor *arg0) {
    if (gCurrentViewportIndex == 0) {
        drawAssetTableSprite(-0x3C, arg0->y.half.lo, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), 0x8F);
    }
}

void updateRaceUiCrashScorePopupSlideOut(RaceUiPopupActor *arg0) {
    arg0->x += arg0->velocity;
    arg0->velocity += 4;
    if (arg0->velocity == 0x38) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceUiCrashScorePopup, arg0);
    }
}

void updateRaceUiCrashScorePopupHold(RaceUiPopupActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiCrashScorePopupSlideOut);
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceUiCrashScorePopup, arg0);
}

void updateRaceUiCrashScorePopupSlideIn(RaceUiPopupActor *arg0) {
    arg0->x -= arg0->velocity;
    arg0->velocity -= 4;
    if (arg0->velocity == 0) {
        arg0->timer = 0x2D;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiCrashScorePopupHold);
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceUiCrashScorePopup, arg0);
}

void initRaceUiCrashScorePopup(RaceUiPopupActor *arg0) {
    arg0->y.word = 0xC;
    arg0->velocity = 0x38;
    arg0->x = 0x1A4;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiCrashScorePopupSlideIn);
}

void drawRaceUiPrizePayoutContinuePrompt(void *arg0) {
    if (gUiBlinkTimer & 8) {
        drawAssetTableSprite(0x20, 0x4A, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), 0x5B);
    }
}

void updateRaceUiPrizePayoutContinuePrompt(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, drawRaceUiPrizePayoutContinuePrompt, arg0);
}

void drawRaceUiPrizePayoutRankPanel(RaceUiAlpha18Actor *arg0) {
    if (gRacePlayers[0].state.rankIndex != 0) {
        if (arg0->alpha != 0xFF) {
            gDPPipeSync(gRegionAllocPtr++);
            gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
            gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
            gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 255, 255, 255, arg0->alpha);
        }
        drawAssetTableSprite(-0x68, -0x2C, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), 0x4D);
        drawAssetTableSprite(-0x42, -0xC, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), gRacePlayers[0].state.rankIndex);
        drawAssetTableSprite(
            -0x22,
            4,
            getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)),
            gRacePlayers[0].state.rankIndex + 0x48
        );
        drawAssetTableSprite(
            -0x68,
            6,
            getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)),
            gRacePlayers[0].characterId + 0x91
        );
        if (arg0->alpha != 0xFF) {
            gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
        }
    }
}

void drawRaceUiPrizePayoutRankPrize(RaceUiAlpha18Actor *arg0) {
    volatile u8 padding[0x18];
    char buffer[8];
    s32 x;
    u8 *ptr;
    volatile s16 *handles;
    s32 space;

    sprintf(buffer + -8, gRaceUiRankPrizeCounterFormat, arg0->score);
    x = 0x20;
    handles = gAssetHandles;
    // clang-format off
 ptr = (u8 *) (((s32) padding) - 0x10); if (!ptr) { } space = ' '; loop: if ((*ptr) != 0) {
        // clang-format on
        if (space != *ptr) {
            drawAssetTableSprite(x, -0x3F, getRelocatableHeapBlockBase(handles[0x1F]), *ptr - 5);
        }
        x += 8;
        ptr++;
        goto loop;
    }

    drawAssetTableSprite(x, -0x3F, getRelocatableHeapBlockBase(handles[0x1F]), 0x37);
}

void drawRaceUiPrizePayoutCollectedCoins(void *arg0) {
    s32 x;
    s32 i;
    s32 space;
    s32 colorValue;
    u16 color;
    s16 *assets;
    char buffer[0x14];

    i = 0;
    do {
        sprintf(&buffer[i], gRaceUiCoinCounterFormat, gRacePlayers[0].state.score);
        x = 0x20;
        if (gRacePlayers[0].state.rankIndex == 3) {
            // clang-format off
            colorValue = 0xD; \
        } else { \
            colorValue = 0xC; \
        } \
        do { \
            assets = &gAssetHandles[i]; \
            space = ' '; \
            color = colorValue; \
            if (space != 0U) {} \
        loop: \
            if (buffer[i] != '\0') { \
                if (buffer[i] != space) { \
                    drawAssetTableSpriteWithExplicitPalette( \
                        (s16)x, \
                        -0xF, \
                        getRelocatableHeapBlockBase(assets[0x1F]), \
                        buffer[i] - 5, \
                        color \
                    ); \
                }
                // clang-format on
                x += 8;
                i++;
                goto loop;
            }
        } while (0);
        drawAssetTableSpriteWithExplicitPalette((s16)x, -0xF, getRelocatableHeapBlockBase(assets[0x1F]), 0x37, color);
    } while (0);
    if (gRacePlayers[0].state.rankIndex == 3) {
        drawAssetTableSprite(0x18, -0xB, getRelocatableHeapBlockBase(assets[0x1F]), 0x4E);
    }
}

void drawRaceUiPrizePayoutTotalMoney(void *arg0) {
    volatile u8 padding[0x18];
    char buffer[8];
    s32 x;
    u8 *ptr;
    volatile s16 *handles;
    s32 space;

    sprintf(buffer + -8, gRaceUiTotalMoneyCounterFormat, gRacePlayers[0].money);
    x = 0x18;
    handles = gAssetHandles;
    // clang-format off
 ptr = (u8 *) (((s32) padding) - 0x10); if (!ptr) { } space = ' '; loop: if ((*ptr) != 0) {
        // clang-format on
        if (space != *ptr) {
            drawAssetTableSprite(x, 0x21, getRelocatableHeapBlockBase(handles[0x1F]), *ptr - 5);
        }
        x += 8;
        ptr++;
        goto loop;
    }

    drawAssetTableSprite(x, 0x21, getRelocatableHeapBlockBase(handles[0x1F]), 0x37);
}

void drawRaceUiPrizePayoutLabels(RaceUiAlpha18Actor *arg0) {
    if (arg0->alpha != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->alpha);
    }
    drawMenuAsciiTextDefaultScale(8, -0x48, gRaceUiRankPrizeLabel, 5);
    drawMenuAsciiTextDefaultScale(0, -0x18, gRaceUiGetCoinLabel, 5);
    drawMenuAsciiTextDefaultScale(4, 0x18, gRaceUiTotalMoneyLabel, 5);
    if (arg0->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

void updateRaceUiPrizePayoutConfirmed(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutRankPanel, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutRankPrize, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutCollectedCoins, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutTotalMoney, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutLabels, arg0);
}

void updateRaceUiPrizePayoutWaitForConfirm(void *arg0) {
    if (gPlayerInputPressed[0] & A_BUTTON) {
        enqueueSoundEffect(SFX_CONFIRM, SFX_PRIORITY_NORMAL);
        gMenuFlowState |= 0x10;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiPrizePayoutConfirmed);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutRankPanel, (void *)arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutRankPrize, (void *)arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutCollectedCoins, (void *)arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutTotalMoney, (void *)arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutLabels, (void *)arg0);
}

void updateRaceUiPrizePayoutCountMoney(RaceUiAlpha18Actor *arg0) {
    s32 unused;
    s32 shouldPlaySound;
    s32 value;
    RacePlayer *player;

    shouldPlaySound = 0;
    value = arg0->score;
    if (value != 0) {
        shouldPlaySound = 1;
    }
    player = (RacePlayer *)(s32)&gRacePlayers[0];
    if (value >= 9) {
        player->money += 8;
        arg0->score -= 8;
    } else {
        gRacePlayers[0].money = player->money + value;
        arg0->score = 0;
    }

    if (player->state.rankIndex != 3) {
        value = player->state.score;
        if (value != 0) {
            shouldPlaySound = 1;
        }
        if (value >= 9) {
            player->money += 8;
            player->state.score = value - 8;
        } else {
            player->state.score = 0;
            player->money += value;
        }
    }

    if (gPlayerInputPressed[0] & A_BUTTON) {
        gRacePlayers[0].money = player->money + arg0->score;
        if (player->state.rankIndex != 3) {
            player->money += player->state.score;
            player->state.score = 0;
        }
        arg0->score = 0;
    }

    if (player->money >= 1000000) {
        player->money = 999999;
    }

    if (player->state.rankIndex != 3) {
        if ((arg0->score == 0) && (player->state.score == 0)) {
            enqueueSoundEffect(SFX_RESULTS_TALLY_COMPLETE, SFX_PRIORITY_NORMAL);
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiPrizePayoutWaitForConfirm);
        }
    } else if (arg0->score == 0) {
        shouldPlaySound = 0;
        enqueueSoundEffect(SFX_RESULTS_TALLY_COMPLETE, SFX_PRIORITY_NORMAL);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiPrizePayoutWaitForConfirm);
    }

    if (shouldPlaySound != 0) {
        s16 timer = arg0->timer;
        if (timer == 0) {
            arg0->timer = 2;
            enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
        } else {
            arg0->timer = timer - 1;
        }
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutRankPanel, (void *)arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutRankPrize, (void *)arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutCollectedCoins, (void *)arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutTotalMoney, (void *)arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutLabels, (void *)arg0);
}

void updateRaceUiPrizePayoutWaitBeforeMoneyCount(RaceUiPrizePayoutActor *actor) {
    actor->timer = actor->timer - 1;
    if (actor->timer == 0) {
        actor->timer = 0;
        setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiPrizePayoutCountMoney);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutRankPanel, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutRankPrize, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutCollectedCoins, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutTotalMoney, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutLabels, actor);
}

void updateRaceUiPrizePayoutRevealGetCoin(RaceUiPrizePayoutActor *actor) {
    actor->timer--;
    if (actor->timer == 0) {
        actor->timer = RACE_UI_PRIZE_PAYOUT_REVEAL_TIMER;
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
        setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiPrizePayoutWaitBeforeMoneyCount);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutRankPanel, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutRankPrize, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutCollectedCoins, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutLabels, actor);
}

void updateRaceUiPrizePayoutRevealRankPrize(RaceUiPrizePayoutActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
        arg0->timer = RACE_UI_PRIZE_PAYOUT_REVEAL_TIMER;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiPrizePayoutRevealGetCoin);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutRankPanel, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutRankPrize, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutLabels, arg0);
}

void updateRaceUiPrizePayoutShowRankPrize(RaceUiPrizePayoutActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        arg0->timer = RACE_UI_PRIZE_PAYOUT_REVEAL_TIMER;
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiPrizePayoutRevealRankPrize);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutRankPanel, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutLabels, arg0);
}

void updateRaceUiPrizePayoutFadeIn(RaceUiPrizePayoutActor *arg0) {
    arg0->alpha += 0x10;
    if (arg0->alpha >= 0x100) {
        arg0->timer = RACE_UI_PRIZE_PAYOUT_REVEAL_TIMER;
        arg0->alpha = 0xFF;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiPrizePayoutShowRankPrize);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutRankPanel, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiPrizePayoutLabels, arg0);
}

void initRaceUiPrizePayout(RaceUiPrizePayoutActor *arg0) {
    arg0->alpha = 0;
    arg0->prizeAmount =
        gRacePrizeAmountsByCourseAndRank[0][gRacePlayers[0].state.rankIndex + gRaceCourseIndex.signedValue * 4];
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiPrizePayoutFadeIn);
}

void drawRaceUiResultsBannerPlacements(RaceUiResultsBannerActor *arg0) {
    u16 characterSpriteIndex;

    if (arg0->alpha != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->alpha);
    }

    if (arg0->player0 != -1) {
        drawAssetTableSprite(-0x88, -0x50, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0);
        drawAssetTableSprite(-0x68, -0x40, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x48);
        drawAssetTableSprite8bpp(
            -0x54,
            -0x50,
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            (characterSpriteIndex = gRacePlayers[arg0->player0].characterId + 0x41)
        );
        drawAssetTableSprite(-0x30, -0x40, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x4C);
    }

    if (arg0->player1 != -1) {
        drawAssetTableSprite(8, -0x30, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 1);
        drawAssetTableSprite(0x28, -0x20, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x49);
        drawAssetTableSprite8bpp(
            0x3C,
            -0x30,
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            (characterSpriteIndex = gRacePlayers[arg0->player1].characterId + 0x41)
        );
        drawAssetTableSprite(0x60, -0x20, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x4C);
    }

    if (arg0->player2 != -1) {
        drawAssetTableSprite(-0x88, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 2);
        drawAssetTableSprite(-0x68, 0, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x4A);
        drawAssetTableSprite8bpp(
            -0x54,
            -0x10,
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            (characterSpriteIndex = gRacePlayers[arg0->player2].characterId + 0x41)
        );
        drawAssetTableSprite(-0x30, 0, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x4C);
    }

    if (arg0->player3 != -1) {
        drawAssetTableSprite(8, 0x10, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 3);
        drawAssetTableSprite(0x28, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x4B);
        drawAssetTableSprite8bpp(
            0x3C,
            0x10,
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            (characterSpriteIndex = gRacePlayers[arg0->player3].characterId + 0x41)
        );
        drawAssetTableSprite(0x60, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x4C);
    }

    if (arg0->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

void drawRaceUiResultsBannerPlayerLabels(RaceUiResultsBannerActor *arg0) {
    RaceUiResultsBannerActor *actor;
    char text[0x64];

    actor = arg0;
    if (actor->alpha != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, actor->alpha);
    }

    if (actor->player0 != -1) {
        sprintf(text, gRaceResultsSlot0WinCountFormat, gRacePlayers[actor->player0].unk18);
        if (gUiBlinkTimer & 1) {
            drawMenuAsciiTextDefaultScale(-0x20, -0x38, text, 6);
        } else {
            drawMenuAsciiTextDefaultScale(-0x20, -0x38, text, 0);
        }
        if (gRacePlayers[actor->player0].isCpu == 0) {
            sprintf(text, gRaceResultsSlot0PlayerLabelFormat, gRacePlayers[actor->player0].playerIndex + 1);
            drawMenuAsciiTextDefaultScale(-0x30, -0x48, text, 5);
        } else {
            drawMenuAsciiTextDefaultScale(-0x30, -0x48, gRaceResultsSlot0CpuLabel, 4);
        }
    }

    if (actor->player1 != -1) {
        sprintf(text, gRaceResultsSlot1WinCountFormat, gRacePlayers[actor->player1].unk18);
        drawMenuAsciiTextDefaultScale(0x70, -0x18, text, 6);
        if (gRacePlayers[actor->player1].isCpu == 0) {
            sprintf(text, gRaceResultsSlot1PlayerLabelFormat, gRacePlayers[actor->player1].playerIndex + 1);
            drawMenuAsciiTextDefaultScale(0x60, -0x28, text, 5);
        } else {
            drawMenuAsciiTextDefaultScale(0x60, -0x28, gRaceResultsSlot1CpuLabel, 4);
        }
    }

    if (actor->player2 != -1) {
        sprintf(text, gRaceResultsSlot2WinCountFormat, gRacePlayers[actor->player2].unk18);
        drawMenuAsciiTextDefaultScale(-0x20, 8, text, 6);
        if (gRacePlayers[actor->player2].isCpu == 0) {
            sprintf(text, gRaceResultsSlot2PlayerLabelFormat, gRacePlayers[actor->player2].playerIndex + 1);
            drawMenuAsciiTextDefaultScale(-0x30, -8, text, 5);
        } else {
            drawMenuAsciiTextDefaultScale(-0x30, -8, gRaceResultsSlot2CpuLabel, 4);
        }
    }

    if (actor->player3 != -1) {
        sprintf(text, gRaceResultsSlot3WinCountFormat, gRacePlayers[actor->player3].unk18);
        drawMenuAsciiTextDefaultScale(0x70, 0x28, text, 6);
        if (gRacePlayers[actor->player3].isCpu == 0) {
            sprintf(text, gRaceResultsSlot3PlayerLabelFormat, gRacePlayers[actor->player3].playerIndex + 1);
            drawMenuAsciiTextDefaultScale(0x60, 0x18, text, 5);
        } else {
            drawMenuAsciiTextDefaultScale(0x60, 0x18, gRaceResultsSlot3CpuLabel, 4);
        }
    }

    if (actor->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

void sortRaceUiResultsBannerPlayers(RaceUiResultsBannerActor *arg0) {
    RaceUiResultsBannerActor *actor;
    s32 i;
    s32 j;
    s32 next;
    s32 temp;

    actor = arg0;
    i = 0;
    do {
        actor->playerIndices[i] = i;
        i++;
    } while (i < 4);

    // clang-format off
    i = 0; do { next = i + 1; j = next;
        // clang-format on
        if (next < 4) {
            do {
                if (gRacePlayers[actor->playerIndices[j]].state.rankIndex < gRacePlayers[actor->playerIndices[i]].state.rankIndex) {
                    temp = actor->playerIndices[i];
                    actor->playerIndices[i] = actor->playerIndices[j];
                    actor->playerIndices[j] = temp;
                }
                j++;
            } while (j < 4);
        }
        next = i + 1;
        i = next;
    } while (next < 3);

    i = 0;
    do {
        if (!(gRacePlayers[actor->playerIndices[i]].state.stateFlags & 0x40)) {
            actor->playerIndices[i] = -1;
        }
        i++;
    } while (i < 4);
}

void updateRaceUiResultsBannerConfirmed(RaceUiResultsBannerActor *actor) {
    sortRaceUiResultsBannerPlayers(actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiResultsBannerPlacements, (void *)actor);
    addRenderCallback(
        &gMenuForegroundRenderCallbackList,
        (RenderCallback)drawRaceUiResultsBannerPlayerLabels,
        (void *)actor
    );
}

void updateRaceUiResultsBannerWaitForInput(RaceUiResultsBannerActor *arg0) {
    s32 *input;
    s32 i;
    RaceUiResultsBannerActor *actor;

    actor = arg0;
    i = 0;
    if ((s32)gPlayerCount > 0) {
        // clang-format off
        input = gPlayerInputPressed; do {
            // clang-format on
            if ((*input & A_BUTTON) && !(gMenuFlowState & 0x10)) {
                gMenuFlowState |= 0x10;
                enqueueSoundEffect(SFX_CONFIRM, SFX_PRIORITY_NORMAL);
                setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiResultsBannerConfirmed);
            }
            i++;
            input++;
        } while (i < (s32)gPlayerCount);
    }
    sortRaceUiResultsBannerPlayers(actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiResultsBannerPlacements, (void *)actor);
    addRenderCallback(
        &gMenuForegroundRenderCallbackList,
        (RenderCallback)drawRaceUiResultsBannerPlayerLabels,
        (void *)actor
    );
}

void updateRaceUiResultsBannerFadeIn(RaceUiResultsBannerActor *actor) {
    actor->alpha += 0x10;
    if (actor->alpha >= 0x100) {
        actor->timer = 0x96;
        actor->alpha = 0xFF;
        setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiResultsBannerWaitForInput);
    }
    sortRaceUiResultsBannerPlayers(actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiResultsBannerPlacements, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiResultsBannerPlayerLabels, actor);
}

void initRaceUiResultsBanner(RaceUiResultsBannerActor *actor) {
    actor->alpha = 0;
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiResultsBannerFadeIn);
}

void drawRaceUiPackedTime(RaceTimer *arg0, s32 x, s32 arg2, s32 arg3) {
    char buffer[0x58];
    RaceTimer *record;
    s16 y;
    u16 color;
    s32 i;

    record = arg0;
    y = arg2;
    color = arg3;
    sprintf(&buffer[-0x10], gRaceUiPackedTimeMinutesFormat, record->minutes);
    i = -0x10;
    do {
        /* IDO register allocation for this function depends on this empty loop. */
        do {
        } while (0);
        drawAssetTableSpriteWithExplicitPaletteWideIndex(
            (s16)x,
            y,
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            (((u8)buffer[i]) - 5) & 0xFFFF,
            color
        );
        i++;
        x += 8;
    } while (i < (-0xE));
    drawAssetTableSpriteWithExplicitPaletteWideIndex(
        (s16)x,
        y,
        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
        0x36,
        color
    );
    x += 8;
    sprintf(&buffer[-0x10], gRaceUiPackedTimeSecondsFormat, record->seconds);
    i = -0x10;
    do {
        drawAssetTableSpriteWithExplicitPaletteWideIndex(
            (s16)x,
            y,
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            (((u8)buffer[i]) - 5) & 0xFFFF,
            color
        );
        i++;
        x += 8;
    } while (i < (-0xE));
    drawAssetTableSpriteWithExplicitPaletteWideIndex(
        (s16)x,
        y,
        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
        0x35,
        color
    );
    x += 8;
    sprintf(&buffer[-0x10], gRaceUiPackedTimeFractionFormat, record->fraction >> 8);
    i = -0x10;
    do {
        drawAssetTableSpriteWithExplicitPaletteWideIndex(
            (s16)x,
            y,
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            (((u8)buffer[i]) - 5) & 0xFFFF,
            color
        );
        i++;
        x += 8;
    } while (i != (-0xE));
}

void drawRaceUiCourseStatsRows(RaceUiCourseStatsActor *arg0) {
    s32 y;
    s32 row;
    s32 offset;
    RaceUiCourseStatsActor *actor;

    actor = arg0;
    y = -0x50;
    for (row = 0, offset = 0; row != 5; row++, offset += 4, y += 0x20) {
        drawAssetTableSprite(-8, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), row + 0x77);
        if ((row == actor->task.userId) && (gUiBlinkTimer & 1)) {
            drawRaceUiPackedTime(
                &((RaceUiCourseStatsNameData *)&gGameSaveDataBuffer[0])
                     ->courseStatsNames[gRaceCourseIndex.signedValue][row],
                0x10,
                y,
                0x10
            );
        } else if (row < 3) {
            drawRaceUiPackedTime(
                &((RaceUiCourseStatsNameData *)&gGameSaveDataBuffer[0])
                     ->courseStatsNames[gRaceCourseIndex.signedValue][row],
                0x10,
                y,
                0xC
            );
        } else {
            drawRaceUiPackedTime(
                &((RaceUiCourseStatsNameData *)&gGameSaveDataBuffer[0])
                     ->courseStatsNames[gRaceCourseIndex.signedValue][row],
                0x10,
                y,
                0xD
            );
        }

        drawAssetTableSprite(
            0x58,
            (s16)y,
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            ((((RaceUiCourseStatsIconData *)&gGameSaveDataBuffer[0])
                  ->courseStatsIcons[gRaceCourseIndex.signedValue][row] &
              7) +
             0x51)
        );
        drawAssetTableSprite(
            0x6C,
            (s16)y,
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            (((*((RaceUiCourseStatsIconData *)&gGameSaveDataBuffer[0]))
                  .courseStatsIcons[gRaceCourseIndex.signedValue][row] >>
              3) +
             0x7C)
        );
    }
}

void drawRaceUiCourseStatsElapsedTime(RaceUiAlpha1AActor *arg0) {

    {
        if (arg0->alpha != 0xFF) {
            gDPPipeSync(gRegionAllocPtr++);
            gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
            gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
            gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 255, 255, 255, arg0->alpha);
        }
        drawRaceUiPackedTime(&gRaceElapsedTimer, -0x6C, -0x30, 0xC);
        if (arg0->alpha != 0xFF) {
            gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
        }
    }
}

void drawRaceUiCourseStatsLapTimeLabel(RaceUiAlpha18Actor *arg0) {

    if (arg0->alpha != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->alpha);
    }
    drawMenuAsciiTextDefaultScale(-0x74, -0x3C, gRaceUiLapTimeLabel, 5);
    if (arg0->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

void updateRaceUiCourseStatsConfirmed(RaceUiCourseStatsActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseStatsElapsedTime, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseStatsRows, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseStatsLapTimeLabel, arg0);
}

void updateRaceUiCourseStatsWaitForConfirm(RaceUiCourseStatsActor *arg0) {
    if (gPlayerInputPressed[0] & A_BUTTON) {
        gMenuFlowState |= 0x10;
        enqueueSoundEffect(SFX_CONFIRM, SFX_PRIORITY_NORMAL);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiCourseStatsConfirmed);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseStatsElapsedTime, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseStatsRows, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseStatsLapTimeLabel, arg0);
}

void updateRaceUiCourseStatsHoldRows(RaceUiCourseStatsActor *arg0) {
    arg0->holdTimer--;
    if (arg0->holdTimer == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiCourseStatsWaitForConfirm);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseStatsElapsedTime, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseStatsLapTimeLabel, arg0);
}

void updateRaceUiCourseStatsFadeInRows(RaceUiCourseStatsActor *arg0) {
    arg0->statsAlpha += 0x10;
    if (arg0->statsAlpha >= 0x100) {
        arg0->holdTimer = 0x1E;
        arg0->statsAlpha = 0xFF;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiCourseStatsHoldRows);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseStatsElapsedTime, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseStatsLapTimeLabel, arg0);
}

void updateRaceUiCourseStatsFadeInTitle(RaceUiCourseStatsActor *arg0) {
    arg0->titleAlpha += 0x10;
    if (arg0->titleAlpha >= 0x100) {
        arg0->titleAlpha = 0xFF;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiCourseStatsFadeInRows);
    }
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseStatsLapTimeLabel, arg0);
}

void initRaceUiCourseStats(RaceUiCourseStatsActor *arg0) {
    arg0->titleAlpha = 0;
    arg0->statsAlpha = 0;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiCourseStatsFadeInTitle);
}

void drawRaceUiScoreAttackPointTotal(RaceUiAlpha18Actor *arg0) {
    volatile s32 x;
    char buffer[0x20];
    RaceUiAlpha18Actor *actor;

    actor = arg0;
    if (actor->alpha != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, actor->alpha);
    }

    drawAssetTableSprite(-0x6C, -0x24, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), 0x20);
    sprintf(buffer + -8, gRaceUiScoreAttackPointTotalFormat, gRacePlayers[0].state.scoreAttackPointTotal);

    if (buffer[-8] != ' ') {
        drawAssetTableSprite(-0x4C, -0x1C, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), buffer[-8] - 5);
    }

    x = -0x44;
    drawAssetTableSprite(
        ((SplitWord *)&x)->half.lo,
        -0x1C,
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)),
        buffer[-7] - 5
    );

    if (actor->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

void drawRaceUiHitPrizePayoutCounters(RaceUiCounterActor *arg0) {
    volatile u8 pad[4];
    SplitWord y;
    s32 i;
    s32 x;
    char buffer[0x20];
    s16 rowY;
    s16 visibleRows;

    y.word = -0x47;
    if (arg0->flag == 0) {
        y.word = -0x33;
    }
    visibleRows = arg0->state;
    if (visibleRows >= 0) {
        x = 0x20;
        sprintf(buffer, gRaceUiCounterHitPrizeFormat, arg0->value, arg0);
        i = 0;
        rowY = y.half.lo;
    loop1:
        if (buffer[i] != 0) {
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex(
                    x,
                    rowY,
                    getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                    ((u8)buffer[i] - 5) & 0xFFFF
                );
            }
            x += 8;
            i++;
            goto loop1;
        }
        drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
        y.word += 0x28;
        visibleRows = arg0->state;
    }

    if (visibleRows > 0) {
        if (arg0->flag != 0) {
            x = 0x20;
            sprintf(buffer, gRaceUiCounterPerfectHitBonusFormat, arg0->bonus, arg0);
            i = 0;
            rowY = y.half.lo;
            i = 0;
        loop2:
            if (buffer[i] != 0) {
                if (buffer[i] != ' ') {
                    drawAssetTableSpriteWideIndex(
                        x,
                        rowY,
                        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                        ((u8)buffer[i] - 5) & 0xFFFF
                    );
                }
                x += 8;
                i++;
                goto loop2;
            }
            drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
            y.word += 0x28;
            visibleRows = arg0->state;
        }
    }

    if (visibleRows >= 2) {
        x = 0x20;
        sprintf(buffer, gRaceUiCounterCompleteBonusFormat, arg0->target, arg0);
        i = 0;
        rowY = y.half.lo;
        i = 0;
    loop3:
        if (buffer[i] != 0) {
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex(
                    x,
                    rowY,
                    getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                    ((u8)buffer[i] - 5) & 0xFFFF
                );
            }
            x += 8;
            i++;
            goto loop3;
        }
        drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
        y.word += 0x28;
        visibleRows = arg0->state;
    }

    if (arg0) {}
    if (visibleRows >= 3) {
        x = 0x18;
        sprintf(buffer, gRaceUiCounterMoneyFormat, gRacePlayers[0].money);
        i = 0;
        rowY = y.half.lo;
        i = 0;
    loop4:
        if (buffer[i] != 0) {
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex(
                    x,
                    rowY,
                    getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                    ((u8)buffer[i] - 5) & 0xFFFF
                );
            }
            x += 8;
            i++;
            goto loop4;
        }
        drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
    }
}

void drawRaceUiScoreAttackCourseValues(RaceUiPopupActor *arg0) {
    volatile u8 padding[0x18];
    char new_var;
    s32 i;
    s32 color;
    s32 y;
    s32 space;

    y = -0x50;
    /* IDO scheduling for this function depends on these statements staying on one line. */
    // clang-format off
    i = 0; space = ' '; while (i != 5) { char buffer[8];
        // clang-format on

        drawAssetTableSprite(-8, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), i + 0x77);

        if ((i == arg0->task.userId) && (gUiBlinkTimer & 1)) {
            color = 0x10;
        } else if (i < 3) {
            color = 0xC;
        } else {
            color = 0xD;
        }

        sprintf(
            buffer,
            gRaceUiCourseValueFormat,
            ((RaceUiCourseValueData *)&gGameSaveDataBuffer[0])->values[gRaceCourseIndex.signedValue][i]
        );

        if ((u8)buffer[0] != space) {
            drawAssetTableSpriteWithExplicitPalette(
                0x10,
                (s16)y,
                getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                buffer[0] - 5,
                color
            );
            {}
        }

        if ((u8)buffer[1] != space) {
            drawAssetTableSpriteWithExplicitPalette(
                0x18,
                (s16)y,
                getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                buffer[1] - 5,
                (u16)color
            );
        }

        drawAssetTableSprite(
            0x46,
            (s16)y,
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            (((RaceUiCourseValueData *)&gGameSaveDataBuffer[0])->icons[gRaceCourseIndex.signedValue][i] & 7) + 0x51
        );
        drawAssetTableSprite(
            0x58,
            (s16)y,
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            (((RaceUiCourseValueData *)&gGameSaveDataBuffer[0])->icons[gRaceCourseIndex.signedValue][i] >> 3) + 0x7C
        );

        i++;
        y += 0x20;
    }
}

void drawRaceUiScoreAttackTargetStatus(RaceUiAlpha18Actor *arg0) {
    char text[0xC];

    {
        if (arg0->alpha != 0xFF) {
            gDPPipeSync(gRegionAllocPtr++);
            gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
            gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
            gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 255, 255, 255, arg0->alpha);
        }
        sprintf(text, gRaceUiScoreAttackTargetFormat, gRacePlayers[0].state.scoreAttackPointTarget);
        drawMenuAsciiTextDefaultScale(-0x54, -0x14, gRaceUiScoreAttackAsteriskLabel, 6);
        drawMenuAsciiTextDefaultScale(-0x3C, -0x14, text, 6);
        if (gRaceChallengeFailed != 0) {
            drawMenuAsciiTextDefaultScale(-0x68, -4, gRaceUiScoreAttackTimeOutLabel, 4);
        }
        if (arg0->alpha != 0xFF) {
            gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
        }
    }
}

void drawRaceUiHitPrizePayoutLabels(RaceUiCounterActor *arg0) {
    s32 y;

    if (arg0->alpha != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->alpha);
    }

    y = -0x50;
    if (arg0->flag == 0) {
        y = -0x3C;
    }

    if (gRaceChallengeFailed != 0) {
        drawMenuAsciiTextDefaultScale(-8, (s16)y, gRaceUiHitPrizeX1Label, 5);
    } else {
        drawMenuAsciiTextDefaultScale(-0xC, (s16)y, gRaceUiHitPrizeX50Label, 5);
    }

    y += 0x28;
    if (arg0->flag != 0) {
        drawMenuAsciiTextDefaultScale(-0xC, (s16)y, gRaceUiPerfectHitLabel, 5);
        y += 0x28;
    }

    drawMenuAsciiTextDefaultScale(-8, (s16)y, gRaceUiCompleteBonusLabel, 5);
    y += 0x28;
    drawMenuAsciiTextDefaultScale(4, (s16)y, gRaceUiHitPrizeTotalMoneyLabel, 5);

    if (arg0->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

void drawRaceUiScoreAttackHitLabels(void *arg0) {
    drawMenuAsciiTextDefaultScale(0x20, -0x48, gRaceUiCourseValueHitLabelRow1, 6);
    drawMenuAsciiTextDefaultScale(0x20, -0x28, gRaceUiCourseValueHitLabelRow2, 6);
    drawMenuAsciiTextDefaultScale(0x20, -8, gRaceUiCourseValueHitLabelRow3, 6);
    drawMenuAsciiTextDefaultScale(0x20, 0x18, gRaceUiCourseValueHitLabelRow4, 4);
    drawMenuAsciiTextDefaultScale(0x20, 0x38, gRaceUiCourseValueHitLabelRow5, 4);
}

void updateRaceUiHitPrizeCourseValuesConfirmed(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackPointTotal, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackCourseValues, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackTargetStatus, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackHitLabels, arg0);
}

void updateRaceUiHitPrizeCourseValuesWaitForConfirm(void *arg0) {
    if (gPlayerInputPressed[0] & A_BUTTON) {
        enqueueSoundEffect(SFX_CONFIRM, SFX_PRIORITY_NORMAL);
        gMenuFlowState |= 0x10;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiHitPrizeCourseValuesConfirmed);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackPointTotal, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackCourseValues, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackTargetStatus, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackHitLabels, arg0);
}

void updateRaceUiHitPrizePayoutConfirmed(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackPointTotal, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiHitPrizePayoutCounters, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackTargetStatus, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiHitPrizePayoutLabels, arg0);
}

void updateRaceUiHitPrizePayoutWaitForConfirm(RaceUiCounterActor *actor) {
    if (gPlayerInputPressed[0] & A_BUTTON) {
        enqueueSoundEffect(SFX_CONFIRM, SFX_PRIORITY_NORMAL);
        if (actor->task.userId < 5) {
            setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiHitPrizeCourseValuesWaitForConfirm);
        } else {
            gMenuFlowState |= 0x10;
            setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiHitPrizePayoutConfirmed);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackPointTotal, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiHitPrizePayoutCounters, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackTargetStatus, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiHitPrizePayoutLabels, actor);
}

void updateRaceUiHitPrizePayoutCountMoney(RaceUiCounterActor *arg0) {
    s16 value;

    if (arg0->timer == 0) {
        arg0->timer = 2;
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
    } else {
        arg0->timer--;
    }

    value = arg0->value;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    /* IDO register allocation depends on this always-zero index. */
    gRacePlayers[gPlayerInputPressed[0] * 0].money += value;
    arg0->value -= value;

    value = arg0->bonus;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    gRacePlayers[0].money += value;
    arg0->bonus -= value;

    value = arg0->target;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    gRacePlayers[0].money += value;
    arg0->target -= value;

    if (gRacePlayers[0].money >= 0xF4240) {
        gRacePlayers[0].money = 0xF423F;
    }

    if ((arg0->value == 0) && (arg0->bonus == 0) && (arg0->target == 0)) {
        enqueueSoundEffect(SFX_RESULTS_TALLY_COMPLETE, SFX_PRIORITY_NORMAL);
        arg0->timer = 0x14;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiHitPrizePayoutWaitForConfirm);
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackPointTotal, (void *)arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiHitPrizePayoutCounters, (void *)arg0);
    addRenderCallback(
        &gMenuForegroundRenderCallbackList,
        (RenderCallback)drawRaceUiScoreAttackTargetStatus,
        (void *)arg0
    );
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiHitPrizePayoutLabels, (void *)arg0);
}

void updateRaceUiHitPrizeWaitBeforePayout(RaceUiCounterActor *arg0) {
    arg0->state = RACE_UI_HIT_PRIZE_SHOW_TOTAL_MONEY;
    arg0->timer--;
    if (arg0->timer == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiHitPrizePayoutCountMoney);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackPointTotal, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiHitPrizePayoutCounters, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackTargetStatus, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiHitPrizePayoutLabels, arg0);
}

void updateRaceUiHitPrizeRevealTotalMoney(RaceUiCounterActor *arg0) {
    arg0->state = RACE_UI_HIT_PRIZE_SHOW_TOTAL_MONEY;
    arg0->timer--;
    if (arg0->timer == 0) {
        arg0->timer = 0xA;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiHitPrizeWaitBeforePayout);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackPointTotal, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiHitPrizePayoutCounters, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackTargetStatus, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiHitPrizePayoutLabels, arg0);
}

void updateRaceUiHitPrizeRevealCompleteBonus(RaceUiCounterActor *arg0) {
    arg0->state = RACE_UI_HIT_PRIZE_SHOW_COMPLETE_BONUS;
    arg0->timer--;
    if (arg0->timer == 0) {
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
        arg0->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiHitPrizeRevealTotalMoney);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackPointTotal, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiHitPrizePayoutCounters, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackTargetStatus, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiHitPrizePayoutLabels, arg0);
}

void updateRaceUiHitPrizeRevealPerfectHit(RaceUiCounterActor *arg0) {
    arg0->state = RACE_UI_HIT_PRIZE_SHOW_PERFECT_HIT;
    arg0->timer--;
    if (arg0->timer == 0) {
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
        arg0->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiHitPrizeRevealCompleteBonus);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackPointTotal, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiHitPrizePayoutCounters, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackTargetStatus, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiHitPrizePayoutLabels, arg0);
}

void updateRaceUiHitPrizeHoldHitPrize(RaceUiCounterActor *arg0) {
    arg0->state = RACE_UI_HIT_PRIZE_SHOW_HIT_PRIZE;
    arg0->timer--;
    if (arg0->timer == 0) {
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
        arg0->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        if (arg0->hasPerfectHitBonus != 0) {
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiHitPrizeRevealPerfectHit);
        } else {
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiHitPrizeRevealCompleteBonus);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackPointTotal, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiHitPrizePayoutCounters, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackTargetStatus, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiHitPrizePayoutLabels, arg0);
}

void updateRaceUiHitPrizeWaitBeforeHitPrize(RaceUiCounterActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        arg0->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiHitPrizeHoldHitPrize);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackPointTotal, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackTargetStatus, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiHitPrizePayoutLabels, arg0);
}

void updateRaceUiHitPrizeFadeIn(RaceUiCounterActor *arg0) {
    arg0->alpha += RACE_UI_RESULTS_FADE_STEP;
    if (arg0->alpha >= 0x100) {
        arg0->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        arg0->alpha = RACE_UI_RESULTS_FULL_ALPHA;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiHitPrizeWaitBeforeHitPrize);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackPointTotal, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiScoreAttackTargetStatus, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiHitPrizePayoutLabels, arg0);
}

void initRaceUiHitPrizePayout(RaceUiCounterActor *arg0) {
    arg0->alpha = 0;
    arg0->hasPerfectHitBonus = FALSE;
    arg0->pendingPerfectHitBonus = 0;
    arg0->pendingCompleteBonus = RACE_UI_HIT_PRIZE_SCORE_RATE;
    if (gRaceCourseIndex.signedValue == RACE_COURSE_ROOKIE_MOUNTAIN) {
        arg0->pendingCompleteBonus = RACE_UI_HIT_PRIZE_QUICKSAND_VALLEY_SCORE_RATE;
    }
    if (gRaceChallengeFailed != 0) {
        arg0->pendingHitPrize = gRacePlayers[0].state.scoreAttackPointTotal;
    } else {
        arg0->pendingHitPrize = gRacePlayers[0].state.scoreAttackPointTotal * 0x32;
        if (gRacePlayers[0].state.scoreAttackPointTotal == gRacePlayers[0].state.scoreAttackPointTarget) {
            arg0->hasPerfectHitBonus = TRUE;
            arg0->pendingPerfectHitBonus = RACE_UI_HIT_PRIZE_PERFECT_HIT_BONUS;
        }
    }
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiHitPrizeFadeIn);
}

void drawRaceUiCourseRecordElapsedTime(RaceUiAlphaActor *arg0) {

    {
        if (arg0->alpha != 0xFF) {
            gDPPipeSync(gRegionAllocPtr++);
            gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
            gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
            gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 255, 255, 255, arg0->alpha);
        }
        drawRaceUiPackedTime(&gRaceElapsedTimer, -0x68, -0x37, 0xC);
        if (arg0->alpha != 0xFF) {
            gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
        }
    }
}

void drawRaceUiCourseRecordPayoutCounters(RaceUiDualCounterActor *arg0) {
    volatile u8 pad[4];
    SplitWord y;
    s32 i;
    s32 x;
    char buffer[0x20];
    s16 tempY;

    y.word = -0x47;
    if (arg0->state >= 0) {
        x = 0x20;
        sprintf(buffer, gRaceUiTargetTimeChallengePrizeCounterFormat, arg0->leftValue);
        i = 0;
        while (1) {
            if (buffer[i] == 0) {
                break;
            }
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex(
                    (s16)x,
                    -0x47,
                    getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                    (((u8)buffer[i]) - 5) & 0xFFFF
                );
            }
            x += 8;
            i++;
        }
        drawAssetTableSpriteWideIndex((s16)x, -0x47, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
        y.word = -0x27;
    }

    if (arg0->state > 0) {
        x = 0x20;
        sprintf(buffer, gRaceUiTargetTimeChallengeSpeedFanGetCounterFormat, arg0->rightValue);
        i = 0;
        tempY = y.half.lo;
        i = 0;
        while (1) {
            if (buffer[i] == 0) {
                break;
            }
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex(
                    (s16)x,
                    tempY,
                    getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                    (((u8)buffer[i]) - 5) & 0xFFFF
                );
            }
            x += 8;
            i++;
        }
        drawAssetTableSpriteWideIndex((s16)x, tempY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
        y.word += 0x20;
    }

    if (arg0->state >= 2) {
        x = 0x20;
        if (arg0->flag != 0) {
            sprintf(buffer, gRaceUiTargetTimeChallengePerfectGetCounterFormat, arg0->bonus);
            i = 0;
            tempY = y.half.lo;
            i = 0;
            while (1) {
                if (buffer[i] == 0) {
                    break;
                }
                if (buffer[i] != ' ') {
                    drawAssetTableSpriteWideIndex(
                        (s16)x,
                        tempY,
                        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                        (((u8)buffer[i]) - 5) & 0xFFFF
                    );
                }
                x += 8;
                i++;
            }
            drawAssetTableSpriteWideIndex((s16)x, tempY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
            y.word += 0x20;
        }
    }

    if (arg0->state >= 3) {
        x = 0x20;
        sprintf(buffer, gRaceUiTargetTimeChallengeCompleteBonusCounterFormat, arg0->leftTarget);
        i = 0;
        tempY = y.half.lo;
        i = 0;
        while (1) {
            if (buffer[i] == 0) {
                break;
            }
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex(
                    (s16)x,
                    tempY,
                    getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                    (((u8)buffer[i]) - 5) & 0xFFFF
                );
            }
            x += 8;
            i++;
        }
        drawAssetTableSpriteWideIndex((s16)x, tempY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
        y.word += 0x20;
    }

    if (arg0->state >= 4) {
        x = 0x18;
        sprintf(buffer, gRaceUiTargetTimeChallengeMoneyCounterFormat, gRacePlayers[0].money);
        i = 0;
        tempY = y.half.lo;
        i = 0;
        while (1) {
            if (buffer[i] == 0) {
                break;
            }
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex(
                    (s16)x,
                    tempY,
                    getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                    (((u8)buffer[i]) - 5) & 0xFFFF
                );
            }
            x += 8;
            i++;
        }
        drawAssetTableSpriteWideIndex((s16)x, tempY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
    }
}

void drawRaceUiCourseRecordRows(RaceUiPopupActor *arg0) {
    s32 y;
    s32 color;
    s32 i;
    s32 offset;

    y = -0x50;
    /* IDO scheduling for this function depends on this initialization staying on one line. */
    // clang-format off
    i = 0; offset = 0; while (1) {
        // clang-format on
        drawAssetTableSprite(-8, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), i + 0x77);
        if ((i == arg0->task.userId) && (gUiBlinkTimer & 1)) {
            color = 0x10;
        } else if (i < 3) {
            color = 0xC;
        } else {
            color = 0xD;
        }
        drawRaceUiPackedTime(
            &((RaceUiResultNameData *)&gGameSaveDataBuffer[0])->resultNames[gRaceCourseIndex.signedValue][i],
            0x10,
            y,
            color
        );
        drawAssetTableSprite(
            0x58,
            (s16)y,
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            (((RaceUiResultIconData *)&gGameSaveDataBuffer[0])->resultIcons[gRaceCourseIndex.signedValue][i] & 7) + 0x51
        );
        drawAssetTableSprite(
            0x6C,
            (s16)y,
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            ((*((RaceUiResultIconData *)&gGameSaveDataBuffer[0])).resultIcons[gRaceCourseIndex.signedValue][i] >> 3) +
                0x7C
        );
        i++;
        offset += 4;
        y += 0x20;
        if (i == 5) {
            break;
        }
    }
}

void drawRaceUiCourseRecordLapTimeLabel(RaceUiAlphaActor *arg0) {

    if (arg0->alpha != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->alpha);
    }
    drawMenuAsciiTextDefaultScale(-0x70, -0x40, gRaceUiTargetTimeChallengeLapTimeLabel, 5);
    if (arg0->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

void drawRaceUiCourseRecordPayoutLabels(RaceUiDualCounterActor *arg0) {
    char buf[0x18];
    char *bufp;
    s32 y;
    s16 textY;
    RaceTimer *targetTime;

    if (arg0->alpha != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->alpha);
    }

    if (gRaceChallengeFailed == 0) {
        drawMenuAsciiTextDefaultScale(-0x68, -0x1F, gRaceUiTargetTimeChallengeTargetLabel, 7);
        targetTime = &gRaceCourseTargetTimes[gRaceCourseIndex.signedValue];
        bufp = buf - 0xC;
        sprintf(
            bufp,
            gRaceUiTargetTimeChallengeTargetTimeFormat,
            targetTime->minutes,
            targetTime->seconds,
            targetTime->fraction >> 8
        );
        drawMenuAsciiTextDefaultScale(-0x68, -0x16, bufp, 7);
        drawMenuAsciiTextDefaultScale(-0x68, -8, gRaceUiTargetTimeChallengeFastLabel, 7);
        sprintf(bufp, gRaceUiTargetTimeChallengeDeltaTimeFormat, arg0->row, arg0->column, arg0->timeFraction >> 8);
        drawMenuAsciiTextDefaultScale(-0x6C, 1, bufp, 7);
    } else {
        drawMenuAsciiTextDefaultScale(-0x68, -0x24, gRaceUiTargetTimeChallengeTimeOutLabel, 4);
    }

    y = -0x50;
    drawMenuAsciiTextDefaultScale(-8, (s16)y, gRaceUiTargetTimeChallengePrizeLabel, 5);
    y += 0x20;
    drawMenuAsciiTextDefaultScale(-0xC, (s16)y, gRaceUiTargetTimeChallengeSpeedFanGetLabel, 5);
    y += 0x20;
    if (arg0->flag != 0) {
        drawMenuAsciiTextDefaultScale(-0xC, (s16)y, gRaceUiTargetTimeChallengePerfectGetLabel, 5);
        y += 0x20;
    }
    drawMenuAsciiTextDefaultScale(-8, (s16)y, gRaceUiTargetTimeChallengeCompleteBonusLabel, 5);
    y += 0x20;
    textY = y;
    drawMenuAsciiTextDefaultScale(-0xC, textY, gRaceUiTargetTimeChallengeMoneyLabel, 5);

    if (arg0->alpha != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

void updateRaceUiCourseRecordRowsConfirmed(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordElapsedTime, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordRows, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordLapTimeLabel, arg0);
}

void updateRaceUiCourseRecordRowsWaitForConfirm(void *arg0) {
    if (gPlayerInputPressed[0] & A_BUTTON) {
        enqueueSoundEffect(SFX_CONFIRM, SFX_PRIORITY_NORMAL);
        gMenuFlowState |= 0x10;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiCourseRecordRowsConfirmed);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordElapsedTime, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordRows, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordLapTimeLabel, arg0);
}

void updateRaceUiCourseRecordPayoutConfirmed(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordElapsedTime, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutCounters, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordLapTimeLabel, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutLabels, arg0);
}

void updateRaceUiCourseRecordPayoutWaitForConfirm(RaceUiDualCounterActor *actor) {
    if (gPlayerInputPressed[0] & A_BUTTON) {
        enqueueSoundEffect(SFX_CONFIRM, SFX_PRIORITY_NORMAL);
        if (actor->task.userId < 5) {
            setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiCourseRecordRowsWaitForConfirm);
        } else {
            gMenuFlowState |= 0x10;
            setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiCourseRecordPayoutConfirmed);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordElapsedTime, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutCounters, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordLapTimeLabel, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutLabels, actor);
}

void updateRaceUiCourseRecordPayoutCountMoney(RaceUiDualCounterActor *arg0) {
    s16 value;

    if (arg0->timer == 0) {
        arg0->timer = 2;
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
    } else {
        arg0->timer--;
    }

    value = arg0->leftValue;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    /* IDO register allocation depends on this always-zero index. */
    gRacePlayers[gPlayerInputPressed[0] * 0].money += value;
    arg0->leftValue -= value;

    value = arg0->rightValue;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    gRacePlayers[0].money += value;
    arg0->rightValue -= value;

    value = arg0->bonus;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    gRacePlayers[0].money += value;
    arg0->bonus -= value;

    value = arg0->leftTarget;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    gRacePlayers[0].money += value;
    arg0->leftTarget -= value;

    if (gRacePlayers[0].money >= 0xF4240) {
        gRacePlayers[0].money = 0xF423F;
    }

    if ((arg0->rightValue == 0) && (arg0->leftValue == 0) && (arg0->bonus == 0) && (arg0->leftTarget == 0)) {
        enqueueSoundEffect(SFX_RESULTS_TALLY_COMPLETE, SFX_PRIORITY_NORMAL);
        arg0->timer = 0x14;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiCourseRecordPayoutWaitForConfirm);
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordElapsedTime, (void *)arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutCounters, (void *)arg0);
    addRenderCallback(
        &gMenuForegroundRenderCallbackList,
        (RenderCallback)drawRaceUiCourseRecordLapTimeLabel,
        (void *)arg0
    );
    addRenderCallback(
        &gMenuForegroundRenderCallbackList,
        (RenderCallback)drawRaceUiCourseRecordPayoutLabels,
        (void *)arg0
    );
}

void updateRaceUiCourseRecordWaitBeforePayout(RaceUiDualCounterActor *actor) {
    actor->state = RACE_UI_COURSE_RECORD_HOLD_FINAL_MONEY_STATE;
    actor->timer--;
    if (actor->timer == 0) {
        setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiCourseRecordPayoutCountMoney);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordElapsedTime, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutCounters, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordLapTimeLabel, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutLabels, actor);
}

void updateRaceUiCourseRecordHoldFinalMoney(RaceUiDualCounterActor *actor) {
    actor->state = RACE_UI_COURSE_RECORD_HOLD_FINAL_MONEY_STATE;
    actor->timer--;
    if (actor->timer == 0) {
        actor->timer = 0xA;
        setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiCourseRecordWaitBeforePayout);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordElapsedTime, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutCounters, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordLapTimeLabel, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutLabels, actor);
}

void updateRaceUiCourseRecordRevealFinalMoney(RaceUiDualCounterActor *actor) {
    actor->state = RACE_UI_COURSE_RECORD_FINAL_MONEY_STATE;
    actor->timer--;
    if (actor->timer == 0) {
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
        actor->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiCourseRecordHoldFinalMoney);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordElapsedTime, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutCounters, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordLapTimeLabel, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutLabels, actor);
}

void updateRaceUiCourseRecordRevealPerfectGet(RaceUiDualCounterActor *actor) {
    actor->state = RACE_UI_COURSE_RECORD_REVEAL_PERFECT_GET_STATE;
    actor->timer--;
    if (actor->timer == 0) {
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
        actor->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiCourseRecordRevealFinalMoney);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordElapsedTime, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutCounters, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordLapTimeLabel, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutLabels, actor);
}

void updateRaceUiCourseRecordRevealSpeedFanGet(RaceUiDualCounterActor *actor) {
    actor->state = RACE_UI_COURSE_RECORD_REVEAL_SPEED_FAN_GET_STATE;
    actor->timer--;
    if (actor->timer == 0) {
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
        actor->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        if (actor->flag != 0) {
            setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiCourseRecordRevealPerfectGet);
        } else {
            setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiCourseRecordRevealFinalMoney);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordElapsedTime, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutCounters, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordLapTimeLabel, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutLabels, actor);
}

void updateRaceUiCourseRecordRevealTimePrize(RaceUiDualCounterActor *actor) {
    actor->state = RACE_UI_COURSE_RECORD_REVEAL_TIME_PRIZE_STATE;
    actor->timer--;
    if (actor->timer == 0) {
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
        actor->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiCourseRecordRevealSpeedFanGet);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordElapsedTime, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutCounters, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordLapTimeLabel, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutLabels, actor);
}

void updateRaceUiCourseRecordWaitBeforeReveal(RaceUiDualCounterActor *actor) {
    actor->timer--;
    if (actor->timer == 0) {
        actor->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
        setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiCourseRecordRevealTimePrize);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordElapsedTime, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordLapTimeLabel, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutLabels, actor);
}

void updateRaceUiCourseRecordFadeIn(RaceUiDualCounterActor *arg0) {
    arg0->alpha += RACE_UI_RESULTS_FADE_STEP;
    if (arg0->alpha >= 0x100) {
        arg0->timer = RACE_UI_RESULTS_REVEAL_TIMER;
        arg0->alpha = RACE_UI_RESULTS_FULL_ALPHA;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiCourseRecordWaitBeforeReveal);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordElapsedTime, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordLapTimeLabel, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiCourseRecordPayoutLabels, arg0);
}

void initRaceUiCourseRecordPayout(RaceUiDualCounterActor *arg0) {
    u32 temp_v0;
    s16 *new_var;

    arg0->alpha = 0;
    arg0->flag = 0;
    arg0->bonus = 0;
    arg0->leftTarget = 0x12C;
    if (gRaceCourseIndex.signedValue == RACE_COURSE_ROOKIE_MOUNTAIN) {
        arg0->leftTarget = 0x64;
    }
    calculateRaceTimerDelta(
        &gRaceCourseTargetTimes[gRaceCourseIndex.signedValue],
        &gRaceElapsedTimer,
        (RaceTimer *)&arg0->row
    );
    new_var = &gRacePlayers[0].state.collectedCourseCoinMarkerCount;
    if (gRaceChallengeFailed != 0) {
        arg0->leftValue = 0;
        temp_v0 = *new_var;
    } else {
        arg0->leftValue = ((arg0->row * 0x3C) + arg0->column) * 0x14;
        if (gRacePlayers[0].state.totalCourseCoinMarkerCount == (temp_v0 = gRacePlayers[0].state.collectedCourseCoinMarkerCount)) {
            arg0->flag = 1;
            arg0->bonus = 0x12C;
            temp_v0 = *new_var;
        }
    }
    arg0->rightValue = temp_v0 * 0xA;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiCourseRecordFadeIn);
}

void drawRaceUiTrickAttackTotals(RaceUiDualCounterActor *arg0) {
    s32 i;
    char buffer[0x28];
    s32 x;

    if (arg0->alpha18 != 0xFF) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->alpha18);
    }

    sprintf(buffer, gRaceUiTrickAttackPointTotalFormat, gRacePlayers[0].state.trickAttackPointTotal);

    x = -0x50;
    if (gRacePlayers[0].state.trickAttackPointTotal >= 10) {
        x = -0x54;
    }
    if (gRacePlayers[0].state.trickAttackPointTotal >= 100) {
        x = -0x58;
    }
    if (gRacePlayers[0].state.trickAttackPointTotal >= 1000) {
        x = -0x5C;
    }

    i = 0;
    while (1) {
        if (buffer[i] == '\0') {
            break;
        }
        if (buffer[i] != ' ') {
            drawAssetTableSprite(x, -0x37, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), buffer[i] - 5);
        }
        i++;
        x += 8;
    }

    x = -0x50;
    sprintf(buffer, gRaceUiTrickAttackSecondaryCounterFormat, gRacePlayers[0].state.trickAttackMakeCount);
    if (gRacePlayers[0].state.trickAttackMakeCount >= 10) {
        x = -0x54;
    }

    i = 0;
    while (1) {
        if (buffer[i] == '\0') {
            break;
        }
        if (buffer[i] != ' ') {
            drawAssetTableSpriteWithExplicitPalette(
                x,
                -0x13,
                getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                buffer[i] - 5,
                0xE
            );
        }
        i++;
        x += 8;
    }

    if (arg0->alpha18 != 0xFF) {
        gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
    }
}

void drawRaceUiTrickPrizePayoutCounters(RaceUiCourseStatsActor *arg0) {
    volatile u8 pad[4];
    SplitWord y;
    s32 i;
    s32 x;
    char buffer[0x20];
    s16 rowY;
    s16 visibleRows;

    y.word = -0x47;
    visibleRows = arg0->visibleRows;
    x = 0x20;
    if (visibleRows >= 0) {
        sprintf(buffer, gRaceUiPendingTrickPrizeFormat, arg0->pendingTrickPrize);
        i = 0;
    loop1:
        if (buffer[i] != 0) {
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex(
                    x,
                    -0x47,
                    getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                    ((u8)buffer[i] - 5) & 0xFFFF
                );
            }
            x += 8;
            i++;
            goto loop1;
        }
        drawAssetTableSpriteWideIndex(x, -0x47, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
        y.word = -0x1F;
        visibleRows = arg0->visibleRows;
    }

    if (visibleRows > 0) {
        x = 0x20;
        sprintf(buffer, gRaceUiPendingMakeBonusFormat, arg0->pendingMakeBonus);
        i = 0;
        rowY = y.half.lo;
        i = 0;
    loop2:
        if (buffer[i] != 0) {
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex(
                    x,
                    rowY,
                    getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                    ((u8)buffer[i] - 5) & 0xFFFF
                );
            }
            x += 8;
            i++;
            goto loop2;
        }
        drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
        y.word += 0x28;
        visibleRows = arg0->visibleRows;
    }

    if (visibleRows >= 2) {
        x = 0x20;
        sprintf(buffer, gRaceUiPendingCompleteBonusFormat, arg0->pendingCompleteBonus);
        i = 0;
        rowY = y.half.lo;
        i = 0;
    loop3:
        if (buffer[i] != 0) {
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex(
                    x,
                    rowY,
                    getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                    ((u8)buffer[i] - 5) & 0xFFFF
                );
            }
            x += 8;
            i++;
            goto loop3;
        }
        drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
        y.word += 0x28;
        visibleRows = arg0->visibleRows;
    }

    if (visibleRows >= 3) {
        x = 0x18;
        sprintf(buffer, gRaceUiPendingMoneyFormat, gRacePlayers[0].money);
        i = 0;
        rowY = y.half.lo;
        i = 0;
    loop4:
        if (buffer[i] != 0) {
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWideIndex(
                    x,
                    rowY,
                    getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                    ((u8)buffer[i] - 5) & 0xFFFF
                );
            }
            x += 8;
            i++;
            goto loop4;
        }
        drawAssetTableSpriteWideIndex(x, rowY, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x37);
    }
}

void drawRaceUiTrickAttackCourseValues(RaceUiCourseStatsActor *arg0) {
    volatile u8 padding[0x18];
    char new_var;
    s32 i;
    s32 color;
    s32 y;
    s32 space;

    y = -0x50;
    // clang-format off
    i = 0; \
    space = ' '; \
    while (i != 5) { \
        char buffer[8]; \
        drawAssetTableSprite(-8, (s16)y, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), i + 0x77); \
        if ((i == arg0->task.userId) && (gUiBlinkTimer & 1)) { \
            color = 0x10; \
        } else if (i < 3) { \
            color = 0xC; \
        } else {
            // clang-format on
            color = 0xD;
        }

        sprintf(
            buffer,
            gRaceUiTrickValueFormat,
            ((RaceUiTrickValueData *)&gGameSaveDataBuffer[0])->values[gRaceCourseIndex.signedValue][i]
        );

        if ((u8)buffer[0] != space) {
            drawAssetTableSpriteWithExplicitPalette(
                0x10,
                (s16)y,
                getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                buffer[0] - 5,
                color
            );
            {}
        }
        if ((u8)buffer[1] != space) {
            drawAssetTableSpriteWithExplicitPalette(
                0x18,
                (s16)y,
                getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                buffer[1] - 5,
                (u16)color
            );
        }
        if ((u8)buffer[2] != space) {
            drawAssetTableSpriteWithExplicitPalette(
                0x20,
                (s16)y,
                getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                buffer[2] - 5,
                (u16)color
            );
        }
        if ((u8)buffer[3] != space) {
            drawAssetTableSpriteWithExplicitPalette(
                0x28,
                (s16)y,
                getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                buffer[3] - 5,
                (u16)color
            );
        }

        drawAssetTableSprite(
            0x40,
            (s16)y,
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            (((RaceUiTrickIconData *)&gGameSaveDataBuffer[0])->icons[gRaceCourseIndex.signedValue][i] & 7) + 0x51
        );
        drawAssetTableSprite(
            0x54,
            (s16)y,
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            (((RaceUiTrickIconData *)&gGameSaveDataBuffer[0])->icons[gRaceCourseIndex.signedValue][i] >> 3) + 0x7C
        );

        i++;
        y += 0x20;
    }
}

void drawRaceUiTrickAttackResultLabels(RaceUiAlpha18Actor *arg0) {
    const char *new_var2;
    s32 w0;
    s32 new_var;

    {
        if (arg0->alpha != 0xFF) {
            gDPPipeSync(gRegionAllocPtr++);
            gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);

            gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
            gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 255, 255, 255, arg0->alpha);
        }
        drawMenuAsciiTextDefaultScale(-0x68, -0x40, new_var2 = gRaceUiTrickAttackResultsTrickLabel, 6);
        drawMenuAsciiTextDefaultScale(-0x64, -0x1C, gRaceUiTrickAttackResultsMakeLabel, 5);
        w0 = 8;
        if (gRaceChallengeFailed != 0) {
            new_var = w0;
            drawMenuAsciiTextDefaultScale(-0x6C, new_var, gRaceUiTrickAttackResultsTimeOutLabel, 4);
        }
        if (arg0->alpha != 0xFF) {
            gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
        }
    }
}

void drawRaceUiTrickPrizePayoutLabels(RaceUiAlpha18Actor *arg0) {
    s32 y;
    RaceUiAlpha18Actor **arg0p = &arg0;

    {
        if ((*arg0p)->alpha != 0xFF) {
            gDPPipeSync(gRegionAllocPtr++);

            gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
            gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
            gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 255, 255, 255, (*arg0p)->alpha);
        }

        y = -0x50;
        drawMenuAsciiTextDefaultScale(-0xC, (s16)y, gRaceUiPendingTrickPrizeLabel, 5);
        y += 0x28;
        drawMenuAsciiTextDefaultScale(-8, (s16)y, gRaceUiPendingMakeBonusLabel, 5);
        y += 0x28;
        drawMenuAsciiTextDefaultScale(-8, (s16)y, gRaceUiPendingCompleteBonusLabel, 5);
        y += 0x28;
        drawMenuAsciiTextDefaultScale(-0xC, (s16)y, gRaceUiPendingMoneyLabel, 5);

        if ((*arg0p)->alpha != 0xFF) {
            gSPDisplayList(gRegionAllocPtr++, gMenuRenderModeResetDl);
        }
    }
}

void drawRaceUiTrickAttackPointLabels(void *arg0) {
    drawMenuAsciiTextDefaultScale(0x30, -0x48, gRaceUiTrickValuePointLabelRow1, 6);
    drawMenuAsciiTextDefaultScale(0x30, -0x28, gRaceUiTrickValuePointLabelRow2, 6);
    drawMenuAsciiTextDefaultScale(0x30, -8, gRaceUiTrickValuePointLabelRow3, 6);
    drawMenuAsciiTextDefaultScale(0x30, 0x18, gRaceUiTrickValuePointLabelRow4, 4);
    drawMenuAsciiTextDefaultScale(0x30, 0x38, gRaceUiTrickValuePointLabelRow5, 4);
}

void updateRaceUiTrickPrizeCourseValuesConfirmed(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackTotals, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackCourseValues, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackResultLabels, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackPointLabels, arg0);
}

void updateRaceUiTrickPrizeCourseValuesWaitForConfirm(void *arg0) {
    if (gPlayerInputPressed[0] & A_BUTTON) {
        enqueueSoundEffect(SFX_CONFIRM, SFX_PRIORITY_NORMAL);
        gMenuFlowState |= 0x10;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickPrizeCourseValuesConfirmed);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackTotals, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackCourseValues, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackResultLabels, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackPointLabels, arg0);
}

void updateRaceUiTrickPrizePayoutConfirmed(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackTotals, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickPrizePayoutCounters, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackResultLabels, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickPrizePayoutLabels, arg0);
}

void updateRaceUiTrickPrizePayoutWaitForConfirm(RaceUiCourseStatsActor *actor) {
    if (gPlayerInputPressed[0] & A_BUTTON) {
        enqueueSoundEffect(SFX_CONFIRM, SFX_PRIORITY_NORMAL);
        if (actor->task.userId < 5) {
            setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiTrickPrizeCourseValuesWaitForConfirm);
        } else {
            gMenuFlowState |= 0x10;
            setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiTrickPrizePayoutConfirmed);
        }
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackTotals, actor);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickPrizePayoutCounters, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackResultLabels, actor);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickPrizePayoutLabels, actor);
}

void updateRaceUiTrickPrizePayoutCountMoney(RaceUiCourseStatsActor *arg0) {
    s16 value;

    if (arg0->timer == 0) {
        arg0->timer = 2;
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
    } else {
        arg0->timer--;
    }

    value = arg0->pendingTrickPrize;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    /* IDO register allocation depends on this always-zero index. */
    gRacePlayers[gPlayerInputPressed[0] * 0].money += value;
    arg0->pendingTrickPrize -= value;

    value = arg0->pendingMakeBonus;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    gRacePlayers[0].money += value;
    arg0->pendingMakeBonus -= value;

    value = arg0->pendingCompleteBonus;
    if ((value >= 8) && !(gPlayerInputPressed[0] & A_BUTTON)) {
        value = 8;
    }
    gRacePlayers[0].money += value;
    arg0->pendingCompleteBonus -= value;

    if (gRacePlayers[0].money >= 0xF4240) {
        gRacePlayers[0].money = 0xF423F;
    }

    if ((arg0->pendingTrickPrize == 0) && (arg0->pendingMakeBonus == 0) && (arg0->pendingCompleteBonus == 0)) {
        enqueueSoundEffect(SFX_RESULTS_TALLY_COMPLETE, SFX_PRIORITY_NORMAL);
        arg0->timer = 0x14;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickPrizePayoutWaitForConfirm);
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackTotals, (void *)arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickPrizePayoutCounters, (void *)arg0);
    addRenderCallback(
        &gMenuForegroundRenderCallbackList,
        (RenderCallback)drawRaceUiTrickAttackResultLabels,
        (void *)arg0
    );
    addRenderCallback(
        &gMenuForegroundRenderCallbackList,
        (RenderCallback)drawRaceUiTrickPrizePayoutLabels,
        (void *)arg0
    );
}

void updateRaceUiTrickPrizePayoutHoldMoneyRow(RaceUiCourseStatsActor *arg0) {
    arg0->visibleRows = RACE_UI_TRICK_PRIZE_SHOW_TOTAL_MONEY;
    arg0->timer--;
    if (arg0->timer == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickPrizePayoutCountMoney);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackTotals, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickPrizePayoutCounters, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackResultLabels, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickPrizePayoutLabels, arg0);
}

void updateRaceUiTrickPrizePayoutRevealMoneyRow(RaceUiCourseStatsActor *arg0) {
    arg0->visibleRows = 3;
    arg0->timer--;
    if (arg0->timer == 0) {
        arg0->timer = 0xA;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickPrizePayoutHoldMoneyRow);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackTotals, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickPrizePayoutCounters, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackResultLabels, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickPrizePayoutLabels, arg0);
}

void updateRaceUiTrickPrizePayoutRevealCompleteBonus(RaceUiCourseStatsActor *arg0) {
    arg0->visibleRows = RACE_UI_TRICK_PRIZE_SHOW_COMPLETE_BONUS;
    arg0->timer--;
    if (arg0->timer == 0) {
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
        arg0->timer = RACE_UI_TRICK_PRIZE_REVEAL_DELAY;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickPrizePayoutRevealMoneyRow);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackTotals, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickPrizePayoutCounters, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackResultLabels, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickPrizePayoutLabels, arg0);
}

void updateRaceUiTrickPrizePayoutRevealMakeBonus(RaceUiCourseStatsActor *arg0) {
    arg0->visibleRows = 1;
    arg0->timer--;
    if (arg0->timer == 0) {
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
        arg0->timer = RACE_UI_TRICK_PRIZE_REVEAL_DELAY;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickPrizePayoutRevealCompleteBonus);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackTotals, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickPrizePayoutCounters, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackResultLabels, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickPrizePayoutLabels, arg0);
}

void updateRaceUiTrickPrizePayoutRevealTrickPrize(RaceUiCourseStatsActor *arg0) {
    arg0->timer--;
    arg0->visibleRows = 0;
    if (arg0->timer == 0) {
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
        arg0->timer = RACE_UI_TRICK_PRIZE_REVEAL_DELAY;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickPrizePayoutRevealMakeBonus);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackTotals, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickPrizePayoutCounters, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackResultLabels, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickPrizePayoutLabels, arg0);
}

void updateRaceUiTrickPrizePayoutWaitBeforeReveal(RaceUiCourseStatsActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        arg0->timer = RACE_UI_TRICK_PRIZE_REVEAL_DELAY;
        enqueueSoundEffect(SFX_COIN_TICK, SFX_PRIORITY_NORMAL);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickPrizePayoutRevealTrickPrize);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackTotals, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackResultLabels, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickPrizePayoutLabels, arg0);
}

void updateRaceUiTrickPrizePayoutFadeIn(RaceUiCourseStatsActor *arg0) {
    arg0->titleAlpha += 0x10;
    if (arg0->titleAlpha >= 0x100) {
        arg0->timer = 0x14;
        arg0->titleAlpha = 0xFF;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickPrizePayoutWaitBeforeReveal);
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackTotals, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickAttackResultLabels, arg0);
    addRenderCallback(&gMenuForegroundRenderCallbackList, (RenderCallback)drawRaceUiTrickPrizePayoutLabels, arg0);
}

void initRaceUiTrickPrizePayout(RaceUiCourseStatsActor *arg0) {
    arg0->titleAlpha = 0;
    arg0->pendingMakeBonus = 0;
    arg0->pendingCompleteBonus = 0x12C;
    if (gRaceChallengeFailed != 0) {
        arg0->pendingTrickPrize = 0;
        *(volatile s16 *)&arg0->pendingMakeBonus = 0;
    } else {
        arg0->pendingTrickPrize = gRacePlayers[0].state.trickAttackPointTotal * 3;
        arg0->pendingMakeBonus = gRacePlayers[0].state.trickAttackMakeCount * 10;
    }
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiTrickPrizePayoutFadeIn);
}

void drawRaceUiResultAwardBadge(CallbackTask *task) {
    drawAssetTableSprite(-0x68, -0x2C, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), 0x4D);
    drawAssetTableSprite(-0x42, -0xC, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), gRacePlayers[0].state.rankIndex);
    drawAssetTableSprite(-0x22, 4, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), gRacePlayers[0].state.rankIndex + 0x48);
    drawAssetTableSprite(-0x68, 6, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), gRacePlayers[0].characterId + 0x91);
}

void updateRaceUiResultAwardBadgeDelay(CallbackTask *task) {
    if ((u16)task->header.userId != 0) {
        task->header.userId = (u16)task->header.userId - 1;
    } else {
        addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceUiResultAwardBadge, task);
    }
}

void renderRacePlayerSparkleEffect(RaceUiSparkleActor *arg0) {
    RaceUiSparkleActor *arg1;
    s32 i;
    s32 j;
    Transform3D sp6C;

    arg1 = arg0;
    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg1->matrixDirty != 0) {
        arg1->matrixDirty = 0;
        sp6C = gIdentityFixedTransform;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                sp6C.rotation[(i * 3) + j] = (sp6C.rotation[(i * 3) + j] * arg1->scale) / 0x1000;
            }
        }
        sp6C.translation.x = arg1->pos.x;
        sp6C.translation.y = arg1->pos.y;
        sp6C.translation.z = arg1->pos.z;
        arg1->matrix = allocFixedTransformMatrix(&sp6C);
    }

    if (arg1->matrix != NULL) {
        gSPDisplayList(gRegionAllocPtr++, gTranslucentSpriteRenderModeDl);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg1->alpha);
        gDPLoadTextureBlock_4b(
            gRegionAllocPtr++,
            arg1->images[arg1->frame],
            G_IM_FMT_CI,
            0x20,
            0x20,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            0,
            0,
            G_TX_NOLOD,
            G_TX_NOLOD
        );
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg1->palettes[arg1->frame]);
        gSPMatrix(gRegionAllocPtr++, arg1->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPVertex(gRegionAllocPtr++, gRacePlayerSparkleQuadVertices, 4, 0);
        gSP1Quadrangle(gRegionAllocPtr++, 2, 1, 0, 3, 3);
    }
}

void updateRacePlayerSparkleOrbit(RaceUiSparkleActor *arg0) {
    RaceUiSparkleTransformScratch stack;
    RacePlayer *player;
    s16 timer;
    s32 angle;

    if (gRaceUpdatePaused == 0) {
        arg0->scale += 0x100;
        if (arg0->scale >= 0x1001) {
            arg0->scale = 0x1000;
        }
        if (gFrameCounter & 1) {
            arg0->frame = (arg0->frame + 1) & 3;
        }

        timer = arg0->timer;
        if (timer != 0) {
            arg0->timer = timer - 1;
            arg0->alpha += arg0->alphaStep;
            if (arg0->alpha >= 0xE0) {
                arg0->alphaStep = -0x10;
            }
            if (arg0->alpha < 0x41) {
                arg0->alphaStep = 0x10;
            }
        } else {
            arg0->alpha -= 0x10;
            if (arg0->alpha <= 0) {
                player = &gRacePlayers[arg0->playerIndex];
                player->state.activeSparkleEffectCount--;
                removeCallbackTask((CallbackTask *)arg0);
                return;
            }
        }

        arg0->unk28 += 0x60;
        angle = arg0->unk28 - gRacePlayers[arg0->playerIndex].state.facingAngle;
        makeFixedRotationY(stack.matrix, angle);
        transformVec3iByFixedMatrix(stack.matrix, &gRacePlayerSparkleOrbitOffset, &stack.vec);

        player = &gRacePlayers[arg0->playerIndex];
        makeFixedRotationXY(stack.matrix, player->state.pitchAngle, player->state.facingAngle);
        transformVec3iByFixedMatrix(stack.matrix, &stack.vec, &arg0->pos);

        player = &gRacePlayers[arg0->playerIndex];
        arg0->pos.x += player->state.renderPosition.x;
        arg0->pos.y += player->state.renderPosition.y;
        arg0->pos.z += player->state.renderPosition.z;
    }

    addRenderCallback(&gModelRenderCallbackQueue7Head, (RenderCallback)renderRacePlayerSparkleEffect, (void *)arg0);
}

void updateRacePlayerSparkleRetarget(RaceUiSparkleActor *arg0) {
    RacePlayer *player;
    s16 timer;
    s32 selectedPlayerIndex;
    s32 activeCount;
    RaceUiSparkleRetargetScratch scratch;

    if (gRaceUpdatePaused == 0) {
        if (gFrameCounter & 1) {
            arg0->frame = (arg0->frame + 1) & 3;
        }

        scratch.vec.x = 0;
        scratch.vec.y = arg0->zOffset;
        scratch.vec.z = 0;

        player = &gRacePlayers[arg0->playerIndex];
        if (arg0) {}
        makeFixedRotationXY(scratch.matrix, player->state.pitchAngle, player->state.facingAngle);
        transformVec3iByFixedMatrix(scratch.matrix, &scratch.vec, &arg0->pos);

        player = &gRacePlayers[arg0->playerIndex];
        arg0->pos.x += player->state.renderPosition.x;
        arg0->pos.y += player->state.renderPosition.y;
        arg0->pos.z += player->state.renderPosition.z;
        selectedPlayerIndex = gRaceOrderPlayerIds[1];

        timer = arg0->timer;
        if (timer == 0) {
            arg0->alpha += arg0->alphaStep;
            arg0->scale -= 0x200;
            if (arg0->scale < 0) {
                arg0->scale = 0;
            }
            if (arg0->alpha < 0) {
                arg0->alpha = 0;
                arg0->alphaStep = 0x10;
                activeCount = randomNextMain() ^ 0;
                if (activeCount > 0) {
                    selectedPlayerIndex = 0;
                    if ((gRacePlayerCount >= 4) && (arg0->playerIndex != gRaceOrderPlayerIds[3]) && (activeCount > 0)) {
                        selectedPlayerIndex = gRaceOrderPlayerIds[3];
                    }
                    if ((gRacePlayerCount >= 3) && (arg0->playerIndex != gRaceOrderPlayerIds[2]) &&
                        (activeCount >= 2) && !(gRacePlayers[gRaceOrderPlayerIds[2]].state.stateFlags & 0x40)) {
                        selectedPlayerIndex = gRaceOrderPlayerIds[2];
                    }
                    if ((gRacePlayerCount >= 2) && (arg0->playerIndex != gRaceOrderPlayerIds[1]) &&
                        (activeCount >= 3) && !(gRacePlayers[gRaceOrderPlayerIds[1]].state.stateFlags & 0x40)) {
                        selectedPlayerIndex = gRaceOrderPlayerIds[1];
                    }
                    if ((gRacePlayerCount > 0) && (arg0->playerIndex != gRaceOrderPlayerIds[0]) && (activeCount >= 4) &&
                        !(gRacePlayers[gRaceOrderPlayerIds[0]].state.stateFlags & 0x40)) {
                        selectedPlayerIndex = gRaceOrderPlayerIds[0];
                    }
                    arg0->playerIndex = selectedPlayerIndex & 3;
                }

                player = &gRacePlayers[arg0->playerIndex];
                player->state.activeSparkleEffectCount++;
                if (gRacePlayerHudStatuses[arg0->playerIndex].active != 0) {
                    enqueueSoundEffect(SFX_SPARKLE_RETARGET, SFX_PRIORITY_HIGH);
                }
                enqueuePositionalSoundEffect(
                    SFX_ITEM_USE,
                    &gRacePlayers[arg0->playerIndex].state.pos,
                    0x7F,
                    SFX_PRIORITY_NORMAL
                );
                arg0->timer = 0x12C;
                setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRacePlayerSparkleOrbit);
            }
        } else {
            arg0->timer = timer - 1;
        }
    }

    addRenderCallback(&gModelRenderCallbackQueue7Head, (RenderCallback)renderRacePlayerSparkleEffect, (void *)arg0);
}

void updateRacePlayerSparkleRise(RaceUiSparkleActor *arg0) {
    s16 sp38[0x10];
    Vec3i sp2C;
    volatile u8 pad[8];
    RacePlayer *player;

    if (gRaceUpdatePaused == 0) {
        if (gFrameCounter & 1) {
            arg0->frame = (arg0->frame + 1) & 3;
        }

        sp2C.x = 0;
        sp2C.y = arg0->zOffset;
        sp2C.z = 0;
        player = &gRacePlayers[arg0->playerIndex];
        makeFixedRotationXY(sp38, player->state.pitchAngle, player->state.facingAngle);
        transformVec3iByFixedMatrix(sp38, &sp2C, &arg0->pos);

        player = &gRacePlayers[arg0->playerIndex];
        arg0->pos.x += player->state.renderPosition.x;
        arg0->pos.y += player->state.renderPosition.y;
        arg0->pos.z += player->state.renderPosition.z;
        arg0->alpha += arg0->alphaStep;
        if (arg0->alpha >= 0x100) {
            arg0->alpha = 0xFF;
            arg0->alphaStep = -0x20;
            arg0->timer = 0x14;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRacePlayerSparkleRetarget);
        }
        arg0->zOffset += 0x20000;
    }

    addRenderCallback(&gModelRenderCallbackQueue7Head, (RenderCallback)renderRacePlayerSparkleEffect, arg0);
}

void initRacePlayerSparkleEffect(RaceUiSparkleActor *arg0) {
    arg0->zOffset = 0x20000;
    arg0->alpha = 0;
    arg0->alphaStep = 0x10;
    arg0->frame = 0;
    arg0->scale = 0x1000;
    arg0->unk28 = 0;

    getAssetTableImageAndPalette(
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)),
        0x23,
        &arg0->images[0],
        &arg0->palettes[0]
    );
    getAssetTableImageAndPalette(
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)),
        0x24,
        &arg0->images[1],
        &arg0->palettes[1]
    );
    getAssetTableImageAndPalette(
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)),
        0x25,
        &arg0->images[2],
        &arg0->palettes[2]
    );
    getAssetTableImageAndPalette(
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)),
        0x26,
        &arg0->images[3],
        &arg0->palettes[3]
    );
    enqueuePositionalSoundEffect(SFX_ITEM_USE, &gRacePlayers[arg0->playerIndex].state.pos, 0x7F, SFX_PRIORITY_NORMAL);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRacePlayerSparkleRise);
}

void spawnGhostItemEffect(s16 arg0) {
    RaceUiSparkleActor *temp =
        createCallbackTaskPreservingArgs((CallbackTaskCallback)initRacePlayerSparkleEffect, 0, 0x62);
    if (temp != NULL) {
        temp->playerIndex = arg0;
    }
}

void renderRaceUiExpiredSpeedFanEffect(RaceUiExpiredSpeedFanActor *arg0) {
    volatile u8 pad[0x20];
    RaceUiDisplayCommand *unused;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        arg0->frontMatrix = allocFixedTransformMatrix(&arg0->frontTransform);
        arg0->backMatrix = allocFixedTransformMatrix(&arg0->backTransform);
    }

    if (arg0->frontMatrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->frontMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, gSpeedFanFrontDisplayList);
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->backMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, gSpeedFanBackDisplayList);
    }
}

void updateRaceUiExpiredSpeedFanEffect(RaceUiExpiredSpeedFanActor *arg0) {
    Transform3D sp30;
    volatile u8 pad[0x10];
    RaceUiExpiredSpeedFanActor *actor;

    actor = arg0;
    actor->spinAngleX += 0x240;
    if (gRaceUpdatePaused == 0) {
        actor->worldPos.y += actor->velocityY;
        actor->velocityY -= 0x8000;
    }

    actor->frontTransform.translation.x = actor->worldPos.x;
    actor->frontTransform.translation.y = actor->worldPos.y;
    actor->frontTransform.translation.z = actor->worldPos.z;

    makeFixedRotationX(sp30.rotation, actor->spinAngleX);
    sp30.translation.x = actor->backOffset.x;
    sp30.translation.y = actor->backOffset.y;
    sp30.translation.z = actor->backOffset.z;
    composeFixedTransforms(&sp30, &actor->frontTransform, &actor->backTransform);

    actor->timer--;
    if (actor->timer == 0) {
        removeCallbackTask(actor);
        return;
    }

    if (gRacePlayers[actor->playerIndex].soundDisabled == 0) {
        addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)renderRaceUiExpiredSpeedFanEffect, arg0);
    }
}

void initRaceUiExpiredSpeedFanEffect(RaceUiExpiredSpeedFanActor *actor) {
    actor->scaleStep = 1;
    actor->backOffset.x = RACE_UI_SNOWBOARD_TRAIL_BACK_OFFSET_X;
    actor->backOffset.y = RACE_UI_SNOWBOARD_TRAIL_BACK_OFFSET_Y;
    actor->backOffset.z = RACE_UI_SNOWBOARD_TRAIL_BACK_OFFSET_Z;
    actor->timer = RACE_UI_SNOWBOARD_TRAIL_TIMER;
    actor->velocityY = RACE_UI_SNOWBOARD_TRAIL_INITIAL_VELOCITY_Y;
    updateRaceUiExpiredSpeedFanEffect(actor);
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiExpiredSpeedFanEffect);
}

void spawnRaceUiExpiredSpeedFanEffect(RacePlayer *player) {
    RaceUiExpiredSpeedFanActor *actor =
        createCallbackTaskPreservingArgs((CallbackTaskCallback)initRaceUiExpiredSpeedFanEffect, 0, 0x62);

    if (actor != NULL) {
        actor->playerIndex = player->playerIndex;
        actor->worldPos = player->state.speedFan.worldPos;
        actor->frontTransform = player->state.speedFan.frontTransform;
        actor->spinAngleX = player->state.speedFan.spinAngleX;
    }
}

void renderRaceUiItemStealTrailEffect(RaceUiRankTrailActor *arg0) {
    volatile u8 pad[0x20];
    RacePlayer *player;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        player = &gRacePlayers[arg0->task.userId];
        arg0->copyBlock.translation.x = player->state.renderPosition.x;
        arg0->copyBlock.translation.y = player->state.renderPosition.y + 0x100000;
        arg0->copyBlock.translation.z = player->state.renderPosition.z;
        arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
    }

    if (arg0->matrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &_ADDR_2003870_VRAM);
    }
}

void updateRaceUiItemStealTrailEffect(RaceUiRankTrailActor *arg0) {
    RacePlayer *player;
    s16 scale;
    s32 i;

    arg0->copyBlock = gIdentityFixedTransform;
    player = &gRacePlayers[arg0->task.userId];
    arg0->pos.x = player->state.renderPosition.x;
    arg0->pos.y = player->state.renderPosition.y;
    arg0->pos.z = player->state.renderPosition.z;

    for (i = 0; i < 4; i++) {
        if (i != arg0->task.userId) {
            pushRacePlayerOutOfCylinderAndApplyItemHit(&arg0->pos, 0xE0000, 0xB0000, 2, i);
        }
    }

    scale = arg0->scale;
    arg0->copyBlock.rotation[0] = (arg0->copyBlock.rotation[0] * scale) / 64;
    arg0->copyBlock.rotation[1] = (arg0->copyBlock.rotation[1] * scale) / 64;
    arg0->copyBlock.rotation[2] = (arg0->copyBlock.rotation[2] * scale) / 64;
    arg0->copyBlock.rotation[3] = (arg0->copyBlock.rotation[3] * scale) / 64;
    arg0->copyBlock.rotation[4] = (arg0->copyBlock.rotation[4] * scale) / 64;
    arg0->copyBlock.rotation[5] = (arg0->copyBlock.rotation[5] * scale) / 64;
    arg0->copyBlock.rotation[6] = (arg0->copyBlock.rotation[6] * scale) / 64;
    arg0->copyBlock.rotation[7] = (arg0->copyBlock.rotation[7] * scale) / 64;
    arg0->copyBlock.rotation[8] = (arg0->copyBlock.rotation[8] * scale) / 64;

    if (scale != 0x10) {
        arg0->scale = scale + 2;
    }

    if (gRacePlayers[arg0->task.userId].state.stateFlags & 0x100000) {
        addRenderCallback(
            &gModelRenderCallbackQueue7Head,
            (RenderCallback)renderRaceUiItemStealTrailEffect,
            (void *)arg0
        );
        return;
    }

    for (i = 0; i < 8; i++) {
        spawnRaceUiBurstTextParticle(arg0->pos.x, arg0->pos.y, arg0->pos.z, i);
    }

    enqueuePositionalSoundEffect(SFX_ITEM_STEAL_END, &gRacePlayers[arg0->task.userId].state.pos, 0x7F, SFX_PRIORITY_NORMAL);
    removeCallbackTask(arg0);
}

void initRaceUiItemStealTrailEffect(RaceUiRankTrailActor *arg0) {
    arg0->scale = 2;
    enqueuePositionalSoundEffect(SFX_ITEM_STEAL_START, &gRacePlayers[arg0->task.userId].state.pos, 0x7F, SFX_PRIORITY_NORMAL);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiItemStealTrailEffect);
}

void renderRaceUiBurstTextParticle(RaceUiTextParticleActor *arg0) {
    volatile u8 padding[4];
    Transform3D sp64;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        sp64 = gIdentityFixedTransform;
        sp64.translation.x = arg0->pos.x;
        sp64.translation.y = arg0->pos.y;
        sp64.translation.z = arg0->pos.z;
        arg0->matrix = allocFixedTransformMatrix(&sp64);
    }

    if (arg0->matrix != NULL) {
        gSPDisplayList(gRegionAllocPtr++, gRaceUiBurstTextRenderSetupDl);
        gDPLoadTextureBlock_4b(
            gRegionAllocPtr++,
            arg0->images[arg0->timer >> 2],
            G_IM_FMT_CI,
            0x20,
            0x20,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            0,
            0,
            G_TX_NOLOD,
            G_TX_NOLOD
        );
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palettes[arg0->timer >> 2]);
        gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPVertex(gRegionAllocPtr++, gRaceUiBurstTextQuadVertices, 4, 0);
        gSP1Quadrangle(gRegionAllocPtr++, 2, 1, 0, 3, 3);
    }
}

void renderRaceUiAltBurstTextParticle(RaceUiTextParticleActor *arg0) {
    volatile u8 padding[4];
    Transform3D sp64;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        sp64 = gIdentityFixedTransform;
        sp64.translation.x = arg0->pos.x;
        sp64.translation.y = arg0->pos.y;
        sp64.translation.z = arg0->pos.z;
        arg0->matrix = allocFixedTransformMatrix(&sp64);
    }

    if (arg0->matrix != NULL) {
        gSPDisplayList(gRegionAllocPtr++, gRaceUiAltBurstTextRenderSetupDl);
        gDPLoadTextureBlock_4b(
            gRegionAllocPtr++,
            arg0->images[arg0->timer >> 2],
            G_IM_FMT_CI,
            0x20,
            0x20,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            0,
            0,
            G_TX_NOLOD,
            G_TX_NOLOD
        );
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palettes[arg0->timer >> 2]);
        gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPVertex(gRegionAllocPtr++, gRaceUiBurstTextQuadVertices, 4, 0);
        gSP1Quadrangle(gRegionAllocPtr++, 2, 1, 0, 3, 3);
    }
}

void updateRaceUiBurstTextParticle(RaceUiTextParticleActor *arg0) {
    s32 velocityY;

    if (gRaceUpdatePaused == 0) {
        velocityY = arg0->velocity.y;
        arg0->pos.x += arg0->velocity.x;
        arg0->pos.y += velocityY;
        arg0->pos.z += arg0->velocity.z;
        arg0->velocity.y = velocityY - 0x8000;
        arg0->timer++;
        if (arg0->timer >= 0x10) {
            removeCallbackTask(arg0);
            return;
        }
    }
    if (arg0->useAltTextures == 0) {
        addRenderCallback(&gModelRenderCallbackQueue7Head, (RenderCallback)renderRaceUiBurstTextParticle, arg0);
        return;
    }
    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)renderRaceUiAltBurstTextParticle, arg0);
}

void initRaceUiBurstTextParticle(RaceUiTextParticleActor *arg0) {
    arg0->timer = 0;
    arg0->velocity = gRaceUiBurstTextInitialVelocities[arg0->task.userId];
    if (arg0->useAltTextures == 0) {
        getAssetTableImageAndPalette(
            getRelocatableHeapBlockBase(gAssetHandles[0x1C]),
            0x27,
            &arg0->images[0],
            &arg0->palettes[0]
        );
        getAssetTableImageAndPalette(
            getRelocatableHeapBlockBase(gAssetHandles[0x1C]),
            0x28,
            &arg0->images[1],
            &arg0->palettes[1]
        );
        getAssetTableImageAndPalette(
            getRelocatableHeapBlockBase(gAssetHandles[0x1C]),
            0x29,
            &arg0->images[2],
            &arg0->palettes[2]
        );
        getAssetTableImageAndPalette(
            getRelocatableHeapBlockBase(gAssetHandles[0x1C]),
            0x2A,
            &arg0->images[3],
            &arg0->palettes[3]
        );
    } else {
        getAssetTableImageAndPalette(
            getRelocatableHeapBlockBase(gAssetHandles[0x1C]),
            0x2B,
            &arg0->images[0],
            &arg0->palettes[0]
        );
        getAssetTableImageAndPalette(
            getRelocatableHeapBlockBase(gAssetHandles[0x1C]),
            0x2C,
            &arg0->images[1],
            &arg0->palettes[1]
        );
        getAssetTableImageAndPalette(
            getRelocatableHeapBlockBase(gAssetHandles[0x1C]),
            0x2D,
            &arg0->images[2],
            &arg0->palettes[2]
        );
        getAssetTableImageAndPalette(
            getRelocatableHeapBlockBase(gAssetHandles[0x1C]),
            0x2E,
            &arg0->images[3],
            &arg0->palettes[3]
        );
    }
    updateRaceUiBurstTextParticle(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiBurstTextParticle);
}

void spawnRaceUiBurstTextParticle(s32 x, s32 y, s32 z, s16 velocityIndex) {
    RaceUiTextParticleActor *actor =
        createCallbackTaskPreservingArgs((CallbackTaskCallback)initRaceUiBurstTextParticle, 5, 0x1E);
    if (actor != NULL) {
        actor->useAltTextures = 0;
        actor->task.userId = velocityIndex;
        actor->pos.x = x;
        actor->pos.y = y;
        actor->pos.z = z;
    }
}

void spawnRaceUiAltBurstTextParticle(s32 x, s32 y, s32 z, s16 velocityIndex) {
    RaceUiTextParticleActor *actor =
        createCallbackTaskPreservingArgs((CallbackTaskCallback)initRaceUiBurstTextParticle, 5, 0x1E);
    if (actor != NULL) {
        actor->useAltTextures = 1;
        actor->task.userId = velocityIndex;
        actor->pos.x = x;
        actor->pos.y = y;
        actor->pos.z = z;
    }
}

void renderGhostSlowdownActor(RaceUiPodiumTrailActor *arg0) {
    volatile u8 pad[0x20];
    volatile RaceUiDisplayCommand *unused;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        arg0->copyBlock.translation.x = arg0->pos.x;
        arg0->copyBlock.translation.y = arg0->pos.y + 0x38000;
        arg0->copyBlock.translation.z = arg0->pos.z;
        arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
    }

    if (arg0->matrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &PAN_DISPLAY_LIST_VRAM);
    }
}

void updateGhostSlowdownDriftAway(RaceUiPodiumTrailActor *arg0) {
    if (gRaceUpdatePaused == 0) {
        arg0->velocity -= 0x10000;
        arg0->pos.x += 0x20000;
        arg0->pos.y += arg0->velocity;
        arg0->pos.z += 0x10000;
        arg0->timer--;
    }

    if (arg0->timer == 0) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)renderGhostSlowdownActor, arg0);
    }
}

void updateGhostSlowdownImpact(RaceUiPodiumTrailActor *arg0) {
    RacePlayer *player;
    s32 height;

    if (gRaceUpdatePaused == 0) {
        arg0->velocity += -0x10000;
        height = arg0->height + arg0->velocity;
        arg0->height = height;
        if ((height < 0x200000) && ((arg0->playImpactSound & 0xFF) != 0)) {
            enqueuePositionalSoundEffect(
                SFX_GHOST_HIT,
                &gRacePlayers[arg0->targetPlayerIndex].state.pos,
                0x7F,
                SFX_PRIORITY_NORMAL
            );
            gRacePlayers[arg0->targetPlayerIndex].state.pendingItemHitFlags |= PLAYER_HITFLAG_GHOST_SLOWDOWN;
            gRacePlayers[arg0->targetPlayerIndex].state.hitSourcePlayerIndex = arg0->playerIndex;
            height = arg0->height;
            arg0->playImpactSound = 0;
        }

        if (height < 0x100000) {
            arg0->timer = 0x14;
            arg0->velocity = 0x80000;
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateGhostSlowdownDriftAway);
            height = arg0->height;
        }

        player = &gRacePlayers[arg0->targetPlayerIndex];
        arg0->pos.x = player->state.renderPosition.x;
        arg0->pos.y = player->state.renderPosition.y + height;
        arg0->pos.z = player->state.renderPosition.z;
    }

    gRacePlayers[arg0->targetPlayerIndex].state.isTargetedByAttack = 1;
    addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)renderGhostSlowdownActor, arg0);
}

void updateGhostSlowdownRise(RaceUiPodiumTrailActor *arg0) {
    RacePlayer *player;
    s16 scale;

    if (arg0->timer == 0) {
        if (gRaceUpdatePaused == 0) {
            arg0->velocity += 0x10000;
            arg0->height += arg0->velocity;
            arg0->copyBlock = gIdentityFixedTransform;
            player = &gRacePlayers[arg0->playerIndex];
            arg0->pos.x = player->state.renderPosition.x;
            arg0->pos.y = player->state.renderPosition.y + arg0->height;
            arg0->pos.z = player->state.renderPosition.z;

            scale = arg0->state;
            arg0->copyBlock.rotation[0] = (arg0->copyBlock.rotation[0] * scale) / 64;
            arg0->copyBlock.rotation[1] = (arg0->copyBlock.rotation[1] * scale) / 64;
            arg0->copyBlock.rotation[2] = (arg0->copyBlock.rotation[2] * scale) / 64;
            arg0->copyBlock.rotation[3] = (arg0->copyBlock.rotation[3] * scale) / 64;
            arg0->copyBlock.rotation[4] = (arg0->copyBlock.rotation[4] * scale) / 64;
            arg0->copyBlock.rotation[5] = (arg0->copyBlock.rotation[5] * scale) / 64;
            arg0->copyBlock.rotation[6] = (arg0->copyBlock.rotation[6] * scale) / 64;
            arg0->copyBlock.rotation[7] = (arg0->copyBlock.rotation[7] * scale) / 64;
            arg0->copyBlock.rotation[8] = (arg0->copyBlock.rotation[8] * scale) / 64;

            if (scale != 0x10) {
                arg0->state = scale + 1;
            }

            if (arg0->height >= 0xA00001) {
                arg0->velocity = -0x10000;
                setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateGhostSlowdownImpact);
            }
        }
        gRacePlayers[arg0->targetPlayerIndex].state.isTargetedByAttack = 1;
        addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)renderGhostSlowdownActor, (void *)arg0);
    } else if (gRaceUpdatePaused == 0) {
        arg0->timer--;
    }
}

void initGhostSlowdownActor(RaceUiPodiumTrailActor *arg0) {
    arg0->state = 1;
    arg0->height = 0;
    arg0->velocity = 0;
    arg0->playImpactSound = 1;
    enqueuePositionalSoundEffect(SFX_GHOST_SLOWDOWN, &gRacePlayers[arg0->playerIndex].state.pos, 0x7F, SFX_PRIORITY_NORMAL);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateGhostSlowdownRise);
}

void spawnPanItemTargets(s16 arg0) {
    s32 spawnIndex;
    s32 playerIndex;
    RaceUiPodiumTrailActor *actor;

    spawnIndex = 0;
    playerIndex = 0;
    if (gRacePlayerCount > 0) {
        do {
            if (arg0 != playerIndex) {
                actor = createCallbackTaskPreservingArgs((CallbackTaskCallback)initGhostSlowdownActor, 0, 0x1E);
                if (actor != NULL) {
                    actor->playerIndex = arg0;
                    actor->targetPlayerIndex = playerIndex;
                    actor->timer = spawnIndex * 3;
                    spawnIndex += 1;
                }
            }
            playerIndex += 1;
        } while (playerIndex < gRacePlayerCount);
    }
}

void drawRaceUiScorePopup(RaceUiPopupActor *arg0) {
    volatile u8 padding[0x20];
    char buffer[8];
    s32 i;

    if (gCurrentViewportIndex == arg0->playerIndex) {
        drawAssetTableSprite(
            (s16)(arg0->x - 0x40),
            (s16)(arg0->y.word - 8),
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            0x8B
        );
        drawAssetTableSprite((s16)(arg0->x + 0x28), (s16)(arg0->y.word - 8),
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]), 0x8C);
        sprintf(buffer, D_800E14D0, arg0->parent);
        for (i = 0; i != 4; i++) {
            if (buffer[i] != ' ') {
                drawAssetTableSpriteWithExplicitPalette(
                    (s16)(arg0->x + (i * 8) - 8),
                    (s16)(arg0->y.word - 8),
                    getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                    buffer[i] - 5,
                    0xE
                );
            }
        }
        drawAssetTableSpriteWithExplicitPalette(
            (s16)(arg0->x + (i * 8) - 8),
            (s16)(arg0->y.word - 8),
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            0x37,
            0xE
        );
    }
}

void updateRaceUiScorePopupSlideOut(RaceUiPopupActor *arg0) {
    arg0->x += arg0->velocity;
    arg0->velocity += 4;
    if (arg0->velocity == 0x38) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceUiScorePopup, arg0);
    }
}

void updateRaceUiScorePopupHold(RaceUiPopupActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiScorePopupSlideOut);
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceUiScorePopup, arg0);
}

void updateRaceUiScorePopupSlideIn(RaceUiPopupActor *arg0) {
    arg0->x -= arg0->velocity;
    arg0->velocity -= 4;
    if (arg0->velocity == 0) {
        arg0->timer = 0x2D;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiScorePopupHold);
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawRaceUiScorePopup, arg0);
}

void initRaceUiScorePopup(RaceUiPopupActor *arg0) {
    if (gViewportStates[arg0->playerIndex].active != 0) {
        if (gPlayerCount == 1) {
            arg0->y.word = -0x24;
        } else {
            arg0->y.word = -0x16;
        }
        arg0->velocity = 0x38;
        arg0->x = 0x1A4;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiScorePopupSlideIn);
    } else {
        removeCallbackTask(arg0);
    }
}

void spawnRaceUiScorePopup(void *arg0, s16 arg1) {
    RaceUiPopupActor *temp = createCallbackTask((CallbackTaskCallback)initRaceUiScorePopup, 0, 0x64);
    if (temp != NULL) {
        temp->parent = arg0;
        temp->playerIndex = arg1;
    }
}

void renderDizzyLandCarousel(DizzyLandCarouselActor *arg0) {
    s16 unused;
    Transform3D spAC;
    Transform3D sp8C;
    Transform3D sp6C;
    s32 sine;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            makeFixedRotationY(spAC.rotation, arg0->yaw);
            spAC.translation.x = arg0->pos.x;
            spAC.translation.y = arg0->pos.y;
            spAC.translation.z = arg0->pos.z;

            sp8C = sp6C = spAC;

            sine = fixedSine((s16)(arg0->yaw << 4)) << 7;
            sp8C.translation.y = (sp8C.translation.y - sine) + 0x80000;
            sp6C.translation.y += sine + 0x80000;

            arg0->canopyMatrix = allocFixedTransformMatrix(&spAC);
            arg0->negativeSineHorsesMatrix = allocFixedTransformMatrix(&sp8C);
            arg0->positiveSineHorsesMatrix = allocFixedTransformMatrix(&sp6C);
        }

        if ((arg0->canopyMatrix != NULL) && (arg0->negativeSineHorsesMatrix != NULL) &&
            (arg0->positiveSineHorsesMatrix != NULL)) {
            gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0x8)));
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0x9)));
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->canopyMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &DIZZY_LAND_CAROUSEL_CANOPY_DISPLAY_LIST_VRAM);
            gSPMatrix(
                RACE_UI_TRAIL_GFX_ALLOC_PTR++,
                arg0->negativeSineHorsesMatrix,
                G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW
            );
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &DIZZY_LAND_CAROUSEL_HORSES_NEGATIVE_SINE_DISPLAY_LIST_VRAM);
            gSPMatrix(
                RACE_UI_TRAIL_GFX_ALLOC_PTR++,
                arg0->positiveSineHorsesMatrix,
                G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW
            );
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &DIZZY_LAND_CAROUSEL_HORSES_POSITIVE_SINE_DISPLAY_LIST_VRAM);
        }
    }
}

void updateDizzyLandCarousel(DizzyLandCarouselActor *actor) {
    actor->yaw += 4;
    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)renderDizzyLandCarousel, (void *)actor);
}

void initDizzyLandCarousel(DizzyLandCarouselActor *actor) {
    actor->yaw = 0;
    actor->pos = gDizzyLandCarouselPositions[actor->task.userId];
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateDizzyLandCarousel);
}

void renderRaceUiHeavyKnockdownTrailEffect(RaceUiRankTrailActor *arg0) {
    volatile u8 pad[0x20];
    RaceUiDisplayCommand *unused;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
    }

    if (arg0->matrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &_ADDR_20019C0_VRAM);
    }
}

void updateRaceUiHeavyKnockdownTrailEffect(RaceUiRankTrailActor *arg0) {
    RacePlayer *player;
    s32 i;

    arg0->copyBlock = gRacePlayers[arg0->task.userId].state.modelPartTransforms[0];
    arg0->copyBlock.rotation[0] = arg0->copyBlock.rotation[0] / 4;
    arg0->copyBlock.rotation[1] = arg0->copyBlock.rotation[1] / 4;
    arg0->copyBlock.rotation[2] = arg0->copyBlock.rotation[2] / 4;
    arg0->copyBlock.rotation[3] = arg0->copyBlock.rotation[3] / 4;
    arg0->copyBlock.rotation[4] = arg0->copyBlock.rotation[4] / 4;
    arg0->copyBlock.rotation[5] = arg0->copyBlock.rotation[5] / 4;
    arg0->copyBlock.rotation[6] = arg0->copyBlock.rotation[6] / 4;
    arg0->copyBlock.rotation[7] = arg0->copyBlock.rotation[7] / 4;
    arg0->copyBlock.rotation[8] = arg0->copyBlock.rotation[8] / 4;

    player = &gRacePlayers[arg0->task.userId];
    if (player->state.stateFlags & 0x400000) {
        addRenderCallback(
            &gRaceModelEffectRenderCallbackList,
            (RenderCallback)renderRaceUiHeavyKnockdownTrailEffect,
            (void *)arg0
        );
        return;
    }

    enqueuePositionalSoundEffect(SFX_ITEM_IMPACT, &player->state.pos, 0x7F, SFX_PRIORITY_NORMAL);

    for (i = 0; i < 8; i++) {
        spawnRaceUiAltBurstTextParticle(
            arg0->copyBlock.translation.x,
            arg0->copyBlock.translation.y,
            arg0->copyBlock.translation.z,
            i
        );
    }

    removeCallbackTask(arg0);
}

void initRaceUiHeavyKnockdownTrailEffect(void *arg0) {
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiHeavyKnockdownTrailEffect);
}

void renderRaceUiThrownTrailObstacle(RaceUiThrownTrailActor *arg0) {
    volatile s32 padlow;
    s32 sine;
    Transform3D sp80;
    volatile s32 pad[0x12];

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            sine = fixedSine(arg0->spin);
            makeFixedRotationY(sp80.rotation, (s16)(arg0->angle + 0x800));
            sp80.translation.x = arg0->pos.x;
            sp80.translation.y = arg0->pos.y + ((sine + 0x1000) << 5) + 0x10000;
            sp80.translation.z = arg0->pos.z;
            scaleFixedMatrix3sByQuarter(sp80.rotation);
            arg0->matrix = allocFixedTransformMatrix(&sp80);
        }

        if (arg0->matrix != NULL) {
            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
            gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
            gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gRegionAllocPtr++, &_ADDR_20019C0_VRAM);
        }
    }
}

void updateRaceUiThrownTrailObstacle(RaceUiThrownTrailActor *arg0) {
    volatile s32 pad[4];
    volatile s16 unused0;
    volatile s16 unused1;
    Vec3i *pos;

    if (gRaceUpdatePaused == 0) {
        pos = &arg0->pos;
        if (isPositionNearAnyRaceViewportFocus(pos) != 0) {
            pushRacePlayersOutOfCylinderAndApplyItemHit(pos, 0x1A0000, 0x600000, 2);
            if (arg0->soundTimer == 0) {
                enqueuePositionalSoundEffect(SFX_GHOST_SLOWDOWN, pos, 0x7F, SFX_PRIORITY_LOW);
                createThrownTrailImpactProjectile(
                    arg0->pos.x,
                    arg0->pos.y + 0x700000,
                    arg0->pos.z,
                    arg0->surface,
                    arg0->angle
                );
                arg0->soundTimer = 0xF;
            } else {
                arg0->soundTimer--;
            }
        }

        arg0->angle += arg0->angleStep;
        if (arg0->targetAngle + 0x140 < arg0->angle) {
            arg0->angleStep = -8;
        }
        if (arg0->angle < arg0->targetAngle - 0x140) {
            arg0->angleStep = 8;
        }
        arg0->spin += 0x40;
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, (RenderCallback)renderRaceUiThrownTrailObstacle, (void *)arg0);
}

void initRaceUiThrownTrailObstacle(RaceUiThrownTrailActor *arg0) {
    arg0->soundTimer = 0xF;
    arg0->pos.y = getRaceCourseSurfaceHeight(arg0->surface, arg0->pos.x, arg0->pos.z);
    arg0->angleStep = 8;
    arg0->angle = arg0->angleStep;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiThrownTrailObstacle);
}

void spawnRaceUiThrownTrailObstacle(s16 arg0, void *arg1, void *arg2, s16 arg3) {
    RaceUiThrownTrailActor *temp = createCallbackTask((CallbackTaskCallback)initRaceUiThrownTrailObstacle, 0, 0x64);
    if (temp != NULL) {
        temp->pos.x = (s32)arg1;
        temp->pos.z = (s32)arg2;
        temp->targetAngle = (arg3 + 0x800) & 0xFFF;
        temp->surface = arg0;
    }
}

void renderRaceUiSingleTrailEffect(RaceUiSingleTrailActor *arg0) {
    volatile u8 pad[0x20];
    RaceUiDisplayCommand *unused;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
    }

    if (arg0->matrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &_ADDR_2003538_VRAM);
    }
}

void updateRaceUiSingleTrailEffect(RaceUiSingleTrailActor *arg0) {
    composeFixedTransforms(
        &arg0->localTransform,
        &gRacePlayers[arg0->task.userId].state.modelPartTransforms[7],
        &arg0->copyBlock
    );

    if (gRaceUpdatePaused == 0) {
        arg0->timer--;
        if (!(gRacePlayers[arg0->task.userId].state.stateFlags & 0x2000)) {
            arg0->timer = 0;
        }
    }

    if (arg0->timer == 0) {
        removeCallbackTask(arg0);
        return;
    }

    addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)renderRaceUiSingleTrailEffect, (void *)arg0);
}

void initRaceUiSingleTrailEffect(RaceUiSingleTrailActor *arg0) {
    arg0->timer = RACE_UI_SINGLE_TRAIL_TIMER;
    makeFixedRotationY(arg0->localTransform.rotation, RACE_UI_SINGLE_TRAIL_LOCAL_ROT_Y);
    arg0->localTransform.translation.x = RACE_UI_SINGLE_TRAIL_LOCAL_X;
    arg0->localTransform.translation.y = RACE_UI_SINGLE_TRAIL_LOCAL_Y;
    arg0->localTransform.translation.z = RACE_UI_SINGLE_TRAIL_LOCAL_Z;
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiSingleTrailEffect);
}

void renderRaceUiFadingImpact(RaceUiFadingImpactActor *arg0) {
    volatile u8 pad[0x20];
    RaceUiDisplayCommand *unused;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
    }

    if (arg0->matrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetPrimColor(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->alpha);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &_ADDR_20035F8_VRAM);
    }
}

void updateRaceUiFadingImpact(RaceUiFadingImpactActor *arg0) {
    volatile u8 pad[0x28];
    s16 scale;
    Vec3i *sp24;
    s32 sp2C;
    RaceUiFadingImpactActor *new_var;

    if (gRaceUpdatePaused == 0) {
        arg0->angle += 0x100;
        makeFixedRotationY(arg0->copyBlock.rotation, arg0->angle);

        scale = arg0->scale;
        arg0->copyBlock.rotation[0] = (arg0->copyBlock.rotation[0] * scale) / 64;
        arg0->copyBlock.rotation[1] = (arg0->copyBlock.rotation[1] * scale) / 64;
        arg0->copyBlock.rotation[2] = (arg0->copyBlock.rotation[2] * scale) / 64;
        new_var = arg0;
        arg0->copyBlock.rotation[3] = (arg0->copyBlock.rotation[3] * scale) / 64;
        new_var->copyBlock.rotation[4] = (new_var->copyBlock.rotation[4] * scale) / 64;
        arg0->copyBlock.rotation[5] = (new_var->copyBlock.rotation[5] * scale) / 64;
        new_var->copyBlock.rotation[6] = (new_var->copyBlock.rotation[6] * scale) / 64;
        arg0->copyBlock.rotation[7] = (arg0->copyBlock.rotation[7] * scale) / 64;
        new_var->copyBlock.rotation[8] = (arg0->copyBlock.rotation[8] * scale) / 64;

        if (scale != 0x34) {
            new_var->scale = scale + 4;
            scale = new_var->scale;
        }

        if (scale >= 0x21) {
            arg0->alpha -= 0x10;
        }

        if (arg0->alpha >= 0x71) {
            sp24 = &arg0->pos;
            sp2C = (new_var->scale * 0x480000) / 64;
            if (tryApplyRacePlayerItemHit(sp24, sp2C, 0x80, 0) != 0) {
                gRacePlayers[0].state.hitSourcePlayerIndex = new_var->task.userId;
            }
            if (tryApplyRacePlayerItemHit(sp24, sp2C, 0x80, 1) != 0) {
                gRacePlayers[1].state.hitSourcePlayerIndex = new_var->task.userId;
            }
            if (tryApplyRacePlayerItemHit(sp24, sp2C, 0x80, 2) != 0) {
                gRacePlayers[2].state.hitSourcePlayerIndex = arg0->task.userId;
            }
            if (tryApplyRacePlayerItemHit(sp24, sp2C, 0x80, 3) != 0) {
                gRacePlayers[3].state.hitSourcePlayerIndex = arg0->task.userId;
            }
        }
    }

    if (arg0->alpha <= 0) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gModelRenderCallbackQueue7Head, (RenderCallback)renderRaceUiFadingImpact, (void *)arg0);
    }
}

void initRaceUiFadingImpact(RaceUiFadingImpactActor *actor) {
    actor->scale = 4;
    actor->alpha = 0xFF;
    actor->angle = 0;
    actor->copyBlock.translation.x = actor->pos.x;
    actor->copyBlock.translation.y = actor->pos.y;
    actor->copyBlock.translation.z = actor->pos.z;
    enqueuePositionalSoundEffect(SFX_AREA_BLAST, &actor->pos, 0x7F, SFX_PRIORITY_NORMAL);
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiFadingImpact);
}

void spawnRaceUiFadingImpact(s32 x, s32 y, s32 z, s32 userId) {
    RaceUiFadingImpactActor *actor = createCallbackTaskWithUserIdPreservingArgs(initRaceUiFadingImpact, 0, 3, userId);
    if (actor != NULL) {
        actor->pos.x = x;
        actor->pos.y = y;
        actor->pos.z = z;
    }
}

void renderRaceUiSpinHitTransitionEffect(RaceUiTransitionActor *arg0) {
    volatile u8 pad[0x20];
    RaceUiDisplayCommand *unused;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        arg0->matrix = allocFixedTransformMatrix(&arg0->transformSource);
    }

    if (arg0->matrix != NULL) {
        gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0xA)));
        gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0xB)));
        gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &_ADDR_2002490_VRAM);
    }
}

void updateRaceUiSpinHitTransitionEffect(RaceUiTransitionActor *arg0) {
    RacePlayer *player;
    s16 temp_v0;
    s16 temp_v1;
    s16 temp_a0;

    if (gRaceUpdatePaused == 0) {
        if (!(gRacePlayers[arg0->task.userId].state.stateFlags & 0x800000)) {
            arg0->unk6C = 0;
        }
        if (arg0->unk6C != 0) {
            player = &gRacePlayers[arg0->task.userId];
            arg0->transformSource.translation.x = player->state.modelPartTransforms[1].translation.x;
            arg0->transformSource.translation.y = player->state.modelPartTransforms[1].translation.y;
            arg0->transformSource.translation.z = player->state.modelPartTransforms[1].translation.z;
        } else {
            if (arg0->unk6E == 0) {
                removeCallbackTask(arg0);
                return;
            }
            temp_v0 = arg0->transformSource.rotation[1];
            temp_v1 = arg0->transformSource.rotation[4];
            temp_a0 = arg0->transformSource.rotation[7];
            arg0->unk68 -= 0x1000;
            arg0->transformSource.translation.y += arg0->unk68;
            arg0->transformSource.rotation[1] = temp_v0 - (temp_v0 / 16);
            arg0->transformSource.rotation[4] = temp_v1 - (temp_v1 / 16);
            arg0->transformSource.rotation[7] = temp_a0 - (temp_a0 / 16);
        }
    }
    addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)renderRaceUiSpinHitTransitionEffect, arg0);
}

void initRaceUiSpinHitTransitionEffect(RaceUiTransitionActor *arg0) {
    s16 *transform = arg0->transformSource.rotation;
    volatile u8 padding[0x20];

    arg0->unk68 = 0;
    makeFixedRotationX(transform, 0x400);
    scaleFixedMatrix3sByQuarter(transform);
    arg0->unk6C = 1;
    arg0->unk6E = 0xF;
    enqueuePositionalSoundEffect(SFX_SPIN_HIT, &gRacePlayers[arg0->task.userId].state.pos, 0x7F, SFX_PRIORITY_NORMAL);
    updateRaceUiSpinHitTransitionEffect(arg0);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiSpinHitTransitionEffect);
}

void renderRaceUiStunOrbitingIcon(RaceUiOrbitingSpriteActor *arg0) {
    volatile u8 padding[4];
    Transform3D sp64;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        sp64 = gIdentityFixedTransform;
        sp64.translation.x = arg0->pos.x;
        sp64.translation.y = arg0->pos.y;
        sp64.translation.z = arg0->pos.z;
        arg0->matrix = allocFixedTransformMatrix(&sp64);
    }

    if (arg0->matrix != NULL) {
        gSPDisplayList(gRegionAllocPtr++, gAlphaSpriteRenderModeDl);
        gDPLoadTextureBlock_4b(
            gRegionAllocPtr++,
            arg0->image,
            G_IM_FMT_CI,
            16,
            16,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            0,
            0,
            G_TX_NOLOD,
            G_TX_NOLOD
        );
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette);
        gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPVertex(gRegionAllocPtr++, gRaceUiStunIconQuadVertices, 4, 0);
        gSP1Quadrangle(gRegionAllocPtr++, 3, 2, 1, 0, 0);
    }
}

void updateRaceUiStunOrbitingIcon(RaceUiOrbitingSpriteActor *arg0) {
    RacePlayer *player;

    player = &gRacePlayers[arg0->playerIndex];
    if (!(player->state.stateFlags & 0x10000)) {
        removeCallbackTask(arg0);
        return;
    }

    transformVec3iByFixedMatrix(player->state.modelPartTransforms[7].rotation, &gRaceUiStunIconHeadOffset, &arg0->pos);
    player = &gRacePlayers[arg0->playerIndex];
    arg0->pos.x += player->state.modelPartTransforms[7].translation.x;
    arg0->pos.y += player->state.modelPartTransforms[7].translation.y + 0x80000;
    arg0->pos.z += player->state.modelPartTransforms[7].translation.z;
    arg0->angle += 0xC0;
    arg0->pos.x -= fixedSine(arg0->angle) << 7;
    arg0->pos.z += fixedCosine(arg0->angle) << 7;
    addRenderCallback(&gModelRenderCallbackQueue7Head, (RenderCallback)renderRaceUiStunOrbitingIcon, (void *)arg0);
}

void initRaceUiStunOrbitingIcon(RaceUiOrbitingSpriteActor *arg0) {
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(ASSET_HANDLE(0x1C)), 0x35, &arg0->image, &arg0->palette);
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiStunOrbitingIcon);
}

void spawnRaceUiStunOrbitingIcons(s16 playerIndex) {
    RaceUiOrbitingSpriteActor *icon;

    icon = createCallbackTaskPreservingArgs((CallbackTaskCallback)initRaceUiStunOrbitingIcon, 0, 4);
    if (icon != NULL) {
        icon->playerIndex = playerIndex;
        icon->angle = RACE_UI_STUN_ORBIT_ANGLE_0;
    }
    icon = createCallbackTaskPreservingArgs((CallbackTaskCallback)initRaceUiStunOrbitingIcon, 0, 4);
    if (icon != NULL) {
        icon->playerIndex = playerIndex;
        icon->angle = RACE_UI_STUN_ORBIT_ANGLE_1;
    }
    icon = createCallbackTaskPreservingArgs((CallbackTaskCallback)initRaceUiStunOrbitingIcon, 0, 4);
    if (icon != NULL) {
        icon->playerIndex = playerIndex;
        icon->angle = RACE_UI_STUN_ORBIT_ANGLE_2;
    }
}

void renderDizzyLandTeacupBumper(DizzyLandTeacupBumperActor *arg0) {
    struct {
        Transform3D transform;
        s16 unused[2];
    } scratch;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            makeFixedRotationY(scratch.transform.rotation, arg0->yaw);
            scratch.transform.rotation[0] = SCALE_MATRIX_COMPONENT(scratch.transform.rotation[0], arg0->xzScale);
            scratch.transform.rotation[3] = SCALE_MATRIX_COMPONENT(scratch.transform.rotation[3], arg0->xzScale);
            scratch.transform.rotation[6] = SCALE_MATRIX_COMPONENT(scratch.transform.rotation[6], arg0->xzScale);
            scratch.transform.rotation[2] = SCALE_MATRIX_COMPONENT(scratch.transform.rotation[2], arg0->xzScale);
            scratch.transform.rotation[5] = SCALE_MATRIX_COMPONENT(scratch.transform.rotation[5], arg0->xzScale);
            scratch.transform.rotation[8] = SCALE_MATRIX_COMPONENT(scratch.transform.rotation[8], arg0->xzScale);
            scratch.transform.translation.x = arg0->pos.x;
            scratch.transform.translation.y = arg0->pos.y;
            scratch.transform.translation.z = arg0->pos.z;
            arg0->matrix = allocFixedTransformMatrix(&scratch.transform);
        }

        if (arg0->matrix != NULL) {
            gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0x8)));
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0x9)));
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &DIZZY_LAND_TEACUP_BUMPER_DISPLAY_LIST_VRAM);
        }
    }
}

void updateDizzyLandTeacupBumper(DizzyLandTeacupBumperActor *arg0) {
    DizzyLandTeacupBumperActor *actor;
    Vec3i *pos;
    s16 scale;

    actor = arg0;
    if (gRaceUpdatePaused == 0) {
        pos = &actor->pos;
        if (isPositionNearAnyRaceViewportFocus(pos) != 0) {
            actor->yaw += actor->yawStep;

            if (isRacePlayerInsideCylinder(pos, 0x200000, 0xF0000, 0) != 0) {
                actor->xzScale = 0x1800;
                enqueuePositionalSoundEffect(SFX_TEACUP_BUMP, pos, 0x7F, SFX_PRIORITY_NORMAL);
                pushRacePlayerOutOfCylinder(pos, 0x2C0000, 0xF0000, 0);
            }
            if (isRacePlayerInsideCylinder(pos, 0x200000, 0xF0000, 1) != 0) {
                actor->xzScale = 0x1800;
                enqueuePositionalSoundEffect(SFX_TEACUP_BUMP, pos, 0x7F, SFX_PRIORITY_NORMAL);
                pushRacePlayerOutOfCylinder(pos, 0x2C0000, 0xF0000, 1);
            }
            if (isRacePlayerInsideCylinder(pos, 0x200000, 0xF0000, 2) != 0) {
                actor->xzScale = 0x1800;
                enqueuePositionalSoundEffect(SFX_TEACUP_BUMP, pos, 0x7F, SFX_PRIORITY_NORMAL);
                pushRacePlayerOutOfCylinder(pos, 0x2C0000, 0xF0000, 2);
            }
            if (isRacePlayerInsideCylinder(pos, 0x200000, 0xF0000, 3) != 0) {
                actor->xzScale = 0x1800;
                enqueuePositionalSoundEffect(SFX_TEACUP_BUMP, pos, 0x7F, SFX_PRIORITY_NORMAL);
                pushRacePlayerOutOfCylinder(pos, 0x2C0000, 0xF0000, 3);
            }

            scale = actor->xzScale;
            if (scale != 0x1000) {
                actor->xzScale = scale - 0x100;
            }
        }
    }
    addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)renderDizzyLandTeacupBumper, actor);
}

void initDizzyLandTeacupBumper(DizzyLandTeacupBumperActor *bumper) {
    bumper->yaw = randomNextSecondary() << 4;
    if (randomNextSecondary() & 1) {
        bumper->yawStep = DIZZY_LAND_TEACUP_BUMPER_ROTATION_STEP;
    } else {
        bumper->yawStep = -DIZZY_LAND_TEACUP_BUMPER_ROTATION_STEP;
    }
    bumper->xzScale = DIZZY_LAND_TEACUP_BUMPER_BASE_SCALE;
    bumper->pos = gDizzyLandTeacupBumperPositions[bumper->task.userId];
    setCallbackTaskCallback(bumper, (CallbackTaskCallback)updateDizzyLandTeacupBumper);
}

void renderDizzyLandFerrisWheel(DizzyLandFerrisWheelActor *arg0) {
    struct {
        Transform3D transform;
        s16 unused[2];
    } scratch;
    Vec3i transformedOffset;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            makeFixedRotationY(scratch.transform.rotation, arg0->yaw);
            scratch.transform.translation.x = arg0->pos.x;
            scratch.transform.translation.y = arg0->pos.y;
            scratch.transform.translation.z = arg0->pos.z;
            arg0->supportMatrix = allocFixedTransformMatrix(&scratch.transform);

            transformVec3iByFixedMatrix(
                scratch.transform.rotation,
                &gDizzyLandFerrisWheelHubOffset,
                &transformedOffset
            );
            scratch.transform.translation.x += transformedOffset.x;
            scratch.transform.translation.y += transformedOffset.y;
            scratch.transform.translation.z += transformedOffset.z;
            makeFixedRotationZY(scratch.transform.rotation, arg0->yaw, arg0->wheelAngle);
            arg0->wheelMatrix = allocFixedTransformMatrix(&scratch.transform);
        }

        if (arg0->wheelMatrix != NULL) {
            gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0x8)));
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0x9)));
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->supportMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &DIZZY_LAND_FERRIS_WHEEL_SUPPORT_DISPLAY_LIST_VRAM);
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->wheelMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, &DIZZY_LAND_FERRIS_WHEEL_DISPLAY_LIST_VRAM);
        }
    }
}

void updateDizzyLandFerrisWheel(DizzyLandFerrisWheelActor *arg0) {
    if (gRaceUpdatePaused == 0) {
        arg0->wheelAngle += 8;
    }
    addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)renderDizzyLandFerrisWheel, (void *)arg0);
}

void initDizzyLandFerrisWheel(DizzyLandFerrisWheelActor *arg0) {
    arg0->pos = gDizzyLandFerrisWheelPositions[arg0->task.userId];
    arg0->yaw = gDizzyLandFerrisWheelYaws[arg0->task.userId];
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateDizzyLandFerrisWheel);
}

void renderDizzyLandSpinningCabinRide(DizzyLandSpinningCabinRideActor *arg0) {
    struct {
        Transform3D transform;
        s16 unused[2];
    } scratch;
    s32 temp2;
    s32 pad;
    s32 pad2;
    SplitWord temp;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) == 0) {
        return;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        makeFixedRotationY(scratch.transform.rotation, arg0->baseYaw);
        scratch.transform.translation.x = arg0->pos.x;
        scratch.transform.translation.y = arg0->pos.y;
        scratch.transform.translation.z = arg0->pos.z;
        arg0->poleMatrix = allocFixedTransformMatrix(&scratch.transform);

        scratch.transform.translation.y += 0x01000000;
        temp.half.lo = fixedSine(arg0->pitchPhase) >> 5;
        temp2 = fixedSine(arg0->rollPhase) >> 5;
        makeFixedRotationYZX(scratch.transform.rotation, temp.half.lo, arg0->spinAngle, temp2);
        arg0->canopyMatrix = allocFixedTransformMatrix(&scratch.transform);
    }

    if (arg0->canopyMatrix != NULL) {
        gDPPipeSync(gRegionAllocPtr++);
        gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0x8)));
        gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0x9)));
        gSPMatrix(gRegionAllocPtr++, arg0->poleMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, &DIZZY_LAND_SPINNING_CABIN_RIDE_POLE_DISPLAY_LIST_VRAM);
        gSPMatrix(gRegionAllocPtr++, arg0->canopyMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, &DIZZY_LAND_SPINNING_CABIN_RIDE_CANOPY_DISPLAY_LIST_VRAM);
    }
}

void updateDizzyLandSpinningCabinRide(DizzyLandSpinningCabinRideActor *arg0) {
    if (gRaceUpdatePaused == 0) {
        arg0->spinAngle += 0x60;
        arg0->pitchPhase += 0x10;
        arg0->rollPhase += 4;
    }
    addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)renderDizzyLandSpinningCabinRide, arg0);
}

void initDizzyLandSpinningCabinRide(DizzyLandSpinningCabinRideActor *arg0) {
    arg0->pos = gDizzyLandSpinningCabinRidePositions[arg0->task.userId];
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateDizzyLandSpinningCabinRide);
}

void renderCourseStartFinishSprite(RaceUiCourseSpriteActor *arg0) {
    volatile u8 padding[4];
    s32 imageIndex;
    RaceUiCourseSpriteActor *new_var;

    new_var = arg0;

    if (gRenderMatricesDirty != 0) {
        new_var->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera((Vec3i *)&new_var->x) != 0) {
        if (arg0->matrixDirty != 0) {
            new_var->matrixDirty = 0;
            new_var->vertices = allocMenuRenderScratch(0x40);
            if (new_var->vertices != NULL) {
                new_var->vertices[0] = gCourseStartFinishSpriteVertices[arg0->task.userId * 4];
                arg0->vertices[0].v.tc[0] += arg0->angle << 5;
                new_var->vertices[1] = gCourseStartFinishSpriteVertices[new_var->task.userId * 4 + 1];
                arg0->vertices[1].v.tc[0] += new_var->angle << 5;
                new_var->vertices[2] = gCourseStartFinishSpriteVertices[new_var->task.userId * 4 + 2];
                arg0->vertices[2].v.tc[0] += arg0->angle << 5;
                new_var->vertices[3] = gCourseStartFinishSpriteVertices[new_var->task.userId * 4 + 3];
                arg0->vertices[3].v.tc[0] += new_var->angle << 5;
            }
        }

        if (new_var->vertices != NULL) {
            imageIndex = 0;
            if ((gRacePlayers[gCurrentViewportIndex].state.lapDigit + 1) >= gRaceLapCount) {
                imageIndex = 1;
            }

            gSPDisplayList(gRegionAllocPtr++, gAlphaSpriteRenderModeDl);
            gSPMatrix(gRegionAllocPtr++, &gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, new_var->palettes[imageIndex]);
            gDPLoadTextureBlock_4b(
                gRegionAllocPtr++,
                new_var->images[imageIndex],
                G_IM_FMT_CI,
                64,
                16,
                0,
                G_TX_WRAP,
                G_TX_CLAMP,
                6,
                G_TX_NOMASK,
                G_TX_NOLOD,
                G_TX_NOLOD
            );
            gSPVertex(gRegionAllocPtr++, new_var->vertices, 4, 0);
            gSP1Quadrangle(gRegionAllocPtr++, 3, 2, 1, 0, 0);
            gSP1Quadrangle(gRegionAllocPtr++, 1, 2, 3, 0, 0);
        }
    }
}

void updateCourseStartFinishSprite(RaceUiCourseSpriteActor *actor) {
    if ((gFrameCounter & COURSE_START_FINISH_SCROLL_FRAME_MASK) == 0) {
        actor->angle = (actor->angle + COURSE_START_FINISH_SCROLL_STEP) & COURSE_START_FINISH_SCROLL_MASK;
    }
    addRenderCallback(&gModelRenderCallbackQueue4Head, (RenderCallback)renderCourseStartFinishSprite, actor);
}

void initCourseStartFinishSprite(RaceUiCourseSpriteActor *actor) {
    s32 unused;
    s16 angle;

    switch (actor->task.userId) {
        case 0:
            getRaceCourseSurfaceSpawnTransform(0, &actor->x, &actor->y, &actor->z, &angle);
            getAssetTableImageAndPalette(
                getRelocatableHeapBlockBase(ASSET_HANDLE(0x1D)),
                0,
                &actor->images[0],
                &actor->palettes[0]
            );
            actor->images[1] = actor->images[0];
            actor->palettes[1] = actor->palettes[0];
            break;
        case 1:
            getRaceCourseSurfaceSpawnTransform(
                gRaceCourseStartEntries[gRaceCourseIndex.signedValue].pathIndex,
                &actor->x,
                &actor->y,
                &actor->z,
                &angle
            );
            getAssetTableImageAndPalette(
                getRelocatableHeapBlockBase(ASSET_HANDLE(0x1D)),
                1,
                &actor->images[0],
                &actor->palettes[0]
            );
            getAssetTableImageAndPalette(
                getRelocatableHeapBlockBase(ASSET_HANDLE(0x1D)),
                2,
                &actor->images[1],
                &actor->palettes[1]
            );
            break;
    }
    unused = 0;
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateCourseStartFinishSprite);
}

void renderSilverMountainSnowParticles(RaceUiEffectParticleActor *arg0) {
    RaceUiEffectParticleActor *actor;
    s32 cameraX;
    s32 cameraY;
    s32 cameraZ;
    union {
        Transform3D transform;
        s64 forceAlignment;
    } transform;
    s32 i;
    Mtx *matrix;

    actor = arg0;
    gDPLoadTextureBlock_4b(
        gRegionAllocPtr++,
        actor->unk20,
        G_IM_FMT_CI,
        32,
        32,
        0,
        G_TX_CLAMP,
        G_TX_CLAMP,
        0,
        0,
        G_TX_NOLOD,
        G_TX_NOLOD
    );
    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, actor->unk1C);
    gSPDisplayList(gRegionAllocPtr++, gAlphaSpriteRenderModeDl);

    transform.transform = gIdentityFixedTransform;
    cameraX = -gRaceCameras[gCurrentViewportIndex].cameraTransform.translation.x;
    cameraY = -gRaceCameras[gCurrentViewportIndex].cameraTransform.translation.y;
    cameraZ = -gRaceCameras[gCurrentViewportIndex].cameraTransform.translation.z;

    i = 0;
    if (actor->count > 0) {
        do {
            transform.transform.translation.x =
                ((actor->particles[i].unk0 - (cameraX & 0xFFFFFF)) & 0xFFFFFF) + cameraX + 0xFF800000;
            transform.transform.translation.y =
                ((actor->particles[i].unk4 - (cameraY & 0xFFFFFF)) & 0xFFFFFF) + cameraY + 0xFF800000;
            transform.transform.translation.z =
                ((actor->particles[i].unk8 - (cameraZ & 0xFFFFFF)) & 0xFFFFFF) + cameraZ + 0xFF800000;
            matrix = allocFixedTransformMatrix(&transform.transform);
            if (matrix != NULL) {
                gSPMatrix(gRegionAllocPtr++, matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPVertex(gRegionAllocPtr++, gSilverMountainSnowParticleVertices, 3, 0);
                gSP1Triangle(gRegionAllocPtr++, 0, 2, 1, 0);
                actor = arg0;
            }
            i++;
        } while (i < actor->count);
    }
}

void updateSilverMountainSnowParticles(RaceUiEffectParticleActor *arg0) {
    register RaceUiEffectParticleActor *actor;
    register s32 i;

    actor = arg0;
    for (i = 0; i < actor->count; i++) {
        actor->particles[i].unk0 += 0xA0000;
        if (actor->particles[i].unk0 >= 0x1000000) {
            actor->particles[i].unk8 = randomNextMain() << 0x10;
            actor->particles[i].unk4 = randomNextMain() << 0x10;
            actor->particles[i].unk0 = 0;
        }
        actor->particles[i].unk4 += 0xFFFD0000;
        actor->particles[i].unk4 &= 0xFFFFFF;
    }
    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)renderSilverMountainSnowParticles, (void *)actor);
}

void initSilverMountainSnowParticles(RaceUiEffectParticleActor *actor) {
    s32 i;

    if (gPlayerCount != 0) {
        actor->count = 0xF;
    } else {
        actor->count = 0x3C;
    }

    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x42, &actor->unk20, &actor->unk1C);
    gAssetHandles[0x25] = allocRelocatableHeapBlock(actor->count * sizeof(RaceUiEffectParticle));
    actor->particles = (RaceUiEffectParticle *)getRelocatableHeapBlockBase(gAssetHandles[0x25]);

    for (i = 0; i < actor->count; i++) {
        actor->particles[i].unk0 = randomNextMain() << 0x10;
        actor->particles[i].unk4 = randomNextMain() << 0x10;
        actor->particles[i].unk8 = randomNextMain() << 0x10;
    }

    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateSilverMountainSnowParticles);
}

void renderRaceCourseRankModel(RaceUiRankParticleActor *arg0) {
    RaceUiDisplayCommand *unused;
    s32 displayListIndex;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }

    if (isPositionNearCurrentRaceViewportCamera(&arg0->pos) != 0) {
        if (arg0->matrixDirty != 0) {
            arg0->matrixDirty = 0;
            arg0->matrix = allocFixedTransformMatrix(&arg0->copyBlock);
        }

        if (arg0->matrix != NULL) {
            displayListIndex = 0;
            if (gRaceLapCount <= gRacePlayers[gCurrentViewportIndex].state.lapDigit + 1) {
                displayListIndex = 1;
            }

            gDPPipeSync(RACE_UI_TRAIL_GFX_ALLOC_PTR++);
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x02, getRelocatableHeapBlockBase(ASSET_HANDLE(0x8)));
            gSPSegment(RACE_UI_TRAIL_GFX_ALLOC_PTR++, 0x03, getRelocatableHeapBlockBase(ASSET_HANDLE(0x9)));
            gSPMatrix(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(RACE_UI_TRAIL_GFX_ALLOC_PTR++, arg0->displayLists[displayListIndex]);
        }
    }
}

void updateRaceCourseRankModel(RaceUiRankParticleActor *arg0) {
    addRenderCallback(&gModelRenderCallbackQueue4Head, (RenderCallback)renderRaceCourseRankModel, arg0);
}

void initRaceCourseRankModel(RaceUiRankParticleActor *actor) {
    struct {
        s16 pad;
        s16 angle;
        s32 unused;
    } local;

    switch (actor->task.userId) {
        case 0:
            getRaceCourseSurfaceSpawnTransform(1, &actor->pos.x, &actor->pos.y, &actor->pos.z, &local.angle);
            break;
        case 1:
            getRaceCourseSurfaceSpawnTransform(
                gRaceCourseStartEntries[gRaceCourseIndex.signedValue].pathIndex,
                &actor->pos.x,
                &actor->pos.y,
                &actor->pos.z,
                &local.angle
            );
            break;
    }

    makeFixedRotationY(actor->copyBlock.rotation, local.angle);
    actor->copyBlock.translation.x = actor->pos.x;
    actor->copyBlock.translation.y = actor->pos.y;
    actor->copyBlock.translation.z = actor->pos.z;
    actor->displayLists[0] =
        gRaceCourseRankModelDisplayLists[(actor->task.userId * 2) + (gRaceCourseIndex.signedValue * 4)];
    actor->displayLists[1] =
        gRaceCourseRankModelDisplayLists[(actor->task.userId * 2) + (gRaceCourseIndex.signedValue * 4) + 1];
    setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceCourseRankModel);
}

void setRaceUiProjectileAnimation(RaceUiProjectileActor *arg0, u16 *arg1) {
    arg0->animationScript = arg1;
    arg0->frameTimer = arg1[0];
    arg0->flags = arg1[1];
    getAssetTableImageAndPalette(
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x1E)),
        arg1[2],
        &arg0->image,
        &arg0->palette
    );
}

s32 advanceRaceUiProjectileAnimation(RaceUiProjectileActor *arg0) {
    s16 temp_v0;

    temp_v0 = arg0->frameTimer;
    if (!(temp_v0 & 0x7FFF)) {
        if (temp_v0 & 0x8000) {
            return 1;
        }
        setRaceUiProjectileAnimation(arg0, arg0->animationScript + 3);
        goto block_5;
    }
    arg0->frameTimer = temp_v0 - 1;
block_5:
    return 0;
}

void renderRaceUiProjectile(RaceUiProjectileActor *arg0) {
    volatile s32 pad0;
    Transform3D sp7C;
    volatile u8 padding[4];
    s32 sp74;
    s32 var_ra;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = (float)1;
    }

    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        sp7C = gIdentityFixedTransform;
        sp7C.translation.x = arg0->pos.x;
        sp7C.translation.y = arg0->pos.y;
        sp7C.translation.z = arg0->pos.z;
        arg0->matrix = allocFixedTransformMatrix(&sp7C);
    }

    if (arg0->matrix != NULL) {
        var_ra = 0x20;
        if (arg0->flags & 4) {
            var_ra = 0x40;
            sp74 = 4;
        } else {
            sp74 = 0;
        }

        gSPDisplayList(gRegionAllocPtr++, gAlphaSpriteRenderModeDl);
        gDPLoadTextureBlock_4b(
            gRegionAllocPtr++,
            arg0->image,
            G_IM_FMT_CI,
            32,
            var_ra,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            0,
            0,
            G_TX_NOLOD,
            G_TX_NOLOD
        );
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->palette);
        gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPVertex(gRegionAllocPtr++, (&gRaceUiProjectileVertices[sp74]), 4, 0);
        gSP1Quadrangle(gRegionAllocPtr++, 3, 2, 1, 0, 0);
    }
}

void updateRaceUiProjectileFlyAway(RaceUiProjectileActor *arg0) {
    struct {
        s16 mtx[0x10];
        s32 pad;
    } sp24;
    RaceUiProjectileActor *actor;
    s32 temp;

    actor = arg0;
    if (!gRaceUpdatePaused) {
        makeFixedRotationXY(
            sp24.mtx,
            gRacePlayers[actor->task.userId].state.pitchAngle,
            gRacePlayers[actor->task.userId].state.facingAngle
        );
        transformVec3iByFixedMatrix(sp24.mtx, &actor->velocity, &actor->pos);
        actor->pos.x += gRacePlayers[actor->task.userId].state.renderPosition.x;
        actor->pos.y += gRacePlayers[actor->task.userId].state.renderPosition.y + actor->verticalVelocity;
        actor->pos.z += gRacePlayers[actor->task.userId].state.renderPosition.z;
        temp = (actor->verticalVelocity += actor->verticalAcceleration);
        actor->verticalAcceleration += 0x10000;
        if (temp >= 0x300001) {
            removeCallbackTask(actor);
            return;
        }
    }
    addRenderCallback(&gModelRenderCallbackQueue4Head, (RenderCallback)renderRaceUiProjectile, actor);
}

void updateRaceUiProjectileStealScores(RaceUiProjectileActor *arg0) {
    RacePlayer *player;
    RacePlayer *otherPlayer;
    s16 sp44[0xE];
    s32 value;
    s32 amount;
    s32 i;
    s16 flags;

    if (gRaceUpdatePaused == 0) {
        player = &gRacePlayers[arg0->task.userId];
        makeFixedRotationXY(
            sp44,
            gRacePlayers[arg0->task.userId].state.pitchAngle,
            gRacePlayers[arg0->task.userId].state.facingAngle
        );
        transformVec3iByFixedMatrix(sp44, &arg0->velocity, &arg0->pos);

        player = &gRacePlayers[arg0->task.userId];
        arg0->pos.x += player->state.renderPosition.x;
        arg0->pos.y += player->state.renderPosition.y;
        arg0->pos.z += player->state.renderPosition.z;
        advanceRaceUiProjectileAnimation(arg0);

        i = 0;
        do {
            otherPlayer = &gRacePlayers[i];
            if ((otherPlayer->isActive != 0) && (otherPlayer->playerIndex != arg0->task.userId)) {
                value = otherPlayer->state.score;
                amount = (value >= 0xA6) ? 0xA6 : value;
                otherPlayer->state.score = value - amount;
                addRacePlayerScore(&gRacePlayers[arg0->task.userId], amount);
            }
            i++;
        } while (i < RACE_PLAYER_COUNT);

        flags = arg0->flags;
        amount = flags & 1;
        if (flags & 8) {
            enqueuePositionalSoundEffect(
                SFX_RAT_FACE_STEAL,
                &gRacePlayers[arg0->task.userId].state.renderPosition,
                0x7F,
                SFX_PRIORITY_NORMAL
            );
            arg0->flags &= ~8;
            amount = 1;
            amount = (flags = arg0->flags) & amount;
        }
        if (amount) {
            arg0->verticalAcceleration = 0;
            arg0->verticalVelocity = 0;
            enqueuePositionalSoundEffect(
                SFX_PROJECTILE_LAUNCH,
                &gRacePlayers[arg0->task.userId].state.renderPosition,
                0x7F,
                SFX_PRIORITY_NORMAL
            );
            setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiProjectileFlyAway);
        }
    }

    addRenderCallback(&gModelRenderCallbackQueue4Head, (RenderCallback)renderRaceUiProjectile, arg0);
}

void updateRaceUiProjectileBounceDown(RaceUiProjectileActor *arg0) {
    s16 sp2C[0x12];
    RacePlayer *player;
    RaceUiProjectileActor *actor;

    actor = arg0;
    if (gRaceUpdatePaused == 0) {
        player = &gRacePlayers[actor->task.userId];
        arg0 += 0;
        if (((!actor) && (!actor)) && (!actor)) {}
        player = &gRacePlayers[actor->task.userId];
        makeFixedRotationXY(sp2C, player->state.pitchAngle, player->state.facingAngle);
        transformVec3iByFixedMatrix(sp2C, &actor->velocity, &actor->pos);

        player = &gRacePlayers[actor->task.userId];
        actor->pos.x += player->state.renderPosition.x;
        actor->pos.y += player->state.renderPosition.y + actor->verticalVelocity;
        actor->pos.z += player->state.renderPosition.z;
        actor->verticalAcceleration += 0xFFFF0000;
        actor->verticalVelocity += actor->verticalAcceleration;
        if (actor->verticalVelocity <= 0) {
            actor->verticalVelocity = 0;
            enqueuePositionalSoundEffect(SFX_RAT_FACE_LAND, &player->state.renderPosition, 0x7F, SFX_PRIORITY_NORMAL);
            advanceRaceUiProjectileAnimation(actor);
            setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiProjectileStealScores);
        }
    }
    addRenderCallback(&gModelRenderCallbackQueue4Head, (RenderCallback)renderRaceUiProjectile, actor);
}

void updateRaceUiProjectileGrounded(RaceUiProjectileActor *arg0) {
    s16 sp2C[0x12];
    RacePlayer *player;
    RaceUiProjectileActor *actor;

    actor = arg0;
    if (gRaceUpdatePaused == 0) {
        makeFixedRotationXY(
            sp2C,
            gRacePlayers[actor->task.userId].state.pitchAngle,
            gRacePlayers[actor->task.userId].state.facingAngle
        );
        transformVec3iByFixedMatrix(sp2C, &actor->velocity, &actor->pos);

        player = &gRacePlayers[actor->task.userId];
        actor->pos.x += player->state.renderPosition.x;
        actor->pos.y += player->state.renderPosition.y;
        actor->pos.z += player->state.renderPosition.z;
        advanceRaceUiProjectileAnimation(actor);
        if (actor->flags != 0) {
            setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiProjectileBounceDown);
            actor->verticalAcceleration = 0x50000;
            actor->verticalVelocity += 0x50000;
        }
    }
    addRenderCallback(&gModelRenderCallbackQueue4Head, (RenderCallback)renderRaceUiProjectile, actor);
}

void updateRaceUiProjectileInitialFall(RaceUiProjectileActor *arg0) {
    s16 sp2C[0x12];
    RacePlayer *player;
    RaceUiProjectileActor *actor;

    actor = arg0;
    if (gRaceUpdatePaused == 0) {
        makeFixedRotationXY(
            sp2C,
            gRacePlayers[actor->task.userId].state.pitchAngle,
            gRacePlayers[actor->task.userId].state.facingAngle
        );
        transformVec3iByFixedMatrix(sp2C, &actor->velocity, &actor->pos);

        player = &gRacePlayers[actor->task.userId];
        actor->pos.x += player->state.renderPosition.x;
        actor->pos.y += player->state.renderPosition.y + actor->verticalVelocity;
        actor->pos.z += player->state.renderPosition.z;
        actor->verticalVelocity += actor->verticalAcceleration;
        actor->verticalAcceleration += 0xFFFF0000;
        if (actor->verticalVelocity <= 0) {
            actor->verticalVelocity = 0;
            setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceUiProjectileGrounded);
        }
    }
    addRenderCallback(&gModelRenderCallbackQueue4Head, (RenderCallback)renderRaceUiProjectile, actor);
}

void initRatFaceItemEffect(RaceUiProjectileActor *arg0) {
    arg0->velocity.x = 0;
    arg0->velocity.y = 0x280000;
    arg0->velocity.z = 0;
    arg0->unk3C = 0;
    arg0->unk4C = 0;
    arg0->verticalVelocity = 0x300000;
    arg0->verticalAcceleration = 0;
    setRaceUiProjectileAnimation(arg0, gRatFaceItemAnimation);
    enqueuePositionalSoundEffect(
        SFX_RAT_FACE_APPEAR,
        &gRacePlayers[arg0->task.userId].state.renderPosition,
        0x7F,
        SFX_PRIORITY_NORMAL
    );
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceUiProjectileInitialFall);
}

void drawTimeTrialRecordDeltaPopup(RaceUiTimeTrialRecordDeltaPopupActor *arg0) {
    if (gCurrentViewportIndex == 0) {
        if (arg0->isSlowerThanRecord != 0) {
            drawAssetTableSprite(
                (s16)(arg0->x - 0x66),
                arg0->y.half.lo,
                getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)),
                0x98
            );
            drawRaceUiPackedTime(&arg0->delta, arg0->x + 0x26, arg0->y.word, 0xC);
            return;
        }
        drawAssetTableSprite(
            (s16)(arg0->x - 0x62),
            arg0->y.half.lo,
            getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)),
            0x97
        );
        drawRaceUiPackedTime(&arg0->delta, arg0->x + 0x22, arg0->y.word, 0x10);
    }
}

void updateTimeTrialRecordDeltaPopupSlideOut(RaceUiTimeTrialRecordDeltaPopupActor *arg0) {
    arg0->x -= arg0->velocity;
    arg0->velocity += 4;
    if (arg0->velocity == 0x38) {
        removeCallbackTask(arg0);
    } else {
        addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawTimeTrialRecordDeltaPopup, arg0);
    }
}

void updateTimeTrialRecordDeltaPopupHold(RaceUiTimeTrialRecordDeltaPopupActor *arg0) {
    arg0->timer--;
    if (arg0->timer == 0) {
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateTimeTrialRecordDeltaPopupSlideOut);
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawTimeTrialRecordDeltaPopup, arg0);
}

void updateTimeTrialRecordDeltaPopupSlideIn(RaceUiTimeTrialRecordDeltaPopupActor *arg0) {
    arg0->x -= arg0->velocity;
    arg0->velocity -= 4;
    if (arg0->velocity == 0) {
        arg0->timer = TIME_TRIAL_RECORD_DELTA_POPUP_HOLD_TIMER;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateTimeTrialRecordDeltaPopupHold);
    }
    addRenderCallback(&gRaceOverlayRenderCallbackList, (RenderCallback)drawTimeTrialRecordDeltaPopup, arg0);
}

void initTimeTrialRecordDeltaPopup(RaceUiTimeTrialRecordDeltaPopupActor *popup) {
    popup->y.word = TIME_TRIAL_RECORD_DELTA_POPUP_START_Y;
    popup->x = TIME_TRIAL_RECORD_DELTA_POPUP_START_X;
    popup->velocity = TIME_TRIAL_RECORD_DELTA_POPUP_START_VELOCITY;
    popup->isSlowerThanRecord = calculateRaceTimerDelta(
        &gRaceElapsedTimer,
        &RACE_UI_TIME_TRIAL_SAVE_DATA->timeTrialRecordSplitTimes[gRaceCourseIndex.signedValue],
        &popup->delta
    );
    setCallbackTaskCallback(popup, (CallbackTaskCallback)updateTimeTrialRecordDeltaPopupSlideIn);
}

void renderRaceCourseCoinMarkers(RaceUiGfxCommandActor *arg0) {
    RaceUiGfxCommandScriptEntry *entry;
    s16 textureIndex;
    s32 i;
    register RaceUiGfxCommandActor *actor;
    u8 padding[1];
    void *image;
    void *palette;

    textureIndex = -1;
    actor = arg0;
    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);

    entry = gCourseCoinMarkerEntryLists[gRaceCourseIndex.signedValue];
    i = 0;
    if (entry->sentinel != -1) {
        for (;;) {
            if ((entry->active != 0) && (isPositionNearCurrentRaceViewportCamera(&entry->position) != 0)) {
                if (textureIndex != entry->sentinel + actor->textureOffset) {
                    textureIndex = entry->sentinel + actor->textureOffset;
                    getAssetTableImageAndPalette(
                        getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)),
                        0x14,
                        &image,
                        &palette
                    );

                    gDPLoadTextureBlock_4b(
                        gRegionAllocPtr++,
                        image,
                        G_IM_FMT_CI,
                        32,
                        32,
                        0,
                        G_TX_CLAMP,
                        G_TX_CLAMP,
                        0,
                        0,
                        0,
                        0
                    );
                    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
                }

                gSPMatrix(gRegionAllocPtr++, &actor->particles[i], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPVertex(gRegionAllocPtr++, gCourseCoinMarkerQuadVertices, 4, 0);
                gSP2Triangles(gRegionAllocPtr++, 3, 2, 1, 0, 3, 1, 0, 0);
            }
            entry++;
            i++;

            if (!(entry->sentinel != -1)) {
                break;
            }
        }
    }

    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
}

void updateRaceCourseCoinMarkers(RaceUiGfxCommandActor *arg0) {
    RaceUiGfxCommandScriptEntry *entry;
    Vec3i *pos;
    RaceUiGfxCommandActor *actor;
    register s32 xzSize;
    register s32 ySize;
    register s32 sentinel;

    entry = gCourseCoinMarkerEntryLists[gRaceCourseIndex.signedValue];
    actor = arg0;
    if (gFrameCounter & 1) {
        actor->textureOffset++;
    }
    if (actor->textureOffset >= 6) {
        actor->textureOffset = 0;
    }

    sentinel = -1;
    ySize = 0xC0000;
    if (entry->sentinel == sentinel) {
        goto done;
    }
    xzSize = 0x70000;
    pos = &entry->position;

loop:
    if (entry->active != 0) {
        if (isPositionNearAnyRaceViewportFocus(pos) != 0) {
            if (isRacePlayerInsideCylinder(pos, xzSize, ySize, 0) != 0) {
                entry->active = 0;
                enqueuePositionalSoundEffect(SFX_CONFIRM, pos, 0x7F, SFX_PRIORITY_NORMAL);
                gRacePlayers[0].state.collectedCourseCoinMarkerCount++;
                if (gRacePlayers[0].state.trailEffectTimer != 0) {
                    gRacePlayers[0].state.trailEffectTimer = 0xF0;
                } else {
                    startSpeedFanEffect(gRacePlayers);
                }
                if (gRacePlayers[0].state.trailEffectTimer) {}
            }
        }
    }

    entry++;
    pos = &entry->position;
    if (entry->sentinel != sentinel) {
        goto loop;
    }

done:
    addRenderCallback(&gEffectRenderCallbackList, (RenderCallback)renderRaceCourseCoinMarkers, actor);
}

void initRaceCourseCoinMarkerMatrices(RaceUiGfxCommandActor *arg0) {
    register RaceUiGfxCommandActor *actor1;
    register RaceUiGfxCommandActor *actor2;
    register RaceUiGfxCommandScriptEntry *script;
    register s32 i;
    register s32 offset;
    register s32 one;
    register Mtx *template;

    script = gCourseCoinMarkerEntryLists[gRaceCourseIndex.signedValue];
    actor1 = arg0;
    actor2 = arg0;
    i = 0;

    template = &gIdentityMatrix;
    offset = 0;
    one = 1;
    while (i < actor2->count) {
        script->active = one;
        actor1->particles[i] = *template;
        setPackedMatrixTranslation(&actor1->particles[i], &script->position);
        i++;
        offset += sizeof(Mtx);
        script++;
    }

    osWritebackDCache(actor1->particles, actor1->count * sizeof(Mtx));
}

void initRaceCourseCoinMarkers(RaceUiGfxCommandActor *actor) {
    RaceUiGfxCommandScriptEntry *marker;
    s32 markerCount;

    marker = gCourseCoinMarkerEntryLists[gRaceCourseIndex.signedValue];
    markerCount = 0;
    while (marker->sentinel != -1) {
        markerCount++;
        marker++;
    }
    gRacePlayers[0].state.totalCourseCoinMarkerCount = markerCount;
    actor->count = markerCount;
    if (markerCount != 0) {
        gAssetHandles[0x24] = allocRelocatableHeapBlock(markerCount * sizeof(Mtx));
        actor->particles = getRelocatableHeapBlockBase(gAssetHandles[0x24]);
        initRaceCourseCoinMarkerMatrices(actor);
        setCallbackTaskCallback(actor, (CallbackTaskCallback)updateRaceCourseCoinMarkers);
    }
}

void renderRaceStartOverlay(RaceUiOverlayActor *arg0) {
    volatile u8 pad[0xC];
    Transform3D transform;

    if (gRenderMatricesDirty != 0) {
        arg0->matrixDirty = 1;
    }
    if (arg0->matrixDirty != 0) {
        arg0->matrixDirty = 0;
        transform = gIdentityFixedTransform;
        transform.translation.x = arg0->pos.x;
        transform.translation.y = arg0->pos.y;
        transform.translation.z = arg0->pos.z;
        arg0->matrix = allocFixedTransformMatrix(&transform);
    }
    if (arg0->matrix != NULL) {
        gSPDisplayList(gRegionAllocPtr++, gAlphaSpriteRenderModeDl);
        gDPLoadTextureTile_4b(
            gRegionAllocPtr++,
            arg0->panelAImage,
            G_IM_FMT_CI,
            48,
            40,
            0,
            0,
            48,
            40,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            G_TX_NOMASK,
            G_TX_NOMASK,
            G_TX_NOLOD,
            G_TX_NOLOD
        );
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->panelAPalette);
        gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPVertex(gRegionAllocPtr++, gRaceStartOverlayVertices, 8, 0);
        gSP1Quadrangle(gRegionAllocPtr++, 3, 2, 1, 0, 0);
        gDPLoadTextureTile_4b(
            gRegionAllocPtr++,
            arg0->panelBImage,
            G_IM_FMT_CI,
            48,
            40,
            0,
            0,
            48,
            40,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            G_TX_NOMASK,
            G_TX_NOMASK,
            G_TX_NOLOD,
            G_TX_NOLOD
        );
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, arg0->panelBPalette);
        gSP1Quadrangle(gRegionAllocPtr++, 7, 6, 5, 4, 0);
    }
}

void updateRaceStartOverlayFallOut(RaceUiOverlayActor *arg0) {
    s16 timer;
    RaceUiOverlayActor *actor;

    timer = arg0->timer;
    actor = arg0;
    if (timer != 0) {
        arg0->pos.y += arg0->velocity;
        arg0->velocity += 0x2000;
        arg0->timer = timer - 1;
    } else {
        removeCallbackTask(actor);
        return;
    }
    addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)renderRaceStartOverlay, actor);
}

void updateRaceStartOverlayAnimate(RaceUiOverlayActor *arg0) {
    s16 temp_v0;
    s32 temp_v0_2;
    void *temp_v0_3;
    RaceUiOverlayActor *temp_s0;

    temp_v0 = arg0->timer;
    temp_s0 = arg0;
    if (temp_v0 == 0) {
        if (arg0->assetTimer == 0) {
            enqueuePositionalSoundEffect(SFX_SHOCK, &temp_s0->pos, 0x7F, SFX_PRIORITY_NORMAL);
        }
        temp_v0_3 = getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F));
        getAssetTableImageAndPalette(
            temp_v0_3,
            (temp_s0->assetTimer >> 1) + 0x3B,
            &temp_s0->panelBImage,
            &temp_s0->panelBPalette
        );
        temp_s0->assetTimer++;
        if (temp_s0->assetTimer >= 0xB) {
            temp_s0->timer = 0x3C;
            setCallbackTaskCallback(temp_s0, (CallbackTaskCallback)updateRaceStartOverlayFallOut);
        }
    } else {
        temp_s0->timer = temp_v0 - 1;
    }

    temp_v0_2 = temp_s0->velocity;
    temp_s0->pos.y += temp_v0_2;
    temp_s0->velocity = temp_v0_2 + 0xC00;
    addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)renderRaceStartOverlay, temp_s0);
}

void updateRaceStartOverlayRiseIn(RaceUiOverlayActor *arg0) {
    s16 timer = arg0->timer;

    if (timer != 0) {
        arg0->pos.y -= arg0->velocity;
        arg0->velocity -= 0x2000;
        arg0->timer = timer - 1;
    } else {
        arg0->timer = 0x14;
        arg0->assetTimer = 0;
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceStartOverlayAnimate);
    }

    addRenderCallback(&gRaceModelEffectRenderCallbackList, (RenderCallback)renderRaceStartOverlay, arg0);
}

void initRaceStartOverlay(RaceUiOverlayActor *arg0) {
    s32 i;

    arg0->pos.x = 0;
    arg0->pos.z = gRacePlayers[0].state.pos.z + 0xFF900000;
    arg0->velocity = 0;
    arg0->pos.y =
        getRaceCourseSurfaceHeight(findRaceCourseSurfaceFromHint(0, 0, arg0->pos.z), arg0->pos.x, arg0->pos.z) +
        0x200000;

    for (i = 0; i < 0x40; i++) {
        arg0->velocity += 0x2000;
        arg0->pos.y += arg0->velocity;
        arg0->timer++;
    }

    getAssetTableImageAndPalette(
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)),
        0x3A,
        &arg0->panelAImage,
        &arg0->panelAPalette
    );
    getAssetTableImageAndPalette(
        getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)),
        0x3B,
        &arg0->panelBImage,
        &arg0->panelBPalette
    );
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceStartOverlayRiseIn);
}

void renderRaceScoreAttackRings(RaceUiRankTextRenderActor *arg0) {
    RaceUiRankTextRenderEntry *var_s4;
    s32 var_s6;
    s32 var_fp;
    Gfx *temp_s0;
    Gfx *temp_s2;
    Gfx *temp_s3;

    var_s4 = gCourseScoreAttackRingEntryLists[gRaceCourseIndex.signedValue];
    var_fp = TRUE;
    var_s6 = 0;

    while (var_s4->active != -1) {
        if ((var_s4->active != 0) && (isPositionNearCurrentRaceViewportCamera(&var_s4->position) != 0)) {
            if (var_fp != 0) {
                gDPPipeSync(gRegionAllocPtr++);

                temp_s2 = gRegionAllocPtr++;
                var_fp = FALSE;
                gSPSegment(temp_s2, 0x02, getRelocatableHeapBlockBase(gAssetHandles[0xA]));

                temp_s3 = gRegionAllocPtr++;
                gSPSegment(temp_s3, 0x03, getRelocatableHeapBlockBase(gAssetHandles[0xB]));
            }

            temp_s0 = gRegionAllocPtr++;
            gSPMatrix(temp_s0, &arg0->matrices[var_s6], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            temp_s0 = gRegionAllocPtr++;
            gSPDisplayList(temp_s0, &_ADDR_20019C0_VRAM);
        }
        var_s4++;
        var_s6++;
    }
}

void updateRaceScoreAttackRings(void *arg0) {
    volatile u8 pad[8];
    RaceUiRankTextRenderEntry *entry;
    RaceScoreAttackRingTrigger *trigger;
    s32 radius;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 i;

    entry = gCourseScoreAttackRingEntryLists[gRaceCourseIndex.signedValue];
    if (entry->active != -1) {
        for (;;) {
            if (entry->active != 0) {
                pushRacePlayersOutOfCylinderAndApplyItemHit(&entry->position, 0xE0000, 0x100000, 2);
                trigger = gRacePlayers[0].state.scoreAttackRingTriggerList;
                if (trigger != NULL) {
                    for (;;) {
                        if (trigger->triggered == 0) {
                            radius = trigger->radius + 0xE0000;
                            dx = trigger->position.x - entry->position.x;
                            if ((dx < radius) && (-radius < dx)) {
                                dy = (trigger->position.y - entry->position.y) + 0xFFF20000;
                                if ((dy < radius) && (-radius < dy)) {
                                    dz = trigger->position.z - entry->position.z;
                                    if ((dz < radius) && (-radius < dz) &&
                                        (integerSquareRoot64((s64)dx * dx + (s64)dy * dy + (s64)dz * dz) < radius)) {
                                        trigger->triggered = 1;
                                        i = 0;
                                        for (;;) {
                                            spawnRaceUiAltBurstTextParticle(
                                                entry->position.x,
                                                entry->position.y + 0x70000,
                                                entry->position.z,
                                                i
                                            );
                                            i++;

                                            if (!(i != 8)) {
                                                break;
                                            }
                                        }
                                        entry->active = 0;
                                        gRacePlayers[0].state.scoreAttackPointTotal++;
                                        break;
                                    }
                                }
                            }
                        }
                        trigger = trigger->next;

                        if (!(trigger != NULL)) {
                            break;
                        }
                    }
                }
            }
            entry++;

            if (!(entry->active != -1)) {
                break;
            }
        }
    }

    addRenderCallback(&gSceneModelRenderCallbackList, (RenderCallback)renderRaceScoreAttackRings, arg0);
}

// clang-format off
void initRaceScoreAttackRingMatrices(RaceUiRankTextRenderActor *arg0) {
    Transform3D scratch;
    RaceUiRankTextRenderEntry *entry;
    RaceUiRankTextRenderActor *actor1;
    RaceUiRankTextRenderActor *actor2;
    s32 i;
    register s32 active;


    actor2 = arg0;
    actor1 = arg0;
    entry = gCourseScoreAttackRingEntryLists[gRaceCourseIndex.signedValue];
    i = 0;
    
        active = 1;
        while (i < actor2->count) {
            makeFixedRotationY(scratch.rotation, entry->angle);
            scratch.rotation[0] = scratch.rotation[0] / 12;
            scratch.rotation[1] = scratch.rotation[1] / 12;
            scratch.rotation[2] = scratch.rotation[2] / 12;
            if ((!i) && (!i)) {}
            scratch.rotation[3] = scratch.rotation[3] / 12;
            scratch.rotation[4] = scratch.rotation[4] / 12;
            scratch.rotation[5] = scratch.rotation[5] / 12;
            scratch.rotation[6] = scratch.rotation[6] / 12;
            scratch.rotation[7] = scratch.rotation[7] / 12;
            scratch.rotation[8] = scratch.rotation[8] / 12;
            scratch.translation.x = entry->position.x;
            scratch.translation.y = entry->position.y;
            scratch.translation.z = entry->position.z;
            packFixedTransformMatrix(&scratch, &actor1->matrices[i]);
            entry->active = active;
            i++;
            entry++;
        }
        
    osWritebackDCache(arg0->matrices, arg0->count << 6);
}
// clang-format on

void initRaceScoreAttackRings(RaceUiRankTextRenderActor *arg0) {
    RaceUiRankTextRenderEntry *var_v0;
    s32 var_v1;

    var_v0 = gCourseScoreAttackRingEntryLists[gRaceCourseIndex.signedValue];
    var_v1 = 0;
    while (var_v0->active != -1) {
        var_v1++;
        var_v0++;
    }
    gRacePlayers[0].state.scoreAttackPointTarget = var_v1;
    arg0->count = var_v1;
    if (var_v1 != 0) {
        gAssetHandles[0x26] = allocRelocatableHeapBlock(var_v1 << 6);
        arg0->matrices = (void *)getRelocatableHeapBlockBase(gAssetHandles[0x26]);
        initRaceScoreAttackRingMatrices(arg0);
        setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceScoreAttackRings);
    }
}
