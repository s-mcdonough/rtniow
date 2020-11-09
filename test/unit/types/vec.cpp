#include "catch2/catch.hpp"
#include "vec.h"

// Hack to prevent warning about unused variables 
void consume(...){}

// Static and runtime checking of an expression E
#define SR_REQUIRE(E) \
    static_assert((E)); \
    REQUIRE((E));

#define SR_CHECK(E) \
    static_assert((E)); \
    CHECK((E));

TEST_CASE("Vector construction", "[vec]")
{
    constexpr vec<3> a;
    constexpr vec<3> b({0,0,0});
    constexpr vec<2> c({1,0});
    constexpr vec<2> d = c;

    class E { constexpr E(){} };
    class F { constexpr F(){} };

    constexpr vec<3,E> e({1.,2.,3.});

    SR_CHECK(a.length() == 0);
    SR_CHECK(b.length() == 0);
    SR_CHECK(c.length() == 1);
    SR_CHECK(c == d);

    consume(a,b,c);
}

TEST_CASE("Vector element-wise math", "[vec],[math]")
{
    constexpr vec3 a({1,1,1});
    constexpr vec3 b({1,2,3});
    constexpr vec3 c({10,1,.1});

    SECTION("Addition")
    {
        SR_REQUIRE((a + a) == vec3({2,2,2}));
        SR_REQUIRE((a + b) == (b + a));
    }

    SECTION("Subtraction")
    {
        SR_REQUIRE((a - a) == vec3());
        SR_REQUIRE((a - b) == -(b - a)); // Anticommutativity
    }

    SECTION("Multiplication")
    {
        SR_REQUIRE(2*a == (a + a));
        SR_REQUIRE(a*b == b);

        SR_REQUIRE(1*a == a); // Identity
        SR_REQUIRE(0*a == vec3()); // Property of zero
        SR_REQUIRE(-1*a == -a); // Negation
        SR_REQUIRE(a*b == b*a); // Commutativity
        SR_REQUIRE((a * b) * c == a * (b * c)); // Association
        SR_REQUIRE(a * (b + c) == (a * b) + (a * c)); // Distribution

        REQUIRE([&]()->bool {
            vec3 a_nc(a); // a non-constant expression version of a, since we are modifying state 
            a_nc *= 2;
            return a_nc == vec3({2,2,2});
        }());
    }

    // TODO: Division
}

TEST_CASE("Vector math", "[vec],[math]")
{
    constexpr vec3 a({3,0,0});
    constexpr vec3 b({0,1,0});
    constexpr vec3 c({5,3,99});
    constexpr vec3 d({1,2,3});

    SECTION("Dot-product")
    {
        SR_REQUIRE(dot(a,b) == 0); // Orthogonal vectors have a dot product of 0
        SR_REQUIRE(std::sqrt(dot(a,a)) == a.length());
        constexpr double s = 5;
        SR_REQUIRE(dot((s*c),d) == s*dot(c,d));
    }

    SECTION("Cross-product")
    {
        Catch::StringMaker<double>::precision = 15;
        using std::pow;
        SR_REQUIRE(cross(a,a) == vec3()); 
        SR_REQUIRE(cross(a,b).length() == (a.length() * b.length())); // as sin(pi/2) == 1 for this case
        REQUIRE(pow(cross(c,d).length(),2) == // Validate cross product using the Grahm identity
            Approx(pow(c.length(),2) * pow(d.length(),2) - pow(dot(c,d),2))); // TODO: This is not *exactly* equal, investigate
    }
}
