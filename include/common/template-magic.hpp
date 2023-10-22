#pragma once

#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>

namespace pvm::variadic {

template<typename TheType, typename... Types>
constexpr auto Contains = (std::is_same_v<TheType, Types> || ...);

template<typename TheType>
constexpr auto Contains<TheType> = false;

template<typename... Types>
inline constexpr auto HasDuplicates = false;

template<typename Head, typename... Tail>
inline constexpr auto HasDuplicates<Head, Tail...> = 
    Contains<Head, Tail...> || HasDuplicates<Tail...>;

template<typename... Types>
concept Unique = !HasDuplicates<Types...>;

template<typename TheType, typename... Types>
concept PresentIn = Contains<TheType, Types...>;

template<size_t N, typename... Types>
struct NthImpl {
    static_assert(N < sizeof...(Types));
};

template<size_t N, typename Head, typename... Tail>
struct NthImpl<N, Head, Tail...> {
    using T = NthImpl<N - 1, Tail...>;
};
template<typename Head, typename... Tail>
struct NthImpl<0, Head, Tail...> {
    using T = Head;
};

template<size_t N, typename... Types>
using Nth = NthImpl<N, Types...>::T;

template<typename FirstType, typename... Types>
std::type_info const& NthTypeInfoImpl(std::size_t n) {
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
std::type_info const& NthTypeInfoImpl(std::size_t n) {
    static_assert(sizeof...(Types) != 0);
    if(n >= sizeof...(Types)) {
        throw std::out_of_range("out of variadic arguments list");
    }

    return VariadicNthInfoImpl<Types...>(n);
}

} // namespace pvm::variadic
