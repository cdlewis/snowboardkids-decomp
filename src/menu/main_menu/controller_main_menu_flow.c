#include "common.h"
#include "game/save_data.h"
#include "assets.h"
#include "game/audio/sound_manager.h"
#include "game/engine/callback_task_scheduler.h"
#include "game/engine/asset_manager.h"
#include "game/engine/system_runtime.h"
#include "game/engine/game_task_scheduler.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/menu/controller_pak/controller_pak_menu.h"
#include "game/menu/renderer/menu_screen_effects.h"
#include "game/menu/main_menu/main_menu_panel_ui.h"
#include "game/menu/main_menu/main_menu_scene_model.h"
#include "game/menu/main_menu/main_menu_scene_model_renderer.h"
#include "game/menu/main_menu/main_menu_race_flow.h"
#include "game/menu/main_menu/main_menu_title_ui.h"
#include "game/race/camera/race_camera.h"
#include "game/race/flow/race_flow.h"
#include "game/demo/main_menu_demo_race_intro.h"
#include "game/demo/title_demo_race_intro.h"
#include "game/engine/viewport_manager.h"
#include "game/menu/renderer/menu_renderer.h"

#define OS_MESG_BLOCK 1

typedef struct OSContPad {
    u16 button;
    s8 stick_x;
    s8 stick_y;
    u8 errno;
} OSContPad;

typedef struct SaveFileIdentity {
    s32 size;
    u32 gameCode;
    u16 companyCode;
    char extName[4];
    char gameName[16];
} SaveFileIdentity;

#define CONTROLLER_PAK_SAVE_READ_SIZE 0x78E0
#define CONTROLLER_PAK_CHECKSUM_START_OFFSET 4
#define CONTROLLER_PAK_MAX_READ_RETRIES 3

#define CONTROLLER_PAK_SAVE_FIELD_OFFSET(field) ((u32)&(((GameSaveData *)0)->field))
#define CONTROLLER_PAK_RECORD_AT(cursor, field) \
    (*(GameSaveRecordTime *)((cursor) + CONTROLLER_PAK_SAVE_FIELD_OFFSET(field)))
#define CONTROLLER_PAK_U8_AT(cursor, field) (*(u8 *)((cursor) + CONTROLLER_PAK_SAVE_FIELD_OFFSET(field)))
#define CONTROLLER_PAK_U16_AT(cursor, field) (*(u16 *)((cursor) + CONTROLLER_PAK_SAVE_FIELD_OFFSET(field)))

extern s32 gRumbleMotorStatuses[4];
extern s16 gRumbleMotorRequestStates[4];

extern s32 osMotorInit(OSMesgQueue *, OSPfs *, s32);
extern s32 osMotorStart(OSPfs *);
extern s32 osMotorStop(OSPfs *);
extern void releaseMenuAssetHandles(void);
extern void enqueueSoundEffect(s32, s32);
extern OSMesgQueue gControllerSubsystemRequestQueue;
extern OSMesgQueue gControllerSubsystemReplyQueue;
extern OSMesgQueue gControllerEventQueue;
extern s16 gControllerEventMessage;
extern OSContPad gControllerPads[];
extern s32 gControllerPakFileNos[];
extern OSPfs gControllerPakHandles[];
extern OSPfs gRumblePakHandles[];
extern SaveFileIdentity gControllerPakSaveFileIdentity;
extern u8 gControllerPakGameName[];
extern u8 gControllerPakExtName[];
extern u8 gConnectedControllerBitmask;
extern u8 gControllerPakSaveGameNameBytes[];
extern u8 gControllerPakSaveGameNameBytesEnd[];
extern u8 gControllerPakSaveExtNameBytes[];
extern u8 gControllerPakSaveExtNameBytesEnd[];
extern u8 gMainMenuReturnFromRace;
extern u8 gControllerReadPending;
extern s32 gControllerPakFileNos[];
extern u8 gControllerPakGameName[];
extern u8 gControllerPakExtName[];
extern u8 gRumblePakConnectedByController[];
extern u8 gControllerPakOperationCounts[];
extern s16 gMenuFadeAlpha;
extern u8 gMainMenuSecretCodeStep;
extern u8 gConnectedControllerCount;
extern u8 gRaceRumbleEnabled;
extern u8 gRumblePakConnectedMask;
extern s32 gControllerPakFreeBytes;
extern s32 gControllerPakFreeFileCount;
extern OSMesgQueue gControllerInputUpdateQueue;
extern u8 gPendingFramebufferSwapCount;
extern u8 gFramebufferSwapHold;
extern u8 gRaceRecordSettingsEnabled;
extern u8 gRaceCourseModelEffectsDisabled;
extern u8 gRaceCourseOverlayEffectsDisabled;
extern s32 gMenuFlowState;
extern s32 gPlayerInputPressed;
extern u8 gControllerPakSaveGameNameBytes[];
extern u8 gControllerPakSaveExtNameBytes[];
extern u8 gControllerPakSaveExtNameBytesEnd[];

void requestControllerRead(void) {
    if ((gControllerReadPending == 0) && (gConnectedControllerBitmask != 0)) {
        osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)CONTROLLER_REQUEST_READ_INPUT, OS_MESG_BLOCK);
        gControllerReadPending = 1;
    }
}

void updateControllerInputState(void) {
    u16 i;
 do { i = 0; do { if ((((s32) gConnectedControllerBitmask) >> i) & 1) { if (gControllerPads[i].errno == 0) { gControllerInputState[i] = *((ControllerInputState *) (&gControllerPads[i])); } } i++; } while (i < 4); gControllerReadPending = 0; } while (0);
}

void requestRumbleMotorInit(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + CONTROLLER_REQUEST_INIT_RUMBLE), OS_MESG_BLOCK);
    osRecvMesg(&gControllerSubsystemReplyQueue, &msg, OS_MESG_BLOCK);
}

extern void requestRumbleMotorInitWithContext(u16 controllerIndex, s32 playerCount, s32 choiceValue);
#pragma weak requestRumbleMotorInitWithContext = requestRumbleMotorInit

void serviceRumbleMotorRequest(u16 arg0) {
    if (gRumbleMotorStatuses[arg0] == RUMBLE_MOTOR_NO_PAK) {
        gRumbleMotorRequestStates[arg0] = 0;
        if (gRumblePakConnectedMask & (1 << arg0)) {
            osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + CONTROLLER_REQUEST_RETRY_RUMBLE_INIT),
                       OS_MESG_BLOCK);
        }
    } else if (gRumbleMotorStatuses[arg0] == RUMBLE_MOTOR_WRONG_DEVICE) {
        gRumbleMotorRequestStates[arg0] = 0;
        if (gRumblePakConnectedMask & (1 << arg0)) {
            osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + CONTROLLER_REQUEST_RETRY_RUMBLE_INIT),
                       OS_MESG_BLOCK);
        }
    } else if (gRumbleMotorStatuses[arg0] == RUMBLE_MOTOR_CONTROLLER_FAILURE) {
        gRumbleMotorRequestStates[arg0] = 0;
        if (gRumblePakConnectedMask & (1 << arg0)) {
            osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + CONTROLLER_REQUEST_RETRY_RUMBLE_INIT),
                       OS_MESG_BLOCK);
        }
    } else {
        if (gRumbleMotorRequestStates[arg0] == 0) {
            osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + CONTROLLER_REQUEST_STOP_RUMBLE),
                       OS_MESG_BLOCK);
        } else {
            osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + CONTROLLER_REQUEST_START_RUMBLE),
                       OS_MESG_BLOCK);
        }
        gRumbleMotorRequestStates[arg0] = 0;
    }
}

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

    osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + CONTROLLER_REQUEST_PROBE_PAK), OS_MESG_BLOCK);
    osRecvMesg(&gControllerSubsystemReplyQueue, &msg, OS_MESG_BLOCK);
}

void probeControllerPak(u16 arg0) {
    u32 ret;

    ret = osPfsInitPak(&gControllerEventQueue, &gControllerPakHandles[arg0], arg0);
    if (ret == 2) {
        ret = osPfsInitPak(&gControllerEventQueue, &gControllerPakHandles[arg0], arg0);
    }

    if (ret == 0) {
        gControllerPakStatusCodes[arg0] = 1;
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

void requestControllerPakSaveStatus(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + CONTROLLER_REQUEST_CHECK_SAVE), OS_MESG_BLOCK);
    osRecvMesg(&gControllerSubsystemReplyQueue, &msg, OS_MESG_BLOCK);
}

extern void requestControllerPakSaveStatusWithContext(u16 controllerIndex, s32 playerCount, s32 choiceValue);
#pragma weak requestControllerPakSaveStatusWithContext = requestControllerPakSaveStatus

void checkControllerPakSaveStatus(u16 arg0) {
    s32 ret;
    s32 maxFiles;
    s32 filesUsed;
    s32 freeBytes;
    s32 i;

    gControllerPakSaveFileIdentity.size = 0x7900;
    gControllerPakSaveFileIdentity.gameCode = 'NSKE';
    gControllerPakSaveFileIdentity.companyCode = 'EB';

    i = 0;
    do {
        gControllerPakSaveFileIdentity.extName[i] = gControllerPakSaveGameNameBytesEnd[i];
        i++;
    } while (i < 4);

    i = 0;
    do {
        gControllerPakSaveFileIdentity.gameName[i] = gControllerPakSaveGameNameBytes[i];
        i++;
    } while (i < 16);

    osPfsInitPak(&gControllerEventQueue, &gControllerPakHandles[arg0], arg0);

    ret = osPfsFindFile(&gControllerPakHandles[arg0], gControllerPakSaveFileIdentity.companyCode,
                        gControllerPakSaveFileIdentity.gameCode, gControllerPakExtName, gControllerPakGameName,
                        &gControllerPakFileNos[arg0]);
    if (ret == 0) {
        gControllerPakStatusCodes[arg0] = 2;
    } else {
        osPfsNumFiles(&gControllerPakHandles[arg0], &maxFiles, &filesUsed);
        if (filesUsed == 0x10) {
            gControllerPakStatusCodes[arg0] = 0xC;
        } else {
            osPfsFreeBlocks(&gControllerPakHandles[arg0], &freeBytes);
            maxFiles = freeBytes / 256;
            if (maxFiles < 0x79) {
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

void requestControllerPakSaveRead(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + CONTROLLER_REQUEST_READ_SAVE), OS_MESG_BLOCK);
    osRecvMesg(&gControllerSubsystemReplyQueue, &msg, OS_MESG_BLOCK);
}

extern void requestControllerPakSaveReadWithContext(u16 controllerIndex, s32 playerCount, s32 choiceValue);
#pragma weak requestControllerPakSaveReadWithContext = requestControllerPakSaveRead

// readControllerPakSave best match: 99.679% at nonmatchings/readControllerPakSave-8498672362023432715/base_28.c
#pragma GLOBAL_ASM("asm/nonmatchings/menu/main_menu/controller_main_menu_flow/readControllerPakSave.s")

#ifdef NON_MATCHING
extern u8 gControllerPakSaveFileIdentityExtAlias[];
extern u8 gControllerPakSaveFileIdentityGameAlias[];

void readControllerPakSave(u16 controllerIndex) {
    union CopyOffset {
        u8 *extSrc;
        s32 offset;
    } work;
    u8 *src;
    s32 readStatus;
    u16 checksumFailed;
    s32 channel;
    u8 *dst;

    checksumFailed = 0;
    osPfsInitPak(&gControllerEventQueue, &gControllerPakHandles[controllerIndex],
                 (channel = controllerIndex & ((short)0xFFFF)));

    gControllerPakSaveFileIdentity.gameCode = 'NSKE';
    gControllerPakSaveFileIdentity.companyCode = 'EB';

    dst = gControllerPakSaveFileIdentityExtAlias;
    work.extSrc = gControllerPakSaveGameNameBytesEnd;
copy_ext:
    dst[10] = *work.extSrc;
    work.extSrc++;
    dst++;
    if (work.extSrc < gControllerPakSaveExtNameBytesEnd) {
        goto copy_ext;
    }

    dst = gControllerPakSaveFileIdentityGameAlias;
    src = gControllerPakSaveGameNameBytes;
copy_name:
    dst[14] = *src;
    src++;
    dst++;
    if (src < gControllerPakSaveExtNameBytes) {
        goto copy_name;
    }

    osPfsFindFile(&gControllerPakHandles[controllerIndex], gControllerPakSaveFileIdentity.companyCode,
                  gControllerPakSaveFileIdentity.gameCode, gControllerPakExtName,
                  gControllerPakGameName, &gControllerPakFileNos[controllerIndex]);
    readStatus = osPfsReadWriteFile(&gControllerPakHandles[controllerIndex], gControllerPakFileNos[controllerIndex],
                                    0, 0, 0x78E0, (u8 *)&GAME_SAVE_DATA_SLOT(controllerIndex));
    if (readStatus == 0) {
        dst = NULL;
        src = GAME_SAVE_DATA_SLOT(controllerIndex).bytes;
        work.offset = 4;
checksum_loop:
        dst += src[0];
        dst += src[1];
        dst += src[2];
        dst += src[3];
        work.offset += 4;
        src += 4;
        if ((work.offset | 0) != 0x78E0) {
            goto checksum_loop;
        }
        if ((s32)dst != GAME_SAVE_DATA_SLOT(controllerIndex).checksum) {
            checksumFailed = 1;
        }

        if (checksumFailed == 0) {
            if (validateControllerPakSave(channel) == 0) {
                gControllerPakRetryCounts[controllerIndex] = 0;
            }
        } else {
            gControllerPakRetryCounts[controllerIndex]++;
        }
    } else {
        gControllerPakRetryCounts[controllerIndex]++;
    }

    if ((readStatus != 0) || (gControllerPakRetryCounts[controllerIndex] != 0)) {
        if (gControllerPakRetryCounts[controllerIndex] != 3) {
            return;
        }
    }
    gControllerPakOperationCounts[controllerIndex]++;
}
#endif

void requestControllerPakSaveWrite(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + CONTROLLER_REQUEST_WRITE_SAVE), OS_MESG_BLOCK);
    osRecvMesg(&gControllerSubsystemReplyQueue, &msg, OS_MESG_BLOCK);
}

// writeControllerPakSave best match: 99.743% at nonmatchings/writeControllerPakSave-1213871690025509423/base_34.c
#pragma GLOBAL_ASM("asm/nonmatchings/menu/main_menu/controller_main_menu_flow/writeControllerPakSave.s")

#ifdef NON_MATCHING
void writeControllerPakSave(u16 controllerIndex) {
    union CopyOffset {
        u8 *copy;
        s32 offset;
    } work;
    s32 *newFileNo;
    OSPfs *volatile pfs;
    s32 *fileNo;
    GameSaveData *save;
    u8 *src;
    s32 i;
    s32 checksum;

    gControllerPakSaveFileIdentity.size = 0x7900;
    gControllerPakSaveFileIdentity.gameCode = 'NSKE';
    gControllerPakSaveFileIdentity.companyCode = 'EB';

    i = 0;
    do {
        gControllerPakSaveFileIdentity.extName[i] =
            gControllerPakSaveGameNameBytesEnd[i];
        i++;
    } while (&gControllerPakSaveGameNameBytesEnd[i] <
             gControllerPakSaveExtNameBytesEnd);

    i = 0;
    do {
        gControllerPakSaveFileIdentity.gameName[i] =
            gControllerPakSaveGameNameBytes[i];
        i++;
    } while (&gControllerPakSaveGameNameBytes[i] <
             gControllerPakSaveGameNameBytesEnd);

    newFileNo = &gControllerPakFileNos[controllerIndex];
    osPfsInitPak(&gControllerEventQueue,
                 (pfs = &gControllerPakHandles[controllerIndex]), controllerIndex);
    fileNo = newFileNo;

    if (osPfsFindFile(pfs, gControllerPakSaveFileIdentity.companyCode,
                      gControllerPakSaveFileIdentity.gameCode, gControllerPakExtName,
                      gControllerPakGameName, fileNo) == 5) {
        osPfsAllocateFile(pfs, gControllerPakSaveFileIdentity.companyCode,
                          gControllerPakSaveFileIdentity.gameCode, gControllerPakExtName,
                          gControllerPakGameName, 0x7900, fileNo);
    }

    save = &GAME_SAVE_DATA_SLOT(controllerIndex);
    checksum = 0;
    src = save->bytes;
    i = 4;
    do {
        checksum += *src++;
        i++;
    } while (i != 0x78E0);
    GAME_SAVE_DATA_SLOT(controllerIndex).checksum = checksum;

    if (osPfsReadWriteFile(pfs, *fileNo, 1, 0, 0x78E0, (u8 *)save) == 0) {
        gControllerPakRetryCounts[controllerIndex] = 0;
    } else {
        gControllerPakRetryCounts[controllerIndex]++;
    }
}
#endif

void requestControllerPakRepair(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + CONTROLLER_REQUEST_REPAIR_PAK), OS_MESG_BLOCK);
    osRecvMesg(&gControllerSubsystemReplyQueue, &msg, OS_MESG_BLOCK);
}

extern void requestControllerPakRepairWithContext(u16 controllerIndex, s32 playerCount, s32 choiceValue);
#pragma weak requestControllerPakRepairWithContext = requestControllerPakRepair

void repairControllerPakId(u16 arg0) {
    OSPfs **sp18;
    OSPfs *pfs;
    s32 ret;

    pfs = &gControllerPakHandles[arg0];
    sp18 = &pfs;
    osPfsInitPak(&gControllerEventQueue, *sp18, arg0);
    ret = osPfsRepairId(pfs);
    if ((ret == 4) || (ret == 0xA)) {
        gControllerPakRetryCounts[arg0] += 1;
    }
}

void requestControllerPakFileList(void) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)CONTROLLER_REQUEST_LIST_FILES, OS_MESG_BLOCK);
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

    osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)(arg0 + CONTROLLER_REQUEST_DELETE_FILE), OS_MESG_BLOCK);
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
            gControllerPakRetryCounts[0] = 0;
            return;
        }
        gControllerPakRetryCounts[0]++;
    }
}

void requestControllerPakFreeSpaceUpdate(void) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&gControllerSubsystemRequestQueue, (OSMesg)CONTROLLER_REQUEST_UPDATE_FREE_SPACE, OS_MESG_BLOCK);
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

// validateControllerPakSave best match: 99.306% at nonmatchings/validateControllerPakSave-8239461464121803931/base_23.c
#pragma GLOBAL_ASM("asm/nonmatchings/menu/main_menu/controller_main_menu_flow/validateControllerPakSave.s")

#ifdef NON_MATCHING
u16 validateControllerPakSave(volatile s32 arg0) {
    u8 *nextByte;
    s32 wordCursorValid;
    u8 *save;
    u32 *wordCursor;
    u8 *byteCursor;
    s32 offset;
    s32 i;
    s32 firstByteStep;
    s32 secondByteStep;
    s32 count;

    save = (u8 *)&GAME_SAVE_DATA_SLOT(arg0 & 0xFFFFFFFFFFFFFFFF);
    wordCursor = (u32 *)save;
    byteCursor = save;
    offset = 0;
    do {
        offset += 4;
        firstByteStep = (byteCursor + 1) - byteCursor;
        wordCursor++;
        byteCursor++;
        secondByteStep = firstByteStep && firstByteStep;
        if (secondByteStep) {
        }
    } while (offset < 0x2C);

    i = 0;
    wordCursorValid = wordCursor && wordCursor;
    byteCursor = save;
    offset += firstByteStep - 1;
    count = 0xC;
    do {
        i++;
        nextByte = byteCursor + 1;
        secondByteStep = nextByte - byteCursor;
        firstByteStep = wordCursorValid;
        if (secondByteStep) {
        }
        if ((firstByteStep ^ 0) && wordCursor) {
        }
        if (save && save) {
        }
        byteCursor++;
    } while (i != count);
    i += secondByteStep - 1;

    return validateControllerPakSaveData(arg0);
}
#endif

s32 validateControllerPakSaveData(s32 channel) {
    s32 pad;
    volatile s32 unusedInvalidCourse;
    u8 *scoreAndCharacterCursor;
    u8 *trickScoreCursor;
    u8 *saveStart;
    u8 *timeTrialCourseCursor;
    u8 *scoreCourseCursor;
    u8 *raceCourseCursor;
    u8 *recordCursor;
    s16 fraction;
    s16 raceFraction;
    s16 targetFraction;
    s32 playerCounter;
    s32 courseOffset;
    s32 invalidSave;
    s8 minutes;
    s8 seconds;
    s8 raceMinutes;
    s8 raceSeconds;
    s8 targetMinutes;
    s8 targetSeconds;
    s32 trickScore;
    s32 trickCharacter;
    s32 timeTrialCharacter;
    s32 raceCharacter;
    s32 scoreCharacter;

    saveStart = (u8 *)&GAME_SAVE_DATA_SLOT(channel);
    invalidSave = 0;
    timeTrialCourseCursor = saveStart;
    scoreCourseCursor = saveStart;
    raceCourseCursor = saveStart;
    courseOffset = 0;
    saveStart += 0;

    do {
        unusedInvalidCourse = 0;
        recordCursor = timeTrialCourseCursor;
        scoreAndCharacterCursor = scoreCourseCursor;
        playerCounter = 0;
        trickScoreCursor = raceCourseCursor;
loop_player:
        minutes = CONTROLLER_PAK_RECORD_AT(recordCursor, timeTrialRecords).minutes;
        playerCounter += 2;
        if ((minutes < 0) || (minutes >= 100)) {
            invalidSave = 1;
        }
        seconds = CONTROLLER_PAK_RECORD_AT(recordCursor, timeTrialRecords).seconds;
        if ((seconds < 0) || (seconds >= 60)) {
            invalidSave = 1;
        }
        fraction = CONTROLLER_PAK_RECORD_AT(recordCursor, timeTrialRecords).fraction;
        if ((fraction < 0) || (fraction >= 0x6301)) {
            invalidSave = 1;
        }
        timeTrialCharacter = CONTROLLER_PAK_U8_AT(scoreAndCharacterCursor, timeTrialCharacterIds);
        if ((timeTrialCharacter & 7) >= 6) {
            invalidSave = 1;
        }
        if ((((s32)timeTrialCharacter >> 3) & 0xF) >= 0xF) {
            invalidSave = 1;
        }
        raceMinutes = CONTROLLER_PAK_RECORD_AT(recordCursor, raceRecords).minutes;
        if ((raceMinutes < 0) || (raceMinutes >= 100)) {
            invalidSave = 1;
        }
        raceSeconds = CONTROLLER_PAK_RECORD_AT(recordCursor, raceRecords).seconds;
        if ((raceSeconds < 0) || (raceSeconds >= 60)) {
            invalidSave = 1;
        }
        raceFraction = CONTROLLER_PAK_RECORD_AT(recordCursor, raceRecords).fraction;
        recordCursor += sizeof(GameSaveRecordTime);
        if ((raceFraction < 0) || (raceFraction >= 0x6301)) {
            invalidSave = 1;
        }
        raceCharacter = CONTROLLER_PAK_U8_AT(scoreAndCharacterCursor, raceRecordCharacterIds);
        if ((raceCharacter & 7) >= 6) {
            invalidSave = 1;
        }
        if ((((s32)raceCharacter >> 3) & 0xF) >= 0xF) {
            invalidSave = 1;
        }
        if (courseOffset == 0x24) {
            if (CONTROLLER_PAK_U8_AT(scoreAndCharacterCursor, scoreAttackScores) >= 0x1F) {
                goto block_32;
            }
        } else if (CONTROLLER_PAK_U8_AT(scoreAndCharacterCursor, scoreAttackScores) >= 0x3D) {
block_32:
            invalidSave = 1;
        }
        scoreCharacter = CONTROLLER_PAK_U8_AT(scoreAndCharacterCursor, scoreAttackCharacterIds);
        if ((scoreCharacter & 7) >= 6) {
            invalidSave = 1;
        }
        if ((((s32)scoreCharacter >> 3) & 0xF) >= 0xF) {
            invalidSave = 1;
        }
        trickScore = (CONTROLLER_PAK_U16_AT(trickScoreCursor, trickAttackScores)) & 0xFFFFu;
        trickScoreCursor += sizeof(u16);
        if ((s32)trickScore >= 0x2710) {
            invalidSave = 1;
        }
        trickCharacter = CONTROLLER_PAK_U8_AT(scoreAndCharacterCursor, trickAttackCharacterIds);
        if ((trickCharacter & 7) >= 6) {
            invalidSave = 1;
        }
        if ((((s32)trickCharacter >> 3) & 0xF) >= 0xF) {
            invalidSave = 1;
        }
        scoreAndCharacterCursor += 1;
        if (playerCounter != 0xA) {
            goto loop_player;
        }
        targetMinutes = CONTROLLER_PAK_RECORD_AT(saveStart, bestLapRecords).minutes;
        timeTrialCourseCursor += sizeof(GameSaveRecordTime) * 5;
        scoreCourseCursor += 5;
        if ((targetMinutes < 0) || (targetMinutes >= 100)) {
            invalidSave = 1;
        }
        targetSeconds = CONTROLLER_PAK_RECORD_AT(saveStart, bestLapRecords).seconds;
        raceCourseCursor += sizeof(u16) * 5;
        courseOffset += 4;
        if ((targetSeconds < 0) || (targetSeconds >= 60)) {
            invalidSave = 1;
        }
        targetFraction = CONTROLLER_PAK_RECORD_AT(saveStart, bestLapRecords).fraction;
        if ((targetFraction < 0) || (targetFraction >= 0x6301)) {
            invalidSave = 1;
        }
        saveStart += sizeof(GameSaveRecordTime);
    } while (courseOffset != 0x2C);

    return invalidSave;
}

void enterMainMenuFromRace(void) {
    gMainMenuReturnFromRace = 1;
    setCurrentGameTaskCallback(initMainMenu, 0);
    createGameTask(4, initTitleDemoRaceIntro, 0x64);
    suspendGameTask(3);
}

void initMainMenu(void) {
    gMainMenuSecretCodeUnlocked = 0;
    gMainMenuSecretCodeStep = 0;
    resetAllViewports();
    configureMenuViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    gFramebufferSwapDelay.value = 0;
    LOAD_ASSET(_5DB9D0, 0x21);
    LOAD_ASSET(_5DCBE0, 0x25);
    LOAD_ASSET(_593D10, 0x22);
    LOAD_RAW_ASSET(_1467B0, 8);
    LOAD_ASSET(_1DE360, 9);
    gCurrentGameTask->fade = 0xFF;
    gCurrentGameTask->selection = 0;
    gCurrentGameTask->delay = 0x32;
    gCurrentGameTask->mainMenuTimer = 0x4B0;
    initCallbackTaskScheduler(0);
    if (gConnectedControllerCount != 0) {
        createCallbackTask((CallbackTaskCallback)&initMainMenuTitleCursor, 0, 0x64);
    } else {
        createCallbackTask((CallbackTaskCallback)&updateTitleScreenStartPrompt, 0, 0x64);
    }
    createCallbackTask((CallbackTaskCallback)&initMainMenuTitleOptions, 0, 0x64);
    createCallbackTask((CallbackTaskCallback)&initMainMenuTitleLogo, 0, 0x64);
    createCallbackTaskWithUserId((CallbackTaskCallback)&initTitleMenuSparkle, 0, 0x64, 0);
    createCallbackTaskWithUserId((CallbackTaskCallback)initMainMenuBoardModels, 0, 0x64, 0);
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
    resetRaceCameras();
    setRaceCameraMode(0, 0x1F);
    gMenuFadeAlpha = (s16) gCurrentGameTask->fade;
    gMenuFadeOverlayActive = 1;
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
            gMenuFadeOverlayActive = 0;
        }
    } else {
        if (gCurrentGameTask->delay != 0) {
            gCurrentGameTask->delay -= 1;
            if (gCurrentGameTask->delay == 0) {
                requestMusicSequenceBank(0);
            }
        }
        temp_a0 = gPlayerInputPressed;
        if (temp_a0 & (STICK_UP | U_JPAD)) {
            if (gCurrentGameTask->selection != 0) {
                gCurrentGameTask->selection -= 1;
                enqueueSoundEffect(0x19, 0x32);
                temp_a0 = gPlayerInputPressed;
            }
        }
        if (temp_a0 & (STICK_DOWN | D_JPAD)) {
            if (gCurrentGameTask->selection != 2) {
                gCurrentGameTask->selection += 1;
                enqueueSoundEffect(0x19, 0x32);
                temp_a0 = gPlayerInputPressed;
            }
        }
        if ((temp_a0 & START_BUTTON) || (temp_a0 & A_BUTTON)) {
            flag = 1;
            enqueueSoundEffect(1, 0x32);
        }
        if (flag == 0) {
            gCurrentGameTask->mainMenuTimer -= 1;
            if (gCurrentGameTask->mainMenuTimer == 0) {
                flag = 1;
            }
        }
        if (flag != 0) {
            setCurrentGameTaskCallback(fadeOutMainMenu, 0);
            requestMusicSequenceStop(0xC);
        }
    }
    updateRaceCamera(0);
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
            updateRaceCamera(0);
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
        gFramebufferSwapDelay.value = 0;
        if (gCurrentGameTask->mainMenuTimer != 0) {
            temp_v1 = gCurrentGameTask->selection;
            if (temp_v1 == 0) {
                createGameTask(2, initNewGameSaveData, 0x64);
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
            createGameTask(4, initMainMenuDemoRaceIntro, 0x64);
            suspendGameTask(3);
        }
    }
}

void initMainMenuModeSelect(void) {
    resetAllViewports();
    gViewportStates[0].overlayAlpha = 0x80;
    gFramebufferSwapDelay.value = 0;
    configureMenuViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    LOAD_RAW_ASSET(_1467B0, 8);
    LOAD_ASSET(_1DE360, 9);
    LOAD_ASSET(_1F1A90, 0x28);
    resetRaceCameras();
    setRaceCameraMode(0, 0x1F);
    initCallbackTaskScheduler(0);
    createCallbackTaskWithUserId((CallbackTaskCallback)initMainMenuBoardModels, 0, 0x64, 0);
    gMenuFlowState = 0;
    gMainMenuSelectionResult = 0;
    createCallbackTaskWithUserId((CallbackTaskCallback)&initMainMenuModeSelectMenuOptions, 0, 0x64, 0);
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
        if (temp_v0 & (STICK_UP | U_JPAD)) {
            if (gMenuFlowState != 0) {
                enqueueSoundEffect(0x19, 0x32);
                gMenuFlowState -= 1;
                temp_v0 = gPlayerInputPressed;
            }
        }
        if ((temp_v0 & (STICK_DOWN | D_JPAD)) && (gMenuFlowState != 2)) {
            enqueueSoundEffect(0x19, 0x32);
            gMenuFlowState += 1;
            temp_v0 = gPlayerInputPressed;
        }
        if (temp_v0 & (A_BUTTON | B_BUTTON | START_BUTTON)) {
            enqueueSoundEffect(0x18, 0x32);
            if (gPlayerInputPressed & B_BUTTON) {
                gMenuFlowState = 2;
            }
            gMainMenuSelectionResult = 1;
            if (gMenuFlowState == 2) {
                requestMusicSequenceStop(0x3C);
            }
            setCurrentGameTaskCallback(fadeOutMainMenuModeSelect, 0);
        }
    }
    updateRaceCamera(0);
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
    updateRaceCamera(0);
    loopMainMenuSceneModelAnimation(4);
    addMainMenuSceneModelDrawCallback(4);
    updateCallbackTasks();
}

void exitMainMenuModeSelect(void) {
    if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay.value = 0;
        setCurrentGameTaskCallback(initMainMenu, 0);
        if (gMenuFlowState == 0) {
            createGameTask(4, startTrainingCourseFlow, 0x64);
            suspendGameTask(3);
            return;
        }
        if (gMenuFlowState == 1) {
            createGameTask(4, startMainMenuModePreviewRaceFlow, 0x64);
            suspendGameTask(3);
        }
    }
}

void initMainMenuSettings(void) {
    resetAllViewports();
    configureMenuViewport(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    gViewportStates[0].overlayAlpha = 0x80;
    gFramebufferSwapDelay.value = 0;
    LOAD_ASSET(_593D10, 0x29);
    LOAD_ASSET(_1F1A90, 0x28);
    LOAD_ASSET(_60F1A0, 0x2A);
    LOAD_RAW_ASSET(_1467B0, 8);
    LOAD_ASSET(_1DE360, 9);
    resetRaceCameras();
    setRaceCameraMode(0, 0x1F);
    initCallbackTaskScheduler(0);
    gMenuFlowState = 0;
    gMainMenuSelectionResult = 0;
    createCallbackTaskWithUserId((CallbackTaskCallback)&initMainMenuSettingsPanel, 0, 0x64, 0);
    createCallbackTaskWithUserId((CallbackTaskCallback)&initMainMenuBoardModels, 0, 0x64, 0);
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
        if ((temp_v1 & (STICK_UP | U_JPAD)) && (gMenuFlowState != 0)) {
            gMenuFlowState -= 1;
            enqueueSoundEffect(0x19, 0x32);
            temp_v1 = gPlayerInputPressed;
        }
        if ((temp_v1 & (STICK_DOWN | D_JPAD)) && (gMenuFlowState != 3)) {
            gMenuFlowState += 1;
            enqueueSoundEffect(0x19, 0x32);
            temp_v1 = gPlayerInputPressed;
        }
        if (temp_v1 & B_BUTTON) {
            gMenuFlowState = 3;
        }
        if (temp_v1 & (STICK_RIGHT | R_JPAD)) {
            switch (gMenuFlowState) {
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
        if (temp_v1 & (STICK_LEFT | L_JPAD)) {
            switch (gMenuFlowState) {
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
        if ((temp_v1 & (A_BUTTON | B_BUTTON | START_BUTTON)) && (gMenuFlowState == 3)) {
            requestMusicSequenceStop(0x3C);
            enqueueSoundEffect(0x18, 0x32);
            setCurrentGameTaskCallback(fadeOutMainMenuSettings, 0);
        }
    }
    updateRaceCamera(0);
    updateCallbackTasks();
}

void fadeOutMainMenuSettings(void) {
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0x100) {
        gMenuFadeAlpha = 0xFF;
        gFramebufferSwapHold = 1;
        setCurrentGameTaskCallback(exitMainMenuSettings, 0);
    }
    updateRaceCamera(0);
    updateCallbackTasks();
}

void exitMainMenuSettings(void) {
    if (gPendingFramebufferSwapCount == 2) {
        releaseMenuAssetHandles();
        gFramebufferSwapHold = 0;
        gFramebufferSwapDelay.value = 0;
        setCurrentGameTaskCallback(initMainMenu, 0);
    }
}

s32 checkMainMenuSecretCode(void) {
    switch (gMainMenuSecretCodeStep) {
    case 0:
        if (gPlayerInputHeld[0] == STICK_DOWN) {
            gMainMenuSecretCodeStep++;
        }
        goto end0;
    case 1:
        if ((gPlayerInputHeld[0] != STICK_DOWN) && (gPlayerInputHeld[0] != 0)) {
            if (gPlayerInputHeld[0] == STICK_UP) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 2:
        if ((gPlayerInputHeld[0] != STICK_UP) && (gPlayerInputHeld[0] != 0)) {
            if (gPlayerInputHeld[0] == D_JPAD) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 3:
        if ((gPlayerInputHeld[0] != D_JPAD) && (gPlayerInputHeld[0] != 0)) {
            if (gPlayerInputHeld[0] == U_JPAD) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 4:
        if ((gPlayerInputHeld[0] != U_JPAD) && (gPlayerInputHeld[0] != 0)) {
            if (gPlayerInputHeld[0] == D_CBUTTONS) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 5:
        if ((gPlayerInputHeld[0] != D_CBUTTONS) && (gPlayerInputHeld[0] != 0)) {
            if (gPlayerInputHeld[0] == U_CBUTTONS) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 6:
        if ((gPlayerInputHeld[0] != U_CBUTTONS) && (gPlayerInputHeld[0] != 0)) {
            if (gPlayerInputHeld[0] == L_TRIG) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 7:
        if ((gPlayerInputHeld[0] != L_TRIG) && (gPlayerInputHeld[0] != 0)) {
            if (gPlayerInputHeld[0] == R_TRIG) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 8:
        if ((gPlayerInputHeld[0] != R_TRIG) && (gPlayerInputHeld[0] != 0)) {
            if (gPlayerInputHeld[0] == Z_TRIG) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 9:
        if ((gPlayerInputHeld[0] != Z_TRIG) && (gPlayerInputHeld[0] != 0)) {
            if (gPlayerInputHeld[0] == L_JPAD) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 10:
        if ((gPlayerInputHeld[0] != L_JPAD) && (gPlayerInputHeld[0] != 0)) {
            if (gPlayerInputHeld[0] == R_CBUTTONS) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 11:
        if ((gPlayerInputHeld[0] != R_CBUTTONS) && (gPlayerInputHeld[0] != 0)) {
            if (gPlayerInputHeld[0] == STICK_UP) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 12:
        if ((gPlayerInputHeld[0] != STICK_UP) && (gPlayerInputHeld[0] != 0)) {
            if (gPlayerInputHeld[0] == B_BUTTON) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 13:
        if ((gPlayerInputHeld[0] != B_BUTTON) && (gPlayerInputHeld[0] != 0)) {
            if (gPlayerInputHeld[0] == R_JPAD) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 14:
        if ((gPlayerInputHeld[0] != R_JPAD) && (gPlayerInputHeld[0] != 0)) {
            if (gPlayerInputHeld[0] == L_CBUTTONS) {
                gMainMenuSecretCodeStep++;
            } else {
                gMainMenuSecretCodeStep = -1;
            }
        }
        goto end0;
    case 15:
        if (gPlayerInputHeld[0] != L_CBUTTONS) {
            if (gPlayerInputHeld[0] != 0) {
                if (gPlayerInputHeld[0] == START_BUTTON) {
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
