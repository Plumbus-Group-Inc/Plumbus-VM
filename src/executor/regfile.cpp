#include "executor/regfile.hpp"

namespace pvm {

void RegFile::write(RegisterId regId, Value val) { m_data.at(regId) = val; }

[[nodiscard]] Value RegFile::read(RegisterId regId) const { return m_data.at(regId); }

void RegFile::writePC(Addr addr) { m_pc = addr; }

[[nodiscard]] Addr RegFile::readPC() const { return m_pc; }

} // namespace pvm