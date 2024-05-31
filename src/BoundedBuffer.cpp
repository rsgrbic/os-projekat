//
// Created by os on 5/29/24.
//

#include "../h/BoundedBuffer.h"

#include "../h/MemAllocator.h"
#include "../h/_semaphore.h"

char BoundedBuffer::getItem()
{
    _sem::sem_wait(itemReady);
    char c=content[front++];
    front=front%size;
    numAvailable--;
    _sem::sem_signal(spaceReady);
    return c;
}

void BoundedBuffer::putItem(char c)
{
    _sem::sem_wait(spaceReady);
    content[back++]=c;
    back=back%size;
    numAvailable++;
    _sem::sem_signal(itemReady);
}

BoundedBuffer::BoundedBuffer()
{   size=maxSize;
    front=back=numAvailable=0;
    content=(char*)MemAllocator::instance().mem_alloc(MemAllocator::sizeInBlocks(size));
    _sem::sem_open(&itemReady,0);
    _sem::sem_open(&spaceReady,size);
}

void* BoundedBuffer::operator new(size_t size)
{
    return MemAllocator::instance().mem_alloc(MemAllocator::sizeInBlocks(size));
}

void BoundedBuffer::operator delete(void* addr)
{
    MemAllocator::instance().mem_free(addr);
}
