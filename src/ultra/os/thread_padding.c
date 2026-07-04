#include "common.h"

/*
 * Unused libultra text padding between osSetThreadPri and osViBlack.
 * The target ROM contains 0x60 bytes of nop instructions here.
 */
#pragma GLOBAL_ASM("src/ultra/os/thread_padding.s")
