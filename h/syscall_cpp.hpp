//
// Created by os on 5/23/24.
//

#ifndef SYSCALL_PP_H
#define SYSCALL_PP_H

#include "syscall_c.h"

void* operator new(size_t size);
void operator delete(void* addr);

class Thread
{
public:
    Thread(void (*body)(void*),void* arg);
    virtual ~Thread();
    int start();
    static void dispatch();
    static int sleep(time_t);
protected:
    Thread();
    virtual void run(){}

private:
    thread_t myHandle;
   static void threadWrapper(void*);
    void (*body)(void*);
    void* arg;
};

class Semaphore
{
public:
    Semaphore(unsigned init=1);
    virtual ~Semaphore();
    int wait();
    int signal();
    int timedWait(time_t t);
    int tryWait();
private:
    sem_t myHandle;
};

class PeriodicThread:public Thread
{
public:
    void terminate();
protected:
    PeriodicThread(time_t period);
    virtual void periodicActivation(){}
    void run () override;
private:
    time_t period;
};

class Console
{
public:
    static char getc();
    static void putc(char);
};

#endif //SYSCALL_PP_H
