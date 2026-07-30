.section .late_rodata

dlabel D_800E10EC
    .float 0.0
enddlabel D_800E10EC

.section .text

glabel queueTrainingCourseRaceInit
    addiu $sp, $sp, -0x18
    sw $ra, 0x14($sp)
    lui $a0, %hi(initTrainingCourseRace)
    addiu $a0, $a0, %lo(initTrainingCourseRace)
    jal setCurrentGameTaskCallback
    or $a1, $zero, $zero
    lw $ra, 0x14($sp)
    addiu $sp, $sp, 0x18
    jr $ra
    nop
endlabel queueTrainingCourseRaceInit
