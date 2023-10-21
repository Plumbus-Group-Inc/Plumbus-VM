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

Executor::Executor(/*Memory &mem,*/ Code &code) : /*m_mem(mem),*/ m_code(code) {}

Executor::Executor(/*Memory &mem,*/ Code &code, std::ostream &ost)
    : /*m_mem(mem),*/ m_code(code), m_ost(ost) {}

Executor::Executor(/*Memory &mem,*/ Code &code, std::istream &ist)
    : /*m_mem(mem),*/ m_code(code), m_ist(ist) {}

Executor::Executor(/*Memory &mem,*/ Code &code, std::ostream &ost, std::istream &ist)
    : /*m_mem(mem),*/ m_code(code), m_ist(ist), m_ost(ost) {}

void Executor::exec() {
  auto instr = getInstr();
  auto opcode = instr.opcode;
  (this->*dispatchTable[opcode])(instr);
}

Instr Executor::getInstr() {
  Decoder dec{};
  auto pc = m_regFile.readPC();
  auto bytecode = m_code.loadInstr(pc);
  auto instr = dec.decode(bytecode);
  return instr;
}

bool Executor::execHalt(Instr instr) { return true; }

bool Executor::execLoadImmInt(Instr instr) {
  handleLoadImm<Int>(instr);
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*dispatchTable[opcode])(nextInstr);
}

bool Executor::execLoadImmFloat(Instr instr) {
  handleLoadImm<Float>(instr);
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*dispatchTable[opcode])(nextInstr);
}

bool Executor::execALUBinary(Instr instr) {
  handleALUBinary(instr);
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*dispatchTable[opcode])(nextInstr);
}

bool Executor::execALUUnary(Instr instr) {
  handleALUUnary(instr);
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*dispatchTable[opcode])(nextInstr);
}

bool Executor::execIO(Instr instr) {
  handleIO(instr);
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*dispatchTable[opcode])(nextInstr);
}

bool Executor::execBLessFloat(Instr instr) {
  handleBranch<Float>(instr, std::less<Float>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*dispatchTable[opcode])(nextInstr);
}

bool Executor::execBLessInt(Instr instr) {
  handleBranch<Int>(instr, std::less<Int>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*dispatchTable[opcode])(nextInstr);
}

bool Executor::execBEqualFloat(Instr instr) {
  handleBranch<Float>(instr, std::equal_to<Float>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*dispatchTable[opcode])(nextInstr);
}

bool Executor::execBEqualInt(Instr instr) {
  handleBranch<Int>(instr, std::equal_to<Int>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*dispatchTable[opcode])(nextInstr);
}

void Executor::handleALUBinary(Instr instr) {
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

void Executor::handleALUUnary(Instr instr) {
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

void Executor::handleIO(Instr instr) {
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

} // namespace pvm
