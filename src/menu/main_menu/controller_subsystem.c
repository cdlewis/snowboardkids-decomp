#include "common.h"
#include "game/menu/main_menu/controller_main_menu_flow.h"

#define OS_MESG_BLOCK 1

typedef struct OSThread_s OSThread;
typedef void *OSMesg;
typedef struct OSMesgQueue_s OSMesgQueue;

typedef struct ControllerThreadMessages {
    OSMesg event;
    OSMesg request;
} ControllerThreadMessages;

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

typedef struct ControllerInputState {
    u16 buttons;
    s8 stickX;
    s8 stickY;
    u8 pad4[2];
} ControllerInputState;

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
extern void *gControllerSubsystemThreadStack;

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
    for (i = 0; i < 4; i++) {
        gControllerInputState[i].buttons = 0;
        gControllerInputState[i].stickX = 0;
        gControllerInputState[i].stickY = 0;
        gRumbleMotorStatuses[i] = 1;
    }

    osCreateThread(&gControllerSubsystemThread, 4, controllerSubsystemThreadMain, gControllerSubsystemThreadStack,
                   &gControllerSubsystemRequestQueue, 0x14);
    osStartThread(&gControllerSubsystemThread);
}

void controllerSubsystemThreadMain(void *arg0) {
    OSMesg msg;
    s32 msgValue;
    s32 channel;

    msg = NULL;
loop:
    osRecvMesg(&gControllerSubsystemRequestQueue, &msg, OS_MESG_BLOCK);
    msgValue = (s32)msg;
    switch (msgValue & 0xF0) {
        case 0x10:
            osContStartReadData(&gControllerEventQueue);
            /* IDO places this event slot immediately before the request message and homed thread argument. */
            osRecvMesg(&gControllerEventQueue, &((ControllerThreadMessages *)&arg0)[-1].event, OS_MESG_BLOCK);
            osContGetReadData(gControllerPads);
            osSendMesg(&gControllerInputUpdateQueue, &gControllerEventMessage, 0);
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
            gRumbleMotorStatuses[channel] = osMotorInit(&gControllerEventQueue, &gRumblePakHandles[channel], channel);
            osSendMesg(&gControllerSubsystemReplyQueue, &gControllerEventMessage, 0);
            break;
        case 0xD0:
            channel = msgValue & 3;
            gRumbleMotorStatuses[channel] = osMotorInit(&gControllerEventQueue, &gRumblePakHandles[channel], channel);
            break;
        case 0x80:
            if ((gRumbleMotorStatuses[msgValue & 3] != 1) && (gRumbleMotorStatuses[msgValue & 3] != 11) &&
                (gRumbleMotorStatuses[msgValue & 3] != 4)) {
                channel = msgValue & 3;
                if (osMotorStart(&gRumblePakHandles[channel]) == 4) {
                    gRumbleMotorStatuses[channel] = 4;
                }
            }
            break;
        case 0x90:
            if ((gRumbleMotorStatuses[msgValue & 3] != 1) && (gRumbleMotorStatuses[msgValue & 3] != 11) &&
                (gRumbleMotorStatuses[msgValue & 3] != 4)) {
                channel = msgValue & 3;
                if (osMotorStop(&gRumblePakHandles[channel]) == 4) {
                    gRumbleMotorStatuses[channel] = 4;
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
    goto loop;
}
