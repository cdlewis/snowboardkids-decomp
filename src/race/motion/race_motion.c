#include "game/race/race_state.h"
#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/engine/relocatable_heap.h"
#include "game/math/fixed_point_math.h"
#include "game/race/motion/race_motion.h"
#include "game/race/player/race_player_progress.h"

typedef struct RaceMotionCoord {
    s16 x;
    s16 y;
    s16 z;
} RaceMotionCoord;

typedef struct RaceMotionFace {
    u16 coord0;
    u16 coord1;
    u16 coord2;
    s8 faceIndex;
    u8 unk7;
} RaceMotionFace;

typedef struct RaceMotionCountedTable {
    u16 count;
    u8 data[1];
} RaceMotionCountedTable;

typedef struct RaceMotionSurface {
    s16 nextFaceIndices[2];
    s16 unk4[2];
    s16 coordIndices[4];
    s16 positionIndex;
    s16 angle;
    u16 unk14[4];
} RaceMotionSurface;

typedef struct RaceMotionRotation {
    s32 x;
    s32 y;
    s32 z;
} RaceMotionRotation;

typedef struct RaceMotionPackedJointRotation {
    s16 xy;
    s16 zAndFlags;
} RaceMotionPackedJointRotation;

typedef struct RaceMotionAnimationHeader {
    s16 frameCount;
} RaceMotionAnimationHeader;

typedef struct RaceMotionAnimationAsset {
    u16 animationOffsets[1];
} RaceMotionAnimationAsset;

#define RACE_MOTION_JOINT_COUNT 12
#define RACE_MOTION_FRAME_SAMPLE_COUNT 2
#define RACE_MOTION_PARTIAL_ANIMATION_JOINT_COUNT 5
#define RACE_MOTION_PARTIAL_ANIMATION_START_JOINT 6
#define RACE_MOTION_PACKED_JOINT_ROTATION_SKIP_BYTES 0x24
#define RACE_MOTION_PACKED_JOINT_ROTATION_PREFIX_COUNT 9
#define RACE_MOTION_MODEL_PART_CAPACITY 14
#define RACE_MOTION_MODEL_POSITION_FRAC_BITS 14

typedef struct RaceMotionRootMotion {
    RaceMotionRotation position;
    char padC[8];
    RaceMotionRotation rotation;
} RaceMotionRootMotion;

typedef struct RaceMotionPackedVector {
    s16 x;
    s16 y;
    s16 z;
} RaceMotionPackedVector;

typedef struct RaceMotionPackedRootMotion {
    RaceMotionPackedVector position;
    RaceMotionPackedVector rotation;
} RaceMotionPackedRootMotion;

typedef struct RaceMotionPackedAnimationFrame {
    RaceMotionPackedRootMotion rootMotion;
    RaceMotionPackedJointRotation joints[RACE_MOTION_JOINT_COUNT];
} RaceMotionPackedAnimationFrame;

typedef struct RaceMotionDecodedRootFrame {
    RaceMotionRotation position;
    RaceMotionRotation rotation;
} RaceMotionDecodedRootFrame;

typedef struct RaceMotionDecodedJointFrame {
    RaceMotionRotation joints[RACE_MOTION_JOINT_COUNT];
    RaceMotionRotation stridePadding[2];
} RaceMotionDecodedJointFrame;

typedef struct RaceMotionPartialJointCursor {
    char pad0[0x33A];
    s16 rotationX;
    s16 rotationY;
    s16 rotationZ;
} RaceMotionPartialJointCursor;

typedef struct RaceMotionBlendAnimationState {
    u16 modelId;
    char pad2[0x76];
    RaceMotionPartialJointCursor jointCursor;
} RaceMotionBlendAnimationState;

typedef struct RaceMotionModelPart {
    u8 partId;
    u8 parentPartId;
    s16 rotationX;
    s16 rotationY;
    s16 rotationZ;
    s32 x;
    s32 y;
    s32 z;
} RaceMotionModelPart;

typedef struct RaceMotionStateJoint {
    s32 unk0;
    s16 unk4;
    s16 rotationX;
    s16 rotationY;
    s16 rotationZ;
    s32 unkC;
    s32 unk10;
} RaceMotionStateJoint;

typedef struct RaceMotionPartialAnimationState {
    char pad0[0x334];
    RaceMotionStateJoint joints[RACE_MOTION_JOINT_COUNT];
} RaceMotionPartialAnimationState;

struct RaceMotionState {
    u16 modelId;
    char pad2[0x332];
    union {
        RaceMotionStateJoint joints[12];
        struct {
            char pad334[6];
            s16 jointX;
            s16 jointY;
            s16 jointZ;
            RaceMotionRootMotion rootMotion;
            char pad360[0xC4];
        } motion;
    } animation;
    char pad424[6];
    s16 modelJointX;
    s16 modelJointY;
    s16 modelJointZ;
    char pad430[0xE];
    s16 nextModelJointX;
    s16 nextModelJointY;
    s16 nextModelJointZ;
    char pad444[0xE];
    s16 animationIndex;
    s32 animationStartOffset;
    s32 frameDataOffset;
    s16 frameCount;
    s16 framesRemaining;
    s16 unk460;
    s16 unk462;
    s16 frameTimerReset;
    s16 frameTimer;
};

struct RaceMotionInitState {
    u8 pad0[0x10];
    u8 modelId;
    u8 pad11[0x327];
    RaceMotionModelPart parts[RACE_MOTION_MODEL_PART_CAPACITY];
    s16 partCount;
};

typedef struct RaceCourseSurfaceLimit {
    s16 maxSurfaceIndex;
    char pad2[0x46];
} RaceCourseSurfaceLimit;

extern RaceMotionCoord *gRaceCourseSurfaceCoords;
extern RaceMotionFace *gRaceCourseSurfaceFaces;
extern RaceMotionSurface *gRaceCourseSurfaces;
extern s32 gRaceCourseSurfaceAngleSin;
extern s32 gRaceCourseSurfaceAngleCos;
extern s32 gRaceCourseCollisionDeltaX;
extern s32 gRaceCourseCollisionDeltaZ;
extern s32 gRaceCourseCollisionEdgeDeltaX;
extern s32 gRaceCourseCollisionEdgeDeltaZ;
extern s32 gRaceCourseCollisionAdjustedX;
extern s32 gRaceCourseCollisionAdjustedZ;
extern volatile s32 gRaceMotionRotationFrameBuffer[];
extern volatile s32 gRaceMotionRotationDecodeFrameBuffer[];
extern RaceMotionRotation gRaceMotionJointFrameBuffer[];
extern RaceMotionRotation gRaceMotionJointDecodeFrameBuffer[];
extern s32 gRaceMotionJointBlendBuffer[];
extern s32 gRacePlayerHitCueId;
extern RaceCourseSurfaceLimit gRaceCourseMaxSurfaceIndices[];
extern u8 gRaceMotionModelPartCounts[];
extern u8 *gRaceMotionModelPartIds[];
extern u8 *gRaceMotionModelParentPartIds[];
extern RaceMotionCoord *gRaceMotionModelPartPositions[];

void initRaceCourseSurfaceData(void) {
    RaceMotionCountedTable *coordTable;
    RaceMotionCountedTable *faceTable;

    coordTable = (RaceMotionCountedTable *)getRelocatableHeapBlockBase(gAssetHandles[0x1B]);
    gRaceCourseSurfaceCoords = (RaceMotionCoord *)coordTable->data;

    faceTable = (RaceMotionCountedTable *)&gRaceCourseSurfaceCoords[coordTable->count];
    gRaceCourseSurfaceFaces = (RaceMotionFace *)faceTable->data;

    faceTable = (RaceMotionCountedTable *)&gRaceCourseSurfaceFaces[faceTable->count];
    gRaceCourseSurfaces = (RaceMotionSurface *)faceTable->data;
}

s32 findRaceCourseSurfaceFromHint(s32 surfaceIndex, s32 x, s32 z) {
    s32 z2;
    s32 index;

    index = surfaceIndex;

loop:
    {
        RaceMotionCoord *coords;
        s32 x0;
        s32 x1;
        s32 z3;
        s32 z0;
        s32 x2;
        s32 x3;
        s32 z1;

        coords = gRaceCourseSurfaceCoords;
        x0 = coords[gRaceCourseSurfaces[index].coordIndices[0]].x << 0x11;
        x1 = coords[gRaceCourseSurfaces[index].coordIndices[1]].x << 0x11;
        x2 = coords[gRaceCourseSurfaces[index].coordIndices[2]].x << 0x11;
        x3 = coords[(&gRaceCourseSurfaces[index])->coordIndices[3]].x << 0x11;
        z0 = coords[gRaceCourseSurfaces[index].coordIndices[0]].z << 0x11;
        z1 = coords[gRaceCourseSurfaces[index].coordIndices[1]].z << 0x11;
        z2 = coords[gRaceCourseSurfaces[index].coordIndices[2]].z << 0x11;
        z3 = coords[gRaceCourseSurfaces[index].coordIndices[3]].z << 0x11;

        if ((s64)(x0 - x1) * (z - z1) - (s64)(z0 - z1) * (x - x1) < 0) {
            if (gRaceCourseSurfaces[index].nextFaceIndices[0] >= 0) {
                index = gRaceCourseSurfaces[index].nextFaceIndices[0];
                goto loop;
            }
        }
        if ((s64)(x3 - x2) * (z - z2) - (s64)(z3 - z2) * (x - x2) < 0) {
            if (gRaceCourseSurfaces[index].nextFaceIndices[1] >= 0) {
                index = gRaceCourseSurfaces[index].nextFaceIndices[1];
                goto loop;
            }
        }
        if ((s64)(x2 - x0) * (z - z0) - (s64)(z2 - z0) * (x - x0) < 0) {
            if (gRaceCourseSurfaces[index].unk4[1] >= 0) {
                index = gRaceCourseSurfaces[index].unk4[1];
                goto loop;
            }
        }
        if ((s64)(x1 - x3) * (z - z3) - (s64)(z1 - z3) * (x - x3) < 0) {
            if (gRaceCourseSurfaces[index].unk4[0] >= 0) {
                index = gRaceCourseSurfaces[index].unk4[0];
                goto loop;
            }
        }
    }

    return index;
}

s32 findRaceCourseSurfaceAtPoint(s32 x, s32 z) {
    s32 z2;
    s32 z3;
    s32 surfaceIndex;

    surfaceIndex = 0;
    if (gRaceCourseMaxSurfaceIndices[gRaceCourseIndex.signedValue].maxSurfaceIndex >= 0) {
        do {
            RaceMotionCoord *coords;
            s32 outsideSurface;
            s32 x0;
            s32 x1;
            s32 z0;
            s32 x2;
            s32 x3;
            s32 z1;

            coords = gRaceCourseSurfaceCoords;
            outsideSurface = FALSE;
            x0 = coords[gRaceCourseSurfaces[surfaceIndex].coordIndices[0]].x << 0x11;
            x1 = coords[gRaceCourseSurfaces[surfaceIndex].coordIndices[1]].x << 0x11;
            x2 = coords[gRaceCourseSurfaces[surfaceIndex].coordIndices[2]].x << 0x11;
            x3 = coords[(&gRaceCourseSurfaces[surfaceIndex])->coordIndices[3]].x << 0x11;
            z0 = coords[gRaceCourseSurfaces[surfaceIndex].coordIndices[0]].z << 0x11;
            z1 = coords[gRaceCourseSurfaces[surfaceIndex].coordIndices[1]].z << 0x11;
            z2 = coords[gRaceCourseSurfaces[surfaceIndex].coordIndices[2]].z << 0x11;
            z3 = coords[gRaceCourseSurfaces[surfaceIndex].coordIndices[3]].z << 0x11;

            if ((s64)(x0 - x1) * (z - z1) - (s64)(z0 - z1) * (x - x1) < 0) {
                outsideSurface = TRUE;
            }
            if ((s64)(x3 - x2) * (z - z2) - (s64)(z3 - z2) * (x - x2) < 0) {
                outsideSurface = TRUE;
            }
            if ((s64)(x2 - x0) * (z - z0) - (s64)(z2 - z0) * (x - x0) < 0) {
                outsideSurface = TRUE;
            }
            if ((s64)(x1 - x3) * (z - z3) - (s64)(z1 - z3) * (x - x3) < 0) {
                outsideSurface = TRUE;
            }

            if (outsideSurface == FALSE) {
                return surfaceIndex;
            }
            surfaceIndex++;
        } while (surfaceIndex <= gRaceCourseMaxSurfaceIndices[gRaceCourseIndex.signedValue].maxSurfaceIndex);
    }

    return 0;
}

void pushRaceCourseSurfaceBoundaryWithVelocity(s32 *arg0, s32 *arg1, s32 arg2) {
    s32 temp5;
    s32 temp6;
    s32 temp2;
    s32 temp3;
    s32 temp1;
    s32 temp4;

    temp1 = integerSquareRoot64((s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX + (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ);
    temp2 = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / temp1;
    temp3 = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / temp1;

    temp4 = ((s64)-temp2 * gRaceCourseCollisionDeltaX + (s64)temp3 * gRaceCourseCollisionDeltaZ) / 0x1000;
    if (-arg2 < (gRaceCourseCollisionDeltaZ = temp4)) {
        temp5 = ((s64)temp3 * *arg0 + (s64)temp2 * *arg1) / 0x1000;
        temp6 = ((s64)-temp2 * *arg0 + (s64)temp3 * *arg1) / 0x1000;
        if (temp6 > 0) {
            temp6 = -temp6;
        }

        *arg0 = ((s64)temp3 * temp5 - (s64)temp2 * temp6) / 0x1000;
        *arg1 = ((s64)temp2 * temp5 + (s64)temp3 * temp6) / 0x1000;

        gRaceCourseCollisionEdgeDeltaZ = -arg2 - gRaceCourseCollisionDeltaZ;
        gRaceCourseCollisionAdjustedX += ((s64)-temp2 * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
        gRaceCourseCollisionAdjustedZ += ((s64)temp3 * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
    }
}

#define CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(pointX, pointZ, loopCondition)                                                        \
    do {                                                                                         \
        deltaX = gRaceCourseCollisionAdjustedX - (pointX);                                       \
        side = gRaceCourseCollisionAdjustedZ - (pointZ);                                         \
        length = integerSquareRoot64((s64)deltaX * deltaX + (s64)side * side);                    \
        if (length < radius) {                                                                    \
            alongEdge = ((s64)deltaX * radius) / length;                                          \
            side = ((s64)side * radius) / length;                                                 \
            gRaceCourseCollisionAdjustedX = alongEdge + (pointX);                                \
            gRaceCourseCollisionAdjustedZ = side + (pointZ);                                     \
        }                                                                                        \
    } while (loopCondition)

#define PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE()                                                                         \
    do {                                                                                         \
        if (-radius < side) {                                                                    \
            velocityAlongEdge =                                                                 \
                ((s64)tangentX * *velocityX + (s64)normalX * *velocityZ) / 0x1000;               \
            edgeLength =                                                                        \
                ((s64)-normalX * *velocityX + (s64)tangentX * *velocityZ) / 0x1000;              \
            if (edgeLength > 0) {                                                                \
                edgeLength = -edgeLength;                                                        \
            }                                                                                    \
            *velocityX =                                                                        \
                ((s64)tangentX * velocityAlongEdge - (s64)normalX * edgeLength) / 0x1000;        \
            *velocityZ =                                                                        \
                ((s64)normalX * velocityAlongEdge + (s64)tangentX * edgeLength) / 0x1000;        \
            gRaceCourseCollisionEdgeDeltaZ = -radius - side;                                     \
            gRaceCourseCollisionAdjustedX +=                                                     \
                ((s64)-normalX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;                       \
            gRaceCourseCollisionAdjustedZ +=                                                     \
                ((s64)tangentX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;                       \
        }                                                                                        \
    } while (0)

void resolveRaceCourseSurfaceCollisionWithNormal(s32 surfaceIndex, s32 x, s32 z, s32 radius, s8 normalIndex,
                                                 s32 *pushX, s32 *pushZ, s32 *velocityX, s32 *velocityZ) {
    s32 x0;
    s32 x1;
    s32 x2;
    s32 x3;
    s32 z0;
    s32 z1;
    s32 z2;
    s32 z3;
    s32 velocityAlongEdge;
    s32 edgeLength;
    s32 normalX;
    s32 tangentX;
    s32 length;
    s32 alongEdge;
    s32 side;
    s32 deltaX;
    s32 surfaceOffset;

    normalIndex = -normalIndex;
    surfaceOffset = surfaceIndex * sizeof(RaceMotionSurface);
    x0 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[0]].x << 0x11;
    x1 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[1]].x << 0x11;
    x2 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[2]].x << 0x11;
    x3 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[3]].x << 0x11;
    z0 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[0]].z << 0x11;
    z1 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[1]].z << 0x11;
    z2 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[2]].z << 0x11;
    z3 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[3]].z << 0x11;

    gRaceCourseCollisionAdjustedX = x;
    gRaceCourseCollisionAdjustedZ = z;

    if (normalIndex == gRaceCourseSurfaces[surfaceIndex].nextFaceIndices[0]) {
        gRaceCourseCollisionEdgeDeltaX = x1 - x0;
        gRaceCourseCollisionEdgeDeltaZ = z1 - z0;
        gRaceCourseCollisionDeltaX = x - x0;
        gRaceCourseCollisionDeltaZ = z - z0;
        pushRaceCourseSurfaceBoundaryWithVelocity(velocityX, velocityZ, radius);
    }

    if (normalIndex == gRaceCourseSurfaces[surfaceIndex].nextFaceIndices[1]) {
        gRaceCourseCollisionEdgeDeltaX = x2 - x3;
        gRaceCourseCollisionEdgeDeltaZ = z2 - z3;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x3;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z3;
        pushRaceCourseSurfaceBoundaryWithVelocity(velocityX, velocityZ, radius);
    }

    if (normalIndex == gRaceCourseSurfaces[surfaceIndex].unk4[0]) {
        gRaceCourseCollisionEdgeDeltaX = x3 - x1;
        gRaceCourseCollisionEdgeDeltaZ = z3 - z1;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x1;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z1;

        length = integerSquareRoot64((s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX +
                                     (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ);
        if (surfaceOffset) {
        }
        normalX = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
        tangentX = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
        alongEdge = ((s64)tangentX * gRaceCourseCollisionDeltaX +
                     (s64)normalX * gRaceCourseCollisionDeltaZ) /
                    0x1000;
        side = ((s64)-normalX * gRaceCourseCollisionDeltaX +
                (s64)tangentX * gRaceCourseCollisionDeltaZ) /
               0x1000;
        edgeLength = ((s64)tangentX * gRaceCourseCollisionEdgeDeltaX +
                      (s64)normalX * gRaceCourseCollisionEdgeDeltaZ) /
                     0x1000;
        if ((gRaceCourseSurfaces[surfaceIndex].unk14[3] & 2) && (alongEdge < 0)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x1, z1, 0);
        } else if ((gRaceCourseSurfaces[surfaceIndex].unk14[3] & 8) && (edgeLength < alongEdge)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x3, z3, (u64)0);
        } else {
            PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE();
        }
    }

    if (normalIndex == gRaceCourseSurfaces[surfaceIndex].unk4[1]) {
        gRaceCourseCollisionEdgeDeltaX = x0 - x2;
        gRaceCourseCollisionEdgeDeltaZ = z0 - z2;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x2;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z2;

        length = integerSquareRoot64((s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX +
                                     (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ);
        normalX = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
        tangentX = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
        alongEdge = ((s64)tangentX * gRaceCourseCollisionDeltaX +
                     (s64)normalX * gRaceCourseCollisionDeltaZ) /
                    0x1000;
        side = ((s64)-normalX * gRaceCourseCollisionDeltaX +
                (s64)tangentX * gRaceCourseCollisionDeltaZ) /
               0x1000;
        edgeLength = ((s64)tangentX * gRaceCourseCollisionEdgeDeltaX +
                      (s64)normalX * gRaceCourseCollisionEdgeDeltaZ) /
                     0x1000;
        if ((gRaceCourseSurfaces[surfaceIndex].unk14[3] & 4) && (alongEdge < 0)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x2, z2, 0);
        } else if ((gRaceCourseSurfaces[surfaceIndex].unk14[3] & 1) && (edgeLength < alongEdge)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x0, z0, 0);
        } else {
            PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE();
        }
    }

    *pushX = gRaceCourseCollisionAdjustedX - x;
    *pushZ = gRaceCourseCollisionAdjustedZ - z;
}

#undef CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT
#undef PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE

#define CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(pointX, pointZ)                          \
    do {                                                                                         \
        deltaX = gRaceCourseCollisionAdjustedX - (pointX);                                       \
        side = gRaceCourseCollisionAdjustedZ - (pointZ);                                         \
        length = integerSquareRoot64((s64)deltaX * deltaX + (s64)side * side);                    \
        if (length < radius) {                                                                    \
            alongEdge = ((s64)deltaX * radius) / length;                                          \
            side = ((s64)side * radius) / length;                                                 \
            gRaceCourseCollisionAdjustedX = alongEdge + (pointX);                                \
            gRaceCourseCollisionAdjustedZ = side + (pointZ);                                     \
        }                                                                                        \
    } while (0)

#define PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE()                                           \
    do {                                                                                         \
        if (-radius < side) {                                                                    \
            velocityAlongEdge = ((s64)tangentX * *velocityX + (s64)normalX * *velocityZ) / 0x1000;       \
            edgeLength = ((s64)-normalX * *velocityX + (s64)tangentX * *velocityZ) / 0x1000;     \
            if (edgeLength > 0) {                                                                \
                edgeLength = -edgeLength;                                                        \
            }                                                                                    \
            *velocityX = ((s64)tangentX * velocityAlongEdge - (s64)normalX * edgeLength) / 0x1000;       \
            *velocityZ = ((s64)normalX * velocityAlongEdge + (s64)tangentX * edgeLength) / 0x1000;       \
            gRaceCourseCollisionEdgeDeltaZ = -radius - side;                                     \
            gRaceCourseCollisionAdjustedX += ((s64)-normalX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000; \
            gRaceCourseCollisionAdjustedZ += ((s64)tangentX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000; \
        }                                                                                        \
    } while (0)

void resolveRaceCourseSurfaceCollisionWithVelocity(s32 surfaceIndex, s32 x, s32 z, s32 radius, s32 *pushX,
                  s32 *pushZ, s32 *velocityX, s32 *velocityZ) {
    s32 x0;
    s32 x1;
    s32 x2;
    s32 x3;
    s32 z0;
    s32 z1;
    s32 z2;
    s32 z3;
    s32 velocityAlongEdge;
    s32 edgeLength;
    s32 normalX;
    s32 tangentX;
    s32 length;
    s32 alongEdge;
    s32 side;
    s32 deltaX;
    s32 surfaceOffset;

    surfaceOffset = surfaceIndex * sizeof(RaceMotionSurface);
    x0 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[0]].x << 0x11;
    x1 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[1]].x << 0x11;
    x2 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[2]].x << 0x11;
    x3 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[3]].x << 0x11;
    z0 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[0]].z << 0x11;
    z1 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[1]].z << 0x11;
    z2 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[2]].z << 0x11;
    z3 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[3]].z << 0x11;

    gRaceCourseCollisionAdjustedX = x;
    gRaceCourseCollisionAdjustedZ = z;

    if (gRaceCourseSurfaces[surfaceIndex].nextFaceIndices[0] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x1 - x0;
        gRaceCourseCollisionEdgeDeltaZ = z1 - z0;
        gRaceCourseCollisionDeltaX = x - x0;
        gRaceCourseCollisionDeltaZ = z - z0;
        pushRaceCourseSurfaceBoundaryWithVelocity(velocityX, velocityZ, radius);
    }

    if (gRaceCourseSurfaces[surfaceIndex].nextFaceIndices[1] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x2 - x3;
        gRaceCourseCollisionEdgeDeltaZ = z2 - z3;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x3;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z3;
        pushRaceCourseSurfaceBoundaryWithVelocity(velocityX, velocityZ, radius);
    }

    if (gRaceCourseSurfaces[surfaceIndex].unk4[0] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x3 - x1;
        gRaceCourseCollisionEdgeDeltaZ = z3 - z1;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x1;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z1;

        length = integerSquareRoot64((s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX +
                                     (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ);
        if (surfaceOffset) {
        }
        normalX = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
        tangentX = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
        alongEdge = ((s64)tangentX * gRaceCourseCollisionDeltaX + (s64)normalX * gRaceCourseCollisionDeltaZ) / 0x1000;
        side = ((s64)-normalX * gRaceCourseCollisionDeltaX + (s64)tangentX * gRaceCourseCollisionDeltaZ) / 0x1000;
        edgeLength = ((s64)tangentX * gRaceCourseCollisionEdgeDeltaX + (s64)normalX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
        if ((gRaceCourseSurfaces[surfaceIndex].unk14[3] & 2) && (alongEdge < 0)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x1, z1);
        } else if ((gRaceCourseSurfaces[surfaceIndex].unk14[3] & 8) && (edgeLength < alongEdge)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x3, z3);
        } else {
            PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE();
        }
    }

    if (gRaceCourseSurfaces[surfaceIndex].unk4[1] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x0 - x2;
        gRaceCourseCollisionEdgeDeltaZ = z0 - z2;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x2;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z2;

        length = integerSquareRoot64((s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX +
                                     (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ);
        normalX = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
        tangentX = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
        alongEdge = ((s64)tangentX * gRaceCourseCollisionDeltaX + (s64)normalX * gRaceCourseCollisionDeltaZ) / 0x1000;
        side = ((s64)-normalX * gRaceCourseCollisionDeltaX + (s64)tangentX * gRaceCourseCollisionDeltaZ) / 0x1000;
        edgeLength = ((s64)tangentX * gRaceCourseCollisionEdgeDeltaX + (s64)normalX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
        if ((gRaceCourseSurfaces[surfaceIndex].unk14[3] & 4) && (alongEdge < 0)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x2, z2);
        } else if ((gRaceCourseSurfaces[surfaceIndex].unk14[3] & 1) && (edgeLength < alongEdge)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x0, z0);
        } else {
            PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE();
        }
    }

    *pushX = gRaceCourseCollisionAdjustedX - x;
    *pushZ = gRaceCourseCollisionAdjustedZ - z;
}

#undef CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT
#undef PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE

void pushRaceCourseSurfaceBoundary(s32 arg0) {
    s32 temp_v1;
    s32 temp_v0;
    s32 length;
    s32 temp_a1;

    length = integerSquareRoot64((s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX + (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ);
    temp_v1 = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
    temp_v0 = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;

    temp_a1 = -temp_v1;
    gRaceCourseCollisionDeltaZ = ((s64)temp_a1 * gRaceCourseCollisionDeltaX + (s64)temp_v0 * gRaceCourseCollisionDeltaZ) / 0x1000;

    if (-arg0 < gRaceCourseCollisionDeltaZ) {
        gRaceCourseCollisionEdgeDeltaZ = -arg0 - gRaceCourseCollisionDeltaZ;
        gRaceCourseCollisionAdjustedX += ((s64)temp_a1 * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
        gRaceCourseCollisionAdjustedZ += ((s64)temp_v0 * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
    }
}

#define CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(pointX, pointZ)                          \
    do {                                                                                         \
        deltaX = gRaceCourseCollisionAdjustedX - (pointX);                                       \
        side = gRaceCourseCollisionAdjustedZ - (pointZ);                                         \
        length = integerSquareRoot64((s64)deltaX * deltaX + (s64)side * side);                    \
        if (length < radius) {                                                                    \
            alongEdge = ((s64)deltaX * radius) / length;                                          \
            side = ((s64)side * radius) / length;                                                 \
            gRaceCourseCollisionAdjustedX = alongEdge + (pointX);                                \
            gRaceCourseCollisionAdjustedZ = side + (pointZ);                                     \
        }                                                                                        \
    } while (0)

#define PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE()                                           \
    do {                                                                                         \
        if (-radius < side) {                                                                    \
            gRaceCourseCollisionEdgeDeltaZ = -radius - side;                                     \
            gRaceCourseCollisionAdjustedX +=                                                     \
                ((s64)-normalX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;                       \
            gRaceCourseCollisionAdjustedZ +=                                                     \
                ((s64)tangentX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;                       \
        }                                                                                        \
    } while (0)

void resolveRaceCourseSurfaceCollision(s32 surfaceIndex, s32 x, s32 z, s32 radius, s32 *pushX, s32 *pushZ) {
    s32 x0;
    s32 x1;
    s32 x2;
    s32 x3;
    s32 z0;
    s32 z1;
    s32 z2;
    s32 z3;
    s32 normalX;
    s32 tangentX;
    s32 length;
    s32 alongEdge;
    s32 side;
    s32 edgeLength;
    s32 deltaX;
    s32 surfaceOffset;

    surfaceOffset = surfaceIndex * sizeof(RaceMotionSurface);
    x0 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[0]].x << 0x11;
    x1 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[1]].x << 0x11;
    x2 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[2]].x << 0x11;
    x3 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[3]].x << 0x11;
    z0 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[0]].z << 0x11;
    z1 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[1]].z << 0x11;
    z2 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[2]].z << 0x11;
    z3 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].coordIndices[3]].z << 0x11;

    gRaceCourseCollisionAdjustedX = x;
    gRaceCourseCollisionAdjustedZ = z;

    if (gRaceCourseSurfaces[surfaceIndex].nextFaceIndices[0] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x1 - x0;
        gRaceCourseCollisionEdgeDeltaZ = z1 - z0;
        gRaceCourseCollisionDeltaX = x - x0;
        gRaceCourseCollisionDeltaZ = z - z0;
        pushRaceCourseSurfaceBoundary(radius);
    }

    if (gRaceCourseSurfaces[surfaceIndex].nextFaceIndices[1] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x2 - x3;
        gRaceCourseCollisionEdgeDeltaZ = z2 - z3;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x3;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z3;
        pushRaceCourseSurfaceBoundary(radius);
    }

    if (gRaceCourseSurfaces[surfaceIndex].unk4[0] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x3 - x1;
        gRaceCourseCollisionEdgeDeltaZ = z3 - z1;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x1;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z1;

        length = integerSquareRoot64((s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX +
                                     (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ);
        if (surfaceOffset) {
        }
        normalX = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
        tangentX = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
        alongEdge = ((s64)tangentX * gRaceCourseCollisionDeltaX +
                     (s64)normalX * gRaceCourseCollisionDeltaZ) /
                    0x1000;
        side = ((s64)-normalX * gRaceCourseCollisionDeltaX +
                (s64)tangentX * gRaceCourseCollisionDeltaZ) /
               0x1000;
        edgeLength = ((s64)tangentX * gRaceCourseCollisionEdgeDeltaX +
                      (s64)normalX * gRaceCourseCollisionEdgeDeltaZ) /
                     0x1000;
        if ((gRaceCourseSurfaces[surfaceIndex].unk14[3] & 2) && (alongEdge < 0)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x1, z1);
        } else if ((gRaceCourseSurfaces[surfaceIndex].unk14[3] & 8) && (edgeLength < alongEdge)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x3, z3);
        } else {
            PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE();
        }
    }

    if (gRaceCourseSurfaces[surfaceIndex].unk4[1] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x0 - x2;
        gRaceCourseCollisionEdgeDeltaZ = z0 - z2;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x2;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z2;

        length = integerSquareRoot64((s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX +
                                     (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ);
        normalX = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
        tangentX = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
        alongEdge = ((s64)tangentX * gRaceCourseCollisionDeltaX +
                     (s64)normalX * gRaceCourseCollisionDeltaZ) /
                    0x1000;
        side = ((s64)-normalX * gRaceCourseCollisionDeltaX +
                (s64)tangentX * gRaceCourseCollisionDeltaZ) /
               0x1000;
        edgeLength = ((s64)tangentX * gRaceCourseCollisionEdgeDeltaX +
                      (s64)normalX * gRaceCourseCollisionEdgeDeltaZ) /
                     0x1000;
        if ((gRaceCourseSurfaces[surfaceIndex].unk14[3] & 4) && (alongEdge < 0)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x2, z2);
        } else if ((gRaceCourseSurfaces[surfaceIndex].unk14[3] & 1) && (edgeLength < alongEdge)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x0, z0);
        } else {
            PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE();
        }
    }

    *pushX = gRaceCourseCollisionAdjustedX - x;
    *pushZ = gRaceCourseCollisionAdjustedZ - z;
}

#undef CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT
#undef PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE

// getRaceCourseSurfaceHeight best match: 99.821% (nonmatchings/getRaceCourseSurfaceHeight-8129558366194613874/base_10.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/motion/race_motion/getRaceCourseSurfaceHeight.s")

#ifdef NON_MATCHING
s32 getRaceCourseSurfaceHeight(s32 arg0, s32 arg1, s32 arg2) {
    register s32 keyframeOffset;
    register s32 faceIndex;
    register s32 faceOffset;
    register RaceMotionFace *face;
    register RaceMotionCoord *coord0;
    register RaceMotionCoord *coord1;
    register RaceMotionCoord *coords;

    keyframeOffset = arg0 * sizeof(RaceMotionSurface);
    faceIndex = ((RaceMotionSurface *)((s32)gRaceCourseSurfaces + keyframeOffset))->unk14[0];
    coord1 = gRaceCourseSurfaceCoords;
    if (faceIndex < ((RaceMotionSurface *)((s32)gRaceCourseSurfaces + keyframeOffset))->unk14[1]) {
        faceOffset = faceIndex * sizeof(RaceMotionFace);
        do {
            face = (RaceMotionFace *)((s32)gRaceCourseSurfaceFaces + faceOffset);
            if (face->unk7 == 0) {
                coords = gRaceCourseSurfaceCoords;
                coord0 = (RaceMotionCoord *)((s32)coords + face->coord0 * sizeof(RaceMotionCoord));
                if ((s64)((coords[face->coord1].x << 0x11) - (coord0->x << 0x11)) *
                    (arg2 - (coord0->z << 0x11)) -
                    (face = (RaceMotionFace *)((s32)gRaceCourseSurfaceFaces + faceOffset),
                    coords = gRaceCourseSurfaceCoords,
                    coord0 = (RaceMotionCoord *)((s32)coords + face->coord0 * sizeof(RaceMotionCoord)),
                    (s64)((coords[face->coord1].z << 0x11) - (coord0->z << 0x11)) *
                    (arg1 - (coord0->x << 0x11))) < 0) {
                    goto next;
                }

                face = (RaceMotionFace *)((s32)gRaceCourseSurfaceFaces + faceOffset);
            }

            coords = gRaceCourseSurfaceCoords;
            coord1 = (RaceMotionCoord *)((s32)coords + face->coord1 * sizeof(RaceMotionCoord));
            coord0 = coords;
            if ((s64)((coord0[face->coord2].x << 0x11) - (coord1->x << 0x11)) *
                (arg2 - (coord1->z << 0x11)) -
                (face = (RaceMotionFace *)((s32)gRaceCourseSurfaceFaces + faceOffset),
                coords = gRaceCourseSurfaceCoords,
                coord1 = (RaceMotionCoord *)((s32)coords + face->coord1 * sizeof(RaceMotionCoord)),
                (s64)((coords[face->coord2].z << 0x11) - (coord1->z << 0x11)) *
                (arg1 - (coord1->x << 0x11))) >= 0) {
                s16 x20;
                s16 y20;
                s16 z20;
                s16 x10;
                s16 y10;
                s16 z10;
                register RaceMotionCoord *coord2;
                register s32 numeratorA;
                register s32 xDelta;
                register s32 height;
                register s32 unused;
                register s32 zDelta;

                face = (RaceMotionFace *)((s32)gRaceCourseSurfaceFaces + faceOffset);
                coords = gRaceCourseSurfaceCoords;
                coord2 = (RaceMotionCoord *)((s32)coords + face->coord2 * sizeof(RaceMotionCoord));
                coord0 = (RaceMotionCoord *)((s32)coords + face->coord0 * sizeof(RaceMotionCoord));

                x20 = (s16)(coord2->x - coord0->x);
                y20 = (s16)(coord2->y - coord0->y);
                z20 = (s16)(coord2->z - coord0->z);
                coord2 = coord0;
                coord1 = (RaceMotionCoord *)((s32)coords + face->coord1 * sizeof(RaceMotionCoord));
                x10 = (s16)(coord1->x - coord2->x);
                y10 = (s16)(coord1->y - coord2->y);
                z10 = (s16)(coord1->z - coord2->z);
                xDelta = arg1 - (coord2->x << 0x11);
                zDelta = arg2 - (coord2->z << 0x11);

                numeratorA = (y20 * z10) - (z20 * y10);
                height = (-((s64)numeratorA * xDelta) -
                    ((s64)((x20 * y10) - (y20 * x10)) * zDelta)) /
                    ((z20 * x10) - (x20 * z10));
                height += gRaceCourseSurfaceCoords[((RaceMotionFace *)((s32)gRaceCourseSurfaceFaces + faceOffset))->coord0].y << 0x11;
                return height;
            }

next:
            faceIndex++;
            faceOffset += sizeof(RaceMotionFace);
        } while (faceIndex < ((RaceMotionSurface *)((s32)gRaceCourseSurfaces + (arg0 * sizeof(RaceMotionSurface))))->unk14[1]);
    }

    if (!gRaceCourseSurfaceFaces) {
    }

    return 0xC0000000;
}
#endif

s32 getRaceCourseSurfaceType(s32 arg0, s32 arg1, s32 arg2) {
    s32 keyframeOffset;
    s32 faceIndex;
    s32 faceOffset;
    RaceMotionFace *face;
    RaceMotionCoord *coord;
    RaceMotionCoord *coords;
    s64 pad;
    s64 rhs;
    s64 lhs;

    keyframeOffset = arg0 * sizeof(RaceMotionSurface);
    faceIndex = ((RaceMotionSurface *)((s32)gRaceCourseSurfaces + keyframeOffset))->unk14[0];
    if (faceIndex < ((RaceMotionSurface *)((s32)gRaceCourseSurfaces + keyframeOffset))->unk14[1]) {
        faceOffset = faceIndex * sizeof(RaceMotionFace);
        do {
            face = (RaceMotionFace *)((s32)gRaceCourseSurfaceFaces + faceOffset);
            if (face->unk7 == 0) {
                coords = gRaceCourseSurfaceCoords;
                coord = (RaceMotionCoord *)((s32)coords + face->coord0 * sizeof(RaceMotionCoord));
                lhs = (s64)((coords[face->coord1].x << 0x11) - (coord->x << 0x11)) *
                    (arg2 - (coord->z << 0x11));

                face = (RaceMotionFace *)((s32)gRaceCourseSurfaceFaces + faceOffset);
                coords = gRaceCourseSurfaceCoords;
                coord = (RaceMotionCoord *)((s32)coords + face->coord0 * sizeof(RaceMotionCoord));
                rhs = (s64)((coords[face->coord1].z << 0x11) - (coord->z << 0x11)) *
                    (arg1 - (coord->x << 0x11));

                if (lhs - rhs < 0) {
                    goto next;
                }

                face = (RaceMotionFace *)((s32)gRaceCourseSurfaceFaces + faceOffset);
            }

            coords = gRaceCourseSurfaceCoords;
            coord = (RaceMotionCoord *)((s32)coords + face->coord1 * sizeof(RaceMotionCoord));
            lhs = (s64)((coords[face->coord2].x << 0x11) - (coord->x << 0x11)) *
                (arg2 - (coord->z << 0x11));

            face = (RaceMotionFace *)((s32)gRaceCourseSurfaceFaces + faceOffset);
            coords = gRaceCourseSurfaceCoords;
            coord = (RaceMotionCoord *)((s32)coords + face->coord1 * sizeof(RaceMotionCoord));
            rhs = (s64)((coords[face->coord2].z << 0x11) - (coord->z << 0x11)) *
                (arg1 - (coord->x << 0x11));

            if (lhs - rhs >= 0) {
                if ((!coord->x) && (!coord->x)) {
                }
                return ((RaceMotionFace *)((s32)gRaceCourseSurfaceFaces + faceOffset))->faceIndex;
            }

next:
            faceIndex++;
            faceOffset += sizeof(RaceMotionFace);
        } while (faceIndex < ((RaceMotionSurface *)((s32)gRaceCourseSurfaces + (arg0 * sizeof(RaceMotionSurface))))->unk14[1]);
    }

    return 0;
}

u32 projectRaceCourseSurfaceProgress(s32 arg0, s32 arg1, s32 arg2) {
    RaceMotionSurface *keyframe = &gRaceCourseSurfaces[arg0];

    s32 deltaX = arg1 - (gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].positionIndex].x << 0x11);
    s32 deltaZ = arg2 - (gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].positionIndex].z << 0x11);

    gRaceCourseSurfaceAngleSin = fixedSine(gRaceCourseSurfaces[arg0].angle);
    gRaceCourseSurfaceAngleCos = fixedCosine(gRaceCourseSurfaces[arg0].angle);

    return ((s64)(-gRaceCourseSurfaceAngleSin) * deltaX + (s64)gRaceCourseSurfaceAngleCos * deltaZ) / 0x1000;
}

void getRaceCourseSurfaceSpawnTransform(s32 arg0, s32 *x, s32 *y, s32 *z, s16 *angle) {
    RaceMotionCoord **coordTablePtr;
    s32 keyframeOffset;

    keyframeOffset = arg0 * sizeof(RaceMotionSurface);
    coordTablePtr = &gRaceCourseSurfaceCoords;
    *x = ((RaceMotionCoord *)((s32)*coordTablePtr +
            (((RaceMotionSurface *)((s32)gRaceCourseSurfaces + keyframeOffset))->positionIndex * sizeof(RaceMotionCoord))))->x << 0x11;
    *y = ((RaceMotionCoord *)((s32)*coordTablePtr +
            (((RaceMotionSurface *)((s32)gRaceCourseSurfaces + keyframeOffset))->positionIndex * sizeof(RaceMotionCoord))))->y << 0x11;
    *z = ((RaceMotionCoord *)((s32)*coordTablePtr +
            (((RaceMotionSurface *)((s32)gRaceCourseSurfaces + keyframeOffset))->positionIndex * sizeof(RaceMotionCoord))))->z << 0x11;
    *angle = -((RaceMotionSurface *)((s32)gRaceCourseSurfaces + keyframeOffset))->angle;
}

// getRaceCourseTargetPositionAhead best match: 99.466% (nonmatchings/getRaceCourseTargetPositionAhead-8498672362023432715/base_13.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/motion/race_motion/getRaceCourseTargetPositionAhead.s")

#ifdef NON_MATCHING
void getRaceCourseTargetPositionAhead(s32 arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4, s32 arg5, s32 arg6) {
    s32 keyframeOffset;
    s32 deltaX;
    s32 deltaZ;
    s32 projected;
    s32 distance;
    s16 pathIndex;
    s16 upperSurfaceIndex;

    if (arg0 != gRaceCourseStartEntries[gRaceCourseIndex.signedValue].pathIndex) {
        keyframeOffset = arg0 * sizeof(RaceMotionSurface);
        deltaZ = arg0;
        gRaceCourseSurfaceAngleSin = fixedSine(gRaceCourseSurfaces[deltaZ].angle);
        gRaceCourseSurfaceAngleCos = fixedCosine(gRaceCourseSurfaces[arg0].angle);

        deltaX = arg1 - (gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].positionIndex].x << 0x11);
        deltaZ = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].positionIndex].z;
        deltaZ = arg2 - (deltaZ << 0x11);
        projected = ((s64)-gRaceCourseSurfaceAngleSin * deltaX + (s64)gRaceCourseSurfaceAngleCos * deltaZ) / 0x1000;

        if ((arg0 >= gRaceCourseStartEntries[gRaceCourseIndex.signedValue].unk38) && ((upperSurfaceIndex = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].unk3A) >= arg0)) {
            distance = projected + 0xC00000;
            *arg3 = ((s64)-gRaceCourseSurfaceAngleSin * distance) / 0x1000;
            deltaZ = gRaceCourseSurfaceAngleCos;
            *arg4 = ((s64)deltaZ * distance) / 0x1000;
        } else if ((gRaceCourseIndex.signedValue == 3) && ((arg0 == 0x11D) || (arg0 == 0x11E))) {
            distance = projected + 0xC00000;
            *arg3 = (distance * (s64)-gRaceCourseSurfaceAngleSin) / 0x1000;
            *arg4 = ((s64)gRaceCourseSurfaceAngleCos * distance) / 0x1000;
        } else {
            distance = projected + 0xFF400000;
            *arg3 = ((s64)-gRaceCourseSurfaceAngleSin * distance) / 0x1000;
            *arg4 = ((s64)gRaceCourseSurfaceAngleCos * distance) / 0x1000;
        }

        *arg3 += gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].positionIndex].x << 0x11;
        *arg4 += gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].positionIndex].z << 0x11;

        projected = *arg4 ^ 0;
        pathIndex = findRaceCourseSurfaceFromHint(arg0, *arg3, projected);
        gRaceCourseSurfaceAngleSin = fixedSine(gRaceCourseSurfaces[(s32)pathIndex].angle);
        keyframeOffset = pathIndex * sizeof(RaceMotionSurface);
        gRaceCourseSurfaceAngleCos = fixedCosine(gRaceCourseSurfaces[pathIndex].angle);

        deltaX = *arg3 - (gRaceCourseSurfaceCoords[gRaceCourseSurfaces[pathIndex].positionIndex].x << 0x11);
        deltaZ = *arg4 - (gRaceCourseSurfaceCoords[gRaceCourseSurfaces[pathIndex].positionIndex].z << 0x11);
        distance = updateRacePlayerSmoothedPathOffset(arg5, pathIndex, arg6);
        projected = ((s64)-gRaceCourseSurfaceAngleSin * deltaX + (s64)gRaceCourseSurfaceAngleCos * deltaZ) / 0x1000;

        *arg3 = ((s64)gRaceCourseSurfaceAngleCos * distance + (s64)-gRaceCourseSurfaceAngleSin * projected) / 0x1000;
        *arg4 = ((s64)gRaceCourseSurfaceAngleSin * distance + (s64)gRaceCourseSurfaceAngleCos * projected) / 0x1000;
        *arg3 += gRaceCourseSurfaceCoords[gRaceCourseSurfaces[pathIndex].positionIndex].x << 0x11;
        *arg4 += gRaceCourseSurfaceCoords[gRaceCourseSurfaces[pathIndex].positionIndex].z << 0x11;
    } else {
        *arg3 = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].unk18;
        *arg4 = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].unk1C;
    }
}
#endif

void getRaceCourseProgressPosition(s32 arg0, s32 *arg1, s32 *arg2, s32 arg3) {
    s32 unused[4];
    s32 temp_v0;

    if (arg0 != gRaceCourseStartEntries[gRaceCourseIndex.signedValue].pathIndex) {
        gRaceCourseSurfaceAngleSin = fixedSine(gRaceCourseSurfaces[arg0].angle);
        gRaceCourseSurfaceAngleCos = fixedCosine(gRaceCourseSurfaces[arg0].angle);
        temp_v0 = getRacePlayerPathOffset(arg3, arg0);
        *arg1 = ((s64)gRaceCourseSurfaceAngleCos * temp_v0) / 0x1000;
        *arg2 = ((s64)gRaceCourseSurfaceAngleSin * temp_v0) / 0x1000;
        *arg1 += gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].positionIndex].x << 0x11;
        *arg2 += gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].positionIndex].z << 0x11;
    } else {
        *arg1 = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].unk40;
        *arg2 = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].unk44;
    }
}

s16 getRaceCourseNextSurface(s32 arg0) {
    return gRaceCourseSurfaces[arg0].nextFaceIndices[1];
}

void setRaceMotionAnimation(RaceMotionState *state, s32 animationIndex) {
    RaceMotionAnimationHeader *animationHeader;
    s16 frameCount;
    s32 frameDataOffset;
    u8 *assetBase;

    assetBase = getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
    animationHeader = (RaceMotionAnimationHeader *)(assetBase +
        ((RaceMotionAnimationAsset *)assetBase)->animationOffsets[animationIndex] * sizeof(u16));
    frameCount = animationHeader->frameCount;
    state->framesRemaining = frameCount;
    state->framesRemaining++;
    state->frameCount = frameCount;
    animationHeader++;
    frameDataOffset = (u8 *)animationHeader -
        (u8 *)getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
    state->animationStartOffset = frameDataOffset;
    state->frameDataOffset = frameDataOffset;
    state->frameTimer = 0;
    state->animationIndex = animationIndex;
}

void loadRaceMotionAnimationFrame(RaceMotionState *state) {
    s16 *frameData;
    RaceMotionState *cursor;
    s32 one;
    s32 jointOffset;
    u8 *base;
    s16 value0;
    s16 value1;
    s16 angle;

    base = getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
    one = (s32)base;
    frameData = (s16 *)(((u32)one) + state->frameDataOffset);
    state->frameTimerReset = 1;
    state->frameTimer = state->frameTimerReset;

    cursor = state;
    for (one = 0; one < 2; one++) {
        cursor->animation.joints[0].unkC = frameData[0] << 14;
        cursor->animation.joints[0].unk10 = frameData[1] << 14;
        cursor = (RaceMotionState *)((s32)cursor + sizeof(RaceMotionStateJoint));
        cursor->animation.joints[0].unk0 = frameData[2] << 14;
        frameData += 3;
    }

    jointOffset = 0;
    cursor = state;
    one = 1;
    do {
        value0 = frameData[0];
        value1 = frameData[1];
        frameData += 2;
        cursor->animation.joints[0].rotationX = (value0 >> 4) & 0xFF0;
        cursor->animation.joints[0].rotationY = (value0 << 4) & 0xFF0;
        cursor->animation.joints[0].rotationZ = (value1 >> 4) & 0xFF0;
        if (value1 & one) {
            cursor->animation.joints[0].rotationX += 8;
        }
        if (value1 & 2) {
            cursor->animation.joints[0].rotationY += 8;
        }
        if (value1 & 4) {
            cursor->animation.joints[0].rotationZ += 8;
        }
        cursor = (RaceMotionState *)((s32)cursor + sizeof(RaceMotionStateJoint));
        jointOffset += sizeof(RaceMotionStateJoint);
    } while (jointOffset != RACE_MOTION_JOINT_COUNT * sizeof(RaceMotionStateJoint));

    state->frameDataOffset = (u8 *)frameData -
        (u8 *)getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
    angle = state->animation.joints[0].rotationX;
    state->nextModelJointX = angle;
    state->modelJointX = angle;
    angle = state->animation.joints[0].rotationY;
    state->nextModelJointY = angle;
    state->modelJointY = angle;
    angle = state->animation.joints[0].rotationZ;
    state->nextModelJointZ = angle;
    state->modelJointZ = angle;
}

void loadRaceMotionJointAnimationFrame(RaceMotionState *state) {
    RaceMotionPackedJointRotation *packedRotation;
    u8 *base;
    RaceMotionPartialJointCursor *jointCursor;
    s32 one;
    s32 jointOffset;

    base = getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
    packedRotation = (RaceMotionPackedJointRotation *)((one = (s32)base) + state->frameDataOffset +
        RACE_MOTION_PACKED_JOINT_ROTATION_SKIP_BYTES);
    state->frameTimerReset = 1;
    jointOffset = RACE_MOTION_PARTIAL_ANIMATION_START_JOINT * sizeof(RaceMotionStateJoint);
    jointCursor = (RaceMotionPartialJointCursor *)((u8 *)state + jointOffset);
    state->frameTimer = state->frameTimerReset;
    one = 1;

    do {
        s16 packedXY = packedRotation->xy;
        s16 packedZAndFlags = packedRotation->zAndFlags;

        packedRotation++;
        jointCursor->rotationX = (packedXY >> 4) & 0xFF0;
        jointCursor->rotationY = (packedXY << 4) & 0xFF0;
        jointCursor->rotationZ = (packedZAndFlags >> 4) & 0xFF0;

        if (packedZAndFlags & one) {
            jointCursor->rotationX += 8;
        }
        if (packedZAndFlags & 2) {
            jointCursor->rotationY += 8;
        }
        jointOffset += sizeof(RaceMotionStateJoint);
        if (packedZAndFlags & 4) {
            jointCursor->rotationZ += 8;
        }
        jointCursor = (RaceMotionPartialJointCursor *)((u8 *)jointCursor + sizeof(RaceMotionStateJoint));
    } while (jointOffset != RACE_MOTION_JOINT_COUNT * sizeof(RaceMotionStateJoint));

    state->frameDataOffset = (u8 *)packedRotation -
        (u8 *)getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
}

void interpolateRaceMotionAnimationFrame(RaceMotionState *state, s32 animationIndex,
                                         s32 frameTimer, s32 frameTimerReset) {
    RaceMotionAnimationAsset *animationAsset;
    s16 *packedData;
    RaceMotionDecodedRootFrame *decodedRootFrame;
    RaceMotionDecodedJointFrame *decodedJointFrame;
    RaceMotionDecodedJointFrame *decodedJointFrameEnd;
    RaceMotionRotation *decodedRoot;
    RaceMotionRotation *decodedJoint;
    RaceMotionPartialJointCursor *jointCursor;
    s32 *jointData;
    s32 *rootData;
    s32 jointIndex;
    s32 one;
    s32 startX;
    s32 startY;
    s32 startZ;
    s32 angleDelta;
    s32 blendOffset;
    s16 packedXY;
    s16 packedZAndFlags;

    animationAsset = getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
    packedData = (s16 *)animationAsset + animationAsset->animationOffsets[animationIndex] + 1;
    decodedRootFrame = (RaceMotionDecodedRootFrame *)gRaceMotionRotationDecodeFrameBuffer; decodedJointFrameEnd = (RaceMotionDecodedJointFrame *)&gRacePlayerHitCueId; decodedJointFrame = (RaceMotionDecodedJointFrame *)gRaceMotionJointDecodeFrameBuffer; one = 1; decode_frame:
    jointIndex = 0;
    decodedRoot = (RaceMotionRotation *)decodedRootFrame;

    for (; jointIndex < RACE_MOTION_FRAME_SAMPLE_COUNT; jointIndex++) {
        decodedRoot->x = packedData[0] << RACE_MOTION_MODEL_POSITION_FRAC_BITS;
        decodedRoot->y = packedData[1] << RACE_MOTION_MODEL_POSITION_FRAC_BITS;
        decodedRoot->z = packedData[2] << RACE_MOTION_MODEL_POSITION_FRAC_BITS;
        decodedRoot++;
        packedData += 3;
    }

    for (jointIndex = 0; jointIndex < RACE_MOTION_JOINT_COUNT; jointIndex++) {
        packedXY = packedData[0];
        packedZAndFlags = packedData[1];
        packedData += 2;
        decodedJoint = &decodedJointFrame->joints[jointIndex];
        decodedJoint->x = (packedXY >> 4) & 0xFF0;
        if (1) {
            decodedJoint->y = (packedXY << 4) & 0xFF0;
            decodedJoint->z = (packedZAndFlags >> 4) & 0xFF0;
        }
        if (packedZAndFlags & one) {
            decodedJoint->x += 8;
        }
        if (packedZAndFlags & 2) {
            if (1) {
            }
            if (1) {
            }
            decodedJoint->y += 8;
        }
        if (packedZAndFlags & 4) {
            decodedJoint->z += 8;
        }
    }

    decodedJointFrame++;
    decodedRootFrame++;
    if ((u32)decodedJointFrame < (u32)decodedJointFrameEnd) {
        goto decode_frame;
    }

    jointData = (s32 *)gRaceMotionJointFrameBuffer;
    jointIndex = 0;
    jointCursor = (RaceMotionPartialJointCursor *)state;

interpolate_joint:
    startX = jointData[0];
    angleDelta = (jointData[42] - startX) & 0xFFF;
    if (angleDelta >= 0x801) {
        angleDelta -= 0x1000;
    }
    blendOffset = (angleDelta * frameTimer) / frameTimerReset;
    jointCursor->rotationX = startX + blendOffset;

    angleDelta = (jointData[43] - (startY = jointData[1])) & 0xFFF;
    if (angleDelta >= 0x801) {
        angleDelta -= 0x1000;
    }
    blendOffset = (angleDelta * frameTimer) / frameTimerReset;
    jointCursor->rotationY = startY + blendOffset;

    angleDelta = (jointData[44] - (startZ = jointData[2])) & 0xFFF;
    if (angleDelta >= 0x801) {
        angleDelta -= 0x1000;
    }
    blendOffset = (angleDelta * frameTimer) / frameTimerReset;
    jointCursor->rotationZ = startZ + blendOffset;

    jointIndex++;
    jointData += 3;
    jointCursor = (RaceMotionPartialJointCursor *)((u8 *)jointCursor + sizeof(RaceMotionStateJoint));
    if (jointIndex != RACE_MOTION_JOINT_COUNT) {
        goto interpolate_joint;
    }

    rootData = (s32 *)gRaceMotionRotationFrameBuffer;
    state->animation.motion.rootMotion.position.x =
        ((rootData[6] - rootData[0]) * frameTimer) / frameTimerReset + rootData[0];
    state->animation.motion.rootMotion.position.y =
        ((rootData[7] - rootData[1]) * frameTimer) / frameTimerReset + rootData[1];
    state->animation.motion.rootMotion.position.z =
        ((rootData[8] - rootData[2]) * frameTimer) / frameTimerReset + rootData[2];
    state->animation.motion.rootMotion.rotation.x =
        ((rootData[9] - rootData[3]) * frameTimer) / frameTimerReset + rootData[3];
    state->animation.motion.rootMotion.rotation.y =
        ((rootData[10] - rootData[4]) * frameTimer) / frameTimerReset + rootData[4];
    state->animation.motion.rootMotion.rotation.z =
        ((rootData[11] - rootData[5]) * frameTimer) / frameTimerReset + rootData[5];

    state->modelJointX = state->nextModelJointX = state->animation.motion.jointX;
    state->modelJointY = state->nextModelJointY = state->animation.motion.jointY;
    state->modelJointZ = state->nextModelJointZ = state->animation.motion.jointZ;
}

// interpolateRaceMotionJointAnimationFrame best match: 98.833% (nonmatchings/interpolateRaceMotionJointAnimationFrame-5176680205357669729/independent_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/motion/race_motion/interpolateRaceMotionJointAnimationFrame.s")

#ifdef NON_MATCHING

extern RaceMotionRotation gRaceMotionJointFrameBufferReload[];
extern volatile s32 gRaceMotionRotationFrameBufferReload[];

void interpolateRaceMotionJointAnimationFrame(RaceMotionState *state, s32 animationIndex,
                                              s32 frameTimer, s32 frameTimerReset) {
    RaceMotionAnimationAsset *animationAsset;
    s16 *packedData;
    RaceMotionDecodedRootFrame *decodedRootFrame;
    RaceMotionDecodedJointFrame *decodedJointFrame;
    RaceMotionDecodedJointFrame *decodedJointFrameEnd;
    RaceMotionRotation *decodedRoot;
    RaceMotionRotation *decodedJoint;
    RaceMotionPartialAnimationState *partialState;
    RaceMotionRootMotion *rootMotion;
    s32 *jointData;
    s32 *rootData;
    s32 jointIndex;
    s32 startX;
    s32 startY;
    s32 startZ;
    s32 angleDelta;
    s32 interpolationOffset;
    s16 packedXY;
    s16 packedZAndFlags;
    s16 rootJointAngle;

    animationAsset = getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
    decodedRootFrame = (RaceMotionDecodedRootFrame *)gRaceMotionRotationFrameBuffer;
    packedData = (s16 *)((u16 *)animationAsset +
        animationAsset->animationOffsets[animationIndex] + 1);
    decodedJointFrame = (RaceMotionDecodedJointFrame *)gRaceMotionJointFrameBuffer;
    decodedJointFrameEnd = (RaceMotionDecodedJointFrame *)&gRacePlayerHitCueId;

decode_frame:
    jointIndex = 0;
    decodedRoot = (RaceMotionRotation *)decodedRootFrame;
    for (; jointIndex < RACE_MOTION_FRAME_SAMPLE_COUNT; jointIndex++) {
        decodedRoot->x = packedData[0] << RACE_MOTION_MODEL_POSITION_FRAC_BITS;
        decodedRoot->y = packedData[1] << RACE_MOTION_MODEL_POSITION_FRAC_BITS;
        decodedRoot->z = packedData[2] << RACE_MOTION_MODEL_POSITION_FRAC_BITS;
        decodedRoot++;
        packedData += 3;
    }

    for (jointIndex = 0; jointIndex < RACE_MOTION_PARTIAL_ANIMATION_JOINT_COUNT; jointIndex++) {
        decodedJoint = &decodedJointFrame->joints[jointIndex];
        packedXY = packedData[0];
        packedZAndFlags = packedData[1];
        packedData += 2;
        goto decode_packed_rotation;
decode_packed_rotation:
        decodedJoint->x = (packedXY >> 4) & 0xFF0;
        decodedJoint->y = (packedXY << 4) & 0xFF0;
        decodedJoint->z = (packedZAndFlags >> 4) & 0xFF0;
        if (packedZAndFlags & 1) {
            decodedJoint->x += 8;
        }
        if (packedZAndFlags & 2) {
            decodedJoint->y += 8;
        }
        if (packedZAndFlags & 4) {
            decodedJoint->z += 8;
        }
    }

    jointIndex = RACE_MOTION_PARTIAL_ANIMATION_JOINT_COUNT;
skip_joint:
    jointIndex++;
    packedData += 2;
    if (jointIndex < RACE_MOTION_JOINT_COUNT) {
        goto skip_joint;
    }

    decodedJointFrame++;
    decodedRootFrame++;
    if ((u32)decodedJointFrame < (u32)decodedJointFrameEnd) {
        goto decode_frame;
    }

    jointData = (s32 *)gRaceMotionJointFrameBufferReload;
    jointIndex = 0;
    partialState = (RaceMotionPartialAnimationState *)state;
interpolate_joint:
    startX = jointData[0];
    angleDelta = (jointData[42] - startX) & 0xFFF;
    if (angleDelta >= 0x801) {
        angleDelta -= 0x1000;
    }
    interpolationOffset = (angleDelta * frameTimer) / frameTimerReset;
    partialState->joints[jointIndex].rotationX = startX + interpolationOffset;

    angleDelta = (jointData[43] - (startY = jointData[1])) & 0xFFF;
    if (angleDelta >= 0x801) {
        angleDelta -= 0x1000;
    }
    interpolationOffset = (angleDelta * frameTimer) / frameTimerReset;
    partialState->joints[jointIndex].rotationY = startY + interpolationOffset;

    angleDelta = (jointData[44] - (startZ = jointData[2])) & 0xFFF;
    if (angleDelta >= 0x801) {
        angleDelta -= 0x1000;
    }
    interpolationOffset = (angleDelta * frameTimer) / frameTimerReset;
    partialState->joints[jointIndex].rotationZ = startZ + interpolationOffset;

    jointIndex++;
    jointData += 3;
    if (jointIndex != RACE_MOTION_PARTIAL_ANIMATION_JOINT_COUNT) {
        goto interpolate_joint;
    }

    rootData = (s32 *)gRaceMotionRotationFrameBufferReload;
    rootMotion = &state->animation.motion.rootMotion;
    rootMotion->position.x =
        ((rootData[6] - rootData[0]) * frameTimer) / frameTimerReset + rootData[0];
    rootMotion->position.y =
        ((rootData[7] - rootData[1]) * frameTimer) / frameTimerReset + rootData[1];
    rootMotion->position.z =
        ((rootData[8] - rootData[2]) * frameTimer) / frameTimerReset + rootData[2];
    rootMotion->rotation.x =
        ((rootData[9] - rootData[3]) * frameTimer) / frameTimerReset + rootData[3];
    rootMotion->rotation.y =
        ((rootData[10] - rootData[4]) * frameTimer) / frameTimerReset + rootData[4];
    rootMotion->rotation.z =
        ((rootData[11] - rootData[5]) * frameTimer) / frameTimerReset + rootData[5];

    rootJointAngle = state->animation.joints[0].rotationX;
    state->modelJointX = rootJointAngle;
    state->nextModelJointX = rootJointAngle;
    rootJointAngle = state->animation.joints[0].rotationY;
    state->nextModelJointY = rootJointAngle;
    state->modelJointY = rootJointAngle;
    rootJointAngle = state->animation.joints[0].rotationZ;
    state->nextModelJointZ = rootJointAngle;
    state->modelJointZ = rootJointAngle;
}
#endif

void blendRaceMotionJointAnimation(RaceMotionState *state, s32 animIndex, s32 timer, s32 duration) {
    RaceMotionAnimationAsset *animationAsset;
    RaceMotionPackedJointRotation *packedRotation;
    RaceMotionDecodedJointFrame *decodedJointFrame;
    RaceMotionDecodedJointFrame *decodedJointFrameEnd;
    RaceMotionRotation *decodedJoint;
    RaceMotionPartialJointCursor *jointCursor;
    s32 *blendData;
    s32 jointIndex;
    s32 startX;
    s32 startY;
    s32 startZ;
    s32 angleDelta;
    s32 blendOffset;
    s16 packedXY;
    s16 packedZAndFlags;

    animationAsset = getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
    packedRotation = (RaceMotionPackedJointRotation *)((u16 *)animationAsset + animationAsset->animationOffsets[animIndex] + 1); decodedJointFrameEnd = (RaceMotionDecodedJointFrame *)&gRacePlayerHitCueId; decodedJointFrame = (RaceMotionDecodedJointFrame *)gRaceMotionJointFrameBuffer; decode_frame: packedRotation += RACE_MOTION_PACKED_JOINT_ROTATION_PREFIX_COUNT; for (jointIndex = RACE_MOTION_PARTIAL_ANIMATION_START_JOINT; jointIndex < RACE_MOTION_JOINT_COUNT; jointIndex++) { decodedJoint = &decodedJointFrame->joints[jointIndex]; packedXY = packedRotation->xy;
        packedZAndFlags = packedRotation->zAndFlags;
        packedRotation++;
        decodedJoint->x = (packedXY >> 4) & 0xFF0;
        decodedJoint->y = (packedXY << 4) & 0xFF0;
        decodedJoint->z = (packedZAndFlags >> 4) & 0xFF0;
        if (packedZAndFlags & 1) {
            decodedJoint->x += 8;
        }
        if (packedZAndFlags & 2) {
            decodedJoint->y += 8;
        }
        if (packedZAndFlags & 4) {
            decodedJoint->z += 8;
        }
    }

    decodedJointFrame++;
    if ((u32)decodedJointFrame < (u32)decodedJointFrameEnd) {
        goto decode_frame;
    }

    blendData = gRaceMotionJointBlendBuffer;
    jointIndex = RACE_MOTION_PARTIAL_ANIMATION_START_JOINT;
    jointCursor = &((RaceMotionBlendAnimationState *)state)->jointCursor;
blend_joint:
    startX = blendData[0];
    angleDelta = (blendData[42] - startX) & 0xFFF;
    if (angleDelta >= 0x801) {
        angleDelta -= 0x1000;
    }
    blendOffset = (angleDelta * timer) / duration;
    jointCursor->rotationX = startX + blendOffset;

    angleDelta = (blendData[43] - (startY = blendData[1])) & 0xFFF;
    if (angleDelta >= 0x801) {
        angleDelta -= 0x1000;
    }
    blendOffset = (angleDelta * timer) / duration;
    jointCursor->rotationY = startY + blendOffset;

    angleDelta = (blendData[44] - (startZ = blendData[2])) & 0xFFF;
    if (angleDelta >= 0x801) {
        angleDelta -= 0x1000;
    }
    blendOffset = (angleDelta * timer) / duration;
    jointCursor->rotationZ = startZ + blendOffset;

    jointIndex++;
    blendData += 3;
    jointCursor = (RaceMotionPartialJointCursor *)((u8 *)jointCursor + sizeof(RaceMotionStateJoint));
    if (jointIndex != RACE_MOTION_JOINT_COUNT) {
        goto blend_joint;
    }
}

void stepRaceMotionLoopingAnimation(RaceMotionState *state) {
    s32 frameTimer;
    s32 nextFrameTimer;

    frameTimer = state->frameTimer;
    nextFrameTimer = frameTimer - 1;
    if (frameTimer == 0) {
        state->framesRemaining--;
        if (state->framesRemaining <= 0) {
            state->framesRemaining = state->frameCount;
            state->frameDataOffset = state->animationStartOffset;
        }
        loadRaceMotionAnimationFrame(state);
        frameTimer = state->frameTimer;
        nextFrameTimer = frameTimer - 1;
    }
    state->frameTimer = nextFrameTimer;
}

void stepRaceMotionLoopingJointAnimation(RaceMotionState *state) {
    s32 frameTimer;
    s32 nextFrameTimer;

    frameTimer = state->frameTimer;
    nextFrameTimer = frameTimer - 1;
    if (frameTimer == 0) {
        state->framesRemaining--;
        if (state->framesRemaining <= 0) {
            state->framesRemaining = state->frameCount;
            state->frameDataOffset = state->animationStartOffset;
        }
        loadRaceMotionJointAnimationFrame(state);
        frameTimer = state->frameTimer;
        nextFrameTimer = frameTimer - 1;
    }
    state->frameTimer = nextFrameTimer;
}

s32 stepRaceMotionAnimationUntilEnd(RaceMotionState *state) {
    s32 frameTimer;
    s32 nextFrameTimer;
    s16 framesRemaining;

    frameTimer = state->frameTimer;
    nextFrameTimer = frameTimer - 1;
    if (frameTimer == 0) {
        framesRemaining = state->framesRemaining;
        if (framesRemaining != 1) {
            state->framesRemaining = framesRemaining - 1;
            loadRaceMotionAnimationFrame(state);
            if (state->framesRemaining == 1) {
                return 1;
            }
            frameTimer = state->frameTimer;
            nextFrameTimer = frameTimer - 1;
        } else {
            return 1;
        }
    }
    state->frameTimer = nextFrameTimer;
    return 0;
}

s32 stepRaceMotionJointAnimationUntilEnd(RaceMotionState *state) {
    s32 frameTimer;
    s32 nextFrameTimer;
    s16 framesRemaining;

    frameTimer = state->frameTimer;
    nextFrameTimer = frameTimer - 1;
    if (frameTimer == 0) {
        framesRemaining = state->framesRemaining;
        if (framesRemaining != 1) {
            state->framesRemaining = framesRemaining - 1;
            loadRaceMotionJointAnimationFrame(state);
            if (state->framesRemaining == 1) {
                return 1;
            }
            frameTimer = state->frameTimer;
            nextFrameTimer = frameTimer - 1;
        } else {
            return 1;
        }
    }
    state->frameTimer = nextFrameTimer;
    return 0;
}

void initRaceMotionModelParts(RaceMotionInitState *state) {
    s32 i;
    u8 *partIds;
    u8 *parentPartIds;
    RaceMotionCoord *positions;
    s16 zero;

    state->partCount = gRaceMotionModelPartCounts[state->modelId];
    partIds = gRaceMotionModelPartIds[state->modelId];
    parentPartIds = gRaceMotionModelParentPartIds[state->modelId];

    for (i = 0; i < state->partCount; i++) {
        state->parts[i].partId = *partIds;
        state->parts[i].parentPartId = *parentPartIds;
        partIds++;
        parentPartIds++;
    }

    positions = gRaceMotionModelPartPositions[state->modelId];

    for (i = 0; i < state->partCount; i++, positions++) {
        state->parts[i].rotationZ = 0;
        zero = state->parts[i].rotationZ;
        state->parts[i].rotationY = zero;
        state->parts[i].rotationX = zero;
        state->parts[i].x = positions->x << RACE_MOTION_MODEL_POSITION_FRAC_BITS;
        state->parts[i].y = positions->y << RACE_MOTION_MODEL_POSITION_FRAC_BITS;
        state->parts[i].z = positions->z << RACE_MOTION_MODEL_POSITION_FRAC_BITS;
    }
}
