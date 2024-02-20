#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "common/config.hpp"
#include "common/instruction.hpp"

namespace pvm {

class Memory final {
public:
  explicit Memory() = default;

  std::uint8_t *allocate(std::size_t size) {
    auto *res = m_data.data();
    m_offset += size;
    return res;
  }

private:
  // TODO: temporary
  static constexpr std::size_t kArenaSize = 1024;
  std::array<std::uint8_t, kArenaSize> m_data{};
  std::size_t m_offset{};
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
