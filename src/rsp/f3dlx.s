.include "macro.inc"

.section .text, "ax"

.globl D_800B1CC0
D_800B1CC0:
glabel gspF3DLX_fifoTextStart
.incbin "assets/rsp/f3dlx.textbin.bin"
endlabel gspF3DLX_fifoTextStart

.globl gspF3DLX_fifoTextEnd
gspF3DLX_fifoTextEnd:
