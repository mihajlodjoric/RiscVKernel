//
// Created by os on 8/16/23.
//

#include "../h/_sem.h"
#include "../h/Scheduler.h"

void _sem::putBlocked(TCB *thread){
    Blocked* newB = (Blocked*) MemoryAllocator::instance()._malloc((sizeof(Blocked) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    newB->thread = thread;
    newB->next = nullptr;
    if(tail == nullptr) head = tail = newB;
    else tail = tail->next = newB;
}

TCB* _sem::getBlocked(){
    if(head == nullptr) return nullptr;
    Blocked* oldB = head;
    TCB* thread = oldB->thread;
    head = head->next;
    if(head == nullptr) tail = nullptr;
    MemoryAllocator::instance()._free(oldB);
    return thread;
}

bool _sem::hasThreadBlocked(){
    return head != nullptr;
}


_sem* _sem::newSem(uint init) {
    _sem* newSem = (_sem*) MemoryAllocator::instance()._malloc((sizeof(_sem) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    newSem->init = init;
    newSem->nRemaining = 0;
    newSem->head = nullptr; newSem->tail = nullptr;
    return newSem;
}

int _sem::_open(sem_t *handle, uint init) {
    _sem* sem = newSem(init);
    *handle = sem;
    return *handle == nullptr ? -1 : 0;
}

int _sem::_close(sem_t handle) {
    while(handle->hasThreadBlocked()){
        TCB* thread = handle->getBlocked();
        thread->setBlocked(false);
        Scheduler::put(thread);
        handle->nRemaining++;
    }
    if(handle->nRemaining == 0) MemoryAllocator::instance()._free(handle);
    return 0;
}

int _sem::_wait(sem_t handle) {
    if(--handle->init < 0){
        handle->putBlocked(TCB::running);
        TCB::running->setBlocked(true);
        thread_dispatch();
        if(handle->nRemaining > 0){
            if(--handle->nRemaining == 0) MemoryAllocator::instance()._free(handle);
            return -2;
        }
    }
    return 0;
}

int _sem::_signal(sem_t handle) {
    if(++handle->init <= 0){
        TCB* thread = handle->getBlocked();
        thread->setBlocked(false);
        Scheduler::put(thread);
    }
    return 0;
}