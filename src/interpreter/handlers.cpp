#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <istream>
#include <iterator>
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

struct ArrHeader final {
  ObjectHeader header{};
  std::uint64_t size{};
};

void exec_halt_halt([[maybe_unused]] Interpreter::State &state,
                    [[maybe_unused]] InstrHALT instr) {
}

void exec_imm_integer(Interpreter::State &state, InstrIMM instr) {
  Int data = static_cast<std::int16_t>(instr.data);
  state.rf().writeAcc(data);
}

void exec_imm_floating(Interpreter::State &state, InstrIMM instr) {
  Float dataF16 = numeric::float16_t{instr.data};
  state.rf().writeAcc(dataF16);
}

void exec_new_array(Interpreter::State &state, InstrNEW instr) {
  auto arrSize = state.rf().readReg(instr.regid);
  auto objSize = state.klasses.at(instr.ttypeid).size + sizeof(ObjectHeader);

  auto *data = state.mem.allocate(arrSize * objSize);
  auto *mem = state.mem.allocate(sizeof(ArrHeader) + arrSize * sizeof(Reg));

  auto *refs = reinterpret_cast<Ref *>(std::next(mem, sizeof(ArrHeader)));
  for (std::size_t i = 0; i < arrSize; ++i) {
    auto *ptr = reinterpret_cast<ObjectHeader *>(data + i * objSize);
    ptr->klass = std::bit_cast<std::uintptr_t>(&state.klasses.at(instr.ttypeid));
    refs[i] = reinterpret_cast<Ref>(ptr);
  }

  auto *arr = reinterpret_cast<ArrHeader *>(mem);
  arr->header.klass = std::bit_cast<std::uintptr_t>(&state.klasses.at(instr.ttypeid));
  arr->size = arrSize;

  state.rf().writeAcc(mem);
}

void exec_new_object(Interpreter::State &state, InstrNEW instr) {
  auto objSize = state.klasses.at(instr.ttypeid).size;

  auto *mem = state.mem.allocate(objSize);
  auto *header = reinterpret_cast<ObjectHeader *>(mem);

  header->klass = std::bit_cast<std::uintptr_t>(&state.klasses.at(instr.ttypeid));
  state.rf().writeAcc(mem);
}

void exec_array_gep(Interpreter::State &state, InstrARRAY instr) {
  auto index = state.rf().readReg<std::iter_difference_t<Ref *>>(instr.regid);
  auto *arr = reinterpret_cast<ArrHeader *>(state.rf().readReg(instr.aregid));

  auto *refs = reinterpret_cast<Ref *>(std::next(arr));
  state.rf().writeAcc(*std::next(refs, index));
}

void exec_array_size(Interpreter::State &state, InstrARRAY instr) {
  auto *arr = reinterpret_cast<ArrHeader *>(state.rf().readReg(instr.aregid));
  state.rf().writeAcc(arr->size);
}

void exec_reg_mov(Interpreter::State &state, InstrREG instr) {
  auto val = state.rf().readAcc();
  state.rf().writeReg(instr.regid, val);
}

void exec_branch_branch(Interpreter::State &state, InstrBRANCH instr) {
  const auto &rf = state.rf();

  if (auto cond = rf.readReg<Bool>(instr.regid); !cond) {
    ++state.pc;
    return;
  }

  state.pc += static_cast<Addr>(instr.offset);
}

void exec_branch_call(Interpreter::State &state, InstrBRANCH instr) {
  state.stack.emplace_back(state.rf(), state.pc + 1);

  const auto &rf = state.rf();
  if (auto cond = rf.readReg<Bool>(instr.regid); !cond) {
    ++state.pc;
    return;
  }

  state.pc += static_cast<Addr>(instr.offset);
}

void exec_branch_ret(Interpreter::State &state, InstrBRANCH instr) {
  auto retVal = state.rf().readReg(instr.regid);
  auto retPC = state.stack.back().retPC;

  state.stack.pop_back();
  state.rf().writeAcc(retVal);
  state.pc = retPC;
}

void exec_obj_get_field(Interpreter::State &state, InstrOBJ_GET instr) {
  auto *header = state.rf().readReg<ObjectHeader *>(instr.oregid);
  auto *klass = reinterpret_cast<Klass *>(header->klass);

  auto field = state.rf().readReg<std::size_t>(instr.fregid);
  auto offset = klass->field2offset[field];

  auto *data = reinterpret_cast<std::uint8_t *>(std::next(header));
  state.rf().writeAcc(*reinterpret_cast<Reg *>(std::next(data, offset)));
}

void exec_obj_set_field(Interpreter::State &state, InstrOBJ_SET instr) {
  auto *header = state.rf().readReg<ObjectHeader *>(instr.oregid);
  auto *klass = reinterpret_cast<Klass *>(header->klass);

  auto field = state.rf().readReg<std::size_t>(instr.fregid);
  auto offset = klass->field2offset[field];

  auto *data = reinterpret_cast<std::uint8_t *>(std::next(header));
  *std::next(data, offset) = state.rf().readReg(instr.dregid);
}

void exec_unary_write(Interpreter::State &state, InstrUNARY instr) {
  auto val = state.rf().readReg(instr.regid);
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
    state.rf().writeAcc(tmp);
  } else if (instr.ttypeid == 2) {
    Float tmp{};
    state.ist >> tmp;
    state.rf().writeAcc(tmp);
  } else {
    throw std::runtime_error{"unknown type id in write instruction"};
  }
}

void exec_unary_abs(Interpreter::State &state, InstrUNARY instr) {
  if (instr.ttypeid == 1) {
    auto tmp = state.rf().readReg<Int>(instr.regid);
    state.rf().writeAcc(std::abs(tmp));
  } else if (instr.ttypeid == 2) {
    auto tmp = state.rf().readReg<Float>(instr.regid);
    state.rf().writeAcc(std::fabs(tmp));
  } else {
    throw std::runtime_error{"unknown type id in write instruction"};
  }
}

void exec_unary_sqrt(Interpreter::State &state, InstrUNARY instr) {
  if (instr.ttypeid == 2) {
    auto tmp = state.rf().readReg<Float>(instr.regid);
    state.rf().writeAcc(std::sqrt(tmp));
  } else {
    throw std::runtime_error{"unknown type id in write instruction"};
  }
}

template <typename In, typename F>
void exec_binary_template(Interpreter::State &state, InstrBINARY instr, F f) {
  auto lhs = state.rf().readReg<In>(instr.regid1);
  auto rhs = state.rf().readReg<In>(instr.regid2);
  state.rf().writeAcc(f(lhs, rhs));
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
