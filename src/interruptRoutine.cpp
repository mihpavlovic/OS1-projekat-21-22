//
// Created by os on 6/26/22.
//
#include "../lib/hw.h"
#include "../h/memoryAllocator.hpp"
#include "../lib/console.h"
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../test/printing.h"
enum WhatCalled{
    m_alloc = 0x01,
    m_free = 0x02,
    t_create = 0x11,
    t_exit = 0x12,
    t_dispatch = 0x13,
    t_create_no_start = 0x14,
    goto_user = 0x15,
    goto_super = 0x16
};

//
// prenos argumenata, samo preko a registara
//

extern "C" void interruptRoutineHandler(){
    //prvo moram preko scause da ustanovim sto sam tu usao uopste


    //__putc('P');
   //__putc('\n');
    uint64 volatile sstatusVar;
    uint64 volatile scauseVar;
    uint64 volatile sepcVar;
    volatile uint16 sysCallCode = 0;
    sstatusVar = Riscv::r_sstatus();
    scauseVar = Riscv::r_scause(); //__asm__ volatile ("csrr %0, scause": "=r" (scauseVar));

    if(scauseVar == (0x01UL << 63 | 0x1)){ // prekid za tajmer
        Riscv::mc_sip(Riscv::SIP_SSIP);
    }
    else if(scauseVar == (0x01UL << 63 | 0x9)){ //spoljasnji hardverski prekid
        console_handler();
    }
    else if(scauseVar == (0x00UL << 63 | 0x2)){ //ilegalna instrukcija

    }
    else if(scauseVar == (0x00UL << 63 | 0x5)){ //nedozvoljena adresa citanja

    }
    else if(scauseVar == (0x00UL << 63 | 0x7)){ //nedozvoljena adresa upisa

    }
    else if(scauseVar == 8 || scauseVar == 9){ //ecall iz korisnickog ili sistemskog rezima
        //__putc('L');
        //__putc('\n');

        __asm__ volatile("mv %[code], a0" : [code] "=r" (sysCallCode));
        if(sysCallCode == WhatCalled::m_alloc){


            size_t size;
            __asm__ volatile("mv %[arg], a1" : [arg] "=r" (size));
            size = MemoryAllocator::calculateBlocks(size);
            void* returning = MemoryAllocator::kmalloc(size);//izvrsenje funkcije

            //pakovanje za vracanje rezultata
//            __putc('K');
//            __putc('\n');
            __asm__ volatile("mv a0, %[result]": : [result] "r" (returning));

            //podesavanje sepc
            sepcVar = Riscv::r_sepc();//__asm__ volatile("csrr %[var], sepc": [var] "=r" (sepcVar));
            sepcVar = sepcVar+4;
            Riscv::w_sepc(sepcVar);//__asm__ volatile("csrw sepc, %[var]": : [var] "r" (sepcVar));

            __asm__ volatile("sd a0, 0x50(s0)" : );//kad bude vracao sa steka da ne vrati staru vec novu vrednost
        }
        else if(sysCallCode == WhatCalled::m_free){
            void* addr;
            __asm__ volatile("mv %[arg], a1" : [arg] "=r" (addr));

            int returning = MemoryAllocator::kfree(addr);//izvrsenje funkcije

//            __putc('K');
//            __putc('\n');

            //pakovanje za vracanje rezultata
            __asm__ volatile("mv a0, %[result]": : [result] "r" (returning));

            //podesavanje sepc
            sepcVar = Riscv::r_sepc();//__asm__ volatile("csrr %[var], sepc": [var] "=r" (sepcVar));
            sepcVar = sepcVar+4;
            Riscv::w_sepc(sepcVar);//__asm__ volatile("csrw sepc, %[var]": : [var] "r" (sepcVar));

            __asm__ volatile("sd a0, 0x50(s0)");//kad bude vracao sa steka da ne vrati staru vec novu vrednost
        }
        else if(sysCallCode == WhatCalled::t_create){
            //__putc('U');
           //__putc('\n');
            TCB** handle;
            void (*routine)(void*);
            void* args;
            __asm__ volatile("mv %[arg], a1" : [arg] "=r" (handle));
            __asm__ volatile("mv %[arg], a2" : [arg] "=r" (routine));
            __asm__ volatile("mv %[arg], a3" : [arg] "=r" (args));
            *handle = TCB::createThread(routine, args);

            int returning;
            if(*handle ==nullptr) returning = -1;
            else returning = 0;
            __asm__ volatile("mv a0, %[result]": : [result] "r" (returning));

            //podesavanje sepc
            sepcVar = Riscv::r_sepc();
            sepcVar = sepcVar+4;
            Riscv::w_sepc(sepcVar);

            __asm__ volatile("sd a0, 0x50(s0)");//kad bude vracao sa steka da ne vrati staru vec novu vrednost

        }
        else if(sysCallCode == WhatCalled::t_exit){
            TCB::running->setFinished(true);
            TCB::dispatch();

            //podesavanje sepc
            sepcVar = Riscv::r_sepc();
            sepcVar = sepcVar+4;
            Riscv::w_sepc(sepcVar);
        }
        else if(sysCallCode == WhatCalled::t_dispatch){

            TCB::dispatch();

            //podesavanje sepc
            sepcVar = Riscv::r_sepc();
            sepcVar = sepcVar+4;
            Riscv::w_sepc(sepcVar);
        }
        else if(sysCallCode == WhatCalled::t_create_no_start){
            //__putc('U');
            //__putc('\n');
            TCB** handle;
            void (*routine)(void*);
            void* args;
            __asm__ volatile("mv %[arg], a1" : [arg] "=r" (handle));
            __asm__ volatile("mv %[arg], a2" : [arg] "=r" (routine));
            __asm__ volatile("mv %[arg], a3" : [arg] "=r" (args));
            *handle = TCB::createThread(routine, args);

            int returning;
            if(*handle ==nullptr) returning = -1;
            else returning = 0;
            __asm__ volatile("mv a0, %[result]": : [result] "r" (returning));

            //podesavanje sepc
            sepcVar = Riscv::r_sepc();
            sepcVar = sepcVar+4;
            Riscv::w_sepc(sepcVar);

            __asm__ volatile("sd a0, 0x50(s0)");//kad bude vracao sa steka da ne vrati staru vec novu vrednost
        }
        else if(sysCallCode == WhatCalled::goto_user){
            __asm__ volatile("csrc sstatus, %[arg]" : : [arg] "r"(0x100));
            //podesavanje sepc
            sepcVar = Riscv::r_sepc();
            sepcVar = sepcVar+4;
            Riscv::w_sepc(sepcVar);
        }
        else if(sysCallCode == WhatCalled::goto_super){
            __asm__ volatile("csrs sstatus, %[arg]" : : [arg] "r"(0x100));
            //podesavanje sepc
            sepcVar = Riscv::r_sepc();
            sepcVar = sepcVar+4;
            Riscv::w_sepc(sepcVar);
        }
        else{
            printString("ERROR scause: ");
            printInt(Riscv::r_scause());
            printString("\n");
        }
        if(sysCallCode != 0x15 && sysCallCode != 0x16) Riscv::w_sstatus(sstatusVar);

    }

    //if(sysCallCode != 0x15 && sysCallCode != 0x16) Riscv::w_sstatus(sstatusVar);
    //console_handler();
    //
    // ne znam kako konzola radi sa prekidima
    //





}