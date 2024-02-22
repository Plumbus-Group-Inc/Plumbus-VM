#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "common/config.hpp"
#include "common/instruction.hpp"
#include "objects/objects.hpp"

namespace pvm {

class Memory final {
public:
  explicit Memory() = default;

  std::uint8_t *allocate(std::size_t size) {
    if (m_offset + size >= m_data.size()) {
      return nullptr;
    }

    auto *res = m_data.data() + m_offset;
    m_offset += size + sizeof(ObjectHeader);
    return res;
  }

private:
  // TODO: temporary
  static constexpr std::size_t kArenaSize = 2048;
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
