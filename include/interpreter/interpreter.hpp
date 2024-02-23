#pragma once

#include "decoder/decoder.hpp"
#include "memory/memory.hpp"
#include "memory/regfile.hpp"
#include "objects/objects.hpp"

namespace pvm {

struct Frame final {
  RegFile rf{};
  Addr retPC{};
};

class Interpreter final {
public:
  struct State final {
    Decoder dec;
    Memory mem;
    Code code;

    std::ostream &ost;
    std::istream &ist;

    std::vector<Frame> stack{};
    Addr pc{};

    std::array<Klass, 1024> klasses{
        Klass{"Null", 0},
        Klass{"Int", sizeof(Int), {{0, 0}}},
        Klass{"Float", sizeof(Float), {{0, 0}}},
        Klass{"Bool", sizeof(Bool), {{0, 0}}},
    };

    auto &rf() {
      return stack.back().rf;
    }

    [[nodiscard]] const auto &rf() const {
      return stack.back().rf;
    }
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
