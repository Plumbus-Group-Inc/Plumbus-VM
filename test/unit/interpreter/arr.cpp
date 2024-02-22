#include <cstddef>
#include <sstream>
#include <vector>

#include <gtest/gtest.h>

#include "common/instruction.hpp"
#include "generated/instruction.hpp"
#include "interpreter/interpreter.hpp"

using namespace pvm;

TEST(Interpreter, Call) {

  // clang-format off
  std::vector<Instr> instrs{

  };
  // clang-format on

  Code code{std::move(instrs)};
  Interpreter interp{code};
  interp.run();
  const auto &rf = interp.getState().stack.back().rf;

  ASSERT_EQ(std::bit_cast<Int>(rf.readAcc()), 15);
}
