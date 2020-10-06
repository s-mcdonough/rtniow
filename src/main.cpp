#include <cstdlib>
#include <iostream>

#include "color.h"
#include "vec.h"
#include "ray.h"

constexpr color ray_color(const ray& r)
{
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1);
    return (1-t)*color({1, 1, 1}) + t*color({.5, .7, 1});
}

int main()
{
    // img
    constexpr auto aspect_ratio = 16. / 10;
    constexpr int img_width = 400;
    constexpr int img_height = img_width / aspect_ratio;

    // camera
    constexpr auto viewport_height = 2.;
    constexpr auto viewport_width = aspect_ratio *  viewport_height;
    constexpr auto focal_len = 2.;

    constexpr vec3 origin({0,0,0});
    constexpr vec3 horizontal({viewport_width,0,0});
    constexpr vec3 vertical({0,viewport_height,0});
    constexpr auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3({0,0,focal_len});

    // render
    using std::cout, std::cerr;
    cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";

    for(auto j=img_height; j>=0; --j)
    {
        cerr << "\rScanlines remaining: " << j << std::flush;
        for(auto i=0; i<img_width; ++i)
        {
            const auto u = static_cast<double>(i) / (img_width-1);
            const auto v = static_cast<double>(j) / (img_height-1);
            const ray r(origin, lower_left_corner + (u*horizontal) + (v*vertical) - origin);
            cout << ray_color(r);
        } 
    }

    cerr << "\nDone!\n";
    std::exit(EXIT_SUCCESS);
}