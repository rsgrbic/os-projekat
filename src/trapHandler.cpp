#include "../h/trapHandler.h"


enum SCAUSE_VALUE: unsigned long
{   SUPERVISORINTERRUPT=0x8000000000000001UL,
    HARDWAREINTERRUPT=0x8000000000000009UL,
    ILLEGALINSTRUCTION=0x2UL,
    ILLEGALREAD=0x5UL,
    ILLEGALWRITE=0x7UL,
    USERSYSCALL=0x8UL,
    KERNELSYSCALL=0x9UL
};

extern "C" uint64 handleSyscallorError(uint64 ID,void* a1,void* a2,void* a3,void* a4){
    uint64 volatile syscall=0;
    uint64 scause;

    __asm__ volatile ("csrr %[ime],scause":[ime]"=r"(scause));
    switch (scause)
    {
    case SCAUSE_VALUE::ILLEGALINSTRUCTION:
       Console::instance().put_output('I'); return -1;break;
    
    case SCAUSE_VALUE::ILLEGALREAD:
        Console::instance().put_output('R');; return -1;break;


    case SCAUSE_VALUE::ILLEGALWRITE:
       Console::instance().put_output('W');; return -1;break;

    case SCAUSE_VALUE::KERNELSYSCALL:
    case SCAUSE_VALUE::USERSYSCALL:
        syscall=1;break;


    
    }
    uint64 volatile retVal=0;

    if(syscall)
    {   uint64 volatile sepc;
        uint64 volatile sstatus;
        switch (ID)
        {
        case SYSCALL_ID::MEM_ALLOC:
            retVal= (size_t)MemAllocator::instance().mem_alloc((size_t)a1);
            break;

        case SYSCALL_ID::MEM_FREE:
            retVal= (int)MemAllocator::instance().mem_free(a1);break;

        case SYSCALL_ID::THREAD_CREATE :
            retVal= (int)_thread::_threadcreate((_thread**)a1, (_thread::Func)(a2),a3,(uint64*)a4);break;

        case SYSCALL_ID::THREAD_EXIT :
            retVal= _thread::_threadexit();
            break;

        case SYSCALL_ID::THREAD_DISPATCH :

            SupervisorRegsGuard({_thread::dispatch();})
            break;

        case SYSCALL_ID::JOIN:
            if(a1==nullptr)return retVal;
            SupervisorRegsGuard({retVal=((_thread*)a1)->join();})
            break;

        case SYSCALL_ID::SEM_OPEN :
            retVal= _sem::sem_open((_sem**)a1,(uint64)a2);
            break;

        case SYSCALL_ID::SEM_CLOSE :
            retVal= _sem::sem_close((_sem*)a1);
            break;

        case SYSCALL_ID::SEM_SIGNAL :
            {retVal= _sem::sem_signal((_sem*)a1);}
            break;
        case SYSCALL_ID::SEM_WAIT:
            SupervisorRegsGuard({retVal=_sem::sem_wait((_sem*)a1);})
            break;

        case SYSCALL_ID::SEM_TIMEDWAIT :
            SupervisorRegsGuard({retVal=_sem::sem_timedwait((_sem*)a1,(time_t)a2);})
            break;

        case SYSCALL_ID::SEM_TRYWAIT :
            SupervisorRegsGuard({retVal= _sem::sem_trywait((_sem*)a1);})break;

        case SYSCALL_ID::TIME_SLEEP :
            SupervisorRegsGuard({retVal=scheduler::instance().sleep((time_t)a1);});
            break;


        case SYSCALL_ID::GETC :
            SupervisorRegsGuard({Console::instance().fillInput();
                                retVal=(uint64)Console::instance().get_input();})

            break;

        case SYSCALL_ID::PUTC :
            SupervisorRegsGuard({Console::instance().put_output((uint64)a1);
                                Console::instance().clearOutput();})
            break;

        case SYSCALL_ID::USERSWITCH:
            __asm__ ("csrc sstatus,%[offs]"::[offs]"r"(0x100));
            break;

        }

    }

    return retVal;
}

extern  "C" void timerInterrupt()
{   uint64 volatile sepc;
    uint64 volatile sstatus;
   SupervisorRegsGuard(
       _sem::timedTick();
       scheduler::instance().tickWakeUp();

    if(_thread::getInc()>=DEFAULT_TIME_SLICE)
    {
        _thread::dispatch();
    }
    )
    __asm__ volatile ("csrc sip,%[num]"::[num]"i"(2));
}

extern "C" void HWhandler()
{
    int id=plic_claim();
    if(id==CONSOLE_IRQ)
    {
        Console::instance().fillInput();
        Console::instance().clearOutput();
    }
    plic_complete(id);
    __asm__ volatile ("csrc sip,%[num]"::[num]"i"(2));
}