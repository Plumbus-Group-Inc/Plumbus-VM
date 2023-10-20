#include <cmath>
#include <functional>
#include <iostream>
#include <stdexcept>

#include "common/config.hpp"
#include "common/instruction.hpp"
#include "executor/executor.hpp"
#include "executor/regfile.hpp"

namespace pvm {

Executor::Executor(std::shared_ptr<Memory> mem) : m_mem(std::move(mem)) {}

Executor::Executor(std::shared_ptr<Memory> mem, std::ostream &ost)
    : m_mem(std::move(mem)), m_ost(ost) {}

Executor::Executor(std::shared_ptr<Memory> mem, std::istream &ist)
    : m_mem(std::move(mem)), m_ist(ist) {}

Executor::Executor(std::shared_ptr<Memory> mem, std::ostream &ost, std::istream &ist)
    : m_mem(std::move(mem)), m_ist(ist), m_ost(ost) {}

void Executor::exec(Instruction instr) {
  switch (instr.opcode) {
  case OPCODE_HALT:
    execHalt(instr);
    break;
  case OPCODE_LOAD_IMM_I:
    execLoadImm<Int>(instr);
    break;
  case OPCODE_LOAD_IMM_F:
    execLoadImm<Float>(instr);
    break;
  case OPCODE_ALU_BINARY:
    execALUBinary(instr);
    break;
  case OPCODE_ALU_UNARY:
    execALUUnary(instr);
    break;
  case OPCODE_IO:
    execIO(instr);
    break;
  case OPCODE_BLESS_F:
    handleBranch<Float>(instr, std::less<Float>{});
    break;
  case OPCODE_BLESS_I:
    handleBranch<Int>(instr, std::less<Int>{});
    break;
  case OPCODE_BEQ_F:
    handleBranch<Float>(instr, std::equal_to<Float>{});
    break;
  case OPCODE_BEQ_I:
    handleBranch<Int>(instr, std::equal_to<Int>{});
    break;
  default:
    throw std::runtime_error{"Unknown opcode"};
  }
}

bool Executor::isHalted() const { return m_halted; }

RegFile const &Executor::getRegFile() const { return m_regFile; }

void Executor::updatePC() {
  auto newPC = m_regFile.readPC() + 1;
  m_regFile.writePC(newPC);
}

void Executor::execHalt(Instruction) { m_halted = true; }

void Executor::execALUBinary(Instruction instr) {
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

void Executor::execALUUnary(Instruction instr) {
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

void Executor::execIO(Instruction instr) {
  switch (instr.op) {
  case OP_READ_I:
    execIORead<Int>(instr);
    break;
  case OP_READ_F:
    execIORead<Float>(instr);
    break;
  case OP_WRITE_I:
    execIOWrite<Int>(instr);
    break;
  case OP_WRITE_F:
    execIOWrite<Float>(instr);
    break;
  default:
    throw std::runtime_error{"Unknown IO operation"};
  }

  updatePC();
}

} // namespace pvm
