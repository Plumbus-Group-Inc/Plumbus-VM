#pragma once

#include <functional>
#include <memory>
#include <ostream>

#include "common/config.hpp"
#include "common/instruction.hpp"
#include "executor/regfile.hpp"
#include "memory/memory.hpp"

namespace pvm {

class Executor final {
private:
  bool m_halted{false};

  RegFile m_regFile{};
  std::shared_ptr<Memory> m_mem;

  std::reference_wrapper<std::istream> m_ist{std::cin};
  std::reference_wrapper<std::ostream> m_ost{std::cout};

public:
  explicit Executor(std::shared_ptr<Memory> mem);
  Executor(std::shared_ptr<Memory> mem, std::ostream &ost);
  Executor(std::shared_ptr<Memory> mem, std::istream &ist);
  Executor(std::shared_ptr<Memory> mem, std::ostream &ost, std::istream &ist);

  void exec(Instruction instr);
  [[nodiscard]] bool isHalted() const;
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
