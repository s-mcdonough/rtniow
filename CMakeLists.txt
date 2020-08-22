cmake_minimum_required(VERSION 3.17)
project(rtniow
        VERSION 0.0.1
        LANGUAGES CXX)

# Flags for all targets in this project
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/W4>"
                    "$<$<CXX_COMPILER_ID:MSVC>:/WX>" 
                    "$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall>"
                    "$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wextra>"
                    "$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-pedantic>"
                    "$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Werror>")


# Used to download and build external cmake projects
include(FetchContent)

# Enable CMake's built in testing framework
include(CTest)
enable_testing()        

add_subdirectory(src)
add_subdirectory(test)