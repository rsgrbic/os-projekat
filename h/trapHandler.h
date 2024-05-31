#ifndef TRAPHANDLER_H
#define TRAPHANDLER_H
#include "../lib/hw.h"
#include "../h/MemAllocator.h"
#include "../h/Console.h"
#include "../h/SYSCALL_enum.h"
#include "../h/_semaphore.h"
#include "../h/_thread.h"
#define SupervisorRegsGuard(codeSeg) \
__asm__ volatile("csrr %0,sepc" : "=r"(sepc)); \
__asm__ volatile("csrr %0,sstatus" : "=r"(sstatus));\
codeSeg \
__asm__ volatile("csrw sepc,%0" :: "r"(sepc)); \
__asm__ volatile("csrw sstatus,%0" :: "r"(sstatus));

#endif
