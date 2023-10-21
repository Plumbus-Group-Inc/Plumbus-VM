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
  (this->*opcodeDispatchTable[opcode])(instr);
}

Instr Executor::getInstr() {
  Decoder dec{};
  auto pc = m_regFile.readPC();
  auto bytecode = m_code.loadInstr(pc);
  auto instr = dec.decode(bytecode);
  return instr;
}

bool Executor::exec_ALU_BINARY(Instr instr) {
  auto opcode = instr.getImm<OpId>();
  return (this->*binOpDispatchTable[opcode])(instr);
}

bool Executor::exec_ALU_UNARY(Instr instr) {
  auto opcode = instr.getImm<OpId>();
  return (this->*unOpDispatchTable[opcode])(instr);
}

bool Executor::exec_IO(Instr instr) {
  auto opcode = instr.getImm<OpId>();
  return (this->*ioOpDispatchTable[opcode])(instr);
}

bool Executor::exec_HALT(Instr instr) { return true; }

bool Executor::exec_LOAD_IMM_I(Instr instr) {
  handleLoadImm<Int>(instr);
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_LOAD_IMM_F(Instr instr) {
  handleLoadImm<Float>(instr);
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_BLESS_F(Instr instr) {
  handleBranch<Float>(instr, std::less<Float>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_BLESS_I(Instr instr) {
  handleBranch<Int>(instr, std::less<Int>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_BEQ_F(Instr instr) {
  handleBranch<Float>(instr, std::equal_to<Float>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_BEQ_I(Instr instr) {
  handleBranch<Int>(instr, std::equal_to<Int>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_ADD_I(Instr instr) {
  handleALUBinary<Int>(instr, std::plus<Int>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_ADD_F(Instr instr) {
  handleALUBinary<Float>(instr, std::plus<Float>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_SUB_I(Instr instr) {
  handleALUBinary<Int>(instr, std::minus<Int>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_SUB_F(Instr instr) {
  handleALUBinary<Float>(instr, std::minus<Float>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_MUL_I(Instr instr) {
  handleALUBinary<Int>(instr, std::multiplies<Int>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_MUL_F(Instr instr) {
  handleALUBinary<Float>(instr, std::multiplies<Float>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_DIV_I(Instr instr) {
  handleALUBinary<Int>(instr, std::divides<Int>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_DIV_F(Instr instr) {
  handleALUBinary<Float>(instr, std::divides<Float>{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_SQRT_I(Instr instr) {
  handleALUUnary<Int, Int>(instr, [](Int val) { return std::sqrt(val); });
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_SQRT_F(Instr instr) {
  handleALUUnary<Float, Float>(instr, [](Float val) { return std::sqrt(val); });
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_CAST_ITOF(Instr instr) {
  handleALUUnary<Int, Float>(instr, std::identity{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_CAST_FTOI(Instr instr) {
  handleALUUnary<Float, Int>(instr, std::identity{});
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_ABS_I(Instr instr) {
  handleALUUnary<Int, Int>(instr, [](Int val) { return std::abs(val); });
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_ABS_F(Instr instr) {
  handleALUUnary<Float, Float>(instr, [](Float val) { return std::fabs(val); });
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_READ_I(Instr instr) {
  handleIORead<Int>(instr);
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_READ_F(Instr instr) {
  handleIORead<Float>(instr);
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_WRITE_I(Instr instr) {
  handleIORead<Int>(instr);
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

bool Executor::exec_WRITE_F(Instr instr) {
  handleIOWrite<Float>(instr);
  auto nextInstr = getInstr();
  auto opcode = nextInstr.opcode;
  return (this->*opcodeDispatchTable[opcode])(nextInstr);
}

} // namespace pvm
