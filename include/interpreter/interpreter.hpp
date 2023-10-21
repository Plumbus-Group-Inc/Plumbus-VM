#pragma once

#include <limits>

#include "decoder/decoder.hpp"
#include "executor/executor.hpp"
#include "memory/memory.hpp"

namespace pvm {

class Interpreter final {
public:
  explicit Interpreter(Code code);
  Interpreter(Code code, std::ostream &ost);
  Interpreter(Code code, std::istream &ist);
  Interpreter(Code code, std::ostream &ost, std::istream &ist);

  void run();

private:
  std::shared_ptr<Memory> m_mem;
  Code m_code;

  Executor m_exec;
  Decoder m_dec;
};

} // namespace pvm
