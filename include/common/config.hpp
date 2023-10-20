#pragma once

#include <cstddef>
#include <cstdint>

namespace pvm {

using Int = std::int32_t;
using Float = float;

using Addr = std::uint32_t;
using Word = std::uint64_t;

using RegisterId = std::uint8_t;
using OperationId = std::uint32_t;
using Opcode = std::uint8_t;

constexpr const size_t kRegistersCount = 1ull << (sizeof(RegisterId) * 8);

} // namespace pvm
