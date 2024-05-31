//
// Created by os on 5/22/24.
//

#ifndef _QUEUE_HPP
#define _QUEUE_HPP
#include "MemAllocator.h"
template<typename T>
class _queue {
public:

    T pop();
    void push(T tt);
    void remove(T tt);
    bool isEmpty();
    void pushFirst(T tt);
    struct Elem
    {
        Elem* next;
        T seg;

        explicit Elem(T tt, Elem* next = nullptr)
        {
            this->seg = tt;
            this->next = next;
        }

        void* operator new(size_t size);

        void operator delete(void *addr);

    };
    Elem *head=nullptr,*tail=nullptr;
    void* operator new(size_t size);

    void operator delete(void* addr);

    _queue() = default;
    _queue(const _queue&) = delete;
    _queue& operator=(const _queue&) = delete;
   // ~_queue();
};

template<typename T>
bool _queue<T>::isEmpty()
{
    return head==nullptr;
}

template <typename T>
void _queue<T>::pushFirst(T tt)
{
    Elem * nElem=new Elem(tt);
    nElem->next=head;
    head=nElem;

}

template<typename T>
void _queue<T>::push(T tt)
{
    Elem* nElem=new Elem(tt);
    if(tail!=nullptr)
    {
        tail->next=nElem;
        tail=nElem;
    }
    else
    {
        tail=head=nElem;
    }
}
template<typename T>
T _queue<T>::pop()
{   if(head==nullptr)return nullptr;
    Elem* elem=head;
    head=head->next;
    if(head==nullptr) tail=nullptr;
    T t=elem->seg;
    delete elem;
    return t;

}
template<typename T>
void _queue<T>::remove(T tt){
    Elem* prev = nullptr;
    Elem* curr = head;

    for(;curr!=nullptr;prev=curr,curr=curr->next) {

        if (curr->seg == tt) {
            if (prev == nullptr) {
                head = curr->next;
                if (tail == curr) {
                    tail = nullptr;
                }
            } else {
                prev->next = curr->next;
                if (tail == curr) {
                    tail = prev;
                }
            }
            delete curr;
            break;
        }
    }
}
template<typename T>
void* _queue<T>::Elem::operator new(size_t size){
    return MemAllocator::instance().mem_alloc(MemAllocator::sizeInBlocks(size));
}
template<typename T>
void _queue<T>::Elem::operator delete(void* addr){
    MemAllocator::instance().mem_free(addr);
}
template<typename T>
void* _queue<T>::operator new(size_t size){

    return MemAllocator::instance().mem_alloc(MemAllocator::sizeInBlocks(size));
}
template<typename T>
void _queue<T>::operator delete(void* addr){
    MemAllocator::instance().mem_free(addr);
}


#endif //_QUEUE_HPP


