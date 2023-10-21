#pragma once

#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include <variant>

#include "common/template-magic.hpp"

namespace pvm {

class ValueMismatchError : public std::runtime_error {
public:
    ValueMismatchError(std::type_info const& requestedType, 
        std::type_info const& currentType);

    std::type_info const& requestedType;
    std::type_info const& currentType;

private:
    static std::string formatMessage(std::type_info const& requetedType,
        std::type_info const& currentType);
};

template<typename... Types>
requires UniqTypesList<Types...>
class ValueImpl final {
public:
    using ValueVariant = std::variant<Types...>;

    template<typename Type>
    requires ListedType<Type, Types...>
    [[nodiscard]] Type read() const {
        auto pvalue = std::get_if<Type>(&m_data);
        if(pvalue == nullptr) {
            throw ValueMismatchError(typeid(Type), this->currentTypeinfo());
        }

        return *pvalue;
    }

    template<typename Type>
    requires ListedType<Type, Types...>
    void write(Type value) {
        if(!this->holds<Type>()) {
            throw ValueMismatchError(typeid(Type), this->currentTypeinfo());
        }

        m_data = value;
    }

    template<typename Type>
    requires ListedType<Type, Types...>
    void overwrite(Type value) {
        m_data = value;
    }

    template<typename Type>
    requires ListedType<Type, Types...>
    [[nodiscard]] bool holds() const noexcept {
        return std::holds_alternative<Type>(m_data);
    }

private:
    [[nodiscard]] std::type_info const& currentTypeinfo() const noexcept {
        return VariadicNthInfo<Types...>(m_data.index());
    }

    ValueVariant m_data;
};

template<typename Type>
concept ValueT = VariadicHasType<Type, int, float>;

using Value = ValueImpl<int, float>;

} // namespace pvm
