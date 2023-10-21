#include "interpreter/interpreter.hpp"

namespace pvm {

Interpreter::Interpreter(Code code)
    : m_code(std::move(code)), m_exec(/**m_mem ,*/ m_code) {}

Interpreter::Interpreter(Code code, std::ostream &ost)
    : m_code(std::move(code)), m_exec(/**m_mem ,*/ m_code, ost) {}

Interpreter::Interpreter(Code code, std::istream &ist)
    : m_code(std::move(code)), m_exec(/**m_mem ,*/ m_code, ist) {}

Interpreter::Interpreter(Code code, std::ostream &ost, std::istream &ist)
    : m_code(std::move(code)), m_exec(/**m_mem ,*/ m_code, ost, ist) {}

void Interpreter::run() { m_exec.exec(); }

} // namespace pvm
