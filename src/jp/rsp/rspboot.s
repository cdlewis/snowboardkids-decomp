.include "macro.inc"

.section .text, "ax"

glabel rspbootTextStart
.incbin "assets.jp/jp/rsp/rspboot.textbin.bin"
endlabel rspbootTextStart

.globl rspbootTextEnd
rspbootTextEnd:
