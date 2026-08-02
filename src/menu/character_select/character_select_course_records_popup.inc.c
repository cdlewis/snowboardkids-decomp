typedef struct CharacterSelectCourseRecordsPopupSaveDataCandidate {
  u8 pad0[0x4E];
  CharacterSelectCourseRecordTime timeTrialRecords[11][5];
  u8 pad12A[0x156 - 0x12A];
  CharacterSelectCourseRecordTime raceRecords[11][5];
  u8 pad232[0x7756 - 0x232];
  u16 trickAttackScores[11][5];
  u8 trickAttackCharacterIds[11][5];
  u8 timeTrialCharacterIds[11][5];
  u8 scoreAttackScores[11][5];
  u8 scoreAttackCharacterIds[11][5];
  u8 raceRecordCharacterIds[11][5];
} CharacterSelectCourseRecordsPopupSaveDataCandidate;

typedef union CharacterSelectCourseRecordsPopupSaveBufferCandidate {
  CharacterSelectCourseRecordsPopupSaveDataCandidate records;
  u8 bytes[sizeof(CharacterSelectCourseRecordsPopupSaveDataCandidate)];
} CharacterSelectCourseRecordsPopupSaveBufferCandidate;

typedef struct CharacterSelectCourseRecordsPopupStackCandidate {
  u8 pad0[4];
  u8 *trickIcon;
  u8 pad8[0x15];
  u8 color;
  u8 pad1E[2];
  u16 quotient;
  u16 remainder;
  u8 pad24[8];
  s16 *courseIds;
} CharacterSelectCourseRecordsPopupStackCandidate;

extern u8 D_800E0B90[];
extern u8 D_800E0B94[];

#define CHARACTER_SELECT_COURSE_SAVE_DATA                                      \
  (*(CharacterSelectCourseRecordsPopupSaveBufferCandidate *)&gGameSaveDataBuffer[0])

void drawCharacterSelectCourseRecordsPopup(
    CharacterSelectCourseWidgetActor *arg0) {
  volatile u8 padding[0x20];
  volatile CharacterSelectCourseRecordsPopupStackCandidate stack;
  s16 *courseId;
  s32 quotient;
  u8 *characterIds;
  s32 digitOffset;
  s32 color;
  s32 yOffset;
  CharacterSelectCourseRecordsPopupSaveDataCandidate *saveData;
  s16 **courseIdsPtr;
  s32 score;
  s32 row;
  u16 tile;
  u8 palette;

  if (gRaceSplitscreenMode == 2) {
    stack.courseIds =
        gCharacterSelectCourseOptionsByUnlock[gHighestUnlockedCourse];
  } else if ((s32)gRaceTypeSelection < 2) {
    stack.courseIds = gCharacterSelectShortCourseOptions;
  } else {
    stack.courseIds = gCharacterSelectSingleCourseOption;
  }

  courseIdsPtr = (s16 **)&stack.courseIds;

  for (row = 0, yOffset = 0; row != 5; row++, yOffset += 0x14) {
    if (row < 3) {
      stack.color = 6;
    } else {
      stack.color = 4;
    }
    do {
    } while (0);
    tile = row + 0x77;
    drawMenuSprite(
        arg0->x, (s16)(arg0->y + yOffset),
        getRelocatableHeapBlockBase((s32)CHARACTER_SELECT_POPUP_FONT_HANDLE), tile,
        0x20U, 0x20U, 0U, 0U);

    if (gPlayerCount == 1) {
      if (gRaceSplitscreenMode != 2) {
        if (gRaceTypeSelection == 0) {
          if (row < 3) {
            stack.color = 0xC;
          } else {
            stack.color = 0xD;
          }
          drawCharacterSelectCourseRecordTime(
              &CHARACTER_SELECT_COURSE_SAVE_DATA.records
                   .raceRecords[stack.courseIds[gRaceCourseIndex.signedValue]][row],
              arg0->x + 0x14, arg0->y + yOffset, (s32)stack.color);
          drawMenuSprite(
              (s16)(arg0->x + 0x54), (s16)(arg0->y + yOffset),
              getRelocatableHeapBlockBase((s32)CHARACTER_SELECT_POPUP_FONT_HANDLE),
              (((&gGameSaveDataBuffer[0].raceRecordCharacterIds[0][0])[(stack.courseIds[gRaceCourseIndex.signedValue] * 5) + row] & 7) +
               0x51) &
                  0xFFFF,
              0x20U, 0x20U, 0U, 0U);
          drawMenuSprite(
              (s16)(arg0->x + 0x65), (s16)(arg0->y + yOffset),
              getRelocatableHeapBlockBase((s32)CHARACTER_SELECT_POPUP_FONT_HANDLE),
              (((s32)
                    (&gGameSaveDataBuffer[0].raceRecordCharacterIds[0][0])[(stack.courseIds[gRaceCourseIndex.signedValue] * 5) + row] >>
                3) +
               0x7C) &
                  0xFFFF,
              0x20U, 0x20U, 0U, 0U);
        } else {
          digitOffset = gRaceTypeSelection;
          if (digitOffset == 1) {
            if (row < 3) {
              color = 0xC;
            } else {
              color = 0xD;
            }
            courseId = &(*courseIdsPtr)[gRaceCourseIndex.signedValue];
            quotient = (s32)(&gGameSaveDataBuffer[0].scoreAttackScores[0][0])[(*courseId * 5) + row] / 10;
            stack.quotient = (u16)quotient;
            if (quotient & 0xFFFF) {
              drawMenuSprite(
                  (s16)(arg0->x + 0x18), (s16)(arg0->y + yOffset),
                  getRelocatableHeapBlockBase((s32)CHARACTER_SELECT_POPUP_FONT_HANDLE),
                  quotient = (stack.quotient + 0x2B) & 0xFFFF, 0x20U, 0x20U,
                  0U, (u8)(color + 1));
              courseId = &stack.courseIds[gRaceCourseIndex.signedValue];
            }
            palette = color + 1;
            stack.remainder =
                (u16)((s32)(&gGameSaveDataBuffer[0].scoreAttackScores[0][0])[(*courseId * 5) + row] % 10) ^ 0;
            drawMenuSprite(
                (s16)(arg0->x + 0x20), (s16)(arg0->y + yOffset),
                getRelocatableHeapBlockBase((s32)CHARACTER_SELECT_POPUP_FONT_HANDLE),
                (stack.remainder + 0x2B) & 0xFFFF, 0x20U, 0x20U, 0U, palette);
            drawMenuAsciiText((s16)(arg0->x + 0x28),
                              (s16)(arg0->y + yOffset + 8), D_800E0B90,
                              (u16)stack.color, 0x100U);
            drawMenuSprite(
                (s16)(arg0->x + 0x48), (s16)(arg0->y + yOffset),
                getRelocatableHeapBlockBase((s32)CHARACTER_SELECT_POPUP_FONT_HANDLE),
                (((&gGameSaveDataBuffer[0].scoreAttackCharacterIds[0][0])[(stack.courseIds[gRaceCourseIndex.signedValue] * 5) + row] & 7) +
                 0x51) &
                    0xFFFF,
                0x20U, 0x20U, 0U, 0U);
            drawMenuSprite(
                (s16)(arg0->x + 0x60), (s16)(arg0->y + yOffset),
                getRelocatableHeapBlockBase((s32)CHARACTER_SELECT_POPUP_FONT_HANDLE),
                (((s32)(&gGameSaveDataBuffer[0].scoreAttackCharacterIds[0][0])[((*courseIdsPtr)[gRaceCourseIndex.signedValue] * 5) + row] >>
                  3) +
                 0x7C) &
                    0xFFFF,
                0x20U, 0x20U, 0U, 0U);
          } else {
            stack.trickIcon = &CHARACTER_SELECT_COURSE_SAVE_DATA.bytes[row];
            if (row < 3) {
              color = 0xC;
            } else {
              color = 0xD;
            }
            palette = color + 1;
            score = CHARACTER_SELECT_COURSE_SAVE_DATA.records
                        .trickAttackScores[7][row];
            digitOffset = 0; do {
              drawMenuSprite(
                  (s16)((arg0->x - digitOffset) + 0x38),
                  (s16)(arg0->y + yOffset),
                  getRelocatableHeapBlockBase((s32)CHARACTER_SELECT_POPUP_FONT_HANDLE),
                  (((s32)score % 10) + 0x2B) & 0xFFFF, 0x20U, 0x20U, 0U,
                  palette);
              digitOffset += 8;
              score = score / 10;
            } while (score != 0);
            drawMenuAsciiText((s16)(arg0->x + 0x40),
                              (s16)(arg0->y + yOffset + 8), D_800E0B94,
                              (u16)stack.color, 0x100U);
            drawMenuSprite(
                (s16)(arg0->x + 0x4C), (s16)(arg0->y + yOffset),
                getRelocatableHeapBlockBase((s32)CHARACTER_SELECT_POPUP_FONT_HANDLE),
                (((characterIds =
                       (saveData =
                            (CharacterSelectCourseRecordsPopupSaveDataCandidate *)
                                stack.trickIcon)
                           ->trickAttackCharacterIds[7])[0] &
                  7) +
                 0x51) &
                    0xFFFF,
                0x20U, 0x20U, 0U, 0U);
            drawMenuSprite(
                (s16)(arg0->x + 0x60), (s16)(arg0->y + yOffset),
                getRelocatableHeapBlockBase((s32)CHARACTER_SELECT_POPUP_FONT_HANDLE),
                (((s32)((CharacterSelectCourseRecordsPopupSaveDataCandidate *)
                            stack.trickIcon)
                      ->trickAttackCharacterIds[7][0] >>
                  3) +
                 0x7C) &
                    0xFFFF,
                0x20U, 0x20U, 0U, 0U);
          }
        }
      } else {
        if (row < 3) {
          stack.color = 0xC;
        } else {
          stack.color = 0xD;
        }
        drawCharacterSelectCourseRecordTime(
            &CHARACTER_SELECT_COURSE_SAVE_DATA.records
                 .timeTrialRecords[(*courseIdsPtr)[gRaceCourseIndex.signedValue]][row],
            arg0->x + 0x14, arg0->y + yOffset, ((s32)stack.color) & 0xFFu);
        drawMenuSprite(
            (s16)(arg0->x + 0x54), (s16)(arg0->y + yOffset),
            getRelocatableHeapBlockBase((s32)CHARACTER_SELECT_POPUP_FONT_HANDLE),
            (((&gGameSaveDataBuffer[0].timeTrialCharacterIds[0][0])[((*courseIdsPtr)[gRaceCourseIndex.signedValue] * 5) + row] & 7) +
             0x51) &
                0xFFFF,
            0x20U, 0x20U, 0U, 0U);
        drawMenuSprite(
            (s16)(arg0->x + 0x65), (s16)(arg0->y + yOffset),
            getRelocatableHeapBlockBase((s32)CHARACTER_SELECT_POPUP_FONT_HANDLE),
            (((&gGameSaveDataBuffer[0].timeTrialCharacterIds[0][0])[((*courseIdsPtr)[gRaceCourseIndex.signedValue] * 5) + row] >>
              3) +
             0x7C) &
                0xFFFF,
            0x20U, 0x20U, 0U, 0U);
      }
    }
  }
}

#undef CHARACTER_SELECT_COURSE_SAVE_DATA
