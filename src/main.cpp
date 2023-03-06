//#include "../lib/hw.h"
//#include "../h/syscall_c.h"

#include "../h/memoryAllocator.hpp"
#include "../h/tcb.hpp"
#include "../test/printing.h"
//#include "../h/scheduler.hpp"
//#include "../h/print.hpp"

#include "../h/syscall_cpp.hpp"





extern "C" void interruptAssembler();

void userMain();

void userMainWrapper(void* arg){
    //goToUserMode();
    userMain();
    //goToSupervisorMode();
    //thread_dispatch();
}

void main(){

    __asm__ volatile("csrw stvec, %[vector]" : : [vector] "r" (&interruptAssembler));
    MemoryAllocator::prepareAllocator();
    //userMain();
    TCB* kernel = nullptr;
    TCB* userMainVar = nullptr;
    thread_create(&kernel, nullptr, nullptr);
    TCB::running = kernel;
    printString("kernel nit napravljena\n");

    goToUserMode();
    thread_create(&userMainVar, &userMainWrapper, nullptr);
    //goToSupervisorMode();
    //userMain();
    //userMainWrapper(nullptr);
    //Threads_CPP_API_test();
    while(TCB::getUserThreadCount()!=0){
        thread_dispatch();
    }
    goToSupervisorMode();
    delete userMainVar;
    delete kernel;
    printString("EVERYTHING FINISHED\n");


}