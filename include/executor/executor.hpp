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

  template <ValueAlt T>
  void handleLoadImm(Instr instr) {
    m_regFile.write(instr.rd, Value(instr.getImm<T>()));
    m_regFile.incrementPC();
  }

  template <ValueAlt T>
  void handleALUBinary(Instr instr, std::function<T(T, T)> func) {
    auto lhs = m_regFile.read(instr.rs1).read<T>();
    auto rhs = m_regFile.read(instr.rs2).read<T>();
    m_regFile.write(instr.rd, Value(func(lhs, rhs)));
    m_regFile.incrementPC();
  }

  template <ValueAlt T1, ValueAlt T2>
  void handleALUUnary(Instr instr, std::function<T2(T1)> func) {
    auto arg = m_regFile.read(instr.rs1).read<T1>();
    m_regFile.write(instr.rd, Value(func(arg)));
    m_regFile.incrementPC();
  }

  template <ValueAlt T>
  void handleIORead(Instr instr) {
    T val = 0;
    m_ist >> val;
    m_regFile.write(instr.rd, Value(val));
    m_regFile.incrementPC();
  }

  template <ValueAlt T>
  void handleIOWrite(Instr instr) {
    m_ost << m_regFile.read(instr.rs1).read<T>() << std::endl;
    m_regFile.incrementPC();
  }

  template <ValueAlt T>
  void handleBranch(Instr instr, std::function<bool(T, T)> func) {
    auto lhs = m_regFile.read(instr.rs1).read<T>();
    auto rhs = m_regFile.read(instr.rs2).read<T>();
    if (!func(lhs, rhs)) {
      m_regFile.incrementPC();
    } else {
      auto pc = m_regFile.readPC();
      m_regFile.writePC(pc + instr.getImm<Int>());
    }
  }
};

} // namespace pvm
