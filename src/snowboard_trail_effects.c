#include "common.h"

#define SNOWBOARD_TRAIL_TIMER 0xF0
#define SNOWBOARD_TRAIL_FLAG_FACING_BACKWARD 0x400

typedef struct {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
    /* 0x08 */ s32 z;
} Vec3i;

typedef struct {
    /* 0x00 */ u32 w0;
    /* 0x04 */ u32 w1;
} SnowboardTrailGfx;

typedef struct {
    /* 0x00 */ s16 state;
    /* 0x02 */ u8 pad02[0x04 - 0x02];
    /* 0x04 */ Vec3i localOffset;
    /* 0x10 */ Vec3i scale;
    /* 0x1C */ Vec3i worldPos;
    /* 0x28 */ s16 rotation[9];
    /* 0x3A */ u8 pad3A[0x3C - 0x3A];
    /* 0x3C */ Vec3i drawPos;
    /* 0x48 */ s16 transform[16];
    /* 0x68 */ s16 modelYaw;
    /* 0x6A */ s16 spinYaw;
    /* 0x6C */ u8 pad6C[0x74 - 0x6C];
    /* 0x74 */ s16 scaleStep;
} SnowboardTrailState;

typedef struct {
    /* 0x000 */ u8 pad000[0x014 - 0x000];
    /* 0x014 */ s8 disabled;
    /* 0x015 */ u8 pad015[0x028 - 0x015];
    /* 0x028 */ u8 trailFrontSource[0x20];
    /* 0x048 */ u8 trailBackSource[0x20];
    /* 0x068 */ u8 pad068[0x06C - 0x068];
    /* 0x06C */ void *trailFrontDisplayList;
    /* 0x070 */ void *trailBackDisplayList;
    /* 0x074 */ u8 pad074[0x094 - 0x074];
    /* 0x094 */ s16 modelTransform[10];
    /* 0x0A8 */ Vec3i velocity;
    /* 0x0B4 */ u8 pad0B4[0x2DA - 0x0B4];
    /* 0x2DA */ s16 trailTimer;
    /* 0x2DC */ u8 trailSide;
    /* 0x2DD */ u8 pad2DD[0x2FC - 0x2DD];
    /* 0x2FC */ s32 flags;
    /* 0x300 */ u8 pad300[0x58C - 0x300];
    /* 0x58C */ SnowboardTrailState trail;
} SnowboardTrailPlayer;

extern void *func_80043040(s16 assetId);
extern void func_800483FC(void *queue, void (*callback)(SnowboardTrailPlayer *), SnowboardTrailState *trail);
extern void *func_8004885C(u8 *source);
extern void func_8005F5C8(SnowboardTrailPlayer *player);
extern void func_80097BAC(s16 *mtx, s16 rotY);
extern void func_80097C18(s16 *mtx, s16 rotY);
extern void func_80097CF0(s16 *mtx, s16 *source, s16 *dest);
extern void func_80098590(s16 *mtx, Vec3i *source, Vec3i *dest);
extern void func_800987A0(s16 *mtx, s16 *rotation, Vec3i *scale, s16 *dest);

extern u8 D_80121B56;
extern s16 D_80112144;
extern s16 D_80112146;
extern SnowboardTrailGfx *D_80124830;
extern void *D_801248BC;
extern u8 D_80156609;

#pragma GLOBAL_ASM("asm/nonmatchings/snowboard_trail_effects/func_800837D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/snowboard_trail_effects/func_8008393C.s")

void func_80083CFC(SnowboardTrailPlayer *player) {
    SnowboardTrailState *trail = &player->trail;

    player->trailTimer = SNOWBOARD_TRAIL_TIMER;
    if (player->flags & SNOWBOARD_TRAIL_FLAG_FACING_BACKWARD) {
        trail->localOffset.x = 0x300000;
        trail->localOffset.y = 0;
        trail->localOffset.z = 0;
        trail->modelYaw = 0;
        player->trailSide = 1;
    } else {
        trail->localOffset.x = -0x300000;
        trail->localOffset.y = 0;
        trail->localOffset.z = 0;
        trail->modelYaw = 0x800;
        player->trailSide = 0;
    }
    trail->spinYaw = 0;
    trail->scale.x = 0xF0000;
    trail->scale.y = 0x230000;
    trail->scale.z = 0;
    trail->scaleStep = 1;
    trail->state = 1;
}
