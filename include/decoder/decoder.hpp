#pragma once

#include <cstdint>

#include "common/instruction.hpp"

namespace pvm {

class Decoder final {
public:
  [[nodiscard]] Instruction parse(std::uint64_t bytecode) const;
};

} // namespace pvm
