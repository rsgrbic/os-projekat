//
// Created by os on 5/21/24.
//

#ifndef TIMEDLIST_HPP
#define TIMEDLIST_HPP
#include "_thread.h"

class _thread;
class timedList
{
public:
    void add (_thread* thr,uint64 delay);
    _thread* remove();
    void dec();
    bool notEmpty();
    void deleteThis(_thread* thr);
    struct Node
    {
        Node* next;
        _thread* thread;
        uint64 waitDelay;

        Node(_thread* tt,uint64 delay, Node* next = nullptr)
        {
            this->thread = tt;
            this->waitDelay=delay;
            this->next = next;
        }

        void* operator new(size_t size);
        void operator delete(void *addr);

    };

    void* operator new(size_t size);
    void operator delete(void *addr);
    //~timedList();
    Node* head=nullptr;
};



#endif //TIMEDLIST_HPP
