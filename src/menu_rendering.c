#include "common.h"
#include "menu_rendering.h"

typedef struct MenuRenderTask MenuRenderTask;
typedef struct RenderCallbackNode RenderCallbackNode;

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
extern void func_80071824(MenuRenderSpriteActor *task, MenuRenderSpriteActorCallback callback);
extern RenderCallbackNode *D_80124868;
extern u32 D_80123758;

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_8000EA80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_8000F030.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_8000F0EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_8000F8AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_8000F970.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_80010074.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_8001061C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_80010BCC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_80011264.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_800112F4.s")

void func_80011854(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_8001185C.s")

void func_80011C3C(MenuRenderSpriteActor *actor);

void func_80011C18(MenuRenderSpriteActor *arg0) {
    func_80071824(arg0, func_80011C3C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_80011C3C.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_80013D0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_80013DFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_80013F88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/menu_rendering/func_80013FEC.s")
