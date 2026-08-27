#ifndef CONTROLLER_MAIN_MENU_FLOW_H
#define CONTROLLER_MAIN_MENU_FLOW_H

#include "common.h"
#include "game/engine/controller_input.h"

#define CONTROLLER_REQUEST_TYPE_MASK 0xF0
#define CONTROLLER_REQUEST_CHANNEL_MASK 0x3
#define CONTROLLER_REQUEST_FILE_INDEX_MASK 0xF

#define RUMBLE_MOTOR_NO_PAK 1
#define RUMBLE_MOTOR_CONTROLLER_FAILURE 4
#define RUMBLE_MOTOR_WRONG_DEVICE 11

#define CONTROLLER_PAK_SAVE_NOTE_SIZE 0x7900
#define CONTROLLER_PAK_SAVE_READ_SIZE 0x78E0
#define CONTROLLER_PAK_CHECKSUM_START_OFFSET 4
#define CONTROLLER_PAK_MAX_READ_RETRIES 3
#define CONTROLLER_PAK_RETRY_LIMIT 3

typedef struct SaveFileIdentity {
    s32 size;
    u32 gameCode;
    u16 companyCode;
    char extName[4];
    char gameName[16];
} SaveFileIdentity;

typedef enum ControllerPakSaveFlowStatus {
    CONTROLLER_PAK_STATUS_PROBE = 0,
    CONTROLLER_PAK_STATUS_SAVE_STATUS = 1,
    CONTROLLER_PAK_STATUS_SAVE_READ = 2,
    CONTROLLER_PAK_STATUS_REPAIR = 3,
    CONTROLLER_PAK_STATUS_RETRY = 4,
    CONTROLLER_PAK_STATUS_READY = 5,
    CONTROLLER_PAK_STATUS_CHOICE_PROMPT = 6,
    CONTROLLER_PAK_STATUS_NO_PAK = 7,
    CONTROLLER_PAK_STATUS_SAVE_FOUND = 8,
    CONTROLLER_PAK_STATUS_READ_FAILED = 0xD,
    CONTROLLER_PAK_STATUS_REPAIR_FAILED = 0xE,
    CONTROLLER_PAK_STATUS_REPAIRED = 0xF,
    CONTROLLER_PAK_STATUS_REPAIR_CONFIRM = 0x11,
    CONTROLLER_PAK_STATUS_USE_EXISTING_SAVE = 0x12,
    CONTROLLER_PAK_STATUS_MENU_COMPLETE = 0x13
} ControllerPakSaveFlowStatus;

typedef enum ControllerSubsystemRequest {
    CONTROLLER_REQUEST_READ_INPUT = 0x10,
    CONTROLLER_REQUEST_PROBE_PAK = 0x20,
    CONTROLLER_REQUEST_CHECK_SAVE = 0x30,
    CONTROLLER_REQUEST_READ_SAVE = 0x40,
    CONTROLLER_REQUEST_WRITE_SAVE = 0x50,
    CONTROLLER_REQUEST_REPAIR_PAK = 0x60,
    CONTROLLER_REQUEST_INIT_RUMBLE = 0x70,
    CONTROLLER_REQUEST_START_RUMBLE = 0x80,
    CONTROLLER_REQUEST_STOP_RUMBLE = 0x90,
    CONTROLLER_REQUEST_LIST_FILES = 0xA0,
    CONTROLLER_REQUEST_DELETE_FILE = 0xB0,
    CONTROLLER_REQUEST_UPDATE_FREE_SPACE = 0xC0,
    CONTROLLER_REQUEST_RETRY_RUMBLE_INIT = 0xD0
} ControllerSubsystemRequest;

extern s16 gControllerPakStatusCodes[4];
extern s16 gMenuChoicePromptState[4];
extern u16 gMenuInputRepeatTimers[4];
extern s32 gRumbleMotorStatuses[4];
extern s16 gRumbleMotorRequestStates[4];
extern u8 gRumblePakConnectedByController[PLAYER_INPUT_COUNT];
extern u8 gControllerPakRetryCounts[4];
/* 0: pak work active; 1: complete before panel creation; >=2: panel is interactive. */
extern u8 gControllerPakOperationCounts[4];
extern u8 gControllerPakSaveGameNameBytes[16];
extern u8 gControllerPakSaveExtNameBytes[4];
extern s32 gControllerPakFreeBytes;
extern s32 gControllerPakFreeFileCount;
extern u8 gMainMenuSecretCodeUnlocked;
extern u8 gMainMenuSelectionResult;
extern u8 gMenuSelectionConfirmTimer;

void initControllerSubsystem(void);
void controllerSubsystemThreadMain(void *threadArg);
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
u16 validateControllerPakSave(volatile s32 arg0);
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
