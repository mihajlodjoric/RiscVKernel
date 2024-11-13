//
// Created by os on 8/3/23.
//

#include "../h/syscall_c.h"

void* mem_alloc(size_t size){
    size_t sz = (size + MEM_BLOCK_SIZE - 1)/MEM_BLOCK_SIZE;
    __asm__ volatile ("mv a1, %[var]" : : [var] "r" (sz));
    __asm__ volatile ("li a0, 0x01");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[value], a0" : [value] "=r" (ret));
    return (void*) ret;
}

int mem_free(void* addr){
    __asm__ volatile ("mv a1, %[value]" : : [value] "r" (addr));
    __asm__ volatile ("li a0, 0x02");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[value], a0" : [value] "=r" (ret));
    return (int) ret;
}

typedef TCB* thread_t;
int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){
    void* stack_space = mem_alloc(DEFAULT_STACK_SIZE);
    __asm__ volatile ("mv a4, %[value]" : : [value] "r" (stack_space));
    __asm__ volatile ("mv a3, %[value]" : : [value] "r" (arg));
    __asm__ volatile ("mv a2, %[value]" : : [value] "r" (start_routine));
    __asm__ volatile ("mv a1, %[value]" : : [value] "r" (handle));
    __asm__ volatile ("li a0, 0x11");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[value], a0" : [value] "=r" (ret));
    return (int) ret;
}

int thread_exit(){
    __asm__ volatile ("li a0, 0x12");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[value], a0" : [value] "=r" (ret));
    return (int) ret;
}

void thread_dispatch(){
    __asm__ volatile ("li a0, 0x13");
    __asm__ volatile ("ecall");
}

void thread_join(thread_t* handle){
    __asm__ volatile ("mv a1, %[value]" : : [value] "r" (handle));
    __asm__ volatile ("li a0, 0x14");
    __asm__ volatile ("ecall");
}

class _sem;
typedef _sem* sem_t;
int sem_open(sem_t* handle, uint init){
    __asm__ volatile ("mv a2, %[value]" : : [value] "r" (init));
    __asm__ volatile ("mv a1, %[value]" : : [value] "r" (handle));
    __asm__ volatile ("li a0, 0x21");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[value], a0" : [value] "=r" (ret));
    return (int) ret;
}

int sem_close(sem_t handle){
    __asm__ volatile ("mv a1, %[value]" : : [value] "r" (handle));
    __asm__ volatile ("li a0, 0x22");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[value], a0" : [value] "=r" (ret));
    return (int) ret;
}

int sem_wait(sem_t handle){
    __asm__ volatile ("mv a1, %[value]" : : [value] "r" (handle));
    __asm__ volatile ("li a0, 0x23");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[value], a0" : [value] "=r" (ret));
    return (int) ret;
}

int sem_signal(sem_t handle){
    __asm__ volatile ("mv a1, %[value]" : : [value] "r" (handle));
    __asm__ volatile ("li a0, 0x24");
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile ("mv %[value], a0" : [value] "=r" (ret));
    return (int) ret;
}

    char getc() {
        __asm__ volatile ("li a0, 0x41");
        __asm__ volatile ("ecall");
        uint64 ret;
        __asm__ volatile ("mv %[value], a0" : [value] "=r"(ret));
        return (char) ret;
    }

    void putc(char c) {
        __asm__ volatile ("mv a1, %[value]" : : [value] "r"(c));
        __asm__ volatile ("li a0, 0x42");
        __asm__ volatile ("ecall");
    }

    void time_sleep(time_t) {}

    void changeToUserMode() {
        __asm__ volatile ("li a0, 0x51");
        __asm__ volatile ("ecall");
    }
