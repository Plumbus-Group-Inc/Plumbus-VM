#include <cmath>
#include <iostream>
#include <stdexcept>

#include "common/config.hpp"
#include "common/instruction.hpp"
#include "executor/executor.hpp"
#include "executor/regfile.hpp"

namespace pvm {

void Executor::exec(Instruction instr) {
  switch (instr.opcode) {
  case OPCODE_HALT:
    execHalt(instr);
    break;
  case OPCODE_LOAD_IMM_I:
    execLoadImmI(instr);
    break;
  case OPCODE_LOAD_IMM_F:
    execLoadImmF(instr);
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
    execBLessF(instr);
    break;
  case OPCODE_BLESS_I:
    execBLessI(instr);
    break;
  case OPCODE_BEQ_F:
    execBEQF(instr);
    break;
  case OPCODE_BEQ_I:
    execBEQI(instr);
    break;
  default:
    throw std::runtime_error{"Unknown opcode"};
  }
}

RegFile const &Executor::getState() const { return m_regFile; }

void Executor::nextPC() {
  auto newPC = m_regFile.readPC() + 1;
  m_regFile.writePC(newPC);
}

void Executor::execHalt(Instruction instr) {}

void Executor::execLoadImmI(Instruction instr) {
  m_regFile.writeI(instr.rd, instr.immi);
  nextPC();
}

void Executor::execLoadImmF(Instruction instr) {
  m_regFile.writeF(instr.rd, instr.immf);
  nextPC();
}

void Executor::execALUBinary(Instruction instr) {
  switch (instr.op) {
  case OP_ADD_I:
    handleALUBinaryI(instr, std::plus<Int>{});
    break;
  case OP_ADD_F:
    handleALUBinaryF(instr, std::plus<Float>{});
    break;
  case OP_SUB_I:
    handleALUBinaryI(instr, std::minus<Int>{});
    break;
  case OP_SUB_F:
    handleALUBinaryF(instr, std::minus<Float>{});
    break;
  case OP_MUL_I:
    handleALUBinaryI(instr, std::multiplies<Int>{});
    break;
  case OP_MUL_F:
    handleALUBinaryF(instr, std::multiplies<Float>{});
    break;
  case OP_DIV_I:
    handleALUBinaryI(instr, std::divides<Int>{});
    break;
  case OP_DIV_F:
    handleALUBinaryF(instr, std::divides<Float>{});
    break;
  default:
    throw std::runtime_error{"Unknown binary operation"};
  }

  nextPC();
}

static Float sqrtF(Float val) { return std::sqrt(val); }
static Int sqrtI(Int val) { return std::sqrt(val); }
static Float absF(Float val) { return std::fabs(val); }
static Int absI(Int val) { return std::abs(val); }

void Executor::execALUUnary(Instruction instr) {
  switch (instr.op) {
  case OP_SQRT_I:
    handleALUUnaryI(instr, sqrtI);
    break;
  case OP_SQRT_F:
    handleALUUnaryF(instr, sqrtF);
    break;
  case OP_CAST_ITOF:
    execALUUnaryCastItoF(instr);
    break;
  case OP_CAST_FTOI:
    execALUUnaryCastFtoI(instr);
    break;
  case OP_ABS_I:
    handleALUUnaryI(instr, absI);
    break;
  case OP_ABS_F:
    handleALUUnaryF(instr, absF);
    break;
  default:
    throw std::runtime_error{"Unknown unary operation"};
  }

  nextPC();
}

void Executor::execALUUnaryCastItoF(Instruction instr) {
  auto arg = m_regFile.readI(instr.rs1);
  m_regFile.writeF(instr.rd, arg);
}

void Executor::execALUUnaryCastFtoI(Instruction instr) {
  auto arg = m_regFile.readF(instr.rs1);
  m_regFile.writeI(instr.rd, arg);
}

void Executor::execIO(Instruction instr) {
  switch (instr.op) {
  case OP_READ_I:
    execIOReadI(instr);
    break;
  case OP_READ_F:
    execIOReadF(instr);
    break;
  case OP_WRITE_I:
    execIOWriteI(instr);
    break;
  case OP_WRITE_F:
    execIOWriteF(instr);
    break;
  default:
    throw std::runtime_error{"Unknown IO operation"};
  }

  nextPC();
}

void Executor::execIOReadI(Instruction instr) {
  Int val = 0;
  std::cin >> val;
  m_regFile.writeI(instr.rd, val);
}

void Executor::execIOReadF(Instruction instr) {
  Float val = 0;
  std::cin >> val;
  m_regFile.writeF(instr.rd, val);
}

void Executor::execIOWriteI(Instruction instr) {
  std::cout << m_regFile.readI(instr.rs1) << std::endl;
}

void Executor::execIOWriteF(Instruction instr) {
  std::cout << m_regFile.readF(instr.rs1) << std::endl;
}

void Executor::execBLessF(Instruction instr) { handleBranch(instr, std::less<Float>{}); }

void Executor::execBLessI(Instruction instr) { handleBranch(instr, std::less<Int>{}); }

void Executor::execBEQF(Instruction instr) {
  handleBranch(instr, std::equal_to<Float>{});
}

void Executor::execBEQI(Instruction instr) { handleBranch(instr, std::equal_to<Int>{}); }

} // namespace pvm
