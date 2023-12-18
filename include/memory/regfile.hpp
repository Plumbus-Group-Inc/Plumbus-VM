#pragma once

#include <array>
#include <cstdint>

#include "common/config.hpp"
#include "common/value.hpp"

namespace pvm {

class RegFile final {
private:
  static constexpr std::size_t kRegNum = (1U << (sizeof(RegId) * 8));

  std::array<Value, kRegNum> m_data{};
  Value m_accum{};
  Addr m_pc{};

public:
  void writeAcc(Value val);
  void writeAcc(Value &&val);
  [[nodiscard]] Value readAcc() const;

  void writeReg(RegId regId, Value val);
  void writeReg(RegId regId, Value &&val);
  [[nodiscard]] Value readReg(RegId regId) const;

  void incrementPC();
  void writePC(Addr addr);
  [[nodiscard]] Addr readPC() const;
};

} // namespace pvm
