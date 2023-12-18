#include <iostream>

#include "interpreter/interpreter.hpp"

namespace pvm {

Interpreter::Interpreter(const Code &code) : Interpreter(code, std::cout, std::cin) {
}

Interpreter::Interpreter(const Code &code, std::ostream &ost)
    : Interpreter(code, ost, std::cin) {
}

Interpreter::Interpreter(const Code &code, std::istream &ist)
    : Interpreter(code, std::cout, ist) {
}

Interpreter::Interpreter(const Code &code, std::ostream &ost, std::istream &ist)
    : m_state{Decoder{}, RegFile{}, Memory{}, Code{code}, ost, ist} {
}

Instr Interpreter::getInstr() {
  auto pc = m_state.rf.readPC();
  auto instr = m_state.code.loadInstr(pc);
  return instr;
}

const Interpreter::State &Interpreter::getState() const {
  return m_state;
}

} // namespace pvm
