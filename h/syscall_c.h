//
// Created by os on 8/3/23.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_C_H
#define PROJECT_BASE_V1_1_SYSCALL_C_H
#include "../lib/hw.h"

void* mem_alloc(size_t size);

int mem_free(void* addr);

class TCB;
typedef TCB* thread_t;
int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);

int thread_exit();

void thread_dispatch();

void thread_join(thread_t* handle);

class _sem;
typedef _sem* sem_t;
int sem_open(sem_t* handle, uint init);

int sem_close(sem_t handle);

int sem_wait(sem_t handle);

int sem_signal(sem_t handle);

char getc();

void putc(char c);

void time_sleep(time_t);

void changeToUserMode();

#endif //PROJECT_BASE_V1_1_SYSCALL_C_H
