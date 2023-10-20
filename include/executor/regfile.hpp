#pragma once

#include <array>
#include <cstdint>

#include "common/config.hpp"
#include "common/value.hpp"

namespace pvm {

class RegFile final {
public:
  void write(RegisterId regId, Value val);
  void writePC(Addr addr);
  template <ValuePayload RequestedType> void write(RegisterId regId, RequestedType val) {
    m_data.at(regId).overwrite(val);
  }

  [[nodiscard]] Value read(RegisterId regId) const;
  [[nodiscard]] Addr readPC() const;
  template <ValuePayload RequestedType> [[nodiscard]] auto read(RegisterId regId) const {
    return m_data.at(regId).read<RequestedType>();
  }

private:
  static constexpr std::size_t kRegNum = 1U << (sizeof(RegisterId) * 8);
  std::array<Value, kRegNum> m_data{};
  Addr m_pc{};
};

} // namespace pvm
