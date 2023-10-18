#pragma once

#include <cstddef>
#include <cstdint>

namespace pvm {

using Addr = std::uint64_t;
using Int = std::int32_t;
using Float = float;

using RegisterId = std::uint8_t;
using OperationId = std::uint32_t;
using Opcode = std::uint8_t;

constexpr const size_t kRegistersCount = 1ull << (sizeof(RegisterId) * 8);

} // namespace pvm
