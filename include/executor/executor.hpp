#pragma once

#include "common/config.hpp"
#include "common/instruction.hpp"
#include "executor/regfile.hpp"

namespace pvm {

class Executor {
public:
  explicit Executor(/* Memory */);
  void exec(Instruction instr);
  [[nodiscard]] RegFile const &getState() const;

private:
  void nextPC();

  RegFile m_regFile{};
  // Memory

  void execHalt(Instruction instr);
  void execLoadImmI(Instruction instr);
  void execLoadImmF(Instruction instr);

  void execALUBinary(Instruction instr);

  template <typename F> void handleALUBinaryF(Instruction instr, F func) {
    auto lhs = m_regFile.readF(instr.rs1);
    auto rhs = m_regFile.readF(instr.rs2);
    m_regFile.writeF(instr.rd, func(lhs, rhs));
  }

  template <typename F> void handleALUBinaryI(Instruction instr, F func) {
    auto lhs = m_regFile.readI(instr.rs1);
    auto rhs = m_regFile.readI(instr.rs2);
    m_regFile.writeI(instr.rd, func(lhs, rhs));
  }

  void execALUUnary(Instruction instr);
  void execALUUnaryCastItoF(Instruction instr);
  void execALUUnaryCastFtoI(Instruction instr);

  template <typename F> void handleALUUnaryF(Instruction instr, F func) {
    auto arg = m_regFile.readF(instr.rs1);
    m_regFile.writeF(instr.rd, func(arg));
  }

  template <typename F> void handleALUUnaryI(Instruction instr, F func) {
    auto arg = m_regFile.readI(instr.rs1);
    m_regFile.writeI(instr.rd, func(arg));
  }

  void execIO(Instruction instr);
  void execIOReadI(Instruction instr);
  void execIOReadF(Instruction instr);
  void execIOWriteI(Instruction instr);
  void execIOWriteF(Instruction instr);

  void execBLessF(Instruction instr);
  void execBLessI(Instruction instr);
  void execBEQF(Instruction instr);
  void execBEQI(Instruction instr);

  template <typename F> void handleBranch(Instruction instr, F func) {
    auto lhs = m_regFile.readI(instr.rs1);
    auto rhs = m_regFile.readI(instr.rs2);
    if (func(lhs, rhs)) {
      auto pc = m_regFile.readPC();
      m_regFile.writePC(pc + instr.immi);
    } else {
      nextPC();
    }
  }
};

} // namespace pvm
