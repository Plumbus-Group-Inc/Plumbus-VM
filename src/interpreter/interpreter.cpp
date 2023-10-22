#include "interpreter/interpreter.hpp"

namespace pvm {

Interpreter::Interpreter(Code code)
    : m_code(std::move(code)), m_exec(m_code, m_mem, std::cout, std::cin) {
}

Interpreter::Interpreter(Code code, std::ostream &ost)
    : m_code(std::move(code)), m_exec(m_code, m_mem, ost, std::cin) {
}

Interpreter::Interpreter(Code code, std::istream &ist)
    : m_code(std::move(code)), m_exec(m_code, m_mem, std::cout, ist) {
}

Interpreter::Interpreter(Code code, std::ostream &ost, std::istream &ist)
    : m_code(std::move(code)), m_exec(m_code, m_mem, ost, ist) {
}

void Interpreter::run() {
  m_exec.exec();
}

} // namespace pvm
