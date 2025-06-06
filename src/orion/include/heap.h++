//
// Created by home on 06/06/25.
//

#ifndef HEAP_H
#define HEAP_H
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <bits/ostream.tcc>

struct HeapBlock {
    uint64_t size;
    uint64_t used;
    bool   isFree;
    uint8_t* data() {
        return reinterpret_cast<uint8_t*>(this + 1);
    }
};

class HeapManager {
public:
    std::vector<uint8_t> buffer;
    std::vector<HeapBlock*> blocks;


    explicit HeapManager(size_t capacity) {
        buffer.resize(capacity);
        auto* block = reinterpret_cast<HeapBlock*>(buffer.data());
        block->size = capacity;
        block->used = 0;
        block->isFree = true;
        blocks.push_back(block);
    }

    uint8_t* alloc(size_t size) {
        size_t totalSize = size + sizeof(HeapBlock);
        for (auto* block : blocks) {
            if (block->isFree && block->size >= totalSize) {
                if (block->size >= totalSize + sizeof(HeapBlock) + 8) {
                    auto* newBlock = reinterpret_cast<HeapBlock*>(
                        reinterpret_cast<uint8_t*>(block) + totalSize
                    );
                    newBlock->size = block->size - totalSize;
                    newBlock->used = 0;
                    newBlock->isFree = true;
                    blocks.push_back(newBlock);

                    block->size = totalSize;
                }

                block->used = size;
                block->isFree = false;
                return block->data();
            }
        }

        fprintf(stderr, "Heap out of memory (requested %zu bytes)\n", size);
        std::abort();
    }

    void free(uint8_t* ptr) {
        auto* block = reinterpret_cast<HeapBlock*>(ptr - sizeof(HeapBlock));
        block->isFree = true;
        block->used = 0;
    }

    uint8_t* realloc(uint8_t* ptr, size_t newSize) {
        auto* oldBlock = reinterpret_cast<HeapBlock*>(ptr - sizeof(HeapBlock));
        if (newSize <= oldBlock->size) {
            oldBlock->used = newSize;
            return ptr;
        }
        uint8_t* newPtr = alloc(newSize);
        std::memcpy(newPtr, ptr, oldBlock->used);
        free(ptr);
        return newPtr;
    }

    HeapBlock* getBlock(uint8_t* ptr) {
        return reinterpret_cast<HeapBlock*>(ptr - sizeof(HeapBlock));
    }
};


#endif //HEAP_H
