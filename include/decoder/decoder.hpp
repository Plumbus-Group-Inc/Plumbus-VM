#pragma once

#include <cstdint>

#include "common/instruction.hpp"

namespace pvm {

class Decoder final {
public:
  [[nodiscard]] Instr decode(std::uint64_t bytecode) const;
};

} // namespace pvm
