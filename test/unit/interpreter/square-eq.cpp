#include <sstream>
#include <vector>

#include <gtest/gtest.h>

#include "common/instruction.hpp"
#include "interpreter/interpreter.hpp"

using namespace pvm;

TEST(Interpreter, SquareEquation) {
  // clang-format off
  std::vector<Instr> instrs{
  //     Instr{.opcode = OPCODE_IO, .rd = 0x1, .rs1 = 0x0, .rs2 = 0x0, .op = OP_READ_F}, // readF r1 # r1 <- a
  //     Instr{.opcode = OPCODE_IO, .rd = 0x2, .rs1 = 0x0, .rs2 = 0x0, .op = OP_READ_F}, // readF r2 # r2 <- b
  //     Instr{.opcode = OPCODE_IO, .rd = 0x3, .rs1 = 0x0, .rs2 = 0x0, .op = OP_READ_F}, // readF r3 # r3 <- c

  //     Instr{.opcode = OPCODE_LOAD_IMM_F, .rd = 0x4, .rs1 = 0x0, .rs2 = 0x0, .immf = 4},      // loadImmF 4, r4  # r4 <- 4
  //     Instr{.opcode = OPCODE_ALU_BINARY, .rd = 0x5, .rs1 = 0x2, .rs2 = 0x2, .op = OP_MUL_F}, // mulF r5, r2, r2 # r5 <- b^2
  //     Instr{.opcode = OPCODE_ALU_BINARY, .rd = 0x6, .rs1 = 0x1, .rs2 = 0x3, .op = OP_MUL_F}, // mulF r6, r1, r3 # r6 <- ac
  //     Instr{.opcode = OPCODE_ALU_BINARY, .rd = 0x7, .rs1 = 0x4, .rs2 = 0x6, .op = OP_MUL_F}, // mulF r7, r4, r6 # r7 <- 4ac
  //     Instr{.opcode = OPCODE_ALU_BINARY, .rd = 0x8, .rs1 = 0x5, .rs2 = 0x7, .op = OP_SUB_F}, // subF r8, r5, r7 # r8 <- D

  //     Instr{.opcode = OPCODE_LOAD_IMM_F, .rd = 0x9, .rs1 = 0x0, .rs2 = 0x0, .immf = 1e-4F},  // loadImmF 10-e6, r9      # r9 <- 1e-6
  //     Instr{.opcode = OPCODE_ALU_UNARY,  .rd = 0xa, .rs1 = 0x8, .rs2 = 0x0, .op = OP_ABS_F}, // absF ra, r8             # ra <- |D|
  //     Instr{.opcode = OPCODE_BLESS_F,    .rd = 0x0, .rs1 = 0xa, .rs2 = 0x9, .immi = 0xe},    // branchLessF 0xe, ra, r9 # one_root

  //     Instr{.opcode = OPCODE_LOAD_IMM_F, .rd = 0xb, .rs1 = 0x0, .rs2 = 0x0, .immf = 0.0},  // loadImmF 0.0, rb         # rb <- 0.0
  //     Instr{.opcode = OPCODE_BLESS_F, .   rd = 0x0, .rs1 = 0x8, .rs2 = 0x9, .immi = 0x11}, // branchLessF 0x11, r8, r9 # no_roots

  //     /* two_roots */
  //     Instr{.opcode = OPCODE_LOAD_IMM_F, .rd = 0x0c, .rs1 = 0x0, .rs2 = 0x0, .immf = 2.0},     // loadImmF 2.0, rc # rc <- 2.0
  //     Instr{.opcode = OPCODE_ALU_BINARY, .rd = 0x0e, .rs1 = 0xc, .rs2 = 0x1, .op = OP_MUL_F},  // mulF re, rc, r1  # re <- 2a
  //     Instr{.opcode = OPCODE_ALU_BINARY, .rd = 0x0f, .rs1 = 0xb, .rs2 = 0x2, .op = OP_SUB_F},  // subF rf, rb, r2  # rf <- -b
  //     Instr{.opcode = OPCODE_ALU_UNARY,  .rd = 0x10, .rs1 = 0x8, .rs2 = 0x0, .op = OP_SQRT_F}, // sqrtF r10, r8    # r10 <- sqrt(D)

  //     Instr{.opcode = OPCODE_ALU_BINARY, .rd = 0x11, .rs1 = 0x0f, .rs2 = 0x10, .op = OP_SUB_F},   // subF r11, rf, r10 # r11 <- -b - sqrt(D)
  //     Instr{.opcode = OPCODE_ALU_BINARY, .rd = 0x12, .rs1 = 0x11, .rs2 = 0x0e, .op = OP_DIV_F},   // divF r12, r11, re # r12 <- x1 <- (-b - sqrt(D)) / 2a
  //     Instr{.opcode = OPCODE_IO,         .rd = 0x00, .rs1 = 0x12, .rs2 = 0x00, .op = OP_WRITE_F}, // writeF r12        # print x1

  //     Instr{.opcode = OPCODE_ALU_BINARY, .rd = 0x13, .rs1 = 0x0f, .rs2 = 0x10, .op = OP_ADD_F},   // addF r13, rf, r10 # r13 <- -b + sqrt(D)
  //     Instr{.opcode = OPCODE_ALU_BINARY, .rd = 0x14, .rs1 = 0x13, .rs2 = 0x0e, .op = OP_DIV_F},   // divF r14, r13, re # r14 <- x2 <- (-b + sqrt(D)) / 2a
  //     Instr{.opcode = OPCODE_IO,         .rd = 0x00, .rs1 = 0x14, .rs2 = 0x00, .op = OP_WRITE_F}, // writeF r14        # print x2
  //     Instr{.opcode = OPCODE_HALT,       .rd = 0x00, .rs1 = 0x00, .rs2 = 0x00, .immi = 0x0},      // halt

  //     /* one_root */
  //     Instr{.opcode = OPCODE_LOAD_IMM_F, .rd = 0x15, .rs1 = 0x00, .rs2 = 0x00, .immf = 0.0},      // loadImmF 0.0, r15   # r15 <- 0.0
  //     Instr{.opcode = OPCODE_LOAD_IMM_F, .rd = 0x16, .rs1 = 0x00, .rs2 = 0x00, .immf = 2.0},      // loadImmF 2.0, r16   # r16 <- 2.0
  //     Instr{.opcode = OPCODE_ALU_BINARY, .rd = 0x17, .rs1 = 0x15, .rs2 = 0x02, .op = OP_SUB_F},   // subF r17, r15, r2   # r17 <- -b
  //     Instr{.opcode = OPCODE_ALU_BINARY, .rd = 0x18, .rs1 = 0x17, .rs2 = 0x16, .op = OP_DIV_F},   // divF r18, r17, r15  # r18 <- - b / 2
  //     Instr{.opcode = OPCODE_ALU_BINARY, .rd = 0x19, .rs1 = 0x18, .rs2 = 0x01, .op = OP_DIV_F},   // divF r19, r18, r1   # r19 <- x <- - b / 2a
  //     Instr{.opcode = OPCODE_IO,         .rd = 0x00, .rs1 = 0x19, .rs2 = 0x00, .op = OP_WRITE_F}, // writeF r8           # print x

  //     /* halt */
  //     Instr{.opcode = OPCODE_HALT, .rd = 0x0, .rs1 = 0x0, .rs2 = 0x0, .immi = 0x0}, // halt
  };
  // clang-format on

  // std::vector<Bytecode> data{};
  // std::transform(instrs.begin(), instrs.end(), std::back_inserter(data),
  //                [](auto instr) { return *reinterpret_cast<Bytecode *>(&instr); });

  // Code code{std::move(data)};

  // std::stringstream ist{};
  // std::stringstream ost{};
  // Interpreter interp{code, ost, ist};

  // ist << "1 3 2" << std::endl;
  // interp.run();

  // Float x2 = 0;
  // ost >> x2;
  // ASSERT_FLOAT_EQ(x2, -2);

  // Float x1 = 0;
  // ost >> x1;
  // ASSERT_FLOAT_EQ(x1, -1);
}
