#include "game/math/fixed_point_math.h"

#define FIXED_MATRIX_ONE 0x1000
#define FIXED_MATRIX_ROWS(matrix) ((s16(*)[3])(matrix))

Transform3D gIdentityFixedTransform = {
    {
     FIXED_MATRIX_ONE, 0,
     0, 0,
     FIXED_MATRIX_ONE, 0,
     0, 0,
     FIXED_MATRIX_ONE, },
    { 0, 0, 0 },
};

/* N64 s15.16 identity matrix in the packed integer/fraction word layout. */
Mtx gIdentityMatrix = {
    {
     { 0x00010000, 0x00000000, 0x00000001, 0x00000000 },
     { 0x00000000, 0x00010000, 0x00000000, 0x00000001 },
     { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
     { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
     },
};

/* Packed matrix template patched with the player's shadow translation. */
Mtx gRacePlayerShadowMatrixTemplate = {
    {
     { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
     { 0x00000000, 0x00000000, 0x00000000, 0x00000001 },
     { 0x40000000, 0x00000000, 0x00004000, 0x00000000 },
     { 0x00000000, 0x40000000, 0x00000000, 0x00000000 },
     },
};

void initFixedTransform(Transform3D *transform) {
    *transform = gIdentityFixedTransform;
}

s16 fixedSine(s16 arg0) {
    s16 temp;

    arg0 &= 0xFFF;
    if (arg0 == 0x400) {
        return FIXED_MATRIX_ONE;
    }
    if (arg0 == 0xC00) {
        return -FIXED_MATRIX_ONE;
    }
    temp = gSineTable[arg0] >> 3;
    return temp;
}

s16 fixedCosine(s16 arg0) {
    s16 temp;

    arg0 = (arg0 + 0x400) & 0xFFF;
    if (arg0 == 0x400) {
        return FIXED_MATRIX_ONE;
    }
    if (arg0 == 0xC00) {
        return -FIXED_MATRIX_ONE;
    }
    temp = gSineTable[arg0] >> 3;
    return temp;
}

void makeFixedRotationX(Mat3x3 arg0, s16 arg1) {
    s32 sine = fixedSine(arg1);
    s16 cosine = fixedCosine(arg1);

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

void makeFixedRotationY(Mat3x3 arg0, s16 arg1) {
    s32 sine = fixedSine(arg1);
    s16 cosine = fixedCosine(arg1);

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

void makeFixedRotationZ(Mat3x3 arg0, s16 arg1) {
    s32 sine = fixedSine(arg1);
    s16 cosine = fixedCosine(arg1);

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

void multiplyFixedMatrix3s(Mat3x3 arg0, Mat3x3 arg1, Mat3x3 arg2) {
    s32 i;
    s32 j;

    i = 0;
    do {
        j = 0;
        do {
            FIXED_MATRIX_ROWS(arg2)
            [i][j] = ((FIXED_MATRIX_ROWS(arg0)[i][0] * FIXED_MATRIX_ROWS(arg1)[0][j]) / FIXED_MATRIX_ONE) +
                     ((FIXED_MATRIX_ROWS(arg0)[i][1] * FIXED_MATRIX_ROWS(arg1)[1][j]) / FIXED_MATRIX_ONE) +
                     ((FIXED_MATRIX_ROWS(arg0)[i][2] * FIXED_MATRIX_ROWS(arg1)[2][j]) / FIXED_MATRIX_ONE);
            j++;
        } while (j != 3);
        i++;
    } while (i != 3);
}

void makeFixedRotationXYZ(Mat3x3 arg0, s16 arg1, s16 arg2, s16 arg3) {
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

    sineX = fixedSine(arg1);
    cosineX = fixedCosine(arg1);
    sineY = fixedSine(arg2);
    cosineY = fixedCosine(arg2);
    sineZ = fixedSine(arg3);
    cosineZ = fixedCosine(arg3);
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

void makeFixedRotationXY(Mat3x3 arg0, s16 arg1, s16 arg2) {
    s32 sineX;
    s32 cosineX;
    s32 sineY;
    s32 cosineY;
    s32 negSineX;
    s32 negSineY;

    sineX = fixedSine(arg1);
    cosineX = fixedCosine(arg1);
    sineY = fixedSine(arg2);
    cosineY = fixedCosine(arg2);
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

void makeFixedRotationZX(Mat3x3 arg0, s16 arg1, s16 arg2) {
    s16 sp38[0x10];
    s16 sp18[0x10];

    makeFixedRotationZ(sp38, arg2);
    makeFixedRotationX(sp18, arg1);
    multiplyFixedMatrix3s(sp38, sp18, arg0);
}

void makeFixedRotationXZ(Mat3x3 arg0, s16 arg1, s16 arg2) {
    s16 sp38[0x10];
    s16 sp18[0x10];

    makeFixedRotationX(sp38, arg1);
    makeFixedRotationZ(sp18, arg2);
    multiplyFixedMatrix3s(sp38, sp18, arg0);
}

void makeFixedRotationZY(Mat3x3 arg0, s16 arg1, s16 arg2) {
    s16 sp38[0x10];
    s16 sp18[0x10];

    makeFixedRotationZ(sp38, arg2);
    makeFixedRotationY(sp18, arg1);
    multiplyFixedMatrix3s(sp38, sp18, arg0);
}

void makeFixedRotationZXY(Mat3x3 arg0, s16 arg1, s16 arg2, s16 arg3) {
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

    sineX = fixedSine(arg1);
    cosineX = fixedCosine(arg1);
    sineY = fixedSine(arg2);
    cosineY = fixedCosine(arg2);
    sineZ = fixedSine(arg3);
    cosineZ = fixedCosine(arg3);
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

void makeFixedRotationYZX(Mat3x3 arg0, s16 arg1, s16 arg2, s16 arg3) {
    s16 sp58[0x10];
    s16 sp38[0x10];
    s16 sp18[0x10];

    makeFixedRotationY(sp58, arg2);
    makeFixedRotationZ(sp38, arg3);
    multiplyFixedMatrix3s(sp58, sp38, sp18);
    makeFixedRotationX(sp38, arg1);
    multiplyFixedMatrix3s(sp18, sp38, arg0);
}

void makeFixedRotationZYX(Mat3x3 arg0, s16 arg1, s16 arg2, s16 arg3) {
    s16 sp58[0x10];
    s16 sp38[0x10];
    s16 sp18[0x10];

    makeFixedRotationY(sp58, arg2);
    makeFixedRotationZ(sp38, arg3);
    multiplyFixedMatrix3s(sp38, sp58, sp18);
    makeFixedRotationX(sp38, arg1);
    multiplyFixedMatrix3s(sp18, sp38, arg0);
}

void makeFixedRotationXZY(Mat3x3 arg0, s16 arg1, s16 arg2, s16 arg3) {
    s16 sp58[0x10];
    s16 sp38[0x10];
    s16 sp18[0x10];

    makeFixedRotationX(sp58, arg1);
    makeFixedRotationZ(sp38, arg3);
    multiplyFixedMatrix3s(sp58, sp38, sp18);
    makeFixedRotationY(sp38, arg2);
    multiplyFixedMatrix3s(sp18, sp38, arg0);
}

void makeFixedRotationYX(Mat3x3 arg0, s16 arg1, s16 arg2) {
    s16 sp38[0x10];
    s16 sp18[0x10];

    makeFixedRotationY(sp38, arg2);
    makeFixedRotationX(sp18, arg1);
    multiplyFixedMatrix3s(sp38, sp18, arg0);
}

void transformVec3iByFixedMatrix(Mat3x3 arg0, Vec3i *source, Vec3i *dest) {
    dest->x = (s64)arg0[MTX_XX] * source->x / FIXED_MATRIX_ONE + (s64)arg0[MTX_YX] * source->y / FIXED_MATRIX_ONE +
              (s64)arg0[MTX_ZX] * source->z / FIXED_MATRIX_ONE;
    dest->y = (s64)arg0[MTX_XY] * source->x / FIXED_MATRIX_ONE + (s64)arg0[MTX_YY] * source->y / FIXED_MATRIX_ONE +
              (s64)arg0[MTX_ZY] * source->z / FIXED_MATRIX_ONE;
    dest->z = (s64)arg0[MTX_XZ] * source->x / FIXED_MATRIX_ONE + (s64)arg0[MTX_YZ] * source->y / FIXED_MATRIX_ONE +
              (s64)arg0[MTX_ZZ] * source->z / FIXED_MATRIX_ONE;
}

void composeFixedTransforms(Transform3D *arg0, Transform3D *arg1, Transform3D *arg2) {
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
    multiplyFixedMatrix3s(arg0->rotation, arg1->rotation, arg2->rotation);
}

void composeFixedTransformTranslation(Transform3D *arg0, Transform3D *arg1, Transform3D *arg2) {
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

s32 integerSquareRoot64(s64 arg0) {
    u64 pad;
    u64 bit;
    u64 root;
    s32 shift;

    root = (bit = 0);
    shift = 0x3E;
    do {
        bit += bit;
        root += root;
        if (((u64)arg0 >> shift) > bit) {
            bit++;
            arg0 -= bit << shift;
            bit++;
            root++;
        }
        shift -= 2;
    } while (shift >= 0);
    return root;
}
