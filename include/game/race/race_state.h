#ifndef RACE_STATE_H
#define RACE_STATE_H

#include "common.h"

typedef union {
    s16 signedValue;
    u16 unsignedValue;
} RaceCourseIndexValue;

extern RaceCourseIndexValue gRaceCourseIndex;

#endif
