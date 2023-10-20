#include "interpreter/interpreter.hpp"

namespace pvm {

Interpreter::Interpreter(Code code) : m_code(std::move(code)), m_exec(m_mem) {}

void Interpreter::run() {
  while (!finished()) {
    runIter();
  }
}

void Interpreter::runIter() {
  m_prevPC = m_exec.getState().readPC();
  auto instrByteCode = m_code.loadWord(m_prevPC);
  auto instr = m_dec.parse(instrByteCode);
  m_exec.exec(instr);
}

bool Interpreter::finished() { return m_exec.getState().readPC() == m_prevPC; }

} // namespace pvm
