

#ifndef BOUNDEDBUFFER_H
#define BOUNDEDBUFFER_H
#include "../lib/hw.h"
class _sem;
class BoundedBuffer {
public:
    char getItem();
    void putItem(char c);
    BoundedBuffer();
    void* operator new(size_t size);
    void operator delete(void* addr);
    uint64 numAvailable;
    static constexpr uint64 maxSize=512;
private:
    char* content;
    uint64 size,front,back;
    _sem *itemReady,*spaceReady;

};



#endif //BOUNDEDBUFFER_H
