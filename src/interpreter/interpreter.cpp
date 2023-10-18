#include "interpreter/interpreter.hpp"

namespace pvm {

void Interpreter::run() {
  while (!finished()) {
    runIter();
  }
}

void Interpreter::runIter() {
  auto pc = m_exec.getState().readPC();
  auto instrByteCode = m_mem.loadWord(pc);
  auto instr = m_dec.parse(instrByteCode);
  m_exec.exec(instr);
}

} // namespace pvm
