#include "memory/memory.hpp"

namespace pvm {

[[nodiscard]] std::uint64_t Memory::loadWord(Addr addr) const {
  return static_cast<std::uint64_t>(m_data[addr].read<Int>());
}

void Memory::storeVal(Addr addr, Value val) { m_data[addr] = val; }

Code::Code(const std::vector<Bytecode> &data) : m_data(data) {}
Code::Code(std::vector<Bytecode> &&data) : m_data(std::move(data)) {}

Bytecode Code::loadInstr(Addr pc) { return m_data[pc]; }

} // namespace pvm