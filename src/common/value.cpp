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

template<ValueType Type>
[[nodiscard]] Type Value::get() const {
    std::add_pointer_t<std::add_const_t<Type>> pvalue =
        std::get_if<Type>(&m_data);
    if(pvalue == nullptr) {
        throw ValueMismatchError(typeid(Type), typeid(Type));
    }

    return *pvalue;
}

template<ValueType Type>
void Value::set(Type value) {
    if(!this->holds<Type>()) {
        throw ValueMismatchError(typeid(Type), typeid(Type));
    }

    m_data = value;
}

template<ValueType Type>
void Value::reset(Type value) {
    m_data = value;
}

template<ValueType Type>
[[nodiscard]] bool Value::holds() const noexcept {
    return std::holds_alternative<Type>(m_data);
}



} // namespace pvm
