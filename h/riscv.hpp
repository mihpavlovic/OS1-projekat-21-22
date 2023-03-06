//
// Created by os on 8/18/22.
//

#ifndef PROJECT_BASE_RISCV_HPP
#define PROJECT_BASE_RISCV_HPP
#include "../lib/hw.h"

class Riscv {
public:
    //pop x3..x31 from stack
    static void popRegisters();//implementirano u pushPopReg.S

    //push x3..x31 to stack
    static void pushRegisters();//implementirano u pushPopReg.S

    enum BitMaskSip{
        SIP_SSIP = (1 << 1),
        SIP_STIP = (1 << 5),
        SIP_SEIP = (1 << 9)
    };
    enum BitMaskSstatus{
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8)
    };

    static void ms_sip(uint64 mask);

    static void mc_sip(uint64 mask);

    static void popSppSpie();

    static void w_scause(uint64 sc);

    static uint64 r_scause();

    static void w_sepc(uint64 se);

    static uint64 r_sepc();

    static uint64 r_sstatus();

    static void w_sstatus(uint64 ss);

};

inline void Riscv::mc_sip(uint64 mask)
{
    __asm__ volatile ("csrc sip, %[arg]" : : [arg] "r"(mask));
}

inline void Riscv::ms_sip(uint64 mask)
{
    __asm__ volatile ("csrs sip, %[arg]" : : [arg] "r"(mask));
}

inline void Riscv::w_scause(uint64 sc) {
    __asm__ volatile("csrw scause, %[value]": :[value] "r" (sc));
}

inline uint64 Riscv::r_scause() {
    uint64 volatile scauseVar;
    __asm__ volatile("csrr %[value], scause": [value] "=r" (scauseVar));
    return scauseVar;
}

inline void Riscv::w_sepc(uint64 se) {
    __asm__ volatile("csrw sepc, %[value]": :[value] "r" (se));
}

inline uint64 Riscv::r_sepc() {
    uint64 volatile sepcVar;
    __asm__ volatile("csrr %[value], sepc": [value] "=r" (sepcVar));
    return sepcVar;
}

inline uint64 Riscv::r_sstatus(){
    uint64 volatile sstatusVar;
    __asm__ volatile ("csrr %[var], sstatus" : [var] "=r"(sstatusVar));
    return sstatusVar;
}

inline void Riscv::w_sstatus(uint64 ss){
    __asm__ volatile ("csrw sstatus, %[sstatusVar]" : : [sstatusVar] "r"(ss));
}



#endif //PROJECT_BASE_RISCV_HPP
