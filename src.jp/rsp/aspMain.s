.include "macro.inc"

.section .text, "ax"

glabel aspMainTextStart
.incbin "assets.jp/rsp/aspMain.textbin.bin"
endlabel aspMainTextStart

.globl aspMainTextEnd
aspMainTextEnd:

.section .rodata, "a"

dlabel aspMainDataStart
.incbin "assets.jp/rsp/aspMain.rodatabin.bin"
enddlabel aspMainDataStart

.globl aspMainDataEnd
aspMainDataEnd:
