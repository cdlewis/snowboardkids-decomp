#include "common.h"
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

#define RACE_MOTION_JOINT_COUNT 12
#define RACE_MOTION_PARTIAL_ANIMATION_START_JOINT 6
#define RACE_MOTION_PACKED_JOINT_ROTATION_SKIP_BYTES 0x24
#define RACE_MOTION_MODEL_PART_CAPACITY 14
#define RACE_MOTION_MODEL_POSITION_FRAC_BITS 14

typedef struct RaceMotionRootMotion {
    RaceMotionRotation position;
    char padC[8];
    RaceMotionRotation rotation;
} RaceMotionRootMotion;

typedef struct RaceMotionPartialJointCursor {
    char pad0[0x33A];
    s16 rotationX;
    s16 rotationY;
    s16 rotationZ;
} RaceMotionPartialJointCursor;

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
    s16 animIndex;
    s32 animStartOffset;
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

typedef struct CourseSpawnEntry {
    s16 pathIndex;
    char pad2[0x16];
    s32 unk18;
    s32 unk1C;
    char pad20[0x18];
    s16 unk38;
    s16 unk3A;
    char pad3C[2];
    s16 maxSurfaceIndex;
    s32 unk40;
    s32 unk44;
} CourseSpawnEntry;

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
extern RaceMotionRotation gRaceMotionJointFrameBuffer[];
extern s32 gRaceMotionJointBlendBuffer[];
extern s16 gAssetHandles[];
extern s32 gRacePlayerHitCueId;
extern s16 gRaceCourseIndex;
extern s16 gRaceCourseMaxSurfaceIndices;
extern CourseSpawnEntry gRaceCourseStartEntries[];
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

// findRaceCourseSurfaceFromHint best match: 94.490% (nonmatchings/findRaceCourseSurfaceFromHint-8075865578671233833/base_12.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/motion/race_motion/findRaceCourseSurfaceFromHint.s")

#ifdef NON_MATCHING
s32 findRaceCourseSurfaceFromHint(s32 surfaceIndex, s32 x, s32 z) {
    RaceMotionSurface *keyframes;
    RaceMotionSurface *keyframe;
    RaceMotionCoord *coords;
    RaceMotionCoord *coord0;
    RaceMotionCoord *coord1;
    RaceMotionCoord *coord2;
    RaceMotionCoord *coord3;
    s32 offset;
    s32 x0;
    s32 x1;
    s32 index;
    s32 x2;
    s32 x3;
    s32 z0;
    s32 z1;
    s32 z2;
    s32 z3;
    s16 next;

    keyframes = gRaceCourseSurfaces;
    index = surfaceIndex;

loop:
    offset = index * sizeof(RaceMotionSurface);
    coords = gRaceCourseSurfaceCoords;
    keyframe = (RaceMotionSurface *)((s32)keyframes + offset);
    coord0 = (RaceMotionCoord *)((s32)coords + keyframe->coordIndices[0] * sizeof(RaceMotionCoord));
    coord1 = (RaceMotionCoord *)((s32)coords + keyframe->coordIndices[1] * sizeof(RaceMotionCoord));
    coord2 = (RaceMotionCoord *)((s32)coords + keyframe->coordIndices[2] * sizeof(RaceMotionCoord));
    coord3 = (RaceMotionCoord *)((s32)coords + keyframe->coordIndices[3] * sizeof(RaceMotionCoord));

    x0 = coord0->x << 0x11;
    z1 = coord1->x << 0x11;
    x1 = z1;
    z0 = coord0->z << 0x11;
    x2 = coord2->x << 0x11;
    x3 = coord3->x << 0x11;
    z1 = coord1->z << 0x11;
    z2 = coord2->z << 0x11;
    z3 = coord3->z << 0x11;

    if ((s64)(x0 - x1) * (z - z1) - (s64)(z0 - z1) * (x - x1) < 0) {
        keyframes = gRaceCourseSurfaces;
        next = ((RaceMotionSurface *)((s32)keyframes + offset))->nextFaceIndices[0];
        if (next >= 0) {
            index = next;
            goto loop;
        }
    }
    if ((s64)(x3 - x2) * (z - z2) - (s64)(z3 - z2) * (x - x2) < 0) {
        keyframes = gRaceCourseSurfaces;
        next = ((RaceMotionSurface *)((s32)keyframes + offset))->nextFaceIndices[1];
        if (next >= 0) {
            index = next;
            goto loop;
        }
    }
    if ((s64)(x2 - x0) * (z - z0) - (s64)(z2 - z0) * (x - x0) < 0) {
        keyframes = gRaceCourseSurfaces;
        next = ((RaceMotionSurface *)((s32)keyframes + offset))->unk4[1];
        if (next >= 0) {
            index = next;
            goto loop;
        }
    }
    if ((s64)(x1 - x3) * (z - z3) - (s64)(z1 - z3) * (x - x3) < 0) {
        keyframes = gRaceCourseSurfaces;
        next = ((RaceMotionSurface *)((s32)keyframes + offset))->unk4[0];
        if (next >= 0) {
            index = next;
            goto loop;
        }
    }

    return index;
}
#endif

// findRaceCourseSurfaceAtPoint best match: 96.293% (base_9.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/motion/race_motion/findRaceCourseSurfaceAtPoint.s")

#ifdef NON_MATCHING
s32 findRaceCourseSurfaceAtPoint(s32 x, s32 z) {
    s32 surfaceIndex;

    surfaceIndex = 0;
    if (*(s16 *)((u8 *)&gRaceCourseMaxSurfaceIndices + gRaceCourseIndex * sizeof(CourseSpawnEntry)) >= 0) {
        do {
            RaceMotionSurface *surface;
            RaceMotionCoord *coord0;
            RaceMotionCoord *coord1;
            RaceMotionCoord *coord2;
            RaceMotionCoord *coord3;
            s32 outsideSurface;
            s32 x0;
            s32 x1;
            s32 z0;
            s32 x2;
            s32 x3;
            s32 z1;
            s32 z2;
            s32 z3;

            surface = &gRaceCourseSurfaces[surfaceIndex];
            outsideSurface = FALSE;
            coord0 = &gRaceCourseSurfaceCoords[surface->coordIndices[0]];
            coord1 = &gRaceCourseSurfaceCoords[surface->coordIndices[1]];
            coord2 = &gRaceCourseSurfaceCoords[surface->coordIndices[2]];
            coord3 = &gRaceCourseSurfaceCoords[surface->coordIndices[3]];

            x0 = coord0->x << 0x11;
            x1 = coord1->x << 0x11;
            coord2 = &gRaceCourseSurfaceCoords[surface->coordIndices[2]];
            z0 = coord0->z << 0x11;
            x2 = coord2->x << 0x11;
            x3 = coord3->x << 0x11;
            z1 = coord1->z << 0x11;
            z2 = coord2->z << 0x11;
            z3 = coord3->z << 0x11;

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
        } while (surfaceIndex <= *(s16 *)((u8 *)&gRaceCourseMaxSurfaceIndices + gRaceCourseIndex * sizeof(CourseSpawnEntry)));
    }

    return 0;
}
#endif

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

// resolveRaceCourseSurfaceCollisionWithNormal best match: 91.140% (base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/motion/race_motion/resolveRaceCourseSurfaceCollisionWithNormal.s")

#ifdef NON_MATCHING
#define CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(pointX, pointZ)                          \
    do {                                                                                         \
        deltaX = gRaceCourseCollisionAdjustedX - (pointX);                                       \
        deltaZ = gRaceCourseCollisionAdjustedZ - (pointZ);                                       \
        dist = integerSquareRoot64((s64)deltaX * deltaX + (s64)deltaZ * deltaZ);                 \
        if (dist < radius) {                                                                     \
            gRaceCourseCollisionAdjustedX = ((s64)deltaX * radius) / dist + (pointX);            \
            gRaceCourseCollisionAdjustedZ = ((s64)deltaZ * radius) / dist + (pointZ);            \
        }                                                                                        \
    } while (0)

#define PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE()                                           \
    do {                                                                                         \
        if (-radius < side) {                                                                    \
            alongEdge = ((s64)tangentX * *velocityX + (s64)normalX * *velocityZ) / 0x1000;       \
            edgeLength = ((s64)tangentZ * *velocityX + (s64)tangentX * *velocityZ) / 0x1000;     \
            if (edgeLength > 0) {                                                                \
                edgeLength = -edgeLength;                                                        \
            }                                                                                    \
            *velocityX = ((s64)tangentX * alongEdge - (s64)normalX * edgeLength) / 0x1000;       \
            *velocityZ = ((s64)normalX * alongEdge + (s64)tangentX * edgeLength) / 0x1000;       \
            gRaceCourseCollisionEdgeDeltaZ = -radius - side;                                     \
            gRaceCourseCollisionAdjustedX += ((s64)tangentZ * gRaceCourseCollisionEdgeDeltaZ) / 0x1000; \
            gRaceCourseCollisionAdjustedZ += ((s64)tangentX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000; \
        }                                                                                        \
    } while (0)

void resolveRaceCourseSurfaceCollisionWithNormal(s16 surfaceIndex, s32 x, s32 z, s32 radius, s8 normalIndex, s32 *pushX,
                  s32 *pushZ, s32 *velocityX, s32 *velocityZ) {
    RaceMotionSurface *surface;
    RaceMotionCoord *coord0;
    RaceMotionCoord *coord1;
    RaceMotionCoord *coord2;
    RaceMotionCoord *coord3;
    s32 surfaceOffset;
    s32 x0;
    s32 x1;
    s32 x2;
    s32 x3;
    s32 z0;
    s32 z1;
    s32 z2;
    s32 z3;
    s32 length;
    s32 normalX;
    s32 tangentX;
    s32 tangentZ;
    s32 alongEdge;
    s32 side;
    s32 edgeLength;
    s32 deltaX;
    s32 deltaZ;
    s32 dist;

    normalIndex = -normalIndex;
    surfaceOffset = surfaceIndex * sizeof(RaceMotionSurface);
    surface = (RaceMotionSurface *)((s32)gRaceCourseSurfaces + surfaceOffset);
    coord0 = &gRaceCourseSurfaceCoords[surface->coordIndices[0]];
    coord1 = &gRaceCourseSurfaceCoords[surface->coordIndices[1]];
    coord2 = &gRaceCourseSurfaceCoords[surface->coordIndices[2]];
    coord3 = &gRaceCourseSurfaceCoords[surface->coordIndices[3]];

    x0 = coord0->x << 0x11;
    x1 = coord1->x << 0x11;
    x2 = coord2->x << 0x11;
    x3 = coord3->x << 0x11;
    z0 = coord0->z << 0x11;
    z1 = coord1->z << 0x11;
    z2 = coord2->z << 0x11;
    z3 = coord3->z << 0x11;

    gRaceCourseCollisionAdjustedX = x;
    gRaceCourseCollisionAdjustedZ = z;

    if (normalIndex == surface->nextFaceIndices[0]) {
        gRaceCourseCollisionEdgeDeltaX = x1 - x0;
        gRaceCourseCollisionEdgeDeltaZ = z1 - z0;
        gRaceCourseCollisionDeltaX = x - x0;
        gRaceCourseCollisionDeltaZ = z - z0;
        pushRaceCourseSurfaceBoundaryWithVelocity(velocityX, velocityZ, radius);
        surface = (RaceMotionSurface *)((s32)gRaceCourseSurfaces + surfaceOffset);
    }

    if (normalIndex == surface->nextFaceIndices[1]) {
        gRaceCourseCollisionEdgeDeltaX = x2 - x3;
        gRaceCourseCollisionEdgeDeltaZ = z2 - z3;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x3;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z3;
        pushRaceCourseSurfaceBoundaryWithVelocity(velocityX, velocityZ, radius);
        surface = (RaceMotionSurface *)((s32)gRaceCourseSurfaces + surfaceOffset);
    }

    if (normalIndex == surface->unk4[0]) {
        gRaceCourseCollisionEdgeDeltaX = x3 - x1;
        gRaceCourseCollisionEdgeDeltaZ = z3 - z1;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x1;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z1;

        length = integerSquareRoot64((s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX +
                                     (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ);
        normalX = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
        tangentX = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
        tangentZ = -normalX;
        alongEdge = ((s64)tangentX * gRaceCourseCollisionDeltaX + (s64)normalX * gRaceCourseCollisionDeltaZ) / 0x1000;
        side = ((s64)tangentZ * gRaceCourseCollisionDeltaX + (s64)tangentX * gRaceCourseCollisionDeltaZ) / 0x1000;
        edgeLength = ((s64)tangentX * gRaceCourseCollisionEdgeDeltaX + (s64)normalX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
        if ((((RaceMotionSurface *)((s32)gRaceCourseSurfaces + surfaceOffset))->unk14[3] & 2) && (alongEdge < 0)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x1, z1);
            surface = (RaceMotionSurface *)((s32)gRaceCourseSurfaces + surfaceOffset);
        } else if ((((RaceMotionSurface *)((s32)gRaceCourseSurfaces + surfaceOffset))->unk14[3] & 8) && (edgeLength < alongEdge)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x3, z3);
            surface = (RaceMotionSurface *)((s32)gRaceCourseSurfaces + surfaceOffset);
        } else {
            PUSH_RACE_COURSE_SURFACE_COLLISION_FROM_EDGE();
            surface = (RaceMotionSurface *)((s32)gRaceCourseSurfaces + surfaceOffset);
        }
    }

    if (normalIndex == surface->unk4[1]) {
        gRaceCourseCollisionEdgeDeltaX = x0 - x2;
        gRaceCourseCollisionEdgeDeltaZ = z0 - z2;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x2;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z2;

        length = integerSquareRoot64((s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX +
                                     (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ);
        normalX = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
        tangentX = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
        tangentZ = -normalX;
        alongEdge = ((s64)tangentX * gRaceCourseCollisionDeltaX + (s64)normalX * gRaceCourseCollisionDeltaZ) / 0x1000;
        side = ((s64)tangentZ * gRaceCourseCollisionDeltaX + (s64)tangentX * gRaceCourseCollisionDeltaZ) / 0x1000;
        edgeLength = ((s64)tangentX * gRaceCourseCollisionEdgeDeltaX + (s64)normalX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
        if ((((RaceMotionSurface *)((s32)gRaceCourseSurfaces + surfaceOffset))->unk14[3] & 4) && (alongEdge < 0)) {
            CLAMP_RACE_COURSE_SURFACE_COLLISION_TO_ENDPOINT(x2, z2);
        } else if ((((RaceMotionSurface *)((s32)gRaceCourseSurfaces + surfaceOffset))->unk14[3] & 1) && (edgeLength < alongEdge)) {
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
#endif

// resolveRaceCourseSurfaceCollisionWithVelocity best match: 86.996% (base_3.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/motion/race_motion/resolveRaceCourseSurfaceCollisionWithVelocity.s")

#ifdef NON_MATCHING
static void clampRaceCourseSurfaceCollisionToEndpoint(s32 pointX, s32 pointZ, s32 radius) {
    s32 dx;
    s32 dz;
    s32 distance;

    dx = gRaceCourseCollisionAdjustedX - pointX;
    dz = gRaceCourseCollisionAdjustedZ - pointZ;
    distance = integerSquareRoot64((s64)dx * dx + (s64)dz * dz);

    if (distance < radius) {
        gRaceCourseCollisionAdjustedX = ((s64)dx * radius) / distance + pointX;
        gRaceCourseCollisionAdjustedZ = ((s64)dz * radius) / distance + pointZ;
    }
}

static void pushRaceCourseSurfaceCollisionFromEdge(s32 radius, s32 *velocityX, s32 *velocityZ) {
    s32 length;
    s32 normalX;
    s32 tangentX;
    s32 tangentZ;
    s32 alongEdge;
    s32 distanceFromEdge;
    s32 projectedVelocityX;
    s32 projectedVelocityZ;

    length = integerSquareRoot64((s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX + (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ);
    normalX = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
    tangentX = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
    tangentZ = -normalX;

    alongEdge = ((s64)tangentX * gRaceCourseCollisionDeltaX + (s64)normalX * gRaceCourseCollisionDeltaZ) / 0x1000;
    distanceFromEdge = ((s64)tangentZ * gRaceCourseCollisionDeltaX + (s64)tangentX * gRaceCourseCollisionDeltaZ) / 0x1000;

    if (-radius < distanceFromEdge) {
        projectedVelocityX = ((s64)tangentX * *velocityX + (s64)normalX * *velocityZ) / 0x1000;
        projectedVelocityZ = ((s64)tangentZ * *velocityX + (s64)tangentX * *velocityZ) / 0x1000;
        if (projectedVelocityZ > 0) {
            projectedVelocityZ = -projectedVelocityZ;
        }

        *velocityX = ((s64)tangentX * projectedVelocityX - (s64)normalX * projectedVelocityZ) / 0x1000;
        *velocityZ = ((s64)normalX * projectedVelocityX + (s64)tangentX * projectedVelocityZ) / 0x1000;

        gRaceCourseCollisionEdgeDeltaZ = -radius - distanceFromEdge;
        gRaceCourseCollisionAdjustedX += ((s64)tangentZ * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
        gRaceCourseCollisionAdjustedZ += ((s64)tangentX * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
    }
}

void resolveRaceCourseSurfaceCollisionWithVelocity(s16 arg0, s32 x, s32 z, s32 radius, s32 *pushX, s32 *pushZ, s32 *velocityX,
                  s32 *velocityZ) {
    RaceMotionSurface *keyframe;
    RaceMotionCoord *coord0;
    RaceMotionCoord *coord1;
    RaceMotionCoord *coord2;
    RaceMotionCoord *coord3;
    s32 x0;
    s32 x1;
    s32 x2;
    s32 x3;
    s32 z0;
    s32 z1;
    s32 z2;
    s32 z3;
    s32 length;
    s32 normalX;
    s32 tangentX;
    s32 alongEdge;

    keyframe = &gRaceCourseSurfaces[arg0];
    coord0 = &gRaceCourseSurfaceCoords[keyframe->coordIndices[0]];
    coord1 = &gRaceCourseSurfaceCoords[keyframe->coordIndices[1]];
    coord2 = &gRaceCourseSurfaceCoords[keyframe->coordIndices[2]];
    coord3 = &gRaceCourseSurfaceCoords[keyframe->coordIndices[3]];

    x0 = coord0->x << 0x11;
    x1 = coord1->x << 0x11;
    x2 = coord2->x << 0x11;
    x3 = coord3->x << 0x11;
    z0 = coord0->z << 0x11;
    z1 = coord1->z << 0x11;
    z2 = coord2->z << 0x11;
    z3 = coord3->z << 0x11;

    gRaceCourseCollisionAdjustedX = x;
    gRaceCourseCollisionAdjustedZ = z;

    if (keyframe->nextFaceIndices[0] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x1 - x0;
        gRaceCourseCollisionEdgeDeltaZ = z1 - z0;
        gRaceCourseCollisionDeltaX = x - x0;
        gRaceCourseCollisionDeltaZ = z - z0;
        pushRaceCourseSurfaceBoundaryWithVelocity(velocityX, velocityZ, radius);
        keyframe = &gRaceCourseSurfaces[arg0];
    }

    if (keyframe->nextFaceIndices[1] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x2 - x3;
        gRaceCourseCollisionEdgeDeltaZ = z2 - z3;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x3;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z3;
        pushRaceCourseSurfaceBoundaryWithVelocity(velocityX, velocityZ, radius);
        keyframe = &gRaceCourseSurfaces[arg0];
    }

    if (keyframe->unk4[0] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x3 - x1;
        gRaceCourseCollisionEdgeDeltaZ = z3 - z1;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x1;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z1;

        length = integerSquareRoot64((s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX + (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ);
        normalX = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
        tangentX = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
        alongEdge = ((s64)tangentX * gRaceCourseCollisionDeltaX + (s64)normalX * gRaceCourseCollisionDeltaZ) / 0x1000;

        if ((keyframe->unk14[3] & 2) && (alongEdge < 0)) {
            clampRaceCourseSurfaceCollisionToEndpoint(x1, z1, radius);
        } else if ((keyframe->unk14[3] & 8) && (length < alongEdge)) {
            clampRaceCourseSurfaceCollisionToEndpoint(x3, z3, radius);
        } else {
            pushRaceCourseSurfaceCollisionFromEdge(radius, velocityX, velocityZ);
        }

        keyframe = &gRaceCourseSurfaces[arg0];
    }

    if (keyframe->unk4[1] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x0 - x2;
        gRaceCourseCollisionEdgeDeltaZ = z0 - z2;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x2;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z2;

        length = integerSquareRoot64((s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX + (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ);
        normalX = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
        tangentX = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
        alongEdge = ((s64)tangentX * gRaceCourseCollisionDeltaX + (s64)normalX * gRaceCourseCollisionDeltaZ) / 0x1000;

        if ((keyframe->unk14[3] & 4) && (alongEdge < 0)) {
            clampRaceCourseSurfaceCollisionToEndpoint(x2, z2, radius);
        } else if ((keyframe->unk14[3] & 1) && (length < alongEdge)) {
            clampRaceCourseSurfaceCollisionToEndpoint(x0, z0, radius);
        } else {
            pushRaceCourseSurfaceCollisionFromEdge(radius, velocityX, velocityZ);
        }
    }

    *pushX = gRaceCourseCollisionAdjustedX - x;
    *pushZ = gRaceCourseCollisionAdjustedZ - z;
}
#endif

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

// resolveRaceCourseSurfaceCollision best match: 74.835% (base_3.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/motion/race_motion/resolveRaceCourseSurfaceCollision.s")

#ifdef NON_MATCHING
void resolveRaceCourseSurfaceCollision(s16 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4, s32 *arg5) {
    RaceMotionSurface *keyframe;
    s32 keyframeOffset;
    s32 x0;
    s32 x1;
    s32 x2;
    s32 x3;
    s32 z0;
    s32 z1;
    s32 z2;
    s32 z3;
    s32 length;
    s32 nx;
    s32 nz;
    s32 along;
    s32 side;
    s32 edgeLength;
    s32 deltaX;
    s32 deltaZ;
    s32 dist;
    u16 flags;

    keyframe = &gRaceCourseSurfaces[arg0];
    x0 = gRaceCourseSurfaceCoords[keyframe->coordIndices[0]].x << 0x11;
    x1 = gRaceCourseSurfaceCoords[keyframe->coordIndices[1]].x << 0x11;
    x2 = gRaceCourseSurfaceCoords[keyframe->coordIndices[2]].x << 0x11;
    x3 = gRaceCourseSurfaceCoords[keyframe->coordIndices[3]].x << 0x11;
    z0 = gRaceCourseSurfaceCoords[keyframe->coordIndices[0]].z << 0x11;
    z1 = gRaceCourseSurfaceCoords[keyframe->coordIndices[1]].z << 0x11;
    z2 = gRaceCourseSurfaceCoords[keyframe->coordIndices[2]].z << 0x11;
    z3 = gRaceCourseSurfaceCoords[keyframe->coordIndices[3]].z << 0x11;

    gRaceCourseCollisionAdjustedX = arg1;
    gRaceCourseCollisionAdjustedZ = arg2;
    keyframeOffset = arg0 * sizeof(RaceMotionSurface);

    if (keyframe->nextFaceIndices[0] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x1 - x0;
        gRaceCourseCollisionEdgeDeltaZ = z1 - z0;
        gRaceCourseCollisionDeltaX = arg1 - x0;
        gRaceCourseCollisionDeltaZ = arg2 - z0;
        pushRaceCourseSurfaceBoundary(arg3);
        keyframe = (RaceMotionSurface *)((s32)gRaceCourseSurfaces + keyframeOffset);
    }

    if (keyframe->nextFaceIndices[1] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x2 - x3;
        gRaceCourseCollisionEdgeDeltaZ = z2 - z3;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x3;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z3;
        pushRaceCourseSurfaceBoundary(arg3);
        keyframe = (RaceMotionSurface *)((s32)gRaceCourseSurfaces + keyframeOffset);
    }

    if (keyframe->unk4[0] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x3 - x1;
        gRaceCourseCollisionEdgeDeltaZ = z3 - z1;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x1;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z1;

        length = integerSquareRoot64((s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX + (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ);
        nx = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
        nz = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
        along = ((s64)nz * gRaceCourseCollisionDeltaX + (s64)nx * gRaceCourseCollisionDeltaZ) / 0x1000;
        side = ((s64)-nx * gRaceCourseCollisionDeltaX + (s64)nz * gRaceCourseCollisionDeltaZ) / 0x1000;
        edgeLength = ((s64)nz * gRaceCourseCollisionEdgeDeltaX + (s64)nx * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
        flags = ((RaceMotionSurface *)((s32)gRaceCourseSurfaces + keyframeOffset))->unk14[3];

        if ((flags & 2) && (along < 0)) {
            deltaX = gRaceCourseCollisionAdjustedX - x1;
            deltaZ = gRaceCourseCollisionAdjustedZ - z1;
            dist = integerSquareRoot64((s64)deltaX * deltaX + (s64)deltaZ * deltaZ);
            if (dist < arg3) {
                gRaceCourseCollisionAdjustedX = (((s64)deltaX * arg3) / dist) + x1;
                gRaceCourseCollisionAdjustedZ = (((s64)deltaZ * arg3) / dist) + z1;
            }
            keyframe = (RaceMotionSurface *)((s32)gRaceCourseSurfaces + keyframeOffset);
        } else {
            if (flags & 8) {
                if (edgeLength < along) {
                    deltaX = gRaceCourseCollisionAdjustedX - x3;
                    deltaZ = gRaceCourseCollisionAdjustedZ - z3;
                    dist = integerSquareRoot64((s64)deltaX * deltaX + (s64)deltaZ * deltaZ);
                    if (dist < arg3) {
                        gRaceCourseCollisionAdjustedX = (((s64)deltaX * arg3) / dist) + x3;
                        gRaceCourseCollisionAdjustedZ = (((s64)deltaZ * arg3) / dist) + z3;
                    }
                    keyframe = (RaceMotionSurface *)((s32)gRaceCourseSurfaces + keyframeOffset);
                } else if (-arg3 < side) {
                    gRaceCourseCollisionEdgeDeltaZ = -arg3 - side;
                    gRaceCourseCollisionAdjustedX += ((s64)-nx * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
                    gRaceCourseCollisionAdjustedZ += ((s64)nz * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
                    keyframe = (RaceMotionSurface *)((s32)gRaceCourseSurfaces + keyframeOffset);
                }
            } else if (-arg3 < side) {
                gRaceCourseCollisionEdgeDeltaZ = -arg3 - side;
                gRaceCourseCollisionAdjustedX += ((s64)-nx * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
                gRaceCourseCollisionAdjustedZ += ((s64)nz * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
                keyframe = (RaceMotionSurface *)((s32)gRaceCourseSurfaces + keyframeOffset);
            }
        }
    }

    if (keyframe->unk4[1] < 0) {
        gRaceCourseCollisionEdgeDeltaX = x0 - x2;
        gRaceCourseCollisionEdgeDeltaZ = z0 - z2;
        gRaceCourseCollisionDeltaX = gRaceCourseCollisionAdjustedX - x2;
        gRaceCourseCollisionDeltaZ = gRaceCourseCollisionAdjustedZ - z2;

        length = integerSquareRoot64((s64)gRaceCourseCollisionEdgeDeltaX * gRaceCourseCollisionEdgeDeltaX + (s64)gRaceCourseCollisionEdgeDeltaZ * gRaceCourseCollisionEdgeDeltaZ);
        nx = ((s64)gRaceCourseCollisionEdgeDeltaZ * 0x1000) / length;
        nz = ((s64)gRaceCourseCollisionEdgeDeltaX * 0x1000) / length;
        along = ((s64)nz * gRaceCourseCollisionDeltaX + (s64)nx * gRaceCourseCollisionDeltaZ) / 0x1000;
        side = ((s64)-nx * gRaceCourseCollisionDeltaX + (s64)nz * gRaceCourseCollisionDeltaZ) / 0x1000;
        edgeLength = ((s64)nz * gRaceCourseCollisionEdgeDeltaX + (s64)nx * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
        flags = ((RaceMotionSurface *)((s32)gRaceCourseSurfaces + keyframeOffset))->unk14[3];

        if ((flags & 4) && (along < 0)) {
            deltaX = gRaceCourseCollisionAdjustedX - x2;
            deltaZ = gRaceCourseCollisionAdjustedZ - z2;
            dist = integerSquareRoot64((s64)deltaX * deltaX + (s64)deltaZ * deltaZ);
            if (dist < arg3) {
                gRaceCourseCollisionAdjustedX = (((s64)deltaX * arg3) / dist) + x2;
                gRaceCourseCollisionAdjustedZ = (((s64)deltaZ * arg3) / dist) + z2;
            }
        } else {
            if (flags & 1) {
                if (edgeLength < along) {
                    deltaX = gRaceCourseCollisionAdjustedX - x0;
                    deltaZ = gRaceCourseCollisionAdjustedZ - z0;
                    dist = integerSquareRoot64((s64)deltaX * deltaX + (s64)deltaZ * deltaZ);
                    if (dist < arg3) {
                        gRaceCourseCollisionAdjustedX = (((s64)deltaX * arg3) / dist) + x0;
                        gRaceCourseCollisionAdjustedZ = (((s64)deltaZ * arg3) / dist) + z0;
                    }
                } else if (-arg3 < side) {
                    gRaceCourseCollisionEdgeDeltaZ = -arg3 - side;
                    gRaceCourseCollisionAdjustedX += ((s64)-nx * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
                    gRaceCourseCollisionAdjustedZ += ((s64)nz * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
                }
            } else if (-arg3 < side) {
                gRaceCourseCollisionEdgeDeltaZ = -arg3 - side;
                gRaceCourseCollisionAdjustedX += ((s64)-nx * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
                gRaceCourseCollisionAdjustedZ += ((s64)nz * gRaceCourseCollisionEdgeDeltaZ) / 0x1000;
            }
        }
    }

    *arg4 = gRaceCourseCollisionAdjustedX - arg1;
    *arg5 = gRaceCourseCollisionAdjustedZ - arg2;
}
#endif

// getRaceCourseSurfaceHeight best match: 99.071% (nonmatchings/getRaceCourseSurfaceHeight-4/output-906-1/source.c)
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
    s64 lhs;

    keyframeOffset = arg0 * sizeof(RaceMotionSurface);
    faceIndex = ((RaceMotionSurface *)((s32)gRaceCourseSurfaces + keyframeOffset))->unk14[0];
    if (faceIndex < ((RaceMotionSurface *)((s32)gRaceCourseSurfaces + keyframeOffset))->unk14[1]) {
        faceOffset = faceIndex * sizeof(RaceMotionFace);
        do {
            face = (RaceMotionFace *)((s32)gRaceCourseSurfaceFaces + faceOffset);
            if (face->unk7 == 0) {
                coords = gRaceCourseSurfaceCoords;
                coord0 = (RaceMotionCoord *)((s32)coords + face->coord0 * sizeof(RaceMotionCoord));
                lhs = (s64)((coords[face->coord1].x << 0x11) - (coord0->x << 0x11)) *
                    (arg2 - (coord0->z << 0x11));

                face = (RaceMotionFace *)((s32)gRaceCourseSurfaceFaces + faceOffset);
                coords = gRaceCourseSurfaceCoords;
                coord0 = (RaceMotionCoord *)((s32)coords + face->coord0 * sizeof(RaceMotionCoord));
                if (lhs - (s64)((coords[face->coord1].z << 0x11) - (coord0->z << 0x11)) *
                    (arg1 - (coord0->x << 0x11)) < 0) {
                    goto next;
                }

                face = (RaceMotionFace *)((s32)gRaceCourseSurfaceFaces + faceOffset);
            }

            coords = gRaceCourseSurfaceCoords;
            coord1 = (RaceMotionCoord *)((s32)coords + face->coord1 * sizeof(RaceMotionCoord));
            coord0 = coords;
            lhs = (s64)((coord0[face->coord2].x << 0x11) - (coord1->x << 0x11)) *
                (arg2 - (coord1->z << 0x11));

            face = (RaceMotionFace *)((s32)gRaceCourseSurfaceFaces + faceOffset);
            coords = gRaceCourseSurfaceCoords;
            coord1 = (RaceMotionCoord *)((s32)coords + face->coord1 * sizeof(RaceMotionCoord));
            if (lhs - (s64)((coords[face->coord2].z << 0x11) - (coord1->z << 0x11)) *
                (arg1 - (coord1->x << 0x11)) >= 0) {
                register RaceMotionCoord *coord2;
                s16 x20;
                s16 y20;
                s16 z20;
                s16 x10;
                s16 y10;
                s16 z10;
                register s32 numeratorA;
                register s32 xDelta;
                register s32 zDelta;

                face = (RaceMotionFace *)((s32)gRaceCourseSurfaceFaces + faceOffset);
                coords = gRaceCourseSurfaceCoords;
                coord2 = (RaceMotionCoord *)((s32)coords + face->coord2 * sizeof(RaceMotionCoord));
                coord0 = (RaceMotionCoord *)((s32)coords + face->coord0 * sizeof(RaceMotionCoord));

                x20 = coord2->x - coord0->x;
                y20 = coord2->y - coord0->y;
                z20 = coord2->z - coord0->z;
                coord2 = coord0;
                coord1 = (RaceMotionCoord *)((s32)coords + face->coord1 * sizeof(RaceMotionCoord));
                x10 = coord1->x - coord2->x;
                y10 = coord1->y - coord2->y;
                z10 = coord1->z - coord2->z;
                xDelta = arg1 - (coord2->x << 0x11);
                zDelta = arg2 - (coord2->z << 0x11);

                numeratorA = (y20 * z10) - (z20 * y10);
                return (-((s64)numeratorA * xDelta) -
                    ((s64)((x20 * y10) - (y20 * x10)) * zDelta)) /
                    ((z20 * x10) - (x20 * z10))
                    + (gRaceCourseSurfaceCoords[((RaceMotionFace *)((s32)gRaceCourseSurfaceFaces + faceOffset))->coord0].y << 0x11);
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

// getRaceCourseTargetPositionAhead best match: 98.993% (nonmatchings/getRaceCourseTargetPositionAhead-7501265508483202319/base.c)
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

    if (arg0 != gRaceCourseStartEntries[gRaceCourseIndex].pathIndex) {
        keyframeOffset = arg0 * sizeof(RaceMotionSurface);
        deltaZ = arg0;
        gRaceCourseSurfaceAngleSin = fixedSine(gRaceCourseSurfaces[deltaZ].angle);
        gRaceCourseSurfaceAngleCos = fixedCosine(gRaceCourseSurfaces[arg0].angle);

        deltaX = arg1 - (gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].positionIndex].x << 0x11);
        deltaZ = arg2 - (gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].positionIndex].z << 0x11);
        projected = ((s64)-gRaceCourseSurfaceAngleSin * deltaX + (s64)gRaceCourseSurfaceAngleCos * deltaZ) / 0x1000;

        if ((arg0 >= gRaceCourseStartEntries[gRaceCourseIndex].unk38) && ((upperSurfaceIndex = gRaceCourseStartEntries[gRaceCourseIndex].unk3A) >= arg0)) {
            distance = projected + 0xC00000;
            *arg3 = ((s64)-gRaceCourseSurfaceAngleSin * distance) / 0x1000;
            deltaZ = gRaceCourseSurfaceAngleCos;
            *arg4 = ((s64)deltaZ * distance) / 0x1000;
        } else if ((gRaceCourseIndex == 3) && ((arg0 == 0x11D) || (arg0 == 0x11E))) {
            distance = projected + 0xC00000;
            *arg3 = (distance * (s64)-gRaceCourseSurfaceAngleSin) / 0x1000;
            *arg4 = ((s64)gRaceCourseSurfaceAngleCos * distance) / 0x1000;
        } else {
            distance = projected + 0xFF400000;
            *arg3 = ((s64)-gRaceCourseSurfaceAngleSin * distance) / 0x1000;
            *arg4 = ((s64)gRaceCourseSurfaceAngleCos * distance) / 0x1000;
        }

        *arg3 += gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].positionIndex].x << 0x11;
        *arg4 = *arg4 + (gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].positionIndex].z << 0x11);

        pathIndex = findRaceCourseSurfaceFromHint(arg0, *arg3, *arg4 ^ 0);
        gRaceCourseSurfaceAngleSin = fixedSine(gRaceCourseSurfaces[(s32)pathIndex].angle);
        keyframeOffset = pathIndex * sizeof(RaceMotionSurface);
        gRaceCourseSurfaceAngleCos = fixedCosine(gRaceCourseSurfaces[pathIndex].angle);

        deltaX = *arg3 - (gRaceCourseSurfaceCoords[gRaceCourseSurfaces[pathIndex].positionIndex].x << 0x11);
        deltaZ = gRaceCourseSurfaceCoords[gRaceCourseSurfaces[pathIndex].positionIndex].z;
        deltaZ = *arg4 - (deltaZ << 0x11);
        distance = updateRacePlayerSmoothedPathOffset(arg5, pathIndex, arg6);
        projected = ((s64)-gRaceCourseSurfaceAngleSin * deltaX + (s64)gRaceCourseSurfaceAngleCos * deltaZ) / 0x1000;

        *arg3 = ((s64)gRaceCourseSurfaceAngleCos * distance + (s64)(0, deltaZ = (keyframeOffset = -gRaceCourseSurfaceAngleSin)) * projected) / 0x1000;
        *arg4 = ((s64)gRaceCourseSurfaceAngleSin * distance + (s64)gRaceCourseSurfaceAngleCos * projected) / 0x1000;
        *arg3 += gRaceCourseSurfaceCoords[gRaceCourseSurfaces[pathIndex].positionIndex].x << 0x11;
        *arg4 += gRaceCourseSurfaceCoords[gRaceCourseSurfaces[pathIndex].positionIndex].z << 0x11;
    } else {
        *arg3 = gRaceCourseStartEntries[gRaceCourseIndex].unk18;
        *arg4 = gRaceCourseStartEntries[gRaceCourseIndex].unk1C;
    }
}
#endif

void getRaceCourseProgressPosition(s32 arg0, s32 *arg1, s32 *arg2, s32 arg3) {
    s32 unused[4];
    s32 temp_v0;

    if (arg0 != gRaceCourseStartEntries[gRaceCourseIndex].pathIndex) {
        gRaceCourseSurfaceAngleSin = fixedSine(gRaceCourseSurfaces[arg0].angle);
        gRaceCourseSurfaceAngleCos = fixedCosine(gRaceCourseSurfaces[arg0].angle);
        temp_v0 = getRacePlayerPathOffset(arg3, arg0);
        *arg1 = ((s64)gRaceCourseSurfaceAngleCos * temp_v0) / 0x1000;
        *arg2 = ((s64)gRaceCourseSurfaceAngleSin * temp_v0) / 0x1000;
        *arg1 += gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].positionIndex].x << 0x11;
        *arg2 += gRaceCourseSurfaceCoords[gRaceCourseSurfaces[arg0].positionIndex].z << 0x11;
    } else {
        *arg1 = gRaceCourseStartEntries[gRaceCourseIndex].unk40;
        *arg2 = gRaceCourseStartEntries[gRaceCourseIndex].unk44;
    }
}

s16 getRaceCourseNextSurface(s32 arg0) {
    return gRaceCourseSurfaces[arg0].nextFaceIndices[1];
}

void setRaceMotionAnimation(RaceMotionState *state, s32 animIndex) {
    s16 *animationFrame;
    s16 frameCount;
    s32 frameDataOffset;
    s32 animationBase;
    RaceMotionState *motionState = state;

    animationBase = getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
    animationFrame = (s16 *)(animationBase + (((u16 *)animationBase)[animIndex] * sizeof(u16)));
    frameCount = *animationFrame;
    motionState->framesRemaining = frameCount;
    motionState->framesRemaining++;
    motionState->frameCount = frameCount;
    animationFrame += 1;
    frameDataOffset = (s32)animationFrame - getRelocatableHeapBlockBase(gAssetHandles[0x16 + motionState->modelId]);
    motionState->animStartOffset = frameDataOffset;
    motionState->frameDataOffset = frameDataOffset;
    motionState->frameTimer = 0;
    motionState->animIndex = animIndex;
}

void loadRaceMotionAnimationFrame(RaceMotionState *state) {
    s16 *frameData;
    RaceMotionState *cursor;
    s32 one;
    s32 jointOffset;
    s32 base;
    s16 value0;
    s16 value1;
    s16 angle;

    base = getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
    frameData = (s16 *)(((one = base) & 0xFFFFFFFFu) + state->frameDataOffset);
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

    state->frameDataOffset = (s32)frameData - getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
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
    s32 base;
    RaceMotionPartialJointCursor *jointCursor;
    s32 one;
    s32 jointOffset;

    base = getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
    packedRotation =
        (RaceMotionPackedJointRotation *)(((one = base) + state->frameDataOffset) + RACE_MOTION_PACKED_JOINT_ROTATION_SKIP_BYTES);
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

    state->frameDataOffset = (s32)packedRotation - getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
}

// interpolateRaceMotionAnimationFrame best match: 78.465% (base_9.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/motion/race_motion/interpolateRaceMotionAnimationFrame.s")

#ifdef NON_MATCHING
#define RACE_MOTION_JOINT_COUNT 12
#define RACE_MOTION_FRAME_SAMPLE_COUNT 2
#define RACE_MOTION_JOINT_FRAME_STRIDE 14

void interpolateRaceMotionAnimationFrame(RaceMotionState *state, s32 animIndex, s32 frameTimer, s32 frameTimerReset) {
    s16 *frameData;
    s32 base;
    s32 i;
    s32 jointOffset;
    volatile RaceMotionRotation *rootFrame;
    volatile RaceMotionRotation *rootSample;
    RaceMotionRotation *jointFrame;
    RaceMotionRotation *jointSample;
    RaceMotionStateJoint *jointCursor;
    s32 start;
    s32 delta;
    s32 x;
    s32 y;
    s32 z;
    s16 packed0;
    s16 packed1;

    base = getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
    frameData = (s16 *)(base + (((u16 *)base)[animIndex] * 2) + 2);

    rootFrame = (RaceMotionRotation *)gRaceMotionRotationFrameBuffer;
    jointFrame = gRaceMotionJointFrameBuffer;
    do {
        i = 0;
        rootSample = rootFrame;
loop_rot:
        rootSample->x = frameData[0] << 14;
        rootSample->y = frameData[1] << 14;
        rootSample->z = frameData[2] << 14;
        i++;
        rootSample++;
        frameData += 3;
        if (i < RACE_MOTION_FRAME_SAMPLE_COUNT) {
            goto loop_rot;
        }

        jointOffset = 0;
        jointSample = jointFrame;
loop_pos:
        packed0 = frameData[0];
        packed1 = frameData[1];
        x = (packed0 >> 4) & 0xFF0;
        y = (packed0 << 4) & 0xFF0;
        z = (packed1 >> 4) & 0xFF0;
        frameData += 2;
        jointSample->x = x;
        jointSample->y = y;
        jointSample->z = z;
        if (packed1 & 1) {
            jointSample->x = x + 8;
        }
        if (packed1 & 2) {
            jointSample->y = y + 8;
        }
        jointOffset += sizeof(RaceMotionRotation);
        if (packed1 & 4) {
            jointSample->z = z + 8;
        }
        jointSample++;
        if (jointOffset < RACE_MOTION_JOINT_COUNT * sizeof(RaceMotionRotation)) {
            goto loop_pos;
        }

        jointFrame += RACE_MOTION_JOINT_FRAME_STRIDE;
        rootFrame += RACE_MOTION_FRAME_SAMPLE_COUNT;
    } while ((u32)jointFrame < (u32)&gRacePlayerHitCueId);

    jointSample = gRaceMotionJointFrameBuffer;
    i = 0;
    jointCursor = state->animation.joints;
    do {
        start = jointSample->x;
        delta = (jointSample[RACE_MOTION_JOINT_FRAME_STRIDE].x - start) & 0xFFF;
        if (delta >= 0x801) {
            delta -= 0x1000;
        }
        jointCursor->rotationX = start + ((delta * frameTimer) / frameTimerReset);

        start = jointSample->y;
        delta = (jointSample[RACE_MOTION_JOINT_FRAME_STRIDE].y - start) & 0xFFF;
        if (delta >= 0x801) {
            delta -= 0x1000;
        }
        jointCursor->rotationY = start + ((delta * frameTimer) / frameTimerReset);

        start = jointSample->z;
        delta = (jointSample[RACE_MOTION_JOINT_FRAME_STRIDE].z - start) & 0xFFF;
        if (delta >= 0x801) {
            delta -= 0x1000;
        }
        jointCursor->rotationZ = start + ((delta * frameTimer) / frameTimerReset);
        i++;
        jointSample++;
        jointCursor++;
    } while (i != RACE_MOTION_JOINT_COUNT);

    state->animation.motion.rootMotion.position.x =
        gRaceMotionRotationFrameBuffer[0] + (((gRaceMotionRotationFrameBuffer[6] - gRaceMotionRotationFrameBuffer[0]) * frameTimer) / frameTimerReset);
    state->animation.motion.rootMotion.position.y =
        gRaceMotionRotationFrameBuffer[1] + (((gRaceMotionRotationFrameBuffer[7] - gRaceMotionRotationFrameBuffer[1]) * frameTimer) / frameTimerReset);
    state->animation.motion.rootMotion.position.z =
        gRaceMotionRotationFrameBuffer[2] + (((gRaceMotionRotationFrameBuffer[8] - gRaceMotionRotationFrameBuffer[2]) * frameTimer) / frameTimerReset);
    state->animation.motion.rootMotion.rotation.x =
        gRaceMotionRotationFrameBuffer[3] + (((gRaceMotionRotationFrameBuffer[9] - gRaceMotionRotationFrameBuffer[3]) * frameTimer) / frameTimerReset);
    state->animation.motion.rootMotion.rotation.y =
        gRaceMotionRotationFrameBuffer[4] + (((gRaceMotionRotationFrameBuffer[10] - gRaceMotionRotationFrameBuffer[4]) * frameTimer) / frameTimerReset);

    state->modelJointX = state->animation.motion.jointX;
    state->nextModelJointX = state->animation.motion.jointX;
    state->nextModelJointY = state->animation.motion.jointY;
    state->modelJointY = state->animation.motion.jointY;
    state->nextModelJointZ = state->animation.motion.jointZ;
    state->modelJointZ = state->animation.motion.jointZ;

    state->animation.motion.rootMotion.rotation.z =
        gRaceMotionRotationFrameBuffer[5] + (((gRaceMotionRotationFrameBuffer[11] - gRaceMotionRotationFrameBuffer[5]) * frameTimer) / frameTimerReset);
}

#undef RACE_MOTION_JOINT_COUNT
#undef RACE_MOTION_FRAME_SAMPLE_COUNT
#undef RACE_MOTION_JOINT_FRAME_STRIDE
#endif

// interpolateRaceMotionJointAnimationFrame best match: 81.286% (base_8.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/motion/race_motion/interpolateRaceMotionJointAnimationFrame.s")

#ifdef NON_MATCHING
#define RACE_MOTION_INTERPOLATED_JOINT_COUNT 5
#define RACE_MOTION_JOINT_COUNT 12
#define RACE_MOTION_FRAME_SAMPLE_COUNT 2
#define RACE_MOTION_JOINT_FRAME_STRIDE 14

void interpolateRaceMotionJointAnimationFrame(RaceMotionState *state, s32 animIndex, s32 frameTimer, s32 frameTimerReset) {
    s16 *frameData;
    s32 base;
    s32 i;
    s32 jointOffset;
    volatile RaceMotionRotation *rootFrame;
    volatile RaceMotionRotation *rootSample;
    RaceMotionRotation *jointFrame;
    RaceMotionRotation *jointSample;
    RaceMotionState *jointCursor;
    s32 start;
    s32 delta;
    s32 x;
    s32 y;
    s32 z;
    s16 packed0;
    s16 packed1;

    base = getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
    frameData = (s16 *)(base + (((u16 *)base)[animIndex] * 2) + 2);

    rootFrame = (RaceMotionRotation *)gRaceMotionRotationFrameBuffer;
    jointFrame = (RaceMotionRotation *)gRaceMotionJointFrameBuffer;
    do {
        i = 0;
        rootSample = rootFrame;
loop_rot:
        rootSample->x = frameData[0] << 14;
        rootSample->y = frameData[1] << 14;
        rootSample->z = frameData[2] << 14;
        i++;
        rootSample++;
        frameData += 3;
        if (i < RACE_MOTION_FRAME_SAMPLE_COUNT) {
            goto loop_rot;
        }

        jointOffset = 0;
        jointSample = jointFrame;
loop_pos:
        packed0 = frameData[0];
        packed1 = frameData[1];
        x = (packed0 >> 4) & 0xFF0;
        y = (packed0 << 4) & 0xFF0;
        z = (packed1 >> 4) & 0xFF0;
        frameData += 2;
        jointSample->x = x;
        jointSample->y = y;
        jointSample->z = z;
        if (packed1 & 1) {
            jointSample->x = x + 8;
        }
        if (packed1 & 2) {
            jointSample->y += 8;
        }
        jointOffset += sizeof(RaceMotionRotation);
        if (packed1 & 4) {
            jointSample->z += 8;
        }
        jointSample++;
        if (jointOffset < RACE_MOTION_INTERPOLATED_JOINT_COUNT * sizeof(RaceMotionRotation)) {
            goto loop_pos;
        }

        i = RACE_MOTION_INTERPOLATED_JOINT_COUNT;
loop_skip:
        i++;
        frameData += 2;
        if (i < RACE_MOTION_JOINT_COUNT) {
            goto loop_skip;
        }

        jointFrame += RACE_MOTION_JOINT_FRAME_STRIDE;
        rootFrame += RACE_MOTION_FRAME_SAMPLE_COUNT;
    } while ((u32)jointFrame < (u32)&gRacePlayerHitCueId);

    jointSample = (RaceMotionRotation *)gRaceMotionJointFrameBuffer;
    i = 0;
    jointCursor = state;
loop_interp:
    start = jointSample->x;
    delta = (jointSample[RACE_MOTION_JOINT_FRAME_STRIDE].x - start) & 0xFFF;
    if (delta >= 0x801) {
        delta -= 0x1000;
    }
    jointCursor->animation.motion.jointX = start + ((delta * frameTimer) / frameTimerReset);

    start = jointSample->y;
    delta = (jointSample[RACE_MOTION_JOINT_FRAME_STRIDE].y - start) & 0xFFF;
    if (delta >= 0x801) {
        delta -= 0x1000;
    }
    jointCursor->animation.motion.jointY = start + ((delta * frameTimer) / frameTimerReset);

    start = jointSample->z;
    delta = (jointSample[RACE_MOTION_JOINT_FRAME_STRIDE].z - start) & 0xFFF;
    if (delta >= 0x801) {
        delta -= 0x1000;
    }
    i++;
    jointSample++;
    jointCursor = (RaceMotionState *)((u8 *)jointCursor + sizeof(RaceMotionStateJoint));
    jointCursor->animation.motion.jointZ = start + ((delta * frameTimer) / frameTimerReset);
    if (i != RACE_MOTION_INTERPOLATED_JOINT_COUNT) {
        goto loop_interp;
    }

    state->animation.motion.rootMotion.position.x =
        gRaceMotionRotationFrameBuffer[0] + (((gRaceMotionRotationFrameBuffer[6] - gRaceMotionRotationFrameBuffer[0]) * frameTimer) / frameTimerReset);
    state->animation.motion.rootMotion.position.y =
        gRaceMotionRotationFrameBuffer[1] + (((gRaceMotionRotationFrameBuffer[7] - gRaceMotionRotationFrameBuffer[1]) * frameTimer) / frameTimerReset);
    state->animation.motion.rootMotion.position.z =
        gRaceMotionRotationFrameBuffer[2] + (((gRaceMotionRotationFrameBuffer[8] - gRaceMotionRotationFrameBuffer[2]) * frameTimer) / frameTimerReset);
    state->animation.motion.rootMotion.rotation.x =
        gRaceMotionRotationFrameBuffer[3] + (((gRaceMotionRotationFrameBuffer[9] - gRaceMotionRotationFrameBuffer[3]) * frameTimer) / frameTimerReset);
    state->animation.motion.rootMotion.rotation.y =
        gRaceMotionRotationFrameBuffer[4] + (((gRaceMotionRotationFrameBuffer[10] - gRaceMotionRotationFrameBuffer[4]) * frameTimer) / frameTimerReset);

    state->modelJointX = state->animation.motion.jointX;
    state->nextModelJointX = state->animation.motion.jointX;
    state->nextModelJointY = state->animation.motion.jointY;
    state->modelJointY = state->animation.motion.jointY;
    state->nextModelJointZ = state->animation.motion.jointZ;
    state->modelJointZ = state->animation.motion.jointZ;

    state->animation.motion.rootMotion.rotation.z =
        gRaceMotionRotationFrameBuffer[5] + (((gRaceMotionRotationFrameBuffer[11] - gRaceMotionRotationFrameBuffer[5]) * frameTimer) / frameTimerReset);
}

#undef RACE_MOTION_INTERPOLATED_JOINT_COUNT
#undef RACE_MOTION_JOINT_COUNT
#undef RACE_MOTION_FRAME_SAMPLE_COUNT
#undef RACE_MOTION_JOINT_FRAME_STRIDE
#endif

// blendRaceMotionJointAnimation best match: 99.038% (nonmatchings/blendRaceMotionJointAnimation-8075865578671233833/base_11.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race/motion/race_motion/blendRaceMotionJointAnimation.s")

#ifdef NON_MATCHING
void blendRaceMotionJointAnimation(RaceMotionState *state, s32 animIndex, s32 timer, s32 duration) {
    s32 frameBase;
    s16 *packed;
    RaceMotionRotation *row;
    RaceMotionRotation *out;
    RaceMotionRotation *end;
    s32 *interp;
    RaceMotionRotation *outAlias;
    void *joint;
    s32 i;
    s16 xy;
    s16 shiftedX;
    s16 zAndFlags;
    s32 jointStride;
    s32 x;
    u16 frameOffset;
    s32 y;
    s32 z;
    s32 delta;

    frameBase = getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
    frameOffset = ((u16 *)frameBase)[animIndex];
    packed = (s16 *)(frameBase + (frameOffset * 2));
    packed++;
    state++;
    state--;
    packed = (s16 *)((u8 *)packed + 0x24);
    row = gRaceMotionJointFrameBuffer;
    end = (RaceMotionRotation *)&gRacePlayerHitCueId;
    do {
        i = 0x48;
        out = row + 6;
        do {
            xy = packed[0];
            zAndFlags = packed[1];
            shiftedX = xy >> 4;
            x = shiftedX & 0xFF0;
            y = (xy << 4) & 0xFF0;
            z = (zAndFlags >> 4) & 0xFF0;
            packed += 2;
            out->x = x;
            out->y = y;
            out->z = z;
            if (zAndFlags & 1) {
                out->x = x + 8;
            }
            if ((zAndFlags & 2) & 0xFFFF) {
                out->y += 8;
            }
            i += 0xC;
            outAlias = out;
            if (zAndFlags & 4) {
                outAlias->z += 8;
            }
            out++;
        } while (i < 0x90);
 row += 14; packed = (s16 *)((u8 *)packed + 0x24); } while (row < end); interp = gRaceMotionJointBlendBuffer; i = 6; joint = (u8 *)state + 0x78; loop: x = interp[0]; jointStride = 0x14; delta = (interp[42] - x) & 0xFFF; if (delta >= 0x801) { delta -= 0x1000; } *(s16 *)((u8 *)joint + 0x33A) = x + ((delta * timer) / duration); y = interp[1]; delta = (interp[43] - y) & 0xFFF; if (delta >= 0x801) { delta -= 0x1000; } *(s16 *)((u8 *)joint + 0x33C) = y + ((delta * timer) / duration); z = interp[2]; delta = (interp[44] - z) & 0xFFF; if (delta >= 0x801) { delta -= 0x1000; } i++; interp += 3; joint = (u8 *)joint + jointStride; *(s16 *)((u8 *)joint + 0x32A) = z + ((delta * timer) / duration);
    if (i != 12) {
        goto loop;
    }
}
#endif

void stepRaceMotionLoopingAnimation(RaceMotionState *state) {
    s32 frameTimer;
    s32 nextFrameTimer;

    frameTimer = state->frameTimer;
    nextFrameTimer = frameTimer - 1;
    if (frameTimer == 0) {
        state->framesRemaining--;
        if (state->framesRemaining <= 0) {
            state->framesRemaining = state->frameCount;
            state->frameDataOffset = state->animStartOffset;
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
            state->frameDataOffset = state->animStartOffset;
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
