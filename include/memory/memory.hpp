#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>

#include "common/config.hpp"
#include "common/instruction.hpp"
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
  explicit Code(const std::vector<Instr> &data);
  explicit Code(std::vector<Instr> &&data);

  [[nodiscard]] Instr loadInstr(Addr pc) const;

private:
  std::vector<Instr> m_data{};
};

} // namespace pvm
