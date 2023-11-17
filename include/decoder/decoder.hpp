#pragma once

#include <cstdint>

#include "common/instruction.hpp"

namespace pvm {

class Decoder final {
public:
  Instr decode(std::uint32_t bytecode);
};

} // namespace pvm
