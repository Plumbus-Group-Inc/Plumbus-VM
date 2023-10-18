#pragma once

#include "executor/regfile.hpp"

namespace pvm {

class Executor {
public:
  explicit Executor(/* Memory */);
  void exec(/* instr */);

private:
  void nextPC();

  RegFile m_regFile{};
  // Memory
};

} // namespace pvm
