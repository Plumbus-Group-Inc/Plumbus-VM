#pragma once

#include <iostream>

#include "common/config.hpp"
#include "common/instruction.hpp"
#include "executor/regfile.hpp"
#include "memory/memory.hpp"

namespace pvm {

class Executor {
public:
  explicit Executor(Memory &mem) : m_mem(mem) {}
  void exec(Instruction instr);
  [[nodiscard]] RegFile const &getState() const;

private:
  void nextPC();

  std::reference_wrapper<Memory> m_mem;
  RegFile m_regFile{};

  void execHalt(Instruction instr);
  template <RequestedValueConcept T> void execLoadImm(Instruction instr) {
    m_regFile.write<T>(instr.rd, instr.immi);
    nextPC();
  }

  void execALUBinary(Instruction instr);
  template <RequestedValueConcept T, typename F>
  void handleALUBinary(Instruction instr, F func) {
    auto lhs = m_regFile.read<T>(instr.rs1);
    auto rhs = m_regFile.read<T>(instr.rs2);
    m_regFile.write<T>(instr.rd, func(lhs, rhs));
  }

  void execALUUnary(Instruction instr);
  template <RequestedValueConcept T1, RequestedValueConcept T2, typename F>
  void handleALUUnary(Instruction instr, F func) {
    auto arg = m_regFile.read<T1>(instr.rs1);
    m_regFile.write<T2>(instr.rd, func(arg));
  }

  void execIO(Instruction instr);
  template <RequestedValueConcept T> void execIORead(Instruction instr) {
    T val = 0;
    std::cin >> val;
    m_regFile.write(instr.rd, val);
  }

  template <RequestedValueConcept T> void execIOWrite(Instruction instr) {
    std::cout << m_regFile.read<T>(instr.rs1) << std::endl;
  }

  void execBLessF(Instruction instr);
  void execBLessI(Instruction instr);
  void execBEQF(Instruction instr);
  void execBEQI(Instruction instr);

  template <typename F> void handleBranch(Instruction instr, F func) {
    auto lhs = m_regFile.read<Int>(instr.rs1);
    auto rhs = m_regFile.read<Int>(instr.rs2);
    if (func(lhs, rhs)) {
      auto pc = m_regFile.readPC();
      m_regFile.writePC(pc + instr.immi);
    } else {
      nextPC();
    }
  }
};

} // namespace pvm
