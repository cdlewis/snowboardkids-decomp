#ifndef GAME_MENU_MAIN_MENU_CONTROLLER_SUBSYSTEM_H
#define GAME_MENU_MAIN_MENU_CONTROLLER_SUBSYSTEM_H

#include "common.h"
#include "PR/os_thread.h"
#include "PR/os_pfs.h"
#include "PR/os_message.h"
#include "PR/os_cont.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"

extern u8 gConnectedControllerCount;
extern s16 gControllerEventMessage;
extern OSMesgQueue gControllerEventQueue;
extern OSContPad gControllerPads[];
extern s32 gControllerPakFileNos[];
extern OSPfs gControllerPakHandles[];
extern SaveFileIdentity gControllerPakSaveFileIdentity;
extern u8 gControllerReadPending;
extern OSMesgQueue gControllerSubsystemReplyQueue;
extern OSMesgQueue gControllerSubsystemRequestQueue;
extern OSPfs gRumblePakHandles[];

extern OSMesg gControllerEventMessages[];
extern OSContStatus gControllerStatuses[];
extern OSMesg gControllerSubsystemReplyMessages[];
extern OSMesg gControllerSubsystemRequestMessages[];
extern OSThread gControllerSubsystemThread;
extern u8 gControllerSubsystemThreadStack[0x2000];

#endif
