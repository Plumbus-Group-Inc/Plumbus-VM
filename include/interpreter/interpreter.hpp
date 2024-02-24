#pragma once

#include <functional>
#include <iostream>
#include <utility>

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
using Klasses = std::vector<Klass>;

struct State final {
  Klasses klasses;
  Config config;

  Decoder dec;
  Memory mem;
  Code code;

  FrameStack stack;
  Addr pc;

  class Builder final {
  private:
    friend struct State;

    Decoder m_dec{};
    Memory m_mem{};
    Code m_code;

    FrameStack m_stack{Frame{}};
    Addr m_pc{};

    Config m_config{};

    Klasses m_klasses{
        Klass{"Null", 0},
        Klass{"Int", sizeof(Int), {{0, 0}}},
        Klass{"Float", sizeof(Float), {{0, 0}}},
        Klass{"Bool", sizeof(Bool), {{0, 0}}},
    };

  public:
    explicit Builder(Code code) : m_code(std::move(code)) {
    }
    State build();

    Builder &decoder(const Decoder &dec);
    Builder &memory(const Memory &mem);

    Builder &stack(const FrameStack &stack);
    Builder &entry(Addr pc);

    Builder &config(const Config &config);
    Builder &klass(const Klass &klass);
  };

  State(const Builder &b);

  RegFile &rf();
  [[nodiscard]] const RegFile &rf() const;
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
  [[nodiscard]] Instr getInstr() const;
};

} // namespace pvm
