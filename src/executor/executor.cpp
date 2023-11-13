#include "decoder/decoder.hpp"
#include "executor/executor.hpp"

namespace pvm {

Executor::Executor(const Code &code, Memory &mem, std::ostream &ost, std::istream &ist)
    : m_mem(mem), m_code(code), m_ist(ist), m_ost(ost) {
}

Instr Executor::getInstr() {
  Decoder dec{};
  auto pc = m_regFile.readPC();
  auto bytecode = m_code.loadInstr(pc);
  auto instr = dec.decode(bytecode);
  return instr;
}

} // namespace pvm
