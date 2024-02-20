#pragma once

#include <cstddef>
#include <cstdint>

namespace pvm {

using Int = std::int32_t;
using Float = float;
using Bool = bool;

using Addr = std::uint32_t;
using Bytecode = std::uint64_t;

using RegId = std::uint32_t;
using Opcode = std::uint8_t;
using OpId = std::uint32_t;
} // namespace pvm
