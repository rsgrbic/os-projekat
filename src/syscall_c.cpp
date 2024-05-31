#include "../h/syscall_c.h"
#include "../h/SYSCALL_enum.h"

typedef _thread* thread_t;

uint64 executeSyscall(uint64 ID,void* a1=nullptr,
                      void* a2=nullptr,
                      void* a3=nullptr,
                      void* a4=nullptr)
{
    uint64 volatile ret=0;
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[ret],a0":[ret]"=r"(ret));
    return ret;
}

void* mem_alloc(size_t size)
{   size_t sizeBlocks= size/MEM_BLOCK_SIZE + (size%MEM_BLOCK_SIZE!=0?1:0);
    return (void*)executeSyscall(SYSCALL_ID::MEM_ALLOC,(void*)sizeBlocks);
}

int mem_free(void* addr)
{
    return (int)executeSyscall(SYSCALL_ID::MEM_FREE,addr);
}

int thread_create(thread_t* handle,void(*start_routine)(void*),void* arg)
{
    void* memallocStack=nullptr;
      uint64 size =sizeof(uint64)*DEFAULT_STACK_SIZE;
        memallocStack=mem_alloc(size);
        if(memallocStack==nullptr)return -1;
        return (int)executeSyscall(SYSCALL_ID::THREAD_CREATE,(void*)handle,(void*)start_routine,(void*)arg,(void*)memallocStack);
}

int thread_exit()
{
   return (int) executeSyscall(SYSCALL_ID::THREAD_EXIT);
}

void thread_dispatch()
{
    executeSyscall(SYSCALL_ID::THREAD_DISPATCH);
}

int sem_open(sem_t* handle, unsigned init)
{
    return (int)executeSyscall(SYSCALL_ID::SEM_OPEN,(void*)handle,(void*)((uint64)init));
}


int sem_close(sem_t handle)
{
    return (int)executeSyscall(SYSCALL_ID::SEM_CLOSE,(void*)handle);
}

int sem_tryWait(sem_t id)
{
    return (int)executeSyscall(SYSCALL_ID::SEM_TRYWAIT,(void*)id);
}

int sem_timedWait(sem_t id,time_t timeout)
{
    return (int)executeSyscall(SYSCALL_ID::SEM_TIMEDWAIT,(void*)id,(void*)timeout);
}

int sem_signal(sem_t id)
{
    return (int)executeSyscall(SYSCALL_ID::SEM_SIGNAL,(void*)id);
}

int time_sleep(time_t time)
{
    return (int)executeSyscall(SYSCALL_ID::TIME_SLEEP,(void*)time);
}

int sem_wait(sem_t id)
{
    return (int)executeSyscall(SYSCALL_ID::SEM_WAIT,(void*)id);
}
char getc()
{
    return (char)executeSyscall(SYSCALL_ID::GETC);
}
void putc(char c)
{
    executeSyscall(SYSCALL_ID::PUTC,(void*)((uint64)c));
}

int join(thread_t t)
{
    return (int)executeSyscall(SYSCALL_ID::JOIN,(void*)t);
}







