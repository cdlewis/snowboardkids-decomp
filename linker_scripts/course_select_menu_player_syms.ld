/* Player-one menu fields are addressed as standalone globals by the course
   select menu code, which predates the contiguous gRacePlayers array. */
gCourseSelectSelectedCourseId = gRacePlayers + 0x6;

/* The course select menu addresses the highlighted column as its own global,
   distinct from the character select roster index array it shares storage with. */
gCourseSelectHighlightedColumn = gCharacterSelectHighlightedRosterIndices;
