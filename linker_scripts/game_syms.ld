/*
 * Project-local aliases for symbols still referenced by extracted assembly.
 *
 * Prefer aliases to canonical splat-generated asset symbols so that renamed
 * C references and legacy assembly references resolve to the same address.
 */

/* Award podium segment 2: initPassAwardCelebration loads _145380 into handle 8. */
gPassAwardPodiumVariant0Dl = _145380_command_000000;
gPassAwardPodiumVariant1Dl = _145380_command_0006C8;
gPassAwardPodiumVariant2Dl = _145380_command_000D78;
/* Menu boards use the same handle and segment with a different bank. */
gMainMenuRotatingBoardDl = _1467B0_command_000E48;
gMainMenuStaticBoardDl = _1467B0_command_000E70;

/* Shared race-model segment 2: handle 0xA. */
D_2000910 = _148220_command_000000;
D_2001678 = _148F88_command_000000;
D_2001730 = _149040_command_000000;
D_2001810 = _149120_command_000000;
D_20018E8 = _1491F8_command_000000;
D_20019C0 = _ADDR_20019C0_command_000000;
D_2001D00 = _149610_command_000000;
gSpeedFanFrontDisplayList = _1499B8_command_000160;
gSpeedFanBackDisplayList = _1499B8_command_000300;
D_2002490 = _ADDR_2002490_command_000000;
D_2002660 = PAN_DISPLAY_LIST_command_000000;
D_20028F0 = _14A200_command_000000;
D_2002DB8 = _14A200_command_0004C8;
gRaceItemPickupDisplayList = SHARED_RACE_GRAPHICS_SUFFIX_command_000000;
gRaceActionPickupDisplayList = SHARED_RACE_GRAPHICS_SUFFIX_command_000080;

/* Rookie Mountain overlay address. */
gRookieMountainBackdropDl = ROOKIE_MOUNTAIN_COURSE_BACKDROP_DISPLAY_LIST_command_000000;

/* Fixed physical/virtual addresses encoded in original data. */
/* Physical GPU alias of gDepthBuffer (0x80369000), not an image asset. */
gDepthBufferPhysicalAddress = gDepthBuffer - 0x80000000;
D_80000000 = 0x80000000;

/* Typed slot-zero subobject views used where IDO must retain distinct relocations. */
gPrimaryCourseUnlockStates = gGameSaveDataBuffer + 0x3F;
gCourseUnlockScanStart = gGameSaveDataBuffer + 0x1;
gCourseUnlockScanEnd = gGameSaveDataBuffer + 0x9;
gPrimaryGameSaveRawData = gGameSaveDataBuffer;

/* Character-select course-menu data subobjects. */
gCharacterSelectCourseOptionsByUnlock = gCharacterSelectCourseMenuData;
gPlayerBadgeDisplayOrder = gCharacterSelectCourseMenuData + 0x42;
gCharacterSelectShortCourseOptions = gCharacterSelectCourseMenuData + 0x58;
gCharacterSelectSingleCourseOption = gCharacterSelectCourseMenuData + 0x60;

/* Race-course effect data subobject used by extracted collision assembly. */
D_800DA856 = gCourseTriggerEntries + 0x16;

/* Race-player ground probe subobjects used by extracted movement assembly. */
D_800DE7F8 = gRacePlayerGroundProbeOffsets + 0x48;
D_800DE7FC = gRacePlayerGroundProbeOffsets + 0x4C;
D_800DE814 = gRacePlayerGroundProbeOffsets + 0x64;
D_800DE818 = gRacePlayerGroundProbeOffsets + 0x68;

/* HUD status is the initialized byte of each 0xB0-byte camera. */
gRacePlayerHudStatuses = gRaceCameras + 0xAC;

/* Keep camera end sentinels opaque to IDO loop-trip-count folding. */
gCourseSelectCameraUpdateEnd = gRaceCameras + 0x160;
gRaceCamerasEnd = gRaceCameras + 0x2C0;

/* Eight 0x0C slots; existing queue names alias their head words. */
gModelRenderCallbackQueues = gModelRenderCallbackList;

/* ORI uses an unsigned low half: do not use a carry-adjusted HI16 here. */
main_BSS_CLEAR_HI = main_BSS_SIZE >> 16;
main_BSS_CLEAR_LO = main_BSS_SIZE & 0xFFFF;
ASSERT(main_BSS_SIZE > 0 && (main_BSS_SIZE & 7) == 0, "boot BSS clear requires nonempty 8-byte units")
ASSERT(buffers_BSS_START >= main_BSS_END, "runtime buffers overlap main BSS")
ASSERT(buffers_BSS_END <= 0x80400000, "runtime layout exceeds original 4 MB RAM")
ASSERT((buffers_BSS_START & 15) == 0, "runtime buffers require 16-byte alignment")
ASSERT((gDepthBuffer & 63) == 0, "depth buffer requires 64-byte alignment")
ASSERT((gFramebuffers & 63) == 0, "framebuffers require 64-byte alignment")
ASSERT(entry_ROM_START == 0x1000 && entry_VRAM == 0x80000400, "hardware boot placement changed")
/* Original IPL3 copies one MB from ROM 0x1000 to the header entry address. */
ASSERT(main_VRAM - main_ROM_START == entry_VRAM - entry_ROM_START, "main violates IPL3 ROM-to-RAM mapping")
ASSERT(main_ROM_END <= entry_ROM_START + 0x100000, "main exceeds IPL3 1 MB boot transfer")

/* Typed views of the packed menu-effect data consumed by race intros. */
gRaceIntroCourseVertices = passAwardLabelFlashTileOffsets + 0x24;
gRaceIntroAlternateCourseVertices = passAwardLabelFlashTileOffsets + 0x1A4;
gRaceIntroCourseCommands = passAwardLabelFlashTileOffsets + 0x264;
gRaceIntroAlternateCourseCommands = passAwardLabelFlashTileOffsets + 0x3E4;

/* Overlapping callback scheduler views preserve the original IDO code generation. */
gCallbackTaskActiveListSentinel = gCallbackTaskSchedulerBssView + 0x10;
gCallbackTaskActiveListHead = gCallbackTaskSchedulerBssView + 0x14;
gCallbackTaskPoolStartView0 = gCallbackTaskPool;
gCallbackTaskPoolStartView1 = gCallbackTaskPool + 0x118;
gCallbackTaskPoolStartView2 = gCallbackTaskPool + 0x230;
gCallbackTaskPoolStartView3 = gCallbackTaskPool + 0x348;
/* The final group cursor stops six pointer slots before the free stack ends. */
gCallbackTaskPoolEnd = gFreeCallbackTaskPool + 0x348;

/* Player sound handle views share the four-entry positional handle array. */
gPlayerPositionalSoundHandle1 = gPlayerPositionalSoundHandle0 + 4;
gPlayerPositionalSoundHandle2 = gPlayerPositionalSoundHandle0 + 8;
gPlayerPositionalSoundHandle3 = gPlayerPositionalSoundHandle0 + 12;

/* The collision effect angle is the low half of the hit angle word. */
gRacePlayerHitEffectAngle = gRacePlayerHitAngle + 2;

/* Direct controller access aliases the four-player input arrays. */
gPlayer2InputHeld = gPlayerInputHeld + 4;
gPlayer3InputHeld = gPlayerInputHeld + 8;
gPlayer4InputHeld = gPlayerInputHeld + 12;
gPlayer2InputPrevious = gPlayerInputPrevious + 4;
gPlayer3InputPrevious = gPlayerInputPrevious + 8;
gPlayer4InputPrevious = gPlayerInputPrevious + 12;
gPlayer2InputPressed = gPlayerInputPressed + 4;
gPlayer3InputPressed = gPlayerInputPressed + 8;
gPlayer4InputPressed = gPlayerInputPressed + 12;
gPlayer2StickX = gPlayerStickX + 1;
gPlayer3StickX = gPlayerStickX + 2;
gPlayer4StickX = gPlayerStickX + 3;
gPlayer2StickY = gPlayerStickY + 1;
gPlayer3StickY = gPlayerStickY + 2;
gPlayer4StickY = gPlayerStickY + 3;

/* Menu callback list names expose the head words of six four-word slots. */
gMenuOverlayRenderCallbackList = gMenuRenderCallbackQueueStorage + 0x0;
D_80124848 = gMenuRenderCallbackQueueStorage + 0x10;
gMenuForegroundRenderCallbackList = gMenuRenderCallbackQueueStorage + 0x20;
gMenuRenderCallbackList = gMenuRenderCallbackQueueStorage + 0x30;
gRaceOverlayRenderCallbackList = gMenuRenderCallbackQueueStorage + 0x40;
gRaceForegroundRenderCallbackList = gMenuRenderCallbackQueueStorage + 0x50;
gBackdropRenderCallbackList = gBackdropRenderCallbackQueueStorage;

/* Model callback list names expose the head words of the eight queue slots. */
gModelRenderCallbackList = gModelRenderCallbackQueueStorage + 0x0;
gRaceObjectRenderCallbackList = gModelRenderCallbackQueueStorage + 0xC;
gSceneModelRenderCallbackList = gModelRenderCallbackQueueStorage + 0x18;
gRaceModelEffectRenderCallbackList = gModelRenderCallbackQueueStorage + 0x24;
gModelRenderCallbackQueue4Head = gModelRenderCallbackQueueStorage + 0x30;
gEffectRenderCallbackList = gModelRenderCallbackQueueStorage + 0x3C;
gModelRenderCallbackQueue6Head = gModelRenderCallbackQueueStorage + 0x48;
gModelRenderCallbackQueue7Head = gModelRenderCallbackQueueStorage + 0x54;

/* Preserve the separate status-byte anchor used by IDO. */
gFrameRenderTaskStatuses = gFrameRenderTasks + 0x66;

/* Legacy audio views share typed heap and message queue storage. */
gSoundPlayerHeapEnd = gSoundPlayerHeap + 4;
gAudioTaskDoneQueue = gAudioTaskDoneQueueStorage;
