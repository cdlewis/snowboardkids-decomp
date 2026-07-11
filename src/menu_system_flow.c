#include "common.h"
#include "sound_manager.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "system_boot.h"
#include "game_task_scheduler.h"
#include "menu_system_flow.h"
#include "main_menu_visual_effects.h"
#include "main_menu_panel_ui.h"
#include "main_menu_scene_model.h"
#include "main_menu_scene_model_renderer.h"
#include "main_menu_mode_race_flow.h"
#include "main_menu_ui.h"
#include "race_camera.h"
#include "race_flow.h"
#include "race_intro_transition.h"
#include "race_intro_transition_2.h"
#include "viewport_manager.h"
#define MENU_RENDERING_S16_STEP_PROTOTYPE
#include "menu_rendering.h"

#define OS_MESG_BLOCK 1

typedef struct OSThread_s OSThread;
typedef void *OSMesg;

typedef struct OSMesgQueue_s {
    OSThread *mtqueue;
    OSThread *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;

typedef struct OSContStatus {
    u16 type;
    u8 status;
    u8 errno;
} OSContStatus;

typedef struct OSContPad {
    u16 button;
    s8 stick_x;
    s8 stick_y;
    u8 errno;
} OSContPad;

typedef struct OSPfs {
    s32 status;
    OSMesgQueue *queue;
    s32 channel;
    u8 id[32];
    u8 label[32];
    s32 version;
    s32 dir_size;
    s32 inode_table;
    s32 minode_table;
    s32 dir_table;
    s32 inode_start_page;
    u8 banks;
    u8 activebank;
} OSPfs;

typedef struct OSPfsState {
    u32 file_size;
    u32 game_code;
    u16 company_code;
    char ext_name[4];
    char game_name[16];
} OSPfsState;

typedef struct SaveFileIdentity {
    s32 size;
    u32 gameCode;
    u16 companyCode;
} SaveFileIdentity;

typedef struct SaveSlotBytes {
    s32 checksum;
    u8 bytes[0x78DC];
    u8 tail[0x18];
} SaveSlotBytes;

typedef struct MainMenuState {
    char pad[0x18];
    s32 fade;
    s32 selection;
    s32 delay;
    s32 timer;
} MainMenuState;

extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
extern s32 osSendMesg(OSMesgQueue *, OSMesg, s32);
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
extern void osSetEventMesg(s32, OSMesgQueue *, OSMesg);
extern s32 osContInit(OSMesgQueue *, u8 *, OSContStatus *);
extern s32 osContStartReadData(OSMesgQueue *);
extern void osContGetReadData(OSContPad *);
extern void osCreateThread(OSThread *, s32, void (*)(void *), void *, void *, s32);
extern void osStartThread(OSThread *);
extern s32 osMotorInit(OSMesgQueue *, OSPfs *, s32);
extern s32 osMotorStart(OSPfs *);
extern s32 osMotorStop(OSPfs *);
extern s32 osPfsInitPak(OSMesgQueue *, OSPfs *, int);
extern s32 osPfsRepairId(OSPfs *);
extern s32 osPfsFindFile(OSPfs *, u16, u32, u8 *, u8 *, s32 *);
extern s32 osPfsFreeBlocks(OSPfs *, s32 *);
extern s32 osPfsNumFiles(OSPfs *, s32 *, s32 *);
extern s32 osPfsFileState(OSPfs *, s32, OSPfsState *);
extern s32 osPfsDeleteFile(OSPfs *, u16, u32, u8 *, u8 *);
extern s32 osPfsFindFile(OSPfs *, u16, u32, u8 *, u8 *, s32 *);
extern s32 osPfsAllocateFile(OSPfs *, u16, u32, u8 *, u8 *, int, s32 *);
extern s32 osPfsReadWriteFile(OSPfs *, s32, u8, int, int, u8 *);
extern void releaseMenuAssetHandles(void);
extern void enqueueSoundEffect(s32, s32);
extern void updateMainMenuSettings(void);
extern void fadeOutMainMenuSettings(void);
extern void updateMainMenuModeSelect(void);
extern void updateMainMenu(void);
extern OSThread gControllerSubsystemThread;
extern OSMesgQueue gControllerSubsystemRequestQueue;
extern OSMesg gControllerSubsystemRequestMessages[];
extern OSMesgQueue gControllerSubsystemReplyQueue;
extern OSMesg gControllerSubsystemReplyMessages[];
extern OSMesgQueue gControllerEventQueue;
extern OSMesg gControllerEventMessages[];
extern s16 gControllerEventMessage;
extern OSContPad gControllerPads[];
extern s32 gControllerPakFileNos[];
extern OSPfs gControllerPakHandles[];
extern OSPfs gRumblePakHandles[];
extern SaveFileIdentity gControllerPakSaveFileIdentity;
extern u8 gControllerPakGameName[];
extern u8 gControllerPakExtName[];
extern MainMenuState *gCurrentGameTask;
extern u8 gConnectedControllerBitmask;
extern u8 D_800B30F4[];
extern u8 D_800B3104[];
extern u8 D_800B3108[];
extern u8 gMainMenuReturnFromRace;
extern u8 gFramebufferSwapDelay;
extern u8 gControllerReadPending;
extern OSContStatus gControllerStatuses[];
extern s16 gControllerInputState;
extern u8 D_800E4C1A;
extern u8 D_800E4C1B;
extern s16 D_800E4C1E;
extern u8 D_800E4C20;
extern u8 D_800E4C21;
extern s16 D_800E4C24;
extern u8 D_800E4C26;
extern u8 D_800E4C27;
extern s16 D_800E4C2A;
extern u8 D_800E4C2C;
extern u8 D_800E4C2D;
extern s32 gControllerPakFileNos[];
extern u8 gControllerPakGameName[];
extern u8 gControllerPakExtName[];
extern u8 gControllerPakRetryCounts;
extern s32 gRumbleMotorStatuses;
extern s32 D_800EC89C;
extern s32 D_800EC8A0;
extern s32 D_800EC8A4;
extern u8 gRumblePakConnectedByController[];
extern void *D_800EC8B8;
extern s16 gControllerPakStatusCodes[];
extern u8 gControllerPakOperationCounts[];
extern SaveSlotBytes gGameSaveDataBuffer[];
extern s32 gPlayerInputHeld;
extern s16 gMenuFadeAlpha;
extern s16 D_801124B8;
extern s16 gRumbleMotorRequestStates[];
extern u8 D_593D10[];
extern u8 D_598A70[];
extern u8 D_1F1A90[];
extern u8 D_1F2220[];
extern u8 D_60F1A0[];
extern u8 D_60F990[];
extern u8 D_1467B0[];
extern u8 D_147910[];
extern u8 D_1DE360[];
extern u8 D_1E0F70[];
extern u8 D_5DB9D0[];
extern u8 D_5DCBE0[];
extern u8 D_5DFDD0[];
extern s8 gMainMenuSecretCodeUnlocked;
extern u8 gMainMenuSecretCodeStep;
extern s8 D_800DEF10;
extern u8 gConnectedControllerCount;
extern u8 gRaceRumbleEnabled;
extern u8 gRumblePakConnectedMask;
extern s8 gMainMenuSelectionResult;
extern OSPfsState gControllerPakFileStates[];
extern s32 gControllerPakFreeBytes;
extern s32 gControllerPakFreeFileCount;
extern OSMesgQueue D_80124070;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern u8 gRaceRecordSettingsEnabled;
extern u8 gRaceCourseModelEffectsDisabled;
extern u8 gRaceCourseOverlayEffectsDisabled;
extern s32 D_801235B4;
extern s32 gPlayerInputPressed;
extern u8 D_800B30F4[];
extern u8 D_800B3104[];
extern u8 D_800B3108[];

// initControllerSubsystem best match: 85.817%
#pragma GLOBAL_ASM("asm/nonmatchings/menu_system_flow/initControllerSubsystem.s")

#ifdef NON_MATCHING
void initControllerSubsystem(void) {
    s32 i;

    osCreateMesgQueue(&gControllerEventQueue, gControllerEventMessages, 1);
    osCreateMesgQueue(&gControllerSubsystemRequestQueue, gControllerSubsystemRequestMessages, 8);
    osCreateMesgQueue(&gControllerSubsystemReplyQueue, gControllerSubsystemReplyMessages, 1);
    osSetEventMesg(5, &gControllerEventQueue, (OSMesg)1);
    osContInit(&gControllerEventQueue, &gConnectedControllerBitmask, gControllerStatuses);

    gConnectedControllerCount = 0;
    gControllerReadPending = 0;

    i = 0;
loop:
    if (((s32)gConnectedControllerBitmask >> i) & 1) {
        i++;
        gConnectedControllerCount++;
        if (i < 4) {
            goto loop;
        }
    } else {
        i++;
    }

    gControllerEventMessage = 9;
    gControllerInputState = 0;
    D_800E4C1A = 0;
    D_800E4C1B = 0;
    gRumbleMotorStatuses = 1;
    D_800E4C1E = 0;
    D_800E4C20 = 0;
    D_800E4C21 = 0;
    D_800EC89C = 1;
    D_800E4C24 = 0;
    D_800E4C26 = 0;
    D_800E4C27 = 0;
    D_800EC8A0 = 1;
    D_800E4C2A = 0;
    D_800E4C2C = 0;
    D_800E4C2D = 0;
    D_800EC8A4 = 1;

    osCreateThread(&gControllerSubsystemThread, 4, controllerSubsystemThreadMain, D_800EC8B8, &gControllerSubsystemRequestQueue, 0x14);
    osStartThread(&gControllerSubsystemThread);
}
#endif

// controllerSubsystemThreadMain best match: 99.507%
#pragma GLOBAL_ASM("asm/nonmatchings/menu_system_flow/controllerSubsystemThreadMain.s")

#ifdef NON_MATCHING
void controllerSubsystemThreadMain(void *arg0) {
    OSMesg msg;
    s32 msgValue;
    s32 channel;

    msg = NULL;
    while (((1 & 0xFFFFFFFFFFFFFFFF) & 0xFFFFFFFFFFFFFFFF) & 1) {
        osRecvMesg(&gControllerSubsystemRequestQueue, &msg, OS_MESG_BLOCK);
        msgValue = (s32)msg;
        switch (msgValue & 0xF0) {
        case 0x10:
            osContStartReadData(&gControllerEventQueue);
            osRecvMesg(&gControllerEventQueue, ((OSMesg *)&arg0) - 2, OS_MESG_BLOCK);
            osContGetReadData(gControllerPads);
            osSendMesg(&D_80124070, &gControllerEventMessage, 0);
            break;
        case 0x20:
            probeControllerPak(msgValue & 3);
            osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, 0);
            break;
        case 0x30:
            checkControllerPakSaveStatus(msgValue & 3);
            osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, 0);
            break;
        case 0x40:
            readControllerPakSave(msgValue & 3);
            osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, 0);
            break;
        case 0x50:
            writeControllerPakSave(msgValue & 3);
            osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, 0);
            break;
        case 0x60:
            repairControllerPakId(msgValue & 3);
            osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, 0);
            break;
        case 0x70:
            channel = msgValue & 3;
            (&gRumbleMotorStatuses)[channel] = osMotorInit(&gControllerEventQueue, &gRumblePakHandles[channel], channel);
            osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, 0);
            break;
        case 0xD0:
            channel = msgValue & 3;
            (&gRumbleMotorStatuses)[channel] = osMotorInit(&gControllerEventQueue, &gRumblePakHandles[channel], channel);
            break;
        case 0x80:
            if (((&gRumbleMotorStatuses)[msgValue & 3] != 1) && ((&gRumbleMotorStatuses)[msgValue & 3] != 11) &&
                ((&gRumbleMotorStatuses)[msgValue & 3] != 4)) {
                channel = msgValue & 3;
                if (osMotorStart(&gRumblePakHandles[channel]) == 4) {
                    (&gRumbleMotorStatuses)[channel] = 4;
                }
            }
            break;
        case 0x90:
            if (((&gRumbleMotorStatuses)[msgValue & 3] != 1) && ((&gRumbleMotorStatuses)[msgValue & 3] != 11) &&
                ((&gRumbleMotorStatuses)[msgValue & 3] != 4)) {
                channel = msgValue & 3;
                if (osMotorStop(&gRumblePakHandles[channel]) == 4) {
                    (&gRumbleMotorStatuses)[channel] = 4;
                }
            }
            break;
        case 0xA0:
            readControllerPakFileStates();
            osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, 0);
            break;
        case 0xB0:
            deleteControllerPakFile(msgValue & 0xF);
            osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, 0);
            break;
        case 0xC0:
            updateControllerPakFreeSpaceInfo();
            osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, 0);
            break;
        }
    }
}
#endif

void requestControllerRead(void) {
    if ((gControllerReadPending == 0) && (gConnectedControllerBitmask != 0)) {
        osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)0x10, OS_MESG_BLOCK);
        gControllerReadPending = 1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/menu_system_flow/updateControllerInputState.s")

void requestRumbleMotorInit(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + 0x70), OS_MESG_BLOCK);
    osRecvMesg(&gControllerSubsystemReplyQueue, &msg, OS_MESG_BLOCK);
}

#pragma GLOBAL_ASM("asm/nonmatchings/menu_system_flow/updateRumbleMotorRequest.s")

void requestRumbleMotorStart(u16 arg0) {
    if (gRaceRumbleEnabled != 0) {
        if (gRumblePakConnectedMask & (1 << arg0)) {
            gRumbleMotorRequestStates[arg0] = 1;
        }
    }
}

void requestControllerPakProbe(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + 0x20), OS_MESG_BLOCK);
    osRecvMesg(&gControllerSubsystemReplyQueue, &msg, OS_MESG_BLOCK);
}

// probeControllerPak best match: 94.507%
#pragma GLOBAL_ASM("asm/nonmatchings/menu_system_flow/probeControllerPak.s")

#ifdef NON_MATCHING
void probeControllerPak(u16 arg0) {
    s32 ret;

    ret = osPfsInitPak(&gControllerEventQueue, &gControllerPakHandles[arg0], arg0);
    if (ret == 2) {
        ret = osPfsInitPak(&gControllerEventQueue, &gControllerPakHandles[arg0], arg0);
    }

    if (ret == 0) {
        gControllerPakStatusCodes[arg0] = ret + 1;
    }

    if ((ret == 1) || (ret == 11)) {
        gControllerPakStatusCodes[arg0] = 10;
    }

    if (ret == 10) {
        if (gRumblePakConnectedByController[arg0] == 1) {
            gControllerPakStatusCodes[arg0] = 16;
        } else {
            gControllerPakStatusCodes[arg0] = 7;
        }
    }

    if (ret != 0) {
        gControllerPakOperationCounts[arg0]++;
    }
}
#endif

void requestControllerPakSaveStatus(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + 0x30), OS_MESG_BLOCK);
    osRecvMesg(&gControllerSubsystemReplyQueue, &msg, OS_MESG_BLOCK);
}

// checkControllerPakSaveStatus best match: 81.386%
#pragma GLOBAL_ASM("asm/nonmatchings/menu_system_flow/checkControllerPakSaveStatus.s")

#ifdef NON_MATCHING
void checkControllerPakSaveStatus(u16 arg0) {
    s32 ret;
    s32 maxFiles;
    s32 filesUsed;
    s32 freeBytes;
    OSPfs *pfs;
    u8 *src;
    u8 *dst;
    u8 byte0;
    u8 byte1;
    u8 byte2;
    u8 byte3;

    gControllerPakSaveFileIdentity.file_size = 0x7900;
    gControllerPakSaveFileIdentity.game_code = 0x4E534B45;
    gControllerPakSaveFileIdentity.company_code = 0x4542;

    src = D_800B3104;
    dst = (u8 *) &gControllerPakSaveFileIdentity;
    do {
        byte0 = *src;
        src++;
        dst++;
        dst[9] = byte0;
    } while (src < D_800B3108);

    src = D_800B30F4;
    dst = (u8 *) &gControllerPakSaveFileIdentity;
    do {
        byte0 = *src++;
        byte1 = *src++;
        byte2 = *src++;
        byte3 = *src++;
        dst += 4;
        dst[0xA] = byte0;
        dst[0xB] = byte1;
        dst[0xC] = byte2;
        dst[0xD] = byte3;
    } while (src != D_800B3104);

    pfs = &gControllerPakHandles[arg0];
    osPfsInitPak(&gControllerEventQueue, pfs, arg0);

    ret = osPfsFindFile(pfs, gControllerPakSaveFileIdentity.company_code, gControllerPakSaveFileIdentity.game_code, gControllerPakExtName, gControllerPakGameName,
                        &gControllerPakFileNos[arg0]);
    if (ret == 0) {
        gControllerPakStatusCodes[arg0] = 2;
    } else {
        osPfsNumFiles(pfs, &maxFiles, &filesUsed);
        if (filesUsed == 0x10) {
            gControllerPakStatusCodes[arg0] = 0xC;
        } else {
            osPfsFreeBlocks(pfs, &freeBytes);
            if ((freeBytes / 256) < 0x79) {
                gControllerPakStatusCodes[arg0] = 0xB;
            } else if (ret == 5) {
                gControllerPakStatusCodes[arg0] = 9;
            }
        }
    }

    if (ret != 0) {
        gControllerPakOperationCounts[arg0]++;
    }
}
#endif

void requestControllerPakSaveRead(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + 0x40), OS_MESG_BLOCK);
    osRecvMesg(&gControllerSubsystemReplyQueue, &msg, OS_MESG_BLOCK);
}

// readControllerPakSave best match: 85.904%
#pragma GLOBAL_ASM("asm/nonmatchings/menu_system_flow/readControllerPakSave.s")

#ifdef NON_MATCHING
void readControllerPakSave(u16 arg0) {
    s32 ret;
    u16 badChecksum;
    OSPfs *pfs;
    s32 channel;
    s32 savedChannel;
    s32 *fileNo;
    SaveSlotBytes *save;
    u8 *src;
    u8 *dst;
    u8 *end;
    u8 *bytes;
    s32 checksum;
    s32 offset;

    channel = arg0 & 0xFFFF;
    savedChannel = channel;
    pfs = &gControllerPakHandles[channel];
    badChecksum = 0;
    osPfsInitPak(&gControllerEventQueue, pfs, channel);

    gControllerPakSaveFileIdentity.gameCode = 'NSKE';
    gControllerPakSaveFileIdentity.companyCode = 'EB';

    src = D_800B3104;
    dst = (u8 *)&gControllerPakSaveFileIdentity;
    end = D_800B3108;
copy_ext:
    dst[10] = *src;
    src++;
    dst++;
    if (src < end) {
        goto copy_ext;
    }

    src = D_800B30F4;
    dst = (u8 *)&gControllerPakSaveFileIdentity;
    end = D_800B3104;
copy_name:
    dst[14] = *src;
    src++;
    dst++;
    if (src < end) {
        goto copy_name;
    }

    fileNo = &gControllerPakFileNos[channel];
    osPfsFindFile(pfs, gControllerPakSaveFileIdentity.companyCode, gControllerPakSaveFileIdentity.gameCode, gControllerPakExtName, gControllerPakGameName, fileNo);

    save = &gGameSaveDataBuffer[channel];
    ret = osPfsReadWriteFile(pfs, *fileNo, 0, 0, 0x78E0, (u8 *)save);
    if (ret == 0) {
        checksum = 0;
        bytes = save->bytes;
        offset = 4;
checksum_loop:
        checksum += bytes[0];
        checksum += bytes[1];
        checksum += bytes[2];
        checksum += bytes[3];
        offset += 4;
        bytes += 4;
        if (offset != 0x78E0) {
            goto checksum_loop;
        }
        if (checksum != save->checksum) {
            badChecksum = 1;
        }

        if (badChecksum == 0) {
            if (validateControllerPakSave(savedChannel) == 0) {
                (&gControllerPakRetryCounts)[channel] = 0;
            }
        } else {
            (&gControllerPakRetryCounts)[channel]++;
        }
    } else {
        (&gControllerPakRetryCounts)[channel]++;
    }

    if ((ret != 0) || ((&gControllerPakRetryCounts)[channel] != 0)) {
        if ((&gControllerPakRetryCounts)[channel] != 3) {
            return;
        }
    }
    gControllerPakOperationCounts[channel]++;
}
#endif

void requestControllerPakSaveWrite(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + 0x50), OS_MESG_BLOCK);
    osRecvMesg(&gControllerSubsystemReplyQueue, &msg, OS_MESG_BLOCK);
}

// writeControllerPakSave best match: 78.684%
#pragma GLOBAL_ASM("asm/nonmatchings/menu_system_flow/writeControllerPakSave.s")

#ifdef NON_MATCHING
void writeControllerPakSave(u16 arg0) {
    OSPfs * volatile pfs;
    s32 *fileNo;
    s32 channel;
    u8 *src;
    u8 *dst;
    u8 *end;
    SaveSlotBytes *save;
    u8 *bytes;
    s32 checksum;
    s32 offset;

    channel = arg0 & 0xFFFF;
    gControllerPakSaveFileIdentity.size = 0x7900;
    gControllerPakSaveFileIdentity.gameCode = 'NSKE';
    gControllerPakSaveFileIdentity.companyCode = 'EB';

    src = D_800B3104;
    dst = (u8 *)&gControllerPakSaveFileIdentity;
    end = D_800B3108;
copy_ext:
    dst[10] = *src;
    src++;
    dst++;
    if (src < end) {
        goto copy_ext;
    }

    src = D_800B30F4;
    dst = (u8 *)&gControllerPakSaveFileIdentity;
    end = D_800B3104;
copy_name:
    dst[14] = *src;
    src++;
    dst++;
    if (src < end) {
        goto copy_name;
    }

    pfs = &gControllerPakHandles[channel];
    osPfsInitPak(&gControllerEventQueue, pfs, channel);

    fileNo = &gControllerPakFileNos[channel];
    if (osPfsFindFile(pfs, gControllerPakSaveFileIdentity.companyCode, gControllerPakSaveFileIdentity.gameCode, gControllerPakExtName, gControllerPakGameName, fileNo) == 5) {
        osPfsAllocateFile(pfs, gControllerPakSaveFileIdentity.companyCode, gControllerPakSaveFileIdentity.gameCode, gControllerPakExtName, gControllerPakGameName, 0x7900, fileNo);
    }

    save = &gGameSaveDataBuffer[channel];
    checksum = 0;
    bytes = save->bytes;
    offset = 4;
checksum_loop:
    checksum += bytes[0];
    checksum += bytes[1];
    checksum += bytes[2];
    checksum += bytes[3];
    offset += 4;
    bytes += 4;
    if (offset != 0x78E0) {
        goto checksum_loop;
    }

    save->checksum = checksum;
    if (osPfsReadWriteFile(pfs, *fileNo, 1, 0, 0x78E0, (u8 *)save) == 0) {
        (&gControllerPakRetryCounts)[channel] = 0;
        return;
    }
    (&gControllerPakRetryCounts)[channel]++;
}
#endif

void requestControllerPakRepair(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + 0x60), OS_MESG_BLOCK);
    osRecvMesg(&gControllerSubsystemReplyQueue, &msg, OS_MESG_BLOCK);
}

void repairControllerPakId(u16 arg0) {
    OSPfs **sp18;
    OSPfs *pfs;
    s32 ret;

    pfs = &gControllerPakHandles[arg0];
    sp18 = &pfs;
    osPfsInitPak(&gControllerEventQueue, *sp18, arg0);
    ret = osPfsRepairId(pfs);
    if ((ret == 4) || (ret == 0xA)) {
        (&gControllerPakRetryCounts)[arg0] += 1;
    }
}

void requestControllerPakFileList(void) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)0xA0, OS_MESG_BLOCK);
    osRecvMesg(&gControllerSubsystemReplyQueue, &msg, OS_MESG_BLOCK);
}

void readControllerPakFileStates(void) {
    s32 i;

    osPfsInitPak(&gControllerEventQueue, &gControllerPakHandles[0], 0);
    for (i = 0; i != 0x10; i++) {
        osPfsFileState(&gControllerPakHandles[0], i, &gControllerPakFileStates[i]);
    }
}

void requestControllerPakDeleteFile(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + 0xB0), OS_MESG_BLOCK);
    osRecvMesg(&gControllerSubsystemReplyQueue, &msg, OS_MESG_BLOCK);
}

void deleteControllerPakFile(u16 arg0) {
    OSPfs *pfs;
    OSPfsState *state;
    u16 companyCode;
    u32 gameCode;
    u8 gameName[16];
    u8 extName[4];
    s32 i;

    pfs = gControllerPakHandles;
    osPfsInitPak(&gControllerEventQueue, pfs, 0);

    state = &gControllerPakFileStates[arg0];
    companyCode = state->company_code;
    gameCode = state->game_code;

    for (i = 0; i < 16; i++) {
        gameName[i] = gControllerPakFileStates[arg0].game_name[i];
    }

    for (i = 0; i < 4; i++) {
        extName[i] = gControllerPakFileStates[arg0].ext_name[i];
    }

    for (i = 0; i != 3; i++) {
        if (osPfsDeleteFile(pfs, companyCode, gameCode, gameName, extName) == 0) {
            gControllerPakRetryCounts = 0;
            return;
        }
        gControllerPakRetryCounts++;
    }
}

void requestControllerPakFreeSpaceUpdate(void) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)0xC0, OS_MESG_BLOCK);
    osRecvMesg(&gControllerSubsystemReplyQueue, &msg, OS_MESG_BLOCK);
}

void updateControllerPakFreeSpaceInfo(void) {
    s32 pad;
    s32 maxFiles;
    s32 filesUsed;

    osPfsInitPak(&gControllerEventQueue, &gControllerPakHandles[0], 0);
    osPfsFreeBlocks(&gControllerPakHandles[0], &gControllerPakFreeBytes);
    osPfsNumFiles(&gControllerPakHandles[0], &maxFiles, &filesUsed);
    gControllerPakFreeFileCount = maxFiles - filesUsed;
}

// validateControllerPakSave best match: 33.167%
#pragma GLOBAL_ASM("asm/nonmatchings/menu_system_flow/validateControllerPakSave.s")

#ifdef NON_MATCHING
u16 validateControllerPakSave(s32 arg0) {
    u8 *var_a3;
    u8 *var_a2;
    u8 *var_v1;
    s32 var_a1;
    s32 var_a0;

    var_a3 = (u8 *)&D_800EC9F0[arg0];
    var_a2 = var_a3;
    var_v1 = var_a3;
    var_a1 = 0;
    do {
        var_a1 += 4;
        var_a2 += 4;
        var_v1 += 1;
    } while (var_a1 < 0x2C);

    var_a0 = 0;
    var_v1 = var_a3;
    do {
        var_a0 += 1;
        var_v1 += 1;
    } while (var_a0 != 0xC);

    return validateControllerPakSaveData(arg0);
}
#endif

s32 validateControllerPakSaveData(s32 arg0) {
    s32 pad;
    volatile s32 sp8;
    u8 *var_a0;
    u8 *var_a2;
    u8 *var_t2;
    u8 *var_t3;
    u8 *var_t4;
    u8 *var_t5;
    u8 *var_v0;
    s16 temp_s0_3;
    s16 temp_s0_7;
    s16 temp_v0_3;
    s32 var_a1;
    s32 var_a3;
    s32 var_v1;
    s8 temp_s0;
    s8 temp_s0_2;
    s8 temp_s0_5;
    s8 temp_s0_6;
    s8 temp_v0;
    s8 temp_v0_2;
    s32 temp_t7;
    s32 temp_s0_10;
    s32 temp_s0_4;
    s32 temp_s0_8;
    s32 temp_s0_9;

    var_t2 = (u8 *)&gGameSaveDataBuffer[arg0];
    var_v1 = 0;
    var_t3 = var_t2;
    var_t4 = var_t2;
    var_t5 = var_t2;
    var_a3 = 0;
    var_t2 += 0;

    do {
        sp8 = 0;
        var_v0 = var_t3;
        var_a0 = var_t4;
        var_a1 = 0;
        var_a2 = var_t5;
loop_2:
        temp_s0 = *(s8 *)(var_v0 + 0x4E);
        var_a1 += 2;
        if ((temp_s0 < 0) || (temp_s0 >= 0x64)) {
            var_v1 = 1;
        }
        temp_s0_2 = *(s8 *)(var_v0 + 0x4F);
        if ((temp_s0_2 < 0) || (temp_s0_2 >= 0x3C)) {
            var_v1 = 1;
        }
        temp_s0_3 = *(s16 *)(var_v0 + 0x50);
        if ((temp_s0_3 < 0) || (temp_s0_3 >= 0x6301)) {
            var_v1 = 1;
        }
        temp_s0_4 = *(u8 *)(var_a0 + 0x77FB);
        if ((temp_s0_4 & 7) >= 6) {
            var_v1 = 1;
        }
        if ((((s32)temp_s0_4 >> 3) & 0xF) >= 0xF) {
            var_v1 = 1;
        }
        temp_s0_5 = *(s8 *)(var_v0 + 0x156);
        if ((temp_s0_5 < 0) || (temp_s0_5 >= 0x64)) {
            var_v1 = 1;
        }
        temp_s0_6 = *(s8 *)(var_v0 + 0x157);
        if ((temp_s0_6 < 0) || (temp_s0_6 >= 0x3C)) {
            var_v1 = 1;
        }
        temp_s0_7 = *(s16 *)(var_v0 + 0x158);
        var_v0 += 4;
        if ((temp_s0_7 < 0) || (temp_s0_7 >= 0x6301)) {
            var_v1 = 1;
        }
        temp_s0_8 = *(u8 *)(var_a0 + 0x78A0);
        if ((temp_s0_8 & 7) >= 6) {
            var_v1 = 1;
        }
        if ((((s32)temp_s0_8 >> 3) & 0xF) >= 0xF) {
            var_v1 = 1;
        }
        if (var_a3 == 0x24) {
            if (*(u8 *)(var_a0 + 0x7832) >= 0x1F) {
                goto block_32;
            }
        } else if (*(u8 *)(var_a0 + 0x7832) >= 0x3D) {
block_32:
            var_v1 = 1;
        }
        temp_s0_9 = *(u8 *)(var_a0 + 0x7869);
        if ((temp_s0_9 & 7) >= 6) {
            var_v1 = 1;
        }
        if ((((s32)temp_s0_9 >> 3) & 0xF) >= 0xF) {
            var_v1 = 1;
        }
        temp_t7 = (*(u16 *)(var_a2 + 0x7756)) & 0xFFFFu;
        var_a2 += 2;
        if ((s32)temp_t7 >= 0x2710) {
            var_v1 = 1;
        }
        temp_s0_10 = *(u8 *)(var_a0 + 0x77C4);
        if ((temp_s0_10 & 7) >= 6) {
            var_v1 = 1;
        }
        if ((((s32)temp_s0_10 >> 3) & 0xF) >= 0xF) {
            var_v1 = 1;
        }
        var_a0 += 1;
        if (var_a1 != 0xA) {
            goto loop_2;
        }
        temp_v0 = *(s8 *)(var_t2 + 0x12A);
        var_t3 += 0x14;
        var_t4 += 5;
        if ((temp_v0 < 0) || (temp_v0 >= 0x64)) {
            var_v1 = 1;
        }
        temp_v0_2 = *(s8 *)(var_t2 + 0x12B);
        var_t5 += 0xA;
        var_a3 += 4;
        if ((temp_v0_2 < 0) || (temp_v0_2 >= 0x3C)) {
            var_v1 = 1;
        }
        temp_v0_3 = *(s16 *)(var_t2 + 0x12C);
        if ((temp_v0_3 < 0) || (temp_v0_3 >= 0x6301)) {
            var_v1 = 1;
        }
        var_t2 += 4;
    } while (var_a3 != 0x2C);

    return var_v1;
}

void enterMainMenuFromRace(void) {
    gMainMenuReturnFromRace = 1;
    setCurrentGameTaskCallback(initMainMenu, 0);
    createGameTask(4, func_8003ED00, 0x64);
    suspendGameTask(3);
}

void initMainMenu(void) {
    gMainMenuSecretCodeUnlocked = 0;
    gMainMenuSecretCodeStep = 0;
    resetAllViewports();
    func_80070C64(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    gFramebufferSwapDelay = 0;
    loadCompressedRomAsset(D_5DB9D0, D_5DCBE0, 0x21);
    loadCompressedRomAsset(D_5DCBE0, D_5DFDD0, 0x25);
    loadCompressedRomAsset(D_593D10, D_598A70, 0x22);
    loadRawRomAsset(D_1467B0, D_147910, 8);
    loadCompressedRomAsset(D_1DE360, D_1E0F70, 9);
    gCurrentGameTask->fade = 0xFF;
    gCurrentGameTask->selection = 0;
    gCurrentGameTask->delay = 0x32;
    gCurrentGameTask->timer = 0x4B0;
    initCallbackTaskScheduler(0);
    if (gConnectedControllerCount != 0) {
        createCallbackTask(&func_80032A50, 0, 0x64);
    } else {
        createCallbackTask(&updateTitleStartPrompt, 0, 0x64);
    }
    createCallbackTask(&func_80032934, 0, 0x64);
    createCallbackTask(&func_80032684, 0, 0x64);
    createCallbackTaskWithUserId(&initTitleMenuSparkle, 0, 0x64, 0);
    createCallbackTaskWithUserId(initTitleMenuBoardModels, 0, 0x64, 0);
    setBootFadeColor(0x20, 0x40, 0x50);
    loadMainMenuSceneModelAnimationBank();
    initMainMenuSceneModel(0, 0);
    setMainMenuSceneModelAnimation(0, 0);
    setMainMenuSceneModelPosition(0, 0x83000, 0, -0x2BF000);
    setMainMenuSceneModelRotation(0, 0, 0x30, 0);
    initMainMenuSceneModel(1, 1);
    setMainMenuSceneModelAnimation(1, 4);
    setMainMenuSceneModelPosition(1, 0x564000, 0, 0x8C000);
    setMainMenuSceneModelRotation(1, 0, 0xDD0, 0);
    initMainMenuSceneModel(2, 2);
    setMainMenuSceneModelAnimation(2, 3);
    setMainMenuSceneModelPosition(2, -0x5ED000, 0, 0x3C8000);
    setMainMenuSceneModelRotation(2, 0, 0x2D0, 0);
    initMainMenuSceneModel(3, 3);
    setMainMenuSceneModelAnimation(3, 1);
    setMainMenuSceneModelPosition(3, -0x4C0000, 0, -0x47000);
    setMainMenuSceneModelRotation(3, 0, 0x1B0, 0);
    initMainMenuSceneModel(4, 4);
    setMainMenuSceneModelAnimation(4, 5);
    setMainMenuSceneModelPosition(4, 0x6DB000, 0, 0x4E9000);
    setMainMenuSceneModelRotation(4, 0, 0xDD0, 0);
    func_8006D5CC();
    func_8006D520(0, 0x1F);
    gMenuFadeAlpha = (s16) gCurrentGameTask->fade;
    D_800DEF10 = 1;
    enqueueSoundEffect(0x4A, 0x32);
    setCurrentGameTaskCallback(&updateMainMenu, 0);
    requestRumbleMotorInit(0U);
    requestRumbleMotorInit(1U);
    requestRumbleMotorInit(2U);
    requestRumbleMotorInit(3U);
}

void updateMainMenu(void) {
    s32 unused[2];
    s32 flag;
    s32 temp_v1;
    s32 temp_a0;

    flag = 0;
    temp_v1 = gCurrentGameTask->fade;
    if (temp_v1 != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) temp_v1, 0x10, 0);
        if (gCurrentGameTask->fade == 0) {
            D_800DEF10 = 0;
        }
    } else {
        if (gCurrentGameTask->delay != 0) {
            gCurrentGameTask->delay -= 1;
            if (gCurrentGameTask->delay == 0) {
                requestMusicSequenceBank(0);
            }
        }
        temp_a0 = gPlayerInputPressed;
        if (temp_a0 & 0x10800) {
            if (gCurrentGameTask->selection != 0) {
                gCurrentGameTask->selection -= 1;
                enqueueSoundEffect(0x19, 0x32);
                temp_a0 = gPlayerInputPressed;
            }
        }
        if (temp_a0 & 0x20400) {
            if (gCurrentGameTask->selection != 2) {
                gCurrentGameTask->selection += 1;
                enqueueSoundEffect(0x19, 0x32);
                temp_a0 = gPlayerInputPressed;
            }
        }
        if ((temp_a0 & 0x1000) || (temp_a0 & 0x8000)) {
            flag = 1;
            enqueueSoundEffect(1, 0x32);
        }
        if (flag == 0) {
            gCurrentGameTask->timer -= 1;
            if (gCurrentGameTask->timer == 0) {
                flag = 1;
            }
        }
        if (flag != 0) {
            setCurrentGameTaskCallback(fadeOutMainMenu, 0);
            requestMusicSequenceStop(0xC);
        }
    }
    func_8006D780(0);
    if (checkMainMenuSecretCode() != 0) {
        gMainMenuSecretCodeUnlocked = 1;
        enqueueSoundEffect(0x26, 0x32);
    }
    loopMainMenuSceneModelAnimation(0);
    loopMainMenuSceneModelAnimation(1);
    loopMainMenuSceneModelAnimation(2);
    loopMainMenuSceneModelAnimation(3);
    loopMainMenuSceneModelAnimation(4);
    if (gCurrentGameTask->fade == 0xEF) {
        addMainMenuSceneModelDrawCallback(0);
        addMainMenuSceneModelDrawCallback(1);
        addMainMenuSceneModelDrawCallback(2);
        addMainMenuSceneModelDrawCallback(3);
        addMainMenuSceneModelDrawCallback(4);
    } else {
        addMainMenuSceneModelDrawCallbackForViewport0(0);
        addMainMenuSceneModelDrawCallback(1);
        addMainMenuSceneModelDrawCallback(2);
        addMainMenuSceneModelDrawCallbackForViewport0(3);
        addMainMenuSceneModelDrawCallback(4);
    }
    updateCallbackTasks();
}

void fadeOutMainMenu(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = gCurrentGameTask->fade;
    if (temp_v0 != 0xFF) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16) temp_v0, 0x28, 1);
        if (gCurrentGameTask->fade == 0xFF) {
            gFramebufferSwapHold = 1;
        } else {
            func_8006D780(0);
            loopMainMenuSceneModelAnimation(0);
            loopMainMenuSceneModelAnimation(1);
            loopMainMenuSceneModelAnimation(2);
            loopMainMenuSceneModelAnimation(3);
            loopMainMenuSceneModelAnimation(4);
            addMainMenuSceneModelDrawCallback(0);
            addMainMenuSceneModelDrawCallback(1);
            addMainMenuSceneModelDrawCallback(2);
            addMainMenuSceneModelDrawCallback(3);
            addMainMenuSceneModelDrawCallback(4);
            updateCallbackTasks();
        }
    } else if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay = 0;
        if (gCurrentGameTask->timer != 0) {
            temp_v1 = gCurrentGameTask->selection;
            if (temp_v1 == 0) {
                createGameTask(2, func_80073140, 0x64);
                removeGameTask(3);
            } else if (temp_v1 == 1) {
                setCurrentGameTaskCallback(initMainMenuModeSelect, 0);
            } else {
                setCurrentGameTaskCallback(initMainMenuSettings, 0);
            }
        } else if (gMainMenuReturnFromRace == 0) {
            setCurrentGameTaskCallback(enterMainMenuFromRace, 0);
        } else {
            gMainMenuReturnFromRace = 0;
            setCurrentGameTaskCallback(initMainMenu, 0);
            createGameTask(4, func_8003E600, 0x64);
            suspendGameTask(3);
        }
    }
}

void initMainMenuModeSelect(void) {
    resetAllViewports();
    D_801124B8 = 0x80;
    gFramebufferSwapDelay = 0;
    func_80070C64(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    loadRawRomAsset(D_1467B0, D_147910, 8);
    loadCompressedRomAsset(D_1DE360, D_1E0F70, 9);
    loadCompressedRomAsset(D_1F1A90, D_1F2220, 0x28);
    func_8006D5CC();
    func_8006D520(0, 0x1F);
    initCallbackTaskScheduler(0);
    createCallbackTaskWithUserId(initTitleMenuBoardModels, 0, 0x64, 0);
    D_801235B4 = 0;
    gMainMenuSelectionResult = 0;
    createCallbackTaskWithUserId(&initMainMenuModeSelectMenuOptions, 0, 0x64, 0);
    setBootFadeColor(0x20, 0x40, 0x50);
    loadMainMenuSceneModelAnimationBank();
    initMainMenuSceneModel(4, 4);
    setMainMenuSceneModelAnimation(4, 5);
    setMainMenuSceneModelPosition(4, 0xFFE00000, 0, 0x509000);
    setMainMenuSceneModelRotation(4, 0, 0x100, 0);
    setCurrentGameTaskCallback(updateMainMenuModeSelect, 0);
    requestMusicSequenceBank(7);
}

void updateMainMenuModeSelect(void) {
    s32 temp_v0;

    gMenuFadeAlpha -= 0x10;
    if (gMenuFadeAlpha < 0) {
        gMenuFadeAlpha = 0;
        temp_v0 = gPlayerInputPressed;
        if (temp_v0 & 0x10800) {
            if (D_801235B4 != 0) {
                enqueueSoundEffect(0x19, 0x32);
                D_801235B4 -= 1;
                temp_v0 = gPlayerInputPressed;
            }
        }
        if ((temp_v0 & 0x20400) && (D_801235B4 != 2)) {
            enqueueSoundEffect(0x19, 0x32);
            D_801235B4 += 1;
            temp_v0 = gPlayerInputPressed;
        }
        if (temp_v0 & 0xD000) {
            enqueueSoundEffect(0x18, 0x32);
            if (gPlayerInputPressed & 0x4000) {
                D_801235B4 = 2;
            }
            gMainMenuSelectionResult = 1;
            if (D_801235B4 == 2) {
                requestMusicSequenceStop(0x3C);
            }
            setCurrentGameTaskCallback(fadeOutMainMenuModeSelect, 0);
        }
    }
    func_8006D780(0);
    loopMainMenuSceneModelAnimation(4);
    addMainMenuSceneModelDrawCallback(4);
    updateCallbackTasks();
}

void fadeOutMainMenuModeSelect(void) {
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0x100) {
        gMenuFadeAlpha = 0xFF;
        gFramebufferSwapHold = 1;
        setCurrentGameTaskCallback(exitMainMenuModeSelect, 0);
    }
    func_8006D780(0);
    loopMainMenuSceneModelAnimation(4);
    addMainMenuSceneModelDrawCallback(4);
    updateCallbackTasks();
}

void exitMainMenuModeSelect(void) {
    if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay = 0;
        setCurrentGameTaskCallback(initMainMenu, 0);
        if (D_801235B4 == 0) {
            createGameTask(4, startTrainingCourseFlow, 0x64);
            suspendGameTask(3);
            return;
        }
        if (D_801235B4 == 1) {
            createGameTask(4, startMainMenuModeDemoRaceFlow, 0x64);
            suspendGameTask(3);
        }
    }
}

void initMainMenuSettings(void) {
    resetAllViewports();
    func_80070C64(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    D_801124B8 = 0x80;
    gFramebufferSwapDelay = 0;
    loadCompressedRomAsset(D_593D10, D_598A70, 0x29);
    loadCompressedRomAsset(D_1F1A90, D_1F2220, 0x28);
    loadCompressedRomAsset(D_60F1A0, D_60F990, 0x2A);
    loadRawRomAsset(D_1467B0, D_147910, 8);
    loadCompressedRomAsset(D_1DE360, D_1E0F70, 9);
    func_8006D5CC();
    func_8006D520(0, 0x1F);
    initCallbackTaskScheduler(0);
    D_801235B4 = 0;
    gMainMenuSelectionResult = 0;
    createCallbackTaskWithUserId(&initMainMenuSettingsPanel, 0, 0x64, 0);
    createCallbackTaskWithUserId(&initTitleMenuBoardModels, 0, 0x64, 0);
    setBootFadeColor(0x20, 0x40, 0x50);
    setCurrentGameTaskCallback(&updateMainMenuSettings, 0);
    requestMusicSequenceBank(7);
}

void updateMainMenuSettings(void) {
    s32 temp_v1;

    gMenuFadeAlpha -= 0x10;
    if (gMenuFadeAlpha < 0) {
        gMenuFadeAlpha = 0;
        temp_v1 = gPlayerInputPressed;
        if ((temp_v1 & 0x10800) && (D_801235B4 != 0)) {
            D_801235B4 -= 1;
            enqueueSoundEffect(0x19, 0x32);
            temp_v1 = gPlayerInputPressed;
        }
        if ((temp_v1 & 0x20400) && (D_801235B4 != 3)) {
            D_801235B4 += 1;
            enqueueSoundEffect(0x19, 0x32);
            temp_v1 = gPlayerInputPressed;
        }
        if (temp_v1 & 0x4000) {
            D_801235B4 = 3;
        }
        if (temp_v1 & 0x40100) {
            switch (D_801235B4) {
            case 0:
                if (gRaceRecordSettingsEnabled != 0) {
                    gRaceRecordSettingsEnabled -= 1;
                    enqueueSoundEffect(0x19, 0x32);
                    temp_v1 = gPlayerInputPressed;
                }
                break;
            case 1:
                if (gRaceCourseModelEffectsDisabled != 1) {
                    gRaceCourseModelEffectsDisabled += 1;
                    enqueueSoundEffect(0x19, 0x32);
                    temp_v1 = gPlayerInputPressed;
                }
                break;
            case 2:
                if (gRaceCourseOverlayEffectsDisabled != 1) {
                    gRaceCourseOverlayEffectsDisabled += 1;
                    enqueueSoundEffect(0x19, 0x32);
                    temp_v1 = gPlayerInputPressed;
                }
                break;
            }
        }
        if (temp_v1 & 0x80200) {
            switch (D_801235B4) {
            case 0:
                if (gRaceRecordSettingsEnabled != 1) {
                    gRaceRecordSettingsEnabled += 1;
                    enqueueSoundEffect(0x19, 0x32);
                    temp_v1 = gPlayerInputPressed;
                }
                break;
            case 1:
                if (gRaceCourseModelEffectsDisabled != 0) {
                    gRaceCourseModelEffectsDisabled -= 1;
                    enqueueSoundEffect(0x19, 0x32);
                    temp_v1 = gPlayerInputPressed;
                }
                break;
            case 2:
                if (gRaceCourseOverlayEffectsDisabled != 0) {
                    gRaceCourseOverlayEffectsDisabled -= 1;
                    enqueueSoundEffect(0x19, 0x32);
                    temp_v1 = gPlayerInputPressed;
                }
                break;
            }
        }
        if ((temp_v1 & 0xD000) && (D_801235B4 == 3)) {
            requestMusicSequenceStop(0x3C);
            enqueueSoundEffect(0x18, 0x32);
            setCurrentGameTaskCallback(fadeOutMainMenuSettings, 0);
        }
    }
    func_8006D780(0);
    updateCallbackTasks();
}

void fadeOutMainMenuSettings(void) {
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0x100) {
        gMenuFadeAlpha = 0xFF;
        gFramebufferSwapHold = 1;
        setCurrentGameTaskCallback(exitMainMenuSettings, 0);
    }
    func_8006D780(0);
    updateCallbackTasks();
}

void exitMainMenuSettings(void) {
    if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay = 0;
        setCurrentGameTaskCallback(initMainMenu, 0);
    }
}

s32 checkMainMenuSecretCode(void) {
    switch (gMainMenuSecretCodeStep) {
    case 0:
        if (gPlayerInputHeld == 0x20000) {
            gMainMenuSecretCodeStep++;
        }
        goto end0;
    case 1:
        if ((gPlayerInputHeld != 0x20000) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x10000) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 2:
        if ((gPlayerInputHeld != 0x10000) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x400) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 3:
        if ((gPlayerInputHeld != 0x400) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x800) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 4:
        if ((gPlayerInputHeld != 0x800) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 4) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 5:
        if ((gPlayerInputHeld != 4) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 8) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 6:
        if ((gPlayerInputHeld != 8) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x20) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 7:
        if ((gPlayerInputHeld != 0x20) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x10) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 8:
        if ((gPlayerInputHeld != 0x10) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x2000) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 9:
        if ((gPlayerInputHeld != 0x2000) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x200) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 10:
        if ((gPlayerInputHeld != 0x200) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 1) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 11:
        if ((gPlayerInputHeld != 1) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x10000) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 12:
        if ((gPlayerInputHeld != 0x10000) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x4000) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 13:
        if ((gPlayerInputHeld != 0x4000) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x100) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 14:
        if ((gPlayerInputHeld != 0x100) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 2) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 15:
        if (gPlayerInputHeld != 2) {
            if (gPlayerInputHeld != 0) {
                if (gPlayerInputHeld == 0x1000) {
                    return 1;
                }
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    default:
    end0:
        return 0;
    }
}
