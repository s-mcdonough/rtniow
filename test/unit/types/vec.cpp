#include "catch2/catch.hpp"
#include "vec.h"

// Hack to prevent warning about unused variables 
void consume(...){}

TEST_CASE("Vector construction", "[vec]")
{
    constexpr vec<3> a;
    constexpr vec<3> b({0,0,0});
    constexpr vec<2> c({1,0});
    constexpr vec<2> d = c * 2;

    class E { constexpr E(){} };
    class F { constexpr F(){} };

    constexpr vec<3,E> e({1.,2.,3.});

    static_assert(a.length() == 0, "a not length 0");
    static_assert(b.length() == 0, "b not length 0");
    static_assert(d.length() == 2, "b not length 0");
    CHECK(a.length() == 0);
    CHECK(b.length() == 0);
    CHECK(d.length() == 2);

    unit_vector(c);

    consume(a,b,c);
}