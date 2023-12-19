#include <cstddef>
#include <sstream>
#include <vector>

#include <gtest/gtest.h>

#include "common/instruction.hpp"
#include "generated/instruction.hpp"
#include "interpreter/interpreter.hpp"

using namespace pvm;

TEST(Interpreter, Call) {
    constexpr std::size_t kInt = 1;

    std::vector<Instr> instrs{
        /* 0 */ Instr{.opType = eIMM, .opID = eIMM_INTEGER, .instrVar =
            InstrIMM::Builder().data(5).build()},
        /* 1 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = 
            InstrREG::Builder().regid(0).build()},
        /* 2 */ Instr{.opType = eBINARY, .opID = eBINARY_EQUAL, .instrVar =
            InstrBINARY::Builder().regid1(0).regid2(0).ttypeid(kInt).build()},
        /* 3 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = 
            InstrREG::Builder().regid(1).build()},
        /* 4 */ Instr{.opType = eBRANCH, .opID = eBRANCH_CALL, 
            .instrVar = InstrBRANCH::Builder().regid(1).offset(3).build()},
        /* 5 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = 
            InstrREG::Builder().regid(0).build()},
        /* 6 */ Instr{.opType = eHALT, .opID = 0, .instrVar = 
            InstrHALT::Builder().zero(0).build()},
        
        // Wanna check is R0 equal to zero and return 0 in this case
        /* 7 */ Instr{.opType = eIMM, .opID = eIMM_INTEGER, .instrVar =
            InstrIMM::Builder().data(0).build()},
        /* 8 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = 
            InstrREG::Builder().regid(1).build()},
        /* 9 */ Instr{.opType = eBINARY, .opID = eBINARY_EQUAL, .instrVar =
            InstrBINARY::Builder().regid1(0).regid2(1).ttypeid(kInt).build()},
        /* 10 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = 
            InstrREG::Builder().regid(2).build()},
        /* 11 */ Instr{.opType = eBRANCH, .opID = eBRANCH_BRANCH, .instrVar = 
            InstrBRANCH::Builder().regid(2).offset(16).build()},

        // Wanna save R0 parameter value for future to R2
        /* 12 */ Instr{.opType = eBINARY, .opID = eBINARY_ADD, .instrVar = 
            InstrBINARY::Builder().ttypeid(kInt).regid1(0).regid2(1).build()},
        /* 13 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = 
            InstrREG::Builder().regid(2).build()},

        // Wanna decrement R0
        /* 14 */ Instr{.opType = eIMM, .opID = eIMM_INTEGER, .instrVar =
            InstrIMM::Builder().data(-1).build()},
        /* 15 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = 
            InstrREG::Builder().regid(1).build()},
        /* 16 */ Instr{.opType = eBINARY, .opID = eBINARY_ADD, .instrVar = 
            InstrBINARY::Builder().ttypeid(kInt).regid1(0).regid2(1).build()},
        /* 17 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = 
            InstrREG::Builder().regid(0).build()},

        // Perform recursive call of this function
        /* 18 */ Instr{.opType = eBINARY, .opID = eBINARY_EQUAL, .instrVar =
            InstrBINARY::Builder().regid1(0).regid2(0).ttypeid(kInt).build()},
        /* 19 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = 
            InstrREG::Builder().regid(3).build()},
        /* 20 */ Instr{.opType = eBRANCH, .opID = eBRANCH_CALL, 
            .instrVar = InstrBRANCH::Builder().regid(3).offset(-13).build()},

        // Add R0 input value to result
        /* 21 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = 
            InstrREG::Builder().regid(0).build()},
        /* 22 */ Instr{.opType = eBINARY, .opID = eBINARY_ADD, .instrVar = 
            InstrBINARY::Builder().ttypeid(kInt).regid1(0).regid2(2).build()},
        /* 23 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = 
            InstrREG::Builder().regid(0).build()},
        /* 24 */ Instr{.opType = eBRANCH, .opID = eBRANCH_RET, 
            .instrVar = InstrBRANCH::Builder().regid(0).offset(0).build()},

        // Recursion termination
        /* 25 */ Instr{.opType = eIMM, .opID = eIMM_INTEGER, .instrVar =
            InstrIMM::Builder().data(0).build()},
        /* 26 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = 
            InstrREG::Builder().regid(0).build()},
        /* 27 */ Instr{.opType = eBRANCH, .opID = eBRANCH_RET, 
            .instrVar = InstrBRANCH::Builder().regid(0).offset(0).build()},
    };

    Code code{std::move(instrs)};
    Interpreter interp{code};
    interp.run();
    const auto &rf = interp.getState().rf;

    ASSERT_EQ(rf.readReg(0).get<Int>(), 15);

}
