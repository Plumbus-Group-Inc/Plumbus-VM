#include "executor/regfile.hpp"

namespace pvm {

void RegFile::write(RegId regId, Value val) { m_data[regId] = val; }

[[nodiscard]] Value RegFile::read(RegId regId) const { return m_data[regId]; }

void RegFile::writePC(Addr addr) { m_pc = addr; }

[[nodiscard]] Addr RegFile::readPC() const { return m_pc; }

void RegFile::incrementPC() { ++m_pc; }

} // namespace pvm
