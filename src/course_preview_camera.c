#include "common.h"

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i;

typedef struct CoursePreviewCamera CoursePreviewCamera;
typedef void (*CoursePreviewCameraCallback)(CoursePreviewCamera *);

typedef union {
    s32 word;
    struct {
        s16 yaw;
        s16 pitch;
    } half;
} PackedAngles;

struct CoursePreviewCamera {
    /* 0x00 */ char pad0[0x8];
    /* 0x08 */ CoursePreviewCameraCallback callback;
    /* 0x0C */ char padC[0x4];
    /* 0x10 */ u16 index;
    /* 0x12 */ char pad12[0x6];
    /* 0x18 */ Vec3i position;
    /* 0x24 */ s32 velocityY;
    /* 0x28 */ s32 radius;
    /* 0x2C */ PackedAngles angle;
    /* 0x30 */ s16 scale;
    /* 0x32 */ s16 tilt;
    /* 0x34 */ s16 pitchVelocity;
    /* 0x36 */ s16 spinVelocity;
    /* 0x38 */ s16 timer;
    /* 0x3A */ s16 stateTimer;
    /* 0x3C */ s32 displayList0;
    /* 0x40 */ s32 displayList1;
    /* 0x44 */ s8 displayListValid;
};

extern void *D_801248D4;
extern void func_800483FC(void *, void *, s32);
extern void func_800556B0(void);
extern void func_80056CA0(void);
extern s16 D_8011216A;
extern s32 func_80043040(s16 arg0);
extern s32 func_800430D0(void);
extern void func_80045990(s32 arg0, s32 arg1, s16 *arg2, s16 *arg3);
extern void func_80097C18(void *, s32);
extern void func_80098590(void *, s32 *, Vec3i *);
extern Vec3i D_800D5CC8[];
void func_80055FA4(CoursePreviewCamera *arg0);
void func_80056070(CoursePreviewCamera *arg0);
extern void func_80055C7C(void);

extern void func_80056348(CoursePreviewCamera *arg0, s16 arg1);
extern void func_800563C4(CoursePreviewCamera *arg0, s32 arg1);
extern void func_800564A8(CoursePreviewCamera *arg0);
extern void func_80071824(CoursePreviewCamera *arg0, CoursePreviewCameraCallback arg1);
extern s8 D_80122288;
extern s16 D_80122282;
extern s16 D_80121B50;

void func_8005663C(CoursePreviewCamera *);
void func_8005673C(CoursePreviewCamera *);
void func_80056998(CoursePreviewCamera *);
void func_80056A58(CoursePreviewCamera *);
void func_80056B1C(CoursePreviewCamera *);
void func_80056BD8(CoursePreviewCamera *);

#pragma GLOBAL_ASM("asm/nonmatchings/course_preview_camera/func_800556B0.s")

void func_80055B04(s32 arg0) {
    func_800483FC(&D_801248D4, func_800556B0, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/course_preview_camera/func_80055B34.s")

#pragma GLOBAL_ASM("asm/nonmatchings/course_preview_camera/func_80055C7C.s")

void func_80055FA4(CoursePreviewCamera *arg0) {
    Vec3i sp44;
    char sp24[0x20];
    CoursePreviewCamera *temp_s0 = arg0;

    arg0->timer--;
    if (arg0->timer == 0) {
        arg0->timer = func_800430D0() + 0x1E;
        func_80071824(arg0, func_80056070);
    }
    func_80097C18(sp24, 0x6D0);
    func_80098590(sp24, &temp_s0->velocityY, &sp44);
    temp_s0->position.x += sp44.x;
    temp_s0->position.y += sp44.y;
    temp_s0->position.z += sp44.z;
    temp_s0->radius -= 0x4000;
    func_800483FC(&D_801248D4, func_80055C7C, (s32) temp_s0);
}

void func_80056070(CoursePreviewCamera *arg0) {
    Vec3i *temp_t3;

    arg0->timer--;
    if (!arg0->timer) {
        arg0->timer = 0x5A;
        temp_t3 = &D_800D5CC8[arg0->index];
        arg0->position = *(&D_800D5CC8[arg0->index]);
        arg0->velocityY = 0;
        arg0->radius = 0;
        arg0->angle.word = 0xFFF00000;
        func_80071824(arg0, func_80055FA4);
    }
}

void func_800560F4(CoursePreviewCamera *arg0) {
    arg0->timer = (arg0->index * 0x1E) + 0x1E;
    func_80045990(func_80043040(D_8011216A), (arg0->index + 3) & 0xFFFF, &arg0->scale, &arg0->pitchVelocity);
    func_80071824(arg0, func_80056070);
}

#pragma GLOBAL_ASM("asm/nonmatchings/course_preview_camera/func_8005616C.s")

void func_80056348(CoursePreviewCamera *arg0, s16 arg1) {
    s16 diff = arg1 - arg0->timer;

    if (diff >= 5) {
        diff = 4;
    }
    if (diff < -4) {
        diff = -4;
    }
    arg0->timer += diff;
}

void func_8005638C(CoursePreviewCamera *arg0, s32 arg1) {
    s32 diff = arg1 - arg0->velocityY;

    if (diff >= 0x2001) {
        diff = 0x2000;
    }
    if (diff < -0x2000) {
        diff = -0x2000;
    }
    arg0->velocityY += diff;
}

#pragma GLOBAL_ASM("asm/nonmatchings/course_preview_camera/func_800563C4.s")

void func_80056444(CoursePreviewCamera *arg0, s16 arg1) {
    s16 diff = arg1 - arg0->pitchVelocity;

    if (diff >= 5) {
        diff = 4;
    }
    if (diff < -4) {
        diff = -4;
    }

    arg0->pitchVelocity += diff;
    arg0->angle.half.pitch += (((-arg0->pitchVelocity * 2) - arg0->angle.half.pitch) >> 3);
}

#pragma GLOBAL_ASM("asm/nonmatchings/course_preview_camera/func_800564A8.s")

void func_800565E8(CoursePreviewCamera *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, 0);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, 0xA);
    func_800564A8(arg0);
}

void func_8005663C(CoursePreviewCamera *arg0) {
    arg0->position.x = 0xB51A13A3;
    arg0->position.y = 0xD0E85B43;
    arg0->position.z = 0x9A645264;
    arg0->scale = 0;
    arg0->angle.half.yaw = 0x17A0;
    arg0->angle.half.pitch = 0;
    arg0->timer = 0x130;
    arg0->pitchVelocity = 0;
    arg0->velocityY = 0;
    arg0->radius = 0x100000;
    arg0->stateTimer = 0;
    if (D_80122288 == 2) {
        func_80071824(arg0, func_800565E8);
    }
}

void func_800566C0(CoursePreviewCamera *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, -0x20000);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, -3);
    func_800564A8(arg0);
    arg0->stateTimer--;
    if (arg0->stateTimer == 0) {
        func_80071824(arg0, func_8005663C);
    }
}

void func_8005673C(CoursePreviewCamera *arg0) {
    arg0->position.x = 0x05CA84CF;
    arg0->position.y = 0x00D0C976;
    arg0->position.z = 0xFAA3DA4A;
    arg0->scale = 0;
    arg0->angle.half.yaw = 0x15E0;
    arg0->angle.half.pitch = 0;
    arg0->timer = 0x130;
    arg0->pitchVelocity = 0;
    arg0->velocityY = 0;
    arg0->radius = 0x100000;
    arg0->stateTimer = 0x154;
    if (D_80122288 == 1) {
        func_80071824(arg0, func_800566C0);
    }
}

void func_800567C4(CoursePreviewCamera *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, 0);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, 0);
    func_800564A8(arg0);
    arg0->stateTimer--;
    if (arg0->stateTimer == 0) {
        func_80071824(arg0, func_8005673C);
    }
}

void func_80056840(CoursePreviewCamera *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, 0);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, 0x30);
    func_800564A8(arg0);
    arg0->stateTimer--;
    if (arg0->stateTimer == 0) {
        arg0->stateTimer = 0x96;
        func_80071824(arg0, func_800567C4);
    }
}

void func_800568C0(CoursePreviewCamera *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, 0x20000);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, 0);
    arg0->stateTimer--;
    func_800564A8(arg0);
    if (arg0->stateTimer == 0) {
        arg0->stateTimer = 0x2A;
        func_80071824(arg0, func_80056840);
    }
}

void func_80056940(CoursePreviewCamera *arg0) {
    func_80056348(arg0, 0x130);
    func_800564A8(arg0);
    if (D_80122282 == 0x35) {
        arg0->stateTimer = 0x6A;
        func_80071824(arg0, func_800568C0);
    }
}

void func_80056998(CoursePreviewCamera *arg0) {
    arg0->position.x = 0xE6C45F50;
    arg0->position.y = 0xED3C9CFB;
    arg0->position.z = 0xD14CD682;
    arg0->scale = 0;
    arg0->angle.half.yaw = 0x1610;
    arg0->angle.half.pitch = 0;
    arg0->timer = 0;
    arg0->pitchVelocity = 0;
    arg0->velocityY = 0;
    arg0->stateTimer = 0x1E;
    func_80071824(arg0, func_80056940);
}

void func_80056A04(CoursePreviewCamera *arg0) {
    func_80056348(arg0, 0x130);
    func_8005638C(arg0, 0);
    func_800563C4(arg0, 0x100000);
    func_80056444(arg0, 0x18);
    func_800564A8(arg0);
}

void func_80056A58(CoursePreviewCamera *arg0) {
    arg0->position.x = 0xF049BD62;
    arg0->position.y = 0xF0E87871;
    arg0->position.z = 0xDA07DE30;
    arg0->scale = 0;
    arg0->angle.half.yaw = 0x1070;
    arg0->angle.half.pitch = 0;
    arg0->timer = 0;
    arg0->pitchVelocity = 0;
    arg0->velocityY = 0;
    arg0->radius = 0x100000;
    arg0->stateTimer = 0x1E;
    func_80071824(arg0, func_80056A04);
}

void func_80056ACC(CoursePreviewCamera *arg0) {
    func_80056348(arg0, 0x130);
    func_800564A8(arg0);
    if (D_80122288 == 2) {
        func_80071824(arg0, func_80056A58);
    }
}

void func_80056B1C(CoursePreviewCamera *arg0) {
    arg0->position.x = 0xAB4FC576;
    arg0->position.y = 0xE13FBC73;
    arg0->position.z = 0xD2B26423;
    arg0->scale = 0;
    arg0->angle.half.yaw = 0xDF0;
    arg0->angle.half.pitch = 0;
    arg0->timer = 0;
    arg0->pitchVelocity = 0;
    arg0->velocityY = 0;
    arg0->stateTimer = 0x1E;
    func_80071824(arg0, func_80056ACC);
}

void func_80056B88(CoursePreviewCamera *arg0) {
    func_80056348(arg0, 0x30);
    func_800564A8(arg0);
    if (D_80122288 == 1) {
        func_80071824(arg0, func_80056B1C);
    }
}

void func_80056BD8(CoursePreviewCamera *arg0) {
    arg0->position.x = 0xAB4FC576;
    arg0->position.y = 0xE0CEDC73;
    arg0->position.z = 0xD2B26423;
    arg0->scale = 0;
    arg0->angle.half.yaw = 0xDF0;
    arg0->angle.half.pitch = 0;
    arg0->timer = 0;
    arg0->pitchVelocity = 0;
    arg0->velocityY = 0;
    arg0->stateTimer = 0x1E;
    func_80071824(arg0, func_80056B88);
}

void func_80056C44(CoursePreviewCamera *arg0) {
    s16 temp_v0 = D_80121B50;

    if (temp_v0 == 3) {
        func_80071824(arg0, func_80056998);
        temp_v0 = D_80121B50;
    }
    if (temp_v0 == 6) {
        func_80071824(arg0, func_80056BD8);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/course_preview_camera/func_80056CA0.s")

void func_8005711C(s32 arg0) {
    func_800483FC(&D_801248D4, func_80056CA0, arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/course_preview_camera/func_8005714C.s")
