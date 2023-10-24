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

private:
  // TODO: temporary
  std::array<Value, 1024> m_data{};
};

class Code final {
public:
  explicit Code(const std::vector<Bytecode> &data);
  explicit Code(std::vector<Bytecode> &&data);

  [[nodiscard]] Bytecode loadInstr(Addr pc) const;

private:
  std::vector<Bytecode> m_data{};
};

} // namespace pvm
