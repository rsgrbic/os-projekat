
#include "../lib/hw.h"
#include "../h/syscall_c.h"
#include "../h/_semaphore.h"
#include "../h/_thread.h"
extern "C" void trapHandler();
extern void userMain();
void userMainWrapper(void* t)
{
   userMain();
}
void delay(void *t)
{
     while((long)t)
     {
         thread_dispatch();
     }
}

int main(){
     uint64 goodAddr=(uint64)&trapHandler | 0x01;
     __asm__ volatile("csrw stvec, %[ime]"::[ime] "r"(goodAddr));
    _thread*kernel,*delayThr,*user;
    thread_create(&kernel,nullptr,nullptr);
    _thread::setRunning(kernel);
    _thread::disableSysThread();
    int control=1;
    thread_create(&user,userMainWrapper,nullptr);
    thread_create(&delayThr,delay,&control);
    __asm__("csrs sstatus,2");
    join(user);
    control=0;
    _thread::deleteThreads();
    _sem::deleteSemaphores();
    return 0;
}

