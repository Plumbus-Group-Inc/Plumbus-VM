#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <ostream>

#include "common/config.hpp"
#include "common/instruction.hpp"
#include "executor/regfile.hpp"
#include "memory/memory.hpp"

namespace pvm {

class Executor final {
private:
  RegFile m_regFile;

  const Code &m_code;
  Memory &m_mem;

  std::istream &m_ist;
  std::ostream &m_ost;

public:
  Executor(const Code &code, Memory &mem, std::ostream &ost, std::istream &ist);

  void exec();

private:
  Instr getInstr();
    void updatePC() {
    auto newPC = m_regFile.readPC() + 1;
    m_regFile.writePC(newPC);
  }
};

} // namespace pvm
