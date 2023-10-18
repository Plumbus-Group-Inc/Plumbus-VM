#include "decoder/decoder.hpp"
#include "common/instruction.hpp"

namespace pvm {

Instruction Decoder::parse(std::uint64_t bytecode) const {
  return *reinterpret_cast<Instruction *>(&bytecode);
}

} // namespace pvm
