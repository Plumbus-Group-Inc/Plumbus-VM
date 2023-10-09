#pragma once

#include <cstddef>
#include <cstdint>

namespace pvm {

using Int = int32_t;
using Float = float;

using RegisterId = uint8_t;
using OperationId = uint32_t;
using Opcode = uint8_t;

constexpr const size_t kRegistersCount = 1ull << (sizeof(RegisterId) * 8);

}
