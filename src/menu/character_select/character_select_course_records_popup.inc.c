void drawCharacterSelectCourseRecordsPopup(CharacterSelectCourseWidgetActor *actor) {
    s16 *selectedCourse;
    s32 assetHandle;
    s32 remainingScore;
    s32 pad[4];
    s32 digitOffset;
    s16 *courseOptions;
    s32 recordPalette;
    s32 rowIndex;
    u16 digits[2];
    u16 pad2;
    u8 rowPalette;
    s32 pad3;

    if (gRaceSplitscreenMode == 2) {
        courseOptions = gCharacterSelectCourseMenuData.courseOptionsByUnlock[gHighestUnlockedCourse];
    } else if (gRaceTypeSelection < 2) {
        courseOptions = gCharacterSelectCourseMenuData.shortCourseOptions;
    } else {
        courseOptions = gCharacterSelectCourseMenuData.singleCourseOption;
    }

    for (rowIndex = 0; rowIndex < 5; rowIndex++) {
        if (rowIndex < 3) {
            rowPalette = 6;
        } else {
            rowPalette = 4;
        }
        drawMenuSprite(
            actor->x,
            actor->y + (0x14 * rowIndex),
            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
            rowIndex + 0x77,
            0x20,
            0x20,
            0,
            0
        );
        if (gPlayerCount == 1) {
            if (gRaceSplitscreenMode != 2) {
                if (gRaceTypeSelection == 0) {
                    if (rowIndex < 3) {
                        rowPalette = 0xC;
                    } else {
                        rowPalette = 0xD;
                    }
                    drawCharacterSelectCourseRecordTime(
                        (
                            CharacterSelectCourseRecordTime *
                        )&gGameSaveDataBuffer->raceRecords[courseOptions[gRaceCourseIndex.signedValue]][rowIndex],
                        actor->x + 0x14,
                        actor->y + (0x14 * rowIndex),
                        rowPalette
                    );
                    drawMenuSprite(
                        actor->x + 0x54,
                        actor->y + (0x14 * rowIndex),
                        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                        (gGameSaveDataBuffer
                             ->raceRecordCharacterIds[courseOptions[gRaceCourseIndex.signedValue]][rowIndex] &
                         7) +
                            0x51,
                        0x20,
                        0x20,
                        0,
                        0
                    );
                    drawMenuSprite(
                        actor->x + 0x65,
                        actor->y + (0x14 * rowIndex),
                        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                        (gGameSaveDataBuffer
                             ->raceRecordCharacterIds[courseOptions[gRaceCourseIndex.signedValue]][rowIndex] >>
                         3) +
                            0x7C,
                        0x20,
                        0x20,
                        0,
                        0
                    );
                } else if (gRaceTypeSelection == 1) {
                    if (rowIndex < 3) {
                        recordPalette = 0xC;
                    } else {
                        recordPalette = 0xD;
                    }

                    digits[0] =
                        gGameSaveDataBuffer->scoreAttackScores[courseOptions[gRaceCourseIndex.signedValue]][rowIndex] /
                        10;
                    if (digits[0]) {
                        drawMenuSprite(
                            actor->x + 0x18,
                            actor->y + (0x14 * rowIndex),
                            getRelocatableHeapBlockBase((s32)gAssetHandles[0x1F]),
                            digits[0] + 0x2B,
                            0x20,
                            0x20,
                            0,
                            recordPalette + 1
                        );
                    }
                    digits[1] =
                        gGameSaveDataBuffer->scoreAttackScores[courseOptions[gRaceCourseIndex.signedValue]][rowIndex] %
                        10;

                    drawMenuSprite(
                        actor->x + 0x20,
                        actor->y + (0x14 * rowIndex),
                        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                        digits[1] + 0x2B,
                        0x20,
                        0x20,
                        0,
                        recordPalette + 1
                    );
                    drawMenuAsciiText(actor->x + 0x28, (actor->y + (0x14 * rowIndex)) + 8, "HIT", rowPalette, 0x100);
                    drawMenuSprite(
                        actor->x + 0x48,
                        actor->y + (0x14 * rowIndex),
                        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                        (gGameSaveDataBuffer
                             ->scoreAttackCharacterIds[courseOptions[gRaceCourseIndex.signedValue]][rowIndex] &
                         7) +
                            0x51,
                        0x20U,
                        0x20U,
                        0U,
                        0U
                    );
                    drawMenuSprite(
                        actor->x + 0x60,
                        actor->y + (0x14 * rowIndex),
                        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                        (gGameSaveDataBuffer
                             ->scoreAttackCharacterIds[courseOptions[gRaceCourseIndex.signedValue]][rowIndex] >>
                         3) +
                            0x7C,
                        0x20U,
                        0x20U,
                        0,
                        0
                    );
                } else {
                    if (rowIndex < 3) {
                        recordPalette = 0xC;
                    } else {
                        recordPalette = 0xD;
                    }

                    remainingScore = gGameSaveDataBuffer->trickAttackScores[7][rowIndex];
                    for (digitOffset = 0;;) {
                        drawMenuSprite(
                            (s16)((actor->x - digitOffset) + 0x38),
                            (s16)(actor->y + (0x14 * rowIndex)),
                            getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                            (remainingScore % 10) + 0x2B,
                            0x20U,
                            0x20U,
                            0U,
                            recordPalette + 1
                        );
                        digitOffset += 8;
                        remainingScore /= 10;
                        if (remainingScore == 0) {
                            break;
                        }
                    }
                    drawMenuAsciiText(actor->x + 0x40, (actor->y + (0x14 * rowIndex)) + 8, "P", rowPalette, 0x100U);
                    drawMenuSprite(
                        actor->x + 0x4C,
                        actor->y + (0x14 * rowIndex),
                        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                        (gGameSaveDataBuffer->trickAttackCharacterIds[7][rowIndex] & 7) + 0x51,
                        0x20U,
                        0x20U,
                        0,
                        0
                    );
                    drawMenuSprite(
                        actor->x + 0x60,
                        actor->y + (0x14 * rowIndex),
                        getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                        (gGameSaveDataBuffer->trickAttackCharacterIds[7][rowIndex] >> 3) + 0x7C,
                        0x20U,
                        0x20,
                        0,
                        0
                    );
                }
            } else {
                if (rowIndex < 3) {
                    rowPalette = 0xC;
                } else {
                    rowPalette = 0xD;
                }
                drawCharacterSelectCourseRecordTime(
                    (
                        CharacterSelectCourseRecordTime *
                    )&gGameSaveDataBuffer->timeTrialRecords[courseOptions[gRaceCourseIndex.signedValue]][rowIndex],
                    actor->x + 0x14,
                    actor->y + (0x14 * rowIndex),
                    rowPalette
                );
                drawMenuSprite(
                    actor->x + 0x54,
                    actor->y + (0x14 * rowIndex),
                    getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                    (gGameSaveDataBuffer->timeTrialCharacterIds[courseOptions[gRaceCourseIndex.signedValue]][rowIndex] &
                     7) +
                        0x51,
                    0x20U,
                    0x20,
                    0,
                    0
                );
                drawMenuSprite(
                    actor->x + 0x65,
                    actor->y + (0x14 * rowIndex),
                    getRelocatableHeapBlockBase(gAssetHandles[0x1F]),
                    (gGameSaveDataBuffer
                         ->timeTrialCharacterIds[courseOptions[gRaceCourseIndex.signedValue]][rowIndex] >>
                     3) +
                        0x7C,
                    0x20U,
                    0x20U,
                    0,
                    0
                );
            }
        }
    }
}
