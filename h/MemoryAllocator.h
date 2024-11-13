//
// Created by os on 7/31/23.
//

#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_H
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_H
#include "../lib/hw.h"

struct FreeMem {
    FreeMem* next;
    FreeMem* prev;
    size_t size;
};

class MemoryAllocator {
    FreeMem* head = nullptr;
    void tryToJoin(FreeMem*);
public:
    static MemoryAllocator& instance();
    void* _malloc(size_t size);
    int _free(void* addr);
};

#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_H
