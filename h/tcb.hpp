//
// Created by os on 8/17/22.
//

#ifndef PROJECT_BASE_TCB_H
#define PROJECT_BASE_TCB_H

#include "./scheduler.hpp"
#include "../lib/hw.h"
#include "./riscv.hpp"
#include "./syscall_c.h"

class TCB {

public:
    using Body = void(*) (void*); //pokazivac na funkciju
    static TCB* createThread(Body , void* args);
    static TCB* createThreadNoStart(Body , void* args);
    static void dispatch(); //menja running i zove contextSwitch
    static void yield(); //prebacuje registre na stek, menja nit, skida od nove niti sa steka
    static uint64 getUserThreadCount(){return userThreadCount;}
    bool isFinished(){return finished;}
    void setFinished(bool f) {finished = f;}
    bool isStarted(){return started;}
    void setStarted(bool s) {started = s;}
    static TCB* running;


private:
    struct Context{
        uint64 ra;
        uint64 sp;
    };
    explicit TCB (Body bd,void* a){
        body = bd;
        finished = false;
        started = false;
        blocked = false;
        isUserThread = false;
        args = a;
        context = {0, 0};
        stack = nullptr;
        if(body!=nullptr){
            this->stack = new uint64 [DEFAULT_STACK_SIZE];//(uint64*)mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);
            this->context.ra = (uint64) &threadWrapper;//(uint64) this->body;
            this->context.sp = (uint64) &((this->stack)[DEFAULT_STACK_SIZE]);
            isUserThread = true;
            userThreadCount++;
        }

    }
    static uint64 userThreadCount;
    Context context;
    //thread_t handle;
    Body body;
    uint64* stack;
    bool finished;
    bool blocked;
    bool started;
    bool isUserThread;
    void* args; // argumenti za body koji se raspakuju u threadWrapper

    static void threadWrapper();

    static void contextSwitch(Context* oldC, Context* newC); //vrsi zamenu konteksta(Context)
    //contextSwitch ubacuje iz masine ra i sp u oldC (kontekst od prethodno izvrsavane niti)
    //a u masinu ubacuje ra i sp iz newC (kontekst od niti koja je sada postala running)

    static uint64 constexpr STACK_SIZE = 1024;
};


#endif //PROJECT_BASE_TCB_H
