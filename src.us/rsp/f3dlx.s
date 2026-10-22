.include "macro.inc"

.section .text, "ax"

.globl gF3dlxMicrocodeText
gF3dlxMicrocodeText:
glabel gspF3DLX_fifoTextStart
.incbin "assets/rsp/f3dlx.textbin.bin"
endlabel gspF3DLX_fifoTextStart

.globl gspF3DLX_fifoTextEnd
gspF3DLX_fifoTextEnd:

.section .rodata, "a"

dlabel gspF3DLX_fifoDataStart
.incbin "assets/rsp/f3dlx.rodatabin.bin"
enddlabel gspF3DLX_fifoDataStart

.globl gspF3DLX_fifoDataEnd
gspF3DLX_fifoDataEnd:
