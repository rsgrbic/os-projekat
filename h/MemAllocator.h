

#ifndef MEMALLOCATOR_H
#define MEMALLOCATOR_H
#include"../lib/hw.h"


class MemAllocator {
public:
    struct FreeMem{
        size_t size;
        FreeMem *next;
        FreeMem *prev;
    };

    MemAllocator(const MemAllocator&) =delete;

	MemAllocator& operator=(const MemAllocator&) =delete;

    void*  mem_alloc(size_t size);
    static size_t sizeInBlocks(size_t byteSize);
    long mem_free(void* addr);

    static MemAllocator& instance();


private:
    static int tryToJoin(FreeMem* cur);
    ~MemAllocator()=default;

    MemAllocator();
    static FreeMem* freememHead;
};



#endif //MEMALLOCATOR_H
