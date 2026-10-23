#ifndef LIBMUS_SEQUENCE_COMMANDS_H
#define LIBMUS_SEQUENCE_COMMANDS_H

#include "libmus/libmus_data.h"

/* Project helpers for the bytecode interpreted by libmus/player.c.
 * These expand to bytes in a u8 initializer, not audio microcode commands.
 * Short operands are 0..127; long operands are 0..32767. Keep the original
 * short/long encoding when matching existing sequence data.
 */

/* The player holds this note instead of advancing to the next event. */
#define MUS_DURATION_INFINITE 0x7FFF

#define MUS_U15(value) (0x80 | (((value) >> 8) & 0x7F)), ((value) & 0xFF)

#define MUS_STOP() Cstop
#define MUS_WAVE(index) Cwave, (index)
#define MUS_WAVE_LONG(index) Cwave, MUS_U15(index)
#define MUS_ENVELOPE_DEFAULT(time0, volume0, time1, volume1, time2, volume2, time3) \
    Cdefa, (time0), (volume0), (time1), (volume1), (time2), (volume2), (time3)
#define MUS_PAN(value) Cpan, (value)
#define MUS_VOLUME(value) Cvolume, (value)
#define MUS_START_EFFECT(index) Cstartfx, (index)
#define MUS_START_EFFECT_LONG(index) Cstartfx, MUS_U15(index)

/* For streams with per-note durations and velocity indices disabled. */
#define MUS_NOTE(note, duration) (note), (duration)
#define MUS_NOTE_LONG(note, duration) (note), MUS_U15(duration)

#endif
