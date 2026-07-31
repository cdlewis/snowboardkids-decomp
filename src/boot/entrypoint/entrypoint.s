#include "hasm.h"

ABS(BOOT_STACK_SIZE, 0x2000)
ABS(STACK_SIZE, BOOT_STACK_SIZE)
ABS(MAIN_BSS_CLEAR_SIZE, 0x7DAC0)
ABS(bootStackTop, gBootThreadStack)

.set noreorder

.section .text

.align 4

LEAF(entrypoint)
  lui     $t0, %hi(main_BSS_START)
  lui     $t1, (MAIN_BSS_CLEAR_SIZE >> 16)
  addiu   $t0, $t0, %lo(main_BSS_START)
  ori     $t1, $t1, (MAIN_BSS_CLEAR_SIZE & 0xFFFF)

.clear_bss:
  addi    $t1, $t1, -0x8
  sw      $zero, 0x0($t0)
  sw      $zero, 0x4($t0)
  bnez    $t1, .clear_bss
   addi   $t0, $t0, 0x8

.enter_program:
  lui     $t2, %hi(main)
  lui     $sp, %hi(bootStackTop)
  addiu   $t2, $t2, %lo(main)

  jr      $t2
   addiu  $sp, $sp, %lo(bootStackTop)
END(entrypoint)

.fill 0x50 - (. - entrypoint)
