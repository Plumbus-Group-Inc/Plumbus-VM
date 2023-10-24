#include <sstream>
#include <variant>

#include "common/value.hpp"

namespace pvm {

Value::Value() noexcept : Value(Null()) {}
Value::Value(Value&& other) noexcept : m_data(std::move(other.m_data)) {}

Value& Value::operator=(Value&& other) noexcept {
    m_data = std::move(other.m_data);
    return *this;
}



Array::Array(Int size) : m_data(size) {}

Array::Array(Array&& other) noexcept : m_data(std::move(other.m_data)) {}

Array& Array::operator=(Array const& other) {
    if(this == &other) {
        return *this;
    }

    m_data = other.m_data;
    return *this;
}
Array& Array::operator=(Array&& other) noexcept {
    m_data = std::move(other.m_data);
    return *this;
}

[[nodiscard]] Int Array::size() const noexcept {
    return static_cast<Int>(m_data.size());
}
[[nodiscard]] Value Array::at(Int pos) const && {
    return m_data.at(pos);
}
[[nodiscard]] Value const& Array::at(Int pos) const & {
    return m_data.at(pos);
}
[[nodiscard]] Value& Array::at(Int pos) & {
    return m_data.at(pos);
}

void Array::resize(Int newSize) {
    m_data.resize(static_cast<size_t>(newSize), Value(Null()));
}
void Array::clear() {
    m_data.clear();
}


} // namespace pvm
