//
// Created by home on 20/04/25.
//

#ifndef DECODER_H
#define DECODER_H

#include <memory>

#include "instructions.h"
#include <string>

class Decoder {
  public:
    static std::unique_ptr<Instruction> decode(const std::string& instruction);
};
#endif //DECODER_H
