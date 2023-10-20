#include "decoder/decoder.hpp"
#include "common/instruction.hpp"

namespace pvm {

Instr Decoder::decode(std::uint64_t bytecode) const {
  return *reinterpret_cast<Instr *>(&bytecode);
}

} // namespace pvm
