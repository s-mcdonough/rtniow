#include <cstdlib>
#include <iostream>

#include <cassert>

#include "color.h"
#include "vec.h"
#include "ray.h"

// Macro's to force compile time rendering
#ifdef RENDER_COMPILE_TIME
#define RENDER_CONSTEXPR constexpr
#define RENDER_CONTAINER(T,N) std::array<T,N>
#else 
#define RENDER_CONSTEXPR
#define RENDER_CONTAINER(T,N) std::vector<T>
#endif

constexpr bool hit_sphere(const point3& center, double radius, const ray& r) {    
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared(); 
    // auto a = dot(r.direction(), r.direction());
    auto b = 2. * dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

constexpr color ray_color(const ray& r)
{
    // if (hit_sphere(point3({0,0,-1}), 0.1, r))
    //     return color({1, 0, 0});

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1);
    return (1-t)*color({1, 1, 1}) + t*color({.5, .7, 1});
}

// TODO: Cleanup
template<int Sz>
using CompileTimeType = typename std::array<color, Sz>;
using RunTimeType = std::vector<color>;
template<int N>
constexpr typename std::conditional_t<N<15000, CompileTimeType<N>, RunTimeType>
container_constructor()
{
    if constexpr (N<15000) return CompileTimeType<N>();
    else return RunTimeType(N);
}

int main()
{
    // img
    constexpr auto aspect_ratio = 16. / 9;
    constexpr int img_width = 50;
    constexpr int img_height = img_width / aspect_ratio;

    // camera
    constexpr auto viewport_height = 2.;
    constexpr auto viewport_width = aspect_ratio *  viewport_height;
    constexpr auto focal_len = 1.;

    constexpr vec3 origin({0,0,0});
    constexpr vec3 horizontal({viewport_width,0,0});
    constexpr vec3 vertical({0,viewport_height,0});
    constexpr auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3({0,0,focal_len});

    constexpr auto total = img_height * img_width;
    constexpr auto llc_minus_o = lower_left_corner - origin;

    // Setup the raytracer functor
    constexpr auto lam = [=](int height, const int width) -> auto {
        if (!std::is_constant_evaluated()) std::cerr << "Rendering at runtime" << std::endl;
        
        auto container = container_constructor<total>();

        int j = height - 1;
        int idx = 0;
        while (j >= 0)
        {
            int i = 0;
            const auto v = static_cast<double>(j) / (img_height-1);
            const auto partial = llc_minus_o + (v*vertical);
            while(i < width)
            {
                const auto u = static_cast<double>(i) / (width-1);
                const ray r(origin, partial + (u*horizontal));
                color c = ray_color(r);
                container[idx++] = (c);
                ++i;
            }
            --j;
        }
        return container;
    };

    // Run the raytracer, this produces a compile time result
    // if the return type is constexpr.
    RENDER_CONSTEXPR auto out = lam(img_height, img_width);

    // Print results 
    using std::cout, std::cerr;
    cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";
    auto tic = total;
    for (const auto& c : out)
    {
        if ((tic % 100) == 0) cerr << "\rPixels remaining: " << tic << std::flush;
        cout << c;
        --tic;
    }

    cerr << "\nDone!\n";
    std::exit(EXIT_SUCCESS);
}