#pragma once

#include <limits>

#include "decoder/decoder.hpp"
#include "memory/memory.hpp"
#include "memory/regfile.hpp"

namespace pvm {

class Interpreter final {
public:
  struct State final {
    Decoder dec;
    RegFile rf;
    Memory mem;
    Code code;
    std::ostream &ost;
    std::istream &ist;
    bool halted = false;
  };

private:
  State m_state;

public:
  explicit Interpreter(const Code &code);
  Interpreter(const Code &code, std::ostream &ost);
  Interpreter(const Code &code, std::istream &ist);
  Interpreter(const Code &code, std::ostream &ost, std::istream &ist);

  void run();
  [[nodiscard]] const State &getState() const;

private:
  Instr getInstr();
};

} // namespace pvm
