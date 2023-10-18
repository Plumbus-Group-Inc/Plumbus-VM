#include "executor/regfile.hpp"

namespace pvm {

void RegFile::writeF(RegisterId regId, Float val) { m_data.at(regId).write(val); }
void RegFile::writeI(RegisterId regId, Int val) { m_data.at(regId).write(val); }
void RegFile::write(RegisterId regId, Value val) { m_data.at(regId) = val; }

[[nodiscard]] Float RegFile::readF(RegisterId regId) const {
  return m_data.at(regId).read<Float>();
}
[[nodiscard]] Int RegFile::readI(RegisterId regId) const {
  return m_data.at(regId).read<Int>();
}
[[nodiscard]] Value RegFile::read(RegisterId regId) const { return m_data.at(regId); }

} // namespace pvm
