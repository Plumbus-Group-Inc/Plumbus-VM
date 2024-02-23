#pragma once

#include <array>
#include <bit>
#include <type_traits>

#include "common/config.hpp"

namespace pvm {

template <typename T>
concept RegConv = requires { sizeof(T) == sizeof(Reg) || std::is_convertible_v<T, Reg>; };

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
  template <RegConv T>
  T readReg(RegId regId) const {
    if constexpr (sizeof(T) == sizeof(Reg)) {
      return std::bit_cast<T>(readReg(regId));
    } else if constexpr (std::is_convertible_v<T, Reg>) {
      return static_cast<T>(readReg(regId));
    }
  }

  void writeReg(RegId regId, Reg val);
  template <RegConv T>
  void writeReg(RegId regId, T val) {
    if constexpr (sizeof(T) == sizeof(Reg)) {
      writeReg(regId, std::bit_cast<Reg>(val));
    } else if constexpr (std::is_convertible_v<T, Reg>) {
      writeReg(regId, static_cast<Reg>(val));
    }
  }
};

} // namespace pvm
