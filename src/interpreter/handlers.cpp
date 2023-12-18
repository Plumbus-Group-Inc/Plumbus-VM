#include <bit>
#include <cstddef>
#include <functional>
#include <istream>
#include <ostream>
#include <stdexcept>

#include "common/config.hpp"
#include "common/value.hpp"
#include "generated/handlers.hpp"
#include "generated/instruction.hpp"

namespace pvm {

void exec_halt_halt(Interpreter::State &state, InstrHALT instr) {
  state.halted = true;
}

void exec_imm_integer(Interpreter::State &state, InstrIMM instr) {
  auto data = std::bit_cast<Int>(instr.data);
  state.rf.writeAcc(Value{data});
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

} // namespace pvm