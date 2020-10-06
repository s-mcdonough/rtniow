#include <catch2/catch.hpp>
#include "ray.h"
#include "types/to_string.h"

TEST_CASE("Ray construction", "[ray]")
{
    point3 orig({0,0,0});
    vec3   dir({1,1,1});
    ray r(orig, dir);
    CHECK(r.at(2) == point3({2,2,2}));
}