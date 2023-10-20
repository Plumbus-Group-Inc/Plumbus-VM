#pragma once

#include <limits>

#include "decoder/decoder.hpp"
#include "executor/executor.hpp"
#include "memory/memory.hpp"

namespace pvm {

class Interpreter final {
public:
  explicit Interpreter(Code code);

  void run();

private:
  std::shared_ptr<Memory> m_mem;
  Executor m_exec;
  Decoder m_dec;
  Code m_code;

  void runIter();
  bool finished();
};

} // namespace pvm
