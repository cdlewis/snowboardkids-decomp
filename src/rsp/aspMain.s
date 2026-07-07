.include "macro.inc"

.section .text, "ax"

glabel aspMainTextStart
.incbin "assets/rsp/aspMain.textbin.bin"
endlabel aspMainTextStart

.globl aspMainTextEnd
aspMainTextEnd:
