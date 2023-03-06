//
// Created by os on 8/17/22.
//

#ifndef PROJECT_BASE_SCHEDULER_HPP
#define PROJECT_BASE_SCHEDULER_HPP

#include "./listTemplate.hpp"

class TCB;

class Scheduler {
    struct Elem{
        TCB* thread;
        Elem* next;
        Elem(TCB* t, Elem* n=nullptr): thread(t), next(n){}
    };
    static Elem* head;
    static Elem* tail;
public:
    static TCB* get();
    static void put(TCB* toPut);
};


#endif //PROJECT_BASE_SCHEDULER_HPP
