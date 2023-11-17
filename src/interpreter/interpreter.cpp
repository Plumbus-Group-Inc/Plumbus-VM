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
    : m_code(code), m_exec(m_code, m_mem, ost, ist) {
}

void Interpreter::run() {
  m_exec.exec();
}

} // namespace pvm
