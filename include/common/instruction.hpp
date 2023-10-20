#pragma once

#include <cstdint>
#include <type_traits>

#include "common/config.hpp"

namespace pvm {

enum : Opcode {
  OPCODE_HALT = 0,
  OPCODE_LOAD_IMM_I,
  OPCODE_LOAD_IMM_F,
  OPCODE_ALU_BINARY,
  OPCODE_ALU_UNARY,
  OPCODE_IO,
  OPCODE_BLESS_F,
  OPCODE_BLESS_I,
  OPCODE_BEQ_F,
  OPCODE_BEQ_I,
};

// Type bit
enum : OpId {
  OP_TYPE_I = 0,
  OP_TYPE_F = 1,
};

// ALU binary raw operations
enum : OpId {
  OP_ADD_I = 0,
  OP_ADD_F,
  OP_SUB_I,
  OP_SUB_F,
  OP_MUL_I,
  OP_MUL_F,
  OP_DIV_I,
  OP_DIV_F,
};

// ALU binary type independent operations
enum : OpId {
  OP_ADD = 0,
  OP_SUB = 2,
  OP_MUL = 4,
  OP_DIV = 6,
};

// ALU unary raw operations
enum : OpId {
  OP_SQRT_I = 0,
  OP_SQRT_F,
  OP_CAST_ITOF,
  OP_CAST_FTOI,
  OP_ABS_I,
  OP_ABS_F,
};

// ALU unary type independent operations
enum : OpId {
  OP_SQRT = 0,
  OP_CAST = 2,
  OP_ABS = 4,
};

// IO raw operations
enum : OpId {
  OP_READ_I = 0,
  OP_READ_F,
  OP_WRITE_I,
  OP_WRITE_F,
};

// IO type independent operations
enum : OpId {
  OP_READ = 0,
  OP_WRITE = 2,
};

// Branch raw operations
enum : OpId {
  OP_BEQUAL_I = 0,
  OP_BEQUAL_F,
  OP_BLESS_I,
  OP_BLESS_F,
};

template <typename T>
concept ImmT =
    std::is_same_v<T, Int> || std::is_same_v<T, Float> || std::is_same_v<T, OpId>;

struct Instr {
  Opcode opcode = 0;

  RegId rd = 0;
  RegId rs1 = 0;
  RegId rs2 = 0;

  union {
    OpId op;
    Int immi;
    Float immf;
  };

  template <ImmT T> T getImm() {
    if constexpr (std::is_same_v<T, Int>) {
      return immi;
    } else if constexpr (std::is_same_v<T, Float>) {
      return immf;
    }
    return op;
  }
};

} // namespace pvm
