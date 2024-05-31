//
// Created by os on 5/21/24.
//

#include "../h/timedList.h"

_thread* timedList::remove()
{
    if(head==nullptr)return nullptr;
    Node* Node=head;
    head=head->next;
    _thread* t=Node->thread;
    delete Node;
    return t;

}

void timedList::dec()
{   if(head==nullptr)return;
    head->waitDelay--;
}

bool timedList::notEmpty()
{return head==nullptr;
}

void timedList::deleteThis(_thread* thr)
{
    Node* prev = nullptr;


    for(Node* curr = head;curr!=nullptr;prev=curr,curr=curr->next) {

        if(curr->thread==thr)
        {   if(prev!=nullptr)
            {
                prev->next=curr->next;
            }
            else
            {
                head=head->next;
            }
            delete curr;
            break;

        }
    }
}

void timedList::add(_thread* thr, uint64 delay)
{   Node * newElem=new Node(thr,delay);
    if (head==nullptr)
    {

        head=newElem;
    }
    else
    {
        Node * curr=head->next,*prev=head;
        if(delay<head->waitDelay)
        {
            newElem->next=head;
            head->waitDelay-=delay;
            head=newElem;
        }
        else
        {
            uint64 sum=prev->waitDelay;
            while(curr!=nullptr)
            {
                if(delay<sum+curr->waitDelay)break;
                sum+=curr->waitDelay;
                prev=curr;
                curr=curr->next;
            }
            if(curr!=nullptr)
            {
                //  if(sum==delay)
                //
                // {
                //      prev->next=newElem;
                //      newElem->next=prev->next;
                //      newElem->waitDelay=0;
                // }
                //else
                {
                    prev->next=newElem;
                    newElem->next=curr;
                    newElem->waitDelay=delay-sum;
                    curr->waitDelay=sum+curr->waitDelay-delay;

                }
            }
            else
            {
                prev->next=newElem;
                newElem->waitDelay=delay-sum;
            }
        }
    }
}
void* timedList::operator new(size_t size){
    return MemAllocator::instance().mem_alloc(MemAllocator::sizeInBlocks(size));
}

void timedList::operator delete(void *addr){
    MemAllocator::instance().mem_free(addr);
}

void* timedList::Node::operator new(size_t size){
    return MemAllocator::instance().mem_alloc(MemAllocator::sizeInBlocks(size));
}

void timedList::Node::operator delete(void *addr){
    MemAllocator::instance().mem_free(addr);
}
