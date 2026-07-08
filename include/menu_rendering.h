#ifndef MENU_RENDERING_H
#define MENU_RENDERING_H

#include "common.h"

typedef struct MenuRenderSprite MenuRenderSprite;
typedef struct MenuRenderSpriteActor MenuRenderSpriteActor;
typedef u16 MenuGlyphScript;

void func_8000F030(s16 x, s16 y, s32 texture, s32 tileIndex, s32 width, s32 height, s32 palette, s32 flip);
void func_8000F8AC(s32 x, s32 y, s32 texture, s32 tileIndex, s32 width, s32 height, s32 palette, s32 alpha, s32 flip);
void func_8000F970(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, u8 palette, u16 alpha, u8 flip,
                   s32 clipLeft, s32 clipTop, s32 clipRight, s32 clipBottom);
void func_80011C18(MenuRenderSpriteActor *actor);
void func_80011D44(MenuRenderSprite *sprite);
s32 func_80011D74(MenuRenderSprite *sprite, s32 arg1, s16 x, s16 y);
void func_800129DC(s16 x, s16 y, u16 *script, s32 palette, u16 scale);
void func_8001303C(s16 x, s16 y, MenuGlyphScript *script, s32 palette, u16 scale, u16 red, u16 blue);
void func_80013154(s32 x, s32 y, u8 *text, s32 palette, s32 scale, s32 colorMode);
void func_80013D0C(s16 x, s16 y, u8 *text, u16 palette, u16 scale);
s32 func_80013F88(s32 value, s32 step, s32 increase);

#endif
