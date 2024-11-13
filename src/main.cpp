//
// Created by os on 8/1/23.
//
#include "../h/MemoryAllocator.h"
#include "../h/syscall_c.h"
#include "../h/riscv.h"
#include "../h/TCB.h"
#include "../h/syscall_cpp.hpp"

extern "C" void interruptVector();
void userMain();
int main(){
    Riscv::w_stvec((uint64)&interruptVector);
    __asm__ volatile ("csrs stvec, 0x01");
    Riscv::w_sstatus(0x02);

    TCB* thread;
    thread_create(&thread, nullptr, nullptr);
    TCB::running = thread;
    changeToUserMode();

    userMain();
    
    return 0;


}