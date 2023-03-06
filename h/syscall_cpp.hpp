//
// Created by os on 9/5/22.
//

#ifndef PROJECT_BASE_SYSCALL_CPP_HPP
#define PROJECT_BASE_SYSCALL_CPP_HPP

#include "syscall_c.h"

void* operator new(size_t);
void operator delete(void*) noexcept;
void* operator new[](size_t);
void operator delete[](void*) noexcept;

class Thread{
public:
    Thread(void (*body)(void*), void* arg);
    virtual ~Thread();
    int start();
    static void dispatch();
    static void wrapper(void* param);

protected:
    Thread();
    virtual void run(){}

private:
    thread_t myHandle = nullptr;
};

#endif //PROJECT_BASE_SYSCALL_CPP_HPP
