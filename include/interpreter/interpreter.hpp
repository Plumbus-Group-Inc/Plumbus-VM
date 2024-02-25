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

using Regs = std::vector<Reg>;

struct Frame final {
  RegFile rf{};
  Addr retPC{};
  Regs stack{};
};

using Frames = std::vector<Frame>;
using Klasses = std::vector<Klass>;

struct State final {
  Klasses klasses;
  Config config;

  Decoder dec;
  Memory mem;
  Code code;

  Frames callStack;
  Addr pc;

  class Builder final {
  private:
    friend struct State;

    Decoder m_dec{};
    Memory m_mem{};
    Code m_code;

    Frames m_stack{Frame{}};
    Addr m_pc{};

    Config m_config{};

    Klasses m_klasses{
        Klass{.name = "Null", .size = 0, .field2offset = {}},
        Klass{.name = "Int",
              .size = sizeof(Int),
              .field2offset = {{0, Field{0, sizeof(Int)}}}},
        Klass{.name = "Bool",
              .size = sizeof(Bool),
              .field2offset = {{0, Field{0, sizeof(Bool)}}}},
        Klass{.name = "Char",
              .size = sizeof(Char),
              .field2offset = {{0, Field{0, sizeof(Char)}}}},
        Klass{.name = "Float",
              .size = sizeof(Float),
              .field2offset = {{0, Field{0, sizeof(Float)}}}},
    };

  public:
    explicit Builder(Code code) : m_code(std::move(code)) {}
    State build();

    Builder &decoder(const Decoder &dec);
    Builder &memory(const Memory &mem);

    Builder &stack(const Frames &stack);
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
