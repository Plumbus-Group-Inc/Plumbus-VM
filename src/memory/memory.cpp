#include "memory/memory.hpp"

namespace pvm {

Code::Code(const Instrs &data) : m_data(data) {
}

Code::Code(Instrs &&data) : m_data(std::move(data)) {
}

Instr Code::loadInstr(Addr pc) const {
  return m_data[pc];
}

} // namespace pvm
