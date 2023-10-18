#pragma once

#include "common/instruction.hpp"
#include "executor/regfile.hpp"

namespace pvm {

class Executor {
public:
  explicit Executor(/* Memory */);
  void exec(Instruction instr);

private:
  void nextPC();

  RegFile m_regFile{};
  // Memory

  void execHalt(Instruction instr);
  void execLoadImmI(Instruction instr);
  void execLoadImmF(Instruction instr);

  void execALUBinary(Instruction instr);
  void execALUBinaryAddI(Instruction instr);
  void execALUBinaryAddF(Instruction instr);
  void execALUBinarySubI(Instruction instr);
  void execALUBinarySubF(Instruction instr);
  void execALUBinaryMulI(Instruction instr);
  void execALUBinaryMulF(Instruction instr);
  void execALUBinaryDivI(Instruction instr);
  void execALUBinaryDivF(Instruction instr);

  void execALUUnary(Instruction instr);
  void execALUUnarySqrtI(Instruction instr);
  void execALUUnarySqrtF(Instruction instr);
  void execALUUnaryCastItoF(Instruction instr);
  void execALUUnaryCastFtoI(Instruction instr);
  void execALUUnaryAbsI(Instruction instr);
  void execALUUnaryAbsF(Instruction instr);

  void execIO(Instruction instr);
  void execIOReadI(Instruction instr);
  void execIOReadF(Instruction instr);
  void execIOWriteI(Instruction instr);
  void execIOWriteF(Instruction instr);

  void execBLessF(Instruction instr);
  void execBLessI(Instruction instr);
  void execBEQF(Instruction instr);
  void execBEQI(Instruction instr);
};

} // namespace pvm
