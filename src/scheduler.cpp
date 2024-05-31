//
// Created by os on 5/15/24.
//

#include "../h/scheduler.h"

#include "../h/trapHandler.h"

_thread* scheduler::get()
{
    return threadQueue.pop();
}

void scheduler::put(_thread* thr)
{
    threadQueue.push(thr);
}

void scheduler::pushFirst(_thread* thr)
{
    threadQueue.pushFirst(thr);
}
scheduler& scheduler::instance()
{
    static scheduler Scheduler;
    return Scheduler;
}

int scheduler::sleep(time_t time)
{
    sleepingThreads.add(_thread::running,time);
    _thread::running->goodSleep=-1;
    _thread::running->setSleeping();
    uint64 volatile sepc;
    uint64 volatile sstatus;

    SupervisorRegsGuard(_thread::dispatch();)
    return _thread::running->goodSleep;
}

int scheduler::tickWakeUp()
{
    if(sleepingThreads.head!=nullptr){
        sleepingThreads.dec();
        while(sleepingThreads.head!=nullptr && (long)sleepingThreads.head->waitDelay<=0)
        {
            _thread* awakenedThr=sleepingThreads.remove();
            awakenedThr->setReady();
            awakenedThr->goodSleep=0;
            put(awakenedThr);
        }
    }
    return 0;
}




