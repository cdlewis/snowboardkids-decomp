void resolveRacePlayerCollisionVolumes(RacePlayer *player)
{
  RacePlayerCollisionSource *source;
  RacePlayerCollisionVolume *volumeView;
  RacePlayerCollisionVolume *parentView;
  RacePlayerCollisionSource *sourceView;
  s16 localAxes[14][0x10];
  s32 i;
  s32 j;
  s32 k;
  Mat3x3 tempMtx;
  s32 sinX;
  s32 cosX;
  s32 sinY;
  Mat3x3 playerMtx;
  s32 cosY;
  s32 sinZ;
  s32 cosZ;
  Mat3x3 baseMtx;
  s32 offset[3];
  Mat3x3 effectMtx;
  s16 parentAxisY;
  s32 temp;
  RacePlayerCollisionSource *sourceIterator;
  s32 sourceStride;
  s32 sourceIndex;
  player->unk288 = player->pitchAngle;
  player->unk28A = player->facingAngle;
  player->unk28C = player->unk2EE;
  if (player->stateFlags & 0x400)
  {
    sourceStride = sizeof(RacePlayerCollisionSource);
    for (i = 0, sourceIndex = 0; i < player->collisionVolumeCount; i++, sourceIndex++)
    {
      sinX = fixedSine(player->collisionSources[player->collisionSources[i].mirroredParentIndex].rotX);
      cosX = fixedCosine(player->collisionSources[player->collisionSources[i].mirroredParentIndex].rotX);
      sinY = fixedSine(-player->collisionSources[player->collisionSources[i].mirroredParentIndex].rotY);
      cosY = fixedCosine(-player->collisionSources[player->collisionSources[i].mirroredParentIndex].rotY);
      sinZ = fixedSine(-player->collisionSources[player->collisionSources[i].mirroredParentIndex].rotZ);
      cosZ = fixedCosine(-player->collisionSources[player->collisionSources[i].mirroredParentIndex].rotZ);
      localAxes[sourceIndex][0] = (((s32) cosY) * cosZ) / 0x1000;
      localAxes[sourceIndex][1] = (((s32) cosY) * sinZ) / 0x1000;
      localAxes[sourceIndex][2] = -sinY;
      temp = (((s32) sinX) * sinY) / 0x1000;
      localAxes[sourceIndex][3] = ((((s32) temp) * cosZ) / 0x1000) + ((((s32) cosX) * (-sinZ)) / 0x1000);
      localAxes[sourceIndex][4] = ((((s32) temp) * sinZ) / 0x1000) + ((((s32) cosX) * cosZ) / 0x1000);
      localAxes[sourceIndex][5] = (((s32) sinX) * cosY) / 0x1000;
      temp = (((s32) cosX) * sinY) / 0x1000;
      localAxes[sourceIndex][6] = ((((s32) temp) * cosZ) / 0x1000) + ((((s32) sinX) * sinZ) / 0x1000);
      localAxes[sourceIndex][7] = ((((s32) temp) * sinZ) / 0x1000) + ((((s32) (-sinX)) * cosZ) / 0x1000);
      localAxes[sourceIndex][8] = (((s32) cosX) * cosY) / 0x1000;
      if (player->stateFlags & 0x200000)
      {
        localAxes[sourceIndex][1] = (localAxes[sourceIndex][1] * player->unk2D6) / 8;
        localAxes[sourceIndex][4] = (localAxes[sourceIndex][4] * player->unk2D6) / 8;
        localAxes[sourceIndex][7] = (localAxes[sourceIndex][7] * player->unk2D6) / 8;
      }
    }

    makeFixedRotationZYX(effectMtx, player->unk6C, -player->unk6E, -player->unk70);
    makeFixedRotationZXY(playerMtx, player->unk288, player->unk28A, player->unk28C);
    offset[0] = (((s64) playerMtx[3]) * player->collisionCenterOffset) / 0x1000;
    offset[1] = (((s64) playerMtx[4]) * player->collisionCenterOffset) / 0x1000;
    offset[2] = (((s64) playerMtx[5]) * player->collisionCenterOffset) / 0x1000;
    multiplyFixedMatrix3s(effectMtx, playerMtx, baseMtx);
    for (sinX = 0; sinX < player->collisionVolumeCount; sinX++)
    {
      sinY = (i = 0);
matrix_row_mirrored:
      for (j = 0; j != 3; j++)
      {
        if (1)
        {
          player->collisionVolumes[sinX].axis[i + j] = (((localAxes[sinX][sinY] * baseMtx[j]) + (localAxes[sinX][sinY + 1] * baseMtx[j + 3])) + (localAxes[sinX][sinY + 2] * baseMtx[j + 6])) / 0x1000;
        }
      }
      i += 3;
      sinY += 3;
      if (sinY != 9)
      {
        goto matrix_row_mirrored;
      }
    }

    player->collisionVolumes[0].point[0] = ((((-((s64) baseMtx[0])) * player->collisionSources[0].sizeX) + (((s64) baseMtx[3]) * (player->collisionSources[0].sizeY - player->collisionCenterOffset))) + (((s64) baseMtx[6]) * player->collisionSources[0].sizeZ)) / 0x1000;
    player->collisionVolumes[0].point[1] = ((((-((s64) baseMtx[1])) * player->collisionSources[0].sizeX) + (((s64) baseMtx[4]) * (player->collisionSources[0].sizeY - player->collisionCenterOffset))) + (((s64) baseMtx[7]) * player->collisionSources[0].sizeZ)) / 0x1000;
    player->collisionVolumes[0].point[2] = ((((-((s64) baseMtx[2])) * player->collisionSources[0].sizeX) + (((s64) baseMtx[5]) * (player->collisionSources[0].sizeY - player->collisionCenterOffset))) + (((s64) baseMtx[8]) * player->collisionSources[0].sizeZ)) / 0x1000;
    player->collisionVolumes[1].point[0] = ((((-((s64) player->collisionVolumes[0].axis[0])) * player->collisionSources[1].sizeX) + (((s64) player->collisionVolumes[0].axis[3]) * player->collisionSources[1].sizeY)) + (((s64) player->collisionVolumes[0].axis[6]) * player->collisionSources[1].sizeZ)) / 0x1000;
    player->collisionVolumes[1].point[1] = ((((-((s64) player->collisionVolumes[0].axis[1])) * player->collisionSources[1].sizeX) + (((s64) player->collisionVolumes[0].axis[4]) * player->collisionSources[1].sizeY)) + (((s64) player->collisionVolumes[0].axis[7]) * player->collisionSources[1].sizeZ)) / 0x1000;
    player->collisionVolumes[1].point[2] = ((((-((s64) player->collisionVolumes[0].axis[2])) * player->collisionSources[1].sizeX) + (((s64) player->collisionVolumes[0].axis[5]) * player->collisionSources[1].sizeY)) + (((s64) player->collisionVolumes[0].axis[8]) * player->collisionSources[1].sizeZ)) / 0x1000;
  }
  else
  {
    sourceStride = sizeof(RacePlayerCollisionSource);
    for (i = 0, sourceIndex = 0; i < player->collisionVolumeCount; i++, sourceIndex++)
    {
      sinX = fixedSine(player->collisionSources[i].rotX);
      cosX = fixedCosine(player->collisionSources[i].rotX);
      sinY = fixedSine(player->collisionSources[i].rotY);
      cosY = fixedCosine(player->collisionSources[i].rotY);
      sinZ = fixedSine(player->collisionSources[i].rotZ);
      cosZ = fixedCosine(player->collisionSources[i].rotZ);
      localAxes[sourceIndex][0] = (((s32) cosY) * cosZ) / 0x1000;
      localAxes[sourceIndex][1] = (((s32) cosY) * sinZ) / 0x1000;
      localAxes[sourceIndex][2] = -sinY;
      temp = (((s32) sinX) * sinY) / 0x1000;
      localAxes[sourceIndex][3] = ((((s32) temp) * cosZ) / 0x1000) + ((((s32) cosX) * (-sinZ)) / 0x1000);
      localAxes[sourceIndex][4] = ((((s32) temp) * sinZ) / 0x1000) + ((((s32) cosX) * cosZ) / 0x1000);
      localAxes[sourceIndex][5] = (((s32) sinX) * cosY) / 0x1000;
      temp = (((s32) cosX) * sinY) / 0x1000;
      localAxes[sourceIndex][6] = ((((s32) temp) * cosZ) / 0x1000) + ((((s32) sinX) * sinZ) / 0x1000);
      localAxes[sourceIndex][7] = ((((s32) temp) * sinZ) / 0x1000) + ((((s32) (-sinX)) * cosZ) / 0x1000);
      localAxes[sourceIndex][8] = (((s32) cosX) * cosY) / 0x1000;
      if (player->stateFlags & 0x200000)
      {
        localAxes[sourceIndex][1] = (localAxes[sourceIndex][1] * player->unk2D6) / 8;
        localAxes[sourceIndex][4] = (localAxes[sourceIndex][4] * player->unk2D6) / 8;
        localAxes[sourceIndex][7] = (localAxes[sourceIndex][7] * player->unk2D6) / 8;
      }
    }

    makeFixedRotationZYX(effectMtx, player->unk6C, player->unk6E, player->unk70);
    makeFixedRotationZXY(playerMtx, player->unk288, player->unk28A, player->unk28C);
    offset[0] = (((s64) playerMtx[3]) * player->collisionCenterOffset) / 0x1000;
    offset[1] = (((s64) playerMtx[4]) * player->collisionCenterOffset) / 0x1000;
    offset[2] = (((s64) playerMtx[5]) * player->collisionCenterOffset) / 0x1000;
    makeFixedRotationY(baseMtx, 0x800);
    multiplyFixedMatrix3s(baseMtx, playerMtx, tempMtx);
    multiplyFixedMatrix3s(effectMtx, tempMtx, baseMtx);
    for (sinX = 0; sinX < player->collisionVolumeCount; sinX++)
    {
      sinY = (i = 0);
matrix_row_normal:
      for (j = 0; j != 3; j++)
      {
        if (1)
        {
          player->collisionVolumes[sinX].axis[i + j] = (((localAxes[sinX][sinY] * baseMtx[j]) + (localAxes[sinX][sinY + 1] * baseMtx[j + 3])) + (localAxes[sinX][sinY + 2] * baseMtx[j + 6])) / 0x1000;
        }
      }
      i += 3;
      sinY += 3;
      if (sinY != 9)
      {
        goto matrix_row_normal;
      }
    }

    player->collisionVolumes[0].point[0] = (((((s64) baseMtx[0]) * player->collisionSources[0].sizeX) + (((s64) baseMtx[3]) * (player->collisionSources[0].sizeY - player->collisionCenterOffset))) + (((s64) baseMtx[6]) * player->collisionSources[0].sizeZ)) / 0x1000;
    player->collisionVolumes[0].point[1] = (((((s64) baseMtx[1]) * player->collisionSources[0].sizeX) + (((s64) baseMtx[4]) * (player->collisionSources[0].sizeY - player->collisionCenterOffset))) + (((s64) baseMtx[7]) * player->collisionSources[0].sizeZ)) / 0x1000;
    player->collisionVolumes[0].point[2] = (((((s64) baseMtx[2]) * player->collisionSources[0].sizeX) + (((s64) baseMtx[5]) * (player->collisionSources[0].sizeY - player->collisionCenterOffset))) + (((s64) baseMtx[8]) * player->collisionSources[0].sizeZ)) / 0x1000;
    player->collisionVolumes[1].point[0] = (((((s64) player->collisionVolumes[0].axis[0]) * player->collisionSources[1].sizeX) + (((s64) player->collisionVolumes[0].axis[3]) * player->collisionSources[1].sizeY)) + (((s64) player->collisionVolumes[0].axis[6]) * player->collisionSources[1].sizeZ)) / 0x1000;
    player->collisionVolumes[1].point[1] = (((((s64) player->collisionVolumes[0].axis[1]) * player->collisionSources[1].sizeX) + (((s64) player->collisionVolumes[0].axis[4]) * player->collisionSources[1].sizeY)) + (((s64) player->collisionVolumes[0].axis[7]) * player->collisionSources[1].sizeZ)) / 0x1000;
    player->collisionVolumes[1].point[2] = (((((s64) player->collisionVolumes[0].axis[2]) * player->collisionSources[1].sizeX) + (((s64) player->collisionVolumes[0].axis[5]) * player->collisionSources[1].sizeY)) + (((s64) player->collisionVolumes[0].axis[8]) * player->collisionSources[1].sizeZ)) / 0x1000;
  }
  player->collisionVolumes[0].point[0] += player->pos.x + offset[0];
  player->collisionVolumes[0].point[1] += (((player->pos.y - player->unk58) + player->unk64) + offset[1]) + 0xA000;
  player->collisionVolumes[0].point[2] += player->pos.z + offset[2];
  player->collisionVolumes[1].point[0] += player->collisionVolumes[0].point[0];
  player->collisionVolumes[1].point[1] += player->collisionVolumes[0].point[1];
  player->collisionVolumes[1].point[2] += player->collisionVolumes[0].point[2];
  for (i = 2; i < player->collisionVolumeCount; i++)
  {
      k = player->collisionSources[i].parentIndex;
      for (sinY = 0; sinY != 3; sinY++)
      {
        parentAxisY = player->collisionVolumes[k].axis[sinY + 3];
        player->collisionVolumes[i].point[sinY] = (((((s64) player->collisionVolumes[k].axis[sinY]) * player->collisionSources[i].sizeX) + (((s64) parentAxisY) * player->collisionSources[i].sizeY)) + (((s64) player->collisionVolumes[k].axis[sinY + 6]) * player->collisionSources[i].sizeZ)) / 0x1000;
        player->collisionVolumes[i].point[sinY] += player->collisionVolumes[k].point[sinY];
      }
  }
  for (i = 0; i < player->collisionVolumeCount; i++)
  {
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
