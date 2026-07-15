.include "macro.inc"

.section .text.mp, "ax", @progbits
.set noreorder

nonmatching initMainMenuSceneModelParts, 0x108

glabel initMainMenuSceneModelParts
    lhu        $t3, 0x2($a0)
    addiu      $a3, $zero, 0x1
    addiu      $t0, $zero, 0x6
    addiu      $a2, $zero, -0x1
    addiu      $t6, $zero, 0x2
    addiu      $t7, $zero, 0x4
    addiu      $t8, $zero, 0x8
    addiu      $t9, $zero, 0xA
    addiu      $t1, $zero, 0x3
    addiu      $t2, $zero, 0x5
    lui        $v0, %hi(gMainMenuSceneModelPartInitDataByModel)
    sll        $t4, $t3, 2
    sb         $a2, 0x1C($a0)
    sb         $a2, 0x30($a0)
    sb         $a3, 0x44($a0)
    sb         $t6, 0x58($a0)
    sb         $a3, 0x6C($a0)
    sb         $t7, 0x80($a0)
    sb         $a3, 0x94($a0)
    sb         $t0, 0xA8($a0)
    sb         $t0, 0xBC($a0)
    sb         $t8, 0xD0($a0)
    sb         $t0, 0xE4($a0)
    sb         $t9, 0xF8($a0)
    sb         $t1, 0x10C($a0)
    sb         $t2, 0x120($a0)
    addu       $v0, $v0, $t4
    lw         $v0, %lo(gMainMenuSceneModelPartInitDataByModel)($v0)
    addiu      $a2, $zero, 0xE
    or         $v1, $zero, $zero
    or         $a1, $a0, $zero
.L80042B30:
    sh         $zero, 0x22($a1)
    lh         $a0, 0x22($a1)
    addiu      $v1, $v1, 0x2
    sh         $a0, 0x20($a1)
    sh         $a0, 0x1E($a1)
    lh         $t5, 0x0($v0)
    addiu      $v0, $v0, 0xC
    sll        $t6, $t5, 16
    sw         $t6, 0x24($a1)
    lh         $t7, -0xA($v0)
    addiu      $a1, $a1, 0x28
    sll        $t8, $t7, 16
    sw         $t8, 0x0($a1)
    lh         $t9, -0x8($v0)
    sh         $zero, 0xE($a1)
    lh         $a0, 0xE($a1)
    sll        $t1, $t9, 16
    sw         $t1, 0x4($a1)
    sh         $a0, 0xC($a1)
    sh         $a0, 0xA($a1)
    lh         $t2, -0x6($v0)
    nop
    sll        $t3, $t2, 16
    sw         $t3, 0x10($a1)
    lh         $t4, -0x4($v0)
    nop
    sll        $t5, $t4, 16
    sw         $t5, 0x14($a1)
    lh         $t6, -0x2($v0)
    nop
    sll        $t7, $t6, 16
    bne        $v1, $a2, .L80042B30
     sw        $t7, 0x18($a1)
    jr         $ra
     nop
endlabel initMainMenuSceneModelParts
    nop

.set reorder
