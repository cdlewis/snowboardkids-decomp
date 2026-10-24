#include "common.h"
#include <PR/os_cont.h>
#include <PR/os_message.h>
#include <PR/os_motor.h>
#include <PR/os_pfs.h>
#include <PR/os_thread.h>
#include "game/menu/main_menu/controller_main_menu_flow.h"
#include "game/engine/system_runtime.h"
#include "game/menu/main_menu/controller_subsystem.h"

typedef struct ControllerThreadMessageSlots {
    OSMesg serialEvent;
    OSMesg request;
} ControllerThreadMessageSlots;

u8 gConnectedControllerCount;
OSThread gControllerSubsystemThread;
u8 gControllerSubsystemThreadStack[0x2000];
OSMesgQueue gControllerSubsystemRequestQueue;
OSMesg gControllerSubsystemRequestMessages[8];
OSMesgQueue gControllerSubsystemReplyQueue;
OSMesg gControllerSubsystemReplyMessages[1];
static u8 sControllerSubsystemReplyPadding[4];
OSMesgQueue gControllerEventQueue;
OSMesg gControllerEventMessages[1];
s16 gControllerEventMessage;
u8 gControllerReadPending;
OSContStatus gControllerStatuses[MAXCONTROLLERS];
OSContPad gControllerPads[MAXCONTROLLERS];
ControllerInputState gControllerInputState[MAXCONTROLLERS];
u8 gControllerSubsystemBssPadding[0x7C68];
s32 gRumbleMotorStatuses[MAXCONTROLLERS];
s16 gRumbleMotorRequestStates[MAXCONTROLLERS];

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
        if (i < MAXCONTROLLERS) {
            goto loop;
        }
    } else {
        i++;
    }

    gControllerEventMessage = 9;
    for (i = 0; i < MAXCONTROLLERS; i++) {
        gControllerInputState[i].buttons = 0;
        gControllerInputState[i].stickX = 0;
        gControllerInputState[i].stickY = 0;
        gRumbleMotorStatuses[i] = RUMBLE_MOTOR_NO_PAK;
    }

    osCreateThread(
        &gControllerSubsystemThread,
        4,
        controllerSubsystemThreadMain,
        gControllerSubsystemThreadArg.argument,
        gControllerSubsystemThreadStack + sizeof(gControllerSubsystemThreadStack),
        0x14
    );
    osStartThread(&gControllerSubsystemThread);
}

void controllerSubsystemThreadMain(void *threadArg) {
    OSMesg requestMessage;
    s32 encodedRequest;
    s32 controllerIndex;

    requestMessage = NULL;
    while (TRUE) {
        osRecvMesg(&gControllerSubsystemRequestQueue, &requestMessage, OS_MESG_BLOCK);
        encodedRequest = (s32)requestMessage;
        switch (encodedRequest & CONTROLLER_REQUEST_TYPE_MASK) {
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
                probeControllerPak(encodedRequest & CONTROLLER_REQUEST_CHANNEL_MASK);
                osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, OS_MESG_NOBLOCK);
                break;
            case CONTROLLER_REQUEST_CHECK_SAVE:
                checkControllerPakSaveStatus(encodedRequest & CONTROLLER_REQUEST_CHANNEL_MASK);
                osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, OS_MESG_NOBLOCK);
                break;
            case CONTROLLER_REQUEST_READ_SAVE:
                readControllerPakSave(encodedRequest & CONTROLLER_REQUEST_CHANNEL_MASK);
                osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, OS_MESG_NOBLOCK);
                break;
            case CONTROLLER_REQUEST_WRITE_SAVE:
                writeControllerPakSave(encodedRequest & CONTROLLER_REQUEST_CHANNEL_MASK);
                osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, OS_MESG_NOBLOCK);
                break;
            case CONTROLLER_REQUEST_REPAIR_PAK:
                repairControllerPakId(encodedRequest & CONTROLLER_REQUEST_CHANNEL_MASK);
                osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, OS_MESG_NOBLOCK);
                break;
            case CONTROLLER_REQUEST_INIT_RUMBLE:
                controllerIndex = encodedRequest & CONTROLLER_REQUEST_CHANNEL_MASK;
                gRumbleMotorStatuses[controllerIndex] =
                    osMotorInit(&gControllerEventQueue, &gRumblePakHandles[controllerIndex], controllerIndex);
                osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, OS_MESG_NOBLOCK);
                break;
            case CONTROLLER_REQUEST_RETRY_RUMBLE_INIT:
                controllerIndex = encodedRequest & CONTROLLER_REQUEST_CHANNEL_MASK;
                gRumbleMotorStatuses[controllerIndex] =
                    osMotorInit(&gControllerEventQueue, &gRumblePakHandles[controllerIndex], controllerIndex);
                break;
            case CONTROLLER_REQUEST_START_RUMBLE:
                if ((gRumbleMotorStatuses[encodedRequest & CONTROLLER_REQUEST_CHANNEL_MASK] != RUMBLE_MOTOR_NO_PAK) &&
                    (gRumbleMotorStatuses[encodedRequest & CONTROLLER_REQUEST_CHANNEL_MASK] !=
                     RUMBLE_MOTOR_WRONG_DEVICE) &&
                    (gRumbleMotorStatuses[encodedRequest & CONTROLLER_REQUEST_CHANNEL_MASK] !=
                     RUMBLE_MOTOR_CONTROLLER_FAILURE)) {
                    controllerIndex = encodedRequest & CONTROLLER_REQUEST_CHANNEL_MASK;
                    if (osMotorStart(&gRumblePakHandles[controllerIndex]) == RUMBLE_MOTOR_CONTROLLER_FAILURE) {
                        gRumbleMotorStatuses[controllerIndex] = RUMBLE_MOTOR_CONTROLLER_FAILURE;
                    }
                }
                break;
            case CONTROLLER_REQUEST_STOP_RUMBLE:
                if ((gRumbleMotorStatuses[encodedRequest & CONTROLLER_REQUEST_CHANNEL_MASK] != RUMBLE_MOTOR_NO_PAK) &&
                    (gRumbleMotorStatuses[encodedRequest & CONTROLLER_REQUEST_CHANNEL_MASK] !=
                     RUMBLE_MOTOR_WRONG_DEVICE) &&
                    (gRumbleMotorStatuses[encodedRequest & CONTROLLER_REQUEST_CHANNEL_MASK] !=
                     RUMBLE_MOTOR_CONTROLLER_FAILURE)) {
                    controllerIndex = encodedRequest & CONTROLLER_REQUEST_CHANNEL_MASK;
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
                deleteControllerPakFile(encodedRequest & CONTROLLER_REQUEST_FILE_INDEX_MASK);
                osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, OS_MESG_NOBLOCK);
                break;
            case CONTROLLER_REQUEST_UPDATE_FREE_SPACE:
                updateControllerPakFreeSpaceInfo();
                osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, OS_MESG_NOBLOCK);
                break;
        }
    }
}
