#pragma once

#include <cstdint>

namespace pvm {

enum : uint8_t {
    OPCODE_HALT = 0,
    OPCODE_LOAD_IMM_I,
    OPCODE_LOAD_IMM_F,
    OPCODE_ALU3,
    OPCODE_ALU2,
    OPCODE_IO,
    OPCODE_BRANCH
};

// Type bit
enum : uint32_t {
    OP_TYPE_I = 0,
    OP_TYPE_F = 1
};

// ALU3 raw operations
enum : uint32_t {
    OP_ADD_I = 0,
    OP_ADD_F,
    OP_SUB_I,
    OP_SUB_F,
    OP_MUL_I,
    OP_MUL_F,
    OP_DIV_I,
    OP_DIV_F
};

// ALU3 type independent operations
enum : uint32_t {
    OP_ADD = 0,
    OP_SUB = 2,
    OP_MUL = 4,
    OP_DIV = 6
};

// ALU2 raw operations
enum : uint32_t {
    OP_SQRT_I = 0,
    OP_SQRT_F,
    OP_CAST_ITOF,
    OP_CAST_FTOI,
    OP_ABS_I,
    OP_ABS_F
};

// ALU2 type independent operations
enum : uint32_t {
    OP_SQRT = 0,
    OP_CAST = 2,
    OP_ABS = 4,
};

// IO raw operations
enum : uint32_t {
    OP_READ_I = 0,
    OP_READ_F,
    OP_WRITE_I,
    OP_WRITE_F
};

// IO type independent operations
enum : uint32_t {
    OP_READ = 0,
    OP_WRITE = 2
};

// Branch raw operations
enum : uint32_t {
    OP_BEQUAL_I = 0,
    OP_BEQUAL_F,
    OP_BLESS_I,
    OP_BLESS_F
};

// Branch type independent operations
enum : uint32_t {
    OP_BEQUAL = 0,
    OP_BLESS = 2
};

struct Instruction {
    uint8_t opcode = 0;

    uint8_t rd = 0;
    uint8_t rs1 = 0;
    uint8_t rs2 = 0;

    union {
        uint32_t op;
        int32_t immi;
        float immf;
    };
};

}
