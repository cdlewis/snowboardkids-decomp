.include "macro.inc"

.section .text, "ax"

.globl gF3dlxMicrocodeText
gF3dlxMicrocodeText:
glabel gspF3DLX_fifoTextStart
.incbin "assets/rsp/f3dlx.textbin.bin"
endlabel gspF3DLX_fifoTextStart

.globl gspF3DLX_fifoTextEnd
gspF3DLX_fifoTextEnd:
