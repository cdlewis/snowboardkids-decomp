/* N64 OS variables */

osTvType = 0x80000300;
osRomBase = 0x80000308;
osResetType = 0x8000030C;
osAppNMIBuffer = 0x8000031C;

gRaceSplitscreenSelectCursorAnimState = gRaceSplitscreenSelectCursorTarget;
D_8010AE51 = gCharacterSelectHudState + 0x1;
D_8010AE52 = gCharacterSelectHudState + 0x2;
D_8010AE52_state = gCharacterSelectHudState + 0x2;
D_8010AE58 = gCharacterSelectHudState + 0x8;
D_8010AE5E = gCharacterSelectHudState + 0xE;
D_8010AE5F = gCharacterSelectHudState + 0xF;
D_8010AE64 = gCharacterSelectHudState + 0x14;
gCharacterSelectCourseExitOptionIndex = gCharacterSelectCourseBss;
gCharacterSelectCourseCursorState = gCharacterSelectCourseBss + 0x8;
gCharacterSelectCourseCursorStateByte = gCharacterSelectCourseCursorState;
gCharacterSelectCourseSubmenuState = gCharacterSelectCourseCursorState + 0x1;
D_8010AE8F = gCharacterSelectCourseCursorState + 0x7;
gControllerPakRaceRecordSaveStatusTransitionStep = gControllerPakRaceRecordSaveStatusTransition;
gActiveGameTaskListView = gActiveGameTaskList;
