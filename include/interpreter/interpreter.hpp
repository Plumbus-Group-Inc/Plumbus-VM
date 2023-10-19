#pragma once

#include <limits>

#include "decoder/decoder.hpp"
#include "executor/executor.hpp"
#include "memory/memory.hpp"

namespace pvm {

class Interpreter final {
public:
  explicit Interpreter(const Memory &mem);

  void run();

private:
  Addr m_prevPC{std::numeric_limits<Addr>::max()};

  Memory m_mem;
  Decoder m_dec{};
  Executor m_exec;

  void runIter();
  bool finished();
};

} // namespace pvm
