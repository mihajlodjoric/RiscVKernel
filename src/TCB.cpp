//
// Created by os on 8/8/23.
//

#include "../h/TCB.h"
#include "../h/Scheduler.h"
#include "../h/MemoryAllocator.h"
#include "../h/riscv.h"
#include "../h/syscall_cpp.hpp"

TCB* TCB::running = nullptr;

void TCB::putWaiting(TCB *thread) {
    Waiting* newW = (Waiting*) MemoryAllocator::instance()._malloc((sizeof(Waiting) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    newW->thread = thread;
    newW->next = nullptr;
    if(tail == nullptr) head = tail = newW;
    else tail = tail->next = newW;
}

TCB* TCB::getWaiting(){
    if(head == nullptr) return nullptr;
    Waiting* oldW = head;
    TCB* thread = oldW->thread;
    head = head->next;
    if(head == nullptr) tail = nullptr;
    MemoryAllocator::instance()._free(oldW);
    return thread;
}

bool TCB::hasThreadWaiting() {
    return head != nullptr;
}

TCB* TCB::newThread(Body body, void* arg, uint64* stack) {
    TCB* tcb = (TCB*) MemoryAllocator::instance()._malloc((sizeof(TCB) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    tcb->body = body;
    tcb->arg = arg;
    if(body != nullptr) tcb->stack = stack;
    else tcb->stack = nullptr;
    tcb->context = {
            (uint64) &threadWrapper,
            stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0
    };
    tcb->finished = false;
    tcb->blocked = false;
    tcb->head = nullptr; tcb->tail = nullptr;
    return tcb;
}


int TCB::createThread(thread_t* handle, Body body, void *arg, uint64* stack) {
    TCB* thread = newThread(body, arg, stack);
    *handle = thread;
    if(body != nullptr) Scheduler::put(thread);
    return *handle == nullptr ? -1 : 0;
}

void TCB::_dispatch() {
    TCB* old = running;
    if(!running->isFinished() && !running->isBlocked())  Scheduler::put(running);
    running = Scheduler::get();
    contextSwitch(&old->context, &running->context);
}

void TCB::_join(thread_t* handle) {
    TCB* thread = *handle;
    if(thread->isFinished()) return;
    thread->putWaiting(running);
    TCB* old = running;
    running = Scheduler::get();
    contextSwitch(&old->context, &running->context);
}

void TCB::finish() {
    running->setFinished(true);
    while(running->hasThreadWaiting()){
        TCB* thread = running->getWaiting();
        Scheduler::put(thread);
    }
}

void TCB::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    thread_exit();
}

