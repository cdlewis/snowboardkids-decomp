#include "common.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"

#define OS_MESG_NOBLOCK 0
#define OS_MESG_BLOCK 1
#define OS_EVENT_SI 5

typedef struct OSThread_s OSThread;
typedef void *OSMesg;
typedef struct OSMesgQueue_s OSMesgQueue;

typedef struct ControllerThreadMessageSlots {
    OSMesg serialEvent;
    OSMesg request;
} ControllerThreadMessageSlots;

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

u8 gControllerSubsystemBssPrefix[8];
ControllerInputState gControllerInputState[4];
u8 gControllerSubsystemBssPadding[0x7C68];
s32 gRumbleMotorStatuses[4];
s16 gRumbleMotorRequestStates[4];

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

extern OSThread gControllerSubsystemThread;
extern OSMesgQueue gControllerSubsystemRequestQueue;
extern OSMesg gControllerSubsystemRequestMessages[];
extern OSMesgQueue gControllerSubsystemReplyQueue;
extern OSMesg gControllerSubsystemReplyMessages[];
extern OSMesgQueue gControllerEventQueue;
extern OSMesg gControllerEventMessages[];
extern OSMesgQueue gControllerInputUpdateQueue;
extern s16 gControllerEventMessage;
extern OSContPad gControllerPads[];
extern OSContStatus gControllerStatuses[];
extern OSPfs gRumblePakHandles[];
extern u8 gConnectedControllerBitmask;
extern u8 gConnectedControllerCount;
extern u8 gControllerReadPending;
extern u8 gControllerSubsystemThreadStack[0x2000];
extern void *D_800EC8B8;

void initControllerSubsystem(void) {
    s32 i;

    osCreateMesgQueue(&gControllerEventQueue, gControllerEventMessages, 1);
    osCreateMesgQueue(&gControllerSubsystemRequestQueue, gControllerSubsystemRequestMessages, 8);
    osCreateMesgQueue(&gControllerSubsystemReplyQueue, gControllerSubsystemReplyMessages, 1);
    osSetEventMesg(OS_EVENT_SI, &gControllerEventQueue, (OSMesg)1);
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
    for (i = 0; i < 4; i++) {
        gControllerInputState[i].buttons = 0;
        gControllerInputState[i].stickX = 0;
        gControllerInputState[i].stickY = 0;
        gRumbleMotorStatuses[i] = RUMBLE_MOTOR_NO_PAK;
    }

    osCreateThread(
        &gControllerSubsystemThread,
        4,
        controllerSubsystemThreadMain,
        D_800EC8B8,
        gControllerSubsystemThreadStack + sizeof(gControllerSubsystemThreadStack),
        0x14
    );
    osStartThread(&gControllerSubsystemThread);
}

void controllerSubsystemThreadMain(void *threadArg) {
    OSMesg message;
    s32 request;
    s32 controllerIndex;

    message = NULL;
    while (TRUE) {
        osRecvMesg(&gControllerSubsystemRequestQueue, &message, OS_MESG_BLOCK);
        request = (s32)message;
        switch (request & CONTROLLER_REQUEST_TYPE_MASK) {
            case CONTROLLER_REQUEST_READ_INPUT:
                osContStartReadData(&gControllerEventQueue);
                /*
                 * The SI event message is discarded. Its slot is adjacent to the request message in the original
                 * stack layout; declaring another OSMesg local makes IDO unnecessarily expand the frame.
                 */
                osRecvMesg(
                    &gControllerEventQueue,
                    &((ControllerThreadMessageSlots *)&threadArg)[-1].serialEvent,
                    OS_MESG_BLOCK
                );
                osContGetReadData(gControllerPads);
                osSendMesg(&gControllerInputUpdateQueue, &gControllerEventMessage, OS_MESG_NOBLOCK);
                break;
            case CONTROLLER_REQUEST_PROBE_PAK:
                probeControllerPak(request & CONTROLLER_REQUEST_CHANNEL_MASK);
                osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, OS_MESG_NOBLOCK);
                break;
            case CONTROLLER_REQUEST_CHECK_SAVE:
                checkControllerPakSaveStatus(request & CONTROLLER_REQUEST_CHANNEL_MASK);
                osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, OS_MESG_NOBLOCK);
                break;
            case CONTROLLER_REQUEST_READ_SAVE:
                readControllerPakSave(request & CONTROLLER_REQUEST_CHANNEL_MASK);
                osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, OS_MESG_NOBLOCK);
                break;
            case CONTROLLER_REQUEST_WRITE_SAVE:
                writeControllerPakSave(request & CONTROLLER_REQUEST_CHANNEL_MASK);
                osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, OS_MESG_NOBLOCK);
                break;
            case CONTROLLER_REQUEST_REPAIR_PAK:
                repairControllerPakId(request & CONTROLLER_REQUEST_CHANNEL_MASK);
                osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, OS_MESG_NOBLOCK);
                break;
            case CONTROLLER_REQUEST_INIT_RUMBLE:
                controllerIndex = request & CONTROLLER_REQUEST_CHANNEL_MASK;
                gRumbleMotorStatuses[controllerIndex] =
                    osMotorInit(&gControllerEventQueue, &gRumblePakHandles[controllerIndex], controllerIndex);
                osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, OS_MESG_NOBLOCK);
                break;
            case CONTROLLER_REQUEST_RETRY_RUMBLE_INIT:
                controllerIndex = request & CONTROLLER_REQUEST_CHANNEL_MASK;
                gRumbleMotorStatuses[controllerIndex] =
                    osMotorInit(&gControllerEventQueue, &gRumblePakHandles[controllerIndex], controllerIndex);
                break;
            case CONTROLLER_REQUEST_START_RUMBLE:
                if ((gRumbleMotorStatuses[request & CONTROLLER_REQUEST_CHANNEL_MASK] != RUMBLE_MOTOR_NO_PAK) &&
                    (gRumbleMotorStatuses[request & CONTROLLER_REQUEST_CHANNEL_MASK] != RUMBLE_MOTOR_WRONG_DEVICE) &&
                    (gRumbleMotorStatuses[request & CONTROLLER_REQUEST_CHANNEL_MASK] !=
                     RUMBLE_MOTOR_CONTROLLER_FAILURE)) {
                    controllerIndex = request & CONTROLLER_REQUEST_CHANNEL_MASK;
                    if (osMotorStart(&gRumblePakHandles[controllerIndex]) == RUMBLE_MOTOR_CONTROLLER_FAILURE) {
                        gRumbleMotorStatuses[controllerIndex] = RUMBLE_MOTOR_CONTROLLER_FAILURE;
                    }
                }
                break;
            case CONTROLLER_REQUEST_STOP_RUMBLE:
                if ((gRumbleMotorStatuses[request & CONTROLLER_REQUEST_CHANNEL_MASK] != RUMBLE_MOTOR_NO_PAK) &&
                    (gRumbleMotorStatuses[request & CONTROLLER_REQUEST_CHANNEL_MASK] != RUMBLE_MOTOR_WRONG_DEVICE) &&
                    (gRumbleMotorStatuses[request & CONTROLLER_REQUEST_CHANNEL_MASK] !=
                     RUMBLE_MOTOR_CONTROLLER_FAILURE)) {
                    controllerIndex = request & CONTROLLER_REQUEST_CHANNEL_MASK;
                    if (osMotorStop(&gRumblePakHandles[controllerIndex]) == RUMBLE_MOTOR_CONTROLLER_FAILURE) {
                        gRumbleMotorStatuses[controllerIndex] = RUMBLE_MOTOR_CONTROLLER_FAILURE;
                    }
                }
                break;
            case CONTROLLER_REQUEST_LIST_FILES:
                readControllerPakFileStates();
                osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, OS_MESG_NOBLOCK);
                break;
            case CONTROLLER_REQUEST_DELETE_FILE:
                deleteControllerPakFile(request & CONTROLLER_REQUEST_FILE_INDEX_MASK);
                osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, OS_MESG_NOBLOCK);
                break;
            case CONTROLLER_REQUEST_UPDATE_FREE_SPACE:
                updateControllerPakFreeSpaceInfo();
                osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, OS_MESG_NOBLOCK);
                break;
        }
    }
}
