//
// Created by os on 8/8/23.
//

#ifndef PROJECT_BASE_V1_1_SCHEDULER_H
#define PROJECT_BASE_V1_1_SCHEDULER_H
#include "TCB.h"
#include "MemoryAllocator.h"

class Scheduler{
public:
    static TCB* get();
    static void put(TCB*);
private:
    struct Elem{
        TCB* thread;
        Elem* next;
    };
    static Elem* head, *tail;

};

#endif //PROJECT_BASE_V1_1_SCHEDULER_H
