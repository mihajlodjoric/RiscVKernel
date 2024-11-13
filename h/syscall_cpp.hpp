//
// Created by os on 8/15/23.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
#define PROJECT_BASE_V1_1_SYSCALL_CPP_HPP

#include "../h/syscall_c.h"
#include "../lib/console.h"

void* operator new (size_t);
void operator delete (void*) noexcept;

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread();

    int start();

    void join();

    static void dispatch();

    static void sleep(time_t){}

protected:
    Thread();
    virtual void run(){}

private:
    thread_t myHandle;
    void (*body)(void*); void* arg;
    static void runWrapper(void* arg);
};

class Semaphore{
public:
    Semaphore(uint init = 1);
    virtual ~Semaphore();

    int wait();
    int signal();
private:
    sem_t myHandle;
};

class Console{
public:
    static char getc(){ return __getc(); }
    static void putc(char c){ __putc(c); }
};

#endif //PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
