#include "common.h"

typedef struct {
    s32 x;
    s32 y;
} Vec2i;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i;

typedef s16 Matrix4s[0x10];

typedef struct {
    char pad[0x18];
    s32 fade;
} MainMenuRaceSetupState;

typedef struct {
    char pad0[0x4];
    s16 pitch;
    s16 yaw;
    char pad8[0x10];
    Vec3i pos;
    s32 depth;
    char pad28[0x8];
    char rotationMtx[0x14];
    Vec3i cameraDelta;
} MainMenuRaceSetupObject;

typedef struct {
    Matrix4s rotationMtx;
    Vec3i worldPos;
    Vec3i localPos;
} TransformScratch;

extern void func_80045914(void);
extern void func_80099658(s32);
extern void func_8009954C(s32);
extern s8 D_800DEED4;
extern u8 D_80123750;
extern u8 D_80123751;
extern s16 D_800DEF14;
extern void func_8005393C(void);
extern void func_8006D780(s32);
extern void func_8007105C(void);
extern void func_80071664(void *, s32, s32, s32);
extern void func_80072114(s32);
extern void func_800720E4(s32);
extern void func_8009956C(void *, s32);
void func_8003E45C(void);
void func_8003E514(void);
void func_8003E5A8(void);

extern MainMenuRaceSetupState *D_801235B8;
extern MainMenuRaceSetupObject *D_800EC9C4;
extern Vec2i D_8010B1B0;
extern void func_80097FE4(void *, s16, s16);
extern void func_8009853C(void *, s16, s16);
extern void func_80098590(void *, s32 *, s32 *);

void func_8003DDD0() {
    TransformScratch scratch;
    MainMenuRaceSetupObject *obj;

    obj = D_800EC9C4;
    func_8009853C(obj->rotationMtx, -obj->pitch, -obj->yaw);
    obj = D_800EC9C4;
    scratch.localPos.x = 0;
    scratch.localPos.y = 0;
    scratch.localPos.z = -obj->depth;
    func_80097FE4(scratch.rotationMtx, obj->pitch, obj->yaw);
    func_80098590(scratch.rotationMtx, &scratch.localPos.x, &scratch.worldPos.x);
    obj = D_800EC9C4;
    obj->cameraDelta.x = scratch.worldPos.x - obj->pos.x;
    obj = D_800EC9C4;
    obj->cameraDelta.y = scratch.worldPos.y - obj->pos.y;
    obj = D_800EC9C4;
    obj->cameraDelta.z = scratch.worldPos.z - obj->pos.z;
}

struct ALSeqPlayer;

void n_alSeqpDelete(struct ALSeqPlayer *seqp) {
    func_8003DDD0(seqp);
}

void func_8003DEC8(void) {
    TransformScratch scratch;
    MainMenuRaceSetupObject *obj;

    obj = D_800EC9C4;
    func_8009853C(obj->rotationMtx, -obj->pitch, -obj->yaw);
    obj = D_800EC9C4;
    scratch.localPos.x = D_8010B1B0.x;
    scratch.localPos.y = D_8010B1B0.y;
    scratch.localPos.z = -obj->depth;
    func_80097FE4(scratch.rotationMtx, obj->pitch, obj->yaw);
    func_80098590(scratch.rotationMtx, &scratch.localPos.x, &scratch.worldPos.x);
    obj = D_800EC9C4;
    obj->cameraDelta.x = scratch.worldPos.x - obj->pos.x;
    obj = D_800EC9C4;
    obj->cameraDelta.y = scratch.worldPos.y - obj->pos.y;
    obj = D_800EC9C4;
    obj->cameraDelta.z = scratch.worldPos.z - obj->pos.z;
}

void func_8003DFB0(void) {
    func_8003DEC8();
}

#pragma GLOBAL_ASM("asm/nonmatchings/main_menu_race_setup/func_8003DFD0.s")

void func_8003E3AC(void) {
    D_801235B8->fade--;
    if (D_801235B8->fade == 0) {
        func_800720E4(4);
        D_801235B8->fade = 0x12C;
        func_8009956C(func_8003E45C, 0);
    }
    func_80071664(func_8005393C, 5, 0x64, 0);
    func_8006D780(0);
    func_8006D780(1);
    func_8006D780(2);
    func_8007105C();
}

void func_8003E45C(void) {
    MainMenuRaceSetupState **state;
    MainMenuRaceSetupState *currentState;

    D_800DEF14 -= 8;
    if (D_800DEF14 < 0) {
        D_800DEF14 = 0;
    }
    state = &D_801235B8;
    currentState = *state;
    currentState->fade -= 1;
    if ((*state)->fade == 0) {
        func_80072114(0x7E);
        func_8009956C(func_8003E514, 0);
    }
    func_80071664(func_8005393C, 5, 0x64, 0);
    func_8006D780(0);
    func_8006D780(1);
    func_8007105C();
}

void func_8003E514(void) {
    D_800DEF14 += 4;
    if (D_800DEF14 >= 0xFF) {
        D_801235B8->fade = 0xFF;
        D_80123751 = 1;
        func_8009956C(func_8003E5A8, 0);
    }
    func_80071664(func_8005393C, 5, 0x64, 0);
    func_8006D780(0);
    func_8006D780(1);
    func_8007105C();
}

void func_8003E5A8(void) {
    if (D_80123750 == 2) {
        func_80045914();
        D_80123751 = 0;
        D_800DEED4 = 0;
        func_80099658(2);
        func_8009954C(4);
    }
}
