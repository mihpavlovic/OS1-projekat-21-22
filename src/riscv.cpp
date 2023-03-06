//
// Created by os on 8/18/22.
//

#include "../h/riscv.hpp"


void Riscv::popSppSpie() {
    //__asm__ volatile("csrc sstatus, %[arg]" : : [arg] "r"(0x100));
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}