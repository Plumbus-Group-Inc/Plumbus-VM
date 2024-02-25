#pragma once

#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace pvm {

constexpr std::uint8_t kBitsInByte = 8;

template <typename T>
consteval std::size_t sizeofBits() {
  return sizeof(T) * kBitsInByte;
}

template <std::size_t msb, std::size_t lsb, std::unsigned_integral T>
constexpr T getBitsNoShift(T word) {
  static_assert(msb >= lsb, "Incorrect bits range");
  static_assert(msb < sizeofBits<T>(), "Bit index out of range");

  auto mask = ~T(0);
  if constexpr (msb != sizeofBits<T>() - 1) {
    mask = ~(mask << (msb + 1));
  }

  return static_cast<T>(word & mask);
}

template <std::size_t msb, std::size_t lsb, std::unsigned_integral T>
constexpr T getBits(T word) {
  return static_cast<T>(getBitsNoShift<msb, lsb>(word) >> lsb);
}

template <std::size_t from, std::size_t to, std::unsigned_integral T>
constexpr T getBitsReverse(T word) {
  constexpr auto kMSB = sizeofBits<T>() - from - 1;
  constexpr auto kLSB = sizeofBits<T>() - to - 1;
  return getBits<kMSB, kLSB>(word);
}

struct BitManipError : std::runtime_error {
  explicit BitManipError(const char *msg) : std::runtime_error(msg) {}
  explicit BitManipError(const std::string &msg) : std::runtime_error(msg) {}
};

template <std::unsigned_integral T>
T getBitsNoShift(T word, std::size_t msb, std::size_t lsb) {
  if (msb < lsb) {
    throw BitManipError{"Incorrect bits range: msb = " + std::to_string(msb) +
                        ", lsb = " + std::to_string(lsb)};
  }

  if (msb >= sizeofBits<T>()) {
    throw BitManipError{"Bit index out of range: msb = " + std::to_string(msb)};
  }

  auto mask = ~T(0);
  if (msb != sizeofBits<T>() - 1) {
    mask = ~(mask << (msb + 1));
  }

  return static_cast<T>(word & mask);
}

template <std::unsigned_integral T>
T getBits(T word, std::size_t msb, std::size_t lsb) {
  return static_cast<T>(getBitsNoShift(word, msb, lsb) >> lsb);
}

template <std::unsigned_integral T>
T getBitsReverse(T word, std::size_t from, std::size_t to) {
  auto msb = sizeofBits<T>() - from - 1;
  auto lsb = sizeofBits<T>() - to - 1;
  return getBits(word, msb, lsb);
}

} // namespace pvm
