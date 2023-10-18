#include "memory/memory.hpp"

namespace pvm {

[[nodiscard]] std::uint64_t Memory::loadWord(Addr addr) const {
  return static_cast<std::uint64_t>(m_data.at(addr).read<Int>());
}

void Memory::storeVal(Addr addr, Value val) { m_data.at(addr) = val; }

void Memory::storeValI(Addr addr, Int val) { m_data.at(addr).write(val); }

void Memory::storeValF(Addr addr, Float val) { m_data.at(addr).write(val); }

} // namespace pvm