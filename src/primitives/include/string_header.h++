//
// Created by home on 05/06/25.
//

#ifndef STRING_HEADER_H
#define STRING_HEADER_H

#include <cstdint>

#pragma pack(push, 1)
struct StringHeader {
    uint64_t length;
    uint64_t capacity;
    uint8_t  flags;
    uint8_t  pad[7];
};
#pragma pack(pop)

#endif //STRING_HEADER_H
