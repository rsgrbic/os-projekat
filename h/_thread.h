
#ifndef THREAD_H
#define THREAD_H

#include "MemAllocator.h"
#include "../lib/hw.h"
#include "../h/SYSCALL_enum.h"
#include "_queue.h"

class scheduler;

class _thread {
    friend class _sem;
    friend class scheduler;
enum STATUS
    {
        READY,
        RUNNING,
        SUSPENDED,
        BLOCKED,
        FINISHED,
        SLEEPING
    };

public:
    typedef _thread* thread_t;
    using Func=void (*)(void*);

    STATUS getStatus() const;
    void setFinished();
    void setReady();
    void setBlocked();
    void setSuspended();
    void setSleeping();
    static time_t getInc();

    static void setRunning(_thread*);
    static int _threadcreate(thread_t* handle, Func func,void* arg,uint64* allocatedStack);

    int join();
    void setTimeout(int t){timeout=t;}
    static void disableSysThread();
    static void yield();
    static void dispatch();
    static int _threadexit();
    static void deleteThreads();
    void* operator new(size_t size);
    void operator delete(void *addr);
    void* operator new[](size_t size);
    void operator delete[](void *addr);
   ~_thread();

private:
    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    static void contextSwitch(Context *oldC,Context *newC);
    static int userSwitch();
    static void wrapper();
    explicit _thread(Func f, void* arg, uint64* allocatedStack);
    static void resetTimer();
    static void popSppSpie();
    static bool sysThread;
    Func func;
    uint64 *stack;
    Context context;
    STATUS status;
    void* argument;
    thread_t parent;
    bool parentWaiting;
    bool correct;
    int timeout;
    _thread* myHandle;
    int goodSleep;
    static time_t mytimeSlice;
    static _queue<_thread*> allThreads;
    static _thread* running;

};



#endif //THREAD_H
