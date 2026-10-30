#include "game/race/items/race_items.h"
#include "game/race/race_state.h"
#include "common.h"
#include <PR/os_libc.h>
#include "game/save_data.h"
#include "game/menu/renderer/menu_render_utils.h"
#include "game/engine/render_callback.h"
#include "assets.h"
#include "game/engine/asset_manager.h"
#include "game/engine/relocatable_heap.h"
#include "game/race/effects/race_visual_effects.h"
#include "game/race/player/race_player_movement.h"
#include "game/race/ui/race_hud.h"
#include "game/race/ui/race_ui_effects.h"
#include "game/race/player/race_player_input.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/game_task_scheduler.h"
#include "game/engine/system_render.h"
#include "game/race/flow/race_flow.h"

#define ASSET_HANDLE(index) (gAssetHandles[index])

#define RACE_HUD_MAIN_FONT_HANDLE (gAssetHandles[0x1C])

#define RACE_HUD_POPUP_FONT_HANDLE (gAssetHandles[0x1F])

u8 gRaceHudShotTileOffsets[8] = { 5, 5, 8, 11, 14, 17, 0, 0 };
u8 gRaceHudItemTileIds[8] = { 4, 20, 21, 22, 23, 24, 25, 0 };

RaceTimer gRaceCourseTargetTimes[10] = {
    { 3, 0, 0 },
    { 3, 0, 0 },
    { 3, 5, 0 },
    { 3, 5, 0 },
    { 3, 5, 0 },
    { 3, 5, 0 },
    { 3, 5, 0 },
    { 3, 5, 0 },
    { 1, 0, 0 },
    { 1, 0, 0 },
};

RaceTimer gScoreAttackTimeLimitsByCourse[10] = {
    { 3, 0,  0 },
    { 3, 0,  0 },
    { 3, 0,  0 },
    { 3, 0,  0 },
    { 3, 0,  0 },
    { 3, 0,  0 },
    { 3, 0,  0 },
    { 3, 0,  0 },
    { 1, 0,  0 },
    { 1, 30, 0 },
};

RaceTimer gTrickAttackTimeLimit = { 3, 0, 0 };

u16 gRaceProgressMeterIconTiles[36] = {
    0x51, 0x64, 0x65, 0x66, 0x65, 0x64, 0x52, 0x67, 0x68, 0x69, 0x68, 0x67, 0x53, 0x6A, 0x6B, 0x6C, 0x6B, 0x6A,
    0x54, 0x6D, 0x6E, 0x6F, 0x6E, 0x6D, 0x55, 0x70, 0x71, 0x72, 0x71, 0x70, 0x56, 0x73, 0x74, 0x75, 0x74, 0x73,
};

u16 gRaceProgressMeterIconPalettes[6] = { 0x17, 0x19, 0x17, 0x19, 0x17, 0x17 };

u32 gRaceCoursePlayerPathOffsetData[0x532] = {
    0x00000000, 0x00000000, 0x0303FDF7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7070D0A, 0xFAF8F7FD, 0x09090909, 0x09090909,
    0xF4F4F4F4, 0xF4F4F4F4, 0xF4F4F4F4, 0xF4F4F4F4, 0xF4F4F4F4, 0xF4F4F4E3, 0xE7E7E7E7, 0xE7E7E7E7, 0xE7DDE7E7,
    0xE7E7E7EA, 0xF00C0C0C, 0x0C0C0C0C, 0x0C0C0C0C, 0x0C0C0C0C, 0x0CF6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6,
    0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xFC030707, 0x07070707, 0x07070707, 0x07070707,
    0x07070707, 0x07070707, 0x07070707, 0x07070707, 0x07070707, 0x07111111, 0x11111111, 0x11111111, 0x11111111,
    0x11111111, 0xFAF6F6F6, 0xF6FE0000, 0xF7000000, 0x00090909, 0x09090909, 0x09000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00001313, 0x13131313,
    0x1302EEEE, 0xEEEEEEEE, 0xEE000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0xF5F5F5F7, 0xFAFB0000, 0x09101010, 0x151E1E1E, 0x1E140B0B, 0x0B0B0B0B, 0x0B0B0B0B,
    0x0B0B0B0B, 0x0B0B0E0E, 0x0E0E0E0E, 0x0E0E0E0E, 0x0E0E0E0E, 0x0E0E0E0E, 0x0E0E0E0E, 0x0E0E0E0E, 0x0EEDEDED,
    0xEDEDEDE0, 0xE0E0ECEC, 0xECECECEC, 0xECECECEC, 0xECFBFBFB, 0xFBFBFBFB, 0xFBFBFBFB, 0x00101010, 0x10101010,
    0x10101010, 0x10100A0A, 0x0A0A0A0A, 0x0A0A1313, 0x13131313, 0x13131313, 0x13131313, 0x13131313, 0x13131313,
    0x08F5F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xECEBEBEB, 0xEBEBEBEB,
    0xEBEBEBEB, 0xEBEBEBEB, 0xEBEBEBEB, 0xFBEBEBEB, 0xEBEB0000, 0x0303030F, 0x0F0F2424, 0x16050505, 0xF9F9F9F9,
    0xF9F9F9F9, 0x0E0E0E0E, 0x0E0E0E0E, 0x0E0E0E0E, 0x0E0E0E0E, 0x0E0E0E0E, 0x0E0E0707, 0x07070707, 0x07070707,
    0x07070711, 0x11111111, 0x11111111, 0x11111111, 0x11111111, 0x11111107, 0x07070707, 0x07070707, 0x07070707,
    0x07070707, 0x07FF2218, 0x03F6FD0B, 0x14090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0xF7F7F7F7, 0xE7E7E7E7, 0xE7030303, 0xFBFB0000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00F5F5F5, 0xF5F50000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00EDEDED, 0xF2F2F2F2, 0xF2F20302, 0x16000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x09090909, 0x09E5E5E5, 0xE5E9E9E9, 0x00FF0D11, 0x110D07E7,
    0xE6E6C9C9, 0xC9C9C9C9, 0xC9C9DEDE, 0xDEDEDEF4, 0xF4F4F4F4, 0xF4E6E6E6, 0xE6E6E6E6, 0xE6E6E6EF, 0xEFEFEF0E,
    0x0E0E0E0E, 0x0E0E0E0E, 0x17171717, 0x1717EFEF, 0xEFEFE7E4, 0xE4E4E4E4, 0xE4E4E4E4, 0xEBEBEBEB, 0xEBE4E4E8,
    0xFCFCFCF1, 0xE0D9D5D6, 0xE3ECF0F0, 0xF0F0F0F0, 0xF0F0F0F0, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7EFEF, 0xEFEFEFEF, 0xEFEFEFEF, 0xEFEFFCFE, 0x03030303,
    0x03030303, 0xEEEEEEEE, 0xEEEEEE05, 0x05050505, 0x05050505, 0x050505F1, 0xF1F1F1F1, 0xF1F1F1F1, 0xF1F1F1F1,
    0xF1F1F1F1, 0xF1F1F1F1, 0xF1F9F9F9, 0xF9F9F9F9, 0xF9000000, 0x00000909, 0x09090909, 0x09090D0D, 0x0D0D0D0D,
    0x0D0D0D0D, 0x0DF2F2F2, 0xF2F2F2F2, 0xDFE6E6EF, 0xEFEF0314, 0x14141414, 0x1414140F, 0x0F0F0F0F, 0x0F0F0F0F,
    0x0F0F0F0F, 0x0FF5F5F5, 0xF5F5F5F5, 0xF5F5F5F5, 0xF5F5F50C, 0x0C0C0C0C, 0x0C0C0C0C, 0x0C0C0C0C, 0x0C0C0000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xF7F70000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x000000F8, 0xF8F8F8F8, 0xF8F8F8F8, 0xF8F8F800, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00030909, 0x09090000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x09090909, 0x09090919, 0x19191910, 0x1010F1F1, 0xF1F1F1F1, 0xE6E6F2FF, 0x080B0B16, 0x16161616, 0x1616160B,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCF4F4,
    0xF4F4F4F4, 0xF4F4F4F4, 0xF4020202, 0x02020202, 0x02020202, 0x02F8F8F8, 0xF8F8F8F8, 0xF8F8F8F8, 0xF8F5F8F8,
    0xF8000000, 0x00000000, 0x00000000, 0x00000000, 0x06060606, 0x060606FC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC,
    0xFCFC0000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03000000, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0x00000000,
    0x0000F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6,
    0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6,
    0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6,
    0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF6000000,
    0x09090909, 0x09090909, 0x09090909, 0x06090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090904, 0xFC010101, 0x01010101, 0x01010101, 0x01010101,
    0x01010101, 0x01010101, 0x01010101, 0x01010101, 0x01010101, 0x01010101, 0x01010101, 0x01010101, 0x01010101,
    0x01010101, 0x01010101, 0x01010101, 0x01010101, 0x01010101, 0x01010101, 0x01010101, 0x01010101, 0x01010101,
    0x01010101, 0x01010101, 0x01010101, 0x01010101, 0x01010101, 0x01010101, 0x01010101, 0x01010101, 0x01010101,
    0x01010101, 0x01000000, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030308,
    0x47352E00, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F0F0, 0xF0F0F0FC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC,
    0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC,
    0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFF, 0x03050DFD,
    0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC,
    0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC,
    0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC, 0xFCFCFCFC,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090908, 0x08090908,
    0x06FF0000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0xF6F6F4EC, 0xF6F4F6F6, 0xF6F6F6F6, 0xF6F6F6F6, 0xF7F6F6F6, 0xFAFEFE01, 0x1B322709, 0xFEFEFEFE,
    0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE,
    0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE,
    0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE, 0xFEFEFEFE, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03000000, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7000000, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09000000, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030300, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F700, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090900, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303, 0x03030303,
    0x03030300, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7,
    0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F7F7, 0xF7F7F700,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
    0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909
};

#if defined(VERSION_JP)
const char gRaceHudChallengeTimeLimitMinutesFormat[8] = "%2.2d";
const char gRaceHudChallengeTimeLimitSecondsFormat[8] = "%2.2d";
const char gRaceHudChallengeTimeLimitFractionFormat[8] = "%2.2d";
const char gRaceHudTrickAttackPointTotalFormat[4] = "%4d";
const char gRaceHudTrickAttackSecondaryCounterFormat[4] = "%2d";
const char gRaceHudTrickAttackPointLabel[8] = "POINT";
const char gRaceHudTrickAttackTimeLimitLabel[12] = "TIME LIMIT";
const char gRaceHudScoreAttackTimeLimitMinutesFormat[8] = "%2.2d";
const char gRaceHudScoreAttackTimeLimitSecondsFormat[8] = "%2.2d";
const char gRaceHudScoreAttackTimeLimitFractionFormat[8] = "%2.2d";
const char gRaceHudScoreAttackPointTotalFormat[4] = "%2d";
const char gRaceHudScoreAttackTimeLimitLabel[12] = "TIME LIMIT";
const char gRaceHudScoreAttackPointTargetFormat[4] = "/%d";
const char gRaceHudTargetTimeChallengeMinutesFormat[8] = "%2.2d";
const char gRaceHudTargetTimeChallengeSecondsFormat[8] = "%2.2d";
const char gRaceHudTargetTimeChallengeFractionFormat[8] = "%2.2d";
const char gRaceHudTargetTimeChallengeSecondaryCounterFormat[4] = "%d";
const char gRaceHudTargetTimeChallengeLapTimeLabel[12] = "LAP TIME";
const char gRaceHudTargetTimeChallengeTargetTimeLabel[12] = "TARGET TIME";
const char gRaceHudTargetTimeChallengeTargetTimeFormat[20] = "%2.2d'%2.2d\"%2.2d";
const char gRaceHudTargetTimeChallengeLapProgressFormat[8] = "*%d/%d";
const char gRaceHudTimeTrialMinutesFormat[8] = "%2.2d";
const char gRaceHudTimeTrialSecondsFormat[8] = "%2.2d";
const char gRaceHudTimeTrialFractionFormat[8] = "%2.2d";
const char gRaceHudTimeTrialLapTimeLabel[12] = "LAP TIME";
const char gRaceHudTimeTrialBestLapLabel[12] = "BEST LAP";
const char gRaceHudTimeTrialBestLapFormat[20] = "%2.2d'%2.2d\"%2.2d";
const char gRaceHudSinglePlayerScoreFormat[8] = "%5ld";
const char gRaceHudTrainingScoreFormat[8] = "%5ld";
const char gRaceHudTwoPlayerScoreFormat[8] = "%5ld";
const char gRaceHudMultiplayerScoreFormat[8] = "%5ld";

#endif
s16 gRaceHudMode;
s16 gRaceHudCoinSpinnerFrame;
RaceTimer gRaceElapsedTimer;
RaceTimer gRaceChallengeTimeLimit;
RaceTimer gRaceTimeTrialFinishTime;
u8 gRaceTimeTrialFinishRecorded;
u8 gRaceChallengeFailed;

void initRaceHud(void) {
    LOAD_ASSET(SHARED_UI_SPRITES, 0x1F);
    gRaceHudMode = RACE_HUD_MODE_NONE;
    gRaceHudCoinSpinnerFrame = 0;

    if (gPlayerCount == 1) {
        if (gRaceSplitscreenMode == 0) {
            gRaceHudMode = RACE_HUD_MODE_ONE_PLAYER;
        }
        if (gRaceSplitscreenMode == 2) {
            gRaceHudMode = RACE_HUD_MODE_TIME_TRIAL;
        }
        if (gRaceSplitscreenMode == 1) {
            if (gRaceTypeSelection == 0) {
                gRaceHudMode = RACE_HUD_MODE_TARGET_TIME_CHALLENGE;
            }
            if (gRaceTypeSelection == 1) {
                gRaceHudMode = RACE_HUD_MODE_SCORE_ATTACK;
                gRaceChallengeTimeLimit = gScoreAttackTimeLimitsByCourse[gRaceCourseIndex.signedValue];
            }
            if (gRaceTypeSelection == 2) {
                gRaceHudMode = RACE_HUD_MODE_TRICK_ATTACK;
                gRaceChallengeTimeLimit = gTrickAttackTimeLimit;
            }
        }
    } else {
        gRaceHudMode = gPlayerCount - 1;
    }

    if ((gTrainingCourseLesson != 0) || (gMainMenuModeSelection != 0)) {
        gRaceHudMode = RACE_HUD_MODE_TRAINING_OR_DEMO;
    }

    gRaceElapsedTimer.minutes = 0;
    gRaceElapsedTimer.seconds = 0;
    gRaceElapsedTimer.fraction = 0;
    gRaceTimeTrialFinishRecorded = 0;
    gRaceChallengeFailed = 0;
}

#if defined(VERSION_US)
const char gRaceHudChallengeTimeLimitMinutesFormat[] = "%2.2d";
const char gRaceHudChallengeTimeLimitSecondsFormat[] = "%2.2d";
const char gRaceHudChallengeTimeLimitFractionFormat[] = "%2.2d";
const char gRaceHudTrickAttackPointTotalFormat[] = "%4d";
const char gRaceHudTrickAttackSecondaryCounterFormat[] = "%2d";

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
#endif
void drawTrickAttackChallengeHud(void *arg0) {
    s32 x;
#if defined(VERSION_JP)
    s32 i;
#else
    char *digit;
#endif
    char buffer[4];
#if defined(VERSION_JP)
    s32 padding[2];
#endif
    s32 palette;
    s32 finalPalette;
#if defined(VERSION_US)
    union {
        char *end;
        s32 i;
    } temp;
#endif

    palette = 0xC;
    if ((gRaceChallengeTimeLimit.minutes == 0) && (gRaceChallengeTimeLimit.seconds < 10) && (gUiBlinkTimer & 1)) {
        palette = 0x10;
    }

    {
        sprintf(buffer, gRaceHudChallengeTimeLimitMinutesFormat, gRaceChallengeTimeLimit.minutes);
        finalPalette = (u16)palette;
        x = 0x40;
#if defined(VERSION_JP)
        for (i = 0; i < 2; i++, x += 8) {
            drawAssetTableSpriteWithExplicitPalette(
                (s16)x,
                0x50,
                getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                (u8)buffer[i] - 5,
                finalPalette
            );
        }
#else
        digit = buffer; temp.end = &buffer[2]; do {
            drawAssetTableSpriteWithExplicitPalette((s16)x, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                          (u8)*digit - 5, finalPalette); digit++;
            x += 8;
        } while ((u32)digit < (u32)temp.end);
#endif

        x += 8;
        sprintf(buffer, gRaceHudChallengeTimeLimitSecondsFormat, gRaceChallengeTimeLimit.seconds);
#if defined(VERSION_JP)
        for (i = 0; i < 2; i++, x += 8) {
            drawAssetTableSpriteWithExplicitPalette(
                (s16)x,
                0x50,
                getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                (u8)buffer[i] - 5,
                finalPalette
            );
        }
#else
        digit = buffer;
        temp.end = &buffer[2];
        temp.end += 0;
        do {
            drawAssetTableSpriteWithExplicitPalette((s16)x, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                          (u8)*digit - 5, finalPalette);
            digit++;
            x += 8;
        } while ((u32)digit < (u32)temp.end);
#endif

        x += 8;
        sprintf(buffer, gRaceHudChallengeTimeLimitFractionFormat, gRaceChallengeTimeLimit.fraction >> 8);
#if defined(VERSION_JP)
        for (i = 0; i < 2; i++, x += 8) {
            drawAssetTableSpriteWithExplicitPalette(
                (s16)x,
                0x50,
                getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                (u8)buffer[i] - 5,
                finalPalette
            );
        }
#else
        digit = buffer;
        temp.end = &buffer[2];
        do {
            drawAssetTableSpriteWithExplicitPalette((s16)x, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                          (u8)*digit - 5, finalPalette);
            digit++;
            x += 8;
        } while ((u32)digit < (u32)temp.end);
#endif
    }

    drawAssetTableSpriteWithExplicitPalette(0x50, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x36, finalPalette);
    drawAssetTableSpriteWithExplicitPalette(0x68, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x35, finalPalette);

    if (gRacePlayers[0].state.trickAttackMakeCount < 10) {
        x = -0x90;
#if defined(VERSION_JP)
        i = 0;
#else
        temp.i = 0;
#endif
        if (gRacePlayers[0].state.trickAttackMakeCount > 0) {
#if defined(VERSION_JP)
            for (;;) {
                drawScaledAssetTableSprite(
                    (s16)x,
                    -0x68,
                    getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                    0x26,
                    1
                );
                i++;
#else
            do {
                drawScaledAssetTableSprite((s16)x, -0x68, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x26, 1);
                temp.i++;
#endif
                x += 8;
#if defined(VERSION_JP)

                if (!(i < gRacePlayers[0].state.trickAttackMakeCount))
                    break;
            }
#else
            } while (temp.i < gRacePlayers[0].state.trickAttackMakeCount);
#endif
        }
    } else {
        drawAssetTableSprite(-0x84, -0x60, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x26);
    }

    {
#if defined(VERSION_US)
        char *end;
#endif

        x = 0x64;
        sprintf(buffer, gRaceHudTrickAttackPointTotalFormat, gRacePlayers[0].state.trickAttackPointTotal);
#if defined(VERSION_JP)
        for (i = 0; i != 4; i++, x += 8) {
            if ((u8)buffer[i] != ' ') {
                drawAssetTableSprite(
                    (s16)x,
                    -0x58,
                    getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                    (u8)buffer[i] - 5
                );
#else
        digit = buffer;
        end = &buffer[4];
        do {
            if ((u8)*digit != ' ') {
                drawAssetTableSprite((s16)x, -0x58, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                              (u8)*digit - 5);
#endif
            }
#if defined(VERSION_JP)
        }
#else
            digit++;
            x += 8;
        } while (digit != end);
#endif
    }

    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x27);
    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x28);
    if (gRaceChallengeFailed != 0) {
        drawAssetTableSprite(0x40, 0x54, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x4F);
    }

    if (gRacePlayers[0].state.trickAttackMakeCount >= 10) {
        sprintf(buffer, gRaceHudTrickAttackSecondaryCounterFormat, gRacePlayers[0].state.trickAttackMakeCount);
        drawAssetTableSprite(-0x64, -0x59, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), (u8)buffer[0] - 5);
        drawAssetTableSprite(-0x5C, -0x59, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), (u8)buffer[1] - 5);
    }
}
// clang-format on
#if defined(VERSION_US)

const char gRaceHudTrickAttackPointLabel[] = "Point";
const char gRaceHudTrickAttackTimeLimitLabel[] = "Time Limit";

#endif
void drawTrickAttackChallengeLabels(void *arg0) {
    drawMenuAsciiTextDefaultScale(0x60, -0x61, gRaceHudTrickAttackPointLabel, 5);
    drawMenuAsciiTextDefaultScale(0x38, 0x47, gRaceHudTrickAttackTimeLimitLabel, 5);
}

#if defined(VERSION_US)
const char gRaceHudScoreAttackTimeLimitMinutesFormat[] = "%2.2d";
const char gRaceHudScoreAttackTimeLimitSecondsFormat[] = "%2.2d";
const char gRaceHudScoreAttackTimeLimitFractionFormat[] = "%2.2d";
const char gRaceHudScoreAttackPointTotalFormat[] = "%2d";

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
#endif
void drawScoreAttackChallengeHud(void *arg0) {
    s32 x;
#if defined(VERSION_JP)
    s32 i;
#else
    char *digit;
#endif
    char buffer[4];
#if defined(VERSION_JP)
    s32 padding[2];
#endif
    s32 palette;
    s32 finalPalette;
#if defined(VERSION_US)
    union {
        char *end;
        s32 i;
    } temp;
#endif

    palette = 0xC;
    if ((gRaceChallengeTimeLimit.minutes == 0) && (gRaceChallengeTimeLimit.seconds < 10) && (gUiBlinkTimer & 1)) {
        palette = 0x10;
#if defined(VERSION_US)
        if (!gRacePlayers[0].state.shieldShotCount) {
        }
#endif
    }

    {
        sprintf(buffer, gRaceHudScoreAttackTimeLimitMinutesFormat, gRaceChallengeTimeLimit.minutes);
        finalPalette = (u16)palette;
        x = 0x40;
#if defined(VERSION_JP)
        for (i = 0; i < 2; i++, x += 8) {
            drawAssetTableSpriteWithExplicitPalette(
                (s16)x,
                0x50,
                getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                (u8)buffer[i] - 5,
                finalPalette
            );
        }
#else
        digit = buffer; \
        temp.end = &buffer[2]; \
        do { \
            drawAssetTableSpriteWithExplicitPalette( \
                (s16)x, \
                0x50, \
                getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), \
                (u8)*digit - 5, \
                finalPalette \
            ); \
            digit++;
            x += 8;
        } while ((u32)digit < (u32)temp.end);
#endif

        x += 8;
        sprintf(buffer, gRaceHudScoreAttackTimeLimitSecondsFormat, gRaceChallengeTimeLimit.seconds);
#if defined(VERSION_JP)
        for (i = 0; i < 2; i++, x += 8) {
            drawAssetTableSpriteWithExplicitPalette(
                (s16)x,
                0x50,
                getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                (u8)buffer[i] - 5,
                finalPalette
            );
        }
#else
        digit = buffer;
        temp.end = &buffer[2];
        temp.end += 0;
        do {
            drawAssetTableSpriteWithExplicitPalette((s16)x, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                          (u8)*digit - 5, finalPalette);
            digit++;
            x += 8;
        } while ((u32)digit < (u32)temp.end);
#endif

        x += 8;
        sprintf(buffer, gRaceHudScoreAttackTimeLimitFractionFormat, gRaceChallengeTimeLimit.fraction >> 8);
#if defined(VERSION_JP)
        for (i = 0; i < 2; i++, x += 8) {
            drawAssetTableSpriteWithExplicitPalette(
                (s16)x,
                0x50,
                getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                (u8)buffer[i] - 5,
                finalPalette
            );
        }
#else
        digit = buffer;
        temp.end = &buffer[2];
        do {
            drawAssetTableSpriteWithExplicitPalette((s16)x, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                          (u8)*digit - 5, finalPalette);
            digit++;
            x += 8;
        } while ((u32)digit < (u32)temp.end);
#endif
    }

    drawAssetTableSpriteWithExplicitPalette(0x50, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x36, finalPalette);
    drawAssetTableSpriteWithExplicitPalette(0x68, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x35, finalPalette);
    drawAssetTableSprite(0x68, -0x60, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x20);

    {
#if defined(VERSION_US)
        char *end;
#endif

        sprintf(buffer, gRaceHudScoreAttackPointTotalFormat, gRacePlayers[0].state.scoreAttackPointTotal);
        x = 0;
#if defined(VERSION_JP)
        for (i = 0; i < 2; i++, x += 8) {
            if ((u8)buffer[i] != ' ') {
                drawAssetTableSprite(
                    (s16)(x + 0x60),
                    -0x50,
                    getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                    (u8)buffer[i] - 5
                );
#else
        digit = buffer;
        end = &buffer[2];
        do {
            if ((u8)*digit != ' ') {
                drawAssetTableSprite((s16)(x + 0x60), -0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                              (u8)*digit - 5);
#endif
            }
#if defined(VERSION_JP)
        }
#else
            digit++;
            x += 8;
        } while ((u32)digit < (u32)end);
#endif
    }

    x = 0;
#if defined(VERSION_JP)
    i = 0;
#else
    temp.i = 0;
#endif
    if (gRacePlayers[0].state.shieldShotCount > 0) {
#if defined(VERSION_JP)
        for (;;) {
#else
        do {
#endif
            drawAssetTableSprite((s16)(x - 0x88), -0x60, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x21);
#if defined(VERSION_JP)
            i++;
#else
            temp.i++;
#endif
            x += 8;
#if defined(VERSION_JP)

            if (!(i < gRacePlayers[0].state.shieldShotCount))
                break;
        }
#else
        } while (temp.i < gRacePlayers[0].state.shieldShotCount);
#endif
    }

    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x22);
    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x23);
    if (gRaceChallengeFailed != 0) {
        drawAssetTableSprite(0x40, 0x54, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x4F);
    }
}
// clang-format on
#if defined(VERSION_US)

const char gRaceHudScoreAttackTimeLimitLabel[] = "Time Limit";
const char gRaceHudScoreAttackPointTargetFormat[] = "/%d";

#endif
void drawScoreAttackChallengeLabels(void *arg0) {
    char sp18[0x20];

    drawMenuAsciiTextDefaultScale(0x38, 0x47, gRaceHudScoreAttackTimeLimitLabel, 5);
    sprintf(sp18, gRaceHudScoreAttackPointTargetFormat, gRacePlayers[0].state.scoreAttackPointTarget);
    drawMenuAsciiTextDefaultScale(0x70, -0x48, sp18, 6);
}
#if defined(VERSION_US)

const char gRaceHudTargetTimeChallengeMinutesFormat[] = "%2.2d";
const char gRaceHudTargetTimeChallengeSecondsFormat[] = "%2.2d";
const char gRaceHudTargetTimeChallengeFractionFormat[] = "%2.2d";
const char gRaceHudTargetTimeChallengeSecondaryCounterFormat[] = "%d";
#endif

void drawTargetTimeChallengeHud(void *arg0) {
    char buffer[12];
    s32 x;
    s32 i;

    sprintf(buffer, gRaceHudTargetTimeChallengeMinutesFormat, gRaceElapsedTimer.minutes);
    x = 0x48;
    for (i = 0; i < 2; i++) {
        drawAssetTableSprite((s16)x, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), (u8)buffer[i] - 5);
        x += 8;
    }

    x += 8;
    sprintf(buffer, gRaceHudTargetTimeChallengeSecondsFormat, gRaceElapsedTimer.seconds);
    for (i = 0; i < 2; i++) {
        drawAssetTableSprite((s16)x, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), (u8)buffer[i] - 5);
        x += 8;
    }

    x += 8;
    sprintf(buffer, gRaceHudTargetTimeChallengeFractionFormat, gRaceElapsedTimer.fraction >> 8);
    for (i = 0; i != 2; i++) {
        drawAssetTableSprite((s16)x, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), (u8)buffer[i] - 5);
        x += 8;
    }

    drawAssetTableSprite(0x58, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x36);
    drawAssetTableSprite(0x70, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x35);

    if (gRaceChallengeFailed != 0) {
        drawAssetTableSprite(0x48, 0x54, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x4F);
    }

    drawAssetTableSprite(-0x88, -0x60, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), gRaceHudItemTileIds[1]);
    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x24);
    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x25);

    sprintf(buffer, gRaceHudTargetTimeChallengeSecondaryCounterFormat, gRacePlayers[0].state.collectedCourseCoinMarkerCount);
    drawAssetTableSprite(-0x60, -0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), (u8)buffer[0] - 5);
    if (buffer[1] != 0) {
        drawAssetTableSprite(-0x58, -0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), (u8)buffer[1] - 5);
    }
}

#if defined(VERSION_US)
const char gRaceHudTargetTimeChallengeLapTimeLabel[] = "Lap Time";
const char gRaceHudTargetTimeChallengeTargetTimeLabel[] = "Target Time";
const char gRaceHudTargetTimeChallengeTargetTimeFormat[] = "%2.2d'%2.2d\"%2.2d";
const char gRaceHudTargetTimeChallengeLapProgressFormat[] = "*%d/%d";

#endif
void drawTargetTimeChallengeLabels(void *arg0) {
    char sp28[0x20];
    RaceTimer *targetTime;

    drawMenuAsciiTextDefaultScale(0x48, 0x47, gRaceHudTargetTimeChallengeLapTimeLabel, 5);
    drawMenuAsciiTextDefaultScale(0x32, -0x60, gRaceHudTargetTimeChallengeTargetTimeLabel, 7);
    targetTime = &gRaceCourseTargetTimes[gRaceCourseIndex.signedValue];
    sprintf(
        sp28,
        gRaceHudTargetTimeChallengeTargetTimeFormat,
        targetTime->minutes,
        targetTime->seconds,
        targetTime->fraction >> 8
    );
    drawMenuAsciiTextDefaultScale(0x48, -0x57, sp28, 7);
    sprintf(
        sp28,
        gRaceHudTargetTimeChallengeLapProgressFormat,
        gRacePlayers[0].state.collectedCourseCoinMarkerCount,
        gRacePlayers[0].state.totalCourseCoinMarkerCount
    );
    sp28[1] = ' ';
    if (sp28[2] != '/') {
        sp28[2] = ' ';
    }
    drawMenuAsciiTextDefaultScale(-0x68, -0x48, sp28, 6);
}

#if defined(VERSION_US)
const char gRaceHudTimeTrialMinutesFormat[] = "%2.2d";
const char gRaceHudTimeTrialSecondsFormat[] = "%2.2d";
const char gRaceHudTimeTrialFractionFormat[] = "%2.2d";

#endif
void drawTimeTrialHud(void *arg0) {
    char buffer[0xC];
    s32 x;
#if defined(VERSION_JP)
    s32 i;
#else
    char *digit;
    char *end;
    s32 endValue;
#endif

    sprintf(buffer, gRaceHudTimeTrialMinutesFormat, gRaceElapsedTimer.minutes);
    x = 0x48;
#if defined(VERSION_JP)
    for (i = 0; i < 2; i++, x += 8) {
        drawAssetTableSprite((s16)x, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), (u8)buffer[i] - 5);
    }
#else
    if (x && RACE_HUD_POPUP_FONT_HANDLE) {}
    digit = buffer;
    end = &buffer[2];
    do {
        drawAssetTableSprite((s16)x, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), (u8)*digit - 5);
        digit++;
        x += 8;
    } while ((u32)digit < (u32)end);
#endif

    x += 8;
    sprintf(buffer, gRaceHudTimeTrialSecondsFormat, gRaceElapsedTimer.seconds);
#if defined(VERSION_JP)
    for (i = 0; i < 2; i++, x += 8) {
        drawAssetTableSprite((s16)x, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), (u8)buffer[i] - 5);
    }
#else
    digit = buffer;
    end = &buffer[2];
    endValue = (u32)end;
    do {
        drawAssetTableSprite((s16)x, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), (u8)*digit - 5);
        digit++;
        x += 8;
    } while ((u32)digit < endValue);
#endif

    x += 8;
#if defined(VERSION_JP)
    sprintf(buffer, gRaceHudTimeTrialFractionFormat, gRaceElapsedTimer.fraction >> 8);
    for (i = 0; i != 2; i++, x += 8) {
        drawAssetTableSprite((s16)x, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), (u8)buffer[i] - 5);
    }
#else
    sprintf(end = buffer, gRaceHudTimeTrialFractionFormat, gRaceElapsedTimer.fraction >> 8);
    digit = end;
    end = &buffer[2];
    do {
        drawAssetTableSprite((s16)x, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), (u8)*digit - 5);
        digit++;
        x += 8;
    } while (digit != end);
#endif

    drawAssetTableSprite(0x58, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x36);
    drawAssetTableSprite(0x70, 0x50, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x35);

    if (gRacePlayers[0].state.itemType != RACE_ITEM_NONE) {
        drawAssetTableSprite(
            -0x10,
            -0x60,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudItemTileIds[gRacePlayers[0].state.itemType]
        );
    }

    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x24);
    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x2A);
}

#if defined(VERSION_US)
const char gRaceHudTimeTrialLapTimeLabel[] = "Lap Time";
const char gRaceHudTimeTrialBestLapLabel[] = "Best Lap";
const char gRaceHudTimeTrialBestLapFormat[] = "%2.2d'%2.2d\"%2.2d";

#endif
void drawTimeTrialLabels(void *unused) {
    char bestLapText[0x20];

    drawMenuAsciiTextDefaultScale(0x48, 0x47, gRaceHudTimeTrialLapTimeLabel, 5);
    drawMenuAsciiTextDefaultScale(0x48, -0x61, gRaceHudTimeTrialBestLapLabel, 7);
    sprintf(
        bestLapText,
        gRaceHudTimeTrialBestLapFormat,
        gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][0].minutes,
        gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][0].seconds,
        gGameSaveDataBuffer[0].timeTrialRecords[gRaceCourseIndex.signedValue][0].fraction >> 8
    );
    drawMenuAsciiTextDefaultScale(0x48, -0x58, bestLapText, 7);
}
#if defined(VERSION_US)

const char gRaceHudSinglePlayerScoreFormat[] = "%5ld";
#endif

void drawSinglePlayerRaceHud(void *arg0) {
    s32 palette;
    s32 i;
    s32 var_s1;
    char buffer[0x20];

    sprintf(buffer, gRaceHudSinglePlayerScoreFormat, gRacePlayers[0].state.score);
    if (gRacePlayers[0].state.score < 100) {
        palette = 0x10;
    } else {
        palette = 0xE;
    }

    for (i = 0, var_s1 = 0x50; i < 5; i++, var_s1 += 8) {
        if (buffer[i] != ' ') {
            drawAssetTableSpriteWithExplicitPalette(
                var_s1,
                0x50,
                getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                (u8)buffer[i] - 5,
                palette
            );
        }
    }

    drawAssetTableSprite(
        0x78,
        0x50,
        getRelocatableHeapBlockBase(RACE_HUD_MAIN_FONT_HANDLE),
        (gRaceHudCoinSpinnerFrame >> 1) + 4
    );

    if (gRacePlayers[0].state.shotPalette != 0) {
        drawScaledAssetTableSprite(
            -0x20,
            -0x60,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudShotTileOffsets[gRacePlayers[0].state.shotType] + gRacePlayers[0].state.shotCount - 1,
            gRacePlayers[0].state.shotPalette
        );
    } else {
        drawAssetTableSprite(
            -0x20,
            -0x60,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudShotTileOffsets[gRacePlayers[0].state.shotType] + gRacePlayers[0].state.shotCount - 1
        );
    }

    if (gRacePlayers[0].state.itemPalette != 0) {
        drawScaledAssetTableSprite(
            0,
            -0x60,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudItemTileIds[gRacePlayers[0].state.itemType],
            gRacePlayers[0].state.itemPalette
        );
    } else {
        drawAssetTableSprite(
            0,
            -0x60,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudItemTileIds[gRacePlayers[0].state.itemType]
        );
    }

    drawAssetTableSprite(
        -0x88,
        0x40,
        getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
        gRacePlayers[0].state.rankIndex
    );
    drawAssetTableSprite(-0x88, -0x60, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x39);
    drawAssetTableSpriteWithExplicitPalette(
        -0x68,
        -0x60,
        getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
        gRacePlayers[0].state.lapDigit + 0x2C,
        0xE
    );
    drawAssetTableSprite(-0x5C, -0x60, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x38);
    drawAssetTableSpriteWithExplicitPalette(
        -0x50,
        -0x60,
        getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
        gRaceLapCount + 0x2B,
        0xE
    );
}

void noopRaceHudCallback(void *arg0) {
}

#if defined(VERSION_US)
const char gRaceHudTrainingScoreFormat[] = "%5ld";

#endif
void drawTrainingRaceHud(void *arg0) {
    s32 palette;
    s32 i;
    s32 x;
    char buffer[0x20];

    sprintf(buffer, gRaceHudTrainingScoreFormat, gRacePlayers[0].state.score);
    if (gRacePlayers[0].state.score < 100) {
        palette = 0x10;
    } else {
        palette = 0xE;
    }

    for (i = 0, x = 0x50; i < 5; i++, x += 8) {
        if (buffer[i] != ' ') {
            drawAssetTableSpriteWithExplicitPalette(
                x,
                0x50,
                getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
                (u8)buffer[i] - 5,
                palette
            );
        }
    }

    drawAssetTableSprite(
        0x78,
        0x50,
        getRelocatableHeapBlockBase(RACE_HUD_MAIN_FONT_HANDLE),
        (gRaceHudCoinSpinnerFrame >> 1) + 4
    );

    if (gRacePlayers[0].state.shotPalette != 0) {
        drawScaledAssetTableSprite(
            -0x20,
            -0x60,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudShotTileOffsets[gRacePlayers[0].state.shotType] + gRacePlayers[0].state.shotCount - 1,
            gRacePlayers[0].state.shotPalette
        );
    } else {
        drawAssetTableSprite(
            -0x20,
            -0x60,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudShotTileOffsets[gRacePlayers[0].state.shotType] + gRacePlayers[0].state.shotCount - 1
        );
    }

    if (gRacePlayers[0].state.itemPalette != 0) {
        drawScaledAssetTableSprite(
            0,
            -0x60,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudItemTileIds[gRacePlayers[0].state.itemType],
            gRacePlayers[0].state.itemPalette
        );
    } else {
        drawAssetTableSprite(
            0,
            -0x60,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudItemTileIds[gRacePlayers[0].state.itemType]
        );
    }

    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x24);
    drawAssetTableSprite(-0x88, 0x40, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x29);
}

#if defined(VERSION_US)
const char gRaceHudTwoPlayerScoreFormat[] = "%5ld";

/* Matching-sensitive: separate stack objects provide buffer/end boundaries.
 * Portable code should use explicit storage and a five-character draw bound. */
#endif
void drawTwoPlayerRaceHud(void *arg0) {
    volatile u8 padding[0x18];
    s32 y;
    s32 x;
    s32 color;
#if defined(VERSION_JP)
    s32 i;
#else
    char *ptr;
    volatile char filler1;
    volatile char filler2;
    char end;
#endif
    char buffer[5];

    if (gCurrentViewportIndex == 0) {
        y = -0x28;
    } else {
        y = 0x1A;
    }

    sprintf(buffer, gRaceHudTwoPlayerScoreFormat, gRacePlayers[gCurrentViewportIndex].state.score);
    x = 0x50;
#if defined(VERSION_US)
    ptr = buffer;
#endif
    if (gRacePlayers[gCurrentViewportIndex].state.score < 0x64) {
        color = 0x10;
    } else {
        color = 0xE;
    }

#if defined(VERSION_JP)
    for (i = 0; i < 5; i++, x += 8) {
        if (buffer[i] != ' ') {
#else
    do {
        if (*ptr != ' ') {
#endif
            drawAssetTableSpriteWithExplicitPalette(
                x,
                y,
                getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
#if defined(VERSION_JP)
                buffer[i] - 5,
#else
                *ptr - 5,
#endif
                color
            );
        }
#if defined(VERSION_JP)
    }
#else
        ptr++;
        x += 8;
    } while (ptr != &end);
#endif

    drawAssetTableSprite(
        0x78,
        y,
        getRelocatableHeapBlockBase(RACE_HUD_MAIN_FONT_HANDLE),
        (gRaceHudCoinSpinnerFrame >> 1) + 4
    );

    if (gRacePlayers[gCurrentViewportIndex].state.shotPalette != 0) {
        drawScaledAssetTableSprite(
            -0x88,
            -0x30,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudShotTileOffsets[gRacePlayers[gCurrentViewportIndex].state.shotType] +
                gRacePlayers[gCurrentViewportIndex].state.shotCount - 1,
            gRacePlayers[gCurrentViewportIndex].state.shotPalette
        );
    } else {
        drawAssetTableSprite(
            -0x88,
            -0x30,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudShotTileOffsets[gRacePlayers[gCurrentViewportIndex].state.shotType] +
                gRacePlayers[gCurrentViewportIndex].state.shotCount - 1
        );
    }

    if (gRacePlayers[gCurrentViewportIndex].state.itemPalette != 0) {
        drawScaledAssetTableSprite(
            -0x68,
            -0x30,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudItemTileIds[gRacePlayers[gCurrentViewportIndex].state.itemType],
            gRacePlayers[gCurrentViewportIndex].state.itemPalette
        );
    } else {
        drawAssetTableSprite(
            -0x68,
            -0x30,
            getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
            gRaceHudItemTileIds[gRacePlayers[gCurrentViewportIndex].state.itemType]
        );
    }

    drawAssetTableSprite(
        -0x88,
        0x12,
        getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
        gRacePlayers[gCurrentViewportIndex].state.rankIndex
    );

    if (gCurrentViewportIndex == 0) {
        y = -0x30;
    } else {
        y = 0x2A;
    }
#if defined(VERSION_US)
    if (buffer) {}
#endif
    drawAssetTableSprite(0x58, y, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x1A);
}

void drawTwoPlayerLapCounter(void *arg0) {
    s32 y;
    s32 viewportIndex;

    viewportIndex = gCurrentViewportIndex;
    if (viewportIndex == 0) {
        y = -0x30;
    } else {
        y = 0x2A;
    }

    drawMenuAsciiChar(0x70, (s16)y, gRacePlayers[viewportIndex].state.lapDigit + '1', 2);
    drawMenuAsciiChar(0x78, (s16)y, '/', 2);
    drawMenuAsciiChar(0x80, (s16)y, gRaceLapCount + '0', 2);
}

void drawMultiplayerRaceHud(void *arg0) {
    RacePlayer *player;
    AssetTable *texture;

    drawScaledAssetTableSprite(
        0x38,
        0x24,
        getRelocatableHeapBlockBase(RACE_HUD_MAIN_FONT_HANDLE),
        (gRaceHudCoinSpinnerFrame >> 1) + 4,
        1
    );

    texture = getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE);
    player = &gRacePlayers[gCurrentViewportIndex];
    drawScaledAssetTableSprite(
        -0x18,
        -0x38,
        texture,
        gRaceHudShotTileOffsets[gRacePlayers[gCurrentViewportIndex].state.shotType] +
            gRacePlayers[gCurrentViewportIndex].state.shotCount - 1,
        player->state.shotPalette + 1
    );

    texture = getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE);
    player = &gRacePlayers[gCurrentViewportIndex];
    drawScaledAssetTableSprite(-8, -0x38, texture, gRaceHudItemTileIds[player->state.itemType], player->state.itemPalette + 1);

    texture = getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE);
    drawScaledAssetTableSprite(-0x4C, 0x18, texture, gRacePlayers[gCurrentViewportIndex].state.rankIndex, 1);

    if (gCurrentViewportIndex < 2) {
        drawAssetTableSprite(-0x44, -0x30, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x1A);
        return;
    }
    drawAssetTableSprite(0x14, -0x30, getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE), 0x1A);
}
#if defined(VERSION_US)

const char gRaceHudMultiplayerScoreFormat[] = "%5ld";
#endif

void drawMultiplayerScoreAndLapCounter(void *arg0) {
    s32 x;
    s32 palette;
    char buffer[0x20];

    sprintf(buffer, gRaceHudMultiplayerScoreFormat, gRacePlayers[gCurrentViewportIndex].state.score);
    if (gRacePlayers[gCurrentViewportIndex].state.score < 0x64) {
        palette = 1;
    } else {
        palette = 2;
    }
    drawMenuAsciiTextDefaultScale(0x14, 0x28, buffer, palette);

    if (gCurrentViewportIndex < 2) {
        x = -0x2C;
    } else {
        x = 0x2C;
    }

    drawMenuAsciiChar((s16)x, -0x30, gRacePlayers[gCurrentViewportIndex].state.lapDigit + '1', 2);
    drawMenuAsciiChar((s16)(x + 8), -0x30, 0x2F, 2);
    drawMenuAsciiChar((s16)(x + 0x10), -0x30, gRaceLapCount + 0x30, 2);
}

void drawThreePlayerNoEntryPanel(void *arg0) {
    drawAssetTableSprite(0xC, 0x2C, getRelocatableHeapBlockBase(ASSET_HANDLE(0x1F)), 0x90);
}

void incrementRaceElapsedTimer(void) {
    if (gRaceUpdatePaused == 0) {
        gRaceElapsedTimer.fraction = gRaceElapsedTimer.fraction + 0x358;
        if (gRaceElapsedTimer.fraction >= 0x6401) {
            gRaceElapsedTimer.fraction = 0;
            gRaceElapsedTimer.seconds = gRaceElapsedTimer.seconds + 1;
            if (gRaceElapsedTimer.seconds >= 0x3C) {
                gRaceElapsedTimer.seconds = 0;
                gRaceElapsedTimer.minutes = gRaceElapsedTimer.minutes + 1;
                if (gRaceElapsedTimer.minutes >= 0x63) {
                    gRaceElapsedTimer.fraction = 0x6300;
                    gRaceElapsedTimer.seconds = 0x3B;
                    gRaceElapsedTimer.minutes = 0x63;
                }
            }
        }
    }
}

void decrementRaceChallengeTimeLimit(void) {
    if (gRaceUpdatePaused == 0) {
        gRaceChallengeTimeLimit.fraction = gRaceChallengeTimeLimit.fraction - 0x358;
        if (gRaceChallengeTimeLimit.fraction < 0) {
            gRaceChallengeTimeLimit.fraction = 0x60F8;
            gRaceChallengeTimeLimit.seconds = gRaceChallengeTimeLimit.seconds - 1;
            if (gRaceChallengeTimeLimit.seconds < 0) {
                gRaceChallengeTimeLimit.seconds = 0x3B;
                gRaceChallengeTimeLimit.minutes = gRaceChallengeTimeLimit.minutes - 1;
                if (gRaceChallengeTimeLimit.minutes < 0) {
                    gRaceChallengeTimeLimit.fraction = 0;
                    gRaceChallengeTimeLimit.seconds = 0;
                    gRaceChallengeTimeLimit.minutes = 0;
                }
            }
        }
    }
}

void drawRaceCourseProgressMeter(void *arg0) {
    s32 i;
    s32 j;
    union {
        s32 playerId;
        AssetTable *assetTable;
    } temp;
    s32 order[4];
    s16 xBase;
    s16 yBase;

    order[0] = 0;
    order[1] = 1;
    order[2] = 2;
    order[3] = 3;

    for (i = 0; i < 3; i++) {
        for (j = i + 1; j < 4; j++) {
            if (gRacePlayers[order[j]].state.rankIndex < gRacePlayers[order[i]].state.rankIndex) {
                temp.playerId = order[i];
                order[i] = order[j];
                order[j] = temp.playerId;
            }
        }
    }

    if (gRaceHudMode == RACE_HUD_MODE_ONE_PLAYER) {
        xBase = 0x78;
        yBase = -0x56;
    }
    if (gRaceHudMode == RACE_HUD_MODE_TWO_PLAYER) {
        xBase = 0x78;
        yBase = -0x48;
    }
    if ((gRaceHudMode == RACE_HUD_MODE_THREE_PLAYER) || (gRaceHudMode == RACE_HUD_MODE_FOUR_PLAYER)) {
        xBase = -8;
        yBase = -0x48;
    }

    drawAssetTableSprite(
        (s16)(xBase + 4),
        (s16)(yBase + 4),
        getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE),
        0x50
    );

    i = 3;
#if defined(VERSION_JP)
    while (i >= 0) {
#else
    do {
#endif
        if (((gRacePlayers[order[i]].state.stateFlags & RACE_PLAYER_COLLISION_SQUASHED) != 0) ||
            (gRacePlayers[order[i]].state.progressMeterSquashFrame != 0)) {
            gRacePlayers[order[i]].state.progressMeterSquashFrame++;
        }
        if (((gRacePlayers[order[i]].state.stateFlags & RACE_PLAYER_COLLISION_SQUASHED) != 0) &&
            (gRacePlayers[order[i]].state.progressMeterSquashFrame >= 5)) {
            gRacePlayers[order[i]].state.progressMeterSquashFrame = 4;
        }
        if (gRacePlayers[order[i]].state.progressMeterSquashFrame >= 6) {
            gRacePlayers[order[i]].state.progressMeterSquashFrame = 0;
        }

        if (gRacePlayers[order[i]].state.progressMeterSquashFrame != 0) {
            if (gRacePlayers[order[i]].state.activeSparkleEffectCount != 0) {
                temp.assetTable = getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE);
                drawAssetTableSpriteWithExplicitPalette(
                    (s16)(xBase - 8),
                    (s16)(gRacePlayers[order[i]].state.progressMeterPosition + yBase),
                    temp.assetTable,
                    (
                        &gRaceProgressMeterIconTiles[gRacePlayers[order[i]].state.progressMeterSquashFrame]
                    )[gRacePlayers[order[i]].characterId * 6],
                    gRaceProgressMeterIconPalettes[gRacePlayers[order[i]].characterId]
                );
            } else {
                temp.assetTable = getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE);
                drawAssetTableSprite(
                    (s16)(xBase - 8),
                    (s16)(gRacePlayers[order[i]].state.progressMeterPosition + yBase),
                    temp.assetTable,
                    (
                        &gRaceProgressMeterIconTiles[gRacePlayers[order[i]].state.progressMeterSquashFrame]
                    )[gRacePlayers[order[i]].characterId * 6]
                );
            }
        } else if (gRacePlayers[order[i]].state.activeSparkleEffectCount != 0) {
            temp.assetTable = getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE);
            drawAssetTableSpriteWithExplicitPalette(
                xBase,
                (s16)(gRacePlayers[order[i]].state.progressMeterPosition + yBase),
                temp.assetTable,
                (
                    &gRaceProgressMeterIconTiles[gRacePlayers[order[i]].state.progressMeterSquashFrame]
                )[gRacePlayers[order[i]].characterId * 6],
                gRaceProgressMeterIconPalettes[gRacePlayers[order[i]].characterId]
            );
        } else {
            temp.assetTable = getRelocatableHeapBlockBase(RACE_HUD_POPUP_FONT_HANDLE);
            drawAssetTableSprite(
                xBase,
                (s16)(gRacePlayers[order[i]].state.progressMeterPosition + yBase),
                temp.assetTable,
                (
                    &gRaceProgressMeterIconTiles[gRacePlayers[order[i]].state.progressMeterSquashFrame]
                )[gRacePlayers[order[i]].characterId * 6]
            );
        }
        i--;
#if defined(VERSION_JP)
    }
#else
    } while (i >= 0);
#endif
}

void updateRaceCourseProgressMeter(void) {
    s32 meterPosition;
    s32 slots[2];
    s32 i;

    for (i = 0; i < 4; i++) {
        getRacePlayerRankingProgress(i, &slots[1], &slots[0]);
        if (slots[1] < 0) {
            slots[1] = 0;
        }
        gRacePlayers[i].state.progressMeterPosition =
            (slots[1] << 7) / (gRaceCourseStartEntries[gRaceCourseIndex.signedValue].pathIndex * 8);
        if (gRacePlayers[i].state.progressMeterPosition >= 0x81) {
            gRacePlayers[i].state.progressMeterPosition = 0x80;
        }
    }

    if (gRaceHudMode == RACE_HUD_MODE_ONE_PLAYER) {
        addRenderCallback(&gRaceOverlayRenderCallbackList, drawRaceCourseProgressMeter, 0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawRaceCourseProgressMeter, 0);
}

void updateRaceHud(void) {
    s32 i;

    gRaceHudCoinSpinnerFrame++;
    if (gRaceHudCoinSpinnerFrame >= 12) {
        gRaceHudCoinSpinnerFrame = 0;
    }

    switch (*(u16 *)&gRaceHudMode) {
        case RACE_HUD_MODE_ONE_PLAYER:
            if (gRacePlayers[0].state.shotPalette != 0) {
                if (gRacePlayers[0].state.shotPalette == 4) {
                    spawnRaceUiSparkle(-0x20, -0x60, 0, 0, 0);
                }
                gRacePlayers[0].state.shotPalette--;
            }
            if (gRacePlayers[0].state.itemPalette != 0) {
                if (gRacePlayers[0].state.itemPalette == 4) {
                    spawnRaceUiSparkle(0, -0x60, 0, 0, 1);
                }
                gRacePlayers[0].state.itemPalette--;
            }
            addRenderCallback(&gRaceOverlayRenderCallbackList, drawSinglePlayerRaceHud, 0);
            addRenderCallback(&gRaceForegroundRenderCallbackList, noopRaceHudCallback, 0);
            updateRaceCourseProgressMeter();
            return;
        case RACE_HUD_MODE_TWO_PLAYER:
            for (i = 0; i < 2; i++) {
                if (gRacePlayers[i].state.shotPalette != 0) {
                    if (gRacePlayers[i].state.shotPalette == 4) {
                        spawnRaceUiSparkle(-0x88, -0x30, (s16)i, 0, 0);
                    }
                    gRacePlayers[i].state.shotPalette--;
                }
                if (gRacePlayers[i].state.itemPalette != 0) {
                    if (gRacePlayers[i].state.itemPalette == 4) {
                        spawnRaceUiSparkle(-0x68, -0x30, (s16)i, 0, 1);
                    }
                    gRacePlayers[i].state.itemPalette--;
                }
            }
            addRenderCallback(&gRaceOverlayRenderCallbackList, drawTwoPlayerRaceHud, 0);
            addRenderCallback(&gRaceForegroundRenderCallbackList, drawTwoPlayerLapCounter, 0);
            updateRaceCourseProgressMeter();
            return;
        case RACE_HUD_MODE_FOUR_PLAYER:
            if (gRacePlayers[3].state.shotPalette != 0) {
                if (gRacePlayers[3].state.shotPalette == 4) {
                    spawnRaceUiSparkle(-0x10, -0x30, 3, 1, 0);
                }
                gRacePlayers[3].state.shotPalette--;
            }
            if (gRacePlayers[3].state.itemPalette != 0) {
                if (gRacePlayers[3].state.itemPalette == 4) {
                    spawnRaceUiSparkle(0, -0x30, 3, 1, 1);
                }
                gRacePlayers[3].state.itemPalette--;
            }
        case RACE_HUD_MODE_THREE_PLAYER:
            for (i = 0; i < 3; i++) {
                if (gRacePlayers[i].state.shotPalette != 0) {
                    if (gRacePlayers[i].state.shotPalette == 4) {
                        spawnRaceUiSparkle(-0x10, -0x30, (s16)i, 1, 0);
                    }
                    gRacePlayers[i].state.shotPalette--;
                }
                if (gRacePlayers[i].state.itemPalette != 0) {
                    if (gRacePlayers[i].state.itemPalette == 4) {
                        spawnRaceUiSparkle(0, -0x30, (s16)i, 1, 1);
                    }
                    gRacePlayers[i].state.itemPalette--;
                }
            }
            addRenderCallback(&gRaceOverlayRenderCallbackList, drawMultiplayerRaceHud, 0);
            addRenderCallback(&gRaceForegroundRenderCallbackList, drawMultiplayerScoreAndLapCounter, 0);
            updateRaceCourseProgressMeter();
            if ((s16)gRaceHudMode == RACE_HUD_MODE_THREE_PLAYER) {
                addRenderCallback(&gMenuRenderCallbackList, drawThreePlayerNoEntryPanel, 0);
            }
            return;
        case RACE_HUD_MODE_TIME_TRIAL: {
            s32 sp40;
            s32 sp3C;

            if (!(gMenuFlowState & 3)) {
                incrementRaceElapsedTimer();
                if (gRaceTimeTrialFinishRecorded == 0) {
                    getRacePlayerRankingProgress(0, &sp40, &sp3C);
                    if ((gRaceCourseStartEntries[gRaceCourseIndex.signedValue].finishLinePathIndex * 8) < sp40) {
                        gRaceTimeTrialFinishTime = gRaceElapsedTimer;
                        gRaceTimeTrialFinishRecorded = 1;
                        createCallbackTask((CallbackTaskCallback)initTimeTrialRecordDeltaPopup, 0, 0);
                    }
                }
            }
            addRenderCallback(&gRaceOverlayRenderCallbackList, drawTimeTrialHud, 0);
            addRenderCallback(&gRaceForegroundRenderCallbackList, drawTimeTrialLabels, 0);
            return;
        }
        case RACE_HUD_MODE_TARGET_TIME_CHALLENGE: {
            RaceTimer sp38;

            if (!(gMenuFlowState & 3)) {
                incrementRaceElapsedTimer();
            }
            if (calculateRaceTimerDelta(
                    &gRaceCourseTargetTimes[gRaceCourseIndex.signedValue],
                    &gRaceElapsedTimer,
                    &sp38
                ) != 0) {
                gRaceChallengeFailed = 1;
            }
            addRenderCallback(&gRaceOverlayRenderCallbackList, drawTargetTimeChallengeHud, 0);
            addRenderCallback(&gRaceForegroundRenderCallbackList, drawTargetTimeChallengeLabels, 0);
            return;
        }
        case RACE_HUD_MODE_SCORE_ATTACK:
            if (!(gMenuFlowState & 3)) {
                decrementRaceChallengeTimeLimit();
            }
            if ((gRaceChallengeTimeLimit.minutes == 0) && (gRaceChallengeTimeLimit.seconds == 0) &&
                (gRaceChallengeTimeLimit.fraction == 0)) {
                gRaceChallengeFailed = 1;
            }
            addRenderCallback(&gRaceOverlayRenderCallbackList, drawScoreAttackChallengeHud, 0);
            addRenderCallback(&gRaceForegroundRenderCallbackList, drawScoreAttackChallengeLabels, 0);
            return;
        case RACE_HUD_MODE_TRICK_ATTACK:
            if (!(gMenuFlowState & 3)) {
                decrementRaceChallengeTimeLimit();
            }
            if ((gRaceChallengeTimeLimit.minutes == 0) && (gRaceChallengeTimeLimit.seconds == 0) &&
                (gRaceChallengeTimeLimit.fraction == 0)) {
                gRaceChallengeFailed = 1;
            }
            addRenderCallback(&gRaceOverlayRenderCallbackList, drawTrickAttackChallengeHud, 0);
            addRenderCallback(&gRaceForegroundRenderCallbackList, drawTrickAttackChallengeLabels, 0);
            return;
        case RACE_HUD_MODE_TRAINING_OR_DEMO:
            if (gRacePlayers[0].state.shotPalette != 0) {
                if (gRacePlayers[0].state.shotPalette == 4) {
                    spawnRaceUiSparkle(-0x20, -0x60, 0, 0, 0);
                }
                gRacePlayers[0].state.shotPalette--;
            }
            if (gRacePlayers[0].state.itemPalette != 0) {
                if (gRacePlayers[0].state.itemPalette == 4) {
                    spawnRaceUiSparkle(0, -0x60, 0, 0, 1);
                }
                gRacePlayers[0].state.itemPalette--;
            }
            addRenderCallback(&gRaceOverlayRenderCallbackList, drawTrainingRaceHud, 0);
            return;
    }
}

s32 calculateRaceTimerDelta(RaceTimer *arg0, RaceTimer *arg1, RaceTimer *arg2) {
    s32 total1;
    s32 diff;

    diff = (arg0->fraction & 0xFF00) + (arg0->seconds * 0x6400) + (arg0->minutes * 0x177000);
    total1 = (arg1->fraction & 0xFF00) + (arg1->seconds * 0x6400) + (arg1->minutes * 0x177000);
    arg0 = (RaceTimer *)1;
    if (diff >= total1) {
        arg0 = (RaceTimer *)0;
        diff -= total1;
    } else {
        diff = total1 - diff;
    }
    arg2->fraction = diff % 0x6400;
    diff /= 0x6400;
    arg1 = (RaceTimer *)60;
    arg2->seconds = diff % (s32)arg1;
    diff /= (s32)arg1;
    arg2->minutes = diff % 99;
    return (s32)arg0;
}
