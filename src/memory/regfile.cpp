#include "memory/regfile.hpp"

namespace pvm {

[[nodiscard]] Reg RegFile::readAcc() const { return readReg(0); }

void RegFile::writeReg(RegId regId, Reg val) { m_data[regId] = val; }

[[nodiscard]] Reg RegFile::readReg(RegId regId) const { return m_data[regId]; }

} // namespace pvm
