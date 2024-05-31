#ifndef SYSCALL_C_H
#define SYSCALL_C_H
#include"../lib/hw.h"

class _thread;
class _sem;
typedef _thread* thread_t;
typedef _sem* sem_t;
void *mem_alloc(size_t size);
int mem_free(void* addr);
int thread_create(thread_t* handle,void (*start_routine)(void*),void* arg);
int thread_exit();
void thread_dispatch();
int sem_open ( sem_t* handle,unsigned init);
int sem_close(sem_t handle);
int sem_tryWait(sem_t id);
int sem_wait(sem_t id);
int sem_timedWait(sem_t id,time_t timeout);
int sem_signal(sem_t id);
int time_sleep(time_t time);
char getc();
void putc(char c);
int join(thread_t t);
#endif