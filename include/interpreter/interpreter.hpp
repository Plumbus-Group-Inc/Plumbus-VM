#pragma once

#include "decoder/decoder.hpp"
#include "executor/executor.hpp"
#include "memory/memory.hpp"

namespace pvm {

class Interpreter final {
public:
  void run();

private:
  Memory m_mem;
  Decoder m_dec;
  Executor m_exec;

  void runIter();
  bool finished();
};

} // namespace pvm
