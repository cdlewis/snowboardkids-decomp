#ifndef MENU_RENDERING_H
#define MENU_RENDERING_H

#include "common.h"

typedef struct MenuRenderSprite MenuRenderSprite;
typedef struct MenuRenderSpriteActor MenuRenderSpriteActor;
typedef u16 MenuGlyphScript;

#ifdef MENU_RENDERING_C
void func_8000EA80(s16 x, s16 y, s32 texture, u16 tileIndex, u16 scaleX, u16 scaleY, u8 startS, u8 startT,
                   u8 width, u8 height);
void drawMenuSprite(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, u8 palette, u8 flip);
void func_8000F0EC(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, u8 flip, u8 palette,
                   s32 clipLeft, s32 clipTop, s32 clipRight, s32 clipBottom);
void drawMenuSpriteWithAlpha(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, u8 palette, u16 alpha, u8 flip);
void func_8000F970(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, u8 palette, u16 alpha, u8 flip,
                   s32 clipLeft, s32 clipTop, s32 clipRight, s32 clipBottom);
void func_80010074(s16 x, s16 y, s32 texture, u16 tileIndex, s32 alpha);
void func_8001061C(s16 x, s16 y, s32 texture, u16 tileIndex, u8 srcX, u8 srcY, u8 width, u8 height, u16 scaleX,
                   u16 scaleY);
void func_80010BCC(s16 x, s16 y, s32 texture, u16 tileIndex, u16 scaleX, u16 scaleY, u8 palette, u8 flip);
void func_80011264(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height);
void func_800112F4(s16 x, s16 y, s32 texture, u16 tileIndex, u16 unused, u16 alpha, s16 clipRight, s16 clipBottom);
void func_80011854(void);
void func_8001185C(s16 x, s16 y, s32 texture, u16 assetId, u16 width, u16 height);
void func_80011C18(MenuRenderSpriteActor *actor);
void func_80011C3C(MenuRenderSpriteActor *actor);
void func_80011D44(MenuRenderSprite *sprite);
s32 func_80011D74(MenuRenderSprite *sprite, s32 arg1, s16 x, s16 y);
void func_80011D6C(void);
void func_800128C8(volatile s16 x, s16 y, u16 *script, s32 palette, u16 scale, u16 arg5);
void func_800129DC(volatile s16 x, s16 y, u16 *script, s32 palette, u16 scale);
void func_80012AE4(s16 x, s16 y, u16 glyph, u8 palette, u16 scale, u16 arg5);
void func_8001303C(volatile s16 x, s16 y, MenuGlyphScript *script, s32 palette, u16 scale, u16 red, u16 blue);
void drawMenuGlyphScript(s32 x, s32 y, u8 *text, s32 palette, s32 scale, s32 colorMode);
void func_80013284(s16 x, s16 y, u16 glyph, u8 palette, u16 scale, u16 colorMode, s32 fontBank);
void func_800137C8(s16 x, s16 y, u16 tileX, s32 tileY, u16 palette, u16 scale);
void drawMenuAsciiText(s16 x, s16 y, u8 *text, u16 palette, u16 scale);
void func_80013DFC(s16 x0, s16 y0, s16 x1, s16 y1, s16 r, s16 g, s16 b);
s32 stepMenuFadeAlpha(s32 value, s16 step, u8 increase);
void func_80013FEC(s16 x, s16 y, s32 texture, u16 imageIndex0, u16 imageIndex1, u8 alpha);
#elif defined(MENU_RENDERING_BROAD_PROTOTYPES)
void func_8000EA80(s16 x, s16 y, s32 texture, s32 tileIndex, s32 scaleX, s32 scaleY, s32 startS, s32 startT,
                   s32 width, s32 height);
void drawMenuSprite(s16 x, s16 y, s32 texture, s32 tileIndex, s32 width, s32 height, s32 palette, s32 flip);
void func_8000F0EC(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, u8 flip, u8 palette,
                   s32 clipLeft, s32 clipTop, s32 clipRight, s32 clipBottom);
#ifdef MENU_RENDERING_F8AC_U16_ALPHA_PROTOTYPE
void drawMenuSpriteWithAlpha(s32 x, s32 y, s32 texture, s32 tileIndex, s32 width, s32 height, s32 palette, u16 alpha, s32 flip);
#else
void drawMenuSpriteWithAlpha(s32 x, s32 y, s32 texture, s32 tileIndex, s32 width, s32 height, s32 palette, s32 alpha, s32 flip);
#endif
void func_8000F970(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, u8 palette, u16 alpha, u8 flip,
                   s32 clipLeft, s32 clipTop, s32 clipRight, s32 clipBottom);
void func_80010074(s16 x, s16 y, s32 texture, s32 tileIndex, s32 alpha);
void func_8001061C(s16 x, s16 y, s32 texture, u16 tileIndex, s32 srcX, s32 srcY, s32 width, s32 height, s32 scaleX,
                   s32 scaleY);
void func_80010BCC(s16 x, s16 y, s32 texture, s32 tileIndex, s32 scaleX, s32 scaleY, s32 palette, s32 flip);
void func_80011264(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height);
void func_800112F4(s16 x, s16 y, s32 texture, u16 tileIndex, u16 unused, u16 alpha, s16 clipRight, s16 clipBottom);
void func_80011854(void);
void func_8001185C(s16 x, s16 y, s32 texture, u16 assetId, u16 width, u16 height);
void func_80011C18(MenuRenderSpriteActor *actor);
void func_80011C3C(MenuRenderSpriteActor *actor);
void func_80011D44(MenuRenderSprite *sprite);
s32 func_80011D74(void *sprite, s32 arg1, s16 x, s16 y);
void func_80011D6C(void);
void func_800128C8(s16 x, s16 y, void *script, s32 palette, s32 scale, s32 arg5);
void func_800129DC(s16 x, s16 y, void *script, s32 palette, s32 scale);
void func_80012AE4(s16 x, s16 y, u16 glyph, u8 palette, u16 scale, u16 arg5);
void func_8001303C(s32 x, s32 y, void *script, s32 palette, s32 scale, s32 red, s32 blue);
void drawMenuGlyphScript(s32 x, s32 y, u8 *text, s32 palette, s32 scale, s32 colorMode);
void func_80013284(s16 x, s16 y, u16 glyph, u8 palette, u16 scale, u16 colorMode, s32 fontBank);
void func_800137C8(s16 x, s16 y, u16 tileX, s32 tileY, u16 palette, u16 scale);
void drawMenuAsciiText(s16 x, s16 y, u8 *text, s32 palette, s32 scale);
void func_80013DFC(s16 x0, s16 y0, s16 x1, s16 y1, s16 r, s16 g, s16 b);
#ifdef MENU_RENDERING_S16_STEP_PROTOTYPE
s32 stepMenuFadeAlpha(s16 value, s32 step, s32 increase);
#else
s32 stepMenuFadeAlpha(s32 value, s32 step, s32 increase);
#endif
void func_80013FEC(s16 x, s16 y, s32 texture, s32 imageIndex0, s32 imageIndex1, s32 alpha);
#else
void func_8000EA80(s16 x, s16 y, s32 texture, s32 tileIndex, s32 scaleX, s32 scaleY, s32 startS, s32 startT,
                   s32 width, s32 height);
void drawMenuSprite(s16 x, s16 y, s32 texture, s32 tileIndex, s32 width, s32 height, s32 palette, s32 flip);
void func_8000F0EC(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, u8 flip, u8 palette,
                   s32 clipLeft, s32 clipTop, s32 clipRight, s32 clipBottom);
void drawMenuSpriteWithAlpha(s32 x, s32 y, s32 texture, s32 tileIndex, s32 width, s32 height, s32 palette, s32 alpha, s32 flip);
void func_8000F970(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height, u8 palette, u16 alpha, u8 flip,
                   s32 clipLeft, s32 clipTop, s32 clipRight, s32 clipBottom);
void func_80010074(s16 x, s16 y, s32 texture, u16 tileIndex, s32 alpha);
void func_8001061C(s16 x, s16 y, s32 texture, u16 tileIndex, u8 srcX, u8 srcY, u8 width, u8 height, u16 scaleX,
                   u16 scaleY);
void func_80010BCC(s16 x, s16 y, s32 texture, u16 tileIndex, u16 scaleX, u16 scaleY, u8 palette, u8 flip);
void func_80011264(s16 x, s16 y, s32 texture, u16 tileIndex, u16 width, u16 height);
void func_800112F4(s16 x, s16 y, s32 texture, u16 tileIndex, u16 unused, u16 alpha, s16 clipRight, s16 clipBottom);
void func_80011854(void);
void func_8001185C(s16 x, s16 y, s32 texture, u16 assetId, u16 width, u16 height);
void func_80011C18(MenuRenderSpriteActor *actor);
void func_80011C3C(MenuRenderSpriteActor *actor);
void func_80011D44(MenuRenderSprite *sprite);
s32 func_80011D74(MenuRenderSprite *sprite, s32 arg1, s16 x, s16 y);
void func_80011D6C(void);
void func_800128C8(s16 x, s16 y, u16 *script, s32 palette, u16 scale, u16 arg5);
void func_800129DC(s16 x, s16 y, u16 *script, s32 palette, u16 scale);
void func_80012AE4(s16 x, s16 y, u16 glyph, u8 palette, u16 scale, u16 arg5);
void func_8001303C(s16 x, s16 y, MenuGlyphScript *script, s32 palette, u16 scale, u16 red, u16 blue);
void drawMenuGlyphScript(s32 x, s32 y, u8 *text, s32 palette, s32 scale, s32 colorMode);
void func_80013284(s16 x, s16 y, u16 glyph, u8 palette, u16 scale, u16 colorMode, s32 fontBank);
void func_800137C8(s16 x, s16 y, u16 tileX, s32 tileY, u16 palette, u16 scale);
void drawMenuAsciiText(s16 x, s16 y, u8 *text, u16 palette, u16 scale);
void func_80013DFC(s16 x0, s16 y0, s16 x1, s16 y1, s16 r, s16 g, s16 b);
#ifdef MENU_RENDERING_S16_STEP_PROTOTYPE
s32 stepMenuFadeAlpha(s16 value, s32 step, s32 increase);
#else
s32 stepMenuFadeAlpha(s32 value, s32 step, s32 increase);
#endif
void func_80013FEC(s16 x, s16 y, s32 texture, u16 imageIndex0, u16 imageIndex1, u8 alpha);
#endif

#endif
