# RTNIOW
Ray Tracing (Not) in One Weekend

## Motivation
I've always had an interest in ray tracing and rendering. I decided to take the plunge once I 
heard about [Ray Tracing In One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html).
In addition to ray tracing, I am leveraging some of the most modern c++ features (c++17-20) to push
as much as possible into the compilation step (constexpr, std::algorithm) and provide compile time 
diagnostics (SFINAE, concepts). 

## Requirements
RTNIOW aims to be as lightweight as possible, but there are some dependencies:
   1. git (for CMake FetchContent)
   2. CMake (>=3.16)
   3. A modern c++ compiler (clang-10, gcc-10 or greater) with concept support
    
This project provides a dockerfile to build this project if your system does not 
have a modern enough compiler.

## Dependencies
RTNIOW uses Catch2 as its unit test framework. Catch2 is fetched by CMake at configuration time.

## Building 
RTNIOW uses cmake  maximum flexibility 
To build the binaries:
   1. Make a build directory, preferably outside the source path.
   2. Configure and generate the build system using cmake, by pointing it to the top level source directory. `cmake <any-options-here> <path-to-src-root>`
   3. Build the project using the selected build system, shown here for Unix Makefiles. `make render` builds the runtime rendering engine, `make ut` builds unit tests, `make test` builds and runs the unit tests with CTest. `make all` builds almost everything. 
      * The compile time rendering is specifically left out of all as it can be quite expensive depending on the frame size, scene, etc. To build the compile time renderer use `make render_ct`. 
      * Compile time unit testing (using `static_assert`) can be run with `make ut_ct`.  
