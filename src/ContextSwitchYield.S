#include"regs.S"

.extern _ZN7_thread8dispatchEv
.global _ZN7_thread5yieldEv
.type _ZN7_thread5yieldEv, @function

_ZN7_thread5yieldEv:
PUSHREGS

call _ZN7_thread8dispatchEv

POPREGS

ret

.global _ZN7_thread13contextSwitchEPNS_7ContextES1_
.type _ZN7_thread13contextSwitchEPNS_7ContextES1_, @function

_ZN7_thread13contextSwitchEPNS_7ContextES1_:
sd ra,0*8(a0)
sd sp,1*8(a0)

ld ra,0*8(a1)
ld sp,1*8(a1)

ret