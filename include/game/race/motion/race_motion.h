#ifndef RACE_MOTION_H
#define RACE_MOTION_H

#include "common.h"
#include "game/race/player/race_player_input.h"

#define RACE_MOTION_JOINT_COUNT 12
#define RACE_MOTION_FRAME_SAMPLE_COUNT 2
#define RACE_MOTION_DECODED_JOINT_FRAME_PADDING_COUNT 2
#define RACE_MOTION_DECODED_JOINT_FRAME_STRIDE (RACE_MOTION_JOINT_COUNT + RACE_MOTION_DECODED_JOINT_FRAME_PADDING_COUNT)
#define RACE_MOTION_PARTIAL_ANIMATION_JOINT_COUNT 5
#define RACE_MOTION_PARTIAL_ANIMATION_START_JOINT 6
#define RACE_MOTION_PACKED_JOINT_ROTATION_SKIP_BYTES 0x24
#define RACE_MOTION_PACKED_JOINT_ROTATION_PREFIX_COUNT 9
#define RACE_MOTION_MODEL_POSITION_FRAC_BITS 14

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
    u16 unused18;
    u16 edgeClampFlags;
} RaceCourseSurface;

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

typedef struct RaceMotionDecodedRootFrame {
    Vec3i position;
    Vec3i rotation;
} RaceMotionDecodedRootFrame;

typedef struct RaceMotionDecodedJointFrame {
    Vec3i joints[RACE_MOTION_JOINT_COUNT];
    Vec3i stridePadding[RACE_MOTION_DECODED_JOINT_FRAME_PADDING_COUNT];
} RaceMotionDecodedJointFrame;

typedef char RaceCourseSurfaceFaceSizeCheck[(sizeof(RaceCourseSurfaceFace) == 8) ? 1 : -1];
typedef char RaceCourseSurfaceSizeCheck[(sizeof(RaceCourseSurface) == 0x1C) ? 1 : -1];

/* Exposes payloads after each RaceMotionCountedTable header in asset slot 0x1B. */
void initRaceCourseSurfaceData(void);
/* Local adjacency traversal from a nearby hint. All negative neighbors are
 * boundaries. An arbitrary distant hint (including zero) can return an unrelated
 * boundary surface; this is not a global nearest-surface search. */
s32 findRaceCourseSurfaceFromHint(s32 surfaceIndex, s32 x, s32 z);
s32 findRaceCourseSurfaceAtPoint(s32 x, s32 z);
void pushRaceCourseSurfaceBoundaryWithVelocity(s32 *velocityX, s32 *velocityZ, s32 radius);
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
);
void resolveRaceCourseSurfaceCollisionWithVelocity(
    s32 surfaceIndex,
    s32 x,
    s32 z,
    s32 radius,
    s32 *pushX,
    s32 *pushZ,
    s32 *velocityX,
    s32 *velocityZ
);
void pushRaceCourseSurfaceBoundary(s32 radius);
void resolveRaceCourseSurfaceCollision(s32 surfaceIndex, s32 x, s32 z, s32 radius, s32 *pushX, s32 *pushZ);
s32 getRaceCourseSurfaceHeight(s32 surfaceIndex, s32 x, s32 z);
s32 getRaceCourseSurfaceType(s32 surfaceIndex, s32 x, s32 z);
u32 projectRaceCourseSurfaceProgress(s32 surfaceIndex, s32 x, s32 z);
void getRaceCourseSurfaceSpawnTransform(s32 surfaceIndex, s32 *x, s32 *y, s32 *z, s16 *angle);
/* Updates player path-offset smoothing and shared sine/cosine scratch as well as target outputs. */
void getRaceCourseTargetPositionAhead(
    s32 surfaceIndex,
    s32 x,
    s32 z,
    s32 *targetX,
    s32 *targetZ,
    s32 playerIndex,
    s32 lookahead
);
/* Position of one of the four original lanes at a station; writes shared angle scratch. */
void getRaceCourseProgressPosition(s32 surfaceIndex, s32 *x, s32 *z, s32 playerIndex);
s16 getRaceCourseNextSurface(s32 surfaceIndex);
void setRaceMotionAnimation(RacePlayer *player, s32 animationIndex);
void loadRaceMotionAnimationFrame(RacePlayer *player);
void loadRaceMotionJointAnimationFrame(RacePlayer *player);
void interpolateRaceMotionAnimationFrame(RacePlayer *player, s32 animIndex, s32 frameTimer, s32 frameTimerReset);
void interpolateRaceMotionJointAnimationFrame(
    RacePlayer *player,
    s32 animationIndex,
    s32 frameTimer,
    s32 frameTimerReset
);
void blendRaceMotionJointAnimation(RacePlayer *player, s32 animationIndex, s32 blendTimer, s32 blendDuration);
void stepRaceMotionLoopingAnimation(RacePlayer *player);
void stepRaceMotionLoopingJointAnimation(RacePlayer *player);
s32 stepRaceMotionAnimationUntilEnd(RacePlayer *player);
s32 stepRaceMotionJointAnimationUntilEnd(RacePlayer *player);
void initRaceMotionModelParts(RacePlayer *player);

#endif
