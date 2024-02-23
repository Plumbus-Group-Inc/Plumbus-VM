#pragma once

#include <cstddef>
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

enum Type : std::uint8_t {
  INT_T = 1,
  FLOAT_T = 2,
  BOOL_T = 3,
};

} // namespace pvm
