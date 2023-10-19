#pragma once

#include <array>
#include <cstdint>

#include "common/config.hpp"
#include "common/value.hpp"

namespace pvm {

class Memory final {
public:
  [[nodiscard]] std::uint64_t loadWord(Addr addr) const;
  void storeVal(Addr addr, Value val);
  void storeValI(Addr addr, Int val);
  void storeValF(Addr addr, Float val);

private:
  // TODO: temporary
  std::array<Value, 1024> m_data{};
};

} // namespace pvm
