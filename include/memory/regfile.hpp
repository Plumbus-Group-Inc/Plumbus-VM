#pragma once

#include <array>
#include <bit>
#include <cstdint>
#include <type_traits>

#include "common/config.hpp"

namespace pvm {

template <typename T>
concept RegSized = requires { sizeof(T) == sizeof(Reg); };

class RegFile final {
private:
  static constexpr std::size_t kRegNum = 256;

  std::array<Reg, kRegNum> m_data{};

public:
  [[nodiscard]] Reg readAcc() const;

  template <typename T>
  void writeAcc(T val) {
    writeReg(0, val);
  }

  [[nodiscard]] Reg readReg(RegId regId) const;

  void writeReg(RegId regId, Reg val);
  template <typename T>
  requires(sizeof(T) == sizeof(Reg) || std::is_convertible_v<T, Reg>)
  void writeReg(RegId regId, T val) {
    if constexpr (sizeof(T) == sizeof(Reg)) {
      writeReg(regId, std::bit_cast<Reg>(val));
    } else if constexpr (std::is_convertible_v<T, Reg>) {
      writeReg(regId, static_cast<Reg>(val));
    }
  }
};

} // namespace pvm
