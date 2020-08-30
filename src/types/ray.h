#pragma once

#include "vec.h"

class ray
{
public:
    constexpr ray() {};
    constexpr ray(const point3& origin, const vec3& direction) :
        o(origin), d(direction) {};

    constexpr point3 at(double t) const
    {
        return o + t*d;
    }

    point3 o;
    vec3   d;
};