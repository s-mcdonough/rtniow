#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <initializer_list>

#include "utils.h" // for constexpr sqrt()

/**
 * An arbitry dimensional constexpr vector.
 * 
 * This vector allows for further specalization to prevent 
 * mixing dissimilar types, such as color and position.
 */
template<int N, typename Tp = void>
class vec
{
public:
    using ValueType = double;
    using SelfType = vec<N,Tp>;
    using ArrayType = std::array<ValueType, N>;

    constexpr vec() : e{} {}
    constexpr vec(ArrayType arr) : e(arr) {}

    constexpr ValueType operator[] (size_t i) const { return e[i]; }
    constexpr ValueType& operator[] (size_t i) { return e[i]; }

    constexpr ValueType length() const
    {
        // std::sqrt is not a constexpr
        return utils::sqrt(length_squared());
    }
    
    constexpr ValueType length_squared() const
    {
        return std::inner_product(e.begin(), e.end(), e.begin(), 0);
    }

    // Arithmatic operators

    SelfType& operator += (const SelfType& v)
    {
        for (auto i=0; i<e.size(); ++i) e[i] += v[i];
        return *this;
    }

    SelfType& operator *= (ValueType v)
    {
        self_modify([=](auto x) -> ValueType { return x * v; });
        return *this;
    }

    SelfType& operator /= (ValueType v)
    {
        return *this *= (1/v);
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

private:
    // Helper to modify all elements of the vector
    template<class UnaryOp>
    void self_modify(UnaryOp op)
    {
        std::transform(e.begin(), e.end(), e.begin(), op);
    }

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
    return std::inner_product(a.begin(), a.end(), b.begin(), 0);
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

// Dont expose these types in the main namespace as they are only used
// to differentiate the proceeding types.
namespace detail
{
    class point {};
    class color {};
}

// Specializations: this prevents conversions between position and color
// (what would that mean?).
using point3 = vec<3,detail::point>;
using color  = vec<3,detail::color>;