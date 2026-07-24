#define FIXED_PRODUCT(a, b) ((s32)(a) * (b) / 0x1000)
#define COLLISION_POINT(x, y, z, sizeX, sizeY, sizeZ) \
    (((s64)(x) * (sizeX) + (s64)(y) * (sizeY) + (s64)(z) * (sizeZ)) / 0x1000)
#define MIRRORED_COLLISION_POINT(x, y, z, sizeX, sizeY, sizeZ) \
    ((-(s64)(x) * (sizeX) + (s64)(y) * (sizeY) + (s64)(z) * (sizeZ)) / 0x1000)

void resolveRacePlayerCollisionVolumes(RacePlayer *player) {
    s16 localAxes[14][16];
    FixedMatrix3s tempMtx;
    FixedMatrix3s playerMtx;
    s32 i;
    s32 j;
    s32 k;
    s32 sinX;
    s32 cosX;
    s32 offset[3];
    FixedMatrix3s effectMtx;
    FixedMatrix3s baseMtx;
    s32 sinY;
    s32 cosY;
    s32 sinZ;
    s32 cosZ;
    s32 temp;
    RacePlayerCollisionSource *source;
    RacePlayerCollisionVolume *volume;
    RacePlayerCollisionVolume *parent;
    RacePlayer *sourceView;
    RacePlayer *volumeView;
    RacePlayer *parentView;
    RacePlayer * volatile savedPlayer;
    RacePlayer *sourceIterator;
    s32 sourceStride;

    player->unk288 = player->pitchAngle;
    player->unk28A = player->facingAngle;
    player->unk28C = player->unk2EE;
    savedPlayer = player;

    if (player->stateFlags & 0x400) {
        sourceStride = sizeof(RacePlayerCollisionSource);
        sourceIterator = player;
        for (i = 0; i < player->collisionVolumeCount; i++, sourceIterator = (RacePlayer *)((u8 *)sourceIterator + sourceStride)) {
            sinX = fixedSine(((RacePlayer *)((u8 *)savedPlayer + sourceIterator->collisionSources->mirroredParentIndex * sourceStride))->collisionSources[0].rotX);
            cosX = fixedCosine(((RacePlayer *)((u8 *)savedPlayer + sourceIterator->collisionSources[0].mirroredParentIndex * sourceStride))->collisionSources[0].rotX);
            sinY = fixedSine(-((RacePlayer *)((u8 *)savedPlayer + sourceIterator->collisionSources[0].mirroredParentIndex * sourceStride))->collisionSources[0].rotY);
            cosY = fixedCosine(-((RacePlayer *)((u8 *)savedPlayer + sourceIterator->collisionSources->mirroredParentIndex * sourceStride))->collisionSources[0].rotY);
            sinZ = fixedSine(-((RacePlayer *)((u8 *)savedPlayer + sourceIterator->collisionSources[0].mirroredParentIndex * sourceStride))->collisionSources[0].rotZ);
            cosZ = fixedCosine(-((RacePlayer *)((u8 *)savedPlayer + sourceIterator->collisionSources[0].mirroredParentIndex * sourceStride))->collisionSources[0].rotZ);

            localAxes[i][0] = FIXED_PRODUCT(cosY, cosZ);
            localAxes[i][1] = FIXED_PRODUCT(cosY, sinZ);
            localAxes[i][2] = -sinY;
            temp = FIXED_PRODUCT(sinX, sinY);
            localAxes[i][3] = FIXED_PRODUCT(temp, cosZ) + FIXED_PRODUCT(cosX, -sinZ);
            localAxes[i][4] = FIXED_PRODUCT(temp, sinZ) + FIXED_PRODUCT(cosX, cosZ);
            localAxes[i][5] = FIXED_PRODUCT(sinX, cosY);
            temp = FIXED_PRODUCT(cosX, sinY);
            localAxes[i][6] = FIXED_PRODUCT(temp, cosZ) + FIXED_PRODUCT(sinX, sinZ);
            localAxes[i][7] = FIXED_PRODUCT(temp, sinZ) + FIXED_PRODUCT(-sinX, cosZ);
            localAxes[i][8] = FIXED_PRODUCT(cosX, cosY);

            if (player->stateFlags & 0x200000) {
                localAxes[i][1] = localAxes[i][1] * player->unk2D6 / 8;
                localAxes[i][4] = localAxes[i][4] * player->unk2D6 / 8;
                localAxes[i][7] = localAxes[i][7] * player->unk2D6 / 8;
            }
        }

        makeFixedRotationZYX(effectMtx, player->unk6C, -player->unk6E, -player->unk70);
        makeFixedRotationZXY(playerMtx, player->unk288, player->unk28A, player->unk28C);
        offset[0] = (s64)playerMtx[3] * player->unk68 / 0x1000;
        offset[1] = (s64)playerMtx[4] * player->unk68 / 0x1000;
        offset[2] = (s64)playerMtx[5] * player->unk68 / 0x1000;
        multiplyFixedMatrix3s(effectMtx, playerMtx, baseMtx);

        for (i = 0; i < player->collisionVolumeCount; i++) {
            for (j = 0; j != 9; j += 3) {
                for (k = 0; k != 3; k++) {
                    player->collisionVolumes[i].axis[j + k] =
                        (baseMtx[k + 6] * localAxes[i][j + 2] + baseMtx[k] * localAxes[i][j] +
                         baseMtx[k + 3] * localAxes[i][j + 1]) /
                        0x1000;
                }
            }
        }

        player->collisionVolumes[0].point[0] = MIRRORED_COLLISION_POINT(baseMtx[0], baseMtx[3], baseMtx[6], player->collisionSources[0].sizeX, player->collisionSources[0].sizeY - player->unk68, player->collisionSources[0].sizeZ);
        player->collisionVolumes[0].point[1] = MIRRORED_COLLISION_POINT(baseMtx[1], baseMtx[4], baseMtx[7], player->collisionSources[0].sizeX, player->collisionSources[0].sizeY - player->unk68, player->collisionSources[0].sizeZ);
        player->collisionVolumes[0].point[2] = MIRRORED_COLLISION_POINT(baseMtx[2], baseMtx[5], baseMtx[8], player->collisionSources[0].sizeX, player->collisionSources[0].sizeY - player->unk68, player->collisionSources[0].sizeZ);
        player->collisionVolumes[1].point[0] = MIRRORED_COLLISION_POINT(player->collisionVolumes[0].axis[0], player->collisionVolumes[0].axis[3], player->collisionVolumes[0].axis[6], player->collisionSources[1].sizeX, player->collisionSources[1].sizeY, player->collisionSources[1].sizeZ);
        player->collisionVolumes[1].point[1] = MIRRORED_COLLISION_POINT(player->collisionVolumes[0].axis[1], player->collisionVolumes[0].axis[4], player->collisionVolumes[0].axis[7], player->collisionSources[1].sizeX, player->collisionSources[1].sizeY, player->collisionSources[1].sizeZ);
        player->collisionVolumes[1].point[2] = MIRRORED_COLLISION_POINT(player->collisionVolumes[0].axis[2], player->collisionVolumes[0].axis[5], player->collisionVolumes[0].axis[8], player->collisionSources[1].sizeX, player->collisionSources[1].sizeY, player->collisionSources[1].sizeZ);
    } else {
        sourceStride = sizeof(RacePlayerCollisionSource);
        sourceIterator = player;
        for (i = 0; i < player->collisionVolumeCount; i++, sourceIterator = (RacePlayer *)((u8 *)sourceIterator + sourceStride)) {
            sinX = fixedSine(sourceIterator->collisionSources[0].rotX);
            cosX = fixedCosine(sourceIterator->collisionSources[0].rotX);
            sinY = fixedSine(sourceIterator->collisionSources[0].rotY);
            cosY = fixedCosine(sourceIterator->collisionSources[0].rotY);
            sinZ = fixedSine(sourceIterator->collisionSources[0].rotZ);
            cosZ = fixedCosine(sourceIterator->collisionSources[0].rotZ);

            localAxes[i][0] = FIXED_PRODUCT(cosY, cosZ);
            localAxes[i][1] = FIXED_PRODUCT(cosY, sinZ);
            localAxes[i][2] = -sinY;
            temp = FIXED_PRODUCT(sinX, sinY);
            localAxes[i][3] = FIXED_PRODUCT(temp, cosZ) + FIXED_PRODUCT(cosX, -sinZ);
            localAxes[i][4] = FIXED_PRODUCT(temp, sinZ) + FIXED_PRODUCT(cosX, cosZ);
            localAxes[i][5] = FIXED_PRODUCT(sinX, cosY);
            temp = FIXED_PRODUCT(cosX, sinY);
            localAxes[i][6] = FIXED_PRODUCT(temp, cosZ) + FIXED_PRODUCT(sinX, sinZ);
            localAxes[i][7] = FIXED_PRODUCT(temp, sinZ) + FIXED_PRODUCT(-sinX, cosZ);
            localAxes[i][8] = FIXED_PRODUCT(cosX, cosY);

            if (player->stateFlags & 0x200000) {
                localAxes[i][1] = localAxes[i][1] * player->unk2D6 / 8;
                localAxes[i][4] = localAxes[i][4] * player->unk2D6 / 8;
                localAxes[i][7] = localAxes[i][7] * player->unk2D6 / 8;
            }
        }

        makeFixedRotationZYX(effectMtx, player->unk6C, player->unk6E, player->unk70);
        makeFixedRotationZXY(playerMtx, player->unk288, player->unk28A, player->unk28C);
        offset[0] = (s64)playerMtx[3] * player->unk68 / 0x1000;
        offset[1] = (s64)playerMtx[4] * player->unk68 / 0x1000;
        offset[2] = (s64)playerMtx[5] * player->unk68 / 0x1000;
        makeFixedRotationY(baseMtx, 0x800);
        multiplyFixedMatrix3s(baseMtx, playerMtx, tempMtx);
        multiplyFixedMatrix3s(effectMtx, tempMtx, baseMtx);

        for (i = 0; i < player->collisionVolumeCount; i++) {
            for (j = 0; j != 9; j += 3) {
                for (k = 0; k != 3; k++) {
                    player->collisionVolumes[i].axis[j + k] =
                        (baseMtx[k + 6] * localAxes[i][j + 2] + baseMtx[k] * localAxes[i][j] +
                         baseMtx[k + 3] * localAxes[i][j + 1]) /
                        0x1000;
                }
            }
        }

        player->collisionVolumes[0].point[0] = COLLISION_POINT(baseMtx[0], baseMtx[3], baseMtx[6], player->collisionSources[0].sizeX, player->collisionSources[0].sizeY - player->unk68, player->collisionSources[0].sizeZ);
        player->collisionVolumes[0].point[1] = COLLISION_POINT(baseMtx[1], baseMtx[4], baseMtx[7], player->collisionSources[0].sizeX, player->collisionSources[0].sizeY - player->unk68, player->collisionSources[0].sizeZ);
        player->collisionVolumes[0].point[2] = COLLISION_POINT(baseMtx[2], baseMtx[5], baseMtx[8], player->collisionSources[0].sizeX, player->collisionSources[0].sizeY - player->unk68, player->collisionSources[0].sizeZ);
        player->collisionVolumes[1].point[0] = COLLISION_POINT(player->collisionVolumes[0].axis[0], player->collisionVolumes[0].axis[3], player->collisionVolumes[0].axis[6], player->collisionSources[1].sizeX, player->collisionSources[1].sizeY, player->collisionSources[1].sizeZ);
        player->collisionVolumes[1].point[1] = COLLISION_POINT(player->collisionVolumes[0].axis[1], player->collisionVolumes[0].axis[4], player->collisionVolumes[0].axis[7], player->collisionSources[1].sizeX, player->collisionSources[1].sizeY, player->collisionSources[1].sizeZ);
        player->collisionVolumes[1].point[2] = COLLISION_POINT(player->collisionVolumes[0].axis[2], player->collisionVolumes[0].axis[5], player->collisionVolumes[0].axis[8], player->collisionSources[1].sizeX, player->collisionSources[1].sizeY, player->collisionSources[1].sizeZ);
    }

    player->collisionVolumes[0].point[0] += player->posX + offset[0];
    player->collisionVolumes[0].point[1] += player->posY - player->unk58 + player->unk64 + offset[1] + 0xA000;
    player->collisionVolumes[0].point[2] += player->posZ + offset[2];
    player->collisionVolumes[1].point[0] += player->collisionVolumes[0].point[0];
    player->collisionVolumes[1].point[1] += player->collisionVolumes[0].point[1];
    player->collisionVolumes[1].point[2] += player->collisionVolumes[0].point[2];

    i = 2;
    if (player->collisionVolumeCount >= 3) {
        volumeView = (RacePlayer *)((u8 *)player + 0x40);
        sourceView = (RacePlayer *)((u8 *)player + 0x28);
        do {
            parentView = (RacePlayer *)((u8 *)player + (sourceView->collisionSources[0].parentIndex << 5));
            for (j = 0; j != 3; j++) {
                volumeView->collisionVolumes[0].point[j] =
                    COLLISION_POINT(parentView->collisionVolumes[0].axis[j],
                                    parentView->collisionVolumes[0].axis[j + 3],
                                    parentView->collisionVolumes[0].axis[j + 6],
                                    sourceView->collisionSources[0].sizeX,
                                    sourceView->collisionSources[0].sizeY,
                                    sourceView->collisionSources[0].sizeZ);
                volumeView->collisionVolumes[0].point[j] += parentView->collisionVolumes[0].point[j];
            }
            i++;
            volumeView = (RacePlayer *)((u8 *)volumeView + sizeof(RacePlayerCollisionVolume));
            sourceView = (RacePlayer *)((u8 *)sourceView + sizeof(RacePlayerCollisionSource));
        } while (i < player->collisionVolumeCount);
    }

    for (i = 0; i < player->collisionVolumeCount; i++) {
        player->collisionVolumes[i].axis[0] /= 4;
        player->collisionVolumes[i].axis[1] /= 4;
        player->collisionVolumes[i].axis[2] /= 4;
        player->collisionVolumes[i].axis[3] /= 4;
        player->collisionVolumes[i].axis[4] /= 4;
        player->collisionVolumes[i].axis[5] /= 4;
        player->collisionVolumes[i].axis[6] /= 4;
        player->collisionVolumes[i].axis[7] /= 4;
        player->collisionVolumes[i].axis[8] /= 4;
    }
}
