.include "macro.inc"

.section .text, "ax"

glabel rspbootTextStart
.incbin "assets/us/rsp/rspboot.textbin.bin"
endlabel rspbootTextStart

.globl rspbootTextEnd
rspbootTextEnd:
