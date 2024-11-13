//
// Created by os on 8/15/23.
//

#include "../h/syscall_cpp.hpp"

void* operator new(size_t size){
    return mem_alloc(size);
}
void operator  delete(void* addr) noexcept{
    if(addr != nullptr) mem_free(addr);
}

Thread::Thread(){
    this->body = runWrapper;
    this->arg = this;
}

Thread::Thread(void (*body)(void *), void *arg) {
    this->body = body;
    this->arg = arg;
}

int Thread::start() {
    return thread_create(&myHandle, body, arg);
}

Thread::~Thread() {
    myHandle = nullptr;
}

void Thread::runWrapper(void* arg) {
    Thread* thread = (Thread*) arg;
    thread->run();
}

void Thread::join() {
    thread_join(&myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}


Semaphore::Semaphore(uint init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}
int Semaphore::wait() {
    return sem_wait(myHandle);
}
int Semaphore::signal() {
    return sem_signal(myHandle);
}


