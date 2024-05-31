//
// Created by os on 5/12/24.
//

#include "../h/MemAllocator.h"

MemAllocator::FreeMem* MemAllocator::freememHead = nullptr;


void* MemAllocator::mem_alloc(size_t size)
{
    if (size == 0)
    {
        return nullptr;
    }
    size_t reqSize=(size+1)*MEM_BLOCK_SIZE;
    FreeMem* cur;
    //First fit algoritam
    for (cur = freememHead; cur != nullptr; cur = cur->next)
    {
        if (cur->size >= reqSize) break;

    }
    if (cur == nullptr || cur->size < reqSize)
    {  
        return nullptr;
    }

    size_t remain = cur->size - reqSize;

    if (remain > 2 * MEM_BLOCK_SIZE)
    {
        //remain -= sizeof(FreeMem);
        FreeMem* newSeg = (FreeMem*)((char*)cur + reqSize);
        newSeg->size = remain;
        if (cur->prev == nullptr) { freememHead = newSeg; }
        else { cur->prev->next = newSeg; }
        newSeg->prev = cur->prev;
        if (cur->next != nullptr)cur->next->prev = newSeg;
        newSeg->next = cur->next;
    }
    else
    {
        if (cur->prev == nullptr){freememHead = cur->next;freememHead->prev=nullptr;}
        else {cur->prev->next = cur->next;cur->next->prev=cur->prev;}
    }
    cur->next = nullptr;
    cur->prev = nullptr;
    cur->size = reqSize;

    return (void*)((char*)cur +MEM_BLOCK_SIZE);
}

size_t MemAllocator::sizeInBlocks(size_t byteSize)
{
    return  byteSize/MEM_BLOCK_SIZE + (byteSize%MEM_BLOCK_SIZE!=0?1:0);
}

long MemAllocator::mem_free(void* addr)
{
    if (addr == nullptr)return 0;
    char* kaddr=(char*)addr - MEM_BLOCK_SIZE;
    FreeMem* cur;

    if (freememHead == nullptr || kaddr < (char*)(freememHead)) cur = nullptr;

    else for (cur = freememHead; cur->next != nullptr ; cur = cur->next)
    {
        if(kaddr < (char*)cur->next)break;
    }

    FreeMem* memSeg = (FreeMem*)kaddr;
    if (memSeg==nullptr)return -2;
    if (memSeg->next != nullptr || memSeg->prev != nullptr)
        return -1;

    if (cur == nullptr)
    {
        memSeg->next = freememHead;
        memSeg->prev = nullptr;
        cur = freememHead;
        if (freememHead)
        {
            freememHead->prev = memSeg;
        }
        freememHead = memSeg;
    }
    else
    {
        memSeg->next = cur->next;
        memSeg->prev = cur;
        if (cur->next != nullptr)
            cur->next->prev = memSeg;
        cur->next = memSeg;
    }


    tryToJoin(memSeg);
    tryToJoin(cur);

    return 0;
}

int MemAllocator::tryToJoin(FreeMem* cur)
{
    if (cur == nullptr)return -1;
    if (cur->next != nullptr && (char*)cur + cur->size  == (char*)cur->next)
    {
        cur->size += cur->next->size;
        cur->next = cur->next->next;
        if (cur->next)cur->next->prev = cur;
        return 0;
    }
    else return -1;
}

MemAllocator::MemAllocator()
{
    size_t start = (size_t)HEAP_START_ADDR + ((size_t)HEAP_START_ADDR % MEM_BLOCK_SIZE != 0
                                                  ? MEM_BLOCK_SIZE - (size_t)HEAP_START_ADDR % MEM_BLOCK_SIZE
                                                  : 0);
    this->freememHead = (FreeMem*)start;
    this->freememHead->next = nullptr;
    this->freememHead->prev = nullptr;
    this->freememHead->size = ((size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR - MEM_BLOCK_SIZE);
}

MemAllocator& MemAllocator::instance()
{
    static MemAllocator alloc;
    return alloc;
}
