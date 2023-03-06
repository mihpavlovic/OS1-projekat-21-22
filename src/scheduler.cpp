//
// Created by os on 8/17/22.
//

#include "../h/scheduler.hpp"
Scheduler::Elem* Scheduler::head=nullptr;
Scheduler::Elem* Scheduler::tail=nullptr;

TCB *Scheduler::get() {
    if(head ==nullptr){return nullptr;}
    TCB* toReturn = head->thread;
    Elem* toFree = head;
    head = head->next;
    if(head == nullptr) {tail = nullptr;}
    toFree->next = nullptr;
    MemoryAllocator::kfree((void*)toFree);
    return toReturn;
}

void Scheduler::put(TCB* toPut) {
    size_t blk = MemoryAllocator::calculateBlocks(sizeof(Scheduler::Elem));
    Elem* toAdd = (Elem*) MemoryAllocator::kmalloc(blk);
    toAdd->thread = toPut;
    toAdd->next = nullptr;
    if(head ==nullptr){
        head = toAdd;
        tail = toAdd;
    }
    else if(head == tail){
        head->next = toAdd;
        tail = toAdd;
    }
    else {
        tail->next = toAdd;
        tail = tail->next;
    }



}
