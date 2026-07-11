#include "common.h"
#include "memory_allocator.h"
#include "callback_task_scheduler.h"
#include "main_menu_effects.h"
#include "main_menu_scene_model.h"
#define MENU_RENDERING_BROAD_PROTOTYPES
#include "menu_rendering.h"
#include "title_menu.h"

struct MainMenuEffectActor {
    char pad0[0x10];
    /* 0x10 */ s16 delay;
    /* 0x12 */ char pad12[6];
    union {
        struct {
            /* 0x18 */ s16 x;
            /* 0x1A */ s16 y;
            union {
                /* 0x1C */ s16 offsetX;
                /* 0x1C */ u16 animFrame;
            };
            union {
                /* 0x1E */ s16 offsetY;
                /* 0x1E */ u16 animTimer;
            };
            union {
                /* 0x20 */ s16 spriteState[2];
                struct {
                    /* 0x20 */ s16 angle;
                    /* 0x22 */ s16 angleVelocity;
                };
                struct {
                    /* 0x20 */ u16 effectFrame;
                    /* 0x22 */ u16 effectTimer;
                };
            };
        };
        /* 0x18 */ s16 linePositions[6];
    };
    union {
        /* 0x24 */ s16 frameIndex;
        /* 0x24 */ u16 uFrameIndex;
        struct {
            /* 0x24 */ u8 frameIndexHi;
            /* 0x25 */ u8 palette;
        };
    };
    union {
        /* 0x26 */ s16 introTimer;
        struct {
            /* 0x26 */ u8 characterId;
            /* 0x27 */ u8 pad27;
        };
    };
    /* 0x28 */ s16 startX;
    /* 0x2A */ s16 startY;
};

extern void func_800483FC(void *, void *, void *);
extern Gfx *gRegionAllocPtr;
extern s32 D_80124838;
extern s32 D_80124868;
extern Gfx D_800DEFF8[];
extern s16 gMenuFadeAlpha;
extern s16 D_80112172;
extern s16 D_8011217C;
extern s16 D_8011217E;
extern u16 D_8010B1A2;
extern u8 D_8010B1A5;
extern s16 D_8010B1A6;
extern u8 D_8010B1A8[];
extern u16 D_8010B1D0;
extern u16 D_800B9530[];
extern u16 D_800B9538[];
extern s32 D_8010ADDC;

void func_8003BF7C(MainMenuEffectActor *arg0);
void func_8003C180(MainMenuEffectActor *arg0);
void func_8003C484(MainMenuEffectActor *arg0);
void func_8003C634(MainMenuEffectActor *arg0);
void func_8003C7E4(MainMenuEffectActor *arg0);
void func_8003CE78(MainMenuEffectActor *arg0);
void func_8003D2F4(MainMenuEffectActor *arg0);
void func_8003D5F8(MainMenuEffectActor *arg0);
void func_8003D580(MainMenuEffectActor *arg0);
void func_8003D4C0(MainMenuEffectActor *arg0);
void func_8003D3C0(MainMenuEffectActor *arg0);
void func_8003D7C0(MainMenuEffectActor *arg0);
void func_8003D88C(MainMenuEffectActor *arg0);
void func_8003DA98(MainMenuEffectActor *arg0);
void func_8003DB84(MainMenuEffectActor *arg0);
void func_8003DCCC(MainMenuEffectActor *arg0);
void func_8003DBE8(MainMenuEffectActor *arg0);
void func_8003C2EC(MainMenuEffectActor *arg0);
void func_8003C93C(MainMenuEffectActor *arg0);
void func_8003CAD8(MainMenuEffectActor *arg0);
void func_8003CC58(MainMenuEffectActor *arg0);
void func_8003CF98(MainMenuEffectActor *arg0);
void func_8003D124(MainMenuEffectActor *arg0);

void func_8003BEF0(MainMenuEffectActor *arg0) {
    func_8000F030(arg0->x + arg0->offsetX, arg0->y + arg0->offsetY, func_80043040(D_80112172), ((u16) arg0->angle + 0x32) & 0xFFFF, 0x20, 0x20, arg0->palette, 0);
}

void func_8003BF7C(MainMenuEffectActor *arg0) {
    MainMenuSceneModel *model;
    s32 divisor;

    model = func_80041E60(arg0->characterId); divisor = 0x24000;
    if (0) {}
    arg0->x = model->displayObjects[7].screenX / divisor;
    arg0->y = -(model->displayObjects[7].screenY / divisor);
    if ((arg0->effectTimer += 1) == 3) {
        arg0->effectTimer = 0;
        arg0->effectFrame++;
    }
    if (arg0->effectFrame == 3) {
        removeCallbackTask(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8003BEF0, arg0);
}

void func_8003C078(MainMenuEffectActor *arg0) {
    arg0->angleVelocity = 0;
    arg0->angle = 0;
    setCallbackTaskCallback(arg0, func_8003BF7C);
}

void func_8003C0A4(s16 arg0, s16 arg1, u8 arg2, u8 arg3) {
    MainMenuEffectActor *temp_v0 = (MainMenuEffectActor *)(D_8010ADDC = (s32)createCallbackTask(func_8003C078, 0, 0x63));

    temp_v0->offsetX = arg0;
    temp_v0->offsetY = arg1;
    temp_v0->characterId = arg2;
    temp_v0->palette = arg3;
}

void func_8003C118(MainMenuEffectActor *arg0) {
    s32 temp = func_80043040(D_80112172);
    MainMenuEffectActor *linePosition =
        (MainMenuEffectActor *)((s32)arg0 + arg0->uFrameIndex * sizeof(arg0->linePositions[0]));

    func_8000F030(linePosition->x, linePosition->offsetY, temp, 0xF, 0x20, 0x20, 0, 0);
}

void func_8003C180(MainMenuEffectActor *arg0) {
    arg0->introTimer = (u16) arg0->introTimer + 1;
    if ((((u16) arg0->introTimer) % 30) == 0) {
        if (arg0->uFrameIndex < 2) {
            arg0->uFrameIndex++;
        }
    }
    if (((u16) arg0->introTimer) == 0x69) {
        removeCallbackTask(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8003C118, arg0);
}

void func_8003C208(MainMenuEffectActor *arg0) {
    arg0->linePositions[2] = -0x38;
    arg0->linePositions[5] = -0x5A;
    arg0->linePositions[0] = -0x30;
    arg0->linePositions[3] = -0x5D;
    arg0->linePositions[1] = -0x28;
    arg0->linePositions[4] = -0x56;
    arg0->introTimer = 0;
    arg0->frameIndex = 0;
    setCallbackTaskCallback(arg0, func_8003C180);
}

void func_8003C264(MainMenuEffectActor *arg0) {
    func_8000F030(arg0->x + arg0->offsetX, arg0->y + arg0->offsetY, func_80043040(D_80112172), ((u16) arg0->angle + 0x10) & 0xFFFF, 0x20, 0x20, 0, 0);
}

void func_8003C2EC(MainMenuEffectActor *arg0) {
    MainMenuSceneModel *model;
    s32 divisor;

    model = func_80041E60(arg0->characterId); divisor = 0x24000;
    if (0) {}
    arg0->x = model->displayObjects[7].screenX / divisor;
    arg0->y = -(model->displayObjects[7].screenY / divisor);
    if ((arg0->effectTimer += 1) == 8) {
        arg0->effectTimer = 0;
        arg0->effectFrame++;
        arg0->effectFrame &= 3;
    }
    if (D_8010B1A2 == 0x2F) {
        removeCallbackTask(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8003C264, arg0);
}

void func_8003C3F4(MainMenuEffectActor *arg0) {
    arg0->angleVelocity = 0;
    arg0->angle = 0;
    setCallbackTaskCallback(arg0, func_8003C2EC);
}

void func_8003C420(s16 arg0, s16 arg1, u8 arg2) {
    MainMenuEffectActor *temp_v0 = (MainMenuEffectActor *)(D_8010ADDC = (s32)createCallbackTask(func_8003C3F4, 0, 0x63));

    temp_v0->offsetX = arg0;
    temp_v0->offsetY = arg1;
    temp_v0->characterId = arg2;
}

// func_8003C484 best match: 99.074%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_effects/func_8003C484.s")

#ifdef NON_MATCHING
void func_8003C484(MainMenuEffectActor *arg0) {
    Gfx *gfx;
    Gfx *new_var;
    s32 w0;
    s32 w1;

    /* IDO scheduling for this function depends on this block staying on one line. */
    do { gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; gfx->words.w0 = 0xE7000000; gfx->words.w1 = 0; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0xFF2FFFFF; w0 = 0xFC119623; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = 0x00504240; do { w0 = 0xB900031D; gfx->words.w0 = w0; gfx->words.w1 = w1; gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; new_var = gfx; new_var->words.w0 = 0xFA000000; new_var->words.w1 = -0x60; } while (0); if (arg0->effectFrame != 0) { func_8000F030(arg0->offsetX, arg0->offsetY, func_80043040(D_80112172), (((0, arg0->effectFrame)) + 0x24) & 0xFFFF, 0x30, 0x20, 0, 0); } gfx = gRegionAllocPtr; gRegionAllocPtr = gfx + 1; w1 = (s32) D_800DEFF8; w0 = 0x06000000; gfx->words.w0 = w0; gfx->words.w1 = w1; if (arg0->effectFrame < 0xC) { func_8000F030(arg0->offsetX, arg0->offsetY, func_80043040(D_80112172), (arg0->effectFrame + 0x25) & 0xFFFF, 0x30, 0x20, 0, 0); } func_8000F030(arg0->x, arg0->y, func_80043040(D_80112172), 0x31, 0x20, 0x20, 0, 0); } while (0);
}
#endif

void func_8003C634(MainMenuEffectActor *arg0) {
    arg0->effectTimer++;
    if (arg0->effectTimer == 3) {
        arg0->effectTimer = 0;
        arg0->effectFrame++;
        switch (arg0->characterId) {
        case 0:
            if (arg0->effectFrame == 4) {
                arg0->characterId = arg0->characterId + 1;
            }
            break;
        case 1:
            if (arg0->effectFrame == 8) {
                arg0->uFrameIndex++;
                if (arg0->uFrameIndex == 3) {
                    arg0->characterId++;
                } else {
                    arg0->effectFrame = 4;
                }
            }
            break;
        case 2:
            break;
        }
    }

    if (arg0->effectFrame == 0xD) {
        removeCallbackTask(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8003C484, arg0);
}

void func_8003C728(MainMenuEffectActor *arg0) {
    arg0->x = -0x3D;
    arg0->y = -0x63;
    arg0->offsetX = -0x4E;
    arg0->offsetY = -0x6C;
    arg0->angleVelocity = 0;
    arg0->frameIndex = 0;
    arg0->angle = 0;
    arg0->characterId = 0;
    setCallbackTaskCallback(arg0, func_8003C634);
}

void func_8003C77C(MainMenuEffectActor *arg0) {
    s32 temp;

    temp = func_80043040(D_80112172);
    func_8000F030(arg0->x, arg0->y, temp, (arg0->animFrame + 9) & 0xFFFF, 0x20, 0x20, 0, 0);
}

void func_8003C7E4(MainMenuEffectActor *arg0) {
    MainMenuEffectActor *temp_a2 = arg0;

    arg0->x++;
    arg0->y--;
    arg0->animTimer++;
    if (arg0->animTimer == 3) {
        arg0->animTimer = 0;
        arg0->animFrame++;
    }
    if (temp_a2->animFrame == 6) {
        removeCallbackTask(temp_a2);
    } else {
        func_800483FC(&D_80124868, func_8003C77C, temp_a2);
    }
}

void func_8003C870(MainMenuEffectActor *arg0) {
    arg0->x = 0x3B;
    arg0->y = -0x38;
    arg0->animTimer = 0;
    arg0->animFrame = 0;
    setCallbackTaskCallback(arg0, func_8003C7E4);
}

void func_8003C8AC(MainMenuEffectActor *arg0) {
    s32 temp = func_80043040(D_80112172);
    func_8000F030(arg0->x - 0xE, arg0->y - 0x50, temp, (u16)(D_800B9530[arg0->animFrame] + 6), 0x20, 0x20, 0, 0);
}

void func_8003C93C(MainMenuEffectActor *arg0) {
    MainMenuSceneModel *model;
    s32 divisor;

    model = func_80041E60(1); divisor = 0x24000;
    if (0) {}
    arg0->x = model->displayObjects[7].screenX / divisor;
    arg0->y = -(model->displayObjects[7].screenY / divisor);
    if ((arg0->animTimer += 1) == 3) {
        arg0->animTimer = 0;
        arg0->animFrame++;
        arg0->animFrame &= 3;
    }
    if (D_8010B1A2 == 0x12) {
        removeCallbackTask(arg0);
        return;
    }
    func_800483FC(&D_80124868, func_8003C8AC, arg0);
}

void func_8003CA44(MainMenuEffectActor *arg0) {
    arg0->animTimer = 0;
    arg0->animFrame = 0;
    setCallbackTaskCallback(arg0, func_8003C93C);
}

void func_8003CA70(MainMenuEffectActor *arg0) {
    s32 temp;

    temp = func_80043040(D_80112172);
    func_8000F030(arg0->x, arg0->y, temp, (arg0->animFrame + 0x14) & 0xFFFF, 0x20, 0x20, 0, 0);
}

void func_8003CAD8(MainMenuEffectActor *arg0) {
    MainMenuEffectActor *temp_a2 = arg0;

    arg0->animTimer++;
    if (arg0->animTimer == 2) {
        arg0->animTimer = 0;
        arg0->animFrame++;
    }
    if (temp_a2->animFrame == 5) {
        removeCallbackTask(temp_a2);
    } else {
        func_800483FC(&D_80124868, func_8003CA70, temp_a2);
    }
}

void func_8003CB4C(MainMenuEffectActor *arg0) {
    arg0->animTimer = 0;
    arg0->animFrame = 0;
    setCallbackTaskCallback(arg0, func_8003CAD8);
}

void func_8003CB78(s16 arg0, s16 arg1) {
    MainMenuEffectActor *temp_v0 = (MainMenuEffectActor *)(D_8010ADDC = (s32)createCallbackTask(func_8003CB4C, 0, 0x64));

    temp_v0->x = arg0;
    temp_v0->y = arg1;
}

void func_8003CBCC(MainMenuEffectActor *arg0) {
    func_8000F030(arg0->x + arg0->offsetX, arg0->y + arg0->offsetY, func_80043040(D_80112172), ((u16) arg0->angle + 0x20) & 0xFFFF, 0x20, 0x20, arg0->palette, 0);
}

void func_8003CC58(MainMenuEffectActor *arg0) {
    MainMenuSceneModel *model;
    s32 divisor;

    model = func_80041E60(arg0->characterId); divisor = 0x24000;
    if (0) {}
    arg0->x = model->displayObjects[7].screenX / divisor;
    arg0->y = -(model->displayObjects[7].screenY / divisor);
    if ((arg0->effectTimer += 1) == 4) {
        arg0->effectTimer = 0;
        if ((arg0->effectFrame += 1) == 5) {
            arg0->effectFrame = 0;
        }
    }
    if (D_8010B1A8[arg0->characterId] == 0) {
        func_800483FC(&D_80124868, func_8003CBCC, arg0);
        return;
    }
    removeCallbackTask(arg0);
}

void func_8003CD70(MainMenuEffectActor *arg0) {
    arg0->angleVelocity = 0;
    arg0->angle = 0;
    setCallbackTaskCallback(arg0, func_8003CC58);
}

void func_8003CD9C(s16 arg0, s16 arg1, u8 arg2, u8 arg3) {
    MainMenuEffectActor *temp_v0 = (MainMenuEffectActor *)(D_8010ADDC = (s32)createCallbackTask(func_8003CD70, 0, 0x64));

    temp_v0->offsetX = arg0;
    temp_v0->offsetY = arg1;
    temp_v0->characterId = arg2;
    temp_v0->palette = arg3;
}

void func_8003CE10(MainMenuEffectActor *arg0) {
    s32 temp;

    temp = func_80043040(D_80112172);
    func_8000F030(arg0->x, arg0->y, temp, (arg0->animFrame + 0x20) & 0xFFFF, 0x20, 0x20, 0, 0);
}

void func_8003CE78(MainMenuEffectActor *arg0) {
    MainMenuEffectActor *temp_a2 = arg0;

    arg0->animTimer++;
    if (arg0->animTimer == 4) {
        arg0->animTimer = 0;
        arg0->animFrame++;
        if (arg0->animFrame == 5) {
            arg0->animFrame = 0;
        }
    }
    if (D_8010B1A2 != 7) {
        func_800483FC(&D_80124868, func_8003CE10, temp_a2);
    } else {
        removeCallbackTask(temp_a2);
    }
}

void func_8003CF00(MainMenuEffectActor *arg0) {
    arg0->x = -0x14;
    arg0->y = -0x59;
    arg0->animTimer = 0;
    arg0->animFrame = 0;
    setCallbackTaskCallback(arg0, func_8003CE78);
}

void func_8003CF3C(MainMenuEffectActor *arg0) {
    s32 temp = func_80043040(D_80112172);
    func_8000F030(arg0->x, arg0->y, temp, arg0->animFrame, 0x20, 0x20, 0, 0);
}

void func_8003CF98(MainMenuEffectActor *arg0) {
    MainMenuEffectActor *temp_a2 = arg0;

    if (arg0->animFrame < 5) {
        arg0->animTimer++;
        if (arg0->animTimer == 1) {
            arg0->animTimer = 0;
            arg0->animFrame++;
        }
    } else {
        arg0->animTimer++;
    }
    if (arg0->animTimer < 0xF) {
        func_800483FC(&D_80124868, func_8003CF3C, temp_a2);
        return;
    }
    removeCallbackTask(temp_a2);
    D_8010B1A2 = 3;
}

void func_8003D03C(MainMenuEffectActor *arg0) {
    arg0->animFrame = 0;
    arg0->animTimer = 0;
    setCallbackTaskCallback(arg0, func_8003CF98);
}

void func_8003D068(s16 arg0, s16 arg1) {
    MainMenuEffectActor *temp = (MainMenuEffectActor *)(D_8010ADDC = (s32)createCallbackTask(func_8003D03C, 0, 0x64));

    temp->x = arg0;
    temp->y = arg1;
}

void func_8003D0BC(MainMenuEffectActor *arg0) {
    s32 temp;

    temp = func_80043040(D_80112172);
    func_8000F030(arg0->x, arg0->y, temp, (arg0->animFrame + 0x1C) & 0xFFFF, 0x20, 0x20, 0, 0);
}

// func_8003D124 best match: 97.653%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_effects/func_8003D124.s")

#ifdef NON_MATCHING
void func_8003D124(MainMenuEffectActor *arg0) {
    s32 var_v1;
    MainMenuEffectActor *temp_a2 = arg0;

    var_v1 = 0xB4;
    if ((u16) arg0->delay == 0) {
        var_v1 = 0x12;
    }
    if (arg0->animFrame < 3) {
        arg0->animTimer++;
        if (arg0->animTimer == 2) {
            arg0->animTimer = 0;
            arg0->animFrame++;
        }
    } else {
        arg0->animTimer++;
    }
    if (arg0->animTimer < var_v1) {
        func_800483FC(&D_80124868, func_8003D0BC, temp_a2);
        return;
    }
    if (D_8010B1A2 == 3) {
        D_8010B1A2 = 4;
    }
    removeCallbackTask(temp_a2);
}
#endif

void func_8003D1EC(MainMenuEffectActor *arg0) {
    arg0->animFrame = 0;
    arg0->animTimer = 0;
    setCallbackTaskCallback(arg0, func_8003D124);
}

void func_8003D218(s16 arg0, s16 arg1, u8 arg2) {
    MainMenuEffectActor *temp_v0 = (MainMenuEffectActor *)(D_8010ADDC = (s32)createCallbackTask(func_8003D1EC, 0, 0x64));

    temp_v0->x = arg0;
    temp_v0->y = arg1;
    temp_v0->delay = arg2;
}

void func_8003D27C(MainMenuEffectActor *arg0) {
    s32 temp;

    temp = func_80043040(D_80112172);
    func_8000F030(arg0->x, arg0->y, temp, (D_800B9538[arg0->animFrame] + 0x19) & 0xFFFF, 0x20, 0x20, 0, 0);
}

void func_8003D2F4(MainMenuEffectActor *arg0) {
    MainMenuEffectActor *temp_a2 = arg0;

    arg0->animTimer++;
    if (arg0->animTimer == 7) {
        arg0->animTimer = 0;
        arg0->animFrame = (arg0->animFrame + 1) & 3;
    }
    if (temp_a2->x < 0x12) {
        temp_a2->x++;
    }
    if (D_8010B1A2 == 4) {
        removeCallbackTask(temp_a2);
    } else {
        func_800483FC(&D_80124868, func_8003D27C, temp_a2);
    }
}

void func_8003D384(MainMenuEffectActor *arg0) {
    arg0->x = -0xA7;
    arg0->y = -0x61;
    arg0->animTimer = 0;
    arg0->animFrame = 0;
    setCallbackTaskCallback(arg0, func_8003D2F4);
}

void func_8003D3C0(MainMenuEffectActor *arg0) {
    func_80011D74(&arg0->angle, 1, arg0->x, arg0->y);
}

void func_8003D3F4(MainMenuEffectActor *arg0) {
    func_800483FC(&D_80124838, func_8003D3C0, arg0);
}

void func_8003D424(MainMenuEffectActor *arg0) {
    s16 *temp_v0;
    s16 temp_v1;
    MainMenuEffectActor *temp_a2 = arg0;

    temp_v0 = &arg0->angle;
    if (*temp_v0 > 0) {
        *temp_v0 -= D_8010B1A5;
        D_8010B1A6 = *temp_v0;
        temp_v1 = *temp_v0;
        if (temp_v1 <= 0) {
            *temp_v0 = 0x140 - temp_v1;
        }
    }
    func_800483FC(&D_80124838, func_8003D3C0, temp_a2);
    if (D_8010B1A2 == 0x3A) {
        setCallbackTaskCallback(temp_a2, func_8003D3F4);
    }
}

void func_8003D4C0(MainMenuEffectActor *arg0) {
    s16 *temp_v0;
    MainMenuEffectActor *temp_a2 = arg0;

    temp_v0 = &arg0->angle;
    if (*temp_v0 > 0) {
        *temp_v0 -= D_8010B1A5;
        D_8010B1A6 = *temp_v0;
        if (*temp_v0 <= 0) {
            *temp_v0 = 0x140 - *temp_v0;
        }
    }
    func_800483FC(&D_80124838, func_8003D3C0, temp_a2);
    if (D_8010B1A2 == 0x39) {
        temp_v0 = &temp_a2->angle;
        setCallbackTaskCallback(temp_a2, func_8003D424);
        temp_a2->x = 0x28;
        temp_v0[1] = 0x10;
    }
}

void func_8003D580(MainMenuEffectActor *arg0) {
    s16 *temp_v0;

    func_800483FC(&D_80124838, func_8003D3C0, arg0);
    temp_v0 = &arg0->angle;
    if (D_8010B1A2 == 0x33) {
        setCallbackTaskCallback(arg0, func_8003D4C0);
        D_8010B1A5 = 8;
        if (*temp_v0 == 0) {
            *temp_v0 = 0x140;
        }
    }
}

void func_8003D5F8(MainMenuEffectActor *arg0) {
    s16 *temp_v0;
    MainMenuEffectActor *temp_a2 = arg0;

    temp_v0 = &arg0->angle;
    if (*temp_v0 > 0) {
        *temp_v0 -= 8;
        D_8010B1A6 = *temp_v0;
        if (*temp_v0 == 0) {
            *temp_v0 = 0x140;
        }
    }
    func_800483FC(&D_80124838, func_8003D3C0, temp_a2);
    if (D_8010B1A2 == 0x2A) {
        setCallbackTaskCallback(temp_a2, func_8003D580);
    }
}

void func_8003D68C(MainMenuEffectActor *arg0) {
    s16 *sp1C;
    MainMenuEffectActor *temp_a2 = arg0;

    sp1C = &arg0->angle;
    if (*sp1C > 0) {
        *sp1C -= 8;
        D_8010B1A6 = *sp1C;
    }
    func_800483FC(&D_80124838, func_8003D3C0, temp_a2);
    if (*sp1C == 0) {
        temp_a2->x = 0x14;
        *sp1C = 0x140;
        setCallbackTaskCallback(temp_a2, func_8003D5F8);
    }
}

void func_8003D720(MainMenuEffectActor *arg0) {
    s16 *temp_v0;
    u16 *temp_v1;
    MainMenuEffectActor *temp_a2 = arg0;

    temp_v0 = &arg0->angle;
    if (*temp_v0 < 0x140) {
        *temp_v0 += D_8010B1A5;
        D_8010B1A6 = *temp_v0;
        temp_v1 = &D_8010B1D0;
        *temp_v1 = *temp_v1 + 1;
    }
    func_800483FC(&D_80124838, func_8003D3C0, temp_a2);
    if (D_8010B1A2 == 0x20) {
        setCallbackTaskCallback(temp_a2, func_8003D68C);
    }
}

void func_8003D7C0(MainMenuEffectActor *arg0) {
    s16 *sp1C;
    MainMenuEffectActor *temp_a2 = arg0;

    sp1C = &arg0->angle;
    *sp1C += D_8010B1A5;
    if (*sp1C >= 0x140) {
        *sp1C -= 0x140;
    }
    D_8010B1A6 = *sp1C;
    func_800483FC(&D_80124838, func_8003D3C0, temp_a2);
    if (D_8010B1A2 == 0x13) {
        setCallbackTaskCallback(temp_a2, func_8003D720);
        temp_a2->x = 0x28;
        *sp1C = 0;
        D_8010B1A6 = *sp1C;
    }
}

void func_8003D88C(MainMenuEffectActor *arg0) {
    s16 *temp_v0;

    temp_v0 = &arg0->angle;
    func_800483FC(&D_80124838, func_8003D3C0, arg0);
    if (D_8010B1A2 == 0x10) {
        setCallbackTaskCallback(arg0, func_8003D7C0);
        arg0->x = 0x14;
        temp_v0[0] = 0;
        temp_v0[1] = 0x100;
        D_8010B1A5 = 0x14;
    }
}

void func_8003D908(MainMenuEffectActor *arg0) {
    func_80017168((DstStruct_80017168 *)arg0->spriteState, func_80043040(D_8011217C));
    arg0->angleVelocity = 0x10;
    arg0->x = arg0->startX;
    arg0->y = arg0->startY;
    D_8010B1D0 = 0;
    setCallbackTaskCallback(arg0, func_8003D88C);
}

void func_8003D974(MainMenuEffectActor *arg0) {
    func_80011D74(&arg0->angle, 1, arg0->x, arg0->y);
}

void func_8003D9A8(MainMenuEffectActor *arg0) {
    arg0->angle--;
    if (arg0->angle < 0) {
        arg0->angle = 0x140;
    }
    func_800483FC(&D_80124838, func_8003D974, arg0);
    if ((D_8010B1A2 == 0x2A) || (D_8010B1A2 == 0x3A)) {
        setCallbackTaskCallback(arg0, func_8003DA98);
    }
}

void func_8003DA24(MainMenuEffectActor *arg0) {
    arg0->angle++;
    if (arg0->angle == 0x140) {
        arg0->angle = 0;
    }
    func_800483FC(&D_80124838, func_8003D974, arg0);
    if (D_8010B1A2 == 0x14) {
        setCallbackTaskCallback(arg0, func_8003DA98);
    }
}

void func_8003DA98(MainMenuEffectActor *arg0) {
    func_800483FC(&D_80124838, func_8003D974, arg0);
    if (D_8010B1A2 == 0x10) {
        setCallbackTaskCallback(arg0, func_8003DA24);
    }
    if ((D_8010B1A2 == 0x20) || (D_8010B1A2 == 0x33)) {
        setCallbackTaskCallback(arg0, func_8003D9A8);
    }
}

void func_8003DB1C(MainMenuEffectActor *arg0) {
    func_80017168((DstStruct_80017168 *)arg0->spriteState, func_80043040(D_8011217C));
    arg0->angle = 0;
    arg0->angleVelocity = 0x1F0;
    arg0->x = 0x14;
    arg0->y = arg0->startY;
    setCallbackTaskCallback(arg0, func_8003DA98);
}

void func_8003DB84(MainMenuEffectActor *arg0) {
    func_80011D74(&arg0->angle, 0, arg0->x, arg0->y);
}

void func_8003DBB8(MainMenuEffectActor *arg0) {
    func_800483FC(&D_80124868, func_8003DB84, arg0);
}

void func_8003DBE8(MainMenuEffectActor *arg0) {
    s16 *temp_v0;

    arg0->animFrame = arg0->animFrame + 1;
    temp_v0 = &arg0->angle;
    arg0->animFrame &= 1;
    if (arg0->animFrame == 0) {
        temp_v0[1] -= 1;
        if (temp_v0[1] == 0x10) {
            setCallbackTaskCallback(arg0, func_8003DBB8);
            D_8010B1A2 = 0x43;
        }
    }
    func_800483FC(&D_80124868, func_8003DB84, arg0);
}

void func_8003DC70(MainMenuEffectActor *arg0) {
    func_800483FC(&D_80124868, func_8003DB84, arg0);
    if (D_8010B1A2 == 0x42) {
        arg0->animFrame = 0;
        setCallbackTaskCallback(arg0, func_8003DBE8);
    }
}

void func_8003DCCC(MainMenuEffectActor *arg0) {
    s16 *temp_v0;

    if (gMenuFadeAlpha == 0) {
        arg0->animFrame = (u16)arg0->animFrame + 1;
        temp_v0 = &arg0->angle;
        if ((u16)arg0->animFrame >= 0x78) {
            if (!(((u16)arg0->animFrame) & 1)) {
                temp_v0[1]++;
                if (temp_v0[1] == 0x60) {
                    arg0->animFrame = 0;
                    setCallbackTaskCallback(arg0, func_8003DC70);
                }
            }
        }
    }
    func_800483FC(&D_80124868, func_8003DB84, arg0);
}

void func_8003DD64(MainMenuEffectActor *arg0) {
    func_80017168((DstStruct_80017168 *)arg0->spriteState, func_80043040(D_8011217E));
    arg0->angle = 0;
    arg0->angleVelocity = 0x10;
    arg0->x = arg0->startX;
    arg0->y = arg0->startY;
    setCallbackTaskCallback(arg0, func_8003DCCC);
}
