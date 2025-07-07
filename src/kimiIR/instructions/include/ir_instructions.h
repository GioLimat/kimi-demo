//
// Created by home on 22/04/25.
//

#ifndef IR_INSTRUCTIONS_H
#define IR_INSTRUCTIONS_H

#include <string>

enum class IRInstruction {
  // Store at stack
  CONST,

  ALLOC,
  REALLOC,
  FREE,

  // Load from stack
  LOAD,
  GLOAD,

  // Store at stack
  STORE,

  // Print from stack
  PRINT,

  // Function
  FN,
  FN_PARAM,
  CALL,
  CALL_END,
  RET,

  // DO WHILE
  DO,
  WHILE,

  // Flow
  IF_FALSE,
  JMP,
  LABEL,

  //Unary
  NEG,
  INC,
  DEC,
  POST_INC,
  POST_DEC,


  // Bitwise
  BIT_NOT,
  BIT_AND,
  BIT_OR,
  SHIFT_LEFT,
  SHIFT_RIGHT,
  BIT_XOR,

  // BLOCKS
  INIT_BLOCK,
  END_BLOCK,
  END_FN,
  END_CONDITION,

    CONST_STR,


  INDEX_ACCESS,

  // Arithmetic operations
  ADD,
  SUB,
  MUL,
  DIV,
  MOD,

  TRUE,
  FALSE,

  EQUAL_EQUAL,
  GRATER_THAN,
  LESS_THAN,
 };


enum class IRType {
  // Integer
  INT8,
  INT32,
  INT64,

  // Float
  FLOAT32,
  FLOAT64,

  CHAR,

  // Boolean
  BOOL,

  STR
};

enum class IRMeta {
  CONST,
  MUT,
};


class IRMapper {
  public:
  static std::string getInstruction(IRInstruction ir);

  static std::string getType(IRType type);

  static std::string getMeta(IRMeta meta);

  static std::string getOperator(const std::string& op);
 };


#endif //IR_INSTRUCTIONS_H
