#include "common.h"

#define FIXED_MATRIX_ONE 0x1000

extern s16 D_800B9810[];

typedef struct {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 z;
} Vec3i;

typedef enum {
    MTX_XX,
    MTX_XY,
    MTX_XZ,
    MTX_YX,
    MTX_YY,
    MTX_YZ,
    MTX_ZX,
    MTX_ZY,
    MTX_ZZ
} FixedMatrixIndex;

typedef s16 FixedMatrix3s[9];
typedef s16 FixedMatrix3sScratch[0x10];

typedef struct {
    /* 0x00 */ FixedMatrix3s rotation;
    /* 0x12 */ s16 pad12;
    /* 0x14 */ Vec3i translation;
} FixedTransform;

s16 func_80097B48(s16 arg0);
void func_80097BAC(FixedMatrix3s arg0, s16 arg1);
void func_80097C18(FixedMatrix3s arg0, s16 arg1);
void func_80097C84(FixedMatrix3s arg0, s16 arg1);
void func_80097CF0(FixedMatrix3s arg0, FixedMatrix3s arg1, FixedMatrix3s arg2);

extern FixedTransform D_800DEE30;

void func_80097A80(FixedTransform *arg0) {
    *arg0 = D_800DEE30;
}

s16 func_80097AE8(s16 arg0) {
    s16 temp;

    arg0 &= 0xFFF;
    if (arg0 == 0x400) {
        return FIXED_MATRIX_ONE;
    }
    if (arg0 == 0xC00) {
        return -FIXED_MATRIX_ONE;
    }
    temp = D_800B9810[arg0] >> 3;
    return temp;
}

s16 func_80097B48(s16 arg0) {
    s16 temp;

    arg0 = (arg0 + 0x400) & 0xFFF;
    if (arg0 == 0x400) {
        return FIXED_MATRIX_ONE;
    }
    if (arg0 == 0xC00) {
        return -FIXED_MATRIX_ONE;
    }
    temp = D_800B9810[arg0] >> 3;
    return temp;
}

void func_80097BAC(FixedMatrix3s arg0, s16 arg1) {
    s32 sine = func_80097AE8(arg1);
    s16 cosine = func_80097B48(arg1);

    arg0[MTX_XX] = FIXED_MATRIX_ONE;
    arg0[MTX_XY] = 0;
    arg0[MTX_XZ] = 0;
    arg0[MTX_YX] = 0;
    arg0[MTX_YY] = cosine;
    arg0[MTX_YZ] = sine;
    arg0[MTX_ZX] = 0;
    arg0[MTX_ZY] = -sine;
    arg0[MTX_ZZ] = cosine;
}

void func_80097C18(FixedMatrix3s arg0, s16 arg1) {
    s32 sine = func_80097AE8(arg1);
    s16 cosine = func_80097B48(arg1);

    arg0[MTX_XX] = cosine;
    arg0[MTX_XY] = 0;
    arg0[MTX_XZ] = -sine;
    arg0[MTX_YX] = 0;
    arg0[MTX_YY] = FIXED_MATRIX_ONE;
    arg0[MTX_YZ] = 0;
    arg0[MTX_ZX] = sine;
    arg0[MTX_ZY] = 0;
    arg0[MTX_ZZ] = cosine;
}

void func_80097C84(FixedMatrix3s arg0, s16 arg1) {
    s32 sine = func_80097AE8(arg1);
    s16 cosine = func_80097B48(arg1);

    arg0[MTX_XX] = cosine;
    arg0[MTX_XY] = sine;
    arg0[MTX_XZ] = 0;
    arg0[MTX_YX] = -sine;
    arg0[MTX_YY] = cosine;
    arg0[MTX_YZ] = 0;
    arg0[MTX_ZX] = 0;
    arg0[MTX_ZY] = 0;
    arg0[MTX_ZZ] = FIXED_MATRIX_ONE;
}

#pragma GLOBAL_ASM("asm/nonmatchings/fixed_point_matrix/func_80097CF0.s")

void func_80097DA4(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3) {
    s32 sineX;
    s32 cosineX;
    s32 sineY;
    s32 cosineY;
    s32 sineZ;
    s32 cosineZ;
    s32 negSineY;
    s32 negSineZ;
    s32 sineXTimesSineY;
    s32 cosineXTimesSineY;

    sineX = func_80097AE8(arg1);
    cosineX = func_80097B48(arg1);
    sineY = func_80097AE8(arg2);
    cosineY = func_80097B48(arg2);
    sineZ = func_80097AE8(arg3);
    cosineZ = func_80097B48(arg3);
    negSineY = -sineY;
    negSineZ = -sineZ;

    arg0[MTX_XX] = (cosineY * cosineZ) / FIXED_MATRIX_ONE;
    arg0[MTX_XY] = (cosineY * sineZ) / FIXED_MATRIX_ONE;
    sineXTimesSineY = (sineX * sineY) / FIXED_MATRIX_ONE;
    arg0[MTX_XZ] = negSineY;
    arg0[MTX_YX] = ((sineXTimesSineY * cosineZ) / FIXED_MATRIX_ONE) + ((cosineX * negSineZ) / FIXED_MATRIX_ONE);
    arg0[MTX_YY] = ((sineXTimesSineY * sineZ) / FIXED_MATRIX_ONE) + ((cosineX * cosineZ) / FIXED_MATRIX_ONE);
    arg0[MTX_YZ] = (sineX * cosineY) / FIXED_MATRIX_ONE;
    cosineXTimesSineY = (cosineX * sineY) / FIXED_MATRIX_ONE;
    arg0[MTX_ZX] = ((cosineXTimesSineY * cosineZ) / FIXED_MATRIX_ONE) + ((sineX * sineZ) / FIXED_MATRIX_ONE);
    arg0[MTX_ZY] = ((cosineXTimesSineY * sineZ) / FIXED_MATRIX_ONE) + (((-sineX) * cosineZ) / FIXED_MATRIX_ONE);
    arg0[MTX_ZZ] = (cosineX * cosineY) / FIXED_MATRIX_ONE;
}

void func_80097FE4(FixedMatrix3s arg0, s16 arg1, s16 arg2) {
    s32 sineX;
    s32 cosineX;
    s32 sineY;
    s32 cosineY;
    s32 negSineX;
    s32 negSineY;

    sineX = func_80097AE8(arg1);
    cosineX = func_80097B48(arg1);
    sineY = func_80097AE8(arg2);
    cosineY = func_80097B48(arg2);
    negSineX = -sineX;
    negSineY = -sineY;

    arg0[MTX_XX] = cosineY;
    arg0[MTX_XY] = 0;
    arg0[MTX_XZ] = negSineY;
    arg0[MTX_ZY] = negSineX;
    arg0[MTX_YY] = cosineX;
    arg0[MTX_YX] = (sineX * sineY) / FIXED_MATRIX_ONE;
    arg0[MTX_YZ] = (sineX * cosineY) / FIXED_MATRIX_ONE;
    arg0[MTX_ZX] = (cosineX * sineY) / FIXED_MATRIX_ONE;
    arg0[MTX_ZY] = negSineX;
    arg0[MTX_ZZ] = (cosineX * cosineY) / FIXED_MATRIX_ONE;
}

void func_800980D0(FixedMatrix3s arg0, s16 arg1, s16 arg2) {
    FixedMatrix3sScratch sp38;
    FixedMatrix3sScratch sp18;

    func_80097C84(sp38, arg2);
    func_80097BAC(sp18, arg1);
    func_80097CF0(sp38, sp18, arg0);
}

void func_80098124(FixedMatrix3s arg0, s16 arg1, s16 arg2) {
    FixedMatrix3sScratch sp38;
    FixedMatrix3sScratch sp18;

    func_80097BAC(sp38, arg1);
    func_80097C84(sp18, arg2);
    func_80097CF0(sp38, sp18, arg0);
}

void func_80098174(FixedMatrix3s arg0, s16 arg1, s16 arg2) {
    FixedMatrix3sScratch sp38;
    FixedMatrix3sScratch sp18;

    func_80097C84(sp38, arg2);
    func_80097C18(sp18, arg1);
    func_80097CF0(sp38, sp18, arg0);
}

void func_800981C8(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3) {
    s32 sineX;
    s32 cosineX;
    s32 sineY;
    s32 cosineY;
    s32 sineZ;
    s32 cosineZ;
    s32 negSineY;
    s32 negSineZ;
    s32 sineZTimesSineX;
    s32 cosineZTimesSineX;

    sineX = func_80097AE8(arg1);
    cosineX = func_80097B48(arg1);
    sineY = func_80097AE8(arg2);
    cosineY = func_80097B48(arg2);
    sineZ = func_80097AE8(arg3);
    cosineZ = func_80097B48(arg3);
    negSineY = -sineY;
    negSineZ = -sineZ;
    sineZTimesSineX = (sineZ * sineX) / FIXED_MATRIX_ONE;

    arg0[MTX_XX] = ((cosineZ * cosineY) + (((sineZ * sineX) / FIXED_MATRIX_ONE) * sineY)) / FIXED_MATRIX_ONE;
    arg0[MTX_XY] = (sineZ * cosineX) / FIXED_MATRIX_ONE;
    arg0[MTX_XZ] = ((negSineY * cosineZ) + (sineZTimesSineX * cosineY)) / FIXED_MATRIX_ONE;
    cosineZTimesSineX = (cosineZ * sineX) / FIXED_MATRIX_ONE;
    arg0[MTX_YX] = ((negSineZ * cosineY) + (cosineZTimesSineX * sineY)) / FIXED_MATRIX_ONE;
    arg0[MTX_YY] = (cosineZ * cosineX) / FIXED_MATRIX_ONE;
    arg0[MTX_YZ] = ((negSineZ * negSineY) + (cosineZTimesSineX * cosineY)) / FIXED_MATRIX_ONE;
    arg0[MTX_ZX] = (cosineX * sineY) / FIXED_MATRIX_ONE;
    arg0[MTX_ZY] = -sineX;
    arg0[MTX_ZZ] = (cosineX * cosineY) / FIXED_MATRIX_ONE;
}

void func_800983E4(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3) {
    FixedMatrix3sScratch sp58;
    FixedMatrix3sScratch sp38;
    FixedMatrix3sScratch sp18;

    func_80097C18(sp58, arg2);
    func_80097C84(sp38, arg3);
    func_80097CF0(sp58, sp38, sp18);
    func_80097BAC(sp38, arg1);
    func_80097CF0(sp18, sp38, arg0);
}

void func_80098458(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3) {
    FixedMatrix3sScratch sp58;
    FixedMatrix3sScratch sp38;
    FixedMatrix3sScratch sp18;

    func_80097C18(sp58, arg2);
    func_80097C84(sp38, arg3);
    func_80097CF0(sp38, sp58, sp18);
    func_80097BAC(sp38, arg1);
    func_80097CF0(sp18, sp38, arg0);
}

void func_800984CC(FixedMatrix3s arg0, s16 arg1, s16 arg2, s16 arg3) {
    FixedMatrix3sScratch sp58;
    FixedMatrix3sScratch sp38;
    FixedMatrix3sScratch sp18;

    func_80097BAC(sp58, arg1);
    func_80097C84(sp38, arg3);
    func_80097CF0(sp58, sp38, sp18);
    func_80097C18(sp38, arg2);
    func_80097CF0(sp18, sp38, arg0);
}

void func_8009853C(FixedMatrix3s arg0, s16 arg1, s16 arg2) {
    FixedMatrix3sScratch sp38;
    FixedMatrix3sScratch sp18;

    func_80097C18(sp38, arg2);
    func_80097BAC(sp18, arg1);
    func_80097CF0(sp38, sp18, arg0);
}

void func_80098590(FixedMatrix3s arg0, Vec3i *source, Vec3i *dest) {
    dest->x = (s64)arg0[MTX_XX] * source->x / FIXED_MATRIX_ONE +
              (s64)arg0[MTX_YX] * source->y / FIXED_MATRIX_ONE +
              (s64)arg0[MTX_ZX] * source->z / FIXED_MATRIX_ONE;
    dest->y = (s64)arg0[MTX_XY] * source->x / FIXED_MATRIX_ONE +
              (s64)arg0[MTX_YY] * source->y / FIXED_MATRIX_ONE +
              (s64)arg0[MTX_ZY] * source->z / FIXED_MATRIX_ONE;
    dest->z = (s64)arg0[MTX_XZ] * source->x / FIXED_MATRIX_ONE +
              (s64)arg0[MTX_YZ] * source->y / FIXED_MATRIX_ONE +
              (s64)arg0[MTX_ZZ] * source->z / FIXED_MATRIX_ONE;
}

void func_800987A0(FixedTransform *arg0, FixedTransform *arg1, FixedTransform *arg2) {
    arg2->translation.x = (s64)arg1->rotation[MTX_XX] * arg0->translation.x / FIXED_MATRIX_ONE +
                           (s64)arg1->rotation[MTX_YX] * arg0->translation.y / FIXED_MATRIX_ONE +
                           (s64)arg1->rotation[MTX_ZX] * arg0->translation.z / FIXED_MATRIX_ONE;
    arg2->translation.y = (s64)arg1->rotation[MTX_XY] * arg0->translation.x / FIXED_MATRIX_ONE +
                           (s64)arg1->rotation[MTX_YY] * arg0->translation.y / FIXED_MATRIX_ONE +
                           (s64)arg1->rotation[MTX_ZY] * arg0->translation.z / FIXED_MATRIX_ONE;
    arg2->translation.z = (s64)arg1->rotation[MTX_XZ] * arg0->translation.x / FIXED_MATRIX_ONE +
                           (s64)arg1->rotation[MTX_YZ] * arg0->translation.y / FIXED_MATRIX_ONE +
                           (s64)arg1->rotation[MTX_ZZ] * arg0->translation.z / FIXED_MATRIX_ONE;
    arg2->translation.x += arg1->translation.x;
    arg2->translation.y += arg1->translation.y;
    arg2->translation.z += arg1->translation.z;
    func_80097CF0(arg0->rotation, arg1->rotation, arg2->rotation);
}

void func_800989EC(FixedTransform *arg0, FixedTransform *arg1, FixedTransform *arg2) {
    arg2->translation.x = (s64)arg1->rotation[MTX_XX] * arg0->translation.x / FIXED_MATRIX_ONE +
                           (s64)arg1->rotation[MTX_YX] * arg0->translation.y / FIXED_MATRIX_ONE +
                           (s64)arg1->rotation[MTX_ZX] * arg0->translation.z / FIXED_MATRIX_ONE;
    arg2->translation.y = (s64)arg1->rotation[MTX_XY] * arg0->translation.x / FIXED_MATRIX_ONE +
                           (s64)arg1->rotation[MTX_YY] * arg0->translation.y / FIXED_MATRIX_ONE +
                           (s64)arg1->rotation[MTX_ZY] * arg0->translation.z / FIXED_MATRIX_ONE;
    arg2->translation.z = (s64)arg1->rotation[MTX_XZ] * arg0->translation.x / FIXED_MATRIX_ONE +
                           (s64)arg1->rotation[MTX_YZ] * arg0->translation.y / FIXED_MATRIX_ONE +
                           (s64)arg1->rotation[MTX_ZZ] * arg0->translation.z / FIXED_MATRIX_ONE;
    arg2->translation.x += arg1->translation.x;
    arg2->translation.y += arg1->translation.y;
    arg2->translation.z += arg1->translation.z;
}

#pragma GLOBAL_ASM("asm/nonmatchings/fixed_point_matrix/func_80098C30.s")
