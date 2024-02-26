#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>

namespace pvm {

struct Field final {
  std::ptrdiff_t offset;
  std::size_t size;
};

using Field2Offset = std::map<std::size_t, Field>;

struct Klass final {
  std::string name{};
  std::size_t size{};
  Field2Offset field2offset{};
};

using MarkWord = std::uint16_t;
using KlassWord = std::uint16_t;

struct ObjectHeader final {
  MarkWord mark{};
  KlassWord klass{};
};

} // namespace pvm
