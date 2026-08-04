#define FIXED_PRODUCT(a, b) ((s32)(a) * (b) / 0x1000)
#define COLLISION_POINT(x, y, z, sizeX, sizeY, sizeZ) \
    (((s64)(x) * (sizeX) + (s64)(y) * (sizeY) + (s64)(z) * (sizeZ)) / 0x1000)
#define MIRRORED_COLLISION_POINT(x, y, z, sizeX, sizeY, sizeZ) \
    ((-(s64)(x) * (sizeX) + (s64)(y) * (sizeY) + (s64)(z) * (sizeZ)) / 0x1000)

void resolveRacePlayerCollisionVolumes(RacePlayer *player) {
    Matrix4s localRotations[14];
    s16 parentAxisY;
    FixedMatrix3s yawAdjustedPlayerRotation;
    FixedMatrix3s playerRotation;
    s32 volumeIndex;
    s32 axisRow;
    s32 axisColumn;
    s32 sinX;
    s32 cosX;
    s32 centerOffset[3];
    FixedMatrix3s effectRotation;
    FixedMatrix3s worldRotation;
    s32 sinY;
    s32 cosY;
    s32 sinZ;
    s32 cosZ;
    s32 rotationProduct;
    RacePlayerCollisionSource *source;
    RacePlayerCollisionVolume *volume;
    RacePlayerCollisionVolume *parent;

    player->unk288 = player->pitchAngle;
    player->unk28A = player->facingAngle;
    player->unk28C = player->unk2EE;

    if (player->stateFlags & RACE_PLAYER_COLLISION_YAW_FLIPPED) {
        for (volumeIndex = 0; volumeIndex < player->collisionVolumeCount; volumeIndex++) {
            source =
                &player->collisionSources[player->collisionSources[volumeIndex].mirroredParentIndex];
            sinX = fixedSine(source->rotX);
            cosX = fixedCosine(source->rotX);
            sinY = fixedSine(-source->rotY);
            cosY = fixedCosine(-source->rotY);
            sinZ = fixedSine(-source->rotZ);
            cosZ = fixedCosine(-source->rotZ);

            localRotations[volumeIndex][0] = FIXED_PRODUCT(cosY, cosZ);
            localRotations[volumeIndex][1] = FIXED_PRODUCT(cosY, sinZ);
            localRotations[volumeIndex][2] = -sinY;
            rotationProduct = FIXED_PRODUCT(sinX, sinY);
            localRotations[volumeIndex][3] =
                FIXED_PRODUCT(rotationProduct, cosZ) + FIXED_PRODUCT(cosX, -sinZ);
            localRotations[volumeIndex][4] =
                FIXED_PRODUCT(rotationProduct, sinZ) + FIXED_PRODUCT(cosX, cosZ);
            localRotations[volumeIndex][5] = FIXED_PRODUCT(sinX, cosY);
            rotationProduct = FIXED_PRODUCT(cosX, sinY);
            localRotations[volumeIndex][6] =
                FIXED_PRODUCT(rotationProduct, cosZ) + FIXED_PRODUCT(sinX, sinZ);
            localRotations[volumeIndex][7] =
                FIXED_PRODUCT(rotationProduct, sinZ) + FIXED_PRODUCT(-sinX, cosZ);
            localRotations[volumeIndex][8] = FIXED_PRODUCT(cosX, cosY);

            if (player->stateFlags & RACE_PLAYER_COLLISION_SQUASHED) {
                localRotations[volumeIndex][1] = localRotations[volumeIndex][1] * player->unk2D6 / 8;
                localRotations[volumeIndex][4] = localRotations[volumeIndex][4] * player->unk2D6 / 8;
                localRotations[volumeIndex][7] = localRotations[volumeIndex][7] * player->unk2D6 / 8;
            }
        }

        makeFixedRotationZYX(effectRotation, player->unk6C, -player->unk6E, -player->unk70);
        makeFixedRotationZXY(playerRotation, player->unk288, player->unk28A, player->unk28C);
        centerOffset[0] = (s64)playerRotation[3] * player->collisionCenterOffset / 0x1000;
        centerOffset[1] = (s64)playerRotation[4] * player->collisionCenterOffset / 0x1000;
        centerOffset[2] = (s64)playerRotation[5] * player->collisionCenterOffset / 0x1000;
        multiplyFixedMatrix3s(effectRotation, playerRotation, worldRotation);

        for (sinX = 0; sinX < player->collisionVolumeCount; sinX++) {
            for (sinY = 0; sinY != 9; sinY += 3) {
                for (axisRow = 0; axisRow != 3; axisRow++) {
                    player->collisionVolumes[sinX].axis[sinY + axisRow] =
                        (localRotations[sinX][sinY] * worldRotation[axisRow] +
                         localRotations[sinX][sinY + 1] * worldRotation[axisRow + 3] +
                         localRotations[sinX][sinY + 2] * worldRotation[axisRow + 6]) /
                        0x1000;
                }
            }
        }

        player->collisionVolumes[0].point[0] = MIRRORED_COLLISION_POINT(worldRotation[0], worldRotation[3], worldRotation[6], player->collisionSources[0].sizeX, player->collisionSources[0].sizeY - player->collisionCenterOffset, player->collisionSources[0].sizeZ);
        player->collisionVolumes[0].point[1] = MIRRORED_COLLISION_POINT(worldRotation[1], worldRotation[4], worldRotation[7], player->collisionSources[0].sizeX, player->collisionSources[0].sizeY - player->collisionCenterOffset, player->collisionSources[0].sizeZ);
        player->collisionVolumes[0].point[2] = MIRRORED_COLLISION_POINT(worldRotation[2], worldRotation[5], worldRotation[8], player->collisionSources[0].sizeX, player->collisionSources[0].sizeY - player->collisionCenterOffset, player->collisionSources[0].sizeZ);
        player->collisionVolumes[1].point[0] = MIRRORED_COLLISION_POINT(player->collisionVolumes[0].axis[0], player->collisionVolumes[0].axis[3], player->collisionVolumes[0].axis[6], player->collisionSources[1].sizeX, player->collisionSources[1].sizeY, player->collisionSources[1].sizeZ);
        player->collisionVolumes[1].point[1] = MIRRORED_COLLISION_POINT(player->collisionVolumes[0].axis[1], player->collisionVolumes[0].axis[4], player->collisionVolumes[0].axis[7], player->collisionSources[1].sizeX, player->collisionSources[1].sizeY, player->collisionSources[1].sizeZ);
        player->collisionVolumes[1].point[2] = MIRRORED_COLLISION_POINT(player->collisionVolumes[0].axis[2], player->collisionVolumes[0].axis[5], player->collisionVolumes[0].axis[8], player->collisionSources[1].sizeX, player->collisionSources[1].sizeY, player->collisionSources[1].sizeZ);
    } else {
        for (volumeIndex = 0; volumeIndex < player->collisionVolumeCount; volumeIndex++) {
            source = &player->collisionSources[volumeIndex];
            sinX = fixedSine(source->rotX);
            cosX = fixedCosine(source->rotX);
            sinY = fixedSine(source->rotY);
            cosY = fixedCosine(source->rotY);
            sinZ = fixedSine(source->rotZ);
            cosZ = fixedCosine(source->rotZ);

            localRotations[volumeIndex][0] = FIXED_PRODUCT(cosY, cosZ);
            localRotations[volumeIndex][1] = FIXED_PRODUCT(cosY, sinZ);
            localRotations[volumeIndex][2] = -sinY;
            rotationProduct = FIXED_PRODUCT(sinX, sinY);
            localRotations[volumeIndex][3] =
                FIXED_PRODUCT(rotationProduct, cosZ) + FIXED_PRODUCT(cosX, -sinZ);
            localRotations[volumeIndex][4] =
                FIXED_PRODUCT(rotationProduct, sinZ) + FIXED_PRODUCT(cosX, cosZ);
            localRotations[volumeIndex][5] = FIXED_PRODUCT(sinX, cosY);
            rotationProduct = FIXED_PRODUCT(cosX, sinY);
            localRotations[volumeIndex][6] =
                FIXED_PRODUCT(rotationProduct, cosZ) + FIXED_PRODUCT(sinX, sinZ);
            localRotations[volumeIndex][7] =
                FIXED_PRODUCT(rotationProduct, sinZ) + FIXED_PRODUCT(-sinX, cosZ);
            localRotations[volumeIndex][8] = FIXED_PRODUCT(cosX, cosY);

            if (player->stateFlags & RACE_PLAYER_COLLISION_SQUASHED) {
                localRotations[volumeIndex][1] = localRotations[volumeIndex][1] * player->unk2D6 / 8;
                localRotations[volumeIndex][4] = localRotations[volumeIndex][4] * player->unk2D6 / 8;
                localRotations[volumeIndex][7] = localRotations[volumeIndex][7] * player->unk2D6 / 8;
            }
        }

        makeFixedRotationZYX(effectRotation, player->unk6C, player->unk6E, player->unk70);
        makeFixedRotationZXY(playerRotation, player->unk288, player->unk28A, player->unk28C);
        centerOffset[0] = (s64)playerRotation[3] * player->collisionCenterOffset / 0x1000;
        centerOffset[1] = (s64)playerRotation[4] * player->collisionCenterOffset / 0x1000;
        centerOffset[2] = (s64)playerRotation[5] * player->collisionCenterOffset / 0x1000;
        makeFixedRotationY(worldRotation, RACE_PLAYER_COLLISION_HALF_TURN);
        multiplyFixedMatrix3s(worldRotation, playerRotation, yawAdjustedPlayerRotation);
        multiplyFixedMatrix3s(effectRotation, yawAdjustedPlayerRotation, worldRotation);

        for (sinX = 0; sinX < player->collisionVolumeCount; sinX++) {
            for (sinY = 0; sinY != 9; sinY += 3) {
                for (axisRow = 0; axisRow != 3; axisRow++) {
                    player->collisionVolumes[sinX].axis[sinY + axisRow] =
                        (localRotations[sinX][sinY] * worldRotation[axisRow] +
                         localRotations[sinX][sinY + 1] * worldRotation[axisRow + 3] +
                         localRotations[sinX][sinY + 2] * worldRotation[axisRow + 6]) /
                        0x1000;
                }
            }
        }

        player->collisionVolumes[0].point[0] = COLLISION_POINT(worldRotation[0], worldRotation[3], worldRotation[6], player->collisionSources[0].sizeX, player->collisionSources[0].sizeY - player->collisionCenterOffset, player->collisionSources[0].sizeZ);
        player->collisionVolumes[0].point[1] = COLLISION_POINT(worldRotation[1], worldRotation[4], worldRotation[7], player->collisionSources[0].sizeX, player->collisionSources[0].sizeY - player->collisionCenterOffset, player->collisionSources[0].sizeZ);
        player->collisionVolumes[0].point[2] = COLLISION_POINT(worldRotation[2], worldRotation[5], worldRotation[8], player->collisionSources[0].sizeX, player->collisionSources[0].sizeY - player->collisionCenterOffset, player->collisionSources[0].sizeZ);
        player->collisionVolumes[1].point[0] = COLLISION_POINT(player->collisionVolumes[0].axis[0], player->collisionVolumes[0].axis[3], player->collisionVolumes[0].axis[6], player->collisionSources[1].sizeX, player->collisionSources[1].sizeY, player->collisionSources[1].sizeZ);
        player->collisionVolumes[1].point[1] = COLLISION_POINT(player->collisionVolumes[0].axis[1], player->collisionVolumes[0].axis[4], player->collisionVolumes[0].axis[7], player->collisionSources[1].sizeX, player->collisionSources[1].sizeY, player->collisionSources[1].sizeZ);
        player->collisionVolumes[1].point[2] = COLLISION_POINT(player->collisionVolumes[0].axis[2], player->collisionVolumes[0].axis[5], player->collisionVolumes[0].axis[8], player->collisionSources[1].sizeX, player->collisionSources[1].sizeY, player->collisionSources[1].sizeZ);
    }

    player->collisionVolumes[0].point[0] += player->pos.x + centerOffset[0];
    player->collisionVolumes[0].point[1] +=
        player->pos.y - player->unk58 + player->unk64 + centerOffset[1] + 0xA000;
    player->collisionVolumes[0].point[2] += player->pos.z + centerOffset[2];
    player->collisionVolumes[1].point[0] += player->collisionVolumes[0].point[0];
    player->collisionVolumes[1].point[1] += player->collisionVolumes[0].point[1];
    player->collisionVolumes[1].point[2] += player->collisionVolumes[0].point[2];

    volumeIndex = 2;
    if (player->collisionVolumeCount >= 3) {
        volume = &player->collisionVolumes[2];
        source = &player->collisionSources[2];
        do {
            parent = &player->collisionVolumes[source->parentIndex];
            for (sinY = 0; sinY != 3; sinY++) {
                parentAxisY = parent->axis[sinY + 3];
                volume->point[sinY] =
                    COLLISION_POINT(parent->axis[sinY],
                                    parentAxisY,
                                    parent->axis[sinY + 6],
                                    source->sizeX,
                                    source->sizeY,
                                    source->sizeZ);
                volume->point[sinY] += parent->point[sinY];
            }
            volumeIndex++;
            volume++;
            source++;
        } while (volumeIndex < player->collisionVolumeCount);
    }

    for (volumeIndex = 0; volumeIndex < player->collisionVolumeCount; volumeIndex++) {
        player->collisionVolumes[volumeIndex].axis[0] /= 4;
        player->collisionVolumes[volumeIndex].axis[1] /= 4;
        player->collisionVolumes[volumeIndex].axis[2] /= 4;
        player->collisionVolumes[volumeIndex].axis[3] /= 4;
        player->collisionVolumes[volumeIndex].axis[4] /= 4;
        player->collisionVolumes[volumeIndex].axis[5] /= 4;
        player->collisionVolumes[volumeIndex].axis[6] /= 4;
        player->collisionVolumes[volumeIndex].axis[7] /= 4;
        player->collisionVolumes[volumeIndex].axis[8] /= 4;
    }
}
