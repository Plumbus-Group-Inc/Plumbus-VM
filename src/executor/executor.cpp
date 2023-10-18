#include <cmath>
#include <iostream>
#include <stdexcept>

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
    execALUBinaryAddI(instr);
    break;
  case OP_ADD_F:
    execALUBinaryAddF(instr);
    break;
  case OP_SUB_I:
    execALUBinarySubI(instr);
    break;
  case OP_SUB_F:
    execALUBinarySubF(instr);
    break;
  case OP_MUL_I:
    execALUBinaryMulI(instr);
    break;
  case OP_MUL_F:
    execALUBinaryMulF(instr);
    break;
  case OP_DIV_I:
    execALUBinaryDivI(instr);
    break;
  case OP_DIV_F:
    execALUBinaryDivF(instr);
    break;
  default:
    throw std::runtime_error{"Unknown binary operation"};
  }

  nextPC();
}

void Executor::execALUBinaryAddI(Instruction instr) {
  auto lhs = m_regFile.readI(instr.rs1);
  auto rhs = m_regFile.readI(instr.rs2);
  m_regFile.writeI(instr.rd, lhs + rhs);
}

void Executor::execALUBinaryAddF(Instruction instr) {
  auto lhs = m_regFile.readF(instr.rs1);
  auto rhs = m_regFile.readF(instr.rs2);
  m_regFile.writeF(instr.rd, lhs + rhs);
}

void Executor::execALUBinarySubI(Instruction instr) {
  auto lhs = m_regFile.readI(instr.rs1);
  auto rhs = m_regFile.readI(instr.rs2);
  m_regFile.writeI(instr.rd, lhs - rhs);
}

void Executor::execALUBinarySubF(Instruction instr) {
  auto lhs = m_regFile.readF(instr.rs1);
  auto rhs = m_regFile.readF(instr.rs2);
  m_regFile.writeF(instr.rd, lhs - rhs);
}

void Executor::execALUBinaryMulI(Instruction instr) {
  auto lhs = m_regFile.readI(instr.rs1);
  auto rhs = m_regFile.readI(instr.rs2);
  m_regFile.writeI(instr.rd, lhs * rhs);
}

void Executor::execALUBinaryMulF(Instruction instr) {
  auto lhs = m_regFile.readF(instr.rs1);
  auto rhs = m_regFile.readF(instr.rs2);
  m_regFile.writeF(instr.rd, lhs * rhs);
}

void Executor::execALUBinaryDivI(Instruction instr) {
  auto lhs = m_regFile.readI(instr.rs1);
  auto rhs = m_regFile.readI(instr.rs2);
  m_regFile.writeI(instr.rd, lhs / rhs);
}

void Executor::execALUBinaryDivF(Instruction instr) {
  auto lhs = m_regFile.readF(instr.rs1);
  auto rhs = m_regFile.readF(instr.rs2);
  m_regFile.writeF(instr.rd, lhs / rhs);
}

void Executor::execALUUnary(Instruction instr) {
  switch (instr.op) {
  case OP_SQRT_I:
    execALUUnarySqrtI(instr);
    break;
  case OP_SQRT_F:
    execALUUnarySqrtF(instr);
    break;
  case OP_CAST_ITOF:
    execALUUnaryCastItoF(instr);
    break;
  case OP_CAST_FTOI:
    execALUUnaryCastFtoI(instr);
    break;
  case OP_ABS_I:
    execALUUnaryAbsI(instr);
    break;
  case OP_ABS_F:
    execALUUnaryAbsF(instr);
    break;
  default:
    throw std::runtime_error{"Unknown unary operation"};
  }

  nextPC();
}

void Executor::execALUUnarySqrtI(Instruction instr) {
  auto arg = m_regFile.readI(instr.rs1);
  m_regFile.writeI(instr.rd, std::sqrt(arg));
}

void Executor::execALUUnarySqrtF(Instruction instr) {
  auto arg = m_regFile.readF(instr.rs1);
  m_regFile.writeF(instr.rd, std::sqrt(arg));
}

void Executor::execALUUnaryCastItoF(Instruction instr) {
  auto arg = m_regFile.readI(instr.rs1);
  m_regFile.writeF(instr.rd, arg);
}

void Executor::execALUUnaryCastFtoI(Instruction instr) {
  auto arg = m_regFile.readF(instr.rs1);
  m_regFile.writeI(instr.rd, arg);
}

void Executor::execALUUnaryAbsI(Instruction instr) {
  auto arg = m_regFile.readI(instr.rs1);
  m_regFile.writeI(instr.rd, std::abs(arg));
}

void Executor::execALUUnaryAbsF(Instruction instr) {
  auto arg = m_regFile.readF(instr.rs1);
  m_regFile.writeF(instr.rd, std::fabs(arg));
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

void Executor::execBLessF(Instruction instr) {}

void Executor::execBLessI(Instruction instr) {}

void Executor::execBEQF(Instruction instr) {}

void Executor::execBEQI(Instruction instr) {}

} // namespace pvm
