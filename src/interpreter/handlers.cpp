#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <stdexcept>

#include <float16_t/float16_t.hpp>

#include "generated/handlers.hpp"
#include "generated/instruction.hpp"

#include "common/common.hpp"
#include "common/config.hpp"
#include "memory/regfile.hpp"
#include "objects/objects.hpp"

namespace pvm {

struct ArrHeader final {
  ObjectHeader header{};
  std::uint64_t size{};
};

class HandlerError final : std::runtime_error {
public:
  explicit HandlerError(const char *msg) : std::runtime_error(msg) {}
  explicit HandlerError(const std::string &msg) : std::runtime_error(msg) {}
};

void exec_halt_halt([[maybe_unused]] State &state, [[maybe_unused]] InstrHALT instr) {}

void exec_imm_integer(State &state, InstrIMM instr) {
  Int data = static_cast<std::int16_t>(instr.data);
  state.rf().writeAcc(data);
}

void exec_imm_floating(State &state, InstrIMM instr) {
  Float dataF16 = numeric::float16_t{instr.data};
  state.rf().writeAcc(dataF16);
}

void exec_imm_bool(State &state, InstrIMM instr) {
  Bool data = static_cast<Bool>(instr.data);
  state.rf().writeAcc(data);
}

void exec_imm_char(State &state, InstrIMM instr) {
  auto data = static_cast<Char>(instr.data);
  state.rf().writeAcc(data);
}

void exec_new_array(State &state, InstrNEW instr) {
  auto arrSize = state.rf().readReg(instr.regid);
  auto objSize = state.klasses.at(instr.ttypeid).size + sizeof(ObjectHeader);

  auto *raw = state.mem.allocate(arrSize * objSize);
  auto *arr = std::bit_cast<ArrHeader *>(
      state.mem.allocate(sizeof(ArrHeader) + arrSize * sizeof(Reg)));
  arr->header.klass = static_cast<KlassWord>(instr.ttypeid);
  arr->size = arrSize;

  auto *refs = std::bit_cast<Ref *>(std::next(arr));
  for (std::size_t i = 0; i < arrSize; ++i) {
    auto *ptr = std::bit_cast<ObjectHeader *>(raw + i * objSize);
    ptr->klass = static_cast<KlassWord>(instr.ttypeid);
    refs[i] = std::bit_cast<Ref>(ptr);
  }

  state.rf().writeAcc(arr);
}

void exec_new_object(State &state, InstrNEW instr) {
  auto objSize = state.klasses.at(instr.ttypeid).size + sizeof(ObjectHeader);
  auto *header = std::bit_cast<ObjectHeader *>(state.mem.allocate(objSize));
  header->klass = static_cast<KlassWord>(instr.ttypeid);
  state.rf().writeAcc(header);
}

void exec_array_gep(State &state, InstrARRAY instr) {
  auto index = state.rf().readReg<std::ptrdiff_t>(instr.regid);
  auto *arr = std::bit_cast<ArrHeader *>(state.rf().readReg(instr.aregid));
  auto *refs = std::bit_cast<Ref *>(std::next(arr));
  state.rf().writeAcc(*std::next(refs, index));
}

void exec_array_size(State &state, InstrARRAY instr) {
  auto *arr = std::bit_cast<ArrHeader *>(state.rf().readReg(instr.aregid));
  state.rf().writeAcc(arr->size);
}

void exec_reg_mov(State &state, InstrREG instr) {
  auto val = state.rf().readAcc();
  state.rf().writeReg(instr.regid, val);
}

void exec_reg_push(State &state, InstrREG instr) {
  auto &calleeStack = state.callStack.back().calleeStack;
  calleeStack.push_back(state.rf().readReg(instr.regid));
}

void exec_branch_branch(State &state, InstrBRANCH instr) {
  if (auto cond = state.rf().readReg<Bool>(instr.regid); cond) {
    state.pc += static_cast<Addr>(instr.offset);
  } else {
    ++state.pc;
  }
}

void exec_branch_call(State &state, InstrBRANCH instr) {
  auto &calleeStack = state.callStack.back().calleeStack;
  state.callStack.emplace_back(state.rf(), state.pc + 1, std::move(calleeStack));
  calleeStack.clear();

  if (auto cond = state.rf().readReg<Bool>(instr.regid); cond) {
    state.pc += static_cast<Addr>(instr.offset);
  } else {
    ++state.pc;
  }
}

void exec_branch_ret(State &state, InstrBRANCH instr) {
  auto r = state.rf().readReg(instr.regid);
  state.pc = state.callStack.back().retPC;
  state.callStack.pop_back();
  state.rf().writeAcc(r);
}

void exec_obj_get_field(State &state, InstrOBJ_GET instr) {
  auto *header = state.rf().readReg<ObjectHeader *>(instr.oregid);
  auto &klass = state.klasses[header->klass];

  auto field = state.rf().readReg<std::size_t>(instr.fregid);
  auto [offset, size] = klass.field2offset[field];

  auto *dataUI8 = std::bit_cast<std::uint8_t *>(std::next(header));
  auto *dataReg = std::bit_cast<Reg *>(std::next(dataUI8, offset));

  auto reg = getBits(*dataReg, size * kBitsInByte - 1, 0);
  state.rf().writeAcc(reg);
}

void exec_obj_set_field(State &state, InstrOBJ_SET instr) {
  auto *header = state.rf().readReg<ObjectHeader *>(instr.oregid);
  auto &klass = state.klasses[header->klass];

  auto field = state.rf().readReg<std::size_t>(instr.fregid);
  auto [offset, size] = klass.field2offset[field];

  auto *dataUI8 = std::bit_cast<std::uint8_t *>(std::next(header));
  auto *dataReg = std::next(dataUI8, offset);

  auto reg = state.rf().readReg(instr.dregid);
  for (std::size_t i = 0; i < size; ++i) {
    dataReg[i] = static_cast<std::uint8_t>(
        getBits(reg, (i + 1) * kBitsInByte - 1, i * kBitsInByte));
  }
}

void exec_unary_write(State &state, InstrUNARY instr) {
  auto val = state.rf().readReg(instr.regid);
  if (instr.ttypeid == INT_T) {
    state.config.ost.get() << std::bit_cast<Int>(val) << std::endl;
  } else if (instr.ttypeid == FLOAT_T) {
    state.config.ost.get() << std::bit_cast<Float>(val) << std::endl;
  } else {
    throw HandlerError{"unknown type id in write instruction"};
  }
}

void exec_unary_read(State &state, InstrUNARY instr) {
  if (instr.ttypeid == INT_T) {
    Int tmp{};
    state.config.ist.get() >> tmp;
    state.rf().writeAcc(tmp);
  } else if (instr.ttypeid == FLOAT_T) {
    Float tmp{};
    state.config.ist.get() >> tmp;
    state.rf().writeAcc(tmp);
  } else {
    throw HandlerError{"unknown type id in write instruction"};
  }
}

void exec_unary_abs(State &state, InstrUNARY instr) {
  if (instr.ttypeid == INT_T) {
    auto tmp = state.rf().readReg<Int>(instr.regid);
    state.rf().writeAcc(std::abs(tmp));
  } else if (instr.ttypeid == FLOAT_T) {
    auto tmp = state.rf().readReg<Float>(instr.regid);
    state.rf().writeAcc(std::fabs(tmp));
  } else {
    throw HandlerError{"unknown type id in write instruction"};
  }
}

void exec_unary_sqrt(State &state, InstrUNARY instr) {
  if (instr.ttypeid == INT_T) {
    auto tmp = state.rf().readReg<Int>(instr.regid);
    state.rf().writeAcc<Int>(std::sqrt(tmp));
  } else if (instr.ttypeid == FLOAT_T) {
    auto tmp = state.rf().readReg<Float>(instr.regid);
    state.rf().writeAcc(std::sqrt(tmp));
  } else {
    throw HandlerError{"unknown type id in write instruction"};
  }
}

template <typename In, typename F>
void exec_binary_template(State &state, InstrBINARY instr, F f) {
  auto lhs = state.rf().readReg<In>(instr.regid1);
  auto rhs = state.rf().readReg<In>(instr.regid2);
  state.rf().writeAcc(f(lhs, rhs));
}

#define EXEC_BINARY_MACRO(name, f)                                                       \
  void exec_binary_##name(State &state, InstrBINARY instr) {                             \
    switch ((instr).ttypeid) {                                                           \
    case INT_T:                                                                          \
      exec_binary_template<Int>(state, instr, f<Int>{});                                 \
      break;                                                                             \
    case FLOAT_T:                                                                        \
      exec_binary_template<Float>(state, instr, f<Float>{});                             \
      break;                                                                             \
    default:                                                                             \
      throw HandlerError{"unknown ttypeid for bin instr"};                               \
    }                                                                                    \
  }

EXEC_BINARY_MACRO(less, std::less)
EXEC_BINARY_MACRO(equal, std::equal_to)

EXEC_BINARY_MACRO(add, std::plus)
EXEC_BINARY_MACRO(sub, std::minus)
EXEC_BINARY_MACRO(div, std::divides)
EXEC_BINARY_MACRO(mul, std::multiplies)

} // namespace pvm
