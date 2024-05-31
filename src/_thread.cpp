#include "../h/_thread.h"
#include "../h/scheduler.h"
_thread::thread_t _thread::running=nullptr;
bool _thread::sysThread=true;
_queue<_thread*> _thread::allThreads;
time_t _thread::mytimeSlice=0;
_thread::_thread(Func f, void* arg, uint64* allocatedStack)
{
    parent=_thread::running;
    parentWaiting=false;
    argument=arg;
    stack=allocatedStack;
    func=f;
    context={(uint64)&wrapper,(uint64)(&stack[DEFAULT_STACK_SIZE])};
    status=STATUS::READY;
    correct=true;
    myHandle=this;
    timeout=0;
    goodSleep=0;
    if(f!=nullptr)
    scheduler::instance().put(this);
}

void _thread::dispatch()
{   resetTimer();
    thread_t old = _thread::running;
    if (old->getStatus()==STATUS::READY || old->getStatus()==STATUS::RUNNING)
    {
    scheduler::instance().put(old);
    }
    else if (old->parent && old->parentWaiting && old->getStatus() ==STATUS::FINISHED)
    {   old->parent->setReady();
        old->parentWaiting=false;
        scheduler::instance().put(old->parent);
    }
    running = scheduler::instance().get();
    running->status=STATUS::RUNNING;
    contextSwitch(&old->context, &running->context);

}

int _thread::_threadcreate(thread_t* handle, Func func, void* arg, uint64* allocatedStack)
{
    thread_t newTh=new _thread(func,arg,allocatedStack);
    if(newTh==nullptr)return -1;
    if (newTh->correct==false)return -2;
    *handle=newTh;
    _thread::allThreads.push(newTh);

    return 0;
}

int _thread::join()
{
    if(parent != _thread::running)return -1;
    if(this->getStatus() == STATUS::FINISHED) return 0;
    parentWaiting=true;
    _thread::running->setSuspended();
    this->yield();

    return 0;

}

void _thread::disableSysThread()
{
    sysThread=false;

}

int _thread::_threadexit()
{
    if(running->getStatus()!=STATUS::RUNNING) return -1;
    running->setFinished();
    __asm__ volatile("li a0,0x13");
    __asm__ volatile("ecall");

    return 0;
}

void _thread::deleteThreads()
{
    while(allThreads.head!=nullptr)
    {
        _thread* old = allThreads.pop();
        delete old;
    }
}

void _thread::wrapper()
    {_thread::popSppSpie();
    if(!sysThread)
    {
    userSwitch();
    }

    running->func(running->argument);
    _threadexit();

}
int _thread::userSwitch()
{
    __asm__ ("li a0, %[sys]" :: [sys] "i" (SYSCALL_ID::USERSWITCH));
    __asm__ ("ecall");
    return 0;
}

void* _thread::operator new(size_t size){
    return MemAllocator::instance().mem_alloc(MemAllocator::sizeInBlocks(size));
}

void _thread::operator delete(void *addr){
    MemAllocator::instance().mem_free(addr);
}
void* _thread::operator new[](size_t size){
    return MemAllocator::instance().mem_alloc(MemAllocator::sizeInBlocks(size));
}

void _thread::operator delete[](void *addr){
    MemAllocator::instance().mem_free(addr);
}


_thread::STATUS _thread::getStatus() const {return  status;}

void _thread::setFinished(){ status=STATUS::FINISHED;;}

void _thread::setReady(){status=STATUS::READY;}

void _thread::setBlocked(){status=STATUS::BLOCKED;}

void _thread::setSuspended(){status=STATUS::SUSPENDED;}

void _thread::setSleeping()
{
    status=STATUS::SLEEPING;
}


_thread::~_thread()
{
    MemAllocator::instance().mem_free(stack);
}

time_t _thread::getInc(){return ++mytimeSlice;}
void _thread::resetTimer(){mytimeSlice=0;}

void _thread::setRunning(_thread* t)
{
    running=t;
}
void _thread::popSppSpie()
{
    __asm__ volatile("csrw sepc,ra");
    __asm__ volatile ("sret");
}
