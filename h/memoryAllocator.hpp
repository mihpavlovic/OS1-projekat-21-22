//
// Created by os on 8/30/22.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"
class MemoryAllocator {

    struct BlockHeader{
        size_t sizeOfBlock;
        BlockHeader* prev;
        BlockHeader* next;
        explicit BlockHeader(size_t s): sizeOfBlock(s), prev(nullptr), next(nullptr){}
    };

    static BlockHeader* freeHead; //glava liste slobodnih
    static BlockHeader* usedHead; //glava liste zauzetih

    //static uint64 calculateBlockswoutBlkHeader(size_t numOfBits);
    static void addToList(BlockHeader* toAdd, BlockHeader* head);
    static bool searchList(BlockHeader* head, BlockHeader* toSearch);
    static void tryToJoin(BlockHeader* toJoin);

public:
    MemoryAllocator() =delete; //zabranjivanje konstruktora

    static uint64 calculateBlocks(size_t numOfBits);

    static void prepareAllocator(); //postavlja potrebne vrednosti u freeHead, potrebno je pozvati u main na pocetku

    static void* kmalloc(size_t numOfBlks);

    static int kfree(void* addr);

};


#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP
