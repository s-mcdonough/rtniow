#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <initializer_list>
#include <numeric>

#include "utils.h" // for constexpr sqrt()

/**
 * An arbitry dimensional constexpr vector.
 * 
 * This vector allows for further specalization to prevent 
 * mixing dissimilar types, such as color and position.
 */
template<int N, typename Tp = void, std::floating_point V = double>
class vec
{
public:
    using ValueType = V;
    using SelfType = vec<N,Tp,V>;
    using ArrayType = std::array<ValueType, N>;

    constexpr vec() noexcept : e{} {}
    constexpr vec(ArrayType arr) noexcept : e(arr) {}

    constexpr ValueType operator[] (size_t i) const { return e[i]; }
    constexpr ValueType& operator[] (size_t i) { return e[i]; }

    constexpr SelfType operator-() const { return SelfType((*this * -1).e); }

    constexpr double x() const { return e[0]; }
    constexpr double y() const { return e[1]; }
    constexpr double z() const { return e[2]; }

    constexpr bool operator == (const SelfType& other) const
    {
        return std::equal(cbegin(),cend(),other.cbegin());
    }

    constexpr bool operator != (const SelfType& other) const
    {
        return !(*this == other);
    }

    constexpr size_t size() const noexcept
    {
        return N;
    }

    constexpr ValueType length() const
    {
        // std::sqrt is not a constexpr
        return utils::sqrt(length_squared());
    }
    
    constexpr ValueType length_squared() const
    {
        return dot(*this, *this);
    }

    // Arithmatic operators

    // SelfType& operator += (const SelfType& v)
    constexpr SelfType& operator += (const auto& v)
    {
        // for (auto i=0; i<e.size(); ++i) e[i] += v[i];
        // return *this;
        this->e = (*this + v).e;
        return *this;
    }

    constexpr SelfType& operator *= (const auto& v)
    {
        this->e = (*this * v).e;
        return *this;
    }

    constexpr SelfType& operator /= (const ValueType v)
    {
        *this *= (1/v);
        return *this;
    }

    // Iterators
    constexpr auto cbegin() const noexcept
    {
        return e.cbegin();
    }

    constexpr auto cend() const noexcept
    {
        return e.cend();
    }

protected:
    std::array<ValueType, N> e;
};

namespace detail
{
    // Core generator to aid in type deduction and implement binary
    // operations between vectors of equivalent types.
    template<int N, typename Tp, class BinOp>
    constexpr vec<N,Tp> BinaryOp(const vec<N,Tp>&a, const vec<N,Tp>& b, BinOp op)
    {
        std::array<typename vec<N,Tp>::ValueType, N> out;
        std::transform(a.cbegin(), a.cend(), b.cbegin(), out.begin(), op);
        return vec<N,Tp>(out);
    }

    // Core generator to aid in type deduction and implement binary
    // operations between a vector and scalar.
    template<int N, typename Tp, class BinOp>
    constexpr vec<N,Tp> BinaryOp(const vec<N,Tp>&a, typename vec<N,Tp>::ValueType b, BinOp op)
    {
        std::array<typename vec<N,Tp>::ValueType, N> out;
        auto clos = [=](auto a_) { return op(a_, b); };
        std::transform(a.cbegin(), a.cend(), out.begin(), clos);
        return vec<N,Tp>(out);
    }

    template<int N, typename Tp, class BinOp>
    constexpr vec<N,Tp> BinaryOp(typename vec<N,Tp>::ValueType b, const vec<N,Tp>&a, BinOp op)
    {
        return BinaryOp(a, b, op);
    }
}

constexpr auto operator+ (const auto& a, const auto& b)
{
    return detail::BinaryOp(a,b,[](auto x, auto y) { return x+y; });
}

constexpr auto operator- (const auto& a, const auto& b)
{
    return detail::BinaryOp(a,b,[](auto x, auto y) { return x-y; });
}

constexpr auto operator* (const auto& a, const auto& b)
{
    return detail::BinaryOp(a,b,[](auto x, auto y) { return x*y; });
}

constexpr auto operator/ (const auto& a, const auto& b)
{
    return detail::BinaryOp(a,b,[](auto x, auto y) { return x/y; });
}

template<int N, typename Tp>
constexpr auto dot(const vec<N,Tp>& a, const vec<N,Tp>& b)
{
    return std::inner_product(a.cbegin(), a.cend(), b.cbegin(), 0);
}

// Cross product only really makes sense in three dimensions
template<typename T>
constexpr auto cross(const vec<3,T>& u, const vec<3,T>& v)
{
    return vec<3,T>({u[1] * v[2] - u[2] * v[1],
                     u[2] * v[0] - u[0] * v[2],
                     u[0] * v[1] - u[1] * v[0]});
}

template<int N, typename Tp>
constexpr vec<N,Tp> unit_vector(const vec<N,Tp>& v)
{
    return v / v.length();
}

template<int N, typename Tp, std::floating_point V>
inline std::ostream& operator << (std::ostream& os, const vec<N,Tp,V>& v)
{
    for (size_t i=0; i<v.size(); ++i)
    {
        os << v[i];
        if (i < v.size()-1) os << " ";
    }
    return os;
}

// Dont expose these types in the main namespace as they are only used
// to differentiate the proceeding types.
namespace detail
{
    class color {};
}

// Specializations: this prevents conversions between position and color
// (what would that mean?).
using point3 = vec<3>;
using vec3   = vec<3>;