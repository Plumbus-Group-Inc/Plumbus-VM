#include <gtest/gtest.h>

#include "interpreter/interpreter.hpp"

using namespace pvm;

TEST(Interpreter, Call) {
  // clang-format off
    Instrs instrs{
        /* 0 */ Instr{.opType = eIMM, .opID = eIMM_INTEGER, .instrVar =
            InstrIMM::Builder().data(5).build()},
        /* 1 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar =
            InstrREG::Builder().regid(0 + 1).build()},
        /* 2 */ Instr{.opType = eBINARY, .opID = eBINARY_EQUAL, .instrVar =
            InstrBINARY::Builder().regid1(0 + 1).regid2(0 + 1).ttypeid(INT_T).build()},
        /* 3 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar =
            InstrREG::Builder().regid(1 + 1).build()},
        /* 4 */ Instr{.opType = eBRANCH, .opID = eBRANCH_CALL,
            .instrVar = InstrBRANCH::Builder().regid(1 + 1).offset(3).build()},
        /* 5 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar =
            InstrREG::Builder().regid(0 + 1).build()},
        /* 6 */ Instr{.opType = eHALT, .opID = 0, .instrVar =
            InstrHALT::Builder().zero(0).build()},

        // Wanna check is R0 equal to zero and return 0 in this case
        /* 7 */ Instr{.opType = eIMM, .opID = eIMM_INTEGER, .instrVar =
            InstrIMM::Builder().data(0).build()},
        /* 8 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar =
            InstrREG::Builder().regid(1 + 1).build()},
        /* 9 */ Instr{.opType = eBINARY, .opID = eBINARY_EQUAL, .instrVar =
            InstrBINARY::Builder().regid1(0 + 1).regid2(1 + 1).ttypeid(INT_T).build()},
        /* 10 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar =
            InstrREG::Builder().regid(2 + 1).build()},
        /* 11 */ Instr{.opType = eBRANCH, .opID = eBRANCH_BRANCH, .instrVar =
            InstrBRANCH::Builder().regid(2 + 1).offset(16).build()},

        // Wanna save R0 parameter value for future to R2
        /* 12 */ Instr{.opType = eBINARY, .opID = eBINARY_ADD, .instrVar =
            InstrBINARY::Builder().ttypeid(INT_T).regid1(0 + 1).regid2(1 + 1).build()},
        /* 13 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar =
            InstrREG::Builder().regid(2 + 1).build()},

        // Wanna decrement R0
        /* 14 */ Instr{.opType = eIMM, .opID = eIMM_INTEGER, .instrVar =
            InstrIMM::Builder().data(std::bit_cast<unsigned>(-1)).build()},
        /* 15 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar =
            InstrREG::Builder().regid(1 + 1).build()},
        /* 16 */ Instr{.opType = eBINARY, .opID = eBINARY_ADD, .instrVar =
            InstrBINARY::Builder().ttypeid(INT_T).regid1(0 + 1).regid2(1 + 1).build()},
        /* 17 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar =
            InstrREG::Builder().regid(0 + 1).build()},

        // Perform recursive call of this function
        /* 18 */ Instr{.opType = eBINARY, .opID = eBINARY_EQUAL, .instrVar =
            InstrBINARY::Builder().regid1(0 + 1).regid2(0 + 1).ttypeid(INT_T).build()},
        /* 19 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar =
            InstrREG::Builder().regid(3 + 1).build()},
        /* 20 */ Instr{.opType = eBRANCH, .opID = eBRANCH_CALL,
            .instrVar = InstrBRANCH::Builder().regid(3 + 1).offset(std::bit_cast<unsigned>(-13)).build()},

        // Add R0 input value to result
        /* 21 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar =
            InstrREG::Builder().regid(0 + 1).build()},
        /* 22 */ Instr{.opType = eBINARY, .opID = eBINARY_ADD, .instrVar =
            InstrBINARY::Builder().ttypeid(INT_T).regid1(0 + 1).regid2(2 + 1).build()},
        /* 23 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar =
            InstrREG::Builder().regid(0 + 1).build()},
        /* 24 */ Instr{.opType = eBRANCH, .opID = eBRANCH_RET,
            .instrVar = InstrBRANCH::Builder().regid(0 + 1).offset(0).build()},

        // Recursion termination
        /* 25 */ Instr{.opType = eIMM, .opID = eIMM_INTEGER, .instrVar =
            InstrIMM::Builder().data(0).build()},
        /* 26 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar =
            InstrREG::Builder().regid(0 + 1).build()},
        /* 27 */ Instr{.opType = eBRANCH, .opID = eBRANCH_RET,
            .instrVar = InstrBRANCH::Builder().regid(0 + 1).offset(0).build()},
    };
  // clang-format on

  Code code{std::move(instrs)};
  Interpreter interp{code};
  interp.run();
  const auto &rf = interp.getState().stack.back().rf;

  ASSERT_EQ(std::bit_cast<Int>(rf.readAcc()), 15);
}
