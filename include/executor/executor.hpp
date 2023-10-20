#pragma once

#include <functional>
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
  [[nodiscard]] RegFile const &getRegFile() const;

private:
  void nextPC();

  std::reference_wrapper<Memory> m_mem;
  RegFile m_regFile{};

  void execHalt(Instruction instr);
  template <ValuePayload T> void execLoadImm(Instruction instr) {
    if constexpr (std::is_same_v<T, Int>) {
      m_regFile.write<T>(instr.rd, instr.immi);
    } else {
      m_regFile.write<T>(instr.rd, instr.immf);
    }
    nextPC();
  }

  void execALUBinary(Instruction instr);
  template <ValuePayload T>
  void handleALUBinary(Instruction instr, std::function<T(T, T)> func) {
    auto lhs = m_regFile.read<T>(instr.rs1);
    auto rhs = m_regFile.read<T>(instr.rs2);
    m_regFile.write<T>(instr.rd, func(lhs, rhs));
  }

  void execALUUnary(Instruction instr);
  template <ValuePayload T1, ValuePayload T2>
  void handleALUUnary(Instruction instr, std::function<T2(T1)> func) {
    auto arg = m_regFile.read<T1>(instr.rs1);
    m_regFile.write<T2>(instr.rd, func(arg));
  }

  void execIO(Instruction instr);
  template <ValuePayload T> void execIORead(Instruction instr) {
    T val = 0;
    std::cin >> val;
    m_regFile.write(instr.rd, val);
  }

  template <ValuePayload T> void execIOWrite(Instruction instr) {
    std::cout << m_regFile.read<T>(instr.rs1) << std::endl;
  }

  void execBLessF(Instruction instr);
  void execBLessI(Instruction instr);
  void execBEQF(Instruction instr);
  void execBEQI(Instruction instr);

  template <ValuePayload T>
  void handleBranch(Instruction instr, std::function<bool(T, T)> func) {
    auto lhs = m_regFile.read<T>(instr.rs1);
    auto rhs = m_regFile.read<T>(instr.rs2);
    if (func(lhs, rhs)) {
      auto pc = m_regFile.readPC();
      m_regFile.writePC(pc + instr.immi);
    } else {
      nextPC();
    }
  }
};

} // namespace pvm
