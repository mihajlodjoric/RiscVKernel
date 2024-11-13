//
// Created by os on 8/4/23.
//

#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/MemoryAllocator.h"
#include "../h/TCB.h"
#include "../h/riscv.h"
#include "../test/printing.hpp"

void Riscv::handleSysCall(){
    //read operation and arguments
    uint64 volatile operation, arg1, arg2, arg3, arg4;
    __asm__ volatile ("mv %[var], a0" : [var] "=r" (operation));
    __asm__ volatile ("mv %[var], a1" : [var] "=r" (arg1));
    __asm__ volatile ("mv %[var], a2" : [var] "=r" (arg2));
    __asm__ volatile ("mv %[var], a3" : [var] "=r" (arg3));
    __asm__ volatile ("mv %[var], a4" : [var] "=r" (arg4));

    uint64 volatile scause;
    scause = Riscv::r_scause();
    if(scause == 8 || scause == 9){
        //save sstatus and sepc registers
        uint64 volatile sstatus, sepc;
        sstatus = r_sstatus();
        sepc = r_sepc();

        //mem_alloc
        if(operation == 0x01){
            size_t size = arg1;
            void* ret = MemoryAllocator::instance()._malloc(size);
            __asm__ volatile ("mv a0, %[var]" : : [var] "r" (ret));
        }
        //mem_free
        else if(operation == 0x02){
            void* addr = (void*) arg1;
            int ret = MemoryAllocator::instance()._free(addr);
            __asm__ volatile ("mv a0, %[var]" : : [var] "r" (ret));
        }
        //thread_create
        else if(operation == 0x11){
            TCB** handle = (TCB**)arg1;
            TCB::Body start_routine = (TCB::Body)arg2;
            void* arg = (void*)arg3;
            uint64* stack_space = (uint64*)arg4;
            uint64 ret = TCB::createThread(handle, start_routine, arg, stack_space);
            __asm__ volatile ("mv a0, %[var]" : : [var] "r" (ret));
        }
        //thread_exit
        else if(operation == 0x12){
            TCB::finish();
            TCB::_dispatch();
        }
        //thread_dispatch
        else if(operation == 0x13){
            TCB::_dispatch();
        }
        //thread_join
        else if(operation == 0x14){
            TCB** handle = (TCB**)arg1;
            TCB::_join(handle);
        }
        //sem_open
        else if(operation == 0x21){
            _sem** handle = (_sem**)arg1;
            uint init = (uint)arg2;
            uint64 ret = _sem::_open(handle, init);
            __asm__ volatile ("mv a0, %[var]" : : [var] "r" (ret));
        }
        //sem_close
        else if(operation == 0x22){
            _sem* handle = (_sem*)arg1;
            uint64 ret = _sem::_close(handle);
            __asm__ volatile ("mv a0, %[var]" : : [var] "r" (ret));
        }
        //sem_wait
        else if(operation == 0x23){
            _sem* handle = (_sem*)arg1;
            uint64 ret = _sem::_wait(handle);
            __asm__ volatile ("mv a0, %[var]" : : [var] "r" (ret));
        }
        //sem_signal
        else if(operation == 0x24){
            _sem* handle = (_sem*)arg1;
            uint64 ret = _sem::_signal(handle);
            __asm__ volatile ("mv a0, %[var]" : : [var] "r" (ret));
        }
        else if(operation == 0x41){
            char ret = __getc();
            __asm__ volatile ("mv a0, %[var]" : : [var] "r" (ret));
        }
        else if(operation == 0x42){
            char c = (char)arg1;
            __putc(c);
        }
        //initialize user mode
        else if(operation == 0x51){
            w_sstatus(sstatus);
            mc_sstatus(1 << 8);
            w_sepc(sepc + 4);
            mc_sip(SIP_SSIE);
            return;
        }

        //restore sstatus and sepc registers
        w_sstatus(sstatus);
        w_sepc(sepc + 4);
        mc_sip(SIP_SSIE);
    }
}

void Riscv::handleTimer(){
    Riscv::mc_sip(Riscv::SIP_SSIE);
}

void Riscv::handleConsole(){
    console_handler();
}

void Riscv::popSppSpie() {
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}
