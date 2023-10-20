#include "interpreter/interpreter.hpp"

namespace pvm {

Interpreter::Interpreter(Code code) : m_exec(m_mem), m_code(std::move(code)) {}

Interpreter::Interpreter(Code code, std::ostream &ost)
    : m_exec(m_mem, ost), m_code(std::move(code)) {}

Interpreter::Interpreter(Code code, std::istream &ist)
    : m_exec(m_mem, ist), m_code(std::move(code)) {}

Interpreter::Interpreter(Code code, std::ostream &ost, std::istream &ist)
    : m_exec(m_mem, ost, ist), m_code(std::move(code)) {}

void Interpreter::run() {
  while (!finished()) {
    runIter();
  }
}

void Interpreter::runIter() {
  auto pc = m_exec.getRegFile().readPC();
  auto instrByteCode = m_code.loadInstr(pc);
  auto instr = m_dec.decode(instrByteCode);
  m_exec.exec(instr);
}

bool Interpreter::finished() { return m_exec.isHalted(); }

} // namespace pvm
