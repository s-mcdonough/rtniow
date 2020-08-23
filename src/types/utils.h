#pragma once

#include <limits>
#include <type_traits>

namespace utils
{
    template < class T >
    concept arithmatic = std::is_arithmetic_v<T>;

    namespace detail
    {
        template<arithmatic T>
        constexpr double sqrtNewtonRaphson(T x, T curr, double prev)
        {
            return curr == prev
                ? curr
                : sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
        }
    }

    template<arithmatic T>
    constexpr double sqrt(T x)
    {
        return (x >= 0 && x < std::numeric_limits<T>::infinity())
            ? detail::sqrtNewtonRaphson(x, x, 0)
            : std::numeric_limits<T>::quiet_NaN();
    }
}