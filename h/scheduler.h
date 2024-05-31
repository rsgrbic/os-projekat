//
// Created by os on 5/15/24.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../h/_queue.h"
#include "timedList.h"
class _thread;
class scheduler {

public:
    _thread* get();
    void put(_thread* thr);
    void pushFirst(_thread* thr);
    static scheduler& instance();
    scheduler(const scheduler&)=delete;
    scheduler& operator=(const scheduler&)=delete;
    int sleep(time_t time);
    int tickWakeUp();
private:
    scheduler()=default;
    _queue<_thread*> threadQueue;
    timedList sleepingThreads;
};



#endif //SCHEDULER_H
