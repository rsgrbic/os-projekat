#include"regs.S"
.extern timerInterrupt
.extern handleSyscallorError
.extern HWhandler
.global trapHandler
.align 4
trapHandler:
j syscallorError
j handleProces
nop
nop
nop
nop
nop
nop
nop
j handleHw
syscallorError:
PUSHREGS
call handleSyscallorError
csrr t0,sepc
addi t0,t0,4
csrw sepc,t0
.irp param, 1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
ld x\param, \param*8(sp)//cuvam A0
.endr
addi sp,sp,256
sret
handleProces:
PUSHREGS
call timerInterrupt
POPREGS
sret

handleHw:
PUSHREGS
call HWhandler
POPREGS
sret



