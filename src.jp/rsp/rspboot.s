.include "macro.inc"

.section .text, "ax"

glabel rspbootTextStart
.incbin "assets.jp/rsp/rspboot.textbin.bin"
endlabel rspbootTextStart

.globl rspbootTextEnd
rspbootTextEnd:
