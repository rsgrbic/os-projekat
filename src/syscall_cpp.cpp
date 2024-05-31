
#include "../h/syscall_cpp.hpp"

void * operator new(size_t size){
    return mem_alloc(size);
}

void operator delete(void* addr){
    mem_free(addr);
}
void* operator new[](size_t size)
{
    return mem_alloc(size);
}
void operator delete[](void* addr){
    mem_free(addr);
}

Thread::Thread(void (*body)(void*),void* arg)
{
    this->body=body;
    this->arg=arg;
    this->myHandle=nullptr;
}

Thread::~Thread()
{
}


int Thread::start()
{   if (myHandle!=nullptr)return -2;
    return thread_create(&myHandle,body,arg);
}

void Thread::threadWrapper(void* t)
{
    if(t){((Thread*)t)->run();}
}

void Thread::dispatch()
{
    thread_dispatch();
}

int Thread::sleep(time_t time)
{
    return time_sleep(time);
}

Thread::Thread()
{
    myHandle=nullptr;
    arg=this;
    body=threadWrapper;
}

Semaphore::Semaphore(unsigned init)
{   myHandle=nullptr;
    sem_open(&myHandle,init);
}
Semaphore::~Semaphore()
{
    sem_close(myHandle);
}
int Semaphore::signal()
{
    return sem_signal(myHandle);
}

int Semaphore::wait()
{
    return sem_wait(myHandle);
}

int Semaphore::tryWait()
{
    return sem_tryWait(myHandle);
}

int Semaphore::timedWait(time_t t)
{
    return sem_timedWait(myHandle,t);
}

void PeriodicThread::terminate()
{
    period=0;
}

PeriodicThread::PeriodicThread(time_t period)
{
    this->period=period;
}

void PeriodicThread::run()
{
    while(period!=0)
    {
        periodicActivation();
        time_sleep(period);

    }
}


char Console::getc()
{
    return ::getc();
}
void Console::putc(char c)
{
    ::putc(c);
}












