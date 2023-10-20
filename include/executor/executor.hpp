#pragma once

#include <functional>
#include <iostream>
#include <memory>

#include "common/config.hpp"
#include "common/instruction.hpp"
#include "executor/regfile.hpp"
#include "memory/memory.hpp"

namespace pvm {

class Executor {
private:
  RegFile m_regFile{};
  bool m_halted{false};
  std::shared_ptr<Memory> m_mem;

public:
  explicit Executor(std::shared_ptr<Memory> mem);

  bool isHalted() const;
  void exec(Instruction instr);
  [[nodiscard]] RegFile const &getRegFile() const;

private:
  void updatePC();

  void execHalt(Instruction instr);

  template <ValueT T> void execLoadImm(Instruction instr) {
    m_regFile.write<T>(instr.rd, instr.getImm<T>());
    updatePC();
  }

  void execALUBinary(Instruction instr);
  template <ValueT T>
  void handleALUBinary(Instruction instr, std::function<T(T, T)> func) {
    auto lhs = m_regFile.read<T>(instr.rs1);
    auto rhs = m_regFile.read<T>(instr.rs2);
    m_regFile.write<T>(instr.rd, func(lhs, rhs));
  }

  void execALUUnary(Instruction instr);
  template <ValueT T1, ValueT T2>
  void handleALUUnary(Instruction instr, std::function<T2(T1)> func) {
    auto arg = m_regFile.read<T1>(instr.rs1);
    m_regFile.write<T2>(instr.rd, func(arg));
  }

  void execIO(Instruction instr);
  template <ValueT T> void execIORead(Instruction instr) {
    T val = 0;
    std::cin >> val;
    m_regFile.write(instr.rd, val);
  }

  template <ValueT T> void execIOWrite(Instruction instr) {
    std::cout << m_regFile.read<T>(instr.rs1) << std::endl;
  }

  template <ValueT T>
  void handleBranch(Instruction instr, std::function<bool(T, T)> func) {
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
