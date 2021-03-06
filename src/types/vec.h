#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <initializer_list>
#include <numeric>

#include "utils.h" // for constexpr sqrt() and arithmatic

#define VEC_ENABLE_IF_3_TUPLE(LENGTH, T) \
    template<int L=LENGTH> \
    typename std::enable_if_t<L==3, T>

/**
 * An arbitry dimensional constexpr vector.
 * 
 * This vector allows for further specalization to prevent 
 * mixing dissimilar types, such as color and position.
 * 
 * @tparam N the size of the vector
 * @tparam Tp template specalization to enforce consistency, e.g.
 * vec<3,color> cannot be added to vec<3> by default.
 * @tparam V the underlying arithmatic type of scalar components
 */
template<int N, typename Tp = void, utils::arithmatic V = double>
class vec
{
public:
    using ValueType = V;
    using SelfType = vec<N,Tp,V>;
    using ArrayType = std::array<ValueType, N>;

    constexpr vec() noexcept : e{} {}
    constexpr vec(const SelfType& other) noexcept : e(other.e) {}
    constexpr vec(ArrayType arr) noexcept : e(arr) {}

    constexpr SelfType& operator= (const SelfType& other) noexcept
    {
        e = other.e;
        return *this;
    }

    constexpr ValueType operator[] (size_t i) const { return e[i]; }
    constexpr ValueType& operator[] (size_t i) { return e[i]; }

    // Negation operator
    constexpr SelfType operator-() const 
    { 
        static_assert(!std::is_unsigned_v<V>, "Cannot negate an unsigned type!");
        return SelfType((*this * -1).e); 
    }

    // Named accessors if vector is a 3-tuple
    VEC_ENABLE_IF_3_TUPLE(N,const ValueType&)
    constexpr x() const { return e[0]; }
    VEC_ENABLE_IF_3_TUPLE(N,const ValueType&)
    constexpr y() const { return e[1]; }
    VEC_ENABLE_IF_3_TUPLE(N,const ValueType&)
    constexpr z() const { return e[2]; }

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
        return utils::sqrt(length_squared());
    }
    
    constexpr ValueType length_squared() const
    {
        return dot(*this, *this);
    }

    // Arithmetic operators

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
        *this *= (1./v);
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
    ArrayType e;
};

namespace detail
{
    // Core generator to aid in type deduction and implement unary
    // operations on vectors.
    template<int N, typename Tp, class UnOp>
    constexpr vec<N,Tp> UnaryOp(const vec<N,Tp>&a, UnOp op)
    {
        std::array<typename vec<N,Tp>::ValueType, N> out;
        std::transform(a.cbegin(), a.cend(), out.begin(), op);
        return vec<N,Tp>(out);
    }

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

constexpr auto sqrt(const auto& a)
{
    return detail::UnaryOp(a, [](auto x) { return utils::sqrt(x); });
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

template<int N, typename Tp, std::floating_point V>
constexpr vec<N,Tp,V> unit_vector(const vec<N,Tp,V>& v)
{
    return v / (v.length());
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

// Specializations: this prevents conversions between position and color
// (what would that mean?).
using point3 = vec<3>;
using vec3   = vec<3>;