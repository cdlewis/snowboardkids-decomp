#ifndef TITLE_DEMO_SNAPSHOT_DATA_H
#define TITLE_DEMO_SNAPSHOT_DATA_H

#include "game/engine/runtime_memory.h"
#include "game/race/player/race_player_input.h"

#if defined(VERSION_JP)
#define DEMO_CAPTURED_HEAP_OFFSET 3296
#else
#define DEMO_CAPTURED_HEAP_OFFSET 0
#endif

/* These render-cache pointers were captured with the complete RacePlayer state.
 * Their offsets preserve the recording's bytes; the heap symbol supplies the
 * current RAM placement. Rendering replaces the caches on the first viewport.
 * Offsets describe recorded locations, not permanent allocations in the heap. */
#define DEMO_HEAP_MATRIX(byteOffset) ((Mtx *)&gRelocatableHeapStart[(byteOffset) + DEMO_CAPTURED_HEAP_OFFSET])
#define DEMO_HEAP_VERTICES(byteOffset) ((Vtx *)&gRelocatableHeapStart[(byteOffset) + DEMO_CAPTURED_HEAP_OFFSET])

/* drawRacePlayerModel allocates each part consecutively, with no other scratch
 * allocations between parts. Every captured run contains all fourteen parts. */
#define DEMO_MODEL_MATRICES(byteOffset) \
    { \
        DEMO_HEAP_MATRIX((byteOffset) + 0 * sizeof(Mtx)), \
        DEMO_HEAP_MATRIX((byteOffset) + 1 * sizeof(Mtx)), \
        DEMO_HEAP_MATRIX((byteOffset) + 2 * sizeof(Mtx)), \
        DEMO_HEAP_MATRIX((byteOffset) + 3 * sizeof(Mtx)), \
        DEMO_HEAP_MATRIX((byteOffset) + 4 * sizeof(Mtx)), \
        DEMO_HEAP_MATRIX((byteOffset) + 5 * sizeof(Mtx)), \
        DEMO_HEAP_MATRIX((byteOffset) + 6 * sizeof(Mtx)), \
        DEMO_HEAP_MATRIX((byteOffset) + 7 * sizeof(Mtx)), \
        DEMO_HEAP_MATRIX((byteOffset) + 8 * sizeof(Mtx)), \
        DEMO_HEAP_MATRIX((byteOffset) + 9 * sizeof(Mtx)), \
        DEMO_HEAP_MATRIX((byteOffset) + 10 * sizeof(Mtx)), \
        DEMO_HEAP_MATRIX((byteOffset) + 11 * sizeof(Mtx)), \
        DEMO_HEAP_MATRIX((byteOffset) + 12 * sizeof(Mtx)), \
        DEMO_HEAP_MATRIX((byteOffset) + 13 * sizeof(Mtx)), \
    }

/* Initializers for title_demo_player_snapshots.inc.c. Rotation rows use the
 * game's fixed-point matrix units; vectors retain raw fixed-point coordinates. */
#define DEMO_VEC3(vx, vy, vz) { .x = (vx), .y = (vy), .z = (vz) }
#define DEMO_TRANSFORM(r00, r01, r02, r10, r11, r12, r20, r21, r22, tx, ty, tz)        \
    {                                                                                  \
        .rotation = { (r00), (r01), (r02), (r10), (r11), (r12), (r20), (r21), (r22) }, \
        .translation = DEMO_VEC3(tx, ty, tz),                                          \
}
#define DEMO_MODEL_PART(parent, mirrored, rx, ry, rz, ox, oy, oz) \
    {                                                             \
        .parentPartIndex = (parent),                              \
        .mirroredPartIndex = (mirrored),                          \
        .rotation = DEMO_VEC3(rx, ry, rz),                        \
        .offset = DEMO_VEC3(ox, oy, oz),                          \
    }

typedef char TitleDemoModelMatrixRunSizeCheck[(RACE_PLAYER_MODEL_PART_CAPACITY == 14) ? 1 : -1];

#endif
