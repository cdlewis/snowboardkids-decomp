#include "independent_types.h"

#pragma weak gRaceMotionRotationDecodeBuffer = gRaceMotionRotationFrameBuffer
#pragma weak gRaceMotionJointDecodeBuffer = gRaceMotionJointFrameBuffer
extern volatile s32 gRaceMotionRotationDecodeBuffer[];
extern RaceMotionRotation gRaceMotionJointDecodeBuffer[];

void interpolateRaceMotionJointAnimationFrame(RaceMotionState *state, s32 animationIndex,
                                              s32 frameTimer, s32 frameTimerReset) {
    RaceMotionAnimationAsset *animationAsset;
    RaceMotionPackedJointRotation *packedRotation;
    RaceMotionDecodedRootFrame *decodedRootFrame;
    RaceMotionDecodedJointFrame *decodedJointFrame;
    RaceMotionDecodedJointFrame *decodedJointFrameEnd;
    RaceMotionRotation *decodedRoot;
    RaceMotionRotation *decodedJoint;
    RaceMotionPartialJointCursor *jointCursor;
    s32 *jointData;
    s32 jointIndex;
    s32 startX;
    s32 startY;
    s32 startZ;
    s32 angleDelta;
    s32 blendOffset;
    s16 packedXY;
    s16 packedZAndFlags;
    s16 rootJointAngle;

    animationAsset = getRelocatableHeapBlockBase(gAssetHandles[0x16 + state->modelId]);
    packedRotation = (RaceMotionPackedJointRotation *)(
        (u16 *)animationAsset + animationAsset->animationOffsets[animationIndex] + 1);
    decodedRootFrame = (RaceMotionDecodedRootFrame *)gRaceMotionRotationDecodeBuffer;
    decodedJointFrame = (RaceMotionDecodedJointFrame *)gRaceMotionJointDecodeBuffer;
    decodedJointFrameEnd = (RaceMotionDecodedJointFrame *)&gRacePlayerHitCueId;

decode_frame:
    decodedRoot = (RaceMotionRotation *)decodedRootFrame;
    for (jointIndex = 0; jointIndex < RACE_MOTION_FRAME_SAMPLE_COUNT; jointIndex++) {
        decodedRoot->x =
            ((RaceMotionPackedVector *)packedRotation)->x << RACE_MOTION_MODEL_POSITION_FRAC_BITS;
        decodedRoot->y =
            ((RaceMotionPackedVector *)packedRotation)->y << RACE_MOTION_MODEL_POSITION_FRAC_BITS;
        decodedRoot->z =
            ((RaceMotionPackedVector *)packedRotation)->z << RACE_MOTION_MODEL_POSITION_FRAC_BITS;
        decodedRoot++;
        packedRotation =
            (RaceMotionPackedJointRotation *)((RaceMotionPackedVector *)packedRotation + 1);
    }

    decodedJoint = decodedJointFrame->joints;
    for (jointIndex = 0; jointIndex < 0x3C; jointIndex += 0xC) {
        packedXY = packedRotation->xy;
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
        decodedJoint++;
    }

    for (jointIndex = RACE_MOTION_INTERPOLATED_JOINT_COUNT;
         jointIndex < RACE_MOTION_JOINT_COUNT; jointIndex++) {
        packedRotation++;
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
    jointCursor =
        (RaceMotionPartialJointCursor *)((u8 *)jointCursor + sizeof(RaceMotionStateJoint));
    if (jointIndex != RACE_MOTION_INTERPOLATED_JOINT_COUNT) {
        goto interpolate_joint;
    }

    decodedRootFrame = (RaceMotionDecodedRootFrame *)gRaceMotionRotationFrameBuffer;
    state->animation.motion.rootMotion.position.x =
        decodedRootFrame[0].position.x +
        ((decodedRootFrame[1].position.x - decodedRootFrame[0].position.x) * frameTimer) /
            frameTimerReset;
    state->animation.motion.rootMotion.position.y =
        decodedRootFrame[0].position.y +
        ((decodedRootFrame[1].position.y - decodedRootFrame[0].position.y) * frameTimer) /
            frameTimerReset;
    state->animation.motion.rootMotion.position.z =
        decodedRootFrame[0].position.z +
        ((decodedRootFrame[1].position.z - decodedRootFrame[0].position.z) * frameTimer) /
            frameTimerReset;
    state->animation.motion.rootMotion.rotation.x =
        decodedRootFrame[0].rotation.x +
        ((decodedRootFrame[1].rotation.x - decodedRootFrame[0].rotation.x) * frameTimer) /
            frameTimerReset;
    state->animation.motion.rootMotion.rotation.y =
        decodedRootFrame[0].rotation.y +
        ((decodedRootFrame[1].rotation.y - decodedRootFrame[0].rotation.y) * frameTimer) /
            frameTimerReset;
    state->animation.motion.rootMotion.rotation.z =
        decodedRootFrame[0].rotation.z +
        ((decodedRootFrame[1].rotation.z - decodedRootFrame[0].rotation.z) * frameTimer) /
            frameTimerReset;

    rootJointAngle = state->animation.motion.jointX;
    state->modelJointX = rootJointAngle;
    state->nextModelJointX = rootJointAngle;
    rootJointAngle = state->animation.motion.jointY;
    state->nextModelJointY = rootJointAngle;
    state->modelJointY = rootJointAngle;
    rootJointAngle = state->animation.motion.jointZ;
    state->nextModelJointZ = rootJointAngle;
    state->modelJointZ = rootJointAngle;
}
