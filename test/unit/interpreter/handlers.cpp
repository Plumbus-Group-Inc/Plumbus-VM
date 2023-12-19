#include <cstddef>
#include <cstdint>
#include <sstream>
#include <vector>

#include <gtest/gtest.h>

#include <float16_t/float16_t.hpp>

#include "common/config.hpp"
#include "common/instruction.hpp"
#include "generated/handlers.hpp"
#include "generated/instruction.hpp"
#include "interpreter/interpreter.hpp"
#include "memory/memory.hpp"
#include "memory/regfile.hpp"

using namespace pvm;

constexpr std::uint32_t kInt = 1;

auto createState() {
  return Interpreter::State{Decoder{}, RegFile{}, Memory{}, Code{std::vector<Instr>{}},
                            std::cout, std::cin};
}

TEST(Handlers, Halt) {
  auto state = createState();
  auto halt = InstrHALT::Builder().zero(0).build();
  ASSERT_NO_FATAL_FAILURE(exec_halt_halt(state, halt));
}

TEST(Handlers, ImmInt) {
  auto data = 1;
  auto state = createState();
  auto imm = InstrIMM::Builder().data(data).build();

  exec_imm_integer(state, imm);
  auto val = state.rf.readAcc();

  ASSERT_EQ(data, val.get<Int>());
}

TEST(Handlers, ImmFloat) {
  auto state = createState();
  numeric::float16_t data = 0.01;
  auto imm = InstrIMM::Builder().data(static_cast<std::uint16_t>(data)).build();

  exec_imm_floating(state, imm);
  auto val = state.rf.readAcc();

  ASSERT_FLOAT_EQ(static_cast<Float>(data), val.get<Float>());
}

TEST(Handlers, Mov) {
  Int data = 1;
  auto regid = 3;
  auto state = createState();
  state.rf.writeAcc(Value{data});

  auto mov = InstrREG::Builder().regid(regid).build();

  exec_reg_mov(state, mov);
  auto val = state.rf.readReg(regid);

  ASSERT_EQ(data, val.get<Int>());
}

TEST(Handlers, BinaryAdd) {
  Int lhs = 1;
  Int rhs = 2;
  RegId lrid = 6;
  RegId rrid = 3;

  auto state = createState();
  state.rf.writeReg(lrid, Value{lhs});
  state.rf.writeReg(rrid, Value{rhs});
  auto add = InstrBINARY::Builder().regid1(lrid).regid2(rrid).ttypeid(kInt).build();

  exec_binary_add(state, add);

  ASSERT_EQ(state.rf.readAcc().get<Int>(), lhs + rhs);
}

TEST(Handlers, BinaryMul) {
  Int lhs = 1;
  Int rhs = 2;
  RegId lrid = 6;
  RegId rrid = 3;

  auto state = createState();
  state.rf.writeReg(lrid, Value{lhs});
  state.rf.writeReg(rrid, Value{rhs});
  auto mul = InstrBINARY::Builder().regid1(lrid).regid2(rrid).ttypeid(kInt).build();

  exec_binary_mul(state, mul);

  ASSERT_EQ(state.rf.readAcc().get<Int>(), lhs * rhs);
}

TEST(Handlers, BinaryLess) {
  Int lhs = 1;
  Int rhs = 2;
  RegId lrid = 6;
  RegId rrid = 3;

  auto state = createState();
  state.rf.writeReg(lrid, Value{lhs});
  state.rf.writeReg(rrid, Value{rhs});
  auto less = InstrBINARY::Builder().regid1(lrid).regid2(rrid).ttypeid(kInt).build();

  exec_binary_less(state, less);

  ASSERT_EQ(state.rf.readAcc().get<Bool>(), lhs < rhs);
}
