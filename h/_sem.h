//
// Created by os on 8/16/23.
//

#ifndef PROJECT_BASE_V1_1__SEM_H
#define PROJECT_BASE_V1_1__SEM_H

#include "../lib/hw.h"
#include "../h/TCB.h"
#include "syscall_c.h"

class _sem{
public:
    typedef _sem* sem_t;
    static int _open(sem_t* handle, uint init);
    static int _close(sem_t handle);
    static int _wait(sem_t handle);
    static int _signal(sem_t handle);
private:
    static _sem* newSem(uint init);
    int init;
    int nRemaining;
    struct Blocked{
        TCB* thread;
        Blocked* next;
    };
    Blocked* head, *tail;
    void putBlocked(TCB* thread);
    TCB* getBlocked();
    bool hasThreadBlocked();
};

#endif //PROJECT_BASE_V1_1__SEM_H
