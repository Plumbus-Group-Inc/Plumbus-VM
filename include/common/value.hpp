#pragma once

#include "common/config.hpp"
#include <cassert>
#include <exception>
#include <stdexcept>
#include <variant>

namespace pvm {

template <typename T>
concept ValueAlt = std::is_same_v<T, Int> || std::is_same_v<T, Float>;

class Value final {
public:
  enum class TypeId {
    UNDEFINED,
    INT,
    FLOAT,
  };

  union Union {
    Int i;
    Float f;
  };

  using Variant = std::variant<Int, Float>;

private:
  template <ValueAlt T>
  struct TypeIdGetter;

public:
  template <ValueAlt T>
  static constexpr TypeId GetTypeId() {
    return TypeIdGetter<T>::id;
  }

public:
  class MismatchError final : std::exception {};

  Value();
  explicit Value(Int value);
  explicit Value(Float value);
  ~Value() = default;

  [[nodiscard]] TypeId getType() const noexcept;
  [[nodiscard]] bool sameType(TypeId type) const noexcept;
  template <ValueAlt T>
  [[nodiscard]] bool sameType() const noexcept;

  template <ValueAlt T>
  T read() const;

  template <ValueAlt T>
  void write(T value);

  template <ValueAlt T>
  void overwrite(T value);

  Variant read(TypeId typeId);
  void write(TypeId typeId, Variant value);
  void overwrite(TypeId typeId, Variant value);

private:
  template <ValueAlt T>
  void validateType() const;

  void validateType(TypeId typeId) const;

  template <ValueAlt T>
  T &getValueRef();

  template <ValueAlt T>
  T const &getValueRef() const;

  TypeId m_type;
  Union m_value;
};

template <>
struct Value::TypeIdGetter<Int> {
  static constexpr const TypeId id = TypeId::INT;
};
template <>
struct Value::TypeIdGetter<Float> {
  static constexpr const TypeId id = TypeId::FLOAT;
};

inline Value::TypeId Value::getType() const noexcept {
  return m_type;
}
inline bool Value::sameType(TypeId type) const noexcept {
  return type == m_type;
}

template <ValueAlt T>
inline bool Value::sameType() const noexcept {
  return GetTypeId<T>() == m_type;
}

template <ValueAlt T>
T Value::read() const {
  this->validateType<T>();
  return this->getValueRef<T>();
}

template <ValueAlt T>
void Value::write(T value) {
  this->validateType<T>();
  this->getValueRef<T>() = value;
}

template <ValueAlt T>
void Value::overwrite(T value) {
  this->m_type = GetTypeId<T>();
  this->getValueRef<T>() = value;
}
template <ValueAlt T>
void Value::validateType() const {
  if (!this->sameType<T>()) {
    throw MismatchError();
  }
}

inline void Value::validateType(TypeId typeId) const {
  if (this->sameType(typeId)) {
    throw MismatchError();
  }
}

template <>
inline Int &Value::getValueRef<Int>() {
  return m_value.i;
}

template <>
inline Float &Value::getValueRef<Float>() {
  return m_value.f;
}

template <>
inline Int const &Value::getValueRef<Int>() const {
  return m_value.i;
}

template <>
inline Float const &Value::getValueRef<Float>() const {
  return m_value.f;
}

} // namespace pvm
