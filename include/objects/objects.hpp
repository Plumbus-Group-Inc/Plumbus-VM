#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>

namespace pvm {

struct Klass final {
  std::string name{};
  std::size_t size{};
  std::map<std::size_t, std::iter_difference_t<std::uint8_t *>> field2offset{};
};

using MarkWord = std::uint32_t;
using KlassWord = std::uint32_t;

struct ObjectHeader final {
  MarkWord mark{};
  KlassWord klass{};
};

} // namespace pvm
