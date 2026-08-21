#include "game/race/race_state.h"
#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/engine/relocatable_heap.h"
#include "game/math/fixed_point_math.h"
#include "game/race/motion/race_motion.h"
#include "game/race/player/race_player_progress.h"

typedef struct RaceCourseSurfaceFace {
    u16 coord0;
    u16 coord1;
    u16 coord2;
    s8 surfaceType;
    u8 skipFirstEdgeCheck;
} RaceCourseSurfaceFace;

typedef struct RaceMotionCountedTable {
    u16 count;
    u8 data[1];
} RaceMotionCountedTable;

typedef struct RaceCourseSurface {
    s16 neighborIndices[4];
    s16 boundaryCoordIndices[4];
    s16 referenceCoordIndex;
    s16 pathAngle;
    u16 faceStartIndex;
    u16 faceEndIndex;
    u16 unk18;
    u16 edgeClampFlags;
} RaceCourseSurface;

typedef char RaceCourseSurfaceFaceSizeCheck[(sizeof(RaceCourseSurfaceFace) == 8) ? 1 : -1];
typedef char RaceCourseSurfaceSizeCheck[(sizeof(RaceCourseSurface) == 0x1C) ? 1 : -1];

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
    Vec3i position;
    char padC[8];
    Vec3i rotation;
} RaceMotionRootMotion;

typedef struct RaceMotionPackedRootMotion {
    Vec3s position;
    Vec3s rotation;
} RaceMotionPackedRootMotion;

typedef struct RaceMotionPackedAnimationFrame {
    RaceMotionPackedRootMotion rootMotion;
    RaceMotionPackedJointRotation joints[RACE_MOTION_JOINT_COUNT];
} RaceMotionPackedAnimationFrame;

typedef struct RaceMotionDecodedRootFrame {
    Vec3i position;
    Vec3i rotation;
} RaceMotionDecodedRootFrame;

typedef struct RaceMotionDecodedJointFrame {
    Vec3i joints[RACE_MOTION_JOINT_COUNT];
    Vec3i stridePadding[2];
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

u8 gRaceMotionModelPartCounts[] = {
    14, 14, 14, 14, 14, 14, 14, 0, 0xFF, 0, 1, 2, 1, 4, 1, 6, 6, 8, 6, 10, 0, 0, 0, 0,
};
u8 gRaceMotionModelParentPartIdData[] = { 0, 1, 4, 5, 2, 3, 6, 7, 10, 11, 8, 9, 0, 0, 0, 0 };
u8 *gRaceMotionModelPartIds[] = {
    &gRaceMotionModelPartCounts[8], &gRaceMotionModelPartCounts[8], &gRaceMotionModelPartCounts[8],
    &gRaceMotionModelPartCounts[8], &gRaceMotionModelPartCounts[8], &gRaceMotionModelPartCounts[8],
    &gRaceMotionModelPartCounts[8],
};
u8 *gRaceMotionModelParentPartIds[] = {
    gRaceMotionModelParentPartIdData, gRaceMotionModelParentPartIdData, gRaceMotionModelParentPartIdData,
    gRaceMotionModelParentPartIdData, gRaceMotionModelParentPartIdData, gRaceMotionModelParentPartIdData,
    gRaceMotionModelParentPartIdData,
};
Vec3s gRaceMotionModelPartPositionsShort[] = {
    { 0,   0,   0 },
    { 0,   37,  0 },
    { -6,  -2,  0 },
    { 0,   -12, 0 },
    { 6,   -2,  0 },
    { 0,   -12, 0 },
    { 0,   0,   0 },
    { 0,   16,  0 },
    { -10, 14,  0 },
    { -8,  -15, 0 },
    { 10,  14,  0 },
    { 8,   -15, 0 },
    { -14, 10,  0 },
    { 14,  10,  0 },
};
Vec3s gRaceMotionModelPartPositionsTall[] = {
    { 0,   0,   0  },
    { 0,   44,  0  },
    { -6,  -5,  0  },
    { 0,   -16, 0  },
    { 6,   -5,  0  },
    { 0,   -16, 0  },
    { 0,   0,   0  },
    { 0,   9,   0  },
    { -10, 8,   0  },
    { -10, -16, 0  },
    { 10,  8,   0  },
    { 10,  -16, 0  },
    { -12, 12,  -3 },
    { 12,  12,  -3 },
};
Vec3s gRaceMotionModelPartPositionsWide[] = {
    { 0,   0,   0  },
    { 0,   51,  0  },
    { -8,  -3,  0  },
    { 0,   -18, -1 },
    { 8,   -3,  0  },
    { 1,   -18, -1 },
    { 0,   5,   -5 },
    { 0,   32,  2  },
    { -23, 23,  5  },
    { -11, -21, 0  },
    { 23,  23,  5  },
    { 11,  -21, 0  },
    { -17, 12,  -8 },
    { 17,  12,  -8 },
};
Vec3s gRaceMotionModelPartPositionsCompact[] = {
    { 0,   0,  0  },
    { 0,   40, 0  },
    { -5,  -7, 0  },
    { -1,  -8, 0  },
    { 5,   -7, 0  },
    { 1,   -8, 0  },
    { 0,   0,  0  },
    { 0,   14, 0  },
    { -13, 8,  -2 },
    { -7,  -9, -3 },
    { 13,  8,  -2 },
    { 7,   -9, -3 },
    { -6,  10, 0  },
    { 6,   10, 0  },
};
Vec3s *gRaceMotionModelPartPositions[] = {
    gRaceMotionModelPartPositionsShort,
    gRaceMotionModelPartPositionsTall,
    gRaceMotionModelPartPositionsShort,
    gRaceMotionModelPartPositionsTall,
    gRaceMotionModelPartPositionsWide,
    gRaceMotionModelPartPositionsCompact,
    NULL,
    NULL,
};

extern Vec3s *gRaceCourseSurfaceCoords;
extern RaceCourseSurfaceFace *gRaceCourseSurfaceFaces;
extern RaceCourseSurface *gRaceCourseSurfaces;
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
extern Vec3i gRaceMotionJointFrameBuffer[];
extern Vec3i gRaceMotionJointDecodeFrameBuffer[];
extern s32 gRaceMotionJointBlendBuffer[];
extern s32 gRacePlayerHitCueId;

void initRaceCourseSurfaceData(void) {
    RaceMotionCountedTable *coordTable;
    RaceMotionCountedTable *faceTable;

    coordTable = (RaceMotionCountedTable *)getRelocatableHeapBlockBase(gAssetHandles[0x1B]);
    gRaceCourseSurfaceCoords = (Vec3s *)coordTable->data;

    faceTable = (RaceMotionCountedTable *)&gRaceCourseSurfaceCoords[coordTable->count];
    gRaceCourseSurfaceFaces = (RaceCourseSurfaceFace *)faceTable->data;

    faceTable = (RaceMotionCountedTable *)&gRaceCourseSurfaceFaces[faceTable->count];
    gRaceCourseSurfaces = (RaceCourseSurface *)faceTable->data;
}

s32 findRaceCourseSurfaceFromHint(s32 surfaceIndex, s32 x, s32 z) {
    s32 z2;
    s32 index;

    index = surfaceIndex;

loop: {
    Vec3s *coords;
    s32 x0;
    s32 x1;
    s32 z3;
    s32 z0;
    s32 x2;
    s32 x3;
    s32 z1;

    coords = gRaceCourseSurfaceCoords;
    x0 = coords[gRaceCourseSurfaces[index].boundaryCoordIndices[0]].x << 0x11;
    x1 = coords[gRaceCourseSurfaces[index].boundaryCoordIndices[1]].x << 0x11;
    x2 = coords[gRaceCourseSurfaces[index].boundaryCoordIndices[2]].x << 0x11;
    x3 = coords[(&gRaceCourseSurfaces[index])->boundaryCoordIndices[3]].x << 0x11;
    z0 = coords[gRaceCourseSurfaces[index].boundaryCoordIndices[0]].z << 0x11;
    z1 = coords[gRaceCourseSurfaces[index].boundaryCoordIndices[1]].z << 0x11;
    z2 = coords[gRaceCourseSurfaces[index].boundaryCoordIndices[2]].z << 0x11;
    z3 = coords[gRaceCourseSurfaces[index].boundaryCoordIndices[3]].z << 0x11;

    if ((s64)(x0 - x1) * (z - z1) - (s64)(z0 - z1) * (x - x1) < 0) {
        if (gRaceCourseSurfaces[index].neighborIndices[0] >= 0) {
            index = gRaceCourseSurfaces[index].neighborIndices[0];
            goto loop;
        }
    }
    if ((s64)(x3 - x2) * (z - z2) - (s64)(z3 - z2) * (x - x2) < 0) {
        if (gRaceCourseSurfaces[index].neighborIndices[1] >= 0) {
            index = gRaceCourseSurfaces[index].neighborIndices[1];
            goto loop;
        }
    }
    if ((s64)(x2 - x0) * (z - z0) - (s64)(z2 - z0) * (x - x0) < 0) {
        if (gRaceCourseSurfaces[index].neighborIndices[3] >= 0) {
            index = gRaceCourseSurfaces[index].neighborIndices[3];
            goto loop;
        }
    }
    if ((s64)(x1 - x3) * (z - z3) - (s64)(z1 - z3) * (x - x3) < 0) {
        if (gRaceCourseSurfaces[index].neighborIndices[2] >= 0) {
            index = gRaceCourseSurfaces[index].neighborIndices[2];
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
    if (gRaceCourseStartEntries[gRaceCourseIndex.signedValue].maxSurfaceIndex >= 0) {
        do {
            Vec3s *coords;
            s32 outsideSurface;
            s32 x0;
            s32 x1;
            s32 z0;
            s32 x2;
            s32 x3;
            s32 z1;

            coords = gRaceCourseSurfaceCoords;
            outsideSurface = FALSE;
            x0 = coords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[0]].x << 0x11;
            x1 = coords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[1]].x << 0x11;
            x2 = coords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[2]].x << 0x11;
            x3 = coords[(&gRaceCourseSurfaces[surfaceIndex])->boundaryCoordIndices[3]].x << 0x11;
            z0 = coords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[0]].z << 0x11;
            z1 = coords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[1]].z << 0x11;
            z2 = coords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[2]].z << 0x11;
            z3 = coords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[3]].z << 0x11;

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
        } while (surfaceIndex <= gRaceCourseStartEntries[gRaceCourseIndex.signedValue].maxSurfaceIndex);
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

    temp1 = integerSquareRoot64(
        (s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX +
        (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ
    );
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

#define CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(pointX, pointZ, loopCondition) \
    do {                                                                               \
        deltaX = gRaceCourseCollisionAdjustedX - (pointX);                             \
        side = gRaceCourseCollisionAdjustedZ - (pointZ);                               \
        length = integerSquareRoot64((s64)deltaX * deltaX + (s64)side * side);         \
        if (length < radius) {                                                         \
            alongEdge = ((s64)deltaX * radius) / length;                               \
            side = ((s64)side * radius) / length;                                      \
            gRaceCourseCollisionAdjustedX = alongEdge + (pointX);                      \
            gRaceCourseCollisionAdjustedZ = side + (pointZ);                           \
        }                                                                              \
    } while (loopCondition)

#define PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE()                                                    \
    do {                                                                                                  \
        if (-radius < side) {                                                                             \
            velocityAlongEdge = ((s64)tangentX * *velocityX + (s64)normalX * *velocityZ) / 0x1000;        \
            edgeLength = ((s64) - normalX * *velocityX + (s64)tangentX * *velocityZ) / 0x1000;            \
            if (edgeLength > 0) {                                                                         \
                edgeLength = -edgeLength;                                                                 \
            }                                                                                             \
            *velocityX = ((s64)tangentX * velocityAlongEdge - (s64)normalX * edgeLength) / 0x1000;        \
            *velocityZ = ((s64)normalX * velocityAlongEdge + (s64)tangentX * edgeLength) / 0x1000;        \
            gRaceCourseCollisionEdgeDeltaZ = -radius - side;                                              \
            gRaceCourseCollisionAdjustedX += ((s64) - normalX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000; \
            gRaceCourseCollisionAdjustedZ += ((s64)tangentX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;   \
        }                                                                                                 \
    } while (0)

void resolveRaceCourseSurfaceCollisionWithNormal(
    s32 surfaceIndex,
    s32 x,
    s32 z,
    s32 radius,
    s8 normalIndex,
    s32 *pushX,
    s32 *pushZ,
    s32 *velocityX,
    s32 *velocityZ
) {
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
    surfaceOffset = surfaceIndex * sizeof(RaceCourseSurface);
    x0 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[0]].x << 0x11;
    x1 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[1]].x << 0x11;
    x2 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[2]].x << 0x11;
    x3 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[3]].x << 0x11;
    z0 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[0]].z << 0x11;
    z1 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[1]].z << 0x11;
    z2 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[2]].z << 0x11;
    z3 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[3]].z << 0x11;

    gRaceCourseCollisionAdjustedX = x;
    gRaceCourseCollisionAdjustedZ = z;

    if (normalIndex == gRaceCourseSurfaces[surfaceIndex].neighborIndices[0]) {
        gRaceCourseCollisionEdgeDeltaX = x1 - x0;
        gRaceCourseCollisionEdgeDeltaZ = z1 - z0;
        gRaceCourseCollisionDeltaX = x - x0;
        gRaceCourseCollisionDeltaZ = z - z0;
        pushRaceCourseSurfaceBoundaryWithVelocity(velocityX, velocityZ, radius);
    }

    if (normalIndex == gRaceCourseSurfaces[surfaceIndex].neighborIndices[1]) {
        gRaceCourseCollisionEdgeDeltaX = x2 - x3;
        gRaceCourseCollisionEdgeDeltaZ = z2 - z3;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x3;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z3;
        pushRaceCourseSurfaceBoundaryWithVelocity(velocityX, velocityZ, radius);
    }

    if (normalIndex == gRaceCourseSurfaces[surfaceIndex].neighborIndices[2]) {
        gRaceCourseCollisionEdgeDeltaX = x3 - x1;
        gRaceCourseCollisionEdgeDeltaZ = z3 - z1;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x1;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z1;

        length = integerSquareRoot64(
            (s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX +
            (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ
        );
        if (surfaceOffset) {}
        normalX = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
        tangentX = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
        alongEdge = ((s64)tangentX * gRaceCourseCollisionDeltaX + (s64)normalX * gRaceCourseCollisionDeltaZ) / 0x1000;
        side = ((s64)-normalX * gRaceCourseCollisionDeltaX + (s64)tangentX * gRaceCourseCollisionDeltaZ) / 0x1000;
        edgeLength =
            ((s64)tangentX * gRaceCourseCollisionEdgeDeltaX + (s64)normalX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
        if ((gRaceCourseSurfaces[surfaceIndex].edgeClampFlags & 2) && (alongEdge < 0)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x1, z1, 0);
        } else if ((gRaceCourseSurfaces[surfaceIndex].edgeClampFlags & 8) && (edgeLength < alongEdge)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x3, z3, (u64)0);
        } else {
            PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE();
        }
    }

    if (normalIndex == gRaceCourseSurfaces[surfaceIndex].neighborIndices[3]) {
        gRaceCourseCollisionEdgeDeltaX = x0 - x2;
        gRaceCourseCollisionEdgeDeltaZ = z0 - z2;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x2;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z2;

        length = integerSquareRoot64(
            (s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX +
            (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ
        );
        normalX = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
        tangentX = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
        alongEdge = ((s64)tangentX * gRaceCourseCollisionDeltaX + (s64)normalX * gRaceCourseCollisionDeltaZ) / 0x1000;
        side = ((s64)-normalX * gRaceCourseCollisionDeltaX + (s64)tangentX * gRaceCourseCollisionDeltaZ) / 0x1000;
        edgeLength =
            ((s64)tangentX * gRaceCourseCollisionEdgeDeltaX + (s64)normalX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
        if ((gRaceCourseSurfaces[surfaceIndex].edgeClampFlags & 4) && (alongEdge < 0)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x2, z2, 0);
        } else if ((gRaceCourseSurfaces[surfaceIndex].edgeClampFlags & 1) && (edgeLength < alongEdge)) {
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

#define CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(pointX, pointZ)        \
    do {                                                                       \
        deltaX = gRaceCourseCollisionAdjustedX - (pointX);                     \
        side = gRaceCourseCollisionAdjustedZ - (pointZ);                       \
        length = integerSquareRoot64((s64)deltaX * deltaX + (s64)side * side); \
        if (length < radius) {                                                 \
            alongEdge = ((s64)deltaX * radius) / length;                       \
            side = ((s64)side * radius) / length;                              \
            gRaceCourseCollisionAdjustedX = alongEdge + (pointX);              \
            gRaceCourseCollisionAdjustedZ = side + (pointZ);                   \
        }                                                                      \
    } while (0)

#define PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE()                                                    \
    do {                                                                                                  \
        if (-radius < side) {                                                                             \
            velocityAlongEdge = ((s64)tangentX * *velocityX + (s64)normalX * *velocityZ) / 0x1000;        \
            edgeLength = ((s64) - normalX * *velocityX + (s64)tangentX * *velocityZ) / 0x1000;            \
            if (edgeLength > 0) {                                                                         \
                edgeLength = -edgeLength;                                                                 \
            }                                                                                             \
            *velocityX = ((s64)tangentX * velocityAlongEdge - (s64)normalX * edgeLength) / 0x1000;        \
            *velocityZ = ((s64)normalX * velocityAlongEdge + (s64)tangentX * edgeLength) / 0x1000;        \
            gRaceCourseCollisionEdgeDeltaZ = -radius - side;                                              \
            gRaceCourseCollisionAdjustedX += ((s64) - normalX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000; \
            gRaceCourseCollisionAdjustedZ += ((s64)tangentX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;   \
        }                                                                                                 \
    } while (0)

void resolveRaceCourseSurfaceCollisionWithVelocity(
    s32 surfaceIndex,
    s32 x,
    s32 z,
    s32 radius,
    s32 *pushX,
    s32 *pushZ,
    s32 *velocityX,
    s32 *velocityZ
) {
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

    surfaceOffset = surfaceIndex * sizeof(RaceCourseSurface);
    x0 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[0]].x << 0x11;
    x1 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[1]].x << 0x11;
    x2 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[2]].x << 0x11;
    x3 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[3]].x << 0x11;
    z0 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[0]].z << 0x11;
    z1 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[1]].z << 0x11;
    z2 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[2]].z << 0x11;
    z3 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[3]].z << 0x11;

    gRaceCourseCollisionAdjustedX = x;
    gRaceCourseCollisionAdjustedZ = z;

    if (gRaceCourseSurfaces[surfaceIndex].neighborIndices[0] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x1 - x0;
        gRaceCourseCollisionEdgeDeltaZ = z1 - z0;
        gRaceCourseCollisionDeltaX = x - x0;
        gRaceCourseCollisionDeltaZ = z - z0;
        pushRaceCourseSurfaceBoundaryWithVelocity(velocityX, velocityZ, radius);
    }

    if (gRaceCourseSurfaces[surfaceIndex].neighborIndices[1] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x2 - x3;
        gRaceCourseCollisionEdgeDeltaZ = z2 - z3;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x3;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z3;
        pushRaceCourseSurfaceBoundaryWithVelocity(velocityX, velocityZ, radius);
    }

    if (gRaceCourseSurfaces[surfaceIndex].neighborIndices[2] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x3 - x1;
        gRaceCourseCollisionEdgeDeltaZ = z3 - z1;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x1;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z1;

        length = integerSquareRoot64(
            (s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX +
            (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ
        );
        if (surfaceOffset) {}
        normalX = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
        tangentX = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
        alongEdge = ((s64)tangentX * gRaceCourseCollisionDeltaX + (s64)normalX * gRaceCourseCollisionDeltaZ) / 0x1000;
        side = ((s64)-normalX * gRaceCourseCollisionDeltaX + (s64)tangentX * gRaceCourseCollisionDeltaZ) / 0x1000;
        edgeLength =
            ((s64)tangentX * gRaceCourseCollisionEdgeDeltaX + (s64)normalX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
        if ((gRaceCourseSurfaces[surfaceIndex].edgeClampFlags & 2) && (alongEdge < 0)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x1, z1);
        } else if ((gRaceCourseSurfaces[surfaceIndex].edgeClampFlags & 8) && (edgeLength < alongEdge)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x3, z3);
        } else {
            PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE();
        }
    }

    if (gRaceCourseSurfaces[surfaceIndex].neighborIndices[3] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x0 - x2;
        gRaceCourseCollisionEdgeDeltaZ = z0 - z2;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x2;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z2;

        length = integerSquareRoot64(
            (s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX +
            (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ
        );
        normalX = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
        tangentX = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
        alongEdge = ((s64)tangentX * gRaceCourseCollisionDeltaX + (s64)normalX * gRaceCourseCollisionDeltaZ) / 0x1000;
        side = ((s64)-normalX * gRaceCourseCollisionDeltaX + (s64)tangentX * gRaceCourseCollisionDeltaZ) / 0x1000;
        edgeLength =
            ((s64)tangentX * gRaceCourseCollisionEdgeDeltaX + (s64)normalX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
        if ((gRaceCourseSurfaces[surfaceIndex].edgeClampFlags & 4) && (alongEdge < 0)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x2, z2);
        } else if ((gRaceCourseSurfaces[surfaceIndex].edgeClampFlags & 1) && (edgeLength < alongEdge)) {
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

    length = integerSquareRoot64(
        (s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX +
        (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ
    );
    temp_v1 = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
    temp_v0 = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;

    temp_a1 = -temp_v1;
    gRaceCourseCollisionDeltaZ =
        ((s64)temp_a1 * gRaceCourseCollisionDeltaX + (s64)temp_v0 * gRaceCourseCollisionDeltaZ) / 0x1000;

    if (-arg0 < gRaceCourseCollisionDeltaZ) {
        gRaceCourseCollisionEdgeDeltaZ = -arg0 - gRaceCourseCollisionDeltaZ;
        gRaceCourseCollisionAdjustedX += ((s64)temp_a1 * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
        gRaceCourseCollisionAdjustedZ += ((s64)temp_v0 * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
    }
}

#define CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(pointX, pointZ)        \
    do {                                                                       \
        deltaX = gRaceCourseCollisionAdjustedX - (pointX);                     \
        side = gRaceCourseCollisionAdjustedZ - (pointZ);                       \
        length = integerSquareRoot64((s64)deltaX * deltaX + (s64)side * side); \
        if (length < radius) {                                                 \
            alongEdge = ((s64)deltaX * radius) / length;                       \
            side = ((s64)side * radius) / length;                              \
            gRaceCourseCollisionAdjustedX = alongEdge + (pointX);              \
            gRaceCourseCollisionAdjustedZ = side + (pointZ);                   \
        }                                                                      \
    } while (0)

#define PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE()                                                    \
    do {                                                                                                  \
        if (-radius < side) {                                                                             \
            gRaceCourseCollisionEdgeDeltaZ = -radius - side;                                              \
            gRaceCourseCollisionAdjustedX += ((s64) - normalX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000; \
            gRaceCourseCollisionAdjustedZ += ((s64)tangentX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;   \
        }                                                                                                 \
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

    surfaceOffset = surfaceIndex * sizeof(RaceCourseSurface);
    x0 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[0]].x << 0x11;
    x1 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[1]].x << 0x11;
    x2 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[2]].x << 0x11;
    x3 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[3]].x << 0x11;
    z0 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[0]].z << 0x11;
    z1 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[1]].z << 0x11;
    z2 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[2]].z << 0x11;
    z3 = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[surfaceIndex].boundaryCoordIndices[3]].z << 0x11;

    gRaceCourseCollisionAdjustedX = x;
    gRaceCourseCollisionAdjustedZ = z;

    if (gRaceCourseSurfaces[surfaceIndex].neighborIndices[0] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x1 - x0;
        gRaceCourseCollisionEdgeDeltaZ = z1 - z0;
        gRaceCourseCollisionDeltaX = x - x0;
        gRaceCourseCollisionDeltaZ = z - z0;
        pushRaceCourseSurfaceBoundary(radius);
    }

    if (gRaceCourseSurfaces[surfaceIndex].neighborIndices[1] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x2 - x3;
        gRaceCourseCollisionEdgeDeltaZ = z2 - z3;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x3;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z3;
        pushRaceCourseSurfaceBoundary(radius);
    }

    if (gRaceCourseSurfaces[surfaceIndex].neighborIndices[2] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x3 - x1;
        gRaceCourseCollisionEdgeDeltaZ = z3 - z1;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x1;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z1;

        length = integerSquareRoot64(
            (s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX +
            (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ
        );
        if (surfaceOffset) {}
        normalX = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
        tangentX = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
        alongEdge = ((s64)tangentX * gRaceCourseCollisionDeltaX + (s64)normalX * gRaceCourseCollisionDeltaZ) / 0x1000;
        side = ((s64)-normalX * gRaceCourseCollisionDeltaX + (s64)tangentX * gRaceCourseCollisionDeltaZ) / 0x1000;
        edgeLength =
            ((s64)tangentX * gRaceCourseCollisionEdgeDeltaX + (s64)normalX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
        if ((gRaceCourseSurfaces[surfaceIndex].edgeClampFlags & 2) && (alongEdge < 0)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x1, z1);
        } else if ((gRaceCourseSurfaces[surfaceIndex].edgeClampFlags & 8) && (edgeLength < alongEdge)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x3, z3);
        } else {
            PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE();
        }
    }

    if (gRaceCourseSurfaces[surfaceIndex].neighborIndices[3] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x0 - x2;
        gRaceCourseCollisionEdgeDeltaZ = z0 - z2;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x2;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z2;

        length = integerSquareRoot64(
            (s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX +
            (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ
        );
        normalX = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
        tangentX = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
        alongEdge = ((s64)tangentX * gRaceCourseCollisionDeltaX + (s64)normalX * gRaceCourseCollisionDeltaZ) / 0x1000;
        side = ((s64)-normalX * gRaceCourseCollisionDeltaX + (s64)tangentX * gRaceCourseCollisionDeltaZ) / 0x1000;
        edgeLength =
            ((s64)tangentX * gRaceCourseCollisionEdgeDeltaX + (s64)normalX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
        if ((gRaceCourseSurfaces[surfaceIndex].edgeClampFlags & 4) && (alongEdge < 0)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x2, z2);
        } else if ((gRaceCourseSurfaces[surfaceIndex].edgeClampFlags & 1) && (edgeLength < alongEdge)) {
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

s32 getRaceCourseSurfaceHeight(s32 surfaceIndex, s32 x, s32 z) {
    s32 faceIndex;
    s16 x20;
    s16 y20;
    s16 z20;
    s16 x10;
    s16 y10;
    s16 z10;
    s32 numeratorA;
    s32 xDelta;
    s32 height;
    s32 pad[2];
    s32 zDelta;

    for (faceIndex = gRaceCourseSurfaces[surfaceIndex].faceStartIndex; faceIndex < gRaceCourseSurfaces[surfaceIndex].faceEndIndex;
         faceIndex++) {
        if ((gRaceCourseSurfaceFaces[faceIndex].skipFirstEdgeCheck != 0) ||
            ((s64)((gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord1].x << 0x11) -
                   (gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord0].x << 0x11)) *
                     (z - (gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord0].z << 0x11)) -
                 (s64)((gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord1].z << 0x11) -
                       (gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord0].z << 0x11)) *
                     (x - (gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord0].x << 0x11)) >=
             0)) {
            if ((s64)((gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord2].x << 0x11) -
                      (gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord1].x << 0x11)) *
                        (z - (gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord1].z << 0x11)) -
                    (s64)((gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord2].z << 0x11) -
                          (gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord1].z << 0x11)) *
                        (x - (gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord1].x << 0x11)) >=
                0) {
                x20 = (s16)(gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord2].x -
                            gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord0].x);
                y20 = (s16)(gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord2].y -
                            gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord0].y);
                z20 = (s16)(gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord2].z -
                            gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord0].z);
                x10 = (s16)(gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord1].x -
                            gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord0].x);
                y10 = (s16)(gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord1].y -
                            gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord0].y);
                z10 = (s16)(gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord1].z -
                            gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord0].z);
                xDelta = x - (gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord0].x << 0x11);
                zDelta = z - (gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord0].z << 0x11);

                numeratorA = (y20 * z10) - (z20 * y10);
                height = (-((s64)numeratorA * xDelta) - ((s64)((x20 * y10) - (y20 * x10)) * zDelta)) /
                         ((z20 * x10) - (x20 * z10));
                height += gRaceCourseSurfaceCoords[gRaceCourseSurfaceFaces[faceIndex].coord0].y << 0x11;
                return height;
            }
        }
    }

    return 0xC0000000;
}

s32 getRaceCourseSurfaceType(s32 arg0, s32 arg1, s32 arg2) {
    s32 surfaceOffset;
    s32 faceIndex;
    s32 faceOffset;
    RaceCourseSurfaceFace *face;
    Vec3s *coord;
    Vec3s *coords;
    s64 pad;
    s64 rhs;
    s64 lhs;

    surfaceOffset = arg0 * sizeof(RaceCourseSurface);
    faceIndex = ((RaceCourseSurface *)((s32)gRaceCourseSurfaces + surfaceOffset))->faceStartIndex;
    if (faceIndex < ((RaceCourseSurface *)((s32)gRaceCourseSurfaces + surfaceOffset))->faceEndIndex) {
        faceOffset = faceIndex * sizeof(RaceCourseSurfaceFace);
        do {
            face = (RaceCourseSurfaceFace *)((s32)gRaceCourseSurfaceFaces + faceOffset);
            if (face->skipFirstEdgeCheck == 0) {
                coords = gRaceCourseSurfaceCoords;
                coord = (Vec3s *)((s32)coords + face->coord0 * sizeof(Vec3s));
                lhs = (s64)((coords[face->coord1].x << 0x11) - (coord->x << 0x11)) * (arg2 - (coord->z << 0x11));

                face = (RaceCourseSurfaceFace *)((s32)gRaceCourseSurfaceFaces + faceOffset);
                coords = gRaceCourseSurfaceCoords;
                coord = (Vec3s *)((s32)coords + face->coord0 * sizeof(Vec3s));
                rhs = (s64)((coords[face->coord1].z << 0x11) - (coord->z << 0x11)) * (arg1 - (coord->x << 0x11));

                if (lhs - rhs < 0) {
                    goto next;
                }

                face = (RaceCourseSurfaceFace *)((s32)gRaceCourseSurfaceFaces + faceOffset);
            }

            coords = gRaceCourseSurfaceCoords;
            coord = (Vec3s *)((s32)coords + face->coord1 * sizeof(Vec3s));
            lhs = (s64)((coords[face->coord2].x << 0x11) - (coord->x << 0x11)) * (arg2 - (coord->z << 0x11));

            face = (RaceCourseSurfaceFace *)((s32)gRaceCourseSurfaceFaces + faceOffset);
            coords = gRaceCourseSurfaceCoords;
            coord = (Vec3s *)((s32)coords + face->coord1 * sizeof(Vec3s));
            rhs = (s64)((coords[face->coord2].z << 0x11) - (coord->z << 0x11)) * (arg1 - (coord->x << 0x11));

            if (lhs - rhs >= 0) {
                if ((!coord->x) && (!coord->x)) {}
                return ((RaceCourseSurfaceFace *)((s32)gRaceCourseSurfaceFaces + faceOffset))->surfaceType;
            }

        next:
            faceIndex++;
            faceOffset += sizeof(RaceCourseSurfaceFace);
        } while (faceIndex <
                 ((RaceCourseSurface *)((s32)gRaceCourseSurfaces + (arg0 * sizeof(RaceCourseSurface))))->faceEndIndex);
    }

    return 0;
}

u32 projectRaceCourseSurfaceProgress(s32 arg0, s32 arg1, s32 arg2) {
    RaceCourseSurface *surface = &gRaceCourseSurfaces[arg0];

    s32 deltaX = arg1 - (gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].referenceCoordIndex].x << 0x11);
    s32 deltaZ = arg2 - (gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].referenceCoordIndex].z << 0x11);

    gRaceCourseSurfaceAngleSin = fixedSine(gRaceCourseSurfaces[arg0].pathAngle);
    gRaceCourseSurfaceAngleCos = fixedCosine(gRaceCourseSurfaces[arg0].pathAngle);

    return ((s64)(-gRaceCourseSurfaceAngleSin) * deltaX + (s64)gRaceCourseSurfaceAngleCos * deltaZ) / 0x1000;
}

void getRaceCourseSurfaceSpawnTransform(s32 arg0, s32 *x, s32 *y, s32 *z, s16 *angle) {
    Vec3s **coordTablePtr;
    s32 surfaceOffset;

    surfaceOffset = arg0 * sizeof(RaceCourseSurface);
    coordTablePtr = &gRaceCourseSurfaceCoords;
    *x = ((Vec3s *)((s32)*coordTablePtr +
                              (((RaceCourseSurface *)((s32)gRaceCourseSurfaces + surfaceOffset))->referenceCoordIndex *
                               sizeof(Vec3s))))
             ->x
         << 0x11;
    *y = ((Vec3s *)((s32)*coordTablePtr +
                              (((RaceCourseSurface *)((s32)gRaceCourseSurfaces + surfaceOffset))->referenceCoordIndex *
                               sizeof(Vec3s))))
             ->y
         << 0x11;
    *z = ((Vec3s *)((s32)*coordTablePtr +
                              (((RaceCourseSurface *)((s32)gRaceCourseSurfaces + surfaceOffset))->referenceCoordIndex *
                               sizeof(Vec3s))))
             ->z
         << 0x11;
    *angle = -((RaceCourseSurface *)((s32)gRaceCourseSurfaces + surfaceOffset))->pathAngle;
}

void getRaceCourseTargetPositionAhead(s32 arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4, s32 arg5, s32 arg6) {
    s32 projected;
    s32 deltaX;
    s32 deltaZ;
    s32 temp;
    s32 distance;
    s16 pathIndex;

    if (arg0 != gRaceCourseStartEntries[gRaceCourseIndex.signedValue].pathIndex) {
        gRaceCourseSurfaceAngleSin = fixedSine(gRaceCourseSurfaces[arg0].pathAngle);
        gRaceCourseSurfaceAngleCos = fixedCosine(gRaceCourseSurfaces[arg0].pathAngle);
        projected = arg1;
        deltaX = projected - (gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].referenceCoordIndex].x << 0x11);
        deltaZ = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].referenceCoordIndex].z;
        deltaZ = arg2 - ((deltaZ << 2) << 15);
        projected =
            ((((s64)(-gRaceCourseSurfaceAngleSin)) * deltaX) + (((s64)gRaceCourseSurfaceAngleCos) * deltaZ)) / 0x1000;
        if ((arg0 >= gRaceCourseStartEntries[gRaceCourseIndex.signedValue].unk38) &&
            (gRaceCourseStartEntries[gRaceCourseIndex.signedValue].unk3A >= arg0)) {
            projected += 0xC00000;
            *arg3 = (((s64)(-gRaceCourseSurfaceAngleSin)) * projected) / 0x1000;
            *arg4 = (((s64)gRaceCourseSurfaceAngleCos) * projected) / 0x1000;
        } else if ((gRaceCourseIndex.signedValue == 3) && ((arg0 == 0x11D) || (arg0 == 0x11E))) {
            projected += 0xC00000;
            *arg3 = (((s64)(-gRaceCourseSurfaceAngleSin)) * projected) / 0x1000;
            *arg4 = (((s64)gRaceCourseSurfaceAngleCos) * projected) / 0x1000;
        } else {
            if (!arg3) {
            }
            projected += 0xFF400000;
            *arg3 = (((s64)(-gRaceCourseSurfaceAngleSin)) * projected) / 0x1000;
            *arg4 = (((s64)gRaceCourseSurfaceAngleCos) * projected) / 0x1000;
        }
        *arg3 += gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].referenceCoordIndex].x
                 << ((((0x11 & 0xFFFFu) & 0xFFFFu) & 0xFFFFu) & 0xFFFFu);
        *arg4 += gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].referenceCoordIndex].z << 0x11;
        pathIndex = findRaceCourseSurfaceFromHint(arg0, *arg3, *arg4);
        gRaceCourseSurfaceAngleSin = fixedSine(gRaceCourseSurfaces[(s32)pathIndex].pathAngle);
        gRaceCourseSurfaceAngleCos = fixedCosine(gRaceCourseSurfaces[pathIndex].pathAngle);
        deltaX = (*arg3) - (gRaceCourseSurfaceCoords[gRaceCourseSurfaces[pathIndex].referenceCoordIndex].x << 0x11);
        deltaZ = (*arg4) - (gRaceCourseSurfaceCoords[gRaceCourseSurfaces[pathIndex].referenceCoordIndex].z << 0x11);
        distance = updateRacePlayerSmoothedPathOffset(arg5, pathIndex, arg6);
        temp =
            ((((s64)(-gRaceCourseSurfaceAngleSin)) * deltaX) + (((s64)gRaceCourseSurfaceAngleCos) * deltaZ)) / 0x1000;
        *arg3 = ((((s64)gRaceCourseSurfaceAngleCos) * distance) + (((s64)(-gRaceCourseSurfaceAngleSin)) * temp)) /
                0x1000;
        *arg4 = ((((s64)gRaceCourseSurfaceAngleSin) * distance) + (((s64)gRaceCourseSurfaceAngleCos) * temp)) /
                0x1000;
        *arg3 += gRaceCourseSurfaceCoords[gRaceCourseSurfaces[pathIndex].referenceCoordIndex].x << 0x11;
        *arg4 += gRaceCourseSurfaceCoords[gRaceCourseSurfaces[pathIndex].referenceCoordIndex].z << 0x11;
    } else {
        *arg3 = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].unk18;
        *arg4 = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].unk1C;
    }
}

void getRaceCourseProgressPosition(s32 arg0, s32 *arg1, s32 *arg2, s32 arg3) {
    s32 unused[4];
    s32 temp_v0;

    if (arg0 != gRaceCourseStartEntries[gRaceCourseIndex.signedValue].pathIndex) {
        gRaceCourseSurfaceAngleSin = fixedSine(gRaceCourseSurfaces[arg0].pathAngle);
        gRaceCourseSurfaceAngleCos = fixedCosine(gRaceCourseSurfaces[arg0].pathAngle);
        temp_v0 = getRacePlayerPathOffset(arg3, arg0);
        *arg1 = ((s64)gRaceCourseSurfaceAngleCos * temp_v0) / 0x1000;
        *arg2 = ((s64)gRaceCourseSurfaceAngleSin * temp_v0) / 0x1000;
        *arg1 += gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].referenceCoordIndex].x << 0x11;
        *arg2 += gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].referenceCoordIndex].z << 0x11;
    } else {
        *arg1 = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].unk40;
        *arg2 = gRaceCourseStartEntries[gRaceCourseIndex.signedValue].unk44;
    }
}

s16 getRaceCourseNextSurface(s32 arg0) {
    return gRaceCourseSurfaces[arg0].neighborIndices[1];
}

void setRaceMotionAnimation(RaceMotionState *state, s32 animationIndex) {
    RaceMotionAnimationHeader *animationHeader;
    s16 frameCount;
    s32 frameDataOffset;
    u8 *assetBase;

    assetBase = getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
    animationHeader =
        (RaceMotionAnimationHeader *)(assetBase +
                                      ((RaceMotionAnimationAsset *)assetBase)->animationOffsets[animationIndex] *
                                          sizeof(u16));
    frameCount = animationHeader->frameCount;
    state->framesRemaining = frameCount;
    state->framesRemaining++;
    state->frameCount = frameCount;
    animationHeader++;
    frameDataOffset = (u8 *)animationHeader - (u8 *)getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
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

    state->frameDataOffset = (u8 *)frameData - (u8 *)getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
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

    state->frameDataOffset =
        (u8 *)packedRotation - (u8 *)getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
}

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void interpolateRaceMotionAnimationFrame(RaceMotionState *state, s32 animationIndex,
                                         s32 frameTimer, s32 frameTimerReset) {
    RaceMotionAnimationAsset *animationAsset;
    s16 *packedData;
    RaceMotionDecodedRootFrame *decodedRootFrame;
    RaceMotionDecodedJointFrame *decodedJointFrame;
    RaceMotionDecodedJointFrame *decodedJointFrameEnd;
    Vec3i *decodedRoot;
    Vec3i *decodedJoint;
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
    decodedRoot = (Vec3i *)decodedRootFrame;

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
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void interpolateRaceMotionJointAnimationFrame(RaceMotionState *state, s32 animationIndex,
                                              s32 frameTimer, s32 frameTimerReset) {
    RaceMotionAnimationAsset *animationAsset;
    s16 *packedData;
    RaceMotionDecodedRootFrame *decodedRootFrame;
    RaceMotionDecodedJointFrame *decodedJointFrame;
    RaceMotionDecodedJointFrame *decodedJointFrameEnd;
    Vec3i *decodedRoot;
    Vec3i *decodedJoint;
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
    decodedRoot = (Vec3i *)decodedRootFrame;

    for (; jointIndex < RACE_MOTION_FRAME_SAMPLE_COUNT; jointIndex++) {
        decodedRoot->x = packedData[0] << RACE_MOTION_MODEL_POSITION_FRAC_BITS;
        decodedRoot->y = packedData[1] << RACE_MOTION_MODEL_POSITION_FRAC_BITS;
        decodedRoot->z = packedData[2] << RACE_MOTION_MODEL_POSITION_FRAC_BITS;
        decodedRoot++;
        packedData += 3;
    }

    for (jointIndex = 0; jointIndex < RACE_MOTION_PARTIAL_ANIMATION_JOINT_COUNT; jointIndex++) {
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

    jointData = (s32 *)gRaceMotionJointFrameBuffer;
    jointIndex = 0;

interpolate_joint:
    startX = jointData[0];
    angleDelta = (jointData[42] - startX) & 0xFFF;
    if (angleDelta >= 0x801) {
        angleDelta -= 0x1000;
    }
    blendOffset = (angleDelta * frameTimer) / frameTimerReset;
    ((RaceMotionPartialAnimationState *)state)->joints[jointIndex].rotationX =
        startX + blendOffset;

    angleDelta = (jointData[43] - (startY = jointData[1])) & 0xFFF;
    if (angleDelta >= 0x801) {
        angleDelta -= 0x1000;
    }
    blendOffset = (angleDelta * frameTimer) / frameTimerReset;
    ((RaceMotionPartialAnimationState *)state)->joints[jointIndex].rotationY =
        startY + blendOffset;

    angleDelta = (jointData[44] - (startZ = jointData[2])) & 0xFFF;
    if (angleDelta >= 0x801) {
        angleDelta -= 0x1000;
    }
    blendOffset = (angleDelta * frameTimer) / frameTimerReset;
    ((RaceMotionPartialAnimationState *)state)->joints[jointIndex].rotationZ =
        startZ + blendOffset;

    jointData += 3;
    jointIndex++;
    if (jointIndex != RACE_MOTION_PARTIAL_ANIMATION_JOINT_COUNT) {
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
// clang-format on

// IDO code generation for this function is sensitive to source line layout.
// clang-format off
void blendRaceMotionJointAnimation(RaceMotionState *state, s32 animIndex, s32 timer, s32 duration) {
    RaceMotionAnimationAsset *animationAsset;
    RaceMotionPackedJointRotation *packedRotation;
    RaceMotionDecodedJointFrame *decodedJointFrame;
    RaceMotionDecodedJointFrame *decodedJointFrameEnd;
    Vec3i *decodedJoint;
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
// clang-format on

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
    Vec3s *positions;
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
