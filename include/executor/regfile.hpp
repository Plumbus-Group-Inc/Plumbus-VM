#pragma once

#include <array>
#include <cstdint>

#include "common/config.hpp"
#include "common/value.hpp"

namespace pvm {

class RegFile final {
public:
  void writeF(RegisterId regId, Float val);
  void writeI(RegisterId regId, Int val);
  void write(RegisterId regId, Value val);
  void writePC(Addr addr);

  [[nodiscard]] Float readF(RegisterId regId) const;
  [[nodiscard]] Int readI(RegisterId regId) const;
  [[nodiscard]] Value read(RegisterId regId) const;
  [[nodiscard]] Addr readPC() const;

private:
  static constexpr std::size_t kRegNum = 1U << (sizeof(RegisterId) * 8);
  std::array<Value, kRegNum> m_data{};
  Addr m_pc{};
};

} // namespace pvm
