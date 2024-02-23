#include <iostream>

#include "interpreter/interpreter.hpp"

namespace pvm {

Interpreter::Interpreter(const Code &code) : Interpreter(code, Config{}) {
}

Interpreter::Interpreter(const Code &code, const Config &config)
    : m_state{Decoder{}, Memory{}, Code{code}, FrameStack{{}}, 0, config} {
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
