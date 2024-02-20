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
#include "generated/handlers.hpp"
#include "generated/instruction.hpp"

namespace pvm {

void exec_halt_halt(Interpreter::State &state, InstrHALT instr) {
}

void exec_imm_integer(Interpreter::State &state, InstrIMM instr) {
  auto data = std::bit_cast<Reg>(static_cast<Int>(instr.data));
  state.stack.back().rf.writeAcc(data);
}

void exec_imm_floating(Interpreter::State &state, InstrIMM instr) {
  std::uint16_t raw = 0;
  auto *rawPtr = reinterpret_cast<std::uint8_t *>(&raw);
  auto *dataPtr = reinterpret_cast<std::uint8_t *>(&instr.data);
  std::copy_n(dataPtr, 2, rawPtr);

  numeric::float16_t dataF16{raw};
  auto data = std::bit_cast<Reg>(static_cast<Float>(dataF16));
  state.stack.back().rf.writeAcc(data);
}

// void exec_imm_array(Interpreter::State &state, InstrIMM instr) {
//   auto a = Array(std::bit_cast<Int>(instr.data));
//   state.stack.back().rf.writeAcc(a);
// }

// void exec_array_set(Interpreter::State &state, InstrARRAY instr) {
//   state.stack.back().rf.readReg(instr.aregid)
//       .get<Array>()
//       .at(state.stack.back().rf.readReg(instr.regid).get<Int>()) =
//       state.stack.back().rf.readAcc();
// }

// void exec_array_get(Interpreter::State &state, InstrARRAY instr) {
//   auto value = state.stack.back().rf.readReg(instr.aregid)
//                    .get<Array>()
//                    .at(state.stack.back().rf.readReg(instr.regid).get<Int>());
//   state.stack.back().rf.writeAcc(value);
// }

void exec_reg_mov(Interpreter::State &state, InstrREG instr) {
  auto val = state.stack.back().rf.readAcc();
  state.stack.back().rf.writeReg(instr.regid, val);
}

void exec_branch_branch(Interpreter::State &state, InstrBRANCH instr) {
  auto &rf = state.stack.back().rf;

  if (auto cond = static_cast<Bool>(rf.readReg(instr.regid)); !cond) {
    ++state.pc;
    return;
  }

  state.pc += instr.offset;
}

// void exec_branch_call(Interpreter::State &state, InstrBRANCH instr) {
//   state.stack.emplace_back(state.stack.back().rf, state.pc);

//   auto &rf = state.stack.back().rf;
//   if (auto cond = std::bit_cast<Bool>(rf.readReg(instr.regid)); !cond) {
//     ++state.pc;
//     return;
//   }

//   rf.writePC(rf.readPC() + std::bit_cast<Addr>(instr.offset));
// }

// void exec_branch_ret(Interpreter::State &state, InstrBRANCH instr) {
//   auto returnValue = state.stack.back().rf.readReg(instr.regid);

//   state.rf = state.stack.back();
//   state.stack.back().rf.writeAcc(returnValue);
//   state.stack.pop_back();

//   ++state.pc;
// }

void exec_unary_write(Interpreter::State &state, InstrUNARY instr) {
  auto val = state.stack.back().rf.readReg(instr.regid);
  if (instr.ttypeid == 1) {
    state.ost << std::bit_cast<Int>(val) << std::endl;
  } else if (instr.ttypeid == 2) {
    state.ost << std::bit_cast<Float>(val) << std::endl;
  } else {
    throw std::runtime_error{"unknown type id in write instruction"};
  }
}

void exec_unary_read(Interpreter::State &state, InstrUNARY instr) {
  if (instr.ttypeid == 1) {
    Int tmp{};
    state.ist >> tmp;
    state.stack.back().rf.writeAcc(std::bit_cast<Reg>(tmp));
  } else if (instr.ttypeid == 2) {
    Float tmp{};
    state.ist >> tmp;
    state.stack.back().rf.writeAcc(std::bit_cast<Reg>(tmp));
  } else {
    throw std::runtime_error{"unknown type id in write instruction"};
  }
}

void exec_unary_abs(Interpreter::State &state, InstrUNARY instr) {
  if (instr.ttypeid == 1) {
    auto tmp = std::bit_cast<Int>(state.stack.back().rf.readReg(instr.regid));
    state.stack.back().rf.writeAcc(std::bit_cast<Reg>(std::abs(tmp)));
  } else if (instr.ttypeid == 2) {
    auto tmp = std::bit_cast<Float>(state.stack.back().rf.readReg(instr.regid));
    state.stack.back().rf.writeAcc(std::bit_cast<Reg>(std::fabs(tmp)));
  } else {
    throw std::runtime_error{"unknown type id in write instruction"};
  }
}

void exec_unary_sqrt(Interpreter::State &state, InstrUNARY instr) {
  if (instr.ttypeid == 2) {
    auto tmp = std::bit_cast<Float>(state.stack.back().rf.readReg(instr.regid));
    state.stack.back().rf.writeAcc(std::bit_cast<Reg>(std::sqrt(tmp)));
  } else {
    throw std::runtime_error{"unknown type id in write instruction"};
  }
}

template <typename In, typename F>
void exec_binary_template(Interpreter::State &state, InstrBINARY instr, F f) {
  auto lhs = std::bit_cast<In>(state.stack.back().rf.readReg(instr.regid1));
  auto rhs = std::bit_cast<In>(state.stack.back().rf.readReg(instr.regid2));
  auto res = f(lhs, rhs);

  if constexpr (sizeof(res) == sizeof(Reg))
    state.stack.back().rf.writeAcc(std::bit_cast<Reg>(res));
  else
    state.stack.back().rf.writeAcc(static_cast<Reg>(res));
  ;
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
