#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>

#include "common/config.hpp"
#include "common/value.hpp"

namespace pvm {

class Memory final {
public:
  explicit Memory() = default;
  explicit Memory(std::vector<Value> const &data) {
    std::copy(data.begin(), data.end(), m_data.begin());
  }

  [[nodiscard]] std::uint64_t loadWord(Addr addr) const;
  void storeVal(Addr addr, Value val);
  void storeValI(Addr addr, Int val);
  void storeValF(Addr addr, Float val);

private:
  // TODO: temporary
  std::array<Value, 1024> m_data{};
};

class Code final {
public:
  explicit Code(const std::vector<Bytecode> &data);
  explicit Code(std::vector<Bytecode> &&data);

  Bytecode loadInstr(Addr pc);

private:
  std::vector<Bytecode> m_data{};
};

} // namespace pvm
