#pragma once

#include "common/config.hpp"
#include <cassert>
#include <exception>
#include <stdexcept>
#include <variant>

namespace pvm {

template <typename RequetedType>
concept ValuePayload =
    std::is_same_v<RequetedType, Int> || std::is_same_v<RequetedType, Float>;

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
  template <ValuePayload RequestedType> struct TypeIdGetter;

public:
  template <ValuePayload RequetedType> static constexpr TypeId GetTypeId() {
    return TypeIdGetter<RequetedType>::id;
  }

public:
  class MismatchError final : std::exception {};

  Value();
  explicit Value(Int value);
  explicit Value(Float value);
  ~Value() = default;

  [[nodiscard]] TypeId getType() const noexcept;
  [[nodiscard]] bool sameType(TypeId type) const noexcept;
  template <ValuePayload RequetedType> [[nodiscard]] bool sameType() const noexcept;

  template <ValuePayload RequetedType> RequetedType read() const;

  template <ValuePayload RequetedType> void write(RequetedType value);

  template <ValuePayload RequetedType> void overwrite(RequetedType value);

  Variant read(TypeId typeId);
  void write(TypeId typeId, Variant value);
  void overwrite(TypeId typeId, Variant value);

private:
  template <ValuePayload RequetedType> void validateType() const;

  void validateType(TypeId typeId) const;

  template <ValuePayload RequetedType> RequetedType &getValueRef();

  template <ValuePayload RequetedType> RequetedType const &getValueRef() const;

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

template <ValuePayload RequetedType> inline bool Value::sameType() const noexcept {
  return GetTypeId<RequetedType>() == m_type;
}

template <ValuePayload RequetedType> RequetedType Value::read() const {
  this->validateType<RequetedType>();
  return this->getValueRef<RequetedType>();
}

template <ValuePayload RequetedType> void Value::write(RequetedType value) {
  this->validateType<RequetedType>();
  this->getValueRef<RequetedType>() = value;
}

template <ValuePayload RequetedType> void Value::overwrite(RequetedType value) {
  this->m_type = GetTypeId<RequetedType>();
  this->getValueRef<RequetedType>() = value;
}
template <ValuePayload RequetedType> void Value::validateType() const {
  if (!this->sameType<RequetedType>()) {
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
