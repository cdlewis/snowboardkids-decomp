#include "common.h"
#include "game_audio.h"
#include "effect_task_scheduler.h"
#include "asset_manager.h"
#include "game_boot.h"
#include "input_task_scheduler.h"
#include "main_menu.h"
#include "main_menu_overlay_effects.h"
#include "main_menu_panel_ui.h"
#include "main_menu_scene_model.h"
#include "main_menu_scene_renderer.h"
#include "main_menu_transition.h"
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
extern void func_80002A1C(void);
extern void func_80002CE4(void);
extern void func_8000262C(void);
extern void func_80002024(void);
extern OSThread D_800E29C8;
extern OSMesgQueue D_800E4B78;
extern OSMesg D_800E4B90[];
extern OSMesgQueue D_800E4BB0;
extern OSMesg D_800E4BC8[];
extern OSMesgQueue D_800E4BD0;
extern OSMesg D_800E4BE8[];
extern s16 D_800E4BEC;
extern OSContPad D_800E4C00[];
extern s32 D_800E4C30[];
extern OSPfs D_800E4C40[];
extern OSPfs D_800E4DE0[];
extern SaveFileIdentity D_800E4F80;
extern u8 D_800E4F8A[];
extern u8 D_800E4F8E[];
extern MainMenuState *D_801235B8;
extern u8 D_800B30F0;
extern u8 D_800B30F4[];
extern u8 D_800B3104[];
extern u8 D_800B3108[];
extern u8 D_800B318C;
extern u8 D_800DEED4;
extern u8 D_800E4BEE;
extern OSContStatus D_800E4BF0[];
extern s16 D_800E4C18;
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
extern s32 D_800E4C30[];
extern u8 D_800E4F8A[];
extern u8 D_800E4F8E[];
extern u8 D_800EC9D8;
extern s32 D_800EC898;
extern s32 D_800EC89C;
extern s32 D_800EC8A0;
extern s32 D_800EC8A4;
extern u8 D_800EC8B4[];
extern void *D_800EC8B8;
extern s16 D_800EC9C8[];
extern u8 D_800EC9E0[];
extern SaveSlotBytes D_800EC9F0[];
extern s32 gPlayerInputHeld;
extern s16 gMenuFadeAlpha;
extern s16 D_801124B8;
extern s16 D_800EC8A8[];
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
extern s8 D_800B3190;
extern u8 D_800B3194;
extern s8 D_800DEF10;
extern u8 D_800E29C0;
extern u8 D_800EC8B0;
extern u8 D_8010ADFA;
extern s8 D_8010B1F0;
extern OSPfsState gControllerPakFileStates[];
extern s32 gControllerPakFreeBytes;
extern s32 gControllerPakFreeFileCount;
extern OSMesgQueue D_80124070;
extern u8 D_80123750;
extern u8 D_80123751;
extern u8 D_8012482A;
extern u8 D_8012482B;
extern u8 D_8012482C;
extern s32 D_801235B4;
extern s32 gPlayerInputPressed;
extern u8 D_800B30F4[];
extern u8 D_800B3104[];
extern u8 D_800B3108[];

// func_80000450 best match: 85.817%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu/func_80000450.s")

#ifdef NON_MATCHING
void func_80000450(void) {
    s32 i;

    osCreateMesgQueue(&D_800E4BD0, D_800E4BE8, 1);
    osCreateMesgQueue(&D_800E4B78, D_800E4B90, 8);
    osCreateMesgQueue(&D_800E4BB0, D_800E4BC8, 1);
    osSetEventMesg(5, &D_800E4BD0, (OSMesg)1);
    osContInit(&D_800E4BD0, &D_800B30F0, D_800E4BF0);

    D_800E29C0 = 0;
    D_800E4BEE = 0;

    i = 0;
loop:
    if (((s32)D_800B30F0 >> i) & 1) {
        i++;
        D_800E29C0++;
        if (i < 4) {
            goto loop;
        }
    } else {
        i++;
    }

    D_800E4BEC = 9;
    D_800E4C18 = 0;
    D_800E4C1A = 0;
    D_800E4C1B = 0;
    D_800EC898 = 1;
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

    osCreateThread(&D_800E29C8, 4, func_800005E4, D_800EC8B8, &D_800E4B78, 0x14);
    osStartThread(&D_800E29C8);
}
#endif

// func_800005E4 best match: 99.507%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu/func_800005E4.s")

#ifdef NON_MATCHING
void func_800005E4(void *arg0) {
    OSMesg msg;
    s32 msgValue;
    s32 channel;

    msg = NULL;
    while (((1 & 0xFFFFFFFFFFFFFFFF) & 0xFFFFFFFFFFFFFFFF) & 1) {
        osRecvMesg(&D_800E4B78, &msg, OS_MESG_BLOCK);
        msgValue = (s32)msg;
        switch (msgValue & 0xF0) {
        case 0x10:
            osContStartReadData(&D_800E4BD0);
            osRecvMesg(&D_800E4BD0, ((OSMesg *)&arg0) - 2, OS_MESG_BLOCK);
            osContGetReadData(D_800E4C00);
            osSendMesg(&D_80124070, &D_800E4BEC, 0);
            break;
        case 0x20:
            func_80000C94(msgValue & 3);
            osSendMesg(&D_800E4BB0, &D_800E4BEC, 0);
            break;
        case 0x30:
            func_80000E00(msgValue & 3);
            osSendMesg(&D_800E4BB0, &D_800E4BEC, 0);
            break;
        case 0x40:
            func_8000105C(msgValue & 3);
            osSendMesg(&D_800E4BB0, &D_800E4BEC, 0);
            break;
        case 0x50:
            func_80001318(msgValue & 3);
            osSendMesg(&D_800E4BB0, &D_800E4BEC, 0);
            break;
        case 0x60:
            func_80001584(msgValue & 3);
            osSendMesg(&D_800E4BB0, &D_800E4BEC, 0);
            break;
        case 0x70:
            channel = msgValue & 3;
            (&D_800EC898)[channel] = osMotorInit(&D_800E4BD0, &D_800E4DE0[channel], channel);
            osSendMesg(&D_800E4BB0, &D_800E4BEC, 0);
            break;
        case 0xD0:
            channel = msgValue & 3;
            (&D_800EC898)[channel] = osMotorInit(&D_800E4BD0, &D_800E4DE0[channel], channel);
            break;
        case 0x80:
            if (((&D_800EC898)[msgValue & 3] != 1) && ((&D_800EC898)[msgValue & 3] != 11) &&
                ((&D_800EC898)[msgValue & 3] != 4)) {
                channel = msgValue & 3;
                if (osMotorStart(&D_800E4DE0[channel]) == 4) {
                    (&D_800EC898)[channel] = 4;
                }
            }
            break;
        case 0x90:
            if (((&D_800EC898)[msgValue & 3] != 1) && ((&D_800EC898)[msgValue & 3] != 11) &&
                ((&D_800EC898)[msgValue & 3] != 4)) {
                channel = msgValue & 3;
                if (osMotorStop(&D_800E4DE0[channel]) == 4) {
                    (&D_800EC898)[channel] = 4;
                }
            }
            break;
        case 0xA0:
            readControllerPakFileStates();
            osSendMesg(&D_800E4BB0, &D_800E4BEC, 0);
            break;
        case 0xB0:
            deleteControllerPakFile(msgValue & 0xF);
            osSendMesg(&D_800E4BB0, &D_800E4BEC, 0);
            break;
        case 0xC0:
            updateControllerPakFreeSpaceInfo();
            osSendMesg(&D_800E4BB0, &D_800E4BEC, 0);
            break;
        }
    }
}
#endif

void func_80000960(void) {
    if ((D_800E4BEE == 0) && (D_800B30F0 != 0)) {
        osSendMesg(&D_800E4B78, (OSMesg)0x10, OS_MESG_BLOCK);
        D_800E4BEE = 1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu/func_800009B0.s")

void func_80000A40(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&D_800E4B78, (OSMesg)(arg0 + 0x70), OS_MESG_BLOCK);
    osRecvMesg(&D_800E4BB0, &msg, OS_MESG_BLOCK);
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu/func_80000A8C.s")

void func_80000C00(u16 arg0) {
    if (D_800EC8B0 != 0) {
        if (D_8010ADFA & (1 << arg0)) {
            D_800EC8A8[arg0] = 1;
        }
    }
}

void func_80000C48(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&D_800E4B78, (OSMesg)(arg0 + 0x20), OS_MESG_BLOCK);
    osRecvMesg(&D_800E4BB0, &msg, OS_MESG_BLOCK);
}

// func_80000C94 best match: 94.507%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu/func_80000C94.s")

#ifdef NON_MATCHING
void func_80000C94(u16 arg0) {
    s32 ret;

    ret = osPfsInitPak(&D_800E4BD0, &D_800E4C40[arg0], arg0);
    if (ret == 2) {
        ret = osPfsInitPak(&D_800E4BD0, &D_800E4C40[arg0], arg0);
    }

    if (ret == 0) {
        D_800EC9C8[arg0] = ret + 1;
    }

    if ((ret == 1) || (ret == 11)) {
        D_800EC9C8[arg0] = 10;
    }

    if (ret == 10) {
        if (D_800EC8B4[arg0] == 1) {
            D_800EC9C8[arg0] = 16;
        } else {
            D_800EC9C8[arg0] = 7;
        }
    }

    if (ret != 0) {
        D_800EC9E0[arg0]++;
    }
}
#endif

void func_80000DB4(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&D_800E4B78, (OSMesg)(arg0 + 0x30), OS_MESG_BLOCK);
    osRecvMesg(&D_800E4BB0, &msg, OS_MESG_BLOCK);
}

// func_80000E00 best match: 81.386%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu/func_80000E00.s")

#ifdef NON_MATCHING
void func_80000E00(u16 arg0) {
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

    D_800E4F80.file_size = 0x7900;
    D_800E4F80.game_code = 0x4E534B45;
    D_800E4F80.company_code = 0x4542;

    src = D_800B3104;
    dst = (u8 *) &D_800E4F80;
    do {
        byte0 = *src;
        src++;
        dst++;
        dst[9] = byte0;
    } while (src < D_800B3108);

    src = D_800B30F4;
    dst = (u8 *) &D_800E4F80;
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

    pfs = &D_800E4C40[arg0];
    osPfsInitPak(&D_800E4BD0, pfs, arg0);

    ret = osPfsFindFile(pfs, D_800E4F80.company_code, D_800E4F80.game_code, D_800E4F8E, D_800E4F8A,
                        &D_800E4C30[arg0]);
    if (ret == 0) {
        D_800EC9C8[arg0] = 2;
    } else {
        osPfsNumFiles(pfs, &maxFiles, &filesUsed);
        if (filesUsed == 0x10) {
            D_800EC9C8[arg0] = 0xC;
        } else {
            osPfsFreeBlocks(pfs, &freeBytes);
            if ((freeBytes / 256) < 0x79) {
                D_800EC9C8[arg0] = 0xB;
            } else if (ret == 5) {
                D_800EC9C8[arg0] = 9;
            }
        }
    }

    if (ret != 0) {
        D_800EC9E0[arg0]++;
    }
}
#endif

void func_80001010(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&D_800E4B78, (OSMesg)(arg0 + 0x40), OS_MESG_BLOCK);
    osRecvMesg(&D_800E4BB0, &msg, OS_MESG_BLOCK);
}

// func_8000105C best match: 85.904%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu/func_8000105C.s")

#ifdef NON_MATCHING
void func_8000105C(u16 arg0) {
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
    pfs = &D_800E4C40[channel];
    badChecksum = 0;
    osPfsInitPak(&D_800E4BD0, pfs, channel);

    D_800E4F80.gameCode = 'NSKE';
    D_800E4F80.companyCode = 'EB';

    src = D_800B3104;
    dst = (u8 *)&D_800E4F80;
    end = D_800B3108;
copy_ext:
    dst[10] = *src;
    src++;
    dst++;
    if (src < end) {
        goto copy_ext;
    }

    src = D_800B30F4;
    dst = (u8 *)&D_800E4F80;
    end = D_800B3104;
copy_name:
    dst[14] = *src;
    src++;
    dst++;
    if (src < end) {
        goto copy_name;
    }

    fileNo = &D_800E4C30[channel];
    osPfsFindFile(pfs, D_800E4F80.companyCode, D_800E4F80.gameCode, D_800E4F8E, D_800E4F8A, fileNo);

    save = &D_800EC9F0[channel];
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
            if (func_80001904(savedChannel) == 0) {
                (&D_800EC9D8)[channel] = 0;
            }
        } else {
            (&D_800EC9D8)[channel]++;
        }
    } else {
        (&D_800EC9D8)[channel]++;
    }

    if ((ret != 0) || ((&D_800EC9D8)[channel] != 0)) {
        if ((&D_800EC9D8)[channel] != 3) {
            return;
        }
    }
    D_800EC9E0[channel]++;
}
#endif

void func_800012CC(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&D_800E4B78, (OSMesg)(arg0 + 0x50), OS_MESG_BLOCK);
    osRecvMesg(&D_800E4BB0, &msg, OS_MESG_BLOCK);
}

// func_80001318 best match: 78.684%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu/func_80001318.s")

#ifdef NON_MATCHING
void func_80001318(u16 arg0) {
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
    D_800E4F80.size = 0x7900;
    D_800E4F80.gameCode = 'NSKE';
    D_800E4F80.companyCode = 'EB';

    src = D_800B3104;
    dst = (u8 *)&D_800E4F80;
    end = D_800B3108;
copy_ext:
    dst[10] = *src;
    src++;
    dst++;
    if (src < end) {
        goto copy_ext;
    }

    src = D_800B30F4;
    dst = (u8 *)&D_800E4F80;
    end = D_800B3104;
copy_name:
    dst[14] = *src;
    src++;
    dst++;
    if (src < end) {
        goto copy_name;
    }

    pfs = &D_800E4C40[channel];
    osPfsInitPak(&D_800E4BD0, pfs, channel);

    fileNo = &D_800E4C30[channel];
    if (osPfsFindFile(pfs, D_800E4F80.companyCode, D_800E4F80.gameCode, D_800E4F8E, D_800E4F8A, fileNo) == 5) {
        osPfsAllocateFile(pfs, D_800E4F80.companyCode, D_800E4F80.gameCode, D_800E4F8E, D_800E4F8A, 0x7900, fileNo);
    }

    save = &D_800EC9F0[channel];
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
        (&D_800EC9D8)[channel] = 0;
        return;
    }
    (&D_800EC9D8)[channel]++;
}
#endif

void func_80001538(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&D_800E4B78, (OSMesg)(arg0 + 0x60), OS_MESG_BLOCK);
    osRecvMesg(&D_800E4BB0, &msg, OS_MESG_BLOCK);
}

void func_80001584(u16 arg0) {
    OSPfs **sp18;
    OSPfs *pfs;
    s32 ret;

    pfs = &D_800E4C40[arg0];
    sp18 = &pfs;
    osPfsInitPak(&D_800E4BD0, *sp18, arg0);
    ret = osPfsRepairId(pfs);
    if ((ret == 4) || (ret == 0xA)) {
        (&D_800EC9D8)[arg0] += 1;
    }
}

void requestControllerPakFileList(void) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&D_800E4B78, (OSMesg)0xA0, OS_MESG_BLOCK);
    osRecvMesg(&D_800E4BB0, &msg, OS_MESG_BLOCK);
}

void readControllerPakFileStates(void) {
    s32 i;

    osPfsInitPak(&D_800E4BD0, &D_800E4C40[0], 0);
    for (i = 0; i != 0x10; i++) {
        osPfsFileState(&D_800E4C40[0], i, &gControllerPakFileStates[i]);
    }
}

void requestControllerPakDeleteFile(u16 arg0) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&D_800E4B78, (OSMesg)(arg0 + 0xB0), OS_MESG_BLOCK);
    osRecvMesg(&D_800E4BB0, &msg, OS_MESG_BLOCK);
}

void deleteControllerPakFile(u16 arg0) {
    OSPfs *pfs;
    OSPfsState *state;
    u16 companyCode;
    u32 gameCode;
    u8 gameName[16];
    u8 extName[4];
    s32 i;

    pfs = D_800E4C40;
    osPfsInitPak(&D_800E4BD0, pfs, 0);

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
            D_800EC9D8 = 0;
            return;
        }
        D_800EC9D8++;
    }
}

void requestControllerPakFreeSpaceUpdate(void) {
    OSMesg msg;

    msg = NULL;

    osSendMesg(&D_800E4B78, (OSMesg)0xC0, OS_MESG_BLOCK);
    osRecvMesg(&D_800E4BB0, &msg, OS_MESG_BLOCK);
}

void updateControllerPakFreeSpaceInfo(void) {
    s32 pad;
    s32 maxFiles;
    s32 filesUsed;

    osPfsInitPak(&D_800E4BD0, &D_800E4C40[0], 0);
    osPfsFreeBlocks(&D_800E4C40[0], &gControllerPakFreeBytes);
    osPfsNumFiles(&D_800E4C40[0], &maxFiles, &filesUsed);
    gControllerPakFreeFileCount = maxFiles - filesUsed;
}

// func_80001904 best match: 33.167%
#pragma GLOBAL_ASM("asm/nonmatchings/main_menu/func_80001904.s")

#ifdef NON_MATCHING
u16 func_80001904(s32 arg0) {
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

    return func_80001994(arg0);
}
#endif

s32 func_80001994(s32 arg0) {
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

    var_t2 = (u8 *)&D_800EC9F0[arg0];
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

void func_80001C30(void) {
    D_800B318C = 1;
    func_8009956C(func_80001C80, 0);
    func_800994F4(4, func_8003ED00, 0x64);
    func_80099614(3);
}

void func_80001C80(void) {
    D_800B3190 = 0;
    D_800B3194 = 0;
    func_800704F0();
    func_80070C64(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    D_800DEED4 = 0;
    loadCompressedRomAsset(D_5DB9D0, D_5DCBE0, 0x21);
    loadCompressedRomAsset(D_5DCBE0, D_5DFDD0, 0x25);
    loadCompressedRomAsset(D_593D10, D_598A70, 0x22);
    loadRawRomAsset(D_1467B0, D_147910, 8);
    loadCompressedRomAsset(D_1DE360, D_1E0F70, 9);
    D_801235B8->fade = 0xFF;
    D_801235B8->selection = 0;
    D_801235B8->delay = 0x32;
    D_801235B8->timer = 0x4B0;
    func_80070EC0(0);
    if (D_800E29C0 != 0) {
        func_80071408(&func_80032A50, 0, 0x64);
    } else {
        func_80071408(&func_800540EC, 0, 0x64);
    }
    func_80071408(&func_80032934, 0, 0x64);
    func_80071408(&func_80032684, 0, 0x64);
    func_80071664(&func_80054714, 0, 0x64, 0);
    func_80071664(func_80055678, 0, 0x64, 0);
    func_8009B58C(0x20, 0x40, 0x50);
    func_80041CF0();
    func_80041D20(0, 0);
    func_80041DD4(0, 0);
    func_8004209C(0, 0x83000, 0, -0x2BF000);
    func_800420FC(0, 0, 0x30, 0);
    func_80041D20(1, 1);
    func_80041DD4(1, 4);
    func_8004209C(1, 0x564000, 0, 0x8C000);
    func_800420FC(1, 0, 0xDD0, 0);
    func_80041D20(2, 2);
    func_80041DD4(2, 3);
    func_8004209C(2, -0x5ED000, 0, 0x3C8000);
    func_800420FC(2, 0, 0x2D0, 0);
    func_80041D20(3, 3);
    func_80041DD4(3, 1);
    func_8004209C(3, -0x4C0000, 0, -0x47000);
    func_800420FC(3, 0, 0x1B0, 0);
    func_80041D20(4, 4);
    func_80041DD4(4, 5);
    func_8004209C(4, 0x6DB000, 0, 0x4E9000);
    func_800420FC(4, 0, 0xDD0, 0);
    func_8006D5CC();
    func_8006D520(0, 0x1F);
    gMenuFadeAlpha = (s16) D_801235B8->fade;
    D_800DEF10 = 1;
    enqueueSoundEffect(0x4A, 0x32);
    func_8009956C(&func_80002024, 0);
    func_80000A40(0U);
    func_80000A40(1U);
    func_80000A40(2U);
    func_80000A40(3U);
}

void func_80002024(void) {
    s32 unused[2];
    s32 flag;
    s32 temp_v1;
    s32 temp_a0;

    flag = 0;
    temp_v1 = D_801235B8->fade;
    if (temp_v1 != 0) {
        D_801235B8->fade = func_80013F88((s16) temp_v1, 0x10, 0);
        if (D_801235B8->fade == 0) {
            D_800DEF10 = 0;
        }
    } else {
        if (D_801235B8->delay != 0) {
            D_801235B8->delay -= 1;
            if (D_801235B8->delay == 0) {
                func_800720E4(0);
            }
        }
        temp_a0 = gPlayerInputPressed;
        if (temp_a0 & 0x10800) {
            if (D_801235B8->selection != 0) {
                D_801235B8->selection -= 1;
                enqueueSoundEffect(0x19, 0x32);
                temp_a0 = gPlayerInputPressed;
            }
        }
        if (temp_a0 & 0x20400) {
            if (D_801235B8->selection != 2) {
                D_801235B8->selection += 1;
                enqueueSoundEffect(0x19, 0x32);
                temp_a0 = gPlayerInputPressed;
            }
        }
        if ((temp_a0 & 0x1000) || (temp_a0 & 0x8000)) {
            flag = 1;
            enqueueSoundEffect(1, 0x32);
        }
        if (flag == 0) {
            D_801235B8->timer -= 1;
            if (D_801235B8->timer == 0) {
                flag = 1;
            }
        }
        if (flag != 0) {
            func_8009956C(func_800022B8, 0);
            func_80072114(0xC);
        }
    }
    func_8006D780(0);
    if (func_80002DA0() != 0) {
        D_800B3190 = 1;
        enqueueSoundEffect(0x26, 0x32);
    }
    func_80042034(0);
    func_80042034(1);
    func_80042034(2);
    func_80042034(3);
    func_80042034(4);
    if (D_801235B8->fade == 0xEF) {
        func_800428C8(0);
        func_800428C8(1);
        func_800428C8(2);
        func_800428C8(3);
        func_800428C8(4);
    } else {
        func_80042A00(0);
        func_800428C8(1);
        func_800428C8(2);
        func_80042A00(3);
        func_800428C8(4);
    }
    func_8007105C();
}

void func_800022B8(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = D_801235B8->fade;
    if (temp_v0 != 0xFF) {
        D_801235B8->fade = func_80013F88((s16) temp_v0, 0x28, 1);
        if (D_801235B8->fade == 0xFF) {
            D_80123751 = 1;
        } else {
            func_8006D780(0);
            func_80042034(0);
            func_80042034(1);
            func_80042034(2);
            func_80042034(3);
            func_80042034(4);
            func_800428C8(0);
            func_800428C8(1);
            func_800428C8(2);
            func_800428C8(3);
            func_800428C8(4);
            func_8007105C();
        }
    } else if (D_80123750 == 2) {
        releaseMenuAssetHandles();
        D_80123751 = 0;
        D_800DEED4 = 0;
        if (D_801235B8->timer != 0) {
            temp_v1 = D_801235B8->selection;
            if (temp_v1 == 0) {
                func_800994F4(2, func_80073140, 0x64);
                func_8009954C(3);
            } else if (temp_v1 == 1) {
                func_8009956C(func_800024A8, 0);
            } else {
                func_8009956C(func_800028B4, 0);
            }
        } else if (D_800B318C == 0) {
            func_8009956C(func_80001C30, 0);
        } else {
            D_800B318C = 0;
            func_8009956C(func_80001C80, 0);
            func_800994F4(4, func_8003E600, 0x64);
            func_80099614(3);
        }
    }
}

void func_800024A8(void) {
    func_800704F0();
    D_801124B8 = 0x80;
    D_800DEED4 = 0;
    func_80070C64(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    loadRawRomAsset(D_1467B0, D_147910, 8);
    loadCompressedRomAsset(D_1DE360, D_1E0F70, 9);
    loadCompressedRomAsset(D_1F1A90, D_1F2220, 0x28);
    func_8006D5CC();
    func_8006D520(0, 0x1F);
    func_80070EC0(0);
    func_80071664(func_80055678, 0, 0x64, 0);
    D_801235B4 = 0;
    D_8010B1F0 = 0;
    func_80071664(&func_80052394, 0, 0x64, 0);
    func_8009B58C(0x20, 0x40, 0x50);
    func_80041CF0();
    func_80041D20(4, 4);
    func_80041DD4(4, 5);
    func_8004209C(4, 0xFFE00000, 0, 0x509000);
    func_800420FC(4, 0, 0x100, 0);
    func_8009956C(func_8000262C, 0);
    func_800720E4(7);
}

void func_8000262C(void) {
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
            D_8010B1F0 = 1;
            if (D_801235B4 == 2) {
                func_80072114(0x3C);
            }
            func_8009956C(func_80002794, 0);
        }
    }
    func_8006D780(0);
    func_80042034(4);
    func_800428C8(4);
    func_8007105C();
}

void func_80002794(void) {
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0x100) {
        gMenuFadeAlpha = 0xFF;
        D_80123751 = 1;
        func_8009956C(func_80002810, 0);
    }
    func_8006D780(0);
    func_80042034(4);
    func_800428C8(4);
    func_8007105C();
}

void func_80002810(void) {
    if (D_80123750 == 2) {
        releaseMenuAssetHandles();
        D_80123751 = 0;
        D_800DEED4 = 0;
        func_8009956C(func_80001C80, 0);
        if (D_801235B4 == 0) {
            func_800994F4(4, func_8003FFD0, 0x64);
            func_80099614(3);
            return;
        }
        if (D_801235B4 == 1) {
            func_800994F4(4, func_8003F520, 0x64);
            func_80099614(3);
        }
    }
}

void func_800028B4(void) {
    func_800704F0();
    func_80070C64(0, 0xA0, 0x78, 0x120, 0xD0, 0x140, 0xF0, 1.3333334f);
    D_801124B8 = 0x80;
    D_800DEED4 = 0;
    loadCompressedRomAsset(D_593D10, D_598A70, 0x29);
    loadCompressedRomAsset(D_1F1A90, D_1F2220, 0x28);
    loadCompressedRomAsset(D_60F1A0, D_60F990, 0x2A);
    loadRawRomAsset(D_1467B0, D_147910, 8);
    loadCompressedRomAsset(D_1DE360, D_1E0F70, 9);
    func_8006D5CC();
    func_8006D520(0, 0x1F);
    func_80070EC0(0);
    D_801235B4 = 0;
    D_8010B1F0 = 0;
    func_80071664(&func_80052E4C, 0, 0x64, 0);
    func_80071664(&func_80055678, 0, 0x64, 0);
    func_8009B58C(0x20, 0x40, 0x50);
    func_8009956C(&func_80002A1C, 0);
    func_800720E4(7);
}

void func_80002A1C(void) {
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
                if (D_8012482A != 0) {
                    D_8012482A -= 1;
                    enqueueSoundEffect(0x19, 0x32);
                    temp_v1 = gPlayerInputPressed;
                }
                break;
            case 1:
                if (D_8012482B != 1) {
                    D_8012482B += 1;
                    enqueueSoundEffect(0x19, 0x32);
                    temp_v1 = gPlayerInputPressed;
                }
                break;
            case 2:
                if (D_8012482C != 1) {
                    D_8012482C += 1;
                    enqueueSoundEffect(0x19, 0x32);
                    temp_v1 = gPlayerInputPressed;
                }
                break;
            }
        }
        if (temp_v1 & 0x80200) {
            switch (D_801235B4) {
            case 0:
                if (D_8012482A != 1) {
                    D_8012482A += 1;
                    enqueueSoundEffect(0x19, 0x32);
                    temp_v1 = gPlayerInputPressed;
                }
                break;
            case 1:
                if (D_8012482B != 0) {
                    D_8012482B -= 1;
                    enqueueSoundEffect(0x19, 0x32);
                    temp_v1 = gPlayerInputPressed;
                }
                break;
            case 2:
                if (D_8012482C != 0) {
                    D_8012482C -= 1;
                    enqueueSoundEffect(0x19, 0x32);
                    temp_v1 = gPlayerInputPressed;
                }
                break;
            }
        }
        if ((temp_v1 & 0xD000) && (D_801235B4 == 3)) {
            func_80072114(0x3C);
            enqueueSoundEffect(0x18, 0x32);
            func_8009956C(func_80002CE4, 0);
        }
    }
    func_8006D780(0);
    func_8007105C();
}

void func_80002CE4(void) {
    gMenuFadeAlpha += 0x10;
    if (gMenuFadeAlpha >= 0x100) {
        gMenuFadeAlpha = 0xFF;
        D_80123751 = 1;
        func_8009956C(func_80002D50, 0);
    }
    func_8006D780(0);
    func_8007105C();
}

void func_80002D50(void) {
    if (D_80123750 == 2) {
        releaseMenuAssetHandles();
        D_80123751 = 0;
        D_800DEED4 = 0;
        func_8009956C(func_80001C80, 0);
    }
}

s32 func_80002DA0(void) {
    switch (D_800B3194) {
    case 0:
        if (gPlayerInputHeld == 0x20000) {
            D_800B3194++;
        }
        goto end0;
    case 1:
        if ((gPlayerInputHeld != 0x20000) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x10000) {
                D_800B3194++;
            } else {
                D_800B3194 = -1;
            }
        }
        goto end0;
    case 2:
        if ((gPlayerInputHeld != 0x10000) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x400) {
                D_800B3194++;
            } else {
                D_800B3194 = -1;
            }
        }
        goto end0;
    case 3:
        if ((gPlayerInputHeld != 0x400) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x800) {
                D_800B3194++;
            } else {
                D_800B3194 = -1;
            }
        }
        goto end0;
    case 4:
        if ((gPlayerInputHeld != 0x800) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 4) {
                D_800B3194++;
            } else {
                D_800B3194 = -1;
            }
        }
        goto end0;
    case 5:
        if ((gPlayerInputHeld != 4) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 8) {
                D_800B3194++;
            } else {
                D_800B3194 = -1;
            }
        }
        goto end0;
    case 6:
        if ((gPlayerInputHeld != 8) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x20) {
                D_800B3194++;
            } else {
                D_800B3194 = -1;
            }
        }
        goto end0;
    case 7:
        if ((gPlayerInputHeld != 0x20) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x10) {
                D_800B3194++;
            } else {
                D_800B3194 = -1;
            }
        }
        goto end0;
    case 8:
        if ((gPlayerInputHeld != 0x10) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x2000) {
                D_800B3194++;
            } else {
                D_800B3194 = -1;
            }
        }
        goto end0;
    case 9:
        if ((gPlayerInputHeld != 0x2000) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x200) {
                D_800B3194++;
            } else {
                D_800B3194 = -1;
            }
        }
        goto end0;
    case 10:
        if ((gPlayerInputHeld != 0x200) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 1) {
                D_800B3194++;
            } else {
                D_800B3194 = -1;
            }
        }
        goto end0;
    case 11:
        if ((gPlayerInputHeld != 1) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x10000) {
                D_800B3194++;
            } else {
                D_800B3194 = -1;
            }
        }
        goto end0;
    case 12:
        if ((gPlayerInputHeld != 0x10000) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x4000) {
                D_800B3194++;
            } else {
                D_800B3194 = -1;
            }
        }
        goto end0;
    case 13:
        if ((gPlayerInputHeld != 0x4000) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 0x100) {
                D_800B3194++;
            } else {
                D_800B3194 = -1;
            }
        }
        goto end0;
    case 14:
        if ((gPlayerInputHeld != 0x100) && (gPlayerInputHeld != 0)) {
            if (gPlayerInputHeld == 2) {
                D_800B3194++;
            } else {
                D_800B3194 = -1;
            }
        }
        goto end0;
    case 15:
        if (gPlayerInputHeld != 2) {
            if (gPlayerInputHeld != 0) {
                if (gPlayerInputHeld == 0x1000) {
                    return 1;
                }
                D_800B3194 = -1;
            }
        }
        goto end0;
    default:
    end0:
        return 0;
    }
}
