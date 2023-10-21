#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <stdexcept>

#include "common/config.hpp"
#include "common/instruction.hpp"
#include "decoder/decoder.hpp"
#include "executor/executor.hpp"
#include "executor/regfile.hpp"

namespace pvm {

class ExecutorGuts final {
private:
  RegFile &m_regFile;
  // Memory &m_mem;
  Code &m_code;

  std::reference_wrapper<std::istream> m_ist{std::cin};
  std::reference_wrapper<std::ostream> m_ost{std::cout};

public:
  explicit ExecutorGuts(RegFile &rf, /*Memory &mem,*/ Code &code);
  ExecutorGuts(RegFile &rf, /*Memory &mem,*/ Code &code, std::ostream &ost);
  ExecutorGuts(RegFile &rf, /*Memory &mem,*/ Code &code, std::istream &ist);
  ExecutorGuts(RegFile &rf, /*Memory &mem,*/ Code &code, std::ostream &ost,
               std::istream &ist);

  void exec();

private:
  Instr getInstr();
  void updatePC() {
    auto newPC = m_regFile.readPC() + 1;
    m_regFile.writePC(newPC);
  }

  bool execHalt(Instr instr);
  bool execLoadImmInt(Instr instr);
  bool execLoadImmFloat(Instr instr);
  bool execALUBinary(Instr instr);
  bool execALUUnary(Instr instr);
  bool execIO(Instr instr);
  bool execBLessFloat(Instr instr);
  bool execBLessInt(Instr instr);
  bool execBEqualFloat(Instr instr);
  bool execBEqualInt(Instr instr);

  const std::array<bool (ExecutorGuts::*)(Instr), OPCODE_NUM> dispatchTable{
      &ExecutorGuts::execHalt,         &ExecutorGuts::execLoadImmInt,
      &ExecutorGuts::execLoadImmFloat, &ExecutorGuts::execALUBinary,
      &ExecutorGuts::execALUUnary,     &ExecutorGuts::execIO,
      &ExecutorGuts::execBLessFloat,   &ExecutorGuts::execBLessInt,
      &ExecutorGuts::execBEqualFloat,  &ExecutorGuts::execBEqualInt,
  };

  void handleALUBinary(Instr instr);
  void handleALUUnary(Instr instr);
  void handleIO(Instr instr);

  template <ValueT T> void handleLoadImm(Instr instr) {
    m_regFile.write<T>(instr.rd, instr.getImm<T>());
    updatePC();
  }

  template <ValueT T> void handleALUBinary(Instr instr, std::function<T(T, T)> func) {
    auto lhs = m_regFile.read<T>(instr.rs1);
    auto rhs = m_regFile.read<T>(instr.rs2);
    m_regFile.write<T>(instr.rd, func(lhs, rhs));
  }

  template <ValueT T1, ValueT T2>
  void handleALUUnary(Instr instr, std::function<T2(T1)> func) {
    auto arg = m_regFile.read<T1>(instr.rs1);
    m_regFile.write<T2>(instr.rd, func(arg));
  }

  template <ValueT T> void handleIORead(Instr instr) {
    T val = 0;
    m_ist.get() >> val;
    m_regFile.write(instr.rd, val);
  }

  template <ValueT T> void handleIOWrite(Instr instr) {
    m_ost.get() << m_regFile.read<T>(instr.rs1) << std::endl;
  }

  template <ValueT T> void handleBranch(Instr instr, std::function<bool(T, T)> func) {
    auto lhs = m_regFile.read<T>(instr.rs1);
    auto rhs = m_regFile.read<T>(instr.rs2);
    if (func(lhs, rhs)) {
      auto pc = m_regFile.readPC();
      m_regFile.writePC(pc + instr.getImm<Int>());
    } else {
      updatePC();
    }
  }
};

ExecutorGuts::ExecutorGuts(RegFile &rf, /*Memory &mem,*/ Code &code)
    : m_regFile(rf), /*m_mem(mem),*/ m_code(code) {}

ExecutorGuts::ExecutorGuts(RegFile &rf, /*Memory &mem,*/ Code &code, std::ostream &ost)
    : m_regFile(rf), /*m_mem(mem),*/ m_code(code), m_ost(ost) {}

ExecutorGuts::ExecutorGuts(RegFile &rf, /*Memory &mem,*/ Code &code, std::istream &ist)
    : m_regFile(rf), /*m_mem(mem),*/ m_code(code), m_ist(ist) {}

ExecutorGuts::ExecutorGuts(RegFile &rf, /*Memory &mem,*/ Code &code, std::ostream &ost,
                           std::istream &ist)
    : m_regFile(rf), /*m_mem(mem),*/ m_code(code), m_ist(ist), m_ost(ost) {}

void ExecutorGuts::exec() {
  auto instr = getInstr();
  auto opcode = instr.opcode;
  (this->*dispatchTable[opcode])(instr);
}

Instr ExecutorGuts::getInstr() {
  Decoder dec{};
  auto pc = m_regFile.readPC();
  auto bytecode = m_code.loadInstr(pc);
  auto instr = dec.decode(bytecode);
  return instr;
}

bool ExecutorGuts::execHalt(Instr instr) { return true; }

bool ExecutorGuts::execLoadImmInt(Instr instr) {
  handleLoadImm<Int>(instr);
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*dispatchTable[opcode])(nextInstr);
}

bool ExecutorGuts::execLoadImmFloat(Instr instr) {
  handleLoadImm<Float>(instr);
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*dispatchTable[opcode])(nextInstr);
}

bool ExecutorGuts::execALUBinary(Instr instr) {
  handleALUBinary(instr);
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*dispatchTable[opcode])(nextInstr);
}

bool ExecutorGuts::execALUUnary(Instr instr) {
  handleALUUnary(instr);
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*dispatchTable[opcode])(nextInstr);
}

bool ExecutorGuts::execIO(Instr instr) {
  handleIO(instr);
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*dispatchTable[opcode])(nextInstr);
}

bool ExecutorGuts::execBLessFloat(Instr instr) {
  handleBranch<Float>(instr, std::less<Float>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*dispatchTable[opcode])(nextInstr);
}

bool ExecutorGuts::execBLessInt(Instr instr) {
  handleBranch<Int>(instr, std::less<Int>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*dispatchTable[opcode])(nextInstr);
}

bool ExecutorGuts::execBEqualFloat(Instr instr) {
  handleBranch<Float>(instr, std::equal_to<Float>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*dispatchTable[opcode])(nextInstr);
}

bool ExecutorGuts::execBEqualInt(Instr instr) {
  handleBranch<Int>(instr, std::equal_to<Int>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*dispatchTable[opcode])(nextInstr);
}

void ExecutorGuts::handleALUBinary(Instr instr) {
  switch (instr.op) {
  case OP_ADD_I:
    handleALUBinary<Int>(instr, std::plus<Int>{});
    break;
  case OP_ADD_F:
    handleALUBinary<Float>(instr, std::plus<Float>{});
    break;
  case OP_SUB_I:
    handleALUBinary<Int>(instr, std::minus<Int>{});
    break;
  case OP_SUB_F:
    handleALUBinary<Float>(instr, std::minus<Float>{});
    break;
  case OP_MUL_I:
    handleALUBinary<Int>(instr, std::multiplies<Int>{});
    break;
  case OP_MUL_F:
    handleALUBinary<Float>(instr, std::multiplies<Float>{});
    break;
  case OP_DIV_I:
    handleALUBinary<Int>(instr, std::divides<Int>{});
    break;
  case OP_DIV_F:
    handleALUBinary<Float>(instr, std::divides<Float>{});
    break;
  default:
    throw std::runtime_error{"Unknown binary operation"};
  }

  updatePC();
}

void ExecutorGuts::handleALUUnary(Instr instr) {
  switch (instr.op) {
  case OP_SQRT_I:
    handleALUUnary<Int, Int>(instr, [](Int val) { return std::sqrt(val); });
    break;
  case OP_SQRT_F:
    handleALUUnary<Float, Float>(instr, [](Float val) { return std::sqrt(val); });
    break;
  case OP_CAST_ITOF:
    handleALUUnary<Int, Float>(instr, std::identity{});
    break;
  case OP_CAST_FTOI:
    handleALUUnary<Float, Int>(instr, std::identity{});
    break;
  case OP_ABS_I:
    handleALUUnary<Int, Int>(instr, [](Int val) { return std::abs(val); });
    break;
  case OP_ABS_F:
    handleALUUnary<Float, Float>(instr, [](Float val) { return std::fabs(val); });
    break;
  default:
    throw std::runtime_error{"Unknown unary operation"};
  }

  updatePC();
}

void ExecutorGuts::handleIO(Instr instr) {
  switch (instr.op) {
  case OP_READ_I:
    handleIORead<Int>(instr);
    break;
  case OP_READ_F:
    handleIORead<Float>(instr);
    break;
  case OP_WRITE_I:
    handleIOWrite<Int>(instr);
    break;
  case OP_WRITE_F:
    handleIOWrite<Float>(instr);
    break;
  default:
    throw std::runtime_error{"Unknown IO operation"};
  }

  updatePC();
}

Executor::Executor(/*Memory &mem,*/ Code &code) : /*m_mem(mem),*/ m_code(code) {}

Executor::Executor(/*Memory &mem,*/ Code &code, std::ostream &ost)
    : /*m_mem(mem),*/ m_code(code), m_ost(ost) {}

Executor::Executor(/*Memory &mem,*/ Code &code, std::istream &ist)
    : /*m_mem(mem),*/ m_code(code), m_ist(ist) {}

Executor::Executor(/*Memory &mem,*/ Code &code, std::ostream &ost, std::istream &ist)
    : /*m_mem(mem),*/ m_code(code), m_ist(ist), m_ost(ost) {}

void Executor::exec() {
  ExecutorGuts eg{m_regFile, /*m_mem,*/ m_code, m_ost, m_ist};
  eg.exec();
}

[[nodiscard]] RegFile const &Executor::getRegFile() const { return m_regFile; }

} // namespace pvm
