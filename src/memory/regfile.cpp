#include "memory/regfile.hpp"

namespace pvm {

void RegFile::writeAcc(Value &&val) {
  m_accum = std::move(val);
}

void RegFile::writeAcc(const Value &val) {
  m_accum = val;
}

[[nodiscard]] Value RegFile::readAcc() const {
  return m_accum;
}

void RegFile::writeReg(RegId regId, Value &&val) {
  m_data[regId] = std::move(val);
}

void RegFile::writeReg(RegId regId, const Value &val) {
  m_data[regId] = val;
}

[[nodiscard]] Value RegFile::readReg(RegId regId) const {
  return m_data[regId];
}

void RegFile::writePC(Addr addr) {
  m_pc = addr;
}

[[nodiscard]] Addr RegFile::readPC() const {
  return m_pc;
}

void RegFile::incrementPC() {
  ++m_pc;
}

} // namespace pvm
