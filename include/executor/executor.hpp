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
  RegFile m_regFile{};
  // Memory &m_mem;
  Code &m_code;

  std::reference_wrapper<std::istream> m_ist{std::cin};
  std::reference_wrapper<std::ostream> m_ost{std::cout};

public:
  explicit Executor(/*Memory &mem,*/ Code &code);
  Executor(/*Memory &mem,*/ Code &code, std::ostream &ost);
  Executor(/*Memory &mem,*/ Code &code, std::istream &ist);
  Executor(/*Memory &mem,*/ Code &code, std::ostream &ost, std::istream &ist);

  void exec();
  [[nodiscard]] RegFile const &getRegFile() const;
};

} // namespace pvm
