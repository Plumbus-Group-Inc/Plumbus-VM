#pragma once

#include <array>
#include <cstdint>

#include "common/config.hpp"
#include "common/value.hpp"

namespace pvm {

class RegFile final {
public:
  void write(RegId regId, Value val);
  [[nodiscard]] Value read(RegId regId) const;

  void incrementPC();
  void writePC(Addr addr);
  [[nodiscard]] Addr readPC() const;

private:
  static constexpr std::size_t kRegNum = 1U << (sizeof(RegId) * 8);
  std::array<Value, kRegNum> m_data{};
  Addr m_pc{};
};

} // namespace pvm
