#ifndef __INCLUDE_OBJECTS_OBJECTS_HH__
#define __INCLUDE_OBJECTS_OBJECTS_HH__

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

using MarkWord = std::uintptr_t;
using KlassWord = std::uintptr_t;

struct ObjectHeader final {
  MarkWord mark{};
  KlassWord klass{};
};

} // namespace pvm

#endif // __INCLUDE_OBJECTS_OBJECTS_HH__
