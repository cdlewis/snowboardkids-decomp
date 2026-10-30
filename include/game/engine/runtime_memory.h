#ifndef RUNTIME_MEMORY_H
#define RUNTIME_MEMORY_H

#include "common.h"

#define RELOCATABLE_HEAP_SIZE 0x1C0000
#define INITIAL_BOOT_STACK_SIZE 0x2000
#define BOOT_THREAD_STACK_SIZE 0x2000
#define GAME_THREAD_STACK_SIZE 0x4000
/* Unclaimed space between the game-thread stack and the fixed RSP buffers. */
#if defined(VERSION_JP)
#define RUNTIME_BUFFER_RESERVED_SIZE 0x38860
#else
#define RUNTIME_BUFFER_RESERVED_SIZE 0x37B80
#endif
#define SOUND_HEAP_SIZE 0x80000
#define MUSIC_SEQUENCE_BUFFER_SIZE 0x10000
#define SOUND_BANK_WAVE_COUNT 0xFB

extern u8 gBootThreadStack[BOOT_THREAD_STACK_SIZE];
extern u8 gGameThreadStack[GAME_THREAD_STACK_SIZE];
extern u8 gRelocatableHeapStart[];

#endif
