/* Preserve the controller thread jump table's original object boundary padding. */
.section .late_rodata
.word 0x00000000
.word 0x00000000
.word 0x00000000
.section .text

nonmatching checkControllerPakSaveStatus, 0x210

glabel checkControllerPakSaveStatus
    /* 1A00 80000E00 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* 1A04 80000E04 3C08800E */  lui        $t0, %hi(gControllerPakSaveFileIdentity)
    /* 1A08 80000E08 25084F80 */  addiu      $t0, $t0, %lo(gControllerPakSaveFileIdentity)
    /* 1A0C 80000E0C AFB00020 */  sw         $s0, 0x20($sp)
    /* 1A10 80000E10 3090FFFF */  andi       $s0, $a0, 0xFFFF
    /* 1A14 80000E14 AFA40048 */  sw         $a0, 0x48($sp)
    /* 1A18 80000E18 3C0F4E53 */  lui        $t7, (0x4E534B45 >> 16)
    /* 1A1C 80000E1C 240E7900 */  addiu      $t6, $zero, 0x7900
    /* 1A20 80000E20 35EF4B45 */  ori        $t7, $t7, (0x4E534B45 & 0xFFFF)
    /* 1A24 80000E24 24184542 */  addiu      $t8, $zero, 0x4542
    /* 1A28 80000E28 3C04800B */  lui        $a0, %hi(gControllerPakSaveExtNameBytesEnd)
    /* 1A2C 80000E2C 3C02800E */  lui        $v0, %hi(gControllerPakSaveFileIdentity)
    /* 1A30 80000E30 3C03800B */  lui        $v1, %hi(gControllerPakSaveGameNameBytesEnd)
    /* 1A34 80000E34 AFBF0024 */  sw         $ra, 0x24($sp)
    /* 1A38 80000E38 AD0E0000 */  sw         $t6, 0x0($t0)
    /* 1A3C 80000E3C AD0F0004 */  sw         $t7, 0x4($t0)
    /* 1A40 80000E40 A5180008 */  sh         $t8, 0x8($t0)
    /* 1A44 80000E44 24633104 */  addiu      $v1, $v1, %lo(gControllerPakSaveGameNameBytesEnd)
    /* 1A48 80000E48 24424F80 */  addiu      $v0, $v0, %lo(gControllerPakSaveFileIdentity)
    /* 1A4C 80000E4C 24843108 */  addiu      $a0, $a0, %lo(gControllerPakSaveExtNameBytesEnd)
  .L80000E50:
    /* 1A50 80000E50 90790000 */  lbu        $t9, 0x0($v1)
    /* 1A54 80000E54 24630001 */  addiu      $v1, $v1, 0x1
    /* 1A58 80000E58 0064082B */  sltu       $at, $v1, $a0
    /* 1A5C 80000E5C 24420001 */  addiu      $v0, $v0, 0x1
    /* 1A60 80000E60 1420FFFB */  bnez       $at, .L80000E50
    /* 1A64 80000E64 A0590009 */   sb        $t9, 0x9($v0)
    /* 1A68 80000E68 3C02800E */  lui        $v0, %hi(gControllerPakSaveFileIdentity)
    /* 1A6C 80000E6C 3C03800B */  lui        $v1, %hi(gControllerPakSaveGameNameBytes)
    /* 1A70 80000E70 3C04800B */  lui        $a0, %hi(gControllerPakSaveGameNameBytesEnd)
    /* 1A74 80000E74 24843104 */  addiu      $a0, $a0, %lo(gControllerPakSaveGameNameBytesEnd)
    /* 1A78 80000E78 246330F4 */  addiu      $v1, $v1, %lo(gControllerPakSaveGameNameBytes)
    /* 1A7C 80000E7C 24424F80 */  addiu      $v0, $v0, %lo(gControllerPakSaveFileIdentity)
  .L80000E80:
    /* 1A80 80000E80 90690000 */  lbu        $t1, 0x0($v1)
    /* 1A84 80000E84 906A0001 */  lbu        $t2, 0x1($v1)
    /* 1A88 80000E88 906B0002 */  lbu        $t3, 0x2($v1)
    /* 1A8C 80000E8C 906C0003 */  lbu        $t4, 0x3($v1)
    /* 1A90 80000E90 24630004 */  addiu      $v1, $v1, 0x4
    /* 1A94 80000E94 24420004 */  addiu      $v0, $v0, 0x4
    /* 1A98 80000E98 A049000A */  sb         $t1, 0xA($v0)
    /* 1A9C 80000E9C A04A000B */  sb         $t2, 0xB($v0)
    /* 1AA0 80000EA0 A04B000C */  sb         $t3, 0xC($v0)
    /* 1AA4 80000EA4 1464FFF6 */  bne        $v1, $a0, .L80000E80
    /* 1AA8 80000EA8 A04C000D */   sb        $t4, 0xD($v0)
    /* 1AAC 80000EAC 00106880 */  sll        $t5, $s0, 2
    /* 1AB0 80000EB0 01B06823 */  subu       $t5, $t5, $s0
    /* 1AB4 80000EB4 000D6880 */  sll        $t5, $t5, 2
    /* 1AB8 80000EB8 01B06821 */  addu       $t5, $t5, $s0
    /* 1ABC 80000EBC 3C0E800E */  lui        $t6, %hi(gControllerPakHandles)
    /* 1AC0 80000EC0 25CE4C40 */  addiu      $t6, $t6, %lo(gControllerPakHandles)
    /* 1AC4 80000EC4 000D68C0 */  sll        $t5, $t5, 3
    /* 1AC8 80000EC8 01AE2821 */  addu       $a1, $t5, $t6
    /* 1ACC 80000ECC 3C04800E */  lui        $a0, %hi(gControllerEventQueue)
    /* 1AD0 80000ED0 24844BD0 */  addiu      $a0, $a0, %lo(gControllerEventQueue)
    /* 1AD4 80000ED4 AFA50030 */  sw         $a1, 0x30($sp)
    /* 1AD8 80000ED8 0C028588 */  jal        osPfsInitPak
    /* 1ADC 80000EDC 02003025 */   or        $a2, $s0, $zero
    /* 1AE0 80000EE0 3C08800E */  lui        $t0, %hi(gControllerPakSaveFileIdentity)
    /* 1AE4 80000EE4 25084F80 */  addiu      $t0, $t0, %lo(gControllerPakSaveFileIdentity)
    /* 1AE8 80000EE8 3C19800E */  lui        $t9, %hi(gControllerPakFileNos)
    /* 1AEC 80000EEC 27394C30 */  addiu      $t9, $t9, %lo(gControllerPakFileNos)
    /* 1AF0 80000EF0 3C0F800E */  lui        $t7, %hi(gControllerPakGameName)
    /* 1AF4 80000EF4 0010C080 */  sll        $t8, $s0, 2
    /* 1AF8 80000EF8 03194821 */  addu       $t1, $t8, $t9
    /* 1AFC 80000EFC 25EF4F8A */  addiu      $t7, $t7, %lo(gControllerPakGameName)
    /* 1B00 80000F00 95050008 */  lhu        $a1, 0x8($t0)
    /* 1B04 80000F04 8D060004 */  lw         $a2, 0x4($t0)
    /* 1B08 80000F08 8FA40030 */  lw         $a0, 0x30($sp)
    /* 1B0C 80000F0C 3C07800E */  lui        $a3, %hi(gControllerPakExtName)
    /* 1B10 80000F10 24E74F8E */  addiu      $a3, $a3, %lo(gControllerPakExtName)
    /* 1B14 80000F14 AFAF0010 */  sw         $t7, 0x10($sp)
    /* 1B18 80000F18 0C028624 */  jal        osPfsFindFile
    /* 1B1C 80000F1C AFA90014 */   sw        $t1, 0x14($sp)
    /* 1B20 80000F20 14400007 */  bnez       $v0, .L80000F40
    /* 1B24 80000F24 00403825 */   or        $a3, $v0, $zero
    /* 1B28 80000F28 00105840 */  sll        $t3, $s0, 1
    /* 1B2C 80000F2C 3C01800F */  lui        $at, %hi(gControllerPakStatusCodes)
    /* 1B30 80000F30 002B0821 */  addu       $at, $at, $t3
    /* 1B34 80000F34 240A0002 */  addiu      $t2, $zero, 0x2
    /* 1B38 80000F38 10000029 */  b          .L80000FE0
    /* 1B3C 80000F3C A42AC9C8 */   sh        $t2, %lo(gControllerPakStatusCodes)($at)
  .L80000F40:
    /* 1B40 80000F40 8FA40030 */  lw         $a0, 0x30($sp)
    /* 1B44 80000F44 27A50040 */  addiu      $a1, $sp, 0x40
    /* 1B48 80000F48 27A6003C */  addiu      $a2, $sp, 0x3C
    /* 1B4C 80000F4C 0C028694 */  jal        osPfsNumFiles
    /* 1B50 80000F50 AFA70044 */   sw        $a3, 0x44($sp)
    /* 1B54 80000F54 8FAC003C */  lw         $t4, 0x3C($sp)
    /* 1B58 80000F58 8FA70044 */  lw         $a3, 0x44($sp)
    /* 1B5C 80000F5C 24010010 */  addiu      $at, $zero, 0x10
    /* 1B60 80000F60 15810007 */  bne        $t4, $at, .L80000F80
    /* 1B64 80000F64 27A50038 */   addiu     $a1, $sp, 0x38
    /* 1B68 80000F68 00107040 */  sll        $t6, $s0, 1
    /* 1B6C 80000F6C 3C01800F */  lui        $at, %hi(gControllerPakStatusCodes)
    /* 1B70 80000F70 002E0821 */  addu       $at, $at, $t6
    /* 1B74 80000F74 240D000C */  addiu      $t5, $zero, 0xC
    /* 1B78 80000F78 10000019 */  b          .L80000FE0
    /* 1B7C 80000F7C A42DC9C8 */   sh        $t5, %lo(gControllerPakStatusCodes)($at)
  .L80000F80:
    /* 1B80 80000F80 8FA40030 */  lw         $a0, 0x30($sp)
    /* 1B84 80000F84 0C0286E8 */  jal        osPfsFreeBlocks
    /* 1B88 80000F88 AFA70044 */   sw        $a3, 0x44($sp)
    /* 1B8C 80000F8C 8FA30038 */  lw         $v1, 0x38($sp)
    /* 1B90 80000F90 8FA70044 */  lw         $a3, 0x44($sp)
    /* 1B94 80000F94 04610003 */  bgez       $v1, .L80000FA4
    /* 1B98 80000F98 00037A03 */   sra       $t7, $v1, 8
    /* 1B9C 80000F9C 246100FF */  addiu      $at, $v1, 0xFF
    /* 1BA0 80000FA0 00017A03 */  sra        $t7, $at, 8
  .L80000FA4:
    /* 1BA4 80000FA4 29E10079 */  slti       $at, $t7, 0x79
    /* 1BA8 80000FA8 10200006 */  beqz       $at, .L80000FC4
    /* 1BAC 80000FAC 2418000B */   addiu     $t8, $zero, 0xB
    /* 1BB0 80000FB0 0010C840 */  sll        $t9, $s0, 1
    /* 1BB4 80000FB4 3C01800F */  lui        $at, %hi(gControllerPakStatusCodes)
    /* 1BB8 80000FB8 00390821 */  addu       $at, $at, $t9
    /* 1BBC 80000FBC 10000008 */  b          .L80000FE0
    /* 1BC0 80000FC0 A438C9C8 */   sh        $t8, %lo(gControllerPakStatusCodes)($at)
  .L80000FC4:
    /* 1BC4 80000FC4 24010005 */  addiu      $at, $zero, 0x5
    /* 1BC8 80000FC8 14E10005 */  bne        $a3, $at, .L80000FE0
    /* 1BCC 80000FCC 24090009 */   addiu     $t1, $zero, 0x9
    /* 1BD0 80000FD0 00105040 */  sll        $t2, $s0, 1
    /* 1BD4 80000FD4 3C01800F */  lui        $at, %hi(gControllerPakStatusCodes)
    /* 1BD8 80000FD8 002A0821 */  addu       $at, $at, $t2
    /* 1BDC 80000FDC A429C9C8 */  sh         $t1, %lo(gControllerPakStatusCodes)($at)
  .L80000FE0:
    /* 1BE0 80000FE0 10E00007 */  beqz       $a3, .L80001000
    /* 1BE4 80000FE4 3C0B800F */   lui       $t3, %hi(gControllerPakOperationCounts)
    /* 1BE8 80000FE8 256BC9E0 */  addiu      $t3, $t3, %lo(gControllerPakOperationCounts)
    /* 1BEC 80000FEC 020B1021 */  addu       $v0, $s0, $t3
    /* 1BF0 80000FF0 904C0000 */  lbu        $t4, 0x0($v0)
    /* 1BF4 80000FF4 00000000 */  nop
    /* 1BF8 80000FF8 258D0001 */  addiu      $t5, $t4, 0x1
    /* 1BFC 80000FFC A04D0000 */  sb         $t5, 0x0($v0)
  .L80001000:
    /* 1C00 80001000 8FBF0024 */  lw         $ra, 0x24($sp)
    /* 1C04 80001004 8FB00020 */  lw         $s0, 0x20($sp)
    /* 1C08 80001008 03E00008 */  jr         $ra
    /* 1C0C 8000100C 27BD0048 */   addiu     $sp, $sp, 0x48
endlabel checkControllerPakSaveStatus
