#include "common.h"

#define RACE_CAMERA_COUNT 4
#define RACE_CAMERA_STATE_SIZE 0xB0
#define RACE_PLAYER_STATE_SIZE 0x60C
#define RACE_PLAYER(slot) ((RacePlayerState *)D_80121D80[(slot)].bytes)

typedef struct {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 z;
} Vec3i;

typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ Vec3i pos;
    /* 0x028 */ u8 pad28[0x290 - 0x28];
    /* 0x290 */ Vec3i cameraPos;
    /* 0x29C */ u8 pad29C[RACE_PLAYER_STATE_SIZE - 0x29C];
} RacePlayerState;

typedef union {
    RacePlayerState state;
    u8 bytes[RACE_PLAYER_STATE_SIZE];
} RacePlayerSlot;

typedef struct {
    /* 0x00 */ u16 playerIndex;
    /* 0x02 */ s16 mode;
    /* 0x04 */ s16 pitch;
    /* 0x06 */ s16 yaw;
    /* 0x08 */ s16 roll;
    /* 0x0A */ u8 padA[2];
    /* 0x0C */ Vec3i focus;
    /* 0x18 */ Vec3i pos;
    /* 0x24 */ s32 distance;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ void (*update)(void);
    /* 0x30 */ u8 rotationMatrix[0x14];
    /* 0x44 */ Vec3i transformOffset;
    /* 0x50 */ u8 transform[0x44];
    /* 0x94 */ Vec3i prevPos;
    /* 0xA0 */ s16 timer;
    /* 0xA2 */ u8 padA2[2];
    /* 0xA4 */ s32 velocity;
    /* 0xA8 */ s32 unkA8;
    /* 0xAC */ u8 initialized;
    /* 0xAD */ u8 padAD[3];
} RaceCamera;

typedef struct StackD7D4 {
    char sp28[0x20];
    s32 sp48;
    s32 sp4C;
    s32 sp50;
    s32 sp54;
    s32 sp58;
    s32 sp5C;
} StackD7D4;

extern void func_8006D8B4(RacePlayerSlot *, s32);
extern void func_8006ECBC(void);
extern void func_8006EF1C(void);
extern void func_8006F048(void);
extern void func_8006F984(void);
extern void func_8006FA20(void);
extern void func_8006FE88(void);
extern void func_8007022C(void);
extern void func_8007031C(void);
extern void func_800704C4(void);
extern void func_800486BC(void *, void *);
extern void func_80097FE4(void *, s16, s16);
extern void func_8009853C(void *, s16, s16);
extern void func_80098590(void *, s32 *, s32 *);
extern RaceCamera D_801121E0[RACE_CAMERA_COUNT];
extern RaceCamera *D_801124A0;
extern RacePlayerSlot D_80121D80[];
extern void *D_800DA880[];
extern s32 D_80121B40;
extern s32 D_80121B44;
extern s32 D_80121B48;
extern s8 D_80121B54;
extern u8 D_80121B58;

void func_8006D520(u16 arg0, u16 arg1) {
    RaceCamera *temp;

    if (D_80121B58 == 0) {
        temp = &D_801121E0[arg0];
        temp->mode = arg1;
        temp->update = D_800DA880[arg1];
    }
}

void func_8006D580(u16 arg0, u16 arg1) {
    RaceCamera *temp = &D_801121E0[arg0];

    temp->mode = arg1;
    temp->update = D_800DA880[arg1];
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006D5CC.s")

void func_8006D700(void) {
    RaceCamera *camera;
    s32 index;

    index = 0;
    if (D_80121B54 > 0) {
 camera = D_801121E0; do {
            (D_801124A0 = camera)->update();
            index += 1;
            camera += 1;
        } while (index < D_80121B54);
    }
}

void func_8006D780(s32 arg0) {
    D_801124A0 = &D_801121E0[arg0];
    D_801124A0->update();
}

void func_8006D7D4(void) {
    StackD7D4 stack;

    func_8009853C(D_801124A0->rotationMatrix, -D_801124A0->pitch, -D_801124A0->yaw);
    stack.sp54 = 0;
    stack.sp58 = 0;
    stack.sp5C = -D_801124A0->distance;
    func_80097FE4(stack.sp28, D_801124A0->pitch, D_801124A0->yaw);
    func_80098590(stack.sp28, &stack.sp54, &stack.sp48);
    D_801124A0->transformOffset.x = stack.sp48 - D_801124A0->pos.x;
    D_801124A0->transformOffset.y = stack.sp4C - D_801124A0->pos.y;
    D_801124A0->transformOffset.z = stack.sp50 - D_801124A0->pos.z;
    func_800486BC(stack.sp28, D_801124A0->transform);
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006D8B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006DDB4.s")

void func_8006E2B4(void) {
}

void func_8006E2BC(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006E2C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006E534.s")

void func_8006EC64(void) {
}

void func_8006EC6C(void) {
}

void func_8006EC74(void) {
    D_801124A0->update = func_8006ECBC;
    D_801124A0->update();
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006ECBC.s")

void func_8006EED4(void) {
    D_801124A0->update = func_8006EF1C;
    D_801124A0->update();
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006EF1C.s")

void func_8006EFF4(void) {
    D_801124A0->timer = 0x96;
    D_801124A0->update = func_8006F048;
    D_801124A0->update();
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006F048.s")

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006F5B0.s")

void func_8006F8AC(void) {
}

void func_8006F8B4(void) {
}

void func_8006F8BC(void) {
    D_801124A0->pos.x = D_80121B40;
    D_801124A0->prevPos.x = D_801124A0->pos.x;
    D_801124A0->pos.y = D_80121B44;
    D_801124A0->prevPos.y = D_801124A0->pos.y;
    D_801124A0->pos.z = D_80121B48;
    D_801124A0->prevPos.z = D_801124A0->pos.z;
    D_801124A0->distance = 0;
    D_801124A0->unk28 = 0;
    D_801124A0->update = func_8006F984;
    D_801124A0->update();
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006F984.s")

#ifdef NON_MATCHING
void func_8006F984(void) {
    s32 stride = RACE_PLAYER_STATE_SIZE;

    D_801124A0->focus.x = RACE_PLAYER(D_801124A0->playerIndex)->cameraPos.x;
    D_801124A0->focus.y = RACE_PLAYER(D_801124A0->playerIndex)->cameraPos.y;
    D_801124A0->focus.z = RACE_PLAYER(D_801124A0->playerIndex)->cameraPos.z;
    func_8006D8B4(D_80121D80, stride);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006FA20.s")

void func_8006FDC0(void) {
    D_801124A0->roll = 0;
    D_801124A0->distance = 0;
    D_801124A0->update = func_8006FA20;
    D_801124A0->timer = 0;
    D_801124A0->update();
}

void func_8006FE24(void) {
    D_801124A0->distance = 0;
    D_801124A0->unk28 = 0;
    D_801124A0->timer = 0;
    D_801124A0->update = func_8006FE88;
    D_801124A0->update();
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8006FE88.s")

void func_80070198(void) {
    D_801124A0->pitch = 0;
    D_801124A0->yaw = 0;
    D_801124A0->roll = 0;
    D_801124A0->pos.x = 0;
    D_801124A0->pos.y = 0x310000;
    D_801124A0->pos.z = 0x900000;
    D_801124A0->distance = 0x670000;
    D_801124A0->update = func_8007022C;
    D_801124A0->update();
}

void func_8007022C(void) {
    func_8006D7D4();
}

void func_8007024C(void) {
    RacePlayerState *player = &D_80121D80[0].state;

    D_801124A0->pos.x = 0;
    D_801124A0->pos.y = 0;
    D_801124A0->pos.z = player->pos.z + 0xFE600000;
    D_801124A0->focus.x = player->pos.x;
    D_801124A0->focus.y = player->pos.y + 0xF0000;
    D_801124A0->focus.z = player->pos.z;
    D_801124A0->timer = 0;
    D_801124A0->distance = 0;
    D_801124A0->unk28 = 0;
    D_801124A0->velocity = 0x10000;
    D_801124A0->update = func_8007031C;
    D_801124A0->update();
}

#pragma GLOBAL_ASM("asm/nonmatchings/race_camera/func_8007031C.s")

void func_8007042C(void) {
    D_801124A0->pitch = 0x70;
    D_801124A0->yaw = 0;
    D_801124A0->roll = 0;
    D_801124A0->pos.x = 0;
    D_801124A0->pos.y = 0x490000;
    D_801124A0->pos.z = 0x900000;
    D_801124A0->distance = 0x500000;
    D_801124A0->update = func_800704C4;
    D_801124A0->update();
}

void func_800704C4(void) {
    func_8006D7D4();
}
