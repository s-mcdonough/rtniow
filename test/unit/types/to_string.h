#pragma once

#include <catch2/catch.hpp>
#include <vec.h>
#include <sstream>

namespace Catch
{
    template<>
    struct StringMaker<vec3> {
        static std::string convert( vec3 const& value ) {
            std::stringstream ss;
            ss << "{" << value << "}";
            return ss.str();
        }
    };
}