#pragma once

#include <cstdint>

namespace pvm {

using Int = std::int64_t;
using Float = double;
using Bool = bool;
using Ref = std::uintptr_t;

using Addr = std::uint64_t;
using Bytecode = std::uint32_t;

using Reg = std::uint64_t;
using RegId = std::uint64_t;

using Opcode = std::uint8_t;
using OpId = std::uint64_t;

enum TrivialType : std::uint8_t {
  NULL_T = 0,
  INT_T,
  BOOL_T,
  FLOAT_T,
};

} // namespace pvm
