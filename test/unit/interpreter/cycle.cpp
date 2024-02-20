#include <cstddef>
#include <sstream>
#include <vector>

#include <gtest/gtest.h>

#include "common/instruction.hpp"
#include "generated/instruction.hpp"
#include "interpreter/interpreter.hpp"

using namespace pvm;

TEST(Interpreter, Cycle) {
  constexpr Int kIterNum = 10;
  constexpr std::size_t kInt = 1;

  // clang-format off
  std::vector<Instr> instrs{
      Instr{.opType = eIMM, .opID = eIMM_INTEGER, .instrVar = InstrIMM::Builder().data(0).build()},
      Instr{.opType = eREG, .opID = eREG_MOV,     .instrVar = InstrREG::Builder().regid(1).build()},

      Instr{.opType = eIMM, .opID = eIMM_INTEGER, .instrVar = InstrIMM::Builder().data(1).build()},
      Instr{.opType = eREG, .opID = eREG_MOV,     .instrVar = InstrREG::Builder().regid(2).build()},

      Instr{.opType = eIMM, .opID = eIMM_INTEGER, .instrVar = InstrIMM::Builder().data(kIterNum).build()},
      Instr{.opType = eREG, .opID = eREG_MOV,     .instrVar = InstrREG::Builder().regid(3).build()},

      Instr{.opType = eIMM, .opID = eIMM_INTEGER, .instrVar = InstrIMM::Builder().data(1).build()},
      Instr{.opType = eREG, .opID = eREG_MOV,     .instrVar = InstrREG::Builder().regid(4).build()},

      Instr{.opType = eIMM, .opID = eIMM_INTEGER, .instrVar = InstrIMM::Builder().data(1).build()},
      Instr{.opType = eREG, .opID = eREG_MOV,     .instrVar = InstrREG::Builder().regid(5).build()},

      Instr{.opType = eBINARY, .opID = eBINARY_ADD, .instrVar = InstrBINARY::Builder().ttypeid(kInt).regid1(4).regid2(1).build()},
      Instr{.opType = eREG, .opID = eREG_MOV,       .instrVar = InstrREG::Builder().regid(4).build()},

      Instr{.opType = eBINARY, .opID = eBINARY_MUL, .instrVar = InstrBINARY::Builder().ttypeid(kInt).regid1(4).regid2(4).build()},
      Instr{.opType = eREG, .opID = eREG_MOV,       .instrVar = InstrREG::Builder().regid(5).build()},

      Instr{.opType = eBINARY, .opID = eBINARY_ADD, .instrVar = InstrBINARY::Builder().ttypeid(kInt).regid1(1).regid2(2).build()},
      Instr{.opType = eREG, .opID = eREG_MOV,       .instrVar = InstrREG::Builder().regid(1).build()},

      Instr{.opType = eBINARY, .opID = eBINARY_LESS, .instrVar = InstrBINARY::Builder().ttypeid(kInt).regid1(1).regid2(3).build()},
      Instr{.opType = eREG, .opID = eREG_MOV,       .instrVar = InstrREG::Builder().regid(6).build()},

      Instr{.opType = eBRANCH, .opID = eBRANCH_BRANCH, .instrVar = InstrBRANCH::Builder().regid(6).offset(std::bit_cast<std::uint32_t>(-8)).build()},

      Instr{.opType = eHALT, .opID = 0, .instrVar = InstrHALT::Builder().zero(0).build()},
  };
  // clang-format on

  Code code{std::move(instrs)};
  Interpreter interp{code};
  interp.run();
  const auto &rf = interp.getState().stack.back().rf;

  ASSERT_EQ(std::bit_cast<Int>(rf.readReg(1)), kIterNum);
  ASSERT_EQ(std::bit_cast<Int>(rf.readReg(4)), 46);
  ASSERT_EQ(std::bit_cast<Int>(rf.readReg(5)), 2116);
}
