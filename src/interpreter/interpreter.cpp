#include <iostream>

#include "interpreter/interpreter.hpp"

namespace pvm {

State State::Builder::build() {
  return State{*this};
}

State::Builder &State::Builder::decoder(const Decoder &dec) {
  m_dec = dec;
  return *this;
}

State::Builder &State::Builder::memory(const Memory &mem) {
  m_mem = mem;
  return *this;
}

State::Builder &State::Builder::stack(const FrameStack &stack) {
  m_stack = stack;
  return *this;
}

State::Builder &State::Builder::entry(Addr pc) {
  m_pc = pc;
  return *this;
}

State::Builder &State::Builder::config(const Config &config) {
  m_config = config;
  return *this;
}

State::Builder &State::Builder::klass(const Klass &klass) {
  m_klasses.push_back(klass);
  return *this;
}

State::State(const Builder &b)
    : klasses(b.m_klasses), config(b.m_config), dec(b.m_dec), mem(b.m_mem),
      code(b.m_code), stack(b.m_stack), pc(b.m_pc) {
}

RegFile &State::rf() {
  return stack.back().rf;
}

[[nodiscard]] const RegFile &State::rf() const {
  return stack.back().rf;
}

Interpreter::Interpreter(const Code &code) : Interpreter(code, Config{}) {
}

Interpreter::Interpreter(const Code &code, const Config &config)
    : m_state{State::Builder(code).config(config)} {
}

Instr Interpreter::getInstr() {
  auto pc = m_state.pc;
  auto instr = m_state.code.loadInstr(pc);
  return instr;
}

const State &Interpreter::getState() const {
  return m_state;
}

} // namespace pvm
