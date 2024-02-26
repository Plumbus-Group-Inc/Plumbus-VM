#include <sstream>
#include <vector>

#include <gtest/gtest.h>

#include <float16_t/float16_t.hpp>

#include "common/instruction.hpp"
#include "generated/instruction.hpp"
#include "interpreter/interpreter.hpp"

using namespace pvm;

TEST(Interpreter, SquareEquation) {
  // clang-format off
  Instrs instrs{
    /* 00 */ Instr{.opType = eUNARY, .opID = eUNARY_READ, .instrVar = InstrUNARY::Builder().ttypeid(FLOAT_T).build()},
    /* 01 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x1).build()},

    /* 02 */ Instr{.opType = eUNARY, .opID = eUNARY_READ, .instrVar = InstrUNARY::Builder().ttypeid(FLOAT_T).build()},
    /* 03 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x2).build()},

    /* 04 */ Instr{.opType = eUNARY, .opID = eUNARY_READ, .instrVar = InstrUNARY::Builder().ttypeid(FLOAT_T).build()},
    /* 05 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x3).build()},

    /* 06 */ Instr{.opType = eIMM, .opID = eIMM_FLOATING, .instrVar = InstrIMM::Builder().data(static_cast<std::uint16_t>(numeric::float16_t{4.0})).build()},
    /* 07 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x4).build()},

    /* 08 */ Instr{.opType = eBINARY, .opID = eBINARY_MUL, .instrVar = InstrBINARY::Builder().ttypeid(FLOAT_T).regid1(0x2).regid2(0x2).build()},
    /* 09 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x5).build()},

    /* 10 */ Instr{.opType = eBINARY, .opID = eBINARY_MUL, .instrVar = InstrBINARY::Builder().ttypeid(FLOAT_T).regid1(0x1).regid2(0x3).build()},
    /* 11 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x6).build()},

    /* 12 */ Instr{.opType = eBINARY, .opID = eBINARY_MUL, .instrVar = InstrBINARY::Builder().ttypeid(FLOAT_T).regid1(0x4).regid2(0x6).build()},
    /* 13 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x7).build()},

    /* 14 */ Instr{.opType = eBINARY, .opID = eBINARY_SUB, .instrVar = InstrBINARY::Builder().ttypeid(FLOAT_T).regid1(0x5).regid2(0x7).build()},
    /* 15 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x8).build()},

    /* 16 */ Instr{.opType = eIMM, .opID = eIMM_FLOATING, .instrVar = InstrIMM::Builder().data(static_cast<std::uint16_t>(numeric::float16_t{1e-4F})).build()},
    /* 17 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x9).build()},

    /* 18 */ Instr{.opType = eUNARY, .opID = eUNARY_ABS, .instrVar = InstrUNARY::Builder().ttypeid(FLOAT_T).regid(0x8).build()},
    /* 19 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0xa).build()},

    /* 20 */ Instr{.opType = eBINARY, .opID = eBINARY_LESS, .instrVar = InstrBINARY::Builder().ttypeid(FLOAT_T).regid1(0xa).regid2(0x9).build()},
    /* 21 */ Instr{.opType = eBRANCH, .opID = eBRANCH_BRANCH, .instrVar = InstrBRANCH::Builder().regid(0x0).offset(24).build()},

    /* 22 */ Instr{.opType = eIMM, .opID = eIMM_FLOATING, .instrVar = InstrIMM::Builder().data(static_cast<std::uint16_t>(numeric::float16_t{0.0})).build()},
    /* 23 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0xb).build()},

    /* 24 */ Instr{.opType = eBINARY, .opID = eBINARY_LESS, .instrVar = InstrBINARY::Builder().ttypeid(FLOAT_T).regid1(0x8).regid2(0x9).build()},
    /* 25 */ Instr{.opType = eBRANCH, .opID = eBRANCH_BRANCH, .instrVar = InstrBRANCH::Builder().regid(0x0).offset(30).build()},

    //     /* two_roots */
    /* 26 */ Instr{.opType = eIMM, .opID = eIMM_FLOATING, .instrVar = InstrIMM::Builder().data(static_cast<std::uint16_t>(numeric::float16_t{2.0})).build()},
    /* 27 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0xc).build()},

    /* 28*/ Instr{.opType = eBINARY, .opID = eBINARY_MUL, .instrVar = InstrBINARY::Builder().ttypeid(FLOAT_T).regid1(0xc).regid2(0x1).build()},
    /* 29 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0xe).build()},

    /* 30 */ Instr{.opType = eBINARY, .opID = eBINARY_SUB, .instrVar = InstrBINARY::Builder().ttypeid(FLOAT_T).regid1(0xb).regid2(0x2).build()},
    /* 31 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0xf).build()},

    /* 32 */ Instr{.opType = eUNARY, .opID = eUNARY_SQRT, .instrVar = InstrUNARY::Builder().ttypeid(FLOAT_T).regid(0x8).build()},
    /* 33 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x10).build()},

    /* 34 */ Instr{.opType = eBINARY, .opID = eBINARY_SUB, .instrVar = InstrBINARY::Builder().ttypeid(FLOAT_T).regid1(0xf).regid2(0x10).build()},
    /* 35 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x11).build()},

    /* 36 */ Instr{.opType = eBINARY, .opID = eBINARY_DIV, .instrVar = InstrBINARY::Builder().ttypeid(FLOAT_T).regid1(0x11).regid2(0xe).build()},
    /* 37 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x12).build()},

    /* 38 */ Instr{.opType = eUNARY, .opID = eUNARY_WRITE, .instrVar = InstrUNARY::Builder().ttypeid(FLOAT_T).regid(0x12).build()},

    /* 39 */ Instr{.opType = eBINARY, .opID = eBINARY_ADD, .instrVar = InstrBINARY::Builder().ttypeid(FLOAT_T).regid1(0xf).regid2(0x10).build()},
    /* 40 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x13).build()},

    /* 41 */ Instr{.opType = eBINARY, .opID = eBINARY_DIV, .instrVar = InstrBINARY::Builder().ttypeid(FLOAT_T).regid1(0x13).regid2(0xe).build()},
    /* 42 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x14).build()},

    /* 43 */ Instr{.opType = eUNARY, .opID = eUNARY_WRITE, .instrVar = InstrUNARY::Builder().ttypeid(FLOAT_T).regid(0x14).build()}, // !!!!!!

    /* 44 */ Instr{.opType = eHALT, .opID = eHALT_HALT, .instrVar = InstrHALT::Builder().build()},

    //     /* one_root */
    /* 45 */ Instr{.opType = eIMM, .opID = eIMM_FLOATING, .instrVar = InstrIMM::Builder().data(static_cast<std::uint16_t>(numeric::float16_t{0.0})).build()},
    /* 46 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x15).build()},

    /* 47 */ Instr{.opType = eIMM, .opID = eIMM_FLOATING, .instrVar = InstrIMM::Builder().data(static_cast<std::uint16_t>(numeric::float16_t{2.0})).build()},
    /* 48 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x16).build()},

    /* 49 */ Instr{.opType = eBINARY, .opID = eBINARY_SUB, .instrVar = InstrBINARY::Builder().ttypeid(FLOAT_T).regid1(0x15).regid2(0x2).build()},
    /* 50 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x17).build()},

    /* 51 */ Instr{.opType = eBINARY, .opID = eBINARY_DIV, .instrVar = InstrBINARY::Builder().ttypeid(FLOAT_T).regid1(0x17).regid2(0x16).build()},
    /* 52 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x18).build()},

    /* 53 */ Instr{.opType = eBINARY, .opID = eBINARY_DIV, .instrVar = InstrBINARY::Builder().ttypeid(FLOAT_T).regid1(0x18).regid2(0x1).build()},
    /* 54 */ Instr{.opType = eREG, .opID = eREG_MOV, .instrVar = InstrREG::Builder().regid(0x19).build()},

    /* 55 */ Instr{.opType = eUNARY, .opID = eUNARY_WRITE, .instrVar = InstrUNARY::Builder().ttypeid(FLOAT_T).regid(0x19).build()},

    //     /* halt */
    /* 56 */ Instr{.opType = eHALT, .opID = eHALT_HALT, .instrVar = InstrHALT::Builder().build()},
  };
  // clang-format on

  Code code{std::move(instrs)};

  std::stringstream ist{};
  std::stringstream ost{};
  Interpreter interp{code, {ost, ist}};

  ist << "1 3 2" << std::endl;
  interp.run();

  Float x2 = 0;
  ost >> x2;
  EXPECT_DOUBLE_EQ(x2, -2);

  Float x1 = 0;
  ost >> x1;
  EXPECT_DOUBLE_EQ(x1, -1);
}
