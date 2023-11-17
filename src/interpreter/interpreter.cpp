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
    : m_state{Decoder{}, RegFile{}, Memory{}, Code{code}}, m_ost(ost), m_ist(ist) {
}

Instr Interpreter::getInstr() {
  auto pc = m_state.rf.readPC();
  auto bytecode = m_state.code.loadInstr(pc);
  auto instr = m_state.dec.decode(bytecode);
  return instr;
}

} // namespace pvm
