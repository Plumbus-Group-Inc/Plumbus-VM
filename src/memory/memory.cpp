#include "memory/memory.hpp"

namespace pvm {

[[nodiscard]] std::uint64_t Memory::loadWord(Addr addr) const {
  return static_cast<std::uint64_t>(m_data.at(addr).read<Int>());
}

void Memory::storeVal(Addr addr, Value val) { m_data.at(addr) = val; }
void Memory::storeValI(Addr addr, Int val) { m_data.at(addr).write(val); }
void Memory::storeValF(Addr addr, Float val) { m_data.at(addr).write(val); }

Code::Code(const std::vector<Word> &data) : m_data(data) {}
Code::Code(std::vector<Word> &&data) : m_data(std::move(data)) {}

Word Code::loadWord(Addr pc) { return m_data.at(pc); }

} // namespace pvm