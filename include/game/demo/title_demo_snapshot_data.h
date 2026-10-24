#ifndef TITLE_DEMO_SNAPSHOT_DATA_H
#define TITLE_DEMO_SNAPSHOT_DATA_H

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

#endif
