#include "common/value.hpp"

namespace pvm {

Value::Value() : m_type(TypeId::UNDEFINED), m_value() {}

Value::Value(Int value) : m_type(TypeId::INT), m_value() { m_value.i = value; }

Value::Value(Float value) : m_type(TypeId::FLOAT), m_value() { m_value.f = value; }

Value::Variant Value::read(TypeId typeId) {
  this->validateType(typeId);
  switch (typeId) {
  case TypeId::INT:
    return Variant(m_value.i);
  case TypeId::FLOAT:
    return Variant(m_value.f);
  default:
    assert(false);
    break;
  }
}
inline void Value::write(TypeId typeId, Variant value) {
  this->validateType(typeId);
  switch (typeId) {
  case TypeId::INT:
    m_value.i = std::get<Int>(value);
    break;
  case TypeId::FLOAT:
    m_value.f = std::get<Float>(value);
    break;
  default:
    assert(false);
    break;
  }
}
inline void Value::overwrite(TypeId typeId, Variant value) {
  switch (typeId) {
  case TypeId::INT:
    m_type = TypeId::INT;
    m_value.i = std::get<Int>(value);
    break;
  case TypeId::FLOAT:
    m_type = TypeId::FLOAT;
    m_value.f = std::get<Float>(value);
    break;
  default:
    assert(false);
    break;
  }
}

} // namespace pvm
