#pragma once

#include "decoder/decoder.hpp"
#include "executor/executor.hpp"

namespace pvm {

class Interpreter final {
public:
  void run();

private:
  Decoder m_dec;
  Executor m_exec;
  // memory
  // pc
};

} // namespace pvm
