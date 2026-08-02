#ifdef NON_MATCHING

typedef struct DrawCharacterSelectCourseRecordsPopupStack {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u8 *trickIconRowBase;
    /* 0x08 */ u8 pad8[0x15];
    /* 0x1D */ u8 textColor;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ u16 quotient;
    /* 0x22 */ u16 remainder;
    /* 0x24 */ u8 pad24[8];
    /* 0x2C */ s16 *courseIds;
    /* 0x30 */ u8 pad30[0x20];
} DrawCharacterSelectCourseRecordsPopupStack;

typedef struct CharacterSelectTrickIconRowView {
    /* 0x0000 */ u8 pad0[0x77E7];
    /* 0x77E7 */ u8 characterBoardId;
} CharacterSelectTrickIconRowView;

extern u8 D_800E0B90[];
extern u8 D_800E0B94[];
extern void drawMenuSpriteWideIndex(s16 x, s16 y, void *texture, s32 tileIndex, u16 width, u16 height,
                                    u8 palette, u8 flip);

#define RECORD_POPUP_ASSET_HANDLES ((volatile s16 *)gAssetHandles)
#define RECORD_POPUP_SAVE_BYTES ((u8 *)&gGameSaveDataBuffer[0])

void drawCharacterSelectCourseRecordsPopup(CharacterSelectCourseWidgetActor *arg0) {
    volatile DrawCharacterSelectCourseRecordsPopupStack stack;
    s16 *selectedCoursePtr;
    s32 newQuotient;
    s32 digitOffset;
    s32 recordColor;
    s32 yOffset;
    s32 rowIndex;
    s32 trickScore;
    u8 secondaryColor;

    if (gRaceSplitscreenMode == 2) {
        stack.courseIds = gCharacterSelectCourseOptionsByUnlock[gHighestUnlockedCourse];
    } else if ((s32)gRaceTypeSelection < 2) {
        stack.courseIds = gCharacterSelectShortCourseOptions;
    } else {
        stack.courseIds = gCharacterSelectSingleCourseOption;
    }

    if (RECORD_POPUP_ASSET_HANDLES) {}

    rowIndex = 0;
    yOffset = 0;
    do {
        if (rowIndex < 3) {
            stack.textColor = 6;
        } else {
            stack.textColor = 4;
        }

        drawMenuSpriteWideIndex(arg0->x, (s16)(arg0->y + yOffset),
                                getRelocatableHeapBlockBase(RECORD_POPUP_ASSET_HANDLES[0x1F]),
                                (rowIndex + 0x77) & 0xFFFF, 0x20U, 0x20U, 0U, 0U);

        if (gPlayerCount == 1) {
            if (gRaceSplitscreenMode != 2) {
                if (gRaceTypeSelection == 0) {
                    if (rowIndex < 3) {
                        stack.textColor = 0xC;
                    } else {
                        stack.textColor = 0xD;
                    }

                    drawCharacterSelectCourseRecordTime(
                        (CharacterSelectCourseRecordTime *)&gGameSaveDataBuffer[0]
                            .raceRecords[stack.courseIds[gRaceCourseIndex.signedValue]][rowIndex],
                        arg0->x + 0x14, arg0->y + yOffset, (s32)stack.textColor);
                    drawMenuSpriteWideIndex(
                        (s16)(arg0->x + 0x54), (s16)(arg0->y + yOffset),
                        getRelocatableHeapBlockBase(RECORD_POPUP_ASSET_HANDLES[0x1F]),
                        ((gGameSaveDataBuffer->raceRecordCharacterIds[0]
                              [(stack.courseIds[gRaceCourseIndex.signedValue] * 5) + rowIndex] &
                          7) +
                         0x51) &
                            0xFFFF,
                        0x20U, 0x20U, 0U, 0U);
                    drawMenuSpriteWideIndex(
                        (s16)(arg0->x + 0x65), (s16)(arg0->y + yOffset),
                        getRelocatableHeapBlockBase(RECORD_POPUP_ASSET_HANDLES[0x1F]),
                        (((s32)gGameSaveDataBuffer->raceRecordCharacterIds[0]
                               [(stack.courseIds[gRaceCourseIndex.signedValue] * 5) + rowIndex] >>
                           3) +
                         0x7C) &
                            0xFFFF,
                        0x20U, 0x20U, 0U, 0U);
                } else {
                    if (gRaceTypeSelection == 1) {
                        if (rowIndex < 3) {
                            recordColor = 0xC;
                        } else {
                            recordColor = 0xD;
                        }

                        selectedCoursePtr = &(0, stack.courseIds)[gRaceCourseIndex.signedValue];
                        newQuotient = (s32)gGameSaveDataBuffer->scoreAttackScores[0]
                                          [(*selectedCoursePtr * 5) + rowIndex] /
                                      10;
                        stack.quotient = (u16)newQuotient;
                        if ((newQuotient & 0xFFFF) & 0xFFFFU) {
                            drawMenuSpriteWideIndex(
                                (s16)(arg0->x + 0x18), (s16)(arg0->y + yOffset),
                                getRelocatableHeapBlockBase(RECORD_POPUP_ASSET_HANDLES[0x1F]),
                                (stack.quotient + 0x2B) & 0xFFFF, 0x20U, 0x20U, 0U,
                                (u8)(recordColor + 1));
                            selectedCoursePtr = &(0, stack.courseIds)[gRaceCourseIndex.signedValue];
                        }

                        stack.remainder = (u16)((s32)gGameSaveDataBuffer->scoreAttackScores[0]
                                                        [(*selectedCoursePtr * 5) + rowIndex] %
                                                10);
                        secondaryColor = recordColor + 1;
                        drawMenuSpriteWideIndex(
                            (s16)(arg0->x + 0x20), (s16)(arg0->y + yOffset),
                            getRelocatableHeapBlockBase(RECORD_POPUP_ASSET_HANDLES[0x1F]),
                            (stack.remainder + 0x2B) & 0xFFFF, 0x20U, 0x20U, 0U, secondaryColor);
                        drawMenuAsciiText((s16)(arg0->x + 0x28), (s16)(arg0->y + yOffset + 8), D_800E0B90,
                                          (u16)stack.textColor, 0x100U);
                        drawMenuSpriteWideIndex(
                            (s16)(arg0->x + 0x48), (s16)(arg0->y + yOffset),
                            getRelocatableHeapBlockBase(RECORD_POPUP_ASSET_HANDLES[0x1F]),
                            ((gGameSaveDataBuffer->scoreAttackCharacterIds[0]
                                  [(stack.courseIds[gRaceCourseIndex.signedValue] * 5) + rowIndex] &
                              7) +
                             0x51) &
                                0xFFFF,
                            0x20U, 0x20U, 0U, 0U);
                        drawMenuSpriteWideIndex(
                            (s16)(arg0->x + 0x60), (s16)(arg0->y + yOffset),
                            getRelocatableHeapBlockBase(RECORD_POPUP_ASSET_HANDLES[0x1F]),
                            (((s32)gGameSaveDataBuffer->scoreAttackCharacterIds[0]
                                   [(stack.courseIds[gRaceCourseIndex.signedValue] * 5) + rowIndex] >>
                               3) +
                             0x7C) &
                                0xFFFF,
                            0x20U, 0x20U, 0U, 0U);
                    } else {
                        if (1) {
                            stack.trickIconRowBase = &RECORD_POPUP_SAVE_BYTES[rowIndex];
                            if (rowIndex < 3) {
                                recordColor = 0xC;
                            } else {
                                recordColor = 0xD;
                            }

                            secondaryColor = recordColor + 1;
                            trickScore = gGameSaveDataBuffer->trickAttackScores[7][rowIndex];
                            digitOffset = 0;
                            do {
                                drawMenuSpriteWideIndex(
                                    (s16)((arg0->x - digitOffset) + 0x38), (s16)(arg0->y + yOffset),
                                    getRelocatableHeapBlockBase(RECORD_POPUP_ASSET_HANDLES[0x1F]),
                                    (((s32)trickScore % 10) + 0x2B) & 0xFFFF, 0x20U, 0x20U, 0U,
                                    secondaryColor);
                                digitOffset += 8;
                                trickScore /= 10;
                            } while (trickScore != 0);
                        }

                        drawMenuAsciiText((s16)(arg0->x + 0x40), (s16)(arg0->y + yOffset + 8), D_800E0B94,
                                          (u16)stack.textColor, 0x100U);
                        drawMenuSpriteWideIndex(
                            (s16)(arg0->x + 0x4C), (s16)(arg0->y + yOffset),
                            getRelocatableHeapBlockBase(RECORD_POPUP_ASSET_HANDLES[0x1F]),
                            ((((CharacterSelectTrickIconRowView *)stack.trickIconRowBase)->characterBoardId & 7) +
                             0x51) &
                                0xFFFF,
                            0x20U, 0x20U, 0U, 0U);
                        drawMenuSpriteWideIndex(
                            (s16)(arg0->x + 0x60), (s16)(arg0->y + yOffset),
                            getRelocatableHeapBlockBase(RECORD_POPUP_ASSET_HANDLES[0x1F]),
                            (((s32)((CharacterSelectTrickIconRowView *)stack.trickIconRowBase)->characterBoardId >>
                              3) +
                             0x7C) &
                                0xFFFF,
                            0x20U, 0x20U, 0U, 0U);
                    }
                }
            } else {
                if (rowIndex < 3) {
                    stack.textColor = 0xC;
                } else {
                    stack.textColor = 0xD;
                }

                drawCharacterSelectCourseRecordTime(
                    (CharacterSelectCourseRecordTime *)&gGameSaveDataBuffer[0]
                        .timeTrialRecords[stack.courseIds[gRaceCourseIndex.signedValue]][rowIndex],
                    arg0->x + 0x14, arg0->y + yOffset, (s32)stack.textColor);
                drawMenuSpriteWideIndex(
                    (s16)(arg0->x + 0x54), (s16)(arg0->y + yOffset),
                    getRelocatableHeapBlockBase(RECORD_POPUP_ASSET_HANDLES[0x1F]),
                    ((gGameSaveDataBuffer->timeTrialCharacterIds[0]
                          [(stack.courseIds[gRaceCourseIndex.signedValue] * 5) + rowIndex] &
                      7) +
                     0x51) &
                        0xFFFF,
                    0x20U, 0x20U, 0U, 0U);
                drawMenuSpriteWideIndex(
                    (s16)(arg0->x + 0x65), (s16)(arg0->y + yOffset),
                    getRelocatableHeapBlockBase(RECORD_POPUP_ASSET_HANDLES[0x1F]),
                    (((s32)gGameSaveDataBuffer->timeTrialCharacterIds[0]
                           [(stack.courseIds[gRaceCourseIndex.signedValue] * 5) + rowIndex] >>
                       3) +
                     0x7C) &
                        0xFFFF,
                    0x20U, 0x20U, 0U, 0U);
            }
        }

        rowIndex += 1;
        yOffset += 0x14;
    } while (rowIndex != 5);
}

#undef RECORD_POPUP_SAVE_BYTES
#undef RECORD_POPUP_ASSET_HANDLES

#endif
