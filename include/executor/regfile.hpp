#pragma once

#include <array>

#include "common/config.hpp"
#include "common/value.hpp"

namespace pvm {

struct RegFile final {
  static constexpr std::size_t kRegNum = 1U << (sizeof(RegisterId) * 8);
  std::array<Value, kRegNum> data{};
};

} // namespace pvm
