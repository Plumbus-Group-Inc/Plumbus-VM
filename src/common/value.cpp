#include <sstream>
#include <variant>

#include "common/value.hpp"

namespace pvm {

Value::Value() noexcept : Value(Null()) {}
Value::Value(Value const& other) : m_data(other.m_data) {}
Value::Value(Value&& other) noexcept : m_data(std::move(other.m_data)) {}

Value& Value::operator=(Value const& other) {
    m_data = other.m_data;
    return *this;
}

Value& Value::operator=(Value&& other) noexcept {
    m_data = std::move(other.m_data);
    return *this;
}

} // namespace pvm
