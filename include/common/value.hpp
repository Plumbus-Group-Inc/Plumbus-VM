#pragma once

#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include <variant>
#include <vector>

#include "common/config.hpp"
#include "common/template-magic.hpp"

namespace pvm {

class Null;
class Value;
class Array;
class Function;
class Object;

class Null {};

class Array {
public:
    Array();
    ~Array() = default;

    explicit Array(Int size);

    Array(Array const& other) = default;
    Array(Array&& other) noexcept;

    Array& operator=(Array const& other);
    Array& operator=(Array&& other) noexcept;

    [[nodiscard]] Int size() const noexcept;
    [[nodiscard]] Value at(Int pos) const &&;
    [[nodiscard]] Value const& at(Int pos) const &;
    [[nodiscard]] Value& at(Int pos) &;

    [[nodiscard]] Value operator[](Int pos) const &&;
    [[nodiscard]] Value const& operator[](Int pos) const &;
    [[nodiscard]] Value& operator[](Int pos) &;

    void resize(Int newSize);
    void clear();
private:
};

template<typename Type>
concept ValueType = pvm::variadic::Contains<Type, Null, Bool, Float, Int, Array>;

class ValueMismatchError : public std::runtime_error {
public:
    ValueMismatchError(std::type_info const& requestedType, 
        std::type_info const& currentType);

    std::type_info const& m_currentType;
    std::type_info const& m_requestedType;
};

class Value {
public:
    using Variant = std::variant<Null, Bool, Float, Int, Array>;

    Value() noexcept;
    ~Value() = default; 

    template<ValueType Type>
    explicit Value(Type const& value) 
        noexcept(std::is_nothrow_copy_constructible_v<Type>);

    template<ValueType Type>
    explicit Value(Type&& value)
        noexcept(std::is_nothrow_move_constructible_v<Type>);

    Value(Value const& other);
    Value(Value&& other) noexcept;

    Value& operator=(Value const& other);
    Value& operator=(Value&& other) noexcept;

    template<ValueType Type>
    [[nodiscard]] Type get() const;

    template<ValueType Type>
    void set(Type value);

    template<ValueType Type>
    void reset(Type value);

    template<ValueType Type>
    [[nodiscard]] bool holds() const noexcept;

private:
    Variant m_data;
};

template<ValueType Type>
Value::Value(Type const& value) 
    noexcept(std::is_nothrow_copy_constructible_v<Type>) : m_data(value) {}

template<ValueType Type>
Value::Value(Type&& value)
    noexcept(std::is_nothrow_move_constructible_v<Type>) : m_data(std::forward(value)) {}

} // namespace pvm
