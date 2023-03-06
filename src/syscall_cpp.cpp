//
// Created by os on 9/5/22.
//

#include "../h/syscall_cpp.hpp"

int Thread::start() {
    if(myHandle){
        if(myHandle->isStarted() == false){
            myHandle->setStarted(true);
            Scheduler::put(myHandle);
            return 0;
        }
        else return -1;
    }
    else{
        thread_create(&myHandle, wrapper, (void*)this);
        return 0;
    }

}

void Thread::wrapper(void* param) {
    Thread* toRun = (Thread*) param;
    toRun->run();
}

Thread::Thread(void (*body)(void *), void *arg) {
    thread_create_no_start(&myHandle, body, arg);
}

Thread::Thread() {
    //thread_create_no_start(&myHandle, wrapper, nullptr);
    myHandle = nullptr;
}

void Thread::dispatch() {
    thread_dispatch();
}

Thread::~Thread() { }
