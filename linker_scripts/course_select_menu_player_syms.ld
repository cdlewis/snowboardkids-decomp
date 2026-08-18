/* Player-one menu fields are addressed as standalone globals by the course
   select menu code, which predates the contiguous gRacePlayers array. */
gCourseSelectSelectedCourseId = gRacePlayers + 0x6;

/* The course select menu addresses the highlighted column as its own global,
   distinct from the character select roster index array it shares storage with. */
gCourseSelectHighlightedColumn = gCharacterSelectHighlightedRosterIndices;

/* The menu input repeat logic reads the held-pad state through its own
   declaration; the target reloads it where a single object would have been
   value-numbered into one load. */
gMenuInputHeld = gPlayerInputHeld;
