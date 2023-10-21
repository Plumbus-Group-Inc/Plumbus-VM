#pragma once

#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>

namespace pvm {

template<typename TheType, typename... Types>
constexpr auto VariadicHasType = (std::is_same_v<TheType, Types> || ...);

template<typename TheType>
constexpr auto VariadicHasType<TheType> = false;

template<typename FirstType, typename... OtherTypes>
inline constexpr auto VariadicUniqImpl = 
    !VariadicHasType<FirstType, OtherTypes...> &&
    VariadicUniqImpl<OtherTypes...>;

template<typename FirstType>
inline constexpr auto VariadicUniqImpl<FirstType> = true;

template<typename... Types>
inline constexpr auto VariadicUniq = VariadicUniqImpl<Types...>;

template<typename... Types>
concept UniqTypesList = VariadicUniq<Types...>;

template<typename TheType, typename... Types>
concept ListedType = VariadicHasType<TheType, Types...>;

template<typename FirstType, typename... Types>
std::type_info const& VariadicNthInfoImpl(std::size_t n) {
    if constexpr(sizeof...(Types) == 0) {
        if (n == 0) {
            return typeid(FirstType);
        }
        std::terminate();
    } else {
        return n == 0 ? typeid(FirstType) : VariadicNthInfoImpl<Types...>(n - 1);
    }
}

template<typename... Types>
std::type_info const& VariadicNthInfo(std::size_t n) {
    static_assert(sizeof...(Types) != 0);
    if(n >= sizeof...(Types)) {
        throw std::out_of_range("out of variadic arguments list");
    }

    return VariadicNthInfoImpl<Types...>(n);
}

} // namespace pvm

