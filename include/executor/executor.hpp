#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <ostream>

#include "common/config.hpp"
#include "common/instruction.hpp"
#include "executor/regfile.hpp"
#include "memory/memory.hpp"

namespace pvm {

class Executor final {
private:
  RegFile m_regFile;

  const Code &m_code;
  Memory &m_mem;

  std::istream &m_ist;
  std::ostream &m_ost;

public:
  Executor(const Code &code, Memory &mem, std::ostream &ost, std::istream &ist);

  void exec();

private:
  Instr getInstr();
  void updatePC() {
    auto newPC = m_regFile.readPC() + 1;
    m_regFile.writePC(newPC);
  }

#define PVM_TRANSFORM(opcode) bool exec_##opcode(Instr instr);
  PVM_LIST_OPCODES()
#undef PVM_TRANSFORM

  const std::array<bool (Executor::*)(Instr), OPCODE_NUM> opcodeDispatchTable{
#define PVM_TRANSFORM(opcode) &Executor::exec_##opcode,
      PVM_LIST_OPCODES()
#undef PVM_TRANSFORM
  };

#define PVM_TRANSFORM(opcode) bool exec_##opcode(Instr instr);
  PVM_LIST_BINARY_OPS()
#undef PVM_TRANSFORM

  const std::array<bool (Executor::*)(Instr), OP_BINARY_NUM> binOpDispatchTable{
#define PVM_TRANSFORM(op) &Executor::exec_##op,
      PVM_LIST_BINARY_OPS()
#undef PVM_TRANSFORM
  };

#define PVM_TRANSFORM(opcode) bool exec_##opcode(Instr instr);
  PVM_LIST_UNARY_OPS()
#undef PVM_TRANSFORM

  const std::array<bool (Executor::*)(Instr), OP_UNARY_NUM> unOpDispatchTable{
#define PVM_TRANSFORM(op) &Executor::exec_##op,
      PVM_LIST_UNARY_OPS()
#undef PVM_TRANSFORM
  };

#define PVM_TRANSFORM(opcode) bool exec_##opcode(Instr instr);
  PVM_LIST_IO_OPS()
#undef PVM_TRANSFORM

  const std::array<bool (Executor::*)(Instr), OP_IO_NUM> ioOpDispatchTable{
#define PVM_TRANSFORM(op) &Executor::exec_##op,
      PVM_LIST_IO_OPS()
#undef PVM_TRANSFORM
  };

  template <ValueT T> void handleLoadImm(Instr instr) {
    m_regFile.write<T>(instr.rd, instr.getImm<T>());
    updatePC();
  }

  template <ValueT T> void handleALUBinary(Instr instr, std::function<T(T, T)> func) {
    auto lhs = m_regFile.read<T>(instr.rs1);
    auto rhs = m_regFile.read<T>(instr.rs2);
    m_regFile.write<T>(instr.rd, func(lhs, rhs));
    updatePC();
  }

  template <ValueT T1, ValueT T2>
  void handleALUUnary(Instr instr, std::function<T2(T1)> func) {
    auto arg = m_regFile.read<T1>(instr.rs1);
    m_regFile.write<T2>(instr.rd, func(arg));
    updatePC();
  }

  template <ValueT T> void handleIORead(Instr instr) {
    T val = 0;
    m_ist >> val;
    m_regFile.write(instr.rd, val);
    updatePC();
  }

  template <ValueT T> void handleIOWrite(Instr instr) {
    m_ost << m_regFile.read<T>(instr.rs1) << std::endl;
    updatePC();
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

} // namespace pvm
