//
// Created by os on 8/30/22.
//

#include "../h/memoryAllocator.hpp"
MemoryAllocator::BlockHeader* MemoryAllocator::usedHead = nullptr;
MemoryAllocator::BlockHeader* MemoryAllocator::freeHead = nullptr;

void MemoryAllocator::prepareAllocator() {
    size_t allHeap = (char*) HEAP_END_ADDR - (char*) HEAP_START_ADDR - sizeof(BlockHeader);
    allHeap = calculateBlocks(allHeap);
    void* addr = (void*) HEAP_START_ADDR;
    BlockHeader pom = BlockHeader(allHeap);
    freeHead = (BlockHeader*)addr;
    *freeHead = pom;
}

uint64 MemoryAllocator::calculateBlocks(size_t numOfBits) {
    uint64 rez = 0;
    //numOfBits += sizeof(BlockHeader);
    if(numOfBits % MEM_BLOCK_SIZE == 0) rez = numOfBits / MEM_BLOCK_SIZE;
    else rez = numOfBits / MEM_BLOCK_SIZE + 1;
    return rez;
}
/*
uint64 MemoryAllocator::calculateBlockswoutBlkHeader(size_t numOfBits) {
    uint64 rez = 0;
    if(numOfBits % MEM_BLOCK_SIZE == 0) rez = numOfBits / MEM_BLOCK_SIZE;
    else rez = numOfBits / MEM_BLOCK_SIZE + 1;
    return rez;
}
*/
void *MemoryAllocator::kmalloc(size_t numOfBlks) {
    size_t sizeofHeaderInBlks = calculateBlocks(sizeof(BlockHeader));//dodavanje velicine headera u prostor koji se odvaja
    numOfBlks += sizeofHeaderInBlks;
    BlockHeader* currFree = freeHead;
    BlockHeader* chosen = nullptr;//koristice se kao izabrani cvor za zauzimanje
    while(currFree!=nullptr && currFree->sizeOfBlock < numOfBlks){//trazim cvor od kog cu uzimati prostor
        currFree = currFree->next;
    }

    if(currFree == nullptr) return nullptr; //ako nije nasao pogodan blok znaci da nema dovoljno mesta

    size_t whatsLeft = currFree->sizeOfBlock - numOfBlks;//gledam koliko mi je ostalo blokova u ovom cvoru
    if(whatsLeft > sizeofHeaderInBlks){//ako mi je ostalo vise blokova od velicine headera pravim novi cvor
        //odsecam sa kraja currFree i adresu stavljam u chosen, u chosen pravim novim novi blockHeader
        currFree->sizeOfBlock = whatsLeft;
        chosen = (BlockHeader*)((char*)currFree + (currFree->sizeOfBlock * MEM_BLOCK_SIZE));

        *chosen = BlockHeader(numOfBlks);

    }
    else{//ako nije ceo cvor pakujem u listu used
        chosen = currFree;

        //ispravljanje liste slobodnih
        if(currFree->prev) currFree->prev->next = currFree->next;
        else freeHead = currFree->next;
    }

    //dodavanje u listu zauzetih treba da se odradi u oba slucaja
    addToList(chosen, usedHead);

    return (void*)((char*)chosen + sizeof(BlockHeader));
}

int MemoryAllocator::kfree(void *addr) {
    BlockHeader* blkToFree = (BlockHeader*) ((char*) addr - sizeof(BlockHeader));
    if(searchList(usedHead, blkToFree)){
        if(blkToFree == usedHead){//ako je na prvom mestu
            usedHead = blkToFree->next;
            blkToFree->next = nullptr;
        }
        else{//ako je u sredini ili na kraju
            blkToFree->prev->next = blkToFree->next;
            if(blkToFree->next!=nullptr) blkToFree->next->prev = blkToFree->prev;//ako nije na kraju
            blkToFree->prev = nullptr;
            blkToFree->next = nullptr;
        }
        addToList(blkToFree, freeHead);
        tryToJoin(blkToFree);
        tryToJoin(blkToFree->prev);
        return 0;
    }
    else{//ako se ne nalazi u listi zauzetih vrati -1
        return -1;
    }
}

inline void MemoryAllocator::addToList(MemoryAllocator::BlockHeader *toAdd, MemoryAllocator::BlockHeader *head) {
    //ako je prazna lista
    if(head == nullptr){
        if(head == usedHead) usedHead = toAdd;
        else if(head == freeHead) freeHead = toAdd;
        return;
    }

    //ako ima u listi
    BlockHeader* curr = head;
    while(curr->next !=nullptr && curr< toAdd) {
        curr = curr->next;
    }
/*
    //ispitivanje da l je u pitanju poslednji i da l treba posle njega da se stavi
    if(curr->next == nullptr && curr < toAdd) {//ako treba iza poslednjeg da se stavi
        curr->next = toAdd;
        toAdd->prev = curr;
        toAdd->next = nullptr;
    }
    else {
        // da l se stavlja ispred prvog provera
        if(curr->prev != nullptr){
            curr->prev->next = toAdd;
        }
        else{//potrebno azuriranje freeHeada i usedHeada ako se oni menjaju
            if(head == usedHead) usedHead = toAdd;
            if(head == freeHead) freeHead = toAdd;
        }
        //ovo ostalo svakako treba odraditi
        toAdd->prev = curr->prev;
        toAdd->next = curr;
        curr->prev = toAdd;
    }
*/
    if(curr->next == nullptr){//ako je stigao curr do poslednjeg treba uporediti sa poslednjim
        if(curr < toAdd){//iza poslednjeg da ide nov
            curr->next = toAdd;
            toAdd->prev = curr;
            toAdd->next = nullptr;
        }
        else{//ispred poslednjeg da ide nov
            if(curr->prev != nullptr){
                curr->prev->next = toAdd;
            }
            toAdd->prev = curr->prev;
            toAdd->next = curr;
            curr->prev = toAdd;
        }
    }
    else{//treba staviti ispred curr
        if(curr->prev != nullptr){
            curr->prev->next = toAdd;
        }
        toAdd->prev = curr->prev;
        toAdd->next = curr;
        curr->prev = toAdd;
    }

}

inline bool MemoryAllocator::searchList(MemoryAllocator::BlockHeader *head, MemoryAllocator::BlockHeader *toSearch) {
    BlockHeader* curr = head;
    while(curr!= nullptr){
        if(curr == toSearch) return true;
        curr = curr->next;
    }
    return false;
}

void MemoryAllocator::tryToJoin(MemoryAllocator::BlockHeader *toJoin) {
    if(toJoin == nullptr) return;

    if(toJoin->next !=nullptr && (char*) toJoin + toJoin->sizeOfBlock*MEM_BLOCK_SIZE == (char*) toJoin->next){
        toJoin->sizeOfBlock += toJoin->next->sizeOfBlock;
        if(toJoin->next->next !=nullptr) toJoin->next->next->prev = toJoin;
        toJoin->next = toJoin->next->next;
    }
}












