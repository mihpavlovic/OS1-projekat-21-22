//
// Created by os on 8/17/22.
//

#ifndef PROJECT_BASE_LISTTEMPLATE_HPP
#define PROJECT_BASE_LISTTEMPLATE_HPP

#include "./memoryAllocator.hpp"
template<typename T>
class List
{
    struct Elem{
        T* data;
        Elem* next;
        Elem(T* d, Elem* n) : data(d), next(n){}
    };
    Elem* head=nullptr;
    Elem* tail=nullptr;
public:
    List():head(nullptr), tail(nullptr){}
    List(const List<T> &) = delete;
    List<T> &operator=(const List<T> &) = delete;


    void addLast(T* whatToAdd){
        size_t size = MemoryAllocator::calculateBlocks(sizeof(Elem));
        Elem* newElem = (Elem*) MemoryAllocator::kmalloc(size);//new Elem(whatToAdd, head);
        if(tail == nullptr){
            tail = newElem;
            head = newElem;
        }
        else{
            tail->next = newElem;
            tail = newElem;
        }
    }
    T* removeFirst(){
        if(head == nullptr) return nullptr;

        Elem* nodeToReturn = head;
        head = head->next;
        if(head == nullptr) tail = nullptr;

        T* dataToReturn = nodeToReturn->data;
        MemoryAllocator::kfree(nodeToReturn);
        return dataToReturn;
    }
};




#endif //PROJECT_BASE_LISTTEMPLATE_HPP
