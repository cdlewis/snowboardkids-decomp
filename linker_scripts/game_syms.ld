/*
 * Project-local aliases for symbols still referenced by extracted assembly.
 *
 * Prefer aliases to canonical splat-generated asset symbols so that renamed
 * C references and legacy assembly references resolve to the same address.
 */

/* Shared race-model overlay addresses. */
D_2000000 = _147910_VRAM;
D_20006C8 = _147910_VRAM + 0x6C8;
D_2000910 = _148220_VRAM;
D_2000D78 = _147910_VRAM + 0xD78;
D_2000E48 = _147910_VRAM + 0xE48;
D_2000E70 = _147910_VRAM + 0xE70;
D_2001678 = _148F88_VRAM;
D_2001730 = _149040_VRAM;
D_2001810 = _149120_VRAM;
D_20018E8 = _1491F8_VRAM;
D_20019C0 = _ADDR_20019C0_VRAM;
D_2001D00 = _149610_VRAM;
gSnowboardTrailFrontDisplayList = _147910_VRAM + 0x2208;
gSnowboardTrailBackDisplayList = _147910_VRAM + 0x23A8;
D_2002490 = _ADDR_2002490_VRAM;
D_2002660 = _ADDR_2002660_VRAM;
D_20028F0 = _14A200_VRAM;
D_2002DB8 = _14A200_VRAM + 0x4C8;
gRaceItemPickupDisplayList = _147910_SUFFIX_VRAM;
gRaceActionPickupDisplayList = _147910_SUFFIX_VRAM + 0x80;

/* Rookie Mountain overlay address. */
D_20058A8 = ROOKIE_MOUNTAIN_COURSE_BACKDROP_DISPLAY_LIST_VRAM;

/* Fixed physical/virtual addresses encoded in original data. */
D_369000 = 0x369000;
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
