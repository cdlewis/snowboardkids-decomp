#ifndef INTERPOLATE_RACE_MOTION_INDEPENDENT_TYPES_H
#define INTERPOLATE_RACE_MOTION_INDEPENDENT_TYPES_H

#include "common.h"
#include "game/engine/asset_manager.h"
#include "game/engine/relocatable_heap.h"
#include "game/race/motion/race_motion.h"

#define RACE_MOTION_JOINT_COUNT 12
#define RACE_MOTION_INTERPOLATED_JOINT_COUNT 5
#define RACE_MOTION_FRAME_SAMPLE_COUNT 2
#define RACE_MOTION_MODEL_POSITION_FRAC_BITS 14

typedef struct RaceMotionRotation {
    s32 x;
    s32 y;
    s32 z;
} RaceMotionRotation;

typedef struct RaceMotionPackedJointRotation {
    s16 xy;
    s16 zAndFlags;
} RaceMotionPackedJointRotation;

typedef struct RaceMotionPackedVector {
    s16 x;
    s16 y;
    s16 z;
} RaceMotionPackedVector;

typedef struct RaceMotionAnimationAsset {
    u16 animationOffsets[1];
} RaceMotionAnimationAsset;

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

typedef struct RaceMotionStateJoint {
    s32 unk0;
    s16 unk4;
    s16 rotationX;
    s16 rotationY;
    s16 rotationZ;
    s32 unkC;
    s32 unk10;
} RaceMotionStateJoint;

typedef struct RaceMotionRootMotion {
    RaceMotionRotation position;
    char padC[8];
    RaceMotionRotation rotation;
} RaceMotionRootMotion;

struct RaceMotionState {
    u16 modelId;
    char pad2[0x332];
    union {
        RaceMotionStateJoint joints[RACE_MOTION_JOINT_COUNT];
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
    char pad444[0x24];
};

extern volatile s32 gRaceMotionRotationFrameBuffer[];
extern RaceMotionRotation gRaceMotionJointFrameBuffer[];
extern s32 gRacePlayerHitCueId;

#endif
