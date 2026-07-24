#ifndef CONTROLLER_MAIN_MENU_FLOW_H
#define CONTROLLER_MAIN_MENU_FLOW_H

#include "common.h"

void initControllerSubsystem(void);
void controllerSubsystemThreadMain(void *arg0);
void requestControllerRead(void);
void updateControllerInputState(void);
void requestRumbleMotorInit(u16 arg0);
void serviceRumbleMotorRequest(u16 arg0);
void requestRumbleMotorStart(u16 arg0);
void requestControllerPakProbe(u16 arg0);
void probeControllerPak(u16 arg0);
void requestControllerPakSaveStatus(u16 arg0);
void checkControllerPakSaveStatus(u16 arg0);
void requestControllerPakSaveRead(u16 arg0);
void readControllerPakSave(u16 arg0);
void requestControllerPakSaveWrite(u16 arg0);
void writeControllerPakSave(u16 arg0);
void requestControllerPakRepair(u16 arg0);
void repairControllerPakId(u16 arg0);
void requestControllerPakFileList(void);
void readControllerPakFileStates(void);
void requestControllerPakDeleteFile(u16 arg0);
void deleteControllerPakFile(u16 arg0);
void requestControllerPakFreeSpaceUpdate(void);
void updateControllerPakFreeSpaceInfo(void);
u16 validateControllerPakSave(s32 arg0);
s32 validateControllerPakSaveData(s32 arg0);
void enterMainMenuFromRace(void);
void initMainMenu(void);
void updateMainMenu(void);
void fadeOutMainMenu(void);
void initMainMenuModeSelect(void);
void updateMainMenuModeSelect(void);
void fadeOutMainMenuModeSelect(void);
void exitMainMenuModeSelect(void);
void initMainMenuSettings(void);
void updateMainMenuSettings(void);
void fadeOutMainMenuSettings(void);
void exitMainMenuSettings(void);
s32 checkMainMenuSecretCode(void);

#endif
