.global interruptVector
.align 4

interruptVector:
    j sysCallHandler
    j timeHandler
    mv t0, t0
    mv t0, t0
    mv t0, t0
    mv t0, t0
    mv t0, t0
    mv t0, t0
    mv t0, t0
    j consoleHandler


sysCallHandler:
    addi sp, sp, -256
    .irp index, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    sd x\index, \index * 8(sp);
    .endr

    call _ZN5Riscv13handleSysCallEv

    .irp index, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    ld x\index, \index * 8(sp);
    .endr
    addi sp, sp, 256
    sret


timeHandler:
    addi sp, sp, -256
    .irp index, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    sd x\index, \index * 8(sp);
    .endr

    call _ZN5Riscv11handleTimerEv

    .irp index, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    ld x\index, \index * 8(sp);
    .endr
    addi sp, sp, 256
    sret


consoleHandler:
   addi sp, sp, -256
   .irp index, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
   sd x\index, \index * 8(sp);
   .endr

   call _ZN5Riscv13handleConsoleEv

   .irp index, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
   ld x\index, \index * 8(sp);
   .endr
   addi sp, sp, 256
   sret