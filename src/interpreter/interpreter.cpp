#include "interpreter/interpreter.hpp"

namespace pvm {

Interpreter::Interpreter(Code code) : m_code(std::move(code)), m_exec(m_mem) {}

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
