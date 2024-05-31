//
// Created by os on 5/17/24.
//

#include "../h/_semaphore.h"
typedef _thread* thread_t ;

_queue<_sem*> _sem::allSemaphores;

int _sem::sem_open(sem_t* handle, unsigned init)
{
    sem_t newSem=new _sem(init);
    if(newSem!=nullptr) {*handle=newSem;
        allSemaphores.push(newSem);
        return 0;}
    return -1;

}

int _sem::sem_close(sem_t handle)
{   if(!handle->open)return -1;

    while(handle->waitThreads.head!=nullptr)
    {
        thread_t elem=handle->waitThreads.remove();
        elem->timeout=-1;
        elem->setReady();
        scheduler::instance().put(elem);
    }
    while (handle->blockedThreads.head!=nullptr)
    {
        thread_t elem=handle->blockedThreads.pop();
        elem->setReady();
        scheduler::instance().put(elem);
    }

    handle->open=false;
    return 0;
}

int _sem::sem_wait(sem_t id)

{   if(id==nullptr)return -4;
    if(!id->open)return -2;
    if(--id->val<0)
    {   _thread::running->setBlocked();
        id->blockedThreads.push(_thread::running);

        _thread::yield();
    }
    return (id->open?0:-1);
}
int _sem::sem_signal(sem_t id)
{
    if(!id->open)return -1;
    if(++id->val<=0)
    {
        _thread* unblocked=id->blockedThreads.pop();
        unblocked->setReady();
        scheduler::instance().put(unblocked);
    }
    return 0;
}

int _sem::sem_timedwait(sem_t id, time_t timeout)
{   if(sem_trywait(id)==0)return 0;
    id->hasWaiting=true;
    id->waitThreads.add(_thread::running,timeout);
    _thread::running->setBlocked();
    _thread::yield();
    if(id->waitThreads.head==nullptr)id->hasWaiting=false;
    return _thread::running->timeout;

}

int _sem::sem_trywait(sem_t id)
{
    if(!id->open)return -1;
    if(id->val > 0)
    {

        return sem_wait(id);

    }
    return 1;
}

void _sem::deleteSemaphores()
{
    while(allSemaphores.head!=nullptr)
    {
        _sem* old=allSemaphores.pop();
        delete old;
    }
}

void* _sem::operator new(size_t size)
    {
        return MemAllocator::instance().mem_alloc(MemAllocator::sizeInBlocks(size));
    }


void _sem::operator delete(void* addr)
{
    MemAllocator::instance().mem_free(addr);
}

void* _sem::operator new[](size_t size)
    {
    return MemAllocator::instance().mem_alloc(MemAllocator::sizeInBlocks(size));
    }


void _sem::operator delete[](void* addr){
    MemAllocator::instance().mem_free(addr);
}

void _sem::timedTick()
{
    _queue<_sem*>::Elem* cur=_sem::allSemaphores.head;
    while (cur!=nullptr)
    {   if(cur->seg->hasWaiting==true)
        {   int k=sem_trywait(cur->seg);
            if(k==1){
                cur->seg->waitThreads.dec();
                while( cur->seg->waitThreads.head!=nullptr && cur->seg->waitThreads.head->waitDelay<=0)
                {
                    _thread* timedOut=cur->seg->waitThreads.remove();
                    timedOut->setReady();
                    timedOut->timeout=-2;
                    scheduler::instance().put(timedOut);
                }
            }
            else if(k==0)
                {
                    _thread* timedOut=cur->seg->waitThreads.remove();
                    timedOut->setReady();
                    timedOut->timeout=0;
                    scheduler::instance().put(timedOut);
                }
    }
        cur=cur->next;
    }

}



