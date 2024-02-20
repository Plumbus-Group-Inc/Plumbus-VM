#include <bit>
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
constexpr std::uint32_t kFloat = 2;

auto createState() {
  auto state = Interpreter::State{
      Decoder{}, Memory{}, Code{std::vector<Instr>{}}, std::cout, std::cin, {{}},
  };
  return state;
}

TEST(Handlers, Halt) {
  auto state = createState();
  auto halt = InstrHALT::Builder().zero(0).build();
  ASSERT_NO_FATAL_FAILURE(exec_halt_halt(state, halt));
}

TEST(Handlers, ImmInt) {
  auto data = -1;
  auto state = createState();
  auto imm = InstrIMM::Builder().data(std::bit_cast<Reg>(data)).build();

  exec_imm_integer(state, imm);
  auto val = state.stack.back().rf.readAcc();

  ASSERT_EQ(data, std::bit_cast<Int>(val));
}

TEST(Handlers, ImmFloat) {
  auto state = createState();
  numeric::float16_t data = 0.01;
  auto imm = InstrIMM::Builder().data(data.data_.bits_).build();

  exec_imm_floating(state, imm);
  auto val = state.stack.back().rf.readAcc();

  ASSERT_FLOAT_EQ(static_cast<Float>(data), std::bit_cast<Float>(val));
}

TEST(Handlers, Mov) {
  Int data = 1;
  RegId regid = 3;
  auto state = createState();
  state.stack.back().rf.writeAcc(std::bit_cast<Reg>(data));

  auto mov = InstrREG::Builder().regid(regid).build();

  exec_reg_mov(state, mov);
  auto val = state.stack.back().rf.readReg(regid);

  ASSERT_EQ(data, std::bit_cast<Int>(val));
}

TEST(Handlers, BinaryAddInt) {
  Int lhs = 1;
  Int rhs = 2;
  RegId lrid = 6;
  RegId rrid = 3;

  auto state = createState();
  state.stack.back().rf.writeReg(lrid, lhs);
  state.stack.back().rf.writeReg(rrid, rhs);
  auto add = InstrBINARY::Builder().regid1(lrid).regid2(rrid).ttypeid(kInt).build();

  exec_binary_add(state, add);

  ASSERT_EQ(std::bit_cast<Int>(state.stack.back().rf.readAcc()), lhs + rhs);
}

TEST(Handlers, BinaryAddFloat) {
  Float lhs = 1;
  Float rhs = 2;
  RegId lrid = 6;
  RegId rrid = 3;

  auto state = createState();
  state.stack.back().rf.writeReg(lrid, lhs);
  state.stack.back().rf.writeReg(rrid, rhs);
  auto add = InstrBINARY::Builder().regid1(lrid).regid2(rrid).ttypeid(kFloat).build();

  exec_binary_add(state, add);
  auto output = std::bit_cast<Float>(state.stack.back().rf.readAcc());

  ASSERT_FLOAT_EQ(output, lhs + rhs);
}

TEST(Handlers, BinarySubInt) {
  Int lhs = 1;
  Int rhs = 2;
  RegId lrid = 6;
  RegId rrid = 3;

  auto state = createState();
  state.stack.back().rf.writeReg(lrid, lhs);
  state.stack.back().rf.writeReg(rrid, rhs);
  auto add = InstrBINARY::Builder().regid1(lrid).regid2(rrid).ttypeid(kInt).build();

  exec_binary_sub(state, add);

  ASSERT_EQ(std::bit_cast<Int>(state.stack.back().rf.readAcc()), lhs - rhs);
}

TEST(Handlers, BinarySubFloat) {
  Float lhs = 1;
  Float rhs = 2;
  RegId lrid = 6;
  RegId rrid = 3;

  auto state = createState();
  state.stack.back().rf.writeReg(lrid, lhs);
  state.stack.back().rf.writeReg(rrid, rhs);
  auto add = InstrBINARY::Builder().regid1(lrid).regid2(rrid).ttypeid(kFloat).build();

  exec_binary_sub(state, add);
  auto output = std::bit_cast<Float>(state.stack.back().rf.readAcc());

  ASSERT_FLOAT_EQ(output, lhs - rhs);
}

TEST(Handlers, BinaryMulInt) {
  Int lhs = 1;
  Int rhs = 2;
  RegId lrid = 6;
  RegId rrid = 3;

  auto state = createState();
  state.stack.back().rf.writeReg(lrid, lhs);
  state.stack.back().rf.writeReg(rrid, rhs);
  auto mul = InstrBINARY::Builder().regid1(lrid).regid2(rrid).ttypeid(kInt).build();

  exec_binary_mul(state, mul);

  ASSERT_EQ(std::bit_cast<Int>(state.stack.back().rf.readAcc()), lhs * rhs);
}

TEST(Handlers, BinaryMulFloat) {
  Float lhs = 1;
  Float rhs = 2;
  RegId lrid = 6;
  RegId rrid = 3;

  auto state = createState();
  state.stack.back().rf.writeReg(lrid, lhs);
  state.stack.back().rf.writeReg(rrid, rhs);
  auto mul = InstrBINARY::Builder().regid1(lrid).regid2(rrid).ttypeid(kFloat).build();

  exec_binary_mul(state, mul);

  ASSERT_FLOAT_EQ(std::bit_cast<Float>(state.stack.back().rf.readAcc()), lhs * rhs);
}

TEST(Handlers, BinaryDivlInt) {
  Int lhs = 1;
  Int rhs = 2;
  RegId lrid = 6;
  RegId rrid = 3;

  auto state = createState();
  state.stack.back().rf.writeReg(lrid, lhs);
  state.stack.back().rf.writeReg(rrid, rhs);
  auto mul = InstrBINARY::Builder().regid1(lrid).regid2(rrid).ttypeid(kInt).build();

  exec_binary_div(state, mul);

  ASSERT_EQ(std::bit_cast<Int>(state.stack.back().rf.readAcc()), lhs / rhs);
}

TEST(Handlers, BinaryDivlFloat) {
  Float lhs = 1;
  Float rhs = 2;
  RegId lrid = 6;
  RegId rrid = 3;

  auto state = createState();
  state.stack.back().rf.writeReg(lrid, lhs);
  state.stack.back().rf.writeReg(rrid, rhs);
  auto mul = InstrBINARY::Builder().regid1(lrid).regid2(rrid).ttypeid(kFloat).build();

  exec_binary_div(state, mul);

  ASSERT_FLOAT_EQ(std::bit_cast<Float>(state.stack.back().rf.readAcc()), lhs / rhs);
}

TEST(Handlers, BinaryLessInt) {
  Int lhs = 1;
  Int rhs = 2;
  RegId lrid = 6;
  RegId rrid = 3;

  auto state = createState();
  state.stack.back().rf.writeReg(lrid, lhs);
  state.stack.back().rf.writeReg(rrid, rhs);
  auto less = InstrBINARY::Builder().regid1(lrid).regid2(rrid).ttypeid(kInt).build();

  exec_binary_less(state, less);

  ASSERT_EQ(static_cast<Bool>(state.stack.back().rf.readAcc()), lhs < rhs);
}

TEST(Handlers, BinaryLessFloat) {
  Float lhs = 1;
  Float rhs = 2;
  RegId lrid = 6;
  RegId rrid = 3;

  auto state = createState();
  state.stack.back().rf.writeReg(lrid, lhs);
  state.stack.back().rf.writeReg(rrid, rhs);
  auto less = InstrBINARY::Builder().regid1(lrid).regid2(rrid).ttypeid(kFloat).build();

  exec_binary_less(state, less);

  ASSERT_EQ(static_cast<Bool>(state.stack.back().rf.readAcc()), lhs < rhs);
}