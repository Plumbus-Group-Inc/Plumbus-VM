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
  // Memory &m_mem;
  Code &m_code;

  std::reference_wrapper<std::istream> m_ist{std::cin};
  std::reference_wrapper<std::ostream> m_ost{std::cout};

public:
  explicit Executor(/*Memory &mem,*/ Code &code);
  Executor(/*Memory &mem,*/ Code &code, std::ostream &ost);
  Executor(/*Memory &mem,*/ Code &code, std::istream &ist);
  Executor(/*Memory &mem,*/ Code &code, std::ostream &ost, std::istream &ist);

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

  const std::array<bool (Executor::*)(Instr), OPCODE_NUM> dispatchTable{
      &Executor::execHalt,       &Executor::execLoadImmInt, &Executor::execLoadImmFloat,
      &Executor::execALUBinary,  &Executor::execALUUnary,   &Executor::execIO,
      &Executor::execBLessFloat, &Executor::execBLessInt,   &Executor::execBEqualFloat,
      &Executor::execBEqualInt,
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

} // namespace pvm
