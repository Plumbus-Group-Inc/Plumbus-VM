#pragma once

#include "common/config.hpp"
#include <cassert>
#include <exception>
#include <stdexcept>
#include <variant>

namespace pvm {

template <typename T>
concept ValueT = std::is_same_v<T, Int> || std::is_same_v<T, Float>;

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
  template <ValueT T> struct TypeIdGetter;

public:
  template <ValueT T> static constexpr TypeId GetTypeId() { return TypeIdGetter<T>::id; }

public:
  class MismatchError final : std::exception {};

  Value();
  explicit Value(Int value);
  explicit Value(Float value);
  ~Value() = default;

  [[nodiscard]] TypeId getType() const noexcept;
  [[nodiscard]] bool sameType(TypeId type) const noexcept;
  template <ValueT T> [[nodiscard]] bool sameType() const noexcept;

  template <ValueT T> T read() const;

  template <ValueT T> void write(T value);

  template <ValueT T> void overwrite(T value);

  Variant read(TypeId typeId);
  void write(TypeId typeId, Variant value);
  void overwrite(TypeId typeId, Variant value);

private:
  template <ValueT T> void validateType() const;

  void validateType(TypeId typeId) const;

  template <ValueT T> T &getValueRef();

  template <ValueT T> T const &getValueRef() const;

  TypeId m_type;
  Union m_value;
};

template <> struct Value::TypeIdGetter<Int> {
  static constexpr const TypeId id = TypeId::INT;
};
template <> struct Value::TypeIdGetter<Float> {
  static constexpr const TypeId id = TypeId::FLOAT;
};

inline Value::TypeId Value::getType() const noexcept { return m_type; }
inline bool Value::sameType(TypeId type) const noexcept { return type == m_type; }

template <ValueT T> inline bool Value::sameType() const noexcept {
  return GetTypeId<T>() == m_type;
}

template <ValueT T> T Value::read() const {
  this->validateType<T>();
  return this->getValueRef<T>();
}

template <ValueT T> void Value::write(T value) {
  this->validateType<T>();
  this->getValueRef<T>() = value;
}

template <ValueT T> void Value::overwrite(T value) {
  this->m_type = GetTypeId<T>();
  this->getValueRef<T>() = value;
}
template <ValueT T> void Value::validateType() const {
  if (!this->sameType<T>()) {
    throw MismatchError();
  }
}

inline void Value::validateType(TypeId typeId) const {
  if (this->sameType(typeId)) {
    throw MismatchError();
  }
}

template <> inline Int &Value::getValueRef<Int>() { return m_value.i; }

template <> inline Float &Value::getValueRef<Float>() { return m_value.f; }

template <> inline Int const &Value::getValueRef<Int>() const { return m_value.i; }

template <> inline Float const &Value::getValueRef<Float>() const { return m_value.f; }

} // namespace pvm
