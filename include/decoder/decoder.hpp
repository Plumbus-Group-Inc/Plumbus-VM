#pragma once

#include "common/instruction.hpp"

namespace pvm {

class Decoder final {
public:
  Instr decode(Bytecode bytecode);
};

} // namespace pvm
