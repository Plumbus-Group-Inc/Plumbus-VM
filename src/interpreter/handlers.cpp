#include <algorithm>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <istream>
#include <ostream>
#include <stdexcept>

#include <float16_t/float16_t.hpp>

#include "common/config.hpp"
#include "common/value.hpp"
#include "generated/handlers.hpp"
#include "generated/instruction.hpp"

namespace pvm {

void exec_halt_halt(Interpreter::State &state, InstrHALT instr) {
}

void exec_imm_integer(Interpreter::State &state, InstrIMM instr) {
  auto data = static_cast<Int>(instr.data);
  state.rf.writeAcc(Value{data});
}

void exec_imm_floating(Interpreter::State &state, InstrIMM instr) {
  std::uint16_t raw = 0;
  auto *rawPtr = reinterpret_cast<std::uint8_t *>(&raw);
  auto *dataPtr = reinterpret_cast<std::uint8_t *>(&instr.data);
  std::copy_n(dataPtr, 2, rawPtr);

  numeric::float16_t dataF16{raw};
  auto data = static_cast<Float>(dataF16);
  state.rf.writeAcc(Value{data});
}

void exec_imm_array(Interpreter::State &state, InstrIMM instr) {
  auto a = Array(std::bit_cast<Int>(instr.data));
  state.rf.writeAcc(Value{a});
}

void exec_array_set(Interpreter::State &state, InstrARRAY instr) {
  state.rf.readReg(instr.aregid)
      .get<Array>()
      .at(state.rf.readReg(instr.regid).get<Int>()) = state.rf.readAcc();
}

void exec_array_get(Interpreter::State &state, InstrARRAY instr) {
  auto value = state.rf.readReg(instr.aregid)
                   .get<Array>()
                   .at(state.rf.readReg(instr.regid).get<Int>());
  state.rf.writeAcc(value);
}

void exec_reg_mov(Interpreter::State &state, InstrREG instr) {
  state.rf.writeReg(instr.regid, state.rf.readAcc());
}

void exec_branch_branch(Interpreter::State &state, InstrBRANCH instr) {
  auto &rf = state.rf;

  if (auto cond = rf.readReg(instr.regid).get<Bool>(); !cond) {
    state.rf.incrementPC();
    return;
  }

  rf.writePC(rf.readPC() + std::bit_cast<Addr>(instr.offset));
}

void exec_branch_call(Interpreter::State &state, InstrBRANCH instr) {
  state.stack.push_back(state.rf);

  auto &rf = state.rf;
  if (auto cond = rf.readReg(instr.regid).get<Bool>(); !cond) {
    state.rf.incrementPC();
    return;
  }

  rf.writePC(rf.readPC() + std::bit_cast<Addr>(instr.offset));
}

void exec_branch_ret(Interpreter::State &state, InstrBRANCH instr) {
  auto returnValue = state.rf.readReg(instr.regid);

  state.rf = state.stack.back();
  state.rf.writeAcc(returnValue);
  state.stack.pop_back();

  state.rf.incrementPC();
}

void exec_unary_write(Interpreter::State &state, InstrUNARY instr) {
  auto val = state.rf.readReg(instr.regid);
  if (instr.ttypeid == 1) {
    state.ost << val.get<Int>() << std::endl;
  } else if (instr.ttypeid == 2) {
    state.ost << val.get<Float>() << std::endl;
  } else {
    throw std::runtime_error{"unknown type id in write instruction"};
  }
}

void exec_unary_read(Interpreter::State &state, InstrUNARY instr) {
  if (instr.ttypeid == 1) {
    Int tmp{};
    state.ist >> tmp;
    state.rf.writeAcc(Value{tmp});
  } else if (instr.ttypeid == 2) {
    Float tmp{};
    state.ist >> tmp;
    state.rf.writeAcc(Value{tmp});
  } else {
    throw std::runtime_error{"unknown type id in write instruction"};
  }
}

void exec_unary_abs(Interpreter::State &state, InstrUNARY instr) {
  if (instr.ttypeid == 1) {
    auto tmp = state.rf.readReg(instr.regid).get<Int>();
    state.rf.writeAcc(Value{std::abs(tmp)});
  } else if (instr.ttypeid == 2) {
    auto tmp = state.rf.readReg(instr.regid).get<Float>();
    state.rf.writeAcc(Value{std::fabs(tmp)});
  } else {
    throw std::runtime_error{"unknown type id in write instruction"};
  }
}

void exec_unary_sqrt(Interpreter::State &state, InstrUNARY instr) {
  if (instr.ttypeid == 2) {
    auto tmp = state.rf.readReg(instr.regid).get<Float>();
    state.rf.writeAcc(Value{std::sqrt(tmp)});
  } else {
    throw std::runtime_error{"unknown type id in write instruction"};
  }
}

template <typename In, typename F>
void exec_binary_template(Interpreter::State &state, InstrBINARY instr, F f) {
  auto lhs = state.rf.readReg(instr.regid1).get<In>();
  auto rhs = state.rf.readReg(instr.regid2).get<In>();
  auto res = f(lhs, rhs);
  state.rf.writeAcc(Value{res});
}

void exec_binary_less(Interpreter::State &state, InstrBINARY instr) {
  switch (instr.ttypeid) {
  case 1:
    exec_binary_template<Int>(state, instr, std::less<Int>{});
    break;
  case 2:
    exec_binary_template<Float>(state, instr, std::less<Float>{});
    break;
  default:
    throw std::runtime_error{"unknown ttypeid for bin instr"};
  }
}

void exec_binary_equal(Interpreter::State &state, InstrBINARY instr) {
  switch (instr.ttypeid) {
  case 1:
    exec_binary_template<Int>(state, instr, std::equal_to<Int>{});
    break;
  case 2:
    exec_binary_template<Float>(state, instr, std::equal_to<Float>{});
    break;
  default:
    throw std::runtime_error{"unknown ttypeid for bin instr"};
  }
}

void exec_binary_add(Interpreter::State &state, InstrBINARY instr) {
  switch (instr.ttypeid) {
  case 1:
    exec_binary_template<Int>(state, instr, std::plus<Int>{});
    break;
  case 2:
    exec_binary_template<Float>(state, instr, std::plus<Float>{});
    break;
  default:
    throw std::runtime_error{"unknown ttypeid for bin instr"};
  }
}

void exec_binary_sub(Interpreter::State &state, InstrBINARY instr) {
  switch (instr.ttypeid) {
  case 1:
    exec_binary_template<Int>(state, instr, std::minus<Int>{});
    break;
  case 2:
    exec_binary_template<Float>(state, instr, std::minus<Float>{});
    break;
  default:
    throw std::runtime_error{"unknown ttypeid for bin instr"};
  }
}

void exec_binary_mul(Interpreter::State &state, InstrBINARY instr) {
  switch (instr.ttypeid) {
  case 1:
    exec_binary_template<Int>(state, instr, std::multiplies<Int>{});
    break;
  case 2:
    exec_binary_template<Float>(state, instr, std::multiplies<Float>{});
    break;
  default:
    throw std::runtime_error{"unknown ttypeid for bin instr"};
  }
}

void exec_binary_div(Interpreter::State &state, InstrBINARY instr) {
  switch (instr.ttypeid) {
  case 1:
    exec_binary_template<Int>(state, instr, std::divides<Int>{});
    break;
  case 2:
    exec_binary_template<Float>(state, instr, std::divides<Float>{});
    break;
  default:
    throw std::runtime_error{"unknown ttypeid for bin instr"};
  }
}

} // namespace pvm
