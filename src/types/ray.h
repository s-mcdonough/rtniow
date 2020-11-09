#pragma once

#include "vec.h"

class ray
{
public:
    constexpr ray() noexcept {};
    constexpr ray(const point3& origin, const vec3& direction) noexcept :
        o(origin), d(direction) {};

    constexpr point3 origin() const { return o; }
    constexpr vec3   direction() const { return d; }

    constexpr point3 at(double t) const
    {
        return o + (t*d);
    }

private:
    point3 o;
    vec3   d;
};