#pragma once

#include <iostream>
#include "vec.h"

namespace detail
{
    class color {};
}

using color = vec<3, detail::color>;

std::ostream& write_color(std::ostream& out, const color& pixel) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(254.999 * pixel[0]) << ' '
        << static_cast<int>(254.999 * pixel[1]) << ' '
        << static_cast<int>(254.999 * pixel[2]) << '\n';
    return out;
}

inline std::ostream& operator<< (std::ostream& os, const color& p)
{
    return write_color(os, p);
}
