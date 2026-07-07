#include "common.h"

typedef struct MenuRenderTask MenuRenderTask;
typedef struct RenderCallbackNode RenderCallbackNode;
typedef struct MenuRenderSprite MenuRenderSprite;
typedef struct MenuRenderSpriteActor MenuRenderSpriteActor;

struct MenuRenderTask {
    /* 0x00 */ MenuRenderTask *prev;
    /* 0x04 */ MenuRenderTask *next;
    /* 0x08 */ void (*callback)(MenuRenderTask *);
    /* 0x0C */ u16 type;
    /* 0x0E */ u16 priority;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 callbackTimer;
    /* 0x16 */ s16 isActive;
};

struct MenuRenderSprite {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 tileSize;
    /* 0x06 */ s16 tileXStep;
    /* 0x08 */ s16 tileYStep;
    /* 0x0A */ s16 padA;
    /* 0x0C */ s16 clipX;
    /* 0x0E */ s16 clipY;
    /* 0x10 */ s16 width;
    /* 0x12 */ s16 height;
    /* 0x14 */ u16 *image;
    /* 0x18 */ u16 *tilemap;
    /* 0x1C */ u8 *tileInfo;
    /* 0x20 */ u16 *palette;
};

struct MenuRenderSpriteActor {
    /* 0x00 */ MenuRenderTask task;
    /* 0x18 */ MenuRenderSprite sprite;
};

typedef void (*MenuRenderSpriteActorCallback)(MenuRenderSpriteActor *);
typedef void (*MenuRenderCallback)(MenuRenderSprite *);

extern void func_800483FC(RenderCallbackNode **queue, MenuRenderCallback callback, MenuRenderSprite *sprite);
extern void func_80071824(void *task, void (*callback)());
s32 func_80011D74(MenuRenderSprite *sprite, s32 arg1, s16 x, s16 y);
void func_800112F4(s16 arg0, s16 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, s32 arg6, s32 arg7);
extern void func_800137C8(s16 x, s16 y, u16 tileX, s32 tileY, u16 palette, u16 scale);
extern RenderCallbackNode *D_80124868;
extern u32 D_80123758;
extern s16 D_800DEF14;
extern s16 D_8015660A;
extern s16 D_8015660C;

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_8000EA80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_8000F030.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_8000F0EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_8000F8AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_8000F970.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_80010074.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_8001061C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_80010BCC.s")

void func_80011264(s16 arg0, s16 arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5) {
    func_800112F4(arg0, arg1, arg2, arg3, arg4, arg5, D_8015660A / 2, D_8015660C / 2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_800112F4.s")

void func_80011854(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_8001185C.s")

void func_80011C3C(MenuRenderSpriteActor *actor);
void func_80011D44(MenuRenderSprite *sprite);

void func_80011C18(MenuRenderSpriteActor *arg0) {
    func_80071824(arg0, func_80011C3C);
}

void func_80011C3C(MenuRenderSpriteActor *actor) {
    if (D_80123758 & 8) {
        actor->sprite.y++;
    }
    if (D_80123758 & 4) {
        actor->sprite.y--;
    }
    if (D_80123758 & 1) {
        actor->sprite.x++;
    }
    if (D_80123758 & 2) {
        actor->sprite.x--;
    }

    if (actor->sprite.x >= 0x141) {
        actor->sprite.x = 0;
    }
    if (actor->sprite.x < 0) {
        actor->sprite.x = 0x13F;
    }
    if (actor->sprite.y >= 0x9C1) {
        actor->sprite.y = 0;
    }
    if (actor->sprite.y < 0) {
        actor->sprite.y = 0x9BF;
    }

    func_800483FC(&D_80124868, func_80011D44, &actor->sprite);
}

void func_80011D44(MenuRenderSprite *arg0) {
    func_80011D74(arg0, 0, 0, 0);
}

void func_80011D6C(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_80011D74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_800128C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_800129DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_80012AE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_8001303C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_80013154.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_80013284.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_800137C8.s")

void func_80013D0C(s16 arg0, s16 arg1, u8 *arg2, u16 arg3, u16 arg4) {
    s32 var_s0;
    u8 *var_s1;
    char mask;
    s32 var_s2;
    s32 temp_s3;
    s32 temp_t9;
    long var_v0;
    s32 var_a2;
    s32 newline;
    s32 var_v1;

    mask = 0xFFFFFFFFFFFFFFFFu;
    temp_t9 = *arg2 & mask;
    var_s0 = arg0;
    var_s1 = arg2;
    var_s2 = arg1;
    temp_s3 = var_s0;
    if ((0, temp_t9) != 0) {
        var_v0 = temp_t9 & 0xFF;
        newline = 0xA;
        do {
            var_v1 = var_v0;
            if (newline == var_v0) {
                var_s0 = temp_s3;
                var_s2 += 8;
            } else {
                var_v0 = var_v1 - 0x20;
                if ((u32)var_v0 < 0x40) {
                    var_a2 = var_v0 & 7;
                    func_800137C8(var_s0, var_s2, ((var_a2 << 3) & 0xFFFF) & 0xFFFF, var_v0 & 0x38, arg3,
                                  arg4);
                }
                var_s0 += 8;
            }
            var_v0 = var_s1[1];
            var_s1++;
        } while (var_v0 != 0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_80013DFC.s")

s32 func_80013F88(value, step, increase)
s32 value;
s16 step;
u8 increase;
{
    if (increase) {
    } else {
        step = step * -1;
    }

    value += step;
    if (increase) {
        if (value >= 0x100) {
            value = 0xFF;
        }
    } else if (value < 0) {
        value = 0;
    }

    D_800DEF14 = value;
    return value;
}

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_80013FEC.s")
