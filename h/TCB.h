//
// Created by os on 8/8/23.
//

#ifndef PROJECT_BASE_V1_1_TCB_H
#define PROJECT_BASE_V1_1_TCB_H
#include "../lib/hw.h"
#include "../h/riscv.h"
#include "../h/_sem.h"


class TCB{
public:
    using Body = void (*)(void*);
    typedef TCB* thread_t;
    static int createThread(thread_t* handle, Body body, void* arg, uint64* stack_space);

    ~TCB(){ delete stack; }
    static TCB* running;
    void setBlocked(bool blocked){this->blocked = blocked;}
    bool isBlocked() const{return blocked;}
    void setFinished(bool finished){this->finished = finished;}
    bool isFinished() const{return finished;}
private:
    friend Riscv;
    Body body;
    void* arg;
    uint64* stack;
    struct Context{
        uint64 ra;
        uint64 sp;
    };
    Context context;
    bool finished, blocked;
    struct Waiting{
        TCB* thread;
        Waiting* next;
    };
    Waiting* head, *tail;

    void putWaiting(TCB* thread);
    TCB* getWaiting();
    bool hasThreadWaiting();

    static TCB* newThread(Body body, void* arg, uint64* stack);
    static void _dispatch();
    static void _join(thread_t* handle);

    static void threadWrapper();
    static void finish();
    static void contextSwitch(Context* old, Context* running);
};

#endif //PROJECT_BASE_V1_1_TCB_H
