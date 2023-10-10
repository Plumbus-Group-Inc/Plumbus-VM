#pragma once

#include <cstdint>

#include "common/config.hpp"

namespace pvm {

enum : Opcode {
  OPCODE_HALT = 0,
  OPCODE_LOAD_IMM_I,
  OPCODE_LOAD_IMM_F,
  OPCODE_ALU_BINARY,
  OPCODE_ALU_UNARY,
  OPCODE_IO,
  OPCODE_BRANCH
};

// Type bit
enum : OperationId { OP_TYPE_I = 0, OP_TYPE_F = 1 };

// ALU binary raw operations
enum : OperationId {
  OP_ADD_I = 0,
  OP_ADD_F,
  OP_SUB_I,
  OP_SUB_F,
  OP_MUL_I,
  OP_MUL_F,
  OP_DIV_I,
  OP_DIV_F
};

// ALU binary type independent operations
enum : OperationId { OP_ADD = 0, OP_SUB = 2, OP_MUL = 4, OP_DIV = 6 };

// ALU unary raw operations
enum : OperationId {
  OP_SQRT_I = 0,
  OP_SQRT_F,
  OP_CAST_ITOF,
  OP_CAST_FTOI,
  OP_ABS_I,
  OP_ABS_F
};

// ALU unary type independent operations
enum : OperationId {
  OP_SQRT = 0,
  OP_CAST = 2,
  OP_ABS = 4,
};

// IO raw operations
enum : OperationId { OP_READ_I = 0, OP_READ_F, OP_WRITE_I, OP_WRITE_F };

// IO type independent operations
enum : OperationId { OP_READ = 0, OP_WRITE = 2 };

// Branch raw operations
enum : OperationId { OP_BEQUAL_I = 0, OP_BEQUAL_F, OP_BLESS_I, OP_BLESS_F };

// Branch type independent operations
enum : OperationId { OP_BEQUAL = 0, OP_BLESS = 2 };

struct Instruction {
  Opcode opcode = 0;

  RegisterId rd = 0;
  RegisterId rs1 = 0;
  RegisterId rs2 = 0;

  union {
    OperationId op;
    Int immi;
    Float immf;
  };
};

} // namespace pvm
