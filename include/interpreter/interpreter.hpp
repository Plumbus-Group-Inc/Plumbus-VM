#pragma once

#include <limits>

#include "decoder/decoder.hpp"
#include "interpreter/regfile.hpp"
#include "memory/memory.hpp"

namespace pvm {

class Interpreter final {
public:
  struct State final {
    Decoder dec;
    RegFile rf;
    Memory mem;
    Code code;
  };

private:
  State m_state;
  std::ostream &m_ost;
  std::istream &m_ist;

public:
  explicit Interpreter(const Code &code);
  Interpreter(const Code &code, std::ostream &ost);
  Interpreter(const Code &code, std::istream &ist);
  Interpreter(const Code &code, std::ostream &ost, std::istream &ist);

  void run();

private:
  Instr getInstr();
};

} // namespace pvm
