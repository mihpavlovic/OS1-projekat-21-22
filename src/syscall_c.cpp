//
// Created by os on 8/10/22.
//
#include "../h/syscall_c.h"

// a0 = x10 ; a1 = x11
void* mem_alloc (size_t size){
    __asm__ volatile("mv a1, %[arg]" : : [arg] "r" (size));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (0x01));
    __asm__ volatile("ecall");

    //povratak vrednosti
    void* result=nullptr;
    __asm__ volatile("mv %[res], a0" : [res] "=r" (result));
    return result;
}



int mem_free (void* deleteParam){
    void* whatToDelete = deleteParam;
    __asm__ volatile("mv a1, %[arg]" : : [arg] "r" (whatToDelete));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (0x02));
    __asm__ volatile("ecall");

    int result;
    __asm__ volatile("mv %[res], a0" : [res] "=r" (result));
    return result;
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){
    __asm__ volatile("mv a3, %[arg]": : [arg] "r" (arg));
    __asm__ volatile("mv a2, %[arg]": : [arg] "r" (start_routine));
    __asm__ volatile("mv a1, %[arg]" : : [arg] "r" (handle));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (0x11));
    __asm__ volatile("ecall");
    //povratak vrednosti
    int result;
    __asm__ volatile("mv %[res], a0" : [res] "=r" (result));
    return result;
}

int thread_exit(){
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (0x12));
    __asm__ volatile("ecall");

    //povratak vrednosti
    int result;
    __asm__ volatile("mv %[res], a0" : [res] "=r" (result));
    return result;
}

void thread_dispatch(){
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (0x13));
    __asm__ volatile("ecall");
}

int thread_create_no_start(thread_t* handle, void(*start_routine)(void*), void* arg){
    __asm__ volatile("mv a3, %[arg]": : [arg] "r" (arg));
    __asm__ volatile("mv a2, %[arg]": : [arg] "r" (start_routine));
    __asm__ volatile("mv a1, %[arg]" : : [arg] "r" (handle));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (0x14));
    __asm__ volatile("ecall");
    //povratak vrednosti
    int result;
    __asm__ volatile("mv %[res], a0" : [res] "=r" (result));
    return result;
}

void goToUserMode(){
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (0x15));
    __asm__ volatile("ecall");
}

void goToSupervisorMode(){
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (0x16));
    __asm__ volatile("ecall");
}
