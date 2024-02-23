#pragma once

#include <functional>
#include <iostream>

#include "decoder/decoder.hpp"
#include "memory/memory.hpp"
#include "memory/regfile.hpp"
#include "objects/objects.hpp"

namespace pvm {

struct Config final {
  std::reference_wrapper<std::ostream> ost{std::cout};
  std::reference_wrapper<std::istream> ist{std::cin};
};

struct Frame final {
  RegFile rf{};
  Addr retPC{};
};

using FrameStack = std::vector<Frame>;

struct State final {
  Decoder dec;
  Memory mem;
  Code code;

  FrameStack stack{};
  Addr pc{};

  Config config;

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

class Interpreter final {
private:
  State m_state;

public:
  explicit Interpreter(const Code &code);
  Interpreter(const Code &code, const Config &config);

  void run();
  [[nodiscard]] const State &getState() const;

private:
  Instr getInstr();
};

} // namespace pvm
