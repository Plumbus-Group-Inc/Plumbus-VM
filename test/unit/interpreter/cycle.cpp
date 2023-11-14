#include <sstream>
#include <vector>

#include <gtest/gtest.h>

#include "common/instruction.hpp"
#include "generated/instruction.hpp"
#include "interpreter/interpreter.hpp"

using namespace pvm;

TEST(Interpreter, Cycle) {

  std::vector<Instr> instrs{
      Instr{.opType = eBINARY,
            .opID = eBINARY_ADD,
            .instrVar = InstrBINARY::Builder().ttypeid(0).regid1(1).regid2(2).get()},
      Instr{.opType = eHALT, .opID = 0, .instrVar = InstrHALT::Builder().zero(0).get()},
  };

  // constexpr Int kIterNum = 100'000;

  // clang-format off
  // std::vector<Instr> instrs{
  //     /* init */
  //     Instr{.opcode = OPCODE_LOAD_IMM_I, .rd = 0x1, .rs1 = 0x0, .rs2 = 0x0, .immi = 0},        // loadImmI 0, r1         # r1 <- 0
  //     Instr{.opcode = OPCODE_LOAD_IMM_I, .rd = 0x2, .rs1 = 0x0, .rs2 = 0x0, .immi = 1},        // loadImmI 1, r2         # r2 <- 1
  //     Instr{.opcode = OPCODE_LOAD_IMM_I, .rd = 0x3, .rs1 = 0x0, .rs2 = 0x0, .immi = kIterNum}, // loadImmI kIterNum, r3  # r3 <- kIterNum
  //     Instr{.opcode = OPCODE_LOAD_IMM_I, .rd = 0x4, .rs1 = 0x0, .rs2 = 0x0, .immi = 1},        // loadImmI 1, r4         # r4 <- 1
  //     Instr{.opcode = OPCODE_LOAD_IMM_I, .rd = 0x5, .rs1 = 0x0, .rs2 = 0x0, .immi = 1},        // loadImmI 1, r5         # r5 <- 1

  //     /* cycle_body */
  //     Instr{.opcode = OPCODE_ALU_BINARY, .rd = 0x4, .rs1 = 0x4, .rs2 = 0x1, .op = OP_ADD_I},   // addI r4, r4, r1        # r4 <- r4 + r1
  //     Instr{.opcode = OPCODE_ALU_BINARY, .rd = 0x5, .rs1 = 0x4, .rs2 = 0x4, .op = OP_MUL_I},   // addI r5, r4, r4        # r5 <- r4 + r4
  //     Instr{.opcode = OPCODE_ALU_BINARY, .rd = 0x1, .rs1 = 0x1, .rs2 = 0x2, .op = OP_ADD_I},   // addI r1, r1, r2        # r1 <- r1 + 1
  //     // Instr{.opcode = OPCODE_IO,         .rd = 0x0, .rs1 = 0x1, .rs2 = 0x0, .op = OP_WRITE_I}, // writeI r1              # print r1
  //     Instr{.opcode = OPCODE_BLESS_I,    .rd = 0x0, .rs1 = 0x1, .rs2 = 0x3, .immi = -3},       // branchLessI -2, r1, r3 # cycle_body

  //     /* halt */
  //     Instr{.opcode = OPCODE_HALT, .rd = 0x0, .rs1 = 0x0, .rs2 = 0x0, .immi = 0x0}, // halt
  // };
  // clang-format on

  // std::vector<Bytecode> data{};
  // std::transform(instrs.begin(), instrs.end(), std::back_inserter(data),
  //                [](auto instr) { return *reinterpret_cast<Bytecode *>(&instr); });

  // Code code{std::move(data)};

  // Interpreter interp{code, std::cout, std::cin};
  // interp.run();
}
