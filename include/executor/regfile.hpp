#pragma once

#include <array>
#include <cstdint>

#include "common/config.hpp"
#include "common/value.hpp"

namespace pvm {

class RegFile final {
public:
  void write(RegId regId, Value val);
  void writePC(Addr addr);
  template <ValueT T> void write(RegId regId, T val) {
    m_data[regId].overwrite(val);
  }

  [[nodiscard]] Value read(RegId regId) const;
  [[nodiscard]] Addr readPC() const;
  template <ValueT T> [[nodiscard]] auto read(RegId regId) const {
    return m_data[regId].read<T>();
  }

private:
  static constexpr std::size_t kRegNum = 1U << (sizeof(RegId) * 8);
  std::array<Value, kRegNum> m_data{};
  Addr m_pc{};
};

} // namespace pvm
