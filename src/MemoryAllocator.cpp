//
// Created by os on 7/31/23.
//

#include "../h/MemoryAllocator.h"

MemoryAllocator& MemoryAllocator::instance() {
    static MemoryAllocator instance;
    return instance;
}

void* MemoryAllocator::_malloc(size_t size)  {
    static bool init = true;
    if(init){
        init = false;
        head = (FreeMem*)((uint64*)HEAP_START_ADDR);
        head->prev = nullptr;
        head->next = nullptr;
        head->size = (size_t)(((uint64*)HEAP_END_ADDR - (uint64*)HEAP_START_ADDR) / MEM_BLOCK_SIZE);
    }
    void* addr = nullptr;
    for(FreeMem* curr = head; curr != nullptr; curr = curr->next){
        if(curr->size >= size){
            addr = curr;
            if(curr->size - size == 0){
                if(curr->prev) curr->prev->next = curr->next;
                else head = curr->next;
                if(curr->next) curr->next->prev = curr->prev;
            }
            else{
                FreeMem* newSeg = (FreeMem*)((uint64*)curr + size * MEM_BLOCK_SIZE);
                newSeg->size = curr->size - size;
                newSeg->prev = curr->prev;
                newSeg->next = curr->next;
                if(curr->prev) curr->prev->next = newSeg;
                else head = newSeg;
                if(curr->next) curr->next->prev = newSeg;
            }
            curr->size = size;
            break;
        }
    }
    return (uint64*)addr + sizeof(FreeMem);
}

void MemoryAllocator::tryToJoin(FreeMem* seg) {
    if(seg != nullptr && seg->next != nullptr && (uint64*)seg + seg->size * MEM_BLOCK_SIZE == (uint64*)(seg->next)){
        seg->size += seg->next->size;
        seg->next = seg->next->next;
        if(seg->next) seg->next->prev = seg;
    }
}

int MemoryAllocator::_free(void* addr) {
    if((uint64*)addr < (uint64*)HEAP_START_ADDR || (uint64*)addr > (uint64*)HEAP_END_ADDR) return -1;
    FreeMem* newSeg = (FreeMem*)((uint64*)addr - sizeof(FreeMem)), *curr;
    if(head == nullptr || (uint64*)newSeg < (uint64*)head) curr = nullptr;
    else{
        for(curr = head; curr->next != nullptr && (uint64*)newSeg < (uint64*)curr ; curr = curr->next);
    }
    newSeg->prev = curr;
    if(curr) newSeg->next = curr->next;
    else newSeg->next = head;

    if(newSeg->next) newSeg->next->prev = newSeg;
    if(curr) curr->next = newSeg;
    else head = newSeg;

    tryToJoin(newSeg);
    tryToJoin(curr);
    return 0;
}
