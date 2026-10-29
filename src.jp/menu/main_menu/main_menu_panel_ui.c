#include "game/engine/system_render.h"
#include "common.h"
#include "game/race/race_state.h"
#include "game/engine/game_task_scheduler.h"
#include "font_encoding.h"
#include "game/audio/sound_manager.h"
#include "game/engine/asset_manager.h"
#include "game/engine/render_callback.h"
#include "game/engine/relocatable_heap.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/system_runtime.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/main_menu/main_menu_panel_ui.h"
#include "game/menu/renderer/menu_renderer.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/menu/renderer/menu_screen_effects.h"
#include "game/race/flow/race_flow.h"

#define mainMenuModeSelectPrimaryIconTiles gMainMenuModeSelectByteData.titleAndPrimaryIcon.bytes.primaryIconTiles
#define mainMenuModeSelectSecondaryIconTiles gMainMenuModeSelectByteData.secondaryIconTiles
#define mainMenuModeSelectUpTargets gMainMenuModeSelectByteData.upTargets
#define mainMenuModeSelectDownTargets gMainMenuModeSelectByteData.downTargets
#define mainMenuModeSelectLeftTargets gMainMenuModeSelectByteData.leftTargets
#define mainMenuModeSelectRightTargets gMainMenuModeSelectByteData.rightTargets
#define MENU_PANEL_INPUT_REPEAT_FRAMES 4
#define MENU_PANEL_TEXTURE_HANDLE (gAssetHandles[0x2A])
#define mainMenuModeSelectTopRowIconTiles gMainMenuModeSelectByteData.topRowIconTiles
#define mainMenuModeSelectBottomRowIconTiles gMainMenuModeSelectByteData.bottomRowIconTiles
#define MENU_PANEL_SCROLL_STEP 0x10
#define MENU_PANEL_SCROLL_LIMIT 0x79

MenuGlyphScript mainMenuModeSelectTitleText[] = {
    0x0003, 0x000D, 0x004C, 0x0052, 0x0036, 0x003A, 0x0047, 0x00C9, 0x00D5, 0x008F, 0x00C9, 0xFFFD,
    0x000A, 0x007F, 0x004F, 0x003F, 0x008E, 0x0094, 0x009B, 0x00A2, 0x0097, 0x00C6, 0x009A, 0x0091,
    0x0032, 0xFFFF,
};

MenuGlyphScript gMainMenuSlapstickDescriptionText[] = {
    0x0105, 0x00A8, 0x004C, 0x0081, 0x002B, 0x007A, 0x00BC, 0x00A9, 0x00B3, 0x0097, 0x00F3, 0x005D,
    0x002B, 0x005F, 0x003F, 0x006E, 0xFFFD, 0x00F8, 0x0066, 0x0041, 0x007B, 0x0068, 0x008E, 0x0094,
    0x0091, 0x0095, 0x0098, 0x00B8, 0x00A0, 0x0095, 0x00B6, 0x00F9, 0x00B2, 0xFFFD, 0x00A1, 0x00B5,
    0x0091, 0x00F3, 0xFFFC, 0x0006, 0x0090, 0x009F, 0x00B7, 0x00B3, 0x009C, 0x0091, 0x004B, 0x003D,
    0x003A, 0x0053, 0x0105, 0xFFFC, 0x0007, 0x00C9, 0xFFFD, 0x009C, 0x0032, 0xFFFB, 0xFFFC, 0x0006,
    0x00A9, 0x00B7, 0x0109, 0x0105, 0xFFFC, 0x0007, 0x00BC, 0x0090, 0x009F, 0x00B8, 0x00A3, 0x00F3,
    0x0066, 0x0041, 0x007B, 0x0068, 0x00A9, 0x010D, 0xFFFD, 0x00A5, 0x0099, 0x00BA, 0x008F, 0x00C9,
    0x00F3, 0x0070, 0x006A, 0x0070, 0x006A, 0x00A3, 0x0099, 0x00BA, 0x00BC, 0x008A, 0x00A2, 0xFFFD,
    0x0091, 0x008A, 0x00A2, 0x009B, 0x00AE, 0x0091, 0x00AE, 0x009C, 0x0032, 0xFFFF,
};

MenuGlyphScript gMainMenuParachuteDescriptionText[] = {
    0xFFFC, 0x0006, 0x0080, 0x0066, 0x004B, 0x003C, 0x002B, 0x0053, 0x0105, 0xFFFC, 0x0007, 0x00BC,
    0x0090, 0x009F, 0x00B8, 0x00A3, 0x00F3, 0x0066, 0x0041, 0x007B, 0xFFFD, 0x0068, 0x00A9, 0x00F3,
    0x0091, 0x0096, 0x0094, 0x0091, 0x00B5, 0x0097, 0x00D9, 0x00AC, 0x00AE, 0x0091, 0x0090, 0x00BC,
    0x008A, 0xFFFD, 0x00A2, 0x0091, 0x0096, 0x00AE, 0x009C, 0x0032, 0xFFFB, 0x009B, 0x00CB, 0x00B6,
    0x0097, 0x009C, 0x00B8, 0x00A3, 0x0066, 0x0041, 0x007B, 0x0068, 0x00A9, 0x005B, 0x006B, 0x005B,
    0x006B, 0xFFFD, 0x00A3, 0xFFFC, 0x0006, 0x0080, 0x0066, 0x004B, 0x003C, 0x002B, 0x0053, 0xFFFC,
    0x0007, 0x00C9, 0x0094, 0x00B7, 0x00A2, 0x0096, 0x00AE, 0x009C, 0x00BC, 0x00F3, 0xFFFD, 0xFFFC,
    0x0006, 0x009E, 0x00A8, 0x0090, 0x0091, 0x00C6, 0x00A9, 0xFFFC, 0x0007, 0x00F3, 0xFFFC, 0x0006,
    0x00A4, 0x00A5, 0x00B2, 0x009C, 0x00B8, 0x0099, 0x00A3, 0x00BC, 0x00C9, 0xFFFD, 0x0096, 0x00AE,
    0x009D, 0x008F, 0xFFFC, 0x0007, 0x0032, 0xFFFF,
};

MenuGlyphScript gMainMenuSnowmanDescriptionText[] = {
    0x0045, 0x007E, 0x00A5, 0x0090, 0x009F, 0x00B8, 0x00A3, 0x00A9, 0x00A7, 0x0095, 0x0093, 0x008A,
    0x00A2, 0x00F3, 0xFFFC, 0x0006, 0x0072, 0x006E, 0xFFFD, 0x0071, 0x006E, 0x00A5, 0x00A3, 0x00CD,
    0x004B, 0x003D, 0x003A, 0x0053, 0x0105, 0xFFFC, 0x0007, 0x00C9, 0x009C, 0x0032, 0xFFFB, 0xFFFC,
    0x0006, 0x0113, 0x00C6, 0x00B8, 0x00AE, 0x0105, 0xFFFC, 0x0007, 0x00BC, 0x0090, 0x009F, 0x00B8,
    0x00A3, 0x00F3, 0x0066, 0x0041, 0x007B, 0x0068, 0x00A9, 0xFFFD, 0xFFFC, 0x0006, 0x0113, 0x00C6,
    0x00B8, 0x00AE, 0xFFFC, 0x0007, 0x00A5, 0x00A4, 0x008A, 0x00A2, 0x009B, 0x00AE, 0x0091, 0x00AE,
    0x009C, 0x0032, 0xFFFC, 0x0006, 0x0113, 0x00C6, 0xFFFD, 0x00B8, 0x00AE, 0x00A5, 0x00A4, 0x008A,
    0x00A2, 0x0091, 0x00B8, 0x0090, 0x0091, 0x00C6, 0x00A9, 0xFFFC, 0x0007, 0x00F3, 0xFFFC, 0x0006,
    0x00A4, 0x00A5, 0x00B2, 0xFFFD, 0x00C9, 0x0096, 0x00AE, 0x009D, 0x008F, 0xFFFC, 0x0007, 0x0032,
    0xFFFB, 0x00C2, 0x00CD, 0x008F, 0x00BC, 0xFFFC, 0x0006, 0x0113, 0x00C6, 0x00B8, 0x00AE, 0xFFFC,
    0x0007, 0x00A5, 0x009A, 0x00B9, 0x00A2, 0x009B, 0x00AE, 0x008A, 0x009F, 0xFFFD, 0x00A3, 0x0096,
    0x00A9, 0x00F3, 0xFFFC, 0x0006, 0x0003, 0x000D, 0x004C, 0x0052, 0x0036, 0x003A, 0x0047, 0x00A3,
    0x000A, 0x007F, 0x004F, 0x003F, 0xFFFD, 0x008E, 0x006C, 0x0050, 0x003B, 0x006C, 0x0050, 0x003B,
    0x0094, 0x009C, 0xFFFC, 0x0007, 0x00A3, 0x00F3, 0x00A9, 0x00B3, 0x0097, 0x00B2, 0x00A3, 0xFFFD,
    0x00A5, 0x00B2, 0x00CA, 0x00B7, 0x00AE, 0x009C, 0x0032, 0xFFFF,
};

MenuGlyphScript gMainMenuFreezeShotDescriptionText[] = {
    0xFFFC, 0x0006, 0x005B, 0x0067, 0x002B, 0x0073, 0x0105, 0xFFFC, 0x0007, 0x00BC, 0x0090, 0x009F,
    0x00B8, 0x00A3, 0x00F3, 0x0066, 0x0041, 0x007B, 0x0068, 0x00A9, 0xFFFD, 0x009E, 0x0099, 0x00C9,
    0xFFFC, 0x0006, 0x0099, 0x0094, 0x00B7, 0x00C8, 0x0098, 0xFFFC, 0x0007, 0x00A5, 0x00A4, 0x008A,
    0x00A2, 0x009B, 0x00AE, 0x0091, 0x00F3, 0xFFFD, 0xFFFC, 0x0006, 0x009B, 0x00CB, 0x00B6, 0x0097,
    0x00A8, 0x0090, 0x0091, 0x00C6, 0x00AE, 0x008A, 0x009F, 0x0097, 0x0092, 0x00C0, 0x0098, 0x00A4,
    0xFFFD, 0x0097, 0x00A4, 0x008A, 0x00A2, 0x009B, 0x00AE, 0x0091, 0x00AE, 0x009C, 0xFFFC, 0x0007,
    0x0032, 0xFFFB, 0x00C2, 0x00CD, 0x008F, 0x00BC, 0xFFFC, 0x0006, 0x0099, 0x0094, 0x00B7, 0x00C8,
    0x0098, 0xFFFC, 0x0007, 0x00A5, 0x009A, 0x00B9, 0x00A2, 0x009B, 0x00AE, 0x008A, 0xFFFD, 0x009F,
    0x00A3, 0x0096, 0x00A9, 0x00F3, 0xFFFC, 0x0006, 0x0003, 0x000D, 0x004C, 0x0052, 0x0036, 0x003A,
    0x0047, 0x00A3, 0x000A, 0x007F, 0x004F, 0xFFFD, 0x003F, 0x008E, 0x006C, 0x0050, 0x003B, 0x006C,
    0x0050, 0x003B, 0x0094, 0x009C, 0xFFFC, 0x0007, 0x00A3, 0x00F3, 0x00A9, 0x00B3, 0x0097, 0xFFFD,
    0x00B2, 0x00A3, 0x00A5, 0x00B2, 0x00CA, 0x00B7, 0x00AE, 0x009C, 0x0032, 0xFFFF,
};

MenuGlyphScript gMainMenuBombDescriptionText[] = {
    0xFFFC, 0x0006, 0x0114, 0x0105, 0xFFFC, 0x0007, 0x00BC, 0x0066, 0x0041, 0x007B, 0x0068, 0x00B3,
    0x0045, 0x007E, 0x00A5, 0x0090, 0x009F, 0x00B8, 0x00A3, 0x00F3, 0xFFFD, 0xFFFC, 0x0006, 0x00CB,
    0x0097, 0x00A9, 0x00A1, 0xFFFC, 0x0007, 0x00BC, 0x0094, 0x0099, 0x008A, 0x00A2, 0x00F3, 0x00AE,
    0x0096, 0x0099, 0x00AE, 0x00B9, 0x009F, 0xFFFD, 0x0066, 0x0041, 0x007B, 0x0068, 0x00A9, 0x00AB,
    0x0096, 0x00A3, 0x00CB, 0x009A, 0x00B9, 0x00A2, 0x0076, 0x0042, 0x003F, 0x009B, 0xFFFD, 0x00AE,
    0x009C, 0x0032, 0xFFFB, 0xFFFC, 0x0006, 0x00CB, 0x0097, 0x00A9, 0x00A1, 0xFFFC, 0x0007, 0x00A5,
    0x00AE, 0x0096, 0x0099, 0x00AE, 0x00B9, 0x00B8, 0x00A3, 0x00F3, 0x0092, 0x008A, 0x009F, 0xFFFD,
    0xFFFC, 0x0006, 0x00C2, 0x00CD, 0x008F, 0x00B2, 0x00AB, 0x008A, 0x00A3, 0x00CB, 0x009A, 0x00B9,
    0x00A2, 0x009B, 0x00AE, 0x0092, 0xFFFC, 0x0007, 0x00A8, 0x00C9, 0xFFFD, 0x00A0, 0x008C, 0x0092,
    0x0091, 0x009B, 0x00AE, 0x009B, 0x008D, 0x0092, 0x0032, 0xFFFF,
};

MenuGlyphScript gMainMenuSpeedFanDescriptionText[] = {
    0xFFFC, 0x0006, 0x004C, 0x0081, 0x002B, 0x007A, 0x005B, 0x0035, 0x003F, 0xFFFC, 0x0007, 0x008E,
    0x00A1, 0x0095, 0x0092, 0x00A3, 0x00F3, 0x007F, 0x002B, 0x007A, 0xFFFD, 0x00A8, 0x0092, 0x009B,
    0x00BA, 0x00A5, 0x005B, 0x0035, 0x003F, 0x00BC, 0x00A1, 0x0091, 0x00A2, 0x00F3, 0x009B, 0x00CB,
    0x00B6, 0xFFFD, 0x0097, 0x00A8, 0x0090, 0x0091, 0x00C6, 0xFFFC, 0x0006, 0x004C, 0x0081, 0x002B,
    0x007A, 0x00BC, 0x0090, 0x00BC, 0x00B7, 0x00AE, 0x009C, 0xFFFC, 0x0007, 0x0032, 0xFFFB, 0x0045,
    0x007E, 0x00A5, 0x00CD, 0x00A1, 0x0095, 0x008A, 0x009F, 0x00B7, 0x009C, 0x00B8, 0x00A3, 0x00A4,
    0x0097, 0x00A4, 0x008A, 0xFFFD, 0x00A2, 0x009B, 0x00AE, 0x0092, 0x00A8, 0x00C9, 0x00F3, 0x00A4,
    0x00B9, 0x00B8, 0x00AE, 0x00C9, 0x00A9, 0x00F3, 0x00C9, 0x0096, 0xFFFD, 0x00B8, 0x00C6, 0x0098,
    0xFFFC, 0x0006, 0x0049, 0x002B, 0x004C, 0x00BC, 0x00AE, 0x008A, 0x009C, 0x00BE, 0x00A4, 0x00A3,
    0x0099, 0x00BA, 0xFFFC, 0x0007, 0x00C9, 0xFFFD, 0x00A1, 0x0095, 0x0092, 0x0099, 0x00A3, 0x008E,
    0x0094, 0x009C, 0x009C, 0x00B1, 0x009B, 0x00AE, 0x009C, 0x0032, 0xFFFF, 0x0000,
};

MenuGlyphScript gMainMenuPansDescriptionText[] = {
    0x0099, 0x00A8, 0x0040, 0x0041, 0x0052, 0x0060, 0x008E, 0x00A1, 0x0095, 0x0092, 0x00A3, 0x00F3,
    0x0090, 0x009F, 0x00AE, 0x00A8, 0xFFFD, 0x00F4, 0x0095, 0x00B6, 0xFFFC, 0x0006, 0x004F, 0x0066,
    0x0041, 0xFFFC, 0x0007, 0x00BC, 0x0094, 0x00A0, 0x00A2, 0x0096, 0x00A2, 0x00F3, 0x0066, 0x0041,
    0x007B, 0xFFFD, 0x0068, 0x009F, 0x00A0, 0x008E, 0xFFFC, 0x0006, 0x0083, 0x0050, 0x003B, 0x003F,
    0x0049, 0x00A5, 0x00A1, 0x00CD, 0x009B, 0x00A2, 0x009B, 0x00AE, 0xFFFD, 0x0091, 0x00AE, 0x009C,
    0xFFFC, 0x0007, 0x0032, 0xFFFB, 0x0066, 0x0041, 0x007B, 0x0068, 0x00A5, 0xFFFC, 0x0006, 0x004F,
    0x0066, 0x0041, 0xFFFC, 0x0007, 0x008E, 0x00A1, 0x0095, 0x00BB, 0x00B9, 0x009F, 0x00A3, 0x0096,
    0xFFFD, 0x00A9, 0x00F3, 0x004F, 0x0041, 0x005F, 0x003F, 0x006E, 0x00B5, 0x0097, 0xFFFC, 0x0006,
    0x0115, 0x0116, 0xFFFC, 0x0007, 0x0040, 0x0041, 0x0052, 0x0060, 0x008E, 0xFFFD, 0x00A1, 0x0095,
    0x0093, 0x00CB, 0x00F3, 0x0095, 0x00BB, 0x009C, 0x0099, 0x00A3, 0x00BC, 0x00C9, 0x0096, 0x00AE,
    0x009C, 0x0032, 0xFFFB, 0x00C2, 0x00CD, 0x008F, 0x00BC, 0xFFFC, 0x0006, 0x004F, 0x0066, 0x0041,
    0xFFFC, 0x0007, 0x00C9, 0x0083, 0x0050, 0x003B, 0x003F, 0x0049, 0x00A5, 0x009A, 0x00B9, 0xFFFD,
    0x00A2, 0x009B, 0x00AE, 0x008A, 0x009F, 0x00A3, 0x0096, 0x00A9, 0x00F3, 0xFFFC, 0x0006, 0x0003,
    0x000D, 0x004C, 0x0052, 0x0036, 0x003A, 0x0047, 0xFFFD, 0x00B3, 0x000A, 0x007F, 0x004F, 0x003F,
    0x008E, 0x006C, 0x0050, 0x003B, 0x006C, 0x0050, 0x003B, 0x0094, 0x009C, 0xFFFC, 0x0007, 0x00A3,
    0x00F3, 0xFFFD, 0x00A9, 0x00B3, 0x0097, 0x00B2, 0x00A3, 0x00A5, 0x00B2, 0x00CA, 0x00B7, 0x00AE,
    0x009C, 0x0032, 0xFFFF, 0x0000,
};

MenuGlyphScript gMainMenuGhostDescriptionText[] = {
    0x0099, 0x00A8, 0x0040, 0x0041, 0x0052, 0x0060, 0x008E, 0x00A1, 0x0095, 0x0092, 0x00A3, 0x00F3,
    0x0066, 0x0041, 0x007B, 0x0068, 0xFFFD, 0x00A8, 0x00C6, 0x00B9, 0x0095, 0x00A5, 0xFFFC, 0x0006,
    0x0070, 0x002B, 0x004C, 0x0053, 0xFFFC, 0x0007, 0x00BC, 0x00A3, 0x00B7, 0x00A1, 0x0091, 0x00A2,
    0x00F3, 0xFFFD, 0x009B, 0x00CB, 0x00B6, 0x0097, 0x00A8, 0x0090, 0x0091, 0x00C6, 0xFFFC, 0x0006,
    0x004C, 0x0081, 0x002B, 0x007A, 0x008E, 0x0094, 0x009E, 0x0097, 0xFFFD, 0x009B, 0x00AE, 0x009C,
    0xFFFC, 0x0007, 0x0032, 0xFFFB, 0x009F, 0x0091, 0x00A2, 0x0091, 0x00A9, 0xFFFC, 0x0006, 0x010D,
    0x00A8, 0x00FC, 0x008E, 0x009C, 0x00CE, 0x00B8, 0x0066, 0x0041, 0x007B, 0x0068, 0xFFFD, 0x00A5,
    0x00A3, 0x00B7, 0x00A1, 0x0096, 0x00AE, 0x009C, 0xFFFC, 0x0007, 0x00BC, 0x00F3, 0x009F, 0x00AE,
    0x00A5, 0x00C2, 0x00CD, 0x008F, 0x00A5, 0xFFFD, 0x00A3, 0x00B7, 0x00A1, 0x0091, 0x009F, 0x00B7,
    0x00F3, 0x0003, 0x00F3, 0x0004, 0x0117, 0x00A8, 0x0066, 0x0041, 0x007B, 0x0068, 0xFFFD, 0x00A5,
    0x00A3, 0x00B7, 0x00A1, 0x0097, 0x0099, 0x00A3, 0x00B2, 0x0090, 0x00B7, 0x00AE, 0x009C, 0x0032,
    0xFFFF, 0x0000,
};

MenuGlyphScript gMainMenuRatFaceDescriptionText[] = {
    0x0099, 0x00A8, 0x0040, 0x0041, 0x0052, 0x0060, 0x008E, 0x00A1, 0x0095, 0x0092, 0x00A3, 0xFFFC,
    0x0006, 0x00A7, 0x00C3, 0x00AF, 0x0102, 0x0118, 0xFFFC, 0x0007, 0xFFFD, 0x00BC, 0x0090, 0x00B6,
    0x00BB, 0x00B9, 0x00A2, 0x00F3, 0x0066, 0x0041, 0x007B, 0x0068, 0x009F, 0x00A0, 0x0095, 0x00B6,
    0xFFFD, 0xFFFC, 0x0006, 0x0094, 0x0106, 0x00F8, 0x0010, 0x00F9, 0xFFFC, 0x0007, 0x008E, 0x00A3,
    0x008A, 0x00A2, 0x0096, 0x00A2, 0x0097, 0x00B9, 0x00AE, 0x009C, 0x0032, 0xFFFB, 0x0066, 0x0041,
    0x007B, 0x0068, 0x009F, 0x00A0, 0x00BC, 0x0090, 0x00AE, 0x00B7, 0xFFFC, 0x0006, 0x0094, 0x0106,
    0xFFFC, 0x0007, 0x008E, 0x00B2, 0x008A, 0x00A2, 0xFFFD, 0x0091, 0x00A4, 0x0091, 0x00A3, 0x00F3,
    0x00B2, 0x00B6, 0x0093, 0x00B8, 0xFFFC, 0x0006, 0x0094, 0x0106, 0xFFFC, 0x0007, 0x00B2, 0x009C,
    0x0097, 0x00A4, 0x0097, 0xFFFD, 0x00A4, 0x00B7, 0x00AE, 0x009C, 0x0032, 0xFFFF,
};

MenuGlyphScript gMainMenuRockDescriptionText[] = {
    0x0099, 0x00A8, 0x0040, 0x0041, 0x0052, 0x0060, 0x00A9, 0x00F3, 0xFFFC, 0x0006, 0x0102, 0x0104,
    0xFFFC, 0x0007, 0x008E, 0x0049, 0x002B, 0x004C, 0x00A5, 0x0094, 0xFFFD, 0x0091, 0x00A2, 0x00F3,
    0xFFFC, 0x0006, 0x0112, 0x00BA, 0x0095, 0x00B6, 0x0094, 0x0091, 0x0095, 0x0098, 0x00A2, 0x0097,
    0x00B8, 0x0066, 0x0041, 0xFFFD, 0x007B, 0x0068, 0x008E, 0x00C2, 0x008B, 0x00AE, 0x009C, 0x00B8,
    0xFFFC, 0x0007, 0x009F, 0x00B1, 0x00A8, 0x00B2, 0x00A8, 0x00C9, 0x009C, 0x0032, 0xFFFB, 0x0066,
    0x0041, 0x007B, 0x0068, 0x00BC, 0x00A3, 0x0094, 0x00B7, 0x009E, 0x0092, 0x00A4, 0x00A3, 0x0099,
    0x00BA, 0x00A5, 0xFFFD, 0x0099, 0x008A, 0x009E, 0x00B7, 0x0094, 0x0091, 0x00A2, 0x0094, 0x0098,
    0x00CB, 0x00F3, 0xFFFC, 0x0006, 0x0066, 0x0041, 0x007B, 0x0068, 0x008E, 0xFFFD, 0x00AA, 0x008A,
    0x0095, 0x0098, 0x00A2, 0x0099, 0x00BA, 0x00CB, 0x009D, 0x00B8, 0x0099, 0x00A3, 0x00BC, 0x00C9,
    0x0096, 0x00AE, 0xFFFD, 0x009C, 0xFFFC, 0x0007, 0x0032, 0xFFFF,
};

MenuGlyphScript gMainMenuInvisibleDescriptionText[] = {
    0x0099, 0x00A8, 0x0040, 0x0041, 0x0052, 0x0060, 0x008E, 0x00A1, 0x0095, 0x0092, 0x00A3, 0x00F3,
    0x009C, 0x0099, 0x009B, 0x00A8, 0xFFFD, 0x0090, 0x0091, 0x00C6, 0x009C, 0x00BC, 0x009F, 0x008E,
    0x0098, 0x009B, 0x00A2, 0x00F3, 0xFFFC, 0x0006, 0x0115, 0x0116, 0xFFFC, 0x0007, 0x00A5, 0x00A4,
    0x00B8, 0xFFFD, 0x0099, 0x00A3, 0x00BC, 0x00C9, 0x0096, 0x00AE, 0x009C, 0x0032, 0xFFFB, 0xFFFC,
    0x0006, 0x0115, 0x0116, 0xFFFC, 0x0007, 0x00A5, 0x00A4, 0x008A, 0x00A2, 0x0091, 0x00B8, 0x0090,
    0x0091, 0x00C6, 0x00A9, 0x00F3, 0xFFFC, 0x0006, 0x004B, 0x003D, 0x003A, 0xFFFD, 0x0053, 0x0105,
    0xFFFC, 0x0007, 0x00B3, 0xFFFC, 0x0006, 0x004F, 0x0066, 0x0041, 0xFFFC, 0x0007, 0x00A8, 0x0099,
    0x0092, 0x00BF, 0x0096, 0x00BC, 0x0090, 0x009F, 0x00B7, 0x00AE, 0xFFFD, 0x009D, 0x008F, 0x0032,
    0x009F, 0x00C6, 0x009B, 0x00F3, 0x0045, 0x007E, 0x00A4, 0x00CA, 0x00A5, 0x0090, 0x009F, 0x00B8,
    0x00A3, 0xFFFD, 0x00B2, 0x00A3, 0x00A5, 0x00B2, 0x00CA, 0x008A, 0x00A2, 0x009B, 0x00AE, 0x0091,
    0x00AE, 0x009C, 0x0032, 0xFFFF,
};

MenuGlyphScript gMainMenuSlapstickTitleText[] = {
    0x00F5, 0x00A9, 0x00B7, 0x0109, 0x0105, 0x00F6, 0xFFFF, 0x0000,
};

MenuGlyphScript gMainMenuParachuteTitleText[] = {
    0x00F5, 0x0080, 0x0066, 0x004B, 0x003C, 0x002B, 0x0053, 0x0105, 0x00F6, 0xFFFF,
};

MenuGlyphScript gMainMenuFreezeShotTitleText[] = {
    0x00F5, 0x005B, 0x0067, 0x002B, 0x0073, 0x0105, 0x00F6, 0xFFFF,
};

MenuGlyphScript gMainMenuSnowmanTitleText[] = {
    0x00F5, 0x0113, 0x00C6, 0x00B8, 0x00AE, 0x0105, 0x00F6, 0xFFFF,
};

MenuGlyphScript gMainMenuBombTitleText[] = {
    0x00F5, 0x0114, 0x0105, 0x00F6, 0xFFFF, 0x0000,
};

MenuGlyphScript gMainMenuSpeedFanTitleText[] = {
    0x00F5, 0x004C, 0x0081, 0x002B, 0x007A, 0x005B, 0x0035, 0x003F, 0x00F6, 0xFFFF,
};

MenuGlyphScript gMainMenuGhostTitleText[] = {
    0x00F5, 0x0070, 0x002B, 0x004C, 0x0053, 0x00F6, 0xFFFF, 0x0000,
};

MenuGlyphScript gMainMenuPansTitleText[] = {
    0x00F5, 0x004F, 0x0066, 0x0041, 0x00F6, 0xFFFF,
};

MenuGlyphScript gMainMenuRockTitleText[] = {
    0x00F5, 0x0102, 0x0104, 0x00F6, 0xFFFF, 0x0000,
};

MenuGlyphScript gMainMenuRatFaceTitleText[] = {
    0x00F5, 0x00A7,
};

MainMenuModeSelectByteData gMainMenuModeSelectByteData = {
    { { 0x00C3, 0x00AF, 0x0102, 0x0118, 0x00F6, 0xFFFF, 0x00F5, 0x0115, 0x0116, 0x00F6, 0xFFFF, 0x0000 } },
    { 5, 8 },
    { 11, 14, 17, 0, 0, 0 },
    { 20, 21, 22, 23, 24, 25, 0, 0 },
    { 0, 1, 2, 3, 4, 5, 1, 2, 3, 3, 4, 5, 9, 0, 0, 0 },
    { 0, 6, 7, 8, 10, 11, 12, 12, 12, 12, 12, 12, 12, 0, 0, 0 },
    { 0, 1, 1, 2, 3, 4, 6, 6, 7, 8, 9, 10, 12, 0, 0, 0 },
    { 0, 2, 3, 4, 5, 5, 7, 8, 9, 10, 11, 11, 12, 0, 0, 0 }
};

MenuGlyphScript *mainMenuModeDescriptionTexts[] = {
    gMainMenuSlapstickDescriptionText, gMainMenuSlapstickDescriptionText, gMainMenuParachuteDescriptionText, gMainMenuFreezeShotDescriptionText, gMainMenuSnowmanDescriptionText, gMainMenuBombDescriptionText, gMainMenuSpeedFanDescriptionText, gMainMenuGhostDescriptionText,
    gMainMenuPansDescriptionText, gMainMenuRockDescriptionText, gMainMenuRatFaceDescriptionText, gMainMenuInvisibleDescriptionText,
};

MenuGlyphScript *mainMenuModeDescriptionTitles[] = {
    gMainMenuSlapstickTitleText, gMainMenuSlapstickTitleText, gMainMenuParachuteTitleText, gMainMenuFreezeShotTitleText, gMainMenuSnowmanTitleText, gMainMenuBombTitleText, gMainMenuSpeedFanTitleText, gMainMenuGhostTitleText,
    gMainMenuPansTitleText, gMainMenuRockTitleText, gMainMenuRatFaceTitleText, &gMainMenuModeSelectByteData.titleAndPrimaryIcon.finalTitleText[6],
};

MenuGlyphScript raceGhostUnavailableText[] = {
    0x0099, 0x00A8, 0x0079, 0x002B, 0x004F, 0x00A9, 0x0070, 0x002B, 0x004C, 0x0053, 0x00A5, 0xFFFD,
    0x00C9, 0x0096, 0x00AE, 0x009D, 0x008F, 0x0032, 0xFFFF, 0x0000,
};

MenuGlyphScript mainMenuSettingsTitleText[] = {
    0x0002, 0x00F1, 0x00F2, 0x00F4, 0x00ED, 0x00EE, 0x00A8, 0x007B, 0x0053, 0x0068, 0x0069, 0x002B,
    0x004C, 0x00A8, 0xFFFD, 0x0068, 0x002B, 0x0068, 0x008E, 0x00AC, 0x008F, 0x0099, 0x0092, 0x009B,
    0x00AE, 0x009C, 0x0032, 0x004C, 0x0052, 0xFFFD, 0x0036, 0x003A, 0x0047, 0x00C9, 0x0093, 0x00B6,
    0x008F, 0x00C9, 0x0097, 0x00C6, 0x009A, 0x0091, 0x0032, 0xFFFF,
};

MenuGlyphScript raceRecordSettingsTitleText[] = {
    0x010C, 0x0107, 0x00EF, 0x008E, 0x0003, 0x000D, 0x004C, 0x0052, 0x0036, 0x003A, 0x0047, 0x00C9,
    0xFFFD, 0x0093, 0x00B6, 0x008F, 0x00C9, 0x00F3, 0x000A, 0x007F, 0x004F, 0x003F, 0x008E, 0x0094,
    0x009B, 0x00A2, 0xFFFD, 0x0097, 0x00C6, 0x009A, 0x0091, 0x0032, 0xFFFF, 0x0000,
};

MenuGlyphScript gRaceRecordOneLapText[] = {
    0x0001, 0x010C, 0x00C9, 0x0090, 0x009E, 0x00CD, 0xFFFF, 0x0000,
};

MenuGlyphScript gRaceRecordTwoLapsText[] = {
    0x0002, 0x010C, 0x00C9, 0x0090, 0x009E, 0x00CD, 0xFFFF, 0x0000,
};

MenuGlyphScript gRaceRecordThreeLapsText[] = {
    0x0003, 0x010C, 0x00C9, 0x0090, 0x009E, 0x00CD, 0xFFFF, 0x0000,
};

MenuGlyphScript gRaceRecordFourLapsText[] = {
    0x0004, 0x010C, 0x00C9, 0x0090, 0x009E, 0x00CD, 0xFFFF, 0x0000,
};

MenuGlyphScript gRaceRecordFiveLapsText[] = {
    0x0005, 0x010C, 0x00C9, 0x0090, 0x009E, 0x00CD, 0xFFFF, 0x0000,
};

MenuGlyphScript gRaceRecordSixLapsText[] = {
    0x0006, 0x010C, 0x00C9, 0x0090, 0x009E, 0x00CD, 0xFFFF, 0x0000,
};

MenuGlyphScript gRaceRecordSevenLapsText[] = {
    0x0007, 0x010C, 0x00C9, 0x0090, 0x009E, 0x00CD, 0xFFFF, 0x0000,
};

MenuGlyphScript gRaceRecordEightLapsText[] = {
    0x0008, 0x010C, 0x00C9, 0x0090, 0x009E, 0x00CD, 0xFFFF, 0x0000,
};

MenuGlyphScript gRaceRecordNineLapsText[] = {
    0x0009, 0x010C, 0x00C9, 0x0090, 0x009E, 0x00CD, 0xFFFF, 0x0000,
};

MenuGlyphScript *raceRecordLapCountTexts[] = {
    gRaceRecordOneLapText,
    gRaceRecordOneLapText,
    gRaceRecordTwoLapsText,
    gRaceRecordThreeLapsText,
    gRaceRecordFourLapsText,
    gRaceRecordFiveLapsText,
    gRaceRecordSixLapsText,
    gRaceRecordSevenLapsText,
    gRaceRecordEightLapsText,
    gRaceRecordNineLapsText,
};

void drawMainMenuModeSelectFrame(void *arg0) {
    s32 edgeX;
    s32 edgeY;
    s32 y;
    s32 yOrigin;
    s32 xOrigin;

    xOrigin = -0x74;
    yOrigin = 0x54;

    drawMenuSprite(-0x84, -0x64, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 2, 0x20, 0x20, 0, 0);
    drawMenuSprite(0x78, -0x64, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 4, 0x20, 0x20, 0, 0);

    edgeX = xOrigin;
    do {
        drawMenuSprite(edgeX, -0x64, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, 0);
        drawMenuSprite(edgeX, -0x3C, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 8, 0x20, 0x20, 0, 0);
        edgeX += 0x10;
    } while (edgeX < 0x7C);

    drawMenuSprite(-0x84, -0x3C, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, 0);
    drawMenuSprite(0x78, -0x3C, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, 0);

    edgeY = -yOrigin;
    do {
        drawMenuSprite(-0x84, edgeY, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 5, 0x20, 0x20, 0, 0);
        drawMenuSprite(0x78, edgeY, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 6, 0x20, 0x20, 0, 0);
        edgeY += 0x10;
    } while (edgeY < -0x34);

    if (yOrigin) {}

    for (edgeX = xOrigin; edgeX != 0x7C; edgeX += 0x10) {
        for (y = -yOrigin; y != -0x34; y += 0x10) {
            drawMenuSprite(edgeX, y, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, 0);
        }
    }

    drawMenuColoredGlyphScript(-0x68, -0x58, mainMenuModeSelectTitleText, 0, 0x100, 5, 0x29);
}

void drawMainMenuModeSelectIcons(MenuPanelActor *arg0) {
    s32 alpha;
    s32 i;
    s32 next;
    s32 x;
    u8 *tile;

    if (arg0) {}

    x = -0x6E;
    // clang-format off
    tile = mainMenuModeSelectTopRowIconTiles; i = 0; do {
        // clang-format on
        next = i + 1;
        alpha = 0;
        if (next == gMainMenuModeSelection) {
            alpha = arg0->x;
        }

        drawMenuFillRectangle((s16)(x + 0xA), -0x16, 0x22, 0x22, 0x40, alpha / 4, 0);
        drawMenuFillRectangle((s16)(x + 4), -0x1C, 0x22, 0x22, 0xFF, alpha, 0);
        drawAssetTableSprite((s16)(x + 5), -0x1B, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), *tile);
        i = next;
        tile++;
        x += 0x2C;
    } while (next < 5);

    x = -0x84;
    tile = mainMenuModeSelectBottomRowIconTiles;
    i = 0;
    do {
        alpha = 0;
        if ((i + 6) == gMainMenuModeSelection) {
            alpha = arg0->x;
        }

        drawMenuFillRectangle((s16)(x + 0xA), 0x1E, 0x22, 0x22, 0, alpha / 4, 0x40);
        drawMenuFillRectangle((s16)(x + 4), 0x18, 0x22, 0x22, arg0->x * 0, alpha, 0xFF);
        drawAssetTableSprite((s16)(x + 5), 0x19, getRelocatableHeapBlockBase(gAssetHandles[0x1F]), *tile);
        i++;
        tile++;
        x += 0x2C;
    } while (i != 6);

    if (gMainMenuModeSelection == 0xC) {
        if (arg0->x != 0) {
            drawAssetTableSprite(-0x30, 0x4D, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 2);
            drawPulsingAssetTableSprite(-0x30, 0x4D, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 3);
            return;
        }
        drawAssetTableSpriteWithExplicitPalette(-0x30, 0x4D, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 2, 1);
        drawPulsingAssetTableSprite(-0x30, 0x4D, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 3);
        return;
    }

    drawAssetTableSpriteWithExplicitPalette(-0x30, 0x4D, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 2, 1);
}

void updateMainMenuModeSelectGrid(MenuPanelActor *arg0) {
    s32 pressedInput;
    s32 var_v0;
    u8 temp_a2;
    u8 temp_a2_2;
    u8 temp_a2_3;
    u8 temp_a2_4;

    // clang-format off
    if (arg0) {} \
    if ((gMenuFadeAlpha == 0) && (gMainMenuSelectionResult == 0)) { \
        pressedInput = gPlayerInputPressed[0]; \
        { \
            if (pressedInput & (STICK_UP | U_JPAD)) { \
                temp_a2 = mainMenuModeSelectUpTargets[gMainMenuModeSelection]; \
                if (temp_a2 != gMainMenuModeSelection) { \
                    gMainMenuModeSelection = temp_a2; \
                    enqueueSoundEffect(SFX_MENU_CURSOR, SFX_PRIORITY_NORMAL); \
                    pressedInput = gPlayerInputPressed[0]; \
                } \
            } else if (pressedInput & (STICK_DOWN | D_JPAD)) { \
                var_v0 = gMainMenuModeSelection; \
                temp_a2_2 = mainMenuModeSelectDownTargets[var_v0]; \
                if (temp_a2_2 != var_v0) { \
                    gMainMenuModeSelection = temp_a2_2; \
                    enqueueSoundEffect(SFX_MENU_CURSOR, SFX_PRIORITY_NORMAL); \
                    pressedInput = gPlayerInputPressed[0]; \
                } \
            } else if (pressedInput & (STICK_RIGHT | R_JPAD)) { \
                temp_a2_3 = mainMenuModeSelectRightTargets[gMainMenuModeSelection]; \
                if (temp_a2_3 != gMainMenuModeSelection) { \
                    gMainMenuModeSelection = temp_a2_3; \
                    enqueueSoundEffect(SFX_MENU_CURSOR, SFX_PRIORITY_NORMAL); \
                    pressedInput = gPlayerInputPressed[0]; \
                } \
            } else if (pressedInput & (STICK_LEFT | L_JPAD)) { \
                temp_a2_4 = mainMenuModeSelectLeftTargets[gMainMenuModeSelection]; \
                if (temp_a2_4 != gMainMenuModeSelection) { \
                    gMainMenuModeSelection = temp_a2_4; \
                    enqueueSoundEffect(SFX_MENU_CURSOR, SFX_PRIORITY_NORMAL); \
                    pressedInput = gPlayerInputPressed[0]; \
                } \
            }
            // clang-format on
            if (pressedInput & (A_BUTTON | B_BUTTON | START_BUTTON)) {
                gMainMenuSelectionResult = 1;
                if (pressedInput & B_BUTTON) {
                    gMainMenuModeSelection = 0xC;
                }
                enqueueSoundEffect(SFX_CONFIRM, SFX_PRIORITY_NORMAL);
            }
        }
    }

    if (gMainMenuSelectionResult == 0) {
        arg0->x = gFrameCounter & 0xF;
        if (arg0->x >= 9) {
            arg0->x = 0x10 - arg0->x;
        }
        arg0->x = (arg0->x * 0x10) + 0x7F;
    } else if (gFrameCounter & 1) {
        arg0->x = 0;
    } else {
        arg0->x = 0xFF;
    }

    addRenderCallback(&gMenuRenderCallbackList, drawMainMenuModeSelectFrame, arg0);
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawMainMenuModeSelectIcons, (void *)arg0);
}

void initMainMenuModeSelectGrid(MenuPanelActor *arg0) {
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateMainMenuModeSelectGrid);
}

void drawMainMenuModeDescriptionPanel(MenuPanelActor *arg0) {
    s32 i;
    u16 unused;
    u16 lineY;
    s32 visibleIndex;
    s32 scriptIndex;
    MenuGlyphScript glyphText[2];
    u16 lineX;
    u16 colorMode;
    MenuGlyphScript *title;
    MenuGlyphScript *titleCursor;

    drawMenuSprite(
        (s16)(arg0->x - 4),
        (s16)(arg0->y + 0x14),
        getRelocatableHeapBlockBase(gAssetHandles[0x2A]),
        2,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0xF8),
        (s16)(arg0->y + 0x14),
        getRelocatableHeapBlockBase(gAssetHandles[0x2A]),
        4,
        0x20,
        0x20,
        0,
        0
    );

    i = 0;
    for (;;) {
        drawMenuSprite(
            (s16)(arg0->x + i + 0xC),
            (s16)(arg0->y + 0x14),
            getRelocatableHeapBlockBase(gAssetHandles[0x2A]),
            3,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            (s16)(arg0->x + i + 0xC),
            (s16)(arg0->y + 0x4C),
            getRelocatableHeapBlockBase(gAssetHandles[0x2A]),
            8,
            0x20,
            0x20,
            0,
            0
        );
        i += 0x10;

        if (!(i < 0xF0)) {
            break;
        }
    }

    drawMenuSprite(
        (s16)(arg0->x - 4),
        (s16)(arg0->y + 0x4C),
        getRelocatableHeapBlockBase(gAssetHandles[0x2A]),
        7,
        0x20,
        0x20,
        0,
        0
    );
    drawMenuSprite(
        (s16)(arg0->x + 0xF8),
        (s16)(arg0->y + 0x4C),
        getRelocatableHeapBlockBase(gAssetHandles[0x2A]),
        9,
        0x20,
        0x20,
        0,
        0
    );

    i = (arg0->selectionState == 4) * 0;
    for (;;) {
        drawMenuSprite(
            (s16)(arg0->x - 4),
            (s16)(arg0->y + i + 0x24),
            getRelocatableHeapBlockBase(gAssetHandles[0x2A]),
            5,
            0x20,
            0x20,
            0,
            0
        );
        drawMenuSprite(
            (s16)(arg0->x + 0xF8),
            (s16)(arg0->y + i + 0x24),
            getRelocatableHeapBlockBase(gAssetHandles[0x2A]),
            6,
            0x20,
            0x20,
            0,
            0
        );
        i += 0x10;

        if (!(i <= 0x2F)) {
            break;
        }
    }

    glyphText[1] = 0xFFFF;
    visibleIndex = 0;
    scriptIndex = 0;
    lineX = 0;
    lineY = 0;
    colorMode = 7;

    for (;;) {
        if (visibleIndex == arg0->selectedTile) {
            visibleIndex = 0;
            break;
        }

        glyphText[0] = arg0->tileList[scriptIndex++];
        switch (glyphText[0]) {
            case 0xFFFF:
                arg0->selectionState = 1;
                visibleIndex = arg0->selectedTile;
                continue;
            case 0xFFFD:
                lineY = lineY + 0x10;
                lineX = 0;
                continue;
            case 0xFFFB:
                arg0->selectionState = 2;
                arg0->tileListStart = &arg0->tileList[scriptIndex];
                visibleIndex = arg0->selectedTile;
                continue;
            case 0xFFFC:
                colorMode = arg0->tileList[scriptIndex++];
                continue;
            default:
                drawMenuColoredGlyphScript(
                    (s16)(arg0->x + lineX),
                    (s16)(arg0->y + lineY + 0x18),
                    glyphText,
                    0,
                    0x100,
                    colorMode,
                    0x29
                );
                lineX = lineX + 0x10;
                visibleIndex++;
                continue;
        }
    }

    if ((arg0->selectionState != 0) && (gMainMenuSelectionResult == 0)) {
        drawMenuSprite(
            (s16)(arg0->x + 0xF4),
            (s16)(arg0->y + 0x48),
            getRelocatableHeapBlockBase(gAssetHandles[0x2A]),
            (gFrameCounter >> 4) & 1,
            0x20,
            0x20,
            0,
            0
        );
    }

    title = mainMenuModeDescriptionTitles[gMainMenuModeSelection];
    titleCursor = title;
    if (*title != 0xFFFF) {
        for (;;) {
            visibleIndex++;
            titleCursor++;

            if (!(*titleCursor != 0xFFFF)) {
                break;
            }
        }
    }
    visibleIndex *= 0x10;
    visibleIndex = -(visibleIndex / 2);

    drawMenuColoredGlyphScript((s16)visibleIndex, (s16)(-0x48 - arg0->y), title, 0, 0x100, 4, 0x29);

    if ((s32)gMainMenuModeSelection < 6) {
        drawAssetTableSprite(
            -0x10,
            (s16)(-0x30 - arg0->y),
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            mainMenuModeSelectPrimaryIconTiles[gMainMenuModeSelection]
        );
        return;
    }

    drawAssetTableSprite(
        -0x10,
        (s16)(-0x30 - arg0->y),
        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
        mainMenuModeSelectSecondaryIconTiles[gMainMenuModeSelection]
    );
}

void scrollMainMenuModeDescriptionPanelOut(MenuPanelActor *arg0) {
    arg0->y += MENU_PANEL_SCROLL_STEP;
    if (arg0->y >= MENU_PANEL_SCROLL_LIMIT) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawMainMenuModeDescriptionPanel, arg0);
}

void updateMainMenuModeDescriptionPanel(MenuPanelActor *arg0) {
    s16 timer;
    u8 state;

    timer = arg0->inputRepeatTimer;
    if (timer == 0) {
        if (arg0->selectionState == 0) {
            arg0->selectedTile++;
        }
        arg0->inputRepeatTimer = MENU_PANEL_INPUT_REPEAT_FRAMES;
    } else {
        arg0->inputRepeatTimer = timer - 1;
    }

    if (gPlayerInputPressed[0] & (A_BUTTON | START_BUTTON)) {
        state = arg0->selectionState;
        switch (state) {
            case 0:
                arg0->selectedTile = 0xE7;
                break;
            case 1:
                gMainMenuSelectionResult = 1;
                setCallbackTaskCallback(arg0, (CallbackTaskCallback)scrollMainMenuModeDescriptionPanelOut);
                break;
            case 2:
                arg0->selectedTile = 0;
                arg0->selectionState = 0;
                arg0->inputRepeatTimer = 0;
                arg0->tileList = arg0->tileListStart;
                break;
        }
        enqueueSoundEffect(SFX_CONFIRM, SFX_PRIORITY_NORMAL);
    }

    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawMainMenuModeDescriptionPanel, arg0);
}

void initMainMenuModeDescriptionPanel(MenuPanelActor *arg0) {
    arg0->x = -0x80;
    arg0->y = 4;
    arg0->selectionState = 0;
    arg0->inputRepeatTimer = 0;
    arg0->tileList = mainMenuModeDescriptionTexts[gMainMenuModeSelection];
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateMainMenuModeDescriptionPanel);
}

void drawMainMenuModeSelectMenuOptions(void *arg0) {
    if (gMenuFlowState == 0) {
        if ((gMainMenuSelectionResult == 0) || (gFrameCounter & 1)) {
            drawAssetTableSprite(-0x30, -0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0);
            drawPulsingAssetTableSprite(-0x30, -0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 3);
        } else {
            drawAssetTableSpriteWithExplicitPalette(
                -0x30,
                -0x20,
                getRelocatableHeapBlockBase(gAssetHandles[0x28]),
                0,
                1
            );
            drawPulsingAssetTableSprite(-0x30, -0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 3);
        }
    } else {
        drawAssetTableSpriteWithExplicitPalette(-0x30, -0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0, 1);
    }

    if (gMenuFlowState == 1) {
        if ((gMainMenuSelectionResult == 0) || (gFrameCounter & 1)) {
            drawAssetTableSprite(-0x30, -8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 1);
            drawPulsingAssetTableSprite(-0x30, -8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 3);
        } else {
            drawAssetTableSpriteWithExplicitPalette(-0x30, -8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 1, 1);
            drawPulsingAssetTableSprite(-0x30, -8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 3);
        }
    } else {
        drawAssetTableSpriteWithExplicitPalette(-0x30, -8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 1, 1);
    }

    if (gMenuFlowState == 2) {
        if ((gMainMenuSelectionResult == 0) || (gFrameCounter & 1)) {
            drawAssetTableSprite(-0x30, 0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 2);
            drawPulsingAssetTableSprite(-0x30, 0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 3);
            return;
        }
        drawAssetTableSpriteWithExplicitPalette(-0x30, 0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 2, 1);
        drawPulsingAssetTableSprite(-0x30, 0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 3);
        return;
    }

    drawAssetTableSpriteWithExplicitPalette(-0x30, 0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 2, 1);
}

void updateMainMenuModeSelectMenuOptions(MenuPanelActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, drawMainMenuModeSelectMenuOptions, arg0);
}

void initMainMenuModeSelectMenuOptions(MenuPanelActor *arg0) {
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateMainMenuModeSelectMenuOptions);
}

void drawRaceSetupPreviewCornerMasks(void *arg0) {
    drawAssetTableSpriteWithDefaultPalette(-0x84, -0x64, getRelocatableHeapBlockBase(gAssetHandles[6]), 6);
    drawAssetTableSpriteWithDefaultPalette(0x74, -0x64, getRelocatableHeapBlockBase(gAssetHandles[6]), 7);
    drawAssetTableSpriteWithDefaultPalette(-0x84, 4, getRelocatableHeapBlockBase(gAssetHandles[6]), 8);
    drawAssetTableSpriteWithDefaultPalette(0x74, 4, getRelocatableHeapBlockBase(gAssetHandles[6]), 9);
}

void updateRaceSetupPreviewCornerMasks(MenuPanelActor *arg0) {
    if (gMainMenuSelectionResult == 0) {
        addRenderCallback(&gMenuRenderCallbackList, drawRaceSetupPreviewCornerMasks, arg0);
        return;
    }
    removeCallbackTask(arg0);
}

void initRaceSetupPreviewCornerMasks(MenuPanelActor *arg0) {
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceSetupPreviewCornerMasks);
}

void drawRaceGhostUnavailableMessage(void *arg0) {
    drawMenuColoredGlyphScript(-0x78, -0x58, raceGhostUnavailableText, 1, 0x100, 5, 0x28);
}

void updateRaceGhostUnavailableMessage(void *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, (RenderCallback)drawRaceGhostUnavailableMessage, arg0);
}

void drawMainMenuSettingsPanel(void *arg0) {
    s32 y;
    s32 edgeX;
    s32 edgeY;
    s32 yOrigin;
    s32 xOrigin;

    xOrigin = -0x74;
    yOrigin = 0x54;

    drawMenuSprite(-0x84, -0x64, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 2, 0x20, 0x20, 0, 0);
    drawMenuSprite(0x78, -0x64, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 4, 0x20, 0x20, 0, 0);

    edgeX = xOrigin;
    for (;;) {
        drawMenuSprite(edgeX, -0x64, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, 0);
        drawMenuSprite(edgeX, -0x2C, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 8, 0x20, 0x20, 0, 0);
        edgeX += 0x10;

        if (!(edgeX < 0x7C)) {
            break;
        }
    }

    drawMenuSprite(-0x84, -0x2C, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, 0);
    drawMenuSprite(0x78, -0x2C, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, 0);

    edgeY = -yOrigin;
    for (;;) {
        drawMenuSprite(-0x84, edgeY, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 5, 0x20, 0x20, 0, 0);
        drawMenuSprite(0x78, edgeY, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 6, 0x20, 0x20, 0, 0);
        edgeY += 0x10;

        if (!(edgeY < -0x24)) {
            break;
        }
    }

    if (yOrigin) {}

    for (edgeX = xOrigin; edgeX != 0x7C; edgeX += 0x10) {
        for (y = -yOrigin; y != -0x24; y += 0x10) {
            drawMenuSprite(edgeX, y, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 0xB, 0x20, 0x20, 0, 0);
        }
    }
    drawMenuColoredGlyphScript(-0x70, -0x58, mainMenuSettingsTitleText, 0, 0x100, 5, 0x29);
}

void drawMainMenuSettingsOptions(void *arg0) {
    s32 var_v0;
    s32 sp28;

    var_v0 = 1;
    sp28 = var_v0;
    if (gMenuFlowState == 0) {
        var_v0 = 2;
        sp28 = var_v0;
    }
    drawAssetTableSpriteWithExplicitPalette(-0x78, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 4, sp28);
    sp28 = 1;
    if (gRaceRecordSettingsEnabled == 1) {
        drawAssetTableSpriteWithExplicitPalette(0x10, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 7, 2);
        drawAssetTableSpriteWithExplicitPalette(0x48, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 8, 1);
        if (gMenuFlowState == 0) {
            drawPulsingAssetTableSprite(0x10, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        } else {
            drawAssetTableSprite(0x10, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        }
    } else {
        drawAssetTableSpriteWithExplicitPalette(0x10, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 7, 1);
        drawAssetTableSpriteWithExplicitPalette(0x48, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 8, 2);
        if (gMenuFlowState == 0) {
            drawPulsingAssetTableSprite(0x48, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        } else {
            drawAssetTableSprite(0x48, -0x10, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        }
    }
    if (gMenuFlowState == 1) {
        sp28 = 2;
    }
    drawAssetTableSpriteWithExplicitPalette(-0x78, 8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 5, sp28);
    sp28 = 1;
    if (gRaceCourseModelEffectsDisabled == 0) {
        drawAssetTableSpriteWithExplicitPalette(0x10, 8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 9, 2);
        drawAssetTableSpriteWithExplicitPalette(0x48, 8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xA, 1);
        if (gMenuFlowState == 1) {
            drawPulsingAssetTableSprite(0x10, 8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        } else {
            drawAssetTableSprite(0x10, 8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        }
    } else {
        drawAssetTableSpriteWithExplicitPalette(0x10, 8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 9, 1);
        drawAssetTableSpriteWithExplicitPalette(0x48, 8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xA, 2);
        if (gMenuFlowState == 1) {
            drawPulsingAssetTableSprite(0x48, 8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        } else {
            drawAssetTableSprite(0x48, 8, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        }
    }
    if (gMenuFlowState == 2) {
        sp28 = 2;
    }
    drawAssetTableSpriteWithExplicitPalette(-0x78, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 6, sp28);
    sp28 = 1;
    if (gRaceCourseOverlayEffectsDisabled == 0) {
        drawAssetTableSpriteWithExplicitPalette(0x10, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 9, 2);
        drawAssetTableSpriteWithExplicitPalette(0x48, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xA, 1);
        if (gMenuFlowState == 2) {
            drawPulsingAssetTableSprite(0x10, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        } else {
            drawAssetTableSprite(0x10, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        }
    } else {
        drawAssetTableSpriteWithExplicitPalette(0x10, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 9, 1);
        drawAssetTableSpriteWithExplicitPalette(0x48, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xA, 2);
        if (gMenuFlowState == 2) {
            drawPulsingAssetTableSprite(0x48, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        } else {
            drawAssetTableSprite(0x48, 0x20, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xB);
        }
    }
    if (gMenuFlowState == 3) {
        sp28 = 2;
    }
    drawAssetTableSpriteWithExplicitPalette(-0x68, 0x48, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 2, sp28);
}

void updateMainMenuSettingsPanel(MenuPanelActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, drawMainMenuSettingsPanel, arg0);
    addRenderCallback(&gMenuRenderCallbackList, drawMainMenuSettingsOptions, arg0);
}

void initMainMenuSettingsPanel(MenuPanelActor *arg0) {
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateMainMenuSettingsPanel);
}

void drawRaceRecordSettingsPanel(void *arg0) {
    s32 x;
    s32 x2;
    s16 xStart;
    s32 y1;
    s32 y;
    s32 y2;

    xStart = 0x74;

    drawMenuSprite(-0x84, 0x1C, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 2, 0x20, 0x20, 0, 0);
    drawMenuSprite(0x78, 0x1C, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 4, 0x20, 0x20, 0, 0);
    x = -xStart;
    for (;;) {
        drawMenuSprite((s16)x, 0x1C, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)x, 0x54, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 8, 0x20, 0x20, 0, 0);
        x += 0x10;

        if (!(x < 0x7C)) {
            break;
        }
    }
    drawMenuSprite(-0x84, 0x54, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, 0);
    drawMenuSprite(0x78, 0x54, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, 0);
    y1 = 0x2C;
    for (;;) {
        drawMenuSprite(-0x84, (s16)y1, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 5, 0x20, 0x20, 0, 0);
        drawMenuSprite(0x78, (s16)y1, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 6, 0x20, 0x20, 0, 0);
        y1 = (y1 + 0x10) * 1;

        if (!(y1 < 0x5C)) {
            break;
        }
    }
    for (x = -0x74; x < 0x7C; x += 0x10) {
        for (y = 0x2C; y != 0x5C; y += 0x10) {
            drawMenuSprite(
                (s16)x,
                (s16)y,
                getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE),
                0xB,
                0x20,
                0x20,
                0,
                0
            );
        }
    }
    drawMenuColoredGlyphScript(-0x70, 0x28, raceRecordSettingsTitleText, 0, 0x100, 7, 0x29);
    drawMenuSprite(-0x40, -0x30, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 2, 0x20, 0x20, 0, 0);
    drawMenuSprite(0x30, -0x30, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 4, 0x20, 0x20, 0, 0);
    x2 = -0x30;
    for (;;) {
        drawMenuSprite((s16)x2, -0x30, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 3, 0x20, 0x20, 0, 0);
        drawMenuSprite((s16)x2, 0, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 8, 0x20, 0x20, 0, 0);
        x2 = (x2 + 0x10) * 1;

        if (!(x2 < 0x30)) {
            break;
        }
    }
    y2 = -0x20;
    for (;;) {
        drawMenuSprite(-0x40, (s16)y2, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 5, 0x20, 0x20, 0, 0);
        drawMenuSprite(0x30, (s16)y2, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 6, 0x20, 0x20, 0, 0);
        y2 += 0x10;

        if (!((y2 - 0) < 0)) {
            break;
        }
    }
    for (x2 = -0x30; x2 != 0x30; x2 += 0x10) {
        for (y = -0x20; y != 0; y += 0x10) {
            drawMenuSprite(
                (s16)x2,
                (s16)y,
                getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE),
                0xB,
                0x20,
                0x20,
                0,
                0
            );
        }
    }
    drawMenuSprite(-0x40, 0, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 7, 0x20, 0x20, 0, 0);
    drawMenuSprite(0x30, 0, getRelocatableHeapBlockBase(MENU_PANEL_TEXTURE_HANDLE), 9, 0x20, 0x20, 0, 0);
    if (gMenuFlowState == 0) {
        if (gRaceLapCount != 1) {
            if (gUiBlinkTimer & 8) {
                drawAssetTableSprite(-0x30, -0x2A, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xC);
            }
        }
        if ((gRaceLapCount != 9) && (gUiBlinkTimer & 8)) {
            drawAssetTableSprite(-0x30, -6, getRelocatableHeapBlockBase(gAssetHandles[0x28]), 0xD);
        }
    }
    if ((gMenuFlowState != 0) && (gUiBlinkTimer & 1)) {
        drawMenuColoredGlyphScript(-0x30, -0x18, raceRecordLapCountTexts[gRaceLapCount], 0, 0x100, 0, 0x29);
    } else {
        drawMenuColoredGlyphScript(-0x30, -0x18, raceRecordLapCountTexts[gRaceLapCount], 0, 0x100, 5, 0x29);
    }
    drawAssetTableSprite(-0x54, -0x60, getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x5D);
    drawAssetTableSprite(4, -0x60, getRelocatableHeapBlockBase(gAssetHandles[0x1C]), 0x5E);
    drawAssetTableSprite(
        -0x4C,
        -0x5C,
        getRelocatableHeapBlockBase(gAssetHandles[0x1D]),
        (u16)courseRecordDigitTileOffsets[gRaceCourseIndex.signedValue]
    );
    drawAssetTableSprite(
        4,
        -0x5C,
        getRelocatableHeapBlockBase(gAssetHandles[0x1D]),
        courseRecordDigitTileOffsets[gRaceCourseIndex.signedValue] + 1
    );
}

void updateRaceRecordSettingsPanel(MenuPanelActor *arg0) {
    addRenderCallback(&gMenuRenderCallbackList, drawRaceRecordSettingsPanel, arg0);
}

void initRaceRecordSettingsPanel(MenuPanelActor *arg0) {
    setCallbackTaskCallback(arg0, (CallbackTaskCallback)updateRaceRecordSettingsPanel);
}
