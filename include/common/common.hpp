#pragma once

#include <concepts>
#include <cstdint>

namespace pvm {

constexpr std::uint8_t kBitsInByte = 8;

template <typename T>
consteval std::size_t sizeofBits() {
  return sizeof(T) * kBitsInByte;
}

template <std::size_t high, std::size_t low, std::unsigned_integral T>
constexpr T getBitsNoShift(T word) {
  static_assert(high >= low, "Incorrect bits range");
  static_assert(high < sizeofBits<T>(), "Bit index out of range");

  auto mask = ~T(0);
  if constexpr (high != sizeofBits<T>() - 1) {
    mask = ~(mask << (high + 1));
  }

  return static_cast<T>(word & mask);
}

template <std::size_t from, std::size_t to, std::unsigned_integral T>
constexpr T getBits(T word) {
  constexpr auto kSize = sizeof(T) * 8;
  constexpr auto kHigh = kSize - from - 1;
  constexpr auto kLow = kSize - to - 1;
  return static_cast<T>(getBitsNoShift<kHigh, kLow>(word) >> kLow);
}

} // namespace pvm
