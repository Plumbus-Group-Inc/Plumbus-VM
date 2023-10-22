#pragma once

#include <cstdint>
#include <type_traits>

#include "common/config.hpp"

namespace pvm {

#define PVM_LIST_OPCODES()                                                     \
  PVM_TRANSFORM(HALT)                                                          \
  PVM_TRANSFORM(LOAD_IMM_I)                                                    \
  PVM_TRANSFORM(LOAD_IMM_F)                                                    \
  PVM_TRANSFORM(ALU_BINARY)                                                    \
  PVM_TRANSFORM(ALU_UNARY)                                                     \
  PVM_TRANSFORM(IO)                                                            \
  PVM_TRANSFORM(BLESS_F)                                                       \
  PVM_TRANSFORM(BLESS_I)                                                       \
  PVM_TRANSFORM(BEQ_F)                                                         \
  PVM_TRANSFORM(BEQ_I)

enum : Opcode {
#define PVM_TRANSFORM(opcode) OPCODE_##opcode,
  PVM_LIST_OPCODES()
#undef PVM_TRANSFORM

      OPCODE_NUM,
};

// Type bit
enum : OpId {
  OP_TYPE_I = 0,
  OP_TYPE_F = 1,
  OP_TYPE_NUM,
};

#define PVM_LIST_BINARY_OPS()                                                  \
  PVM_TRANSFORM(ADD_I)                                                         \
  PVM_TRANSFORM(ADD_F)                                                         \
  PVM_TRANSFORM(SUB_I)                                                         \
  PVM_TRANSFORM(SUB_F)                                                         \
  PVM_TRANSFORM(MUL_I)                                                         \
  PVM_TRANSFORM(MUL_F)                                                         \
  PVM_TRANSFORM(DIV_I)                                                         \
  PVM_TRANSFORM(DIV_F)

// ALU binary raw operations
enum : OpId {
#define PVM_TRANSFORM(opcode) OP_##opcode,
  PVM_LIST_BINARY_OPS()
#undef PVM_TRANSFORM

      OP_BINARY_NUM
};

// ALU binary type independent operations
enum : OpId {
  OP_ADD = 0,
  OP_SUB = 2,
  OP_MUL = 4,
  OP_DIV = 6,
};

#define PVM_LIST_UNARY_OPS()                                                   \
  PVM_TRANSFORM(SQRT_I)                                                        \
  PVM_TRANSFORM(SQRT_F)                                                        \
  PVM_TRANSFORM(CAST_ITOF)                                                     \
  PVM_TRANSFORM(CAST_FTOI)                                                     \
  PVM_TRANSFORM(ABS_I)                                                         \
  PVM_TRANSFORM(ABS_F)

// ALU unary raw operations
enum : OpId {
#define PVM_TRANSFORM(opcode) OP_##opcode,
  PVM_LIST_UNARY_OPS()
#undef PVM_TRANSFORM

      OP_UNARY_NUM,
};

// ALU unary type independent operations
enum : OpId {
  OP_SQRT = 0,
  OP_CAST = 2,
  OP_ABS = 4,
};

#define PVM_LIST_IO_OPS()                                                      \
  PVM_TRANSFORM(READ_I)                                                        \
  PVM_TRANSFORM(READ_F)                                                        \
  PVM_TRANSFORM(WRITE_I)                                                       \
  PVM_TRANSFORM(WRITE_F)

// IO raw operations
enum : OpId {
#define PVM_TRANSFORM(opcode) OP_##opcode,
  PVM_LIST_IO_OPS()
#undef PVM_TRANSFORM
      OP_IO_NUM,
};

// IO type independent operations
enum : OpId {
  OP_READ = 0,
  OP_WRITE = 2,
};

#define PVM_LIST_BRANCH_OPS()                                                  \
  PVM_TRANSFORM(BEQUAL_I)                                                      \
  PVM_TRANSFORM(BEQUAL_F)                                                      \
  PVM_TRANSFORM(BLESS_I)                                                       \
  PVM_TRANSFORM(BLESS_F)

// Branch raw operations
enum : OpId {
#define PVM_TRANSFORM(opcode) OP_##opcode,
  PVM_LIST_BRANCH_OPS()
#undef PVM_TRANSFORM
      OP_BRANCH_NUM,
};

template <typename T>
concept ImmT = std::is_same_v<T, Int> || std::is_same_v<T, Float> ||
               std::is_same_v<T, OpId>;

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
