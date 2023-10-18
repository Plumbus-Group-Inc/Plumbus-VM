#pragma once

namespace pvm {

class Executor {
public:
  explicit Executor(/* Memory */);
  void exec(/* instr */);

private:
  void nextPC();

  // Reg file
  // Memory
};

} // namespace pvm
