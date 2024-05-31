
#ifndef _SEMAPHORE_HPP
#define _SEMAPHORE_HPP
#include "../lib/hw.h"
#include "_thread.h"
#include "timedList.h"
#include "_queue.h"
#include "scheduler.h"

class _thread;
class _sem{
public:
    typedef _sem* sem_t;
   // static _queue<sem_t> allSemaphores;
    static int sem_open(sem_t* handle, unsigned init);
    static  int sem_close(sem_t handle);
    static int sem_wait(sem_t id);
    static int sem_signal(sem_t id);
    static int sem_timedwait(sem_t id, time_t timeout);
    static int sem_trywait(sem_t id);
    static void deleteSemaphores();
    void* operator new(size_t size);
    void operator delete(void* addr);
    void* operator new[](size_t size);
    void operator delete[](void* addr);
    static _queue<_sem*> allSemaphores;
    static void timedTick();
private:
    sem_t myHandle;
    explicit _sem(unsigned init): val(init),open(true),hasWaiting(false){myHandle=this;}
    int val;
    bool open;
    bool hasWaiting;
    _queue<_thread*> blockedThreads;
    timedList waitThreads;

};



#endif //_SEMAPHORE_HPP
