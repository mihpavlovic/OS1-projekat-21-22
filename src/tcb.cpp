//
// Created by os on 8/17/22.
//

#include "../h/tcb.hpp"

TCB* TCB::running = nullptr;
uint64 TCB::userThreadCount = 0;

void TCB::yield() {
    Riscv::pushRegisters();
    dispatch();
    Riscv::popRegisters();
}

void TCB::dispatch() {
    TCB* oldRun = TCB::running;
    if(!(oldRun->finished)){ Scheduler::put(oldRun);}
    running = Scheduler::get();
    contextSwitch(&(oldRun->context), &(running->context));

}

void TCB::threadWrapper() {
    //nesto sa spp i spie sto ne kontam, pukne kada odkomentarisem, videti sta se desava
    Riscv::popSppSpie();
    running->started = true;
    //if(running->isUserThread) goToUserMode();
    running->body(running->args);
    running->setFinished(true);
    if(running->isUserThread) userThreadCount--;
    thread_dispatch();//dispatch(); //yield(); pise na vezbama

}

TCB *TCB::createThread(TCB::Body body, void* args) {
    TCB* tmp = new TCB(body, args);

    if (body != nullptr) { Scheduler::put(tmp); }
    return tmp;
}

TCB *TCB::createThreadNoStart(TCB::Body body, void *args) {
    TCB* tmp = new TCB(body, args);
    return tmp;
}
