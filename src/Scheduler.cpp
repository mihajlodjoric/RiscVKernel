//
// Created by os on 8/8/23.
//

#include "../h/Scheduler.h"

Scheduler::Elem* Scheduler::head = nullptr;
Scheduler::Elem* Scheduler::tail = nullptr;

void Scheduler::put(TCB *thread) {
    Elem* newE = (Elem*) MemoryAllocator::instance()._malloc((sizeof(Elem) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    newE->thread = thread;
    newE->next = nullptr;
    if(tail == nullptr) head = tail = newE;
    else tail = tail->next = newE;
}

TCB* Scheduler::get(){
    if(head == nullptr) return nullptr;
    Elem* oldE = head;
    TCB* thread = oldE->thread;
    head = head->next;
    if(head == nullptr) tail = nullptr;
    MemoryAllocator::instance()._free(oldE);
    return thread;
}

