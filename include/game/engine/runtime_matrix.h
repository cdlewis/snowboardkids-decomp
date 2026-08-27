#ifndef RUNTIME_MATRIX_H
#define RUNTIME_MATRIX_H

#include "common.h"

typedef union RuntimeMtx {
    Mtx mtx;
    u32 words[16];
    s64 forceStructureAlignment;
} RuntimeMtx;

#endif
