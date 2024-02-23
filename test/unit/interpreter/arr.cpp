#include <cstddef>
#include <vector>

#include <gtest/gtest.h>

#include "common/config.hpp"
#include "generated/instruction.hpp"
#include "interpreter/interpreter.hpp"

using namespace pvm;

TEST(Array, GetSet) {
  constexpr Int kData = 0xD;
  constexpr std::size_t kSize = 10;

  enum Registers : std::size_t {
    K_ONE_REG = 1,
    K_ZERO_REG,

    K_DATA_REG,
    K_DATA_IDX_REG,

    K_ARR_REG,
    K_REF_REG,
  };

  // clang-format off
  std::vector<Instr> instrs{
    /* 00 */ Instr{.opType = eIMM, .opID = eIMM_INTEGER, .instrVar = InstrIMM::Builder().data(0).build()},
    /* 01 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(K_ZERO_REG).build()},

    /* 02 */ Instr{.opType = eIMM, .opID = eIMM_INTEGER, .instrVar = InstrIMM::Builder().data(1).build()},
    /* 03 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(K_ONE_REG).build()},

    /* 04 */ Instr{.opType = eIMM, .opID = eIMM_INTEGER, .instrVar = InstrIMM::Builder().data(2).build()},
    /* 05 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(K_DATA_IDX_REG).build()},

    /* 06 */ Instr{.opType = eIMM, .opID = eIMM_INTEGER, .instrVar = InstrIMM::Builder().data(kData).build()},
    /* 07 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(K_DATA_REG).build()},

    /* 08 */ Instr{.opType = eIMM, .opID = eIMM_INTEGER, .instrVar = InstrIMM::Builder().data(kSize).build()},
    /* 09 */ Instr{.opType = eNEW, .opID = eNEW_ARRAY, .instrVar = InstrNEW::Builder().ttypeid(INT_T).regid(0).build()},
    /* 10 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(K_ARR_REG).build()},

    /* 11 */ Instr{.opType = eARRAY, .opID = eARRAY_GEP, .instrVar = InstrARRAY::Builder().aregid(K_ARR_REG).regid(K_DATA_IDX_REG).build()},
    /* 12 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(K_REF_REG).build()},

    /* 13 */ Instr{.opType = eOBJ_SET, .opID = eOBJ_SET_FIELD, .instrVar = InstrOBJ_SET::Builder().oregid(K_REF_REG).fregid(K_ZERO_REG).dregid(K_DATA_REG).build()},
    /* 14 */ Instr{.opType = eOBJ_GET, .opID = eOBJ_GET_FIELD, .instrVar = InstrOBJ_GET::Builder().oregid(K_REF_REG).fregid(K_ZERO_REG).build()},
    /* 15 */ Instr{.opType = eHALT, .opID = 0, .instrVar = InstrHALT::Builder().zero(0).build()},
  };
  // clang-format on

  Code code{std::move(instrs)};
  Interpreter interp{code};
  interp.run();
  const auto &rf = interp.getState().stack.back().rf;

  ASSERT_EQ(std::bit_cast<Int>(rf.readAcc()), kData);
}
