//
// Created by home on 22/04/25.
//

#ifndef IR_INSTRUCTIONS_H
#define IR_INSTRUCTIONS_H

#include <string>

enum class IRInstruction {
  // Store at stack
  CONST,

  // Load from stack
  LOAD,

  // Store at stack
  STORE,

  // Print from stack
  PRINT,

  // Arithmetic operations
  ADD,
  SUB,
  MUL,
  DIV,
  MOD,
 };


enum class IRType {
  // Integer
  INT8,
  INT32,
  INT64,

  // Float
  FLOAT32,
  FLOAT64,

  // Boolean
  BOOL,
};

enum class IRMeta {
  CONST,
  MUT,
  GLOBAL,
};


class IRMapper {
  public:
  static std::string getInstruction(IRInstruction ir);

  static std::string getType(IRType type);

  static std::string getMeta(IRMeta meta);

  static std::string getOperator(const std::string& op);
 };


#endif //IR_INSTRUCTIONS_H
