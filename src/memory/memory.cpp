#include "memory/memory.hpp"

namespace pvm {

[[nodiscard]] std::uint64_t Memory::loadWord(Addr addr) const {
  return static_cast<std::uint64_t>(m_data[addr].get<Int>());
}

void Memory::storeVal(Addr addr, Value val) {
  m_data[addr] = val;
}

Code::Code(const std::vector<Instr> &data) : m_data(data) {
}
Code::Code(std::vector<Instr> &&data) : m_data(std::move(data)) {
}

Instr Code::loadInstr(Addr pc) const {
  return m_data[pc];
}

} // namespace pvm
