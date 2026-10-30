.include "macro.inc"

.section .text, "ax"

glabel rspbootTextStart
#if defined(VERSION_JP)
.incbin "assets.jp/rsp/rspboot.textbin.bin"
#else
.incbin "assets/rsp/rspboot.textbin.bin"
#endif
endlabel rspbootTextStart

.globl rspbootTextEnd
rspbootTextEnd:
