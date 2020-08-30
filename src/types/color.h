#pragma once

#include <iostream>
#include "vec.h"

class color : public vec<3, detail::color>
{
public:
    constexpr double r() const { return e[0]; }
    constexpr double g() const { return e[1]; }
    constexpr double b() const { return e[2]; }
};

std::ostream& write_color(std::ostream& out, const color& pixel) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel.r()) << ' '
        << static_cast<int>(255.999 * pixel.g()) << ' '
        << static_cast<int>(255.999 * pixel.b()) << '\n';
    return out;
}

inline std::ostream& operator<< (std::ostream& os, const color& p)
{
    return write_color(os, p);
}
