#include <bit>
#include <cmath>
#include <cstdint>
#include <functional>
#include <istream>
#include <ostream>
#include <stdexcept>

#include <float16_t/float16_t.hpp>

#include "common/common.hpp"
#include "common/config.hpp"
#include "generated/handlers.hpp"
#include "generated/instruction.hpp"
#include "memory/regfile.hpp"
#include "objects/objects.hpp"

namespace pvm {

void exec_halt_halt([[maybe_unused]] Interpreter::State &state,
                    [[maybe_unused]] InstrHALT instr) {
}

void exec_imm_integer(Interpreter::State &state, InstrIMM instr) {
  Int data = static_cast<std::int16_t>(instr.data);
  state.topFrame().rf.writeAcc(data);
}

void exec_imm_floating(Interpreter::State &state, InstrIMM instr) {
  Float dataF16 = numeric::float16_t{instr.data};
  state.topFrame().rf.writeAcc(dataF16);
}

void exec_new_array(Interpreter::State &state, InstrNEW instr) {
  auto size = state.topFrame().rf.readReg(instr.regid);
  auto *ref = reinterpret_cast<ObjectHeader *>(state.mem.allocate(size * sizeof(Reg)));

  ref->klass = instr.ttypeid;
  state.topFrame().rf.writeAcc(ref);
}

void exec_new_object(Interpreter::State &state, InstrNEW instr) {

}

// void exec_imm_array(Interpreter::State &state, InstrIMM instr) {
//   auto a = Array(std::bit_cast<Int>(instr.data));
//   state.topFrame().rf.writeAcc(a);
// }

// void exec_array_set(Interpreter::State &state, InstrARRAY instr) {
//   state.topFrame().rf.readReg(instr.aregid)
//       .get<Array>()
//       .at(state.topFrame().rf.readReg(instr.regid).get<Int>()) =
//       state.topFrame().rf.readAcc();
// }

// void exec_array_get(Interpreter::State &state, InstrARRAY instr) {
//   auto value = state.topFrame().rf.readReg(instr.aregid)
//                    .get<Array>()
//                    .at(state.topFrame().rf.readReg(instr.regid).get<Int>());
//   state.topFrame().rf.writeAcc(value);
// }

void exec_reg_mov(Interpreter::State &state, InstrREG instr) {
  auto val = state.topFrame().rf.readAcc();
  state.topFrame().rf.writeReg(instr.regid, val);
}

void exec_branch_branch(Interpreter::State &state, InstrBRANCH instr) {
  const auto &rf = state.topFrame().rf;

  if (auto cond = static_cast<Bool>(rf.readReg(instr.regid)); !cond) {
    ++state.pc;
    return;
  }

  state.pc += static_cast<Addr>(instr.offset);
}

void exec_branch_call(Interpreter::State &state, InstrBRANCH instr) {
  state.stack.emplace_back(state.topFrame().rf, state.pc + 1);

  const auto &rf = state.topFrame().rf;
  if (auto cond = static_cast<Bool>(rf.readReg(instr.regid)); !cond) {
    ++state.pc;
    return;
  }

  state.pc += instr.offset;
}

void exec_branch_ret(Interpreter::State &state, InstrBRANCH instr) {
  auto retVal = state.topFrame().rf.readReg(instr.regid);
  auto retPC = state.topFrame().retPC;

  state.stack.pop_back();
  state.topFrame().rf.writeAcc(retVal);
  state.pc = retPC;
}

void exec_unary_write(Interpreter::State &state, InstrUNARY instr) {
  auto val = state.topFrame().rf.readReg(instr.regid);
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
    state.topFrame().rf.writeAcc(tmp);
  } else if (instr.ttypeid == 2) {
    Float tmp{};
    state.ist >> tmp;
    state.topFrame().rf.writeAcc(tmp);
  } else {
    throw std::runtime_error{"unknown type id in write instruction"};
  }
}

void exec_unary_abs(Interpreter::State &state, InstrUNARY instr) {
  if (instr.ttypeid == 1) {
    auto tmp = std::bit_cast<Int>(state.topFrame().rf.readReg(instr.regid));
    state.topFrame().rf.writeAcc(std::abs(tmp));
  } else if (instr.ttypeid == 2) {
    auto tmp = std::bit_cast<Float>(state.topFrame().rf.readReg(instr.regid));
    state.topFrame().rf.writeAcc(std::fabs(tmp));
  } else {
    throw std::runtime_error{"unknown type id in write instruction"};
  }
}

void exec_unary_sqrt(Interpreter::State &state, InstrUNARY instr) {
  if (instr.ttypeid == 2) {
    auto tmp = std::bit_cast<Float>(state.topFrame().rf.readReg(instr.regid));
    state.topFrame().rf.writeAcc(std::sqrt(tmp));
  } else {
    throw std::runtime_error{"unknown type id in write instruction"};
  }
}

template <typename In, typename F>
void exec_binary_template(Interpreter::State &state, InstrBINARY instr, F f) {
  auto lhs = std::bit_cast<In>(state.topFrame().rf.readReg(instr.regid1));
  auto rhs = std::bit_cast<In>(state.topFrame().rf.readReg(instr.regid2));
  state.topFrame().rf.writeAcc(f(lhs, rhs));
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
