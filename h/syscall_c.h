//
// Created by os on 6/18/22.
//

//containes all syscall declarations
#ifndef PROJECT_BASE_SYSCALL_C_H
#define PROJECT_BASE_SYSCALL_C_H

#include "../lib/hw.h"
#include "./tcb.hpp"
typedef TCB* thread_t;

extern "C" void* mem_alloc (size_t);

extern "C" int mem_free (void*);

extern "C" int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);

extern "C" int thread_exit();//gasi running

extern "C" void thread_dispatch();//oduzima procesor running i daje sledecoj iz schedulera

extern "C" int thread_create_no_start(thread_t* handle, void(*start_routine)(void*), void* arg);

extern "C" void goToUserMode();

extern "C" void goToSupervisorMode();


#endif //PROJECT_BASE_SYSCALL_C_H